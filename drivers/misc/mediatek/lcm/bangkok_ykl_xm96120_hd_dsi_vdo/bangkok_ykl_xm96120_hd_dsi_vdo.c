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
// Set XM Command Password 1
{0x00,1,{0x00}},
{0xFF,3,{0x21,0x60,0x01}},

//Set XM Command Password 2
{0x00,1,{0x80}},
{0xFF,2,{0x21,0x60}},

//VCOM disable time
{0x00,1,{0x91}},
{0xA4,1,{0x13}},

//IP ID off
{0x00,1,{0x90}},
{0xCF,1,{0x00}},

//TE follow cmd1
{0x00,1,{0xE0}},
{0xB2,1,{0x81}},

{0x00,1,{0x80}},
{0xEF,1,{0x0F}},

//resolution sel 720
{0x00,1,{0x80}},
{0xA4,1,{0x40}},

//SW panel size 720x1440
{0x00,1,{0xA1}}, 
{0xC0,4,{0x02,0xD0,0x05,0xa0}},

//SW Panel Size Enable
{0x00,1,{0xa6}},
{0xc0,1,{0x90}},

//tcon rtn/vfp_h
{0x00,1,{0x81}},
{0xB2,2,{0x81,0x00}},

//vbp
{0x00,1,{0x85}},
{0xB2,1,{0x10}},

//shift1
{0x00,1,{0xC4}},
{0xB2,1,{0x50}},

//vdo sync
{0x00,1,{0x82}},
{0xB3,1,{0x10}},

//p750 opt
{0x00,1,{0xa7}},
{0xc0,1,{0x00}},

//IM
{0x00,1,{0x81}},
{0xc0,1,{0x02}},

//BGR
{0x00,1,{0x82}},
{0xC0,1,{0x29}},

//sd_en_ic off
{0x00,1,{0x9c}},
{0xa6,1,{0x00}},

//speed up off
{0x00,1,{0x96}},
{0xa3,2,{0x0b,0x01}},

//sdpl_en
{0x00,1,{0x9b}},
{0xa6,1,{0x15}},

//Blanking pull low
{0x00,1,{0x80}},
{0xa5,1,{0x05}},

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Tuning Setting

//GVDDP / GVDDN
{0x00,1,{0xA0}},
{0xA4,2,{0xB6,0xB6}}, //GVDDP = 4.8V ; GVDDN = -4.8V

//VCOM
{0x00,1,{0xB0}},
{0xA4,3,{0x00,0x2D,0x2D}}, //VCOM = -0.68V

//VGH / VGL 
{0x00,1,{0x90}},
{0xAB,3,{0xA8,0xD0,0x51}}, //VGH = 15V ; VGL = -11V ;pump mode = b001

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																				
//ivo init
//clk
{0x00,1,{0x80}},
{0xb4,10,{0x00,0x00,0x30,0x00,0x38,0x06,0x05,0xa1,0x00,0x00}},
{0x00,1,{0x90}},
{0xb4,10,{0x00,0x00,0x30,0x00,0x38,0x05,0x05,0xa2,0x00,0x00}},
{0x00,1,{0xa0}},
{0xb4,10,{0x00,0x00,0x30,0x00,0x38,0x04,0x05,0xa3,0x00,0x00}},
{0x00,1,{0xb0}},
{0xb4,10,{0x00,0x00,0x30,0x00,0x38,0x03,0x05,0xa4,0x00,0x00}},
{0x00,1,{0x80}},
{0xb5,10,{0x00,0x00,0x30,0x00,0x38,0x02,0x05,0xa5,0x00,0x00}},
{0x00,1,{0x90}},
{0xb5,10,{0x00,0x00,0x30,0x00,0x38,0x01,0x05,0xa6,0x00,0x00}},
{0x00,1,{0xa0}},
{0xb5,10,{0x00,0x00,0x30,0x00,0x30,0x01,0x05,0xa7,0x00,0x00}},
{0x00,1,{0xb0}},
{0xb5,10,{0x00,0x00,0x30,0x00,0x30,0x02,0x05,0xa8,0x00,0x00}},

//vst
{0x00,1,{0x80}},
{0xb8,12,{0x88,0x03,0x03,0x87,0x03,0x03,0x86,0x03,0x03,0x85,0x03,0x03}}, 

//vend
{0x00,1,{0x90}},
{0xb8,12,{0x35,0x9d,0x03,0x35,0x9e,0x03,0x35,0x9f,0x03,0x35,0xa0,0x03}},
{0x00,1,{0xd0}},
{0xb8,4,{0x81,0x00,0x01,0x00}},

//frame clk
{0x00,1,{0xa0}},
{0xb8,6,{0x09,0x1f,0x00,0x09,0x1f,0x00}}, 

//u2d 
{0x00,1,{0x80}},
{0xbc,16,{0x1c,0x1a,0x0c,0x0a,0x08,0x06,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 
{0x00,1,{0x90}},
{0xbc,16,{0x00,0x00,0x16,0x18,0x00,0x00,0x1b,0x19,0x0b,0x09,0x07,0x05,0x01,0x01,0x01,0x00}}, 
{0x00,1,{0xa0}},
{0xbc,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x15,0x17,0x00,0x00}}, 

//d2u 
{0x00,1,{0xb0}},
{0xbc,16,{0x15,0x17,0x09,0x0b,0x05,0x07,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 
{0x00,1,{0xc0}},
{0xbc,16,{0x00,0x00,0x1b,0x19,0x00,0x00,0x16,0x18,0x0a,0x0c,0x06,0x08,0x01,0x01,0x01,0x00}},
{0x00,1,{0xd0}},
{0xbc,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x1a,0x00,0x00,0x00}},

//enmode
{0x00,1,{0x80}},
{0xb9,16,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 
{0x00,1,{0x90}},
{0xb9,16,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 
{0x00,1,{0xa0}},
{0xb9,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 

{0x00,1,{0xb0}},
{0xb9,16,{0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, 
{0x00,1,{0xc0}},
{0xb9,16,{0x00,0x00,0x05,0x05,0x00,0x00,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00}}, 
{0x00,1,{0xd0}},
{0xb9,12,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x00,0x00}}, 

//gip lvd
{0x00,1,{0x80}},
{0xba,11,{0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa}},

//gm2.2
{0x00,1,{0x80}},  //
{0xD4,38,{0x02,0x06,0x0D,0x14,0x1A,0x1F,0x23,0x27,0x2B,0x36,0x3F,0x4D,0x57,0x66,0x72,0x72,0x7F,0x8F,0x9A,0xA9,0xB4,0xC2,0xC7,0xCC,0xD2,0xD8,0xE0,0xE9,0xF6,0xFF,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00}},
{0x00,1,{0x80}},
{0xD5,38,{0x02,0x06,0x0D,0x14,0x1A,0x1F,0x23,0x27,0x2B,0x36,0x3F,0x4D,0x57,0x66,0x72,0x72,0x7F,0x8F,0x9A,0xA9,0xB4,0xC2,0xC7,0xCC,0xD2,0xD8,0xE0,0xE9,0xF6,0xFF,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00}},
{0x00,1,{0x80}},
{0xD6,38,{0x02,0x06,0x0D,0x14,0x1A,0x1F,0x23,0x27,0x2B,0x36,0x3F,0x4D,0x57,0x66,0x72,0x72,0x7F,0x8F,0x9A,0xA9,0xB4,0xC2,0xC7,0xCC,0xD2,0xD8,0xE0,0xE9,0xF6,0xFF,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00}},
{0x00,1,{0x80}},
{0xD7,38,{0x02,0x06,0x0D,0x14,0x1A,0x1F,0x23,0x27,0x2B,0x36,0x3F,0x4D,0x57,0x66,0x72,0x72,0x7F,0x8F,0x9A,0xA9,0xB4,0xC2,0xC7,0xCC,0xD2,0xD8,0xE0,0xE9,0xF6,0xFF,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00}},
{0x00,1,{0x80}},
{0xD8,38,{0x02,0x06,0x0D,0x14,0x1A,0x1F,0x23,0x27,0x2B,0x36,0x3F,0x4D,0x57,0x66,0x72,0x72,0x7F,0x8F,0x9A,0xA9,0xB4,0xC2,0xC7,0xCC,0xD2,0xD8,0xE0,0xE9,0xF6,0xFF,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00}},
{0x00,1,{0x80}},
{0xD9,38,{0x02,0x06,0x0D,0x14,0x1A,0x1F,0x23,0x27,0x2B,0x36,0x3F,0x4D,0x57,0x66,0x72,0x72,0x7F,0x8F,0x9A,0xA9,0xB4,0xC2,0xC7,0xCC,0xD2,0xD8,0xE0,0xE9,0xF6,0xFF,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00}},

//4LANE
{0x00,1,{0x80}},
{0xE1,1,{0xB0}},          
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
	params->dsi.vertical_backporch=21;
	params->dsi.vertical_frontporch=17;
	params->dsi.vertical_active_line=FRAME_HEIGHT;
	//params->dsi.line_byte=2180;
	params->dsi.horizontal_sync_active=10;
    params->dsi.horizontal_backporch=50;
	params->dsi.horizontal_frontporch=50;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;


//Begin modified by xiongbo.huang for task 8418608 on 2019/10/11
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
        //params->dsi.ssc_range = 7;
    params->dsi.PLL_CLOCK=236;
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

	cmd=0x00;
	data=0x12; //vsp 5.8v
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
	data=0x12; //vsp 5.8v
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
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(15);

	//lcd_queue_load_tp_fw();
	//MDELAY(22);
	push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);
}
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    // Sleep Mode On
	{0x28,0,{0x00}},
	{REGFLAG_DELAY,50,{0x00}},
	{0x10,0,{0x00}},
	{REGFLAG_DELAY,120,{0x00}},
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
    	MDELAY(5);
	LCD_DEBUG("kernel:lcm_suspend\n");

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
struct LCM_DRIVER bangkok_ykl_xm96120_hd_dsi_vdo_lcm_drv =
{
    .name           = "bangkok_ykl_xm96120_hd_dsi_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
};
#endif

