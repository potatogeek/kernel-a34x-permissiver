// SPDX-License-Identifier: GPL-2.0
/*
 * memory.c: PROM library functions for acquiring/using memory descriptors
 *	     given to us from the ARCS firmware.
 *
 * Copyright (C) 1996 by David S. Miller
 * Copyright (C) 1999, 2000, 2001 by Ralf Baechle
 * Copyright (C) 1999, 2000 by Silicon Graphics, Inc.
 *
 * PROM library functions for acquiring/using memory descriptors given to us
 * from the ARCS firmware.  This is only used when CONFIG_ARC_MEMORY is set
 * because on some machines like SGI IP27 the ARC memory configuration data
 * completely bogus and alternate easier to use mechanisms are available.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
#include <linux/memblock.h>
>>>>>>> upstream/android-13
#include <linux/swap.h>

#include <asm/sgialib.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/bootinfo.h>

#undef DEBUG

<<<<<<< HEAD
=======
#define MAX_PROM_MEM 5
static phys_addr_t prom_mem_base[MAX_PROM_MEM] __initdata;
static phys_addr_t prom_mem_size[MAX_PROM_MEM] __initdata;
static unsigned int nr_prom_mem __initdata;

>>>>>>> upstream/android-13
/*
 * For ARC firmware memory functions the unit of meassuring memory is always
 * a 4k page of memory
 */
#define ARC_PAGE_SHIFT	12

struct linux_mdesc * __init ArcGetMemoryDescriptor(struct linux_mdesc *Current)
{
	return (struct linux_mdesc *) ARC_CALL1(get_mdesc, Current);
}

#ifdef DEBUG /* convenient for debugging */
static char *arcs_mtypes[8] = {
	"Exception Block",
	"ARCS Romvec Page",
	"Free/Contig RAM",
	"Generic Free RAM",
	"Bad Memory",
	"Standalone Program Pages",
	"ARCS Temp Storage Area",
	"ARCS Permanent Storage Area"
};

static char *arc_mtypes[8] = {
	"Exception Block",
	"SystemParameterBlock",
	"FreeMemory",
	"Bad Memory",
	"LoadedProgram",
	"FirmwareTemporary",
	"FirmwarePermanent",
	"FreeContiguous"
};
#define mtypes(a) (prom_flags & PROM_FLAG_ARCS) ? arcs_mtypes[a.arcs] \
						: arc_mtypes[a.arc]
#endif

<<<<<<< HEAD
=======
enum {
	mem_free, mem_prom_used, mem_reserved
};

>>>>>>> upstream/android-13
static inline int memtype_classify_arcs(union linux_memtypes type)
{
	switch (type.arcs) {
	case arcs_fcontig:
	case arcs_free:
<<<<<<< HEAD
		return BOOT_MEM_RAM;
	case arcs_atmp:
		return BOOT_MEM_ROM_DATA;
=======
		return mem_free;
	case arcs_atmp:
		return mem_prom_used;
>>>>>>> upstream/android-13
	case arcs_eblock:
	case arcs_rvpage:
	case arcs_bmem:
	case arcs_prog:
	case arcs_aperm:
<<<<<<< HEAD
		return BOOT_MEM_RESERVED;
=======
		return mem_reserved;
>>>>>>> upstream/android-13
	default:
		BUG();
	}
	while(1);				/* Nuke warning.  */
}

static inline int memtype_classify_arc(union linux_memtypes type)
{
	switch (type.arc) {
	case arc_free:
	case arc_fcontig:
<<<<<<< HEAD
		return BOOT_MEM_RAM;
	case arc_atmp:
		return BOOT_MEM_ROM_DATA;
=======
		return mem_free;
	case arc_atmp:
		return mem_prom_used;
>>>>>>> upstream/android-13
	case arc_eblock:
	case arc_rvpage:
	case arc_bmem:
	case arc_prog:
	case arc_aperm:
<<<<<<< HEAD
		return BOOT_MEM_RESERVED;
=======
		return mem_reserved;
>>>>>>> upstream/android-13
	default:
		BUG();
	}
	while(1);				/* Nuke warning.  */
}

static int __init prom_memtype_classify(union linux_memtypes type)
{
	if (prom_flags & PROM_FLAG_ARCS)	/* SGI is ``different'' ... */
		return memtype_classify_arcs(type);

	return memtype_classify_arc(type);
}

<<<<<<< HEAD
void __init prom_meminit(void)
=======
void __weak __init prom_meminit(void)
>>>>>>> upstream/android-13
{
	struct linux_mdesc *p;

#ifdef DEBUG
	int i = 0;

	printk("ARCS MEMORY DESCRIPTOR dump:\n");
	p = ArcGetMemoryDescriptor(PROM_NULL_MDESC);
	while(p) {
		printk("[%d,%p]: base<%08lx> pages<%08lx> type<%s>\n",
		       i, p, p->base, p->pages, mtypes(p->type));
		p = ArcGetMemoryDescriptor(p);
		i++;
	}
#endif

<<<<<<< HEAD
=======
	nr_prom_mem = 0;
>>>>>>> upstream/android-13
	p = PROM_NULL_MDESC;
	while ((p = ArcGetMemoryDescriptor(p))) {
		unsigned long base, size;
		long type;

		base = p->base << ARC_PAGE_SHIFT;
		size = p->pages << ARC_PAGE_SHIFT;
		type = prom_memtype_classify(p->type);

<<<<<<< HEAD
		add_memory_region(base, size, type);
	}
}

void __init prom_free_prom_memory(void)
{
	unsigned long addr;
=======
		/* ignore mirrored RAM on IP28/IP30 */
		if (base < PHYS_OFFSET)
			continue;

		memblock_add(base, size);

		if (type == mem_reserved)
			memblock_reserve(base, size);

		if (type == mem_prom_used) {
			memblock_reserve(base, size);
			if (nr_prom_mem >= 5) {
				pr_err("Too many ROM DATA regions");
				continue;
			}
			prom_mem_base[nr_prom_mem] = base;
			prom_mem_size[nr_prom_mem] = size;
			nr_prom_mem++;
		}
	}
}

void __weak __init prom_cleanup(void)
{
}

void __init prom_free_prom_memory(void)
{
>>>>>>> upstream/android-13
	int i;

	if (prom_flags & PROM_FLAG_DONT_FREE_TEMP)
		return;

<<<<<<< HEAD
	for (i = 0; i < boot_mem_map.nr_map; i++) {
		if (boot_mem_map.map[i].type != BOOT_MEM_ROM_DATA)
			continue;

		addr = boot_mem_map.map[i].addr;
		free_init_pages("prom memory",
				addr, addr + boot_mem_map.map[i].size);
	}
=======
	for (i = 0; i < nr_prom_mem; i++) {
		free_init_pages("prom memory",
			prom_mem_base[i], prom_mem_base[i] + prom_mem_size[i]);
	}
	/*
	 * at this point it isn't safe to call PROM functions
	 * give platforms a way to do PROM cleanups
	 */
	prom_cleanup();
>>>>>>> upstream/android-13
}
