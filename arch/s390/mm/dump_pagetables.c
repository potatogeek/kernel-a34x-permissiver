// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/sections.h>
#include <asm/pgtable.h>
=======
#include <linux/set_memory.h>
#include <linux/ptdump.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/mm.h>
#include <linux/kfence.h>
#include <linux/kasan.h>
#include <asm/ptdump.h>
#include <asm/kasan.h>
#include <asm/sections.h>
>>>>>>> upstream/android-13

static unsigned long max_addr;

struct addr_marker {
	unsigned long start_address;
	const char *name;
};

enum address_markers_idx {
<<<<<<< HEAD
	IDENTITY_NR = 0,
	KERNEL_START_NR,
	KERNEL_END_NR,
	VMEMMAP_NR,
	VMALLOC_NR,
	MODULES_NR,
};

static struct addr_marker address_markers[] = {
	[IDENTITY_NR]	  = {0, "Identity Mapping"},
	[KERNEL_START_NR] = {(unsigned long)_stext, "Kernel Image Start"},
	[KERNEL_END_NR]	  = {(unsigned long)_end, "Kernel Image End"},
	[VMEMMAP_NR]	  = {0, "vmemmap Area"},
	[VMALLOC_NR]	  = {0, "vmalloc Area"},
	[MODULES_NR]	  = {0, "Modules Area"},
=======
	IDENTITY_BEFORE_NR = 0,
	IDENTITY_BEFORE_END_NR,
	KERNEL_START_NR,
	KERNEL_END_NR,
#ifdef CONFIG_KFENCE
	KFENCE_START_NR,
	KFENCE_END_NR,
#endif
	IDENTITY_AFTER_NR,
	IDENTITY_AFTER_END_NR,
#ifdef CONFIG_KASAN
	KASAN_SHADOW_START_NR,
	KASAN_SHADOW_END_NR,
#endif
	VMEMMAP_NR,
	VMEMMAP_END_NR,
	VMALLOC_NR,
	VMALLOC_END_NR,
	MODULES_NR,
	MODULES_END_NR,
};

static struct addr_marker address_markers[] = {
	[IDENTITY_BEFORE_NR]	= {0, "Identity Mapping Start"},
	[IDENTITY_BEFORE_END_NR] = {(unsigned long)_stext, "Identity Mapping End"},
	[KERNEL_START_NR]	= {(unsigned long)_stext, "Kernel Image Start"},
	[KERNEL_END_NR]		= {(unsigned long)_end, "Kernel Image End"},
#ifdef CONFIG_KFENCE
	[KFENCE_START_NR]	= {0, "KFence Pool Start"},
	[KFENCE_END_NR]		= {0, "KFence Pool End"},
#endif
	[IDENTITY_AFTER_NR]	= {(unsigned long)_end, "Identity Mapping Start"},
	[IDENTITY_AFTER_END_NR]	= {0, "Identity Mapping End"},
#ifdef CONFIG_KASAN
	[KASAN_SHADOW_START_NR]	= {KASAN_SHADOW_START, "Kasan Shadow Start"},
	[KASAN_SHADOW_END_NR]	= {KASAN_SHADOW_END, "Kasan Shadow End"},
#endif
	[VMEMMAP_NR]		= {0, "vmemmap Area Start"},
	[VMEMMAP_END_NR]	= {0, "vmemmap Area End"},
	[VMALLOC_NR]		= {0, "vmalloc Area Start"},
	[VMALLOC_END_NR]	= {0, "vmalloc Area End"},
	[MODULES_NR]		= {0, "Modules Area Start"},
	[MODULES_END_NR]	= {0, "Modules Area End"},
>>>>>>> upstream/android-13
	{ -1, NULL }
};

struct pg_state {
<<<<<<< HEAD
	int level;
	unsigned int current_prot;
	unsigned long start_address;
	unsigned long current_address;
	const struct addr_marker *marker;
};

=======
	struct ptdump_state ptdump;
	struct seq_file *seq;
	int level;
	unsigned int current_prot;
	bool check_wx;
	unsigned long wx_pages;
	unsigned long start_address;
	const struct addr_marker *marker;
};

#define pt_dump_seq_printf(m, fmt, args...)	\
({						\
	struct seq_file *__m = (m);		\
						\
	if (__m)				\
		seq_printf(__m, fmt, ##args);	\
})

#define pt_dump_seq_puts(m, fmt)		\
({						\
	struct seq_file *__m = (m);		\
						\
	if (__m)				\
		seq_printf(__m, fmt);		\
})

>>>>>>> upstream/android-13
static void print_prot(struct seq_file *m, unsigned int pr, int level)
{
	static const char * const level_name[] =
		{ "ASCE", "PGD", "PUD", "PMD", "PTE" };

<<<<<<< HEAD
	seq_printf(m, "%s ", level_name[level]);
	if (pr & _PAGE_INVALID) {
		seq_printf(m, "I\n");
		return;
	}
	seq_puts(m, (pr & _PAGE_PROTECT) ? "RO " : "RW ");
	seq_puts(m, (pr & _PAGE_NOEXEC) ? "NX\n" : "X\n");
}

static void note_page(struct seq_file *m, struct pg_state *st,
		     unsigned int new_prot, int level)
{
	static const char units[] = "KMGTPE";
	int width = sizeof(unsigned long) * 2;
	const char *unit = units;
	unsigned int prot, cur;
	unsigned long delta;

	/*
	 * If we have a "break" in the series, we need to flush the state
	 * that we have now. "break" is either changing perms, levels or
	 * address space marker.
	 */
	prot = new_prot;
	cur = st->current_prot;

	if (!st->level) {
		/* First entry */
		st->current_prot = new_prot;
		st->level = level;
		st->marker = address_markers;
		seq_printf(m, "---[ %s ]---\n", st->marker->name);
	} else if (prot != cur || level != st->level ||
		   st->current_address >= st->marker[1].start_address) {
		/* Print the actual finished series */
		seq_printf(m, "0x%0*lx-0x%0*lx",
			   width, st->start_address,
			   width, st->current_address);
		delta = (st->current_address - st->start_address) >> 10;
=======
	pt_dump_seq_printf(m, "%s ", level_name[level]);
	if (pr & _PAGE_INVALID) {
		pt_dump_seq_printf(m, "I\n");
		return;
	}
	pt_dump_seq_puts(m, (pr & _PAGE_PROTECT) ? "RO " : "RW ");
	pt_dump_seq_puts(m, (pr & _PAGE_NOEXEC) ? "NX\n" : "X\n");
}

static void note_prot_wx(struct pg_state *st, unsigned long addr)
{
#ifdef CONFIG_DEBUG_WX
	if (!st->check_wx)
		return;
	if (st->current_prot & _PAGE_INVALID)
		return;
	if (st->current_prot & _PAGE_PROTECT)
		return;
	if (st->current_prot & _PAGE_NOEXEC)
		return;
	/* The first lowcore page is currently still W+X. */
	if (addr == PAGE_SIZE)
		return;
	WARN_ONCE(1, "s390/mm: Found insecure W+X mapping at address %pS\n",
		  (void *)st->start_address);
	st->wx_pages += (addr - st->start_address) / PAGE_SIZE;
#endif /* CONFIG_DEBUG_WX */
}

static void note_page(struct ptdump_state *pt_st, unsigned long addr, int level, u64 val)
{
	int width = sizeof(unsigned long) * 2;
	static const char units[] = "KMGTPE";
	const char *unit = units;
	unsigned long delta;
	struct pg_state *st;
	struct seq_file *m;
	unsigned int prot;

	st = container_of(pt_st, struct pg_state, ptdump);
	m = st->seq;
	prot = val & (_PAGE_PROTECT | _PAGE_NOEXEC);
	if (level == 4 && (val & _PAGE_INVALID))
		prot = _PAGE_INVALID;
	/* For pmd_none() & friends val gets passed as zero. */
	if (level != 4 && !val)
		prot = _PAGE_INVALID;
	/* Final flush from generic code. */
	if (level == -1)
		addr = max_addr;
	if (st->level == -1) {
		pt_dump_seq_printf(m, "---[ %s ]---\n", st->marker->name);
		st->start_address = addr;
		st->current_prot = prot;
		st->level = level;
	} else if (prot != st->current_prot || level != st->level ||
		   addr >= st->marker[1].start_address) {
		note_prot_wx(st, addr);
		pt_dump_seq_printf(m, "0x%0*lx-0x%0*lx ",
				   width, st->start_address,
				   width, addr);
		delta = (addr - st->start_address) >> 10;
>>>>>>> upstream/android-13
		while (!(delta & 0x3ff) && unit[1]) {
			delta >>= 10;
			unit++;
		}
<<<<<<< HEAD
		seq_printf(m, "%9lu%c ", delta, *unit);
		print_prot(m, st->current_prot, st->level);
		if (st->current_address >= st->marker[1].start_address) {
			st->marker++;
			seq_printf(m, "---[ %s ]---\n", st->marker->name);
		}
		st->start_address = st->current_address;
		st->current_prot = new_prot;
=======
		pt_dump_seq_printf(m, "%9lu%c ", delta, *unit);
		print_prot(m, st->current_prot, st->level);
		while (addr >= st->marker[1].start_address) {
			st->marker++;
			pt_dump_seq_printf(m, "---[ %s ]---\n", st->marker->name);
		}
		st->start_address = addr;
		st->current_prot = prot;
>>>>>>> upstream/android-13
		st->level = level;
	}
}

<<<<<<< HEAD
/*
 * The actual page table walker functions. In order to keep the
 * implementation of print_prot() short, we only check and pass
 * _PAGE_INVALID and _PAGE_PROTECT flags to note_page() if a region,
 * segment or page table entry is invalid or read-only.
 * After all it's just a hint that the current level being walked
 * contains an invalid or read-only entry.
 */
static void walk_pte_level(struct seq_file *m, struct pg_state *st,
			   pmd_t *pmd, unsigned long addr)
{
	unsigned int prot;
	pte_t *pte;
	int i;

	for (i = 0; i < PTRS_PER_PTE && addr < max_addr; i++) {
		st->current_address = addr;
		pte = pte_offset_kernel(pmd, addr);
		prot = pte_val(*pte) &
			(_PAGE_PROTECT | _PAGE_INVALID | _PAGE_NOEXEC);
		note_page(m, st, prot, 4);
		addr += PAGE_SIZE;
	}
}

static void walk_pmd_level(struct seq_file *m, struct pg_state *st,
			   pud_t *pud, unsigned long addr)
{
	unsigned int prot;
	pmd_t *pmd;
	int i;

	for (i = 0; i < PTRS_PER_PMD && addr < max_addr; i++) {
		st->current_address = addr;
		pmd = pmd_offset(pud, addr);
		if (!pmd_none(*pmd)) {
			if (pmd_large(*pmd)) {
				prot = pmd_val(*pmd) &
					(_SEGMENT_ENTRY_PROTECT |
					 _SEGMENT_ENTRY_NOEXEC);
				note_page(m, st, prot, 3);
			} else
				walk_pte_level(m, st, pmd, addr);
		} else
			note_page(m, st, _PAGE_INVALID, 3);
		addr += PMD_SIZE;
	}
}

static void walk_pud_level(struct seq_file *m, struct pg_state *st,
			   p4d_t *p4d, unsigned long addr)
{
	unsigned int prot;
	pud_t *pud;
	int i;

	for (i = 0; i < PTRS_PER_PUD && addr < max_addr; i++) {
		st->current_address = addr;
		pud = pud_offset(p4d, addr);
		if (!pud_none(*pud))
			if (pud_large(*pud)) {
				prot = pud_val(*pud) &
					(_REGION_ENTRY_PROTECT |
					 _REGION_ENTRY_NOEXEC);
				note_page(m, st, prot, 2);
			} else
				walk_pmd_level(m, st, pud, addr);
		else
			note_page(m, st, _PAGE_INVALID, 2);
		addr += PUD_SIZE;
	}
}

static void walk_p4d_level(struct seq_file *m, struct pg_state *st,
			   pgd_t *pgd, unsigned long addr)
{
	p4d_t *p4d;
	int i;

	for (i = 0; i < PTRS_PER_P4D && addr < max_addr; i++) {
		st->current_address = addr;
		p4d = p4d_offset(pgd, addr);
		if (!p4d_none(*p4d))
			walk_pud_level(m, st, p4d, addr);
		else
			note_page(m, st, _PAGE_INVALID, 2);
		addr += P4D_SIZE;
	}
}

static void walk_pgd_level(struct seq_file *m)
{
	unsigned long addr = 0;
	struct pg_state st;
	pgd_t *pgd;
	int i;

	memset(&st, 0, sizeof(st));
	for (i = 0; i < PTRS_PER_PGD && addr < max_addr; i++) {
		st.current_address = addr;
		pgd = pgd_offset_k(addr);
		if (!pgd_none(*pgd))
			walk_p4d_level(m, &st, pgd, addr);
		else
			note_page(m, &st, _PAGE_INVALID, 1);
		addr += PGDIR_SIZE;
		cond_resched();
	}
	/* Flush out the last page */
	st.current_address = max_addr;
	note_page(m, &st, 0, 0);
}

static int ptdump_show(struct seq_file *m, void *v)
{
	walk_pgd_level(m);
	return 0;
}

static int ptdump_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, ptdump_show, NULL);
}

static const struct file_operations ptdump_fops = {
	.open		= ptdump_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int pt_dump_init(void)
{
=======
#ifdef CONFIG_DEBUG_WX
void ptdump_check_wx(void)
{
	struct pg_state st = {
		.ptdump = {
			.note_page = note_page,
			.range = (struct ptdump_range[]) {
				{.start = 0, .end = max_addr},
				{.start = 0, .end = 0},
			}
		},
		.seq = NULL,
		.level = -1,
		.current_prot = 0,
		.check_wx = true,
		.wx_pages = 0,
		.start_address = 0,
		.marker = (struct addr_marker[]) {
			{ .start_address =  0, .name = NULL},
			{ .start_address = -1, .name = NULL},
		},
	};

	if (!MACHINE_HAS_NX)
		return;
	ptdump_walk_pgd(&st.ptdump, &init_mm, NULL);
	if (st.wx_pages)
		pr_warn("Checked W+X mappings: FAILED, %lu W+X pages found\n", st.wx_pages);
	else
		pr_info("Checked W+X mappings: passed, no unexpected W+X pages found\n");
}
#endif /* CONFIG_DEBUG_WX */

#ifdef CONFIG_PTDUMP_DEBUGFS
static int ptdump_show(struct seq_file *m, void *v)
{
	struct pg_state st = {
		.ptdump = {
			.note_page = note_page,
			.range = (struct ptdump_range[]) {
				{.start = 0, .end = max_addr},
				{.start = 0, .end = 0},
			}
		},
		.seq = m,
		.level = -1,
		.current_prot = 0,
		.check_wx = false,
		.wx_pages = 0,
		.start_address = 0,
		.marker = address_markers,
	};

	get_online_mems();
	mutex_lock(&cpa_mutex);
	ptdump_walk_pgd(&st.ptdump, &init_mm, NULL);
	mutex_unlock(&cpa_mutex);
	put_online_mems();
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(ptdump);
#endif /* CONFIG_PTDUMP_DEBUGFS */

/*
 * Heapsort from lib/sort.c is not a stable sorting algorithm, do a simple
 * insertion sort to preserve the original order of markers with the same
 * start address.
 */
static void sort_address_markers(void)
{
	struct addr_marker tmp;
	int i, j;

	for (i = 1; i < ARRAY_SIZE(address_markers) - 1; i++) {
		tmp = address_markers[i];
		for (j = i - 1; j >= 0 && address_markers[j].start_address > tmp.start_address; j--)
			address_markers[j + 1] = address_markers[j];
		address_markers[j + 1] = tmp;
	}
}

static int pt_dump_init(void)
{
#ifdef CONFIG_KFENCE
	unsigned long kfence_start = (unsigned long)__kfence_pool;
#endif
>>>>>>> upstream/android-13
	/*
	 * Figure out the maximum virtual address being accessible with the
	 * kernel ASCE. We need this to keep the page table walker functions
	 * from accessing non-existent entries.
	 */
	max_addr = (S390_lowcore.kernel_asce & _REGION_ENTRY_TYPE_MASK) >> 2;
	max_addr = 1UL << (max_addr * 11 + 31);
<<<<<<< HEAD
	address_markers[MODULES_NR].start_address = MODULES_VADDR;
	address_markers[VMEMMAP_NR].start_address = (unsigned long) vmemmap;
	address_markers[VMALLOC_NR].start_address = VMALLOC_START;
	debugfs_create_file("kernel_page_tables", 0400, NULL, NULL, &ptdump_fops);
=======
	address_markers[IDENTITY_AFTER_END_NR].start_address = ident_map_size;
	address_markers[MODULES_NR].start_address = MODULES_VADDR;
	address_markers[MODULES_END_NR].start_address = MODULES_END;
	address_markers[VMEMMAP_NR].start_address = (unsigned long) vmemmap;
	address_markers[VMEMMAP_END_NR].start_address = (unsigned long)vmemmap + vmemmap_size;
	address_markers[VMALLOC_NR].start_address = VMALLOC_START;
	address_markers[VMALLOC_END_NR].start_address = VMALLOC_END;
#ifdef CONFIG_KFENCE
	address_markers[KFENCE_START_NR].start_address = kfence_start;
	address_markers[KFENCE_END_NR].start_address = kfence_start + KFENCE_POOL_SIZE;
#endif
	sort_address_markers();
#ifdef CONFIG_PTDUMP_DEBUGFS
	debugfs_create_file("kernel_page_tables", 0400, NULL, NULL, &ptdump_fops);
#endif /* CONFIG_PTDUMP_DEBUGFS */
>>>>>>> upstream/android-13
	return 0;
}
device_initcall(pt_dump_init);
