<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * framebuffer-coreboot.c
 *
 * Memory based framebuffer accessed through coreboot table.
 *
 * Copyright 2012-2013 David Herrmann <dh.herrmann@gmail.com>
 * Copyright 2017 Google Inc.
 * Copyright 2017 Samuel Holland <samuel@sholland.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_data/simplefb.h>
#include <linux/platform_device.h>

#include "coreboot_table.h"

#define CB_TAG_FRAMEBUFFER 0x12

static const struct simplefb_format formats[] = SIMPLEFB_FORMATS;

static int framebuffer_probe(struct coreboot_device *dev)
{
	int i;
	u32 length;
	struct lb_framebuffer *fb = &dev->framebuffer;
	struct platform_device *pdev;
	struct resource res;
	struct simplefb_platform_data pdata = {
		.width = fb->x_resolution,
		.height = fb->y_resolution,
		.stride = fb->bytes_per_line,
		.format = NULL,
	};

	for (i = 0; i < ARRAY_SIZE(formats); ++i) {
		if (fb->bits_per_pixel     == formats[i].bits_per_pixel &&
		    fb->red_mask_pos       == formats[i].red.offset &&
		    fb->red_mask_size      == formats[i].red.length &&
		    fb->green_mask_pos     == formats[i].green.offset &&
		    fb->green_mask_size    == formats[i].green.length &&
		    fb->blue_mask_pos      == formats[i].blue.offset &&
		    fb->blue_mask_size     == formats[i].blue.length &&
		    fb->reserved_mask_pos  == formats[i].transp.offset &&
		    fb->reserved_mask_size == formats[i].transp.length)
			pdata.format = formats[i].name;
	}
	if (!pdata.format)
		return -ENODEV;

	memset(&res, 0, sizeof(res));
	res.flags = IORESOURCE_MEM | IORESOURCE_BUSY;
	res.name = "Coreboot Framebuffer";
	res.start = fb->physical_address;
	length = PAGE_ALIGN(fb->y_resolution * fb->bytes_per_line);
	res.end = res.start + length - 1;
	if (res.end <= res.start)
		return -EINVAL;

	pdev = platform_device_register_resndata(&dev->dev,
						 "simple-framebuffer", 0,
						 &res, 1, &pdata,
						 sizeof(pdata));
	if (IS_ERR(pdev))
		pr_warn("coreboot: could not register framebuffer\n");
	else
		dev_set_drvdata(&dev->dev, pdev);

	return PTR_ERR_OR_ZERO(pdev);
}

<<<<<<< HEAD
static int framebuffer_remove(struct coreboot_device *dev)
=======
static void framebuffer_remove(struct coreboot_device *dev)
>>>>>>> upstream/android-13
{
	struct platform_device *pdev = dev_get_drvdata(&dev->dev);

	platform_device_unregister(pdev);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static struct coreboot_driver framebuffer_driver = {
	.probe = framebuffer_probe,
	.remove = framebuffer_remove,
	.drv = {
		.name = "framebuffer",
	},
	.tag = CB_TAG_FRAMEBUFFER,
};
<<<<<<< HEAD

static int __init coreboot_framebuffer_init(void)
{
	return coreboot_driver_register(&framebuffer_driver);
}

static void coreboot_framebuffer_exit(void)
{
	coreboot_driver_unregister(&framebuffer_driver);
}

module_init(coreboot_framebuffer_init);
module_exit(coreboot_framebuffer_exit);
=======
module_coreboot_driver(framebuffer_driver);
>>>>>>> upstream/android-13

MODULE_AUTHOR("Samuel Holland <samuel@sholland.org>");
MODULE_LICENSE("GPL");
