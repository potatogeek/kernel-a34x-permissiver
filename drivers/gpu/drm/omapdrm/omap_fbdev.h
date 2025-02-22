<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * omap_fbdev.h -- OMAP DRM FBDEV Compatibility
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

#ifndef __OMAPDRM_FBDEV_H__
#define __OMAPDRM_FBDEV_H__

struct drm_device;
struct drm_fb_helper;

#ifdef CONFIG_DRM_FBDEV_EMULATION
void omap_fbdev_init(struct drm_device *dev);
void omap_fbdev_fini(struct drm_device *dev);
#else
static inline void omap_fbdev_init(struct drm_device *dev)
{
}
static inline void omap_fbdev_fini(struct drm_device *dev)
{
}
#endif

#endif /* __OMAPDRM_FBDEV_H__ */
