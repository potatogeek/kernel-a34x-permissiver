<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  HID driver for ELECOM devices:
 *  - BM084 Bluetooth Mouse
 *  - EX-G Trackballs (M-XT3DRBK, M-XT3URBK, M-XT4DRBK)
 *  - DEFT Trackballs (M-DT1DRBK, M-DT1URBK, M-DT2DRBK, M-DT2URBK)
 *  - HUGE Trackballs (M-HT1DRBK, M-HT1URBK)
 *
 *  Copyright (c) 2010 Richard Nauber <Richard.Nauber@gmail.com>
 *  Copyright (c) 2016 Yuxuan Shui <yshuiv7@gmail.com>
 *  Copyright (c) 2017 Diego Elio Pettenò <flameeyes@flameeyes.eu>
 *  Copyright (c) 2017 Alex Manoussakis <amanou@gnu.org>
 *  Copyright (c) 2017 Tomasz Kramkowski <tk@the-tk.com>
<<<<<<< HEAD
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
=======
 *  Copyright (c) 2020 YOSHIOKA Takuma <lo48576@hard-wi.red>
 */

/*
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>

#include "hid-ids.h"

/*
 * Certain ELECOM mice misreport their button count meaning that they only work
 * correctly with the ELECOM mouse assistant software which is unavailable for
 * Linux. A four extra INPUT reports and a FEATURE report are described by the
 * report descriptor but it does not appear that these enable software to
 * control what the extra buttons map to. The only simple and straightforward
 * solution seems to involve fixing up the report descriptor.
<<<<<<< HEAD
 *
 * Report descriptor format:
 * Positions 13, 15, 21 and 31 store the button bit count, button usage minimum,
 * button usage maximum and padding bit count respectively.
=======
>>>>>>> upstream/android-13
 */
#define MOUSE_BUTTONS_MAX 8
static void mouse_button_fixup(struct hid_device *hdev,
			       __u8 *rdesc, unsigned int rsize,
<<<<<<< HEAD
			       int nbuttons)
{
	if (rsize < 32 || rdesc[12] != 0x95 ||
	    rdesc[14] != 0x75 || rdesc[15] != 0x01 ||
	    rdesc[20] != 0x29 || rdesc[30] != 0x75)
		return;
	hid_info(hdev, "Fixing up Elecom mouse button count\n");
	nbuttons = clamp(nbuttons, 0, MOUSE_BUTTONS_MAX);
	rdesc[13] = nbuttons;
	rdesc[21] = nbuttons;
	rdesc[31] = MOUSE_BUTTONS_MAX - nbuttons;
=======
			       unsigned int button_bit_count,
			       unsigned int padding_bit,
			       unsigned int button_report_size,
			       unsigned int button_usage_maximum,
			       int nbuttons)
{
	if (rsize < 32 || rdesc[button_bit_count] != 0x95 ||
	    rdesc[button_report_size] != 0x75 ||
	    rdesc[button_report_size + 1] != 0x01 ||
	    rdesc[button_usage_maximum] != 0x29 || rdesc[padding_bit] != 0x75)
		return;
	hid_info(hdev, "Fixing up Elecom mouse button count\n");
	nbuttons = clamp(nbuttons, 0, MOUSE_BUTTONS_MAX);
	rdesc[button_bit_count + 1] = nbuttons;
	rdesc[button_usage_maximum + 1] = nbuttons;
	rdesc[padding_bit + 1] = MOUSE_BUTTONS_MAX - nbuttons;
>>>>>>> upstream/android-13
}

static __u8 *elecom_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
	switch (hdev->product) {
	case USB_DEVICE_ID_ELECOM_BM084:
		/* The BM084 Bluetooth mouse includes a non-existing horizontal
		 * wheel in the HID descriptor. */
		if (*rsize >= 48 && rdesc[46] == 0x05 && rdesc[47] == 0x0c) {
			hid_info(hdev, "Fixing up Elecom BM084 report descriptor\n");
			rdesc[47] = 0x00;
		}
		break;
<<<<<<< HEAD
	case USB_DEVICE_ID_ELECOM_M_XT3URBK:
	case USB_DEVICE_ID_ELECOM_M_XT3DRBK:
	case USB_DEVICE_ID_ELECOM_M_XT4DRBK:
		mouse_button_fixup(hdev, rdesc, *rsize, 6);
=======
	case USB_DEVICE_ID_ELECOM_M_XGL20DLBK:
		/*
		 * Report descriptor format:
		 * 20: button bit count
		 * 28: padding bit count
		 * 22: button report size
		 * 14: button usage maximum
		 */
		mouse_button_fixup(hdev, rdesc, *rsize, 20, 28, 22, 14, 8);
		break;
	case USB_DEVICE_ID_ELECOM_M_XT3URBK:
	case USB_DEVICE_ID_ELECOM_M_XT3DRBK:
	case USB_DEVICE_ID_ELECOM_M_XT4DRBK:
		/*
		 * Report descriptor format:
		 * 12: button bit count
		 * 30: padding bit count
		 * 14: button report size
		 * 20: button usage maximum
		 */
		mouse_button_fixup(hdev, rdesc, *rsize, 12, 30, 14, 20, 6);
>>>>>>> upstream/android-13
		break;
	case USB_DEVICE_ID_ELECOM_M_DT1URBK:
	case USB_DEVICE_ID_ELECOM_M_DT1DRBK:
	case USB_DEVICE_ID_ELECOM_M_HT1URBK:
	case USB_DEVICE_ID_ELECOM_M_HT1DRBK:
<<<<<<< HEAD
		mouse_button_fixup(hdev, rdesc, *rsize, 8);
=======
		/*
		 * Report descriptor format:
		 * 12: button bit count
		 * 30: padding bit count
		 * 14: button report size
		 * 20: button usage maximum
		 */
		mouse_button_fixup(hdev, rdesc, *rsize, 12, 30, 14, 20, 8);
>>>>>>> upstream/android-13
		break;
	}
	return rdesc;
}

static const struct hid_device_id elecom_devices[] = {
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_BM084) },
<<<<<<< HEAD
=======
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_XGL20DLBK) },
>>>>>>> upstream/android-13
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_XT3URBK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_XT3DRBK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_XT4DRBK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_DT1URBK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_DT1DRBK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_HT1URBK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_M_HT1DRBK) },
	{ }
};
MODULE_DEVICE_TABLE(hid, elecom_devices);

static struct hid_driver elecom_driver = {
	.name = "elecom",
	.id_table = elecom_devices,
	.report_fixup = elecom_report_fixup
};
module_hid_driver(elecom_driver);

MODULE_LICENSE("GPL");
