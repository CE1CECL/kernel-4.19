/*History modified by xiongbo.huang for least_resume time (8423451) on 2019/10/11*/
#ifndef CONFIG_MTK_LCM_DEVICE_TREE_SUPPORT
#ifndef BUILD_LK
    #include <linux/string.h>
    #include <linux/kernel.h>
#else
#include <string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#elif defined(BUILD_UBOOT)
    #include <asm/arch/mt_gpio.h>
#else
    //#include <mt-plat/mt_gpio.h>
    //#include <mach/gpio_const.h>
#endif

//min.luo defect 8249117:[SWD_TEST]ctp resume faster 20190813
//extern void lcd_queue_load_tp_fw(void);

#ifndef  GTP_RST_PORT
#define GTP_RST_PORT    0
#endif

#ifdef BUILD_LK
#define LCD_DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define LCD_DEBUG(fmt, args...) printk(fmt, ##args)
#endif

extern int tps65132_write_bytes(unsigned char addr, unsigned char value);


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1440)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef GPIO_LCD_BIAS_ENP_PIN
#define GPIO_LCD_BIAS_ENP_PIN        			(GPIO173 | 0x80000000)
#endif
#ifndef GPIO_LCD_BIAS_ENN_PIN
#define GPIO_LCD_BIAS_ENN_PIN        			(GPIO171 | 0x80000000)
#endif
#define GPIO_LCD_RST_PIN               		(GPIO45 | 0x80000000)

//min.luo defect 8597127:[SWD_TEST]optimize lcd cmd driver 20191113  start
#define REGFLAG_END_OF_TABLE		(0xFFFD)
#define REGFLAG_DELAY		0xFFFC
#define REGFLAG_UDELAY	0xFFFB
//min.luo defect 8597127:[SWD_TEST]optimize lcd cmd driver 20191113  end
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
//begin modified by xiongbo.huang for task 8670109 on 2019/12/03
static struct LCM_setting_table  lcm_initialization_setting_ty[] = {
//======================Initial Code
{0xB9,3,{0xF1,0x12,0x83}},

{0xB1,5,{0x85,0x00,0x00,0xDA,0x01}},

{0xB2,3,{0xF0,0x12,0xD0}},

{0xB3,10,{0x10,0x10,0x28,0x28,0x03,0xFF,0x00,0x00,0x00,0x00}},

{0xB4,1,{0x00}},

{0xB5,2,{0x09,0x09}},

//{0xB6,2,{0x1E,0x1E}},   //VCOM

{0xB8,1,{0x75}},

{0xBA,27,{0x33,0x81,0x05,0xF9,0x0E,0x0E,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x25,0x00,0x90,0x0A,0x00,0x00,0x01,0x4F,0xC1,0x03,0x02,0x37}},

{0xBC,1,{0x4F}},

{0xBF,3,{0x02,0x11,0x00}},

{0xC0,9,{0x73,0x73,0x50,0x50,0x00,0x00,0x08,0x70,0x00}},

{0xC1,12,{0x25,0x00,0x32,0x32,0xBB,0xE1,0xFF,0xFF,0xAF,0xAF,0x7F,0x7F}},

{0xC6,6,{0x82,0x00,0xBF,0xFF,0x00,0x7F}},

{0xCA,4,{0x00,0x01,0x0F,0x0F}},

{0xCC,1,{0x0B}},

{0xE0,34,{0x00,0x23,0x27,0x26,0x38,0x3F,0x54,0x3F,0x08,0x0D,0x0E,0x13,0x15,0x13,0x14,0x10,0x16,0x00,0x23,0x27,0x26,0x38,0x3F,0x54,0x3F,0x08,0x0D,0x0E,0x13,0x15,0x13,0x14,0x10,0x16}},

{0xE3,14,{0x07,0x07,0x0B,0x0B,0x03,0x0B,0x00,0x00,0x00,0x00,0xFF,0x80,0xC0,0x10}},

{0xE9,63,{0xC8,0x10,0x0A,0x05,0xA5,0x80,0x81,0x12,0x31,0x23,0x6F,0x88,0x80,0x2D,0x47,0x0A,0x00,0x00,0xC2,0x00,0x00,0x00,0x00,0x00,0xC2,0x00,0x00,0x00,0x85,0x88,0xF8,0x51,0x18,0x87,0x58,0x83,0x88,0x88,0x88,0x84,0x88,0xF8,0x40,0x08,0x86,0x48,0x82,0x88,0x88,0x88,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

{0xEA,63,{0x00,0x1A,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x84,0x88,0x8F,0x04,0x28,0x84,0x68,0x80,0x88,0x88,0x88,0x85,0x88,0x8F,0x15,0x38,0x85,0x78,0x81,0x88,0x88,0x88,0x23,0x00,0x00,0x00,0xE8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x0F,0x50,0x00,0x40,0x80,0x2D,0xC0,0x00,0x00,0x00,0x30,0x00}},

{0xEF,3,{0xFF,0xFF,0x01}},

{0xC7,6,{0x10, 0x00, 0x0A, 0x00, 0x00, 0x02}},/////////////////////TE
         
{0x11,1,{0x00}},
{REGFLAG_DELAY, 120, {}},  
    
{0x29,1,{0x00}},
{REGFLAG_END_OF_TABLE,1,{0x00}}, 
};
//end modified by xiongbo.huang for task 8670109 on 2019/12/03
// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const struct LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(struct LCM_UTIL_FUNCS));
}

static void lcm_get_params(struct LCM_PARAMS *params)
{
	//LCD_DEBUG("\t\t nt36525 [lcm_get_params]\n");

	memset(params, 0, sizeof(struct LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

//Begin add by xiongbo.huang for defect 8423473 on 2019/10/23
        params->physical_width = 67;
        params->physical_height = 142;
//End add by xiongbo.huang for defect 8423473 on 2019/10/23

	// enable tearing-free
	//params->dbi.te_mode 			= LCM_DBI_TE_MODE_VSYNC_ONLY;
	//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;


	params->dsi.mode   = SYNC_PULSE_VDO_MODE;

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
//Begin add by xiongbo.huang for task 8425305 on 2019/11/07
        params->dsi.vertical_sync_active=4;
	params->dsi.vertical_backporch=7;
	params->dsi.vertical_frontporch=16;
	params->dsi.vertical_active_line=FRAME_HEIGHT;
	//params->dsi.line_byte=2180;
	params->dsi.horizontal_sync_active=20;
    params->dsi.horizontal_backporch=60;
	params->dsi.horizontal_frontporch=60;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;


//Begin modified by xiongbo.huang for task 8418608 on 2019/10/11
	params->dsi.clk_lp_per_line_enable = 1;//ADD++
		
	params->dsi.cont_clock  = 0; //ADD++
	params->dsi.esd_check_enable = 1;

	params->dsi.customization_esd_check_enable = 1;

	params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
	params->dsi.lcm_esd_check_table[0].count        = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
	
	
//End modified by xiongbo.huang for task 8408608 on 2019/10/11
	/*params->dsi.lcm_esd_check_table[2].cmd          = 0x0F;
	params->dsi.lcm_esd_check_table[2].count        = 1;
	params->dsi.lcm_esd_check_table[2].para_list[0] = 0xC0;*/

	//params->dsi.fbk_div =30;              // fref=26MHz,  fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)   //32
        //params->dsi.ssc_range = 7;/
    params->dsi.PLL_CLOCK=241;//215
    params->dsi.ssc_disable = 1;
//begin 20180416 liujunting add for round corner
        #ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
//begin modified by xiongbo.huang for task 8188257
	params->round_corner_en = 1;//begin modified by xiongbo.huang for task 8611118 on 2019/11/20
//end modified by xiongbo.huang for task 8188257
        params->corner_pattern_width = 720;//Note:这里是屏幕的宽度，不是原始图片宽\E5\BA?
        params->corner_pattern_height = 25;//圆角的高\E5\BA?//modified by xiongbo.huang for task 8347218 on 2019/09/20

	params->corner_pattern_height_bot = 25;//modified by xiongbo.huang for task 8347218 on 2019/09/20
        #endif
//end 20180416 liujunting add for round corner
}

static void push_table(struct LCM_setting_table *table, unsigned int count,
		       unsigned char force_update)
{
	unsigned int i;
	for (i = 0; i < count; i++) {
		unsigned cmd;

		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY:
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}

}

static void lcm_init(void)
{
	unsigned char cmd = 0x0;
	unsigned char data = 0xFF;
	int ret=0;
	SET_RESET_PIN(0);
	MDELAY(5);	
	SET_RESET_PIN(1);
	MDELAY(10);
	cmd=0x00;
	data=0x10; //vsp 5.6v
	//VPS=0x00;data=0x0A;VSP=5V,
	//         data=0x0E;VSP=5.4V,
	//	   data=0x12;VSP=5.8V,
	//VNG=0x01;data=0x0A;VNG=-5V,
	//         data=0x0E;VNG=-5.4V,
	//	   data=0x12;VSP=-5.8V,
#ifndef CONFIG_FPGA_EARLY_PORTING
	//enable power
	set_gpio_lcd_enp(0);
	set_gpio_lcd_enn(0);
	MDELAY(5);

	set_gpio_lcd_enp(1);
	MDELAY(5);

	ret=tps65132_write_bytes(cmd,data);
	if(ret<0)
	LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write success-----\n",cmd);

	MDELAY(5);
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ONE);
	set_gpio_lcd_enn(1);
	MDELAY(5);
	cmd=0x01;
	data=0x10; //vsp 5.6v
	//VPS=0x00;data=0x0A;VSP=5V,
	//         data=0x0E;VSP=5.4V,
	//	   data=0x12;VSP=5.8V,
	//VNG=0x01;data=0x0A;VNG=-5V,
	//         data=0x0E;VNG=-5.4V,
	//	   data=0x12;VSP=-5.8V,

	ret=tps65132_write_bytes(cmd,data);
	if(ret<0)
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write success-----\n",cmd);


#endif
	//LCD_DEBUG("kernel:vsp,vsn on end\n");
	MDELAY(5);
	//reset high to low to high
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(120);

	//lcd_queue_load_tp_fw();
	//MDELAY(22);
	push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);
}
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    // Sleep Mode On
//	{0x28,0,{0x00}},
//	{REGFLAG_DELAY,150,{0x00}},
	{0x10,0,{0x00}},
	{REGFLAG_DELAY,150,{0x00}},
	{0xE9,1,{0xC1}},	
	{0xC6,6,{0x01,0x00,0xFF,0xFF,0x00,0xFF}},		
	{REGFLAG_END_OF_TABLE,1,{0x00}},
};
static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	LCD_DEBUG("kernel:vsp,vsn off begin\n");
	set_gpio_lcd_enn(1);
	set_gpio_lcd_enn(0);
	MDELAY(10);
	set_gpio_lcd_enp(1);
	set_gpio_lcd_enp(0);
    MDELAY(20);
	LCD_DEBUG("kernel:lcm_suspend\n");
	SET_RESET_PIN(0);
	MDELAY(5);

}
static void lcm_resume(void)
{
    lcm_init();
    //LCD_DEBUG("lk:morgan_nt36525_lcm_resume\n");

}

static unsigned int lcm_compare_id(void)
{
	return 1;
}
struct LCM_DRIVER bangkok_tdt_st7703_hd_dsi_vdo_lcm_drv =
{
    .name           = "bangkok_tdt_st7703_hd_dsi_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
};
#endif

