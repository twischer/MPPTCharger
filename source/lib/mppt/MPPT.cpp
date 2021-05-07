#include "MPPT.hpp"
#include "IMPPTOutput.hpp"

void MPPT::update(const float voltage, const float power)
{
	static const uint8_t PWM_UPDATE_DIFF = 1;
	// TODO move to class
	static bool increase = true;

	/* always 1 W + 3% of current power */
	const float hysteresis = 0.1;

	if (voltage < 10.0 || pwm > (IMPPTOutput::maxValue - PWM_UPDATE_DIFF))
		increase = false;
	else if (pwm < PWM_UPDATE_DIFF)
		increase = true;
	/* compensate ADC uncertainty. TODO Search for diff >15% */
	else if ( (power + hysteresis) < lastPower) {
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

	output.write(pwm);
}

