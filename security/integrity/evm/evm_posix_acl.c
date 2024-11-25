<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2011 IBM Corporation
 *
 * Author:
 * Mimi Zohar <zohar@us.ibm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 */

#include <linux/module.h>
=======
 */

>>>>>>> upstream/android-13
#include <linux/xattr.h>
#include <linux/evm.h>

int posix_xattr_acl(const char *xattr)
{
	int xattr_len = strlen(xattr);

	if ((strlen(XATTR_NAME_POSIX_ACL_ACCESS) == xattr_len)
	     && (strncmp(XATTR_NAME_POSIX_ACL_ACCESS, xattr, xattr_len) == 0))
		return 1;
	if ((strlen(XATTR_NAME_POSIX_ACL_DEFAULT) == xattr_len)
	     && (strncmp(XATTR_NAME_POSIX_ACL_DEFAULT, xattr, xattr_len) == 0))
		return 1;
	return 0;
}
