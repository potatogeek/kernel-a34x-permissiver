<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/**************************************************************************
 * Copyright (c) 2014 Patrik Jakobsson
 * All Rights Reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
=======
>>>>>>> upstream/android-13
 **************************************************************************/

#ifndef _GEM_H
#define _GEM_H

<<<<<<< HEAD
extern int psb_gem_create(struct drm_file *file, struct drm_device *dev,
			  u64 size, u32 *handlep, int stolen, u32 align);
=======
struct drm_device;

extern const struct drm_gem_object_funcs psb_gem_object_funcs;

extern int psb_gem_create(struct drm_file *file, struct drm_device *dev,
			  u64 size, u32 *handlep, int stolen, u32 align);

>>>>>>> upstream/android-13
#endif
