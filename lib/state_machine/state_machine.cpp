#include "state_machine.h"
#include "cam_mux.h"
#include "strike_hal.h"
#include "drv8833_hal.h"   // for spinToward()

using namespace StateMachine;

namespace {
  // Current state and entry timestamp
  State    s      = State::IDLE;
  uint32_t tEntry = 0;
}

void StateMachine::init() {
  // Initialize strike system and timestamp
  StrikeHal::init();
  tEntry = millis();
}

void StateMachine::run() {
  // Update strike cooldown
  StrikeHal::poll();

  switch (s) {

    case State::IDLE:
      if (CamMux::selectedRange() < 1000) {      // target within 1 m?
        s      = State::ACQUIRE;
        tEntry = millis();
      }
      break;

    case State::ACQUIRE:
      if (millis() - tEntry > 1000) {            // locked for 1 s?
        s      = State::ORBIT;
        tEntry = millis();
      }
      break;

    case State::ORBIT:
      // Rotate toward the target angle
      DrvHal::spinToward(CamMux::selectedAngle());

      // If aligned and close, go strike
      if (abs(CamMux::selectedAngle()) < 10 &&  // within ±10°
          CamMux::selectedRange()  < 120) {     // within 12 cm
        s      = State::STRIKE;
        tEntry = millis();
      }
      break;

    case State::STRIKE:
      // Attempt the strike (prints “>>> BANG! <<<” if off cooldown)
      StrikeHal::fire();

      // After the jab window, return to orbit
      if (millis() - tEntry > 200) {            // 200 ms jab window
        s      = State::ORBIT;
        tEntry = millis();
      }
      break;
  }
}

State StateMachine::current() {
  return s;
}
