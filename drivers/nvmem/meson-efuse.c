<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Amlogic Meson GX eFuse Driver
 *
 * Copyright (c) 2016 Endless Computers, Inc.
 * Author: Carlo Caione <carlo@endlessm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 */

=======
 */

#include <linux/clk.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/nvmem-provider.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#include <linux/firmware/meson/meson_sm.h>

static int meson_efuse_read(void *context, unsigned int offset,
			    void *val, size_t bytes)
{
<<<<<<< HEAD
	return meson_sm_call_read((u8 *)val, bytes, SM_EFUSE_READ, offset,
=======
	struct meson_sm_firmware *fw = context;

	return meson_sm_call_read(fw, (u8 *)val, bytes, SM_EFUSE_READ, offset,
>>>>>>> upstream/android-13
				  bytes, 0, 0, 0);
}

static int meson_efuse_write(void *context, unsigned int offset,
			     void *val, size_t bytes)
{
<<<<<<< HEAD
	return meson_sm_call_write((u8 *)val, bytes, SM_EFUSE_WRITE, offset,
=======
	struct meson_sm_firmware *fw = context;

	return meson_sm_call_write(fw, (u8 *)val, bytes, SM_EFUSE_WRITE, offset,
>>>>>>> upstream/android-13
				   bytes, 0, 0, 0);
}

static const struct of_device_id meson_efuse_match[] = {
	{ .compatible = "amlogic,meson-gxbb-efuse", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, meson_efuse_match);

static int meson_efuse_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
	struct nvmem_device *nvmem;
	struct nvmem_config *econfig;
	unsigned int size;

	if (meson_sm_call(SM_EFUSE_USER_MAX, &size, 0, 0, 0, 0, 0) < 0)
		return -EINVAL;
=======
	struct meson_sm_firmware *fw;
	struct device_node *sm_np;
	struct nvmem_device *nvmem;
	struct nvmem_config *econfig;
	struct clk *clk;
	unsigned int size;
	int ret;

	sm_np = of_parse_phandle(pdev->dev.of_node, "secure-monitor", 0);
	if (!sm_np) {
		dev_err(&pdev->dev, "no secure-monitor node\n");
		return -ENODEV;
	}

	fw = meson_sm_get(sm_np);
	of_node_put(sm_np);
	if (!fw)
		return -EPROBE_DEFER;

	clk = devm_clk_get(dev, NULL);
	if (IS_ERR(clk)) {
		ret = PTR_ERR(clk);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "failed to get efuse gate");
		return ret;
	}

	ret = clk_prepare_enable(clk);
	if (ret) {
		dev_err(dev, "failed to enable gate");
		return ret;
	}

	ret = devm_add_action_or_reset(dev,
				       (void(*)(void *))clk_disable_unprepare,
				       clk);
	if (ret) {
		dev_err(dev, "failed to add disable callback");
		return ret;
	}

	if (meson_sm_call(fw, SM_EFUSE_USER_MAX, &size, 0, 0, 0, 0, 0) < 0) {
		dev_err(dev, "failed to get max user");
		return -EINVAL;
	}
>>>>>>> upstream/android-13

	econfig = devm_kzalloc(dev, sizeof(*econfig), GFP_KERNEL);
	if (!econfig)
		return -ENOMEM;

	econfig->dev = dev;
	econfig->name = dev_name(dev);
	econfig->stride = 1;
	econfig->word_size = 1;
	econfig->reg_read = meson_efuse_read;
	econfig->reg_write = meson_efuse_write;
	econfig->size = size;
<<<<<<< HEAD
=======
	econfig->priv = fw;
>>>>>>> upstream/android-13

	nvmem = devm_nvmem_register(&pdev->dev, econfig);

	return PTR_ERR_OR_ZERO(nvmem);
}

static struct platform_driver meson_efuse_driver = {
	.probe = meson_efuse_probe,
	.driver = {
		.name = "meson-efuse",
		.of_match_table = meson_efuse_match,
	},
};

module_platform_driver(meson_efuse_driver);

MODULE_AUTHOR("Carlo Caione <carlo@endlessm.com>");
MODULE_DESCRIPTION("Amlogic Meson GX NVMEM driver");
MODULE_LICENSE("GPL v2");
