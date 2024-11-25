
#ifndef __DRM_SYNCOBJ_H__
#define __DRM_SYNCOBJ_H__

#include "linux/dma-fence.h"

struct drm_syncobj_cb;


struct drm_syncobj {
	
	struct kref refcount;
	
	struct dma_fence __rcu *fence;
	
	struct list_head cb_list;
	
	spinlock_t lock;
	
	struct file *file;
};

typedef void (*drm_syncobj_func_t)(struct drm_syncobj *syncobj,
				   struct drm_syncobj_cb *cb);


struct drm_syncobj_cb {
	struct list_head node;
	drm_syncobj_func_t func;
};

void drm_syncobj_free(struct kref *kref);


static inline void
drm_syncobj_get(struct drm_syncobj *obj)
{
	kref_get(&obj->refcount);
}


static inline void
drm_syncobj_put(struct drm_syncobj *obj)
{
	kref_put(&obj->refcount, drm_syncobj_free);
}


static inline struct dma_fence *
drm_syncobj_fence_get(struct drm_syncobj *syncobj)
{
	struct dma_fence *fence;

	rcu_read_lock();
	fence = dma_fence_get_rcu_safe(&syncobj->fence);
	rcu_read_unlock();

	return fence;
}

struct drm_syncobj *drm_syncobj_find(struct drm_file *file_private,
				     u32 handle);
void drm_syncobj_add_callback(struct drm_syncobj *syncobj,
			      struct drm_syncobj_cb *cb,
			      drm_syncobj_func_t func);
void drm_syncobj_remove_callback(struct drm_syncobj *syncobj,
				 struct drm_syncobj_cb *cb);
void drm_syncobj_replace_fence(struct drm_syncobj *syncobj,
			       struct dma_fence *fence);
int drm_syncobj_find_fence(struct drm_file *file_private,
			   u32 handle,
			   struct dma_fence **fence);
void drm_syncobj_free(struct kref *kref);
int drm_syncobj_create(struct drm_syncobj **out_syncobj, uint32_t flags,
		       struct dma_fence *fence);
int drm_syncobj_get_handle(struct drm_file *file_private,
			   struct drm_syncobj *syncobj, u32 *handle);
int drm_syncobj_get_fd(struct drm_syncobj *syncobj, int *p_fd);

#endif
