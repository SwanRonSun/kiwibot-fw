#include "state_machine.h"
#include "cam_mux.h"
#include "strike_hal.h"
#include "drv8833_hal.h"
#include "failsafe.h"

using namespace StateMachine;

namespace {
  // Current state and entry timestamp
  State    s      = State::IDLE;
  uint32_t tEntry = 0;
  
  // State timeouts (ms)
  const uint32_t ACQUIRE_TIMEOUT = 2000;   // 2s to acquire target
  const uint32_t ORBIT_TIMEOUT   = 10000;  // 10s max orbit time
  const uint32_t STRIKE_WINDOW   = 200;    // 200ms strike duration
  
  // State names for debugging
  const char* STATE_NAMES[] = {
    "IDLE", "ACQUIRE", "ORBIT", "STRIKE"
  };
}

void StateMachine::init() {
  // Initialize subsystems
  StrikeHal::init();
  Failsafe::init();
  tEntry = millis();
  Serial.println("State Machine initialized");
}

void StateMachine::run() {
  // Always check failsafe first
  Failsafe::poll();
  if (Failsafe::tripped) {
    if (s != State::IDLE) {
      Serial.println("Failsafe active - returning to IDLE");
      s = State::IDLE;
      DrvHal::stop();
    }
    return;
  }
  
  // Update subsystems
  StrikeHal::poll();
  
  // Get current sensor data
  int16_t angle = CamMux::selectedAngle();
  int16_t range = CamMux::selectedRange();
  uint32_t elapsed = millis() - tEntry;
  
  // Previous state for transition detection
  State prevState = s;
  
  switch (s) {
    case State::IDLE:
      if (range < 1000) {  // Target within 1m
        s = State::ACQUIRE;
        tEntry = millis();
        Serial.print("Target detected at ");
        Serial.print(range);
        Serial.println("mm - entering ACQUIRE");
      }
      break;

    case State::ACQUIRE:
      if (elapsed > 1000) {  // Locked for 1s
        s = State::ORBIT;
        tEntry = millis();
        Serial.println("Target acquired - entering ORBIT");
      } else if (range > 1200) {  // Lost target
        s = State::IDLE;
        tEntry = millis();
        Serial.println("Target lost - returning to IDLE");
      } else if (elapsed > ACQUIRE_TIMEOUT) {  // Timeout
        s = State::IDLE;
        tEntry = millis();
        Serial.println("Acquire timeout - returning to IDLE");
      }
      break;

    case State::ORBIT:
      // Active orbit control
      DrvHal::spinToward(angle);
      
      // Check strike conditions
      if (abs(angle) < 10 && range < 120) {  // Within ±10° and 12cm
        s = State::STRIKE;
        tEntry = millis();
        Serial.println("Strike position! Angle=");
        Serial.print(angle);
        Serial.print("° Range=");
        Serial.print(range);
        Serial.println("mm");
      } else if (range > 1200) {  // Lost target
        s = State::IDLE;
        tEntry = millis();
        DrvHal::stop();
        Serial.println("Target lost during orbit - returning to IDLE");
      } else if (elapsed > ORBIT_TIMEOUT) {  // Timeout
        s = State::IDLE;
        tEntry = millis();
        DrvHal::stop();
        Serial.println("Orbit timeout - returning to IDLE");
      }
      break;

    case State::STRIKE:
      // Attempt strike (respects cooldown)
      if (StrikeHal::fire()) {
        Serial.println("Strike executed!");
      }
      
      // Return to orbit after strike window
      if (elapsed > STRIKE_WINDOW) {
        s = State::ORBIT;
        tEntry = millis();
        Serial.println("Strike complete - returning to ORBIT");
      }
      break;
  }
  
  // Log state transitions
  if (s != prevState) {
    char buf[64];
    snprintf(buf, sizeof(buf), "State transition: %s -> %s", 
             STATE_NAMES[static_cast<uint8_t>(prevState)],
             STATE_NAMES[static_cast<uint8_t>(s)]);
    Serial.println(buf);
  }
}

State StateMachine::current() {
  return s;
}

const char* StateMachine::currentName() {
  return STATE_NAMES[static_cast<uint8_t>(s)];
}