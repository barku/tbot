
#include <Makeblock.h>
#include <Arduino.h>

#include <Servo.h>

#include "usr.h"

#define USR_DEBUG

// servo is connected to ME Orion through a slot on Me RJ25 Adapter

MePort port(PORT_6);

Servo servo;
int16_t servo1Pin =  port.pin1();
int16_t servo2Pin =  port.pin2();

MeUltrasonicSensor ultraSensor(PORT_3); /* Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */

//******************************************************************************

USR::USR()
{
    uint8_t deg;
    
    usrInited      = false;
    servoId        = 0;

    deg = 0 - ((USR_STEP_NR / 2) * USR_STEP_DEG);
    for (uint8_t i = 0; i < USR_STEP_NR; i++)
    {
        degArray[i] = deg;
        deg += USR_STEP_DEG;
    }
}

USR::~USR()
{
    usrInited = false;
}

bool USR::init(uint8_t _servoId)
{
    if (_servoId == 1)
    {
        servo.attach(servo1Pin);
    }
    else if (_servoId == 2)
    {
        servo.attach(servo2Pin);
    }
    else
    {
        return false;  
    }

    usrInited = true;
    return true;
}

bool USR::setAngleDeg(int8_t _servoDeg)
{
    uint8_t servoVal;
    double dist;

    if (_servoDeg > USR_DEG_MAX)
        _servoDeg = USR_DEG_MAX;
    if (_servoDeg < USR_DEG_MIN)
        _servoDeg = USR_DEG_MIN;
    
    servoVal = (0 - _servoDeg) + 90 + USR_CENTER_OFFSET_DEG; 
    servo.write(servoVal);
    
    return true;
}

int8_t getAngleDeg(void)
{
    uint8_t servoDeg;

    servoDeg = 0 - ((int16_t) servo.read() - 90);

    return (int8_t) servoDeg;
}

double USR::getDist(void)
{
    return ultraSensor.distanceCm();
}

bool USR::radarStart(void)
{
    radarRunning = true;
    radarDone = false;
    retryCount = 0;
    
    if ((stepReverse == false) && (stepCount >= USR_STEP_NR - 1))
    {
        stepCount = USR_STEP_NR - 1;    
        stepReverse = true;
    }
    else
    {
        stepCount = 0;    
        stepReverse = false;
    }
    
    stepMoving = false;
    stepTimeMs = 0;
    moveTimeMs = 500;

    return true;
}

bool USR::usrUpdate(long cms)
{
    double dist;
    
    // if radar is not running,
    if (radarRunning != true)
        return false;

    // if radar just set the first step position and stepTimeMs
    if (stepTimeMs == 0)
    {
        setAngleDeg(degArray[stepCount]);
        stepTimeMs = cms + moveTimeMs;
        return true;
    }

    // if time is not yet there, return
    if (cms < stepTimeMs)
        return false;

    stepDelayMs = cms - stepTimeMs; // the time delay for expected service routine in ms    
    dist = getDist();
    
// MCDEBUG++    
#if 1
    if (dist < 1)
    {
        // not sure if this is a bug, when USR is returning a very small value (0.0 ~ 0.1), 
        // it usually keeps returning small value if we keep getting readings,
        // by postponding the reading for a few ms, it seems to be working with high success.
        if (retryCount < USR_RETRY_MAX)
        {
            stepTimeMs = cms + 10;
            retryCount++;
            return true;
        }
    }
#endif
//MCDEBUG--       

    if (retryCount >= USR_RETRY_MAX)
    {
        distArray[stepCount] = 0;
    }
    else
    {
        distArray[stepCount] = (uint16_t) (dist * 10);
    }

#ifdef USR_DEBUG
    Serial.print(millis(), DEC);
    Serial.print(" CMS=");
    Serial.print(cms, DEC);
    Serial.print(" STM=");
    Serial.print(stepTimeMs, DEC);
    Serial.print(" SDM=");
    Serial.print(stepDelayMs, DEC);
    Serial.print(" SC=");
    Serial.print(stepCount, DEC);
    Serial.print(" RC=");
    Serial.print(retryCount, DEC);
    Serial.print(" dist=");
    Serial.print(distArray[stepCount], DEC);
    Serial.println("");
#endif // #ifdef USR_DEBUG

    if (stepReverse == true)
        stepCount--;
    else
        stepCount++;
        
    stepTimeMs = cms + moveTimeMs;

    if ( ((stepReverse == false) && (stepCount >= USR_STEP_NR))
      || ((stepReverse == true)  && (stepCount <  0          )) )
    {
        radarRunning = false;
        radarDone = true;
    }
    else
    {
        setAngleDeg(degArray[stepCount]);
    }
    
    return true;
}

bool USR::radarIsEnded(void)
{
    if (radarRunning != true)
        return true;

    return false;
}

uint16_t USR::getStepDistMm(uint8_t stepId)
{
     if (stepId >= USR_STEP_NR)
     {
         return 0;
     }

     return distArray[stepId];
}

uint8_t USR::getRetryCount(void)
{
    return retryCount;
}

uint16_t USR::getUpdateDelayMs(void)
{
    return stepDelayMs;
}


