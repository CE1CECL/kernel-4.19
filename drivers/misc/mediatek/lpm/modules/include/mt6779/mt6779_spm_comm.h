/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __MT6779_SPM_COMM_H__
#define __MT6779_SPM_COMM_H__

struct mt6779_spm_wake_status {
	u32 r12;			/* SPM_SW_RSV_0 */
	u32 r12_ext;		/* SPM_WAKEUP_EXT_STA */
	u32 raw_sta;		/* SPM_WAKEUP_STA */
	u32 raw_ext_sta;	/* SPM_WAKEUP_EXT_STA */
	u32 md32pcm_wakeup_sta;/* MD32CPM_WAKEUP_STA */
	u32 md32pcm_event_sta;/* MD32PCM_EVENT_STA */
	u32 wake_misc;		/* SPM_SW_RSV_5 */
	u32 timer_out;		/* SPM_SW_RSV_6 */
	u32 r13;			/* PCM_REG13_DATA */
	u32 idle_sta;		/* SUBSYS_IDLE_STA */
	u32 req_sta0;		/* SRC_REQ_STA_0 */
	u32 req_sta1;		/* SRC_REQ_STA_1 */
	u32 req_sta2;		/* SRC_REQ_STA_2 */
	u32 req_sta3;		/* SRC_REQ_STA_3 */
	u32 req_sta4;		/* SRC_REQ_STA_4 */
	u32 debug_flag;		/* PCM_WDT_LATCH_SPARE_0 */
	u32 debug_flag1;	/* PCM_WDT_LATCH_SPARE_1 */
	u32 b_sw_flag0;		/* SPM_SW_RSV_7 */
	u32 b_sw_flag1;		/* SPM_SW_RSV_8 */
	u32 isr;			/* SPM_IRQ_STA */
	u32 sw_flag0;		/* SPM_SW_FLAG_0 */
	u32 sw_flag1;		/* SPM_SW_FLAG_1 */
	u32 clk_settle;		/* SPM_CLK_SETTLE */
	u32 src_req;	/* SPM_SRC_REQ */
	u32 log_index;
	u32 is_abort;
};

/* --- SPM Flag Define --- */
#define SPM_FLAG_DISABLE_CPU_PDN               (0x1U << 0)
#define SPM_FLAG_DISABLE_INFRA_PDN             (0x1U << 1)
#define SPM_FLAG_DISABLE_DDRPHY_PDN            (0x1U << 2)
#define SPM_FLAG_DISABLE_VCORE_DVS             (0x1U << 3)
#define SPM_FLAG_DISABLE_VCORE_DFS             (0x1U << 4)
#define SPM_FLAG_DISABLE_COMMON_SCENARIO       (0x1U << 5)
#define SPM_FLAG_DISABLE_BUS_CLK_OFF           (0x1U << 6)
#define SPM_FLAG_DISABBLE_ARMPLL_OFF           (0x1U << 7)
#define SPM_FLAG_KEEP_CSYSPWRUPACK_HIGH        (0x1U << 8)
#define SPM_FLAG_DISABLE_CPU_VPROC_VSRAM_PDN   (0x1U << 9)
#define SPM_FLAG_RUN_COMMON_SCENARIO           (0x1U << 10)
#define SPM_FLAG_ENABLE_MET_DEBUG_USAGE        (0x1U << 11)
#define SPM_FLAG_RESERVED_BIT12                (0x1U << 12)
#define SPM_FLAG_SODI_NO_EVENT                 (0x1U << 13)
#define SPM_FLAG_ENABLE_SODI3                  (0x1U << 14)
#define SPM_FLAG_RESERVED_BIT15                (0x1U << 15)
#define SPM_FLAG_DISABLE_SYSRAM_SLEEP          (0x1U << 16)
#define SPM_FLAG_DISABLE_SSPM_SRAM_SLEEP       (0x1U << 17)
#define SPM_FLAG_RESERVED_BIT18                (0x1U << 18)
#define SPM_FLAG_SUSPEND_OPTION                (0x1U << 19)
#define SPM_FLAG_DEEPIDLE_OPTION               (0x1U << 20)
#define SPM_FLAG_SODI_OPTION                   (0x1U << 21)
#define SPM_FLAG_RESERVED_BIT22                (0x1U << 22)
#define SPM_FLAG_RESERVED_BIT23                (0x1U << 23)
#define SPM_FLAG_RESERVED_BIT24                (0x1U << 24)
#define SPM_FLAG_RESERVED_BIT25                (0x1U << 25)
#define SPM_FLAG_RESERVED_BIT26                (0x1U << 26)
#define SPM_FLAG_SPM_FLAG_DONT_TOUCH_BIT27     (0x1U << 27)
#define SPM_FLAG_SPM_FLAG_DONT_TOUCH_BIT28     (0x1U << 28)
#define SPM_FLAG_SPM_FLAG_DONT_TOUCH_BIT29     (0x1U << 29)
#define SPM_FLAG_SPM_FLAG_DONT_TOUCH_BIT30     (0x1U << 30)
#define SPM_FLAG_SPM_FLAG_DONT_TOUCH_BIT31     (0x1U << 31)
/* --- SPM Flag1 Define --- */
#define SPM_FLAG1_DISABLE_AXI_BUS_TO_26M       (0x1U << 0)
#define SPM_FLAG1_DISABLE_SYSPLL_OFF           (0x1U << 1)
#define SPM_FLAG1_DISABLE_PWRAP_CLK_SWITCH     (0x1U << 2)
#define SPM_FLAG1_DISABLE_ULPOSC_OFF           (0x1U << 3)
#define SPM_FLAG1_FW_SET_ULPOSC_ON             (0x1U << 4)
#define SPM_FLAG1_RESERVED_BIT5                (0x1U << 5)
#define SPM_FLAG1_DISABLE_NO_RESUME            (0x1U << 6)
#define SPM_FLAG1_ENABLE_BIG_BUCK_OFF          (0x1U << 7)
#define SPM_FLAG1_ENABLE_BIG_BUCK_ON           (0x1U << 8)
#define SPM_FLAG1_FORCE_CPU_BUCK_OFF           (0x1U << 9)
#define SPM_FLAG1_DISABLE_SRCLKEN_LOW          (0x1U << 10)
#define SPM_FLAG1_DISABLE_SCP_CLK_SWITCH       (0x1U << 11)
#define SPM_FLAG1_DISABLE_TOP_26M_CK_OFF       (0x1U << 12)
#define SPM_FLAG1_DISABPE_PCM_26M_SWITCH       (0x1U << 13)
#define SPM_FLAG1_DISABLE_CKSQ_OFF             (0x1U << 14)
#define SPM_FLAG1_RESERVED_BIT15               (0x1U << 15)
#define SPM_FLAG1_RESERVED_BIT16               (0x1U << 16)
#define SPM_FLAG1_RESERVED_BIT17               (0x1U << 17)
#define SPM_FLAG1_RESERVED_BIT18               (0x1U << 18)
#define SPM_FLAG1_RESERVED_BIT19               (0x1U << 19)
#define SPM_FLAG1_DISABLE_DEVAPC_SRAM_SLEEP    (0x1U << 20)
#define SPM_FLAG1_DISABLE_AXI_MEM_CLK_OFF      (0x1U << 21)
#define SPM_FLAG1_DISABLE_VS1_VOTER            (0x1U << 22)
#define SPM_FLAG1_DISABLE_VS2_VOTER            (0x1U << 23)
#define SPM_FLAG1_DISABLE_SCP_VREQ_MASK_CON    (0x1U << 24)
#define SPM_FLAG1_DISABLE_MD_BUCK_0P55         (0x1U << 25)
#define SPM_FLAG1_DISABLE_MD_LDO_0P60          (0x1U << 26)
#define SPM_FLAG1_RESERVED_BIT27               (0x1U << 27)
#define SPM_FLAG1_RESERVED_BIT28               (0x1U << 28)
#define SPM_FLAG1_RESERVED_BIT29               (0x1U << 29)
#define SPM_FLAG1_RESERVED_BIT30               (0x1U << 30)
#define SPM_FLAG1_RESERVED_BIT31               (0x1U << 31)
/* --- SPM DEBUG Define --- */
#define DEBUG_IDX_26M_WAKE                     (0x1U << 0)
#define DEBUG_IDX_26M_SLEEP                    (0x1U << 1)
#define DEBUG_IDX_INFRA_WAKE                   (0x1U << 2)
#define DEBUG_IDX_INFRA_SLEEP                  (0x1U << 3)
#define DEBUG_IDX_APSRC_WAKE                   (0x1U << 4)
#define DEBUG_IDX_APSRC_SLEEP                  (0x1U << 5)
#define DEBUG_IDX_VRF18_WAKE                   (0x1U << 6)
#define DEBUG_IDX_VRF18_SLEEP                  (0x1U << 7)
#define DEBUG_IDX_DDREN_WAKE                   (0x1U << 8)
#define DEBUG_IDX_DDREN_SLEEP                  (0x1U << 9)
#define DEBUG_IDX_DRAM_SREF_ABORT_IN_APSRC     (0x1U << 10)
#define DEBUG_IDX_DRAM_SREF_ABORT_IN_DDREN     (0x1U << 11)
#define DEBUG_IDX_CPU_PDN                      (0x1U << 12)
#define DEBUG_IDX_VPROC1_OFF                   (0x1U << 13)
#define DEBUG_IDX_VPROC2_OFF                   (0x1U << 14)
#define DEBUG_IDX_ARMPLL_OFF                   (0x1U << 15)
#define DEBUG_IDX_SYSRAM_SLP                   (0x1U << 16)
#define DEBUG_IDX_SSPM_WFI                     (0x1U << 17)
#define DEBUG_IDX_SSPM_SRAM_SLP                (0x1U << 18)
#define DEBUG_IDX_SSPM_ON                      (0x1U << 19)
#define DEBUG_IDX_SYSRAM_ON                    (0x1U << 20)
#define DEBUG_IDX_ARMPLL_ON                    (0x1U << 21)
#define DEBUG_IDX_VPROC1_ON                    (0x1U << 22)
#define DEBUG_IDX_VPROC2_ON                    (0x1U << 23)
#define DEBUG_IDX_SCP_VCORE_0P625V             (0x1U << 24)
#define DEBUG_IDX_SCP_VCORE_0P700V             (0x1U << 25)
#define DEBUG_IDX_SCP_VCORE_0P750V             (0x1U << 26)
#define DEBUG_IDX_SCP_VCORE_0P800V             (0x1U << 27)
#define DEBUG_IDX_VCORE_DVFS_START             (0x1U << 28)
#define DEBUG_IDX_APSRC_SLEEP_ABORT            (0x1U << 29)
#define DEBUG_IDX_AXI_MEM_CLK_OFF              (0x1U << 30)
#define DEBUG_IDX_AXI_MEM_CLK_ON               (0x1U << 31)
/* --- SPM DEBUG1 Define --- */
#define DEBUG_IDX_SPM_GO_WAKEUP_NOW            (0x1U << 0)
#define DEBUG_IDX_RESERVED_BIT1                (0x1U << 1)
#define DEBUG_IDX_SYSPLL_OFF                   (0x1U << 2)
#define DEBUG_IDX_SYSPLL_ON                    (0x1U << 3)
#define DEBUG_IDX_CURRENT_IS_VCORE_DFS         (0x1U << 4)
#define DEBUG_IDX_INFRA_MTCMOS_OFF             (0x1U << 5)
#define DEBUG_IDX_INFRA_MTCMOS_ON              (0x1U << 6)
#define DEBUG_IDX_VTCXO_SLEEP_ABORT_0          (0x1U << 7)
#define DEBUG_IDX_VTCXO_SLEEP_ABORT_1          (0x1U << 8)
#define DEBUG_IDX_RESERVED_BIT9                (0x1U << 9)
#define DEBUG_IDX_RESERVED_BIT10               (0x1U << 10)
#define DEBUG_IDX_PWRAP_CLK_TO_ULPOSC          (0x1U << 11)
#define DEBUG_IDX_PWRAP_CLK_TO_26M             (0x1U << 12)
#define DEBUG_IDX_SCP_CLK_TO_32K               (0x1U << 13)
#define DEBUG_IDX_SCP_CLK_TO_26M               (0x1U << 14)
#define DEBUG_IDX_BUS_CLK_OFF                  (0x1U << 15)
#define DEBUG_IDX_BUS_CLK_ON                   (0x1U << 16)
#define DEBUG_IDX_SRCLKEN2_LOW                 (0x1U << 17)
#define DEBUG_IDX_SRCLKEN2_HIGH                (0x1U << 18)
#define DEBUG_IDX_DORMANT_WAKEUP               (0x1U << 19)
#define DEBUG_IDX_ULPOSC_IS_OFF_BUT_SHOULD_ON  (0x1U << 20)
#define DEBUG_IDX_PMIC_IRQ_ACK_LOW_ABORT       (0x1U << 21)
#define DEBUG_IDX_PMIC_IRQ_ACK_HIGH_ABORT      (0x1U << 22)
#define DEBUG_IDX_PWRAP_SLEEP_ACK_LOW_ABORT    (0x1U << 23)
#define DEBUG_IDX_PWRAP_SLEEP_ACK_HIGH_ABORT   (0x1U << 24)
#define DEBUG_IDX_EMI_SLP_IDLE_ABORT           (0x1U << 25)
#define DEBUG_IDX_SCP_SLP_ACK_LOW_ABORT        (0x1U << 26)
#define DEBUG_IDX_SCP_SLP_ACK_HIGH_ABORT       (0x1U << 27)
#define DEBUG_IDX_SPM_DVFS_CMD_RDY_ABORT       (0x1U << 28)
#define DEBUG_IDX_MCUSYS_PWR_ACK_LOW_ABORT     (0x1U << 29)
#define DEBUG_IDX_CORE_PWR_ACK_HIGH_ABORT      (0x1U << 30)
#define DEBUG_IDX_CURRENT_IS_LP                (0x1U << 31)
/* --- SPM DEBUG2 Define --- */
#define DEBUG_IDX_VS2_BIT0                     (0x1U << 0)
#define DEBUG_IDX_VS2_BTT1                     (0x1U << 1)
#define DEBUG_IDX_VS2_BIT2                     (0x1U << 2)
#define DEBUG_IDX_VS1_BIT0                     (0x1U << 3)
#define DEBUG_IDX_VS1_BIT1                     (0x1U << 4)


/* ABORT MASK for DEBUG FOORTPRINT */
#define DEBUG_ABORT_MASK (DEBUG_IDX_DRAM_SREF_ABORT_IN_APSRC \
	| DEBUG_IDX_DRAM_SREF_ABORT_IN_DDREN)

#define DEBUG_ABORT_MASK_1 (DEBUG_IDX_VTCXO_SLEEP_ABORT_0 \
	| DEBUG_IDX_VTCXO_SLEEP_ABORT_1 \
	| DEBUG_IDX_PMIC_IRQ_ACK_LOW_ABORT \
	| DEBUG_IDX_PMIC_IRQ_ACK_HIGH_ABORT \
	| DEBUG_IDX_PWRAP_SLEEP_ACK_LOW_ABORT \
	| DEBUG_IDX_PWRAP_SLEEP_ACK_HIGH_ABORT \
	| DEBUG_IDX_EMI_SLP_IDLE_ABORT \
	| DEBUG_IDX_SCP_SLP_ACK_LOW_ABORT \
	| DEBUG_IDX_SCP_SLP_ACK_HIGH_ABORT \
	| DEBUG_IDX_SPM_DVFS_CMD_RDY_ABORT \
	| DEBUG_IDX_MCUSYS_PWR_ACK_LOW_ABORT \
	| DEBUG_IDX_CORE_PWR_ACK_HIGH_ABORT)

#endif
