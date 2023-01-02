EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
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
L Connector_Generic:Conn_01x02 J1
U 1 1 6160C8B5
P 1850 1700
F 0 "J1" H 2050 1800 50  0000 L CNN
F 1 "VH3.96_1X02 20V Power" H 1930 1601 50  0000 L CNN
F 2 "myFootPrintLibs:JST_VH_B2P-VH-B_1x02_P3.96mm_Vertical" H 1850 1700 50  0001 C CNN
F 3 "~" H 1850 1700 50  0001 C CNN
F 4 "Power20V" H 1850 1450 50  0000 C CNN "Field4"
	1    1850 1700
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4007 D1
U 1 1 61610D83
P 3450 1900
F 0 "D1" H 3450 1683 50  0000 C CNN
F 1 "1N4007" H 3450 1774 50  0000 C CNN
F 2 "myFootPrintLibs:D_SMA_Handsoldering" H 3450 1725 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 3450 1900 50  0001 C CNN
	1    3450 1900
	-1   0    0    1   
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x a2
U 1 1 6160FAE5
P 8700 2600
F 0 "a2" H 8700 1511 50  0000 C CNN
F 1 "Arduino_Nano_v2.x" H 8700 1420 50  0000 C CNN
F 2 "myFootPrintLibs:Arduino_Nano" H 8700 2600 50  0001 C CIN
F 3 "https://www.arduino.cc/en/uploads/Main/ArduinoNanoManual23.pdf" H 8700 2600 50  0001 C CNN
	1    8700 2600
	1    0    0    -1  
$EndComp
Text GLabel 1350 1800 0    50   Input ~ 0
20V
Wire Wire Line
	1350 1800 1650 1800
Text GLabel 2950 1900 0    50   Input ~ 0
20V
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 616260E3
P 1850 2350
F 0 "J2" H 1930 2342 50  0000 L CNN
F 1 "Heater VH3.96_1X02" H 1930 2251 50  0000 L CNN
F 2 "myFootPrintLibs:JST_VH_B2P-VH-B_1x02_P3.96mm_Vertical" H 1850 2350 50  0001 C CNN
F 3 "~" H 1850 2350 50  0001 C CNN
F 4 "Heater" H 1850 2500 50  0000 C CNN "Field4"
	1    1850 2350
	1    0    0    -1  
$EndComp
Text Notes 1500 1550 0    50   ~ 0
20V power supply
Text Notes 1700 3650 0    50   ~ 0
thermal couple input
$Comp
L power:GND #PWR02
U 1 1 6162742C
P 1050 1850
F 0 "#PWR02" H 1050 1600 50  0001 C CNN
F 1 "GND" H 1055 1677 50  0000 C CNN
F 2 "" H 1050 1850 50  0001 C CNN
F 3 "" H 1050 1850 50  0001 C CNN
	1    1050 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 1850 1050 1700
Wire Wire Line
	1050 1700 1650 1700
$Comp
L power:GND #PWR01
U 1 1 61629AC5
P 950 2450
F 0 "#PWR01" H 950 2200 50  0001 C CNN
F 1 "GND" H 955 2277 50  0000 C CNN
F 2 "" H 950 2450 50  0001 C CNN
F 3 "" H 950 2450 50  0001 C CNN
	1    950  2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  2450 950  2350
Wire Wire Line
	950  2350 1650 2350
$Sheet
S 1500 4500 1550 750 
U 61610B79
F0 "power__output" 50
F1 "power_output.sch" 50
F2 "enable20VHeater" I L 1500 4750 50 
F3 "20VHeaterPower" O R 3050 4900 50 
F4 "20V" I L 1500 5050 50 
$EndSheet
$Comp
L mySymbolLibrary:Max20003A_DC_buck_36V_To_5V a1
U 1 1 6162BF99
P 5100 1800
F 0 "a1" H 5100 2125 50  0000 C CNN
F 1 "Max20003A_DC_BUCK_5V" H 5100 2034 50  0000 C CNN
F 2 "myFootPrintLibs:MAX20003A_dc_buck_module" H 5100 1800 50  0001 C CNN
F 3 "" H 5100 1800 50  0001 C CNN
	1    5100 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 6162D647
P 4550 1650
F 0 "#PWR012" H 4550 1400 50  0001 C CNN
F 1 "GND" H 4555 1477 50  0000 C CNN
F 2 "" H 4550 1650 50  0001 C CNN
F 3 "" H 4550 1650 50  0001 C CNN
	1    4550 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1650 4750 1650
Wire Wire Line
	4750 1650 4750 1700
$Comp
L Device:CP C7
U 1 1 6162E647
P 4150 2150
F 0 "C7" H 4268 2196 50  0000 L CNN
F 1 "470uf" H 4268 2105 50  0000 L CNN
F 2 "myFootPrintLibs:CP_Radial_D8.0mm_P3.80mm" H 4188 2000 50  0001 C CNN
F 3 "~" H 4150 2150 50  0001 C CNN
	1    4150 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2000 4600 1900
Connection ~ 4600 1900
Wire Wire Line
	4600 1900 4750 1900
Wire Wire Line
	4000 1900 4150 1900
$Comp
L power:GND #PWR013
U 1 1 616320CD
P 4600 2400
F 0 "#PWR013" H 4600 2150 50  0001 C CNN
F 1 "GND" H 4605 2227 50  0000 C CNN
F 2 "" H 4600 2400 50  0001 C CNN
F 3 "" H 4600 2400 50  0001 C CNN
	1    4600 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2400 4600 2300
$Sheet
S 5300 4500 1350 650 
U 6163E871
F0 "opamp" 50
F1 "opamp.sch" 50
F2 "thermal_couple" I L 5300 4600 50 
F3 "opampOutput_tc" O R 6650 4800 50 
F4 "V5.0" I L 5300 5000 50 
$EndSheet
Wire Wire Line
	5550 2150 5550 1900
Wire Wire Line
	5550 1900 5450 1900
Text GLabel 5000 5000 0    50   Input ~ 0
V5.0
Wire Wire Line
	9350 900  9350 1250
Wire Wire Line
	8900 1250 8900 1600
$Comp
L power:GND #PWR018
U 1 1 6168F1B0
P 8800 3900
F 0 "#PWR018" H 8800 3650 50  0001 C CNN
F 1 "GND" H 8805 3727 50  0000 C CNN
F 2 "" H 8800 3900 50  0001 C CNN
F 3 "" H 8800 3900 50  0001 C CNN
	1    8800 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 3600 8800 3700
Wire Wire Line
	8700 3600 8700 3700
Wire Wire Line
	8700 3700 8800 3700
Connection ~ 8800 3700
Wire Wire Line
	8800 3700 8800 3900
Text Label 9300 3300 0    50   ~ 0
opamp_to_adc
Wire Wire Line
	6850 4800 6650 4800
Text Label 6850 4800 0    50   ~ 0
opamp_to_adc
Wire Wire Line
	9500 2600 9200 2600
Text GLabel 9350 900  2    50   Input ~ 0
V5.0
Wire Wire Line
	5000 5000 5300 5000
Text Label 5100 4600 2    50   ~ 0
thermal_couple
Wire Wire Line
	5100 4600 5300 4600
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 6169660B
P 1850 3350
F 0 "J3" H 1930 3342 50  0000 L CNN
F 1 "Thermo JST_2.54_1X02" H 1930 3251 50  0000 L CNN
F 2 "myFootPrintLibs:Molex_KK-254_AE-6410-02A_1x02_P2.54mm_Vertical" H 1850 3350 50  0001 C CNN
F 3 "~" H 1850 3350 50  0001 C CNN
F 4 "Thermocouple" H 1850 3450 50  0000 C CNN "Field4"
	1    1850 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 61697163
P 1250 3450
F 0 "#PWR03" H 1250 3200 50  0001 C CNN
F 1 "GND" H 1255 3277 50  0000 C CNN
F 2 "" H 1250 3450 50  0001 C CNN
F 3 "" H 1250 3450 50  0001 C CNN
	1    1250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3450 1250 3350
Wire Wire Line
	1250 3350 1650 3350
Text Label 1550 3500 3    50   ~ 0
thermal_couple
Wire Wire Line
	1550 3500 1550 3450
Wire Wire Line
	1550 3450 1650 3450
Text Label 1200 2550 3    50   ~ 0
heaterPower
Wire Wire Line
	1200 2550 1200 2450
Wire Wire Line
	1200 2450 1650 2450
Text Label 3350 4900 0    50   ~ 0
heaterPower
Wire Wire Line
	3350 4900 3050 4900
Text Label 8100 2300 2    50   ~ 0
enable_heater
Text Label 1250 4750 2    50   ~ 0
enable_heater
Wire Wire Line
	1250 4750 1500 4750
$Comp
L power:GND #PWR015
U 1 1 616A6C15
P 5750 1800
F 0 "#PWR015" H 5750 1550 50  0001 C CNN
F 1 "GND" H 5755 1627 50  0000 C CNN
F 2 "" H 5750 1800 50  0001 C CNN
F 3 "" H 5750 1800 50  0001 C CNN
	1    5750 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 1800 5750 1700
Wire Wire Line
	5750 1700 5450 1700
Wire Wire Line
	5550 2150 6000 2150
Wire Wire Line
	5550 2250 5550 2150
Connection ~ 5550 2150
$Comp
L power:GND #PWR014
U 1 1 616B2A08
P 5550 2600
F 0 "#PWR014" H 5550 2350 50  0001 C CNN
F 1 "GND" H 5555 2427 50  0000 C CNN
F 2 "" H 5550 2600 50  0001 C CNN
F 3 "" H 5550 2600 50  0001 C CNN
	1    5550 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2600 5550 2550
$Comp
L Device:C C8
U 1 1 616B5645
P 4600 2150
F 0 "C8" H 4715 2196 50  0000 L CNN
F 1 "22uf" H 4715 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 4638 2000 50  0001 C CNN
F 3 "~" H 4600 2150 50  0001 C CNN
	1    4600 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 2000 4150 1900
Connection ~ 4150 1900
Wire Wire Line
	4150 1900 4600 1900
$Comp
L power:GND #PWR011
U 1 1 616B9107
P 4150 2400
F 0 "#PWR011" H 4150 2150 50  0001 C CNN
F 1 "GND" H 4155 2227 50  0000 C CNN
F 2 "" H 4150 2400 50  0001 C CNN
F 3 "" H 4150 2400 50  0001 C CNN
	1    4150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 2400 4150 2300
$Comp
L Device:C C11
U 1 1 616BA986
P 6400 2400
F 0 "C11" H 6515 2446 50  0000 L CNN
F 1 "47uf" H 6515 2355 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 6438 2250 50  0001 C CNN
F 3 "~" H 6400 2400 50  0001 C CNN
	1    6400 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 616BCD2D
P 6000 2400
F 0 "C10" H 6115 2446 50  0000 L CNN
F 1 "47uf" H 6115 2355 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 6038 2250 50  0001 C CNN
F 3 "~" H 6000 2400 50  0001 C CNN
	1    6000 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2250 6000 2150
$Comp
L power:GND #PWR016
U 1 1 616BEC30
P 6000 2600
F 0 "#PWR016" H 6000 2350 50  0001 C CNN
F 1 "GND" H 6005 2427 50  0000 C CNN
F 2 "" H 6000 2600 50  0001 C CNN
F 3 "" H 6000 2600 50  0001 C CNN
	1    6000 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2600 6000 2550
$Comp
L power:GND #PWR017
U 1 1 616C03AE
P 6400 2600
F 0 "#PWR017" H 6400 2350 50  0001 C CNN
F 1 "GND" H 6405 2427 50  0000 C CNN
F 2 "" H 6400 2600 50  0001 C CNN
F 3 "" H 6400 2600 50  0001 C CNN
	1    6400 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2600 6400 2550
Text Notes 1550 2650 0    50   ~ 0
heater
$Sheet
S 1600 6350 2300 850 
U 616C338D
F0 "lcd" 50
F1 "lcd.sch" 50
F2 "sda" I L 1600 6650 50 
F3 "scl" I L 1600 6850 50 
F4 "buzzer_pwm" I L 1600 7000 50 
F5 "5V" I L 1600 6500 50 
F6 "enc_button" B R 3900 6500 50 
F7 "enc_left" B R 3900 6650 50 
F8 "enc_right" B R 3900 6800 50 
$EndSheet
Text GLabel 1400 6500 0    50   Input ~ 0
V5.0
Wire Wire Line
	1400 6500 1600 6500
Text Label 1400 6850 2    50   ~ 0
scl
Text Label 1400 6650 2    50   ~ 0
sda
Wire Wire Line
	1400 6650 1600 6650
Wire Wire Line
	1400 6850 1600 6850
Text Label 9350 3100 0    50   ~ 0
scl
Text Label 9350 3000 0    50   ~ 0
sda
Wire Wire Line
	9350 3100 9200 3100
Wire Wire Line
	9200 3000 9350 3000
Text Label 8000 2700 2    50   ~ 0
rot_button
Text Label 8000 2800 2    50   ~ 0
rot_left
Text Label 8000 2900 2    50   ~ 0
rot_right
Wire Wire Line
	8200 3100 8000 3100
Wire Wire Line
	8100 2300 8200 2300
$Comp
L Device:C C16
U 1 1 6170114A
P 10300 1450
F 0 "C16" H 10415 1496 50  0000 L CNN
F 1 "47uf" H 10415 1405 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 10338 1300 50  0001 C CNN
F 3 "~" H 10300 1450 50  0001 C CNN
	1    10300 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 61704D37
P 10300 1700
F 0 "#PWR029" H 10300 1450 50  0001 C CNN
F 1 "GND" H 10305 1527 50  0000 C CNN
F 2 "" H 10300 1700 50  0001 C CNN
F 3 "" H 10300 1700 50  0001 C CNN
	1    10300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 1700 10300 1600
$Comp
L Device:C C15
U 1 1 61706A9E
P 9850 1450
F 0 "C15" H 9965 1496 50  0000 L CNN
F 1 "47uf" H 9965 1405 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 9888 1300 50  0001 C CNN
F 3 "~" H 9850 1450 50  0001 C CNN
	1    9850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 1300 9850 1250
Wire Wire Line
	9850 1250 10300 1250
Wire Wire Line
	10300 1250 10300 1300
$Comp
L power:GND #PWR028
U 1 1 6170DA17
P 9850 1700
F 0 "#PWR028" H 9850 1450 50  0001 C CNN
F 1 "GND" H 9855 1527 50  0000 C CNN
F 2 "" H 9850 1700 50  0001 C CNN
F 3 "" H 9850 1700 50  0001 C CNN
	1    9850 1700
	1    0    0    -1  
$EndComp
NoConn ~ 8200 2000
NoConn ~ 8200 2100
NoConn ~ 8200 2200
NoConn ~ 8200 2400
NoConn ~ 8200 2600
NoConn ~ 8000 3000
NoConn ~ 9300 2400
NoConn ~ 9200 3200
NoConn ~ 9400 2700
NoConn ~ 9200 2100
NoConn ~ 9200 2000
NoConn ~ 8800 1600
Text Label 4200 6500 0    50   ~ 0
rot_button
Text Label 4250 6800 0    50   ~ 0
rot_right
Text Label 4250 6650 0    50   ~ 0
rot_left
Wire Wire Line
	4200 6500 3900 6500
Wire Wire Line
	3900 6800 4250 6800
Wire Wire Line
	4250 6650 3900 6650
Text Label 1350 7000 2    50   ~ 0
buzzerPwm
Wire Wire Line
	1350 7000 1600 7000
Text Label 8000 2500 2    50   ~ 0
buzzerPwm
Wire Wire Line
	8000 2500 8200 2500
Text Notes 7100 1100 0    50   ~ 0
Note: if only connect USB, the 5V is only 4.6V (because there is a Diode)
$Comp
L Device:C C20
U 1 1 616974EC
P 9350 1450
F 0 "C20" H 9465 1496 50  0000 L CNN
F 1 "100nf" H 9465 1405 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 9388 1300 50  0001 C CNN
F 3 "~" H 9350 1450 50  0001 C CNN
	1    9350 1450
	1    0    0    -1  
$EndComp
Text Label 9500 2600 0    50   ~ 0
20VDetect
Wire Wire Line
	9400 2700 9200 2700
Text Label 10150 5850 0    50   ~ 0
env_temp_adc
Wire Wire Line
	10150 5850 10000 5850
Text Label 8700 5400 2    50   ~ 0
enable_env_temp_check
Wire Wire Line
	8700 5400 8850 5400
Text Label 9350 2900 0    50   ~ 0
enable_env_temp_check
Wire Wire Line
	9350 2900 9200 2900
Text Label 9350 2800 0    50   ~ 0
env_temp_adc
Wire Wire Line
	9350 2800 9200 2800
Wire Wire Line
	9350 1300 9350 1250
Connection ~ 9350 1250
Wire Wire Line
	9350 1250 8900 1250
$Comp
L power:GND #PWR0103
U 1 1 61768E37
P 9350 1700
F 0 "#PWR0103" H 9350 1450 50  0001 C CNN
F 1 "GND" H 9355 1527 50  0000 C CNN
F 2 "" H 9350 1700 50  0001 C CNN
F 3 "" H 9350 1700 50  0001 C CNN
	1    9350 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 1700 9350 1600
$Comp
L Mechanical:MountingHole H1
U 1 1 6177D4EB
P 1250 950
F 0 "H1" H 1350 996 50  0000 L CNN
F 1 "MountingHole" H 1350 905 50  0000 L CNN
F 2 "myFootPrintLibs:MountingHole_2.0mm" H 1250 950 50  0001 C CNN
F 3 "~" H 1250 950 50  0001 C CNN
	1    1250 950 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 6177E0D1
P 1250 7450
F 0 "H2" H 1350 7496 50  0000 L CNN
F 1 "MountingHole" H 1350 7405 50  0000 L CNN
F 2 "myFootPrintLibs:MountingHole_2.0mm" H 1250 7450 50  0001 C CNN
F 3 "~" H 1250 7450 50  0001 C CNN
	1    1250 7450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 6177F10B
P 10650 6300
F 0 "H3" H 10750 6346 50  0000 L CNN
F 1 "MountingHole" H 10750 6255 50  0000 L CNN
F 2 "myFootPrintLibs:MountingHole_2.0mm" H 10650 6300 50  0001 C CNN
F 3 "~" H 10650 6300 50  0001 C CNN
	1    10650 6300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 617801B5
P 10350 750
F 0 "H4" H 10450 796 50  0000 L CNN
F 1 "MountingHole" H 10450 705 50  0000 L CNN
F 2 "myFootPrintLibs:MountingHole_2.0mm" H 10350 750 50  0001 C CNN
F 3 "~" H 10350 750 50  0001 C CNN
	1    10350 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2250 6400 2150
Connection ~ 6000 2150
Wire Wire Line
	8000 3000 8200 3000
Wire Wire Line
	9850 1600 9850 1700
Wire Wire Line
	9350 1250 9850 1250
Connection ~ 9850 1250
Wire Wire Line
	8200 2900 8000 2900
Wire Wire Line
	8200 2800 8000 2800
NoConn ~ 8200 3300
NoConn ~ 8000 3100
Wire Wire Line
	9300 3300 9200 3300
Wire Wire Line
	9300 2400 9200 2400
Wire Wire Line
	8000 2700 8200 2700
Text Label 7650 3200 2    50   ~ 0
relayTrigger
Wire Wire Line
	7650 3200 8200 3200
Wire Wire Line
	6000 2150 6400 2150
$Comp
L Device:CP C9
U 1 1 616A841F
P 5550 2400
F 0 "C9" H 5668 2446 50  0000 L CNN
F 1 "330uf" H 5668 2355 50  0000 L CNN
F 2 "myFootPrintLibs:CP_EIA-7343-20_Kemet-V_Pad2.25x2.55mm_HandSolder" H 5588 2250 50  0001 C CNN
F 3 "~" H 5550 2400 50  0001 C CNN
	1    5550 2400
	1    0    0    -1  
$EndComp
Text GLabel 1350 5050 0    50   Input ~ 0
20V
Wire Wire Line
	1350 5050 1500 5050
$Comp
L Diode:1N4007 D3
U 1 1 619C3549
P 3850 1900
F 0 "D3" H 3850 1683 50  0000 C CNN
F 1 "1N4007" H 3850 1774 50  0000 C CNN
F 2 "myFootPrintLibs:D_SMA_Handsoldering" H 3850 1725 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 3850 1900 50  0001 C CNN
	1    3850 1900
	-1   0    0    1   
$EndComp
Wire Wire Line
	3600 1900 3700 1900
Text GLabel 7150 2150 2    50   Input ~ 0
V5.0
Connection ~ 6400 2150
Wire Wire Line
	6400 2150 6500 2150
$Comp
L Device:D_Schottky D7
U 1 1 619C1BB0
P 6850 2150
F 0 "D7" H 6850 1933 50  0000 C CNN
F 1 "D_Schottky 5922" H 6850 2024 50  0000 C CNN
F 2 "myFootPrintLibs:D_SMA_Handsoldering" H 6850 2150 50  0001 C CNN
F 3 "~" H 6850 2150 50  0001 C CNN
	1    6850 2150
	-1   0    0    1   
$EndComp
Wire Wire Line
	7000 2150 7150 2150
Text Label 6500 1700 2    50   ~ 0
20VDetect
Wire Wire Line
	6500 1700 6500 2150
Connection ~ 6500 2150
Wire Wire Line
	6500 2150 6700 2150
Text Notes 6650 2300 0    50   ~ 0
stop back power
$Sheet
S 8850 5300 1150 750 
U 616BCAEC
F0 "NTC_room_temp" 50
F1 "NTC_room_temp.sch" 50
F2 "env_temp_adc" O R 10000 5850 50 
F3 "enable_env_temp_check" I L 8850 5400 50 
$EndSheet
Wire Wire Line
	2950 1900 3300 1900
$EndSCHEMATC
