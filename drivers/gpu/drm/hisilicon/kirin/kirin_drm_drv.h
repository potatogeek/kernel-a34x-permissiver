<<<<<<< HEAD
/*
 * Copyright (c) 2016 Linaro Limited.
 * Copyright (c) 2014-2016 Hisilicon Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2016 Linaro Limited.
 * Copyright (c) 2014-2016 HiSilicon Limited.
>>>>>>> upstream/android-13
 */

#ifndef __KIRIN_DRM_DRV_H__
#define __KIRIN_DRM_DRV_H__

<<<<<<< HEAD
#define MAX_CRTC	2

/* display controller init/cleanup ops */
struct kirin_dc_ops {
	int (*init)(struct platform_device *pdev);
	void (*cleanup)(struct platform_device *pdev);
};

struct kirin_drm_private {
	struct drm_fbdev_cma *fbdev;
};

extern const struct kirin_dc_ops ade_dc_ops;
=======
#define to_kirin_crtc(crtc) \
	container_of(crtc, struct kirin_crtc, base)

#define to_kirin_plane(plane) \
	container_of(plane, struct kirin_plane, base)

/* kirin-format translate table */
struct kirin_format {
	u32 pixel_format;
	u32 hw_format;
};

struct kirin_crtc {
	struct drm_crtc base;
	void *hw_ctx;
	bool enable;
};

struct kirin_plane {
	struct drm_plane base;
	void *hw_ctx;
	u32 ch;
};

/* display controller init/cleanup ops */
struct kirin_drm_data {
	const u32 *channel_formats;
	u32 channel_formats_cnt;
	int config_max_width;
	int config_max_height;
	u32 num_planes;
	u32 prim_plane;

	const struct drm_driver *driver;
	const struct drm_crtc_helper_funcs *crtc_helper_funcs;
	const struct drm_crtc_funcs *crtc_funcs;
	const struct drm_plane_helper_funcs *plane_helper_funcs;
	const struct drm_plane_funcs  *plane_funcs;
	const struct drm_mode_config_funcs *mode_config_funcs;

	void *(*alloc_hw_ctx)(struct platform_device *pdev,
			      struct drm_crtc *crtc);
	void (*cleanup_hw_ctx)(void *hw_ctx);
};

extern struct kirin_drm_data ade_driver_data;
>>>>>>> upstream/android-13

#endif /* __KIRIN_DRM_DRV_H__ */
