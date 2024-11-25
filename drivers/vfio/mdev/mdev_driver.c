<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * MDEV driver
 *
 * Copyright (c) 2016, NVIDIA CORPORATION. All rights reserved.
 *     Author: Neo Jia <cjia@nvidia.com>
 *             Kirti Wankhede <kwankhede@nvidia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/iommu.h>
#include <linux/mdev.h>

#include "mdev_private.h"

static int mdev_attach_iommu(struct mdev_device *mdev)
{
	int ret;
	struct iommu_group *group;

	group = iommu_group_alloc();
	if (IS_ERR(group))
		return PTR_ERR(group);

	ret = iommu_group_add_device(group, &mdev->dev);
	if (!ret)
		dev_info(&mdev->dev, "MDEV: group_id = %d\n",
			 iommu_group_id(group));

	iommu_group_put(group);
	return ret;
}

static void mdev_detach_iommu(struct mdev_device *mdev)
{
	iommu_group_remove_device(&mdev->dev);
	dev_info(&mdev->dev, "MDEV: detaching iommu\n");
}

static int mdev_probe(struct device *dev)
{
<<<<<<< HEAD
	struct mdev_driver *drv = to_mdev_driver(dev->driver);
=======
	struct mdev_driver *drv =
		container_of(dev->driver, struct mdev_driver, driver);
>>>>>>> upstream/android-13
	struct mdev_device *mdev = to_mdev_device(dev);
	int ret;

	ret = mdev_attach_iommu(mdev);
	if (ret)
		return ret;

<<<<<<< HEAD
	if (drv && drv->probe) {
		ret = drv->probe(dev);
=======
	if (drv->probe) {
		ret = drv->probe(mdev);
>>>>>>> upstream/android-13
		if (ret)
			mdev_detach_iommu(mdev);
	}

	return ret;
}

<<<<<<< HEAD
static int mdev_remove(struct device *dev)
{
	struct mdev_driver *drv = to_mdev_driver(dev->driver);
	struct mdev_device *mdev = to_mdev_device(dev);

	if (drv && drv->remove)
		drv->remove(dev);

	mdev_detach_iommu(mdev);

=======
static void mdev_remove(struct device *dev)
{
	struct mdev_driver *drv =
		container_of(dev->driver, struct mdev_driver, driver);
	struct mdev_device *mdev = to_mdev_device(dev);

	if (drv->remove)
		drv->remove(mdev);

	mdev_detach_iommu(mdev);
}

static int mdev_match(struct device *dev, struct device_driver *drv)
{
	/*
	 * No drivers automatically match. Drivers are only bound by explicit
	 * device_driver_attach()
	 */
>>>>>>> upstream/android-13
	return 0;
}

struct bus_type mdev_bus_type = {
	.name		= "mdev",
	.probe		= mdev_probe,
	.remove		= mdev_remove,
<<<<<<< HEAD
=======
	.match		= mdev_match,
>>>>>>> upstream/android-13
};
EXPORT_SYMBOL_GPL(mdev_bus_type);

/**
 * mdev_register_driver - register a new MDEV driver
 * @drv: the driver to register
<<<<<<< HEAD
 * @owner: module owner of driver to be registered
 *
 * Returns a negative value on error, otherwise 0.
 **/
int mdev_register_driver(struct mdev_driver *drv, struct module *owner)
{
	/* initialize common driver fields */
	drv->driver.name = drv->name;
	drv->driver.bus = &mdev_bus_type;
	drv->driver.owner = owner;
=======
 *
 * Returns a negative value on error, otherwise 0.
 **/
int mdev_register_driver(struct mdev_driver *drv)
{
	/* initialize common driver fields */
	drv->driver.bus = &mdev_bus_type;
>>>>>>> upstream/android-13

	/* register with core */
	return driver_register(&drv->driver);
}
EXPORT_SYMBOL(mdev_register_driver);

/*
 * mdev_unregister_driver - unregister MDEV driver
 * @drv: the driver to unregister
 */
void mdev_unregister_driver(struct mdev_driver *drv)
{
	driver_unregister(&drv->driver);
}
EXPORT_SYMBOL(mdev_unregister_driver);

int mdev_bus_register(void)
{
	return bus_register(&mdev_bus_type);
}

void mdev_bus_unregister(void)
{
	bus_unregister(&mdev_bus_type);
}
