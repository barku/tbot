
#include "led.h"

// valid LED pins on makeblock orion
static const unsigned char valid_pins[] = {0,1,13};

LED::LED()
{
    pin_inited           = false;
    pin                  = 0;

    state_on             = false;
    duty_on_ms           = 0;
    duty_off_ms          = 0;
    
    state_last_change_ms = 0;
    state_next_change_ms = 0;
}

LED::~LED()
{
    pin_inited = false;
}

bool LED::CheckPinIsValid(uint8_t led_pin)
{
    uint8_t nr_of_valid_pins;

    nr_of_valid_pins = sizeof(valid_pins);

//    Serial.println("%s nr_of_valid_pins = %d", __FUNCTION__, nr_of_valid_pins);

    for (int i = 0; i < nr_of_valid_pins; i++)
    {
       if (led_pin == valid_pins[i])
          return true;
    }
    
    return false;
}

bool LED::SetPin(uint8_t led_pin)
{
    if (!CheckPinIsValid(led_pin)) return false;

    pin = led_pin;
    pinMode(pin, OUTPUT);
    pin_inited = true;

    Off();
}

void LED::On(void)
{
    if (pin_inited != true) return;

    digitalWrite(pin, HIGH);
    state_on = true;
}

void LED::Off(void)
{
    if (pin_inited != true) return;

    digitalWrite(pin, LOW);
    state_on = false;
}

bool LED::SetDuty(uint16_t led_duty_on_ms, uint16_t led_duty_off_ms)
{
    if (pin_inited != true) return false;

    if ((led_duty_on_ms >= 1) && (led_duty_off_ms >= 1))
    {
        duty_on_ms  = led_duty_on_ms;
        duty_off_ms = led_duty_off_ms;
        return true;
    }
    else
    {
        duty_on_ms  = 0;
        duty_off_ms = 0;
        return false;
    }
}

void LED::Update(long cms)
{
    if (pin_inited != true) return;
    if ((duty_on_ms == 0) || (duty_off_ms == 0)) return;

    if ((state_last_change_ms == 0) || (state_next_change_ms == 0)) 
    {
        state_last_change_ms = cms;
        if (state_on)
        {
            Off();
            state_next_change_ms = state_last_change_ms + duty_off_ms;
        }
        else
        {
            On();
            state_next_change_ms = state_last_change_ms + duty_on_ms;
        }
    }
    else if (cms >= state_next_change_ms)
    {
        state_last_change_ms = cms;
        if (state_on)
        {
            Off();
            state_next_change_ms = state_last_change_ms + duty_off_ms;
        }
        else
        {        
            On();
            state_next_change_ms = state_last_change_ms + duty_on_ms;
        }
    }
}

