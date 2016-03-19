/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : usr.h                                                         */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Ultrasonic Radar                                              */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#ifndef _USR_H_
#define _USR_H_

#include "config.h"

#ifdef ENABLE_ULTRASONIC_RADAR

/******************************************************************************/

#define USR_CENTER_OFFSET_DEG -10 // the offset angle in degree when the ultrasonic radar is facing the front

#define USR_STEP_NR    7 // the number of step in a radar scan cycle
#define USR_STEP_DEG  20 // the rotation degree per step
#define USR_DEG_MIN  -75 // the minimum angle the radar can be rotated to
#define USR_DEG_MAX  +75 // the maxmimum angle the radar can be rotated to
#define USR_RETRY_MAX 10

#define USR_SERVO_1   1
#define USR_SERVO_2   2

class Usr
{
public:
    Usr();
    ~Usr();
    
    bool     init(uint8_t _servoId);
    bool     setAngleDeg(int8_t _servoDeg);
    int8_t   getAngleDeg(void);
    double   getDist(void);

    bool     startScan(void);
    bool     updateScan(long cms);
    bool     scanIsEnded(void);
    uint16_t getStepDistMm(uint8_t stepId);
    uint8_t  getRetryCount(void);
    uint16_t getUpdateDelayMs(void);
    uint16_t getCurrentDistMm(void);
    
private:
    bool     inited;      // whether ultrasonic radar is initialized
    uint8_t  servoId;        // the servoId used for the ultrasonic radar

    bool     radarRunning;   // currently performing a radar measuring cycle
    bool     radarDone;      // the radar scanning cycle is completed, should be able to see some result
    uint8_t  retryCount;     // counting the number of retries per radar scan
    int8_t   stepCount;      // the step count during a measuring cycle (0 ~ USR_STEP_NR-1)
    bool     stepReverse;    // do radar scan the other way around
    bool     stepMoving;     // moving to the next step, otherwise, should be measuring the distance
    long     stepDelayMs;    // keep a record of how many ms delay per expected update routine 
    long     stepTimeMs;     // when should the next step be performed
    long     moveTimeMs;     // when should the next step be performed

    uint8_t  degArray[USR_STEP_NR];  // the rotation angle in degree for each step
    uint16_t distArray[USR_STEP_NR]; // the distance detected are stored here
};

/******************************************************************************/

#endif // #ifdef ENABLE_ULTRASONIC_RADAR

#endif // #ifndef _USR_H_

/******************************** END OF FILE *********************************/

