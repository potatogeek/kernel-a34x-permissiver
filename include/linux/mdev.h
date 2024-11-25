<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Mediated device definition
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

#ifndef MDEV_H
#define MDEV_H

<<<<<<< HEAD
struct mdev_device;
=======
struct mdev_type;

struct mdev_device {
	struct device dev;
	guid_t uuid;
	void *driver_data;
	struct list_head next;
	struct mdev_type *type;
	struct device *iommu_device;
	bool active;
};

static inline struct mdev_device *to_mdev_device(struct device *dev)
{
	return container_of(dev, struct mdev_device, dev);
}

/*
 * Called by the parent device driver to set the device which represents
 * this mdev in iommu protection scope. By default, the iommu device is
 * NULL, that indicates using vendor defined isolation.
 *
 * @dev: the mediated device that iommu will isolate.
 * @iommu_device: a pci device which represents the iommu for @dev.
 */
static inline void mdev_set_iommu_device(struct mdev_device *mdev,
					 struct device *iommu_device)
{
	mdev->iommu_device = iommu_device;
}

static inline struct device *mdev_get_iommu_device(struct mdev_device *mdev)
{
	return mdev->iommu_device;
}

unsigned int mdev_get_type_group_id(struct mdev_device *mdev);
unsigned int mtype_get_type_group_id(struct mdev_type *mtype);
struct device *mtype_get_parent_dev(struct mdev_type *mtype);
>>>>>>> upstream/android-13

/**
 * struct mdev_parent_ops - Structure to be registered for each parent device to
 * register the device to mdev module.
 *
 * @owner:		The module owner.
<<<<<<< HEAD
=======
 * @device_driver:	Which device driver to probe() on newly created devices
>>>>>>> upstream/android-13
 * @dev_attr_groups:	Attributes of the parent device.
 * @mdev_attr_groups:	Attributes of the mediated device.
 * @supported_type_groups: Attributes to define supported types. It is mandatory
 *			to provide supported types.
 * @create:		Called to allocate basic resources in parent device's
 *			driver for a particular mediated device. It is
 *			mandatory to provide create ops.
<<<<<<< HEAD
 *			@kobj: kobject of type for which 'create' is called.
 *			@mdev: mdev_device structure on of mediated device
 *			      that is being created
 *			Returns integer: success (0) or error (< 0)
 * @remove:		Called to free resources in parent device's driver for a
=======
 *			@mdev: mdev_device structure on of mediated device
 *			      that is being created
 *			Returns integer: success (0) or error (< 0)
 * @remove:		Called to free resources in parent device's driver for
>>>>>>> upstream/android-13
 *			a mediated device. It is mandatory to provide 'remove'
 *			ops.
 *			@mdev: mdev_device device structure which is being
 *			       destroyed
 *			Returns integer: success (0) or error (< 0)
<<<<<<< HEAD
 * @open:		Open mediated device.
 *			@mdev: mediated device.
 *			Returns integer: success (0) or error (< 0)
 * @release:		release mediated device
 *			@mdev: mediated device.
=======
>>>>>>> upstream/android-13
 * @read:		Read emulation callback
 *			@mdev: mediated device structure
 *			@buf: read buffer
 *			@count: number of bytes to read
 *			@ppos: address.
 *			Retuns number on bytes read on success or error.
 * @write:		Write emulation callback
 *			@mdev: mediated device structure
 *			@buf: write buffer
 *			@count: number of bytes to be written
 *			@ppos: address.
 *			Retuns number on bytes written on success or error.
 * @ioctl:		IOCTL callback
 *			@mdev: mediated device structure
 *			@cmd: ioctl command
 *			@arg: arguments to ioctl
 * @mmap:		mmap callback
 *			@mdev: mediated device structure
 *			@vma: vma structure
<<<<<<< HEAD
=======
 * @request:		request callback to release device
 *			@mdev: mediated device structure
 *			@count: request sequence number
>>>>>>> upstream/android-13
 * Parent device that support mediated device should be registered with mdev
 * module with mdev_parent_ops structure.
 **/
struct mdev_parent_ops {
	struct module   *owner;
<<<<<<< HEAD
=======
	struct mdev_driver *device_driver;
>>>>>>> upstream/android-13
	const struct attribute_group **dev_attr_groups;
	const struct attribute_group **mdev_attr_groups;
	struct attribute_group **supported_type_groups;

<<<<<<< HEAD
	int     (*create)(struct kobject *kobj, struct mdev_device *mdev);
	int     (*remove)(struct mdev_device *mdev);
	int     (*open)(struct mdev_device *mdev);
	void    (*release)(struct mdev_device *mdev);
=======
	int     (*create)(struct mdev_device *mdev);
	int     (*remove)(struct mdev_device *mdev);
	int     (*open_device)(struct mdev_device *mdev);
	void    (*close_device)(struct mdev_device *mdev);
>>>>>>> upstream/android-13
	ssize_t (*read)(struct mdev_device *mdev, char __user *buf,
			size_t count, loff_t *ppos);
	ssize_t (*write)(struct mdev_device *mdev, const char __user *buf,
			 size_t count, loff_t *ppos);
	long	(*ioctl)(struct mdev_device *mdev, unsigned int cmd,
			 unsigned long arg);
	int	(*mmap)(struct mdev_device *mdev, struct vm_area_struct *vma);
<<<<<<< HEAD
=======
	void	(*request)(struct mdev_device *mdev, unsigned int count);
>>>>>>> upstream/android-13
};

/* interface for exporting mdev supported type attributes */
struct mdev_type_attribute {
	struct attribute attr;
<<<<<<< HEAD
	ssize_t (*show)(struct kobject *kobj, struct device *dev, char *buf);
	ssize_t (*store)(struct kobject *kobj, struct device *dev,
			 const char *buf, size_t count);
=======
	ssize_t (*show)(struct mdev_type *mtype,
			struct mdev_type_attribute *attr, char *buf);
	ssize_t (*store)(struct mdev_type *mtype,
			 struct mdev_type_attribute *attr, const char *buf,
			 size_t count);
>>>>>>> upstream/android-13
};

#define MDEV_TYPE_ATTR(_name, _mode, _show, _store)		\
struct mdev_type_attribute mdev_type_attr_##_name =		\
	__ATTR(_name, _mode, _show, _store)
#define MDEV_TYPE_ATTR_RW(_name) \
	struct mdev_type_attribute mdev_type_attr_##_name = __ATTR_RW(_name)
#define MDEV_TYPE_ATTR_RO(_name) \
	struct mdev_type_attribute mdev_type_attr_##_name = __ATTR_RO(_name)
#define MDEV_TYPE_ATTR_WO(_name) \
	struct mdev_type_attribute mdev_type_attr_##_name = __ATTR_WO(_name)

/**
 * struct mdev_driver - Mediated device driver
<<<<<<< HEAD
 * @name: driver name
=======
>>>>>>> upstream/android-13
 * @probe: called when new device created
 * @remove: called when device removed
 * @driver: device driver structure
 *
 **/
struct mdev_driver {
<<<<<<< HEAD
	const char *name;
	int  (*probe)(struct device *dev);
	void (*remove)(struct device *dev);
	struct device_driver driver;
};

#define to_mdev_driver(drv)	container_of(drv, struct mdev_driver, driver)

extern void *mdev_get_drvdata(struct mdev_device *mdev);
extern void mdev_set_drvdata(struct mdev_device *mdev, void *data);
extern uuid_le mdev_uuid(struct mdev_device *mdev);

extern struct bus_type mdev_bus_type;

extern int  mdev_register_device(struct device *dev,
				 const struct mdev_parent_ops *ops);
extern void mdev_unregister_device(struct device *dev);

extern int  mdev_register_driver(struct mdev_driver *drv, struct module *owner);
extern void mdev_unregister_driver(struct mdev_driver *drv);

extern struct device *mdev_parent_dev(struct mdev_device *mdev);
extern struct device *mdev_dev(struct mdev_device *mdev);
extern struct mdev_device *mdev_from_dev(struct device *dev);
=======
	int (*probe)(struct mdev_device *dev);
	void (*remove)(struct mdev_device *dev);
	struct device_driver driver;
};

static inline void *mdev_get_drvdata(struct mdev_device *mdev)
{
	return mdev->driver_data;
}
static inline void mdev_set_drvdata(struct mdev_device *mdev, void *data)
{
	mdev->driver_data = data;
}
static inline const guid_t *mdev_uuid(struct mdev_device *mdev)
{
	return &mdev->uuid;
}

extern struct bus_type mdev_bus_type;

int mdev_register_device(struct device *dev, const struct mdev_parent_ops *ops);
void mdev_unregister_device(struct device *dev);

int mdev_register_driver(struct mdev_driver *drv);
void mdev_unregister_driver(struct mdev_driver *drv);

struct device *mdev_parent_dev(struct mdev_device *mdev);
static inline struct device *mdev_dev(struct mdev_device *mdev)
{
	return &mdev->dev;
}
static inline struct mdev_device *mdev_from_dev(struct device *dev)
{
	return dev->bus == &mdev_bus_type ? to_mdev_device(dev) : NULL;
}
>>>>>>> upstream/android-13

#endif /* MDEV_H */
