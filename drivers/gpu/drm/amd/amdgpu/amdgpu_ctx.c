/*
 * Copyright 2015 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: monk liu <monk.liu@amd.com>
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_auth.h>
#include "amdgpu.h"
#include "amdgpu_sched.h"
=======
#include <drm/drm_auth.h>
#include "amdgpu.h"
#include "amdgpu_sched.h"
#include "amdgpu_ras.h"
#include <linux/nospec.h>

#define to_amdgpu_ctx_entity(e)	\
	container_of((e), struct amdgpu_ctx_entity, entity)

const unsigned int amdgpu_ctx_num_entities[AMDGPU_HW_IP_NUM] = {
	[AMDGPU_HW_IP_GFX]	=	1,
	[AMDGPU_HW_IP_COMPUTE]	=	4,
	[AMDGPU_HW_IP_DMA]	=	2,
	[AMDGPU_HW_IP_UVD]	=	1,
	[AMDGPU_HW_IP_VCE]	=	1,
	[AMDGPU_HW_IP_UVD_ENC]	=	1,
	[AMDGPU_HW_IP_VCN_DEC]	=	1,
	[AMDGPU_HW_IP_VCN_ENC]	=	1,
	[AMDGPU_HW_IP_VCN_JPEG]	=	1,
};
>>>>>>> upstream/android-13

static int amdgpu_ctx_priority_permit(struct drm_file *filp,
				      enum drm_sched_priority priority)
{
<<<<<<< HEAD
=======
	if (priority < 0 || priority >= DRM_SCHED_PRIORITY_COUNT)
		return -EINVAL;

>>>>>>> upstream/android-13
	/* NORMAL and below are accessible by everyone */
	if (priority <= DRM_SCHED_PRIORITY_NORMAL)
		return 0;

	if (capable(CAP_SYS_NICE))
		return 0;

	if (drm_is_current_master(filp))
		return 0;

	return -EACCES;
}

<<<<<<< HEAD
=======
static enum gfx_pipe_priority amdgpu_ctx_sched_prio_to_compute_prio(enum drm_sched_priority prio)
{
	switch (prio) {
	case DRM_SCHED_PRIORITY_HIGH:
	case DRM_SCHED_PRIORITY_KERNEL:
		return AMDGPU_GFX_PIPE_PRIO_HIGH;
	default:
		return AMDGPU_GFX_PIPE_PRIO_NORMAL;
	}
}

static unsigned int amdgpu_ctx_prio_sched_to_hw(struct amdgpu_device *adev,
						 enum drm_sched_priority prio,
						 u32 hw_ip)
{
	unsigned int hw_prio;

	hw_prio = (hw_ip == AMDGPU_HW_IP_COMPUTE) ?
			amdgpu_ctx_sched_prio_to_compute_prio(prio) :
			AMDGPU_RING_PRIO_DEFAULT;
	hw_ip = array_index_nospec(hw_ip, AMDGPU_HW_IP_NUM);
	if (adev->gpu_sched[hw_ip][hw_prio].num_scheds == 0)
		hw_prio = AMDGPU_RING_PRIO_DEFAULT;

	return hw_prio;
}

static int amdgpu_ctx_init_entity(struct amdgpu_ctx *ctx, u32 hw_ip,
				   const u32 ring)
{
	struct amdgpu_device *adev = ctx->adev;
	struct amdgpu_ctx_entity *entity;
	struct drm_gpu_scheduler **scheds = NULL, *sched = NULL;
	unsigned num_scheds = 0;
	unsigned int hw_prio;
	enum drm_sched_priority priority;
	int r;

	entity = kzalloc(struct_size(entity, fences, amdgpu_sched_jobs),
			 GFP_KERNEL);
	if (!entity)
		return  -ENOMEM;

	entity->sequence = 1;
	priority = (ctx->override_priority == DRM_SCHED_PRIORITY_UNSET) ?
				ctx->init_priority : ctx->override_priority;
	hw_prio = amdgpu_ctx_prio_sched_to_hw(adev, priority, hw_ip);

	hw_ip = array_index_nospec(hw_ip, AMDGPU_HW_IP_NUM);
	scheds = adev->gpu_sched[hw_ip][hw_prio].sched;
	num_scheds = adev->gpu_sched[hw_ip][hw_prio].num_scheds;

	/* disable load balance if the hw engine retains context among dependent jobs */
	if (hw_ip == AMDGPU_HW_IP_VCN_ENC ||
	    hw_ip == AMDGPU_HW_IP_VCN_DEC ||
	    hw_ip == AMDGPU_HW_IP_UVD_ENC ||
	    hw_ip == AMDGPU_HW_IP_UVD) {
		sched = drm_sched_pick_best(scheds, num_scheds);
		scheds = &sched;
		num_scheds = 1;
	}

	r = drm_sched_entity_init(&entity->entity, priority, scheds, num_scheds,
				  &ctx->guilty);
	if (r)
		goto error_free_entity;

	ctx->entities[hw_ip][ring] = entity;
	return 0;

error_free_entity:
	kfree(entity);

	return r;
}

>>>>>>> upstream/android-13
static int amdgpu_ctx_init(struct amdgpu_device *adev,
			   enum drm_sched_priority priority,
			   struct drm_file *filp,
			   struct amdgpu_ctx *ctx)
{
<<<<<<< HEAD
	unsigned i, j;
	int r;

	if (priority < 0 || priority >= DRM_SCHED_PRIORITY_MAX)
		return -EINVAL;

=======
	int r;

>>>>>>> upstream/android-13
	r = amdgpu_ctx_priority_permit(filp, priority);
	if (r)
		return r;

	memset(ctx, 0, sizeof(*ctx));
<<<<<<< HEAD
	ctx->adev = adev;
	kref_init(&ctx->refcount);
	spin_lock_init(&ctx->ring_lock);
	ctx->fences = kcalloc(amdgpu_sched_jobs * AMDGPU_MAX_RINGS,
			      sizeof(struct dma_fence*), GFP_KERNEL);
	if (!ctx->fences)
		return -ENOMEM;

	mutex_init(&ctx->lock);

	for (i = 0; i < AMDGPU_MAX_RINGS; ++i) {
		ctx->rings[i].sequence = 1;
		ctx->rings[i].fences = &ctx->fences[amdgpu_sched_jobs * i];
	}

=======

	ctx->adev = adev;

	kref_init(&ctx->refcount);
	spin_lock_init(&ctx->ring_lock);
	mutex_init(&ctx->lock);

>>>>>>> upstream/android-13
	ctx->reset_counter = atomic_read(&adev->gpu_reset_counter);
	ctx->reset_counter_query = ctx->reset_counter;
	ctx->vram_lost_counter = atomic_read(&adev->vram_lost_counter);
	ctx->init_priority = priority;
	ctx->override_priority = DRM_SCHED_PRIORITY_UNSET;

<<<<<<< HEAD
	/* create context entity for each ring */
	for (i = 0; i < adev->num_rings; i++) {
		struct amdgpu_ring *ring = adev->rings[i];
		struct drm_sched_rq *rq;

		rq = &ring->sched.sched_rq[priority];

		if (ring == &adev->gfx.kiq.ring)
			continue;

		r = drm_sched_entity_init(&ctx->rings[i].entity,
					  &rq, 1, &ctx->guilty);
		if (r)
			goto failed;
	}

	r = amdgpu_queue_mgr_init(adev, &ctx->queue_mgr);
	if (r)
		goto failed;

	return 0;

failed:
	for (j = 0; j < i; j++)
		drm_sched_entity_destroy(&ctx->rings[j].entity);
	kfree(ctx->fences);
	ctx->fences = NULL;
	return r;
=======
	return 0;
}

static void amdgpu_ctx_fini_entity(struct amdgpu_ctx_entity *entity)
{

	int i;

	if (!entity)
		return;

	for (i = 0; i < amdgpu_sched_jobs; ++i)
		dma_fence_put(entity->fences[i]);

	kfree(entity);
>>>>>>> upstream/android-13
}

static void amdgpu_ctx_fini(struct kref *ref)
{
	struct amdgpu_ctx *ctx = container_of(ref, struct amdgpu_ctx, refcount);
	struct amdgpu_device *adev = ctx->adev;
	unsigned i, j;

	if (!adev)
		return;

<<<<<<< HEAD
	for (i = 0; i < AMDGPU_MAX_RINGS; ++i)
		for (j = 0; j < amdgpu_sched_jobs; ++j)
			dma_fence_put(ctx->rings[i].fences[j]);
	kfree(ctx->fences);
	ctx->fences = NULL;

	amdgpu_queue_mgr_fini(adev, &ctx->queue_mgr);

	mutex_destroy(&ctx->lock);

	kfree(ctx);
}

=======
	for (i = 0; i < AMDGPU_HW_IP_NUM; ++i) {
		for (j = 0; j < AMDGPU_MAX_ENTITY_NUM; ++j) {
			amdgpu_ctx_fini_entity(ctx->entities[i][j]);
			ctx->entities[i][j] = NULL;
		}
	}

	mutex_destroy(&ctx->lock);
	kfree(ctx);
}

int amdgpu_ctx_get_entity(struct amdgpu_ctx *ctx, u32 hw_ip, u32 instance,
			  u32 ring, struct drm_sched_entity **entity)
{
	int r;

	if (hw_ip >= AMDGPU_HW_IP_NUM) {
		DRM_ERROR("unknown HW IP type: %d\n", hw_ip);
		return -EINVAL;
	}

	/* Right now all IPs have only one instance - multiple rings. */
	if (instance != 0) {
		DRM_DEBUG("invalid ip instance: %d\n", instance);
		return -EINVAL;
	}

	if (ring >= amdgpu_ctx_num_entities[hw_ip]) {
		DRM_DEBUG("invalid ring: %d %d\n", hw_ip, ring);
		return -EINVAL;
	}

	if (ctx->entities[hw_ip][ring] == NULL) {
		r = amdgpu_ctx_init_entity(ctx, hw_ip, ring);
		if (r)
			return r;
	}

	*entity = &ctx->entities[hw_ip][ring]->entity;
	return 0;
}

>>>>>>> upstream/android-13
static int amdgpu_ctx_alloc(struct amdgpu_device *adev,
			    struct amdgpu_fpriv *fpriv,
			    struct drm_file *filp,
			    enum drm_sched_priority priority,
			    uint32_t *id)
{
	struct amdgpu_ctx_mgr *mgr = &fpriv->ctx_mgr;
	struct amdgpu_ctx *ctx;
	int r;

	ctx = kmalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	mutex_lock(&mgr->lock);
<<<<<<< HEAD
	r = idr_alloc(&mgr->ctx_handles, ctx, 1, 0, GFP_KERNEL);
=======
	r = idr_alloc(&mgr->ctx_handles, ctx, 1, AMDGPU_VM_MAX_NUM_CTX, GFP_KERNEL);
>>>>>>> upstream/android-13
	if (r < 0) {
		mutex_unlock(&mgr->lock);
		kfree(ctx);
		return r;
	}

	*id = (uint32_t)r;
	r = amdgpu_ctx_init(adev, priority, filp, ctx);
	if (r) {
		idr_remove(&mgr->ctx_handles, *id);
		*id = 0;
		kfree(ctx);
	}
	mutex_unlock(&mgr->lock);
	return r;
}

static void amdgpu_ctx_do_release(struct kref *ref)
{
	struct amdgpu_ctx *ctx;
<<<<<<< HEAD
	u32 i;

	ctx = container_of(ref, struct amdgpu_ctx, refcount);

	for (i = 0; i < ctx->adev->num_rings; i++) {

		if (ctx->adev->rings[i] == &ctx->adev->gfx.kiq.ring)
			continue;

		drm_sched_entity_destroy(&ctx->rings[i].entity);
=======
	u32 i, j;

	ctx = container_of(ref, struct amdgpu_ctx, refcount);
	for (i = 0; i < AMDGPU_HW_IP_NUM; ++i) {
		for (j = 0; j < amdgpu_ctx_num_entities[i]; ++j) {
			if (!ctx->entities[i][j])
				continue;

			drm_sched_entity_destroy(&ctx->entities[i][j]->entity);
		}
>>>>>>> upstream/android-13
	}

	amdgpu_ctx_fini(ref);
}

static int amdgpu_ctx_free(struct amdgpu_fpriv *fpriv, uint32_t id)
{
	struct amdgpu_ctx_mgr *mgr = &fpriv->ctx_mgr;
	struct amdgpu_ctx *ctx;

	mutex_lock(&mgr->lock);
	ctx = idr_remove(&mgr->ctx_handles, id);
	if (ctx)
		kref_put(&ctx->refcount, amdgpu_ctx_do_release);
	mutex_unlock(&mgr->lock);
	return ctx ? 0 : -EINVAL;
}

static int amdgpu_ctx_query(struct amdgpu_device *adev,
			    struct amdgpu_fpriv *fpriv, uint32_t id,
			    union drm_amdgpu_ctx_out *out)
{
	struct amdgpu_ctx *ctx;
	struct amdgpu_ctx_mgr *mgr;
	unsigned reset_counter;

	if (!fpriv)
		return -EINVAL;

	mgr = &fpriv->ctx_mgr;
	mutex_lock(&mgr->lock);
	ctx = idr_find(&mgr->ctx_handles, id);
	if (!ctx) {
		mutex_unlock(&mgr->lock);
		return -EINVAL;
	}

	/* TODO: these two are always zero */
	out->state.flags = 0x0;
	out->state.hangs = 0x0;

	/* determine if a GPU reset has occured since the last call */
	reset_counter = atomic_read(&adev->gpu_reset_counter);
	/* TODO: this should ideally return NO, GUILTY, or INNOCENT. */
	if (ctx->reset_counter_query == reset_counter)
		out->state.reset_status = AMDGPU_CTX_NO_RESET;
	else
		out->state.reset_status = AMDGPU_CTX_UNKNOWN_RESET;
	ctx->reset_counter_query = reset_counter;

	mutex_unlock(&mgr->lock);
	return 0;
}

<<<<<<< HEAD
static int amdgpu_ctx_query2(struct amdgpu_device *adev,
	struct amdgpu_fpriv *fpriv, uint32_t id,
	union drm_amdgpu_ctx_out *out)
{
=======
#define AMDGPU_RAS_COUNTE_DELAY_MS 3000

static int amdgpu_ctx_query2(struct amdgpu_device *adev,
			     struct amdgpu_fpriv *fpriv, uint32_t id,
			     union drm_amdgpu_ctx_out *out)
{
	struct amdgpu_ras *con = amdgpu_ras_get_context(adev);
>>>>>>> upstream/android-13
	struct amdgpu_ctx *ctx;
	struct amdgpu_ctx_mgr *mgr;

	if (!fpriv)
		return -EINVAL;

	mgr = &fpriv->ctx_mgr;
	mutex_lock(&mgr->lock);
	ctx = idr_find(&mgr->ctx_handles, id);
	if (!ctx) {
		mutex_unlock(&mgr->lock);
		return -EINVAL;
	}

	out->state.flags = 0x0;
	out->state.hangs = 0x0;

	if (ctx->reset_counter != atomic_read(&adev->gpu_reset_counter))
		out->state.flags |= AMDGPU_CTX_QUERY2_FLAGS_RESET;

	if (ctx->vram_lost_counter != atomic_read(&adev->vram_lost_counter))
		out->state.flags |= AMDGPU_CTX_QUERY2_FLAGS_VRAMLOST;

	if (atomic_read(&ctx->guilty))
		out->state.flags |= AMDGPU_CTX_QUERY2_FLAGS_GUILTY;

<<<<<<< HEAD
=======
	if (adev->ras_enabled && con) {
		/* Return the cached values in O(1),
		 * and schedule delayed work to cache
		 * new vaues.
		 */
		int ce_count, ue_count;

		ce_count = atomic_read(&con->ras_ce_count);
		ue_count = atomic_read(&con->ras_ue_count);

		if (ce_count != ctx->ras_counter_ce) {
			ctx->ras_counter_ce = ce_count;
			out->state.flags |= AMDGPU_CTX_QUERY2_FLAGS_RAS_CE;
		}

		if (ue_count != ctx->ras_counter_ue) {
			ctx->ras_counter_ue = ue_count;
			out->state.flags |= AMDGPU_CTX_QUERY2_FLAGS_RAS_UE;
		}

		schedule_delayed_work(&con->ras_counte_delay_work,
				      msecs_to_jiffies(AMDGPU_RAS_COUNTE_DELAY_MS));
	}

>>>>>>> upstream/android-13
	mutex_unlock(&mgr->lock);
	return 0;
}

int amdgpu_ctx_ioctl(struct drm_device *dev, void *data,
		     struct drm_file *filp)
{
	int r;
	uint32_t id;
	enum drm_sched_priority priority;

	union drm_amdgpu_ctx *args = data;
<<<<<<< HEAD
	struct amdgpu_device *adev = dev->dev_private;
	struct amdgpu_fpriv *fpriv = filp->driver_priv;

	r = 0;
	id = args->in.ctx_id;
	priority = amdgpu_to_sched_priority(args->in.priority);

	/* For backwards compatibility reasons, we need to accept
	 * ioctls with garbage in the priority field */
	if (priority == DRM_SCHED_PRIORITY_INVALID)
=======
	struct amdgpu_device *adev = drm_to_adev(dev);
	struct amdgpu_fpriv *fpriv = filp->driver_priv;

	id = args->in.ctx_id;
	r = amdgpu_to_sched_priority(args->in.priority, &priority);

	/* For backwards compatibility reasons, we need to accept
	 * ioctls with garbage in the priority field */
	if (r == -EINVAL)
>>>>>>> upstream/android-13
		priority = DRM_SCHED_PRIORITY_NORMAL;

	switch (args->in.op) {
	case AMDGPU_CTX_OP_ALLOC_CTX:
		r = amdgpu_ctx_alloc(adev, fpriv, filp, priority, &id);
		args->out.alloc.ctx_id = id;
		break;
	case AMDGPU_CTX_OP_FREE_CTX:
		r = amdgpu_ctx_free(fpriv, id);
		break;
	case AMDGPU_CTX_OP_QUERY_STATE:
		r = amdgpu_ctx_query(adev, fpriv, id, &args->out);
		break;
	case AMDGPU_CTX_OP_QUERY_STATE2:
		r = amdgpu_ctx_query2(adev, fpriv, id, &args->out);
		break;
	default:
		return -EINVAL;
	}

	return r;
}

struct amdgpu_ctx *amdgpu_ctx_get(struct amdgpu_fpriv *fpriv, uint32_t id)
{
	struct amdgpu_ctx *ctx;
	struct amdgpu_ctx_mgr *mgr;

	if (!fpriv)
		return NULL;

	mgr = &fpriv->ctx_mgr;

	mutex_lock(&mgr->lock);
	ctx = idr_find(&mgr->ctx_handles, id);
	if (ctx)
		kref_get(&ctx->refcount);
	mutex_unlock(&mgr->lock);
	return ctx;
}

int amdgpu_ctx_put(struct amdgpu_ctx *ctx)
{
	if (ctx == NULL)
		return -EINVAL;

	kref_put(&ctx->refcount, amdgpu_ctx_do_release);
	return 0;
}

<<<<<<< HEAD
int amdgpu_ctx_add_fence(struct amdgpu_ctx *ctx, struct amdgpu_ring *ring,
			      struct dma_fence *fence, uint64_t* handler)
{
	struct amdgpu_ctx_ring *cring = & ctx->rings[ring->idx];
	uint64_t seq = cring->sequence;
	unsigned idx = 0;
	struct dma_fence *other = NULL;

	idx = seq & (amdgpu_sched_jobs - 1);
	other = cring->fences[idx];
=======
void amdgpu_ctx_add_fence(struct amdgpu_ctx *ctx,
			  struct drm_sched_entity *entity,
			  struct dma_fence *fence, uint64_t *handle)
{
	struct amdgpu_ctx_entity *centity = to_amdgpu_ctx_entity(entity);
	uint64_t seq = centity->sequence;
	struct dma_fence *other = NULL;
	unsigned idx = 0;

	idx = seq & (amdgpu_sched_jobs - 1);
	other = centity->fences[idx];
>>>>>>> upstream/android-13
	if (other)
		BUG_ON(!dma_fence_is_signaled(other));

	dma_fence_get(fence);

	spin_lock(&ctx->ring_lock);
<<<<<<< HEAD
	cring->fences[idx] = fence;
	cring->sequence++;
	spin_unlock(&ctx->ring_lock);

	dma_fence_put(other);
	if (handler)
		*handler = seq;

	return 0;
}

struct dma_fence *amdgpu_ctx_get_fence(struct amdgpu_ctx *ctx,
				       struct amdgpu_ring *ring, uint64_t seq)
{
	struct amdgpu_ctx_ring *cring = & ctx->rings[ring->idx];
=======
	centity->fences[idx] = fence;
	centity->sequence++;
	spin_unlock(&ctx->ring_lock);

	dma_fence_put(other);
	if (handle)
		*handle = seq;
}

struct dma_fence *amdgpu_ctx_get_fence(struct amdgpu_ctx *ctx,
				       struct drm_sched_entity *entity,
				       uint64_t seq)
{
	struct amdgpu_ctx_entity *centity = to_amdgpu_ctx_entity(entity);
>>>>>>> upstream/android-13
	struct dma_fence *fence;

	spin_lock(&ctx->ring_lock);

	if (seq == ~0ull)
<<<<<<< HEAD
		seq = ctx->rings[ring->idx].sequence - 1;

	if (seq >= cring->sequence) {
=======
		seq = centity->sequence - 1;

	if (seq >= centity->sequence) {
>>>>>>> upstream/android-13
		spin_unlock(&ctx->ring_lock);
		return ERR_PTR(-EINVAL);
	}


<<<<<<< HEAD
	if (seq + amdgpu_sched_jobs < cring->sequence) {
=======
	if (seq + amdgpu_sched_jobs < centity->sequence) {
>>>>>>> upstream/android-13
		spin_unlock(&ctx->ring_lock);
		return NULL;
	}

<<<<<<< HEAD
	fence = dma_fence_get(cring->fences[seq & (amdgpu_sched_jobs - 1)]);
=======
	fence = dma_fence_get(centity->fences[seq & (amdgpu_sched_jobs - 1)]);
>>>>>>> upstream/android-13
	spin_unlock(&ctx->ring_lock);

	return fence;
}

<<<<<<< HEAD
void amdgpu_ctx_priority_override(struct amdgpu_ctx *ctx,
				  enum drm_sched_priority priority)
{
	int i;
	struct amdgpu_device *adev = ctx->adev;
	struct drm_sched_rq *rq;
	struct drm_sched_entity *entity;
	struct amdgpu_ring *ring;
	enum drm_sched_priority ctx_prio;
=======
static void amdgpu_ctx_set_entity_priority(struct amdgpu_ctx *ctx,
					    struct amdgpu_ctx_entity *aentity,
					    int hw_ip,
					    enum drm_sched_priority priority)
{
	struct amdgpu_device *adev = ctx->adev;
	unsigned int hw_prio;
	struct drm_gpu_scheduler **scheds = NULL;
	unsigned num_scheds;

	/* set sw priority */
	drm_sched_entity_set_priority(&aentity->entity, priority);

	/* set hw priority */
	if (hw_ip == AMDGPU_HW_IP_COMPUTE) {
		hw_prio = amdgpu_ctx_prio_sched_to_hw(adev, priority,
						      AMDGPU_HW_IP_COMPUTE);
		hw_prio = array_index_nospec(hw_prio, AMDGPU_RING_PRIO_MAX);
		scheds = adev->gpu_sched[hw_ip][hw_prio].sched;
		num_scheds = adev->gpu_sched[hw_ip][hw_prio].num_scheds;
		drm_sched_entity_modify_sched(&aentity->entity, scheds,
					      num_scheds);
	}
}

void amdgpu_ctx_priority_override(struct amdgpu_ctx *ctx,
				  enum drm_sched_priority priority)
{
	enum drm_sched_priority ctx_prio;
	unsigned i, j;
>>>>>>> upstream/android-13

	ctx->override_priority = priority;

	ctx_prio = (ctx->override_priority == DRM_SCHED_PRIORITY_UNSET) ?
			ctx->init_priority : ctx->override_priority;
<<<<<<< HEAD

	for (i = 0; i < adev->num_rings; i++) {
		ring = adev->rings[i];
		entity = &ctx->rings[i].entity;
		rq = &ring->sched.sched_rq[ctx_prio];

		if (ring->funcs->type == AMDGPU_RING_TYPE_KIQ)
			continue;

		drm_sched_entity_set_rq(entity, rq);
	}
}

int amdgpu_ctx_wait_prev_fence(struct amdgpu_ctx *ctx, unsigned ring_id)
{
	struct amdgpu_ctx_ring *cring = &ctx->rings[ring_id];
	unsigned idx = cring->sequence & (amdgpu_sched_jobs - 1);
	struct dma_fence *other = cring->fences[idx];

	if (other) {
		signed long r;
		r = dma_fence_wait(other, true);
		if (r < 0) {
			if (r != -ERESTARTSYS)
				DRM_ERROR("Error (%ld) waiting for fence!\n", r);

			return r;
		}
	}

	return 0;
=======
	for (i = 0; i < AMDGPU_HW_IP_NUM; ++i) {
		for (j = 0; j < amdgpu_ctx_num_entities[i]; ++j) {
			if (!ctx->entities[i][j])
				continue;

			amdgpu_ctx_set_entity_priority(ctx, ctx->entities[i][j],
						       i, ctx_prio);
		}
	}
}

int amdgpu_ctx_wait_prev_fence(struct amdgpu_ctx *ctx,
			       struct drm_sched_entity *entity)
{
	struct amdgpu_ctx_entity *centity = to_amdgpu_ctx_entity(entity);
	struct dma_fence *other;
	unsigned idx;
	long r;

	spin_lock(&ctx->ring_lock);
	idx = centity->sequence & (amdgpu_sched_jobs - 1);
	other = dma_fence_get(centity->fences[idx]);
	spin_unlock(&ctx->ring_lock);

	if (!other)
		return 0;

	r = dma_fence_wait(other, true);
	if (r < 0 && r != -ERESTARTSYS)
		DRM_ERROR("Error (%ld) waiting for fence!\n", r);

	dma_fence_put(other);
	return r;
>>>>>>> upstream/android-13
}

void amdgpu_ctx_mgr_init(struct amdgpu_ctx_mgr *mgr)
{
	mutex_init(&mgr->lock);
	idr_init(&mgr->ctx_handles);
}

<<<<<<< HEAD
void amdgpu_ctx_mgr_entity_flush(struct amdgpu_ctx_mgr *mgr)
{
	struct amdgpu_ctx *ctx;
	struct idr *idp;
	uint32_t id, i;
	long max_wait = MAX_WAIT_SCHED_ENTITY_Q_EMPTY;
=======
long amdgpu_ctx_mgr_entity_flush(struct amdgpu_ctx_mgr *mgr, long timeout)
{
	struct amdgpu_ctx *ctx;
	struct idr *idp;
	uint32_t id, i, j;
>>>>>>> upstream/android-13

	idp = &mgr->ctx_handles;

	mutex_lock(&mgr->lock);
	idr_for_each_entry(idp, ctx, id) {
<<<<<<< HEAD

		if (!ctx->adev) {
			mutex_unlock(&mgr->lock);
			return;
		}

		for (i = 0; i < ctx->adev->num_rings; i++) {

			if (ctx->adev->rings[i] == &ctx->adev->gfx.kiq.ring)
				continue;

			max_wait = drm_sched_entity_flush(&ctx->rings[i].entity,
							  max_wait);
		}
	}
	mutex_unlock(&mgr->lock);
=======
		for (i = 0; i < AMDGPU_HW_IP_NUM; ++i) {
			for (j = 0; j < amdgpu_ctx_num_entities[i]; ++j) {
				struct drm_sched_entity *entity;

				if (!ctx->entities[i][j])
					continue;

				entity = &ctx->entities[i][j]->entity;
				timeout = drm_sched_entity_flush(entity, timeout);
			}
		}
	}
	mutex_unlock(&mgr->lock);
	return timeout;
>>>>>>> upstream/android-13
}

void amdgpu_ctx_mgr_entity_fini(struct amdgpu_ctx_mgr *mgr)
{
	struct amdgpu_ctx *ctx;
	struct idr *idp;
<<<<<<< HEAD
	uint32_t id, i;
=======
	uint32_t id, i, j;
>>>>>>> upstream/android-13

	idp = &mgr->ctx_handles;

	idr_for_each_entry(idp, ctx, id) {
<<<<<<< HEAD

		if (!ctx->adev)
			return;

		for (i = 0; i < ctx->adev->num_rings; i++) {

			if (ctx->adev->rings[i] == &ctx->adev->gfx.kiq.ring)
				continue;

			if (kref_read(&ctx->refcount) == 1)
				drm_sched_entity_fini(&ctx->rings[i].entity);
			else
				DRM_ERROR("ctx %p is still alive\n", ctx);
=======
		if (kref_read(&ctx->refcount) != 1) {
			DRM_ERROR("ctx %p is still alive\n", ctx);
			continue;
		}

		for (i = 0; i < AMDGPU_HW_IP_NUM; ++i) {
			for (j = 0; j < amdgpu_ctx_num_entities[i]; ++j) {
				struct drm_sched_entity *entity;

				if (!ctx->entities[i][j])
					continue;

				entity = &ctx->entities[i][j]->entity;
				drm_sched_entity_fini(entity);
			}
>>>>>>> upstream/android-13
		}
	}
}

void amdgpu_ctx_mgr_fini(struct amdgpu_ctx_mgr *mgr)
{
	struct amdgpu_ctx *ctx;
	struct idr *idp;
	uint32_t id;

	amdgpu_ctx_mgr_entity_fini(mgr);

	idp = &mgr->ctx_handles;

	idr_for_each_entry(idp, ctx, id) {
		if (kref_put(&ctx->refcount, amdgpu_ctx_fini) != 1)
			DRM_ERROR("ctx %p is still alive\n", ctx);
	}

	idr_destroy(&mgr->ctx_handles);
	mutex_destroy(&mgr->lock);
}
<<<<<<< HEAD
=======

static void amdgpu_ctx_fence_time(struct amdgpu_ctx *ctx,
		struct amdgpu_ctx_entity *centity, ktime_t *total, ktime_t *max)
{
	ktime_t now, t1;
	uint32_t i;

	*total = *max = 0;

	now = ktime_get();
	for (i = 0; i < amdgpu_sched_jobs; i++) {
		struct dma_fence *fence;
		struct drm_sched_fence *s_fence;

		spin_lock(&ctx->ring_lock);
		fence = dma_fence_get(centity->fences[i]);
		spin_unlock(&ctx->ring_lock);
		if (!fence)
			continue;
		s_fence = to_drm_sched_fence(fence);
		if (!dma_fence_is_signaled(&s_fence->scheduled)) {
			dma_fence_put(fence);
			continue;
		}
		t1 = s_fence->scheduled.timestamp;
		if (!ktime_before(t1, now)) {
			dma_fence_put(fence);
			continue;
		}
		if (dma_fence_is_signaled(&s_fence->finished) &&
			s_fence->finished.timestamp < now)
			*total += ktime_sub(s_fence->finished.timestamp, t1);
		else
			*total += ktime_sub(now, t1);
		t1 = ktime_sub(now, t1);
		dma_fence_put(fence);
		*max = max(t1, *max);
	}
}

ktime_t amdgpu_ctx_mgr_fence_usage(struct amdgpu_ctx_mgr *mgr, uint32_t hwip,
		uint32_t idx, uint64_t *elapsed)
{
	struct idr *idp;
	struct amdgpu_ctx *ctx;
	uint32_t id;
	struct amdgpu_ctx_entity *centity;
	ktime_t total = 0, max = 0;

	if (idx >= AMDGPU_MAX_ENTITY_NUM)
		return 0;
	idp = &mgr->ctx_handles;
	mutex_lock(&mgr->lock);
	idr_for_each_entry(idp, ctx, id) {
		ktime_t ttotal, tmax;

		if (!ctx->entities[hwip][idx])
			continue;

		centity = ctx->entities[hwip][idx];
		amdgpu_ctx_fence_time(ctx, centity, &ttotal, &tmax);

		/* Harmonic mean approximation diverges for very small
		 * values. If ratio < 0.01% ignore
		 */
		if (AMDGPU_CTX_FENCE_USAGE_MIN_RATIO(tmax, ttotal))
			continue;

		total = ktime_add(total, ttotal);
		max = ktime_after(tmax, max) ? tmax : max;
	}

	mutex_unlock(&mgr->lock);
	if (elapsed)
		*elapsed = max;

	return total;
}
>>>>>>> upstream/android-13
