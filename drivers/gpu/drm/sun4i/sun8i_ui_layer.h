<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (C) Icenowy Zheng <icenowy@aosc.io>
 *
 * Based on sun4i_layer.h, which is:
 *   Copyright (C) 2015 Free Electrons
 *   Copyright (C) 2015 NextThing Co
 *
 *   Maxime Ripard <maxime.ripard@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _SUN8I_UI_LAYER_H_
#define _SUN8I_UI_LAYER_H_

#include <drm/drm_plane.h>

<<<<<<< HEAD
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0x0)
#define SUN8I_MIXER_CHAN_UI_LAYER_SIZE(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0x4)
#define SUN8I_MIXER_CHAN_UI_LAYER_COORD(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0x8)
#define SUN8I_MIXER_CHAN_UI_LAYER_PITCH(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0xc)
#define SUN8I_MIXER_CHAN_UI_LAYER_TOP_LADDR(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0x10)
#define SUN8I_MIXER_CHAN_UI_LAYER_BOT_LADDR(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0x14)
#define SUN8I_MIXER_CHAN_UI_LAYER_FCOLOR(ch, layer) \
			(0x2000 + 0x1000 * (ch) + 0x20 * (layer) + 0x18)
#define SUN8I_MIXER_CHAN_UI_TOP_HADDR(ch)	(0x2000 + 0x1000 * (ch) + 0x80)
#define SUN8I_MIXER_CHAN_UI_BOT_HADDR(ch)	(0x2000 + 0x1000 * (ch) + 0x84)
#define SUN8I_MIXER_CHAN_UI_OVL_SIZE(ch)	(0x2000 + 0x1000 * (ch) + 0x88)
=======
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR(base, layer) \
			((base) + 0x20 * (layer) + 0x0)
#define SUN8I_MIXER_CHAN_UI_LAYER_SIZE(base, layer) \
			((base) + 0x20 * (layer) + 0x4)
#define SUN8I_MIXER_CHAN_UI_LAYER_COORD(base, layer) \
			((base) + 0x20 * (layer) + 0x8)
#define SUN8I_MIXER_CHAN_UI_LAYER_PITCH(base, layer) \
			((base) + 0x20 * (layer) + 0xc)
#define SUN8I_MIXER_CHAN_UI_LAYER_TOP_LADDR(base, layer) \
			((base) + 0x20 * (layer) + 0x10)
#define SUN8I_MIXER_CHAN_UI_LAYER_BOT_LADDR(base, layer) \
			((base) + 0x20 * (layer) + 0x14)
#define SUN8I_MIXER_CHAN_UI_LAYER_FCOLOR(base, layer) \
			((base) + 0x20 * (layer) + 0x18)
#define SUN8I_MIXER_CHAN_UI_TOP_HADDR(base) \
			((base) + 0x80)
#define SUN8I_MIXER_CHAN_UI_BOT_HADDR(base) \
			((base) + 0x84)
#define SUN8I_MIXER_CHAN_UI_OVL_SIZE(base) \
			((base) + 0x88)
>>>>>>> upstream/android-13

#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_EN		BIT(0)
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_MASK	GENMASK(2, 1)
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_FBFMT_MASK	GENMASK(12, 8)
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_FBFMT_OFFSET	8
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MASK	GENMASK(31, 24)
<<<<<<< HEAD
=======
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA(x)		((x) << 24)

#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_PIXEL		((0) << 1)
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_LAYER		((1) << 1)
#define SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_COMBINED	((2) << 1)
>>>>>>> upstream/android-13

struct sun8i_mixer;

struct sun8i_ui_layer {
	struct drm_plane	plane;
	struct sun8i_mixer	*mixer;
	int			channel;
	int			overlay;
};

static inline struct sun8i_ui_layer *
plane_to_sun8i_ui_layer(struct drm_plane *plane)
{
	return container_of(plane, struct sun8i_ui_layer, plane);
}

struct sun8i_ui_layer *sun8i_ui_layer_init_one(struct drm_device *drm,
					       struct sun8i_mixer *mixer,
					       int index);
#endif /* _SUN8I_UI_LAYER_H_ */
