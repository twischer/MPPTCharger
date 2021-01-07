/**
 * Implementation found at
 * https://www.sigmdel.ca/michel/program/esp8266/arduino/watchdogs2_en.html
 */
#include <Arduino.h>
#include <Ticker.h>
#include <Esp.h>

class SoftwareWatchdog {
private:
	static const uint32_t TIMEOUT = 15*1000;

	Ticker ticker;
	uint32_t time;
	uint32_t timeout;


	void ICACHE_RAM_ATTR callback() {
		if ((millis() - time > TIMEOUT) || (timeout - time != TIMEOUT))
			ESP.restart();
	}

	static void ICACHE_RAM_ATTR static_callback(SoftwareWatchdog* _this) {
		if (_this)
			_this->callback();
	}

public:
	SoftwareWatchdog() : ticker() {
		enable();
	}

	void enable() {
		feed();
		ticker.attach_ms(TIMEOUT, static_callback, this);
	}

	void feed() {
		time = millis();
		timeout = time + TIMEOUT;
	}

	void disable() {
		ticker.detach();
	}
};

