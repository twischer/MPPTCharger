#include "ADCCalc.hpp"

const float ADCCalc::powerGain = adc2unit[ADC_VOLTAGE_IN].gain *
	adc2unit[ADC_CURRENT_IN].gain;
const float ADCCalc::energyGain = powerGain / MS_PER_H;

