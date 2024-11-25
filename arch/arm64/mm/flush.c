<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/mm/flush.c
 *
 * Copyright (C) 1995-2002 Russell King
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/export.h>
#include <linux/mm.h>
#include <linux/pagemap.h>

#include <asm/cacheflush.h>
#include <asm/cache.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
void sync_icache_aliases(void *kaddr, unsigned long len)
{
	unsigned long addr = (unsigned long)kaddr;

	if (icache_is_aliasing()) {
		__clean_dcache_area_pou(kaddr, len);
		__flush_icache_all();
=======
void sync_icache_aliases(unsigned long start, unsigned long end)
{
	if (icache_is_aliasing()) {
		dcache_clean_pou(start, end);
		icache_inval_all_pou();
>>>>>>> upstream/android-13
	} else {
		/*
		 * Don't issue kick_all_cpus_sync() after I-cache invalidation
		 * for user mappings.
		 */
<<<<<<< HEAD
		__flush_icache_range(addr, addr + len);
	}
}

static void flush_ptrace_access(struct vm_area_struct *vma, struct page *page,
				unsigned long uaddr, void *kaddr,
				unsigned long len)
{
	if (vma->vm_flags & VM_EXEC)
		sync_icache_aliases(kaddr, len);
=======
		caches_clean_inval_pou(start, end);
	}
}

static void flush_ptrace_access(struct vm_area_struct *vma, unsigned long start,
				unsigned long end)
{
	if (vma->vm_flags & VM_EXEC)
		sync_icache_aliases(start, end);
>>>>>>> upstream/android-13
}

/*
 * Copy user data from/to a page which is mapped into a different processes
 * address space.  Really, we want to allow our "user space" model to handle
 * this.
 */
void copy_to_user_page(struct vm_area_struct *vma, struct page *page,
		       unsigned long uaddr, void *dst, const void *src,
		       unsigned long len)
{
	memcpy(dst, src, len);
<<<<<<< HEAD
	flush_ptrace_access(vma, page, uaddr, dst, len);
=======
	flush_ptrace_access(vma, (unsigned long)dst, (unsigned long)dst + len);
>>>>>>> upstream/android-13
}

void __sync_icache_dcache(pte_t pte)
{
	struct page *page = pte_page(pte);

<<<<<<< HEAD
	if (!test_and_set_bit(PG_dcache_clean, &page->flags))
		sync_icache_aliases(page_address(page),
				    PAGE_SIZE << compound_order(page));
=======
	if (!test_bit(PG_dcache_clean, &page->flags)) {
		sync_icache_aliases((unsigned long)page_address(page),
				    (unsigned long)page_address(page) +
					    page_size(page));
		set_bit(PG_dcache_clean, &page->flags);
	}
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(__sync_icache_dcache);

/*
 * This function is called when a page has been modified by the kernel. Mark
 * it as dirty for later flushing when mapped in user space (if executable,
 * see __sync_icache_dcache).
 */
void flush_dcache_page(struct page *page)
{
	if (test_bit(PG_dcache_clean, &page->flags))
		clear_bit(PG_dcache_clean, &page->flags);
}
EXPORT_SYMBOL(flush_dcache_page);

/*
 * Additional functions defined in assembly.
 */
<<<<<<< HEAD
EXPORT_SYMBOL(flush_cache_all);
EXPORT_SYMBOL(__flush_icache_range);
=======
EXPORT_SYMBOL(caches_clean_inval_pou);
>>>>>>> upstream/android-13

#ifdef CONFIG_ARCH_HAS_PMEM_API
void arch_wb_cache_pmem(void *addr, size_t size)
{
	/* Ensure order against any prior non-cacheable writes */
	dmb(osh);
<<<<<<< HEAD
	__clean_dcache_area_pop(addr, size);
=======
	dcache_clean_pop((unsigned long)addr, (unsigned long)addr + size);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(arch_wb_cache_pmem);

void arch_invalidate_pmem(void *addr, size_t size)
{
<<<<<<< HEAD
	__inval_dcache_area(addr, size);
=======
	dcache_inval_poc((unsigned long)addr, (unsigned long)addr + size);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(arch_invalidate_pmem);
#endif
