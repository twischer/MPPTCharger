#include "MPPT.hpp"

int8_t MPPT::getPwmUpdate(const uint32_t voltage, const uint32_t power)
{
	static const uint8_t PWM_UPDATE_DIFF = 1;

	/* See https://hackaday.io/project/4613/gallery */
	if (power > lastPower) {
		if (voltage > lastVoltage) {
			/* voltage increases */
			return PWM_UPDATE_DIFF;
		} else {
			/* current increases */
			return -PWM_UPDATE_DIFF;
		}
	} else {
		/* power decreses */
		if (voltage > lastVoltage) {
			/* current decresses */
			return -PWM_UPDATE_DIFF;
		} else {
			/* voltage decresses */
			return PWM_UPDATE_DIFF;
		}
	}
}

void MPPT::update(const uint32_t voltage, const uint32_t power)
{
	const int8_t updateVal = getPwmUpdate(voltage, power);

	if (pwm <= -updateVal)
		pwm = 0;
	else if ( pwm >= (maxPWM - updateVal) )
		pwm = maxPWM;
	else
		pwm += updateVal;

	/* inverted logic */
	sigmaDeltaWrite(0, maxPWM - pwm);

	lastVoltage = voltage;
	lastPower = power;
}

