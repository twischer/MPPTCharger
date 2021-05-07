#ifndef __MPPT_PWM_OUTPUT_HPP__
#define __MPPT_PWM_OUTPUT_HPP__

#include <Arduino.h>
#include <IMPPTOutput.hpp>

class MPPTPWMOutput : public IMPPTOutput {
private:
	const uint8_t outPin;

public:
	MPPTPWMOutput(const uint8_t outPin) : outPin(outPin) {
		/* inverted logic: high value reduces PWM output voltage of TL494 */
		pwmWrite(outPin, maxValue);
	}

	virtual void write(uint8_t value) override {
		/* inverted logic */
		// TODO check range of wiring PI PWM
		pwmWrite(outPin, maxValue - value);
	}
};
#endif /* __MPPT_PWM_OUTPUT_HPP__ */

