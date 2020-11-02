#include "ADCReader.hpp"

const ADCReader::adc2unit_t ADCReader::adc2unit[ADC_MAX] = {
	[ADC_CURRENT_IN] = {
		.offset = -733,
		.gain = 0.04245421,
	},
	[ADC_VOLTAGE_IN] = {
		.offset = -4,
		.gain = 0.02524754,
	},
	[ADC_VOLTAGE_OUT] = {
		.offset = -5,
		.gain = 0.05386298,
	},
};

ADC_MODE(ADC_TOUT_3V3)

/**
 * @return true when all ADC values were updated once
 */
bool ADCReader::update()
{
	const uint16_t adc = analogRead(A0);
	const int16_t offset = adc2unit[nextChannel].offset;

	const uint16_t value = (adc > -offset) ? (adc + offset) : 0;
	values[nextChannel].update(value);

	nextChannel++;
	if (nextChannel >= ADC_MAX)
		nextChannel = 0;

	GPOC = ADC_CH_SEL_MASK << ADC_CH_SEL_PIN;
	GPOS = (nextChannel & ADC_CH_SEL_MASK) << ADC_CH_SEL_PIN;

	// TODO optimization: Only return true if the values have really changed
	return (nextChannel == 0);
}

