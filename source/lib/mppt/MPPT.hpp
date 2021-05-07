#ifndef __MPPT_HPP__
#define __MPPT_HPP__

#include <stdint.h>
#include <IMPPTOutput.hpp>

class MPPT {
private:
	IMPPTOutput& output;
	uint8_t pwm;
	uint32_t lastVoltage;
	uint32_t lastPower;

public:
	MPPT(IMPPTOutput& output) : output(output), pwm(0), lastVoltage(0), lastPower(0) {
		update(0, 0);
	}

	void update(const float voltage, const float power);

	float getPwmLevel() {
		return pwm * 100.0 / IMPPTOutput::maxValue;
	}
};

#endif /* __MPPT_HPP__ */

