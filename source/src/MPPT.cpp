#include "MPPT.hpp"

void MPPT::update(const float voltage, const float power)
{
	static const uint8_t PWM_UPDATE_DIFF = 1;
	// TODO move to class
	static bool increase = true;

	if (voltage < 10.0 || pwm > (maxPWM - PWM_UPDATE_DIFF))
		increase = false;
	else if (pwm < PWM_UPDATE_DIFF)
		increase = true;
	/* compensate ADC uncertainty. TODO Search for diff >15% */
	else if ( (power + 1.5) < lastPower) {
		increase = !increase;
		lastPower = power;
	} else if (power > lastPower)
		lastPower = power;

	if (increase)
		pwm += PWM_UPDATE_DIFF;
	else {
		/* required in case voltage to small */
		if (pwm <= PWM_UPDATE_DIFF)
			pwm = 0;
		else
			pwm -= PWM_UPDATE_DIFF;
	}

	/* inverted logic */
	sigmaDeltaWrite(SIGMA_DELTA_CHANNEL, maxPWM - pwm);
}

