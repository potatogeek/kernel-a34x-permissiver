<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/* 
 * Copyright (C) 2000, 2001, 2002 Jeff Dike (jdike@karaya.com)
 * Copyright 2003 PathScale, Inc.
 * Derived from include/asm-i386/pgalloc.h and include/asm-i386/pgtable.h
<<<<<<< HEAD
 * Licensed under the GPL
=======
>>>>>>> upstream/android-13
 */

#ifndef __UM_PGALLOC_H
#define __UM_PGALLOC_H

#include <linux/mm.h>

<<<<<<< HEAD
=======
#include <asm-generic/pgalloc.h>

>>>>>>> upstream/android-13
#define pmd_populate_kernel(mm, pmd, pte) \
	set_pmd(pmd, __pmd(_PAGE_TABLE + (unsigned long) __pa(pte)))

#define pmd_populate(mm, pmd, pte) 				\
	set_pmd(pmd, __pmd(_PAGE_TABLE +			\
		((unsigned long long)page_to_pfn(pte) <<	\
			(unsigned long long) PAGE_SHIFT)))
<<<<<<< HEAD
#define pmd_pgtable(pmd) pmd_page(pmd)
=======
>>>>>>> upstream/android-13

/*
 * Allocate and free page tables.
 */
extern pgd_t *pgd_alloc(struct mm_struct *);
<<<<<<< HEAD
extern void pgd_free(struct mm_struct *mm, pgd_t *pgd);

extern pte_t *pte_alloc_one_kernel(struct mm_struct *, unsigned long);
extern pgtable_t pte_alloc_one(struct mm_struct *, unsigned long);

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long) pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	__free_page(pte);
}

#define __pte_free_tlb(tlb,pte, address)		\
do {							\
	pgtable_page_dtor(pte);				\
=======

#define __pte_free_tlb(tlb,pte, address)		\
do {							\
	pgtable_pte_page_dtor(pte);			\
>>>>>>> upstream/android-13
	tlb_remove_page((tlb),(pte));			\
} while (0)

#ifdef CONFIG_3_LEVEL_PGTABLES

<<<<<<< HEAD
static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	free_page((unsigned long)pmd);
}

#define __pmd_free_tlb(tlb,x, address)   tlb_remove_page((tlb),virt_to_page(x))
#endif

#define check_pgt_cache()	do { } while (0)
=======
#define __pmd_free_tlb(tlb, pmd, address)		\
do {							\
	pgtable_pmd_page_dtor(virt_to_page(pmd));	\
	tlb_remove_page((tlb),virt_to_page(pmd));	\
} while (0)						\

#endif
>>>>>>> upstream/android-13

#endif

