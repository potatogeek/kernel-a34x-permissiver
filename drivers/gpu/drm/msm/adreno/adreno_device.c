<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2013-2014 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * Copyright (c) 2014,2017 The Linux Foundation. All rights reserved.
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 */

#include "adreno_gpu.h"

<<<<<<< HEAD
#define ANY_ID 0xff

=======
>>>>>>> upstream/android-13
bool hang_debug = false;
MODULE_PARM_DESC(hang_debug, "Dump registers when hang is detected (can be slow!)");
module_param_named(hang_debug, hang_debug, bool, 0600);

<<<<<<< HEAD
static const struct adreno_info gpulist[] = {
	{
=======
bool snapshot_debugbus = false;
MODULE_PARM_DESC(snapshot_debugbus, "Include debugbus sections in GPU devcoredump (if not fused off)");
module_param_named(snapshot_debugbus, snapshot_debugbus, bool, 0600);

bool allow_vram_carveout = false;
MODULE_PARM_DESC(allow_vram_carveout, "Allow using VRAM Carveout, in place of IOMMU");
module_param_named(allow_vram_carveout, allow_vram_carveout, bool, 0600);

static const struct adreno_info gpulist[] = {
	{
		.rev   = ADRENO_REV(2, 0, 0, 0),
		.revn  = 200,
		.name  = "A200",
		.fw = {
			[ADRENO_FW_PM4] = "yamato_pm4.fw",
			[ADRENO_FW_PFP] = "yamato_pfp.fw",
		},
		.gmem  = SZ_256K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a2xx_gpu_init,
	}, { /* a200 on i.mx51 has only 128kib gmem */
		.rev   = ADRENO_REV(2, 0, 0, 1),
		.revn  = 201,
		.name  = "A200",
		.fw = {
			[ADRENO_FW_PM4] = "yamato_pm4.fw",
			[ADRENO_FW_PFP] = "yamato_pfp.fw",
		},
		.gmem  = SZ_128K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a2xx_gpu_init,
	}, {
		.rev   = ADRENO_REV(2, 2, 0, ANY_ID),
		.revn  = 220,
		.name  = "A220",
		.fw = {
			[ADRENO_FW_PM4] = "leia_pm4_470.fw",
			[ADRENO_FW_PFP] = "leia_pfp_470.fw",
		},
		.gmem  = SZ_512K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a2xx_gpu_init,
	}, {
>>>>>>> upstream/android-13
		.rev   = ADRENO_REV(3, 0, 5, ANY_ID),
		.revn  = 305,
		.name  = "A305",
		.fw = {
			[ADRENO_FW_PM4] = "a300_pm4.fw",
			[ADRENO_FW_PFP] = "a300_pfp.fw",
		},
		.gmem  = SZ_256K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a3xx_gpu_init,
	}, {
		.rev   = ADRENO_REV(3, 0, 6, 0),
		.revn  = 307,        /* because a305c is revn==306 */
		.name  = "A306",
		.fw = {
			[ADRENO_FW_PM4] = "a300_pm4.fw",
			[ADRENO_FW_PFP] = "a300_pfp.fw",
		},
		.gmem  = SZ_128K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a3xx_gpu_init,
	}, {
		.rev   = ADRENO_REV(3, 2, ANY_ID, ANY_ID),
		.revn  = 320,
		.name  = "A320",
		.fw = {
			[ADRENO_FW_PM4] = "a300_pm4.fw",
			[ADRENO_FW_PFP] = "a300_pfp.fw",
		},
		.gmem  = SZ_512K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a3xx_gpu_init,
	}, {
		.rev   = ADRENO_REV(3, 3, 0, ANY_ID),
		.revn  = 330,
		.name  = "A330",
		.fw = {
			[ADRENO_FW_PM4] = "a330_pm4.fw",
			[ADRENO_FW_PFP] = "a330_pfp.fw",
		},
		.gmem  = SZ_1M,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a3xx_gpu_init,
	}, {
<<<<<<< HEAD
=======
		.rev   = ADRENO_REV(4, 0, 5, ANY_ID),
		.revn  = 405,
		.name  = "A405",
		.fw = {
			[ADRENO_FW_PM4] = "a420_pm4.fw",
			[ADRENO_FW_PFP] = "a420_pfp.fw",
		},
		.gmem  = SZ_256K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a4xx_gpu_init,
	}, {
>>>>>>> upstream/android-13
		.rev   = ADRENO_REV(4, 2, 0, ANY_ID),
		.revn  = 420,
		.name  = "A420",
		.fw = {
			[ADRENO_FW_PM4] = "a420_pm4.fw",
			[ADRENO_FW_PFP] = "a420_pfp.fw",
		},
		.gmem  = (SZ_1M + SZ_512K),
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a4xx_gpu_init,
	}, {
		.rev   = ADRENO_REV(4, 3, 0, ANY_ID),
		.revn  = 430,
		.name  = "A430",
		.fw = {
			[ADRENO_FW_PM4] = "a420_pm4.fw",
			[ADRENO_FW_PFP] = "a420_pfp.fw",
		},
		.gmem  = (SZ_1M + SZ_512K),
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init  = a4xx_gpu_init,
	}, {
<<<<<<< HEAD
=======
		.rev   = ADRENO_REV(5, 0, 8, ANY_ID),
		.revn = 508,
		.name = "A508",
		.fw = {
			[ADRENO_FW_PM4] = "a530_pm4.fw",
			[ADRENO_FW_PFP] = "a530_pfp.fw",
		},
		.gmem = (SZ_128K + SZ_8K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "a508_zap.mdt",
	}, {
		.rev   = ADRENO_REV(5, 0, 9, ANY_ID),
		.revn = 509,
		.name = "A509",
		.fw = {
			[ADRENO_FW_PM4] = "a530_pm4.fw",
			[ADRENO_FW_PFP] = "a530_pfp.fw",
		},
		.gmem = (SZ_256K + SZ_16K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		/* Adreno 509 uses the same ZAP as 512 */
		.zapfw = "a512_zap.mdt",
	}, {
		.rev   = ADRENO_REV(5, 1, 0, ANY_ID),
		.revn = 510,
		.name = "A510",
		.fw = {
			[ADRENO_FW_PM4] = "a530_pm4.fw",
			[ADRENO_FW_PFP] = "a530_pfp.fw",
		},
		.gmem = SZ_256K,
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.init = a5xx_gpu_init,
	}, {
		.rev   = ADRENO_REV(5, 1, 2, ANY_ID),
		.revn = 512,
		.name = "A512",
		.fw = {
			[ADRENO_FW_PM4] = "a530_pm4.fw",
			[ADRENO_FW_PFP] = "a530_pfp.fw",
		},
		.gmem = (SZ_256K + SZ_16K),
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "a512_zap.mdt",
	}, {
>>>>>>> upstream/android-13
		.rev = ADRENO_REV(5, 3, 0, 2),
		.revn = 530,
		.name = "A530",
		.fw = {
			[ADRENO_FW_PM4] = "a530_pm4.fw",
			[ADRENO_FW_PFP] = "a530_pfp.fw",
			[ADRENO_FW_GPMU] = "a530v3_gpmu.fw2",
		},
		.gmem = SZ_1M,
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_TWO_PASS_USE_WFI |
			ADRENO_QUIRK_FAULT_DETECT_MASK,
		.init = a5xx_gpu_init,
		.zapfw = "a530_zap.mdt",
	}, {
<<<<<<< HEAD
=======
		.rev = ADRENO_REV(5, 4, 0, ANY_ID),
		.revn = 540,
		.name = "A540",
		.fw = {
			[ADRENO_FW_PM4] = "a530_pm4.fw",
			[ADRENO_FW_PFP] = "a530_pfp.fw",
			[ADRENO_FW_GPMU] = "a540_gpmu.fw2",
		},
		.gmem = SZ_1M,
		/*
		 * Increase inactive period to 250 to avoid bouncing
		 * the GDSC which appears to make it grumpy
		 */
		.inactive_period = 250,
		.quirks = ADRENO_QUIRK_LMLOADKILL_DISABLE,
		.init = a5xx_gpu_init,
		.zapfw = "a540_zap.mdt",
	}, {
		.rev = ADRENO_REV(6, 1, 8, ANY_ID),
		.revn = 618,
		.name = "A618",
		.fw = {
			[ADRENO_FW_SQE] = "a630_sqe.fw",
			[ADRENO_FW_GMU] = "a630_gmu.bin",
		},
		.gmem = SZ_512K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
	}, {
>>>>>>> upstream/android-13
		.rev = ADRENO_REV(6, 3, 0, ANY_ID),
		.revn = 630,
		.name = "A630",
		.fw = {
			[ADRENO_FW_SQE] = "a630_sqe.fw",
			[ADRENO_FW_GMU] = "a630_gmu.bin",
		},
		.gmem = SZ_1M,
<<<<<<< HEAD
		.init = a6xx_gpu_init,
=======
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
		.zapfw = "a630_zap.mdt",
		.hwcg = a630_hwcg,
	}, {
		.rev = ADRENO_REV(6, 4, 0, ANY_ID),
		.revn = 640,
		.name = "A640",
		.fw = {
			[ADRENO_FW_SQE] = "a630_sqe.fw",
			[ADRENO_FW_GMU] = "a640_gmu.bin",
		},
		.gmem = SZ_1M,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
		.zapfw = "a640_zap.mdt",
		.hwcg = a640_hwcg,
	}, {
		.rev = ADRENO_REV(6, 5, 0, ANY_ID),
		.revn = 650,
		.name = "A650",
		.fw = {
			[ADRENO_FW_SQE] = "a650_sqe.fw",
			[ADRENO_FW_GMU] = "a650_gmu.bin",
		},
		.gmem = SZ_1M + SZ_128K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
		.zapfw = "a650_zap.mdt",
		.hwcg = a650_hwcg,
	}, {
		.rev = ADRENO_REV(6, 6, 0, ANY_ID),
		.revn = 660,
		.name = "A660",
		.fw = {
			[ADRENO_FW_SQE] = "a660_sqe.fw",
			[ADRENO_FW_GMU] = "a660_gmu.bin",
		},
		.gmem = SZ_1M + SZ_512K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
		.zapfw = "a660_zap.mdt",
		.hwcg = a660_hwcg,
	}, {
		.rev = ADRENO_REV(6, 3, 5, ANY_ID),
		.name = "Adreno 7c Gen 3",
		.fw = {
			[ADRENO_FW_SQE] = "a660_sqe.fw",
			[ADRENO_FW_GMU] = "a660_gmu.bin",
		},
		.gmem = SZ_512K,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
		.hwcg = a660_hwcg,
	}, {
		.rev = ADRENO_REV(6, 8, 0, ANY_ID),
		.revn = 680,
		.name = "A680",
		.fw = {
			[ADRENO_FW_SQE] = "a630_sqe.fw",
			[ADRENO_FW_GMU] = "a640_gmu.bin",
		},
		.gmem = SZ_2M,
		.inactive_period = DRM_MSM_INACTIVE_PERIOD,
		.init = a6xx_gpu_init,
		.zapfw = "a640_zap.mdt",
		.hwcg = a640_hwcg,
>>>>>>> upstream/android-13
	},
};

MODULE_FIRMWARE("qcom/a300_pm4.fw");
MODULE_FIRMWARE("qcom/a300_pfp.fw");
MODULE_FIRMWARE("qcom/a330_pm4.fw");
MODULE_FIRMWARE("qcom/a330_pfp.fw");
MODULE_FIRMWARE("qcom/a420_pm4.fw");
MODULE_FIRMWARE("qcom/a420_pfp.fw");
MODULE_FIRMWARE("qcom/a530_pm4.fw");
MODULE_FIRMWARE("qcom/a530_pfp.fw");
MODULE_FIRMWARE("qcom/a530v3_gpmu.fw2");
MODULE_FIRMWARE("qcom/a530_zap.mdt");
MODULE_FIRMWARE("qcom/a530_zap.b00");
MODULE_FIRMWARE("qcom/a530_zap.b01");
MODULE_FIRMWARE("qcom/a530_zap.b02");
MODULE_FIRMWARE("qcom/a630_sqe.fw");
MODULE_FIRMWARE("qcom/a630_gmu.bin");
<<<<<<< HEAD
=======
MODULE_FIRMWARE("qcom/a630_zap.mbn");
>>>>>>> upstream/android-13

static inline bool _rev_match(uint8_t entry, uint8_t id)
{
	return (entry == ANY_ID) || (entry == id);
}

<<<<<<< HEAD
=======
bool adreno_cmp_rev(struct adreno_rev rev1, struct adreno_rev rev2)
{

	return _rev_match(rev1.core, rev2.core) &&
		_rev_match(rev1.major, rev2.major) &&
		_rev_match(rev1.minor, rev2.minor) &&
		_rev_match(rev1.patchid, rev2.patchid);
}

>>>>>>> upstream/android-13
const struct adreno_info *adreno_info(struct adreno_rev rev)
{
	int i;

	/* identify gpu: */
	for (i = 0; i < ARRAY_SIZE(gpulist); i++) {
		const struct adreno_info *info = &gpulist[i];
<<<<<<< HEAD
		if (_rev_match(info->rev.core, rev.core) &&
				_rev_match(info->rev.major, rev.major) &&
				_rev_match(info->rev.minor, rev.minor) &&
				_rev_match(info->rev.patchid, rev.patchid))
=======
		if (adreno_cmp_rev(info->rev, rev))
>>>>>>> upstream/android-13
			return info;
	}

	return NULL;
}

struct msm_gpu *adreno_load_gpu(struct drm_device *dev)
{
	struct msm_drm_private *priv = dev->dev_private;
	struct platform_device *pdev = priv->gpu_pdev;
	struct msm_gpu *gpu = NULL;
	struct adreno_gpu *adreno_gpu;
	int ret;

	if (pdev)
<<<<<<< HEAD
		gpu = platform_get_drvdata(pdev);
=======
		gpu = dev_to_gpu(&pdev->dev);
>>>>>>> upstream/android-13

	if (!gpu) {
		dev_err_once(dev->dev, "no GPU device was found\n");
		return NULL;
	}

	adreno_gpu = to_adreno_gpu(gpu);

	/*
	 * The number one reason for HW init to fail is if the firmware isn't
	 * loaded yet. Try that first and don't bother continuing on
	 * otherwise
	 */

	ret = adreno_load_fw(adreno_gpu);
	if (ret)
		return NULL;

	/* Make sure pm runtime is active and reset any previous errors */
	pm_runtime_set_active(&pdev->dev);

	ret = pm_runtime_get_sync(&pdev->dev);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(dev->dev, "Couldn't power up the GPU: %d\n", ret);
=======
		pm_runtime_put_sync(&pdev->dev);
		DRM_DEV_ERROR(dev->dev, "Couldn't power up the GPU: %d\n", ret);
>>>>>>> upstream/android-13
		return NULL;
	}

	mutex_lock(&dev->struct_mutex);
	ret = msm_gpu_hw_init(gpu);
	mutex_unlock(&dev->struct_mutex);
	pm_runtime_put_autosuspend(&pdev->dev);
	if (ret) {
<<<<<<< HEAD
		dev_err(dev->dev, "gpu hw init failed: %d\n", ret);
=======
		DRM_DEV_ERROR(dev->dev, "gpu hw init failed: %d\n", ret);
>>>>>>> upstream/android-13
		return NULL;
	}

#ifdef CONFIG_DEBUG_FS
	if (gpu->funcs->debugfs_init) {
		gpu->funcs->debugfs_init(gpu, dev->primary);
		gpu->funcs->debugfs_init(gpu, dev->render);
	}
#endif

	return gpu;
}

static void set_gpu_pdev(struct drm_device *dev,
		struct platform_device *pdev)
{
	struct msm_drm_private *priv = dev->dev_private;
	priv->gpu_pdev = pdev;
}

static int find_chipid(struct device *dev, struct adreno_rev *rev)
{
	struct device_node *node = dev->of_node;
	const char *compat;
	int ret;
	u32 chipid;

	/* first search the compat strings for qcom,adreno-XYZ.W: */
	ret = of_property_read_string_index(node, "compatible", 0, &compat);
	if (ret == 0) {
		unsigned int r, patch;

<<<<<<< HEAD
		if (sscanf(compat, "qcom,adreno-%u.%u", &r, &patch) == 2) {
=======
		if (sscanf(compat, "qcom,adreno-%u.%u", &r, &patch) == 2 ||
		    sscanf(compat, "amd,imageon-%u.%u", &r, &patch) == 2) {
>>>>>>> upstream/android-13
			rev->core = r / 100;
			r %= 100;
			rev->major = r / 10;
			r %= 10;
			rev->minor = r;
			rev->patchid = patch;

			return 0;
		}
	}

	/* and if that fails, fall back to legacy "qcom,chipid" property: */
	ret = of_property_read_u32(node, "qcom,chipid", &chipid);
	if (ret) {
<<<<<<< HEAD
		dev_err(dev, "could not parse qcom,chipid: %d\n", ret);
=======
		DRM_DEV_ERROR(dev, "could not parse qcom,chipid: %d\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	rev->core = (chipid >> 24) & 0xff;
	rev->major = (chipid >> 16) & 0xff;
	rev->minor = (chipid >> 8) & 0xff;
	rev->patchid = (chipid & 0xff);

	dev_warn(dev, "Using legacy qcom,chipid binding!\n");
	dev_warn(dev, "Use compatible qcom,adreno-%u%u%u.%u instead.\n",
		rev->core, rev->major, rev->minor, rev->patchid);

	return 0;
}

static int adreno_bind(struct device *dev, struct device *master, void *data)
{
	static struct adreno_platform_config config = {};
	const struct adreno_info *info;
	struct drm_device *drm = dev_get_drvdata(master);
<<<<<<< HEAD
=======
	struct msm_drm_private *priv = drm->dev_private;
>>>>>>> upstream/android-13
	struct msm_gpu *gpu;
	int ret;

	ret = find_chipid(dev, &config.rev);
	if (ret)
		return ret;

	dev->platform_data = &config;
	set_gpu_pdev(drm, to_platform_device(dev));

	info = adreno_info(config.rev);

	if (!info) {
		dev_warn(drm->dev, "Unknown GPU revision: %u.%u.%u.%u\n",
			config.rev.core, config.rev.major,
			config.rev.minor, config.rev.patchid);
		return -ENXIO;
	}

	DBG("Found GPU: %u.%u.%u.%u", config.rev.core, config.rev.major,
		config.rev.minor, config.rev.patchid);

<<<<<<< HEAD
=======
	priv->is_a2xx = config.rev.core == 2;
	priv->has_cached_coherent = config.rev.core >= 6;

>>>>>>> upstream/android-13
	gpu = info->init(drm);
	if (IS_ERR(gpu)) {
		dev_warn(drm->dev, "failed to load adreno gpu\n");
		return PTR_ERR(gpu);
	}

<<<<<<< HEAD
	dev_set_drvdata(dev, gpu);

=======
>>>>>>> upstream/android-13
	return 0;
}

static void adreno_unbind(struct device *dev, struct device *master,
		void *data)
{
<<<<<<< HEAD
	struct msm_gpu *gpu = dev_get_drvdata(dev);

	gpu->funcs->pm_suspend(gpu);
=======
	struct msm_gpu *gpu = dev_to_gpu(dev);

	pm_runtime_force_suspend(dev);
>>>>>>> upstream/android-13
	gpu->funcs->destroy(gpu);

	set_gpu_pdev(dev_get_drvdata(master), NULL);
}

static const struct component_ops a3xx_ops = {
		.bind   = adreno_bind,
		.unbind = adreno_unbind,
};

<<<<<<< HEAD
static int adreno_probe(struct platform_device *pdev)
{
	return component_add(&pdev->dev, &a3xx_ops);
=======
static void adreno_device_register_headless(void)
{
	/* on imx5, we don't have a top-level mdp/dpu node
	 * this creates a dummy node for the driver for that case
	 */
	struct platform_device_info dummy_info = {
		.parent = NULL,
		.name = "msm",
		.id = -1,
		.res = NULL,
		.num_res = 0,
		.data = NULL,
		.size_data = 0,
		.dma_mask = ~0,
	};
	platform_device_register_full(&dummy_info);
}

static int adreno_probe(struct platform_device *pdev)
{

	int ret;

	ret = component_add(&pdev->dev, &a3xx_ops);
	if (ret)
		return ret;

	if (of_device_is_compatible(pdev->dev.of_node, "amd,imageon"))
		adreno_device_register_headless();

	return 0;
>>>>>>> upstream/android-13
}

static int adreno_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &a3xx_ops);
	return 0;
}

<<<<<<< HEAD
static const struct of_device_id dt_match[] = {
	{ .compatible = "qcom,adreno" },
	{ .compatible = "qcom,adreno-3xx" },
=======
static void adreno_shutdown(struct platform_device *pdev)
{
	pm_runtime_force_suspend(&pdev->dev);
}

static const struct of_device_id dt_match[] = {
	{ .compatible = "qcom,adreno" },
	{ .compatible = "qcom,adreno-3xx" },
	/* for compatibility with imx5 gpu: */
	{ .compatible = "amd,imageon" },
>>>>>>> upstream/android-13
	/* for backwards compat w/ downstream kgsl DT files: */
	{ .compatible = "qcom,kgsl-3d0" },
	{}
};

#ifdef CONFIG_PM
static int adreno_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct msm_gpu *gpu = platform_get_drvdata(pdev);
=======
	struct msm_gpu *gpu = dev_to_gpu(dev);
>>>>>>> upstream/android-13

	return gpu->funcs->pm_resume(gpu);
}

static int adreno_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct msm_gpu *gpu = platform_get_drvdata(pdev);
=======
	struct msm_gpu *gpu = dev_to_gpu(dev);
>>>>>>> upstream/android-13

	return gpu->funcs->pm_suspend(gpu);
}
#endif

static const struct dev_pm_ops adreno_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend, pm_runtime_force_resume)
	SET_RUNTIME_PM_OPS(adreno_suspend, adreno_resume, NULL)
};

static struct platform_driver adreno_driver = {
	.probe = adreno_probe,
	.remove = adreno_remove,
<<<<<<< HEAD
=======
	.shutdown = adreno_shutdown,
>>>>>>> upstream/android-13
	.driver = {
		.name = "adreno",
		.of_match_table = dt_match,
		.pm = &adreno_pm_ops,
	},
};

void __init adreno_register(void)
{
	platform_driver_register(&adreno_driver);
}

void __exit adreno_unregister(void)
{
	platform_driver_unregister(&adreno_driver);
}
