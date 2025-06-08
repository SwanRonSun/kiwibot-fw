#pragma once
#include <Arduino.h>

namespace CamMux {

// Call once in setup()
void init(uint32_t baud_left = 115200);

// Call every loop() – non-blocking
void poll();

// Accessors (updated atomically inside poll)
int16_t latestAngle();   // degrees  –180…+180
int16_t latestRange();   // millimetres

} // namespace CamMux
