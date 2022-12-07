/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __IMX519_EEPROM_H__
#define __IMX519_EEPROM_H__

#include "kd_camera_typedef.h"

unsigned int read_imx519_LRC(BYTE *data);

unsigned int read_imx519_DCC(BYTE *data);

#endif

