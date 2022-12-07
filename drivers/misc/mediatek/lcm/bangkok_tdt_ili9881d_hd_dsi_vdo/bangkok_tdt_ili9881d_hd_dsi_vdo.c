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
//GIP timing
{0xFF,3,{0x98,0x81,0x01}},                                   
{0x91,1,{0x00}},                    
{0x92,1,{0x00}},                    
{0x93,1,{0x73}},                    
{0x94,1,{0x73}},                    
{0x95,1,{0x00}},                    
{0x96,1,{0x06}},                    
{0x97,1,{0x02}},                    
{0x98,1,{0x00}},        

{0x09,1,{0x01}},
{0x0A,1,{0x01}}, 
{0x0B,1,{0x01}},
{0x0C,1,{0x01}},
{0x0D,1,{0x01}},
{0x0E,1,{0x01}},
{0x0F,1,{0x00}},
{0x10,1,{0x00}},
{0x11,1,{0x00}},
{0x12,1,{0x00}},
{0x13,1,{0x01}},
{0x14,1,{0x00}},
{0x15,1,{0x08}},
{0x16,1,{0x08}},
{0x17,1,{0x00}},
{0x18,1,{0x08}},
{0x19,1,{0x00}},
{0x1A,1,{0x00}},
{0x1B,1,{0x00}},         
{0x1C,1,{0x00}},         
{0x1D,1,{0x00}},
{0x1E,1,{0xC0}},
{0x1f,1,{0x00}},
{0x20,1,{0x03}},
{0x21,1,{0x04}},
{0x22,1,{0x00}},
{0x23,1,{0x00}},
{0x24,1,{0x00}},
{0x25,1,{0x00}},
{0x26,1,{0x00}},
{0x27,1,{0x00}},
{0x28,1,{0x33}},
{0x29,1,{0x02}},
{0x2A,1,{0x00}},
{0x2B,1,{0x00}},
{0x2C,1,{0x00}},
{0x2D,1,{0x00}},
{0x2E,1,{0x00}},
{0x2F,1,{0x00}},           
{0x30,1,{0x00}},
{0x31,1,{0x00}},
{0x32,1,{0x00}},
{0x33,1,{0x00}},
{0x34,1,{0x03}},
{0x35,1,{0x00}},
{0x36,1,{0x03}},
{0x37,1,{0x00}},
{0x38,1,{0x00}},

{0x39,1,{0x07}},                    
{0x3a,1,{0x00}},                    
{0x3b,1,{0x00}},                    
{0x3c,1,{0x00}},                                                
{0x40,1,{0x03}},                    
{0x41,1,{0x20}},                    
{0x42,1,{0x00}},                    
{0x43,1,{0x00}},                    
{0x44,1,{0x03}},                    
{0x45,1,{0x00}},                    
{0x46,1,{0x01}},                    
{0x47,1,{0x08}},                    
{0x48,1,{0x00}},                    
{0x49,1,{0x00}},                    
{0x4a,1,{0x00}},                    
{0x4b,1,{0x00}},                 

{0x4c,1,{0x86}},                    
{0x4d,1,{0xA8}},                    
{0x4e,1,{0x64}},                    
{0x4f,1,{0x22}},                    
{0x50,1,{0x22}},                    
{0x51,1,{0x22}},                    
{0x52,1,{0x22}},                    
{0x53,1,{0x22}},                    
{0x54,1,{0x22}},                    
{0x55,1,{0xAC}},                    
{0x56,1,{0x22}},                    
                             
{0x57,1,{0x97}},                    
{0x58,1,{0xB9}},                    
{0x59,1,{0x75}},                    
{0x5a,1,{0x22}},                    
{0x5b,1,{0x22}},                    
{0x5c,1,{0x22}},                    
{0x5d,1,{0x22}},                    
{0x5e,1,{0x22}},                    
{0x5f,1,{0x22}},                    
{0x60,1,{0xBD}},                    
{0x61,1,{0x22}},                    
         
{0x62,1,{0x06}},                    

{0x63,1,{0x0D}},    
{0x64,1,{0x0B}},                    
{0x65,1,{0x5B}},                    
{0x66,1,{0x59}},                    
{0x67,1,{0x57}},                    
{0x68,1,{0x55}},                    
{0x69,1,{0x02}},                    
{0x6a,1,{0x02}},    //92                  
{0x6b,1,{0x02}},                    
{0x6c,1,{0x02}},                    
{0x6d,1,{0x02}},                    
{0x6e,1,{0x02}},                    
{0x6f,1,{0x02}},                    
{0x70,1,{0x02}},                    
{0x71,1,{0x02}},                    
{0x72,1,{0x02}},                    
{0x73,1,{0x02}},                    
{0x74,1,{0x02}},                    
{0x75,1,{0x07}},                    
{0x76,1,{0x09}},                    
{0x77,1,{0x02}},                    
{0x78,1,{0x02}},                    

{0x79,1,{0x0C}},                    
{0x7a,1,{0x0A}},                    
{0x7b,1,{0x5A}},                    
{0x7c,1,{0x58}},                    
{0x7d,1,{0x56}},                    
{0x7e,1,{0x54}},                    
{0x7f,1,{0x02}},                    
{0x80,1,{0x02}},                    
{0x81,1,{0x02}},                    
{0x82,1,{0x02}},                    
{0x83,1,{0x02}},                    
{0x84,1,{0x02}},                    
{0x85,1,{0x02}},                    
{0x86,1,{0x02}},                    
{0x87,1,{0x02}},                    
{0x88,1,{0x02}},                    
{0x89,1,{0x02}},                    
{0x8a,1,{0x02}},                    
{0x8b,1,{0x06}},                    
{0x8c,1,{0x08}},                    
{0x8d,1,{0x02}},                    
{0x8e,1,{0x02}},                    

{0xa0,1,{0x38}},                    
{0xa1,1,{0x00}},                    
{0xa2,1,{0x00}},                    
{0xa3,1,{0x00}},                    
{0xa4,1,{0x00}},                    
{0xa5,1,{0x38}},                    
{0xa6,1,{0x08}},                    
{0xa7,1,{0x00}},                    
{0xa8,1,{0x00}},                    
{0xa9,1,{0x00}},                    
{0xaa,1,{0x00}},                    
{0xab,1,{0x00}},                    
{0xac,1,{0x00}},                    
{0xad,1,{0x00}},                    
{0xae,1,{0xff}},                    
{0xaf,1,{0x00}},                    
{0xb0,1,{0x00}},    

{0xF1,1,{0xF0}},   //1440 

{0xFF,3,{0x98,0x81,0x02}},						
{0xA0,20,{0x00,0x21,0x35,0x19,0x1F,0x34,0x28,0x26,0xAF,0x1B,0x27,0x8D,0x1C,0x1D,0x53,0x27,0x2B,0x52,0x5E,0x26}},						
{0xC0,20,{0x00,0x21,0x35,0x19,0x1F,0x34,0x28,0x26,0xAF,0x1B,0x27,0x8D,0x1C,0x1D,0x53,0x27,0x2B,0x52,0x5E,0x26}},						  

{0x18,1,{0xF4}},        // SH on , default E4 

{0xFF,3,{0x98,0x81,0x05}},     
{0x22,1,{0x3A}},            

{0xFF,3,{0x98,0x81,0x04}},

{0x00,1,{0x00}},     // 3lan  

{0x5D,1,{0x73}},   //  4.8V 5B     //VREG1 4.5V 
{0x5E,1,{0x73}},   //5B     //VREG2 -4.5V
{0x60,1,{0x4D}},     //VCM1 
{0x62,1,{0x52}},     //VCM2  
{0x82,1,{0x38}},     //VREF_VGH_MOD_CLPSEL 15V 
{0x84,1,{0x38}},     //VREF_VGH_DC 15V     
{0x86,1,{0x20}},     //VREF_VGL_CLPSEL -11V       

{0x5B,1,{0x33}},     //  vcore_sel Voltage
{0x6C,1,{0x10}},     //  vcore bias L
{0x77,1,{0x03}},     //  vcore_sel Voltage
{0x7B,1,{0x02}},     //  vcore bias R
                  
{0xFF,3,{0x98,0x81,0x00}},     
{0x35,1,{0x00}},           
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
	params->dsi.LANE_NUM				= LCM_THREE_LANE;//LCM_FOUR_LANE;
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
        params->dsi.vertical_sync_active=6;
	params->dsi.vertical_backporch=14;//23;//15;
	params->dsi.vertical_frontporch=15;//25; //16;
	params->dsi.vertical_active_line=FRAME_HEIGHT;
	//params->dsi.line_byte=2180;
	params->dsi.horizontal_sync_active=10;//10;//8;
    params->dsi.horizontal_backporch=80;//62;//48;
	params->dsi.horizontal_frontporch=90;//72;//52;
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
    params->dsi.PLL_CLOCK=334;
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
struct LCM_DRIVER bangkok_tdt_ili9881d_hd_dsi_vdo_lcm_drv =
{
    .name           = "bangkok_tdt_ili9881d_hd_dsi_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
};
#endif

