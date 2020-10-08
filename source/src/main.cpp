#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <sigma_delta.h>
#include <TelnetStream2.h>
#include <WiFiManager.h>
#include "ADCReader.hpp"

static const float ADC_VOLTAGE_OUT_MAX = 42.0;
ADCReader adcs;

void setup()
{
	Serial.begin(115200);
	WiFiManager wifiManager;
	wifiManager.autoConnect();

	TelnetStream2.begin();

	sigmaDeltaSetup(0, 312500);
	sigmaDeltaWrite(0, 255);
	/* GPIO12 is connected to dead time control of TL494 */
	sigmaDeltaAttachPin(0, 12);
}

void log()
{
	//if (!TelnetStream2.available())
	//	return;

	TelnetStream2.println("ESP8266 MPPT Charger");
	TelnetStream2.printf("Uin:\t");
	const float uin = adcs.get(ADC_VOLTAGE_IN);
	TelnetStream2.print(uin);
	TelnetStream2.println("V");

	TelnetStream2.print("Iin:\t");
	const float iin = adcs.get(ADC_CURRENT_IN);
	TelnetStream2.print(iin);
	TelnetStream2.println("A");

	TelnetStream2.print("Pin:\t");
	TelnetStream2.print(uin * iin);
	TelnetStream2.println("W");

	TelnetStream2.print("Ein:\t");
	TelnetStream2.print(uin * iin * 0.5);
	TelnetStream2.println("Wh");

	TelnetStream2.print("Uout:\t");
	const float uout = adcs.get(ADC_VOLTAGE_OUT);
	TelnetStream2.print(uout);
	TelnetStream2.print("V ");
	TelnetStream2.print(uout * 100 / ADC_VOLTAGE_OUT_MAX);
	TelnetStream2.println("%");

	/* move courser to the left */
	TelnetStream2.print("\u001b[1000D");
	/* move courser to the top */
	TelnetStream2.print("\u001b[6A");
}

void loop()
{
	switch (TelnetStream2.read()) {
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


	static unsigned long next = millis();
	if (millis() > next) {
		next += 500;
		adcs.update();
		log();
	}
}

