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
L Mechanical:Fiducial FID1
U 1 1 60A261CC
P 1600 3450
F 0 "FID1" H 1685 3496 50  0000 L CNN
F 1 "Artwork_girl" H 1685 3405 50  0000 L CNN
F 2 "otter:shark_girl_kani_aji_" H 1600 3450 50  0001 C CNN
F 3 "~" H 1600 3450 50  0001 C CNN
	1    1600 3450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Fiducial FID2
U 1 1 60A26521
P 1600 3650
F 0 "FID2" H 1685 3696 50  0000 L CNN
F 1 "Artwork_stars" H 1685 3605 50  0000 L CNN
F 2 "otter:stars_kani_aji_" H 1600 3650 50  0001 C CNN
F 3 "~" H 1600 3650 50  0001 C CNN
	1    1600 3650
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Fiducial FID3
U 1 1 60A26B95
P 1600 3850
F 0 "FID3" H 1685 3896 50  0000 L CNN
F 1 "Artwork_stars_mask" H 1685 3805 50  0000 L CNN
F 2 "otter:stars_mask_kani_aji_" H 1600 3850 50  0001 C CNN
F 3 "~" H 1600 3850 50  0001 C CNN
	1    1600 3850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Female J2
U 1 1 6398F814
P 6050 3350
F 0 "J2" H 6078 3376 50  0000 L CNN
F 1 "GND" H 6078 3285 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6050 3350 50  0001 C CNN
F 3 "~" H 6050 3350 50  0001 C CNN
	1    6050 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:Thermistor_NTC TH1
U 1 1 6398FA96
P 5700 3350
F 0 "TH1" V 5410 3350 50  0000 C CNN
F 1 "NTC" V 5501 3350 50  0000 C CNN
F 2 "otter:R_0402" H 5700 3400 50  0001 C CNN
F 3 "~" H 5700 3400 50  0001 C CNN
	1    5700 3350
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x01_Female J1
U 1 1 639904D2
P 5350 3350
F 0 "J1" H 5378 3284 50  0000 L CNN
F 1 "NTC" H 5378 3375 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 5350 3350 50  0001 C CNN
F 3 "~" H 5350 3350 50  0001 C CNN
	1    5350 3350
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Female J3
U 1 1 639916F5
P 6250 2000
F 0 "J3" H 6278 2026 50  0000 L CNN
F 1 "MP" H 6278 1935 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6250 2000 50  0001 C CNN
F 3 "~" H 6250 2000 50  0001 C CNN
	1    6250 2000
	1    0    0    -1  
$EndComp
NoConn ~ 6050 2000
$Comp
L Connector:Conn_01x01_Female J4
U 1 1 63991A5F
P 6250 2150
F 0 "J4" H 6278 2176 50  0000 L CNN
F 1 "MP" H 6278 2085 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6250 2150 50  0001 C CNN
F 3 "~" H 6250 2150 50  0001 C CNN
	1    6250 2150
	1    0    0    -1  
$EndComp
NoConn ~ 6050 2150
$Comp
L Connector:Conn_01x01_Female J5
U 1 1 63991D2D
P 6250 2300
F 0 "J5" H 6278 2326 50  0000 L CNN
F 1 "MP" H 6278 2235 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6250 2300 50  0001 C CNN
F 3 "~" H 6250 2300 50  0001 C CNN
	1    6250 2300
	1    0    0    -1  
$EndComp
NoConn ~ 6050 2300
$Comp
L Connector:Conn_01x01_Female J6
U 1 1 63991F87
P 6250 2450
F 0 "J6" H 6278 2476 50  0000 L CNN
F 1 "MP" H 6278 2385 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6250 2450 50  0001 C CNN
F 3 "~" H 6250 2450 50  0001 C CNN
	1    6250 2450
	1    0    0    -1  
$EndComp
NoConn ~ 6050 2450
$EndSCHEMATC
