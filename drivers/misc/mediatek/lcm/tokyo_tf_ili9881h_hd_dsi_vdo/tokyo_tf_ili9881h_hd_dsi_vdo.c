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

#include "../../pmic/include/mt6357/mtk_pmic_api.h"

//min.luo defect 8249117:[SWD_TEST]ctp resume faster 20190813
extern void lcd_queue_load_tp_fw(void);

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

//min.luo task 8170583:[Power consumption]modify lcd density 20190724
//#undef LCM_DENSITY
#define LCM_DENSITY (320)

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
#ifndef GPIO_LCM_ID0
#define GPIO_LCM_ID0				(GPIO23 | 0x80000000)
#endif
#ifndef GPIO_LCM_ID1
#define GPIO_LCM_ID1				(GPIO16 | 0x80000000)
#endif

#define REGFLAG_END_OF_TABLE		(0xFD)
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
	unsigned char cmd;
	unsigned char count;
	unsigned char para_list[64];
};
//update initial param for IC nt36525 0.01
/*static void init_lcm_registers(void)
{
	unsigned int data_array[16];
	
	data_array[0] = 0x00023902;
	data_array[1] = 0x000020FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A905;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006907;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000CB08;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000870E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000550F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000001F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A969;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006489;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000648A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000648B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000648C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000EB95;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000EB96;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000023FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A212;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008115;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000A16;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000024FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002001;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000502;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000503;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009E04;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009E05;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009F06;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009F07;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000C08;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000309;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000D0A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000E0B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000F0C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000100D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000110E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000120F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001310;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000411;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000412;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002013;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002014;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002015;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002016;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002017;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000518;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000519;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009E1A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009E1B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009F1C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009F1D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000C1E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000031F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000D20;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000E21;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000F22;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001023;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001124;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001225;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001326;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000427;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000428;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002029;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000202A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000202B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000C2F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00004030;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00004033;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000C34;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00007737;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A3A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000953B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000923D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000154D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000264E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000374F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00004850;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008451;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00007352;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006253;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00005154;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008655;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00007856;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A5A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000955B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008F5C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000A5D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000105E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008060;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006861;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001164;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001185;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B692;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000893;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000694;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000000AB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000000AD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000005B0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B0B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000025FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000820A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C10B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000010C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008217;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000618;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000F19;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A1F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009520;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000523;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A924;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A26;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009527;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000052A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A92B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000802F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001040;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008041;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A642;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009543;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000546;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A947;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000954C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000954E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A64F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009550;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000553;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A954;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000555;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A956;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000805A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000805B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A5D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000955E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A5F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009560;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A61;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009562;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000565;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A966;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000066D5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00004204;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000FF06;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000B0C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000010D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000020E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000060F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000710;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002813;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008014;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008116;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001A19;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000D1A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000121B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000821C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B51E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B51F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000024;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000042F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B530;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001131;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001132;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000434;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B535;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008136;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00006737;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00001138;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000103F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B540;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D658;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D659;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D65A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000AD5B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000005C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000265D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000105E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A63;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009564;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A65;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009566;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A67;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009568;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000006B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000006D;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000570;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D271;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A73;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009574;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000577;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D278;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A7A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000957B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000057E;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D27F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A82;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009583;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A84;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009585;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009A86;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009587;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000058A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A98B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000008F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000090;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000592;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000F093;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000D99;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000369A;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000C9B;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009E9C;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000027FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000013;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00005514;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000A44;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008E45;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00003EA9;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002CAA;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000036AB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000032AC;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00003CAD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002EAE;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000040AF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00003AB0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000030B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000010FF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000001FB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000003BA;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000035;
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00023902;
	data_array[1] = 0x00000336;
	dsi_set_cmdq(data_array, 2, 1);	
	
	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);

	MDELAY(120);//There is needed 120msec after Sleep Out -command

	data_array[0] = 0x00290500;
	dsi_set_cmdq(data_array, 1, 1);
	
	MDELAY(20);
}*/

static struct LCM_setting_table  lcm_initialization_setting_ty[] = {
{0XFF,3,{0x98,0x81,0x01}},
{0X00,1,{0x46}},
{0X01,1,{0x16}},
{0X02,1,{0x10}},    //10
{0X03,1,{0x10}},

{0X08,1,{0x80}},  
{0X09,1,{0x12}},   //DUMMT CK
{0X0a,1,{0x71}},
{0X0b,1,{0x00}}, //clk keep 10 off 00
{0X14,1,{0x8A}}, //KEEP
{0X15,1,{0x8A}},//KEEP
{0X0c,1,{0x10}}, //01
{0X0d,1,{0x10}}, //01
{0X0e,1,{0x00}}, 
{0X0f,1,{0x00}},

{0X10,1,{0x01}},
{0X11,1,{0x01}},
{0X12,1,{0x01}},


{0X24,1,{0x00}},  //01 
{0X25,1,{0x09}},  
{0X26,1,{0x10}},
{0X27,1,{0x10}},

{0X31,1,{0x21}},  //STV_C
{0X32,1,{0x07}},  
{0X33,1,{0x01}},  
{0X34,1,{0x00}},  
{0X35,1,{0x02}}, 
{0X36,1,{0x07}},
{0X37,1,{0x07}},
{0X38,1,{0x07}},
{0X39,1,{0x07}},  
{0X3a,1,{0x17}},//CLK8
{0X3b,1,{0x15}},  //CLK6
{0X3c,1,{0x07}}, 
{0X3d,1,{0x07}},  
{0X3e,1,{0x13}},  //CLK4
{0X3f,1,{0x11}},  //CLK2
{0X40,1,{0x09}},  //STV_A  
{0X41,1,{0x07}},
{0X42,1,{0x07}},
{0X43,1,{0x07}}, 
{0X44,1,{0x07}},  
{0X45,1,{0x07}},  
{0X46,1,{0x07}},

{0X47,1,{0x20}},  //STV_C
{0X48,1,{0x07}},  
{0X49,1,{0x01}},  
{0X4a,1,{0x00}},
{0X4b,1,{0x02}},
{0X4c,1,{0x07}},
{0X4d,1,{0x07}},  
{0X4e,1,{0x07}},
{0X4f,1,{0x07}},
{0X50,1,{0x16}},  //CLK7
{0X51,1,{0x14}},  //CLK5
{0X52,1,{0x07}},
{0X53,1,{0x07}},  
{0X54,1,{0x12}},  //CLK3
{0X55,1,{0x10}},  //CLK1
{0X56,1,{0x08}},  //STV_A
{0X57,1,{0x07}},
{0X58,1,{0x07}},
{0X59,1,{0x07}},
{0X5a,1,{0x07}},
{0X5b,1,{0x07}},
{0X5c,1,{0x07}},

{0X61,1,{0x08}},
{0X62,1,{0x07}},
{0X63,1,{0x01}},
{0X64,1,{0x00}},
{0X65,1,{0x02}},
{0X66,1,{0x07}},
{0X67,1,{0x07}},
{0X68,1,{0x07}},
{0X69,1,{0x07}}, 
{0X6a,1,{0x10}},
{0X6b,1,{0x12}},
{0X6c,1,{0x07}},
{0X6d,1,{0x07}},
{0X6e,1,{0x14}},
{0X6f,1,{0x16}},
{0X70,1,{0x20}}, 
{0X71,1,{0x07}},  
{0X72,1,{0x07}},
{0X73,1,{0x07}},
{0X74,1,{0x07}}, 
{0X75,1,{0x07}},  
{0X76,1,{0x07}},

{0X77,1,{0x09}},
{0X78,1,{0x07}},  
{0X79,1,{0x01}},  
{0X7a,1,{0x00}},
{0X7b,1,{0x02}},
{0X7c,1,{0x07}},
{0X7d,1,{0x07}},  
{0X7e,1,{0x07}},
{0X7f,1,{0x07}},
{0X80,1,{0x11}},  
{0X81,1,{0x13}},
{0X82,1,{0x07}},
{0X83,1,{0x07}},  
{0X84,1,{0x15}},
{0X85,1,{0x17}},
{0X86,1,{0x21}},
{0X87,1,{0x07}},
{0X88,1,{0x07}},  
{0X89,1,{0x07}},  
{0X8a,1,{0x07}},
{0X8b,1,{0x07}},
{0X8c,1,{0x07}},

{0XA0,1,{0x01}},
{0XA1,1,{0x10}},  
{0XA2,1,{0x08}},  
{0XA5,1,{0x10}},  
{0XA6,1,{0x10}},
{0XA7,1,{0x00}},
{0XA8,1,{0x00}},  
{0XA9,1,{0x09}},  
{0XAa,1,{0x09}},

{0Xb9,1,{0x40}},

{0Xd0,1,{0x01}},
{0Xd1,1,{0x00}},
{0Xdc,1,{0x35}},
{0Xdd,1,{0x42}},
{0Xe2,1,{0x00}},
{0Xe6,1,{0x22}},
{0Xe7,1,{0x54}},              //V-porch SRC=V0

// GVDDP GVDDN VCOM VGH VGHO VGL VGLO setup
{0XFF,3,{0x98,0x81,0x05}},
{0X03,1,{0x00}}, //VCOM
{0X04,1,{0xD4}}, //FC //VCOM  -1.4v
{0X58,1,{0x62}},

{0X63,1,{0x88}}, //GVDDN -5.24v
{0X64,1,{0x8A}}, //GVDDP +5.2v
{0X68,1,{0xAA}}, //VGHO  15v WAVEFORM
{0X69,1,{0xB1}}, //VGH  16V 
{0X6A,1,{0xA1}}, ////VGLO  -12v   WAVEFORM
{0X6B,1,{0x93}}, //VGL=-13V


// Resolution 720RGB*1520
{0XFF,3,{0x98,0x81,0x06}},
{0X0F,1,{0x40}},
{0X11,1,{0x03}},
{0X13,1,{0x54}}, //+0.25%//40c1
{0X14,1,{0x41}},
{0X15,1,{0x01}},  //-0.25%//406e
{0X16,1,{0x41}},

{0X17,1,{0xFF}},
{0X18,1,{0x00}},

//{0X20,01,15 //video command off
{0X48,1,{0x0F}}, //1 bit ESD
{0X4D,1,{0x80}}, //1 bit ESD
{0X4E,1,{0x40}}, //1 bit ESD

///////////////////GAMMA/////////////
{0XFF,3,{0x98,0x81,0x08}},
{0XE0,27,{0x40,0x24,0x8A,0xC0,0x01,0x55,0x34,0x59,0x84,0xA7,0xA9,0xDB,0x04,0x29,0x4D,0xAA,0x74,0xA4,0xC3,0xEB,0xFF,0x0D,0x3A,0x6F,0x98,0x03,0xFF}},
{0XE1,27,{0x40,0x24,0x8A,0xC0,0x01,0x55,0x34,0x59,0x84,0xA7,0xA9,0xDB,0x04,0x29,0x4D,0xAA,0x74,0xA4,0xC3,0xEB,0xFF,0x0D,0x3A,0x6F,0x98,0x03,0xFF}},

{0XFF,3,{0x98,0x81,0x06}},
{0XD6,1,{0x85}}, //FTE=TSVD1, FTE1=TSHD
{0X27,1,{0x20}}, //VFP 
{0X28,1,{0x20}}, //VBP
{0X2E,1,{0x01}},//NL enable
{0XC0,1,{0xF7}},
{0XC1,1,{0x02}},
{0XC2,1,{0x04}},

//{0X7C,1,40 //3-Lane                 //0525
//{0XDD,1,10 //3-Lane                 //0525

{0XFF,3,{0x98,0x81,0x0E}},
{0X00,1,{0xA0}}, //LV mode

{0X01,1,{0x28}}, //DELY_VID
{0X11,1,{0x90}},
{0X13,1,{0x14}}, 

{0XFF,3,{0x98,0x81,0x02}},
{0X40,1,{0x43}},
{0X42,1,{0x00}},
{0X4A,1,{0x08}},
{0X4D,1,{0x4E}},
{0X4E,1,{0x00}},
{0X1A,1,{0x48}}, //pump clk 1H


{0XFF,3,{0x98,0x81,0x07}},
{0X0F,1,{0x02}}, //TP_term_GVDD_on         //0525

{0XFF,3,{0x98,0x81,0x00}},//Page0

//{0X35,1,{00}}, //TE enable
//{0X36,1,00}},

	// Sleep Mode On
{0x11,1,{0x00}},
{REGFLAG_DELAY_MS_V3,120,{0x00}},
{0x29,1,{0x00}},
//{REGFLAG_DELAY_MS_V3,20,{0x00}},
{REGFLAG_END_OF_TABLE,1,{0x00}},
};

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const struct LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(struct LCM_UTIL_FUNCS));
}

static void lcm_get_params(struct LCM_PARAMS *params)
{
	//LCD_DEBUG("\t\t ili9881h [lcm_get_params]\n");

	memset(params, 0, sizeof(struct LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;
	
	//min.luo task 8170583:[Power consumption]modify lcd density 20190724
	params->density = LCM_DENSITY;

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
	params->dsi.vertical_sync_active=4;  
	params->dsi.vertical_backporch=8;	
	params->dsi.vertical_frontporch=236;  
	params->dsi.vertical_active_line=FRAME_HEIGHT;

	//params->dsi.line_byte=2180;		
	params->dsi.horizontal_sync_active=8;  
	params->dsi.horizontal_backporch=60;   
	params->dsi.horizontal_frontporch=60;  
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;		

	params->dsi.HS_TRAIL= 7;  // 4.3406868
	params->dsi.HS_PRPR = 4;
	
	params->dsi.CLK_TRAIL= 50;

	//params->dsi.esd_check_enable = 1;

	//params->dsi.customization_esd_check_enable = 1;

	//params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
	//params->dsi.lcm_esd_check_table[0].count        = 1;
	//params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c; 

	/*params->dsi.lcm_esd_check_table[2].cmd          = 0x0F;
	params->dsi.lcm_esd_check_table[2].count        = 1;
	params->dsi.lcm_esd_check_table[2].para_list[0] = 0xC0;*/

	//params->dsi.fbk_div =30;              // fref=26MHz,  fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)   //32


//begin 20180607 liujunting modify for HW requirement
       //min.luo defect 8256838:[SWD_TEST]modify lcd fps to 60 20190816
 	params->dsi.PLL_CLOCK=290;
        params->dsi.ssc_disable = 1;
//end 20180607 liujunting modify for HW requirement
        //params->dsi.ssc_range = 7;/

//min.luo defect 8223070:[SWD_TEST]add round corner for 1st lcm ft8006p 20190809  start
        #ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	//params->round_corner_en = 1;
        params->corner_pattern_width = 720;//Note:这里是屏幕的宽度，不是原始图片宽度
        params->corner_pattern_height = 70;//圆角的高度
        params->corner_pattern_height_bot = 70;
        #endif
//min.luo defect 8223070:[SWD_TEST]add round corner for 1st lcm ft8006p 20190809  end
}

static void push_table(struct LCM_setting_table *table, unsigned int count,
		       unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {
		unsigned cmd;

		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY_MS_V3:
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

//begin 20180315 delete by liujunting
	//cmd=0x03;
	//data=0x40;
	//03h,40---tablet mode

	//ret=tps65132_write_bytes(cmd,data);
        //MDELAY(10);
//end 20180315 delete by liujunting

	cmd=0x00;
	data=0x10;//5.6v//0x12; //vsp 5.8v
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
	MDELAY(2);

	ret=tps65132_write_bytes(cmd,data);
	if(ret<0)
	LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----tps65132---cmd=%0x-- i2c write success-----\n",cmd);
	
	MDELAY(5);
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ONE);
	set_gpio_lcd_enn(1);
	MDELAY(2);
	cmd=0x01;
	data=0x10;//5.6v//0x12; //vsn -5.8v
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
	MDELAY(10);
	//reset high to low to high

	//min.luo defect 8249117:[SWD_TEST]ctp resume faster 20190813  start
	#if 0
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
	#else
	SET_RESET_PIN(1);
	MDELAY(2);
	SET_RESET_PIN(0);
	MDELAY(2);
	SET_RESET_PIN(1);
	MDELAY(5); 

        lcd_queue_load_tp_fw();
       #endif
	//min.luo defect 8249117:[SWD_TEST]ctp resume faster 20190813  end

	
	MDELAY(22); 
	push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);  
	//init_lcm_registers();
	//LCD_DEBUG("lk:a5a_infini\n");
}
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Sleep Mode On
	{0x28,1,{0x00}},
	{REGFLAG_DELAY_MS_V3,50,{0x00}},
	{0x10,1,{0x00}},
	{REGFLAG_DELAY_MS_V3,120,{0x00}},
	{REGFLAG_END_OF_TABLE,1,{0x00}},
};
static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
//begin 20180321 liujunting modify for nt	
	//reset
	//min.luo task 8132794:[Power consumption]save power comsume when lcd/tp sleep in 20190722  start
	#if 0
    	SET_RESET_PIN(1);
	MDELAY(2);
	SET_RESET_PIN(0);
	MDELAY(2);
	SET_RESET_PIN(1);
	MDELAY(5);
	#endif
	//min.luo task 8132794:[Power consumption]save power comsume when lcd/tp sleep in 20190722  end
//end 20180321 liujunting modify for nt
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
/*
    //reset low to high
    SET_RESET_PIN(1);
    MDELAY(50);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(150);
    //enable power
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ZERO);
    MDELAY(50);
    
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
    MDELAY(10);
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ONE);
    MDELAY(10);

    //push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);
    init_lcm_registers();
*/

    lcm_init();
    //LCD_DEBUG("lk:tokyo_tf_ili9881h_hd_dsi_vdo_lcm_resume\n");

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
	//if (id_type==3)//ID pin=01;*/
    		return 1;
	//else 
		//return 0;
}

//min.luo task 8172782:[Power consumption]close ldo28 for saving camera sleep current 20190725  start
#if 0
static void lcm_init_power(void)
{
#ifndef CONFIG_FPGA_EARLY_PORTING
#if 0//def BUILD_LK
	pmic_set_register_value(PMIC_RG_LDO_VLDO28_EN_0,1);
	pmic_set_register_value(PMIC_RG_LDO_VLDO28_SW_OP_EN,1);
	pmic_set_register_value(PMIC_RG_LDO_VLDO28_EN_1,1);

	MDELAY(10);

	//hwPowerOn(MT6328_POWER_LDO_VGP1, VOL_2800, "LCM_DRV");
	//hwPowerOn(MT6328_POWER_LDO_VGP2, VOL_2800, "LCM_DRV");

	
#else
	printk("%s, begin\n", __func__);
	//hwPowerOn(MT6328_POWER_LDO_VGP1, VOL_DEFAULT, "LCM_DRV");
	mt6357_upmu_set_rg_ldo_vldo28_en(1);
	mt6357_upmu_set_rg_ldo_vldo28_1_en(1);
	mt6357_upmu_set_rg_ldo_vldo28_sw_op_en(1);
	printk("%s, end\n", __func__);
#endif
#endif
		
}
#endif
//min.luo task 8172782:[Power consumption]close ldo28 for saving camera sleep current 20190725  end

struct LCM_DRIVER tokyo_tf_ili9881h_hd_dsi_vdo_lcm_drv =
{
    .name           = "tokyo_tf_ili9881h_hd_dsi_vdo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
   // .init_power     = lcm_init_power,//min.luo task 8172782:[Power consumption]close ldo28 for saving camera sleep current 20190725
   
};
#endif

