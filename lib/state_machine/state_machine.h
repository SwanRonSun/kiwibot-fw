#pragma once
#include <Arduino.h>

namespace StateMachine {

enum class State : uint8_t { IDLE, ACQUIRE, ORBIT, STRIKE };

void init();   // call once in setup()
void run();    // call every loop()

State current();  // helper to print/log

}  // namespace StateMachine
