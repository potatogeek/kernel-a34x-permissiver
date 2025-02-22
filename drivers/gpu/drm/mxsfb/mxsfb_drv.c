<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2016 Marek Vasut <marex@denx.de>
 *
 * This code is based on drivers/video/fbdev/mxsfb.c :
 * Copyright (C) 2010 Juergen Beisert, Pengutronix
 * Copyright (C) 2008-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright (C) 2008 Embedded Alley Solutions, Inc All Rights Reserved.
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
 */

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/component.h>
#include <linux/list.h>
#include <linux/of_device.h>
#include <linux/of_graph.h>
#include <linux/of_reserved_mem.h>
#include <linux/pm_runtime.h>
#include <linux/reservation.h>

#include <drm/drmP.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
#include <drm/drm_simple_kms_helper.h>
=======
 */

#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_connector.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_mode_config.h>
#include <drm/drm_of.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "mxsfb_drv.h"
#include "mxsfb_regs.h"

enum mxsfb_devtype {
	MXSFB_V3,
	MXSFB_V4,
<<<<<<< HEAD
=======
	/*
	 * Starting at i.MX6 the hardware version register is gone, use the
	 * i.MX family number as the version.
	 */
	MXSFB_V6,
>>>>>>> upstream/android-13
};

static const struct mxsfb_devdata mxsfb_devdata[] = {
	[MXSFB_V3] = {
		.transfer_count	= LCDC_V3_TRANSFER_COUNT,
		.cur_buf	= LCDC_V3_CUR_BUF,
		.next_buf	= LCDC_V3_NEXT_BUF,
<<<<<<< HEAD
		.debug0		= LCDC_V3_DEBUG0,
		.hs_wdth_mask	= 0xff,
		.hs_wdth_shift	= 24,
		.ipversion	= 3,
=======
		.hs_wdth_mask	= 0xff,
		.hs_wdth_shift	= 24,
		.has_overlay	= false,
		.has_ctrl2	= false,
>>>>>>> upstream/android-13
	},
	[MXSFB_V4] = {
		.transfer_count	= LCDC_V4_TRANSFER_COUNT,
		.cur_buf	= LCDC_V4_CUR_BUF,
		.next_buf	= LCDC_V4_NEXT_BUF,
<<<<<<< HEAD
		.debug0		= LCDC_V4_DEBUG0,
		.hs_wdth_mask	= 0x3fff,
		.hs_wdth_shift	= 18,
		.ipversion	= 4,
	},
};

static const uint32_t mxsfb_formats[] = {
	DRM_FORMAT_XRGB8888,
	DRM_FORMAT_RGB565
};

static struct mxsfb_drm_private *
drm_pipe_to_mxsfb_drm_private(struct drm_simple_display_pipe *pipe)
{
	return container_of(pipe, struct mxsfb_drm_private, pipe);
}

=======
		.hs_wdth_mask	= 0x3fff,
		.hs_wdth_shift	= 18,
		.has_overlay	= false,
		.has_ctrl2	= true,
	},
	[MXSFB_V6] = {
		.transfer_count	= LCDC_V4_TRANSFER_COUNT,
		.cur_buf	= LCDC_V4_CUR_BUF,
		.next_buf	= LCDC_V4_NEXT_BUF,
		.hs_wdth_mask	= 0x3fff,
		.hs_wdth_shift	= 18,
		.has_overlay	= true,
		.has_ctrl2	= true,
	},
};

>>>>>>> upstream/android-13
void mxsfb_enable_axi_clk(struct mxsfb_drm_private *mxsfb)
{
	if (mxsfb->clk_axi)
		clk_prepare_enable(mxsfb->clk_axi);
}

void mxsfb_disable_axi_clk(struct mxsfb_drm_private *mxsfb)
{
	if (mxsfb->clk_axi)
		clk_disable_unprepare(mxsfb->clk_axi);
}

<<<<<<< HEAD
static const struct drm_mode_config_funcs mxsfb_mode_config_funcs = {
	.fb_create		= drm_gem_fb_create,
=======
static struct drm_framebuffer *
mxsfb_fb_create(struct drm_device *dev, struct drm_file *file_priv,
		const struct drm_mode_fb_cmd2 *mode_cmd)
{
	const struct drm_format_info *info;

	info = drm_get_format_info(dev, mode_cmd);
	if (!info)
		return ERR_PTR(-EINVAL);

	if (mode_cmd->width * info->cpp[0] != mode_cmd->pitches[0]) {
		dev_dbg(dev->dev, "Invalid pitch: fb width must match pitch\n");
		return ERR_PTR(-EINVAL);
	}

	return drm_gem_fb_create(dev, file_priv, mode_cmd);
}

static const struct drm_mode_config_funcs mxsfb_mode_config_funcs = {
	.fb_create		= mxsfb_fb_create,
>>>>>>> upstream/android-13
	.atomic_check		= drm_atomic_helper_check,
	.atomic_commit		= drm_atomic_helper_commit,
};

<<<<<<< HEAD
static void mxsfb_pipe_enable(struct drm_simple_display_pipe *pipe,
			      struct drm_crtc_state *crtc_state,
			      struct drm_plane_state *plane_state)
{
	struct mxsfb_drm_private *mxsfb = drm_pipe_to_mxsfb_drm_private(pipe);

	drm_panel_prepare(mxsfb->panel);
	mxsfb_crtc_enable(mxsfb);
	drm_panel_enable(mxsfb->panel);
}

static void mxsfb_pipe_disable(struct drm_simple_display_pipe *pipe)
{
	struct mxsfb_drm_private *mxsfb = drm_pipe_to_mxsfb_drm_private(pipe);

	drm_panel_disable(mxsfb->panel);
	mxsfb_crtc_disable(mxsfb);
	drm_panel_unprepare(mxsfb->panel);
}

static void mxsfb_pipe_update(struct drm_simple_display_pipe *pipe,
			      struct drm_plane_state *plane_state)
{
	struct mxsfb_drm_private *mxsfb = drm_pipe_to_mxsfb_drm_private(pipe);

	mxsfb_plane_atomic_update(mxsfb, plane_state);
}

static int mxsfb_pipe_enable_vblank(struct drm_simple_display_pipe *pipe)
{
	struct mxsfb_drm_private *mxsfb = drm_pipe_to_mxsfb_drm_private(pipe);

	/* Clear and enable VBLANK IRQ */
	mxsfb_enable_axi_clk(mxsfb);
	writel(CTRL1_CUR_FRAME_DONE_IRQ, mxsfb->base + LCDC_CTRL1 + REG_CLR);
	writel(CTRL1_CUR_FRAME_DONE_IRQ_EN, mxsfb->base + LCDC_CTRL1 + REG_SET);
	mxsfb_disable_axi_clk(mxsfb);
=======
static const struct drm_mode_config_helper_funcs mxsfb_mode_config_helpers = {
	.atomic_commit_tail = drm_atomic_helper_commit_tail_rpm,
};

static int mxsfb_attach_bridge(struct mxsfb_drm_private *mxsfb)
{
	struct drm_device *drm = mxsfb->drm;
	struct drm_connector_list_iter iter;
	struct drm_panel *panel;
	struct drm_bridge *bridge;
	int ret;

	ret = drm_of_find_panel_or_bridge(drm->dev->of_node, 0, 0, &panel,
					  &bridge);
	if (ret)
		return ret;

	if (panel) {
		bridge = devm_drm_panel_bridge_add_typed(drm->dev, panel,
							 DRM_MODE_CONNECTOR_DPI);
		if (IS_ERR(bridge))
			return PTR_ERR(bridge);
	}

	if (!bridge)
		return -ENODEV;

	ret = drm_bridge_attach(&mxsfb->encoder, bridge, NULL, 0);
	if (ret)
		return dev_err_probe(drm->dev, ret, "Failed to attach bridge\n");

	mxsfb->bridge = bridge;

	/*
	 * Get hold of the connector. This is a bit of a hack, until the bridge
	 * API gives us bus flags and formats.
	 */
	drm_connector_list_iter_begin(drm, &iter);
	mxsfb->connector = drm_connector_list_iter_next(&iter);
	drm_connector_list_iter_end(&iter);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void mxsfb_pipe_disable_vblank(struct drm_simple_display_pipe *pipe)
{
	struct mxsfb_drm_private *mxsfb = drm_pipe_to_mxsfb_drm_private(pipe);

	/* Disable and clear VBLANK IRQ */
	mxsfb_enable_axi_clk(mxsfb);
	writel(CTRL1_CUR_FRAME_DONE_IRQ_EN, mxsfb->base + LCDC_CTRL1 + REG_CLR);
	writel(CTRL1_CUR_FRAME_DONE_IRQ, mxsfb->base + LCDC_CTRL1 + REG_CLR);
	mxsfb_disable_axi_clk(mxsfb);
}

static struct drm_simple_display_pipe_funcs mxsfb_funcs = {
	.enable		= mxsfb_pipe_enable,
	.disable	= mxsfb_pipe_disable,
	.update		= mxsfb_pipe_update,
	.prepare_fb	= drm_gem_fb_simple_display_pipe_prepare_fb,
	.enable_vblank	= mxsfb_pipe_enable_vblank,
	.disable_vblank	= mxsfb_pipe_disable_vblank,
};

static int mxsfb_load(struct drm_device *drm, unsigned long flags)
=======
static irqreturn_t mxsfb_irq_handler(int irq, void *data)
{
	struct drm_device *drm = data;
	struct mxsfb_drm_private *mxsfb = drm->dev_private;
	u32 reg;

	reg = readl(mxsfb->base + LCDC_CTRL1);

	if (reg & CTRL1_CUR_FRAME_DONE_IRQ)
		drm_crtc_handle_vblank(&mxsfb->crtc);

	writel(CTRL1_CUR_FRAME_DONE_IRQ, mxsfb->base + LCDC_CTRL1 + REG_CLR);

	return IRQ_HANDLED;
}

static void mxsfb_irq_disable(struct drm_device *drm)
{
	struct mxsfb_drm_private *mxsfb = drm->dev_private;

	mxsfb_enable_axi_clk(mxsfb);

	/* Disable and clear VBLANK IRQ */
	writel(CTRL1_CUR_FRAME_DONE_IRQ_EN, mxsfb->base + LCDC_CTRL1 + REG_CLR);
	writel(CTRL1_CUR_FRAME_DONE_IRQ, mxsfb->base + LCDC_CTRL1 + REG_CLR);

	mxsfb_disable_axi_clk(mxsfb);
}

static int mxsfb_irq_install(struct drm_device *dev, int irq)
{
	if (irq == IRQ_NOTCONNECTED)
		return -ENOTCONN;

	mxsfb_irq_disable(dev);

	return request_irq(irq, mxsfb_irq_handler, 0,  dev->driver->name, dev);
}

static void mxsfb_irq_uninstall(struct drm_device *dev)
{
	struct mxsfb_drm_private *mxsfb = dev->dev_private;

	mxsfb_irq_disable(dev);
	free_irq(mxsfb->irq, dev);
}

static int mxsfb_load(struct drm_device *drm,
		      const struct mxsfb_devdata *devdata)
>>>>>>> upstream/android-13
{
	struct platform_device *pdev = to_platform_device(drm->dev);
	struct mxsfb_drm_private *mxsfb;
	struct resource *res;
	int ret;

	mxsfb = devm_kzalloc(&pdev->dev, sizeof(*mxsfb), GFP_KERNEL);
	if (!mxsfb)
		return -ENOMEM;

<<<<<<< HEAD
	drm->dev_private = mxsfb;
	mxsfb->devdata = &mxsfb_devdata[pdev->id_entry->driver_data];
=======
	mxsfb->drm = drm;
	drm->dev_private = mxsfb;
	mxsfb->devdata = devdata;
>>>>>>> upstream/android-13

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	mxsfb->base = devm_ioremap_resource(drm->dev, res);
	if (IS_ERR(mxsfb->base))
		return PTR_ERR(mxsfb->base);

	mxsfb->clk = devm_clk_get(drm->dev, NULL);
	if (IS_ERR(mxsfb->clk))
		return PTR_ERR(mxsfb->clk);

	mxsfb->clk_axi = devm_clk_get(drm->dev, "axi");
	if (IS_ERR(mxsfb->clk_axi))
		mxsfb->clk_axi = NULL;

	mxsfb->clk_disp_axi = devm_clk_get(drm->dev, "disp_axi");
	if (IS_ERR(mxsfb->clk_disp_axi))
		mxsfb->clk_disp_axi = NULL;

	ret = dma_set_mask_and_coherent(drm->dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;

	pm_runtime_enable(drm->dev);

<<<<<<< HEAD
=======
	/* Modeset init */
	drm_mode_config_init(drm);

	ret = mxsfb_kms_init(mxsfb);
	if (ret < 0) {
		dev_err(drm->dev, "Failed to initialize KMS pipeline\n");
		goto err_vblank;
	}

>>>>>>> upstream/android-13
	ret = drm_vblank_init(drm, drm->mode_config.num_crtc);
	if (ret < 0) {
		dev_err(drm->dev, "Failed to initialise vblank\n");
		goto err_vblank;
	}

<<<<<<< HEAD
	/* Modeset init */
	drm_mode_config_init(drm);

	ret = mxsfb_create_output(drm);
	if (ret < 0) {
		dev_err(drm->dev, "Failed to create outputs\n");
		goto err_vblank;
	}

	ret = drm_simple_display_pipe_init(drm, &mxsfb->pipe, &mxsfb_funcs,
			mxsfb_formats, ARRAY_SIZE(mxsfb_formats), NULL,
			&mxsfb->connector);
	if (ret < 0) {
		dev_err(drm->dev, "Cannot setup simple display pipe\n");
		goto err_vblank;
	}

	ret = drm_panel_attach(mxsfb->panel, &mxsfb->connector);
	if (ret) {
		dev_err(drm->dev, "Cannot connect panel\n");
=======
	/* Start with vertical blanking interrupt reporting disabled. */
	drm_crtc_vblank_off(&mxsfb->crtc);

	ret = mxsfb_attach_bridge(mxsfb);
	if (ret) {
		if (ret != -EPROBE_DEFER)
			dev_err(drm->dev, "Cannot connect bridge: %d\n", ret);
>>>>>>> upstream/android-13
		goto err_vblank;
	}

	drm->mode_config.min_width	= MXSFB_MIN_XRES;
	drm->mode_config.min_height	= MXSFB_MIN_YRES;
	drm->mode_config.max_width	= MXSFB_MAX_XRES;
	drm->mode_config.max_height	= MXSFB_MAX_YRES;
	drm->mode_config.funcs		= &mxsfb_mode_config_funcs;
<<<<<<< HEAD

	drm_mode_config_reset(drm);

	pm_runtime_get_sync(drm->dev);
	ret = drm_irq_install(drm, platform_get_irq(pdev, 0));
=======
	drm->mode_config.helper_private	= &mxsfb_mode_config_helpers;

	drm_mode_config_reset(drm);

	ret = platform_get_irq(pdev, 0);
	if (ret < 0)
		goto err_vblank;
	mxsfb->irq = ret;

	pm_runtime_get_sync(drm->dev);
	ret = mxsfb_irq_install(drm, mxsfb->irq);
>>>>>>> upstream/android-13
	pm_runtime_put_sync(drm->dev);

	if (ret < 0) {
		dev_err(drm->dev, "Failed to install IRQ handler\n");
<<<<<<< HEAD
		goto err_irq;
=======
		goto err_vblank;
>>>>>>> upstream/android-13
	}

	drm_kms_helper_poll_init(drm);

<<<<<<< HEAD
	mxsfb->fbdev = drm_fbdev_cma_init(drm, 32,
					  drm->mode_config.num_connector);
	if (IS_ERR(mxsfb->fbdev)) {
		ret = PTR_ERR(mxsfb->fbdev);
		mxsfb->fbdev = NULL;
		dev_err(drm->dev, "Failed to init FB CMA area\n");
		goto err_cma;
	}

=======
>>>>>>> upstream/android-13
	platform_set_drvdata(pdev, drm);

	drm_helper_hpd_irq_event(drm);

	return 0;

<<<<<<< HEAD
err_cma:
	drm_irq_uninstall(drm);
err_irq:
	drm_panel_detach(mxsfb->panel);
=======
>>>>>>> upstream/android-13
err_vblank:
	pm_runtime_disable(drm->dev);

	return ret;
}

static void mxsfb_unload(struct drm_device *drm)
{
<<<<<<< HEAD
	struct mxsfb_drm_private *mxsfb = drm->dev_private;

	if (mxsfb->fbdev)
		drm_fbdev_cma_fini(mxsfb->fbdev);

=======
>>>>>>> upstream/android-13
	drm_kms_helper_poll_fini(drm);
	drm_mode_config_cleanup(drm);

	pm_runtime_get_sync(drm->dev);
<<<<<<< HEAD
	drm_irq_uninstall(drm);
=======
	mxsfb_irq_uninstall(drm);
>>>>>>> upstream/android-13
	pm_runtime_put_sync(drm->dev);

	drm->dev_private = NULL;

	pm_runtime_disable(drm->dev);
}

<<<<<<< HEAD
static void mxsfb_lastclose(struct drm_device *drm)
{
	struct mxsfb_drm_private *mxsfb = drm->dev_private;

	drm_fbdev_cma_restore_mode(mxsfb->fbdev);
}

static void mxsfb_irq_preinstall(struct drm_device *drm)
{
	struct mxsfb_drm_private *mxsfb = drm->dev_private;

	mxsfb_pipe_disable_vblank(&mxsfb->pipe);
}

static irqreturn_t mxsfb_irq_handler(int irq, void *data)
{
	struct drm_device *drm = data;
	struct mxsfb_drm_private *mxsfb = drm->dev_private;
	u32 reg;

	mxsfb_enable_axi_clk(mxsfb);

	reg = readl(mxsfb->base + LCDC_CTRL1);

	if (reg & CTRL1_CUR_FRAME_DONE_IRQ)
		drm_crtc_handle_vblank(&mxsfb->pipe.crtc);

	writel(CTRL1_CUR_FRAME_DONE_IRQ, mxsfb->base + LCDC_CTRL1 + REG_CLR);

	mxsfb_disable_axi_clk(mxsfb);

	return IRQ_HANDLED;
}

DEFINE_DRM_GEM_CMA_FOPS(fops);

static struct drm_driver mxsfb_driver = {
	.driver_features	= DRIVER_GEM | DRIVER_MODESET |
				  DRIVER_PRIME | DRIVER_ATOMIC |
				  DRIVER_HAVE_IRQ,
	.lastclose		= mxsfb_lastclose,
	.irq_handler		= mxsfb_irq_handler,
	.irq_preinstall		= mxsfb_irq_preinstall,
	.irq_uninstall		= mxsfb_irq_preinstall,
	.gem_free_object_unlocked = drm_gem_cma_free_object,
	.gem_vm_ops		= &drm_gem_cma_vm_ops,
	.dumb_create		= drm_gem_cma_dumb_create,
	.prime_handle_to_fd	= drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle	= drm_gem_prime_fd_to_handle,
	.gem_prime_export	= drm_gem_prime_export,
	.gem_prime_import	= drm_gem_prime_import,
	.gem_prime_get_sg_table	= drm_gem_cma_prime_get_sg_table,
	.gem_prime_import_sg_table = drm_gem_cma_prime_import_sg_table,
	.gem_prime_vmap		= drm_gem_cma_prime_vmap,
	.gem_prime_vunmap	= drm_gem_cma_prime_vunmap,
	.gem_prime_mmap		= drm_gem_cma_prime_mmap,
=======
DEFINE_DRM_GEM_CMA_FOPS(fops);

static const struct drm_driver mxsfb_driver = {
	.driver_features	= DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	DRM_GEM_CMA_DRIVER_OPS,
>>>>>>> upstream/android-13
	.fops	= &fops,
	.name	= "mxsfb-drm",
	.desc	= "MXSFB Controller DRM",
	.date	= "20160824",
	.major	= 1,
	.minor	= 0,
};

<<<<<<< HEAD
static const struct platform_device_id mxsfb_devtype[] = {
	{ .name = "imx23-fb", .driver_data = MXSFB_V3, },
	{ .name = "imx28-fb", .driver_data = MXSFB_V4, },
	{ .name = "imx6sx-fb", .driver_data = MXSFB_V4, },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, mxsfb_devtype);

static const struct of_device_id mxsfb_dt_ids[] = {
	{ .compatible = "fsl,imx23-lcdif", .data = &mxsfb_devtype[0], },
	{ .compatible = "fsl,imx28-lcdif", .data = &mxsfb_devtype[1], },
	{ .compatible = "fsl,imx6sx-lcdif", .data = &mxsfb_devtype[2], },
=======
static const struct of_device_id mxsfb_dt_ids[] = {
	{ .compatible = "fsl,imx23-lcdif", .data = &mxsfb_devdata[MXSFB_V3], },
	{ .compatible = "fsl,imx28-lcdif", .data = &mxsfb_devdata[MXSFB_V4], },
	{ .compatible = "fsl,imx6sx-lcdif", .data = &mxsfb_devdata[MXSFB_V6], },
>>>>>>> upstream/android-13
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mxsfb_dt_ids);

static int mxsfb_probe(struct platform_device *pdev)
{
	struct drm_device *drm;
	const struct of_device_id *of_id =
			of_match_device(mxsfb_dt_ids, &pdev->dev);
	int ret;

	if (!pdev->dev.of_node)
		return -ENODEV;

<<<<<<< HEAD
	if (of_id)
		pdev->id_entry = of_id->data;

=======
>>>>>>> upstream/android-13
	drm = drm_dev_alloc(&mxsfb_driver, &pdev->dev);
	if (IS_ERR(drm))
		return PTR_ERR(drm);

<<<<<<< HEAD
	ret = mxsfb_load(drm, 0);
=======
	ret = mxsfb_load(drm, of_id->data);
>>>>>>> upstream/android-13
	if (ret)
		goto err_free;

	ret = drm_dev_register(drm, 0);
	if (ret)
		goto err_unload;

<<<<<<< HEAD
=======
	drm_fbdev_generic_setup(drm, 32);

>>>>>>> upstream/android-13
	return 0;

err_unload:
	mxsfb_unload(drm);
err_free:
<<<<<<< HEAD
	drm_dev_unref(drm);
=======
	drm_dev_put(drm);
>>>>>>> upstream/android-13

	return ret;
}

static int mxsfb_remove(struct platform_device *pdev)
{
	struct drm_device *drm = platform_get_drvdata(pdev);

	drm_dev_unregister(drm);
	mxsfb_unload(drm);
<<<<<<< HEAD
	drm_dev_unref(drm);
=======
	drm_dev_put(drm);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static struct platform_driver mxsfb_platform_driver = {
	.probe		= mxsfb_probe,
	.remove		= mxsfb_remove,
	.id_table	= mxsfb_devtype,
	.driver	= {
		.name		= "mxsfb",
		.of_match_table	= mxsfb_dt_ids,
=======
#ifdef CONFIG_PM_SLEEP
static int mxsfb_suspend(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);

	return drm_mode_config_helper_suspend(drm);
}

static int mxsfb_resume(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);

	return drm_mode_config_helper_resume(drm);
}
#endif

static const struct dev_pm_ops mxsfb_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(mxsfb_suspend, mxsfb_resume)
};

static struct platform_driver mxsfb_platform_driver = {
	.probe		= mxsfb_probe,
	.remove		= mxsfb_remove,
	.driver	= {
		.name		= "mxsfb",
		.of_match_table	= mxsfb_dt_ids,
		.pm		= &mxsfb_pm_ops,
>>>>>>> upstream/android-13
	},
};

module_platform_driver(mxsfb_platform_driver);

MODULE_AUTHOR("Marek Vasut <marex@denx.de>");
MODULE_DESCRIPTION("Freescale MXS DRM/KMS driver");
MODULE_LICENSE("GPL");
