/*
 * cam_mux.cpp – dual-camera packet parser with validation
 */

#include "cam_mux.h"
#include <SoftwareSerial.h>

namespace {

// Constants
const int16_t MAX_VALID_RANGE = 2000;    // 2 meters max
const int16_t MIN_VALID_RANGE = 50;      // 5 cm min
const uint32_t PACKET_TIMEOUT_MS = 500;  // Consider camera offline after 500ms

// LEFT camera (hardware Serial) state
volatile int16_t l_ang = 0, l_rng = 32767;
uint8_t  l_buf[4];
uint8_t  l_idx = 0;
uint32_t l_lastPacket = 0;
uint32_t l_packetsReceived = 0;
uint32_t l_errorsDetected = 0;

// RIGHT camera (SoftwareSerial) state
static SoftwareSerial* rSer = nullptr;
volatile int16_t r_ang = 0, r_rng = 32767;
uint8_t  r_buf[4];
uint8_t  r_idx = 0;
uint32_t r_lastPacket = 0;
uint32_t r_packetsReceived = 0;
uint32_t r_errorsDetected = 0;

// Validate and store packet data
inline bool validateAndStore(int16_t angle, int16_t range, 
                           volatile int16_t& ang, volatile int16_t& rng,
                           uint32_t& lastPacket, uint32_t& packets, uint32_t& errors) {
    // Validate ranges
    if (angle < -180 || angle > 180) {
        errors++;
        return false;
    }
    if (range < MIN_VALID_RANGE || range > MAX_VALID_RANGE) {
        errors++;
        return false;
    }
    
    // Store valid data
    ang = angle;
    rng = range;
    lastPacket = millis();
    packets++;
    return true;
}

// Push one byte into buffer; once 4 bytes are in, validate and unpack
inline void pushByte(uint8_t b, uint8_t& idx, uint8_t* buf,
                     volatile int16_t& ang, volatile int16_t& rng,
                     uint32_t& lastPacket, uint32_t& packets, uint32_t& errors) {
    buf[idx++] = b;
    if (idx == 4) {
        int16_t newAngle = static_cast<int16_t>(buf[0] | (buf[1] << 8));
        int16_t newRange = static_cast<int16_t>(buf[2] | (buf[3] << 8));
        validateAndStore(newAngle, newRange, ang, rng, lastPacket, packets, errors);
        idx = 0;
    }
}

// Check if camera has timed out
inline bool isTimedOut(uint32_t lastPacket) {
    return (millis() - lastPacket) > PACKET_TIMEOUT_MS;
}

}  // end anonymous namespace

// Initialize left (Serial) and right (SoftwareSerial) cameras
void CamMux::init(uint32_t baudLeft,
                  uint32_t baudRight,
                  uint8_t  rightRxPin) {
    Serial.begin(baudLeft);
    Serial.println("CamMux: Left camera on HW Serial");
    
    rSer = new SoftwareSerial(rightRxPin, 255);  // RX only
    rSer->begin(baudRight);
    
    char buf[64];
    snprintf(buf, sizeof(buf), "CamMux: Right camera on pin D%d @ %lu baud", 
             rightRxPin, baudRight);
    Serial.println(buf);
    
    // Initialize timestamps
    l_lastPacket = millis();
    r_lastPacket = millis();
}

// Poll both streams for new packets
void CamMux::poll() {
    // Process left camera
    while (Serial.available()) {
        pushByte(Serial.read(), l_idx, l_buf, l_ang, l_rng, 
                l_lastPacket, l_packetsReceived, l_errorsDetected);
    }
    
    // Process right camera
    if (rSer) {
        while (rSer->available()) {
            pushByte(rSer->read(), r_idx, r_buf, r_ang, r_rng,
                    r_lastPacket, r_packetsReceived, r_errorsDetected);
        }
    }
}

// Accessors with timeout handling
int16_t CamMux::leftAngle() {
    return isTimedOut(l_lastPacket) ? 0 : l_ang;
}

int16_t CamMux::leftRange() {
    return isTimedOut(l_lastPacket) ? 32767 : l_rng;
}

int16_t CamMux::rightAngle() {
    return isTimedOut(r_lastPacket) ? 0 : r_ang;
}

int16_t CamMux::rightRange() {
    return isTimedOut(r_lastPacket) ? 32767 : r_rng;
}

int16_t CamMux::selectedAngle() {
    int16_t lRange = leftRange();
    int16_t rRange = rightRange();
    return (lRange <= rRange) ? leftAngle() : rightAngle();
}

int16_t CamMux::selectedRange() {
    int16_t lRange = leftRange();
    int16_t rRange = rightRange();
    return (lRange <= rRange) ? lRange : rRange;
}

// Debug statistics
void CamMux::printStats() {
    char buf[128];
    snprintf(buf, sizeof(buf), 
             "CamMux Stats - L: %lu pkts, %lu errs, %s | R: %lu pkts, %lu errs, %s",
             l_packetsReceived, l_errorsDetected, 
             isTimedOut(l_lastPacket) ? "TIMEOUT" : "OK",
             r_packetsReceived, r_errorsDetected,
             isTimedOut(r_lastPacket) ? "TIMEOUT" : "OK");
    Serial.println(buf);
}

//────────────────────────────────────────────────────────────────
// TEST-ONLY hook: override left-camera values directly for tests
//────────────────────────────────────────────────────────────────
void CamMux::_test_setLeft(int16_t angle, int16_t range) {
    l_ang = angle;
    l_rng = range;
    l_lastPacket = millis();  // Reset timeout
}