<<<<<<< HEAD
/*
 * Copyright (c) 2015-2018 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__
#include "dpu_encoder_phys.h"
#include "dpu_hw_interrupts.h"
#include "dpu_core_irq.h"
#include "dpu_formats.h"
#include "dpu_trace.h"
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015-2018, 2020-2021 The Linux Foundation. All rights reserved.
 */

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__
#include <linux/delay.h>
#include "dpu_encoder_phys.h"
#include "dpu_hw_interrupts.h"
#include "dpu_hw_pingpong.h"
#include "dpu_core_irq.h"
#include "dpu_formats.h"
#include "dpu_trace.h"
#include "disp/msm_disp_snapshot.h"
>>>>>>> upstream/android-13

#define DPU_DEBUG_CMDENC(e, fmt, ...) DPU_DEBUG("enc%d intf%d " fmt, \
		(e) && (e)->base.parent ? \
		(e)->base.parent->base.id : -1, \
		(e) ? (e)->base.intf_idx - INTF_0 : -1, ##__VA_ARGS__)

#define DPU_ERROR_CMDENC(e, fmt, ...) DPU_ERROR("enc%d intf%d " fmt, \
		(e) && (e)->base.parent ? \
		(e)->base.parent->base.id : -1, \
		(e) ? (e)->base.intf_idx - INTF_0 : -1, ##__VA_ARGS__)

#define to_dpu_encoder_phys_cmd(x) \
	container_of(x, struct dpu_encoder_phys_cmd, base)

#define PP_TIMEOUT_MAX_TRIALS	10

/*
 * Tearcheck sync start and continue thresholds are empirically found
 * based on common panels In the future, may want to allow panels to override
 * these default values
 */
#define DEFAULT_TEARCHECK_SYNC_THRESH_START	4
#define DEFAULT_TEARCHECK_SYNC_THRESH_CONTINUE	4

#define DPU_ENC_WR_PTR_START_TIMEOUT_US 20000

<<<<<<< HEAD
static inline int _dpu_encoder_phys_cmd_get_idle_timeout(
		struct dpu_encoder_phys_cmd *cmd_enc)
{
	return KICKOFF_TIMEOUT_MS;
}

static inline bool dpu_encoder_phys_cmd_is_master(
		struct dpu_encoder_phys *phys_enc)
=======
#define DPU_ENC_MAX_POLL_TIMEOUT_US	2000

static bool dpu_encoder_phys_cmd_is_master(struct dpu_encoder_phys *phys_enc)
>>>>>>> upstream/android-13
{
	return (phys_enc->split_role != ENC_ROLE_SLAVE) ? true : false;
}

static bool dpu_encoder_phys_cmd_mode_fixup(
		struct dpu_encoder_phys *phys_enc,
		const struct drm_display_mode *mode,
		struct drm_display_mode *adj_mode)
{
<<<<<<< HEAD
	if (phys_enc)
		DPU_DEBUG_CMDENC(to_dpu_encoder_phys_cmd(phys_enc), "\n");
=======
	DPU_DEBUG_CMDENC(to_dpu_encoder_phys_cmd(phys_enc), "\n");
>>>>>>> upstream/android-13
	return true;
}

static void _dpu_encoder_phys_cmd_update_intf_cfg(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
			to_dpu_encoder_phys_cmd(phys_enc);
	struct dpu_hw_ctl *ctl;
	struct dpu_hw_intf_cfg intf_cfg = { 0 };

<<<<<<< HEAD
	if (!phys_enc)
		return;

	ctl = phys_enc->hw_ctl;
	if (!ctl || !ctl->ops.setup_intf_cfg)
=======
	ctl = phys_enc->hw_ctl;
	if (!ctl->ops.setup_intf_cfg)
>>>>>>> upstream/android-13
		return;

	intf_cfg.intf = phys_enc->intf_idx;
	intf_cfg.intf_mode_sel = DPU_CTL_MODE_SEL_CMD;
	intf_cfg.stream_sel = cmd_enc->stream_sel;
	intf_cfg.mode_3d = dpu_encoder_helper_get_3d_blend_mode(phys_enc);
	ctl->ops.setup_intf_cfg(ctl, &intf_cfg);
}

static void dpu_encoder_phys_cmd_pp_tx_done_irq(void *arg, int irq_idx)
{
	struct dpu_encoder_phys *phys_enc = arg;
	unsigned long lock_flags;
	int new_cnt;
	u32 event = DPU_ENCODER_FRAME_EVENT_DONE;

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp)
=======
	if (!phys_enc->hw_pp)
>>>>>>> upstream/android-13
		return;

	DPU_ATRACE_BEGIN("pp_done_irq");
	/* notify all synchronous clients first, then asynchronous clients */
	if (phys_enc->parent_ops->handle_frame_done)
		phys_enc->parent_ops->handle_frame_done(phys_enc->parent,
				phys_enc, event);

	spin_lock_irqsave(phys_enc->enc_spinlock, lock_flags);
	new_cnt = atomic_add_unless(&phys_enc->pending_kickoff_cnt, -1, 0);
	spin_unlock_irqrestore(phys_enc->enc_spinlock, lock_flags);

	trace_dpu_enc_phys_cmd_pp_tx_done(DRMID(phys_enc->parent),
					  phys_enc->hw_pp->idx - PINGPONG_0,
					  new_cnt, event);

	/* Signal any waiting atomic commit thread */
	wake_up_all(&phys_enc->pending_kickoff_wq);
	DPU_ATRACE_END("pp_done_irq");
}

static void dpu_encoder_phys_cmd_pp_rd_ptr_irq(void *arg, int irq_idx)
{
	struct dpu_encoder_phys *phys_enc = arg;
	struct dpu_encoder_phys_cmd *cmd_enc;

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp)
=======
	if (!phys_enc->hw_pp)
>>>>>>> upstream/android-13
		return;

	DPU_ATRACE_BEGIN("rd_ptr_irq");
	cmd_enc = to_dpu_encoder_phys_cmd(phys_enc);

	if (phys_enc->parent_ops->handle_vblank_virt)
		phys_enc->parent_ops->handle_vblank_virt(phys_enc->parent,
			phys_enc);

	atomic_add_unless(&cmd_enc->pending_vblank_cnt, -1, 0);
	wake_up_all(&cmd_enc->pending_vblank_wq);
	DPU_ATRACE_END("rd_ptr_irq");
}

static void dpu_encoder_phys_cmd_ctl_start_irq(void *arg, int irq_idx)
{
	struct dpu_encoder_phys *phys_enc = arg;
<<<<<<< HEAD
	struct dpu_encoder_phys_cmd *cmd_enc;

	if (!phys_enc || !phys_enc->hw_ctl)
		return;

	DPU_ATRACE_BEGIN("ctl_start_irq");
	cmd_enc = to_dpu_encoder_phys_cmd(phys_enc);
=======

	DPU_ATRACE_BEGIN("ctl_start_irq");
>>>>>>> upstream/android-13

	atomic_add_unless(&phys_enc->pending_ctlstart_cnt, -1, 0);

	/* Signal any waiting ctl start interrupt */
	wake_up_all(&phys_enc->pending_kickoff_wq);
	DPU_ATRACE_END("ctl_start_irq");
}

static void dpu_encoder_phys_cmd_underrun_irq(void *arg, int irq_idx)
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
static void _dpu_encoder_phys_cmd_setup_irq_hw_idx(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_irq *irq;

	irq = &phys_enc->irq[INTR_IDX_CTL_START];
	irq->hw_idx = phys_enc->hw_ctl->idx;
	irq->irq_idx = -EINVAL;

	irq = &phys_enc->irq[INTR_IDX_PINGPONG];
	irq->hw_idx = phys_enc->hw_pp->idx;
	irq->irq_idx = -EINVAL;

	irq = &phys_enc->irq[INTR_IDX_RDPTR];
	irq->hw_idx = phys_enc->hw_pp->idx;
	irq->irq_idx = -EINVAL;

	irq = &phys_enc->irq[INTR_IDX_UNDERRUN];
	irq->hw_idx = phys_enc->intf_idx;
	irq->irq_idx = -EINVAL;
}

=======
>>>>>>> upstream/android-13
static void dpu_encoder_phys_cmd_mode_set(
		struct dpu_encoder_phys *phys_enc,
		struct drm_display_mode *mode,
		struct drm_display_mode *adj_mode)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);
<<<<<<< HEAD
	struct dpu_rm *rm = &phys_enc->dpu_kms->rm;
	struct dpu_rm_hw_iter iter;
	int i, instance;

	if (!phys_enc || !mode || !adj_mode) {
=======
	struct dpu_encoder_irq *irq;

	if (!mode || !adj_mode) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid args\n");
		return;
	}
	phys_enc->cached_mode = *adj_mode;
	DPU_DEBUG_CMDENC(cmd_enc, "caching mode:\n");
	drm_mode_debug_printmodeline(adj_mode);

<<<<<<< HEAD
	instance = phys_enc->split_role == ENC_ROLE_SLAVE ? 1 : 0;

	/* Retrieve previously allocated HW Resources. Shouldn't fail */
	dpu_rm_init_hw_iter(&iter, phys_enc->parent->base.id, DPU_HW_BLK_CTL);
	for (i = 0; i <= instance; i++) {
		if (dpu_rm_get_hw(rm, &iter))
			phys_enc->hw_ctl = (struct dpu_hw_ctl *)iter.hw;
	}

	if (IS_ERR_OR_NULL(phys_enc->hw_ctl)) {
		DPU_ERROR_CMDENC(cmd_enc, "failed to init ctl: %ld\n",
				PTR_ERR(phys_enc->hw_ctl));
		phys_enc->hw_ctl = NULL;
		return;
	}

	_dpu_encoder_phys_cmd_setup_irq_hw_idx(phys_enc);
=======
	irq = &phys_enc->irq[INTR_IDX_CTL_START];
	irq->irq_idx = phys_enc->hw_ctl->caps->intr_start;

	irq = &phys_enc->irq[INTR_IDX_PINGPONG];
	irq->irq_idx = phys_enc->hw_pp->caps->intr_done;

	irq = &phys_enc->irq[INTR_IDX_RDPTR];
	irq->irq_idx = phys_enc->hw_pp->caps->intr_rdptr;

	irq = &phys_enc->irq[INTR_IDX_UNDERRUN];
	irq->irq_idx = phys_enc->hw_intf->cap->intr_underrun;
>>>>>>> upstream/android-13
}

static int _dpu_encoder_phys_cmd_handle_ppdone_timeout(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
			to_dpu_encoder_phys_cmd(phys_enc);
	u32 frame_event = DPU_ENCODER_FRAME_EVENT_ERROR;
	bool do_log = false;
<<<<<<< HEAD

	if (!phys_enc || !phys_enc->hw_pp || !phys_enc->hw_ctl)
		return -EINVAL;

=======
	struct drm_encoder *drm_enc;

	if (!phys_enc->hw_pp)
		return -EINVAL;

	drm_enc = phys_enc->parent;

>>>>>>> upstream/android-13
	cmd_enc->pp_timeout_report_cnt++;
	if (cmd_enc->pp_timeout_report_cnt == PP_TIMEOUT_MAX_TRIALS) {
		frame_event |= DPU_ENCODER_FRAME_EVENT_PANEL_DEAD;
		do_log = true;
	} else if (cmd_enc->pp_timeout_report_cnt == 1) {
		do_log = true;
	}

<<<<<<< HEAD
	trace_dpu_enc_phys_cmd_pdone_timeout(DRMID(phys_enc->parent),
=======
	trace_dpu_enc_phys_cmd_pdone_timeout(DRMID(drm_enc),
>>>>>>> upstream/android-13
		     phys_enc->hw_pp->idx - PINGPONG_0,
		     cmd_enc->pp_timeout_report_cnt,
		     atomic_read(&phys_enc->pending_kickoff_cnt),
		     frame_event);

	/* to avoid flooding, only log first time, and "dead" time */
	if (do_log) {
		DRM_ERROR("id:%d pp:%d kickoff timeout %d cnt %d koff_cnt %d\n",
<<<<<<< HEAD
			  DRMID(phys_enc->parent),
=======
			  DRMID(drm_enc),
>>>>>>> upstream/android-13
			  phys_enc->hw_pp->idx - PINGPONG_0,
			  phys_enc->hw_ctl->idx - CTL_0,
			  cmd_enc->pp_timeout_report_cnt,
			  atomic_read(&phys_enc->pending_kickoff_cnt));
<<<<<<< HEAD

		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_RDPTR);
		dpu_dbg_dump(false, __func__, true, true);
=======
		msm_disp_snapshot_state(drm_enc->dev);
		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_RDPTR);
>>>>>>> upstream/android-13
	}

	atomic_add_unless(&phys_enc->pending_kickoff_cnt, -1, 0);

	/* request a ctl reset before the next kickoff */
	phys_enc->enable_state = DPU_ENC_ERR_NEEDS_HW_RESET;

	if (phys_enc->parent_ops->handle_frame_done)
		phys_enc->parent_ops->handle_frame_done(
<<<<<<< HEAD
				phys_enc->parent, phys_enc, frame_event);
=======
				drm_enc, phys_enc, frame_event);
>>>>>>> upstream/android-13

	return -ETIMEDOUT;
}

static int _dpu_encoder_phys_cmd_wait_for_idle(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
			to_dpu_encoder_phys_cmd(phys_enc);
	struct dpu_encoder_wait_info wait_info;
	int ret;

<<<<<<< HEAD
	if (!phys_enc) {
		DPU_ERROR("invalid encoder\n");
		return -EINVAL;
	}

=======
>>>>>>> upstream/android-13
	wait_info.wq = &phys_enc->pending_kickoff_wq;
	wait_info.atomic_cnt = &phys_enc->pending_kickoff_cnt;
	wait_info.timeout_ms = KICKOFF_TIMEOUT_MS;

	ret = dpu_encoder_helper_wait_for_irq(phys_enc, INTR_IDX_PINGPONG,
			&wait_info);
	if (ret == -ETIMEDOUT)
		_dpu_encoder_phys_cmd_handle_ppdone_timeout(phys_enc);
	else if (!ret)
		cmd_enc->pp_timeout_report_cnt = 0;

	return ret;
}

static int dpu_encoder_phys_cmd_control_vblank_irq(
		struct dpu_encoder_phys *phys_enc,
		bool enable)
{
	int ret = 0;
	int refcount;

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp) {
=======
	if (!phys_enc->hw_pp) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid encoder\n");
		return -EINVAL;
	}

	refcount = atomic_read(&phys_enc->vblank_refcount);

	/* Slave encoders don't report vblank */
	if (!dpu_encoder_phys_cmd_is_master(phys_enc))
		goto end;

	/* protect against negative */
	if (!enable && refcount == 0) {
		ret = -EINVAL;
		goto end;
	}

	DRM_DEBUG_KMS("id:%u pp:%d enable=%s/%d\n", DRMID(phys_enc->parent),
		      phys_enc->hw_pp->idx - PINGPONG_0,
		      enable ? "true" : "false", refcount);

	if (enable && atomic_inc_return(&phys_enc->vblank_refcount) == 1)
		ret = dpu_encoder_helper_register_irq(phys_enc, INTR_IDX_RDPTR);
	else if (!enable && atomic_dec_return(&phys_enc->vblank_refcount) == 0)
		ret = dpu_encoder_helper_unregister_irq(phys_enc,
				INTR_IDX_RDPTR);

end:
	if (ret) {
		DRM_ERROR("vblank irq err id:%u pp:%d ret:%d, enable %s/%d\n",
			  DRMID(phys_enc->parent),
			  phys_enc->hw_pp->idx - PINGPONG_0, ret,
			  enable ? "true" : "false", refcount);
	}

	return ret;
}

static void dpu_encoder_phys_cmd_irq_control(struct dpu_encoder_phys *phys_enc,
		bool enable)
{
<<<<<<< HEAD
	struct dpu_encoder_phys_cmd *cmd_enc;

	if (!phys_enc)
		return;

	cmd_enc = to_dpu_encoder_phys_cmd(phys_enc);

=======
>>>>>>> upstream/android-13
	trace_dpu_enc_phys_cmd_irq_ctrl(DRMID(phys_enc->parent),
			phys_enc->hw_pp->idx - PINGPONG_0,
			enable, atomic_read(&phys_enc->vblank_refcount));

	if (enable) {
		dpu_encoder_helper_register_irq(phys_enc, INTR_IDX_PINGPONG);
		dpu_encoder_helper_register_irq(phys_enc, INTR_IDX_UNDERRUN);
		dpu_encoder_phys_cmd_control_vblank_irq(phys_enc, true);

		if (dpu_encoder_phys_cmd_is_master(phys_enc))
			dpu_encoder_helper_register_irq(phys_enc,
					INTR_IDX_CTL_START);
	} else {
		if (dpu_encoder_phys_cmd_is_master(phys_enc))
			dpu_encoder_helper_unregister_irq(phys_enc,
					INTR_IDX_CTL_START);

		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_UNDERRUN);
		dpu_encoder_phys_cmd_control_vblank_irq(phys_enc, false);
		dpu_encoder_helper_unregister_irq(phys_enc, INTR_IDX_PINGPONG);
	}
}

static void dpu_encoder_phys_cmd_tearcheck_config(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);
	struct dpu_hw_tear_check tc_cfg = { 0 };
	struct drm_display_mode *mode;
	bool tc_enable = true;
	u32 vsync_hz;
<<<<<<< HEAD
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms;

	if (!phys_enc || !phys_enc->hw_pp) {
=======
	struct dpu_kms *dpu_kms;

	if (!phys_enc->hw_pp) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid encoder\n");
		return;
	}
	mode = &phys_enc->cached_mode;

	DPU_DEBUG_CMDENC(cmd_enc, "pp %d\n", phys_enc->hw_pp->idx - PINGPONG_0);

	if (!phys_enc->hw_pp->ops.setup_tearcheck ||
		!phys_enc->hw_pp->ops.enable_tearcheck) {
		DPU_DEBUG_CMDENC(cmd_enc, "tearcheck not supported\n");
		return;
	}

	dpu_kms = phys_enc->dpu_kms;
<<<<<<< HEAD
	if (!dpu_kms || !dpu_kms->dev || !dpu_kms->dev->dev_private) {
		DPU_ERROR("invalid device\n");
		return;
	}
	priv = dpu_kms->dev->dev_private;
=======
>>>>>>> upstream/android-13

	/*
	 * TE default: dsi byte clock calculated base on 70 fps;
	 * around 14 ms to complete a kickoff cycle if te disabled;
	 * vclk_line base on 60 fps; write is faster than read;
	 * init == start == rdptr;
	 *
	 * vsync_count is ratio of MDP VSYNC clock frequency to LCD panel
	 * frequency divided by the no. of rows (lines) in the LCDpanel.
	 */
	vsync_hz = dpu_kms_get_clk_rate(dpu_kms, "vsync");
	if (vsync_hz <= 0) {
		DPU_DEBUG_CMDENC(cmd_enc, "invalid - vsync_hz %u\n",
				 vsync_hz);
		return;
	}

<<<<<<< HEAD
	tc_cfg.vsync_count = vsync_hz / (mode->vtotal * mode->vrefresh);

	/* enable external TE after kickoff to avoid premature autorefresh */
	tc_cfg.hw_vsync_mode = 0;

	/*
	 * By setting sync_cfg_height to near max register value, we essentially
	 * disable dpu hw generated TE signal, since hw TE will arrive first.
	 * Only caveat is if due to error, we hit wrap-around.
	 */
	tc_cfg.sync_cfg_height = 0xFFF0;
=======
	tc_cfg.vsync_count = vsync_hz /
				(mode->vtotal * drm_mode_vrefresh(mode));

	/*
	 * Set the sync_cfg_height to twice vtotal so that if we lose a
	 * TE event coming from the display TE pin we won't stall immediately
	 */
	tc_cfg.hw_vsync_mode = 1;
	tc_cfg.sync_cfg_height = mode->vtotal * 2;
>>>>>>> upstream/android-13
	tc_cfg.vsync_init_val = mode->vdisplay;
	tc_cfg.sync_threshold_start = DEFAULT_TEARCHECK_SYNC_THRESH_START;
	tc_cfg.sync_threshold_continue = DEFAULT_TEARCHECK_SYNC_THRESH_CONTINUE;
	tc_cfg.start_pos = mode->vdisplay;
	tc_cfg.rd_ptr_irq = mode->vdisplay + 1;

	DPU_DEBUG_CMDENC(cmd_enc,
		"tc %d vsync_clk_speed_hz %u vtotal %u vrefresh %u\n",
		phys_enc->hw_pp->idx - PINGPONG_0, vsync_hz,
<<<<<<< HEAD
		mode->vtotal, mode->vrefresh);
=======
		mode->vtotal, drm_mode_vrefresh(mode));
>>>>>>> upstream/android-13
	DPU_DEBUG_CMDENC(cmd_enc,
		"tc %d enable %u start_pos %u rd_ptr_irq %u\n",
		phys_enc->hw_pp->idx - PINGPONG_0, tc_enable, tc_cfg.start_pos,
		tc_cfg.rd_ptr_irq);
	DPU_DEBUG_CMDENC(cmd_enc,
		"tc %d hw_vsync_mode %u vsync_count %u vsync_init_val %u\n",
		phys_enc->hw_pp->idx - PINGPONG_0, tc_cfg.hw_vsync_mode,
		tc_cfg.vsync_count, tc_cfg.vsync_init_val);
	DPU_DEBUG_CMDENC(cmd_enc,
		"tc %d cfgheight %u thresh_start %u thresh_cont %u\n",
		phys_enc->hw_pp->idx - PINGPONG_0, tc_cfg.sync_cfg_height,
		tc_cfg.sync_threshold_start, tc_cfg.sync_threshold_continue);

	phys_enc->hw_pp->ops.setup_tearcheck(phys_enc->hw_pp, &tc_cfg);
	phys_enc->hw_pp->ops.enable_tearcheck(phys_enc->hw_pp, tc_enable);
}

static void _dpu_encoder_phys_cmd_pingpong_config(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_ctl || !phys_enc->hw_pp
			|| !phys_enc->hw_ctl->ops.setup_intf_cfg) {
		DPU_ERROR("invalid arg(s), enc %d\n", phys_enc != 0);
=======
	if (!phys_enc->hw_pp || !phys_enc->hw_ctl->ops.setup_intf_cfg) {
		DPU_ERROR("invalid arg(s), enc %d\n", phys_enc != NULL);
>>>>>>> upstream/android-13
		return;
	}

	DPU_DEBUG_CMDENC(cmd_enc, "pp %d, enabling mode:\n",
			phys_enc->hw_pp->idx - PINGPONG_0);
	drm_mode_debug_printmodeline(&phys_enc->cached_mode);

	_dpu_encoder_phys_cmd_update_intf_cfg(phys_enc);
	dpu_encoder_phys_cmd_tearcheck_config(phys_enc);
}

static bool dpu_encoder_phys_cmd_needs_single_flush(
		struct dpu_encoder_phys *phys_enc)
{
	/**
	 * we do separate flush for each CTL and let
	 * CTL_START synchronize them
	 */
	return false;
}

static void dpu_encoder_phys_cmd_enable_helper(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_hw_ctl *ctl;
<<<<<<< HEAD
	u32 flush_mask = 0;

	if (!phys_enc || !phys_enc->hw_ctl || !phys_enc->hw_pp) {
		DPU_ERROR("invalid arg(s), encoder %d\n", phys_enc != 0);
=======

	if (!phys_enc->hw_pp) {
		DPU_ERROR("invalid arg(s), encoder %d\n", phys_enc != NULL);
>>>>>>> upstream/android-13
		return;
	}

	dpu_encoder_helper_split_config(phys_enc, phys_enc->intf_idx);

	_dpu_encoder_phys_cmd_pingpong_config(phys_enc);

	if (!dpu_encoder_phys_cmd_is_master(phys_enc))
<<<<<<< HEAD
		goto skip_flush;

	ctl = phys_enc->hw_ctl;
	ctl->ops.get_bitmask_intf(ctl, &flush_mask, phys_enc->intf_idx);
	ctl->ops.update_pending_flush(ctl, flush_mask);

skip_flush:
	return;
=======
		return;

	ctl = phys_enc->hw_ctl;
	ctl->ops.update_pending_flush_intf(ctl, phys_enc->intf_idx);
>>>>>>> upstream/android-13
}

static void dpu_encoder_phys_cmd_enable(struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp) {
=======
	if (!phys_enc->hw_pp) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid phys encoder\n");
		return;
	}

	DPU_DEBUG_CMDENC(cmd_enc, "pp %d\n", phys_enc->hw_pp->idx - PINGPONG_0);

	if (phys_enc->enable_state == DPU_ENC_ENABLED) {
		DPU_ERROR("already enabled\n");
		return;
	}

	dpu_encoder_phys_cmd_enable_helper(phys_enc);
	phys_enc->enable_state = DPU_ENC_ENABLED;
}

static void _dpu_encoder_phys_cmd_connect_te(
		struct dpu_encoder_phys *phys_enc, bool enable)
{
<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp ||
			!phys_enc->hw_pp->ops.connect_external_te)
=======
	if (!phys_enc->hw_pp || !phys_enc->hw_pp->ops.connect_external_te)
>>>>>>> upstream/android-13
		return;

	trace_dpu_enc_phys_cmd_connect_te(DRMID(phys_enc->parent), enable);
	phys_enc->hw_pp->ops.connect_external_te(phys_enc->hw_pp, enable);
}

static void dpu_encoder_phys_cmd_prepare_idle_pc(
		struct dpu_encoder_phys *phys_enc)
{
	_dpu_encoder_phys_cmd_connect_te(phys_enc, false);
}

static int dpu_encoder_phys_cmd_get_line_count(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_hw_pingpong *hw_pp;

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp)
=======
	if (!phys_enc->hw_pp)
>>>>>>> upstream/android-13
		return -EINVAL;

	if (!dpu_encoder_phys_cmd_is_master(phys_enc))
		return -EINVAL;

	hw_pp = phys_enc->hw_pp;
	if (!hw_pp->ops.get_line_count)
		return -EINVAL;

	return hw_pp->ops.get_line_count(hw_pp);
}

static void dpu_encoder_phys_cmd_disable(struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp) {
=======
	if (!phys_enc->hw_pp) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid encoder\n");
		return;
	}
	DRM_DEBUG_KMS("id:%u pp:%d state:%d\n", DRMID(phys_enc->parent),
		      phys_enc->hw_pp->idx - PINGPONG_0,
		      phys_enc->enable_state);

	if (phys_enc->enable_state == DPU_ENC_DISABLED) {
		DPU_ERROR_CMDENC(cmd_enc, "already disabled\n");
		return;
	}

	if (phys_enc->hw_pp->ops.enable_tearcheck)
		phys_enc->hw_pp->ops.enable_tearcheck(phys_enc->hw_pp, false);
	phys_enc->enable_state = DPU_ENC_DISABLED;
}

static void dpu_encoder_phys_cmd_destroy(struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);

<<<<<<< HEAD
	if (!phys_enc) {
		DPU_ERROR("invalid encoder\n");
		return;
	}
=======
>>>>>>> upstream/android-13
	kfree(cmd_enc);
}

static void dpu_encoder_phys_cmd_get_hw_resources(
		struct dpu_encoder_phys *phys_enc,
<<<<<<< HEAD
		struct dpu_encoder_hw_resources *hw_res,
		struct drm_connector_state *conn_state)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);

	if (!phys_enc) {
		DPU_ERROR("invalid encoder\n");
		return;
	}

	if ((phys_enc->intf_idx - INTF_0) >= INTF_MAX) {
		DPU_ERROR("invalid intf idx:%d\n", phys_enc->intf_idx);
		return;
	}

	DPU_DEBUG_CMDENC(cmd_enc, "\n");
=======
		struct dpu_encoder_hw_resources *hw_res)
{
>>>>>>> upstream/android-13
	hw_res->intfs[phys_enc->intf_idx - INTF_0] = INTF_MODE_CMD;
}

static void dpu_encoder_phys_cmd_prepare_for_kickoff(
<<<<<<< HEAD
		struct dpu_encoder_phys *phys_enc,
		struct dpu_encoder_kickoff_params *params)
=======
		struct dpu_encoder_phys *phys_enc)
>>>>>>> upstream/android-13
{
	struct dpu_encoder_phys_cmd *cmd_enc =
			to_dpu_encoder_phys_cmd(phys_enc);
	int ret;

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_pp) {
=======
	if (!phys_enc->hw_pp) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid encoder\n");
		return;
	}
	DRM_DEBUG_KMS("id:%u pp:%d pending_cnt:%d\n", DRMID(phys_enc->parent),
		      phys_enc->hw_pp->idx - PINGPONG_0,
		      atomic_read(&phys_enc->pending_kickoff_cnt));

	/*
	 * Mark kickoff request as outstanding. If there are more than one,
	 * outstanding, then we have to wait for the previous one to complete
	 */
	ret = _dpu_encoder_phys_cmd_wait_for_idle(phys_enc);
	if (ret) {
		/* force pending_kickoff_cnt 0 to discard failed kickoff */
		atomic_set(&phys_enc->pending_kickoff_cnt, 0);
		DRM_ERROR("failed wait_for_idle: id:%u ret:%d pp:%d\n",
			  DRMID(phys_enc->parent), ret,
			  phys_enc->hw_pp->idx - PINGPONG_0);
	}

	DPU_DEBUG_CMDENC(cmd_enc, "pp:%d pending_cnt %d\n",
			phys_enc->hw_pp->idx - PINGPONG_0,
			atomic_read(&phys_enc->pending_kickoff_cnt));
}

<<<<<<< HEAD
=======
static bool dpu_encoder_phys_cmd_is_ongoing_pptx(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_hw_pp_vsync_info info;

	if (!phys_enc)
		return false;

	phys_enc->hw_pp->ops.get_vsync_info(phys_enc->hw_pp, &info);
	if (info.wr_ptr_line_count > 0 &&
	    info.wr_ptr_line_count < phys_enc->cached_mode.vdisplay)
		return true;

	return false;
}

static void dpu_encoder_phys_cmd_prepare_commit(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
		to_dpu_encoder_phys_cmd(phys_enc);
	int trial = 0;

	if (!phys_enc)
		return;
	if (!phys_enc->hw_pp)
		return;
	if (!dpu_encoder_phys_cmd_is_master(phys_enc))
		return;

	/* If autorefresh is already disabled, we have nothing to do */
	if (!phys_enc->hw_pp->ops.get_autorefresh(phys_enc->hw_pp, NULL))
		return;

	/*
	 * If autorefresh is enabled, disable it and make sure it is safe to
	 * proceed with current frame commit/push. Sequence fallowed is,
	 * 1. Disable TE
	 * 2. Disable autorefresh config
	 * 4. Poll for frame transfer ongoing to be false
	 * 5. Enable TE back
	 */
	_dpu_encoder_phys_cmd_connect_te(phys_enc, false);
	phys_enc->hw_pp->ops.setup_autorefresh(phys_enc->hw_pp, 0, false);

	do {
		udelay(DPU_ENC_MAX_POLL_TIMEOUT_US);
		if ((trial * DPU_ENC_MAX_POLL_TIMEOUT_US)
				> (KICKOFF_TIMEOUT_MS * USEC_PER_MSEC)) {
			DPU_ERROR_CMDENC(cmd_enc,
					"disable autorefresh failed\n");
			break;
		}

		trial++;
	} while (dpu_encoder_phys_cmd_is_ongoing_pptx(phys_enc));

	_dpu_encoder_phys_cmd_connect_te(phys_enc, true);

	DPU_DEBUG_CMDENC(to_dpu_encoder_phys_cmd(phys_enc),
			 "disabled autorefresh\n");
}

>>>>>>> upstream/android-13
static int _dpu_encoder_phys_cmd_wait_for_ctl_start(
		struct dpu_encoder_phys *phys_enc)
{
	struct dpu_encoder_phys_cmd *cmd_enc =
			to_dpu_encoder_phys_cmd(phys_enc);
	struct dpu_encoder_wait_info wait_info;
	int ret;

<<<<<<< HEAD
	if (!phys_enc || !phys_enc->hw_ctl) {
		DPU_ERROR("invalid argument(s)\n");
		return -EINVAL;
	}

=======
>>>>>>> upstream/android-13
	wait_info.wq = &phys_enc->pending_kickoff_wq;
	wait_info.atomic_cnt = &phys_enc->pending_ctlstart_cnt;
	wait_info.timeout_ms = KICKOFF_TIMEOUT_MS;

	ret = dpu_encoder_helper_wait_for_irq(phys_enc, INTR_IDX_CTL_START,
			&wait_info);
	if (ret == -ETIMEDOUT) {
		DPU_ERROR_CMDENC(cmd_enc, "ctl start interrupt wait failed\n");
		ret = -EINVAL;
	} else if (!ret)
		ret = 0;

	return ret;
}

static int dpu_encoder_phys_cmd_wait_for_tx_complete(
		struct dpu_encoder_phys *phys_enc)
{
	int rc;
<<<<<<< HEAD
	struct dpu_encoder_phys_cmd *cmd_enc;

	if (!phys_enc)
		return -EINVAL;

	cmd_enc = to_dpu_encoder_phys_cmd(phys_enc);
=======
>>>>>>> upstream/android-13

	rc = _dpu_encoder_phys_cmd_wait_for_idle(phys_enc);
	if (rc) {
		DRM_ERROR("failed wait_for_idle: id:%u ret:%d intf:%d\n",
			  DRMID(phys_enc->parent), rc,
			  phys_enc->intf_idx - INTF_0);
	}

	return rc;
}

static int dpu_encoder_phys_cmd_wait_for_commit_done(
		struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	int rc = 0;
	struct dpu_encoder_phys_cmd *cmd_enc;

	if (!phys_enc)
		return -EINVAL;

	cmd_enc = to_dpu_encoder_phys_cmd(phys_enc);

	/* only required for master controller */
	if (dpu_encoder_phys_cmd_is_master(phys_enc))
		rc = _dpu_encoder_phys_cmd_wait_for_ctl_start(phys_enc);

	/* required for both controllers */
	if (!rc && cmd_enc->serialize_wait4pp)
		dpu_encoder_phys_cmd_prepare_for_kickoff(phys_enc, NULL);

	return rc;
=======
	/* only required for master controller */
	if (!dpu_encoder_phys_cmd_is_master(phys_enc))
		return 0;

	return _dpu_encoder_phys_cmd_wait_for_ctl_start(phys_enc);
>>>>>>> upstream/android-13
}

static int dpu_encoder_phys_cmd_wait_for_vblank(
		struct dpu_encoder_phys *phys_enc)
{
	int rc = 0;
	struct dpu_encoder_phys_cmd *cmd_enc;
	struct dpu_encoder_wait_info wait_info;

<<<<<<< HEAD
	if (!phys_enc)
		return -EINVAL;

=======
>>>>>>> upstream/android-13
	cmd_enc = to_dpu_encoder_phys_cmd(phys_enc);

	/* only required for master controller */
	if (!dpu_encoder_phys_cmd_is_master(phys_enc))
		return rc;

	wait_info.wq = &cmd_enc->pending_vblank_wq;
	wait_info.atomic_cnt = &cmd_enc->pending_vblank_cnt;
<<<<<<< HEAD
	wait_info.timeout_ms = _dpu_encoder_phys_cmd_get_idle_timeout(cmd_enc);
=======
	wait_info.timeout_ms = KICKOFF_TIMEOUT_MS;
>>>>>>> upstream/android-13

	atomic_inc(&cmd_enc->pending_vblank_cnt);

	rc = dpu_encoder_helper_wait_for_irq(phys_enc, INTR_IDX_RDPTR,
			&wait_info);

	return rc;
}

static void dpu_encoder_phys_cmd_handle_post_kickoff(
		struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	if (!phys_enc)
		return;

=======
>>>>>>> upstream/android-13
	/**
	 * re-enable external TE, either for the first time after enabling
	 * or if disabled for Autorefresh
	 */
	_dpu_encoder_phys_cmd_connect_te(phys_enc, true);
}

static void dpu_encoder_phys_cmd_trigger_start(
		struct dpu_encoder_phys *phys_enc)
{
<<<<<<< HEAD
	if (!phys_enc)
		return;

=======
>>>>>>> upstream/android-13
	dpu_encoder_helper_trigger_start(phys_enc);
}

static void dpu_encoder_phys_cmd_init_ops(
		struct dpu_encoder_phys_ops *ops)
{
<<<<<<< HEAD
=======
	ops->prepare_commit = dpu_encoder_phys_cmd_prepare_commit;
>>>>>>> upstream/android-13
	ops->is_master = dpu_encoder_phys_cmd_is_master;
	ops->mode_set = dpu_encoder_phys_cmd_mode_set;
	ops->mode_fixup = dpu_encoder_phys_cmd_mode_fixup;
	ops->enable = dpu_encoder_phys_cmd_enable;
	ops->disable = dpu_encoder_phys_cmd_disable;
	ops->destroy = dpu_encoder_phys_cmd_destroy;
	ops->get_hw_resources = dpu_encoder_phys_cmd_get_hw_resources;
	ops->control_vblank_irq = dpu_encoder_phys_cmd_control_vblank_irq;
	ops->wait_for_commit_done = dpu_encoder_phys_cmd_wait_for_commit_done;
	ops->prepare_for_kickoff = dpu_encoder_phys_cmd_prepare_for_kickoff;
	ops->wait_for_tx_complete = dpu_encoder_phys_cmd_wait_for_tx_complete;
	ops->wait_for_vblank = dpu_encoder_phys_cmd_wait_for_vblank;
	ops->trigger_start = dpu_encoder_phys_cmd_trigger_start;
	ops->needs_single_flush = dpu_encoder_phys_cmd_needs_single_flush;
<<<<<<< HEAD
	ops->hw_reset = dpu_encoder_helper_hw_reset;
=======
>>>>>>> upstream/android-13
	ops->irq_control = dpu_encoder_phys_cmd_irq_control;
	ops->restore = dpu_encoder_phys_cmd_enable_helper;
	ops->prepare_idle_pc = dpu_encoder_phys_cmd_prepare_idle_pc;
	ops->handle_post_kickoff = dpu_encoder_phys_cmd_handle_post_kickoff;
	ops->get_line_count = dpu_encoder_phys_cmd_get_line_count;
}

struct dpu_encoder_phys *dpu_encoder_phys_cmd_init(
		struct dpu_enc_phys_init_params *p)
{
	struct dpu_encoder_phys *phys_enc = NULL;
	struct dpu_encoder_phys_cmd *cmd_enc = NULL;
<<<<<<< HEAD
	struct dpu_hw_mdp *hw_mdp;
=======
>>>>>>> upstream/android-13
	struct dpu_encoder_irq *irq;
	int i, ret = 0;

	DPU_DEBUG("intf %d\n", p->intf_idx - INTF_0);

	cmd_enc = kzalloc(sizeof(*cmd_enc), GFP_KERNEL);
	if (!cmd_enc) {
		ret = -ENOMEM;
		DPU_ERROR("failed to allocate\n");
<<<<<<< HEAD
		goto fail;
	}
	phys_enc = &cmd_enc->base;

	hw_mdp = dpu_rm_get_mdp(&p->dpu_kms->rm);
	if (IS_ERR_OR_NULL(hw_mdp)) {
		ret = PTR_ERR(hw_mdp);
		DPU_ERROR("failed to get mdptop\n");
		goto fail_mdp_init;
	}
	phys_enc->hw_mdptop = hw_mdp;
=======
		return ERR_PTR(ret);
	}
	phys_enc = &cmd_enc->base;
	phys_enc->hw_mdptop = p->dpu_kms->hw_mdp;
>>>>>>> upstream/android-13
	phys_enc->intf_idx = p->intf_idx;

	dpu_encoder_phys_cmd_init_ops(&phys_enc->ops);
	phys_enc->parent = p->parent;
	phys_enc->parent_ops = p->parent_ops;
	phys_enc->dpu_kms = p->dpu_kms;
	phys_enc->split_role = p->split_role;
	phys_enc->intf_mode = INTF_MODE_CMD;
	phys_enc->enc_spinlock = p->enc_spinlock;
	cmd_enc->stream_sel = 0;
	phys_enc->enable_state = DPU_ENC_DISABLED;
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

	irq = &phys_enc->irq[INTR_IDX_CTL_START];
	irq->name = "ctl_start";
<<<<<<< HEAD
	irq->intr_type = DPU_IRQ_TYPE_CTL_START;
=======
>>>>>>> upstream/android-13
	irq->intr_idx = INTR_IDX_CTL_START;
	irq->cb.func = dpu_encoder_phys_cmd_ctl_start_irq;

	irq = &phys_enc->irq[INTR_IDX_PINGPONG];
	irq->name = "pp_done";
<<<<<<< HEAD
	irq->intr_type = DPU_IRQ_TYPE_PING_PONG_COMP;
=======
>>>>>>> upstream/android-13
	irq->intr_idx = INTR_IDX_PINGPONG;
	irq->cb.func = dpu_encoder_phys_cmd_pp_tx_done_irq;

	irq = &phys_enc->irq[INTR_IDX_RDPTR];
	irq->name = "pp_rd_ptr";
<<<<<<< HEAD
	irq->intr_type = DPU_IRQ_TYPE_PING_PONG_RD_PTR;
=======
>>>>>>> upstream/android-13
	irq->intr_idx = INTR_IDX_RDPTR;
	irq->cb.func = dpu_encoder_phys_cmd_pp_rd_ptr_irq;

	irq = &phys_enc->irq[INTR_IDX_UNDERRUN];
	irq->name = "underrun";
<<<<<<< HEAD
	irq->intr_type = DPU_IRQ_TYPE_INTF_UNDER_RUN;
=======
>>>>>>> upstream/android-13
	irq->intr_idx = INTR_IDX_UNDERRUN;
	irq->cb.func = dpu_encoder_phys_cmd_underrun_irq;

	atomic_set(&phys_enc->vblank_refcount, 0);
	atomic_set(&phys_enc->pending_kickoff_cnt, 0);
	atomic_set(&phys_enc->pending_ctlstart_cnt, 0);
	atomic_set(&cmd_enc->pending_vblank_cnt, 0);
	init_waitqueue_head(&phys_enc->pending_kickoff_wq);
	init_waitqueue_head(&cmd_enc->pending_vblank_wq);

	DPU_DEBUG_CMDENC(cmd_enc, "created\n");

	return phys_enc;
<<<<<<< HEAD

fail_mdp_init:
	kfree(cmd_enc);
fail:
	return ERR_PTR(ret);
=======
>>>>>>> upstream/android-13
}
