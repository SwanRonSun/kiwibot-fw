#pragma once
#include <Arduino.h>

namespace CamMux {

/**
 * Initialize both camera streams.
 *
 * Left camera uses hardware Serial (RX0) at a default of 115200 baud.
 * Right camera uses SoftwareSerial on the specified pin at a default of 250000 baud.
 *
 * @param baud_left    Baud rate for the left (hardware) serial port.
 * @param baud_right   Baud rate for the right (software) serial port.
 * @param right_rx_pin Digital pin number to use as RX for the right camera.
 */
void init(uint32_t baud_left   = 115200,
          uint32_t baud_right  = 250000,
          uint8_t  right_rx_pin = 2);

/** Must be called in loop(): reads and parses any incoming 4-byte packets. */
void poll();

/** Get the latest packet values from each camera. */
int16_t leftAngle();    // degrees, –180 … +180
int16_t leftRange();    // millimeters
int16_t rightAngle();
int16_t rightRange();

/**
 * Of the two cameras, which one has the nearer range?
 * Returns the corresponding angle or range.
 */
int16_t selectedAngle();
int16_t selectedRange();

/**
 * TEST-ONLY: override the left-camera packet values directly.
 * Allows unit tests to drive state logic without needing externs.
 */
void _test_setLeft(int16_t angle, int16_t range);

}  // namespace CamMux
