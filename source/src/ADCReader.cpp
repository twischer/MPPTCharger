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
		.offset = -0.2693149,
		.gain = 0.05386298,
	},
};

ADC_MODE(ADC_TOUT_3V3)

/**
 * @return true when all ADC values were updated once
 */
bool ADCReader::update()
{
	const float adc = analogRead(A0);

	const adc2unit_t& a2u = adc2unit[nextChannel];
	const float value = adc * a2u.gain + a2u.offset;
	const float limitedValue = (value < 0.0) ? 0.0 : value;
	values[nextChannel].update(limitedValue);

	nextChannel++;
	if (nextChannel >= ADC_MAX)
		nextChannel = 0;

	GPOC = ADC_CH_SEL_MASK << ADC_CH_SEL_PIN;
	GPOS = (nextChannel & ADC_CH_SEL_MASK) << ADC_CH_SEL_PIN;

	return (nextChannel == 0);
}

