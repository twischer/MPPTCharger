#include <stdio.h>
//#include <wiringPi.h>
#include <Arduino.h>
#include <INA219.h>
#include <MPPT.hpp>
#include <MPPTPWMOutput.hpp>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	LED	0

INA219 monitor;
MPPTPWMOutput mpptOutput(LED);
MPPT mppt(mpptOutput);

int main (void)
{
	printf ("Raspberry Pi blink\n") ;

	wiringPiSetup () ;
	pinMode (LED, OUTPUT) ;
	monitor.begin();

	Serial.print("raw shunt voltage: ");
	Serial.println(monitor.shuntVoltageRaw());

	Serial.print("raw bus voltage:   ");
	Serial.println(monitor.busVoltageRaw());

	Serial.println("--");

	Serial.print("shunt voltage: ");
	Serial.print(monitor.shuntVoltage() * 1000, 4);
	Serial.println(" mV");

	Serial.print("shunt current: ");
	Serial.print(monitor.shuntCurrent() * 1000, 4);
	Serial.println(" mA");

	Serial.print("bus voltage:   ");
	Serial.print(monitor.busVoltage(), 4);
	Serial.println(" V");

	Serial.print("bus power:     ");
	Serial.print(monitor.busPower() * 1000, 4);
	Serial.println(" mW");

	for (;;)
	{
		digitalWrite (LED, HIGH) ;	// On
		delay (500) ;		// mS
		digitalWrite (LED, LOW) ;	// Off
		delay (500) ;
	}
	return 0 ;
}

