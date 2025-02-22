<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2017 Linus Walleij <linus.walleij@linaro.org>
 * Parts of this file were based on sources as follows:
 *
 * Copyright (C) 2006-2008 Intel Corporation
 * Copyright (C) 2007 Amos Lee <amos_lee@storlinksemi.com>
 * Copyright (C) 2007 Dave Airlie <airlied@linux.ie>
 * Copyright (C) 2011 Texas Instruments
 * Copyright (C) 2017 Eric Anholt
<<<<<<< HEAD
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms of
 * such GNU licence.
=======
>>>>>>> upstream/android-13
 */

/**
 * DOC: Faraday TV Encoder TVE200 DRM Driver
 *
 * The Faraday TV Encoder TVE200 is also known as the Gemini TV Interface
 * Controller (TVC) and is found in the Gemini Chipset from Storlink
 * Semiconductor (later Storm Semiconductor, later Cortina Systems)
 * but also in the Grain Media GM8180 chipset. On the Gemini the module
 * is connected to 8 data lines and a single clock line, comprising an
 * 8-bit BT.656 interface.
 *
 * This is a very basic YUV display driver. The datasheet specifies that
 * it supports the ITU BT.656 standard. It requires a 27 MHz clock which is
 * the hallmark of any TV encoder supporting both PAL and NTSC.
 *
 * This driver exposes a standard KMS interface for this TV encoder.
 */

#include <linux/clk.h>
#include <linux/dma-buf.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/shmem_fs.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/version.h>

#include <drm/drmP.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_panel.h>
#include <drm/drm_of.h>
#include <drm/drm_bridge.h>
=======

#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "tve200_drm.h"

#define DRIVER_DESC      "DRM module for Faraday TVE200"

static const struct drm_mode_config_funcs mode_config_funcs = {
	.fb_create = drm_gem_fb_create,
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
};

static int tve200_modeset_init(struct drm_device *dev)
{
	struct drm_mode_config *mode_config;
	struct tve200_drm_dev_private *priv = dev->dev_private;
	struct drm_panel *panel;
	struct drm_bridge *bridge;
	int ret = 0;

	drm_mode_config_init(dev);
	mode_config = &dev->mode_config;
	mode_config->funcs = &mode_config_funcs;
	mode_config->min_width = 352;
	mode_config->max_width = 720;
	mode_config->min_height = 240;
	mode_config->max_height = 576;

	ret = drm_of_find_panel_or_bridge(dev->dev->of_node,
					  0, 0, &panel, &bridge);
	if (ret && ret != -ENODEV)
		return ret;
	if (panel) {
<<<<<<< HEAD
		bridge = drm_panel_bridge_add(panel,
					      DRM_MODE_CONNECTOR_Unknown);
=======
		bridge = drm_panel_bridge_add_typed(panel,
						    DRM_MODE_CONNECTOR_Unknown);
>>>>>>> upstream/android-13
		if (IS_ERR(bridge)) {
			ret = PTR_ERR(bridge);
			goto out_bridge;
		}
	} else {
		/*
		 * TODO: when we are using a different bridge than a panel
		 * (such as a dumb VGA connector) we need to devise a different
		 * method to get the connector out of the bridge.
		 */
		dev_err(dev->dev, "the bridge is not a panel\n");
		goto out_bridge;
	}

	ret = tve200_display_init(dev);
	if (ret) {
		dev_err(dev->dev, "failed to init display\n");
		goto out_bridge;
	}

	ret = drm_simple_display_pipe_attach_bridge(&priv->pipe,
						    bridge);
	if (ret) {
		dev_err(dev->dev, "failed to attach bridge\n");
		goto out_bridge;
	}

	priv->panel = panel;
<<<<<<< HEAD
	priv->connector = panel->connector;
=======
	priv->connector = drm_panel_bridge_connector(bridge);
>>>>>>> upstream/android-13
	priv->bridge = bridge;

	dev_info(dev->dev, "attached to panel %s\n",
		 dev_name(panel->dev));

	ret = drm_vblank_init(dev, 1);
	if (ret) {
		dev_err(dev->dev, "failed to init vblank\n");
		goto out_bridge;
	}

	drm_mode_config_reset(dev);
<<<<<<< HEAD

	/*
	 * Passing in 16 here will make the RGB656 mode the default
	 * Passing in 32 will use XRGB8888 mode
	 */
	drm_fb_cma_fbdev_init(dev, 16, 0);
=======
>>>>>>> upstream/android-13
	drm_kms_helper_poll_init(dev);

	goto finish;

out_bridge:
	if (panel)
		drm_panel_bridge_remove(bridge);
	drm_mode_config_cleanup(dev);
finish:
	return ret;
}

DEFINE_DRM_GEM_CMA_FOPS(drm_fops);

<<<<<<< HEAD
static struct drm_driver tve200_drm_driver = {
	.driver_features =
		DRIVER_MODESET | DRIVER_GEM | DRIVER_PRIME | DRIVER_ATOMIC,
	.lastclose = drm_fb_helper_lastclose,
=======
static const struct drm_driver tve200_drm_driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_ATOMIC,
>>>>>>> upstream/android-13
	.ioctls = NULL,
	.fops = &drm_fops,
	.name = "tve200",
	.desc = DRIVER_DESC,
	.date = "20170703",
	.major = 1,
	.minor = 0,
	.patchlevel = 0,
<<<<<<< HEAD
	.dumb_create = drm_gem_cma_dumb_create,
	.gem_free_object_unlocked = drm_gem_cma_free_object,
	.gem_vm_ops = &drm_gem_cma_vm_ops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_import = drm_gem_prime_import,
	.gem_prime_export = drm_gem_prime_export,
	.gem_prime_get_sg_table	= drm_gem_cma_prime_get_sg_table,
	.gem_prime_import_sg_table = drm_gem_cma_prime_import_sg_table,
	.gem_prime_vmap = drm_gem_cma_prime_vmap,
	.gem_prime_vunmap = drm_gem_cma_prime_vunmap,
	.gem_prime_mmap = drm_gem_cma_prime_mmap,
=======
	DRM_GEM_CMA_DRIVER_OPS,
>>>>>>> upstream/android-13
};

static int tve200_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct tve200_drm_dev_private *priv;
	struct drm_device *drm;
	struct resource *res;
	int irq;
	int ret;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	drm = drm_dev_alloc(&tve200_drm_driver, dev);
	if (IS_ERR(drm))
		return PTR_ERR(drm);
	platform_set_drvdata(pdev, drm);
	priv->drm = drm;
	drm->dev_private = priv;

	/* Clock the silicon so we can access the registers */
	priv->pclk = devm_clk_get(dev, "PCLK");
	if (IS_ERR(priv->pclk)) {
		dev_err(dev, "unable to get PCLK\n");
		ret = PTR_ERR(priv->pclk);
		goto dev_unref;
	}
	ret = clk_prepare_enable(priv->pclk);
	if (ret) {
		dev_err(dev, "failed to enable PCLK\n");
		goto dev_unref;
	}

	/* This clock is for the pixels (27MHz) */
	priv->clk = devm_clk_get(dev, "TVE");
	if (IS_ERR(priv->clk)) {
		dev_err(dev, "unable to get TVE clock\n");
		ret = PTR_ERR(priv->clk);
		goto clk_disable;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->regs)) {
		dev_err(dev, "%s failed mmio\n", __func__);
		ret = -EINVAL;
		goto clk_disable;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		ret = irq;
		goto clk_disable;
	}

	/* turn off interrupts before requesting the irq */
	writel(0, priv->regs + TVE200_INT_EN);

	ret = devm_request_irq(dev, irq, tve200_irq, 0, "tve200", priv);
	if (ret) {
		dev_err(dev, "failed to request irq %d\n", ret);
		goto clk_disable;
	}

	ret = tve200_modeset_init(drm);
	if (ret)
		goto clk_disable;

	ret = drm_dev_register(drm, 0);
	if (ret < 0)
		goto clk_disable;

<<<<<<< HEAD
=======
	/*
	 * Passing in 16 here will make the RGB565 mode the default
	 * Passing in 32 will use XRGB8888 mode
	 */
	drm_fbdev_generic_setup(drm, 16);

>>>>>>> upstream/android-13
	return 0;

clk_disable:
	clk_disable_unprepare(priv->pclk);
dev_unref:
<<<<<<< HEAD
	drm_dev_unref(drm);
=======
	drm_dev_put(drm);
>>>>>>> upstream/android-13
	return ret;
}

static int tve200_remove(struct platform_device *pdev)
{
	struct drm_device *drm = platform_get_drvdata(pdev);
	struct tve200_drm_dev_private *priv = drm->dev_private;

	drm_dev_unregister(drm);
<<<<<<< HEAD
	drm_fb_cma_fbdev_fini(drm);
=======
>>>>>>> upstream/android-13
	if (priv->panel)
		drm_panel_bridge_remove(priv->bridge);
	drm_mode_config_cleanup(drm);
	clk_disable_unprepare(priv->pclk);
<<<<<<< HEAD
	drm_dev_unref(drm);
=======
	drm_dev_put(drm);
>>>>>>> upstream/android-13

	return 0;
}

static const struct of_device_id tve200_of_match[] = {
	{
		.compatible = "faraday,tve200",
	},
	{},
};

static struct platform_driver tve200_driver = {
	.driver = {
		.name           = "tve200",
		.of_match_table = of_match_ptr(tve200_of_match),
	},
	.probe = tve200_probe,
	.remove = tve200_remove,
};
module_platform_driver(tve200_driver);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Linus Walleij <linus.walleij@linaro.org>");
MODULE_LICENSE("GPL");
