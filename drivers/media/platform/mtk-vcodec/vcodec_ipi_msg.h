/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _VCODEC_IPI_MSG_H_
#define _VCODEC_IPI_MSG_H_

#include <linux/videodev2.h>
#include <linux/v4l2-controls.h>

enum mtk_venc_hw_id {
	MTK_VENC_CORE_0 = 0,
	MTK_VENC_CORE_1 = 1,
	MTK_VENC_HW_NUM = 2,
};

enum mtk_vdec_hw_id {
	MTK_VDEC_CORE = 0,
	MTK_VDEC_LAT = 1,
	MTK_VDEC_HW_NUM = 2,
};

enum mtk_fmt_type {
	MTK_FMT_DEC = 0,
	MTK_FMT_ENC = 1,
	MTK_FMT_FRAME = 2,
};

struct mtk_video_fmt {
	__u32	fourcc;
	__u32	type;   /* enum mtk_fmt_type */
	__u32	num_planes;
};

struct mtk_codec_framesizes {
	__u32	fourcc;
	__u32	profile;
	__u32	level;
	struct	v4l2_frmsize_stepwise	stepwise;
};


struct mtk_color_desc {
	__u32	color_primaries;
	__u32	transform_character;
	__u32	matrix_coeffs;
	__u32	display_primaries_x[3];
	__u32	display_primaries_y[3];
	__u32	white_point_x;
	__u32	white_point_y;
	__u32	max_display_mastering_luminance;
	__u32	min_display_mastering_luminance;
	__u32	max_content_light_level;
	__u32	max_pic_light_level;
	__u32   is_hdr;
	__u32   full_range;
};

struct mtk_hdr_dynamic_info {
	__u32    max_sc_lR;
		// u(17); Max R Nits *10; in the range of 0x00000-0x186A0
	__u32    max_sc_lG;
		// u(17); Max G Nits *10; in the range of 0x00000-0x186A0
	__u32    max_sc_lB;
		// u(17); Max B Nits *10; in the range of 0x00000-0x186A0
	__u32    avg_max_rgb;
		// u(17); Average maxRGB Nits *10; in 0x00000-0x186A0
	__u32    distribution_values[9];
		/* u(17)
		 * 0=1% percentile maxRGB Nits *10
		 * 1=Maximum Nits of 99YF *10
		 * 2=Average Nits of DPY100F
		 * 3=25% percentile maxRGB Nits *10
		 * 4=50% percentile maxRGB Nits *10
		 * 5=75% percentile maxRGB Nits *10
		 * 6=90% percentile maxRGB Nits *10
		 * 7=95% percentile maxRGB Nits *10
		 * 8=99.95% percentile maxRGB Nits *10
		 */
};

struct vdec_pic_info {
	__u32 pic_w;
	__u32 pic_h;
	__u32 buf_w;
	__u32 buf_h;
	__u32 fb_sz[VIDEO_MAX_PLANES];
	__u32 bitdepth;
	__u32 layout_mode;
	__u32 fourcc;
};

struct vdec_dec_info {
	__u32 dpb_sz;
	__u32 vdec_changed_info;
	__u64 bs_dma;
	__u64 bs_fd;
	__u64 fb_dma[VIDEO_MAX_PLANES];
	__u64 fb_fd[VIDEO_MAX_PLANES];
	__u64 vdec_bs_va;
	__u64 vdec_fb_va;
	__u32 fb_num_planes;
	__u32 index;
	__u32 wait_key_frame;
	__u32 error_map;
	__u64 timestamp;
	__u32 queued_frame_buf_count;
};

#endif
