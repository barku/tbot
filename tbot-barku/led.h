#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <Arduino.h>

class LED
{
public:
    LED();
    ~LED();
    
    bool CheckPinIsValid(uint8_t led_pin);
    bool SetPin(uint8_t led_pin);
    
    void On(void);
    void Off(void);

    bool SetDuty(uint16_t led_duty_on_ms, uint16_t led_duty_off_ms);
    void Update(long ms);

private:
    bool     pin_inited;
    uint8_t  pin;
    
    uint8_t  state_on;
    
    uint16_t duty_on_ms;
    uint16_t duty_off_ms;
    
    uint32_t state_last_change_ms;
    uint32_t state_next_change_ms;
};

#endif // #ifndef _LED_H_


