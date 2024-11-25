/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NS_COMMON_H
#define _LINUX_NS_COMMON_H

<<<<<<< HEAD
=======
#include <linux/refcount.h>

>>>>>>> upstream/android-13
struct proc_ns_operations;

struct ns_common {
	atomic_long_t stashed;
	const struct proc_ns_operations *ops;
	unsigned int inum;
<<<<<<< HEAD
=======
	refcount_t count;
>>>>>>> upstream/android-13
};

#endif
