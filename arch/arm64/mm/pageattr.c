<<<<<<< HEAD
/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>

<<<<<<< HEAD
#include <asm/pgtable.h>
=======
#include <asm/cacheflush.h>
>>>>>>> upstream/android-13
#include <asm/set_memory.h>
#include <asm/tlbflush.h>

struct page_change_data {
	pgprot_t set_mask;
	pgprot_t clear_mask;
};

<<<<<<< HEAD
static int change_page_range(pte_t *ptep, pgtable_t token, unsigned long addr,
			void *data)
=======
bool rodata_full __ro_after_init = IS_ENABLED(CONFIG_RODATA_FULL_DEFAULT_ENABLED);

bool can_set_direct_map(void)
{
	return rodata_full || debug_pagealloc_enabled();
}

static int change_page_range(pte_t *ptep, unsigned long addr, void *data)
>>>>>>> upstream/android-13
{
	struct page_change_data *cdata = data;
	pte_t pte = READ_ONCE(*ptep);

	pte = clear_pte_bit(pte, cdata->clear_mask);
	pte = set_pte_bit(pte, cdata->set_mask);

	set_pte(ptep, pte);
	return 0;
}

/*
 * This function assumes that the range is mapped with PAGE_SIZE pages.
 */
static int __change_memory_common(unsigned long start, unsigned long size,
				pgprot_t set_mask, pgprot_t clear_mask)
{
	struct page_change_data data;
	int ret;

	data.set_mask = set_mask;
	data.clear_mask = clear_mask;

	ret = apply_to_page_range(&init_mm, start, size, change_page_range,
					&data);

	flush_tlb_kernel_range(start, start + size);
	return ret;
}

static int change_memory_common(unsigned long addr, int numpages,
				pgprot_t set_mask, pgprot_t clear_mask)
{
	unsigned long start = addr;
<<<<<<< HEAD
	unsigned long size = PAGE_SIZE*numpages;
	unsigned long end = start + size;
	struct vm_struct *area;
=======
	unsigned long size = PAGE_SIZE * numpages;
	unsigned long end = start + size;
	struct vm_struct *area;
	int i;
>>>>>>> upstream/android-13

	if (!PAGE_ALIGNED(addr)) {
		start &= PAGE_MASK;
		end = start + size;
		WARN_ON_ONCE(1);
	}

	/*
	 * Kernel VA mappings are always live, and splitting live section
	 * mappings into page mappings may cause TLB conflicts. This means
	 * we have to ensure that changing the permission bits of the range
	 * we are operating on does not result in such splitting.
	 *
	 * Let's restrict ourselves to mappings created by vmalloc (or vmap).
	 * Those are guaranteed to consist entirely of page mappings, and
	 * splitting is never needed.
	 *
	 * So check whether the [addr, addr + size) interval is entirely
	 * covered by precisely one VM area that has the VM_ALLOC flag set.
	 */
	area = find_vm_area((void *)addr);
	if (!area ||
<<<<<<< HEAD
	    end > (unsigned long)area->addr + area->size ||
=======
	    end > (unsigned long)kasan_reset_tag(area->addr) + area->size ||
>>>>>>> upstream/android-13
	    !(area->flags & VM_ALLOC))
		return -EINVAL;

	if (!numpages)
		return 0;

<<<<<<< HEAD
=======
	/*
	 * If we are manipulating read-only permissions, apply the same
	 * change to the linear mapping of the pages that back this VM area.
	 */
	if (rodata_full && (pgprot_val(set_mask) == PTE_RDONLY ||
			    pgprot_val(clear_mask) == PTE_RDONLY)) {
		for (i = 0; i < area->nr_pages; i++) {
			__change_memory_common((u64)page_address(area->pages[i]),
					       PAGE_SIZE, set_mask, clear_mask);
		}
	}

	/*
	 * Get rid of potentially aliasing lazily unmapped vm areas that may
	 * have permissions set that deviate from the ones we are setting here.
	 */
	vm_unmap_aliases();

>>>>>>> upstream/android-13
	return __change_memory_common(start, size, set_mask, clear_mask);
}

int set_memory_ro(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages,
					__pgprot(PTE_RDONLY),
					__pgprot(PTE_WRITE));
}

int set_memory_rw(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages,
					__pgprot(PTE_WRITE),
					__pgprot(PTE_RDONLY));
}

int set_memory_nx(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages,
					__pgprot(PTE_PXN),
<<<<<<< HEAD
					__pgprot(0));
}
EXPORT_SYMBOL_GPL(set_memory_nx);
=======
					__pgprot(PTE_MAYBE_GP));
}
>>>>>>> upstream/android-13

int set_memory_x(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages,
<<<<<<< HEAD
					__pgprot(0),
					__pgprot(PTE_PXN));
}
EXPORT_SYMBOL_GPL(set_memory_x);
=======
					__pgprot(PTE_MAYBE_GP),
					__pgprot(PTE_PXN));
}
>>>>>>> upstream/android-13

int set_memory_valid(unsigned long addr, int numpages, int enable)
{
	if (enable)
		return __change_memory_common(addr, PAGE_SIZE * numpages,
					__pgprot(PTE_VALID),
					__pgprot(0));
	else
		return __change_memory_common(addr, PAGE_SIZE * numpages,
					__pgprot(0),
					__pgprot(PTE_VALID));
}

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_PAGEALLOC
void __kernel_map_pages(struct page *page, int numpages, int enable)
{
	set_memory_valid((unsigned long)page_address(page), numpages, enable);
}
#ifdef CONFIG_HIBERNATION
/*
 * When built with CONFIG_DEBUG_PAGEALLOC and CONFIG_HIBERNATION, this function
 * is used to determine if a linear map page has been marked as not-valid by
 * CONFIG_DEBUG_PAGEALLOC. Walk the page table and check the PTE_VALID bit.
 * This is based on kern_addr_valid(), which almost does what we need.
=======
int set_direct_map_invalid_noflush(struct page *page)
{
	struct page_change_data data = {
		.set_mask = __pgprot(0),
		.clear_mask = __pgprot(PTE_VALID),
	};

	if (!can_set_direct_map())
		return 0;

	return apply_to_page_range(&init_mm,
				   (unsigned long)page_address(page),
				   PAGE_SIZE, change_page_range, &data);
}

int set_direct_map_default_noflush(struct page *page)
{
	struct page_change_data data = {
		.set_mask = __pgprot(PTE_VALID | PTE_WRITE),
		.clear_mask = __pgprot(PTE_RDONLY),
	};

	if (!can_set_direct_map())
		return 0;

	return apply_to_page_range(&init_mm,
				   (unsigned long)page_address(page),
				   PAGE_SIZE, change_page_range, &data);
}

#ifdef CONFIG_DEBUG_PAGEALLOC
void __kernel_map_pages(struct page *page, int numpages, int enable)
{
	if (!can_set_direct_map())
		return;

	set_memory_valid((unsigned long)page_address(page), numpages, enable);
}
#endif /* CONFIG_DEBUG_PAGEALLOC */

/*
 * This function is used to determine if a linear map page has been marked as
 * not-valid. Walk the page table and check the PTE_VALID bit. This is based
 * on kern_addr_valid(), which almost does what we need.
>>>>>>> upstream/android-13
 *
 * Because this is only called on the kernel linear map,  p?d_sect() implies
 * p?d_present(). When debug_pagealloc is enabled, sections mappings are
 * disabled.
 */
bool kernel_page_present(struct page *page)
{
	pgd_t *pgdp;
<<<<<<< HEAD
=======
	p4d_t *p4dp;
>>>>>>> upstream/android-13
	pud_t *pudp, pud;
	pmd_t *pmdp, pmd;
	pte_t *ptep;
	unsigned long addr = (unsigned long)page_address(page);

<<<<<<< HEAD
=======
	if (!can_set_direct_map())
		return true;

>>>>>>> upstream/android-13
	pgdp = pgd_offset_k(addr);
	if (pgd_none(READ_ONCE(*pgdp)))
		return false;

<<<<<<< HEAD
	pudp = pud_offset(pgdp, addr);
=======
	p4dp = p4d_offset(pgdp, addr);
	if (p4d_none(READ_ONCE(*p4dp)))
		return false;

	pudp = pud_offset(p4dp, addr);
>>>>>>> upstream/android-13
	pud = READ_ONCE(*pudp);
	if (pud_none(pud))
		return false;
	if (pud_sect(pud))
		return true;

	pmdp = pmd_offset(pudp, addr);
	pmd = READ_ONCE(*pmdp);
	if (pmd_none(pmd))
		return false;
	if (pmd_sect(pmd))
		return true;

	ptep = pte_offset_kernel(pmdp, addr);
	return pte_valid(READ_ONCE(*ptep));
}
<<<<<<< HEAD
#endif /* CONFIG_HIBERNATION */
#endif /* CONFIG_DEBUG_PAGEALLOC */
=======
>>>>>>> upstream/android-13
