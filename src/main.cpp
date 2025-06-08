#include <Arduino.h>
#include "cam_mux.h"

void setup() { CamMux::init(); }

void loop() {
  CamMux::poll();
  static uint32_t t0;
  if (millis() - t0 > 500) {
    Serial.print("L(");  Serial.print(CamMux::leftAngle());  Serial.print(',');
    Serial.print(CamMux::leftRange());  Serial.print(")  R(");
    Serial.print(CamMux::rightAngle()); Serial.print(',');
    Serial.print(CamMux::rightRange()); Serial.print(")  -> SEL(");
    Serial.print(CamMux::selectedAngle()); Serial.print(',');
    Serial.print(CamMux::selectedRange()); Serial.println(')');
    t0 = millis();
  }
}
