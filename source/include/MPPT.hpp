#ifndef __MPPT_HPP__
#define __MPPT_HPP__

#include <stdint.h>
#include <sigma_delta.h>

class MPPT {
private:
	static const uint8_t SIGMA_DELTA_CHANNEL = 0;
	static const uint8_t maxPWM = 0xFF;

	uint8_t outPin;
	uint8_t pwm;
	uint32_t lastVoltage;
	uint32_t lastPower;

	int8_t getPwmUpdate(const uint32_t voltage, const uint32_t power);

public:
	MPPT(const uint8_t outPin) : outPin(outPin), pwm(0), lastVoltage(0), lastPower(0) {
		/* do not use max of 312,5kHz due to ESP freezes near by 128 -> 40MHz */
		sigmaDeltaSetup(SIGMA_DELTA_CHANNEL, 156250);
		/* inverted logic: high value reduces PWM output voltage of TL494 */
		sigmaDeltaWrite(SIGMA_DELTA_CHANNEL, maxPWM);
		sigmaDeltaAttachPin(outPin, SIGMA_DELTA_CHANNEL);

		update(0, 0);
	}

	void update(const uint32_t voltage, const uint32_t power);

	float getPwmLevel() {
		return pwm * 100.0 / maxPWM;
	}
};

#endif /* __MPPT_HPP__ */

