// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) STMicroelectronics SA 2017
 *
 * Authors: Philippe Cornu <philippe.cornu@st.com>
 *          Yannick Fertre <yannick.fertre@st.com>
 *          Fabien Dessenne <fabien.dessenne@st.com>
 *          Mickael Reulier <mickael.reulier@st.com>
 */

#include <linux/component.h>
<<<<<<< HEAD
#include <linux/of_platform.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
=======
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "ltdc.h"

#define STM_MAX_FB_WIDTH	2048
#define STM_MAX_FB_HEIGHT	2048 /* same as width to handle orientation */

static const struct drm_mode_config_funcs drv_mode_config_funcs = {
	.fb_create = drm_gem_fb_create,
<<<<<<< HEAD
	.output_poll_changed = drm_fb_helper_output_poll_changed,
=======
>>>>>>> upstream/android-13
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
};

static int stm_gem_cma_dumb_create(struct drm_file *file,
				   struct drm_device *dev,
				   struct drm_mode_create_dumb *args)
{
	unsigned int min_pitch = DIV_ROUND_UP(args->width * args->bpp, 8);

	/*
	 * in order to optimize data transfer, pitch is aligned on
	 * 128 bytes, height is aligned on 4 bytes
	 */
	args->pitch = roundup(min_pitch, 128);
	args->height = roundup(args->height, 4);

	return drm_gem_cma_dumb_create_internal(file, dev, args);
}

DEFINE_DRM_GEM_CMA_FOPS(drv_driver_fops);

<<<<<<< HEAD
static struct drm_driver drv_driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_PRIME |
			   DRIVER_ATOMIC,
	.lastclose = drm_fb_helper_lastclose,
=======
static const struct drm_driver drv_driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_ATOMIC,
>>>>>>> upstream/android-13
	.name = "stm",
	.desc = "STMicroelectronics SoC DRM",
	.date = "20170330",
	.major = 1,
	.minor = 0,
	.patchlevel = 0,
	.fops = &drv_driver_fops,
<<<<<<< HEAD
	.dumb_create = stm_gem_cma_dumb_create,
	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_free_object_unlocked = drm_gem_cma_free_object,
	.gem_vm_ops = &drm_gem_cma_vm_ops,
	.gem_prime_export = drm_gem_prime_export,
	.gem_prime_import = drm_gem_prime_import,
	.gem_prime_get_sg_table = drm_gem_cma_prime_get_sg_table,
	.gem_prime_import_sg_table = drm_gem_cma_prime_import_sg_table,
	.gem_prime_vmap = drm_gem_cma_prime_vmap,
	.gem_prime_vunmap = drm_gem_cma_prime_vunmap,
	.gem_prime_mmap = drm_gem_cma_prime_mmap,
=======
	DRM_GEM_CMA_DRIVER_OPS_WITH_DUMB_CREATE(stm_gem_cma_dumb_create),
>>>>>>> upstream/android-13
};

static int drv_load(struct drm_device *ddev)
{
	struct platform_device *pdev = to_platform_device(ddev->dev);
	struct ltdc_device *ldev;
	int ret;

	DRM_DEBUG("%s\n", __func__);

	ldev = devm_kzalloc(ddev->dev, sizeof(*ldev), GFP_KERNEL);
	if (!ldev)
		return -ENOMEM;

	ddev->dev_private = (void *)ldev;

<<<<<<< HEAD
	drm_mode_config_init(ddev);
=======
	ret = drmm_mode_config_init(ddev);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/*
	 * set max width and height as default value.
	 * this value would be used to check framebuffer size limitation
	 * at drm_mode_addfb().
	 */
	ddev->mode_config.min_width = 0;
	ddev->mode_config.min_height = 0;
	ddev->mode_config.max_width = STM_MAX_FB_WIDTH;
	ddev->mode_config.max_height = STM_MAX_FB_HEIGHT;
	ddev->mode_config.funcs = &drv_mode_config_funcs;

	ret = ltdc_load(ddev);
	if (ret)
<<<<<<< HEAD
		goto err;
=======
		return ret;
>>>>>>> upstream/android-13

	drm_mode_config_reset(ddev);
	drm_kms_helper_poll_init(ddev);

<<<<<<< HEAD
	if (ddev->mode_config.num_connector) {
		ret = drm_fb_cma_fbdev_init(ddev, 16, 0);
		if (ret)
			DRM_DEBUG("Warning: fails to create fbdev\n");
	}

	platform_set_drvdata(pdev, ddev);

	return 0;
err:
	drm_mode_config_cleanup(ddev);
	return ret;
=======
	platform_set_drvdata(pdev, ddev);

	return 0;
>>>>>>> upstream/android-13
}

static void drv_unload(struct drm_device *ddev)
{
	DRM_DEBUG("%s\n", __func__);

<<<<<<< HEAD
	drm_fb_cma_fbdev_fini(ddev);
	drm_kms_helper_poll_fini(ddev);
	ltdc_unload(ddev);
	drm_mode_config_cleanup(ddev);
}

=======
	drm_kms_helper_poll_fini(ddev);
	ltdc_unload(ddev);
}

static __maybe_unused int drv_suspend(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);
	struct ltdc_device *ldev = ddev->dev_private;
	struct drm_atomic_state *state;

	WARN_ON(ldev->suspend_state);

	state = drm_atomic_helper_suspend(ddev);
	if (IS_ERR(state))
		return PTR_ERR(state);

	ldev->suspend_state = state;
	pm_runtime_force_suspend(dev);

	return 0;
}

static __maybe_unused int drv_resume(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);
	struct ltdc_device *ldev = ddev->dev_private;
	int ret;

	if (WARN_ON(!ldev->suspend_state))
		return -ENOENT;

	pm_runtime_force_resume(dev);
	ret = drm_atomic_helper_resume(ddev, ldev->suspend_state);
	if (ret)
		pm_runtime_force_suspend(dev);

	ldev->suspend_state = NULL;

	return ret;
}

static __maybe_unused int drv_runtime_suspend(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);

	DRM_DEBUG_DRIVER("\n");
	ltdc_suspend(ddev);

	return 0;
}

static __maybe_unused int drv_runtime_resume(struct device *dev)
{
	struct drm_device *ddev = dev_get_drvdata(dev);

	DRM_DEBUG_DRIVER("\n");
	return ltdc_resume(ddev);
}

static const struct dev_pm_ops drv_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(drv_suspend, drv_resume)
	SET_RUNTIME_PM_OPS(drv_runtime_suspend,
			   drv_runtime_resume, NULL)
};

>>>>>>> upstream/android-13
static int stm_drm_platform_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct drm_device *ddev;
	int ret;

	DRM_DEBUG("%s\n", __func__);

	dma_set_coherent_mask(dev, DMA_BIT_MASK(32));

	ddev = drm_dev_alloc(&drv_driver, dev);
	if (IS_ERR(ddev))
		return PTR_ERR(ddev);

	ret = drv_load(ddev);
	if (ret)
		goto err_put;

	ret = drm_dev_register(ddev, 0);
	if (ret)
		goto err_put;

<<<<<<< HEAD
=======
	drm_fbdev_generic_setup(ddev, 16);

>>>>>>> upstream/android-13
	return 0;

err_put:
	drm_dev_put(ddev);

	return ret;
}

static int stm_drm_platform_remove(struct platform_device *pdev)
{
	struct drm_device *ddev = platform_get_drvdata(pdev);

	DRM_DEBUG("%s\n", __func__);

	drm_dev_unregister(ddev);
	drv_unload(ddev);
	drm_dev_put(ddev);

	return 0;
}

static const struct of_device_id drv_dt_ids[] = {
	{ .compatible = "st,stm32-ltdc"},
	{ /* end node */ },
};
MODULE_DEVICE_TABLE(of, drv_dt_ids);

static struct platform_driver stm_drm_platform_driver = {
	.probe = stm_drm_platform_probe,
	.remove = stm_drm_platform_remove,
	.driver = {
		.name = "stm32-display",
		.of_match_table = drv_dt_ids,
<<<<<<< HEAD
=======
		.pm = &drv_pm_ops,
>>>>>>> upstream/android-13
	},
};

module_platform_driver(stm_drm_platform_driver);

MODULE_AUTHOR("Philippe Cornu <philippe.cornu@st.com>");
MODULE_AUTHOR("Yannick Fertre <yannick.fertre@st.com>");
MODULE_AUTHOR("Fabien Dessenne <fabien.dessenne@st.com>");
MODULE_AUTHOR("Mickael Reulier <mickael.reulier@st.com>");
MODULE_DESCRIPTION("STMicroelectronics ST DRM LTDC driver");
MODULE_LICENSE("GPL v2");
