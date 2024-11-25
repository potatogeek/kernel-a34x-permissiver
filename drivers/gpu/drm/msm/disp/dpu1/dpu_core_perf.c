<<<<<<< HEAD
/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
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
/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__

#include <linux/debugfs.h>
#include <linux/errno.h>
#include <linux/mutex.h>
<<<<<<< HEAD
=======
#include <linux/pm_opp.h>
>>>>>>> upstream/android-13
#include <linux/sort.h>
#include <linux/clk.h>
#include <linux/bitmap.h>

#include "dpu_kms.h"
#include "dpu_trace.h"
#include "dpu_crtc.h"
#include "dpu_core_perf.h"

<<<<<<< HEAD
#define DPU_PERF_MODE_STRING_SIZE	128

=======
>>>>>>> upstream/android-13
/**
 * enum dpu_perf_mode - performance tuning mode
 * @DPU_PERF_MODE_NORMAL: performance controlled by user mode client
 * @DPU_PERF_MODE_MINIMUM: performance bounded by minimum setting
 * @DPU_PERF_MODE_FIXED: performance bounded by fixed setting
<<<<<<< HEAD
=======
 * @DPU_PERF_MODE_MAX: maximum value, used for error checking
>>>>>>> upstream/android-13
 */
enum dpu_perf_mode {
	DPU_PERF_MODE_NORMAL,
	DPU_PERF_MODE_MINIMUM,
	DPU_PERF_MODE_FIXED,
	DPU_PERF_MODE_MAX
};

<<<<<<< HEAD
static struct dpu_kms *_dpu_crtc_get_kms(struct drm_crtc *crtc)
{
	struct msm_drm_private *priv;

	if (!crtc->dev || !crtc->dev->dev_private) {
		DPU_ERROR("invalid device\n");
		return NULL;
	}

	priv = crtc->dev->dev_private;
	if (!priv || !priv->kms) {
		DPU_ERROR("invalid kms\n");
		return NULL;
	}

	return to_dpu_kms(priv->kms);
}

static bool _dpu_core_perf_crtc_is_power_on(struct drm_crtc *crtc)
{
	return dpu_crtc_is_enabled(crtc);
}

static bool _dpu_core_video_mode_intf_connected(struct drm_crtc *crtc)
{
	struct drm_crtc *tmp_crtc;
	bool intf_connected = false;

	if (!crtc)
		goto end;

	drm_for_each_crtc(tmp_crtc, crtc->dev) {
		if ((dpu_crtc_get_intf_mode(tmp_crtc) == INTF_MODE_VIDEO) &&
				_dpu_core_perf_crtc_is_power_on(tmp_crtc)) {
			DPU_DEBUG("video interface connected crtc:%d\n",
				tmp_crtc->base.id);
			intf_connected = true;
			goto end;
		}
	}

end:
	return intf_connected;
}

=======
/**
 * _dpu_core_perf_calc_bw() - to calculate BW per crtc
 * @kms:  pointer to the dpu_kms
 * @crtc: pointer to a crtc
 * Return: returns aggregated BW for all planes in crtc.
 */
static u64 _dpu_core_perf_calc_bw(struct dpu_kms *kms,
		struct drm_crtc *crtc)
{
	struct drm_plane *plane;
	struct dpu_plane_state *pstate;
	u64 crtc_plane_bw = 0;
	u32 bw_factor;

	drm_atomic_crtc_for_each_plane(plane, crtc) {
		pstate = to_dpu_plane_state(plane->state);
		if (!pstate)
			continue;

		crtc_plane_bw += pstate->plane_fetch_bw;
	}

	bw_factor = kms->catalog->perf.bw_inefficiency_factor;
	if (bw_factor) {
		crtc_plane_bw *= bw_factor;
		do_div(crtc_plane_bw, 100);
	}

	return crtc_plane_bw;
}

/**
 * _dpu_core_perf_calc_clk() - to calculate clock per crtc
 * @kms:  pointer to the dpu_kms
 * @crtc: pointer to a crtc
 * @state: pointer to a crtc state
 * Return: returns max clk for all planes in crtc.
 */
static u64 _dpu_core_perf_calc_clk(struct dpu_kms *kms,
		struct drm_crtc *crtc, struct drm_crtc_state *state)
{
	struct drm_plane *plane;
	struct dpu_plane_state *pstate;
	struct drm_display_mode *mode;
	u64 crtc_clk;
	u32 clk_factor;

	mode = &state->adjusted_mode;

	crtc_clk = mode->vtotal * mode->hdisplay * drm_mode_vrefresh(mode);

	drm_atomic_crtc_for_each_plane(plane, crtc) {
		pstate = to_dpu_plane_state(plane->state);
		if (!pstate)
			continue;

		crtc_clk = max(pstate->plane_clk, crtc_clk);
	}

	clk_factor = kms->catalog->perf.clk_inefficiency_factor;
	if (clk_factor) {
		crtc_clk *= clk_factor;
		do_div(crtc_clk, 100);
	}

	return crtc_clk;
}

static struct dpu_kms *_dpu_crtc_get_kms(struct drm_crtc *crtc)
{
	struct msm_drm_private *priv;
	priv = crtc->dev->dev_private;
	return to_dpu_kms(priv->kms);
}

>>>>>>> upstream/android-13
static void _dpu_core_perf_calc_crtc(struct dpu_kms *kms,
		struct drm_crtc *crtc,
		struct drm_crtc_state *state,
		struct dpu_core_perf_params *perf)
{
<<<<<<< HEAD
	struct dpu_crtc_state *dpu_cstate;
	int i;

=======
>>>>>>> upstream/android-13
	if (!kms || !kms->catalog || !crtc || !state || !perf) {
		DPU_ERROR("invalid parameters\n");
		return;
	}

<<<<<<< HEAD
	dpu_cstate = to_dpu_crtc_state(state);
	memset(perf, 0, sizeof(struct dpu_core_perf_params));

	if (!dpu_cstate->bw_control) {
		for (i = 0; i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
			perf->bw_ctl[i] = kms->catalog->perf.max_bw_high *
					1000ULL;
			perf->max_per_pipe_ib[i] = perf->bw_ctl[i];
		}
		perf->core_clk_rate = kms->perf.max_core_clk_rate;
	} else if (kms->perf.perf_tune.mode == DPU_PERF_MODE_MINIMUM) {
		for (i = 0; i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
			perf->bw_ctl[i] = 0;
			perf->max_per_pipe_ib[i] = 0;
		}
		perf->core_clk_rate = 0;
	} else if (kms->perf.perf_tune.mode == DPU_PERF_MODE_FIXED) {
		for (i = 0; i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
			perf->bw_ctl[i] = kms->perf.fix_core_ab_vote;
			perf->max_per_pipe_ib[i] = kms->perf.fix_core_ib_vote;
		}
		perf->core_clk_rate = kms->perf.fix_core_clk_rate;
	}

	DPU_DEBUG(
		"crtc=%d clk_rate=%llu core_ib=%llu core_ab=%llu llcc_ib=%llu llcc_ab=%llu mem_ib=%llu mem_ab=%llu\n",
			crtc->base.id, perf->core_clk_rate,
			perf->max_per_pipe_ib[DPU_POWER_HANDLE_DBUS_ID_MNOC],
			perf->bw_ctl[DPU_POWER_HANDLE_DBUS_ID_MNOC],
			perf->max_per_pipe_ib[DPU_POWER_HANDLE_DBUS_ID_LLCC],
			perf->bw_ctl[DPU_POWER_HANDLE_DBUS_ID_LLCC],
			perf->max_per_pipe_ib[DPU_POWER_HANDLE_DBUS_ID_EBI],
			perf->bw_ctl[DPU_POWER_HANDLE_DBUS_ID_EBI]);
=======
	memset(perf, 0, sizeof(struct dpu_core_perf_params));

	if (kms->perf.perf_tune.mode == DPU_PERF_MODE_MINIMUM) {
		perf->bw_ctl = 0;
		perf->max_per_pipe_ib = 0;
		perf->core_clk_rate = 0;
	} else if (kms->perf.perf_tune.mode == DPU_PERF_MODE_FIXED) {
		perf->bw_ctl = kms->perf.fix_core_ab_vote;
		perf->max_per_pipe_ib = kms->perf.fix_core_ib_vote;
		perf->core_clk_rate = kms->perf.fix_core_clk_rate;
	} else {
		perf->bw_ctl = _dpu_core_perf_calc_bw(kms, crtc);
		perf->max_per_pipe_ib = kms->catalog->perf.min_dram_ib;
		perf->core_clk_rate = _dpu_core_perf_calc_clk(kms, crtc, state);
	}

	DRM_DEBUG_ATOMIC(
		"crtc=%d clk_rate=%llu core_ib=%llu core_ab=%llu\n",
			crtc->base.id, perf->core_clk_rate,
			perf->max_per_pipe_ib, perf->bw_ctl);
>>>>>>> upstream/android-13
}

int dpu_core_perf_crtc_check(struct drm_crtc *crtc,
		struct drm_crtc_state *state)
{
	u32 bw, threshold;
	u64 bw_sum_of_intfs = 0;
	enum dpu_crtc_client_type curr_client_type;
<<<<<<< HEAD
	bool is_video_mode;
	struct dpu_crtc_state *dpu_cstate;
	struct drm_crtc *tmp_crtc;
	struct dpu_kms *kms;
	int i;
=======
	struct dpu_crtc_state *dpu_cstate;
	struct drm_crtc *tmp_crtc;
	struct dpu_kms *kms;
>>>>>>> upstream/android-13

	if (!crtc || !state) {
		DPU_ERROR("invalid crtc\n");
		return -EINVAL;
	}

	kms = _dpu_crtc_get_kms(crtc);
<<<<<<< HEAD
	if (!kms || !kms->catalog) {
=======
	if (!kms->catalog) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid parameters\n");
		return 0;
	}

	/* we only need bandwidth check on real-time clients (interfaces) */
	if (dpu_crtc_get_client_type(crtc) == NRT_CLIENT)
		return 0;

	dpu_cstate = to_dpu_crtc_state(state);

	/* obtain new values */
	_dpu_core_perf_calc_crtc(kms, crtc, state, &dpu_cstate->new_perf);

<<<<<<< HEAD
	for (i = DPU_POWER_HANDLE_DBUS_ID_MNOC;
			i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
		bw_sum_of_intfs = dpu_cstate->new_perf.bw_ctl[i];
		curr_client_type = dpu_crtc_get_client_type(crtc);

		drm_for_each_crtc(tmp_crtc, crtc->dev) {
			if (_dpu_core_perf_crtc_is_power_on(tmp_crtc) &&
			    (dpu_crtc_get_client_type(tmp_crtc) ==
					    curr_client_type) &&
			    (tmp_crtc != crtc)) {
				struct dpu_crtc_state *tmp_cstate =
					to_dpu_crtc_state(tmp_crtc->state);

				DPU_DEBUG("crtc:%d bw:%llu ctrl:%d\n",
					tmp_crtc->base.id,
					tmp_cstate->new_perf.bw_ctl[i],
					tmp_cstate->bw_control);
				/*
				 * For bw check only use the bw if the
				 * atomic property has been already set
				 */
				if (tmp_cstate->bw_control)
					bw_sum_of_intfs +=
						tmp_cstate->new_perf.bw_ctl[i];
			}
=======
	bw_sum_of_intfs = dpu_cstate->new_perf.bw_ctl;
	curr_client_type = dpu_crtc_get_client_type(crtc);

	drm_for_each_crtc(tmp_crtc, crtc->dev) {
		if (tmp_crtc->enabled &&
		    (dpu_crtc_get_client_type(tmp_crtc) ==
				curr_client_type) && (tmp_crtc != crtc)) {
			struct dpu_crtc_state *tmp_cstate =
				to_dpu_crtc_state(tmp_crtc->state);

			DRM_DEBUG_ATOMIC("crtc:%d bw:%llu ctrl:%d\n",
				tmp_crtc->base.id, tmp_cstate->new_perf.bw_ctl,
				tmp_cstate->bw_control);

				bw_sum_of_intfs += tmp_cstate->new_perf.bw_ctl;
>>>>>>> upstream/android-13
		}

		/* convert bandwidth to kb */
		bw = DIV_ROUND_UP_ULL(bw_sum_of_intfs, 1000);
<<<<<<< HEAD
		DPU_DEBUG("calculated bandwidth=%uk\n", bw);

		is_video_mode = dpu_crtc_get_intf_mode(crtc) == INTF_MODE_VIDEO;
		threshold = (is_video_mode ||
			_dpu_core_video_mode_intf_connected(crtc)) ?
			kms->catalog->perf.max_bw_low :
			kms->catalog->perf.max_bw_high;

		DPU_DEBUG("final threshold bw limit = %d\n", threshold);

		if (!dpu_cstate->bw_control) {
			DPU_DEBUG("bypass bandwidth check\n");
		} else if (!threshold) {
=======
		DRM_DEBUG_ATOMIC("calculated bandwidth=%uk\n", bw);

		threshold = kms->catalog->perf.max_bw_high;

		DRM_DEBUG_ATOMIC("final threshold bw limit = %d\n", threshold);

		if (!threshold) {
>>>>>>> upstream/android-13
			DPU_ERROR("no bandwidth limits specified\n");
			return -E2BIG;
		} else if (bw > threshold) {
			DPU_ERROR("exceeds bandwidth: %ukb > %ukb\n", bw,
					threshold);
			return -E2BIG;
		}
	}

	return 0;
}

static int _dpu_core_perf_crtc_update_bus(struct dpu_kms *kms,
<<<<<<< HEAD
		struct drm_crtc *crtc, u32 bus_id)
{
	struct dpu_core_perf_params perf = { { 0 } };
=======
		struct drm_crtc *crtc)
{
	struct dpu_core_perf_params perf = { 0 };
>>>>>>> upstream/android-13
	enum dpu_crtc_client_type curr_client_type
					= dpu_crtc_get_client_type(crtc);
	struct drm_crtc *tmp_crtc;
	struct dpu_crtc_state *dpu_cstate;
<<<<<<< HEAD
	int ret = 0;

	drm_for_each_crtc(tmp_crtc, crtc->dev) {
		if (_dpu_core_perf_crtc_is_power_on(tmp_crtc) &&
=======
	int i, ret = 0;
	u64 avg_bw;

	drm_for_each_crtc(tmp_crtc, crtc->dev) {
		if (tmp_crtc->enabled &&
>>>>>>> upstream/android-13
			curr_client_type ==
				dpu_crtc_get_client_type(tmp_crtc)) {
			dpu_cstate = to_dpu_crtc_state(tmp_crtc->state);

<<<<<<< HEAD
			perf.max_per_pipe_ib[bus_id] =
				max(perf.max_per_pipe_ib[bus_id],
				dpu_cstate->new_perf.max_per_pipe_ib[bus_id]);

			DPU_DEBUG("crtc=%d bus_id=%d bw=%llu\n",
				tmp_crtc->base.id, bus_id,
				dpu_cstate->new_perf.bw_ctl[bus_id]);
		}
	}
=======
			perf.max_per_pipe_ib = max(perf.max_per_pipe_ib,
					dpu_cstate->new_perf.max_per_pipe_ib);

			perf.bw_ctl += dpu_cstate->new_perf.bw_ctl;

			DRM_DEBUG_ATOMIC("crtc=%d bw=%llu paths:%d\n",
				  tmp_crtc->base.id,
				  dpu_cstate->new_perf.bw_ctl, kms->num_paths);
		}
	}

	if (!kms->num_paths)
		return 0;

	avg_bw = perf.bw_ctl;
	do_div(avg_bw, (kms->num_paths * 1000)); /*Bps_to_icc*/

	for (i = 0; i < kms->num_paths; i++)
		icc_set_bw(kms->path[i], avg_bw, perf.max_per_pipe_ib);

>>>>>>> upstream/android-13
	return ret;
}

/**
<<<<<<< HEAD
 * @dpu_core_perf_crtc_release_bw() - request zero bandwidth
 * @crtc - pointer to a crtc
=======
 * dpu_core_perf_crtc_release_bw() - request zero bandwidth
 * @crtc: pointer to a crtc
>>>>>>> upstream/android-13
 *
 * Function checks a state variable for the crtc, if all pending commit
 * requests are done, meaning no more bandwidth is needed, release
 * bandwidth request.
 */
void dpu_core_perf_crtc_release_bw(struct drm_crtc *crtc)
{
<<<<<<< HEAD
	struct drm_crtc *tmp_crtc;
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *dpu_cstate;
	struct dpu_kms *kms;
	int i;
=======
	struct dpu_crtc *dpu_crtc;
	struct dpu_kms *kms;
>>>>>>> upstream/android-13

	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return;
	}

	kms = _dpu_crtc_get_kms(crtc);
<<<<<<< HEAD
	if (!kms || !kms->catalog) {
=======
	if (!kms->catalog) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid kms\n");
		return;
	}

	dpu_crtc = to_dpu_crtc(crtc);
<<<<<<< HEAD
	dpu_cstate = to_dpu_crtc_state(crtc->state);

	/* only do this for command mode rt client */
	if (dpu_crtc_get_intf_mode(crtc) != INTF_MODE_CMD)
		return;

	/*
	 * If video interface present, cmd panel bandwidth cannot be
	 * released.
	 */
	if (dpu_crtc_get_intf_mode(crtc) == INTF_MODE_CMD)
		drm_for_each_crtc(tmp_crtc, crtc->dev) {
			if (_dpu_core_perf_crtc_is_power_on(tmp_crtc) &&
				dpu_crtc_get_intf_mode(tmp_crtc) ==
						INTF_MODE_VIDEO)
				return;
		}

	/* Release the bandwidth */
	if (kms->perf.enable_bw_release) {
		trace_dpu_cmd_release_bw(crtc->base.id);
		DPU_DEBUG("Release BW crtc=%d\n", crtc->base.id);
		for (i = 0; i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
			dpu_crtc->cur_perf.bw_ctl[i] = 0;
			_dpu_core_perf_crtc_update_bus(kms, crtc, i);
		}
=======

	if (atomic_dec_return(&kms->bandwidth_ref) > 0)
		return;

	/* Release the bandwidth */
	if (kms->perf.enable_bw_release) {
		trace_dpu_cmd_release_bw(crtc->base.id);
		DRM_DEBUG_ATOMIC("Release BW crtc=%d\n", crtc->base.id);
		dpu_crtc->cur_perf.bw_ctl = 0;
		_dpu_core_perf_crtc_update_bus(kms, crtc);
>>>>>>> upstream/android-13
	}
}

static int _dpu_core_perf_set_core_clk_rate(struct dpu_kms *kms, u64 rate)
{
	struct dss_clk *core_clk = kms->perf.core_clk;

	if (core_clk->max_rate && (rate > core_clk->max_rate))
		rate = core_clk->max_rate;

	core_clk->rate = rate;
<<<<<<< HEAD
	return msm_dss_clk_set_rate(core_clk, 1);
=======
	return dev_pm_opp_set_rate(&kms->pdev->dev, core_clk->rate);
>>>>>>> upstream/android-13
}

static u64 _dpu_core_perf_get_core_clk_rate(struct dpu_kms *kms)
{
	u64 clk_rate = kms->perf.perf_tune.min_core_clk;
	struct drm_crtc *crtc;
	struct dpu_crtc_state *dpu_cstate;

	drm_for_each_crtc(crtc, kms->dev) {
<<<<<<< HEAD
		if (_dpu_core_perf_crtc_is_power_on(crtc)) {
=======
		if (crtc->enabled) {
>>>>>>> upstream/android-13
			dpu_cstate = to_dpu_crtc_state(crtc->state);
			clk_rate = max(dpu_cstate->new_perf.core_clk_rate,
							clk_rate);
			clk_rate = clk_round_rate(kms->perf.core_clk->clk,
					clk_rate);
		}
	}

	if (kms->perf.perf_tune.mode == DPU_PERF_MODE_FIXED)
		clk_rate = kms->perf.fix_core_clk_rate;

<<<<<<< HEAD
	DPU_DEBUG("clk:%llu\n", clk_rate);
=======
	DRM_DEBUG_ATOMIC("clk:%llu\n", clk_rate);
>>>>>>> upstream/android-13

	return clk_rate;
}

int dpu_core_perf_crtc_update(struct drm_crtc *crtc,
		int params_changed, bool stop_req)
{
	struct dpu_core_perf_params *new, *old;
<<<<<<< HEAD
	int update_bus = 0, update_clk = 0;
	u64 clk_rate = 0;
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *dpu_cstate;
	int i;
	struct msm_drm_private *priv;
=======
	bool update_bus = false, update_clk = false;
	u64 clk_rate = 0;
	struct dpu_crtc *dpu_crtc;
	struct dpu_crtc_state *dpu_cstate;
>>>>>>> upstream/android-13
	struct dpu_kms *kms;
	int ret;

	if (!crtc) {
		DPU_ERROR("invalid crtc\n");
		return -EINVAL;
	}

	kms = _dpu_crtc_get_kms(crtc);
<<<<<<< HEAD
	if (!kms || !kms->catalog) {
		DPU_ERROR("invalid kms\n");
		return -EINVAL;
	}
	priv = kms->dev->dev_private;
=======
	if (!kms->catalog) {
		DPU_ERROR("invalid kms\n");
		return -EINVAL;
	}
>>>>>>> upstream/android-13

	dpu_crtc = to_dpu_crtc(crtc);
	dpu_cstate = to_dpu_crtc_state(crtc->state);

<<<<<<< HEAD
	DPU_DEBUG("crtc:%d stop_req:%d core_clk:%llu\n",
=======
	DRM_DEBUG_ATOMIC("crtc:%d stop_req:%d core_clk:%llu\n",
>>>>>>> upstream/android-13
			crtc->base.id, stop_req, kms->perf.core_clk_rate);

	old = &dpu_crtc->cur_perf;
	new = &dpu_cstate->new_perf;

<<<<<<< HEAD
	if (_dpu_core_perf_crtc_is_power_on(crtc) && !stop_req) {
		for (i = 0; i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
			/*
			 * cases for bus bandwidth update.
			 * 1. new bandwidth vote - "ab or ib vote" is higher
			 *    than current vote for update request.
			 * 2. new bandwidth vote - "ab or ib vote" is lower
			 *    than current vote at end of commit or stop.
			 */
			if ((params_changed && ((new->bw_ctl[i] >
						old->bw_ctl[i]) ||
				  (new->max_per_pipe_ib[i] >
						old->max_per_pipe_ib[i]))) ||
			    (!params_changed && ((new->bw_ctl[i] <
						old->bw_ctl[i]) ||
				  (new->max_per_pipe_ib[i] <
						old->max_per_pipe_ib[i])))) {
				DPU_DEBUG(
					"crtc=%d p=%d new_bw=%llu,old_bw=%llu\n",
					crtc->base.id, params_changed,
					new->bw_ctl[i], old->bw_ctl[i]);
				old->bw_ctl[i] = new->bw_ctl[i];
				old->max_per_pipe_ib[i] =
						new->max_per_pipe_ib[i];
				update_bus |= BIT(i);
			}
		}

		if ((params_changed &&
				(new->core_clk_rate > old->core_clk_rate)) ||
				(!params_changed &&
				(new->core_clk_rate < old->core_clk_rate))) {
			old->core_clk_rate = new->core_clk_rate;
			update_clk = 1;
		}
	} else {
		DPU_DEBUG("crtc=%d disable\n", crtc->base.id);
		memset(old, 0, sizeof(*old));
		memset(new, 0, sizeof(*new));
		update_bus = ~0;
		update_clk = 1;
	}
	trace_dpu_perf_crtc_update(crtc->base.id,
				new->bw_ctl[DPU_POWER_HANDLE_DBUS_ID_MNOC],
				new->bw_ctl[DPU_POWER_HANDLE_DBUS_ID_LLCC],
				new->bw_ctl[DPU_POWER_HANDLE_DBUS_ID_EBI],
				new->core_clk_rate, stop_req,
				update_bus, update_clk);

	for (i = 0; i < DPU_POWER_HANDLE_DBUS_ID_MAX; i++) {
		if (update_bus & BIT(i)) {
			ret = _dpu_core_perf_crtc_update_bus(kms, crtc, i);
			if (ret) {
				DPU_ERROR("crtc-%d: failed to update bw vote for bus-%d\n",
					  crtc->base.id, i);
				return ret;
			}
=======
	if (crtc->enabled && !stop_req) {
		/*
		 * cases for bus bandwidth update.
		 * 1. new bandwidth vote - "ab or ib vote" is higher
		 *    than current vote for update request.
		 * 2. new bandwidth vote - "ab or ib vote" is lower
		 *    than current vote at end of commit or stop.
		 */
		if ((params_changed && ((new->bw_ctl > old->bw_ctl) ||
			(new->max_per_pipe_ib > old->max_per_pipe_ib)))	||
			(!params_changed && ((new->bw_ctl < old->bw_ctl) ||
			(new->max_per_pipe_ib < old->max_per_pipe_ib)))) {
			DRM_DEBUG_ATOMIC("crtc=%d p=%d new_bw=%llu,old_bw=%llu\n",
				crtc->base.id, params_changed,
				new->bw_ctl, old->bw_ctl);
			old->bw_ctl = new->bw_ctl;
			old->max_per_pipe_ib = new->max_per_pipe_ib;
			update_bus = true;
		}

		if ((params_changed &&
			(new->core_clk_rate > old->core_clk_rate)) ||
			(!params_changed &&
			(new->core_clk_rate < old->core_clk_rate))) {
			old->core_clk_rate = new->core_clk_rate;
			update_clk = true;
		}
	} else {
		DRM_DEBUG_ATOMIC("crtc=%d disable\n", crtc->base.id);
		memset(old, 0, sizeof(*old));
		update_bus = true;
		update_clk = true;
	}

	trace_dpu_perf_crtc_update(crtc->base.id, new->bw_ctl,
		new->core_clk_rate, stop_req, update_bus, update_clk);

	if (update_bus) {
		ret = _dpu_core_perf_crtc_update_bus(kms, crtc);
		if (ret) {
			DPU_ERROR("crtc-%d: failed to update bus bw vote\n",
				  crtc->base.id);
			return ret;
>>>>>>> upstream/android-13
		}
	}

	/*
	 * Update the clock after bandwidth vote to ensure
	 * bandwidth is available before clock rate is increased.
	 */
	if (update_clk) {
		clk_rate = _dpu_core_perf_get_core_clk_rate(kms);

		trace_dpu_core_perf_update_clk(kms->dev, stop_req, clk_rate);

		ret = _dpu_core_perf_set_core_clk_rate(kms, clk_rate);
		if (ret) {
			DPU_ERROR("failed to set %s clock rate %llu\n",
					kms->perf.core_clk->clk_name, clk_rate);
			return ret;
		}

		kms->perf.core_clk_rate = clk_rate;
<<<<<<< HEAD
		DPU_DEBUG("update clk rate = %lld HZ\n", clk_rate);
=======
		DRM_DEBUG_ATOMIC("update clk rate = %lld HZ\n", clk_rate);
>>>>>>> upstream/android-13
	}
	return 0;
}

#ifdef CONFIG_DEBUG_FS

static ssize_t _dpu_core_perf_mode_write(struct file *file,
		    const char __user *user_buf, size_t count, loff_t *ppos)
{
	struct dpu_core_perf *perf = file->private_data;
	struct dpu_perf_cfg *cfg = &perf->catalog->perf;
	u32 perf_mode = 0;
<<<<<<< HEAD
	char buf[10];

	if (!perf)
		return -ENODEV;

	if (count >= sizeof(buf))
		return -EFAULT;

	if (copy_from_user(buf, user_buf, count))
		return -EFAULT;

	buf[count] = 0;	/* end of string */

	if (kstrtouint(buf, 0, &perf_mode))
		return -EFAULT;

	if (perf_mode >= DPU_PERF_MODE_MAX)
		return -EFAULT;
=======
	int ret;

	ret = kstrtouint_from_user(user_buf, count, 0, &perf_mode);
	if (ret)
		return ret;

	if (perf_mode >= DPU_PERF_MODE_MAX)
		return -EINVAL;
>>>>>>> upstream/android-13

	if (perf_mode == DPU_PERF_MODE_FIXED) {
		DRM_INFO("fix performance mode\n");
	} else if (perf_mode == DPU_PERF_MODE_MINIMUM) {
		/* run the driver with max clk and BW vote */
		perf->perf_tune.min_core_clk = perf->max_core_clk_rate;
		perf->perf_tune.min_bus_vote =
				(u64) cfg->max_bw_high * 1000;
		DRM_INFO("minimum performance mode\n");
	} else if (perf_mode == DPU_PERF_MODE_NORMAL) {
		/* reset the perf tune params to 0 */
		perf->perf_tune.min_core_clk = 0;
		perf->perf_tune.min_bus_vote = 0;
		DRM_INFO("normal performance mode\n");
	}
	perf->perf_tune.mode = perf_mode;

	return count;
}

static ssize_t _dpu_core_perf_mode_read(struct file *file,
			char __user *buff, size_t count, loff_t *ppos)
{
	struct dpu_core_perf *perf = file->private_data;
<<<<<<< HEAD
	int len = 0;
	char buf[DPU_PERF_MODE_STRING_SIZE] = {'\0'};

	if (!perf)
		return -ENODEV;

	if (*ppos)
		return 0;	/* the end */

	len = snprintf(buf, sizeof(buf),
=======
	int len;
	char buf[128];

	len = scnprintf(buf, sizeof(buf),
>>>>>>> upstream/android-13
			"mode %d min_mdp_clk %llu min_bus_vote %llu\n",
			perf->perf_tune.mode,
			perf->perf_tune.min_core_clk,
			perf->perf_tune.min_bus_vote);
<<<<<<< HEAD
	if (len < 0 || len >= sizeof(buf))
		return 0;

	if ((count < sizeof(buf)) || copy_to_user(buff, buf, len))
		return -EFAULT;

	*ppos += len;   /* increase offset */

	return len;
=======

	return simple_read_from_buffer(buff, count, ppos, buf, len);
>>>>>>> upstream/android-13
}

static const struct file_operations dpu_core_perf_mode_fops = {
	.open = simple_open,
	.read = _dpu_core_perf_mode_read,
	.write = _dpu_core_perf_mode_write,
};

<<<<<<< HEAD
static void dpu_core_perf_debugfs_destroy(struct dpu_core_perf *perf)
{
	debugfs_remove_recursive(perf->debugfs_root);
	perf->debugfs_root = NULL;
}

int dpu_core_perf_debugfs_init(struct dpu_core_perf *perf,
		struct dentry *parent)
{
	struct dpu_mdss_cfg *catalog = perf->catalog;
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms;

	priv = perf->dev->dev_private;
	if (!priv || !priv->kms) {
		DPU_ERROR("invalid KMS reference\n");
		return -EINVAL;
	}

	dpu_kms = to_dpu_kms(priv->kms);

	perf->debugfs_root = debugfs_create_dir("core_perf", parent);
	if (!perf->debugfs_root) {
		DPU_ERROR("failed to create core perf debugfs\n");
		return -EINVAL;
	}

	debugfs_create_u64("max_core_clk_rate", 0600, perf->debugfs_root,
			&perf->max_core_clk_rate);
	debugfs_create_u64("core_clk_rate", 0600, perf->debugfs_root,
			&perf->core_clk_rate);
	debugfs_create_u32("enable_bw_release", 0600, perf->debugfs_root,
			(u32 *)&perf->enable_bw_release);
	debugfs_create_u32("threshold_low", 0600, perf->debugfs_root,
			(u32 *)&catalog->perf.max_bw_low);
	debugfs_create_u32("threshold_high", 0600, perf->debugfs_root,
			(u32 *)&catalog->perf.max_bw_high);
	debugfs_create_u32("min_core_ib", 0600, perf->debugfs_root,
			(u32 *)&catalog->perf.min_core_ib);
	debugfs_create_u32("min_llcc_ib", 0600, perf->debugfs_root,
			(u32 *)&catalog->perf.min_llcc_ib);
	debugfs_create_u32("min_dram_ib", 0600, perf->debugfs_root,
			(u32 *)&catalog->perf.min_dram_ib);
	debugfs_create_file("perf_mode", 0600, perf->debugfs_root,
			(u32 *)perf, &dpu_core_perf_mode_fops);
	debugfs_create_u64("fix_core_clk_rate", 0600, perf->debugfs_root,
			&perf->fix_core_clk_rate);
	debugfs_create_u64("fix_core_ib_vote", 0600, perf->debugfs_root,
			&perf->fix_core_ib_vote);
	debugfs_create_u64("fix_core_ab_vote", 0600, perf->debugfs_root,
=======
int dpu_core_perf_debugfs_init(struct dpu_kms *dpu_kms, struct dentry *parent)
{
	struct dpu_core_perf *perf = &dpu_kms->perf;
	struct dpu_mdss_cfg *catalog = perf->catalog;
	struct dentry *entry;

	entry = debugfs_create_dir("core_perf", parent);

	debugfs_create_u64("max_core_clk_rate", 0600, entry,
			&perf->max_core_clk_rate);
	debugfs_create_u64("core_clk_rate", 0600, entry,
			&perf->core_clk_rate);
	debugfs_create_u32("enable_bw_release", 0600, entry,
			(u32 *)&perf->enable_bw_release);
	debugfs_create_u32("threshold_low", 0600, entry,
			(u32 *)&catalog->perf.max_bw_low);
	debugfs_create_u32("threshold_high", 0600, entry,
			(u32 *)&catalog->perf.max_bw_high);
	debugfs_create_u32("min_core_ib", 0600, entry,
			(u32 *)&catalog->perf.min_core_ib);
	debugfs_create_u32("min_llcc_ib", 0600, entry,
			(u32 *)&catalog->perf.min_llcc_ib);
	debugfs_create_u32("min_dram_ib", 0600, entry,
			(u32 *)&catalog->perf.min_dram_ib);
	debugfs_create_file("perf_mode", 0600, entry,
			(u32 *)perf, &dpu_core_perf_mode_fops);
	debugfs_create_u64("fix_core_clk_rate", 0600, entry,
			&perf->fix_core_clk_rate);
	debugfs_create_u64("fix_core_ib_vote", 0600, entry,
			&perf->fix_core_ib_vote);
	debugfs_create_u64("fix_core_ab_vote", 0600, entry,
>>>>>>> upstream/android-13
			&perf->fix_core_ab_vote);

	return 0;
}
<<<<<<< HEAD
#else
static void dpu_core_perf_debugfs_destroy(struct dpu_core_perf *perf)
{
}

int dpu_core_perf_debugfs_init(struct dpu_core_perf *perf,
		struct dentry *parent)
{
	return 0;
}
=======
>>>>>>> upstream/android-13
#endif

void dpu_core_perf_destroy(struct dpu_core_perf *perf)
{
	if (!perf) {
		DPU_ERROR("invalid parameters\n");
		return;
	}

<<<<<<< HEAD
	dpu_core_perf_debugfs_destroy(perf);
	perf->max_core_clk_rate = 0;
	perf->core_clk = NULL;
	perf->phandle = NULL;
=======
	perf->max_core_clk_rate = 0;
	perf->core_clk = NULL;
>>>>>>> upstream/android-13
	perf->catalog = NULL;
	perf->dev = NULL;
}

int dpu_core_perf_init(struct dpu_core_perf *perf,
		struct drm_device *dev,
		struct dpu_mdss_cfg *catalog,
<<<<<<< HEAD
		struct dpu_power_handle *phandle,
=======
>>>>>>> upstream/android-13
		struct dss_clk *core_clk)
{
	perf->dev = dev;
	perf->catalog = catalog;
<<<<<<< HEAD
	perf->phandle = phandle;
=======
>>>>>>> upstream/android-13
	perf->core_clk = core_clk;

	perf->max_core_clk_rate = core_clk->max_rate;
	if (!perf->max_core_clk_rate) {
		DPU_DEBUG("optional max core clk rate, use default\n");
		perf->max_core_clk_rate = DPU_PERF_DEFAULT_MAX_CORE_CLK_RATE;
	}

	return 0;
}
