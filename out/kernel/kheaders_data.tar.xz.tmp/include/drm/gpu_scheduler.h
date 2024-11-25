

#ifndef _DRM_GPU_SCHEDULER_H_
#define _DRM_GPU_SCHEDULER_H_

#include <drm/spsc_queue.h>
#include <linux/dma-fence.h>

#define MAX_WAIT_SCHED_ENTITY_Q_EMPTY msecs_to_jiffies(1000)

struct drm_gpu_scheduler;
struct drm_sched_rq;

enum drm_sched_priority {
	DRM_SCHED_PRIORITY_MIN,
	DRM_SCHED_PRIORITY_LOW = DRM_SCHED_PRIORITY_MIN,
	DRM_SCHED_PRIORITY_NORMAL,
	DRM_SCHED_PRIORITY_HIGH_SW,
	DRM_SCHED_PRIORITY_HIGH_HW,
	DRM_SCHED_PRIORITY_KERNEL,
	DRM_SCHED_PRIORITY_MAX,
	DRM_SCHED_PRIORITY_INVALID = -1,
	DRM_SCHED_PRIORITY_UNSET = -2
};


struct drm_sched_entity {
	struct list_head		list;
	struct drm_sched_rq		*rq;
	spinlock_t			rq_lock;

	struct spsc_queue		job_queue;

	atomic_t			fence_seq;
	uint64_t			fence_context;

	struct dma_fence		*dependency;
	struct dma_fence_cb		cb;
	atomic_t			*guilty;
	struct dma_fence                *last_scheduled;
	struct task_struct		*last_user;
};


struct drm_sched_rq {
	spinlock_t			lock;
	struct drm_gpu_scheduler	*sched;
	struct list_head		entities;
	struct drm_sched_entity		*current_entity;
};


struct drm_sched_fence {
        
	struct dma_fence		scheduled;

        
	struct dma_fence		finished;

        
	struct dma_fence_cb		cb;
        
	struct dma_fence		*parent;
        
	struct drm_gpu_scheduler	*sched;
        
	spinlock_t			lock;
        
	void				*owner;
};

struct drm_sched_fence *to_drm_sched_fence(struct dma_fence *f);


struct drm_sched_job {
	struct spsc_node		queue_node;
	struct drm_gpu_scheduler	*sched;
	struct drm_sched_fence		*s_fence;
	struct dma_fence_cb		finish_cb;
	struct work_struct		finish_work;
	struct list_head		node;
	struct delayed_work		work_tdr;
	uint64_t			id;
	atomic_t			karma;
	enum drm_sched_priority		s_priority;
	struct drm_sched_entity  *entity;
};

static inline bool drm_sched_invalidate_job(struct drm_sched_job *s_job,
					    int threshold)
{
	return (s_job && atomic_inc_return(&s_job->karma) > threshold);
}


struct drm_sched_backend_ops {
	
	struct dma_fence *(*dependency)(struct drm_sched_job *sched_job,
					struct drm_sched_entity *s_entity);

	
	struct dma_fence *(*run_job)(struct drm_sched_job *sched_job);

	
	void (*timedout_job)(struct drm_sched_job *sched_job);

	
	void (*free_job)(struct drm_sched_job *sched_job);
};


struct drm_gpu_scheduler {
	const struct drm_sched_backend_ops	*ops;
	uint32_t			hw_submission_limit;
	long				timeout;
	const char			*name;
	struct drm_sched_rq		sched_rq[DRM_SCHED_PRIORITY_MAX];
	wait_queue_head_t		wake_up_worker;
	wait_queue_head_t		job_scheduled;
	atomic_t			hw_rq_count;
	atomic64_t			job_id_count;
	struct task_struct		*thread;
	struct list_head		ring_mirror_list;
	spinlock_t			job_list_lock;
	int				hang_limit;
};

int drm_sched_init(struct drm_gpu_scheduler *sched,
		   const struct drm_sched_backend_ops *ops,
		   uint32_t hw_submission, unsigned hang_limit, long timeout,
		   const char *name);
void drm_sched_fini(struct drm_gpu_scheduler *sched);

int drm_sched_entity_init(struct drm_sched_entity *entity,
			  struct drm_sched_rq **rq_list,
			  unsigned int num_rq_list,
			  atomic_t *guilty);
long drm_sched_entity_flush(struct drm_sched_entity *entity, long timeout);
void drm_sched_entity_fini(struct drm_sched_entity *entity);
void drm_sched_entity_destroy(struct drm_sched_entity *entity);
void drm_sched_entity_push_job(struct drm_sched_job *sched_job,
			       struct drm_sched_entity *entity);
void drm_sched_entity_set_rq(struct drm_sched_entity *entity,
			     struct drm_sched_rq *rq);

struct drm_sched_fence *drm_sched_fence_create(
	struct drm_sched_entity *s_entity, void *owner);
void drm_sched_fence_scheduled(struct drm_sched_fence *fence);
void drm_sched_fence_finished(struct drm_sched_fence *fence);
int drm_sched_job_init(struct drm_sched_job *job,
		       struct drm_sched_entity *entity,
		       void *owner);
void drm_sched_hw_job_reset(struct drm_gpu_scheduler *sched,
			    struct drm_sched_job *job);
void drm_sched_job_recovery(struct drm_gpu_scheduler *sched);
bool drm_sched_dependency_optimized(struct dma_fence* fence,
				    struct drm_sched_entity *entity);
void drm_sched_job_kickout(struct drm_sched_job *s_job);

#endif
