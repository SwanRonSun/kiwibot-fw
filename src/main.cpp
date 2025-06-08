#include <Arduino.h>
#include "cam_mux.h"
#include "state_machine.h"

void setup() {
  Serial.begin(115200);
  CamMux::init();          // left cam on RX0 @115 200 by default
  StateMachine::init();
}

void loop() {
  CamMux::poll();
  StateMachine::run();

  static uint32_t tPrint = 0;
  if (millis() - tPrint > 500) {        // twice per second
    Serial.print("State = ");
    Serial.println(static_cast<uint8_t>(StateMachine::current())); // 0-3
    tPrint = millis();
  }
}
