/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ALPHA_PGALLOC_H
#define _ALPHA_PGALLOC_H

#include <linux/mm.h>
#include <linux/mmzone.h>

<<<<<<< HEAD
=======
#include <asm-generic/pgalloc.h>

>>>>>>> upstream/android-13
/*      
 * Allocate and free page tables. The xxx_kernel() versions are
 * used to allocate a kernel page table - this turns on ASN bits
 * if any.
 */

static inline void
pmd_populate(struct mm_struct *mm, pmd_t *pmd, pgtable_t pte)
{
	pmd_set(pmd, (pte_t *)(page_to_pa(pte) + PAGE_OFFSET));
}
<<<<<<< HEAD
#define pmd_pgtable(pmd) pmd_page(pmd)
=======
>>>>>>> upstream/android-13

static inline void
pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
	pmd_set(pmd, pte);
}

static inline void
<<<<<<< HEAD
pgd_populate(struct mm_struct *mm, pgd_t *pgd, pmd_t *pmd)
{
	pgd_set(pgd, pmd);
=======
pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
	pud_set(pud, pmd);
>>>>>>> upstream/android-13
}

extern pgd_t *pgd_alloc(struct mm_struct *mm);

<<<<<<< HEAD
static inline void
pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

static inline pmd_t *
pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
	pmd_t *ret = (pmd_t *)__get_free_page(GFP_KERNEL|__GFP_ZERO);
	return ret;
}

static inline void
pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	free_page((unsigned long)pmd);
}

static inline pte_t *
pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_ZERO);
	return pte;
}

static inline void
pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline pgtable_t
pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte = pte_alloc_one_kernel(mm, address);
	struct page *page;

	if (!pte)
		return NULL;
	page = virt_to_page(pte);
	if (!pgtable_page_ctor(page)) {
		__free_page(page);
		return NULL;
	}
	return page;
}

static inline void
pte_free(struct mm_struct *mm, pgtable_t page)
{
	pgtable_page_dtor(page);
	__free_page(page);
}

#define check_pgt_cache()	do { } while (0)

=======
>>>>>>> upstream/android-13
#endif /* _ALPHA_PGALLOC_H */
