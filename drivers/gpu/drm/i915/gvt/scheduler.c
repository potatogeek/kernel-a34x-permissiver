/*
 * Copyright(c) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Zhi Wang <zhi.a.wang@intel.com>
 *
 * Contributors:
 *    Ping Gao <ping.a.gao@intel.com>
 *    Tina Zhang <tina.zhang@intel.com>
 *    Chanbin Du <changbin.du@intel.com>
 *    Min He <min.he@intel.com>
 *    Bing Niu <bing.niu@intel.com>
 *    Zhenyu Wang <zhenyuw@linux.intel.com>
 *
 */

#include <linux/kthread.h>

<<<<<<< HEAD
#include "i915_drv.h"
=======
#include "gem/i915_gem_pm.h"
#include "gt/intel_context.h"
#include "gt/intel_execlists_submission.h"
#include "gt/intel_lrc.h"
#include "gt/intel_ring.h"

#include "i915_drv.h"
#include "i915_gem_gtt.h"
>>>>>>> upstream/android-13
#include "gvt.h"

#define RING_CTX_OFF(x) \
	offsetof(struct execlist_ring_context, x)

static void set_context_pdp_root_pointer(
		struct execlist_ring_context *ring_context,
		u32 pdp[8])
{
	int i;

	for (i = 0; i < 8; i++)
		ring_context->pdps[i].val = pdp[7 - i];
}

static void update_shadow_pdps(struct intel_vgpu_workload *workload)
{
<<<<<<< HEAD
	struct drm_i915_gem_object *ctx_obj =
		workload->req->hw_context->state->obj;
	struct execlist_ring_context *shadow_ring_context;
	struct page *page;
=======
	struct execlist_ring_context *shadow_ring_context;
	struct intel_context *ctx = workload->req->context;
>>>>>>> upstream/android-13

	if (WARN_ON(!workload->shadow_mm))
		return;

	if (WARN_ON(!atomic_read(&workload->shadow_mm->pincount)))
		return;

<<<<<<< HEAD
	page = i915_gem_object_get_page(ctx_obj, LRC_STATE_PN);
	shadow_ring_context = kmap(page);
	set_context_pdp_root_pointer(shadow_ring_context,
			(void *)workload->shadow_mm->ppgtt_mm.shadow_pdps);
	kunmap(page);
=======
	shadow_ring_context = (struct execlist_ring_context *)ctx->lrc_reg_state;
	set_context_pdp_root_pointer(shadow_ring_context,
			(void *)workload->shadow_mm->ppgtt_mm.shadow_pdps);
>>>>>>> upstream/android-13
}

/*
 * when populating shadow ctx from guest, we should not overrride oa related
 * registers, so that they will not be overlapped by guest oa configs. Thus
 * made it possible to capture oa data from host for both host and guests.
 */
static void sr_oa_regs(struct intel_vgpu_workload *workload,
		u32 *reg_state, bool save)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = workload->vgpu->gvt->dev_priv;
	u32 ctx_oactxctrl = dev_priv->perf.oa.ctx_oactxctrl_offset;
	u32 ctx_flexeu0 = dev_priv->perf.oa.ctx_flexeu0_offset;
=======
	struct drm_i915_private *dev_priv = workload->vgpu->gvt->gt->i915;
	u32 ctx_oactxctrl = dev_priv->perf.ctx_oactxctrl_offset;
	u32 ctx_flexeu0 = dev_priv->perf.ctx_flexeu0_offset;
>>>>>>> upstream/android-13
	int i = 0;
	u32 flex_mmio[] = {
		i915_mmio_reg_offset(EU_PERF_CNTL0),
		i915_mmio_reg_offset(EU_PERF_CNTL1),
		i915_mmio_reg_offset(EU_PERF_CNTL2),
		i915_mmio_reg_offset(EU_PERF_CNTL3),
		i915_mmio_reg_offset(EU_PERF_CNTL4),
		i915_mmio_reg_offset(EU_PERF_CNTL5),
		i915_mmio_reg_offset(EU_PERF_CNTL6),
	};

<<<<<<< HEAD
	if (workload->ring_id != RCS)
=======
	if (workload->engine->id != RCS0)
>>>>>>> upstream/android-13
		return;

	if (save) {
		workload->oactxctrl = reg_state[ctx_oactxctrl + 1];

		for (i = 0; i < ARRAY_SIZE(workload->flex_mmio); i++) {
			u32 state_offset = ctx_flexeu0 + i * 2;

			workload->flex_mmio[i] = reg_state[state_offset + 1];
		}
	} else {
		reg_state[ctx_oactxctrl] =
			i915_mmio_reg_offset(GEN8_OACTXCONTROL);
		reg_state[ctx_oactxctrl + 1] = workload->oactxctrl;

		for (i = 0; i < ARRAY_SIZE(workload->flex_mmio); i++) {
			u32 state_offset = ctx_flexeu0 + i * 2;
			u32 mmio = flex_mmio[i];

			reg_state[state_offset] = mmio;
			reg_state[state_offset + 1] = workload->flex_mmio[i];
		}
	}
}

static int populate_shadow_context(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct intel_gvt *gvt = vgpu->gvt;
<<<<<<< HEAD
	int ring_id = workload->ring_id;
	struct drm_i915_gem_object *ctx_obj =
		workload->req->hw_context->state->obj;
	struct execlist_ring_context *shadow_ring_context;
	struct page *page;
	void *dst;
	unsigned long context_gpa, context_page_num;
	int i;

	gvt_dbg_sched("ring id %d workload lrca %x", ring_id,
			workload->ctx_desc.lrca);

	context_page_num = gvt->dev_priv->engine[ring_id]->context_size;

	context_page_num = context_page_num >> PAGE_SHIFT;

	if (IS_BROADWELL(gvt->dev_priv) && ring_id == RCS)
		context_page_num = 19;

	i = 2;

	while (i < context_page_num) {
		context_gpa = intel_vgpu_gma_to_gpa(vgpu->gtt.ggtt_mm,
				(u32)((workload->ctx_desc.lrca + i) <<
				I915_GTT_PAGE_SHIFT));
		if (context_gpa == INTEL_GVT_INVALID_ADDR) {
			gvt_vgpu_err("Invalid guest context descriptor\n");
			return -EFAULT;
		}

		page = i915_gem_object_get_page(ctx_obj, LRC_HEADER_PAGES + i);
		dst = kmap(page);
		intel_gvt_hypervisor_read_gpa(vgpu, context_gpa, dst,
				I915_GTT_PAGE_SIZE);
		kunmap(page);
		i++;
	}

	page = i915_gem_object_get_page(ctx_obj, LRC_STATE_PN);
	shadow_ring_context = kmap(page);
=======
	struct intel_context *ctx = workload->req->context;
	struct execlist_ring_context *shadow_ring_context;
	void *dst;
	void *context_base;
	unsigned long context_gpa, context_page_num;
	unsigned long gpa_base; /* first gpa of consecutive GPAs */
	unsigned long gpa_size; /* size of consecutive GPAs */
	struct intel_vgpu_submission *s = &vgpu->submission;
	int i;
	bool skip = false;
	int ring_id = workload->engine->id;
	int ret;

	GEM_BUG_ON(!intel_context_is_pinned(ctx));

	context_base = (void *) ctx->lrc_reg_state -
				(LRC_STATE_PN << I915_GTT_PAGE_SHIFT);

	shadow_ring_context = (void *) ctx->lrc_reg_state;
>>>>>>> upstream/android-13

	sr_oa_regs(workload, (u32 *)shadow_ring_context, true);
#define COPY_REG(name) \
	intel_gvt_hypervisor_read_gpa(vgpu, workload->ring_context_gpa \
		+ RING_CTX_OFF(name.val), &shadow_ring_context->name.val, 4)
#define COPY_REG_MASKED(name) {\
		intel_gvt_hypervisor_read_gpa(vgpu, workload->ring_context_gpa \
					      + RING_CTX_OFF(name.val),\
					      &shadow_ring_context->name.val, 4);\
		shadow_ring_context->name.val |= 0xffff << 16;\
	}

	COPY_REG_MASKED(ctx_ctrl);
	COPY_REG(ctx_timestamp);

<<<<<<< HEAD
	if (ring_id == RCS) {
		COPY_REG(bb_per_ctx_ptr);
		COPY_REG(rcs_indirect_ctx);
		COPY_REG(rcs_indirect_ctx_offset);
	}
#undef COPY_REG
#undef COPY_REG_MASKED

	intel_gvt_hypervisor_read_gpa(vgpu,
			workload->ring_context_gpa +
			sizeof(*shadow_ring_context),
			(void *)shadow_ring_context +
			sizeof(*shadow_ring_context),
			I915_GTT_PAGE_SIZE - sizeof(*shadow_ring_context));

	sr_oa_regs(workload, (u32 *)shadow_ring_context, false);
	kunmap(page);
	return 0;
}

static inline bool is_gvt_request(struct i915_request *req)
{
	return i915_gem_context_force_single_submission(req->gem_context);
}

static void save_ring_hw_state(struct intel_vgpu *vgpu, int ring_id)
{
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
	u32 ring_base = dev_priv->engine[ring_id]->mmio_base;
	i915_reg_t reg;

	reg = RING_INSTDONE(ring_base);
	vgpu_vreg(vgpu, i915_mmio_reg_offset(reg)) = I915_READ_FW(reg);
	reg = RING_ACTHD(ring_base);
	vgpu_vreg(vgpu, i915_mmio_reg_offset(reg)) = I915_READ_FW(reg);
	reg = RING_ACTHD_UDW(ring_base);
	vgpu_vreg(vgpu, i915_mmio_reg_offset(reg)) = I915_READ_FW(reg);
=======
	if (workload->engine->id == RCS0) {
		COPY_REG(bb_per_ctx_ptr);
		COPY_REG(rcs_indirect_ctx);
		COPY_REG(rcs_indirect_ctx_offset);
	} else if (workload->engine->id == BCS0)
		intel_gvt_hypervisor_read_gpa(vgpu,
				workload->ring_context_gpa +
				BCS_TILE_REGISTER_VAL_OFFSET,
				(void *)shadow_ring_context +
				BCS_TILE_REGISTER_VAL_OFFSET, 4);
#undef COPY_REG
#undef COPY_REG_MASKED

	/* don't copy Ring Context (the first 0x50 dwords),
	 * only copy the Engine Context part from guest
	 */
	intel_gvt_hypervisor_read_gpa(vgpu,
			workload->ring_context_gpa +
			RING_CTX_SIZE,
			(void *)shadow_ring_context +
			RING_CTX_SIZE,
			I915_GTT_PAGE_SIZE - RING_CTX_SIZE);

	sr_oa_regs(workload, (u32 *)shadow_ring_context, false);

	gvt_dbg_sched("ring %s workload lrca %x, ctx_id %x, ctx gpa %llx",
			workload->engine->name, workload->ctx_desc.lrca,
			workload->ctx_desc.context_id,
			workload->ring_context_gpa);

	/* only need to ensure this context is not pinned/unpinned during the
	 * period from last submission to this this submission.
	 * Upon reaching this function, the currently submitted context is not
	 * supposed to get unpinned. If a misbehaving guest driver ever does
	 * this, it would corrupt itself.
	 */
	if (s->last_ctx[ring_id].valid &&
			(s->last_ctx[ring_id].lrca ==
				workload->ctx_desc.lrca) &&
			(s->last_ctx[ring_id].ring_context_gpa ==
				workload->ring_context_gpa))
		skip = true;

	s->last_ctx[ring_id].lrca = workload->ctx_desc.lrca;
	s->last_ctx[ring_id].ring_context_gpa = workload->ring_context_gpa;

	if (IS_RESTORE_INHIBIT(shadow_ring_context->ctx_ctrl.val) || skip)
		return 0;

	s->last_ctx[ring_id].valid = false;
	context_page_num = workload->engine->context_size;
	context_page_num = context_page_num >> PAGE_SHIFT;

	if (IS_BROADWELL(gvt->gt->i915) && workload->engine->id == RCS0)
		context_page_num = 19;

	/* find consecutive GPAs from gma until the first inconsecutive GPA.
	 * read from the continuous GPAs into dst virtual address
	 */
	gpa_size = 0;
	for (i = 2; i < context_page_num; i++) {
		context_gpa = intel_vgpu_gma_to_gpa(vgpu->gtt.ggtt_mm,
				(u32)((workload->ctx_desc.lrca + i) <<
				I915_GTT_PAGE_SHIFT));
		if (context_gpa == INTEL_GVT_INVALID_ADDR) {
			gvt_vgpu_err("Invalid guest context descriptor\n");
			return -EFAULT;
		}

		if (gpa_size == 0) {
			gpa_base = context_gpa;
			dst = context_base + (i << I915_GTT_PAGE_SHIFT);
		} else if (context_gpa != gpa_base + gpa_size)
			goto read;

		gpa_size += I915_GTT_PAGE_SIZE;

		if (i == context_page_num - 1)
			goto read;

		continue;

read:
		intel_gvt_hypervisor_read_gpa(vgpu, gpa_base, dst, gpa_size);
		gpa_base = context_gpa;
		gpa_size = I915_GTT_PAGE_SIZE;
		dst = context_base + (i << I915_GTT_PAGE_SHIFT);
	}
	ret = intel_gvt_scan_engine_context(workload);
	if (ret) {
		gvt_vgpu_err("invalid cmd found in guest context pages\n");
		return ret;
	}
	s->last_ctx[ring_id].valid = true;
	return 0;
}

static inline bool is_gvt_request(struct i915_request *rq)
{
	return intel_context_force_single_submission(rq->context);
}

static void save_ring_hw_state(struct intel_vgpu *vgpu,
			       const struct intel_engine_cs *engine)
{
	struct intel_uncore *uncore = engine->uncore;
	i915_reg_t reg;

	reg = RING_INSTDONE(engine->mmio_base);
	vgpu_vreg(vgpu, i915_mmio_reg_offset(reg)) =
		intel_uncore_read(uncore, reg);

	reg = RING_ACTHD(engine->mmio_base);
	vgpu_vreg(vgpu, i915_mmio_reg_offset(reg)) =
		intel_uncore_read(uncore, reg);

	reg = RING_ACTHD_UDW(engine->mmio_base);
	vgpu_vreg(vgpu, i915_mmio_reg_offset(reg)) =
		intel_uncore_read(uncore, reg);
>>>>>>> upstream/android-13
}

static int shadow_context_status_change(struct notifier_block *nb,
		unsigned long action, void *data)
{
<<<<<<< HEAD
	struct i915_request *req = data;
	struct intel_gvt *gvt = container_of(nb, struct intel_gvt,
				shadow_ctx_notifier_block[req->engine->id]);
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
	enum intel_engine_id ring_id = req->engine->id;
	struct intel_vgpu_workload *workload;
	unsigned long flags;

	if (!is_gvt_request(req)) {
=======
	struct i915_request *rq = data;
	struct intel_gvt *gvt = container_of(nb, struct intel_gvt,
				shadow_ctx_notifier_block[rq->engine->id]);
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
	enum intel_engine_id ring_id = rq->engine->id;
	struct intel_vgpu_workload *workload;
	unsigned long flags;

	if (!is_gvt_request(rq)) {
>>>>>>> upstream/android-13
		spin_lock_irqsave(&scheduler->mmio_context_lock, flags);
		if (action == INTEL_CONTEXT_SCHEDULE_IN &&
		    scheduler->engine_owner[ring_id]) {
			/* Switch ring from vGPU to host. */
			intel_gvt_switch_mmio(scheduler->engine_owner[ring_id],
<<<<<<< HEAD
					      NULL, ring_id);
=======
					      NULL, rq->engine);
>>>>>>> upstream/android-13
			scheduler->engine_owner[ring_id] = NULL;
		}
		spin_unlock_irqrestore(&scheduler->mmio_context_lock, flags);

		return NOTIFY_OK;
	}

	workload = scheduler->current_workload[ring_id];
	if (unlikely(!workload))
		return NOTIFY_OK;

	switch (action) {
	case INTEL_CONTEXT_SCHEDULE_IN:
		spin_lock_irqsave(&scheduler->mmio_context_lock, flags);
		if (workload->vgpu != scheduler->engine_owner[ring_id]) {
			/* Switch ring from host to vGPU or vGPU to vGPU. */
			intel_gvt_switch_mmio(scheduler->engine_owner[ring_id],
<<<<<<< HEAD
					      workload->vgpu, ring_id);
=======
					      workload->vgpu, rq->engine);
>>>>>>> upstream/android-13
			scheduler->engine_owner[ring_id] = workload->vgpu;
		} else
			gvt_dbg_sched("skip ring %d mmio switch for vgpu%d\n",
				      ring_id, workload->vgpu->id);
		spin_unlock_irqrestore(&scheduler->mmio_context_lock, flags);
		atomic_set(&workload->shadow_ctx_active, 1);
		break;
	case INTEL_CONTEXT_SCHEDULE_OUT:
<<<<<<< HEAD
		save_ring_hw_state(workload->vgpu, ring_id);
		atomic_set(&workload->shadow_ctx_active, 0);
		break;
	case INTEL_CONTEXT_SCHEDULE_PREEMPTED:
		save_ring_hw_state(workload->vgpu, ring_id);
=======
		save_ring_hw_state(workload->vgpu, rq->engine);
		atomic_set(&workload->shadow_ctx_active, 0);
		break;
	case INTEL_CONTEXT_SCHEDULE_PREEMPTED:
		save_ring_hw_state(workload->vgpu, rq->engine);
>>>>>>> upstream/android-13
		break;
	default:
		WARN_ON(1);
		return NOTIFY_OK;
	}
	wake_up(&workload->shadow_ctx_status_wq);
	return NOTIFY_OK;
}

<<<<<<< HEAD
static void shadow_context_descriptor_update(struct intel_context *ce)
{
	u64 desc = 0;

	desc = ce->lrc_desc;

	/* Update bits 0-11 of the context descriptor which includes flags
	 * like GEN8_CTX_* cached in desc_template
	 */
	desc &= U64_MAX << 12;
	desc |= ce->gem_context->desc_template & ((1ULL << 12) - 1);

	ce->lrc_desc = desc;
=======
static void
shadow_context_descriptor_update(struct intel_context *ce,
				 struct intel_vgpu_workload *workload)
{
	u64 desc = ce->lrc.desc;

	/*
	 * Update bits 0-11 of the context descriptor which includes flags
	 * like GEN8_CTX_* cached in desc_template
	 */
	desc &= ~(0x3ull << GEN8_CTX_ADDRESSING_MODE_SHIFT);
	desc |= (u64)workload->ctx_desc.addressing_mode <<
		GEN8_CTX_ADDRESSING_MODE_SHIFT;

	ce->lrc.desc = desc;
>>>>>>> upstream/android-13
}

static int copy_workload_to_ring_buffer(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct i915_request *req = workload->req;
	void *shadow_ring_buffer_va;
	u32 *cs;
<<<<<<< HEAD

	if ((IS_KABYLAKE(req->i915) || IS_BROXTON(req->i915))
		&& is_inhibit_context(req->hw_context))
		intel_vgpu_restore_inhibit_context(vgpu, req);

=======
	int err;

	if (GRAPHICS_VER(req->engine->i915) == 9 && is_inhibit_context(req->context))
		intel_vgpu_restore_inhibit_context(vgpu, req);

	/*
	 * To track whether a request has started on HW, we can emit a
	 * breadcrumb at the beginning of the request and check its
	 * timeline's HWSP to see if the breadcrumb has advanced past the
	 * start of this request. Actually, the request must have the
	 * init_breadcrumb if its timeline set has_init_bread_crumb, or the
	 * scheduler might get a wrong state of it during reset. Since the
	 * requests from gvt always set the has_init_breadcrumb flag, here
	 * need to do the emit_init_breadcrumb for all the requests.
	 */
	if (req->engine->emit_init_breadcrumb) {
		err = req->engine->emit_init_breadcrumb(req);
		if (err) {
			gvt_vgpu_err("fail to emit init breadcrumb\n");
			return err;
		}
	}

>>>>>>> upstream/android-13
	/* allocate shadow ring buffer */
	cs = intel_ring_begin(workload->req, workload->rb_len / sizeof(u32));
	if (IS_ERR(cs)) {
		gvt_vgpu_err("fail to alloc size =%ld shadow  ring buffer\n",
			workload->rb_len);
		return PTR_ERR(cs);
	}

	shadow_ring_buffer_va = workload->shadow_ring_buffer_va;

	/* get shadow ring buffer va */
	workload->shadow_ring_buffer_va = cs;

	memcpy(cs, shadow_ring_buffer_va,
			workload->rb_len);

	cs += workload->rb_len / sizeof(u32);
	intel_ring_advance(workload->req, cs);

	return 0;
}

static void release_shadow_wa_ctx(struct intel_shadow_wa_ctx *wa_ctx)
{
	if (!wa_ctx->indirect_ctx.obj)
		return;

<<<<<<< HEAD
	i915_gem_object_unpin_map(wa_ctx->indirect_ctx.obj);
	i915_gem_object_put(wa_ctx->indirect_ctx.obj);
=======
	i915_gem_object_lock(wa_ctx->indirect_ctx.obj, NULL);
	i915_gem_object_unpin_map(wa_ctx->indirect_ctx.obj);
	i915_gem_object_unlock(wa_ctx->indirect_ctx.obj);
	i915_gem_object_put(wa_ctx->indirect_ctx.obj);

	wa_ctx->indirect_ctx.obj = NULL;
	wa_ctx->indirect_ctx.shadow_va = NULL;
}

static void set_dma_address(struct i915_page_directory *pd, dma_addr_t addr)
{
	struct scatterlist *sg = pd->pt.base->mm.pages->sgl;

	/* This is not a good idea */
	sg->dma_address = addr;
}

static void set_context_ppgtt_from_shadow(struct intel_vgpu_workload *workload,
					  struct intel_context *ce)
{
	struct intel_vgpu_mm *mm = workload->shadow_mm;
	struct i915_ppgtt *ppgtt = i915_vm_to_ppgtt(ce->vm);
	int i = 0;

	if (mm->ppgtt_mm.root_entry_type == GTT_TYPE_PPGTT_ROOT_L4_ENTRY) {
		set_dma_address(ppgtt->pd, mm->ppgtt_mm.shadow_pdps[0]);
	} else {
		for (i = 0; i < GVT_RING_CTX_NR_PDPS; i++) {
			struct i915_page_directory * const pd =
				i915_pd_entry(ppgtt->pd, i);
			/* skip now as current i915 ppgtt alloc won't allocate
			   top level pdp for non 4-level table, won't impact
			   shadow ppgtt. */
			if (!pd)
				break;

			set_dma_address(pd, mm->ppgtt_mm.shadow_pdps[i]);
		}
	}
}

static int
intel_gvt_workload_req_alloc(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct i915_request *rq;

	if (workload->req)
		return 0;

	rq = i915_request_create(s->shadow[workload->engine->id]);
	if (IS_ERR(rq)) {
		gvt_vgpu_err("fail to allocate gem request\n");
		return PTR_ERR(rq);
	}

	workload->req = i915_request_get(rq);
	return 0;
>>>>>>> upstream/android-13
}

/**
 * intel_gvt_scan_and_shadow_workload - audit the workload by scanning and
 * shadow it as well, include ringbuffer,wa_ctx and ctx.
 * @workload: an abstract entity for each execlist submission.
 *
 * This function is called before the workload submitting to i915, to make
 * sure the content of the workload is valid.
 */
int intel_gvt_scan_and_shadow_workload(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct intel_vgpu_submission *s = &vgpu->submission;
<<<<<<< HEAD
	struct i915_gem_context *shadow_ctx = s->shadow_ctx;
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
	struct intel_engine_cs *engine = dev_priv->engine[workload->ring_id];
	struct intel_context *ce;
	struct i915_request *rq;
	int ret;

	lockdep_assert_held(&dev_priv->drm.struct_mutex);

	if (workload->req)
		return 0;

	/* pin shadow context by gvt even the shadow context will be pinned
	 * when i915 alloc request. That is because gvt will update the guest
	 * context from shadow context when workload is completed, and at that
	 * moment, i915 may already unpined the shadow context to make the
	 * shadow_ctx pages invalid. So gvt need to pin itself. After update
	 * the guest context, gvt can unpin the shadow_ctx safely.
	 */
	ce = intel_context_pin(shadow_ctx, engine);
	if (IS_ERR(ce)) {
		gvt_vgpu_err("fail to pin shadow context\n");
		return PTR_ERR(ce);
	}

	shadow_ctx->desc_template &= ~(0x3 << GEN8_CTX_ADDRESSING_MODE_SHIFT);
	shadow_ctx->desc_template |= workload->ctx_desc.addressing_mode <<
				    GEN8_CTX_ADDRESSING_MODE_SHIFT;

	if (!test_and_set_bit(workload->ring_id, s->shadow_ctx_desc_updated))
		shadow_context_descriptor_update(ce);

	ret = intel_gvt_scan_and_shadow_ringbuffer(workload);
	if (ret)
		goto err_unpin;

	if ((workload->ring_id == RCS) &&
	    (workload->wa_ctx.indirect_ctx.size != 0)) {
=======
	int ret;

	lockdep_assert_held(&vgpu->vgpu_lock);

	if (workload->shadow)
		return 0;

	if (!test_and_set_bit(workload->engine->id, s->shadow_ctx_desc_updated))
		shadow_context_descriptor_update(s->shadow[workload->engine->id],
						 workload);

	ret = intel_gvt_scan_and_shadow_ringbuffer(workload);
	if (ret)
		return ret;

	if (workload->engine->id == RCS0 &&
	    workload->wa_ctx.indirect_ctx.size) {
>>>>>>> upstream/android-13
		ret = intel_gvt_scan_and_shadow_wa_ctx(&workload->wa_ctx);
		if (ret)
			goto err_shadow;
	}

<<<<<<< HEAD
	rq = i915_request_alloc(engine, shadow_ctx);
	if (IS_ERR(rq)) {
		gvt_vgpu_err("fail to allocate gem request\n");
		ret = PTR_ERR(rq);
		goto err_shadow;
	}
	workload->req = i915_request_get(rq);

	ret = populate_shadow_context(workload);
	if (ret)
		goto err_req;

	return 0;
err_req:
	rq = fetch_and_zero(&workload->req);
	i915_request_put(rq);
err_shadow:
	release_shadow_wa_ctx(&workload->wa_ctx);
err_unpin:
	intel_context_unpin(ce);
=======
	workload->shadow = true;
	return 0;

err_shadow:
	release_shadow_wa_ctx(&workload->wa_ctx);
>>>>>>> upstream/android-13
	return ret;
}

static void release_shadow_batch_buffer(struct intel_vgpu_workload *workload);

static int prepare_shadow_batch_buffer(struct intel_vgpu_workload *workload)
{
	struct intel_gvt *gvt = workload->vgpu->gvt;
	const int gmadr_bytes = gvt->device_info.gmadr_bytes_in_cmd;
	struct intel_vgpu_shadow_bb *bb;
<<<<<<< HEAD
=======
	struct i915_gem_ww_ctx ww;
>>>>>>> upstream/android-13
	int ret;

	list_for_each_entry(bb, &workload->shadow_bb, list) {
		/* For privilge batch buffer and not wa_ctx, the bb_start_cmd_va
		 * is only updated into ring_scan_buffer, not real ring address
		 * allocated in later copy_workload_to_ring_buffer. pls be noted
		 * shadow_ring_buffer_va is now pointed to real ring buffer va
		 * in copy_workload_to_ring_buffer.
		 */

		if (bb->bb_offset)
			bb->bb_start_cmd_va = workload->shadow_ring_buffer_va
				+ bb->bb_offset;

<<<<<<< HEAD
		if (bb->ppgtt) {
			/* for non-priv bb, scan&shadow is only for
			 * debugging purpose, so the content of shadow bb
			 * is the same as original bb. Therefore,
			 * here, rather than switch to shadow bb's gma
			 * address, we directly use original batch buffer's
			 * gma address, and send original bb to hardware
			 * directly
			 */
			if (bb->clflush & CLFLUSH_AFTER) {
				drm_clflush_virt_range(bb->va,
						bb->obj->base.size);
				bb->clflush &= ~CLFLUSH_AFTER;
			}
			i915_gem_obj_finish_shmem_access(bb->obj);
			bb->accessing = false;

		} else {
			bb->vma = i915_gem_object_ggtt_pin(bb->obj,
					NULL, 0, 0, 0);
			if (IS_ERR(bb->vma)) {
				ret = PTR_ERR(bb->vma);
=======
		/*
		 * For non-priv bb, scan&shadow is only for
		 * debugging purpose, so the content of shadow bb
		 * is the same as original bb. Therefore,
		 * here, rather than switch to shadow bb's gma
		 * address, we directly use original batch buffer's
		 * gma address, and send original bb to hardware
		 * directly
		 */
		if (!bb->ppgtt) {
			i915_gem_ww_ctx_init(&ww, false);
retry:
			i915_gem_object_lock(bb->obj, &ww);

			bb->vma = i915_gem_object_ggtt_pin_ww(bb->obj, &ww,
							      NULL, 0, 0, 0);
			if (IS_ERR(bb->vma)) {
				ret = PTR_ERR(bb->vma);
				if (ret == -EDEADLK) {
					ret = i915_gem_ww_ctx_backoff(&ww);
					if (!ret)
						goto retry;
				}
>>>>>>> upstream/android-13
				goto err;
			}

			/* relocate shadow batch buffer */
			bb->bb_start_cmd_va[1] = i915_ggtt_offset(bb->vma);
			if (gmadr_bytes == 8)
				bb->bb_start_cmd_va[2] = 0;

<<<<<<< HEAD
			/* No one is going to touch shadow bb from now on. */
			if (bb->clflush & CLFLUSH_AFTER) {
				drm_clflush_virt_range(bb->va,
						bb->obj->base.size);
				bb->clflush &= ~CLFLUSH_AFTER;
			}

			ret = i915_gem_object_set_to_gtt_domain(bb->obj,
					false);
			if (ret)
				goto err;

			i915_gem_obj_finish_shmem_access(bb->obj);
			bb->accessing = false;

=======
>>>>>>> upstream/android-13
			ret = i915_vma_move_to_active(bb->vma,
						      workload->req,
						      0);
			if (ret)
				goto err;
<<<<<<< HEAD
=======

			/* No one is going to touch shadow bb from now on. */
			i915_gem_object_flush_map(bb->obj);
			i915_gem_ww_ctx_fini(&ww);
>>>>>>> upstream/android-13
		}
	}
	return 0;
err:
<<<<<<< HEAD
=======
	i915_gem_ww_ctx_fini(&ww);
>>>>>>> upstream/android-13
	release_shadow_batch_buffer(workload);
	return ret;
}

static void update_wa_ctx_2_shadow_ctx(struct intel_shadow_wa_ctx *wa_ctx)
{
	struct intel_vgpu_workload *workload =
		container_of(wa_ctx, struct intel_vgpu_workload, wa_ctx);
	struct i915_request *rq = workload->req;
	struct execlist_ring_context *shadow_ring_context =
<<<<<<< HEAD
		(struct execlist_ring_context *)rq->hw_context->lrc_reg_state;
=======
		(struct execlist_ring_context *)rq->context->lrc_reg_state;
>>>>>>> upstream/android-13

	shadow_ring_context->bb_per_ctx_ptr.val =
		(shadow_ring_context->bb_per_ctx_ptr.val &
		(~PER_CTX_ADDR_MASK)) | wa_ctx->per_ctx.shadow_gma;
	shadow_ring_context->rcs_indirect_ctx.val =
		(shadow_ring_context->rcs_indirect_ctx.val &
		(~INDIRECT_CTX_ADDR_MASK)) | wa_ctx->indirect_ctx.shadow_gma;
}

static int prepare_shadow_wa_ctx(struct intel_shadow_wa_ctx *wa_ctx)
{
	struct i915_vma *vma;
	unsigned char *per_ctx_va =
		(unsigned char *)wa_ctx->indirect_ctx.shadow_va +
		wa_ctx->indirect_ctx.size;
<<<<<<< HEAD
=======
	struct i915_gem_ww_ctx ww;
	int ret;
>>>>>>> upstream/android-13

	if (wa_ctx->indirect_ctx.size == 0)
		return 0;

<<<<<<< HEAD
	vma = i915_gem_object_ggtt_pin(wa_ctx->indirect_ctx.obj, NULL,
				       0, CACHELINE_BYTES, 0);
	if (IS_ERR(vma))
		return PTR_ERR(vma);
=======
	i915_gem_ww_ctx_init(&ww, false);
retry:
	i915_gem_object_lock(wa_ctx->indirect_ctx.obj, &ww);

	vma = i915_gem_object_ggtt_pin_ww(wa_ctx->indirect_ctx.obj, &ww, NULL,
					  0, CACHELINE_BYTES, 0);
	if (IS_ERR(vma)) {
		ret = PTR_ERR(vma);
		if (ret == -EDEADLK) {
			ret = i915_gem_ww_ctx_backoff(&ww);
			if (!ret)
				goto retry;
		}
		return ret;
	}

	i915_gem_ww_ctx_fini(&ww);
>>>>>>> upstream/android-13

	/* FIXME: we are not tracking our pinned VMA leaving it
	 * up to the core to fix up the stray pin_count upon
	 * free.
	 */

	wa_ctx->indirect_ctx.shadow_gma = i915_ggtt_offset(vma);

	wa_ctx->per_ctx.shadow_gma = *((unsigned int *)per_ctx_va + 1);
	memset(per_ctx_va, 0, CACHELINE_BYTES);

	update_wa_ctx_2_shadow_ctx(wa_ctx);
	return 0;
}

<<<<<<< HEAD
static void release_shadow_batch_buffer(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
=======
static void update_vreg_in_ctx(struct intel_vgpu_workload *workload)
{
	vgpu_vreg_t(workload->vgpu, RING_START(workload->engine->mmio_base)) =
		workload->rb_start;
}

static void release_shadow_batch_buffer(struct intel_vgpu_workload *workload)
{
>>>>>>> upstream/android-13
	struct intel_vgpu_shadow_bb *bb, *pos;

	if (list_empty(&workload->shadow_bb))
		return;

	bb = list_first_entry(&workload->shadow_bb,
			struct intel_vgpu_shadow_bb, list);

<<<<<<< HEAD
	mutex_lock(&dev_priv->drm.struct_mutex);

	list_for_each_entry_safe(bb, pos, &workload->shadow_bb, list) {
		if (bb->obj) {
			if (bb->accessing)
				i915_gem_obj_finish_shmem_access(bb->obj);

			if (bb->va && !IS_ERR(bb->va))
				i915_gem_object_unpin_map(bb->obj);

			if (bb->vma && !IS_ERR(bb->vma)) {
				i915_vma_unpin(bb->vma);
				i915_vma_close(bb->vma);
			}
			__i915_gem_object_release_unless_active(bb->obj);
=======
	list_for_each_entry_safe(bb, pos, &workload->shadow_bb, list) {
		if (bb->obj) {
			i915_gem_object_lock(bb->obj, NULL);
			if (bb->va && !IS_ERR(bb->va))
				i915_gem_object_unpin_map(bb->obj);

			if (bb->vma && !IS_ERR(bb->vma))
				i915_vma_unpin(bb->vma);

			i915_gem_object_unlock(bb->obj);
			i915_gem_object_put(bb->obj);
>>>>>>> upstream/android-13
		}
		list_del(&bb->list);
		kfree(bb);
	}
<<<<<<< HEAD

	mutex_unlock(&dev_priv->drm.struct_mutex);
}

static int prepare_workload(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
=======
}

static int
intel_vgpu_shadow_mm_pin(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct intel_vgpu_mm *m;
>>>>>>> upstream/android-13
	int ret = 0;

	ret = intel_vgpu_pin_mm(workload->shadow_mm);
	if (ret) {
		gvt_vgpu_err("fail to vgpu pin mm\n");
		return ret;
	}

<<<<<<< HEAD
	update_shadow_pdps(workload);

=======
	if (workload->shadow_mm->type != INTEL_GVT_MM_PPGTT ||
	    !workload->shadow_mm->ppgtt_mm.shadowed) {
		gvt_vgpu_err("workload shadow ppgtt isn't ready\n");
		return -EINVAL;
	}

	if (!list_empty(&workload->lri_shadow_mm)) {
		list_for_each_entry(m, &workload->lri_shadow_mm,
				    ppgtt_mm.link) {
			ret = intel_vgpu_pin_mm(m);
			if (ret) {
				list_for_each_entry_from_reverse(m,
								 &workload->lri_shadow_mm,
								 ppgtt_mm.link)
					intel_vgpu_unpin_mm(m);
				gvt_vgpu_err("LRI shadow ppgtt fail to pin\n");
				break;
			}
		}
	}

	if (ret)
		intel_vgpu_unpin_mm(workload->shadow_mm);

	return ret;
}

static void
intel_vgpu_shadow_mm_unpin(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu_mm *m;

	if (!list_empty(&workload->lri_shadow_mm)) {
		list_for_each_entry(m, &workload->lri_shadow_mm,
				    ppgtt_mm.link)
			intel_vgpu_unpin_mm(m);
	}
	intel_vgpu_unpin_mm(workload->shadow_mm);
}

static int prepare_workload(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
	struct intel_vgpu_submission *s = &vgpu->submission;
	int ret = 0;

	ret = intel_vgpu_shadow_mm_pin(workload);
	if (ret) {
		gvt_vgpu_err("fail to pin shadow mm\n");
		return ret;
	}

	update_shadow_pdps(workload);

	set_context_ppgtt_from_shadow(workload, s->shadow[workload->engine->id]);

>>>>>>> upstream/android-13
	ret = intel_vgpu_sync_oos_pages(workload->vgpu);
	if (ret) {
		gvt_vgpu_err("fail to vgpu sync oos pages\n");
		goto err_unpin_mm;
	}

	ret = intel_vgpu_flush_post_shadow(workload->vgpu);
	if (ret) {
		gvt_vgpu_err("fail to flush post shadow\n");
		goto err_unpin_mm;
	}

	ret = copy_workload_to_ring_buffer(workload);
	if (ret) {
		gvt_vgpu_err("fail to generate request\n");
		goto err_unpin_mm;
	}

	ret = prepare_shadow_batch_buffer(workload);
	if (ret) {
		gvt_vgpu_err("fail to prepare_shadow_batch_buffer\n");
		goto err_unpin_mm;
	}

	ret = prepare_shadow_wa_ctx(&workload->wa_ctx);
	if (ret) {
		gvt_vgpu_err("fail to prepare_shadow_wa_ctx\n");
		goto err_shadow_batch;
	}

	if (workload->prepare) {
		ret = workload->prepare(workload);
		if (ret)
			goto err_shadow_wa_ctx;
	}

	return 0;
err_shadow_wa_ctx:
	release_shadow_wa_ctx(&workload->wa_ctx);
err_shadow_batch:
	release_shadow_batch_buffer(workload);
err_unpin_mm:
<<<<<<< HEAD
	intel_vgpu_unpin_mm(workload->shadow_mm);
=======
	intel_vgpu_shadow_mm_unpin(workload);
>>>>>>> upstream/android-13
	return ret;
}

static int dispatch_workload(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu *vgpu = workload->vgpu;
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
	int ring_id = workload->ring_id;
	int ret;

	gvt_dbg_sched("ring id %d prepare to dispatch workload %p\n",
		ring_id, workload);

	mutex_lock(&vgpu->vgpu_lock);
	mutex_lock(&dev_priv->drm.struct_mutex);
=======
	struct i915_request *rq;
	int ret;

	gvt_dbg_sched("ring id %s prepare to dispatch workload %p\n",
		      workload->engine->name, workload);

	mutex_lock(&vgpu->vgpu_lock);

	ret = intel_gvt_workload_req_alloc(workload);
	if (ret)
		goto err_req;
>>>>>>> upstream/android-13

	ret = intel_gvt_scan_and_shadow_workload(workload);
	if (ret)
		goto out;

<<<<<<< HEAD
	ret = prepare_workload(workload);

out:
	if (ret)
		workload->status = ret;

	if (!IS_ERR_OR_NULL(workload->req)) {
		gvt_dbg_sched("ring id %d submit workload to i915 %p\n",
				ring_id, workload->req);
		i915_request_add(workload->req);
		workload->dispatched = true;
	}

	mutex_unlock(&dev_priv->drm.struct_mutex);
=======
	ret = populate_shadow_context(workload);
	if (ret) {
		release_shadow_wa_ctx(&workload->wa_ctx);
		goto out;
	}

	ret = prepare_workload(workload);
out:
	if (ret) {
		/* We might still need to add request with
		 * clean ctx to retire it properly..
		 */
		rq = fetch_and_zero(&workload->req);
		i915_request_put(rq);
	}

	if (!IS_ERR_OR_NULL(workload->req)) {
		gvt_dbg_sched("ring id %s submit workload to i915 %p\n",
			      workload->engine->name, workload->req);
		i915_request_add(workload->req);
		workload->dispatched = true;
	}
err_req:
	if (ret)
		workload->status = ret;
>>>>>>> upstream/android-13
	mutex_unlock(&vgpu->vgpu_lock);
	return ret;
}

<<<<<<< HEAD
static struct intel_vgpu_workload *pick_next_workload(
		struct intel_gvt *gvt, int ring_id)
=======
static struct intel_vgpu_workload *
pick_next_workload(struct intel_gvt *gvt, struct intel_engine_cs *engine)
>>>>>>> upstream/android-13
{
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
	struct intel_vgpu_workload *workload = NULL;

	mutex_lock(&gvt->sched_lock);

	/*
	 * no current vgpu / will be scheduled out / no workload
	 * bail out
	 */
	if (!scheduler->current_vgpu) {
<<<<<<< HEAD
		gvt_dbg_sched("ring id %d stop - no current vgpu\n", ring_id);
=======
		gvt_dbg_sched("ring %s stop - no current vgpu\n", engine->name);
>>>>>>> upstream/android-13
		goto out;
	}

	if (scheduler->need_reschedule) {
<<<<<<< HEAD
		gvt_dbg_sched("ring id %d stop - will reschedule\n", ring_id);
		goto out;
	}

	if (list_empty(workload_q_head(scheduler->current_vgpu, ring_id)))
=======
		gvt_dbg_sched("ring %s stop - will reschedule\n", engine->name);
		goto out;
	}

	if (!scheduler->current_vgpu->active ||
	    list_empty(workload_q_head(scheduler->current_vgpu, engine)))
>>>>>>> upstream/android-13
		goto out;

	/*
	 * still have current workload, maybe the workload disptacher
	 * fail to submit it for some reason, resubmit it.
	 */
<<<<<<< HEAD
	if (scheduler->current_workload[ring_id]) {
		workload = scheduler->current_workload[ring_id];
		gvt_dbg_sched("ring id %d still have current workload %p\n",
				ring_id, workload);
=======
	if (scheduler->current_workload[engine->id]) {
		workload = scheduler->current_workload[engine->id];
		gvt_dbg_sched("ring %s still have current workload %p\n",
			      engine->name, workload);
>>>>>>> upstream/android-13
		goto out;
	}

	/*
	 * pick a workload as current workload
	 * once current workload is set, schedule policy routines
	 * will wait the current workload is finished when trying to
	 * schedule out a vgpu.
	 */
<<<<<<< HEAD
	scheduler->current_workload[ring_id] = container_of(
			workload_q_head(scheduler->current_vgpu, ring_id)->next,
			struct intel_vgpu_workload, list);

	workload = scheduler->current_workload[ring_id];

	gvt_dbg_sched("ring id %d pick new workload %p\n", ring_id, workload);
=======
	scheduler->current_workload[engine->id] =
		list_first_entry(workload_q_head(scheduler->current_vgpu,
						 engine),
				 struct intel_vgpu_workload, list);

	workload = scheduler->current_workload[engine->id];

	gvt_dbg_sched("ring %s pick new workload %p\n", engine->name, workload);
>>>>>>> upstream/android-13

	atomic_inc(&workload->vgpu->submission.running_workload_num);
out:
	mutex_unlock(&gvt->sched_lock);
	return workload;
}

<<<<<<< HEAD
=======
static void update_guest_pdps(struct intel_vgpu *vgpu,
			      u64 ring_context_gpa, u32 pdp[8])
{
	u64 gpa;
	int i;

	gpa = ring_context_gpa + RING_CTX_OFF(pdps[0].val);

	for (i = 0; i < 8; i++)
		intel_gvt_hypervisor_write_gpa(vgpu,
				gpa + i * 8, &pdp[7 - i], 4);
}

static __maybe_unused bool
check_shadow_context_ppgtt(struct execlist_ring_context *c, struct intel_vgpu_mm *m)
{
	if (m->ppgtt_mm.root_entry_type == GTT_TYPE_PPGTT_ROOT_L4_ENTRY) {
		u64 shadow_pdp = c->pdps[7].val | (u64) c->pdps[6].val << 32;

		if (shadow_pdp != m->ppgtt_mm.shadow_pdps[0]) {
			gvt_dbg_mm("4-level context ppgtt not match LRI command\n");
			return false;
		}
		return true;
	} else {
		/* see comment in LRI handler in cmd_parser.c */
		gvt_dbg_mm("invalid shadow mm type\n");
		return false;
	}
}

>>>>>>> upstream/android-13
static void update_guest_context(struct intel_vgpu_workload *workload)
{
	struct i915_request *rq = workload->req;
	struct intel_vgpu *vgpu = workload->vgpu;
<<<<<<< HEAD
	struct intel_gvt *gvt = vgpu->gvt;
	struct drm_i915_gem_object *ctx_obj = rq->hw_context->state->obj;
	struct execlist_ring_context *shadow_ring_context;
	struct page *page;
	void *src;
	unsigned long context_gpa, context_page_num;
	int i;
=======
	struct execlist_ring_context *shadow_ring_context;
	struct intel_context *ctx = workload->req->context;
	void *context_base;
	void *src;
	unsigned long context_gpa, context_page_num;
	unsigned long gpa_base; /* first gpa of consecutive GPAs */
	unsigned long gpa_size; /* size of consecutive GPAs*/
	int i;
	u32 ring_base;
	u32 head, tail;
	u16 wrap_count;
>>>>>>> upstream/android-13

	gvt_dbg_sched("ring id %d workload lrca %x\n", rq->engine->id,
		      workload->ctx_desc.lrca);

<<<<<<< HEAD
	context_page_num = rq->engine->context_size;
	context_page_num = context_page_num >> PAGE_SHIFT;

	if (IS_BROADWELL(gvt->dev_priv) && rq->engine->id == RCS)
		context_page_num = 19;

	i = 2;

	while (i < context_page_num) {
=======
	GEM_BUG_ON(!intel_context_is_pinned(ctx));

	head = workload->rb_head;
	tail = workload->rb_tail;
	wrap_count = workload->guest_rb_head >> RB_HEAD_WRAP_CNT_OFF;

	if (tail < head) {
		if (wrap_count == RB_HEAD_WRAP_CNT_MAX)
			wrap_count = 0;
		else
			wrap_count += 1;
	}

	head = (wrap_count << RB_HEAD_WRAP_CNT_OFF) | tail;

	ring_base = rq->engine->mmio_base;
	vgpu_vreg_t(vgpu, RING_TAIL(ring_base)) = tail;
	vgpu_vreg_t(vgpu, RING_HEAD(ring_base)) = head;

	context_page_num = rq->engine->context_size;
	context_page_num = context_page_num >> PAGE_SHIFT;

	if (IS_BROADWELL(rq->engine->i915) && rq->engine->id == RCS0)
		context_page_num = 19;

	context_base = (void *) ctx->lrc_reg_state -
			(LRC_STATE_PN << I915_GTT_PAGE_SHIFT);

	/* find consecutive GPAs from gma until the first inconsecutive GPA.
	 * write to the consecutive GPAs from src virtual address
	 */
	gpa_size = 0;
	for (i = 2; i < context_page_num; i++) {
>>>>>>> upstream/android-13
		context_gpa = intel_vgpu_gma_to_gpa(vgpu->gtt.ggtt_mm,
				(u32)((workload->ctx_desc.lrca + i) <<
					I915_GTT_PAGE_SHIFT));
		if (context_gpa == INTEL_GVT_INVALID_ADDR) {
			gvt_vgpu_err("invalid guest context descriptor\n");
			return;
		}

<<<<<<< HEAD
		page = i915_gem_object_get_page(ctx_obj, LRC_HEADER_PAGES + i);
		src = kmap(page);
		intel_gvt_hypervisor_write_gpa(vgpu, context_gpa, src,
				I915_GTT_PAGE_SIZE);
		kunmap(page);
		i++;
=======
		if (gpa_size == 0) {
			gpa_base = context_gpa;
			src = context_base + (i << I915_GTT_PAGE_SHIFT);
		} else if (context_gpa != gpa_base + gpa_size)
			goto write;

		gpa_size += I915_GTT_PAGE_SIZE;

		if (i == context_page_num - 1)
			goto write;

		continue;

write:
		intel_gvt_hypervisor_write_gpa(vgpu, gpa_base, src, gpa_size);
		gpa_base = context_gpa;
		gpa_size = I915_GTT_PAGE_SIZE;
		src = context_base + (i << I915_GTT_PAGE_SHIFT);
>>>>>>> upstream/android-13
	}

	intel_gvt_hypervisor_write_gpa(vgpu, workload->ring_context_gpa +
		RING_CTX_OFF(ring_header.val), &workload->rb_tail, 4);

<<<<<<< HEAD
	page = i915_gem_object_get_page(ctx_obj, LRC_STATE_PN);
	shadow_ring_context = kmap(page);
=======
	shadow_ring_context = (void *) ctx->lrc_reg_state;

	if (!list_empty(&workload->lri_shadow_mm)) {
		struct intel_vgpu_mm *m = list_last_entry(&workload->lri_shadow_mm,
							  struct intel_vgpu_mm,
							  ppgtt_mm.link);
		GEM_BUG_ON(!check_shadow_context_ppgtt(shadow_ring_context, m));
		update_guest_pdps(vgpu, workload->ring_context_gpa,
				  (void *)m->ppgtt_mm.guest_pdps);
	}
>>>>>>> upstream/android-13

#define COPY_REG(name) \
	intel_gvt_hypervisor_write_gpa(vgpu, workload->ring_context_gpa + \
		RING_CTX_OFF(name.val), &shadow_ring_context->name.val, 4)

	COPY_REG(ctx_ctrl);
	COPY_REG(ctx_timestamp);

#undef COPY_REG

	intel_gvt_hypervisor_write_gpa(vgpu,
			workload->ring_context_gpa +
			sizeof(*shadow_ring_context),
			(void *)shadow_ring_context +
			sizeof(*shadow_ring_context),
			I915_GTT_PAGE_SIZE - sizeof(*shadow_ring_context));
<<<<<<< HEAD

	kunmap(page);
}

void intel_vgpu_clean_workloads(struct intel_vgpu *vgpu,
				unsigned long engine_mask)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
	struct intel_engine_cs *engine;
	struct intel_vgpu_workload *pos, *n;
	unsigned int tmp;

	/* free the unsubmited workloads in the queues. */
	for_each_engine_masked(engine, dev_priv, engine_mask, tmp) {
=======
}

void intel_vgpu_clean_workloads(struct intel_vgpu *vgpu,
				intel_engine_mask_t engine_mask)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct intel_engine_cs *engine;
	struct intel_vgpu_workload *pos, *n;
	intel_engine_mask_t tmp;

	/* free the unsubmited workloads in the queues. */
	for_each_engine_masked(engine, vgpu->gvt->gt, engine_mask, tmp) {
>>>>>>> upstream/android-13
		list_for_each_entry_safe(pos, n,
			&s->workload_q_head[engine->id], list) {
			list_del_init(&pos->list);
			intel_vgpu_destroy_workload(pos);
		}
		clear_bit(engine->id, s->shadow_ctx_desc_updated);
	}
}

static void complete_current_workload(struct intel_gvt *gvt, int ring_id)
{
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
	struct intel_vgpu_workload *workload =
		scheduler->current_workload[ring_id];
	struct intel_vgpu *vgpu = workload->vgpu;
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct i915_request *rq = workload->req;
	int event;

	mutex_lock(&vgpu->vgpu_lock);
	mutex_lock(&gvt->sched_lock);

	/* For the workload w/ request, needs to wait for the context
	 * switch to make sure request is completed.
	 * For the workload w/o request, directly complete the workload.
	 */
	if (rq) {
		wait_event(workload->shadow_ctx_status_wq,
			   !atomic_read(&workload->shadow_ctx_active));

		/* If this request caused GPU hang, req->fence.error will
		 * be set to -EIO. Use -EIO to set workload status so
		 * that when this request caused GPU hang, didn't trigger
		 * context switch interrupt to guest.
		 */
		if (likely(workload->status == -EINPROGRESS)) {
			if (workload->req->fence.error == -EIO)
				workload->status = -EIO;
			else
				workload->status = 0;
		}

<<<<<<< HEAD
		if (!workload->status && !(vgpu->resetting_eng &
					   ENGINE_MASK(ring_id))) {
=======
		if (!workload->status &&
		    !(vgpu->resetting_eng & BIT(ring_id))) {
>>>>>>> upstream/android-13
			update_guest_context(workload);

			for_each_set_bit(event, workload->pending_events,
					 INTEL_GVT_EVENT_MAX)
				intel_vgpu_trigger_virtual_event(vgpu, event);
		}

<<<<<<< HEAD
		/* unpin shadow ctx as the shadow_ctx update is done */
		mutex_lock(&rq->i915->drm.struct_mutex);
		intel_context_unpin(rq->hw_context);
		mutex_unlock(&rq->i915->drm.struct_mutex);

=======
>>>>>>> upstream/android-13
		i915_request_put(fetch_and_zero(&workload->req));
	}

	gvt_dbg_sched("ring id %d complete workload %p status %d\n",
			ring_id, workload, workload->status);

	scheduler->current_workload[ring_id] = NULL;

	list_del_init(&workload->list);

<<<<<<< HEAD
	if (!workload->status) {
		release_shadow_batch_buffer(workload);
		release_shadow_wa_ctx(&workload->wa_ctx);
	}

	if (workload->status || (vgpu->resetting_eng & ENGINE_MASK(ring_id))) {
=======
	if (workload->status || vgpu->resetting_eng & BIT(ring_id)) {
>>>>>>> upstream/android-13
		/* if workload->status is not successful means HW GPU
		 * has occurred GPU hang or something wrong with i915/GVT,
		 * and GVT won't inject context switch interrupt to guest.
		 * So this error is a vGPU hang actually to the guest.
		 * According to this we should emunlate a vGPU hang. If
		 * there are pending workloads which are already submitted
		 * from guest, we should clean them up like HW GPU does.
		 *
		 * if it is in middle of engine resetting, the pending
		 * workloads won't be submitted to HW GPU and will be
		 * cleaned up during the resetting process later, so doing
		 * the workload clean up here doesn't have any impact.
		 **/
<<<<<<< HEAD
		intel_vgpu_clean_workloads(vgpu, ENGINE_MASK(ring_id));
=======
		intel_vgpu_clean_workloads(vgpu, BIT(ring_id));
>>>>>>> upstream/android-13
	}

	workload->complete(workload);

<<<<<<< HEAD
=======
	intel_vgpu_shadow_mm_unpin(workload);
	intel_vgpu_destroy_workload(workload);

>>>>>>> upstream/android-13
	atomic_dec(&s->running_workload_num);
	wake_up(&scheduler->workload_complete_wq);

	if (gvt->scheduler.need_reschedule)
		intel_gvt_request_service(gvt, INTEL_GVT_REQUEST_EVENT_SCHED);

	mutex_unlock(&gvt->sched_lock);
	mutex_unlock(&vgpu->vgpu_lock);
}

<<<<<<< HEAD
struct workload_thread_param {
	struct intel_gvt *gvt;
	int ring_id;
};

static int workload_thread(void *priv)
{
	struct workload_thread_param *p = (struct workload_thread_param *)priv;
	struct intel_gvt *gvt = p->gvt;
	int ring_id = p->ring_id;
=======
static int workload_thread(void *arg)
{
	struct intel_engine_cs *engine = arg;
	const bool need_force_wake = GRAPHICS_VER(engine->i915) >= 9;
	struct intel_gvt *gvt = engine->i915->gvt;
>>>>>>> upstream/android-13
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
	struct intel_vgpu_workload *workload = NULL;
	struct intel_vgpu *vgpu = NULL;
	int ret;
<<<<<<< HEAD
	bool need_force_wake = IS_SKYLAKE(gvt->dev_priv)
			|| IS_KABYLAKE(gvt->dev_priv)
			|| IS_BROXTON(gvt->dev_priv);
	DEFINE_WAIT_FUNC(wait, woken_wake_function);

	kfree(p);

	gvt_dbg_core("workload thread for ring %d started\n", ring_id);

	while (!kthread_should_stop()) {
		add_wait_queue(&scheduler->waitq[ring_id], &wait);
		do {
			workload = pick_next_workload(gvt, ring_id);
=======
	DEFINE_WAIT_FUNC(wait, woken_wake_function);

	gvt_dbg_core("workload thread for ring %s started\n", engine->name);

	while (!kthread_should_stop()) {
		intel_wakeref_t wakeref;

		add_wait_queue(&scheduler->waitq[engine->id], &wait);
		do {
			workload = pick_next_workload(gvt, engine);
>>>>>>> upstream/android-13
			if (workload)
				break;
			wait_woken(&wait, TASK_INTERRUPTIBLE,
				   MAX_SCHEDULE_TIMEOUT);
		} while (!kthread_should_stop());
<<<<<<< HEAD
		remove_wait_queue(&scheduler->waitq[ring_id], &wait);
=======
		remove_wait_queue(&scheduler->waitq[engine->id], &wait);
>>>>>>> upstream/android-13

		if (!workload)
			break;

<<<<<<< HEAD
		gvt_dbg_sched("ring id %d next workload %p vgpu %d\n",
				workload->ring_id, workload,
				workload->vgpu->id);

		intel_runtime_pm_get(gvt->dev_priv);

		gvt_dbg_sched("ring id %d will dispatch workload %p\n",
				workload->ring_id, workload);

		if (need_force_wake)
			intel_uncore_forcewake_get(gvt->dev_priv,
					FORCEWAKE_ALL);
=======
		gvt_dbg_sched("ring %s next workload %p vgpu %d\n",
			      engine->name, workload,
			      workload->vgpu->id);

		wakeref = intel_runtime_pm_get(engine->uncore->rpm);

		gvt_dbg_sched("ring %s will dispatch workload %p\n",
			      engine->name, workload);

		if (need_force_wake)
			intel_uncore_forcewake_get(engine->uncore,
						   FORCEWAKE_ALL);
		/*
		 * Update the vReg of the vGPU which submitted this
		 * workload. The vGPU may use these registers for checking
		 * the context state. The value comes from GPU commands
		 * in this workload.
		 */
		update_vreg_in_ctx(workload);
>>>>>>> upstream/android-13

		ret = dispatch_workload(workload);

		if (ret) {
			vgpu = workload->vgpu;
			gvt_vgpu_err("fail to dispatch workload, skip\n");
			goto complete;
		}

<<<<<<< HEAD
		gvt_dbg_sched("ring id %d wait workload %p\n",
				workload->ring_id, workload);
=======
		gvt_dbg_sched("ring %s wait workload %p\n",
			      engine->name, workload);
>>>>>>> upstream/android-13
		i915_request_wait(workload->req, 0, MAX_SCHEDULE_TIMEOUT);

complete:
		gvt_dbg_sched("will complete workload %p, status: %d\n",
<<<<<<< HEAD
				workload, workload->status);

		complete_current_workload(gvt, ring_id);

		if (need_force_wake)
			intel_uncore_forcewake_put(gvt->dev_priv,
					FORCEWAKE_ALL);

		intel_runtime_pm_put(gvt->dev_priv);
=======
			      workload, workload->status);

		complete_current_workload(gvt, engine->id);

		if (need_force_wake)
			intel_uncore_forcewake_put(engine->uncore,
						   FORCEWAKE_ALL);

		intel_runtime_pm_put(engine->uncore->rpm, wakeref);
>>>>>>> upstream/android-13
		if (ret && (vgpu_is_vm_unhealthy(ret)))
			enter_failsafe_mode(vgpu, GVT_FAILSAFE_GUEST_ERR);
	}
	return 0;
}

void intel_gvt_wait_vgpu_idle(struct intel_vgpu *vgpu)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct intel_gvt *gvt = vgpu->gvt;
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;

	if (atomic_read(&s->running_workload_num)) {
		gvt_dbg_sched("wait vgpu idle\n");

		wait_event(scheduler->workload_complete_wq,
				!atomic_read(&s->running_workload_num));
	}
}

void intel_gvt_clean_workload_scheduler(struct intel_gvt *gvt)
{
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
	struct intel_engine_cs *engine;
	enum intel_engine_id i;

	gvt_dbg_core("clean workload scheduler\n");

<<<<<<< HEAD
	for_each_engine(engine, gvt->dev_priv, i) {
=======
	for_each_engine(engine, gvt->gt, i) {
>>>>>>> upstream/android-13
		atomic_notifier_chain_unregister(
					&engine->context_status_notifier,
					&gvt->shadow_ctx_notifier_block[i]);
		kthread_stop(scheduler->thread[i]);
	}
}

int intel_gvt_init_workload_scheduler(struct intel_gvt *gvt)
{
	struct intel_gvt_workload_scheduler *scheduler = &gvt->scheduler;
<<<<<<< HEAD
	struct workload_thread_param *param = NULL;
=======
>>>>>>> upstream/android-13
	struct intel_engine_cs *engine;
	enum intel_engine_id i;
	int ret;

	gvt_dbg_core("init workload scheduler\n");

	init_waitqueue_head(&scheduler->workload_complete_wq);

<<<<<<< HEAD
	for_each_engine(engine, gvt->dev_priv, i) {
		init_waitqueue_head(&scheduler->waitq[i]);

		param = kzalloc(sizeof(*param), GFP_KERNEL);
		if (!param) {
			ret = -ENOMEM;
			goto err;
		}

		param->gvt = gvt;
		param->ring_id = i;

		scheduler->thread[i] = kthread_run(workload_thread, param,
			"gvt workload %d", i);
=======
	for_each_engine(engine, gvt->gt, i) {
		init_waitqueue_head(&scheduler->waitq[i]);

		scheduler->thread[i] = kthread_run(workload_thread, engine,
						   "gvt:%s", engine->name);
>>>>>>> upstream/android-13
		if (IS_ERR(scheduler->thread[i])) {
			gvt_err("fail to create workload thread\n");
			ret = PTR_ERR(scheduler->thread[i]);
			goto err;
		}

		gvt->shadow_ctx_notifier_block[i].notifier_call =
					shadow_context_status_change;
		atomic_notifier_chain_register(&engine->context_status_notifier,
					&gvt->shadow_ctx_notifier_block[i]);
	}
<<<<<<< HEAD
	return 0;
err:
	intel_gvt_clean_workload_scheduler(gvt);
	kfree(param);
	param = NULL;
	return ret;
}

=======

	return 0;

err:
	intel_gvt_clean_workload_scheduler(gvt);
	return ret;
}

static void
i915_context_ppgtt_root_restore(struct intel_vgpu_submission *s,
				struct i915_ppgtt *ppgtt)
{
	int i;

	if (i915_vm_is_4lvl(&ppgtt->vm)) {
		set_dma_address(ppgtt->pd, s->i915_context_pml4);
	} else {
		for (i = 0; i < GEN8_3LVL_PDPES; i++) {
			struct i915_page_directory * const pd =
				i915_pd_entry(ppgtt->pd, i);

			set_dma_address(pd, s->i915_context_pdps[i]);
		}
	}
}

>>>>>>> upstream/android-13
/**
 * intel_vgpu_clean_submission - free submission-related resource for vGPU
 * @vgpu: a vGPU
 *
 * This function is called when a vGPU is being destroyed.
 *
 */
void intel_vgpu_clean_submission(struct intel_vgpu *vgpu)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
<<<<<<< HEAD

	intel_vgpu_select_submission_ops(vgpu, ALL_ENGINES, 0);
	i915_gem_context_put(s->shadow_ctx);
=======
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	intel_vgpu_select_submission_ops(vgpu, ALL_ENGINES, 0);

	i915_context_ppgtt_root_restore(s, i915_vm_to_ppgtt(s->shadow[0]->vm));
	for_each_engine(engine, vgpu->gvt->gt, id)
		intel_context_put(s->shadow[id]);

>>>>>>> upstream/android-13
	kmem_cache_destroy(s->workloads);
}


/**
 * intel_vgpu_reset_submission - reset submission-related resource for vGPU
 * @vgpu: a vGPU
 * @engine_mask: engines expected to be reset
 *
 * This function is called when a vGPU is being destroyed.
 *
 */
void intel_vgpu_reset_submission(struct intel_vgpu *vgpu,
<<<<<<< HEAD
		unsigned long engine_mask)
=======
				 intel_engine_mask_t engine_mask)
>>>>>>> upstream/android-13
{
	struct intel_vgpu_submission *s = &vgpu->submission;

	if (!s->active)
		return;

	intel_vgpu_clean_workloads(vgpu, engine_mask);
	s->ops->reset(vgpu, engine_mask);
}

<<<<<<< HEAD
=======
static void
i915_context_ppgtt_root_save(struct intel_vgpu_submission *s,
			     struct i915_ppgtt *ppgtt)
{
	int i;

	if (i915_vm_is_4lvl(&ppgtt->vm)) {
		s->i915_context_pml4 = px_dma(ppgtt->pd);
	} else {
		for (i = 0; i < GEN8_3LVL_PDPES; i++) {
			struct i915_page_directory * const pd =
				i915_pd_entry(ppgtt->pd, i);

			s->i915_context_pdps[i] = px_dma(pd);
		}
	}
}

>>>>>>> upstream/android-13
/**
 * intel_vgpu_setup_submission - setup submission-related resource for vGPU
 * @vgpu: a vGPU
 *
 * This function is called when a vGPU is being created.
 *
 * Returns:
 * Zero on success, negative error code if failed.
 *
 */
int intel_vgpu_setup_submission(struct intel_vgpu *vgpu)
{
<<<<<<< HEAD
	struct intel_vgpu_submission *s = &vgpu->submission;
	enum intel_engine_id i;
	struct intel_engine_cs *engine;
	int ret;

	s->shadow_ctx = i915_gem_context_create_gvt(
			&vgpu->gvt->dev_priv->drm);
	if (IS_ERR(s->shadow_ctx))
		return PTR_ERR(s->shadow_ctx);
=======
	struct drm_i915_private *i915 = vgpu->gvt->gt->i915;
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct intel_engine_cs *engine;
	struct i915_ppgtt *ppgtt;
	enum intel_engine_id i;
	int ret;

	ppgtt = i915_ppgtt_create(&i915->gt);
	if (IS_ERR(ppgtt))
		return PTR_ERR(ppgtt);

	i915_context_ppgtt_root_save(s, ppgtt);

	for_each_engine(engine, vgpu->gvt->gt, i) {
		struct intel_context *ce;

		INIT_LIST_HEAD(&s->workload_q_head[i]);
		s->shadow[i] = ERR_PTR(-EINVAL);

		ce = intel_context_create(engine);
		if (IS_ERR(ce)) {
			ret = PTR_ERR(ce);
			goto out_shadow_ctx;
		}

		i915_vm_put(ce->vm);
		ce->vm = i915_vm_get(&ppgtt->vm);
		intel_context_set_single_submission(ce);

		/* Max ring buffer size */
		if (!intel_uc_wants_guc_submission(&engine->gt->uc))
			ce->ring_size = SZ_2M;

		s->shadow[i] = ce;
	}
>>>>>>> upstream/android-13

	bitmap_zero(s->shadow_ctx_desc_updated, I915_NUM_ENGINES);

	s->workloads = kmem_cache_create_usercopy("gvt-g_vgpu_workload",
						  sizeof(struct intel_vgpu_workload), 0,
						  SLAB_HWCACHE_ALIGN,
						  offsetof(struct intel_vgpu_workload, rb_tail),
						  sizeof_field(struct intel_vgpu_workload, rb_tail),
						  NULL);

	if (!s->workloads) {
		ret = -ENOMEM;
		goto out_shadow_ctx;
	}

<<<<<<< HEAD
	for_each_engine(engine, vgpu->gvt->dev_priv, i)
		INIT_LIST_HEAD(&s->workload_q_head[i]);

	atomic_set(&s->running_workload_num, 0);
	bitmap_zero(s->tlb_handle_pending, I915_NUM_ENGINES);

	return 0;

out_shadow_ctx:
	i915_gem_context_put(s->shadow_ctx);
=======
	atomic_set(&s->running_workload_num, 0);
	bitmap_zero(s->tlb_handle_pending, I915_NUM_ENGINES);

	memset(s->last_ctx, 0, sizeof(s->last_ctx));

	i915_vm_put(&ppgtt->vm);
	return 0;

out_shadow_ctx:
	i915_context_ppgtt_root_restore(s, ppgtt);
	for_each_engine(engine, vgpu->gvt->gt, i) {
		if (IS_ERR(s->shadow[i]))
			break;

		intel_context_put(s->shadow[i]);
	}
	i915_vm_put(&ppgtt->vm);
>>>>>>> upstream/android-13
	return ret;
}

/**
 * intel_vgpu_select_submission_ops - select virtual submission interface
 * @vgpu: a vGPU
<<<<<<< HEAD
=======
 * @engine_mask: either ALL_ENGINES or target engine mask
>>>>>>> upstream/android-13
 * @interface: expected vGPU virtual submission interface
 *
 * This function is called when guest configures submission interface.
 *
 * Returns:
 * Zero on success, negative error code if failed.
 *
 */
int intel_vgpu_select_submission_ops(struct intel_vgpu *vgpu,
<<<<<<< HEAD
				     unsigned long engine_mask,
				     unsigned int interface)
{
=======
				     intel_engine_mask_t engine_mask,
				     unsigned int interface)
{
	struct drm_i915_private *i915 = vgpu->gvt->gt->i915;
>>>>>>> upstream/android-13
	struct intel_vgpu_submission *s = &vgpu->submission;
	const struct intel_vgpu_submission_ops *ops[] = {
		[INTEL_VGPU_EXECLIST_SUBMISSION] =
			&intel_vgpu_execlist_submission_ops,
	};
	int ret;

<<<<<<< HEAD
	if (WARN_ON(interface >= ARRAY_SIZE(ops)))
		return -EINVAL;

	if (WARN_ON(interface == 0 && engine_mask != ALL_ENGINES))
=======
	if (drm_WARN_ON(&i915->drm, interface >= ARRAY_SIZE(ops)))
		return -EINVAL;

	if (drm_WARN_ON(&i915->drm,
			interface == 0 && engine_mask != ALL_ENGINES))
>>>>>>> upstream/android-13
		return -EINVAL;

	if (s->active)
		s->ops->clean(vgpu, engine_mask);

	if (interface == 0) {
		s->ops = NULL;
		s->virtual_submission_interface = 0;
		s->active = false;
		gvt_dbg_core("vgpu%d: remove submission ops\n", vgpu->id);
		return 0;
	}

	ret = ops[interface]->init(vgpu, engine_mask);
	if (ret)
		return ret;

	s->ops = ops[interface];
	s->virtual_submission_interface = interface;
	s->active = true;

	gvt_dbg_core("vgpu%d: activate ops [ %s ]\n",
			vgpu->id, s->ops->name);

	return 0;
}

/**
 * intel_vgpu_destroy_workload - destroy a vGPU workload
<<<<<<< HEAD
 * @vgpu: a vGPU
=======
 * @workload: workload to destroy
>>>>>>> upstream/android-13
 *
 * This function is called when destroy a vGPU workload.
 *
 */
void intel_vgpu_destroy_workload(struct intel_vgpu_workload *workload)
{
	struct intel_vgpu_submission *s = &workload->vgpu->submission;

<<<<<<< HEAD
=======
	intel_context_unpin(s->shadow[workload->engine->id]);
	release_shadow_batch_buffer(workload);
	release_shadow_wa_ctx(&workload->wa_ctx);

	if (!list_empty(&workload->lri_shadow_mm)) {
		struct intel_vgpu_mm *m, *mm;
		list_for_each_entry_safe(m, mm, &workload->lri_shadow_mm,
					 ppgtt_mm.link) {
			list_del(&m->ppgtt_mm.link);
			intel_vgpu_mm_put(m);
		}
	}

	GEM_BUG_ON(!list_empty(&workload->lri_shadow_mm));
>>>>>>> upstream/android-13
	if (workload->shadow_mm)
		intel_vgpu_mm_put(workload->shadow_mm);

	kmem_cache_free(s->workloads, workload);
}

static struct intel_vgpu_workload *
alloc_workload(struct intel_vgpu *vgpu)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct intel_vgpu_workload *workload;

	workload = kmem_cache_zalloc(s->workloads, GFP_KERNEL);
	if (!workload)
		return ERR_PTR(-ENOMEM);

	INIT_LIST_HEAD(&workload->list);
	INIT_LIST_HEAD(&workload->shadow_bb);
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&workload->lri_shadow_mm);
>>>>>>> upstream/android-13

	init_waitqueue_head(&workload->shadow_ctx_status_wq);
	atomic_set(&workload->shadow_ctx_active, 0);

	workload->status = -EINPROGRESS;
	workload->vgpu = vgpu;

	return workload;
}

#define RING_CTX_OFF(x) \
	offsetof(struct execlist_ring_context, x)

static void read_guest_pdps(struct intel_vgpu *vgpu,
		u64 ring_context_gpa, u32 pdp[8])
{
	u64 gpa;
	int i;

	gpa = ring_context_gpa + RING_CTX_OFF(pdps[0].val);

	for (i = 0; i < 8; i++)
		intel_gvt_hypervisor_read_gpa(vgpu,
				gpa + i * 8, &pdp[7 - i], 4);
}

static int prepare_mm(struct intel_vgpu_workload *workload)
{
	struct execlist_ctx_descriptor_format *desc = &workload->ctx_desc;
	struct intel_vgpu_mm *mm;
	struct intel_vgpu *vgpu = workload->vgpu;
<<<<<<< HEAD
	intel_gvt_gtt_type_t root_entry_type;
=======
	enum intel_gvt_gtt_type root_entry_type;
>>>>>>> upstream/android-13
	u64 pdps[GVT_RING_CTX_NR_PDPS];

	switch (desc->addressing_mode) {
	case 1: /* legacy 32-bit */
		root_entry_type = GTT_TYPE_PPGTT_ROOT_L3_ENTRY;
		break;
	case 3: /* legacy 64-bit */
		root_entry_type = GTT_TYPE_PPGTT_ROOT_L4_ENTRY;
		break;
	default:
		gvt_vgpu_err("Advanced Context mode(SVM) is not supported!\n");
		return -EINVAL;
	}

	read_guest_pdps(workload->vgpu, workload->ring_context_gpa, (void *)pdps);

	mm = intel_vgpu_get_ppgtt_mm(workload->vgpu, root_entry_type, pdps);
	if (IS_ERR(mm))
		return PTR_ERR(mm);

	workload->shadow_mm = mm;
	return 0;
}

#define same_context(a, b) (((a)->context_id == (b)->context_id) && \
		((a)->lrca == (b)->lrca))

/**
 * intel_vgpu_create_workload - create a vGPU workload
 * @vgpu: a vGPU
<<<<<<< HEAD
=======
 * @engine: the engine
>>>>>>> upstream/android-13
 * @desc: a guest context descriptor
 *
 * This function is called when creating a vGPU workload.
 *
 * Returns:
 * struct intel_vgpu_workload * on success, negative error code in
 * pointer if failed.
 *
 */
struct intel_vgpu_workload *
<<<<<<< HEAD
intel_vgpu_create_workload(struct intel_vgpu *vgpu, int ring_id,
			   struct execlist_ctx_descriptor_format *desc)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct list_head *q = workload_q_head(vgpu, ring_id);
	struct intel_vgpu_workload *last_workload = NULL;
	struct intel_vgpu_workload *workload = NULL;
	struct drm_i915_private *dev_priv = vgpu->gvt->dev_priv;
	u64 ring_context_gpa;
	u32 head, tail, start, ctl, ctx_ctl, per_ctx, indirect_ctx;
=======
intel_vgpu_create_workload(struct intel_vgpu *vgpu,
			   const struct intel_engine_cs *engine,
			   struct execlist_ctx_descriptor_format *desc)
{
	struct intel_vgpu_submission *s = &vgpu->submission;
	struct list_head *q = workload_q_head(vgpu, engine);
	struct intel_vgpu_workload *last_workload = NULL;
	struct intel_vgpu_workload *workload = NULL;
	u64 ring_context_gpa;
	u32 head, tail, start, ctl, ctx_ctl, per_ctx, indirect_ctx;
	u32 guest_head;
>>>>>>> upstream/android-13
	int ret;

	ring_context_gpa = intel_vgpu_gma_to_gpa(vgpu->gtt.ggtt_mm,
			(u32)((desc->lrca + 1) << I915_GTT_PAGE_SHIFT));
	if (ring_context_gpa == INTEL_GVT_INVALID_ADDR) {
		gvt_vgpu_err("invalid guest context LRCA: %x\n", desc->lrca);
		return ERR_PTR(-EINVAL);
	}

	intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(ring_header.val), &head, 4);

	intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(ring_tail.val), &tail, 4);

<<<<<<< HEAD
=======
	guest_head = head;

>>>>>>> upstream/android-13
	head &= RB_HEAD_OFF_MASK;
	tail &= RB_TAIL_OFF_MASK;

	list_for_each_entry_reverse(last_workload, q, list) {

		if (same_context(&last_workload->ctx_desc, desc)) {
<<<<<<< HEAD
			gvt_dbg_el("ring id %d cur workload == last\n",
					ring_id);
			gvt_dbg_el("ctx head %x real head %lx\n", head,
					last_workload->rb_tail);
=======
			gvt_dbg_el("ring %s cur workload == last\n",
				   engine->name);
			gvt_dbg_el("ctx head %x real head %lx\n", head,
				   last_workload->rb_tail);
>>>>>>> upstream/android-13
			/*
			 * cannot use guest context head pointer here,
			 * as it might not be updated at this time
			 */
			head = last_workload->rb_tail;
			break;
		}
	}

<<<<<<< HEAD
	gvt_dbg_el("ring id %d begin a new workload\n", ring_id);
=======
	gvt_dbg_el("ring %s begin a new workload\n", engine->name);
>>>>>>> upstream/android-13

	/* record some ring buffer register values for scan and shadow */
	intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(rb_start.val), &start, 4);
	intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(rb_ctrl.val), &ctl, 4);
	intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(ctx_ctrl.val), &ctx_ctl, 4);

<<<<<<< HEAD
=======
	if (!intel_gvt_ggtt_validate_range(vgpu, start,
				_RING_CTL_BUF_SIZE(ctl))) {
		gvt_vgpu_err("context contain invalid rb at: 0x%x\n", start);
		return ERR_PTR(-EINVAL);
	}

>>>>>>> upstream/android-13
	workload = alloc_workload(vgpu);
	if (IS_ERR(workload))
		return workload;

<<<<<<< HEAD
	workload->ring_id = ring_id;
	workload->ctx_desc = *desc;
	workload->ring_context_gpa = ring_context_gpa;
	workload->rb_head = head;
=======
	workload->engine = engine;
	workload->ctx_desc = *desc;
	workload->ring_context_gpa = ring_context_gpa;
	workload->rb_head = head;
	workload->guest_rb_head = guest_head;
>>>>>>> upstream/android-13
	workload->rb_tail = tail;
	workload->rb_start = start;
	workload->rb_ctl = ctl;

<<<<<<< HEAD
	if (ring_id == RCS) {
=======
	if (engine->id == RCS0) {
>>>>>>> upstream/android-13
		intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(bb_per_ctx_ptr.val), &per_ctx, 4);
		intel_gvt_hypervisor_read_gpa(vgpu, ring_context_gpa +
			RING_CTX_OFF(rcs_indirect_ctx.val), &indirect_ctx, 4);

		workload->wa_ctx.indirect_ctx.guest_gma =
			indirect_ctx & INDIRECT_CTX_ADDR_MASK;
		workload->wa_ctx.indirect_ctx.size =
			(indirect_ctx & INDIRECT_CTX_SIZE_MASK) *
			CACHELINE_BYTES;
<<<<<<< HEAD
		workload->wa_ctx.per_ctx.guest_gma =
			per_ctx & PER_CTX_ADDR_MASK;
		workload->wa_ctx.per_ctx.valid = per_ctx & 1;
	}

	gvt_dbg_el("workload %p ring id %d head %x tail %x start %x ctl %x\n",
			workload, ring_id, head, tail, start, ctl);
=======

		if (workload->wa_ctx.indirect_ctx.size != 0) {
			if (!intel_gvt_ggtt_validate_range(vgpu,
				workload->wa_ctx.indirect_ctx.guest_gma,
				workload->wa_ctx.indirect_ctx.size)) {
				gvt_vgpu_err("invalid wa_ctx at: 0x%lx\n",
				    workload->wa_ctx.indirect_ctx.guest_gma);
				kmem_cache_free(s->workloads, workload);
				return ERR_PTR(-EINVAL);
			}
		}

		workload->wa_ctx.per_ctx.guest_gma =
			per_ctx & PER_CTX_ADDR_MASK;
		workload->wa_ctx.per_ctx.valid = per_ctx & 1;
		if (workload->wa_ctx.per_ctx.valid) {
			if (!intel_gvt_ggtt_validate_range(vgpu,
				workload->wa_ctx.per_ctx.guest_gma,
				CACHELINE_BYTES)) {
				gvt_vgpu_err("invalid per_ctx at: 0x%lx\n",
					workload->wa_ctx.per_ctx.guest_gma);
				kmem_cache_free(s->workloads, workload);
				return ERR_PTR(-EINVAL);
			}
		}
	}

	gvt_dbg_el("workload %p ring %s head %x tail %x start %x ctl %x\n",
		   workload, engine->name, head, tail, start, ctl);
>>>>>>> upstream/android-13

	ret = prepare_mm(workload);
	if (ret) {
		kmem_cache_free(s->workloads, workload);
		return ERR_PTR(ret);
	}

	/* Only scan and shadow the first workload in the queue
	 * as there is only one pre-allocated buf-obj for shadow.
	 */
<<<<<<< HEAD
	if (list_empty(workload_q_head(vgpu, ring_id))) {
		intel_runtime_pm_get(dev_priv);
		mutex_lock(&dev_priv->drm.struct_mutex);
		ret = intel_gvt_scan_and_shadow_workload(workload);
		mutex_unlock(&dev_priv->drm.struct_mutex);
		intel_runtime_pm_put(dev_priv);
=======
	if (list_empty(q)) {
		intel_wakeref_t wakeref;

		with_intel_runtime_pm(engine->gt->uncore->rpm, wakeref)
			ret = intel_gvt_scan_and_shadow_workload(workload);
>>>>>>> upstream/android-13
	}

	if (ret) {
		if (vgpu_is_vm_unhealthy(ret))
			enter_failsafe_mode(vgpu, GVT_FAILSAFE_GUEST_ERR);
		intel_vgpu_destroy_workload(workload);
		return ERR_PTR(ret);
	}

<<<<<<< HEAD
=======
	ret = intel_context_pin(s->shadow[engine->id]);
	if (ret) {
		intel_vgpu_destroy_workload(workload);
		return ERR_PTR(ret);
	}

>>>>>>> upstream/android-13
	return workload;
}

/**
 * intel_vgpu_queue_workload - Qeue a vGPU workload
 * @workload: the workload to queue in
 */
void intel_vgpu_queue_workload(struct intel_vgpu_workload *workload)
{
	list_add_tail(&workload->list,
<<<<<<< HEAD
		workload_q_head(workload->vgpu, workload->ring_id));
	intel_gvt_kick_schedule(workload->vgpu->gvt);
	wake_up(&workload->vgpu->gvt->scheduler.waitq[workload->ring_id]);
=======
		      workload_q_head(workload->vgpu, workload->engine));
	intel_gvt_kick_schedule(workload->vgpu->gvt);
	wake_up(&workload->vgpu->gvt->scheduler.waitq[workload->engine->id]);
>>>>>>> upstream/android-13
}
