#include <Arduino.h>
#include "drv8833_hal.h"

void setup() {
  DrvHal::init();
}

void loop() {
  static uint32_t t0;
  if (millis() - t0 > 1000) {          // every second
    static int8_t demo = 0;
    int8_t speeds[3] = { demo, -demo, 0 };
    DrvHal::setWheelSpeeds(speeds);
    DrvHal::updateOutputs();
    demo += 10;                        // ramp 0→100
    if (demo > 100) demo = 0;
    t0 = millis();
  }
}
