#include <linux/types.h>
#include <linux/init.h>		/* For init/exit macros */
#include <linux/module.h>	/* For MODULE_ marcros  */
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif
#include <linux/power_supply.h>
#include <linux/delay.h>
#include <mt-plat/mtk_boot.h>
#include <mt-plat/upmu_common.h>
#include "bq24158.h"
#include <mtk_charger.h>
#include <charger_class.h>

#define VBUS_THR 4000

enum bq24158_charging_status {
	BQ24158_CHG_STATUS_READY = 0,
	BQ24158_CHG_STATUS_PROGRESS,
	BQ24158_CHG_STATUS_DONE,
	BQ24158_CHG_STATUS_FAULT,
	BQ24158_CHG_STATUS_MAX,
};
//Begin modified by hailong.chen for task 6439451 on 2018/06/30
struct bq24158_INFO {
	struct device *dev;
	struct i2c_client *i2c;
	struct charger_device *chg_dev;
	struct charger_properties chg_props;
	struct notifier_block nb;

	struct power_supply *charger;
	struct power_supply_desc charger_desc;
	struct delayed_work work;
	struct semaphore suspend_lock;
#if 0
	struct gtimer otg_kthread_gtimer;
	struct workqueue_struct *otg_boost_workq;
	struct work_struct kick_work;
	unsigned int polling_interval;
	bool polling_enabled;
#endif
};
//static void enable_boost_polling(bool poll_en);
//End modified by hailong.chen for task 6439451 on 2018/06/30

struct bq24158_INFO *bq24158_info = NULL;

static const struct i2c_device_id bq24158_i2c_id[] = { {"bq24158", 0}, {} };

/*-Begin added by dapeng.qiao for XR-8325735 on 20190917*/	
static int g_bq24158_hw_exist;
int g_bq24158_hw_ver = 0;
/*-End added by dapeng.qiao for XR-8325735 on 20190917*/	

bool chargin_hw_init_done_bq24158 = false;
static int bq24158_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int bq24158_suspend(struct device *dev);
static int bq24158_resume(struct device *dev);
static SIMPLE_DEV_PM_OPS(bq24158_pm_ops, bq24158_suspend, bq24158_resume);

#ifdef CONFIG_OF
static const struct of_device_id bq24158_of_match[] = {
	{.compatible = "mediatek,bq24158"},
	{},
};

MODULE_DEVICE_TABLE(of, bq24158_of_match);
#endif

static struct i2c_driver bq24158_driver = {
	.driver = {
		   .name = "bq24158",
#ifdef CONFIG_OF
		   .of_match_table = bq24158_of_match,
#endif
		   .pm = &bq24158_pm_ops,
	},
	.probe = bq24158_driver_probe,
	.id_table = bq24158_i2c_id,
};

/**********************************************************
  *
  *   [Global Variable]
  *
  *********************************************************/
unsigned char bq24158_reg[bq24158_REG_NUM] = { 0 };

//static DEFINE_MUTEX(bq24158_i2c_access);

/**********************************************************
  *
  *   [I2C Function For Read/Write bq24158]
  *
  *********************************************************/
int bq24158_read_byte(unsigned char cmd, unsigned char *returnData)
{
	char cmd_buf[1] = { 0x00 };
	char readData = 0;
	int ret = 0;

	pm_stay_awake(bq24158_info->dev);
	//mutex_lock(&bq24158_i2c_access);
	down(&bq24158_info->suspend_lock);
	cmd_buf[0] = cmd;
	ret = i2c_master_send(bq24158_info->i2c, &cmd_buf[0], 1);
	if (ret < 0) {
		//mutex_unlock(&bq24158_i2c_access);
		up(&bq24158_info->suspend_lock);
		return 0;
	}

	ret = i2c_master_recv(bq24158_info->i2c, &cmd_buf[0], 1);
	if (ret < 0) {
		//mutex_unlock(&bq24158_i2c_access);
		up(&bq24158_info->suspend_lock);
		return 0;
	}

	readData = cmd_buf[0];
	*returnData = readData;
	up(&bq24158_info->suspend_lock);
	//mutex_unlock(&bq24158_i2c_access);
	pm_relax(bq24158_info->dev);
	return 1;
}

int bq24158_write_byte(unsigned char cmd, unsigned char writeData)
{
	char write_data[2] = { 0 };
	int ret = 0;

	pm_stay_awake(bq24158_info->dev);
	//mutex_lock(&bq24158_i2c_access);
	down(&bq24158_info->suspend_lock);
	write_data[0] = cmd;
	write_data[1] = writeData;

	ret = i2c_master_send(bq24158_info->i2c, write_data, 2);
	if (ret < 0) {
		//mutex_unlock(&bq24158_i2c_access);
		up(&bq24158_info->suspend_lock);
		return 0;
	}
	up(&bq24158_info->suspend_lock);
	//mutex_unlock(&bq24158_i2c_access);
	pm_relax(bq24158_info->dev);

	return 1;
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
unsigned int bq24158_read_interface(unsigned char RegNum, unsigned char *val, unsigned char MASK,
				  unsigned char SHIFT)
{
	unsigned char bq24158_reg = 0;
	int ret = 0;

	ret = bq24158_read_byte(RegNum, &bq24158_reg);

	bq24158_reg &= (MASK << SHIFT);
	*val = (bq24158_reg >> SHIFT);

	pr_info("[bq24158_read_interface] reg[%x] = 0x%x\n",RegNum, *val);

	return ret;
}

unsigned int bq24158_config_interface(unsigned char RegNum, unsigned char val, unsigned char MASK,
				  unsigned char SHIFT)
{
	unsigned char bq24158_reg = 0;
	int ret = 0;

	ret = bq24158_read_byte(RegNum, &bq24158_reg);

	bq24158_reg &= ~(MASK << SHIFT);
	bq24158_reg |= (val << SHIFT);

	if (RegNum == bq24158_CON4 && val == 1 && MASK == CON4_RESET_MASK
	    && SHIFT == CON4_RESET_SHIFT) {
		/* RESET bit */
	} else if (RegNum == bq24158_CON4) {
		bq24158_reg &= ~0x80;	/* RESET bit read returs 1, so clear it */
	}

	ret = bq24158_write_byte(RegNum, bq24158_reg);
	pr_info("[bq24158_config_interface] write Reg[%x]=0x%x\n", RegNum, bq24158_reg);

	return ret;
}

/* write one register directly */
unsigned int bq24158_reg_config_interface(unsigned char RegNum, unsigned char val)
{
	int ret = 0;

	ret = bq24158_write_byte(RegNum, val);

	return ret;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
/* CON0 */

void bq24158_set_tmr_rst(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_TMR_RST_MASK),
				       (unsigned char) (CON0_TMR_RST_SHIFT)
	    );
}

unsigned int bq24158_get_otg_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON0),
				     (&val), (unsigned char) (CON0_OTG_MASK),
				     (unsigned char) (CON0_OTG_SHIFT)
	    );
	return val;
}

void bq24158_set_en_stat(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_EN_STAT_MASK),
				       (unsigned char) (CON0_EN_STAT_SHIFT)
	    );
}

unsigned int bq24158_get_chip_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON0),
				     (&val), (unsigned char) (CON0_STAT_MASK),
				     (unsigned char) (CON0_STAT_SHIFT)
	    );
	return val;
}

static int bq24158_get_charge_status(enum bq24158_charging_status *chg_stat)
{
	int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON0),
				     (&val), (unsigned char) (CON0_STAT_MASK),
				     (unsigned char) (CON0_STAT_SHIFT)
	    );
	if (ret)
		*chg_stat = (enum bq24158_charging_status)val;
	return ret;
}

unsigned int bq24158_get_boost_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON0),
				     (&val), (unsigned char) (CON0_BOOST_MASK),
				     (unsigned char) (CON0_BOOST_SHIFT)
	    );
	return val;
}

unsigned int bq24158_get_fault_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON0),
				     (&val), (unsigned char) (CON0_FAULT_MASK),
				     (unsigned char) (CON0_FAULT_SHIFT)
	    );
	return val;
}

/* CON1 */

void bq24158_set_input_charging_current(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_LIN_LIMIT_MASK),
				       (unsigned char) (CON1_LIN_LIMIT_SHIFT)
	    );
}

unsigned int bq24158_get_input_charging_current(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON1),
				     (&val), (unsigned char) (CON1_LIN_LIMIT_MASK),
				     (unsigned char) (CON1_LIN_LIMIT_SHIFT)
	    );
	return val;
}

void bq24158_set_v_low(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_LOW_V_MASK),
				       (unsigned char) (CON1_LOW_V_SHIFT)
	    );
}

void bq24158_set_te(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_TE_MASK),
				       (unsigned char) (CON1_TE_SHIFT)
	    );
}

void bq24158_set_ce(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_CE_MASK),
				       (unsigned char) (CON1_CE_SHIFT)
	    );
}

unsigned int bq24158_get_ce(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON1),
				     (&val), (unsigned char) (CON1_CE_MASK),
				     (unsigned char) (CON1_CE_SHIFT)
	    );

	return val;
}

void bq24158_set_hz_mode(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_HZ_MODE_MASK),
				       (unsigned char) (CON1_HZ_MODE_SHIFT)
	    );
}

void bq24158_set_opa_mode(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_OPA_MODE_MASK),
				       (unsigned char) (CON1_OPA_MODE_SHIFT)
	    );
}

/* CON2 */

void bq24158_set_oreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_OREG_MASK),
				       (unsigned char) (CON2_OREG_SHIFT)
	    );
}

unsigned int bq24158_get_oreg(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON2),
				     (&val), (unsigned char) (CON2_OREG_MASK),
				     (unsigned char) (CON2_OREG_SHIFT)
	    );

	return val;
}

void bq24158_set_otg_pl(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_OTG_PL_MASK),
				       (unsigned char) (CON2_OTG_PL_SHIFT)
	    );
}

void bq24158_set_otg_en(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_OTG_EN_MASK),
				       (unsigned char) (CON2_OTG_EN_SHIFT)
	    );
}

/* CON3 */

unsigned int bq24158_get_vender_code(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON3),
				     (&val), (unsigned char) (CON3_VENDER_CODE_MASK),
				     (unsigned char) (CON3_VENDER_CODE_SHIFT)
	    );
	return val;
}

unsigned int bq24158_get_pn(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON3),
				     (&val), (unsigned char) (CON3_PIN_MASK),
				     (unsigned char) (CON3_PIN_SHIFT)
	    );
	return val;
}

unsigned int bq24158_get_revision(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON3),
				     (&val), (unsigned char) (CON3_REVISION_MASK),
				     (unsigned char) (CON3_REVISION_SHIFT)
	    );
	return val;
}

/* CON4 */

void bq24158_set_reset(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_RESET_MASK),
				       (unsigned char) (CON4_RESET_SHIFT)
	    );
}

void bq24158_set_iocharge(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_I_CHR_MASK),
				       (unsigned char) (CON4_I_CHR_SHIFT)
	    );
}

unsigned int bq24158_get_iocharge(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON4),
				     (&val), (unsigned char) (CON4_I_CHR_MASK),
				     (unsigned char) (CON4_I_CHR_SHIFT)
	    );
	return val;
}

void bq24158_set_iterm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_I_TERM_MASK),
				       (unsigned char) (CON4_I_TERM_SHIFT)
	    );
}


unsigned int bq24158_get_iterm(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON4),
				     (&val), (unsigned char) (CON4_I_TERM_MASK),
				     (unsigned char) (CON4_I_TERM_SHIFT)
	    );
	return val;
}


/* CON5 */

void bq24158_set_dis_vreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_DIS_VREG_MASK),
				       (unsigned char) (CON5_DIS_VREG_SHIFT)
	    );
}

void bq24158_set_io_level(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_IO_LEVEL_MASK),
				       (unsigned char) (CON5_IO_LEVEL_SHIFT)
	    );
}

/* Begin added by bitao.xiong for defect-9031140 on 2020-03-21 */
static unsigned int bq24158_get_io_level(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON5),
				     (&val), (unsigned char) (CON5_IO_LEVEL_MASK),
				     (unsigned char) (CON5_IO_LEVEL_SHIFT)
	    );
	return val;
}
/* End added by bitao.xiong for defect-9031140 on 2020-03-21 */

unsigned int bq24158_get_sp_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON5),
				     (&val), (unsigned char) (CON5_SP_STATUS_MASK),
				     (unsigned char) (CON5_SP_STATUS_SHIFT)
	    );
	return val;
}

unsigned int bq241585_get_en_level(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface((unsigned char) (bq24158_CON5),
				     (&val), (unsigned char) (CON5_EN_LEVEL_MASK),
				     (unsigned char) (CON5_EN_LEVEL_SHIFT)
	    );
	return val;
}

void bq24158_set_vsp(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_VSP_MASK),
				       (unsigned char) (CON5_VSP_SHIFT)
	    );
}

/* CON6 */

void bq24158_set_i_safe(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_ISAFE_MASK),
				       (unsigned char) (CON6_ISAFE_SHIFT)
	    );
}

void bq24158_set_v_safe(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq24158_config_interface((unsigned char) (bq24158_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VSAFE_MASK),
				       (unsigned char) (CON6_VSAFE_SHIFT)
	    );
}

/**********************************************************
  *
  *   [Charge Function]
  *
  *********************************************************/
#define STATUS_OK	0
#define STATUS_FAIL	1
#define STATUS_UNSUPPORTED	-1
#define GETARRAYNUM(array) (sizeof(array)/sizeof(array[0]))

const u32 VBAT_CV_VTH_bq24158[] = {
	BATTERY_VOLT_03_500000_V, BATTERY_VOLT_03_520000_V, BATTERY_VOLT_03_540000_V, BATTERY_VOLT_03_560000_V,
	BATTERY_VOLT_03_580000_V, BATTERY_VOLT_03_600000_V, BATTERY_VOLT_03_620000_V, BATTERY_VOLT_03_640000_V,
	BATTERY_VOLT_03_660000_V, BATTERY_VOLT_03_680000_V, BATTERY_VOLT_03_700000_V, BATTERY_VOLT_03_720000_V,
	BATTERY_VOLT_03_740000_V, BATTERY_VOLT_03_760000_V, BATTERY_VOLT_03_780000_V, BATTERY_VOLT_03_800000_V,
	BATTERY_VOLT_03_820000_V, BATTERY_VOLT_03_840000_V, BATTERY_VOLT_03_860000_V, BATTERY_VOLT_03_880000_V,
	BATTERY_VOLT_03_900000_V, BATTERY_VOLT_03_920000_V, BATTERY_VOLT_03_940000_V, BATTERY_VOLT_03_960000_V,
	BATTERY_VOLT_03_980000_V, BATTERY_VOLT_04_000000_V, BATTERY_VOLT_04_020000_V, BATTERY_VOLT_04_040000_V,
	BATTERY_VOLT_04_060000_V, BATTERY_VOLT_04_080000_V, BATTERY_VOLT_04_100000_V, BATTERY_VOLT_04_120000_V,
	BATTERY_VOLT_04_140000_V, BATTERY_VOLT_04_160000_V, BATTERY_VOLT_04_180000_V, BATTERY_VOLT_04_200000_V,
	BATTERY_VOLT_04_220000_V, BATTERY_VOLT_04_240000_V, BATTERY_VOLT_04_260000_V, BATTERY_VOLT_04_280000_V,
	BATTERY_VOLT_04_300000_V, BATTERY_VOLT_04_320000_V, BATTERY_VOLT_04_340000_V, BATTERY_VOLT_04_360000_V,
	BATTERY_VOLT_04_380000_V, BATTERY_VOLT_04_400000_V, BATTERY_VOLT_04_420000_V, BATTERY_VOLT_04_440000_V
};

// Avoid the issue that the PC can't choose 550mA gear when charging
const u32 CS_VTH_bq24158[] = {
	500000, 650000, 750000, 850000, 950000, 1050000, 1150000, 1250000
};

const u32 INPUT_CS_VTH_bq24158[] = {
	100000, 500000, 800000
};

const u32 VCDT_HV_VTH_bq24158[] = {
	BATTERY_VOLT_04_200000_V, BATTERY_VOLT_04_250000_V, BATTERY_VOLT_04_300000_V, BATTERY_VOLT_04_350000_V,
	BATTERY_VOLT_04_400000_V, BATTERY_VOLT_04_450000_V, BATTERY_VOLT_04_500000_V, BATTERY_VOLT_04_550000_V,
	BATTERY_VOLT_04_600000_V, BATTERY_VOLT_06_000000_V, BATTERY_VOLT_06_500000_V, BATTERY_VOLT_07_000000_V,
	BATTERY_VOLT_07_500000_V, BATTERY_VOLT_08_500000_V, BATTERY_VOLT_09_500000_V, BATTERY_VOLT_10_500000_V
};

u32 charging_value_to_parameter_bq24158(const u32 *parameter, const u32 array_size, const u32 val)
{
	if (val < array_size)
		return parameter[val];
	pr_info("Can't find the parameter \r\n");
	return parameter[0];
}

u32 charging_parameter_to_value_bq24158(const u32 *parameter, const u32 array_size, const u32 val)
{
	u32 i;

	for (i = 0; i < array_size; i++)
		if (val == *(parameter + i))
			return i;

	pr_info("NO register value match \r\n");

	return 0;
}

static u32 bmt_find_closest_level(const u32 *pList, u32 number, u32 level)
{
	u32 i;
	u32 max_value_in_last_element;

	if (pList[0] < pList[1])
		max_value_in_last_element = true;
	else
		max_value_in_last_element = false;

	if (max_value_in_last_element == true) {
		for (i = (number - 1); i != 0; i--)	/* max value in the last element */
		{
			if (pList[i] <= level)
				return pList[i];
		}

		pr_info("Can't find closest level, small value first \r\n");
		return pList[0];
		/* return CHARGE_CURRENT_0_00_MA; */
	} else {
		for (i = 0; i < number; i++)	/* max value in the first element */
			if (pList[i] <= level)
				return pList[i];

		pr_info("Can't find closest level, large value first \r\n");
		return pList[number - 1];
		/* return CHARGE_CURRENT_0_00_MA; */
	}
}

static u32 bq24158_charging_hw_init(void)
{
	u32 status = STATUS_OK;

	pr_info("bq24158 charging_hw_init\r\n");
	
	// Modified init parameters by bin.song.hz for 5301378 at 2017.8.30 begin
   // bq24158_reg_config_interface(0x06, 0x8A);       /* B[7:4].I_max=1350mA,  B[3:0].V_max=4.4V *///Modified by hailong.chen for task 6948976 on 2018-09-04
    bq24158_reg_config_interface(0x06, 0xFA);		//I_MAX = 1550Ma  V_MAX = 4.4
    bq24158_reg_config_interface(0x02, 0xB6);       /* B[7:2].Voreg=4.4V, B[1].OTG_PL=1, B[0].OTG_EN=0 */
    bq24158_reg_config_interface(0x00, 0xC0);       /* B[7].TMR_RST=1, B[6].EN_STAT=1 */
    bq24158_reg_config_interface(0x01, 0xC8);       /* B[7:6].Iin_Limit=unlimit, B[5:4].Vlow=3.4V, B[3].TE=1, B[2].CE=0, B[1].HZ_MODE=0, B[0].OPA_MODE=0 */
    bq24158_reg_config_interface(0x05, 0x06);       /* B[5].LOW_CHG=0, B[2:0].Vsp = 4.68V */
    bq24158_reg_config_interface(0x04, 0x73);       /* B[6:4].I_chrg=1250mA, B[2:0].I_term.200mA*/
	// Modified init parameters by bin.song.hz for 5301378 at 2017.8.30 end

	return status;
}

static int bq24158_charger_enable(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	pr_info("bq24158_charger_enable %d\r\n", en);

	if (true == enable) {
		bq24158_set_ce(0);
		bq24158_set_hz_mode(0);
		bq24158_set_opa_mode(0);
	} else {
		bq24158_set_ce(1);
		bq24158_set_hz_mode(1);
	}

	return status;
}

static int bq24158_charger_is_enabled(struct charger_device *chg_dev, bool *en)
{
	*en = bq24158_get_ce();
	pr_info("bq24158_charger_is_enabled %d\r\n", *en);
	return 0;
}

static int bq24158_charger_get_ichg(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;
	/* Begin modified by bitao.xiong for defect-9031140 on 2020-03-21 */
	if (bq24158_get_io_level()) {
		*(u32 *) uA =  (22100 / 68) * 1000;//Actual charging current
	} else {
		/* Get current level */
		array_size = GETARRAYNUM(CS_VTH_bq24158);
		reg_value = bq24158_get_iocharge();
		*(u32 *) uA = charging_value_to_parameter_bq24158(CS_VTH_bq24158, array_size, reg_value);
	}
	/* End modified by bitao.xiong for defect-9031140 on 2020-03-21 */
	pr_info("bq24158_charger_get_ichg %dmA\r\n", *uA/1000);

	return status;
}

static int bq24158_charger_set_ichg(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u32 set_chr_current = 0;
	u32 array_size;
	u32 register_value = 0;

	if (uA <= 325000) { //325 = (22100 / 68) * 1000
		bq24158_set_io_level(1);
		set_chr_current = (22100 / 68) * 1000;//Actual charging current
	} else {
		bq24158_set_io_level(0);
		array_size = GETARRAYNUM(CS_VTH_bq24158);
		set_chr_current = bmt_find_closest_level(CS_VTH_bq24158, array_size, uA);
		register_value = charging_parameter_to_value_bq24158(CS_VTH_bq24158, array_size, set_chr_current);
		bq24158_set_iocharge(register_value);
	}

	pr_info("guzh bq24158_charger_set_ichg %dmA  register_value = %d\n", set_chr_current/1000,register_value);

	return status;
}

static int bq24158_charger_get_min_ichg(struct charger_device *chg_dev, u32 *uA)
{
	*uA = CS_VTH_bq24158[0];
	pr_info("bq24158_charger_get_min_ichg %dmA\r\n", *uA/1000);
	return 0;
}

static int bq24158_charger_set_cv(struct charger_device *chg_dev, u32 uV)
{
	u32 status = STATUS_OK;
	u16 register_value;
	u32 set_cv_voltage;	
	u32 array_size;

	array_size = GETARRAYNUM(VBAT_CV_VTH_bq24158);
	set_cv_voltage = bmt_find_closest_level(VBAT_CV_VTH_bq24158, array_size, uV);
	register_value = charging_parameter_to_value_bq24158(VBAT_CV_VTH_bq24158, array_size, set_cv_voltage);
	bq24158_set_oreg(register_value);

	pr_info("bq24158_charger_set_cv %dmV\r\n", set_cv_voltage/1000);
	return status;
}

static int bq24158_charger_get_cv(struct charger_device *chg_dev, u32 *uV)
{
	u32 status = STATUS_OK;
	u32 array_size;
	u8 reg_value;

	/* Get current level */
	array_size = GETARRAYNUM(VBAT_CV_VTH_bq24158);
	reg_value = bq24158_get_oreg();
	*(u32 *) uV = charging_value_to_parameter_bq24158(VBAT_CV_VTH_bq24158, array_size, reg_value);

	pr_info("bq24158_charger_get_cv %dmV\r\n", *uV/1000);

	return status;
}

static int bq24158_charger_set_aicr(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u32 register_value;

	if (uA > 800000) {
		register_value = 0x3;
	} 
	else if(uA > 500000 )
	{
		register_value = 0x2;
	}
	else if(uA > 100000)
	{
		register_value = 0x1;
	}
	else
	{
		register_value = 0x0;
	}

	bq24158_set_input_charging_current(register_value);

	pr_info("bq24158_charger_set_aicr %dmA\r\n", uA/1000);

	return status;
}

static int bq24158_charger_get_aicr(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;

	/* Get current level */
	reg_value = bq24158_get_input_charging_current();
	switch(reg_value)
	{
	case 3:
		*(u32 *) uA = 1000000;
		break;
	case 2:
		*(u32 *) uA = 800000;
		break;	
	case 1:
		*(u32 *) uA = 500000;
		break;	
	case 0:
		*(u32 *) uA = 100000;
		break;	
	default:
		*(u32 *) uA = 0;
		break;								
	}

	pr_info("bq24158_charger_get_aicr %dmA\r\n", *uA/1000);

	return status;
}

static int bq24158_charger_get_min_aicr(struct charger_device *chg_dev, u32 *uA)
{
	*uA = INPUT_CS_VTH_bq24158[0];
	pr_info("bq24158_charger_get_min_aicr %dmA\r\n", *uA/1000);
	return 0;
}

static int bq24158_charger_set_eoc_current(struct charger_device *chg_dev, u32 uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;
	if(uA >= 400000)
		reg_value = 0x07;
	else if(uA <=50000)
		reg_value = 0;
	else
		reg_value = uA/50000 - 1;

	bq24158_set_iterm(reg_value);

	pr_info("bq24158_charger_set_eoc_current %dmA\r\n", (reg_value+1)*50);

	return status;
}

static int bq24158_charger_get_eoc_current(struct charger_device *chg_dev, u32 *uA)
{
	u32 status = STATUS_OK;
	u8 reg_value;

	reg_value = bq24158_get_iterm();
	*uA = (reg_value+1) * 50000;

	return status;
}


static int bq24158_charger_enable_te(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	pr_info("bq24158_charger_enable_te %d\r\n", en);

	if (true == enable) {
		bq24158_set_te(1);
	} else {
		bq24158_set_te(0);
	}

	return status;
}

static int bq24158_charger_enable_otg(struct charger_device *chg_dev, bool en)
{
	int status = STATUS_OK;
	bool enable = en;

	pr_info("bq24158_charger_enable_otg %d\r\n", en);

	if (true == enable) {
		bq24158_set_hz_mode(0);
		bq24158_set_opa_mode(1);
		bq24158_set_otg_pl(1);
		bq24158_set_otg_en(1);
		//enable_boost_polling(1);//Added by hailong.chen for task 6439451 on 2018/06/30
	} else {
		bq24158_set_otg_pl(0);
		bq24158_set_otg_en(0);
		bq24158_set_opa_mode(0);
		bq24158_set_hz_mode(1);
		//enable_boost_polling(0);//Added by hailong.chen for task 6439451 on 2018/06/30
	}

	return status;
}

static int bq24158_charger_is_charging_done(struct charger_device *chg_dev, bool *done)
{
	u32 status = STATUS_OK;
	u32 ret_val;

	ret_val = bq24158_get_chip_status();

	if (ret_val == 0x2)
		* done = true;//Modified by rongxiao.deng for task 5642329 on 2017/11/24
	else
		* done = false;//Modified by rongxiao.deng for task 5642329 on 2017/11/24

	pr_info("bq24158_charger_is_charging_done %d\r\n", *done);

	return status;
}

static int bq24158_charger_dump_registers(struct charger_device *chg_dev)
{
	u32 status = STATUS_OK;

	bq24158_dump_register();

	return status;
}

static int bq24158_do_event(struct charger_device *chg_dev, u32 event,
				   u32 args)
{
	struct bq24158_INFO *bq = charger_get_data(chg_dev);

	dev_info(bq->dev, "%s\n", __func__);

	if (!bq->charger) {
		dev_notice(bq->dev, "%s: cannot get psy\n", __func__);
		return -ENODEV;
	}

	switch (event) {
	case EVENT_FULL:
	case EVENT_RECHARGE:
	case EVENT_DISCHARGE:
		power_supply_changed(bq->charger);
		break;
	default:
		break;
	}

	return 0;
}


/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/

#define VENDOR_CODE_bq24158   0x50
#define VENDOR_CODE_MASK      0xF8
#define IC_VER_MASK           0x07
  
void bq24158_hw_component_detect(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq24158_read_interface(0x03, &val, 0xFF, 0x0);

	if((val&VENDOR_CODE_MASK) == VENDOR_CODE_bq24158)
		g_bq24158_hw_exist = 1;
	else
		g_bq24158_hw_exist = 0;

    g_bq24158_hw_ver = val & IC_VER_MASK;

	pr_info("[bq24158_hw_component_detect] exist=%d, Reg[03]=0x%x\n", g_bq24158_hw_exist, val);
}

int is_bq24158_exist(void)
{
	pr_info("[is_bq24158_exist] g_bq24158_hw_exist=%d\n", g_bq24158_hw_exist);

	return g_bq24158_hw_exist;
}

void bq24158_dump_register(void)
{
	int i = 0;

	pr_info("[bq24158]\n");
	for (i = 0; i < bq24158_REG_NUM; i++) {
		bq24158_read_byte(i, &bq24158_reg[i]);
		pr_info("[0x%x]=0x%x\n", i, bq24158_reg[i]);
	}
}

//Begin added by hailong.chen for task 6439451 on 2018/06/30
static int bq24158_reset_watch_dog_timer(struct charger_device *chg_dev)
{
	bq24158_set_tmr_rst(1);
	pr_info("%s: use !\n", __func__);
	return 0;
}

#if 0
static void usbotg_boost_kick_work(struct work_struct *work)
{

	struct bq24158_INFO *boost_manager =
		container_of(work, struct bq24158_INFO, kick_work);

	pr_debug_ratelimited("usbotg_boost_kick_work\n");

	bq24158_set_tmr_rst(1);

	if (boost_manager->polling_enabled == true)
		gtimer_start(&boost_manager->otg_kthread_gtimer,
			     boost_manager->polling_interval);
}

static int usbotg_gtimer_func(struct gtimer *data)
{
	struct bq24158_INFO *boost_manager =
		container_of(data, struct bq24158_INFO,
			     otg_kthread_gtimer);

	queue_work(boost_manager->otg_boost_workq,
		   &boost_manager->kick_work);

	return 0;
}

static void enable_boost_polling(bool poll_en)
{
	if (bq24158_info) {
		if (poll_en) {
			gtimer_start(&bq24158_info->otg_kthread_gtimer,
				     bq24158_info->polling_interval);
			bq24158_info->polling_enabled = true;
		} else {
			bq24158_info->polling_enabled = false;
			gtimer_stop(&bq24158_info->otg_kthread_gtimer);
		}
	}
}
//End added by hailong.chen for task 6439451 on 2018/06/30
#endif

//Add begin by zihaogu for task-8189443 on 20191101
static int bq24158_get_mivr_state(struct charger_device *chg_dev, bool *in_loop)
{

       *in_loop = bq24158_get_sp_status();
       pr_info("bq24158 VIN loop status = %d \n",bq24158_get_sp_status());
       return 0;
}
//Add end by zihaogu for task-8189443 on 20191101


/* Begin modified by hailong.chen for task 7311063 on 2019-01-04 */
static int bq24158_charger_plug_in(struct charger_device *chg_dev)
{
/* Begin modified by zihaogu for defect 8634748 on 2019-12-13 */
	if (0x1 == bq24158_get_iterm()) {
		pr_info("bq24158 wdt timeout,reset some reg\n");
#if 0
		bq24158_reg_config_interface(0x06, 0xFA);
		bq24158_set_tmr_rst(0x1);//wdt reset
		bq24158_set_v_low(0x0);//Vlow=3.4V
		bq24158_set_te(0x1);//TE=1
		bq24158_set_vsp(0x6);//Vsp=4.68V
		//modify by zihaogu for 33mΩ I_term = 206mA
		bq24158_set_iterm(0x1);//I_term=150mA
#endif
                bq24158_reg_config_interface(0x06, 0xFA);
                bq24158_reg_config_interface(0x02, 0xB6);
                bq24158_reg_config_interface(0x01, 0xC8);
                bq24158_reg_config_interface(0x05, 0x06);
                bq24158_set_iterm(0x3);
                bq24158_reg_config_interface(0x00, 0xC0);
	}
/* End modified by zihaogu for defect 8634748 on 2019-12-13 */
	return 0;
}
/* End modified by hailong.chen for task 7311063 on 2019-01-04 */


static int bq24158_get_charger_type(struct bq24158_INFO *bq, union power_supply_propval *val)
{
	static struct power_supply *chg_psy;

	if (chg_psy == NULL) {
			chg_psy = power_supply_get_by_name("mtk_charger_type");
	}

	if (IS_ERR_OR_NULL(chg_psy)) {
		dev_notice(bq->dev, "%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	} else {
		power_supply_get_property(chg_psy,
				POWER_SUPPLY_PROP_USB_TYPE, val);
		switch (val->intval) {
		case POWER_SUPPLY_USB_TYPE_UNKNOWN:
			bq->charger_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
			break;
		case POWER_SUPPLY_USB_TYPE_SDP:
			bq->charger_desc.type = POWER_SUPPLY_TYPE_USB;
			break;
		case POWER_SUPPLY_USB_TYPE_CDP:
			bq->charger_desc.type = POWER_SUPPLY_TYPE_USB_CDP;
			break;
		case POWER_SUPPLY_USB_TYPE_DCP:
			bq->charger_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
			break;
		}
	}
	return 0;
}

static int bq24158_get_online(struct bq24158_INFO *bq, union power_supply_propval *val)
{
	static struct power_supply *chg_psy;
	union power_supply_propval prop;

	if (chg_psy == NULL) {
			chg_psy = power_supply_get_by_name("mtk_charger_type");
	}

	if (IS_ERR_OR_NULL(chg_psy)) {
		dev_notice(bq->dev, "%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	} else {
		power_supply_get_property(chg_psy,
				POWER_SUPPLY_PROP_VOLTAGE_NOW, &prop);
		val->intval = prop.intval > VBUS_THR ? 1 : 0;
	}
	return 0;
}

static int bq24158_power_supply_get_property(struct power_supply *psy,
					     enum power_supply_property psp,
					     union power_supply_propval *val)
{
	struct bq24158_INFO *bq = power_supply_get_drvdata(psy);
	int ret = 0;
	enum bq24158_charging_status chg_stat = BQ24158_CHG_STATUS_MAX;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		ret = bq24158_get_online(bq, val);
		if (ret)
			return -ENODATA;
		break;
	case POWER_SUPPLY_PROP_STATUS:
		ret = bq24158_get_charge_status(&chg_stat);
		if (!ret)
			pr_err("%s:get charge status failed\n", __func__);
		switch (chg_stat) {
		case BQ24158_CHG_STATUS_READY:
			val->intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
			break;
		case BQ24158_CHG_STATUS_PROGRESS:
			val->intval = POWER_SUPPLY_STATUS_CHARGING;
			break;
		case BQ24158_CHG_STATUS_DONE:
			val->intval = POWER_SUPPLY_STATUS_FULL;
			break;
		case BQ24158_CHG_STATUS_FAULT:
			val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
			break;
		default:
			ret = -ENODATA;
		}
		break;
	case POWER_SUPPLY_PROP_USB_TYPE:
		ret = bq24158_get_charger_type(bq, val);
		if (ret) return -ENODATA;
		break;

	default:
		ret = -ENODATA;
	}

	return ret;
}

static __maybe_unused enum power_supply_property bq24158_power_supply_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_TYPE,
	POWER_SUPPLY_PROP_USB_TYPE,
};

static __maybe_unused char *bq24158_charger_supplied_to[] = {
	"battery",
	"mtk-master-charger"
};

static enum power_supply_usb_type bq24158_charger_usb_types[] = {
	POWER_SUPPLY_USB_TYPE_UNKNOWN,
	POWER_SUPPLY_USB_TYPE_SDP,
	POWER_SUPPLY_USB_TYPE_DCP,
	POWER_SUPPLY_USB_TYPE_CDP,
	POWER_SUPPLY_USB_TYPE_FLOAT,
};

static int bq24158_notifier_call(struct notifier_block *nb,
				unsigned long event, void *data)
{
	struct power_supply *psy = data;
	struct bq24158_INFO *bq = container_of(nb, struct bq24158_INFO, nb);
	union power_supply_propval prop;
	int ret;
	if (event != PSY_EVENT_PROP_CHANGED)
		return NOTIFY_OK;
	if (strcmp(psy->desc->name, "mtk_charger_type") != 0) {
		return NOTIFY_OK;
	}
  	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_ONLINE,
  			&prop);
  	if (ret != 0)
  		return NOTIFY_OK;

	if (prop.intval) {
		cancel_delayed_work(&bq->work);
		schedule_delayed_work(&bq->work, msecs_to_jiffies(100));
	} else {
		cancel_delayed_work(&bq->work);
	}

	power_supply_changed(bq->charger);
	
	dev_info(bq->dev, "%s\n", __func__);
	return NOTIFY_OK;
}


static __maybe_unused int bq24158_power_supply_init(struct bq24158_INFO *bq)
{
	struct power_supply_config psy_cfg = {
		.drv_data = bq,
		.of_node = bq->dev->of_node};

	psy_cfg.supplied_to = bq24158_charger_supplied_to;
	psy_cfg.num_supplicants = ARRAY_SIZE(bq24158_charger_supplied_to);

	bq->charger_desc.name = "bq24158";
	bq->charger_desc.type = POWER_SUPPLY_TYPE_USB;
	bq->charger_desc.properties = bq24158_power_supply_props;
	bq->charger_desc.num_properties =
			ARRAY_SIZE(bq24158_power_supply_props);
	bq->charger_desc.get_property = bq24158_power_supply_get_property;
	bq->charger_desc.usb_types = bq24158_charger_usb_types;
	bq->charger_desc.num_usb_types = ARRAY_SIZE(bq24158_charger_usb_types);

	bq->charger = devm_power_supply_register(bq->dev,
						 &bq->charger_desc,
						 &psy_cfg);
	if (bq->charger) {
		bq->nb.notifier_call = bq24158_notifier_call;
		return power_supply_reg_notifier(&bq->nb);
		
	} else { 
		return PTR_ERR_OR_ZERO(bq->charger);
	}
}

#define BQ24158_TIMER_TIMEOUT		5
static void bq24158_timer_work(struct work_struct *work)
{
	struct bq24158_INFO *bq = container_of(work, struct bq24158_INFO,
						work.work);
	union power_supply_propval prop;
	int ret = 0;
	ret = bq24158_get_charger_type(bq, &prop);
	if (ret || prop.intval ==  POWER_SUPPLY_USB_TYPE_UNKNOWN)
		return;
	power_supply_changed(bq->charger);
	schedule_delayed_work(&bq->work, BQ24158_TIMER_TIMEOUT * HZ);
	dev_info(bq->dev, "%s\n", __func__);
	
}

static const struct charger_ops bq24158_chg_ops = {
	/* enable */
	.enable = bq24158_charger_enable,
	.is_enabled = bq24158_charger_is_enabled,
	/* charging current */
	.get_charging_current = bq24158_charger_get_ichg,
	.set_charging_current = bq24158_charger_set_ichg,
	.get_min_charging_current = bq24158_charger_get_min_ichg,
	/* charging voltage */
	.set_constant_voltage = bq24158_charger_set_cv,
	.get_constant_voltage = bq24158_charger_get_cv,
	/* charging input current */
	.get_input_current = bq24158_charger_get_aicr,
	.set_input_current = bq24158_charger_set_aicr,
	.get_min_input_current = bq24158_charger_get_min_aicr,
	/* set termination current */
	.get_eoc_current = bq24158_charger_get_eoc_current,
	.set_eoc_current = bq24158_charger_set_eoc_current,
	/* charing termination */
	.enable_termination = bq24158_charger_enable_te,
	/* OTG */
	.enable_otg = bq24158_charger_enable_otg,
	/* misc */
	.is_charging_done = bq24158_charger_is_charging_done,
	.dump_registers = bq24158_charger_dump_registers,
	/* Event */
	.event = bq24158_do_event,
//Begin added by hailong.chen for task 6439451 on 2018/06/30
	.kick_wdt = bq24158_reset_watch_dog_timer,
//End added by hailong.chen for task 6439451 on 2018/06/30
	/* Begin added by hailong.chen for task 7052341 on 2018-10-25 */
	.plug_in = bq24158_charger_plug_in,
	/* End added by hailong.chen for task 7052341 on 2018-10-25 */
	.get_mivr_state = bq24158_get_mivr_state,
};

static const struct charger_properties bq24158_chg_props = {
	.alias_name = "bq24158",
};

char charger_module_name[256];//Modified by hailong.chen for task 5662647 cancel the annotation to adding charge ic deviceinfo on 2017/11/28

static int bq24158_driver_probe(struct i2c_client *i2c, const struct i2c_device_id *id)
{
	union power_supply_propval prop;
	struct power_supply *chg_psy;

	pr_info("******** bq24158_driver_probe!! ********\n");
	chg_psy = power_supply_get_by_name("mtk_charger_type");
	if (IS_ERR_OR_NULL(chg_psy)) {
		pr_notice("%s  requests probe deferral:Couldn't get chg_psy\n", __func__);
		return -EPROBE_DEFER;
	}

	bq24158_info = devm_kzalloc(&i2c->dev, sizeof(*bq24158_info), GFP_KERNEL);
	if (!bq24158_info)
		return -ENOMEM;

	bq24158_info->dev = &i2c->dev;
	bq24158_info->i2c = i2c;
	i2c_set_clientdata(i2c, bq24158_info);
	
	//mutex_init(&bq24158_i2c_access);
	sema_init(&bq24158_info->suspend_lock, 1);

	bq24158_reg_config_interface(0x06, 0xFA);
	bq24158_hw_component_detect();
	
	if(g_bq24158_hw_exist == 0)
	    return -ENODEV;
	    
	bq24158_charging_hw_init();

	bq24158_power_supply_init(bq24158_info);
	/* charger class register */
	bq24158_info->chg_dev = charger_device_register("primary_chg", 
                                              bq24158_info->dev, 
                                              bq24158_info,
					      &bq24158_chg_ops,
					      &bq24158_chg_props);
	if (IS_ERR(bq24158_info->chg_dev)) {
		dev_info(bq24158_info->dev, "charger device register fail\n");
		return PTR_ERR(bq24158_info->chg_dev);
	}

	INIT_DELAYED_WORK(&bq24158_info->work, bq24158_timer_work);

	if (!bq24158_get_online(bq24158_info, &prop) && prop.intval)
		schedule_delayed_work(&bq24158_info->work, msecs_to_jiffies(2000));

	device_init_wakeup(bq24158_info->dev, true);
	bq24158_dump_register();
	#if 0
//Begin added by hailong.chen for task 6439451 on 2018/06/30
	gtimer_init(&bq24158_info->otg_kthread_gtimer, bq24158_info->dev, "otg_boost");
	bq24158_info->otg_kthread_gtimer.callback = usbotg_gtimer_func;

	bq24158_info->otg_boost_workq = create_singlethread_workqueue("otg_boost_workq");
	INIT_WORK(&bq24158_info->kick_work, usbotg_boost_kick_work);
	bq24158_info->polling_interval = 20;
	#endif
//End added by hailong.chen for task 6439451 on 2018/06/30
	chargin_hw_init_done_bq24158 = true;
    	//Begin Modified by hailong.chen for task 5662647 cancel the annotation to adding charge ic deviceinfo on 2017/11/28
    	sprintf(charger_module_name,"bq24158:TI:0x%x", g_bq24158_hw_ver);
    	//End Modified by hailong.chen for task 5662647 cancel the annotation to adding charge ic deviceinfo on 2017/11/28
	return 0;
}


static int bq24158_suspend(struct device *dev)
{
	pr_info("%s\n", __func__);
	down(&bq24158_info->suspend_lock);
	return 0;
}


static int bq24158_resume(struct device *dev)
{
	pr_info("%s\n", __func__);
	up(&bq24158_info->suspend_lock);
	return 0;
}
/**********************************************************
  *
  *   [platform_driver API]
  *
  *********************************************************/
unsigned char g_reg_value_bq24158 = 0;
static ssize_t show_bq24158_access(struct device *dev, struct device_attribute *attr, char *buf)
{
	pr_info("[show_bq24158_access] 0x%x\n", g_reg_value_bq24158);
	return sprintf(buf, "%u\n", g_reg_value_bq24158);
}

static ssize_t store_bq24158_access(struct device *dev, struct device_attribute *attr,
				    const char *buf, size_t size)
{
	int ret = 0;
	char *pvalue = NULL, *addr, *val;
	unsigned int reg_value = 0;
	unsigned int reg_address = 0;

	pr_info("[store_bq24158_access]\n");

	if (buf != NULL && size != 0) {

		pvalue = (char *)buf;
		if (size > 3) {
			addr = strsep(&pvalue, " ");
			ret = kstrtou32(addr, 16, (unsigned int *)&reg_address);
		} else
			ret = kstrtou32(pvalue, 16, (unsigned int *)&reg_address);

		if (size > 3) {
			val = strsep(&pvalue, " ");
			ret = kstrtou32(val, 16, (unsigned int *)&reg_value);

			pr_info("[store_bq24158_access] write bq24158 reg 0x%x with value 0x%x !\n",
			     reg_address, reg_value);
			ret = bq24158_config_interface(reg_address, reg_value, 0xFF, 0x0);
		} else {
			ret = bq24158_read_interface(reg_address, &g_reg_value_bq24158, 0xFF, 0x0);
			pr_info("[store_bq24158_access] read bq24158 reg 0x%x with value 0x%x !\n",
			     reg_address, g_reg_value_bq24158);
			pr_info("[store_bq24158_access] Please use \"cat bq24158_access\" to get value\r\n");
		}
	}
	return size;
}

static DEVICE_ATTR(bq24158_access, 0664, show_bq24158_access, store_bq24158_access);	/* 664 */

static int bq24158_user_space_probe(struct platform_device *dev)
{
	int ret_device_file = 0;

	pr_info("******** bq24158_user_space_probe!! ********\n");

	ret_device_file = device_create_file(&(dev->dev), &dev_attr_bq24158_access);

	return 0;
}

struct platform_device bq24158_user_space_device = {
	.name = "bq24158-user",
	.id = -1,
};

static struct platform_driver bq24158_user_space_driver = {
	.probe = bq24158_user_space_probe,
	.driver = {
		   .name = "bq24158-user",
	},
};

static int __init bq24158_init(void)
{
	int ret = 0;

	if (i2c_add_driver(&bq24158_driver) != 0) {
		pr_info("[bq24158_init] failed to register bq24158 i2c driver.\n");
	} else {
		pr_info("[bq24158_init] Success to register bq24158 i2c driver.\n");
	}

    //Begin Modified by hailong.chen for task 5662647 cancel the annotation to make the charging ic compatible on 2017/11/28
   /* if(g_bq24158_hw_exist == 0)
    {
        pr_info("[bq24158_init] bq24158 is not exist, delete bq24158 i2c driver.\n");
        i2c_del_driver(&bq24158_driver);
        return 0;
    }*/
    //End Modified by hailong.chen for task 5662647 cancel the annotation to make the charging ic compatible on 2017/11/28

	/* bq24158 user space access interface */
	ret = platform_device_register(&bq24158_user_space_device);
	if (ret) {
		pr_info("****[bq24158_init] Unable to device register(%d)\n", ret);
		return ret;
	}
	ret = platform_driver_register(&bq24158_user_space_driver);
	if (ret) {
		pr_info("****[bq24158_init] Unable to register driver (%d)\n", ret);
		return ret;
	}

	return 0;
}

static void __exit bq24158_exit(void)
{
	i2c_del_driver(&bq24158_driver);
}

module_init(bq24158_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C bq24158 Driver");
MODULE_AUTHOR("James Lo<james.lo@mediatek.com>");
