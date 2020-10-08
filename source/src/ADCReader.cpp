#include "ADCReader.hpp"

ADC_MODE(ADC_TOUT_3V3)

void ADCReader::update()
{
	const uint16_t value = analogRead(A0);
	values[nextChannel].update(value);

	nextChannel++;
	if (nextChannel >= ADC_MAX)
		nextChannel = 0;

	GPOC = ADC_CH_SEL_MASK << ADC_CH_SEL_PIN;
	GPOS = (nextChannel & ADC_CH_SEL_MASK) << ADC_CH_SEL_PIN;
}

