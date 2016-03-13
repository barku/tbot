#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <Arduino.h>

class LED
{
public:
    LED();
    ~LED();
    
    bool checkPinIsValid(uint8_t ledPin);
    bool setPin(uint8_t ledPin);
    
    void on(void);
    void off(void);

    bool setDuty(uint16_t ledDutyOnMs, uint16_t ledDutyOffMs);
    void update(long ms);

private:
    bool     pinInited;
    uint8_t  pin;
    
    uint8_t  stateOn;
    
    uint16_t dutyOnMs;
    uint16_t dutyOffMs;
    
    uint32_t stateLastChangeMs;
    uint32_t stateNextChangeMs;
};

#endif // #ifndef _LED_H_


