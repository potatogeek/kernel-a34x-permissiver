<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * omap_crtc.h -- OMAP DRM CRTC
 *
 * Copyright (C) 2011 Texas Instruments
 * Author: Rob Clark <rob@ti.com>
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

#ifndef __OMAPDRM_CRTC_H__
#define __OMAPDRM_CRTC_H__

#include <linux/types.h>

enum omap_channel;

struct drm_crtc;
struct drm_device;
struct drm_plane;
<<<<<<< HEAD
=======
struct omap_drm_pipeline;
>>>>>>> upstream/android-13
struct omap_dss_device;
struct videomode;

struct videomode *omap_crtc_timings(struct drm_crtc *crtc);
enum omap_channel omap_crtc_channel(struct drm_crtc *crtc);
<<<<<<< HEAD
void omap_crtc_pre_init(struct omap_drm_private *priv);
void omap_crtc_pre_uninit(void);
struct drm_crtc *omap_crtc_init(struct drm_device *dev,
		struct drm_plane *plane, struct omap_dss_device *dssdev);
int omap_crtc_wait_pending(struct drm_crtc *crtc);
void omap_crtc_error_irq(struct drm_crtc *crtc, u32 irqstatus);
void omap_crtc_vblank_irq(struct drm_crtc *crtc);
=======
struct drm_crtc *omap_crtc_init(struct drm_device *dev,
				struct omap_drm_pipeline *pipe,
				struct drm_plane *plane);
int omap_crtc_wait_pending(struct drm_crtc *crtc);
void omap_crtc_error_irq(struct drm_crtc *crtc, u32 irqstatus);
void omap_crtc_vblank_irq(struct drm_crtc *crtc);
void omap_crtc_framedone_irq(struct drm_crtc *crtc, uint32_t irqstatus);
void omap_crtc_flush(struct drm_crtc *crtc);
>>>>>>> upstream/android-13

#endif /* __OMAPDRM_CRTC_H__ */
