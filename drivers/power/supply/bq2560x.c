#include <linux/types.h>
#include <linux/init.h>		/* For init/exit macros */
#include <linux/module.h>	/* For MODULE_ marcros  */
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/string.h> /* Added by bitao.xiong for task-7884939 on 2019-07-04 */
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif
#include "charger_class.h"
//#include "mtk_charger.h"
//#if defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
//#include <mt-plat/charger_class.h>
//#include "mtk_charger_intf.h"
//#endif /* #if (CONFIG_MTK_GAUGE_VERSION == 30) */

#include "bq2560x.h"
/* compatible with charge ic hl7019 */


#define STATUS_OK    0
#define STATUS_UNSUPPORTED    -1
#define STATUS_FAIL -2
#define GETARRAYNUM(array) (sizeof(array)/sizeof(array[0]))

/* ============================================================ // */
/* Global variable */
/*-Begin added by dapeng.qiao for  task 8997711 on 2020-03-16*/
//extern int g_bq2560x_hw_exist;
//extern int g_sgm41511_hw_exist;
static  int g_bq2560x_hw_exist;
static  int g_sgm41511_hw_exist;

//int g_bq2560x_hw_ver;
/*-End added by dapeng.qiao for  task 8997711 on 2020-03-16-*/
/* ============================================================ // */

#if defined(MTK_WIRELESS_CHARGER_SUPPORT)
#define WIRELESS_CHARGER_EXIST_STATE 0

#if defined(GPIO_PWR_AVAIL_WLC)
unsigned int wireless_charger_gpio_number = GPIO_PWR_AVAIL_WLC;
#else
unsigned int wireless_charger_gpio_number = 0;
#endif

#endif



/* ============================================================ // */
/* function prototype */
/* ============================================================ // */
//#define CHARGE_LOG_PRINT 1 //jjj for debug

/* ============================================================ // */
/* extern variable */
/* ============================================================ // */

/* ============================================================ // */
/* extern function */
/* ============================================================ // */
static unsigned int bmt_find_closest_level(const unsigned int *pList, unsigned int number,
					 unsigned int level);
//static unsigned int charging_error;
//static unsigned int charging_get_error_state(void);
//static unsigned int charging_set_error_state(void);
//static unsigned int g_input_current;
/**********************************************************
  *
  *   [I2C Slave Setting]
  *
  *********************************************************/
#define BQ2560X_SLAVE_ADDR         0x6B
#define BQ2560x_SLAVE_ADDR_WRITE   0xD6
#define BQ2560x_SLAVE_ADDR_READ    0xD7

bool HL7019_EXIST_FALG = false;
bool BQ25601_EXIST_FALG = false;
/* Begin added by bitao.xiong for defect-8423557 on 2019-10-16 */
bool SGM41511_EXIST_FLAG = false;
/* End added by bitao.xiong for defect-8423557 on 2019-10-16 */

//static struct i2c_client *new_client;
//static const struct i2c_device_id bq2560x_i2c_id[] = { {"bq2560x", 0}, {} };


//static int bq2560x_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);

#if 0
static void bq2560x_shutdown(struct platform_device *dev)
{
	pr_info("[bq2560x_shutdown] driver shutdown\n");
	bq2560x_set_chg_config(0x0);	/* charger disable */
}

#endif

struct BQ2560X_INFO {
	struct device *dev;
	struct i2c_client *i2c;
#if 1//defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
	struct charger_device *chg_dev;
#endif /* #if (CONFIG_MTK_GAUGE_VERSION == 30) */
	bool charge_enabled;/* Register bit status */

};

struct BQ2560X_INFO *bq2560x_info = NULL;

static const struct i2c_device_id bq2560x_i2c_id[] = { {"bq2560x", 0}, {} };

//kal_bool chargin_hw_init_done = false;
bool chargin_hw_init_done = false;
static int bq2560x_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);
#ifdef CONFIG_OF
static const struct of_device_id bq2560x_of_match[] = {
	{.compatible = "mediatek,swithing_charger",},
	{},
};

MODULE_DEVICE_TABLE(of, bq2560x_of_match);
#endif

static struct i2c_driver bq2560x_driver = {
	.driver = {
		   .name = "bq2560x",
#ifdef CONFIG_OF
		   .of_match_table = bq2560x_of_match,
#endif
		   },
	.probe = bq2560x_driver_probe,
	.id_table = bq2560x_i2c_id,
	//.shutdown = bq2560x_shutdown,
};

/**********************************************************
  *
  *   [Global Variable]
  *
  *********************************************************/
unsigned char bq2560x_reg[bq2560x_REG_NUM] = { 0 };

static DEFINE_MUTEX(bq2560x_i2c_access);
int g_charge_ic_hw_exist = 0;
/**********************************************************
  *
  *   [I2C Function For Read/Write bq2560x]
  *
  *********************************************************/
int bq2560x_read_byte(unsigned char cmd, unsigned char *returnData)
{
	char cmd_buf[1] = { 0x00 };
	char readData = 0;
	int ret = 0;

	mutex_lock(&bq2560x_i2c_access);

	cmd_buf[0] = cmd;
	ret = i2c_master_send(bq2560x_info->i2c, &cmd_buf[0], 1);
	if (ret < 0) {

		mutex_unlock(&bq2560x_i2c_access);

		return 0;
	}

	ret = i2c_master_recv(bq2560x_info->i2c, &cmd_buf[0], 1);
	if (ret < 0) {
		mutex_unlock(&bq2560x_i2c_access);
		return 0;
	}

	readData = cmd_buf[0];
	*returnData = readData;

	mutex_unlock(&bq2560x_i2c_access);

	return 1;
}

int bq2560x_write_byte(unsigned char cmd, unsigned char writeData)
{
	char write_data[2] = { 0 };
	int ret = 0;

	mutex_lock(&bq2560x_i2c_access);

	write_data[0] = cmd;
	write_data[1] = writeData;

	ret = i2c_master_send(bq2560x_info->i2c, write_data, 2);
	if (ret < 0) {
		mutex_unlock(&bq2560x_i2c_access);
		return 0;
	}

	mutex_unlock(&bq2560x_i2c_access);
	return 1;
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
unsigned int bq2560x_read_interface(unsigned char RegNum, unsigned char *val, unsigned char MASK,
				  unsigned char SHIFT)
{
	unsigned char bq2560x_reg = 0;
	int ret = 0;

	pr_info("read----------RegNum = 0x%x, *val = 0x%x, MASK = %x, SHIFT = %x\n",RegNum,*val,MASK,SHIFT);

	ret = bq2560x_read_byte(RegNum, &bq2560x_reg);
#ifdef CHARGE_LOG_PRINT
	pr_info("qqq, [bq2560x_read_interface] Reg[%x]= 0x%x\n", RegNum, bq2560x_reg);
#endif
	bq2560x_reg &= (MASK << SHIFT);
	*val = (bq2560x_reg >> SHIFT);
#ifdef CHARGE_LOG_PRINT
	pr_info("qqq, [bq2560x_read_interface] Val=0x%x\n", *val);
#endif
	return ret;
}

unsigned int bq2560x_config_interface(unsigned char RegNum, unsigned char val, unsigned char MASK,
				    unsigned char SHIFT)
{
	unsigned char bq2560x_reg = 0;
	int ret = 0;

	pr_info("write---------- RegNum = 0x%x, *val = 0x%x, MASK = %x, SHIFT = %x\n",RegNum,val,MASK,SHIFT);

	ret = bq2560x_read_byte(RegNum, &bq2560x_reg);
#ifdef CHARGE_LOG_PRINT
	pr_info("qqq, [bq2560x_config_interface] Reg[%x]=0x%x\n", RegNum, bq2560x_reg);
#endif
	bq2560x_reg &= ~(MASK << SHIFT);
	bq2560x_reg |= (val << SHIFT);

	ret = bq2560x_write_byte(RegNum, bq2560x_reg);
#ifdef CHARGE_LOG_PRINT
	pr_info("qqq, [bq2560x_config_interface] Write Reg[%x]=0x%x\n", RegNum, bq2560x_reg);
#endif
	/* Check */
	bq2560x_read_byte(RegNum, &bq2560x_reg);
#ifdef CHARGE_LOG_PRINT
	pr_info("qqq, [bq2560x_config_interface] Check Reg[%x]=0x%x\n", RegNum, bq2560x_reg);
#endif
	return ret;
}

/* write one register directly */
unsigned int bq2560x_reg_config_interface(unsigned char RegNum, unsigned char val)
{
	int ret = 0;

	ret = bq2560x_write_byte(RegNum, val);

	return ret;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
/* CON0---------------------------------------------------- */
//0 \A8C Disable, 1 \A8C Enable      Default: Disable (0)
void bq2560x_set_en_hiz(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_EN_HIZ_MASK),
				       (unsigned char) (CON0_EN_HIZ_SHIFT)
	    );
}

/* Begin added by bitao.xiong for defect-8180576 on 2019-08-03 */
int bq2560x_get_en_hiz_stat(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON0),
		(&val),
		(unsigned char) (CON0_EN_HIZ_MASK),
		(unsigned char) (CON0_EN_HIZ_SHIFT)
	);

	return val;
}
/*End added by bitao.xiong for defect-8180576 on 2019-08-03 */

//EN_ICHG_MON
//00 - Enable STAT pin function(default)
//01 - Reserved
//10 - Reserved
//11 - Disable STAT
void bq2560x_set_stat_ctrl(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_STAT_IMON_CTRL),
				       (unsigned char) (CON0_STAT_IMON_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_stat_ctrl(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_STAT_IMON_HL_CTRL),
				       (unsigned char) (CON5_STAT_IMON_HL_SHIFT)
	    );
}
#endif

//Input Current Limit
//Offset: 100 mA
//Range: 100 mA (000000) \A8C 3.2 A(11111),1600mA,800mA,400mA,200mA,100mA
//Default:2400 mA (10111),maximum input current limit, not typical.
//IINDPM bits are changed automatically after input source
//detection is completed
//PSEL = Hi = 500 mA
//PSEL = Lo = 2.4 A
//Host can over-write IINDPM register bits after input source detection is completed.
void bq2560x_set_iinlim(unsigned int val)
{
	unsigned int ret = 0;

#ifdef CHARGE_LOG_PRINT
	pr_info("[%s][%d] qqq, dump function\n",__func__,__LINE__);
#endif
	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_IINLIM_MASK),
				       (unsigned char) (CON0_IINLIM_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_iinlim(unsigned int val)
{
	unsigned int ret = 0;

#ifdef CHARGE_LOG_PRINT
	pr_info("[%s][%d] qqq, dump function\n",__func__,__LINE__);
#endif
	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_IINLIM_HL_MASK),
				       (unsigned char) (CON0_IINLIM_HL_SHIFT)
	    );
}

unsigned int hl7019_get_iinlim(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON0),
		(&val),
		(unsigned char) (CON0_IINLIM_HL_MASK),
		(unsigned char) (CON0_IINLIM_HL_SHIFT)
	);
	return val;
}
#endif

unsigned int bq2560x_get_iinlim(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON0),
		(&val),
		(unsigned char) (CON0_IINLIM_MASK),
		(unsigned char) (CON0_IINLIM_SHIFT)
	);
	return val;
}


/* CON1---------------------------------------------------- */
#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_reg_rst(unsigned char val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_REG_RST_MASK),
				       (unsigned char) (CON1_REG_RST_SHIFT)
	    );
}
#endif


//I2C Watchdog Timer Reset 0 \A8CNormal ; 1 \A8C Reset
void bq2560x_set_wdt_rst(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_WDT_RST_MASK),
				       (unsigned char) (CON1_WDT_RST_SHIFT)
	    );
}

//0 \A8C OTG Disable 1 \A8C OTG Enable
void bq2560x_set_otg_config(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_OTG_CONFIG_MASK),
				       (unsigned char) (CON1_OTG_CONFIG_SHIFT)
	    );
}

//Default: Charge Battery (1)     0 - Charge Disable 1- Charge Enable
//Note:1. Charge is enabled when both CE pin is pulled low AND CHG_CONFIG bit is 1.
void bq2560x_set_chg_config(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_CHG_CONFIG_MASK),
				       (unsigned char) (CON1_CHG_CONFIG_SHIFT)
	    );
}

//System Minimum Voltage
//000: 2.6 V
//001: 2.8 V
//010: 3 V
//011: 3.2 V
//100: 3.4 V
//101: 3.5 V
//110: 3.6 V
//111: 3.7 V
//Default: 3.5 V (101)
void bq2560x_set_sys_min(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_SYS_MIN_MASK),
				       (unsigned char) (CON1_SYS_MIN_SHIFT)
	    );
}

//Min_VBAT_SEL
//0 \A8C 2.8 V BAT falling,
//1 \A8C 2.5 V BAT falling
//Minimum battery voltage for OTG mode. Default falling 2.8 V (0);Rising threshold 3.0 V (0)
void bq2560x_set_batlowv(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_MIN_VBAT_SEL_MASK),
				       (unsigned char) (CON1_MIN_VBAT_SEL_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_batlowv(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_MIN_VBAT_SEL_HL_MASK),
				       (unsigned char) (CON4_MIN_VBAT_SEL_HL_SHIFT)
	    );
}
#endif

/* CON2---------------------------------------------------- */
//BOOST_LIM
//0 = 0.5 A,1 = 1.2 A,  Default: 1.2 A (1)
//Note:The current limit options listed are minimum current limit specs.
void bq2560x_set_boost_lim(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_BOOST_LIM_MASK),
				       (unsigned char) (CON2_BOOST_LIM_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_boost_lim(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_BOOST_LIM_HL_MASK),
				       (unsigned char) (CON1_BOOST_LIM_HL_SHIFT)
	    );
}
#endif

//0 \A8C Use higher Q1 RDSON when programmed IINDPM < 700mA(better accuracy)
//1 \A8C Use lower Q1 RDSON always(better efficiency)
//In boost mode, full FET is always used and this bit has no effect
void bq2560x_set_Q1_FULLON(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_Q1_FULLON_MASK),
				       (unsigned char) (CON2_Q1_FULLON_SHIFT)
	    );
}

//Fast Charge Current
//1920mA,960mA,480mA,240mA,120mA,60mA
//Default: 2040mA (100010)
//Range: 0 mA (0000000) \A8C 3000mA (110010)
//Note:ICHG = 0 mA disables charge.ICHG > 3000 mA (110010 clamped to register value 3000 mA(110010))
void bq2560x_set_ichg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface(
		(unsigned char) (bq2560x_CON2),
		(unsigned char) (val),
		(unsigned char) (CON2_ICHG_MASK),
		(unsigned char) (CON2_ICHG_SHIFT)
	);
}

unsigned int bq2560x_get_ichg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON2),
		(&val),
		(unsigned char) (CON2_ICHG_MASK),
		(unsigned char) (CON2_ICHG_SHIFT)
	);

	return val;
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_ichg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface(
		(unsigned char) (bq2560x_CON2),
		(unsigned char) (val),
		(unsigned char) (CON2_ICHG_HL_MASK),
		(unsigned char) (CON2_ICHG_HL_SHIFT)
	);
}

unsigned int hl7019_get_ichg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON2),
		(&val),
		(unsigned char) (CON2_ICHG_HL_MASK),
		(unsigned char) (CON2_ICHG_HL_SHIFT)
	);

	return val;
}

void hl7019_set_bcold(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface(
		(unsigned char) (bq2560x_CON2),
		(unsigned char) (val),
		(unsigned char) (CON2_BCOLD_HL_MASK),
		(unsigned char) (CON2_BCOLD_HL_SHIFT)
	);
}
void hl7019_set_force_20pct(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface(
		(unsigned char) (bq2560x_CON2),
		(unsigned char) (val),
		(unsigned char) (CON2_FORCE_20PCT_HL_MASK),
		(unsigned char) (CON2_FORCE_20PCT_HL_SHIFT)
	);
}
#endif

/* CON3---------------------------------------------------- */
//Precharge Current
//480mA,240mA,120mA,60mA
//Default: 180 mA (0010)
//Offset: 60 mA
//Note: IPRECHG > 780 mA
//clamped to 780 mA (1100)
void bq2560x_set_iprechg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON3),
				       (unsigned char) (val),
				       (unsigned char) (CON3_IPRECHG_MASK),
				       (unsigned char) (CON3_IPRECHG_SHIFT)
	    );
}

//Termination Current
//480mA,240mA,120mA,60mA
//Default: 180 mA (0010)
//Offset: 60 mA
void bq2560x_set_iterm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON3),
				       (unsigned char) (val),
				       (unsigned char) (CON3_ITERM_MASK), (unsigned char) (CON3_ITERM_SHIFT)
	    );
}


unsigned int bq2560x_get_iterm(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON3),
		(&val),
		(unsigned char) (CON3_ITERM_MASK),
		(unsigned char) (CON3_ITERM_SHIFT)
	);
	return val;
}

/* CON4---------------------------------------------------- */
//Charge Voltage
//Offset: 3.856 V
//Range: 3.856 V to 4.624 V(11000)
//512mV,256mV,128mV,64mV,32mV
//Default: 4.208 V (01011)
//Special Value:(01111): 4.352 V
//Note: Value above 11000 (4.624V) is clamped to
void bq2560x_set_vreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VREG_MASK), (unsigned char) (CON4_VREG_SHIFT)
	    );
}

unsigned int bq2560x_get_vreg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
	(unsigned char) (bq2560x_CON4),
	   (&val),
	   (unsigned char) (CON4_VREG_MASK),
	   (unsigned char) (CON4_VREG_SHIFT)
	);

	return val;
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_vreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VREG_HL_MASK), (unsigned char) (CON4_VREG_HL_SHIFT)
	    );
}

unsigned int hl7019_get_vreg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
	(unsigned char) (bq2560x_CON4),
	   (&val),
	   (unsigned char) (CON4_VREG_HL_MASK),
	   (unsigned char) (CON4_VREG_HL_SHIFT)
	);

	return val;
}

/* Begin added by bitao.xiong for task-8430235 on 2019-12-02 */
void hl7019_set_ir_compensation(unsigned int val_r, unsigned int val_vol)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (HL7019_CON_0C),
				       (unsigned char) (val_r),
				       (unsigned char) (CON12_IR_R_MASK), (unsigned char) (CON12_IR_R_SHIFT)
	    );
	ret = bq2560x_config_interface((unsigned char) (HL7019_CON_0C),
					(unsigned char) (val_vol),
					(unsigned char) (CON12_IR_VOL_MASK), (unsigned char) (CON12_IR_VOL_SHIFT)
		);
}
/* End added by bitao.xiong for task-8430235 on 2019-12-02 */
#endif

//TOPOFF_TIMER
//The extended time following the termination condition is met.
//When disabled, charge terminated when termination conditions are met
//00 \A8C Disabled (Default)
//01 \A8C 15 minutes
//10 \A8C 30 minutes
//11 \A8C 45 minutes
void bq2560x_set_topoff_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_TOPOFF_TIMER_MASK), (unsigned char) (CON4_TOPOFF_TIMER_SHIFT)
	    );

}

//Recharge threshold
//Default: 100mV (0)
//0 \A8C 100 mV
//1 \A8C 200 mV
void bq2560x_set_vrechg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VRECHG_MASK),
				       (unsigned char) (CON4_VRECHG_SHIFT)
	    );
}

/* CON5---------------------------------------------------- */
//Default: Enable termination (1) 0 \A8C Disable  1 \A8C Enable
void bq2560x_set_en_term(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_EN_TERM_MASK),
				       (unsigned char) (CON5_EN_TERM_SHIFT)
	    );
}


//00 \A8C Disable timer, 01 \A8C 40 s, 10 \A8C 80 s,11 \A8C 160 s
//Default: 40 s (01)
void bq2560x_set_watchdog(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_WATCHDOG_MASK),
				       (unsigned char) (CON5_WATCHDOG_SHIFT)
	    );
}

//0 \A8C Disable
//1 \A8C Enable both fast charge and precharge timer
//Default: Enable (1)
void bq2560x_set_en_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_EN_TIMER_MASK),
				       (unsigned char) (CON5_EN_TIMER_SHIFT)
	    );
}

//0 \A8C 5 hrs
//1 \A8C 10 hrs
//Default: 10 hours (1)
void bq2560x_set_chg_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_CHG_TIMER_MASK),
				       (unsigned char) (CON5_CHG_TIMER_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_chg_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_CHG_TIMER_HL_MASK),
				       (unsigned char) (CON5_CHG_TIMER_HL_SHIFT)
	    );
}
#endif

//Thermal Regulation Threshold:0 - 90\A1\E3C,   1 - 110\A1\E3C
void bq2560x_set_treg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_TREG_MASK),
				       (unsigned char) (CON5_TREG_SHIFT)
	    );
}

/* CON6---------------------------------------------------- */
//VAC OVP threshold:
//00 - 5.5 V
//01 \A8C 6.5 V (5-V input)
//10 \A8C 10.5 V (9-V input)
//11 \A8C 14 V (12-V input)
//Default: 6.5V (01)
void bq2560x_set_ovp(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_OVP_MASK),
				       (unsigned char) (CON6_OVP_SHIFT)
	    );

}

//Boost Regulation Voltage:
//00 - 4.85V
//01 - 5.00V
//10 - 5.15V
//11 - 5.30V
void bq2560x_set_boostv(unsigned int val)
{

	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_BOOSTV_MASK),
				       (unsigned char) (CON6_BOOSTV_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_boostv(unsigned int val)
{

	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_BOOSTV_HL_MASK),
				       (unsigned char) (CON6_BOOSTV_HL_SHIFT)
	    );
}

void hl7019_set_bhot(unsigned int val)
{

	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_BHOT_HL_MASK),
				       (unsigned char) (CON6_BHOT_HL_SHIFT)
	    );
}

void hl7019_set_treg(unsigned int val)
{

	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_TREG_HL_MASK),
				       (unsigned char) (CON6_TREG_HL_SHIFT)
	    );
}
#endif

//Absolute VINDPM Threshold
//Offset: 3.9 V
//800mV,400mV,200mV,100mV
//Range: 3.9 V (0000) \A8C 5.4 V(1111)
//Default: 4.5V (0110)
void bq2560x_set_vindpm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VINDPM_MASK),
				       (unsigned char) (CON6_VINDPM_SHIFT)
	    );
}

unsigned int bq2560x_get_vindpm_reg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON6),
				     (&val), (unsigned char) (CON6_VINDPM_MASK), (unsigned char) (CON6_VINDPM_SHIFT)
	    );
	return val;
}

#ifdef COMPATIBLE_WITH_HL7019

void hl7019_set_vindpm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_VINDPM_HL_MASK),
				       (unsigned char) (CON0_VINDPM_HL_SHIFT)
	    );
}

unsigned int hl7019_get_vindpm_reg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON0),
				     (&val), (unsigned char) (CON0_VINDPM_HL_MASK), (unsigned char) (CON0_VINDPM_HL_SHIFT)
	    );
	return val;
}

#endif



/* CON7---------------------------------------------------- */
//0 \A8C Disable
//1 \A8C Safety timer slowed by 2X
//during input DPM (both V and I) or JEITA cool, or thermal regulation
void bq2560x_set_tmr2x_en(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_TMR2X_EN_MASK),
				       (unsigned char) (CON7_TMR2X_EN_SHIFT)
	    );
}

//0 \A8C Allow Q4 turn on, 1 \A8C Turn off
//Q4 with tBATFET_DLY delay time (REG07[3])
//Default: Allow Q4 turn on(0)
void bq2560x_set_batfet_disable(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_BATFET_DISABLE_MASK),
				       (unsigned char) (CON7_BATFET_DISABLE_SHIFT)
	    );
}

//0 \A8C Turn off BATFET immediately when BATFET_DIS bit is set
//1 \A8CTurn off BATFET after tBATFET_DLY (typ. 10 s) when BATFET_DIS bit is set
//Default: 1 Turn off BATFET after tBATFET_DLY (typ. 10 s) when BATFET_DIS bit is set
void bq2560x_set_batfet_delay(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_BATFET_DLY_MASK),
				       (unsigned char) (CON7_BATFET_DLY_SHIFT)
	    );
}

//0 \A8C Disable BATFET reset function
//1 \A8C Enable BATFET reset function
//Default: 1 Enable BATFET reset function
void bq2560x_set_batfet_reset_enable(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_BATFET_RST_EN_MASK),
				       (unsigned char) (CON7_BATFET_RST_EN_SHIFT)
	    );
}

//00 - Disable function (VINDPM set by register)
//01 - VBAT + 200mV
//10 - VBAT + 250mV
//11 - VBAT + 300mV
//Sets VINDPM to track BAT voltage.
//Actual VINDPM is higher of register value and VBAT + VDPM_BAT_TRACK
void bq2560x_set_VDPM_bat_track(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_VDPM_BAT_TRACK_MASK),
				       (unsigned char) (CON7_VDPM_BAT_TRACK_SHIFT)
	    );
}

#ifdef COMPATIBLE_WITH_HL7019
void hl7019_set_int_mask(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_INT_MASK_HL_MASK),
				       (unsigned char) (CON7_INT_MASK_HL_SHIFT)
	    );
}
#endif

/* CON8---------------------------------------------------- */
//get all status
unsigned int bq2560x_get_system_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val), (unsigned char) (0xFF), (unsigned char) (0x0)
	    );
	return val;
}

//VBUS Status register
//000: No input
//001: USB Host SDP (500 mA) \A1\FA PSEL HIGH
//010: Adapter 2.4A \A1\FA PSEL LOW
//111: OTG
//Software current limit is reported in IINDPM register
unsigned int bq2560x_get_vbus_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val),
				     (unsigned char) (CON8_VBUS_STAT_MASK),
				     (unsigned char) (CON8_VBUS_STAT_SHIFT)
	    );
	return val;
}

//Charging status:
//00 \A8C Not Charging
//01 \A8C Pre-charge (< VBATLOWV)
//10 \A8C Fast Charging
//11 \A8C Charge Termination
unsigned int bq2560x_get_chrg_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val),
				     (unsigned char) (CON8_CHRG_STAT_MASK),
				     (unsigned char) (CON8_CHRG_STAT_SHIFT)
	    );
	return val;
}

#ifdef COMPATIBLE_WITH_HL7019
unsigned int hl7019_get_chrg_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val),
				     (unsigned char) (CON8_CHRG_STAT_HL_MASK),
				     (unsigned char) (CON8_CHRG_STAT_HL_SHIFT)
	    );
	return val;
}
#endif

//Power Good status:
//0 \A8C Power Not Good
//1 \A8C Power Good
unsigned int bq2560x_get_pg_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val),
				     (unsigned char) (CON8_PG_STAT_MASK),
				     (unsigned char) (CON8_PG_STAT_SHIFT)
	    );
	return val;
}

//0 \A8C Not in thermal regulation
//1 \A8C in ther mAl regulation
unsigned int bq2560x_get_therm_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val),
				     (unsigned char) (CON8_THERM_STAT_MASK),
				     (unsigned char) (CON8_THERM_STAT_SHIFT)
	    );
	return val;
}

//0 \A8C Not in VSYSMin regulation (BAT > VSYSMin)
//1 \A8C in VSYSMin regulation (BAT < VSYSMin)
unsigned int bq2560x_get_vsys_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
				     (&val),
				     (unsigned char) (CON8_VSYS_STAT_MASK),
				     (unsigned char) (CON8_VSYS_STAT_SHIFT)
	    );
	return val;
}

/*CON9-----------------------------------------------------------*/
//get all default status
unsigned int bq2560x_get_default_state(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON9),
				     (&val), (unsigned char) (0xFF), (unsigned char) (0x0)
	    );
	return val;
}

//0 \A8C Normal, 1- Watchdog timer expiration
unsigned int bq2560x_get_wdt_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON9),
	                           (&val),
	                           (unsigned char)(CON9_WATCHDOG_FAULT_MASK),
	                           (unsigned char)(CON9_WATCHDOG_FAULT_SHIFT)
	                          );
	return val;
}

//0 \A8C Normal, 1 \A8C VBUS overloaded in OTG, or VBUS OVP, or battery is
//too low (any conditions that we cannot start boost function)
unsigned int bq2560x_get_boost_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON9),
	                           (&val),
	                           (unsigned char)(CON9_OTG_FAULT_MASK),
	                           (unsigned char)(CON9_OTG_FAULT_SHIFT)
	                          );
	return val;
}

//CHRG_FAULT
//00 \A8C Normal, 01 \A8C input fault (VAC OVP or VBAT < VBUS < 3.8 V),
//10 - shutdown, 11 \A8C Charge Safety Timer Expiration
unsigned int bq2560x_get_chrg_fault_state(void)
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON9),
	                           (&val),
	                           (unsigned char)(CON9_CHRG_FAULT_MASK),
	                           (unsigned char)(CON9_CHRG_FAULT_SHIFT)
	                          );
	return val;
}

//0 \A8C Normal, 1 \A8C BATOVP
unsigned int bq2560x_get_bat_state(void)
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON9),
	                           (&val),
	                           (unsigned char)(CON9_BAT_FAULT_MASK),
	                           (unsigned char)(CON9_BAT_FAULT_SHIFT)
	                          );
	return val;
}

//JEITA
//000 \A8C Normal, 010 \A8C Warm, 011 \A8C Cool, 101 \A8C Cold, 110 \A8C Hot (Buck mode)
//000 \A8C Normal, 101 \A8C Cold, 110 \A8C Hot (Boost mode)
unsigned int bq2560x_get_NTC_state(void)
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON9),
	                           (&val),
	                           (unsigned char)(CON9_NTC_FAULT_MASK),
	                           (unsigned char)(CON9_NTC_FAULT_SHIFT)
	                          );
	return val;
}
/*CON10----------------------------------------------------------*/
//0 \A8C Not VBUS attached,
//1 \A8C VBUS Attached
unsigned int bq2560x_get_VbusGood_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON10),
	                           (&val),
	                           (unsigned char)(CON10_VBUS_GD_MASK),
	                           (unsigned char)(CON10_VBUS_GD_SHIFT)
	                          );
	return val;
}

//0 \A8C Top off timer not counting.
//1 \A8C Top off timer counting
unsigned int bq2560x_get_TOPOFF_ACTIVE_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON10),
	                           (&val),
	                           (unsigned char)(CON10_TOPOFF_ACTIVE_MASK),
	                           (unsigned char)(CON10_TOPOFF_ACTIVE_SHIFT)
	                          );
	return val;
}

//0 \A8C Device is NOT in ACOV
//1 \A8C Device is in ACOV
unsigned int bq2560x_get_ACOV_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON10),
	                           (&val),
	                           (unsigned char)(CON10_ACOV_STAT_MASK),
	                           (unsigned char)(CON10_ACOV_STAT_SHIFT)
	                          );
	return val;
}

//0 \A8C Not in VINDPM, 1 \A8C in VINDPM
unsigned int bq2560x_get_VINDPM_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;

	/* Begin modified by bitao.xiong for task-8430235 on 2019-10-15 */
	if (HL7019_EXIST_FALG) {
		ret = bq2560x_read_interface((unsigned char) (bq2560x_CON8),
						(&val),
						(unsigned char) (CON8_CHRG_STAT_HL_MASK),
						(unsigned char) (CON8_CHRG_STAT_HL_SHIFT)
			);
	} else {
		ret=bq2560x_read_interface((unsigned char)(bq2560x_CON10),
								(&val),
								(unsigned char)(CON10_VINDPM_STAT_MASK),
								(unsigned char)(CON10_VINDPM_STAT_SHIFT)
								);
	}
	/* End modified by bitao.xiong for task-8430235 on 2019-10-15 */
	return val;
}

//0 \A8C Not in IINDPM, 1 \A8C in IINDPM
unsigned int bq2560x_get_IINDPM_state(void )
{
	unsigned int ret=0;
	unsigned char val=0;
	ret=bq2560x_read_interface((unsigned char)(bq2560x_CON10),
	                           (&val),
	                           (unsigned char)(CON10_IINDPM_STAT_MASK),
	                           (unsigned char)(CON10_IINDPM_STAT_SHIFT)
	                          );
	return val;
}

//bit1 0 - Allow VINDPM INT pulse 1 - Mask VINDPM INT pulse
//bit0 0 - Allow IINDPM INT pulse 1 - Mask IINDPM INT pulse
void bq2560x_set_int_mask(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON10),
				       (unsigned char) (val),
				       (unsigned char) (CON10_INT_MASK_MASK),
				       (unsigned char) (CON10_INT_MASK_SHIFT)
	    );
}

/*CON11----------------------------------------------------------*/
//Register reset
//0 \A8C Keep current register setting
//1 \A8C Reset to default register value and reset safety timer
//Note: Bit resets to 0 after register reset is completed
void bq2560x_set_reg_rst(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq2560x_config_interface((unsigned char) (bq2560x_CON11),
				       (unsigned char) (val),
				       (unsigned char) (CON11_REG_RST_MASK),
				       (unsigned char) (CON11_REG_RST_SHIFT)
	    );
}

//get Register reset
unsigned int bq2560x_get_reg_rst(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface((unsigned char) (bq2560x_CON11),
			     (&val),
			     (unsigned char) (CON8_VSYS_STAT_MASK),
			     (unsigned char) (CON8_VSYS_STAT_SHIFT)
   	 );
	if(ret)
		return true;
	else
		return false;
}
#ifdef COMPATIBLE_WITH_HL7019
//hl7019 : 001
unsigned char hl7019_get_PN(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (HL7019_CON_PN),
		(&val),
		(unsigned char) (HL7019_PN_MASK),
		(unsigned char) (HL7019_PN_SHIFT)
	);
	/* Begin modified by bitao.xiong for task-8430235 on 2019-12-02 */
	if(0x20 == val){
		return 1;
	}
	/* End modified by bitao.xiong for task-8430235 on 2019-12-02 */
	return 0;
}
#endif

//bq25601 : 0010
unsigned char bq2560x_get_PN(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON11),
		(&val),
		(unsigned char) (CON11_PN_MASK),
		(unsigned char) (CON11_PN_SHIFT)
	);

		return val;
}

/* Begin added by bitao.xiong for defect-8423557 on 2019-10-16 */
static unsigned char bq2560x_is_sgm(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq2560x_read_interface(
		(unsigned char) (bq2560x_CON11),
		(&val),
		(unsigned char) (CON11_Reserved_MASK),
		(unsigned char) (CON11_Reserved_SHIFT)
	);

		return val;
}
/* End added by bitao.xiong for defect-8423557 on 2019-10-16 */

#ifdef COMPATIBLE_WITH_HL7019
unsigned int get_charge_ic_id(void){

	unsigned int ret;
	unsigned int status = STATUS_FAIL;
	
	HL7019_EXIST_FALG = false;
	BQ25601_EXIST_FALG = false;
	SGM41511_EXIST_FLAG = false;

	ret=hl7019_get_PN();		//first check hl7019 charge ic, unique ID code
	if(ret == 1){
		HL7019_EXIST_FALG = true;
		status = STATUS_OK;
	}else{
		ret=bq2560x_get_PN();
		if(ret == 2){
			/* Begin modified by bitao.xiong for  defect-8423557 on 2019-10-16 */
			if (bq2560x_is_sgm()) {
				SGM41511_EXIST_FLAG = true;
				g_sgm41511_hw_exist = 1;
			} else {
				BQ25601_EXIST_FALG = true;
				g_bq2560x_hw_exist = 1;
			}
			status = STATUS_OK;
			/* End modified by bitao.xiong for  defect-8423557 on 2019-10-16 */
		}
	}
	printk("%s, HL7019_EXIST_FALG=%d, BQ25601_EXIST_FALG=%d, SGM41511_EXIST_FLAG=%d\n",
	__func__,HL7019_EXIST_FALG,BQ25601_EXIST_FALG, SGM41511_EXIST_FLAG);
	return status;
}
#endif

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
void charge_ic_hw_component_detect(void)
{
	unsigned char val = 0;

	val = get_charge_ic_id();

	if(val == STATUS_OK)
		g_charge_ic_hw_exist = 1;
	else
		g_charge_ic_hw_exist = 0;

	pr_info("[charge_ic_hw_component_detect] exist=%d, Reg[03]=0x%x\n", g_charge_ic_hw_exist, val);
}

int is_bq2560x_exist(void)
{
	pr_info("[is_bq2560x_exist] g_charge_ic_hw_exist=%d\n", g_charge_ic_hw_exist);

	return g_charge_ic_hw_exist;
}


void bq2560x_dump_register(void)
{
	unsigned char i = 0;
	unsigned char num = 0;

	if(HL7019_EXIST_FALG)
		num = hl7019_REG_NUM;
	else
		num = bq2560x_REG_NUM;

	for (i = 0; i < num; i++) {
		bq2560x_read_byte(i, &bq2560x_reg[i]);
		printk("bq2560x_dump_register[0x%x]=0x%x ", i, bq2560x_reg[i]);
	}
	pr_info("\n");
}


#define USE_POWER_PATH_WHEN_CHARGE_FULL
static int bq2560x_charger_enable(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

#ifdef CHARGE_LOG_PRINT
	pr_info("[%s][%d] qqq, dump function\n",__func__,en);
#endif

	if (true == enable) {
	#if 1//ndef USE_POWER_PATH_WHEN_CHARGE_FULL // change by TCT-cuiping.shi
		bq2560x_set_en_hiz(0x0);
	#endif
		bq2560x_set_chg_config(0x1);		/* charger enable */
	} else {
		if(HL7019_EXIST_FALG)
			hl7019_set_ichg(0);				/* ICHG = 0 mA disables charge.*/
		else
			bq2560x_set_ichg(0);				/* ICHG = 0 mA disables charge.*/
		bq2560x_set_chg_config(0x0);
//		if (charging_get_error_state())
//			chr_err("[charging_enable] under test mode: disable charging\n");
	#ifndef USE_POWER_PATH_WHEN_CHARGE_FULL
		bq2560x_set_en_hiz(0x1);	/* disable power path */
	#endif
	}

	return status;
}

/* ============================================================ // */
unsigned int charging_value_to_parameter(const unsigned int *parameter, const unsigned int array_size,
				       const unsigned int val)
{
	if (val < array_size) {
		return parameter[val];
	} else {
		pr_info("Can't find the parameter \r\n");
		return parameter[0];
	}
}

unsigned int charging_parameter_to_value(const unsigned int *parameter, const unsigned int array_size,
				       const unsigned int val)
{
	unsigned int i;

	for (i = 0; i < array_size; i++) {
		if (val == *(parameter + i))
			return i;
	}

	pr_info("NO register value match \r\n");
	/* TODO: ASSERT(0);    // not find the value */
	return 0;
}



static int bq2560x_charger_plug_in(struct charger_device *chg_dev)
{

	int ret;

	ret = bq2560x_charger_enable(chg_dev, true);
	return ret;
}


static int bq2560x_charger_plug_out(struct charger_device *chg_dev)
{
	int ret;

	ret = bq2560x_charger_enable(chg_dev, false);
	return ret;
}

static int bq2560x_charger_get_ichg(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;

	/* Get current level */
	
	if(HL7019_EXIST_FALG){
		reg_value = hl7019_get_ichg();
		array_size = GETARRAYNUM(CS_VTH_HL);
		*(u32 *) uA = charging_value_to_parameter(CS_VTH_HL, array_size, reg_value);
	}else{
		reg_value = bq2560x_get_ichg();
		array_size = GETARRAYNUM(CS_VTH);
		*(u32 *) uA = charging_value_to_parameter(CS_VTH, array_size, reg_value);
	}
	pr_info("bq2560x_charger_get_ichg %dmA\r\n", *uA/1000);

	return status;
}

static int bq2560x_charger_set_ichg(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u32 set_chr_current = 0;
	u32 array_size;
	u32 register_value;

#ifdef CHARGE_LOG_PRINT
	pr_info("bq2560x_charger_set_ichg uA = %d\r\n", uA);
#endif
	if(HL7019_EXIST_FALG){
		array_size = GETARRAYNUM(CS_VTH_HL);
		set_chr_current = bmt_find_closest_level(CS_VTH_HL, array_size, uA);
		register_value = charging_parameter_to_value(CS_VTH_HL, array_size, set_chr_current);
		hl7019_set_ichg(register_value);
	}else{
		array_size = GETARRAYNUM(CS_VTH);
		set_chr_current = bmt_find_closest_level(CS_VTH, array_size, uA);
		register_value = charging_parameter_to_value(CS_VTH, array_size, set_chr_current);
		bq2560x_set_ichg(register_value);
	}
	pr_info("bq2560x_charger_set_ichg %dmA\r\n", set_chr_current/1000);

	return status;
}

static int bq2560x_charger_get_min_ichg(struct charger_device *chg_dev, u32 *uA)
{
	*uA = CS_VTH[8];
	pr_info("bq2560x_charger_get_min_ichg %dmA\r\n", *uA/1000);
	return 0;
}

static int bq2560x_charger_set_cv(struct charger_device *chg_dev, u32 uV)
{
	unsigned int status = STATUS_OK;
	unsigned int array_size;
	unsigned int set_cv_voltage;
	unsigned short register_value;
	static short pre_register_value = -1;


	pr_info("charging_set_cv_voltage set_cv_voltage=%d\n",uV);
	if(HL7019_EXIST_FALG){
		array_size = GETARRAYNUM(VBAT_CV_VTH_HL);
		set_cv_voltage = bmt_find_closest_level(VBAT_CV_VTH_HL, array_size, uV);
		register_value = charging_parameter_to_value(VBAT_CV_VTH_HL, array_size, set_cv_voltage);
		hl7019_set_vreg(register_value);
	}else{
		array_size = GETARRAYNUM(VBAT_CV_VTH);
		set_cv_voltage = bmt_find_closest_level(VBAT_CV_VTH, array_size, uV);
		register_value = charging_parameter_to_value(VBAT_CV_VTH, array_size, set_cv_voltage);
		bq2560x_set_vreg(register_value);
	}
	pr_info("charging_set_cv_voltage register_value=0x%x %d %d\n", register_value, uV, set_cv_voltage);
	
	if (pre_register_value != register_value){
		bq2560x_set_chg_config(1);
		pr_info("pre_register_value=0x%x,register_value=0x%x\n",pre_register_value, register_value);
		pre_register_value = register_value;
	}

	return status;
}

static int bq2560x_charger_get_cv(struct charger_device *chg_dev, u32 *uV)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;

	/* Get current level */
	reg_value = bq2560x_get_vreg();

	if(HL7019_EXIST_FALG){
		reg_value = hl7019_get_vreg();
		array_size = GETARRAYNUM(VBAT_CV_VTH_HL);
		*(u32 *) uV = charging_value_to_parameter(VBAT_CV_VTH_HL, array_size, reg_value);
	}else{
		reg_value = bq2560x_get_vreg();
		array_size = GETARRAYNUM(VBAT_CV_VTH);
		*(u32 *) uV = charging_value_to_parameter(VBAT_CV_VTH, array_size, reg_value);
	}

	pr_info("bq2560x_charger_get_cv %dmV\r\n", *uV/1000);

	return status;
}


#ifdef COMPATIBLE_WITH_HL7019
static int bq2560x_set_mivr(struct charger_device *chg_dev, u32 uV)
{
	u32 status = STATUS_OK;
	if(HL7019_EXIST_FALG)
		hl7019_set_vindpm(uV);
	else
		bq2560x_set_vindpm(uV);
	return status;
}

static int bq2560x_get_mivr(struct charger_device *chg_dev, bool *in_loop)
{
	u32 status = STATUS_OK;
	
	(*in_loop) = bq2560x_get_VINDPM_state();
	return status;
}
#endif


static int bq2560x_charger_get_aicr(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;

	/* Get current level */
	
	if(HL7019_EXIST_FALG){
		reg_value = hl7019_get_iinlim();
		array_size = GETARRAYNUM(INPUT_CS_VTH_HL);
		*(u32 *) uA = charging_value_to_parameter(INPUT_CS_VTH_HL, array_size, reg_value);
	}else{
		reg_value = bq2560x_get_iinlim();
		array_size = GETARRAYNUM(INPUT_CS_VTH);
		/* Begin modified by bitao.xiong for task-8219984 on 2019-08-27 */
		*(u32 *) uA = charging_value_to_parameter(INPUT_CS_VTH, array_size, reg_value) * 10;
		/* End modified by bitao.xiong for task-8219984 on 2019-08-27 */
	}
	pr_info("bq2560x_charger_get_aicr %dmA\r\n", *uA/1000);

	return status;
}

static int bq2560x_charger_set_aicr(struct charger_device *chg_dev, u32 uA)
{
	unsigned int status = STATUS_OK;
	unsigned int current_value = uA;
	unsigned int set_chr_current;
	unsigned int array_size;
	unsigned int register_value;

#ifdef CHARGE_LOG_PRINT
	pr_info("[%s][%d] qqq, dump function\n",__func__,__LINE__);
#endif
	if(HL7019_EXIST_FALG){
		array_size = GETARRAYNUM(INPUT_CS_VTH_HL);
		set_chr_current = bmt_find_closest_level(INPUT_CS_VTH_HL, array_size, current_value);
		register_value = charging_parameter_to_value(INPUT_CS_VTH_HL, array_size, set_chr_current);
		hl7019_set_iinlim(register_value);
	}else{
		array_size = GETARRAYNUM(INPUT_CS_VTH);
		/* Begin modified by bitao.xiong for task-8219984 on 2019-08-27 */
		current_value /= 10;
		/* End modified by bitao.xiong for task-8219984 on 2019-08-27 */
		set_chr_current = bmt_find_closest_level(INPUT_CS_VTH, array_size, current_value);
		register_value = charging_parameter_to_value(INPUT_CS_VTH, array_size, set_chr_current);
		bq2560x_set_iinlim(register_value);
	}
	pr_info("charging_set_cc_input_current set_cc_current=%d,set_chr_current=%d,register_value=%d\n",
		uA,set_chr_current,register_value);
	pr_info("bq2560x_charger_set_aicr %dmA\r\n", uA/1000);

	return status;
}

static int bq2560x_charger_get_min_aicr(struct charger_device *chg_dev, u32 *uA)
{
	/* Begin modified by bitao.xiong for task-8219984 on 2019-08-27 */
	*uA = CHARGE_CURRENT_100_00_MA * 10;//CHARGE_CURRENT_1000_00_MA;//INPUT_CS_VTH[0];
	/* End modified by bitao.xiong for task-8219984 on 2019-08-27 */
	pr_info("bq2560x_charger_get_min_aicr %dmA\r\n", *uA/1000);
	return 0;
}

//set ITERM
static int bq2560x_charger_set_eoc_current(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;
	/* Begin modified by bitao.xiong for defect-8423557 on 2019-11-27 */
	u32 iterm_step_value = 60000;
	if (HL7019_EXIST_FALG) {
		iterm_step_value = 128000;
	}

	if(uA >= 480000) {
		uA = 480000;
	}

	if (uA <= iterm_step_value) {
		uA = iterm_step_value;
	}

	reg_value = uA/iterm_step_value - 1;

	bq2560x_set_iterm(reg_value);

	pr_info("bq2560x_charger_set_eoc_current %dmA\r\n", (reg_value+1)*iterm_step_value/1000);
	/* End modified by bitao.xiong for defect-8423557 on 2019-11-27 */
	return status;
}

static int bq2560x_charger_get_eoc_current(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;
	/* Begin modified by bitao.xiong for defect-8423557 on 2019-11-27 */
	u32 iterm_step_value = 60000;

	if (HL7019_EXIST_FALG) {
		iterm_step_value = 128000;
	}

	reg_value = bq2560x_get_iterm();
	*uA = (reg_value+1) * iterm_step_value;
	/* End modified by bitao.xiong for defect-8423557 on 2019-11-27 */
	return status;
}


static int bq2560x_charger_enable_te(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	pr_info("bq2560x_charger_enable_te %d\r\n", en);

	if (true == enable) {
		bq2560x_set_en_term(1);
	} else {
		bq2560x_set_en_term(0);
	}

	return status;
}

/* Begin added by bitao.xiong for defect-8180576 on 2019-08-03 */
static int bq2560x_enable_power_path(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;
	if (true == enable) {
		bq2560x_set_en_hiz(0x0);	/* enable power path */
	} else {
		bq2560x_set_en_hiz(0x1);	/* disable power path */
	}
	return status;
}

static int bq2560x_is_power_path_enable(struct charger_device *chg_dev, bool *en)
{
	int status = STATUS_OK;

	if (bq2560x_get_en_hiz_stat()) {
		*en = false;
	} else {
		*en = true;
	}

	return status;
}
/* End added by bitao.xiong for defect-8180576 on 2019-08-03 */

/* Begin added by bitao.xiong for task-8219984 on 2019-08-06 */
static int bq2560x_set_vindpm_voltage(struct charger_device *chg_dev,
				      u32 vindpm)
{
	int status = 0;
	unsigned int array_size;

	vindpm /= 1000;
	/* Begin modified by bitao.xiong for task-8430235 on 2019-10-15 */
	if (HL7019_EXIST_FALG) {
		array_size = ARRAY_SIZE(VINDPM_REG_HL);
		vindpm = bmt_find_closest_level(VINDPM_REG_HL, array_size, vindpm);
		vindpm = charging_parameter_to_value(VINDPM_REG_HL, array_size, vindpm);

		pr_info("%s vindpm =%d\n", __func__, vindpm);
		bq2560x_set_mivr(chg_dev, vindpm);
	} else {
		array_size = ARRAY_SIZE(VINDPM_REG);
		vindpm = bmt_find_closest_level(VINDPM_REG, array_size, vindpm);
		vindpm = charging_parameter_to_value(VINDPM_REG, array_size, vindpm);

		pr_info("%s vindpm =%d\r\n", __func__, vindpm);
		bq2560x_set_mivr(chg_dev, vindpm);
	}
	/* End modified by bitao.xiong for task-8430235 on 2019-10-15 */

	return status;
}
/* End added by bitao.xiong for task-8219984 on 2019-08-06 */

static int bq2560x_charger_enable_otg(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	pr_info("bq2560x_charger_enable_otg %d\r\n", en);

	if (true == enable) {
//Added-start by baiwei.peng, fix mini sw otg fail for defect-9793476, 2020/08/19
#ifdef TARGET_BUILD_MMITEST
		bq2560x_enable_power_path(chg_dev, true);
#endif
//Added-end by baiwei.peng, fix mini sw otg fail for defect-9793476, 2020/08/19
		bq2560x_set_otg_config(1);							/*  1,OTG enable; default; 0,OTG disabl */
#ifdef COMPATIBLE_WITH_HL7019
	if(HL7019_EXIST_FALG) {
		hl7019_set_boostv(0x7);				/* Set boostv 4.998v */
		hl7019_set_boost_lim(0x1);  	/* 1.2A on Vbus*/
	} else {
		bq2560x_set_boostv(0x2);				/* Set boostv 5.15v */
		bq2560x_set_boost_lim(0x1);  /* 1.2A on Vbus*/
	}
#endif
	} else {
		bq2560x_set_otg_config(0);							/*  1,OTG enable; default; 0,OTG disabl */
//Added-start by baiwei.peng, fix mini sw otg fail for defect-9793476, 2020/08/19
#ifdef TARGET_BUILD_MMITEST
                bq2560x_enable_power_path(chg_dev, false);
#endif
//Added-end by baiwei.peng, fix mini sw otg fail for defect-9793476, 2020/08/19
	}

	return status;
}

static int bq2560x_charger_is_charging_done(struct charger_device *chg_dev, bool *done)
{
	unsigned int status = STATUS_OK;
	unsigned int reg_value;

	if(HL7019_EXIST_FALG)
		reg_value = hl7019_get_chrg_state();
	else
		reg_value = bq2560x_get_chrg_state();

	if (reg_value == 0x3)	/* check if chrg done */
		* done  = true;
	else
		* done  = false;
	pr_info("bq2560x_charger_is_charging_done %d\r\n", *done);

	return status;
}

static int bq2560x_charger_dump_registers(struct charger_device *chg_dev)
{
	u32 status = STATUS_OK;

	pr_info("charging_dump_register\r\n");
	bq2560x_dump_register();

	return status;
}

static int bq2560x_do_event(struct charger_device *chg_dev, u32 event, u32 args)
{
	if (chg_dev == NULL)
		return -EINVAL;

	pr_info("%s: event = %d\n", __func__, event);

	switch (event) {
	/*case EVENT_EOC:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_EOC);
		break;*/
	/*case EVENT_RECHARGE:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_RECHG);
		break;*/
	default:
		break;
	}
	return 0;
}


static unsigned int bmt_find_closest_level(const unsigned int *pList, unsigned int number,
					 unsigned int level)
{
	unsigned int i;
	unsigned int max_value_in_last_element;

	if (pList[0] < pList[1])
		max_value_in_last_element = true;
	else
		max_value_in_last_element = false;

	if (max_value_in_last_element == true) {
		for (i = (number - 1); i != 0; i--) {	/* max value in the last element */
			if (pList[i] <= level) {
				return pList[i];
			}
		}

		pr_info("Can't find closest level \r\n");
		return pList[0];
		/* return CHARGE_CURRENT_0_00_MA; */
	} else {
		for (i = 0; i < number; i++) {	/* max value in the first element */
			if (pList[i] <= level)
				return pList[i];
		}

		pr_info("Can't find closest level \r\n");
		return pList[number - 1];
		/* return CHARGE_CURRENT_0_00_MA; */
	}
}

//static unsigned int charging_get_error_state(void)
//{
//	return charging_error;
//}

//static unsigned int charging_set_error_state(void)
//{
//	unsigned int status = STATUS_OK;
//
//	charging_error = true;
//
//	return status;
//}
//
//static unsigned int charging_reset_error_state(void)
//{
//	unsigned int status = STATUS_OK;
//
//	charging_error = false;
//
//	return status;
//}
/*
static unsigned int charging_set_chrind_ck_pdn(void)
{
	unsigned int status = STATUS_OK;
	unsigned int pwr_dn;

	pwr_dn = true;

	#if defined(CONFIG_MTK_PMIC_CHIP_MT6353)
		pmic_set_register_value(PMIC_CLK_DRV_CHRIND_CK_PDN, pwr_dn);
	#else
		pmic_set_register_value(PMIC_RG_DRV_CHRIND_CK_PDN, pwr_dn);
	#endif

	return status;
}

static unsigned int charging_reset_chrind_ck_pdn(void)
{
	unsigned int status = STATUS_OK;
	unsigned int pwr_dn;

	pwr_dn = false;

	#if defined(CONFIG_MTK_PMIC_CHIP_MT6353)
	pmic_set_register_value(PMIC_CLK_DRV_CHRIND_CK_PDN, pwr_dn);
      #else
	pmic_set_register_value(PMIC_RG_DRV_CHRIND_CK_PDN, pwr_dn);
      #endif

	return status;
}
*/
static unsigned int bq2560x_charging_hw_init(void)
{
	unsigned int status = STATUS_OK;

#ifdef CHARGE_LOG_PRINT
	pr_info("qqq,[%s],[%s],[%d] charge ic init function\n",__FILE__,__func__,__LINE__);
#endif
//	bq2560x_set_chg_config(0x1);		/* charger enable */
//	udelay(10);
//	bq2560x_set_chg_config(0);		/* charger disable,in order to avoid power_on_reset start charging fail */
//	udelay(100);
//	bq2560x_set_chg_config(0x1);		/* charger enable */
//	chr_err("[%s][%d] qqq, bq2560x soft charge reset\n",__func__,__LINE__);


	if (HL7019_EXIST_FALG)
	{
		/* Begin added by bitao.xiong for task-8430235 on 2019-12-24 */
		hl7019_set_reg_rst(0x1);
		mdelay(2);
		/* End added by bitao.xiong for task-8430235 on 2019-12-24 */
		hl7019_set_reg_rst(0x0);
		bq2560x_set_wdt_rst(0x0);				/* Kick watchdog */
		bq2560x_set_sys_min(0x5);				/* Minimum system voltage 3.5V */
		bq2560x_set_otg_config(0x0);				/* Disable OTG boost */
		hl7019_set_chg_timer(0x02);				/* Default:12 hours (10),set charge time 12h*/
		hl7019_set_boostv(0x7);					/* 0111 output voltage of boost mode = 4.998V */
		hl7019_set_bhot(0x0);						/* 00-> 55 degree 36% */
		hl7019_set_treg(0x3);						/* 11 thermal regulation threshold 120 degree*/
		/* Begin modified by bitao.xiong for defect-8423557 on 2019-11-27 */
		bq2560x_set_iterm(0x0);					/* Termination current 128mA */
		/* End modified by bitao.xiong for defect-8423557 on 2019-11-27 */
		hl7019_set_bcold(0x0);					/* Set boost mode temperature monitor threshold */
		hl7019_set_force_20pct(0x0);				/* 20% ichg */
	//#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
		hl7019_set_vreg(0x38);					/* VREG 4.4V 111000 */
	//#else
	//	hl7019_set_vreg(0x28);						/* VREG 4.208V 101100 */
	//#endif
		/* Begin added by bitao.xiong for task-8430235 on 2019-12-02 */
		hl7019_set_ir_compensation(0x2, 0x2);   /*IR Compensation, R=20mΩ, V=32mV*/
		/* End added by bitao.xiong for task-8430235 on 2019-12-02 */
		hl7019_set_batlowv(0x1);				/* BATLOWV 3.0V */
		bq2560x_set_vrechg(0x0);				/* Recharge threshold,Default: 100mV(0),VRECHG 0.1V (4.4-4.3V) */
		bq2560x_set_iprechg(0x3);			/* Precharge current 540mA */
		/* Begin modified by hailong.chen for task 9525580 on 2020-06-19 */
		hl7019_set_ichg(0x0);				/* ICHG default 2048mA,011000 set value in pturn_on_charging() */
		hl7019_set_iinlim(0x02);				/* Iinlim 2000,default =2000mA,110, set value in pturn_on_charging() */
		/* End modified by hailong.chen for task 9525580 on 2020-06-19 */
		bq2560x_set_en_term(0x1);			/* disable termination */
		hl7019_set_stat_ctrl(0x0);				/* Disable STAT pin function */
		bq2560x_set_watchdog(0x0);			/* WDT 40s */ //disable WDT by jingjing.jiang.hz
		bq2560x_set_en_timer(0x1);			/* Enable charge timer */
		hl7019_set_int_mask(0x0);				/* Disable fault interrupt */
		hl7019_set_vindpm(0x6);				/* VIN DPM check 4.35V */
	//	bq2560x_set_ts_en(0);					/* Thermal regulation : TBD*/
		bq2560x_set_en_hiz(0x0);
	}else{
		bq2560x_set_reg_rst(0x0);
		bq2560x_set_wdt_rst(0x0);			/* Kick watchdog *///jingjing.jiang.hz
		bq2560x_set_sys_min(0x5);			/* Minimum system voltage 3.5V */
		bq2560x_set_otg_config(0x0);			/* Disable OTG boost */
		bq2560x_set_chg_timer(0x01);			/* Default: 10 hours (1),set charge time 10h*/
		bq2560x_set_ovp(0x1);				 /* VAC OVP threshold:11---14 V (12-V input) */
		/* Begin modified by bitao.xiong for defect-8423557 on 2019-11-27 */
        bq2560x_set_iterm(0x2);             /* Termination current 180mA */
		/* End modified by bitao.xiong for defect-8423557 on 2019-11-27 */

	//#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
		bq2560x_set_vreg(0x11);				/* VREG 4.4V */
	//#else
	//	bq2560x_set_vreg(0x0B);				/* VREG 4.208V */
	//#endif

		bq2560x_set_batlowv(0x1);				/* precharge2cc voltage, BATLOWV 3.0V */
		bq2560x_set_vrechg(0x0);				/* Recharge threshold,Default: 100mV(0),VRECHG 0.1V (4.4-4.3V) */
		bq2560x_set_iprechg(0x8);				/* Precharge current 540mA */
		/* Begin modified by hailong.chen for task 9525580 on 2020-06-19 */
		bq2560x_set_ichg(0x9);					/* ICHG default 2048mA, set value in pturn_on_charging() */
		bq2560x_set_iinlim(0x4);				/* Iinlim 2000,default = 2400mA,10111, set value in pturn_on_charging() */
		/* End modified by hailong.chen for task 9525580 on 2020-06-19 */
		bq2560x_set_en_term(0x1);				/* disable termination */
		bq2560x_set_stat_ctrl(0x3);			/* Disable STAT pin function */
		bq2560x_set_watchdog(0x0);			/* WDT 40s */ //disable WDT by jingjing.jiang.hz
		bq2560x_set_en_timer(0x0);			/* 0--Disable charge timer */
		bq2560x_set_int_mask(0x0);			/* Disable fault interrupt */
		bq2560x_set_vindpm(0x7);				/* VIN DPM check 4.6V */
	//	bq2560x_set_ts_en(0);					/* Thermal regulation : TBD*/
		bq2560x_set_en_hiz(0x0);
	}
	return status;
}

//static unsigned int charging_set_pwrstat_led_enable(void)
//{
//	unsigned int status = STATUS_OK;
//
//	pmic_set_register_value(PMIC_CLK_DRV_32K_CK_PDN, 0x0);	/* Disable all power down */
//	// pchr_led
//	pmic_set_register_value(PMIC_CLK_DRV_CHRIND_CK_PDN, 0x0);	/* Disable CHRIND power down */
//	pmic_set_register_value(PMIC_CHRIND_MODE, 0x2); 			/* register mode */
//	pmic_set_register_value(PMIC_CHRIND_EN_SEL, 1); 			/* 0: Auto, 1: SW */
//	pmic_set_register_value(PMIC_CHRIND_EN, 0);
//	return status;
//}
//
//static unsigned int charging_set_pwrstat_led_disable(void)
//{
//	unsigned int status = STATUS_OK;
//
//	pmic_set_register_value(PMIC_CLK_DRV_32K_CK_PDN, 0x0);	/* Disable all power down */
//	// pchr_led
//	pmic_set_register_value(PMIC_CLK_DRV_CHRIND_CK_PDN, 0x0);	/* Disable CHRIND power down */
//	pmic_set_register_value(PMIC_CHRIND_MODE, 0x2); 			/* register mode */
//	pmic_set_register_value(PMIC_CHRIND_EN_SEL, 1); 			/* 0: Auto, 1: SW */
//	pmic_set_register_value(PMIC_CHRIND_EN, 1);
//	return status;
//}
#if 0
/* Begin added by bitao.xiong for task-8219984 on 2019-08-15 */
static int bq2560x_set_ship_mode(struct charger_device *chg_dev, bool en)
{
	unsigned int status = STATUS_OK;

	if (en)
		bq2560x_set_batfet_disable(1);

	return status;
}
/* End added by bitao.xiong for task-8219984 on 2019-08-15 */
#endif
static const struct charger_ops bq2560x_chg_ops = {
	.plug_in = bq2560x_charger_plug_in,
	.plug_out = bq2560x_charger_plug_out,
	/* enable */
	.enable = bq2560x_charger_enable,
//	.disable = bq2560x_charger_enable,
	/* charging current */
	.get_charging_current = bq2560x_charger_get_ichg,
	.set_charging_current = bq2560x_charger_set_ichg,
	.get_min_charging_current = bq2560x_charger_get_min_ichg,
	/* charging voltage */
	.set_constant_voltage = bq2560x_charger_set_cv,
	.get_constant_voltage = bq2560x_charger_get_cv,
	/* charging input current */
	.get_input_current = bq2560x_charger_get_aicr,
	.set_input_current = bq2560x_charger_set_aicr,
	.get_min_input_current = bq2560x_charger_get_min_aicr,
	/* set termination current */
	.get_eoc_current = bq2560x_charger_get_eoc_current,
	.set_eoc_current = bq2560x_charger_set_eoc_current,
	/* charing termination */
	.enable_termination = bq2560x_charger_enable_te,
	/* Begin added by bitao.xiong for defect-8180576 on 2019-08-03 */
	/* Power path */
	.enable_powerpath = bq2560x_enable_power_path,
	.is_powerpath_enabled = bq2560x_is_power_path_enable,
	/* End added by bitao.xiong for defect-8180576 on 2019-08-03 */
	/* Begin added by bitao.xiong for task-8219984 on 2019-08-15 */
	/* set ship mode*/
	#if 0
	.set_ship_mode = bq2560x_set_ship_mode,
	#endif
	/* End added by bitao.xiong for task-8219984 on 2019-08-15 */
	/* OTG */
	.enable_otg = bq2560x_charger_enable_otg,
	/* misc */
	.is_charging_done = bq2560x_charger_is_charging_done,
	.dump_registers = bq2560x_charger_dump_registers,
	/* Begin modified by bitao.xiong for task-8219984 on 2019-08-06 */
	.set_mivr = bq2560x_set_vindpm_voltage,
	/* End modified by bitao.xiong for task-8219984 on 2019-08-06 */
	.get_mivr_state = bq2560x_get_mivr,
	/* Event */
	.event = bq2560x_do_event,
};

static const struct charger_properties bq2560x_chg_props = {
	.alias_name = "bq2560x",
};

//extern char charger_module_name[256];
static char charger_module_name[256];

static int bq2560x_driver_probe(struct i2c_client *i2c, const struct i2c_device_id *id)
{
	pr_info("[bq2560x_driver_probe]\n");

	bq2560x_info = devm_kzalloc(&i2c->dev, sizeof(*bq2560x_info), GFP_KERNEL);
	if (!bq2560x_info)
		return -ENOMEM;

	bq2560x_info->dev = &i2c->dev;
	bq2560x_info->i2c = i2c;
	i2c_set_clientdata(i2c, bq2560x_info);

	mutex_init(&bq2560x_i2c_access);

	bq2560x_reg_config_interface(0x06, 0x7A);
	charge_ic_hw_component_detect();

	if(g_charge_ic_hw_exist == 0)
	    return -ENODEV;

	bq2560x_charging_hw_init();

#if 1//defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
	/* charger class register */
	bq2560x_info->chg_dev = charger_device_register("primary_chg",
                                              bq2560x_info->dev,
                                              bq2560x_info,
					      &bq2560x_chg_ops,
					      &bq2560x_chg_props);
	if (IS_ERR(bq2560x_info->chg_dev)) {
		dev_info(bq2560x_info->dev, "charger device register fail\n");
		return PTR_ERR(bq2560x_info->chg_dev);
	}
#endif /* #if (CONFIG_MTK_GAUGE_VERSION == 30) */


	bq2560x_dump_register();
	chargin_hw_init_done = true;
	/* Begin added by bitao.xiong for task-7884939 on 2019-07-04 */
	if (is_bq2560x_exist()) {
		if (HL7019_EXIST_FALG)
			strcpy(charger_module_name,"HL7019");
		else if (SGM41511_EXIST_FLAG)
			strcpy(charger_module_name,"SG:SGM41511");
		else
			strcpy(charger_module_name,"TI:BQ25601");
	}
	/* End added by bitao.xiong for task-7884939 on 2019-07-04 */

	return 0;
}

/**********************************************************
  *
  *   [platform_driver API]
  *
  *********************************************************/
unsigned char g_reg_value_bq2560x = 0;
static ssize_t show_bq2560x_access(struct device *dev, struct device_attribute *attr, char *buf)
{
	pr_info("[show_bq2560x_access] 0x%x\n", g_reg_value_bq2560x);
	return sprintf(buf, "%u\n", g_reg_value_bq2560x);
}

static ssize_t store_bq2560x_access(struct device *dev, struct device_attribute *attr,
				    const char *buf, size_t size)
{
	int ret = 0;
	char *pvalue = NULL, *addr, *val;
	unsigned int reg_value = 0;
	unsigned int reg_address = 0;

	pr_info("[store_bq2560x_access]\n");

	if (buf != NULL && size != 0) {
		pr_info("[store_bq2560x_access] buf is %s and size is %zu\n", buf, size);
		pvalue = (char *)buf;
		if (size > 3) {
			addr = strsep(&pvalue, " ");
			ret = kstrtou32(addr, 16, (unsigned int *)&reg_address);
		} else
			ret = kstrtou32(pvalue, 16, (unsigned int *)&reg_address);

		if (size > 3) {
			val = strsep(&pvalue, " ");
			ret = kstrtou32(val, 16, (unsigned int *)&reg_value);

			pr_info("[store_bq2560x_access] write bq2560x reg 0x%x with value 0x%x !\n",
			     reg_address, reg_value);
			ret = bq2560x_config_interface(reg_address, reg_value, 0xFF, 0x0);
		} else {
			ret = bq2560x_read_interface(reg_address, &g_reg_value_bq2560x, 0xFF, 0x0);
			pr_info("[store_bq2560x_access] read bq2560x reg 0x%x with value 0x%x !\n",
				 reg_address, g_reg_value_bq2560x);
			pr_info("[store_bq2560x_access] Please use \"cat bq2560x_access\" to get value\r\n");
		}
	}
	return size;
}

static DEVICE_ATTR(bq2560x_access, 0664, show_bq2560x_access, store_bq2560x_access);	/* 664 */

static int bq2560x_user_space_probe(struct platform_device *dev)
{
	int ret_device_file = 0;

	pr_info("******** bq2560x_user_space_probe!! ********\n");

	ret_device_file = device_create_file(&(dev->dev), &dev_attr_bq2560x_access);

	return 0;
}

struct platform_device bq2560x_user_space_device = {
	.name = "bq2560x-user",
	.id = -1,
};

static struct platform_driver bq2560x_user_space_driver = {
	.probe = bq2560x_user_space_probe,
	.driver = {
		   .name = "bq2560x-user",
		   },
};


static int __init bq2560x_init(void)
{
	int ret = 0;
/*-Begin added by dapeng.qiao for  task 8997711 on 2020-03-16*/
	if (i2c_add_driver(&bq2560x_driver) != 0) {
		//g_bq2560x_hw_exist = 0;  // move to get_charge_ic_id()
		pr_info("[bq2560x_init] failed to register bq2560x i2c driver.\n");
	} else {
		//g_bq2560x_hw_exist = 1;  // move to get_charge_ic_id()
		pr_info("[bq2560x_init] Success to register bq2560x i2c driver.\n");
	}
/*-End added by dapeng.qiao for  task 8997711 on 2020-03-16*/

	/* bq2560x user space access interface */
	ret = platform_device_register(&bq2560x_user_space_device);
	if (ret) {
		pr_info("****[bq2560x_init] Unable to device register(%d)\n", ret);
		return ret;
	}
	ret = platform_driver_register(&bq2560x_user_space_driver);
	if (ret) {
		pr_info("****[bq2560x_init] Unable to register driver (%d)\n", ret);
		return ret;
	}

	return 0;
}

static void __exit bq2560x_exit(void)
{
	i2c_del_driver(&bq2560x_driver);
}

 module_init(bq2560x_init);
/* module_exit(bq2560x_exit); */

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C bq2560x Driver");
MODULE_AUTHOR("YT Lee<yt.lee@mediatek.com>");
