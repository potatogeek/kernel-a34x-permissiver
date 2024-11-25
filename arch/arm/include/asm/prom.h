<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  arch/arm/include/asm/prom.h
 *
 *  Copyright (C) 2009 Canonical Ltd. <jeremy.kerr@canonical.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASMARM_PROM_H
#define __ASMARM_PROM_H

#ifdef CONFIG_OF

<<<<<<< HEAD
extern const struct machine_desc *setup_machine_fdt(unsigned int dt_phys);
=======
extern const struct machine_desc *setup_machine_fdt(void *dt_virt);
>>>>>>> upstream/android-13
extern void __init arm_dt_init_cpu_maps(void);

#else /* CONFIG_OF */

<<<<<<< HEAD
static inline const struct machine_desc *setup_machine_fdt(unsigned int dt_phys)
=======
static inline const struct machine_desc *setup_machine_fdt(void *dt_virt)
>>>>>>> upstream/android-13
{
	return NULL;
}

static inline void arm_dt_init_cpu_maps(void) { }

#endif /* CONFIG_OF */
#endif /* ASMARM_PROM_H */
