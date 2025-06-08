#pragma once
#include <Arduino.h>

namespace StrikeHal {

/**
 * Initialize the strike system. Call once in setup().
 */
void init();

/**
 * Fire the spike. Only fires if not in cooldown.
 * Returns true if the strike was actually triggered.
 */
bool fire();

/**
 * Must be called every loop() to update cooldown timer
 * and handle strike pulse duration.
 */
void poll();

/**
 * Check if the strike system is ready to fire
 * (not in cooldown and not currently striking).
 */
bool isReady();

/**
 * Get total number of strikes executed since boot.
 */
uint32_t getStrikeCount();
  
} // namespace StrikeHal