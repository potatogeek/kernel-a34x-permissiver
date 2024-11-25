<<<<<<< HEAD
// SPDX-License-Identifier: LGPL-2.1
=======
// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
>>>>>>> upstream/android-13
#undef _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include "str_error.h"

<<<<<<< HEAD
=======
/* make sure libbpf doesn't use kernel-only integer typedefs */
#pragma GCC poison u8 u16 u32 u64 s8 s16 s32 s64

>>>>>>> upstream/android-13
/*
 * Wrapper to allow for building in non-GNU systems such as Alpine Linux's musl
 * libc, while checking strerror_r() return to avoid having to check this in
 * all places calling it.
 */
<<<<<<< HEAD
char *str_error(int err, char *dst, int len)
{
	int ret = strerror_r(err, dst, len);
=======
char *libbpf_strerror_r(int err, char *dst, int len)
{
	int ret = strerror_r(err < 0 ? -err : err, dst, len);
>>>>>>> upstream/android-13
	if (ret)
		snprintf(dst, len, "ERROR: strerror_r(%d)=%d", err, ret);
	return dst;
}
