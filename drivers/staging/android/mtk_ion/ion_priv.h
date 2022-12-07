/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _ION_PRIV_H
#define _ION_PRIV_H

#include <linux/device.h>
#include <linux/dma-direction.h>
#include <linux/kref.h>
#include <linux/mm_types.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/shrinker.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include "mtk/ion_drv.h"

#include "ion.h"

#ifdef CONFIG_MTK_IOMMU_V2
#define MTK_ION_DMABUF_SUPPORT
#endif

struct ion_buffer *ion_handle_buffer(struct ion_handle *handle);

struct ion_buffer {
	struct kref ref;
	union {
		struct rb_node node;
		struct list_head list;
	};
	struct ion_device *dev;
	struct ion_heap *heap;
	unsigned long flags;
	unsigned long private_flags;
	size_t size;
	void *priv_virt;
	struct mutex lock; /* mutex */
	int kmap_cnt;
	void *vaddr;
	int dmap_cnt;
	struct sg_table *sg_table;
	struct page **pages;
	struct list_head vmas;
	/* used to track orphaned buffers */
	int handle_count;
	char task_comm[TASK_COMM_LEN];
	pid_t pid;
	char alloc_dbg[ION_MM_DBG_NAME_LEN];
#ifdef MTK_ION_DMABUF_SUPPORT
	struct list_head attachments;
#endif
};

void ion_buffer_destroy(struct ion_buffer *buffer);

struct ion_device {
	struct miscdevice dev;
	struct rb_root buffers;
	struct mutex buffer_lock; /* mutex */
	struct rw_semaphore lock;
	struct plist_head heaps;
	long (*custom_ioctl)(struct ion_client *client, unsigned int cmd,
			     unsigned long arg);
	struct rb_root clients;
#if IS_ENABLED(CONFIG_DEBUG_FS)
	struct dentry *debug_root;
	struct dentry *heaps_debug_root;
	struct dentry *clients_debug_root;
#endif
#if IS_ENABLED(CONFIG_PROC_FS)
	struct proc_dir_entry *proc_root;
	struct proc_dir_entry *heaps_proc_root;
	struct proc_dir_entry *clients_proc_root;
#endif
	int heap_cnt;
};

struct ion_client {
	struct rb_node node;
	struct ion_device *dev;
	struct rb_root handles;
	struct idr idr;
	struct mutex lock; /* mutex */
	const char *name;
	char *display_name;
	int display_serial;
	struct task_struct *task;
	pid_t pid;
#if IS_ENABLED(CONFIG_DEBUG_FS)
	struct dentry *debug_root;
#endif
#if IS_ENABLED(CONFIG_PROC_FS)
	struct proc_dir_entry *proc_root;
#endif
	char dbg_name[ION_MM_DBG_NAME_LEN]; /* add by K for debug! */
};

struct ion_handle_debug {
	int fd;
	unsigned long long user_ts; /* alloc or import timestamp */
};

struct ion_handle {
	struct kref ref;
	unsigned int user_ref_count;
	struct ion_client *client;
	struct ion_buffer *buffer;
	struct rb_node node;
	unsigned int kmap_cnt;
	int id;
	struct ion_handle_debug dbg; /*add by K for debug */
};

struct ion_heap_ops {
	int (*allocate)(struct ion_heap *heap,
			struct ion_buffer *buffer, unsigned long len,
			unsigned long align, unsigned long flags);
	void (*free)(struct ion_buffer *buffer);
	void * (*map_kernel)(struct ion_heap *heap, struct ion_buffer *buffer);
	void (*unmap_kernel)(struct ion_heap *heap, struct ion_buffer *buffer);
	int (*map_user)(struct ion_heap *mapper, struct ion_buffer *buffer,
			struct vm_area_struct *vma);
	int (*shrink)(struct ion_heap *heap, gfp_t gfp_mask, int nr_to_scan);
	int (*phys)(struct ion_heap *heap, struct ion_buffer *buffer,
		    ion_phys_addr_t *addr, size_t *len);
	int (*page_pool_total)(struct ion_heap *heap);
#if defined(CONFIG_MTK_IOMMU_PGTABLE_EXT) && \
	(CONFIG_MTK_IOMMU_PGTABLE_EXT > 32)
	void (*get_table)(struct ion_buffer *buffer, struct sg_table *table);
#endif
#ifdef MTK_ION_DMABUF_SUPPORT
	/* For user with dma-buf standard flow to get iova, we can get port
	 * id from struct device*, users no need to do config buffer.
	 */
	int (*dma_buf_config)(struct ion_buffer *buffer, struct device *dev);
#endif
};

#define ION_HEAP_FLAG_DEFER_FREE BIT(0)

#define ION_PRIV_FLAG_SHRINKER_FREE BIT(0)

struct ion_heap {
	struct plist_node node;
	struct ion_device *dev;
	enum ion_heap_type type;
	struct ion_heap_ops *ops;
	unsigned long flags;
	unsigned int id;
	const char *name;
	struct shrinker shrinker;
	struct list_head free_list;
	size_t free_list_size;
	spinlock_t free_lock; /* spin lock */
	wait_queue_head_t waitqueue;
	struct task_struct *task;

	int (*debug_show)(struct ion_heap *heap, struct seq_file *s, void *p);
	atomic_long_t total_allocated;
};

bool ion_buffer_cached(struct ion_buffer *buffer);

bool ion_buffer_fault_user_mappings(struct ion_buffer *buffer);

struct ion_device *ion_device_create(long (*custom_ioctl)
				     (struct ion_client *client,
				      unsigned int cmd,
				      unsigned long arg));

void ion_device_destroy(struct ion_device *dev);

void ion_device_add_heap(struct ion_device *dev, struct ion_heap *heap);

void *ion_heap_map_kernel(struct ion_heap *heap,
			  struct ion_buffer *buffer);
void ion_heap_unmap_kernel(struct ion_heap *heap,
			   struct ion_buffer *buffer);
int ion_heap_map_user(struct ion_heap *heap, struct ion_buffer *buffer,
		      struct vm_area_struct *vma);

int ion_heap_buffer_zero(struct ion_buffer *buffer);
int ion_heap_pages_zero(struct page *page, size_t size, pgprot_t pgprot);

void ion_heap_init_shrinker(struct ion_heap *heap);

int ion_heap_init_deferred_free(struct ion_heap *heap);

void ion_heap_freelist_add(struct ion_heap *heap, struct ion_buffer *buffer);

size_t ion_heap_freelist_drain(struct ion_heap *heap, size_t size);

size_t ion_heap_freelist_shrink(struct ion_heap *heap, size_t size);

size_t ion_heap_freelist_size(struct ion_heap *heap);


struct ion_heap *ion_heap_create(struct ion_platform_heap *heap_data);
void ion_heap_destroy(struct ion_heap *heap);
struct ion_heap *ion_system_heap_create(struct ion_platform_heap *unused);
void ion_system_heap_destroy(struct ion_heap *heap);

struct ion_heap *
ion_system_contig_heap_create(struct ion_platform_heap *unused);
void ion_system_contig_heap_destroy(struct ion_heap *heap);

struct ion_heap *ion_carveout_heap_create(struct ion_platform_heap *heap_data);
void ion_carveout_heap_destroy(struct ion_heap *heap);

struct ion_heap *ion_chunk_heap_create(struct ion_platform_heap *heap_data);
void ion_chunk_heap_destroy(struct ion_heap *heap);
struct ion_heap *ion_cma_heap_create(struct ion_platform_heap *data);
void ion_cma_heap_destroy(struct ion_heap *heap);


struct ion_page_pool {
	int high_count;
	int low_count;
	int cached; /* avoid use bool in struct */
	struct list_head high_items;
	struct list_head low_items;
	struct mutex mutex; /* mutex */
	gfp_t gfp_mask;
	unsigned int order;
	struct plist_node list;
};

struct ion_page_pool *ion_page_pool_create(gfp_t gfp_mask, unsigned int order,
					   bool cached);
void ion_page_pool_destroy(struct ion_page_pool *pool);
struct page *ion_page_pool_alloc(struct ion_page_pool *pool);
void ion_page_pool_free(struct ion_page_pool *pool, struct page *page);
long ion_page_pool_nr_pages(void);

int ion_page_pool_shrink(struct ion_page_pool *pool, gfp_t gfp_mask,
			 int nr_to_scan);

void ion_pages_sync_for_device(struct device *dev, struct page *page,
			       size_t size, enum dma_data_direction dir);

long ion_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

int ion_sync_for_device(struct ion_client *client, int fd);

struct ion_handle *ion_handle_get_by_id_nolock(struct ion_client *client,
					       int id);

void ion_free_nolock(struct ion_client *client, struct ion_handle *handle);

int ion_handle_put_nolock(struct ion_handle *handle);

int ion_handle_put(struct ion_handle *handle);

int ion_query_heaps(struct ion_client *client, struct ion_heap_query *query);

int clone_sg_table(const struct sg_table *source, struct sg_table *dest);

extern struct ion_device *g_ion_device;
#ifdef CONFIG_MTK_IOMMU_V2
extern struct device *g_iommu_device;
#endif

extern atomic64_t page_sz_cnt;

int ion_share_dma_buf_fd_nolock(struct ion_client *client,
				struct ion_handle *handle);

struct ion_handle *pass_to_user(struct ion_handle *handle);
void user_ion_free_nolock(struct ion_client *client, struct ion_handle *handle);

struct ion_handle *__ion_alloc(struct ion_client *client, size_t len,
			       size_t align, unsigned int heap_id_mask,
			       unsigned int flags, bool grab_handle);

#endif /* _ION_PRIV_H */
