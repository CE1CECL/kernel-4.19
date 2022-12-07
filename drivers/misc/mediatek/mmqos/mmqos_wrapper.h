/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MMQOS_WRAPPER_H__
#define __MMQOS_WRAPPER_H__

#include <dt-bindings/interconnect/mtk,mmqos.h>
#include <linux/interconnect.h>
#include <soc/mediatek/mmqos.h>
#include "smi_master_port.h"


enum {
	BW_COMP_NONE = 0,
	BW_COMP_DEFAULT,
	BW_COMP_END
};

enum virtual_source_id {
	VIRTUAL_DISP = 0,
	VIRTUAL_CCU_COMMON
};

struct mm_qos_request {
	struct list_head owner_node;	/* To update all master once */
	u32 master_id;	/* larb and port combination */
	u32 bw_value;	/* Master data BW */
	u32 hrt_value;	/* Master hrt BW */
	u32 comp_type;	/* compression type */
	bool init;	/* initialized check */
	bool updated;	/* update check */
	struct icc_path *icc_path;
};

#if IS_ENABLED(CONFIG_INTERCONNECT_MTK_MMQOS_COMMON)

s32 mm_qos_add_request(struct list_head *owner_list,
	struct mm_qos_request *req, u32 master_id);

s32 mm_qos_set_request(struct mm_qos_request *req,
	u32 bw_value, u32 hrt_value, u32 comp_type);

s32 mm_qos_set_bw_request(struct mm_qos_request *req,
	u32 bw_value, s32 comp_type);

s32 mm_qos_set_hrt_request(struct mm_qos_request *req, u32 hrt_value);

void mm_qos_update_all_request(struct list_head *owner_list);

void mm_qos_remove_all_request(struct list_head *owner_list);

void mm_qos_update_all_request_zero(struct list_head *owner_list);

s32 mm_hrt_get_available_hrt_bw(u32 master_id);

s32 mm_hrt_add_bw_throttle_notifier(struct notifier_block *nb);

s32 mm_hrt_remove_bw_throttle_notifier(struct notifier_block *nb);


s32 get_virtual_port(enum virtual_source_id id);
#else
static inline s32 mm_qos_add_request(struct list_head *owner_list,
	struct mm_qos_request *req, u32 master_id)
	{ return 0; }
static inline s32 mm_qos_set_request(struct mm_qos_request *req, u32 bw_value,
	u32 hrt_value, s32 comp_type)
	{ return 0; }
static inline s32 mm_qos_set_bw_request(struct mm_qos_request *req,
	u32 bw_value, s32 comp_type)
	{ return 0; }
static inline s32 mm_qos_set_hrt_request(struct mm_qos_request *req,
	u32 hrt_value)
	{ return 0; }
static inline void mm_qos_update_all_request(struct list_head *owner_list)
	{ return; }
static inline void mm_qos_remove_all_request(struct list_head *owner_list)
	{ return; }
static inline void mm_qos_update_all_request_zero(
	struct list_head *owner_list)
	{ return; }
static inline s32 mm_hrt_get_available_hrt_bw(u32 master_id)
	{ return -1; }
static inline s32 mm_hrt_add_bw_throttle_notifier(struct notifier_block *nb)
	{ return 0; }
static inline s32 mm_hrt_remove_bw_throttle_notifier(struct notifier_block *nb)
	{ return 0; }
static inline s32 get_virtual_port(enum virtual_source_id id)
	{ return 0; }
#endif
#endif /* __MMQOS_WRAPPER_H__ */
