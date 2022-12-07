/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _VDEC_VPU_IF_H_
#define _VDEC_VPU_IF_H_

#include "mtk_vpu.h"

struct vdec_vpu_inst {
	enum ipi_id id;
	void *vsi;
	int32_t failure;
	uint32_t inst_addr;
	unsigned int signaled;
	struct mtk_vcodec_ctx *ctx;
	struct platform_device *dev;
	wait_queue_head_t wq;
	ipi_handler_t handler;
};

int vpu_dec_init(struct vdec_vpu_inst *vpu);

int vpu_dec_start(struct vdec_vpu_inst *vpu, uint32_t *data, unsigned int len);

int vpu_dec_end(struct vdec_vpu_inst *vpu);

int vpu_dec_deinit(struct vdec_vpu_inst *vpu);

int vpu_dec_reset(struct vdec_vpu_inst *vpu);

void vpu_dec_ipi_handler(void *data, unsigned int len, void *priv);

#endif
