/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _VENC_IPI_MSG_H_
#define _VENC_IPI_MSG_H_

#include <linux/videodev2.h>
#include "vcodec_ipi_msg.h"

#define MTK_MAX_ENC_CODECS_SUPPORT       (64)
#define AP_IPIMSG_VENC_BASE 0xC000
#define VCU_IPIMSG_VENC_BASE 0xD000
#define VCU_IPIMSG_VENC_SEND_BASE 0xE000

#define VENC_MAX_FB_NUM              VIDEO_MAX_FRAME
#define VENC_MAX_BS_NUM              VIDEO_MAX_FRAME


enum venc_ipi_msg_id {
	AP_IPIMSG_ENC_INIT = AP_IPIMSG_VENC_BASE,
	AP_IPIMSG_ENC_SET_PARAM,
	AP_IPIMSG_ENC_ENCODE,
	AP_IPIMSG_ENC_DEINIT,
	AP_IPIMSG_ENC_QUERY_CAP,

	VCU_IPIMSG_ENC_INIT_DONE = VCU_IPIMSG_VENC_BASE,
	VCU_IPIMSG_ENC_SET_PARAM_DONE,
	VCU_IPIMSG_ENC_ENCODE_DONE,
	VCU_IPIMSG_ENC_DEINIT_DONE,
	VCU_IPIMSG_ENC_QUERY_CAP_ACK,
	VCU_IPIMSG_ENC_ENCODE_ACK,

	VCU_IPIMSG_ENC_POWER_ON = VCU_IPIMSG_VENC_SEND_BASE,
	VCU_IPIMSG_ENC_POWER_OFF,
	VCU_IPIMSG_ENC_WAIT_ISR,
	VCU_IPIMSG_ENC_PUT_BUFFER
};

enum venc_get_param_type {
	GET_PARAM_CAPABILITY_SUPPORTED_FORMATS,
	GET_PARAM_CAPABILITY_FRAME_SIZES,
	GET_PARAM_FREE_BUFFERS,
	GET_PARAM_ROI_RC_QP,
	GET_PARAM_RESOLUTION_CHANGE,
};

enum venc_set_param_type {
	VENC_SET_PARAM_ENC,
	VENC_SET_PARAM_FORCE_INTRA,
	VENC_SET_PARAM_ADJUST_BITRATE,
	VENC_SET_PARAM_ADJUST_FRAMERATE,
	VENC_SET_PARAM_GOP_SIZE,
	VENC_SET_PARAM_INTRA_PERIOD,
	VENC_SET_PARAM_SKIP_FRAME,
	VENC_SET_PARAM_PREPEND_HEADER,
	VENC_SET_PARAM_TS_MODE,
	VENC_SET_PARAM_SCENARIO,
	VENC_SET_PARAM_NONREFP,
	VENC_SET_PARAM_DETECTED_FRAMERATE,
	VENC_SET_PARAM_RFS_ON,
	VENC_SET_PARAM_PREPEND_SPSPPS_TO_IDR,
	VENC_SET_PARAM_OPERATION_RATE,
	VENC_SET_PARAM_BITRATE_MODE,
	VENC_SET_PARAM_ROI_ON,
	VENC_SET_PARAM_HEIF_GRID_SIZE,
	VENC_SET_PARAM_COLOR_DESC,
	VENC_SET_PARAM_SEC_MODE,
	VENC_SET_PARAM_TSVC,
	VENC_SET_PARAM_NONREFPFREQ,
};

struct venc_ap_ipi_msg_init {
	__u32 msg_id;
	__u32 reserved;
	__u64 venc_inst;
};

struct venc_ap_ipi_query_cap {
	__u32 msg_id;
	__u32 id;
#ifndef CONFIG_64BIT
	union {
		__u64 ap_inst_addr_64;
		__u32 ap_inst_addr;
	};
	union {
		__u64 ap_data_addr_64;
		__u32 ap_data_addr;
	};
#else
	__u64 ap_inst_addr;
	__u64 ap_data_addr;
#endif
};

struct venc_vcu_ipi_query_cap_ack {
	__u32 msg_id;
	__s32 status;
#ifndef CONFIG_64BIT
	union {
		__u64 ap_inst_addr_64;
		__u32 ap_inst_addr;
	};
	__u32 id;
	union {
		__u64 ap_data_addr_64;
		__u32 ap_data_addr;
	};
#else
	__u64 ap_inst_addr;
	__u32 id;
	__u64 ap_data_addr;
#endif
	__u32 vcu_data_addr;
};

struct venc_ap_ipi_msg_set_param {
	__u32 msg_id;
	__u32 vcu_inst_addr;
	__u32 param_id;
	__u32 data_item;
	__u32 data[8];
};

struct venc_ap_ipi_msg_enc {
	__u32 msg_id;
	__u32 vcu_inst_addr;
	__u32 input_addr[3];
	__u32 input_size[3];
	__u32 bs_addr;
	__u32 bs_size;
	__u32 data_offset[3];
	__s16 input_fd[3];
	__s16 bs_fd;
	__u8 fb_num_planes;
	__u8 bs_mode;
};

struct venc_ap_ipi_msg_deinit {
	__u32 msg_id;
	__u32 vcu_inst_addr;
};

enum venc_ipi_msg_status {
	VENC_IPI_MSG_STATUS_OK,
	VENC_IPI_MSG_STATUS_FAIL,
};

struct venc_vcu_ipi_msg_common {
	__u32 msg_id;
	__s32 status;
	__u64 venc_inst;
};

struct venc_vcu_ipi_msg_init {
	__u32 msg_id;
	__s32 status;
	__u64 venc_inst;
	__u32 vcu_inst_addr;
	__u32 reserved;
};

struct venc_vcu_ipi_msg_set_param {
	__u32 msg_id;
	__s32 status;
	__u64 venc_inst;
	__u32 param_id;
	__u32 data_item;
	__u32 data[6];
};

enum venc_ipi_msg_enc_state {
	VEN_IPI_MSG_ENC_STATE_FRAME,
	VEN_IPI_MSG_ENC_STATE_PART,
	VEN_IPI_MSG_ENC_STATE_SKIP,
	VEN_IPI_MSG_ENC_STATE_ERROR,
};

struct venc_vcu_ipi_msg_enc {
	__u32 msg_id;
	__s32 status;
	__u64 venc_inst;
	__u32 state;
	__u32 is_key_frm;
	__u32 bs_size;
	__u32 reserved;
};

struct venc_vcu_ipi_msg_deinit {
	__u32 msg_id;
	__s32 status;
	__u64 venc_inst;
};

struct venc_vcu_ipi_msg_waitisr {
	__u32 msg_id;
	__s32 status;
	__u64 venc_inst;
	__u32 irq_status;
	__u32 timeout;
};

struct venc_vcu_config {
	__u32 input_fourcc;
	__u32 bitrate;
	__u32 pic_w;
	__u32 pic_h;
	__u32 buf_w;
	__u32 buf_h;
	__u32 gop_size;
	__u32 intra_period;
	__u32 framerate;
	__u32 profile;
	__u32 level;
	__u32 wfd;
	__u32 operationrate;
	__u32 scenario;
	__u32 prependheader;
	__u32 bitratemode;
	__u32 roi_rc_qp;
	__u32 roion;
	__u32 heif_grid_size;
	struct mtk_color_desc color_desc;
	__u32 resolutionChange;
	__u32 max_w;
	__u32 max_h;
	__u32 num_b_frame;
	__u32 slbc_ready;
	__u32 i_qp;
	__u32 p_qp;
	__u32 b_qp;
	__u32 svp_mode;
	__u32 tsvc;
};

enum venc_bs_mode {
	VENC_BS_MODE_SPS = 0,
	VENC_BS_MODE_PPS,
	VENC_BS_MODE_SEQ_HDR,
	VENC_BS_MODE_FRAME,
	VENC_BS_MODE_FRAME_FINAL,
	VENC_BS_MODE_MAX
};

struct venc_info {
	__u64 bs_dma;
	__u64 bs_fd;
	__u64 fb_dma[VIDEO_MAX_PLANES];
	__u64 fb_fd[VIDEO_MAX_PLANES];
	__u64 venc_bs_va;
	__u64 venc_fb_va;
	__u32 fb_num_planes;
	__u32 index;
	__u64 timestamp;
	__u32 roimap;
};

struct ring_input_list {
	__u64 venc_bs_va_list[VENC_MAX_FB_NUM];
	__u64 venc_fb_va_list[VENC_MAX_FB_NUM];
	__u32 bs_size[VENC_MAX_FB_NUM];
	__u32 is_key_frm[VENC_MAX_FB_NUM];
	__s32 read_idx;
	__s32 write_idx;
	__s32 count;
	__s32 reserved;
};

struct venc_vsi {
	struct venc_vcu_config config;
	__u32  sizeimage[VIDEO_MAX_PLANES];
	struct ring_input_list list_free;
	struct venc_info       venc;
	__u32 sync_mode;
	__u32 meta_size;
	__u64 meta_addr;
	__s16 meta_fd;
};

#endif /* _VENC_IPI_MSG_H_ */
