// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include "cam_cal_list.h"
#include "eeprom_i2c_common_driver.h"
#include "eeprom_i2c_custom_driver.h"
#include "kd_imgsensor.h"

#define IMX586_MAX_EEPROM_SIZE 0x24D0

struct stCAM_CAL_LIST_STRUCT g_camCalList[] = {
	{IMX519_SENSOR_ID, 0xA0, Common_read_region},
	{S5K2T7SP_SENSOR_ID, 0xA4, Common_read_region},
	{IMX386_SENSOR_ID, 0xA0, Common_read_region},
	{IMX386_MONO_SENSOR_ID, 0xA0, Common_read_region},
	{IMX398_SENSOR_ID, 0xA0, Common_read_region},
	{IMX350_SENSOR_ID, 0xA0, Common_read_region},
	{IMX586_SENSOR_ID, 0xA0, Common_read_region, IMX586_MAX_EEPROM_SIZE},
	{IMX576_SENSOR_ID, 0xA2, Common_read_region},
	{S5K2L7_SENSOR_ID, 0xA0, Common_read_region},
	{IMX230_SENSOR_ID, 0xA0, Common_read_region},
	{IMX338_SENSOR_ID, 0xA0, Common_read_region},
	{IMX318_SENSOR_ID, 0xA0, Common_read_region},
	{IMX258_SENSOR_ID, 0xA0, Common_read_region},
	{S5K4E6_SENSOR_ID, 0xA8, Common_read_region},
	/*  ADD before this line */
	{0, 0, 0}	/*end of list */
};

unsigned int cam_cal_get_sensor_list(
	struct stCAM_CAL_LIST_STRUCT **ppCamcalList)
{
	if (ppCamcalList == NULL)
		return 1;

	*ppCamcalList = &g_camCalList[0];
	return 0;
}


