// SPDX-License-Identifier: GPL-2.0

/* This file is generated by GenLP_setting.pl v1.5.7 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

const unsigned int AP_PMIC_REG_gs_suspend_32kless_data[] = {
/*	Address		Mask		Golden Setting Value */
	0x20, 0xA, 0xA,/* TOP_CON */
	0x24, 0x1F00, 0x1F00,/* TEST_CON0 */
	0x30, 0x1, 0x1,/* SMT_CON0 */
	0x10C, 0x10, 0x10,/* TOP_CKPDN_CON0 */
	0x112, 0x4, 0x4,/* TOP_CKPDN_CON1 */
	0x118, 0x8, 0x8,/* TOP_CKSEL_CON0 */
	0x11E, 0x300, 0x0,/* TOP_CKSEL_CON1 */
	0x408, 0x3, 0x1,/* RG_SPI_CON0 */
	0x514, 0xFF, 0x7F,/* SCK_TOP_CKPDN_CON0 */
	0x788, 0x3FF8, 0x6D8,/* DCXO_CW00 */
	0x790, 0x1, 0x0,/* DCXO_CW02 */
	0x7A8, 0x1000, 0x1000,/* DCXO_CW12 */
	0xC0C, 0x5FD, 0x1E4,/* BM_TOP_CKPDN_CON0 */
	0xC1E, 0x3, 0x3,/* BM_TOP_CKHWEN_CON0 */
	0xC2C, 0x1, 0x1,/* BM_TOP_RST_CON1 */
	0xD08, 0x301, 0x301,/* FGADC_CON0 */
	0xD0E, 0x70F, 0x708,/* FGADC_CON3 */
	0xD3E, 0xFFFF, 0x1F,/* FGADC_SON_CON0 */
	0xD40, 0x3FF, 0x14,/* FGADC_SON_CON1 */
	0xD42, 0x3FF, 0xFF,/* FGADC_SON_CON2 */
	0xD44, 0xFFFF, 0x1F,/* FGADC_SOFF_CON0 */
	0xD46, 0x3FF, 0x14,/* FGADC_SOFF_CON1 */
	0xD48, 0x3FF, 0xFF,/* FGADC_SOFF_CON2 */
	0xF2C, 0x1, 0x0,/* BIF_CON18 */
	0xF8C, 0x3FF, 0x115,/* HK_TOP_CLK_CON0 */
	0x1188, 0xE03F, 0x14,/* AUXADC_CON0 */
	0x1198, 0x3FF, 0x13,/* AUXADC_CON6 */
	0x119E, 0x7000, 0x6000,/* AUXADC_CON9 */
	0x11BE, 0x19, 0x11,/* AUXADC_NAG_0 */
	0x11D4, 0x2, 0x0,/* AUXADC_NAG_11 */
	0x1212, 0x2, 0x0,/* AUXADC_IMP5 */
	0x1224, 0x2, 0x0,/* AUXADC_LBAT8 */
	0x1226, 0x1, 0x1,/* AUXADC_BAT_TEMP_0 */
	0x122A, 0x3, 0x3,/* AUXADC_BAT_TEMP_2 */
	0x122E, 0x2000, 0x2000,/* AUXADC_BAT_TEMP_4 */
	0x1238, 0x2, 0x0,/* AUXADC_BAT_TEMP_9 */
	0x123A, 0x1, 0x1,/* AUXADC_LBAT2_0 */
	0x123C, 0x3, 0x2,/* AUXADC_LBAT2_1 */
	0x1240, 0x2000, 0x2000,/* AUXADC_LBAT2_3 */
	0x124A, 0x2, 0x0,/* AUXADC_LBAT2_8 */
	0x125C, 0x2, 0x0,/* AUXADC_THR8 */
	0x125E, 0x8003, 0x8002,/* AUXADC_MDRT_0 */
	0x1262, 0x4, 0x4,/* AUXADC_MDRT_2 */
	0x140C, 0x7, 0x0,/* BUCK_TOP_CLK_CON0 */
	0x1412, 0x7, 0x7,/* BUCK_TOP_CLK_HWEN_CON0 */
	0x1440, 0x400, 0x400,/* BUCK_TOP_ELR0 */
	0x1488, 0x3, 0x0,/* BUCK_VPU_CON0 */
	0x1494, 0x5, 0x0,/* BUCK_VPU_OP_EN */
	0x149A, 0x5, 0x0,/* BUCK_VPU_OP_CFG */
	0x14A0, 0x5, 0x5,/* BUCK_VPU_OP_MODE */
	0x1508, 0x3, 0x1,/* BUCK_VCORE_CON0 */
	0x1514, 0x5, 0x5,/* BUCK_VCORE_OP_EN */
	0x151A, 0x5, 0x5,/* BUCK_VCORE_OP_CFG */
	0x1520, 0x5, 0x5,/* BUCK_VCORE_OP_MODE */
	0x1588, 0x3, 0x0,/* BUCK_VGPU11_CON0 */
	0x1594, 0x5, 0x0,/* BUCK_VGPU11_OP_EN */
	0x159A, 0x5, 0x5,/* BUCK_VGPU11_OP_CFG */
	0x15A0, 0x5, 0x5,/* BUCK_VGPU11_OP_MODE */
	0x1608, 0x3, 0x0,/* BUCK_VGPU12_CON0 */
	0x1614, 0x5, 0x0,/* BUCK_VGPU12_OP_EN */
	0x161A, 0x5, 0x5,/* BUCK_VGPU12_OP_CFG */
	0x1620, 0x5, 0x5,/* BUCK_VGPU12_OP_MODE */
	0x1688, 0x3, 0x1,/* BUCK_VMODEM_CON0 */
	0x1694, 0x5, 0x5,/* BUCK_VMODEM_OP_EN */
	0x169A, 0x5, 0x5,/* BUCK_VMODEM_OP_CFG */
	0x16A0, 0x5, 0x5,/* BUCK_VMODEM_OP_MODE */
	0x1708, 0x3, 0x0,/* BUCK_VPROC1_CON0 */
	0x1714, 0x5, 0x0,/* BUCK_VPROC1_OP_EN */
	0x171A, 0x5, 0x5,/* BUCK_VPROC1_OP_CFG */
	0x1720, 0x5, 0x5,/* BUCK_VPROC1_OP_MODE */
	0x1788, 0x3, 0x0,/* BUCK_VPROC2_CON0 */
	0x1794, 0x5, 0x0,/* BUCK_VPROC2_OP_EN */
	0x179A, 0x5, 0x5,/* BUCK_VPROC2_OP_CFG */
	0x17A0, 0x5, 0x5,/* BUCK_VPROC2_OP_MODE */
	0x1808, 0x3, 0x1,/* BUCK_VS1_CON0 */
	0x1814, 0x5, 0x5,/* BUCK_VS1_OP_EN */
	0x181A, 0x5, 0x5,/* BUCK_VS1_OP_CFG */
	0x1820, 0x5, 0x5,/* BUCK_VS1_OP_MODE */
	0x1888, 0x3, 0x1,/* BUCK_VS2_CON0 */
	0x1894, 0x5, 0x5,/* BUCK_VS2_OP_EN */
	0x189A, 0x5, 0x5,/* BUCK_VS2_OP_CFG */
	0x18A0, 0x5, 0x5,/* BUCK_VS2_OP_MODE */
	0x1908, 0x3, 0x0,/* BUCK_VPA_CON0 */
	0x1B0E, 0x3, 0x3,/* TOP_TOP_CKHWEN_CON0 */
	0x1B10, 0x1, 0x1,/* LDO_TOP_CLK_DCM_CON0 */
	0x1B34, 0x2, 0x2,/* VRTC28_CON */
	0x1B88, 0x8000, 0x0,/* LDO_VFE28_CON0 */
	0x1B8C, 0x2, 0x2,/* LDO_VFE28_OP_EN */
	0x1B98, 0x8010, 0x0,/* LDO_VXO22_CON0 */
	0x1B9C, 0x1, 0x1,/* LDO_VXO22_OP_EN */
	0x1BA2, 0x1, 0x1,/* LDO_VXO22_OP_CFG */
	0x1BA8, 0x8000, 0x0,/* LDO_VRF18_CON0 */
	0x1BAC, 0x2, 0x2,/* LDO_VRF18_OP_EN */
	0x1BB8, 0x8000, 0x0,/* LDO_VRF12_CON0 */
	0x1BBC, 0x2, 0x2,/* LDO_VRF12_OP_EN */
	0x1BC8, 0x8001, 0x0,/* LDO_VEFUSE_CON0 */
	0x1BD8, 0x8001, 0x0,/* LDO_VCN33_1_CON0 */
	0x1BE8, 0x8000, 0x0,/* LDO_VCN33_1_MULTI_SW */
	0x1C08, 0x8001, 0x0,/* LDO_VCN33_2_CON0 */
	0x1C18, 0x8000, 0x0,/* LDO_VCN33_2_MULTI_SW */
	0x1C1A, 0x8001, 0x0,/* LDO_VCN13_CON0 */
	0x1C2A, 0x8001, 0x0,/* LDO_VCN18_CON0 */
	0x1C3A, 0x8010, 0x0,/* LDO_VA09_CON0 */
	0x1C3E, 0x1, 0x1,/* LDO_VA09_OP_EN */
	0x1C44, 0x1, 0x1,/* LDO_VA09_OP_CFG */
	0x1C4A, 0x8001, 0x0,/* LDO_VCAMIO_CON0 */
	0x1C5A, 0x8010, 0x0,/* LDO_VA12_CON0 */
	0x1C5E, 0x1, 0x1,/* LDO_VA12_OP_EN */
	0x1C64, 0x1, 0x1,/* LDO_VA12_OP_CFG */
	0x1C88, 0x8010, 0x0,/* LDO_VAUX18_CON0 */
	0x1C8C, 0x1, 0x1,/* LDO_VAUX18_OP_EN */
	0x1C92, 0x1, 0x1,/* LDO_VAUX18_OP_CFG */
	0x1C98, 0x8010, 0x0,/* LDO_VAUD18_CON0 */
	0x1C9C, 0x1, 0x1,/* LDO_VAUD18_OP_EN */
	0x1CA2, 0x1, 0x1,/* LDO_VAUD18_OP_CFG */
	0x1CA8, 0x8010, 0x0,/* LDO_VIO18_CON0 */
	0x1CAC, 0x1, 0x1,/* LDO_VIO18_OP_EN */
	0x1CB2, 0x1, 0x1,/* LDO_VIO18_OP_CFG */
	0x1CB8, 0x8001, 0x0,/* LDO_VEMC_CON0 */
	0x1CC8, 0x8001, 0x0,/* LDO_VSIM1_CON0 */
	0x1CD8, 0x8001, 0x0,/* LDO_VSIM2_CON0 */
	0x1D08, 0x8010, 0x0,/* LDO_VUSB_CON0 */
	0x1D0C, 0x1, 0x1,/* LDO_VUSB_OP_EN */
	0x1D12, 0x1, 0x1,/* LDO_VUSB_OP_CFG */
	0x1D1A, 0x8001, 0x0,/* LDO_VRFCK_CON0 */
	0x1D1E, 0x4002, 0x4002,/* LDO_VRFCK_OP_EN */
	0x1D2A, 0x8001, 0x0,/* LDO_VBBCK_CON0 */
	0x1D2E, 0x4000, 0x4000,/* LDO_VBBCK_OP_EN */
	0x1D3A, 0x8000, 0x0,/* LDO_VBIF28_CON0 */
	0x1D3E, 0x1, 0x1,/* LDO_VBIF28_OP_EN */
	0x1D4A, 0x8001, 0x0,/* LDO_VIBR_CON0 */
	0x1D5A, 0x8001, 0x1,/* LDO_VIO28_CON0 */
	0x1D88, 0x8010, 0x0,/* LDO_VM18_CON0 */
	0x1D8C, 0x1, 0x1,/* LDO_VM18_OP_EN */
	0x1D92, 0x1, 0x1,/* LDO_VM18_OP_CFG */
	0x1D98, 0x8001, 0x0,/* LDO_VUFS_CON0 */
	0x1E88, 0x8001, 0x0,/* LDO_VSRAM_PROC1_CON0 */
	0x1EA6, 0x8001, 0x0,/* LDO_VSRAM_PROC2_CON0 */
	0x1F08, 0x8010, 0x0,/* LDO_VSRAM_OTHERS_CON0 */
	0x1F14, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_EN */
	0x1F1A, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_CFG */
	0x1F2C, 0x8011, 0x1 /* LDO_VSRAM_MD_CON0 */
};

const unsigned int *AP_PMIC_REG_gs_suspend_32kless =
				AP_PMIC_REG_gs_suspend_32kless_data;

unsigned int AP_PMIC_REG_gs_suspend_32kless_len = 426;

const unsigned int AP_PMIC_REG_gs_deepidle___lp_mp3_32kless_data[] = {
/*	Address		Mask		Golden Setting Value */
	0x20, 0xA, 0xA,/* TOP_CON */
	0x24, 0x1F00, 0x1F00,/* TEST_CON0 */
	0x30, 0x1, 0x1,/* SMT_CON0 */
	0x10C, 0x10, 0x10,/* TOP_CKPDN_CON0 */
	0x112, 0x4, 0x4,/* TOP_CKPDN_CON1 */
	0x118, 0x8, 0x8,/* TOP_CKSEL_CON0 */
	0x11E, 0x300, 0x0,/* TOP_CKSEL_CON1 */
	0x408, 0x3, 0x1,/* RG_SPI_CON0 */
	0x514, 0xFF, 0x7F,/* SCK_TOP_CKPDN_CON0 */
	0x788, 0x3FF8, 0x16D8,/* DCXO_CW00 */
	0x790, 0x1, 0x0,/* DCXO_CW02 */
	0x7A8, 0x1000, 0x1000,/* DCXO_CW12 */
	0xC0C, 0x5FD, 0x1E4,/* BM_TOP_CKPDN_CON0 */
	0xC1E, 0x3, 0x3,/* BM_TOP_CKHWEN_CON0 */
	0xC2C, 0x1, 0x1,/* BM_TOP_RST_CON1 */
	0xD08, 0x301, 0x301,/* FGADC_CON0 */
	0xD0E, 0x70F, 0x708,/* FGADC_CON3 */
	0xD3E, 0xFFFF, 0x1F,/* FGADC_SON_CON0 */
	0xD40, 0x3FF, 0x14,/* FGADC_SON_CON1 */
	0xD42, 0x3FF, 0xFF,/* FGADC_SON_CON2 */
	0xD44, 0xFFFF, 0x1F,/* FGADC_SOFF_CON0 */
	0xD46, 0x3FF, 0x14,/* FGADC_SOFF_CON1 */
	0xD48, 0x3FF, 0xFF,/* FGADC_SOFF_CON2 */
	0xF2C, 0x1, 0x0,/* BIF_CON18 */
	0xF8C, 0x3FF, 0x115,/* HK_TOP_CLK_CON0 */
	0x1188, 0xE03F, 0x14,/* AUXADC_CON0 */
	0x1198, 0x3FF, 0x13,/* AUXADC_CON6 */
	0x119E, 0x7000, 0x6000,/* AUXADC_CON9 */
	0x11BE, 0x19, 0x11,/* AUXADC_NAG_0 */
	0x11D4, 0x2, 0x0,/* AUXADC_NAG_11 */
	0x1212, 0x2, 0x0,/* AUXADC_IMP5 */
	0x1224, 0x2, 0x0,/* AUXADC_LBAT8 */
	0x1226, 0x1, 0x1,/* AUXADC_BAT_TEMP_0 */
	0x122A, 0x3, 0x3,/* AUXADC_BAT_TEMP_2 */
	0x122E, 0x2000, 0x2000,/* AUXADC_BAT_TEMP_4 */
	0x1238, 0x2, 0x0,/* AUXADC_BAT_TEMP_9 */
	0x123A, 0x1, 0x1,/* AUXADC_LBAT2_0 */
	0x123C, 0x3, 0x2,/* AUXADC_LBAT2_1 */
	0x1240, 0x2000, 0x2000,/* AUXADC_LBAT2_3 */
	0x124A, 0x2, 0x0,/* AUXADC_LBAT2_8 */
	0x125C, 0x2, 0x0,/* AUXADC_THR8 */
	0x125E, 0x8003, 0x8002,/* AUXADC_MDRT_0 */
	0x1262, 0x4, 0x4,/* AUXADC_MDRT_2 */
	0x140C, 0x7, 0x0,/* BUCK_TOP_CLK_CON0 */
	0x1412, 0x7, 0x7,/* BUCK_TOP_CLK_HWEN_CON0 */
	0x1440, 0x400, 0x400,/* BUCK_TOP_ELR0 */
	0x1488, 0x3, 0x0,/* BUCK_VPU_CON0 */
	0x1494, 0x5, 0x0,/* BUCK_VPU_OP_EN */
	0x149A, 0x5, 0x5,/* BUCK_VPU_OP_CFG */
	0x14A0, 0x5, 0x5,/* BUCK_VPU_OP_MODE */
	0x1508, 0x3, 0x1,/* BUCK_VCORE_CON0 */
	0x1514, 0x5, 0x5,/* BUCK_VCORE_OP_EN */
	0x151A, 0x5, 0x5,/* BUCK_VCORE_OP_CFG */
	0x1520, 0x5, 0x5,/* BUCK_VCORE_OP_MODE */
	0x1588, 0x3, 0x0,/* BUCK_VGPU11_CON0 */
	0x1594, 0x5, 0x0,/* BUCK_VGPU11_OP_EN */
	0x159A, 0x5, 0x5,/* BUCK_VGPU11_OP_CFG */
	0x15A0, 0x5, 0x5,/* BUCK_VGPU11_OP_MODE */
	0x1608, 0x3, 0x0,/* BUCK_VGPU12_CON0 */
	0x1614, 0x5, 0x0,/* BUCK_VGPU12_OP_EN */
	0x161A, 0x5, 0x5,/* BUCK_VGPU12_OP_CFG */
	0x1620, 0x5, 0x5,/* BUCK_VGPU12_OP_MODE */
	0x1688, 0x3, 0x1,/* BUCK_VMODEM_CON0 */
	0x1694, 0x5, 0x1,/* BUCK_VMODEM_OP_EN */
	0x169A, 0x5, 0x5,/* BUCK_VMODEM_OP_CFG */
	0x16A0, 0x5, 0x5,/* BUCK_VMODEM_OP_MODE */
	0x1708, 0x3, 0x0,/* BUCK_VPROC1_CON0 */
	0x1714, 0x5, 0x0,/* BUCK_VPROC1_OP_EN */
	0x171A, 0x5, 0x5,/* BUCK_VPROC1_OP_CFG */
	0x1720, 0x5, 0x5,/* BUCK_VPROC1_OP_MODE */
	0x1788, 0x3, 0x0,/* BUCK_VPROC2_CON0 */
	0x1794, 0x5, 0x0,/* BUCK_VPROC2_OP_EN */
	0x179A, 0x5, 0x5,/* BUCK_VPROC2_OP_CFG */
	0x17A0, 0x5, 0x5,/* BUCK_VPROC2_OP_MODE */
	0x1808, 0x3, 0x1,/* BUCK_VS1_CON0 */
	0x1814, 0x5, 0x5,/* BUCK_VS1_OP_EN */
	0x181A, 0x5, 0x5,/* BUCK_VS1_OP_CFG */
	0x1820, 0x5, 0x5,/* BUCK_VS1_OP_MODE */
	0x1888, 0x3, 0x1,/* BUCK_VS2_CON0 */
	0x1894, 0x5, 0x5,/* BUCK_VS2_OP_EN */
	0x189A, 0x5, 0x5,/* BUCK_VS2_OP_CFG */
	0x18A0, 0x5, 0x5,/* BUCK_VS2_OP_MODE */
	0x1908, 0x3, 0x0,/* BUCK_VPA_CON0 */
	0x1B0E, 0x3, 0x3,/* TOP_TOP_CKHWEN_CON0 */
	0x1B10, 0x1, 0x1,/* LDO_TOP_CLK_DCM_CON0 */
	0x1B34, 0x2, 0x2,/* VRTC28_CON */
	0x1B88, 0x8000, 0x0,/* LDO_VFE28_CON0 */
	0x1B8C, 0x2, 0x2,/* LDO_VFE28_OP_EN */
	0x1B98, 0x8010, 0x0,/* LDO_VXO22_CON0 */
	0x1B9C, 0x4, 0x4,/* LDO_VXO22_OP_EN */
	0x1BA2, 0x4, 0x4,/* LDO_VXO22_OP_CFG */
	0x1BA8, 0x8000, 0x0,/* LDO_VRF18_CON0 */
	0x1BAC, 0x2, 0x2,/* LDO_VRF18_OP_EN */
	0x1BB8, 0x8000, 0x0,/* LDO_VRF12_CON0 */
	0x1BBC, 0x2, 0x2,/* LDO_VRF12_OP_EN */
	0x1BC8, 0x8001, 0x0,/* LDO_VEFUSE_CON0 */
	0x1BD8, 0x8001, 0x0,/* LDO_VCN33_1_CON0 */
	0x1BE8, 0x8000, 0x0,/* LDO_VCN33_1_MULTI_SW */
	0x1C08, 0x8001, 0x0,/* LDO_VCN33_2_CON0 */
	0x1C18, 0x8000, 0x0,/* LDO_VCN33_2_MULTI_SW */
	0x1C1A, 0x8001, 0x0,/* LDO_VCN13_CON0 */
	0x1C2A, 0x8001, 0x0,/* LDO_VCN18_CON0 */
	0x1C3A, 0x8010, 0x0,/* LDO_VA09_CON0 */
	0x1C3E, 0x4, 0x4,/* LDO_VA09_OP_EN */
	0x1C44, 0x4, 0x4,/* LDO_VA09_OP_CFG */
	0x1C4A, 0x8001, 0x0,/* LDO_VCAMIO_CON0 */
	0x1C5A, 0x8010, 0x0,/* LDO_VA12_CON0 */
	0x1C5E, 0x4, 0x4,/* LDO_VA12_OP_EN */
	0x1C64, 0x4, 0x4,/* LDO_VA12_OP_CFG */
	0x1C88, 0x8010, 0x0,/* LDO_VAUX18_CON0 */
	0x1C8C, 0x4, 0x4,/* LDO_VAUX18_OP_EN */
	0x1C92, 0x4, 0x4,/* LDO_VAUX18_OP_CFG */
	0x1C98, 0x8001, 0x0,/* LDO_VAUD18_CON0 */
	0x1CA8, 0x8011, 0x1,/* LDO_VIO18_CON0 */
	0x1CB8, 0x8001, 0x1,/* LDO_VEMC_CON0 */
	0x1CC8, 0x8001, 0x0,/* LDO_VSIM1_CON0 */
	0x1CD8, 0x8001, 0x0,/* LDO_VSIM2_CON0 */
	0x1D08, 0x8010, 0x0,/* LDO_VUSB_CON0 */
	0x1D0C, 0x4, 0x4,/* LDO_VUSB_OP_EN */
	0x1D12, 0x4, 0x4,/* LDO_VUSB_OP_CFG */
	0x1D1A, 0x8001, 0x0,/* LDO_VRFCK_CON0 */
	0x1D1E, 0x4002, 0x4002,/* LDO_VRFCK_OP_EN */
	0x1D2A, 0x8001, 0x0,/* LDO_VBBCK_CON0 */
	0x1D2E, 0x4000, 0x4000,/* LDO_VBBCK_OP_EN */
	0x1D3A, 0x8000, 0x0,/* LDO_VBIF28_CON0 */
	0x1D3E, 0x4, 0x4,/* LDO_VBIF28_OP_EN */
	0x1D4A, 0x8001, 0x0,/* LDO_VIBR_CON0 */
	0x1D5A, 0x8001, 0x1,/* LDO_VIO28_CON0 */
	0x1D88, 0x8010, 0x0,/* LDO_VM18_CON0 */
	0x1D8C, 0x4, 0x4,/* LDO_VM18_OP_EN */
	0x1D92, 0x4, 0x4,/* LDO_VM18_OP_CFG */
	0x1D98, 0x8001, 0x1,/* LDO_VUFS_CON0 */
	0x1E88, 0x8012, 0x2,/* LDO_VSRAM_PROC1_CON0 */
	0x1EA6, 0x8012, 0x2,/* LDO_VSRAM_PROC2_CON0 */
	0x1F08, 0x8010, 0x0,/* LDO_VSRAM_OTHERS_CON0 */
	0x1F14, 0x4, 0x4,/* LDO_VSRAM_OTHERS_OP_EN */
	0x1F1A, 0x4, 0x4,/* LDO_VSRAM_OTHERS_OP_CFG */
	0x1F2C, 0x8011, 0x1 /* LDO_VSRAM_MD_CON0 */
};

const unsigned int *AP_PMIC_REG_gs_deepidle___lp_mp3_32kless =
				AP_PMIC_REG_gs_deepidle___lp_mp3_32kless_data;

unsigned int AP_PMIC_REG_gs_deepidle___lp_mp3_32kless_len = 414;

const unsigned int AP_PMIC_REG_gs_sodi3p0_32kless_data[] = {
/*	Address		Mask		Golden Setting Value */
	0x20, 0xA, 0xA,/* TOP_CON */
	0x24, 0x1F00, 0x1F00,/* TEST_CON0 */
	0x30, 0x1, 0x1,/* SMT_CON0 */
	0x10C, 0x10, 0x10,/* TOP_CKPDN_CON0 */
	0x112, 0x4, 0x4,/* TOP_CKPDN_CON1 */
	0x118, 0x8, 0x8,/* TOP_CKSEL_CON0 */
	0x11E, 0x300, 0x0,/* TOP_CKSEL_CON1 */
	0x408, 0x3, 0x1,/* RG_SPI_CON0 */
	0x514, 0xFF, 0x7F,/* SCK_TOP_CKPDN_CON0 */
	0x788, 0x3FF8, 0x6D8,/* DCXO_CW00 */
	0x790, 0x1, 0x0,/* DCXO_CW02 */
	0x7A8, 0x1000, 0x1000,/* DCXO_CW12 */
	0xC0C, 0x5FD, 0x1E4,/* BM_TOP_CKPDN_CON0 */
	0xC1E, 0x3, 0x3,/* BM_TOP_CKHWEN_CON0 */
	0xC2C, 0x1, 0x1,/* BM_TOP_RST_CON1 */
	0xD08, 0x301, 0x301,/* FGADC_CON0 */
	0xD0E, 0x70F, 0x708,/* FGADC_CON3 */
	0xD3E, 0xFFFF, 0x1F,/* FGADC_SON_CON0 */
	0xD40, 0x3FF, 0x14,/* FGADC_SON_CON1 */
	0xD42, 0x3FF, 0xFF,/* FGADC_SON_CON2 */
	0xD44, 0xFFFF, 0x1F,/* FGADC_SOFF_CON0 */
	0xD46, 0x3FF, 0x14,/* FGADC_SOFF_CON1 */
	0xD48, 0x3FF, 0xFF,/* FGADC_SOFF_CON2 */
	0xF2C, 0x1, 0x0,/* BIF_CON18 */
	0xF8C, 0x3FF, 0x115,/* HK_TOP_CLK_CON0 */
	0x1188, 0xE03F, 0x14,/* AUXADC_CON0 */
	0x1198, 0x3FF, 0x13,/* AUXADC_CON6 */
	0x119E, 0x7000, 0x6000,/* AUXADC_CON9 */
	0x11BE, 0x19, 0x11,/* AUXADC_NAG_0 */
	0x11D4, 0x2, 0x0,/* AUXADC_NAG_11 */
	0x1212, 0x2, 0x0,/* AUXADC_IMP5 */
	0x1224, 0x2, 0x0,/* AUXADC_LBAT8 */
	0x1226, 0x1, 0x1,/* AUXADC_BAT_TEMP_0 */
	0x122A, 0x3, 0x3,/* AUXADC_BAT_TEMP_2 */
	0x122E, 0x2000, 0x2000,/* AUXADC_BAT_TEMP_4 */
	0x1238, 0x2, 0x0,/* AUXADC_BAT_TEMP_9 */
	0x123A, 0x1, 0x1,/* AUXADC_LBAT2_0 */
	0x123C, 0x3, 0x2,/* AUXADC_LBAT2_1 */
	0x1240, 0x2000, 0x2000,/* AUXADC_LBAT2_3 */
	0x124A, 0x2, 0x0,/* AUXADC_LBAT2_8 */
	0x125C, 0x2, 0x0,/* AUXADC_THR8 */
	0x125E, 0x8003, 0x8002,/* AUXADC_MDRT_0 */
	0x1262, 0x4, 0x4,/* AUXADC_MDRT_2 */
	0x140C, 0x7, 0x0,/* BUCK_TOP_CLK_CON0 */
	0x1412, 0x7, 0x7,/* BUCK_TOP_CLK_HWEN_CON0 */
	0x1440, 0x400, 0x400,/* BUCK_TOP_ELR0 */
	0x1488, 0x3, 0x0,/* BUCK_VPU_CON0 */
	0x1494, 0x5, 0x0,/* BUCK_VPU_OP_EN */
	0x149A, 0x5, 0x5,/* BUCK_VPU_OP_CFG */
	0x14A0, 0x5, 0x5,/* BUCK_VPU_OP_MODE */
	0x1508, 0x3, 0x1,/* BUCK_VCORE_CON0 */
	0x1514, 0x5, 0x5,/* BUCK_VCORE_OP_EN */
	0x151A, 0x5, 0x5,/* BUCK_VCORE_OP_CFG */
	0x1520, 0x5, 0x5,/* BUCK_VCORE_OP_MODE */
	0x1588, 0x3, 0x0,/* BUCK_VGPU11_CON0 */
	0x1594, 0x5, 0x0,/* BUCK_VGPU11_OP_EN */
	0x159A, 0x5, 0x5,/* BUCK_VGPU11_OP_CFG */
	0x15A0, 0x5, 0x5,/* BUCK_VGPU11_OP_MODE */
	0x1608, 0x3, 0x0,/* BUCK_VGPU12_CON0 */
	0x1614, 0x5, 0x0,/* BUCK_VGPU12_OP_EN */
	0x161A, 0x5, 0x5,/* BUCK_VGPU12_OP_CFG */
	0x1620, 0x5, 0x5,/* BUCK_VGPU12_OP_MODE */
	0x1688, 0x3, 0x1,/* BUCK_VMODEM_CON0 */
	0x1694, 0x5, 0x5,/* BUCK_VMODEM_OP_EN */
	0x169A, 0x5, 0x5,/* BUCK_VMODEM_OP_CFG */
	0x16A0, 0x5, 0x5,/* BUCK_VMODEM_OP_MODE */
	0x1708, 0x3, 0x0,/* BUCK_VPROC1_CON0 */
	0x1714, 0x5, 0x0,/* BUCK_VPROC1_OP_EN */
	0x171A, 0x5, 0x5,/* BUCK_VPROC1_OP_CFG */
	0x1720, 0x5, 0x5,/* BUCK_VPROC1_OP_MODE */
	0x1788, 0x3, 0x3,/* BUCK_VPROC2_CON0 */
	0x1794, 0x5, 0x0,/* BUCK_VPROC2_OP_EN */
	0x179A, 0x5, 0x5,/* BUCK_VPROC2_OP_CFG */
	0x17A0, 0x5, 0x5,/* BUCK_VPROC2_OP_MODE */
	0x1808, 0x3, 0x1,/* BUCK_VS1_CON0 */
	0x1814, 0x5, 0x5,/* BUCK_VS1_OP_EN */
	0x181A, 0x5, 0x5,/* BUCK_VS1_OP_CFG */
	0x1820, 0x5, 0x5,/* BUCK_VS1_OP_MODE */
	0x1888, 0x3, 0x1,/* BUCK_VS2_CON0 */
	0x1894, 0x5, 0x5,/* BUCK_VS2_OP_EN */
	0x189A, 0x5, 0x5,/* BUCK_VS2_OP_CFG */
	0x18A0, 0x5, 0x5,/* BUCK_VS2_OP_MODE */
	0x1908, 0x3, 0x0,/* BUCK_VPA_CON0 */
	0x1B0E, 0x3, 0x3,/* TOP_TOP_CKHWEN_CON0 */
	0x1B10, 0x1, 0x1,/* LDO_TOP_CLK_DCM_CON0 */
	0x1B34, 0x2, 0x2,/* VRTC28_CON */
	0x1B88, 0x8000, 0x0,/* LDO_VFE28_CON0 */
	0x1B8C, 0x2, 0x2,/* LDO_VFE28_OP_EN */
	0x1B98, 0x8010, 0x0,/* LDO_VXO22_CON0 */
	0x1B9C, 0x1, 0x1,/* LDO_VXO22_OP_EN */
	0x1BA2, 0x1, 0x1,/* LDO_VXO22_OP_CFG */
	0x1BA8, 0x8000, 0x0,/* LDO_VRF18_CON0 */
	0x1BAC, 0x2, 0x2,/* LDO_VRF18_OP_EN */
	0x1BB8, 0x8000, 0x0,/* LDO_VRF12_CON0 */
	0x1BBC, 0x2, 0x2,/* LDO_VRF12_OP_EN */
	0x1BC8, 0x8001, 0x0,/* LDO_VEFUSE_CON0 */
	0x1BD8, 0x8001, 0x1,/* LDO_VCN33_1_CON0 */
	0x1BE8, 0x8000, 0x8000,/* LDO_VCN33_1_MULTI_SW */
	0x1C08, 0x8001, 0x0,/* LDO_VCN33_2_CON0 */
	0x1C18, 0x8000, 0x0,/* LDO_VCN33_2_MULTI_SW */
	0x1C1A, 0x8001, 0x0,/* LDO_VCN13_CON0 */
	0x1C2A, 0x8001, 0x0,/* LDO_VCN18_CON0 */
	0x1C3A, 0x8010, 0x0,/* LDO_VA09_CON0 */
	0x1C3E, 0x1, 0x1,/* LDO_VA09_OP_EN */
	0x1C44, 0x1, 0x1,/* LDO_VA09_OP_CFG */
	0x1C4A, 0x8001, 0x0,/* LDO_VCAMIO_CON0 */
	0x1C5A, 0x8010, 0x0,/* LDO_VA12_CON0 */
	0x1C5E, 0x1, 0x1,/* LDO_VA12_OP_EN */
	0x1C64, 0x1, 0x1,/* LDO_VA12_OP_CFG */
	0x1C88, 0x8010, 0x0,/* LDO_VAUX18_CON0 */
	0x1C8C, 0x1, 0x1,/* LDO_VAUX18_OP_EN */
	0x1C92, 0x1, 0x1,/* LDO_VAUX18_OP_CFG */
	0x1C98, 0x8010, 0x0,/* LDO_VAUD18_CON0 */
	0x1C9C, 0x1, 0x1,/* LDO_VAUD18_OP_EN */
	0x1CA2, 0x1, 0x1,/* LDO_VAUD18_OP_CFG */
	0x1CA8, 0x8011, 0x1,/* LDO_VIO18_CON0 */
	0x1CB8, 0x8001, 0x1,/* LDO_VEMC_CON0 */
	0x1CC8, 0x8001, 0x0,/* LDO_VSIM1_CON0 */
	0x1CD8, 0x8001, 0x0,/* LDO_VSIM2_CON0 */
	0x1D08, 0x8010, 0x0,/* LDO_VUSB_CON0 */
	0x1D0C, 0x1, 0x1,/* LDO_VUSB_OP_EN */
	0x1D12, 0x1, 0x1,/* LDO_VUSB_OP_CFG */
	0x1D1A, 0x8001, 0x0,/* LDO_VRFCK_CON0 */
	0x1D1E, 0x4002, 0x4002,/* LDO_VRFCK_OP_EN */
	0x1D2A, 0x8001, 0x0,/* LDO_VBBCK_CON0 */
	0x1D2E, 0x4000, 0x4000,/* LDO_VBBCK_OP_EN */
	0x1D3A, 0x8000, 0x0,/* LDO_VBIF28_CON0 */
	0x1D3E, 0x1, 0x1,/* LDO_VBIF28_OP_EN */
	0x1D4A, 0x8001, 0x0,/* LDO_VIBR_CON0 */
	0x1D5A, 0x8001, 0x1,/* LDO_VIO28_CON0 */
	0x1D88, 0x8010, 0x0,/* LDO_VM18_CON0 */
	0x1D8C, 0x1, 0x1,/* LDO_VM18_OP_EN */
	0x1D92, 0x1, 0x1,/* LDO_VM18_OP_CFG */
	0x1D98, 0x8001, 0x1,/* LDO_VUFS_CON0 */
	0x1E88, 0x8012, 0x2,/* LDO_VSRAM_PROC1_CON0 */
	0x1EA6, 0x8012, 0x2,/* LDO_VSRAM_PROC2_CON0 */
	0x1F08, 0x8010, 0x0,/* LDO_VSRAM_OTHERS_CON0 */
	0x1F14, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_EN */
	0x1F1A, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_CFG */
	0x1F2C, 0x8011, 0x1 /* LDO_VSRAM_MD_CON0 */
};

const unsigned int *AP_PMIC_REG_gs_sodi3p0_32kless =
				AP_PMIC_REG_gs_sodi3p0_32kless_data;

unsigned int AP_PMIC_REG_gs_sodi3p0_32kless_len = 420;
