/*
 * SPDX-License-Identifier: MIT
 *
<<<<<<< HEAD
 * Copyright � 2008-2018 Intel Corporation
=======
 * Copyright © 2008-2018 Intel Corporation
>>>>>>> upstream/android-13
 */

#ifndef _I915_GPU_ERROR_H_
#define _I915_GPU_ERROR_H_

<<<<<<< HEAD
=======
#include <linux/atomic.h>
>>>>>>> upstream/android-13
#include <linux/kref.h>
#include <linux/ktime.h>
#include <linux/sched.h>

#include <drm/drm_mm.h>

<<<<<<< HEAD
#include "intel_device_info.h"
#include "intel_ringbuffer.h"
#include "intel_uc_fw.h"
=======
#include "gt/intel_engine.h"
#include "gt/intel_gt_types.h"
#include "gt/uc/intel_uc_fw.h"

#include "intel_device_info.h"
>>>>>>> upstream/android-13

#include "i915_gem.h"
#include "i915_gem_gtt.h"
#include "i915_params.h"
#include "i915_scheduler.h"

struct drm_i915_private;
<<<<<<< HEAD
struct intel_overlay_error_state;
struct intel_display_error_state;

struct i915_gpu_state {
	struct kref ref;
	ktime_t time;
	ktime_t boottime;
	ktime_t uptime;
	unsigned long capture;
	unsigned long epoch;

	struct drm_i915_private *i915;

	char error_msg[128];
	bool simulated;
	bool awake;
	bool wakelock;
	bool suspended;
	int iommu;
	u32 reset_count;
	u32 suspend_count;
	struct intel_device_info device_info;
	struct intel_driver_caps driver_caps;
	struct i915_params params;

	struct i915_error_uc {
		struct intel_uc_fw guc_fw;
		struct intel_uc_fw huc_fw;
		struct drm_i915_error_object *guc_log;
	} uc;
=======
struct i915_vma_compress;
struct intel_engine_capture_vma;
struct intel_overlay_error_state;

struct i915_vma_coredump {
	struct i915_vma_coredump *next;

	char name[20];

	u64 gtt_offset;
	u64 gtt_size;
	u32 gtt_page_sizes;

	int num_pages;
	int page_count;
	int unused;
	u32 *pages[];
};

struct i915_request_coredump {
	unsigned long flags;
	pid_t pid;
	u32 context;
	u32 seqno;
	u32 head;
	u32 tail;
	struct i915_sched_attr sched_attr;
};

struct intel_engine_coredump {
	const struct intel_engine_cs *engine;

	bool hung;
	bool simulated;
	u32 reset_count;

	/* position of active request inside the ring */
	u32 rq_head, rq_post, rq_tail;

	/* Register state */
	u32 ccid;
	u32 start;
	u32 tail;
	u32 head;
	u32 ctl;
	u32 mode;
	u32 hws;
	u32 ipeir;
	u32 ipehr;
	u32 esr;
	u32 bbstate;
	u32 instpm;
	u32 instps;
	u64 bbaddr;
	u64 acthd;
	u32 fault_reg;
	u64 faddr;
	u32 rc_psmi; /* sleep state */
	struct intel_instdone instdone;

	struct i915_gem_context_coredump {
		char comm[TASK_COMM_LEN];

		u64 total_runtime;
		u32 avg_runtime;

		pid_t pid;
		int active;
		int guilty;
		struct i915_sched_attr sched_attr;
	} context;

	struct i915_vma_coredump *vma;

	struct i915_request_coredump execlist[EXECLIST_MAX_PORTS];
	unsigned int num_ports;

	struct {
		u32 gfx_mode;
		union {
			u64 pdp[4];
			u32 pp_dir_base;
		};
	} vm_info;

	struct intel_engine_coredump *next;
};

struct intel_gt_coredump {
	const struct intel_gt *_gt;
	bool awake;
	bool simulated;

	struct intel_gt_info info;
>>>>>>> upstream/android-13

	/* Generic register state */
	u32 eir;
	u32 pgtbl_er;
	u32 ier;
	u32 gtier[6], ngtier;
<<<<<<< HEAD
	u32 ccid;
=======
>>>>>>> upstream/android-13
	u32 derrmr;
	u32 forcewake;
	u32 error; /* gen6+ */
	u32 err_int; /* gen7 */
	u32 fault_data0; /* gen8, gen9 */
	u32 fault_data1; /* gen8, gen9 */
	u32 done_reg;
	u32 gac_eco;
	u32 gam_ecochk;
	u32 gab_ctl;
	u32 gfx_mode;
<<<<<<< HEAD

	u32 nfence;
	u64 fence[I915_MAX_NUM_FENCES];
	struct intel_overlay_error_state *overlay;
	struct intel_display_error_state *display;

	struct drm_i915_error_engine {
		int engine_id;
		/* Software tracked state */
		bool idle;
		bool waiting;
		int num_waiters;
		unsigned long hangcheck_timestamp;
		bool hangcheck_stalled;
		enum intel_engine_hangcheck_action hangcheck_action;
		struct i915_address_space *vm;
		int num_requests;
		u32 reset_count;

		/* position of active request inside the ring */
		u32 rq_head, rq_post, rq_tail;

		/* our own tracking of ring head and tail */
		u32 cpu_ring_head;
		u32 cpu_ring_tail;

		u32 last_seqno;

		/* Register state */
		u32 start;
		u32 tail;
		u32 head;
		u32 ctl;
		u32 mode;
		u32 hws;
		u32 ipeir;
		u32 ipehr;
		u32 bbstate;
		u32 instpm;
		u32 instps;
		u32 seqno;
		u64 bbaddr;
		u64 acthd;
		u32 fault_reg;
		u64 faddr;
		u32 rc_psmi; /* sleep state */
		u32 semaphore_mboxes[I915_NUM_ENGINES - 1];
		struct intel_instdone instdone;

		struct drm_i915_error_context {
			char comm[TASK_COMM_LEN];
			pid_t pid;
			u32 handle;
			u32 hw_id;
			int ban_score;
			int active;
			int guilty;
			bool bannable;
			struct i915_sched_attr sched_attr;
		} context;

		struct drm_i915_error_object {
			u64 gtt_offset;
			u64 gtt_size;
			int num_pages;
			int page_count;
			int unused;
			u32 *pages[0];
		} *ringbuffer, *batchbuffer, *wa_batchbuffer, *ctx, *hws_page;

		struct drm_i915_error_object **user_bo;
		long user_bo_count;

		struct drm_i915_error_object *wa_ctx;
		struct drm_i915_error_object *default_state;

		struct drm_i915_error_request {
			long jiffies;
			pid_t pid;
			u32 context;
			int ban_score;
			u32 seqno;
			u32 start;
			u32 head;
			u32 tail;
			struct i915_sched_attr sched_attr;
		} *requests, execlist[EXECLIST_MAX_PORTS];
		unsigned int num_ports;

		struct drm_i915_error_waiter {
			char comm[TASK_COMM_LEN];
			pid_t pid;
			u32 seqno;
		} *waiters;

		struct {
			u32 gfx_mode;
			union {
				u64 pdp[4];
				u32 pp_dir_base;
			};
		} vm_info;
	} engine[I915_NUM_ENGINES];

	struct drm_i915_error_buffer {
		u32 size;
		u32 name;
		u32 wseqno;
		u64 gtt_offset;
		u32 read_domains;
		u32 write_domain;
		s32 fence_reg:I915_MAX_NUM_FENCE_BITS;
		u32 tiling:2;
		u32 dirty:1;
		u32 purgeable:1;
		u32 userptr:1;
		s32 engine:4;
		u32 cache_level:3;
	} *active_bo[I915_NUM_ENGINES], *pinned_bo;
	u32 active_bo_count[I915_NUM_ENGINES], pinned_bo_count;
	struct i915_address_space *active_vm[I915_NUM_ENGINES];
};

struct i915_gpu_error {
	/* For hangcheck timer */
#define DRM_I915_HANGCHECK_PERIOD 1500 /* in ms */
#define DRM_I915_HANGCHECK_JIFFIES msecs_to_jiffies(DRM_I915_HANGCHECK_PERIOD)

	struct delayed_work hangcheck_work;

	/* For reset and error_state handling. */
	spinlock_t lock;
	/* Protected by the above dev->gpu_error.lock. */
	struct i915_gpu_state *first_error;

	atomic_t pending_fb_pin;

	unsigned long missed_irq_rings;

	/**
	 * State variable controlling the reset flow and count
	 *
	 * This is a counter which gets incremented when reset is triggered,
	 *
	 * Before the reset commences, the I915_RESET_BACKOFF bit is set
	 * meaning that any waiters holding onto the struct_mutex should
	 * relinquish the lock immediately in order for the reset to start.
	 *
	 * If reset is not completed successfully, the I915_WEDGE bit is
	 * set meaning that hardware is terminally sour and there is no
	 * recovery. All waiters on the reset_queue will be woken when
	 * that happens.
	 *
	 * This counter is used by the wait_seqno code to notice that reset
	 * event happened and it needs to restart the entire ioctl (since most
	 * likely the seqno it waited for won't ever signal anytime soon).
	 *
	 * This is important for lock-free wait paths, where no contended lock
	 * naturally enforces the correct ordering between the bail-out of the
	 * waiter and the gpu reset work code.
	 */
	unsigned long reset_count;

	/**
	 * flags: Control various stages of the GPU reset
	 *
	 * #I915_RESET_BACKOFF - When we start a reset, we want to stop any
	 * other users acquiring the struct_mutex. To do this we set the
	 * #I915_RESET_BACKOFF bit in the error flags when we detect a reset
	 * and then check for that bit before acquiring the struct_mutex (in
	 * i915_mutex_lock_interruptible()?). I915_RESET_BACKOFF serves a
	 * secondary role in preventing two concurrent global reset attempts.
	 *
	 * #I915_RESET_HANDOFF - To perform the actual GPU reset, we need the
	 * struct_mutex. We try to acquire the struct_mutex in the reset worker,
	 * but it may be held by some long running waiter (that we cannot
	 * interrupt without causing trouble). Once we are ready to do the GPU
	 * reset, we set the I915_RESET_HANDOFF bit and wakeup any waiters. If
	 * they already hold the struct_mutex and want to participate they can
	 * inspect the bit and do the reset directly, otherwise the worker
	 * waits for the struct_mutex.
	 *
	 * #I915_RESET_ENGINE[num_engines] - Since the driver doesn't need to
	 * acquire the struct_mutex to reset an engine, we need an explicit
	 * flag to prevent two concurrent reset attempts in the same engine.
	 * As the number of engines continues to grow, allocate the flags from
	 * the most significant bits.
	 *
	 * #I915_WEDGED - If reset fails and we can no longer use the GPU,
	 * we set the #I915_WEDGED bit. Prior to command submission, e.g.
	 * i915_request_alloc(), this bit is checked and the sequence
	 * aborted (with -EIO reported to userspace) if set.
	 */
	unsigned long flags;
#define I915_RESET_BACKOFF	0
#define I915_RESET_HANDOFF	1
#define I915_RESET_MODESET	2
#define I915_WEDGED		(BITS_PER_LONG - 1)
#define I915_RESET_ENGINE	(I915_WEDGED - I915_NUM_ENGINES)

	/** Number of times an engine has been reset */
	u32 reset_engine_count[I915_NUM_ENGINES];

	/** Set of stalled engines with guilty requests, in the current reset */
	u32 stalled_mask;

	/** Reason for the current *global* reset */
	const char *reason;

	/**
	 * Waitqueue to signal when a hang is detected. Used to for waiters
	 * to release the struct_mutex for the reset to procede.
	 */
	wait_queue_head_t wait_queue;

	/**
	 * Waitqueue to signal when the reset has completed. Used by clients
	 * that wait for dev_priv->mm.wedged to settle.
	 */
	wait_queue_head_t reset_queue;

	/* For missed irq/seqno simulation. */
	unsigned long test_irq_rings;
=======
	u32 gtt_cache;
	u32 aux_err; /* gen12 */
	u32 sfc_done[GEN12_SFC_DONE_MAX]; /* gen12 */
	u32 gam_done; /* gen12 */

	u32 nfence;
	u64 fence[I915_MAX_NUM_FENCES];

	struct intel_engine_coredump *engine;

	struct intel_uc_coredump {
		struct intel_uc_fw guc_fw;
		struct intel_uc_fw huc_fw;
		struct i915_vma_coredump *guc_log;
	} *uc;

	struct intel_gt_coredump *next;
};

struct i915_gpu_coredump {
	struct kref ref;
	ktime_t time;
	ktime_t boottime;
	ktime_t uptime;
	unsigned long capture;

	struct drm_i915_private *i915;

	struct intel_gt_coredump *gt;

	char error_msg[128];
	bool simulated;
	bool wakelock;
	bool suspended;
	int iommu;
	u32 reset_count;
	u32 suspend_count;

	struct intel_device_info device_info;
	struct intel_runtime_info runtime_info;
	struct intel_driver_caps driver_caps;
	struct i915_params params;

	struct intel_overlay_error_state *overlay;

	struct scatterlist *sgl, *fit;
};

struct i915_gpu_error {
	/* For reset and error_state handling. */
	spinlock_t lock;
	/* Protected by the above dev->gpu_error.lock. */
	struct i915_gpu_coredump *first_error;

	atomic_t pending_fb_pin;

	/** Number of times the device has been reset (global) */
	atomic_t reset_count;

	/** Number of times an engine has been reset */
	atomic_t reset_engine_count[I915_NUM_ENGINES];
>>>>>>> upstream/android-13
};

struct drm_i915_error_state_buf {
	struct drm_i915_private *i915;
<<<<<<< HEAD
	unsigned int bytes;
	unsigned int size;
	int err;
	u8 *buf;
	loff_t start;
	loff_t pos;
=======
	struct scatterlist *sgl, *cur, *end;

	char *buf;
	size_t bytes;
	size_t size;
	loff_t iter;

	int err;
>>>>>>> upstream/android-13
};

#if IS_ENABLED(CONFIG_DRM_I915_CAPTURE_ERROR)

__printf(2, 3)
void i915_error_printf(struct drm_i915_error_state_buf *e, const char *f, ...);
<<<<<<< HEAD
int i915_error_state_to_str(struct drm_i915_error_state_buf *estr,
			    const struct i915_gpu_state *gpu);
int i915_error_state_buf_init(struct drm_i915_error_state_buf *eb,
			      struct drm_i915_private *i915,
			      size_t count, loff_t pos);

static inline void
i915_error_state_buf_release(struct drm_i915_error_state_buf *eb)
{
	kfree(eb->buf);
}

struct i915_gpu_state *i915_capture_gpu_state(struct drm_i915_private *i915);
void i915_capture_error_state(struct drm_i915_private *dev_priv,
			      u32 engine_mask,
			      const char *error_msg);

static inline struct i915_gpu_state *
i915_gpu_state_get(struct i915_gpu_state *gpu)
=======

struct i915_gpu_coredump *i915_gpu_coredump(struct intel_gt *gt,
					    intel_engine_mask_t engine_mask);
void i915_capture_error_state(struct intel_gt *gt,
			      intel_engine_mask_t engine_mask);

struct i915_gpu_coredump *
i915_gpu_coredump_alloc(struct drm_i915_private *i915, gfp_t gfp);

struct intel_gt_coredump *
intel_gt_coredump_alloc(struct intel_gt *gt, gfp_t gfp);

struct intel_engine_coredump *
intel_engine_coredump_alloc(struct intel_engine_cs *engine, gfp_t gfp);

struct intel_engine_capture_vma *
intel_engine_coredump_add_request(struct intel_engine_coredump *ee,
				  struct i915_request *rq,
				  gfp_t gfp);

void intel_engine_coredump_add_vma(struct intel_engine_coredump *ee,
				   struct intel_engine_capture_vma *capture,
				   struct i915_vma_compress *compress);

struct i915_vma_compress *
i915_vma_capture_prepare(struct intel_gt_coredump *gt);

void i915_vma_capture_finish(struct intel_gt_coredump *gt,
			     struct i915_vma_compress *compress);

void i915_error_state_store(struct i915_gpu_coredump *error);

static inline struct i915_gpu_coredump *
i915_gpu_coredump_get(struct i915_gpu_coredump *gpu)
>>>>>>> upstream/android-13
{
	kref_get(&gpu->ref);
	return gpu;
}

<<<<<<< HEAD
void __i915_gpu_state_free(struct kref *kref);
static inline void i915_gpu_state_put(struct i915_gpu_state *gpu)
{
	if (gpu)
		kref_put(&gpu->ref, __i915_gpu_state_free);
}

struct i915_gpu_state *i915_first_error_state(struct drm_i915_private *i915);
void i915_reset_error_state(struct drm_i915_private *i915);

#else

static inline void i915_capture_error_state(struct drm_i915_private *dev_priv,
					    u32 engine_mask,
					    const char *error_msg)
{
}

static inline struct i915_gpu_state *
i915_first_error_state(struct drm_i915_private *i915)
=======
ssize_t
i915_gpu_coredump_copy_to_buffer(struct i915_gpu_coredump *error,
				 char *buf, loff_t offset, size_t count);

void __i915_gpu_coredump_free(struct kref *kref);
static inline void i915_gpu_coredump_put(struct i915_gpu_coredump *gpu)
{
	if (gpu)
		kref_put(&gpu->ref, __i915_gpu_coredump_free);
}

struct i915_gpu_coredump *i915_first_error_state(struct drm_i915_private *i915);
void i915_reset_error_state(struct drm_i915_private *i915);
void i915_disable_error_state(struct drm_i915_private *i915, int err);

#else

static inline void
i915_capture_error_state(struct intel_gt *gt, intel_engine_mask_t engine_mask)
{
}

static inline struct i915_gpu_coredump *
i915_gpu_coredump_alloc(struct drm_i915_private *i915, gfp_t gfp)
>>>>>>> upstream/android-13
{
	return NULL;
}

<<<<<<< HEAD
=======
static inline struct intel_gt_coredump *
intel_gt_coredump_alloc(struct intel_gt *gt, gfp_t gfp)
{
	return NULL;
}

static inline struct intel_engine_coredump *
intel_engine_coredump_alloc(struct intel_engine_cs *engine, gfp_t gfp)
{
	return NULL;
}

static inline struct intel_engine_capture_vma *
intel_engine_coredump_add_request(struct intel_engine_coredump *ee,
				  struct i915_request *rq,
				  gfp_t gfp)
{
	return NULL;
}

static inline void
intel_engine_coredump_add_vma(struct intel_engine_coredump *ee,
			      struct intel_engine_capture_vma *capture,
			      struct i915_vma_compress *compress)
{
}

static inline struct i915_vma_compress *
i915_vma_capture_prepare(struct intel_gt_coredump *gt)
{
	return NULL;
}

static inline void
i915_vma_capture_finish(struct intel_gt_coredump *gt,
			struct i915_vma_compress *compress)
{
}

static inline void
i915_error_state_store(struct i915_gpu_coredump *error)
{
}

static inline void i915_gpu_coredump_put(struct i915_gpu_coredump *gpu)
{
}

static inline struct i915_gpu_coredump *
i915_first_error_state(struct drm_i915_private *i915)
{
	return ERR_PTR(-ENODEV);
}

>>>>>>> upstream/android-13
static inline void i915_reset_error_state(struct drm_i915_private *i915)
{
}

<<<<<<< HEAD
=======
static inline void i915_disable_error_state(struct drm_i915_private *i915,
					    int err)
{
}

>>>>>>> upstream/android-13
#endif /* IS_ENABLED(CONFIG_DRM_I915_CAPTURE_ERROR) */

#endif /* _I915_GPU_ERROR_H_ */
