<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * PGD allocation/freeing
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author: Catalin Marinas <catalin.marinas@arm.com>
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

#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/slab.h>

#include <asm/pgalloc.h>
#include <asm/page.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
#ifdef CONFIG_RKP
#include <linux/rkp.h>
#endif

=======
>>>>>>> upstream/android-13
static struct kmem_cache *pgd_cache __ro_after_init;

pgd_t *pgd_alloc(struct mm_struct *mm)
{
<<<<<<< HEAD
#ifdef CONFIG_RKP
	pgd_t *ret = NULL;

	ret = (pgd_t *) rkp_ro_alloc();

	if (!ret) {
		if (PGD_SIZE == PAGE_SIZE)
			ret = (pgd_t *)__get_free_page(PGALLOC_GFP);
		else
			ret = kmem_cache_alloc(pgd_cache, PGALLOC_GFP);
	}

	if(unlikely(!ret)) {
		pr_warn("%s: pgd alloc is failed\n", __func__);
		return ret;
	}

	if (rkp_started)
		uh_call(UH_APP_RKP, RKP_PGD_RO, (u64)ret, 0, 0, 0);

	return ret;
#else
	if (PGD_SIZE == PAGE_SIZE)
		return (pgd_t *)__get_free_page(PGALLOC_GFP);
	else
		return kmem_cache_alloc(pgd_cache, PGALLOC_GFP);
#endif
=======
	gfp_t gfp = GFP_PGTABLE_USER;

	if (PGD_SIZE == PAGE_SIZE)
		return (pgd_t *)__get_free_page(gfp);
	else
		return kmem_cache_alloc(pgd_cache, gfp);
>>>>>>> upstream/android-13
}

void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
<<<<<<< HEAD
#ifdef CONFIG_RKP
	if (rkp_started)
		uh_call(UH_APP_RKP, RKP_PGD_RWX, (u64)pgd, 0, 0, 0);

	/* if pgd memory come from read only buffer, the put it back */
	if (is_rkp_ro_buffer((u64)pgd)) {
		rkp_ro_free((void *)pgd);
	} else {
		if (PGD_SIZE == PAGE_SIZE)
			free_page((unsigned long)pgd);
		else
			kmem_cache_free(pgd_cache, pgd);
	}
#else
=======
>>>>>>> upstream/android-13
	if (PGD_SIZE == PAGE_SIZE)
		free_page((unsigned long)pgd);
	else
		kmem_cache_free(pgd_cache, pgd);
<<<<<<< HEAD
#endif
}

void __init pgd_cache_init(void)
=======
}

void __init pgtable_cache_init(void)
>>>>>>> upstream/android-13
{
	if (PGD_SIZE == PAGE_SIZE)
		return;

#ifdef CONFIG_ARM64_PA_BITS_52
	/*
	 * With 52-bit physical addresses, the architecture requires the
	 * top-level table to be aligned to at least 64 bytes.
	 */
	BUILD_BUG_ON(PGD_SIZE < 64);
#endif

	/*
	 * Naturally aligned pgds required by the architecture.
	 */
	pgd_cache = kmem_cache_create("pgd_cache", PGD_SIZE, PGD_SIZE,
				      SLAB_PANIC, NULL);
}
