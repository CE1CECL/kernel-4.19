
#ifndef TOUCHPANEL_H__
#define TOUCHPANEL_H__

#include "tpd_bl_chip.h"
#include "tpd.h"
#include <linux/firmware.h>

#define BLESTECH_TP_LOG
#ifdef BLESTECH_TP_LOG
#define BF_TP_LOG(fmt,arg...)   do{printk("<betterlife_tp>[%s:%d]"fmt"\n",__func__, __LINE__, ##arg);}while(0)
#else
#define BF_TP_LOG(fmt,arg...)   do{}while(0)
#endif

#define TPD_OK  0
#define TPD_MAX_RESET_COUNT 3
#define TPD_RESET_ISSUE_WORKAROUND

#define GTP_GPIO_OUTPUT(pin, level) tpd_gpio_output(pin, level)

#define BL_KERNEL_VERSION   "betterlife_driver_version_v1.0"


#define BTL_SLAVE_ADDR  0x2C //0x70 //7bit i2c addr
#define BTL_FLASH_ADDR  0x2C //flash addr 

#if(TS_CHIP == BL8XXX_60)

#define MAX_POINT_NUM   2
#define BTL_FLASH_ID    0x20
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x8000
#define PJ_ID_OFFSET    0x7b5b

#define I2C_TRANSFER_WSIZE  (128)
#define I2C_TRANSFER_RSIZE  (128)
#define I2C_VERFIY_SIZE     (128)

#define FW_CHECKSUM_DELAY_TIME  100


enum BL8XXX_60_flash_cmd {

    READ_MAIN_CMD       = 0x02,
    ERASE_ALL_MAIN_CMD  = 0x04,
    ERASE_PAGE_MAIN_CMD = 0x06,
    WRITE_MAIN_CMD      = 0x08,
    RW_REGISTER_CMD     = 0x0a,
};

#elif(TS_CHIP == BL8XXX_61)

#define MAX_POINT_NUM   2
#define BTL_FLASH_ID    0x20
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x8000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE      (64)
#define I2C_TRANSFER_RSIZE      (64)
#define I2C_VERFIY_SIZE         (64)

#define FW_CHECKSUM_DELAY_TIME  100


enum BL8XXX_61_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD      = 0x09,
    RW_REGISTER_CMD         = 0x0a,
    READ_MAIN_CMD           = 0x0D,
    WRITE_MAIN_CMD          = 0x0F,
    WRITE_RAM_CMD           = 0x11,
    READ_RAM_CMD            = 0x12,
};

#elif(TS_CHIP == BL8XXX_63)

#define MAX_POINT_NUM   2
#define BTL_FLASH_ID    0x20
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0xc000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL8XXX_63_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD      = 0x09,
    RW_REGISTER_CMD         = 0x0a,
    READ_MAIN_CMD           = 0x0D,
    WRITE_MAIN_CMD          = 0x0F,
    WRITE_RAM_CMD           = 0x11,
    READ_RAM_CMD            = 0x12,
};
#elif(TS_CHIP == BL6XX0)

#define MAX_POINT_NUM   2
#define BTL_FLASH_ID    0x20
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x8000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL6XX0_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD      = 0x09,
    RW_REGISTER_CMD         = 0x0a,
    READ_MAIN_CMD           = 0x0D,
    WRITE_MAIN_CMD          = 0x0F,
    WRITE_RAM_CMD           = 0x11,
    READ_RAM_CMD            = 0x12,
};
#elif(TS_CHIP == BL6XX1)

#define MAX_POINT_NUM   2
#define BTL_FLASH_ID    0x21
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x8000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL6XX1_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD  = 0x09,
    RW_REGISTER_CMD     = 0x0a,
    READ_MAIN_CMD       = 0x0D,
    WRITE_MAIN_CMD      = 0x0F,
    WRITE_RAM_CMD       = 0x11,
    READ_RAM_CMD        = 0x12,
};

#elif(TS_CHIP == BL6XX3)

#define MAX_POINT_NUM   1
#define BTL_FLASH_ID    0x22
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x8000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL6XX3_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD  = 0x09,
    RW_REGISTER_CMD     = 0x0a,
    READ_MAIN_CMD       = 0x0D,
    WRITE_MAIN_CMD      = 0x0F,
    WRITE_RAM_CMD       = 0x11,
    READ_RAM_CMD        = 0x12,
};


#elif(TS_CHIP == BL7XX1)
#define MAX_POINT_NUM   10
#define BTL_FLASH_ID    0x41
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x10000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL7XX1_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD  = 0x09,
    RW_REGISTER_CMD     = 0x0a,
    READ_MAIN_CMD       = 0x0D,
    WRITE_MAIN_CMD      = 0x0F,
    WRITE_RAM_CMD       = 0x11,
    READ_RAM_CMD        = 0x12,
};
#elif(TS_CHIP == BL7XX3)
#define MAX_POINT_NUM   10
#define BTL_FLASH_ID    0x42
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0x10000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL7XX3_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD  = 0x09,
    RW_REGISTER_CMD     = 0x0a,
    READ_MAIN_CMD       = 0x0D,
    WRITE_MAIN_CMD      = 0x0F,
    WRITE_RAM_CMD       = 0x11,
    READ_RAM_CMD        = 0x12,
};
#elif(TS_CHIP == BL7XX0)
//#define MAX_POINT_NUM   5
#define MAX_POINT_NUM   10
#define BTL_FLASH_ID    0x40
#define BTL_NEED_RESET_PIN
#define MAX_FLASH_SIZE  0xc000
#define PJ_ID_OFFSET    0xcb

#define I2C_TRANSFER_WSIZE  (256)
#define I2C_TRANSFER_RSIZE  (256)
#define I2C_VERFIY_SIZE     (256)

#define FW_CHECKSUM_DELAY_TIME  250


enum BL7XX0_flash_cmd {

    ERASE_SECTOR_MAIN_CMD   = 0x06,
    ERASE_ALL_MAIN_CMD  = 0x09,
    RW_REGISTER_CMD     = 0x0a,
    READ_MAIN_CMD       = 0x0D,
    WRITE_MAIN_CMD      = 0x0F,
    WRITE_RAM_CMD       = 0x11,
    READ_RAM_CMD        = 0x12,
};

#endif

enum fw_reg {

    WORK_MODE_REG       = 0x00,
    CHECKSUM_REG        = 0x3f,
    CHECKSUM_CAL_REG    = 0x8a,
    AC_REG              = 0x8b,
    RESOLUTION_REG      = 0x98,
    LPM_REG             = 0xa5,
    PROXIMITY_REG       = 0xb0,
    PROXIMITY_FLAG_REG  = 0xB1,
    CALL_REG            = 0xb2,
    PJ_ID_REG           = 0xb5,
    BL_PROTECT_REG      = 0xee,
};


enum work_mode {

    NORMAL_MODE     = 0x00,
    FACTORY_MODE    = 0x40,
};

enum lpm {

    ACTIVE_MODE     = 0x00,
    MONITOR_MODE    = 0x01,
    STANDBY_MODE    = 0x02,
    SLEEP_MODE      = 0x03,
    GESTURE_MODE    = 0x04,
};

enum checksum {

    CHECKSUM_READY  = 0x01,
    CHECKSUM_CAL    = 0xaa,
    CHECKSUM_ARG    = 0xba,
};

enum apk_cmd {

    WRITE_CMD       = 0x00,
    READ_CMD        = 0x01,
    WAKE_CMD        = 0x02,
    INTERRUPT_CMD   = 0x03,
    DRIVER_INFO     = 0x04,
    I2C_LOCK_CMD    = 0x05,
};

enum tpd_event {
    TPD_DOWN    = 0x00,
    TPD_UP      = 0x01,
    TPD_MOVE    = 0x02,
    TPD_RESERVED = 0x03,
};


enum update_mode {

    INIT_UPDATE     = 0x00,
    BIN_UPDATE      = 0x01,
};


/*#define CONFIG_TPD_ROTATE_270*//*if use,90/270/180 move to defconfig file*/
#define TPD_DELAY       (2*HZ/100)

//#define BTL_APK_SUPPORT
#define BTL_MULT_READ_REG

#define TPD_RES_X   720  //800
#define TPD_RES_Y   1440 //1280

/***********bl proximity sensor config********/
//#define BTL_CTP_PROXIMITY_SENSOR
#ifdef  BTL_CTP_PROXIMITY_SENSOR
#define BTL_PS_DEVICE       "btl_ps"
#define BTL_PS_INPUT_DEV    "proximity"

#define BTL_CTP_PROXIMITY_MODE_REG  0xB0
#define BTL_CTP_PROXIMITY_FLAG_REG  0xB1
#define BTL_CTP_PROXIMITY_NEAR      0xC0
#define BTL_CTP_PROXIMITY_LEAVE     0xE0

#define LTR_IOCTL_MAGIC         0x1C
#define LTR_IOCTL_GET_PFLAG     _IOR(LTR_IOCTL_MAGIC, 1, int)
#define LTR_IOCTL_GET_LFLAG     _IOR(LTR_IOCTL_MAGIC, 2, int)
#define LTR_IOCTL_SET_PFLAG     _IOW(LTR_IOCTL_MAGIC, 3, int)
#define LTR_IOCTL_SET_LFLAG     _IOW(LTR_IOCTL_MAGIC, 4, int)
#define LTR_IOCTL_GET_DATA      _IOW(LTR_IOCTL_MAGIC, 5, unsigned char)
#define LTR_IOCTL_GET_CHIPINFO  _IOR(LTR_IOCTL_MAGIC, 6, char)
#endif//BTL_CTP_PROXIMITY_SENSOR


/**********bl gesture value config************/
//#define   BTL_CTP_GESTURE_ENABLE
#ifdef  BTL_CTP_GESTURE_ENABLE

#define     BL_U_LEFT       0x0D
#define     BL_U_RIGHT      0x0E
#define     BL_U_UP         0x0F
#define     BL_U_DOWN       0x10
#define     BL_MOVE_UP      0x15
#define     BL_MOVE_DOWN    0x02
#define     BL_MOVE_LEFT    0x03
#define     BL_MOVE_RIGHT   0x04
#define     BL_O_GEST       0x07
#define     BL_E_GEST       0x08
#define     BL_M_GEST       0x06
#define     BL_W_GEST       0x05
#define     BL_S_GEST       0x0a
#define     BL_V_LEFT       0x11
#define     BL_V_RIGHT      0x12
#define     BL_V_UP         0x13
#define     BL_V_DOWN       0x14
#define     BL_C_GEST       0x09
#define     BL_Z_GEST       0x0b
#define     BL_DOUBLE_CLICK     0x0c

#define     BL_KEY_GESTURE_U    KEY_U
#define     BL_KEY_GESTURE_UP   KEY_UP
#define     BL_KEY_GESTURE_DOWN KEY_DOWN
#define     BL_KEY_GESTURE_LEFT KEY_LEFT
#define     BL_KEY_GESTURE_RIGHT    KEY_RIGHT
#define     BL_KEY_GESTURE_O    KEY_O
#define     BL_KEY_GESTURE_E    KEY_E
#define     BL_KEY_GESTURE_M    KEY_M
//#define   BL_KEY_GESTURE_L    KEY_L
#define     BL_KEY_GESTURE_W    KEY_W
#define     BL_KEY_GESTURE_S    KEY_S
#define     BL_KEY_GESTURE_V    KEY_V
#define     BL_KEY_GESTURE_C    KEY_C
#define     BL_KEY_GESTURE_Z    KEY_Z

#endif


/**************bl key config************/
#define TPD_HAVE_TOUCH_ID_BUTTON
#ifdef TPD_HAVE_TOUCH_ID_BUTTON
#define TPD_TOUCH_ID_BUTTON_X1  80
#define TPD_TOUCH_ID_BUTTON_Y1  900
#define TPD_TOUCH_ID_BUTTON1    KEY_MENU

#define TPD_TOUCH_ID_BUTTON_X2  240
#define TPD_TOUCH_ID_BUTTON_Y2  900
#define TPD_TOUCH_ID_BUTTON2    KEY_HOMEPAGE

#define TPD_TOUCH_ID_BUTTON_X3  400
#define TPD_TOUCH_ID_BUTTON_Y3  900
#define TPD_TOUCH_ID_BUTTON3    KEY_BACK
#endif//TPD_HAVE_TOUCH_ID_BUTTON


/***btl esd protect config********/
#define BTL_ESD_PROTECT
#ifdef  BTL_ESD_PROTECT
#define BTL_ESD_PROTECT_ADDR    0xAF
#define SWITCH_OFF      (0)
#define SWITCH_ON       (1)
#endif//BTL_ESD_PROTECT


/*****ac/usb power detect config*******/
//#define   BTL_AC_USB_POWER_DETECT_ENABLE
#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
#define POLLING_TIME    3000//ms

#define AC_POWER_ONLINE_STATUS_PATH "/sys/class/power_supply/ac/online"
#define USB_POWER_ONLINE_STATUS_PATH    "/sys/class/power_supply/usb/online"
#define BATTERY_CHARGE_STATUS_PATH  "/sys/class/power_supply/battery/status"
#define BATTERY_CAPACITY_LEVEL_PATH "/sys/class/power_supply/battery/capacity"
#endif

/*************bl pressure config***************/
//#define   BTL_CTP_PRESSURE
#ifdef  BTL_CTP_PRESSURE

#endif


/**********bl factory scan config*************/
//#define   BTL_FACTORY_SCAN_MODE
#ifdef  BTL_FACTORY_SCAN_MODE

#endif
//add by xucs start
/**********bl factory test enable*************/
#define BTL_FACTORY_TEST_EN
#ifdef BTL_FACTORY_TEST_EN
#define FILE_NAME_LENGTH               128
#define BTL_MAX_COMPATIBLE_TYPE        4
#endif
//add by xcus end

extern struct i2c_client* btl_i2c_client;
//add by xucs start
extern int bl_is_suspend;
extern int btl_log_level;
//add by xucs end
extern unsigned int btl_tpd_rst_gpio_number;
extern unsigned int btl_tpd_int_gpio_number;
extern unsigned int bl_touch_irq;
extern void bl_i2c_lock(int enable);
extern int btl_i2c_write(struct i2c_client* client, unsigned char* writebuf, int writelen);
extern int btl_i2c_read(struct i2c_client* client, unsigned char* writebuf, int writelen, unsigned char* readbuf, int readlen);
extern int btl_write_reg(struct i2c_client* client, u8 regaddr, u8 regvalue);
extern int btl_read_reg(struct i2c_client* client, u8 regaddr, u8* regvalue);
extern int btl_i2c_apk_write(struct i2c_client* client, unsigned char* writebuf, int writelen);
extern int btl_i2c_apk_read(struct i2c_client* client, unsigned char* readbuf, int readlen);
extern struct tpd_device* tpd;
//extern unsigned int tpd_rst_gpio_number;
extern void tpd_button(unsigned int x, unsigned int y, unsigned int down);
extern int bl_fw_upgrade_with_app_file(struct i2c_client* client, char* firmware_name);
extern int bl_get_chip_id(__u8* buf);

extern void bl_enable_irq(void);
extern void bl_disable_irq(void);
//add by xucs start
#if defined(BTL_ESD_PROTECT)
extern void bl_esd_switch(s32 on);
#endif
#if defined(BTL_FACTORY_TEST_EN)
extern int btl_test_init(void);
extern int btl_test_exit(void);
#endif
void btl_chip_reset(void);
//add by xucs end

#endif /* TOUCHPANEL_H__ */
