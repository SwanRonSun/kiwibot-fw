#include <unity.h>
#include "kiwi_ik.h"

// shared output buffer
static int8_t wheels[3];

void test_forward() {
  // (vx=0, vy=1) → front wheel +127, rears ≈ -63
  KiwiIK::cartesianToWheel(0.0f, 1.0f, wheels);
  TEST_ASSERT_EQUAL_INT(127, wheels[0]);
  TEST_ASSERT_EQUAL_INT(-63, wheels[1]);
  TEST_ASSERT_EQUAL_INT(-63, wheels[2]);
}

void test_strafe_right() {
  // (vx=1, vy=0) → front=0, rear-left ≈ +109, rear-right ≈ -109
  KiwiIK::cartesianToWheel(1.0f, 0.0f, wheels);
  TEST_ASSERT_EQUAL_INT(0,   wheels[0]);
  TEST_ASSERT_EQUAL_INT(109, wheels[1]);
  TEST_ASSERT_EQUAL_INT(-109,wheels[2]);
}

void test_rotate_ccw() {
  // (vx=1, vy=1) mixes translation+rotation; just check non-zero
  KiwiIK::cartesianToWheel(1.0f, 1.0f, wheels);
  TEST_ASSERT_NOT_EQUAL(0, wheels[0]);
  TEST_ASSERT_NOT_EQUAL(0, wheels[1]);
  TEST_ASSERT_NOT_EQUAL(0, wheels[2]);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_forward);
  RUN_TEST(test_strafe_right);
  RUN_TEST(test_rotate_ccw);
  UNITY_END();
}

void loop() {
  // no-op
}
