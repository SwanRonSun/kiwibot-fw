#include "strike_hal.h"

namespace StrikeHal {

static const uint32_t COOLDOWN_MS = 500;  // half-second between strikes
static uint32_t lastStrikeTime = 0;
static bool ready = false;

void init() {
  ready = true;
  lastStrikeTime = millis() - COOLDOWN_MS;
}

bool fire() {
  if (!ready) return false;
  uint32_t now = millis();
  if (now - lastStrikeTime >= COOLDOWN_MS) {
    Serial.println(">>> BANG! <<<");
    lastStrikeTime = now;
    return true;
  }
  return false;
}

void poll() {
  // nothing else to do now; cooldown managed in fire()
}

} // namespace StrikeHal
