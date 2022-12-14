/* LPS22HH driver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "lps22hh.h"

static const struct i2c_device_id lps22hh_i2c_id[] = {{LPS22HH_DEV_NAME,0},{}};

static struct i2c_driver lps22hh_i2c_driver;

struct lps22hh_data *obj_i2c_data = NULL;

static DEFINE_MUTEX(lps22hh_i2c_mutex);
static DEFINE_MUTEX(lps22hh_op_mutex);

/*--------------------read function----------------------------------*/
int lps22hh_i2c_read_block(struct i2c_client *client, u8 addr, u8 *data, u8 len)
{
    u8 beg = addr;
    int err;
    struct i2c_msg msgs[2]={{0},{0}};
    
    mutex_lock(&lps22hh_i2c_mutex);
    
    msgs[0].addr = client->addr;
    msgs[0].flags = 0;
    msgs[0].len =1;
    msgs[0].buf = &beg;

    msgs[1].addr = client->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = len;
    msgs[1].buf = data;
    
    if (!client)
    {
        mutex_unlock(&lps22hh_i2c_mutex);
        return -EINVAL;
    }
    else if (len > C_I2C_FIFO_SIZE) 
    {
        ST_ERR(" length %d exceeds %d\n", len, C_I2C_FIFO_SIZE);
        mutex_unlock(&lps22hh_i2c_mutex);
        return -EINVAL;
    }
    err = i2c_transfer(client->adapter, msgs, sizeof(msgs)/sizeof(msgs[0]));
    
    if (err < 0) 
    {
        ST_ERR("i2c_transfer error: (%d %p %d) %d\n",addr, data, len, err);
        err = -EIO;
    } 
    else 
    {
        err = 0;
    }
    mutex_unlock(&lps22hh_i2c_mutex);
    return err;
}

int lps22hh_i2c_write_block(struct i2c_client *client, u8 addr, u8 *data, u8 len)
{   /*because address also occupies one byte, the maximum length for write is 7 bytes*/
    int err, idx, num;
    u8 buf[C_I2C_FIFO_SIZE];
    err =0;
    mutex_lock(&lps22hh_i2c_mutex);
    if (!client)
    {
        mutex_unlock(&lps22hh_i2c_mutex);
        return -EINVAL;
    }
    else if (len >= C_I2C_FIFO_SIZE) 
    {        
        ST_ERR(" length %d exceeds %d\n", len, C_I2C_FIFO_SIZE);
        mutex_unlock(&lps22hh_i2c_mutex);
        return -EINVAL;
    }    

    num = 0;
    buf[num++] = addr;
    for (idx = 0; idx < len; idx++)
    {
        buf[num++] = data[idx];
    }

    err = i2c_master_send(client, buf, num);
    if (err < 0)
    {
        ST_ERR("send command error!!\n");
        mutex_unlock(&lps22hh_i2c_mutex);
        return -EFAULT;
    } 
    mutex_unlock(&lps22hh_i2c_mutex);
    return err; //if success will return transfer lenth
}
/*----------------------------------------------------------------------------*/

void dumpReg(struct lps22hh_data *obj)
{
    struct i2c_client *client = obj->client;
    int i=0;
    u8 addr = 0x10;
    u8 regdata = 0;
	
    for (i = 0; i < 10; i++)
    {
        //dump all
        lps22hh_i2c_read_block(client, addr, &regdata, 1);
        ST_LOG("Reg addr=%x regdata=%x\n", addr, regdata);
        addr++;
    }
}

#ifdef CONFIG_LPS22HH_BARO_DRY
int lps22hh_set_interrupt(struct lps22hh_data *obj, u8 intenable)
{
	struct i2c_client *client = obj->client;

    u8 databuf[2];
    u8 addr = LPS22HH_REG_CTRL_REG3;
    int res = 0;

    memset(databuf, 0, sizeof(u8)*2); 

    if((lps22hh_i2c_read_block(client, addr, databuf, 0x01))<0)
    {
        ST_ERR("read reg_ctl_reg1 register err!\n");
        return LPS22HH_ERR_I2C;
    }

    databuf[0] = 0x04;

    res = lps22hh_i2c_write_block(client, LPS22HH_REG_CTRL_REG3, databuf, 0x01);
    if(res < 0)
    {
        return LPS22HH_ERR_I2C;
    }
    
    return LPS22HH_SUCCESS;    
}
#endif

static int lps22hh_check_device_id(struct lps22hh_data *obj)
{
	struct i2c_client *client = obj->client;
	u8 databuf[2] = {0};
	int err = 0;
	
	err = lps22hh_i2c_read_block(client, LPS22HH_REG_WHO_AM_I, databuf, 0x01);
	if (err < 0)
	{
        	ST_ERR("read LPS22HH_REG_WHO_AM_I fail\n");
		return LPS22HH_ERR_I2C;
	}
	
        ST_LOG("LPS22HH who am I = 0x%x\n", databuf[0]);
	if (databuf[0] != LPS22HH_FIXED_DEVID)
	{
        	ST_LOG("Chip id is incorrect\n");
		return LPS22HH_ERR_IDENTIFICATION;
	}
	
	return LPS22HH_SUCCESS;
}

static ssize_t reg_value_show(struct device_driver *ddri, char *buf)
{
    struct lps22hh_data *obj = obj_i2c_data;

    u8 reg_value;
    ssize_t res;
    int err;
	
    if (obj == NULL)
    {
        ST_ERR("i2c_data obj is null!!\n");
        return 0;
    }
    err = lps22hh_i2c_read_block(obj->client, obj->reg_addr, &reg_value, 0x01);
	if (err < 0)
	{
		res = snprintf(buf, PAGE_SIZE, "i2c read error!!\n");
        return res;
	}
	
    res = snprintf(buf, PAGE_SIZE, "0x%04X\n", reg_value); 
    return res;
}

static ssize_t reg_value_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct lps22hh_data *obj = obj_i2c_data;

    u8 reg_value;
	int res;
	
    if (obj == NULL)
    {
        ST_ERR("i2c_data obj is null!!\n");
        return 0;
    }
    
    if(1 == sscanf(buf, "0x%hhx", &reg_value))
    {
	    res = lps22hh_i2c_write_block(obj->client, obj->reg_addr, &reg_value, 0x01);
	    if(res < 0)
        {
            return LPS22HH_ERR_I2C;
        }
    }    
    else
    {
        ST_ERR("invalid content: '%s', length = %d\n", buf, (int)count);
    }

    return count;
}

static ssize_t reg_addr_show(struct device_driver *ddri, char *buf)
{
    struct lps22hh_data *obj = obj_i2c_data;

    ssize_t res;

    if (obj == NULL)
    {
        ST_ERR("i2c_data obj is null!!\n");
        return 0;
    }
    
    res = snprintf(buf, PAGE_SIZE, "0x%04X\n", obj->reg_addr); 
    return res;
}

static ssize_t reg_addr_store(struct device_driver *ddri, const char *buf, size_t count)
{
    struct lps22hh_data *obj = obj_i2c_data;
	u8 reg_addr;

    if (obj == NULL)
    {
        ST_ERR("i2c_data obj is null!!\n");
        return 0;
    }
    
    if(1 == sscanf(buf, "0x%hhx", &reg_addr))
    {
        obj->reg_addr = reg_addr;
    }    
    else
    {
        ST_ERR("invalid content: '%s', length = %d\n", buf, (int)count);
    }

    return count;
}
//static DRIVER_ATTR(reg_value, S_IWUSR | S_IRUGO, lps22hh_attr_i2c_show_reg_value, lps22hh_attr_i2c_store_reg_value);
//static DRIVER_ATTR(reg_addr,  S_IWUSR | S_IRUGO, lps22hh_attr_baro_show_reg_addr,  lps22hh_attr_i2c_store_reg_addr);

static DRIVER_ATTR_RW(reg_value);
static DRIVER_ATTR_RW(reg_addr);

static struct driver_attribute *lps22hh_attr_i2c_list[] = {
    &driver_attr_reg_value,
	&driver_attr_reg_addr,
};

int lps22hh_i2c_create_attr(struct device_driver *driver) 
{
    int idx, err = 0;
    int num = (int)(sizeof(lps22hh_attr_i2c_list)/sizeof(lps22hh_attr_i2c_list[0]));
    if (driver == NULL)
    {
        return -EINVAL;
    }

    for(idx = 0; idx < num; idx++)
    {
        if((err = driver_create_file(driver, lps22hh_attr_i2c_list[idx])))
        {            
            ST_ERR("driver_create_file (%s) = %d\n", lps22hh_attr_i2c_list[idx]->attr.name, err);
            break;
        }
    }    
    return err;
}

int lps22hh_i2c_delete_attr(struct device_driver *driver)
{
    int idx ,err = 0;
    int num = (int)(sizeof(lps22hh_attr_i2c_list)/sizeof(lps22hh_attr_i2c_list[0]));

    if(driver == NULL)
    {
        return -EINVAL;
    }
    

    for(idx = 0; idx < num; idx++)
    {
        driver_remove_file(driver, lps22hh_attr_i2c_list[idx]);
    }
    

    return err;
}

/*----------------------------------------------------------------------------*/
#ifndef KERNEL_4_4
static void lps22hh_chip_power(struct lps22hh_data *obj, unsigned int on) 
{
    return;
}

static int lps22hh_suspend(struct i2c_client *client, pm_message_t msg) 
{
    struct lps22hh_data *obj = i2c_get_clientdata(client);
    struct lps22hh_baro *baro_obj = &obj->lps22hh_baro_data;
    int err = 0;
	
    ST_FUN();
    
    if((msg.event == PM_EVENT_SUSPEND) && (baro_obj->enabled == 1))
    {   
        mutex_lock(&lps22hh_op_mutex);
        if(obj == NULL)
        {    
            mutex_unlock(&lps22hh_op_mutex);
            ST_ERR("null pointer!!\n");
            return -EINVAL;
        }

        err = lps22hh_baro_set_power_mode(baro_obj, false);		
        if(err)
        {
            ST_ERR("write power control fail!!\n");
            mutex_unlock(&lps22hh_op_mutex);
            return err;        
        }
        
        atomic_set(&baro_obj->suspend, 1);
		mutex_unlock(&lps22hh_op_mutex);
        lps22hh_chip_power(obj, 0);
    }
    
	ST_LOG("lps22hh i2c suspended\n");
    return err;
}
/*----------------------------------------------------------------------------*/
static int lps22hh_resume(struct i2c_client *client)
{
    struct lps22hh_data *obj = i2c_get_clientdata(client);
    struct lps22hh_baro *baro_obj = &obj->lps22hh_baro_data;
    int err;
	
    ST_FUN();
    lps22hh_chip_power(obj, 1);
	if (baro_obj->enabled == 1) 
    {
	    mutex_lock(&lps22hh_op_mutex);
	    if(obj == NULL)
	    {
		    mutex_unlock(&lps22hh_op_mutex);
		    ST_ERR("null pointer!!\n");
		    return -EINVAL;
	    }
    
        err = lps22hh_baro_set_power_mode(baro_obj, true);
        if(err)
        {
            mutex_unlock(&lps22hh_op_mutex);
            ST_ERR("initialize client fail!!\n");
            return err;        
        }
        atomic_set(&baro_obj->suspend, 0);
        mutex_unlock(&lps22hh_op_mutex);
    }
	ST_LOG("lps22hh i2c resumed\n");
    return 0;
}
#endif
/*----------------------------------------------------------------------------*/
static int lps22hh_i2c_detect(struct i2c_client *client, struct i2c_board_info *info) 
{    
    strcpy(info->type, LPS22HH_DEV_NAME);
    return 0;
}

/*----------------------------------------------------------------------------*/

static int lps22hh_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct lps22hh_data *obj;
    int err = 0;
	ST_LOG("lps22hh_i2c_probe enter\n");
	
	if(!(obj = kzalloc(sizeof(*obj), GFP_KERNEL)))
    {
        err = -ENOMEM;
		return err;
    }
    memset(obj, 0, sizeof(struct lps22hh_data));
	obj_i2c_data = obj;
    obj->client = client;
    i2c_set_clientdata(client, obj);
	
	err = lps22hh_check_device_id(obj);
	if (err)
	{
        ST_ERR("check device error!\n");
		goto exit_check_device_failed;
	}

    err = lps22hh_i2c_create_attr(&lps22hh_i2c_driver.driver);
	if (err)
	{
        ST_ERR("create attr error!\n");
		goto exit_create_attr_failed;
	}
#ifdef CONFIG_LPS22HH_BARO_DRY
	//TODO: request irq for data ready or pedometer, SMD, TILT, etc.
#endif	
    baro_driver_add(&lps22hh_baro_init_info);

	ST_LOG("lps22hh_i2c_probe ok\n");
    return 0;

exit_create_attr_failed:
exit_check_device_failed:
    kfree(obj);
	return err;

}
/*----------------------------------------------------------------------------*/
static int lps22hh_i2c_remove(struct i2c_client *client)
{
    lps22hh_i2c_delete_attr(&lps22hh_i2c_driver.driver);
    i2c_unregister_device(client);
    kfree(i2c_get_clientdata(client));
    return 0;
}
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_OF
static const struct of_device_id lps22hh_of_match[] = {
    {.compatible = "mediatek,barometer"},
    {}
};
#endif

static struct i2c_driver lps22hh_i2c_driver = {
    .driver = {
        .name           = LPS22HH_DEV_NAME,
#ifdef CONFIG_OF
        .of_match_table = lps22hh_of_match,
#endif
    },
    .probe              = lps22hh_i2c_probe,
    .remove             = lps22hh_i2c_remove,
    .detect             = lps22hh_i2c_detect,
#ifndef KERNEL_4_4	
    .suspend            = lps22hh_suspend,
    .resume             = lps22hh_resume,
#endif
    .id_table           = lps22hh_i2c_id,
};

/*----------------------------------------------------------------------------*/
static int __init lps22hh_module_init(void)
{
    ST_FUN();

    if (i2c_add_driver(&lps22hh_i2c_driver))
    {
        ST_ERR("add driver error\n");
        return -1;
    }
   
    return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit lps22hh_module_exit(void)
{
    i2c_del_driver(&lps22hh_i2c_driver);

    ST_FUN();
}
/*----------------------------------------------------------------------------*/
module_init(lps22hh_module_init);
module_exit(lps22hh_module_exit);
/*----------------------------------------------------------------------------*/
MODULE_DESCRIPTION("STMicroelectronics lps22hh driver");
MODULE_AUTHOR("William ZENG");
MODULE_LICENSE("GPL v2");
