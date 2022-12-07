/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __VPU_POOL_H__
#define __VPU_POOL_H__

#include <linux/types.h>
#include <linux/mutex.h>
#include "vpu_drv.h"

enum vpu_pool_type {
	VPU_POOL,
	VPU_POOL_DEP,
};


struct vpu_pool {
	char name[16];
	struct mutex lock;
	struct list_head pool;
	int type;
	int size;
	void *priv;
};

struct vpu_pool_dep {
	struct list_head link;     /* link in the 1st level pool */
	struct list_head sub_pool; /* head of the sub-pool */
	unsigned long *user;
	uint64_t head_id;
	uint64_t tail_id;
};

void vpu_pool_init(struct vpu_pool *p, const char *name, int type);

int vpu_pool_size(struct vpu_pool *p);

int vpu_pool_is_empty(struct vpu_pool *p);

int vpu_pool_enqueue(struct vpu_pool *p, struct vpu_request *req,
	unsigned int *priority);

struct vpu_request *vpu_pool_dequeue(struct vpu_pool *p,
	unsigned int *priority);

#endif

