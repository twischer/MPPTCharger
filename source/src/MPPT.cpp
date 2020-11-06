#include "MPPT.hpp"

void MPPT::update(const uint32_t voltage, const uint32_t power)
{
	static const uint8_t PWM_UPDATE_DIFF = 1;
	// TODO move to class
	static bool increase = true;

	if (pwm < PWM_UPDATE_DIFF)
		increase = true;
	// TODO use voltage value e.g. 10 V check what is reasonabel for TL494
	// Use only bool input e.g. minVoltageExceeded
	else if (voltage < 600 || pwm > (maxPWM - PWM_UPDATE_DIFF))
		increase = false;
	/* compensate ADC uncertainty */
	else if ( (power + 10) < lastPower )
		increase = !increase;

	if (increase)
		pwm += PWM_UPDATE_DIFF;
	else
		pwm -= PWM_UPDATE_DIFF;

	/* inverted logic */
	sigmaDeltaWrite(SIGMA_DELTA_CHANNEL, maxPWM - pwm);

	lastVoltage = voltage; // TODO Not required any longer
	lastPower = power;
}

