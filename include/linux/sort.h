/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SORT_H
#define _LINUX_SORT_H

#include <linux/types.h>

<<<<<<< HEAD
void sort(void *base, size_t num, size_t size,
	  int (*cmp)(const void *, const void *),
	  void (*swap)(void *, void *, int));
=======
void sort_r(void *base, size_t num, size_t size,
	    cmp_r_func_t cmp_func,
	    swap_func_t swap_func,
	    const void *priv);

void sort(void *base, size_t num, size_t size,
	  cmp_func_t cmp_func,
	  swap_func_t swap_func);
>>>>>>> upstream/android-13

#endif
