<<<<<<< HEAD
/*
 * Copyright (C) 2012 Texas Instruments
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Texas Instruments
 * Author: Rob Clark <robdclark@gmail.com>
>>>>>>> upstream/android-13
 */

/* LCDC DRM driver, based on da8xx-fb */

#include <linux/component.h>
<<<<<<< HEAD
#include <linux/pinctrl/consumer.h>
#include <linux/suspend.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>

#include "tilcdc_drv.h"
#include "tilcdc_regs.h"
#include "tilcdc_tfp410.h"
#include "tilcdc_panel.h"
#include "tilcdc_external.h"
=======
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_debugfs.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_mm.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>


#include "tilcdc_drv.h"
#include "tilcdc_external.h"
#include "tilcdc_panel.h"
#include "tilcdc_regs.h"
>>>>>>> upstream/android-13

static LIST_HEAD(module_list);

static const u32 tilcdc_rev1_formats[] = { DRM_FORMAT_RGB565 };

static const u32 tilcdc_straight_formats[] = { DRM_FORMAT_RGB565,
					       DRM_FORMAT_BGR888,
					       DRM_FORMAT_XBGR8888 };

static const u32 tilcdc_crossed_formats[] = { DRM_FORMAT_BGR565,
					      DRM_FORMAT_RGB888,
					      DRM_FORMAT_XRGB8888 };

static const u32 tilcdc_legacy_formats[] = { DRM_FORMAT_RGB565,
					     DRM_FORMAT_RGB888,
					     DRM_FORMAT_XRGB8888 };

void tilcdc_module_init(struct tilcdc_module *mod, const char *name,
		const struct tilcdc_module_ops *funcs)
{
	mod->name = name;
	mod->funcs = funcs;
	INIT_LIST_HEAD(&mod->list);
	list_add(&mod->list, &module_list);
}

void tilcdc_module_cleanup(struct tilcdc_module *mod)
{
	list_del(&mod->list);
}

static struct of_device_id tilcdc_of_match[];

<<<<<<< HEAD
static struct drm_framebuffer *tilcdc_fb_create(struct drm_device *dev,
		struct drm_file *file_priv, const struct drm_mode_fb_cmd2 *mode_cmd)
{
	return drm_gem_fb_create(dev, file_priv, mode_cmd);
}

=======
>>>>>>> upstream/android-13
static int tilcdc_atomic_check(struct drm_device *dev,
			       struct drm_atomic_state *state)
{
	int ret;

	ret = drm_atomic_helper_check_modeset(dev, state);
	if (ret)
		return ret;

	ret = drm_atomic_helper_check_planes(dev, state);
	if (ret)
		return ret;

	/*
	 * tilcdc ->atomic_check can update ->mode_changed if pixel format
	 * changes, hence will we check modeset changes again.
	 */
	ret = drm_atomic_helper_check_modeset(dev, state);
	if (ret)
		return ret;

	return ret;
}

<<<<<<< HEAD
static int tilcdc_commit(struct drm_device *dev,
		  struct drm_atomic_state *state,
		  bool async)
{
	int ret;

	ret = drm_atomic_helper_prepare_planes(dev, state);
	if (ret)
		return ret;

	ret = drm_atomic_helper_swap_state(state, true);
	if (ret) {
		drm_atomic_helper_cleanup_planes(dev, state);
		return ret;
	}

	/*
	 * Everything below can be run asynchronously without the need to grab
	 * any modeset locks at all under one condition: It must be guaranteed
	 * that the asynchronous work has either been cancelled (if the driver
	 * supports it, which at least requires that the framebuffers get
	 * cleaned up with drm_atomic_helper_cleanup_planes()) or completed
	 * before the new state gets committed on the software side with
	 * drm_atomic_helper_swap_state().
	 *
	 * This scheme allows new atomic state updates to be prepared and
	 * checked in parallel to the asynchronous completion of the previous
	 * update. Which is important since compositors need to figure out the
	 * composition of the next frame right after having submitted the
	 * current layout.
	 */

	drm_atomic_helper_commit_modeset_disables(dev, state);

	drm_atomic_helper_commit_planes(dev, state, 0);

	drm_atomic_helper_commit_modeset_enables(dev, state);

	drm_atomic_helper_wait_for_vblanks(dev, state);

	drm_atomic_helper_cleanup_planes(dev, state);

	return 0;
}

static const struct drm_mode_config_funcs mode_config_funcs = {
	.fb_create = tilcdc_fb_create,
	.output_poll_changed = drm_fb_helper_output_poll_changed,
	.atomic_check = tilcdc_atomic_check,
	.atomic_commit = tilcdc_commit,
=======
static const struct drm_mode_config_funcs mode_config_funcs = {
	.fb_create = drm_gem_fb_create,
	.atomic_check = tilcdc_atomic_check,
	.atomic_commit = drm_atomic_helper_commit,
>>>>>>> upstream/android-13
};

static void modeset_init(struct drm_device *dev)
{
	struct tilcdc_drm_private *priv = dev->dev_private;
	struct tilcdc_module *mod;

	list_for_each_entry(mod, &module_list, list) {
		DBG("loading module: %s", mod->name);
		mod->funcs->modeset_init(mod, dev);
	}

	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;
<<<<<<< HEAD
	dev->mode_config.max_width = tilcdc_crtc_max_width(priv->crtc);
=======
	dev->mode_config.max_width = priv->max_width;
>>>>>>> upstream/android-13
	dev->mode_config.max_height = 2048;
	dev->mode_config.funcs = &mode_config_funcs;
}

#ifdef CONFIG_CPU_FREQ
static int cpufreq_transition(struct notifier_block *nb,
				     unsigned long val, void *data)
{
	struct tilcdc_drm_private *priv = container_of(nb,
			struct tilcdc_drm_private, freq_transition);

	if (val == CPUFREQ_POSTCHANGE)
		tilcdc_crtc_update_clk(priv->crtc);

	return 0;
}
#endif

<<<<<<< HEAD
=======
static irqreturn_t tilcdc_irq(int irq, void *arg)
{
	struct drm_device *dev = arg;
	struct tilcdc_drm_private *priv = dev->dev_private;

	return tilcdc_crtc_irq(priv->crtc);
}

static int tilcdc_irq_install(struct drm_device *dev, unsigned int irq)
{
	struct tilcdc_drm_private *priv = dev->dev_private;
	int ret;

	ret = request_irq(irq, tilcdc_irq, 0, dev->driver->name, dev);
	if (ret)
		return ret;

	priv->irq_enabled = false;

	return 0;
}

static void tilcdc_irq_uninstall(struct drm_device *dev)
{
	struct tilcdc_drm_private *priv = dev->dev_private;

	if (!priv->irq_enabled)
		return;

	free_irq(priv->irq, dev);
	priv->irq_enabled = false;
}

>>>>>>> upstream/android-13
/*
 * DRM operations:
 */

static void tilcdc_fini(struct drm_device *dev)
{
	struct tilcdc_drm_private *priv = dev->dev_private;

#ifdef CONFIG_CPU_FREQ
	if (priv->freq_transition.notifier_call)
		cpufreq_unregister_notifier(&priv->freq_transition,
					    CPUFREQ_TRANSITION_NOTIFIER);
#endif

	if (priv->crtc)
		tilcdc_crtc_shutdown(priv->crtc);

	if (priv->is_registered)
		drm_dev_unregister(dev);

	drm_kms_helper_poll_fini(dev);
<<<<<<< HEAD

	drm_fb_cma_fbdev_fini(dev);

	drm_irq_uninstall(dev);
	drm_mode_config_cleanup(dev);
	tilcdc_remove_external_device(dev);
=======
	tilcdc_irq_uninstall(dev);
	drm_mode_config_cleanup(dev);
>>>>>>> upstream/android-13

	if (priv->clk)
		clk_put(priv->clk);

	if (priv->mmio)
		iounmap(priv->mmio);

	if (priv->wq) {
		flush_workqueue(priv->wq);
		destroy_workqueue(priv->wq);
	}

	dev->dev_private = NULL;

	pm_runtime_disable(dev->dev);

	drm_dev_put(dev);
}

<<<<<<< HEAD
static int tilcdc_init(struct drm_driver *ddrv, struct device *dev)
=======
static int tilcdc_init(const struct drm_driver *ddrv, struct device *dev)
>>>>>>> upstream/android-13
{
	struct drm_device *ddev;
	struct platform_device *pdev = to_platform_device(dev);
	struct device_node *node = dev->of_node;
	struct tilcdc_drm_private *priv;
	struct resource *res;
	u32 bpp = 0;
	int ret;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	ddev = drm_dev_alloc(ddrv, dev);
	if (IS_ERR(ddev))
		return PTR_ERR(ddev);

	ddev->dev_private = priv;
	platform_set_drvdata(pdev, ddev);
	drm_mode_config_init(ddev);

	priv->is_componentized =
		tilcdc_get_external_components(dev, NULL) > 0;

	priv->wq = alloc_ordered_workqueue("tilcdc", 0);
	if (!priv->wq) {
		ret = -ENOMEM;
		goto init_failed;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "failed to get memory resource\n");
		ret = -EINVAL;
		goto init_failed;
	}

<<<<<<< HEAD
	priv->mmio = ioremap_nocache(res->start, resource_size(res));
=======
	priv->mmio = ioremap(res->start, resource_size(res));
>>>>>>> upstream/android-13
	if (!priv->mmio) {
		dev_err(dev, "failed to ioremap\n");
		ret = -ENOMEM;
		goto init_failed;
	}

	priv->clk = clk_get(dev, "fck");
	if (IS_ERR(priv->clk)) {
		dev_err(dev, "failed to get functional clock\n");
		ret = -ENODEV;
		goto init_failed;
	}

<<<<<<< HEAD
	if (of_property_read_u32(node, "max-bandwidth", &priv->max_bandwidth))
		priv->max_bandwidth = TILCDC_DEFAULT_MAX_BANDWIDTH;

	DBG("Maximum Bandwidth Value %d", priv->max_bandwidth);

	if (of_property_read_u32(node, "max-width", &priv->max_width))
		priv->max_width = TILCDC_DEFAULT_MAX_WIDTH;

	DBG("Maximum Horizontal Pixel Width Value %dpixels", priv->max_width);

	if (of_property_read_u32(node, "max-pixelclock",
					&priv->max_pixelclock))
		priv->max_pixelclock = TILCDC_DEFAULT_MAX_PIXELCLOCK;

	DBG("Maximum Pixel Clock Value %dKHz", priv->max_pixelclock);

=======
>>>>>>> upstream/android-13
	pm_runtime_enable(dev);

	/* Determine LCD IP Version */
	pm_runtime_get_sync(dev);
	switch (tilcdc_read(ddev, LCDC_PID_REG)) {
	case 0x4c100102:
		priv->rev = 1;
		break;
	case 0x4f200800:
	case 0x4f201000:
		priv->rev = 2;
		break;
	default:
		dev_warn(dev, "Unknown PID Reg value 0x%08x, "
			"defaulting to LCD revision 1\n",
			tilcdc_read(ddev, LCDC_PID_REG));
		priv->rev = 1;
		break;
	}

	pm_runtime_put_sync(dev);

	if (priv->rev == 1) {
		DBG("Revision 1 LCDC supports only RGB565 format");
		priv->pixelformats = tilcdc_rev1_formats;
		priv->num_pixelformats = ARRAY_SIZE(tilcdc_rev1_formats);
		bpp = 16;
	} else {
		const char *str = "\0";

		of_property_read_string(node, "blue-and-red-wiring", &str);
		if (0 == strcmp(str, "crossed")) {
			DBG("Configured for crossed blue and red wires");
			priv->pixelformats = tilcdc_crossed_formats;
			priv->num_pixelformats =
				ARRAY_SIZE(tilcdc_crossed_formats);
			bpp = 32; /* Choose bpp with RGB support for fbdef */
		} else if (0 == strcmp(str, "straight")) {
			DBG("Configured for straight blue and red wires");
			priv->pixelformats = tilcdc_straight_formats;
			priv->num_pixelformats =
				ARRAY_SIZE(tilcdc_straight_formats);
			bpp = 16; /* Choose bpp with RGB support for fbdef */
		} else {
			DBG("Blue and red wiring '%s' unknown, use legacy mode",
			    str);
			priv->pixelformats = tilcdc_legacy_formats;
			priv->num_pixelformats =
				ARRAY_SIZE(tilcdc_legacy_formats);
			bpp = 16; /* This is just a guess */
		}
	}

<<<<<<< HEAD
=======
	if (of_property_read_u32(node, "max-bandwidth", &priv->max_bandwidth))
		priv->max_bandwidth = TILCDC_DEFAULT_MAX_BANDWIDTH;

	DBG("Maximum Bandwidth Value %d", priv->max_bandwidth);

	if (of_property_read_u32(node, "max-width", &priv->max_width)) {
		if (priv->rev == 1)
			priv->max_width = TILCDC_DEFAULT_MAX_WIDTH_V1;
		else
			priv->max_width = TILCDC_DEFAULT_MAX_WIDTH_V2;
	}

	DBG("Maximum Horizontal Pixel Width Value %dpixels", priv->max_width);

	if (of_property_read_u32(node, "max-pixelclock",
				 &priv->max_pixelclock))
		priv->max_pixelclock = TILCDC_DEFAULT_MAX_PIXELCLOCK;

	DBG("Maximum Pixel Clock Value %dKHz", priv->max_pixelclock);

>>>>>>> upstream/android-13
	ret = tilcdc_crtc_create(ddev);
	if (ret < 0) {
		dev_err(dev, "failed to create crtc\n");
		goto init_failed;
	}
	modeset_init(ddev);

#ifdef CONFIG_CPU_FREQ
	priv->freq_transition.notifier_call = cpufreq_transition;
	ret = cpufreq_register_notifier(&priv->freq_transition,
			CPUFREQ_TRANSITION_NOTIFIER);
	if (ret) {
		dev_err(dev, "failed to register cpufreq notifier\n");
		priv->freq_transition.notifier_call = NULL;
		goto init_failed;
	}
#endif

	if (priv->is_componentized) {
		ret = component_bind_all(dev, ddev);
		if (ret < 0)
			goto init_failed;

		ret = tilcdc_add_component_encoder(ddev);
		if (ret < 0)
			goto init_failed;
	} else {
		ret = tilcdc_attach_external_device(ddev);
		if (ret)
			goto init_failed;
	}

	if (!priv->external_connector &&
	    ((priv->num_encoders == 0) || (priv->num_connectors == 0))) {
		dev_err(dev, "no encoders/connectors found\n");
		ret = -EPROBE_DEFER;
		goto init_failed;
	}

	ret = drm_vblank_init(ddev, 1);
	if (ret < 0) {
		dev_err(dev, "failed to initialize vblank\n");
		goto init_failed;
	}

<<<<<<< HEAD
	ret = drm_irq_install(ddev, platform_get_irq(pdev, 0));
=======
	ret = platform_get_irq(pdev, 0);
	if (ret < 0)
		goto init_failed;
	priv->irq = ret;

	ret = tilcdc_irq_install(ddev, priv->irq);
>>>>>>> upstream/android-13
	if (ret < 0) {
		dev_err(dev, "failed to install IRQ handler\n");
		goto init_failed;
	}

	drm_mode_config_reset(ddev);

<<<<<<< HEAD
	ret = drm_fb_cma_fbdev_init(ddev, bpp, 0);
	if (ret)
		goto init_failed;

=======
>>>>>>> upstream/android-13
	drm_kms_helper_poll_init(ddev);

	ret = drm_dev_register(ddev, 0);
	if (ret)
		goto init_failed;
<<<<<<< HEAD

	priv->is_registered = true;
=======
	priv->is_registered = true;

	drm_fbdev_generic_setup(ddev, bpp);
>>>>>>> upstream/android-13
	return 0;

init_failed:
	tilcdc_fini(ddev);

	return ret;
}

<<<<<<< HEAD
static irqreturn_t tilcdc_irq(int irq, void *arg)
{
	struct drm_device *dev = arg;
	struct tilcdc_drm_private *priv = dev->dev_private;
	return tilcdc_crtc_irq(priv->crtc);
}

=======
>>>>>>> upstream/android-13
#if defined(CONFIG_DEBUG_FS)
static const struct {
	const char *name;
	uint8_t  rev;
	uint8_t  save;
	uint32_t reg;
} registers[] =		{
#define REG(rev, save, reg) { #reg, rev, save, reg }
		/* exists in revision 1: */
		REG(1, false, LCDC_PID_REG),
		REG(1, true,  LCDC_CTRL_REG),
		REG(1, false, LCDC_STAT_REG),
		REG(1, true,  LCDC_RASTER_CTRL_REG),
		REG(1, true,  LCDC_RASTER_TIMING_0_REG),
		REG(1, true,  LCDC_RASTER_TIMING_1_REG),
		REG(1, true,  LCDC_RASTER_TIMING_2_REG),
		REG(1, true,  LCDC_DMA_CTRL_REG),
		REG(1, true,  LCDC_DMA_FB_BASE_ADDR_0_REG),
		REG(1, true,  LCDC_DMA_FB_CEILING_ADDR_0_REG),
		REG(1, true,  LCDC_DMA_FB_BASE_ADDR_1_REG),
		REG(1, true,  LCDC_DMA_FB_CEILING_ADDR_1_REG),
		/* new in revision 2: */
		REG(2, false, LCDC_RAW_STAT_REG),
		REG(2, false, LCDC_MASKED_STAT_REG),
		REG(2, true, LCDC_INT_ENABLE_SET_REG),
		REG(2, false, LCDC_INT_ENABLE_CLR_REG),
		REG(2, false, LCDC_END_OF_INT_IND_REG),
		REG(2, true,  LCDC_CLK_ENABLE_REG),
#undef REG
};

#endif

#ifdef CONFIG_DEBUG_FS
static int tilcdc_regs_show(struct seq_file *m, void *arg)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_device *dev = node->minor->dev;
	struct tilcdc_drm_private *priv = dev->dev_private;
	unsigned i;

	pm_runtime_get_sync(dev->dev);

	seq_printf(m, "revision: %d\n", priv->rev);

	for (i = 0; i < ARRAY_SIZE(registers); i++)
		if (priv->rev >= registers[i].rev)
			seq_printf(m, "%s:\t %08x\n", registers[i].name,
					tilcdc_read(dev, registers[i].reg));

	pm_runtime_put_sync(dev->dev);

	return 0;
}

static int tilcdc_mm_show(struct seq_file *m, void *arg)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_device *dev = node->minor->dev;
	struct drm_printer p = drm_seq_file_printer(m);
	drm_mm_print(&dev->vma_offset_manager->vm_addr_space_mm, &p);
	return 0;
}

static struct drm_info_list tilcdc_debugfs_list[] = {
<<<<<<< HEAD
		{ "regs", tilcdc_regs_show, 0 },
		{ "mm",   tilcdc_mm_show,   0 },
};

static int tilcdc_debugfs_init(struct drm_minor *minor)
{
	struct drm_device *dev = minor->dev;
	struct tilcdc_module *mod;
	int ret;

	ret = drm_debugfs_create_files(tilcdc_debugfs_list,
			ARRAY_SIZE(tilcdc_debugfs_list),
			minor->debugfs_root, minor);
=======
		{ "regs", tilcdc_regs_show, 0, NULL },
		{ "mm",   tilcdc_mm_show,   0, NULL },
};

static void tilcdc_debugfs_init(struct drm_minor *minor)
{
	struct tilcdc_module *mod;

	drm_debugfs_create_files(tilcdc_debugfs_list,
				 ARRAY_SIZE(tilcdc_debugfs_list),
				 minor->debugfs_root, minor);
>>>>>>> upstream/android-13

	list_for_each_entry(mod, &module_list, list)
		if (mod->funcs->debugfs_init)
			mod->funcs->debugfs_init(mod, minor);
<<<<<<< HEAD

	if (ret) {
		dev_err(dev->dev, "could not install tilcdc_debugfs_list\n");
		return ret;
	}

	return ret;
=======
>>>>>>> upstream/android-13
}
#endif

DEFINE_DRM_GEM_CMA_FOPS(fops);

<<<<<<< HEAD
static struct drm_driver tilcdc_driver = {
	.driver_features    = (DRIVER_HAVE_IRQ | DRIVER_GEM | DRIVER_MODESET |
			       DRIVER_PRIME | DRIVER_ATOMIC),
	.lastclose          = drm_fb_helper_lastclose,
	.irq_handler        = tilcdc_irq,
	.gem_free_object_unlocked = drm_gem_cma_free_object,
	.gem_print_info     = drm_gem_cma_print_info,
	.gem_vm_ops         = &drm_gem_cma_vm_ops,
	.dumb_create        = drm_gem_cma_dumb_create,

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
static const struct drm_driver tilcdc_driver = {
	.driver_features    = DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	DRM_GEM_CMA_DRIVER_OPS,
>>>>>>> upstream/android-13
#ifdef CONFIG_DEBUG_FS
	.debugfs_init       = tilcdc_debugfs_init,
#endif
	.fops               = &fops,
	.name               = "tilcdc",
	.desc               = "TI LCD Controller DRM",
	.date               = "20121205",
	.major              = 1,
	.minor              = 0,
};

/*
 * Power management:
 */

#ifdef CONFIG_PM_SLEEP
static int tilcdc_pm_suspend(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct tilcdc_drm_private *priv = ddev->dev_private;

	priv->saved_state = drm_atomic_helper_suspend(ddev);
=======
	int ret = 0;

	ret = drm_mode_config_helper_suspend(ddev);
>>>>>>> upstream/android-13

	/* Select sleep pin state */
	pinctrl_pm_select_sleep_state(dev);

<<<<<<< HEAD
	return 0;
=======
	return ret;
>>>>>>> upstream/android-13
}

static int tilcdc_pm_resume(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct tilcdc_drm_private *priv = ddev->dev_private;
	int ret = 0;

	/* Select default pin state */
	pinctrl_pm_select_default_state(dev);

	if (priv->saved_state)
		ret = drm_atomic_helper_resume(ddev, priv->saved_state);

	return ret;
=======

	/* Select default pin state */
	pinctrl_pm_select_default_state(dev);
	return  drm_mode_config_helper_resume(ddev);
>>>>>>> upstream/android-13
}
#endif

static const struct dev_pm_ops tilcdc_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(tilcdc_pm_suspend, tilcdc_pm_resume)
};

/*
 * Platform driver:
 */
static int tilcdc_bind(struct device *dev)
{
	return tilcdc_init(&tilcdc_driver, dev);
}

static void tilcdc_unbind(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);

	/* Check if a subcomponent has already triggered the unloading. */
	if (!ddev->dev_private)
		return;

	tilcdc_fini(dev_get_drvdata(dev));
}

static const struct component_master_ops tilcdc_comp_ops = {
	.bind = tilcdc_bind,
	.unbind = tilcdc_unbind,
};

static int tilcdc_pdev_probe(struct platform_device *pdev)
{
	struct component_match *match = NULL;
	int ret;

	/* bail out early if no DT data: */
	if (!pdev->dev.of_node) {
		dev_err(&pdev->dev, "device-tree data is missing\n");
		return -ENXIO;
	}

	ret = tilcdc_get_external_components(&pdev->dev, &match);
	if (ret < 0)
		return ret;
	else if (ret == 0)
		return tilcdc_init(&tilcdc_driver, &pdev->dev);
	else
		return component_master_add_with_match(&pdev->dev,
						       &tilcdc_comp_ops,
						       match);
}

static int tilcdc_pdev_remove(struct platform_device *pdev)
{
	int ret;

	ret = tilcdc_get_external_components(&pdev->dev, NULL);
	if (ret < 0)
		return ret;
	else if (ret == 0)
		tilcdc_fini(platform_get_drvdata(pdev));
	else
		component_master_del(&pdev->dev, &tilcdc_comp_ops);

	return 0;
}

static struct of_device_id tilcdc_of_match[] = {
		{ .compatible = "ti,am33xx-tilcdc", },
		{ .compatible = "ti,da850-tilcdc", },
		{ },
};
MODULE_DEVICE_TABLE(of, tilcdc_of_match);

static struct platform_driver tilcdc_platform_driver = {
	.probe      = tilcdc_pdev_probe,
	.remove     = tilcdc_pdev_remove,
	.driver     = {
		.name   = "tilcdc",
		.pm     = &tilcdc_pm_ops,
		.of_match_table = tilcdc_of_match,
	},
};

static int __init tilcdc_drm_init(void)
{
	DBG("init");
<<<<<<< HEAD
	tilcdc_tfp410_init();
=======
>>>>>>> upstream/android-13
	tilcdc_panel_init();
	return platform_driver_register(&tilcdc_platform_driver);
}

static void __exit tilcdc_drm_fini(void)
{
	DBG("fini");
	platform_driver_unregister(&tilcdc_platform_driver);
	tilcdc_panel_fini();
<<<<<<< HEAD
	tilcdc_tfp410_fini();
=======
>>>>>>> upstream/android-13
}

module_init(tilcdc_drm_init);
module_exit(tilcdc_drm_fini);

MODULE_AUTHOR("Rob Clark <robdclark@gmail.com");
MODULE_DESCRIPTION("TI LCD Controller DRM Driver");
MODULE_LICENSE("GPL");
