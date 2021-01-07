#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <wiring_private.h>
#include <GDBStub.h>
#include <TelnetStream.h>
#include <StreamUtils.h>
#include <WiFiManager.h>
#include <INA219Calc.hpp>
#include "SoftwareWatchdog.h"
#include "ADCReader.hpp"
#include "MPPT.hpp"

static const char AP_SSID[] = "MPPTCharger";
static const char AP_PASSWORD[] = "zweiundvierzig";

static const bool ENABLE_GDB_STUB = false;

static const uint8_t INA219_SCL = 2;
static const uint8_t INA219_SDA = 13;
static const float SHUNT_R = 0.005;
static const float SHUNT_MAX_V = 0.04;
static const float MAX_CURRENT = SHUNT_MAX_V / SHUNT_R;

static const float ADC_VOLTAGE_OUT_MIN = 33.0;
static const float ADC_VOLTAGE_OUT_MAX = 42.0;
static const float ADC_VOLTAGE_OUT_PROTECT = ADC_VOLTAGE_OUT_MAX * 1.01; /* max + 1% */

SoftwareWatchdog swWatchdog;
ADCReader adcs;
INA219Calc ina219;
/* GPIO12 is connected to dead time control of TL494 */
const uint8_t TL494_DTC_PIN = 12;
MPPT mppt(TL494_DTC_PIN);
WriteBufferingStream telnet(TelnetStream, 160);


static void outSwitchOff()
{
	digitalWrite(TL494_DTC_PIN, HIGH);
	pinMode(TL494_DTC_PIN, OUTPUT);
}

RF_PRE_INIT() {
	/* disable step up/down converter to keep initial solar power for ESP boot up */
	outSwitchOff();
}

/* do not reset GPIO pins between RF_PRE_INIT() and setup() */
void resetPins() {}

void setup()
{
	Serial.begin(115200);
	Serial.print("Reset reason: ");
	Serial.println(ESP.getResetInfo());

	if (ENABLE_GDB_STUB) {
		/* See https://arduino-esp8266.readthedocs.io/en/latest/gdb.html for usage */
		gdbstub_init();
	}

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

	TelnetStream.begin();
}

static void updateTelnet()
{
	// TODO only execute the following when a client is connected */
	//if (!TelnetStream.available())
	//	return;

	/* clear screen and move to home position */
	telnet.print("\033[H\033[2J");
	telnet.println("ESP8266 MPPT Charger");

	const uint32_t seconds = millis() / 1000;
	const uint32_t minutes = seconds / 60;
	const uint32_t hours = minutes / 60;
	telnet.printf("Uptime:\t%02u:%02u:%02uh", hours, (minutes % 60), (seconds % 60));
	telnet.println();

	telnet.print("Uin:\t");
	telnet.print(ina219.busVoltage());
	telnet.println("V");

	telnet.print("Iin:\t");
	telnet.print(ina219.shuntCurrent());
	telnet.println("A");

	telnet.print("Pin:\t");
	telnet.print(ina219.busPower());
	telnet.println("W");

	telnet.print("Ein:\t");
	telnet.print(ina219.busEnergy());
	telnet.println("Wh");

	telnet.print("Uout:\t");
	const float uout = adcs.get(ADC_VOLTAGE_OUT);
	static bool outProtected = false;
	if (uout > ADC_VOLTAGE_OUT_PROTECT) {
		outSwitchOff();
		outProtected = true;
	}
	telnet.print(uout);
	telnet.print("V ");

	static const float voltage2level = 100 / (ADC_VOLTAGE_OUT_MAX - ADC_VOLTAGE_OUT_MIN);
	const float uoutBattStart = uout - ADC_VOLTAGE_OUT_MIN;
	const float chargeLevel = (uoutBattStart > 0) ? (uoutBattStart * voltage2level) : 0.0;

	telnet.print(chargeLevel);
	telnet.println("%");

	telnet.print("PWMout:\t");
	if (outProtected)
		telnet.println("PROTECTED!!!");
	else {
		const float pwm = mppt.getPwmLevel();
		telnet.print(pwm);
		telnet.print("% ");

		static float lastPWM = 0.0;
		if (pwm > lastPWM)
			telnet.println("u");
		else
			telnet.println("d");
		lastPWM = pwm;
	}
	telnet.flush();
}

void loop()
{
	switch (TelnetStream.read()) {
	case 'w':
	case 'W': {
		WiFiManager wifiManager;
		wifiManager.startConfigPortal(AP_SSID, AP_PASSWORD);
		break;
	}
	case 'R':
		TelnetStream.stop();
		delay(100);
		ESP.restart();
		break;
	case 'e':
	case 'q':
	case 'c':
	case 'C':
		TelnetStream.println("bye bye");
		TelnetStream.flush();
		TelnetStream.stop();
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
			if (counter > 6) {
				counter = 0;
				/* print values when desition was made */
				updateTelnet();
				swWatchdog.feed();
			}
			counter++;
		}
	}

}

