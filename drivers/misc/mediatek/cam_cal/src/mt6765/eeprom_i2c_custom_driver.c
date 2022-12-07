// SPDX-License-Identifier: GPL-2.0
#define PFX "CAM_CAL"
#define pr_fmt(fmt) PFX "[%s] " fmt, __func__


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/of.h>
#include "cam_cal.h"
#include "cam_cal_define.h"
#include "cam_cal_list.h"
#include <linux/dma-mapping.h>
#ifdef CONFIG_COMPAT
/* 64 bit */
#include <linux/fs.h>
#include <linux/compat.h>
#endif

#define EEPROM_I2C_MSG_SIZE_READ 2

/*Begin ersen.shang for config af otp T10017042 20210106*/
#define GC8034_OTP_ID_SIZE           9
struct gc8034_dd_t {
	u16 x;
	u16 y;
	u16 t;
};

struct gc8034_otp_t {
	u8  otp_id[GC8034_OTP_ID_SIZE];
	u8  dd_cnt;
	u8  dd_flag;
	struct gc8034_dd_t dd_param[160];
	u8  reg_flag;
	u8  reg_num;
	u8  reg_page[10];
	u8  reg_addr[10];
	u8  reg_value[10];
	u8  module_id;
	u8  lens_id;
	u8  vcm_id;
	u8  vcm_driver_id;
	u8  year;
	u8  month;
	u8  day;
	u8  af_flag;
	u16 af_infinity;
	u16 af_macro;
	u8  wb_flag;
	u16 rg_gain;
	u16 bg_gain;
	u8  golden_flag;
	u16 golden_rg;
	u16 golden_bg;
	u8  lsc_flag;		/* 0:Empty 1:Success 2:Invalid */
	u8  lsc_param[396];
};

typedef struct module_info_struct
{
    u8 mid;
    u8 afflag;
    u8 year;
    u8 month;
    u8 day;
    u8 lensid;
    u8 vcmid;
    u8 drivericid;
    u8 operationflag;
    u8 reserved[6];
} module_info __attribute__((aligned(1)));

typedef struct awb_info_struct
{
    u8 r;
    u8 gr;
    u8 gb;
    u8 b;
    u8 golden_r;
    u8 golden_gr;
    u8 golden_gb;
    u8 golden_b;
} awb_info __attribute__((aligned(1)));

typedef struct af_info_struct
{
    u8 macro_h;
    u8 macro_l;
    u8 inf_h;
    u8 inf_l;
    u8 reserved[2];
} af_info __attribute__((aligned(1)));

typedef struct otp_struct
{
    u8 module_info_flag;
    module_info module_info_group1;
    u8 module_info_group1_chksum;
    module_info module_info_group2;
    u8 module_info_group2_chksum;

    u8 awb_flag;
    awb_info awb_group1;
    u8 awb_group1_chksum;
    awb_info awb_group2;
    u8 awb_group2_chksum;

    u8 af_flag;
    af_info af_group1;
    u8 af_group1_chksum;
    af_info af_group2;
    u8 af_group2_chksum;
    u8 lsc_flag;
    u8 lsc1_chksum;
    u8 lsc2_chksum;

    u8 lsc_buffer1[360];
    u8 lsc_buffer2[360];

    u16 rg;
    u16 bg;
    u16 rg_ratio_typical;
    u16 bg_ratio_typical;
} otp_data __attribute__((aligned(1)));

extern u8 otp_buffer[0x800];
extern struct gc8034_otp_t gc8034_otp_info;
extern unsigned int s5k4h7_otp_status;
#define EEPROM_AF_FLAG (0x0080)
#define EEPROM_AF_CHKSUM (0x0100)
/*End   ersen.shang for config af otp T10017042 20210106*/

static DEFINE_SPINLOCK(g_spinLock);
static struct i2c_client *g_pstI2CclientG;

 #define PAGE_SIZE_ 256
static int iReadRegI2C(u8 *a_pSendData, u16 a_sizeSendData,
		u8 *a_pRecvData, u16 a_sizeRecvData, u16 i2cId)
{
	int  i4RetValue = 0;
	struct i2c_msg msg[EEPROM_I2C_MSG_SIZE_READ];

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr = (i2cId >> 1);
	spin_unlock(&g_spinLock);

	msg[0].addr = g_pstI2CclientG->addr;
	msg[0].flags = g_pstI2CclientG->flags & I2C_M_TEN;
	msg[0].len = a_sizeSendData;
	msg[0].buf = a_pSendData;

	msg[1].addr = g_pstI2CclientG->addr;
	msg[1].flags = g_pstI2CclientG->flags & I2C_M_TEN;
	msg[1].flags |= I2C_M_RD;
	msg[1].len = a_sizeRecvData;
	msg[1].buf = a_pRecvData;

	i4RetValue = i2c_transfer(g_pstI2CclientG->adapter,
				msg,
				EEPROM_I2C_MSG_SIZE_READ);

	if (i4RetValue != EEPROM_I2C_MSG_SIZE_READ) {
		pr_debug("I2C read failed!!\n");
		return -1;
	}
	return 0;
}

static int custom_read_region(u32 addr, u8 *data, u16 i2c_id, u32 size)
{
	u8 *buff = data;
	u32 size_to_read = size;

	int ret = 0;

	while (size_to_read > 0) {
		u8 page = addr / PAGE_SIZE_;
		u8 offset = addr % PAGE_SIZE_;
		char *Buff = data;

		if (iReadRegI2C(&offset, 1, (u8 *)Buff, 1,
			i2c_id + (page << 1)) < 0) {
			pr_debug("fail addr=0x%x 0x%x, P=%d, offset=0x%x",
				addr, *Buff, page, offset);
			break;
		}
		addr++;
		buff++;
		size_to_read--;
		ret++;
	}
	pr_debug("addr =%x size %d data read = %d\n", addr, size, ret);
	return ret;
}



unsigned int Custom_read_region(struct i2c_client *client, unsigned int addr,
				unsigned char *data, unsigned int size)
{
	g_pstI2CclientG = client;
	if (custom_read_region(addr, data, g_pstI2CclientG->addr, size) == 0)
		return size;
	else
		return 0;
}

/*Begin ersen.shang for config af otp T10017042 20210106*/
unsigned int gc8034_read_region(struct i2c_client *client, unsigned int addr,
                                unsigned char *data, unsigned int size)
{
    pr_err("addr =%x size %d\n", addr, size);
    if (addr == 0x0)
    {
        if (gc8034_otp_info.af_flag)
        {
            *(u32 *)data = 0x010b00ff;
        }
    }
    else if (addr == 0x1)
    {
        *(u32 *)data = 0x2;
    }
    else if (addr == 0x2)
    {
        *(u16 *)data = gc8034_otp_info.af_infinity;
    }
    else if (addr == 0x3)
    {
        *(u16 *)data = gc8034_otp_info.af_macro;
    }

    return size;
}

unsigned int s5k4h7_read_region(struct i2c_client *client, unsigned int addr,
                                unsigned char *data, unsigned int size)
{

    u16 af_inf, af_macro;
    af_info  *p_af;
    otp_data *p_otp = (otp_data *)&otp_buffer;

    pr_err("addr =%x size %d\n", addr, size);

    if ((p_otp->af_flag & 0x0c) == 0x04)
    {
        p_af = &p_otp->af_group1;
    }
    else if ((p_otp->af_flag & 0x03) == 0x01)
    {
        p_af = &p_otp->af_group2;
    }

    af_inf   = (p_af->inf_h << 8) | p_af->inf_l;
    af_macro = (p_af->macro_h << 8) | p_af->macro_l;

    if (addr == 0x0)
    {
        if (((s5k4h7_otp_status & EEPROM_AF_CHKSUM) == EEPROM_AF_CHKSUM) && ((s5k4h7_otp_status & EEPROM_AF_FLAG) == EEPROM_AF_FLAG))
        {
            *(u32 *)data = 0x010b00ff;
        }
    }
    else if (addr == 0x1)
    {
        *(u32 *)data = 0x2;
    }
    else if (addr == 0x2)
    {
        *(u16 *)data = af_inf;
    }
    else if (addr == 0x3)
    {
        *(u16 *)data = af_macro;
    }

    return size;
}
/*End   ersen.shang for config af otp T10017042 20210106*/