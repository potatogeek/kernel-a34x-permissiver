<<<<<<< HEAD
/*
 * Copyright 2017, Michael Ellerman, IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation;
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2017, Michael Ellerman, IBM Corporation.
>>>>>>> upstream/android-13
 */
#ifndef _LINUX_SET_MEMORY_H_
#define _LINUX_SET_MEMORY_H_

#ifdef CONFIG_ARCH_HAS_SET_MEMORY
#include <asm/set_memory.h>
#else
static inline int set_memory_ro(unsigned long addr, int numpages) { return 0; }
static inline int set_memory_rw(unsigned long addr, int numpages) { return 0; }
static inline int set_memory_x(unsigned long addr,  int numpages) { return 0; }
static inline int set_memory_nx(unsigned long addr, int numpages) { return 0; }
#endif

<<<<<<< HEAD
=======
#ifndef CONFIG_ARCH_HAS_SET_DIRECT_MAP
static inline int set_direct_map_invalid_noflush(struct page *page)
{
	return 0;
}
static inline int set_direct_map_default_noflush(struct page *page)
{
	return 0;
}

static inline bool kernel_page_present(struct page *page)
{
	return true;
}
#else /* CONFIG_ARCH_HAS_SET_DIRECT_MAP */
/*
 * Some architectures, e.g. ARM64 can disable direct map modifications at
 * boot time. Let them overrive this query.
 */
#ifndef can_set_direct_map
static inline bool can_set_direct_map(void)
{
	return true;
}
#define can_set_direct_map can_set_direct_map
#endif
#endif /* CONFIG_ARCH_HAS_SET_DIRECT_MAP */

>>>>>>> upstream/android-13
#ifndef set_mce_nospec
static inline int set_mce_nospec(unsigned long pfn, bool unmap)
{
	return 0;
}
#endif

#ifndef clear_mce_nospec
static inline int clear_mce_nospec(unsigned long pfn)
{
	return 0;
}
#endif

#ifndef CONFIG_ARCH_HAS_MEM_ENCRYPT
static inline int set_memory_encrypted(unsigned long addr, int numpages)
{
	return 0;
}

static inline int set_memory_decrypted(unsigned long addr, int numpages)
{
	return 0;
}
#endif /* CONFIG_ARCH_HAS_MEM_ENCRYPT */

#endif /* _LINUX_SET_MEMORY_H_ */
