/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SPARC64_MMZONE_H
#define _SPARC64_MMZONE_H

<<<<<<< HEAD
#ifdef CONFIG_NEED_MULTIPLE_NODES
=======
#ifdef CONFIG_NUMA
>>>>>>> upstream/android-13

#include <linux/cpumask.h>

extern struct pglist_data *node_data[];

#define NODE_DATA(nid)		(node_data[nid])

extern int numa_cpu_lookup_table[];
extern cpumask_t numa_cpumask_lookup_table[];

<<<<<<< HEAD
#endif /* CONFIG_NEED_MULTIPLE_NODES */
=======
#endif /* CONFIG_NUMA */
>>>>>>> upstream/android-13

#endif /* _SPARC64_MMZONE_H */
