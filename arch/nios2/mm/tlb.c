/*
 * Nios2 TLB handling
 *
 * Copyright (C) 2009, Wind River Systems Inc
 *   Implemented by fredrik.markstrom@gmail.com and ivarholmqvist@gmail.com
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/pagemap.h>

#include <asm/tlb.h>
#include <asm/mmu_context.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/cpuinfo.h>

#define TLB_INDEX_MASK		\
	((((1UL << (cpuinfo.tlb_ptr_sz - cpuinfo.tlb_num_ways_log2))) - 1) \
		<< PAGE_SHIFT)

<<<<<<< HEAD
/* Used as illegal PHYS_ADDR for TLB mappings
 */
#define MAX_PHYS_ADDR 0

=======
>>>>>>> upstream/android-13
static void get_misc_and_pid(unsigned long *misc, unsigned long *pid)
{
	*misc  = RDCTL(CTL_TLBMISC);
	*misc &= (TLBMISC_PID | TLBMISC_WAY);
	*pid  = *misc & TLBMISC_PID;
}

/*
<<<<<<< HEAD
 * All entries common to a mm share an asid.  To effectively flush these
 * entries, we just bump the asid.
 */
void flush_tlb_mm(struct mm_struct *mm)
{
	if (current->mm == mm)
		flush_tlb_all();
	else
		memset(&mm->context, 0, sizeof(mm_context_t));
=======
 * This provides a PTEADDR value for addr that will cause a TLB miss
 * (fast TLB miss). TLB invalidation replaces entries with this value.
 */
static unsigned long pteaddr_invalid(unsigned long addr)
{
	return ((addr | 0xC0000000UL) >> PAGE_SHIFT) << 2;
>>>>>>> upstream/android-13
}

/*
 * This one is only used for pages with the global bit set so we don't care
 * much about the ASID.
 */
<<<<<<< HEAD
void flush_tlb_one_pid(unsigned long addr, unsigned long mmu_pid)
=======
static void replace_tlb_one_pid(unsigned long addr, unsigned long mmu_pid, unsigned long tlbacc)
>>>>>>> upstream/android-13
{
	unsigned int way;
	unsigned long org_misc, pid_misc;

<<<<<<< HEAD
	pr_debug("Flush tlb-entry for vaddr=%#lx\n", addr);

=======
>>>>>>> upstream/android-13
	/* remember pid/way until we return. */
	get_misc_and_pid(&org_misc, &pid_misc);

	WRCTL(CTL_PTEADDR, (addr >> PAGE_SHIFT) << 2);

	for (way = 0; way < cpuinfo.tlb_num_ways; way++) {
		unsigned long pteaddr;
		unsigned long tlbmisc;
		unsigned long pid;

<<<<<<< HEAD
		tlbmisc = pid_misc | TLBMISC_RD | (way << TLBMISC_WAY_SHIFT);
		WRCTL(CTL_TLBMISC, tlbmisc);
		pteaddr = RDCTL(CTL_PTEADDR);
		tlbmisc = RDCTL(CTL_TLBMISC);
		pid = (tlbmisc >> TLBMISC_PID_SHIFT) & TLBMISC_PID_MASK;
		if (((((pteaddr >> 2) & 0xfffff)) == (addr >> PAGE_SHIFT)) &&
				pid == mmu_pid) {
			unsigned long vaddr = CONFIG_NIOS2_IO_REGION_BASE +
				((PAGE_SIZE * cpuinfo.tlb_num_lines) * way) +
				(addr & TLB_INDEX_MASK);
			pr_debug("Flush entry by writing %#lx way=%dl pid=%ld\n",
				vaddr, way, (pid_misc >> TLBMISC_PID_SHIFT));

			WRCTL(CTL_PTEADDR, (vaddr >> 12) << 2);
			tlbmisc = pid_misc | TLBMISC_WE |
				(way << TLBMISC_WAY_SHIFT);
			WRCTL(CTL_TLBMISC, tlbmisc);
			WRCTL(CTL_TLBACC, (MAX_PHYS_ADDR >> PAGE_SHIFT));
		}
=======
		tlbmisc = TLBMISC_RD | (way << TLBMISC_WAY_SHIFT);
		WRCTL(CTL_TLBMISC, tlbmisc);

		pteaddr = RDCTL(CTL_PTEADDR);
		if (((pteaddr >> 2) & 0xfffff) != (addr >> PAGE_SHIFT))
			continue;

		tlbmisc = RDCTL(CTL_TLBMISC);
		pid = (tlbmisc >> TLBMISC_PID_SHIFT) & TLBMISC_PID_MASK;
		if (pid != mmu_pid)
			continue;

		tlbmisc = (mmu_pid << TLBMISC_PID_SHIFT) | TLBMISC_WE |
			  (way << TLBMISC_WAY_SHIFT);
		WRCTL(CTL_TLBMISC, tlbmisc);
		if (tlbacc == 0)
			WRCTL(CTL_PTEADDR, pteaddr_invalid(addr));
		WRCTL(CTL_TLBACC, tlbacc);
		/*
		 * There should be only a single entry that maps a
		 * particular {address,pid} so break after a match.
		 */
		break;
>>>>>>> upstream/android-13
	}

	WRCTL(CTL_TLBMISC, org_misc);
}

<<<<<<< HEAD
=======
static void flush_tlb_one_pid(unsigned long addr, unsigned long mmu_pid)
{
	pr_debug("Flush tlb-entry for vaddr=%#lx\n", addr);

	replace_tlb_one_pid(addr, mmu_pid, 0);
}

static void reload_tlb_one_pid(unsigned long addr, unsigned long mmu_pid, pte_t pte)
{
	pr_debug("Reload tlb-entry for vaddr=%#lx\n", addr);

	replace_tlb_one_pid(addr, mmu_pid, pte_val(pte));
}

>>>>>>> upstream/android-13
void flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
			unsigned long end)
{
	unsigned long mmu_pid = get_pid_from_context(&vma->vm_mm->context);

	while (start < end) {
		flush_tlb_one_pid(start, mmu_pid);
		start += PAGE_SIZE;
	}
}

<<<<<<< HEAD
void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	while (start < end) {
		flush_tlb_one(start);
		start += PAGE_SIZE;
	}
=======
void reload_tlb_page(struct vm_area_struct *vma, unsigned long addr, pte_t pte)
{
	unsigned long mmu_pid = get_pid_from_context(&vma->vm_mm->context);

	reload_tlb_one_pid(addr, mmu_pid, pte);
>>>>>>> upstream/android-13
}

/*
 * This one is only used for pages with the global bit set so we don't care
 * much about the ASID.
 */
<<<<<<< HEAD
void flush_tlb_one(unsigned long addr)
=======
static void flush_tlb_one(unsigned long addr)
>>>>>>> upstream/android-13
{
	unsigned int way;
	unsigned long org_misc, pid_misc;

	pr_debug("Flush tlb-entry for vaddr=%#lx\n", addr);

	/* remember pid/way until we return. */
	get_misc_and_pid(&org_misc, &pid_misc);

	WRCTL(CTL_PTEADDR, (addr >> PAGE_SHIFT) << 2);

	for (way = 0; way < cpuinfo.tlb_num_ways; way++) {
		unsigned long pteaddr;
		unsigned long tlbmisc;

<<<<<<< HEAD
		tlbmisc = pid_misc | TLBMISC_RD | (way << TLBMISC_WAY_SHIFT);
		WRCTL(CTL_TLBMISC, tlbmisc);
		pteaddr = RDCTL(CTL_PTEADDR);
		tlbmisc = RDCTL(CTL_TLBMISC);

		if ((((pteaddr >> 2) & 0xfffff)) == (addr >> PAGE_SHIFT)) {
			unsigned long vaddr = CONFIG_NIOS2_IO_REGION_BASE +
				((PAGE_SIZE * cpuinfo.tlb_num_lines) * way) +
				(addr & TLB_INDEX_MASK);

			pr_debug("Flush entry by writing %#lx way=%dl pid=%ld\n",
				vaddr, way, (pid_misc >> TLBMISC_PID_SHIFT));

			tlbmisc = pid_misc | TLBMISC_WE |
				(way << TLBMISC_WAY_SHIFT);
			WRCTL(CTL_PTEADDR, (vaddr >> 12) << 2);
			WRCTL(CTL_TLBMISC, tlbmisc);
			WRCTL(CTL_TLBACC, (MAX_PHYS_ADDR >> PAGE_SHIFT));
		}
=======
		tlbmisc = TLBMISC_RD | (way << TLBMISC_WAY_SHIFT);
		WRCTL(CTL_TLBMISC, tlbmisc);

		pteaddr = RDCTL(CTL_PTEADDR);
		if (((pteaddr >> 2) & 0xfffff) != (addr >> PAGE_SHIFT))
			continue;

		pr_debug("Flush entry by writing way=%dl pid=%ld\n",
			 way, (pid_misc >> TLBMISC_PID_SHIFT));

		tlbmisc = TLBMISC_WE | (way << TLBMISC_WAY_SHIFT);
		WRCTL(CTL_TLBMISC, tlbmisc);
		WRCTL(CTL_PTEADDR, pteaddr_invalid(addr));
		WRCTL(CTL_TLBACC, 0);
>>>>>>> upstream/android-13
	}

	WRCTL(CTL_TLBMISC, org_misc);
}

<<<<<<< HEAD
=======
void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	while (start < end) {
		flush_tlb_one(start);
		start += PAGE_SIZE;
	}
}

>>>>>>> upstream/android-13
void dump_tlb_line(unsigned long line)
{
	unsigned int way;
	unsigned long org_misc;

	pr_debug("dump tlb-entries for line=%#lx (addr %08lx)\n", line,
		line << (PAGE_SHIFT + cpuinfo.tlb_num_ways_log2));

	/* remember pid/way until we return */
	org_misc = (RDCTL(CTL_TLBMISC) & (TLBMISC_PID | TLBMISC_WAY));

	WRCTL(CTL_PTEADDR, line << 2);

	for (way = 0; way < cpuinfo.tlb_num_ways; way++) {
		unsigned long pteaddr;
		unsigned long tlbmisc;
		unsigned long tlbacc;

		WRCTL(CTL_TLBMISC, TLBMISC_RD | (way << TLBMISC_WAY_SHIFT));
		pteaddr = RDCTL(CTL_PTEADDR);
		tlbmisc = RDCTL(CTL_TLBMISC);
		tlbacc = RDCTL(CTL_TLBACC);

<<<<<<< HEAD
		if ((tlbacc << PAGE_SHIFT) != (MAX_PHYS_ADDR & PAGE_MASK)) {
=======
		if ((tlbacc << PAGE_SHIFT) != 0) {
>>>>>>> upstream/android-13
			pr_debug("-- way:%02x vpn:0x%08lx phys:0x%08lx pid:0x%02lx flags:%c%c%c%c%c\n",
				way,
				(pteaddr << (PAGE_SHIFT-2)),
				(tlbacc << PAGE_SHIFT),
				((tlbmisc >> TLBMISC_PID_SHIFT) &
				TLBMISC_PID_MASK),
				(tlbacc & _PAGE_READ ? 'r' : '-'),
				(tlbacc & _PAGE_WRITE ? 'w' : '-'),
				(tlbacc & _PAGE_EXEC ? 'x' : '-'),
				(tlbacc & _PAGE_GLOBAL ? 'g' : '-'),
				(tlbacc & _PAGE_CACHED ? 'c' : '-'));
		}
	}

	WRCTL(CTL_TLBMISC, org_misc);
}

void dump_tlb(void)
{
	unsigned int i;

	for (i = 0; i < cpuinfo.tlb_num_lines; i++)
		dump_tlb_line(i);
}

<<<<<<< HEAD
void flush_tlb_pid(unsigned long pid)
{
=======
void flush_tlb_pid(unsigned long mmu_pid)
{
	unsigned long addr = 0;
>>>>>>> upstream/android-13
	unsigned int line;
	unsigned int way;
	unsigned long org_misc, pid_misc;

	/* remember pid/way until we return */
	get_misc_and_pid(&org_misc, &pid_misc);

	for (line = 0; line < cpuinfo.tlb_num_lines; line++) {
<<<<<<< HEAD
		WRCTL(CTL_PTEADDR, line << 2);

		for (way = 0; way < cpuinfo.tlb_num_ways; way++) {
			unsigned long pteaddr;
			unsigned long tlbmisc;
			unsigned long tlbacc;

			tlbmisc = pid_misc | TLBMISC_RD |
				(way << TLBMISC_WAY_SHIFT);
			WRCTL(CTL_TLBMISC, tlbmisc);
			pteaddr = RDCTL(CTL_PTEADDR);
			tlbmisc = RDCTL(CTL_TLBMISC);
			tlbacc = RDCTL(CTL_TLBACC);

			if (((tlbmisc>>TLBMISC_PID_SHIFT) & TLBMISC_PID_MASK)
				== pid) {
				tlbmisc = pid_misc | TLBMISC_WE |
					(way << TLBMISC_WAY_SHIFT);
				WRCTL(CTL_TLBMISC, tlbmisc);
				WRCTL(CTL_TLBACC,
					(MAX_PHYS_ADDR >> PAGE_SHIFT));
			}
		}

		WRCTL(CTL_TLBMISC, org_misc);
=======
		WRCTL(CTL_PTEADDR, pteaddr_invalid(addr));

		for (way = 0; way < cpuinfo.tlb_num_ways; way++) {
			unsigned long tlbmisc;
			unsigned long pid;

			tlbmisc = TLBMISC_RD | (way << TLBMISC_WAY_SHIFT);
			WRCTL(CTL_TLBMISC, tlbmisc);
			tlbmisc = RDCTL(CTL_TLBMISC);
			pid = (tlbmisc >> TLBMISC_PID_SHIFT) & TLBMISC_PID_MASK;
			if (pid != mmu_pid)
				continue;

			tlbmisc = TLBMISC_WE | (way << TLBMISC_WAY_SHIFT);
			WRCTL(CTL_TLBMISC, tlbmisc);
			WRCTL(CTL_TLBACC, 0);
		}

		addr += PAGE_SIZE;
	}

	WRCTL(CTL_TLBMISC, org_misc);
}

/*
 * All entries common to a mm share an asid.  To effectively flush these
 * entries, we just bump the asid.
 */
void flush_tlb_mm(struct mm_struct *mm)
{
	if (current->mm == mm) {
		unsigned long mmu_pid = get_pid_from_context(&mm->context);
		flush_tlb_pid(mmu_pid);
	} else {
		memset(&mm->context, 0, sizeof(mm_context_t));
>>>>>>> upstream/android-13
	}
}

void flush_tlb_all(void)
{
<<<<<<< HEAD
	int i;
	unsigned long vaddr = CONFIG_NIOS2_IO_REGION_BASE;
	unsigned int way;
	unsigned long org_misc, pid_misc, tlbmisc;

	/* remember pid/way until we return */
	get_misc_and_pid(&org_misc, &pid_misc);
	pid_misc |= TLBMISC_WE;

	/* Map each TLB entry to physcal address 0 with no-access and a
	   bad ptbase */
	for (way = 0; way < cpuinfo.tlb_num_ways; way++) {
		tlbmisc = pid_misc | (way << TLBMISC_WAY_SHIFT);
		for (i = 0; i < cpuinfo.tlb_num_lines; i++) {
			WRCTL(CTL_PTEADDR, ((vaddr) >> PAGE_SHIFT) << 2);
			WRCTL(CTL_TLBMISC, tlbmisc);
			WRCTL(CTL_TLBACC, (MAX_PHYS_ADDR >> PAGE_SHIFT));
			vaddr += 1UL << 12;
		}
=======
	unsigned long addr = 0;
	unsigned int line;
	unsigned int way;
	unsigned long org_misc, pid_misc;

	/* remember pid/way until we return */
	get_misc_and_pid(&org_misc, &pid_misc);

	/* Start at way 0, way is auto-incremented after each TLBACC write */
	WRCTL(CTL_TLBMISC, TLBMISC_WE);

	/* Map each TLB entry to physcal address 0 with no-access and a
	   bad ptbase */
	for (line = 0; line < cpuinfo.tlb_num_lines; line++) {
		WRCTL(CTL_PTEADDR, pteaddr_invalid(addr));
		for (way = 0; way < cpuinfo.tlb_num_ways; way++)
			WRCTL(CTL_TLBACC, 0);

		addr += PAGE_SIZE;
>>>>>>> upstream/android-13
	}

	/* restore pid/way */
	WRCTL(CTL_TLBMISC, org_misc);
}

void set_mmu_pid(unsigned long pid)
{
<<<<<<< HEAD
	WRCTL(CTL_TLBMISC, (RDCTL(CTL_TLBMISC) & TLBMISC_WAY) |
		((pid & TLBMISC_PID_MASK) << TLBMISC_PID_SHIFT));
=======
	unsigned long tlbmisc;

	tlbmisc = RDCTL(CTL_TLBMISC);
	tlbmisc = (tlbmisc & TLBMISC_WAY);
	tlbmisc |= (pid & TLBMISC_PID_MASK) << TLBMISC_PID_SHIFT;
	WRCTL(CTL_TLBMISC, tlbmisc);
>>>>>>> upstream/android-13
}
