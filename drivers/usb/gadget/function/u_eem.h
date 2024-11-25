<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * u_eem.h
 *
 * Utility definitions for the eem function
 *
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
<<<<<<< HEAD
 * Author: Andrzej Pietrasiewicz <andrzej.p@samsung.com>
=======
 * Author: Andrzej Pietrasiewicz <andrzejtp2010@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef U_EEM_H
#define U_EEM_H

#include <linux/usb/composite.h>

struct f_eem_opts {
	struct usb_function_instance	func_inst;
	struct net_device		*net;
	bool				bound;

	/*
	 * Read/write access to configfs attributes is handled by configfs.
	 *
	 * This is to protect the data from concurrent access by read/write
	 * and create symlink/remove symlink.
	 */
	struct mutex			lock;
	int				refcnt;
};

#endif /* U_EEM_H */
