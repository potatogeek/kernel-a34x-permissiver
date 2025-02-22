<<<<<<< HEAD
/*
 * Copyright (C) 2012 Red Hat
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License v2. See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/module.h>
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Red Hat
 */

#include <linux/module.h>

#include <drm/drm_crtc_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_file.h>
#include <drm/drm_gem_shmem_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_print.h>

>>>>>>> upstream/android-13
#include "udl_drv.h"

static int udl_usb_suspend(struct usb_interface *interface,
			   pm_message_t message)
{
	struct drm_device *dev = usb_get_intfdata(interface);

<<<<<<< HEAD
	drm_kms_helper_poll_disable(dev);
	return 0;
=======
	return drm_mode_config_helper_suspend(dev);
>>>>>>> upstream/android-13
}

static int udl_usb_resume(struct usb_interface *interface)
{
	struct drm_device *dev = usb_get_intfdata(interface);

<<<<<<< HEAD
	drm_kms_helper_poll_enable(dev);
	udl_modeset_restore(dev);
	return 0;
}

static const struct vm_operations_struct udl_gem_vm_ops = {
	.fault = udl_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static const struct file_operations udl_driver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.mmap = udl_drm_gem_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.unlocked_ioctl	= drm_ioctl,
	.release = drm_release,
	.compat_ioctl = drm_compat_ioctl,
	.llseek = noop_llseek,
};

static void udl_driver_release(struct drm_device *dev)
{
	udl_fini(dev);
	udl_modeset_cleanup(dev);
	drm_dev_fini(dev);
	kfree(dev);
}

static struct drm_driver driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_PRIME,
	.release = udl_driver_release,

	/* gem hooks */
	.gem_free_object_unlocked = udl_gem_free_object,
	.gem_vm_ops = &udl_gem_vm_ops,

	.dumb_create = udl_dumb_create,
	.dumb_map_offset = udl_gem_mmap,
	.fops = &udl_driver_fops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_export = udl_gem_prime_export,
	.gem_prime_import = udl_gem_prime_import,
=======
	return drm_mode_config_helper_resume(dev);
}

/*
 * FIXME: Dma-buf sharing requires DMA support by the importing device.
 *        This function is a workaround to make USB devices work as well.
 *        See todo.rst for how to fix the issue in the dma-buf framework.
 */
static struct drm_gem_object *udl_driver_gem_prime_import(struct drm_device *dev,
							  struct dma_buf *dma_buf)
{
	struct udl_device *udl = to_udl(dev);

	if (!udl->dmadev)
		return ERR_PTR(-ENODEV);

	return drm_gem_prime_import_dev(dev, dma_buf, udl->dmadev);
}

DEFINE_DRM_GEM_FOPS(udl_driver_fops);

static const struct drm_driver driver = {
	.driver_features = DRIVER_ATOMIC | DRIVER_GEM | DRIVER_MODESET,

	/* GEM hooks */
	.fops = &udl_driver_fops,
	DRM_GEM_SHMEM_DRIVER_OPS,
	.gem_prime_import = udl_driver_gem_prime_import,
>>>>>>> upstream/android-13

	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

static struct udl_device *udl_driver_create(struct usb_interface *interface)
{
<<<<<<< HEAD
	struct usb_device *udev = interface_to_usbdev(interface);
	struct udl_device *udl;
	int r;

	udl = kzalloc(sizeof(*udl), GFP_KERNEL);
	if (!udl)
		return ERR_PTR(-ENOMEM);

	r = drm_dev_init(&udl->drm, &driver, &interface->dev);
	if (r) {
		kfree(udl);
		return ERR_PTR(r);
	}

	udl->udev = udev;
	udl->drm.dev_private = udl;

	r = udl_init(udl);
	if (r) {
		drm_dev_fini(&udl->drm);
		kfree(udl);
		return ERR_PTR(r);
	}

	usb_set_intfdata(interface, udl);
=======
	struct udl_device *udl;
	int r;

	udl = devm_drm_dev_alloc(&interface->dev, &driver,
				 struct udl_device, drm);
	if (IS_ERR(udl))
		return udl;

	r = udl_init(udl);
	if (r)
		return ERR_PTR(r);

	usb_set_intfdata(interface, udl);

>>>>>>> upstream/android-13
	return udl;
}

static int udl_usb_probe(struct usb_interface *interface,
			 const struct usb_device_id *id)
{
	int r;
	struct udl_device *udl;

	udl = udl_driver_create(interface);
	if (IS_ERR(udl))
		return PTR_ERR(udl);

	r = drm_dev_register(&udl->drm, 0);
	if (r)
<<<<<<< HEAD
		goto err_free;

	DRM_INFO("Initialized udl on minor %d\n", udl->drm.primary->index);

	return 0;

err_free:
	drm_dev_put(&udl->drm);
	return r;
=======
		return r;

	DRM_INFO("Initialized udl on minor %d\n", udl->drm.primary->index);

	drm_fbdev_generic_setup(&udl->drm, 0);

	return 0;
>>>>>>> upstream/android-13
}

static void udl_usb_disconnect(struct usb_interface *interface)
{
	struct drm_device *dev = usb_get_intfdata(interface);

<<<<<<< HEAD
	drm_kms_helper_poll_disable(dev);
	udl_fbdev_unplug(dev);
=======
	drm_kms_helper_poll_fini(dev);
>>>>>>> upstream/android-13
	udl_drop_usb(dev);
	drm_dev_unplug(dev);
}

/*
 * There are many DisplayLink-based graphics products, all with unique PIDs.
 * So we match on DisplayLink's VID + Vendor-Defined Interface Class (0xff)
 * We also require a match on SubClass (0x00) and Protocol (0x00),
 * which is compatible with all known USB 2.0 era graphics chips and firmware,
 * but allows DisplayLink to increment those for any future incompatible chips
 */
static const struct usb_device_id id_table[] = {
	{.idVendor = 0x17e9, .bInterfaceClass = 0xff,
	 .bInterfaceSubClass = 0x00,
	 .bInterfaceProtocol = 0x00,
	 .match_flags = USB_DEVICE_ID_MATCH_VENDOR |
			USB_DEVICE_ID_MATCH_INT_CLASS |
			USB_DEVICE_ID_MATCH_INT_SUBCLASS |
			USB_DEVICE_ID_MATCH_INT_PROTOCOL,},
	{},
};
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver udl_driver = {
	.name = "udl",
	.probe = udl_usb_probe,
	.disconnect = udl_usb_disconnect,
	.suspend = udl_usb_suspend,
	.resume = udl_usb_resume,
	.id_table = id_table,
};
module_usb_driver(udl_driver);
MODULE_LICENSE("GPL");
