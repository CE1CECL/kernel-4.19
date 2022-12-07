/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MMDVFS_PMQOS_H__
#define __MMDVFS_PMQOS_H__

#include <linux/pm_qos.h>

#define MAX_FREQ_STEP 6

enum {
	BW_THROTTLE_START = 1,
	BW_THROTTLE_START_RECOVER,
	BW_THROTTLE_END
};

enum {
	BW_COMP_NONE = 0,
	BW_COMP_DEFAULT,
	BW_COMP_VENC,
	BW_COMP_END
};

enum virtual_source_id {
	VIRTUAL_DISP = 0,
	VIRTUAL_MD,
	VIRTUAL_CCU_COMMON,
	VIRTUAL_CCU_COMMON2
};

struct mm_qos_request {
	struct plist_node owner_node;	/* To update all master once */
	struct list_head larb_node;	/* To update larb setting */
	struct list_head port_node;	/* To update ostd in the same port */
	u32 master_id;	/* larb and port combination */
	u32 bw_value;	/* Master data BW */
	u32 hrt_value;	/* Master hrt BW */
	u32 ostd;	/* Master ostd */
	u32 comp_type;	/* compression type */
	bool init;	/* initialized check */
	bool updated;	/* update check */
	struct pm_qos_request qos_request;	/* EMI setting */
};

enum mmdvfs_limit_source {
	MMDVFS_LIMIT_THERMAL = 0,
	MMDVFS_LIMIT_CAM,
};

s32 mm_qos_add_request(struct plist_head *owner_list,
	struct mm_qos_request *req, u32 master_id);

s32 mm_qos_set_request(struct mm_qos_request *req,
	u32 bw_value, u32 hrt_value, u32 comp_type);

s32 mm_qos_set_bw_request(struct mm_qos_request *req,
	u32 bw_value, s32 comp_type);

s32 mm_qos_set_hrt_request(struct mm_qos_request *req, u32 hrt_value);

void mm_qos_update_all_request(struct plist_head *owner_list);

void mm_qos_remove_all_request(struct plist_head *owner_list);

void mm_qos_update_all_request_zero(struct plist_head *owner_list);

s32 mm_hrt_get_available_hrt_bw(u32 master_id);

s32 mm_hrt_add_bw_throttle_notifier(struct notifier_block *nb);

s32 mm_hrt_remove_bw_throttle_notifier(struct notifier_block *nb);

void mmdvfs_set_max_camera_hrt_bw(u32 bw);

int mmdvfs_qos_get_freq_steps(
	u32 pm_qos_class, u64 *freq_steps, u32 *step_size);

int mmdvfs_qos_force_step(int step);

void mmdvfs_qos_enable(bool enable);

void mmdvfs_autok_qos_enable(bool enable);

u64 mmdvfs_qos_get_freq(u32 pm_qos_class);


void mmdvfs_qos_limit_config(u32 pm_qos_class, u32 limit_value,
	enum mmdvfs_limit_source source);

void mmdvfs_print_larbs_info(void);

enum mmdvfs_prepare_event {
	MMDVFS_PREPARE_CALIBRATION_START, MMDVFS_PREPARE_CALIBRATION_END
};

void mmdvfs_prepare_action(enum mmdvfs_prepare_event event);


s32 get_virtual_port(enum virtual_source_id id);

#endif /* __MMDVFS_PMQOS_H__ */

