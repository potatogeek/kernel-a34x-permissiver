<<<<<<< HEAD
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 * Author: Rob Clark <rob@ti.com>
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - https://www.ti.com/
 * Author: Rob Clark <rob@ti.com>
>>>>>>> upstream/android-13
 */

#ifndef __OMAPDRM_DRV_H__
#define __OMAPDRM_DRV_H__

#include <linux/module.h>
#include <linux/types.h>
#include <linux/workqueue.h>

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_gem.h>
#include <drm/omap_drm.h>

#include "dss/omapdss.h"

#include "omap_connector.h"
=======
#include "dss/omapdss.h"
#include "dss/dss.h"

#include <drm/drm_gem.h>
#include <drm/omap_drm.h>

>>>>>>> upstream/android-13
#include "omap_crtc.h"
#include "omap_encoder.h"
#include "omap_fb.h"
#include "omap_fbdev.h"
#include "omap_gem.h"
#include "omap_irq.h"
#include "omap_plane.h"

<<<<<<< HEAD
#define DBG(fmt, ...) DRM_DEBUG(fmt"\n", ##__VA_ARGS__)
#define VERB(fmt, ...) if (0) DRM_DEBUG(fmt, ##__VA_ARGS__) /* verbose debug */
=======
#define DBG(fmt, ...) DRM_DEBUG_DRIVER(fmt"\n", ##__VA_ARGS__)
#define VERB(fmt, ...) if (0) DRM_DEBUG_DRIVER(fmt, ##__VA_ARGS__) /* verbose debug */
>>>>>>> upstream/android-13

#define MODULE_NAME     "omapdrm"

struct omap_drm_usergart;

<<<<<<< HEAD
=======
struct omap_drm_pipeline {
	struct drm_crtc *crtc;
	struct drm_encoder *encoder;
	struct drm_connector *connector;
	struct omap_dss_device *output;
	unsigned int alias_id;
};

>>>>>>> upstream/android-13
struct omap_drm_private {
	struct drm_device *ddev;
	struct device *dev;
	u32 omaprev;

	struct dss_device *dss;
	struct dispc_device *dispc;
<<<<<<< HEAD
	const struct dispc_ops *dispc_ops;

	unsigned int num_crtcs;
	struct drm_crtc *crtcs[8];
=======

	bool irq_enabled;

	unsigned int num_pipes;
	struct omap_drm_pipeline pipes[8];
	struct omap_drm_pipeline *channels[8];
>>>>>>> upstream/android-13

	unsigned int num_planes;
	struct drm_plane *planes[8];

<<<<<<< HEAD
	unsigned int num_encoders;
	struct drm_encoder *encoders[8];

	unsigned int num_connectors;
	struct drm_connector *connectors[8];

=======
>>>>>>> upstream/android-13
	struct drm_fb_helper *fbdev;

	struct workqueue_struct *wq;

	/* lock for obj_list below */
	struct mutex list_lock;

	/* list of GEM objects: */
	struct list_head obj_list;

	struct omap_drm_usergart *usergart;
	bool has_dmm;

	/* properties: */
	struct drm_property *zorder_prop;

	/* irq handling: */
	spinlock_t wait_lock;		/* protects the wait_list */
	struct list_head wait_list;	/* list of omap_irq_wait */
	u32 irq_mask;			/* enabled irqs in addition to wait_list */

	/* memory bandwidth limit if it is needed on the platform */
	unsigned int max_bandwidth;
};


<<<<<<< HEAD
int omap_debugfs_init(struct drm_minor *minor);
=======
void omap_debugfs_init(struct drm_minor *minor);
>>>>>>> upstream/android-13

#endif /* __OMAPDRM_DRV_H__ */
