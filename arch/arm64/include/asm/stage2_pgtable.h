<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2016 - ARM Ltd
 *
 * stage2 page table helpers
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

#ifndef __ARM64_S2_PGTABLE_H_
#define __ARM64_S2_PGTABLE_H_

<<<<<<< HEAD
#include <asm/pgtable.h>

/*
 * The hardware supports concatenation of up to 16 tables at stage2 entry level
 * and we use the feature whenever possible.
 *
 * Now, the minimum number of bits resolved at any level is (PAGE_SHIFT - 3).
 * On arm64, the smallest PAGE_SIZE supported is 4k, which means
 *             (PAGE_SHIFT - 3) > 4 holds for all page sizes.
 * This implies, the total number of page table levels at stage2 expected
 * by the hardware is actually the number of levels required for (KVM_PHYS_SHIFT - 4)
 * in normal translations(e.g, stage1), since we cannot have another level in
 * the range (KVM_PHYS_SHIFT, KVM_PHYS_SHIFT - 4).
 */
#define STAGE2_PGTABLE_LEVELS		ARM64_HW_PGTABLE_LEVELS(KVM_PHYS_SHIFT - 4)

/*
 * With all the supported VA_BITs and 40bit guest IPA, the following condition
 * is always true:
 *
 *       STAGE2_PGTABLE_LEVELS <= CONFIG_PGTABLE_LEVELS
 *
 * We base our stage-2 page table walker helpers on this assumption and
 * fall back to using the host version of the helper wherever possible.
 * i.e, if a particular level is not folded (e.g, PUD) at stage2, we fall back
 * to using the host version, since it is guaranteed it is not folded at host.
 *
 * If the condition breaks in the future, we can rearrange the host level
 * definitions and reuse them for stage2. Till then...
 */
#if STAGE2_PGTABLE_LEVELS > CONFIG_PGTABLE_LEVELS
#error "Unsupported combination of guest IPA and host VA_BITS."
#endif

/* S2_PGDIR_SHIFT is the size mapped by top-level stage2 entry */
#define S2_PGDIR_SHIFT			ARM64_HW_PGTABLE_LEVEL_SHIFT(4 - STAGE2_PGTABLE_LEVELS)
#define S2_PGDIR_SIZE			(_AC(1, UL) << S2_PGDIR_SHIFT)
#define S2_PGDIR_MASK			(~(S2_PGDIR_SIZE - 1))

/*
 * The number of PTRS across all concatenated stage2 tables given by the
 * number of bits resolved at the initial level.
 */
#define PTRS_PER_S2_PGD			(1 << (KVM_PHYS_SHIFT - S2_PGDIR_SHIFT))

/*
 * KVM_MMU_CACHE_MIN_PAGES is the number of stage2 page table translation
 * levels in addition to the PGD.
 */
#define KVM_MMU_CACHE_MIN_PAGES		(STAGE2_PGTABLE_LEVELS - 1)


#if STAGE2_PGTABLE_LEVELS > 3

#define S2_PUD_SHIFT			ARM64_HW_PGTABLE_LEVEL_SHIFT(1)
#define S2_PUD_SIZE			(_AC(1, UL) << S2_PUD_SHIFT)
#define S2_PUD_MASK			(~(S2_PUD_SIZE - 1))

#define stage2_pgd_none(pgd)				pgd_none(pgd)
#define stage2_pgd_clear(pgd)				pgd_clear(pgd)
#define stage2_pgd_present(pgd)				pgd_present(pgd)
#define stage2_pgd_populate(pgd, pud)			pgd_populate(NULL, pgd, pud)
#define stage2_pud_offset(pgd, address)			pud_offset(pgd, address)
#define stage2_pud_free(pud)				pud_free(NULL, pud)

#define stage2_pud_table_empty(pudp)			kvm_page_empty(pudp)

static inline phys_addr_t stage2_pud_addr_end(phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + S2_PUD_SIZE) & S2_PUD_MASK;

	return (boundary - 1 < end - 1) ? boundary : end;
}

#endif		/* STAGE2_PGTABLE_LEVELS > 3 */


#if STAGE2_PGTABLE_LEVELS > 2

#define S2_PMD_SHIFT			ARM64_HW_PGTABLE_LEVEL_SHIFT(2)
#define S2_PMD_SIZE			(_AC(1, UL) << S2_PMD_SHIFT)
#define S2_PMD_MASK			(~(S2_PMD_SIZE - 1))

#define stage2_pud_none(pud)				pud_none(pud)
#define stage2_pud_clear(pud)				pud_clear(pud)
#define stage2_pud_present(pud)				pud_present(pud)
#define stage2_pud_populate(pud, pmd)			pud_populate(NULL, pud, pmd)
#define stage2_pmd_offset(pud, address)			pmd_offset(pud, address)
#define stage2_pmd_free(pmd)				pmd_free(NULL, pmd)

#define stage2_pud_huge(pud)				pud_huge(pud)
#define stage2_pmd_table_empty(pmdp)			kvm_page_empty(pmdp)

static inline phys_addr_t stage2_pmd_addr_end(phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + S2_PMD_SIZE) & S2_PMD_MASK;

	return (boundary - 1 < end - 1) ? boundary : end;
}

#endif		/* STAGE2_PGTABLE_LEVELS > 2 */

#define stage2_pte_table_empty(ptep)			kvm_page_empty(ptep)

#if STAGE2_PGTABLE_LEVELS == 2
#include <asm/stage2_pgtable-nopmd.h>
#elif STAGE2_PGTABLE_LEVELS == 3
#include <asm/stage2_pgtable-nopud.h>
#endif


#define stage2_pgd_index(addr)				(((addr) >> S2_PGDIR_SHIFT) & (PTRS_PER_S2_PGD - 1))

static inline phys_addr_t stage2_pgd_addr_end(phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + S2_PGDIR_SIZE) & S2_PGDIR_MASK;
=======
#include <linux/pgtable.h>

/*
 * PGDIR_SHIFT determines the size a top-level page table entry can map
 * and depends on the number of levels in the page table. Compute the
 * PGDIR_SHIFT for a given number of levels.
 */
#define pt_levels_pgdir_shift(lvls)	ARM64_HW_PGTABLE_LEVEL_SHIFT(4 - (lvls))

/*
 * The hardware supports concatenation of up to 16 tables at stage2 entry
 * level and we use the feature whenever possible, which means we resolve 4
 * additional bits of address at the entry level.
 *
 * This implies, the total number of page table levels required for
 * IPA_SHIFT at stage2 expected by the hardware can be calculated using
 * the same logic used for the (non-collapsable) stage1 page tables but for
 * (IPA_SHIFT - 4).
 */
#define stage2_pgtable_levels(ipa)	ARM64_HW_PGTABLE_LEVELS((ipa) - 4)
#define kvm_stage2_levels(kvm)		VTCR_EL2_LVLS(kvm->arch.vtcr)

/* stage2_pgdir_shift() is the size mapped by top-level stage2 entry for the VM */
#define stage2_pgdir_shift(kvm)		pt_levels_pgdir_shift(kvm_stage2_levels(kvm))
#define stage2_pgdir_size(kvm)		(1ULL << stage2_pgdir_shift(kvm))
#define stage2_pgdir_mask(kvm)		~(stage2_pgdir_size(kvm) - 1)

/*
 * kvm_mmmu_cache_min_pages() is the number of pages required to install
 * a stage-2 translation. We pre-allocate the entry level page table at
 * the VM creation.
 */
#define kvm_mmu_cache_min_pages(kvm)	(kvm_stage2_levels(kvm) - 1)

static inline phys_addr_t
stage2_pgd_addr_end(struct kvm *kvm, phys_addr_t addr, phys_addr_t end)
{
	phys_addr_t boundary = (addr + stage2_pgdir_size(kvm)) & stage2_pgdir_mask(kvm);
>>>>>>> upstream/android-13

	return (boundary - 1 < end - 1) ? boundary : end;
}

#endif	/* __ARM64_S2_PGTABLE_H_ */
