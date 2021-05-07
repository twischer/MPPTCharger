#ifndef __IMPPTOUTPUT_HPP__
#define __IMPPTOUTPUT_HPP__

#include <stdint.h>

class IMPPTOutput {
public:
	static const uint8_t maxValue = 0xFF;

	virtual void write(uint8_t value) = 0;
};
#endif /* __IMPPTOUTPUT_HPP__ */

