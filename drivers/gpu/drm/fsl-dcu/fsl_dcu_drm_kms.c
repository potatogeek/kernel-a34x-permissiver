<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 *
 * Freescale DCU drm device driver
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drmP.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
=======
 */

#include <drm/drm_atomic_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_probe_helper.h>
>>>>>>> upstream/android-13

#include "fsl_dcu_drm_crtc.h"
#include "fsl_dcu_drm_drv.h"

static const struct drm_mode_config_funcs fsl_dcu_drm_mode_config_funcs = {
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
	.fb_create = drm_gem_fb_create,
};

int fsl_dcu_drm_modeset_init(struct fsl_dcu_drm_device *fsl_dev)
{
	int ret;

	drm_mode_config_init(fsl_dev->drm);

	fsl_dev->drm->mode_config.min_width = 0;
	fsl_dev->drm->mode_config.min_height = 0;
	fsl_dev->drm->mode_config.max_width = 2031;
	fsl_dev->drm->mode_config.max_height = 2047;
	fsl_dev->drm->mode_config.funcs = &fsl_dcu_drm_mode_config_funcs;

	ret = fsl_dcu_drm_crtc_create(fsl_dev);
	if (ret)
		goto err;

	ret = fsl_dcu_drm_encoder_create(fsl_dev, &fsl_dev->crtc);
	if (ret)
		goto err;

	ret = fsl_dcu_create_outputs(fsl_dev);
	if (ret)
		goto err;

	drm_mode_config_reset(fsl_dev->drm);
	drm_kms_helper_poll_init(fsl_dev->drm);

	return 0;

err:
	drm_mode_config_cleanup(fsl_dev->drm);
	return ret;
}
