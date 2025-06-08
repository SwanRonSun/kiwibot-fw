#include "state_machine.h"
#include "cam_mux.h"

using namespace StateMachine;

static State s         = State::IDLE;
static uint32_t tEntry = 0;

void StateMachine::init() {
  tEntry = millis();
}

void StateMachine::run() {
  switch (s) {

  case State::IDLE:
    if (CamMux::selectedRange() < 1000) {        // any target within 1 m
      s = State::ACQUIRE;
      tEntry = millis();
    }
    break;

  case State::ACQUIRE:
    if (millis() - tEntry > 1000) {              // 1 s stable vision lock
      s = State::ORBIT;
      tEntry = millis();
    }
    break;

  case State::ORBIT:
    if (abs(CamMux::selectedAngle()) < 10 &&     // ~±10°
        CamMux::selectedRange() < 120) {         // 12 cm
      s = State::STRIKE;
      tEntry = millis();
    }
    break;

  case State::STRIKE:
    if (millis() - tEntry > 200) {               // short jab window
      s = State::ORBIT;                          // go back to orbiting
      tEntry = millis();
    }
    break;
  }
}

State StateMachine::current() { return s; }
