#include <Arduino.h>

#include "led.h"

#define LED_BLUE_PIN       13                    // set up blue LED pin number
#define LED_BLUE_ON_OFF_MS 500
LED LedBlue;

void setup()
{
    Serial.begin(9600);                          // configure serial port baud rate and start serial port
    Serial.println("");
    Serial.println("...ABC...");

    LedBlue.SetPin(LED_BLUE_PIN);               // setup on-board blue led as heartbeat for now
    LedBlue.SetDuty(LED_BLUE_ON_OFF_MS, LED_BLUE_ON_OFF_MS);
    Serial.println("HEARBEAT OK");
}

void loop()
{
    // do something here

      
    // update heartbeat
    LedBlue.Update(millis());
}

