<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * OpenRISC Linux
 *
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_OPENRISC___FLS_H
#define __ASM_OPENRISC___FLS_H


#ifdef CONFIG_OPENRISC_HAVE_INST_FL1

static inline unsigned long __fls(unsigned long x)
{
	int ret;

	__asm__ ("l.fl1 %0,%1"
		 : "=r" (ret)
		 : "r" (x));

	return ret-1;
}

#else
#include <asm-generic/bitops/__fls.h>
#endif

#endif /* __ASM_OPENRISC___FLS_H */
