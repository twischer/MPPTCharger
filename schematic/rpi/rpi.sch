EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Raspberry_Pi_2_3 J?
U 1 1 609CF659
P 6000 3700
F 0 "J?" H 6000 5181 50  0000 C CNN
F 1 "Raspberry_Pi_2_3" H 6000 5090 50  0000 C CNN
F 2 "" H 6000 3700 50  0001 C CNN
F 3 "https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/rpi_SCH_3bplus_1p0_reduced.pdf" H 6000 3700 50  0001 C CNN
	1    6000 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 609D201C
P 4850 3300
F 0 "R?" V 4643 3300 50  0000 C CNN
F 1 "240" V 4734 3300 50  0000 C CNN
F 2 "" V 4780 3300 50  0001 C CNN
F 3 "~" H 4850 3300 50  0001 C CNN
	1    4850 3300
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 609D43FD
P 3200 3200
F 0 "J?" H 3118 2775 50  0000 C CNN
F 1 "XL4016" H 3118 2866 50  0000 C CNN
F 2 "" H 3200 3200 50  0001 C CNN
F 3 "~" H 3200 3200 50  0001 C CNN
	1    3200 3200
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 609D4F97
P 3600 3450
F 0 "R?" H 3530 3404 50  0000 R CNN
F 1 "430" H 3530 3495 50  0000 R CNN
F 2 "" V 3530 3450 50  0001 C CNN
F 3 "~" H 3600 3450 50  0001 C CNN
	1    3600 3450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 609D548B
P 3900 3450
F 0 "R?" H 3830 3404 50  0000 R CNN
F 1 "2k2" H 3830 3495 50  0000 R CNN
F 2 "" V 3830 3450 50  0001 C CNN
F 3 "~" H 3900 3450 50  0001 C CNN
	1    3900 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 3300 3600 3300
Wire Wire Line
	3600 3300 3900 3300
Connection ~ 3600 3300
Wire Wire Line
	3600 3600 3900 3600
Wire Wire Line
	3600 3600 3400 3600
Wire Wire Line
	3400 3600 3400 3400
Connection ~ 3600 3600
Wire Wire Line
	5000 3300 5200 3300
Wire Wire Line
	4400 3300 3900 3300
Connection ~ 3900 3300
Wire Notes Line
	4200 3850 4200 2700
Wire Notes Line
	4200 2700 3100 2700
Wire Notes Line
	3100 2700 3100 3850
Wire Notes Line
	3100 3850 4200 3850
Text Notes 3300 2700 0    50   ~ 0
Step Down Module
$Comp
L power:GND #PWR?
U 1 1 609DBCC9
P 3400 3600
F 0 "#PWR?" H 3400 3350 50  0001 C CNN
F 1 "GND" H 3405 3427 50  0000 C CNN
F 2 "" H 3400 3600 50  0001 C CNN
F 3 "" H 3400 3600 50  0001 C CNN
	1    3400 3600
	1    0    0    -1  
$EndComp
Connection ~ 3400 3600
$Comp
L power:GND #PWR?
U 1 1 609DD28E
P 5800 5000
F 0 "#PWR?" H 5800 4750 50  0001 C CNN
F 1 "GND" H 5805 4827 50  0000 C CNN
F 2 "" H 5800 5000 50  0001 C CNN
F 3 "" H 5800 5000 50  0001 C CNN
	1    5800 5000
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4148 D?
U 1 1 609E0467
P 4550 3300
F 0 "D?" H 4550 3517 50  0000 C CNN
F 1 "1N4148" H 4550 3426 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 4550 3125 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 4550 3300 50  0001 C CNN
	1    4550 3300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
