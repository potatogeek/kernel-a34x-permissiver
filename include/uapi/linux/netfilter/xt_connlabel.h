/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
<<<<<<< HEAD
=======

#ifndef _UAPI_XT_CONNLABEL_H
#define _UAPI_XT_CONNLABEL_H

>>>>>>> upstream/android-13
#include <linux/types.h>

#define XT_CONNLABEL_MAXBIT 127
enum xt_connlabel_mtopts {
	XT_CONNLABEL_OP_INVERT = 1 << 0,
	XT_CONNLABEL_OP_SET    = 1 << 1,
};

struct xt_connlabel_mtinfo {
	__u16 bit;
	__u16 options;
};
<<<<<<< HEAD
=======

#endif /* _UAPI_XT_CONNLABEL_H */
>>>>>>> upstream/android-13
