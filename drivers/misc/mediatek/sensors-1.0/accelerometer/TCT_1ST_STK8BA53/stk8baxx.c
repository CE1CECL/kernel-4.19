/*
 * stk8baxx_driver.c - Linux driver for sensortek stk8baxx accelerometer
 * Copyright (C) 2017 Sensortek
 *
 * history01:  Modified by weizhi.chen for Task 6925967 STK8BA53 driver update on 2018/09/12
 */
#include <asm/atomic.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/vmalloc.h>

#include <accel.h>
#include "cust_acc.h"
//#include "hwmsen_dev.h"
//#include "hwmsen_helper.h"
//#include "hwmsensor.h"
#include "sensors_io.h"
#include "step_counter.h"

/********* Global *********/
/**
 * enalbe INTERRUPT_MODE:
 *          SIGMOTION, DATA use the same INT1.
 * disable INTERRUPT_MODE:
 *          DATA read via polling, SIGMOTION use INT1.
 */
//#define INTERRUPT_MODE

/* Turn on sig motion */
#define STK_SIG_MOTION
#define SENSOR_DIRECTION_DEBUG 0

/* enable check code feature */
//#define STK_CHECK_CODE
#ifdef STK_CHECK_CODE
    /* Ignore the first STK_CHECKCODE_IGNORE+1 data for STK_CHECK_CODE feature */
    #define STK_CHECKCODE_IGNORE    3
    /* for stk8baxx_data.cc_status */
    #define STK_CCSTATUS_NORMAL     0x0
    #define STK_CCSTATUS_ZSIM       0x1
    #define STK_CCSTATUS_XYSIM      0x2
#endif /* STK_CHECK_CODE */

/*
 * enable low-pass mode.
 * Define STK_FIR to turn ON low pass mode.
 */
#define STK_FIR
#ifdef STK_FIR
    #define STK_FIR_LEN         2
    #define STK_FIR_LEN_MAX     32
#endif /* STK_FIR */

#ifdef STK_FIR
struct data_fir
{
    s16 xyz[STK_FIR_LEN_MAX][3];
    int sum[3];
    int idx;
    int count;
};
#endif /* STK_FIR */

/*
 * enable Zero-G simulation.
 * This feature only works when both of STK_FIR and STK_ZG are turn ON.
 */
//#define STK_ZG
#if (defined STK_FIR && defined STK_ZG)
    #define ZG_FACTOR   0
#endif /* defined STK_FIR && defined STK_ZG */

#define STK_ACC_DRIVER_VERSION "404.1.6" /* kernel-version:major-number:minor-number */
#define STK_ACC_DEV_NAME "stk8baxx"
#define STK_ACC_DTS_NAME "mediatek,stk8baxx"
#define STK8BAXX_IRQ_INT1_LABEL "STK_ACCEL_INT1"
#define STK8BAXX_IRQ_INT1_NAME "stk8baxx_int1"

#ifndef INTERRUPT_MODE /* no INTERRUPT_MODE, polling mode */
    #define STK8BAXX_IRQ_INT2_LABEL "STK_ACCEL_INT2"
    #define STK8BAXX_IRQ_INT2_NAME "stk8baxx_int2"
#endif /* no INTERRUPT_MODE */

#define STK_ACC_TAG                 "[STK8BAXX]"
#define STK_ACC_FUN(f)              printk(KERN_INFO STK_ACC_TAG" %s\n", __FUNCTION__)
#define ACC_LOG(fmt, args...)       printk(KERN_INFO STK_ACC_TAG" %s %d: "fmt"\n", __FUNCTION__, __LINE__, ##args)

#define ACC_PR_ERR(fmt, args...)    printk(KERN_ERR STK_ACC_TAG" %s %d: "fmt"\n", __FUNCTION__, __LINE__, ##args)
#define STK_ACC_ERR(fmt, args...)   printk(KERN_INFO STK_ACC_TAG" %s %d: "fmt"\n", __FUNCTION__, __LINE__, ##args)
#define STK_ACC_LOG(fmt, args...)   printk(KERN_INFO STK_ACC_TAG" %s %d: "fmt"\n", __FUNCTION__, __LINE__, ##args)

#ifdef CONFIG_CUSTOM_KERNEL_ACCELEROMETER_MODULE
bool success_Flag;
#endif /* CONFIG_CUSTOM_KERNEL_ACCELEROMETER_MODULE */

//add for gsensor Auto-rotate cannot work begin
#define STK_FAIL_MODE
#ifdef  gsensor_data_debug
static int gsensor_x = 111;
static int gsensor_y = 111;
static int gsensor_z = 111;
static int gsensor_number = 0 ;
#endif
#ifdef STK_FAIL_MODE
#define STK_DATA_COUNT	10
static s16 pre_xyz[3];
static int cnt_restart;
#endif
//add for gsensor Auto-rotate cannot work end

/* axis */
#define STK_AXIS_X                  0
#define STK_AXIS_Y                  1
#define STK_AXIS_Z                  2
#define STK_AXES_NUM                3

/* file_operateions parameters */
#define STK_BUFSIZE                 60

/* calibration parameters */
#define STK_CALI_SAMPLE_NO          10
#define STK_CALI_VER0               0x18
#define STK_CALI_VER1               0x03
#define STK_CALI_END                '\0'
#define STK_CALI_FILE               "/data/misc/sensor/stkacccali.conf"
#define STK_CALI_FILE_SIZE          25
/* parameter for cali_status/atomic_t and cali file */
#define STK_K_SUCCESS_FILE          0x01
/* parameter for cali_status/atomic_t */
#define STK_K_FAIL_WRITE_OFST       0xF2
#define STK_K_FAIL_I2C              0xF8
#define STK_K_FAIL_W_FILE           0xFB
#define STK_K_FAIL_VERIFY_CALI      0xFD
#define STK_K_RUNNING               0xFE
#define STK_K_NO_CALI               0xFF

/* stk8baxx slave address */
#define STK8BAXX_SLAVE_ADDR         0x18

/* stk8baxx register */
#define STK8BAXX_REG_CHIPID             0x00
#define STK8BAXX_REG_XOUT1              0x02
#define STK8BAXX_REG_XOUT2              0x03
#define STK8BAXX_REG_YOUT1              0x04
#define STK8BAXX_REG_YOUT2              0x05
#define STK8BAXX_REG_ZOUT1              0x06
#define STK8BAXX_REG_ZOUT2              0x07
#define STK8BAXX_REG_INTSTS1            0x09
#define STK8BAXX_REG_INTSTS2            0x0A
#define STK8BAXX_REG_RANGESEL           0x0F
#define STK8BAXX_REG_BWSEL              0x10
#define STK8BAXX_REG_POWMODE            0x11
#define STK8BAXX_REG_SWRST              0x14
#define STK8BAXX_REG_INTEN1             0x16
#define STK8BAXX_REG_INTEN2             0x17
#define STK8BAXX_REG_INTMAP1            0x19
#define STK8BAXX_REG_INTMAP2            0x1A
#define STK8BAXX_REG_INTCFG1            0x20
#define STK8BAXX_REG_INTCFG2            0x21
#define STK8BAXX_REG_SLOPEDLY           0x27
#define STK8BAXX_REG_SLOPETHD           0x28
#define STK8BAXX_REG_SIGMOT1            0x29
#define STK8BAXX_REG_SIGMOT2            0x2A
#define STK8BAXX_REG_SIGMOT3            0x2B
#define STK8BAXX_REG_INTFCFG            0x34
#define STK8BAXX_REG_OFSTCOMP1          0x36
#define STK8BAXX_REG_OFSTX              0x38
#define STK8BAXX_REG_OFSTY              0x39
#define STK8BAXX_REG_OFSTZ              0x3A

/* STK8BAXX_REG_CHIPID */
#define STK8BA50_R_ID                   0x86
#define STK8BA53_ID                     0x87

/* STK8BAXX_REG_INTSTS1 */
#define STK8BAXX_INTSTS1_SIG_MOT_STS     0x1
#define STK8BAXX_INTSTS1_ANY_MOT_STS     0x4

/* STK8BAXX_REG_RANGESEL */
#define STK8BAXX_RANGESEL_2G             0x3
#define STK8BAXX_RANGESEL_4G             0x5
#define STK8BAXX_RANGESEL_8G             0x8
#define STK8BAXX_RANGESEL_BW_MASK        0x1F
#define STK8BAXX_RANGESEL_DEF            STK8BAXX_RANGESEL_2G

/* STK8BAXX_REG_BWSEL */
#ifdef INTERRUPT_MODE
#define STK8BAXX_BWSEL_INIT_ODR          0x0A    /* ODR = BW x 2 = 62.5Hz */
#else /* no INTERRUPT_MODE, polling mode */
#define STK8BAXX_BWSEL_INIT_ODR          0x0F    /* ODR = BW x 2 = 125Hz */
#endif /* INTERRUPT_MODE */
/* ODR: 31.25, 62.5, 125 */
const static int STK8BAXX_SAMPLE_TIME[] = {32000, 16000, 8000}; /* usec */
#define STK8BAXX_SPTIME_BASE             0x9     /* for 32000, ODR:31.25 */
#define STK8BAXX_SPTIME_BOUND            0xB     /* for 8000, ODR:125 */

/* STK8BAXX_REG_POWMODE */
#define STK8BAXX_PWMD_SUSPEND            0x80
#define STK8BAXX_PWMD_LOWPOWER           0x40
#define STK8BAXX_PWMD_NORMAL             0x00
#define STK8BAXX_PWMD_SLP_MASK           0x3E

/* STK8BAXX_REG_SWRST */
#define STK8BAXX_SWRST_VAL               0xB6

/* STK8BAXX_REG_INTEN1 */
#define STK8BAXX_INTEN1_SLP_EN_XYZ       0x07
#define STK8BAXX_INTEN1_SLP_EN_X         0x01
#define STK8BAXX_INTEN1_SLP_EN_Y         0x02
#define STK8BAXX_INTEN1_SLP_EN_Z         0x04

/* STK8BAXX_REG_INTEN2 */
#define STK8BAXX_INTEN2_DATA_EN          0x10

/* STK8BAXX_REG_INTMAP1 */
#define STK8BAXX_INTMAP1_SIGMOT2INT1         0x01
#define STK8BAXX_INTMAP1_ANYMOT2INT1         0x04

/* STK8BAXX_REG_INTMAP2 */
#define STK8BAXX_INTMAP2_DATA2INT1           0x01

/* STK8BAXX_REG_INTCFG1 */
#define STK8BAXX_INTCFG1_INT1_ACTIVE_H       0x01
#define STK8BAXX_INTCFG1_INT1_ACTIVE_L       0x00
#define STK8BAXX_INTCFG1_INT1_OD_PUSHPULL    0x00

/* STK8BAXX_REG_INTCFG2 */
#define STK8BAXX_INTCFG2_NOLATCHED           0x00
#define STK8BAXX_INTCFG2_LATCHED             0x0F
#define STK8BAXX_INTCFG2_INT_RST             0x80

/* STK8BAXX_REG_SLOPETHD */
#define STK8BAXX_SLOPETHD_DEF                0xFE

/* STK8BAXX_REG_SIGMOT1 */
#define STK8BAXX_SIGMOT1_SKIP_TIME_3SEC      0x96    /* default value */

/* STK8BAXX_REG_SIGMOT2 */
#define STK8BAXX_SIGMOT2_SIG_MOT_EN          0x02

/* STK8BAXX_REG_SIGMOT3 */
#define STK8BAXX_SIGMOT3_PROOF_TIME_1SEC     0x32    /* default value */

/* STK8BAXX_REG_INTFCFG */
#define STK8BAXX_INTFCFG_I2C_WDT_EN          0x04

/* STK8BAXX_REG_OFSTCOMP1 */
#define STK8BAXX_OFSTCOMP1_OFST_RST          0x80

/* STK8BAXX_REG_OFSTx */
#define STK8BAXX_OFST_LSB                    128     /* 8 bits for +-1G */

struct stk8baxx_data
{
    /* platform related */
    struct i2c_client           *client;
    struct acc_hw               hw;
    struct hwmsen_convert       cvt;
    /* chip informateion */
    int                         pid;
    /* system operation */
    atomic_t                    enabled;            /* chip is enabled or not */
    atomic_t                    enabled_for_acc;    /* enable status for acc_control_path.enable_nodata */
    atomic_t                    cali_status;        /* cali status */
    s16                         cali_sw[STK_AXES_NUM];  /* cali data */
    atomic_t                    recv;               /* recv data. DEVICE_ATTR(recv, ...) */
    struct mutex                reg_lock;           /* mutex lock for register R/W */
    u8                          power_mode;
    bool                        temp_enable;        /* record current power status. For Suspend/Resume used. */
    int                         sensitivity;        /* sensitivity, bit number per G */
    s16                         xyz[3];             /* The latest data of xyz */
    s16                         steps;              /* The latest step counter value */
#ifdef STK_SIG_MOTION
   // int                         interrupt_int1_pin; /* get from device tree */
   // int                         irq1;               /* for all data usage(DATA, SIGMOTION) */
    struct workqueue_struct     *alldata_workqueue; /* all data workqueue for int1. (DATA, SIGMOTION) */
    struct work_struct          alldata_work;       /* all data work for int1. (DATA, SIGMOTION) */
    int                         sig_motion_enable;
#endif 

    int                         interrupt_int1_pin; /* get from device tree */
    int                         irq1;               /* for sig usage */
    struct delayed_work         sig_delaywork;      /* sig delay work for int1. */
    struct delayed_work         accel_delaywork;
    struct hrtimer              accel_timer;
    ktime_t                     poll_delay;
    bool                        suspend;
#ifdef STK_CHECK_CODE
    int                         cc_count;
    u8                          cc_status;          /* refer STK_CCSTATUS_x */
#endif /* STK_CHECK_CODE */
#ifdef STK_FIR
    struct data_fir             fir;
    /*
     * fir_len
     * 0: turn OFF FIR operation
     * 1 ~ STK_FIR_LEN_MAX: turn ON FIR operation
     */
    atomic_t                    fir_len;
#endif /* STK_FIR */
};

struct stk8baxx_data *stk_data;


#ifdef STK_SIG_MOTION
int stk8baxx_set_acc_interrupter(bool flag);
int stk_interrupt_mode_setup(void);
int stk8baxx_init_flag = -1;

#endif

static int stk_acc_init(void);
static int stk_acc_uninit(void);

static struct acc_init_info stk_acc_init_info =
{
    .name = STK_ACC_DEV_NAME,
    .init = stk_acc_init,
    .uninit = stk_acc_uninit,
};
/********* Functions *********/
/**
 * stk8baxx register write
 * @brief: Register writing via I2C
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] reg: Register address
 * @param[in] val: Data, what you want to write.
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk8baxx_reg_write(struct stk8baxx_data *stk, u8 reg, u8 val)
{
    int error = 0;
    mutex_lock(&stk->reg_lock);
    error = i2c_smbus_write_byte_data(stk->client, reg, val);
    mutex_unlock(&stk->reg_lock);

    if (error)
    {
        STK_ACC_ERR("transfer failed to write reg:0x%x with val:0x%x, error=%d\n", reg, val, error);
    }

    return error;
}

/**
 * stk8baxx register read
 * @brief: Register reading via I2C
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] reg: Register address
 * @param[in] len: 0/1, for normal usage.
 * @param[out] val: Data, the register what you want to read.
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk8baxx_reg_read(struct stk8baxx_data *stk, u8 reg, int len, u8 *val)
{
    int error = 0;
    struct i2c_msg msgs[2] = {
        {
            .addr = stk->client->addr,
            .flags = 0,
            .len = 1,
            .buf = &reg
        },
        {
            .addr = stk->client->addr,
            .flags = I2C_M_RD,
            .len = (0 >= len) ? 1 : len,
            .buf = val
        }
    };

    mutex_lock(&stk->reg_lock);
    error = i2c_transfer(stk->client->adapter, msgs, 2);
    mutex_unlock(&stk->reg_lock);

    if (2 == error)
        error = 0;
    else if (0 > error)
    {
        STK_ACC_ERR("transfer failed to read reg:0x%x with len:%d, error=%d\n", reg, len, error);
        error = -EIO;
    }
    else
    {
        STK_ACC_ERR("size error in reading reg:0x%x with len:%d, error=%d\n", reg, len, error);
        error = -EIO;
    }

    return error;
}

/**
 * @brief: Read PID and write to stk8baxx_data.pid.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 *
 * @return: Success or fail.
 *          0: Success
 *          others: Fail
 */
static int stk_get_pid(struct stk8baxx_data *stk)
{
    int error = 0;
    u8 val = 0;
    error = stk8baxx_reg_read(stk, STK8BAXX_REG_CHIPID, 0, &val);

    if (error)
        STK_ACC_ERR("failed to read PID");
    else
        stk->pid = (int)val;

    return error;
}

/**
 * @brief: Initialize some data in stk8baxx_data.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_data_initialize(struct stk8baxx_data *stk)
{
    atomic_set(&stk->enabled, 0);
    atomic_set(&stk->enabled_for_acc, 0);
    atomic_set(&stk->cali_status, STK_K_NO_CALI);
    memset(stk->cali_sw, 0x0, sizeof(stk->cali_sw));
    atomic_set(&stk->recv, 0);
    stk->power_mode = STK8BAXX_PWMD_NORMAL;
    stk->temp_enable = false;
#ifdef STK_FIR
    memset(&stk->fir, 0, sizeof(struct data_fir));
    atomic_set(&stk->fir_len, STK_FIR_LEN);
#endif /* STK_FIR */
//add for gsensor Auto-rotate cannot work begin
#ifdef STK_FAIL_MODE
    pre_xyz[0] = 0;
    pre_xyz[1] = 0;
    pre_xyz[2] = 0;
    cnt_restart = 0;
#endif
//add for gsensor Auto-rotate cannot work end
}

/**
 * @brief: SW reset for stk8baxx
 *
 * @param[in/out] stk: struct stk8baxx_data *
 *
 * @return: Success or fail.
 *          0: Success
 *          others: Fail
 */
static int stk_sw_reset(struct stk8baxx_data *stk)
{
    int error = 0;
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_SWRST, STK8BAXX_SWRST_VAL);

    if (error)
        return error;

    usleep_range(1000, 2000);
    return 0;
}

/**
 * @brief: Change power mode
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] pwd_md: power mode for STK8BAXX_REG_POWMODE
 *              STK8BAXX_PWMD_SUSPEND
 *              STK8BAXX_PWMD_LOWPOWER
 *              STK8BAXX_PWMD_NORMAL
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk_change_power_mode(struct stk8baxx_data *stk, u8 pwd_md)
{
    if (pwd_md != stk->power_mode)
    {
        int error = 0;
        u8 val = 0;
        error = stk8baxx_reg_read(stk, STK8BAXX_REG_POWMODE, 0, &val);

        if (error)
            return error;

        val &= STK8BAXX_PWMD_SLP_MASK;
        error = stk8baxx_reg_write(stk, STK8BAXX_REG_POWMODE, (val | pwd_md));

        if (error)
            return error;

        stk->power_mode = pwd_md;
    }
    else
        STK_ACC_LOG("Same as original power mode: 0x%X\n", stk->power_mode);

    return 0;
}

/**
 * @brief: Get sensitivity. Set result to stk8baxx_data.sensitivity.
 *          sensitivity = number bit per G (LSB/g)
 *          Example: RANGESEL=8g, 12 bits for STK832x full resolution
 *          Ans: number bit per G = 2^12 / (8x2) = 256 (LSB/g)
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_get_sensitivity(struct stk8baxx_data *stk)
{
    u8 val = 0;
    stk->sensitivity = 0;

    if ( 0 == stk8baxx_reg_read(stk, STK8BAXX_REG_RANGESEL, 0, &val))
    {
        val &= STK8BAXX_RANGESEL_BW_MASK;

        if (STK8BA53_ID == stk->pid)
        {
            switch (val)
            {
                case STK8BAXX_RANGESEL_2G:
                    stk->sensitivity = 1024;
                    break;

                case STK8BAXX_RANGESEL_4G:
                    stk->sensitivity = 512;
                    break;

                case STK8BAXX_RANGESEL_8G:
                    stk->sensitivity = 256;
                    break;

                default:
                    break;
            }
        }
        else
        {
            switch (val)
            {
                case STK8BAXX_RANGESEL_2G:
                    stk->sensitivity = 256;
                    break;

                case STK8BAXX_RANGESEL_4G:
                    stk->sensitivity = 128;
                    break;

                case STK8BAXX_RANGESEL_8G:
                    stk->sensitivity = 64;
                    break;

                default:
                    break;
            }
        }
    }
}

/**
 * @brief: Set range
 *          1. Setting STK8BAXX_REG_RANGESEL
 *          2. Calculate sensitivity and store to stk8baxx_data.sensitivity
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] range: range for STK8BAXX_REG_RANGESEL
 *              STK8BAXX_RANGESEL_2G
 *              STK8BAXX_RANGESEL_4G
 *              STK8BAXX_RANGESEL_8G
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk_range_selection(struct stk8baxx_data *stk, u8 range)
{
    int result = 0;
    result = stk8baxx_reg_write(stk, STK8BAXX_REG_RANGESEL, range);

    if (result)
        return result;

    stk_get_sensitivity(stk);
    return 0;
}

/**
 * stk_set_enable
 * @brief: Turn ON/OFF the power state of stk8baxx.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] en: turn ON/OFF
 *              0 for suspend mode;
 *              1 for normal mode.
 */
static void stk_set_enable(struct stk8baxx_data *stk, char en)
{
    if (en == atomic_read(&stk->enabled))
        return;

    if (en)
    {
        /* ID46: Low-power -> Suspend -> Normal */
        if (stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND))
            return;

        if (stk_change_power_mode(stk, STK8BAXX_PWMD_NORMAL))
            return;

#ifndef INTERRUPT_MODE /* polling mode */
//modified by qiuyang.duan@tcl.com  defect:8463227	2019/12/03 [Performance][DOU][PIO]Battery is losing fast when idle(14% in 16 hours)  kernel can not suspend
       // hrtimer_start(&stk->accel_timer, stk->poll_delay, HRTIMER_MODE_REL);
//modified end by qiuyang.duan@tcl.com 	2019/12/03 defect:8463227 [Performance][DOU][PIO]Battery is losing fast when idle(14% in 16 hours)  kernel can not suspend   
          
#endif /* no INTERRUPT_MODE */
#ifdef STK_CHECK_CODE
        stk->cc_count = 0;
        stk->cc_status = STK_CCSTATUS_NORMAL;
#endif /* STK_CHECK_CODE */
    }
    else
    {
        #ifndef STK_SIG_MOTION
        if (stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND))
            return;
        #else
        if (stk->sig_motion_enable != 1)
        {
            if (stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND))
                return;
        }
        #endif

#ifndef INTERRUPT_MODE /* polling mode */
//modified begin by qiuyang.duan@tcl.com defect:8463227	2019/12/03 [Performance][DOU][PIO]Battery is losing fast when idle(14% in 16 hours)  kernel can not suspend
       // hrtimer_cancel(&stk->accel_timer);
//modified end by qiuyang.duan@tcl.com 	2019/12/03 defect:8463227 [Performance][DOU][PIO]Battery is losing fast when idle(14% in 16 hours)  kernel can not suspend   
       
#endif /* no INTERRUPT_MODE */
    }

//add for gsensor Auto-rotate cannot work begin
#ifdef STK_FAIL_MODE
    pre_xyz[0] = 0;
    pre_xyz[1] = 0;
    pre_xyz[2] = 0;
    cnt_restart = 0;
#endif
//add for gsensor Auto-rotate cannot work end

    atomic_set(&stk->enabled, en);
}

/**
 * @brief: Get delay
 *
 * @param[in/out] stk: struct stk8baxx_data *
 *
 * @return: delay in usec
 *          Please refer STK8BAXX_SAMPLE_TIME[]
 */
static int stk_get_delay(struct stk8baxx_data *stk)
{
    u8 data = 0;
    int delay_us = 0;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_BWSEL, 0, &data))
    {
        STK_ACC_ERR("failed to read delay");
    }
    else if ((STK8BAXX_SPTIME_BASE > data) || (STK8BAXX_SPTIME_BOUND < data))
    {
        STK_ACC_ERR("BW out of range, 0x%X", data);
    }
    else
    {
        delay_us = STK8BAXX_SAMPLE_TIME[data - STK8BAXX_SPTIME_BASE];
    }

    return delay_us;
}

/**
 * @brief: Set delay
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] delay_us: delay in usec
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk_set_delay(struct stk8baxx_data *stk, int delay_us)
{
    int error = 0;
    bool enable = false;
    unsigned char sr_no;

    for (sr_no = 0; sr_no <= STK8BAXX_SPTIME_BOUND - STK8BAXX_SPTIME_BASE;
         sr_no++)
        if (delay_us >= STK8BAXX_SAMPLE_TIME[sr_no])
            break;

    if (sr_no == STK8BAXX_SPTIME_BOUND - STK8BAXX_SPTIME_BASE + 1)
    {
        sr_no--;
        //delay_us = STK8BAXX_SAMPLE_TIME[sr_no];
    }

    sr_no += STK8BAXX_SPTIME_BASE;

    if (atomic_read(&stk->enabled))
    {
        stk_set_enable(stk, 0);
        enable = true;
    }

    error = stk8baxx_reg_write(stk, STK8BAXX_REG_BWSEL, sr_no);
    STK_ACC_ERR("qiuyang1 STK8BAXX_REG_BWSEL %x",sr_no);

    if (error)
        STK_ACC_ERR("failed to change ODR");

    if (enable)
    {
        stk_set_enable(stk, 1);
    }

#ifndef INTERRUPT_MODE /* polling mode */
    stk->poll_delay = ns_to_ktime(
                          STK8BAXX_SAMPLE_TIME[sr_no - STK8BAXX_SPTIME_BASE] * NSEC_PER_USEC);
#endif /* no INTERRUPT_MODE */
    return error;
}

#ifdef STK_CHECK_CODE
/**
 * @brief: check stiction or not
 *          If 3 times and continue stiction will change stk8baxx_data.cc_status
 *          to STK_CCSTATUS_ZSIM or STK_CCSTATUS_XYZIM.
 *          Others, keep stk8baxx_data.cc_status to STK_CCSTATUS_NORMAL.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] clean: clean internal flag of check_result or not.
 *                  true: clean check_result
 *                  false: don't clean check_result
 */
static void stk_check_data(struct stk8baxx_data *stk, bool clean)
{
    static s8 event_no = 0;
    static s8 check_result = 0;
    int max_value = 0, min_value = 0;
    if (STK8BA53_ID == stk->pid)
    {
        /* 12 bits per axis */
        max_value = 2047;
        min_value = -2048;
    }
    else
    {
        /* 10 bits per axis */
        max_value = 511;
        min_value = -512;
    }

    if (18 <= event_no)
        return;

    if (max_value == stk->xyz[0] || min_value == stk->xyz[0]
        || max_value == stk->xyz[1] || min_value == stk->xyz[1]
        || max_value == stk->xyz[2] || min_value == stk->xyz[2])
    {
        STK_ACC_LOG("acc:0x%X, 0x%X, 0x%X\n", stk->xyz[0], stk->xyz[1], stk->xyz[2]);
        check_result++;
    }
    else
    {
        check_result = 0;
        goto exit;
    }

    if (clean)
    {
        if (3 <= check_result)
        {
            if (max_value != stk->xyz[0] && min_value != stk->xyz[0]
                && max_value != stk->xyz[1] && min_value != stk->xyz[1])
                stk->cc_status = STK_CCSTATUS_ZSIM;
            else
                stk->cc_status = STK_CCSTATUS_XYSIM;

            STK_ACC_LOG("incorrect reading");
        }

        check_result = 0;
    }

exit:
    event_no++;
    return;
}

/**
 * @brief: sqrt operation
 *
 * @return sqrt(in)
 */
static int stk_sqrt(int in)
{
    int root, bit;
    root = 0;
    for (bit = 0x4000; bit > 0; bit >>= 2)
    {
        int trial = root + bit;
        root >>= 1;
        if (trial <= in)
        {
            root += bit;
            in -= trial;
        }
    }
    return root;
}

/**
 * @brief: check_code operation
 *          z = sqrt(x^2 + y^2)
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_check_code(struct stk8baxx_data *stk)
{
    u16 x, y;
    int sen;
    sen = stk->sensitivity;

    if (0 <= stk->xyz[0])
        x = stk->xyz[0];
    else
        x = -stk->xyz[0];

    if (0 <= stk->xyz[1])
        y = stk->xyz[1];
    else
        y = -stk->xyz[1];

    if ((x >= sen) || (y >= sen))
    {
        stk->xyz[2] = 0;
        return;
    }

    stk->xyz[2] = stk_sqrt(sen * sen - x * x - y * y);
}
#endif /* STK_CHECK_CODE */

#ifdef STK_FIR
/**
 * @brief: low-pass filter operation
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_low_pass_fir(struct stk8baxx_data *stk)
{
    int firlength = atomic_read(&stk->fir_len);
#ifdef STK_ZG
    s16 avg;
    int jitter_boundary = stk->sensitivity / 128;
#if 0

    if (0 == jitter_boundary)
        jitter_boundary = 1;

#endif
#endif /* STK_ZG */

    if (0 == firlength)
    {
        /* stk8baxx_data.fir_len == 0: turn OFF FIR operation */
        return;
    }

    if (firlength > stk->fir.count)
    {
        stk->fir.xyz[stk->fir.idx][0] = stk->xyz[0];
        stk->fir.xyz[stk->fir.idx][1] = stk->xyz[1];
        stk->fir.xyz[stk->fir.idx][2] = stk->xyz[2];
        stk->fir.sum[0] += stk->xyz[0];
        stk->fir.sum[1] += stk->xyz[1];
        stk->fir.sum[2] += stk->xyz[2];
        stk->fir.count++;
        stk->fir.idx++;
    }
    else
    {
        if (firlength <= stk->fir.idx)
            stk->fir.idx = 0;

        stk->fir.sum[0] -= stk->fir.xyz[stk->fir.idx][0];
        stk->fir.sum[1] -= stk->fir.xyz[stk->fir.idx][1];
        stk->fir.sum[2] -= stk->fir.xyz[stk->fir.idx][2];
        stk->fir.xyz[stk->fir.idx][0] = stk->xyz[0];
        stk->fir.xyz[stk->fir.idx][1] = stk->xyz[1];
        stk->fir.xyz[stk->fir.idx][2] = stk->xyz[2];
        stk->fir.sum[0] += stk->xyz[0];
        stk->fir.sum[1] += stk->xyz[1];
        stk->fir.sum[2] += stk->xyz[2];
        
        stk->fir.idx++;
#ifdef STK_ZG
        avg = stk->fir.sum[0] / firlength;

        if (abs(avg) <= jitter_boundary)
            stk->xyz[0] = (avg * ZG_FACTOR) / firlength;
        else
            stk->xyz[0] = avg / firlength;

        avg = stk->fir.sum[1] / firlength;

        if (abs(avg) <= jitter_boundary)
            stk->xyz[1] = (avg * ZG_FACTOR) / firlength;
        else
            stk->xyz[1] = avg / firlength;

        avg = stk->fir.sum[2] / firlength;

        if (abs(avg) <= jitter_boundary)
            stk->xyz[2] = (avg * ZG_FACTOR) / firlength;
        else
            stk->xyz[2] = avg / firlength;

#else /* STK_ZG */
        stk->xyz[0] = stk->fir.sum[0] / firlength;
        stk->xyz[1] = stk->fir.sum[1] / firlength;
        stk->xyz[2] = stk->fir.sum[2] / firlength;
#endif /* STK_ZG */
    }
}
#endif /* STK_FIR */

/**
 * @brief: read accel raw data from register.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_read_accel_rawdata(struct stk8baxx_data *stk)
{
    u8 dataL = 0;
    u8 dataH = 0;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_XOUT1, 0, &dataL))
        return;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_XOUT2, 0, &dataH))
        return;

    stk->xyz[0] = dataH << 8 | dataL;
    if (STK8BA53_ID == stk->pid)
        stk->xyz[0] >>= 4;
    else
        stk->xyz[0] >>= 6;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_YOUT1, 0, &dataL))
        return;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_YOUT2, 0, &dataH))
        return;

    stk->xyz[1] = dataH << 8 | dataL;
    if (STK8BA53_ID == stk->pid)
        stk->xyz[1] >>= 4;
    else
        stk->xyz[1] >>= 6;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_ZOUT1, 0, &dataL))
        return;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_ZOUT2, 0, &dataH))
        return;

    stk->xyz[2] = dataH << 8 | dataL;
    if (STK8BA53_ID == stk->pid)
        stk->xyz[2] >>= 4;
    else
        stk->xyz[2] >>= 6;

//add for gsensor Auto-rotate cannot work begin
#ifdef STK_FAIL_MODE
    if (pre_xyz[0] == stk->xyz[0] && pre_xyz[1] == stk->xyz[1] && pre_xyz[2] == stk->xyz[2]) {
        //STK_ACC_ERR("raw value same as previous, cnt=%d", cnt_restart);
        cnt_restart++;
    } else {
        pre_xyz[0] = stk->xyz[0];
        pre_xyz[1] = stk->xyz[1];
        pre_xyz[2] = stk->xyz[2];
        cnt_restart = 0;
    }

    if (STK_DATA_COUNT <= cnt_restart) {
        // restart
        STK_ACC_LOG("restart, cnt=%d", cnt_restart);

        if (stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND)) {
            STK_ACC_ERR("SUSPEND failed");
        }
        msleep(32);

        if (stk_change_power_mode(stk, STK8BAXX_PWMD_NORMAL)) {
            STK_ACC_ERR("SUSPEND failed");
        }
        msleep(32);

        if (stk8baxx_reg_read(stk, STK8BAXX_REG_XOUT1, 0, &dataL))
            return;
        if (stk8baxx_reg_read(stk, STK8BAXX_REG_XOUT2, 0, &dataH))
            return;
        stk->xyz[0] = dataH << 8 | dataL;
        if (STK8BA53_ID == stk->pid)
            stk->xyz[0] >>= 4;
        else
            stk->xyz[0] >>= 6;

        if (stk8baxx_reg_read(stk, STK8BAXX_REG_YOUT1, 0, &dataL))
            return;
        if (stk8baxx_reg_read(stk, STK8BAXX_REG_YOUT2, 0, &dataH))
            return;
        stk->xyz[1] = dataH << 8 | dataL;
        if (STK8BA53_ID == stk->pid)
            stk->xyz[1] >>= 4;
        else
            stk->xyz[1] >>= 6;

        if (stk8baxx_reg_read(stk, STK8BAXX_REG_ZOUT1, 0, &dataL))
            return;
        if (stk8baxx_reg_read(stk, STK8BAXX_REG_ZOUT2, 0, &dataH))
            return;
        stk->xyz[2] = dataH << 8 | dataL;
        if (STK8BA53_ID == stk->pid)
            stk->xyz[2] >>= 4;
        else
            stk->xyz[2] >>= 6;
       
        pre_xyz[0] = stk->xyz[0];
        pre_xyz[1] = stk->xyz[1];
        pre_xyz[2] = stk->xyz[2];
        cnt_restart = 0;
        STK_ACC_LOG("gy_acc already restart x=%d,y=%d,z=%d",pre_xyz[0],pre_xyz[1],pre_xyz[2]);
//add for gsensor Auto-rotate cannot work end
    }
#endif
}

/**
 * @brief: read accel data from register.
 *          Store result to stk8baxx_data.xyz[].
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_read_accel_data(struct stk8baxx_data *stk)
{
    stk_read_accel_rawdata(stk);
#ifdef STK_CHECK_CODE

    if ((STK_CHECKCODE_IGNORE + 1) == stk->cc_count
        || (STK_CHECKCODE_IGNORE + 2) == stk->cc_count)
        stk_check_data(stk, false);
    else if ((STK_CHECKCODE_IGNORE + 3) == stk->cc_count)
        stk_check_data(stk, true);
    else if (STK_CCSTATUS_ZSIM == stk->cc_status)
        stk_check_code(stk);

    if ((STK_CHECKCODE_IGNORE + 6) > stk->cc_count)
        stk->cc_count++;

#endif /* STK_CHECK_CODE */
#ifdef STK_FIR
    stk_low_pass_fir(stk);
#endif /* STK_FIR */
}

/**
 * @brief: Write calibration config file to STK_CALI_FILE.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] w_buf: cali data what want to write to STK_CALI_FILE.
 * @param[in] buf_size: size of w_buf.
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk_write_to_file(struct stk8baxx_data *stk,
                             char *w_buf, int8_t buf_size)
{/*
    struct file *cali_file;
    char r_buf[buf_size];
    mm_segment_t fs;
    ssize_t ret;
    int i;
    cali_file = filp_open(STK_CALI_FILE, O_CREAT | O_RDWR, 0666);

    if (IS_ERR(cali_file))
    {
        STK_ACC_ERR("err=%ld, failed to open %s", PTR_ERR(cali_file), STK_CALI_FILE);
        return -ENOENT;
    }
    else
    {
        fs = get_fs();
        set_fs(get_ds());
        ret = cali_file->f_op->write(cali_file, w_buf, buf_size,
                                     &cali_file->f_pos);

        if (0 > ret)
        {
            STK_ACC_ERR("write error, ret=%d", (int)ret);
            filp_close(cali_file, NULL);
            return -EIO;
        }

        cali_file->f_pos = 0x0;
        ret = cali_file->f_op->read(cali_file, r_buf, buf_size,
                                    &cali_file->f_pos);

        if (0 > ret)
        {
            STK_ACC_ERR("read error, ret=%d", (int)ret);
            filp_close(cali_file, NULL);
            return -EIO;
        }

        set_fs(fs);

        for (i = 0; i < buf_size; i++)
        {
            if (r_buf[i] != w_buf[i])
            {
                STK_ACC_ERR("read back error! r_buf[%d]=0x%X, w_buf[%d]=0x%X", i, r_buf[i], i, w_buf[i]);
                filp_close(cali_file, NULL);
                return -1;
            }
        }
    }

    filp_close(cali_file, NULL);*/
    return 0;
}

/**
 * @brief: Get calibration config file from STK_CALI_FILE.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[out] r_buf: cali data what want to read from STK_CALI_FILE.
 * @param[in] buf_size: size of r_buf.
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk_get_from_file(struct stk8baxx_data *stk,
                             char *r_buf, int8_t buf_size)
{
    struct file *cali_file;
    mm_segment_t fs;
    ssize_t ret;
    cali_file = filp_open(STK_CALI_FILE, O_RDONLY, 0);

    if (IS_ERR(cali_file))
    {
        STK_ACC_ERR("err=%ld, failed to open %s", PTR_ERR(cali_file), STK_CALI_FILE);
        return -ENOENT;
    }
    else
    {
        fs = get_fs();
        set_fs(get_ds());
        ret = cali_file->f_op->read(cali_file, r_buf, buf_size,
                                    &cali_file->f_pos);
        set_fs(fs);

        if (0 > ret)
        {
            STK_ACC_ERR("read error, ret=%d\n", (int)ret);
            filp_close(cali_file, NULL);
            return -EIO;
        }
    }

    filp_close(cali_file, NULL);
    return -1;
}

/**
 * @brief:
 */
static void stk_get_cali(struct stk8baxx_data *stk, u8 cali[3])
{
    char stk_file[STK_CALI_FILE_SIZE];

    if (stk_get_from_file(stk, stk_file, STK_CALI_FILE_SIZE) == 0)
    {
        if (STK_CALI_VER0 == stk_file[0]
            && STK_CALI_VER1 == stk_file[1]
            && STK_CALI_END == stk_file[STK_CALI_FILE_SIZE - 1])
        {
            atomic_set(&stk->cali_status, (int)stk_file[8]);
            cali[0] = stk_file[3];
            cali[1] = stk_file[5];
            cali[2] = stk_file[7];
            STK_ACC_LOG("offset:%d,%d,%d, mode=0x%X", stk_file[3], stk_file[5], stk_file[7], stk_file[8]);
#if 0
            STK_ACC_LOG("variance=%u,%u,%u",
                        (stk_file[9] << 24 | stk_file[10] << 16 | stk_file[11] << 8 | stk_file[12]),
                        (stk_file[13] << 24 | stk_file[14] << 16 | stk_file[15] << 8 | stk_file[16]),
                        (stk_file[17] << 24 | stk_file[18] << 16 | stk_file[19] << 8 | stk_file[20]));
#endif
        }
        else
        {
            int i;
            STK_ACC_ERR("wrong cali version number");

            for (i = 0; i < STK_CALI_FILE_SIZE; i++)
                STK_ACC_LOG("cali_file[%d]=0x%X\n", i, stk_file[i]);
        }
    }
}

/**
 * @brief: Get sample_no of samples then calculate average
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] delay_ms: delay in msec
 * @param[in] sample_no: amount of sample
 * @param[out] acc_ave: XYZ average
 */
static void stk_calculate_average(struct stk8baxx_data *stk,
                                  unsigned int delay_ms, int sample_no, int acc_ave[3])
{
    int i;

    for (i = 0; i < sample_no; i++)
    {
        msleep(delay_ms);
        stk_read_accel_data(stk);
        acc_ave[0] += stk->xyz[0];
        acc_ave[1] += stk->xyz[1];
        acc_ave[2] += stk->xyz[2];
    }

    /*
     * Take ceiling operation.
     * ave = (ave + SAMPLE_NO/2) / SAMPLE_NO
     *     = ave/SAMPLE_NO + 1/2
     * Example: ave=7, SAMPLE_NO=10
     * Ans: ave = 7/10 + 1/2 = (int)(1.2) = 1
     */
    for (i = 0; i < 3; i++)
    {
        if ( 0 <= acc_ave[i])
            acc_ave[i] = (acc_ave[i] + sample_no / 2) / sample_no;
        else
            acc_ave[i] = (acc_ave[i] - sample_no / 2) / sample_no;
    }

    /*
     * For Z-axis
     * Pre-condition: Sensor be put on a flat plane, with +z face up.
     */
    if (0 < acc_ave[2])
        acc_ave[2] -= stk->sensitivity;
    else
        acc_ave[2] += stk->sensitivity;
}

/**
 * @brief: Align STK8BAXX_REG_OFSTx sensitivity with STK8BAXX_REG_RANGESEL
 *  Description:
 *  Example:
 *      RANGESEL=0x3 -> +-2G / 12bits for STK832x full resolution
 *              number bit per G = 2^12 / (2x2) = 1024 (LSB/g)
 *              (2x2) / 2^12 = 0.97 mG/bit
 *      OFSTx: There are 8 bits to describe OFSTx for +-1G
 *              number bit per G = 2^8 / (1x2) = 128 (LSB/g)
 *              (1x2) / 2^8 = 7.8125mG/bit
 *      Align: acc_OFST = acc * 128 / 1024
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in/out] acc: accel data
 *
 */
static void stk_align_offset_sensitivity(struct stk8baxx_data *stk, int acc[3])
{
    int axis;

    /*
     * Take ceiling operation.
     * ave = (ave + SAMPLE_NO/2) / SAMPLE_NO
     *     = ave/SAMPLE_NO + 1/2
     * Example: ave=7, SAMPLE_NO=10
     * Ans: ave = 7/10 + 1/2 = (int)(1.2) = 1
     */
    for (axis = 0; axis < 3; axis++)
    {
        if (acc[axis] > 0)
        {
            acc[axis] = (acc[axis] * STK8BAXX_OFST_LSB + stk->sensitivity / 2)
                        / stk->sensitivity;
        }
        else
        {
            acc[axis] = (acc[axis] * STK8BAXX_OFST_LSB - stk->sensitivity / 2)
                        / stk->sensitivity;
        }
    }
}

/**
 * @brief: Read all register (0x0 ~ 0x3F)
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[out] show_buffer: record all register value
 *
 * @return: buffer length or fail
 *          positive value: return buffer length
 *          -1: Fail
 */
static int stk_show_all_reg(struct stk8baxx_data *stk, char *show_buffer)
{
    int reg;
    int len = 0;
    u8 data = 0;

    if (NULL == show_buffer)
        return -1;

    for (reg = 0; reg <= 0x3F; reg++)
    {
        if (stk8baxx_reg_read(stk, reg, 0, &data))
        {
            len = -1;
            goto exit;
        }

        if (0 >= (PAGE_SIZE - len))
        {
            STK_ACC_ERR("print string out of PAGE_SIZE");
            goto exit;
        }

        len += scnprintf(show_buffer + len, PAGE_SIZE - len,
                         "[0x%2X]=0x%2X, ", reg, data);
    }

    len += scnprintf(show_buffer + len, PAGE_SIZE - len, "\n");
exit:
    return len;
}

/**
 * @brief: Get offset
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[out] offset: offset value read from register
 *                  STK8BAXX_REG_OFSTX,  STK8BAXX_REG_OFSTY, STK8BAXX_REG_OFSTZ
 *
 * @return: Success or fail
 *          0: Success
 *          -1: Fail
 */
static int stk_get_offset(struct stk8baxx_data *stk, u8 offset[3])
{
    int error = 0;
    bool enable = false;

    if (!atomic_read(&stk->enabled))
        stk_set_enable(stk, 1);
    else
        enable = true;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_OFSTX, 0, &offset[0]))
    {
        error = -1;
        goto exit;
    }

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_OFSTY, 0, &offset[1]))
    {
        error = -1;
        goto exit;
    }

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_OFSTZ, 0, &offset[2]))
    {
        error = -1;
        goto exit;
    }

exit:

    if (!enable)
        stk_set_enable(stk, 0);

    return error;
}

/**
 * @brief: Set offset
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] offset: offset value write to register
 *                  STK8BAXX_REG_OFSTX,  STK8BAXX_REG_OFSTY, STK8BAXX_REG_OFSTZ
 *
 * @return: Success or fail
 *          0: Success
 *          -1: Fail
 */
static int stk_set_offset(struct stk8baxx_data *stk, u8 offset[3])
{
    int error = 0;
    bool enable = false;

    if (!atomic_read(&stk->enabled))
        stk_set_enable(stk, 1);
    else
        enable = true;

    if (stk8baxx_reg_write(stk, STK8BAXX_REG_OFSTX, offset[0]))
    {
        error = -1;
        goto exit;
    }

    if (stk8baxx_reg_write(stk, STK8BAXX_REG_OFSTY, offset[1]))
    {
        error = -1;
        goto exit;
    }

    if (stk8baxx_reg_write(stk, STK8BAXX_REG_OFSTZ, offset[2]))
    {
        error = -1;
        goto exit;
    }

exit:

    if (!enable)
        stk_set_enable(stk, 0);

    return error;
}

/**
 * @brief: Verify offset.
 *          Read register of STK8BAXX_REG_OFSTx, then check data are the same as
 *          what we wrote or not.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] offset: offset value to compare with the value in register
 *
 * @return: Success or fail
 *          0: Success
 *          STK_K_FAIL_I2C: I2C error
 *          STK_K_FAIL_WRITE_OFSET: offset value not the same as the value in
 *                                  register
 */
static int stk_verify_offset(struct stk8baxx_data *stk, u8 offset[3])
{
    int axis;
    u8 offset_from_reg[3] = {0, 0, 0};

    if (stk_get_offset(stk, offset_from_reg))
        return STK_K_FAIL_I2C;

    for (axis = 0; axis < 3; axis++)
    {
        if (offset_from_reg[axis] != offset[axis])
        {
            STK_ACC_ERR("set OFST failed! offset[%d]=%d, read from reg[%d]=%d",
                        axis, offset[axis], axis, offset_from_reg[axis]);
            atomic_set(&stk->cali_status, STK_K_FAIL_WRITE_OFST);
            return STK_K_FAIL_WRITE_OFST;
        }
    }

    return 0;
}

/**
 * @brief: Write calibration data to config file
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] offset: offset value
 * @param[in] status: status
 *                  STK_K_SUCCESS_FILE
 *
 * @return: Success or fail
 *          0: Success
 *          -1: Fail
 */
static int stk_write_cali_to_file(struct stk8baxx_data *stk,
                                  u8 offset[3], u8 status)
{
    char file_buf[STK_CALI_FILE_SIZE];
    memset(file_buf, 0, sizeof(file_buf));
    file_buf[0] = STK_CALI_VER0;
    file_buf[1] = STK_CALI_VER1;
    file_buf[3] = offset[0];
    file_buf[5] = offset[1];
    file_buf[7] = offset[2];
    file_buf[8] = status;
    file_buf[STK_CALI_FILE_SIZE - 2] = '\0';
    file_buf[STK_CALI_FILE_SIZE - 1] = STK_CALI_END;

    if (stk_write_to_file(stk, file_buf, STK_CALI_FILE_SIZE))
        return -1;

    return 0;
}

/**
 * @brief: Calibration action
 *          1. Calculate calibration data
 *          2. Write data to STK8BAXX_REG_OFSTx
 *          3. Check calibration well-done with chip register
 *          4. Write calibration data to file
 *          Pre-condition: Sensor be put on a flat plane, with +z face up.
 *
 * @param[in/out] stk: struct stk8baxx_data *
 * @param[in] delay_us: delay in usec
 *
 * @return: Success or fail
 *          0: Success
 *          STK_K_FAIL_I2C: I2C error
 *          STK_K_FAIL_WRITE_OFSET: offset value not the same as the value in
 *                                  register
 *          STK_K_FAIL_W_FILE: fail during writing cali to file
 */
static int stk_cali_do(struct stk8baxx_data *stk, int delay_us)
{
    int error = 0;
    int acc_ave[3] = {0, 0, 0};
    unsigned int delay_ms = delay_us / 1000;
    u8 offset[3] = {0, 0, 0};
    int acc_verify[3] = {0, 0, 0};
    const unsigned char verify_diff = stk->sensitivity / 10;
    int axis;
#ifdef STK_CHECK_CODE
    msleep(delay_ms * STK_CHECKCODE_IGNORE);
#endif /* STK_CHECK_CODE */
    stk_calculate_average(stk, delay_ms, STK_CALI_SAMPLE_NO, acc_ave);
    stk_align_offset_sensitivity(stk, acc_ave);

    for (axis = 0; axis < 3; axis++)
        offset[axis] = -acc_ave[axis];

    STK_ACC_LOG("New offset for XYZ: %d, %d, %d\n", acc_ave[0], acc_ave[1], acc_ave[2]);
    error = stk_set_offset(stk, offset);

    if (error)
        return STK_K_FAIL_I2C;

    /* Read register, then check OFSTx are the same as we wrote or not */
    error = stk_verify_offset(stk, offset);

    if (error)
        return error;

    /* verify cali */
    stk_calculate_average(stk, delay_ms, 3, acc_verify);

    if (verify_diff < abs(acc_verify[0]) || verify_diff < abs(acc_verify[1])
        || verify_diff < abs(acc_verify[2]))
    {
        STK_ACC_ERR("Check data x:%d, y:%d, z:%d. Check failed!",
                    acc_verify[0], acc_verify[1], acc_verify[2]);
        return STK_K_FAIL_VERIFY_CALI;
    }

    /* write cali to file */
    error = stk_write_cali_to_file(stk, offset, STK_K_SUCCESS_FILE);

    if (error)
    {
        STK_ACC_ERR("failed to stk_write_cali_to_file, error=%d", error);
        return STK_K_FAIL_W_FILE;
    }

    atomic_set(&stk->cali_status, STK_K_SUCCESS_FILE);
    return 0;
}

/**
 * @brief: Set calibration
 *          1. Change delay to 8000msec
 *          2. Reset offset value by trigger OFST_RST
 *          3. Calibration action
 *          4. Change delay value back
 *
 * @param[in/out] stk: struct stk8baxx_data *
 */
static void stk_set_cali(struct stk8baxx_data *stk)
{
    int error = 0;
    bool enable;
    int org_delay_us, real_delay_us;
    atomic_set(&stk->cali_status, STK_K_RUNNING);
    org_delay_us = stk_get_delay(stk);
    /* Use several samples (with ODR:125) for calibration data base */
    error = stk_set_delay(stk, 8000);

    if (error)
    {
        STK_ACC_ERR("failed to stk_set_delay, error=%d", error);
        atomic_set(&stk->cali_status, STK_K_FAIL_I2C);
        return;
    }

    real_delay_us = stk_get_delay(stk);

    /* SW reset before getting calibration data base */
    if (atomic_read(&stk->enabled))
    {
        enable = true;
        stk_set_enable(stk, 0);
    }
    else
        enable = false;

    stk_set_enable(stk, 1);
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_OFSTCOMP1,
                              STK8BAXX_OFSTCOMP1_OFST_RST);

    if (error)
    {
        atomic_set(&stk->cali_status, STK_K_FAIL_I2C);
        goto exit_for_OFST_RST;
    }

    /* Action for calibration */
    error = stk_cali_do(stk, real_delay_us);

    if (error)
    {
        STK_ACC_ERR("failed to stk_cali_do, error=%d", error);
        atomic_set(&stk->cali_status, error);
        goto exit_for_OFST_RST;
    }

    STK_ACC_LOG("successful calibration");
exit_for_OFST_RST:

    if (!enable)
        stk_set_enable(stk, 0);

    stk_set_delay(stk, org_delay_us);
}

/**
 * @brief: Reset calibration
 *          1. Reset offset value by trigger OFST_RST
 *          2. Calibration action
 */
static void stk_reset_cali(struct stk8baxx_data *stk)
{
    stk8baxx_reg_write(stk, STK8BAXX_REG_OFSTCOMP1,
            STK8BAXX_OFSTCOMP1_OFST_RST);
    atomic_set(&stk->cali_status, STK_K_NO_CALI);
    memset(stk->cali_sw, 0x0, sizeof(stk->cali_sw));
}

/**
 * @brief: Get power status
 *          Send 0 or 1 to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in] attr: struct device_attribute *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t enable_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    char en;
    en = atomic_read(&stk->enabled);
    return scnprintf(buf, PAGE_SIZE, "%d\n", en);
}

/**
 * @brief: Set power status
 *          Get 0 or 1 from userspace, then set stk8baxx power status.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t enable_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct stk8baxx_data *stk = stk_data;
    unsigned int data;
    int error;
    error = kstrtouint(buf, 10, &data);

    if (error)
    {
        STK_ACC_ERR("kstrtoul failed, error=%d", error);
        return error;
    }

    if ((1 == data) || (0 == data))
        stk_set_enable(stk, data);
    else
        STK_ACC_ERR("invalid argument, en=%d", data);

    return count;
}

/**
 * @brief: Get accel data
 *          Send accel data to userspce.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t value_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;

    stk_read_accel_data(stk);
    return scnprintf(buf, PAGE_SIZE, "%hd %hd %hd\n",
                     stk->xyz[0], stk->xyz[1], stk->xyz[2]);
}

/**
 * @brief: Get delay value in usec
 *          Send delay in usec to userspce.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t delay_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    return scnprintf(buf, PAGE_SIZE, "%lld\n", (long long)stk_get_delay(stk) * 1000);
}

/**
 * @brief: Set delay value in usec
 *          Get delay value in usec from userspace, then write to register.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t delay_store(struct device_driver *ddri, const char *buf, size_t count)
{
#if 0
    struct stk8baxx_data *stk = stk_data;
    long long data;
    int error;
    error = kstrtoll(buf, 10, &data);

    if (error)
    {
        STK_ACC_ERR("kstrtoul failed, error=%d", error);
        return error;
    }
    //stk_set_delay(stk, (int)(data / 1000));
    do_div(data, 1000);
    stk_set_delay(stk, (int)data);
    #endif
    return count;
}

/**
 * @brief: Get calibration status
 *          Send calibration status to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t cali_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    u8 cali[3] = {0, 0, 0};

    if (STK_K_RUNNING != atomic_read(&stk->cali_status))
        stk_get_cali(stk, cali);

    return scnprintf(buf, PAGE_SIZE, "0x%02X\n", atomic_read(&stk->cali_status));
}

/**
 * @brief: Trigger to calculate calibration data
 *          Get 1 from userspace, then start to calculate calibration data.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t cali_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct stk8baxx_data *stk = stk_data;

    if (sysfs_streq(buf, "1"))
        stk_set_cali(stk);
    else
    {
        STK_ACC_ERR("invalid value %d", *buf);
        return -EINVAL;
    }

    return count;
}

/**
 * @brief: Get offset value
 *          Send X/Y/Z offset value to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t offset_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    u8 offset[3] = {0, 0, 0};
    stk_get_offset(stk, offset);
    return scnprintf(buf, PAGE_SIZE, "0x%X 0x%X 0x%X\n",
                     offset[0], offset[1], offset[2]);
}

/**
 * @brief: Set offset value
 *          Get X/Y/Z offset value from userspace, then write to register.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t offset_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct stk8baxx_data *stk = stk_data;
    char *token[10];
    u8 r_offset[3];
    int error, data, i;

    for (i = 0; i < 3; i++)
        token[i] = strsep((char **)&buf, " ");

    error = kstrtoint(token[0], 16, &data);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    r_offset[0] = (u8)data;
    error = kstrtoint(token[1], 16, &data);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    r_offset[1] = (u8)data;
    error = kstrtoint(token[2], 16, &data);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    r_offset[2] = (u8)data;
    STK_ACC_LOG("offset=0x%X, 0x%X, 0x%X", r_offset[0], r_offset[1], r_offset[2]);
    stk_set_offset(stk, r_offset);
    return count;
}

/**
 * @brief: Register writting
 *          Get address and content from userspace, then write to register.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t send_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct stk8baxx_data *stk = stk_data;
    char *token[10];
    int addr, cmd, error, i;
    bool enable = false;

    for (i = 0; i < 2; i++)
        token[i] = strsep((char **)&buf, " ");

    error = kstrtoint(token[0], 16, &addr);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    error = kstrtoint(token[1], 16, &cmd);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    STK_ACC_LOG("write reg[0x%X]=0x%X", addr, cmd);

    if (!atomic_read(&stk->enabled))
        stk_set_enable(stk, 1);
    else
        enable = true;

    if (stk8baxx_reg_write(stk, (u8)addr, (u8)cmd))
    {
        error = -1;
        goto exit;
    }

exit:

    if (!enable)
        stk_set_enable(stk, 0);

    if (error)
        return -1;

    return count;
}

/**
 * @brief: Read stk8baxx_data.recv(from stk_recv_store), then send to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t recv_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    return scnprintf(buf, PAGE_SIZE, "0x%X\n", atomic_read(&stk->recv));
}

/**
 * @brief: Get the read address from userspace, then store the result to
 *          stk8baxx_data.recv.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t recv_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct stk8baxx_data *stk = stk_data;
    int addr, error;
    u8 data = 0;
    bool enable = false;
    error = kstrtoint(buf, 16, &addr);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    if (!atomic_read(&stk->enabled))
        stk_set_enable(stk, 1);
    else
        enable = true;

    if (stk8baxx_reg_read(stk, (u8)addr, 0, &data))
    {
        error = -1;
        goto exit;
    }

    atomic_set(&stk->recv, data);
    STK_ACC_LOG("read reg[0x%X]=0x%X", addr, data);
exit:

    if (!enable)
        stk_set_enable(stk, 0);

    if (error)
        return -1;

    return count;
}

/**
 * @brief: Get direction from userspace, then store the result to register
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t layout_store(struct device_driver *ddri, const char *buf, size_t count)
{
    int direction = 0;
    int ret = 0;

    ret = kstrtoint(buf, 10, &direction);
    printk("%s--direction = %d--ret = %d\n",__func__, direction, ret);
    if (ret == 0) {
        if (hwmsen_get_convert(direction, &stk_data->cvt))
            STK_ACC_LOG("fail to set direction\n");
    }

    return count;
}

/**
 * @brief: Read all register value, then send result to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t allreg_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    int result;
    result = stk_show_all_reg(stk, buf);

    if (0 >  result)
        return result;

    return (ssize_t)result;
}

/**
 * @brief: Check PID, then send chip number to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t chipinfo_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;

    if (STK8BA53_ID == stk->pid)
        return scnprintf(buf, PAGE_SIZE, "stk8ba53\n");
    else if (STK8BA50_R_ID == stk->pid)
        return scnprintf(buf, PAGE_SIZE, "stk8ba50-r\n");

    return scnprintf(buf, PAGE_SIZE, "unknown\n");
}

/**
 * @brief: Get direction, then send direction to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t layout_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    return scnprintf(buf, PAGE_SIZE, "direction = %d\n[%+2d %+2d %+2d]\n[%+2d %+2d %+2d]\n",
                       stk->hw.direction, stk->cvt.sign[0],stk->cvt.sign[1], stk->cvt.sign[2],
                       stk->cvt.map[0], stk->cvt.map[1], stk->cvt.map[2]);
}

#ifdef STK_FIR
/**
 * @brief: Get FIR parameter, then send to userspace.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 *
 * @return: ssize_t
 */
static ssize_t firlen_show(struct device_driver *ddri, char *buf)
{
    struct stk8baxx_data *stk = stk_data;
    int len = atomic_read(&stk->fir_len);

    if (len)
    {
        STK_ACC_LOG("FIR count=%2d, idx=%2d", stk->fir.count, stk->fir.idx);
        STK_ACC_LOG("sum = [\t%d \t%d \t%d]", stk->fir.sum[0], stk->fir.sum[1], stk->fir.sum[2]);
        STK_ACC_LOG("avg = [\t%d \t%d \t%d]", stk->fir.sum[0] / len, stk->fir.sum[1] / len, stk->fir.sum[2] / len);
    }

    return scnprintf(buf, PAGE_SIZE, "%d\n", len);
}

/**
 * @brief: Get FIR length from userspace, then write to stk8baxx_data.fir_len.
 *
 * @param[in] ddri: struct device_driver *
 * @param[in/out] buf: char *
 * @param[in] count: size_t
 *
 * @return: ssize_t
 */
static ssize_t firlen_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct stk8baxx_data *stk = stk_data;
    int firlen, error;
    error = kstrtoint(buf, 10, &firlen);

    if (error)
    {
        STK_ACC_ERR("kstrtoint failed, error=%d", error);
        return error;
    }

    if (STK_FIR_LEN_MAX < firlen)
        STK_ACC_ERR("maximum FIR length is %d", STK_FIR_LEN_MAX);
    else
    {
        memset(&stk->fir, 0, sizeof(struct data_fir));
        atomic_set(&stk->fir_len, firlen);
    }

    return count;
}
#endif /* STK_FIR */

/*****************************************
 *** show_chip_orientation
 *****************************************/
static ssize_t orientation_show(struct device_driver *ptDevDrv, char *pbBuf)
{
	ssize_t		  _tLength = 0;

	ACC_LOG("[%s] default direction: %d\n", __func__, stk_data->hw.direction);

	_tLength = snprintf(pbBuf, PAGE_SIZE, "default direction = %d\n", stk_data->hw.direction);

	return _tLength;
}

/*****************************************
 *** store_chip_orientation
 *****************************************/
static ssize_t orientation_store(struct device_driver *ptDevDrv, const char *pbBuf, size_t tCount)
{
	int _nDirection = 0;
	int ret = 0;
	ret = kstrtoint(pbBuf, 10, &_nDirection);
	if (ret == 0) {
        
#if SENSOR_DIRECTION_DEBUG
        stk_data->hw.direction = _nDirection;

#endif    
		if (hwmsen_get_convert(_nDirection, &stk_data->cvt))
			ACC_PR_ERR("ERR: fail to set direction\n");
	}
	return tCount;
}
#if 0
static DRIVER_ATTR(enable, 0664, stk_enable_show, stk_enable_store);
static DRIVER_ATTR(value, 0444, stk_value_show, NULL);
static DRIVER_ATTR(delay, 0664, stk_delay_show, stk_delay_store);
static DRIVER_ATTR(cali, 0664, stk_cali_show, stk_cali_store);
static DRIVER_ATTR(offset, 0664, stk_offset_show, stk_offset_store);
static DRIVER_ATTR(send, 0220, NULL, stk_send_store);
static DRIVER_ATTR(recv, 0664, stk_recv_show, stk_recv_store);
static DRIVER_ATTR(allreg, 0444, stk_allreg_show, NULL);
static DRIVER_ATTR(chipinfo, 0444, stk_chipinfo_show, NULL);
static DRIVER_ATTR(layout, 0664, stk_layout_show, stk_layout_store);
static DRIVER_ATTR(orientation, 0664, show_chip_orientation, store_chip_orientation);
#endif

static DRIVER_ATTR_RW(enable);
static DRIVER_ATTR_RO(value);
static DRIVER_ATTR_RW(delay);
static DRIVER_ATTR_RW(cali);
static DRIVER_ATTR_RW(offset);
static DRIVER_ATTR_WO(send);
static DRIVER_ATTR_RW(recv);
static DRIVER_ATTR_RO(allreg);
static DRIVER_ATTR_RO(chipinfo);
static DRIVER_ATTR_RW(layout);
static DRIVER_ATTR_RW(orientation);

#ifdef STK_FIR
    static DRIVER_ATTR_RW(firlen);
#endif /* STK_FIR */

static struct driver_attribute *stk_attr_list[] =
{
    &driver_attr_enable,
    &driver_attr_value,
    &driver_attr_delay,
    &driver_attr_cali,
    &driver_attr_offset,
    &driver_attr_send,
    &driver_attr_recv,
    &driver_attr_allreg,
    &driver_attr_chipinfo,
    &driver_attr_layout,
    &driver_attr_orientation,
#ifdef STK_FIR
    &driver_attr_firlen,
#endif /* STK_FIR */
};

//tct need chipinfo which disaplay in mmi^M
static ssize_t gsensor_show(struct device* dev,struct device_attribute *attr, char *buf)
{
     struct stk8baxx_data *stk = stk_data;
 
     if (STK8BA53_ID == stk->pid)
         return scnprintf(buf, PAGE_SIZE, "stk8ba53\n");
     else if (STK8BA50_R_ID == stk->pid)
         return scnprintf(buf, PAGE_SIZE, "stk8ba50-r\n");
 
     return scnprintf(buf, PAGE_SIZE, "unknown\n");

}
extern struct device* get_deviceinfo_dev(void);
static DEVICE_ATTR(gsensor, S_IWUSR | S_IRUGO, gsensor_show, NULL);
static int create_chipinfo_node(void)
{
       struct device * chipinfo;
       int err=0;

       chipinfo=get_deviceinfo_dev();
       err=device_create_file(chipinfo, &dev_attr_gsensor);
       if (err){
               ACC_PR_ERR("Failed to create device file(%s)!\n", dev_attr_gsensor.attr.name);
               return -ENODEV;
       }
       return err;
}


/**
 * @brief:
 */
static int stk_create_attr(struct device_driver *driver)
{
    int i, error = 0;
    int num = (int)(sizeof(stk_attr_list) / sizeof(stk_attr_list[0]));

    if (NULL == driver)
        return -EINVAL;

    for (i = 0; i < num; i++)
    {
        error = driver_create_file(driver, stk_attr_list[i]);

        if (error)
        {
            STK_ACC_ERR("driver_create_file (%s) = %d",
                        stk_attr_list[i]->attr.name, error);
            break;
        }
    }

    return error;
}

/**
 * @brief:
 */
static void stk_create_attr_exit(struct device_driver *driver)
{
    int i;
    int num = (int)(sizeof(stk_attr_list) / sizeof(stk_attr_list[0]));

    if (NULL == driver)
        return;

    for (i = 0; i < num; i++)
        driver_remove_file(driver, stk_attr_list[i]);
}

/**
 * @brief:
 */
static void stk_report_accel_data(struct stk8baxx_data *stk)
{
#ifdef STK_CHECK_CODE

    if ((STK_CCSTATUS_XYSIM == stk->cc_status)
        || ((STK_CHECKCODE_IGNORE + 6) > stk->cc_count))
        return;

#endif /* STK_CHECK_CODE */
   // ACC_LOG("x:%d, y:%d, z:%d", stk->xyz[0], stk->xyz[1], stk->xyz[2]);
}
#ifdef STK_SIG_MOTION

//#ifdef INTERRUPT_MODE
/**
 * @brief:
 */

static void stk_reset_latched_int(struct stk8baxx_data *stk)
{
    u8 data = 0;

    if (stk8baxx_reg_read(stk, STK8BAXX_REG_INTCFG2, 0, &data))
        return;

    if (stk8baxx_reg_write(stk, STK8BAXX_REG_INTCFG2, (data | STK8BAXX_INTCFG2_INT_RST)))
        return;
}


/**
 * @brief: Queue work list.
 *              ???????
 *          5. Enable IRQ.
 *
 * @param[in] work: struct work_struct *
 */
static void stk_data_irq_work(struct work_struct *work)
{
    struct stk8baxx_data *stk =
        container_of(work, struct stk8baxx_data, alldata_work);
    //bool enable = true;
    u8 data = 0;

    STK_ACC_LOG("stk_data_irq_work start");
    msleep(30);
    /*
    if (!atomic_read(&stk->enabled))
    {
        stk_set_enable(stk, 1);
        enable = false;
    }
    */
    /* ANY MOTION */
    if (!stk8baxx_reg_read(stk, STK8BAXX_REG_INTSTS1, 0, &data))
    {
        if (STK8BAXX_INTSTS1_ANY_MOT_STS & data)
        {
            STK_ACC_LOG("Get trigger for any motion interrupt");
            step_notify(TYPE_SIGNIFICANT);
            stk_reset_latched_int(stk);
        }
    }
    /*
    if (!enable)
        stk_set_enable(stk, 0);
    */
    enable_irq(stk->irq1);
}


/**
 * @brief: IRQ handler. This function will be trigger after receiving IRQ.
 *          1. Disable IRQ without waiting.
 *          2. Send work to quque.
 *
 * @param[in] irq: irq number
 * @param[in] data: void *
 *
 * @return: IRQ_HANDLED
 */
static irqreturn_t stk_all_data_handler(int irq, void *data)
{
    //struct stk8baxx_data *stk = data;
    struct stk8baxx_data *stk = stk_data;
    disable_irq_nosync(stk->irq1);
    queue_work(stk->alldata_workqueue, &stk->alldata_work);
    STK_ACC_LOG("GSE into stk_all_data_handler");
    return IRQ_HANDLED;
}


/**
 * @brief:Set interrupt sign trigger function
 */
int stk_interrupt_mode_setup(void)
{
    int error = 0;
    struct device_node *stk_node;
    u32 ints[2] = {0, 0};
    struct stk8baxx_data *stk = stk_data;

    stk_node = of_find_compatible_node(NULL,NULL,"mediatek,GSE_1-eint");
    if (stk_node)
    {
        of_property_read_u32_array(stk_node, "interrupts", ints, ARRAY_SIZE(ints));
        stk->interrupt_int1_pin = ints[0];
        /*
        gpio_direction_input(stk->interrupt_int1_pin);
        error = gpio_to_irq(stk->interrupt_int1_pin);

        if (0 > error)
        {
            STK_ACC_ERR("gpio_to_irq failed");
            error = -EINVAL;
            goto exit_gpio_request_1;
        }

        stk->irq1 = error;
        */
        stk->irq1 = irq_of_parse_and_map(stk_node, 0);
        STK_ACC_LOG("irq #=%d, interrupt pin=%d", stk->irq1, stk->interrupt_int1_pin);

        error = request_irq(stk->irq1, stk_all_data_handler,
                                        IRQF_TRIGGER_LOW, "mediatek,GSE_1-eint", NULL);

        if (0 > error)
        {
            STK_ACC_ERR("request_any_context_irq(%d) failed for %d", stk->irq1, error);
            goto exit_gpio_to_irq_1;
        }

        //make IRQ can wake up system
        enable_irq_wake(stk->irq1);
    }
    else
    {
        STK_ACC_ERR("Null device node of ACCEL-eint");
        return -EINVAL;
    }

    return 0;
exit_gpio_to_irq_1:
    gpio_free(stk->interrupt_int1_pin);
/*
exit_gpio_request_1:
    cancel_work_sync(&stk->alldata_work);
    destroy_workqueue(stk->alldata_workqueue);
exit_err:
*/
    return error;
}

static int stk8baxx_sig_motion_set_enable(struct i2c_client *client, bool enable)
{
	int res = 0;
	struct stk8baxx_data *stk = i2c_get_clientdata(client);

	if (enable) {
		stk->sig_motion_enable = 1;
	//	enable_irq(stk->irq1);
		STK_ACC_LOG("set pick up enable\n");		
	} else {
		stk->sig_motion_enable = 0;
	//	disable_irq_nosync(stk->irq1);
		STK_ACC_LOG("set pick up disable\n");		
	}

	res = i2c_smbus_write_byte_data(client, STK8BAXX_REG_SIGMOT2, enable ? 0x04 : 0x00);
	if (res < 0) {
		STK_ACC_ERR("failed to write reg 0x%x, error=0x%x\n", STK8BAXX_REG_SIGMOT2, res);
		return res;
	}

	ACC_LOG("OK\n");
	return 0;
}


int stk8baxx_set_acc_interrupter(bool flag)
{
    int res =0;
    struct stk8baxx_data *stk = stk_data;

    printk("stk8baxx_set_acc_interrupter en =%d\n",flag);

    res = stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND);
    if(res)
    {
        STK_ACC_ERR("stk_change_power_mode fail\n");
        return res;
    }

    if(flag)
    {
        res = stk8baxx_sig_motion_set_enable(stk->client, true);
        if(res !=0)
        {
            printk("SetIntEnable faild\n");
        }
        res = stk_change_power_mode(stk, STK8BAXX_PWMD_NORMAL);
        if(res)
        {
            STK_ACC_ERR("stk_change_power_mode fail\n");
            return res;
        }
        msleep(500);
    }
    else
    {
        res = stk8baxx_sig_motion_set_enable(stk->client, false);
        if(res !=0)
        {
            printk("SetIntEnable faild\n");
        }
        if(atomic_read(&stk->enabled))
        {
            res = stk_change_power_mode(stk, STK8BAXX_PWMD_NORMAL);
            if(res)
            {
                STK_ACC_ERR("stk_change_power_mode fail\n");
                return res;
            }
        }
    }

    return res;
}
#endif

/**
 * @brief:
 */
/*
static void stk_interrupt_mode_exit(struct stk8baxx_data *stk)
{
    free_irq(stk->irq1, stk);
    gpio_free(stk->interrupt_int1_pin);
    cancel_work_sync(&stk->alldata_work);
    destroy_workqueue(stk->alldata_workqueue);
}*/
//#else /* no INTERRUPT_MODE */
/**
 * @brief: Queue delayed_work list.
 *          1. ??????.
 *          2. Enable IRQ.
 *
 * @param[in] work: struct work_struct *
 */
/*
static void stk_sig_irq_delay_work(struct work_struct *work)
{
    struct stk8baxx_data *stk =
        container_of(work, struct stk8baxx_data, sig_delaywork.work);
    u8 data = 0;

    if (!stk8baxx_reg_read(stk, STK8BAXX_REG_INTSTS1, 0, &data))
    {
        if (STK8BAXX_INTSTS1_SIG_MOT_STS & data)
        {
            STK_ACC_LOG("Get trigger for sig motion");
        }
    }

    enable_irq(stk->irq1);
}
*/
/**
 * @brief: IRQ handler. This function will be trigger after receiving IRQ.
 *          1. Disable IRQ without waiting.
 *          2. Send delayed_work to quque.
 *
 * @param[in] irq: irq number
 * @param[in] data: void *
 *
 * @return: IRQ_HANDLED
 */
/*
static irqreturn_t stk_sig_handler(int irq, void *data)
{
    struct stk8baxx_data *stk = data;
    disable_irq_nosync(irq);
    schedule_delayed_work(&stk->sig_delaywork, 0);
    return IRQ_HANDLED;
}
*/
/**
 * @brief: Queue delayed_work list.
 *              ???????
 *
 * @param[in] work: struct work_struct *
 */
static void stk_accel_delay_work(struct work_struct *work)
{
    struct stk8baxx_data *stk =
        container_of(work, struct stk8baxx_data, accel_delaywork.work);
    bool enable = true;

    if (!atomic_read(&stk->enabled))
    {
        stk_set_enable(stk, 1);
        enable = false;
    }

    stk_read_accel_data(stk);
    stk_report_accel_data(stk);

    if (!enable)
        stk_set_enable(stk, 0);
}

/**
 * @brief: This function will send delayed_work to queue.
 *          This function will be called regularly with period:
 *          stk8baxx_data.poll_delay.
 *
 * @param[in] timer: struct hrtimer *
 *
 * @return: HRTIMER_RESTART.
 */
static enum hrtimer_restart stk_accel_timer_func(struct hrtimer *timer)
{
    struct stk8baxx_data *stk =
        container_of(timer, struct stk8baxx_data, accel_timer);
    schedule_delayed_work(&stk->accel_delaywork, 0);
    hrtimer_forward_now(&stk->accel_timer, stk->poll_delay);
    return HRTIMER_RESTART;
}

static int stk_polling_mode_setup(struct stk8baxx_data *stk)
{

    /* polling accel data */
    INIT_DELAYED_WORK(&stk->accel_delaywork, stk_accel_delay_work);
    hrtimer_init(&stk->accel_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    stk->poll_delay = ns_to_ktime(
                          STK8BAXX_SAMPLE_TIME[0]
                          * NSEC_PER_USEC);
    stk->accel_timer.function = stk_accel_timer_func;
    return 0;

}

/**
 * @brief:
 */
static void stk_polling_mode_exit(struct stk8baxx_data *stk)
{
    hrtimer_try_to_cancel(&stk->accel_timer);
    cancel_delayed_work_sync(&stk->accel_delaywork);
    free_irq(stk->irq1, stk);
    gpio_free(stk->interrupt_int1_pin);
    cancel_delayed_work_sync(&stk->sig_delaywork);
}
//#endif /* INTERRUPT_MODE */

/**
 * @brief: stk8baxx register initialize
 *
 * @param[in/out] stk: struct stk8baxx_data *
 *
 * @return: Success or fail.
 *          0: Success
 *          others: Fail
 */
static int stk_reg_init(struct stk8baxx_data *stk)
{
    int error = 0;
    /* SW reset */
    error = stk_sw_reset(stk);

    if (error)
        return error;

    ///* SUSPEND */
    //stk_set_enable(stk, 0);
    /* ID46: Low-power -> Suspend -> Normal */
    error = stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND);

    if (error)
        return error;

    error = stk_change_power_mode(stk, STK8BAXX_PWMD_NORMAL);

    if (error)
        return error;

    atomic_set(&stk->enabled, 1);
    /* INT1, push-pull, active high. */

//regs inital for SIG_MOTION interrupt	
#ifdef STK_SIG_MOTION

    error = stk8baxx_reg_write(stk, STK8BAXX_REG_INTCFG1,
                                  STK8BAXX_INTCFG1_INT1_ACTIVE_L | STK8BAXX_INTCFG1_INT1_OD_PUSHPULL);
	if (error)
        return error;

    /* map sig motion interrupt to int1 */
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_INTMAP1, STK8BAXX_INTMAP1_ANYMOT2INT1);
    if (error)
        return error;
	
    /*
     * latch int
     * In interrupt mode + significant mode, both of them share the same INT.
     * Set latched to make sure we can get SIG data(SIG_MOT_STS) before signal fall down.
     * Read SIG flow:
     * Get INT --> check INTSTS1.SIG_MOT_STS status -> INTCFG2.INT_RST(relese all latched INT)
     * In latch mode, echo interrupt(SIT_MOT_STS) will cause all INT(INT1)
     * rising up.
     */
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_INTCFG2, STK8BAXX_INTCFG2_LATCHED);
    if (error)
        return error;

    /* enable new data interrupt for sig motion */
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_INTEN1, STK8BAXX_INTEN1_SLP_EN_Z);
    if (error)
        return error;
    /* SLOPE THRESHOLD */
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_SLOPETHD, STK8BAXX_SLOPETHD_DEF);
    if (error)
        return error;
	
#endif

 
    /* According to STK_DEF_DYNAMIC_RANGE */
    error = stk_range_selection(stk, STK8BAXX_RANGESEL_DEF);

    if (error)
        return error;

    /* ODR */
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_BWSEL, STK8BAXX_BWSEL_INIT_ODR);

    if (error)
        return error;

    /* i2c watchdog enable */
    error = stk8baxx_reg_write(stk, STK8BAXX_REG_INTFCFG,
                              STK8BAXX_INTFCFG_I2C_WDT_EN);

    if (error)
        return error;

    /* SUSPEND */
    //stk_set_enable(stk, 0);
    error = stk_change_power_mode(stk, STK8BAXX_PWMD_SUSPEND);

    if (error)
        return error;

    atomic_set(&stk->enabled, 0);
    return 0;
}

/**
 * @brief
 */
static int stk_fops_open(struct inode *inode, struct file *file)
{
    file->private_data = stk_data->client;

    if (NULL == file->private_data)
    {
        STK_ACC_ERR("Null point for i2c_client");
        return -EINVAL;
    }

    return nonseekable_open(inode, file);
}

/**
 * @brief
 */
static int stk_fops_release(struct inode *inode, struct file *file)
{
    file->private_data = NULL;
    return 0;
}

/**
 * @brief:
 */

//u8 xyzCurrent[3] = {0, 0, 0};
static long stk_fops_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct i2c_client *client;
    struct stk8baxx_data *stk;
    char strbuf[STK_BUFSIZE];
    void __user *data;
    int error = 0, i = 0;
    bool enable = true;
//    struct GSENSOR_VECTOR3D sensor_vector;
    struct SENSOR_DATA sensor_data;
    u8 xyz[3] = {0, 0, 0};
    int accelData[3] = {0,0,0};
    int accelOffset[3] = {0,0,0};
    int sum_axis[3] = {0, 0, 0} ;
   client = (struct i2c_client *)file->private_data;
   stk = i2c_get_clientdata(client); 

#if 0
    if (_IOC_DIR(cmd) & _IOC_READ)
        error = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        error = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));

    if (error)
    {
        STK_ACC_ERR("access error: %08X, (%2d, %2d)", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
        return -EFAULT;
    }
#endif
    switch (cmd)
    {
        case GSENSOR_IOCTL_INIT:
            //stk_reg_init(stk);
            break;

 #if 0   
        case GSENSOR_IOCTL_READ_CHIPINFO:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            sprintf(strbuf, "STK832x Chip");

            if (copy_to_user(data, strbuf, strlen(strbuf) + 1))
            {
                error = -EFAULT;
                break;
            }

            break;
#endif
        case GSENSOR_IOCTL_READ_SENSORDATA:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            if (!atomic_read(&stk->enabled))
            {
                stk_set_enable(stk, 1);
                enable = false;
            }
            for(i = 0 ;i < 13;i++)
            {
             stk_read_accel_data(stk);
             if(i > 2)
              {
                 sum_axis[0] += stk->xyz[0];
                 sum_axis[1] += stk->xyz[1];
                 sum_axis[2] += stk->xyz[2];
              }
            }
            
            for(i = 0 ;i < 3;i++)
               stk->xyz[i] = sum_axis[i]/10;
            
            if (!enable)
                stk_set_enable(stk, 0);

            stk->xyz[STK_AXIS_X] += stk->cvt.sign[STK_AXIS_X] * stk->cali_sw[stk->cvt.map[STK_AXIS_X]];
            stk->xyz[STK_AXIS_Y] += stk->cvt.sign[STK_AXIS_Y] * stk->cali_sw[stk->cvt.map[STK_AXIS_Y]];
            stk->xyz[STK_AXIS_Z] += stk->cvt.sign[STK_AXIS_Z] * stk->cali_sw[stk->cvt.map[STK_AXIS_Z]];

            accelData[stk->cvt.map[STK_AXIS_X]] = (int)(stk->cvt.sign[STK_AXIS_X] * stk->xyz[STK_AXIS_X]);
            accelData[stk->cvt.map[STK_AXIS_Y]] = (int)(stk->cvt.sign[STK_AXIS_Y] * stk->xyz[STK_AXIS_Y]);
            accelData[stk->cvt.map[STK_AXIS_Z]] = (int)(stk->cvt.sign[STK_AXIS_Z] * stk->xyz[STK_AXIS_Z]);

            if (abs(stk->cali_sw[STK_AXIS_Z]) > 325)
            {
                  accelData[stk->cvt.map[STK_AXIS_Z]] -= 512;
            }

            stk->xyz[STK_AXIS_X] = accelData[STK_AXIS_X] * GRAVITY_EARTH_1000 / stk->sensitivity;
            stk->xyz[STK_AXIS_Y] = accelData[STK_AXIS_Y] * GRAVITY_EARTH_1000 / stk->sensitivity;
            stk->xyz[STK_AXIS_Z] = accelData[STK_AXIS_Z] * GRAVITY_EARTH_1000 / stk->sensitivity;

            sprintf(strbuf, "%04x %04x %04x", stk->xyz[0], stk->xyz[1], stk->xyz[2]);

            if (copy_to_user(data, strbuf, strlen(strbuf) + 1))
            {
                error = -EFAULT;
                break;
            }

            break;
#if 0
        case GSENSOR_IOCTL_READ_OFFSET:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            stk_get_offset(stk, xyz);
            sensor_vector.x = (unsigned short)(xyz[0]);
            sensor_vector.y = (unsigned short)(xyz[1]);
            sensor_vector.z = (unsigned short)(xyz[2]);

            if (copy_to_user(data, &sensor_vector, sizeof(sensor_vector)))
            {
                error = -EFAULT;
                break;
            }

            break;

        case GSENSOR_IOCTL_READ_GAIN:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            stk_get_sensitivity(stk);
            sensor_vector.x = (unsigned short)(stk->sensitivity);
            sensor_vector.y = (unsigned short)(stk->sensitivity);
            sensor_vector.z = (unsigned short)(stk->sensitivity);

            if (copy_to_user(data, &sensor_vector, sizeof(sensor_vector)))
            {
                error = -EFAULT;
                break;
            }

            break;

        case GSENSOR_IOCTL_READ_RAW_DATA:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            if (!atomic_read(&stk->enabled))
            {
                stk_set_enable(stk, 1);
                enable = false;
            }

            stk_read_accel_rawdata(stk);

            if (!enable)
                stk_set_enable(stk, 0);

            sprintf(strbuf, "%04x %04x %04x", stk->xyz[0], stk->xyz[1], stk->xyz[2]);

            if (copy_to_user(data, strbuf, strlen(strbuf) + 1))
            {
                error = -EFAULT;
                break;
            }

            break;
#endif

        case GSENSOR_IOCTL_SET_CALI:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            if (copy_from_user(&sensor_data, data, sizeof(sensor_data)))
            {
                error = -EFAULT;
                break;
            }

            /* write cali to file */
            //error = stk_write_cali_to_file(stk, xyz, STK_K_SUCCESS_FILE);
            accelOffset[STK_AXIS_X] = sensor_data.x > 0 ? (sensor_data.x + 38) : (sensor_data.x - 38);
            accelOffset[STK_AXIS_Y] = sensor_data.y > 0 ? (sensor_data.y + 38) : (sensor_data.y - 38);
            accelOffset[STK_AXIS_Z] = sensor_data.z > 0 ? (sensor_data.z + 38) : (sensor_data.z - 38);

            accelOffset[STK_AXIS_X] = accelOffset[STK_AXIS_X] / 76;
            accelOffset[STK_AXIS_Y] = accelOffset[STK_AXIS_Y] / 76;
            accelOffset[STK_AXIS_Z] = accelOffset[STK_AXIS_Z] / 76;

            //Begin Added by weizhi.chen for Task 7142338 on 2018/11/22
            xyz[STK_AXIS_X] = (u8)(accelOffset[stk->cvt.map[STK_AXIS_X]] / stk->cvt.sign[STK_AXIS_X]);
            xyz[STK_AXIS_Y] = (u8)(accelOffset[stk->cvt.map[STK_AXIS_Y]] / stk->cvt.sign[STK_AXIS_Y]);
            xyz[STK_AXIS_Z] = (u8)(accelOffset[stk->cvt.map[STK_AXIS_Z]] / stk->cvt.sign[STK_AXIS_Z]);
            //End Added by weizhi.chen for Task 7142338 on 2018/11/22

            error = stk_set_offset(stk, xyz);
            if (error)
            {
                STK_ACC_ERR("failed to stk_write_cali_to_file, error=%d", error);
                error = -EFAULT;
                break;
            }
            atomic_set(&stk->cali_status, STK_K_SUCCESS_FILE);

            break;

        case GSENSOR_IOCTL_GET_CALI:
            data = (void __user *)arg;

            if (NULL == data)
            {
                error = -EINVAL;
                break;
            }

            //stk_get_cali(stk, xyz);
            stk_get_offset(stk, xyz);
            sensor_data.x = (unsigned short)(xyz[0] * GRAVITY_EARTH_1000 / stk->sensitivity);
            sensor_data.y = (unsigned short)(xyz[1] * GRAVITY_EARTH_1000 / stk->sensitivity);
            sensor_data.z = (unsigned short)(xyz[2] * GRAVITY_EARTH_1000 / stk->sensitivity);

            if (copy_to_user(data, &sensor_data, sizeof(sensor_data)))
            {
                error = -EFAULT;
                break;
            }

            break;

        case GSENSOR_IOCTL_CLR_CALI:
            /* write cali to file */
            //error = stk_write_cali_to_file(stk, xyz, STK_K_SUCCESS_FILE);
            stk8baxx_reg_write(stk, STK8BAXX_REG_OFSTCOMP1, STK8BAXX_OFSTCOMP1_OFST_RST);
            /*
            stk_get_offset(stk, xyzCurrent);
            error = stk_set_offset(stk, xyz);
            if (error)
            {
                STK_ACC_ERR("failed to stk_write_cali_to_file, error=%d", error);
                error = -EFAULT;
                break;
            }
            */
	msleep(20);
            break;

        default:
            STK_ACC_ERR("unknown IOCTL: 0x%08X", cmd);
            error = -ENOIOCTLCMD;
            break;
    }

    return error;
}

#ifdef CONFIG_COMPAT
/**
 * @brief:
 */
static long stk_fops_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return 0;

#if 0

    struct i2c_client *client = (struct i2c_client *)file->private_data;
    struct stk8baxx_data *stk = i2c_get_clientdata(client);
    long error = 0;
    void __user *arg32 = compat_ptr(arg);

    if (!file->f_op || !file->f_op->unlocked_ioctl)
        return -ENOTTY;

    switch (cmd)
    {
        case COMPAT_GSENSOR_IOCTL_INIT:
            stk_reg_init(stk);
            break;

        case COMPAT_GSENSOR_IOCTL_READ_CHIPINFO:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_CHIPINFO, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_READ_CHIPINFO failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_READ_SENSORDATA:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_SENSORDATA, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_READ_SENSORDATA failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_READ_OFFSET:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_OFFSET, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_READ_OFFSET failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_READ_GAIN:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_GAIN, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_READ_GAIN failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_READ_RAW_DATA:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_READ_RAW_DATA, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_READ_RAW_DATA failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_SET_CALI:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_SET_CALI, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_SET_CALI failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_GET_CALI:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_GET_CALI, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_GET_CALI failed.");
                return error;
            }

            break;

        case COMPAT_GSENSOR_IOCTL_CLR_CALI:
            if (NULL == arg32)
            {
                error = -EINVAL;
                break;
            }

            error = file->f_op->unlocked_ioctl(file, GSENSOR_IOCTL_CLR_CALI, (unsigned long)arg32);

            if (error)
            {
                STK_ACC_ERR("GSENSOR_IOCTL_CLR_CALI failed.");
                return error;
            }

            break;

        default:
            STK_ACC_ERR("unknown IOCTL: 0x%08X", cmd);
            error = -ENOIOCTLCMD;
            break;
    }

    return error;
    #endif
}
#endif

static struct file_operations stk_miscdevice_fops =
{
    .owner = THIS_MODULE,
    .open = stk_fops_open,
    .release = stk_fops_release,
    .unlocked_ioctl = stk_fops_unlocked_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = stk_fops_compat_ioctl,
#endif
};

static struct miscdevice stk_miscdevice =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "gsensor",
    .fops = &stk_miscdevice_fops,
};

/**
 * @brief: Open data rerport to HAL.
 *      refer: drivers/misc/mediatek/accelerometer/inc/accel.h
 */
static int gsensor_open_report_data(int open)
{
    STK_ACC_FUN();
    /* TODO. should queuq work to report event if  is_report_input_direct=true */
    return 0;
}

#ifdef CONFIG_CUSTOM_KERNEL_SENSORHUB
/**
 * @brief:
 */
static int stk_scp_setPowerMode(int enable)
{
    static bool gsensor_scp_en_status;
    static unsigned int gsensor_scp_en_map;
    int error;
    SCP_SENSOR_HUB_DATA stk_scp_sensor_hub_data;
    int len;

    if (en)
        gsensor_scp_en_map |= (1 << ID_ACCELEROMETER);
    else
        gsensor_scp_en_map &= ~(1 << ID_ACCELEROMETER);

    if (0 == gsensor_scp_en_map)
        enable = 0;
    else
        enable = 1;

    if (gsensor_scp_en_status != enable)
    {
        gsensor_scp_en_status = enable;
        stk_scp_sensor_hub_data.activate_req.sensorType = ID_ACCELEROMETER;
        stk_scp_sensor_hub_data.activate_req.action = SENSOR_HUB_ACTIVATE;
        stk_scp_sensor_hub_data.activate_req.enable = enable;
        len = sizeof(stk_scp_sensor_hub.activate_req);
        error = SCP_sensorHub_req_send(&stk_scp_sensor_hub_data, &len, 1);

        if (error)
        {
            STK_ACC_ERR("SCP_sensorHub_req_send fail");
            return error;
        }
    }

    return 0;
}

/**
 * @brief: Only enable not report event to HAL.
 *      refer: drivers/misc/mediatek/accelerometer/inc/accel.h
 */
static int scp_gsensor_enable_nodata(int en)
{
    struct stk8baxx_data *stk = stk_data;
    int error = 0;

    if (en == atomic_read(&stk->enabled))
        STK_ACC_LOG("Gsensor device have updated!");
    else
    {
        error = stk_scp_setPowerMode(en);

        if (error)
        {
            STK_ACC_ERR("failed");
            return -1;
        }

        atomic_set(&stk->enabled_for_acc, en ? 1 : 0);
        STK_ACC_LOG("enabled_for_acc is %d", stk->enabled_for_acc);
    }

    return 0;
}
#else /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
/**
 * @brief: Only enable not report event to HAL.
 *      refer: drivers/misc/mediatek/accelerometer/inc/accel.h
 */
static int gsensor_enable_nodata(int en)
{
    struct stk8baxx_data *stk = stk_data;

    if (en)
    {
        stk_set_enable(stk, 1);
        atomic_set(&stk->enabled_for_acc, 1);
    }
    else
    {
        stk_set_enable(stk, 0);
        atomic_set(&stk->enabled_for_acc, 0);
    }

    STK_ACC_LOG("enabled_for_acc is %d", en);
    return 0;
}
#endif /* CONFIG_CUSTOM_KERNEL_SENSORHUB */

/**
 * @brief:
 */
static int gsensor_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
    STK_ACC_FUN();
    return 0;
}

/**
 * @brief:
 */
static int gsensor_flush(void)
{
    STK_ACC_FUN();
    return acc_flush_report();
}

/**
 * @brief:
 */
static int gsensor_set_delay(u64 delay_ns)
{
#ifdef CONFIG_CUSTOM_KERNEL_SENSORHUB
    int error;
    SCP_SENSOR_HUB_DATA stk_scp_sensor_hub_data;
    int len;
#endif /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    //struct stk8baxx_data *stk = stk_data;
    //STK_ACC_LOG("delay= %d ms", (int)(delay_ns / 1000));
    do_div(delay_ns, 1000);
    STK_ACC_LOG("delay= %d us", (int)delay_ns);
#ifdef CONFIG_CUSTOM_KERNEL_SENSORHUB
    stk_scp_sensor_hub_data.set_delay_req.sensorType = ID_ACCELEROMETER;
    stk_scp_sensor_hub_data.set_delay_req.action = SENSOR_HUB_SET_DELAY;
    //stk_scp_sensor_hub_data.set_delay_req.delay = (int)(delay_ns / 1000 / 1000);
    stk_scp_sensor_hub_data.set_delay_req.delay = ((int)delay_ns) / 1000);
    len = sizeof(stk_scp_sensor_hub.set_delay_req);
    error = SCP_sensorHub_req_send(&stk_scp_sensor_hub, &len, 1);

    if (error)
    {
        STK_ACC_ERR("SCP_sensorHub_req_send fail");
        return error;
    }

#endif /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    //stk_set_delay(stk, (int)(delay_ns / 1000));
//modified begin by qiuyang.duan@tcl.com 	2019/12/03 defect:8463227 [Performance][DOU][PIO]Battery is losing fast when idle(14% in 16 hours)  kernel can not suspend
    //stk_set_delay(stk, (int)delay_ns);
//modified end by qiuyang.duan@tcl.com 	2019/12/03 defect:8463227 [Performance][DOU][PIO]Battery is losing fast when idle(14% in 16 hours)  kernel can not suspend   
    return 0;
}

/**
 * @brief:
 */
static int gsensor_get_data(int *x, int *y, int *z, int *status)
{
#ifdef CONFIG_CUSTOM_KERNEL_SENSORHUB
    int error;
    SCP_SENSOR_HUB_DATA stk_scp_sensor_hub_data;
    int len;
    stk_scp_sensor_hub_data.get_data_req.sensorType = ID_ACCELEROMETER;
    stk_scp_sensor_hub_data.get_data_req.action = SENSOR_HUB_GET_DATA;
    len = sizeof(stk_scp_sensor_hub.get_data_req);
    error = SCP_sensorHub_req_send(&stk_scp_sensor_hub, &len, 1);

    if (error)
    {
        STK_ACC_ERR("SCP_sensorHub_req_send fail");
        return error;
    }

    if (ID_ACCELEROMETER != stk_scp_sensor_hub_data.get_data_rsp.sensorType
        || SENSOR_HUB_GET_DATA != stk_scp_sensor_hub_data.get_data_rsp.action
        || 0 != stk_scp_sensor_hub_data.get_data_rsp.errCode)
    {
        STK_ACC_ERR("error : %d\n", stk_scp_sensor_hub_data.get_data_rsp.errCode);
        return stk_scp_sensor_hub_data.get_data_rsp.errCode;
    }

    *x = (int)stk_scp_sensor_hub_data.get_data_rsp.int16_Data[0] * GRAVITY_EARTH_1000 / 1000;
    *y = (int)stk_scp_sensor_hub_data.get_data_rsp.int16_Data[1] * GRAVITY_EARTH_1000 / 1000;
    *z = (int)stk_scp_sensor_hub_data.get_data_rsp.int16_Data[2] * GRAVITY_EARTH_1000 / 1000;
    STK_ACC_LOG("x = %d, y = %d, z = %d\n", *x, *y, *z);
#else /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    struct stk8baxx_data *stk = stk_data;
    bool enable = false;
    char buff[256];
    int error;
    int accelData[STK_AXES_NUM] = {0};

    int reg;
    int len = 0;
    u8 data = 0;
    char show_buffer[1024];

    if(stk->suspend)
    {
        STK_ACC_ERR("[%s] power is suspend \n",__func__);
        return 0;
    }

    if (!atomic_read(&stk->enabled))
        stk_set_enable(stk, 1);
    else
        enable = true;

    stk_read_accel_data(stk);


    for (reg = 0; reg <= 0x3F; reg++)
    {
        if (stk8baxx_reg_read(stk, reg, 0, &data))
        {
            len = -1;
            STK_ACC_ERR("stk8ba tct read 0x%2x error\n",reg);
            printk("stk8ba tct %s\n ", show_buffer);
            return -1;
        }

        if (0 >= (PAGE_SIZE - len))
        {
            STK_ACC_ERR("print string out of PAGE_SIZE");
            printk("stk8ba tct %s\n ", show_buffer);
            return -1;
        }
        len += scnprintf(show_buffer + len, PAGE_SIZE - len,
                         "[0x%2X]=0x%2X, ", reg, data);
    }


//       printk("tct [0x%2X]=0x%2X, ", reg, data);
//       printk("stk8ba tct %s\n ", show_buffer);

//    ACC_LOG("raw data x:%d, y:%d, z:%d", stk->xyz[0], stk->xyz[1], stk->xyz[2]);
#if SENSOR_DIRECTION_DEBUG
                    
    stk->xyz[STK_AXIS_X] += stk_data->cvt.sign[STK_AXIS_X] * stk->cali_sw[stk_data->cvt.map[STK_AXIS_X]];
    stk->xyz[STK_AXIS_Y] += stk_data->cvt.sign[STK_AXIS_Y] * stk->cali_sw[stk_data->cvt.map[STK_AXIS_Y]];
    stk->xyz[STK_AXIS_Z] += stk_data->cvt.sign[STK_AXIS_Z] * stk->cali_sw[stk_data->cvt.map[STK_AXIS_Z]];
    
    accelData[stk_data->cvt.map[STK_AXIS_X]] = (int)(stk_data->cvt.sign[STK_AXIS_X] * stk->xyz[STK_AXIS_X]);
    accelData[stk_data->cvt.map[STK_AXIS_Y]] = (int)(stk_data->cvt.sign[STK_AXIS_Y] * stk->xyz[STK_AXIS_Y]);
    accelData[stk_data->cvt.map[STK_AXIS_Z]] = (int)(stk_data->cvt.sign[STK_AXIS_Z] * stk->xyz[STK_AXIS_Z]);    
#else		

    stk->xyz[STK_AXIS_X] += stk->cvt.sign[STK_AXIS_X] * stk->cali_sw[stk->cvt.map[STK_AXIS_X]];
    stk->xyz[STK_AXIS_Y] += stk->cvt.sign[STK_AXIS_Y] * stk->cali_sw[stk->cvt.map[STK_AXIS_Y]];
    stk->xyz[STK_AXIS_Z] += stk->cvt.sign[STK_AXIS_Z] * stk->cali_sw[stk->cvt.map[STK_AXIS_Z]];
    
    accelData[stk->cvt.map[STK_AXIS_X]] = (int)(stk->cvt.sign[STK_AXIS_X] * stk->xyz[STK_AXIS_X]);
    accelData[stk->cvt.map[STK_AXIS_Y]] = (int)(stk->cvt.sign[STK_AXIS_Y] * stk->xyz[STK_AXIS_Y]);
    accelData[stk->cvt.map[STK_AXIS_Z]] = (int)(stk->cvt.sign[STK_AXIS_Z] * stk->xyz[STK_AXIS_Z]);
#endif



    //ACC_LOG("map data x:%d, y:%d, z:%d", accelData[STK_AXIS_X], accelData[STK_AXIS_Y], accelData[STK_AXIS_Z]);

    if (abs(stk->cali_sw[STK_AXIS_Z]) > 325)
    {
        accelData[stk->cvt.map[STK_AXIS_Z]] -= 512;
    }

    accelData[STK_AXIS_X] = accelData[STK_AXIS_X] * GRAVITY_EARTH_1000 / stk->sensitivity;
    accelData[STK_AXIS_Y] = accelData[STK_AXIS_Y] * GRAVITY_EARTH_1000 / stk->sensitivity;
    accelData[STK_AXIS_Z] = accelData[STK_AXIS_Z] * GRAVITY_EARTH_1000 / stk->sensitivity;

    if (!enable)
        stk_set_enable(stk, 0);

    sprintf(buff, "%04x %04x %04x", accelData[STK_AXIS_X], accelData[STK_AXIS_Y], accelData[STK_AXIS_Z]);
    error = sscanf(buff, "%x %x %x", x, y, z);
    if (3 != error)
    {
        STK_ACC_ERR("Invalid argument");
        return -EINVAL;
    }

#endif /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    *status = SENSOR_STATUS_ACCURACY_MEDIUM;
    return 0;
}

static int stk_readCalibration(int *dat)
{
    struct stk8baxx_data *stk = stk_data;

    STK_ACC_LOG("ori x:%d, y:%d, z:%d", stk->cali_sw[STK_AXIS_X], stk->cali_sw[STK_AXIS_Y], stk->cali_sw[STK_AXIS_Z]);
    dat[STK_AXIS_X] = stk->cali_sw[STK_AXIS_X];
    dat[STK_AXIS_Y] = stk->cali_sw[STK_AXIS_Y];
    dat[STK_AXIS_Z] = stk->cali_sw[STK_AXIS_Z];

    return 0;
}

static int stk_writeCalibration(int *dat)
{
    struct stk8baxx_data *stk = stk_data;
    int err = 0;
    int cali[STK_AXES_NUM];

    err = stk_readCalibration(cali);

    STK_ACC_LOG("raw cali_sw[%d][%d][%d] dat[%d][%d][%d]",
            cali[0], cali[1], cali[2], dat[0], dat[1], dat[2]);

    cali[STK_AXIS_X] += dat[STK_AXIS_X];
    cali[STK_AXIS_Y] += dat[STK_AXIS_Y];
    cali[STK_AXIS_Z] += dat[STK_AXIS_Z];

    stk->cali_sw[STK_AXIS_X] = cali[STK_AXIS_X];
    stk->cali_sw[STK_AXIS_Y] = cali[STK_AXIS_Y];
    stk->cali_sw[STK_AXIS_Z] = cali[STK_AXIS_Z];

    STK_ACC_LOG("new cali_sw[%d][%d][%d]",
            stk->cali_sw[0], stk->cali_sw[1], stk->cali_sw[2]);

    mdelay(1);

    return err;
}

static int stk_factory_enable_sensor(bool enable, int64_t sample_ms)
{
    int en = (true == enable ? 1 : 0);
#ifdef CONFIG_CUSTOM_KERNEL_SENSORHUB

    if (scp_gsensor_enable_nodata(en))
#else /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    if (gsensor_enable_nodata(en))
#endif /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    {
        STK_ACC_ERR("enable sensor failed");
        return -1;
    }

    return 0;
}

static int stk_factory_get_data(int32_t data[3], int *status)
{
    return gsensor_get_data(&data[0], &data[1], &data[2], status);
}

static int stk_factory_get_raw_data(int32_t data[3])
{
    struct stk8baxx_data *stk = stk_data;

    stk_read_accel_rawdata(stk);
    data[0] = (int32_t)stk->xyz[0];
    data[1] = (int32_t)stk->xyz[1];
    data[2] = (int32_t)stk->xyz[2];
    return 0;
}

static int stk_factory_enable_cali(void)
{
    struct stk8baxx_data *stk = stk_data;

    stk_set_cali(stk);

    return 0;
}

static int stk_factory_clear_cali(void)
{
    struct stk8baxx_data *stk = stk_data;

    stk_reset_cali(stk);

    return 0;
}

static int stk_factory_set_cali(int32_t data[3])
{
    int error = 0;
    struct stk8baxx_data *stk = stk_data;
    int cali[3] = {0, 0, 0};
    u8 xyz[3] = {0, 0, 0};

    atomic_set(&stk->cali_status, STK_K_RUNNING);
    cali[0] = data[0] * stk->sensitivity / GRAVITY_EARTH_1000;
    cali[1] = data[1] * stk->sensitivity / GRAVITY_EARTH_1000;
    cali[2] = data[2] * stk->sensitivity / GRAVITY_EARTH_1000;

    STK_ACC_LOG("new x:%d, y:%d, z:%d", cali[0], cali[1], cali[2]);
    xyz[0] = (u8)cali[0];
    xyz[1] = (u8)cali[1];
    xyz[2] = (u8)cali[2];
    /* write cali to file */
    error = stk_write_cali_to_file(stk, xyz, STK_K_SUCCESS_FILE);

    if (error)
    {
        STK_ACC_ERR("failed to stk_write_cali_to_file, error=%d", error);
        return -1;
    }
    
    error = stk_writeCalibration(cali);
    if (error)
    {
        STK_ACC_ERR("stk_writeCalibration failed!");
        return -1;
    }
    atomic_set(&stk->cali_status, STK_K_SUCCESS_FILE);

    return 0;
}

static int stk_factory_get_cali(int32_t data[3])
{
    struct stk8baxx_data *stk = stk_data;

#if 1
    data[0] = (int32_t)(stk->cali_sw[0] * GRAVITY_EARTH_1000 / stk->sensitivity);
    data[1] = (int32_t)(stk->cali_sw[1] * GRAVITY_EARTH_1000 / stk->sensitivity);
    data[2] = (int32_t)(stk->cali_sw[2] * GRAVITY_EARTH_1000 / stk->sensitivity);
#else
    u8 cali[3] = {0, 0, 0};

    if (STK_K_RUNNING != atomic_read(&stk->cali_status))
    {
        stk_get_cali(stk, cali);
        data[0] = (int32_t)(cali[0] * GRAVITY_EARTH_1000 / stk->sensitivity);
        data[1] = (int32_t)(cali[1] * GRAVITY_EARTH_1000 / stk->sensitivity);
        data[2] = (int32_t)(cali[2] * GRAVITY_EARTH_1000 / stk->sensitivity);
    }
    else
    {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
    }
#endif
    STK_ACC_LOG("x:%d, y:%d, z:%d", data[0], data[1], data[2]);

    return 0;
}

static int stk_factory_do_self_test(void)
{
    return 0;
}

static struct accel_factory_fops stk_factory_fops =
{
    .enable_sensor = stk_factory_enable_sensor,
    .get_data = stk_factory_get_data,
    .get_raw_data = stk_factory_get_raw_data,
    .enable_calibration = stk_factory_enable_cali,
    .clear_cali = stk_factory_clear_cali,
    .set_cali = stk_factory_set_cali,
    .get_cali = stk_factory_get_cali,
    .do_self_test = stk_factory_do_self_test,
};

static struct accel_factory_public stk_factory_device =
{
    .gain = 1,
    .sensitivity = 1,
    .fops = &stk_factory_fops,
};

/**
 * @brief: Proble function for i2c_driver.
 *
 * @param[in] client: struct i2c_client *
 * @param[in] id: struct i2c_device_id *
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
static int stk8baxx_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int error = 0;
    struct stk8baxx_data *stk;
    struct acc_control_path stk_acc_control_path = {0};
    struct acc_data_path stk_acc_data_path = {0};
    struct device_node *node = NULL;
    STK_ACC_LOG("driver version:%s", STK_ACC_DRIVER_VERSION);
    /* kzalloc: allocate memory and set to zero. */
    stk = kzalloc(sizeof(struct stk8baxx_data), GFP_KERNEL);

    if (!stk)
    {
        STK_ACC_ERR("memory allocation error");
        return -ENOMEM;
    }

    node = of_find_compatible_node(NULL, NULL, "mediatek,stk8ba53_dir");
    error = get_accel_dts_func(node, &stk->hw);
    //error = get_accel_dts_func(client->dev.of_node, &stk->hw);
    if (0 > error)
    {
        STK_ACC_ERR("Dts info fail");
        goto err_free_mem;
    }

    client->addr = STK8BAXX_SLAVE_ADDR;

    /* direction */
    error = hwmsen_get_convert(stk->hw.direction, &stk->cvt);

    if (error)
    {
        STK_ACC_ERR("invalid direction: %d", stk->hw.direction);
        goto err_free_mem;
    }
	

    stk->client = client;
    i2c_set_clientdata(client, stk);
    mutex_init(&stk->reg_lock);
    stk_data_initialize(stk);
//    STK_ACC_LOG("qiuyang.duan error = %d",error);
    if (stk_get_pid(stk))
    {
        error = -1;
        goto err_mutex_destory;
    }
//    STK_ACC_LOG("qiuyang.duan2 error = %d",error);
    STK_ACC_LOG("PID 0x%x", stk->pid);

    if (STK8BA50_R_ID != stk->pid
            && STK8BA53_ID != stk->pid)
    {
        error = -EINVAL;
        STK_ACC_LOG("chip id not match");
        goto err_mutex_destory;
    }

#ifdef STK_SIG_MOTION

    stk->alldata_workqueue = create_singlethread_workqueue("stk_int1_wq");

    if (stk->alldata_workqueue)
        INIT_WORK(&stk->alldata_work, stk_data_irq_work);
    else
    {
        STK_ACC_ERR("create_singlethread_workqueue error");
        error = -EPERM;
        goto exit_work_req_error;
    }

/*

    error = stk_interrupt_mode_setup(stk);

    if (0 > error)
        goto err_mutex_destory;

*/
#endif
    stk_data = stk;
    error = stk_polling_mode_setup(stk);
    if (0 > error)
        goto err_mutex_destory;

    if (stk_reg_init(stk))
    {
        STK_ACC_ERR("stk8baxx initialization failed");
        goto exit_stk_init_error;
    }

    error = misc_register(&stk_miscdevice);

    if (error)
    {
        STK_ACC_ERR("stk8baxx misc_register failed");
        goto exit_misc_error;
    }

    error = stk_create_attr(&stk_acc_init_info.platform_diver_addr->driver);

    if (error)
    {
        STK_ACC_ERR("stk_create_attr failed");
        goto exit_misc_error;
    }

    error = create_chipinfo_node();

    if (error)
    {
        STK_ACC_ERR("create chipinfo node failed");
        goto exit_misc_error;
    }
    /* MTK Android usage +++ */
    stk_acc_control_path.is_use_common_factory = false;
    /* factory */
    /*
    error = accel_factory_device_register(&stk_factory_device);
    if (error)
    {
        STK_ACC_ERR("accel_factory_device_register failed");
        goto exit_misc_error;
    }
    */
    stk_acc_control_path.open_report_data = gsensor_open_report_data;
#ifdef CONFIG_CUSTOM_KERNEL_SENSORHUB
    stk_acc_control_path.enable_nodata = scp_gsensor_enable_nodata;
    stk_acc_control_path.is_support_batch = stk_acc_hw->is_batch_support;
#else /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    stk_acc_control_path.enable_nodata = gsensor_enable_nodata;
    stk_acc_control_path.is_support_batch = false;
#endif /* CONFIG_CUSTOM_KERNEL_SENSORHUB */
    stk_acc_control_path.batch = gsensor_batch;
    stk_acc_control_path.flush = gsensor_flush;
    stk_acc_control_path.set_delay = gsensor_set_delay;
    stk_acc_control_path.is_report_input_direct = false;
    error = acc_register_control_path(&stk_acc_control_path);

    if (error)
    {
        STK_ACC_ERR("acc_register_control_path fail");
        goto exit_register_control_path;
    }

    stk_acc_data_path.get_data = gsensor_get_data;
    stk_acc_data_path.vender_div = 1000;
    error = acc_register_data_path(&stk_acc_data_path);

    if (error)
    {
        STK_ACC_ERR("acc_register_data_path fail");
        goto exit_register_control_path;
    }
#if 0
    error = batch_register_support_info(ID_ACCELEROMETER, stk_acc_control_path.is_support_batch, 102, 0);

    if (error)
    {
        STK_ACC_ERR("batch_register_support_info fail");
        goto exit_register_control_path;
    }
#endif
    /* MTK Android usage --- */
    stk8baxx_init_flag = 0;

    stk->suspend = false;
    STK_ACC_LOG("Success");
    return 0;
exit_register_control_path:
    stk_create_attr_exit(&stk_acc_init_info.platform_diver_addr->driver);
exit_misc_error:
    misc_deregister(&stk_miscdevice);
exit_stk_init_error:
#ifdef INTERRUPT_MODE
    stk_interrupt_mode_exit(stk);
#else /* no INTERRUPT_MODE */
    stk_polling_mode_exit(stk);
#endif /* INTERRUPT_MODE */
exit_work_req_error:
    cancel_work_sync(&stk->alldata_work);
    destroy_workqueue(stk->alldata_workqueue);
//    STK_ACC_ERR("qiuyang.duan4,%d",error);    
err_mutex_destory:
    mutex_destroy(&stk->reg_lock);
//    STK_ACC_ERR("qiuyang.duan5,%d",error);
err_free_mem:
    kfree(stk);
    stk8baxx_init_flag = -1;
//    STK_ACC_ERR("qiuyang.duan6,%d",error);    
    return error;
}

/**
 * @brief
 */
static int stk8baxx_i2c_remove(struct i2c_client *client)
{
    struct stk8baxx_data *stk = i2c_get_clientdata(client);
    accel_factory_device_deregister(&stk_factory_device);
    stk_create_attr_exit(&stk_acc_init_info.platform_diver_addr->driver);
    misc_deregister(&stk_miscdevice);
#ifdef INTERRUPT_MODE
    stk_interrupt_mode_exit(stk);
#else /* no INTERRUPT_MODE */
    stk_polling_mode_exit(stk);
#endif /* INTERRUPT_MODE */
    mutex_destroy(&stk->reg_lock);
    kfree(stk);
    return 0;
}

#ifdef CONFIG_PM_SLEEP
/**
 * @brief:
 */
static int stk_suspend(struct device *dev)
{
    struct i2c_client *client = to_i2c_client(dev);
    struct stk8baxx_data *stk = i2c_get_clientdata(client);

    if (NULL == stk)
    {
        STK_ACC_ERR("Null point to find stk8baxx_data");
        return -EINVAL;
    }

    #ifdef STK_SIG_MOTION
    if(stk->sig_motion_enable == 1)
        return 0;
    #endif

    if (STK8BAXX_PWMD_SUSPEND != stk->power_mode)
    {
        u8 val = 0;

        if (stk8baxx_reg_read(stk, STK8BAXX_REG_POWMODE, 0, &val))
            return -EINVAL;

        val &= STK8BAXX_PWMD_SLP_MASK;
        if (stk8baxx_reg_write(stk, STK8BAXX_REG_POWMODE, (val | STK8BAXX_PWMD_SUSPEND)))
            return -EINVAL;
    }
    stk->suspend = true;
    return 0;
}

/**
 * @brief:
 */
static int stk_resume(struct device *dev)
{
    struct i2c_client *client = to_i2c_client(dev);
    struct stk8baxx_data *stk = i2c_get_clientdata(client);

    if (NULL == stk)
    {
        STK_ACC_ERR("Null point to find stk8baxx_data");
        return -EINVAL;
    }

    #ifdef STK_SIG_MOTION
    if(stk->sig_motion_enable == 1)
        return 0;
    #endif

    if (STK8BAXX_PWMD_SUSPEND != stk->power_mode)
    {
        u8 val = 0;

        if (stk8baxx_reg_read(stk, STK8BAXX_REG_POWMODE, 0, &val))
            return -EINVAL;

        val &= STK8BAXX_PWMD_SLP_MASK;
        if (stk8baxx_reg_write(stk, STK8BAXX_REG_POWMODE, (val | stk->power_mode)))
            return -EINVAL;
    }
    stk->suspend = false;
    return 0;
}
#endif /* CONFIG_PM_SLEEP */

static const struct i2c_device_id stk8baxx_i2c_id[] =
{
    {STK_ACC_DEV_NAME, 0},
    {}
};
MODULE_DEVICE_TABLE(i2c, stk8baxx_i2c_id);

static const struct of_device_id stk_acc_match[] =
{
    {.compatible = "mediatek,gsensor_stk8ba53"},
    {},
};
MODULE_DEVICE_TABLE(i2c, stk_acc_match);

#ifdef CONFIG_PM_SLEEP
static const struct dev_pm_ops stk_i2c_pm_ops =
{
    SET_SYSTEM_SLEEP_PM_OPS(stk_suspend, stk_resume)
};
#endif /* CONFIG_PM_SLEEP */

static struct i2c_driver stk8baxx_i2c_driver =
{
    .probe          = stk8baxx_i2c_probe,
    .remove         = stk8baxx_i2c_remove,
    .id_table       = stk8baxx_i2c_id,
    .driver         = {
        .name               = STK_ACC_DEV_NAME,
#ifdef CONFIG_PM_SLEEP
        .pm                 = &stk_i2c_pm_ops,
#endif /* CONFIG_PM_SLEEP */
        .of_match_table     = stk_acc_match,
    },
};

/**
 * @brief:
 *
 * @return: Success or fail
 *          0: Success
 *          others: Fail
 */
#ifdef CONFIG_MTK_ROBUST   
extern char gsensor_module_name[256];
#endif
static int stk_acc_init(void)
{
    STK_ACC_FUN();

    if (i2c_add_driver(&stk8baxx_i2c_driver))
    {
        STK_ACC_ERR("Add i2c driver fail");
        return -1;
    }
    
    if ( -1 == stk8baxx_init_flag)
    {
        STK_ACC_ERR("stk8baxx init error");
        return -1;
    }
#ifdef CONFIG_MTK_ROBUST  
    sprintf(gsensor_module_name,"STK8BA53:sensortek:NA");
#endif
    return 0;
}

/**
 * @brief:
 *
 * @return: Success
 *          0: Success
 */
static int stk_acc_uninit(void)
{
    i2c_del_driver(&stk8baxx_i2c_driver);
    return 0;
}

/**
 * @brief:
 *
 * @return: Success or fail.
 *          0: Success
 *          others: Fail
 */
static int __init stk8baxx_init(void)
{
    STK_ACC_FUN();

    acc_driver_add(&stk_acc_init_info);
#ifdef CONFIG_CUSTOM_KERNEL_ACCELEROMETER_MODULE
    success_Flag = true;
#endif /* CONFIG_CUSTOM_KERNEL_ACCELEROMETER_MODULE */
    return 0;
}

static void __exit stk8baxx_exit(void)
{
    STK_ACC_FUN();
#ifdef CONFIG_CUSTOM_KERNEL_ACCELEROMETER_MODULE
    success_Flag = false;
#endif /* CONFIG_CUSTOM_KERNEL_ACCELEROMETER_MODULE */
}

module_init(stk8baxx_init);
module_exit(stk8baxx_exit);

MODULE_AUTHOR("Sensortek");
MODULE_DESCRIPTION("stk8baxx 3-Axis accelerometer driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(STK_ACC_DRIVER_VERSION);
