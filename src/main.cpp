#include <Arduino.h>
#include "drv8833_hal.h"
#include "failsafe.h"
#include "cam_mux.h"

void setup() {
  DrvHal::init();
  Failsafe::init();
  CamMux::init();               // 115200 by default
}

void loop() {
  Failsafe::poll();
  CamMux::poll();

  static uint32_t t0;
  if (millis() - t0 > 1000) {   // once a second
    Serial.print("Angle = ");
    Serial.print(CamMux::latestAngle());
    Serial.print("  Range = ");
    Serial.println(CamMux::latestRange());
    t0 = millis();
  }
}
