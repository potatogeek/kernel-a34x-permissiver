<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2015-2018, The Linux Foundation. All rights reserved.
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

#ifndef _DPU_PLANE_H_
#define _DPU_PLANE_H_

#include <drm/drm_crtc.h>

#include "dpu_kms.h"
#include "dpu_hw_mdss.h"
#include "dpu_hw_sspp.h"

/**
 * struct dpu_plane_state: Define dpu extension of drm plane state object
 * @base:	base drm plane state object
<<<<<<< HEAD
 * @property_state: Local storage for msm_prop properties
 * @property_values:	cached plane property values
 * @aspace:	pointer to address space for input/output buffers
 * @input_fence:	dereferenced input fence pointer
 * @stage:	assigned by crtc blender
=======
 * @aspace:	pointer to address space for input/output buffers
 * @stage:	assigned by crtc blender
 * @needs_qos_remap: qos remap settings need to be updated
>>>>>>> upstream/android-13
 * @multirect_index: index of the rectangle of SSPP
 * @multirect_mode: parallel or time multiplex multirect mode
 * @pending:	whether the current update is still pending
 * @scaler3_cfg: configuration data for scaler3
 * @pixel_ext: configuration data for pixel extensions
<<<<<<< HEAD
 * @scaler_check_state: indicates status of user provided pixel extension data
 * @cdp_cfg:	CDP configuration
=======
 * @cdp_cfg:	CDP configuration
 * @plane_fetch_bw: calculated BW per plane
 * @plane_clk: calculated clk per plane
>>>>>>> upstream/android-13
 */
struct dpu_plane_state {
	struct drm_plane_state base;
	struct msm_gem_address_space *aspace;
<<<<<<< HEAD
	void *input_fence;
	enum dpu_stage stage;
=======
	enum dpu_stage stage;
	bool needs_qos_remap;
>>>>>>> upstream/android-13
	uint32_t multirect_index;
	uint32_t multirect_mode;
	bool pending;

	/* scaler configuration */
	struct dpu_hw_scaler3_cfg scaler3_cfg;
	struct dpu_hw_pixel_ext pixel_ext;

	struct dpu_hw_pipe_cdp_cfg cdp_cfg;
<<<<<<< HEAD
=======
	u64 plane_fetch_bw;
	u64 plane_clk;
>>>>>>> upstream/android-13
};

/**
 * struct dpu_multirect_plane_states: Defines multirect pair of drm plane states
 * @r0: drm plane configured on rect 0
 * @r1: drm plane configured on rect 1
 */
struct dpu_multirect_plane_states {
	const struct drm_plane_state *r0;
	const struct drm_plane_state *r1;
};

#define to_dpu_plane_state(x) \
	container_of(x, struct dpu_plane_state, base)

/**
 * dpu_plane_pipe - return sspp identifier for the given plane
 * @plane:   Pointer to DRM plane object
 * Returns: sspp identifier of the given plane
 */
enum dpu_sspp dpu_plane_pipe(struct drm_plane *plane);

/**
 * is_dpu_plane_virtual - check for virtual plane
 * @plane: Pointer to DRM plane object
 * returns: true - if the plane is virtual
 *          false - if the plane is primary
 */
bool is_dpu_plane_virtual(struct drm_plane *plane);

/**
 * dpu_plane_get_ctl_flush - get control flush mask
 * @plane:   Pointer to DRM plane object
 * @ctl: Pointer to control hardware
 * @flush_sspp: Pointer to sspp flush control word
 */
void dpu_plane_get_ctl_flush(struct drm_plane *plane, struct dpu_hw_ctl *ctl,
		u32 *flush_sspp);

/**
<<<<<<< HEAD
 * dpu_plane_restore - restore hw state if previously power collapsed
 * @plane: Pointer to drm plane structure
 */
void dpu_plane_restore(struct drm_plane *plane);

/**
=======
>>>>>>> upstream/android-13
 * dpu_plane_flush - final plane operations before commit flush
 * @plane: Pointer to drm plane structure
 */
void dpu_plane_flush(struct drm_plane *plane);

/**
<<<<<<< HEAD
 * dpu_plane_kickoff - final plane operations before commit kickoff
 * @plane: Pointer to drm plane structure
 */
void dpu_plane_kickoff(struct drm_plane *plane);

/**
=======
>>>>>>> upstream/android-13
 * dpu_plane_set_error: enable/disable error condition
 * @plane: pointer to drm_plane structure
 */
void dpu_plane_set_error(struct drm_plane *plane, bool error);

/**
 * dpu_plane_init - create new dpu plane for the given pipe
 * @dev:   Pointer to DRM device
 * @pipe:  dpu hardware pipe identifier
<<<<<<< HEAD
 * @primary_plane: true if this pipe is primary plane for crtc
=======
 * @type:  Plane type - PRIMARY/OVERLAY/CURSOR
>>>>>>> upstream/android-13
 * @possible_crtcs: bitmask of crtc that can be attached to the given pipe
 * @master_plane_id: primary plane id of a multirect pipe. 0 value passed for
 *                   a regular plane initialization. A non-zero primary plane
 *                   id will be passed for a virtual pipe initialization.
 *
 */
struct drm_plane *dpu_plane_init(struct drm_device *dev,
<<<<<<< HEAD
		uint32_t pipe, bool primary_plane,
=======
		uint32_t pipe, enum drm_plane_type type,
>>>>>>> upstream/android-13
		unsigned long possible_crtcs, u32 master_plane_id);

/**
 * dpu_plane_validate_multirecti_v2 - validate the multirect planes
 *				      against hw limitations
 * @plane: drm plate states of the multirect pair
 */
int dpu_plane_validate_multirect_v2(struct dpu_multirect_plane_states *plane);

/**
 * dpu_plane_clear_multirect - clear multirect bits for the given pipe
 * @drm_state: Pointer to DRM plane state
 */
void dpu_plane_clear_multirect(const struct drm_plane_state *drm_state);

/**
<<<<<<< HEAD
 * dpu_plane_wait_input_fence - wait for input fence object
 * @plane:   Pointer to DRM plane object
 * @wait_ms: Wait timeout value
 * Returns: Zero on success
 */
int dpu_plane_wait_input_fence(struct drm_plane *plane, uint32_t wait_ms);

/**
=======
>>>>>>> upstream/android-13
 * dpu_plane_color_fill - enables color fill on plane
 * @plane:  Pointer to DRM plane object
 * @color:  RGB fill color value, [23..16] Blue, [15..8] Green, [7..0] Red
 * @alpha:  8-bit fill alpha value, 255 selects 100% alpha
 * Returns: 0 on success
 */
int dpu_plane_color_fill(struct drm_plane *plane,
		uint32_t color, uint32_t alpha);

<<<<<<< HEAD
/**
 * dpu_plane_set_revalidate - sets revalidate flag which forces a full
 *	validation of the plane properties in the next atomic check
 * @plane: Pointer to DRM plane object
 * @enable: Boolean to set/unset the flag
 */
void dpu_plane_set_revalidate(struct drm_plane *plane, bool enable);

=======
>>>>>>> upstream/android-13
#endif /* _DPU_PLANE_H_ */
