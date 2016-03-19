/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : dwm.h                                                         */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Differential Wheel Motion                                     */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#ifndef _DWM_H_
#define _DWM_H_

#include "config.h"

#ifdef ENABLE_DIFF_WHEEL_MOTION

/******************************************************************************/

//#include <Makeblock.h>
//#include <Wire.h>

#define DWM_TRACK_WIDTH_MM          (float)  30.0 // the distance between the center of left wheel and right wheel
#define DWM_TRACK_SEPARATION_MM     (float) 132.0 // the distance between the center of the left and right track (wheels)
#define DWM_TRACK_DIAMETER_MM       (float)  70.0 // the diameter of the wheel including track thickness
#define DWM_WHEEL_MM_PER_MOTOR_TURN (float) 220.0 // the distance moved when the motor output shaft is rotated one turn

class Dwm
{
public:
    Dwm();
    ~Dwm();

    void begin(void);
    void stop(void);
    int16_t clipDeg180(int16_t deg);
    int16_t clipDeg360(int16_t deg);
    bool moveDist(int16_t distMm, int16_t speed, int16_t angleDeg);
//    bool moveSpeed(int16_t speed, int16_t angleDeg);
    void getSpeed(float *lSpeed, float *rSpeed);
    void getPosition(float *lPos, float *rPos);
    
private:
};

/******************************************************************************/

#endif // #ifdef ENABLE_DIFF_WHEEL_MOTION

#endif // #ifndef _DWM_H_

/******************************** END OF FILE *********************************/

