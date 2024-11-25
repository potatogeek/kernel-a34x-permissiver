<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/mm/copypage.c
 *
 * Copyright (C) 2002 Deep Blue Solutions Ltd, All Rights Reserved.
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
 */

=======
 */

#include <linux/bitops.h>
>>>>>>> upstream/android-13
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/cacheflush.h>
<<<<<<< HEAD

void __cpu_copy_user_page(void *kto, const void *kfrom, unsigned long vaddr)
{
	struct page *page = virt_to_page(kto);
	copy_page(kto, kfrom);
	flush_dcache_page(page);
}
EXPORT_SYMBOL_GPL(__cpu_copy_user_page);

void __cpu_clear_user_page(void *kaddr, unsigned long vaddr)
{
	clear_page(kaddr);
}
EXPORT_SYMBOL_GPL(__cpu_clear_user_page);
=======
#include <asm/cpufeature.h>
#include <asm/mte.h>

void copy_highpage(struct page *to, struct page *from)
{
	struct page *kto = page_address(to);
	struct page *kfrom = page_address(from);

	copy_page(kto, kfrom);

	if (system_supports_mte() && test_bit(PG_mte_tagged, &from->flags)) {
		set_bit(PG_mte_tagged, &to->flags);
		page_kasan_tag_reset(to);
		/*
		 * We need smp_wmb() in between setting the flags and clearing the
		 * tags because if another thread reads page->flags and builds a
		 * tagged address out of it, there is an actual dependency to the
		 * memory access, but on the current thread we do not guarantee that
		 * the new page->flags are visible before the tags were updated.
		 */
		smp_wmb();
		mte_copy_page_tags(kto, kfrom);
	}
}
EXPORT_SYMBOL(copy_highpage);

void copy_user_highpage(struct page *to, struct page *from,
			unsigned long vaddr, struct vm_area_struct *vma)
{
	copy_highpage(to, from);
	flush_dcache_page(to);
}
EXPORT_SYMBOL_GPL(copy_user_highpage);
>>>>>>> upstream/android-13
