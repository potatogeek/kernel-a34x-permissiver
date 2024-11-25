<<<<<<< HEAD
/* Copyright (c) 2015-2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2015-2018, 2020-2021 The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__
#include "dpu_encoder_phys.h"
#include "dpu_hw_interrupts.h"
<<<<<<< HEAD
#include "dpu_core_irq.h"
#include "dpu_formats.h"
#include "dpu_trace.h"

#define DPU_DEBUG_VIDENC(e, fmt, ...) DPU_DEBUG("enc%d intf%d " fmt, \
		(e) && (e)->base.parent ? \
		(e)->base.parent->base.id : -1, \
=======
#include "dpu_hw_merge3d.h"
#include "dpu_core_irq.h"
#include "dpu_formats.h"
#include "dpu_trace.h"
#include "disp/msm_disp_snapshot.h"

#define DPU_DEBUG_VIDENC(e, fmt, ...) DPU_DEBUG("enc%d intf%d " fmt, \
		(e) && (e)->parent ? \
		(e)->parent->base.id : -1, \
>>>>>>> upstream/android-13
		(e) && (e)->hw_intf ? \
		(e)->hw_intf->idx - INTF_0 : -1, ##__VA_ARGS__)

#define DPU_ERROR_VIDENC(e, fmt, ...) DPU_ERROR("enc%d intf%d " fmt, \
<<<<<<< HEAD
		(e) && (e)->base.parent ? \
		(e)->base.parent->base.id : -1, \
=======
		(e) && (e)->parent ? \
		(e)->parent->base.id : -1, \
>>>>>>> upstream/android-13
		(e) && (e)->hw_intf ? \
		(e)->hw_intf->idx - INTF_0 : -1, ##__VA_ARGS__)

#define to_dpu_encoder_phys_vid(x) \
	container_of(x, struct dpu_encoder_phys_vid, base)

static bool dpu_encoder_phys_vid_is_master(
		struct dpu_encoder_phys *phys_enc)
{
	bool ret = false;

	if (phys_enc->split_role != ENC_ROLE_SLAVE)
		ret = true;

	return ret;
}

static void drm_mode_to_intf_timing_params(
<<<<<<< HEAD
		const struct dpu_encoder_phys_vid *vid_enc,
=======
		const struct dpu_encoder_phys *phys_enc,
>>>>>>> upstream/android-13
		const struct drm_display_mode *mode,
		struct intf_timing_params *timing)
{
	memset(timing, 0, sizeof(*timing));

	if ((mode->htotal < mode->hsync_end)
			|| (mode->hsync_start < mode->hdisplay)
			|| (mode->vtotal < mode->vsync_end)
			|| (mode->vsync_start < mode->vdisplay)
			|| (mode->hsync_end < mode->hsync_start)
			|| (mode->vsync_end < mode->vsync_start)) {
		DPU_ERROR(
		    "invalid params - hstart:%d,hend:%d,htot:%d,hdisplay:%d\n",
				mode->hsync_start, mode->hsync_end,
				mode->htotal, mode->hdisplay);
		DPU_ERROR("vstart:%d,vend:%d,vtot:%d,vdisplay:%d\n",
				mode->vsync_start, mode->vsync_end,
				mode->vtotal, mode->vdisplay);
		return;
	}

	/*
	 * https://www.kernel.org/doc/htmldocs/drm/ch02s05.html
	 *  Active Region      Front Porch   Sync   Back Porch
	 * <-----------------><------------><-----><----------->
	 * <- [hv]display --->
	 * <--------- [hv]sync_start ------>
	 * <----------------- [hv]sync_end ------->
	 * <---------------------------- [hv]total ------------->
	 */
	timing->width = mode->hdisplay;	/* active width */
	timing->height = mode->vdisplay;	/* active height */
	timing->xres = timing->width;
	timing->yres = timing->height;
	timing->h_back_porch = mode->htotal - mode->hsync_end;
	timing->h_front_porch = mode->hsync_start - mode->hdisplay;
	timing->v_back_porch = mode->vtotal - mode->vsync_end;
	timing->v_front_porch = mode->vsync_start - mode->vdisplay;
	timing->hsync_pulse_width = mode->hsync_end - mode->hsync_start;
	timing->vsync_pulse_width = mode->vsync_end - mode->vsync_start;
	timing->hsync_polarity = (mode->flags & DRM_MODE_FLAG_NHSYNC) ? 1 : 0;
	timing->vsync_polarity = (mode->flags & DRM_MODE_FLAG_NVSYNC) ? 1 : 0;
	timing->border_clr = 0;
	timing->underflow_clr = 0xff;
	timing->hsync_skew = mode->hskew;

	/* DSI controller cannot handle active-low sync signals. */
<<<<<<< HEAD
	if (vid_enc->hw_intf->cap->type == INTF_DSI) {
=======
	if (phys_enc->hw_intf->cap->type == INTF_DSI) {
>>>>>>> upstream/android-13
		timing->hsync_polarity = 0;
		timing->vsync_polarity = 0;
	}

	/*
	 * For edp only:
	 * DISPLAY_V_START = (VBP * HCYCLE) + HBP
	 * DISPLAY_V_END = (VBP + VACTIVE) * HCYCLE - 1 - HFP
	 */
	/*
	 * if (vid_enc->hw->cap->type == INTF_EDP) {
	 * display_v_start += mode->htotal - mode->hsync_start;
	 * display_v_end -= mode->hsync_start - mode->hdisplay;
	 * }
	 */
<<<<<<< HEAD
}

static inline u32 get_horizontal_total(const struct intf_timing_params *timing)
=======
	/* for DP/EDP, Shift timings to align it to bottom right */
	if ((phys_enc->hw_intf->cap->type == INTF_DP) ||
		(phys_enc->hw_intf->cap->type == INTF_EDP)) {
		timing->h_back_porch += timing->h_front_porch;
		timing->h_front_porch = 0;
		timing->v_back_porch += timing->v_front_porch;
		timing->v_front_porch = 0;
	}
}

static u32 get_horizontal_total(const struct intf_timing_params *timing)
>>>>>>> upstream/android-13
{
	u32 active = timing->xres;
	u32 inactive =
	    timing->h_back_porch + timing->h_front_porch +
	    timing->hsync_pulse_width;
	return active + inactive;
}

<<<<<<< HEAD
static inline u32 get_vertical_total(const struct intf_timing_params *timing)
=======
static u32 get_vertical_total(const struct intf_timing_params *timing)
>>>>>>> upstream/android-13
{
	u32 active = timing->yres;
	u32 inactive =
	    timing->v_back_porch + timing->v_front_porch +
	    timing->vsync_pulse_width;
	return active + inactive;
}

/*
 * programmable_fetch_get_num_lines:
 *	Number of fetch lines in vertical front porch
 * @timing: Pointer to the intf timing information for the requested mode
 *
 * Returns the number of fetch lines in vertical front porch at which mdp
 * can start fetching the next frame.
 *
 * Number of needed prefetch lines is anything that cannot be absorbed in the
 * start of frame time (back porch + vsync pulse width).
 *
 * Some panels have very large VFP, however we only need a total number of
 * lines based on the chip worst case latencies.
 */
static u32 programmable_fetch_get_num_lines(
<<<<<<< HEAD
		struct dpu_encoder_phys_vid *vid_enc,
		const struct intf_timing_params *timing)
{
	u32 worst_case_needed_lines =
	    vid_enc->hw_intf->cap->prog_fetch_lines_worst_case;
=======
		struct dpu_encoder_phys *phys_enc,
		const struct intf_timing_params *timing)
{
	u32 worst_case_needed_lines =
	    phys_enc->hw_intf->cap->prog_fetch_lines_worst_case;
>>>>>>> upstream/android-13
	u32 start_of_frame_lines =
	    timing->v_back_porch + timing->vsync_pulse_width;
	u32 needed_vfp_lines = worst_case_needed_lines - start_of_frame_lines;
	u32 actual_vfp_lines = 0;

	/* Fetch must be outside active lines, otherwise undefined. */
	if (start_of_frame_lines >= worst_case_needed_lines) {
<<<<<<< HEAD
		DPU_DEBUG_VIDENC(vid_enc,
=======
		DPU_DEBUG_VIDENC(phys_enc,
>>>>>>> upstream/android-13
				"prog fetch is not needed, large vbp+vsw\n");
		actual_vfp_lines = 0;
	} else if (timing->v_front_porch < needed_vfp_lines) {
		/* Warn fetch needed, but not enough porch in panel config */
		pr_warn_once
			("low vbp+vfp may lead to perf issues in some cases\n");
<<<<<<< HEAD
		DPU_DEBUG_VIDENC(vid_enc,
				"less vfp than fetch req, using entire vfp\n");
		actual_vfp_lines = timing->v_front_porch;
	} else {
		DPU_DEBUG_VIDENC(vid_enc, "room in vfp for needed prefetch\n");
		actual_vfp_lines = needed_vfp_lines;
	}

	DPU_DEBUG_VIDENC(vid_enc,
		"v_front_porch %u v_back_porch %u vsync_pulse_width %u\n",
		timing->v_front_porch, timing->v_back_porch,
		timing->vsync_pulse_width);
	DPU_DEBUG_VIDENC(vid_enc,
=======
		DPU_DEBUG_VIDENC(phys_enc,
				"less vfp than fetch req, using entire vfp\n");
		actual_vfp_lines = timing->v_front_porch;
	} else {
		DPU_DEBUG_VIDENC(phys_enc, "room in vfp for needed prefetch\n");
		actual_vfp_lines = needed_vfp_lines;
	}

	DPU_DEBUG_VIDENC(phys_enc,
		"v_front_porch %u v_back_porch %u vsync_pulse_width %u\n",
		timing->v_front_porch, timing->v_back_porch,
		timing->vsync_pulse_width);
	DPU_DEBUG_VIDENC(phys_enc,
>>>>>>> upstream/android-13
		"wc_lines %u needed_vfp_lines %u actual_vfp_lines %u\n",
		worst_case_needed_lines, needed_vfp_lines, actual_vfp_lines);

	return actual_vfp_lines;
}

/*
 * programmable_fetch_config: Programs HW to prefetch lines by offsetting
 *	the start of fetch into the vertical front porch for cases where the
 *	vsync pulse width and vertical back porch time is insufficient
 *
 *	Gets # of lines to pre-fetch, then calculate VSYNC counter value.
 *	HW layer requires VSYNC counter of first pixel of tgt VFP line.
 *
 * @timing: Pointer to the intf timing information for the requested mode
 */
static void programmable_fetch_config(struct dpu_encoder_phys *phys_enc,
				      const struct intf_timing_params *timing)
{
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc =
		to_dpu_encoder_phys_vid(phys_enc);
=======
>>>>>>> upstream/android-13
	struct intf_prog_fetch f = { 0 };
	u32 vfp_fetch_lines = 0;
	u32 horiz_total = 0;
	u32 vert_total = 0;
	u32 vfp_fetch_start_vsync_counter = 0;
	unsigned long lock_flags;

<<<<<<< HEAD
	if (WARN_ON_ONCE(!vid_enc->hw_intf->ops.setup_prg_fetch))
		return;

	vfp_fetch_lines = programmable_fetch_get_num_lines(vid_enc, timing);
=======
	if (WARN_ON_ONCE(!phys_enc->hw_intf->ops.setup_prg_fetch))
		return;

	vfp_fetch_lines = programmable_fetch_get_num_lines(phys_enc, timing);
>>>>>>> upstream/android-13
	if (vfp_fetch_lines) {
		vert_total = get_vertical_total(timing);
		horiz_total = get_horizontal_total(timing);
		vfp_fetch_start_vsync_counter =
		    (vert_total - vfp_fetch_lines) * horiz_total + 1;
		f.enable = 1;
		f.fetch_start = vfp_fetch_start_vsync_counter;
	}

<<<<<<< HEAD
	DPU_DEBUG_VIDENC(vid_enc,
=======
	DPU_DEBUG_VIDENC(phys_enc,
>>>>>>> upstream/android-13
		"vfp_fetch_lines %u vfp_fetch_start_vsync_counter %u\n",
		vfp_fetch_lines, vfp_fetch_start_vsync_counter);

	spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
<<<<<<< HEAD
	vid_enc->hw_intf->ops.setup_prg_fetch(vid_enc->hw_intf, &f);
=======
	phys_enc->hw_intf->ops.setup_prg_fetch(phys_enc->hw_intf, &f);
>>>>>>> upstream/android-13
	spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);
}

static bool dpu_encoder_phys_vid_mode_fixup(
		struct dpu_encoder_phys *phys_enc,
		const struct drm_display_mode *mode,
		struct drm_display_mode *adj_mode)
{
<<<<<<< HEAD
	if (phys_enc)
		DPU_DEBUG_VIDENC(to_dpu_encoder_phys_vid(phys_enc), "\n");
=======
	DPU_DEBUG_VIDENC(phys_enc, "\n");
>>>>>>> upstream/android-13

	/*
	 * Modifying mode has consequences when the mode comes back to us
	 */
	return true;
}

static void dpu_encoder_phys_vid_setup_timing_engine(
		struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc;
=======
>>>>>>> upstream/android-13
	struct drm_display_mode mode;
	struct intf_timing_params timing_params = { 0 };
	const struct dpu_format *fmt = NULL;
	u32 fmt_fourcc = DRM_FORMAT_RGB888;
	unsigned long lock_flags;
	struct dpu_hw_intf_cfg intf_cfg = { 0 };

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_ctl->ops.setup_intf_cfg) {
		DPU_ERROR("invalid encoder %d\n", phys_enc != 0);
=======
	if (!phys_enc->hw_ctl->ops.setup_intf_cfg) {
		DPU_ERROR("invalid encoder %d\n", phys_enc != NULL);
>>>>>>> upstream/android-13
		return;
	}

	mode = phys_enc->cached_mode;
<<<<<<< HEAD
	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	if (!vid_enc->hw_intf->ops.setup_timing_gen) {
=======
	if (!phys_enc->hw_intf->ops.setup_timing_gen) {
>>>>>>> upstream/android-13
		DPU_ERROR("timing engine setup is not supported\n");
		return;
	}

<<<<<<< HEAD
	DPU_DEBUG_VIDENC(vid_enc, "enabling mode:\n");
=======
	DPU_DEBUG_VIDENC(phys_enc, "enabling mode:\n");
>>>>>>> upstream/android-13
	drm_mode_debug_printmodeline(&mode);

	if (phys_enc->split_role != ENC_ROLE_SOLO) {
		mode.hdisplay >>= 1;
		mode.htotal >>= 1;
		mode.hsync_start >>= 1;
		mode.hsync_end >>= 1;

<<<<<<< HEAD
		DPU_DEBUG_VIDENC(vid_enc,
=======
		DPU_DEBUG_VIDENC(phys_enc,
>>>>>>> upstream/android-13
			"split_role %d, halve horizontal %d %d %d %d\n",
			phys_enc->split_role,
			mode.hdisplay, mode.htotal,
			mode.hsync_start, mode.hsync_end);
	}

<<<<<<< HEAD
	drm_mode_to_intf_timing_params(vid_enc, &mode, &timing_params);

	fmt = dpu_get_dpu_format(fmt_fourcc);
	DPU_DEBUG_VIDENC(vid_enc, "fmt_fourcc 0x%X\n", fmt_fourcc);

	intf_cfg.intf = vid_enc->hw_intf->idx;
	intf_cfg.intf_mode_sel = DPU_CTL_MODE_SEL_VID;
	intf_cfg.stream_sel = 0; /* Don't care value for video mode */
	intf_cfg.mode_3d = dpu_encoder_helper_get_3d_blend_mode(phys_enc);

	spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
	vid_enc->hw_intf->ops.setup_timing_gen(vid_enc->hw_intf,
			&timing_params, fmt);
	phys_enc->hw_ctl->ops.setup_intf_cfg(phys_enc->hw_ctl, &intf_cfg);
	spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);

	programmable_fetch_config(phys_enc, &timing_params);

	vid_enc->timing_params = timing_params;
=======
	drm_mode_to_intf_timing_params(phys_enc, &mode, &timing_params);

	fmt = dpu_get_dpu_format(fmt_fourcc);
	DPU_DEBUG_VIDENC(phys_enc, "fmt_fourcc 0x%X\n", fmt_fourcc);

	intf_cfg.intf = phys_enc->hw_intf->idx;
	intf_cfg.intf_mode_sel = DPU_CTL_MODE_SEL_VID;
	intf_cfg.stream_sel = 0; /* Don't care value for video mode */
	intf_cfg.mode_3d = dpu_encoder_helper_get_3d_blend_mode(phys_enc);
	if (phys_enc->hw_pp->merge_3d)
		intf_cfg.merge_3d = phys_enc->hw_pp->merge_3d->idx;

	spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
	phys_enc->hw_intf->ops.setup_timing_gen(phys_enc->hw_intf,
			&timing_params, fmt);
	phys_enc->hw_ctl->ops.setup_intf_cfg(phys_enc->hw_ctl, &intf_cfg);

	/* setup which pp blk will connect to this intf */
	if (phys_enc->hw_intf->ops.bind_pingpong_blk)
		phys_enc->hw_intf->ops.bind_pingpong_blk(
				phys_enc->hw_intf,
				true,
				phys_enc->hw_pp->idx);

	if (phys_enc->hw_pp->merge_3d)
		phys_enc->hw_pp->merge_3d->ops.setup_3d_mode(phys_enc->hw_pp->merge_3d, intf_cfg.mode_3d);

	spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);

	programmable_fetch_config(phys_enc, &timing_params);
>>>>>>> upstream/android-13
}

static void dpu_encoder_phys_vid_vblank_irq(void *arg, int irq_idx)
{
	struct dpu_encoder_phys *phys_enc = arg;
	struct dpu_hw_ctl *hw_ctl;
	unsigned long lock_flags;
	u32 flush_register = 0;
<<<<<<< HEAD
	int new_cnt = -1, old_cnt = -1;

	if (!phys_enc)
		return;

	hw_ctl = phys_enc->hw_ctl;
	if (!hw_ctl)
		return;
=======

	hw_ctl = phys_enc->hw_ctl;
>>>>>>> upstream/android-13

	DPU_ATRACE_BEGIN("vblank_irq");

	if (phys_enc->parent_ops->handle_vblank_virt)
		phys_enc->parent_ops->handle_vblank_virt(phys_enc->parent,
				phys_enc);

<<<<<<< HEAD
	old_cnt  = atomic_read(&phys_enc->pending_kickoff_cnt);
=======
	atomic_read(&phys_enc->pending_kickoff_cnt);
>>>>>>> upstream/android-13

	/*
	 * only decrement the pending flush count if we've actually flushed
	 * hardware. due to sw irq latency, vblank may have already happened
	 * so we need to double-check with hw that it accepted the flush bits
	 */
	spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
<<<<<<< HEAD
	if (hw_ctl && hw_ctl->ops.get_flush_register)
		flush_register = hw_ctl->ops.get_flush_register(hw_ctl);

	if (!(flush_register & hw_ctl->ops.get_pending_flush(hw_ctl)))
		new_cnt = atomic_add_unless(&phys_enc->pending_kickoff_cnt,
				-1, 0);
=======
	if (hw_ctl->ops.get_flush_register)
		flush_register = hw_ctl->ops.get_flush_register(hw_ctl);

	if (!(flush_register & hw_ctl->ops.get_pending_flush(hw_ctl)))
		atomic_add_unless(&phys_enc->pending_kickoff_cnt, -1, 0);
>>>>>>> upstream/android-13
	spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);

	/* Signal any waiting atomic commit thread */
	wake_up_all(&phys_enc->pending_kickoff_wq);
<<<<<<< HEAD
=======

	phys_enc->parent_ops->handle_frame_done(phys_enc->parent, phys_enc,
			DPU_ENCODER_FRAME_EVENT_DONE);

>>>>>>> upstream/android-13
	DPU_ATRACE_END("vblank_irq");
}

static void dpu_encoder_phys_vid_underrun_irq(void *arg, int irq_idx)
{
	struct dpu_encoder_phys *phys_enc = arg;

<<<<<<< HEAD
	if (!phys_enc)
		return;

=======
>>>>>>> upstream/android-13
	if (phys_enc->parent_ops->handle_underrun_virt)
		phys_enc->parent_ops->handle_underrun_virt(phys_enc->parent,
			phys_enc);
}

<<<<<<< HEAD
static bool _dpu_encoder_phys_is_dual_ctl(struct dpu_encoder_phys *phys_enc)
{
	if (!phys_enc)
		return false;

	if (phys_enc->topology_name == DPU_RM_TOPOLOGY_DUALPIPE)
		return true;

	return false;
}

static bool dpu_encoder_phys_vid_needs_single_flush(
		struct dpu_encoder_phys *phys_enc)
{
	return (phys_enc && _dpu_encoder_phys_is_dual_ctl(phys_enc));
}

static void _dpu_encoder_phys_vid_setup_irq_hw_idx(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_irq *irq;

	/*
	 * Initialize irq->hw_idx only when irq is not registered.
	 * Prevent invalidating irq->irq_idx as modeset may be
	 * called many times during dfps.
	 */

	irq = &phys_enc->irq[INTR_IDX_VSYNC];
	if (irq->irq_idx < 0)
		irq->hw_idx = phys_enc->intf_idx;

	irq = &phys_enc->irq[INTR_IDX_UNDERRUN];
	if (irq->irq_idx < 0)
		irq->hw_idx = phys_enc->intf_idx;
=======
static bool dpu_encoder_phys_vid_needs_single_flush(
		struct dpu_encoder_phys *phys_enc)
{
	return phys_enc->split_role != ENC_ROLE_SOLO;
>>>>>>> upstream/android-13
}

static void dpu_encoder_phys_vid_mode_set(
		struct dpu_encoder_phys *phys_enc,
		struct drm_display_mode *mode,
		struct drm_display_mode *adj_mode)
{
<<<<<<< HEAD
	struct dpu_rm *rm;
	struct dpu_rm_hw_iter iter;
	int i, instance;
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc || !phys_enc->dpu_kms) {
		DPU_ERROR("invalid encoder/kms\n");
		return;
	}

	rm = &phys_enc->dpu_kms->rm;
	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
=======
	struct dpu_encoder_irq *irq;
>>>>>>> upstream/android-13

	if (adj_mode) {
		phys_enc->cached_mode = *adj_mode;
		drm_mode_debug_printmodeline(adj_mode);
<<<<<<< HEAD
		DPU_DEBUG_VIDENC(vid_enc, "caching mode:\n");
	}

	instance = phys_enc->split_role == ENC_ROLE_SLAVE ? 1 : 0;

	/* Retrieve previously allocated HW Resources. Shouldn't fail */
	dpu_rm_init_hw_iter(&iter, phys_enc->parent->base.id, DPU_HW_BLK_CTL);
	for (i = 0; i <= instance; i++) {
		if (dpu_rm_get_hw(rm, &iter))
			phys_enc->hw_ctl = (struct dpu_hw_ctl *)iter.hw;
	}
	if (IS_ERR_OR_NULL(phys_enc->hw_ctl)) {
		DPU_ERROR_VIDENC(vid_enc, "failed to init ctl, %ld\n",
				PTR_ERR(phys_enc->hw_ctl));
		phys_enc->hw_ctl = NULL;
		return;
	}

	_dpu_encoder_phys_vid_setup_irq_hw_idx(phys_enc);
=======
		DPU_DEBUG_VIDENC(phys_enc, "caching mode:\n");
	}

	irq = &phys_enc->irq[INTR_IDX_VSYNC];
	irq->irq_idx = phys_enc->hw_intf->cap->intr_vsync;

	irq = &phys_enc->irq[INTR_IDX_UNDERRUN];
	irq->irq_idx = phys_enc->hw_intf->cap->intr_underrun;
>>>>>>> upstream/android-13
}

static int dpu_encoder_phys_vid_control_vblank_irq(
		struct dpu_encoder_phys *phys_enc,
		bool enable)
{
	int ret = 0;
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc;
	int refcount;

	if (!phys_enc) {
		DPU_ERROR("invalid encoder\n");
		return -EINVAL;
	}

	refcount = atomic_read(&phys_enc->vblank_refcount);
	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
=======
	int refcount;

	refcount = atomic_read(&phys_enc->vblank_refcount);
>>>>>>> upstream/android-13

	/* Slave encoders don't report vblank */
	if (!dpu_encoder_phys_vid_is_master(phys_enc))
		goto end;

	/* protect against negative */
	if (!enable && refcount == 0) {
		ret = -EINVAL;
		goto end;
	}

<<<<<<< HEAD
	DRM_DEBUG_KMS("id:%u enable=%d/%d\n", DRMID(phys_enc->parent), enable,
=======
	DRM_DEBUG_VBL("id:%u enable=%d/%d\n", DRMID(phys_enc->parent), enable,
>>>>>>> upstream/android-13
		      atomic_read(&phys_enc->vblank_refcount));

	if (enable && atomic_inc_return(&phys_enc->vblank_refcount) == 1)
		ret = dpu_encoder_helper_register_irq(phys_enc, INTR_IDX_VSYNC);
	else if (!enable && atomic_dec_return(&phys_enc->vblank_refcount) == 0)
		ret = dpu_encoder_helper_unregister_irq(phys_enc,
				INTR_IDX_VSYNC);

end:
	if (ret) {
		DRM_ERROR("failed: id:%u intf:%d ret:%d enable:%d refcnt:%d\n",
			  DRMID(phys_enc->parent),
<<<<<<< HEAD
			  vid_enc->hw_intf->idx - INTF_0, ret, enable,
=======
			  phys_enc->hw_intf->idx - INTF_0, ret, enable,
>>>>>>> upstream/android-13
			  refcount);
	}
	return ret;
}

static void dpu_encoder_phys_vid_enable(struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	struct msm_drm_private *priv;
	struct dpu_encoder_phys_vid *vid_enc;
	struct dpu_hw_intf *intf;
	struct dpu_hw_ctl *ctl;
	u32 flush_mask = 0;

	if (!phys_enc || !phys_enc->parent || !phys_enc->parent->dev ||
			!phys_enc->parent->dev->dev_private) {
		DPU_ERROR("invalid encoder/device\n");
		return;
	}
	priv = phys_enc->parent->dev->dev_private;

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	intf = vid_enc->hw_intf;
	ctl = phys_enc->hw_ctl;
	if (!vid_enc->hw_intf || !phys_enc->hw_ctl) {
		DPU_ERROR("invalid hw_intf %d hw_ctl %d\n",
				vid_enc->hw_intf != 0, phys_enc->hw_ctl != 0);
		return;
	}

	DPU_DEBUG_VIDENC(vid_enc, "\n");

	if (WARN_ON(!vid_enc->hw_intf->ops.enable_timing))
		return;

	dpu_encoder_helper_split_config(phys_enc, vid_enc->hw_intf->idx);
=======
	struct dpu_hw_ctl *ctl;

	ctl = phys_enc->hw_ctl;

	DPU_DEBUG_VIDENC(phys_enc, "\n");

	if (WARN_ON(!phys_enc->hw_intf->ops.enable_timing))
		return;

	dpu_encoder_helper_split_config(phys_enc, phys_enc->hw_intf->idx);
>>>>>>> upstream/android-13

	dpu_encoder_phys_vid_setup_timing_engine(phys_enc);

	/*
	 * For single flush cases (dual-ctl or pp-split), skip setting the
	 * flush bit for the slave intf, since both intfs use same ctl
	 * and HW will only flush the master.
	 */
	if (dpu_encoder_phys_vid_needs_single_flush(phys_enc) &&
		!dpu_encoder_phys_vid_is_master(phys_enc))
		goto skip_flush;

<<<<<<< HEAD
	ctl->ops.get_bitmask_intf(ctl, &flush_mask, intf->idx);
	ctl->ops.update_pending_flush(ctl, flush_mask);

skip_flush:
	DPU_DEBUG_VIDENC(vid_enc, "update pending flush ctl %d flush_mask %x\n",
		ctl->idx - CTL_0, flush_mask);
=======
	ctl->ops.update_pending_flush_intf(ctl, phys_enc->hw_intf->idx);
	if (ctl->ops.update_pending_flush_merge_3d && phys_enc->hw_pp->merge_3d)
		ctl->ops.update_pending_flush_merge_3d(ctl, phys_enc->hw_pp->merge_3d->idx);

skip_flush:
	DPU_DEBUG_VIDENC(phys_enc,
		"update pending flush ctl %d intf %d\n",
		ctl->idx - CTL_0, phys_enc->hw_intf->idx);

	atomic_set(&phys_enc->underrun_cnt, 0);
>>>>>>> upstream/android-13

	/* ctl_flush & timing engine enable will be triggered by framework */
	if (phys_enc->enable_state == DPU_ENC_DISABLED)
		phys_enc->enable_state = DPU_ENC_ENABLING;
}

static void dpu_encoder_phys_vid_destroy(struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc) {
		DPU_ERROR("invalid encoder\n");
		return;
	}

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	DPU_DEBUG_VIDENC(vid_enc, "\n");
	kfree(vid_enc);
=======
	DPU_DEBUG_VIDENC(phys_enc, "\n");
	kfree(phys_enc);
>>>>>>> upstream/android-13
}

static void dpu_encoder_phys_vid_get_hw_resources(
		struct dpu_encoder_phys *phys_enc,
<<<<<<< HEAD
		struct dpu_encoder_hw_resources *hw_res,
		struct drm_connector_state *conn_state)
{
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc || !hw_res) {
		DPU_ERROR("invalid arg(s), enc %d hw_res %d conn_state %d\n",
				phys_enc != 0, hw_res != 0, conn_state != 0);
		return;
	}

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	if (!vid_enc->hw_intf) {
		DPU_ERROR("invalid arg(s), hw_intf\n");
		return;
	}

	DPU_DEBUG_VIDENC(vid_enc, "\n");
	hw_res->intfs[vid_enc->hw_intf->idx - INTF_0] = INTF_MODE_VIDEO;
}

static int _dpu_encoder_phys_vid_wait_for_vblank(
		struct dpu_encoder_phys *phys_enc, bool notify)
=======
		struct dpu_encoder_hw_resources *hw_res)
{
	hw_res->intfs[phys_enc->intf_idx - INTF_0] = INTF_MODE_VIDEO;
}

static int dpu_encoder_phys_vid_wait_for_vblank(
		struct dpu_encoder_phys *phys_enc)
>>>>>>> upstream/android-13
{
	struct dpu_encoder_wait_info wait_info;
	int ret;

<<<<<<< HEAD
	if (!phys_enc) {
		pr_err("invalid encoder\n");
		return -EINVAL;
	}

=======
>>>>>>> upstream/android-13
	wait_info.wq = &phys_enc->pending_kickoff_wq;
	wait_info.atomic_cnt = &phys_enc->pending_kickoff_cnt;
	wait_info.timeout_ms = KICKOFF_TIMEOUT_MS;

	if (!dpu_encoder_phys_vid_is_master(phys_enc)) {
<<<<<<< HEAD
		if (notify && phys_enc->parent_ops->handle_frame_done)
			phys_enc->parent_ops->handle_frame_done(
					phys_enc->parent, phys_enc,
					DPU_ENCODER_FRAME_EVENT_DONE);
=======
>>>>>>> upstream/android-13
		return 0;
	}

	/* Wait for kickoff to complete */
	ret = dpu_encoder_helper_wait_for_irq(phys_enc, INTR_IDX_VSYNC,
			&wait_info);

	if (ret == -ETIMEDOUT) {
		dpu_encoder_helper_report_irq_timeout(phys_enc, INTR_IDX_VSYNC);
<<<<<<< HEAD
	} else if (!ret && notify && phys_enc->parent_ops->handle_frame_done)
		phys_enc->parent_ops->handle_frame_done(
				phys_enc->parent, phys_enc,
				DPU_ENCODER_FRAME_EVENT_DONE);
=======
	}
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int dpu_encoder_phys_vid_wait_for_vblank(
		struct dpu_encoder_phys *phys_enc)
{
	return _dpu_encoder_phys_vid_wait_for_vblank(phys_enc, true);
}

static void dpu_encoder_phys_vid_prepare_for_kickoff(
		struct dpu_encoder_phys *phys_enc,
		struct dpu_encoder_kickoff_params *params)
{
	struct dpu_encoder_phys_vid *vid_enc;
	struct dpu_hw_ctl *ctl;
	int rc;

	if (!phys_enc || !params) {
		DPU_ERROR("invalid encoder/parameters\n");
		return;
	}
	vid_enc = to_dpu_encoder_phys_vid(phys_enc);

	ctl = phys_enc->hw_ctl;
	if (!ctl || !ctl->ops.wait_reset_status)
=======
static int dpu_encoder_phys_vid_wait_for_commit_done(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_hw_ctl *hw_ctl = phys_enc->hw_ctl;
	int ret;

	if (!hw_ctl)
		return 0;

	ret = wait_event_timeout(phys_enc->pending_kickoff_wq,
		(hw_ctl->ops.get_flush_register(hw_ctl) == 0),
		msecs_to_jiffies(50));
	if (ret <= 0) {
		DPU_ERROR("vblank timeout\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static void dpu_encoder_phys_vid_prepare_for_kickoff(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_hw_ctl *ctl;
	int rc;
	struct drm_encoder *drm_enc;

	drm_enc = phys_enc->parent;

	ctl = phys_enc->hw_ctl;
	if (!ctl->ops.wait_reset_status)
>>>>>>> upstream/android-13
		return;

	/*
	 * hw supports hardware initiated ctl reset, so before we kickoff a new
	 * frame, need to check and wait for hw initiated ctl reset completion
	 */
	rc = ctl->ops.wait_reset_status(ctl);
	if (rc) {
<<<<<<< HEAD
		DPU_ERROR_VIDENC(vid_enc, "ctl %d reset failure: %d\n",
				ctl->idx, rc);
		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_VSYNC);
		dpu_dbg_dump(false, __func__, true, true);
=======
		DPU_ERROR_VIDENC(phys_enc, "ctl %d reset failure: %d\n",
				ctl->idx, rc);
		msm_disp_snapshot_state(drm_enc->dev);
		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_VSYNC);
>>>>>>> upstream/android-13
	}
}

static void dpu_encoder_phys_vid_disable(struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	struct msm_drm_private *priv;
	struct dpu_encoder_phys_vid *vid_enc;
	unsigned long lock_flags;
	int ret;

	if (!phys_enc || !phys_enc->parent || !phys_enc->parent->dev ||
			!phys_enc->parent->dev->dev_private) {
		DPU_ERROR("invalid encoder/device\n");
		return;
	}
	priv = phys_enc->parent->dev->dev_private;

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	if (!vid_enc->hw_intf || !phys_enc->hw_ctl) {
		DPU_ERROR("invalid hw_intf %d hw_ctl %d\n",
				vid_enc->hw_intf != 0, phys_enc->hw_ctl != 0);
		return;
	}

	DPU_DEBUG_VIDENC(vid_enc, "\n");

	if (WARN_ON(!vid_enc->hw_intf->ops.enable_timing))
=======
	unsigned long lock_flags;
	int ret;

	if (!phys_enc->parent || !phys_enc->parent->dev) {
		DPU_ERROR("invalid encoder/device\n");
		return;
	}

	if (!phys_enc->hw_intf) {
		DPU_ERROR("invalid hw_intf %d hw_ctl %d\n",
				phys_enc->hw_intf != NULL, phys_enc->hw_ctl != NULL);
		return;
	}

	if (WARN_ON(!phys_enc->hw_intf->ops.enable_timing))
>>>>>>> upstream/android-13
		return;

	if (phys_enc->enable_state == DPU_ENC_DISABLED) {
		DPU_ERROR("already disabled\n");
		return;
	}

	spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
<<<<<<< HEAD
	vid_enc->hw_intf->ops.enable_timing(vid_enc->hw_intf, 0);
=======
	phys_enc->hw_intf->ops.enable_timing(phys_enc->hw_intf, 0);
>>>>>>> upstream/android-13
	if (dpu_encoder_phys_vid_is_master(phys_enc))
		dpu_encoder_phys_inc_pending(phys_enc);
	spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);

	/*
	 * Wait for a vsync so we know the ENABLE=0 latched before
	 * the (connector) source of the vsync's gets disabled,
	 * otherwise we end up in a funny state if we re-enable
	 * before the disable latches, which results that some of
	 * the settings changes for the new modeset (like new
	 * scanout buffer) don't latch properly..
	 */
	if (dpu_encoder_phys_vid_is_master(phys_enc)) {
<<<<<<< HEAD
		ret = _dpu_encoder_phys_vid_wait_for_vblank(phys_enc, false);
=======
		ret = dpu_encoder_phys_vid_wait_for_vblank(phys_enc);
>>>>>>> upstream/android-13
		if (ret) {
			atomic_set(&phys_enc->pending_kickoff_cnt, 0);
			DRM_ERROR("wait disable failed: id:%u intf:%d ret:%d\n",
				  DRMID(phys_enc->parent),
<<<<<<< HEAD
				  vid_enc->hw_intf->idx - INTF_0, ret);
=======
				  phys_enc->hw_intf->idx - INTF_0, ret);
>>>>>>> upstream/android-13
		}
	}

	phys_enc->enable_state = DPU_ENC_DISABLED;
}

static void dpu_encoder_phys_vid_handle_post_kickoff(
		struct dpu_encoder_phys *phys_enc)
{
	unsigned long lock_flags;
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc) {
		DPU_ERROR("invalid encoder\n");
		return;
	}

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	DPU_DEBUG_VIDENC(vid_enc, "enable_state %d\n", phys_enc->enable_state);
=======
>>>>>>> upstream/android-13

	/*
	 * Video mode must flush CTL before enabling timing engine
	 * Video encoders need to turn on their interfaces now
	 */
	if (phys_enc->enable_state == DPU_ENC_ENABLING) {
		trace_dpu_enc_phys_vid_post_kickoff(DRMID(phys_enc->parent),
<<<<<<< HEAD
				    vid_enc->hw_intf->idx - INTF_0);
		spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
		vid_enc->hw_intf->ops.enable_timing(vid_enc->hw_intf, 1);
=======
				    phys_enc->hw_intf->idx - INTF_0);
		spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
		phys_enc->hw_intf->ops.enable_timing(phys_enc->hw_intf, 1);
>>>>>>> upstream/android-13
		spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);
		phys_enc->enable_state = DPU_ENC_ENABLED;
	}
}

static void dpu_encoder_phys_vid_irq_control(struct dpu_encoder_phys *phys_enc,
		bool enable)
{
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc;
	int ret;

	if (!phys_enc)
		return;

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);

	trace_dpu_enc_phys_vid_irq_ctrl(DRMID(phys_enc->parent),
			    vid_enc->hw_intf->idx - INTF_0,
=======
	int ret;

	trace_dpu_enc_phys_vid_irq_ctrl(DRMID(phys_enc->parent),
			    phys_enc->hw_intf->idx - INTF_0,
>>>>>>> upstream/android-13
			    enable,
			    atomic_read(&phys_enc->vblank_refcount));

	if (enable) {
		ret = dpu_encoder_phys_vid_control_vblank_irq(phys_enc, true);
<<<<<<< HEAD
		if (ret)
=======
		if (WARN_ON(ret))
>>>>>>> upstream/android-13
			return;

		dpu_encoder_helper_register_irq(phys_enc, INTR_IDX_UNDERRUN);
	} else {
		dpu_encoder_phys_vid_control_vblank_irq(phys_enc, false);
		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_UNDERRUN);
	}
}

<<<<<<< HEAD
static void dpu_encoder_phys_vid_setup_misr(struct dpu_encoder_phys *phys_enc,
						bool enable, u32 frame_count)
{
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc)
		return;
	vid_enc = to_dpu_encoder_phys_vid(phys_enc);

	if (vid_enc->hw_intf && vid_enc->hw_intf->ops.setup_misr)
		vid_enc->hw_intf->ops.setup_misr(vid_enc->hw_intf,
							enable, frame_count);
}

static u32 dpu_encoder_phys_vid_collect_misr(struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc)
		return 0;
	vid_enc = to_dpu_encoder_phys_vid(phys_enc);

	return vid_enc->hw_intf && vid_enc->hw_intf->ops.collect_misr ?
		vid_enc->hw_intf->ops.collect_misr(vid_enc->hw_intf) : 0;
}

static int dpu_encoder_phys_vid_get_line_count(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_vid *vid_enc;

	if (!phys_enc)
		return -EINVAL;

	if (!dpu_encoder_phys_vid_is_master(phys_enc))
		return -EINVAL;

	vid_enc = to_dpu_encoder_phys_vid(phys_enc);
	if (!vid_enc->hw_intf || !vid_enc->hw_intf->ops.get_line_count)
		return -EINVAL;

	return vid_enc->hw_intf->ops.get_line_count(vid_enc->hw_intf);
=======
static int dpu_encoder_phys_vid_get_line_count(
		struct dpu_encoder_phys *phys_enc)
{
	if (!dpu_encoder_phys_vid_is_master(phys_enc))
		return -EINVAL;

	if (!phys_enc->hw_intf || !phys_enc->hw_intf->ops.get_line_count)
		return -EINVAL;

	return phys_enc->hw_intf->ops.get_line_count(phys_enc->hw_intf);
}

static int dpu_encoder_phys_vid_get_frame_count(
		struct dpu_encoder_phys *phys_enc)
{
	struct intf_status s = {0};
	u32 fetch_start = 0;
	struct drm_display_mode mode = phys_enc->cached_mode;

	if (!dpu_encoder_phys_vid_is_master(phys_enc))
		return -EINVAL;

	if (!phys_enc->hw_intf || !phys_enc->hw_intf->ops.get_status)
		return -EINVAL;

	phys_enc->hw_intf->ops.get_status(phys_enc->hw_intf, &s);

	if (s.is_prog_fetch_en && s.is_en) {
		fetch_start = mode.vtotal - (mode.vsync_start - mode.vdisplay);
		if ((s.line_count > fetch_start) &&
			(s.line_count <= mode.vtotal))
			return s.frame_count + 1;
	}

	return s.frame_count;
>>>>>>> upstream/android-13
}

static void dpu_encoder_phys_vid_init_ops(struct dpu_encoder_phys_ops *ops)
{
	ops->is_master = dpu_encoder_phys_vid_is_master;
	ops->mode_set = dpu_encoder_phys_vid_mode_set;
	ops->mode_fixup = dpu_encoder_phys_vid_mode_fixup;
	ops->enable = dpu_encoder_phys_vid_enable;
	ops->disable = dpu_encoder_phys_vid_disable;
	ops->destroy = dpu_encoder_phys_vid_destroy;
	ops->get_hw_resources = dpu_encoder_phys_vid_get_hw_resources;
	ops->control_vblank_irq = dpu_encoder_phys_vid_control_vblank_irq;
<<<<<<< HEAD
	ops->wait_for_commit_done = dpu_encoder_phys_vid_wait_for_vblank;
=======
	ops->wait_for_commit_done = dpu_encoder_phys_vid_wait_for_commit_done;
>>>>>>> upstream/android-13
	ops->wait_for_vblank = dpu_encoder_phys_vid_wait_for_vblank;
	ops->wait_for_tx_complete = dpu_encoder_phys_vid_wait_for_vblank;
	ops->irq_control = dpu_encoder_phys_vid_irq_control;
	ops->prepare_for_kickoff = dpu_encoder_phys_vid_prepare_for_kickoff;
	ops->handle_post_kickoff = dpu_encoder_phys_vid_handle_post_kickoff;
	ops->needs_single_flush = dpu_encoder_phys_vid_needs_single_flush;
<<<<<<< HEAD
	ops->setup_misr = dpu_encoder_phys_vid_setup_misr;
	ops->collect_misr = dpu_encoder_phys_vid_collect_misr;
	ops->hw_reset = dpu_encoder_helper_hw_reset;
	ops->get_line_count = dpu_encoder_phys_vid_get_line_count;
=======
	ops->get_line_count = dpu_encoder_phys_vid_get_line_count;
	ops->get_frame_count = dpu_encoder_phys_vid_get_frame_count;
>>>>>>> upstream/android-13
}

struct dpu_encoder_phys *dpu_encoder_phys_vid_init(
		struct dpu_enc_phys_init_params *p)
{
	struct dpu_encoder_phys *phys_enc = NULL;
<<<<<<< HEAD
	struct dpu_encoder_phys_vid *vid_enc = NULL;
	struct dpu_rm_hw_iter iter;
	struct dpu_hw_mdp *hw_mdp;
=======
>>>>>>> upstream/android-13
	struct dpu_encoder_irq *irq;
	int i, ret = 0;

	if (!p) {
		ret = -EINVAL;
		goto fail;
	}

<<<<<<< HEAD
	vid_enc = kzalloc(sizeof(*vid_enc), GFP_KERNEL);
	if (!vid_enc) {
=======
	phys_enc = kzalloc(sizeof(*phys_enc), GFP_KERNEL);
	if (!phys_enc) {
>>>>>>> upstream/android-13
		ret = -ENOMEM;
		goto fail;
	}

<<<<<<< HEAD
	phys_enc = &vid_enc->base;

	hw_mdp = dpu_rm_get_mdp(&p->dpu_kms->rm);
	if (IS_ERR_OR_NULL(hw_mdp)) {
		ret = PTR_ERR(hw_mdp);
		DPU_ERROR("failed to get mdptop\n");
		goto fail;
	}
	phys_enc->hw_mdptop = hw_mdp;
	phys_enc->intf_idx = p->intf_idx;

	/**
	 * hw_intf resource permanently assigned to this encoder
	 * Other resources allocated at atomic commit time by use case
	 */
	dpu_rm_init_hw_iter(&iter, 0, DPU_HW_BLK_INTF);
	while (dpu_rm_get_hw(&p->dpu_kms->rm, &iter)) {
		struct dpu_hw_intf *hw_intf = (struct dpu_hw_intf *)iter.hw;

		if (hw_intf->idx == p->intf_idx) {
			vid_enc->hw_intf = hw_intf;
			break;
		}
	}

	if (!vid_enc->hw_intf) {
		ret = -EINVAL;
		DPU_ERROR("failed to get hw_intf\n");
		goto fail;
	}

	DPU_DEBUG_VIDENC(vid_enc, "\n");
=======
	phys_enc->hw_mdptop = p->dpu_kms->hw_mdp;
	phys_enc->intf_idx = p->intf_idx;

	DPU_DEBUG_VIDENC(phys_enc, "\n");
>>>>>>> upstream/android-13

	dpu_encoder_phys_vid_init_ops(&phys_enc->ops);
	phys_enc->parent = p->parent;
	phys_enc->parent_ops = p->parent_ops;
	phys_enc->dpu_kms = p->dpu_kms;
	phys_enc->split_role = p->split_role;
	phys_enc->intf_mode = INTF_MODE_VIDEO;
	phys_enc->enc_spinlock = p->enc_spinlock;
	for (i = 0; i < INTR_IDX_MAX; i++) {
		irq = &phys_enc->irq[i];
		INIT_LIST_HEAD(&irq->cb.list);
		irq->irq_idx = -EINVAL;
<<<<<<< HEAD
		irq->hw_idx = -EINVAL;
=======
>>>>>>> upstream/android-13
		irq->cb.arg = phys_enc;
	}

	irq = &phys_enc->irq[INTR_IDX_VSYNC];
	irq->name = "vsync_irq";
<<<<<<< HEAD
	irq->intr_type = DPU_IRQ_TYPE_INTF_VSYNC;
=======
>>>>>>> upstream/android-13
	irq->intr_idx = INTR_IDX_VSYNC;
	irq->cb.func = dpu_encoder_phys_vid_vblank_irq;

	irq = &phys_enc->irq[INTR_IDX_UNDERRUN];
	irq->name = "underrun";
<<<<<<< HEAD
	irq->intr_type = DPU_IRQ_TYPE_INTF_UNDER_RUN;
=======
>>>>>>> upstream/android-13
	irq->intr_idx = INTR_IDX_UNDERRUN;
	irq->cb.func = dpu_encoder_phys_vid_underrun_irq;

	atomic_set(&phys_enc->vblank_refcount, 0);
	atomic_set(&phys_enc->pending_kickoff_cnt, 0);
	init_waitqueue_head(&phys_enc->pending_kickoff_wq);
	phys_enc->enable_state = DPU_ENC_DISABLED;

<<<<<<< HEAD
	DPU_DEBUG_VIDENC(vid_enc, "created intf idx:%d\n", p->intf_idx);
=======
	DPU_DEBUG_VIDENC(phys_enc, "created intf idx:%d\n", p->intf_idx);
>>>>>>> upstream/android-13

	return phys_enc;

fail:
	DPU_ERROR("failed to create encoder\n");
<<<<<<< HEAD
	if (vid_enc)
=======
	if (phys_enc)
>>>>>>> upstream/android-13
		dpu_encoder_phys_vid_destroy(phys_enc);

	return ERR_PTR(ret);
}
