// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/percpu.h>
<<<<<<< HEAD
#include <linux/quicklist.h>
=======
>>>>>>> upstream/android-13
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>
#ifdef CONFIG_CMA
#include <linux/cma.h>
#endif
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include "internal.h"

=======
#include "internal.h"
#include <trace/hooks/mm.h>
>>>>>>> upstream/android-13
void __attribute__((weak)) arch_report_meminfo(struct seq_file *m)
{
}

static void show_val_kb(struct seq_file *m, const char *s, unsigned long num)
{
	seq_put_decimal_ull_width(m, s, num << (PAGE_SHIFT - 10), 8);
	seq_write(m, " kB\n", 4);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_RBIN
unsigned long rbin_total;
#endif

>>>>>>> upstream/android-13
static int meminfo_proc_show(struct seq_file *m, void *v)
{
	struct sysinfo i;
	unsigned long committed;
	long cached;
	long available;
	unsigned long pages[NR_LRU_LISTS];
	unsigned long sreclaimable, sunreclaim;
	int lru;
<<<<<<< HEAD
	long rbinfree;

	si_meminfo(&i);
	si_swapinfo(&i);
	committed = percpu_counter_read_positive(&vm_committed_as);

	cached = global_node_page_state(NR_FILE_PAGES) -
			total_swapcache_pages() - i.bufferram;
#ifdef CONFIG_ION_RBIN_HEAP
	cached += atomic_read(&rbin_cached_pages);
#endif
=======
#ifdef CONFIG_RBIN
	int stats[NR_RBIN_STAT_ITEMS] = {0,};
#endif

	si_meminfo(&i);
	si_swapinfo(&i);
	committed = vm_memory_committed();

	cached = global_node_page_state(NR_FILE_PAGES) -
			total_swapcache_pages() - i.bufferram;
#ifdef CONFIG_RBIN
	rbin_oem_func(GET_RBIN_STATS, stats);
	cached += stats[RBIN_CACHED];
#endif

>>>>>>> upstream/android-13
	if (cached < 0)
		cached = 0;

	for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
		pages[lru] = global_node_page_state(NR_LRU_BASE + lru);

	available = si_mem_available();
<<<<<<< HEAD
	rbinfree = global_zone_page_state(NR_FREE_RBIN_PAGES);
	sreclaimable = global_node_page_state(NR_SLAB_RECLAIMABLE);
	sunreclaim = global_node_page_state(NR_SLAB_UNRECLAIMABLE);

	show_val_kb(m, "MemTotal:       ", i.totalram);
	show_val_kb(m, "MemFree:        ", i.freeram + rbinfree);
	show_val_kb(m, "MemAvailable:   ", available + rbinfree);
=======
	sreclaimable = global_node_page_state_pages(NR_SLAB_RECLAIMABLE_B);
	sunreclaim = global_node_page_state_pages(NR_SLAB_UNRECLAIMABLE_B);

	show_val_kb(m, "MemTotal:       ", i.totalram);
#ifdef CONFIG_RBIN
	show_val_kb(m, "MemFree:        ", i.freeram + stats[RBIN_FREE]);
	show_val_kb(m, "MemAvailable:   ", available + stats[RBIN_FREE]);
#else
	show_val_kb(m, "MemFree:        ", i.freeram);
	show_val_kb(m, "MemAvailable:   ", available);
#endif
>>>>>>> upstream/android-13
	show_val_kb(m, "Buffers:        ", i.bufferram);
	show_val_kb(m, "Cached:         ", cached);
	show_val_kb(m, "SwapCached:     ", total_swapcache_pages());
	show_val_kb(m, "Active:         ", pages[LRU_ACTIVE_ANON] +
					   pages[LRU_ACTIVE_FILE]);
	show_val_kb(m, "Inactive:       ", pages[LRU_INACTIVE_ANON] +
					   pages[LRU_INACTIVE_FILE]);
	show_val_kb(m, "Active(anon):   ", pages[LRU_ACTIVE_ANON]);
	show_val_kb(m, "Inactive(anon): ", pages[LRU_INACTIVE_ANON]);
	show_val_kb(m, "Active(file):   ", pages[LRU_ACTIVE_FILE]);
	show_val_kb(m, "Inactive(file): ", pages[LRU_INACTIVE_FILE]);
	show_val_kb(m, "Unevictable:    ", pages[LRU_UNEVICTABLE]);
	show_val_kb(m, "Mlocked:        ", global_zone_page_state(NR_MLOCK));

#ifdef CONFIG_HIGHMEM
	show_val_kb(m, "HighTotal:      ", i.totalhigh);
	show_val_kb(m, "HighFree:       ", i.freehigh);
	show_val_kb(m, "LowTotal:       ", i.totalram - i.totalhigh);
	show_val_kb(m, "LowFree:        ", i.freeram - i.freehigh);
#endif

#ifndef CONFIG_MMU
	show_val_kb(m, "MmapCopy:       ",
		    (unsigned long)atomic_long_read(&mmap_pages_allocated));
#endif
<<<<<<< HEAD

#ifdef CONFIG_ION_RBIN_HEAP
	show_val_kb(m, "RbinTotal:      ", totalrbin_pages);
	show_val_kb(m, "RbinAlloced:    ",
		    atomic_read(&rbin_allocated_pages)
		    + atomic_read(&rbin_pool_pages));
	show_val_kb(m, "RbinPool:       ", atomic_read(&rbin_pool_pages));
	show_val_kb(m, "RbinFree:       ", rbinfree);
	show_val_kb(m, "RbinCached:     ",
		    atomic_read(&rbin_cached_pages));
#endif
=======
#ifdef CONFIG_RBIN
	show_val_kb(m, "RbinTotal:      ", rbin_total);
	show_val_kb(m, "RbinAlloced:    ", stats[RBIN_ALLOCATED] + stats[RBIN_POOL]);
	show_val_kb(m, "RbinPool:       ", stats[RBIN_POOL]);
	show_val_kb(m, "RbinFree:       ", stats[RBIN_FREE]);
	show_val_kb(m, "RbinCached:     ", stats[RBIN_CACHED]);
#endif

>>>>>>> upstream/android-13
	show_val_kb(m, "SwapTotal:      ", i.totalswap);
	show_val_kb(m, "SwapFree:       ", i.freeswap);
	show_val_kb(m, "Dirty:          ",
		    global_node_page_state(NR_FILE_DIRTY));
	show_val_kb(m, "Writeback:      ",
		    global_node_page_state(NR_WRITEBACK));
	show_val_kb(m, "AnonPages:      ",
		    global_node_page_state(NR_ANON_MAPPED));
	show_val_kb(m, "Mapped:         ",
		    global_node_page_state(NR_FILE_MAPPED));
	show_val_kb(m, "Shmem:          ", i.sharedram);
	show_val_kb(m, "KReclaimable:   ", sreclaimable +
		    global_node_page_state(NR_KERNEL_MISC_RECLAIMABLE));
	show_val_kb(m, "Slab:           ", sreclaimable + sunreclaim);
	show_val_kb(m, "SReclaimable:   ", sreclaimable);
	show_val_kb(m, "SUnreclaim:     ", sunreclaim);
	seq_printf(m, "KernelStack:    %8lu kB\n",
<<<<<<< HEAD
		   global_zone_page_state(NR_KERNEL_STACK_KB));
#ifdef CONFIG_SHADOW_CALL_STACK
	seq_printf(m, "ShadowCallStack:%8lu kB\n",
		   global_zone_page_state(NR_KERNEL_SCS_BYTES) / 1024);
#endif
	show_val_kb(m, "PageTables:     ",
		    global_zone_page_state(NR_PAGETABLE));
#ifdef CONFIG_QUICKLIST
	show_val_kb(m, "Quicklists:     ", quicklist_total_size());
#endif

	show_val_kb(m, "NFS_Unstable:   ",
		    global_node_page_state(NR_UNSTABLE_NFS));
=======
		   global_node_page_state(NR_KERNEL_STACK_KB));
#ifdef CONFIG_SHADOW_CALL_STACK
	seq_printf(m, "ShadowCallStack:%8lu kB\n",
		   global_node_page_state(NR_KERNEL_SCS_KB));
#endif
	show_val_kb(m, "PageTables:     ",
		    global_node_page_state(NR_PAGETABLE));

	show_val_kb(m, "NFS_Unstable:   ", 0);
>>>>>>> upstream/android-13
	show_val_kb(m, "Bounce:         ",
		    global_zone_page_state(NR_BOUNCE));
	show_val_kb(m, "WritebackTmp:   ",
		    global_node_page_state(NR_WRITEBACK_TEMP));
	show_val_kb(m, "CommitLimit:    ", vm_commit_limit());
	show_val_kb(m, "Committed_AS:   ", committed);
	seq_printf(m, "VmallocTotal:   %8lu kB\n",
		   (unsigned long)VMALLOC_TOTAL >> 10);
	show_val_kb(m, "VmallocUsed:    ", vmalloc_nr_pages());
	show_val_kb(m, "VmallocChunk:   ", 0ul);
	show_val_kb(m, "Percpu:         ", pcpu_nr_pages());

#ifdef CONFIG_MEMORY_FAILURE
	seq_printf(m, "HardwareCorrupted: %5lu kB\n",
		   atomic_long_read(&num_poisoned_pages) << (PAGE_SHIFT - 10));
#endif

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	show_val_kb(m, "AnonHugePages:  ",
<<<<<<< HEAD
		    global_node_page_state(NR_ANON_THPS) * HPAGE_PMD_NR);
	show_val_kb(m, "ShmemHugePages: ",
		    global_node_page_state(NR_SHMEM_THPS) * HPAGE_PMD_NR);
	show_val_kb(m, "ShmemPmdMapped: ",
		    global_node_page_state(NR_SHMEM_PMDMAPPED) * HPAGE_PMD_NR);
=======
		    global_node_page_state(NR_ANON_THPS));
	show_val_kb(m, "ShmemHugePages: ",
		    global_node_page_state(NR_SHMEM_THPS));
	show_val_kb(m, "ShmemPmdMapped: ",
		    global_node_page_state(NR_SHMEM_PMDMAPPED));
	show_val_kb(m, "FileHugePages:  ",
		    global_node_page_state(NR_FILE_THPS));
	show_val_kb(m, "FilePmdMapped:  ",
		    global_node_page_state(NR_FILE_PMDMAPPED));
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_CMA
	show_val_kb(m, "CmaTotal:       ", totalcma_pages);
	show_val_kb(m, "CmaFree:        ",
		    global_zone_page_state(NR_FREE_CMA_PAGES));
#endif
<<<<<<< HEAD
=======
	trace_android_vh_meminfo_proc_show(m);
>>>>>>> upstream/android-13

	hugetlb_report_meminfo(m);

	arch_report_meminfo(m);

	return 0;
}

static int __init proc_meminfo_init(void)
{
	proc_create_single("meminfo", 0, NULL, meminfo_proc_show);
	return 0;
}
fs_initcall(proc_meminfo_init);
