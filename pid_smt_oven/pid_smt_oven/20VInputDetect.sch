EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 4100 3050 2    50   Input ~ 0
20V
$Comp
L Device:R R?
U 1 1 616C0C32
P 4050 3450
AR Path="/615718CF/616C0C32" Ref="R?"  Part="1" 
AR Path="/615928EE/616C0C32" Ref="R?"  Part="1" 
AR Path="/615B8473/616C0C32" Ref="R?"  Part="1" 
AR Path="/61610B79/616C0C32" Ref="R?"  Part="1" 
AR Path="/616C0C32" Ref="R?"  Part="1" 
AR Path="/616BCAEC/616C0C32" Ref="R14"  Part="1" 
F 0 "R14" V 3843 3450 50  0000 C CNN
F 1 "24k" V 3934 3450 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 3980 3450 50  0001 C CNN
F 3 "~" H 4050 3450 50  0001 C CNN
	1    4050 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 616C0C57
P 4050 4200
AR Path="/615718CF/616C0C57" Ref="R?"  Part="1" 
AR Path="/615928EE/616C0C57" Ref="R?"  Part="1" 
AR Path="/615B8473/616C0C57" Ref="R?"  Part="1" 
AR Path="/61610B79/616C0C57" Ref="R?"  Part="1" 
AR Path="/616C0C57" Ref="R?"  Part="1" 
AR Path="/616BCAEC/616C0C57" Ref="R15"  Part="1" 
F 0 "R15" V 3843 4200 50  0000 C CNN
F 1 "6.8k" V 3934 4200 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 3980 4200 50  0001 C CNN
F 3 "~" H 4050 4200 50  0001 C CNN
	1    4050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3600 4050 3750
$Comp
L power:GND #PWR?
U 1 1 616C46F2
P 4050 4700
AR Path="/616C46F2" Ref="#PWR?"  Part="1" 
AR Path="/616BCAEC/616C46F2" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 4050 4450 50  0001 C CNN
F 1 "GND" H 4055 4527 50  0000 C CNN
F 2 "" H 4050 4700 50  0001 C CNN
F 3 "" H 4050 4700 50  0001 C CNN
	1    4050 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 4350 4050 4550
Wire Wire Line
	4050 3300 4050 3050
Wire Wire Line
	4050 3050 4100 3050
$Comp
L Device:D_Zener_Small D?
U 1 1 616C7443
P 4400 4250
AR Path="/615718CF/616C7443" Ref="D?"  Part="1" 
AR Path="/615928EE/616C7443" Ref="D?"  Part="1" 
AR Path="/615B8473/616C7443" Ref="D?"  Part="1" 
AR Path="/61610B79/616C7443" Ref="D?"  Part="1" 
AR Path="/616BCAEC/616C7443" Ref="D6"  Part="1" 
F 0 "D6" V 4354 4320 50  0000 L CNN
F 1 "4.7v" V 4445 4320 50  0000 L CNN
F 2 "myFootPrintLibs:D_MiniMELF_Handsoldering_Zener" V 4400 4250 50  0001 C CNN
F 3 "~" V 4400 4250 50  0001 C CNN
F 4 "keep 5V lowest for Pmosfet gate" V 4400 4250 50  0001 C CNN "note"
	1    4400 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 4150 4400 3950
Wire Wire Line
	4400 3950 4050 3950
Connection ~ 4050 3950
Wire Wire Line
	4050 3950 4050 4050
Wire Wire Line
	4400 4350 4400 4550
Wire Wire Line
	4400 4550 4050 4550
Connection ~ 4050 4550
Wire Wire Line
	4050 4550 4050 4700
Text HLabel 3850 3750 0    50   Input ~ 0
20VDetect
Wire Wire Line
	3850 3750 4050 3750
Connection ~ 4050 3750
Wire Wire Line
	4050 3750 4050 3950
$Comp
L Device:Thermistor_NTC TH1
U 1 1 616DA8A2
P 6950 3600
F 0 "TH1" H 7048 3646 50  0000 L CNN
F 1 "Thermistor_NTC 10k" H 7048 3555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P15.24mm_Horizontal" H 6950 3650 50  0001 C CNN
F 3 "~" H 6950 3650 50  0001 C CNN
	1    6950 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 616DC218
P 6950 4300
AR Path="/615718CF/616DC218" Ref="R?"  Part="1" 
AR Path="/615928EE/616DC218" Ref="R?"  Part="1" 
AR Path="/615B8473/616DC218" Ref="R?"  Part="1" 
AR Path="/61610B79/616DC218" Ref="R?"  Part="1" 
AR Path="/616DC218" Ref="R?"  Part="1" 
AR Path="/616BCAEC/616DC218" Ref="R16"  Part="1" 
F 0 "R16" V 6743 4300 50  0000 C CNN
F 1 "10k" V 6834 4300 50  0000 C CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" V 6880 4300 50  0001 C CNN
F 3 "~" H 6950 4300 50  0001 C CNN
	1    6950 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 3750 6950 3950
$Comp
L power:GND #PWR?
U 1 1 616DD04F
P 6950 4550
AR Path="/616DD04F" Ref="#PWR?"  Part="1" 
AR Path="/616BCAEC/616DD04F" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 6950 4300 50  0001 C CNN
F 1 "GND" H 6955 4377 50  0000 C CNN
F 2 "" H 6950 4550 50  0001 C CNN
F 3 "" H 6950 4550 50  0001 C CNN
	1    6950 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 4550 6950 4450
Text HLabel 6650 3950 0    50   Input ~ 0
env_temp_adc
Wire Wire Line
	6650 3950 6950 3950
Connection ~ 6950 3950
Wire Wire Line
	6950 3950 6950 4150
Text HLabel 7150 2750 2    50   Input ~ 0
enable_env_temp_check
$Comp
L Device:C C?
U 1 1 616FF8BA
P 6550 3300
AR Path="/616FF8BA" Ref="C?"  Part="1" 
AR Path="/616BCAEC/616FF8BA" Ref="C23"  Part="1" 
F 0 "C23" H 6665 3346 50  0000 L CNN
F 1 "100nf" H 6665 3255 50  0000 L CNN
F 2 "myFootPrintLibs:R_C_0805_2012Metric_Pad1.20x1.50mm_HandSolder" H 6588 3150 50  0001 C CNN
F 3 "~" H 6550 3300 50  0001 C CNN
	1    6550 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3150 6550 3100
Wire Wire Line
	6550 3100 6950 3100
Connection ~ 6950 3100
Wire Wire Line
	6950 3100 6950 3450
Wire Wire Line
	6950 2750 6950 3100
$Comp
L power:GND #PWR?
U 1 1 617008DD
P 6550 3500
AR Path="/617008DD" Ref="#PWR?"  Part="1" 
AR Path="/616BCAEC/617008DD" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 6550 3250 50  0001 C CNN
F 1 "GND" H 6555 3327 50  0000 C CNN
F 2 "" H 6550 3500 50  0001 C CNN
F 3 "" H 6550 3500 50  0001 C CNN
	1    6550 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3500 6550 3450
Wire Wire Line
	7150 2750 6950 2750
Text Notes 7200 4000 0    50   ~ 0
R constant is the same as R ntc, v difference is more.
Text Notes 8150 3500 0    50   ~ 0
Rconst(k)	Rntc(k)	SuppV	I(ma)	Vtemp	VtempDelta\n2	10	5	0.416666667	0.833333333	\n2	9	5	0.454545455	0.909090909	0.075757576\n5	10	5	0.333333333	1.666666667	\n5	9	5	0.357142857	1.785714286	0.119047619\n8	10	5	0.277777778	2.222222222	\n8	9	5	0.294117647	2.352941176	0.130718954\n10	10	5	0.25	2.5	\n10	9	5	0.263157895	2.631578947	0.131578947\n12	10	5	0.227272727	2.727272727	\n12	9	5	0.238095238	2.857142857	0.12987013\n
$EndSCHEMATC
