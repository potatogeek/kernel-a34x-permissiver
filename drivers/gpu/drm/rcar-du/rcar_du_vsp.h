<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * rcar_du_vsp.h  --  R-Car Display Unit VSP-Based Compositor
 *
 * Copyright (C) 2015 Renesas Electronics Corporation
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

#ifndef __RCAR_DU_VSP_H__
#define __RCAR_DU_VSP_H__

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_crtc.h>

struct rcar_du_format_info;
struct rcar_du_vsp;
=======
#include <drm/drm_plane.h>

struct drm_framebuffer;
struct rcar_du_format_info;
struct rcar_du_vsp;
struct sg_table;
>>>>>>> upstream/android-13

struct rcar_du_vsp_plane {
	struct drm_plane plane;
	struct rcar_du_vsp *vsp;
	unsigned int index;
};

struct rcar_du_vsp {
	unsigned int index;
	struct device *vsp;
	struct rcar_du_device *dev;
	struct rcar_du_vsp_plane *planes;
	unsigned int num_planes;
};

static inline struct rcar_du_vsp_plane *to_rcar_vsp_plane(struct drm_plane *p)
{
	return container_of(p, struct rcar_du_vsp_plane, plane);
}

/**
 * struct rcar_du_vsp_plane_state - Driver-specific plane state
 * @state: base DRM plane state
 * @format: information about the pixel format used by the plane
 * @sg_tables: scatter-gather tables for the frame buffer memory
 */
struct rcar_du_vsp_plane_state {
	struct drm_plane_state state;

	const struct rcar_du_format_info *format;
	struct sg_table sg_tables[3];
};

static inline struct rcar_du_vsp_plane_state *
to_rcar_vsp_plane_state(struct drm_plane_state *state)
{
	return container_of(state, struct rcar_du_vsp_plane_state, state);
}

#ifdef CONFIG_DRM_RCAR_VSP
int rcar_du_vsp_init(struct rcar_du_vsp *vsp, struct device_node *np,
		     unsigned int crtcs);
void rcar_du_vsp_enable(struct rcar_du_crtc *crtc);
void rcar_du_vsp_disable(struct rcar_du_crtc *crtc);
void rcar_du_vsp_atomic_begin(struct rcar_du_crtc *crtc);
void rcar_du_vsp_atomic_flush(struct rcar_du_crtc *crtc);
<<<<<<< HEAD
=======
int rcar_du_vsp_map_fb(struct rcar_du_vsp *vsp, struct drm_framebuffer *fb,
		       struct sg_table sg_tables[3]);
void rcar_du_vsp_unmap_fb(struct rcar_du_vsp *vsp, struct drm_framebuffer *fb,
			  struct sg_table sg_tables[3]);
>>>>>>> upstream/android-13
#else
static inline int rcar_du_vsp_init(struct rcar_du_vsp *vsp,
				   struct device_node *np,
				   unsigned int crtcs)
{
	return -ENXIO;
}
static inline void rcar_du_vsp_enable(struct rcar_du_crtc *crtc) { };
static inline void rcar_du_vsp_disable(struct rcar_du_crtc *crtc) { };
static inline void rcar_du_vsp_atomic_begin(struct rcar_du_crtc *crtc) { };
static inline void rcar_du_vsp_atomic_flush(struct rcar_du_crtc *crtc) { };
<<<<<<< HEAD
=======
static inline int rcar_du_vsp_map_fb(struct rcar_du_vsp *vsp,
				     struct drm_framebuffer *fb,
				     struct sg_table sg_tables[3])
{
	return -ENXIO;
}
static inline void rcar_du_vsp_unmap_fb(struct rcar_du_vsp *vsp,
					struct drm_framebuffer *fb,
					struct sg_table sg_tables[3])
{
}
>>>>>>> upstream/android-13
#endif

#endif /* __RCAR_DU_VSP_H__ */
