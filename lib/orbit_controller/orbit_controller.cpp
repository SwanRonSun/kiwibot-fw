#include "orbit_controller.h"

namespace {
  double Kp = 0.0, Ki = 0.0, Kd = 0.0;
  double integral = 0.0;
  double prevError = 0.0;
}

void OrbitController::init(double p, double i, double d) {
  Kp = p; Ki = i; Kd = d;
  integral = 0.0;
  prevError = 0.0;
}

double OrbitController::compute(double error, double dt) {
  integral += error * dt;
  double derivative = (dt > 0.0) ? (error - prevError) / dt : 0.0;
  double output = Kp * error + Ki * integral + Kd * derivative;
  prevError = error;
  return output;
}
