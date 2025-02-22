/*
 * Copyright (C) 2018-2019 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *
 */

#ifndef __IF_CB_MANAGER_H__
#define __IF_CB_MANAGER_H__

struct usb_ops {
	void (*usb_set_vbus_current)(void *data, int state);
	int (*usb_restart_host_mode)(void *data, int lanes);
};

struct muic_ops {
	int (*muic_check_usb_killer)(void *data);
	void (*muic_set_bypass)(void *data, int enable);
	void (*muic_set_bc12)(void *data, int enable);
};

struct usbpd_ops {
	int (*usbpd_sbu_test_read)(void *data);
	void (*usbpd_set_host_on)(void *data, int mode);
	void (*usbpd_cc_control_command)(void *data, int is_off);
<<<<<<< HEAD
	void (*usbpd_wait_entermode)(void *data, int on);
};

struct lvs_ops {
	int (*lvs_cc_attach)(void *data, int on);
=======
>>>>>>> upstream/android-13
};

struct usb_dev {
	const struct usb_ops *ops;
	void *data;
};

struct muic_dev {
	const struct muic_ops *ops;
	void *data;
};

struct usbpd_dev {
	const struct usbpd_ops *ops;
	void *data;
};

<<<<<<< HEAD
struct lvs_dev {
	const struct lvs_ops *ops;
	void *data;
};

=======
>>>>>>> upstream/android-13
struct if_cb_manager {
	struct usb_dev *usb_d;
	struct muic_dev *muic_d;
	struct usbpd_dev *usbpd_d;
<<<<<<< HEAD
	struct lvs_dev *lvs_d;
=======
>>>>>>> upstream/android-13
};

extern struct if_cb_manager *register_usb(struct usb_dev *usb);
extern struct if_cb_manager *register_muic(struct muic_dev *muic);
extern struct if_cb_manager *register_usbpd(struct usbpd_dev *usbpd);
<<<<<<< HEAD
extern struct if_cb_manager *register_lvs(struct lvs_dev *lvs);
=======
>>>>>>> upstream/android-13
extern void usb_set_vbus_current(struct if_cb_manager *man_core, int state);
extern int usb_restart_host_mode(struct if_cb_manager *man_core, int lanes);
extern int muic_check_usb_killer(struct if_cb_manager *man_core);
extern void muic_set_bypass(struct if_cb_manager *man_core, int enable);
extern void muic_set_bc12(struct if_cb_manager *man_core, int enable);
extern int usbpd_sbu_test_read(struct if_cb_manager *man_core);
extern void usbpd_set_host_on(struct if_cb_manager *man_core, int mode);
extern void usbpd_cc_control_command(struct if_cb_manager *man_core, int is_off);
<<<<<<< HEAD
extern void usbpd_wait_entermode(struct if_cb_manager *man_core, int on);
=======
>>>>>>> upstream/android-13

#endif /* __IF_CB_MANAGER_H__ */
