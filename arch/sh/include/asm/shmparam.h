<<<<<<< HEAD
/*
=======
/* SPDX-License-Identifier: GPL-2.0
 *
>>>>>>> upstream/android-13
 * include/asm-sh/shmparam.h
 *
 * Copyright (C) 1999 Niibe Yutaka
 * Copyright (C) 2006 Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASM_SH_SHMPARAM_H
#define __ASM_SH_SHMPARAM_H

/*
 * SH-4 and SH-3 7705 have an aliasing dcache. Bump this up to a sensible value
 * for everyone, and work out the specifics from the probed cache descriptor.
 */
#define	SHMLBA	0x4000		 /* attach addr a multiple of this */

#define __ARCH_FORCE_SHMLBA

#endif /* __ASM_SH_SHMPARAM_H */
