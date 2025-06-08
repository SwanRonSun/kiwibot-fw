#define UNIT_TEST      // enable test hooks in cam_mux
#include <unity.h>
#include "state_machine.h"
#include "cam_mux.h"

// Fake millis() to control time progression in tests
static uint32_t fakeMillis = 0;
uint32_t millis() { return fakeMillis; }

void setUp()   {}  // not used
void tearDown(){}  // not used

void test_transitions() {
  // Start up
  StateMachine::init();
  TEST_ASSERT_EQUAL(StateMachine::State::IDLE, StateMachine::current());

  // Simulate a target appearing at 800 mm
  CamMux::_test_setLeft(45, 800);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ACQUIRE, StateMachine::current());

  // Advance time by 1.1 seconds
  fakeMillis += 1100;
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ORBIT, StateMachine::current());

  // Move within strike window
  CamMux::_test_setLeft(0, 100);
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::STRIKE, StateMachine::current());

  // After 300 ms, should return to ORBIT
  fakeMillis += 300;
  StateMachine::run();
  TEST_ASSERT_EQUAL(StateMachine::State::ORBIT, StateMachine::current());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_transitions);
  UNITY_END();
  return 0;
}
