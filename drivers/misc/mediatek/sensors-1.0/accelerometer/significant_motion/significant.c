/*
 * Author: yucong xiong <yucong.xion@mediatek.com>
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

#include "cust_acc.h"
#include "accel.h"

#include "hwmsen_helper.h"
#include "step_counter.h"
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/pinctrl/pinctrl-state.h>


/******************************************************************************
 * configuration
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define DEBUG_SIGNIFICANT
#ifdef DEBUG_SIGNIFICANT
#define SIGNIFICANT_TAG				  "[SIGNIFICANT] "
#define SIGNIFICANT_ERR(fmt, args...)	pr_err(SIGNIFICANT_TAG "%s %d:"fmt,__FUNCTION__,__LINE__,##args)
#define SIGNIFICANT_LOG(fmt, args...)	pr_debug(SIGNIFICANT_TAG "%s %d:"fmt,__FUNCTION__,__LINE__,##args)
#define SIGNIFICANT_FUN()               printk(SIGNIFICANT_TAG"%s\n", __FUNCTION__)
#else
#define SIGNIFICANT_TAG
#define SIGNIFICANT_ERR(fmt, args...)	do {} while (0)
#define SIGNIFICANT_ERR(fmt, args...)	do {} while (0)
#endif

static int significant_local_init(void);
static int significant_local_uninit(void);
static struct step_c_init_info  significant_init_info = {
	.name   = "SIGNIFICANT_MOTION",
	.init   = significant_local_init,
	.uninit = significant_local_uninit,
};
static DEFINE_MUTEX(SIGNIFICANT_MUTEX);

/********************** extern begin***************************/
static int (*set_acc_interrupter)(bool flag);
static int (*setup_eint)(void);


#if defined(CONFIG_TOKYO_2ND_MC3416) || defined(CONFIG_SEOUL_2ND_MC3416)
extern int mc3413_acc_init_flag;
extern int mc3413_set_acc_interrupter(bool flag);
extern int mc3413_setup_eint(void);
#endif
#ifdef CONFIG_MTK_U50A_BMA253_NEW
extern int bma2xx_set_acc_interrupter(bool flag);
extern int bma2xx_acc_init_flag;
extern int bma2xx_setup_eint(void);
#endif


#if defined(CONFIG_FERMI_1ST_N2DM)
extern int n2dm_set_acc_interrupter(bool flag);
extern int n2dm_setup_eint(void);
extern int n2dm_acc_init_flag;
#endif
#if defined(CONFIG_SEOUL_1ST_STK8BA53) || defined(CONFIG_TOKYO_1ST_STK8BA53) || defined(CONFIG_TCT_1ST_STK8BA53)
extern int stk8baxx_set_acc_interrupter(bool flag);
extern int stk_interrupt_mode_setup(void);
extern int stk8baxx_init_flag;
#endif

extern int mir3da_set_acc_interrupter(bool flag);
extern int mir3da_acc_init_flag;
extern int mir3da_setup_eint(void);
/********************** extern end ***************************/
bool significant_enable_status = false;
/********************** step_c begin***************************/
static int step_c_open_report_data(int open)
{
	return 0;
}

static int step_c_set_delay(u64 delay)
{
	return 0;
}

static int step_d_set_delay(u64 delay)
{
    return 0;
}


static int step_c_enable_significant(int en)
{
	int res =0;
	
    SIGNIFICANT_ERR("en = %d\n",en);
	if(en)
	{
	    
	    
	    res = (*set_acc_interrupter)(true);
	    if(res != 0)
	    {
	        SIGNIFICANT_ERR("set_acc_interrupter(true) failed\n");
	    }   
	}
	else
	{
	    
		
		
		res = (*set_acc_interrupter)(false);
	    if(res != 0)
	    {
	        SIGNIFICANT_ERR("set_acc_interrupter(false) failed\n");
	    } 
	}
	
	return res;
}
static int step_c_get_data_significant(uint32_t *value, int *status) 
{
	return 0;
}

static int step_c_enable_nodata(int en)
{
	return 0;
}
static int step_c_enable_step_detect(int en)
{
    return 0;
}
static int step_c_get_data(uint32_t *value, int *status)
{
	return 0;
}
static int step_c_get_data_step_d(uint32_t *value, int *status) 
{
	return 0;
}
/********************** step_c end ***************************/

static int significant_local_init(void)
{
	int res = 0;
    int error = 1;
	struct step_c_control_path step_ctl={0};
	struct step_c_data_path step_data={0};	
	SIGNIFICANT_ERR("local_init comein\n");
	SIGNIFICANT_FUN();
	mutex_lock(&SIGNIFICANT_MUTEX);

#if defined(CONFIG_TOKYO_2ND_MC3416) || defined(CONFIG_SEOUL_2ND_MC3416)
    if(mc3413_acc_init_flag == 0)
    {
         set_acc_interrupter = &mc3413_set_acc_interrupter;
         setup_eint = &mc3413_setup_eint; 
         error = 0;
    }
#endif
#ifdef CONFIG_MTK_U50A_BMA253_NEW    
    if(bma2xx_acc_init_flag == 0)
    {
        set_acc_interrupter = &bma2xx_set_acc_interrupter;
        setup_eint = &bma2xx_setup_eint; 
        error = 0;
    }
#endif

#if defined(CONFIG_FERMI_1ST_N2DM)
    if(n2dm_acc_init_flag == 0)
    {
         set_acc_interrupter = &n2dm_set_acc_interrupter;
         setup_eint = &n2dm_setup_eint; 
         error = 0;
    }
#endif


#if defined(CONFIG_SEOUL_1ST_STK8BA53) || defined(CONFIG_TOKYO_1ST_STK8BA53) || defined(CONFIG_TCT_1ST_STK8BA53)
    if(stk8baxx_init_flag == 0)
    {
        set_acc_interrupter = &stk8baxx_set_acc_interrupter;
        setup_eint = &stk_interrupt_mode_setup; 
        error = 0;
    }
#endif



    if (1 == error)
    {
        SIGNIFICANT_ERR("no accelerometers!\n");
        return -1;
    }

    res = (*setup_eint)();  
    if(res != 0)
    {
       SIGNIFICANT_ERR("setup_eint faild\n");
    }
   
	step_ctl.open_report_data= step_c_open_report_data;
	step_ctl.enable_nodata = step_c_enable_nodata;
	step_ctl.enable_step_detect = step_c_enable_step_detect;

	step_ctl.is_report_input_direct = false;
	//step_ctl.is_support_batch = false;
	step_ctl.enable_significant = step_c_enable_significant;
    step_ctl.step_c_set_delay = step_c_set_delay;
    step_ctl.step_d_set_delay = step_d_set_delay;
	res = step_c_register_control_path(&step_ctl);
	if(res)
	{
		SIGNIFICANT_ERR("register step counter control path err\n");
		goto step_c_local_init_failed;
	}
    step_data.get_data = step_c_get_data;
	step_data.get_data_step_d = step_c_get_data_step_d;
	step_data.get_data_significant = step_c_get_data_significant;

	step_data.vender_div = 1;
	res = step_c_register_data_path(&step_data);
	if(res)
	{
		SIGNIFICANT_ERR("register step counter data path err= %d\n", res);
		goto step_c_local_init_failed;
	}
	
	mutex_unlock(&SIGNIFICANT_MUTEX);
	SIGNIFICANT_ERR("significant motion sensor ok\n");
	return 0;
	
step_c_local_init_failed:
	mutex_unlock(&SIGNIFICANT_MUTEX);
	SIGNIFICANT_ERR("%s init failed! res = %d\n", __FUNCTION__,res);
	return res;
}
static int significant_local_uninit(void)
{
    return 0;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
static int __init significant_init(void)
{
    
    step_c_driver_add(&significant_init_info); 
	return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit significant_exit(void)
{
    SIGNIFICANT_ERR("significant_exit\n");
}
/*----------------------------------------------------------------------------*/
module_init(significant_init);
module_exit(significant_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("lanying.he");
MODULE_DESCRIPTION("significant driver");
MODULE_LICENSE("GPL");


