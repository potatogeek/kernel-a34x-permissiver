<<<<<<< HEAD
/*
 * Copyright © 2014 NVIDIA Corporation
 * Copyright © 2015 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright © 2014 NVIDIA Corporation
 * Copyright © 2015 Broadcom Corporation
>>>>>>> upstream/android-13
 */

#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/slab.h>
#include <linux/soc/brcmstb/brcmstb.h>
#include <linux/sys_soc.h>

<<<<<<< HEAD
#include <soc/brcmstb/common.h>

static u32 family_id;
static u32 product_id;

static const struct of_device_id brcmstb_machine_match[] = {
	{ .compatible = "brcm,brcmstb", },
	{ }
};

bool soc_is_brcmstb(void)
{
	const struct of_device_id *match;
	struct device_node *root;

	root = of_find_node_by_path("/");
	if (!root)
		return false;

	match = of_match_node(brcmstb_machine_match, root);
	of_node_put(root);

	return match != NULL;
}

=======
static u32 family_id;
static u32 product_id;

>>>>>>> upstream/android-13
u32 brcmstb_get_family_id(void)
{
	return family_id;
}
EXPORT_SYMBOL(brcmstb_get_family_id);

u32 brcmstb_get_product_id(void)
{
	return product_id;
}
EXPORT_SYMBOL(brcmstb_get_product_id);

static const struct of_device_id sun_top_ctrl_match[] = {
	{ .compatible = "brcm,bcm7125-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7346-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7358-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7360-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7362-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7420-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7425-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7429-sun-top-ctrl", },
	{ .compatible = "brcm,bcm7435-sun-top-ctrl", },
	{ .compatible = "brcm,brcmstb-sun-top-ctrl", },
	{ }
};

static int __init brcmstb_soc_device_early_init(void)
{
	struct device_node *sun_top_ctrl;
	void __iomem *sun_top_ctrl_base;
	int ret = 0;

	/* We could be on a multi-platform kernel, don't make this fatal but
	 * bail out early
	 */
	sun_top_ctrl = of_find_matching_node(NULL, sun_top_ctrl_match);
	if (!sun_top_ctrl)
		return ret;

	sun_top_ctrl_base = of_iomap(sun_top_ctrl, 0);
	if (!sun_top_ctrl_base) {
		ret = -ENODEV;
		goto out;
	}

	family_id = readl(sun_top_ctrl_base);
	product_id = readl(sun_top_ctrl_base + 0x4);
	iounmap(sun_top_ctrl_base);
out:
	of_node_put(sun_top_ctrl);
	return ret;
}
early_initcall(brcmstb_soc_device_early_init);

static int __init brcmstb_soc_device_init(void)
{
	struct soc_device_attribute *soc_dev_attr;
	struct device_node *sun_top_ctrl;
	struct soc_device *soc_dev;
	int ret = 0;

	/* We could be on a multi-platform kernel, don't make this fatal but
	 * bail out early
	 */
	sun_top_ctrl = of_find_matching_node(NULL, sun_top_ctrl_match);
	if (!sun_top_ctrl)
		return ret;

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr) {
		ret = -ENOMEM;
		goto out;
	}

	soc_dev_attr->family = kasprintf(GFP_KERNEL, "%x",
					 family_id >> 28 ?
					 family_id >> 16 : family_id >> 8);
	soc_dev_attr->soc_id = kasprintf(GFP_KERNEL, "%x",
					 product_id >> 28 ?
					 product_id >> 16 : product_id >> 8);
	soc_dev_attr->revision = kasprintf(GFP_KERNEL, "%c%d",
					 ((product_id & 0xf0) >> 4) + 'A',
					   product_id & 0xf);

	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev)) {
		kfree(soc_dev_attr->family);
		kfree(soc_dev_attr->soc_id);
		kfree(soc_dev_attr->revision);
		kfree(soc_dev_attr);
		ret = -ENOMEM;
	}
out:
	of_node_put(sun_top_ctrl);
	return ret;
}
arch_initcall(brcmstb_soc_device_init);
