EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
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
L Amplifier_Operational:MCP6002-xSN U1
U 1 1 6157FBDA
P 5000 2600
F 0 "U1" H 5000 2967 50  0000 C CNN
F 1 "MCP6002" H 5000 2876 50  0000 C CNN
F 2 "myFootPrintLibs:SO-8_3.9x4.9mm_P1.27mm" H 5000 2600 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 5000 2600 50  0001 C CNN
	1    5000 2600
	1    0    0    -1  
$EndComp
Text HLabel 3000 2500 0    60   Input ~ 0
thermal_couple
$Comp
L Device:R R10
U 1 1 61582400
P 4500 3550
F 0 "R10" H 4570 3596 50  0000 L CNN
F 1 "1k" H 4570 3505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4430 3550 50  0001 C CNN
F 3 "~" H 4500 3550 50  0001 C CNN
	1    4500 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3400 4500 3100
Wire Wire Line
	4500 2700 4700 2700
$Comp
L power:GND #PWR04
U 1 1 61583D87
P 4500 3750
F 0 "#PWR04" H 4500 3500 50  0001 C CNN
F 1 "GND" H 4505 3577 50  0000 C CNN
F 2 "" H 4500 3750 50  0001 C CNN
F 3 "" H 4500 3750 50  0001 C CNN
	1    4500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3750 4500 3700
$Comp
L Device:R R12
U 1 1 61584BEB
P 5000 3100
F 0 "R12" V 4793 3100 50  0000 C CNN
F 1 "240k" V 4884 3100 50  0000 C CIN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 4930 3100 50  0001 C CNN
F 3 "~" H 5000 3100 50  0001 C CNN
	1    5000 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 3100 4500 3100
Connection ~ 4500 3100
Wire Wire Line
	4500 3100 4500 2700
Wire Wire Line
	5150 3100 5450 3100
Wire Wire Line
	5450 3100 5450 2600
Wire Wire Line
	5450 2600 5300 2600
Text HLabel 6200 2600 2    60   Output ~ 0
opampOutput_tc
Wire Wire Line
	6200 2600 5850 2600
Connection ~ 5450 2600
$Comp
L Amplifier_Operational:MCP6002-xSN U1
U 3 1 615916E3
P 4600 5650
F 0 "U1" H 4600 5700 50  0000 L CNN
F 1 "MCP6002" H 4550 5600 50  0000 L CNN
F 2 "myFootPrintLibs:SO-8_3.9x4.9mm_P1.27mm" H 4600 5650 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 4600 5650 50  0001 C CNN
	3    4600 5650
	1    0    0    -1  
$EndComp
Text HLabel 4500 4600 0    60   Input ~ 0
V5.0
Wire Wire Line
	4500 4600 4500 4850
$Comp
L power:GND #PWR08
U 1 1 61598474
P 4500 6050
F 0 "#PWR08" H 4500 5800 50  0001 C CNN
F 1 "GND" H 4505 5877 50  0000 C CNN
F 2 "" H 4500 6050 50  0001 C CNN
F 3 "" H 4500 6050 50  0001 C CNN
	1    4500 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 6050 4500 5950
$Comp
L Device:C C3
U 1 1 6159A04D
P 4050 5600
F 0 "C3" H 4165 5646 50  0000 L CNN
F 1 "10nf" H 4165 5555 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 4088 5450 50  0001 C CNN
F 3 "~" H 4050 5600 50  0001 C CNN
	1    4050 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 5450 4050 5250
Wire Wire Line
	4050 5250 4500 5250
Connection ~ 4500 5250
Wire Wire Line
	4500 5250 4500 5350
$Comp
L Device:C C1
U 1 1 6159C372
P 3200 5600
F 0 "C1" H 3315 5646 50  0000 L CNN
F 1 "22uf" H 3315 5555 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 3238 5450 50  0001 C CNN
F 3 "~" H 3200 5600 50  0001 C CNN
	1    3200 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 6159D250
P 4050 5850
F 0 "#PWR07" H 4050 5600 50  0001 C CNN
F 1 "GND" H 4055 5677 50  0000 C CNN
F 2 "" H 4050 5850 50  0001 C CNN
F 3 "" H 4050 5850 50  0001 C CNN
	1    4050 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 5850 4050 5750
$Comp
L power:GND #PWR05
U 1 1 6159E66B
P 3200 5850
F 0 "#PWR05" H 3200 5600 50  0001 C CNN
F 1 "GND" H 3205 5677 50  0000 C CNN
F 2 "" H 3200 5850 50  0001 C CNN
F 3 "" H 3200 5850 50  0001 C CNN
	1    3200 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 5850 3200 5750
Wire Wire Line
	3200 5450 3200 5250
Wire Wire Line
	3200 5250 3600 5250
Connection ~ 4050 5250
$Comp
L Device:C C2
U 1 1 615A32A8
P 3600 5600
F 0 "C2" H 3715 5646 50  0000 L CNN
F 1 "100nf" H 3715 5555 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 3638 5450 50  0001 C CNN
F 3 "~" H 3600 5600 50  0001 C CNN
	1    3600 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 5450 3600 5250
Connection ~ 3600 5250
Wire Wire Line
	3600 5250 4050 5250
$Comp
L power:GND #PWR06
U 1 1 615A5AF9
P 3600 5850
F 0 "#PWR06" H 3600 5600 50  0001 C CNN
F 1 "GND" H 3605 5677 50  0000 C CNN
F 2 "" H 3600 5850 50  0001 C CNN
F 3 "" H 3600 5850 50  0001 C CNN
	1    3600 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 5750 3600 5850
$Comp
L power:GND #PWR019
U 1 1 615BBDA5
P 4050 3300
F 0 "#PWR019" H 4050 3050 50  0001 C CNN
F 1 "GND" H 4055 3127 50  0000 C CNN
F 2 "" H 4050 3300 50  0001 C CNN
F 3 "" H 4050 3300 50  0001 C CNN
	1    4050 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3300 4050 3100
Wire Wire Line
	4050 2800 4050 2500
Connection ~ 4050 2500
Wire Wire Line
	4050 2500 4700 2500
$Comp
L Device:R R1
U 1 1 615BE19D
P 3600 2500
F 0 "R1" V 3393 2500 50  0000 C CNN
F 1 "10k" V 3484 2500 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 3530 2500 50  0001 C CNN
F 3 "~" H 3600 2500 50  0001 C CNN
	1    3600 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 2500 4050 2500
Text Notes 2900 3200 0    60   ~ 0
limit the input to 0.2V
Wire Wire Line
	3000 2500 3450 2500
Text Label 4700 4850 0    50   ~ 0
5V
Wire Wire Line
	4700 4850 4500 4850
Connection ~ 4500 4850
Wire Wire Line
	4500 4850 4500 5250
Text Notes 1950 2150 0    50   ~ 0
with this diode, cannot detect thermocouple disconnect.\nwithout this, when thermo not connected, opamp output is high\nwith this diode, not connect thermo, opamp output is still low. so, cannot detect thermo not connect
Wire Wire Line
	5850 2950 5850 2600
Connection ~ 5850 2600
Wire Wire Line
	5850 2600 5450 2600
$Comp
L power:GND #PWR?
U 1 1 61908316
P 5850 3350
F 0 "#PWR?" H 5850 3100 50  0001 C CNN
F 1 "GND" H 5855 3177 50  0000 C CNN
F 2 "" H 5850 3350 50  0001 C CNN
F 3 "" H 5850 3350 50  0001 C CNN
	1    5850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3250 5850 3350
$EndSCHEMATC
