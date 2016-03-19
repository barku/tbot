/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : irrx.h                                                        */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Infra-red Receiver                                            */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#ifndef _IRRX_H_
#define _IRRX_H_

#include "config.h"

#ifdef ENABLE_IR_RECEIVER

/******************************************************************************/

#define IRRX_DEBOUNCE_TIME_MS 200

class Irrx
{
public:
    Irrx();
    ~Irrx();
    
    void    init(void);
    void    printData(uint8_t data);
    bool    check(long cms);
    bool    isDataReady(void);
    uint8_t getData(void);

private:
    bool    inited;
    bool    rxValid;
    uint8_t data;
    long    rxValidMs;
};

/******************************************************************************/

#endif // #ifdef ENABLE_IR_RECEIVER

#endif // #ifndef _IRRX_H_

/******************************** END OF FILE *********************************/

