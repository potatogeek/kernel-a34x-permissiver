/* SPDX-License-Identifier: GPL-2.0 */
/*
 * /proc/kcore definitions
 */
#ifndef _LINUX_KCORE_H
#define _LINUX_KCORE_H

enum kcore_type {
	KCORE_TEXT,
	KCORE_VMALLOC,
	KCORE_RAM,
	KCORE_VMEMMAP,
	KCORE_USER,
<<<<<<< HEAD
	KCORE_OTHER,
	KCORE_REMAP,
=======
>>>>>>> upstream/android-13
};

struct kcore_list {
	struct list_head list;
	unsigned long addr;
<<<<<<< HEAD
	unsigned long vaddr;
=======
>>>>>>> upstream/android-13
	size_t size;
	int type;
};

struct vmcore {
	struct list_head list;
	unsigned long long paddr;
	unsigned long long size;
	loff_t offset;
};

struct vmcoredd_node {
	struct list_head list;	/* List of dumps */
	void *buf;		/* Buffer containing device's dump */
	unsigned int size;	/* Size of the buffer */
};

#ifdef CONFIG_PROC_KCORE
void __init kclist_add(struct kcore_list *, void *, size_t, int type);
<<<<<<< HEAD
static inline
void kclist_add_remap(struct kcore_list *m, void *addr, void *vaddr, size_t sz)
{
	m->vaddr = (unsigned long)vaddr;
	kclist_add(m, addr, sz, KCORE_REMAP);
}
=======
>>>>>>> upstream/android-13

extern int __init register_mem_pfn_is_ram(int (*fn)(unsigned long pfn));
#else
static inline
void kclist_add(struct kcore_list *new, void *addr, size_t size, int type)
{
}
<<<<<<< HEAD

static inline
void kclist_add_remap(struct kcore_list *m, void *addr, void *vaddr, size_t sz)
{
}
=======
>>>>>>> upstream/android-13
#endif

#endif /* _LINUX_KCORE_H */
