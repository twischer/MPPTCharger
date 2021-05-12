#ifndef __MPPT_PWM_OUTPUT_HPP__
#define __MPPT_PWM_OUTPUT_HPP__

#include <Arduino.h>
#include <IMPPTOutput.hpp>

class MPPTPWMOutput : public IMPPTOutput {
private:
	const uint8_t outPin;

public:
	MPPTPWMOutput(const uint8_t outPin) : outPin(outPin) {
		pwmSetMode(PWM_MODE_MS);
		/* pwmFrequency in Hz = 19.2e6 Hz / pwmClock / pwmRange */
		// TODO set frequency lower than XL4016 internal frequency
		//pwmSetClock() seems to go from 2 to 4095
		pwmSetRange(maxValue);
		/* inverted logic: high value reduces PWM output voltage of TL494 */
		pwmWrite( outPin, maxValue);
		pinMode(outPin, PWM_OUTPUT);
	}

	virtual void write(uint8_t value) override {
		/* inverted logic */
		pwmWrite(outPin, maxValue - value);
	}
};
#endif /* __MPPT_PWM_OUTPUT_HPP__ */

