<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * rcar_du_kms.h  --  R-Car Display Unit Mode Setting
 *
 * Copyright (C) 2013-2014 Renesas Electronics Corporation
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

#ifndef __RCAR_DU_KMS_H__
#define __RCAR_DU_KMS_H__

#include <linux/types.h>

struct drm_file;
struct drm_device;
struct drm_mode_create_dumb;
struct rcar_du_device;

struct rcar_du_format_info {
	u32 fourcc;
<<<<<<< HEAD
	unsigned int bpp;
	unsigned int planes;
=======
	u32 v4l2;
	unsigned int bpp;
	unsigned int planes;
	unsigned int hsub;
>>>>>>> upstream/android-13
	unsigned int pnmr;
	unsigned int edf;
};

const struct rcar_du_format_info *rcar_du_format_info(u32 fourcc);

int rcar_du_modeset_init(struct rcar_du_device *rcdu);

int rcar_du_dumb_create(struct drm_file *file, struct drm_device *dev,
			struct drm_mode_create_dumb *args);

#endif /* __RCAR_DU_KMS_H__ */
