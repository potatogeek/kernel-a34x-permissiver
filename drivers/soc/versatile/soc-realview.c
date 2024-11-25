<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2014 Linaro Ltd.
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>
#include <linux/platform_device.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/of.h>

/* System ID in syscon */
#define REALVIEW_SYS_ID_OFFSET 0x00

static const struct of_device_id realview_soc_of_match[] = {
	{ .compatible = "arm,realview-eb-soc",	},
	{ .compatible = "arm,realview-pb1176-soc", },
	{ .compatible = "arm,realview-pb11mp-soc", },
	{ .compatible = "arm,realview-pba8-soc", },
	{ .compatible = "arm,realview-pbx-soc", },
	{ }
};

static u32 realview_coreid;

static const char *realview_arch_str(u32 id)
{
	switch ((id >> 8) & 0xf) {
	case 0x04:
		return "AHB";
	case 0x05:
		return "Multi-layer AXI";
	default:
		return "Unknown";
	}
}

<<<<<<< HEAD
static ssize_t realview_get_manf(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
=======
static ssize_t
manufacturer_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	return sprintf(buf, "%02x\n", realview_coreid >> 24);
}

<<<<<<< HEAD
static struct device_attribute realview_manf_attr =
	__ATTR(manufacturer,  S_IRUGO, realview_get_manf,  NULL);

static ssize_t realview_get_board(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
=======
static DEVICE_ATTR_RO(manufacturer);

static ssize_t
board_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	return sprintf(buf, "HBI-%03x\n", ((realview_coreid >> 16) & 0xfff));
}

<<<<<<< HEAD
static struct device_attribute realview_board_attr =
	__ATTR(board,  S_IRUGO, realview_get_board,  NULL);

static ssize_t realview_get_arch(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
=======
static DEVICE_ATTR_RO(board);

static ssize_t
fpga_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	return sprintf(buf, "%s\n", realview_arch_str(realview_coreid));
}

<<<<<<< HEAD
static struct device_attribute realview_arch_attr =
	__ATTR(fpga,  S_IRUGO, realview_get_arch,  NULL);

static ssize_t realview_get_build(struct device *dev,
			       struct device_attribute *attr,
			       char *buf)
=======
static DEVICE_ATTR_RO(fpga);

static ssize_t
build_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	return sprintf(buf, "%02x\n", (realview_coreid & 0xFF));
}

<<<<<<< HEAD
static struct device_attribute realview_build_attr =
	__ATTR(build,  S_IRUGO, realview_get_build,  NULL);
=======
static DEVICE_ATTR_RO(build);

static struct attribute *realview_attrs[] = {
	&dev_attr_manufacturer.attr,
	&dev_attr_board.attr,
	&dev_attr_fpga.attr,
	&dev_attr_build.attr,
	NULL
};

ATTRIBUTE_GROUPS(realview);
>>>>>>> upstream/android-13

static int realview_soc_probe(struct platform_device *pdev)
{
	struct regmap *syscon_regmap;
	struct soc_device *soc_dev;
	struct soc_device_attribute *soc_dev_attr;
	struct device_node *np = pdev->dev.of_node;
	int ret;

	syscon_regmap = syscon_regmap_lookup_by_phandle(np, "regmap");
	if (IS_ERR(syscon_regmap))
		return PTR_ERR(syscon_regmap);

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		return -ENOMEM;

	ret = of_property_read_string(np, "compatible",
				      &soc_dev_attr->soc_id);
	if (ret)
		return -EINVAL;

	soc_dev_attr->machine = "RealView";
	soc_dev_attr->family = "Versatile";
<<<<<<< HEAD
=======
	soc_dev_attr->custom_attr_group = realview_groups[0];
>>>>>>> upstream/android-13
	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev)) {
		kfree(soc_dev_attr);
		return -ENODEV;
	}
	ret = regmap_read(syscon_regmap, REALVIEW_SYS_ID_OFFSET,
			  &realview_coreid);
	if (ret)
		return -ENODEV;

<<<<<<< HEAD
	device_create_file(soc_device_to_device(soc_dev), &realview_manf_attr);
	device_create_file(soc_device_to_device(soc_dev), &realview_board_attr);
	device_create_file(soc_device_to_device(soc_dev), &realview_arch_attr);
	device_create_file(soc_device_to_device(soc_dev), &realview_build_attr);

=======
>>>>>>> upstream/android-13
	dev_info(&pdev->dev, "RealView Syscon Core ID: 0x%08x, HBI-%03x\n",
		 realview_coreid,
		 ((realview_coreid >> 16) & 0xfff));
	/* FIXME: add attributes for SoC to sysfs */
	return 0;
}

static struct platform_driver realview_soc_driver = {
	.probe = realview_soc_probe,
	.driver = {
		.name = "realview-soc",
		.of_match_table = realview_soc_of_match,
	},
};
builtin_platform_driver(realview_soc_driver);
