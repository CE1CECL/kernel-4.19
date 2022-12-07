/* SPDX-License-Identifier: GPL-2.0 */

#ifndef MTK_VCODEC_MEM_H
#define MTK_VCODEC_MEM_H

#include <media/videobuf2-dma-contig.h>
#include <uapi/linux/mtk_vcu_controls.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/soc/mediatek/mtk-cmdq.h>

#ifndef CONFIG_ARM64
#include "mm/dma.h"
#endif

#ifndef dmac_map_area
#define dmac_map_area __dma_map_area
#endif
#ifndef dmac_unmap_area
#define dmac_unmap_area __dma_unmap_area
#endif
#ifndef dmac_flush_range
#define dmac_flush_range __dma_flush_range
#endif

#define CODEC_MAX_BUFFER 512U
#define CODEC_ALLOCATE_MAX_BUFFER_SIZE 0x8000000UL /*128MB*/
#define CODEC_MSK(addr) ((addr >> PAGE_SHIFT) & 0xFFFF)

struct mtk_vcu_mem {
	void *mem_priv;
	size_t size;
	struct dma_buf *dbuf;
	dma_addr_t iova;
	atomic_t ref_cnt;
};

struct vcu_pa_pages {
	unsigned long pa;
	unsigned long kva;
	atomic_t ref_cnt;
	struct list_head list;
};

struct mtk_vcu_queue {
	void *vcu;
	struct mutex mmap_lock;
	struct device *dev;
	struct device *cmdq_dev;
	unsigned int num_buffers;
	const struct vb2_mem_ops *mem_ops;
	struct mtk_vcu_mem bufs[CODEC_MAX_BUFFER];
	uint64_t map_buf_pa;
	struct vcu_pa_pages pa_pages;
};

struct mtk_vcu_queue *mtk_vcu_mem_init(struct device *dev,
	struct device *cmdq_dev);

void mtk_vcu_mem_release(struct mtk_vcu_queue *vcu_queue);

void *mtk_vcu_set_buffer(struct mtk_vcu_queue *vcu_queue,
	struct mem_obj *mem_buff_data, struct vb2_buffer *src_vb,
	struct vb2_buffer *dst_vb);

void *mtk_vcu_get_buffer(struct mtk_vcu_queue *vcu_queue,
						 struct mem_obj *mem_buff_data);
void *mtk_vcu_get_page(struct mtk_vcu_queue *vcu_queue,
						 struct mem_obj *mem_buff_data);

int mtk_vcu_free_buffer(struct mtk_vcu_queue *vcu_queue,
						struct mem_obj *mem_buff_data);
int mtk_vcu_free_page(struct mtk_vcu_queue *vcu_queue,
						struct mem_obj *mem_buff_data);

void mtk_vcu_buffer_ref_dec(struct mtk_vcu_queue *vcu_queue,
	void *mem_priv);

int vcu_buffer_flush_all(struct device *dev, struct mtk_vcu_queue *vcu_queue);

int vcu_buffer_cache_sync(struct device *dev, struct mtk_vcu_queue *vcu_queue,
	dma_addr_t dma_addr, size_t size, int op);

#endif

