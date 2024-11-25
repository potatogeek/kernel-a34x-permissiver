<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * shmob_drm_plane.h  --  SH Mobile DRM Planes
 *
 * Copyright (C) 2012 Renesas Electronics Corporation
 *
 * Laurent Pinchart (laurent.pinchart@ideasonboard.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __SHMOB_DRM_PLANE_H__
#define __SHMOB_DRM_PLANE_H__

<<<<<<< HEAD
=======
struct drm_plane;
>>>>>>> upstream/android-13
struct shmob_drm_device;

int shmob_drm_plane_create(struct shmob_drm_device *sdev, unsigned int index);
void shmob_drm_plane_setup(struct drm_plane *plane);

#endif /* __SHMOB_DRM_PLANE_H__ */
