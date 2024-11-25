<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Generic show_mem() implementation
 *
 * Copyright (C) 2008 Johannes Weiner <hannes@saeurebad.de>
<<<<<<< HEAD
 * All code subject to the GPL version 2.
 */

#include <linux/mm.h>
#include <linux/quicklist.h>
#include <linux/cma.h>

=======
 */

#include <linux/mm.h>
#include <linux/cma.h>
#include <linux/vmalloc.h>
#include <trace/hooks/mm.h>
>>>>>>> upstream/android-13
void show_mem(unsigned int filter, nodemask_t *nodemask)
{
	pg_data_t *pgdat;
	unsigned long total = 0, reserved = 0, highmem = 0;

	printk("Mem-Info:\n");
<<<<<<< HEAD
	show_mem_extra_call_notifiers();
	show_free_areas(filter, nodemask);

	for_each_online_pgdat(pgdat) {
		unsigned long flags;
		int zoneid;

		pgdat_resize_lock(pgdat, &flags);
=======
	show_free_areas(filter, nodemask);

	for_each_online_pgdat(pgdat) {
		int zoneid;

>>>>>>> upstream/android-13
		for (zoneid = 0; zoneid < MAX_NR_ZONES; zoneid++) {
			struct zone *zone = &pgdat->node_zones[zoneid];
			if (!populated_zone(zone))
				continue;

			total += zone->present_pages;
<<<<<<< HEAD
			reserved += zone->present_pages - zone->managed_pages;
=======
			reserved += zone->present_pages - zone_managed_pages(zone);
>>>>>>> upstream/android-13

			if (is_highmem_idx(zoneid))
				highmem += zone->present_pages;
		}
<<<<<<< HEAD
		pgdat_resize_unlock(pgdat, &flags);
=======
>>>>>>> upstream/android-13
	}

	printk("%lu pages RAM\n", total);
	printk("%lu pages HighMem/MovableOnly\n", highmem);
	printk("%lu pages reserved\n", reserved);
#ifdef CONFIG_CMA
	printk("%lu pages cma reserved\n", totalcma_pages);
#endif
<<<<<<< HEAD
#ifdef CONFIG_QUICKLIST
	printk("%lu pages in pagetable cache\n",
		quicklist_total_size());
#endif
#ifdef CONFIG_MEMORY_FAILURE
	printk("%lu pages hwpoisoned\n", atomic_long_read(&num_poisoned_pages));
#endif
}
=======
#ifdef CONFIG_MEMORY_FAILURE
	printk("%lu pages hwpoisoned\n", atomic_long_read(&num_poisoned_pages));
#endif
	pr_info("%s: %lu kB\n", "VmallocUsed", vmalloc_nr_pages() << 2);
	trace_android_vh_show_mem(filter, nodemask);
}
EXPORT_SYMBOL_GPL(show_mem);
>>>>>>> upstream/android-13
