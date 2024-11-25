<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2014-2018 The Linux Foundation. All rights reserved.
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__
#include <linux/sort.h>
#include <linux/debugfs.h>
#include <linux/ktime.h>
<<<<<<< HEAD
#include <drm/drm_mode.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_flip_work.h>
#include <drm/drm_rect.h>
=======
#include <linux/bits.h>

#include <drm/drm_atomic.h>
#include <drm/drm_crtc.h>
#include <drm/drm_flip_work.h>
#include <drm/drm_mode.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_rect.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "dpu_kms.h"
#include "dpu_hw_lm.h"
#include "dpu_hw_ctl.h"
<<<<<<< HEAD
=======
#include "dpu_hw_dspp.h"
>>>>>>> upstream/android-13
#include "dpu_crtc.h"
#include "dpu_plane.h"
#include "dpu_encoder.h"
#include "dpu_vbif.h"
<<<<<<< HEAD
#include "dpu_power_handle.h"
#include "dpu_core_perf.h"
#include "dpu_trace.h"

#define DPU_DRM_BLEND_OP_NOT_DEFINED    0
#define DPU_DRM_BLEND_OP_OPAQUE         1
#define DPU_DRM_BLEND_OP_PREMULTIPLIED  2
#define DPU_DRM_BLEND_OP_COVERAGE       3
#define DPU_DRM_BLEND_OP_MAX            4

=======
#include "dpu_core_perf.h"
#include "dpu_trace.h"

>>>>>>> upstream/android-13
/* layer mixer index on dpu_crtc */
#define LEFT_MIXER 0
#define RIGHT_MIXER 1

<<<<<<< HEAD
#define MISR_BUFF_SIZE			256

static inline struct dpu_kms *_dpu_crtc_get_kms(struct drm_crtc *crtc)
{
	struct msm_drm_private *priv;

	if (!crtc || !crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid crtc\n");
		return NULL;
	}
	priv = crtc->dev->dev_private;
	if (!priv || !priv->kms) {
		DPU_ERROR("invalid kms\n");
		return NULL;
	}
=======
/* timeout in ms waiting for frame done */
#define DPU_CRTC_FRAME_DONE_TIMEOUT_MS	60

#define	CONVERT_S3_15(val) \
	(((((u64)val) & ~BIT_ULL(63)) >> 17) & GENMASK_ULL(17, 0))

static struct dpu_kms *_dpu_crtc_get_kms(struct drm_crtc *crtc)
{
	struct msm_drm_private *priv = crtc->dev->dev_private;
>>>>>>> upstream/android-13

	return to_dpu_kms(priv->kms);
}

<<<<<<< HEAD
static inline int _dpu_crtc_power_enable(struct dpu_crtc *dpu_crtc, bool enable)
{
	struct drm_crtc *crtc;
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms;

	if (!dpu_crtc) {
		DPU_ERROR("invalid dpu crtc\n");
		return -EINVAL;
	}

	crtc = &dpu_crtc->base;
	if (!crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid drm device\n");
		return -EINVAL;
	}

	priv = crtc->dev->dev_private;
	if (!priv->kms) {
		DPU_ERROR("invalid kms\n");
		return -EINVAL;
	}

	dpu_kms = to_dpu_kms(priv->kms);

	if (enable)
		pm_runtime_get_sync(&dpu_kms->pdev->dev);
	else
		pm_runtime_put_sync(&dpu_kms->pdev->dev);

	return 0;
}

/**
 * _dpu_crtc_rp_to_crtc - get crtc from resource pool object
 * @rp: Pointer to resource pool
 * return: Pointer to drm crtc if success; null otherwise
 */
static struct drm_crtc *_dpu_crtc_rp_to_crtc(struct dpu_crtc_respool *rp)
{
	if (!rp)
		return NULL;

	return container_of(rp, struct dpu_crtc_state, rp)->base.crtc;
}

/**
 * _dpu_crtc_rp_reclaim - reclaim unused, or all if forced, resources in pool
 * @rp: Pointer to resource pool
 * @force: True to reclaim all resources; otherwise, reclaim only unused ones
 * return: None
 */
static void _dpu_crtc_rp_reclaim(struct dpu_crtc_respool *rp, bool force)
{
	struct dpu_crtc_res *res, *next;
	struct drm_crtc *crtc;

	crtc = _dpu_crtc_rp_to_crtc(rp);
	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	DPU_DEBUG("crtc%d.%u %s\n", crtc->base.id, rp->sequence_id,
			force ? "destroy" : "free_unused");

	list_for_each_entry_safe(res, next, &rp->res_list, list) {
		if (!force && !(res->flags & DPU_CRTC_RES_FLAG_FREE))
			continue;
		DPU_DEBUG("crtc%d.%u reclaim res:0x%x/0x%llx/%pK/%d\n",
				crtc->base.id, rp->sequence_id,
				res->type, res->tag, res->val,
				atomic_read(&res->refcount));
		list_del(&res->list);
		if (res->ops.put)
			res->ops.put(res->val);
		kfree(res);
	}
}

/**
 * _dpu_crtc_rp_free_unused - free unused resource in pool
 * @rp: Pointer to resource pool
 * return: none
 */
static void _dpu_crtc_rp_free_unused(struct dpu_crtc_respool *rp)
{
	mutex_lock(rp->rp_lock);
	_dpu_crtc_rp_reclaim(rp, false);
	mutex_unlock(rp->rp_lock);
}

/**
 * _dpu_crtc_rp_destroy - destroy resource pool
 * @rp: Pointer to resource pool
 * return: None
 */
static void _dpu_crtc_rp_destroy(struct dpu_crtc_respool *rp)
{
	mutex_lock(rp->rp_lock);
	list_del_init(&rp->rp_list);
	_dpu_crtc_rp_reclaim(rp, true);
	mutex_unlock(rp->rp_lock);
}

/**
 * _dpu_crtc_hw_blk_get - get callback for hardware block
 * @val: Resource handle
 * @type: Resource type
 * @tag: Search tag for given resource
 * return: Resource handle
 */
static void *_dpu_crtc_hw_blk_get(void *val, u32 type, u64 tag)
{
	DPU_DEBUG("res:%d/0x%llx/%pK\n", type, tag, val);
	return dpu_hw_blk_get(val, type, tag);
}

/**
 * _dpu_crtc_hw_blk_put - put callback for hardware block
 * @val: Resource handle
 * return: None
 */
static void _dpu_crtc_hw_blk_put(void *val)
{
	DPU_DEBUG("res://%pK\n", val);
	dpu_hw_blk_put(val);
}

/**
 * _dpu_crtc_rp_duplicate - duplicate resource pool and reset reference count
 * @rp: Pointer to original resource pool
 * @dup_rp: Pointer to duplicated resource pool
 * return: None
 */
static void _dpu_crtc_rp_duplicate(struct dpu_crtc_respool *rp,
		struct dpu_crtc_respool *dup_rp)
{
	struct dpu_crtc_res *res, *dup_res;
	struct drm_crtc *crtc;

	if (!rp || !dup_rp || !rp->rp_head) {
		DPU_ERROR("invalid resource pool\n");
		return;
	}

	crtc = _dpu_crtc_rp_to_crtc(rp);
	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	DPU_DEBUG("crtc%d.%u duplicate\n", crtc->base.id, rp->sequence_id);

	mutex_lock(rp->rp_lock);
	dup_rp->sequence_id = rp->sequence_id + 1;
	INIT_LIST_HEAD(&dup_rp->res_list);
	dup_rp->ops = rp->ops;
	list_for_each_entry(res, &rp->res_list, list) {
		dup_res = kzalloc(sizeof(struct dpu_crtc_res), GFP_KERNEL);
		if (!dup_res) {
			mutex_unlock(rp->rp_lock);
			return;
		}
		INIT_LIST_HEAD(&dup_res->list);
		atomic_set(&dup_res->refcount, 0);
		dup_res->type = res->type;
		dup_res->tag = res->tag;
		dup_res->val = res->val;
		dup_res->ops = res->ops;
		dup_res->flags = DPU_CRTC_RES_FLAG_FREE;
		DPU_DEBUG("crtc%d.%u dup res:0x%x/0x%llx/%pK/%d\n",
				crtc->base.id, dup_rp->sequence_id,
				dup_res->type, dup_res->tag, dup_res->val,
				atomic_read(&dup_res->refcount));
		list_add_tail(&dup_res->list, &dup_rp->res_list);
		if (dup_res->ops.get)
			dup_res->ops.get(dup_res->val, 0, -1);
	}

	dup_rp->rp_lock = rp->rp_lock;
	dup_rp->rp_head = rp->rp_head;
	INIT_LIST_HEAD(&dup_rp->rp_list);
	list_add_tail(&dup_rp->rp_list, rp->rp_head);
	mutex_unlock(rp->rp_lock);
}

/**
 * _dpu_crtc_rp_reset - reset resource pool after allocation
 * @rp: Pointer to original resource pool
 * @rp_lock: Pointer to serialization resource pool lock
 * @rp_head: Pointer to crtc resource pool head
 * return: None
 */
static void _dpu_crtc_rp_reset(struct dpu_crtc_respool *rp,
		struct mutex *rp_lock, struct list_head *rp_head)
{
	if (!rp || !rp_lock || !rp_head) {
		DPU_ERROR("invalid resource pool\n");
		return;
	}

	mutex_lock(rp_lock);
	rp->rp_lock = rp_lock;
	rp->rp_head = rp_head;
	INIT_LIST_HEAD(&rp->rp_list);
	rp->sequence_id = 0;
	INIT_LIST_HEAD(&rp->res_list);
	rp->ops.get = _dpu_crtc_hw_blk_get;
	rp->ops.put = _dpu_crtc_hw_blk_put;
	list_add_tail(&rp->rp_list, rp->rp_head);
	mutex_unlock(rp_lock);
}

=======
>>>>>>> upstream/android-13
static void dpu_crtc_destroy(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);

<<<<<<< HEAD
	DPU_DEBUG("\n");

	if (!crtc)
		return;

	dpu_crtc->phandle = NULL;

	drm_crtc_cleanup(crtc);
	mutex_destroy(&dpu_crtc->crtc_lock);
	kfree(dpu_crtc);
}

static void _dpu_crtc_setup_blend_cfg(struct dpu_crtc_mixer *mixer,
		struct dpu_plane_state *pstate)
{
	struct dpu_hw_mixer *lm = mixer->hw_lm;

	/* default to opaque blending */
	lm->ops.setup_blend_config(lm, pstate->stage, 0XFF, 0,
				DPU_BLEND_FG_ALPHA_FG_CONST |
				DPU_BLEND_BG_ALPHA_BG_CONST);
=======
	if (!crtc)
		return;

	drm_crtc_cleanup(crtc);
	kfree(dpu_crtc);
}

static struct drm_encoder *get_encoder_from_crtc(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_encoder *encoder;

	drm_for_each_encoder(encoder, dev)
		if (encoder->crtc == crtc)
			return encoder;

	return NULL;
}

static u32 dpu_crtc_get_vblank_counter(struct drm_crtc *crtc)
{
	struct drm_encoder *encoder;

	encoder = get_encoder_from_crtc(crtc);
	if (!encoder) {
		DRM_ERROR("no encoder found for crtc %d\n", crtc->index);
		return false;
	}

	return dpu_encoder_get_frame_count(encoder);
}

static bool dpu_crtc_get_scanout_position(struct drm_crtc *crtc,
					   bool in_vblank_irq,
					   int *vpos, int *hpos,
					   ktime_t *stime, ktime_t *etime,
					   const struct drm_display_mode *mode)
{
	unsigned int pipe = crtc->index;
	struct drm_encoder *encoder;
	int line, vsw, vbp, vactive_start, vactive_end, vfp_end;

	encoder = get_encoder_from_crtc(crtc);
	if (!encoder) {
		DRM_ERROR("no encoder found for crtc %d\n", pipe);
		return false;
	}

	vsw = mode->crtc_vsync_end - mode->crtc_vsync_start;
	vbp = mode->crtc_vtotal - mode->crtc_vsync_end;

	/*
	 * the line counter is 1 at the start of the VSYNC pulse and VTOTAL at
	 * the end of VFP. Translate the porch values relative to the line
	 * counter positions.
	 */

	vactive_start = vsw + vbp + 1;
	vactive_end = vactive_start + mode->crtc_vdisplay;

	/* last scan line before VSYNC */
	vfp_end = mode->crtc_vtotal;

	if (stime)
		*stime = ktime_get();

	line = dpu_encoder_get_linecount(encoder);

	if (line < vactive_start)
		line -= vactive_start;
	else if (line > vactive_end)
		line = line - vfp_end - vactive_start;
	else
		line -= vactive_start;

	*vpos = line;
	*hpos = 0;

	if (etime)
		*etime = ktime_get();

	return true;
}

static void _dpu_crtc_setup_blend_cfg(struct dpu_crtc_mixer *mixer,
		struct dpu_plane_state *pstate, struct dpu_format *format)
{
	struct dpu_hw_mixer *lm = mixer->hw_lm;
	uint32_t blend_op;
	uint32_t fg_alpha, bg_alpha;

	fg_alpha = pstate->base.alpha >> 8;
	bg_alpha = 0xff - fg_alpha;

	/* default to opaque blending */
	if (pstate->base.pixel_blend_mode == DRM_MODE_BLEND_PIXEL_NONE ||
	    !format->alpha_enable) {
		blend_op = DPU_BLEND_FG_ALPHA_FG_CONST |
			DPU_BLEND_BG_ALPHA_BG_CONST;
	} else if (pstate->base.pixel_blend_mode == DRM_MODE_BLEND_PREMULTI) {
		blend_op = DPU_BLEND_FG_ALPHA_FG_CONST |
			DPU_BLEND_BG_ALPHA_FG_PIXEL;
		if (fg_alpha != 0xff) {
			bg_alpha = fg_alpha;
			blend_op |= DPU_BLEND_BG_MOD_ALPHA |
				    DPU_BLEND_BG_INV_MOD_ALPHA;
		} else {
			blend_op |= DPU_BLEND_BG_INV_ALPHA;
		}
	} else {
		/* coverage blending */
		blend_op = DPU_BLEND_FG_ALPHA_FG_PIXEL |
			DPU_BLEND_BG_ALPHA_FG_PIXEL;
		if (fg_alpha != 0xff) {
			bg_alpha = fg_alpha;
			blend_op |= DPU_BLEND_FG_MOD_ALPHA |
				    DPU_BLEND_FG_INV_MOD_ALPHA |
				    DPU_BLEND_BG_MOD_ALPHA |
				    DPU_BLEND_BG_INV_MOD_ALPHA;
		} else {
			blend_op |= DPU_BLEND_BG_INV_ALPHA;
		}
	}

	lm->ops.setup_blend_config(lm, pstate->stage,
				fg_alpha, bg_alpha, blend_op);

	DRM_DEBUG_ATOMIC("format:%p4cc, alpha_en:%u blend_op:0x%x\n",
		  &format->base.pixel_format, format->alpha_enable, blend_op);
>>>>>>> upstream/android-13
}

static void _dpu_crtc_program_lm_output_roi(struct drm_crtc *crtc)
{
<<<<<<< HEAD
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *crtc_state;
	int lm_idx, lm_horiz_position;

	dpu_crtc = to_dpu_crtc(crtc);
	crtc_state = to_dpu_crtc_state(crtc->state);

	lm_horiz_position = 0;
	for (lm_idx = 0; lm_idx < dpu_crtc->num_mixers; lm_idx++) {
		const struct drm_rect *lm_roi = &crtc_state->lm_bounds[lm_idx];
		struct dpu_hw_mixer *hw_lm = dpu_crtc->mixers[lm_idx].hw_lm;
=======
	struct dpu_crtc_state *crtc_state;
	int lm_idx, lm_horiz_position;

	crtc_state = to_dpu_crtc_state(crtc->state);

	lm_horiz_position = 0;
	for (lm_idx = 0; lm_idx < crtc_state->num_mixers; lm_idx++) {
		const struct drm_rect *lm_roi = &crtc_state->lm_bounds[lm_idx];
		struct dpu_hw_mixer *hw_lm = crtc_state->mixers[lm_idx].hw_lm;
>>>>>>> upstream/android-13
		struct dpu_hw_mixer_cfg cfg;

		if (!lm_roi || !drm_rect_visible(lm_roi))
			continue;

		cfg.out_width = drm_rect_width(lm_roi);
		cfg.out_height = drm_rect_height(lm_roi);
		cfg.right_mixer = lm_horiz_position++;
		cfg.flags = 0;
		hw_lm->ops.setup_mixer_out(hw_lm, &cfg);
	}
}

static void _dpu_crtc_blend_setup_mixer(struct drm_crtc *crtc,
	struct dpu_crtc *dpu_crtc, struct dpu_crtc_mixer *mixer)
{
	struct drm_plane *plane;
	struct drm_framebuffer *fb;
	struct drm_plane_state *state;
<<<<<<< HEAD
	struct dpu_crtc_state *cstate;
	struct dpu_plane_state *pstate = NULL;
	struct dpu_format *format;
	struct dpu_hw_ctl *ctl;
	struct dpu_hw_mixer *lm;
	struct dpu_hw_stage_cfg *stage_cfg;
=======
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc->state);
	struct dpu_plane_state *pstate = NULL;
	struct dpu_format *format;
	struct dpu_hw_ctl *ctl = mixer->lm_ctl;
	struct dpu_hw_stage_cfg *stage_cfg = &dpu_crtc->stage_cfg;
>>>>>>> upstream/android-13

	u32 flush_mask;
	uint32_t stage_idx, lm_idx;
	int zpos_cnt[DPU_STAGE_MAX + 1] = { 0 };
	bool bg_alpha_enable = false;
<<<<<<< HEAD

	if (!dpu_crtc || !mixer) {
		DPU_ERROR("invalid dpu_crtc or mixer\n");
		return;
	}

	ctl = mixer->hw_ctl;
	lm = mixer->hw_lm;
	stage_cfg = &dpu_crtc->stage_cfg;
	cstate = to_dpu_crtc_state(crtc->state);

=======
	DECLARE_BITMAP(fetch_active, SSPP_MAX);

	memset(fetch_active, 0, sizeof(fetch_active));
>>>>>>> upstream/android-13
	drm_atomic_crtc_for_each_plane(plane, crtc) {
		state = plane->state;
		if (!state)
			continue;

		pstate = to_dpu_plane_state(state);
		fb = state->fb;

		dpu_plane_get_ctl_flush(plane, ctl, &flush_mask);
<<<<<<< HEAD

		DPU_DEBUG("crtc %d stage:%d - plane %d sspp %d fb %d\n",
=======
		set_bit(dpu_plane_pipe(plane), fetch_active);

		DRM_DEBUG_ATOMIC("crtc %d stage:%d - plane %d sspp %d fb %d\n",
>>>>>>> upstream/android-13
				crtc->base.id,
				pstate->stage,
				plane->base.id,
				dpu_plane_pipe(plane) - SSPP_VIG0,
				state->fb ? state->fb->base.id : -1);

		format = to_dpu_format(msm_framebuffer_format(pstate->base.fb));
<<<<<<< HEAD
		if (!format) {
			DPU_ERROR("invalid format\n");
			return;
		}
=======
>>>>>>> upstream/android-13

		if (pstate->stage == DPU_STAGE_BASE && format->alpha_enable)
			bg_alpha_enable = true;

		stage_idx = zpos_cnt[pstate->stage]++;
		stage_cfg->stage[pstate->stage][stage_idx] =
					dpu_plane_pipe(plane);
		stage_cfg->multirect_index[pstate->stage][stage_idx] =
					pstate->multirect_index;

		trace_dpu_crtc_setup_mixer(DRMID(crtc), DRMID(plane),
					   state, pstate, stage_idx,
					   dpu_plane_pipe(plane) - SSPP_VIG0,
					   format->base.pixel_format,
					   fb ? fb->modifier : 0);

		/* blend config update */
<<<<<<< HEAD
		for (lm_idx = 0; lm_idx < dpu_crtc->num_mixers; lm_idx++) {
			_dpu_crtc_setup_blend_cfg(mixer + lm_idx, pstate);
=======
		for (lm_idx = 0; lm_idx < cstate->num_mixers; lm_idx++) {
			_dpu_crtc_setup_blend_cfg(mixer + lm_idx,
						pstate, format);
>>>>>>> upstream/android-13

			mixer[lm_idx].flush_mask |= flush_mask;

			if (bg_alpha_enable && !format->alpha_enable)
				mixer[lm_idx].mixer_op_mode = 0;
			else
				mixer[lm_idx].mixer_op_mode |=
						1 << pstate->stage;
		}
	}

<<<<<<< HEAD
=======
	if (ctl->ops.set_active_pipes)
		ctl->ops.set_active_pipes(ctl, fetch_active);

>>>>>>> upstream/android-13
	 _dpu_crtc_program_lm_output_roi(crtc);
}

/**
 * _dpu_crtc_blend_setup - configure crtc mixers
 * @crtc: Pointer to drm crtc structure
 */
static void _dpu_crtc_blend_setup(struct drm_crtc *crtc)
{
<<<<<<< HEAD
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *dpu_crtc_state;
	struct dpu_crtc_mixer *mixer;
	struct dpu_hw_ctl *ctl;
	struct dpu_hw_mixer *lm;

	int i;

	if (!crtc)
		return;

	dpu_crtc = to_dpu_crtc(crtc);
	dpu_crtc_state = to_dpu_crtc_state(crtc->state);
	mixer = dpu_crtc->mixers;

	DPU_DEBUG("%s\n", dpu_crtc->name);

	if (dpu_crtc->num_mixers > CRTC_DUAL_MIXERS) {
		DPU_ERROR("invalid number mixers: %d\n", dpu_crtc->num_mixers);
		return;
	}

	for (i = 0; i < dpu_crtc->num_mixers; i++) {
		if (!mixer[i].hw_lm || !mixer[i].hw_ctl) {
			DPU_ERROR("invalid lm or ctl assigned to mixer\n");
			return;
		}
		mixer[i].mixer_op_mode = 0;
		mixer[i].flush_mask = 0;
		if (mixer[i].hw_ctl->ops.clear_all_blendstages)
			mixer[i].hw_ctl->ops.clear_all_blendstages(
					mixer[i].hw_ctl);
=======
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc->state);
	struct dpu_crtc_mixer *mixer = cstate->mixers;
	struct dpu_hw_ctl *ctl;
	struct dpu_hw_mixer *lm;
	int i;

	DRM_DEBUG_ATOMIC("%s\n", dpu_crtc->name);

	for (i = 0; i < cstate->num_mixers; i++) {
		mixer[i].mixer_op_mode = 0;
		mixer[i].flush_mask = 0;
		if (mixer[i].lm_ctl->ops.clear_all_blendstages)
			mixer[i].lm_ctl->ops.clear_all_blendstages(
					mixer[i].lm_ctl);
>>>>>>> upstream/android-13
	}

	/* initialize stage cfg */
	memset(&dpu_crtc->stage_cfg, 0, sizeof(struct dpu_hw_stage_cfg));

	_dpu_crtc_blend_setup_mixer(crtc, dpu_crtc, mixer);

<<<<<<< HEAD
	for (i = 0; i < dpu_crtc->num_mixers; i++) {
		ctl = mixer[i].hw_ctl;
=======
	for (i = 0; i < cstate->num_mixers; i++) {
		ctl = mixer[i].lm_ctl;
>>>>>>> upstream/android-13
		lm = mixer[i].hw_lm;

		lm->ops.setup_alpha_out(lm, mixer[i].mixer_op_mode);

		mixer[i].flush_mask |= ctl->ops.get_bitmask_mixer(ctl,
			mixer[i].hw_lm->idx);

		/* stage config flush mask */
		ctl->ops.update_pending_flush(ctl, mixer[i].flush_mask);

<<<<<<< HEAD
		DPU_DEBUG("lm %d, op_mode 0x%X, ctl %d, flush mask 0x%x\n",
=======
		DRM_DEBUG_ATOMIC("lm %d, op_mode 0x%X, ctl %d, flush mask 0x%x\n",
>>>>>>> upstream/android-13
			mixer[i].hw_lm->idx - LM_0,
			mixer[i].mixer_op_mode,
			ctl->idx - CTL_0,
			mixer[i].flush_mask);

		ctl->ops.setup_blendstage(ctl, mixer[i].hw_lm->idx,
			&dpu_crtc->stage_cfg);
	}
}

/**
 *  _dpu_crtc_complete_flip - signal pending page_flip events
 * Any pending vblank events are added to the vblank_event_list
 * so that the next vblank interrupt shall signal them.
 * However PAGE_FLIP events are not handled through the vblank_event_list.
 * This API signals any pending PAGE_FLIP events requested through
 * DRM_IOCTL_MODE_PAGE_FLIP and are cached in the dpu_crtc->event.
 * @crtc: Pointer to drm crtc structure
 */
static void _dpu_crtc_complete_flip(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct drm_device *dev = crtc->dev;
	unsigned long flags;

	spin_lock_irqsave(&dev->event_lock, flags);
	if (dpu_crtc->event) {
		DRM_DEBUG_VBL("%s: send event: %pK\n", dpu_crtc->name,
			      dpu_crtc->event);
		trace_dpu_crtc_complete_flip(DRMID(crtc));
		drm_crtc_send_vblank_event(crtc, dpu_crtc->event);
		dpu_crtc->event = NULL;
	}
	spin_unlock_irqrestore(&dev->event_lock, flags);
}

enum dpu_intf_mode dpu_crtc_get_intf_mode(struct drm_crtc *crtc)
{
	struct drm_encoder *encoder;

<<<<<<< HEAD
	if (!crtc || !crtc->dev) {
		DPU_ERROR("invalid crtc\n");
		return INTF_MODE_NONE;
	}

	drm_for_each_encoder(encoder, crtc->dev)
		if (encoder->crtc == crtc)
			return dpu_encoder_get_intf_mode(encoder);
=======
	/*
	 * TODO: This function is called from dpu debugfs and as part of atomic
	 * check. When called from debugfs, the crtc->mutex must be held to
	 * read crtc->state. However reading crtc->state from atomic check isn't
	 * allowed (unless you have a good reason, a big comment, and a deep
	 * understanding of how the atomic/modeset locks work (<- and this is
	 * probably not possible)). So we'll keep the WARN_ON here for now, but
	 * really we need to figure out a better way to track our operating mode
	 */
	WARN_ON(!drm_modeset_is_locked(&crtc->mutex));

	/* TODO: Returns the first INTF_MODE, could there be multiple values? */
	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask)
		return dpu_encoder_get_intf_mode(encoder);
>>>>>>> upstream/android-13

	return INTF_MODE_NONE;
}

<<<<<<< HEAD
static void dpu_crtc_vblank_cb(void *data)
{
	struct drm_crtc *crtc = (struct drm_crtc *)data;
=======
void dpu_crtc_vblank_callback(struct drm_crtc *crtc)
{
>>>>>>> upstream/android-13
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);

	/* keep statistics on vblank callback - with auto reset via debugfs */
	if (ktime_compare(dpu_crtc->vblank_cb_time, ktime_set(0, 0)) == 0)
		dpu_crtc->vblank_cb_time = ktime_get();
	else
		dpu_crtc->vblank_cb_count++;
<<<<<<< HEAD
	_dpu_crtc_complete_flip(crtc);
=======
>>>>>>> upstream/android-13
	drm_crtc_handle_vblank(crtc);
	trace_dpu_crtc_vblank_cb(DRMID(crtc));
}

static void dpu_crtc_frame_event_work(struct kthread_work *work)
{
<<<<<<< HEAD
	struct msm_drm_private *priv;
	struct dpu_crtc_frame_event *fevent;
	struct drm_crtc *crtc;
	struct dpu_crtc *dpu_crtc;
	struct dpu_kms *dpu_kms;
	unsigned long flags;
	bool frame_done = false;

	if (!work) {
		DPU_ERROR("invalid work handle\n");
		return;
	}

	fevent = container_of(work, struct dpu_crtc_frame_event, work);
	if (!fevent->crtc || !fevent->crtc->state) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	crtc = fevent->crtc;
	dpu_crtc = to_dpu_crtc(crtc);

	dpu_kms = _dpu_crtc_get_kms(crtc);
	if (!dpu_kms) {
		DPU_ERROR("invalid kms handle\n");
		return;
	}
	priv = dpu_kms->dev->dev_private;
	DPU_ATRACE_BEGIN("crtc_frame_event");

	DRM_DEBUG_KMS("crtc%d event:%u ts:%lld\n", crtc->base.id, fevent->event,
=======
	struct dpu_crtc_frame_event *fevent = container_of(work,
			struct dpu_crtc_frame_event, work);
	struct drm_crtc *crtc = fevent->crtc;
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	unsigned long flags;
	bool frame_done = false;

	DPU_ATRACE_BEGIN("crtc_frame_event");

	DRM_DEBUG_ATOMIC("crtc%d event:%u ts:%lld\n", crtc->base.id, fevent->event,
>>>>>>> upstream/android-13
			ktime_to_ns(fevent->ts));

	if (fevent->event & (DPU_ENCODER_FRAME_EVENT_DONE
				| DPU_ENCODER_FRAME_EVENT_ERROR
				| DPU_ENCODER_FRAME_EVENT_PANEL_DEAD)) {

		if (atomic_read(&dpu_crtc->frame_pending) < 1) {
<<<<<<< HEAD
			/* this should not happen */
			DRM_ERROR("crtc%d ev:%u ts:%lld frame_pending:%d\n",
					crtc->base.id,
					fevent->event,
					ktime_to_ns(fevent->ts),
					atomic_read(&dpu_crtc->frame_pending));
=======
			/* ignore vblank when not pending */
>>>>>>> upstream/android-13
		} else if (atomic_dec_return(&dpu_crtc->frame_pending) == 0) {
			/* release bandwidth and other resources */
			trace_dpu_crtc_frame_event_done(DRMID(crtc),
							fevent->event);
			dpu_core_perf_crtc_release_bw(crtc);
		} else {
			trace_dpu_crtc_frame_event_more_pending(DRMID(crtc),
								fevent->event);
		}

<<<<<<< HEAD
		if (fevent->event & DPU_ENCODER_FRAME_EVENT_DONE)
			dpu_core_perf_crtc_update(crtc, 0, false);

=======
>>>>>>> upstream/android-13
		if (fevent->event & (DPU_ENCODER_FRAME_EVENT_DONE
					| DPU_ENCODER_FRAME_EVENT_ERROR))
			frame_done = true;
	}

	if (fevent->event & DPU_ENCODER_FRAME_EVENT_PANEL_DEAD)
		DPU_ERROR("crtc%d ts:%lld received panel dead event\n",
				crtc->base.id, ktime_to_ns(fevent->ts));

	if (frame_done)
		complete_all(&dpu_crtc->frame_done_comp);

	spin_lock_irqsave(&dpu_crtc->spin_lock, flags);
	list_add_tail(&fevent->list, &dpu_crtc->frame_event_list);
	spin_unlock_irqrestore(&dpu_crtc->spin_lock, flags);
	DPU_ATRACE_END("crtc_frame_event");
}

/*
 * dpu_crtc_frame_event_cb - crtc frame event callback API. CRTC module
 * registers this API to encoder for all frame event callbacks like
 * frame_error, frame_done, idle_timeout, etc. Encoder may call different events
 * from different context - IRQ, user thread, commit_thread, etc. Each event
 * should be carefully reviewed and should be processed in proper task context
 * to avoid schedulin delay or properly manage the irq context's bottom half
 * processing.
 */
static void dpu_crtc_frame_event_cb(void *data, u32 event)
{
	struct drm_crtc *crtc = (struct drm_crtc *)data;
	struct dpu_crtc *dpu_crtc;
	struct msm_drm_private *priv;
	struct dpu_crtc_frame_event *fevent;
	unsigned long flags;
	u32 crtc_id;

<<<<<<< HEAD
	if (!crtc || !crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid parameters\n");
		return;
	}

=======
>>>>>>> upstream/android-13
	/* Nothing to do on idle event */
	if (event & DPU_ENCODER_FRAME_EVENT_IDLE)
		return;

	dpu_crtc = to_dpu_crtc(crtc);
	priv = crtc->dev->dev_private;
	crtc_id = drm_crtc_index(crtc);

	trace_dpu_crtc_frame_event_cb(DRMID(crtc), event);

	spin_lock_irqsave(&dpu_crtc->spin_lock, flags);
	fevent = list_first_entry_or_null(&dpu_crtc->frame_event_list,
			struct dpu_crtc_frame_event, list);
	if (fevent)
		list_del_init(&fevent->list);
	spin_unlock_irqrestore(&dpu_crtc->spin_lock, flags);

	if (!fevent) {
		DRM_ERROR_RATELIMITED("crtc%d event %d overflow\n", crtc->base.id, event);
		return;
	}

	fevent->event = event;
	fevent->crtc = crtc;
	fevent->ts = ktime_get();
<<<<<<< HEAD
	kthread_queue_work(&priv->event_thread[crtc_id].worker, &fevent->work);
}

void dpu_crtc_complete_commit(struct drm_crtc *crtc,
		struct drm_crtc_state *old_state)
{
	if (!crtc || !crtc->state) {
		DPU_ERROR("invalid crtc\n");
		return;
	}
	trace_dpu_crtc_complete_commit(DRMID(crtc));
}

static void _dpu_crtc_setup_mixer_for_encoder(
		struct drm_crtc *crtc,
		struct drm_encoder *enc)
{
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct dpu_kms *dpu_kms = _dpu_crtc_get_kms(crtc);
	struct dpu_rm *rm = &dpu_kms->rm;
	struct dpu_crtc_mixer *mixer;
	struct dpu_hw_ctl *last_valid_ctl = NULL;
	int i;
	struct dpu_rm_hw_iter lm_iter, ctl_iter;

	dpu_rm_init_hw_iter(&lm_iter, enc->base.id, DPU_HW_BLK_LM);
	dpu_rm_init_hw_iter(&ctl_iter, enc->base.id, DPU_HW_BLK_CTL);

	/* Set up all the mixers and ctls reserved by this encoder */
	for (i = dpu_crtc->num_mixers; i < ARRAY_SIZE(dpu_crtc->mixers); i++) {
		mixer = &dpu_crtc->mixers[i];

		if (!dpu_rm_get_hw(rm, &lm_iter))
			break;
		mixer->hw_lm = (struct dpu_hw_mixer *)lm_iter.hw;

		/* CTL may be <= LMs, if <, multiple LMs controlled by 1 CTL */
		if (!dpu_rm_get_hw(rm, &ctl_iter)) {
			DPU_DEBUG("no ctl assigned to lm %d, using previous\n",
					mixer->hw_lm->idx - LM_0);
			mixer->hw_ctl = last_valid_ctl;
		} else {
			mixer->hw_ctl = (struct dpu_hw_ctl *)ctl_iter.hw;
			last_valid_ctl = mixer->hw_ctl;
		}

		/* Shouldn't happen, mixers are always >= ctls */
		if (!mixer->hw_ctl) {
			DPU_ERROR("no valid ctls found for lm %d\n",
					mixer->hw_lm->idx - LM_0);
			return;
		}

		mixer->encoder = enc;

		dpu_crtc->num_mixers++;
		DPU_DEBUG("setup mixer %d: lm %d\n",
				i, mixer->hw_lm->idx - LM_0);
		DPU_DEBUG("setup mixer %d: ctl %d\n",
				i, mixer->hw_ctl->idx - CTL_0);
	}
}

static void _dpu_crtc_setup_mixers(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct drm_encoder *enc;

	dpu_crtc->num_mixers = 0;
	dpu_crtc->mixers_swapped = false;
	memset(dpu_crtc->mixers, 0, sizeof(dpu_crtc->mixers));

	mutex_lock(&dpu_crtc->crtc_lock);
	/* Check for mixers on all encoders attached to this crtc */
	list_for_each_entry(enc, &crtc->dev->mode_config.encoder_list, head) {
		if (enc->crtc != crtc)
			continue;

		_dpu_crtc_setup_mixer_for_encoder(crtc, enc);
	}

	mutex_unlock(&dpu_crtc->crtc_lock);
=======
	kthread_queue_work(priv->event_thread[crtc_id].worker, &fevent->work);
}

void dpu_crtc_complete_commit(struct drm_crtc *crtc)
{
	trace_dpu_crtc_complete_commit(DRMID(crtc));
	dpu_core_perf_crtc_update(crtc, 0, false);
	_dpu_crtc_complete_flip(crtc);
>>>>>>> upstream/android-13
}

static void _dpu_crtc_setup_lm_bounds(struct drm_crtc *crtc,
		struct drm_crtc_state *state)
{
<<<<<<< HEAD
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *cstate;
	struct drm_display_mode *adj_mode;
	u32 crtc_split_width;
	int i;

	if (!crtc || !state) {
		DPU_ERROR("invalid args\n");
		return;
	}

	dpu_crtc = to_dpu_crtc(crtc);
	cstate = to_dpu_crtc_state(state);

	adj_mode = &state->adjusted_mode;
	crtc_split_width = dpu_crtc_get_mixer_width(dpu_crtc, cstate, adj_mode);

	for (i = 0; i < dpu_crtc->num_mixers; i++) {
=======
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(state);
	struct drm_display_mode *adj_mode = &state->adjusted_mode;
	u32 crtc_split_width = adj_mode->hdisplay / cstate->num_mixers;
	int i;

	for (i = 0; i < cstate->num_mixers; i++) {
>>>>>>> upstream/android-13
		struct drm_rect *r = &cstate->lm_bounds[i];
		r->x1 = crtc_split_width * i;
		r->y1 = 0;
		r->x2 = r->x1 + crtc_split_width;
<<<<<<< HEAD
		r->y2 = dpu_crtc_get_mixer_height(dpu_crtc, cstate, adj_mode);

		trace_dpu_crtc_setup_lm_bounds(DRMID(crtc), i, r);
	}

	drm_mode_debug_printmodeline(adj_mode);
}

static void dpu_crtc_atomic_begin(struct drm_crtc *crtc,
		struct drm_crtc_state *old_state)
{
	struct dpu_crtc *dpu_crtc;
	struct drm_encoder *encoder;
	struct drm_device *dev;
	unsigned long flags;
	struct dpu_crtc_smmu_state_data *smmu_state;

	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	if (!crtc->state->enable) {
		DPU_DEBUG("crtc%d -> enable %d, skip atomic_begin\n",
=======
		r->y2 = adj_mode->vdisplay;

		trace_dpu_crtc_setup_lm_bounds(DRMID(crtc), i, r);
	}
}

static void _dpu_crtc_get_pcc_coeff(struct drm_crtc_state *state,
		struct dpu_hw_pcc_cfg *cfg)
{
	struct drm_color_ctm *ctm;

	memset(cfg, 0, sizeof(struct dpu_hw_pcc_cfg));

	ctm = (struct drm_color_ctm *)state->ctm->data;

	if (!ctm)
		return;

	cfg->r.r = CONVERT_S3_15(ctm->matrix[0]);
	cfg->g.r = CONVERT_S3_15(ctm->matrix[1]);
	cfg->b.r = CONVERT_S3_15(ctm->matrix[2]);

	cfg->r.g = CONVERT_S3_15(ctm->matrix[3]);
	cfg->g.g = CONVERT_S3_15(ctm->matrix[4]);
	cfg->b.g = CONVERT_S3_15(ctm->matrix[5]);

	cfg->r.b = CONVERT_S3_15(ctm->matrix[6]);
	cfg->g.b = CONVERT_S3_15(ctm->matrix[7]);
	cfg->b.b = CONVERT_S3_15(ctm->matrix[8]);
}

static void _dpu_crtc_setup_cp_blocks(struct drm_crtc *crtc)
{
	struct drm_crtc_state *state = crtc->state;
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc->state);
	struct dpu_crtc_mixer *mixer = cstate->mixers;
	struct dpu_hw_pcc_cfg cfg;
	struct dpu_hw_ctl *ctl;
	struct dpu_hw_dspp *dspp;
	int i;


	if (!state->color_mgmt_changed)
		return;

	for (i = 0; i < cstate->num_mixers; i++) {
		ctl = mixer[i].lm_ctl;
		dspp = mixer[i].hw_dspp;

		if (!dspp || !dspp->ops.setup_pcc)
			continue;

		if (!state->ctm) {
			dspp->ops.setup_pcc(dspp, NULL);
		} else {
			_dpu_crtc_get_pcc_coeff(state, &cfg);
			dspp->ops.setup_pcc(dspp, &cfg);
		}

		mixer[i].flush_mask |= ctl->ops.get_bitmask_dspp(ctl,
			mixer[i].hw_dspp->idx);

		/* stage config flush mask */
		ctl->ops.update_pending_flush(ctl, mixer[i].flush_mask);

		DRM_DEBUG_ATOMIC("lm %d, ctl %d, flush mask 0x%x\n",
			mixer[i].hw_lm->idx - DSPP_0,
			ctl->idx - CTL_0,
			mixer[i].flush_mask);
	}
}

static void dpu_crtc_atomic_begin(struct drm_crtc *crtc,
		struct drm_atomic_state *state)
{
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc->state);
	struct drm_encoder *encoder;

	if (!crtc->state->enable) {
		DRM_DEBUG_ATOMIC("crtc%d -> enable %d, skip atomic_begin\n",
>>>>>>> upstream/android-13
				crtc->base.id, crtc->state->enable);
		return;
	}

<<<<<<< HEAD
	DPU_DEBUG("crtc%d\n", crtc->base.id);

	dpu_crtc = to_dpu_crtc(crtc);
	dev = crtc->dev;
	smmu_state = &dpu_crtc->smmu_state;

	if (!dpu_crtc->num_mixers) {
		_dpu_crtc_setup_mixers(crtc);
		_dpu_crtc_setup_lm_bounds(crtc, crtc->state);
	}

	if (dpu_crtc->event) {
		WARN_ON(dpu_crtc->event);
	} else {
		spin_lock_irqsave(&dev->event_lock, flags);
		dpu_crtc->event = crtc->state->event;
		crtc->state->event = NULL;
		spin_unlock_irqrestore(&dev->event_lock, flags);
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		if (encoder->crtc != crtc)
			continue;

		/* encoder will trigger pending mask now */
		dpu_encoder_trigger_kickoff_pending(encoder);
	}
=======
	DRM_DEBUG_ATOMIC("crtc%d\n", crtc->base.id);

	_dpu_crtc_setup_lm_bounds(crtc, crtc->state);

	/* encoder will trigger pending mask now */
	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask)
		dpu_encoder_trigger_kickoff_pending(encoder);
>>>>>>> upstream/android-13

	/*
	 * If no mixers have been allocated in dpu_crtc_atomic_check(),
	 * it means we are trying to flush a CRTC whose state is disabled:
	 * nothing else needs to be done.
	 */
<<<<<<< HEAD
	if (unlikely(!dpu_crtc->num_mixers))
=======
	if (unlikely(!cstate->num_mixers))
>>>>>>> upstream/android-13
		return;

	_dpu_crtc_blend_setup(crtc);

<<<<<<< HEAD
=======
	_dpu_crtc_setup_cp_blocks(crtc);

>>>>>>> upstream/android-13
	/*
	 * PP_DONE irq is only used by command mode for now.
	 * It is better to request pending before FLUSH and START trigger
	 * to make sure no pp_done irq missed.
	 * This is safe because no pp_done will happen before SW trigger
	 * in command mode.
	 */
}

static void dpu_crtc_atomic_flush(struct drm_crtc *crtc,
<<<<<<< HEAD
		struct drm_crtc_state *old_crtc_state)
=======
		struct drm_atomic_state *state)
>>>>>>> upstream/android-13
{
	struct dpu_crtc *dpu_crtc;
	struct drm_device *dev;
	struct drm_plane *plane;
	struct msm_drm_private *priv;
<<<<<<< HEAD
	struct msm_drm_thread *event_thread;
	unsigned long flags;
	struct dpu_crtc_state *cstate;

	if (!crtc || !crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	if (!crtc->state->enable) {
		DPU_DEBUG("crtc%d -> enable %d, skip atomic_flush\n",
=======
	unsigned long flags;
	struct dpu_crtc_state *cstate;

	if (!crtc->state->enable) {
		DRM_DEBUG_ATOMIC("crtc%d -> enable %d, skip atomic_flush\n",
>>>>>>> upstream/android-13
				crtc->base.id, crtc->state->enable);
		return;
	}

<<<<<<< HEAD
	DPU_DEBUG("crtc%d\n", crtc->base.id);
=======
	DRM_DEBUG_ATOMIC("crtc%d\n", crtc->base.id);
>>>>>>> upstream/android-13

	dpu_crtc = to_dpu_crtc(crtc);
	cstate = to_dpu_crtc_state(crtc->state);
	dev = crtc->dev;
	priv = dev->dev_private;

	if (crtc->index >= ARRAY_SIZE(priv->event_thread)) {
		DPU_ERROR("invalid crtc index[%d]\n", crtc->index);
		return;
	}

<<<<<<< HEAD
	event_thread = &priv->event_thread[crtc->index];

	if (dpu_crtc->event) {
		DPU_DEBUG("already received dpu_crtc->event\n");
	} else {
		spin_lock_irqsave(&dev->event_lock, flags);
		dpu_crtc->event = crtc->state->event;
		crtc->state->event = NULL;
		spin_unlock_irqrestore(&dev->event_lock, flags);
	}
=======
	WARN_ON(dpu_crtc->event);
	spin_lock_irqsave(&dev->event_lock, flags);
	dpu_crtc->event = crtc->state->event;
	crtc->state->event = NULL;
	spin_unlock_irqrestore(&dev->event_lock, flags);
>>>>>>> upstream/android-13

	/*
	 * If no mixers has been allocated in dpu_crtc_atomic_check(),
	 * it means we are trying to flush a CRTC whose state is disabled:
	 * nothing else needs to be done.
	 */
<<<<<<< HEAD
	if (unlikely(!dpu_crtc->num_mixers))
		return;

	/*
	 * For planes without commit update, drm framework will not add
	 * those planes to current state since hardware update is not
	 * required. However, if those planes were power collapsed since
	 * last commit cycle, driver has to restore the hardware state
	 * of those planes explicitly here prior to plane flush.
	 */
	drm_atomic_crtc_for_each_plane(plane, crtc)
		dpu_plane_restore(plane);

=======
	if (unlikely(!cstate->num_mixers))
		return;

>>>>>>> upstream/android-13
	/* update performance setting before crtc kickoff */
	dpu_core_perf_crtc_update(crtc, 1, false);

	/*
	 * Final plane updates: Give each plane a chance to complete all
	 *                      required writes/flushing before crtc's "flush
	 *                      everything" call below.
	 */
	drm_atomic_crtc_for_each_plane(plane, crtc) {
		if (dpu_crtc->smmu_state.transition_error)
			dpu_plane_set_error(plane, true);
		dpu_plane_flush(plane);
	}

	/* Kickoff will be scheduled by outer layer */
}

/**
 * dpu_crtc_destroy_state - state destroy hook
 * @crtc: drm CRTC
 * @state: CRTC state object to release
 */
static void dpu_crtc_destroy_state(struct drm_crtc *crtc,
		struct drm_crtc_state *state)
{
<<<<<<< HEAD
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *cstate;

	if (!crtc || !state) {
		DPU_ERROR("invalid argument(s)\n");
		return;
	}

	dpu_crtc = to_dpu_crtc(crtc);
	cstate = to_dpu_crtc_state(state);

	DPU_DEBUG("crtc%d\n", crtc->base.id);

	_dpu_crtc_rp_destroy(&cstate->rp);
=======
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(state);

	DRM_DEBUG_ATOMIC("crtc%d\n", crtc->base.id);
>>>>>>> upstream/android-13

	__drm_atomic_helper_crtc_destroy_state(state);

	kfree(cstate);
}

static int _dpu_crtc_wait_for_frame_done(struct drm_crtc *crtc)
{
<<<<<<< HEAD
	struct dpu_crtc *dpu_crtc;
	int ret, rc = 0;

	if (!crtc) {
		DPU_ERROR("invalid argument\n");
		return -EINVAL;
	}
	dpu_crtc = to_dpu_crtc(crtc);

	if (!atomic_read(&dpu_crtc->frame_pending)) {
		DPU_DEBUG("no frames pending\n");
=======
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	int ret, rc = 0;

	if (!atomic_read(&dpu_crtc->frame_pending)) {
		DRM_DEBUG_ATOMIC("no frames pending\n");
>>>>>>> upstream/android-13
		return 0;
	}

	DPU_ATRACE_BEGIN("frame done completion wait");
	ret = wait_for_completion_timeout(&dpu_crtc->frame_done_comp,
<<<<<<< HEAD
			msecs_to_jiffies(DPU_FRAME_DONE_TIMEOUT));
=======
			msecs_to_jiffies(DPU_CRTC_FRAME_DONE_TIMEOUT_MS));
>>>>>>> upstream/android-13
	if (!ret) {
		DRM_ERROR("frame done wait timed out, ret:%d\n", ret);
		rc = -ETIMEDOUT;
	}
	DPU_ATRACE_END("frame done completion wait");

	return rc;
}

void dpu_crtc_commit_kickoff(struct drm_crtc *crtc)
{
	struct drm_encoder *encoder;
<<<<<<< HEAD
	struct drm_device *dev;
	struct dpu_crtc *dpu_crtc;
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms;
	struct dpu_crtc_state *cstate;
	int ret;

	if (!crtc) {
		DPU_ERROR("invalid argument\n");
		return;
	}
	dev = crtc->dev;
	dpu_crtc = to_dpu_crtc(crtc);
	dpu_kms = _dpu_crtc_get_kms(crtc);

	if (!dpu_kms || !dpu_kms->dev || !dpu_kms->dev->dev_private) {
		DPU_ERROR("invalid argument\n");
		return;
	}

	priv = dpu_kms->dev->dev_private;
	cstate = to_dpu_crtc_state(crtc->state);
=======
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct dpu_kms *dpu_kms = _dpu_crtc_get_kms(crtc);
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc->state);
>>>>>>> upstream/android-13

	/*
	 * If no mixers has been allocated in dpu_crtc_atomic_check(),
	 * it means we are trying to start a CRTC whose state is disabled:
	 * nothing else needs to be done.
	 */
<<<<<<< HEAD
	if (unlikely(!dpu_crtc->num_mixers))
=======
	if (unlikely(!cstate->num_mixers))
>>>>>>> upstream/android-13
		return;

	DPU_ATRACE_BEGIN("crtc_commit");

<<<<<<< HEAD
	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		struct dpu_encoder_kickoff_params params = { 0 };

		if (encoder->crtc != crtc)
			continue;

		/*
		 * Encoder will flush/start now, unless it has a tx pending.
		 * If so, it may delay and flush at an irq event (e.g. ppdone)
		 */
		dpu_encoder_prepare_for_kickoff(encoder, &params);
	}

	/* wait for frame_event_done completion */
	DPU_ATRACE_BEGIN("wait_for_frame_done_event");
	ret = _dpu_crtc_wait_for_frame_done(crtc);
	DPU_ATRACE_END("wait_for_frame_done_event");
	if (ret) {
		DPU_ERROR("crtc%d wait for frame done failed;frame_pending%d\n",
				crtc->base.id,
				atomic_read(&dpu_crtc->frame_pending));
		goto end;
	}

	if (atomic_inc_return(&dpu_crtc->frame_pending) == 1) {
		/* acquire bandwidth and other resources */
		DPU_DEBUG("crtc%d first commit\n", crtc->base.id);
	} else
		DPU_DEBUG("crtc%d commit\n", crtc->base.id);
=======
	/*
	 * Encoder will flush/start now, unless it has a tx pending. If so, it
	 * may delay and flush at an irq event (e.g. ppdone)
	 */
	drm_for_each_encoder_mask(encoder, crtc->dev,
				  crtc->state->encoder_mask)
		dpu_encoder_prepare_for_kickoff(encoder);

	if (atomic_inc_return(&dpu_crtc->frame_pending) == 1) {
		/* acquire bandwidth and other resources */
		DRM_DEBUG_ATOMIC("crtc%d first commit\n", crtc->base.id);
	} else
		DRM_DEBUG_ATOMIC("crtc%d commit\n", crtc->base.id);
>>>>>>> upstream/android-13

	dpu_crtc->play_count++;

	dpu_vbif_clear_errors(dpu_kms);

<<<<<<< HEAD
	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		if (encoder->crtc != crtc)
			continue;

		dpu_encoder_kickoff(encoder);
	}

end:
=======
	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask)
		dpu_encoder_kickoff(encoder);

>>>>>>> upstream/android-13
	reinit_completion(&dpu_crtc->frame_done_comp);
	DPU_ATRACE_END("crtc_commit");
}

<<<<<<< HEAD
/**
 * _dpu_crtc_vblank_enable_no_lock - update power resource and vblank request
 * @dpu_crtc: Pointer to dpu crtc structure
 * @enable: Whether to enable/disable vblanks
 *
 * @Return: error code
 */
static int _dpu_crtc_vblank_enable_no_lock(
		struct dpu_crtc *dpu_crtc, bool enable)
{
	struct drm_device *dev;
	struct drm_crtc *crtc;
	struct drm_encoder *enc;

	if (!dpu_crtc) {
		DPU_ERROR("invalid crtc\n");
		return -EINVAL;
	}

	crtc = &dpu_crtc->base;
	dev = crtc->dev;

	if (enable) {
		int ret;

		/* drop lock since power crtc cb may try to re-acquire lock */
		mutex_unlock(&dpu_crtc->crtc_lock);
		ret = _dpu_crtc_power_enable(dpu_crtc, true);
		mutex_lock(&dpu_crtc->crtc_lock);
		if (ret)
			return ret;

		list_for_each_entry(enc, &dev->mode_config.encoder_list, head) {
			if (enc->crtc != crtc)
				continue;

			trace_dpu_crtc_vblank_enable(DRMID(&dpu_crtc->base),
						     DRMID(enc), enable,
						     dpu_crtc);

			dpu_encoder_register_vblank_callback(enc,
					dpu_crtc_vblank_cb, (void *)crtc);
		}
	} else {
		list_for_each_entry(enc, &dev->mode_config.encoder_list, head) {
			if (enc->crtc != crtc)
				continue;

			trace_dpu_crtc_vblank_enable(DRMID(&dpu_crtc->base),
						     DRMID(enc), enable,
						     dpu_crtc);

			dpu_encoder_register_vblank_callback(enc, NULL, NULL);
		}

		/* drop lock since power crtc cb may try to re-acquire lock */
		mutex_unlock(&dpu_crtc->crtc_lock);
		_dpu_crtc_power_enable(dpu_crtc, false);
		mutex_lock(&dpu_crtc->crtc_lock);
	}

	return 0;
}

/**
 * _dpu_crtc_set_suspend - notify crtc of suspend enable/disable
 * @crtc: Pointer to drm crtc object
 * @enable: true to enable suspend, false to indicate resume
 */
static void _dpu_crtc_set_suspend(struct drm_crtc *crtc, bool enable)
{
	struct dpu_crtc *dpu_crtc;
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms;
	int ret = 0;

	if (!crtc || !crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid crtc\n");
		return;
	}
	dpu_crtc = to_dpu_crtc(crtc);
	priv = crtc->dev->dev_private;

	if (!priv->kms) {
		DPU_ERROR("invalid crtc kms\n");
		return;
	}
	dpu_kms = to_dpu_kms(priv->kms);

	DRM_DEBUG_KMS("crtc%d suspend = %d\n", crtc->base.id, enable);

	mutex_lock(&dpu_crtc->crtc_lock);

	/*
	 * If the vblank is enabled, release a power reference on suspend
	 * and take it back during resume (if it is still enabled).
	 */
	trace_dpu_crtc_set_suspend(DRMID(&dpu_crtc->base), enable, dpu_crtc);
	if (dpu_crtc->suspend == enable)
		DPU_DEBUG("crtc%d suspend already set to %d, ignoring update\n",
				crtc->base.id, enable);
	else if (dpu_crtc->enabled && dpu_crtc->vblank_requested) {
		ret = _dpu_crtc_vblank_enable_no_lock(dpu_crtc, !enable);
		if (ret)
			DPU_ERROR("%s vblank enable failed: %d\n",
					dpu_crtc->name, ret);
	}

	dpu_crtc->suspend = enable;
	mutex_unlock(&dpu_crtc->crtc_lock);
=======
static void dpu_crtc_reset(struct drm_crtc *crtc)
{
	struct dpu_crtc_state *cstate = kzalloc(sizeof(*cstate), GFP_KERNEL);

	if (crtc->state)
		dpu_crtc_destroy_state(crtc, crtc->state);

	__drm_atomic_helper_crtc_reset(crtc, &cstate->base);
>>>>>>> upstream/android-13
}

/**
 * dpu_crtc_duplicate_state - state duplicate hook
 * @crtc: Pointer to drm crtc structure
<<<<<<< HEAD
 * @Returns: Pointer to new drm_crtc_state structure
 */
static struct drm_crtc_state *dpu_crtc_duplicate_state(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *cstate, *old_cstate;

	if (!crtc || !crtc->state) {
		DPU_ERROR("invalid argument(s)\n");
		return NULL;
	}

	dpu_crtc = to_dpu_crtc(crtc);
	old_cstate = to_dpu_crtc_state(crtc->state);
=======
 */
static struct drm_crtc_state *dpu_crtc_duplicate_state(struct drm_crtc *crtc)
{
	struct dpu_crtc_state *cstate, *old_cstate = to_dpu_crtc_state(crtc->state);

>>>>>>> upstream/android-13
	cstate = kmemdup(old_cstate, sizeof(*old_cstate), GFP_KERNEL);
	if (!cstate) {
		DPU_ERROR("failed to allocate state\n");
		return NULL;
	}

	/* duplicate base helper */
	__drm_atomic_helper_crtc_duplicate_state(crtc, &cstate->base);

<<<<<<< HEAD
	_dpu_crtc_rp_duplicate(&old_cstate->rp, &cstate->rp);

	return &cstate->base;
}

/**
 * dpu_crtc_reset - reset hook for CRTCs
 * Resets the atomic state for @crtc by freeing the state pointer (which might
 * be NULL, e.g. at driver load time) and allocating a new empty state object.
 * @crtc: Pointer to drm crtc structure
 */
static void dpu_crtc_reset(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *cstate;

	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	/* revert suspend actions, if necessary */
	if (dpu_kms_is_suspend_state(crtc->dev))
		_dpu_crtc_set_suspend(crtc, false);

	/* remove previous state, if present */
	if (crtc->state) {
		dpu_crtc_destroy_state(crtc, crtc->state);
		crtc->state = 0;
	}

	dpu_crtc = to_dpu_crtc(crtc);
	cstate = kzalloc(sizeof(*cstate), GFP_KERNEL);
	if (!cstate) {
		DPU_ERROR("failed to allocate state\n");
		return;
	}

	_dpu_crtc_rp_reset(&cstate->rp, &dpu_crtc->rp_lock,
			&dpu_crtc->rp_head);

	cstate->base.crtc = crtc;
	crtc->state = &cstate->base;
}

static void dpu_crtc_handle_power_event(u32 event_type, void *arg)
{
	struct drm_crtc *crtc = arg;
	struct dpu_crtc *dpu_crtc;
	struct drm_encoder *encoder;
	struct dpu_crtc_mixer *m;
	u32 i, misr_status;

	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return;
	}
	dpu_crtc = to_dpu_crtc(crtc);

	mutex_lock(&dpu_crtc->crtc_lock);

	trace_dpu_crtc_handle_power_event(DRMID(crtc), event_type);

	switch (event_type) {
	case DPU_POWER_EVENT_POST_ENABLE:
		/* restore encoder; crtc will be programmed during commit */
		drm_for_each_encoder(encoder, crtc->dev) {
			if (encoder->crtc != crtc)
				continue;

			dpu_encoder_virt_restore(encoder);
		}

		for (i = 0; i < dpu_crtc->num_mixers; ++i) {
			m = &dpu_crtc->mixers[i];
			if (!m->hw_lm || !m->hw_lm->ops.setup_misr ||
					!dpu_crtc->misr_enable)
				continue;

			m->hw_lm->ops.setup_misr(m->hw_lm, true,
					dpu_crtc->misr_frame_count);
		}
		break;
	case DPU_POWER_EVENT_PRE_DISABLE:
		for (i = 0; i < dpu_crtc->num_mixers; ++i) {
			m = &dpu_crtc->mixers[i];
			if (!m->hw_lm || !m->hw_lm->ops.collect_misr ||
					!dpu_crtc->misr_enable)
				continue;

			misr_status = m->hw_lm->ops.collect_misr(m->hw_lm);
			dpu_crtc->misr_data[i] = misr_status ? misr_status :
							dpu_crtc->misr_data[i];
		}
		break;
	case DPU_POWER_EVENT_POST_DISABLE:
		/**
		 * Nothing to do. All the planes on the CRTC will be
		 * programmed for every frame
		 */
		break;
	default:
		DPU_DEBUG("event:%d not handled\n", event_type);
		break;
	}

	mutex_unlock(&dpu_crtc->crtc_lock);
}

static void dpu_crtc_disable(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *cstate;
	struct drm_display_mode *mode;
	struct drm_encoder *encoder;
	struct msm_drm_private *priv;
	int ret;
	unsigned long flags;

	if (!crtc || !crtc->dev || !crtc->dev->dev_private || !crtc->state) {
		DPU_ERROR("invalid crtc\n");
		return;
	}
	dpu_crtc = to_dpu_crtc(crtc);
	cstate = to_dpu_crtc_state(crtc->state);
	mode = &cstate->base.adjusted_mode;
	priv = crtc->dev->dev_private;

	DRM_DEBUG_KMS("crtc%d\n", crtc->base.id);

	if (dpu_kms_is_suspend_state(crtc->dev))
		_dpu_crtc_set_suspend(crtc, true);

	/* Disable/save vblank irq handling */
	drm_crtc_vblank_off(crtc);

	mutex_lock(&dpu_crtc->crtc_lock);
=======
	return &cstate->base;
}

static void dpu_crtc_disable(struct drm_crtc *crtc,
			     struct drm_atomic_state *state)
{
	struct drm_crtc_state *old_crtc_state = drm_atomic_get_old_crtc_state(state,
									      crtc);
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc->state);
	struct drm_encoder *encoder;
	unsigned long flags;
	bool release_bandwidth = false;

	DRM_DEBUG_KMS("crtc%d\n", crtc->base.id);

	/* Disable/save vblank irq handling */
	drm_crtc_vblank_off(crtc);

	drm_for_each_encoder_mask(encoder, crtc->dev,
				  old_crtc_state->encoder_mask) {
		/* in video mode, we hold an extra bandwidth reference
		 * as we cannot drop bandwidth at frame-done if any
		 * crtc is being used in video mode.
		 */
		if (dpu_encoder_get_intf_mode(encoder) == INTF_MODE_VIDEO)
			release_bandwidth = true;
		dpu_encoder_assign_crtc(encoder, NULL);
	}
>>>>>>> upstream/android-13

	/* wait for frame_event_done completion */
	if (_dpu_crtc_wait_for_frame_done(crtc))
		DPU_ERROR("crtc%d wait for frame done failed;frame_pending%d\n",
				crtc->base.id,
				atomic_read(&dpu_crtc->frame_pending));

	trace_dpu_crtc_disable(DRMID(crtc), false, dpu_crtc);
<<<<<<< HEAD
	if (dpu_crtc->enabled && !dpu_crtc->suspend &&
			dpu_crtc->vblank_requested) {
		ret = _dpu_crtc_vblank_enable_no_lock(dpu_crtc, false);
		if (ret)
			DPU_ERROR("%s vblank enable failed: %d\n",
					dpu_crtc->name, ret);
	}
=======
>>>>>>> upstream/android-13
	dpu_crtc->enabled = false;

	if (atomic_read(&dpu_crtc->frame_pending)) {
		trace_dpu_crtc_disable_frame_pending(DRMID(crtc),
				     atomic_read(&dpu_crtc->frame_pending));
<<<<<<< HEAD
		dpu_core_perf_crtc_release_bw(crtc);
=======
		if (release_bandwidth)
			dpu_core_perf_crtc_release_bw(crtc);
>>>>>>> upstream/android-13
		atomic_set(&dpu_crtc->frame_pending, 0);
	}

	dpu_core_perf_crtc_update(crtc, 0, true);

<<<<<<< HEAD
	drm_for_each_encoder(encoder, crtc->dev) {
		if (encoder->crtc != crtc)
			continue;
		dpu_encoder_register_frame_event_callback(encoder, NULL, NULL);
	}

	if (dpu_crtc->power_event)
		dpu_power_handle_unregister_event(dpu_crtc->phandle,
				dpu_crtc->power_event);

	memset(dpu_crtc->mixers, 0, sizeof(dpu_crtc->mixers));
	dpu_crtc->num_mixers = 0;
	dpu_crtc->mixers_swapped = false;
=======
	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask)
		dpu_encoder_register_frame_event_callback(encoder, NULL, NULL);

	memset(cstate->mixers, 0, sizeof(cstate->mixers));
	cstate->num_mixers = 0;
>>>>>>> upstream/android-13

	/* disable clk & bw control until clk & bw properties are set */
	cstate->bw_control = false;
	cstate->bw_split_vote = false;

<<<<<<< HEAD
	mutex_unlock(&dpu_crtc->crtc_lock);

=======
>>>>>>> upstream/android-13
	if (crtc->state->event && !crtc->state->active) {
		spin_lock_irqsave(&crtc->dev->event_lock, flags);
		drm_crtc_send_vblank_event(crtc, crtc->state->event);
		crtc->state->event = NULL;
		spin_unlock_irqrestore(&crtc->dev->event_lock, flags);
	}
<<<<<<< HEAD
}

static void dpu_crtc_enable(struct drm_crtc *crtc,
		struct drm_crtc_state *old_crtc_state)
{
	struct dpu_crtc *dpu_crtc;
	struct drm_encoder *encoder;
	struct msm_drm_private *priv;
	int ret;

	if (!crtc || !crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid crtc\n");
		return;
	}
	priv = crtc->dev->dev_private;

	DRM_DEBUG_KMS("crtc%d\n", crtc->base.id);
	dpu_crtc = to_dpu_crtc(crtc);

	drm_for_each_encoder(encoder, crtc->dev) {
		if (encoder->crtc != crtc)
			continue;
=======

	pm_runtime_put_sync(crtc->dev->dev);
}

static void dpu_crtc_enable(struct drm_crtc *crtc,
		struct drm_atomic_state *state)
{
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct drm_encoder *encoder;
	bool request_bandwidth = false;

	pm_runtime_get_sync(crtc->dev->dev);

	DRM_DEBUG_KMS("crtc%d\n", crtc->base.id);

	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask) {
		/* in video mode, we hold an extra bandwidth reference
		 * as we cannot drop bandwidth at frame-done if any
		 * crtc is being used in video mode.
		 */
		if (dpu_encoder_get_intf_mode(encoder) == INTF_MODE_VIDEO)
			request_bandwidth = true;
>>>>>>> upstream/android-13
		dpu_encoder_register_frame_event_callback(encoder,
				dpu_crtc_frame_event_cb, (void *)crtc);
	}

<<<<<<< HEAD
	mutex_lock(&dpu_crtc->crtc_lock);
	trace_dpu_crtc_enable(DRMID(crtc), true, dpu_crtc);
	if (!dpu_crtc->enabled && !dpu_crtc->suspend &&
			dpu_crtc->vblank_requested) {
		ret = _dpu_crtc_vblank_enable_no_lock(dpu_crtc, true);
		if (ret)
			DPU_ERROR("%s vblank enable failed: %d\n",
					dpu_crtc->name, ret);
	}
	dpu_crtc->enabled = true;

	mutex_unlock(&dpu_crtc->crtc_lock);

	/* Enable/restore vblank irq handling */
	drm_crtc_vblank_on(crtc);

	dpu_crtc->power_event = dpu_power_handle_register_event(
		dpu_crtc->phandle,
		DPU_POWER_EVENT_POST_ENABLE | DPU_POWER_EVENT_POST_DISABLE |
		DPU_POWER_EVENT_PRE_DISABLE,
		dpu_crtc_handle_power_event, crtc, dpu_crtc->name);

=======
	if (request_bandwidth)
		atomic_inc(&_dpu_crtc_get_kms(crtc)->bandwidth_ref);

	trace_dpu_crtc_enable(DRMID(crtc), true, dpu_crtc);
	dpu_crtc->enabled = true;

	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask)
		dpu_encoder_assign_crtc(encoder, crtc);

	/* Enable/restore vblank irq handling */
	drm_crtc_vblank_on(crtc);
>>>>>>> upstream/android-13
}

struct plane_state {
	struct dpu_plane_state *dpu_pstate;
	const struct drm_plane_state *drm_pstate;
	int stage;
	u32 pipe_id;
};

static int dpu_crtc_atomic_check(struct drm_crtc *crtc,
<<<<<<< HEAD
		struct drm_crtc_state *state)
{
	struct dpu_crtc *dpu_crtc;
	struct plane_state *pstates;
	struct dpu_crtc_state *cstate;
=======
		struct drm_atomic_state *state)
{
	struct drm_crtc_state *crtc_state = drm_atomic_get_new_crtc_state(state,
									  crtc);
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct dpu_crtc_state *cstate = to_dpu_crtc_state(crtc_state);
	struct plane_state *pstates;
>>>>>>> upstream/android-13

	const struct drm_plane_state *pstate;
	struct drm_plane *plane;
	struct drm_display_mode *mode;

<<<<<<< HEAD
	int cnt = 0, rc = 0, mixer_width, i, z_pos;
=======
	int cnt = 0, rc = 0, mixer_width = 0, i, z_pos;
>>>>>>> upstream/android-13

	struct dpu_multirect_plane_states multirect_plane[DPU_STAGE_MAX * 2];
	int multirect_count = 0;
	const struct drm_plane_state *pipe_staged[SSPP_MAX];
	int left_zpos_cnt = 0, right_zpos_cnt = 0;
	struct drm_rect crtc_rect = { 0 };

<<<<<<< HEAD
	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return -EINVAL;
	}

	pstates = kzalloc(sizeof(*pstates) * DPU_STAGE_MAX * 4, GFP_KERNEL);

	dpu_crtc = to_dpu_crtc(crtc);
	cstate = to_dpu_crtc_state(state);

	if (!state->enable || !state->active) {
		DPU_DEBUG("crtc%d -> enable %d, active %d, skip atomic_check\n",
				crtc->base.id, state->enable, state->active);
		goto end;
	}

	mode = &state->adjusted_mode;
	DPU_DEBUG("%s: check", dpu_crtc->name);

	/* force a full mode set if active state changed */
	if (state->active_changed)
		state->mode_changed = true;

	memset(pipe_staged, 0, sizeof(pipe_staged));

	mixer_width = dpu_crtc_get_mixer_width(dpu_crtc, cstate, mode);

	_dpu_crtc_setup_lm_bounds(crtc, state);
=======
	pstates = kzalloc(sizeof(*pstates) * DPU_STAGE_MAX * 4, GFP_KERNEL);

	if (!crtc_state->enable || !crtc_state->active) {
		DRM_DEBUG_ATOMIC("crtc%d -> enable %d, active %d, skip atomic_check\n",
				crtc->base.id, crtc_state->enable,
				crtc_state->active);
		memset(&cstate->new_perf, 0, sizeof(cstate->new_perf));
		goto end;
	}

	mode = &crtc_state->adjusted_mode;
	DRM_DEBUG_ATOMIC("%s: check\n", dpu_crtc->name);

	/* force a full mode set if active state changed */
	if (crtc_state->active_changed)
		crtc_state->mode_changed = true;

	memset(pipe_staged, 0, sizeof(pipe_staged));

	if (cstate->num_mixers) {
		mixer_width = mode->hdisplay / cstate->num_mixers;

		_dpu_crtc_setup_lm_bounds(crtc, crtc_state);
	}
>>>>>>> upstream/android-13

	crtc_rect.x2 = mode->hdisplay;
	crtc_rect.y2 = mode->vdisplay;

	 /* get plane state for all drm planes associated with crtc state */
<<<<<<< HEAD
	drm_atomic_crtc_state_for_each_plane_state(plane, pstate, state) {
=======
	drm_atomic_crtc_state_for_each_plane_state(plane, pstate, crtc_state) {
>>>>>>> upstream/android-13
		struct drm_rect dst, clip = crtc_rect;

		if (IS_ERR_OR_NULL(pstate)) {
			rc = PTR_ERR(pstate);
			DPU_ERROR("%s: failed to get plane%d state, %d\n",
					dpu_crtc->name, plane->base.id, rc);
			goto end;
		}
		if (cnt >= DPU_STAGE_MAX * 4)
			continue;

		pstates[cnt].dpu_pstate = to_dpu_plane_state(pstate);
		pstates[cnt].drm_pstate = pstate;
		pstates[cnt].stage = pstate->normalized_zpos;
		pstates[cnt].pipe_id = dpu_plane_pipe(plane);

		if (pipe_staged[pstates[cnt].pipe_id]) {
			multirect_plane[multirect_count].r0 =
				pipe_staged[pstates[cnt].pipe_id];
			multirect_plane[multirect_count].r1 = pstate;
			multirect_count++;

			pipe_staged[pstates[cnt].pipe_id] = NULL;
		} else {
			pipe_staged[pstates[cnt].pipe_id] = pstate;
		}

		cnt++;

		dst = drm_plane_state_dest(pstate);
		if (!drm_rect_intersect(&clip, &dst)) {
			DPU_ERROR("invalid vertical/horizontal destination\n");
			DPU_ERROR("display: " DRM_RECT_FMT " plane: "
				  DRM_RECT_FMT "\n", DRM_RECT_ARG(&crtc_rect),
				  DRM_RECT_ARG(&dst));
			rc = -E2BIG;
			goto end;
		}
	}

	for (i = 1; i < SSPP_MAX; i++) {
		if (pipe_staged[i]) {
			dpu_plane_clear_multirect(pipe_staged[i]);

			if (is_dpu_plane_virtual(pipe_staged[i]->plane)) {
				DPU_ERROR(
					"r1 only virt plane:%d not supported\n",
					pipe_staged[i]->plane->base.id);
				rc  = -EINVAL;
				goto end;
			}
		}
	}

	z_pos = -1;
	for (i = 0; i < cnt; i++) {
		/* reset counts at every new blend stage */
		if (pstates[i].stage != z_pos) {
			left_zpos_cnt = 0;
			right_zpos_cnt = 0;
			z_pos = pstates[i].stage;
		}

		/* verify z_pos setting before using it */
		if (z_pos >= DPU_STAGE_MAX - DPU_STAGE_0) {
			DPU_ERROR("> %d plane stages assigned\n",
					DPU_STAGE_MAX - DPU_STAGE_0);
			rc = -EINVAL;
			goto end;
		} else if (pstates[i].drm_pstate->crtc_x < mixer_width) {
			if (left_zpos_cnt == 2) {
				DPU_ERROR("> 2 planes @ stage %d on left\n",
					z_pos);
				rc = -EINVAL;
				goto end;
			}
			left_zpos_cnt++;

		} else {
			if (right_zpos_cnt == 2) {
				DPU_ERROR("> 2 planes @ stage %d on right\n",
					z_pos);
				rc = -EINVAL;
				goto end;
			}
			right_zpos_cnt++;
		}

		pstates[i].dpu_pstate->stage = z_pos + DPU_STAGE_0;
<<<<<<< HEAD
		DPU_DEBUG("%s: zpos %d", dpu_crtc->name, z_pos);
=======
		DRM_DEBUG_ATOMIC("%s: zpos %d\n", dpu_crtc->name, z_pos);
>>>>>>> upstream/android-13
	}

	for (i = 0; i < multirect_count; i++) {
		if (dpu_plane_validate_multirect_v2(&multirect_plane[i])) {
			DPU_ERROR(
			"multirect validation failed for planes (%d - %d)\n",
					multirect_plane[i].r0->plane->base.id,
					multirect_plane[i].r1->plane->base.id);
			rc = -EINVAL;
			goto end;
		}
	}

<<<<<<< HEAD
	rc = dpu_core_perf_crtc_check(crtc, state);
=======
	atomic_inc(&_dpu_crtc_get_kms(crtc)->bandwidth_ref);

	rc = dpu_core_perf_crtc_check(crtc, crtc_state);
>>>>>>> upstream/android-13
	if (rc) {
		DPU_ERROR("crtc%d failed performance check %d\n",
				crtc->base.id, rc);
		goto end;
	}

	/* validate source split:
	 * use pstates sorted by stage to check planes on same stage
	 * we assume that all pipes are in source split so its valid to compare
	 * without taking into account left/right mixer placement
	 */
	for (i = 1; i < cnt; i++) {
		struct plane_state *prv_pstate, *cur_pstate;
		struct drm_rect left_rect, right_rect;
		int32_t left_pid, right_pid;
		int32_t stage;

		prv_pstate = &pstates[i - 1];
		cur_pstate = &pstates[i];
		if (prv_pstate->stage != cur_pstate->stage)
			continue;

		stage = cur_pstate->stage;

		left_pid = prv_pstate->dpu_pstate->base.plane->base.id;
		left_rect = drm_plane_state_dest(prv_pstate->drm_pstate);

		right_pid = cur_pstate->dpu_pstate->base.plane->base.id;
		right_rect = drm_plane_state_dest(cur_pstate->drm_pstate);

		if (right_rect.x1 < left_rect.x1) {
			swap(left_pid, right_pid);
			swap(left_rect, right_rect);
		}

		/**
		 * - planes are enumerated in pipe-priority order such that
		 *   planes with lower drm_id must be left-most in a shared
		 *   blend-stage when using source split.
		 * - planes in source split must be contiguous in width
		 * - planes in source split must have same dest yoff and height
		 */
		if (right_pid < left_pid) {
			DPU_ERROR(
				"invalid src split cfg. priority mismatch. stage: %d left: %d right: %d\n",
				stage, left_pid, right_pid);
			rc = -EINVAL;
			goto end;
		} else if (right_rect.x1 != drm_rect_width(&left_rect)) {
			DPU_ERROR("non-contiguous coordinates for src split. "
				  "stage: %d left: " DRM_RECT_FMT " right: "
				  DRM_RECT_FMT "\n", stage,
				  DRM_RECT_ARG(&left_rect),
				  DRM_RECT_ARG(&right_rect));
			rc = -EINVAL;
			goto end;
		} else if (left_rect.y1 != right_rect.y1 ||
			   drm_rect_height(&left_rect) != drm_rect_height(&right_rect)) {
			DPU_ERROR("source split at stage: %d. invalid "
				  "yoff/height: left: " DRM_RECT_FMT " right: "
				  DRM_RECT_FMT "\n", stage,
				  DRM_RECT_ARG(&left_rect),
				  DRM_RECT_ARG(&right_rect));
			rc = -EINVAL;
			goto end;
		}
	}

end:
<<<<<<< HEAD
	_dpu_crtc_rp_free_unused(&cstate->rp);
=======
>>>>>>> upstream/android-13
	kfree(pstates);
	return rc;
}

int dpu_crtc_vblank(struct drm_crtc *crtc, bool en)
{
<<<<<<< HEAD
	struct dpu_crtc *dpu_crtc;
	int ret;

	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return -EINVAL;
	}
	dpu_crtc = to_dpu_crtc(crtc);

	mutex_lock(&dpu_crtc->crtc_lock);
	trace_dpu_crtc_vblank(DRMID(&dpu_crtc->base), en, dpu_crtc);
	if (dpu_crtc->enabled && !dpu_crtc->suspend) {
		ret = _dpu_crtc_vblank_enable_no_lock(dpu_crtc, en);
		if (ret)
			DPU_ERROR("%s vblank enable failed: %d\n",
					dpu_crtc->name, ret);
	}
	dpu_crtc->vblank_requested = en;
	mutex_unlock(&dpu_crtc->crtc_lock);
=======
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
	struct drm_encoder *enc;

	trace_dpu_crtc_vblank(DRMID(&dpu_crtc->base), en, dpu_crtc);

	/*
	 * Normally we would iterate through encoder_mask in crtc state to find
	 * attached encoders. In this case, we might be disabling vblank _after_
	 * encoder_mask has been cleared.
	 *
	 * Instead, we "assign" a crtc to the encoder in enable and clear it in
	 * disable (which is also after encoder_mask is cleared). So instead of
	 * using encoder mask, we'll ask the encoder to toggle itself iff it's
	 * currently assigned to our crtc.
	 *
	 * Note also that this function cannot be called while crtc is disabled
	 * since we use drm_crtc_vblank_on/off. So we don't need to worry
	 * about the assigned crtcs being inconsistent with the current state
	 * (which means no need to worry about modeset locks).
	 */
	list_for_each_entry(enc, &crtc->dev->mode_config.encoder_list, head) {
		trace_dpu_crtc_vblank_enable(DRMID(crtc), DRMID(enc), en,
					     dpu_crtc);

		dpu_encoder_toggle_vblank_for_crtc(enc, crtc, en);
	}
>>>>>>> upstream/android-13

	return 0;
}

#ifdef CONFIG_DEBUG_FS
static int _dpu_debugfs_status_show(struct seq_file *s, void *data)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_plane_state *pstate = NULL;
	struct dpu_crtc_mixer *m;

	struct drm_crtc *crtc;
	struct drm_plane *plane;
	struct drm_display_mode *mode;
	struct drm_framebuffer *fb;
	struct drm_plane_state *state;
	struct dpu_crtc_state *cstate;

	int i, out_width;

<<<<<<< HEAD
	if (!s || !s->private)
		return -EINVAL;

	dpu_crtc = s->private;
	crtc = &dpu_crtc->base;
	cstate = to_dpu_crtc_state(crtc->state);

	mutex_lock(&dpu_crtc->crtc_lock);
	mode = &crtc->state->adjusted_mode;
	out_width = dpu_crtc_get_mixer_width(dpu_crtc, cstate, mode);
=======
	dpu_crtc = s->private;
	crtc = &dpu_crtc->base;

	drm_modeset_lock_all(crtc->dev);
	cstate = to_dpu_crtc_state(crtc->state);

	mode = &crtc->state->adjusted_mode;
	out_width = mode->hdisplay / cstate->num_mixers;
>>>>>>> upstream/android-13

	seq_printf(s, "crtc:%d width:%d height:%d\n", crtc->base.id,
				mode->hdisplay, mode->vdisplay);

	seq_puts(s, "\n");

<<<<<<< HEAD
	for (i = 0; i < dpu_crtc->num_mixers; ++i) {
		m = &dpu_crtc->mixers[i];
		if (!m->hw_lm)
			seq_printf(s, "\tmixer[%d] has no lm\n", i);
		else if (!m->hw_ctl)
			seq_printf(s, "\tmixer[%d] has no ctl\n", i);
		else
			seq_printf(s, "\tmixer:%d ctl:%d width:%d height:%d\n",
				m->hw_lm->idx - LM_0, m->hw_ctl->idx - CTL_0,
				out_width, mode->vdisplay);
=======
	for (i = 0; i < cstate->num_mixers; ++i) {
		m = &cstate->mixers[i];
		seq_printf(s, "\tmixer:%d ctl:%d width:%d height:%d\n",
			m->hw_lm->idx - LM_0, m->lm_ctl->idx - CTL_0,
			out_width, mode->vdisplay);
>>>>>>> upstream/android-13
	}

	seq_puts(s, "\n");

	drm_atomic_crtc_for_each_plane(plane, crtc) {
		pstate = to_dpu_plane_state(plane->state);
		state = plane->state;

		if (!pstate || !state)
			continue;

		seq_printf(s, "\tplane:%u stage:%d\n", plane->base.id,
			pstate->stage);

		if (plane->state->fb) {
			fb = plane->state->fb;

			seq_printf(s, "\tfb:%d image format:%4.4s wxh:%ux%u ",
				fb->base.id, (char *) &fb->format->format,
				fb->width, fb->height);
			for (i = 0; i < ARRAY_SIZE(fb->format->cpp); ++i)
				seq_printf(s, "cpp[%d]:%u ",
						i, fb->format->cpp[i]);
			seq_puts(s, "\n\t");

			seq_printf(s, "modifier:%8llu ", fb->modifier);
			seq_puts(s, "\n");

			seq_puts(s, "\t");
			for (i = 0; i < ARRAY_SIZE(fb->pitches); i++)
				seq_printf(s, "pitches[%d]:%8u ", i,
							fb->pitches[i]);
			seq_puts(s, "\n");

			seq_puts(s, "\t");
			for (i = 0; i < ARRAY_SIZE(fb->offsets); i++)
				seq_printf(s, "offsets[%d]:%8u ", i,
							fb->offsets[i]);
			seq_puts(s, "\n");
		}

		seq_printf(s, "\tsrc_x:%4d src_y:%4d src_w:%4d src_h:%4d\n",
			state->src_x, state->src_y, state->src_w, state->src_h);

		seq_printf(s, "\tdst x:%4d dst_y:%4d dst_w:%4d dst_h:%4d\n",
			state->crtc_x, state->crtc_y, state->crtc_w,
			state->crtc_h);
		seq_printf(s, "\tmultirect: mode: %d index: %d\n",
			pstate->multirect_mode, pstate->multirect_index);

		seq_puts(s, "\n");
	}
	if (dpu_crtc->vblank_cb_count) {
		ktime_t diff = ktime_sub(ktime_get(), dpu_crtc->vblank_cb_time);
		s64 diff_ms = ktime_to_ms(diff);
		s64 fps = diff_ms ? div_s64(
				dpu_crtc->vblank_cb_count * 1000, diff_ms) : 0;

		seq_printf(s,
			"vblank fps:%lld count:%u total:%llums total_framecount:%llu\n",
				fps, dpu_crtc->vblank_cb_count,
				ktime_to_ms(diff), dpu_crtc->play_count);

		/* reset time & count for next measurement */
		dpu_crtc->vblank_cb_count = 0;
		dpu_crtc->vblank_cb_time = ktime_set(0, 0);
	}

<<<<<<< HEAD
	seq_printf(s, "vblank_enable:%d\n", dpu_crtc->vblank_requested);

	mutex_unlock(&dpu_crtc->crtc_lock);
=======
	drm_modeset_unlock_all(crtc->dev);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int _dpu_debugfs_status_open(struct inode *inode, struct file *file)
{
	return single_open(file, _dpu_debugfs_status_show, inode->i_private);
}

static ssize_t _dpu_crtc_misr_setup(struct file *file,
		const char __user *user_buf, size_t count, loff_t *ppos)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_mixer *m;
	int i = 0, rc;
	char buf[MISR_BUFF_SIZE + 1];
	u32 frame_count, enable;
	size_t buff_copy;

	if (!file || !file->private_data)
		return -EINVAL;

	dpu_crtc = file->private_data;
	buff_copy = min_t(size_t, count, MISR_BUFF_SIZE);
	if (copy_from_user(buf, user_buf, buff_copy)) {
		DPU_ERROR("buffer copy failed\n");
		return -EINVAL;
	}

	buf[buff_copy] = 0; /* end of string */

	if (sscanf(buf, "%u %u", &enable, &frame_count) != 2)
		return -EINVAL;

	rc = _dpu_crtc_power_enable(dpu_crtc, true);
	if (rc)
		return rc;

	mutex_lock(&dpu_crtc->crtc_lock);
	dpu_crtc->misr_enable = enable;
	dpu_crtc->misr_frame_count = frame_count;
	for (i = 0; i < dpu_crtc->num_mixers; ++i) {
		dpu_crtc->misr_data[i] = 0;
		m = &dpu_crtc->mixers[i];
		if (!m->hw_lm || !m->hw_lm->ops.setup_misr)
			continue;

		m->hw_lm->ops.setup_misr(m->hw_lm, enable, frame_count);
	}
	mutex_unlock(&dpu_crtc->crtc_lock);
	_dpu_crtc_power_enable(dpu_crtc, false);

	return count;
}

static ssize_t _dpu_crtc_misr_read(struct file *file,
		char __user *user_buff, size_t count, loff_t *ppos)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_mixer *m;
	int i = 0, rc;
	u32 misr_status;
	ssize_t len = 0;
	char buf[MISR_BUFF_SIZE + 1] = {'\0'};

	if (*ppos)
		return 0;

	if (!file || !file->private_data)
		return -EINVAL;

	dpu_crtc = file->private_data;
	rc = _dpu_crtc_power_enable(dpu_crtc, true);
	if (rc)
		return rc;

	mutex_lock(&dpu_crtc->crtc_lock);
	if (!dpu_crtc->misr_enable) {
		len += snprintf(buf + len, MISR_BUFF_SIZE - len,
			"disabled\n");
		goto buff_check;
	}

	for (i = 0; i < dpu_crtc->num_mixers; ++i) {
		m = &dpu_crtc->mixers[i];
		if (!m->hw_lm || !m->hw_lm->ops.collect_misr)
			continue;

		misr_status = m->hw_lm->ops.collect_misr(m->hw_lm);
		dpu_crtc->misr_data[i] = misr_status ? misr_status :
							dpu_crtc->misr_data[i];
		len += snprintf(buf + len, MISR_BUFF_SIZE - len, "lm idx:%d\n",
					m->hw_lm->idx - LM_0);
		len += snprintf(buf + len, MISR_BUFF_SIZE - len, "0x%x\n",
							dpu_crtc->misr_data[i]);
	}

buff_check:
	if (count <= len) {
		len = 0;
		goto end;
	}

	if (copy_to_user(user_buff, buf, len)) {
		len = -EFAULT;
		goto end;
	}

	*ppos += len;   /* increase offset */

end:
	mutex_unlock(&dpu_crtc->crtc_lock);
	_dpu_crtc_power_enable(dpu_crtc, false);
	return len;
}

#define DEFINE_DPU_DEBUGFS_SEQ_FOPS(__prefix)                          \
static int __prefix ## _open(struct inode *inode, struct file *file)	\
{									\
	return single_open(file, __prefix ## _show, inode->i_private);	\
}									\
static const struct file_operations __prefix ## _fops = {		\
	.owner = THIS_MODULE,						\
	.open = __prefix ## _open,					\
	.release = single_release,					\
	.read = seq_read,						\
	.llseek = seq_lseek,						\
}
=======
DEFINE_SHOW_ATTRIBUTE(_dpu_debugfs_status);
>>>>>>> upstream/android-13

static int dpu_crtc_debugfs_state_show(struct seq_file *s, void *v)
{
	struct drm_crtc *crtc = (struct drm_crtc *) s->private;
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);
<<<<<<< HEAD
	struct dpu_crtc_res *res;
	struct dpu_crtc_respool *rp;
	int i;
=======
>>>>>>> upstream/android-13

	seq_printf(s, "client type: %d\n", dpu_crtc_get_client_type(crtc));
	seq_printf(s, "intf_mode: %d\n", dpu_crtc_get_intf_mode(crtc));
	seq_printf(s, "core_clk_rate: %llu\n",
			dpu_crtc->cur_perf.core_clk_rate);
<<<<<<< HEAD
	for (i = DPU_POWER_HANDLE_DBUS_ID_MNOC;
			i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
		seq_printf(s, "bw_ctl[%s]: %llu\n",
				dpu_power_handle_get_dbus_name(i),
				dpu_crtc->cur_perf.bw_ctl[i]);
		seq_printf(s, "max_per_pipe_ib[%s]: %llu\n",
				dpu_power_handle_get_dbus_name(i),
				dpu_crtc->cur_perf.max_per_pipe_ib[i]);
	}

	mutex_lock(&dpu_crtc->rp_lock);
	list_for_each_entry(rp, &dpu_crtc->rp_head, rp_list) {
		seq_printf(s, "rp.%d: ", rp->sequence_id);
		list_for_each_entry(res, &rp->res_list, list)
			seq_printf(s, "0x%x/0x%llx/%pK/%d ",
					res->type, res->tag, res->val,
					atomic_read(&res->refcount));
		seq_puts(s, "\n");
	}
	mutex_unlock(&dpu_crtc->rp_lock);

	return 0;
}
DEFINE_DPU_DEBUGFS_SEQ_FOPS(dpu_crtc_debugfs_state);

static int _dpu_crtc_init_debugfs(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc;
	struct dpu_kms *dpu_kms;

	static const struct file_operations debugfs_status_fops = {
		.open =		_dpu_debugfs_status_open,
		.read =		seq_read,
		.llseek =	seq_lseek,
		.release =	single_release,
	};
	static const struct file_operations debugfs_misr_fops = {
		.open =		simple_open,
		.read =		_dpu_crtc_misr_read,
		.write =	_dpu_crtc_misr_setup,
	};

	if (!crtc)
		return -EINVAL;
	dpu_crtc = to_dpu_crtc(crtc);

	dpu_kms = _dpu_crtc_get_kms(crtc);
	if (!dpu_kms)
		return -EINVAL;

	dpu_crtc->debugfs_root = debugfs_create_dir(dpu_crtc->name,
			crtc->dev->primary->debugfs_root);
	if (!dpu_crtc->debugfs_root)
		return -ENOMEM;

	/* don't error check these */
	debugfs_create_file("status", 0400,
			dpu_crtc->debugfs_root,
			dpu_crtc, &debugfs_status_fops);
=======
	seq_printf(s, "bw_ctl: %llu\n", dpu_crtc->cur_perf.bw_ctl);
	seq_printf(s, "max_per_pipe_ib: %llu\n",
				dpu_crtc->cur_perf.max_per_pipe_ib);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dpu_crtc_debugfs_state);

static int _dpu_crtc_init_debugfs(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);

	dpu_crtc->debugfs_root = debugfs_create_dir(dpu_crtc->name,
			crtc->dev->primary->debugfs_root);

	debugfs_create_file("status", 0400,
			dpu_crtc->debugfs_root,
			dpu_crtc, &_dpu_debugfs_status_fops);
>>>>>>> upstream/android-13
	debugfs_create_file("state", 0600,
			dpu_crtc->debugfs_root,
			&dpu_crtc->base,
			&dpu_crtc_debugfs_state_fops);
<<<<<<< HEAD
	debugfs_create_file("misr_data", 0600, dpu_crtc->debugfs_root,
					dpu_crtc, &debugfs_misr_fops);

	return 0;
}

static void _dpu_crtc_destroy_debugfs(struct drm_crtc *crtc)
{
	struct dpu_crtc *dpu_crtc;

	if (!crtc)
		return;
	dpu_crtc = to_dpu_crtc(crtc);
	debugfs_remove_recursive(dpu_crtc->debugfs_root);
}
=======

	return 0;
}
>>>>>>> upstream/android-13
#else
static int _dpu_crtc_init_debugfs(struct drm_crtc *crtc)
{
	return 0;
}
<<<<<<< HEAD

static void _dpu_crtc_destroy_debugfs(struct drm_crtc *crtc)
{
}
=======
>>>>>>> upstream/android-13
#endif /* CONFIG_DEBUG_FS */

static int dpu_crtc_late_register(struct drm_crtc *crtc)
{
	return _dpu_crtc_init_debugfs(crtc);
}

static void dpu_crtc_early_unregister(struct drm_crtc *crtc)
{
<<<<<<< HEAD
	_dpu_crtc_destroy_debugfs(crtc);
=======
	struct dpu_crtc *dpu_crtc = to_dpu_crtc(crtc);

	debugfs_remove_recursive(dpu_crtc->debugfs_root);
>>>>>>> upstream/android-13
}

static const struct drm_crtc_funcs dpu_crtc_funcs = {
	.set_config = drm_atomic_helper_set_config,
	.destroy = dpu_crtc_destroy,
	.page_flip = drm_atomic_helper_page_flip,
	.reset = dpu_crtc_reset,
	.atomic_duplicate_state = dpu_crtc_duplicate_state,
	.atomic_destroy_state = dpu_crtc_destroy_state,
	.late_register = dpu_crtc_late_register,
	.early_unregister = dpu_crtc_early_unregister,
<<<<<<< HEAD
};

static const struct drm_crtc_helper_funcs dpu_crtc_helper_funcs = {
	.disable = dpu_crtc_disable,
=======
	.enable_vblank  = msm_crtc_enable_vblank,
	.disable_vblank = msm_crtc_disable_vblank,
	.get_vblank_timestamp = drm_crtc_vblank_helper_get_vblank_timestamp,
	.get_vblank_counter = dpu_crtc_get_vblank_counter,
};

static const struct drm_crtc_helper_funcs dpu_crtc_helper_funcs = {
	.atomic_disable = dpu_crtc_disable,
>>>>>>> upstream/android-13
	.atomic_enable = dpu_crtc_enable,
	.atomic_check = dpu_crtc_atomic_check,
	.atomic_begin = dpu_crtc_atomic_begin,
	.atomic_flush = dpu_crtc_atomic_flush,
<<<<<<< HEAD
};

/* initialize crtc */
struct drm_crtc *dpu_crtc_init(struct drm_device *dev, struct drm_plane *plane)
{
	struct drm_crtc *crtc = NULL;
	struct dpu_crtc *dpu_crtc = NULL;
	struct msm_drm_private *priv = NULL;
	struct dpu_kms *kms = NULL;
	int i;

	priv = dev->dev_private;
	kms = to_dpu_kms(priv->kms);

=======
	.get_scanout_position = dpu_crtc_get_scanout_position,
};

/* initialize crtc */
struct drm_crtc *dpu_crtc_init(struct drm_device *dev, struct drm_plane *plane,
				struct drm_plane *cursor)
{
	struct drm_crtc *crtc = NULL;
	struct dpu_crtc *dpu_crtc = NULL;
	int i;

>>>>>>> upstream/android-13
	dpu_crtc = kzalloc(sizeof(*dpu_crtc), GFP_KERNEL);
	if (!dpu_crtc)
		return ERR_PTR(-ENOMEM);

	crtc = &dpu_crtc->base;
	crtc->dev = dev;

<<<<<<< HEAD
	mutex_init(&dpu_crtc->crtc_lock);
	spin_lock_init(&dpu_crtc->spin_lock);
	atomic_set(&dpu_crtc->frame_pending, 0);

	mutex_init(&dpu_crtc->rp_lock);
	INIT_LIST_HEAD(&dpu_crtc->rp_head);

=======
	spin_lock_init(&dpu_crtc->spin_lock);
	atomic_set(&dpu_crtc->frame_pending, 0);

>>>>>>> upstream/android-13
	init_completion(&dpu_crtc->frame_done_comp);

	INIT_LIST_HEAD(&dpu_crtc->frame_event_list);

	for (i = 0; i < ARRAY_SIZE(dpu_crtc->frame_events); i++) {
		INIT_LIST_HEAD(&dpu_crtc->frame_events[i].list);
		list_add(&dpu_crtc->frame_events[i].list,
				&dpu_crtc->frame_event_list);
		kthread_init_work(&dpu_crtc->frame_events[i].work,
				dpu_crtc_frame_event_work);
	}

<<<<<<< HEAD
	drm_crtc_init_with_planes(dev, crtc, plane, NULL, &dpu_crtc_funcs,
=======
	drm_crtc_init_with_planes(dev, crtc, plane, cursor, &dpu_crtc_funcs,
>>>>>>> upstream/android-13
				NULL);

	drm_crtc_helper_add(crtc, &dpu_crtc_helper_funcs);

<<<<<<< HEAD
=======
	drm_crtc_enable_color_mgmt(crtc, 0, true, 0);

>>>>>>> upstream/android-13
	/* save user friendly CRTC name for later */
	snprintf(dpu_crtc->name, DPU_CRTC_NAME_SIZE, "crtc%u", crtc->base.id);

	/* initialize event handling */
	spin_lock_init(&dpu_crtc->event_lock);

<<<<<<< HEAD
	dpu_crtc->phandle = &kms->phandle;

	DPU_DEBUG("%s: successfully initialized crtc\n", dpu_crtc->name);
=======
	DRM_DEBUG_KMS("%s: successfully initialized crtc\n", dpu_crtc->name);
>>>>>>> upstream/android-13
	return crtc;
}
