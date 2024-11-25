/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_NODE_H_
#define _LINUX_NODE_H_

#include <linux/device.h>
#include <linux/cpumask.h>
#include <linux/workqueue.h>

struct node {
	struct device	dev;

#if defined(CONFIG_MEMORY_HOTPLUG_SPARSE) && defined(CONFIG_HUGETLBFS)
	struct work_struct	node_work;
#endif
};

struct memory_block;
extern struct node *node_devices[];
typedef  void (*node_registration_func_t)(struct node *);

#if defined(CONFIG_MEMORY_HOTPLUG_SPARSE) && defined(CONFIG_NUMA)
int link_mem_sections(int nid, unsigned long start_pfn,
		      unsigned long end_pfn,
		      enum meminit_context context);
#else
static inline int link_mem_sections(int nid, unsigned long start_pfn,
				    unsigned long end_pfn,
				    enum meminit_context context)
{
	return 0;
}
#endif

extern void unregister_node(struct node *node);
#ifdef CONFIG_NUMA

extern int __register_one_node(int nid);


static inline int register_one_node(int nid)
{
	int error = 0;

	if (node_online(nid)) {
		struct pglist_data *pgdat = NODE_DATA(nid);
		unsigned long start_pfn = pgdat->node_start_pfn;
		unsigned long end_pfn = start_pfn + pgdat->node_spanned_pages;

		error = __register_one_node(nid);
		if (error)
			return error;
		
		error = link_mem_sections(nid, start_pfn, end_pfn,
					  MEMINIT_EARLY);
	}

	return error;
}

extern void unregister_one_node(int nid);
extern int register_cpu_under_node(unsigned int cpu, unsigned int nid);
extern int unregister_cpu_under_node(unsigned int cpu, unsigned int nid);
extern int register_mem_sect_under_node(struct memory_block *mem_blk,
						void *arg);
extern void unregister_memory_block_under_nodes(struct memory_block *mem_blk);

#ifdef CONFIG_HUGETLBFS
extern void register_hugetlbfs_with_node(node_registration_func_t doregister,
					 node_registration_func_t unregister);
#endif
#else
static inline int __register_one_node(int nid)
{
	return 0;
}
static inline int register_one_node(int nid)
{
	return 0;
}
static inline int unregister_one_node(int nid)
{
	return 0;
}
static inline int register_cpu_under_node(unsigned int cpu, unsigned int nid)
{
	return 0;
}
static inline int unregister_cpu_under_node(unsigned int cpu, unsigned int nid)
{
	return 0;
}
static inline int register_mem_sect_under_node(struct memory_block *mem_blk,
							void *arg)
{
	return 0;
}
static inline void unregister_memory_block_under_nodes(struct memory_block *mem_blk)
{
}

static inline void register_hugetlbfs_with_node(node_registration_func_t reg,
						node_registration_func_t unreg)
{
}
#endif

#define to_node(device) container_of(device, struct node, dev)

#endif 
