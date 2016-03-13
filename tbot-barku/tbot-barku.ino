
#include <Arduino.h>
#include <Makeblock.h>

#include "led.h"
#include "usr.h"

//******************************************************************************

// blue LED as heartbeat

#define LED_BLUE_PIN         13                  
#define LED_BLUE_ON_OFF_MS   700

LED LedBlue;

// IR Remote Control Receiver

#define IRRECV_DEBOUNCE_TIME_MS 100

MeInfraredReceiver IrRecv(PORT_4);

uint8_t irRecvData;
bool    irRecvValid;
long    irRecvValidMs;

//******************************************************************************

uint8_t IrRecvCheck(void)
{
    uint8_t data;
    long cms;

    if (IrRecv.available() || IrRecv.buttonState()) 
    {
        cms = millis();
        
        data = (uint8_t) IrRecv.read();
        if (data == 0xFF)
        {
            return 0x00;
        }

        if ((cms - irRecvValidMs) >  IRRECV_DEBOUNCE_TIME_MS)
        {
            irRecvValidMs = cms;
            irRecvValid = true;
        }
        else
        {
            irRecvValid = false;
        }
    } 
    else 
    {
        return 0x00;
    }
    
    if (irRecvValid != true)    
        return 0x00;

    Serial.print(millis(), DEC);
    Serial.print(" ");
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

    return data;
}

//******************************************************************************

void setup()
{
    Serial.begin(9600);                          // configure serial port baud rate and start serial port
    Serial.println("");
    Serial.println(".ABC.");

    LedBlue.setPin(LED_BLUE_PIN);               // setup on-board blue led as heartbeat for now
    LedBlue.setDuty(LED_BLUE_ON_OFF_MS, LED_BLUE_ON_OFF_MS);
    Serial.println("HEARBEAT OK");

    IrRecv.begin();                             // start infrared receiver
    irRecvData = 0;
    irRecvValid = false;
    irRecvValidMs = 0;
    Serial.println("IR RECV  OK");

}

void loop()
{
    irRecvData = IrRecvCheck();
    switch(irRecvData)
    {
        case 0x00:  
        case 0xFF:              break;  // invalid or no irRecvData
/*
        case IR_BUTTON_UP:      break;
        case IR_BUTTON_DOWN:    break;
        case IR_BUTTON_LEFT:    break;
        case IR_BUTTON_RIGHT:   break;

        case IR_BUTTON_SETTING: break;

        case IR_BUTTON_A:       break;
        case IR_BUTTON_B:       break;
        case IR_BUTTON_C:       break;
        case IR_BUTTON_D:       break;
        case IR_BUTTON_E:       break;
        case IR_BUTTON_F:       break;

        case IR_BUTTON_0:       break;
        case IR_BUTTON_1:       break;
        case IR_BUTTON_2:       break;
        case IR_BUTTON_3:       break;
        case IR_BUTTON_4:       break;
        case IR_BUTTON_5:       break;
        case IR_BUTTON_6:       break;
        case IR_BUTTON_7:       break;
        case IR_BUTTON_8:       break;
        case IR_BUTTON_9:       break;            
*/        
        default:                break;
    }
    
    // do more stuff here?
    
    // update heartbeat
    LedBlue.update(millis());
}

