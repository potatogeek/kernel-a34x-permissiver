<<<<<<< HEAD
/*
 * Copyright (C) 2014-2015 Broadcom
 * Copyright (C) 2013 Red Hat
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2014-2015 Broadcom
 * Copyright (C) 2013 Red Hat
>>>>>>> upstream/android-13
 */

/**
 * DOC: Broadcom VC4 Graphics Driver
 *
 * The Broadcom VideoCore 4 (present in the Raspberry Pi) contains a
 * OpenGL ES 2.0-compatible 3D engine called V3D, and a highly
 * configurable display output pipeline that supports HDMI, DSI, DPI,
 * and Composite TV output.
 *
 * The 3D engine also has an interface for submitting arbitrary
 * compute shader-style jobs using the same shader processor as is
 * used for vertex and fragment shaders in GLES 2.0.  However, given
 * that the hardware isn't able to expose any standard interfaces like
 * OpenGL compute shaders or OpenCL, it isn't supported by this
 * driver.
 */

#include <linux/clk.h>
#include <linux/component.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/dma-mapping.h>
>>>>>>> upstream/android-13
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>

#include "uapi/drm/vc4_drm.h"
=======

#include <drm/drm_aperture.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_vblank.h>

#include "uapi/drm/vc4_drm.h"

>>>>>>> upstream/android-13
#include "vc4_drv.h"
#include "vc4_regs.h"

#define DRIVER_NAME "vc4"
#define DRIVER_DESC "Broadcom VC4 graphics"
#define DRIVER_DATE "20140616"
#define DRIVER_MAJOR 0
#define DRIVER_MINOR 0
#define DRIVER_PATCHLEVEL 0

/* Helper function for mapping the regs on a platform device. */
void __iomem *vc4_ioremap_regs(struct platform_device *dev, int index)
{
	struct resource *res;
	void __iomem *map;

	res = platform_get_resource(dev, IORESOURCE_MEM, index);
	map = devm_ioremap_resource(&dev->dev, res);
<<<<<<< HEAD
	if (IS_ERR(map)) {
		DRM_ERROR("Failed to map registers: %ld\n", PTR_ERR(map));
		return map;
	}
=======
	if (IS_ERR(map))
		return map;
>>>>>>> upstream/android-13

	return map;
}

static int vc4_get_param_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file_priv)
{
	struct vc4_dev *vc4 = to_vc4_dev(dev);
	struct drm_vc4_get_param *args = data;
	int ret;

	if (args->pad != 0)
		return -EINVAL;

<<<<<<< HEAD
	switch (args->param) {
	case DRM_VC4_PARAM_V3D_IDENT0:
		ret = pm_runtime_get_sync(&vc4->v3d->pdev->dev);
		if (ret < 0)
			return ret;
		args->value = V3D_READ(V3D_IDENT0);
		pm_runtime_mark_last_busy(&vc4->v3d->pdev->dev);
		pm_runtime_put_autosuspend(&vc4->v3d->pdev->dev);
		break;
	case DRM_VC4_PARAM_V3D_IDENT1:
		ret = pm_runtime_get_sync(&vc4->v3d->pdev->dev);
		if (ret < 0)
			return ret;
		args->value = V3D_READ(V3D_IDENT1);
		pm_runtime_mark_last_busy(&vc4->v3d->pdev->dev);
		pm_runtime_put_autosuspend(&vc4->v3d->pdev->dev);
		break;
	case DRM_VC4_PARAM_V3D_IDENT2:
		ret = pm_runtime_get_sync(&vc4->v3d->pdev->dev);
		if (ret < 0)
			return ret;
		args->value = V3D_READ(V3D_IDENT2);
		pm_runtime_mark_last_busy(&vc4->v3d->pdev->dev);
		pm_runtime_put_autosuspend(&vc4->v3d->pdev->dev);
=======
	if (!vc4->v3d)
		return -ENODEV;

	switch (args->param) {
	case DRM_VC4_PARAM_V3D_IDENT0:
		ret = vc4_v3d_pm_get(vc4);
		if (ret)
			return ret;
		args->value = V3D_READ(V3D_IDENT0);
		vc4_v3d_pm_put(vc4);
		break;
	case DRM_VC4_PARAM_V3D_IDENT1:
		ret = vc4_v3d_pm_get(vc4);
		if (ret)
			return ret;
		args->value = V3D_READ(V3D_IDENT1);
		vc4_v3d_pm_put(vc4);
		break;
	case DRM_VC4_PARAM_V3D_IDENT2:
		ret = vc4_v3d_pm_get(vc4);
		if (ret)
			return ret;
		args->value = V3D_READ(V3D_IDENT2);
		vc4_v3d_pm_put(vc4);
>>>>>>> upstream/android-13
		break;
	case DRM_VC4_PARAM_SUPPORTS_BRANCHES:
	case DRM_VC4_PARAM_SUPPORTS_ETC1:
	case DRM_VC4_PARAM_SUPPORTS_THREADED_FS:
	case DRM_VC4_PARAM_SUPPORTS_FIXED_RCL_ORDER:
	case DRM_VC4_PARAM_SUPPORTS_MADVISE:
	case DRM_VC4_PARAM_SUPPORTS_PERFMON:
		args->value = true;
		break;
	default:
		DRM_DEBUG("Unknown parameter %d\n", args->param);
		return -EINVAL;
	}

	return 0;
}

static int vc4_open(struct drm_device *dev, struct drm_file *file)
{
	struct vc4_file *vc4file;

	vc4file = kzalloc(sizeof(*vc4file), GFP_KERNEL);
	if (!vc4file)
		return -ENOMEM;

	vc4_perfmon_open_file(vc4file);
	file->driver_priv = vc4file;
	return 0;
}

static void vc4_close(struct drm_device *dev, struct drm_file *file)
{
<<<<<<< HEAD
	struct vc4_file *vc4file = file->driver_priv;

=======
	struct vc4_dev *vc4 = to_vc4_dev(dev);
	struct vc4_file *vc4file = file->driver_priv;

	if (vc4file->bin_bo_used)
		vc4_v3d_bin_bo_put(vc4);

>>>>>>> upstream/android-13
	vc4_perfmon_close_file(vc4file);
	kfree(vc4file);
}

<<<<<<< HEAD
static const struct vm_operations_struct vc4_vm_ops = {
	.fault = vc4_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static const struct file_operations vc4_drm_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.mmap = vc4_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.compat_ioctl = drm_compat_ioctl,
	.llseek = noop_llseek,
};
=======
DEFINE_DRM_GEM_FOPS(vc4_drm_fops);
>>>>>>> upstream/android-13

static const struct drm_ioctl_desc vc4_drm_ioctls[] = {
	DRM_IOCTL_DEF_DRV(VC4_SUBMIT_CL, vc4_submit_cl_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_WAIT_SEQNO, vc4_wait_seqno_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_WAIT_BO, vc4_wait_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_CREATE_BO, vc4_create_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_MMAP_BO, vc4_mmap_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_CREATE_SHADER_BO, vc4_create_shader_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_GET_HANG_STATE, vc4_get_hang_state_ioctl,
			  DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(VC4_GET_PARAM, vc4_get_param_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_SET_TILING, vc4_set_tiling_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_GET_TILING, vc4_get_tiling_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_LABEL_BO, vc4_label_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_GEM_MADVISE, vc4_gem_madvise_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_PERFMON_CREATE, vc4_perfmon_create_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_PERFMON_DESTROY, vc4_perfmon_destroy_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VC4_PERFMON_GET_VALUES, vc4_perfmon_get_values_ioctl, DRM_RENDER_ALLOW),
};

static struct drm_driver vc4_drm_driver = {
	.driver_features = (DRIVER_MODESET |
			    DRIVER_ATOMIC |
			    DRIVER_GEM |
<<<<<<< HEAD
			    DRIVER_HAVE_IRQ |
			    DRIVER_RENDER |
			    DRIVER_PRIME |
			    DRIVER_SYNCOBJ),
	.lastclose = drm_fb_helper_lastclose,
	.open = vc4_open,
	.postclose = vc4_close,
	.irq_handler = vc4_irq,
	.irq_preinstall = vc4_irq_preinstall,
	.irq_postinstall = vc4_irq_postinstall,
	.irq_uninstall = vc4_irq_uninstall,

	.get_scanout_position = vc4_crtc_get_scanoutpos,
	.get_vblank_timestamp = drm_calc_vbltimestamp_from_scanoutpos,
=======
			    DRIVER_RENDER |
			    DRIVER_SYNCOBJ),
	.open = vc4_open,
	.postclose = vc4_close,
>>>>>>> upstream/android-13

#if defined(CONFIG_DEBUG_FS)
	.debugfs_init = vc4_debugfs_init,
#endif

	.gem_create_object = vc4_create_object,
<<<<<<< HEAD
	.gem_free_object_unlocked = vc4_free_object,
	.gem_vm_ops = &vc4_vm_ops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_import = drm_gem_prime_import,
	.gem_prime_export = vc4_prime_export,
	.gem_prime_res_obj = vc4_prime_res_obj,
	.gem_prime_get_sg_table	= drm_gem_cma_prime_get_sg_table,
	.gem_prime_import_sg_table = vc4_prime_import_sg_table,
	.gem_prime_vmap = vc4_prime_vmap,
	.gem_prime_vunmap = drm_gem_cma_prime_vunmap,
	.gem_prime_mmap = vc4_prime_mmap,

	.dumb_create = vc4_dumb_create,
=======

	DRM_GEM_CMA_DRIVER_OPS_WITH_DUMB_CREATE(vc4_dumb_create),
>>>>>>> upstream/android-13

	.ioctls = vc4_drm_ioctls,
	.num_ioctls = ARRAY_SIZE(vc4_drm_ioctls),
	.fops = &vc4_drm_fops,

	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

static int compare_dev(struct device *dev, void *data)
{
	return dev == data;
}

static void vc4_match_add_drivers(struct device *dev,
				  struct component_match **match,
				  struct platform_driver *const *drivers,
				  int count)
{
	int i;

	for (i = 0; i < count; i++) {
		struct device_driver *drv = &drivers[i]->driver;
		struct device *p = NULL, *d;

<<<<<<< HEAD
		while ((d = bus_find_device(&platform_bus_type, p, drv,
					    (void *)platform_bus_type.match))) {
=======
		while ((d = platform_find_device_by_driver(p, drv))) {
>>>>>>> upstream/android-13
			put_device(p);
			component_match_add(dev, match, compare_dev, d);
			p = d;
		}
		put_device(p);
	}
}

<<<<<<< HEAD
static void vc4_kick_out_firmware_fb(void)
{
	struct apertures_struct *ap;

	ap = alloc_apertures(1);
	if (!ap)
		return;

	/* Since VC4 is a UMA device, the simplefb node may have been
	 * located anywhere in memory.
	 */
	ap->ranges[0].base = 0;
	ap->ranges[0].size = ~0;

	drm_fb_helper_remove_conflicting_framebuffers(ap, "vc4drmfb", false);
	kfree(ap);
}

=======
>>>>>>> upstream/android-13
static int vc4_drm_bind(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct drm_device *drm;
	struct vc4_dev *vc4;
<<<<<<< HEAD
=======
	struct device_node *node;
	struct drm_crtc *crtc;
>>>>>>> upstream/android-13
	int ret = 0;

	dev->coherent_dma_mask = DMA_BIT_MASK(32);

<<<<<<< HEAD
	vc4 = devm_kzalloc(dev, sizeof(*vc4), GFP_KERNEL);
	if (!vc4)
		return -ENOMEM;

	drm = drm_dev_alloc(&vc4_drm_driver, dev);
	if (IS_ERR(drm))
		return PTR_ERR(drm);
	platform_set_drvdata(pdev, drm);
	vc4->dev = drm;
	drm->dev_private = vc4;

	ret = vc4_bo_cache_init(drm);
	if (ret)
		goto dev_put;

	drm_mode_config_init(drm);

	vc4_gem_init(drm);

	ret = component_bind_all(dev, drm);
	if (ret)
		goto gem_destroy;

	vc4_kick_out_firmware_fb();
=======
	/* If VC4 V3D is missing, don't advertise render nodes. */
	node = of_find_matching_node_and_match(NULL, vc4_v3d_dt_match, NULL);
	if (!node || !of_device_is_available(node))
		vc4_drm_driver.driver_features &= ~DRIVER_RENDER;
	of_node_put(node);

	vc4 = devm_drm_dev_alloc(dev, &vc4_drm_driver, struct vc4_dev, base);
	if (IS_ERR(vc4))
		return PTR_ERR(vc4);

	drm = &vc4->base;
	platform_set_drvdata(pdev, drm);
	INIT_LIST_HEAD(&vc4->debugfs_list);

	mutex_init(&vc4->bin_bo_lock);

	ret = vc4_bo_cache_init(drm);
	if (ret)
		return ret;

	ret = drmm_mode_config_init(drm);
	if (ret)
		return ret;

	ret = vc4_gem_init(drm);
	if (ret)
		return ret;

	ret = component_bind_all(dev, drm);
	if (ret)
		return ret;

	ret = vc4_plane_create_additional_planes(drm);
	if (ret)
		goto unbind_all;

	ret = drm_aperture_remove_framebuffers(false, &vc4_drm_driver);
	if (ret)
		goto unbind_all;

	ret = vc4_kms_load(drm);
	if (ret < 0)
		goto unbind_all;

	drm_for_each_crtc(crtc, drm)
		vc4_crtc_disable_at_boot(crtc);
>>>>>>> upstream/android-13

	ret = drm_dev_register(drm, 0);
	if (ret < 0)
		goto unbind_all;

<<<<<<< HEAD
	vc4_kms_load(drm);
=======
	drm_fbdev_generic_setup(drm, 16);
>>>>>>> upstream/android-13

	return 0;

unbind_all:
	component_unbind_all(dev, drm);
<<<<<<< HEAD
gem_destroy:
	vc4_gem_destroy(drm);
	drm_mode_config_cleanup(drm);
	vc4_bo_cache_destroy(drm);
dev_put:
	drm_dev_put(drm);
=======

>>>>>>> upstream/android-13
	return ret;
}

static void vc4_drm_unbind(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct vc4_dev *vc4 = to_vc4_dev(drm);

	drm_dev_unregister(drm);

	drm_fb_cma_fbdev_fini(drm);

	drm_mode_config_cleanup(drm);

	drm_atomic_private_obj_fini(&vc4->ctm_manager);

	drm_dev_put(drm);
=======

	drm_dev_unregister(drm);

	drm_atomic_helper_shutdown(drm);
>>>>>>> upstream/android-13
}

static const struct component_master_ops vc4_drm_ops = {
	.bind = vc4_drm_bind,
	.unbind = vc4_drm_unbind,
};

<<<<<<< HEAD
static struct platform_driver *const component_drivers[] = {
=======
/*
 * This list determines the binding order of our components, and we have
 * a few constraints:
 *   - The TXP driver needs to be bound before the PixelValves (CRTC)
 *     but after the HVS to set the possible_crtc field properly
 *   - The HDMI driver needs to be bound after the HVS so that we can
 *     lookup the HVS maximum core clock rate and figure out if we
 *     support 4kp60 or not.
 */
static struct platform_driver *const component_drivers[] = {
	&vc4_hvs_driver,
>>>>>>> upstream/android-13
	&vc4_hdmi_driver,
	&vc4_vec_driver,
	&vc4_dpi_driver,
	&vc4_dsi_driver,
	&vc4_txp_driver,
<<<<<<< HEAD
	&vc4_hvs_driver,
=======
>>>>>>> upstream/android-13
	&vc4_crtc_driver,
	&vc4_v3d_driver,
};

static int vc4_platform_drm_probe(struct platform_device *pdev)
{
	struct component_match *match = NULL;
	struct device *dev = &pdev->dev;

	vc4_match_add_drivers(dev, &match,
			      component_drivers, ARRAY_SIZE(component_drivers));

	return component_master_add_with_match(dev, &vc4_drm_ops, match);
}

static int vc4_platform_drm_remove(struct platform_device *pdev)
{
	component_master_del(&pdev->dev, &vc4_drm_ops);

	return 0;
}

static const struct of_device_id vc4_of_match[] = {
<<<<<<< HEAD
=======
	{ .compatible = "brcm,bcm2711-vc5", },
>>>>>>> upstream/android-13
	{ .compatible = "brcm,bcm2835-vc4", },
	{ .compatible = "brcm,cygnus-vc4", },
	{},
};
MODULE_DEVICE_TABLE(of, vc4_of_match);

static struct platform_driver vc4_platform_driver = {
	.probe		= vc4_platform_drm_probe,
	.remove		= vc4_platform_drm_remove,
	.driver		= {
		.name	= "vc4-drm",
		.of_match_table = vc4_of_match,
	},
};

static int __init vc4_drm_register(void)
{
	int ret;

	ret = platform_register_drivers(component_drivers,
					ARRAY_SIZE(component_drivers));
	if (ret)
		return ret;

	return platform_driver_register(&vc4_platform_driver);
}

static void __exit vc4_drm_unregister(void)
{
	platform_unregister_drivers(component_drivers,
				    ARRAY_SIZE(component_drivers));
	platform_driver_unregister(&vc4_platform_driver);
}

module_init(vc4_drm_register);
module_exit(vc4_drm_unregister);

MODULE_ALIAS("platform:vc4-drm");
MODULE_DESCRIPTION("Broadcom VC4 DRM Driver");
MODULE_AUTHOR("Eric Anholt <eric@anholt.net>");
MODULE_LICENSE("GPL v2");
