<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Definitions for container bus type.
 *
 * Copyright (C) 2013, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
 */

#ifndef _LINUX_CONTAINER_H
#define _LINUX_CONTAINER_H

>>>>>>> upstream/android-13
#include <linux/device.h>

/* drivers/base/power/container.c */
extern struct bus_type container_subsys;

struct container_dev {
	struct device dev;
	int (*offline)(struct container_dev *cdev);
};

static inline struct container_dev *to_container_dev(struct device *dev)
{
	return container_of(dev, struct container_dev, dev);
}
<<<<<<< HEAD
=======

#endif /* _LINUX_CONTAINER_H */
>>>>>>> upstream/android-13
