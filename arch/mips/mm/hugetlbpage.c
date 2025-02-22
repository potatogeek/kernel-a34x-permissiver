/*
 * MIPS Huge TLB Page Support for Kernel.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2002, Rohit Seth <rohit.seth@intel.com>
 * Copyright 2005, Embedded Alley Solutions, Inc.
 * Matt Porter <mporter@embeddedalley.com>
 * Copyright (C) 2008, 2009 Cavium Networks, Inc.
 */

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
#include <linux/pagemap.h>
#include <linux/err.h>
#include <linux/sysctl.h>
#include <asm/mman.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
pte_t *huge_pte_alloc(struct mm_struct *mm, unsigned long addr,
		      unsigned long sz)
{
	pgd_t *pgd;
=======
pte_t *huge_pte_alloc(struct mm_struct *mm, struct vm_area_struct *vma,
		      unsigned long addr, unsigned long sz)
{
	pgd_t *pgd;
	p4d_t *p4d;
>>>>>>> upstream/android-13
	pud_t *pud;
	pte_t *pte = NULL;

	pgd = pgd_offset(mm, addr);
<<<<<<< HEAD
	pud = pud_alloc(mm, pgd, addr);
=======
	p4d = p4d_alloc(mm, pgd, addr);
	pud = pud_alloc(mm, p4d, addr);
>>>>>>> upstream/android-13
	if (pud)
		pte = (pte_t *)pmd_alloc(mm, pud, addr);

	return pte;
}

pte_t *huge_pte_offset(struct mm_struct *mm, unsigned long addr,
		       unsigned long sz)
{
	pgd_t *pgd;
<<<<<<< HEAD
=======
	p4d_t *p4d;
>>>>>>> upstream/android-13
	pud_t *pud;
	pmd_t *pmd = NULL;

	pgd = pgd_offset(mm, addr);
	if (pgd_present(*pgd)) {
<<<<<<< HEAD
		pud = pud_offset(pgd, addr);
		if (pud_present(*pud))
			pmd = pmd_offset(pud, addr);
=======
		p4d = p4d_offset(pgd, addr);
		if (p4d_present(*p4d)) {
			pud = pud_offset(p4d, addr);
			if (pud_present(*pud))
				pmd = pmd_offset(pud, addr);
		}
>>>>>>> upstream/android-13
	}
	return (pte_t *) pmd;
}

<<<<<<< HEAD
/*
 * This function checks for proper alignment of input addr and len parameters.
 */
int is_aligned_hugepage_range(unsigned long addr, unsigned long len)
{
	if (len & ~HPAGE_MASK)
		return -EINVAL;
	if (addr & ~HPAGE_MASK)
		return -EINVAL;
	return 0;
}

=======
>>>>>>> upstream/android-13
int pmd_huge(pmd_t pmd)
{
	return (pmd_val(pmd) & _PAGE_HUGE) != 0;
}

int pud_huge(pud_t pud)
{
	return (pud_val(pud) & _PAGE_HUGE) != 0;
}
