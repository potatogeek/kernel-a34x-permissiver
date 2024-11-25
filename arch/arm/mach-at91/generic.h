<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/arch/arm/mach-at91/generic.h
 *
 *  Copyright (C) 2005 David Brownell
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _AT91_GENERIC_H
#define _AT91_GENERIC_H

#ifdef CONFIG_PM
extern void __init at91rm9200_pm_init(void);
extern void __init at91sam9_pm_init(void);
<<<<<<< HEAD
extern void __init sama5_pm_init(void);
extern void __init sama5d2_pm_init(void);
#else
static inline void __init at91rm9200_pm_init(void) { }
static inline void __init at91sam9_pm_init(void) { }
static inline void __init sama5_pm_init(void) { }
static inline void __init sama5d2_pm_init(void) { }
=======
extern void __init sam9x60_pm_init(void);
extern void __init sama5_pm_init(void);
extern void __init sama5d2_pm_init(void);
extern void __init sama7_pm_init(void);
#else
static inline void __init at91rm9200_pm_init(void) { }
static inline void __init at91sam9_pm_init(void) { }
static inline void __init sam9x60_pm_init(void) { }
static inline void __init sama5_pm_init(void) { }
static inline void __init sama5d2_pm_init(void) { }
static inline void __init sama7_pm_init(void) { }
>>>>>>> upstream/android-13
#endif

#endif /* _AT91_GENERIC_H */
