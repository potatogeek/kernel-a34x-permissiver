// SPDX-License-Identifier: GPL-2.0
/*
 * All the USB notify logic
 *
 * (C) Copyright 2005 Greg Kroah-Hartman <gregkh@suse.de>
 *
 * notifier functions originally based on those in kernel/sys.c
 * but fixed up to not be so broken.
 *
 * Released under the GPLv2 only.
 */


#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/notifier.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include "usb.h"

static BLOCKING_NOTIFIER_HEAD(usb_notifier_list);
<<<<<<< HEAD
static ATOMIC_NOTIFIER_HEAD(usb_atomic_notifier_list);
=======
>>>>>>> upstream/android-13

/**
 * usb_register_notify - register a notifier callback whenever a usb change happens
 * @nb: pointer to the notifier block for the callback events.
 *
 * These changes are either USB devices or busses being added or removed.
 */
void usb_register_notify(struct notifier_block *nb)
{
	blocking_notifier_chain_register(&usb_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_register_notify);

/**
 * usb_unregister_notify - unregister a notifier callback
 * @nb: pointer to the notifier block for the callback events.
 *
 * usb_register_notify() must have been previously called for this function
 * to work properly.
 */
void usb_unregister_notify(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&usb_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_unregister_notify);


void usb_notify_add_device(struct usb_device *udev)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_DEVICE_ADD, udev);
}

void usb_notify_remove_device(struct usb_device *udev)
{
<<<<<<< HEAD
	/* Protect against simultaneous usbfs open */
	mutex_lock(&usbfs_mutex);
	blocking_notifier_call_chain(&usb_notifier_list,
			USB_DEVICE_REMOVE, udev);
	mutex_unlock(&usbfs_mutex);
=======
	blocking_notifier_call_chain(&usb_notifier_list,
			USB_DEVICE_REMOVE, udev);
>>>>>>> upstream/android-13
}

void usb_notify_add_bus(struct usb_bus *ubus)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_BUS_ADD, ubus);
}

void usb_notify_remove_bus(struct usb_bus *ubus)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_BUS_REMOVE, ubus);
}
<<<<<<< HEAD

/**
 * usb_register_atomic_notify - register a atomic notifier callback whenever a
 * HC dies
 * @nb: pointer to the atomic notifier block for the callback events.
 *
 */
void usb_register_atomic_notify(struct notifier_block *nb)
{
	atomic_notifier_chain_register(&usb_atomic_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_register_atomic_notify);

/**
 * usb_unregister_atomic_notify - unregister a atomic notifier callback
 * @nb: pointer to the notifier block for the callback events.
 *
 */
void usb_unregister_atomic_notify(struct notifier_block *nb)
{
	atomic_notifier_chain_unregister(&usb_atomic_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_unregister_atomic_notify);


void usb_atomic_notify_dead_bus(struct usb_bus *ubus)
{
	atomic_notifier_call_chain(&usb_atomic_notifier_list, USB_BUS_DIED,
					 ubus);
}
=======
>>>>>>> upstream/android-13
