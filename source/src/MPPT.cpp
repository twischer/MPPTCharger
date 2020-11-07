#include "MPPT.hpp"

void MPPT::update(const uint32_t voltage, const uint32_t power)
{
	static const uint8_t PWM_UPDATE_DIFF = 1;
	// TODO move to class
	static bool increase = true;

	// TODO use voltage value e.g. 10 V check what is reasonabel for TL494
	// Use only bool input e.g. minVoltageExceeded
	if (voltage < 600 || pwm > (maxPWM - PWM_UPDATE_DIFF))
		increase = false;
	else if (pwm < PWM_UPDATE_DIFF)
		increase = true;
	/* compensate ADC uncertainty */
	// circa 1W bzw vielleicht prozentual
	else if ( (power + 10) < lastPower) {
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

	lastVoltage = voltage; // TODO Not required any longer
}

