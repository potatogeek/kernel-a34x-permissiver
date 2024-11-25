/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _MOTOROLA_PGALLOC_H
#define _MOTOROLA_PGALLOC_H

#include <asm/tlb.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
extern pmd_t *get_pointer_table(void);
extern int free_pointer_table(pmd_t *);

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte;

	pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_ZERO);
	if (pte) {
		__flush_page_to_ram(pte);
		flush_tlb_kernel_page(pte);
		nocache_page(pte);
	}

	return pte;
=======
extern void mmu_page_ctor(void *page);
extern void mmu_page_dtor(void *page);

enum m68k_table_types {
	TABLE_PGD = 0,
	TABLE_PMD = 0, /* same size as PGD */
	TABLE_PTE = 1,
};

extern void init_pointer_table(void *table, int type);
extern void *get_pointer_table(int type);
extern int free_pointer_table(void *table, int type);

/*
 * Allocate and free page tables. The xxx_kernel() versions are
 * used to allocate a kernel page table - this turns on ASN bits
 * if any.
 */

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm)
{
	return get_pointer_table(TABLE_PTE);
>>>>>>> upstream/android-13
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
<<<<<<< HEAD
	cache_page(pte);
	free_page((unsigned long) pte);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	struct page *page;
	pte_t *pte;

	page = alloc_pages(GFP_KERNEL|__GFP_ZERO, 0);
	if(!page)
		return NULL;
	if (!pgtable_page_ctor(page)) {
		__free_page(page);
		return NULL;
	}

	pte = kmap(page);
	__flush_page_to_ram(pte);
	flush_tlb_kernel_page(pte);
	nocache_page(pte);
	kunmap(page);
	return page;
}

static inline void pte_free(struct mm_struct *mm, pgtable_t page)
{
	pgtable_page_dtor(page);
	cache_page(kmap(page));
	kunmap(page);
	__free_page(page);
}

static inline void __pte_free_tlb(struct mmu_gather *tlb, pgtable_t page,
				  unsigned long address)
{
	pgtable_page_dtor(page);
	cache_page(kmap(page));
	kunmap(page);
	__free_page(page);
=======
	free_pointer_table(pte, TABLE_PTE);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm)
{
	return get_pointer_table(TABLE_PTE);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pgtable)
{
	free_pointer_table(pgtable, TABLE_PTE);
}

static inline void __pte_free_tlb(struct mmu_gather *tlb, pgtable_t pgtable,
				  unsigned long address)
{
	free_pointer_table(pgtable, TABLE_PTE);
>>>>>>> upstream/android-13
}


static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
<<<<<<< HEAD
	return get_pointer_table();
=======
	return get_pointer_table(TABLE_PMD);
>>>>>>> upstream/android-13
}

static inline int pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
<<<<<<< HEAD
	return free_pointer_table(pmd);
=======
	return free_pointer_table(pmd, TABLE_PMD);
>>>>>>> upstream/android-13
}

static inline int __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmd,
				 unsigned long address)
{
<<<<<<< HEAD
	return free_pointer_table(pmd);
=======
	return free_pointer_table(pmd, TABLE_PMD);
>>>>>>> upstream/android-13
}


static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
<<<<<<< HEAD
	pmd_free(mm, (pmd_t *)pgd);
=======
	free_pointer_table(pgd, TABLE_PGD);
>>>>>>> upstream/android-13
}

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
<<<<<<< HEAD
	return (pgd_t *)get_pointer_table();
=======
	return get_pointer_table(TABLE_PGD);
>>>>>>> upstream/android-13
}


static inline void pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
	pmd_set(pmd, pte);
}

static inline void pmd_populate(struct mm_struct *mm, pmd_t *pmd, pgtable_t page)
{
<<<<<<< HEAD
	pmd_set(pmd, page_address(page));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pmd_t *pmd)
{
	pgd_set(pgd, pmd);
=======
	pmd_set(pmd, page);
}

static inline void pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
	pud_set(pud, pmd);
>>>>>>> upstream/android-13
}

#endif /* _MOTOROLA_PGALLOC_H */
