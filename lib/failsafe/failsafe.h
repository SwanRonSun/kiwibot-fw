#pragma once
#include <Arduino.h>

namespace Failsafe {

void init();          // call once in setup()
void poll();          // call every loop() iteration

extern volatile bool tripped;   // global flag any module can check
}
