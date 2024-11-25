<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * File attributes for Mediated devices
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

#include <linux/sysfs.h>
#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uuid.h>
#include <linux/mdev.h>

#include "mdev_private.h"

/* Static functions */

static ssize_t mdev_type_attr_show(struct kobject *kobj,
				     struct attribute *__attr, char *buf)
{
	struct mdev_type_attribute *attr = to_mdev_type_attr(__attr);
	struct mdev_type *type = to_mdev_type(kobj);
	ssize_t ret = -EIO;

	if (attr->show)
<<<<<<< HEAD
		ret = attr->show(kobj, type->parent->dev, buf);
=======
		ret = attr->show(type, attr, buf);
>>>>>>> upstream/android-13
	return ret;
}

static ssize_t mdev_type_attr_store(struct kobject *kobj,
				      struct attribute *__attr,
				      const char *buf, size_t count)
{
	struct mdev_type_attribute *attr = to_mdev_type_attr(__attr);
	struct mdev_type *type = to_mdev_type(kobj);
	ssize_t ret = -EIO;

	if (attr->store)
<<<<<<< HEAD
		ret = attr->store(&type->kobj, type->parent->dev, buf, count);
=======
		ret = attr->store(type, attr, buf, count);
>>>>>>> upstream/android-13
	return ret;
}

static const struct sysfs_ops mdev_type_sysfs_ops = {
	.show = mdev_type_attr_show,
	.store = mdev_type_attr_store,
};

<<<<<<< HEAD
static ssize_t create_store(struct kobject *kobj, struct device *dev,
			    const char *buf, size_t count)
{
	char *str;
	uuid_le uuid;
=======
static ssize_t create_store(struct mdev_type *mtype,
			    struct mdev_type_attribute *attr, const char *buf,
			    size_t count)
{
	char *str;
	guid_t uuid;
>>>>>>> upstream/android-13
	int ret;

	if ((count < UUID_STRING_LEN) || (count > UUID_STRING_LEN + 1))
		return -EINVAL;

	str = kstrndup(buf, count, GFP_KERNEL);
	if (!str)
		return -ENOMEM;

<<<<<<< HEAD
	ret = uuid_le_to_bin(str, &uuid);
=======
	ret = guid_parse(str, &uuid);
>>>>>>> upstream/android-13
	kfree(str);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = mdev_device_create(kobj, dev, uuid);
=======
	ret = mdev_device_create(mtype, &uuid);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return count;
}

<<<<<<< HEAD
MDEV_TYPE_ATTR_WO(create);
=======
static MDEV_TYPE_ATTR_WO(create);
>>>>>>> upstream/android-13

static void mdev_type_release(struct kobject *kobj)
{
	struct mdev_type *type = to_mdev_type(kobj);

	pr_debug("Releasing group %s\n", kobj->name);
<<<<<<< HEAD
=======
	/* Pairs with the get in add_mdev_supported_type() */
	mdev_put_parent(type->parent);
>>>>>>> upstream/android-13
	kfree(type);
}

static struct kobj_type mdev_type_ktype = {
	.sysfs_ops = &mdev_type_sysfs_ops,
	.release = mdev_type_release,
};

<<<<<<< HEAD
struct mdev_type *add_mdev_supported_type(struct mdev_parent *parent,
					  struct attribute_group *group)
{
	struct mdev_type *type;
=======
static struct mdev_type *add_mdev_supported_type(struct mdev_parent *parent,
						 unsigned int type_group_id)
{
	struct mdev_type *type;
	struct attribute_group *group =
		parent->ops->supported_type_groups[type_group_id];
>>>>>>> upstream/android-13
	int ret;

	if (!group->name) {
		pr_err("%s: Type name empty!\n", __func__);
		return ERR_PTR(-EINVAL);
	}

	type = kzalloc(sizeof(*type), GFP_KERNEL);
	if (!type)
		return ERR_PTR(-ENOMEM);

	type->kobj.kset = parent->mdev_types_kset;
	type->parent = parent;
<<<<<<< HEAD
=======
	/* Pairs with the put in mdev_type_release() */
	mdev_get_parent(parent);
	type->type_group_id = type_group_id;
>>>>>>> upstream/android-13

	ret = kobject_init_and_add(&type->kobj, &mdev_type_ktype, NULL,
				   "%s-%s", dev_driver_string(parent->dev),
				   group->name);
	if (ret) {
		kobject_put(&type->kobj);
		return ERR_PTR(ret);
	}

	ret = sysfs_create_file(&type->kobj, &mdev_type_attr_create.attr);
	if (ret)
		goto attr_create_failed;

	type->devices_kobj = kobject_create_and_add("devices", &type->kobj);
	if (!type->devices_kobj) {
		ret = -ENOMEM;
		goto attr_devices_failed;
	}

	ret = sysfs_create_files(&type->kobj,
				 (const struct attribute **)group->attrs);
	if (ret) {
		ret = -ENOMEM;
		goto attrs_failed;
	}
<<<<<<< HEAD

	type->group = group;
=======
>>>>>>> upstream/android-13
	return type;

attrs_failed:
	kobject_put(type->devices_kobj);
attr_devices_failed:
	sysfs_remove_file(&type->kobj, &mdev_type_attr_create.attr);
attr_create_failed:
	kobject_del(&type->kobj);
	kobject_put(&type->kobj);
	return ERR_PTR(ret);
}

static void remove_mdev_supported_type(struct mdev_type *type)
{
<<<<<<< HEAD
	sysfs_remove_files(&type->kobj,
			   (const struct attribute **)type->group->attrs);
=======
	struct attribute_group *group =
		type->parent->ops->supported_type_groups[type->type_group_id];

	sysfs_remove_files(&type->kobj,
			   (const struct attribute **)group->attrs);
>>>>>>> upstream/android-13
	kobject_put(type->devices_kobj);
	sysfs_remove_file(&type->kobj, &mdev_type_attr_create.attr);
	kobject_del(&type->kobj);
	kobject_put(&type->kobj);
}

static int add_mdev_supported_type_groups(struct mdev_parent *parent)
{
	int i;

	for (i = 0; parent->ops->supported_type_groups[i]; i++) {
		struct mdev_type *type;

<<<<<<< HEAD
		type = add_mdev_supported_type(parent,
					parent->ops->supported_type_groups[i]);
=======
		type = add_mdev_supported_type(parent, i);
>>>>>>> upstream/android-13
		if (IS_ERR(type)) {
			struct mdev_type *ltype, *tmp;

			list_for_each_entry_safe(ltype, tmp, &parent->type_list,
						  next) {
				list_del(&ltype->next);
				remove_mdev_supported_type(ltype);
			}
			return PTR_ERR(type);
		}
		list_add(&type->next, &parent->type_list);
	}
	return 0;
}

/* mdev sysfs functions */
void parent_remove_sysfs_files(struct mdev_parent *parent)
{
	struct mdev_type *type, *tmp;

	list_for_each_entry_safe(type, tmp, &parent->type_list, next) {
		list_del(&type->next);
		remove_mdev_supported_type(type);
	}

	sysfs_remove_groups(&parent->dev->kobj, parent->ops->dev_attr_groups);
	kset_unregister(parent->mdev_types_kset);
}

int parent_create_sysfs_files(struct mdev_parent *parent)
{
	int ret;

	parent->mdev_types_kset = kset_create_and_add("mdev_supported_types",
					       NULL, &parent->dev->kobj);

	if (!parent->mdev_types_kset)
		return -ENOMEM;

	INIT_LIST_HEAD(&parent->type_list);

	ret = sysfs_create_groups(&parent->dev->kobj,
				  parent->ops->dev_attr_groups);
	if (ret)
		goto create_err;

	ret = add_mdev_supported_type_groups(parent);
	if (ret)
		sysfs_remove_groups(&parent->dev->kobj,
				    parent->ops->dev_attr_groups);
	else
		return ret;

create_err:
	kset_unregister(parent->mdev_types_kset);
	return ret;
}

static ssize_t remove_store(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t count)
{
<<<<<<< HEAD
=======
	struct mdev_device *mdev = to_mdev_device(dev);
>>>>>>> upstream/android-13
	unsigned long val;

	if (kstrtoul(buf, 0, &val) < 0)
		return -EINVAL;

	if (val && device_remove_file_self(dev, attr)) {
		int ret;

<<<<<<< HEAD
		ret = mdev_device_remove(dev, false);
		if (ret) {
			device_create_file(dev, attr);
			return ret;
		}
=======
		ret = mdev_device_remove(mdev);
		if (ret)
			return ret;
>>>>>>> upstream/android-13
	}

	return count;
}

static DEVICE_ATTR_WO(remove);

static const struct attribute *mdev_device_attrs[] = {
	&dev_attr_remove.attr,
	NULL,
};

<<<<<<< HEAD
int  mdev_create_sysfs_files(struct device *dev, struct mdev_type *type)
{
	int ret;

	ret = sysfs_create_link(type->devices_kobj, &dev->kobj, dev_name(dev));
	if (ret)
		return ret;

	ret = sysfs_create_link(&dev->kobj, &type->kobj, "mdev_type");
	if (ret)
		goto type_link_failed;

	ret = sysfs_create_files(&dev->kobj, mdev_device_attrs);
=======
int mdev_create_sysfs_files(struct mdev_device *mdev)
{
	struct mdev_type *type = mdev->type;
	struct kobject *kobj = &mdev->dev.kobj;
	int ret;

	ret = sysfs_create_link(type->devices_kobj, kobj, dev_name(&mdev->dev));
	if (ret)
		return ret;

	ret = sysfs_create_link(kobj, &type->kobj, "mdev_type");
	if (ret)
		goto type_link_failed;

	ret = sysfs_create_files(kobj, mdev_device_attrs);
>>>>>>> upstream/android-13
	if (ret)
		goto create_files_failed;

	return ret;

create_files_failed:
<<<<<<< HEAD
	sysfs_remove_link(&dev->kobj, "mdev_type");
type_link_failed:
	sysfs_remove_link(type->devices_kobj, dev_name(dev));
	return ret;
}

void mdev_remove_sysfs_files(struct device *dev, struct mdev_type *type)
{
	sysfs_remove_files(&dev->kobj, mdev_device_attrs);
	sysfs_remove_link(&dev->kobj, "mdev_type");
	sysfs_remove_link(type->devices_kobj, dev_name(dev));
=======
	sysfs_remove_link(kobj, "mdev_type");
type_link_failed:
	sysfs_remove_link(mdev->type->devices_kobj, dev_name(&mdev->dev));
	return ret;
}

void mdev_remove_sysfs_files(struct mdev_device *mdev)
{
	struct kobject *kobj = &mdev->dev.kobj;

	sysfs_remove_files(kobj, mdev_device_attrs);
	sysfs_remove_link(kobj, "mdev_type");
	sysfs_remove_link(mdev->type->devices_kobj, dev_name(&mdev->dev));
>>>>>>> upstream/android-13
}
