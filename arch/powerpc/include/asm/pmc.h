<<<<<<< HEAD
/*
 * pmc.h
 * Copyright (C) 2004  David Gibson, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * pmc.h
 * Copyright (C) 2004  David Gibson, IBM Corporation
>>>>>>> upstream/android-13
 */
#ifndef _POWERPC_PMC_H
#define _POWERPC_PMC_H
#ifdef __KERNEL__

#include <asm/ptrace.h>

typedef void (*perf_irq_t)(struct pt_regs *);
extern perf_irq_t perf_irq;

int reserve_pmc_hardware(perf_irq_t new_perf_irq);
void release_pmc_hardware(void);
void ppc_enable_pmcs(void);

#ifdef CONFIG_PPC_BOOK3S_64
#include <asm/lppaca.h>
#include <asm/firmware.h>

static inline void ppc_set_pmu_inuse(int inuse)
{
#if defined(CONFIG_PPC_PSERIES) || defined(CONFIG_KVM_BOOK3S_HV_POSSIBLE)
	if (firmware_has_feature(FW_FEATURE_LPAR)) {
#ifdef CONFIG_PPC_PSERIES
		get_lppaca()->pmcregs_in_use = inuse;
#endif
<<<<<<< HEAD
	} else {
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
		get_paca()->pmcregs_in_use = inuse;
#endif
	}
#endif
}

=======
	}
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
	get_paca()->pmcregs_in_use = inuse;
#endif
#endif
}

#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
static inline int ppc_get_pmu_inuse(void)
{
	return get_paca()->pmcregs_in_use;
}
#endif

>>>>>>> upstream/android-13
extern void power4_enable_pmcs(void);

#else /* CONFIG_PPC64 */

static inline void ppc_set_pmu_inuse(int inuse) { }

#endif

#endif /* __KERNEL__ */
#endif /* _POWERPC_PMC_H */
