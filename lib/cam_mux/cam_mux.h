#pragma once
#include <Arduino.h>

namespace CamMux {

void init(uint32_t baud_left = 115200,
          uint32_t baud_right = 250000, uint8_t right_rx_pin = 2);
void poll();

int16_t selectedAngle();   // angle of nearer target
int16_t selectedRange();   // its range
int16_t leftAngle();
int16_t leftRange();
int16_t rightAngle();
int16_t rightRange();

} // namespace CamMux

