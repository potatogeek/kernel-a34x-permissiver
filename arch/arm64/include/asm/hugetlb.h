<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm64/include/asm/hugetlb.h
 *
 * Copyright (C) 2013 Linaro Ltd.
 *
 * Based on arch/x86/include/asm/hugetlb.h
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
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_HUGETLB_H
#define __ASM_HUGETLB_H

#include <asm/page.h>

<<<<<<< HEAD
static inline pte_t huge_ptep_get(pte_t *ptep)
{
	return READ_ONCE(*ptep);
}



static inline void hugetlb_free_pgd_range(struct mmu_gather *tlb,
					  unsigned long addr, unsigned long end,
					  unsigned long floor,
					  unsigned long ceiling)
{
	free_pgd_range(tlb, addr, end, floor, ceiling);
}

static inline int is_hugepage_only_range(struct mm_struct *mm,
					 unsigned long addr, unsigned long len)
{
	return 0;
}

static inline int prepare_hugepage_range(struct file *file,
					 unsigned long addr, unsigned long len)
{
	struct hstate *h = hstate_file(file);
	if (len & ~huge_page_mask(h))
		return -EINVAL;
	if (addr & ~huge_page_mask(h))
		return -EINVAL;
	return 0;
}

static inline int huge_pte_none(pte_t pte)
{
	return pte_none(pte);
}

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	return pte_wrprotect(pte);
}
=======
#ifdef CONFIG_ARCH_ENABLE_HUGEPAGE_MIGRATION
#define arch_hugetlb_migration_supported arch_hugetlb_migration_supported
extern bool arch_hugetlb_migration_supported(struct hstate *h);
#endif
>>>>>>> upstream/android-13

static inline void arch_clear_hugepage_flags(struct page *page)
{
	clear_bit(PG_dcache_clean, &page->flags);
}
<<<<<<< HEAD

extern pte_t arch_make_huge_pte(pte_t entry, struct vm_area_struct *vma,
				struct page *page, int writable);
#define arch_make_huge_pte arch_make_huge_pte
extern void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
			    pte_t *ptep, pte_t pte);
extern int huge_ptep_set_access_flags(struct vm_area_struct *vma,
				      unsigned long addr, pte_t *ptep,
				      pte_t pte, int dirty);
extern pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
				     unsigned long addr, pte_t *ptep);
extern void huge_ptep_set_wrprotect(struct mm_struct *mm,
				    unsigned long addr, pte_t *ptep);
extern void huge_ptep_clear_flush(struct vm_area_struct *vma,
				  unsigned long addr, pte_t *ptep);
extern void huge_pte_clear(struct mm_struct *mm, unsigned long addr,
			   pte_t *ptep, unsigned long sz);
#define huge_pte_clear huge_pte_clear
=======
#define arch_clear_hugepage_flags arch_clear_hugepage_flags

pte_t arch_make_huge_pte(pte_t entry, unsigned int shift, vm_flags_t flags);
#define arch_make_huge_pte arch_make_huge_pte
#define __HAVE_ARCH_HUGE_SET_HUGE_PTE_AT
extern void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
			    pte_t *ptep, pte_t pte);
#define __HAVE_ARCH_HUGE_PTEP_SET_ACCESS_FLAGS
extern int huge_ptep_set_access_flags(struct vm_area_struct *vma,
				      unsigned long addr, pte_t *ptep,
				      pte_t pte, int dirty);
#define __HAVE_ARCH_HUGE_PTEP_GET_AND_CLEAR
extern pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
				     unsigned long addr, pte_t *ptep);
#define __HAVE_ARCH_HUGE_PTEP_SET_WRPROTECT
extern void huge_ptep_set_wrprotect(struct mm_struct *mm,
				    unsigned long addr, pte_t *ptep);
#define __HAVE_ARCH_HUGE_PTEP_CLEAR_FLUSH
extern void huge_ptep_clear_flush(struct vm_area_struct *vma,
				  unsigned long addr, pte_t *ptep);
#define __HAVE_ARCH_HUGE_PTE_CLEAR
extern void huge_pte_clear(struct mm_struct *mm, unsigned long addr,
			   pte_t *ptep, unsigned long sz);
>>>>>>> upstream/android-13
extern void set_huge_swap_pte_at(struct mm_struct *mm, unsigned long addr,
				 pte_t *ptep, pte_t pte, unsigned long sz);
#define set_huge_swap_pte_at set_huge_swap_pte_at

<<<<<<< HEAD
#include <asm-generic/hugetlb.h>

#ifdef CONFIG_ARCH_HAS_GIGANTIC_PAGE
static inline bool gigantic_page_supported(void) { return true; }
#endif

=======
void __init arm64_hugetlb_cma_reserve(void);

#include <asm-generic/hugetlb.h>

>>>>>>> upstream/android-13
#endif /* __ASM_HUGETLB_H */
