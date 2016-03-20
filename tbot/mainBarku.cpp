/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : mainBarku.c                                                   */
/* Author    : Mark Chang (markmhchang@gmail.com)                            */
/* Purpose   : setup and main loop                                           */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160318 created by Mark Chang                                    */
/*                                                                           */
/*****************************************************************************/

#include "config.h"

#ifdef MAIN_BARKU

/******************************************************************************/

#include "led.h"
#include "irrx.h"
#include "usr.h"
#include "dwm.h"

#include <Makeblock.h>

/**************************************/

#ifdef ENABLE_LED

// blue LED as heartbeat
#define LED_BLUE_PIN         13                  
#define LED_BLUE_ON_OFF_MS   700

Led ledBlue;

#endif // #ifdef ENABLE_LED

/**************************************/

#ifdef ENABLE_ULTRASONIC_RADAR

Usr usr;

bool     usrEnable = false;
uint8_t  usrUpdateCount; 
uint32_t usrUpdateDelayMs;
bool     usrUpdateStatus;
#endif // #ifdef ENABLE_ULTRASONIC_RADAR

/**************************************/

// Differential Wheel Motion
#ifdef ENABLE_DIFF_WHEEL_MOTION

Dwm dwm;

#endif // #ifdef ENABLE_DIFF_WHEEL_MOTION

/**************************************/

#ifdef ENABLE_IR_RECEIVER

Irrx irrx;

uint8_t irData;

#endif // #ifdef ENABLE_IR_RECEIVER

/**************************************/

#define CMD_DIR_STOP  0
#define CMD_DIR_UP    1
#define CMD_DIR_DOWN  2
#define CMD_DIR_LEFT  3
#define CMD_DIR_RIGHT 4

uint8_t cmdMode  = 'A';
int16_t cmdSpeed  = 50;
int16_t cmdDistMm = 100;

/******************************************************************************/

void printTextLog(char *str)
{
    Serial.print(millis(), DEC);
    Serial.print(" ");
    Serial.println(str);      
}

void cmd09(uint8_t value)
{
    float lSpeed, rSpeed;

    if (cmdMode == 'A') 
    {
        cmdDistMm = 20 * (value + 1);
        Serial.print(millis(), DEC);
        Serial.print(" CMD ");
        Serial.print(value, DEC);
        Serial.print(" DIST=");
        Serial.println(cmdDistMm, DEC);
    }
    else if (cmdMode == 'B')
    {
        dwm.getSpeed(&lSpeed, &rSpeed);
        cmdSpeed = 10 * (value + 1);
        Serial.print(millis(), DEC);
        Serial.print(" CMD ");
        Serial.print(value, DEC);
        Serial.print(" SPEED=");
        Serial.print(cmdSpeed, DEC);      
        Serial.print(" (");
        Serial.print(lSpeed, 2);
        Serial.print(",");
        Serial.print(rSpeed, 2);
        Serial.println(")");

//        modeBUsrDistLastMs = millis() - MODE_B_USR_INTERVAL_MS;
    }
}

void cmdDir(uint8_t dir)
{
    float lPos, rPos;
    
    if (cmdMode == 'A')
    {
#ifdef ENABLE_DIFF_WHEEL_MOTION
        if (dir == CMD_DIR_UP)
        {
//            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" FORWARD DIST=");
            Serial.print(cmdDistMm, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");

            dwm.moveDist(cmdDistMm, cmdSpeed, 0);
        }
        else if (dir == CMD_DIR_DOWN)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" BACKWARD DIST=");
            Serial.print(cmdDistMm, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveDist(-cmdDistMm, cmdSpeed, 0);
        }
        else if (dir == CMD_DIR_LEFT)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" LEFT DIST=");
            Serial.print(cmdDistMm, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveDist(cmdDistMm, cmdSpeed, -45);
        }
        else if (dir == CMD_DIR_RIGHT)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" RIGHT DIST=");
            Serial.print(cmdDistMm, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveDist(cmdDistMm, cmdSpeed, 45);
        }
        else if (dir == CMD_DIR_STOP)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" STOP DIST=");
            Serial.print(cmdDistMm, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
//            dwm.moveSpeed(0, 0);
        }
#endif // #ifdef ENABLE_DIFF_WHEEL_MOTION
    }
    else if (cmdMode == 'B')
    {
/*      
#ifdef ENABLE_DIFF_WHEEL_MOTION
        if (dir == CMD_DIR_UP)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" FORWARD SPEED=");
            Serial.print(cmdSpeed, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveSpeed(cmdSpeed, 0);
        }
        else if (dir == CMD_DIR_DOWN)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" BACKWARD SPEED=");
            Serial.print(cmdSpeed, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveSpeed(-cmdSpeed, 0);
        }
        else if (dir == CMD_DIR_LEFT)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" BACKWARD SPEED=");
            Serial.print(cmdSpeed, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveSpeed(-cmdSpeed, -90);
        }
        else if (dir == CMD_DIR_RIGHT)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" BACKWARD SPEED=");
            Serial.print(cmdSpeed, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveSpeed(-cmdSpeed, 90);
        }
        else if (dir == CMD_DIR_STOP)
        {
            dwm.getPosition(&lPos, &rPos);
            Serial.print(millis(), DEC);
            Serial.print(" STOP SPEED=");
            Serial.print(cmdSpeed, DEC);      
            Serial.print(" (");
            Serial.print(lPos, 2);
            Serial.print(",");
            Serial.print(rPos, 2);
            Serial.println(")");
            
            dwm.moveSpeed(0, 0);
        }
#endif // #ifdef ENABLE_DIFF_WHEEL_MOTION
*/
    }
    else
    {
        if (dir == CMD_DIR_UP)
        {
          
        }
        else if (dir == CMD_DIR_DOWN)
        {
          
        }
        else if (dir == CMD_DIR_LEFT)
        {
          
        }
        else if (dir == CMD_DIR_RIGHT)
        {
          
        }
        else if (dir == CMD_DIR_STOP)
        {
#ifdef ENABLE_ULTRASONIC_RADAR
                                usr.startScan();
                                usrEnable        = true;
                                usrUpdateCount   = 0;
                                usrUpdateDelayMs = 0;
#endif // #ifdef ENABLE_ULTRASONIC_RADAR                                
        }
    }
}

void cmdHandler(uint8_t cmd)
{
    switch(cmd)
    {
        case 0x00:  
        case 0xFF:              break;  // invalid or no irRecvData

        case IR_BUTTON_UP:      cmdDir(CMD_DIR_UP);    break;
        case IR_BUTTON_DOWN:    cmdDir(CMD_DIR_DOWN);  break;
        case IR_BUTTON_LEFT:    cmdDir(CMD_DIR_LEFT);  break;
        case IR_BUTTON_RIGHT:   cmdDir(CMD_DIR_RIGHT); break;
        case IR_BUTTON_SETTING: cmdDir(CMD_DIR_STOP);  break;

        // Command Mode A - IR remote bot in distance mode 
        // UP DOWN LEFT RIGHT RIGHT moves the bot for some distances set by 0~9
        case IR_BUTTON_A:       cmdMode = 'A'; printTextLog("IR MODE A - DIST"); break; 
        
        // Command Mode B - IR remote bot in speed mode 
        // UP DOWN LEFT RIGHT RIGHT moves the bot at some speed set by 0~9 until the next key press
        // bot will stop when detected obstacle using ultrasonic sensor at 0 heading
        case IR_BUTTON_B:       cmdMode = 'B'; printTextLog("IR MODE B - SPEED"); break;

        // Command Mode C - Wander mode with ultrasonic radar
        case IR_BUTTON_C:       cmdMode = 'C'; printTextLog("IR MODE C - WANDER"); break;
        
        case IR_BUTTON_D:       cmdMode = 'D'; printTextLog("IR MODE D"); break;
        case IR_BUTTON_E:       cmdMode = 'E'; printTextLog("IR MODE E"); break;        
        case IR_BUTTON_F:       cmdMode = 'F'; printTextLog("IR MODE F"); break;

        case IR_BUTTON_0:       cmd09(0); break;
        case IR_BUTTON_1:       cmd09(1); break;
        case IR_BUTTON_2:       cmd09(2); break;
        case IR_BUTTON_3:       cmd09(3); break;
        case IR_BUTTON_4:       cmd09(4); break;
        case IR_BUTTON_5:       cmd09(5); break;
        case IR_BUTTON_6:       cmd09(6); break;
        case IR_BUTTON_7:       cmd09(7); break;
        case IR_BUTTON_8:       cmd09(8); break;
        case IR_BUTTON_9:       cmd09(9); break;            

        default:                break;
    }
}

/******************************************************************************/

void setupBarku(void)
{
#ifdef ENABLE_LED
    ledBlue.setPin(LED_BLUE_PIN);               // setup on-board blue led as heartbeat for now
    ledBlue.setDuty(LED_BLUE_ON_OFF_MS, LED_BLUE_ON_OFF_MS);
    Serial.println("HEARBEAT OK");
#endif // #ifdef ENABLE_LED

#ifdef ENABLE_ULTRASONIC_RADAR
    usr.init(USR_SERVO_1);
    usr.setAngleDeg(0);
    Serial.println("USR      OK");
#endif // #ifdef ENABLE_ULTRASONIC_RADAR
    
#ifdef ENABLE_DIFF_WHEEL_MOTION
    dwm.begin();
    Serial.println("DWM      OK");
#endif // #ifdef ENABLE_DIFF_WHEEL_MOTION

#ifdef ENABLE_IR_RECEIVER
    irrx.init();
    Serial.println("IR RECV  OK");
#endif // #ifdef ENABLE_IR_RECEIVER
}

void loopBarku(void)
{
    
#ifdef ENABLE_LED
    // update heartbeat
    ledBlue.update(millis());
#endif // #ifdef ENABLE_LED

#ifdef ENABLE_IR_RECEIVER
    if (irrx.check(millis()) == true)
    {
        irData = irrx.getData();
         cmdHandler(irData);
    }
#endif // #ifdef ENABLE_IR_RECEIVER

#ifdef ENABLE_ULTRASONIC_RADAR
    // update UltraSonicRadar
    if (usrEnable == true)
    {
        usrUpdateStatus = usr.updateScan(millis());  
        if (usrUpdateStatus == true)
        {
            usrUpdateCount++;
            usrUpdateDelayMs += usr.getUpdateDelayMs();
        }

        if (usr.scanIsEnded() == true)
        {
            // print the result of USR
            Serial.print(millis(), DEC);
            Serial.print(" USR UC=");
            Serial.print(usrUpdateCount, DEC);
            Serial.print(" RC=");
            Serial.print(usr.getRetryCount(), DEC);
            Serial.print(" RD=");
            Serial.print(usrUpdateDelayMs, DEC);
            Serial.print(" - ");
            for (uint8_t i = 0; i < USR_STEP_NR; i++)
            {
                Serial.print(usr.getStepDistMm(i), DEC);
                Serial.print("  ");
            }
            Serial.println("");
            usrEnable = false;
        }
    }
#endif // #ifdef ENABLE_ULTRASONIC_RADAR
}

/******************************************************************************/

#endif // #ifdef MAIN_BARKU

/******************************** END OF FILE *********************************/

