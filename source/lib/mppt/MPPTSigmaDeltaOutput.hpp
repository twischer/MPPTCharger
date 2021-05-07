#ifndef __MPPT_SIGMA_DELTA_OUTPUT_HPP__
#define __MPPT_SIGMA_DELTA_OUTPUT_HPP__

#include <IMPPTOutput.hpp>
#include <sigma_delta.h>

class MPPTSigmaDeltaOutput : public IMPPTOutput {
private:
	const uint8_t sigmaDeltaChannel;

public:
	MPPTSigmaDeltaOutput(const uint8_t outPin, const uint8_t sigmaDeltaChannel = 0) :
		sigmaDeltaChannel(sigmaDeltaChannel) {
		/* do not use max of 312,5kHz due to ESP freezes near by 128 -> 40MHz */
		sigmaDeltaSetup(sigmaDeltaChannel, 19531);
		/* inverted logic: high value reduces PWM output voltage of TL494 */
		sigmaDeltaWrite(sigmaDeltaChannel, maxValue);
		sigmaDeltaAttachPin(outPin, sigmaDeltaChannel);
	}

	virtual void write(uint8_t value) override {
		/* inverted logic */
		sigmaDeltaWrite(sigmaDeltaChannel, maxValue - value);
	}
};
#endif /* __MPPT_SIGMA_DELTA_OUTPUT_HPP__ */

