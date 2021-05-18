#include <stdio.h>
#include <Arduino.h>
#include <INA219.h>
#include <MPPT.hpp>
#include <MPPTPWMOutput.hpp>

/* Connected to pin 12 on pin header (PWM0) */
#define GPIO18	1

int main (void)
{
	printf ("Raspberry Pi blink\n") ;

	if (wiringPiSetup() == -1)
		exit (1);

	MPPTPWMOutput mpptOutput(GPIO18);
	MPPT mppt(mpptOutput);

/*	INA219 monitor;
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
	Serial.println(" mW");*/

	for (;;)
	{
		float voltage = 0.0;
		float power = 0.0;
		scanf("%f %f\n", &voltage, &power);
		mppt.update(voltage, power);
		Serial.print("PWM: ");
		Serial.println(mppt.getPwmLevel());

		/* waiting 100ms */
		//delay (100) ;
	}
	return 0 ;
}

