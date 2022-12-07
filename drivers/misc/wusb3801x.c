/************************************************************************
 *
 *  WILLSEMI TypeC Chipset Driver for Linux & Android.  
 *
 *
 * ######################################################################
 *
 *  Author: lei.huang (lhuang@sh-willsemi.com)
 *
 * Copyright (c) 2019, WillSemi Inc. All rights reserved.
 *
 ************************************************************************/


#include <linux/debugfs.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/param.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/printk.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>

#include <linux/miscdevice.h>
#include <linux/kthread.h>
#include <linux/kernel.h>

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/timer.h>
//#include <linux/wakelock.h>
#include <linux/of_irq.h>
#include <linux/pinctrl/consumer.h>
#include <linux/regmap.h>



/* Register Map */

#define WUSB3801_REG_VERSION_ID         0x01
#define WUSB3801_REG_CONTROL0           0x02
#define WUSB3801_REG_INTERRUPT          0x03
#define WUSB3801_REG_STATUS             0x04
#define WUSB3801_REG_CONTROL1           0x05
#define WUSB3801_REG_TEST0              0x06
#define WUSB3801_REG_TEST_01            0x07
#define WUSB3801_REG_TEST_02            0x08
#define WUSB3801_REG_TEST_03            0x09
#define WUSB3801_REG_TEST_04            0x0A
#define WUSB3801_REG_TEST_05            0x0B
#define WUSB3801_REG_TEST_06            0x0C
#define WUSB3801_REG_TEST_07            0x0D
#define WUSB3801_REG_TEST_08            0x0E
#define WUSB3801_REG_TEST_09            0x0F
#define WUSB3801_REG_TEST_0A            0x10
#define WUSB3801_REG_TEST_0B            0x11
#define WUSB3801_REG_TEST_0C            0x12
#define WUSB3801_REG_TEST_0D            0x13
#define WUSB3801_REG_TEST_0E            0x14
#define WUSB3801_REG_TEST_0F            0x15
#define WUSB3801_REG_TEST_10            0x16
#define WUSB3801_REG_TEST_11            0x17
#define WUSB3801_REG_TEST_12            0x18

#define WUSB3801X_FAIL    -1
#define WUSB3801X_SUCCESS 0

/*****************************************************************************
 *  Log
 ****************************************************************************/
#define LOG_NONE    0
#define LOG_ERR     1
#define LOG_DEBG    2
#define LOG_FULL    3

#define LOG_LEVEL LOG_FULL

#define wusb3801x_log(num, fmt, args...) \
do { \
        if (LOG_LEVEL >= (int)num) \
            pr_err(fmt, ##args); \
} while (0)

//-------------------------------------------------------------------
struct wusb3801x_chip{
    struct i2c_client *client;
    struct device *dev;
    struct regmap *regmap;
    
    struct mutex   i2c_lock;
};

struct wusb3801x_chip *chip = NULL;


static const struct regmap_config wusb3801x_regmap_config = {
    .reg_bits = 8,
    .val_bits = 8,
};

static int wusb3801x_write_reg(int reg, int value)
{
    int ret;
    
    mutex_lock(&chip->i2c_lock);
    ret = regmap_write(chip->regmap, reg, value);
    mutex_unlock(&chip->i2c_lock);
    if(ret < 0){
        wusb3801x_log(LOG_ERR, "[%s] i2c write error!\n", __func__);
        return WUSB3801X_FAIL;	
    }
    return WUSB3801X_SUCCESS; 
}

static int wusb3801x_read_reg(int reg)
{
    int ret;
    int value;    

    mutex_lock(&chip->i2c_lock);
    ret = regmap_read(chip->regmap, reg, &value);
    mutex_unlock(&chip->i2c_lock);

    if(ret < 0){
        wusb3801x_log(LOG_ERR, "[%s] i2c read error!\n", __func__);
        return WUSB3801X_FAIL;
    }
    return value;
}


static int test_cc_patch(void)
{		
	int rc;

	wusb3801x_write_reg(WUSB3801_REG_TEST_02, 0x82);	
	msleep(150);
	wusb3801x_write_reg(WUSB3801_REG_TEST_09, 0xC0);	
	msleep(150);
	rc = wusb3801x_read_reg(WUSB3801_REG_TEST0);
	msleep(10);
	wusb3801x_write_reg(WUSB3801_REG_TEST_09, 0x00);	
	msleep(10);
	wusb3801x_write_reg(WUSB3801_REG_TEST_02, 0x80);
	msleep(10);	
	wusb3801x_write_reg(WUSB3801_REG_TEST_02, 0x00);	

	wusb3801x_log(LOG_DEBG, "%s rc = [0x%02x] \n",__func__, rc);
	return rc;
}

static ssize_t show_typec_orient(struct device *dev, struct device_attribute *attr, char *buf)
{
    int ret = 0;

    ret = wusb3801x_read_reg(0x04);
    wusb3801x_log(LOG_DEBG, "[%s] REG0x04 = 0x%02x\n", __func__, ret);

    if(ret == 0)//unattached
    {
        ret = 1;
    }
    else if(((ret>>2)&0x07) == 0x02)//attachedsource
    {
        ret = 4;
    }
    else if(((ret>>2)&0x07) == 0x01)//attachedsink
    {
        ret = 6;
    }
    else if(((ret>>2)&0x07) == 0x03)//attachedsink
    {
        ret = 11;
    }
    else
        ret = 0;
    return sprintf(buf, "%d\n", ret);
}

static DEVICE_ATTR(typec_orient, 0444, show_typec_orient, NULL);


/* update by vendor */
static ssize_t show_fcc_status(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret = 0, rc = 0, result = 0;

	ret = wusb3801x_read_reg(0x04);
	wusb3801x_log(LOG_DEBG, "[%s] REG0x04 = 0x%02x\n", __func__, ret);

	if(ret <= 0)  result = 0;
	else if(((ret>>2) & 0x07) == 0x02)//source
	{
		rc = test_cc_patch();
		wusb3801x_log(LOG_DEBG, "[%s] rc = 0x%02x\n", __func__, rc);

		if(rc & 0x40) result = 2;//cc2
		else          result = 1;//cc1

	}
	else if(((ret>>2) & 0x07) == 0x01)//sink
	{
		if((ret & 0x03) == 1)  result = 1;
		else                   result = 2;
	}
	else result = 0;

	return sprintf(buf, "0x%02x\n", result);
}
/* end */

static DEVICE_ATTR(fcc_status, 0444, show_fcc_status, NULL);


//---------------------------------------------------------------------------
static void wusb3801x_create_device_node(struct device *dev)
{
    device_create_file(dev, &dev_attr_typec_orient);
    device_create_file(dev, &dev_attr_fcc_status);
}


static int wusb3801_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{
    int ret=0;
    chip = devm_kzalloc(&client->dev, sizeof(*chip), GFP_KERNEL);
    if (!chip) {
        wusb3801x_log(LOG_ERR,"[%s] : Unable to allocate memory\n", __func__);
        return -ENOMEM;
    }
    chip->client = client;
    chip->dev = &client->dev;
    chip->regmap = devm_regmap_init_i2c(client, &wusb3801x_regmap_config);
    if (IS_ERR(chip->regmap)) {
        wusb3801x_log(LOG_ERR, "[%s] Failed to allocate regmap!\n", __func__);
        devm_kfree(&client->dev, chip);
        return PTR_ERR(chip->regmap);
    }
    i2c_set_clientdata(client, chip);
    dev_set_drvdata(&(client->dev), chip);

    mutex_init(&chip->i2c_lock);
    wusb3801x_create_device_node(&(client->dev));

//    test_cc_patch();

    wusb3801x_log(LOG_DEBG, "[%s] wusb3801x probe successful!\n", __func__);
    return ret;
}

static int wusb3801_remove(struct i2c_client *client)
{
    int ret =0;
    ret = wusb3801x_read_reg(WUSB3801_REG_STATUS); 
    wusb3801x_write_reg(0x00, 0x00);
    return 0;
}

static const struct i2c_device_id wusb3801_id_table[] = {
    {"wusb3801x", 0},
    {},
};

static const struct of_device_id wusb3801_match_table[] = {
    { .compatible = "qcom, wusb3801", .data = NULL},
    {},
};
MODULE_DEVICE_TABLE(i2c, wusb3801_id_table);

static struct i2c_driver wusb3801x_driver = {
    .driver = {
        .name       = "wusb3801",
        .owner      = THIS_MODULE,
        .of_match_table = wusb3801_match_table,
    },
    .probe      = wusb3801_probe,
    .remove     = wusb3801_remove,
    .id_table   = wusb3801_id_table,
};

static int __init wusb3801x_driver_init(void)
{
    return (i2c_add_driver(&wusb3801x_driver));
}

late_initcall(wusb3801x_driver_init);

static void __exit wusb3801x_driver_exit(void)
{
    i2c_del_driver(&wusb3801x_driver);
}

module_exit(wusb3801x_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lhuang@sh-willsemi.com");
MODULE_DESCRIPTION("WUSB3801x USB Type-C driver");
MODULE_ALIAS("i2c:wusb3801x");
