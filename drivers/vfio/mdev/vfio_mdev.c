<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * VFIO based driver for Mediated device
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vfio.h>
#include <linux/mdev.h>

#include "mdev_private.h"

<<<<<<< HEAD
#define DRIVER_VERSION  "0.1"
#define DRIVER_AUTHOR   "NVIDIA Corporation"
#define DRIVER_DESC     "VFIO based driver for Mediated device"

static int vfio_mdev_open(void *device_data)
{
	struct mdev_device *mdev = device_data;
	struct mdev_parent *parent = mdev->parent;
	int ret;

	if (unlikely(!parent->ops->open))
		return -EINVAL;

	if (!try_module_get(THIS_MODULE))
		return -ENODEV;

	ret = parent->ops->open(mdev);
	if (ret)
		module_put(THIS_MODULE);

	return ret;
}

static void vfio_mdev_release(void *device_data)
{
	struct mdev_device *mdev = device_data;
	struct mdev_parent *parent = mdev->parent;

	if (likely(parent->ops->release))
		parent->ops->release(mdev);

	module_put(THIS_MODULE);
}

static long vfio_mdev_unlocked_ioctl(void *device_data,
				     unsigned int cmd, unsigned long arg)
{
	struct mdev_device *mdev = device_data;
	struct mdev_parent *parent = mdev->parent;

	if (unlikely(!parent->ops->ioctl))
		return -EINVAL;
=======
static int vfio_mdev_open_device(struct vfio_device *core_vdev)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;

	if (unlikely(!parent->ops->open_device))
		return 0;

	return parent->ops->open_device(mdev);
}

static void vfio_mdev_close_device(struct vfio_device *core_vdev)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;

	if (likely(parent->ops->close_device))
		parent->ops->close_device(mdev);
}

static long vfio_mdev_unlocked_ioctl(struct vfio_device *core_vdev,
				     unsigned int cmd, unsigned long arg)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;

	if (unlikely(!parent->ops->ioctl))
		return 0;
>>>>>>> upstream/android-13

	return parent->ops->ioctl(mdev, cmd, arg);
}

<<<<<<< HEAD
static ssize_t vfio_mdev_read(void *device_data, char __user *buf,
			      size_t count, loff_t *ppos)
{
	struct mdev_device *mdev = device_data;
	struct mdev_parent *parent = mdev->parent;
=======
static ssize_t vfio_mdev_read(struct vfio_device *core_vdev, char __user *buf,
			      size_t count, loff_t *ppos)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;
>>>>>>> upstream/android-13

	if (unlikely(!parent->ops->read))
		return -EINVAL;

	return parent->ops->read(mdev, buf, count, ppos);
}

<<<<<<< HEAD
static ssize_t vfio_mdev_write(void *device_data, const char __user *buf,
			       size_t count, loff_t *ppos)
{
	struct mdev_device *mdev = device_data;
	struct mdev_parent *parent = mdev->parent;
=======
static ssize_t vfio_mdev_write(struct vfio_device *core_vdev,
			       const char __user *buf, size_t count,
			       loff_t *ppos)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;
>>>>>>> upstream/android-13

	if (unlikely(!parent->ops->write))
		return -EINVAL;

	return parent->ops->write(mdev, buf, count, ppos);
}

<<<<<<< HEAD
static int vfio_mdev_mmap(void *device_data, struct vm_area_struct *vma)
{
	struct mdev_device *mdev = device_data;
	struct mdev_parent *parent = mdev->parent;
=======
static int vfio_mdev_mmap(struct vfio_device *core_vdev,
			  struct vm_area_struct *vma)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;
>>>>>>> upstream/android-13

	if (unlikely(!parent->ops->mmap))
		return -EINVAL;

	return parent->ops->mmap(mdev, vma);
}

<<<<<<< HEAD
static const struct vfio_device_ops vfio_mdev_dev_ops = {
	.name		= "vfio-mdev",
	.open		= vfio_mdev_open,
	.release	= vfio_mdev_release,
=======
static void vfio_mdev_request(struct vfio_device *core_vdev, unsigned int count)
{
	struct mdev_device *mdev = to_mdev_device(core_vdev->dev);
	struct mdev_parent *parent = mdev->type->parent;

	if (parent->ops->request)
		parent->ops->request(mdev, count);
	else if (count == 0)
		dev_notice(mdev_dev(mdev),
			   "No mdev vendor driver request callback support, blocked until released by user\n");
}

static const struct vfio_device_ops vfio_mdev_dev_ops = {
	.name		= "vfio-mdev",
	.open_device	= vfio_mdev_open_device,
	.close_device	= vfio_mdev_close_device,
>>>>>>> upstream/android-13
	.ioctl		= vfio_mdev_unlocked_ioctl,
	.read		= vfio_mdev_read,
	.write		= vfio_mdev_write,
	.mmap		= vfio_mdev_mmap,
<<<<<<< HEAD
};

static int vfio_mdev_probe(struct device *dev)
{
	struct mdev_device *mdev = to_mdev_device(dev);

	return vfio_add_group_dev(dev, &vfio_mdev_dev_ops, mdev);
}

static void vfio_mdev_remove(struct device *dev)
{
	vfio_del_group_dev(dev);
}

static struct mdev_driver vfio_mdev_driver = {
	.name	= "vfio_mdev",
	.probe	= vfio_mdev_probe,
	.remove	= vfio_mdev_remove,
};

static int __init vfio_mdev_init(void)
{
	return mdev_register_driver(&vfio_mdev_driver, THIS_MODULE);
}

static void __exit vfio_mdev_exit(void)
{
	mdev_unregister_driver(&vfio_mdev_driver);
}

module_init(vfio_mdev_init)
module_exit(vfio_mdev_exit)

MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
=======
	.request	= vfio_mdev_request,
};

static int vfio_mdev_probe(struct mdev_device *mdev)
{
	struct vfio_device *vdev;
	int ret;

	vdev = kzalloc(sizeof(*vdev), GFP_KERNEL);
	if (!vdev)
		return -ENOMEM;

	vfio_init_group_dev(vdev, &mdev->dev, &vfio_mdev_dev_ops);
	ret = vfio_register_group_dev(vdev);
	if (ret)
		goto out_uninit;

	dev_set_drvdata(&mdev->dev, vdev);
	return 0;

out_uninit:
	vfio_uninit_group_dev(vdev);
	kfree(vdev);
	return ret;
}

static void vfio_mdev_remove(struct mdev_device *mdev)
{
	struct vfio_device *vdev = dev_get_drvdata(&mdev->dev);

	vfio_unregister_group_dev(vdev);
	vfio_uninit_group_dev(vdev);
	kfree(vdev);
}

struct mdev_driver vfio_mdev_driver = {
	.driver = {
		.name = "vfio_mdev",
		.owner = THIS_MODULE,
		.mod_name = KBUILD_MODNAME,
	},
	.probe	= vfio_mdev_probe,
	.remove	= vfio_mdev_remove,
};
>>>>>>> upstream/android-13
