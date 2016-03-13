
#include "led.h"

// valid LED pins on makeblock orion
static const unsigned char validPins[] = {0,1,13};

LED::LED()
{
    pinInited           = false;
    pin                  = 0;

    stateOn             = false;
    dutyOnMs           = 0;
    dutyOffMs          = 0;
    
    stateLastChangeMs = 0;
    stateNextChangeMs = 0;
}

LED::~LED()
{
    pinInited = false;
}

bool LED::checkPinIsValid(uint8_t ledPin)
{
    uint8_t nrOfValidPins;

    nrOfValidPins = sizeof(validPins);

    for (int i = 0; i < nrOfValidPins; i++)
    {
       if (ledPin == validPins[i])
          return true;
    }
    
    return false;
}

bool LED::setPin(uint8_t ledPin)
{
    if (!checkPinIsValid(ledPin)) return false;

    pin = ledPin;
    pinMode(pin, OUTPUT);
    pinInited = true;

    off();
}

void LED::on(void)
{
    if (pinInited != true) return;

    digitalWrite(pin, HIGH);
    stateOn = true;
}

void LED::off(void)
{
    if (pinInited != true) return;

    digitalWrite(pin, LOW);
    stateOn = false;
}

bool LED::setDuty(uint16_t ledDutyOnMs, uint16_t ledDutyOffMs)
{
    if (pinInited != true) return false;

    if ((ledDutyOnMs >= 1) && (ledDutyOffMs >= 1))
    {
        dutyOnMs  = ledDutyOnMs;
        dutyOffMs = ledDutyOffMs;
        return true;
    }
    else
    {
        dutyOnMs  = 0;
        dutyOffMs = 0;
        return false;
    }
}

void LED::update(long cms)
{
    if (pinInited != true) return;
    if ((dutyOnMs == 0) || (dutyOffMs == 0)) return;

    if ((stateLastChangeMs == 0) || (stateNextChangeMs == 0)) 
    {
        stateLastChangeMs = cms;
        if (stateOn)
        {
            off();
            stateNextChangeMs = stateLastChangeMs + dutyOffMs;
        }
        else
        {
            on();
            stateNextChangeMs = stateLastChangeMs + dutyOnMs;
        }
    }
    else if (cms >= stateNextChangeMs)
    {
        stateLastChangeMs = cms;
        if (stateOn)
        {
            off();
            stateNextChangeMs = stateLastChangeMs + dutyOffMs;
        }
        else
        {        
            on();
            stateNextChangeMs = stateLastChangeMs + dutyOnMs;
        }
    }
}

