#ifndef __ADCCALC_HPP__
#define __ADCCALC_HPP__
#include "ADCReader.hpp"

enum ADC_CALC {
	ADC_POWER_IN = ADC_MAX,
	ADC_ENERGY_IN,
};

class ADCCalc : public ADCReader {
private:
	static const float powerGain;
	static const uint32_t MS_PER_H = 60 * 60 * 1000;
	static const float energyGain;


	unsigned long lastUpdateTime;
	uint32_t power;
	float energy;

    public:
	ADCCalc() : lastUpdateTime(0), power(0), energy(0.0)
	{
	}

	bool update() {
		if (!ADCReader::update())
			/* nothing to do */
			return false;

		power = get(ADC_VOLTAGE_IN) * get(ADC_CURRENT_IN);

		const unsigned long now = millis();
		if (lastUpdateTime > 0) {
			const uint64_t diffTime = now - lastUpdateTime;
			energy += static_cast<float>(power) * diffTime;
		}

		lastUpdateTime = now;
		return true;
	}

	uint32_t get(const uint8_t channel) {
		if (channel < ADC_MAX)
			return ADCReader::get(channel);
		else if (channel == ADC_POWER_IN) {
			return power;
		} else if (channel == ADC_ENERGY_IN) {
			return energy;
		} else
			return 0;
	}

	float getInUnit(const uint8_t channel) {
		if (channel < ADC_MAX)
			return ADCReader::getInUnit(channel);
		else if (channel == ADC_POWER_IN) {
			/* in W */
			return power * powerGain;
		} else if (channel == ADC_ENERGY_IN) {
			/* in Wh */
			return energy * energyGain;
		} else
			return 0;
	}
};

#endif /* __ADCCALC_HPP__ */

