<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * coreboot_table.h
 *
 * Internal header for coreboot table access.
 *
 * Copyright 2014 Gerd Hoffmann <kraxel@redhat.com>
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

#ifndef __COREBOOT_TABLE_H
#define __COREBOOT_TABLE_H

<<<<<<< HEAD
#include <linux/io.h>
=======
#include <linux/device.h>
>>>>>>> upstream/android-13

/* Coreboot table header structure */
struct coreboot_table_header {
	char signature[4];
	u32 header_bytes;
	u32 header_checksum;
	u32 table_bytes;
	u32 table_checksum;
	u32 table_entries;
};

/* List of coreboot entry structures that is used */
/* Generic */
struct coreboot_table_entry {
	u32 tag;
	u32 size;
};

/* Points to a CBMEM entry */
struct lb_cbmem_ref {
	u32 tag;
	u32 size;

	u64 cbmem_addr;
};

/* Describes framebuffer setup by coreboot */
struct lb_framebuffer {
	u32 tag;
	u32 size;

	u64 physical_address;
	u32 x_resolution;
	u32 y_resolution;
	u32 bytes_per_line;
	u8  bits_per_pixel;
	u8  red_mask_pos;
	u8  red_mask_size;
	u8  green_mask_pos;
	u8  green_mask_size;
	u8  blue_mask_pos;
	u8  blue_mask_size;
	u8  reserved_mask_pos;
	u8  reserved_mask_size;
};

/* A device, additionally with information from coreboot. */
struct coreboot_device {
	struct device dev;
	union {
		struct coreboot_table_entry entry;
		struct lb_cbmem_ref cbmem_ref;
		struct lb_framebuffer framebuffer;
	};
};

/* A driver for handling devices described in coreboot tables. */
struct coreboot_driver {
	int (*probe)(struct coreboot_device *);
<<<<<<< HEAD
	int (*remove)(struct coreboot_device *);
=======
	void (*remove)(struct coreboot_device *);
>>>>>>> upstream/android-13
	struct device_driver drv;
	u32 tag;
};

/* Register a driver that uses the data from a coreboot table. */
int coreboot_driver_register(struct coreboot_driver *driver);

/* Unregister a driver that uses the data from a coreboot table. */
void coreboot_driver_unregister(struct coreboot_driver *driver);

<<<<<<< HEAD
/* Initialize coreboot table module given a pointer to iomem */
int coreboot_table_init(struct device *dev, void __iomem *ptr);

/* Cleanup coreboot table module */
int coreboot_table_exit(void);
=======
/* module_coreboot_driver() - Helper macro for drivers that don't do
 * anything special in module init/exit.  This eliminates a lot of
 * boilerplate.  Each module may only use this macro once, and
 * calling it replaces module_init() and module_exit()
 */
#define module_coreboot_driver(__coreboot_driver) \
	module_driver(__coreboot_driver, coreboot_driver_register, \
			coreboot_driver_unregister)
>>>>>>> upstream/android-13

#endif /* __COREBOOT_TABLE_H */
