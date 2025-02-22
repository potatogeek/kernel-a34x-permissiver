<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
    V4L2 device support.

    Copyright (C) 2008  Hans Verkuil <hverkuil@xs4all.nl>

<<<<<<< HEAD
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/i2c.h>
#include <linux/slab.h>
#if defined(CONFIG_SPI)
#include <linux/spi/spi.h>
#endif
=======
#include <linux/slab.h>
>>>>>>> upstream/android-13
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>

int v4l2_device_register(struct device *dev, struct v4l2_device *v4l2_dev)
{
	if (v4l2_dev == NULL)
		return -EINVAL;

	INIT_LIST_HEAD(&v4l2_dev->subdevs);
	spin_lock_init(&v4l2_dev->lock);
	v4l2_prio_init(&v4l2_dev->prio);
	kref_init(&v4l2_dev->ref);
	get_device(dev);
	v4l2_dev->dev = dev;
	if (dev == NULL) {
		/* If dev == NULL, then name must be filled in by the caller */
		if (WARN_ON(!v4l2_dev->name[0]))
			return -EINVAL;
		return 0;
	}

	/* Set name to driver name + device name if it is empty. */
	if (!v4l2_dev->name[0])
		snprintf(v4l2_dev->name, sizeof(v4l2_dev->name), "%s %s",
			dev->driver->name, dev_name(dev));
	if (!dev_get_drvdata(dev))
		dev_set_drvdata(dev, v4l2_dev);
	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_device_register);

static void v4l2_device_release(struct kref *ref)
{
	struct v4l2_device *v4l2_dev =
		container_of(ref, struct v4l2_device, ref);

	if (v4l2_dev->release)
		v4l2_dev->release(v4l2_dev);
}

int v4l2_device_put(struct v4l2_device *v4l2_dev)
{
	return kref_put(&v4l2_dev->ref, v4l2_device_release);
}
EXPORT_SYMBOL_GPL(v4l2_device_put);

int v4l2_device_set_name(struct v4l2_device *v4l2_dev, const char *basename,
						atomic_t *instance)
{
	int num = atomic_inc_return(instance) - 1;
	int len = strlen(basename);

	if (basename[len - 1] >= '0' && basename[len - 1] <= '9')
		snprintf(v4l2_dev->name, sizeof(v4l2_dev->name),
				"%s-%d", basename, num);
	else
		snprintf(v4l2_dev->name, sizeof(v4l2_dev->name),
				"%s%d", basename, num);
	return num;
}
EXPORT_SYMBOL_GPL(v4l2_device_set_name);

void v4l2_device_disconnect(struct v4l2_device *v4l2_dev)
{
	if (v4l2_dev->dev == NULL)
		return;

	if (dev_get_drvdata(v4l2_dev->dev) == v4l2_dev)
		dev_set_drvdata(v4l2_dev->dev, NULL);
	put_device(v4l2_dev->dev);
	v4l2_dev->dev = NULL;
}
EXPORT_SYMBOL_GPL(v4l2_device_disconnect);

void v4l2_device_unregister(struct v4l2_device *v4l2_dev)
{
	struct v4l2_subdev *sd, *next;

	/* Just return if v4l2_dev is NULL or if it was already
	 * unregistered before. */
	if (v4l2_dev == NULL || !v4l2_dev->name[0])
		return;
	v4l2_device_disconnect(v4l2_dev);

	/* Unregister subdevs */
	list_for_each_entry_safe(sd, next, &v4l2_dev->subdevs, list) {
		v4l2_device_unregister_subdev(sd);
<<<<<<< HEAD
#if IS_ENABLED(CONFIG_I2C)
		if (sd->flags & V4L2_SUBDEV_FL_IS_I2C) {
			struct i2c_client *client = v4l2_get_subdevdata(sd);

			/*
			 * We need to unregister the i2c client
			 * explicitly. We cannot rely on
			 * i2c_del_adapter to always unregister
			 * clients for us, since if the i2c bus is a
			 * platform bus, then it is never deleted.
			 *
			 * Device tree or ACPI based devices must not
			 * be unregistered as they have not been
			 * registered by us, and would not be
			 * re-created by just probing the V4L2 driver.
			 */
			if (client &&
			    !client->dev.of_node && !client->dev.fwnode)
				i2c_unregister_device(client);
			continue;
		}
#endif
#if defined(CONFIG_SPI)
		if (sd->flags & V4L2_SUBDEV_FL_IS_SPI) {
			struct spi_device *spi = v4l2_get_subdevdata(sd);

			if (spi && !spi->dev.of_node && !spi->dev.fwnode)
				spi_unregister_device(spi);
			continue;
		}
#endif
=======
		if (sd->flags & V4L2_SUBDEV_FL_IS_I2C)
			v4l2_i2c_subdev_unregister(sd);
		else if (sd->flags & V4L2_SUBDEV_FL_IS_SPI)
			v4l2_spi_subdev_unregister(sd);
>>>>>>> upstream/android-13
	}
	/* Mark as unregistered, thus preventing duplicate unregistrations */
	v4l2_dev->name[0] = '\0';
}
EXPORT_SYMBOL_GPL(v4l2_device_unregister);

int v4l2_device_register_subdev(struct v4l2_device *v4l2_dev,
				struct v4l2_subdev *sd)
{
<<<<<<< HEAD
#if defined(CONFIG_MEDIA_CONTROLLER)
	struct media_entity *entity = &sd->entity;
#endif
=======
>>>>>>> upstream/android-13
	int err;

	/* Check for valid input */
	if (!v4l2_dev || !sd || sd->v4l2_dev || !sd->name[0])
		return -EINVAL;

	/*
	 * The reason to acquire the module here is to avoid unloading
	 * a module of sub-device which is registered to a media
	 * device. To make it possible to unload modules for media
	 * devices that also register sub-devices, do not
	 * try_module_get() such sub-device owners.
	 */
	sd->owner_v4l2_dev = v4l2_dev->dev && v4l2_dev->dev->driver &&
		sd->owner == v4l2_dev->dev->driver->owner;

	if (!sd->owner_v4l2_dev && !try_module_get(sd->owner))
		return -ENODEV;

	sd->v4l2_dev = v4l2_dev;
	/* This just returns 0 if either of the two args is NULL */
	err = v4l2_ctrl_add_handler(v4l2_dev->ctrl_handler, sd->ctrl_handler,
				    NULL, true);
	if (err)
		goto error_module;

#if defined(CONFIG_MEDIA_CONTROLLER)
	/* Register the entity. */
	if (v4l2_dev->mdev) {
<<<<<<< HEAD
		err = media_device_register_entity(v4l2_dev->mdev, entity);
=======
		err = media_device_register_entity(v4l2_dev->mdev, &sd->entity);
>>>>>>> upstream/android-13
		if (err < 0)
			goto error_module;
	}
#endif

	if (sd->internal_ops && sd->internal_ops->registered) {
		err = sd->internal_ops->registered(sd);
		if (err)
			goto error_unregister;
	}

	spin_lock(&v4l2_dev->lock);
	list_add_tail(&sd->list, &v4l2_dev->subdevs);
	spin_unlock(&v4l2_dev->lock);

	return 0;

error_unregister:
#if defined(CONFIG_MEDIA_CONTROLLER)
<<<<<<< HEAD
	media_device_unregister_entity(entity);
=======
	media_device_unregister_entity(&sd->entity);
>>>>>>> upstream/android-13
#endif
error_module:
	if (!sd->owner_v4l2_dev)
		module_put(sd->owner);
	sd->v4l2_dev = NULL;
	return err;
}
EXPORT_SYMBOL_GPL(v4l2_device_register_subdev);

<<<<<<< HEAD
static void v4l2_device_release_subdev_node(struct video_device *vdev)
{
	struct v4l2_subdev *sd = video_get_drvdata(vdev);
	sd->devnode = NULL;
	kfree(vdev);
}

int v4l2_device_register_subdev_nodes(struct v4l2_device *v4l2_dev)
=======
static void v4l2_subdev_release(struct v4l2_subdev *sd)
{
	struct module *owner = !sd->owner_v4l2_dev ? sd->owner : NULL;

	if (sd->internal_ops && sd->internal_ops->release)
		sd->internal_ops->release(sd);
	sd->devnode = NULL;
	module_put(owner);
}

static void v4l2_device_release_subdev_node(struct video_device *vdev)
{
	v4l2_subdev_release(video_get_drvdata(vdev));
	kfree(vdev);
}

int __v4l2_device_register_subdev_nodes(struct v4l2_device *v4l2_dev,
					bool read_only)
>>>>>>> upstream/android-13
{
	struct video_device *vdev;
	struct v4l2_subdev *sd;
	int err;

	/* Register a device node for every subdev marked with the
	 * V4L2_SUBDEV_FL_HAS_DEVNODE flag.
	 */
	list_for_each_entry(sd, &v4l2_dev->subdevs, list) {
		if (!(sd->flags & V4L2_SUBDEV_FL_HAS_DEVNODE))
			continue;

		if (sd->devnode)
			continue;

		vdev = kzalloc(sizeof(*vdev), GFP_KERNEL);
		if (!vdev) {
			err = -ENOMEM;
			goto clean_up;
		}

		video_set_drvdata(vdev, sd);
<<<<<<< HEAD
		strlcpy(vdev->name, sd->name, sizeof(vdev->name));
=======
		strscpy(vdev->name, sd->name, sizeof(vdev->name));
		vdev->dev_parent = sd->dev;
>>>>>>> upstream/android-13
		vdev->v4l2_dev = v4l2_dev;
		vdev->fops = &v4l2_subdev_fops;
		vdev->release = v4l2_device_release_subdev_node;
		vdev->ctrl_handler = sd->ctrl_handler;
<<<<<<< HEAD
		err = __video_register_device(vdev, VFL_TYPE_SUBDEV, -1, 1,
					      sd->owner);
		if (err < 0) {
			kfree(vdev);
			goto clean_up;
		}
		sd->devnode = vdev;
=======
		if (read_only)
			set_bit(V4L2_FL_SUBDEV_RO_DEVNODE, &vdev->flags);
		sd->devnode = vdev;
		err = __video_register_device(vdev, VFL_TYPE_SUBDEV, -1, 1,
					      sd->owner);
		if (err < 0) {
			sd->devnode = NULL;
			kfree(vdev);
			goto clean_up;
		}
>>>>>>> upstream/android-13
#if defined(CONFIG_MEDIA_CONTROLLER)
		sd->entity.info.dev.major = VIDEO_MAJOR;
		sd->entity.info.dev.minor = vdev->minor;

		/* Interface is created by __video_register_device() */
		if (vdev->v4l2_dev->mdev) {
			struct media_link *link;

			link = media_create_intf_link(&sd->entity,
						      &vdev->intf_devnode->intf,
						      MEDIA_LNK_FL_ENABLED |
						      MEDIA_LNK_FL_IMMUTABLE);
			if (!link) {
				err = -ENOMEM;
				goto clean_up;
			}
		}
#endif
	}
	return 0;

clean_up:
	list_for_each_entry(sd, &v4l2_dev->subdevs, list) {
		if (!sd->devnode)
			break;
		video_unregister_device(sd->devnode);
	}

	return err;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(v4l2_device_register_subdev_nodes);
=======
EXPORT_SYMBOL_GPL(__v4l2_device_register_subdev_nodes);
>>>>>>> upstream/android-13

void v4l2_device_unregister_subdev(struct v4l2_subdev *sd)
{
	struct v4l2_device *v4l2_dev;

	/* return if it isn't registered */
	if (sd == NULL || sd->v4l2_dev == NULL)
		return;

	v4l2_dev = sd->v4l2_dev;

	spin_lock(&v4l2_dev->lock);
	list_del(&sd->list);
	spin_unlock(&v4l2_dev->lock);

	if (sd->internal_ops && sd->internal_ops->unregistered)
		sd->internal_ops->unregistered(sd);
	sd->v4l2_dev = NULL;

#if defined(CONFIG_MEDIA_CONTROLLER)
	if (v4l2_dev->mdev) {
		/*
		 * No need to explicitly remove links, as both pads and
		 * links are removed by the function below, in the right order
		 */
		media_device_unregister_entity(&sd->entity);
	}
#endif
<<<<<<< HEAD
	video_unregister_device(sd->devnode);
	if (!sd->owner_v4l2_dev)
		module_put(sd->owner);
=======
	if (sd->devnode)
		video_unregister_device(sd->devnode);
	else
		v4l2_subdev_release(sd);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(v4l2_device_unregister_subdev);
