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

<<<<<<< HEAD
typedef int (*usb_role_switch_set_t)(struct device *dev, enum usb_role role);
typedef enum usb_role (*usb_role_switch_get_t)(struct device *dev);

/**
 * struct usb_role_switch_desc - USB Role Switch Descriptor
=======
typedef int (*usb_role_switch_set_t)(struct usb_role_switch *sw,
				     enum usb_role role);
typedef enum usb_role (*usb_role_switch_get_t)(struct usb_role_switch *sw);

/**
 * struct usb_role_switch_desc - USB Role Switch Descriptor
 * @fwnode: The device node to be associated with the role switch
>>>>>>> upstream/android-13
 * @usb2_port: Optional reference to the host controller port device (USB2)
 * @usb3_port: Optional reference to the host controller port device (USB3)
 * @udc: Optional reference to the peripheral controller device
 * @set: Callback for setting the role
 * @get: Callback for getting the role (optional)
 * @allow_userspace_control: If true userspace may change the role through sysfs
<<<<<<< HEAD
=======
 * @driver_data: Private data pointer
 * @name: Name for the switch (optional)
>>>>>>> upstream/android-13
 *
 * @usb2_port and @usb3_port will point to the USB host port and @udc to the USB
 * device controller behind the USB connector with the role switch. If
 * @usb2_port, @usb3_port and @udc are included in the description, the
 * reference count for them should be incremented by the caller of
 * usb_role_switch_register() before registering the switch.
 */
struct usb_role_switch_desc {
<<<<<<< HEAD
=======
	struct fwnode_handle *fwnode;
>>>>>>> upstream/android-13
	struct device *usb2_port;
	struct device *usb3_port;
	struct device *udc;
	usb_role_switch_set_t set;
	usb_role_switch_get_t get;
	bool allow_userspace_control;
<<<<<<< HEAD
};

int usb_role_switch_set_role(struct usb_role_switch *sw, enum usb_role role);
enum usb_role usb_role_switch_get_role(struct usb_role_switch *sw);
struct usb_role_switch *usb_role_switch_get(struct device *dev);
void usb_role_switch_put(struct usb_role_switch *sw);

struct usb_role_switch *
=======
	void *driver_data;
	const char *name;
};


#if IS_ENABLED(CONFIG_USB_ROLE_SWITCH)
int usb_role_switch_set_role(struct usb_role_switch *sw, enum usb_role role);
enum usb_role usb_role_switch_get_role(struct usb_role_switch *sw);
struct usb_role_switch *usb_role_switch_get(struct device *dev);
struct usb_role_switch *fwnode_usb_role_switch_get(struct fwnode_handle *node);
void usb_role_switch_put(struct usb_role_switch *sw);

struct usb_role_switch *
usb_role_switch_find_by_fwnode(const struct fwnode_handle *fwnode);

struct usb_role_switch *
>>>>>>> upstream/android-13
usb_role_switch_register(struct device *parent,
			 const struct usb_role_switch_desc *desc);
void usb_role_switch_unregister(struct usb_role_switch *sw);

<<<<<<< HEAD
=======
void usb_role_switch_set_drvdata(struct usb_role_switch *sw, void *data);
void *usb_role_switch_get_drvdata(struct usb_role_switch *sw);
const char *usb_role_string(enum usb_role role);
#else
static inline int usb_role_switch_set_role(struct usb_role_switch *sw,
		enum usb_role role)
{
	return 0;
}

static inline enum usb_role usb_role_switch_get_role(struct usb_role_switch *sw)
{
	return USB_ROLE_NONE;
}

static inline struct usb_role_switch *usb_role_switch_get(struct device *dev)
{
	return ERR_PTR(-ENODEV);
}

static inline struct usb_role_switch *
fwnode_usb_role_switch_get(struct fwnode_handle *node)
{
	return ERR_PTR(-ENODEV);
}

static inline void usb_role_switch_put(struct usb_role_switch *sw) { }

static inline struct usb_role_switch *
usb_role_switch_find_by_fwnode(const struct fwnode_handle *fwnode)
{
	return NULL;
}

static inline struct usb_role_switch *
usb_role_switch_register(struct device *parent,
			 const struct usb_role_switch_desc *desc)
{
	return ERR_PTR(-ENODEV);
}

static inline void usb_role_switch_unregister(struct usb_role_switch *sw) { }

static inline void
usb_role_switch_set_drvdata(struct usb_role_switch *sw, void *data)
{
}

static inline void *usb_role_switch_get_drvdata(struct usb_role_switch *sw)
{
	return NULL;
}

static inline const char *usb_role_string(enum usb_role role)
{
	return "unknown";
}

#endif

>>>>>>> upstream/android-13
#endif /* __LINUX_USB_ROLE_H */
