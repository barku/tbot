/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : dwm.cpp                                                       */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Differential Wheel Motion                                     */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#include "config.h"

#ifdef ENABLE_DIFF_WHEEL_MOTION

/******************************************************************************/

#include "dwm.h"
#include <Makeblock.h>

// EncoderMotor address at port 1 = 0x09, at port 2 = 0x0A
MeEncoderMotor emotor_l(0x09, SLOT1);          //  Setup left  encoder motor at slot1
MeEncoderMotor emotor_r(0x09, SLOT2);          //  Setup right encoder motor at slot2

Dwm::Dwm()
{
    ;
}

Dwm::~Dwm()
{
    ;
}

void Dwm::begin(void)
{
    emotor_l.begin();                              // start encoder motor
    emotor_r.begin();                              // start encoder motor
}

/*
boolean leftflag,rightflag;
int minSpeed = 55;
int factor = 23;
*/


/*
int run_speed = 200;

inline boolean emotor_l_run_dist(int16_t dist)
{
    return emotor_l.RunTurns((float)  dist / DWM_WHEEL_MM_PER_MOTOR_TURN, run_speed);
}

inline boolean emotor_r_run_dist(int16_t dist)
{
    return emotor_r.RunTurns((float) -dist / DWM_WHEEL_MM_PER_MOTOR_TURN, run_speed);
}
*/

int16_t Dwm::clipDeg180(int16_t deg)
{
   while (deg >   180) deg -= 360;
   while (deg <= -180) deg += 360;
   return deg;
}

int16_t Dwm::clipDeg360(int16_t deg)
{
   while (deg <    0) deg += 360;
   while (deg >= 360) deg -= 360;
   return deg;
}

void Dwm::stop(void)
{
    emotor_l.RunSpeed(0);
    emotor_r.RunSpeed(0);
}

bool Dwm::moveDist(int16_t distMm, int16_t speed, int16_t angleDeg)
{
    angleDeg = clipDeg180(angleDeg);
    if (angleDeg == 0)
    {
        emotor_l.RunTurns((float) distMm / DWM_WHEEL_MM_PER_MOTOR_TURN, (float) speed);
        emotor_r.RunTurns((float)-distMm / DWM_WHEEL_MM_PER_MOTOR_TURN, (float) speed);
        return true;
    }
    else
    {
        return false;
    }
}
/*
bool Dwm::moveSpeed(int16_t speed, int16_t angleDeg)
{
    angleDeg = clipDeg180(angleDeg);
    if (angleDeg == 0)
    {
        emotor_l.RunSpeed((float)  speed);
        emotor_r.RunSpeed((float) -speed);
        return true;
    }
    else if (angleDeg < 0)
    {
        if (angleDeg >= -90)
        {
            emotor_l.RunSpeed((float)  speed);
            emotor_r.RunSpeed((float) -speed * (1.0 + ((float) (0 - angleDeg) / 90.0)));
        }
        else
        {
           return false;
        }
    }
    else 
    {
        if (angleDeg <= 90)
        {
            emotor_l.RunSpeed((float)  speed * (1.0 + ((float) angleDeg / 90.0)));
            emotor_r.RunSpeed((float) -speed);
        }
        else
        {
           return false;
        }
    }
}
*/
void Dwm::getSpeed(float *lSpeed, float *rSpeed)
{
    *lSpeed = emotor_l.GetCurrentSpeed();
    *rSpeed = emotor_r.GetCurrentSpeed();
}

void Dwm::getPosition(float *lPos, float *rPos)
{
// retry to overcome i2c read fail  
#define NR_OF_RETRIES 3
    float pos;
    uint8_t lRetry, rRetry;

    for (lRetry = 0; lRetry < NR_OF_RETRIES; lRetry++)
    {
       pos = emotor_l.GetCurrentPosition();
       if (pos != 0.0)
          break;
    }
    *lPos = pos;
    
    for (rRetry = 0; rRetry < NR_OF_RETRIES; rRetry++)
    {
       pos = emotor_r.GetCurrentPosition();
       if (pos != 0.0)
          break;
    }
    *rPos = pos;
/*    
    if ((lRetry > 1) || (rRetry > 1))
    {
        Serial.print("lPosRty=");
        Serial.print(lRetry, DEC);
        Serial.print(" rPosRty=");
        Serial.println(rRetry, DEC);
    }
*/    
}

/******************************************************************************/

#endif // #ifdef ENABLE_DIFF_WHEEL_MOTION

/******************************** END OF FILE *********************************/

