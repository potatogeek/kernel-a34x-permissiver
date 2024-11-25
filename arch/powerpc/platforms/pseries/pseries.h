<<<<<<< HEAD
/*
 * Copyright 2006 IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2006 IBM Corporation.
>>>>>>> upstream/android-13
 */

#ifndef _PSERIES_PSERIES_H
#define _PSERIES_PSERIES_H

#include <linux/interrupt.h>
#include <asm/rtas.h>

struct device_node;

extern void request_event_sources_irqs(struct device_node *np,
				       irq_handler_t handler, const char *name);

#include <linux/of.h>

struct pt_regs;

extern int pSeries_system_reset_exception(struct pt_regs *regs);
extern int pSeries_machine_check_exception(struct pt_regs *regs);
<<<<<<< HEAD
=======
extern long pseries_machine_check_realmode(struct pt_regs *regs);
>>>>>>> upstream/android-13

#ifdef CONFIG_SMP
extern void smp_init_pseries(void);

/* Get state of physical CPU from query_cpu_stopped */
int smp_query_cpu_stopped(unsigned int pcpu);
#define QCSS_STOPPED 0
#define QCSS_STOPPING 1
#define QCSS_NOT_STOPPED 2
#define QCSS_HARDWARE_ERROR -1
#define QCSS_HARDWARE_BUSY -2
#else
<<<<<<< HEAD
static inline void smp_init_pseries(void) { };
=======
static inline void smp_init_pseries(void) { }
>>>>>>> upstream/android-13
#endif

extern void pseries_kexec_cpu_down(int crash_shutdown, int secondary);

extern void pSeries_final_fixup(void);

/* Poweron flag used for enabling auto ups restart */
extern unsigned long rtas_poweron_auto;

<<<<<<< HEAD
/* Provided by HVC VIO */
extern void hvc_vio_init_early(void);

=======
>>>>>>> upstream/android-13
/* Dynamic logical Partitioning/Mobility */
extern void dlpar_free_cc_nodes(struct device_node *);
extern void dlpar_free_cc_property(struct property *);
extern struct device_node *dlpar_configure_connector(__be32,
						struct device_node *);
extern int dlpar_attach_node(struct device_node *, struct device_node *);
extern int dlpar_detach_node(struct device_node *);
extern int dlpar_acquire_drc(u32 drc_index);
extern int dlpar_release_drc(u32 drc_index);
<<<<<<< HEAD

void queue_hotplug_event(struct pseries_hp_errorlog *hp_errlog,
			 struct completion *hotplug_done, int *rc);
#ifdef CONFIG_MEMORY_HOTPLUG
int dlpar_memory(struct pseries_hp_errorlog *hp_elog);
=======
extern int dlpar_unisolate_drc(u32 drc_index);

void queue_hotplug_event(struct pseries_hp_errorlog *hp_errlog);
int handle_dlpar_errorlog(struct pseries_hp_errorlog *hp_errlog);

#ifdef CONFIG_MEMORY_HOTPLUG
int dlpar_memory(struct pseries_hp_errorlog *hp_elog);
int dlpar_hp_pmem(struct pseries_hp_errorlog *hp_elog);
>>>>>>> upstream/android-13
#else
static inline int dlpar_memory(struct pseries_hp_errorlog *hp_elog)
{
	return -EOPNOTSUPP;
}
<<<<<<< HEAD
=======
static inline int dlpar_hp_pmem(struct pseries_hp_errorlog *hp_elog)
{
	return -EOPNOTSUPP;
}
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_HOTPLUG_CPU
int dlpar_cpu(struct pseries_hp_errorlog *hp_elog);
#else
static inline int dlpar_cpu(struct pseries_hp_errorlog *hp_elog)
{
	return -EOPNOTSUPP;
}
#endif

/* PCI root bridge prepare function override for pseries */
struct pci_host_bridge;
int pseries_root_bridge_prepare(struct pci_host_bridge *bridge);

extern struct pci_controller_ops pseries_pci_controller_ops;
<<<<<<< HEAD
=======
int pseries_msi_allocate_domains(struct pci_controller *phb);
void pseries_msi_free_domains(struct pci_controller *phb);
>>>>>>> upstream/android-13

unsigned long pseries_memory_block_size(void);

extern int CMO_PrPSP;
extern int CMO_SecPSP;
extern unsigned long CMO_PageSize;

static inline int cmo_get_primary_psp(void)
{
	return CMO_PrPSP;
}

static inline int cmo_get_secondary_psp(void)
{
	return CMO_SecPSP;
}

static inline unsigned long cmo_get_page_size(void)
{
	return CMO_PageSize;
}

int dlpar_workqueue_init(void);

<<<<<<< HEAD
void pseries_setup_rfi_flush(void);
=======
extern u32 pseries_security_flavor;
void pseries_setup_security_mitigations(void);
void pseries_lpar_read_hblkrm_characteristics(void);
>>>>>>> upstream/android-13

#endif /* _PSERIES_PSERIES_H */
