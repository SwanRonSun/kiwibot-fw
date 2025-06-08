#include "drv8833_hal.h"
#include <stdio.h>            // for snprintf

static int8_t pending[3] = {0, 0, 0};

void DrvHal::init() {
  Serial.begin(115200);
}

void DrvHal::setWheelSpeeds(const int8_t pwm[3]) {
  memcpy(pending, pwm, 3);
}

void DrvHal::updateOutputs() {
  char buf[48];
  snprintf(buf, sizeof(buf),
           "Wheels PWM = [%+4d %+4d %+4d]\n",
           pending[0], pending[1], pending[2]);
  Serial.print(buf);
}

void DrvHal::spinToward(int16_t angleDeg) {
  // Simple proportional turn: full speed at |180°|, zero at 0°
  int8_t pwm = (int8_t)constrain(angleDeg / 2, -100, 100);  // scale gain = 0.5
  int8_t v[3] = { pwm, pwm, pwm };  // all wheels same sign → pure rotation
  setWheelSpeeds(v);
  updateOutputs();
}


/*  <<< THIS BLOCK IS THE ONE THE LINKER IS COMPLAINING ABOUT >>> */
void DrvHal::stop() {
  int8_t zero[3] = {0, 0, 0};
  setWheelSpeeds(zero);
  updateOutputs();
}
