#include "cam_mux.h"
#include <SoftwareSerial.h>

namespace {
// ----- LEFT (hardware) -----
volatile int16_t l_ang = 0, l_rng = 32767;
uint8_t  l_buf[4]; uint8_t l_idx = 0;

// ----- RIGHT (software) -----
SoftwareSerial rSer;                 // constructed later
volatile int16_t r_ang = 0, r_rng = 32767;
uint8_t  r_buf[4]; uint8_t r_idx = 0;
}

void CamMux::init(uint32_t baudL, uint32_t baudR, uint8_t rRxPin) {
  Serial.begin(baudL);
  rSer.~SoftwareSerial();            // destroy default
  new (&rSer) SoftwareSerial(rRxPin, /*TX unused*/255); // placement-new
  rSer.begin(baudR);
}

static inline void pushByte(uint8_t byte, uint8_t &idx, uint8_t *buf,
                            volatile int16_t &ang, volatile int16_t &rng) {
  buf[idx++] = byte;
  if (idx == 4) {
    ang = (int16_t)(buf[0] | (buf[1] << 8));
    rng = (int16_t)(buf[2] | (buf[3] << 8));
    idx = 0;
  }
}

void CamMux::poll() {
  while (Serial.available())   pushByte(Serial.read(),  l_idx, l_buf, l_ang, l_rng);
  while (rSer.available())     pushByte(rSer.read(),    r_idx, r_buf, r_ang, r_rng);
}

/* getters */
int16_t CamMux::leftAngle()      { return l_ang; }
int16_t CamMux::leftRange()      { return l_rng; }
int16_t CamMux::rightAngle()     { return r_ang; }
int16_t CamMux::rightRange()     { return r_rng; }
int16_t CamMux::selectedAngle()  { return (l_rng <= r_rng) ? l_ang : r_ang; }
int16_t CamMux::selectedRange()  { return (l_rng <= r_rng) ? l_rng : r_rng; }
