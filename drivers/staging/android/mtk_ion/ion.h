/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_ION_H
#define _LINUX_ION_H

#include <linux/types.h>

#include "../uapi/mtk_ion/ion.h"

struct ion_handle;
struct ion_device;
struct ion_heap;
struct ion_mapper;
struct ion_client;
struct ion_buffer;

#define ion_phys_addr_t unsigned long

struct ion_platform_heap {
	enum ion_heap_type type;
	unsigned int id;
	const char *name;
	ion_phys_addr_t base;
	size_t size;
	ion_phys_addr_t align;
	void *priv;
};

struct ion_platform_data {
	int nr;
	struct ion_platform_heap *heaps;
};

struct ion_client *ion_client_create(struct ion_device *dev,
				     const char *name);

void ion_client_destroy(struct ion_client *client);

struct ion_handle *ion_alloc(struct ion_client *client, size_t len,
			     size_t align, unsigned int heap_id_mask,
			     unsigned int flags);

void ion_free(struct ion_client *client, struct ion_handle *handle);

void *ion_map_kernel(struct ion_client *client, struct ion_handle *handle);

void ion_unmap_kernel(struct ion_client *client, struct ion_handle *handle);

struct dma_buf *
ion_share_dma_buf(struct ion_client *client, struct ion_handle *handle);

int ion_share_dma_buf_fd(struct ion_client *client, struct ion_handle *handle);

struct ion_handle *ion_import_dma_buf(struct ion_client *client,
				      struct dma_buf *dmabuf);

struct ion_handle *ion_import_dma_buf_fd(struct ion_client *client, int fd);

int ion_phys(struct ion_client *client, struct ion_handle *handle,
	     ion_phys_addr_t *addr, size_t *len);

#endif /* _LINUX_ION_H */
