// SPDX-License-Identifier: GPL-2.0

#ifndef __LINUX_USB_ROLE_H
#define __LINUX_USB_ROLE_H

#include <linux/device.h>

struct usb_role_switch;

enum usb_role {
	USB_ROLE_NONE,
	USB_ROLE_HOST,
	USB_ROLE_DEVICE,
};

typedef int (*usb_role_switch_set_t)(struct device *dev, enum usb_role role);
typedef enum usb_role (*usb_role_switch_get_t)(struct device *dev);


struct usb_role_switch_desc {
	struct device *usb2_port;
	struct device *usb3_port;
	struct device *udc;
	usb_role_switch_set_t set;
	usb_role_switch_get_t get;
	bool allow_userspace_control;
};

int usb_role_switch_set_role(struct usb_role_switch *sw, enum usb_role role);
enum usb_role usb_role_switch_get_role(struct usb_role_switch *sw);
struct usb_role_switch *usb_role_switch_get(struct device *dev);
void usb_role_switch_put(struct usb_role_switch *sw);

struct usb_role_switch *
usb_role_switch_register(struct device *parent,
			 const struct usb_role_switch_desc *desc);
void usb_role_switch_unregister(struct usb_role_switch *sw);

#endif 
