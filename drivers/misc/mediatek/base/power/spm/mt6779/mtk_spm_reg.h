/* SPDX-License-Identifier: GPL-2.0 */



#ifndef _MT_SPM_REG_
#define _MT_SPM_REG_

#include "sleep_def.h"
#include "pcm_def.h"


#define POWERON_CONFIG_EN              (SPM_BASE + 0x0000)
#define SPM_POWER_ON_VAL0              (SPM_BASE + 0x0004)
#define SPM_POWER_ON_VAL1              (SPM_BASE + 0x0008)
#define SPM_CLK_CON                    (SPM_BASE + 0x000C)
#define SPM_CLK_SETTLE                 (SPM_BASE + 0x0010)
#define SPM_AP_STANDBY_CON             (SPM_BASE + 0x0014)
#define PCM_CON0                       (SPM_BASE + 0x0018)
#define PCM_CON1                       (SPM_BASE + 0x001C)
#define PCM_TIMER_VAL                  (SPM_BASE + 0x0020)
#define PCM_WDT_VAL                    (SPM_BASE + 0x0024)
#define PCM_REG_DATA_INI               (SPM_BASE + 0x0028)
#define PCM_PWR_IO_EN                  (SPM_BASE + 0x002C)
#define SPM_VCORE_DVFS_SHORTCUT_CON00  (SPM_BASE + 0x0030)
#define SPM_VCORE_DVFS_SHORTCUT_CON01  (SPM_BASE + 0x0034)
#define SPM_VCORE_DVFS_SHORTCUT_CON02  (SPM_BASE + 0x0038)
#define SPM_VCORE_DVFS_SHORTCUT_CON03  (SPM_BASE + 0x003C)
#define SPM_VCORE_DVFS_SHORTCUT_CON04  (SPM_BASE + 0x0040)
#define SPM_VCORE_DVFS_SHORTCUT_CON05  (SPM_BASE + 0x0044)
#define SPM_VCORE_DVFS_SHORTCUT_CON06  (SPM_BASE + 0x0048)
#define SPM_VCORE_DVFS_SHORTCUT_CON07  (SPM_BASE + 0x004C)
#define SPM_VCORE_DVFS_SHORTCUT_CON08  (SPM_BASE + 0x0050)
#define SPM_VCORE_DVFS_SHORTCUT_CON09  (SPM_BASE + 0x0054)
#define SPM_VCORE_DVFS_SHORTCUT_CON10  (SPM_BASE + 0x0058)
#define SPM_VCORE_DVFS_SHORTCUT_CON11  (SPM_BASE + 0x005C)
#define SPM_VCORE_DVFS_SHORTCUT_CON12  (SPM_BASE + 0x0060)
#define SPM_VCORE_DVFS_SHORTCUT_CON13  (SPM_BASE + 0x0064)
#define SPM_VCORE_DVFS_SHORTCUT_CON14  (SPM_BASE + 0x0068)
#define SPM_VCORE_DVFS_SHORTCUT_CON15  (SPM_BASE + 0x006C)
#define SPM_VCORE_DVFS_SHORTCUT_CON16  (SPM_BASE + 0x0070)
#define SPM_VCORE_DVFS_SHORTCUT_CON17  (SPM_BASE + 0x0074)
#define SPM_VCORE_DVFS_SHORTCUT_CON18  (SPM_BASE + 0x0078)
#define SPM_VCORE_DVFS_SHORTCUT_CON19  (SPM_BASE + 0x007C)
#define SPM_VCORE_DVFS_SHORTCUT_CON20  (SPM_BASE + 0x0080)
#define SPM_VCORE_DVFS_SHORTCUT_CON21  (SPM_BASE + 0x0084)
#define SPM_VCORE_DVFS_SHORTCUT_CON22  (SPM_BASE + 0x0088)
#define SPM_VCORE_DVFS_SHORTCUT_CON23  (SPM_BASE + 0x008C)
#define SPM_VCORE_DVFS_SHORTCUT_CON24  (SPM_BASE + 0x0090)
#define SPM_VCORE_DVFS_SHORTCUT_CON25  (SPM_BASE + 0x0094)
#define SPM_VCORE_DVFS_SHORTCUT_CON26  (SPM_BASE + 0x0098)
#define SPM_VCORE_DVFS_SHORTCUT_CON27  (SPM_BASE + 0x009C)
#define SPM_VCORE_DVFS_SHORTCUT_CON28  (SPM_BASE + 0x00A0)
#define SPM_VCORE_DVFS_SHORTCUT_CON29  (SPM_BASE + 0x00A4)
#define SPM_VCORE_DVFS_SHORTCUT_CON30  (SPM_BASE + 0x00A8)
#define SPM_VCORE_DVFS_SHORTCUT_CON31  (SPM_BASE + 0x00AC)
#define SPM_CPU_WAKEUP_EVENT           (SPM_BASE + 0x00B0)
#define SPM_IRQ_MASK                   (SPM_BASE + 0x00B4)
#define SPM_SRC_REQ                    (SPM_BASE + 0x00B8)
#define SPM_SRC_MASK                   (SPM_BASE + 0x00BC)
#define SPM_SRC2_MASK                  (SPM_BASE + 0x00C0)
#define SPM_SRC3_MASK                  (SPM_BASE + 0x00C4)
#define SPM_SRC4_MASK                  (SPM_BASE + 0x00C8)
#define SPM_SRC5_MASK                  (SPM_BASE + 0x00CC)
#define SPM_WAKEUP_EVENT_MASK          (SPM_BASE + 0x00D0)
#define SPM_WAKEUP_EVENT_EXT_MASK      (SPM_BASE + 0x00D4)
#define SPM_TWAM_EVENT_CLEAR           (SPM_BASE + 0x00D8)
#define SPM_SRC6_MASK                  (SPM_BASE + 0x00DC)
#define PCM_DEBUG_CON                  (SPM_BASE + 0x00E0)
#define AHB_BUS_CON                    (SPM_BASE + 0x00E4)
#define DDR_EN_DBC_CON0                (SPM_BASE + 0x00E8)
#define DDR_EN_DBC_CON1                (SPM_BASE + 0x00EC)
#define SPM_RESOURCE_ACK_CON0          (SPM_BASE + 0x00F0)
#define SPM_RESOURCE_ACK_CON1          (SPM_BASE + 0x00F4)
#define SPM_RESOURCE_ACK_CON2          (SPM_BASE + 0x00F8)
#define SPM_RESOURCE_ACK_CON3          (SPM_BASE + 0x00FC)
#define PCM_REG0_DATA                  (SPM_BASE + 0x0100)
#define PCM_REG2_DATA                  (SPM_BASE + 0x0104)
#define PCM_REG6_DATA                  (SPM_BASE + 0x0108)
#define PCM_REG7_DATA                  (SPM_BASE + 0x010C)
#define PCM_REG13_DATA                 (SPM_BASE + 0x0110)
#define SRC_REQ_STA_0                  (SPM_BASE + 0x0114)
#define SRC_REQ_STA_1                  (SPM_BASE + 0x0118)
#define SRC_REQ_STA_2                  (SPM_BASE + 0x011C)
#define SRC_REQ_STA_3                  (SPM_BASE + 0x0120)
#define SRC_REQ_STA_4                  (SPM_BASE + 0x0124)
#define PCM_TIMER_OUT                  (SPM_BASE + 0x0128)
#define PCM_WDT_OUT                    (SPM_BASE + 0x012C)
#define SPM_IRQ_STA                    (SPM_BASE + 0x0130)
#define SUBSYS_IDLE_STA                (SPM_BASE + 0x0134)
#define MD32PCM_WAKEUP_STA             (SPM_BASE + 0x0138)
#define MD32PCM_EVENT_STA              (SPM_BASE + 0x013C)
#define SPM_WAKEUP_STA                 (SPM_BASE + 0x0140)
#define SPM_WAKEUP_EXT_STA             (SPM_BASE + 0x0144)
#define SPM_WAKEUP_MISC                (SPM_BASE + 0x0148)
#define BUS_PROTECT_RDY                (SPM_BASE + 0x014C)
#define BUS_PROTECT2_RDY               (SPM_BASE + 0x0150)
#define BUS_PROTECT3_RDY               (SPM_BASE + 0x0154)
#define BUS_PROTECT4_RDY               (SPM_BASE + 0x0158)
#define PCM_STA                        (SPM_BASE + 0x015C)
#define PWR_STATUS                     (SPM_BASE + 0x0160)
#define PWR_STATUS_2ND                 (SPM_BASE + 0x0164)
#define CPU_PWR_STATUS                 (SPM_BASE + 0x0168)
#define SPM_SRC_RDY_STA                (SPM_BASE + 0x016C)
#define SPM_TWAM_LAST_STA0             (SPM_BASE + 0x0170)
#define SPM_TWAM_LAST_STA1             (SPM_BASE + 0x0174)
#define SPM_TWAM_LAST_STA2             (SPM_BASE + 0x0178)
#define SPM_TWAM_LAST_STA3             (SPM_BASE + 0x017C)
#define SPM_TWAM_CURR_STA0             (SPM_BASE + 0x0180)
#define SPM_TWAM_CURR_STA1             (SPM_BASE + 0x0184)
#define SPM_TWAM_CURR_STA2             (SPM_BASE + 0x0188)
#define SPM_TWAM_CURR_STA3             (SPM_BASE + 0x018C)
#define SPM_TWAM_TIMER_OUT             (SPM_BASE + 0x0190)
#define SPM_TWAM_CON                   (SPM_BASE + 0x0194)
#define SPM_TWAM_WINDOW_LEN            (SPM_BASE + 0x0198)
#define SPM_TWAM_IDLE_SEL              (SPM_BASE + 0x019C)
#define MBIST_EFUSE_REPAIR_ACK_STA     (SPM_BASE + 0x01A0)
#define SPM_PC_STA                     (SPM_BASE + 0x01A4)
#define DVFSRC_EVENT_STA               (SPM_BASE + 0x01A8)
#define MCUSYS_IDLE_STA                (SPM_BASE + 0x01AC)
#define GIC_WAKEUP_STA                 (SPM_BASE + 0x01B0)
#define SYS_TIMER_OUT_L                (SPM_BASE + 0x01B4)
#define SYS_TIMER_OUT_H                (SPM_BASE + 0x01B8)
#define SPM_CG_CHECK_STA               (SPM_BASE + 0x01BC)
#define SPM_DVFS_HISTORY_STA0          (SPM_BASE + 0x01C0)
#define SPM_DVFS_HISTORY_STA1          (SPM_BASE + 0x01C4)
#define SPM_DDREN_SLEEP_COUNT          (SPM_BASE + 0x01C8)
#define SPM_DDREN_WAKE_COUNT           (SPM_BASE + 0x01CC)
#define SPM_APSRC_COUNT                (SPM_BASE + 0x01D0)
#define SPM_VRF18_COUNT                (SPM_BASE + 0x01D4)
#define SPM_INFRA_COUNT                (SPM_BASE + 0x01D8)
#define SPM_26M_COUNT                  (SPM_BASE + 0x01DC)
#define MCUSYS_PWR_CON                 (SPM_BASE + 0x0200)
#define MP0_CPUTOP_PWR_CON             (SPM_BASE + 0x0204)
#define MP0_CPU0_PWR_CON               (SPM_BASE + 0x0208)
#define MP0_CPU1_PWR_CON               (SPM_BASE + 0x020C)
#define MP0_CPU2_PWR_CON               (SPM_BASE + 0x0210)
#define MP0_CPU3_PWR_CON               (SPM_BASE + 0x0214)
#define MP0_CPU4_PWR_CON               (SPM_BASE + 0x0218)
#define MP0_CPU5_PWR_CON               (SPM_BASE + 0x021C)
#define MP0_CPU6_PWR_CON               (SPM_BASE + 0x0220)
#define MP0_CPU7_PWR_CON               (SPM_BASE + 0x0224)
#define MP0_CPU0_WFI_EN                (SPM_BASE + 0x0228)
#define MP0_CPU1_WFI_EN                (SPM_BASE + 0x022C)
#define MP0_CPU2_WFI_EN                (SPM_BASE + 0x0230)
#define MP0_CPU3_WFI_EN                (SPM_BASE + 0x0234)
#define MP0_CPU4_WFI_EN                (SPM_BASE + 0x0238)
#define MP0_CPU5_WFI_EN                (SPM_BASE + 0x023C)
#define MP0_CPU6_WFI_EN                (SPM_BASE + 0x0240)
#define MP0_CPU7_WFI_EN                (SPM_BASE + 0x0244)
#define ARMPLL_CLK_SEL                 (SPM_BASE + 0x0248)
#define ARMPLL_CLK_CON                 (SPM_BASE + 0x024C)
#define CPU_SPARE_CON                  (SPM_BASE + 0x0250)
#define CPU_SPARE_CON_SET              (SPM_BASE + 0x0254)
#define CPU_SPARE_CON_CLR              (SPM_BASE + 0x0258)
#define EXT_INT_WAKEUP_REQ             (SPM_BASE + 0x025C)
#define EXT_INT_WAKEUP_REQ_SET         (SPM_BASE + 0x0260)
#define EXT_INT_WAKEUP_REQ_CLR         (SPM_BASE + 0x0264)
#define ROOT_CPUTOP_ADDR               (SPM_BASE + 0x0268)
#define ROOT_CORE_ADDR                 (SPM_BASE + 0x026C)
#define SYS_TIMER_CON                  (SPM_BASE + 0x0274)
#define SYS_TIMER_START_POINT_L        (SPM_BASE + 0x0278)
#define SYS_TIMER_START_POINT_H        (SPM_BASE + 0x027C)
#define SYS_TIMER_LATCH_OUT_0_L        (SPM_BASE + 0x0280)
#define SYS_TIMER_LATCH_OUT_0_H        (SPM_BASE + 0x0284)
#define SYS_TIMER_LATCH_OUT_1_L        (SPM_BASE + 0x0288)
#define SYS_TIMER_LATCH_OUT_1_H        (SPM_BASE + 0x028C)
#define SYS_TIMER_LATCH_OUT_2_L        (SPM_BASE + 0x0290)
#define SYS_TIMER_LATCH_OUT_2_H        (SPM_BASE + 0x0294)
#define SYS_TIMER_LATCH_OUT_3_L        (SPM_BASE + 0x0298)
#define SYS_TIMER_LATCH_OUT_3_H        (SPM_BASE + 0x029C)
#define SYS_TIMER_LATCH_OUT_4_L        (SPM_BASE + 0x02A0)
#define SYS_TIMER_LATCH_OUT_4_H        (SPM_BASE + 0x02A4)
#define SYS_TIMER_LATCH_OUT_5_L        (SPM_BASE + 0x02A8)
#define SYS_TIMER_LATCH_OUT_5_H        (SPM_BASE + 0x02AC)
#define SYS_TIMER_LATCH_OUT_6_L        (SPM_BASE + 0x02B0)
#define SYS_TIMER_LATCH_OUT_6_H        (SPM_BASE + 0x02B4)
#define SYS_TIMER_LATCH_OUT_7_L        (SPM_BASE + 0x02B8)
#define SYS_TIMER_LATCH_OUT_7_H        (SPM_BASE + 0x02BC)
#define SYS_TIMER_LATCH_OUT_8_L        (SPM_BASE + 0x02C0)
#define SYS_TIMER_LATCH_OUT_8_H        (SPM_BASE + 0x02C4)
#define SYS_TIMER_LATCH_OUT_9_L        (SPM_BASE + 0x02C8)
#define SYS_TIMER_LATCH_OUT_9_H        (SPM_BASE + 0x02CC)
#define SYS_TIMER_LATCH_OUT_10_L       (SPM_BASE + 0x02D0)
#define SYS_TIMER_LATCH_OUT_10_H       (SPM_BASE + 0x02D4)
#define SYS_TIMER_LATCH_OUT_11_L       (SPM_BASE + 0x02D8)
#define SYS_TIMER_LATCH_OUT_11_H       (SPM_BASE + 0x02DC)
#define SYS_TIMER_LATCH_OUT_12_L       (SPM_BASE + 0x02E0)
#define SYS_TIMER_LATCH_OUT_12_H       (SPM_BASE + 0x02E4)
#define SYS_TIMER_LATCH_OUT_13_L       (SPM_BASE + 0x02E8)
#define SYS_TIMER_LATCH_OUT_13_H       (SPM_BASE + 0x02EC)
#define SYS_TIMER_LATCH_OUT_14_L       (SPM_BASE + 0x02F0)
#define SYS_TIMER_LATCH_OUT_14_H       (SPM_BASE + 0x02F4)
#define SYS_TIMER_LATCH_OUT_15_L       (SPM_BASE + 0x02F8)
#define SYS_TIMER_LATCH_OUT_15_H       (SPM_BASE + 0x02FC)
#define VDE_PWR_CON                    (SPM_BASE + 0x0300)
#define VEN_PWR_CON                    (SPM_BASE + 0x0304)
#define ISP_PWR_CON                    (SPM_BASE + 0x0308)
#define DIS_PWR_CON                    (SPM_BASE + 0x030C)
#define IFR_PWR_CON                    (SPM_BASE + 0x0310)
#define DPY_PWR_CON                    (SPM_BASE + 0x0314)
#define MD1_PWR_CON                    (SPM_BASE + 0x0318)
#define AUDIO_PWR_CON                  (SPM_BASE + 0x031C)
#define CONN_PWR_CON                   (SPM_BASE + 0x0320)
#define CAM_PWR_CON                    (SPM_BASE + 0x0324)
#define MFG0_PWR_CON                   (SPM_BASE + 0x0328)
#define MFG1_PWR_CON                   (SPM_BASE + 0x032C)
#define MFG2_PWR_CON                   (SPM_BASE + 0x0330)
#define MFG3_PWR_CON                   (SPM_BASE + 0x0334)
#define MFG4_PWR_CON                   (SPM_BASE + 0x0338)
#define VPU_VCORE_PWR_CON              (SPM_BASE + 0x033C)
#define VPU_CONN_PWR_CON               (SPM_BASE + 0x0340)
#define VPU_CORE0_PWR_CON              (SPM_BASE + 0x0344)
#define VPU_CORE1_PWR_CON              (SPM_BASE + 0x0348)
#define VPU_CORE2_PWR_CON              (SPM_BASE + 0x034C)
#define IPE_PWR_CON                    (SPM_BASE + 0x0350)
#define SYSRAM_CON                     (SPM_BASE + 0x0354)
#define SYSROM_CON                     (SPM_BASE + 0x0358)
#define SSPM_SRAM_CON                  (SPM_BASE + 0x035C)
#define SCP_SRAM_CON                   (SPM_BASE + 0x0360)
#define AUDIO_DSP_CON0                 (SPM_BASE + 0x0364)
#define AUDIO_DSP_CON1                 (SPM_BASE + 0x0368)
#define DPY_SHU_SRAM_CON               (SPM_BASE + 0x036C)
#define UFS_SRAM_CON                   (SPM_BASE + 0x0370)
#define PERMI_TBL_RD_SRAM_CON          (SPM_BASE + 0x0374)
#define PERMI_TBL_WR_SRAM_CON          (SPM_BASE + 0x0378)
#define INVD_TBL_RD_SRAM_CON           (SPM_BASE + 0x037C)
#define INVD_TBL_WR_SRAM_CON           (SPM_BASE + 0x0380)
#define VPU_VCORE_SRAM_CON             (SPM_BASE + 0x0384)
#define VPU_CONN_SRAM_CON              (SPM_BASE + 0x0388)
#define VPU_CORE0_SRAM_CON             (SPM_BASE + 0x038C)
#define VPU_CORE1_SRAM_CON             (SPM_BASE + 0x0390)
#define VPU_CORE2_SRAM_CON             (SPM_BASE + 0x0394)
#define VPU_CORE0_SRAM_STA             (SPM_BASE + 0x0398)
#define VPU_CORE1_SRAM_STA             (SPM_BASE + 0x039C)
#define VPU_CORE2_SRAM_STA             (SPM_BASE + 0x03A0)
#define DEBUGTOP_SRAM_CON              (SPM_BASE + 0x03A4)
#define USB_SRAM_CON                   (SPM_BASE + 0x03A8)
#define DUMMY_SRAM_CON                 (SPM_BASE + 0x03AC)
#define MD_EXT_BUCK_ISO_CON            (SPM_BASE + 0x03B0)
#define EXT_BUCK_ISO                   (SPM_BASE + 0x03B4)
#define DXCC_SRAM_CON                  (SPM_BASE + 0x03B8)
#define GCE_SRAM_CON                   (SPM_BASE + 0x03BC)
#define MSDC_SRAM_CON                  (SPM_BASE + 0x03C0)
#define SPM_MAS_PAUSE_MASK_B           (SPM_BASE + 0x0400)
#define SPM_MAS_PAUSE2_MASK_B          (SPM_BASE + 0x0404)
#define SPM_MAS_PAUSE3_MASK_B          (SPM_BASE + 0x0408)
#define SPM_MAS_PAUSE_MM_MASK_B        (SPM_BASE + 0x040C)
#define SPM_MAS_PAUSE_MCU_MASK_B       (SPM_BASE + 0x0410)
#define SPM2SW_MAILBOX_0               (SPM_BASE + 0x0414)
#define SPM2SW_MAILBOX_1               (SPM_BASE + 0x0418)
#define SPM2SW_MAILBOX_2               (SPM_BASE + 0x041C)
#define SPM2SW_MAILBOX_3               (SPM_BASE + 0x0420)
#define SW2SPM_MAILBOX_0               (SPM_BASE + 0x0424)
#define SW2SPM_MAILBOX_1               (SPM_BASE + 0x0428)
#define SW2SPM_MAILBOX_2               (SPM_BASE + 0x042C)
#define SW2SPM_MAILBOX_3               (SPM_BASE + 0x0430)
#define AP_MDSRC_REQ                   (SPM_BASE + 0x0434)
#define SPM2MD_DVFS_CON                (SPM_BASE + 0x0438)
#define MD2SPM_DVFS_CON                (SPM_BASE + 0x043C)
#define ULPOSC_CON                     (SPM_BASE + 0x0440)
#define SPM2CONN_MAILBOX               (SPM_BASE + 0x0444)
#define SPM_SWINT                      (SPM_BASE + 0x0448)
#define SPM_SWINT_SET                  (SPM_BASE + 0x044C)
#define SPM_SWINT_CLR                  (SPM_BASE + 0x0450)
#define AP2MD_PEER_WAKEUP              (SPM_BASE + 0x0454)
#define SPM_PLL_CON                    (SPM_BASE + 0x0458)
#define SPM_S1_MODE_CH                 (SPM_BASE + 0x045C)
#define DRAMC_DPY_CLK_SW_CON_SEL       (SPM_BASE + 0x0460)
#define DRAMC_DPY_CLK_SW_CON           (SPM_BASE + 0x0464)
#define DRAMC_DPY_CLK_SW_CON_SEL2      (SPM_BASE + 0x0468)
#define DRAMC_DPY_CLK_SW_CON2          (SPM_BASE + 0x046C)
#define DRAMC_DPY_CLK_SW_CON_SEL3      (SPM_BASE + 0x0470)
#define DRAMC_DPY_CLK_SW_CON3          (SPM_BASE + 0x0474)
#define DRAMC_DPY_CLK_SPM_CON          (SPM_BASE + 0x0478)
#define SPM_MEM_CK_SEL                 (SPM_BASE + 0x047C)
#define SPM_SEMA_M0                    (SPM_BASE + 0x0480)
#define SPM_SEMA_M1                    (SPM_BASE + 0x0484)
#define SPM_SEMA_M2                    (SPM_BASE + 0x0488)
#define SPM_SEMA_M3                    (SPM_BASE + 0x048C)
#define SPM_SEMA_M4                    (SPM_BASE + 0x0490)
#define SPM_SEMA_M5                    (SPM_BASE + 0x0494)
#define SPM_SEMA_M6                    (SPM_BASE + 0x0498)
#define SPM_SEMA_M7                    (SPM_BASE + 0x049C)
#define SPM_AP_SEMA                    (SPM_BASE + 0x04A0)
#define SPM_SPM_SEMA                   (SPM_BASE + 0x04A4)
#define SPM_SSPM_CON                   (SPM_BASE + 0x04A8)
#define SPM_SCP_CON                    (SPM_BASE + 0x04AC)
#define SPM_ADSP_CON                   (SPM_BASE + 0x04B0)
#define SPM2ADSP_MAILBOX               (SPM_BASE + 0x04B4)
#define ADSP2SPM_MAILBOX               (SPM_BASE + 0x04B8)
#define SPM_SCP_MAILBOX                (SPM_BASE + 0x04BC)
#define SCP_SPM_MAILBOX                (SPM_BASE + 0x04C0)
#define SPM2SSPM_MAILBOX_0             (SPM_BASE + 0x04C4)
#define SPM2SSPM_MAILBOX_1             (SPM_BASE + 0x04C8)
#define SPM2SSPM_MAILBOX_2             (SPM_BASE + 0x04CC)
#define SPM2SSPM_MAILBOX_3             (SPM_BASE + 0x04D0)
#define SSPM2SPM_MAILBOX_0             (SPM_BASE + 0x04D4)
#define SSPM2SPM_MAILBOX_1             (SPM_BASE + 0x04D8)
#define SSPM2SPM_MAILBOX_2             (SPM_BASE + 0x04DC)
#define SSPM2SPM_MAILBOX_3             (SPM_BASE + 0x04E0)
#define SPM_CIRQ_CON                   (SPM_BASE + 0x04E4)
#define SW2SPM_INT                     (SPM_BASE + 0x04E8)
#define SW2SPM_INT_SET                 (SPM_BASE + 0x04EC)
#define SW2SPM_INT_CLR                 (SPM_BASE + 0x04F0)
#define SPM_DVFS_MISC                  (SPM_BASE + 0x04F4)
#define SCP_VCORE_LEVEL                (SPM_BASE + 0x04F8)
#define CONN2SPM_MAILBOX               (SPM_BASE + 0x04FC)
#define SRCLKEN_RC_CFG                 (SPM_BASE + 0x0500)
#define RC_CENTRAL_CFG1                (SPM_BASE + 0x0504)
#define RC_CENTRAL_CFG2                (SPM_BASE + 0x0508)
#define RC_CMD_ARB_CFG                 (SPM_BASE + 0x050C)
#define RC_PMIC_RCEN_ADDR              (SPM_BASE + 0x0510)
#define RC_PMIC_RCEN_SET_CLR_ADDR      (SPM_BASE + 0x0514)
#define RC_DCXO_FPM_CFG                (SPM_BASE + 0x0518)
#define RC_CENTRAL_CFG3                (SPM_BASE + 0x051C)
#define RC_M00_SRCLKEN_CFG             (SPM_BASE + 0x0520)
#define RC_M01_SRCLKEN_CFG             (SPM_BASE + 0x0524)
#define RC_M02_SRCLKEN_CFG             (SPM_BASE + 0x0528)
#define RC_M03_SRCLKEN_CFG             (SPM_BASE + 0x052C)
#define RC_M04_SRCLKEN_CFG             (SPM_BASE + 0x0530)
#define RC_M05_SRCLKEN_CFG             (SPM_BASE + 0x0534)
#define RC_M06_SRCLKEN_CFG             (SPM_BASE + 0x0538)
#define RC_M07_SRCLKEN_CFG             (SPM_BASE + 0x053C)
#define RC_M08_SRCLKEN_CFG             (SPM_BASE + 0x0540)
#define RC_M09_SRCLKEN_CFG             (SPM_BASE + 0x0544)
#define RC_M10_SRCLKEN_CFG             (SPM_BASE + 0x0548)
#define RC_M11_SRCLKEN_CFG             (SPM_BASE + 0x054C)
#define RC_M12_SRCLKEN_CFG             (SPM_BASE + 0x0550)
#define RC_FSM_STA_0                   (SPM_BASE + 0x0560)
#define RC_CMD_STA_0                   (SPM_BASE + 0x0564)
#define RC_CMD_STA_1                   (SPM_BASE + 0x0568)
#define RC_SPI_STA_0                   (SPM_BASE + 0x056C)
#define RC_PI_PO_STA_0                 (SPM_BASE + 0x0570)
#define RC_M00_REQ_STA_0               (SPM_BASE + 0x0580)
#define RC_M01_REQ_STA_0               (SPM_BASE + 0x0584)
#define RC_M02_REQ_STA_0               (SPM_BASE + 0x0588)
#define RC_M03_REQ_STA_0               (SPM_BASE + 0x058C)
#define RC_M04_REQ_STA_0               (SPM_BASE + 0x0590)
#define RC_M05_REQ_STA_0               (SPM_BASE + 0x0594)
#define RC_M06_REQ_STA_0               (SPM_BASE + 0x0598)
#define RC_M07_REQ_STA_0               (SPM_BASE + 0x059C)
#define RC_M08_REQ_STA_0               (SPM_BASE + 0x05A0)
#define RC_M09_REQ_STA_0               (SPM_BASE + 0x05A4)
#define RC_M10_REQ_STA_0               (SPM_BASE + 0x05A8)
#define RC_M11_REQ_STA_0               (SPM_BASE + 0x05AC)
#define RC_M12_REQ_STA_0               (SPM_BASE + 0x05B0)
#define RC_MISC_0                      (SPM_BASE + 0x05B4)
#define RC_SPM_CTRL                    (SPM_BASE + 0x05B8)
#define SUBSYS_INTF_CFG                (SPM_BASE + 0x05BC)
#define RC_DEBUG_TRACE_0_LSB           (SPM_BASE + 0x05C0)
#define RC_DEBUG_TRACE_0_MSB           (SPM_BASE + 0x05C4)
#define RC_DEBUG_TRACE_1_LSB           (SPM_BASE + 0x05C8)
#define RC_DEBUG_TRACE_1_MSB           (SPM_BASE + 0x05CC)
#define RC_DEBUG_TRACE_2_LSB           (SPM_BASE + 0x05D0)
#define RC_DEBUG_TRACE_2_MSB           (SPM_BASE + 0x05D4)
#define RC_DEBUG_TRACE_3_LSB           (SPM_BASE + 0x05D8)
#define RC_DEBUG_TRACE_3_MSB           (SPM_BASE + 0x05DC)
#define RC_DEBUG_TRACE_4_LSB           (SPM_BASE + 0x05E0)
#define RC_DEBUG_TRACE_4_MSB           (SPM_BASE + 0x05E4)
#define RC_DEBUG_TRACE_5_LSB           (SPM_BASE + 0x05E8)
#define RC_DEBUG_TRACE_5_MSB           (SPM_BASE + 0x05EC)
#define RC_DEBUG_TRACE_6_LSB           (SPM_BASE + 0x05F0)
#define RC_DEBUG_TRACE_6_MSB           (SPM_BASE + 0x05F4)
#define RC_DEBUG_TRACE_7_LSB           (SPM_BASE + 0x05F8)
#define RC_DEBUG_TRACE_7_MSB           (SPM_BASE + 0x05FC)
#define SPM_SW_FLAG_0                  (SPM_BASE + 0x0600)
#define SPM_SW_FLAG_1                  (SPM_BASE + 0x0604)
#define SPM_SW_RSV_0                   (SPM_BASE + 0x0608)
#define SPM_SW_RSV_1                   (SPM_BASE + 0x060C)
#define SPM_SW_RSV_2                   (SPM_BASE + 0x0610)
#define SPM_SW_RSV_3                   (SPM_BASE + 0x0614)
#define SPM_SW_RSV_4                   (SPM_BASE + 0x0618)
#define SPM_SW_RSV_5                   (SPM_BASE + 0x061C)
#define SPM_SW_RSV_6                   (SPM_BASE + 0x0620)
#define SPM_SW_RSV_7                   (SPM_BASE + 0x0624)
#define SPM_SW_RSV_8                   (SPM_BASE + 0x0628)
#define SPM_SW_RSV_9                   (SPM_BASE + 0x062C)
#define SPM_SW_RSV_10                  (SPM_BASE + 0x0630)
#define SPM_SW_RSV_11                  (SPM_BASE + 0x0634)
#define SPM_SW_RSV_18                  (SPM_BASE + 0x0638)
#define SPM_SW_RSV_19                  (SPM_BASE + 0x063C)
#define SPM_RSV_CON_0                  (SPM_BASE + 0x0640)
#define SPM_RSV_CON_1                  (SPM_BASE + 0x0644)
#define SPM_RSV_STA_0                  (SPM_BASE + 0x0648)
#define SPM_RSV_STA_1                  (SPM_BASE + 0x064C)
#define SPM_SPARE_CON                  (SPM_BASE + 0x0650)
#define SPM_SPARE_CON_SET              (SPM_BASE + 0x0654)
#define SPM_SPARE_CON_CLR              (SPM_BASE + 0x0658)
#define SPM_DV_CON_0                   (SPM_BASE + 0x065C)
#define SPM_DV_CON_1                   (SPM_BASE + 0x0660)
#define SPM_FORCE_DVFS                 (SPM_BASE + 0x0664)
#define INFRA2SPM_DEEPIDLE_CG_CHECK_0_MASK (SPM_BASE + 0x0668)
#define INFRA2SPM_DEEPIDLE_CG_CHECK_1_MASK (SPM_BASE + 0x066C)
#define INFRA2SPM_DEEPIDLE_CG_CHECK_2_MASK (SPM_BASE + 0x0670)
#define INFRA2SPM_DEEPIDLE_CG_CHECK_3_MASK (SPM_BASE + 0x0674)
#define INFRA2SPM_DEEPIDLE_CG_CHECK_4_MASK (SPM_BASE + 0x0678)
#define INFRA2SPM_SODI_CG_CHECK_0_MASK (SPM_BASE + 0x067C)
#define INFRA2SPM_SODI_CG_CHECK_1_MASK (SPM_BASE + 0x0680)
#define INFRA2SPM_SODI_CG_CHECK_2_MASK (SPM_BASE + 0x0684)
#define INFRA2SPM_SODI_CG_CHECK_3_MASK (SPM_BASE + 0x0688)
#define INFRA2SPM_SODI_CG_CHECK_4_MASK (SPM_BASE + 0x068C)
#define INFRA2SPM_SODI3_CG_CHECK_0_MASK (SPM_BASE + 0x0690)
#define INFRA2SPM_SODI3_CG_CHECK_1_MASK (SPM_BASE + 0x0694)
#define INFRA2SPM_SODI3_CG_CHECK_2_MASK (SPM_BASE + 0x0698)
#define INFRA2SPM_SODI3_CG_CHECK_3_MASK (SPM_BASE + 0x069C)
#define INFRA2SPM_SODI3_CG_CHECK_4_MASK (SPM_BASE + 0x06A0)
#define INFRA2SPM_MCDSR_CG_CHECK_0_MASK (SPM_BASE + 0x06A4)
#define INFRA2SPM_MCDSR_CG_CHECK_1_MASK (SPM_BASE + 0x06A8)
#define INFRA2SPM_MCDSR_CG_CHECK_2_MASK (SPM_BASE + 0x06AC)
#define INFRA2SPM_MCDSR_CG_CHECK_3_MASK (SPM_BASE + 0x06B0)
#define INFRA2SPM_MCDSR_CG_CHECK_4_MASK (SPM_BASE + 0x06B4)
#define OTHER2SPM_CG_CHECK_MASK        (SPM_BASE + 0x06B8)
#define SPM_TIMER_0                    (SPM_BASE + 0x06BC)
#define SPM_TIMER_1                    (SPM_BASE + 0x06C0)
#define SPM_TIMER_2                    (SPM_BASE + 0x06C4)
#define VS1_PSR_MASK_B                 (SPM_BASE + 0x06C8)
#define VS2_PSR_MASK_B                 (SPM_BASE + 0x06CC)
#define UFS_PSRI_SW                    (SPM_BASE + 0x06D0)
#define UFS_PSRI_SW_SET                (SPM_BASE + 0x06D4)
#define UFS_PSRI_SW_CLR                (SPM_BASE + 0x06D8)
#define SPM_DVFS_CON                   (SPM_BASE + 0x0700)
#define SPM_DVFS_CON_STA               (SPM_BASE + 0x0704)
#define SPM_DVFS_LEVEL                 (SPM_BASE + 0x0708)
#define SPM_DVFS_STA                   (SPM_BASE + 0x070C)
#define SPM_DVFS_CMD0                  (SPM_BASE + 0x0710)
#define SPM_DVFS_CMD1                  (SPM_BASE + 0x0714)
#define SPM_DVFS_CMD2                  (SPM_BASE + 0x0718)
#define SPM_DVFS_CMD3                  (SPM_BASE + 0x071C)
#define SPM_DVFS_CMD4                  (SPM_BASE + 0x0720)
#define SPM_DVFS_CMD5                  (SPM_BASE + 0x0724)
#define SPM_DVFS_CMD6                  (SPM_BASE + 0x0728)
#define SPM_DVFS_CMD7                  (SPM_BASE + 0x072C)
#define SPM_DVFS_CMD8                  (SPM_BASE + 0x0730)
#define SPM_DVFS_CMD9                  (SPM_BASE + 0x0734)
#define SPM_DVFS_CMD10                 (SPM_BASE + 0x0738)
#define SPM_DVFS_CMD11                 (SPM_BASE + 0x073C)
#define SPM_DVFS_CMD12                 (SPM_BASE + 0x0740)
#define SPM_DVFS_CMD13                 (SPM_BASE + 0x0744)
#define SPM_DVFS_CMD14                 (SPM_BASE + 0x0748)
#define SPM_DVFS_CMD15                 (SPM_BASE + 0x074C)
#define SPM_DVFS_CMD16                 (SPM_BASE + 0x0750)
#define SPM_DVFS_CMD17                 (SPM_BASE + 0x0754)
#define SPM_DVFS_CMD18                 (SPM_BASE + 0x0758)
#define SPM_DVFS_CMD19                 (SPM_BASE + 0x075C)
#define SPM_DVFS_CMD20                 (SPM_BASE + 0x0760)
#define SPM_DVFS_CMD21                 (SPM_BASE + 0x0764)
#define SPM_DVFS_CMD22                 (SPM_BASE + 0x0768)
#define SPM_DVFS_CMD23                 (SPM_BASE + 0x076C)
#define SPM_VCORE_DVFS_SHORTCUT00      (SPM_BASE + 0x0770)
#define SPM_VCORE_DVFS_SHORTCUT01      (SPM_BASE + 0x0774)
#define SPM_VCORE_DVFS_SHORTCUT02      (SPM_BASE + 0x0778)
#define SPM_VCORE_DVFS_SHORTCUT03      (SPM_BASE + 0x077C)
#define SPM_VCORE_DVFS_SHORTCUT04      (SPM_BASE + 0x0780)
#define SPM_VCORE_DVFS_SHORTCUT05      (SPM_BASE + 0x0784)
#define SPM_VCORE_DVFS_SHORTCUT06      (SPM_BASE + 0x0788)
#define SPM_VCORE_DVFS_SHORTCUT07      (SPM_BASE + 0x078C)
#define SPM_VCORE_DVFS_SHORTCUT08      (SPM_BASE + 0x0790)
#define SPM_VCORE_DVFS_SHORTCUT09      (SPM_BASE + 0x0794)
#define SPM_VCORE_DVFS_SHORTCUT10      (SPM_BASE + 0x0798)
#define SPM_VCORE_DVFS_SHORTCUT11      (SPM_BASE + 0x079C)
#define SPM_VCORE_DVFS_SHORTCUT12      (SPM_BASE + 0x07A0)
#define SPM_VCORE_DVFS_SHORTCUT13      (SPM_BASE + 0x07A4)
#define SPM_VCORE_DVFS_SHORTCUT14      (SPM_BASE + 0x07A8)
#define SPM_VCORE_DVFS_SHORTCUT15      (SPM_BASE + 0x07AC)
#define SPM_VCORE_DVFS_SHORTCUT_STA0   (SPM_BASE + 0x07B0)
#define SPM_VCORE_DVFS_SHORTCUT_STA1   (SPM_BASE + 0x07B4)
#define SPM_VCORE_DVFS_SHORTCUT_IN     (SPM_BASE + 0x07B8)
#define SPM_DVS_DFS_LEVEL              (SPM_BASE + 0x07BC)
#define PCM_WDT_LATCH_0                (SPM_BASE + 0x0800)
#define PCM_WDT_LATCH_1                (SPM_BASE + 0x0804)
#define PCM_WDT_LATCH_2                (SPM_BASE + 0x0808)
#define PCM_WDT_LATCH_3                (SPM_BASE + 0x080C)
#define PCM_WDT_LATCH_4                (SPM_BASE + 0x0810)
#define PCM_WDT_LATCH_5                (SPM_BASE + 0x0814)
#define PCM_WDT_LATCH_6                (SPM_BASE + 0x0818)
#define PCM_WDT_LATCH_7                (SPM_BASE + 0x081C)
#define PCM_WDT_LATCH_8                (SPM_BASE + 0x0820)
#define PCM_WDT_LATCH_9                (SPM_BASE + 0x0824)
#define PCM_WDT_LATCH_10               (SPM_BASE + 0x0828)
#define PCM_WDT_LATCH_11               (SPM_BASE + 0x082C)
#define PCM_WDT_LATCH_12               (SPM_BASE + 0x0830)
#define PCM_WDT_LATCH_13               (SPM_BASE + 0x0834)
#define PCM_WDT_LATCH_14               (SPM_BASE + 0x0838)
#define PCM_WDT_LATCH_15               (SPM_BASE + 0x083C)
#define PCM_WDT_LATCH_16               (SPM_BASE + 0x0840)
#define PCM_WDT_LATCH_17               (SPM_BASE + 0x0844)
#define PCM_WDT_LATCH_18               (SPM_BASE + 0x0848)
#define DVFSRC_IRQ_LATCH_0             (SPM_BASE + 0x084C)
#define DVFSRC_IRQ_LATCH_1             (SPM_BASE + 0x0850)
#define DRAMC_GATING_ERR_LATCH_CH0_0   (SPM_BASE + 0x0854)
#define DRAMC_GATING_ERR_LATCH_CH0_1   (SPM_BASE + 0x0858)
#define DRAMC_GATING_ERR_LATCH_CH0_2   (SPM_BASE + 0x085C)
#define DRAMC_GATING_ERR_LATCH_CH0_3   (SPM_BASE + 0x0860)
#define DRAMC_GATING_ERR_LATCH_CH0_4   (SPM_BASE + 0x0864)
#define DRAMC_GATING_ERR_LATCH_CH0_5   (SPM_BASE + 0x0868)
#define DRAMC_GATING_ERR_LATCH_CH0_6   (SPM_BASE + 0x086C)
#define DRAMC_GATING_ERR_LATCH_CH0_7   (SPM_BASE + 0x0870)
#define DRAMC_GATING_ERR_LATCH_CH0_8   (SPM_BASE + 0x0874)
#define DRAMC_GATING_ERR_LATCH_CH1_0   (SPM_BASE + 0x0878)
#define DRAMC_GATING_ERR_LATCH_CH1_1   (SPM_BASE + 0x087C)
#define DRAMC_GATING_ERR_LATCH_CH1_2   (SPM_BASE + 0x0880)
#define DRAMC_GATING_ERR_LATCH_CH1_3   (SPM_BASE + 0x0884)
#define DRAMC_GATING_ERR_LATCH_CH1_4   (SPM_BASE + 0x0888)
#define DRAMC_GATING_ERR_LATCH_CH1_5   (SPM_BASE + 0x088C)
#define DRAMC_GATING_ERR_LATCH_CH1_6   (SPM_BASE + 0x0890)
#define DRAMC_GATING_ERR_LATCH_CH1_7   (SPM_BASE + 0x0894)
#define DRAMC_GATING_ERR_LATCH_CH1_8   (SPM_BASE + 0x0898)
#define PCM_WDT_LATCH_SPARE_0          (SPM_BASE + 0x089C)
#define PCM_WDT_LATCH_SPARE_1          (SPM_BASE + 0x08A0)
#define PCM_WDT_LATCH_SPARE_2          (SPM_BASE + 0x08A4)
#define DRAMC_GATING_ERR_LATCH_SPARE_0 (SPM_BASE + 0x08A8)
#define PCM_WDT_LATCH_19               (SPM_BASE + 0x08B0)
#define PCM_WDT_LATCH_20               (SPM_BASE + 0x08B4)
#define PCM_WDT_LATCH_21               (SPM_BASE + 0x08B8)
#define PCM_WDT_LATCH_22               (SPM_BASE + 0x08BC)
#define PCM_WDT_LATCH_23               (SPM_BASE + 0x08C0)
#define PCM_WDT_LATCH_24               (SPM_BASE + 0x08C4)
#define PCM_WDT_LATCH_25               (SPM_BASE + 0x08C8)
#define PCM_WDT_LATCH_26               (SPM_BASE + 0x08CC)
#define PCM_WDT_LATCH_27               (SPM_BASE + 0x08D0)
#define PCM_WDT_LATCH_28               (SPM_BASE + 0x08D4)
#define PCM_WDT_LATCH_29               (SPM_BASE + 0x08D8)
#define PCM_WDT_LATCH_30               (SPM_BASE + 0x08DC)
#define PCM_WDT_LATCH_31               (SPM_BASE + 0x08E0)
#define PCM_WDT_LATCH_32               (SPM_BASE + 0x08E4)
#define PCM_WDT_LATCH_33               (SPM_BASE + 0x08E8)
#define PCM_WDT_LATCH_34               (SPM_BASE + 0x08EC)
#define PCM_WDT_LATCH_35               (SPM_BASE + 0x08F0)
#define PCM_WDT_LATCH_36               (SPM_BASE + 0x08F4)
#define PCM_WDT_LATCH_37               (SPM_BASE + 0x08F8)
#define PCM_WDT_LATCH_38               (SPM_BASE + 0x08FC)
#define SPM_ACK_CHK_CON_0              (SPM_BASE + 0x0900)
#define SPM_ACK_CHK_PC_0               (SPM_BASE + 0x0904)
#define SPM_ACK_CHK_SEL_0              (SPM_BASE + 0x0908)
#define SPM_ACK_CHK_TIMER_0            (SPM_BASE + 0x090C)
#define SPM_ACK_CHK_STA_0              (SPM_BASE + 0x0910)
#define SPM_ACK_CHK_SWINT_0            (SPM_BASE + 0x0914)
#define SPM_ACK_CHK_CON_1              (SPM_BASE + 0x0918)
#define SPM_ACK_CHK_PC_1               (SPM_BASE + 0x091C)
#define SPM_ACK_CHK_SEL_1              (SPM_BASE + 0x0920)
#define SPM_ACK_CHK_TIMER_1            (SPM_BASE + 0x0924)
#define SPM_ACK_CHK_STA_1              (SPM_BASE + 0x0928)
#define SPM_ACK_CHK_SWINT_1            (SPM_BASE + 0x092C)
#define SPM_ACK_CHK_CON_2              (SPM_BASE + 0x0930)
#define SPM_ACK_CHK_PC_2               (SPM_BASE + 0x0934)
#define SPM_ACK_CHK_SEL_2              (SPM_BASE + 0x0938)
#define SPM_ACK_CHK_TIMER_2            (SPM_BASE + 0x093C)
#define SPM_ACK_CHK_STA_2              (SPM_BASE + 0x0940)
#define SPM_ACK_CHK_SWINT_2            (SPM_BASE + 0x0944)
#define SPM_ACK_CHK_CON_3              (SPM_BASE + 0x0948)
#define SPM_ACK_CHK_PC_3               (SPM_BASE + 0x094C)
#define SPM_ACK_CHK_SEL_3              (SPM_BASE + 0x0950)
#define SPM_ACK_CHK_TIMER_3            (SPM_BASE + 0x0954)
#define SPM_ACK_CHK_STA_3              (SPM_BASE + 0x0958)
#define SPM_ACK_CHK_SWINT_3            (SPM_BASE + 0x095C)
#define SPM_ACK_CHK_CON_4              (SPM_BASE + 0x0960)
#define SPM_ACK_CHK_PC_4               (SPM_BASE + 0x0964)
#define SPM_ACK_CHK_SEL_4              (SPM_BASE + 0x0968)
#define SPM_ACK_CHK_TIMER_4            (SPM_BASE + 0x096C)
#define SPM_ACK_CHK_STA_4              (SPM_BASE + 0x0970)
#define SPM_ACK_CHK_SWINT_4            (SPM_BASE + 0x0974)
#define SPM_PMSR_CURR_STA0             (SPM_BASE + 0x0E00)
#define SPM_PMSR_CURR_STA1             (SPM_BASE + 0x0E04)
#define SPM_PMSR_CURR_STA2             (SPM_BASE + 0x0E08)
#define SPM_PMSR_CURR_STA3             (SPM_BASE + 0x0E0C)
#define SPM_PMSR_CURR_STA4             (SPM_BASE + 0x0E10)
#define SPM_PMSR_CURR_STA5             (SPM_BASE + 0x0E14)
#define SPM_PMSR_CURR_STA6             (SPM_BASE + 0x0E18)
#define SPM_PMSR_CURR_STA7             (SPM_BASE + 0x0E1C)
#define SPM_PMSR_CURR_STA8             (SPM_BASE + 0x0E20)
#define SPM_PMSR_CURR_STA9             (SPM_BASE + 0x0E24)
#define SPM_PMSR_CURR_STA10            (SPM_BASE + 0x0E28)
#define SPM_PMSR_CURR_STA11            (SPM_BASE + 0x0E2C)
#define SPM_PMSR_CURR_STA12            (SPM_BASE + 0x0E30)
#define SPM_PMSR_CURR_STA13            (SPM_BASE + 0x0E34)
#define SPM_PMSR_CURR_STA14            (SPM_BASE + 0x0E38)
#define SPM_PMSR_CURR_STA15            (SPM_BASE + 0x0E3C)
#define SPM_PMSR_CURR_STA16            (SPM_BASE + 0x0E40)
#define SPM_PMSR_CURR_STA17            (SPM_BASE + 0x0E44)
#define SPM_PMSR_CURR_STA18            (SPM_BASE + 0x0E48)
#define SPM_PMSR_CURR_STA19            (SPM_BASE + 0x0E4C)
#define SPM_PMSR_CURR_STA20            (SPM_BASE + 0x0E50)
#define SPM_PMSR_CURR_STA21            (SPM_BASE + 0x0E54)
#define SPM_PMSR_CURR_STA22            (SPM_BASE + 0x0E58)
#define SPM_PMSR_CURR_STA23            (SPM_BASE + 0x0E5C)
#define SPM_PMSR_CURR_STA24            (SPM_BASE + 0x0E60)
#define SPM_PMSR_CURR_STA25            (SPM_BASE + 0x0E64)
#define SPM_PMSR_CURR_STA26            (SPM_BASE + 0x0E68)
#define SPM_PMSR_CURR_STA27            (SPM_BASE + 0x0E6C)
#define SPM_PMSR_CURR_STA28            (SPM_BASE + 0x0E70)
#define SPM_PMSR_CURR_STA29            (SPM_BASE + 0x0E74)
#define SPM_PMSR_CURR_STA30            (SPM_BASE + 0x0E78)
#define SPM_PMSR_CURR_STA31            (SPM_BASE + 0x0E7C)
#define SPM_PMSR_CURR_STA32            (SPM_BASE + 0x0E80)
#define SPM_PMSR_CURR_STA33            (SPM_BASE + 0x0E84)
#define SPM_PMSR_CURR_STA34            (SPM_BASE + 0x0E88)
#define SPM_PMSR_CURR_STA35            (SPM_BASE + 0x0E8C)
#define SPM_PMSR_CURR_STA36            (SPM_BASE + 0x0E90)
#define SPM_PMSR_CURR_STA37            (SPM_BASE + 0x0E94)
#define SPM_PMSR_CURR_STA38            (SPM_BASE + 0x0E98)
#define SPM_PMSR_CURR_STA39            (SPM_BASE + 0x0E9C)
#define SPM_PMSR_CURR_STA40            (SPM_BASE + 0x0EA0)
#define SPM_PMSR_CURR_STA41            (SPM_BASE + 0x0EA4)
#define SPM_PMSR_CURR_STA42            (SPM_BASE + 0x0EA8)
#define SPM_PMSR_CURR_STA43            (SPM_BASE + 0x0EAC)
#define SPM_PMSR_CURR_STA44            (SPM_BASE + 0x0EB0)
#define SPM_PMSR_CURR_STA45            (SPM_BASE + 0x0EB4)
#define SPM_PMSR_CURR_STA46            (SPM_BASE + 0x0EB8)
#define SPM_PMSR_CURR_STA47            (SPM_BASE + 0x0EBC)
#define SPM_PMSR_TIEMR_STA0            (SPM_BASE + 0x0EC0)
#define SPM_PMSR_TIEMR_STA1            (SPM_BASE + 0x0EC4)
#define SPM_PMSR_TIEMR_STA2            (SPM_BASE + 0x0EC8)
#define SPM_PMSR_GENERAL_CON0          (SPM_BASE + 0x0ECC)
#define SPM_PMSR_GENERAL_CON1          (SPM_BASE + 0x0ED0)
#define SPM_PMSR_GENERAL_CON2          (SPM_BASE + 0x0ED4)
#define SPM_PMSR_GENERAL_CON3          (SPM_BASE + 0x0ED8)
#define SPM_PMSR_GENERAL_CON4          (SPM_BASE + 0x0EDC)
#define SPM_PMSR_GENERAL_CON5          (SPM_BASE + 0x0EE0)
#define SPM_PMSR_GENERAL_CON6          (SPM_BASE + 0x0EE4)
#define SPM_PMSR_SW_RESET              (SPM_BASE + 0x0EE8)
#define SPM_PMSR_LAST_STA0             (SPM_BASE + 0x0F00)
#define SPM_PMSR_LAST_STA1             (SPM_BASE + 0x0F04)
#define SPM_PMSR_LAST_STA2             (SPM_BASE + 0x0F08)
#define SPM_PMSR_LAST_STA3             (SPM_BASE + 0x0F0C)
#define SPM_PMSR_LAST_STA4             (SPM_BASE + 0x0F10)
#define SPM_PMSR_LAST_STA5             (SPM_BASE + 0x0F14)
#define SPM_PMSR_LAST_STA6             (SPM_BASE + 0x0F18)
#define SPM_PMSR_LAST_STA7             (SPM_BASE + 0x0F1C)
#define SPM_PMSR_LAST_STA8             (SPM_BASE + 0x0F20)
#define SPM_PMSR_LAST_STA9             (SPM_BASE + 0x0F24)
#define SPM_PMSR_LAST_STA10            (SPM_BASE + 0x0F28)
#define SPM_PMSR_LAST_STA11            (SPM_BASE + 0x0F2C)
#define SPM_PMSR_LAST_STA12            (SPM_BASE + 0x0F30)
#define SPM_PMSR_LAST_STA13            (SPM_BASE + 0x0F34)
#define SPM_PMSR_LAST_STA14            (SPM_BASE + 0x0F38)
#define SPM_PMSR_LAST_STA15            (SPM_BASE + 0x0F3C)
#define SPM_PMSR_LAST_STA16            (SPM_BASE + 0x0F40)
#define SPM_PMSR_LAST_STA17            (SPM_BASE + 0x0F44)
#define SPM_PMSR_LAST_STA18            (SPM_BASE + 0x0F48)
#define SPM_PMSR_LAST_STA19            (SPM_BASE + 0x0F4C)
#define SPM_PMSR_LAST_STA20            (SPM_BASE + 0x0F50)
#define SPM_PMSR_LAST_STA21            (SPM_BASE + 0x0F54)
#define SPM_PMSR_LAST_STA22            (SPM_BASE + 0x0F58)
#define SPM_PMSR_LAST_STA23            (SPM_BASE + 0x0F5C)
#define SPM_PMSR_LAST_STA24            (SPM_BASE + 0x0F60)
#define SPM_PMSR_LAST_STA25            (SPM_BASE + 0x0F64)
#define SPM_PMSR_LAST_STA26            (SPM_BASE + 0x0F68)
#define SPM_PMSR_LAST_STA27            (SPM_BASE + 0x0F6C)
#define SPM_PMSR_LAST_STA28            (SPM_BASE + 0x0F70)
#define SPM_PMSR_LAST_STA29            (SPM_BASE + 0x0F74)
#define SPM_PMSR_LAST_STA30            (SPM_BASE + 0x0F78)
#define SPM_PMSR_LAST_STA31            (SPM_BASE + 0x0F7C)
#define SPM_PMSR_LAST_STA32            (SPM_BASE + 0x0F80)
#define SPM_PMSR_LAST_STA33            (SPM_BASE + 0x0F84)
#define SPM_PMSR_LAST_STA34            (SPM_BASE + 0x0F88)
#define SPM_PMSR_LAST_STA35            (SPM_BASE + 0x0F8C)
#define SPM_PMSR_LAST_STA36            (SPM_BASE + 0x0F90)
#define SPM_PMSR_LAST_STA37            (SPM_BASE + 0x0F94)
#define SPM_PMSR_LAST_STA38            (SPM_BASE + 0x0F98)
#define SPM_PMSR_LAST_STA39            (SPM_BASE + 0x0F9C)
#define SPM_PMSR_LAST_STA40            (SPM_BASE + 0x0FA0)
#define SPM_PMSR_LAST_STA41            (SPM_BASE + 0x0FA4)
#define SPM_PMSR_LAST_STA42            (SPM_BASE + 0x0FA8)
#define SPM_PMSR_LAST_STA43            (SPM_BASE + 0x0FAC)
#define SPM_PMSR_LAST_STA44            (SPM_BASE + 0x0FB0)
#define SPM_PMSR_LAST_STA45            (SPM_BASE + 0x0FB4)
#define SPM_PMSR_LAST_STA46            (SPM_BASE + 0x0FB8)
#define SPM_PMSR_LAST_STA47            (SPM_BASE + 0x0FBC)
#define SPM_PMSR_SEL_CON0              (SPM_BASE + 0x0FC0)
#define SPM_PMSR_SEL_CON1              (SPM_BASE + 0x0FC4)
#define SPM_PMSR_SEL_CON2              (SPM_BASE + 0x0FC8)
#define SPM_PMSR_SEL_CON3              (SPM_BASE + 0x0FCC)
#define SPM_PMSR_SEL_CON4              (SPM_BASE + 0x0FD0)
#define SPM_PMSR_SEL_CON5              (SPM_BASE + 0x0FD4)
#define SPM_PMSR_SEL_CON6              (SPM_BASE + 0x0FD8)
#define SPM_PMSR_SEL_CON7              (SPM_BASE + 0x0FDC)
#define SPM_PMSR_MON_CON0              (SPM_BASE + 0x0FE0)
#define SPM_PMSR_MON_CON1              (SPM_BASE + 0x0FE4)
#define SPM_PMSR_MON_CON2              (SPM_BASE + 0x0FE8)
#define SPM_PMSR_LEN_CON0              (SPM_BASE + 0x0FEC)
#define SPM_PMSR_LEN_CON1              (SPM_BASE + 0x0FF0)
#define SPM_PMSR_LEN_CON2              (SPM_BASE + 0x0FF4)

/* POWERON_CONFIG_EN (0x10006000+0x000) */
#define BCLK_CG_EN_LSB                      (1U << 0)       /* 1b */
#define PROJECT_CODE_LSB                    (1U << 16)      /* 16b */
/* SPM_POWER_ON_VAL0 (0x10006000+0x004) */
#define POWER_ON_VAL0_LSB                   (1U << 0)       /* 32b */
/* SPM_POWER_ON_VAL1 (0x10006000+0x008) */
#define POWER_ON_VAL1_LSB                   (1U << 0)       /* 32b */
/* SPM_CLK_CON (0x10006000+0x00C) */
#define REG_SRCCLKEN0_CTL_LSB               (1U << 0)       /* 2b */
#define REG_SRCCLKEN1_CTL_LSB               (1U << 2)       /* 2b */
#define REG_SPM_LOCK_INFRA_DCM_LSB          (1U << 4)       /* 1b */
#define REG_SRCCLKEN_MASK_LSB               (1U << 5)       /* 3b */
#define REG_MD1_C32RM_EN_LSB                (1U << 8)       /* 1b */
#define REG_MD2_C32RM_EN_LSB                (1U << 9)       /* 1b */
#define REG_CLKSQ0_SEL_CTRL_LSB             (1U << 10)      /* 1b */
#define REG_CLKSQ1_SEL_CTRL_LSB             (1U << 11)      /* 1b */
#define REG_SRCCLKEN0_EN_LSB                (1U << 12)      /* 1b */
#define REG_SRCCLKEN1_EN_LSB                (1U << 13)      /* 1b */
#define REG_SYSCLK0_SRC_MASK_B_LSB          (1U << 14)      /* 9b */
#define REG_SYSCLK1_SRC_MASK_B_LSB          (1U << 23)      /* 9b */
/* SPM_CLK_SETTLE (0x10006000+0x010) */
#define SYSCLK_SETTLE_LSB                   (1U << 0)       /* 28b */
/* SPM_AP_STANDBY_CON (0x10006000+0x014) */
#define REG_WFI_OP_LSB                      (1U << 0)       /* 1b */
#define REG_WFI_TYPE_LSB                    (1U << 1)       /* 1b */
#define REG_MP0_CPUTOP_IDLE_MASK_LSB        (1U << 2)       /* 1b */
#define REG_MP1_CPUTOP_IDLE_MASK_LSB        (1U << 3)       /* 1b */
#define REG_MCUSYS_IDLE_MASK_LSB            (1U << 4)       /* 1b */
#define REG_MD_APSRC_1_SEL_LSB              (1U << 25)      /* 1b */
#define REG_MD_APSRC_0_SEL_LSB              (1U << 26)      /* 1b */
#define REG_CONN_APSRC_SEL_LSB              (1U << 29)      /* 1b */
/* PCM_CON0 (0x10006000+0x018) */
#define PCM_CK_EN_LSB                       (1U << 2)       /* 1b */
#define RG_EN_IM_SLEEP_DVS_LSB              (1U << 3)       /* 1b */
#define PCM_SW_RESET_LSB                    (1U << 15)      /* 1b */
#define PCM_CON0_PROJECT_CODE_LSB           (1U << 16)      /* 16b */
/* PCM_CON1 (0x10006000+0x01C) */
#define RG_IM_SLAVE_LSB                     (1U << 0)       /* 1b */
#define RG_IM_SLEEP_LSB                     (1U << 1)       /* 1b */
#define SPM_SRAM_SLEEP_B_ECO_EN_LSB         (1U << 2)       /* 1b */
#define RG_AHBMIF_APBEN_LSB                 (1U << 3)       /* 1b */
#define RG_IM_PDN_LSB                       (1U << 4)       /* 1b */
#define RG_PCM_TIMER_EN_LSB                 (1U << 5)       /* 1b */
#define RG_SPM_EVENT_COUNTER_CLR_LSB        (1U << 6)       /* 1b */
#define RG_DIS_MIF_PROT_LSB                 (1U << 7)       /* 1b */
#define RG_PCM_WDT_EN_LSB                   (1U << 8)       /* 1b */
#define RG_PCM_WDT_WAKE_LSB                 (1U << 9)       /* 1b */
#define REG_SPM_SRAM_SLEEP_B_LSB            (1U << 10)      /* 1b */
#define REG_SPM_SRAM_ISOINT_B_LSB           (1U << 11)      /* 1b */
#define REG_EVENT_LOCK_EN_LSB               (1U << 12)      /* 1b */
#define REG_SRCCLKEN_FAST_RESP_LSB          (1U << 13)      /* 1b */
#define REG_MD32_APB_INTERNAL_EN_LSB        (1U << 14)      /* 1b */
#define RG_PCM_IRQ_MSK_LSB                  (1U << 15)      /* 1b */
#define PCM_CON1_PROJECT_CODE_LSB           (1U << 16)      /* 16b */
/* PCM_TIMER_VAL (0x10006000+0x020) */
#define REG_PCM_TIMER_VAL_LSB               (1U << 0)       /* 32b */
/* PCM_WDT_VAL (0x10006000+0x024) */
#define RG_PCM_WDT_VAL_LSB                  (1U << 0)       /* 32b */
/* PCM_REG_DATA_INI (0x10006000+0x028) */
#define PCM_REG_DATA_INI_LSB                (1U << 0)       /* 32b */
/* PCM_PWR_IO_EN (0x10006000+0x02C) */
#define PCM_PWR_IO_EN_LSB                   (1U << 0)       /* 8b */
#define RG_RF_SYNC_EN_LSB                   (1U << 16)      /* 8b */
/* SPM_VCORE_DVFS_SHORTCUT_CON00 (0x10006000+0x030) */
#define SPM_VCORE_DVFS_SHORTCUT_CON00_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON01 (0x10006000+0x034) */
#define SPM_VCORE_DVFS_SHORTCUT_CON01_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON02 (0x10006000+0x038) */
#define SPM_VCORE_DVFS_SHORTCUT_CON02_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON03 (0x10006000+0x03C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON03_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON04 (0x10006000+0x040) */
#define SPM_VCORE_DVFS_SHORTCUT_CON04_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON05 (0x10006000+0x044) */
#define SPM_VCORE_DVFS_SHORTCUT_CON05_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON06 (0x10006000+0x048) */
#define SPM_VCORE_DVFS_SHORTCUT_CON06_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON07 (0x10006000+0x04C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON07_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON08 (0x10006000+0x050) */
#define SPM_VCORE_DVFS_SHORTCUT_CON08_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON09 (0x10006000+0x054) */
#define SPM_VCORE_DVFS_SHORTCUT_CON09_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON10 (0x10006000+0x058) */
#define SPM_VCORE_DVFS_SHORTCUT_CON10_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON11 (0x10006000+0x05C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON11_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON12 (0x10006000+0x060) */
#define SPM_VCORE_DVFS_SHORTCUT_CON12_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON13 (0x10006000+0x064) */
#define SPM_VCORE_DVFS_SHORTCUT_CON13_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON14 (0x10006000+0x068) */
#define SPM_VCORE_DVFS_SHORTCUT_CON14_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON15 (0x10006000+0x06C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON15_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON16 (0x10006000+0x070) */
#define SPM_VCORE_DVFS_SHORTCUT_CON16_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON17 (0x10006000+0x074) */
#define SPM_VCORE_DVFS_SHORTCUT_CON17_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON18 (0x10006000+0x078) */
#define SPM_VCORE_DVFS_SHORTCUT_CON18_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON19 (0x10006000+0x07C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON19_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON20 (0x10006000+0x080) */
#define SPM_VCORE_DVFS_SHORTCUT_CON20_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON21 (0x10006000+0x084) */
#define SPM_VCORE_DVFS_SHORTCUT_CON21_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON22 (0x10006000+0x088) */
#define SPM_VCORE_DVFS_SHORTCUT_CON22_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON23 (0x10006000+0x08C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON23_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON24 (0x10006000+0x090) */
#define SPM_VCORE_DVFS_SHORTCUT_CON24_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON25 (0x10006000+0x094) */
#define SPM_VCORE_DVFS_SHORTCUT_CON25_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON26 (0x10006000+0x098) */
#define SPM_VCORE_DVFS_SHORTCUT_CON26_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON27 (0x10006000+0x09C) */
#define SPM_VCORE_DVFS_SHORTCUT_CON27_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON28 (0x10006000+0x0A0) */
#define SPM_VCORE_DVFS_SHORTCUT_CON28_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON29 (0x10006000+0x0A4) */
#define SPM_VCORE_DVFS_SHORTCUT_CON29_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON30 (0x10006000+0x0A8) */
#define SPM_VCORE_DVFS_SHORTCUT_CON30_LSB   (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_CON31 (0x10006000+0x0AC) */
#define SPM_VCORE_DVFS_SHORTCUT_CON31_LSB   (1U << 0)       /* 32b */
/* SPM_CPU_WAKEUP_EVENT (0x10006000+0x0B0) */
#define REG_CPU_WAKEUP_LSB                  (1U << 0)       /* 1b */
/* SPM_IRQ_MASK (0x10006000+0x0B4) */
#define REG_SPM_IRQ_MASK_LSB                (1U << 0)       /* 32b */
/* SPM_SRC_REQ (0x10006000+0x0B8) */
#define REG_SPM_APSRC_REQ_LSB               (1U << 0)       /* 1b */
#define REG_SPM_F26M_REQ_LSB                (1U << 1)       /* 1b */
#define REG_SPM_INFRA_REQ_LSB               (1U << 3)       /* 1b */
#define REG_SPM_VRF18_REQ_LSB               (1U << 4)       /* 1b */
#define REG_SPM_DDR_EN_REQ_LSB              (1U << 7)       /* 1b */
#define REG_SPM_DDR_EN2_REQ_LSB             (1U << 8)       /* 1b */
#define REG_SPM_DVFS_REQ_LSB                (1U << 9)       /* 1b */
#define REG_SPM_SW_MAILBOX_REQ_LSB          (1U << 10)      /* 1b */
#define REG_SPM_SSPM_MAILBOX_REQ_LSB        (1U << 11)      /* 1b */
#define REG_SPM_ADSP_MAILBOX_REQ_LSB        (1U << 12)      /* 1b */
#define REG_SPM_SCP_MAILBOX_REQ_LSB         (1U << 13)      /* 1b */
#define REG_SPM_MCUSYS_PWR_EVENT_REQ_LSB    (1U << 14)      /* 1b */
#define CPU_MD_DVFS_SOP_FORCE_ON_LSB        (1U << 15)      /* 1b */
/* SPM_SRC_MASK (0x10006000+0x0BC) */
#define REG_MD_SRCCLKENA_0_MASK_B_LSB       (1U << 0)       /* 1b */
#define REG_MD_SRCCLKENA2INFRA_REQ_0_MASK_B_LSB (1U << 1)       /* 1b */
#define REG_MD_APSRC2INFRA_REQ_0_MASK_B_LSB (1U << 2)       /* 1b */
#define REG_MD_APSRC_REQ_0_MASK_B_LSB       (1U << 3)       /* 1b */
#define REG_MD_VRF18_REQ_0_MASK_B_LSB       (1U << 4)       /* 1b */
#define REG_MD_DDR_EN_0_MASK_B_LSB          (1U << 5)       /* 1b */
#define REG_MD_DDR_EN2_0_MASK_B_LSB         (1U << 6)       /* 1b */
#define REG_MD_SRCCLKENA_1_MASK_B_LSB       (1U << 7)       /* 1b */
#define REG_MD_SRCCLKENA2INFRA_REQ_1_MASK_B_LSB (1U << 8)       /* 1b */
#define REG_MD_APSRC2INFRA_REQ_1_MASK_B_LSB (1U << 9)       /* 1b */
#define REG_MD_APSRC_REQ_1_MASK_B_LSB       (1U << 10)      /* 1b */
#define REG_MD_VRF18_REQ_1_MASK_B_LSB       (1U << 11)      /* 1b */
#define REG_MD_DDR_EN_1_MASK_B_LSB          (1U << 12)      /* 1b */
#define REG_MD_DDR_EN2_1_MASK_B_LSB         (1U << 13)      /* 1b */
#define REG_CONN_SRCCLKENA_MASK_B_LSB       (1U << 14)      /* 1b */
#define REG_CONN_SRCCLKENB_MASK_B_LSB       (1U << 15)      /* 1b */
#define REG_CONN_INFRA_REQ_MASK_B_LSB       (1U << 16)      /* 1b */
#define REG_CONN_APSRC_REQ_MASK_B_LSB       (1U << 17)      /* 1b */
#define REG_CONN_VRF18_REQ_MASK_B_LSB       (1U << 18)      /* 1b */
#define REG_CONN_DDR_EN_MASK_B_LSB          (1U << 19)      /* 1b */
#define REG_CONN_DDR_EN2_MASK_B_LSB         (1U << 20)      /* 1b */
#define REG_SRCCLKENI0_SRCCLKENA_MASK_B_LSB (1U << 21)      /* 1b */
#define REG_SRCCLKENI0_INFRA_REQ_MASK_B_LSB (1U << 22)      /* 1b */
#define REG_SRCCLKENI1_SRCCLKENA_MASK_B_LSB (1U << 23)      /* 1b */
#define REG_SRCCLKENI1_INFRA_REQ_MASK_B_LSB (1U << 24)      /* 1b */
#define REG_SRCCLKENI2_SRCCLKENA_MASK_B_LSB (1U << 25)      /* 1b */
#define REG_SRCCLKENI2_INFRA_REQ_MASK_B_LSB (1U << 26)      /* 1b */
#define REG_INFRASYS_APSRC_REQ_MASK_B_LSB   (1U << 27)      /* 1b */
#define REG_INFRASYS_DDR_EN_MASK_B_LSB      (1U << 28)      /* 1b */
#define REG_INFRASYS_DDR_EN2_MASK_B_LSB     (1U << 29)      /* 1b */
#define REG_MD32_SRCCLKENA_MASK_B_LSB       (1U << 30)      /* 1b */
#define REG_CONN_VFE28_REQ_MASK_B_LSB       (1U << 31)      /* 1b */
/* SPM_SRC2_MASK (0x10006000+0x0C0) */
#define REG_MD32_INFRA_REQ_MASK_B_LSB       (1U << 0)       /* 1b */
#define REG_MD32_APSRC_REQ_MASK_B_LSB       (1U << 1)       /* 1b */
#define REG_MD32_VRF18_REQ_MASK_B_LSB       (1U << 2)       /* 1b */
#define REG_MD32_DDR_EN_MASK_B_LSB          (1U << 3)       /* 1b */
#define REG_MD32_DDR_EN2_MASK_B_LSB         (1U << 4)       /* 1b */
#define REG_SCP_SRCCLKENA_MASK_B_LSB        (1U << 5)       /* 1b */
#define REG_SCP_INFRA_REQ_MASK_B_LSB        (1U << 6)       /* 1b */
#define REG_SCP_APSRC_REQ_MASK_B_LSB        (1U << 7)       /* 1b */
#define REG_SCP_VRF18_REQ_MASK_B_LSB        (1U << 8)       /* 1b */
#define REG_SCP_DDR_EN_MASK_B_LSB           (1U << 9)       /* 1b */
#define REG_SCP_DDR_EN2_MASK_B_LSB          (1U << 10)      /* 1b */
#define REG_UFS_SRCCLKENA_MASK_B_LSB        (1U << 11)      /* 1b */
#define REG_UFS_INFRA_REQ_MASK_B_LSB        (1U << 12)      /* 1b */
#define REG_UFS_APSRC_REQ_MASK_B_LSB        (1U << 13)      /* 1b */
#define REG_UFS_VRF18_REQ_MASK_B_LSB        (1U << 14)      /* 1b */
#define REG_UFS_DDR_EN_MASK_B_LSB           (1U << 15)      /* 1b */
#define REG_UFS_DDR_EN2_MASK_B_LSB          (1U << 16)      /* 1b */
#define REG_DISP0_APSRC_REQ_MASK_B_LSB      (1U << 17)      /* 1b */
#define REG_DISP0_DDR_EN_MASK_B_LSB         (1U << 18)      /* 1b */
#define REG_DISP0_DDR_EN2_MASK_B_LSB        (1U << 19)      /* 1b */
#define REG_DISP1_APSRC_REQ_MASK_B_LSB      (1U << 20)      /* 1b */
#define REG_DISP1_DDR_EN_MASK_B_LSB         (1U << 21)      /* 1b */
#define REG_DISP1_DDR_EN2_MASK_B_LSB        (1U << 22)      /* 1b */
#define REG_GCE_INFRA_REQ_MASK_B_LSB        (1U << 23)      /* 1b */
#define REG_GCE_APSRC_REQ_MASK_B_LSB        (1U << 24)      /* 1b */
#define REG_GCE_VRF18_REQ_MASK_B_LSB        (1U << 25)      /* 1b */
#define REG_GCE_DDR_EN_MASK_B_LSB           (1U << 26)      /* 1b */
#define REG_GCE_DDR_EN2_MASK_B_LSB          (1U << 27)      /* 1b */
#define REG_EMI_CH0_DDR_EN_MASK_B_LSB       (1U << 28)      /* 1b */
#define REG_EMI_CH1_DDR_EN_MASK_B_LSB       (1U << 29)      /* 1b */
#define REG_EMI_CH0_DDR_EN2_MASK_B_LSB      (1U << 30)      /* 1b */
#define REG_EMI_CH1_DDR_EN2_MASK_B_LSB      (1U << 31)      /* 1b */
/* SPM_SRC3_MASK (0x10006000+0x0C4) */
#define REG_DVFSRC_EVENT_TRIGGER_MASK_B_LSB (1U << 0)       /* 1b */
#define REG_SW2SPM_INT0_MASK_B_LSB          (1U << 1)       /* 1b */
#define REG_SW2SPM_INT1_MASK_B_LSB          (1U << 2)       /* 1b */
#define REG_SW2SPM_INT2_MASK_B_LSB          (1U << 3)       /* 1b */
#define REG_SW2SPM_INT3_MASK_B_LSB          (1U << 4)       /* 1b */
#define REG_SC_ADSP2SPM_WAKEUP_MASK_B_LSB   (1U << 5)       /* 1b */
#define REG_SC_SSPM2SPM_WAKEUP_MASK_B_LSB   (1U << 6)       /* 4b */
#define REG_SC_SCP2SPM_WAKEUP_MASK_B_LSB    (1U << 10)      /* 1b */
#define REG_CSYSPWRREQ_MASK_LSB             (1U << 11)      /* 1b */
#define REG_SPM_SRCCLKENA_RESERVED_MASK_B_LSB (1U << 12)      /* 1b */
#define REG_SPM_INFRA_REQ_RESERVED_MASK_B_LSB (1U << 13)      /* 1b */
#define REG_SPM_APSRC_REQ_RESERVED_MASK_B_LSB (1U << 14)      /* 1b */
#define REG_SPM_VRF18_REQ_RESERVED_MASK_B_LSB (1U << 15)      /* 1b */
#define REG_SPM_DDR_EN_RESERVED_MASK_B_LSB  (1U << 16)      /* 1b */
#define REG_SPM_DDR_EN2_RESERVED_MASK_B_LSB (1U << 17)      /* 1b */
#define REG_AUDIO_DSP_SRCCLKENA_MASK_B_LSB  (1U << 18)      /* 1b */
#define REG_AUDIO_DSP_INFRA_REQ_MASK_B_LSB  (1U << 19)      /* 1b */
#define REG_AUDIO_DSP_APSRC_REQ_MASK_B_LSB  (1U << 20)      /* 1b */
#define REG_AUDIO_DSP_VRF18_REQ_MASK_B_LSB  (1U << 21)      /* 1b */
#define REG_AUDIO_DSP_DDR_EN_MASK_B_LSB     (1U << 22)      /* 1b */
#define REG_AUDIO_DSP_DDR_EN2_MASK_B_LSB    (1U << 23)      /* 1b */
#define REG_MCUSYS_PWR_EVENT_MASK_B_LSB     (1U << 24)      /* 1b */
#define REG_MSDC0_SRCCLKENA_MASK_B_LSB      (1U << 25)      /* 1b */
#define REG_MSDC0_INFRA_REQ_MASK_B_LSB      (1U << 26)      /* 1b */
#define REG_MSDC0_APSRC_REQ_MASK_B_LSB      (1U << 27)      /* 1b */
#define REG_MSDC0_VRF18_REQ_MASK_B_LSB      (1U << 28)      /* 1b */
#define REG_MSDC0_DDR_EN_MASK_B_LSB         (1U << 29)      /* 1b */
#define REG_MSDC0_DDR_EN2_MASK_B_LSB        (1U << 30)      /* 1b */
#define REG_CONN_SRCCLKENB2PWRAP_MASK_B_LSB (1U << 31)      /* 1b */
/* SPM_SRC4_MASK (0x10006000+0x0C8) */
#define CCIF_EVENT_MASK_B_LSB               (1U << 0)       /* 16b */
#define REG_APU_CORE0_SRCCLKENA_MASK_B_LSB  (1U << 16)      /* 1b */
#define REG_APU_CORE0_INFRA_REQ_MASK_B_LSB  (1U << 17)      /* 1b */
#define REG_APU_CORE0_APSRC_REQ_MASK_B_LSB  (1U << 18)      /* 1b */
#define REG_APU_CORE0_VRF18_REQ_MASK_B_LSB  (1U << 19)      /* 1b */
#define REG_APU_CORE0_DDR_EN_MASK_B_LSB     (1U << 20)      /* 1b */
#define REG_APU_CORE1_SRCCLKENA_MASK_B_LSB  (1U << 21)      /* 1b */
#define REG_APU_CORE1_INFRA_REQ_MASK_B_LSB  (1U << 22)      /* 1b */
#define REG_APU_CORE1_APSRC_REQ_MASK_B_LSB  (1U << 23)      /* 1b */
#define REG_APU_CORE1_VRF18_REQ_MASK_B_LSB  (1U << 24)      /* 1b */
#define REG_APU_CORE1_DDR_EN_MASK_B_LSB     (1U << 25)      /* 1b */
#define REG_APU_CORE2_SRCCLKENA_MASK_B_LSB  (1U << 26)      /* 1b */
#define REG_APU_CORE2_INFRA_REQ_MASK_B_LSB  (1U << 27)      /* 1b */
#define REG_APU_CORE2_APSRC_REQ_MASK_B_LSB  (1U << 28)      /* 1b */
#define REG_APU_CORE2_VRF18_REQ_MASK_B_LSB  (1U << 29)      /* 1b */
#define REG_APU_CORE2_DDR_EN_MASK_B_LSB     (1U << 30)      /* 1b */
#define REG_APU_CORE2_DDR_EN2_MASK_B_LSB    (1U << 31)      /* 1b */
/* SPM_SRC5_MASK (0x10006000+0x0CC) */
#define REG_MCUSYS_MERGE_APSRC_REQ_MASK_B_LSB (1U << 0)       /* 9b */
#define REG_MCUSYS_MERGE_DDR_EN_MASK_B_LSB  (1U << 9)       /* 9b */
#define REG_MCUSYS_MERGE_DDR_EN2_MASK_B_LSB (1U << 18)      /* 9b */
#define REG_APU_CORE0_DDR_EN2_MASK_B_LSB    (1U << 27)      /* 1b */
#define REG_APU_CORE1_DDR_EN2_MASK_B_LSB    (1U << 28)      /* 1b */
#define REG_CG_CHECK_DDR_EN_MASK_B_LSB      (1U << 29)      /* 1b */
#define REG_CG_CHECK_DDR_EN2_MASK_B_LSB     (1U << 30)      /* 1b */
/* SPM_WAKEUP_EVENT_MASK (0x10006000+0x0D0) */
#define REG_WAKEUP_EVENT_MASK_LSB           (1U << 0)       /* 32b */
/* SPM_WAKEUP_EVENT_EXT_MASK (0x10006000+0x0D4) */
#define REG_EXT_WAKEUP_EVENT_MASK_LSB       (1U << 0)       /* 32b */
/* SPM_TWAM_EVENT_CLEAR (0x10006000+0x0D8) */
#define SPM_TWAM_EVENT_CLEAR_LSB            (1U << 0)       /* 1b */
/* SPM_SRC6_MASK (0x10006000+0x0DC) */
#define REG_MSDC1_SRCCLKENA_MASK_B_LSB      (1U << 0)       /* 1b */
#define REG_MSDC1_INFRA_REQ_MASK_B_LSB      (1U << 1)       /* 1b */
#define REG_MSDC1_APSRC_REQ_MASK_B_LSB      (1U << 2)       /* 1b */
#define REG_MSDC1_VRF18_REQ_MASK_B_LSB      (1U << 3)       /* 1b */
#define REG_MSDC1_DDR_EN_MASK_B_LSB         (1U << 4)       /* 1b */
#define REG_MSDC1_DDR_EN2_MASK_B_LSB        (1U << 5)       /* 1b */
#define REG_MSDC1_SRCCLKENA_ACK_MASK_LSB    (1U << 16)      /* 1b */
#define REG_MSDC1_INFRA_ACK_MASK_LSB        (1U << 17)      /* 1b */
#define REG_MSDC1_APSRC_ACK_MASK_LSB        (1U << 18)      /* 1b */
#define REG_MSDC1_VRF18_ACK_MASK_LSB        (1U << 19)      /* 1b */
#define REG_MSDC1_DDR_EN_ACK_MASK_LSB       (1U << 20)      /* 1b */
#define REG_MSDC1_DDR_EN2_ACK_MASK_LSB      (1U << 21)      /* 1b */
/* PCM_DEBUG_CON (0x10006000+0x0E0) */
#define PCM_DEBUG_OUT_ENABLE_LSB            (1U << 0)       /* 1b */
/* AHB_BUS_CON (0x10006000+0x0E4) */
#define AHB_HADDR_EXT_LSB                   (1U << 0)       /* 2b */
#define REG_AHB_LOCK_LSB                    (1U << 8)       /* 1b */
/* DDR_EN_DBC_CON0 (0x10006000+0x0E8) */
#define REG_ALL_DDR_EN_DBC_LEN_LSB          (1U << 0)       /* 10b */
#define REG_MD_DDR_EN_0_DBC_LEN_LSB         (1U << 10)      /* 10b */
#define REG_CONN_DDR_EN_DBC_LEN_LSB         (1U << 20)      /* 10b */
/* DDR_EN_DBC_CON1 (0x10006000+0x0EC) */
#define REG_ALL_DDR_EN_DBC_EN_LSB           (1U << 0)       /* 1b */
#define REG_MD_DDR_EN_0_DBC_EN_LSB          (1U << 1)       /* 1b */
#define REG_CONN_DDR_EN_DBC_EN_LSB          (1U << 2)       /* 1b */
/* SPM_RESOURCE_ACK_CON0 (0x10006000+0x0F0) */
#define REG_MD_SRCCLKENA_ACK_0_MASK_LSB     (1U << 0)       /* 1b */
#define REG_MD_INFRA_ACK_0_MASK_LSB         (1U << 1)       /* 1b */
#define REG_MD_APSRC_ACK_0_MASK_LSB         (1U << 2)       /* 1b */
#define REG_MD_VRF18_ACK_0_MASK_LSB         (1U << 3)       /* 1b */
#define REG_MD_DDR_EN_ACK_0_MASK_LSB        (1U << 4)       /* 1b */
#define REG_MD_DDR_EN2_ACK_0_MASK_LSB       (1U << 5)       /* 1b */
#define REG_MD_SRCCLKENA_ACK_1_MASK_LSB     (1U << 6)       /* 1b */
#define REG_MD_INFRA_ACK_1_MASK_LSB         (1U << 7)       /* 1b */
#define REG_MD_APSRC_ACK_1_MASK_LSB         (1U << 8)       /* 1b */
#define REG_MD_VRF18_ACK_1_MASK_LSB         (1U << 9)       /* 1b */
#define REG_MD_DDR_EN_ACK_1_MASK_LSB        (1U << 10)      /* 1b */
#define REG_MD_DDR_EN2_ACK_1_MASK_LSB       (1U << 11)      /* 1b */
#define REG_CONN_SRCCLKENA_ACK_MASK_LSB     (1U << 12)      /* 1b */
#define REG_CONN_INFRA_ACK_MASK_LSB         (1U << 13)      /* 1b */
#define REG_CONN_APSRC_ACK_MASK_LSB         (1U << 14)      /* 1b */
#define REG_CONN_VRF18_ACK_MASK_LSB         (1U << 15)      /* 1b */
#define REG_CONN_DDR_EN_ACK_MASK_LSB        (1U << 16)      /* 1b */
#define REG_CONN_DDR_EN2_ACK_MASK_LSB       (1U << 17)      /* 1b */
#define REG_MD32_SRCCLKENA_ACK_MASK_LSB     (1U << 18)      /* 1b */
#define REG_MD32_INFRA_ACK_MASK_LSB         (1U << 19)      /* 1b */
#define REG_MD32_APSRC_ACK_MASK_LSB         (1U << 20)      /* 1b */
#define REG_MD32_VRF18_ACK_MASK_LSB         (1U << 21)      /* 1b */
#define REG_MD32_DDR_EN_ACK_MASK_LSB        (1U << 22)      /* 1b */
#define REG_MD32_DDR_EN2_ACK_MASK_LSB       (1U << 23)      /* 1b */
#define REG_SCP_SRCCLKENA_ACK_MASK_LSB      (1U << 24)      /* 1b */
#define REG_SCP_INFRA_ACK_MASK_LSB          (1U << 25)      /* 1b */
#define REG_SCP_APSRC_ACK_MASK_LSB          (1U << 26)      /* 1b */
#define REG_SCP_VRF18_ACK_MASK_LSB          (1U << 27)      /* 1b */
#define REG_SCP_DDR_EN_ACK_MASK_LSB         (1U << 28)      /* 1b */
#define REG_SCP_DDR_EN2_ACK_MASK_LSB        (1U << 29)      /* 1b */
/* SPM_RESOURCE_ACK_CON1 (0x10006000+0x0F4) */
#define REG_AUDIO_DSP_SRCCLKENA_ACK_MASK_LSB (1U << 0)       /* 1b */
#define REG_AUDIO_DSP_INFRA_ACK_MASK_LSB    (1U << 1)       /* 1b */
#define REG_AUDIO_DSP_APSRC_ACK_MASK_LSB    (1U << 2)       /* 1b */
#define REG_AUDIO_DSP_VRF18_ACK_MASK_LSB    (1U << 3)       /* 1b */
#define REG_AUDIO_DSP_DDR_EN_ACK_MASK_LSB   (1U << 4)       /* 1b */
#define REG_AUDIO_DSP_DDR_EN2_ACK_MASK_LSB  (1U << 5)       /* 1b */
#define REG_UFS_SRCCLKENA_ACK_MASK_LSB      (1U << 6)       /* 1b */
#define REG_UFS_INFRA_ACK_MASK_LSB          (1U << 7)       /* 1b */
#define REG_UFS_APSRC_ACK_MASK_LSB          (1U << 8)       /* 1b */
#define REG_UFS_VRF18_ACK_MASK_LSB          (1U << 9)       /* 1b */
#define REG_UFS_DDR_EN_ACK_MASK_LSB         (1U << 10)      /* 1b */
#define REG_UFS_DDR_EN2_ACK_MASK_LSB        (1U << 11)      /* 1b */
#define REG_DISP0_APSRC_ACK_MASK_LSB        (1U << 12)      /* 1b */
#define REG_DISP0_DDR_EN_ACK_MASK_LSB       (1U << 13)      /* 1b */
#define REG_DISP0_DDR_EN2_ACK_MASK_LSB      (1U << 14)      /* 1b */
#define REG_DISP1_APSRC_ACK_MASK_LSB        (1U << 15)      /* 1b */
#define REG_DISP1_DDR_EN_ACK_MASK_LSB       (1U << 16)      /* 1b */
#define REG_DISP1_DDR_EN2_ACK_MASK_LSB      (1U << 17)      /* 1b */
#define REG_GCE_INFRA_ACK_MASK_LSB          (1U << 18)      /* 1b */
#define REG_GCE_APSRC_ACK_MASK_LSB          (1U << 19)      /* 1b */
#define REG_GCE_VRF18_ACK_MASK_LSB          (1U << 20)      /* 1b */
#define REG_GCE_DDR_EN_ACK_MASK_LSB         (1U << 21)      /* 1b */
#define REG_GCE_DDR_EN2_ACK_MASK_LSB        (1U << 22)      /* 1b */
#define REG_MSDC0_SRCCLKENA_ACK_MASK_LSB    (1U << 23)      /* 1b */
#define REG_MSDC0_INFRA_ACK_MASK_LSB        (1U << 24)      /* 1b */
#define REG_MSDC0_APSRC_ACK_MASK_LSB        (1U << 25)      /* 1b */
#define REG_MSDC0_VRF18_ACK_MASK_LSB        (1U << 26)      /* 1b */
#define REG_MSDC0_DDR_EN_ACK_MASK_LSB       (1U << 27)      /* 1b */
#define REG_MSDC0_DDR_EN2_ACK_MASK_LSB      (1U << 28)      /* 1b */
#define REG_APU_CORE1_DDR_EN2_ACK_MASK_LSB  (1U << 29)      /* 1b */
#define REG_APU_CORE2_DDR_EN2_ACK_MASK_LSB  (1U << 30)      /* 1b */
/* SPM_RESOURCE_ACK_CON2 (0x10006000+0x0F8) */
#define REG_APU_CORE0_SRCCLKENA_ACK_MASK_LSB (1U << 0)       /* 1b */
#define REG_APU_CORE0_INFRA_ACK_MASK_LSB    (1U << 1)       /* 1b */
#define REG_APU_CORE0_APSRC_ACK_MASK_LSB    (1U << 2)       /* 1b */
#define REG_APU_CORE0_VRF18_ACK_MASK_LSB    (1U << 3)       /* 1b */
#define REG_APU_CORE0_DDR_EN_ACK_MASK_LSB   (1U << 4)       /* 1b */
#define REG_APU_CORE0_DDR_EN2_ACK_MASK_LSB  (1U << 5)       /* 1b */
#define REG_APU_CORE1_SRCCLKENA_ACK_MASK_LSB (1U << 6)       /* 1b */
#define REG_APU_CORE1_INFRA_ACK_MASK_LSB    (1U << 7)       /* 1b */
#define REG_APU_CORE1_APSRC_ACK_MASK_LSB    (1U << 8)       /* 1b */
#define REG_APU_CORE1_VRF18_ACK_MASK_LSB    (1U << 9)       /* 1b */
#define REG_APU_CORE1_DDR_EN_ACK_MASK_LSB   (1U << 10)      /* 1b */
#define REG_APU_CORE2_SRCCLKENA_ACK_MASK_LSB (1U << 11)      /* 1b */
#define REG_APU_CORE2_INFRA_ACK_MASK_LSB    (1U << 12)      /* 1b */
#define REG_APU_CORE2_APSRC_ACK_MASK_LSB    (1U << 13)      /* 1b */
#define REG_APU_CORE2_VRF18_ACK_MASK_LSB    (1U << 14)      /* 1b */
#define REG_APU_CORE2_DDR_EN_ACK_MASK_LSB   (1U << 15)      /* 1b */
#define SPM_DDR_EN_ACK_WAIT_CYCLE_LSB       (1U << 16)      /* 8b */
#define SPM_DDR_EN2_ACK_WAIT_CYCLE_LSB      (1U << 24)      /* 8b */
/* SPM_RESOURCE_ACK_CON3 (0x10006000+0x0FC) */
#define SPM_F26M_ACK_WAIT_CYCLE_LSB         (1U << 0)       /* 8b */
#define SPM_INFRA_ACK_WAIT_CYCLE_LSB        (1U << 8)       /* 8b */
#define SPM_APSRC_ACK_WAIT_CYCLE_LSB        (1U << 16)      /* 8b */
#define SPM_VRF18_ACK_WAIT_CYCLE_LSB        (1U << 24)      /* 8b */
/* PCM_REG0_DATA (0x10006000+0x100) */
#define PCM_REG0_RF_LSB                     (1U << 0)       /* 32b */
/* PCM_REG2_DATA (0x10006000+0x104) */
#define PCM_REG2_RF_LSB                     (1U << 0)       /* 32b */
/* PCM_REG6_DATA (0x10006000+0x108) */
#define PCM_REG6_RF_LSB                     (1U << 0)       /* 32b */
/* PCM_REG7_DATA (0x10006000+0x10C) */
#define PCM_REG7_RF_LSB                     (1U << 0)       /* 32b */
/* PCM_REG13_DATA (0x10006000+0x110) */
#define PCM_REG13_RF_LSB                    (1U << 0)       /* 32b */
/* SRC_REQ_STA_0 (0x10006000+0x114) */
#define MD_SRCCLKENA_0_LSB                  (1U << 0)       /* 1b */
#define MD_SRCCLKENA2INFRA_REQ_0_LSB        (1U << 1)       /* 1b */
#define MD_APSRC2INFRA_REQ_0_LSB            (1U << 2)       /* 1b */
#define MD_APSRC_REQ_0_LSB                  (1U << 3)       /* 1b */
#define MD_VRF18_REQ_0_LSB                  (1U << 4)       /* 1b */
#define MD_DDR_EN_0_LSB                     (1U << 5)       /* 1b */
#define MD_DDR_EN2_0_LSB                    (1U << 6)       /* 1b */
#define MD_SRCCLKENA_1_LSB                  (1U << 7)       /* 1b */
#define MD_SRCCLKENA2INFRA_REQ_1_LSB        (1U << 8)       /* 1b */
#define MD_APSRC2INFRA_REQ_1_LSB            (1U << 9)       /* 1b */
#define MD_APSRC_REQ_1_LSB                  (1U << 10)      /* 1b */
#define MD_VRF18_REQ_1_LSB                  (1U << 11)      /* 1b */
#define MD_DDR_EN_1_LSB                     (1U << 12)      /* 1b */
#define MD_DDR_EN2_1_LSB                    (1U << 13)      /* 1b */
#define CONN_SRCCLKENA_LSB                  (1U << 14)      /* 1b */
#define CONN_INFRA_REQ_LSB                  (1U << 15)      /* 1b */
#define CONN_APSRC_REQ_LSB                  (1U << 16)      /* 1b */
#define CONN_VRF18_REQ_LSB                  (1U << 17)      /* 1b */
#define CONN_DDR_EN_LSB                     (1U << 18)      /* 1b */
#define CONN_DDR_EN2_LSB                    (1U << 19)      /* 1b */
#define SRCCLKENI_LSB                       (1U << 20)      /* 3b */
#define MD32_SRCCLKENA_LSB                  (1U << 23)      /* 1b */
#define MD32_INFRA_REQ_LSB                  (1U << 24)      /* 1b */
#define MD32_APSRC_REQ_LSB                  (1U << 25)      /* 1b */
#define MD32_VRF18_REQ_LSB                  (1U << 26)      /* 1b */
#define MD32_DDR_EN_LSB                     (1U << 27)      /* 1b */
#define MD32_DDR_EN2_LSB                    (1U << 28)      /* 1b */
#define DVFSRC_EVENT_TRIGGER_LSB            (1U << 29)      /* 1b */
#define MCUSYS_SPMC_PWR_ON_ACK_LSB          (1U << 30)      /* 1b */
/* SRC_REQ_STA_1 (0x10006000+0x118) */
#define SCP_SRCCLKENA_LSB                   (1U << 0)       /* 1b */
#define SCP_INFRA_REQ_LSB                   (1U << 1)       /* 1b */
#define SCP_APSRC_REQ_LSB                   (1U << 2)       /* 1b */
#define SCP_VRF18_REQ_LSB                   (1U << 3)       /* 1b */
#define SCP_DDR_EN_LSB                      (1U << 4)       /* 1b */
#define SCP_DDR_EN2_LSB                     (1U << 5)       /* 1b */
#define AUDIO_DSP_SRCCLKENA_LSB             (1U << 6)       /* 1b */
#define AUDIO_DSP_INFRA_REQ_LSB             (1U << 7)       /* 1b */
#define AUDIO_DSP_APSRC_REQ_LSB             (1U << 8)       /* 1b */
#define AUDIO_DSP_VRF18_REQ_LSB             (1U << 9)       /* 1b */
#define AUDIO_DSP_DDR_EN_LSB                (1U << 10)      /* 1b */
#define AUDIO_DSP_DDR_EN2_LSB               (1U << 11)      /* 1b */
#define UFS_SRCCLKENA_LSB                   (1U << 12)      /* 1b */
#define UFS_INFRA_REQ_LSB                   (1U << 13)      /* 1b */
#define UFS_APSRC_REQ_LSB                   (1U << 14)      /* 1b */
#define UFS_VRF18_REQ_LSB                   (1U << 15)      /* 1b */
#define UFS_DDR_EN_LSB                      (1U << 16)      /* 1b */
#define UFS_DDR_EN2_LSB                     (1U << 17)      /* 1b */
#define DISP0_APSRC_REQ_LSB                 (1U << 18)      /* 1b */
#define DISP0_DDR_EN_LSB                    (1U << 19)      /* 1b */
#define DISP0_DDR_EN2_LSB                   (1U << 20)      /* 1b */
#define DISP1_APSRC_REQ_LSB                 (1U << 21)      /* 1b */
#define DISP1_DDR_EN_LSB                    (1U << 22)      /* 1b */
#define DISP1_DDR_EN2_LSB                   (1U << 23)      /* 1b */
#define GCE_INFRA_REQ_LSB                   (1U << 24)      /* 1b */
#define GCE_APSRC_REQ_LSB                   (1U << 25)      /* 1b */
#define GCE_VRF18_REQ_LSB                   (1U << 26)      /* 1b */
#define GCE_DDR_EN_LSB                      (1U << 27)      /* 1b */
#define GCE_DDR_EN2_LSB                     (1U << 28)      /* 1b */
#define INFRASYS_APSRC_REQ_LSB              (1U << 29)      /* 1b */
#define INFRASYS_DDR_EN_LSB                 (1U << 30)      /* 1b */
#define INFRASYS_DDR_EN2_LSB                (1U << 31)      /* 1b */
/* SRC_REQ_STA_2 (0x10006000+0x11C) */
#define MCUSYS_MERGE_DDR_EN_LSB             (1U << 0)       /* 9b */
#define EMI_SELF_REFRESH_CH_LSB             (1U << 9)       /* 2b */
#define SW2SPM_INT_LSB                      (1U << 11)      /* 4b */
#define SC_ADSP2SPM_WAKEUP_LSB              (1U << 15)      /* 1b */
#define SC_SSPM2SPM_WAKEUP_LSB              (1U << 16)      /* 4b */
#define SC_SCP2SPM_WAKEUP_LSB               (1U << 20)      /* 1b */
#define SPM_SRCCLKENA_RESERVED_LSB          (1U << 21)      /* 1b */
#define SPM_INFRA_REQ_RESERVED_LSB          (1U << 22)      /* 1b */
#define SPM_APSRC_REQ_RESERVED_LSB          (1U << 23)      /* 1b */
#define SPM_VRF18_REQ_RESERVED_LSB          (1U << 24)      /* 1b */
#define SPM_DDR_EN_RESERVED_LSB             (1U << 25)      /* 1b */
#define SPM_DDR_EN2_RESERVED_LSB            (1U << 26)      /* 1b */
/* SRC_REQ_STA_3 (0x10006000+0x120) */
#define CCIF_EVENT_RAW_STATUS_LSB           (1U << 0)       /* 16b */
#define F26M_STATE_LSB                      (1U << 16)      /* 1b */
#define INFRA_STATE_LSB                     (1U << 17)      /* 1b */
#define APSRC_STATE_LSB                     (1U << 18)      /* 1b */
#define VRF18_STATE_LSB                     (1U << 19)      /* 1b */
#define DDR_EN_STATE_LSB                    (1U << 20)      /* 1b */
#define DDR_EN2_STATE_LSB                   (1U << 21)      /* 1b */
#define DVFS_STATE_LSB                      (1U << 22)      /* 1b */
#define SW_MAILBOX_STATE_LSB                (1U << 23)      /* 1b */
#define SSPM_MAILBOX_STATE_LSB              (1U << 24)      /* 1b */
#define ADSP_MAILBOX_STATE_LSB              (1U << 25)      /* 1b */
#define SCP_MAILBOX_STATE_LSB               (1U << 26)      /* 1b */
#define MCUSYS_PWR_EVENT_STATE_LSB          (1U << 27)      /* 1b */
/* SRC_REQ_STA_4 (0x10006000+0x124) */
#define MSDC0_SRCCLKENA_LSB                 (1U << 0)       /* 1b */
#define MSDC0_INFRA_REQ_LSB                 (1U << 1)       /* 1b */
#define MSDC0_APSRC_REQ_LSB                 (1U << 2)       /* 1b */
#define MSDC0_VRF18_REQ_LSB                 (1U << 3)       /* 1b */
#define MSDC0_DDR_EN_LSB                    (1U << 4)       /* 1b */
#define MSDC0_DDR_EN2_LSB                   (1U << 5)       /* 1b */
#define APU_CORE0_SRCCLKENA_LSB             (1U << 6)       /* 1b */
#define APU_CORE0_INFRA_REQ_LSB             (1U << 7)       /* 1b */
#define APU_CORE0_APSRC_REQ_LSB             (1U << 8)       /* 1b */
#define APU_CORE0_VRF18_REQ_LSB             (1U << 9)       /* 1b */
#define APU_CORE0_DDR_EN_LSB                (1U << 10)      /* 1b */
#define APU_CORE0_DDR_EN2_LSB               (1U << 11)      /* 1b */
#define APU_CORE1_SRCCLKENA_LSB             (1U << 12)      /* 1b */
#define APU_CORE1_INFRA_REQ_LSB             (1U << 13)      /* 1b */
#define APU_CORE1_APSRC_REQ_LSB             (1U << 14)      /* 1b */
#define APU_CORE1_VRF18_REQ_LSB             (1U << 15)      /* 1b */
#define APU_CORE1_DDR_EN_LSB                (1U << 16)      /* 1b */
#define APU_CORE1_DDR_EN2_LSB               (1U << 17)      /* 1b */
#define APU_CORE2_SRCCLKENA_LSB             (1U << 18)      /* 1b */
#define APU_CORE2_INFRA_REQ_LSB             (1U << 19)      /* 1b */
#define APU_CORE2_APSRC_REQ_LSB             (1U << 20)      /* 1b */
#define APU_CORE2_VRF18_REQ_LSB             (1U << 21)      /* 1b */
#define APU_CORE2_DDR_EN_LSB                (1U << 22)      /* 1b */
#define APU_CORE2_DDR_EN2_LSB               (1U << 23)      /* 1b */
#define CG_CHECK_DDR_EN_LSB                 (1U << 24)      /* 1b */
#define CG_CHECK_DDR_EN2_LSB                (1U << 25)      /* 1b */
#define MSDC1_SRCCLKENA_LSB                 (1U << 26)      /* 1b */
#define MSDC1_INFRA_REQ_LSB                 (1U << 27)      /* 1b */
#define MSDC1_APSRC_REQ_LSB                 (1U << 28)      /* 1b */
#define MSDC1_VRF18_REQ_LSB                 (1U << 29)      /* 1b */
#define MSDC1_DDR_EN_LSB                    (1U << 30)      /* 1b */
#define MSDC1_DDR_EN2_LSB                   (1U << 31)      /* 1b */
/* PCM_TIMER_OUT (0x10006000+0x128) */
#define PCM_TIMER_LSB                       (1U << 0)       /* 32b */
/* PCM_WDT_OUT (0x10006000+0x12C) */
#define PCM_WDT_TIMER_VAL_OUT_LSB           (1U << 0)       /* 32b */
/* SPM_IRQ_STA (0x10006000+0x130) */
#define TWAM_IRQ_LSB                        (1U << 2)       /* 1b */
#define PCM_IRQ_LSB                         (1U << 3)       /* 1b */
/* SUBSYS_IDLE_STA (0x10006000+0x134) */
#define SUBSYS_IDLE_SIGNALS_LSB             (1U << 0)       /* 32b */
/* MD32PCM_WAKEUP_STA (0x10006000+0x138) */
#define MD32PCM_WAKEUP_STA_LSB              (1U << 0)       /* 32b */
/* MD32PCM_EVENT_STA (0x10006000+0x13C) */
#define MD32PCM_EVENT_STA_LSB               (1U << 0)       /* 32b */
/* SPM_WAKEUP_STA (0x10006000+0x140) */
#define F32K_WAKEUP_EVENT_L_LSB             (1U << 0)       /* 16b */
#define ASYN_WAKEUP_EVENT_L_LSB             (1U << 16)      /* 16b */
/* SPM_WAKEUP_EXT_STA (0x10006000+0x144) */
#define EXT_WAKEUP_EVENT_LSB                (1U << 0)       /* 32b */
/* SPM_WAKEUP_MISC (0x10006000+0x148) */
#define GIC_WAKEUP_LSB                      (1U << 0)       /* 10b */
#define DVFSRC_IRQ_LSB                      (1U << 16)      /* 1b */
#define PCM_TIMER_EVENT_LSB                 (1U << 17)      /* 1b */
#define PMIC_EINT_OUT_B_LSB                 (1U << 18)      /* 2b */
#define TWAM_IRQ_B_LSB                      (1U << 20)      /* 1b */
#define PMSR_IRQ_B_SET0_LSB                 (1U << 21)      /* 1b */
#define PMSR_IRQ_B_SET1_LSB                 (1U << 22)      /* 1b */
#define PMSR_IRQ_B_SET2_LSB                 (1U << 23)      /* 1b */
#define SPM_ACK_CHK_WAKEUP_0_LSB            (1U << 24)      /* 1b */
#define SPM_ACK_CHK_WAKEUP_1_LSB            (1U << 25)      /* 1b */
#define SPM_ACK_CHK_WAKEUP_2_LSB            (1U << 26)      /* 1b */
#define SPM_ACK_CHK_WAKEUP_3_LSB            (1U << 27)      /* 1b */
#define SPM_ACK_CHK_WAKEUP_4_LSB            (1U << 28)      /* 1b */
#define SPM_ACK_CHK_WAKEUP_ALL_LSB          (1U << 29)      /* 1b */
#define PMIC_IRQ_ACK_LSB                    (1U << 30)      /* 1b */
#define PMIC_SCP_IRQ_LSB                    (1U << 31)      /* 1b */
/* BUS_PROTECT_RDY (0x10006000+0x14C) */
#define PROTECT_READY_LSB                   (1U << 0)       /* 32b */
/* BUS_PROTECT2_RDY (0x10006000+0x150) */
#define PROTECT2_READY_LSB                  (1U << 0)       /* 32b */
/* BUS_PROTECT3_RDY (0x10006000+0x154) */
#define PROTECT3_READY_LSB                  (1U << 0)       /* 32b */
/* BUS_PROTECT4_RDY (0x10006000+0x158) */
#define BUS_PROTECT_MM_RDY_LSB              (1U << 0)       /* 16b */
#define BUS_PROTECT_MCU_RDY_LSB             (1U << 16)      /* 16b */
/* PCM_STA (0x10006000+0x15C) */
#define PCM_CK_SEL_O_LSB                    (1U << 0)       /* 3b */
#define EXT_SRC_STA_LSB                     (1U << 4)       /* 3b */
#define RAM_SEQ_ATPG_EN_TO_SPM_LSB          (1U << 7)       /* 1b */
#define WDT_GPU_EXT_BUCK_ISO_LSB            (1U << 8)       /* 1b */
/* PWR_STATUS (0x10006000+0x160) */
#define PWR_STATUS_LSB                      (1U << 0)       /* 32b */
/* PWR_STATUS_2ND (0x10006000+0x164) */
#define PWR_STATUS_2ND_LSB                  (1U << 0)       /* 32b */
/* CPU_PWR_STATUS (0x10006000+0x168) */
#define CPU_PWR_STATUS_LSB                  (1U << 0)       /* 32b */
/* SPM_SRC_RDY_STA (0x10006000+0x16C) */
#define SPM_INFRA_INTERNAL_ACK_LSB          (1U << 0)       /* 1b */
#define SPM_VRF18_INTERNAL_ACK_LSB          (1U << 1)       /* 1b */
#define SPM_DDR_EN2_INTERNAL_ACK_LSB        (1U << 2)       /* 1b */
#define SPM_DDR_EN2_INTERNAL_ACK_SEL_LSB    (1U << 3)       /* 1b */
/* SPM_TWAM_LAST_STA0 (0x10006000+0x170) */
#define LAST_IDLE_CNT_0_LSB                 (1U << 0)       /* 32b */
/* SPM_TWAM_LAST_STA1 (0x10006000+0x174) */
#define LAST_IDLE_CNT_1_LSB                 (1U << 0)       /* 32b */
/* SPM_TWAM_LAST_STA2 (0x10006000+0x178) */
#define LAST_IDLE_CNT_2_LSB                 (1U << 0)       /* 32b */
/* SPM_TWAM_LAST_STA3 (0x10006000+0x17C) */
#define LAST_IDLE_CNT_3_LSB                 (1U << 0)       /* 32b */
/* SPM_TWAM_CURR_STA0 (0x10006000+0x180) */
#define CURRENT_IDLE_CNT_0_LSB              (1U << 0)       /* 32b */
/* SPM_TWAM_CURR_STA1 (0x10006000+0x184) */
#define CURRENT_IDLE_CNT_1_LSB              (1U << 0)       /* 32b */
/* SPM_TWAM_CURR_STA2 (0x10006000+0x188) */
#define CURRENT_IDLE_CNT_2_LSB              (1U << 0)       /* 32b */
/* SPM_TWAM_CURR_STA3 (0x10006000+0x18C) */
#define CURRENT_IDLE_CNT_3_LSB              (1U << 0)       /* 32b */
/* SPM_TWAM_TIMER_OUT (0x10006000+0x190) */
#define TWAM_TIMER_LSB                      (1U << 0)       /* 32b */
/* SPM_TWAM_CON (0x10006000+0x194) */
#define REG_TWAM_ENABLE_LSB                 (1U << 0)       /* 1b */
#define REG_TWAM_SPEED_MODE_EN_LSB          (1U << 1)       /* 1b */
#define REG_TWAM_SW_RST_LSB                 (1U << 2)       /* 1b */
#define REG_TWAM_IRQ_MASK_LSB               (1U << 3)       /* 1b */
#define REG_TWAM_MON_TYPE_0_LSB             (1U << 4)       /* 2b */
#define REG_TWAM_MON_TYPE_1_LSB             (1U << 6)       /* 2b */
#define REG_TWAM_MON_TYPE_2_LSB             (1U << 8)       /* 2b */
#define REG_TWAM_MON_TYPE_3_LSB             (1U << 10)      /* 2b */
/* SPM_TWAM_WINDOW_LEN (0x10006000+0x198) */
#define REG_TWAM_WINDOW_LEN_LSB             (1U << 0)       /* 32b */
/* SPM_TWAM_IDLE_SEL (0x10006000+0x19C) */
#define REG_TWAM_SIG_SEL_0_LSB              (1U << 0)       /* 7b */
#define REG_TWAM_SIG_SEL_1_LSB              (1U << 8)       /* 7b */
#define REG_TWAM_SIG_SEL_2_LSB              (1U << 16)      /* 7b */
#define REG_TWAM_SIG_SEL_3_LSB              (1U << 24)      /* 7b */
/* MBIST_EFUSE_REPAIR_ACK_STA (0x10006000+0x1A0) */
#define MFG_EFUSE_S2P_RX_DONE_LSB           (1U << 0)       /* 1b */
#define CAM_EFUSE_S2P_RX_DONE_LSB           (1U << 1)       /* 1b */
#define INFRA_PWR_EFUSE_S2P_RX_DONE_LSB     (1U << 2)       /* 1b */
#define INFRA_AO_EFUSE_S2P_RX_DONE_LSB      (1U << 3)       /* 1b */
/* SPM_PC_STA (0x10006000+0x1A4) */
#define MON_PC_LSB                          (1U << 0)       /* 32b */
/* DVFSRC_EVENT_STA (0x10006000+0x1A8) */
#define DVFSRC_EVENT_LSB                    (1U << 0)       /* 32b */
/* MCUSYS_IDLE_STA (0x10006000+0x1AC) */
#define ARMBUS_IDLE_TO_26M_LSB              (1U << 0)       /* 1b */
#define MP0_CLUSTER_IDLE_TO_PWR_OFF_LSB     (1U << 1)       /* 1b */
#define MCUSYS_DDR_EN_0_LSB                 (1U << 2)       /* 1b */
#define MCUSYS_DDR_EN_1_LSB                 (1U << 3)       /* 1b */
#define MCUSYS_DDR_EN_2_LSB                 (1U << 4)       /* 1b */
#define MCUSYS_DDR_EN_3_LSB                 (1U << 5)       /* 1b */
#define MCUSYS_DDR_EN_4_LSB                 (1U << 6)       /* 1b */
#define MCUSYS_DDR_EN_5_LSB                 (1U << 7)       /* 1b */
#define MCUSYS_DDR_EN_6_LSB                 (1U << 8)       /* 1b */
#define MCUSYS_DDR_EN_7_LSB                 (1U << 9)       /* 1b */
#define MP0_CPU_IDLE_TO_PWR_OFF_LSB         (1U << 16)      /* 8b */
#define WFI_AF_SEL_LSB                      (1U << 24)      /* 8b */
/* GIC_WAKEUP_STA (0x10006000+0x1B0) */
#define GIC_WAKEUP_STA_GIC_WAKEUP_LSB       (1U << 10)      /* 10b */
/* SYS_TIMER_OUT_L (0x10006000+0x1B4) */
#define SYS_TIMER_OUT_L_LSB                 (1U << 0)       /* 32b */
/* SYS_TIMER_OUT_H (0x10006000+0x1B8) */
#define SYS_TIMER_OUT_H_LSB                 (1U << 0)       /* 32b */
/* SPM_CG_CHECK_STA (0x10006000+0x1BC) */
#define SPM_DEEPIDLE_CG_CHECK_RESULT_LSB    (1U << 0)       /* 1b */
#define SPM_SODI_CG_CHECK_RESULT_LSB        (1U << 1)       /* 1b */
#define SPM_SODI3_CG_CHECK_RESULT_LSB       (1U << 2)       /* 1b */
#define SPM_MCDSR_CG_CHECK_RESULT_LSB       (1U << 3)       /* 1b */
/* SPM_DVFS_HISTORY_STA0 (0x10006000+0x1C0) */
#define SPM_DVFS_HISTORY_STA0_LSB           (1U << 0)       /* 32b */
/* SPM_DVFS_HISTORY_STA1 (0x10006000+0x1C4) */
#define SPM_DVFS_HISTORY_STA1_LSB           (1U << 0)       /* 32b */
/* SPM_DDREN_SLEEP_COUNT (0x10006000+0x1C8) */
#define SPM_DDREN_SLEEP_COUNT_LSB           (1U << 0)       /* 32b */
/* SPM_DDREN_WAKE_COUNT (0x10006000+0x1CC) */
#define SPM_DDREN_WAKE_COUNT_LSB            (1U << 0)       /* 32b */
/* SPM_APSRC_COUNT (0x10006000+0x1D0) */
#define SPM_APSRC_SLEEP_COUNT_LSB           (1U << 0)       /* 16b */
#define SPM_APSRC_WAKE_COUNT_LSB            (1U << 16)      /* 16b */
/* SPM_VRF18_COUNT (0x10006000+0x1D4) */
#define SPM_VRF18_SLEEP_COUNT_LSB           (1U << 0)       /* 16b */
#define SPM_VRF18_WAKE_COUNT_LSB            (1U << 16)      /* 16b */
/* SPM_INFRA_COUNT (0x10006000+0x1D8) */
#define SPM_INFRA_SLEEP_COUNT_LSB           (1U << 0)       /* 16b */
#define SPM_INFRA_WAKE_COUNT_LSB            (1U << 16)      /* 16b */
/* SPM_26M_COUNT (0x10006000+0x1DC) */
#define SPM_26M_SLEEP_COUNT_LSB             (1U << 0)       /* 16b */
#define SPM_26M_WAKE_COUNT_LSB              (1U << 16)      /* 16b */
/* MCUSYS_PWR_CON (0x10006000+0x200) */
#define MCUSYS_SPMC_PWR_RST_B_LSB           (1U << 0)       /* 1b */
#define MCUSYS_SPMC_PWR_ON_LSB              (1U << 2)       /* 1b */
#define MCUSYS_SPMC_PWR_CLK_DIS_LSB         (1U << 4)       /* 1b */
#define MCUSYS_SPMC_RESETPWRON_CONFIG_LSB   (1U << 5)       /* 1b */
#define MCUSYS_SPMC_DORMANT_EN_LSB          (1U << 6)       /* 1b */
#define MCUSYS_VPROC_EXT_OFF_LSB            (1U << 7)       /* 1b */
#define MCUSYS_PWR_CON_MCUSYS_SPMC_PWR_ON_ACK_LSB (1U << 31)      /* 1b */
/* MP0_CPUTOP_PWR_CON (0x10006000+0x204) */
#define MP0_SPMC_PWR_RST_B_CPUTOP_LSB       (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPUTOP_LSB          (1U << 2)       /* 1b */
#define MP0_SPMC_PWR_CLK_DIS_CPUTOP_LSB     (1U << 4)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPUTOP_LSB (1U << 5)       /* 1b */
#define MP0_SPMC_DORMANT_EN_CPUTOP_LSB      (1U << 6)       /* 1b */
#define MP0_VPROC_EXT_OFF_LSB               (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPUTOP_LSB      (1U << 31)      /* 1b */
/* MP0_CPU0_PWR_CON (0x10006000+0x208) */
#define MP0_SPMC_PWR_RST_B_CPU0_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU0_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU0_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU0_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU0_LSB        (1U << 31)      /* 1b */
/* MP0_CPU1_PWR_CON (0x10006000+0x20C) */
#define MP0_SPMC_PWR_RST_B_CPU1_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU1_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU1_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU1_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU1_LSB        (1U << 31)      /* 1b */
/* MP0_CPU2_PWR_CON (0x10006000+0x210) */
#define MP0_SPMC_PWR_RST_B_CPU2_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU2_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU2_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU2_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU2_LSB        (1U << 31)      /* 1b */
/* MP0_CPU3_PWR_CON (0x10006000+0x214) */
#define MP0_SPMC_PWR_RST_B_CPU3_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU3_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU3_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU3_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU3_LSB        (1U << 31)      /* 1b */
/* MP0_CPU4_PWR_CON (0x10006000+0x218) */
#define MP0_SPMC_PWR_RST_B_CPU4_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU4_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU4_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU4_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU4_LSB        (1U << 31)      /* 1b */
/* MP0_CPU5_PWR_CON (0x10006000+0x21C) */
#define MP0_SPMC_PWR_RST_B_CPU5_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU5_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU5_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU5_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU5_LSB        (1U << 31)      /* 1b */
/* MP0_CPU6_PWR_CON (0x10006000+0x220) */
#define MP0_SPMC_PWR_RST_B_CPU6_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU6_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU6_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU6_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU6_LSB        (1U << 31)      /* 1b */
/* MP0_CPU7_PWR_CON (0x10006000+0x224) */
#define MP0_SPMC_PWR_RST_B_CPU7_LSB         (1U << 0)       /* 1b */
#define MP0_SPMC_PWR_ON_CPU7_LSB            (1U << 2)       /* 1b */
#define MP0_SPMC_RESETPWRON_CONFIG_CPU7_LSB (1U << 5)       /* 1b */
#define MP0_VPROC_EXT_OFF_CPU7_LSB          (1U << 7)       /* 1b */
#define MP0_SPMC_PWR_ON_ACK_CPU7_LSB        (1U << 31)      /* 1b */
/* MP0_CPU0_WFI_EN (0x10006000+0x228) */
#define MP0_CPU0_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU1_WFI_EN (0x10006000+0x22C) */
#define MP0_CPU1_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU2_WFI_EN (0x10006000+0x230) */
#define MP0_CPU2_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU3_WFI_EN (0x10006000+0x234) */
#define MP0_CPU3_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU4_WFI_EN (0x10006000+0x238) */
#define MP0_CPU4_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU5_WFI_EN (0x10006000+0x23C) */
#define MP0_CPU5_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU6_WFI_EN (0x10006000+0x240) */
#define MP0_CPU6_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* MP0_CPU7_WFI_EN (0x10006000+0x244) */
#define MP0_CPU7_WFI_EN_LSB                 (1U << 0)       /* 1b */
/* ARMPLL_CLK_SEL (0x10006000+0x248) */
#define ARMPLL_CLK_SEL_LSB                  (1U << 0)       /* 4b */
/* ARMPLL_CLK_CON (0x10006000+0x24C) */
#define SC_ARM_FHC_PAUSE_LSB                (1U << 0)       /* 4b */
#define SC_ARM_CK_OFF_LSB                   (1U << 4)       /* 4b */
#define SC_ARMPLL_OFF_LSB                   (1U << 8)       /* 1b */
#define SC_ARMBPLL_OFF_LSB                  (1U << 9)       /* 1b */
#define SC_ARMBBPLL_OFF_LSB                 (1U << 10)      /* 1b */
#define SC_CCIPLL_CKOFF_LSB                 (1U << 11)      /* 1b */
#define SC_ARMDDS_OFF_LSB                   (1U << 12)      /* 1b */
#define SC_ARMBPLL_S_OFF_LSB                (1U << 13)      /* 1b */
#define SC_ARMBBPLL_S_OFF_LSB               (1U << 14)      /* 1b */
#define SC_CCIPLL_PWROFF_LSB                (1U << 15)      /* 1b */
#define SC_ARMPLLOUT_OFF_LSB                (1U << 16)      /* 1b */
#define SC_ARMBPLLOUT_OFF_LSB               (1U << 17)      /* 1b */
#define SC_ARMBBPLLOUT_OFF_LSB              (1U << 18)      /* 1b */
#define SC_CCIPLL_OUT_OFF_LSB               (1U << 19)      /* 1b */
/* CPU_SPARE_CON (0x10006000+0x250) */
#define CPU_SPARE_CON_LSB                   (1U << 0)       /* 32b */
/* CPU_SPARE_CON_SET (0x10006000+0x254) */
#define CPU_SPARE_CON_SET_LSB               (1U << 0)       /* 32b */
/* CPU_SPARE_CON_CLR (0x10006000+0x258) */
#define CPU_SPARE_CON_CLR_LSB               (1U << 0)       /* 32b */
/* EXT_INT_WAKEUP_REQ (0x10006000+0x25C) */
#define EXT_INT_WAKEUP_REQ_LSB              (1U << 0)       /* 10b */
/* EXT_INT_WAKEUP_REQ_SET (0x10006000+0x260) */
#define EXT_INT_WAKEUP_REQ_SET_LSB          (1U << 0)       /* 10b */
/* EXT_INT_WAKEUP_REQ_CLR (0x10006000+0x264) */
#define EXT_INT_WAKEUP_REQ_CLR_LSB          (1U << 0)       /* 10b */
/* ROOT_CPUTOP_ADDR (0x10006000+0x268) */
#define ROOT_CPUTOP_ADDR_LSB                (1U << 0)       /* 32b */
/* ROOT_CORE_ADDR (0x10006000+0x26C) */
#define ROOT_CORE_ADDR_LSB                  (1U << 0)       /* 32b */
/* SYS_TIMER_CON (0x10006000+0x274) */
#define SYS_TIMER_START_LSB                 (1U << 0)       /* 1b */
#define SYS_TIMER_UPDATE_LSB                (1U << 1)       /* 1b */
#define SYS_TIMER_START_DONE_LSB            (1U << 2)       /* 1b */
#define SYS_TIMER_UPDATE_DONE_LSB           (1U << 3)       /* 1b */
#define SYS_TIMER_ID_LSB                    (1U << 8)       /* 8b */
/* SYS_TIMER_START_POINT_L (0x10006000+0x278) */
#define SYS_TIMER_START_POINT_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_START_POINT_H (0x10006000+0x27C) */
#define SYS_TIMER_START_POINT_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_0_L (0x10006000+0x280) */
#define SYS_TIMER_LATCH_OUT_0_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_0_H (0x10006000+0x284) */
#define SYS_TIMER_LATCH_OUT_0_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_1_L (0x10006000+0x288) */
#define SYS_TIMER_LATCH_OUT_1_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_1_H (0x10006000+0x28C) */
#define SYS_TIMER_LATCH_OUT_1_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_2_L (0x10006000+0x290) */
#define SYS_TIMER_LATCH_OUT_2_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_2_H (0x10006000+0x294) */
#define SYS_TIMER_LATCH_OUT_2_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_3_L (0x10006000+0x298) */
#define SYS_TIMER_LATCH_OUT_3_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_3_H (0x10006000+0x29C) */
#define SYS_TIMER_LATCH_OUT_3_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_4_L (0x10006000+0x2A0) */
#define SYS_TIMER_LATCH_OUT_4_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_4_H (0x10006000+0x2A4) */
#define SYS_TIMER_LATCH_OUT_4_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_5_L (0x10006000+0x2A8) */
#define SYS_TIMER_LATCH_OUT_5_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_5_H (0x10006000+0x2AC) */
#define SYS_TIMER_LATCH_OUT_5_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_6_L (0x10006000+0x2B0) */
#define SYS_TIMER_LATCH_OUT_6_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_6_H (0x10006000+0x2B4) */
#define SYS_TIMER_LATCH_OUT_6_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_7_L (0x10006000+0x2B8) */
#define SYS_TIMER_LATCH_OUT_7_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_7_H (0x10006000+0x2BC) */
#define SYS_TIMER_LATCH_OUT_7_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_8_L (0x10006000+0x2C0) */
#define SYS_TIMER_LATCH_OUT_8_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_8_H (0x10006000+0x2C4) */
#define SYS_TIMER_LATCH_OUT_8_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_9_L (0x10006000+0x2C8) */
#define SYS_TIMER_LATCH_OUT_9_L_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_9_H (0x10006000+0x2CC) */
#define SYS_TIMER_LATCH_OUT_9_H_LSB         (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_10_L (0x10006000+0x2D0) */
#define SYS_TIMER_LATCH_OUT_10_L_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_10_H (0x10006000+0x2D4) */
#define SYS_TIMER_LATCH_OUT_10_H_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_11_L (0x10006000+0x2D8) */
#define SYS_TIMER_LATCH_OUT_11_L_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_11_H (0x10006000+0x2DC) */
#define SYS_TIMER_LATCH_OUT_11_H_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_12_L (0x10006000+0x2E0) */
#define SYS_TIMER_LATCH_OUT_12_L_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_12_H (0x10006000+0x2E4) */
#define SYS_TIMER_LATCH_OUT_12_H_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_13_L (0x10006000+0x2E8) */
#define SYS_TIMER_LATCH_OUT_13_L_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_13_H (0x10006000+0x2EC) */
#define SYS_TIMER_LATCH_OUT_13_H_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_14_L (0x10006000+0x2F0) */
#define SYS_TIMER_LATCH_OUT_14_L_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_14_H (0x10006000+0x2F4) */
#define SYS_TIMER_LATCH_OUT_14_H_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_15_L (0x10006000+0x2F8) */
#define SYS_TIMER_LATCH_OUT_15_L_LSB        (1U << 0)       /* 32b */
/* SYS_TIMER_LATCH_OUT_15_H (0x10006000+0x2FC) */
#define SYS_TIMER_LATCH_OUT_15_H_LSB        (1U << 0)       /* 32b */
/* VDE_PWR_CON (0x10006000+0x300) */
#define VDE_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define VDE_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define VDE_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define VDE_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define VDE_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define VDE_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define VDE_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* VEN_PWR_CON (0x10006000+0x304) */
#define VEN_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define VEN_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define VEN_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define VEN_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define VEN_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define VEN_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define VEN_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* ISP_PWR_CON (0x10006000+0x308) */
#define ISP_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define ISP_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define ISP_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define ISP_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define ISP_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define ISP_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define ISP_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* DIS_PWR_CON (0x10006000+0x30C) */
#define DIS_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define DIS_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define DIS_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define DIS_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define DIS_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define DIS_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define DIS_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* IFR_PWR_CON (0x10006000+0x310) */
#define IFR_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define IFR_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define IFR_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define IFR_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define IFR_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define IFR_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define IFR_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* DPY_PWR_CON (0x10006000+0x314) */
#define DPY_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define DPY_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define DPY_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define DPY_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define DPY_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define DPY_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define DPY_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* MD1_PWR_CON (0x10006000+0x318) */
#define MD1_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define MD1_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define MD1_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define MD1_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define MD1_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define MD1_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
/* AUDIO_PWR_CON (0x10006000+0x31C) */
#define AUDIO_PWR_RST_B_LSB                 (1U << 0)       /* 1b */
#define AUDIO_PWR_ISO_LSB                   (1U << 1)       /* 1b */
#define AUDIO_PWR_ON_LSB                    (1U << 2)       /* 1b */
#define AUDIO_PWR_ON_2ND_LSB                (1U << 3)       /* 1b */
#define AUDIO_PWR_CLK_DIS_LSB               (1U << 4)       /* 1b */
#define AUDIO_SRAM_PDN_LSB                  (1U << 8)       /* 4b */
#define AUDIO_SRAM_PDN_ACK_LSB              (1U << 12)      /* 4b */
/* CONN_PWR_CON (0x10006000+0x320) */
#define CONN_PWR_RST_B_LSB                  (1U << 0)       /* 1b */
#define CONN_PWR_ISO_LSB                    (1U << 1)       /* 1b */
#define CONN_PWR_ON_LSB                     (1U << 2)       /* 1b */
#define CONN_PWR_ON_2ND_LSB                 (1U << 3)       /* 1b */
#define CONN_PWR_CLK_DIS_LSB                (1U << 4)       /* 1b */
/* CAM_PWR_CON (0x10006000+0x324) */
#define CAM_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define CAM_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define CAM_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define CAM_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define CAM_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define CAM_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define CAM_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* MFG0_PWR_CON (0x10006000+0x328) */
#define MFG0_PWR_RST_B_LSB                  (1U << 0)       /* 1b */
#define MFG0_PWR_ISO_LSB                    (1U << 1)       /* 1b */
#define MFG0_PWR_ON_LSB                     (1U << 2)       /* 1b */
#define MFG0_PWR_ON_2ND_LSB                 (1U << 3)       /* 1b */
#define MFG0_PWR_CLK_DIS_LSB                (1U << 4)       /* 1b */
#define MFG0_SRAM_PDN_LSB                   (1U << 8)       /* 4b */
#define MFG0_SRAM_PDN_ACK_LSB               (1U << 12)      /* 4b */
/* MFG1_PWR_CON (0x10006000+0x32C) */
#define MFG1_PWR_RST_B_LSB                  (1U << 0)       /* 1b */
#define MFG1_PWR_ISO_LSB                    (1U << 1)       /* 1b */
#define MFG1_PWR_ON_LSB                     (1U << 2)       /* 1b */
#define MFG1_PWR_ON_2ND_LSB                 (1U << 3)       /* 1b */
#define MFG1_PWR_CLK_DIS_LSB                (1U << 4)       /* 1b */
#define MFG1_SRAM_PDN_LSB                   (1U << 8)       /* 4b */
#define MFG1_SRAM_PDN_ACK_LSB               (1U << 12)      /* 4b */
/* MFG2_PWR_CON (0x10006000+0x330) */
#define MFG2_PWR_RST_B_LSB                  (1U << 0)       /* 1b */
#define MFG2_PWR_ISO_LSB                    (1U << 1)       /* 1b */
#define MFG2_PWR_ON_LSB                     (1U << 2)       /* 1b */
#define MFG2_PWR_ON_2ND_LSB                 (1U << 3)       /* 1b */
#define MFG2_PWR_CLK_DIS_LSB                (1U << 4)       /* 1b */
#define MFG2_SRAM_PDN_LSB                   (1U << 8)       /* 4b */
#define MFG2_SRAM_PDN_ACK_LSB               (1U << 12)      /* 4b */
/* MFG3_PWR_CON (0x10006000+0x334) */
#define MFG3_PWR_RST_B_LSB                  (1U << 0)       /* 1b */
#define MFG3_PWR_ISO_LSB                    (1U << 1)       /* 1b */
#define MFG3_PWR_ON_LSB                     (1U << 2)       /* 1b */
#define MFG3_PWR_ON_2ND_LSB                 (1U << 3)       /* 1b */
#define MFG3_PWR_CLK_DIS_LSB                (1U << 4)       /* 1b */
#define MFG3_SRAM_PDN_LSB                   (1U << 8)       /* 4b */
#define MFG3_SRAM_PDN_ACK_LSB               (1U << 12)      /* 4b */
/* MFG4_PWR_CON (0x10006000+0x338) */
#define MFG4_PWR_RST_B_LSB                  (1U << 0)       /* 1b */
#define MFG4_PWR_ISO_LSB                    (1U << 1)       /* 1b */
#define MFG4_PWR_ON_LSB                     (1U << 2)       /* 1b */
#define MFG4_PWR_ON_2ND_LSB                 (1U << 3)       /* 1b */
#define MFG4_PWR_CLK_DIS_LSB                (1U << 4)       /* 1b */
#define MFG4_SRAM_PDN_LSB                   (1U << 8)       /* 4b */
#define MFG4_SRAM_PDN_ACK_LSB               (1U << 12)      /* 4b */
/* VPU_VCORE_PWR_CON (0x10006000+0x33C) */
#define VPU_VCORE_PWR_RST_B_LSB             (1U << 0)       /* 1b */
#define VPU_VCORE_PWR_ISO_LSB               (1U << 1)       /* 1b */
#define VPU_VCORE_PWR_ON_LSB                (1U << 2)       /* 1b */
#define VPU_VCORE_PWR_ON_2ND_LSB            (1U << 3)       /* 1b */
#define VPU_VCORE_PWR_CLK_DIS_LSB           (1U << 4)       /* 1b */
/* VPU_CONN_PWR_CON (0x10006000+0x340) */
#define VPU_CONN_PWR_RST_B_LSB              (1U << 0)       /* 1b */
#define VPU_CONN_PWR_ISO_LSB                (1U << 1)       /* 1b */
#define VPU_CONN_PWR_ON_LSB                 (1U << 2)       /* 1b */
#define VPU_CONN_PWR_ON_2ND_LSB             (1U << 3)       /* 1b */
#define VPU_CONN_PWR_CLK_DIS_LSB            (1U << 4)       /* 1b */
/* VPU_CORE0_PWR_CON (0x10006000+0x344) */
#define VPU_CORE0_PWR_RST_B_LSB             (1U << 0)       /* 1b */
#define VPU_CORE0_PWR_ISO_LSB               (1U << 1)       /* 1b */
#define VPU_CORE0_PWR_ON_LSB                (1U << 2)       /* 1b */
#define VPU_CORE0_PWR_ON_2ND_LSB            (1U << 3)       /* 1b */
#define VPU_CORE0_PWR_CLK_DIS_LSB           (1U << 4)       /* 1b */
/* VPU_CORE1_PWR_CON (0x10006000+0x348) */
#define VPU_CORE1_PWR_RST_B_LSB             (1U << 0)       /* 1b */
#define VPU_CORE1_PWR_ISO_LSB               (1U << 1)       /* 1b */
#define VPU_CORE1_PWR_ON_LSB                (1U << 2)       /* 1b */
#define VPU_CORE1_PWR_ON_2ND_LSB            (1U << 3)       /* 1b */
#define VPU_CORE1_PWR_CLK_DIS_LSB           (1U << 4)       /* 1b */
/* VPU_CORE2_PWR_CON (0x10006000+0x34C) */
#define VPU_CORE2_PWR_RST_B_LSB             (1U << 0)       /* 1b */
#define VPU_CORE2_PWR_ISO_LSB               (1U << 1)       /* 1b */
#define VPU_CORE2_PWR_ON_LSB                (1U << 2)       /* 1b */
#define VPU_CORE2_PWR_ON_2ND_LSB            (1U << 3)       /* 1b */
#define VPU_CORE2_PWR_CLK_DIS_LSB           (1U << 4)       /* 1b */
/* IPE_PWR_CON (0x10006000+0x350) */
#define IPE_PWR_RST_B_LSB                   (1U << 0)       /* 1b */
#define IPE_PWR_ISO_LSB                     (1U << 1)       /* 1b */
#define IPE_PWR_ON_LSB                      (1U << 2)       /* 1b */
#define IPE_PWR_ON_2ND_LSB                  (1U << 3)       /* 1b */
#define IPE_PWR_CLK_DIS_LSB                 (1U << 4)       /* 1b */
#define IPE_SRAM_PDN_LSB                    (1U << 8)       /* 4b */
#define IPE_SRAM_PDN_ACK_LSB                (1U << 12)      /* 4b */
/* SYSRAM_CON (0x10006000+0x354) */
#define SYSRAM_SRAM_CKISO_LSB               (1U << 0)       /* 1b */
#define SYSRAM_SRAM_ISOINT_B_LSB            (1U << 1)       /* 1b */
#define SYSRAM_SRAM_SLEEP_B_LSB             (1U << 4)       /* 8b */
#define SYSRAM_SRAM_PDN_LSB                 (1U << 16)      /* 8b */
/* SYSROM_CON (0x10006000+0x358) */
#define SYSROM_SRAM_PDN_LSB                 (1U << 0)       /* 6b */
/* SSPM_SRAM_CON (0x10006000+0x35C) */
#define SSPM_SRAM_CKISO_LSB                 (1U << 0)       /* 1b */
#define SSPM_SRAM_ISOINT_B_LSB              (1U << 1)       /* 1b */
#define SSPM_SRAM_SLEEP_B_LSB               (1U << 4)       /* 1b */
#define SSPM_SRAM_PDN_LSB                   (1U << 16)      /* 1b */
/* SCP_SRAM_CON (0x10006000+0x360) */
#define SCP_SRAM_CKISO_LSB                  (1U << 0)       /* 1b */
#define SCP_SRAM_ISOINT_B_LSB               (1U << 1)       /* 1b */
#define SCP_SRAM_SLEEP_B_LSB                (1U << 4)       /* 1b */
#define SCP_SRAM_PDN_LSB                    (1U << 16)      /* 1b */
/* AUDIO_DSP_CON0 (0x10006000+0x364) */
#define AUDIO_DSP_CON0_SRAM_CKISO_LSB       (1U << 0)       /* 1b */
#define AUDIO_DSP_CON0_SRAM_ISOINT_B_LSB    (1U << 1)       /* 1b */
#define AUDIO_DSP_CON0_SRAM_SLEEP_B_LSB     (1U << 4)       /* 16b */
/* AUDIO_DSP_CON1 (0x10006000+0x368) */
#define AUDIO_DSP_CON1_SRAM_PDN_LSB         (1U << 0)       /* 17b */
/* DPY_SHU_SRAM_CON (0x10006000+0x36C) */
#define DPY_SHU_SRAM_CKISO_LSB              (1U << 0)       /* 1b */
#define DPY_SHU_SRAM_ISOINT_B_LSB           (1U << 1)       /* 1b */
#define DPY_SHU_SRAM_SLEEP_B_LSB            (1U << 4)       /* 2b */
#define DPY_SHU_SRAM_PDN_LSB                (1U << 16)      /* 2b */
/* UFS_SRAM_CON (0x10006000+0x370) */
#define UFS_SRAM_CKISO_LSB                  (1U << 0)       /* 1b */
#define UFS_SRAM_ISOINT_B_LSB               (1U << 1)       /* 1b */
#define UFS_SRAM_SLEEP_B_LSB                (1U << 4)       /* 5b */
#define UFS_SRAM_PDN_LSB                    (1U << 16)      /* 5b */
/* PERMI_TBL_RD_SRAM_CON (0x10006000+0x374) */
#define PERMI_TBL_RD_SRAM_CKISO_LSB         (1U << 0)       /* 1b */
#define PERMI_TBL_RD_SRAM_ISOINT_B_LSB      (1U << 1)       /* 2b */
#define PERMI_TBL_RD_SRAM_SLEEP_B_LSB       (1U << 4)       /* 2b */
#define PERMI_TBL_RD_SRAM_PDN_LSB           (1U << 16)      /* 2b */
/* PERMI_TBL_WR_SRAM_CON (0x10006000+0x378) */
#define PERMI_TBL_WR_SRAM_CKISO_LSB         (1U << 0)       /* 1b */
#define PERMI_TBL_WR_SRAM_ISOINT_B_LSB      (1U << 1)       /* 2b */
#define PERMI_TBL_WR_SRAM_SLEEP_B_LSB       (1U << 4)       /* 2b */
#define PERMI_TBL_WR_SRAM_PDN_LSB           (1U << 16)      /* 2b */
/* INVD_TBL_RD_SRAM_CON (0x10006000+0x37C) */
#define INVD_TBL_RD_SRAM_CKISO_LSB          (1U << 0)       /* 1b */
#define INVD_TBL_RD_SRAM_ISOINT_B_LSB       (1U << 1)       /* 1b */
#define INVD_TBL_RD_SRAM_SLEEP_B_LSB        (1U << 4)       /* 1b */
#define INVD_TBL_RD_SRAM_PDN_LSB            (1U << 16)      /* 1b */
/* INVD_TBL_WR_SRAM_CON (0x10006000+0x380) */
#define INVD_TBL_WR_SRAM_CKISO_LSB          (1U << 0)       /* 1b */
#define INVD_TBL_WR_SRAM_ISOINT_B_LSB       (1U << 1)       /* 1b */
#define INVD_TBL_WR_SRAM_SLEEP_B_LSB        (1U << 4)       /* 1b */
#define INVD_TBL_WR_SRAM_PDN_LSB            (1U << 16)      /* 1b */
/* VPU_VCORE_SRAM_CON (0x10006000+0x384) */
#define VPU_VCORE_SRAM_CKISO_LSB            (1U << 0)       /* 1b */
#define VPU_VCORE_SRAM_ISOINT_B_LSB         (1U << 1)       /* 1b */
#define VPU_VCORE_SRAM_SLEEP_B_LSB          (1U << 4)       /* 4b */
#define VPU_VCORE_SRAM_PDN_LSB              (1U << 16)      /* 4b */
#define VPU_VCORE_SRAM_SLEEP_B_ACK_LSB      (1U << 24)      /* 4b */
#define VPU_VCORE_SRAM_PDN_ACK_LSB          (1U << 28)      /* 4b */
/* VPU_CONN_SRAM_CON (0x10006000+0x388) */
#define VPU_CONN_SRAM_CKISO_LSB             (1U << 0)       /* 1b */
#define VPU_CONN_SRAM_ISOINT_B_LSB          (1U << 1)       /* 1b */
#define VPU_CONN_SRAM_SLEEP_B_LSB           (1U << 4)       /* 4b */
#define VPU_CONN_SRAM_PDN_LSB               (1U << 16)      /* 4b */
#define VPU_CONN_SRAM_SLEEP_B_ACK_LSB       (1U << 24)      /* 4b */
#define VPU_CONN_SRAM_PDN_ACK_LSB           (1U << 28)      /* 4b */
/* VPU_CORE0_SRAM_CON (0x10006000+0x38C) */
#define VPU_CORE0_SRAM_CKISO_LSB            (1U << 0)       /* 1b */
#define VPU_CORE0_SRAM_ISOINT_B_LSB         (1U << 1)       /* 1b */
#define VPU_CORE0_SRAM_SLEEP_B_LSB          (1U << 4)       /* 10b */
#define VPU_CORE0_SRAM_PDN_LSB              (1U << 16)      /* 12b */
/* VPU_CORE1_SRAM_CON (0x10006000+0x390) */
#define VPU_CORE1_SRAM_CKISO_LSB            (1U << 0)       /* 1b */
#define VPU_CORE1_SRAM_ISOINT_B_LSB         (1U << 1)       /* 1b */
#define VPU_CORE1_SRAM_SLEEP_B_LSB          (1U << 4)       /* 10b */
#define VPU_CORE1_SRAM_PDN_LSB              (1U << 16)      /* 12b */
/* VPU_CORE2_SRAM_CON (0x10006000+0x394) */
#define VPU_CORE2_SRAM_CKISO_LSB            (1U << 0)       /* 1b */
#define VPU_CORE2_SRAM_ISOINT_B_LSB         (1U << 1)       /* 1b */
#define VPU_CORE2_SRAM_SLEEP_B_LSB          (1U << 4)       /* 10b */
#define VPU_CORE2_SRAM_PDN_LSB              (1U << 16)      /* 12b */
/* VPU_CORE0_SRAM_STA (0x10006000+0x398) */
#define VPU_CORE0_SRAM_SLEEP_B_ACK_LSB      (1U << 4)       /* 10b */
#define VPU_CORE0_SRAM_PDN_ACK_LSB          (1U << 16)      /* 12b */
/* VPU_CORE1_SRAM_STA (0x10006000+0x39C) */
#define VPU_CORE1_SRAM_SLEEP_B_ACK_LSB      (1U << 4)       /* 10b */
#define VPU_CORE1_SRAM_PDN_ACK_LSB          (1U << 16)      /* 12b */
/* VPU_CORE2_SRAM_STA (0x10006000+0x3A0) */
#define VPU_CORE2_SRAM_SLEEP_B_ACK_LSB      (1U << 4)       /* 10b */
#define VPU_CORE2_SRAM_PDN_ACK_LSB          (1U << 16)      /* 12b */
/* DEBUGTOP_SRAM_CON (0x10006000+0x3A4) */
#define DEBUGTOP_SRAM_PDN_LSB               (1U << 0)       /* 1b */
/* USB_SRAM_CON (0x10006000+0x3A8) */
#define USB_SRAM_PDN_LSB                    (1U << 0)       /* 7b */
/* DUMMY_SRAM_CON (0x10006000+0x3AC) */
#define DUMMY_SRAM_CKISO_LSB                (1U << 0)       /* 1b */
#define DUMMY_SRAM_ISOINT_B_LSB             (1U << 1)       /* 1b */
#define DUMMY_SRAM_SLEEP_B_LSB              (1U << 4)       /* 8b */
#define DUMMY_SRAM_PDN_LSB                  (1U << 16)      /* 8b */
/* MD_EXT_BUCK_ISO_CON (0x10006000+0x3B0) */
#define VMODEM_EXT_BUCK_ISO_LSB             (1U << 0)       /* 1b */
#define VMD_EXT_BUCK_ISO_LSB                (1U << 1)       /* 1b */
/* EXT_BUCK_ISO (0x10006000+0x3B4) */
#define VIMVO_EXT_BUCK_ISO_LSB              (1U << 2)       /* 1b */
#define IPU_EXT_BUCK_ISO_LSB                (1U << 5)       /* 3b */
/* DXCC_SRAM_CON (0x10006000+0x3B8) */
#define DXCC_SRAM_CKISO_LSB                 (1U << 0)       /* 1b */
#define DXCC_SRAM_ISOINT_B_LSB              (1U << 1)       /* 1b */
#define DXCC_SRAM_SLEEP_B_LSB               (1U << 4)       /* 1b */
#define DXCC_SRAM_PDN_LSB                   (1U << 16)      /* 1b */
/* GCE_SRAM_CON (0x10006000+0x3BC) */
#define GCE_SRAM_CKISO_LSB                  (1U << 0)       /* 1b */
#define GCE_SRAM_ISOINT_B_LSB               (1U << 1)       /* 1b */
#define GCE_SRAM_SLEEP_B_LSB                (1U << 4)       /* 1b */
#define GCE_SRAM_PDN_LSB                    (1U << 16)      /* 1b */
/* MSDC_SRAM_CON (0x10006000+0x3C0) */
#define MSDC_SRAM_CKISO_LSB                 (1U << 0)       /* 1b */
#define MSDC_SRAM_ISOINT_B_LSB              (1U << 1)       /* 1b */
#define MSDC_SRAM_SLEEP_B_LSB               (1U << 4)       /* 5b */
#define MSDC_SRAM_PDN_LSB                   (1U << 16)      /* 5b */
/* SPM_MAS_PAUSE_MASK_B (0x10006000+0x400) */
#define SPM_MAS_PAUSE_MASK_B_LSB            (1U << 0)       /* 32b */
/* SPM_MAS_PAUSE2_MASK_B (0x10006000+0x404) */
#define SPM_MAS_PAUSE2_MASK_B_LSB           (1U << 0)       /* 32b */
/* SPM_MAS_PAUSE3_MASK_B (0x10006000+0x408) */
#define SPM_MAS_PAUSE3_MASK_B_LSB           (1U << 0)       /* 32b */
/* SPM_MAS_PAUSE_MM_MASK_B (0x10006000+0x40C) */
#define SPM_MAS_PAUSE_MM_MASK_B_LSB         (1U << 0)       /* 16b */
/* SPM_MAS_PAUSE_MCU_MASK_B (0x10006000+0x410) */
#define SPM_MAS_PAUSE_MCU_MASK_B_LSB        (1U << 0)       /* 16b */
/* SPM2SW_MAILBOX_0 (0x10006000+0x414) */
#define SPM2SW_MAILBOX_0_LSB                (1U << 0)       /* 32b */
/* SPM2SW_MAILBOX_1 (0x10006000+0x418) */
#define SPM2SW_MAILBOX_1_LSB                (1U << 0)       /* 32b */
/* SPM2SW_MAILBOX_2 (0x10006000+0x41C) */
#define SPM2SW_MAILBOX_2_LSB                (1U << 0)       /* 32b */
/* SPM2SW_MAILBOX_3 (0x10006000+0x420) */
#define SPM2SW_MAILBOX_3_LSB                (1U << 0)       /* 32b */
/* SW2SPM_MAILBOX_0 (0x10006000+0x424) */
#define SW2SPM_MAILBOX_0_LSB                (1U << 0)       /* 32b */
/* SW2SPM_MAILBOX_1 (0x10006000+0x428) */
#define SW2SPM_MAILBOX_1_LSB                (1U << 0)       /* 32b */
/* SW2SPM_MAILBOX_2 (0x10006000+0x42C) */
#define SW2SPM_MAILBOX_2_LSB                (1U << 0)       /* 32b */
/* SW2SPM_MAILBOX_3 (0x10006000+0x430) */
#define SW2SPM_MAILBOX_3_LSB                (1U << 0)       /* 32b */
/* AP_MDSRC_REQ (0x10006000+0x434) */
#define AP_MDSMSRC_REQ_LSB                  (1U << 0)       /* 1b */
#define AP_L1SMSRC_REQ_LSB                  (1U << 1)       /* 1b */
#define AP_MD2SRC_REQ_LSB                   (1U << 2)       /* 1b */
#define AP_MDSMSRC_ACK_LSB                  (1U << 4)       /* 1b */
#define AP_L1SMSRC_ACK_LSB                  (1U << 5)       /* 1b */
#define AP_MD2SRC_ACK_LSB                   (1U << 6)       /* 1b */
/* SPM2MD_DVFS_CON (0x10006000+0x438) */
#define SPM2MD_DVFS_CON_LSB                 (1U << 0)       /* 32b */
/* MD2SPM_DVFS_CON (0x10006000+0x43C) */
#define MD2SPM_DVFS_CON_LSB                 (1U << 0)       /* 32b */
/* ULPOSC_CON (0x10006000+0x440) */
#define ULPOSC_EN_LSB                       (1U << 0)       /* 1b */
#define ULPOSC_RST_LSB                      (1U << 1)       /* 1b */
#define ULPOSC_CG_EN_LSB                    (1U << 2)       /* 1b */
#define ULPOSC_CLK_SEL_LSB                  (1U << 3)       /* 1b */
/* SPM2CONN_MAILBOX (0x10006000+0x444) */
#define SPM2CONN_MAILBOX_LSB                (1U << 0)       /* 32b */
/* SPM_SWINT (0x10006000+0x448) */
#define SPM_SWINT_LSB                       (1U << 0)       /* 32b */
/* SPM_SWINT_SET (0x10006000+0x44C) */
#define SPM_SWINT_SET_LSB                   (1U << 0)       /* 32b */
/* SPM_SWINT_CLR (0x10006000+0x450) */
#define SPM_SWINT_CLR_LSB                   (1U << 0)       /* 32b */
/* AP2MD_PEER_WAKEUP (0x10006000+0x454) */
#define AP2MD_PEER_WAKEUP_LSB               (1U << 0)       /* 1b */
/* SPM_PLL_CON (0x10006000+0x458) */
#define SC_MAINPLLOUT_OFF_LSB               (1U << 0)       /* 1b */
#define SC_UNIPLLOUT_OFF_LSB                (1U << 1)       /* 1b */
#define SC_MAINPLL_OFF_LSB                  (1U << 4)       /* 1b */
#define SC_UNIPLL_OFF_LSB                   (1U << 5)       /* 1b */
#define SC_MAINPLL_S_OFF_LSB                (1U << 8)       /* 1b */
#define SC_UNIPLL_S_OFF_LSB                 (1U << 9)       /* 1b */
#define SC_SMI_CK_OFF_LSB                   (1U << 16)      /* 1b */
#define SC_MD32K_CK_OFF_LSB                 (1U << 17)      /* 1b */
#define SC_CKSQ1_OFF_LSB                    (1U << 18)      /* 1b */
/* SPM_S1_MODE_CH (0x10006000+0x45C) */
#define SPM_S1_MODE_CH_LSB                  (1U << 0)       /* 2b */
#define S1_EMI_CK_SWITCH_LSB                (1U << 8)       /* 2b */
#define SPM_S1_MODE_CH_EMI_SELF_REFRESH_CH_LSB (1U << 16)      /* 2b */
/* DRAMC_DPY_CLK_SW_CON_SEL (0x10006000+0x460) */
#define SW_DR_GATE_RETRY_EN_SEL_LSB         (1U << 0)       /* 2b */
#define SW_EMI_CLK_OFF_SEL_LSB              (1U << 2)       /* 2b */
#define SW_DPY_MODE_SW_SEL_LSB              (1U << 4)       /* 2b */
#define SW_DMSUS_OFF_SEL_LSB                (1U << 6)       /* 2b */
#define SW_MEM_CK_OFF_SEL_LSB               (1U << 8)       /* 2b */
#define SW_DPY_2ND_DLL_EN_SEL_LSB           (1U << 10)      /* 2b */
#define SW_DPY_DLL_EN_SEL_LSB               (1U << 12)      /* 2b */
#define SW_DPY_DLL_CK_EN_SEL_LSB            (1U << 14)      /* 2b */
#define SW_DPY_VREF_EN_SEL_LSB              (1U << 16)      /* 2b */
#define SW_PHYPLL_EN_SEL_LSB                (1U << 18)      /* 2b */
#define SW_DDRPHY_FB_CK_EN_SEL_LSB          (1U << 20)      /* 2b */
#define SW_DMDRAMCSHU_ACK_SEL_LSB           (1U << 24)      /* 2b */
#define SW_EMI_CLK_OFF_ACK_SEL_LSB          (1U << 26)      /* 2b */
#define SW_DR_SHORT_QUEUE_ACK_SEL_LSB       (1U << 28)      /* 2b */
#define SW_DRAMC_DFS_STA_SEL_LSB            (1U << 30)      /* 2b */
/* DRAMC_DPY_CLK_SW_CON (0x10006000+0x464) */
#define SW_DR_GATE_RETRY_EN_LSB             (1U << 0)       /* 2b */
#define SW_EMI_CLK_OFF_LSB                  (1U << 2)       /* 2b */
#define SW_DPY_MODE_SW_LSB                  (1U << 4)       /* 2b */
#define SW_DMSUS_OFF_LSB                    (1U << 6)       /* 2b */
#define SW_MEM_CK_OFF_LSB                   (1U << 8)       /* 2b */
#define SW_DPY_2ND_DLL_EN_LSB               (1U << 10)      /* 2b */
#define SW_DPY_DLL_EN_LSB                   (1U << 12)      /* 2b */
#define SW_DPY_DLL_CK_EN_LSB                (1U << 14)      /* 2b */
#define SW_DPY_VREF_EN_LSB                  (1U << 16)      /* 2b */
#define SW_PHYPLL_EN_LSB                    (1U << 18)      /* 2b */
#define SW_DDRPHY_FB_CK_EN_LSB              (1U << 20)      /* 2b */
#define SC_DR_SHU_EN_ACK_LSB                (1U << 24)      /* 2b */
#define EMI_CLK_OFF_ACK_LSB                 (1U << 26)      /* 2b */
#define SC_DR_SHORT_QUEUE_ACK_LSB           (1U << 28)      /* 2b */
#define SC_DRAMC_DFS_STA_LSB                (1U << 30)      /* 2b */
/* DRAMC_DPY_CLK_SW_CON_SEL2 (0x10006000+0x468) */
#define SW_PHYPLL_SHU_EN_SEL_LSB            (1U << 0)       /* 1b */
#define SW_PHYPLL2_SHU_EN_SEL_LSB           (1U << 1)       /* 1b */
#define SW_PHYPLL_MODE_SW_SEL_LSB           (1U << 2)       /* 1b */
#define SW_PHYPLL2_MODE_SW_SEL_LSB          (1U << 3)       /* 1b */
#define SW_DR_SHORT_QUEUE_SEL_LSB           (1U << 4)       /* 1b */
#define SW_DR_SHU_EN_SEL_LSB                (1U << 5)       /* 1b */
#define SW_DR_SHU_LEVEL_SEL_LSB             (1U << 6)       /* 1b */
#define SW_DR_SHU_LEVEL_SRAM_CH0_SEL_LSB    (1U << 8)       /* 1b */
#define SW_DR_SHU_LEVEL_SRAM_CH1_SEL_LSB    (1U << 12)      /* 1b */
#define SW_DPY_BCLK_ENABLE_SEL_LSB          (1U << 16)      /* 2b */
#define SW_SHU_RESTORE_SEL_LSB              (1U << 18)      /* 2b */
#define SW_DPHY_PRECAL_UP_SEL_LSB           (1U << 20)      /* 2b */
#define SW_DPHY_RXDLY_TRACKING_EN_SEL_LSB   (1U << 22)      /* 2b */
#define SW_TX_TRACKING_DIS_SEL_LSB          (1U << 24)      /* 2b */
#define SW_DMYRD_MOD_SEL_LSB                (1U << 26)      /* 2b */
#define SW_DMYRD_INTV_SEL_LSB               (1U << 28)      /* 2b */
#define SW_DMYRD_EN_SEL_LSB                 (1U << 30)      /* 2b */
/* DRAMC_DPY_CLK_SW_CON2 (0x10006000+0x46C) */
#define SW_PHYPLL_SHU_EN_LSB                (1U << 0)       /* 1b */
#define SW_PHYPLL2_SHU_EN_LSB               (1U << 1)       /* 1b */
#define SW_PHYPLL_MODE_SW_LSB               (1U << 2)       /* 1b */
#define SW_PHYPLL2_MODE_SW_LSB              (1U << 3)       /* 1b */
#define SW_DR_SHORT_QUEUE_LSB               (1U << 4)       /* 1b */
#define SW_DR_SHU_EN_LSB                    (1U << 5)       /* 1b */
#define SW_DR_SHU_LEVEL_LSB                 (1U << 6)       /* 2b */
#define SW_DR_SHU_LEVEL_SRAM_CH0_LSB        (1U << 8)       /* 4b */
#define SW_DR_SHU_LEVEL_SRAM_CH1_LSB        (1U << 12)      /* 4b */
#define SW_DPY_BCLK_ENABLE_LSB              (1U << 16)      /* 2b */
#define SW_SHU_RESTORE_LSB                  (1U << 18)      /* 2b */
#define SW_DPHY_PRECAL_UP_LSB               (1U << 20)      /* 2b */
#define SW_DPHY_RXDLY_TRACKING_EN_LSB       (1U << 22)      /* 2b */
#define SW_TX_TRACKING_DIS_LSB              (1U << 24)      /* 2b */
#define SW_DMYRD_MOD_LSB                    (1U << 26)      /* 2b */
#define SW_DMYRD_INTV_LSB                   (1U << 28)      /* 2b */
#define SW_DMYRD_EN_LSB                     (1U << 30)      /* 2b */
/* DRAMC_DPY_CLK_SW_CON_SEL3 (0x10006000+0x470) */
#define SW_DR_SRAM_LOAD_SEL_LSB             (1U << 0)       /* 2b */
#define SW_DR_SRAM_RESTORE_SEL_LSB          (1U << 2)       /* 2b */
#define SW_DR_SHU_LEVEL_SRAM_LATCH_SEL_LSB  (1U << 4)       /* 2b */
#define SW_DRS_DIS_REQ_SEL_LSB              (1U << 6)       /* 2b */
#define SW_TX_TRACK_RETRY_EN_SEL_LSB        (1U << 8)       /* 2b */
#define SW_DR_RESERVED_0_SEL_LSB            (1U << 10)      /* 2b */
#define SW_DR_RESERVED_1_SEL_LSB            (1U << 12)      /* 2b */
#define SW_DR_RESERVED_2_SEL_LSB            (1U << 14)      /* 2b */
#define SW_DR_RESERVED_3_SEL_LSB            (1U << 16)      /* 2b */
#define SW_DR_RESERVED_4_SEL_LSB            (1U << 18)      /* 2b */
#define SW_DR_SRAM_LOAD_ACK_SEL_LSB         (1U << 20)      /* 2b */
#define SW_DR_SRAM_PLL_LOAD_ACK_SEL_LSB     (1U << 22)      /* 2b */
#define SW_DR_SRAM_RESTORE_ACK_SEL_LSB      (1U << 24)      /* 2b */
#define SW_DRS_DIS_ACK_SEL_LSB              (1U << 26)      /* 2b */
/* DRAMC_DPY_CLK_SW_CON3 (0x10006000+0x474) */
#define SW_DR_SRAM_LOAD_LSB                 (1U << 0)       /* 2b */
#define SW_DR_SRAM_RESTORE_LSB              (1U << 2)       /* 2b */
#define SW_DR_SHU_LEVEL_SRAM_LATCH_LSB      (1U << 4)       /* 2b */
#define SW_DRS_DIS_REQ_LSB                  (1U << 6)       /* 2b */
#define SW_TX_TRACK_RETRY_EN_LSB            (1U << 8)       /* 2b */
#define SW_DR_RESERVED_0_LSB                (1U << 10)      /* 2b */
#define SW_DR_RESERVED_1_LSB                (1U << 12)      /* 2b */
#define SW_DR_RESERVED_2_LSB                (1U << 14)      /* 2b */
#define SW_DR_RESERVED_3_LSB                (1U << 16)      /* 2b */
#define SW_DR_RESERVED_4_LSB                (1U << 18)      /* 2b */
#define SC_DR_SRAM_LOAD_ACK_LSB             (1U << 20)      /* 2b */
#define SC_DR_SRAM_PLL_LOAD_ACK_LSB         (1U << 22)      /* 2b */
#define SC_DR_SRAM_RESTORE_ACK_LSB          (1U << 24)      /* 2b */
#define SC_DRS_DIS_ACK_LSB                  (1U << 26)      /* 2b */
/* DRAMC_DPY_CLK_SPM_CON (0x10006000+0x478) */
#define SC_DMYRD_EN_MOD_SEL_PCM_LSB         (1U << 0)       /* 1b */
#define SC_DMYRD_INTV_SEL_PCM_LSB           (1U << 1)       /* 1b */
#define SC_DMYRD_EN_PCM_LSB                 (1U << 2)       /* 1b */
#define SC_DRS_DIS_REQ_PCM_LSB              (1U << 3)       /* 1b */
#define SC_DR_GATE_RETRY_EN_PCM_LSB         (1U << 4)       /* 1b */
#define SC_DR_SHU_LEVEL_SRAM_CH0_PCM_LSB    (1U << 5)       /* 4b */
#define SC_DR_SHU_LEVEL_SRAM_CH1_PCM_LSB    (1U << 9)       /* 4b */
#define SC_DR_SHORT_QUEUE_PCM_LSB           (1U << 13)      /* 1b */
#define SC_DR_RESERVED_0_PCM_LSB            (1U << 14)      /* 1b */
#define SC_DR_RESERVED_1_PCM_LSB            (1U << 15)      /* 1b */
#define SC_DR_RESERVED_2_PCM_LSB            (1U << 16)      /* 1b */
#define SC_DR_RESERVED_3_PCM_LSB            (1U << 17)      /* 1b */
#define SC_DR_RESERVED_4_PCM_LSB            (1U << 18)      /* 1b */
#define SC_DR_RESERVED_5_PCM_LSB            (1U << 19)      /* 1b */
#define SC_DR_SRAM_LOAD_ACK_ALL_LSB         (1U << 20)      /* 1b */
#define SC_DR_SRAM_PLL_LOAD_ACK_ALL_LSB     (1U << 21)      /* 1b */
#define SC_DR_SRAM_RESTORE_ACK_ALL_LSB      (1U << 22)      /* 1b */
#define SC_DRS_DIS_ACK_ALL_LSB              (1U << 23)      /* 1b */
#define DR_SHORT_QUEUE_ACK_ALL_LSB          (1U << 24)      /* 1b */
#define DRAMC_SPCMD_APSRC_REQ_LSB           (1U << 25)      /* 1b */
/* SPM_MEM_CK_SEL (0x10006000+0x47C) */
#define SC_MEM_CK_SEL_LSB                   (1U << 0)       /* 1b */
/* SPM_SEMA_M0 (0x10006000+0x480) */
#define SPM_SEMA_M0_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M1 (0x10006000+0x484) */
#define SPM_SEMA_M1_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M2 (0x10006000+0x488) */
#define SPM_SEMA_M2_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M3 (0x10006000+0x48C) */
#define SPM_SEMA_M3_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M4 (0x10006000+0x490) */
#define SPM_SEMA_M4_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M5 (0x10006000+0x494) */
#define SPM_SEMA_M5_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M6 (0x10006000+0x498) */
#define SPM_SEMA_M6_LSB                     (1U << 0)       /* 8b */
/* SPM_SEMA_M7 (0x10006000+0x49C) */
#define SPM_SEMA_M7_LSB                     (1U << 0)       /* 8b */
/* SPM_AP_SEMA (0x10006000+0x4A0) */
#define SPM_AP_SEMA_LSB                     (1U << 0)       /* 1b */
/* SPM_SPM_SEMA (0x10006000+0x4A4) */
#define SPM_SPM_SEMA_LSB                    (1U << 0)       /* 1b */
/* SPM_SSPM_CON (0x10006000+0x4A8) */
#define SC_SPM2SSPM_WAKEUP_LSB              (1U << 0)       /* 4b */
#define SPM_SSPM_CON_SC_SSPM2SPM_WAKEUP_LSB (1U << 4)       /* 4b */
#define REG_MD32_26M_CK_SEL_LSB             (1U << 8)       /* 1b */
#define REG_MD32_DCM_EN_LSB                 (1U << 9)       /* 1b */
/* SPM_SCP_CON (0x10006000+0x4AC) */
#define SC_SPM2SCP_WAKEUP_LSB               (1U << 0)       /* 1b */
#define SPM_SCP_CON_SC_SCP2SPM_WAKEUP_LSB   (1U << 4)       /* 1b */
#define REG_SCP_26M_CK_SEL_LSB              (1U << 5)       /* 1b */
#define REG_SCP_DCM_EN_LSB                  (1U << 6)       /* 1b */
#define SCP_SECURE_V_REQ_MASK_LSB           (1U << 7)       /* 1b */
#define SCP_SLP_REQ_LSB                     (1U << 8)       /* 1b */
#define SCP_SLP_ACK_LSB                     (1U << 9)       /* 1b */
/* SPM_ADSP_CON (0x10006000+0x4B0) */
#define SC_SPM2ADSP_WAKEUP_LSB              (1U << 0)       /* 1b */
#define SPM_ADSP_CON_SC_ADSP2SPM_WAKEUP_LSB (1U << 4)       /* 1b */
#define ADSP_CLK_ON_LSB                     (1U << 5)       /* 1b */
/* SPM2ADSP_MAILBOX (0x10006000+0x4B4) */
#define SPM2ADSP_MAILBOX_LSB                (1U << 0)       /* 32b */
/* ADSP2SPM_MAILBOX (0x10006000+0x4B8) */
#define ADSP2SPM_MAILBOX_LSB                (1U << 0)       /* 32b */
/* SPM_SCP_MAILBOX (0x10006000+0x4BC) */
#define SPM_SCP_MAILBOX_LSB                 (1U << 0)       /* 32b */
/* SCP_SPM_MAILBOX (0x10006000+0x4C0) */
#define SCP_SPM_MAILBOX_LSB                 (1U << 0)       /* 32b */
/* SPM2SSPM_MAILBOX_0 (0x10006000+0x4C4) */
#define SPM2SSPM_MAILBOX_0_LSB              (1U << 0)       /* 32b */
/* SPM2SSPM_MAILBOX_1 (0x10006000+0x4C8) */
#define SPM2SSPM_MAILBOX_1_LSB              (1U << 0)       /* 32b */
/* SPM2SSPM_MAILBOX_2 (0x10006000+0x4CC) */
#define SPM2SSPM_MAILBOX_2_LSB              (1U << 0)       /* 32b */
/* SPM2SSPM_MAILBOX_3 (0x10006000+0x4D0) */
#define SPM2SSPM_MAILBOX_3_LSB              (1U << 0)       /* 32b */
/* SSPM2SPM_MAILBOX_0 (0x10006000+0x4D4) */
#define SSPM2SPM_MAILBOX_0_LSB              (1U << 0)       /* 32b */
/* SSPM2SPM_MAILBOX_1 (0x10006000+0x4D8) */
#define SSPM2SPM_MAILBOX_1_LSB              (1U << 0)       /* 32b */
/* SSPM2SPM_MAILBOX_2 (0x10006000+0x4DC) */
#define SSPM2SPM_MAILBOX_2_LSB              (1U << 0)       /* 32b */
/* SSPM2SPM_MAILBOX_3 (0x10006000+0x4E0) */
#define SSPM2SPM_MAILBOX_3_LSB              (1U << 0)       /* 32b */
/* SPM_CIRQ_CON (0x10006000+0x4E4) */
#define CIRQ_CLK_SEL_LSB                    (1U << 0)       /* 1b */
/* SW2SPM_INT (0x10006000+0x4E8) */
#define SW2SPM_INT_SW2SPM_INT_LSB           (1U << 0)       /* 4b */
/* SW2SPM_INT_SET (0x10006000+0x4EC) */
#define SW2SPM_INT_SET_LSB                  (1U << 0)       /* 4b */
/* SW2SPM_INT_CLR (0x10006000+0x4F0) */
#define SW2SPM_INT_CLR_LSB                  (1U << 0)       /* 4b */
/* SPM_DVFS_MISC (0x10006000+0x4F4) */
#define MSDC_DVFS_REQUEST_LSB               (1U << 0)       /* 1b */
#define SPM2EMI_SLP_PROT_EN_LSB             (1U << 1)       /* 1b */
#define SPM_DVFS_FORCE_ENABLE_LSB           (1U << 2)       /* 1b */
#define FORCE_DVFS_WAKE_LSB                 (1U << 3)       /* 1b */
#define SPM_DVFSRC_ENABLE_LSB               (1U << 4)       /* 1b */
#define SPM_DVFS_HISTORY_EN_LSB             (1U << 5)       /* 1b */
#define DVFSRC_IRQ_WAKEUP_EVENT_MASK_LSB    (1U << 6)       /* 1b */
#define SPM2RC_EVENT_ABORT_LSB              (1U << 7)       /* 1b */
#define MM_DVFS_HALT_LSB                    (1U << 8)       /* 5b */
#define EMI_SLP_IDLE_LSB                    (1U << 14)      /* 1b */
#define SDIO_READY_TO_SPM_LSB               (1U << 15)      /* 1b */
#define SC_MM_CK_SEL_LSB                    (1U << 16)      /* 4b */
#define SC_MM_CK_SEL_EN_LSB                 (1U << 20)      /* 1b */
#define SPM2MM_FORCE_ULTRA_LSB              (1U << 21)      /* 1b */
#define SPM2MM_DBL_OSTD_ACT_LSB             (1U << 22)      /* 1b */
#define SPM2MM_ULTRAREQ_LSB                 (1U << 23)      /* 1b */
#define SPM2MD_ULTRAREQ_LSB                 (1U << 24)      /* 1b */
#define SPM2ISP_ULTRAREQ_LSB                (1U << 25)      /* 1b */
#define MM2SPM_FORCE_ULTRA_ACK_D2T_LSB      (1U << 26)      /* 1b */
#define MM2SPM_DBL_OSTD_ACT_ACK_D2T_LSB     (1U << 27)      /* 1b */
#define SPM2ISP_ULTRAACK_D2T_LSB            (1U << 28)      /* 1b */
#define SPM2MM_ULTRAACK_D2T_LSB             (1U << 29)      /* 1b */
#define SPM2MD_ULTRAACK_D2T_LSB             (1U << 30)      /* 1b */
/* SCP_VCORE_LEVEL (0x10006000+0x4F8) */
#define SCP_VCORE_LEVEL_LSB                 (1U << 0)       /* 16b */
/* CONN2SPM_MAILBOX (0x10006000+0x4FC) */
#define CONN2SPM_MAILBOX_LSB                (1U << 0)       /* 32b */
/* SRCLKEN_RC_CFG (0x10006000+0x500) */
#define SRCLKEN_RC_CFG_LSB                  (1U << 0)       /* 32b */
/* RC_CENTRAL_CFG1 (0x10006000+0x504) */
#define RC_CENTRAL_CFG1_LSB                 (1U << 0)       /* 32b */
/* RC_CENTRAL_CFG2 (0x10006000+0x508) */
#define RC_CENTRAL_CFG2_LSB                 (1U << 0)       /* 32b */
/* RC_CMD_ARB_CFG (0x10006000+0x50C) */
#define RC_CMD_ARB_CFG_LSB                  (1U << 0)       /* 32b */
/* RC_PMIC_RCEN_ADDR (0x10006000+0x510) */
#define RC_PMIC_RCEN_ADDR_LSB               (1U << 0)       /* 16b */
#define RC_PMIC_RCEN_RESERVE_LSB            (1U << 16)      /* 16b */
/* RC_PMIC_RCEN_SET_CLR_ADDR (0x10006000+0x514) */
#define RC_PMIC_RCEN_SET_ADDR_LSB           (1U << 0)       /* 16b */
#define RC_PMIC_RCEN_CLR_ADDR_LSB           (1U << 16)      /* 16b */
/* RC_DCXO_FPM_CFG (0x10006000+0x518) */
#define RC_DCXO_FPM_CFG_LSB                 (1U << 0)       /* 32b */
/* RC_CENTRAL_CFG3 (0x10006000+0x51C) */
#define RC_CENTRAL_CFG3_LSB                 (1U << 0)       /* 32b */
/* RC_M00_SRCLKEN_CFG (0x10006000+0x520) */
#define RC_M00_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M01_SRCLKEN_CFG (0x10006000+0x524) */
#define RC_M01_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M02_SRCLKEN_CFG (0x10006000+0x528) */
#define RC_M02_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M03_SRCLKEN_CFG (0x10006000+0x52C) */
#define RC_M03_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M04_SRCLKEN_CFG (0x10006000+0x530) */
#define RC_M04_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M05_SRCLKEN_CFG (0x10006000+0x534) */
#define RC_M05_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M06_SRCLKEN_CFG (0x10006000+0x538) */
#define RC_M06_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M07_SRCLKEN_CFG (0x10006000+0x53C) */
#define RC_M07_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M08_SRCLKEN_CFG (0x10006000+0x540) */
#define RC_M08_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M09_SRCLKEN_CFG (0x10006000+0x544) */
#define RC_M09_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M10_SRCLKEN_CFG (0x10006000+0x548) */
#define RC_M10_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M11_SRCLKEN_CFG (0x10006000+0x54C) */
#define RC_M11_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_M12_SRCLKEN_CFG (0x10006000+0x550) */
#define RC_M12_SRCLKEN_CFG_LSB              (1U << 0)       /* 32b */
/* RC_FSM_STA_0 (0x10006000+0x560) */
#define RC_FSM_STA_0_LSB                    (1U << 0)       /* 32b */
/* RC_CMD_STA_0 (0x10006000+0x564) */
#define RC_CMD_STA_0_LSB                    (1U << 0)       /* 32b */
/* RC_CMD_STA_1 (0x10006000+0x568) */
#define RC_CMD_STA_1_LSB                    (1U << 0)       /* 32b */
/* RC_SPI_STA_0 (0x10006000+0x56C) */
#define RC_SPI_STA_0_LSB                    (1U << 0)       /* 32b */
/* RC_PI_PO_STA_0 (0x10006000+0x570) */
#define RC_PI_PO_STA_0_LSB                  (1U << 0)       /* 32b */
/* RC_M00_REQ_STA_0 (0x10006000+0x580) */
#define RC_M00_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M01_REQ_STA_0 (0x10006000+0x584) */
#define RC_M01_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M02_REQ_STA_0 (0x10006000+0x588) */
#define RC_M02_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M03_REQ_STA_0 (0x10006000+0x58C) */
#define RC_M03_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M04_REQ_STA_0 (0x10006000+0x590) */
#define RC_M04_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M05_REQ_STA_0 (0x10006000+0x594) */
#define RC_M05_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M06_REQ_STA_0 (0x10006000+0x598) */
#define RC_M06_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M07_REQ_STA_0 (0x10006000+0x59C) */
#define RC_M07_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M08_REQ_STA_0 (0x10006000+0x5A0) */
#define RC_M08_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M09_REQ_STA_0 (0x10006000+0x5A4) */
#define RC_M09_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M10_REQ_STA_0 (0x10006000+0x5A8) */
#define RC_M10_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M11_REQ_STA_0 (0x10006000+0x5AC) */
#define RC_M11_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_M12_REQ_STA_0 (0x10006000+0x5B0) */
#define RC_M12_REQ_STA_0_LSB                (1U << 0)       /* 32b */
/* RC_MISC_0 (0x10006000+0x5B4) */
#define SRCVOLTEN_LSB                       (1U << 0)       /* 1b */
#define SPM_SRCVOLTEN_LSB                   (1U << 1)       /* 1b */
#define RC_VREQ_LSB                         (1U << 2)       /* 1b */
#define RC_SPM_SRCCLKENO_0_ACK_LSB          (1U << 3)       /* 1b */
/* RC_SPM_CTRL (0x10006000+0x5B8) */
#define SPM_AP_26M_RDY_LSB                  (1U << 0)       /* 1b */
#define KEEP_RC_SPI_ACTIVE_LSB              (1U << 1)       /* 1b */
#define SPM2RC_DMY_CTRL_LSB                 (1U << 2)       /* 6b */
/* SUBSYS_INTF_CFG (0x10006000+0x5BC) */
#define SRCLKEN_FPM_MASK_B_LSB              (1U << 0)       /* 13b */
#define SRCLKEN_BBLPM_MASK_B_LSB            (1U << 16)      /* 13b */
/* RC_DEBUG_TRACE_0_LSB (0x10006000+0x5C0) */
#define RO_PMRC_TRACE_00_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_0_MSB (0x10006000+0x5C4) */
#define RO_PMRC_TRACE_00_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_1_LSB (0x10006000+0x5C8) */
#define RO_PMRC_TRACE_01_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_1_MSB (0x10006000+0x5CC) */
#define RO_PMRC_TRACE_01_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_2_LSB (0x10006000+0x5D0) */
#define RO_PMRC_TRACE_02_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_2_MSB (0x10006000+0x5D4) */
#define RO_PMRC_TRACE_02_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_3_LSB (0x10006000+0x5D8) */
#define RO_PMRC_TRACE_03_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_3_MSB (0x10006000+0x5DC) */
#define RO_PMRC_TRACE_03_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_4_LSB (0x10006000+0x5E0) */
#define RO_PMRC_TRACE_04_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_4_MSB (0x10006000+0x5E4) */
#define RO_PMRC_TRACE_04_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_5_LSB (0x10006000+0x5E8) */
#define RO_PMRC_TRACE_05_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_5_MSB (0x10006000+0x5EC) */
#define RO_PMRC_TRACE_05_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_6_LSB (0x10006000+0x5F0) */
#define RO_PMRC_TRACE_06_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_6_MSB (0x10006000+0x5F4) */
#define RO_PMRC_TRACE_06_MSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_7_LSB (0x10006000+0x5F8) */
#define RO_PMRC_TRACE_07_LSB_LSB            (1U << 0)       /* 32b */
/* RC_DEBUG_TRACE_7_MSB (0x10006000+0x5FC) */
#define RO_PMRC_TRACE_07_MSB_LSB            (1U << 0)       /* 32b */
/* SPM_SW_FLAG_0 (0x10006000+0x600) */
#define SPM_SW_FLAG_0_LSB                   (1U << 0)       /* 32b */
/* SPM_SW_FLAG_1 (0x10006000+0x604) */
#define SPM_SW_FLAG_1_LSB                   (1U << 0)       /* 32b */
/* SPM_SW_RSV_0 (0x10006000+0x608) */
#define SPM_SW_RSV_0_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_1 (0x10006000+0x60C) */
#define SPM_SW_RSV_1_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_2 (0x10006000+0x610) */
#define SPM_SW_RSV_2_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_3 (0x10006000+0x614) */
#define SPM_SW_RSV_3_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_4 (0x10006000+0x618) */
#define SPM_SW_RSV_4_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_5 (0x10006000+0x61C) */
#define SPM_SW_RSV_5_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_6 (0x10006000+0x620) */
#define SPM_SW_RSV_6_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_7 (0x10006000+0x624) */
#define SPM_SW_RSV_7_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_8 (0x10006000+0x628) */
#define SPM_SW_RSV_8_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_9 (0x10006000+0x62C) */
#define SPM_SW_RSV_9_LSB                    (1U << 0)       /* 32b */
/* SPM_SW_RSV_10 (0x10006000+0x630) */
#define SPM_SW_RSV_10_LSB                   (1U << 0)       /* 32b */
/* SPM_SW_RSV_11 (0x10006000+0x634) */
#define SPM_SW_RSV_11_LSB                   (1U << 0)       /* 32b */
/* SPM_SW_RSV_18 (0x10006000+0x638) */
#define SPM_SW_RSV_18_LSB                   (1U << 0)       /* 32b */
/* SPM_SW_RSV_19 (0x10006000+0x63C) */
#define SPM_SW_RSV_19_LSB                   (1U << 0)       /* 32b */
/* SPM_RSV_CON_0 (0x10006000+0x640) */
#define SPM_RSV_CON_0_LSB                   (1U << 0)       /* 32b */
/* SPM_RSV_CON_1 (0x10006000+0x644) */
#define SPM_RSV_CON_1_LSB                   (1U << 0)       /* 32b */
/* SPM_RSV_STA_0 (0x10006000+0x648) */
#define SPM_RSV_STA_0_LSB                   (1U << 0)       /* 32b */
/* SPM_RSV_STA_1 (0x10006000+0x64C) */
#define SPM_RSV_STA_1_LSB                   (1U << 0)       /* 32b */
/* SPM_SPARE_CON (0x10006000+0x650) */
#define SPM_SPARE_CON_LSB                   (1U << 0)       /* 32b */
/* SPM_SPARE_CON_SET (0x10006000+0x654) */
#define SPM_SPARE_CON_SET_LSB               (1U << 0)       /* 32b */
/* SPM_SPARE_CON_CLR (0x10006000+0x658) */
#define SPM_SPARE_CON_CLR_LSB               (1U << 0)       /* 32b */
/* SPM_DV_CON_0 (0x10006000+0x65C) */
#define SPM_DV_CON_0_LSB                    (1U << 0)       /* 32b */
/* SPM_DV_CON_1 (0x10006000+0x660) */
#define SPM_DV_CON_1_LSB                    (1U << 0)       /* 32b */
/* SPM_FORCE_DVFS (0x10006000+0x664) */
#define SPM_FORCE_DVFS_LSB                  (1U << 0)       /* 32b */
/* INFRA2SPM_DEEPIDLE_CG_CHECK_0_MASK (0x10006000+0x668) */
#define INFRA2SPM_DEEPIDLE_CG_CHECK_0_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_DEEPIDLE_CG_CHECK_1_MASK (0x10006000+0x66C) */
#define INFRA2SPM_DEEPIDLE_CG_CHECK_1_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_DEEPIDLE_CG_CHECK_2_MASK (0x10006000+0x670) */
#define INFRA2SPM_DEEPIDLE_CG_CHECK_2_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_DEEPIDLE_CG_CHECK_3_MASK (0x10006000+0x674) */
#define INFRA2SPM_DEEPIDLE_CG_CHECK_3_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_DEEPIDLE_CG_CHECK_4_MASK (0x10006000+0x678) */
#define INFRA2SPM_DEEPIDLE_CG_CHECK_4_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_SODI_CG_CHECK_0_MASK (0x10006000+0x67C) */
#define INFRA2SPM_SODI_CG_CHECK_0_MASK_LSB  (1U << 0)       /* 32b */
/* INFRA2SPM_SODI_CG_CHECK_1_MASK (0x10006000+0x680) */
#define INFRA2SPM_SODI_CG_CHECK_1_MASK_LSB  (1U << 0)       /* 32b */
/* INFRA2SPM_SODI_CG_CHECK_2_MASK (0x10006000+0x684) */
#define INFRA2SPM_SODI_CG_CHECK_2_MASK_LSB  (1U << 0)       /* 32b */
/* INFRA2SPM_SODI_CG_CHECK_3_MASK (0x10006000+0x688) */
#define INFRA2SPM_SODI_CG_CHECK_3_MASK_LSB  (1U << 0)       /* 32b */
/* INFRA2SPM_SODI_CG_CHECK_4_MASK (0x10006000+0x68C) */
#define INFRA2SPM_SODI_CG_CHECK_4_MASK_LSB  (1U << 0)       /* 32b */
/* INFRA2SPM_SODI3_CG_CHECK_0_MASK (0x10006000+0x690) */
#define INFRA2SPM_SODI3_CG_CHECK_0_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_SODI3_CG_CHECK_1_MASK (0x10006000+0x694) */
#define INFRA2SPM_SODI3_CG_CHECK_1_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_SODI3_CG_CHECK_2_MASK (0x10006000+0x698) */
#define INFRA2SPM_SODI3_CG_CHECK_2_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_SODI3_CG_CHECK_3_MASK (0x10006000+0x69C) */
#define INFRA2SPM_SODI3_CG_CHECK_3_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_SODI3_CG_CHECK_4_MASK (0x10006000+0x6A0) */
#define INFRA2SPM_SODI3_CG_CHECK_4_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_MCDSR_CG_CHECK_0_MASK (0x10006000+0x6A4) */
#define INFRA2SPM_MCDSR_CG_CHECK_0_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_MCDSR_CG_CHECK_1_MASK (0x10006000+0x6A8) */
#define INFRA2SPM_MCDSR_CG_CHECK_1_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_MCDSR_CG_CHECK_2_MASK (0x10006000+0x6AC) */
#define INFRA2SPM_MCDSR_CG_CHECK_2_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_MCDSR_CG_CHECK_3_MASK (0x10006000+0x6B0) */
#define INFRA2SPM_MCDSR_CG_CHECK_3_MASK_LSB (1U << 0)       /* 32b */
/* INFRA2SPM_MCDSR_CG_CHECK_4_MASK (0x10006000+0x6B4) */
#define INFRA2SPM_MCDSR_CG_CHECK_4_MASK_LSB (1U << 0)       /* 32b */
/* OTHER2SPM_CG_CHECK_MASK (0x10006000+0x6B8) */
#define APMIXEDSYS2SPM_SODI3_CG_CHECK_MASK_LSB (1U << 0)       /* 5b */
#define AUDIO2SPM_MCDSR_CG_CHECK_MASK_LSB   (1U << 5)       /* 1b */
#define USB2SPM_MCDSR_CG_CHECK_MASK_LSB     (1U << 6)       /* 1b */
#define AUDIO2SPM_SODI_CG_CHECK_MASK_LSB    (1U << 7)       /* 1b */
#define USB2SPM_SODI_CG_CHECK_MASK_LSB      (1U << 8)       /* 1b */
#define AUDIO2SPM_SODI3_CG_CHECK_MASK_LSB   (1U << 9)       /* 1b */
#define USB2SPM_SODI3_CG_CHECK_MASK_LSB     (1U << 10)      /* 1b */
#define AUDIO2SPM_DEEPIDLE_CG_CHECK_MASK_LSB (1U << 11)      /* 1b */
#define USB2SPM_DEEPIDLE_CG_CHECK_MASK_LSB  (1U << 12)      /* 1b */
/* SPM_TIMER_0 (0x10006000+0x6BC) */
#define SPM_TIMER_VAL_0_LSB                 (1U << 0)       /* 14b */
#define SPM_TIMER_0_LSB                     (1U << 14)      /* 14b */
#define SPM_TIMER_EN_0_LSB                  (1U << 28)      /* 1b */
#define SPM_TIMER_CLR_0_LSB                 (1U << 29)      /* 1b */
#define SPM_TIMEOUT_WAKEUP_EN_0_LSB         (1U << 30)      /* 1b */
#define SPM_TIMEOUT_0_LSB                   (1U << 31)      /* 1b */
/* SPM_TIMER_1 (0x10006000+0x6C0) */
#define SPM_TIMER_VAL_1_LSB                 (1U << 0)       /* 14b */
#define SPM_TIMER_1_LSB                     (1U << 14)      /* 14b */
#define SPM_TIMER_EN_1_LSB                  (1U << 28)      /* 1b */
#define SPM_TIMER_CLR_1_LSB                 (1U << 29)      /* 1b */
#define SPM_TIMEOUT_WAKEUP_EN_1_LSB         (1U << 30)      /* 1b */
#define SPM_TIMEOUT_1_LSB                   (1U << 31)      /* 1b */
/* SPM_TIMER_2 (0x10006000+0x6C4) */
#define SPM_TIMER_VAL_2_LSB                 (1U << 0)       /* 14b */
#define SPM_TIMER_2_LSB                     (1U << 14)      /* 14b */
#define SPM_TIMER_EN_2_LSB                  (1U << 28)      /* 1b */
#define SPM_TIMER_CLR_2_LSB                 (1U << 29)      /* 1b */
#define SPM_TIMEOUT_WAKEUP_EN_2_LSB         (1U << 30)      /* 1b */
#define SPM_TIMEOUT_2_LSB                   (1U << 31)      /* 1b */
/* VS1_PSR_MASK_B (0x10006000+0x6C8) */
#define VS1_OPP0_PSR_MASK_B_LSB             (1U << 0)       /* 8b */
#define VS1_OPP1_PSR_MASK_B_LSB             (1U << 8)       /* 8b */
/* VS2_PSR_MASK_B (0x10006000+0x6CC) */
#define VS2_OPP0_PSR_MASK_B_LSB             (1U << 0)       /* 8b */
#define VS2_OPP1_PSR_MASK_B_LSB             (1U << 8)       /* 8b */
#define VS2_OPP2_PSR_MASK_B_LSB             (1U << 16)      /* 8b */
/* UFS_PSRI_SW (0x10006000+0x6D0) */
#define UFS_PSRI_SW_LSB                     (1U << 0)       /* 1b */
/* UFS_PSRI_SW_SET (0x10006000+0x6D4) */
#define UFS_PSRI_SW_SET_LSB                 (1U << 0)       /* 1b */
/* UFS_PSRI_SW_CLR (0x10006000+0x6D8) */
#define UFS_PSRI_SW_CLR_LSB                 (1U << 0)       /* 1b */
/* SPM_DVFS_CON (0x10006000+0x700) */
#define SPM_DVFS_CON_LSB                    (1U << 0)       /* 32b */
/* SPM_DVFS_CON_STA (0x10006000+0x704) */
#define SPM_DVFS_CON_STA_LSB                (1U << 0)       /* 32b */
/* SPM_DVFS_LEVEL (0x10006000+0x708) */
#define SPM_DVFS_LEVEL_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_STA (0x10006000+0x70C) */
#define TARGET_DVFS_LEVEL_LSB               (1U << 0)       /* 32b */
/* SPM_DVFS_CMD0 (0x10006000+0x710) */
#define SPM_DVFS_CMD0_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD1 (0x10006000+0x714) */
#define SPM_DVFS_CMD1_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD2 (0x10006000+0x718) */
#define SPM_DVFS_CMD2_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD3 (0x10006000+0x71C) */
#define SPM_DVFS_CMD3_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD4 (0x10006000+0x720) */
#define SPM_DVFS_CMD4_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD5 (0x10006000+0x724) */
#define SPM_DVFS_CMD5_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD6 (0x10006000+0x728) */
#define SPM_DVFS_CMD6_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD7 (0x10006000+0x72C) */
#define SPM_DVFS_CMD7_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD8 (0x10006000+0x730) */
#define SPM_DVFS_CMD8_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD9 (0x10006000+0x734) */
#define SPM_DVFS_CMD9_LSB                   (1U << 0)       /* 32b */
/* SPM_DVFS_CMD10 (0x10006000+0x738) */
#define SPM_DVFS_CMD10_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD11 (0x10006000+0x73C) */
#define SPM_DVFS_CMD11_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD12 (0x10006000+0x740) */
#define SPM_DVFS_CMD12_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD13 (0x10006000+0x744) */
#define SPM_DVFS_CMD13_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD14 (0x10006000+0x748) */
#define SPM_DVFS_CMD14_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD15 (0x10006000+0x74C) */
#define SPM_DVFS_CMD15_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD16 (0x10006000+0x750) */
#define SPM_DVFS_CMD16_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD17 (0x10006000+0x754) */
#define SPM_DVFS_CMD17_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD18 (0x10006000+0x758) */
#define SPM_DVFS_CMD18_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD19 (0x10006000+0x75C) */
#define SPM_DVFS_CMD19_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD20 (0x10006000+0x760) */
#define SPM_DVFS_CMD20_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD21 (0x10006000+0x764) */
#define SPM_DVFS_CMD21_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD22 (0x10006000+0x768) */
#define SPM_DVFS_CMD22_LSB                  (1U << 0)       /* 32b */
/* SPM_DVFS_CMD23 (0x10006000+0x76C) */
#define SPM_DVFS_CMD23_LSB                  (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT00 (0x10006000+0x770) */
#define SPM_VCORE_DVFS_SHORTCUT00_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT01 (0x10006000+0x774) */
#define SPM_VCORE_DVFS_SHORTCUT01_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT02 (0x10006000+0x778) */
#define SPM_VCORE_DVFS_SHORTCUT02_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT03 (0x10006000+0x77C) */
#define SPM_VCORE_DVFS_SHORTCUT03_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT04 (0x10006000+0x780) */
#define SPM_VCORE_DVFS_SHORTCUT04_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT05 (0x10006000+0x784) */
#define SPM_VCORE_DVFS_SHORTCUT05_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT06 (0x10006000+0x788) */
#define SPM_VCORE_DVFS_SHORTCUT06_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT07 (0x10006000+0x78C) */
#define SPM_VCORE_DVFS_SHORTCUT07_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT08 (0x10006000+0x790) */
#define SPM_VCORE_DVFS_SHORTCUT08_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT09 (0x10006000+0x794) */
#define SPM_VCORE_DVFS_SHORTCUT09_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT10 (0x10006000+0x798) */
#define SPM_VCORE_DVFS_SHORTCUT10_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT11 (0x10006000+0x79C) */
#define SPM_VCORE_DVFS_SHORTCUT11_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT12 (0x10006000+0x7A0) */
#define SPM_VCORE_DVFS_SHORTCUT12_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT13 (0x10006000+0x7A4) */
#define SPM_VCORE_DVFS_SHORTCUT13_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT14 (0x10006000+0x7A8) */
#define SPM_VCORE_DVFS_SHORTCUT14_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT15 (0x10006000+0x7AC) */
#define SPM_VCORE_DVFS_SHORTCUT15_LSB       (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_STA0 (0x10006000+0x7B0) */
#define SPM_VCORE_DVFS_SHORTCUT_STA0_LSB    (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_STA1 (0x10006000+0x7B4) */
#define SPM_VCORE_DVFS_SHORTCUT_STA1_LSB    (1U << 0)       /* 32b */
/* SPM_VCORE_DVFS_SHORTCUT_IN (0x10006000+0x7B8) */
#define SPM_VCORE_DVFS_SHORTCUT_IN_LSB      (1U << 0)       /* 32b */
/* SPM_DVS_DFS_LEVEL (0x10006000+0x7BC) */
#define SPM_DFS_LEVEL_LSB                   (1U << 0)       /* 16b */
#define SPM_DVS_LEVEL_LSB                   (1U << 16)      /* 16b */
/* PCM_WDT_LATCH_0 (0x10006000+0x800) */
#define PCM_WDT_LATCH_0_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_1 (0x10006000+0x804) */
#define PCM_WDT_LATCH_1_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_2 (0x10006000+0x808) */
#define PCM_WDT_LATCH_2_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_3 (0x10006000+0x80C) */
#define PCM_WDT_LATCH_3_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_4 (0x10006000+0x810) */
#define PCM_WDT_LATCH_4_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_5 (0x10006000+0x814) */
#define PCM_WDT_LATCH_5_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_6 (0x10006000+0x818) */
#define PCM_WDT_LATCH_6_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_7 (0x10006000+0x81C) */
#define PCM_WDT_LATCH_7_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_8 (0x10006000+0x820) */
#define PCM_WDT_LATCH_8_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_9 (0x10006000+0x824) */
#define PCM_WDT_LATCH_9_LSB                 (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_10 (0x10006000+0x828) */
#define PCM_WDT_LATCH_10_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_11 (0x10006000+0x82C) */
#define PCM_WDT_LATCH_11_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_12 (0x10006000+0x830) */
#define PCM_WDT_LATCH_12_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_13 (0x10006000+0x834) */
#define PCM_WDT_LATCH_13_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_14 (0x10006000+0x838) */
#define PCM_WDT_LATCH_14_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_15 (0x10006000+0x83C) */
#define PCM_WDT_LATCH_15_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_16 (0x10006000+0x840) */
#define PCM_WDT_LATCH_16_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_17 (0x10006000+0x844) */
#define PCM_WDT_LATCH_17_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_18 (0x10006000+0x848) */
#define PCM_WDT_LATCH_18_LSB                (1U << 0)       /* 32b */
/* DVFSRC_IRQ_LATCH_0 (0x10006000+0x84C) */
#define DVFSRC_IRQ_LATCH_0_LSB              (1U << 0)       /* 32b */
/* DVFSRC_IRQ_LATCH_1 (0x10006000+0x850) */
#define DVFSRC_IRQ_LATCH_1_LSB              (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_0 (0x10006000+0x854) */
#define DRAMC_GATING_ERR_LATCH_CH0_0_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_1 (0x10006000+0x858) */
#define DRAMC_GATING_ERR_LATCH_CH0_1_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_2 (0x10006000+0x85C) */
#define DRAMC_GATING_ERR_LATCH_CH0_2_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_3 (0x10006000+0x860) */
#define DRAMC_GATING_ERR_LATCH_CH0_3_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_4 (0x10006000+0x864) */
#define DRAMC_GATING_ERR_LATCH_CH0_4_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_5 (0x10006000+0x868) */
#define DRAMC_GATING_ERR_LATCH_CH0_5_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_6 (0x10006000+0x86C) */
#define DRAMC_GATING_ERR_LATCH_CH0_6_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_7 (0x10006000+0x870) */
#define DRAMC_GATING_ERR_LATCH_CH0_7_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH0_8 (0x10006000+0x874) */
#define DRAMC_GATING_ERR_LATCH_CH0_8_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_0 (0x10006000+0x878) */
#define DRAMC_GATING_ERR_LATCH_CH1_0_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_1 (0x10006000+0x87C) */
#define DRAMC_GATING_ERR_LATCH_CH1_1_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_2 (0x10006000+0x880) */
#define DRAMC_GATING_ERR_LATCH_CH1_2_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_3 (0x10006000+0x884) */
#define DRAMC_GATING_ERR_LATCH_CH1_3_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_4 (0x10006000+0x888) */
#define DRAMC_GATING_ERR_LATCH_CH1_4_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_5 (0x10006000+0x88C) */
#define DRAMC_GATING_ERR_LATCH_CH1_5_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_6 (0x10006000+0x890) */
#define DRAMC_GATING_ERR_LATCH_CH1_6_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_7 (0x10006000+0x894) */
#define DRAMC_GATING_ERR_LATCH_CH1_7_LSB    (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_CH1_8 (0x10006000+0x898) */
#define DRAMC_GATING_ERR_LATCH_CH1_8_LSB    (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_SPARE_0 (0x10006000+0x89C) */
#define PCM_WDT_LATCH_SPARE_0_LSB           (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_SPARE_1 (0x10006000+0x8A0) */
#define PCM_WDT_LATCH_SPARE_1_LSB           (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_SPARE_2 (0x10006000+0x8A4) */
#define PCM_WDT_LATCH_SPARE_2_LSB           (1U << 0)       /* 32b */
/* DRAMC_GATING_ERR_LATCH_SPARE_0 (0x10006000+0x8A8) */
#define DRAMC_GATING_ERR_LATCH_SPARE_0_LSB  (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_19 (0x10006000+0x8B0) */
#define PCM_WDT_LATCH_19_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_20 (0x10006000+0x8B4) */
#define PCM_WDT_LATCH_20_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_21 (0x10006000+0x8B8) */
#define PCM_WDT_LATCH_21_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_22 (0x10006000+0x8BC) */
#define PCM_WDT_LATCH_22_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_23 (0x10006000+0x8C0) */
#define PCM_WDT_LATCH_23_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_24 (0x10006000+0x8C4) */
#define PCM_WDT_LATCH_24_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_25 (0x10006000+0x8C8) */
#define PCM_WDT_LATCH_25_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_26 (0x10006000+0x8CC) */
#define PCM_WDT_LATCH_26_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_27 (0x10006000+0x8D0) */
#define PCM_WDT_LATCH_27_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_28 (0x10006000+0x8D4) */
#define PCM_WDT_LATCH_28_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_29 (0x10006000+0x8D8) */
#define PCM_WDT_LATCH_29_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_30 (0x10006000+0x8DC) */
#define PCM_WDT_LATCH_30_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_31 (0x10006000+0x8E0) */
#define PCM_WDT_LATCH_31_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_32 (0x10006000+0x8E4) */
#define PCM_WDT_LATCH_32_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_33 (0x10006000+0x8E8) */
#define PCM_WDT_LATCH_33_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_34 (0x10006000+0x8EC) */
#define PCM_WDT_LATCH_34_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_35 (0x10006000+0x8F0) */
#define PCM_WDT_LATCH_35_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_36 (0x10006000+0x8F4) */
#define PCM_WDT_LATCH_36_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_37 (0x10006000+0x8F8) */
#define PCM_WDT_LATCH_37_LSB                (1U << 0)       /* 32b */
/* PCM_WDT_LATCH_38 (0x10006000+0x8FC) */
#define PCM_WDT_LATCH_38_LSB                (1U << 0)       /* 32b */
/* SPM_ACK_CHK_CON_0 (0x10006000+0x900) */
#define SPM_ACK_CHK_SW_EN_0_LSB             (1U << 0)       /* 1b */
#define SPM_ACK_CHK_CLR_ALL_0_LSB           (1U << 1)       /* 1b */
#define SPM_ACK_CHK_CLR_TIMER_0_LSB         (1U << 2)       /* 1b */
#define SPM_ACK_CHK_CLR_IRQ_0_LSB           (1U << 3)       /* 1b */
#define SPM_ACK_CHK_STA_EN_0_LSB            (1U << 4)       /* 1b */
#define SPM_ACK_CHK_WAKEUP_EN_0_LSB         (1U << 5)       /* 1b */
#define SPM_ACK_CHK_WDT_EN_0_LSB            (1U << 6)       /* 1b */
#define SPM_ACK_CHK_LOCK_PC_TRACE_EN_0_LSB  (1U << 7)       /* 1b */
#define SPM_ACK_CHK_HW_EN_0_LSB             (1U << 8)       /* 1b */
#define SPM_ACK_CHK_HW_MODE_0_LSB           (1U << 9)       /* 3b */
#define SPM_ACK_CHK_FAIL_0_LSB              (1U << 15)      /* 1b */
/* SPM_ACK_CHK_PC_0 (0x10006000+0x904) */
#define SPM_ACK_CHK_HW_TRIG_PC_VAL_0_LSB    (1U << 0)       /* 16b */
#define SPM_ACK_CHK_HW_TARG_PC_VAL_0_LSB    (1U << 16)      /* 16b */
/* SPM_ACK_CHK_SEL_0 (0x10006000+0x908) */
#define SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_0_LSB (1U << 0)       /* 5b */
#define SPM_ACK_CHK_HW_TRIG_GROUP_SEL_0_LSB (1U << 5)       /* 3b */
#define SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_0_LSB (1U << 16)      /* 5b */
#define SPM_ACK_CHK_HW_TARG_GROUP_SEL_0_LSB (1U << 21)      /* 3b */
/* SPM_ACK_CHK_TIMER_0 (0x10006000+0x90C) */
#define SPM_ACK_CHK_TIMER_VAL_0_LSB         (1U << 0)       /* 16b */
#define SPM_ACK_CHK_TIMER_0_LSB             (1U << 16)      /* 16b */
/* SPM_ACK_CHK_STA_0 (0x10006000+0x910) */
#define SPM_ACK_CHK_STA_0_LSB               (1U << 0)       /* 32b */
/* SPM_ACK_CHK_SWINT_0 (0x10006000+0x914) */
#define SPM_ACK_CHK_SWINT_EN_0_LSB          (1U << 0)       /* 32b */
/* SPM_ACK_CHK_CON_1 (0x10006000+0x918) */
#define SPM_ACK_CHK_SW_EN_1_LSB             (1U << 0)       /* 1b */
#define SPM_ACK_CHK_CLR_ALL_1_LSB           (1U << 1)       /* 1b */
#define SPM_ACK_CHK_CLR_TIMER_1_LSB         (1U << 2)       /* 1b */
#define SPM_ACK_CHK_CLR_IRQ_1_LSB           (1U << 3)       /* 1b */
#define SPM_ACK_CHK_STA_EN_1_LSB            (1U << 4)       /* 1b */
#define SPM_ACK_CHK_WAKEUP_EN_1_LSB         (1U << 5)       /* 1b */
#define SPM_ACK_CHK_WDT_EN_1_LSB            (1U << 6)       /* 1b */
#define SPM_ACK_CHK_LOCK_PC_TRACE_EN_1_LSB  (1U << 7)       /* 1b */
#define SPM_ACK_CHK_HW_EN_1_LSB             (1U << 8)       /* 1b */
#define SPM_ACK_CHK_HW_MODE_1_LSB           (1U << 9)       /* 3b */
#define SPM_ACK_CHK_FAIL_1_LSB              (1U << 15)      /* 1b */
/* SPM_ACK_CHK_PC_1 (0x10006000+0x91C) */
#define SPM_ACK_CHK_HW_TRIG_PC_VAL_1_LSB    (1U << 0)       /* 16b */
#define SPM_ACK_CHK_HW_TARG_PC_VAL_1_LSB    (1U << 16)      /* 16b */
/* SPM_ACK_CHK_SEL_1 (0x10006000+0x920) */
#define SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_1_LSB (1U << 0)       /* 5b */
#define SPM_ACK_CHK_HW_TRIG_GROUP_SEL_1_LSB (1U << 5)       /* 3b */
#define SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_1_LSB (1U << 16)      /* 5b */
#define SPM_ACK_CHK_HW_TARG_GROUP_SEL_1_LSB (1U << 21)      /* 3b */
/* SPM_ACK_CHK_TIMER_1 (0x10006000+0x924) */
#define SPM_ACK_CHK_TIMER_VAL_1_LSB         (1U << 0)       /* 16b */
#define SPM_ACK_CHK_TIMER_1_LSB             (1U << 16)      /* 16b */
/* SPM_ACK_CHK_STA_1 (0x10006000+0x928) */
#define SPM_ACK_CHK_STA_1_LSB               (1U << 0)       /* 32b */
/* SPM_ACK_CHK_SWINT_1 (0x10006000+0x92C) */
#define SPM_ACK_CHK_SWINT_EN_1_LSB          (1U << 0)       /* 32b */
/* SPM_ACK_CHK_CON_2 (0x10006000+0x930) */
#define SPM_ACK_CHK_SW_EN_2_LSB             (1U << 0)       /* 1b */
#define SPM_ACK_CHK_CLR_ALL_2_LSB           (1U << 1)       /* 1b */
#define SPM_ACK_CHK_CLR_TIMER_2_LSB         (1U << 2)       /* 1b */
#define SPM_ACK_CHK_CLR_IRQ_2_LSB           (1U << 3)       /* 1b */
#define SPM_ACK_CHK_STA_EN_2_LSB            (1U << 4)       /* 1b */
#define SPM_ACK_CHK_WAKEUP_EN_2_LSB         (1U << 5)       /* 1b */
#define SPM_ACK_CHK_WDT_EN_2_LSB            (1U << 6)       /* 1b */
#define SPM_ACK_CHK_LOCK_PC_TRACE_EN_2_LSB  (1U << 7)       /* 1b */
#define SPM_ACK_CHK_HW_EN_2_LSB             (1U << 8)       /* 1b */
#define SPM_ACK_CHK_HW_MODE_2_LSB           (1U << 9)       /* 3b */
#define SPM_ACK_CHK_FAIL_2_LSB              (1U << 15)      /* 1b */
/* SPM_ACK_CHK_PC_2 (0x10006000+0x934) */
#define SPM_ACK_CHK_HW_TRIG_PC_VAL_2_LSB    (1U << 0)       /* 16b */
#define SPM_ACK_CHK_HW_TARG_PC_VAL_2_LSB    (1U << 16)      /* 16b */
/* SPM_ACK_CHK_SEL_2 (0x10006000+0x938) */
#define SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_2_LSB (1U << 0)       /* 5b */
#define SPM_ACK_CHK_HW_TRIG_GROUP_SEL_2_LSB (1U << 5)       /* 3b */
#define SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_2_LSB (1U << 16)      /* 5b */
#define SPM_ACK_CHK_HW_TARG_GROUP_SEL_2_LSB (1U << 21)      /* 3b */
/* SPM_ACK_CHK_TIMER_2 (0x10006000+0x93C) */
#define SPM_ACK_CHK_TIMER_VAL_2_LSB         (1U << 0)       /* 16b */
#define SPM_ACK_CHK_TIMER_2_LSB             (1U << 16)      /* 16b */
/* SPM_ACK_CHK_STA_2 (0x10006000+0x940) */
#define SPM_ACK_CHK_STA_2_LSB               (1U << 0)       /* 32b */
/* SPM_ACK_CHK_SWINT_2 (0x10006000+0x944) */
#define SPM_ACK_CHK_SWINT_EN_2_LSB          (1U << 0)       /* 32b */
/* SPM_ACK_CHK_CON_3 (0x10006000+0x948) */
#define SPM_ACK_CHK_SW_EN_3_LSB             (1U << 0)       /* 1b */
#define SPM_ACK_CHK_CLR_ALL_3_LSB           (1U << 1)       /* 1b */
#define SPM_ACK_CHK_CLR_TIMER_3_LSB         (1U << 2)       /* 1b */
#define SPM_ACK_CHK_CLR_IRQ_3_LSB           (1U << 3)       /* 1b */
#define SPM_ACK_CHK_STA_EN_3_LSB            (1U << 4)       /* 1b */
#define SPM_ACK_CHK_WAKEUP_EN_3_LSB         (1U << 5)       /* 1b */
#define SPM_ACK_CHK_WDT_EN_3_LSB            (1U << 6)       /* 1b */
#define SPM_ACK_CHK_LOCK_PC_TRACE_EN_3_LSB  (1U << 7)       /* 1b */
#define SPM_ACK_CHK_HW_EN_3_LSB             (1U << 8)       /* 1b */
#define SPM_ACK_CHK_HW_MODE_3_LSB           (1U << 9)       /* 3b */
#define SPM_ACK_CHK_FAIL_3_LSB              (1U << 15)      /* 1b */
/* SPM_ACK_CHK_PC_3 (0x10006000+0x94C) */
#define SPM_ACK_CHK_HW_TRIG_PC_VAL_3_LSB    (1U << 0)       /* 16b */
#define SPM_ACK_CHK_HW_TARG_PC_VAL_3_LSB    (1U << 16)      /* 16b */
/* SPM_ACK_CHK_SEL_3 (0x10006000+0x950) */
#define SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_3_LSB (1U << 0)       /* 5b */
#define SPM_ACK_CHK_HW_TRIG_GROUP_SEL_3_LSB (1U << 5)       /* 3b */
#define SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_3_LSB (1U << 16)      /* 5b */
#define SPM_ACK_CHK_HW_TARG_GROUP_SEL_3_LSB (1U << 21)      /* 3b */
/* SPM_ACK_CHK_TIMER_3 (0x10006000+0x954) */
#define SPM_ACK_CHK_TIMER_VAL_3_LSB         (1U << 0)       /* 16b */
#define SPM_ACK_CHK_TIMER_3_LSB             (1U << 16)      /* 16b */
/* SPM_ACK_CHK_STA_3 (0x10006000+0x958) */
#define SPM_ACK_CHK_STA_3_LSB               (1U << 0)       /* 32b */
/* SPM_ACK_CHK_SWINT_3 (0x10006000+0x95C) */
#define SPM_ACK_CHK_SWINT_EN_3_LSB          (1U << 0)       /* 32b */
/* SPM_ACK_CHK_CON_4 (0x10006000+0x960) */
#define SPM_ACK_CHK_SW_EN_4_LSB             (1U << 0)       /* 1b */
#define SPM_ACK_CHK_CLR_ALL_4_LSB           (1U << 1)       /* 1b */
#define SPM_ACK_CHK_CLR_TIMER_4_LSB         (1U << 2)       /* 1b */
#define SPM_ACK_CHK_CLR_IRQ_4_LSB           (1U << 3)       /* 1b */
#define SPM_ACK_CHK_STA_EN_4_LSB            (1U << 4)       /* 1b */
#define SPM_ACK_CHK_WAKEUP_EN_4_LSB         (1U << 5)       /* 1b */
#define SPM_ACK_CHK_WDT_EN_4_LSB            (1U << 6)       /* 1b */
#define SPM_ACK_CHK_LOCK_PC_TRACE_EN_4_LSB  (1U << 7)       /* 1b */
#define SPM_ACK_CHK_HW_EN_4_LSB             (1U << 8)       /* 1b */
#define SPM_ACK_CHK_HW_MODE_4_LSB           (1U << 9)       /* 3b */
#define SPM_ACK_CHK_FAIL_4_LSB              (1U << 15)      /* 1b */
/* SPM_ACK_CHK_PC_4 (0x10006000+0x964) */
#define SPM_ACK_CHK_HW_TRIG_PC_VAL_4_LSB    (1U << 0)       /* 16b */
#define SPM_ACK_CHK_HW_TARG_PC_VAL_4_LSB    (1U << 16)      /* 16b */
/* SPM_ACK_CHK_SEL_4 (0x10006000+0x968) */
#define SPM_ACK_CHK_HW_TRIG_SIGNAL_SEL_4_LSB (1U << 0)       /* 5b */
#define SPM_ACK_CHK_HW_TRIG_GROUP_SEL_4_LSB (1U << 5)       /* 3b */
#define SPM_ACK_CHK_HW_TARG_SIGNAL_SEL_4_LSB (1U << 16)      /* 5b */
#define SPM_ACK_CHK_HW_TARG_GROUP_SEL_4_LSB (1U << 21)      /* 3b */
/* SPM_ACK_CHK_TIMER_4 (0x10006000+0x96C) */
#define SPM_ACK_CHK_TIMER_VAL_4_LSB         (1U << 0)       /* 16b */
#define SPM_ACK_CHK_TIMER_4_LSB             (1U << 16)      /* 16b */
/* SPM_ACK_CHK_STA_4 (0x10006000+0x970) */
#define SPM_ACK_CHK_STA_4_LSB               (1U << 0)       /* 32b */
/* SPM_ACK_CHK_SWINT_4 (0x10006000+0x974) */
#define SPM_ACK_CHK_SWINT_EN_4_LSB          (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA0 (0x10006000+0xE00) */
#define SPM_PMSR_CURR_STA0_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA1 (0x10006000+0xE04) */
#define SPM_PMSR_CURR_STA1_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA2 (0x10006000+0xE08) */
#define SPM_PMSR_CURR_STA2_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA3 (0x10006000+0xE0C) */
#define SPM_PMSR_CURR_STA3_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA4 (0x10006000+0xE10) */
#define SPM_PMSR_CURR_STA4_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA5 (0x10006000+0xE14) */
#define SPM_PMSR_CURR_STA5_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA6 (0x10006000+0xE18) */
#define SPM_PMSR_CURR_STA6_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA7 (0x10006000+0xE1C) */
#define SPM_PMSR_CURR_STA7_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA8 (0x10006000+0xE20) */
#define SPM_PMSR_CURR_STA8_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA9 (0x10006000+0xE24) */
#define SPM_PMSR_CURR_STA9_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA10 (0x10006000+0xE28) */
#define SPM_PMSR_CURR_STA10_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA11 (0x10006000+0xE2C) */
#define SPM_PMSR_CURR_STA11_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA12 (0x10006000+0xE30) */
#define SPM_PMSR_CURR_STA12_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA13 (0x10006000+0xE34) */
#define SPM_PMSR_CURR_STA13_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA14 (0x10006000+0xE38) */
#define SPM_PMSR_CURR_STA14_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA15 (0x10006000+0xE3C) */
#define SPM_PMSR_CURR_STA15_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA16 (0x10006000+0xE40) */
#define SPM_PMSR_CURR_STA16_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA17 (0x10006000+0xE44) */
#define SPM_PMSR_CURR_STA17_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA18 (0x10006000+0xE48) */
#define SPM_PMSR_CURR_STA18_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA19 (0x10006000+0xE4C) */
#define SPM_PMSR_CURR_STA19_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA20 (0x10006000+0xE50) */
#define SPM_PMSR_CURR_STA20_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA21 (0x10006000+0xE54) */
#define SPM_PMSR_CURR_STA21_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA22 (0x10006000+0xE58) */
#define SPM_PMSR_CURR_STA22_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA23 (0x10006000+0xE5C) */
#define SPM_PMSR_CURR_STA23_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA24 (0x10006000+0xE60) */
#define SPM_PMSR_CURR_STA24_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA25 (0x10006000+0xE64) */
#define SPM_PMSR_CURR_STA25_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA26 (0x10006000+0xE68) */
#define SPM_PMSR_CURR_STA26_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA27 (0x10006000+0xE6C) */
#define SPM_PMSR_CURR_STA27_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA28 (0x10006000+0xE70) */
#define SPM_PMSR_CURR_STA28_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA29 (0x10006000+0xE74) */
#define SPM_PMSR_CURR_STA29_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA30 (0x10006000+0xE78) */
#define SPM_PMSR_CURR_STA30_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA31 (0x10006000+0xE7C) */
#define SPM_PMSR_CURR_STA31_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA32 (0x10006000+0xE80) */
#define SPM_PMSR_CURR_STA32_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA33 (0x10006000+0xE84) */
#define SPM_PMSR_CURR_STA33_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA34 (0x10006000+0xE88) */
#define SPM_PMSR_CURR_STA34_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA35 (0x10006000+0xE8C) */
#define SPM_PMSR_CURR_STA35_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA36 (0x10006000+0xE90) */
#define SPM_PMSR_CURR_STA36_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA37 (0x10006000+0xE94) */
#define SPM_PMSR_CURR_STA37_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA38 (0x10006000+0xE98) */
#define SPM_PMSR_CURR_STA38_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA39 (0x10006000+0xE9C) */
#define SPM_PMSR_CURR_STA39_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA40 (0x10006000+0xEA0) */
#define SPM_PMSR_CURR_STA40_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA41 (0x10006000+0xEA4) */
#define SPM_PMSR_CURR_STA41_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA42 (0x10006000+0xEA8) */
#define SPM_PMSR_CURR_STA42_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA43 (0x10006000+0xEAC) */
#define SPM_PMSR_CURR_STA43_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA44 (0x10006000+0xEB0) */
#define SPM_PMSR_CURR_STA44_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA45 (0x10006000+0xEB4) */
#define SPM_PMSR_CURR_STA45_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA46 (0x10006000+0xEB8) */
#define SPM_PMSR_CURR_STA46_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_CURR_STA47 (0x10006000+0xEBC) */
#define SPM_PMSR_CURR_STA47_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_TIEMR_STA0 (0x10006000+0xEC0) */
#define PMSR_TIMER_SET0_LSB                 (1U << 0)       /* 32b */
/* SPM_PMSR_TIEMR_STA1 (0x10006000+0xEC4) */
#define PMSR_TIMER_SET1_LSB                 (1U << 0)       /* 32b */
/* SPM_PMSR_TIEMR_STA2 (0x10006000+0xEC8) */
#define PMSR_TIMER_SET2_LSB                 (1U << 0)       /* 32b */
/* SPM_PMSR_GENERAL_CON0 (0x10006000+0xECC) */
#define PMSR_ENABLE_SET0_LSB                (1U << 0)       /* 1b */
#define PMSR_ENABLE_SET1_LSB                (1U << 1)       /* 1b */
#define PMSR_ENABLE_SET2_LSB                (1U << 2)       /* 1b */
#define PMSR_IRQ_CLR_SET0_LSB               (1U << 3)       /* 1b */
#define PMSR_IRQ_CLR_SET1_LSB               (1U << 4)       /* 1b */
#define PMSR_IRQ_CLR_SET2_LSB               (1U << 5)       /* 1b */
#define PMSR_SPEED_MODE_EN_SET0_LSB         (1U << 6)       /* 1b */
#define PMSR_SPEED_MODE_EN_SET1_LSB         (1U << 7)       /* 1b */
#define PMSR_SPEED_MODE_EN_SET2_LSB         (1U << 8)       /* 1b */
#define PMSR_EVENT_CLR_SET0_LSB             (1U << 9)       /* 1b */
#define PMSR_EVENT_CLR_SET1_LSB             (1U << 10)      /* 1b */
#define PMSR_EVENT_CLR_SET2_LSB             (1U << 11)      /* 1b */
#define REG_PMSR_IRQ_MASK_SET0_LSB          (1U << 12)      /* 1b */
#define REG_PMSR_IRQ_MASK_SET1_LSB          (1U << 13)      /* 1b */
#define REG_PMSR_IRQ_MASK_SET2_LSB          (1U << 14)      /* 1b */
#define REG_PMSR_IRQ_WAKEUP_EVENT_MASK_SET0_LSB (1U << 15)      /* 1b */
#define REG_PMSR_IRQ_WAKEUP_EVENT_MASK_SET1_LSB (1U << 16)      /* 1b */
#define REG_PMSR_IRQ_WAKEUP_EVENT_MASK_SET2_LSB (1U << 17)      /* 1b */
#define PMSR_GEN_SW_RST_EN_LSB              (1U << 18)      /* 1b */
#define PMSR_MODULE_ENABLE_LSB              (1U << 19)      /* 1b */
#define PMSR_MODE_LSB                       (1U << 20)      /* 2b */
#define SPM_PMSR_GENERAL_CON0_PMSR_IRQ_B_SET0_LSB (1U << 29)      /* 1b */
#define SPM_PMSR_GENERAL_CON0_PMSR_IRQ_B_SET1_LSB (1U << 30)      /* 1b */
#define SPM_PMSR_GENERAL_CON0_PMSR_IRQ_B_SET2_LSB (1U << 31)      /* 1b */
/* SPM_PMSR_GENERAL_CON1 (0x10006000+0xED0) */
#define PMSR_COUNTER_THRES_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_GENERAL_CON2 (0x10006000+0xED4) */
#define PMSR_DEBUG_IN_0_MASK_B_LSB          (1U << 0)       /* 32b */
/* SPM_PMSR_GENERAL_CON3 (0x10006000+0xED8) */
#define PMSR_DEBUG_IN_1_MASK_B_LSB          (1U << 0)       /* 32b */
/* SPM_PMSR_GENERAL_CON4 (0x10006000+0xEDC) */
#define PMSR_DEBUG_IN_2_MASK_B_LSB          (1U << 0)       /* 32b */
/* SPM_PMSR_GENERAL_CON5 (0x10006000+0xEE0) */
#define PMSR_DEBUG_IN_3_MASK_B_LSB          (1U << 0)       /* 32b */
/* SPM_PMSR_GENERAL_CON6 (0x10006000+0xEE4) */
#define REG_PMSR_STATE_SIG_SEL_0_LSB        (1U << 0)       /* 2b */
#define REG_PMSR_STATE_SIG_SEL_1_LSB        (1U << 2)       /* 2b */
#define REG_PMSR_STATE_SIG_SEL_2_LSB        (1U << 4)       /* 2b */
#define REG_PMSR_STATE_SIG_SEL_3_LSB        (1U << 6)       /* 2b */
#define REG_PMSR_STATE_SIG_SEL_4_LSB        (1U << 8)       /* 2b */
#define REG_PMSR_STATE_SIG_SEL_5_LSB        (1U << 10)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_6_LSB        (1U << 12)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_7_LSB        (1U << 14)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_8_LSB        (1U << 16)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_9_LSB        (1U << 18)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_10_LSB       (1U << 20)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_11_LSB       (1U << 22)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_12_LSB       (1U << 24)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_13_LSB       (1U << 26)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_14_LSB       (1U << 28)      /* 2b */
#define REG_PMSR_STATE_SIG_SEL_15_LSB       (1U << 30)      /* 2b */
/* SPM_PMSR_SW_RESET (0x10006000+0xEE8) */
#define PMSR_SW_RST_EN_SET0_LSB             (1U << 0)       /* 1b */
#define PMSR_SW_RST_EN_SET1_LSB             (1U << 1)       /* 1b */
#define PMSR_SW_RST_EN_SET2_LSB             (1U << 2)       /* 1b */
/* SPM_PMSR_LAST_STA0 (0x10006000+0xF00) */
#define SPM_PMSR_LAST_STA0_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA1 (0x10006000+0xF04) */
#define SPM_PMSR_LAST_STA1_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA2 (0x10006000+0xF08) */
#define SPM_PMSR_LAST_STA2_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA3 (0x10006000+0xF0C) */
#define SPM_PMSR_LAST_STA3_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA4 (0x10006000+0xF10) */
#define SPM_PMSR_LAST_STA4_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA5 (0x10006000+0xF14) */
#define SPM_PMSR_LAST_STA5_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA6 (0x10006000+0xF18) */
#define SPM_PMSR_LAST_STA6_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA7 (0x10006000+0xF1C) */
#define SPM_PMSR_LAST_STA7_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA8 (0x10006000+0xF20) */
#define SPM_PMSR_LAST_STA8_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA9 (0x10006000+0xF24) */
#define SPM_PMSR_LAST_STA9_LSB              (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA10 (0x10006000+0xF28) */
#define SPM_PMSR_LAST_STA10_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA11 (0x10006000+0xF2C) */
#define SPM_PMSR_LAST_STA11_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA12 (0x10006000+0xF30) */
#define SPM_PMSR_LAST_STA12_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA13 (0x10006000+0xF34) */
#define SPM_PMSR_LAST_STA13_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA14 (0x10006000+0xF38) */
#define SPM_PMSR_LAST_STA14_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA15 (0x10006000+0xF3C) */
#define SPM_PMSR_LAST_STA15_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA16 (0x10006000+0xF40) */
#define SPM_PMSR_LAST_STA16_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA17 (0x10006000+0xF44) */
#define SPM_PMSR_LAST_STA17_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA18 (0x10006000+0xF48) */
#define SPM_PMSR_LAST_STA18_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA19 (0x10006000+0xF4C) */
#define SPM_PMSR_LAST_STA19_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA20 (0x10006000+0xF50) */
#define SPM_PMSR_LAST_STA20_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA21 (0x10006000+0xF54) */
#define SPM_PMSR_LAST_STA21_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA22 (0x10006000+0xF58) */
#define SPM_PMSR_LAST_STA22_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA23 (0x10006000+0xF5C) */
#define SPM_PMSR_LAST_STA23_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA24 (0x10006000+0xF60) */
#define SPM_PMSR_LAST_STA24_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA25 (0x10006000+0xF64) */
#define SPM_PMSR_LAST_STA25_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA26 (0x10006000+0xF68) */
#define SPM_PMSR_LAST_STA26_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA27 (0x10006000+0xF6C) */
#define SPM_PMSR_LAST_STA27_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA28 (0x10006000+0xF70) */
#define SPM_PMSR_LAST_STA28_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA29 (0x10006000+0xF74) */
#define SPM_PMSR_LAST_STA29_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA30 (0x10006000+0xF78) */
#define SPM_PMSR_LAST_STA30_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA31 (0x10006000+0xF7C) */
#define SPM_PMSR_LAST_STA31_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA32 (0x10006000+0xF80) */
#define SPM_PMSR_LAST_STA32_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA33 (0x10006000+0xF84) */
#define SPM_PMSR_LAST_STA33_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA34 (0x10006000+0xF88) */
#define SPM_PMSR_LAST_STA34_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA35 (0x10006000+0xF8C) */
#define SPM_PMSR_LAST_STA35_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA36 (0x10006000+0xF90) */
#define SPM_PMSR_LAST_STA36_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA37 (0x10006000+0xF94) */
#define SPM_PMSR_LAST_STA37_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA38 (0x10006000+0xF98) */
#define SPM_PMSR_LAST_STA38_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA39 (0x10006000+0xF9C) */
#define SPM_PMSR_LAST_STA39_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA40 (0x10006000+0xFA0) */
#define SPM_PMSR_LAST_STA40_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA41 (0x10006000+0xFA4) */
#define SPM_PMSR_LAST_STA41_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA42 (0x10006000+0xFA8) */
#define SPM_PMSR_LAST_STA42_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA43 (0x10006000+0xFAC) */
#define SPM_PMSR_LAST_STA43_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA44 (0x10006000+0xFB0) */
#define SPM_PMSR_LAST_STA44_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA45 (0x10006000+0xFB4) */
#define SPM_PMSR_LAST_STA45_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA46 (0x10006000+0xFB8) */
#define SPM_PMSR_LAST_STA46_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_LAST_STA47 (0x10006000+0xFBC) */
#define SPM_PMSR_LAST_STA47_LSB             (1U << 0)       /* 32b */
/* SPM_PMSR_SEL_CON0 (0x10006000+0xFC0) */
#define REG_PMSR_SIG_SEL_0_LSB              (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_1_LSB              (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_2_LSB              (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_3_LSB              (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_4_LSB              (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_5_LSB              (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON1 (0x10006000+0xFC4) */
#define REG_PMSR_SIG_SEL_6_LSB              (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_7_LSB              (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_8_LSB              (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_9_LSB              (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_10_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_11_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON2 (0x10006000+0xFC8) */
#define REG_PMSR_SIG_SEL_12_LSB             (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_13_LSB             (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_14_LSB             (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_15_LSB             (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_16_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_17_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON3 (0x10006000+0xFCC) */
#define REG_PMSR_SIG_SEL_18_LSB             (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_19_LSB             (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_20_LSB             (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_21_LSB             (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_22_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_23_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON4 (0x10006000+0xFD0) */
#define REG_PMSR_SIG_SEL_24_LSB             (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_25_LSB             (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_26_LSB             (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_27_LSB             (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_28_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_29_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON5 (0x10006000+0xFD4) */
#define REG_PMSR_SIG_SEL_30_LSB             (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_31_LSB             (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_32_LSB             (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_33_LSB             (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_34_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_35_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON6 (0x10006000+0xFD8) */
#define REG_PMSR_SIG_SEL_36_LSB             (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_37_LSB             (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_38_LSB             (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_39_LSB             (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_40_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_41_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_SEL_CON7 (0x10006000+0xFDC) */
#define REG_PMSR_SIG_SEL_42_LSB             (1U << 0)       /* 5b */
#define REG_PMSR_SIG_SEL_43_LSB             (1U << 5)       /* 5b */
#define REG_PMSR_SIG_SEL_44_LSB             (1U << 10)      /* 5b */
#define REG_PMSR_SIG_SEL_45_LSB             (1U << 15)      /* 5b */
#define REG_PMSR_SIG_SEL_46_LSB             (1U << 20)      /* 5b */
#define REG_PMSR_SIG_SEL_47_LSB             (1U << 25)      /* 5b */
/* SPM_PMSR_MON_CON0 (0x10006000+0xFE0) */
#define REG_PMSR_MON_TYPE_0_LSB             (1U << 0)       /* 2b */
#define REG_PMSR_MON_TYPE_1_LSB             (1U << 2)       /* 2b */
#define REG_PMSR_MON_TYPE_2_LSB             (1U << 4)       /* 2b */
#define REG_PMSR_MON_TYPE_3_LSB             (1U << 6)       /* 2b */
#define REG_PMSR_MON_TYPE_4_LSB             (1U << 8)       /* 2b */
#define REG_PMSR_MON_TYPE_5_LSB             (1U << 10)      /* 2b */
#define REG_PMSR_MON_TYPE_6_LSB             (1U << 12)      /* 2b */
#define REG_PMSR_MON_TYPE_7_LSB             (1U << 14)      /* 2b */
#define REG_PMSR_MON_TYPE_8_LSB             (1U << 16)      /* 2b */
#define REG_PMSR_MON_TYPE_9_LSB             (1U << 18)      /* 2b */
#define REG_PMSR_MON_TYPE_10_LSB            (1U << 20)      /* 2b */
#define REG_PMSR_MON_TYPE_11_LSB            (1U << 22)      /* 2b */
#define REG_PMSR_MON_TYPE_12_LSB            (1U << 24)      /* 2b */
#define REG_PMSR_MON_TYPE_13_LSB            (1U << 26)      /* 2b */
#define REG_PMSR_MON_TYPE_14_LSB            (1U << 28)      /* 2b */
#define REG_PMSR_MON_TYPE_15_LSB            (1U << 30)      /* 2b */
/* SPM_PMSR_MON_CON1 (0x10006000+0xFE4) */
#define REG_PMSR_MON_TYPE_16_LSB            (1U << 0)       /* 2b */
#define REG_PMSR_MON_TYPE_17_LSB            (1U << 2)       /* 2b */
#define REG_PMSR_MON_TYPE_18_LSB            (1U << 4)       /* 2b */
#define REG_PMSR_MON_TYPE_19_LSB            (1U << 6)       /* 2b */
#define REG_PMSR_MON_TYPE_20_LSB            (1U << 8)       /* 2b */
#define REG_PMSR_MON_TYPE_21_LSB            (1U << 10)      /* 2b */
#define REG_PMSR_MON_TYPE_22_LSB            (1U << 12)      /* 2b */
#define REG_PMSR_MON_TYPE_23_LSB            (1U << 14)      /* 2b */
#define REG_PMSR_MON_TYPE_24_LSB            (1U << 16)      /* 2b */
#define REG_PMSR_MON_TYPE_25_LSB            (1U << 18)      /* 2b */
#define REG_PMSR_MON_TYPE_26_LSB            (1U << 20)      /* 2b */
#define REG_PMSR_MON_TYPE_27_LSB            (1U << 22)      /* 2b */
#define REG_PMSR_MON_TYPE_28_LSB            (1U << 24)      /* 2b */
#define REG_PMSR_MON_TYPE_29_LSB            (1U << 26)      /* 2b */
#define REG_PMSR_MON_TYPE_30_LSB            (1U << 28)      /* 2b */
#define REG_PMSR_MON_TYPE_31_LSB            (1U << 30)      /* 2b */
/* SPM_PMSR_MON_CON2 (0x10006000+0xFE8) */
#define REG_PMSR_MON_TYPE_32_LSB            (1U << 0)       /* 2b */
#define REG_PMSR_MON_TYPE_33_LSB            (1U << 2)       /* 2b */
#define REG_PMSR_MON_TYPE_34_LSB            (1U << 4)       /* 2b */
#define REG_PMSR_MON_TYPE_35_LSB            (1U << 6)       /* 2b */
#define REG_PMSR_MON_TYPE_36_LSB            (1U << 8)       /* 2b */
#define REG_PMSR_MON_TYPE_37_LSB            (1U << 10)      /* 2b */
#define REG_PMSR_MON_TYPE_38_LSB            (1U << 12)      /* 2b */
#define REG_PMSR_MON_TYPE_39_LSB            (1U << 14)      /* 2b */
#define REG_PMSR_MON_TYPE_40_LSB            (1U << 16)      /* 2b */
#define REG_PMSR_MON_TYPE_41_LSB            (1U << 18)      /* 2b */
#define REG_PMSR_MON_TYPE_42_LSB            (1U << 20)      /* 2b */
#define REG_PMSR_MON_TYPE_43_LSB            (1U << 22)      /* 2b */
#define REG_PMSR_MON_TYPE_44_LSB            (1U << 24)      /* 2b */
#define REG_PMSR_MON_TYPE_45_LSB            (1U << 26)      /* 2b */
#define REG_PMSR_MON_TYPE_46_LSB            (1U << 28)      /* 2b */
#define REG_PMSR_MON_TYPE_47_LSB            (1U << 30)      /* 2b */
/* SPM_PMSR_LEN_CON0 (0x10006000+0xFEC) */
#define REG_PMSR_WINDOW_LEN_SET0_LSB        (1U << 0)       /* 32b */
/* SPM_PMSR_LEN_CON1 (0x10006000+0xFF0) */
#define REG_PMSR_WINDOW_LEN_SET1_LSB        (1U << 0)       /* 32b */
/* SPM_PMSR_LEN_CON2 (0x10006000+0xFF4) */
#define REG_PMSR_WINDOW_LEN_SET2_LSB        (1U << 0)       /* 32b */

#define SPM_PROJECT_CODE	0xb16
#define SPM_REGWR_CFG_KEY	(SPM_PROJECT_CODE << 16)

#define spm_read(addr)	__raw_readl((void __force __iomem *)(addr))
#define spm_write(addr, val) mt_reg_sync_writel(val, addr)

#endif
