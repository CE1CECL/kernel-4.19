/* SPDX-License-Identifier: GPL-2.0 */
/*
 * The owner field of the rw_semaphore structure will be set to
 * RWSEM_READER_OWNED when a reader grabs the lock. A writer will clear
 * the owner field when it unlocks. A reader, on the other hand, will
 * not touch the owner field when it unlocks.
 *
 * In essence, the owner field now has the following 4 states:
 *  1) 0
 *     - lock is free or the owner hasn't set the field yet
 *  2) RWSEM_READER_OWNED
 *     - lock is currently or previously owned by readers (lock is free
 *       or not set by owner yet)
 *  3) RWSEM_ANONYMOUSLY_OWNED bit set with some other bits set as well
 *     - lock is owned by an anonymous writer, so spinning on the lock
 *       owner should be disabled.
 *  4) Other non-zero value
 *     - a writer owns the lock and other writers can spin on the lock owner.
 */
#ifdef CONFIG_TCT_UI_TURBO
#include <linux/tct/uiturbo.h>
#endif
#define RWSEM_ANONYMOUSLY_OWNED	(1UL << 0)
#define RWSEM_READER_OWNED	((struct task_struct *)RWSEM_ANONYMOUSLY_OWNED)

#ifdef CONFIG_DEBUG_RWSEMS
# define DEBUG_RWSEMS_WARN_ON(c)	DEBUG_LOCKS_WARN_ON(c)
#else
# define DEBUG_RWSEMS_WARN_ON(c)
#endif

#ifdef CONFIG_RWSEM_SPIN_ON_OWNER

#ifdef CONFIG_TCT_UI_TURBO
enum rwsem_waiter_type {
	RWSEM_WAITING_FOR_WRITE,
	RWSEM_WAITING_FOR_READ
};

struct rwsem_waiter {
	struct list_head list;
	struct task_struct *task;
	enum rwsem_waiter_type type;
};
#endif

/*
 * All writes to owner are protected by WRITE_ONCE() to make sure that
 * store tearing can't happen as optimistic spinners may read and use
 * the owner value concurrently without lock. Read from owner, however,
 * may not need READ_ONCE() as long as the pointer value is only used
 * for comparison and isn't being dereferenced.
 */
static inline void rwsem_set_owner(struct rw_semaphore *sem)
{
	WRITE_ONCE(sem->owner, current);
}

static inline void rwsem_clear_owner(struct rw_semaphore *sem)
{
	WRITE_ONCE(sem->owner, NULL);
}

static inline void rwsem_set_reader_owned(struct rw_semaphore *sem)
{
	/*
	 * We check the owner value first to make sure that we will only
	 * do a write to the rwsem cacheline when it is really necessary
	 * to minimize cacheline contention.
	 */
	if (READ_ONCE(sem->owner) != RWSEM_READER_OWNED)
		WRITE_ONCE(sem->owner, RWSEM_READER_OWNED);
}

/*
 * Return true if the a rwsem waiter can spin on the rwsem's owner
 * and steal the lock, i.e. the lock is not anonymously owned.
 * N.B. !owner is considered spinnable.
 */
static inline bool is_rwsem_owner_spinnable(struct task_struct *owner)
{
	return !((unsigned long)owner & RWSEM_ANONYMOUSLY_OWNED);
}

/*
 * Return true if rwsem is owned by an anonymous writer or readers.
 */
static inline bool rwsem_has_anonymous_owner(struct task_struct *owner)
{
	return (unsigned long)owner & RWSEM_ANONYMOUSLY_OWNED;
}
#else
static inline void rwsem_set_owner(struct rw_semaphore *sem)
{
}

static inline void rwsem_clear_owner(struct rw_semaphore *sem)
{
}

static inline void rwsem_set_reader_owned(struct rw_semaphore *sem)
{
}
#endif

#ifdef CONFIG_TCT_UI_TURBO
static inline bool rwsem_owner_is_writer(struct task_struct *owner)
{
	return owner && owner != RWSEM_READER_OWNED;
}

static inline void
rwsem_uiturbo_list_add(struct task_struct *p,
		       struct list_head *entry,
		       struct list_head *head)
{
	if (unlikely(!entry || !head)) {
		return;
	}
	if (test_task_uiturbo(p)) {
		struct list_head *pos;
		struct rwsem_waiter *waiter;
		list_for_each(pos, head) {
			waiter = list_entry(pos, struct rwsem_waiter, list);
			if (!test_task_uiturbo(waiter->task)) {
				list_add(entry, waiter->list.prev);
				return;
			}
		}
	}
	list_add_tail(entry, head);
}

static inline void
rwsem_dynamic_uiturbo_enqueue(struct task_struct *tsk,
			      struct task_struct *waiter_task,
			      struct task_struct *owner,
			      struct rw_semaphore *sem)
{
	if (waiter_task && test_set_dynamic_uiturbo(tsk) &&
	    rwsem_owner_is_writer(owner) && !test_task_uiturbo(owner) &&
	    sem && !sem->ui_dep_task) {
		sem->ui_dep_task = owner;
		dynamic_uiturbo_enqueue(owner, DYNAMIC_UITURBO_RWSEM,
					tsk->uiturbo_depth);
	}
}

static inline void
rwsem_dynamic_uiturbo_dequeue(struct rw_semaphore *sem,
			      struct task_struct *tsk)
{
	if (tsk && sem && sem->ui_dep_task == tsk) {
		dynamic_uiturbo_dequeue(tsk, DYNAMIC_UITURBO_RWSEM);
		sem->ui_dep_task = NULL;
	}
}
#endif
