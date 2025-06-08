#pragma once

namespace OrbitController {

/**
 * Initialize PID gains.
 * @param kp Proportional gain
 * @param ki Integral gain
 * @param kd Derivative gain
 */
void init(double kp, double ki, double kd);

/**
 * Compute control output for a given error and timestep.
 * @param error Error term (e.g. angle error in degrees)
 * @param dt    Time since last call, in seconds
 * @return PID output
 */
double compute(double error, double dt);

}  // namespace OrbitController
