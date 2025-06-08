#include "drv8833_hal.h"
#include "orbit_controller.h"
#include <stdio.h>

namespace {
  // Pin assignments for future hardware integration
  const uint8_t MOTOR_PINS[3][2] = {
    {3, 5},   // Motor 0: IN1, IN2 (PWM capable pins)
    {6, 9},   // Motor 1: IN1, IN2 (PWM capable pins)
    {10, 11}  // Motor 2: IN1, IN2 (PWM capable pins)
  };
  
  // Motor PWM values
  int8_t pending[3] = {0, 0, 0};
  
  // Timing for orbit controller
  unsigned long lastOrbitTime = 0;
}

void DrvHal::init() {
  Serial.begin(115200);
  
  // Future hardware initialization
  // for(int i = 0; i < 3; i++) {
  //   pinMode(MOTOR_PINS[i][0], OUTPUT);
  //   pinMode(MOTOR_PINS[i][1], OUTPUT);
  // }
  
  Serial.println("DRV8833 HAL initialized (mock mode)");
}

void DrvHal::setWheelSpeeds(const int8_t pwm[3]) {
  memcpy(pending, pwm, 3);
}

void DrvHal::updateOutputs() {
  char buf[64];
  snprintf(buf, sizeof(buf),
           "Wheels PWM = [%+4d %+4d %+4d]",
           pending[0], pending[1], pending[2]);
  Serial.println(buf);
  
  // Future hardware implementation
  // for(int i = 0; i < 3; i++) {
  //   if(pending[i] >= 0) {
  //     analogWrite(MOTOR_PINS[i][0], pending[i]);
  //     digitalWrite(MOTOR_PINS[i][1], LOW);
  //   } else {
  //     digitalWrite(MOTOR_PINS[i][0], LOW);
  //     analogWrite(MOTOR_PINS[i][1], -pending[i]);
  //   }
  // }
}

void DrvHal::spinToward(int16_t angleDeg) {
  // Calculate dt for PID controller
  unsigned long now = millis();
  double dt = (lastOrbitTime > 0) ? (now - lastOrbitTime) / 1000.0 : 0.016; // default 16ms
  
  // Use OrbitController for smooth control
  double output = OrbitController::compute(angleDeg, dt);
  int8_t pwm = constrain(output, -100, 100);
  
  // For Kiwi drive pure rotation, all wheels rotate in same direction
  int8_t v[3] = { pwm, pwm, pwm };
  setWheelSpeeds(v);
  updateOutputs();
  
  // Debug output
  char buf[80];
  snprintf(buf, sizeof(buf), "Orbit: angle=%d° dt=%.3fs output=%.1f pwm=%d", 
           angleDeg, dt, output, pwm);
  Serial.println(buf);
  
  lastOrbitTime = now;
}

void DrvHal::stop() {
  int8_t zero[3] = {0, 0, 0};
  setWheelSpeeds(zero);
  updateOutputs();
  Serial.println("Motors stopped");
}