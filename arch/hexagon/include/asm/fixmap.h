<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Fixmap support for Hexagon - enough to support highmem features
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ASM_FIXMAP_H
#define _ASM_FIXMAP_H

/*
 * A lot of the fixmap info is already in mem-layout.h
 */
#include <asm/mem-layout.h>

#include <asm-generic/fixmap.h>

<<<<<<< HEAD
#define kmap_get_fixmap_pte(vaddr) \
	pte_offset_kernel(pmd_offset(pud_offset(pgd_offset_k(vaddr), \
				(vaddr)), (vaddr)), (vaddr))

=======
>>>>>>> upstream/android-13
#endif
