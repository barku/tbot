/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : irrx.cpp                                                      */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : Infra-red Receiver                                            */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#include "config.h"

#ifdef ENABLE_IR_RECEIVER

/******************************************************************************/

#include "irrx.h"
#include <Makeblock.h>

MeInfraredReceiver IrRecv(PORT_4);

Irrx::Irrx()
{
    inited = false;
}

Irrx::~Irrx()
{
    inited = false;
}

void Irrx::init(void)
{
    rxValid = false;
    data = 0;
    rxValidMs = 0;
    inited = true;
    IrRecv.begin();                             // start infrared receiver
}

void Irrx::printData(uint8_t data)
{
    switch(data)
    {
//        case 0xFF       :                                           break;  // seems to be getting this after each button pressed, ignore
            
        case IR_BUTTON_UP:      Serial.println("IR_BUTTON_UP");      break;  // sams as IR_BUTTON_PLUS
        case IR_BUTTON_DOWN:    Serial.println("IR_BUTTON_DOWN");    break;  // sams as IR_BUTTON_MINUS
        case IR_BUTTON_LEFT:    Serial.println("IR_BUTTON_LEFT");    break;  // sams as IR_BUTTON_PREVIOUS
        case IR_BUTTON_RIGHT:   Serial.println("IR_BUTTON_RIGHT");   break;  // sams as IR_BUTTON_NEXT
      
        case IR_BUTTON_SETTING: Serial.println("IR_BUTTON_SETTING"); break;  // sams as IR_BUTTON_PLAY
            
        case IR_BUTTON_A:       Serial.println("IR_BUTTON_A");       break;  // sams as IR_BUTTON_POWER
        case IR_BUTTON_B:       Serial.println("IR_BUTTON_B");       break;  // no other alias
        case IR_BUTTON_C:       Serial.println("IR_BUTTON_C");       break;  // sams as IR_BUTTON_MENU
        case IR_BUTTON_D:       Serial.println("IR_BUTTON_D");       break;  // sams as IR_BUTTON_TEST
        case IR_BUTTON_E:       Serial.println("IR_BUTTON_E");       break;  // sams as IR_BUTTON_RETURN
        case IR_BUTTON_F:       Serial.println("IR_BUTTON_F");       break;  // sams as IR_BUTTON_CLR
            
        case IR_BUTTON_0:       Serial.println("IR_BUTTON_0");       break;
        case IR_BUTTON_1:       Serial.println("IR_BUTTON_1");       break;
        case IR_BUTTON_2:       Serial.println("IR_BUTTON_2");       break;
        case IR_BUTTON_3:       Serial.println("IR_BUTTON_3");       break;
        case IR_BUTTON_4:       Serial.println("IR_BUTTON_4");       break;
        case IR_BUTTON_5:       Serial.println("IR_BUTTON_5");       break;
        case IR_BUTTON_6:       Serial.println("IR_BUTTON_6");       break;
        case IR_BUTTON_7:       Serial.println("IR_BUTTON_7");       break;
        case IR_BUTTON_8:       Serial.println("IR_BUTTON_8");       break;
        case IR_BUTTON_9:       Serial.println("IR_BUTTON_9");       break;            
        default         :       Serial.print(  "IR_BUTTON UNKNOWN=0x");
                                Serial.println(data, HEX);           break;
    }
}

bool Irrx::check(long cms)
{
    uint8_t tmpData;

    rxValid = false;

    if (IrRecv.available() || IrRecv.buttonState()) 
    {
        tmpData = (uint8_t) IrRecv.read();
        if (tmpData == 0xFF)
            return false;

        if ((cms - rxValidMs) >  IRRX_DEBOUNCE_TIME_MS)
        {
            rxValidMs = cms;
            rxValid = true;
            data = tmpData;
            return true;
        }
    } 

    return false;
}

bool Irrx::isDataReady(void)
{
   return rxValid;  
}

uint8_t Irrx::getData(void)
{
    rxValid = false;
    return data;
}

/******************************************************************************/

#endif // #ifdef ENABLE_IR_RECEIVER

/******************************** END OF FILE *********************************/

