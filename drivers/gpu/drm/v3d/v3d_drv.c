// SPDX-License-Identifier: GPL-2.0+
/* Copyright (C) 2014-2018 Broadcom */

/**
 * DOC: Broadcom V3D Graphics Driver
 *
 * This driver supports the Broadcom V3D 3.3 and 4.1 OpenGL ES GPUs.
 * For V3D 2.x support, see the VC4 driver.
 *
<<<<<<< HEAD
 * Currently only single-core rendering using the binner and renderer
 * is supported.  The TFU (texture formatting unit) and V3D 4.x's CSD
 * (compute shader dispatch) are not yet supported.
=======
 * The V3D GPU includes a tiled render (composed of a bin and render
 * pipelines), the TFU (texture formatting unit), and the CSD (compute
 * shader dispatch).
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
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

#include "uapi/drm/v3d_drm.h"
=======
#include <linux/reset.h>

#include <drm/drm_drv.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_managed.h>
#include <uapi/drm/v3d_drm.h>

>>>>>>> upstream/android-13
#include "v3d_drv.h"
#include "v3d_regs.h"

#define DRIVER_NAME "v3d"
#define DRIVER_DESC "Broadcom V3D graphics"
#define DRIVER_DATE "20180419"
#define DRIVER_MAJOR 1
#define DRIVER_MINOR 0
#define DRIVER_PATCHLEVEL 0

<<<<<<< HEAD
#ifdef CONFIG_PM
static int v3d_runtime_suspend(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);
	struct v3d_dev *v3d = to_v3d_dev(drm);

	v3d_irq_disable(v3d);

	clk_disable_unprepare(v3d->clk);

	return 0;
}

static int v3d_runtime_resume(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);
	struct v3d_dev *v3d = to_v3d_dev(drm);
	int ret;

	ret = clk_prepare_enable(v3d->clk);
	if (ret != 0)
		return ret;

	/* XXX: VPM base */

	v3d_mmu_set_page_table(v3d);
	v3d_irq_enable(v3d);

	return 0;
}
#endif

static const struct dev_pm_ops v3d_v3d_pm_ops = {
	SET_RUNTIME_PM_OPS(v3d_runtime_suspend, v3d_runtime_resume, NULL)
};

=======
>>>>>>> upstream/android-13
static int v3d_get_param_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file_priv)
{
	struct v3d_dev *v3d = to_v3d_dev(dev);
	struct drm_v3d_get_param *args = data;
	int ret;
	static const u32 reg_map[] = {
		[DRM_V3D_PARAM_V3D_UIFCFG] = V3D_HUB_UIFCFG,
		[DRM_V3D_PARAM_V3D_HUB_IDENT1] = V3D_HUB_IDENT1,
		[DRM_V3D_PARAM_V3D_HUB_IDENT2] = V3D_HUB_IDENT2,
		[DRM_V3D_PARAM_V3D_HUB_IDENT3] = V3D_HUB_IDENT3,
		[DRM_V3D_PARAM_V3D_CORE0_IDENT0] = V3D_CTL_IDENT0,
		[DRM_V3D_PARAM_V3D_CORE0_IDENT1] = V3D_CTL_IDENT1,
		[DRM_V3D_PARAM_V3D_CORE0_IDENT2] = V3D_CTL_IDENT2,
	};

	if (args->pad != 0)
		return -EINVAL;

	/* Note that DRM_V3D_PARAM_V3D_CORE0_IDENT0 is 0, so we need
	 * to explicitly allow it in the "the register in our
	 * parameter map" check.
	 */
	if (args->param < ARRAY_SIZE(reg_map) &&
	    (reg_map[args->param] ||
	     args->param == DRM_V3D_PARAM_V3D_CORE0_IDENT0)) {
		u32 offset = reg_map[args->param];

		if (args->value != 0)
			return -EINVAL;

<<<<<<< HEAD
		ret = pm_runtime_get_sync(v3d->dev);
=======
		ret = pm_runtime_get_sync(v3d->drm.dev);
		if (ret < 0)
			return ret;
>>>>>>> upstream/android-13
		if (args->param >= DRM_V3D_PARAM_V3D_CORE0_IDENT0 &&
		    args->param <= DRM_V3D_PARAM_V3D_CORE0_IDENT2) {
			args->value = V3D_CORE_READ(0, offset);
		} else {
			args->value = V3D_READ(offset);
		}
<<<<<<< HEAD
		pm_runtime_mark_last_busy(v3d->dev);
		pm_runtime_put_autosuspend(v3d->dev);
		return 0;
	}

	/* Any params that aren't just register reads would go here. */

	DRM_DEBUG("Unknown parameter %d\n", args->param);
	return -EINVAL;
=======
		pm_runtime_mark_last_busy(v3d->drm.dev);
		pm_runtime_put_autosuspend(v3d->drm.dev);
		return 0;
	}


	switch (args->param) {
	case DRM_V3D_PARAM_SUPPORTS_TFU:
		args->value = 1;
		return 0;
	case DRM_V3D_PARAM_SUPPORTS_CSD:
		args->value = v3d_has_csd(v3d);
		return 0;
	case DRM_V3D_PARAM_SUPPORTS_CACHE_FLUSH:
		args->value = 1;
		return 0;
	case DRM_V3D_PARAM_SUPPORTS_PERFMON:
		args->value = (v3d->ver >= 40);
		return 0;
	default:
		DRM_DEBUG("Unknown parameter %d\n", args->param);
		return -EINVAL;
	}
>>>>>>> upstream/android-13
}

static int
v3d_open(struct drm_device *dev, struct drm_file *file)
{
	struct v3d_dev *v3d = to_v3d_dev(dev);
	struct v3d_file_priv *v3d_priv;
<<<<<<< HEAD
	struct drm_sched_rq *rq;
=======
	struct drm_gpu_scheduler *sched;
>>>>>>> upstream/android-13
	int i;

	v3d_priv = kzalloc(sizeof(*v3d_priv), GFP_KERNEL);
	if (!v3d_priv)
		return -ENOMEM;

	v3d_priv->v3d = v3d;

	for (i = 0; i < V3D_MAX_QUEUES; i++) {
<<<<<<< HEAD
		rq = &v3d->queue[i].sched.sched_rq[DRM_SCHED_PRIORITY_NORMAL];
		drm_sched_entity_init(&v3d_priv->sched_entity[i], &rq, 1, NULL);
	}

=======
		sched = &v3d->queue[i].sched;
		drm_sched_entity_init(&v3d_priv->sched_entity[i],
				      DRM_SCHED_PRIORITY_NORMAL, &sched,
				      1, NULL);
	}

	v3d_perfmon_open_file(v3d_priv);
>>>>>>> upstream/android-13
	file->driver_priv = v3d_priv;

	return 0;
}

static void
v3d_postclose(struct drm_device *dev, struct drm_file *file)
{
	struct v3d_file_priv *v3d_priv = file->driver_priv;
	enum v3d_queue q;

	for (q = 0; q < V3D_MAX_QUEUES; q++) {
		drm_sched_entity_destroy(&v3d_priv->sched_entity[q]);
	}

<<<<<<< HEAD
	kfree(v3d_priv);
}

static const struct file_operations v3d_drm_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.mmap = v3d_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.compat_ioctl = drm_compat_ioctl,
	.llseek = noop_llseek,
};
=======
	v3d_perfmon_close_file(v3d_priv);
	kfree(v3d_priv);
}

DEFINE_DRM_GEM_FOPS(v3d_drm_fops);
>>>>>>> upstream/android-13

/* DRM_AUTH is required on SUBMIT_CL for now, while we don't have GMP
 * protection between clients.  Note that render nodes would be be
 * able to submit CLs that could access BOs from clients authenticated
<<<<<<< HEAD
 * with the master node.
=======
 * with the master node.  The TFU doesn't use the GMP, so it would
 * need to stay DRM_AUTH until we do buffer size/offset validation.
>>>>>>> upstream/android-13
 */
static const struct drm_ioctl_desc v3d_drm_ioctls[] = {
	DRM_IOCTL_DEF_DRV(V3D_SUBMIT_CL, v3d_submit_cl_ioctl, DRM_RENDER_ALLOW | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(V3D_WAIT_BO, v3d_wait_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(V3D_CREATE_BO, v3d_create_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(V3D_MMAP_BO, v3d_mmap_bo_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(V3D_GET_PARAM, v3d_get_param_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(V3D_GET_BO_OFFSET, v3d_get_bo_offset_ioctl, DRM_RENDER_ALLOW),
<<<<<<< HEAD
};

static const struct vm_operations_struct v3d_vm_ops = {
	.fault = v3d_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static struct drm_driver v3d_drm_driver = {
	.driver_features = (DRIVER_GEM |
			    DRIVER_RENDER |
			    DRIVER_PRIME |
=======
	DRM_IOCTL_DEF_DRV(V3D_SUBMIT_TFU, v3d_submit_tfu_ioctl, DRM_RENDER_ALLOW | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(V3D_SUBMIT_CSD, v3d_submit_csd_ioctl, DRM_RENDER_ALLOW | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(V3D_PERFMON_CREATE, v3d_perfmon_create_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(V3D_PERFMON_DESTROY, v3d_perfmon_destroy_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(V3D_PERFMON_GET_VALUES, v3d_perfmon_get_values_ioctl, DRM_RENDER_ALLOW),
};

static const struct drm_driver v3d_drm_driver = {
	.driver_features = (DRIVER_GEM |
			    DRIVER_RENDER |
>>>>>>> upstream/android-13
			    DRIVER_SYNCOBJ),

	.open = v3d_open,
	.postclose = v3d_postclose,

#if defined(CONFIG_DEBUG_FS)
	.debugfs_init = v3d_debugfs_init,
#endif

<<<<<<< HEAD
	.gem_free_object_unlocked = v3d_free_object,
	.gem_vm_ops = &v3d_vm_ops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_import = drm_gem_prime_import,
	.gem_prime_export = drm_gem_prime_export,
	.gem_prime_res_obj = v3d_prime_res_obj,
	.gem_prime_get_sg_table	= v3d_prime_get_sg_table,
	.gem_prime_import_sg_table = v3d_prime_import_sg_table,
	.gem_prime_mmap = v3d_prime_mmap,
=======
	.gem_create_object = v3d_create_object,
	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_import_sg_table = v3d_prime_import_sg_table,
	.gem_prime_mmap = drm_gem_prime_mmap,
>>>>>>> upstream/android-13

	.ioctls = v3d_drm_ioctls,
	.num_ioctls = ARRAY_SIZE(v3d_drm_ioctls),
	.fops = &v3d_drm_fops,

	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

static const struct of_device_id v3d_of_match[] = {
	{ .compatible = "brcm,7268-v3d" },
	{ .compatible = "brcm,7278-v3d" },
	{},
};
MODULE_DEVICE_TABLE(of, v3d_of_match);

static int
map_regs(struct v3d_dev *v3d, void __iomem **regs, const char *name)
{
	struct resource *res =
<<<<<<< HEAD
		platform_get_resource_byname(v3d->pdev, IORESOURCE_MEM, name);

	*regs = devm_ioremap_resource(v3d->dev, res);
=======
		platform_get_resource_byname(v3d_to_pdev(v3d), IORESOURCE_MEM, name);

	*regs = devm_ioremap_resource(v3d->drm.dev, res);
>>>>>>> upstream/android-13
	return PTR_ERR_OR_ZERO(*regs);
}

static int v3d_platform_drm_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct drm_device *drm;
	struct v3d_dev *v3d;
	int ret;
<<<<<<< HEAD
	u32 ident1;

	dev->coherent_dma_mask = DMA_BIT_MASK(36);

	v3d = kzalloc(sizeof(*v3d), GFP_KERNEL);
	if (!v3d)
		return -ENOMEM;
	v3d->dev = dev;
	v3d->pdev = pdev;
	drm = &v3d->drm;

	ret = map_regs(v3d, &v3d->bridge_regs, "bridge");
	if (ret)
		goto dev_free;

	ret = map_regs(v3d, &v3d->hub_regs, "hub");
	if (ret)
		goto dev_free;

	ret = map_regs(v3d, &v3d->core_regs[0], "core0");
	if (ret)
		goto dev_free;
=======
	u32 mmu_debug;
	u32 ident1;
	u64 mask;


	v3d = devm_drm_dev_alloc(dev, &v3d_drm_driver, struct v3d_dev, drm);
	if (IS_ERR(v3d))
		return PTR_ERR(v3d);

	drm = &v3d->drm;

	platform_set_drvdata(pdev, drm);

	ret = map_regs(v3d, &v3d->hub_regs, "hub");
	if (ret)
		return ret;

	ret = map_regs(v3d, &v3d->core_regs[0], "core0");
	if (ret)
		return ret;

	mmu_debug = V3D_READ(V3D_MMU_DEBUG_INFO);
	mask = DMA_BIT_MASK(30 + V3D_GET_FIELD(mmu_debug, V3D_MMU_PA_WIDTH));
	ret = dma_set_mask_and_coherent(dev, mask);
	if (ret)
		return ret;

	v3d->va_width = 30 + V3D_GET_FIELD(mmu_debug, V3D_MMU_VA_WIDTH);
>>>>>>> upstream/android-13

	ident1 = V3D_READ(V3D_HUB_IDENT1);
	v3d->ver = (V3D_GET_FIELD(ident1, V3D_HUB_IDENT1_TVER) * 10 +
		    V3D_GET_FIELD(ident1, V3D_HUB_IDENT1_REV));
	v3d->cores = V3D_GET_FIELD(ident1, V3D_HUB_IDENT1_NCORES);
	WARN_ON(v3d->cores > 1); /* multicore not yet implemented */

<<<<<<< HEAD
	if (v3d->ver < 41) {
		ret = map_regs(v3d, &v3d->gca_regs, "gca");
		if (ret)
			goto dev_free;
=======
	v3d->reset = devm_reset_control_get_exclusive(dev, NULL);
	if (IS_ERR(v3d->reset)) {
		ret = PTR_ERR(v3d->reset);

		if (ret == -EPROBE_DEFER)
			return ret;

		v3d->reset = NULL;
		ret = map_regs(v3d, &v3d->bridge_regs, "bridge");
		if (ret) {
			dev_err(dev,
				"Failed to get reset control or bridge regs\n");
			return ret;
		}
	}

	if (v3d->ver < 41) {
		ret = map_regs(v3d, &v3d->gca_regs, "gca");
		if (ret)
			return ret;
>>>>>>> upstream/android-13
	}

	v3d->mmu_scratch = dma_alloc_wc(dev, 4096, &v3d->mmu_scratch_paddr,
					GFP_KERNEL | __GFP_NOWARN | __GFP_ZERO);
	if (!v3d->mmu_scratch) {
		dev_err(dev, "Failed to allocate MMU scratch page\n");
<<<<<<< HEAD
		ret = -ENOMEM;
		goto dev_free;
=======
		return -ENOMEM;
>>>>>>> upstream/android-13
	}

	pm_runtime_use_autosuspend(dev);
	pm_runtime_set_autosuspend_delay(dev, 50);
	pm_runtime_enable(dev);

<<<<<<< HEAD
	ret = drm_dev_init(&v3d->drm, &v3d_drm_driver, dev);
	if (ret)
		goto dma_free;

	platform_set_drvdata(pdev, drm);
	drm->dev_private = v3d;

	ret = v3d_gem_init(drm);
	if (ret)
		goto dev_destroy;
=======
	ret = v3d_gem_init(drm);
	if (ret)
		goto dma_free;
>>>>>>> upstream/android-13

	ret = v3d_irq_init(v3d);
	if (ret)
		goto gem_destroy;

	ret = drm_dev_register(drm, 0);
	if (ret)
		goto irq_disable;

	return 0;

irq_disable:
	v3d_irq_disable(v3d);
gem_destroy:
	v3d_gem_destroy(drm);
<<<<<<< HEAD
dev_destroy:
	drm_dev_put(drm);
dma_free:
	dma_free_wc(dev, 4096, v3d->mmu_scratch, v3d->mmu_scratch_paddr);
dev_free:
	kfree(v3d);
=======
dma_free:
	dma_free_wc(dev, 4096, v3d->mmu_scratch, v3d->mmu_scratch_paddr);
>>>>>>> upstream/android-13
	return ret;
}

static int v3d_platform_drm_remove(struct platform_device *pdev)
{
	struct drm_device *drm = platform_get_drvdata(pdev);
	struct v3d_dev *v3d = to_v3d_dev(drm);

	drm_dev_unregister(drm);

	v3d_gem_destroy(drm);

<<<<<<< HEAD
	drm_dev_put(drm);

	dma_free_wc(v3d->dev, 4096, v3d->mmu_scratch, v3d->mmu_scratch_paddr);
=======
	dma_free_wc(v3d->drm.dev, 4096, v3d->mmu_scratch,
		    v3d->mmu_scratch_paddr);
>>>>>>> upstream/android-13

	return 0;
}

static struct platform_driver v3d_platform_driver = {
	.probe		= v3d_platform_drm_probe,
	.remove		= v3d_platform_drm_remove,
	.driver		= {
		.name	= "v3d",
		.of_match_table = v3d_of_match,
	},
};

<<<<<<< HEAD
static int __init v3d_drm_register(void)
{
	return platform_driver_register(&v3d_platform_driver);
}

static void __exit v3d_drm_unregister(void)
{
	platform_driver_unregister(&v3d_platform_driver);
}

module_init(v3d_drm_register);
module_exit(v3d_drm_unregister);
=======
module_platform_driver(v3d_platform_driver);
>>>>>>> upstream/android-13

MODULE_ALIAS("platform:v3d-drm");
MODULE_DESCRIPTION("Broadcom V3D DRM Driver");
MODULE_AUTHOR("Eric Anholt <eric@anholt.net>");
MODULE_LICENSE("GPL v2");
