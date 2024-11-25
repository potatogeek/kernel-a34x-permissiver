/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_TOPOLOGY_H
#define _ASM_POWERPC_TOPOLOGY_H
#ifdef __KERNEL__


struct device;
struct device_node;
<<<<<<< HEAD
=======
struct drmem_lmb;
>>>>>>> upstream/android-13

#ifdef CONFIG_NUMA

/*
 * If zone_reclaim_mode is enabled, a RECLAIM_DISTANCE of 10 will mean that
 * all zones on all nodes will be eligible for zone_reclaim().
 */
#define RECLAIM_DISTANCE 10

#include <asm/mmzone.h>

#define cpumask_of_node(node) ((node) == -1 ?				\
			       cpu_all_mask :				\
			       node_to_cpumask_map[node])

struct pci_bus;
#ifdef CONFIG_PCI
extern int pcibus_to_node(struct pci_bus *bus);
#else
static inline int pcibus_to_node(struct pci_bus *bus)
{
	return -1;
}
#endif

#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))

<<<<<<< HEAD
=======
int cpu_relative_distance(__be32 *cpu1_assoc, __be32 *cpu2_assoc);
>>>>>>> upstream/android-13
extern int __node_distance(int, int);
#define node_distance(a, b) __node_distance(a, b)

extern void __init dump_numa_cpu_topology(void);

extern int sysfs_add_device_to_node(struct device *dev, int nid);
extern void sysfs_remove_device_from_node(struct device *dev, int nid);
<<<<<<< HEAD
extern int numa_update_cpu_topology(bool cpus_locked);
=======
>>>>>>> upstream/android-13

static inline void update_numa_cpu_lookup_table(unsigned int cpu, int node)
{
	numa_cpu_lookup_table[cpu] = node;
}

static inline int early_cpu_to_node(int cpu)
{
	int nid;

	nid = numa_cpu_lookup_table[cpu];

	/*
	 * Fall back to node 0 if nid is unset (it should be, except bugs).
	 * This allows callers to safely do NODE_DATA(early_cpu_to_node(cpu)).
	 */
	return (nid < 0) ? 0 : nid;
}
<<<<<<< HEAD
=======

int of_drconf_to_nid_single(struct drmem_lmb *lmb);
void update_numa_distance(struct device_node *node);

extern void map_cpu_to_node(int cpu, int node);
#ifdef CONFIG_HOTPLUG_CPU
extern void unmap_cpu_from_node(unsigned long cpu);
#endif /* CONFIG_HOTPLUG_CPU */

>>>>>>> upstream/android-13
#else

static inline int early_cpu_to_node(int cpu) { return 0; }

static inline void dump_numa_cpu_topology(void) {}

static inline int sysfs_add_device_to_node(struct device *dev, int nid)
{
	return 0;
}

static inline void sysfs_remove_device_from_node(struct device *dev,
						int nid)
{
}

<<<<<<< HEAD
static inline int numa_update_cpu_topology(bool cpus_locked)
=======
static inline void update_numa_cpu_lookup_table(unsigned int cpu, int node) {}

static inline int cpu_relative_distance(__be32 *cpu1_assoc, __be32 *cpu2_assoc)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static inline void update_numa_cpu_lookup_table(unsigned int cpu, int node) {}
=======
static inline int of_drconf_to_nid_single(struct drmem_lmb *lmb)
{
	return first_online_node;
}

static inline void update_numa_distance(struct device_node *node) {}

#ifdef CONFIG_SMP
static inline void map_cpu_to_node(int cpu, int node) {}
#ifdef CONFIG_HOTPLUG_CPU
static inline void unmap_cpu_from_node(unsigned long cpu) {}
#endif /* CONFIG_HOTPLUG_CPU */
#endif /* CONFIG_SMP */
>>>>>>> upstream/android-13

#endif /* CONFIG_NUMA */

#if defined(CONFIG_NUMA) && defined(CONFIG_PPC_SPLPAR)
<<<<<<< HEAD
extern int start_topology_update(void);
extern int stop_topology_update(void);
extern int prrn_is_enabled(void);
extern int find_and_online_cpu_nid(int cpu);
extern int timed_topology_update(int nsecs);
extern void __init shared_proc_topology_init(void);
#else
static inline int start_topology_update(void)
{
	return 0;
}
static inline int stop_topology_update(void)
{
	return 0;
}
static inline int prrn_is_enabled(void)
{
	return 0;
}
=======
extern int find_and_online_cpu_nid(int cpu);
extern int cpu_to_coregroup_id(int cpu);
#else
>>>>>>> upstream/android-13
static inline int find_and_online_cpu_nid(int cpu)
{
	return 0;
}
<<<<<<< HEAD
static inline int timed_topology_update(int nsecs)
{
	return 0;
}

#ifdef CONFIG_SMP
static inline void shared_proc_topology_init(void) {}
#endif
=======

static inline int cpu_to_coregroup_id(int cpu)
{
#ifdef CONFIG_SMP
	return cpu_to_core_id(cpu);
#else
	return 0;
#endif
}

>>>>>>> upstream/android-13
#endif /* CONFIG_NUMA && CONFIG_PPC_SPLPAR */

#include <asm-generic/topology.h>

#ifdef CONFIG_SMP
#include <asm/cputable.h>

#ifdef CONFIG_PPC64
#include <asm/smp.h>

#define topology_physical_package_id(cpu)	(cpu_to_chip_id(cpu))
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
#define topology_sibling_cpumask(cpu)	(per_cpu(cpu_sibling_map, cpu))
#define topology_core_cpumask(cpu)	(per_cpu(cpu_core_map, cpu))
#define topology_core_id(cpu)		(cpu_to_core_id(cpu))

<<<<<<< HEAD
int dlpar_cpu_readd(int cpu);
=======
>>>>>>> upstream/android-13
#endif
#endif

#endif /* __KERNEL__ */
#endif	/* _ASM_POWERPC_TOPOLOGY_H */
