<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * u_ncm.h
 *
 * Utility definitions for the ncm function
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

#ifndef U_NCM_H
#define U_NCM_H

#include <linux/usb/composite.h>

struct f_ncm_opts {
	struct usb_function_instance	func_inst;
	struct net_device		*net;
	bool				bound;

<<<<<<< HEAD
=======
	struct config_group		*ncm_interf_group;
	struct usb_os_desc		ncm_os_desc;
	char				ncm_ext_compat_id[16];
>>>>>>> upstream/android-13
	/*
	 * Read/write access to configfs attributes is handled by configfs.
	 *
	 * This is to protect the data from concurrent access by read/write
	 * and create symlink/remove symlink.
	 */
	struct mutex			lock;
	int				refcnt;
};

#endif /* U_NCM_H */
