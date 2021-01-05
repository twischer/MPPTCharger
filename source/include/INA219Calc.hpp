#ifndef _INA219CALC_HPP_
#define _INA219CALC_HPP_

#include <Arduino.h>
#include <INA219.h>

class INA219Calc : public INA219 {
private:
	uint64_t lastTime;
	float lastPower;
	float energyWh;

	/* integrate graph between (x1,y1) and (x2,y2) with trapezoid rule */
	float integrate(uint64_t x1, uint64_t x2, float y1, float y2) {
		return (uint32_t)(x2 - x1) * (y2 + y1) / 2.0;
	}

public:
	INA219Calc() : INA219(), lastTime(0), lastPower(0.0), energyWh(0.0) {}

	float busPower() {
		const float power = INA219::busPower();

		const uint64_t now = micros64();
		if (lastTime > 0) {
			const float energyWus = integrate(lastTime, now, lastPower, power);
			const float energyWs = energyWus / 1000000.0;
			energyWh += energyWs / 3600.0;
		}
		lastTime = now;
		lastPower = power;

		return power;
	}

	float busEnergy() const {
		return energyWh;
	}
};

#endif /* _INA219CALC_HPP_ */

