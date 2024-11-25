/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PERF_ASM_UACCESS_H_
#define _PERF_ASM_UACCESS_H_

#define __get_user(src, dest)						\
({									\
	(src) = *dest;							\
	0;								\
})

#define get_user	__get_user

<<<<<<< HEAD
#define access_ok(type, addr, size)	1
=======
#define access_ok(addr, size)	1
>>>>>>> upstream/android-13

#endif
