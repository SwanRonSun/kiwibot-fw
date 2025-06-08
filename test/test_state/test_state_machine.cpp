#include <Arduino.h>
#include <unity.h>
#include "state_machine.h"
#include "cam_mux.h"

// We'll use a different approach for time control
// Instead of overriding millis(), we'll just use delays

void setUp()   { 
  // Reset for each test
}

void tearDown(){}

void test_idle_to_acquire() {
  StateMachine::init();
  TEST_ASSERT_EQUAL(StateMachine::State::IDLE, StateMachine::current());

  // Target appears at 800mm
  CamMux::_test_setLeft(45, 800);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ACQUIRE, StateMachine::current());
}

void test_acquire_to_orbit() {
  // Start in ACQUIRE state
  StateMachine::init();
  CamMux::_test_setLeft(45, 800);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ACQUIRE, StateMachine::current());
  
  // Wait for acquire timeout (1+ seconds)
  delay(1100);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ORBIT, StateMachine::current());
}

void test_orbit_to_strike() {
  // Get to ORBIT state
  StateMachine::init();
  CamMux::_test_setLeft(45, 800);
  StateMachine::run();
  delay(1100);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ORBIT, StateMachine::current());
  
  // Move within strike window
  CamMux::_test_setLeft(5, 100);  // 5° angle, 100mm range
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::STRIKE, StateMachine::current());
}

void test_strike_returns_to_orbit() {
  // Get to STRIKE state
  StateMachine::init();
  CamMux::_test_setLeft(45, 800);
  StateMachine::run();
  delay(1100);
  StateMachine::run();
  CamMux::_test_setLeft(5, 100);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::STRIKE, StateMachine::current());
  
  // After strike window (200ms), should return to ORBIT
  delay(250);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ORBIT, StateMachine::current());
}

void setup() {
  delay(2000);  // Allow time for serial monitor
  
  UNITY_BEGIN();
  RUN_TEST(test_idle_to_acquire);
  RUN_TEST(test_acquire_to_orbit);
  RUN_TEST(test_orbit_to_strike);
  RUN_TEST(test_strike_returns_to_orbit);
  UNITY_END();
}

void loop() {
  // Nothing to do here
}