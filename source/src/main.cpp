#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <wiring_private.h>
#include <TelnetStream2.h>
#include <WiFiManager.h>
#include <INA219.h>
#include "SoftwareWatchdog.h"
#include "ADCCalc.hpp"
#include "MPPT.hpp"

static const char AP_SSID[] = "MPPTCharger";
static const char AP_PASSWORD[] = "zweiundvierzig";

static const uint8_t INA219_SCL = 2;
static const uint8_t INA219_SDA = 0;
static const float SHUNT_R = 0.005;
static const float SHUNT_MAX_V = 0.04;
static const float MAX_CURRENT = SHUNT_MAX_V / SHUNT_R;

static const float ADC_VOLTAGE_OUT_MIN = 33.0;
static const float ADC_VOLTAGE_OUT_MAX = 42.0;
SoftwareWatchdog swWatchdog;
ADCCalc adcs;
INA219 ina219;
/* GPIO12 is connected to dead time control of TL494 */
const uint8_t TL494_DTC_PIN = 12;
MPPT mppt(TL494_DTC_PIN);


RF_PRE_INIT() {
	/* disable step up/down converter to keep initial solar power for ESP boot up */
	digitalWrite(TL494_DTC_PIN, HIGH);
	pinMode(TL494_DTC_PIN, OUTPUT);
}

/* do not reset GPIO pins between RF_PRE_INIT() and setup() */
void resetPins() {}

void setup()
{
	Serial.begin(115200);
	WiFiManager wifiManager;
	/* do not block in Web interface when STA WiFi not available */
	wifiManager.setConfigPortalTimeout(1);
	/* connecting to wifi might take some time */
	swWatchdog.feed();
	wifiManager.autoConnect(AP_SSID, AP_PASSWORD);
	swWatchdog.feed();

	Wire.begin(INA219_SDA, INA219_SCL);
	ina219.begin();
	ina219.configure(INA219::RANGE_32V, INA219::GAIN_1_40MV, INA219::ADC_64SAMP,
			INA219::ADC_64SAMP, INA219::CONT_SH_BUS);
	ina219.calibrate(SHUNT_R, SHUNT_MAX_V, D_V_BUS_MAX, MAX_CURRENT);

	TelnetStream2.begin();
}

void log()
{
	// TODO only execute the following when a client is connected */
	//if (!TelnetStream2.available())
	//	return;

	/* clear screen and move to home position */
	TelnetStream2.print("\033[H\033[2J");

	TelnetStream2.println("ESP8266 MPPT Charger");
	TelnetStream2.printf("Uin:\t");

	TelnetStream2.print(ina219.busVoltage());
	TelnetStream2.println("V");

	TelnetStream2.print("Iin:\t");
	TelnetStream2.print(ina219.shuntCurrent());
	TelnetStream2.println("A");

	TelnetStream2.print("Pin:\t");
	TelnetStream2.print(ina219.busPower());
	TelnetStream2.println("W");

	// TODO provide class which inheritates from ArduinoINA and implements measurment
/*	TelnetStream2.print("Ein:\t");
	TelnetStream2.print(adcs.get(ADC_ENERGY_IN));
	TelnetStream2.println("Wh");*/

	TelnetStream2.print("Uout:\t");
	const float uout = adcs.get(ADC_VOLTAGE_OUT);
	TelnetStream2.print(uout);
	TelnetStream2.print("V ");

	static const float voltage2level = 100 / (ADC_VOLTAGE_OUT_MAX - ADC_VOLTAGE_OUT_MIN);
	const float uoutBattStart = uout - ADC_VOLTAGE_OUT_MIN;
	const float chargeLevel = (uoutBattStart > 0) ? (uoutBattStart * voltage2level) : 0.0;
	TelnetStream2.print(chargeLevel);
	TelnetStream2.println("%");

	TelnetStream2.print("PWMout:\t");
	const float pwm = mppt.getPwmLevel();
	TelnetStream2.print(pwm);
	TelnetStream2.print("% ");

	static float lastPWM = 0.0;
	if (pwm > lastPWM)
		TelnetStream2.println("u");
	else
		TelnetStream2.println("d");
	lastPWM = pwm;
}

void loop()
{
	switch (TelnetStream2.read()) {
	case 'w':
	case 'W': {
		WiFiManager wifiManager;
		wifiManager.startConfigPortal(AP_SSID, AP_PASSWORD);
		break;
	}
	case 'R':
		TelnetStream2.stop();
		delay(100);
		ESP.restart();
		break;
	case 'e':
	case 'q':
	case 'c':
	case 'C':
		TelnetStream2.println("bye bye");
		TelnetStream2.flush();
		TelnetStream2.stop();
		break;
	}


	const unsigned long now = millis();
	static unsigned long next = now;
	static uint8_t counter = 0;
	if (next < now) {
		next += 50;
		if (adcs.update()) {
			/* wait for averaging */
			// TODO may be high diffs will only be craeted on a certain PWM level
			// therefore avarage over multiple PWM levels
				mppt.update(ina219.busVoltage(),
					ina219.busPower());
			if (counter > 2) {
				counter = 0;
				/* print values when desition was made */
				log();
				swWatchdog.feed();
			}
			counter++;
		}
	}

}

