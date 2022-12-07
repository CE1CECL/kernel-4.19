/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MTK_JPEG_CORE_H
#define _MTK_JPEG_CORE_H

#include <linux/interrupt.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fh.h>

#define MTK_JPEG_NAME		"mtk-jpeg"

#define MTK_JPEG_FMT_FLAG_DEC_OUTPUT	BIT(0)
#define MTK_JPEG_FMT_FLAG_DEC_CAPTURE	BIT(1)
#define MTK_JPEG_FMT_FLAG_ENC_OUTPUT	BIT(2)
#define MTK_JPEG_FMT_FLAG_ENC_CAPTURE	BIT(3)

#define MTK_JPEG_FMT_TYPE_OUTPUT	1
#define MTK_JPEG_FMT_TYPE_CAPTURE	2

#define MTK_JPEG_MIN_WIDTH	32
#define MTK_JPEG_MIN_HEIGHT	32
#define MTK_JPEG_MAX_WIDTH	8192
#define MTK_JPEG_MAX_HEIGHT	8192

#define MTK_JPEG_DEFAULT_SIZEIMAGE	(1 * 1024 * 1024)

#define MTK_JPEG_ENCODE		0
#define MTK_JPEG_DECODE		1

enum mtk_jpeg_ctx_state {
	MTK_JPEG_INIT = 0,
	MTK_JPEG_RUNNING,
	MTK_JPEG_SOURCE_CHANGE,
};

enum mtk_jpeg_mode {
	MTK_JPEG_ENC,
	MTK_JPEG_DEC,
};

enum jpeg_enc_yuv_fmt {
	JPEG_YUV_FORMAT_YUYV = 0,
	JPEG_YUV_FORMAT_YVYU = 1,
	JPEG_YUV_FORMAT_NV12 = 2,
	JEPG_YUV_FORMAT_NV21 = 3,
};

enum JPEG_ENCODE_QUALITY_ENUM {
	JPEG_ENCODE_QUALITY_Q60 = 0x0,
	JPEG_ENCODE_QUALITY_Q80 = 0x1,
	JPEG_ENCODE_QUALITY_Q90 = 0x2,
	JPEG_ENCODE_QUALITY_Q95 = 0x3,
	JPEG_ENCODE_QUALITY_Q39 = 0x4,
	JPEG_ENCODE_QUALITY_Q68 = 0x5,
	JPEG_ENCODE_QUALITY_Q84 = 0x6,
	JPEG_ENCODE_QUALITY_Q92 = 0x7,
	JPEG_ENCODE_QUALITY_Q48 = 0x8,
	JPEG_ENCODE_QUALITY_Q74 = 0xA,
	JPEG_ENCODE_QUALITY_Q87 = 0xB,
	JPEG_ENCODE_QUALITY_Q34 = 0xC,
	JPEG_ENCODE_QUALITY_Q64 = 0xE,
	JPEG_ENCODE_QUALITY_Q82 = 0xF,
	JPEG_ENCODE_QUALITY_Q97 = 0x10,
	JPEG_ENCODE_QUALITY_ALL = 0xFFFFFFFF
};

struct mtk_jpeg_dev {
	struct mutex		lock;
	spinlock_t		hw_lock;
	struct workqueue_struct	*workqueue;
	struct device		*dev;
	struct v4l2_device	v4l2_dev;
	struct v4l2_m2m_dev	*m2m_dev;
	void			*alloc_ctx;
	struct video_device	*vfd_jpeg;
	void __iomem		*reg_base;
	struct clk		*clk_jpeg;
	struct clk		*clk_jpeg_smi;
	enum mtk_jpeg_mode  mode;
};

struct mtk_jpeg_fmt {
	u32	fourcc;
	int	h_sample[VIDEO_MAX_PLANES];
	int	v_sample[VIDEO_MAX_PLANES];
	int	colplanes;
	int	h_align;
	int	v_align;
	u32	flags;
};

struct mtk_jpeg_q_data {
	struct mtk_jpeg_fmt	*fmt;
	u32			w;
	u32			h;
	u32			bytesperline[VIDEO_MAX_PLANES];
	u32			sizeimage[VIDEO_MAX_PLANES];
};

struct jpeg_enc_param {
	u32 enable_exif;
	u32 enc_quality;
	u32 restart_interval;
};

struct mtk_jpeg_enc_param {
	u32 enc_w;
	u32 enc_h;
	u32 enable_exif;
	u32 enc_quality;
	u32 enc_format;
	u32 restart_interval;
	u32 img_stride;
	u32 mem_stride;
	u32 total_encdu;
};
struct mtk_jpeg_ctx {
	struct mtk_jpeg_dev		*jpeg;
	struct mtk_jpeg_q_data		out_q;
	struct mtk_jpeg_q_data		cap_q;
	struct v4l2_fh			fh;
	enum mtk_jpeg_ctx_state		state;
	struct jpeg_enc_param		jpeg_param;
	struct v4l2_ctrl_handler	ctrl_hdl;

	enum v4l2_colorspace colorspace;
	enum v4l2_ycbcr_encoding ycbcr_enc;
	enum v4l2_quantization quantization;
	enum v4l2_xfer_func xfer_func;
};

#endif /* _MTK_JPEG_CORE_H */
