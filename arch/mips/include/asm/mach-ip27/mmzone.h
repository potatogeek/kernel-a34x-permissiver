/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_MACH_MMZONE_H
#define _ASM_MACH_MMZONE_H

#include <asm/sn/addrs.h>
#include <asm/sn/arch.h>
<<<<<<< HEAD
#include <asm/sn/hub.h>

#define pa_to_nid(addr)		NASID_TO_COMPACT_NODEID(NASID_GET(addr))

#define LEVELS_PER_SLICE	128

struct slice_data {
	unsigned long irq_enable_mask[2];
	int level_to_irq[LEVELS_PER_SLICE];
};
=======
#include <asm/sn/agent.h>
#include <asm/sn/klkernvars.h>

#define pa_to_nid(addr)		NASID_GET(addr)
>>>>>>> upstream/android-13

struct hub_data {
	kern_vars_t	kern_vars;
	DECLARE_BITMAP(h_bigwin_used, HUB_NUM_BIG_WINDOW);
	cpumask_t	h_cpus;
<<<<<<< HEAD
	unsigned long slice_map;
	unsigned long irq_alloc_mask[2];
	struct slice_data slice[2];
=======
>>>>>>> upstream/android-13
};

struct node_data {
	struct pglist_data pglist;
	struct hub_data hub;
};

extern struct node_data *__node_data[];

#define NODE_DATA(n)		(&__node_data[(n)]->pglist)
#define hub_data(n)		(&__node_data[(n)]->hub)

#endif /* _ASM_MACH_MMZONE_H */
