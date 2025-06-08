#pragma once
#include <stdint.h>

/**
 * Kiwi Drive Inverse Kinematics
 * 
 * Converts desired robot velocity to individual wheel speeds
 * for a 3-wheel omni-directional (kiwi) drive configuration.
 */
namespace KiwiIK {

/**
 * Convert Cartesian velocity to wheel PWM values.
 * 
 * @param vx    X velocity (right positive) in range [-1.0, 1.0]
 * @param vy    Y velocity (forward positive) in range [-1.0, 1.0]
 * @param out   Output array for 3 wheel PWM values [-127, 127]
 */
void cartesianToWheel(float vx, float vy, int8_t out[3]);

/**
 * Convert Cartesian velocity + rotation to wheel PWM values.
 * 
 * @param vx    X velocity (right positive) in range [-1.0, 1.0]
 * @param vy    Y velocity (forward positive) in range [-1.0, 1.0]
 * @param vr    Rotational velocity (CCW positive) in range [-1.0, 1.0]
 * @param out   Output array for 3 wheel PWM values [-127, 127]
 */
void cartesianToWheelWithRotation(float vx, float vy, float vr, int8_t out[3]);

} // namespace KiwiIK