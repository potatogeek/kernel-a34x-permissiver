<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2016 Marek Vasut <marex@denx.de>
 *
 * i.MX23/i.MX28/i.MX6SX MXSFB LCD controller driver.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __MXSFB_DRV_H__
#define __MXSFB_DRV_H__

<<<<<<< HEAD
struct mxsfb_devdata {
	unsigned int	 transfer_count;
	unsigned int	 cur_buf;
	unsigned int	 next_buf;
	unsigned int	 debug0;
	unsigned int	 hs_wdth_mask;
	unsigned int	 hs_wdth_shift;
	unsigned int	 ipversion;
=======
#include <drm/drm_crtc.h>
#include <drm/drm_device.h>
#include <drm/drm_encoder.h>
#include <drm/drm_plane.h>

struct clk;

struct mxsfb_devdata {
	unsigned int	transfer_count;
	unsigned int	cur_buf;
	unsigned int	next_buf;
	unsigned int	hs_wdth_mask;
	unsigned int	hs_wdth_shift;
	bool		has_overlay;
	bool		has_ctrl2;
>>>>>>> upstream/android-13
};

struct mxsfb_drm_private {
	const struct mxsfb_devdata	*devdata;

	void __iomem			*base;	/* registers */
	struct clk			*clk;
	struct clk			*clk_axi;
	struct clk			*clk_disp_axi;

<<<<<<< HEAD
	struct drm_simple_display_pipe	pipe;
	struct drm_connector		connector;
	struct drm_panel		*panel;
	struct drm_fbdev_cma		*fbdev;
};

int mxsfb_setup_crtc(struct drm_device *dev);
int mxsfb_create_output(struct drm_device *dev);
=======
	unsigned int			irq;

	struct drm_device		*drm;
	struct {
		struct drm_plane	primary;
		struct drm_plane	overlay;
	} planes;
	struct drm_crtc			crtc;
	struct drm_encoder		encoder;
	struct drm_connector		*connector;
	struct drm_bridge		*bridge;
};

static inline struct mxsfb_drm_private *
to_mxsfb_drm_private(struct drm_device *drm)
{
	return drm->dev_private;
}
>>>>>>> upstream/android-13

void mxsfb_enable_axi_clk(struct mxsfb_drm_private *mxsfb);
void mxsfb_disable_axi_clk(struct mxsfb_drm_private *mxsfb);

<<<<<<< HEAD
void mxsfb_crtc_enable(struct mxsfb_drm_private *mxsfb);
void mxsfb_crtc_disable(struct mxsfb_drm_private *mxsfb);
void mxsfb_plane_atomic_update(struct mxsfb_drm_private *mxsfb,
			       struct drm_plane_state *state);
=======
int mxsfb_kms_init(struct mxsfb_drm_private *mxsfb);
>>>>>>> upstream/android-13

#endif /* __MXSFB_DRV_H__ */
