/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _VDEC_VCU_IF_H_
#define _VDEC_VCU_IF_H_

#include <linux/dma-buf.h>
#include "mtk_vcu.h"

struct vdec_vcu_inst {
	enum ipi_id id;
	void *vsi;
	int32_t failure;
	uint32_t inst_addr;
	unsigned int signaled;
	struct mtk_vcodec_ctx *ctx;
	struct platform_device *dev;
	wait_queue_head_t wq;
	ipi_handler_t handler;
	bool abort;
	int daemon_pid;
	struct mutex *ctx_ipi_binding;
};

int vcu_dec_init(struct vdec_vcu_inst *vcu);

int vcu_dec_start(struct vdec_vcu_inst *vcu,
	uint32_t *data, unsigned int len,
	struct mtk_vcodec_mem *bs, struct vdec_fb *fb);

int vcu_dec_end(struct vdec_vcu_inst *vcu);

int vcu_dec_deinit(struct vdec_vcu_inst *vcu);

int vcu_dec_reset(struct vdec_vcu_inst *vcu);

int vcu_dec_ipi_handler(void *data, unsigned int len, void *priv);
int vcu_dec_query_cap(struct vdec_vcu_inst *vcu, unsigned int id, void *out);
int vcu_dec_set_param(struct vdec_vcu_inst *vcu, unsigned int id,
					  void *param, unsigned int size);
int get_mapped_fd(struct dma_buf *dmabuf);
void close_mapped_fd(unsigned int target_fd);

#endif
