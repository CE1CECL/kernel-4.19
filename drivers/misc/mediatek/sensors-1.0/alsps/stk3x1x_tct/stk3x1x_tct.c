/*
 *  stk3x1x.c - Linux kernel modules for sensortek stk301x, stk321x, stk331x
 *  , and stk3410 proximity/ambient light sensor
 *
 *  Copyright (C) 2012~2013 Lex Hsieh / sensortek <lex_hsieh@sensortek.com.tw>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#ifdef CONFIG_PM_WAKELOCKS
#include <linux/pm_wakeup.h>
#else
#include <linux/wakelock.h>
#endif
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <cust_alsps.h>
#include <alsps.h>
#include <linux/dma-mapping.h>
#ifdef CONFIG_OF
#include <linux/of_gpio.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#endif

static struct alsps_init_info stk3x1x_init_info;

//modify(add) by junfeng.zhou.sz for add power supply begin . 20140214
//#define POWER_REGULATOR
#ifdef POWER_REGULATOR
#include <linux/regulator/consumer.h>
/* POWER SUPPLY VOLTAGE RANGE */
#define STK3X1X_VDD_MIN_UV	2000000
#define STK3X1X_VDD_MAX_UV	3300000
#define STK3X1X_VIO_MIN_UV	1750000
#define STK3X1X_VIO_MAX_UV	1950000
#endif
//modify(add) by junfeng.zhou.sz for add power supply end .

//modify(add) by junfeng.zhou.sz for add print time begin . 20140214
#define PRINT_TIME
#ifdef PRINT_TIME
#include <linux/rtc.h>
void print_local_time(char *param)
{
    	struct timespec ts;
	struct rtc_time tm;

	getnstimeofday(&ts);
	rtc_time_to_tm(ts.tv_sec+28800, &tm);
	if(param == NULL)
	    return ;
	printk(KERN_INFO "%s:%s %d-%02d-%02d %02d:%02d:%02d\n",__func__,param,
			 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
}
#endif
//modify(add) by junfeng.zhou.sz for add print time end .


#define DRIVER_VERSION  "3.6.0 20140324"

int stk_is_probe_success = -1;  //flag for probe success
#define STK_CHK_REG //check the register

/* Driver Settings */
#define CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD
#ifdef CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD

#define STK_ALS_CHANGE_THD	5	/* The threshold to trigger ALS interrupt, unit: lux */
#define STK_ALS_DARKCODE_THD	5
#endif	/* #ifdef CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD */
#define STK_INT_PS_MODE			1	/* 1, 2, or 3	*/
#define STK_TUNE0

#define CALI_EVERY_TIME
//#define STK_DEBUG_PRINTF

#define STK_ALS_FIR
//#define STK_IRS

/*Begin modified by zhikui.li@tcl.com for, Close the anti-oil algorithm*/
#define CTTRACKING
/*end modified by zhikui.li@tcl.com for, Close the anti-oil algorithm*/
#define ALS_CORRECT_FACTOR    800

#define STK3311SA_PID		0x1E
#define STK33562_PID        0x51
#define STK33562_IC_USE        1

#include "stk3x1x_tct.h"

/* Define Register Map */
#define STK_STATE_REG 			0x00
#define STK_PSCTRL_REG 			0x01
#define STK_ALSCTRL_REG 			0x02
#define STK_LEDCTRL_REG 			0x03
#define STK_INT_REG 				0x04
#define STK_WAIT_REG 			0x05
#define STK_THDH1_PS_REG 		0x06
#define STK_THDH2_PS_REG 		0x07
#define STK_THDL1_PS_REG 		0x08
#define STK_THDL2_PS_REG 		0x09
#define STK_THDH1_ALS_REG 		0x0A
#define STK_THDH2_ALS_REG 		0x0B
#define STK_THDL1_ALS_REG 		0x0C
#define STK_THDL2_ALS_REG 		0x0D
#define STK_FLAG_REG 			0x10
#define STK_DATA1_PS_REG	 	0x11
#define STK_DATA2_PS_REG 		0x12
#define STK_DATA1_ALS_REG 		0x13
#define STK_DATA2_ALS_REG 		0x14
#define STK_DATA1_OFFSET_REG 	0x15
#define STK_DATA2_OFFSET_REG 	0x16
#define STK_DATA1_IR_REG 		0x17
#define STK_DATA2_IR_REG 		0x18
#define STK_PDT_ID_REG 			0x3E
#define STK_RSRVD_REG 			0x3F
#define STK_SW_RESET_REG		0x80

#define STK_GSCTRL_REG			0x1A
#define STK_FLAG2_REG			0x1C

/* Define state reg */
#define STK_STATE_EN_IRS_SHIFT  	7
#define STK_STATE_EN_AK_SHIFT  	6
#define STK_STATE_EN_ASO_SHIFT  	5
#define STK_STATE_EN_IRO_SHIFT  	4
#define STK_STATE_EN_WAIT_SHIFT  	2
#define STK_STATE_EN_ALS_SHIFT  	1
#define STK_STATE_EN_PS_SHIFT  	0

#define STK_STATE_EN_IRS_MASK	0x80
#define STK_STATE_EN_AK_MASK	0x40
#define STK_STATE_EN_ASO_MASK	0x20
#define STK_STATE_EN_IRO_MASK	0x10
#define STK_STATE_EN_WAIT_MASK	0x04
#define STK_STATE_EN_ALS_MASK	0x02
#define STK_STATE_EN_PS_MASK	0x01

/* Define PS ctrl reg */
#define STK_PS_PRS_SHIFT  		6
#define STK_PS_GAIN_SHIFT  		4
#define STK_PS_IT_SHIFT  			0

#define STK_PS_PRS_MASK			0xC0
#define STK_PS_GAIN_MASK			0x30
#define STK_PS_IT_MASK			0x0F

/* Define ALS ctrl reg */
#define STK_ALS_PRS_SHIFT  		6
#define STK_ALS_GAIN_SHIFT  		4
#define STK_ALS_IT_SHIFT  			0

#define STK_ALS_PRS_MASK		0xC0
#define STK_ALS_GAIN_MASK		0x30
#define STK_ALS_IT_MASK			0x0F

/* Define LED ctrl reg */
#define STK_LED_IRDR_SHIFT  		6
#define STK_LED_DT_SHIFT  		0

#define STK_LED_IRDR_MASK		0xC0
#define STK_LED_DT_MASK			0x3F

/* Define interrupt reg */
#define STK_INT_CTRL_SHIFT  		7
#define STK_INT_OUI_SHIFT  		4
#define STK_INT_ALS_SHIFT  		3
#define STK_INT_PS_SHIFT  			0

#define STK_INT_CTRL_MASK		0x80
#define STK_INT_OUI_MASK			0x10
#define STK_INT_ALS_MASK			0x08
#define STK_INT_PS_MASK			0x07

#define STK_INT_ALS				0x08

/* Define flag reg */
#define STK_FLG_ALSDR_SHIFT  		7
#define STK_FLG_PSDR_SHIFT  		6
#define STK_FLG_ALSINT_SHIFT  		5
#define STK_FLG_PSINT_SHIFT  		4
#define STK_FLG_OUI_SHIFT  		2
#define STK_FLG_IR_RDY_SHIFT  		1
#define STK_FLG_NF_SHIFT  		0

#define STK_FLG_ALSDR_MASK		0x80
#define STK_FLG_PSDR_MASK		0x40
#define STK_FLG_ALSINT_MASK		0x20
#define STK_FLG_PSINT_MASK		0x10
#define STK_FLG_OUI_MASK			0x04
#define STK_FLG_IR_RDY_MASK		0x02
#define STK_FLG_NF_MASK			0x01

/* Define flag2 reg */
#define STK_FLG2_INT_GS_SHIFT		6
#define STK_FLG2_GS10_SHIFT		5
#define STK_FLG2_GS01_SHIFT		4

#define STK_FLG2_INT_GS_MASK	0x40
#define STK_FLG2_GS10_MASK		0x20
#define STK_FLG2_GS01_MASK		0x10


/* misc define */
#define MIN_ALS_POLL_DELAY_NS	60000000


#ifdef STK_TUNE0

#define STK_MAX_MIN_DIFF	100
#define STK_LT_N_CT	60
#define STK_HT_N_CT	90
#define STK_HI_PS_LT_N_CT	120
#define STK_HI_PS_HT_N_CT	240

/*Begin:Modify by TCTSZ zhikui.li@tcl.com, Update threshold*/
#define STK_LOW_PS_LT_N_CT	50
#define STK_LOW_PS_HT_N_CT	100

#ifdef CTTRACKING
#define STK_H_PS		1250
#define STK_H_HT		190
#define STK_H_LT		140

#define STK3X3X_PRX_THD_BOOT_SHIFT_PIO      300//100//400
#define STK3X3X_PRX_THD_BOOT_SHIFT_1_PIO    200
#endif //end CTTRACKING
/*END:Modify by TCTSZ zhikui.li@tcl.com, Update threshold*/
#endif //STK_TUNE0
#define STK3X3X_PS_BGIR_THRESHOLD       0x64

#define STK_IRC_MAX_ALS_CODE		20000
#define STK_IRC_MIN_ALS_CODE		25
#define STK_IRC_MIN_IR_CODE		50
#define STK_IRC_ALS_DENOMI		2
#define STK_IRC_ALS_NUMERA		5
#define STK_IRC_ALS_CORREC		748

#define DEVICE_NAME		"stk3x1x"
#define ALS_NAME "stk3x1x-als"
#define PS_NAME "stk3x1x-ps"

#define TRACE_TAG 0x4646
#define DEF_TH_TAG 0x7878
#define DEF_TH_OFFSET_TAG 0x5454
#define CALI_DATA_TAG 0x9595
#define DEF_TH_LIMIT_TAG 0x3131
#define DEF_MMI_DATA 0x1234
#ifndef I2C_MASK_FLAG
#define I2C_MASK_FLAG   (0x00ff)
#define I2C_DMA_FLAG    (0x2000)
#define I2C_ENEXT_FLAG  (0x0200)
#endif

struct factory_thd_offset {
	u16 tag;
	u16 low_ps_cali_th_offset_h;
	u16 low_ps_cali_th_offset_l;
	u16 hi_ps_cali_th_offset_h;
	u16 hi_ps_cali_th_offset_l;
	u16 reserve;
}__attribute__ ((__packed__));

struct factory_cali_cfg {
	u16 tag;
	u8 psctrl;
	u8 ledctrl;
}__attribute__ ((__packed__));

struct factory_thd_limit {
	u16 tag;
	u16 mmi_thd_h_limit;
	u16 mmi_thd_l_limit;
}__attribute__ ((__packed__));

struct factory_mmi_data {
	u16 tag;
	u16 mmi_data_offset;
	u16 mmi_data_rawoff;
	u16 mmi_data_rawdata;
}__attribute__ ((__packed__));


//************************************************
//  Note: this strust must not be over 40 bytes
//************************************************
struct trace_data {
	u16 tag;
	struct factory_thd_offset factory_def_thd_offset;
	struct factory_cali_cfg   factory_cali_data;
	struct factory_thd_limit  factory_def_thd_limit;
	struct factory_mmi_data   factory_def_mmi_data;
}__attribute__ ((__packed__));

int dark_code_flag = 0;
#ifdef STK_ALS_FIR
#define STK_FIR_LEN	16
#define MAX_FIR_LEN 64

struct data_filter {
    	u16 raw[MAX_FIR_LEN];
    	int sum;
    	int number;
    	int idx;
};
#endif

struct alsps_hw alsps_cust;
static struct alsps_hw *hw = &alsps_cust;

//modify(add) by junfeng.zhou.sz for add i2c lock with 656597 begin . 20140421
struct mutex stk_i2c_lock;
//modify(add) by junfeng.zhou.sz for add i2c lock with 656597 end .
struct stk3x1x_data {
	struct i2c_client *client;
	struct stk3x1x_platform_data *pdata;

	int32_t irq;
    	struct work_struct stk_work;
	struct workqueue_struct *stk_wq;

	uint16_t ir_code;
	uint16_t als_correct_factor;
	uint8_t alsctrl_reg;
	uint8_t psctrl_reg;
	uint8_t ledctrl_reg;
	uint8_t state_reg;
	int		int_pin;
	uint8_t wait_reg;
	uint8_t int_reg;

	uint16_t ps_thd_h;
	uint16_t ps_thd_l;
	struct mutex io_lock;
    	struct mutex io_als_lock;
	int32_t ps_distance_last;
	bool ps_enabled;
	bool re_enable_ps;
#ifdef CONFIG_PM_WAKELOCKS
	//struct wakeup_source ps_wakelock;
	struct wakeup_source *ps_wakelock;
#else
	struct wake_lock ps_wakelock;
#endif
	int32_t als_lux_last;
	uint32_t als_transmittance;
	bool als_enabled;
	bool re_enable_als;
	ktime_t ps_poll_delay;
	ktime_t als_poll_delay;

	struct work_struct stk_als_work;
	struct hrtimer als_timer;
	struct workqueue_struct *stk_als_wq;

	bool first_boot;
#ifdef STK_TUNE0
	uint16_t psa;
	uint16_t psi;
	uint16_t psi_set;
	uint16_t ps_high_thd_boot;
	uint16_t ps_low_thd_boot;
	uint16_t ps_high_thd_def;
	uint16_t ps_low_thd_def;
#ifdef CTTRACKING
    bool  ps_thd_update;
    bool  ps_ori_thd;
	struct mutex state_lock;
#endif
/*Begin:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold, outdoor sunlight uses dynamic calibration*/
	int32_t near_far_state_last;
	uint16_t  old_ps_thd_h;
	uint16_t  old_ps_thd_l;
	uint16_t  cali_ps_thd_h;
	uint16_t  cali_ps_thd_l;
/*END by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold, outdoor sunlight uses dynamic calibration*/
	struct hrtimer ps_tune0_timer;
	struct workqueue_struct *stk_ps_tune0_wq;
    struct work_struct stk_ps_tune0_work;
	ktime_t ps_tune0_delay;
	bool tune_zero_init_proc;
	uint32_t ps_stat_data[3];
	int data_count;
	int psi_flag;
#endif
#ifdef STK_ALS_FIR
	struct data_filter      fir;
	atomic_t                firlength;
#endif
	atomic_t	recv_reg;


	bool use_fir;
#ifdef POWER_REGULATOR
	struct regulator *vdd;
	struct regulator *vio;
	bool power_enabled;
#endif


   int in_suspend;
   int als_enable_pre;
   int ps_enable_pre;
	bool is_have_good_cali;
	uint16_t  last_good_psi;
	uint16_t  min_psi;
	bool is_cali;

	struct trace_data factory_data;

// [Feature]Add-BEGIN by TCTSZ. ning.wei@tcl.com, 2015/08/03, for add stk p/l sensor pull up&down ALM389755
/*	struct pinctrl *pinctrl;
	struct pinctrl_state *pin_default;
	struct pinctrl_state *pin_sleep;*/
// [Feature]-Add-END by TCTSZ. ning.wei@tcl.com, 2015/08/03, for add stk p/l sensor pull up&down ALM389755

	struct mutex enable_mutex;

	struct als_control_path als_ctl;
	struct als_data_path als_data;
	struct ps_control_path ps_ctl;
	struct ps_data_path ps_data;
        bool sunshine_state;
	int	ps_thd_reading;
};

struct stk3x1x_data *stk3x1x_obj = NULL;

static uint16_t STK3X3X_LT_N_CT;
static uint16_t STK3X3X_HT_N_CT;
static uint16_t STK3X3X_PRX_THD_BOOT_SHIFT;
static uint16_t STK3X3X_PRX_THD_BOOT_SHIFT_1;

//#define ALS_REPORT_LUX_TABLE
#ifdef ALS_REPORT_LUX_TABLE
static uint32_t lux_report_table[] =
{
	30,         //30b
	60,         //40b
	90,         //50b
	110,        //60b
	130,        //70b
	150,        //80b
	180,        //90b
	230,        //110b
	1000,       //120b
	2000,       //250b
};
static uint32_t lux_adc_table[] =
{
	0,
	8,
	80,
	100,
	120,
	140,
	160,
	200,
	250,
	3500,
};
#endif
#if( !defined(CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD))
static uint32_t lux_threshold_table[] =
{
	3,
	10,
	40,
	65,
	145,
	300,
	550,
	930,
	1250,
	1700,
};

#define LUX_THD_TABLE_SIZE (sizeof(lux_threshold_table)/sizeof(uint32_t)+1)
static uint16_t code_threshold_table[LUX_THD_TABLE_SIZE+1];
#endif

static int32_t stk3x1x_enable_ps(struct stk3x1x_data *obj_data, uint8_t enable, uint8_t validate_reg);
static int32_t stk3x1x_enable_als(struct stk3x1x_data *obj_data, uint8_t enable);
static int32_t stk3x1x_set_ps_thd_l(struct stk3x1x_data *obj_data, uint16_t thd_l);
static int32_t stk3x1x_set_ps_thd_h(struct stk3x1x_data *obj_data, uint16_t thd_h);
static int32_t stk3x1x_set_als_thd_l(struct stk3x1x_data *obj_data, uint16_t thd_l);
static int32_t stk3x1x_set_als_thd_h(struct stk3x1x_data *obj_data, uint16_t thd_h);
static int stk3x3x_read_threshold(struct stk3x1x_data *obj_data);
#ifdef POWER_REGULATOR
static int stk3x1x_device_ctl(struct stk3x1x_data *obj_data, bool enable);
#endif
static int32_t stk3x1x_get_ir_reading(struct stk3x1x_data *obj_data);
#ifdef STK_TUNE0
static int stk_ps_tune_zero_func_fae(struct stk3x1x_data *obj_data);
#endif
#ifdef STK_CHK_REG
static int stk3x1x_validate_n_handle(struct i2c_client *client);
#endif
#ifndef STK33562_IC_USE
static int stk_ps_val(struct stk3x1x_data *obj_data);
#endif
static int stk3x1x_i2c_read_data(struct i2c_client *client, unsigned char command, int length, unsigned char *values)
{
	uint8_t retry;
	int err;
	struct i2c_msg msgs[] =
	{
		{
			.addr = client->addr,
			.flags = 0,
			//.ext_flag = 0,
			.len = 1,
			.buf = &command,
		},
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			//.ext_flag = 0,
			.len = length,
			.buf = values,
		}
	};

	mutex_lock(&stk_i2c_lock);
	for (retry = 0; retry < 5; retry++)
	{
		err = i2c_transfer(client->adapter, msgs, 2);
		if (err == 2)
			break;
		else
			mdelay(5);
	}
	mutex_unlock(&stk_i2c_lock);
	if (retry >= 5)
	{
		printk(KERN_ERR "%s: i2c read fail, err=%d\n", __func__, err);
		return -EIO;
	}
	return 0;
}

static int stk3x1x_i2c_write_data(struct i2c_client *client, unsigned char command, int length, unsigned char *values)
{
	int retry;
	int err;
	unsigned char data[11];
	struct i2c_msg msg = {0};
	int index ;

    	mutex_lock(&stk_i2c_lock);
    	if (!client)
	{
	    	mutex_unlock(&stk_i2c_lock);
		return -EINVAL;
	}
    	else if (length >= 10)
	{
        	printk(KERN_ERR "%s:length %d exceeds 10\n", __func__, length);
        	mutex_unlock(&stk_i2c_lock);
        	return -EINVAL;
    	}

	data[0] = command;
	for (index=1;index<=length;index++)
		data[index] = values[index-1];

	msg.addr = client->addr;
	msg.flags = 0;
	//msg.ext_flag = 0;
	msg.len = length+1;
	msg.buf = data;

	for (retry = 0; retry < 5; retry++)
	{

		err = i2c_transfer(client->adapter, &msg, 1);
		if (err == 1)
			break;
		else
			mdelay(5);
	}
	mutex_unlock(&stk_i2c_lock);
	if (retry >= 5)
	{
		printk(KERN_ERR "%s: i2c write fail, err=%d\n", __func__, err);
		return -EIO;
	}
	return 0;
}

static int stk3x1x_i2c_smbus_read_byte_data(struct i2c_client *client, unsigned char command)
{
	unsigned char value;
	int err;
	err = stk3x1x_i2c_read_data(client, command, 1, &value);
	if(err < 0)
		return err;
	return value;
}

static int stk3x1x_i2c_smbus_write_byte_data(struct i2c_client *client, unsigned char command, unsigned char value)
{
	int err;
	err = stk3x1x_i2c_write_data(client, command, 1, &value);
	return err;
}

uint32_t stk_alscode2lux(struct stk3x1x_data *obj_data, uint32_t alscode)
{
#if 0
    alscode += ((alscode<<7)+(alscode<<3)+(alscode>>1));
    alscode<<=3;
#endif
    //alscode/=obj_data->als_transmittance;
    alscode = alscode * obj_data->als_transmittance /1000;
	return alscode;
}

uint32_t stk_lux2alscode(struct stk3x1x_data *obj_data, uint32_t lux)
{
    	lux*=obj_data->als_transmittance;
    	lux/=1100;
    	if (unlikely(lux>=(1<<16)))
        	lux = (1<<16) -1;
    	return lux;
}

#ifndef CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD
static void stk_init_code_threshold_table(struct stk3x1x_data *obj_data)
{
    	uint32_t i,j;
    	uint32_t alscode;

    	code_threshold_table[0] = 0;
#ifdef STK_DEBUG_PRINTF
    	printk(KERN_INFO "alscode[0]=%d\n",0);
#endif
    	for (i=1,j=0;i<LUX_THD_TABLE_SIZE;i++,j++)
    	{
        	alscode = stk_lux2alscode(obj_data, lux_threshold_table[j]);
        	printk(KERN_INFO "alscode[%d]=%d\n",i,alscode);
        	code_threshold_table[i] = (uint16_t)(alscode);
    	}
    	code_threshold_table[i] = 0xffff;
    	printk(KERN_INFO "alscode[%d]=%d\n",i,alscode);
}

static uint32_t stk_get_lux_interval_index(uint16_t alscode)
{
    	uint32_t i;
    	for (i=1;i<=LUX_THD_TABLE_SIZE;i++)
    	{
        	if ((alscode>=code_threshold_table[i-1])&&(alscode<code_threshold_table[i]))
        	{
            		return i;
        	}
    	}
    	return LUX_THD_TABLE_SIZE;
}
#else
void stk_als_set_new_thd(struct stk3x1x_data *obj_data, uint16_t alscode)
{
    	int32_t high_thd,low_thd;
    	high_thd = alscode + stk_lux2alscode(obj_data, STK_ALS_CHANGE_THD);
    	low_thd = alscode - stk_lux2alscode(obj_data, STK_ALS_CHANGE_THD);
    	if (high_thd >= (1<<16))
        	high_thd = (1<<16) -1;
    	if (low_thd <0)
        	low_thd = 0;
    	stk3x1x_set_als_thd_h(obj_data, (uint16_t)high_thd);
    	stk3x1x_set_als_thd_l(obj_data, (uint16_t)low_thd);
}
#endif // CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD


static void stk3x1x_proc_plat_data(struct stk3x1x_data *obj_data, struct stk3x1x_platform_data *plat_data)
{
	uint8_t w_reg;

	obj_data->state_reg = plat_data->state_reg;
	obj_data->psctrl_reg = plat_data->psctrl_reg;

	obj_data->alsctrl_reg = plat_data->alsctrl_reg;
	obj_data->ledctrl_reg = plat_data->ledctrl_reg;

	obj_data->wait_reg = plat_data->wait_reg;
	if(obj_data->wait_reg < 2)
	{
		printk(KERN_WARNING "%s: wait_reg should be larger than 2, force to write 2\n", __func__);
		obj_data->wait_reg = 2;
	}
	else if (obj_data->wait_reg > 0xFF)
	{
		printk(KERN_WARNING "%s: wait_reg should be less than 0xFF, force to write 0xFF\n", __func__);
		obj_data->wait_reg = 0xFF;
	}
#ifndef STK_TUNE0
	obj_data->ps_thd_h = plat_data->ps_thd_h;
	obj_data->ps_thd_l = plat_data->ps_thd_l;
#endif

	w_reg = 0;
	w_reg |= 0x01;

	obj_data->int_reg = w_reg;
	return;
}

static int32_t stk3x1x_init_all_reg(struct stk3x1x_data *obj_data)
{
	int32_t ret;

    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_STATE_REG, obj_data->state_reg);
    	if (ret < 0)
    	{
        	printk(KERN_ERR "%s: write i2c error\n", __func__);
        	return ret;
    	}
    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_PSCTRL_REG, obj_data->psctrl_reg);
    	if (ret < 0)
    	{
        	printk(KERN_ERR "%s: write i2c error\n", __func__);
        	return ret;
    	}
    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_ALSCTRL_REG, obj_data->alsctrl_reg);
    	if (ret < 0)
    	{
        	printk(KERN_ERR "%s: write i2c error\n", __func__);
        	return ret;
    	}
    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_LEDCTRL_REG, obj_data->ledctrl_reg);
    	if (ret < 0)
    	{
        	printk(KERN_ERR "%s: write i2c error\n", __func__);
        	return ret;
    	}
    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_WAIT_REG, obj_data->wait_reg);
    	if (ret < 0)
    	{
        	printk(KERN_ERR "%s: write i2c error\n", __func__);
        	return ret;
    	}
#ifdef STK_TUNE0
	obj_data->psa = 0x0;
	obj_data->psi = 0xFFFF;
#else
	stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
	stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
#endif

    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_INT_REG, obj_data->int_reg);
    	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}
#ifdef STK33562_IC_USE
/*    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0x4E, 0x02);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write  0x4E i2c error\n", __func__);
		return ret;
	}*/
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0xA0, 0x10);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0xA0  i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0xDB, 0x01);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0xDB i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0x4D, 0x00);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0x4D i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0xF6, 0xB2);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0xF6 i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0xA0, 0x10);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0xA0 i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0xA1, 0x7F);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0xA1 i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0xAA, 0x64);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0xAA i2c error\n", __func__);
		return ret;
	}
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0x4F, 0x43);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write 0x4F i2c error\n", __func__);
		return ret;
	}
#endif

	/*
    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, 0x87, 0x60);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}
	*/

	return 0;
}


static int32_t stk3x1x_check_pid(struct stk3x1x_data *obj_data)
{
	unsigned char value[2], pid_msb;
	int err;

	err = stk3x1x_i2c_read_data(obj_data->client, STK_PDT_ID_REG, 2, &value[0]);
	if(err < 0)
	{
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, err);
		return err;
	}

	if(value[0] == STK3311SA_PID)
		obj_data->ledctrl_reg &= 0x3F;
#ifdef STK_DEBUG_PRINTF
	printk("%s: PID=0x%x, RID=0x%x obj_data->ledctrl_reg=%d\n", __func__, value[0], value[1],obj_data->ledctrl_reg);
#endif
	if(value[1] == 0xC0)
		printk(KERN_INFO "%s: RID=0xC0!!!!!!!!!!!!!\n", __func__);

	if(value[0] == 0)
	{
		printk(KERN_ERR "PID=0x0, please make sure the chip is stk3x1x!\n");
		return -2;
	}

	pid_msb = value[0] & 0xF0;
	switch(pid_msb)
	{
		case 0x10:
		case 0x20:
		case 0x30:
		case 0x50:
			return 0;
		default:
			printk(KERN_ERR "%s: invalid PID(%#x)\n", __func__, value[0]);
			return -1;
	}
	return 0;
}


static int32_t stk3x1x_software_reset(struct stk3x1x_data *obj_data)
{
    	int32_t r;
    	uint8_t w_reg;

    	w_reg = 0x7F;
    	r = stk3x1x_i2c_smbus_write_byte_data(obj_data->client,STK_WAIT_REG,w_reg);
    	if (r<0)
    	{
        	printk(KERN_ERR "%s: software reset: write i2c error, ret=%d\n", __func__, r);
        	return r;
    	}

    	r = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_WAIT_REG);
    	if (w_reg != r)
    	{
        	printk(KERN_ERR "%s: software reset: read-back value is not the same\n", __func__);
        	return -1;
    	}

    	r = stk3x1x_i2c_smbus_write_byte_data(obj_data->client,STK_SW_RESET_REG,0);
    	if (r<0)
    	{
        	printk(KERN_ERR "%s: software reset: read error after reset\n", __func__);
        	return r;
    	}

	usleep_range(13000, 15000);
    	return 0;
}


static int32_t stk3x1x_set_als_thd_l(struct stk3x1x_data *obj_data, uint16_t thd_l)
{
	unsigned char val[2];
	int ret;
	val[0] = (thd_l & 0xFF00) >> 8;
	val[1] = thd_l & 0x00FF;
	ret = stk3x1x_i2c_write_data(obj_data->client, STK_THDL1_ALS_REG, 2, val);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	return ret;
}
static int32_t stk3x1x_set_als_thd_h(struct stk3x1x_data *obj_data, uint16_t thd_h)
{
	unsigned char val[2];
	int ret;
	val[0] = (thd_h & 0xFF00) >> 8;
	val[1] = thd_h & 0x00FF;
	ret = stk3x1x_i2c_write_data(obj_data->client, STK_THDH1_ALS_REG, 2, val);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	return ret;
}

static int32_t stk3x1x_set_ps_thd_l(struct stk3x1x_data *obj_data, uint16_t thd_l)
{
	unsigned char val[2];
	int ret;
	val[0] = (thd_l & 0xFF00) >> 8;
	val[1] = thd_l & 0x00FF;
	ret = stk3x1x_i2c_write_data(obj_data->client, STK_THDL1_PS_REG, 2, val);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	else
		obj_data->old_ps_thd_l=thd_l;
#ifdef STK_DEBUG_PRINTF
	printk(KERN_ERR "%s: STK_THDL1_PS_REG =%d\n", __func__, thd_l);
#endif
	return ret;
}
static int32_t stk3x1x_set_ps_thd_h(struct stk3x1x_data *obj_data, uint16_t thd_h)
{
	unsigned char val[2];
	int ret;
	val[0] = (thd_h & 0xFF00) >> 8;
	val[1] = thd_h & 0x00FF;
	ret = stk3x1x_i2c_write_data(obj_data->client, STK_THDH1_PS_REG, 2, val);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	else
		obj_data->old_ps_thd_h=thd_h;
#ifdef STK_DEBUG_PRINTF
	printk(KERN_ERR "%s: STK_THDH1_PS_REG = %d\n", __func__, thd_h);
#endif
	return ret;
}

static uint32_t stk3x1x_get_ps_reading(struct stk3x1x_data *obj_data)
{
	unsigned char value[2];
	int err;
	err = stk3x1x_i2c_read_data(obj_data->client, STK_DATA1_PS_REG, 2, &value[0]);
	if(err < 0)
	{
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, err);
		return err;
	}
	return ((value[0]<<8) | value[1]);
}


static int32_t stk3x1x_set_flag(struct stk3x1x_data *obj_data, uint8_t org_flag_reg, uint8_t clr)
{
	uint8_t w_flag;
	int ret;

	w_flag = org_flag_reg | (STK_FLG_ALSINT_MASK | STK_FLG_PSINT_MASK | STK_FLG_OUI_MASK | STK_FLG_IR_RDY_MASK);
	w_flag &= (~clr);
#ifdef STK_DEBUG_PRINTF
	printk(KERN_INFO "%s: org_flag_reg=0x%x, w_flag = 0x%x\n", __func__, org_flag_reg, w_flag);
#endif
    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client,STK_FLAG_REG, w_flag);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	return ret;
}

static int32_t stk3x1x_get_flag(struct stk3x1x_data *obj_data)
{
	int ret;
    	ret = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_FLAG_REG);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	return ret;
}


static int32_t stk3x1x_set_state(struct stk3x1x_data *obj_data, uint8_t state)
{
	int ret;
    	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client,STK_STATE_REG, state);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	return ret;
}

static int32_t stk3x1x_get_state(struct stk3x1x_data *obj_data)
{
	int ret;
    	ret = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_STATE_REG);
	if(ret < 0)
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, ret);
	return ret;
}


static int32_t stk3x1x_enable_ps(struct stk3x1x_data *obj_data, uint8_t enable, uint8_t validate_reg)
{
    	int32_t ret;
	uint8_t w_state_reg;
	uint8_t curr_ps_enable;
	uint32_t reading;
	int32_t near_far_state;

	curr_ps_enable = obj_data->ps_enabled?1:0;
	if(curr_ps_enable == enable)
		return 0;

	mutex_lock(&obj_data->enable_mutex);
        obj_data->sunshine_state = false;
	obj_data->ps_thd_reading = 0;
/*Begin:Modify by TCTSZ zhikui.li@tcl.com,Fix ret value error*/
#ifdef POWER_REGULATOR
    	if (enable) {
		ret = stk3x1x_device_ctl(obj_data, enable);
		if (ret) {
			goto out_err;
		}
	}
#endif

#ifdef STK_CHK_REG
	if(validate_reg)
	{
		ret = stk3x1x_validate_n_handle(obj_data->client);
		if(ret < 0)
			printk(KERN_ERR "stk3x1x_validate_n_handle fail: %d\n", ret);
	}
#endif /* #ifdef STK_CHK_REG */

#ifdef STK_TUNE0
	if (!(obj_data->psi_set) && !enable)
	{

		hrtimer_cancel(&obj_data->ps_tune0_timer);
		cancel_work_sync(&obj_data->stk_ps_tune0_work);

	}
#endif
	if(obj_data->first_boot == true)
	{
		obj_data->first_boot = false;
	}

	ret = stk3x1x_get_state(obj_data);
	if(ret < 0){
		goto out_err;
	}
	w_state_reg = ret;


	w_state_reg &= ~(STK_STATE_EN_PS_MASK | STK_STATE_EN_WAIT_MASK | STK_STATE_EN_AK_MASK);
	if(enable)
	{
		w_state_reg |= STK_STATE_EN_PS_MASK;
		if(!(obj_data->als_enabled))
			w_state_reg |= STK_STATE_EN_WAIT_MASK;
	}
	ret = stk3x1x_set_state(obj_data, w_state_reg);
	if(ret < 0){
		goto out_err;
	}
        ret = 0;
	obj_data->state_reg = w_state_reg;

    if(enable)
    {

#ifdef STK_TUNE0
	#ifndef CALI_EVERY_TIME
		if (!(obj_data->psi_set))
			hrtimer_start(&obj_data->ps_tune0_timer, obj_data->ps_tune0_delay, HRTIMER_MODE_REL);
	#else
		if (1)
		{

#ifdef CTTRACKING
		    obj_data->ps_thd_update = false;
		    obj_data->ps_ori_thd = false;
#endif
/*Begin:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold, outdoor sunlight uses dynamic calibration*/
            obj_data->near_far_state_last=-1;
		    obj_data->old_ps_thd_h = 0xFFFF;
			obj_data->old_ps_thd_l = 0xFFFF;
			obj_data->cali_ps_thd_h = 0xFFFF;
			obj_data->cali_ps_thd_l = 0xFFFF;
/*END by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold, outdoor sunlight uses dynamic calibration*/
			obj_data->psi_set = 0;
			obj_data->psa = 0x0;
			obj_data->psi = 0xFFFF;
			obj_data->ps_high_thd_boot = obj_data->ps_high_thd_def;
            obj_data->ps_low_thd_boot = obj_data->ps_low_thd_def;
			obj_data->ps_thd_h = obj_data->ps_high_thd_boot;
			obj_data->ps_thd_l = obj_data->ps_low_thd_boot;

			obj_data->ps_thd_h = 0xFFFF; //change thd_h to 0xFFFF
			obj_data->ps_thd_l = 0xFFFF;	//change thd_l to 0xFFFF force report far when enable

			stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
			stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);

			hrtimer_start(&obj_data->ps_tune0_timer, obj_data->ps_tune0_delay, HRTIMER_MODE_REL);
        }
	#endif
#endif

#ifdef STK_DEBUG_PRINTF
		printk(KERN_INFO "%s: HT=%d,LT=%d\n", __func__, obj_data->ps_thd_h,  obj_data->ps_thd_l);
#endif
		enable_irq(obj_data->irq);

		obj_data->ps_enabled = true;
		obj_data->is_cali = false;
#ifdef STK_CHK_REG
		if(!validate_reg)
		{
			obj_data->ps_distance_last = 1;
			ps_report_interrupt_data(1);
#ifdef CONFIG_PM_WAKELOCKS
            if (obj_data->ps_wakelock!=NULL)
			    __pm_wakeup_event(obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
			    //__pm_wakeup_event(&obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
#else
			wake_lock_timeout(&obj_data->ps_wakelock, HZ/2);
#endif
			reading = stk3x1x_get_ps_reading(obj_data);
			printk(KERN_INFO "%s: force report ps input event=1, ps code = %d\n",__func__, reading);
        }
		else
#endif /* #ifdef STK_CHK_REG */
		{
			usleep_range(4000, 5000);
			ret = stk3x1x_get_flag(obj_data);
			if (ret < 0) {
				goto out_err;
			}
			near_far_state = ret & STK_FLG_NF_MASK;
			//report a far event to cover the dirty data from dirver or android
			// 0 near  and 1 far
			obj_data->ps_distance_last = near_far_state;
/*begin modified by zhikui.li,Report the far event when opening psensor*/
            ps_report_interrupt_data(1);
/*end modified by zhikui.li,Report the far event when opening psensor*/
			//ps_report_interrupt_data(near_far_state);
			//wake_lock_timeout(&obj_data->ps_wakelock, HZ/2);
			reading = stk3x1x_get_ps_reading(obj_data);
			printk(KERN_INFO "%s: ps input event=%d, ps code = %d\n",__func__, near_far_state, reading);
            ret = 0;
		}
    #ifdef STK33562_IC_USE
        stk3x3x_read_threshold(obj_data);
/* MODIFIED-BEGIN by zhikui.li,Ignore the first 5 data*/
        obj_data->psi_flag = 0;
/* MODIFIED-END by zhikui.li,Ignore the first 5 data*/
    #endif
		printk(KERN_INFO "%s: enable p-sensor ret =%d\n",__func__,ret);
	}
	else  //turn off the psensor
	{

		disable_irq(obj_data->irq);

		cancel_work_sync(&obj_data->stk_work);
		obj_data->ps_enabled = false;
		obj_data->is_cali = false;

		obj_data->ps_thd_h = 0xFFFF; //change thd_h to 0xFFF
		obj_data->ps_thd_l = 0xFFFF;	//change thd_l to 0x0000

		stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
		stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
#ifdef CTTRACKING
		obj_data->ps_thd_update = false;
		obj_data->ps_ori_thd = false;
#endif
		printk(KERN_INFO "%s: disable p-sensor ret =%d\n",__func__,ret);
	}

#ifdef POWER_REGULATOR
	if (!enable) {
		ret = stk3x1x_device_ctl(obj_data, enable);
		if (ret) {
			goto out_err;
		}
                ret = 0;
	}
#endif
/*end:Modify by TCTSZ zhikui.li@tcl.com,Fix ret value error*/
	mutex_unlock(&obj_data->enable_mutex);

	return ret;
out_err:
	mutex_unlock(&obj_data->enable_mutex);

	return ret;

}

static int32_t stk3x1x_enable_als(struct stk3x1x_data *obj_data, uint8_t enable)
{
    	int32_t ret;
	uint8_t w_state_reg;
	uint8_t curr_als_enable = (obj_data->als_enabled)?1:0;

	printk("%s:enable=%d\n", __func__, enable);

	if(curr_als_enable == enable)
		return 0;

	mutex_lock(&obj_data->enable_mutex);
#ifdef POWER_REGULATOR
	if (enable) {
		ret = stk3x1x_device_ctl(obj_data, enable);
		if (ret)
			goto out_err;
	}
#endif
#ifdef STK_CHK_REG
	ret = stk3x1x_validate_n_handle(obj_data->client);
	if(ret < 0)
		printk(KERN_ERR "stk3x1x_validate_n_handle fail: %d\n", ret);
#endif /* #ifdef STK_CHK_REG */

#ifdef STK_IRS
	if(enable && !(obj_data->ps_enabled))
	{
		ret = stk3x1x_get_ir_reading(obj_data);
		if(ret > 0)
			obj_data->ir_code = ret;
	}
#endif


	ret = stk3x1x_get_state(obj_data);
	if(ret < 0)
		goto out_err;

	w_state_reg = (uint8_t)(ret & (~(STK_STATE_EN_ALS_MASK | STK_STATE_EN_WAIT_MASK)));
	if(enable)
		w_state_reg |= STK_STATE_EN_ALS_MASK;
	else if (obj_data->ps_enabled)
		w_state_reg |= STK_STATE_EN_WAIT_MASK;

	ret = stk3x1x_set_state(obj_data, w_state_reg);
	if(ret < 0)
		goto out_err;

	obj_data->state_reg = w_state_reg;

    	if (enable)
    	{
		obj_data->als_enabled = true;

		//mdelay(100);
		hrtimer_start(&obj_data->als_timer, ns_to_ktime(210 * NSEC_PER_MSEC), HRTIMER_MODE_REL);

    	}
	else
	{
		obj_data->als_enabled = false;

		hrtimer_cancel(&obj_data->als_timer);
		cancel_work_sync(&obj_data->stk_als_work);
//begin modify by zhikui.li, Clear data when turning off the light sensor
#ifdef STK_ALS_FIR
	        obj_data->fir.number = 0;
	        obj_data->fir.idx = 0;
	        obj_data->fir.sum = 0;
            obj_data->als_lux_last = -1;
            dark_code_flag = 0;
#endif
//end modify by zhikui.li, Clear data when turning off the light sensor

	}
#ifdef POWER_REGULATOR
	if (!enable) {
		ret = stk3x1x_device_ctl(obj_data, enable);
		if (ret)
			goto out_err;
	}
#endif
	mutex_unlock(&obj_data->enable_mutex);
    	return ret;

out_err:
	mutex_unlock(&obj_data->enable_mutex);
	return ret;
}

static int32_t stk3x1x_get_als_reading(struct stk3x1x_data *obj_data)
{
    	int32_t word_data;
#ifdef STK_ALS_FIR
	int index;
	int firlen = atomic_read(&obj_data->firlength);
#endif
	unsigned char value[2];
	int ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, STK_DATA1_ALS_REG, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}
	word_data = (value[0]<<8) | value[1];

#ifdef STK_ALS_FIR
	if(obj_data->fir.number < firlen)
	{
		obj_data->fir.raw[obj_data->fir.number] = word_data;
		obj_data->fir.sum += word_data;
		obj_data->fir.number++;
		obj_data->fir.idx++;
	}
	else
	{
		index = obj_data->fir.idx % firlen;
		obj_data->fir.sum -= obj_data->fir.raw[index];
		obj_data->fir.raw[index] = word_data;
		obj_data->fir.sum += word_data;
		obj_data->fir.idx++;
		word_data = obj_data->fir.sum/firlen;
	}
#endif

	return word_data;
}

static int32_t stk3x1x_set_irs_it_slp(struct stk3x1x_data *obj_data, uint16_t *slp_time)
{
	uint8_t irs_alsctrl;
	int32_t ret;

	irs_alsctrl = (obj_data->alsctrl_reg & 0x0F) - 2;
	switch(irs_alsctrl)
	{
		case 6:
			*slp_time = 12;
			break;
		case 7:
			*slp_time = 24;
			break;
		case 8:
			*slp_time = 48;
			break;
		case 9:
			*slp_time = 96;
			break;
		default:
			printk(KERN_ERR "%s: unknown ALS IT=0x%x\n", __func__, irs_alsctrl);
			ret = -EINVAL;
			return ret;
	}
	irs_alsctrl |= (obj_data->alsctrl_reg & 0xF0);
	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_ALSCTRL_REG, irs_alsctrl);
	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}
	return 0;
}

static int32_t stk3x1x_get_ir_reading(struct stk3x1x_data *obj_data)
{
    	int32_t word_data, ret;
	uint8_t w_reg, retry = 0;
	uint16_t irs_slp_time = 100;
	bool re_enable_ps = false;
	unsigned char value[2];

	if(obj_data->ps_enabled)
	{
#ifdef STK_TUNE0
		if (!(obj_data->psi_set))
		{
			hrtimer_cancel(&obj_data->ps_tune0_timer);
			cancel_work_sync(&obj_data->stk_ps_tune0_work);
		}
#endif
		stk3x1x_enable_ps(obj_data, 0, 1);
		re_enable_ps = true;
	}

	ret = stk3x1x_set_irs_it_slp(obj_data, &irs_slp_time);
	if(ret < 0)
		goto irs_err_i2c_rw;

	ret = stk3x1x_get_state(obj_data);
	if(ret < 0)
		goto irs_err_i2c_rw;

	w_reg = ret | STK_STATE_EN_IRS_MASK;
	ret = stk3x1x_set_state(obj_data, w_reg);
	if(ret < 0)
		goto irs_err_i2c_rw;
	msleep(irs_slp_time);

	do
	{
		usleep_range(3000, 4000);
		//msleep(3);
		ret = stk3x1x_get_flag(obj_data);
		if (ret < 0)
			goto irs_err_i2c_rw;
		retry++;
	}while(retry < 10 && ((ret&STK_FLG_IR_RDY_MASK) == 0));

	if(retry == 10)
	{
		printk(KERN_ERR "%s: ir data is not ready for 300ms\n", __func__);
		ret = -EINVAL;
		goto irs_err_i2c_rw;
	}

	ret = stk3x1x_set_flag(obj_data, ret, STK_FLG_IR_RDY_MASK);
    	if (ret < 0)
		goto irs_err_i2c_rw;

	ret = stk3x1x_i2c_read_data(obj_data->client, STK_DATA1_IR_REG, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		goto irs_err_i2c_rw;
	}
	word_data = ((value[0]<<8) | value[1]);

	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_ALSCTRL_REG, obj_data->alsctrl_reg );
	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		goto irs_err_i2c_rw;
	}
	if(re_enable_ps)
		stk3x1x_enable_ps(obj_data, 1, 1);
	return word_data;

irs_err_i2c_rw:
	if(re_enable_ps)
		stk3x1x_enable_ps(obj_data, 1, 1);
	return ret;
}

#ifdef STK_CHK_REG
static int stk3x1x_chk_reg_valid(struct stk3x1x_data *obj_data)
{
	unsigned char value[9];
	//int err;

	uint8_t cnt;

	for(cnt=0;cnt<9;cnt++)
	{
		value[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, (cnt+1));
		if(value[cnt] < 0)
		{
			printk(KERN_ERR "%s fail, ret=%d", __func__, value[cnt]);
			return value[cnt];
		}
	}
	/*err = stk3x1x_i2c_read_data(obj_data->client, STK_PSCTRL_REG, 9, &value[0]);
	if(err < 0)
	{
		printk(KERN_ERR "%s: fail, ret=%d\n", __func__, err);
		return err;
	}*/

	if(value[0] != obj_data->psctrl_reg)
	{
		printk(KERN_ERR "%s: invalid reg 0x01=0x%2x\n", __func__, value[0]);
		return 0xFF;
	}
	if(value[1] != obj_data->alsctrl_reg)
	{
		printk(KERN_ERR "%s: invalid reg 0x02=0x%2x\n", __func__, value[1]);
		return 0xFF;
	}
	if(value[2] != obj_data->ledctrl_reg)
	{
		printk(KERN_ERR "%s: invalid reg 0x03=0x%2x\n", __func__, value[2]);
		return 0xFF;
	}
	if(value[3] != obj_data->int_reg)
	{
		printk(KERN_ERR "%s: invalid reg 0x04=0x%2x\n", __func__, value[3]);
		return 0xFF;
	}
	if(value[4] != obj_data->wait_reg)
	{
		printk(KERN_ERR "%s: invalid reg 0x05=0x%2x\n", __func__, value[4]);
		return 0xFF;
	}
	if(value[5] != ((obj_data->ps_thd_h & 0xFF00) >> 8))
	{
		printk(KERN_ERR "%s: invalid reg 0x06=0x%2x  ps_thd_h=0x%x\n", __func__, value[5],obj_data->ps_thd_h);
		return 0xFF;
	}
	if(value[6] != (obj_data->ps_thd_h & 0x00FF))
	{
		printk(KERN_ERR "%s: invalid reg 0x07=0x%2x  ps_thd_h=0x%x\n", __func__, value[6],obj_data->ps_thd_h);
		return 0xFF;
	}
	if(value[7] != ((obj_data->ps_thd_l & 0xFF00) >> 8))
	{
		printk(KERN_ERR "%s: invalid reg 0x08=0x%2x\n", __func__, value[7]);
		return 0xFF;
	}
	if(value[8] != (obj_data->ps_thd_l & 0x00FF))
	{
		printk(KERN_ERR "%s: invalid reg 0x09=0x%2x\n", __func__, value[8]);
		return 0xFF;
	}

	return 0;
}

static int stk3x1x_validate_n_handle(struct i2c_client *client)
{
	struct stk3x1x_data *obj_data = i2c_get_clientdata(client);
	int err;

	err = stk3x1x_chk_reg_valid(obj_data);
	if(err < 0)
	{
		printk(KERN_ERR "stk3x1x_chk_reg_valid fail: %d\n", err);
		return err;
	}

	if(err == 0xFF)
	{
		printk(KERN_ERR "%s: Re-init chip\n", __func__);
		err = stk3x1x_software_reset(obj_data);
		if(err < 0)
			return err;
		err = stk3x1x_init_all_reg(obj_data);
		if(err < 0)
			return err;

		//obj_data->psa = 0;
		//obj_data->psi = 0xFFFF;
		stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
		stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
#ifdef STK_ALS_FIR
		memset(&obj_data->fir, 0x00, sizeof(obj_data->fir));
#endif

		return 0xFF;
	}
	return 0;
}
#endif /* #ifdef STK_CHK_REG */
static ssize_t stk_als_code_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	int32_t reading;
	//reading = stk3x1x_get_als_reading(obj_data);
	unsigned char value[2];
	int ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, STK_DATA1_ALS_REG, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}
	reading = (value[0]<<8) | value[1];
    return scnprintf(buf, PAGE_SIZE, "%d\n", reading);
}


static ssize_t stk_als_enable_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    int32_t ret;

	ret = stk3x1x_get_state(obj_data);
	if(ret < 0)
		return ret;
    ret = (ret & STK_STATE_EN_ALS_MASK)?1:0;

	return scnprintf(buf, PAGE_SIZE, "%d\n", ret);
}

static ssize_t stk_als_enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data = dev_get_drvdata(dev);
	uint8_t en;
	if (sysfs_streq(buf, "1"))
		en = 1;
	else if (sysfs_streq(buf, "0"))
		en = 0;
	else
	{
		printk(KERN_ERR "%s, invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}
    	printk(KERN_INFO "%s: Enable ALS : %d\n", __func__, en);
    	mutex_lock(&obj_data->io_lock);
    	stk3x1x_enable_als(obj_data, en);
    	mutex_unlock(&obj_data->io_lock);
    	return size;
}

static ssize_t stk_als_lux_show(struct device *dev, struct device_attribute *attr, char *buf)
{
     struct stk3x1x_data *obj_data = dev_get_drvdata(dev);
     int32_t als_reading;
     uint32_t als_lux;
     als_reading = stk3x1x_get_als_reading(obj_data);
     als_reading = als_reading * obj_data->als_correct_factor / 1000;
     als_lux = stk_alscode2lux(obj_data, als_reading);
     return scnprintf(buf, PAGE_SIZE, "%d lux\n", als_lux);
}

static ssize_t stk_als_lux_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 16, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
    obj_data->als_lux_last = value;
	//input_report_abs(obj_data->als_input_dev, ABS_MISC, value);
	//input_sync(obj_data->als_input_dev);
	//stk3x1x_report_value(obj_data->als_input_dev, ABS_MISC, value);
	printk(KERN_INFO "%s: als input event %ld lux\n",__func__, value);

    return size;
}


static ssize_t stk_als_transmittance_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	int32_t transmittance;
    	transmittance = obj_data->als_transmittance;
    	return scnprintf(buf, PAGE_SIZE, "%d\n", transmittance);
}


static ssize_t stk_als_transmittance_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
    	obj_data->als_transmittance = value;
    	return size;
}

static ssize_t stk_als_delay_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	int64_t delay;
	mutex_lock(&obj_data->io_lock);
	delay = ktime_to_ns(obj_data->als_poll_delay);
	mutex_unlock(&obj_data->io_lock);
	return scnprintf(buf, PAGE_SIZE, "%lld\n", delay);
}


static ssize_t stk_als_delay_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    	uint64_t value = 0;
	int ret;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	ret = kstrtoull(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoull failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
#ifdef STK_DEBUG_PRINTF
	printk(KERN_INFO "%s: set als poll delay=%lld\n", __func__, value);
#endif
	if(value < MIN_ALS_POLL_DELAY_NS)
	{
		printk(KERN_ERR "%s: delay is too small\n", __func__);
		value = MIN_ALS_POLL_DELAY_NS;
	}
	mutex_lock(&obj_data->io_lock);
	if(value != ktime_to_ns(obj_data->als_poll_delay))
		obj_data->als_poll_delay = ns_to_ktime(value);
#ifdef STK_ALS_FIR
	obj_data->fir.number = 0;
	obj_data->fir.idx = 0;
	obj_data->fir.sum = 0;
#endif
	mutex_unlock(&obj_data->io_lock);
	return size;
}

static ssize_t stk_als_ir_code_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	int32_t reading;
    	reading = stk3x1x_get_ir_reading(obj_data);
    	return scnprintf(buf, PAGE_SIZE, "%d\n", reading);
}

#ifdef STK_ALS_FIR
static ssize_t stk_als_firlen_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	int len = atomic_read(&obj_data->firlength);

	printk(KERN_INFO "%s: len = %2d, idx = %2d\n", __func__, len, obj_data->fir.idx);
	printk(KERN_INFO "%s: sum = %5d, ave = %5d\n", __func__, obj_data->fir.sum, obj_data->fir.sum/len);

	return scnprintf(buf, PAGE_SIZE, "%d\n", len);
}


static ssize_t stk_als_firlen_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    	uint64_t value = 0;
	int ret;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	ret = kstrtoull(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoull failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	if(value > MAX_FIR_LEN)
	{
		printk(KERN_ERR "%s: firlen exceed maximum filter length\n", __func__);
	}
	else if (value < 1)
	{
		atomic_set(&obj_data->firlength, 1);
		memset(&obj_data->fir, 0x00, sizeof(obj_data->fir));
	}
	else
	{
		atomic_set(&obj_data->firlength, value);
		memset(&obj_data->fir, 0x00, sizeof(obj_data->fir));
	}
	return size;
}
#endif  /* #ifdef STK_ALS_FIR */
#ifndef STK33562_IC_USE
static int stk_ps_reading_val_reg(struct stk3x1x_data *obj_data)
{
	int32_t word_data;
	unsigned char value[4];
	int ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, 0x20, 4, value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}

	word_data = (value[0]<<8) | value[1];
	word_data += ((value[2]<<8) | value[3]);

	return word_data;
}
#else
static int stk_ps_reading_val_reg_stk33562(struct stk3x1x_data *obj_data)
{

	u8 ps_off_data_0[4] = {0};
	u8 ps_off_data_1[4] = {0};
	u16 ps_off_data[4] = {0};
    u8 bgir_raw_data[4] = {0};
	int i = 0,ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, 0x24, 4, ps_off_data_0);
	ret = stk3x1x_i2c_read_data(obj_data->client, 0x28, 4, ps_off_data_1);
	ps_off_data[0] = (ps_off_data_0[0] <<8 | ps_off_data_0[1]);
	ps_off_data[1] = (ps_off_data_0[2] <<8 | ps_off_data_0[3]);
	ps_off_data[2] = (ps_off_data_1[0] <<8 | ps_off_data_1[1]);
	ps_off_data[3] = (ps_off_data_1[2] <<8 | ps_off_data_1[3]);
	for(i=0;i<4;i++)
	{
	     if(ps_off_data[i]>150){
	        printk("stk bright pd  %d = %d\n",i ,ps_off_data[i]);
	        return 0xFFFF;
	     }
	}
	ret = stk3x1x_i2c_read_data(obj_data->client, 0x34, 4, bgir_raw_data);
    if( ((bgir_raw_data[0] & 0x7f) >= 3) ||
            ((bgir_raw_data[1] & 0x7f) >= 3) || ((bgir_raw_data[2] & 0x7f) >= 3) || ((bgir_raw_data[3] & 0x7f) >= 3) )
    {
	      printk("stk bright bgir0=%d,  bgir1=%d, bgir2=%d, bgir3=%d\n",bgir_raw_data[0],bgir_raw_data[1],bgir_raw_data[2],bgir_raw_data[3]);
          return 0xFFFF;
    }
    return 0;
}
#endif
static ssize_t stk_ps_code_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	uint32_t reading;
    	reading = stk3x1x_get_ps_reading(obj_data);
    	return scnprintf(buf, PAGE_SIZE, "%d\n", reading);
}

static ssize_t stk_ps_prx_raw_off_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	uint32_t reading;
    	reading = stk3x1x_get_ps_reading(obj_data);
    	return scnprintf(buf, PAGE_SIZE, "%d\n", reading);
}

static ssize_t stk_ps_prx_raw_org_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	uint32_t reading;
    	reading = stk3x1x_get_ps_reading(obj_data);
    	return scnprintf(buf, PAGE_SIZE, "%d\n", reading);
}


static ssize_t stk_ps_enable_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    	int32_t ret;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	ret = stk3x1x_get_state(obj_data);
	if(ret < 0)
		return ret;
    	ret = (ret & STK_STATE_EN_PS_MASK)?1:0;

	return scnprintf(buf, PAGE_SIZE, "%d\n", ret);
}

static ssize_t stk_ps_enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	uint8_t en;
	if (sysfs_streq(buf, "1"))
		en = 1;
	else if (sysfs_streq(buf, "0"))
		en = 0;
	else
	{
		printk(KERN_ERR "%s, invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}
    	printk(KERN_INFO "%s: Enable PS : %d\n", __func__, en);
    	mutex_lock(&obj_data->io_lock);
    	stk3x1x_enable_ps(obj_data, en, 1);
    	mutex_unlock(&obj_data->io_lock);
    	return size;
}

static ssize_t stk_ps_enable_aso_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    	int32_t ret;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

    	ret = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_STATE_REG);
    	ret = (ret & STK_STATE_EN_ASO_MASK)?1:0;

	return scnprintf(buf, PAGE_SIZE, "%d\n", ret);
}

static ssize_t stk_ps_enable_aso_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	uint8_t en;
    int32_t ret;
	uint8_t w_state_reg;

	if (sysfs_streq(buf, "1"))
		en = 1;
	else if (sysfs_streq(buf, "0"))
		en = 0;
	else
	{
		printk(KERN_ERR "%s, invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}
    printk(KERN_INFO "%s: Enable PS ASO : %d\n", __func__, en);

    ret = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, STK_STATE_REG);
    if (ret < 0)
    {
        printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
    }
	w_state_reg = (uint8_t)(ret & (~STK_STATE_EN_ASO_MASK));
	if(en)
		w_state_reg |= STK_STATE_EN_ASO_MASK;

    ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_STATE_REG, w_state_reg);
    if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}

	return size;
}


static ssize_t stk_ps_offset_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    int32_t word_data;
	unsigned char value[2];
	int ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, STK_DATA1_OFFSET_REG, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x", __func__, ret);
		return ret;
	}
	word_data = (value[0]<<8) | value[1];

	return scnprintf(buf, PAGE_SIZE, "%d\n", word_data);
}

static ssize_t stk_ps_offset_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	unsigned long offset = 0;
	int ret;
	unsigned char val[2];

	ret = kstrtoul(buf, 10, &offset);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
	if(offset > 65535)
	{
		printk(KERN_ERR "%s: invalid value, offset=%ld\n", __func__, offset);
		return -EINVAL;
	}

	val[0] = (offset & 0xFF00) >> 8;
	val[1] = offset & 0x00FF;
	ret = stk3x1x_i2c_write_data(obj_data->client, STK_DATA1_OFFSET_REG, 2, val);
	if(ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}

	return size;
}


static ssize_t stk_ps_distance_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	int32_t dist=1, ret;

    	ret = stk3x1x_get_flag(obj_data);
	if(ret < 0)
		return ret;
    	dist = (ret & STK_FLG_NF_MASK)?1:0;

    	obj_data->ps_distance_last = dist;
	ps_report_interrupt_data(dist);
#ifdef CONFIG_PM_WAKELOCKS
    if (obj_data->ps_wakelock!=NULL)
	    __pm_wakeup_event(obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
	    //__pm_wakeup_event(&obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
#else
	wake_lock_timeout(&obj_data->ps_wakelock, HZ/2);
#endif
	printk(KERN_INFO "%s: ps input event %d cm\n",__func__, dist);
    	return scnprintf(buf, PAGE_SIZE, "%d\n", dist);
}


static ssize_t stk_ps_distance_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
    	obj_data->ps_distance_last = value;
	ps_report_interrupt_data(value);
#ifdef CONFIG_PM_WAKELOCKS
    if (obj_data->ps_wakelock!=NULL)
    	__pm_wakeup_event(obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
	//__pm_wakeup_event(&obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
#else
	wake_lock_timeout(&obj_data->ps_wakelock, HZ/2);
#endif
	printk(KERN_INFO "%s: ps input event %ld cm\n",__func__, value);
    	return size;
}


static ssize_t stk_ps_code_thd_l_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    	int32_t ps_thd_l1_reg, ps_thd_l2_reg;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	ps_thd_l1_reg = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_THDL1_PS_REG);
    	if(ps_thd_l1_reg < 0)
	{
		printk(KERN_ERR "%s fail, err=0x%x\n", __func__, ps_thd_l1_reg);
		return -EINVAL;
	}
    	ps_thd_l2_reg = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_THDL2_PS_REG);
    	if(ps_thd_l2_reg < 0)
	{
		printk(KERN_ERR "%s fail, err=0x%x\n", __func__, ps_thd_l2_reg);
		return -EINVAL;
	}
	ps_thd_l1_reg = ps_thd_l1_reg<<8 | ps_thd_l2_reg;
    	return scnprintf(buf, PAGE_SIZE, "%d\n", ps_thd_l1_reg);
}


static ssize_t stk_ps_code_thd_l_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
    	stk3x1x_set_ps_thd_l(obj_data, value);
    	return size;
}

static ssize_t stk_ps_code_thd_h_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    	int32_t ps_thd_h1_reg, ps_thd_h2_reg;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
    	ps_thd_h1_reg = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_THDH1_PS_REG);
    	if(ps_thd_h1_reg < 0)
	{
		printk(KERN_ERR "%s fail, err=0x%x\n", __func__, ps_thd_h1_reg);
		return -EINVAL;
	}
    	ps_thd_h2_reg = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,STK_THDH2_PS_REG);
    	if(ps_thd_h2_reg < 0)
	{
		printk(KERN_ERR "%s fail, err=0x%x\n", __func__, ps_thd_h2_reg);
		return -EINVAL;
	}
	ps_thd_h1_reg = ps_thd_h1_reg<<8 | ps_thd_h2_reg;
    	return scnprintf(buf, PAGE_SIZE, "%d\n", ps_thd_h1_reg);
}


static ssize_t stk_ps_code_thd_h_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
    	stk3x1x_set_ps_thd_h(obj_data, value);
    	return size;
}


static ssize_t stk_all_reg_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    	int32_t ps_reg[0x22];
	uint8_t cnt;
	int len = 0;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	for(cnt=0;cnt<0x20;cnt++)
	{
		ps_reg[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, (cnt));
		if(ps_reg[cnt] < 0)
		{
			printk(KERN_ERR "%s fail, ret=%d\n", __func__, ps_reg[cnt]);
			return -EINVAL;
		}
		else
		{
			printk(KERN_INFO "reg[0x%2X]=0x%2X\n", cnt, ps_reg[cnt]);
			len += scnprintf(buf+len, PAGE_SIZE-len, "[%2X]%2X,", cnt, ps_reg[cnt]);
		}
	}
	ps_reg[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, STK_PDT_ID_REG);
	if(ps_reg[cnt] < 0)
	{
		printk( KERN_ERR "%s fail, ret=%d\n", __func__, ps_reg[cnt]);
		return -EINVAL;
	}
	printk( KERN_INFO "reg[0x%x]=0x%2X\n", STK_PDT_ID_REG, ps_reg[cnt]);
	cnt++;
	ps_reg[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, STK_RSRVD_REG);
	if(ps_reg[cnt] < 0)
	{
		printk( KERN_ERR "%s fail, ret=%d\n", __func__, ps_reg[cnt]);
		return -EINVAL;
	}
	printk( KERN_INFO "reg[0x%x]=0x%2X\n", STK_RSRVD_REG, ps_reg[cnt]);
	len += scnprintf(buf+len, PAGE_SIZE-len, "[%2X]%2X,[%2X]%2X\n", cnt-1, ps_reg[cnt-1], cnt, ps_reg[cnt]);
	return len;
/*
    return scnprintf(buf, PAGE_SIZE, "[0]%2X [1]%2X [2]%2X [3]%2X [4]%2X [5]%2X [6/7 HTHD]%2X,%2X [8/9 LTHD]%2X, %2X [A]%2X [B]%2X [C]%2X [D]%2X [E/F Aoff]%2X,%2X,[10]%2X [11/12 PS]%2X,%2X [13]%2X [14]%2X [15/16 Foff]%2X,%2X [17]%2X [18]%2X [3E]%2X [3F]%2X\n", 	
		ps_reg[0], ps_reg[1], ps_reg[2], ps_reg[3], ps_reg[4], ps_reg[5], ps_reg[6], ps_reg[7], ps_reg[8],
		ps_reg[9], ps_reg[10], ps_reg[11], ps_reg[12], ps_reg[13], ps_reg[14], ps_reg[15], ps_reg[16], ps_reg[17],
		ps_reg[18], ps_reg[19], ps_reg[20], ps_reg[21], ps_reg[22], ps_reg[23], ps_reg[24], ps_reg[25], ps_reg[26]);
		*/
}

static ssize_t stk_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    	int32_t ps_reg[27];
	uint8_t cnt;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	for(cnt=0;cnt<25;cnt++)
	{
		ps_reg[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, (cnt));
		if(ps_reg[cnt] < 0)
		{
			printk(KERN_ERR "%s fail, ret=%d\n", __func__, ps_reg[cnt]);
			return -EINVAL;
		}
		else
		{
			printk(KERN_INFO "reg[0x%2X]=0x%2X\n", cnt, ps_reg[cnt]);
		}
	}
	ps_reg[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, STK_PDT_ID_REG);
	if(ps_reg[cnt] < 0)
	{
		printk( KERN_ERR "%s fail, ret=%d\n", __func__, ps_reg[cnt]);
		return -EINVAL;
	}
	printk( KERN_INFO "reg[0x%x]=0x%2X\n", STK_PDT_ID_REG, ps_reg[cnt]);
	cnt++;
	ps_reg[cnt] = stk3x1x_i2c_smbus_read_byte_data(obj_data->client, STK_RSRVD_REG);
	if(ps_reg[cnt] < 0)
	{
		printk( KERN_ERR "%s fail, ret=%d\n", __func__, ps_reg[cnt]);
		return -EINVAL;
	}
	printk( KERN_INFO "reg[0x%x]=0x%2X\n", STK_RSRVD_REG, ps_reg[cnt]);

    return scnprintf(buf, PAGE_SIZE, "[PS=%2X] [ALS=%2X] [WAIT=0x%4Xms] [EN_ASO=%2X] [EN_AK=%2X] [NEAR/FAR=%2X] [FLAG_OUI=%2X] [FLAG_PSINT=%2X] [FLAG_ALSINT=%2X]\n",
		ps_reg[0]&0x01,(ps_reg[0]&0x02)>>1,((ps_reg[0]&0x04)>>2)*ps_reg[5]*6,(ps_reg[0]&0x20)>>5,
		(ps_reg[0]&0x40)>>6,ps_reg[16]&0x01,(ps_reg[16]&0x04)>>2,(ps_reg[16]&0x10)>>4,(ps_reg[16]&0x20)>>5);
}

static ssize_t stk_recv_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	return scnprintf(buf, PAGE_SIZE, "0x%04X\n", atomic_read(&obj_data->recv_reg));
}


static ssize_t stk_recv_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
    	unsigned long value = 0;
	int ret;
	int32_t recv_data;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	if((ret = kstrtoul(buf, 16, &value)) < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
	recv_data = stk3x1x_i2c_smbus_read_byte_data(obj_data->client,value);
//	printk("%s: reg 0x%x=0x%x\n", __func__, (int)value, recv_data);
	atomic_set(&obj_data->recv_reg, recv_data);
	return size;
}


static ssize_t stk_send_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}


static ssize_t stk_send_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	int addr, cmd;
	int32_t ret, i;
	char *token[10];
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	for (i = 0; i < 2; i++)
		token[i] = strsep((char **)&buf, " ");
	if((ret = kstrtoul(token[0], 16, (unsigned long *)&(addr))) < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
	if((ret = kstrtoul(token[1], 16, (unsigned long *)&(cmd))) < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}
	printk(KERN_INFO "%s: write reg 0x%x=0x%x\n", __func__, addr, cmd);

	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, (unsigned char)addr, (unsigned char)cmd);
	if (0 != ret)
	{
		printk(KERN_ERR "%s: stk3x1x_i2c_smbus_write_byte_data fail\n", __func__);
		return ret;
	}

	return size;
}

#ifdef STK_TUNE0

static ssize_t stk_ps_cali_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);
	int32_t word_data;
	unsigned char value[2];
	int ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, 0x20, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}
	word_data = (value[0]<<8) | value[1];

	ret = stk3x1x_i2c_read_data(obj_data->client, 0x22, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}
	word_data += ((value[0]<<8) | value[1]);

#ifdef CALI_EVERY_TIME
	printk(KERN_INFO "%s: boot HT=%d, LT=%d\n", __func__, obj_data->ps_high_thd_boot, obj_data->ps_low_thd_boot);
#endif

	printk("%s: psi_set=%d, psa=%d,psi=%d, word_data=%d\n", __func__,
		obj_data->psi_set, obj_data->psa, obj_data->psi, word_data);

	return 0;
}

static ssize_t stk_mmi_prx_thd_h_limit_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", obj_data->pdata->mmi_thd_h_limit);
}

static ssize_t stk_mmi_prx_thd_h_limit_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned long value = 0;
	int ret;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	ret = kstrtoul(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	obj_data->pdata->mmi_thd_h_limit = value;

	return size;
}

static ssize_t stk_mmi_prx_thd_l_limit_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", obj_data->pdata->mmi_thd_l_limit);
}

static ssize_t stk_mmi_prx_thd_l_limit_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned long value = 0;
	int ret;
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	ret = kstrtoul(buf, 10, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	obj_data->pdata->mmi_thd_l_limit = value;

	return size;
}

static ssize_t stk_trace_param_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t stk_trace_param_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	struct stk3x1x_data *obj_data =  dev_get_drvdata(dev);

	printk("%s:%d\n", __func__, (int)size);
	memcpy(&obj_data->factory_data, buf, sizeof(struct trace_data));

	if (obj_data->factory_data.tag == TRACE_TAG) {
		if (obj_data->factory_data.factory_def_thd_offset.tag == DEF_TH_OFFSET_TAG) {
			obj_data->pdata->low_ps_ct_h_offset = obj_data->factory_data.factory_def_thd_offset.low_ps_cali_th_offset_h;
			obj_data->pdata->low_ps_ct_l_offset = obj_data->factory_data.factory_def_thd_offset.low_ps_cali_th_offset_l;
			obj_data->pdata->hi_ps_ct_h_offset  = obj_data->factory_data.factory_def_thd_offset.hi_ps_cali_th_offset_h;
			obj_data->pdata->hi_ps_ct_l_offset  = obj_data->factory_data.factory_def_thd_offset.hi_ps_cali_th_offset_l;

			obj_data->is_have_good_cali = false;
			obj_data->last_good_psi = 0xffff;
			obj_data->min_psi = 0xffff;

			printk("trace:low_ps_ct_h_off=%d,low_ps_ct_l_off=%d,hi_ps_ct_h_off=%d,hi_ps_ct_l_off=%d\n", \
			obj_data->pdata->low_ps_ct_h_offset, obj_data->pdata->low_ps_ct_l_offset, \
			obj_data->pdata->hi_ps_ct_h_offset, obj_data->pdata->hi_ps_ct_l_offset);
		}

		if (obj_data->factory_data.factory_cali_data.tag == CALI_DATA_TAG) {
			obj_data->psctrl_reg = obj_data->factory_data.factory_cali_data.psctrl;
			obj_data->ledctrl_reg = obj_data->factory_data.factory_cali_data.ledctrl;

			obj_data->is_have_good_cali = false;
			obj_data->last_good_psi = 0xffff;
			obj_data->min_psi = 0xffff;

			printk("Get psensor cali data from trace,psctrl_reg=%02x,ledctrl_reg=%02x\n", obj_data->psctrl_reg, obj_data->ledctrl_reg);
		}

		if (obj_data->factory_data.factory_def_thd_limit.tag == DEF_TH_LIMIT_TAG) {
			obj_data->pdata->mmi_thd_h_limit = obj_data->factory_data.factory_def_thd_limit.mmi_thd_h_limit;
			obj_data->pdata->mmi_thd_l_limit = obj_data->factory_data.factory_def_thd_limit.mmi_thd_l_limit;
			printk("Get psensor judge faulty thredhold from trace:H=%d,L=%d\n", obj_data->pdata->mmi_thd_h_limit, obj_data->pdata->mmi_thd_l_limit);
		}

		if (obj_data->factory_data.factory_def_mmi_data.tag == DEF_MMI_DATA) {
			printk("Get psensor ramdata from trace:%d\n", obj_data->factory_data.factory_def_mmi_data.mmi_data_rawdata);
		}
	}

	return size;
}


#endif	/* #ifdef STK_TUNE0 */

static struct device_attribute als_enable_attribute = __ATTR(als_enable,0664,stk_als_enable_show,stk_als_enable_store);
static struct device_attribute als_lux_attribute = __ATTR(lux,0664,stk_als_lux_show,stk_als_lux_store);
static struct device_attribute als_code_attribute = __ATTR(als_code, 0444, stk_als_code_show, NULL);
static struct device_attribute als_transmittance_attribute = __ATTR(transmittance,0664,stk_als_transmittance_show,stk_als_transmittance_store);
static struct device_attribute als_poll_delay_attribute = __ATTR(delay,0664,stk_als_delay_show,stk_als_delay_store);
static struct device_attribute als_ir_code_attribute = __ATTR(ircode,0444,stk_als_ir_code_show,NULL);
#ifdef STK_ALS_FIR
static struct device_attribute als_firlen_attribute = __ATTR(firlen,0664,stk_als_firlen_show,stk_als_firlen_store);
#endif

static struct attribute *stk_als_attrs [] =
{
	&als_enable_attribute.attr,
	&als_lux_attribute.attr,
	&als_code_attribute.attr,
    	&als_transmittance_attribute.attr,
	&als_poll_delay_attribute.attr,
	&als_ir_code_attribute.attr,
#ifdef STK_ALS_FIR
	&als_firlen_attribute.attr,
#endif
    	NULL
};

static struct attribute_group stk_als_attribute_group = {
	//.name = "driver",
	.attrs = stk_als_attrs,
};


static struct device_attribute ps_enable_attribute = __ATTR(ps_enable,0664,stk_ps_enable_show,stk_ps_enable_store);
static struct device_attribute ps_enable_aso_attribute = __ATTR(enableaso,0664,stk_ps_enable_aso_show,stk_ps_enable_aso_store);
static struct device_attribute ps_distance_attribute = __ATTR(distance,0664,stk_ps_distance_show, stk_ps_distance_store);
static struct device_attribute ps_offset_attribute = __ATTR(prx_offset,0664,stk_ps_offset_show, stk_ps_offset_store);
static struct device_attribute ps_code_attribute = __ATTR(ps_code, 0444, stk_ps_code_show, NULL);
static struct device_attribute ps_prx_raw_off_attribute = __ATTR(prx_raw_off, 0444, stk_ps_prx_raw_off_show, NULL);
static struct device_attribute ps_prx_raw_org_attribute = __ATTR(prx_raw_org, 0444, stk_ps_prx_raw_org_show, NULL);
static struct device_attribute ps_code_thd_l_attribute = __ATTR(codethdl,0664,stk_ps_code_thd_l_show,stk_ps_code_thd_l_store);
static struct device_attribute ps_code_thd_h_attribute = __ATTR(codethdh,0664,stk_ps_code_thd_h_show,stk_ps_code_thd_h_store);
static struct device_attribute ps_recv_attribute = __ATTR(recv,0664,stk_recv_show,stk_recv_store);
static struct device_attribute ps_send_attribute = __ATTR(send,0664,stk_send_show, stk_send_store);
static struct device_attribute all_reg_attribute = __ATTR(allreg, 0444, stk_all_reg_show, NULL);
static struct device_attribute status_attribute = __ATTR(status, 0444, stk_status_show, NULL);
static struct device_attribute mmi_prx_thd_h_limit_attribute = __ATTR(mmi_prx_thd_h_limit, 0664, stk_mmi_prx_thd_h_limit_show, stk_mmi_prx_thd_h_limit_store);
static struct device_attribute mmi_prx_thd_l_limit_attribute = __ATTR(mmi_prx_thd_l_limit, 0664, stk_mmi_prx_thd_l_limit_show, stk_mmi_prx_thd_l_limit_store);
static struct device_attribute trace_param_attribute = __ATTR(trace_param, 0664, stk_trace_param_show, stk_trace_param_store);
#ifdef STK_TUNE0
static struct device_attribute ps_cali_attribute = __ATTR(cali,0444,stk_ps_cali_show, NULL);
#endif

static struct attribute *stk_ps_attrs [] =
{
    	&ps_enable_attribute.attr,
    	&ps_enable_aso_attribute.attr,
    	&ps_distance_attribute.attr,
	&ps_offset_attribute.attr,
    	&ps_code_attribute.attr,
    	&ps_prx_raw_off_attribute.attr,
	&ps_prx_raw_org_attribute.attr,
	&ps_code_thd_l_attribute.attr,
	&ps_code_thd_h_attribute.attr,
	&ps_recv_attribute.attr,
	&ps_send_attribute.attr,
	&all_reg_attribute.attr,
	&status_attribute.attr,
	&mmi_prx_thd_h_limit_attribute.attr,
	&mmi_prx_thd_l_limit_attribute.attr,
	&trace_param_attribute.attr,

#ifdef STK_TUNE0
	&ps_cali_attribute.attr,
#endif
    NULL
};

static struct attribute_group stk_ps_attribute_group = {
	//.name = "driver",
	.attrs = stk_ps_attrs,
};


static ssize_t als_show(struct device_driver *ddri, char *buf)
{
	int32_t reading;
	unsigned char value[2];
	int ret;

	ret = stk3x1x_i2c_read_data(stk3x1x_obj->client, STK_DATA1_ALS_REG, 2, &value[0]);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}
	reading = (value[0]<<8) | value[1];
    	return scnprintf(buf, PAGE_SIZE, "%d\n", reading);
}

static ssize_t ps_show(struct device_driver *ddri, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", stk3x1x_get_ps_reading(stk3x1x_obj));
}

static ssize_t mmi_prx_thd_h_limit_show(struct device_driver *ddri, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", stk3x1x_obj->pdata->mmi_thd_h_limit);
}

static ssize_t mmi_prx_thd_h_limit_store(struct device_driver *ddri, const char *buf, size_t size)
{
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 16, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	stk3x1x_obj->pdata->mmi_thd_h_limit = value;

	return ret;
}

static ssize_t mmi_prx_thd_l_limit_show(struct device_driver *ddri, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", stk3x1x_obj->pdata->mmi_thd_l_limit);
}

static ssize_t mmi_prx_thd_l_limit_store(struct device_driver *ddri, const char *buf, size_t size)
{
	unsigned long value = 0;
	int ret;
	ret = kstrtoul(buf, 16, &value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s:kstrtoul failed, ret=0x%x\n", __func__, ret);
		return ret;
	}

	stk3x1x_obj->pdata->mmi_thd_l_limit = value;

	return ret;
}

/*begin modified by zhikui.li,add deviceinfo node*/
static ssize_t psensor_show(struct device* dev,struct device_attribute *attr, char *buf)
{
        return snprintf(buf, PAGE_SIZE, "%s\n", "STK3X3X Chip");
}
extern struct device* get_deviceinfo_dev(void);
static DEVICE_ATTR(psensor, 0444, psensor_show, NULL);
static int create_chipinfo_node(void)
{
       struct device * chipinfo;
       int err=0;
       chipinfo = get_deviceinfo_dev();
       if(chipinfo != NULL){
           err = device_create_file(chipinfo, &dev_attr_psensor);
           if (err){
               printk("Failed to create device file(%s)!\n", dev_attr_psensor.attr.name);
           }
       }
       return err;
}
/*end modified by zhikui.li,add deviceinfo node*/
/*----------------------------------------------------------------------------*/
#if 0
static DRIVER_ATTR(als,     S_IWUSR | S_IRUGO, stk3x1x_show_als,   NULL);
static DRIVER_ATTR(ps,      S_IWUSR | S_IRUGO, stk3x1x_show_ps,    NULL);
static DRIVER_ATTR(mmi_prx_thd_h_limit,     S_IWUSR | S_IRUGO, stk3x1x_show_mmi_h_thredhold,   stk3x1x_store_mmi_h_thredhold);
static DRIVER_ATTR(mmi_prx_thd_l_limit,     S_IWUSR | S_IRUGO, stk3x1x_show_mmi_l_thredhold,   stk3x1x_store_mmi_l_thredhold);
#else
static DRIVER_ATTR_RO(als);
static DRIVER_ATTR_RO(ps);
static DRIVER_ATTR_RW(mmi_prx_thd_h_limit);
static DRIVER_ATTR_RW(mmi_prx_thd_l_limit);
#endif
/*----------------------------------------------------------------------------*/

static struct driver_attribute *stk3x1x_platform_attr_list[] = {
    &driver_attr_als,
    &driver_attr_ps,
    &driver_attr_mmi_prx_thd_h_limit,
    &driver_attr_mmi_prx_thd_l_limit,
};

static int stk3x1x_create_platform_driver_attr(struct device_driver *driver)
{
	int idx, err = 0;
	int num = (int)(sizeof(stk3x1x_platform_attr_list)/sizeof(stk3x1x_platform_attr_list[0]));
	if (driver == NULL)
	{
		return -EINVAL;
	}

	for(idx = 0; idx < num; idx++)
	{
		if((err = driver_create_file(driver, stk3x1x_platform_attr_list[idx])))
		{
			printk("%s:driver_create_file (%s) = %d\n", __func__, stk3x1x_platform_attr_list[idx]->attr.name, err);
			break;
		}
	}
	return err;
}
#ifndef STK33562_IC_USE
static int stk_ps_val(struct stk3x1x_data *obj_data)
{
	int mode;
	int32_t word_data, lii;
	unsigned char value[4];
	int ret;

	ret = stk3x1x_i2c_read_data(obj_data->client, 0x20, 4, value);
	if(ret < 0)
	{
		printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
		return ret;
	}
	word_data = (value[0]<<8) | value[1];
	word_data += ((value[2]<<8) | value[3]);

	mode = (obj_data->psctrl_reg) & 0x3F;
	if(mode == 0x30)
		lii = 100;
	else if (mode == 0x31)
		lii = 200;
	else if (mode == 0x32)
		lii = 400;
	else if (mode == 0x33)
		lii = 800;
	else
	{
		printk(KERN_ERR "%s: unsupported PS_IT(0x%x)\n", __func__, mode);
		return -1;
	}

	if(word_data > lii)
	{
		printk(KERN_INFO "%s: word_data=%d, lii=%d\n", __func__, word_data, lii);
		return 0xFFFF;
	}
	return 0;
}
#else
static int stk3x3x_ps_prx_val(struct stk3x1x_data *obj_data)
{
#if 0
    uint8_t  ps_invalid_flag, bgir_raw_data[4] = {0};
    int rv = 0,i;
    bool bgir_out_of_range = false;

	rv = stk3x1x_i2c_read_data(obj_data->client, 0xA7, 1, &ps_invalid_flag);
	if(rv < 0)
	{
		printk(KERN_ERR "%s read 0xA7 fail, ret=0x%x\n", __func__, rv);
		return rv;
	}

	rv = stk3x1x_i2c_read_data(obj_data->client, 0x34, 4, &bgir_raw_data[0]);
	if(rv < 0)
	{
		printk(KERN_ERR "%s bgir_raw_data fail, ret=0x%x\n", __func__, rv);
		return rv;
	}

    for (i = 0; i < 4; i++)
    {
        if (*(bgir_raw_data + i) >= STK3X3X_PS_BGIR_THRESHOLD)
        {
            bgir_out_of_range = true;
            printk(KERN_ERR, "stk3x3x_ps_prx_val: BGIR invalid, PS[%d] = 0x%X", i, *(bgir_raw_data + i));
            break;
        }
    }

    if ( ((ps_invalid_flag >> 5) & 0x1) || bgir_out_of_range)
    {
        rv = 0xFFFF;
    }
    return rv;
#else
    u8 ps_invalid_flag;
    u8 ps_off_data_0[4] = {0};
    u8 ps_off_data_1[4] = {0};
    u16 ps_off_data[4] = {0};
    u8 bgir_raw_data[4] = {0};
	int i = 0,ret;
    ret = stk3x1x_i2c_read_data(obj_data->client, 0xA7, 1, &ps_invalid_flag);
    if (ret < 0)
    {
            printk(KERN_ERR "%s fail, err=0x%x", __FUNCTION__, ret);
            return ret;
    }
	ret = stk3x1x_i2c_read_data(obj_data->client, 0x24, 4, ps_off_data_0);
    if (ret < 0)
    {
            printk(KERN_ERR "%s  ps_off_data_0 fail, err=0x%x", __FUNCTION__, ret);
            return ret;
    }
    ret = stk3x1x_i2c_read_data(obj_data->client, 0x28, 4, ps_off_data_1);
    if (ret < 0)
    {
            printk(KERN_ERR "%s ps_off_data_1 fail, err=0x%x", __FUNCTION__, ret);
            return ret;
    }
	ps_off_data[0] = (ps_off_data_0[0] <<8 | ps_off_data_0[1]);
	ps_off_data[1] = (ps_off_data_0[2] <<8 | ps_off_data_0[3]);
	ps_off_data[2] = (ps_off_data_1[0] <<8 | ps_off_data_1[1]);
	ps_off_data[3] = (ps_off_data_1[2] <<8 | ps_off_data_1[3]);

	ret = stk3x1x_i2c_read_data(obj_data->client, 0x34, 4, bgir_raw_data);
    if(ret < 0)
    {
            printk(KERN_ERR "%s fail, err=0x%x", __FUNCTION__, ret);
            return ret;
    }

    if( ((ps_invalid_flag >> 5) & 0x1) || ((bgir_raw_data[0] & 0x7f) >= 15) ||
            ((bgir_raw_data[1] & 0x7f) >= 15) || ((bgir_raw_data[2] & 0x7f) >= 15) || ((bgir_raw_data[3] & 0x7f) >= 15) )//10
    {
	      printk("stk sunlight bgir0=%d,  bgir1=%d, bgir2=%d, bgir3=%d\n",bgir_raw_data[0],bgir_raw_data[1],bgir_raw_data[2],bgir_raw_data[3]);
          return 0xFFFF;
    }

	//printk("stk sunlight bgir(%d,%d,%d,%d) ps_invalid_flag=%d,ps_off_data(%d,%d,%d,%d)\n",bgir_raw_data[0],bgir_raw_data[1],bgir_raw_data[2],bgir_raw_data[3],((ps_invalid_flag >> 5) & 0x1),ps_off_data[0],ps_off_data[1],ps_off_data[2],ps_off_data[3]);
	for(i=0;i<4;i++)
	{
	     if(ps_off_data[i]>1200){//1000
	        printk("stk sunlight pd %d = %d\n",i ,ps_off_data[i]);
	        return 0xFFFF;
	     }
	}
    return 0;
#endif
}
#endif

static int stk3x3x_read_threshold(struct stk3x1x_data *obj_data)
{
	uint16_t h_threshold;
	uint16_t l_threshold;
	uint8_t  ps_threshold[2];
	int rv = 0;
	{
		STK3X3X_LT_N_CT   =                 STK_LOW_PS_LT_N_CT;
		STK3X3X_HT_N_CT   =                 STK_LOW_PS_HT_N_CT;
		STK3X3X_PRX_THD_BOOT_SHIFT   =      STK3X3X_PRX_THD_BOOT_SHIFT_PIO;
		STK3X3X_PRX_THD_BOOT_SHIFT_1 =      STK3X3X_PRX_THD_BOOT_SHIFT_1_PIO;
	}

	rv = stk3x1x_i2c_read_data(obj_data->client, STK_THDH1_PS_REG, 2, &ps_threshold[0]);
	if(rv < 0)
	{
		printk(KERN_ERR "%s read THDH1 threshold fail, ret=0x%x\n", __func__, rv);
        return rv;
	}
	h_threshold = (ps_threshold[0] << 8) | ps_threshold[1];

	rv = stk3x1x_i2c_read_data(obj_data->client, STK_THDL1_PS_REG, 2, &ps_threshold[0]);
	if(rv < 0)
	{
		printk(KERN_ERR "%s read THDL1 threshold fail, ret=0x%x\n", __func__, rv);
        return rv;
	}
	l_threshold = (ps_threshold[0] << 8) | ps_threshold[1];

	printk(KERN_ERR "stk3x3x_read_IC threshold::THD_H = %d, THD_L = %d",
		  (uint16_t)h_threshold, (uint16_t)l_threshold);

	if( ((0 != h_threshold)&&(0 != l_threshold)) && (h_threshold > l_threshold) &&
		    ( ((h_threshold - l_threshold) == (STK3X3X_HT_N_CT - STK3X3X_LT_N_CT))
		      ||((h_threshold - l_threshold) == (STK3X3X_HT_N_CT - STK3X3X_LT_N_CT + STK3X3X_PRX_THD_BOOT_SHIFT-STK3X3X_PRX_THD_BOOT_SHIFT_1)) || ((h_threshold-l_threshold)==(STK_H_HT-STK_H_LT)) )  )
	{
				if ( ((h_threshold - l_threshold)==(STK3X3X_HT_N_CT - STK3X3X_LT_N_CT))
				     || ((h_threshold - l_threshold)==(STK_H_HT-STK_H_LT)) )
				{
					obj_data->ps_thd_h  = h_threshold + STK3X3X_PRX_THD_BOOT_SHIFT;
					obj_data->ps_thd_l = l_threshold + STK3X3X_PRX_THD_BOOT_SHIFT_1;
				}
				else
				{
					obj_data->ps_thd_h  = h_threshold;
					obj_data->ps_thd_l = l_threshold;
				}
				//As an outdoor bright screen threshold
				obj_data->cali_ps_thd_h = obj_data->ps_thd_h;
			    obj_data->cali_ps_thd_l = obj_data->ps_thd_l;

				printk(KERN_ERR "stk3x3x_read_threshold:: IC's PS near(last) = %d, far(last) = %d",
								 (uint16_t)obj_data->ps_thd_h,(uint16_t)obj_data->ps_thd_l);


	}
	else
	{
			printk(KERN_ERR "stk3x3x_read_threshold::default PS near(default) = %d, far(default) = %d",
				(uint16_t)obj_data->ps_thd_h,(uint16_t)obj_data->ps_thd_l);

	}
	return rv;
}

#ifdef STK_TUNE0

static int stk_ps_tune_zero_final(struct stk3x1x_data *obj_data)
{
	int ret;

	obj_data->tune_zero_init_proc = false;
	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_INT_REG, obj_data->int_reg);
	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}

	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_STATE_REG, 0);
	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}

	if(obj_data->data_count == -1)
	{
		printk(KERN_INFO "%s: exceed limit\n", __func__);
		hrtimer_cancel(&obj_data->ps_tune0_timer);

		obj_data->ps_thd_h = obj_data->ps_high_thd_def;
		obj_data->ps_thd_l = obj_data->ps_low_thd_def;
		obj_data->ps_high_thd_boot = obj_data->ps_thd_h;
		obj_data->ps_low_thd_boot = obj_data->ps_thd_l;

		obj_data->ps_thd_h = hw->ps_threshold_high;
		obj_data->ps_thd_l = hw->ps_threshold_low;

		stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
		stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);

		return 0;
	}

	obj_data->psa = obj_data->ps_stat_data[0];
	obj_data->psi = obj_data->ps_stat_data[2];

#ifndef CALI_EVERY_TIME
	obj_data->ps_thd_h = obj_data->ps_stat_data[1] + STK_HT_N_CT;
	obj_data->ps_thd_l = obj_data->ps_stat_data[1] + STK_LT_N_CT;
#else
	/*obj_data->ps_thd_h = obj_data->ps_stat_data[1] + STK_HT_N_CT*2;
	obj_data->ps_thd_l = obj_data->ps_stat_data[1] + STK_LT_N_CT*2;*/
	obj_data->ps_thd_h = obj_data->ps_high_thd_def;
	obj_data->ps_thd_l = obj_data->ps_low_thd_def;
	obj_data->ps_high_thd_boot = obj_data->ps_thd_h;
	obj_data->ps_low_thd_boot = obj_data->ps_thd_l;
#endif

	obj_data->ps_thd_h = hw->ps_threshold_high;
	obj_data->ps_thd_l = hw->ps_threshold_low;

	stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
	stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
	printk(KERN_INFO "%s: set HT=%d,LT=%d\n", __func__, obj_data->ps_thd_h,  obj_data->ps_thd_l);
	hrtimer_cancel(&obj_data->ps_tune0_timer);
	return 0;
}

static int32_t stk_tune_zero_get_obj_data(struct stk3x1x_data *obj_data)
{
	uint32_t ps_adc;
	int ret;
#ifdef STK33562_IC_USE
    ret = stk3x3x_ps_prx_val(obj_data);
#else
	ret = stk_ps_val(obj_data);
#endif
	if(ret == 0xFFFF)
	{
		obj_data->data_count = -1;
		stk_ps_tune_zero_final(obj_data);
		return 0;
	}

	ps_adc = stk3x1x_get_ps_reading(obj_data);
#ifdef STK_DEBUG_PRINTF
	printk(KERN_INFO "%s: ps_adc #%d=%d\n", __func__, obj_data->data_count, ps_adc);
#endif
	if(ps_adc < 0)
		return ps_adc;

	obj_data->ps_stat_data[1]  +=  ps_adc;
	if(ps_adc > obj_data->ps_stat_data[0])
		obj_data->ps_stat_data[0] = ps_adc;
	if(ps_adc < obj_data->ps_stat_data[2])
		obj_data->ps_stat_data[2] = ps_adc;
	obj_data->data_count++;

	if(obj_data->data_count == 5)
	{
		obj_data->ps_stat_data[1]  /= obj_data->data_count;
		obj_data->min_psi = obj_data->ps_stat_data[1];
		printk("%s:min_psi=%d", __func__, obj_data->min_psi);
		stk_ps_tune_zero_final(obj_data);
	}

	return 0;
}

static int stk_ps_tune_zero_init(struct stk3x1x_data *obj_data)
{
	int32_t ret = 0;
	uint8_t w_state_reg;

	obj_data->psi_set = 0;
	obj_data->tune_zero_init_proc = true;
	obj_data->ps_stat_data[0] = 0;
	obj_data->ps_stat_data[2] = 9999;
	obj_data->ps_stat_data[1] = 0;
	obj_data->data_count = 0;

	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_INT_REG, 0);
	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}

	w_state_reg = (STK_STATE_EN_PS_MASK | STK_STATE_EN_WAIT_MASK);
	ret = stk3x1x_i2c_smbus_write_byte_data(obj_data->client, STK_STATE_REG, w_state_reg);
	if (ret < 0)
	{
		printk(KERN_ERR "%s: write i2c error\n", __func__);
		return ret;
	}
	obj_data->state_reg = w_state_reg;
	hrtimer_start(&obj_data->ps_tune0_timer, obj_data->ps_tune0_delay, HRTIMER_MODE_REL);
	return 0;
}
void stk3x3x_ps_prx_tune_zero_threshold_reset(struct stk3x1x_data *obj_data, uint16_t ps_raw_data)
{

    if (ps_raw_data > obj_data->ps_stat_data[0])
        obj_data->ps_stat_data[0] = ps_raw_data;//max

    if (ps_raw_data < obj_data->ps_stat_data[2])
        obj_data->ps_stat_data[2] = ps_raw_data;//min
    return ;
}
void stk3x3x_ps_prx_tune0_zero_tracking_recali(struct stk3x1x_data *obj_data, uint16_t word_data)
{
    uint16_t ct_value;
    if (obj_data->near_far_state_last == 1)//far
    {
        obj_data->ps_stat_data[1] += word_data;
        stk3x3x_ps_prx_tune_zero_threshold_reset(obj_data, word_data);
        obj_data->data_count ++;
        //printk("stk3x3x_ps_prx_tune0_zero_tracking_recali:count = %d, word_data = %d", obj_data->data_count, word_data);

        if (obj_data->data_count == 5)
        {
            obj_data->ps_stat_data[1] /= obj_data->data_count;//average value
            ct_value = obj_data->ps_thd_h - STK_LOW_PS_HT_N_CT;//lzk_debug
            printk("stk3x3x_ps_prx_tune0_zero_tracking_recali:ct_value=%d,word_data=%d,(%d,%d,%d,%d)(thd=%d)\n", ct_value, word_data,obj_data->ps_stat_data[0],obj_data->ps_stat_data[1],obj_data->ps_stat_data[2],obj_data->psi,obj_data->ps_thd_h);

            if ((obj_data->ps_stat_data[1] < ct_value) &&
                ((ct_value - obj_data->ps_stat_data[1]) >= 5) &&
                ((obj_data->ps_stat_data[0] - obj_data->ps_stat_data[2]) <= 10))
            {
                obj_data->ps_thd_h  = obj_data->ps_stat_data[1] + STK_LOW_PS_HT_N_CT;
                obj_data->ps_thd_l  = obj_data->ps_stat_data[1] + STK_LOW_PS_LT_N_CT;
                obj_data->psi       = obj_data->ps_stat_data[1];
			    stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
			    stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
				printk("stk3x3x_ps_set_thd::thd_near = %d far=%d max-min = %d\n", obj_data->ps_thd_h,obj_data->ps_thd_l,(obj_data->ps_stat_data[0] - obj_data->ps_stat_data[2]));
            }
            obj_data->ps_stat_data[2] = 9999;
            obj_data->ps_stat_data[0] = 0;
            obj_data->ps_stat_data[1] = 0;
            obj_data->data_count = 0;
        }
    }
}

/*Begin modified by zhikui.li@tcl.com for, Close the anti-oil algorithm*/
static int stk_ps_tune_zero_func_fae(struct stk3x1x_data *obj_data)
{
	int32_t word_data=0;
	int ret, diff, is_cali = 0;
	unsigned char value[2];
	static int debouse_cnt = 0;
    int reading;
#ifdef CTTRACKING
#else
        static int count = 0;
#endif

#ifndef CALI_EVERY_TIME
	if(obj_data->psi_set || !(obj_data->ps_enabled))
#else
	if(!(obj_data->ps_enabled))
#endif
	{
		return 0;
	}
#ifdef STK_CHK_REG
	if (0xff == stk3x1x_validate_n_handle(obj_data->client)) {
		printk ("%s:register value break\n", __func__);
		return 0;
	}

	// if calibration done,check ESD only
#ifdef CTTRACKING
	if ((obj_data->is_cali) && (obj_data->ps_thd_update== true)) {
		return 0;
	}
#else
	if (obj_data->is_cali) {
/*Begin modified by zhikui.li@tcl.com for,Read rawdata after the screen is off, add print log*/
         count++;
         if((count>=50)&&(obj_data->near_far_state_last==0)){
		     ret = stk3x1x_i2c_read_data(obj_data->client, 0x11, 2, &value[0]);
	         if(ret < 0)
		     {
                  printk(KERN_ERR "%s read 0x11 fail, ret=0x%x\n", __func__, ret);
		     }
		     word_data = (value[0]<<8) | value[1];
		     printk("%s: no oil algorith read word_data=%d\n", __func__, word_data);
             count=0;
         }else if(count>=50){
             count=0;
         }
/*end modified by zhikui.li@tcl.com for,Read rawdata after the screen is off,add print log*/
		return 0;
	}
#endif
#endif

	ret = stk3x1x_get_flag(obj_data);
	if(ret < 0)
		return ret;
	if(!(ret&STK_FLG_PSDR_MASK))
	{
		//printk(KERN_INFO "%s: ps data is not ready yet\n", __func__);
		return 0;
	}

#ifdef STK33562_IC_USE
    ret = stk3x3x_ps_prx_val(obj_data);
/* MODIFIED-BEGIN by zhikui.li,Ignore the first 5 data*/
    if(obj_data->psi_flag < 5){
		stk3x1x_i2c_read_data(obj_data->client, 0x11, 2, &value[0]);
		word_data = (value[0]<<8) | value[1];
		//printk(KERN_ERR "%s Ignore data =%d\n", __func__, word_data);
        obj_data->psi_flag++;
        return 0;
    }
/* MODIFIED-end by zhikui.li,Ignore the first 5 data*/
#else
	ret = stk_ps_val(obj_data);
#endif
	if(ret == 0)
	{
		ret = stk3x1x_i2c_read_data(obj_data->client, 0x11, 2, &value[0]);
		if(ret < 0)
		{
			printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
			return ret;
		}
		word_data = (value[0]<<8) | value[1];
		if(word_data == 0)
		{
			//printk(KERN_ERR "%s: incorrect word data (0)\n", __func__);
			return 0xFFFF;
		}
#ifdef CTTRACKING
		if((word_data>obj_data->psa) && (obj_data->ps_ori_thd == false))
#else
		if(word_data > obj_data->psa)
#endif
		{
			obj_data->psa = word_data;
		}
#ifdef CTTRACKING
		if((word_data < obj_data->psi) && (obj_data->ps_ori_thd == false))
#else
		if(word_data < obj_data->psi)
#endif
		{
			obj_data->psi = word_data;
#ifdef STK33562_IC_USE
            reading = stk_ps_reading_val_reg_stk33562(obj_data);
            if((reading==0) && (abs(obj_data->min_psi - obj_data->psi) >40) && (obj_data->psi>0))
#else
			reading = stk_ps_reading_val_reg(obj_data);
			if( ((reading<5)||(reading<35 && obj_data->psi < obj_data->pdata->mmi_thd_h_limit+200))
			    && (abs(obj_data->min_psi - obj_data->psi) >50) )
#endif
            {
				obj_data->min_psi = min(obj_data->min_psi, obj_data->psi);
			}
#ifdef STK_DEBUG_PRINTF
			printk(KERN_INFO "%s: update psi: psa=%d,psi=%d min_psi=%d\n", __func__, obj_data->psa, obj_data->psi,obj_data->min_psi);
#endif
		}
	}
	else
	{
		//sunshine or other problem  set default thd
		if (obj_data->min_psi != 0xffff)
		{
			obj_data->ps_thd_h = obj_data->min_psi + obj_data->pdata->low_ps_ct_h_offset*3;
			obj_data->ps_thd_l = obj_data->min_psi + obj_data->pdata->low_ps_ct_l_offset*3;
            obj_data->sunshine_state = true;
		}
		else
		{
			ret = stk3x1x_i2c_read_data(obj_data->client, 0x11, 2, &value[0]);
			if(ret < 0)
			{
				printk(KERN_ERR "%s fail, ret=0x%x\n", __func__, ret);
				return ret;
			}
			word_data = (value[0]<<8) | value[1];
#ifndef STK33562_IC_USE
			obj_data->ps_thd_h= 1400;
			obj_data->ps_thd_l= 1300;
#endif
/*Begin:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold,outdoor sunlight uses dynamic calibration*/
			if((obj_data->cali_ps_thd_h != 0xFFFF)&&(obj_data->cali_ps_thd_h < obj_data->ps_thd_h)){
				obj_data->ps_thd_h= obj_data->cali_ps_thd_h + 100;
				obj_data->ps_thd_l= obj_data->cali_ps_thd_l + 100;
			}
			printk(KERN_INFO "%s:min_psi invalid,use old thd %d,%d word_data=%d\n",__func__,obj_data->ps_thd_h,obj_data->ps_thd_l,word_data);
			obj_data->sunshine_state = false;//Mmi is invalid, use dynamic calibration
/*End:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold,outdoor sunlight uses dynamic calibration*/
		}
#ifdef STK_DEBUG_PRINTF
		printk(KERN_INFO "%s: use default thd %d %d word_data=%d mmi=%d\n", __func__, obj_data->ps_thd_h, obj_data->ps_thd_l,word_data,obj_data->min_psi);
#endif
		stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
		stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
		return 0;
	}
#ifdef STK33562_IC_USE
#ifdef CTTRACKING
    if(obj_data->ps_ori_thd == true)
    {
        stk3x3x_ps_prx_tune0_zero_tracking_recali(obj_data, word_data);
#ifdef CTTRACKING
	    mutex_lock(&obj_data->state_lock);
	    if((obj_data->near_far_state_last == 0) && (obj_data->ps_thd_update==false)){ //near  //Oil condition threshold
		    if(word_data > (obj_data->psi + STK_H_PS)) {
                obj_data->ps_thd_h = obj_data->psi + STK_H_HT;
				obj_data->ps_thd_l = obj_data->psi + STK_H_LT;
				obj_data->ps_thd_update = true;
				obj_data->ps_ori_thd = true;
				is_cali = 1;
				debouse_cnt=15;
				stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
				stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
				printk(KERN_ERR "%s: word_data = %d, psi = %d,update HT=%d, LT=%d is_cali=%d\n", __func__,word_data, obj_data->psi, obj_data->ps_thd_h, obj_data->ps_thd_l,is_cali);
		    }
	    }
	    mutex_unlock(&obj_data->state_lock);
#endif
        return 0;
    }else
    {
        obj_data->ps_stat_data[2] = 9999;
        obj_data->ps_stat_data[0] = 0;
	    obj_data->ps_stat_data[1] = 0;
	    obj_data->data_count = 0;
    }
#endif
#endif
	diff = obj_data->psa - obj_data->psi;
    if((obj_data->psi <= 550)&&(diff > obj_data->pdata->low_ps_ct_h_offset))
	{
		if (obj_data->sunshine_state)
		{
#ifndef STK33562_IC_USE
			if(obj_data->min_psi<2000)
#endif
			{
				obj_data->ps_thd_h = obj_data->min_psi + obj_data->pdata->low_ps_ct_h_offset*3;
				obj_data->ps_thd_l = obj_data->min_psi + obj_data->pdata->low_ps_ct_l_offset*3;
				stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
				stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
			}
			return 0;
		}
		obj_data->ps_thd_h = obj_data->psi + obj_data->pdata->low_ps_ct_h_offset;
		obj_data->ps_thd_l = obj_data->psi + obj_data->pdata->low_ps_ct_l_offset;
		is_cali = 1;
	}
	else if(((obj_data->psi <= obj_data->pdata->mmi_thd_h_limit)&&(obj_data->psi > 550))&&(diff > obj_data->pdata->low_ps_ct_h_offset))
	{
		if (obj_data->sunshine_state)
		{
#ifndef STK33562_IC_USE
			if(obj_data->min_psi<2000)
#endif
			{
				obj_data->ps_thd_h = obj_data->min_psi + obj_data->pdata->low_ps_ct_h_offset*3;
				obj_data->ps_thd_l = obj_data->min_psi + obj_data->pdata->low_ps_ct_l_offset*3;
				stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
				stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
			}
			return 0;
		}
		obj_data->ps_thd_h = obj_data->psi + obj_data->pdata->low_ps_ct_h_offset;
		obj_data->ps_thd_l = obj_data->psi + obj_data->pdata->low_ps_ct_l_offset;
	    is_cali = 1;
	}
	else if((obj_data->psi > obj_data->pdata->mmi_thd_h_limit)&&(diff > obj_data->pdata->low_ps_ct_h_offset))
	{
		if (obj_data->sunshine_state)
		{
#ifndef STK33562_IC_USE
			if(obj_data->min_psi<2000)
#endif
			{
				obj_data->ps_thd_h = obj_data->min_psi + obj_data->pdata->low_ps_ct_h_offset*3;
				obj_data->ps_thd_l = obj_data->min_psi + obj_data->pdata->low_ps_ct_l_offset*3;
				stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
				stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
			}
			return 0;
		}
		//obj_data->ps_thd_h = obj_data->psi + min( max(obj_data->psi*2/10,120), 130);
		//obj_data->ps_thd_l = obj_data->psi + min( max(obj_data->psi*2/10,60), 65);
		obj_data->ps_thd_h = (obj_data->psi + obj_data->pdata->low_ps_ct_h_offset + 80);
		obj_data->ps_thd_l = (obj_data->psi + obj_data->pdata->low_ps_ct_l_offset)+ 40;

#ifdef STK_DEBUG_PRINTF
       if((obj_data->old_ps_thd_h != obj_data->ps_thd_h)||(obj_data->old_ps_thd_l !=obj_data->ps_thd_l))
		     printk("%s:min_psi=%d,HT=%d,LT=%d\n", __func__, obj_data->min_psi,obj_data->ps_thd_h, obj_data->ps_thd_l);
#endif
		obj_data->ps_thd_reading=0;
		stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
		stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
	}

#ifdef CTTRACKING
	mutex_lock(&obj_data->state_lock);
	if((obj_data->near_far_state_last == 0) && (obj_data->ps_thd_update==false)){ //near  //Oil condition threshold
		if(word_data > (obj_data->psi + STK_H_PS)) {//&& (word_data >1800)
                obj_data->ps_thd_h = obj_data->psi + STK_H_HT;
				obj_data->ps_thd_l = obj_data->psi + STK_H_LT;
				obj_data->ps_thd_update = true;
				obj_data->ps_ori_thd = true;
				is_cali = 1;
				debouse_cnt=15;
				stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
				stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
				printk(KERN_ERR "%s:ps update word_data = %d, psi = %d\n",__func__, word_data, obj_data->psi);
				printk(KERN_ERR "%s: update HT=%d, LT=%d is_cali=%d\n", __func__, obj_data->ps_thd_h, obj_data->ps_thd_l,is_cali);
		}
	}
	mutex_unlock(&obj_data->state_lock);
#endif
#ifdef STK_DEBUG_PRINTF
		printk("%s:is_cali=%d,word_data=%d  HT=%d, LT=%d\n", __func__,is_cali,word_data, obj_data->ps_thd_h, obj_data->ps_thd_l);
#endif
	if(is_cali)
	{
		obj_data->is_have_good_cali = true;
		obj_data->last_good_psi = obj_data->psi;
		if((obj_data->old_ps_thd_h != obj_data->ps_thd_h) || (obj_data->old_ps_thd_l !=obj_data->ps_thd_l) )
		{
			printk("%s:old(%d:%d) HT=%d, LT=%d\n", __func__,obj_data->old_ps_thd_h,obj_data->old_ps_thd_l, obj_data->ps_thd_h, obj_data->ps_thd_l);
			stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
			stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
		}
#ifdef STK_DEBUG_PRINTF
		printk(KERN_INFO "%s: FAE tune0 psa-psi(%d) > STK_DIFF found debouse_cnt=%d\n", __func__, diff,debouse_cnt);
#endif
		if (debouse_cnt == 15) {
			#ifndef STK_CHK_REG
			// when enable 	STK_CHK_REG, do not stop the timer,
			// use to run ESD thread
			hrtimer_cancel(&obj_data->ps_tune0_timer);
			#endif
			debouse_cnt = 0;
            obj_data->sunshine_state = false;
			obj_data->ps_thd_reading=0;
			obj_data->is_cali = !(!is_cali);
		}
		else {
			debouse_cnt++;
		}
	}

	return 0;
}
/*end modified by zhikui.li@tcl.com for, Close the anti-oil algorithm*/
static void stk_ps_tune0_work_func(struct work_struct *work)
{
	struct stk3x1x_data *obj_data = container_of(work, struct stk3x1x_data, stk_ps_tune0_work);
	if(obj_data->tune_zero_init_proc)
		stk_tune_zero_get_obj_data(obj_data);
	else
		stk_ps_tune_zero_func_fae(obj_data);
	return;
}


static enum hrtimer_restart stk_ps_tune0_timer_func(struct hrtimer *timer)
{
	struct stk3x1x_data *obj_data = container_of(timer, struct stk3x1x_data, ps_tune0_timer);
	queue_work(obj_data->stk_ps_tune0_wq, &obj_data->stk_ps_tune0_work);
	hrtimer_forward_now(&obj_data->ps_tune0_timer, obj_data->ps_tune0_delay);
	return HRTIMER_RESTART;
}
#endif

static enum hrtimer_restart stk_als_timer_func(struct hrtimer *timer)
{
	struct stk3x1x_data *obj_data = container_of(timer, struct stk3x1x_data, als_timer);
	queue_work(obj_data->stk_als_wq, &obj_data->stk_als_work);
	hrtimer_forward_now(&obj_data->als_timer, obj_data->als_poll_delay);
	return HRTIMER_RESTART;

}

static void stk_als_poll_work_func(struct work_struct *work)
{
	struct stk3x1x_data *obj_data = container_of(work, struct stk3x1x_data, stk_als_work);
	int32_t reading, reading_lux,/* als_comperator,*/ flag_reg;
#ifdef ALS_REPORT_LUX_TABLE
	uint8_t index=0;
#endif
//modify(add) by junfeng.zhou.sz for fix i2c timeout error with 661727 begin . 20140501
    	if(obj_data->in_suspend)
    	{
        	printk(KERN_INFO "%s devices in suspend \n",__func__);
        	return ;
    	}
//modify(add) by junfeng.zhou.sz for fix i2c timeout error with 661727 end .
       flag_reg = stk3x1x_get_flag(obj_data);
       if(flag_reg < 0)
           return;
       if(!(flag_reg&STK_FLG_ALSDR_MASK))
           return;
       mutex_lock(&obj_data->io_als_lock);
//modify(add) by zhikui.li Optimize lightsensor response sensitivity  begin
        if(obj_data->alsctrl_reg ==0x39)
             reading = stk3x1x_get_als_reading(obj_data)*2;
        else
	     reading = stk3x1x_get_als_reading(obj_data);
//modify(add) by zhikui.li Optimize lightsensor response sensitivity  end
        if(reading < 0)
        {
              mutex_unlock(&obj_data->io_als_lock);
              return;
        }
    	else if(reading <= STK_ALS_DARKCODE_THD)
    	{
//modify(add) by zhikui.li Optimize lightsensor response sensitivity  begin
            if(dark_code_flag > 5000)
        	{
            		obj_data->als_lux_last = 0;
                        if(dark_code_flag > 0xFFFF)
                              dark_code_flag = 500;
        	}
//modify(add) by zhikui.li Optimize lightsensor response sensitivity  END
        	else if(dark_code_flag %2 == 0)
        	{
            		obj_data->als_lux_last = 1;
            		dark_code_flag++;
        	}
        	else
        	{
            		obj_data->als_lux_last = 0;
            		dark_code_flag++;
        	}
        	mutex_unlock(&obj_data->io_als_lock);
        	return;
    	}
    	else if((dark_code_flag!=0) && (reading < (STK_ALS_DARKCODE_THD+STK_ALS_CHANGE_THD)))
    	{
        	mutex_unlock(&obj_data->io_als_lock);
        	return;
    	}
//    else if((dark_code_flag!=0) && (reading >= (STK_ALS_DARKCODE_THD+STK_ALS_CHANGE_THD)))
    	else
    	{
        	dark_code_flag = 0;
    	}

    	mutex_unlock(&obj_data->io_als_lock);
    /*
	if(obj_data->ir_code)
	{
		obj_data->als_correct_factor = 1000;
		if(reading < STK_IRC_MAX_ALS_CODE && reading > STK_IRC_MIN_ALS_CODE &&
			obj_data->ir_code > STK_IRC_MIN_IR_CODE)
		{
			als_comperator = reading * STK_IRC_ALS_NUMERA / STK_IRC_ALS_DENOMI;
			if(obj_data->ir_code > als_comperator)
				obj_data->als_correct_factor = STK_IRC_ALS_CORREC;
		}
		obj_data->ir_code = 0;
	}
	*/
	obj_data->als_correct_factor = ALS_CORRECT_FACTOR;//add by junfeng.zhou
#if 0
	reading = reading * obj_data->als_correct_factor / 1000;
#else
    obj_data->als_correct_factor = obj_data->als_transmittance;
#endif
	reading_lux = stk_alscode2lux(obj_data, reading);
//modify(add) by zhikui.li,Optimize lightsensor response sensitivity  begin
#ifdef TARGET_BUILD_MMITEST
	{
         if(obj_data->als_lux_last == reading_lux)
         {
             reading_lux--;
         }
#else
	if((abs(obj_data->als_lux_last - reading_lux) >= 2)//STK_ALS_CHANGE_THD
		|| ((reading_lux<40) && (abs(obj_data->als_lux_last - reading_lux) >= 2))  )
	{
#endif
		obj_data->als_lux_last = reading_lux;
#ifdef ALS_REPORT_LUX_TABLE
        for(index = 0;index<10;index++)
        {
            if(obj_data->als_lux_last > lux_adc_table[index] && \
                obj_data->als_lux_last < lux_adc_table[index+1])
                {
                    obj_data->als_lux_last = lux_report_table[index];
                    break;
                }
        }
        if(obj_data->als_lux_last > 3500)
        {
            obj_data->als_lux_last = 2000;
        }
#endif
	if((obj_data->alsctrl_reg ==0x39)&&(obj_data->als_lux_last > 22000))
		obj_data->als_lux_last = 22000;
//modify(add) by zhikui.li, Optimize lightsensor response sensitivity end
	}
#ifdef STK_DEBUG_PRINTF
	printk(KERN_INFO "%s: als input event reading_lux=%d report=%d\n",__func__, reading_lux,obj_data->als_lux_last);
#endif
	return;
}

static void stk_work_func(struct work_struct *work)
{
	uint32_t reading;
#if ((STK_INT_PS_MODE != 0x03) && (STK_INT_PS_MODE != 0x02))
    	int32_t ret;
    	uint8_t disable_flag = 0;
    	uint8_t org_flag_reg;
#endif	/* #if ((STK_INT_PS_MODE != 0x03) && (STK_INT_PS_MODE != 0x02)) */

#ifndef CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD
	uint32_t nLuxIndex;
#endif
	struct stk3x1x_data *obj_data = container_of(work, struct stk3x1x_data, stk_work);
	int32_t near_far_state;
	int32_t als_comperator;
#ifdef ALS_REPORT_LUX_TABLE
	uint8_t index=0;
#endif
#if (STK_INT_PS_MODE	== 0x03)
	near_far_state = gpio_get_value(obj_data->int_pin);
#elif	(STK_INT_PS_MODE	== 0x02)
	near_far_state = !(gpio_get_value(obj_data->int_pin));
#endif

#if ((STK_INT_PS_MODE == 0x03) || (STK_INT_PS_MODE	== 0x02))
	obj_data->ps_distance_last = near_far_state;
	ps_report_interrupt_data(near_far_state);
#ifdef CONFIG_PM_WAKELOCKS
    if (obj_data->ps_wakelock!=NULL)
    	__pm_wakeup_event(obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
	//__pm_wakeup_event(&obj_data->ps_wakelock, jiffies_to_msecs(HZ/2));
#else
	wake_lock_timeout(&obj_data->ps_wakelock, HZ/2);
#endif
	reading = stk3x1x_get_ps_reading(obj_data);
#ifdef STK_DEBUG_PRINTF
	printk(KERN_INFO "%s: ps input event %d cm, ps code = %d\n",__func__, near_far_state, reading);
#endif
#else
	/* mode 0x01 or 0x04 */
	org_flag_reg = stk3x1x_get_flag(obj_data);
	if(org_flag_reg < 0)
		goto err_i2c_rw;

    	if (org_flag_reg & STK_FLG_ALSINT_MASK)
    	{
		disable_flag |= STK_FLG_ALSINT_MASK;
        	reading = stk3x1x_get_als_reading(obj_data);
		if(reading < 0)
		{
			printk(KERN_ERR "%s: stk3x1x_get_als_reading fail, ret=%d\n", __func__, reading);
			goto err_i2c_rw;
		}
#ifndef CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD
        	nLuxIndex = stk_get_lux_interval_index(reading);
        	stk3x1x_set_als_thd_h(obj_data, code_threshold_table[nLuxIndex]);
        	stk3x1x_set_als_thd_l(obj_data, code_threshold_table[nLuxIndex-1]);
#else
        	stk_als_set_new_thd(obj_data, reading);
#endif //CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD

		if(obj_data->ir_code)
		{
			if(reading < STK_IRC_MAX_ALS_CODE && reading > STK_IRC_MIN_ALS_CODE &&
			obj_data->ir_code > STK_IRC_MIN_IR_CODE)
			{
				als_comperator = reading * STK_IRC_ALS_NUMERA / STK_IRC_ALS_DENOMI;
				if(obj_data->ir_code > als_comperator)
					obj_data->als_correct_factor = STK_IRC_ALS_CORREC;
				else
					obj_data->als_correct_factor = 1000;
			}
			printk(KERN_INFO "%s: als=%d, ir=%d, als_correct_factor=%d\n", __func__, reading, obj_data->ir_code, obj_data->als_correct_factor);
			obj_data->ir_code = 0;
		}
        obj_data->als_correct_factor = ALS_CORRECT_FACTOR;
#if 0
	    reading = reading * obj_data->als_correct_factor / 1000;
#else
        obj_data->als_correct_factor = obj_data->als_transmittance;
#endif
#ifdef TARGET_BUILD_MMITEST
        if(obj_data->als_lux_last == reading)
        {
             reading--;
        }
#else
		obj_data->als_lux_last = stk_alscode2lux(obj_data, reading);
#endif
#ifdef ALS_REPORT_LUX_TABLE
        	for(index = 0;index<10;index++)
        	{
		    	if(obj_data->als_lux_last > lux_adc_table[index] && \
		        	obj_data->als_lux_last < lux_adc_table[index+1])
		        {
		            obj_data->als_lux_last = lux_report_table[index];
		            break;
		        }
        	}
        	if(obj_data->als_lux_last > 3500)
        	{
            		obj_data->als_lux_last = 2000;
        	}
#endif

#ifdef STK_DEBUG_PRINTF
		printk(KERN_INFO "%s: als input event %d lux\n",__func__, obj_data->als_lux_last);
#endif
    	}
    	if (org_flag_reg & STK_FLG_PSINT_MASK)
    	{
		disable_flag |= STK_FLG_PSINT_MASK;
		near_far_state = (org_flag_reg & STK_FLG_NF_MASK)?1:0;

		obj_data->ps_distance_last = near_far_state;
		printk(KERN_INFO "%s: ps-distant = %s \n",__func__,obj_data->ps_distance_last? "far" : "near");
#ifdef PRINT_TIME
		print_local_time("stk_report_data_time");
#endif
		ps_report_interrupt_data(near_far_state);
		//wake_lock_timeout(&obj_data->ps_wakelock, HZ/2);
		reading = stk3x1x_get_ps_reading(obj_data);

		printk(KERN_INFO "%s: reading = %d  psi=%d HT=%d, LT=%d\n",__func__,reading,obj_data->psi,obj_data->ps_thd_h, obj_data->ps_thd_l );
#ifdef STK_DEBUG_PRINTF
		printk(KERN_INFO "%s: ps input event=%d, ps code = %d \n",__func__, near_far_state, reading);
#endif
#ifdef CTTRACKING
		mutex_lock(&obj_data->state_lock);
        obj_data->near_far_state_last = near_far_state;
		if(near_far_state == 0){ //near
/*Begin:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold, outdoor sunlight uses dynamic calibration*/
			obj_data->cali_ps_thd_h = obj_data->old_ps_thd_h;
			obj_data->cali_ps_thd_l = obj_data->old_ps_thd_l;
/*END:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold, outdoor sunlight uses dynamic calibration*/
			if((reading > (obj_data->psi + STK_H_PS))) {// && (reading>1800)
				obj_data->ps_thd_h = obj_data->psi + STK_H_HT;
				obj_data->ps_thd_l = obj_data->psi + STK_H_LT;
				stk3x1x_set_ps_thd_h(obj_data, obj_data->ps_thd_h);
				stk3x1x_set_ps_thd_l(obj_data, obj_data->ps_thd_l);
				obj_data->ps_thd_update = true;
				obj_data->ps_ori_thd = true;
				printk(KERN_ERR "%s:ps update reading = %d, psi = %d\n",__func__, reading, obj_data->psi);
				printk(KERN_ERR "%s: update HT=%d, LT=%d\n", __func__, obj_data->ps_thd_h, obj_data->ps_thd_l);
			}
		}else{//far
			if(obj_data->ps_thd_update) {
				//printk(KERN_ERR "%s: clear psa psi \n", __func__);
				//obj_data->psa = 0x0;//lzk_debug
				//obj_data->psi = 0xFFFF;
				obj_data->ps_thd_update = false;
			}
		}
		mutex_unlock(&obj_data->state_lock);
#else
/*Begin:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold,outdoor sunlight uses dynamic calibration*/
		if(near_far_state == 0){ //near
			obj_data->cali_ps_thd_h = obj_data->old_ps_thd_h;
			obj_data->cali_ps_thd_l = obj_data->old_ps_thd_l;
        }
        obj_data->near_far_state_last = near_far_state;
/*END:Modify by TCTSZ zhikui.li@tcl.com, changes MMI fail threshold,outdoor sunlight uses dynamic calibration*/
#endif

	}

	if(disable_flag)
	{
		ret = stk3x1x_set_flag(obj_data, org_flag_reg, disable_flag);
		if(ret < 0)
			goto err_i2c_rw;
	}
#endif
	usleep_range(1000, 2000);
	//msleep(1);
    	enable_irq(obj_data->irq);
	return;

err_i2c_rw:
	msleep(30);
	enable_irq(obj_data->irq);
	return;
}

static irqreturn_t stk_oss_irq_handler(int irq, void *data)
{
	struct stk3x1x_data *pData = data;
#ifdef CONFIG_PM_WAKELOCKS
    if (pData->ps_wakelock!=NULL)
    	__pm_wakeup_event(pData->ps_wakelock, jiffies_to_msecs(HZ/2));
	//__pm_wakeup_event(&pData->ps_wakelock, jiffies_to_msecs(HZ/2));
#else
	wake_lock_timeout(&pData->ps_wakelock, HZ/2);
#endif
	disable_irq_nosync(irq);
	queue_work(pData->stk_wq,&pData->stk_work);
	return IRQ_HANDLED;
}

static int32_t stk3x1x_init_all_setting(struct i2c_client *client, struct stk3x1x_platform_data *plat_data)
{
	int32_t ret;
	struct stk3x1x_data *obj_data = i2c_get_clientdata(client);

	stk3x1x_proc_plat_data(obj_data, plat_data);

    ret = stk3x1x_check_pid(obj_data);
	if(ret < 0)
		return ret;

	ret = stk3x1x_software_reset(obj_data);
	if(ret < 0)
		return ret;


	ret = stk3x1x_init_all_reg(obj_data);
	if(ret < 0)
		return ret;

	obj_data->als_enabled = false;
	obj_data->ps_enabled = false;
	obj_data->re_enable_als = false;
	obj_data->re_enable_ps = false;
	obj_data->ir_code = 0;
	obj_data->als_correct_factor = obj_data->als_correct_factor;//add by junfeng.zhou
	obj_data->first_boot = true;
#ifndef CONFIG_STK_PS_ALS_USE_CHANGE_THRESHOLD
	stk_init_code_threshold_table(obj_data);
#endif
#ifdef STK_TUNE0
	stk_ps_tune_zero_init(obj_data);
#endif
#ifdef STK_ALS_FIR
	memset(&obj_data->fir, 0x00, sizeof(obj_data->fir));
	atomic_set(&obj_data->firlength, STK_FIR_LEN);
#endif
	atomic_set(&obj_data->recv_reg, 0);

    return 0;
}
// [Feature]Add-BEGIN by TCTSZ. ning.wei@tcl.com, 2015/08/03, for add stk p/l sensor pull up&down ALM389755
/*static int stk3x1x_pinctrl_init(struct stk3x1x_data *obj_data)
{
	struct i2c_client *client = obj_data->client;

	obj_data->pinctrl = devm_pinctrl_get(&client->dev);
	if (IS_ERR_OR_NULL(obj_data->pinctrl)) {
		dev_err(&client->dev, "Failed to get pinctrl\n");
		return PTR_ERR(obj_data->pinctrl);
	}

	obj_data->pin_default =
		pinctrl_lookup_state(obj_data->pinctrl, "default");
	if (IS_ERR_OR_NULL(obj_data->pin_default)) {
		dev_err(&client->dev, "Failed to look up default state\n");
		return PTR_ERR(obj_data->pin_default);
	}

	obj_data->pin_sleep =
		pinctrl_lookup_state(obj_data->pinctrl, "sleep");
	if (IS_ERR_OR_NULL(obj_data->pin_sleep)) {
		dev_err(&client->dev, "Failed to look up sleep state\n");
		return PTR_ERR(obj_data->pin_sleep);
	}

	return 0;
}*/
// [Feature]-Add-END by TCTSZ. ning.wei@tcl.com, 2015/08/03, for add stk p/l sensor pull up&down ALM389755

const static struct of_device_id stk_of_match[] = {
        { .compatible = "mediatek,als_eint", },
        {},
};

static int stk3x1x_setup_irq(struct i2c_client *client)
{
	 int irq=0, err = -EIO;
	 struct stk3x1x_data *obj_data = i2c_get_clientdata(client);
#if 0
	 struct device_node *node =  client->dev.of_node;
#else
     //struct device_node *node = of_find_compatible_node(NULL, NULL, "mediatek,als_eint");
      struct device_node *node = NULL;
      node = of_find_matching_node(node, stk_of_match);
#endif

	  if (node) {
		  irq = irq_of_parse_and_map(node, 0);
	  }else{
		  printk("stk3x1x_setup_irq not find  eint device node!.");
      }
#ifdef STK_DEBUG_PRINTF
	printk(KERN_INFO "%s: int pin #=%d, irq=%d\n",__func__, obj_data->int_pin, irq);
#endif
	if (irq <= 0)
	{
		printk(KERN_ERR "irq number is not specified, irq # = %d, int pin=%d\n",irq, obj_data->int_pin);
		return irq;
	}
	obj_data->irq = irq;
	/*err = gpio_request(obj_data->int_pin,"stk-int");
	if(err < 0)
	{
		printk(KERN_ERR "%s: gpio_request, err=%d\n", __func__, err);
		return err;
	}
	err = gpio_direction_input(obj_data->int_pin);
	if(err < 0)
	{
		printk(KERN_ERR "%s: gpio_direction_input, err=%d\n", __func__, err);
		return err;
	}*/

// [Feature]Add-BEGIN by TCTSZ. ning.wei@tcl.com, 2015/08/03, for add stk p/l sensor pull up&down ALM389755
	/*err = stk3x1x_pinctrl_init(obj_data);
	if (err) {
		printk(KERN_ERR "%s: Can't initialize pinctrl, err=%d \n", __func__, err);
		return err;

	}
	err = pinctrl_select_state(obj_data->pinctrl, obj_data->pin_default);
	if (err) {
		printk(KERN_ERR "%s: Can't select pinctrl default state, err=%d \n", __func__, err);
		return err;
	}*/
// [Feature]-Add-END by TCTSZ. ning.wei@tcl.com, 2015/08/03, for add stk p/l sensor pull up&down ALM389755

#if ((STK_INT_PS_MODE == 0x03) || (STK_INT_PS_MODE	== 0x02))
	err = request_any_context_irq(irq, stk_oss_irq_handler, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, DEVICE_NAME, obj_data);
#else
	err = request_any_context_irq(irq, stk_oss_irq_handler, IRQF_TRIGGER_LOW, DEVICE_NAME, obj_data);
#endif
	if (err < 0)
	{
		printk(KERN_WARNING "%s: request_any_context_irq(%d) failed for (%d)\n", __func__, irq, err);
		goto err_request_any_context_irq;
	}
	disable_irq(irq);

	return 0;
err_request_any_context_irq:
	//gpio_free(obj_data->int_pin);
	return err;
}



static int stk3x1x_suspend(struct device *dev)
{
	struct stk3x1x_data *obj_data = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(dev);
	int err;

	mutex_lock(&obj_data->io_lock);

	printk("%s: start \n", __func__);
#ifdef STK_CHK_REG
	if (obj_data->ps_enabled || obj_data->als_enabled) {
		err = stk3x1x_validate_n_handle(obj_data->client);
		if(err < 0)
		{
			printk(KERN_ERR "stk3x1x_validate_n_handle fail: %d\n", err);
		}
		else if (err == 0xFF)
		{
			if(obj_data->ps_enabled)
				stk3x1x_enable_ps(obj_data, 1, 0);
		}
	}
#endif /* #ifdef STK_CHK_REG */

#ifdef STK_TUNE0
		hrtimer_cancel(&obj_data->ps_tune0_timer);
		cancel_work_sync(&obj_data->stk_ps_tune0_work);
#endif

	if(obj_data->als_enabled)
	{
		printk(KERN_INFO "%s: Enable ALS : 0\n", __func__);
		stk3x1x_enable_als(obj_data, 0);
		obj_data->re_enable_als = true;
	}

	if(obj_data->ps_enabled)
	{
		if(device_may_wakeup(&client->dev))
		{
			err = enable_irq_wake(obj_data->irq);
			if (err)
				printk(KERN_WARNING "%s: set_irq_wake(%d) failed, err=(%d)\n", __func__, obj_data->irq, err);
		}
		else
		{
			printk(KERN_ERR "%s: not support wakeup source\n", __func__);
		}
	}

	mutex_unlock(&obj_data->io_lock);

    	printk(KERN_INFO "%s end\n", __func__);
	//obj_data->als_enable_pre = curr_als_enable;
	//stk3x1x_enable_als(obj_data, 0);
	//obj_data->ps_enable_pre = curr_ps_enable;
	//stk3x1x_enable_ps(obj_data, 0,1);
	obj_data->in_suspend = 1;
	//modify(add) by junfeng.zhou.sz for fix i2c timeout error with 661727 end .
	return 0;
}

static int stk3x1x_resume(struct device *dev)
{
	struct stk3x1x_data *obj_data = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(dev);
	int err;

	printk(KERN_INFO "%s\n", __func__);

	mutex_lock(&obj_data->io_lock);

#ifdef STK_CHK_REG

	if (obj_data->ps_enabled || obj_data->als_enabled) {
		err = stk3x1x_validate_n_handle(obj_data->client);
		if(err < 0)
		{
			printk(KERN_ERR "stk3x1x_validate_n_handle fail: %d\n", err);
		}
		else if (err == 0xFF)
		{
			if(obj_data->ps_enabled)
			stk3x1x_enable_ps(obj_data, 1, 0);
		}


		hrtimer_start(&obj_data->ps_tune0_timer, obj_data->ps_tune0_delay, HRTIMER_MODE_REL);

	}
#endif /* #ifdef STK_CHK_REG */

	if(obj_data->re_enable_als)
	{
		printk(KERN_INFO "%s: Enable ALS : 1\n", __func__);
		stk3x1x_enable_als(obj_data, 1);
		obj_data->re_enable_als = false;
	}

	if(obj_data->ps_enabled)
	{
		if(device_may_wakeup(&client->dev))
		{
			err = disable_irq_wake(obj_data->irq);
			if (err)
				printk(KERN_WARNING "%s: disable_irq_wake(%d) failed, err=(%d)\n", __func__, obj_data->irq, err);
		}
	}

	mutex_unlock(&obj_data->io_lock);

    	printk(KERN_INFO "%s\n", __func__);
    	obj_data->in_suspend = 0;
    	//stk3x1x_enable_als(obj_data, obj_data->als_enable_pre);
    	//stk3x1x_enable_ps(obj_data, obj_data->ps_enable_pre,1);
    	//modify(add) by junfeng.zhou.sz for fix i2c timeout error with 661727 end .
	return 0;
}

static const struct dev_pm_ops stk3x1x_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(stk3x1x_suspend, stk3x1x_resume)
};

//modify(add) by junfeng.zhou.sz for add power supply begin . 20140214
#ifdef POWER_REGULATOR
static int stk3x1x_power_ctl(struct stk3x1x_data *data, bool on)
{
	int ret = 0;

	if (!on && data->power_enabled) {
		ret = regulator_disable(data->vdd);
		if (ret) {
			dev_err(&data->client->dev,
				"Regulator vdd disable failed ret=%d\n", ret);
			return ret;
		}

		ret = regulator_disable(data->vio);
		if (ret) {
			dev_err(&data->client->dev,
				"Regulator vio disable failed ret=%d\n", ret);
			//retregulator_enable(data->vdd);
			return ret;
		}
		data->power_enabled = on;
		dev_dbg(&data->client->dev, "stk3x1x_power_ctl on=%d\n",
				on);
	} else if (on && !data->power_enabled) {

		ret = regulator_enable(data->vdd);
		if (ret) {
			dev_err(&data->client->dev,
				"Regulator vdd enable failed ret=%d\n", ret);
			return ret;
		}

		ret = regulator_enable(data->vio);
		if (ret) {
			dev_err(&data->client->dev,
				"Regulator vio enable failed ret=%d\n", ret);
			regulator_disable(data->vdd);
			return ret;
		}
		data->power_enabled = on;
		dev_dbg(&data->client->dev, "stk3x1x_power_ctl on=%d\n",
				on);
	} else {
		dev_warn(&data->client->dev,
				"Power on=%d. enabled=%d\n",
				on, data->power_enabled);
	}

	return ret;
}

static int stk3x1x_power_init(struct stk3x1x_data *data, bool on)
{
	int ret;

	if (!on) {
		if (regulator_count_voltages(data->vdd) > 0)
			regulator_set_voltage(data->vdd,
					0, STK3X1X_VDD_MAX_UV);

		regulator_put(data->vdd);

		if (regulator_count_voltages(data->vio) > 0)
			regulator_set_voltage(data->vio,
					0, STK3X1X_VIO_MAX_UV);

		regulator_put(data->vio);
	} else {
		data->vdd = regulator_get(&data->client->dev, "vdd");
		if (IS_ERR(data->vdd)) {
			ret = PTR_ERR(data->vdd);
			dev_err(&data->client->dev,
				"Regulator get failed vdd ret=%d\n", ret);
			return ret;
		}

		if (regulator_count_voltages(data->vdd) > 0) {
			ret = regulator_set_voltage(data->vdd,
					STK3X1X_VDD_MIN_UV,
					STK3X1X_VDD_MAX_UV);
			if (ret) {
				dev_err(&data->client->dev,
					"Regulator set failed vdd ret=%d\n",
					ret);
				goto reg_vdd_put;
			}
		}

		data->vio = regulator_get(&data->client->dev, "vio");
		if (IS_ERR(data->vio)) {
			ret = PTR_ERR(data->vio);
			dev_err(&data->client->dev,
				"Regulator get failed vio ret=%d\n", ret);
			goto reg_vdd_set;
		}

		if (regulator_count_voltages(data->vio) > 0) {
			ret = regulator_set_voltage(data->vio,
					STK3X1X_VIO_MIN_UV,
					STK3X1X_VIO_MAX_UV);
			if (ret) {
				dev_err(&data->client->dev,
				"Regulator set failed vio ret=%d\n", ret);
				goto reg_vio_put;
			}
		}
	}

	return 0;

reg_vio_put:
	regulator_put(data->vio);
reg_vdd_set:
	if (regulator_count_voltages(data->vdd) > 0)
		regulator_set_voltage(data->vdd, 0, STK3X1X_VDD_MAX_UV);
reg_vdd_put:
	regulator_put(data->vdd);
	return ret;
}

static int stk3x1x_device_ctl(struct stk3x1x_data *obj_data, bool enable)
{
	int ret;
	struct device *dev = &obj_data->client->dev;

	if (enable && !obj_data->power_enabled) {
		ret = stk3x1x_power_ctl(obj_data, true);
		if (ret) {
			dev_err(dev, "Failed to enable device power\n");
			goto err_exit;
		}
/*		ret = stk3x1x_init_all_setting(obj_data->client, obj_data->pdata);
		if (ret < 0) {
			stk3x1x_power_ctl(obj_data, false);
			dev_err(dev, "Failed to re-init device setting\n");
			goto err_exit;
		}*/
	} else if (!enable && obj_data->power_enabled) {
		if (!obj_data->als_enabled && !obj_data->ps_enabled) {
			ret = stk3x1x_power_ctl(obj_data, false);
			if (ret) {
				dev_err(dev, "Failed to disable device power\n");
				goto err_exit;
			}
		} else {
			dev_dbg(dev, "device control: als_enabled=%d, ps_enabled=%d\n",
				obj_data->als_enabled, obj_data->ps_enabled);
		}
	} else {
		dev_dbg(dev, "device control: enable=%d, power_enabled=%d\n",
			enable, obj_data->power_enabled);
	}
	return 0;

err_exit:
	return ret;
}
#endif
//modify(add) by junfeng.zhou.sz for add power supply end .

static int stk3x1x_set_wq(struct stk3x1x_data *obj_data)
{
	obj_data->stk_als_wq = create_singlethread_workqueue("stk_als_wq");
	INIT_WORK(&obj_data->stk_als_work, stk_als_poll_work_func);
	hrtimer_init(&obj_data->als_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
//modify(add) by zhikui.li Optimize lightsensor response sensitivity  begin
        if(obj_data->alsctrl_reg ==0x39)
	     obj_data->als_poll_delay = ns_to_ktime(100 * NSEC_PER_MSEC);
        else
	     obj_data->als_poll_delay = ns_to_ktime(110 * NSEC_PER_MSEC);
//modify(add) by zhikui.li Optimize lightsensor response sensitivity  end
	obj_data->als_timer.function = stk_als_timer_func;


#ifdef STK_TUNE0
	obj_data->stk_ps_tune0_wq = create_singlethread_workqueue("stk_ps_tune0_wq");
	INIT_WORK(&obj_data->stk_ps_tune0_work, stk_ps_tune0_work_func);
	hrtimer_init(&obj_data->ps_tune0_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	obj_data->ps_tune0_delay = ns_to_ktime(60 * NSEC_PER_MSEC);
	obj_data->ps_tune0_timer.function = stk_ps_tune0_timer_func;
#endif

	obj_data->stk_wq = create_singlethread_workqueue("stk_wq");
	INIT_WORK(&obj_data->stk_work, stk_work_func);

	return 0;
}

static int stk3x1x_remove_wq(struct stk3x1x_data *obj_data)
{
	cancel_work_sync(&obj_data->stk_work);
	destroy_workqueue(obj_data->stk_wq);

#ifdef STK_TUNE0
	hrtimer_try_to_cancel(&obj_data->ps_tune0_timer);
	cancel_work_sync(&obj_data->stk_ps_tune0_work);
	destroy_workqueue(obj_data->stk_ps_tune0_wq);
#endif
	hrtimer_try_to_cancel(&obj_data->als_timer);
	cancel_work_sync(&obj_data->stk_als_work);
	destroy_workqueue(obj_data->stk_als_wq);

	return 0;
}

#ifdef CONFIG_OF
static int stk3x1x_parse_dt(struct device_node *np,
			struct stk3x1x_platform_data *pdata)
{
	int rc;
	u32 temp_val;

	/*pdata->int_pin = of_get_named_gpio_flags(np, "stk,irq-gpio",
				0, &pdata->int_flags);
	if (pdata->int_pin < 0) {
		dev_err(dev, "Unable to read irq-gpio\n");
		return pdata->int_pin;
	}*/

	rc = of_property_read_u32(np, "stk,transmittance", &temp_val);
	if (!rc)
//modify(add) by zhikui.li,Optimize light sensor mmi display begin
#if 0//def TARGET_BUILD_MMITEST
		pdata->transmittance = temp_val-20;
#else
		pdata->transmittance = temp_val;
#endif
//modify(add) by zhikui.li, Optimize light sensor mmi display end
	else {
		printk("%s:Unable to read transmittance\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,state-reg", &temp_val);
	if (!rc)
		pdata->state_reg = temp_val;
	else {
		printk("%s:Unable to read state-reg\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,psctrl-reg", &temp_val);
	if (!rc)
		pdata->psctrl_reg = (u8)temp_val;
	else {
		printk("%s:Unable to read psctrl-reg\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,alsctrl-reg", &temp_val);
	if (!rc)
		pdata->alsctrl_reg = (u8)temp_val;
	else {
		printk("%s:Unable to read alsctrl-reg\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,ledctrl-reg", &temp_val);
	if (!rc)
		pdata->ledctrl_reg = (u8)temp_val;
	else {
		printk("%s:Unable to read ledctrl-reg\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,wait-reg", &temp_val);
	if (!rc)
		pdata->wait_reg = (u8)temp_val;
	else {
		printk("%s:Unable to read wait-reg\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,ps-thdh", &temp_val);
	if (!rc)
		pdata->ps_thd_h = (u16)temp_val;
	else {
		printk("%s:Unable to read ps-thdh\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,ps-thdl", &temp_val);
	if (!rc)
		pdata->ps_thd_l = (u16)temp_val;
	else {
		printk("%s:Unable to read ps-thdl\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,mmi-thdh-limit", &temp_val);
	if (!rc)
		pdata->mmi_thd_h_limit = (u16)temp_val;
	else {
		printk("%s:Unable to read ps-thdl\n", __func__);
		return rc;
	}

	rc = of_property_read_u32(np, "stk,mmi-thdl-limit", &temp_val);
	if (!rc)
		pdata->mmi_thd_l_limit = (u16)temp_val;
	else {
		printk("%s:Unable to read ps-thdl\n", __func__);
		return rc;
	}

	pdata->use_fir = of_property_read_bool(np, "stk,use-fir");

	return 0;
}
#else
static int stk3x1x_parse_dt(struct device_node *np,
			struct stk3x1x_platform_data *pdata)
{
	return -ENODEV;
}
#endif /* !CONFIG_OF */

///////////////////////////////////////////////////////////////
#define EPL_NEAR 0
#define EPL_FAR  1
int stk_detect_pocket(void)
{
	struct stk3x1x_data *obj_data = stk3x1x_obj;
	int ret = -1;
	uint8_t old_mode = 0;
	uint8_t w_state_reg = 0;
	int ps = 0;


	printk("%s", __func__);

	if (stk3x1x_obj)
	{
		mutex_lock(&obj_data->enable_mutex);
		if (obj_data->ps_enabled)
		{
			ret = obj_data->ps_distance_last;
			printk("stk sensor pocket mode ps near far = %d\n", ret);
		}
		else
		{
			#ifdef POWER_REGULATOR
			stk3x1x_device_ctl(obj_data, 1);
			#endif

			#ifdef STK_CHK_REG
			stk3x1x_validate_n_handle(obj_data->client);
			#endif
			old_mode =stk3x1x_get_state(obj_data);
			w_state_reg &= ~(STK_STATE_EN_PS_MASK | STK_STATE_EN_WAIT_MASK | STK_STATE_EN_AK_MASK);
			w_state_reg |= STK_STATE_EN_PS_MASK;
			stk3x1x_set_state(obj_data, w_state_reg);

			mdelay(30);

			ps = stk3x1x_get_ps_reading(obj_data);
			stk3x1x_set_state(obj_data, old_mode);

			#ifdef POWER_REGULATOR
			stk3x1x_device_ctl(obj_data, 0);
			#endif


			if ( ps > (obj_data->psi+ min( max(obj_data->min_psi*3/10,400), 600)) )
			{
				ret = 0;
			}
			else
			{

				ret = 1;
			}

			printk("pocket mode ps value = %d,old_mod=0x%x\n", ps, old_mode);
		}
		mutex_unlock(&obj_data->enable_mutex);

		return ret;
	}

	return ret;
}

EXPORT_SYMBOL_GPL(stk_detect_pocket);
////////////////////////////////////////////////////////////////////


static int als_open_report_data(int open)
{
	//should queuq work to report event if  is_report_input_direct=true
	return 0;
}

// if use  this typ of enable , Gsensor only enabled but not report inputEvent to HAL

static int als_enable_nodata(int en)
{
	int res = 0;

    	printk("stk3x1x_obj als enable value = %d\n", en);


	if(!stk3x1x_obj)
	{
		printk("stk3x1x_obj is null!!\n");
		return -1;
	}
	res = stk3x1x_enable_als(stk3x1x_obj, en);

	if(res){
		printk("als_enable_nodata is failed!!\n");
		return -1;
	}
	return 0;
}

static int als_set_delay(u64 ns)
{
	return 0;
}
static int als_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{

	return als_set_delay(samplingPeriodNs);
}

static int als_flush(void)
{
	return als_flush_report();
}

static int als_get_data(int* value, int* status)
{
	int err = 0;
	if(!stk3x1x_obj)
	{
		printk("stk3x1x_obj is null!!\n");
		return -1;
	}
	*value = stk3x1x_obj->als_lux_last;
	*status = SENSOR_STATUS_ACCURACY_MEDIUM;
	return err;
}

static int ps_open_report_data(int open)
{
	//should queuq work to report event if  is_report_input_direct=true
	return 0;
}

// if use  this typ of enable , Gsensor only enabled but not report inputEvent to HAL

static int ps_enable_nodata(int en)
{
	int res = 0;

    	printk("stk3x1x_obj ps enable value = %d\n", en);


	if(!stk3x1x_obj)
	{
		printk("stk3x1x_obj is null!!\n");
		return -1;
	}
	res = stk3x1x_enable_ps(stk3x1x_obj, en, 1);

	if(res){
		printk("ps_enable_nodata is failed!!\n");
		return -1;
	}
	return 0;

}

static int ps_set_delay(u64 ns)
{
	return 0;
}

static int ps_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	return ps_set_delay(samplingPeriodNs);
}

static int ps_flush(void)
{
	return ps_flush_report();
}

static int ps_get_data(int* value, int* status)
{

    	if(!stk3x1x_obj)
	{
		printk("stk3x1x_obj is null!!\n");
		return -1;
	}

        *value = stk3x1x_obj->ps_distance_last;
        *status = SENSOR_STATUS_ACCURACY_MEDIUM;


	return 0;
}

static int stk3x1X_setup_mtk_layer_interface(struct stk3x1x_data *obj_data)
{
	int err = -1;

	obj_data->als_ctl.is_use_common_factory =false;
	obj_data->ps_ctl.is_use_common_factory = false;

	obj_data->als_ctl.open_report_data= als_open_report_data;
	obj_data->als_ctl.enable_nodata = als_enable_nodata;
	obj_data->als_ctl.set_delay  = als_set_delay;
	obj_data->als_ctl.flush = als_flush;
	obj_data->als_ctl.batch = als_batch;
	obj_data->als_ctl.is_report_input_direct = false;
	obj_data->als_ctl.is_support_batch = false;

	err = als_register_control_path(&obj_data->als_ctl);
	if (err)
	{
		goto out;
	}

	obj_data->als_data.get_data = als_get_data;
	obj_data->als_data.vender_div = 100;
	err = als_register_data_path(&obj_data->als_data);
	if (err)
	{
		goto out;
	}


	obj_data->ps_ctl.open_report_data= ps_open_report_data;
	obj_data->ps_ctl.enable_nodata = ps_enable_nodata;
	obj_data->ps_ctl.set_delay  = ps_set_delay;
	obj_data->ps_ctl.batch = ps_batch;
	obj_data->ps_ctl.flush = ps_flush;
	obj_data->ps_ctl.is_report_input_direct = true;
	obj_data->ps_ctl.is_support_batch = false;


	err = ps_register_control_path(&obj_data->ps_ctl);
	if(err)
	{
		goto out;
	}

	obj_data->ps_data.get_data = ps_get_data;
	obj_data->ps_data.vender_div = 100;
	err = ps_register_data_path(&obj_data->ps_data);
	if(err)
	{
		goto out;
	}

	/*err = batch_register_support_info(ID_LIGHT,obj_data->als_ctl.is_support_batch, 100, 0);
	if(err)
	{
		printk("register light batch support err = %d\n", err);
		goto out;
	}

	err = batch_register_support_info(ID_PROXIMITY,obj_data->ps_ctl.is_support_batch, 100, 0);
	if(err)
	{
		printk("register proximity batch support err = %d\n", err);
		goto out;
	}*/


out:
	return err;
}

static int stk3x1x_probe(struct i2c_client *client,
                        const struct i2c_device_id *id)
{
    	int err = -ENODEV;
    	struct stk3x1x_data *obj_data = NULL;
	struct stk3x1x_platform_data *plat_data = NULL;
#if 0
	struct device_node *node = NULL;
	node = of_find_compatible_node(NULL, NULL, "mediatek,stk3x1x");
        if(node != NULL)
        {
             err = get_alsps_dts_func(node, hw);
	     if (err < 0) {
		    printk(KERN_ERR"get alsps info from dts failed\n");
		    return -ENOMEM;
	     }
        }
        else
        {
            printk(KERN_ERR"get alsps dts node fail\n");
            err = -EFAULT;
		return -ENOMEM;
        }
#else
	err = get_alsps_dts_func(client->dev.of_node, hw);
	if(err)
	{
		printk(KERN_ERR "%s: get dts info fail\n", __func__);
		return -ENOMEM;
	}
#endif
    	printk(KERN_INFO "%s: driver version = %s\n", __func__, DRIVER_VERSION);

    	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
    	{
        	printk(KERN_ERR "%s: No Support for I2C_FUNC_I2C\n", __func__);
        	return -ENODEV;
    	}

	obj_data = kzalloc(sizeof(struct stk3x1x_data),GFP_KERNEL);
	if(!obj_data)
	{
		printk(KERN_ERR "%s: failed to allocate stk3x1x_data\n", __func__);
		return -ENOMEM;
	}

	client->addr = (client->addr & I2C_MASK_FLAG )&(~ I2C_DMA_FLAG);
	stk3x1x_obj = obj_data;
	obj_data->client = client;
	i2c_set_clientdata(client,obj_data);
	mutex_init(&obj_data->io_lock);
    	mutex_init(&obj_data->io_als_lock);
    	mutex_init(&stk_i2c_lock);
	mutex_init(&obj_data->enable_mutex);
#ifdef CTTRACKING
	mutex_init(&obj_data->state_lock);
#endif
#ifdef CONFIG_PM_WAKELOCKS
    //wakeup_source_init(&obj_data->ps_wakelock, "stk_input_wakelock");
    obj_data->ps_wakelock = wakeup_source_register(NULL, "stk_input_wakelock");
    if (obj_data->ps_wakelock == NULL) {
         printk(KERN_ERR"fail to request stk_input_wakelock\n");
    }
#else
	wake_lock_init(&obj_data->ps_wakelock,WAKE_LOCK_SUSPEND, "stk_input_wakelock");
#endif
#if 0
	node = of_find_compatible_node(NULL, NULL, "mediatek,stk3x1x");
	if (node) {
		plat_data = devm_kzalloc(&client->dev,
			sizeof(struct stk3x1x_platform_data), GFP_KERNEL);
		if (!plat_data) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			err = -ENOMEM;
			goto err_malloc_platform_data;
		}

		err = stk3x1x_parse_dt(node, plat_data);
		dev_err(&client->dev,
			"%s: stk3x1x_parse_dt ret=%d\n", __func__, err);
		if (err)
			goto err_parse_dt;
	}
#else
	if (client->dev.of_node) {
		plat_data = devm_kzalloc(&client->dev,
			sizeof(struct stk3x1x_platform_data), GFP_KERNEL);
		if (!plat_data) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			goto err_malloc_platform_data;
		}
		err = stk3x1x_parse_dt(client->dev.of_node, plat_data);
		dev_err(&client->dev,
			"%s: stk3x1x_parse_dt ret=%d\n", __func__, err);
		if (err)
			goto err_parse_dt;
	}
#endif
	else {
		plat_data = client->dev.platform_data;
	}

	if (!plat_data) {
		dev_err(&client->dev,
			"%s: no stk3x1x platform data!\n", __func__);
		goto err_malloc_platform_data;
	}

	obj_data->als_transmittance = plat_data->transmittance;
	obj_data->int_pin = plat_data->int_pin;
	obj_data->use_fir = plat_data->use_fir;
	obj_data->ps_low_thd_def = plat_data->ps_thd_l;
	obj_data->ps_high_thd_def = plat_data->ps_thd_h;
	obj_data->pdata = plat_data;
    	obj_data->in_suspend = 0;
	obj_data->is_have_good_cali = false;
	obj_data->is_cali = false;
	obj_data->last_good_psi = 0xffff;
	obj_data->min_psi = 0xffff;
	plat_data->low_ps_ct_h_offset = STK_LOW_PS_HT_N_CT;
	plat_data->low_ps_ct_l_offset = STK_LOW_PS_LT_N_CT;
	plat_data->hi_ps_ct_h_offset = STK_HI_PS_HT_N_CT;
	plat_data->hi_ps_ct_l_offset = STK_HI_PS_LT_N_CT;


	if (obj_data->als_transmittance == 0) {
		dev_err(&client->dev,
			"%s: Please set als_transmittance\n", __func__);
		goto err_malloc_platform_data;
	}

//modify(add) by junfeng.zhou.sz for add power supply begin . 20140214
#ifdef POWER_REGULATOR
    	err = stk3x1x_power_init(obj_data, true);
	if (err)
		goto err_power_init;

	err = stk3x1x_power_ctl(obj_data, true);
	if (err)
		goto err_power_on;

	obj_data->als_enabled = false;
	obj_data->ps_enabled = false;

	err = stk3x1x_power_ctl(obj_data, false);
	if (err)
		goto err_init_all_setting_power;
#endif
//modify(add) by junfeng.zhou.sz for add power supply end .

	stk3x1x_set_wq(obj_data);
	err = stk3x1x_init_all_setting(client, plat_data);
	if(err < 0)
		goto err_chip_init;

	// Set up MTK als ps driver layer start
	stk3x1X_setup_mtk_layer_interface(obj_data);

	#if 0
	device_create_file(&(client->dev),&als_code_attribute);
	/*Add by falin.luo for show ps raw data in mmi start*/
	device_create_file(&(client->dev),&ps_raw_data_attribute);
	device_create_file(&(client->dev),&ps_code_thd_l_attribute);
	device_create_file(&(client->dev),&ps_code_thd_h_attribute);
	/*Add by falin.luo for show ps raw data in mmi end*/
	device_create_file(&(client->dev),&ps_offset_attribute);
	device_create_file(&(client->dev),&ps_prx_raw_off_attribute);
	device_create_file(&(client->dev),&ps_prx_raw_org_attribute);
	device_create_file(&(client->dev),&mmi_prx_thd_h_limit_attribute);
	device_create_file(&(client->dev),&mmi_prx_thd_l_limit_attribute);
	device_create_file(&(client->dev),&trace_param_attribute);
	#endif

	err = sysfs_create_group(&(client->dev.kobj), &stk_ps_attribute_group);
	err = sysfs_create_group(&(client->dev.kobj), &stk_als_attribute_group);

	stk3x1x_create_platform_driver_attr(&stk3x1x_init_info.platform_diver_addr->driver);

	err = stk3x1x_setup_irq(client);
	if(err < 0)
		goto err_stk3x1x_setup_irq;

	device_init_wakeup(&client->dev, true);

	stk_is_probe_success = 1;
/*Begin modified by zhikui.li@tcl.com for Increase the robust detection interface*/
	//i2c_check_status_create("ProximitySensor",1);
	//i2c_check_status_create("LightSensor",1);
/*End modified by zhikui.li@tcl.com for Increase the robust detection interface*/
	printk(KERN_INFO "%s: probe successfully\n", __func__);
/*begin modified by zhikui.li,add deviceinfo node*/
    if((err=create_chipinfo_node()))
    {
       printk("create chipinfo node err = %d\n", err);
    }
/*end modified by zhikui.li,add deviceinfo node*/
	return 0;

err_stk3x1x_setup_irq:

err_chip_init:
	stk3x1x_remove_wq(obj_data);

#ifdef POWER_REGULATOR
err_init_all_setting_power:
	stk3x1x_power_ctl(obj_data, false);
err_power_on:
	stk3x1x_power_init(obj_data, false);
err_power_init:
#endif

err_parse_dt:
	if (client->dev.of_node && (plat_data != NULL))
		devm_kfree(&client->dev, plat_data);
err_malloc_platform_data:
#ifdef CONFIG_PM_WAKELOCKS
	//wakeup_source_trash(&obj_data->ps_wakelock);
	wakeup_source_unregister(obj_data->ps_wakelock);
#else
	wake_lock_destroy(&obj_data->ps_wakelock);
#endif
	mutex_destroy(&obj_data->enable_mutex);
    	mutex_destroy(&stk_i2c_lock);
    	mutex_destroy(&obj_data->io_als_lock);
    	mutex_destroy(&obj_data->io_lock);
#ifdef CTTRACKING
	mutex_destroy(&obj_data->state_lock);
#endif
	if (obj_data)
		kfree(obj_data);

	stk_is_probe_success = -1;
    	return -ENODEV;
}


static int stk3x1x_remove(struct i2c_client *client)
{
	struct stk3x1x_data *obj_data = i2c_get_clientdata(client);

	device_init_wakeup(&client->dev, false);

	free_irq(obj_data->irq, obj_data);

	destroy_workqueue(obj_data->stk_wq);

	sysfs_remove_group(&obj_data->client->dev.kobj, &stk_ps_attribute_group);
	sysfs_remove_group(&obj_data->client->dev.kobj, &stk_als_attribute_group);

	hrtimer_try_to_cancel(&obj_data->als_timer);
	destroy_workqueue(obj_data->stk_als_wq);

#ifdef STK_TUNE0
	destroy_workqueue(obj_data->stk_ps_tune0_wq);
#endif
#ifdef CONFIG_PM_WAKELOCKS
    wakeup_source_unregister(obj_data->ps_wakelock);
	//wakeup_source_trash(&obj_data->ps_wakelock);
#else
	wake_lock_destroy(&obj_data->ps_wakelock);
#endif
    	mutex_destroy(&stk_i2c_lock);
    	mutex_destroy(&obj_data->io_als_lock);
    	mutex_destroy(&obj_data->io_lock);
	kfree(obj_data);
	//i2c_check_status_create("ProximitySensor",0);
	//i2c_check_status_create("LightSensor",0);
	return 0;
}

static const struct i2c_device_id stk_ps_id[] =
{
    	{ DEVICE_NAME, 0},
    	{}
};
MODULE_DEVICE_TABLE(i2c, stk_ps_id);

#ifdef CONFIG_OF
static const struct of_device_id alsps_of_match[] = {
        {.compatible = "mediatek,alsps"},
        {},
};
#endif

static struct i2c_driver stk_ps_driver =
{
    	.driver = {
        	.name = DEVICE_NAME,
		.owner = THIS_MODULE,
		//modify(add) by junfeng.zhou.sz for fix i2c timeout error with 661727 begin . 20140501
		.pm = &stk3x1x_pm_ops,
		//modify(add) by junfeng.zhou.sz for fix i2c timeout error with 661727 end .
		#ifdef CONFIG_OF
		.of_match_table = alsps_of_match,
		#endif
    	},
    	.probe = stk3x1x_probe,
    	.remove = stk3x1x_remove,
    	.id_table = stk_ps_id,
};


static int stk3x1x_local_init(void)
{

	if(i2c_add_driver(&stk_ps_driver))
	{
		printk("%s:add driver error\n", __func__);
		return -1;
	}
	if(-1 == stk_is_probe_success)
	{
	   return -1;
	}

	return 0;
}

static int stk3x1x_local_uninit(void)
{
	i2c_del_driver(&stk_ps_driver);
	return 0;
}

static struct alsps_init_info stk3x1x_init_info = {
		.name = "stk3x1x",
		.init = stk3x1x_local_init,
		.uninit = stk3x1x_local_uninit,

};

static int __init stk3x1x_init(void)
{
#if 0
          int ret;
          struct device_node *node = NULL;
	const char *name = "mediatek,stk3x1x";
	node = of_find_compatible_node(NULL, NULL, name);
          if(node!=NULL)
	   ret=get_alsps_dts_func(node, hw);
	else
	   printk("stk3x1x_init error!\n");
	if(ret<0) {
		printk("%s:get dts info fail\n", __func__);
		return -1;
	}
#endif
	printk("%s\n", __func__);
	alsps_driver_add(&stk3x1x_init_info);
	return 0;
}

static void __exit stk3x1x_exit(void)
{

}

module_init(stk3x1x_init);
module_exit(stk3x1x_exit);
MODULE_AUTHOR("Lex Hsieh <lex_hsieh@sensortek.com.tw>");
MODULE_DESCRIPTION("Sensortek stk3x1x Proximity Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);
