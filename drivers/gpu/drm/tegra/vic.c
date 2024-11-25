<<<<<<< HEAD
/*
 * Copyright (c) 2015, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015, NVIDIA Corporation.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
>>>>>>> upstream/android-13
#include <linux/host1x.h>
#include <linux/iommu.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>

#include <soc/tegra/pmc.h>

#include "drm.h"
#include "falcon.h"
#include "vic.h"

struct vic_config {
	const char *firmware;
	unsigned int version;
<<<<<<< HEAD
=======
	bool supports_sid;
>>>>>>> upstream/android-13
};

struct vic {
	struct falcon falcon;
<<<<<<< HEAD
	bool booted;
=======
>>>>>>> upstream/android-13

	void __iomem *regs;
	struct tegra_drm_client client;
	struct host1x_channel *channel;
<<<<<<< HEAD
	struct iommu_domain *domain;
	struct device *dev;
	struct clk *clk;
=======
	struct device *dev;
	struct clk *clk;
	struct reset_control *rst;
>>>>>>> upstream/android-13

	/* Platform configuration */
	const struct vic_config *config;
};

static inline struct vic *to_vic(struct tegra_drm_client *client)
{
	return container_of(client, struct vic, client);
}

static void vic_writel(struct vic *vic, u32 value, unsigned int offset)
{
	writel(value, vic->regs + offset);
}

<<<<<<< HEAD
static int vic_runtime_resume(struct device *dev)
{
	struct vic *vic = dev_get_drvdata(dev);

	return clk_prepare_enable(vic->clk);
}

static int vic_runtime_suspend(struct device *dev)
{
	struct vic *vic = dev_get_drvdata(dev);

	clk_disable_unprepare(vic->clk);

	vic->booted = false;

	return 0;
}

static int vic_boot(struct vic *vic)
{
=======
static int vic_boot(struct vic *vic)
{
#ifdef CONFIG_IOMMU_API
	struct iommu_fwspec *spec = dev_iommu_fwspec_get(vic->dev);
#endif
>>>>>>> upstream/android-13
	u32 fce_ucode_size, fce_bin_data_offset;
	void *hdr;
	int err = 0;

<<<<<<< HEAD
	if (vic->booted)
		return 0;
=======
#ifdef CONFIG_IOMMU_API
	if (vic->config->supports_sid && spec) {
		u32 value;

		value = TRANSCFG_ATT(1, TRANSCFG_SID_FALCON) |
			TRANSCFG_ATT(0, TRANSCFG_SID_HW);
		vic_writel(vic, value, VIC_TFBIF_TRANSCFG);

		if (spec->num_ids > 0) {
			value = spec->ids[0] & 0xffff;

			/*
			 * STREAMID0 is used for input/output buffers.
			 * Initialize it to SID_VIC in case context isolation
			 * is not enabled, and SID_VIC is used for both firmware
			 * and data buffers.
			 *
			 * If context isolation is enabled, it will be
			 * overridden by the SETSTREAMID opcode as part of
			 * each job.
			 */
			vic_writel(vic, value, VIC_THI_STREAMID0);

			/* STREAMID1 is used for firmware loading. */
			vic_writel(vic, value, VIC_THI_STREAMID1);
		}
	}
#endif
>>>>>>> upstream/android-13

	/* setup clockgating registers */
	vic_writel(vic, CG_IDLE_CG_DLY_CNT(4) |
			CG_IDLE_CG_EN |
			CG_WAKEUP_DLY_CNT(4),
		   NV_PVIC_MISC_PRI_VIC_CG);

	err = falcon_boot(&vic->falcon);
	if (err < 0)
		return err;

<<<<<<< HEAD
	hdr = vic->falcon.firmware.vaddr;
	fce_bin_data_offset = *(u32 *)(hdr + VIC_UCODE_FCE_DATA_OFFSET);
	hdr = vic->falcon.firmware.vaddr +
		*(u32 *)(hdr + VIC_UCODE_FCE_HEADER_OFFSET);
	fce_ucode_size = *(u32 *)(hdr + FCE_UCODE_SIZE_OFFSET);

	falcon_execute_method(&vic->falcon, VIC_SET_APPLICATION_ID, 1);
	falcon_execute_method(&vic->falcon, VIC_SET_FCE_UCODE_SIZE,
			      fce_ucode_size);
	falcon_execute_method(&vic->falcon, VIC_SET_FCE_UCODE_OFFSET,
			      (vic->falcon.firmware.paddr + fce_bin_data_offset)
				>> 8);
=======
	hdr = vic->falcon.firmware.virt;
	fce_bin_data_offset = *(u32 *)(hdr + VIC_UCODE_FCE_DATA_OFFSET);

	/* Old VIC firmware needs kernel help with setting up FCE microcode. */
	if (fce_bin_data_offset != 0x0 && fce_bin_data_offset != 0xa5a5a5a5) {
		hdr = vic->falcon.firmware.virt +
			*(u32 *)(hdr + VIC_UCODE_FCE_HEADER_OFFSET);
		fce_ucode_size = *(u32 *)(hdr + FCE_UCODE_SIZE_OFFSET);

		falcon_execute_method(&vic->falcon, VIC_SET_FCE_UCODE_SIZE,
				      fce_ucode_size);
		falcon_execute_method(
			&vic->falcon, VIC_SET_FCE_UCODE_OFFSET,
			(vic->falcon.firmware.iova + fce_bin_data_offset) >> 8);
	}
>>>>>>> upstream/android-13

	err = falcon_wait_idle(&vic->falcon);
	if (err < 0) {
		dev_err(vic->dev,
			"failed to set application ID and FCE base\n");
		return err;
	}

<<<<<<< HEAD
	vic->booted = true;

	return 0;
}

static void *vic_falcon_alloc(struct falcon *falcon, size_t size,
			      dma_addr_t *iova)
{
	struct tegra_drm *tegra = falcon->data;

	return tegra_drm_alloc(tegra, size, iova);
}

static void vic_falcon_free(struct falcon *falcon, size_t size,
			    dma_addr_t iova, void *va)
{
	struct tegra_drm *tegra = falcon->data;

	return tegra_drm_free(tegra, size, va, iova);
}

static const struct falcon_ops vic_falcon_ops = {
	.alloc = vic_falcon_alloc,
	.free = vic_falcon_free
};

static int vic_init(struct host1x_client *client)
{
	struct tegra_drm_client *drm = host1x_to_drm_client(client);
	struct iommu_group *group = iommu_group_get(client->dev);
	struct drm_device *dev = dev_get_drvdata(client->parent);
=======
	return 0;
}

static int vic_init(struct host1x_client *client)
{
	struct tegra_drm_client *drm = host1x_to_drm_client(client);
	struct drm_device *dev = dev_get_drvdata(client->host);
>>>>>>> upstream/android-13
	struct tegra_drm *tegra = dev->dev_private;
	struct vic *vic = to_vic(drm);
	int err;

<<<<<<< HEAD
	if (group && tegra->domain) {
		err = iommu_attach_group(tegra->domain, group);
		if (err < 0) {
			dev_err(vic->dev, "failed to attach to domain: %d\n",
				err);
			return err;
		}

		vic->domain = tegra->domain;
	}

	if (!vic->falcon.data) {
		vic->falcon.data = tegra;
		err = falcon_load_firmware(&vic->falcon);
		if (err < 0)
			goto detach;
	}

	vic->channel = host1x_channel_request(client->dev);
=======
	err = host1x_client_iommu_attach(client);
	if (err < 0 && err != -ENODEV) {
		dev_err(vic->dev, "failed to attach to domain: %d\n", err);
		return err;
	}

	vic->channel = host1x_channel_request(client);
>>>>>>> upstream/android-13
	if (!vic->channel) {
		err = -ENOMEM;
		goto detach;
	}

	client->syncpts[0] = host1x_syncpt_request(client, 0);
	if (!client->syncpts[0]) {
		err = -ENOMEM;
		goto free_channel;
	}

	err = tegra_drm_register_client(tegra, drm);
	if (err < 0)
		goto free_syncpt;

<<<<<<< HEAD
	return 0;

free_syncpt:
	host1x_syncpt_free(client->syncpts[0]);
free_channel:
	host1x_channel_put(vic->channel);
detach:
	if (group && tegra->domain)
		iommu_detach_group(tegra->domain, group);
=======
	/*
	 * Inherit the DMA parameters (such as maximum segment size) from the
	 * parent host1x device.
	 */
	client->dev->dma_parms = client->host->dma_parms;

	return 0;

free_syncpt:
	host1x_syncpt_put(client->syncpts[0]);
free_channel:
	host1x_channel_put(vic->channel);
detach:
	host1x_client_iommu_detach(client);
>>>>>>> upstream/android-13

	return err;
}

static int vic_exit(struct host1x_client *client)
{
	struct tegra_drm_client *drm = host1x_to_drm_client(client);
<<<<<<< HEAD
	struct iommu_group *group = iommu_group_get(client->dev);
	struct drm_device *dev = dev_get_drvdata(client->parent);
=======
	struct drm_device *dev = dev_get_drvdata(client->host);
>>>>>>> upstream/android-13
	struct tegra_drm *tegra = dev->dev_private;
	struct vic *vic = to_vic(drm);
	int err;

<<<<<<< HEAD
=======
	/* avoid a dangling pointer just in case this disappears */
	client->dev->dma_parms = NULL;

>>>>>>> upstream/android-13
	err = tegra_drm_unregister_client(tegra, drm);
	if (err < 0)
		return err;

<<<<<<< HEAD
	host1x_syncpt_free(client->syncpts[0]);
	host1x_channel_put(vic->channel);

	if (vic->domain) {
		iommu_detach_group(vic->domain, group);
		vic->domain = NULL;
=======
	host1x_syncpt_put(client->syncpts[0]);
	host1x_channel_put(vic->channel);
	host1x_client_iommu_detach(client);

	if (client->group) {
		dma_unmap_single(vic->dev, vic->falcon.firmware.phys,
				 vic->falcon.firmware.size, DMA_TO_DEVICE);
		tegra_drm_free(tegra, vic->falcon.firmware.size,
			       vic->falcon.firmware.virt,
			       vic->falcon.firmware.iova);
	} else {
		dma_free_coherent(vic->dev, vic->falcon.firmware.size,
				  vic->falcon.firmware.virt,
				  vic->falcon.firmware.iova);
>>>>>>> upstream/android-13
	}

	return 0;
}

static const struct host1x_client_ops vic_client_ops = {
	.init = vic_init,
	.exit = vic_exit,
};

<<<<<<< HEAD
=======
static int vic_load_firmware(struct vic *vic)
{
	struct host1x_client *client = &vic->client.base;
	struct tegra_drm *tegra = vic->client.drm;
	dma_addr_t iova;
	size_t size;
	void *virt;
	int err;

	if (vic->falcon.firmware.virt)
		return 0;

	err = falcon_read_firmware(&vic->falcon, vic->config->firmware);
	if (err < 0)
		return err;

	size = vic->falcon.firmware.size;

	if (!client->group) {
		virt = dma_alloc_coherent(vic->dev, size, &iova, GFP_KERNEL);
		if (!virt)
			return -ENOMEM;
	} else {
		virt = tegra_drm_alloc(tegra, size, &iova);
	}

	vic->falcon.firmware.virt = virt;
	vic->falcon.firmware.iova = iova;

	err = falcon_load_firmware(&vic->falcon);
	if (err < 0)
		goto cleanup;

	/*
	 * In this case we have received an IOVA from the shared domain, so we
	 * need to make sure to get the physical address so that the DMA API
	 * knows what memory pages to flush the cache for.
	 */
	if (client->group) {
		dma_addr_t phys;

		phys = dma_map_single(vic->dev, virt, size, DMA_TO_DEVICE);

		err = dma_mapping_error(vic->dev, phys);
		if (err < 0)
			goto cleanup;

		vic->falcon.firmware.phys = phys;
	}

	return 0;

cleanup:
	if (!client->group)
		dma_free_coherent(vic->dev, size, virt, iova);
	else
		tegra_drm_free(tegra, size, virt, iova);

	return err;
}


static int vic_runtime_resume(struct device *dev)
{
	struct vic *vic = dev_get_drvdata(dev);
	int err;

	err = clk_prepare_enable(vic->clk);
	if (err < 0)
		return err;

	usleep_range(10, 20);

	err = reset_control_deassert(vic->rst);
	if (err < 0)
		goto disable;

	usleep_range(10, 20);

	err = vic_load_firmware(vic);
	if (err < 0)
		goto assert;

	err = vic_boot(vic);
	if (err < 0)
		goto assert;

	return 0;

assert:
	reset_control_assert(vic->rst);
disable:
	clk_disable_unprepare(vic->clk);
	return err;
}

static int vic_runtime_suspend(struct device *dev)
{
	struct vic *vic = dev_get_drvdata(dev);
	int err;

	err = reset_control_assert(vic->rst);
	if (err < 0)
		return err;

	usleep_range(2000, 4000);

	clk_disable_unprepare(vic->clk);

	return 0;
}

>>>>>>> upstream/android-13
static int vic_open_channel(struct tegra_drm_client *client,
			    struct tegra_drm_context *context)
{
	struct vic *vic = to_vic(client);
	int err;

<<<<<<< HEAD
	err = pm_runtime_get_sync(vic->dev);
	if (err < 0)
		return err;

	err = vic_boot(vic);
	if (err < 0) {
		pm_runtime_put(vic->dev);
		return err;
	}

=======
	err = pm_runtime_resume_and_get(vic->dev);
	if (err < 0)
		return err;

>>>>>>> upstream/android-13
	context->channel = host1x_channel_get(vic->channel);
	if (!context->channel) {
		pm_runtime_put(vic->dev);
		return -ENOMEM;
	}

	return 0;
}

static void vic_close_channel(struct tegra_drm_context *context)
{
	struct vic *vic = to_vic(context->client);

	host1x_channel_put(context->channel);
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	pm_runtime_put(vic->dev);
}

static const struct tegra_drm_client_ops vic_ops = {
	.open_channel = vic_open_channel,
	.close_channel = vic_close_channel,
	.submit = tegra_drm_submit,
};

#define NVIDIA_TEGRA_124_VIC_FIRMWARE "nvidia/tegra124/vic03_ucode.bin"

static const struct vic_config vic_t124_config = {
	.firmware = NVIDIA_TEGRA_124_VIC_FIRMWARE,
	.version = 0x40,
<<<<<<< HEAD
=======
	.supports_sid = false,
>>>>>>> upstream/android-13
};

#define NVIDIA_TEGRA_210_VIC_FIRMWARE "nvidia/tegra210/vic04_ucode.bin"

static const struct vic_config vic_t210_config = {
	.firmware = NVIDIA_TEGRA_210_VIC_FIRMWARE,
	.version = 0x21,
<<<<<<< HEAD
=======
	.supports_sid = false,
>>>>>>> upstream/android-13
};

#define NVIDIA_TEGRA_186_VIC_FIRMWARE "nvidia/tegra186/vic04_ucode.bin"

static const struct vic_config vic_t186_config = {
	.firmware = NVIDIA_TEGRA_186_VIC_FIRMWARE,
	.version = 0x18,
<<<<<<< HEAD
};

static const struct of_device_id vic_match[] = {
	{ .compatible = "nvidia,tegra124-vic", .data = &vic_t124_config },
	{ .compatible = "nvidia,tegra210-vic", .data = &vic_t210_config },
	{ .compatible = "nvidia,tegra186-vic", .data = &vic_t186_config },
	{ },
};
=======
	.supports_sid = true,
};

#define NVIDIA_TEGRA_194_VIC_FIRMWARE "nvidia/tegra194/vic.bin"

static const struct vic_config vic_t194_config = {
	.firmware = NVIDIA_TEGRA_194_VIC_FIRMWARE,
	.version = 0x19,
	.supports_sid = true,
};

static const struct of_device_id tegra_vic_of_match[] = {
	{ .compatible = "nvidia,tegra124-vic", .data = &vic_t124_config },
	{ .compatible = "nvidia,tegra210-vic", .data = &vic_t210_config },
	{ .compatible = "nvidia,tegra186-vic", .data = &vic_t186_config },
	{ .compatible = "nvidia,tegra194-vic", .data = &vic_t194_config },
	{ },
};
MODULE_DEVICE_TABLE(of, tegra_vic_of_match);
>>>>>>> upstream/android-13

static int vic_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct host1x_syncpt **syncpts;
	struct resource *regs;
	struct vic *vic;
	int err;

<<<<<<< HEAD
=======
	/* inherit DMA mask from host1x parent */
	err = dma_coerce_mask_and_coherent(dev, *dev->parent->dma_mask);
	if (err < 0) {
		dev_err(&pdev->dev, "failed to set DMA mask: %d\n", err);
		return err;
	}

>>>>>>> upstream/android-13
	vic = devm_kzalloc(dev, sizeof(*vic), GFP_KERNEL);
	if (!vic)
		return -ENOMEM;

	vic->config = of_device_get_match_data(dev);

	syncpts = devm_kzalloc(dev, sizeof(*syncpts), GFP_KERNEL);
	if (!syncpts)
		return -ENOMEM;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs) {
		dev_err(&pdev->dev, "failed to get registers\n");
		return -ENXIO;
	}

	vic->regs = devm_ioremap_resource(dev, regs);
	if (IS_ERR(vic->regs))
		return PTR_ERR(vic->regs);

	vic->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(vic->clk)) {
		dev_err(&pdev->dev, "failed to get clock\n");
		return PTR_ERR(vic->clk);
	}

<<<<<<< HEAD
	vic->falcon.dev = dev;
	vic->falcon.regs = vic->regs;
	vic->falcon.ops = &vic_falcon_ops;
=======
	if (!dev->pm_domain) {
		vic->rst = devm_reset_control_get(dev, "vic");
		if (IS_ERR(vic->rst)) {
			dev_err(&pdev->dev, "failed to get reset\n");
			return PTR_ERR(vic->rst);
		}
	}

	vic->falcon.dev = dev;
	vic->falcon.regs = vic->regs;
>>>>>>> upstream/android-13

	err = falcon_init(&vic->falcon);
	if (err < 0)
		return err;

<<<<<<< HEAD
	err = falcon_read_firmware(&vic->falcon, vic->config->firmware);
	if (err < 0)
		goto exit_falcon;

=======
>>>>>>> upstream/android-13
	platform_set_drvdata(pdev, vic);

	INIT_LIST_HEAD(&vic->client.base.list);
	vic->client.base.ops = &vic_client_ops;
	vic->client.base.dev = dev;
	vic->client.base.class = HOST1X_CLASS_VIC;
	vic->client.base.syncpts = syncpts;
	vic->client.base.num_syncpts = 1;
	vic->dev = dev;

	INIT_LIST_HEAD(&vic->client.list);
	vic->client.version = vic->config->version;
	vic->client.ops = &vic_ops;

	err = host1x_client_register(&vic->client.base);
	if (err < 0) {
		dev_err(dev, "failed to register host1x client: %d\n", err);
<<<<<<< HEAD
		platform_set_drvdata(pdev, NULL);
=======
>>>>>>> upstream/android-13
		goto exit_falcon;
	}

	pm_runtime_enable(&pdev->dev);
	if (!pm_runtime_enabled(&pdev->dev)) {
		err = vic_runtime_resume(&pdev->dev);
		if (err < 0)
			goto unregister_client;
	}

	return 0;

unregister_client:
	host1x_client_unregister(&vic->client.base);
exit_falcon:
	falcon_exit(&vic->falcon);

	return err;
}

static int vic_remove(struct platform_device *pdev)
{
	struct vic *vic = platform_get_drvdata(pdev);
	int err;

	err = host1x_client_unregister(&vic->client.base);
	if (err < 0) {
		dev_err(&pdev->dev, "failed to unregister host1x client: %d\n",
			err);
		return err;
	}

	if (pm_runtime_enabled(&pdev->dev))
		pm_runtime_disable(&pdev->dev);
	else
		vic_runtime_suspend(&pdev->dev);

	falcon_exit(&vic->falcon);

	return 0;
}

static const struct dev_pm_ops vic_pm_ops = {
	SET_RUNTIME_PM_OPS(vic_runtime_suspend, vic_runtime_resume, NULL)
};

struct platform_driver tegra_vic_driver = {
	.driver = {
		.name = "tegra-vic",
<<<<<<< HEAD
		.of_match_table = vic_match,
=======
		.of_match_table = tegra_vic_of_match,
>>>>>>> upstream/android-13
		.pm = &vic_pm_ops
	},
	.probe = vic_probe,
	.remove = vic_remove,
};

#if IS_ENABLED(CONFIG_ARCH_TEGRA_124_SOC)
MODULE_FIRMWARE(NVIDIA_TEGRA_124_VIC_FIRMWARE);
#endif
#if IS_ENABLED(CONFIG_ARCH_TEGRA_210_SOC)
MODULE_FIRMWARE(NVIDIA_TEGRA_210_VIC_FIRMWARE);
#endif
#if IS_ENABLED(CONFIG_ARCH_TEGRA_186_SOC)
MODULE_FIRMWARE(NVIDIA_TEGRA_186_VIC_FIRMWARE);
#endif
<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_ARCH_TEGRA_194_SOC)
MODULE_FIRMWARE(NVIDIA_TEGRA_194_VIC_FIRMWARE);
#endif
>>>>>>> upstream/android-13
