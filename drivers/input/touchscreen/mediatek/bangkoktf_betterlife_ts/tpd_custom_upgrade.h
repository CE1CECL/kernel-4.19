
#ifndef TOUCHPANEL_UPGRADE_H__
#define TOUCHPANEL_UPGRADE_H__

#include "tpd_bl_chip.h"

#define BL_UPDATE_FIRMWARE_ENABLE

#ifdef  BL_UPDATE_FIRMWARE_ENABLE

#define BL_AUTO_UPDATE_FARMWARE
#define BL_UPDATE_FARMWARE_FROM_BIN

#if (TS_CHIP == BL7XX0)
#define BL_CHIPINFO_BASE_OFFSET (0xbc00)
#define VERTIFY_START_OFFSET    0xbc23
#define VERTIFY_END_OFFSET      0xbc24
#elif ((TS_CHIP == BL7XX1)||(TS_CHIP == BL7XX3))
#define BL_CHIPINFO_BASE_OFFSET (0xfc00)
#define VERTIFY_START_OFFSET    0xfc23
#define VERTIFY_END_OFFSET      0xfc24
#else
#define BL_CHIPINFO_BASE_OFFSET (0x200)
#define VERTIFY_START_OFFSET    0x5FC
#define VERTIFY_END_OFFSET      0x5FD
#endif
#define BL_CHIP_ID_OFFSET       (BL_CHIPINFO_BASE_OFFSET + 0)
#define BL_FWVER_MAIN_OFFSET    (BL_CHIPINFO_BASE_OFFSET + 0x2a)
#define BL_FWVER_ARGU_OFFSET    (BL_CHIPINFO_BASE_OFFSET + 0x2b)
#define BL_FACTORY_ID_OFFSET    (BL_CHIPINFO_BASE_OFFSET + 0x2c)


#define BL_CHIP_ID_REG      (0xb8)
#define BL_FWVER_REG        (0xb6)

//#define BL_UPDATE_ARGUMENT_CHECKSUM
//#define BL_UPDATE_I2C_USE_400K_SPEED



int bl_update_fw(struct i2c_client* client);
#endif//BL_UPDATE_FIRMWARE_ENABLE

#if (TS_CHIP == BL7410)
#define BL_UPDATE_BY_I2C_MODE
#else
//#define BL_UPDATE_BY_I2C_MODE
#endif

#endif /* TOUCHPANEL_UPGRADE_H__ */
