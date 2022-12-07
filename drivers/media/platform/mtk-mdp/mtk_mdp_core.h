
#ifndef __MTK_MDP_CORE_H__
#define __MTK_MDP_CORE_H__

#include <linux/videodev2.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-mem2mem.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-dma-contig.h>

#include "mtk_mdp_vpu.h"
#include "mtk_mdp_comp.h"


#define MTK_MDP_MODULE_NAME		"mtk-mdp"

#define MTK_MDP_SHUTDOWN_TIMEOUT	((100*HZ)/1000) /* 100ms */
#define MTK_MDP_MAX_CTRL_NUM		10

#define MTK_MDP_FMT_FLAG_OUTPUT		BIT(0)
#define MTK_MDP_FMT_FLAG_CAPTURE	BIT(1)

#define MTK_MDP_VPU_INIT		BIT(0)
#define MTK_MDP_SRC_FMT			BIT(1)
#define MTK_MDP_DST_FMT			BIT(2)
#define MTK_MDP_CTX_ERROR		BIT(5)

struct mtk_mdp_pix_align {
	u16 org_w;
	u16 org_h;
	u16 target_w;
	u16 target_h;
};

struct mtk_mdp_fmt {
	u32	pixelformat;
	u16	num_planes;
	u16	num_comp;
	u8	depth[VIDEO_MAX_PLANES];
	u8	row_depth[VIDEO_MAX_PLANES];
	u32	flags;
	struct mtk_mdp_pix_align *align;
};

struct mtk_mdp_addr {
	dma_addr_t addr[MTK_MDP_MAX_NUM_PLANE];
};

struct mtk_mdp_ctrls {
	struct v4l2_ctrl *rotate;
	struct v4l2_ctrl *hflip;
	struct v4l2_ctrl *vflip;
	struct v4l2_ctrl *global_alpha;
};

struct mtk_mdp_frame {
	u32				width;
	u32				height;
	struct v4l2_rect		crop;
	unsigned long			payload[VIDEO_MAX_PLANES];
	unsigned int			pitch[VIDEO_MAX_PLANES];
	struct mtk_mdp_addr		addr;
	const struct mtk_mdp_fmt	*fmt;
	u8				alpha;
};

struct mtk_mdp_variant {
	struct mtk_mdp_pix_limit	*pix_max;
	struct mtk_mdp_pix_limit	*pix_min;
	struct mtk_mdp_pix_align	*pix_align;
	u16				h_scale_up_max;
	u16				v_scale_up_max;
	u16				h_scale_down_max;
	u16				v_scale_down_max;
};

struct mtk_mdp_dev {
	struct mutex			lock;
	struct mutex			vpulock;
	struct platform_device		*pdev;
	struct mtk_mdp_variant		*variant;
	u16				id;
	struct mtk_mdp_comp		*comp[MTK_MDP_COMP_ID_MAX];
	struct v4l2_m2m_dev		*m2m_dev;
	struct list_head		ctx_list;
	struct video_device		*vdev;
	struct v4l2_device		v4l2_dev;
	struct workqueue_struct		*job_wq;
	struct platform_device		*vpu_dev;
	int				ctx_num;
	unsigned long			id_counter;
	struct workqueue_struct		*wdt_wq;
	struct work_struct		wdt_work;
};

struct mtk_mdp_ctx {
	struct list_head		list;
	struct mtk_mdp_frame		s_frame;
	struct mtk_mdp_frame		d_frame;
	u32				flags;
	u32				state;
	int				id;
	int				rotation;
	u32				hflip:1;
	u32				vflip:1;
	struct mtk_mdp_dev		*mdp_dev;
	struct v4l2_m2m_ctx		*m2m_ctx;
	struct v4l2_fh			fh;
	struct v4l2_ctrl_handler	ctrl_handler;
	struct mtk_mdp_ctrls		ctrls;
	bool				ctrls_rdy;
	enum v4l2_colorspace		colorspace;
	enum v4l2_ycbcr_encoding	ycbcr_enc;
	enum v4l2_xfer_func		xfer_func;
	enum v4l2_quantization		quant;

	struct mtk_mdp_vpu		vpu;
	struct mutex			slock;
	struct work_struct		work;
};

extern int mtk_mdp_dbg_level;

#if defined(DEBUG)

#define mtk_mdp_dbg(level, fmt, args...)				 \
	do {								 \
		if (mtk_mdp_dbg_level >= level)				 \
			pr_info("[MTK_MDP] level=%d %s(),%d: " fmt "\n", \
				level, __func__, __LINE__, ##args);	 \
	} while (0)

#define mtk_mdp_err(fmt, args...)					\
	pr_err("[MTK_MDP][ERROR] %s:%d: " fmt "\n", __func__, __LINE__, \
	       ##args)


#define mtk_mdp_dbg_enter()  mtk_mdp_dbg(3, "+")
#define mtk_mdp_dbg_leave()  mtk_mdp_dbg(3, "-")

#else

#define mtk_mdp_dbg(level, fmt, args...) {}
#define mtk_mdp_err(fmt, args...)
#define mtk_mdp_dbg_enter()
#define mtk_mdp_dbg_leave()

#endif

#endif /* __MTK_MDP_CORE_H__ */
