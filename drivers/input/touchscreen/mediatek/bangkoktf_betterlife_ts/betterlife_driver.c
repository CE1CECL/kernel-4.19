
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
//#include <linux/rtpm_prio.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "tpd_bl_common.h"
#include "tpd_custom_upgrade.h"
//#include "bl_fw.h"
#include "tpd.h"
#include <uapi/linux/sched/types.h>

//#define TIMER_DEBUG
#if defined(TIMER_DEBUG)
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#endif//TIMER_DEBUG

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>

//#include <mt_gpio.h>
//#include <mt_gpio_core.h>

static spinlock_t irq_lock;
static int tpd_irq_flag = 0;
struct mutex i2c_lock;  //add for i2c_rw_access
//add by xucs start
int bl_is_suspend = 0;
int btl_log_level = 0;
//add by xucs end
struct i2c_client* btl_i2c_client = NULL;
static struct task_struct* thread = NULL;

static int tpd_flag;
static DECLARE_WAIT_QUEUE_HEAD(waiter);
static irqreturn_t tpd_eint_interrupt_handler(int irq, void* dev_id);

static int tpd_probe(struct i2c_client* client, const struct i2c_device_id* id);
static int tpd_i2c_detect(struct i2c_client* client, struct i2c_board_info* info);
static int tpd_remove(struct i2c_client* client);
static int touch_event_handler(void* unused);
static void tpd_resume(struct device* h);
static void tpd_suspend(struct device* h);



unsigned int btl_tpd_rst_gpio_number = 0;
unsigned int btl_tpd_int_gpio_number = 1;
unsigned int bl_touch_irq = 0;

void bl_enable_irq(void)
{
    unsigned long flag;

    spin_lock_irqsave(&irq_lock, flag);
    if(!tpd_irq_flag) {  // 0-disabled
        tpd_irq_flag = 1;  // 1-enabled
        if(bl_touch_irq != 0){
            enable_irq(bl_touch_irq);
	}
    }
    spin_unlock_irqrestore(&irq_lock, flag);
}

void bl_disable_irq(void)
{
    unsigned long flag;

    spin_lock_irqsave(&irq_lock, flag);
    if(tpd_irq_flag) {
        tpd_irq_flag = 0;
        if(bl_touch_irq != 0){
            disable_irq(bl_touch_irq);
	}
    }
    spin_unlock_irqrestore(&irq_lock, flag);
}

//add for i2c_rw_access
void bl_i2c_lock(int enable)
{
    if(enable)
        mutex_lock(&i2c_lock);
    else
        mutex_unlock(&i2c_lock);
}

struct touch_info {
    int y[MAX_POINT_NUM];
    int x[MAX_POINT_NUM];
    int p[MAX_POINT_NUM];
    int id[MAX_POINT_NUM];
    int count;

#ifdef BTL_CTP_PRESSURE
    int pressure;
#endif
};

static const struct i2c_device_id bl_tpd_id[] = {{"betterlife_ts", 0}, {} };
static const struct of_device_id bl_dt_match[] = {
    {.compatible = "mediatek,cap_touch_betterlife"},
    {},
};
MODULE_DEVICE_TABLE(of, bl_dt_match);

#ifdef  BTL_ESD_PROTECT
static struct delayed_work bl_esd_check_work;
static struct workqueue_struct* bl_esd_check_workqueue = NULL;
//static struct timer_list key_timer;
atomic_t report_timeout;
static void bl_esd_check_func(struct work_struct*);
static u32 reg_value_F9_pre = 0xFF;
static u8 esd_mode_kernel_flag = 0;
static u8 reg_value = 0;
void bl_esd_switch(s32 on);
void bl_recovery_reset(void);
void bl_recovery_reset_poweroff(void);
static volatile int recovery_count = 0;
static u8 run_num = 0;

static int esd_reset_num_0x99 = 0;
static int esd_reset_num_0x97 = 0;

static spinlock_t esd_lock;
static u8  esd_running;
static u32 clk_tick_cnt;

//void bl_key_timer_function(unsigned long data);
#endif//BTL_ESD_PROTECT

#ifdef  BTL_APK_SUPPORT
//cmd|i2c_addr|data_len|ndata
static struct proc_dir_entry* proc_entry;
extern int is_apk_debug;
extern wait_queue_head_t debug_queue;
extern int debug_sync_flag;
//static unsigned char *apk_data = NULL;
extern struct file_operations bl_apk_fops;
#endif//BTL_APK_SUPPORT



#ifdef CONFIG_MTK_I2C_EXTENSION

#define DMA_MAX_LEN 264
static u8* g_dma_buff_va = NULL;
static dma_addr_t g_dma_buff_pa = 0;

static void btl_dma_alloct(void)
{
    if(NULL == g_dma_buff_va) {
        tpd->dev->dev.coherent_dma_mask = DMA_BIT_MASK(32);
        g_dma_buff_va = (u8*)dma_alloc_coherent(&tpd->dev->dev, DMA_MAX_LEN, &g_dma_buff_pa, GFP_KERNEL);
    }

    if(!g_dma_buff_va) {
        TPD_DEBUG("btl [DMA][Error] Allocate DMA I2C Buffer failed!\n");
    }
}

static void btl_dma_release(void)
{
    if(g_dma_buff_va) {
        dma_free_coherent(&tpd->dev->dev, DMA_MAX_LEN, g_dma_buff_va, g_dma_buff_pa);
        g_dma_buff_va = NULL;
        g_dma_buff_pa = 0;
        TPD_DEBUG("btl [DMA][release] Allocate DMA I2C Buffer release!\n");
    }
}
#endif//CONFIG_MTK_I2C_EXTENSION

#ifdef BTL_CTP_PROXIMITY_SENSOR
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

//extern int ril_call_state;    //call flag
int ril_call_state = 0;

static int PROXIMITY_STATE = 0;
u8 ps_value = 0;
struct input_dev* ps_input_dev;

int TP_face_mode_switch(int on);
extern void bl_touch_ps_data_report(int value);
#endif/*BTL_CTP_PROXIMITY_SENSOR*/

#ifdef  BTL_CTP_GESTURE_ENABLE
int bl_ts_gesture_init(struct input_dev* input_dev)
{

    input_set_capability(input_dev, EV_KEY, KEY_POWER);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_U);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_UP);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_DOWN);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_LEFT);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_RIGHT);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_O);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_E);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_M);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_W);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_S);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_V);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_C);
    input_set_capability(input_dev, EV_KEY, BL_KEY_GESTURE_Z);
    __set_bit(BL_KEY_GESTURE_U, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_UP, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_DOWN, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_LEFT, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_RIGHT, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_O, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_E, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_M, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_W, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_S, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_V, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_C, input_dev->keybit);
    __set_bit(BL_KEY_GESTURE_Z, input_dev->keybit);

    return 0;
}

static void bl_ts_gesture_report(int gesture_id)
{
    int gesture;
    struct input_dev* input_dev = tpd->dev;

    if(0 == gesture_id) return;

    switch(gesture_id) {
    case BL_U_LEFT:
        gesture = BL_KEY_GESTURE_U;
        break;
    case BL_U_RIGHT:
        gesture = BL_KEY_GESTURE_U;
        break;
    case BL_U_UP:
        gesture = BL_KEY_GESTURE_U;
        break;
    case BL_U_DOWN:
        gesture = BL_KEY_GESTURE_U;
        break;

    case BL_MOVE_UP:
        gesture = BL_KEY_GESTURE_UP;
        break;
    case BL_MOVE_DOWN:
        gesture = BL_KEY_GESTURE_DOWN;
        break;
    case BL_MOVE_LEFT:
        gesture = BL_KEY_GESTURE_LEFT;
        break;
    case BL_MOVE_RIGHT:
        gesture = BL_KEY_GESTURE_RIGHT;
        break;
    case BL_O_GEST:
        gesture = BL_KEY_GESTURE_O;
        break;
    case BL_E_GEST:
        gesture = BL_KEY_GESTURE_E;
        break;
    case BL_M_GEST:
        gesture = BL_KEY_GESTURE_M;
        break;
    case BL_W_GEST:
        gesture = BL_KEY_GESTURE_W;
        break;
    case BL_S_GEST:
        gesture = BL_KEY_GESTURE_S;
        break;
    case BL_V_LEFT:
        gesture = BL_KEY_GESTURE_V;
        break;
    case BL_V_UP:
        gesture = BL_KEY_GESTURE_V;
        break;
    case BL_V_DOWN:
        gesture = BL_KEY_GESTURE_V;
        break;
    case BL_V_RIGHT:
        gesture = BL_KEY_GESTURE_V;
        break;
    case BL_C_GEST:
        gesture = BL_KEY_GESTURE_C;
        break;
    case BL_Z_GEST:
        gesture = BL_KEY_GESTURE_Z;
        break;

    default:
        gesture = -1;
        break;
    }

    /* report event key */
    if(gesture != -1) {
        //TPD_DEBUG("btl Gesture Code=0x%x", gesture);
        input_report_key(input_dev, gesture, 1);
        input_sync(input_dev);
        input_report_key(input_dev, gesture, 0);
        input_sync(input_dev);
    }
}
#endif

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
#include <linux/power_supply.h>
//static u8 ac_usb_power_online;
static u8 charging = 0;
struct delayed_work ac_usb_dwork;
#endif

#if defined(TIMER_DEBUG)
static struct timer_list test_timer;

static void timer_func(unsigned long data)
{
    //tpd_flag = 1;
    //wake_up_interruptible(&waiter);

    mod_timer(&test_timer, jiffies + 100 * (1000 / HZ));
}

static int init_test_timer(void)
{
    memset((void*)&test_timer, 0, sizeof(test_timer));
    test_timer.expires  = jiffies + 100 * (1000 / HZ);
    test_timer.function = timer_func;
    test_timer.data     = 0;
    init_timers(&test_timer);
    add_timer(&test_timer);
    return 0;
}
#endif//TIMER_DEBUG

#if defined(CONFIG_TPD_ROTATE_90) || defined(CONFIG_TPD_ROTATE_270)
static void tpd_swap_xy(int* x, int* y)
{
    int temp = 0;

    temp = *x;
    *x = *y;
    *y = temp;
}
#endif

#if defined(CONFIG_TPD_ROTATE_90)

static void tpd_rotate_90(int* x, int* y)
{
    //int temp;

    *x = TPD_RES_X + 1 - *x;

    *x = (*x * TPD_RES_Y) / TPD_RES_X;
    *y = (*y * TPD_RES_X) / TPD_RES_Y;

    tpd_swap_xy(x, y);
}

#endif

#if defined(CONFIG_TPD_ROTATE_180)
static void tpd_rotate_180(int* x, int* y)
{
    *y = TPD_RES_Y + 1 - *y;
    *x = TPD_RES_X + 1 - *x;
}
#endif

#if defined(CONFIG_TPD_ROTATE_X_180)
static void tpd_rotate_x_180(int* x, int* y)
{
    *x = TPD_RES_X + 1 - *x;
}
#endif

#if defined(CONFIG_TPD_ROTATE_270)
static void tpd_rotate_270(int* x, int* y)
{
    //int temp;

    *y = TPD_RES_Y + 1 - *y;

    *x = (*x * TPD_RES_Y) / TPD_RES_X;
    *y = (*y * TPD_RES_X) / TPD_RES_Y;

    tpd_swap_xy(x, y);
}
#endif//CONFIG_TPD_ROTATE_XXX


/*****************i2c DMA config*******************************/
#ifdef CONFIG_MTK_I2C_EXTENSION
int btl_i2c_apk_write(struct i2c_client* client, unsigned char* writebuf, int writelen)
{
    int ret = 0;
    int retry = 3;
    // for DMA I2c transfer
    while(retry--)
    {
        if((NULL != client) && (writelen > 0) && (writelen <= DMA_MAX_LEN)) {
            // DMA Write
            memcpy(g_dma_buff_va, writebuf, writelen);
            client->addr = (client->addr & I2C_MASK_FLAG) | I2C_DMA_FLAG;
        
            ret = i2c_master_send(client, (unsigned char*)g_dma_buff_pa, writelen);
            client->addr = (client->addr & I2C_MASK_FLAG) & (~ I2C_DMA_FLAG);
        
            if(ret == writelen) 
            {
                return ret;
            }
			else
            {
                continue;
            }
        }
    }
	if(retry < 0)
	{
        TPD_DEBUG("btl i2c write failed\n");
        return -1;
	}
}

int btl_i2c_apk_read(struct i2c_client* client, unsigned char* readbuf, int readlen)
{
    int ret = 0;
    int retry = 3;
    // DMA Read
    while(retry--)
    {
        if((NULL != client) && (readlen > 0) && (readlen <= DMA_MAX_LEN)) {
            client->addr = (client->addr & I2C_MASK_FLAG) | I2C_DMA_FLAG;
            ret = i2c_master_recv(client, (unsigned char*)g_dma_buff_pa, readlen);
            memcpy(readbuf, g_dma_buff_va, readlen);
            client->addr = (client->addr & I2C_MASK_FLAG) & (~ I2C_DMA_FLAG);
            //BF_TP_LOG("btl_i2c_apk_read  ret = %d , readlen = %d", ret, readlen);
            if(ret == readlen) 
            {
                return ret;
            }
    		else
            {
                continue;
            }
        }
    }
	if(retry<0)
	{
        TPD_DEBUG("btl i2c read failed\n");
        return -1;
	}
}

int btl_i2c_read(struct i2c_client* client, unsigned char* writebuf, int writelen, unsigned char* readbuf, int readlen)
{
    int ret = 0;
    int retry = 3;
    // for DMA I2c transfer
    while(retry--)
    {
        if((NULL != client) && (writelen > 0) && (writelen <= DMA_MAX_LEN)) {
            // DMA Write
            memcpy(g_dma_buff_va, writebuf, writelen);
            client->addr = (client->addr & I2C_MASK_FLAG) | I2C_DMA_FLAG;
    
            ret = i2c_master_send(client, (unsigned char*)g_dma_buff_pa, writelen);
            client->addr = (client->addr & I2C_MASK_FLAG) & (~ I2C_DMA_FLAG);
    
            if(ret == writelen) 
			{
			    break;
                TPD_DEBUG("btl i2c write failed\n");
            }
			else
			{
			    continue;
			}
        }
    }
	if(retry<0)
	{
		TPD_DEBUG("btl i2c write failed\n");
        return -1;
	}
    // DMA Read
    if((NULL != client) && (readlen > 0) && (readlen <= DMA_MAX_LEN)) {
        client->addr = (client->addr & I2C_MASK_FLAG) | I2C_DMA_FLAG;
        ret = i2c_master_recv(client, (unsigned char*)g_dma_buff_pa, readlen);
        memcpy(readbuf, g_dma_buff_va, readlen);
        client->addr = (client->addr & I2C_MASK_FLAG) & (~ I2C_DMA_FLAG);

        if(ret == readlen) return ret;
    }

    return -1;
}

int btl_i2c_write(struct i2c_client* client, unsigned char* writebuf, int writelen)
{
    int i = 0;
    int ret = 0;
	int retry = 3;
    while(retry)
    {
        if(writelen <= 8) {
            client->ext_flag = client->ext_flag & (~I2C_DMA_FLAG);
            ret = i2c_master_send(client, writebuf, writelen);
        } else if((writelen > 8 && writelen <= DMA_MAX_LEN) && (0 != g_dma_buff_pa)) {
            for(i = 0; i < writelen; i++)
                g_dma_buff_va[i] = writebuf[i];
    
            client->addr = (client->addr & I2C_MASK_FLAG) | I2C_DMA_FLAG;
            ret = i2c_master_send(client, (unsigned char*)g_dma_buff_pa, writelen);
            client->addr = client->addr & I2C_MASK_FLAG & (~I2C_DMA_FLAG);
        }
		if(ret == writelen)
		{
		    return ret;
		}
		else
		{
		    continue;
		}
    }
    if(retry<0)
    {
        return -1;
    }
}

#else//CONFIG_MTK_I2C_EXTENSION
int btl_i2c_read(struct i2c_client* client, unsigned char* writebuf, int writelen, unsigned char* readbuf, int readlen)
{
    int ret = 0;
    int retry = 3;
	while(retry--)
	{
        if(readlen > 0) {
            if(writelen > 0) {
                struct i2c_msg msgs[] = {
                    {
                        .addr = client->addr,
                        .flags = 0,
                        .len = writelen,
                        .buf = writebuf,
                    },
                    {
                        .addr = client->addr,
                        .flags = I2C_M_RD,
                        .len = readlen,
                        .buf = readbuf,
                    },
                };
    
                ret = i2c_transfer(client->adapter, msgs, 2);
                if(ret < 0)
                {
                    continue;
                }
				else
				{
				    break;
				}
    
            } else {
                struct i2c_msg msgs[] = {
                    {
                        .addr = client->addr,
                        .flags = I2C_M_RD,
                        .len = readlen,
                        .buf = readbuf,
                    },
                };
    
                ret = i2c_transfer(client->adapter, msgs, 1);
                if(ret < 0)
            	{
            	    continue;
            	}
				else
				{
                    break;
				}
            }
        }
	}
    if(retry<0)
    {
        
		BF_TP_LOG("btl i2c read error.");
        return -1;
    }
	return ret;
}

int btl_i2c_write(struct i2c_client* client, unsigned char* writebuf, int writelen)
{
    int ret = 0;
    int retry = 3;
    struct i2c_msg msgs[] = {
        {
            .addr = client->addr,
            .flags = 0,
            .len = writelen,
            .buf = writebuf,
        },
    };
    while(retry--)
    {
        if(writelen > 0) {
            ret = i2c_transfer(client->adapter, msgs, 1);
            if(ret < 0) {
				continue;
            }
			else
			{
			    break;
			}
        }
    }
	if(retry < 0)
	{
        BF_TP_LOG("btl i2c write error.");
		return -1;
	}
	
    return ret;
}

int btl_i2c_apk_write(struct i2c_client* client, unsigned char* writebuf, int writelen)
{
    int ret = 0;
	int retry = 3;
    struct i2c_msg msgs[] = {
        {
            .addr = client->addr,
            .flags = 0,
            .len = writelen,
            .buf = writebuf,
        },
    };
    while(retry--)
    {
        ret = i2c_transfer(client->adapter, msgs, 1);
        if(ret < 0)
        {
            continue;
        }
		else
		{
		    break;
		}
    }
	if(retry<0)
	{
	    BF_TP_LOG("btl i2c write error.");
	    return -1;
	}
	else
	{
        return ret;
	}
}

int btl_i2c_apk_read(struct i2c_client* client, unsigned char* readbuf, int readlen)
{
    int ret = 0;
	int retry = 3;
    struct i2c_msg msgs[] = {
        {
            .addr = client->addr,
            .flags = 1,
            .len = readlen,
            .buf = readbuf,
        },
    };
    while(retry--)
    {
        ret = i2c_transfer(client->adapter, msgs, 1);
        if(ret < 0)
    	{
    	    continue;
    	}
	    else
    	{
    	    break;
    	}
    }
	if(retry<0)
	{
	    BF_TP_LOG("btl i2c read error.");
	    return -1;
	}
	else
	{
	    return ret;
	}
}
#endif//CONFIG_MTK_I2C_EXTENSION

int btl_write_reg(struct i2c_client* client, u8 regaddr, u8 regvalue)
{
    unsigned char buf[2] = {0};

    buf[0] = regaddr;
    buf[1] = regvalue;

    return btl_i2c_write(client, buf, sizeof(buf));
}

int btl_read_reg(struct i2c_client* client, u8 regaddr, u8* regvalue)
{
    return btl_i2c_read(client, &regaddr, 1, regvalue, 1);
}



static struct i2c_driver tpd_i2c_driver = {
    .driver = {
        .of_match_table = of_match_ptr(bl_dt_match),
        .name = "betterlife_ts",
    },
    .probe = tpd_probe,
    .remove = tpd_remove,
    .id_table = bl_tpd_id,
    .detect = tpd_i2c_detect,
};

static int of_get_bl_platform_data(struct device* dev)
{
    /*int ret, num;*/
    //struct pinctrl *pinctrl1 = NULL;
    //struct pinctrl_state *bl_touch_default = NULL;

    if(dev->of_node) {
        const struct of_device_id* match;

        match = of_match_device(of_match_ptr(bl_dt_match), dev);
        if(!match) {
            TPD_DEBUG("btl Error: No device match found\n");
            return -ENODEV;
        }
    }

    //tpd_rst_gpio_number = of_get_named_gpio(dev->of_node, "rst-gpio", 0);
    //btl_tpd_int_gpio_number = of_get_named_gpio(dev->of_node, "int-gpio", 0);
    /*ret = of_property_read_u32(dev->of_node, "rst-gpio", &num);
    if (!ret)
        tpd_rst_gpio_number = num;
    ret = of_property_read_u32(dev->of_node, "int-gpio", &num);
    if (!ret)
        btl_tpd_int_gpio_number = num;*/

    TPD_DEBUG("btl g_vproc_en_gpio_number %d\n",   btl_tpd_rst_gpio_number);
    TPD_DEBUG("btl g_vproc_vsel_gpio_number %d\n", btl_tpd_int_gpio_number);
    return 0;
}

void bl_ts_set_int_gpio(unsigned char level)
{
    tpd_gpio_output(btl_tpd_int_gpio_number, level);
}


static void tpd_down(int x, int y, int p, int id)
{
#if defined(CONFIG_TPD_ROTATE_90)
    tpd_rotate_90(&x, &y);
#elif defined(CONFIG_TPD_ROTATE_270)
    tpd_rotate_270(&x, &y);
#elif defined(CONFIG_TPD_ROTATE_180)
    tpd_rotate_180(&x, &y);
#elif defined(CONFIG_TPD_ROTATE_X_180)
    tpd_rotate_x_180(&x, &y);
#endif

#ifdef TPD_HAVE_TOUCH_ID_BUTTON
    if(x == TPD_TOUCH_ID_BUTTON_X1 && y == TPD_TOUCH_ID_BUTTON_Y1) {  //touch id
        input_report_key(tpd->dev, TPD_TOUCH_ID_BUTTON1, 1);
        input_sync(tpd->dev);
        return;
    }

    if(x == TPD_TOUCH_ID_BUTTON_X2 && y == TPD_TOUCH_ID_BUTTON_Y2) {  //touch id
        input_report_key(tpd->dev, TPD_TOUCH_ID_BUTTON2, 1);
        input_sync(tpd->dev);
        return;
    }

    if(x == TPD_TOUCH_ID_BUTTON_X3 && y == TPD_TOUCH_ID_BUTTON_Y3) {  //touch id
        input_report_key(tpd->dev, TPD_TOUCH_ID_BUTTON3, 1);
        input_sync(tpd->dev);
        return;
    }
#endif//TPD_HAVE_TOUCH_ID_BUTTON

    input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, id);
    input_report_key(tpd->dev, BTN_TOUCH, 1);
    input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
    input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);

#ifdef BTL_CTP_PRESSURE
    input_report_abs(tpd->dev, ABS_MT_PRESSURE, pressure);
#endif

    TPD_DEBUG("btl-tpd-down");

    input_mt_sync(tpd->dev);
}

static void tpd_up(int x, int y)
{
#if defined(CONFIG_TPD_ROTATE_90)
    tpd_rotate_90(&x, &y);
#elif defined(CONFIG_TPD_ROTATE_270)
    tpd_rotate_270(&x, &y);
#elif defined(CONFIG_TPD_ROTATE_180)
    tpd_rotate_180(&x, &y);
#elif defined(CONFIG_TPD_ROTATE_X_180)
    tpd_rotate_x_180(&x, &y);
#endif

#ifdef TPD_HAVE_TOUCH_ID_BUTTON
    if(x == TPD_TOUCH_ID_BUTTON_X1 && y == TPD_TOUCH_ID_BUTTON_Y1) {  //touch id
        input_report_key(tpd->dev, TPD_TOUCH_ID_BUTTON1, 0);
        input_sync(tpd->dev);
        return;
    }

    if(x == TPD_TOUCH_ID_BUTTON_X2 && y == TPD_TOUCH_ID_BUTTON_Y2) {  //touch id
        input_report_key(tpd->dev, TPD_TOUCH_ID_BUTTON2, 0);
        input_sync(tpd->dev);
        return;
    }

    if(x == TPD_TOUCH_ID_BUTTON_X3 && y == TPD_TOUCH_ID_BUTTON_Y3) {  //touch id
        input_report_key(tpd->dev, TPD_TOUCH_ID_BUTTON3, 0);
        input_sync(tpd->dev);
        return;
    }
#endif//TPD_HAVE_TOUCH_ID_BUTTON

    input_report_key(tpd->dev, BTN_TOUCH, 0);

#ifdef BTL_CTP_PRESSURE
    input_report_abs(tpd->dev, ABS_MT_PRESSURE, 0xFF);
#endif

    TPD_DEBUG("btl-tpd-up");
    input_mt_sync(tpd->dev);
}

static void btl_release_all_points(void)
{
    unsigned char i;
	for(i=0; i<MAX_POINT_NUM;i++)
    {
        tpd_up(0,0);
    }
	input_sync(tpd->dev);
}

static int tpd_touchinfo(struct touch_info* cinfo, struct touch_info* pinfo)
{
    int i = 0;
    //u8 data[20] = {0};
    u8 data[2 + 6 * MAX_POINT_NUM] = {0};
    //MAX_POINT_NUM
    u8 addr = 0x01;
    u16 high_byte, low_byte;
    int ret = 0;

#ifdef  BTL_CTP_GESTURE_ENABLE
    u8 gesture_id = 0xFF;
#endif

    /*
        pinfo : pre data
        cinfo : now data
    */
    memcpy(pinfo, cinfo, sizeof(struct touch_info));
    memset(cinfo, 0, sizeof(struct touch_info));

#ifdef BTL_CTP_PROXIMITY_SENSOR
    if(ril_call_state == 1) {
        bl_i2c_lock(1);
        i2c_smbus_read_i2c_block_data(btl_i2c_client, BTL_CTP_PROXIMITY_FLAG_REG, 1, &ps_value);
        bl_i2c_lock(0);

        if(ps_value  == BTL_CTP_PROXIMITY_NEAR) {  //near
            if(PROXIMITY_STATE == 0) {
                PROXIMITY_STATE = 1;
                bl_touch_ps_data_report(0);
                return 0;
            }
        } else if(ps_value == BTL_CTP_PROXIMITY_LEAVE) { //far
            if(PROXIMITY_STATE == 1) {
                PROXIMITY_STATE =  0;
                bl_touch_ps_data_report(1);
            }
        }
    }
#endif//end BTL_CTP_PROXIMITY_SENSOR

#ifdef BTL_CTP_GESTURE_ENABLE
    if(1 == bl_is_suspend) {
        ret = btl_read_reg(btl_i2c_client, 0x01, &gesture_id);
        if(ret > 0) {
            bl_ts_gesture_report(gesture_id);
            return 0;
        }
    }
#endif

    bl_i2c_lock(1);
    //ret = btl_i2c_read(btl_i2c_client,&addr,1,data,14);
    ret = btl_i2c_read(btl_i2c_client, &addr, 1, data, 2 + 6 * MAX_POINT_NUM);
    bl_i2c_lock(0);

    for(i = 0; i < MAX_POINT_NUM; i++)
        cinfo->p[i] = 1;    /* Put up */

    /*get the number of the touch points*/
    cinfo->count = data[1] & 0x0f;

    /*get the pressure of the touch points*/
#ifdef BTL_CTP_PRESSURE
    cinfo->pressure = data[6] & 0x0f;
#endif


    //for(i = 0; i < cinfo->count; i++) {
    for(i = 0; i < MAX_POINT_NUM; i++) {
        cinfo->p[i] = (data[2 + 6 * i] >> 6) & 0x0003; /* event flag */
        cinfo->id[i] = data[4 + 6 * i] >> 4; // touch id

        /*get the X coordinate, 2 bytes*/
        high_byte = data[2 + 6 * i];
        high_byte <<= 8;
        high_byte &= 0x0F00;

        low_byte = data[2 + 6 * i + 1];
        low_byte &= 0x00FF;
        cinfo->x[i] = high_byte | low_byte;

        /*get the Y coordinate, 2 bytes*/
        high_byte = data[2 + 6 * i + 2];
        high_byte <<= 8;
        high_byte &= 0x0F00;

        low_byte = data[2 + 6 * i + 3];
        low_byte &= 0x00FF;
        cinfo->y[i] = high_byte | low_byte;
    }

    return true;
}

static int touch_event_handler(void* unused)
{
    int i = 0;

    struct touch_info cinfo, pinfo;//finfo;
    struct sched_param param = {.sched_priority = 4 };

    sched_setscheduler(current, SCHED_RR, &param);

#ifdef BTL_ESD_PROTECT
#if 0
    init_timer(&key_timer);
    key_timer.expires = jiffies + 10 * HZ;
    key_timer.data = 0;//(unsigned long)ts;
    key_timer.function = bl_key_timer_function;
    atomic_set(&report_timeout, 0);
#endif
    //atomic_set(&ts->is_i2c_busy, 0);
    INIT_DELAYED_WORK(&bl_esd_check_work, bl_esd_check_func);
    bl_esd_check_workqueue = create_workqueue("bl_esd_check");

    clk_tick_cnt = 1 * HZ;// HZ: clock ticks in 1 second generated by system
    TPD_DEBUG("btl Clock ticks for an esd cycle: %d", clk_tick_cnt);
    spin_lock_init(&esd_lock);
    bl_esd_switch(SWITCH_ON);
#endif//BTL_ESD_PROTECT

    do {
        /*enable_irq(bl_touch_irq);*/
        set_current_state(TASK_INTERRUPTIBLE);

        wait_event_interruptible(waiter, tpd_flag != 0);

        tpd_flag = 0;

        set_current_state(TASK_RUNNING);

#ifdef BTL_ESD_PROTECT
        //mod_timer(&key_timer,jiffies + HZ/5);
#endif

        if(tpd_touchinfo(&cinfo, &pinfo)) {

            BF_TP_LOG("cinfo.count  = %d ", cinfo.count);
#if 0
            if(cinfo.count > 0) {
                for(i = 0; i < cinfo.count; i++)
                    tpd_down(cinfo.x[i], cinfo.y[i], i + 1, cinfo.id[i]);
            } else {
                /*pinfo : pre data  cinfo : now data*/
                //tpd_up(cinfo.x[0], cinfo.y[0]);
                tpd_up(pinfo.x[0], pinfo.y[0]);
            }

#else

            for(i = 0; i < MAX_POINT_NUM; i++) {
                if(cinfo.p[i] == TPD_DOWN || cinfo.p[i] == TPD_MOVE) {
                    if(cinfo.count > 0 && cinfo.count <= MAX_POINT_NUM) {
                        tpd_down(cinfo.x[i], cinfo.y[i], i + 1, cinfo.id[i]);
                        BF_TP_LOG("tpd_down  x = %d ", cinfo.x[i]);
                        BF_TP_LOG("tpd_down  y = %d", cinfo.y[i]);
                        BF_TP_LOG("tpd_down  p = %d", cinfo.p[i]);
                        BF_TP_LOG("tpd_down  id = %d ", cinfo.id[i]);
                    }
                }
            }

            if(cinfo.count == 0) {
                tpd_up(cinfo.x[0], cinfo.y[0]);
                BF_TP_LOG("tpd_up x = %d ", cinfo.x[0]);
                BF_TP_LOG("tpd_up  y = %d", cinfo.y[0]);
                BF_TP_LOG("tpd_up  p = %d", cinfo.p[0]);
                BF_TP_LOG("tpd_up  id = %d ", cinfo.id[0]);
            }

#endif

            input_sync(tpd->dev);
        }

        //enable_irq(bl_touch_irq);
        bl_enable_irq();

    } while(!kthread_should_stop());

    TPD_DEBUG("btl touch_event_handler exit\n");

    return 0;
}

static int tpd_i2c_detect(struct i2c_client* client, struct i2c_board_info* info)
{
    strcpy(info->type, TPD_DEVICE);

    return 0;
}

static irqreturn_t tpd_eint_interrupt_handler(int irq, void* dev_id)
{
    tpd_flag = 1;
    BF_TP_LOG("tpd_eint_interrupt_handler irq");
    spin_lock(&irq_lock);
    if(tpd_irq_flag) {
        tpd_irq_flag = 0;
        disable_irq_nosync(bl_touch_irq);
    }
    spin_unlock(&irq_lock);
    wake_up_interruptible(&waiter);
    return IRQ_HANDLED;
}

static int tpd_irq_registration(void)
{
#if 0
    struct device_node* node = NULL;
    int ret = 0;

    node = of_find_compatible_node(NULL, NULL, "mediatek,bl_touch");
    if(node) {
        /*bl_touch_irq = gpio_to_irq(btl_tpd_int_gpio_number);*/
        bl_touch_irq = irq_of_parse_and_map(node, 0);
        ret = request_irq(bl_touch_irq, tpd_eint_interrupt_handler,
                          IRQF_TRIGGER_FALLING, TPD_DEVICE, NULL);
        if(ret > 0)
            TPD_DMESG("tpd request_irq IRQ LINE NOT AVAILABLE!.");
    } else {
        TPD_DMESG("[%s] tpd request_irq can not find touch eint device node!.", __func__);
    }

#else

    struct device_node* node = NULL;
    int ret = 0;
    //u32 ints[2] = {0,0};

    node = of_find_matching_node(node, touch_of_match);
    if(node) {
        /*bl_touch_irq = gpio_to_irq(btl_tpd_int_gpio_number);*/
        //of_property_read_u32_array(node,"debounce", ints, ARRAY_SIZE(ints));
        //gpio_set_debounce(ints[0], ints[1]);

        bl_touch_irq = irq_of_parse_and_map(node, 0);
        TPD_DEBUG("btl bl_touch_irq=%d\n", bl_touch_irq);
        ret = request_irq(bl_touch_irq, tpd_eint_interrupt_handler, IRQF_TRIGGER_FALLING, "TOUCH_PANEL-eint", NULL);
        if(ret > 0)
            TPD_DEBUG("btl tpd request_irq IRQ LINE NOT AVAILABLE!.");
        else if(ret == 0)
            TPD_DEBUG("btl tpd request_irq right \n");

    } else {
        TPD_DEBUG("btl [%s] tpd request_irq can not find touch eint device node!.", __func__);
    }

#endif

    return 0;
}

static int greg_rw = 0x00;
static int greg_addr = 0xb6;
static int greg_value = 0;
static int greg_len = 0;
static int greg_r_flag = 0;
static ssize_t btl_show_reg(struct device* ddri, struct device_attribute* attr, char* buf)
{
    u8 data[100];

    u8 addr = greg_addr;

    //u8 test[30]={0};
    u8 test_buf[20] = {0};
    u8 test_buf16[20] = {0};
    int k;

    s16 value16 = 0;
    s16* pvalue16 = NULL;

    u8 buf1[512] = {0};
    //s16 buf16[512] = {0};
    u8 buf16[512] = {0};

    if(greg_rw == 0) { //write
        bl_i2c_lock(1);
        btl_i2c_read(btl_i2c_client, &addr, 1, data, 2);
        bl_i2c_lock(0);
        return sprintf(buf, "greg_addr=%x,data[0]=0x%x,data[1]=0x%x\n", greg_addr, data[0], data[1]);
    } else if(greg_rw == 1) { //read

        bl_i2c_lock(1);
        btl_i2c_read(btl_i2c_client, &addr, 1, data, greg_len);

        bl_i2c_lock(0);

        if(greg_r_flag == 0) {

            for(k = 0; k < greg_len; k++) {
                memset(test_buf, 0, 20);
                sprintf(test_buf, " 0x%x", data[k]);
                strcat(buf1, test_buf);
            }
            return sprintf(buf, "greg_addr = %x data =%s\n", greg_addr, buf1);

        } else if(greg_r_flag == 1) {
            for(k = 0; k < greg_len ;) {
                pvalue16 = (s16*)(&data[k]);
                value16 = *pvalue16;

                memset(test_buf16, 0, 20);
                sprintf(test_buf16, " %6d", value16);

                k = k + 2;
                strcat(buf16, test_buf16);

            }

            return sprintf(buf, "greg_addr = %x data =%s\n", greg_addr, buf16);
        }

    }

    return 0;
}

static ssize_t btl_store_reg(struct device* dev, struct device_attribute* attr, const char* buf, size_t size)
{

    char* next;
    greg_rw = simple_strtoul(buf, &next, 16);
    greg_addr = simple_strtoul(next + 1, &next, 16);
    greg_value = simple_strtoul(next + 1, &next, 16);
    greg_r_flag = simple_strtoul(next + 1, &next, 16);

    if(greg_rw == 0) { //write
        bl_i2c_lock(1);
        btl_write_reg(btl_i2c_client, greg_addr, greg_value);
        bl_i2c_lock(0);
        //TPD_DEBUG("btl write greg_addr=0x%x,greg_value=0x%x\n",greg_addr,greg_value);

    } else if(greg_rw == 1) { //read
        greg_len = greg_value;
        //TPD_DEBUG("btl read greg_addr=0x%x",greg_addr);
    }

    return size;
}
static DEVICE_ATTR(reg, 0664, btl_show_reg, btl_store_reg);



#ifdef BL_UPDATE_FARMWARE_FROM_BIN
static int gfw_version = 0;
static ssize_t bl_show_fwupdate(struct device* ddri, struct device_attribute* attr, char* buf)
{
    return sprintf(buf, "gfw_version=%d\n", gfw_version);
}

static ssize_t bl_store_fwupdate(struct device* dev, struct device_attribute* attr, const char* buf, size_t size)
{
    gfw_version = bl_fw_upgrade_with_app_file(btl_i2c_client, "BTL.bin");
    TPD_DEBUG("gfw_version=0x%x", gfw_version);
    return size;
}
static DEVICE_ATTR(fwupdate, 0664, bl_show_fwupdate, bl_store_fwupdate);
#endif

#ifdef CONFIG_BTL_CHECK_CHIPID
static ssize_t bl_show_chipid(struct device* ddri, struct device_attribute* attr, char* buf)
{
    int ret_value = 0;
    u8  chip_id = 0;
    TPD_DEBUG("bl_show_debug");
    bl_i2c_lock(1);
    ret_value = bl_get_chip_id(&chip_id);
    bl_i2c_lock(0);
    btl_chip_reset();
    if(ret_value < 0)
        return sprintf(buf, "read chipid error %d, 0x%02x\n", ret_value, chip_id);
    else
        return sprintf(buf, "chipid is 0x%02x\n", chip_id);
}

static ssize_t bl_store_chipid(struct device* dev, struct device_attribute* attr, const char* buf, size_t size)
{
    TPD_DEBUG("bl_store_debug");
    return 1;
}
static DEVICE_ATTR(chipid, 0664, bl_show_chipid, bl_store_chipid);


#endif

//static u32 kernel_version = BL_KERNEL_VERSION;
static ssize_t bl_show_kernelversion(struct device* ddri, struct device_attribute* attr, char* buf)
{
    return sprintf(buf, "btl driver version=%s\n", BL_KERNEL_VERSION);
}
static ssize_t bl_store_kernelversion(struct device* dev, struct device_attribute* attr, const char* buf, size_t size)
{
    return size;
}
static DEVICE_ATTR(kernelversion, 0664, bl_show_kernelversion, bl_store_kernelversion);
//add by xucs start
static ssize_t btl_show_log_level(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "log level:%d\n",btl_log_level);
}
static ssize_t btl_store_log_level(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
{
    int value = 0;

    sscanf(buf, "%d", &value);
    btl_log_level = value;
    return size;
}
static DEVICE_ATTR(log_level, 0664, btl_show_log_level, btl_store_log_level);
//add by xucs end
/*******AC USB power detect config *****************/
#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
static int bl_send_power_state(unsigned char power_state)
{
    return  btl_write_reg(btl_i2c_client, AC_REG, power_state);
}

#ifdef BTL_CTP_PROXIMITY_SENSOR
static int bl_send_call_state(unsigned char call_state)
{
    return  btl_write_reg(btl_i2c_client, CALL_REG, call_state);
}
#endif

#if 0
static void Btl_DrvReadFile(char* pFilePath, u8* pBuf, u16 nLength)
{
    struct file* pFile = NULL;
    mm_segment_t old_fs;
    ssize_t nReadBytes = 0;

    old_fs = get_fs();
    set_fs(get_ds());

    pFile = filp_open(pFilePath, O_RDONLY, 0);
    if(IS_ERR(pFile)) {
        TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work Open file failed: %s\n", pFilePath);
        return;
    }

    pFile->f_op->llseek(pFile, 0, SEEK_SET);
    nReadBytes = pFile->f_op->read(pFile, pBuf, nLength, &pFile->f_pos);
    TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work Read %d bytes!\n", (int)nReadBytes);

    set_fs(old_fs);
    filp_close(pFile, NULL);
}

static void bl_ts_ac_usb_power_detect_work(struct work_struct* work)
{
    struct file* filp = NULL;
    mm_segment_t old_fs;
    loff_t offset = 0;
    unsigned char buf[2] = {0};
    unsigned char i;
    static unsigned usb_file_open_fail = 0;
    static unsigned ac_file_open_fail = 0;
    static unsigned char online_bak = 0;

    if(!bl_is_suspend) {
        for(i = 0; i < 2; i++) {
            if(i) {
                if(usb_file_open_fail < 5) {
                    filp = filp_open(USB_POWER_ONLINE_STATUS_PATH, O_RDONLY, 0);
                } else {
                    continue;
                }
            } else {
                if(ac_file_open_fail < 5) {
                    filp = filp_open(AC_POWER_ONLINE_STATUS_PATH, O_RDONLY, 0);
                } else {
                    continue;
                }
            }
            if(IS_ERR(filp)) {
                if(i) {
                    usb_file_open_fail++;
                    TPD_DEBUG("open file:%s fail!!\n", USB_POWER_ONLINE_STATUS_PATH);
                } else {
                    ac_file_open_fail++;
                    TPD_DEBUG("open file:%s fail!!\n", AC_POWER_ONLINE_STATUS_PATH);
                }

            } else {
                old_fs = get_fs();
                set_fs(KERNEL_DS);

                filp->f_op->read(filp, (char*)buf, 1, &offset);
                if(buf[0] == '1') {
                    ac_usb_power_online = 0x01;
                } else {
                    ac_usb_power_online = 0x00;
                }
                if(online_bak != ac_usb_power_online) {

                    online_bak = ac_usb_power_online;
                    bl_send_power_state(ac_usb_power_online);
                    TPD_DEBUG("__%s power online state = %c__\n", i ? "USB" : "AC", buf[0]);
                    break;
                }
                set_fs(old_fs);
                filp_close(filp, NULL);
            }
        }

        schedule_delayed_work(&ac_usb_dwork, msecs_to_jiffies(POLLING_TIME));
    }
}

static void bl_ts_ac_usb_power_detect_work(struct work_struct* work)
{

    unsigned char buf[20] = {0};
    int ret = -1;
    unsigned char fw_ac_usb_value = 0xFF;
    static unsigned char charging_bak = 0;
    static char* status_text[] = {"Unknown", "Charging", "Discharging", "Not charging", "Full"};//power_supply_sysfs.c

    if(!bl_is_suspend) {
        Btl_DrvReadFile(BATTERY_CHARGE_STATUS_PATH, buf, 8);

        if(!strcmp(buf, status_text[1])) {
            charging = 0x01;
        } else {
            charging = 0x00;
        }

        bl_i2c_lock(1);
        ret = btl_read_reg(btl_i2c_client, AC_REG, &fw_ac_usb_value);
        TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work charging=%d ret=%d fw_ac_usb_value=%d\n", charging, ret, fw_ac_usb_value);

        if((ret > 0) && ((charging_bak != charging) || (fw_ac_usb_value != charging))) {
            charging_bak = charging;
            ret = bl_send_power_state(charging);
            if(ret > 0) {
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work bl_write_reg ok\n");
            } else {
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work bl_write_reg error\n");
            }
        }
        bl_i2c_lock(0);

        schedule_delayed_work(&ac_usb_dwork, msecs_to_jiffies(POLLING_TIME));
    }

}
#else
static void bl_ts_ac_usb_power_detect_work(struct work_struct* work)
{

    int ret = -1;
    unsigned char fw_ac_usb_value = 0xFF;

#ifdef BTL_CTP_PROXIMITY_SENSOR
    unsigned char fw_call_value = 0xFF;
    int ret_call = -1;
#endif
    static unsigned char charging_bak = 0;
    struct power_supply* usb_psy;
    struct power_supply* ac_psy;
    union power_supply_propval usb_online;
    union power_supply_propval ac_online;
    u8 usb_charging = 0;
    u8 ac_charging = 0;

    if(!bl_is_suspend) {
        usb_psy = power_supply_get_by_name("usb");
        if(usb_psy->desc->get_property(usb_psy, POWER_SUPPLY_PROP_ONLINE, &usb_online)) {
            usb_charging = 0;
        } else {
            usb_charging = usb_online.intval;
        }



        ac_psy = power_supply_get_by_name("ac");
        if(ac_psy->desc->get_property(ac_psy, POWER_SUPPLY_PROP_ONLINE, &ac_online)) {
            ac_charging = 0;
        } else {
            ac_charging = ac_online.intval;
        }

        if(usb_charging || ac_charging) {
            charging = 0x01;
        } else {
            charging = 0x00;
        }
        //TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work usb_charging=%d ac_charging=%d\n",usb_charging,ac_charging);


#ifdef BTL_CTP_PROXIMITY_SENSOR
        ret_call = btl_read_reg(btl_i2c_client, CALL_REG, &fw_call_value);
        if((ret_call > 0) && (fw_call_value != ril_call_state)) {
            ret_call = bl_send_call_state(ril_call_state);
            if(ret_call > 0) {
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work bl_write_reg 0xb2 ok\n");
            } else {
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work bl_write_reg 0xb2 error\n");
            }
        }
#endif
        bl_i2c_lock(1);
        ret = btl_read_reg(btl_i2c_client, AC_REG, &fw_ac_usb_value);
        bl_i2c_lock(0);
        TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work charging=%d ret=%d fw_ac_usb_value=%d\n", charging, ret, fw_ac_usb_value);

        if((ret > 0) && ((charging_bak != charging) || (fw_ac_usb_value != charging))) {
            charging_bak = charging;
            bl_i2c_lock(1);
            ret = bl_send_power_state(charging);
            bl_i2c_lock(0);
            if(ret > 0) {
                u8 t_reg_value = 0;
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work bl_write_reg ok\n");
                bl_i2c_lock(1);
                btl_read_reg(btl_i2c_client, AC_REG, &t_reg_value);
                bl_i2c_lock(0);
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work read power state:%d\n", t_reg_value);
            } else {
                TPD_DEBUG("btl bl_ts_ac_usb_power_detect_work bl_write_reg error\n");
            }


        }
        //bl_i2c_lock(0);
        schedule_delayed_work(&ac_usb_dwork, msecs_to_jiffies(POLLING_TIME));
    }

}

#endif

static void bl_ts_ac_usb_power_detect_init(struct delayed_work* dwork)
{
    INIT_DELAYED_WORK(&ac_usb_dwork, bl_ts_ac_usb_power_detect_work);
    schedule_delayed_work(&ac_usb_dwork, msecs_to_jiffies(POLLING_TIME));
}
#endif//BTL_AC_USB_POWER_DETECT_ENABLE

#ifdef BTL_ESD_PROTECT

void bl_esd_switch(s32 on)
{

    spin_lock(&esd_lock);

    if(SWITCH_ON == on) {
        // switch on esd
        if(!esd_running) {
            esd_running = 1;
            spin_unlock(&esd_lock);
            TPD_DEBUG("btl bl_esd_switch started");
            queue_delayed_work(bl_esd_check_workqueue, &bl_esd_check_work, clk_tick_cnt);
        } else {
            spin_unlock(&esd_lock);
        }

    } else {
        // switch off esd
        if(esd_running) {
            esd_running = 0;
            spin_unlock(&esd_lock);
            TPD_DEBUG("btl bl_esd_switch cancelled");
            cancel_delayed_work_sync(&bl_esd_check_work);
        } else {
            spin_unlock(&esd_lock);
        }
    }
}

static void bl_esd_check_func(struct work_struct* work)
{

    s32 ret = -1;
    s32 ret_F9 = -1;
    //u8 reg_value_F9 = 0;
    u8 esd_buff_F9[4] = {0};
    u8 addr = 0xF9;

    u32 reg_value_F9 = 0;
    u32* p_reg_value_F9 = NULL;

    if(bl_is_suspend) {
        TPD_DEBUG("btl bl_esd_check_func Esd suspended!\n");
        return;
    }

    TPD_DEBUG("btl bl_esd_check_func reg_value=%d,recovery_count=%d\n", reg_value, recovery_count);
    TPD_DEBUG("btl bl_esd_check_func esd_reset_num_0x99=%d,esd_reset_num_0x97=%d\n", esd_reset_num_0x99, esd_reset_num_0x97);
    TPD_DEBUG("btl bl_esd_check_func run_num=%d,reg_value_F9_pre=%d,reg_value_F9=%d\n", run_num, reg_value_F9_pre, reg_value_F9);

    //if(atomic_read(&ts->is_i2c_busy) != 1)
    {

        bl_i2c_lock(1);
        ret = btl_read_reg(btl_i2c_client, 0x01, &reg_value);

        ret_F9 = btl_i2c_read(btl_i2c_client, &addr, 1, esd_buff_F9, 4);
        p_reg_value_F9 = (u32*)(&esd_buff_F9[0]);
        reg_value_F9 = *p_reg_value_F9;

        run_num = run_num + 1;

        if(((ret < 0) && (ret_F9 < 0)) || (reg_value == 0x99) || (reg_value == 0x1)
           || (((run_num > 1) && (reg_value_F9 == reg_value_F9_pre)))) {
            esd_reset_num_0x99++;
            bl_recovery_reset();
            //esd_mode_kernel_flag = 1;

        } else {
            recovery_count = 0;
        }

        if(reg_value == 0x97) {
            esd_reset_num_0x97++;
            bl_recovery_reset_poweroff();
        }

        bl_i2c_lock(0);

        reg_value_F9_pre = reg_value_F9;
        if(run_num > 200) run_num = 2;
    }//no reset


    if(!bl_is_suspend) {
        queue_delayed_work(bl_esd_check_workqueue, &bl_esd_check_work, clk_tick_cnt);
    } else {
        TPD_DEBUG("btl Esd suspended!");
    }

    TPD_DEBUG("bl_esd_check_func end\n");
    return;
}

void bl_recovery_reset(void)
{
    bl_disable_irq();

    bl_ts_set_int_gpio(0);
    TPD_DEBUG("bl_esd_check_func : bl_recovery_reset start recovery_count =%d \n", recovery_count);

    if(recovery_count > 0) {
        esd_mode_kernel_flag = 1;
    }

    if(recovery_count > 0 && reg_value != 0x99) {
        tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
        msleep(100);
        tpd_gpio_output(btl_tpd_rst_gpio_number, 0);
    }

    msleep(50);

    if(recovery_count > 0 && reg_value != 0x99) {
        tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
    }

    if(recovery_count > 0)
        recovery_count = 0;
    else
        recovery_count++;

    bl_ts_set_int_gpio(1);
    msleep(200);

    tpd_gpio_as_int(btl_tpd_int_gpio_number);
    //mt_set_gpio_pull_enable(5, GPIO_PULL_ENABLE);
    //mt_set_gpio_pull_select(5, GPIO_PULL_UP);

    bl_enable_irq();

    run_num = 0;
    TPD_DEBUG("bl_esd_check_func : bl_recovery_reset end\n");
}


void bl_recovery_reset_poweroff(void)
{

    bl_disable_irq();

    TPD_DEBUG("bl_esd_check_func : bl_recovery_reset_poweroff start\n");


    tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
    msleep(100);
    tpd_gpio_output(btl_tpd_rst_gpio_number, 0);

    //regulator_disable(bl_ts->vdd);
    msleep(50);
    tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
    //regulator_enable(bl_ts->vdd);

    msleep(200);
    bl_enable_irq();

    TPD_DEBUG("bl_esd_check_func : bl_recovery_reset_poweroff end\n");
}

#if 0
void bl_key_timer_function(unsigned long data)
{
    atomic_set(&report_timeout, 1);
}
#endif
#endif //BTL_ESD_PROTECT

/***********************************************************************************************/
#ifdef BTL_CTP_PROXIMITY_SENSOR
/***********************************************************************************************/
#if 0
static void bl_pls_report_init(void)
{
    TPD_DEBUG("btl %s\n", __func__);
    //input_report_abs(ps_input_dev, ABS_DISTANCE, 1); //report far
    //input_sync(ps_input_dev);
}
#endif

int TP_face_mode_switch(int on)///enable  ctp sensor
{
    u8 data[2];
    u8 read_value;
    u8 i;
    int ret = -1;

    TPD_DEBUG("btl TP_face_mode_switch on=%d \n", on);

    data[0] = BTL_CTP_PROXIMITY_MODE_REG;
    data[1] = 0x00;

    if(1 == on) {
        data[1] = 0x01;
    } else {
        data[1] = 0x00;
    }

    if(btl_i2c_client) {
        ret = i2c_master_send(btl_i2c_client, &data[0], 2);
    } else {
        return -ENODEV;
    }

    if(ret < 0) {
        //bl8818_ts_reset();
        //bl8818_write_reg(bl_ts->driver.client,FT5X0X_REG_PERIODACTIVE, 7);
        //i2c_master_send(bl_ts->driver.client, &data[0], 2);
        //enable_irq(bl_ts->irq_number);
    }

    for(i = 0; i < 3; i++) {
        msleep(2);
        btl_read_reg(btl_i2c_client , BTL_CTP_PROXIMITY_MODE_REG, &read_value);
        //i2c_smbus_read_i2c_block_data(i2c_client, BTL_CTP_PROXIMITY_MODE_REG, 1, &read_value);
        TPD_DEBUG("btl TP_face_mode_switch read read_value=%x i=%d", read_value, i);

        if(read_value == data[1]) {
            break;
        } else {
            ret = i2c_master_send(btl_i2c_client, &data[0], 2);
        }
    }

    if(1 == on) {
        //bl_pls_report_init();
    }

    return 0;
}

#if 0 //BTL_CTP_PROXIMITY_SENSOR fops start
static int bl_ps_open(struct inode* inode, struct file* file)
{
    TPD_DEBUG("btl %s\n", __func__);
    if(bl_ps_opened)
        return -EBUSY;
    bl_ps_opened = 1;
    return 0;
}

static int bl_ps_release(struct inode* inode, struct file* file)
{
    TPD_DEBUG("btl %s", __func__);
    bl_ps_opened = 0;
    return TP_face_mode_switch(0);
}

static long bl_ps_unlocked_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
    void __user* argp = (void __user*)arg;
    int flag;
    int plag;
    char strbuf[256];
    TPD_DEBUG(KERN_EMERG "%s: cmd %d", __func__, _IOC_NR(cmd));
    TPD_DEBUG("btl %s cmd=%d\n", __func__, cmd);

    //handle ioctl
    switch(cmd) {
    case LTR_IOCTL_GET_PFLAG:
        //case LTR_IOCTL_GET_LFLAG:
        //flag = atomic_read(&p_flag);
        plag = PROXIMITY_STATE;
        if(copy_to_user(argp, &flag, sizeof(flag))) {
            return -EFAULT;
        }
        break;

    case LTR_IOCTL_GET_DATA:
        break;

    case LTR_IOCTL_GET_CHIPINFO:
        sprintf(strbuf, "LTR558ALS");
        if(copy_to_user(argp, strbuf, strlen(strbuf) + 1))
            return -EFAULT;
        break;

    case LTR_IOCTL_SET_PFLAG:
        //case LTR_IOCTL_SET_LFLAG:
        if(copy_from_user(&flag, argp, sizeof(flag))) {
            return -EFAULT;
        }

        TPD_DEBUG("LTR_IOCTL_SET_PFLAG=%d\n", flag);
        if(flag == 1) {
            TP_face_mode_switch(1);
        } else if(flag == 0) {
            TP_face_mode_switch(0);
        }
        break;

    default:
        TPD_DEBUG("btl %s: invalid cmd %d\n", __func__, _IOC_NR(cmd));
        return -EINVAL;
    }

    return 0;
}

static struct file_operations bl_ps_fops = {
    .owner      = THIS_MODULE,
    .open       = bl_ps_open,
    .release    = bl_ps_release,
    .unlocked_ioctl = bl_ps_unlocked_ioctl,
};

static struct miscdevice bl_ps_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = BTL_PS_DEVICE,
    .fops = &bl_ps_fops,
};
#endif

#endif//BTL_CTP_PROXIMITY_SENSOR  fops end


//begin add by bo_liu for 11022639
static int power_state = 0;

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



static int tpd_local_init(void)
{
    int retval;

    TPD_DEBUG("btl I2C Touchscreen Driver...\n");
    tpd->reg = regulator_get(tpd->tpd_dev, "vtouch");
    retval = regulator_set_voltage(tpd->reg, 2800000, 2800000);
    if(retval != 0) {
        TPD_DEBUG("btl Failed to set reg-vtouch voltage: %d\n", retval);
	printk("btl Failed to set reg-vtouch voltage: %d\n", retval);
	//begin add by bo_liu for 11022639
	retval = semi_power_source_ctrl(0);
	//end add by bo_liu for 11022639
        return -1;
    }

	//begin add by bo_liu for 11022639
	if (retval == 0)
	{
		power_state = 1;
	}

	retval = semi_power_source_ctrl(1);

	//end add by bo_liu for 11022639


    if(i2c_add_driver(&tpd_i2c_driver) != 0) {
        TPD_DEBUG("btl unable to add i2c driver.\n");
        return -1;
    }

    /* tpd_load_status = 1; */
    /*if (tpd_dts_data.use_tpd_button) {
        tpd_button_setting(tpd_dts_data.tpd_key_num, tpd_dts_data.tpd_key_local,
        tpd_dts_data.tpd_key_dim_local);
    }*/

    TPD_DEBUG("btl end %s, %d\n", __func__, __LINE__);
    //tpd_type_cap = 1;

    return 0;
}

void btl_chip_reset(void)
{
#ifdef BTL_NEED_RESET_PIN
    tpd_gpio_output(btl_tpd_rst_gpio_number, 0);
    msleep(5);//20
    tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
    msleep(40);//100
#else
    bl_ts_set_int_gpio(0);
    msleep(5);//20
    bl_ts_set_int_gpio(1);
    msleep(40);//100
    tpd_gpio_as_int(btl_tpd_int_gpio_number);
#endif
}


extern char ctp_module_name[256];

static int tpd_probe(struct i2c_client* client, const struct i2c_device_id* id)
{
    int retval = 0;
    btl_i2c_client = client;

    if(btl_i2c_client->addr != BTL_SLAVE_ADDR) {
        BF_TP_LOG("btl:1 i2c_client_btl->addr = %02x\n", btl_i2c_client->addr);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;
        BF_TP_LOG("btl:2 i2c_client_btl->addr = %02x\n", btl_i2c_client->addr);
    }

    mutex_init(&i2c_lock);
    spin_lock_init(&irq_lock);

//begin add by bo_liu for 11022639
#if 0
    retval = regulator_enable(tpd->reg);
    if(retval != 0) {
        BF_TP_LOG("btl Failed to enable reg-vtouch: %d\n", retval);
    } else {
        BF_TP_LOG(" btl successs to enable reg-vtouch: %d\n", retval);
    }
#endif
//end add by bo_liu for 11022639

    BF_TP_LOG("mtk_tpd: tpd_probe btl\n");
#ifdef CONFIG_MTK_I2C_EXTENSION
			btl_dma_alloct();
#endif//CONFIG_MTK_I2C_EXTENSION

#ifdef CONFIG_BTL_CHECK_CHIPID
    {
        u8 chip_id = 0x00;
        btl_chip_reset();
        msleep(5);//50
        bl_i2c_lock(1);
        TPD_DEBUG("btl check chipid");
        retval = bl_get_chip_id(&chip_id);
        bl_i2c_lock(0);
        if(retval < 0 || chip_id != BTL_FLASH_ID) {
            BF_TP_LOG("BL6xx0 check chipid  error :chip_id = 0x%02x", chip_id);
	//begin add by bo_liu for 11022639
		semi_power_source_ctrl(0);
	//end add by bo_liu for 11022639
            return -ENODEV;
        } else {
            BF_TP_LOG("BL6xx0 chip_id = 0x%02x", chip_id);
        }
        btl_chip_reset();
        msleep(5);//50
        device_create_file(&client->dev, &dev_attr_chipid);
    }
#endif


#ifdef BL_AUTO_UPDATE_FARMWARE
    msleep(50);
    BF_TP_LOG("btl auto update fw\n");
	bl_i2c_lock(1);
    bl_update_fw(btl_i2c_client);
	bl_i2c_lock(0);
    /* Reset CTP */
    btl_chip_reset();
#endif//BL_AUTO_UPDATE_FARMWARE


    of_get_bl_platform_data(&client->dev);
    device_create_file(&client->dev, &dev_attr_reg);

#ifdef BL_UPDATE_FARMWARE_FROM_BIN
    device_create_file(&client->dev, &dev_attr_fwupdate);
#endif
    device_create_file(&client->dev, &dev_attr_kernelversion);
//add by xucs start
    device_create_file(&client->dev, &dev_attr_log_level);
//add by xucs end
#ifdef TPD_HAVE_TOUCH_ID_BUTTON
    set_bit(KEY_MENU, tpd->dev->keybit);
    set_bit(KEY_HOMEPAGE, tpd->dev->keybit);
    set_bit(KEY_BACK, tpd->dev->keybit);
#endif


    TPD_DEBUG("btl tpd_gpio_as_int gpio:%d\n", btl_tpd_int_gpio_number);
    tpd_gpio_as_int(btl_tpd_int_gpio_number);

    tpd_irq_registration();
    //bl_enable_irq();
    tpd_load_status = 1;

    snprintf(ctp_module_name, 256, "A3133A0:TWS\n");

#ifdef BTL_APK_SUPPORT
    /* Create Proc Entry File */
    init_waitqueue_head(&debug_queue);
    debug_sync_flag = 0;
    is_apk_debug = 0;
    proc_entry = proc_create("Betterlife_ts", 0777, NULL, &bl_apk_fops);
#endif//BTL_APK_SUPPORT

    /* allocate input device */
#ifdef BTL_CTP_PROXIMITY_SENSOR
    ps_input_dev = input_allocate_device();
    ps_input_dev->name = BTL_PS_INPUT_DEV;
    set_bit(EV_ABS, ps_input_dev->evbit);
    input_set_abs_params(ps_input_dev, ABS_DISTANCE, 0, 1, 0, 0);
    retval = input_register_device(ps_input_dev);
    if(retval) {
        BF_TP_LOG("btl tpd_probe: failed to register input device\n");
    }

#if 0
    retval = misc_register(&bl_ps_device);
    if(retval) {
        TPD_DEBUG("btl %s: bl_ps_device register failed\n", __func__);
    }
#endif
#endif/*BTL_CTP_PROXIMITY_SENSOR*/

#ifdef  BTL_CTP_GESTURE_ENABLE
    retval = bl_ts_gesture_init(tpd->dev);
    if(retval) {
        BF_TP_LOG("bl init gesture fail");
    }
#endif
//add by xucs start
#ifdef BTL_FACTORY_TEST_EN
    retval = btl_test_init();
    if (retval) {
        BF_TP_LOG("btl_test_init fail");
    }
#endif
//add by xucs end

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
    bl_ts_ac_usb_power_detect_init(&ac_usb_dwork);
#endif

    thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
    if(IS_ERR(thread)) {
        retval = PTR_ERR(thread);
        BF_TP_LOG("btl failed to create kernel thread \n");
    }

    BF_TP_LOG("btl Touch Panel Device Probe %s\n", (retval < TPD_OK) ? "FAIL" : "PASS");

#if defined(TIMER_DEBUG)
    init_test_timer();
#endif

    return 0;
}

static void tpd_suspend(struct device* h)
{
#ifndef BTL_CTP_GESTURE_ENABLE
    int retval = TPD_OK;
    char data = 0x3;
#endif

    BF_TP_LOG("lqw   111111 TPD enter sleep\n");

#ifdef BTL_CTP_PROXIMITY_SENSOR
    TPD_DEBUG("btl tpd_suspend ril_call_state =%d\n", ril_call_state);
    if(ril_call_state == 1)
        return ;
#endif

#ifdef BTL_ESD_PROTECT
    reg_value_F9_pre = 0xFF;
    esd_mode_kernel_flag = 0;

    //bl_is_suspend = 1;
    bl_esd_switch(SWITCH_OFF);
#endif

#ifdef  BTL_CTP_GESTURE_ENABLE

#else//BTL_CTP_GESTURE_ENABLE

    bl_disable_irq();

    bl_i2c_lock(1);
    BF_TP_LOG("lqw   22222222222 TPD enter sleep\n");
    i2c_smbus_write_i2c_block_data(btl_i2c_client, 0xA5, 1, &data);  /* TP enter sleep mode */
    BF_TP_LOG("lqw   3333333333 TPD enter sleep\n");
    bl_i2c_lock(0);

//begin add by bo_liu for 11022639
#if 0
    retval = regulator_disable(tpd->reg);
#endif
	retval = semi_power_source_ctrl(0);
//end add by bo_liu for 11022639
    if(retval != 0)
        TPD_DMESG("Failed to disable reg-vtouch: %d\n", retval);
#endif//BTL_CTP_GESTURE_ENABLE
    btl_release_all_points();
    bl_is_suspend = 1;

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
    cancel_delayed_work_sync(&ac_usb_dwork);
#endif

}

static void tpd_resume(struct device* h)
{
    u8 retval = 0;

    BF_TP_LOG("lqw  2222222 TPD wake up\n");

#ifdef BTL_CTP_PROXIMITY_SENSOR
    TPD_DEBUG("btl tpd_resume ril_call_state =%d\n", ril_call_state);
    if(ril_call_state == 1)
        return;
#endif

#ifdef  BTL_CTP_GESTURE_ENABLE

    btl_read_reg(btl_i2c_client, 0xA5, &retval);

#else//BTL_CTP_GESTURE_ENABLE
//begin add by bo_liu for 11022639
#if 0
    retval = regulator_enable(tpd->reg);
#endif
	retval = semi_power_source_ctrl(1);
//end add by bo_liu for 11022639
    if(retval != 0)
        TPD_DMESG("Failed to enable reg-vtouch: %d\n", retval);

    msleep(5);//50

    btl_chip_reset();
    bl_enable_irq();
#endif//BTL_CTP_GESTURE_ENABLE

    bl_is_suspend = 0;

#ifdef BTL_ESD_PROTECT
    bl_esd_switch(SWITCH_ON);
#endif

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
    schedule_delayed_work(&ac_usb_dwork, msecs_to_jiffies(POLLING_TIME));
#endif
}

static int tpd_remove(struct i2c_client* client)
{
    TPD_DEBUG("TPD removed\n");

#ifdef CONFIG_MTK_I2C_EXTENSION
    btl_dma_release();
#endif

#ifdef BTL_APK_SUPPORT
    if(proc_entry != NULL) {
        remove_proc_entry("Betterlife_ts", NULL);
    }
#endif
//add by xucs start
#if defined(BTL_FACTORY_TEST_EN)
		btl_test_exit();
#endif
//add by xucs end

#ifdef BTL_ESD_PROTECT
    destroy_workqueue(bl_esd_check_workqueue);
#endif

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
    cancel_delayed_work_sync(&ac_usb_dwork);
#endif

    return 0;
}

static struct tpd_driver_t tpd_device_driver = {
    .tpd_device_name = "betterlife_ts",
    .tpd_local_init = tpd_local_init,
    .suspend = tpd_suspend,
    .resume = tpd_resume,
};

/* called when loaded into kernel */
static int __init tpd_driver_init(void)
{
    BF_TP_LOG("MediaTek btl touch panel driver init\n");
    tpd_get_dts_info();
    if(tpd_driver_add(&tpd_device_driver) < 0)
        BF_TP_LOG("MediaTek add btl driver failed\n");

    return 0;
}

/* should never be called */
static void __exit tpd_driver_exit(void)
{
    TPD_DEBUG("MediaTek btl touch panel driver exit\n");
    tpd_driver_remove(&tpd_device_driver);
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);

