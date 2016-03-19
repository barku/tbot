/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : usr.c                                                         */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Ultrasonic Radar                                              */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#include <Makeblock.h>
#include <Arduino.h>

#include "config.h"

#ifdef ENABLE_ULTRASONIC_RADAR

/******************************************************************************/

#include <Servo.h>

#include "usr.h"

#define USR_DEBUG

// servo is connected to ME Orion through a slot on Me RJ25 Adapter

MePort port(PORT_6);

Servo servo;
int16_t servo1Pin =  port.pin1();
int16_t servo2Pin =  port.pin2();

MeUltrasonicSensor ultraSensor(PORT_3); /* Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */

//******************************************************************************

Usr::Usr()
{
    uint8_t deg;
    
    inited      = false;
    servoId        = 0;

    deg = 0 - ((USR_STEP_NR / 2) * USR_STEP_DEG);
    for (uint8_t i = 0; i < USR_STEP_NR; i++)
    {
        degArray[i] = deg;
        deg += USR_STEP_DEG;
    }
}

Usr::~Usr()
{
    inited = false;
}

bool Usr::init(uint8_t _servoId)
{
    if (_servoId == 1)
    {
        servo.attach(servo1Pin);
    }
    else if (_servoId == 2)
    {
        servo.attach(servo2Pin);
    }
    else
    {
        return false;  
    }

    inited = true;
    return true;
}

bool Usr::setAngleDeg(int8_t _servoDeg)
{
    uint8_t servoVal;
    double dist;

    if (_servoDeg > USR_DEG_MAX)
        _servoDeg = USR_DEG_MAX;
    if (_servoDeg < USR_DEG_MIN)
        _servoDeg = USR_DEG_MIN;
    
    servoVal = (0 - _servoDeg) + 90 + USR_CENTER_OFFSET_DEG; 
    servo.write(servoVal);
    
    return true;
}

int8_t Usr::getAngleDeg(void)
{
    uint8_t servoDeg;

    servoDeg = 0 - ((int16_t) servo.read() - 90);

    return (int8_t) servoDeg;
}

double Usr::getDist(void)
{
    return ultraSensor.distanceCm();
}

bool Usr::startScan(void)
{
    radarRunning = true;
    radarDone = false;
    retryCount = 0;
    
    if ((stepReverse == false) && (stepCount >= USR_STEP_NR - 1))
    {
        stepCount = USR_STEP_NR - 1;    
        stepReverse = true;
    }
    else
    {
        stepCount = 0;    
        stepReverse = false;
    }
    
    stepMoving = false;
    stepTimeMs = 0;
    moveTimeMs = 500;

    return true;
}

bool Usr::updateScan(long cms)
{
    double dist;
    
    // if radar is not running,
    if (radarRunning != true)
        return false;

    // if radar just set the first step position and stepTimeMs
    if (stepTimeMs == 0)
    {
        setAngleDeg(degArray[stepCount]);
        stepTimeMs = cms + moveTimeMs;
        return true;
    }

    // if time is not yet there, return
    if (cms < stepTimeMs)
        return false;

    stepDelayMs = cms - stepTimeMs; // the time delay for expected service routine in ms    
    dist = getDist();
    
// MCDEBUG++    
#if 1
    if (dist < 1)
    {
        // not sure if this is a bug, when USR is returning a very small value (0.0 ~ 0.1), 
        // it usually keeps returning small value if we keep getting readings,
        // by postponding the reading for a few ms, it seems to be working with high success.
        if (retryCount < USR_RETRY_MAX)
        {
            stepTimeMs = cms + 10;
            retryCount++;
            return true;
        }
    }
#endif
//MCDEBUG--       

    if (retryCount >= USR_RETRY_MAX)
    {
        distArray[stepCount] = 0;
    }
    else
    {
        distArray[stepCount] = (uint16_t) (dist * 10);
    }

#ifdef USR_DEBUG
    Serial.print(millis(), DEC);
    Serial.print(" CMS=");
    Serial.print(cms, DEC);
    Serial.print(" STM=");
    Serial.print(stepTimeMs, DEC);
    Serial.print(" SDM=");
    Serial.print(stepDelayMs, DEC);
    Serial.print(" SC=");
    Serial.print(stepCount, DEC);
    Serial.print(" RC=");
    Serial.print(retryCount, DEC);
    Serial.print(" dist=");
    Serial.print(distArray[stepCount], DEC);
    Serial.println("");
#endif // #ifdef USR_DEBUG

    if (stepReverse == true)
        stepCount--;
    else
        stepCount++;
        
    stepTimeMs = cms + moveTimeMs;

    if ( ((stepReverse == false) && (stepCount >= USR_STEP_NR))
      || ((stepReverse == true)  && (stepCount <  0          )) )
    {
        radarRunning = false;
        radarDone = true;
    }
    else
    {
        setAngleDeg(degArray[stepCount]);
    }
    
    return true;
}

bool Usr::scanIsEnded(void)
{
    if (radarRunning != true)
        return true;

    return false;
}

uint16_t Usr::getStepDistMm(uint8_t stepId)
{
     if (stepId >= USR_STEP_NR)
     {
         return 0;
     }

     return distArray[stepId];
}

uint8_t Usr::getRetryCount(void)
{
    return retryCount;
}

uint16_t Usr::getUpdateDelayMs(void)
{
    return stepDelayMs;
}

uint16_t Usr::getCurrentDistMm(void)
{
    float distMm;
/*
    distMm = ultraSensor.distanceCm();
    
//    Serial.print("getCurrentDistMm ");
    Serial.println(distMm, 5);

//    distMm *= 10.0;
*/
    distMm = 90;    
    return (uint16_t) distMm;
}

/******************************************************************************/

#endif // #ifdef ENABLE_ULTRASONIC_RADAR

/******************************** END OF FILE *********************************/

