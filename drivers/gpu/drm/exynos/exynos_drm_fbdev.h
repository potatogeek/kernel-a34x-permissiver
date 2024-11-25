<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *
 * Authors:
 *	Inki Dae <inki.dae@samsung.com>
 *	Joonyoung Shim <jy0922.shim@samsung.com>
 *	Seung-Woo Kim <sw0312.kim@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _EXYNOS_DRM_FBDEV_H_
#define _EXYNOS_DRM_FBDEV_H_

#ifdef CONFIG_DRM_FBDEV_EMULATION

int exynos_drm_fbdev_init(struct drm_device *dev);
void exynos_drm_fbdev_fini(struct drm_device *dev);
<<<<<<< HEAD
void exynos_drm_fbdev_suspend(struct drm_device *drm);
void exynos_drm_fbdev_resume(struct drm_device *drm);
=======
>>>>>>> upstream/android-13

#else

static inline int exynos_drm_fbdev_init(struct drm_device *dev)
{
	return 0;
}

static inline void exynos_drm_fbdev_fini(struct drm_device *dev)
{
}

static inline void exynos_drm_fbdev_restore_mode(struct drm_device *dev)
{
}

#define exynos_drm_output_poll_changed (NULL)

<<<<<<< HEAD
static inline void exynos_drm_fbdev_suspend(struct drm_device *drm)
{
}

static inline void exynos_drm_fbdev_resume(struct drm_device *drm)
{
}

=======
>>>>>>> upstream/android-13
#endif

#endif
