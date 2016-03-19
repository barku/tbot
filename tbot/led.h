/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : led.h                                                         */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Status LED light                                              */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#ifndef _LED_H_
#define _LED_H_

#include "config.h"

#ifdef ENABLE_LED

/******************************************************************************/

class Led
{
public:
    Led();
    ~Led();
    
    bool checkPinIsValid(uint8_t ledPin);
    bool setPin(uint8_t ledPin);
    
    void on(void);
    void off(void);

    bool setDuty(uint16_t ledDutyOnMs, uint16_t ledDutyOffMs);
    void update(long cms);

private:
    bool     pinInited;
    uint8_t  pin;
    
    uint8_t  stateOn;
    
    uint16_t dutyOnMs;
    uint16_t dutyOffMs;
    
    uint32_t stateLastChangeMs;
    uint32_t stateNextChangeMs;
};

/******************************************************************************/

#endif // #ifdef ENABLE_LED

#endif // #ifndef _LED_H_

/******************************** END OF FILE *********************************/

