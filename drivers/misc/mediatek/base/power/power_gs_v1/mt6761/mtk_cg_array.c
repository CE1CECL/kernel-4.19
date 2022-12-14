// SPDX-License-Identifier: GPL-2.0


// This file is generated by GenLP_setting.pl v1.5.9

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

const unsigned int AP_CG_Golden_Setting_tcl_gs_dpidle_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x80808000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808080, 0x80808080,// CLK_CFG_3
	0x10000080, 0x80008080, 0x80008000,// CLK_CFG_4
	0x10000090, 0x80800080, 0x00800080,// CLK_CFG_5
	0x100000A0, 0x00008080, 0x00008080,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10000320, 0x0000003C, 0x0000003C,// CLK_AUDDIV_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF60,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000200,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C00C, 0x0000001E, 0x00000000,// AP_PLL_CON3
	0x1000C014, 0x00036BF0, 0x000020A0,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x0F080104,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_dpidle =
				AP_CG_Golden_Setting_tcl_gs_dpidle_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_dpidle_len = 99;

const unsigned int AP_CG_Golden_Setting_tcl_gs_suspend_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x80808000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808080, 0x80808080,// CLK_CFG_3
	0x10000080, 0x80008080, 0x80008080,// CLK_CFG_4
	0x10000090, 0x80800080, 0x80800080,// CLK_CFG_5
	0x100000A0, 0x00008080, 0x00008080,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00000000,// CLK_MISC_CFG_0
	0x10000320, 0x0000003C, 0x0000003C,// CLK_AUDDIV_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF60,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000200,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C00C, 0x0000001E, 0x00000000,// AP_PLL_CON3
	0x1000C014, 0x00036BF0, 0x000020A0,// AP_PLL_CON5
	0x10017008, 0xFFFFFFFF, 0x00000000,// World_CLK_CNTCV_L
	0x1001700C, 0xFFFFFFFF, 0x00000000,// World_CLK_CNTCV_H
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x0F080104,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_suspend =
				AP_CG_Golden_Setting_tcl_gs_suspend_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_suspend_len = 99;

const unsigned int AP_CG_Golden_Setting_tcl_gs_vp_mjc_data[] = {
	// Address     Mask        Golden Setting Value
	0x14000100, 0x3FFFFFFF, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_vp_mjc =
				AP_CG_Golden_Setting_tcl_gs_vp_mjc_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_vp_mjc_len = 6;

const unsigned int AP_CG_Golden_Setting_tcl_gs_topck_name_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x00000000,// CLK_CFG_1
	0x10000060, 0x80808080, 0x00000000,// CLK_CFG_2
	0x10000070, 0x80808080, 0x00000000,// CLK_CFG_3
	0x10000080, 0x80808080, 0x00000000,// CLK_CFG_4
	0x10000090, 0x80800080, 0x00000000,// CLK_CFG_5
	0x100000A0, 0x80808080, 0x00000000,// CLK_CFG_6
	0x100000B0, 0x00008080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00013F78, 0x00000000,// CLK_MISC_CFG_0
	0x10000320, 0x0000003C, 0x00000000,// CLK_AUDDIV_0
	0x10001080, 0x08FF781F, 0x00000000,// MODULE_SW_CG_0_SET
	0x10001084, 0x08FF781F, 0x00000000,// MODULE_SW_CG_0_CLR
	0x10001088, 0x80004482, 0x00000000,// MODULE_SW_CG_1_SET
	0x1000108C, 0x80004482, 0x00000000,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x37FC87D5, 0x00000000,// MODULE_SW_CG_2_SET
	0x100010A8, 0x37FC87D4, 0x00000000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x00000000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x0000FE5F, 0x00000000,// MODULE_SW_CG_3_SET
	0x100010C4, 0x0000FE5F, 0x00000000,// MODULE_SW_CG_3_CLR
	0x11220000, 0x0F080104, 0x00000000,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x00000000,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000000,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00000000 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_topck_name =
				AP_CG_Golden_Setting_tcl_gs_topck_name_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_topck_name_len = 81;

const unsigned int AP_CG_Golden_Setting_tcl_gs_paging_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00800000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808000, 0x80808000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00008080,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00008000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x0F080104,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_paging =
					AP_CG_Golden_Setting_tcl_gs_paging_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_paging_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_mp3_play_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808000, 0x00000000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00000000,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00008000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x09080100,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_mp3_play =
				AP_CG_Golden_Setting_tcl_gs_mp3_play_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_mp3_play_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_clkon_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x00008000,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80800000,// CLK_CFG_2
	0x10000070, 0x80808080, 0x00000000,// CLK_CFG_3
	0x10000080, 0x80808080, 0x00000000,// CLK_CFG_4
	0x10000090, 0x80808080, 0x00000000,// CLK_CFG_5
	0x100000A0, 0x80808080, 0x00000000,// CLK_CFG_6
	0x100000B0, 0x00008080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00013F78, 0x00003F00,// CLK_MISC_CFG_0
	0x10000320, 0x0000003C, 0x00000000,// CLK_AUDDIV_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x00000000,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x98EFFD6F,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x00000800,// MODULE_SW_CG_1_SET
	0x1000108C, 0x9F9C7F96, 0x9F9C3F96,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x00000000,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x0FFC07DD,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x00000000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00000000,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00018FFF,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x80000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x80000000,// PERIAXI_SI0_CTL
	0x1000C00C, 0x0000001E, 0x0000001E,// AP_PLL_CON3
	0x1000C014, 0x00036BF0, 0x00016BF0,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0x00000000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x00000000,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x00000000,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000000,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00001111,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00001111,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00000000 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_clkon =
					AP_CG_Golden_Setting_tcl_gs_clkon_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_clkon_len = 99;

const unsigned int AP_CG_Golden_Setting_tcl_gs_dcm_off_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x00008000,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80800000,// CLK_CFG_2
	0x10000070, 0x80808000, 0x00000000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00000000,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00000000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000080,// CLK_CFG_7
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x102380F0, 0xFFFF0000, 0x00000000,// GCE_CTL_INT0
	0x17000000, 0x00001111, 0x00001111,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00001111,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00000000 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_dcm_off =
				AP_CG_Golden_Setting_tcl_gs_dcm_off_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_dcm_off_len = 39;

const unsigned int AP_CG_Golden_Setting_tcl_gs_vr_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x00008000,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80800000,// CLK_CFG_2
	0x10000070, 0x80808000, 0x00000000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00000000,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00000000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86D9,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0x00000000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x08080100,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000000,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00001111,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00001111,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00000000 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_vr =
					AP_CG_Golden_Setting_tcl_gs_vr_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_vr_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_vp_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808000,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808000, 0x00000000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00000000,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00000000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86D9,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0x00000000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x09080100,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_vp =
					AP_CG_Golden_Setting_tcl_gs_vp_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_vp_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_access_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000000, 0x00000000,// CLK_MODE
	0x10000040, 0x00000000, 0x00000000,// CLK_CFG_0
	0x10000050, 0x00000000, 0x00000000,// CLK_CFG_1
	0x10000060, 0x00000000, 0x00000000,// CLK_CFG_2
	0x10000070, 0x00000000, 0x00000000,// CLK_CFG_3
	0x10000080, 0x00000000, 0x00000000,// CLK_CFG_4
	0x10000090, 0x00000000, 0x00000000,// CLK_CFG_5
	0x100000A0, 0x00000000, 0x00000000,// CLK_CFG_6
	0x100000B0, 0x00000000, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00000000, 0x00000000,// CLK_MISC_CFG_0
	0x10000320, 0x00000000, 0x00000000,// CLK_AUDDIV_0
	0x10001074, 0x00000000, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x00000000, 0x00000000,// MODULE_SW_CG_0_SET
	0x10001084, 0x00000000, 0x00000000,// MODULE_SW_CG_0_CLR
	0x10001088, 0x00000000, 0x00000000,// MODULE_SW_CG_1_SET
	0x1000108C, 0x00000000, 0x00000000,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x00000000, 0x00000000,// MODULE_SW_CG_2_SET
	0x100010A8, 0x00000000, 0x00000000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x00000000, 0x00000000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x00000000, 0x00000000,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00000000, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x00000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x00000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C00C, 0x00000000, 0x00000000,// AP_PLL_CON3
	0x1000C014, 0x00000000, 0x00000000,// AP_PLL_CON5
	0x102380F0, 0x00000000, 0x00000000,// GCE_CTL_INT0
	0x11220000, 0x00000000, 0x00000000,// AUDIO_TOP_CON0
	0x11220004, 0x00000000, 0x00000000,// AUDIO_TOP_CON1
	0x14000100, 0x00000000, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000000, 0x00000000,// CAMSYS_CG_CON
	0x17000000, 0x00000000, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00000000, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00000000, 0x00000000 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_access =
					AP_CG_Golden_Setting_tcl_gs_access_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_access_len = 99;

const unsigned int AP_CG_Golden_Setting_tcl_gs_pdn_ao_data[] = {
	// Address     Mask        Golden Setting Value
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C00C, 0x0000001E, 0x00000000,// AP_PLL_CON3
	0x1000C014, 0x00036BF0, 0x00000000,// AP_PLL_CON5
	0x15000000, 0x00000FC1, 0x00000000,// CAMSYS_CG_CON
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_pdn_ao =
					AP_CG_Golden_Setting_tcl_gs_pdn_ao_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_pdn_ao_len = 12;

const unsigned int AP_CG_Golden_Setting_tcl_gs_idle_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x80800000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808080, 0x80808080,// CLK_CFG_3
	0x10000080, 0x80008080, 0x80008080,// CLK_CFG_4
	0x10000090, 0x80800080, 0x00800080,// CLK_CFG_5
	0x100000A0, 0x00008080, 0x00008080,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10000320, 0x0000003C, 0x00000000,// CLK_AUDDIV_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF60,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000200,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C00C, 0x0000001E, 0x00000000,// AP_PLL_CON3
	0x1000C014, 0x00036BF0, 0x000022A0,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x0F080104,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_idle =
					AP_CG_Golden_Setting_tcl_gs_idle_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_idle_len = 99;

const unsigned int AP_CG_Golden_Setting_tcl_gs_talk_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00800000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808000, 0x00808000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00000000,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00008000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0x00000000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x08080100,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_talk =
					AP_CG_Golden_Setting_tcl_gs_talk_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_talk_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_connsys_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00800000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808000, 0x80808000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00008080,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00008000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x0F080104,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_connsys =
				AP_CG_Golden_Setting_tcl_gs_connsys_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_connsys_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_sodi_data[] = {
	// Address     Mask        Golden Setting Value
	0x14000100, 0x3FFFFFFF, 0x00000000,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_sodi =
					AP_CG_Golden_Setting_tcl_gs_sodi_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_sodi_len = 6;

const unsigned int AP_CG_Golden_Setting_tcl_gs_datalink_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000000, 0x00000700, 0x00000000,// CLK_MODE
	0x10000040, 0x80808000, 0x00800000,// CLK_CFG_0
	0x10000050, 0x80808080, 0x80808080,// CLK_CFG_1
	0x10000060, 0x80808080, 0x80808080,// CLK_CFG_2
	0x10000070, 0x80808000, 0x80808000,// CLK_CFG_3
	0x10000080, 0x00008080, 0x00008080,// CLK_CFG_4
	0x100000A0, 0x00008000, 0x00008000,// CLK_CFG_6
	0x100000B0, 0x00000080, 0x00000000,// CLK_CFG_7
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x11220000, 0x0F080104, 0x0F080104,// AUDIO_TOP_CON0
	0x11220004, 0x000000F0, 0x000000F0,// AUDIO_TOP_CON1
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
	0x15000000, 0x00000FC1, 0x00000FC1,// CAMSYS_CG_CON
	0x17000000, 0x00001111, 0x00000000,// VCODECSYS_CG_CON
	0x17000004, 0x00001111, 0x00000000,// VCODECSYS_CG_SET
	0x17000008, 0x00001111, 0x00001111 // VCODECSYS_CG_CLR
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_datalink =
				AP_CG_Golden_Setting_tcl_gs_datalink_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_datalink_len = 90;

const unsigned int AP_CG_Golden_Setting_tcl_gs_flight_data[] = {
	// Address     Mask        Golden Setting Value
	0x10000104, 0x00001000, 0x00001000,// CLK_MISC_CFG_0
	0x10001074, 0x00000004, 0x00000000,// PERI_BUS_DCM_CTRL
	0x10001080, 0x98FFFF7F, 0x98FFFF70,// MODULE_SW_CG_0_SET
	0x10001084, 0x98FFFF7F, 0x00000210,// MODULE_SW_CG_0_CLR
	0x10001088, 0x9F9C7F96, 0x1B9C0B96,// MODULE_SW_CG_1_SET
	0x1000108C, 0x1D9C7F96, 0x00080180,// MODULE_SW_CG_1_CLR
	0x100010A4, 0x3FFC87DD, 0x3FFC86DD,// MODULE_SW_CG_2_SET
	0x100010A8, 0x3FFC87DD, 0x30008000,// MODULE_SW_CG_2_CLR
	0x100010AC, 0x30008000, 0x30008000,// MODULE_SW_CG_2_STA
	0x100010C0, 0x003FFFFF, 0x00018FFF,// MODULE_SW_CG_3_SET
	0x100010C4, 0x00018FFF, 0x00000000,// MODULE_SW_CG_3_CLR
	0x10001200, 0x80000000, 0x00000000,// INFRA_TOPAXI_SI0_CTL
	0x1000320C, 0x80000000, 0x00000000,// PERIAXI_SI0_CTL
	0x1000C014, 0x00000200, 0x00000200,// AP_PLL_CON5
	0x102380F0, 0xFFFF0000, 0xFFFF0000,// GCE_CTL_INT0
	0x14000100, 0x3FFFFFFF, 0x3FFFFFFF,// MMSYS_CG_CON0
};

const unsigned int *AP_CG_Golden_Setting_tcl_gs_flight =
					AP_CG_Golden_Setting_tcl_gs_flight_data;

unsigned int AP_CG_Golden_Setting_tcl_gs_flight_len = 48;

