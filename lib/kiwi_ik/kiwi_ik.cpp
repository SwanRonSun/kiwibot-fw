#include "kiwi_ik.h"
#include <math.h>

namespace KiwiIK {

// Kiwi drive configuration
// Wheel 0: Front wheel at 90° (pointing forward)
// Wheel 1: Rear-left at 210° 
// Wheel 2: Rear-right at 330°
static const float WHEEL_ANGLES[3] = {
    90.0f * M_PI / 180.0f,   // Front
    210.0f * M_PI / 180.0f,  // Rear-left  
    330.0f * M_PI / 180.0f   // Rear-right
};

void cartesianToWheel(float vx, float vy, int8_t out[3]) {
    // Normalize input velocity to unit circle
    float mag = sqrtf(vx * vx + vy * vy);
    if (mag > 1.0f) {
        vx /= mag;
        vy /= mag;
    }
    
    // Calculate wheel velocities using inverse kinematics
    // For each wheel: v_wheel = -sin(θ) * vx + cos(θ) * vy
    float wheelVel[3];
    for (int i = 0; i < 3; i++) {
        wheelVel[i] = -sinf(WHEEL_ANGLES[i]) * vx + cosf(WHEEL_ANGLES[i]) * vy;
    }
    
    // Find max velocity for scaling
    float maxVel = 0.0f;
    for (int i = 0; i < 3; i++) {
        float absVel = fabsf(wheelVel[i]);
        if (absVel > maxVel) maxVel = absVel;
    }
    
    // Scale to PWM range [-127, 127]
    if (maxVel > 0.0f) {
        float scale = 127.0f / maxVel;
        for (int i = 0; i < 3; i++) {
            out[i] = (int8_t)(wheelVel[i] * scale);
        }
    } else {
        out[0] = out[1] = out[2] = 0;
    }
}

void cartesianToWheelWithRotation(float vx, float vy, float vr, int8_t out[3]) {
    // Normalize translation velocity
    float mag = sqrtf(vx * vx + vy * vy);
    if (mag > 1.0f) {
        vx /= mag;
        vy /= mag;
    }
    
    // Clamp rotation
    if (vr > 1.0f) vr = 1.0f;
    if (vr < -1.0f) vr = -1.0f;
    
    // Calculate wheel velocities
    // Translation: v_trans = -sin(θ) * vx + cos(θ) * vy
    // Rotation: v_rot = R * vr (where R is robot radius, normalized to 1)
    float wheelVel[3];
    for (int i = 0; i < 3; i++) {
        float v_trans = -sinf(WHEEL_ANGLES[i]) * vx + cosf(WHEEL_ANGLES[i]) * vy;
        float v_rot = vr; // All wheels contribute equally to rotation
        wheelVel[i] = v_trans + v_rot;
    }
    
    // Find max velocity for scaling
    float maxVel = 0.0f;
    for (int i = 0; i < 3; i++) {
        float absVel = fabsf(wheelVel[i]);
        if (absVel > maxVel) maxVel = absVel;
    }
    
    // Scale to PWM range [-127, 127]
    if (maxVel > 0.0f) {
        float scale = 127.0f / maxVel;
        for (int i = 0; i < 3; i++) {
            out[i] = (int8_t)(wheelVel[i] * scale);
        }
    } else {
        out[0] = out[1] = out[2] = 0;
    }
}

} // namespace KiwiIK