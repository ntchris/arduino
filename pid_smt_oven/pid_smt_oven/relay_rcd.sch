EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L mySymbolLibrary:S8050 Q?
U 1 1 617A259A
P 5700 4850
F 0 "Q?" H 5890 4896 50  0000 L CNN
F 1 "S8050" H 5890 4805 50  0000 L CNN
F 2 "myFootPrintLibs:SOT-23_PNP_NPN" H 5900 4775 50  0001 L CIN
F 3 "http://www.unisonic.com.tw/datasheet/S8050.pdf" H 5700 4850 50  0001 L CNN
	1    5700 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 5050 5800 5100
$Comp
L Device:R R?
U 1 1 617A25A7
P 5250 4850
F 0 "R?" V 5043 4850 50  0000 C CNN
F 1 "3k" V 5134 4850 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 5180 4850 50  0001 C CNN
F 3 "~" H 5250 4850 50  0001 C CNN
	1    5250 4850
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 4850 5500 4850
Wire Wire Line
	4750 4850 5100 4850
$Comp
L Relay:G5Q-1A K?
U 1 1 617A25B0
P 5600 3700
F 0 "K?" H 5170 3654 50  0000 R CNN
F 1 "ADW11" H 5170 3745 50  0000 R CNN
F 2 "Relay_THT:Relay_SPDT_Omron-G5LE-1" H 6925 3650 50  0001 C CNN
F 3 "https://www.panasonic-electric-works.com/pew/es/downloads/ds_dw_hl_en.pdf" H 5600 3700 50  0001 C CNN
	1    5600 3700
	-1   0    0    1   
$EndComp
Wire Wire Line
	5800 4000 5800 4350
Wire Wire Line
	5800 2300 5800 2650
$Comp
L Device:R R?
U 1 1 617A25BE
P 5800 2950
F 0 "R?" H 5730 2904 50  0000 R CNN
F 1 "50" H 5730 2995 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5730 2950 50  0001 C CNN
F 3 "~" H 5800 2950 50  0001 C CNN
	1    5800 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	5800 3100 5800 3250
$Comp
L Device:C C?
U 1 1 617A25C5
P 6450 3500
F 0 "C?" H 6565 3546 50  0000 L CNN
F 1 "220uf" H 6565 3455 50  0000 L CNN
F 2 "myFootPrintLibs:CP_EIA-7343-20_Kemet-V_Pad2.25x2.55mm_HandSolder" H 6488 3350 50  0001 C CNN
F 3 "~" H 6450 3500 50  0001 C CNN
	1    6450 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 3350 6450 3250
Wire Wire Line
	6450 3250 5800 3250
Connection ~ 5800 3250
Wire Wire Line
	5800 3250 5800 3400
$Comp
L Device:R R?
U 1 1 617A25D6
P 4650 3050
F 0 "R?" H 4580 3004 50  0000 R CNN
F 1 "1k" H 4580 3095 50  0000 R CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 4580 3050 50  0001 C CNN
F 3 "~" H 4650 3050 50  0001 C CNN
	1    4650 3050
	-1   0    0    1   
$EndComp
$Comp
L Diode:LL4148 D?
U 1 1 617A25DC
P 4650 3500
F 0 "D?" V 4604 3580 50  0000 L CNN
F 1 "D" V 4695 3580 50  0000 L CNN
F 2 "myFootPrintLibs:D_MiniMELF_Handsoldering_Zener" H 4650 3500 50  0001 C CNN
F 3 "~" H 4650 3500 50  0001 C CNN
	1    4650 3500
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 3200 4650 3350
Wire Wire Line
	4650 3650 4650 4350
Wire Wire Line
	4650 4350 5800 4350
Connection ~ 5800 4350
Wire Wire Line
	5800 4350 5800 4650
Wire Wire Line
	4650 2900 4650 2650
Wire Wire Line
	4650 2650 5800 2650
Connection ~ 5800 2650
Wire Wire Line
	5800 2650 5800 2800
Text Notes 2200 3250 0    79   ~ 0
R1 1K to discharge and release energy \nfrom coil when coil is released
Text Notes 6550 3700 0    50   ~ 0
C1 is to help coil to trigger faster and reduce current.
Text Notes 6000 2850 0    50   ~ 0
R3 is to keep coil magnet keeping the contact, \nwhich requires less current.
Text HLabel 5800 2300 2    50   Input ~ 0
5V
Text HLabel 5800 5300 2    50   BiDi ~ 0
Gnd
Text HLabel 4750 4850 0    50   Input ~ 0
relayTrigger
Text HLabel 5400 3100 0    50   Input ~ 0
highVolIn
Wire Wire Line
	5400 3100 5400 3400
Text HLabel 5300 4050 0    50   Input ~ 0
highVolOut
Wire Wire Line
	5300 4050 5400 4050
Wire Wire Line
	5400 4050 5400 4000
Wire Wire Line
	6450 3650 6450 5100
Wire Wire Line
	6450 5100 5800 5100
Connection ~ 5800 5100
Wire Wire Line
	5800 5100 5800 5300
$EndSCHEMATC
