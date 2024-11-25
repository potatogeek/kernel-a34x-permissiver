<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * Freescale i.MX drm driver
 *
 * Copyright (C) 2011 Sascha Hauer, Pengutronix
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
=======
 */

>>>>>>> upstream/android-13
#include <linux/component.h>
#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_of.h>
#include <video/imx-ipu-v3.h>
=======

#include <video/imx-ipu-v3.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_of.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "imx-drm.h"
#include "ipuv3-plane.h"

#define MAX_CRTC	4

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_DRM_FBDEV_EMULATION)
static int legacyfb_depth = 16;
module_param(legacyfb_depth, int, 0444);
#endif
=======
static int legacyfb_depth = 16;
module_param(legacyfb_depth, int, 0444);
>>>>>>> upstream/android-13

DEFINE_DRM_GEM_CMA_FOPS(imx_drm_driver_fops);

void imx_drm_connector_destroy(struct drm_connector *connector)
{
	drm_connector_unregister(connector);
	drm_connector_cleanup(connector);
}
EXPORT_SYMBOL_GPL(imx_drm_connector_destroy);

<<<<<<< HEAD
void imx_drm_encoder_destroy(struct drm_encoder *encoder)
{
	drm_encoder_cleanup(encoder);
}
EXPORT_SYMBOL_GPL(imx_drm_encoder_destroy);

=======
>>>>>>> upstream/android-13
static int imx_drm_atomic_check(struct drm_device *dev,
				struct drm_atomic_state *state)
{
	int ret;

<<<<<<< HEAD
	ret = drm_atomic_helper_check_modeset(dev, state);
	if (ret)
		return ret;

	ret = drm_atomic_helper_check_planes(dev, state);
=======
	ret = drm_atomic_helper_check(dev, state);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	/*
	 * Check modeset again in case crtc_state->mode_changed is
	 * updated in plane's ->atomic_check callback.
	 */
	ret = drm_atomic_helper_check_modeset(dev, state);
	if (ret)
		return ret;

	/* Assign PRG/PRE channels and check if all constrains are satisfied. */
	ret = ipu_planes_assign_pre(dev, state);
	if (ret)
		return ret;

	return ret;
}

static const struct drm_mode_config_funcs imx_drm_mode_config_funcs = {
	.fb_create = drm_gem_fb_create,
<<<<<<< HEAD
	.output_poll_changed = drm_fb_helper_output_poll_changed,
=======
>>>>>>> upstream/android-13
	.atomic_check = imx_drm_atomic_check,
	.atomic_commit = drm_atomic_helper_commit,
};

static void imx_drm_atomic_commit_tail(struct drm_atomic_state *state)
{
	struct drm_device *dev = state->dev;
	struct drm_plane *plane;
	struct drm_plane_state *old_plane_state, *new_plane_state;
	bool plane_disabling = false;
	int i;

	drm_atomic_helper_commit_modeset_disables(dev, state);

	drm_atomic_helper_commit_planes(dev, state,
				DRM_PLANE_COMMIT_ACTIVE_ONLY |
				DRM_PLANE_COMMIT_NO_DISABLE_AFTER_MODESET);

	drm_atomic_helper_commit_modeset_enables(dev, state);

	for_each_oldnew_plane_in_state(state, plane, old_plane_state, new_plane_state, i) {
		if (drm_atomic_plane_disabling(old_plane_state, new_plane_state))
			plane_disabling = true;
	}

	/*
	 * The flip done wait is only strictly required by imx-drm if a deferred
	 * plane disable is in-flight. As the core requires blocking commits
	 * to wait for the flip it is done here unconditionally. This keeps the
	 * workitem around a bit longer than required for the majority of
	 * non-blocking commits, but we accept that for the sake of simplicity.
	 */
	drm_atomic_helper_wait_for_flip_done(dev, state);

	if (plane_disabling) {
		for_each_old_plane_in_state(state, plane, old_plane_state, i)
			ipu_plane_disable_deferred(plane);

	}

	drm_atomic_helper_commit_hw_done(state);
}

static const struct drm_mode_config_helper_funcs imx_drm_mode_config_helpers = {
	.atomic_commit_tail = imx_drm_atomic_commit_tail,
};


int imx_drm_encoder_parse_of(struct drm_device *drm,
	struct drm_encoder *encoder, struct device_node *np)
{
	uint32_t crtc_mask = drm_of_find_possible_crtcs(drm, np);

	/*
	 * If we failed to find the CRTC(s) which this encoder is
	 * supposed to be connected to, it's because the CRTC has
	 * not been registered yet.  Defer probing, and hope that
	 * the required CRTC is added later.
	 */
	if (crtc_mask == 0)
		return -EPROBE_DEFER;

	encoder->possible_crtcs = crtc_mask;

<<<<<<< HEAD
	/* FIXME: this is the mask of outputs which can clone this output. */
	encoder->possible_clones = ~0;
=======
	/* FIXME: cloning support not clear, disable it all for now */
	encoder->possible_clones = 0;
>>>>>>> upstream/android-13

	return 0;
}
EXPORT_SYMBOL_GPL(imx_drm_encoder_parse_of);

static const struct drm_ioctl_desc imx_drm_ioctls[] = {
	/* none so far */
};

<<<<<<< HEAD
static struct drm_driver imx_drm_driver = {
	.driver_features	= DRIVER_MODESET | DRIVER_GEM | DRIVER_PRIME |
				  DRIVER_ATOMIC,
	.lastclose		= drm_fb_helper_lastclose,
	.gem_free_object_unlocked = drm_gem_cma_free_object,
	.gem_vm_ops		= &drm_gem_cma_vm_ops,
	.dumb_create		= drm_gem_cma_dumb_create,

	.prime_handle_to_fd	= drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle	= drm_gem_prime_fd_to_handle,
	.gem_prime_import	= drm_gem_prime_import,
	.gem_prime_export	= drm_gem_prime_export,
	.gem_prime_get_sg_table	= drm_gem_cma_prime_get_sg_table,
	.gem_prime_import_sg_table = drm_gem_cma_prime_import_sg_table,
	.gem_prime_vmap		= drm_gem_cma_prime_vmap,
	.gem_prime_vunmap	= drm_gem_cma_prime_vunmap,
	.gem_prime_mmap		= drm_gem_cma_prime_mmap,
=======
static int imx_drm_dumb_create(struct drm_file *file_priv,
			       struct drm_device *drm,
			       struct drm_mode_create_dumb *args)
{
	u32 width = args->width;
	int ret;

	args->width = ALIGN(width, 8);

	ret = drm_gem_cma_dumb_create(file_priv, drm, args);
	if (ret)
		return ret;

	args->width = width;
	return ret;
}

static const struct drm_driver imx_drm_driver = {
	.driver_features	= DRIVER_MODESET | DRIVER_GEM | DRIVER_ATOMIC,
	DRM_GEM_CMA_DRIVER_OPS_WITH_DUMB_CREATE(imx_drm_dumb_create),
>>>>>>> upstream/android-13
	.ioctls			= imx_drm_ioctls,
	.num_ioctls		= ARRAY_SIZE(imx_drm_ioctls),
	.fops			= &imx_drm_driver_fops,
	.name			= "imx-drm",
	.desc			= "i.MX DRM graphics",
	.date			= "20120507",
	.major			= 1,
	.minor			= 0,
	.patchlevel		= 0,
};

static int compare_of(struct device *dev, void *data)
{
	struct device_node *np = data;

	/* Special case for DI, dev->of_node may not be set yet */
	if (strcmp(dev->driver->name, "imx-ipuv3-crtc") == 0) {
		struct ipu_client_platformdata *pdata = dev->platform_data;

		return pdata->of_node == np;
	}

	/* Special case for LDB, one device for two channels */
<<<<<<< HEAD
	if (of_node_cmp(np->name, "lvds-channel") == 0) {
=======
	if (of_node_name_eq(np, "lvds-channel")) {
>>>>>>> upstream/android-13
		np = of_get_parent(np);
		of_node_put(np);
	}

	return dev->of_node == np;
}

static int imx_drm_bind(struct device *dev)
{
	struct drm_device *drm;
	int ret;

	drm = drm_dev_alloc(&imx_drm_driver, dev);
	if (IS_ERR(drm))
		return PTR_ERR(drm);

	/*
<<<<<<< HEAD
	 * enable drm irq mode.
	 * - with irq_enabled = true, we can use the vblank feature.
	 *
	 * P.S. note that we wouldn't use drm irq handler but
	 *      just specific driver own one instead because
	 *      drm framework supports only one irq handler and
	 *      drivers can well take care of their interrupts
	 */
	drm->irq_enabled = true;

	/*
=======
>>>>>>> upstream/android-13
	 * set max width and height as default value(4096x4096).
	 * this value would be used to check framebuffer size limitation
	 * at drm_mode_addfb().
	 */
	drm->mode_config.min_width = 1;
	drm->mode_config.min_height = 1;
	drm->mode_config.max_width = 4096;
	drm->mode_config.max_height = 4096;
	drm->mode_config.funcs = &imx_drm_mode_config_funcs;
	drm->mode_config.helper_private = &imx_drm_mode_config_helpers;
<<<<<<< HEAD
	drm->mode_config.allow_fb_modifiers = true;

	drm_mode_config_init(drm);
=======
	drm->mode_config.normalize_zpos = true;

	ret = drmm_mode_config_init(drm);
	if (ret)
		goto err_kms;
>>>>>>> upstream/android-13

	ret = drm_vblank_init(drm, MAX_CRTC);
	if (ret)
		goto err_kms;

	dev_set_drvdata(dev, drm);

	/* Now try and bind all our sub-components */
	ret = component_bind_all(dev, drm);
	if (ret)
		goto err_kms;

	drm_mode_config_reset(drm);

	/*
	 * All components are now initialised, so setup the fb helper.
	 * The fb helper takes copies of key hardware information, so the
	 * crtcs/connectors/encoders must not change after this point.
	 */
<<<<<<< HEAD
#if IS_ENABLED(CONFIG_DRM_FBDEV_EMULATION)
=======
>>>>>>> upstream/android-13
	if (legacyfb_depth != 16 && legacyfb_depth != 32) {
		dev_warn(dev, "Invalid legacyfb_depth.  Defaulting to 16bpp\n");
		legacyfb_depth = 16;
	}
<<<<<<< HEAD
	ret = drm_fb_cma_fbdev_init(drm, legacyfb_depth, MAX_CRTC);
	if (ret)
		goto err_unbind;
#endif
=======
>>>>>>> upstream/android-13

	drm_kms_helper_poll_init(drm);

	ret = drm_dev_register(drm, 0);
	if (ret)
<<<<<<< HEAD
		goto err_fbhelper;

	return 0;

err_fbhelper:
	drm_kms_helper_poll_fini(drm);
#if IS_ENABLED(CONFIG_DRM_FBDEV_EMULATION)
	drm_fb_cma_fbdev_fini(drm);
err_unbind:
#endif
	component_unbind_all(drm->dev, drm);
err_kms:
	drm_mode_config_cleanup(drm);
=======
		goto err_poll_fini;

	drm_fbdev_generic_setup(drm, legacyfb_depth);

	return 0;

err_poll_fini:
	drm_kms_helper_poll_fini(drm);
	component_unbind_all(drm->dev, drm);
err_kms:
>>>>>>> upstream/android-13
	drm_dev_put(drm);

	return ret;
}

static void imx_drm_unbind(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);

	drm_dev_unregister(drm);

	drm_kms_helper_poll_fini(drm);

<<<<<<< HEAD
	drm_fb_cma_fbdev_fini(drm);

	drm_mode_config_cleanup(drm);

	component_unbind_all(drm->dev, drm);
	dev_set_drvdata(dev, NULL);

	drm_dev_put(drm);
=======
	component_unbind_all(drm->dev, drm);

	drm_dev_put(drm);

	dev_set_drvdata(dev, NULL);
>>>>>>> upstream/android-13
}

static const struct component_master_ops imx_drm_ops = {
	.bind = imx_drm_bind,
	.unbind = imx_drm_unbind,
};

static int imx_drm_platform_probe(struct platform_device *pdev)
{
	int ret = drm_of_component_probe(&pdev->dev, compare_of, &imx_drm_ops);

	if (!ret)
		ret = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32));

	return ret;
}

static int imx_drm_platform_remove(struct platform_device *pdev)
{
	component_master_del(&pdev->dev, &imx_drm_ops);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int imx_drm_suspend(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	return drm_mode_config_helper_suspend(drm_dev);
}

static int imx_drm_resume(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	return drm_mode_config_helper_resume(drm_dev);
}
#endif

static SIMPLE_DEV_PM_OPS(imx_drm_pm_ops, imx_drm_suspend, imx_drm_resume);

static const struct of_device_id imx_drm_dt_ids[] = {
	{ .compatible = "fsl,imx-display-subsystem", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, imx_drm_dt_ids);

static struct platform_driver imx_drm_pdrv = {
	.probe		= imx_drm_platform_probe,
	.remove		= imx_drm_platform_remove,
	.driver		= {
		.name	= "imx-drm",
		.pm	= &imx_drm_pm_ops,
		.of_match_table = imx_drm_dt_ids,
	},
};

static struct platform_driver * const drivers[] = {
	&imx_drm_pdrv,
	&ipu_drm_driver,
};

static int __init imx_drm_init(void)
{
	return platform_register_drivers(drivers, ARRAY_SIZE(drivers));
}
module_init(imx_drm_init);

static void __exit imx_drm_exit(void)
{
	platform_unregister_drivers(drivers, ARRAY_SIZE(drivers));
}
module_exit(imx_drm_exit);

MODULE_AUTHOR("Sascha Hauer <s.hauer@pengutronix.de>");
MODULE_DESCRIPTION("i.MX drm driver core");
MODULE_LICENSE("GPL");
