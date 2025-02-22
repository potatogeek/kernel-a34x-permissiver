/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_SH_HUGETLB_H
#define _ASM_SH_HUGETLB_H

#include <asm/cacheflush.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm-generic/hugetlb.h>


static inline int is_hugepage_only_range(struct mm_struct *mm,
					 unsigned long addr,
					 unsigned long len) {
	return 0;
}
=======
>>>>>>> upstream/android-13

/*
 * If the arch doesn't supply something else, assume that hugepage
 * size aligned regions are ok without further preparation.
 */
<<<<<<< HEAD
=======
#define __HAVE_ARCH_PREPARE_HUGEPAGE_RANGE
>>>>>>> upstream/android-13
static inline int prepare_hugepage_range(struct file *file,
			unsigned long addr, unsigned long len)
{
	if (len & ~HPAGE_MASK)
		return -EINVAL;
	if (addr & ~HPAGE_MASK)
		return -EINVAL;
	return 0;
}

<<<<<<< HEAD
static inline void hugetlb_free_pgd_range(struct mmu_gather *tlb,
					  unsigned long addr, unsigned long end,
					  unsigned long floor,
					  unsigned long ceiling)
{
	free_pgd_range(tlb, addr, end, floor, ceiling);
}

static inline void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
				   pte_t *ptep, pte_t pte)
{
	set_pte_at(mm, addr, ptep, pte);
}

static inline pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
					    unsigned long addr, pte_t *ptep)
{
	return ptep_get_and_clear(mm, addr, ptep);
}

=======
#define __HAVE_ARCH_HUGE_PTEP_CLEAR_FLUSH
>>>>>>> upstream/android-13
static inline void huge_ptep_clear_flush(struct vm_area_struct *vma,
					 unsigned long addr, pte_t *ptep)
{
}

<<<<<<< HEAD
static inline int huge_pte_none(pte_t pte)
{
	return pte_none(pte);
}

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	return pte_wrprotect(pte);
}

static inline void huge_ptep_set_wrprotect(struct mm_struct *mm,
					   unsigned long addr, pte_t *ptep)
{
	ptep_set_wrprotect(mm, addr, ptep);
}

static inline int huge_ptep_set_access_flags(struct vm_area_struct *vma,
					     unsigned long addr, pte_t *ptep,
					     pte_t pte, int dirty)
{
	return ptep_set_access_flags(vma, addr, ptep, pte, dirty);
}

static inline pte_t huge_ptep_get(pte_t *ptep)
{
	return *ptep;
}

=======
>>>>>>> upstream/android-13
static inline void arch_clear_hugepage_flags(struct page *page)
{
	clear_bit(PG_dcache_clean, &page->flags);
}
<<<<<<< HEAD
=======
#define arch_clear_hugepage_flags arch_clear_hugepage_flags

#include <asm-generic/hugetlb.h>
>>>>>>> upstream/android-13

#endif /* _ASM_SH_HUGETLB_H */
