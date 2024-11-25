<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Linux/PA-RISC Project (http://www.parisc-linux.org/)
 *
 * Floating-point emulation code
 *  Copyright (C) 2001 Hewlett-Packard (Paul Bame) <bame@debian.org>
<<<<<<< HEAD
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * BEGIN_DESC
 * 
 *  File: 
 *      @(#)	pa/fp/fpu.h		$Revision: 1.1 $
 * 
 *  Purpose:
 *      <<please update with a synopis of the functionality provided by this file>>
 * 
 * 
 * END_DESC  
*/

#ifdef __NO_PA_HDRS
    PA header file -- do not include this header file for non-PA builds.
#endif

=======
 */
>>>>>>> upstream/android-13

#ifndef _MACHINE_FPU_INCLUDED /* allows multiple inclusion */
#define _MACHINE_FPU_INCLUDED

<<<<<<< HEAD
#if 0
#ifndef _SYS_STDSYMS_INCLUDED
#    include <sys/stdsyms.h>
#endif   /* _SYS_STDSYMS_INCLUDED  */
#include  <machine/pdc/pdc_rqsts.h>
#endif

=======
>>>>>>> upstream/android-13
#define PA83_FPU_FLAG    0x00000001
#define PA89_FPU_FLAG    0x00000002
#define PA2_0_FPU_FLAG   0x00000010

#define TIMEX_EXTEN_FLAG 0x00000004

#define ROLEX_EXTEN_FLAG 0x00000008
#define COPR_FP 	0x00000080	/* Floating point -- Coprocessor 0 */
#define SFU_MPY_DIVIDE	0x00008000	/* Multiply/Divide __ SFU 0 */

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
#define EM_FPU_TYPE_OFFSET 272

/* version of EMULATION software for COPR,0,0 instruction */
#define EMULATION_VERSION 4

/*
<<<<<<< HEAD
 * The only was to differeniate between TIMEX and ROLEX (or PCX-S and PCX-T)
 * is thorough the potential type field from the PDC_MODEL call.  The 
 * following flags are used at assist this differeniation.
=======
 * The only way to differentiate between TIMEX and ROLEX (or PCX-S and PCX-T)
 * is through the potential type field from the PDC_MODEL call.
 * The following flags are used to assist this differentiation.
>>>>>>> upstream/android-13
 */

#define ROLEX_POTENTIAL_KEY_FLAGS	PDC_MODEL_CPU_KEY_WORD_TO_IO
#define TIMEX_POTENTIAL_KEY_FLAGS	(PDC_MODEL_CPU_KEY_QUAD_STORE | \
					 PDC_MODEL_CPU_KEY_RECIP_SQRT)

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
#endif /* ! _MACHINE_FPU_INCLUDED */
