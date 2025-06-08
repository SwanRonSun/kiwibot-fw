#include "strike_hal.h"

namespace StrikeHal {

// Constants
static const uint8_t STRIKE_PIN = 7;           // Future solenoid control pin
static const uint32_t COOLDOWN_MS = 500;       // Half-second between strikes
static const uint32_t STRIKE_DURATION_MS = 50; // 50ms pulse duration

// State
static uint32_t lastStrikeTime = 0;
static uint32_t strikeCount = 0;
static bool ready = false;
static bool striking = false;
static uint32_t strikeStartTime = 0;

void init() {
  ready = true;
  lastStrikeTime = millis() - COOLDOWN_MS;
  
  // Future hardware initialization
  // pinMode(STRIKE_PIN, OUTPUT);
  // digitalWrite(STRIKE_PIN, LOW);
  
  Serial.println("Strike system initialized (mock mode)");
}

bool fire() {
  if (!ready || striking) return false;
  
  uint32_t now = millis();
  if (now - lastStrikeTime >= COOLDOWN_MS) {
    // Start strike
    striking = true;
    strikeStartTime = now;
    lastStrikeTime = now;
    strikeCount++;
    
    // Mock output
    Serial.print(">>> BANG! <<< Strike #");
    Serial.println(strikeCount);
    
    // Future hardware implementation
    // digitalWrite(STRIKE_PIN, HIGH);
    
    return true;
  }
  return false;
}

void poll() {
  // Handle strike pulse duration
  if (striking && (millis() - strikeStartTime >= STRIKE_DURATION_MS)) {
    striking = false;
    
    // Future hardware implementation
    // digitalWrite(STRIKE_PIN, LOW);
    
    Serial.println("Strike retracted");
  }
}

bool isReady() {
  return ready && !striking && (millis() - lastStrikeTime >= COOLDOWN_MS);
}

uint32_t getStrikeCount() {
  return strikeCount;
}

} // namespace StrikeHal