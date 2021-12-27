#include "ADCReader.hpp"

const ADCReader::adc2unit_t ADCReader::adc2unit[ADC_MAX] = {
	[ADC_CURRENT_IN] = {
		.offset = -31.11894,
		.gain = 0.04245421,
	},
	[ADC_VOLTAGE_IN] = {
		.offset = -0.1009902,
		.gain = 0.02524754,
	},
	[ADC_VOLTAGE_OUT] = {
		.offset = -0.1384028,
		.gain = 0.05386298,
	},
};

ADC_MODE(ADC_TOUT_3V3)

/**
 * @return true when all ADC values were updated once
 */
bool ADCReader::update()
{
	for (int i=0; i<16; i++) {
		const float adc = analogRead(A0);

		const adc2unit_t& a2u = adc2unit[nextChannel];
		const float value = adc * a2u.gain + a2u.offset;
		const float limitedValue = (value < 0.0) ? 0.0 : value;
		values[nextChannel].update(limitedValue);
	}

	nextChannel++;
	if (nextChannel >= ADC_MAX)
		nextChannel = 0;

	for (uint8_t i=0; i<ARRAY_SIZE(ADC_CH_SEL_PINS); i++) {
		const uint32_t mask = 1 << ADC_CH_SEL_PINS[i];
		if (nextChannel & (1 << i))
			GPOS = mask;
		else
			GPOC = mask;
	}

	return (nextChannel == 0);
}

