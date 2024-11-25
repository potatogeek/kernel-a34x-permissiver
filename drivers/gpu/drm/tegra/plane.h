<<<<<<< HEAD
/*
 * Copyright (C) 2017 NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 NVIDIA CORPORATION.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef TEGRA_PLANE_H
#define TEGRA_PLANE_H 1

#include <drm/drm_plane.h>

<<<<<<< HEAD
=======
struct icc_path;
>>>>>>> upstream/android-13
struct tegra_bo;
struct tegra_dc;

struct tegra_plane {
	struct drm_plane base;
	struct tegra_dc *dc;
	unsigned int offset;
	unsigned int index;
<<<<<<< HEAD
=======

	struct icc_path *icc_mem;
	struct icc_path *icc_mem_vfilter;
>>>>>>> upstream/android-13
};

struct tegra_cursor {
	struct tegra_plane base;

	struct tegra_bo *bo;
	unsigned int width;
	unsigned int height;
};

static inline struct tegra_plane *to_tegra_plane(struct drm_plane *plane)
{
	return container_of(plane, struct tegra_plane, base);
}

struct tegra_plane_legacy_blending_state {
	bool alpha;
	bool top;
};

struct tegra_plane_state {
	struct drm_plane_state base;

<<<<<<< HEAD
=======
	struct sg_table *sgt[3];
	dma_addr_t iova[3];

>>>>>>> upstream/android-13
	struct tegra_bo_tiling tiling;
	u32 format;
	u32 swap;

<<<<<<< HEAD
	bool bottom_up;
=======
	bool reflect_x;
	bool reflect_y;
>>>>>>> upstream/android-13

	/* used for legacy blending support only */
	struct tegra_plane_legacy_blending_state blending[2];
	bool opaque;
<<<<<<< HEAD
=======

	/* bandwidths are in ICC units, i.e. kbytes/sec */
	u32 total_peak_memory_bandwidth;
	u32 peak_memory_bandwidth;
	u32 avg_memory_bandwidth;
>>>>>>> upstream/android-13
};

static inline struct tegra_plane_state *
to_tegra_plane_state(struct drm_plane_state *state)
{
	if (state)
		return container_of(state, struct tegra_plane_state, base);

	return NULL;
}

<<<<<<< HEAD
extern const struct drm_plane_funcs tegra_plane_funcs;

=======
static inline const struct tegra_plane_state *
to_const_tegra_plane_state(const struct drm_plane_state *state)
{
	return to_tegra_plane_state((struct drm_plane_state *)state);
}

extern const struct drm_plane_funcs tegra_plane_funcs;

int tegra_plane_prepare_fb(struct drm_plane *plane,
			   struct drm_plane_state *state);
void tegra_plane_cleanup_fb(struct drm_plane *plane,
			    struct drm_plane_state *state);

>>>>>>> upstream/android-13
int tegra_plane_state_add(struct tegra_plane *plane,
			  struct drm_plane_state *state);

int tegra_plane_format(u32 fourcc, u32 *format, u32 *swap);
<<<<<<< HEAD
bool tegra_plane_format_is_yuv(unsigned int format, bool *planar);
int tegra_plane_setup_legacy_state(struct tegra_plane *tegra,
				   struct tegra_plane_state *state);
=======
bool tegra_plane_format_is_indexed(unsigned int format);
bool tegra_plane_format_is_yuv(unsigned int format, bool *planar, unsigned int *bpc);
int tegra_plane_setup_legacy_state(struct tegra_plane *tegra,
				   struct tegra_plane_state *state);
int tegra_plane_interconnect_init(struct tegra_plane *plane);
>>>>>>> upstream/android-13

#endif /* TEGRA_PLANE_H */
