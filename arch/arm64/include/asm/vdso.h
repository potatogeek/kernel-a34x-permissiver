<<<<<<< HEAD
/*
 * Copyright (C) 2012 ARM Limited
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Limited
>>>>>>> upstream/android-13
 */
#ifndef __ASM_VDSO_H
#define __ASM_VDSO_H

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> upstream/android-13
/*
 * Default link address for the vDSO.
 * Since we randomise the VDSO mapping, there's little point in trying
 * to prelink this.
 */
#define VDSO_LBASE	0x0

<<<<<<< HEAD
=======
#define __VVAR_PAGES    2

>>>>>>> upstream/android-13
#ifndef __ASSEMBLY__

#include <generated/vdso-offsets.h>
#ifdef CONFIG_COMPAT_VDSO
#include <generated/vdso32-offsets.h>
#endif

<<<<<<< HEAD
#define VDSO_SYMBOL(base, name)				    \
({							    \
	(void *)((vdso_offset_##name & ~1UL) - VDSO_LBASE + \
		(unsigned long)(base));			    \
=======
#define VDSO_SYMBOL(base, name)						   \
({									   \
	(void *)(vdso_offset_##name - VDSO_LBASE + (unsigned long)(base)); \
>>>>>>> upstream/android-13
})

#endif /* !__ASSEMBLY__ */

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> upstream/android-13
#endif /* __ASM_VDSO_H */
