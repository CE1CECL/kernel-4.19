#ifndef CONFIG_MTK_LCM_DEVICE_TREE_SUPPORT
#ifndef BUILD_LK
    #include <linux/string.h>
    #include <linux/kernel.h>
#else
#include <string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
    #include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
    #include <asm/arch/mt_gpio.h>
    #include <platform/mt_pmic.h>
#else
    //#include <mt-plat/mt_gpio.h>
    //#include <mach/gpio_const.h>
#endif

//#include "../../pmic/include/mt6357/mtk_pmic_api.h"

#ifndef  GTP_RST_PORT
#define GTP_RST_PORT    0
#endif

#ifdef BUILD_LK
#define LCD_DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define LCD_DEBUG(fmt, args...) printk(fmt, ##args)
#endif

extern int tps65132_write_bytes(unsigned char addr, unsigned char value);

//extern void tpd_gpio_output(int pin, int level);

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1520)//(1440)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif
#if 0
#ifndef GPIO_LCD_BIAS_ENP_PIN
#define GPIO_LCD_BIAS_ENP_PIN        			(GPIO173 | 0x80000000)
#endif
#ifndef GPIO_LCD_BIAS_ENN_PIN
#define GPIO_LCD_BIAS_ENN_PIN        			(GPIO171 | 0x80000000)
#endif

#ifndef GPIO_LCM_ID0
#define GPIO_LCM_ID0				(GPIO16 | 0x80000000)
#endif
#ifndef GPIO_LCM_ID1
#define GPIO_LCM_ID1				(GPIO23 | 0x80000000)
#endif
#endif
#define REGFLAG_END_OF_TABLE		(0xFFFD)
#define REGFLAG_DELAY		0xFFFC
#define REGFLAG_UDELAY	0xFFFB
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static struct LCM_UTIL_FUNCS lcm_util = {0};

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define SET_RESET_PIN(v)    				(lcm_util.set_reset_pin((v)))
#define MDELAY(n)					(lcm_util.mdelay(n))
#define UDELAY(n)					(lcm_util.udelay(n))
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)     

#define set_gpio_lcd_enn(cmd) \
	lcm_util.set_gpio_lcd_enn_bias(cmd)
#define set_gpio_lcd_enp(cmd) \
	lcm_util.set_gpio_lcd_enp_bias(cmd)


struct LCM_setting_table {
	unsigned int cmd;
	unsigned char count;
	unsigned char para_list[64];
};

//extern void lcm_array_fill(int array_left,int array_num,struct LCM_setting_table *lcm_pata,unsigned int  data_array[],unsigned int i);
//extern void lcm_array_cmdq(const struct LCM_UTIL_FUNCS *util,struct LCM_setting_table *lcm_pata,unsigned int acount);

//begin modify by kun.zheng for task 9391619 on 2020/05/09
static struct LCM_setting_table  lcm_initialization_setting_ty[] = {
{0xFF,1,{0x20}},
{0xFB,1,{0x01}},
{0x05,1,{0xA9}},
{0x07,1,{0x69}},
{0x08,1,{0xCB}},
{0x0E,1,{0x87}},
{0x0F,1,{0x55}},
{0x1F,1,{0x00}},
{0x69,1,{0xA9}},

//{0x88,1,{0x00}},  //after LCM OTP, Vcom setting should be removed.
//{0x89,1,{0x69}},
//{0x8A,1,{0x69}},
//{0x8B,1,{0x69}},
//{0x8C,1,{0x69}},

{0x95,1,{0xEB}},
{0x96,1,{0xEB}},//
////GAMMA SETTING////
//CMD2_Page0
{0xFF,1,{0x20}},
{0xFB,1,{0x01}},
//R+
{0xB0,16,{0x00,0x00,0x00,0x34,0x00,0x6E,0x00,0x97,0x00,0xB7,0x00,0xD1,0x00,0xE8,0x00,0xFB}},
{0xB1,16,{0x01,0x0C,0x01,0x42,0x01,0x68,0x01,0xA3,0x01,0xCD,0x02,0x0D,0x02,0x3D,0x02,0x3F}},
{0xB2,16,{0x02,0x6E,0x02,0xA6,0x02,0xCE,0x03,0x01,0x03,0x22,0x03,0x55,0x03,0x63,0x03,0x74}},
{0xB3,12,{0x03,0x86,0x03,0x9C,0x03,0xB4,0x03,0xC7,0x03,0xCF,0x03,0xD9}},
//G+
{0xB4,16,{0x00,0x00,0x00,0x34,0x00,0x6E,0x00,0x97,0x00,0xB7,0x00,0xD1,0x00,0xE8,0x00,0xFB}},
{0xB5,16,{0x01,0x0C,0x01,0x42,0x01,0x68,0x01,0xA3,0x01,0xCD,0x02,0x0D,0x02,0x3D,0x02,0x3F}},
{0xB6,16,{0x02,0x6E,0x02,0xA6,0x02,0xCE,0x03,0x01,0x03,0x22,0x03,0x55,0x03,0x63,0x03,0x74}},
{0xB7,12,{0x03,0x86,0x03,0x9C,0x03,0xB4,0x03,0xC7,0x03,0xCF,0x03,0xD9}},
//B+
{0xB8,16,{0x00,0x00,0x00,0x34,0x00,0x6E,0x00,0x97,0x00,0xB7,0x00,0xD1,0x00,0xE8,0x00,0xFB}},
{0xB9,16,{0x01,0x0C,0x01,0x42,0x01,0x68,0x01,0xA3,0x01,0xCD,0x02,0x0D,0x02,0x3D,0x02,0x3F}},
{0xBA,16,{0x02,0x6E,0x02,0xA6,0x02,0xCE,0x03,0x01,0x03,0x22,0x03,0x55,0x03,0x63,0x03,0x74}},
{0xBB,12,{0x03,0x86,0x03,0x9C,0x03,0xB4,0x03,0xC7,0x03,0xCF,0x03,0xD9}},
//CMD2_Page1
{0xFF,1,{0x21}},
{0xFB,1,{0x01}},
//R-
{0xB0,16,{0x00,0x00,0x00,0x34,0x00,0x6E,0x00,0x97,0x00,0xB7,0x00,0xD1,0x00,0xE8,0x00,0xFB}},
{0xB1,16,{0x01,0x0C,0x01,0x42,0x01,0x68,0x01,0xA3,0x01,0xCD,0x02,0x0D,0x02,0x3D,0x02,0x3F}},
{0xB2,16,{0x02,0x6E,0x02,0xA6,0x02,0xCE,0x03,0x01,0x03,0x22,0x03,0x55,0x03,0x63,0x03,0x74}},
{0xB3,12,{0x03,0x86,0x03,0x9C,0x03,0xB4,0x03,0xC7,0x03,0xCF,0x03,0xD9}},
//G-
{0xB4,16,{0x00,0x00,0x00,0x34,0x00,0x6E,0x00,0x97,0x00,0xB7,0x00,0xD1,0x00,0xE8,0x00,0xFB}},
{0xB5,16,{0x01,0x0C,0x01,0x42,0x01,0x68,0x01,0xA3,0x01,0xCD,0x02,0x0D,0x02,0x3D,0x02,0x3F}},
{0xB6,16,{0x02,0x6E,0x02,0xA6,0x02,0xCE,0x03,0x01,0x03,0x22,0x03,0x55,0x03,0x63,0x03,0x74}},
{0xB7,12,{0x03,0x86,0x03,0x9C,0x03,0xB4,0x03,0xC7,0x03,0xCF,0x03,0xD9}},
//B-
{0xB8,16,{0x00,0x00,0x00,0x34,0x00,0x6E,0x00,0x97,0x00,0xB7,0x00,0xD1,0x00,0xE8,0x00,0xFB}},
{0xB9,16,{0x01,0x0C,0x01,0x42,0x01,0x68,0x01,0xA3,0x01,0xCD,0x02,0x0D,0x02,0x3D,0x02,0x3F}},
{0xBA,16,{0x02,0x6E,0x02,0xA6,0x02,0xCE,0x03,0x01,0x03,0x22,0x03,0x55,0x03,0x63,0x03,0x74}},
{0xBB,12,{0x03,0x86,0x03,0x9C,0x03,0xB4,0x03,0xC7,0x03,0xCF,0x03,0xD9}},
//
//
//
{0xFF,1,{0x23}},
{0xFB,1,{0x01}},
{0x12,1,{0xAB}},
{0x15,1,{0xF5}},
{0x16,1,{0x0B}},

{0xFF,1,{0x24}},
{0xFB,1,{0x01}},
{0x00,1,{0x20}},
{0x01,1,{0x20}},
{0x02,1,{0x05}},
{0x03,1,{0x05}},
{0x04,1,{0x9E}},
{0x05,1,{0x9E}},
{0x06,1,{0x9F}},
{0x07,1,{0x9F}},
{0x08,1,{0x0C}},
{0x09,1,{0x03}},
{0x0A,1,{0x0D}},
{0x0B,1,{0x0E}},
{0x0C,1,{0x0F}},
{0x0D,1,{0x10}},
{0x0E,1,{0x11}},
{0x0F,1,{0x12}},
{0x10,1,{0x13}},
{0x11,1,{0x04}},
{0x12,1,{0x04}},
{0x13,1,{0x20}},
{0x14,1,{0x20}},
{0x15,1,{0x20}},

{0x16,1,{0x20}},
{0x17,1,{0x20}},
{0x18,1,{0x05}},
{0x19,1,{0x05}},
{0x1A,1,{0x9E}},
{0x1B,1,{0x9E}},
{0x1C,1,{0x9F}},
{0x1D,1,{0x9F}},
{0x1E,1,{0x0C}},
{0x1F,1,{0x03}},
{0x20,1,{0x0D}},
{0x21,1,{0x0E}},
{0x22,1,{0x0F}},
{0x23,1,{0x10}},
{0x24,1,{0x11}},
{0x25,1,{0x12}},
{0x26,1,{0x13}},
{0x27,1,{0x04}},
{0x28,1,{0x04}},
{0x29,1,{0x20}},
{0x2A,1,{0x20}},
{0x2B,1,{0x20}},

{0x2F,1,{0x0C}},
{0x30,1,{0x40}},
{0x33,1,{0x40}},
{0x34,1,{0x0C}},
{0x37,1,{0x77}},
{0x3A,1,{0x9A}},
{0x3B,1,{0x95}},
{0x3D,1,{0x92}},
{0x4D,1,{0x15}},
{0x4E,1,{0x26}},
{0x4F,1,{0x37}},
{0x50,1,{0x48}},
{0x51,1,{0x84}},
{0x52,1,{0x73}},
{0x53,1,{0x62}},
{0x54,1,{0x51}},
{0x55,1,{0x86}},
{0x56,1,{0x78}},
{0x5A,1,{0x9A}},
{0x5B,1,{0x95}},
{0x5C,1,{0x8F}},
{0x5D,1,{0x0A}},
{0x5E,1,{0x10}},

{0x60,1,{0x80}},
{0x61,1,{0x7C}},
{0x64,1,{0x11}},

{0x85,1,{0x11}},
{0x92,1,{0xAD}},
{0x93,1,{0x08}},
{0x94,1,{0x06}},

{0xAB,1,{0x00}},
{0xAD,1,{0x00}},
{0xB0,1,{0x05}},
{0xB1,1,{0xA9}},

{0xFF,1,{0x25}},
{0xFB,1,{0x01}},
{0x0A,1,{0x82}},
{0x0B,1,{0x9C}},
{0x0C,1,{0x01}},
{0x17,1,{0x82}},
{0x18,1,{0x06}},
{0x19,1,{0x0F}},
{0x1F,1,{0x9A}},
{0x20,1,{0x95}},
{0x23,1,{0x05}},
{0x24,1,{0xA9}},
{0x26,1,{0x9A}},
{0x27,1,{0x95}},
{0x2A,1,{0x05}},
{0x2B,1,{0xA9}},
{0x2F,1,{0x80}},
{0x40,1,{0x10}},
{0x41,1,{0x80}},
{0x42,1,{0xA6}},
{0x43,1,{0x95}},
{0x46,1,{0x05}},
{0x47,1,{0xA9}},
{0x4C,1,{0x95}},
{0x4E,1,{0x95}},
{0x4F,1,{0xA6}},
{0x50,1,{0x95}},
{0x53,1,{0x05}},
{0x54,1,{0xA9}},
{0x55,1,{0x05}},
{0x56,1,{0xA9}},
{0x5A,1,{0x80}},
{0x5B,1,{0x80}},
{0x5D,1,{0x9A}},
{0x5E,1,{0x95}},
{0x5F,1,{0x9A}},
{0x60,1,{0x95}},
{0x61,1,{0x9A}},
{0x62,1,{0x95}},
{0x65,1,{0x05}},
{0x66,1,{0xA9}},
{0xD5,1,{0x66}},

{0xFF,1,{0x26}},
{0xFB,1,{0x01}},
{0x04,1,{0x42}},
{0x06,1,{0xFF}},
{0x0C,1,{0x0B}},
{0x0D,1,{0x01}},
{0x0E,1,{0x02}},
{0x0F,1,{0x06}},
{0x10,1,{0x07}},
{0x13,1,{0x24}},
{0x14,1,{0x88}},
{0x16,1,{0x81}},
{0x19,1,{0x1A}},
{0x1A,1,{0x0D}},
{0x1B,1,{0x12}},
{0x1C,1,{0x82}},
{0x1E,1,{0xAD}},
{0x1F,1,{0xAD}},
{0x24,1,{0x00}},
{0x2F,1,{0x04}},
{0x30,1,{0xAD}},
{0x31,1,{0x11}},
{0x32,1,{0x11}},
{0x34,1,{0x04}},
{0x35,1,{0xAD}},
{0x36,1,{0x81}},
{0x37,1,{0x67}},
{0x38,1,{0x11}},
{0x3F,1,{0x10}},
{0x40,1,{0xAD}},
{0x58,1,{0xD6}},
{0x59,1,{0xD6}},
{0x5A,1,{0xD6}},
{0x5B,1,{0xAD}},
{0x5C,1,{0x00}},
{0x5D,1,{0x26}},
{0x5E,1,{0x10}},
{0x63,1,{0x9A}},
{0x64,1,{0x95}},
{0x65,1,{0x9A}},
{0x66,1,{0x95}},
{0x67,1,{0x9A}},
{0x68,1,{0x95}},
{0x6B,1,{0x00}},
{0x6D,1,{0x00}},
{0x70,1,{0x05}},
{0x71,1,{0xD2}},
{0x73,1,{0x9A}},
{0x74,1,{0x95}},
{0x77,1,{0x05}},
{0x78,1,{0xD2}},
{0x7A,1,{0x9A}},
{0x7B,1,{0x95}},
{0x7E,1,{0x05}},
{0x7F,1,{0xD2}},
{0x82,1,{0x9A}},
{0x83,1,{0x95}},
{0x84,1,{0x9A}},
{0x85,1,{0x95}},
{0x86,1,{0x9A}},
{0x87,1,{0x95}},
{0x8A,1,{0x05}},
{0x8B,1,{0xA9}},
{0x8F,1,{0x00}},
{0x90,1,{0x00}},
{0x92,1,{0x05}},
{0x93,1,{0xF0}},
{0x99,1,{0x0D}},
{0x9A,1,{0x36}},
{0x9B,1,{0x0C}},
{0x9C,1,{0x9E}},

{0xFF,1,{0x27}},
{0xFB,1,{0x01}},
{0x13,1,{0x00}},
{0x14,1,{0x55}},

///////////////////////////////////////////////////////
//////////////////// 525B modified ////////////////////
///////////////////////////////////////////////////////

{0xFF,1,{0x24}},
//{REGFLAG_DELAY,1,{0x00}},
{0xFB,1,{0x01}},
// Notch \'ac\'db\'c3\'f6\'b3]\'a9w (notch 48 lines + ASW/EQ setting follow Normal \'b0\'cf\'b0\'ec)
{0x88,1,{0x30}},
{0x89,1,{0xCF}},
{0x8A,1,{0x0A}},

// Dynamic Long H setting
{0xFF,1,{0x26}},
//{REGFLAG_DELAY,1,{0x00}},
{0xFB,1,{0x01}},
{0x44,1,{0x09}},
{0x45,1,{0x8E}},
{0xA9,1,{0x3E}},
{0xAA,1,{0x2A}},
{0xAB,1,{0x28}},
{0xAC,1,{0x32}},
{0xAD,1,{0x3C}},
{0xAE,1,{0x2E}},
{0xAF,1,{0x38}},
{0xB0,1,{0x2C}},

//{0xFF,1,{0x20}},
//{0xFB,1,{0x01}},
//{0x30,1,{0x00}},

//{0xFF,1,{0x24}},
//{0xFB,1,{0x01}},
//{0xC4,1,{0x29}},
//{0xC5,1,{0x70}},

///////////////////////////////////////////////////////

{0xFF,1,{0x24}},
{0xFB,1,{0x01}},
{0xC4,1,{0x24}},
{0xC5,1,{0x30}},

{0xFF,1,{0x26}},
{0xFB,1,{0x01}},
{0x45,1,{0x0E}},
{0x13,1,{0x26}},
{0xA9,1,{0x3E}},
{0xAA,1,{0x2A}},
{0xAB,1,{0x3A}},
{0xAC,1,{0x32}},
{0xAD,1,{0x3C}},
{0xAE,1,{0x2E}},
{0xAF,1,{0x34}},
{0xB0,1,{0x2C}},


{0xFF,1,{0x10}},
{0xFB,1,{0x01}},
{0xBA,1,{0x03}},
{0x35,1,{0x00}},
{0x36,1,{0x00}},//add by yusen.ke.sz for fix kernel display error on 20200911
//begin modify by kun.zheng for task 8998289 on 2020/04/28
{0x11,1,{0x00}},
{REGFLAG_DELAY,100,{0x00}},//modify by yusen.ke.sz for fix lcm sleep out error on 20201012
{0x29,1,{0x00}},
{REGFLAG_DELAY,20,{0x00}},//modify by yusen.ke.sz for fix lcm sleep out error on 20201012
{REGFLAG_END_OF_TABLE,1,{0x00}},
//end modify by kun.zheng for task 8998289 on 2020/04/28
};
//end modify by kun.zheng for task 9391619 on 2020/05/09

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const struct LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(struct LCM_UTIL_FUNCS));
}

static void lcm_get_params(struct LCM_PARAMS *params)
{
	//LCD_DEBUG("\t\t ft8006 [lcm_get_params]\n");

	memset(params, 0, sizeof(struct LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	//params->density = LCM_DENSITY;  //density in TctOverrideProp.mk is effective

	// enable tearing-free
	//params->dbi.te_mode 			= LCM_DBI_TE_MODE_VSYNC_ONLY;
	//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;


	params->dsi.mode   = SYNC_EVENT_VDO_MODE;


	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM				= LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order	= LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   		= LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     		= LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      		= LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	params->dsi.packet_size=256;

	// Video mode setting

	params->dsi.intermediat_buffer_num = 2;

	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

	//params->dsi.word_count=480*3;

	//here is for esd protect by legen
	//params->dsi.noncont_clock = true;
	//params->dsi.noncont_clock_period=2;
	params->dsi.lcm_ext_te_enable=false;
	//for esd protest end by legen
	//params->dsi.word_count=FRAME_WIDTH*3;
	params->dsi.vertical_sync_active=2;
	params->dsi.vertical_backporch=4;
	params->dsi.vertical_frontporch=8;
	params->dsi.vertical_active_line=FRAME_HEIGHT;

	//params->dsi.line_byte=2180;
	params->dsi.horizontal_sync_active=2;
	params->dsi.horizontal_backporch=108;
	params->dsi.horizontal_frontporch=100;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;

	//params->dsi.HS_TRAIL= 7;  // 4.3406868
	//params->dsi.HS_PRPR = 4;

	//params->dsi.CLK_TRAIL= 50;

        //begin modify by kun.zheng for task 8997701 on 2020/04/01
        params->dsi.esd_check_enable = 1;
        params->dsi.customization_esd_check_enable = 1;

        params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
        params->dsi.lcm_esd_check_table[0].count        = 1;
        params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
        //end modify by kun.zheng for task 8997701 on 2020/04/01

	/*params->dsi.lcm_esd_check_table[2].cmd          = 0x0F;
	params->dsi.lcm_esd_check_table[2].count        = 1;
	params->dsi.lcm_esd_check_table[2].para_list[0] = 0xC0;*/

	//params->dsi.fbk_div =30;              // fref=26MHz,  fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)   //32

 	params->dsi.PLL_CLOCK=272;

        params->dsi.ssc_disable = 1;
        //params->dsi.ssc_range = 7;

        #ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	params->round_corner_en = 0;
        params->corner_pattern_width = 720;//Note:这里是屏幕的宽度，不是原始图片宽度
        params->corner_pattern_height = 70;//圆角的高度
        params->corner_pattern_height_bot = 70;
        #endif
}

#if 1
static void push_table(struct LCM_setting_table *table, unsigned int count,
		       unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {
		unsigned cmd;

		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY://modify by yusen.ke.sz for fix lcm sleep out error on 20201012
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}

}
#endif

//begin modify by kun.zheng for task 8998289 on 2020/04/07
static void lcm_init(void)
{
	unsigned char cmd = 0x0;
	unsigned char data = 0xFF;
	int ret=0;

	cmd=0x00;
	data=0x10; //5.6v//vsp 5.8v
	//VPS=0x00;data=0x0A;VSP=5V,
	//         data=0x0E;VSP=5.4V,
	//VNG=0x01;data=0x0A;VNG=-5V,
	//         data=0x0E;VNG=-5.4V,
#ifndef CONFIG_FPGA_EARLY_PORTING
	//enable power
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ZERO);
	set_gpio_lcd_enp(0);
	set_gpio_lcd_enn(0);
	MDELAY(2);

	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
	//LCD_DEBUG("kernel:vsp,vsn on begin\n");
	set_gpio_lcd_enp(1);
	MDELAY(1);

	ret=tps65132_write_bytes(cmd,data);
	if(ret<0)
	LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write success-----\n",cmd);

	MDELAY(5);
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ONE);
	set_gpio_lcd_enn(1);
	MDELAY(1);

	cmd=0x01;
	data=0x10;//5.6v //vsn -5.8v
	//VPS=0x00;data=0x0A;VSP=5V,
	//         data=0x0E;VSP=5.4V,
	//VNG=0x01;data=0x0A;VNG=-5V,
	//         data=0x0E;VNG=-5.4V,

	ret=tps65132_write_bytes(cmd,data);
	if(ret<0)
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write success-----\n",cmd);


#endif
	//LCD_DEBUG("kernel:vsp,vsn on end\n");
	MDELAY(12);

        //#if 0
	//reset high to low to high
	SET_RESET_PIN(1);    //tokyolitetmo_nt36525bh,lcd_reset and tp_reset share a pin,
	MDELAY(10);           //so don't need to set tp_reset status
	SET_RESET_PIN(0);
        //begin add by kun.zheng for task 8998289 on 2020/05/26
        MDELAY(5);
        SET_RESET_PIN(1);
        MDELAY(10);
        SET_RESET_PIN(0);
        //end add by kun.zheng for task 8998289 on 2020/05/26
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(15);
	/*#else
	SET_RESET_PIN(1);

	tpd_gpio_output(GTP_RST_PORT,1);
	MDELAY(5);
	tpd_gpio_output(GTP_RST_PORT,0);
	MDELAY(6);//5

	//MDELAY(1);
	SET_RESET_PIN(0);
	MDELAY(8);//5

	tpd_gpio_output(GTP_RST_PORT,1);

	SET_RESET_PIN(1);
	MDELAY(22);
	#endif */

	push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);  
	//lcm_array_cmdq(&lcm_util,lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table));  
	//init_lcm_registers();
	//LCD_DEBUG("lk:a5a_infini\n");
}
//end modify by kun.zheng for task 8998289 on 2020/04/07

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Sleep Mode On
	{0x28,1,{0x00}},
	{REGFLAG_DELAY,50,{0x00}},//modify by yusen.ke.sz for fix lcm sleep out error on 20201012
	{0x10,1,{0x00}},
	{REGFLAG_DELAY,120,{0x00}},//modify by yusen.ke.sz for fix lcm sleep out error on 20201012
	{REGFLAG_END_OF_TABLE,1,{0x00}},
};
static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	//lcm_array_cmdq(&lcm_util,lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table));  

	//reset
	#if 0
    	SET_RESET_PIN(1);
	MDELAY(2);
	SET_RESET_PIN(0);
	MDELAY(2);
	SET_RESET_PIN(1);
	MDELAY(5);
	#endif

	//LCD_DEBUG("kernel:vsp,vsn off begin\n");
	set_gpio_lcd_enn(1);
	set_gpio_lcd_enn(0);
	MDELAY(5);
	//mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
	//mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
	set_gpio_lcd_enp(1);
	set_gpio_lcd_enp(0);
    	MDELAY(5);
	//LCD_DEBUG("kernel:lcm_suspend\n");

}
static void lcm_resume(void)
{
    lcm_init();
}

static unsigned int lcm_compare_id(void)
{
	//int id_type=0;

	/*mt_set_gpio_mode(GPIO_LCM_ID0,GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCM_ID0, GPIO_DIR_IN);
	//mt_set_gpio_pull_select(GPIO_LCM_ID0,GPIO_PULL_DOWN);
	mt_set_gpio_pull_enable(GPIO_LCM_ID0, GPIO_PULL_DISABLE);// def 0

	mt_set_gpio_mode(GPIO_LCM_ID1,GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCM_ID1, GPIO_DIR_IN);
	//mt_set_gpio_pull_select(GPIO_LCM_ID1,GPIO_PULL_DOWN);
	mt_set_gpio_pull_enable(GPIO_LCM_ID1, GPIO_PULL_DISABLE);//def 0

	MDELAY(10);
	id_type = mt_get_gpio_in(GPIO_LCM_ID1)<<1 | mt_get_gpio_in(GPIO_LCM_ID0);
	//if (id_type==1)//ID pin=01;*/
    		return 1;
	//else
		//return 0;
}

struct LCM_DRIVER tokyolitetmo_nt36525bh_hd_dsi_vdo_lcm_drv =
{
    .name           = "tokyolitetmo_nt36525bh_hd_dsi_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    //.init_power   = lcm_init_power,

};
#endif

