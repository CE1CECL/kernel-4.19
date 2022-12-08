#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/ide.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/irq.h>
#include <linux/of_irq.h>

#include "head_def.h"
#include "tpd.h"
#include "semi_touch_device.h"
#include "semi_touch_custom.h"
#include "semi_touch_function.h"
//begin add by bo_liu for 11022639
#include <linux/gpio.h>
#include <linux/delay.h>
//end add by bo_liu for 11022639

int semi_touch_get_irq(int rst_pin)
{
    int irq_no = 0;
    struct device_node* node = NULL;
    unsigned int ints[2] = { 0, 0 };

    node = of_find_matching_node(node, touch_of_match);
    check_return_if_zero(node, NULL);

    of_property_read_u32_array(node, "debounce", ints, ARRAY_SIZE(ints));
    gpio_set_debounce(ints[0], ints[1]);

    irq_no = irq_of_parse_and_map(node, 0);
    check_return_if_fail(irq_no, NULL);

    return irq_no;
}

int semi_touch_work_done(void)
{
    return 0;
}

int semi_touch_resource_release(void)
{
    return 0;
}



/********************************************************************************************************************************/
/*proximity support*/
#if SEMI_TOUCH_PROXIMITY_OPEN
#include "hwmsensor.h"
#include "hwmsen_dev.h"
#include "sensors_io.h"
struct hwmsen_object sm_obj_ps;

// int semi_touch_proximity_update(unsigned char enter)
// {
//     int ret = 0;

//     if(is_proximity_function_en(st_dev.stc.custom_function_en))
//     {
//         if(enter)
//         {
//             enter_proximity_gate(st_dev.stc.ctp_run_status);
//         }
//         else
//         {
//             leave_proximity_gate(st_dev.stc.ctp_run_status);
//         }

//         ret = semi_touch_proximity_switch(enter);
//         check_return_if_fail(ret, NULL);

//         kernel_log_d("proximity %s...\n", enter ? "enter" : "leave");
//     }

//     return ret;
// }

int semi_touch_proximity_operate(void* self, uint32_t command, void* buff_in, int size_in, void* buff_out, int size_out, int* actualout)
{
    int ret = -SEMI_DRV_INVALID_CMD;
    int value = 0;
    //struct hwm_sensor_data *sensor_data;

    switch (command)
    {
        case SENSOR_DELAY:
            if((buff_out == NULL) || (size_out< sizeof(int)))
			{
				ret = -SEMI_DRV_INVALID_PARAM;
			}
            else
            {
                ret = SEMI_DRV_ERR_OK;  //do nothing
            }
            break;
        case SENSOR_ENABLE:
            if((buff_in == NULL) || (size_in < sizeof(int)))
			{
				ret = -SEMI_DRV_INVALID_PARAM;
			}
			else
			{				
				value = *(int*)buff_in;
				ret = semi_touch_proximity_switch(value > 0);
			}
			break;
        case SENSOR_GET_DATA:
            if((buff_out == NULL) || (size_out < sizeof(struct hwm_sensor_data)))
			{
				ret = -SEMI_DRV_INVALID_PARAM;
			}
            else
            {
                //trigger mode, so not needed
                //sensor_data = (struct hwm_sensor_data *)buff_out;
                //sensor_data->values[0] = 0;
				//sensor_data->value_divide = 1;
				//sensor_data->status = SENSOR_STATUS_ACCURACY_MEDIUM;
            }
            break;
        default:
            break;
    }

    check_return_if_fail(ret, NULL);

    return ret;
}

int semi_touch_proximity_init(void)
{
    int ret = 0;

    open_proximity_function(st_dev.stc.custom_function_en);
    
    sm_obj_ps.polling = 0;   //0--interrupt mode;1--polling mode;
    sm_obj_ps.sensor_operate = semi_touch_proximity_operate;

    ret = hwmsen_attach(ID_PROXIMITY, &sm_obj_ps);
    check_return_if_fail(ret, NULL);

    return ret;
}

irqreturn_t semi_touch_proximity_report(unsigned char proximity)
{
    struct hwm_sensor_data sensor_data;

    if(is_proximity_function_en(st_dev.stc.custom_function_en))
    {
        sensor_data.values[0] = proximity;
        sensor_data.value_divide = 1;
        sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;

        hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data);
    }

    return IRQ_RETVAL(IRQ_HANDLED);
}

int semi_touch_proximity_stop(void)
{
    hwmsen_detach(ID_PROXIMITY);

    return 0;
}
#endif

/********************************************************************************************************************************/
/*mtk touch screen device*/
static const struct of_device_id sm_of_match[] = 
{
    {.compatible = "mediatek,chsc_cap_touch", },
    {}
};

static const struct i2c_device_id sm_ts_id[] = 
{
    {CHSC_DEVICE_NAME, 0},
    {}
};

//begin add by bo_liu for 11022639

int power_state = 0;

static int semi_power_source_ctrl(int enable)
{
	int ret = 0;

  	//tpd_gpio_output(GTP_RST_PORT, 0);
	//tpd_gpio_output(GTP_INT_PORT, 0);

    if (enable) {
        if (power_state) {
            ret = regulator_enable(tpd->reg);
            if (ret) {
                printk("bo_liu enable vdd regulator failed,ret=%d", ret);
            }
            power_state = false;
        }
    } else {
        if (!power_state) {
		tpd_gpio_output(46, 0);
            mdelay(10);
            ret = regulator_disable(tpd->reg);
            if (ret) {
                printk("bo_liu disable vdd regulator failed,ret=%d", ret);
            }
            power_state = true;
        }
    }

    return ret;


}

//end add by bo_liu for 11022639

extern char ctp_module_name[256];

static int semi_touch_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret = 0;

    ret = semi_touch_init(client);
    if(SEMI_DRV_ERR_OK != ret)
    {
        semi_touch_deinit(client);
		//begin add by bo_liu for 11022639
		semi_power_source_ctrl(0);
		//end add by bo_liu for 11022639
    }
    check_return_if_fail(ret, NULL);

    tpd_load_status = 1;

    snprintf(ctp_module_name, 256, "CHSC5XXX:DWS\n");

    kernel_log_d("semitouch probe finished\r\n");

    return ret;
}

static int semi_touch_remove(struct i2c_client *client)
{
    int ret = 0;

    ret = semi_touch_deinit(client);

    kernel_log_d("semitouch remove complete\r\n");

    return ret;
}

static int semi_touch_i2c_detect(struct i2c_client *client, struct i2c_board_info *info)
{
    strcpy(info->type, TPD_DEVICE);

    return 0;
}

static struct i2c_driver sm_touch_driver = 
{
    .driver = 
    {
        .owner = THIS_MODULE,
        .name = "semi_touch",
        .of_match_table = of_match_ptr(sm_of_match),
    },
    .id_table = sm_ts_id,
    .probe = semi_touch_probe,
    .remove = semi_touch_remove,
    .detect = semi_touch_i2c_detect,
};

int semi_touch_power_init(void)
{

    int ret;
    /*set TP volt*/
    tpd->reg = regulator_get(tpd->tpd_dev, "vtouch");
    ret = regulator_set_voltage(tpd->reg, 2800000, 2800000);
	//begin add by bo_liu for 11022639
	if (ret == 0)
	{
		power_state = 1;
	}
	//end add by bo_liu for 11022639
    check_return_if_fail(ret, NULL);

	//begin add by bo_liu for 11022639
	ret = semi_power_source_ctrl(1);
    //ret = regulator_enable(tpd->reg);
	//end add by bo_liu for 11022639
    check_return_if_fail(ret, NULL);

    return 0;
}

static int semi_touch_local_init(void)
{
    int ret = 0;

    ret = semi_touch_power_init();
    check_return_if_fail(ret, NULL);

    ret = i2c_add_driver(&sm_touch_driver);
    check_return_if_fail(ret, NULL);

    return ret;
}

void semi_touch_suspend_entry(struct device* dev)
{
    //struct i2c_client *client = st_dev.client;
//begin add by bo_liu for 11052959
	int ret = 0;
//end add by bo_liu for 11052959

    if(is_proximity_function_en(st_dev.stc.custom_function_en))
    {
        if(is_proximity_activate(st_dev.stc.ctp_run_status))
        {
            kernel_log_d("proximity is active, so fake suspend...");
            return;
        }
    }

    if(is_guesture_function_en(st_dev.stc.custom_function_en))
    {
        semi_touch_guesture_switch(1);
    }
    else
    {
        semi_touch_suspend_ctrl(1);
        semi_touch_clear_report();
        //disable_irq(client->irq);
//begin add by bo_liu for 11052959
	ret = semi_power_source_ctrl(0);
	if (ret){
		printk("semi_power_source_ctrl 0 fail, func:%s, line:%d\n", __func__, __LINE__);
	}
//end add by bo_liu for 11052959
        kernel_log_d("tpd real suspend...\n");
    }
}

void semi_touch_resume_entry(struct device* dev)
{
    unsigned char bootCheckOk = 0;
    unsigned char glove_activity = is_glove_activate(st_dev.stc.ctp_run_status);
//begin add by bo_liu for 11052959
	int ret = 0;

	ret = semi_touch_power_init();
	if (ret){
		printk("semi_touch_power_init fail, func:%s, line:%d\n", __func__, __LINE__);
	}
//end add by bo_liu for 11052959

    if(is_proximity_function_en(st_dev.stc.custom_function_en))
    {
        if(is_proximity_activate(st_dev.stc.ctp_run_status))
        {
            kernel_log_d("proximity is active, so fake resume...");
            return;
        }
    }

    //reset tp + iic detected
    semi_touch_reset_and_detect();
    //set_status_pointing(st_dev.stc.ctp_run_status);
    semi_touch_clear_report();
    //enable_irq(client->irq);

    if(glove_activity)
    {
        semi_touch_start_up_check(&bootCheckOk);
        if(bootCheckOk)
        {
            semi_touch_glove_switch(1);
        }
    }
    kernel_log_d("tpd_resume...\r\n");
}

static struct tpd_driver_t tpd_device_driver = 
{
    .tpd_device_name = CHSC_DEVICE_NAME,
    .tpd_local_init = semi_touch_local_init,
    .suspend = semi_touch_suspend_entry,
    .resume = semi_touch_resume_entry,

};

static int __init tpd_driver_init(void)
{
    int ret = 0;

    tpd_get_dts_info();
    ret = tpd_driver_add(&tpd_device_driver);
    check_return_if_fail(ret, NULL);

    return ret;
}

static void __exit tpd_driver_exit(void)
{
    tpd_driver_remove(&tpd_device_driver);
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);
