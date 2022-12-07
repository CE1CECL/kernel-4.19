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

/* Include platform define if necessary */
#ifdef EEPROM_PLATFORM_DEFINE
#include "eeprom_platform_def.h"
#endif

static DEFINE_SPINLOCK(g_spinLock);


static struct i2c_client *g_pstI2CclientG;

/* add for linux-4.4 */
#ifndef I2C_WR_FLAG
#define I2C_WR_FLAG		(0x1000)
#define I2C_MASK_FLAG	(0x00ff)
#endif

#define EEPROM_I2C_MSG_SIZE_READ 2
#ifndef EEPROM_I2C_READ_MSG_LENGTH_MAX
#define EEPROM_I2C_READ_MSG_LENGTH_MAX 32
#endif

static int Read_I2C_CAM_CAL(u16 a_u2Addr, u32 ui4_length, u8 *a_puBuff)
{
	int i4RetValue = 0;
	char puReadCmd[2] = { (char)(a_u2Addr >> 8), (char)(a_u2Addr & 0xFF) };
	struct i2c_msg msg[EEPROM_I2C_MSG_SIZE_READ];


	if (ui4_length > EEPROM_I2C_READ_MSG_LENGTH_MAX) {
		pr_debug("exceed one transition %d bytes limitation\n",
			 EEPROM_I2C_READ_MSG_LENGTH_MAX);
		return -1;
	}
	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr =
		g_pstI2CclientG->addr & (I2C_MASK_FLAG | I2C_WR_FLAG);
	spin_unlock(&g_spinLock);

	msg[0].addr = g_pstI2CclientG->addr;
	msg[0].flags = g_pstI2CclientG->flags & I2C_M_TEN;
	msg[0].len = 2;
	msg[0].buf = puReadCmd;

	msg[1].addr = g_pstI2CclientG->addr;
	msg[1].flags = g_pstI2CclientG->flags & I2C_M_TEN;
	msg[1].flags |= I2C_M_RD;
	msg[1].len = ui4_length;
	msg[1].buf = a_puBuff;

	i4RetValue = i2c_transfer(g_pstI2CclientG->adapter,
				msg,
				EEPROM_I2C_MSG_SIZE_READ);

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr = g_pstI2CclientG->addr & I2C_MASK_FLAG;
	spin_unlock(&g_spinLock);

	if (i4RetValue != EEPROM_I2C_MSG_SIZE_READ) {
		pr_debug("I2C read data failed!!\n");
		return -1;
	}

	return 0;
}

int iReadData_CAM_CAL(unsigned int ui4_offset,
	unsigned int ui4_length, unsigned char *pinputdata)
{
	int i4RetValue = 0;
	int i4ResidueDataLength;
	u32 u4IncOffset = 0;
	u32 u4CurrentOffset;
	u8 *pBuff;

	i4ResidueDataLength = (int)ui4_length;
	u4CurrentOffset = ui4_offset;
	pBuff = pinputdata;
	do {
		if (i4ResidueDataLength >= EEPROM_I2C_READ_MSG_LENGTH_MAX) {
			i4RetValue = Read_I2C_CAM_CAL(
				(u16) u4CurrentOffset,
				EEPROM_I2C_READ_MSG_LENGTH_MAX, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iReadData failed!!\n");
				return -1;
			}
			u4IncOffset += EEPROM_I2C_READ_MSG_LENGTH_MAX;
			i4ResidueDataLength -= EEPROM_I2C_READ_MSG_LENGTH_MAX;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
		} else {
			i4RetValue =
			    Read_I2C_CAM_CAL(
			    (u16) u4CurrentOffset, i4ResidueDataLength, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iReadData failed!!\n");
				return -1;
			}
			u4IncOffset += i4ResidueDataLength;
			i4ResidueDataLength = 0;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
			/* break; */
		}
	} while (i4ResidueDataLength > 0);



	return 0;
}

#if defined (CONFIG_TRAN_CAMERA_WESTALGO_DUALCAM)
//#define EEPROM_WRITE_PROTECT_DISABLE_WRITE_SLAVE_ADDR 0xA0
#define EEPROM_WRITE_PROTECT_DISABLE_WRITE_ADDR   0x8000
#define EEPROM_WRITE_PROTECT_DISABLE_WRITE_VALUE   0x06

//#define EEPROM_WRITE_PROTECT_ENABLE_WRITE_SLAVE_ADDR   0xA0
#define EEPROM_WRITE_PROTECT_ENABLE_WRITE_ADDR   0x8000
#define EEPROM_WRITE_PROTECT_ENABLE_WRITE_VALUE   0x0e

#define EEPROM_WRITE_PROTECT_DISABLE_WRITE_SLAVE_ADDR_S5K3L6 0x00
#define EEPROM_WRITE_PROTECT_DISABLE_WRITE_ADDR_S5K3L6   0x8000
#define EEPROM_WRITE_PROTECT_DISABLE_WRITE_VALUE_S5K3L6   0x06

#define EEPROM_WRITE_PROTECT_ENABLE_WRITE_SLAVE_ADDR_S5K3L6   0xFF
#define EEPROM_WRITE_PROTECT_ENABLE_WRITE_ADDR_S5K3L6   0x8000
#define EEPROM_WRITE_PROTECT_ENABLE_WRITE_VALUE_S5K3L6   0x0e

static int Write_I2C_CAM_CAL(u16 a_u2Addr, u32 a_u4Bytes, u8 *puDataInBytes)
{
#if 1
	u32 u4Index;
	int i4RetValue;
	char puSendCmd[8] = {
		(char)(a_u2Addr >> 8),
		(char)(a_u2Addr & 0xFF),
		0, 0, 0, 0, 0, 0
	};
	if (a_u4Bytes + 2 > 8) {
		pr_debug("exceed I2c-mt65xx.c 8 bytes limitation\n");
		return -1;
	}
	pr_debug(" write a_u2Addr:0x%x\n", a_u2Addr);

	for (u4Index = 0; u4Index < a_u4Bytes; u4Index += 1)
		puSendCmd[(u4Index + 2)] = puDataInBytes[u4Index];

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr =
		g_pstI2CclientG->addr & (I2C_MASK_FLAG | I2C_WR_FLAG);
	spin_unlock(&g_spinLock);
	pr_debug("write i2c addr:0x%x, I2C_MASK_FLAG:0x%x, I2C_WR_FLAG:0x%x\n", g_pstI2CclientG->addr, I2C_MASK_FLAG, I2C_WR_FLAG);
	i4RetValue = i2c_master_send(g_pstI2CclientG, puSendCmd, (a_u4Bytes + 2));
	if (i4RetValue != (a_u4Bytes + 2)) {
		pr_debug("I2C write  failed!!\n");
		return -1;
	}
	mdelay(5); /* for tWR singnal --> write data form buffer to memory. */

    pr_debug("[EEPROM] iWriteData done!!\n");

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr = g_pstI2CclientG->addr & I2C_MASK_FLAG;
	spin_unlock(&g_spinLock);
#endif
	return 0;
}
#if 0
static int Write_I2C_CAM_CAL_PROTECT(u16 a_slaveAddr, u16 a_u2Addr, u32 a_u4Bytes, u8 *puDataInBytes)
{
#if 1	
	u16 oldSlaveAddr;
	u32 u4Index;
	int i4RetValue;
	char puSendCmd[8] = {
		(char)(a_u2Addr >> 8),
		(char)(a_u2Addr & 0xFF),
		0, 0, 0, 0, 0, 0
	};
	if (a_u4Bytes + 2 > 8) {
		pr_debug("exceed I2c-mt65xx.c 8 bytes limitation\n");
		return -1;
	}
	pr_debug(" write a_u2Addr:0x%x\n", a_u2Addr);

	for (u4Index = 0; u4Index < a_u4Bytes; u4Index += 1)
		puSendCmd[(u4Index + 2)] = puDataInBytes[u4Index];

	
	spin_lock(&g_spinLock);
	oldSlaveAddr = g_pstI2CclientG->addr;
	g_pstI2CclientG->addr =
		a_slaveAddr & (I2C_MASK_FLAG | I2C_WR_FLAG);
	spin_unlock(&g_spinLock);
	pr_debug("write protect before i2c addr:0x%x, I2C_MASK_FLAG:0x%x, I2C_WR_FLAG:0x%x, a_slaveAddr:0x%x\n", g_pstI2CclientG->addr, I2C_MASK_FLAG, I2C_WR_FLAG,a_slaveAddr);
	i4RetValue = i2c_master_send(g_pstI2CclientG, puSendCmd, (a_u4Bytes + 2));
	if (i4RetValue != (a_u4Bytes + 2)) {
		pr_debug("I2C write  failed!!\n");
		//return -1;
	}
	mdelay(5); /* for tWR singnal --> write data form buffer to memory. */

    	pr_debug("[EEPROM] iWriteData done!!\n");

	spin_lock(&g_spinLock);
	g_pstI2CclientG->addr = oldSlaveAddr;
	g_pstI2CclientG->addr = g_pstI2CclientG->addr & I2C_MASK_FLAG;
	pr_debug("write protect after i2c addr:0x%x, I2C_MASK_FLAG:0x%x, I2C_WR_FLAG:0x%x, a_slaveAddr:0x%x\n", g_pstI2CclientG->addr, I2C_MASK_FLAG, I2C_WR_FLAG, a_slaveAddr);
	spin_unlock(&g_spinLock);
#endif
	return 0;
}
#endif
static bool disable_write_protect(void)
{
	u8 delay = 10;
	u8 disable_value = EEPROM_WRITE_PROTECT_DISABLE_WRITE_VALUE;
	if(Write_I2C_CAM_CAL(EEPROM_WRITE_PROTECT_DISABLE_WRITE_ADDR, 1, &disable_value) < 0)
	{
		pr_debug("disable eeprom write  protect error.\n");
		return false;
	}
	mdelay(delay);
	pr_debug("disable eeprom write  protect ok.\n");
	return true;
}
static bool enable_write_protect(void)
{
	u8 delay = 10;
	u8 enable_value = EEPROM_WRITE_PROTECT_ENABLE_WRITE_VALUE;
	if(Write_I2C_CAM_CAL(EEPROM_WRITE_PROTECT_ENABLE_WRITE_ADDR, 1, &enable_value) < 0)
	{
		pr_debug("enable eeprom write  protect error.\n");
		return false;
	}
	mdelay(delay);
	pr_debug("enable eeprom write  protect ok.\n");
	return true;
}

static bool disable_write_protect_s5k3l6(void)
{
	u8 delay = 10;
	u8 disable_value = EEPROM_WRITE_PROTECT_DISABLE_WRITE_VALUE_S5K3L6;
	if(Write_I2C_CAM_CAL(EEPROM_WRITE_PROTECT_DISABLE_WRITE_ADDR_S5K3L6, 1, &disable_value) < 0)
	{
		pr_debug("disable eeprom write  protect error.\n");
		return false;
	}
	mdelay(delay);
	pr_debug("disable eeprom write  protect ok.\n");
	return true;
}
static bool enable_write_protect_s5k3l6(void)
{
	u8 delay = 10;
	u8 enable_value = EEPROM_WRITE_PROTECT_ENABLE_WRITE_VALUE_S5K3L6;
	if(Write_I2C_CAM_CAL(EEPROM_WRITE_PROTECT_ENABLE_WRITE_ADDR_S5K3L6, 1, &enable_value) < 0)
	{
		pr_debug("enable eeprom write  protect error.\n");
		return false;
	}
	mdelay(delay);
	pr_debug("enable eeprom write  protect ok.\n");
	return true;
}

#define LENGTH_EEPROM_WRITE 1

//#undef DEBUG_LOG_WA
#define DEBUG_LOG_WA
#ifdef DEBUG_LOG_WA
static void logData(u16 len, u8* buf, const char * tagname) {
    u16 i = 0;
    for (; i < len; i++) {
        pr_err("%s data[%d] = 0x%x\n", tagname, i, buf[i]);
    }
}
#endif

int iWriteData_CAM_CAL(unsigned int ui4_offset,
	unsigned int ui4_length, unsigned char *pinputdata)
{

#if 1
	int i4RetValue = 0;
	int i4ResidueDataLength;
	u32 u4IncOffset = 0;
	u32 u4CurrentOffset;
	u8 *pBuff;
#ifdef DEBUG_LOG_WA
    u8 pTmpBuf[LENGTH_EEPROM_WRITE];
    u8 retry = 0, delay = 0;
#endif

    pr_err("[CAM_CAL] iWriteData\n");
    pr_err("ui4_offset:0x%x, ui4_length:%d", ui4_offset, ui4_length);
    logData(ui4_length, pinputdata, "write");
    disable_write_protect();
    if (ui4_offset + ui4_length >= 0x2000) {
        pr_debug("[CAM_CAL] Write Error!! S-24CS64A not supprt address >= 0x2000!!\n");
        return -1;
    }

	i4ResidueDataLength = (int)ui4_length;
	u4CurrentOffset = ui4_offset;
	pBuff = pinputdata;
	do {
        if (i4ResidueDataLength >= LENGTH_EEPROM_WRITE) {
            i4RetValue = Write_I2C_CAM_CAL((u16)u4CurrentOffset, LENGTH_EEPROM_WRITE, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iWriteData failed!!\n");
				return -1;
			}
#ifdef DEBUG_LOG_WA

	do {
            logData(LENGTH_EEPROM_WRITE, pBuff, "write");
            i4RetValue = Read_I2C_CAM_CAL((u16)u4CurrentOffset, LENGTH_EEPROM_WRITE, pTmpBuf);
            if (i4RetValue != 0) {
                pr_debug("[CAM_CAL] I2C iReadData failed!!\n");
                return -1;
            }
            logData(LENGTH_EEPROM_WRITE, pTmpBuf, "read");
                if (0 == strncmp(pBuff, pTmpBuf, LENGTH_EEPROM_WRITE)) {
//                    printk("write addr 0x%0x data 0x%0x",offset,(u32)*(data + i));
                    break;
                } else {
                    printk("try to write offset(%x) retry: %d\n", u4CurrentOffset, retry);
                    i4RetValue = Write_I2C_CAM_CAL((u16)u4CurrentOffset, LENGTH_EEPROM_WRITE, pBuff);
                    delay += 5;
                    mdelay(delay);
                }
	} while(retry != 0);
#endif

            u4IncOffset += LENGTH_EEPROM_WRITE;
            i4ResidueDataLength -= LENGTH_EEPROM_WRITE;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
		} else {
			i4RetValue =
			    Write_I2C_CAM_CAL(
			    (u16) u4CurrentOffset, i4ResidueDataLength, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iReadData failed!!\n");
				return -1;
			}
            u4IncOffset += LENGTH_EEPROM_WRITE;
            i4ResidueDataLength -= LENGTH_EEPROM_WRITE;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
			/* break; */
		}
	} while (i4ResidueDataLength > 0);
	enable_write_protect();
#endif
    pr_err("[CAM_CAL] iWriteData done\n");
	return 0;
}



int iWriteData_CAM_CAL_S5K3L6(unsigned int ui4_offset,
	unsigned int ui4_length, unsigned char *pinputdata)
{

#if 1
	int i4RetValue = 0;
	int i4ResidueDataLength;
	u32 u4IncOffset = 0;
	u32 u4CurrentOffset;
	u8 *pBuff;
#ifdef DEBUG_LOG_WA
    u8 pTmpBuf[LENGTH_EEPROM_WRITE];
    u8 retry = 0, delay = 0;
#endif

    pr_err("[CAM_CAL] iWriteData\n");
    pr_err("ui4_offset:0x%x, ui4_length:%d", ui4_offset, ui4_length);
    logData(ui4_length, pinputdata, "write");
    disable_write_protect_s5k3l6();
    if (ui4_offset + ui4_length >= 0x2000) {
        pr_debug("[CAM_CAL] Write Error!! S-24CS64A not supprt address >= 0x2000!!\n");
        return -1;
    }

	i4ResidueDataLength = (int)ui4_length;
	u4CurrentOffset = ui4_offset;
	pBuff = pinputdata;
	do {
        if (i4ResidueDataLength >= LENGTH_EEPROM_WRITE) {
            i4RetValue = Write_I2C_CAM_CAL((u16)u4CurrentOffset, LENGTH_EEPROM_WRITE, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iWriteData failed!!\n");
				return -1;
			}
#ifdef DEBUG_LOG_WA

	do {
            logData(LENGTH_EEPROM_WRITE, pBuff, "write");
            i4RetValue = Read_I2C_CAM_CAL((u16)u4CurrentOffset, LENGTH_EEPROM_WRITE, pTmpBuf);
            if (i4RetValue != 0) {
                pr_debug("[CAM_CAL] I2C iReadData failed!!\n");
                return -1;
            }
            logData(LENGTH_EEPROM_WRITE, pTmpBuf, "read");
                if (0 == strncmp(pBuff, pTmpBuf, LENGTH_EEPROM_WRITE)) {
//                    printk("write addr 0x%0x data 0x%0x",offset,(u32)*(data + i));
                    break;
                } else {
                    printk("try to write offset(%x) retry: %d\n", u4CurrentOffset, retry);
                    i4RetValue = Write_I2C_CAM_CAL((u16)u4CurrentOffset, LENGTH_EEPROM_WRITE, pBuff);
                    delay += 5;
                    mdelay(delay);
                }
	} while(retry != 0);
#endif

            u4IncOffset += LENGTH_EEPROM_WRITE;
            i4ResidueDataLength -= LENGTH_EEPROM_WRITE;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
		} else {
			i4RetValue =
			    Write_I2C_CAM_CAL(
			    (u16) u4CurrentOffset, i4ResidueDataLength, pBuff);
			if (i4RetValue != 0) {
				pr_debug("I2C iReadData failed!!\n");
				return -1;
			}
            u4IncOffset += LENGTH_EEPROM_WRITE;
            i4ResidueDataLength -= LENGTH_EEPROM_WRITE;
			u4CurrentOffset = ui4_offset + u4IncOffset;
			pBuff = pinputdata + u4IncOffset;
			/* break; */
		}
	} while (i4ResidueDataLength > 0);
	enable_write_protect_s5k3l6();
#endif
    pr_err("[CAM_CAL] iWriteData done\n");
	return 0;
}

#endif


unsigned int Common_read_region(struct i2c_client *client, unsigned int addr,
				unsigned char *data, unsigned int size)
{
	g_pstI2CclientG = client;
	if (iReadData_CAM_CAL(addr, size, data) == 0)
		return size;
	else
		return 0;
}

