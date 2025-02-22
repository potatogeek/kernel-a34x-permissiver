/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_BUILTIN_FFS_H_
#define _ASM_GENERIC_BITOPS_BUILTIN_FFS_H_

/**
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
<<<<<<< HEAD
 * differs in spirit from the above ffz (man ffs).
 */
static __always_inline int ffs(int x)
{
	return __builtin_ffs(x);
}
=======
 * differs in spirit from ffz (man ffs).
 */
#define ffs(x) __builtin_ffs(x)
>>>>>>> upstream/android-13

#endif
