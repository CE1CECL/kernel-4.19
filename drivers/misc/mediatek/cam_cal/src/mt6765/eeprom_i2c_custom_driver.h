/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __EEPROM_I2C_CUSTOM_DRIVER_H
#define __EEPROM_I2C_CUSTOM_DRIVER_H
#include <linux/i2c.h>

unsigned int Custom_read_region(struct i2c_client *client,
				unsigned int addr,
				unsigned char *data,
				unsigned int size);

/*Begin ersen.shang for config af otp T10017042 20210106*/
unsigned int gc8034_read_region(struct i2c_client *client, 
				unsigned int addr,
				unsigned char *data, 
				unsigned int size);

unsigned int s5k4h7_read_region(struct i2c_client *client, 
				unsigned int addr,
				unsigned char *data,
				unsigned int size);
/*End   ersen.shang for config af otp T10017042 20210106*/

#endif				/* __CAM_CAL_LIST_H */
