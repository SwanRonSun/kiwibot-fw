#include "cam_mux.h"

namespace {
  // simple 4-byte ring buffer
  uint8_t  buf[4];
  uint8_t  idx = 0;

  volatile int16_t angle = 0;
  volatile int16_t range = 0;
}

void CamMux::init(uint32_t baud_left) {
  Serial.begin(baud_left);     // using RX0 only for now
}

void CamMux::poll() {
  while (Serial.available()) {
    buf[idx++] = Serial.read();
    if (idx == 4) {
      // little-endian  int16_t angle, int16_t range
      angle =  (int16_t)(buf[0] | (buf[1] << 8));
      range =  (int16_t)(buf[2] | (buf[3] << 8));
      idx = 0;                 // reset for next packet
    }
  }
}

int16_t CamMux::latestAngle() { return angle; }
int16_t CamMux::latestRange() { return range; }
