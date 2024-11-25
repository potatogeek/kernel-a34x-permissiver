<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * rcar_du_crtc.h  --  R-Car Display Unit CRTCs
 *
 * Copyright (C) 2013-2015 Renesas Electronics Corporation
 *
 * Contact: Laurent Pinchart (laurent.pinchart@ideasonboard.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __RCAR_DU_CRTC_H__
#define __RCAR_DU_CRTC_H__

#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
=======
#include <drm/drm_crtc.h>
#include <drm/drm_writeback.h>
>>>>>>> upstream/android-13

#include <media/vsp1.h>

struct rcar_du_group;
struct rcar_du_vsp;

/**
 * struct rcar_du_crtc - the CRTC, representing a DU superposition processor
 * @crtc: base DRM CRTC
<<<<<<< HEAD
 * @clock: the CRTC functional clock
 * @extclock: external pixel dot clock (optional)
 * @mmio_offset: offset of the CRTC registers in the DU MMIO block
 * @index: CRTC software and hardware index
 * @initialized: whether the CRTC has been initialized and clocks enabled
=======
 * @dev: the DU device
 * @clock: the CRTC functional clock
 * @extclock: external pixel dot clock (optional)
 * @mmio_offset: offset of the CRTC registers in the DU MMIO block
 * @index: CRTC hardware index
 * @initialized: whether the CRTC has been initialized and clocks enabled
 * @dsysr: cached value of the DSYSR register
>>>>>>> upstream/android-13
 * @vblank_enable: whether vblank events are enabled on this CRTC
 * @event: event to post when the pending page flip completes
 * @flip_wait: wait queue used to signal page flip completion
 * @vblank_lock: protects vblank_wait and vblank_count
 * @vblank_wait: wait queue used to signal vertical blanking
 * @vblank_count: number of vertical blanking interrupts to wait for
<<<<<<< HEAD
 * @outputs: bitmask of the outputs (enum rcar_du_output) driven by this CRTC
 * @group: CRTC group this CRTC belongs to
 * @vsp: VSP feeding video to this CRTC
 * @vsp_pipe: index of the VSP pipeline feeding video to this CRTC
=======
 * @group: CRTC group this CRTC belongs to
 * @cmm: CMM associated with this CRTC
 * @vsp: VSP feeding video to this CRTC
 * @vsp_pipe: index of the VSP pipeline feeding video to this CRTC
 * @writeback: the writeback connector
>>>>>>> upstream/android-13
 */
struct rcar_du_crtc {
	struct drm_crtc crtc;

<<<<<<< HEAD
=======
	struct rcar_du_device *dev;
>>>>>>> upstream/android-13
	struct clk *clock;
	struct clk *extclock;
	unsigned int mmio_offset;
	unsigned int index;
	bool initialized;

<<<<<<< HEAD
=======
	u32 dsysr;

>>>>>>> upstream/android-13
	bool vblank_enable;
	struct drm_pending_vblank_event *event;
	wait_queue_head_t flip_wait;

	spinlock_t vblank_lock;
	wait_queue_head_t vblank_wait;
	unsigned int vblank_count;

<<<<<<< HEAD
	unsigned int outputs;

	struct rcar_du_group *group;
	struct rcar_du_vsp *vsp;
	unsigned int vsp_pipe;
};

#define to_rcar_crtc(c)	container_of(c, struct rcar_du_crtc, crtc)
=======
	struct rcar_du_group *group;
	struct platform_device *cmm;
	struct rcar_du_vsp *vsp;
	unsigned int vsp_pipe;

	const char *const *sources;
	unsigned int sources_count;

	struct drm_writeback_connector writeback;
};

#define to_rcar_crtc(c)		container_of(c, struct rcar_du_crtc, crtc)
#define wb_to_rcar_crtc(c)	container_of(c, struct rcar_du_crtc, writeback)
>>>>>>> upstream/android-13

/**
 * struct rcar_du_crtc_state - Driver-specific CRTC state
 * @state: base DRM CRTC state
 * @crc: CRC computation configuration
<<<<<<< HEAD
=======
 * @outputs: bitmask of the outputs (enum rcar_du_output) driven by this CRTC
>>>>>>> upstream/android-13
 */
struct rcar_du_crtc_state {
	struct drm_crtc_state state;

	struct vsp1_du_crc_config crc;
<<<<<<< HEAD
=======
	unsigned int outputs;
>>>>>>> upstream/android-13
};

#define to_rcar_crtc_state(s) container_of(s, struct rcar_du_crtc_state, state)

enum rcar_du_output {
	RCAR_DU_OUTPUT_DPAD0,
	RCAR_DU_OUTPUT_DPAD1,
	RCAR_DU_OUTPUT_LVDS0,
	RCAR_DU_OUTPUT_LVDS1,
	RCAR_DU_OUTPUT_HDMI0,
	RCAR_DU_OUTPUT_HDMI1,
	RCAR_DU_OUTPUT_TCON,
	RCAR_DU_OUTPUT_MAX,
};

int rcar_du_crtc_create(struct rcar_du_group *rgrp, unsigned int swindex,
			unsigned int hwindex);
<<<<<<< HEAD
void rcar_du_crtc_suspend(struct rcar_du_crtc *rcrtc);
void rcar_du_crtc_resume(struct rcar_du_crtc *rcrtc);

void rcar_du_crtc_route_output(struct drm_crtc *crtc,
			       enum rcar_du_output output);
void rcar_du_crtc_finish_page_flip(struct rcar_du_crtc *rcrtc);

=======

void rcar_du_crtc_finish_page_flip(struct rcar_du_crtc *rcrtc);

void rcar_du_crtc_dsysr_clr_set(struct rcar_du_crtc *rcrtc, u32 clr, u32 set);

>>>>>>> upstream/android-13
#endif /* __RCAR_DU_CRTC_H__ */
