<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  HID driver for Xin-Mo devices, currently only the Dual Arcade controller.
 *  Fixes the negative axis event values (the devices sends -2) to match the
 *  logical axis minimum of the HID report descriptor (the report announces
 *  -1). It is needed because hid-input discards out of bounds values.
 *  (This module is based on "hid-saitek" and "hid-lg".)
 *
 *  Copyright (c) 2013 Olivier Scherler
 */

/*
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "hid-ids.h"

/*
 * Fix negative events that are out of bounds.
 */
static int xinmo_event(struct hid_device *hdev, struct hid_field *field,
		struct hid_usage *usage, __s32 value)
{
	switch (usage->code) {
	case ABS_X:
	case ABS_Y:
	case ABS_Z:
	case ABS_RX:
		if (value < -1) {
			input_event(field->hidinput->input, usage->type,
				usage->code, -1);
			return 1;
		}
		break;
	}

	return 0;
}

static const struct hid_device_id xinmo_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIN_MO, USB_DEVICE_ID_XIN_MO_DUAL_ARCADE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIN_MO, USB_DEVICE_ID_THT_2P_ARCADE) },
	{ }
};

MODULE_DEVICE_TABLE(hid, xinmo_devices);

static struct hid_driver xinmo_driver = {
	.name = "xinmo",
	.id_table = xinmo_devices,
	.event = xinmo_event
};

module_hid_driver(xinmo_driver);
MODULE_LICENSE("GPL");
