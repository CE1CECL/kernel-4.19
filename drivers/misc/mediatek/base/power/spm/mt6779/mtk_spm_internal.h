/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __MTK_SPM_INTERNAL_H__
#define __MTK_SPM_INTERNAL_H__

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/io.h>

#include <linux/arm-smccc.h>
#include <linux/soc/mediatek/mtk_sip_svc.h>
#include <mt-plat/sync_write.h>
#include <mt-plat/aee.h>

#include <mtk_spm_reg.h>
#include <pwr_ctrl.h>
//FIXME
#include <mtk_spm.h>
#include <mtk_idle.h>

/* IMPORTANT NOTE: Check cpuidle header file version every time !! */
/* For mt6763, use cpuidle_v2/mtk_cpuidle.h */
/* For mt6739/mt6775/mt6771 or later project, use cpuidle_v3/mtk_cpuidle.h */
#include <cpuidle_v3/mtk_cpuidle.h> /* atf/dormant header file */


#define MTK_SPM_ARCH_SCENARIO_ORIENTED  (0)
#define MTK_SPM_ARCH_RESOURCE_ORIENTED  (1)
#define MTK_SPM_ARCH_TYPE	(MTK_SPM_ARCH_RESOURCE_ORIENTED)



#define MTK_IDLE_FEATURE_ENABLE_DPIDLE  (0)
#define MTK_IDLE_FEATURE_ENABLE_SODI    (0)
#define MTK_IDLE_FEATURE_ENABLE_SODI3   (0)
#define MTK_FEATURE_ENABLE_KICK_SPMFW	(1)
#define MTK_SPM_HARDWARE_CG_CHECK	(0)

#define LOG_BUF_SIZE        256
#define SPM_WAKE_PERIOD     600 /* sec */

#define PCM_TIMER_RAMP_BASE_DPIDLE          80          /* 80/32000 = 2.5 ms */
#define PCM_TIMER_RAMP_BASE_SUSPEND_50MS    0xA0
#define PCM_TIMER_RAMP_BASE_SUSPEND_SHORT   0x7D000     /* 16sec */
#define PCM_TIMER_RAMP_BASE_SUSPEND_LONG    0x927C00    /* 5min */


#define DEEPIDLE_OPT_VCORE_LOW_VOLT      (1 << 4)

/* PCM_WDT_VAL */
#define PCM_WDT_TIMEOUT		(30 * 32768)	/* 30s */
/* PCM_TIMER_VAL */
#define PCM_TIMER_MAX		(0xffffffff - PCM_WDT_TIMEOUT)

static inline size_t mt_secure_call(size_t function_id,
				size_t arg0, size_t arg1, size_t arg2,
				size_t arg3)
{
	struct arm_smccc_res res;

	arm_smccc_smc(function_id, arg0, arg1,
	arg2, arg3, 0, 0, 0, &res);
	return res.a0;
}

/* SMC call's marco */
#define SMC_CALL(_name, _arg0, _arg1, _arg2) \
	mt_secure_call(MTK_SIP_KERNEL_SPM_##_name, _arg0, _arg1, _arg2, 0)

extern spinlock_t __spm_lock;

extern void __iomem *spm_base;
#undef SPM_BASE
#define SPM_BASE spm_base

extern void __iomem *sleep_reg_md_base;
#undef SLEEP_REG_MD_BASE
#define SLEEP_REG_MD_BASE sleep_reg_md_base


extern struct pwr_ctrl pwrctrl_sleep;
extern struct pwr_ctrl pwrctrl_dp;
extern struct pwr_ctrl pwrctrl_so3;
extern struct pwr_ctrl pwrctrl_so;

/* For PHYPLL mode check */
#define DEBUG_PHYPLL_MASK (R0_SC_DPHY_RXDLY_TRACK_EN \
	| R0_SC_PHYPLL_SHU_EN \
	| R0_SC_PHYPLL2_SHU_EN \
	| R0_SC_PHYPLL_MODE_SW \
	| R0_SC_PHYPLL2_MODE_SW)

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

/* SMC: defined parameters for MTK_SIP_KERNEL_SPM_ARGS */
enum {
	SPM_ARGS_SPMFW_IDX_KICK = 0,
	SPM_ARGS_SPMFW_INIT,
	SPM_ARGS_SUSPEND,
	SPM_ARGS_SUSPEND_FINISH,
	SPM_ARGS_SODI,
	SPM_ARGS_SODI_FINISH,
	SPM_ARGS_DPIDLE,
	SPM_ARGS_DPIDLE_FINISH,
	SPM_ARGS_PCM_WDT,
	SPM_ARGS_SUSPEND_CALLBACK,
	SPM_ARGS_HARDWARE_CG_CHECK,
	SPM_ARGS_NUM,
};

/* Note: defined parameters for SPM_ARGS_SUSPEND_CALLBACK */
enum {
	SUSPEND_CALLBACK = 0,
	RESUME_CALLBACK,
};
enum {
	SUSPEND_CALLBACK_USER_ADSP = 0,
	SUSPEND_CALLBACK_USER_NUM,
};

/* Note: used by kernel/sspm/atf */
enum {
	SPM_SUSPEND,
	SPM_RESUME,
	SPM_DPIDLE_ENTER,
	SPM_DPIDLE_LEAVE,
	SPM_ENTER_SODI,
	SPM_LEAVE_SODI,
	SPM_ENTER_SODI3,
	SPM_LEAVE_SODI3,
	SPM_SUSPEND_PREPARE,
	SPM_POST_SUSPEND,
	SPM_DPIDLE_PREPARE,
	SPM_POST_DPIDLE,
	SPM_SODI_PREPARE,
	SPM_POST_SODI,
	SPM_SODI3_PREPARE,
	SPM_POST_SODI3,
	SPM_VCORE_PWARP_CMD,
	SPM_PWR_CTRL_SUSPEND,
	SPM_PWR_CTRL_DPIDLE,
	SPM_PWR_CTRL_SODI,
	SPM_PWR_CTRL_SODI3,
	SPM_PWR_CTRL_VCOREFS,
	SPM_TWAM_ENABLE,
};

enum {
	WR_NONE = 0,
	WR_UART_BUSY = 1,
	WR_ABORT = 2,
	WR_PCM_TIMER = 3,
	WR_WAKE_SRC = 4,
	WR_DVFSRC = 5,
	WR_PMSR = 6,
	WR_TWAM = 7,
	WR_SPM_ACK_CHK = 8,
	WR_UNKNOWN = 9,
};

enum vcorefs_smc_cmd {
	VCOREFS_SMC_CMD_0,
	VCOREFS_SMC_CMD_1,
	VCOREFS_SMC_CMD_2,
	VCOREFS_SMC_CMD_3,
	NUM_VCOREFS_SMC_CMD,
};

struct wake_status {
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
	u32 log_index;
	u32 is_abort;
};

struct spm_lp_scen {
	struct pcm_desc *pcmdesc;
	struct pwr_ctrl *pwrctrl;
	struct wake_status *wakestatus;
};

struct md_sleep_status {
	u32 slp_sleep_info1;
	u32 slp_cnt_high;
	u32 slp_cnt_low;
	u32 slp_cnt_reserve1;
	u32 slp_cnt_reserve2;
	u32 slp_sleep_time_high;
	u32 slp_sleep_time_low;
	u32 slp_sleep_time_reserve1;
	u32 slp_sleep_time_reserve2;
	u32 slp_sleep_info2;
};

enum mtk_resource_level_id {
	SPM_RES_LEVEL_DRAM,
	SPM_RES_LEVEL_SYSPLL,
	SPM_RES_LEVEL_BUS_26M,
	SPM_RES_LEVEL_PMIC_LP,
	NR_SPM_RES_LEVEL_TYPES,
	NR_SPM_RES_TYPES = NR_SPM_RES_LEVEL_TYPES,
};

enum {
	VCORE_0P6V		= 0,
	VCORE_0P575V,
	NF_VCORE,
};

enum {
	/* CLK_CFG_0 1000_0020 */
	CLKMUX_AXI		= 0,
	CLKMUX_MM		= 1,
	CLKMUX_SCP		= 2,
	CLKMUX_CKSYS_FMEM	= 3,

	/* CLK_CFG_1 1000_0030*/
	CLKMUX_IMG		= 4,
	CLKMUX_IPE		= 5,
	CLKMUX_DPE		= 6,
	CLKMUX_CAM		= 7,

	/* CLK_CFG_2 1000_0040*/
	CLKMUX_CCU		= 8,
	CLKMUX_DSP		= 9,
	CLKMUX_DSP1		= 10,
	CLKMUX_DSP2		= 11,

	/* CLK_CFG_3 1000_0050*/
	CLKMUX_DSP3		= 12,
	CLKMUX_IPU_IF		= 13,
	CLKMUX_MFG		= 14,
	CLKMUX_MFG_52M		= 15,

	/* CLK_CFG_4 1000_0060*/
	CLKMUX_CAMTG		= 16,
	CLKMUX_CAMTG2		= 17,
	CLKMUX_CAMTG3		= 18,
	CLKMUX_CAMTG4		= 19,

	/* CLK_CFG_5 1000_0070*/
	CLKMUX_UART		= 20,
	CLKMUX_SPI		= 21,
	CLKMUX_MSDC50_0_HCLK	= 22,
	CLKMUX_MSDC50_0		= 23,

	/* CLK_CFG_6 1000_0080*/
	CLKMUX_MSDC30_1		= 24,
	CLKMUX_AUDIO		= 25,
	CLKMUX_AUD_INTBUS	= 26,
	CLKMUX_PWRAP_ULPOSC	= 27,

	/* CLK_CFG_7 1000_0090*/
	CLKMUX_ATB		= 28,
	CLKMUX_POWMCU		= 29,
	CLKMUX_DPI0		= 30,
	CLKMUX_SCAM		= 31,

	/* CLK_CFG_8 1000_00A0*/
	CLKMUX_DISP_PWM		= 32,
	CLKMUX_USB_TOP		= 33,
	CLKMUX_SSUSB_XHCI	= 34,
	CLKMUX_SPM		= 35,

	/* CLK_CFG_9 1000_00B0*/
	CLKMUX_I2C		= 36,
	CLKMUX_SENINF		= 37,
	CLKMUX_SENINF1		= 38,
	CLKMUX_SENINF2		= 39,

	/* CLK_CFG_10 1000_00C0*/
	CLKMUX_DXCC		= 40,
	CLKMUX_AUD_ENGEN1	= 41,
	CLKMUX_AUD_ENGEN2	= 42,
	CLKMUX_AES_UFSFDE	= 43,

	/* CLK_CFG_11 1000_00D0*/
	CLKMUX_UFS		= 44,
	CLKMUX_AUD_1		= 45,
	CLKMUX_AUD_2		= 46,
	CLKMUX_ADSP		= 47,

	/* CLK_CFG_12 1000_00E0*/
	CLKMUX_DPMAIF_MAIN	= 48,
	CLKMUX_VENC		= 49,
	CLKMUX_VDEC		= 50,
	CLKMUX_CAMTM		= 51,

	/* CLK_CFG_13 1000_00F0*/
	CLKMUX_PWM		= 52,
	CLKMUX_AUDIO_H		= 53,
	CLKMUX_BUS_AXIMEM	= 54,
	CLKMUX_CAMTG5		= 55,

	/* CLK_CFG_14 1000_0640*/
	CLKMUX_MEM		= 56,

	NF_CLKMUX,
};

#define CLK_CHECK   (1 << 31)
#define NF_CLK_CFG            ((NF_CLKMUX / 4) + 1)

void spm_pm_stay_awake(int sec);
int spm_load_firmware_status(void);



int mtk_spm_irq_register(unsigned int spm_irq_0);
void mtk_spm_irq_backup(void);
void mtk_spm_irq_restore(void);



long spm_get_current_time_ms(void);
void rekick_vcorefs_scenario(void); /* FIXME: To be removed */
int __spm_get_pcm_timer_val(const struct pwr_ctrl *pwrctrl);
void __spm_set_pwrctrl_pcm_flags(struct pwr_ctrl *pwrctrl, u32 flags);
void __spm_set_pwrctrl_pcm_flags1(struct pwr_ctrl *pwrctrl, u32 flags);
void __spm_sync_pcm_flags(struct pwr_ctrl *pwrctrl);
void __spm_get_wakeup_status(struct wake_status *wakesta);
void __spm_save_ap_sleep_info(struct wake_status *wakesta);
void __spm_save_26m_sleep_info(void);
unsigned int __spm_output_wake_reason(
	const struct wake_status *wakesta, bool suspend, const char *scenario);
unsigned int __spm_get_wake_period(int pwake_time, unsigned int last_wr);

 /* SPM_IRQ_MASK */
#define ISRM_TWAM       (1U << 2)
#define ISRM_PCM_RETURN (1U << 3)
#define ISRM_RET_IRQ0   (1U << 8)
#define ISRM_RET_IRQ1   (1U << 9)
#define ISRM_RET_IRQ2   (1U << 10)
#define ISRM_RET_IRQ3   (1U << 11)
#define ISRM_RET_IRQ4   (1U << 12)
#define ISRM_RET_IRQ5   (1U << 13)
#define ISRM_RET_IRQ6   (1U << 14)
#define ISRM_RET_IRQ7   (1U << 15)
#define ISRM_RET_IRQ8   (1U << 16)
#define ISRM_RET_IRQ9   (1U << 17)
#define ISRM_RET_IRQ_AUX (\
	ISRM_REQ_IRQ1 | ISRM_REQ_IRQ2 | ISRM_REQ_IRQ3 | \
	ISRM_REQ_IRQ4 | ISRM_REQ_IRQ5 | ISRM_REQ_IRQ6 | \
	ISRM_REQ_IRQ7 | ISRM_REQ_IRQ8 | ISRM_REQ_IRQ9)
#define ISRM_ALL_EXC_TWAM \
	(ISRM_RET_IRQ_AUX /*| ISRM_RET_IRQ0 | ISRM_PCM_RETURN*/)
#define ISRM_ALL        (ISRM_ALL_EXC_TWAM | ISRM_TWAM)

/* SPM_IRQ_STA */
#define ISRS_TWAM           (1U << 2)
#define ISRS_PCM_RETURN     (1U << 3)
#define ISRS_SW_INT0        (1U << 4)
#define ISRS_SW_INT1        (1U << 5)
#define ISRC_TWAM           ISRS_TWAM
#define ISRC_ALL_EXC_TWAM   ISRS_PCM_RETURN
#define ISRC_ALL            (ISRC_ALL_EXC_TWAM | ISRC_TWAM)

void spm_twam_register_handler(twam_handler_t handler);
twam_handler_t spm_twam_handler_get(void);
void spm_twam_register_handler(twam_handler_t handler);
void spm_twam_enable_monitor(bool en_monitor, bool debug_signal,
			 twam_handler_t cb_handler);
void spm_twam_config_channel(struct twam_cfg *cfg, bool speed_mode,
			unsigned int window_len_hz);
bool spm_twam_met_enable(void);
void spm_twam_set_idle_select(unsigned int sel);
struct ddrphy_golden_cfg {
	u32 base;
	u32 offset;
	u32 mask;
	u32 value;
};

int spm_get_spmfw_idx(void);
void spm_do_dram_config_check(void);



void mtk_idle_power_pre_process(int idle_type, unsigned int op_cond);
void mtk_idle_power_pre_process_async_wait(int idle_type, unsigned int op_cond);
void mtk_idle_power_post_process(int idle_type, unsigned int op_cond);
void mtk_idle_power_post_process_async_wait(int idle_type,
	unsigned int op_cond);



/* call resource_usage check for resource-oriented adjust */
extern bool mtk_idle_resource_pre_process(void);
/* call dormant/atf driver for idle scenario */
extern int mtk_idle_trigger_wfi(
	int idle_type, unsigned int idle_flag, int cpu);
/* call before wfi and setup spm related settings */
extern void mtk_idle_pre_process_by_chip(int idle_type, int cpu,
	unsigned int op_cond, unsigned int idle_flag);
/* call after wfi and clean up spm related settings */
extern void mtk_idle_post_process_by_chip(int idle_type, int cpu,
	unsigned int op_cond, unsigned int idle_flag);
/* get sleep dpidle last wake reason */
extern unsigned int get_slp_dp_last_wr(void);


/* spm architecture (resource/scenario-oriented) */
extern void mtk_spm_arch_type_init(void);
extern bool mtk_spm_arch_type_get(void);
extern void mtk_spm_arch_type_set(bool type);

extern void mtk_suspend_cond_info(void);

/* append idle block info to input logbuf */
extern int mtk_idle_cond_append_info(
	bool short_log, int idle_type, char *logptr, unsigned int logsize);

/* enable idle cg monitor and print log to SYS_FTRACE */
extern void mtk_idle_cg_monitor(int sel);

/* read mtcmos/cg statue for later check */
extern void mtk_idle_cond_update_state(void);

/* check idle condition for specific idle type */
extern bool mtk_idle_cond_check(int idle_type);

bool mtk_idle_check_vcore_cond(void);

/* check clkmux for vcore lp mode */
extern bool mtk_idle_cond_vcore_lp_mode(int idle_type);
extern unsigned int mtk_idle_cond_vcore_ulposc_state(void);

/* mask/unmask block mask */
void mtk_idle_cond_update_mask(
	int idle_type, unsigned int reg, unsigned int mask);

bool mtk_idle_check_clkmux(int idle_type,
	unsigned int block_mask[NR_TYPES][NF_CLK_CFG]);
int spm_dvfs_flag_init(int dvfsrc_en);
void spm_go_to_vcorefs(int spm_flags);
void spm_vcorefs_init(void);
int is_spm_enabled(void);
extern struct spm_lp_scen __spm_suspend;
extern struct spm_lp_scen __spm_dpidle;
extern struct spm_lp_scen __spm_sodi3;
extern struct spm_lp_scen __spm_sodi;

extern u64 spm_26M_off_count;
extern u64 spm_26M_off_duration;
extern u64 ap_pd_count;
extern u64 ap_slp_duration;

#endif /* __MTK_SPM_INTERNAL_H__ */
