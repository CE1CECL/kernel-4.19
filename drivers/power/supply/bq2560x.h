/*****************************************************************************
*
* Filename:
* ---------
*   bq2560x.h
*
* Project:
* --------
*   Android
*
* Description:
* ------------
*   bq2560x header file
*
* Author:
* -------
*
****************************************************************************/

#ifndef _bq2560x_SW_H_
#define _bq2560x_SW_H_

#define COMPATIBLE_WITH_HL7019

/* Enum of Voltage List */
enum BATTERY_VOLTAGE_ENUM {
	BATTERY_VOLT_00_000000_V = 0,
	BATTERY_VOLT_00_100000_V = 100000,
	BATTERY_VOLT_00_200000_V = 200000,
	BATTERY_VOLT_00_300000_V = 300000,
	BATTERY_VOLT_00_400000_V = 400000,
	BATTERY_VOLT_00_500000_V = 500000,
	BATTERY_VOLT_00_600000_V = 600000,
	BATTERY_VOLT_00_700000_V = 700000,
	BATTERY_VOLT_00_725000_V = 725000,
	BATTERY_VOLT_00_750000_V = 750000,
	BATTERY_VOLT_00_775000_V = 775000,
	BATTERY_VOLT_00_800000_V = 800000,
	BATTERY_VOLT_00_825000_V = 825000,
	BATTERY_VOLT_00_850000_V = 850000,
	BATTERY_VOLT_00_875000_V = 875000,
	BATTERY_VOLT_00_900000_V = 900000,
	BATTERY_VOLT_00_925000_V = 925000,
	BATTERY_VOLT_00_950000_V = 950000,
	BATTERY_VOLT_00_975000_V = 975000,
	BATTERY_VOLT_01_000000_V = 1000000,
	BATTERY_VOLT_01_025000_V = 1025000,
	BATTERY_VOLT_01_050000_V = 1050000,
	BATTERY_VOLT_01_075000_V = 1075000,
	BATTERY_VOLT_01_100000_V = 1100000,
	BATTERY_VOLT_01_125000_V = 1125000,
	BATTERY_VOLT_01_150000_V = 1150000,
	BATTERY_VOLT_01_175000_V = 1175000,
	BATTERY_VOLT_01_200000_V = 1200000,
	BATTERY_VOLT_01_225000_V = 1225000,
	BATTERY_VOLT_01_250000_V = 1250000,
	BATTERY_VOLT_01_275000_V = 1275000,
	BATTERY_VOLT_01_300000_V = 1300000,
	BATTERY_VOLT_01_325000_V = 1325000,
	BATTERY_VOLT_01_350000_V = 1350000,
	BATTERY_VOLT_01_375000_V = 1375000,
	BATTERY_VOLT_01_400000_V = 1400000,
	BATTERY_VOLT_01_425000_V = 1425000,
	BATTERY_VOLT_01_450000_V = 1450000,
	BATTERY_VOLT_01_475000_V = 1475000,
	BATTERY_VOLT_01_500000_V = 1500000,
	BATTERY_VOLT_01_520000_V = 1520000,
	BATTERY_VOLT_01_525000_V = 1525000,
	BATTERY_VOLT_01_540000_V = 1540000,
	BATTERY_VOLT_01_550000_V = 1550000,
	BATTERY_VOLT_01_560000_V = 1560000,
	BATTERY_VOLT_01_575000_V = 1575000,
	BATTERY_VOLT_01_580000_V = 1580000,
	BATTERY_VOLT_01_600000_V = 1600000,
	BATTERY_VOLT_01_620000_V = 1620000,
	BATTERY_VOLT_01_625000_V = 1625000,
	BATTERY_VOLT_01_640000_V = 1640000,
	BATTERY_VOLT_01_650000_V = 1650000,
	BATTERY_VOLT_01_660000_V = 1660000,
	BATTERY_VOLT_01_675000_V = 1675000,
	BATTERY_VOLT_01_680000_V = 1680000,
	BATTERY_VOLT_01_700000_V = 1700000,
	BATTERY_VOLT_01_720000_V = 1720000,
	BATTERY_VOLT_01_725000_V = 1725000,
	BATTERY_VOLT_01_740000_V = 1740000,
	BATTERY_VOLT_01_750000_V = 1750000,
	BATTERY_VOLT_01_760000_V = 1760000,
	BATTERY_VOLT_01_775000_V = 1775000,
	BATTERY_VOLT_01_780000_V = 1780000,
	BATTERY_VOLT_01_800000_V = 1800000,
	BATTERY_VOLT_01_820000_V = 1820000,
	BATTERY_VOLT_01_825000_V = 1825000,
	BATTERY_VOLT_01_840000_V = 1840000,
	BATTERY_VOLT_01_850000_V = 1850000,
	BATTERY_VOLT_01_860000_V = 1860000,
	BATTERY_VOLT_01_875000_V = 1875000,
	BATTERY_VOLT_01_880000_V = 1880000,
	BATTERY_VOLT_01_900000_V = 1900000,
	BATTERY_VOLT_01_920000_V = 1920000,
	BATTERY_VOLT_01_925000_V = 1925000,
	BATTERY_VOLT_01_940000_V = 1940000,
	BATTERY_VOLT_01_950000_V = 1950000,
	BATTERY_VOLT_01_960000_V = 1960000,
	BATTERY_VOLT_01_975000_V = 1975000,
	BATTERY_VOLT_01_980000_V = 1980000,
	BATTERY_VOLT_02_000000_V = 2000000,
	BATTERY_VOLT_02_020000_V = 2020000,
	BATTERY_VOLT_02_040000_V = 2040000,
	BATTERY_VOLT_02_050000_V = 2050000,
	BATTERY_VOLT_02_060000_V = 2060000,
	BATTERY_VOLT_02_080000_V = 2080000,
	BATTERY_VOLT_02_100000_V = 2100000,
	BATTERY_VOLT_02_120000_V = 2120000,
	BATTERY_VOLT_02_125000_V = 2125000,
	BATTERY_VOLT_02_140000_V = 2140000,
	BATTERY_VOLT_02_200000_V = 2200000,
	BATTERY_VOLT_02_275000_V = 2275000,
	BATTERY_VOLT_02_300000_V = 2300000,
	BATTERY_VOLT_02_350000_V = 2350000,
	BATTERY_VOLT_02_425000_V = 2425000,
	BATTERY_VOLT_02_400000_V = 2400000,
	BATTERY_VOLT_02_500000_V = 2500000,
	BATTERY_VOLT_02_575000_V = 2575000,
	BATTERY_VOLT_02_600000_V = 2600000,
	BATTERY_VOLT_02_650000_V = 2650000,
	BATTERY_VOLT_02_700000_V = 2700000,
	BATTERY_VOLT_02_725000_V = 2725000,
	BATTERY_VOLT_02_750000_V = 2750000,
	BATTERY_VOLT_02_800000_V = 2800000,
	BATTERY_VOLT_02_850000_V = 2850000,
	BATTERY_VOLT_02_875000_V = 2875000,
	BATTERY_VOLT_02_900000_V = 2900000,
	BATTERY_VOLT_02_950000_V = 2950000,
	BATTERY_VOLT_03_000000_V = 3000000,
	BATTERY_VOLT_03_025000_V = 3025000,
	BATTERY_VOLT_03_100000_V = 3100000,
	BATTERY_VOLT_03_175000_V = 3175000,
	BATTERY_VOLT_03_200000_V = 3200000,
	BATTERY_VOLT_03_250000_V = 3250000,
	BATTERY_VOLT_03_275000_V = 3275000,
	BATTERY_VOLT_03_300000_V = 3300000,
	BATTERY_VOLT_03_325000_V = 3325000,
	BATTERY_VOLT_03_500000_V = 3500000,
	BATTERY_VOLT_03_520000_V = 3520000,
	BATTERY_VOLT_03_540000_V = 3540000,
	BATTERY_VOLT_03_560000_V = 3560000,
	BATTERY_VOLT_03_580000_V = 3580000,
	BATTERY_VOLT_03_600000_V = 3600000,
	BATTERY_VOLT_03_620000_V = 3620000,
	BATTERY_VOLT_03_640000_V = 3640000,
	BATTERY_VOLT_03_650000_V = 3650000,
	BATTERY_VOLT_03_660000_V = 3660000,
	BATTERY_VOLT_03_680000_V = 3680000,
	BATTERY_VOLT_03_700000_V = 3700000,
	BATTERY_VOLT_03_720000_V = 3720000,
	BATTERY_VOLT_03_740000_V = 3740000,
	BATTERY_VOLT_03_760000_V = 3760000,
	BATTERY_VOLT_03_775000_V = 3775000,
	BATTERY_VOLT_03_780000_V = 3780000,
	BATTERY_VOLT_03_800000_V = 3800000,
	BATTERY_VOLT_03_820000_V = 3820000,
	BATTERY_VOLT_03_825000_V = 3825000,
	BATTERY_VOLT_03_840000_V = 3840000,
	BATTERY_VOLT_03_850000_V = 3850000,
	BATTERY_VOLT_03_860000_V = 3860000,
	BATTERY_VOLT_03_880000_V = 3880000,
	BATTERY_VOLT_03_900000_V = 3900000,
	BATTERY_VOLT_03_920000_V = 3920000,
	BATTERY_VOLT_03_940000_V = 3940000,
	BATTERY_VOLT_03_950000_V = 3950000,
	BATTERY_VOLT_03_960000_V = 3960000,
	BATTERY_VOLT_03_975000_V = 3975000,
	BATTERY_VOLT_03_980000_V = 3980000,
	BATTERY_VOLT_04_000000_V = 4000000,
	BATTERY_VOLT_04_012500_V = 4012500,
	BATTERY_VOLT_04_020000_V = 4020000,
	BATTERY_VOLT_04_025000_V = 4025000,
	BATTERY_VOLT_04_037500_V = 4037500,
	BATTERY_VOLT_04_040000_V = 4040000,
	BATTERY_VOLT_04_050000_V = 4050000,
	BATTERY_VOLT_04_060000_V = 4060000,
	BATTERY_VOLT_04_062500_V = 4062500,
	BATTERY_VOLT_04_067500_V = 4067500,
	BATTERY_VOLT_04_075000_V = 4075000,
	BATTERY_VOLT_04_080000_V = 4080000,
	BATTERY_VOLT_04_087500_V = 4087500,
	BATTERY_VOLT_04_100000_V = 4100000,
	BATTERY_VOLT_04_112500_V = 4112500,
	BATTERY_VOLT_04_115000_V = 4115000,
	BATTERY_VOLT_04_116000_V = 4116000,
	BATTERY_VOLT_04_120000_V = 4120000,
	BATTERY_VOLT_04_125000_V = 4125000,
	BATTERY_VOLT_04_137500_V = 4137500,
	BATTERY_VOLT_04_140000_V = 4140000,
	BATTERY_VOLT_04_150000_V = 4150000,
	BATTERY_VOLT_04_160000_V = 4160000,
	BATTERY_VOLT_04_162500_V = 4162500,
	BATTERY_VOLT_04_175000_V = 4175000,
	BATTERY_VOLT_04_180000_V = 4180000,
	BATTERY_VOLT_04_187500_V = 4187500,
	BATTERY_VOLT_04_200000_V = 4200000,
	BATTERY_VOLT_04_212500_V = 4212500,
	BATTERY_VOLT_04_220000_V = 4220000,
	BATTERY_VOLT_04_225000_V = 4225000,
	BATTERY_VOLT_04_237500_V = 4237500,
	BATTERY_VOLT_04_240000_V = 4240000,
	BATTERY_VOLT_04_250000_V = 4250000,
	BATTERY_VOLT_04_260000_V = 4260000,
	BATTERY_VOLT_04_262500_V = 4262500,
	BATTERY_VOLT_04_275000_V = 4275000,
	BATTERY_VOLT_04_280000_V = 4280000,
	BATTERY_VOLT_04_287500_V = 4287500,
	BATTERY_VOLT_04_300000_V = 4300000,
	BATTERY_VOLT_04_320000_V = 4320000,
	BATTERY_VOLT_04_325000_V = 4325000,
	BATTERY_VOLT_04_340000_V = 4340000,
	BATTERY_VOLT_04_350000_V = 4350000,
	BATTERY_VOLT_04_360000_V = 4360000,
	BATTERY_VOLT_04_362500_V = 4362500,
	BATTERY_VOLT_04_375000_V = 4375000,
	BATTERY_VOLT_04_380000_V = 4380000,
	BATTERY_VOLT_04_400000_V = 4400000,
	BATTERY_VOLT_04_411500_V = 4411500,
	BATTERY_VOLT_04_420000_V = 4420000,
	BATTERY_VOLT_04_425000_V = 4425000,
	BATTERY_VOLT_04_440000_V = 4440000,
	BATTERY_VOLT_04_450000_V = 4450000,
	BATTERY_VOLT_04_460000_V = 4460000,
	BATTERY_VOLT_04_480000_V = 4480000,
	BATTERY_VOLT_04_500000_V = 4500000,
	BATTERY_VOLT_04_520000_V = 4520000,
	BATTERY_VOLT_04_540000_V = 4540000,
	BATTERY_VOLT_04_550000_V = 4550000,
	BATTERY_VOLT_04_560000_V = 4560000,
	BATTERY_VOLT_04_580000_V = 4580000,
	BATTERY_VOLT_04_600000_V = 4600000,
	BATTERY_VOLT_04_620000_V = 4620000,
	BATTERY_VOLT_04_640000_V = 4640000,
	BATTERY_VOLT_04_660000_V = 4660000,
	BATTERY_VOLT_04_680000_V = 4680000,
	BATTERY_VOLT_04_700000_V = 4700000,
	BATTERY_VOLT_04_720000_V = 4720000,
	BATTERY_VOLT_04_740000_V = 4740000,
	BATTERY_VOLT_04_760000_V = 4760000,
	BATTERY_VOLT_04_780000_V = 4780000,
	BATTERY_VOLT_04_800000_V = 4800000,
	BATTERY_VOLT_04_850000_V = 4850000,
	BATTERY_VOLT_04_950000_V = 4950000,
	BATTERY_VOLT_05_000000_V = 5000000,
	BATTERY_VOLT_05_150000_V = 5150000,
	BATTERY_VOLT_05_250000_V = 5250000,
	BATTERY_VOLT_05_300000_V = 5300000,
	BATTERY_VOLT_05_450000_V = 5450000,
	BATTERY_VOLT_06_000000_V = 6000000,
	BATTERY_VOLT_06_500000_V = 6500000,
	BATTERY_VOLT_06_750000_V = 6750000,
	BATTERY_VOLT_07_000000_V = 7000000,
	BATTERY_VOLT_07_250000_V = 7250000,
	BATTERY_VOLT_07_500000_V = 7500000,
	BATTERY_VOLT_08_000000_V = 8000000,
	BATTERY_VOLT_08_500000_V = 8500000,
	BATTERY_VOLT_09_000000_V = 9000000,
	BATTERY_VOLT_09_500000_V = 9500000,
	BATTERY_VOLT_10_000000_V = 10000000,
	BATTERY_VOLT_10_500000_V = 10500000,
	BATTERY_VOLT_MAX,
	BATTERY_VOLT_INVALID
};

/* Enum of charger current List */
enum CHR_CURRENT_ENUM {
	CHARGE_CURRENT_0_00_MA = 0,
	CHARGE_CURRENT_50_00_MA = 5000,
	CHARGE_CURRENT_62_50_MA = 6250,
	CHARGE_CURRENT_70_00_MA = 7000,
	CHARGE_CURRENT_75_00_MA = 7500,
	CHARGE_CURRENT_87_50_MA = 8750,
	CHARGE_CURRENT_99_00_MA = 9900,
	CHARGE_CURRENT_100_00_MA = 10000,
	CHARGE_CURRENT_150_00_MA = 15000,
	CHARGE_CURRENT_200_00_MA = 20000,
	CHARGE_CURRENT_225_00_MA = 22500,
	CHARGE_CURRENT_250_00_MA = 25000,
	CHARGE_CURRENT_300_00_MA = 30000,
	CHARGE_CURRENT_350_00_MA = 35000,
	CHARGE_CURRENT_400_00_MA = 40000,
	CHARGE_CURRENT_425_00_MA = 42500,
	CHARGE_CURRENT_450_00_MA = 45000,
	CHARGE_CURRENT_500_00_MA = 50000,
	CHARGE_CURRENT_550_00_MA = 55000,
	CHARGE_CURRENT_600_00_MA = 60000,
	CHARGE_CURRENT_625_00_MA = 62500,
	CHARGE_CURRENT_650_00_MA = 65000,
	CHARGE_CURRENT_700_00_MA = 70000,
	CHARGE_CURRENT_750_00_MA = 75000,
	CHARGE_CURRENT_775_00_MA = 77500,
	CHARGE_CURRENT_800_00_MA = 80000,
	CHARGE_CURRENT_850_00_MA = 85000,
	CHARGE_CURRENT_900_00_MA = 90000,
	CHARGE_CURRENT_925_00_MA = 92500,
	CHARGE_CURRENT_950_00_MA = 95000,
	CHARGE_CURRENT_1000_00_MA = 100000,
	CHARGE_CURRENT_1050_00_MA = 105000,
	CHARGE_CURRENT_1075_00_MA = 107500,
	CHARGE_CURRENT_1100_00_MA = 110000,
	CHARGE_CURRENT_1150_00_MA = 115000,
	CHARGE_CURRENT_1200_00_MA = 120000,
	CHARGE_CURRENT_1225_00_MA = 122500,
	CHARGE_CURRENT_1250_00_MA = 125000,
	CHARGE_CURRENT_1300_00_MA = 130000,
	CHARGE_CURRENT_1375_00_MA = 137500,
	CHARGE_CURRENT_1400_00_MA = 140000,
	CHARGE_CURRENT_1450_00_MA = 145000,
	CHARGE_CURRENT_1500_00_MA = 150000,
	CHARGE_CURRENT_1525_00_MA = 152500,
	CHARGE_CURRENT_1600_00_MA = 160000,
	CHARGE_CURRENT_1675_00_MA = 167500,
	CHARGE_CURRENT_1700_00_MA = 170000,
	CHARGE_CURRENT_1750_00_MA = 175000,
	CHARGE_CURRENT_1800_00_MA = 180000,
	CHARGE_CURRENT_1825_00_MA = 182500,
	CHARGE_CURRENT_1900_00_MA = 190000,
	CHARGE_CURRENT_1950_00_MA = 195000,
	CHARGE_CURRENT_1975_00_MA = 197500,
	CHARGE_CURRENT_2000_00_MA = 200000,
	CHARGE_CURRENT_2025_00_MA = 202500,
	CHARGE_CURRENT_2050_00_MA = 205000,
	CHARGE_CURRENT_2100_00_MA = 210000,
	CHARGE_CURRENT_2125_00_MA = 212500,
	CHARGE_CURRENT_2175_00_MA = 217500,
	CHARGE_CURRENT_2200_00_MA = 220000,
	CHARGE_CURRENT_2300_00_MA = 230000,
	CHARGE_CURRENT_2250_00_MA = 225000,
	CHARGE_CURRENT_2275_00_MA = 227500,
	CHARGE_CURRENT_2325_00_MA = 232500,
	CHARGE_CURRENT_2350_00_MA = 235000,
	CHARGE_CURRENT_2400_00_MA = 240000,
	CHARGE_CURRENT_2425_00_MA = 242500,
	CHARGE_CURRENT_2500_00_MA = 250000,
	CHARGE_CURRENT_2575_00_MA = 257500,
	CHARGE_CURRENT_2600_00_MA = 260000,
	CHARGE_CURRENT_2650_00_MA = 265000,
	CHARGE_CURRENT_2700_00_MA = 270000,
	CHARGE_CURRENT_2725_00_MA = 272500,
	CHARGE_CURRENT_2800_00_MA = 280000,
	CHARGE_CURRENT_2875_00_MA = 287500,
	CHARGE_CURRENT_2900_00_MA = 290000,
	CHARGE_CURRENT_3000_00_MA = 300000,
	CHARGE_CURRENT_3100_00_MA = 310000,
	CHARGE_CURRENT_MAX
};


/*HL7019*/
const unsigned int VBAT_CV_VTH_HL[] = {
	3504000, 3520000, 3536000, 3552000,
	3568000, 3584000, 3600000, 3616000,
	3632000, 3648000, 3664000, 3680000,
	3696000, 3712000, 3728000, 3744000,
	3760000, 3776000, 3792000, 3808000,
	3824000, 3840000, 3856000, 3872000, 
	3888000, 3904000, 3920000, 3936000,
	3952000, 3968000, 3984000, 4000000,
	4016000, 4032010, 4048000, 4064000,
	4080000, 4096000, 4112000, 4128000,
	4144000, 4160000, 4176000, 4192000,
	4208000, 4224000, 4240000, 4256000,
	4272000, 4288000, 4304000, 4320000,
	4336000, 4352000, 4368000, 4384000,
	4400000, 4416000, 4432000, 4448000,
	4464000, 4480000, 4496000, 4512000
};

/*BQ2560x REG04 VREG[7:3]*/
//Charge Voltage
//Offset: 3.856 V
//Range: 3.856V to 4.624V(11000)
//512mV,256mV,128mV,64mV,32mV
//Default: 4.208V(01011)
//Special Value:(01111): 4.352V
//Note: Value above 11000 (4.624V) is clamped to register value 11000 (4.624 V)
const unsigned int VBAT_CV_VTH[] = {
	3856000, 3888000, 3920000, 3952000,
	3984000, 4016000, 4048000, 4080000,
	4112000, 4144000, 4176000, 4208000,
	4240000, 4272000, 4304000, 4336000,
	4368000, 4400000, 4432000, 4464000,
	4496000, 4528000, 4560000, 4592000,
	4624000
};

/* HL7019 */
const unsigned int CS_VTH_HL[] = {
	512000,		576000,	640000,		704000,	
	768000,		832000,	896000,		960000,
	1024000, 1088000, 1152000, 1216000,	
	1280000, 1344000, 1408000, 1472000,	
	1536000, 1600000, 1664000, 1728000,	
	1792000, 1856000, 1920000, 1984000,	
	2048000, 2112000, 2176000, 2240000,	
	2304000, 2368000, 2432000, 2496000,	
	2560000, 2624000, 2688000, 2752000,	
	2816000, 2880000, 2944000, 3008000,	
	3072000, 3136000, 3200000, 3264000,	
	3328000, 3392000, 3456000, 3520000,	
	3584000, 3648000, 3712000, 3776000,	
	3840000, 3904000, 3968000, 4032000,	
	4096000, 4160000, 4224000, 4288000,	
	4352000, 4416000, 4480000, 4544000,	
	4608000, 4672000, 4736000, 4800000,	
	4864000, 4928000, 4992000, 5056000
};

/*BQ2560x REG02 ICHG[5:0]*/
//Fast Charge Current
//Default: 2040mA (100010)
//1920mA,960mA,480mA,240mA,120mA,60mA
//Range: 0 mA (0000000) ?3000mA (110010)
//Note:ICHG = 0 mA disables charge.
//ICHG > 3000 mA (110010 clamped
//to register value 3000 mA(110010))
const unsigned int CS_VTH[] = {
	0,				60000,		120000,		180000,
	240000,		300000,		360000,		420000,
	480000,		540000,		600000,		660000,
	720000,		780000,		840000,		900000,
	960000,		1020000,	1080000,	1140000,
	1200000,	1260000,	1320000,	1380000,
	1440000,	1500000,	1560000,	1620000,
	1680000,	1740000,	1800000,	1860000,
	1920000,	1980000,	2040000,	2100000,
	2160000,	2220000,	2280000,	2340000,
	2400000,	2460000,	2520000,	2580000,
	2640000,	2700000,	2760000,	2820000,
	2880000,	2940000,	3000000
};

/*BQ2560x REG00 IINLIM[4:0]*/
//Input Current Limit
//Offset: 100 mA
//1600mA,800mA,400mA,200mA,100mA
//Range: 100mA(000000) \A8C 3.2A(11111)
//Default:2400 mA (10111),maximum input current limit, not typical.
//IINDPM bits are changed automatically after input source detection is completed
//PSEL = Hi = 500 mA
//PSEL = Lo = 2.4 A
//Host can over-write IINDPM register bits after input source detection is completed.
const unsigned int INPUT_CS_VTH[] = {
	CHARGE_CURRENT_0_00_MA,/* Added by bitao.xiong for task-8430235 on 2019-10-15 */
	CHARGE_CURRENT_100_00_MA,
	CHARGE_CURRENT_200_00_MA,
	CHARGE_CURRENT_300_00_MA,
	CHARGE_CURRENT_400_00_MA,
	CHARGE_CURRENT_500_00_MA,
	CHARGE_CURRENT_600_00_MA,
	CHARGE_CURRENT_700_00_MA,
	CHARGE_CURRENT_800_00_MA,
	CHARGE_CURRENT_900_00_MA,
	CHARGE_CURRENT_1000_00_MA,
	CHARGE_CURRENT_1100_00_MA,
	CHARGE_CURRENT_1200_00_MA,
	CHARGE_CURRENT_1300_00_MA,
	CHARGE_CURRENT_1400_00_MA,
	CHARGE_CURRENT_1500_00_MA,
	CHARGE_CURRENT_1600_00_MA,
	CHARGE_CURRENT_1700_00_MA,
	CHARGE_CURRENT_1800_00_MA,
	CHARGE_CURRENT_1900_00_MA,
	CHARGE_CURRENT_2000_00_MA,
	CHARGE_CURRENT_2100_00_MA,
	CHARGE_CURRENT_2200_00_MA,
	CHARGE_CURRENT_2300_00_MA,
	CHARGE_CURRENT_2400_00_MA,
	CHARGE_CURRENT_2500_00_MA,
	CHARGE_CURRENT_2600_00_MA,
	CHARGE_CURRENT_2700_00_MA,
	CHARGE_CURRENT_2800_00_MA,
	CHARGE_CURRENT_2900_00_MA,
	CHARGE_CURRENT_3000_00_MA,
	CHARGE_CURRENT_3100_00_MA,
	CHARGE_CURRENT_MAX
};

const unsigned int INPUT_CS_VTH_HL[] = {
	100000, 150000,500000, 900000, 
	1000000, 1500000,2000000,3000000
};

const unsigned int VCDT_HV_VTH[] = {
	BATTERY_VOLT_04_200000_V,
	BATTERY_VOLT_04_250000_V,
	BATTERY_VOLT_04_300000_V,
	BATTERY_VOLT_04_350000_V,
	BATTERY_VOLT_04_400000_V,
	BATTERY_VOLT_04_450000_V,
	BATTERY_VOLT_04_500000_V,
	BATTERY_VOLT_04_550000_V,
	BATTERY_VOLT_04_600000_V,
	BATTERY_VOLT_06_000000_V,
	BATTERY_VOLT_06_500000_V,
	BATTERY_VOLT_07_000000_V,
	BATTERY_VOLT_07_500000_V,
	BATTERY_VOLT_08_500000_V,
	BATTERY_VOLT_09_500000_V,
	BATTERY_VOLT_10_500000_V
};

/* Begin added by bitao.xiong for task-8219984 on 2019-08-06 */
const unsigned int VINDPM_REG[] = {
	3900, 4000, 4100, 4200, 4300, 4400,
	4500, 4600, 4700, 4800, 4900, 5000,
	5100, 5200, 5300, 5400, 5500, 5600,
	5700, 5800, 5900, 6000, 6100, 6200,
	6300, 6400
};

const unsigned int VINDPM_REG_HL[] = {
	3880, 3970, 4030, 4120, 4200, 4290,
	4350, 4400, 4520, 4610, 4670, 4760,
	4840, 4930, 4990, 5080, 8320, 8500,
	8640, 8820, 9010, 9190, 9330, 9510,
	9690, 9870, 10010, 10190,10380, 10560,
	10700, 10880
};
/* End added by bitao.xiong for task-8219984 on 2019-08-06 */

/**********************************************************
  *
  *   [REGISTER] 
  *
  *********************************************************/
#define bq2560x_CON0		0x00
#define bq2560x_CON1		0x01
#define bq2560x_CON2		0x02
#define bq2560x_CON3		0x03
#define bq2560x_CON4		0x04
#define bq2560x_CON5		0x05
#define bq2560x_CON6		0x06
#define bq2560x_CON7		0x07
#define bq2560x_CON8		0x08
#define bq2560x_CON9		0x09
#define bq2560x_CON10		0x0A
#define bq2560x_CON11		0x0B
#define bq2560x_REG_NUM		12
#ifdef COMPATIBLE_WITH_HL7019
#define hl7019_CON12		0x0C
#define hl7019_CON13		0x0D
#define hl7019_REG_NUM		14
#endif
/**********************************************************
  *
  *   [MASK/SHIFT] 
  *
  *********************************************************/
//CON0
#define CON0_EN_HIZ_MASK   0x01
#define CON0_EN_HIZ_SHIFT  7

#define	CON0_STAT_IMON_CTRL	0x03
#define	CON0_STAT_IMON_SHIFT 5

#define CON0_IINLIM_MASK   0x1F
#define CON0_IINLIM_SHIFT  0

#ifdef COMPATIBLE_WITH_HL7019
	#define	CON0_VINDPM_HL_MASK	0x0F
	#define	CON0_VINDPM_HL_SHIFT	3
	
	#define CON0_IINLIM_HL_MASK   0x07
	#define CON0_IINLIM_HL_SHIFT  0
#endif
//CON1
#define CON1_PFM_DIS_MASK     0x01
#define CON1_PFM_DIS_SHIFT    7

#ifdef COMPATIBLE_WITH_HL7019
	#define CON1_REG_RST_MASK 0x01
	#define CON1_REG_RST_SHIFT 7
#endif

#define CON1_WDT_RST_MASK     0x01
#define CON1_WDT_RST_SHIFT    6

#define CON1_OTG_CONFIG_MASK	0x01
#define CON1_OTG_CONFIG_SHIFT	5

#define CON1_CHG_CONFIG_MASK        0x01
#define CON1_CHG_CONFIG_SHIFT       4

#define CON1_SYS_MIN_MASK        0x07
#define CON1_SYS_MIN_SHIFT       1

#define	CON1_MIN_VBAT_SEL_MASK	0x01
#define	CON1_MIN_VBAT_SEL_SHIFT	0

#ifdef COMPATIBLE_WITH_HL7019
	#define CON1_BOOST_LIM_HL_MASK  0x01
	#define CON1_BOOST_LIM_HL_SHIFT  0
#endif

//CON2
#define CON2_BOOST_LIM_MASK   0x01
#define CON2_BOOST_LIM_SHIFT  7

#define	CON2_Q1_FULLON_MASK		0x01
#define	CON2_Q1_FULLON_SHIFT	6

#define CON2_ICHG_MASK    0x3F
#define CON2_ICHG_SHIFT   0

#ifdef COMPATIBLE_WITH_HL7019
	#define CON2_ICHG_HL_MASK    0x3F
	#define CON2_ICHG_HL_SHIFT   2
	#define CON2_BCOLD_HL_MASK    0x1
	#define CON2_BCOLD_HL_SHIFT   1
	#define CON2_FORCE_20PCT_HL_MASK    0x1
	#define CON2_FORCE_20PCT_HL_SHIFT   0
#endif
//CON3
#define CON3_IPRECHG_MASK   0x0F
#define CON3_IPRECHG_SHIFT  4

#define CON3_ITERM_MASK           0x0F
#define CON3_ITERM_SHIFT          0

//CON4
#define CON4_VREG_MASK     0x1F
#define CON4_VREG_SHIFT    3
#ifdef COMPATIBLE_WITH_HL7019
	#define CON4_VREG_HL_MASK     0x3F
	#define CON4_VREG_HL_SHIFT    2
#endif

#define	CON4_TOPOFF_TIMER_MASK 0x03
#define	CON4_TOPOFF_TIMER_SHIFT 1

#ifdef COMPATIBLE_WITH_HL7019
	#define	CON4_MIN_VBAT_SEL_HL_MASK	0x01
	#define	CON4_MIN_VBAT_SEL_HL_SHIFT	1
#endif

#define CON4_VRECHG_MASK    0x01
#define CON4_VRECHG_SHIFT   0


//CON5
#define CON5_EN_TERM_MASK      0x01
#define CON5_EN_TERM_SHIFT     7

#ifdef COMPATIBLE_WITH_HL7019
#define	CON5_STAT_IMON_HL_CTRL	0x01
#define	CON5_STAT_IMON_HL_SHIFT 6
#endif

#define CON5_WATCHDOG_MASK     0x03
#define CON5_WATCHDOG_SHIFT    4

#define CON5_EN_TIMER_MASK      0x01
#define CON5_EN_TIMER_SHIFT     3

#define CON5_CHG_TIMER_MASK     0x01
#define CON5_CHG_TIMER_SHIFT    2

#ifdef COMPATIBLE_WITH_HL7019
	#define CON5_CHG_TIMER_HL_MASK     0x03
	#define CON5_CHG_TIMER_HL_SHIFT    1
#endif

#define CON5_TREG_MASK     0x01
#define CON5_TREG_SHIFT    1

#define CON5_JEITA_ISET_MASK     0x01
#define CON5_JEITA_ISET_SHIFT    0

//CON6
#define	CON6_OVP_MASK		0x03
#define	CON6_OVP_SHIFT		6

#define	CON6_BOOSTV_MASK	0x03
#define	CON6_BOOSTV_SHIFT	4

#define	CON6_VINDPM_MASK	0x0F
#define	CON6_VINDPM_SHIFT	0

#ifdef COMPATIBLE_WITH_HL7019
	#define	CON6_BOOSTV_HL_MASK	0x0f
	#define	CON6_BOOSTV_HL_SHIFT	4
	#define	CON6_BHOT_HL_MASK	0x03
	#define	CON6_BHOT_HL_SHIFT	2
	#define	CON6_TREG_HL_MASK	0x03
	#define	CON6_TREG_HL_SHIFT	0
#endif

//CON7
#define	CON7_FORCE_DPDM_MASK	0x01
#define	CON7_FORCE_DPDM_SHIFT	7

#define CON7_TMR2X_EN_MASK      0x01
#define CON7_TMR2X_EN_SHIFT     6

#define CON7_BATFET_DISABLE_MASK      0x01
#define CON7_BATFET_DISABLE_SHIFT     5

#define CON7_JEITA_VSET_MASK      0x01
#define CON7_JEITA_VSET_SHIFT     4

#define	CON7_BATFET_DLY_MASK		0x01
#define	CON7_BATFET_DLY_SHIFT		3

#define	CON7_BATFET_RST_EN_MASK		0x01
#define	CON7_BATFET_RST_EN_SHIFT	2

#define	CON7_VDPM_BAT_TRACK_MASK	0x03
#define	CON7_VDPM_BAT_TRACK_SHIFT	0

#ifdef COMPATIBLE_WITH_HL7019
	#define	CON7_INT_MASK_HL_MASK			0x03
	#define	CON7_INT_MASK_HL_SHIFT			0
#endif
//CON8
#define CON8_VBUS_STAT_MASK      0x07
#define CON8_VBUS_STAT_SHIFT     5

#define CON8_CHRG_STAT_MASK           0x03
#define CON8_CHRG_STAT_SHIFT          3
#ifdef COMPATIBLE_WITH_HL7019
#define CON8_CHRG_STAT_HL_MASK           0x03
#define CON8_CHRG_STAT_HL_SHIFT          4
#endif
#define CON8_PG_STAT_MASK           0x01
#define CON8_PG_STAT_SHIFT          2

#define CON8_THERM_STAT_MASK           0x01
#define CON8_THERM_STAT_SHIFT          1

#define CON8_VSYS_STAT_MASK           0x01
#define CON8_VSYS_STAT_SHIFT          0

//CON9
#define CON9_WATCHDOG_FAULT_MASK      0x01
#define CON9_WATCHDOG_FAULT_SHIFT     7

#define CON9_OTG_FAULT_MASK           0x01
#define CON9_OTG_FAULT_SHIFT          6

#define CON9_CHRG_FAULT_MASK           0x03
#define CON9_CHRG_FAULT_SHIFT          4

#define CON9_BAT_FAULT_MASK           0x01
#define CON9_BAT_FAULT_SHIFT          3

#define CON9_NTC_FAULT_MASK           0x07
#define CON9_NTC_FAULT_SHIFT          0

//CON10
#define	CON10_VBUS_GD_MASK				0x01
#define	CON10_VBUS_GD_SHIFT				7

#define	CON10_VINDPM_STAT_MASK			0x01
#define	CON10_VINDPM_STAT_SHIFT			6

#define	CON10_IINDPM_STAT_MASK			0x01
#define	CON10_IINDPM_STAT_SHIFT			5

#define	CON10_TOPOFF_ACTIVE_MASK		0x01
#define	CON10_TOPOFF_ACTIVE_SHIFT		3

#define	CON10_ACOV_STAT_MASK			0x01
#define	CON10_ACOV_STAT_SHIFT			2

#define	CON10_VINDPM_INT_MASK			0x01
#define	CON10_VINDPM_INT_SHIFT			1

#define	CON10_IINDPM_INT_MASK			0x01
#define	CON10_IINDPM_INT_SHIFT			0

#define	CON10_INT_MASK_MASK				0x03
#define	CON10_INT_MASK_SHIFT			0

#ifdef COMPATIBLE_WITH_HL7019
	#define HL7019_CON_PN			bq2560x_CON10
	#define HL7019_PN_MASK			0xFF
	#define HL7019_PN_SHIFT			0
	#define HL7019_CON_0C			0x0C
	#define CON12_RESERVE_MASK			0xFF
	#define CON12_RESERVE_SHIFT		0
	/* Begin added by bitao.xiong for task-8430235 on 2019-12-02 */
	#define CON12_IR_R_MASK			0x07
	#define CON12_IR_R_SHIFT 		5
	#define CON12_IR_VOL_MASK		0x07
	#define CON12_IR_VOL_SHIFT 		2
	/* End added by bitao.xiong for task-8430235 on 2019-12-02 */
	#define NO_MASK			0xFF
	#define NO_SHIFT		0
#endif
//CON11
#define CON11_REG_RST_MASK     0x01
#define CON11_REG_RST_SHIFT    7

#define CON11_PN_MASK		0x0F
#define CON11_PN_SHIFT		3

#define CON11_Rev_MASK           0x03
#define CON11_Rev_SHIFT          0

/* Begin added by bitao.xiong for defect-8423557 on 2019-10-16 */
#define CON11_Reserved_MASK		0x01
#define CON11_Reserved_SHIFT 	2
/* End added by bitao.xiong for defect-8423557 on 2019-10-16 */

/**********************************************************
  *
  *   [Extern Function] 
  *
  *********************************************************/
//CON0----------------------------------------------------
extern void bq2560x_set_en_hiz(unsigned int val);
extern void bq2560x_set_stat_ctrl(unsigned int val);
extern void bq2560x_set_iinlim(unsigned int val);
//CON1----------------------------------------------------
extern void bq2560x_set_wdt_rst(unsigned int val);
extern void bq2560x_set_otg_config(unsigned int val);
extern void bq2560x_set_chg_config(unsigned int val);
extern void bq2560x_set_sys_min(unsigned int val);
extern void bq2560x_set_batlowv(unsigned int val);
//CON2----------------------------------------------------
extern void bq2560x_set_boost_lim(unsigned int val);
extern void bq2560x_set_Q1_FULLON(unsigned int val);
extern void bq2560x_set_ichg(unsigned int val);
//CON3----------------------------------------------------
extern void bq2560x_set_iprechg(unsigned int val);
extern void bq2560x_set_iterm(unsigned int val);
//CON4----------------------------------------------------
extern void bq2560x_set_vreg(unsigned int val);
extern void bq2560x_set_topoff_timer(unsigned int val);
extern void bq2560x_set_vrechg(unsigned int val);
//CON5----------------------------------------------------
extern void bq2560x_set_en_term(unsigned int val);
extern void bq2560x_set_watchdog(unsigned int val);
extern void bq2560x_set_en_timer(unsigned int val);
extern void bq2560x_set_chg_timer(unsigned int val);
extern void bq2560x_set_treg(unsigned int val);
//CON6----------------------------------------------------
extern void bq2560x_set_ovp(unsigned int val);
extern void bq2560x_set_boostv(unsigned int vall);
extern void bq2560x_set_vindpm(unsigned int val);
//CON7----------------------------------------------------
extern void bq2560x_set_tmr2x_en(unsigned int val);
extern void bq2560x_set_batfet_disable(unsigned int val);
extern void bq2560x_set_batfet_delay(unsigned int val);
extern void bq2560x_set_batfet_reset_enable(unsigned int val);
extern void bq2560x_set_VDPM_bat_track(unsigned int val);
//CON8----------------------------------------------------
extern unsigned int bq2560x_get_system_status(void);
extern unsigned int bq2560x_get_vbus_state(void);
extern unsigned int bq2560x_get_chrg_state(void);
extern unsigned int bq2560x_get_pg_state(void);
extern unsigned int bq2560x_get_therm_state(void);
extern unsigned int bq2560x_get_vsys_state(void);
//CON9-----------------------------------------------------
extern unsigned int bq2560x_get_default_state(void);
extern unsigned int bq2560x_get_wdt_state(void );
extern unsigned int bq2560x_get_boost_state(void );
extern unsigned int bq2560x_get_chrg_fault_state(void);
extern unsigned int bq2560x_get_bat_state(void);
extern unsigned int bq2560x_get_NTC_state(void);
//CON10----------------------------------------------------
extern unsigned int bq2560x_get_VbusGood_state(void );
extern unsigned int bq2560x_get_TOPOFF_ACTIVE_state(void );
extern unsigned int bq2560x_get_ACOV_state(void );
extern unsigned int bq2560x_get_VINDPM_state(void );
extern unsigned int bq2560x_get_IINDPM_state(void );
extern void bq2560x_set_int_mask(unsigned int val);
//CON11----------------------------------------------------
extern void bq2560x_set_reg_rst(unsigned int val);
extern unsigned int bq2560x_get_reg_rst(void);
extern unsigned char bq2560x_get_PN(void);
//---------------------------------------------------------
extern void bq2560x_dump_register(void);
extern unsigned int bq2560x_reg_config_interface (unsigned char RegNum, unsigned char val);

extern unsigned int bq2560x_read_interface (unsigned char RegNum, unsigned char *val, unsigned char MASK, unsigned char SHIFT);
extern unsigned int bq2560x_config_interface (unsigned char RegNum, unsigned char val, unsigned char MASK, unsigned char SHIFT);

//#ifdef COMPATIBLE_WITH_HL7019
///* CON0---------------------------------------------------- */
///* CON1---------------------------------------------------- */
//void HL701x_set_reg_rst(unsigned char val);
//void HL701x_set_batlowv(unsigned char val);
//
///* CON2---------------------------------------------------- */
//void HL701x_set_boost_lim(unsigned char val);
//
///* CON3---------------------------------------------------- */
///* CON4---------------------------------------------------- */
//void HL701x_set_topoff_timer(unsigned char val);
///* CON5---------------------------------------------------- */
///* CON6---------------------------------------------------- */
///* CON7---------------------------------------------------- */
///* CON8---------------------------------------------------- */
///*CON10----------------------------------------------------------*/
//void HL701x_set_int_mask(unsigned char val);
//#endif

#endif // _bq2560x_SW_H_

