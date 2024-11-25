<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/include/asm/page.h
 *
 * Copyright (C) 1995-2003 Russell King
 * Copyright (C) 2017 ARM Ltd.
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
#ifndef __ASM_PAGE_DEF_H
#define __ASM_PAGE_DEF_H

#include <linux/const.h>

/* PAGE_SHIFT determines the page size */
<<<<<<< HEAD
/* CONT_SHIFT determines the number of pages which can be tracked together  */
#define PAGE_SHIFT		CONFIG_ARM64_PAGE_SHIFT
#define CONT_SHIFT		CONFIG_ARM64_CONT_SHIFT
#define PAGE_SIZE		(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

#define CONT_SIZE		(_AC(1, UL) << (CONT_SHIFT + PAGE_SHIFT))
#define CONT_MASK		(~(CONT_SIZE-1))

=======
#define PAGE_SHIFT		CONFIG_ARM64_PAGE_SHIFT
#define PAGE_SIZE		(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

>>>>>>> upstream/android-13
#endif /* __ASM_PAGE_DEF_H */
