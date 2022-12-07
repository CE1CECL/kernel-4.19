// SPDX-License-Identifier: GPL-2.0

#include <linux/file.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/sync_file.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <linux/atomic.h>

//#include "sync_debug.h"

static atomic_t fd_counter;
#define fd_max 500

#define CREATE_TRACE_POINTS
//#include "sync_trace.h"

struct mdp_sync_create_fence_data {
	__u32	value;
	char	name[32];
	__s32	fence; /* fd of new fence */
};

#define MDP_SYNC_IOC_MAGIC	'W'

#define MDP_SYNC_IOC_CREATE_FENCE	_IOWR(MDP_SYNC_IOC_MAGIC, 0,\
		struct mdp_sync_create_fence_data)

#define MDP_SYNC_IOC_INC	_IOW(MDP_SYNC_IOC_MAGIC, 1, __u32)

struct sync_timeline {
	struct kref		kref;
	char			name[32];

	/* protected by lock */
	u64			context;
	int			value;

	struct rb_root		pt_tree;
	struct list_head	pt_list;
	spinlock_t		lock;

	struct list_head	sync_timeline_list;
};

static inline struct sync_timeline *fence_parent(struct dma_fence *fence)
{
	return container_of(fence->lock, struct sync_timeline, lock);
}

struct sync_pt {
	struct dma_fence base;
	struct list_head link;
	struct rb_node node;
};

static const struct dma_fence_ops timeline_fence_ops;

static inline struct sync_pt *fence_to_sync_pt(struct dma_fence *fence)
{
	if (fence->ops != &timeline_fence_ops)
		return NULL;
	return container_of(fence, struct sync_pt, base);
}

struct sync_timeline *mdp_timeline_create(const char *name)
{
	struct sync_timeline *obj;

	obj = kzalloc(sizeof(*obj), GFP_KERNEL);
	if (!obj)
		return NULL;

	kref_init(&obj->kref);
	obj->context = dma_fence_context_alloc(1);
	strlcpy(obj->name, name, sizeof(obj->name));

	obj->pt_tree = RB_ROOT;
	INIT_LIST_HEAD(&obj->pt_list);
	spin_lock_init(&obj->lock);

	//sync_timeline_debug_add(obj);

	return obj;
}

static void sync_timeline_free(struct kref *kref)
{
	struct sync_timeline *obj =
		container_of(kref, struct sync_timeline, kref);
	//sync_timeline_debug_remove(obj);

	kfree(obj);
}

static void sync_timeline_get(struct sync_timeline *obj)
{
	kref_get(&obj->kref);
}

static void sync_timeline_put(struct sync_timeline *obj)
{
	kref_put(&obj->kref, sync_timeline_free);
}

static const char *timeline_fence_get_driver_name(struct dma_fence *fence)
{
	return "mdp_sync";
}

static const char *timeline_fence_get_timeline_name(struct dma_fence *fence)
{
	struct sync_timeline *parent = fence_parent(fence);

	return parent->name;
}

static void timeline_fence_release(struct dma_fence *fence)
{
	struct sync_pt *pt = fence_to_sync_pt(fence);
	struct sync_timeline *parent = fence_parent(fence);

	if (pt && !list_empty(&pt->link)) {
		unsigned long flags;

		spin_lock_irqsave(fence->lock, flags);
		if (!list_empty(&pt->link)) {
			list_del(&pt->link);
			rb_erase(&pt->node, &parent->pt_tree);
		}
		spin_unlock_irqrestore(fence->lock, flags);
	}

	sync_timeline_put(parent);
	dma_fence_free(fence);
	atomic_dec(&fd_counter);
}

static bool timeline_fence_signaled(struct dma_fence *fence)
{
	struct sync_timeline *parent = fence_parent(fence);

	return !__dma_fence_is_later(fence->seqno, parent->value);
}

static bool timeline_fence_enable_signaling(struct dma_fence *fence)
{
	return true;
}

static void timeline_fence_value_str(struct dma_fence *fence,
				    char *str, int size)
{
	snprintf(str, size, "%d", fence->seqno);
}

static void timeline_fence_timeline_value_str(struct dma_fence *fence,
					     char *str, int size)
{
	struct sync_timeline *parent = fence_parent(fence);

	snprintf(str, size, "%d", parent->value);
}

static const struct dma_fence_ops timeline_fence_ops = {
	.get_driver_name = timeline_fence_get_driver_name,
	.get_timeline_name = timeline_fence_get_timeline_name,
	.enable_signaling = timeline_fence_enable_signaling,
	.signaled = timeline_fence_signaled,
	.wait = dma_fence_default_wait,
	.release = timeline_fence_release,
	.fence_value_str = timeline_fence_value_str,
	.timeline_value_str = timeline_fence_timeline_value_str,
};

static void sync_timeline_signal(struct sync_timeline *obj, unsigned int inc)
{
	struct sync_pt *pt, *next;

	//trace_sync_timeline(obj);

	spin_lock_irq(&obj->lock);

	obj->value += inc;

	list_for_each_entry_safe(pt, next, &obj->pt_list, link) {
		if (!timeline_fence_signaled(&pt->base))
			break;

		list_del_init(&pt->link);
		rb_erase(&pt->node, &obj->pt_tree);

		/*
		 * A signal callback may release the last reference to this
		 * fence, causing it to be freed. That operation has to be
		 * last to avoid a use after free inside this loop, and must
		 * be after we remove the fence from the timeline in order to
		 * prevent deadlocking on timeline->lock inside
		 * timeline_fence_release().
		 */
		dma_fence_signal_locked(&pt->base);
	}

	spin_unlock_irq(&obj->lock);
}

static struct sync_pt *sync_pt_create(struct sync_timeline *obj,
					  unsigned int value)
{
	struct sync_pt *pt;

	pt = kzalloc(sizeof(*pt), GFP_KERNEL);
	if (!pt)
		return ERR_PTR(-ENOMEM);

	sync_timeline_get(obj);
	dma_fence_init(&pt->base, &timeline_fence_ops, &obj->lock,
		   obj->context, value);

	atomic_inc(&fd_counter);

	INIT_LIST_HEAD(&pt->link);

	spin_lock_irq(&obj->lock);
	if (!dma_fence_is_signaled_locked(&pt->base)) {
		struct rb_node **p = &obj->pt_tree.rb_node;
		struct rb_node *parent = NULL;

		while (*p) {
			struct sync_pt *other;
			int cmp;

			parent = *p;
			other = rb_entry(parent, typeof(*pt), node);
			cmp = value - other->base.seqno;
			if (cmp > 0) {
				p = &parent->rb_right;
			} else if (cmp < 0) {
				p = &parent->rb_left;
			} else {
				if (dma_fence_get_rcu(&other->base)) {
					dma_fence_put(&pt->base);
					pt = ERR_PTR(-EEXIST);
					goto unlock;
				}
				p = &parent->rb_left;
			}
		}
		rb_link_node(&pt->node, parent, p);
		rb_insert_color(&pt->node, &obj->pt_tree);

		parent = rb_next(&pt->node);
		list_add_tail(&pt->link,
			      parent ? &rb_entry(parent, typeof(*pt),
				node)->link : &obj->pt_list);
	}
unlock:
	spin_unlock_irq(&obj->lock);

	return pt;
}


/* opening mdp_sync create a new sync obj */
static int mdp_sync_open(struct inode *inode, struct file *file)
{
	struct sync_timeline *obj;
	char task_comm[TASK_COMM_LEN];

	get_task_comm(task_comm, current);

	obj = mdp_timeline_create(task_comm);
	if (!obj)
		return -ENOMEM;

	file->private_data = obj;

	return 0;
}

static int mdp_sync_release(struct inode *inode, struct file *file)
{
	struct sync_timeline *obj = file->private_data;

	smp_wmb();/*memory barrier*/

	sync_timeline_put(obj);
	return 0;
}

static long mdp_sync_ioctl_create_fence(struct sync_timeline *obj,
				       unsigned long arg)
{
	int fd = 0;
	int err;
	struct sync_pt *pt;
	struct sync_file *sync_file;
	struct mdp_sync_create_fence_data data;

	if (atomic_read(&fd_counter) > fd_max)
		return -EFAULT;

	fd = get_unused_fd_flags(O_CLOEXEC);

	if (fd < 0)
		return fd;

	if (copy_from_user(&data, (void __user *)arg, sizeof(data))) {
		err = -EFAULT;
		goto err;
	}

	pt = sync_pt_create(obj, data.value);
	if (IS_ERR(pt)) {
		err = PTR_ERR(pt);
		goto err;
	}

	sync_file = sync_file_create(&pt->base);
	dma_fence_put(&pt->base);
	if (!sync_file) {
		err = -ENOMEM;
		goto err;
	}

	data.fence = fd;
	if (copy_to_user((void __user *)arg, &data, sizeof(data))) {
		fput(sync_file->file);
		err = -EFAULT;
		goto err;
	}

	fd_install(fd, sync_file->file);
	return 0;

err:
	put_unused_fd(fd);
	return err;
}

static long mdp_sync_ioctl_inc(struct sync_timeline *obj, unsigned long arg)
{
	u32 value;

	if (copy_from_user(&value, (void __user *)arg, sizeof(value)))
		return -EFAULT;

	while (value > INT_MAX)  {
		sync_timeline_signal(obj, INT_MAX);
		value -= INT_MAX;
	}

	sync_timeline_signal(obj, value);
	return 0;
}

static long mdp_sync_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	struct sync_timeline *obj = file->private_data;

	switch (cmd) {
	case MDP_SYNC_IOC_CREATE_FENCE:
		return mdp_sync_ioctl_create_fence(obj, arg);

	case MDP_SYNC_IOC_INC:
		return mdp_sync_ioctl_inc(obj, arg);

	default:
		return -ENOTTY;
	}
}

const struct file_operations mdp_sync_fops = {
	.open           = mdp_sync_open,
	.release        = mdp_sync_release,
	.unlocked_ioctl = mdp_sync_ioctl,
	.compat_ioctl	= mdp_sync_ioctl,
};


static struct miscdevice mdp_sync_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "mdp_sync",
	.fops	= &mdp_sync_fops,
};

static int __init mdp_sync_device_init(void)
{
	return misc_register(&mdp_sync_dev);
}
device_initcall(mdp_sync_device_init);

