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
L MCU_Module:Arduino_Nano_v2.x A1
U 1 1 615E789E
P 8750 3200
F 0 "A1" H 8750 2111 50  0000 C CNN
F 1 "Arduino_Nano_v2.x" H 8750 2020 50  0000 C CNN
F 2 "myFootPrintLibs:Arduino_Nano" H 8750 3200 50  0001 C CIN
F 3 "https://www.arduino.cc/en/uploads/Main/ArduinoNanoManual23.pdf" H 8750 3200 50  0001 C CNN
	1    8750 3200
	1    0    0    -1  
$EndComp
$Comp
L mySymbolLibrary:AO3401_PMosfet Q1
U 1 1 61619C21
P 7000 3650
F 0 "Q1" H 7155 3696 50  0000 L CNN
F 1 "AO3401_PMosfet" H 7155 3605 50  0000 L CNN
F 2 "myFootPrintLibs:SOT-23_PMOSFET" H 7150 3725 50  0001 L CIN
F 3 "www.aosmd.com/pdfs/datasheet/AO3401.pdf" H 6950 3650 50  0001 L CNN
	1    7000 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 6161B408
P 6000 3600
F 0 "R3" V 5793 3600 50  0000 C CNN
F 1 "2k" V 5884 3600 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 5930 3600 50  0001 C CNN
F 3 "~" H 6000 3600 50  0001 C CNN
	1    6000 3600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 6161F208
P 5450 4350
F 0 "#PWR02" H 5450 4100 50  0001 C CNN
F 1 "GND" H 5455 4177 50  0000 C CNN
F 2 "" H 5450 4350 50  0001 C CNN
F 3 "" H 5450 4350 50  0001 C CNN
	1    5450 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3050 1900 3050
$Comp
L power:+5VD #PWR03
U 1 1 616245AD
P 7050 2650
F 0 "#PWR03" H 7050 2500 50  0001 C CNN
F 1 "+5VD" H 7065 2823 50  0000 C CNN
F 2 "" H 7050 2650 50  0001 C CNN
F 3 "" H 7050 2650 50  0001 C CNN
	1    7050 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 2650 7050 2800
Wire Wire Line
	3400 5550 7050 5550
Wire Wire Line
	7050 5550 7050 3850
$Comp
L power:GND #PWR01
U 1 1 61626449
P 1600 3850
F 0 "#PWR01" H 1600 3600 50  0001 C CNN
F 1 "GND" H 1605 3677 50  0000 C CNN
F 2 "" H 1600 3850 50  0001 C CNN
F 3 "" H 1600 3850 50  0001 C CNN
	1    1600 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 3850 1600 3750
Wire Wire Line
	1500 3650 1500 3750
Wire Wire Line
	1500 3750 1600 3750
Connection ~ 1600 3750
Wire Wire Line
	1600 3750 1600 3650
$Comp
L power:GND #PWR06
U 1 1 6162BD35
P 2050 6450
F 0 "#PWR06" H 2050 6200 50  0001 C CNN
F 1 "GND" V 2055 6322 50  0000 R CNN
F 2 "" H 2050 6450 50  0001 C CNN
F 3 "" H 2050 6450 50  0001 C CNN
	1    2050 6450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2050 3400 1900 3400
Wire Wire Line
	1900 3400 1900 3350
Wire Wire Line
	2750 6800 2600 6800
Wire Wire Line
	2600 6800 2600 6750
Text Notes 1150 6150 0    118  ~ 0
PC to Usb Power
Text Notes 2900 2650 0    118  ~ 0
USBPower_To_Device
Text Notes 550  4150 0    118  ~ 0
Big Usb A Receptacle
Text Notes 8600 5050 0    118  ~ 0
consider usb micro receptacle, \nor big USB receptacle
Wire Wire Line
	6150 3600 6500 3600
$Comp
L Device:C C1
U 1 1 616482DB
P 6500 3950
F 0 "C1" H 6615 3996 50  0000 L CNN
F 1 "1uf" H 6250 3850 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 6538 3800 50  0001 C CNN
F 3 "~" H 6500 3950 50  0001 C CNN
	1    6500 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6164F3F1
P 5450 3350
F 0 "R1" H 5520 3396 50  0000 L CNN
F 1 "50k" H 5520 3305 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 5380 3350 50  0001 C CNN
F 3 "~" H 5450 3350 50  0001 C CNN
	1    5450 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+5VD #PWR07
U 1 1 61650AD7
P 2400 6950
F 0 "#PWR07" H 2400 6800 50  0001 C CNN
F 1 "+5VD" H 2415 7123 50  0000 C CNN
F 2 "" H 2400 6950 50  0001 C CNN
F 3 "" H 2400 6950 50  0001 C CNN
	1    2400 6950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 61654D82
P 8850 4450
F 0 "#PWR04" H 8850 4200 50  0001 C CNN
F 1 "GND" H 8855 4277 50  0000 C CNN
F 2 "" H 8850 4450 50  0001 C CNN
F 3 "" H 8850 4450 50  0001 C CNN
	1    8850 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 4200 8850 4250
Wire Wire Line
	8750 4200 8750 4250
Wire Wire Line
	8750 4250 8850 4250
Connection ~ 8850 4250
Wire Wire Line
	8850 4250 8850 4450
NoConn ~ 8250 2600
NoConn ~ 8250 2700
NoConn ~ 8250 2800
NoConn ~ 8250 2900
NoConn ~ 8250 3100
NoConn ~ 8250 3200
NoConn ~ 8250 3300
NoConn ~ 8250 3400
NoConn ~ 8250 3500
NoConn ~ 8250 3600
NoConn ~ 8250 3700
NoConn ~ 8250 3800
NoConn ~ 8250 3900
NoConn ~ 9250 3900
NoConn ~ 9250 3800
NoConn ~ 9250 3700
NoConn ~ 9250 3600
NoConn ~ 9250 3400
NoConn ~ 9250 3300
NoConn ~ 9250 3200
NoConn ~ 9250 3000
NoConn ~ 9250 2600
NoConn ~ 8850 2200
NoConn ~ 8650 2200
$Comp
L SparkFun-Connectors:USB_MINI-B_SMT J2
U 1 1 616698AD
P 1650 6750
F 0 "J2" H 1650 6190 45  0000 C CNN
F 1 "USB_MINI-B_SMT" H 1650 6274 45  0000 C CNN
F 2 "myFootPrintLibs:USB_Mini-B_Wuerth_65100516121_Horizontal" H 1650 7200 20  0001 C CNN
F 3 "" H 1650 6750 50  0001 C CNN
F 4 "CONN-08193" H 1650 6369 60  0000 C CNN "Field4"
	1    1650 6750
	-1   0    0    1   
$EndComp
Wire Wire Line
	1900 6950 2400 6950
Wire Wire Line
	2100 6650 2100 6750
Wire Wire Line
	2100 6750 1900 6750
Wire Wire Line
	2600 6750 2200 6750
Wire Wire Line
	2200 6750 2200 6850
Wire Wire Line
	2200 6850 1900 6850
NoConn ~ 1900 6650
Wire Wire Line
	2050 6450 2050 6550
Wire Wire Line
	2050 6550 1950 6550
Wire Wire Line
	1400 6500 1400 6400
Wire Wire Line
	1400 6400 1950 6400
Wire Wire Line
	1950 6400 1950 6550
Connection ~ 1950 6550
Wire Wire Line
	1950 6550 1900 6550
Text Label 2800 6600 0    50   ~ 0
D+
Wire Wire Line
	2800 6600 2800 6650
Wire Wire Line
	2100 6650 2800 6650
Text Label 2750 6800 0    50   ~ 0
D-
Wire Wire Line
	2100 3200 2100 3250
Wire Wire Line
	1900 3250 2100 3250
NoConn ~ 9250 2700
Text Label 2100 3200 0    50   ~ 0
D+
Text Label 2050 3400 0    50   ~ 0
D-
Wire Wire Line
	9250 3500 9700 3500
NoConn ~ 8250 3000
$Comp
L mySymbolLibrary:USB_A J1
U 1 1 61644F62
P 1600 3250
F 0 "J1" H 1657 3717 50  0000 C CNN
F 1 "USB_A" H 1657 3626 50  0000 C CNN
F 2 "myFootPrintLibs:SAMTEC_USB-A-S-X-X-SM2" H 1750 3200 50  0001 C CNN
F 3 " ~" H 1750 3200 50  0001 C CNN
	1    1600 3250
	1    0    0    -1  
$EndComp
$Comp
L mySymbolLibrary:LED D1
U 1 1 6161E91D
P 3400 5850
F 0 "D1" V 3439 5732 50  0000 R CNN
F 1 "LED" V 3348 5732 50  0000 R CNN
F 2 "myFootPrintLibs:LED_D3.0mm" H 3400 5850 50  0001 C CNN
F 3 "~" H 3400 5850 50  0001 C CNN
	1    3400 5850
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 61620B10
P 3400 6550
F 0 "#PWR0101" H 3400 6300 50  0001 C CNN
F 1 "GND" H 3405 6377 50  0000 C CNN
F 2 "" H 3400 6550 50  0001 C CNN
F 3 "" H 3400 6550 50  0001 C CNN
	1    3400 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 61621A0E
P 3400 6350
F 0 "R4" H 3470 6396 50  0000 L CNN
F 1 "2k" H 3470 6305 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 3330 6350 50  0001 C CNN
F 3 "~" H 3400 6350 50  0001 C CNN
	1    3400 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 6000 3400 6200
Wire Wire Line
	3400 6500 3400 6550
Text Notes 6600 1700 0    50   ~ 0
arduino output can be as low as 4.6V because it\nhas a diode from 5V usb to its supply. \nso output is 4.6, and there is D1 here, \nit is reduced even lower, not high enough,  \nso the Pmosfet cannot be fully turned OFF. 
Text Label 9700 3500 0    50   ~ 0
usb_control_pin
$Comp
L mySymbolLibrary:S8050 Q2
U 1 1 6165F8F0
P 5350 4000
F 0 "Q2" H 5540 4046 50  0000 L CNN
F 1 "S8050" H 5540 3955 50  0000 L CNN
F 2 "myFootPrintLibs:SOT-23_PNP_NPN" H 5550 3925 50  0001 L CIN
F 3 "http://www.unisonic.com.tw/datasheet/S8050.pdf" H 5350 4000 50  0001 L CNN
	1    5350 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 616742D2
P 4350 4000
F 0 "R2" H 4420 4046 50  0000 L CNN
F 1 "5k" H 4420 3955 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 4280 4000 50  0001 C CNN
F 3 "~" H 4350 4000 50  0001 C CNN
	1    4350 4000
	0    1    1    0   
$EndComp
Text Label 4050 4000 2    50   ~ 0
usb_control_pin
Text Notes 3650 4450 0    79   ~ 0
high to enable\nlow to disable
Wire Wire Line
	5450 2800 5450 3200
Connection ~ 7050 2800
Wire Wire Line
	7050 2800 7050 3450
Connection ~ 5450 3600
Wire Wire Line
	5450 3600 5450 3500
Text Notes 3900 3150 0    50   ~ 0
pull high to always  \nenable by default
Wire Wire Line
	5450 4200 5450 4350
Wire Wire Line
	5450 2800 7050 2800
Wire Wire Line
	3400 5550 3400 5700
$Comp
L power:GND #PWR0102
U 1 1 6170B9AC
P 6500 4250
F 0 "#PWR0102" H 6500 4000 50  0001 C CNN
F 1 "GND" H 6505 4077 50  0000 C CNN
F 2 "" H 6500 4250 50  0001 C CNN
F 3 "" H 6500 4250 50  0001 C CNN
	1    6500 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3800 6500 3600
Wire Wire Line
	6500 4100 6500 4250
Wire Wire Line
	5450 3600 5850 3600
Connection ~ 6500 3600
Wire Wire Line
	6500 3600 6800 3600
Connection ~ 4750 4000
Wire Wire Line
	4750 4000 5150 4000
$Comp
L power:GND #PWR0103
U 1 1 61732716
P 4750 4750
F 0 "#PWR0103" H 4750 4500 50  0001 C CNN
F 1 "GND" H 4755 4577 50  0000 C CNN
F 2 "" H 4750 4750 50  0001 C CNN
F 3 "" H 4750 4750 50  0001 C CNN
	1    4750 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5550 3400 5550
Wire Wire Line
	2550 3050 2550 5550
Connection ~ 3400 5550
Wire Wire Line
	4200 4000 4050 4000
Wire Wire Line
	4500 4000 4750 4000
$Comp
L Device:R R5
U 1 1 6174DB22
P 4750 3150
F 0 "R5" H 4820 3196 50  0000 L CNN
F 1 "50k" H 4820 3105 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4680 3150 50  0001 C CNN
F 3 "~" H 4750 3150 50  0001 C CNN
	1    4750 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3300 4750 4000
Wire Wire Line
	4750 3000 4750 2800
Wire Wire Line
	4750 2800 5450 2800
Connection ~ 5450 2800
$Comp
L mySymbolLibrary:MOMENTARY-SWITCH-SPST-PTH-6.0MM S1
U 1 1 61762657
P 4750 4400
F 0 "S1" H 4750 4710 45  0000 C CNN
F 1 "MOMENTARY-SWITCH" H 4750 4626 45  0000 C CNN
F 2 "myFootPrintLibs:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 4750 4600 20  0001 C CNN
F 3 "" H 4750 4400 50  0001 C CNN
F 4 " SWCH-08441" H 4750 4531 60  0000 C CNN "Field4"
	1    4750 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 3600 5450 3800
Wire Wire Line
	4750 4750 4750 4600
Wire Wire Line
	4750 4200 4750 4000
Text Notes 9100 2200 0    50   ~ 0
arduino 5V has a diode from USB 5V, \nso do not connect arduino 5V to USB 5V.
$EndSCHEMATC
