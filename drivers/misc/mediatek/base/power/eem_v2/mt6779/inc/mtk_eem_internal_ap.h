/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MTK_EEM_INTERNAL_AP_H_
#define _MTK_EEM_INTERNAL_AP_H_

struct eem_det;
struct eem_ctrl;

struct eem_det_ops {
	/* interface to EEM */
	void (*enable)(struct eem_det *det, int reason);
	void (*disable)(struct eem_det *det, int reason);
	void (*disable_locked)(struct eem_det *det, int reason);
	void (*switch_bank)(struct eem_det *det, enum eem_phase phase);

	int (*init01)(struct eem_det *det);
	int (*init02)(struct eem_det *det);
	int (*mon_mode)(struct eem_det *det);

	int (*get_status)(struct eem_det *det);
	void (*dump_status)(struct eem_det *det);

	void (*set_phase)(struct eem_det *det, enum eem_phase phase);

	/* interface to thermal */
	int (*get_temp)(struct eem_det *det);

	/* interface to DVFS */
	int (*get_volt)(struct eem_det *det);
	int (*set_volt)(struct eem_det *det);
	void (*restore_default_volt)(struct eem_det *det);
	void (*get_freq_table)(struct eem_det *det);
	void (*get_orig_volt_table)(struct eem_det *det);

	/* interface to PMIC */
	int (*volt_2_pmic)(struct eem_det *det, int volt);
	int (*volt_2_eem)(struct eem_det *det, int volt);
	int (*pmic_2_volt)(struct eem_det *det, int pmic_val);
	int (*eem_2_pmic)(struct eem_det *det, int eev_val);
};

struct eem_det {
	const char *name;
	struct eem_det_ops *ops;
	int status;			/* TODO: enable/disable */
	int features;		/* enum eem_features */
	enum eem_ctrl_id ctrl_id;

	/* devinfo */
	unsigned int EEMINITEN;
	unsigned int EEMMONEN;
	unsigned int MDES;
	unsigned int BDES;
	unsigned int DCMDET;
	unsigned int DCBDET;
	unsigned int AGEDELTA;
	unsigned int MTDES;
	unsigned int SPEC;

	/* constant */
	unsigned int DETWINDOW;
	unsigned int VMAX;
	unsigned int VMIN;
	unsigned int DTHI;
	unsigned int DTLO;
	unsigned int VBOOT;
	unsigned int DETMAX;
	unsigned int AGECONFIG;
	unsigned int AGEM;
	unsigned int DVTFIXED;
	unsigned int VCO;
	unsigned int DCCONFIG;

	/* Generated by EEM init01. Used in EEM init02 */
	unsigned int DCVOFFSETIN;
	unsigned int AGEVOFFSETIN;
	unsigned int EEMCTL0;

	/* for PMIC */
	unsigned int eem_v_base;
	unsigned int eem_step;
	unsigned int pmic_base;
	unsigned int pmic_step;

	/* for debug */
	unsigned int dcvalues[NR_EEM_PHASE];

	unsigned int freqpct30[NR_EEM_PHASE];
	unsigned int eem_26c[NR_EEM_PHASE];
	unsigned int vop30[NR_EEM_PHASE];
	unsigned int eem_eemEn[NR_EEM_PHASE];
	int temp; /* temperature */
	unsigned int real_vboot;

#if DUMP_DATA_TO_DE
	unsigned int reg_dump_data[ARRAY_SIZE(reg_dump_addr_off)][NR_EEM_PHASE];
#endif
	/* slope */
	unsigned int MTS;
	unsigned int BTS;
	unsigned int t250;
	/* dvfs */
	unsigned int init2_vop30;
	unsigned int init2_vop74;
	unsigned int mon_vop30;
	unsigned int mon_vop74;
	/* could be got @ the same time with freq_tbl[] */
	unsigned int num_freq_tbl;
	/* maximum frequency used to calculate percentage */
	unsigned int max_freq_khz;
	unsigned char freq_tbl[NR_FREQ]; /* percentage to maximum freq */
	/* orig volt table for restoreing to dvfs*/
	unsigned int volt_tbl_orig[NR_FREQ];
	unsigned int volt_tbl[NR_FREQ]; /* pmic value */
	unsigned int volt_tbl_init2[NR_FREQ]; /* pmic value */
	unsigned int volt_tbl_pmic[NR_FREQ]; /* pmic value */
	/* unsigned int volt_offset_drcc[NR_FREQ]; *//* pmic value */
	int volt_aging[NR_FREQ]; /* aging margin value */

	/* Picachu */
	unsigned int pi_efuse;
	unsigned int pi_dvtfixed;

	int volt_dcv:8;
	int volt_offset:8;
	int volt_clamp:8;

	unsigned int disabled:8; /* Disabled by error or sysfs */
	unsigned int low_temp_off:8;
	unsigned int isTempInv:1;
	unsigned int isHighTemp:1;
	unsigned int isAddExtra:2;
	unsigned int volt_policy:1;
	/* only when init2, eem need to set volt to upower */
	unsigned int set_volt_to_upower:1;
	unsigned int init2_done:1;
	unsigned int vop_check:1;
	unsigned int detcount;
#if ENABLE_LOO
	unsigned char loo_role;
	unsigned char loo_couple;
	struct mutex *loo_mutex;
#endif
};

struct eem_devinfo {
	/* M_HW_RES0 0x11f1_0580 */
	unsigned int PI_DVTFIX_B:4;
	unsigned int FT_PGM:4;
	unsigned int TURBO:1;
	unsigned int RSV1:1;
	unsigned int REV:2;
	unsigned int PACKAGE:2;
	unsigned int FABCODE:2;
	unsigned int PI_DVTFIX_L:4;
	unsigned int FT_BIN:4;
	unsigned int RSV2:8;

	/* M_HW_RES4 */
	unsigned int CCI_BDES:8;
	unsigned int CCI_MDES:8;
	unsigned int CCI_DCBDET:8;
	unsigned int CCI_DCMDET:8;

	/* M_HW_RES5 */
	unsigned int GPU_LO_MTDES:8;
	unsigned int GPU_LO_INITEN:1;
	unsigned int GPU_LO_MONEN:1;
	unsigned int GPU_LO_DVFS_LOW:3;
	unsigned int GPU_LO_SPEC:3;
	unsigned int CCI_MTDES:8;
	unsigned int CCI_INITEN:1;
	unsigned int CCI_MONEN:1;
	unsigned int CCI_DVFS_LOW:3;
	unsigned int CCI_SPEC:3;

	/* M_HW_RES6 */
	unsigned int GPU_LO_BDES:8;
	unsigned int GPU_LO_MDES:8;
	unsigned int GPU_LO_DCBDET:8;
	unsigned int GPU_LO_DCMDET:8;

	/* M_HW_RES16 */
	unsigned int CPU_L_BDES:8;
	unsigned int CPU_L_MDES:8;
	unsigned int CPU_L_DCBDET:8;
	unsigned int CPU_L_DCMDET:8;

	/* M_HW_RES17 */
	unsigned int CPU_B_MTDES:8;
	unsigned int CPU_B_INITEN:1;
	unsigned int CPU_B_MONEN:1;
	unsigned int CPU_B_DVFS_LOW:3;
	unsigned int CPU_B_SPEC:3;
	unsigned int CPU_L_MTDES:8;
	unsigned int CPU_L_INITEN:1;
	unsigned int CPU_L_MONEN:1;
	unsigned int CPU_L_DVFS_LOW:3;
	unsigned int CPU_L_SPEC:3;

	/* M_HW_RES18 */
	unsigned int CPU_B_BDES:8;
	unsigned int CPU_B_MDES:8;
	unsigned int CPU_B_DCBDET:8;
	unsigned int CPU_B_DCMDET:8;

	/* M_HW_RES19 */
	unsigned int GPU_BDES:8;
	unsigned int GPU_MDES:8;
	unsigned int GPU_DCBDET:8;
	unsigned int GPU_DCMDET:8;

	/* M_HW_RES23 */
	unsigned int GPU_HI_MTDES:8;
	unsigned int GPU_HI_INITEN:1;
	unsigned int GPU_HI_MONEN:1;
	unsigned int GPU_HI_DVFS_LOW:3;
	unsigned int GPU_HI_SPEC:3;
	unsigned int GPU_MTDES:8;
	unsigned int GPU_INITEN:1;
	unsigned int GPU_MONEN:1;
	unsigned int GPU_DVFS_LOW:3;
	unsigned int GPU_SPEC:3;

	/* M_HW_RES24 */
	unsigned int GPU_HI_BDES:8;
	unsigned int GPU_HI_MDES:8;
	unsigned int GPU_HI_DCBDET:8;
	unsigned int GPU_HI_DCMDET:8;

};

extern unsigned int freq[NR_FREQ];

extern struct mutex record_mutex;
extern struct mutex lcpu_mutex;
extern struct mutex bcpu_mutex;
extern struct mutex gpu_mutex;

/* #define DRCC_SUPPORT 1 */
extern void mt_record_lock(unsigned long *flags);
extern void mt_record_unlock(unsigned long *flags);

/* table used to apply to dvfs at final */
extern unsigned int record_tbl_locked[NR_FREQ];

extern struct eem_det_ops gpu_det_ops;
extern struct eem_det_ops vpu_det_ops;
extern struct eem_det_ops mdla_det_ops;
extern struct eem_det_ops cpu_det_ops;
extern struct eem_det_ops cci_det_ops;

extern int get_volt_cpu(struct eem_det *det);
extern int set_volt_cpu(struct eem_det *det);
extern void restore_default_volt_cpu(struct eem_det *det);
extern void get_freq_table_cpu(struct eem_det *det);
extern void get_orig_volt_table_cpu(struct eem_det *det);
extern int get_volt_gpu(struct eem_det *det);
extern int set_volt_gpu(struct eem_det *det);
extern void restore_default_volt_gpu(struct eem_det *det);
extern void get_freq_table_gpu(struct eem_det *det);
extern void get_orig_volt_table_gpu(struct eem_det *det);
extern int get_volt_vpu(struct eem_det *det);
extern int set_volt_vpu(struct eem_det *det);
extern int get_volt_mdla(struct eem_det *det);
extern void restore_default_volt_vpu(struct eem_det *det);
extern void get_freq_table_vpu(struct eem_det *det);
extern void get_orig_volt_table_vpu(struct eem_det *det);

extern void base_ops_enable(struct eem_det *det, int reason);
extern void base_ops_disable(struct eem_det *det, int reason);
extern void base_ops_disable_locked(struct eem_det *det, int reason);
extern void base_ops_switch_bank(struct eem_det *det, enum eem_phase phase);

extern int base_ops_init01(struct eem_det *det);
extern int base_ops_init02(struct eem_det *det);
extern int base_ops_mon_mode(struct eem_det *det);

extern int base_ops_get_status(struct eem_det *det);
extern void base_ops_dump_status(struct eem_det *det);

extern void base_ops_set_phase(struct eem_det *det, enum eem_phase phase);
extern int base_ops_get_temp(struct eem_det *det);
extern int base_ops_get_volt(struct eem_det *det);
extern int base_ops_set_volt(struct eem_det *det);
extern void base_ops_restore_default_volt(struct eem_det *det);
extern void base_ops_get_freq_table(struct eem_det *det);
extern void base_ops_get_orig_volt_table(struct eem_det *det);
#endif

