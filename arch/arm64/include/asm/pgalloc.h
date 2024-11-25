<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/include/asm/pgalloc.h
 *
 * Copyright (C) 2000-2001 Russell King
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
#ifndef __ASM_PGALLOC_H
#define __ASM_PGALLOC_H

#include <asm/pgtable-hwdef.h>
#include <asm/processor.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
#ifdef CONFIG_RKP
#include <linux/rkp.h>
#endif

#define check_pgt_cache()		do { } while (0)

#define PGALLOC_GFP	(GFP_KERNEL | __GFP_ZERO)
=======
#define __HAVE_ARCH_PGD_FREE
#include <asm-generic/pgalloc.h>

>>>>>>> upstream/android-13
#define PGD_SIZE	(PTRS_PER_PGD * sizeof(pgd_t))

#if CONFIG_PGTABLE_LEVELS > 2

<<<<<<< HEAD
static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
#ifdef CONFIG_RKP
	/* FIXME not zeroing the page */
	pmd_t *rkp_ropage = NULL;

	if (mm == &init_mm) {
		rkp_ropage = (pmd_t *)rkp_ro_alloc();
		if (rkp_ropage)
			return rkp_ropage;
		else
			return (pmd_t *)__get_free_page(PGALLOC_GFP);
	}
	else
#endif
	return (pmd_t *)__get_free_page(PGALLOC_GFP);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmdp)
{
	BUG_ON((unsigned long)pmdp & (PAGE_SIZE-1));
#ifdef CONFIG_RKP
	if(is_rkp_ro_buffer((u64)pmdp))
		rkp_ro_free((void *)pmdp);
	else
#endif
	free_page((unsigned long)pmdp);
}

=======
>>>>>>> upstream/android-13
static inline void __pud_populate(pud_t *pudp, phys_addr_t pmdp, pudval_t prot)
{
	set_pud(pudp, __pud(__phys_to_pud_val(pmdp) | prot));
}

static inline void pud_populate(struct mm_struct *mm, pud_t *pudp, pmd_t *pmdp)
{
<<<<<<< HEAD
	__pud_populate(pudp, __pa(pmdp), PMD_TYPE_TABLE);
=======
	pudval_t pudval = PUD_TYPE_TABLE;

	pudval |= (mm == &init_mm) ? PUD_TABLE_UXN : PUD_TABLE_PXN;
	__pud_populate(pudp, __pa(pmdp), pudval);
>>>>>>> upstream/android-13
}
#else
static inline void __pud_populate(pud_t *pudp, phys_addr_t pmdp, pudval_t prot)
{
	BUILD_BUG();
}
#endif	/* CONFIG_PGTABLE_LEVELS > 2 */

#if CONFIG_PGTABLE_LEVELS > 3

<<<<<<< HEAD
static inline pud_t *pud_alloc_one(struct mm_struct *mm, unsigned long addr)
{
#ifdef CONFIG_RKP
	pud_t *rkp_ropage = NULL;
	
	rkp_ropage = (pud_t *)rkp_ro_alloc();
	if (rkp_ropage)
		return rkp_ropage;
	else
#endif
	return (pud_t *)__get_free_page(PGALLOC_GFP);
}

static inline void pud_free(struct mm_struct *mm, pud_t *pudp)
{
	BUG_ON((unsigned long)pudp & (PAGE_SIZE-1));
#ifdef CONFIG_RKP
	if(is_rkp_ro_buffer((u64)pudp))
		rkp_ro_free((void *)pudp);
	else
#endif
	free_page((unsigned long)pudp);
}

static inline void __pgd_populate(pgd_t *pgdp, phys_addr_t pudp, pgdval_t prot)
{
	set_pgd(pgdp, __pgd(__phys_to_pgd_val(pudp) | prot));
}

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgdp, pud_t *pudp)
{
	__pgd_populate(pgdp, __pa(pudp), PUD_TYPE_TABLE);
}
#else
static inline void __pgd_populate(pgd_t *pgdp, phys_addr_t pudp, pgdval_t prot)
=======
static inline void __p4d_populate(p4d_t *p4dp, phys_addr_t pudp, p4dval_t prot)
{
	set_p4d(p4dp, __p4d(__phys_to_p4d_val(pudp) | prot));
}

static inline void p4d_populate(struct mm_struct *mm, p4d_t *p4dp, pud_t *pudp)
{
	p4dval_t p4dval = P4D_TYPE_TABLE;

	p4dval |= (mm == &init_mm) ? P4D_TABLE_UXN : P4D_TABLE_PXN;
	__p4d_populate(p4dp, __pa(pudp), p4dval);
}
#else
static inline void __p4d_populate(p4d_t *p4dp, phys_addr_t pudp, p4dval_t prot)
>>>>>>> upstream/android-13
{
	BUILD_BUG();
}
#endif	/* CONFIG_PGTABLE_LEVELS > 3 */

extern pgd_t *pgd_alloc(struct mm_struct *mm);
extern void pgd_free(struct mm_struct *mm, pgd_t *pgdp);

<<<<<<< HEAD
static inline pte_t *
pte_alloc_one_kernel(struct mm_struct *mm, unsigned long addr)
{
	return (pte_t *)__get_free_page(PGALLOC_GFP);
}

static inline pgtable_t
pte_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	struct page *pte;

	pte = alloc_pages(PGALLOC_GFP, 0);
	if (!pte)
		return NULL;
	if (!pgtable_page_ctor(pte)) {
		__free_page(pte);
		return NULL;
	}
	return pte;
}

/*
 * Free a PTE table.
 */
static inline void pte_free_kernel(struct mm_struct *mm, pte_t *ptep)
{
	if (ptep)
		free_page((unsigned long)ptep);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	__free_page(pte);
}

=======
>>>>>>> upstream/android-13
static inline void __pmd_populate(pmd_t *pmdp, phys_addr_t ptep,
				  pmdval_t prot)
{
	set_pmd(pmdp, __pmd(__phys_to_pmd_val(ptep) | prot));
}

/*
 * Populate the pmdp entry with a pointer to the pte.  This pmd is part
 * of the mm address space.
 */
static inline void
pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmdp, pte_t *ptep)
{
<<<<<<< HEAD
	/*
	 * The pmd must be loaded with the physical address of the PTE table
	 */
	__pmd_populate(pmdp, __pa(ptep), PMD_TYPE_TABLE);
=======
	VM_BUG_ON(mm && mm != &init_mm);
	__pmd_populate(pmdp, __pa(ptep), PMD_TYPE_TABLE | PMD_TABLE_UXN);
>>>>>>> upstream/android-13
}

static inline void
pmd_populate(struct mm_struct *mm, pmd_t *pmdp, pgtable_t ptep)
{
<<<<<<< HEAD
	__pmd_populate(pmdp, page_to_phys(ptep), PMD_TYPE_TABLE);
}
#define pmd_pgtable(pmd) pmd_page(pmd)
=======
	VM_BUG_ON(mm == &init_mm);
	__pmd_populate(pmdp, page_to_phys(ptep), PMD_TYPE_TABLE | PMD_TABLE_PXN);
}
>>>>>>> upstream/android-13

#endif
