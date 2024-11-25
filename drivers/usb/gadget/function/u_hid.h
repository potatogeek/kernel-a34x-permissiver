<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * u_hid.h
 *
 * Utility definitions for the hid function
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
<<<<<<< HEAD
 * Author: Andrzej Pietrasiewicz <andrzej.p@samsung.com>
=======
 * Author: Andrzej Pietrasiewicz <andrzejtp2010@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef U_HID_H
#define U_HID_H

#include <linux/usb/composite.h>

struct f_hid_opts {
	struct usb_function_instance	func_inst;
	int				minor;
	unsigned char			subclass;
	unsigned char			protocol;
<<<<<<< HEAD
=======
	unsigned char			no_out_endpoint;
>>>>>>> upstream/android-13
	unsigned short			report_length;
	unsigned short			report_desc_length;
	unsigned char			*report_desc;
	bool				report_desc_alloc;

	/*
	 * Protect the data form concurrent access by read/write
	 * and create symlink/remove symlink.
	 */
<<<<<<< HEAD
	 struct mutex			lock;
	 int				refcnt;
=======
	struct mutex			lock;
	int				refcnt;
>>>>>>> upstream/android-13
};

int ghid_setup(struct usb_gadget *g, int count);
void ghid_cleanup(void);

#endif /* U_HID_H */
