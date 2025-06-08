/*
 * cam_mux.cpp – dual-camera packet parser (always includes test hook)
 */

#include "cam_mux.h"
#include <SoftwareSerial.h>

namespace {

// ← LEFT camera (hardware Serial) globals
volatile int16_t l_ang = 0, l_rng = 32767;
uint8_t  l_buf[4];
uint8_t  l_idx = 0;

// ← RIGHT camera (SoftwareSerial) globals
static SoftwareSerial* rSer = nullptr;
volatile int16_t r_ang = 0, r_rng = 32767;
uint8_t  r_buf[4];
uint8_t  r_idx = 0;

// Push one byte into buf; once 4 bytes are in, unpack into (ang, rng)
inline void pushByte(uint8_t b, uint8_t& idx, uint8_t* buf,
                     volatile int16_t& ang, volatile int16_t& rng) {
    buf[idx++] = b;
    if (idx == 4) {
        ang = static_cast<int16_t>(buf[0] | (buf[1] << 8));
        rng = static_cast<int16_t>(buf[2] | (buf[3] << 8));
        idx = 0;
    }
}

}  // end anonymous namespace

// Initialize left (Serial) and right (SoftwareSerial) cameras
void CamMux::init(uint32_t baudLeft,
                  uint32_t baudRight,
                  uint8_t  rightRxPin) {
    Serial.begin(baudLeft);
    rSer = new SoftwareSerial(rightRxPin, 255);  // RX only
    rSer->begin(baudRight);
}

// Poll both streams for new packets
void CamMux::poll() {
    while (Serial.available()) {
        pushByte(Serial.read(), l_idx, l_buf, l_ang, l_rng);
    }
    if (rSer) {
        while (rSer->available()) {
            pushByte(rSer->read(), r_idx, r_buf, r_ang, r_rng);
        }
    }
}

// Accessors
int16_t CamMux::leftAngle()     { return l_ang; }
int16_t CamMux::leftRange()     { return l_rng; }
int16_t CamMux::rightAngle()    { return r_ang; }
int16_t CamMux::rightRange()    { return r_rng; }

int16_t CamMux::selectedAngle() {
    return (l_rng <= r_rng) ? l_ang : r_ang;
}
int16_t CamMux::selectedRange() {
    return (l_rng <= r_rng) ? l_rng : r_rng;
}

//────────────────────────────────────────────────────────────────
// TEST-ONLY hook: override left-camera values directly for tests
//────────────────────────────────────────────────────────────────
void CamMux::_test_setLeft(int16_t angle, int16_t range) {
    l_ang = angle;
    l_rng = range;
}
