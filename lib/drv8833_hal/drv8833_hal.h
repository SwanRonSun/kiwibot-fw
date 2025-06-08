#pragma once
#include <Arduino.h>

namespace DrvHal {

void init();                                     // call once in setup()
void setWheelSpeeds(const int8_t pwm[3]);        // desired -127..+127
void updateOutputs();                            // flush to hardware (mock)
void stop();                                     // immediate all-stop

} // namespace DrvHal
