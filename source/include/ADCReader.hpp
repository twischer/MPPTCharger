#ifndef __ADCREADER_HPP__
#define __ADCREADER_HPP__

#include <stdint.h>
#include <assert.h>
#include <Filters.h>
#include <Filters/MedianFilter.hpp>
#include <Filters/SMA.hpp>

enum ADC_CHANNEL {
	ADC_CURRENT_IN,
	ADC_VOLTAGE_IN,
	ADC_VOLTAGE_OUT,
	ADC_MAX
};

static const uint8_t ADC_CH_SEL_PIN	= 13;
static const uint8_t ADC_CH_SEL_PINS	= 3;
static const uint32_t ADC_CH_SEL_MASK	= (1 << ADC_CH_SEL_PINS) - 1;

class ADCReader {
private:
	class ADCFilter {
	private:
		MedianFilter<5, float> med;
		SMA<48, float, float> sma;
		float value;
	public:
		ADCFilter() : med(0.0), sma(0.0), value(0.0) {}

		void update(float input) {
			value = sma( /*med(*/input);// );
		}

		float get() {
			return value;
		}
	};

	ADCFilter values[ADC_MAX];
	uint8_t nextChannel;

	typedef struct {
		const float offset;
		const float gain;
	} adc2unit_t;

	static const adc2unit_t adc2unit[ADC_MAX];

public:
	ADCReader() : nextChannel(0) {
		assert(ADC_MAX < ADC_CH_SEL_MASK);

		for (uint8_t i=ADC_CH_SEL_PIN; i<ADC_CH_SEL_PIN+ADC_CH_SEL_PINS; i++) {
			pinMode(i, OUTPUT);
			digitalWrite(i, LOW);
		}
	}

	bool update();

	float get(const uint8_t channel) {
		return values[channel].get();
	}
};

#endif /* __ADCREADER_HPP__ */

