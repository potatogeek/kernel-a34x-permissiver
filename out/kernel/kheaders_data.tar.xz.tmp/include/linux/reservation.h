
#ifndef _LINUX_RESERVATION_H
#define _LINUX_RESERVATION_H

#include <linux/ww_mutex.h>
#include <linux/dma-fence.h>
#include <linux/slab.h>
#include <linux/seqlock.h>
#include <linux/rcupdate.h>

extern struct ww_class reservation_ww_class;
extern struct lock_class_key reservation_seqcount_class;
extern const char reservation_seqcount_string[];


struct reservation_object_list {
	struct rcu_head rcu;
	u32 shared_count, shared_max;
	struct dma_fence __rcu *shared[];
};


struct reservation_object {
	struct ww_mutex lock;
	seqcount_t seq;

	struct dma_fence __rcu *fence_excl;
	struct reservation_object_list __rcu *fence;
	struct reservation_object_list *staged;
};

#define reservation_object_held(obj) lockdep_is_held(&(obj)->lock.base)
#define reservation_object_assert_held(obj) \
	lockdep_assert_held(&(obj)->lock.base)


static inline void
reservation_object_init(struct reservation_object *obj)
{
	ww_mutex_init(&obj->lock, &reservation_ww_class);

	__seqcount_init(&obj->seq, reservation_seqcount_string, &reservation_seqcount_class);
	RCU_INIT_POINTER(obj->fence, NULL);
	RCU_INIT_POINTER(obj->fence_excl, NULL);
	obj->staged = NULL;
}


static inline void
reservation_object_fini(struct reservation_object *obj)
{
	int i;
	struct reservation_object_list *fobj;
	struct dma_fence *excl;

	
	excl = rcu_dereference_protected(obj->fence_excl, 1);
	if (excl)
		dma_fence_put(excl);

	fobj = rcu_dereference_protected(obj->fence, 1);
	if (fobj) {
		for (i = 0; i < fobj->shared_count; ++i)
			dma_fence_put(rcu_dereference_protected(fobj->shared[i], 1));

		kfree(fobj);
	}
	kfree(obj->staged);

	ww_mutex_destroy(&obj->lock);
}


static inline struct reservation_object_list *
reservation_object_get_list(struct reservation_object *obj)
{
	return rcu_dereference_protected(obj->fence,
					 reservation_object_held(obj));
}


static inline int
reservation_object_lock(struct reservation_object *obj,
			struct ww_acquire_ctx *ctx)
{
	return ww_mutex_lock(&obj->lock, ctx);
}


static inline int
reservation_object_lock_interruptible(struct reservation_object *obj,
				      struct ww_acquire_ctx *ctx)
{
	return ww_mutex_lock_interruptible(&obj->lock, ctx);
}



static inline bool __must_check
reservation_object_trylock(struct reservation_object *obj)
{
	return ww_mutex_trylock(&obj->lock);
}


static inline void
reservation_object_unlock(struct reservation_object *obj)
{
	ww_mutex_unlock(&obj->lock);
}


static inline struct dma_fence *
reservation_object_get_excl(struct reservation_object *obj)
{
	return rcu_dereference_protected(obj->fence_excl,
					 reservation_object_held(obj));
}


static inline struct dma_fence *
reservation_object_get_excl_rcu(struct reservation_object *obj)
{
	struct dma_fence *fence;

	if (!rcu_access_pointer(obj->fence_excl))
		return NULL;

	rcu_read_lock();
	fence = dma_fence_get_rcu_safe(&obj->fence_excl);
	rcu_read_unlock();

	return fence;
}

int reservation_object_reserve_shared(struct reservation_object *obj);
void reservation_object_add_shared_fence(struct reservation_object *obj,
					 struct dma_fence *fence);

void reservation_object_add_excl_fence(struct reservation_object *obj,
				       struct dma_fence *fence);

int reservation_object_get_fences_rcu(struct reservation_object *obj,
				      struct dma_fence **pfence_excl,
				      unsigned *pshared_count,
				      struct dma_fence ***pshared);

int reservation_object_copy_fences(struct reservation_object *dst,
				   struct reservation_object *src);

long reservation_object_wait_timeout_rcu(struct reservation_object *obj,
					 bool wait_all, bool intr,
					 unsigned long timeout);

bool reservation_object_test_signaled_rcu(struct reservation_object *obj,
					  bool test_all);

#endif 
