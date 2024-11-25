/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASMS390_SET_MEMORY_H
#define _ASMS390_SET_MEMORY_H

<<<<<<< HEAD
=======
#include <linux/mutex.h>

extern struct mutex cpa_mutex;

>>>>>>> upstream/android-13
#define SET_MEMORY_RO	1UL
#define SET_MEMORY_RW	2UL
#define SET_MEMORY_NX	4UL
#define SET_MEMORY_X	8UL
<<<<<<< HEAD
=======
#define SET_MEMORY_4K  16UL
>>>>>>> upstream/android-13

int __set_memory(unsigned long addr, int numpages, unsigned long flags);

static inline int set_memory_ro(unsigned long addr, int numpages)
{
	return __set_memory(addr, numpages, SET_MEMORY_RO);
}

static inline int set_memory_rw(unsigned long addr, int numpages)
{
	return __set_memory(addr, numpages, SET_MEMORY_RW);
}

static inline int set_memory_nx(unsigned long addr, int numpages)
{
	return __set_memory(addr, numpages, SET_MEMORY_NX);
}

static inline int set_memory_x(unsigned long addr, int numpages)
{
	return __set_memory(addr, numpages, SET_MEMORY_X);
}

<<<<<<< HEAD
=======
static inline int set_memory_4k(unsigned long addr, int numpages)
{
	return __set_memory(addr, numpages, SET_MEMORY_4K);
}

>>>>>>> upstream/android-13
#endif
