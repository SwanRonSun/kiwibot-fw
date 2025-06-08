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
 * Must be called every loop() to update cooldown timer.
 */
void poll();
  
} // namespace StrikeHal
