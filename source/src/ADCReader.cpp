#include "ADCReader.hpp"

const ADCReader::adc2unit_t ADCReader::adc2unit[ADC_MAX] = {
	[ADC_CURRENT_IN] = {
		.offset = 0, // TODO
		.gain = 1.0,
	},
	[ADC_VOLTAGE_IN] = {
		.offset = 0, // TODO
		.gain = 1.0,
	},
	[ADC_VOLTAGE_OUT] = {
		.offset = 0, // TODO
		.gain = 1.0,
	},
};

ADC_MODE(ADC_TOUT_3V3)

/**
 * @return true when all ADC values were updated once
 */
bool ADCReader::update()
{
	const uint16_t value = analogRead(A0) + adc2unit[nextChannel].offset;
	values[nextChannel].update(value);

	nextChannel++;
	if (nextChannel >= ADC_MAX)
		nextChannel = 0;

	GPOC = ADC_CH_SEL_MASK << ADC_CH_SEL_PIN;
	GPOS = (nextChannel & ADC_CH_SEL_MASK) << ADC_CH_SEL_PIN;

	// TODO optimization: Only return true if the values have really changed
	return (nextChannel == 0);
}

