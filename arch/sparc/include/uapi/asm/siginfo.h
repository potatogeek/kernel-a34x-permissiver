/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI__SPARC_SIGINFO_H
#define _UAPI__SPARC_SIGINFO_H

#if defined(__sparc__) && defined(__arch64__)

<<<<<<< HEAD
#define __ARCH_SI_PREAMBLE_SIZE	(4 * sizeof(int))
=======
>>>>>>> upstream/android-13
#define __ARCH_SI_BAND_T int

#endif /* defined(__sparc__) && defined(__arch64__) */

<<<<<<< HEAD

#define __ARCH_SI_TRAPNO

=======
>>>>>>> upstream/android-13
#include <asm-generic/siginfo.h>


#define SI_NOINFO	32767		/* no information in siginfo_t */

<<<<<<< HEAD
/*
 * SIGEMT si_codes
 */
#define EMT_TAGOVF	1	/* tag overflow */
#define NSIGEMT		1

=======
>>>>>>> upstream/android-13
#endif /* _UAPI__SPARC_SIGINFO_H */
