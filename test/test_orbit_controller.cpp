#include <unity.h>
#include "orbit_controller.h"

// Test zero error → zero output
void test_zero_error() {
  OrbitController::init(1.0, 1.0, 1.0);
  double out = OrbitController::compute(0.0, 1.0);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, out);
}

// Test pure proportional action
void test_proportional() {
  OrbitController::init(2.0, 0.0, 0.0);
  double out = OrbitController::compute(1.5, 1.0);
  TEST_ASSERT_EQUAL_DOUBLE(3.0, out);
}

// Test integral accumulation over two steps
void test_integral() {
  OrbitController::init(0.0, 1.0, 0.0);
  double out1 = OrbitController::compute(1.0, 2.0);  // integral = 2
  TEST_ASSERT_EQUAL_DOUBLE(2.0, out1);
  double out2 = OrbitController::compute(1.0, 3.0);  // integral = 5
  TEST_ASSERT_EQUAL_DOUBLE(5.0, out2);
}

// Test derivative action over two steps
void test_derivative() {
  OrbitController::init(0.0, 0.0, 1.0);
  double out1 = OrbitController::compute(2.0, 1.0);  // derivative = 2
  TEST_ASSERT_EQUAL_DOUBLE(2.0, out1);
  double out2 = OrbitController::compute(4.0, 2.0);  // derivative = (4-2)/2 = 1
  TEST_ASSERT_EQUAL_DOUBLE(1.0, out2);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_zero_error);
  RUN_TEST(test_proportional);
  RUN_TEST(test_integral);
  RUN_TEST(test_derivative);
  return UNITY_END();
}
