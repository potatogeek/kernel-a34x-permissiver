<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * rcar_du_encoder.h  --  R-Car Display Unit Encoder
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

#ifndef __RCAR_DU_ENCODER_H__
#define __RCAR_DU_ENCODER_H__

<<<<<<< HEAD
#include <drm/drm_crtc.h>
#include <drm/drm_encoder.h>

struct drm_panel;
=======
#include <drm/drm_encoder.h>

>>>>>>> upstream/android-13
struct rcar_du_device;

struct rcar_du_encoder {
	struct drm_encoder base;
	enum rcar_du_output output;
};

#define to_rcar_encoder(e) \
	container_of(e, struct rcar_du_encoder, base)

<<<<<<< HEAD
#define rcar_encoder_to_drm_encoder(e)	(&(e)->base)

int rcar_du_encoder_init(struct rcar_du_device *rcdu,
			 enum rcar_du_output output,
			 struct device_node *enc_node,
			 struct device_node *con_node);
=======
int rcar_du_encoder_init(struct rcar_du_device *rcdu,
			 enum rcar_du_output output,
			 struct device_node *enc_node);
>>>>>>> upstream/android-13

#endif /* __RCAR_DU_ENCODER_H__ */
