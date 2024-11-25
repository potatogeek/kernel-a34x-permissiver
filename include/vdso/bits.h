/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __VDSO_BITS_H
#define __VDSO_BITS_H

<<<<<<< HEAD
#define BIT(nr)			(1UL << (nr))
=======
#include <vdso/const.h>

#define BIT(nr)			(UL(1) << (nr))
>>>>>>> upstream/android-13

#endif	/* __VDSO_BITS_H */
