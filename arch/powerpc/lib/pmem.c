<<<<<<< HEAD
/*
 * Copyright(c) 2017 IBM Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright(c) 2017 IBM Corporation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/string.h>
#include <linux/export.h>
#include <linux/uaccess.h>
<<<<<<< HEAD

#include <asm/cacheflush.h>

=======
#include <linux/libnvdimm.h>

#include <asm/cacheflush.h>

static inline void __clean_pmem_range(unsigned long start, unsigned long stop)
{
	unsigned long shift = l1_dcache_shift();
	unsigned long bytes = l1_dcache_bytes();
	void *addr = (void *)(start & ~(bytes - 1));
	unsigned long size = stop - (unsigned long)addr + (bytes - 1);
	unsigned long i;

	for (i = 0; i < size >> shift; i++, addr += bytes)
		asm volatile(PPC_DCBSTPS(%0, %1): :"i"(0), "r"(addr): "memory");
}

static inline void __flush_pmem_range(unsigned long start, unsigned long stop)
{
	unsigned long shift = l1_dcache_shift();
	unsigned long bytes = l1_dcache_bytes();
	void *addr = (void *)(start & ~(bytes - 1));
	unsigned long size = stop - (unsigned long)addr + (bytes - 1);
	unsigned long i;

	for (i = 0; i < size >> shift; i++, addr += bytes)
		asm volatile(PPC_DCBFPS(%0, %1): :"i"(0), "r"(addr): "memory");
}

static inline void clean_pmem_range(unsigned long start, unsigned long stop)
{
	if (cpu_has_feature(CPU_FTR_ARCH_207S))
		return __clean_pmem_range(start, stop);
}

static inline void flush_pmem_range(unsigned long start, unsigned long stop)
{
	if (cpu_has_feature(CPU_FTR_ARCH_207S))
		return __flush_pmem_range(start, stop);
}

>>>>>>> upstream/android-13
/*
 * CONFIG_ARCH_HAS_PMEM_API symbols
 */
void arch_wb_cache_pmem(void *addr, size_t size)
{
	unsigned long start = (unsigned long) addr;
<<<<<<< HEAD
	flush_inval_dcache_range(start, start + size);
}
EXPORT_SYMBOL(arch_wb_cache_pmem);
=======
	clean_pmem_range(start, start + size);
}
EXPORT_SYMBOL_GPL(arch_wb_cache_pmem);
>>>>>>> upstream/android-13

void arch_invalidate_pmem(void *addr, size_t size)
{
	unsigned long start = (unsigned long) addr;
<<<<<<< HEAD
	flush_inval_dcache_range(start, start + size);
}
EXPORT_SYMBOL(arch_invalidate_pmem);
=======
	flush_pmem_range(start, start + size);
}
EXPORT_SYMBOL_GPL(arch_invalidate_pmem);
>>>>>>> upstream/android-13

/*
 * CONFIG_ARCH_HAS_UACCESS_FLUSHCACHE symbols
 */
long __copy_from_user_flushcache(void *dest, const void __user *src,
		unsigned size)
{
	unsigned long copied, start = (unsigned long) dest;

	copied = __copy_from_user(dest, src, size);
<<<<<<< HEAD
	flush_inval_dcache_range(start, start + size);
=======
	clean_pmem_range(start, start + size);
>>>>>>> upstream/android-13

	return copied;
}

<<<<<<< HEAD
void *memcpy_flushcache(void *dest, const void *src, size_t size)
=======
void memcpy_flushcache(void *dest, const void *src, size_t size)
>>>>>>> upstream/android-13
{
	unsigned long start = (unsigned long) dest;

	memcpy(dest, src, size);
<<<<<<< HEAD
	flush_inval_dcache_range(start, start + size);

	return dest;
=======
	clean_pmem_range(start, start + size);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(memcpy_flushcache);

void memcpy_page_flushcache(char *to, struct page *page, size_t offset,
	size_t len)
{
	memcpy_flushcache(to, page_to_virt(page) + offset, len);
}
EXPORT_SYMBOL(memcpy_page_flushcache);
