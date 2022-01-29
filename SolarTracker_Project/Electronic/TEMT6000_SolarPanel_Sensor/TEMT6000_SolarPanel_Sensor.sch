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
L Connector_Generic:Conn_01x04 J1
U 1 1 61317962
P 4350 2650
F 0 "J1" H 4268 2225 50  0000 C CNN
F 1 "Conn_01x04" H 4268 2316 50  0000 C CNN
F 2 "Connector_JST:JST_PH_S4B-PH-SM4-TB_1x04-1MP_P2.00mm_Horizontal" H 4350 2650 50  0001 C CNN
F 3 "~" H 4350 2650 50  0001 C CNN
	1    4350 2650
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0101
U 1 1 61318294
P 4750 2350
F 0 "#PWR0101" H 4750 2200 50  0001 C CNN
F 1 "+3.3V" H 4765 2523 50  0000 C CNN
F 2 "" H 4750 2350 50  0001 C CNN
F 3 "" H 4750 2350 50  0001 C CNN
	1    4750 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 61318495
P 4750 2850
F 0 "#PWR0102" H 4750 2600 50  0001 C CNN
F 1 "GND" H 4755 2677 50  0000 C CNN
F 2 "" H 4750 2850 50  0001 C CNN
F 3 "" H 4750 2850 50  0001 C CNN
	1    4750 2850
	1    0    0    -1  
$EndComp
Text GLabel 4750 2550 2    50   Input ~ 0
Signal1
Text GLabel 4750 2650 2    50   Input ~ 0
Signal2
Wire Wire Line
	4750 2350 4750 2450
Wire Wire Line
	4750 2450 4550 2450
Wire Wire Line
	4550 2550 4750 2550
Wire Wire Line
	4750 2650 4550 2650
Wire Wire Line
	4550 2750 4750 2750
Wire Wire Line
	4750 2750 4750 2850
$Comp
L TEMT6000:TEMT6000X01 Q1
U 1 1 61319559
P 5850 2400
F 0 "Q1" H 6094 2446 50  0000 L CNN
F 1 "TEMT6000X01" H 6094 2355 50  0000 L CNN
F 2 "TEMT6000:Vishay_Semiconductors-TEMT6000X01-Manufacturer_Recommended" H 5850 3000 50  0001 L CNN
F 3 "http://www.vishay.com/docs/81579/temt6000.pdf" H 5850 3100 50  0001 L CNN
F 4 "0603" H 5850 3200 50  0001 L CNN "Case Code Imperial"
F 5 "1608" H 5850 3300 50  0001 L CNN "Case Code Metric"
F 6 "0603" H 5850 3400 50  0001 L CNN "Case Package"
F 7 "Red" H 5850 3500 50  0001 L CNN "Colour"
F 8 "Manufacturer URL" H 5850 3600 50  0001 L CNN "Component Link 1 Description"
F 9 "http://www.vishay.com/" H 5850 3700 50  0001 L CNN "Component Link 1 URL"
F 10 "Rev. 1.9, 08/2011" H 5850 3800 50  0001 L CNN "Datasheet Version"
F 11 "20 mA" H 5850 3900 50  0001 L CNN "Forward Current"
F 12 "2 V" H 5850 4000 50  0001 L CNN "Forward Voltage"
F 13 "0.8 mm" H 5850 4100 50  0001 L CNN "Height"
F 14 "Red" H 5850 4200 50  0001 L CNN "Illumination Colour"
F 15 "1.6 mm" H 5850 4300 50  0001 L CNN "Length"
F 16 "Diffused" H 5850 4400 50  0001 L CNN "Lens Style"
F 17 "54 mcd" H 5850 4500 50  0001 L CNN "Luminous Intensity"
F 18 "85 degC" H 5850 4600 50  0001 L CNN "Max Operating Temperature"
F 19 "-55 degC" H 5850 4700 50  0001 L CNN "Min Operating Temperature"
F 20 "Surface Mount" H 5850 4800 50  0001 L CNN "Mount"
F 21 "2 V" H 5850 4900 50  0001 L CNN "Nominal Vf"
F 22 "3-Pin Surface Mount Device, Body 4 x 2 mm" H 5850 5000 50  0001 L CNN "Package Description"
F 23 "3000" H 5850 5100 50  0001 L CNN "Package Quantity"
F 24 "Tape and Reel" H 5850 5200 50  0001 L CNN "Packaging"
F 25 "2" H 5850 5300 50  0001 L CNN "Pins"
F 26 "75 mW" H 5850 5400 50  0001 L CNN "Power Rating"
F 27 "No" H 5850 5500 50  0001 L CNN "Radiation Hardening"
F 28 "true" H 5850 5600 50  0001 L CNN "Ro HSCompliant"
F 29 "130 degrees" H 5850 5700 50  0001 L CNN "Viewing Angle"
F 30 "0.8 mm" H 5850 5800 50  0001 L CNN "Width"
F 31 "Trans" H 5850 5900 50  0001 L CNN "category"
F 32 "2127488" H 5850 6000 50  0001 L CNN "ciiva ids"
F 33 "8fe9385e557aea8b" H 5850 6100 50  0001 L CNN "library id"
F 34 "Vishay Semiconductors" H 5850 6200 50  0001 L CNN "manufacturer"
F 35 "1206" H 5850 6300 50  0001 L CNN "package"
F 36 "1475110345" H 5850 6400 50  0001 L CNN "release date"
F 37 "51C26A5C-1739-499A-80E2-73943D7B8DC5" H 5850 6500 50  0001 L CNN "vault revision"
F 38 "yes" H 5850 6600 50  0001 L CNN "imported"
	1    5850 2400
	1    0    0    -1  
$EndComp
$Comp
L TEMT6000:TEMT6000X01 Q2
U 1 1 6131B4A8
P 7300 2400
F 0 "Q2" H 7544 2446 50  0000 L CNN
F 1 "TEMT6000X01" H 7544 2355 50  0000 L CNN
F 2 "TEMT6000:Vishay_Semiconductors-TEMT6000X01-Manufacturer_Recommended" H 7300 3000 50  0001 L CNN
F 3 "http://www.vishay.com/docs/81579/temt6000.pdf" H 7300 3100 50  0001 L CNN
F 4 "0603" H 7300 3200 50  0001 L CNN "Case Code Imperial"
F 5 "1608" H 7300 3300 50  0001 L CNN "Case Code Metric"
F 6 "0603" H 7300 3400 50  0001 L CNN "Case Package"
F 7 "Red" H 7300 3500 50  0001 L CNN "Colour"
F 8 "Manufacturer URL" H 7300 3600 50  0001 L CNN "Component Link 1 Description"
F 9 "http://www.vishay.com/" H 7300 3700 50  0001 L CNN "Component Link 1 URL"
F 10 "Rev. 1.9, 08/2011" H 7300 3800 50  0001 L CNN "Datasheet Version"
F 11 "20 mA" H 7300 3900 50  0001 L CNN "Forward Current"
F 12 "2 V" H 7300 4000 50  0001 L CNN "Forward Voltage"
F 13 "0.8 mm" H 7300 4100 50  0001 L CNN "Height"
F 14 "Red" H 7300 4200 50  0001 L CNN "Illumination Colour"
F 15 "1.6 mm" H 7300 4300 50  0001 L CNN "Length"
F 16 "Diffused" H 7300 4400 50  0001 L CNN "Lens Style"
F 17 "54 mcd" H 7300 4500 50  0001 L CNN "Luminous Intensity"
F 18 "85 degC" H 7300 4600 50  0001 L CNN "Max Operating Temperature"
F 19 "-55 degC" H 7300 4700 50  0001 L CNN "Min Operating Temperature"
F 20 "Surface Mount" H 7300 4800 50  0001 L CNN "Mount"
F 21 "2 V" H 7300 4900 50  0001 L CNN "Nominal Vf"
F 22 "3-Pin Surface Mount Device, Body 4 x 2 mm" H 7300 5000 50  0001 L CNN "Package Description"
F 23 "3000" H 7300 5100 50  0001 L CNN "Package Quantity"
F 24 "Tape and Reel" H 7300 5200 50  0001 L CNN "Packaging"
F 25 "2" H 7300 5300 50  0001 L CNN "Pins"
F 26 "75 mW" H 7300 5400 50  0001 L CNN "Power Rating"
F 27 "No" H 7300 5500 50  0001 L CNN "Radiation Hardening"
F 28 "true" H 7300 5600 50  0001 L CNN "Ro HSCompliant"
F 29 "130 degrees" H 7300 5700 50  0001 L CNN "Viewing Angle"
F 30 "0.8 mm" H 7300 5800 50  0001 L CNN "Width"
F 31 "Trans" H 7300 5900 50  0001 L CNN "category"
F 32 "2127488" H 7300 6000 50  0001 L CNN "ciiva ids"
F 33 "8fe9385e557aea8b" H 7300 6100 50  0001 L CNN "library id"
F 34 "Vishay Semiconductors" H 7300 6200 50  0001 L CNN "manufacturer"
F 35 "1206" H 7300 6300 50  0001 L CNN "package"
F 36 "1475110345" H 7300 6400 50  0001 L CNN "release date"
F 37 "51C26A5C-1739-499A-80E2-73943D7B8DC5" H 7300 6500 50  0001 L CNN "vault revision"
F 38 "yes" H 7300 6600 50  0001 L CNN "imported"
	1    7300 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 6131CC75
P 6050 2100
F 0 "#PWR0103" H 6050 1950 50  0001 C CNN
F 1 "+3.3V" H 6065 2273 50  0000 C CNN
F 2 "" H 6050 2100 50  0001 C CNN
F 3 "" H 6050 2100 50  0001 C CNN
	1    6050 2100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 6131CDF3
P 7500 2100
F 0 "#PWR0104" H 7500 1950 50  0001 C CNN
F 1 "+3.3V" H 7515 2273 50  0000 C CNN
F 2 "" H 7500 2100 50  0001 C CNN
F 3 "" H 7500 2100 50  0001 C CNN
	1    7500 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 6131D988
P 6050 2900
F 0 "R1" H 6118 2946 50  0000 L CNN
F 1 "10k" H 6118 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6090 2890 50  0001 C CNN
F 3 "~" H 6050 2900 50  0001 C CNN
	1    6050 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 6131DCC5
P 7500 2900
F 0 "R2" H 7568 2946 50  0000 L CNN
F 1 "10k" H 7568 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7540 2890 50  0001 C CNN
F 3 "~" H 7500 2900 50  0001 C CNN
	1    7500 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 6131DF9B
P 6050 3200
F 0 "#PWR0105" H 6050 2950 50  0001 C CNN
F 1 "GND" H 6055 3027 50  0000 C CNN
F 2 "" H 6050 3200 50  0001 C CNN
F 3 "" H 6050 3200 50  0001 C CNN
	1    6050 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 6131E07C
P 7500 3200
F 0 "#PWR0106" H 7500 2950 50  0001 C CNN
F 1 "GND" H 7505 3027 50  0000 C CNN
F 2 "" H 7500 3200 50  0001 C CNN
F 3 "" H 7500 3200 50  0001 C CNN
	1    7500 3200
	1    0    0    -1  
$EndComp
NoConn ~ 5850 2400
NoConn ~ 7300 2400
Wire Wire Line
	6050 2200 6050 2100
Wire Wire Line
	6050 2600 6050 2700
Wire Wire Line
	6050 3050 6050 3100
Wire Wire Line
	7500 3200 7500 3100
Wire Wire Line
	7500 2750 7500 2700
Wire Wire Line
	7500 2200 7500 2100
Text GLabel 6450 2700 2    50   Input ~ 0
Signal1
Text GLabel 7900 2700 2    50   Input ~ 0
Signal2
Connection ~ 7500 2700
Wire Wire Line
	7500 2700 7500 2600
Connection ~ 6050 2700
Wire Wire Line
	6050 2700 6050 2750
$Comp
L Device:CP C1
U 1 1 618B0FE3
P 5200 2600
F 0 "C1" H 5318 2646 50  0000 L CNN
F 1 "1uF" H 5318 2555 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3216-18_Kemet-A" H 5238 2450 50  0001 C CNN
F 3 "~" H 5200 2600 50  0001 C CNN
	1    5200 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2750 4750 2750
Connection ~ 4750 2750
Wire Wire Line
	5200 2450 4750 2450
Connection ~ 4750 2450
$Comp
L Device:C C2
U 1 1 618B57FA
P 6400 2900
F 0 "C2" H 6515 2946 50  0000 L CNN
F 1 "100nF" H 6515 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6438 2750 50  0001 C CNN
F 3 "~" H 6400 2900 50  0001 C CNN
	1    6400 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2700 6400 2700
Wire Wire Line
	6400 2750 6400 2700
Connection ~ 6400 2700
Wire Wire Line
	6400 2700 6450 2700
Wire Wire Line
	6400 3050 6400 3100
Wire Wire Line
	6400 3100 6050 3100
Connection ~ 6050 3100
Wire Wire Line
	6050 3100 6050 3200
$Comp
L Device:C C3
U 1 1 618B643D
P 7850 2900
F 0 "C3" H 7965 2946 50  0000 L CNN
F 1 "100nF" H 7965 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7888 2750 50  0001 C CNN
F 3 "~" H 7850 2900 50  0001 C CNN
	1    7850 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 2700 7850 2700
Wire Wire Line
	7850 2750 7850 2700
Connection ~ 7850 2700
Wire Wire Line
	7850 2700 7900 2700
Wire Wire Line
	7850 3050 7850 3100
Wire Wire Line
	7850 3100 7500 3100
Connection ~ 7500 3100
Wire Wire Line
	7500 3100 7500 3050
$EndSCHEMATC
