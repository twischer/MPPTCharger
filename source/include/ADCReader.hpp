#ifndef __ADCREADER_HPP__
#define __ADCREADER_HPP__

#include <stdint.h>
#include <assert.h>
// TODO #include <Filters.h>
// TODO #include <Filters/MedianFilter.hpp>
// TODO #include <Filters/SMA.hpp>

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

enum ADC_CHANNEL {
	ADC_CURRENT_IN,
	ADC_VOLTAGE_IN,
	ADC_VOLTAGE_OUT,
	ADC_MAX
};

static const uint8_t ADC_CH_SEL_PINS[] = {0, 14, 15};

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
		assert( ADC_MAX < (1 << ARRAY_SIZE(ADC_CH_SEL_PINS)) );

		for (uint8_t i=0; i<ARRAY_SIZE(ADC_CH_SEL_PINS); i++) {
			const uint8_t pin = ADC_CH_SEL_PINS[i];
			pinMode(pin, OUTPUT);
			digitalWrite(pin, LOW);
		}
	}

	bool update();

	float get(const uint8_t channel) {
		return values[channel].get();
	}
};

#endif /* __ADCREADER_HPP__ */

