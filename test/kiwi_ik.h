#pragma once
#include <stdint.h>

/**
 * Converts a desired Cartesian velocity (vx, vy) into
 * three wheel PWM values in the range [-127..+127].
 *
 * @param vx    X velocity (right positive) as a float
 * @param vy    Y velocity (forward positive) as a float
 * @param out   int8_t[3] array to receive {w0, w1, w2}
 */
namespace KiwiIK {
    void cartesianToWheel(float vx, float vy, int8_t out[3]);
}
