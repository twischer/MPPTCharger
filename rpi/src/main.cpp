#include <stdio.h>
//#include <wiringPi.h>
#include <Arduino.h>
//#include <INA219.h>
//#include <MPPT.hpp>
#include <MPPTPWMOutput.hpp>

/* Connected to pin 12 on pin header */
#define GPIO18	18

//INA219 monitor;

int main (void)
{
	printf ("Raspberry Pi blink\n") ;

	if (wiringPiSetupGpio() == -1)
		exit (1);

	MPPTPWMOutput mpptOutput(GPIO18);
	mpptOutput.write(128);

//	MPPT mppt(mpptOutput);
/*	monitor.begin();

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
*/
	for (;;)
	{
/*		mppt.update(12, 5);
		Serial.print("PWM: ");
		Serial.println(mppt.getPwmLevel());
*/
		delay (500) ;
	}
	return 0 ;
}

