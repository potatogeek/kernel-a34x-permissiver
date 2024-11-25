/*
 * Copyright © 2016 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <linux/pm_domain.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD

#include "mock_engine.h"
#include "mock_context.h"
#include "mock_request.h"
#include "mock_gem_device.h"
#include "mock_gem_object.h"
#include "mock_gtt.h"
#include "mock_uncore.h"

void mock_device_flush(struct drm_i915_private *i915)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	lockdep_assert_held(&i915->drm.struct_mutex);

	for_each_engine(engine, i915, id)
		mock_engine_flush(engine);

	i915_retire_requests(i915);
	GEM_BUG_ON(i915->gt.active_requests);
=======
#include <linux/iommu.h>

#include <drm/drm_managed.h>

#include "gt/intel_gt.h"
#include "gt/intel_gt_requests.h"
#include "gt/mock_engine.h"
#include "intel_memory_region.h"
#include "intel_region_ttm.h"

#include "mock_request.h"
#include "mock_gem_device.h"
#include "mock_gtt.h"
#include "mock_uncore.h"
#include "mock_region.h"

#include "gem/selftests/mock_context.h"
#include "gem/selftests/mock_gem_object.h"

void mock_device_flush(struct drm_i915_private *i915)
{
	struct intel_gt *gt = &i915->gt;
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	do {
		for_each_engine(engine, gt, id)
			mock_engine_flush(engine);
	} while (intel_gt_retire_requests_timeout(gt, MAX_SCHEDULE_TIMEOUT,
						  NULL));
>>>>>>> upstream/android-13
}

static void mock_device_release(struct drm_device *dev)
{
	struct drm_i915_private *i915 = to_i915(dev);
<<<<<<< HEAD
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	mutex_lock(&i915->drm.struct_mutex);
	mock_device_flush(i915);
	i915_gem_contexts_lost(i915);
	mutex_unlock(&i915->drm.struct_mutex);

	cancel_delayed_work_sync(&i915->gt.retire_work);
	cancel_delayed_work_sync(&i915->gt.idle_work);
	i915_gem_drain_workqueue(i915);

	mutex_lock(&i915->drm.struct_mutex);
	for_each_engine(engine, i915, id)
		mock_engine_free(engine);
	i915_gem_contexts_fini(i915);
	mutex_unlock(&i915->drm.struct_mutex);

	drain_workqueue(i915->wq);
	i915_gem_drain_freed_objects(i915);

	mutex_lock(&i915->drm.struct_mutex);
	mock_fini_ggtt(i915);
	mutex_unlock(&i915->drm.struct_mutex);
	WARN_ON(!list_empty(&i915->gt.timelines));

	destroy_workqueue(i915->wq);

	kmem_cache_destroy(i915->priorities);
	kmem_cache_destroy(i915->dependencies);
	kmem_cache_destroy(i915->requests);
	kmem_cache_destroy(i915->vmas);
	kmem_cache_destroy(i915->objects);

	i915_gemfs_fini(i915);

	drm_mode_config_cleanup(&i915->drm);

	drm_dev_fini(&i915->drm);
	put_device(&i915->drm.pdev->dev);
}

static struct drm_driver mock_driver = {
	.name = "mock",
	.driver_features = DRIVER_GEM,
	.release = mock_device_release,

	.gem_close_object = i915_gem_close_object,
	.gem_free_object_unlocked = i915_gem_free_object,
=======

	if (!i915->do_release)
		goto out;

	mock_device_flush(i915);
	intel_gt_driver_remove(&i915->gt);

	i915_gem_drain_workqueue(i915);
	i915_gem_drain_freed_objects(i915);

	mock_fini_ggtt(&i915->ggtt);
	destroy_workqueue(i915->wq);

	intel_region_ttm_device_fini(i915);
	intel_gt_driver_late_release(&i915->gt);
	intel_memory_regions_driver_release(i915);

	drm_mode_config_cleanup(&i915->drm);

out:
	i915_params_free(&i915->params);
}

static const struct drm_driver mock_driver = {
	.name = "mock",
	.driver_features = DRIVER_GEM,
	.release = mock_device_release,
>>>>>>> upstream/android-13
};

static void release_dev(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);

	kfree(pdev);
}

<<<<<<< HEAD
static void mock_retire_work_handler(struct work_struct *work)
{
}

static void mock_idle_work_handler(struct work_struct *work)
{
}

=======
>>>>>>> upstream/android-13
static int pm_domain_resume(struct device *dev)
{
	return pm_generic_runtime_resume(dev);
}

static int pm_domain_suspend(struct device *dev)
{
	return pm_generic_runtime_suspend(dev);
}

static struct dev_pm_domain pm_domain = {
	.ops = {
		.runtime_suspend = pm_domain_suspend,
		.runtime_resume = pm_domain_resume,
	},
};

struct drm_i915_private *mock_gem_device(void)
{
<<<<<<< HEAD
	struct drm_i915_private *i915;
	struct pci_dev *pdev;
	int err;

	pdev = kzalloc(sizeof(*pdev) + sizeof(*i915), GFP_KERNEL);
	if (!pdev)
		goto err;

=======
#if IS_ENABLED(CONFIG_IOMMU_API) && defined(CONFIG_INTEL_IOMMU)
	static struct dev_iommu fake_iommu = { .priv = (void *)-1 };
#endif
	struct drm_i915_private *i915;
	struct pci_dev *pdev;
	int ret;

	pdev = kzalloc(sizeof(*pdev), GFP_KERNEL);
	if (!pdev)
		return NULL;
>>>>>>> upstream/android-13
	device_initialize(&pdev->dev);
	pdev->class = PCI_BASE_CLASS_DISPLAY << 16;
	pdev->dev.release = release_dev;
	dev_set_name(&pdev->dev, "mock");
<<<<<<< HEAD
	dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));

#if IS_ENABLED(CONFIG_IOMMU_API) && defined(CONFIG_INTEL_IOMMU)
	/* hack to disable iommu for the fake device; force identity mapping */
	pdev->dev.archdata.iommu = (void *)-1;
#endif
=======
	dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));

#if IS_ENABLED(CONFIG_IOMMU_API) && defined(CONFIG_INTEL_IOMMU)
	/* HACK to disable iommu for the fake device; force identity mapping */
	pdev->dev.iommu = &fake_iommu;
#endif
	if (!devres_open_group(&pdev->dev, NULL, GFP_KERNEL)) {
		put_device(&pdev->dev);
		return NULL;
	}

	i915 = devm_drm_dev_alloc(&pdev->dev, &mock_driver,
				  struct drm_i915_private, drm);
	if (IS_ERR(i915)) {
		pr_err("Failed to allocate mock GEM device: err=%ld\n", PTR_ERR(i915));
		devres_release_group(&pdev->dev, NULL);
		put_device(&pdev->dev);

		return NULL;
	}

	pci_set_drvdata(pdev, i915);
>>>>>>> upstream/android-13

	dev_pm_domain_set(&pdev->dev, &pm_domain);
	pm_runtime_enable(&pdev->dev);
	pm_runtime_dont_use_autosuspend(&pdev->dev);
	if (pm_runtime_enabled(&pdev->dev))
		WARN_ON(pm_runtime_get_sync(&pdev->dev));

<<<<<<< HEAD
	i915 = (struct drm_i915_private *)(pdev + 1);
	pci_set_drvdata(pdev, i915);

	err = drm_dev_init(&i915->drm, &mock_driver, &pdev->dev);
	if (err) {
		pr_err("Failed to initialise mock GEM device: err=%d\n", err);
		goto put_device;
	}
	i915->drm.pdev = pdev;
	i915->drm.dev_private = i915;
=======

	i915_params_copy(&i915->params, &i915_modparams);

	intel_runtime_pm_init_early(&i915->runtime_pm);
>>>>>>> upstream/android-13

	/* Using the global GTT may ask questions about KMS users, so prepare */
	drm_mode_config_init(&i915->drm);

<<<<<<< HEAD
	mkwrite_device_info(i915)->gen = -1;
=======
	mkwrite_device_info(i915)->graphics_ver = -1;
>>>>>>> upstream/android-13

	mkwrite_device_info(i915)->page_sizes =
		I915_GTT_PAGE_SIZE_4K |
		I915_GTT_PAGE_SIZE_64K |
		I915_GTT_PAGE_SIZE_2M;

<<<<<<< HEAD
	mock_uncore_init(i915);
	i915_gem_init__mm(i915);

	init_waitqueue_head(&i915->gpu_error.wait_queue);
	init_waitqueue_head(&i915->gpu_error.reset_queue);
=======
	mkwrite_device_info(i915)->memory_regions = REGION_SMEM;
	intel_memory_regions_hw_probe(i915);

	mock_uncore_init(&i915->uncore, i915);

	i915_gem_init__mm(i915);
	intel_gt_init_early(&i915->gt, i915);
	atomic_inc(&i915->gt.wakeref.count); /* disable; no hw support */
	i915->gt.awake = -ENODEV;

	ret = intel_region_ttm_device_init(i915);
	if (ret)
		goto err_ttm;
>>>>>>> upstream/android-13

	i915->wq = alloc_ordered_workqueue("mock", 0);
	if (!i915->wq)
		goto err_drv;

	mock_init_contexts(i915);

<<<<<<< HEAD
	INIT_DELAYED_WORK(&i915->gt.retire_work, mock_retire_work_handler);
	INIT_DELAYED_WORK(&i915->gt.idle_work, mock_idle_work_handler);

	i915->gt.awake = true;

	i915->objects = KMEM_CACHE(mock_object, SLAB_HWCACHE_ALIGN);
	if (!i915->objects)
		goto err_wq;

	i915->vmas = KMEM_CACHE(i915_vma, SLAB_HWCACHE_ALIGN);
	if (!i915->vmas)
		goto err_objects;

	i915->requests = KMEM_CACHE(mock_request,
				    SLAB_HWCACHE_ALIGN |
				    SLAB_RECLAIM_ACCOUNT |
				    SLAB_TYPESAFE_BY_RCU);
	if (!i915->requests)
		goto err_vmas;

	i915->dependencies = KMEM_CACHE(i915_dependency,
					SLAB_HWCACHE_ALIGN |
					SLAB_RECLAIM_ACCOUNT);
	if (!i915->dependencies)
		goto err_requests;

	i915->priorities = KMEM_CACHE(i915_priolist, SLAB_HWCACHE_ALIGN);
	if (!i915->priorities)
		goto err_dependencies;

	INIT_LIST_HEAD(&i915->gt.timelines);
	INIT_LIST_HEAD(&i915->gt.active_rings);
	INIT_LIST_HEAD(&i915->gt.closed_vma);

	mutex_lock(&i915->drm.struct_mutex);

	mock_init_ggtt(i915);

	mkwrite_device_info(i915)->ring_mask = BIT(0);
	i915->kernel_context = mock_context(i915, NULL);
	if (!i915->kernel_context)
		goto err_unlock;

	i915->engine[RCS] = mock_engine(i915, "mock", RCS);
	if (!i915->engine[RCS])
		goto err_context;

	mutex_unlock(&i915->drm.struct_mutex);

	WARN_ON(i915_gemfs_init(i915));
=======
	mock_init_ggtt(i915, &i915->ggtt);
	i915->gt.vm = i915_vm_get(&i915->ggtt.vm);

	mkwrite_device_info(i915)->platform_engine_mask = BIT(0);
	i915->gt.info.engine_mask = BIT(0);

	i915->gt.engine[RCS0] = mock_engine(i915, "mock", RCS0);
	if (!i915->gt.engine[RCS0])
		goto err_unlock;

	if (mock_engine_init(i915->gt.engine[RCS0]))
		goto err_context;

	__clear_bit(I915_WEDGED, &i915->gt.reset.flags);
	intel_engines_driver_register(i915);

	i915->do_release = true;
	ida_init(&i915->selftest.mock_region_instances);
>>>>>>> upstream/android-13

	return i915;

err_context:
<<<<<<< HEAD
	i915_gem_contexts_fini(i915);
err_unlock:
	mutex_unlock(&i915->drm.struct_mutex);
	kmem_cache_destroy(i915->priorities);
err_dependencies:
	kmem_cache_destroy(i915->dependencies);
err_requests:
	kmem_cache_destroy(i915->requests);
err_vmas:
	kmem_cache_destroy(i915->vmas);
err_objects:
	kmem_cache_destroy(i915->objects);
err_wq:
	destroy_workqueue(i915->wq);
err_drv:
	drm_mode_config_cleanup(&i915->drm);
	drm_dev_fini(&i915->drm);
put_device:
	put_device(&pdev->dev);
err:
	return NULL;
}
=======
	intel_gt_driver_remove(&i915->gt);
err_unlock:
	destroy_workqueue(i915->wq);
err_drv:
	intel_region_ttm_device_fini(i915);
err_ttm:
	intel_gt_driver_late_release(&i915->gt);
	intel_memory_regions_driver_release(i915);
	drm_mode_config_cleanup(&i915->drm);
	mock_destroy_device(i915);

	return NULL;
}

void mock_destroy_device(struct drm_i915_private *i915)
{
	struct device *dev = i915->drm.dev;

	devres_release_group(dev, NULL);
	put_device(dev);
}
>>>>>>> upstream/android-13
