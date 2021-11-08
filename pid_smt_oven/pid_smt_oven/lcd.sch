EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
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
L Connector:Conn_01x04_Female J4
U 1 1 615D2EE1
P 4750 2850
F 0 "J4" H 4778 2826 50  0000 L CNN
F 1 "JST_2.54_1X04_LCD" H 4778 2735 50  0000 L CNN
F 2 "myFootPrintLibs:Molex_KK-254_AE-6410-04A_1x04_P2.54mm_Vertical" H 4750 2850 50  0001 C CNN
F 3 "~" H 4750 2850 50  0001 C CNN
	1    4750 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 615D3EFE
P 3150 2800
F 0 "#PWR021" H 3150 2550 50  0001 C CNN
F 1 "GND" H 3155 2627 50  0000 C CNN
F 2 "" H 3150 2800 50  0001 C CNN
F 3 "" H 3150 2800 50  0001 C CNN
	1    3150 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2800 3150 2750
Wire Wire Line
	3150 2750 4550 2750
Wire Wire Line
	3550 2850 3750 2850
Text HLabel 3750 2950 0    60   Input ~ 0
sda
Text HLabel 3750 3050 0    60   Input ~ 0
scl
Text Notes 1900 900  0    60   ~ 0
LCD Display Screen 12864 with PCF i2c module
$Comp
L mySymbolLibrary:S8050 Q2
U 1 1 615E86F4
P 7650 4050
F 0 "Q2" H 7840 4096 50  0000 L CNN
F 1 "S8050" H 7840 4005 50  0000 L CNN
F 2 "myFootPrintLibs:SOT-23_PNP_NPN" H 7850 3975 50  0001 L CIN
F 3 "http://www.unisonic.com.tw/datasheet/S8050.pdf" H 7650 4050 50  0001 L CNN
	1    7650 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:Speaker LS1
U 1 1 615EA1DB
P 7950 3250
F 0 "LS1" H 8120 3246 50  0000 L CNN
F 1 "Speaker" H 8120 3155 50  0000 L CNN
F 2 "myFootPrintLibs:Buzzer_9x4.2" H 7950 3050 50  0001 C CNN
F 3 "~" H 7940 3200 50  0001 C CNN
	1    7950 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 615EA453
P 7400 2600
F 0 "#PWR027" H 7400 2350 50  0001 C CNN
F 1 "GND" H 7405 2427 50  0000 C CNN
F 2 "" H 7400 2600 50  0001 C CNN
F 3 "" H 7400 2600 50  0001 C CNN
	1    7400 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 4250 7750 4450
Wire Wire Line
	7750 3350 7750 3600
$Comp
L Device:R R11
U 1 1 615EEF56
P 7750 2750
F 0 "R11" H 7820 2796 50  0000 L CNN
F 1 "30" H 7820 2705 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 7680 2750 50  0001 C CNN
F 3 "~" H 7750 2750 50  0001 C CNN
	1    7750 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2900 7750 3050
$Comp
L Device:R R9
U 1 1 615F1D93
P 7150 4050
F 0 "R9" V 6943 4050 50  0000 C CNN
F 1 "4k" V 7034 4050 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 7080 4050 50  0001 C CNN
F 3 "~" H 7150 4050 50  0001 C CNN
	1    7150 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	7300 4050 7450 4050
Text HLabel 6500 4050 0    60   Input ~ 0
buzzer_pwm
Wire Wire Line
	6500 4050 7000 4050
Text Notes 8250 3500 0    79   ~ 0
passive buzzer 16R or 42R
$Comp
L Diode:LL4148 D4
U 1 1 615F4068
P 7350 3300
F 0 "D4" V 7304 3380 50  0000 L CNN
F 1 "1N4148" V 7395 3380 50  0000 L CNN
F 2 "myFootPrintLibs:D_MiniMELF_Handsoldering_Zener" H 7350 3125 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 7350 3300 50  0001 C CNN
	1    7350 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	7350 3050 7750 3050
Connection ~ 7750 3050
Wire Wire Line
	7750 3050 7750 3250
Wire Wire Line
	7350 3600 7750 3600
Connection ~ 7750 3600
Wire Wire Line
	7750 3600 7750 3850
Wire Wire Line
	7350 3150 7350 3050
Wire Wire Line
	7350 3450 7350 3600
Wire Wire Line
	3750 2950 4550 2950
Wire Wire Line
	3750 3050 4550 3050
Text HLabel 3550 2850 0    50   Input ~ 0
5V
Text Label 3750 2050 0    50   ~ 0
5V
Wire Wire Line
	3750 2050 3750 2150
Connection ~ 3750 2850
Wire Wire Line
	3750 2850 4550 2850
Text Label 7750 2000 0    50   ~ 0
5V
Wire Wire Line
	7750 2000 7750 2200
$Comp
L Device:CP C?
U 1 1 6171DFB2
P 3300 2300
AR Path="/6171DFB2" Ref="C?"  Part="1" 
AR Path="/616C338D/6171DFB2" Ref="C17"  Part="1" 
F 0 "C17" H 3418 2346 50  0000 L CNN
F 1 "220uf" H 3418 2255 50  0000 L CNN
F 2 "myFootPrintLibs:CP_EIA-7343-20_Kemet-V_Pad2.25x2.55mm_HandSolder" H 3338 2150 50  0001 C CNN
F 3 "~" H 3300 2300 50  0001 C CNN
	1    3300 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2150 3750 2150
Connection ~ 3750 2150
Wire Wire Line
	3750 2150 3750 2850
$Comp
L power:GND #PWR030
U 1 1 6171F627
P 3300 2500
F 0 "#PWR030" H 3300 2250 50  0001 C CNN
F 1 "GND" H 3305 2327 50  0000 C CNN
F 2 "" H 3300 2500 50  0001 C CNN
F 3 "" H 3300 2500 50  0001 C CNN
	1    3300 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2450 3300 2500
$Comp
L Device:C C?
U 1 1 61724F69
P 7400 2350
AR Path="/61724F69" Ref="C?"  Part="1" 
AR Path="/616C338D/61724F69" Ref="C18"  Part="1" 
F 0 "C18" H 7515 2396 50  0000 L CNN
F 1 "22uf" H 7515 2305 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 7438 2200 50  0001 C CNN
F 3 "~" H 7400 2350 50  0001 C CNN
	1    7400 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 2200 7750 2200
Connection ~ 7750 2200
Wire Wire Line
	7750 2200 7750 2600
$Comp
L power:GND #PWR031
U 1 1 61727C06
P 7750 4450
F 0 "#PWR031" H 7750 4200 50  0001 C CNN
F 1 "GND" H 7755 4277 50  0000 C CNN
F 2 "" H 7750 4450 50  0001 C CNN
F 3 "" H 7750 4450 50  0001 C CNN
	1    7750 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 2600 7400 2500
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 6173C26D
P 3500 4350
F 0 "J5" H 3418 3925 50  0000 C CNN
F 1 "XH2.54_1X04_RotaryEnc" H 3418 4016 50  0000 C CNN
F 2 "myFootPrintLibs:JST_XH_1x04_P2.54mm_Vertical" H 3500 4350 50  0001 C CNN
F 3 "~" H 3500 4350 50  0001 C CNN
	1    3500 4350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 6173C273
P 3750 4550
F 0 "#PWR0108" H 3750 4300 50  0001 C CNN
F 1 "GND" H 3755 4377 50  0000 C CNN
F 2 "" H 3750 4550 50  0001 C CNN
F 3 "" H 3750 4550 50  0001 C CNN
	1    3750 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4450 3750 4450
Wire Wire Line
	3750 4450 3750 4550
Wire Wire Line
	3700 4350 4400 4350
Wire Wire Line
	4400 4350 4400 4500
Wire Wire Line
	4400 4250 3700 4250
Wire Wire Line
	3700 4150 4050 4150
Text HLabel 4050 3900 2    50   Input ~ 0
enc_button
Wire Wire Line
	4050 3900 4050 4150
Text HLabel 4400 4100 2    50   Input ~ 0
enc_left
Wire Wire Line
	4400 4100 4400 4250
Text HLabel 4400 4500 2    50   Input ~ 0
enc_right
Text Notes 8100 2750 0    79   ~ 0
20R - 100R
$EndSCHEMATC
