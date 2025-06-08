#include "failsafe.h"
#include "drv8833_hal.h"   // we need to stop motors

volatile bool Failsafe::tripped = false;

void Failsafe::init() {
  Serial.println("Failsafe armed.  Press 'k' to kill.");
}

void Failsafe::poll() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'k' || c == 'K') {
      tripped = true;
      DrvHal::stop();
      Serial.println("** FAILSAFE TRIPPED **  Wheels set to 0");
    }
    // purge any CR/LF that might follow
  }
}
