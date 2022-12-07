/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MTK_VCODEC_DEC_H_
#define _MTK_VCODEC_DEC_H_

#include <media/videobuf2-core.h>
#include <media/videobuf2-v4l2.h>

#define VCODEC_CAPABILITY_4K_DISABLED   0x10
#define VCODEC_DEC_4K_CODED_WIDTH       4096U
#define VCODEC_DEC_4K_CODED_HEIGHT      2304U
#define MTK_VDEC_MAX_W  2048U
#define MTK_VDEC_MAX_H  1088U

struct vdec_fb {
	struct mtk_vcodec_mem   fb_base[VIDEO_MAX_PLANES];
	unsigned int    status;
	unsigned int    num_planes;
	long long timestamp;
	unsigned int    index;
	int             general_buf_fd;
	struct  dma_buf *dma_general_buf;
	dma_addr_t dma_general_addr;
};

enum eos_types {
	NON_EOS = 0,
	EOS_WITH_DATA,
	EOS
};

struct mtk_video_dec_buf {
	struct vb2_v4l2_buffer  vb;
	struct list_head        list;

	bool    used;
	bool    ready_to_display;
	bool    queued_in_vb2;
	bool    queued_in_v4l2;
	enum eos_types lastframe;
	struct mtk_vcodec_mem bs_buffer;
	struct vdec_fb  frame_buffer;
	int     flags;
	int     general_user_fd;
};

extern const struct v4l2_ioctl_ops mtk_vdec_ioctl_ops;
extern const struct v4l2_m2m_ops mtk_vdec_m2m_ops;


void mtk_vdec_unlock(struct mtk_vcodec_ctx *ctx, u32 hw_id);
int mtk_vdec_lock(struct mtk_vcodec_ctx *ctx, u32 hw_id);
int mtk_vcodec_dec_queue_init(void *priv, struct vb2_queue *src_vq,
	struct vb2_queue *dst_vq);
void mtk_vcodec_dec_set_default_params(struct mtk_vcodec_ctx *ctx);
void mtk_vcodec_dec_empty_queues(struct file *file, struct mtk_vcodec_ctx *ctx);
void mtk_vcodec_dec_release(struct mtk_vcodec_ctx *ctx);
int mtk_vcodec_dec_ctrls_setup(struct mtk_vcodec_ctx *ctx);


#endif /* _MTK_VCODEC_DEC_H_ */
