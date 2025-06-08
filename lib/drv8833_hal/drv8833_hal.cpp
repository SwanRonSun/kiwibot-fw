#include "drv8833_hal.h"

static int8_t pending[3] = {0,0,0};

void DrvHal::init() {
  Serial.begin(115200);                // console output
}

void DrvHal::setWheelSpeeds(const int8_t pwm[3]) {
  memcpy(pending, pwm, 3);
}

void DrvHal::updateOutputs() {
  char buf[40];
	snprintf(buf, sizeof(buf),
        	"Wheels PWM = [%+4d %+4d %+4d]\n",
         pending[0], pending[1], pending[2]);
Serial.print(buf);        // works on every Arduino core

}

void DrvHal::stop() {
  int8_t z[3] = {0,0,0};
  setWheelSpeeds(z);
  updateOutputs();
}
