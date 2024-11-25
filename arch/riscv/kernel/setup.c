<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2009 Sunplus Core Technology Co., Ltd.
 *  Chen Liqin <liqin.chen@sunplusct.com>
 *  Lennox Wu <lennox.wu@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 */

#include <linux/bootmem.h>
=======
 * Copyright (C) 2020 FORTH-ICS/CARV
 *  Nick Kossifidis <mick@ics.forth.gr>
 */

>>>>>>> upstream/android-13
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/initrd.h>
=======
>>>>>>> upstream/android-13
#include <linux/console.h>
#include <linux/screen_info.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/sched/task.h>
<<<<<<< HEAD
#include <linux/swiotlb.h>

#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/pgtable.h>
#include <asm/smp.h>
#include <asm/sbi.h>
#include <asm/tlbflush.h>
#include <asm/thread_info.h>

#ifdef CONFIG_EARLY_PRINTK
static void sbi_console_write(struct console *co, const char *buf,
			      unsigned int n)
{
	int i;

	for (i = 0; i < n; ++i) {
		if (buf[i] == '\n')
			sbi_console_putchar('\r');
		sbi_console_putchar(buf[i]);
	}
}

struct console riscv_sbi_early_console_dev __initdata = {
	.name	= "early",
	.write	= sbi_console_write,
	.flags	= CON_PRINTBUFFER | CON_BOOT | CON_ANYTIME,
	.index	= -1
};
#endif

#ifdef CONFIG_DUMMY_CONSOLE
struct screen_info screen_info = {
=======
#include <linux/smp.h>
#include <linux/efi.h>
#include <linux/crash_dump.h>

#include <asm/cpu_ops.h>
#include <asm/early_ioremap.h>
#include <asm/pgtable.h>
#include <asm/setup.h>
#include <asm/set_memory.h>
#include <asm/sections.h>
#include <asm/sbi.h>
#include <asm/tlbflush.h>
#include <asm/thread_info.h>
#include <asm/kasan.h>
#include <asm/efi.h>

#include "head.h"

#if defined(CONFIG_DUMMY_CONSOLE) || defined(CONFIG_EFI)
struct screen_info screen_info __section(".data") = {
>>>>>>> upstream/android-13
	.orig_video_lines	= 30,
	.orig_video_cols	= 80,
	.orig_video_mode	= 0,
	.orig_video_ega_bx	= 0,
	.orig_video_isVGA	= 1,
	.orig_video_points	= 8
};
#endif

<<<<<<< HEAD
unsigned long va_pa_offset;
EXPORT_SYMBOL(va_pa_offset);
unsigned long pfn_base;
EXPORT_SYMBOL(pfn_base);

unsigned long empty_zero_page[PAGE_SIZE / sizeof(unsigned long)] __page_aligned_bss;
EXPORT_SYMBOL(empty_zero_page);

/* The lucky hart to first increment this variable will boot the other cores */
atomic_t hart_lottery;

#ifdef CONFIG_BLK_DEV_INITRD
static void __init setup_initrd(void)
{
	unsigned long size;

	if (initrd_start >= initrd_end) {
		printk(KERN_INFO "initrd not found or empty");
		goto disable;
	}
	if (__pa(initrd_end) > PFN_PHYS(max_low_pfn)) {
		printk(KERN_ERR "initrd extends beyond end of memory");
		goto disable;
	}

	size =  initrd_end - initrd_start;
	memblock_reserve(__pa(initrd_start), size);
	initrd_below_start_ok = 1;

	printk(KERN_INFO "Initial ramdisk at: 0x%p (%lu bytes)\n",
		(void *)(initrd_start), size);
	return;
disable:
	pr_cont(" - disabling initrd\n");
	initrd_start = 0;
	initrd_end = 0;
}
#endif /* CONFIG_BLK_DEV_INITRD */

pgd_t swapper_pg_dir[PTRS_PER_PGD] __page_aligned_bss;
pgd_t trampoline_pg_dir[PTRS_PER_PGD] __initdata __aligned(PAGE_SIZE);

#ifndef __PAGETABLE_PMD_FOLDED
#define NUM_SWAPPER_PMDS ((uintptr_t)-PAGE_OFFSET >> PGDIR_SHIFT)
pmd_t swapper_pmd[PTRS_PER_PMD*((-PAGE_OFFSET)/PGDIR_SIZE)] __page_aligned_bss;
pmd_t trampoline_pmd[PTRS_PER_PGD] __initdata __aligned(PAGE_SIZE);
#endif

asmlinkage void __init setup_vm(void)
{
	extern char _start;
	uintptr_t i;
	uintptr_t pa = (uintptr_t) &_start;
	pgprot_t prot = __pgprot(pgprot_val(PAGE_KERNEL) | _PAGE_EXEC);

	va_pa_offset = PAGE_OFFSET - pa;
	pfn_base = PFN_DOWN(pa);

	/* Sanity check alignment and size */
	BUG_ON((PAGE_OFFSET % PGDIR_SIZE) != 0);
	BUG_ON((pa % (PAGE_SIZE * PTRS_PER_PTE)) != 0);

#ifndef __PAGETABLE_PMD_FOLDED
	trampoline_pg_dir[(PAGE_OFFSET >> PGDIR_SHIFT) % PTRS_PER_PGD] =
		pfn_pgd(PFN_DOWN((uintptr_t)trampoline_pmd),
			__pgprot(_PAGE_TABLE));
	trampoline_pmd[0] = pfn_pmd(PFN_DOWN(pa), prot);

	for (i = 0; i < (-PAGE_OFFSET)/PGDIR_SIZE; ++i) {
		size_t o = (PAGE_OFFSET >> PGDIR_SHIFT) % PTRS_PER_PGD + i;
		swapper_pg_dir[o] =
			pfn_pgd(PFN_DOWN((uintptr_t)swapper_pmd) + i,
				__pgprot(_PAGE_TABLE));
	}
	for (i = 0; i < ARRAY_SIZE(swapper_pmd); i++)
		swapper_pmd[i] = pfn_pmd(PFN_DOWN(pa + i * PMD_SIZE), prot);
#else
	trampoline_pg_dir[(PAGE_OFFSET >> PGDIR_SHIFT) % PTRS_PER_PGD] =
		pfn_pgd(PFN_DOWN(pa), prot);

	for (i = 0; i < (-PAGE_OFFSET)/PGDIR_SIZE; ++i) {
		size_t o = (PAGE_OFFSET >> PGDIR_SHIFT) % PTRS_PER_PGD + i;
		swapper_pg_dir[o] =
			pfn_pgd(PFN_DOWN(pa + i * PGDIR_SIZE), prot);
	}
#endif
}

void __init parse_dtb(unsigned int hartid, void *dtb)
{
	early_init_dt_scan(__va(dtb));
}

static void __init setup_bootmem(void)
{
	struct memblock_region *reg;
	phys_addr_t mem_size = 0;

	/* Find the memory region containing the kernel */
	for_each_memblock(memory, reg) {
		phys_addr_t vmlinux_end = __pa(_end);
		phys_addr_t end = reg->base + reg->size;

		if (reg->base <= vmlinux_end && vmlinux_end <= end) {
			/*
			 * Reserve from the start of the region to the end of
			 * the kernel
			 */
			memblock_reserve(reg->base, vmlinux_end - reg->base);
			mem_size = min(reg->size, (phys_addr_t)-PAGE_OFFSET);
		}
	}
	BUG_ON(mem_size == 0);

	set_max_mapnr(PFN_DOWN(mem_size));
	max_low_pfn = PFN_DOWN(memblock_end_of_DRAM());
	max_pfn = max_low_pfn;

#ifdef CONFIG_BLK_DEV_INITRD
	setup_initrd();
#endif /* CONFIG_BLK_DEV_INITRD */

	early_init_fdt_reserve_self();
	early_init_fdt_scan_reserved_mem();
	memblock_allow_resize();
	memblock_dump_all();

	for_each_memblock(memory, reg) {
		unsigned long start_pfn = memblock_region_memory_base_pfn(reg);
		unsigned long end_pfn = memblock_region_memory_end_pfn(reg);

		memblock_set_node(PFN_PHYS(start_pfn),
		                  PFN_PHYS(end_pfn - start_pfn),
		                  &memblock.memory, 0);
	}
=======
/*
 * The lucky hart to first increment this variable will boot the other cores.
 * This is used before the kernel initializes the BSS so it can't be in the
 * BSS.
 */
atomic_t hart_lottery __section(".sdata")
#ifdef CONFIG_XIP_KERNEL
= ATOMIC_INIT(0xC001BEEF)
#endif
;
unsigned long boot_cpu_hartid;
static DEFINE_PER_CPU(struct cpu, cpu_devices);

void riscv_cpuid_to_hartid_mask(const struct cpumask *in, struct cpumask *out)
{
	int cpu;

	cpumask_clear(out);
	for_each_cpu(cpu, in)
		cpumask_set_cpu(cpuid_to_hartid_map(cpu), out);
}
EXPORT_SYMBOL_GPL(riscv_cpuid_to_hartid_mask);

/*
 * Place kernel memory regions on the resource tree so that
 * kexec-tools can retrieve them from /proc/iomem. While there
 * also add "System RAM" regions for compatibility with other
 * archs, and the rest of the known regions for completeness.
 */
static struct resource kimage_res = { .name = "Kernel image", };
static struct resource code_res = { .name = "Kernel code", };
static struct resource data_res = { .name = "Kernel data", };
static struct resource rodata_res = { .name = "Kernel rodata", };
static struct resource bss_res = { .name = "Kernel bss", };
#ifdef CONFIG_CRASH_DUMP
static struct resource elfcorehdr_res = { .name = "ELF Core hdr", };
#endif

static int __init add_resource(struct resource *parent,
				struct resource *res)
{
	int ret = 0;

	ret = insert_resource(parent, res);
	if (ret < 0) {
		pr_err("Failed to add a %s resource at %llx\n",
			res->name, (unsigned long long) res->start);
		return ret;
	}

	return 1;
}

static int __init add_kernel_resources(void)
{
	int ret = 0;

	/*
	 * The memory region of the kernel image is continuous and
	 * was reserved on setup_bootmem, register it here as a
	 * resource, with the various segments of the image as
	 * child nodes.
	 */

	code_res.start = __pa_symbol(_text);
	code_res.end = __pa_symbol(_etext) - 1;
	code_res.flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;

	rodata_res.start = __pa_symbol(__start_rodata);
	rodata_res.end = __pa_symbol(__end_rodata) - 1;
	rodata_res.flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;

	data_res.start = __pa_symbol(_data);
	data_res.end = __pa_symbol(_edata) - 1;
	data_res.flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;

	bss_res.start = __pa_symbol(__bss_start);
	bss_res.end = __pa_symbol(__bss_stop) - 1;
	bss_res.flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;

	kimage_res.start = code_res.start;
	kimage_res.end = bss_res.end;
	kimage_res.flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;

	ret = add_resource(&iomem_resource, &kimage_res);
	if (ret < 0)
		return ret;

	ret = add_resource(&kimage_res, &code_res);
	if (ret < 0)
		return ret;

	ret = add_resource(&kimage_res, &rodata_res);
	if (ret < 0)
		return ret;

	ret = add_resource(&kimage_res, &data_res);
	if (ret < 0)
		return ret;

	ret = add_resource(&kimage_res, &bss_res);

	return ret;
}

static void __init init_resources(void)
{
	struct memblock_region *region = NULL;
	struct resource *res = NULL;
	struct resource *mem_res = NULL;
	size_t mem_res_sz = 0;
	int num_resources = 0, res_idx = 0;
	int ret = 0;

	/* + 1 as memblock_alloc() might increase memblock.reserved.cnt */
	num_resources = memblock.memory.cnt + memblock.reserved.cnt + 1;
	res_idx = num_resources - 1;

	mem_res_sz = num_resources * sizeof(*mem_res);
	mem_res = memblock_alloc(mem_res_sz, SMP_CACHE_BYTES);
	if (!mem_res)
		panic("%s: Failed to allocate %zu bytes\n", __func__, mem_res_sz);

	/*
	 * Start by adding the reserved regions, if they overlap
	 * with /memory regions, insert_resource later on will take
	 * care of it.
	 */
	ret = add_kernel_resources();
	if (ret < 0)
		goto error;

#ifdef CONFIG_KEXEC_CORE
	if (crashk_res.start != crashk_res.end) {
		ret = add_resource(&iomem_resource, &crashk_res);
		if (ret < 0)
			goto error;
	}
#endif

#ifdef CONFIG_CRASH_DUMP
	if (elfcorehdr_size > 0) {
		elfcorehdr_res.start = elfcorehdr_addr;
		elfcorehdr_res.end = elfcorehdr_addr + elfcorehdr_size - 1;
		elfcorehdr_res.flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;
		add_resource(&iomem_resource, &elfcorehdr_res);
	}
#endif

	for_each_reserved_mem_region(region) {
		res = &mem_res[res_idx--];

		res->name = "Reserved";
		res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;
		res->start = __pfn_to_phys(memblock_region_reserved_base_pfn(region));
		res->end = __pfn_to_phys(memblock_region_reserved_end_pfn(region)) - 1;

		/*
		 * Ignore any other reserved regions within
		 * system memory.
		 */
		if (memblock_is_memory(res->start)) {
			/* Re-use this pre-allocated resource */
			res_idx++;
			continue;
		}

		ret = add_resource(&iomem_resource, res);
		if (ret < 0)
			goto error;
	}

	/* Add /memory regions to the resource tree */
	for_each_mem_region(region) {
		res = &mem_res[res_idx--];

		if (unlikely(memblock_is_nomap(region))) {
			res->name = "Reserved";
			res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;
		} else {
			res->name = "System RAM";
			res->flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;
		}

		res->start = __pfn_to_phys(memblock_region_memory_base_pfn(region));
		res->end = __pfn_to_phys(memblock_region_memory_end_pfn(region)) - 1;

		ret = add_resource(&iomem_resource, res);
		if (ret < 0)
			goto error;
	}

	/* Clean-up any unused pre-allocated resources */
	if (res_idx >= 0)
		memblock_free(__pa(mem_res), (res_idx + 1) * sizeof(*mem_res));
	return;

 error:
	/* Better an empty resource tree than an inconsistent one */
	release_child_resources(&iomem_resource);
	memblock_free(__pa(mem_res), mem_res_sz);
}


static void __init parse_dtb(void)
{
	/* Early scan of device tree from init memory */
	if (early_init_dt_scan(dtb_early_va)) {
		const char *name = of_flat_dt_get_machine_name();

		if (name) {
			pr_info("Machine model: %s\n", name);
			dump_stack_set_arch_desc("%s (DT)", name);
		}
		return;
	}

	pr_err("No DTB passed to the kernel\n");
#ifdef CONFIG_CMDLINE_FORCE
	strscpy(boot_command_line, CONFIG_CMDLINE, COMMAND_LINE_SIZE);
	pr_info("Forcing kernel command line to: %s\n", boot_command_line);
#endif
>>>>>>> upstream/android-13
}

void __init setup_arch(char **cmdline_p)
{
<<<<<<< HEAD
#if defined(CONFIG_EARLY_PRINTK)
       if (likely(early_console == NULL)) {
               early_console = &riscv_sbi_early_console_dev;
               register_console(early_console);
       }
#endif
	*cmdline_p = boot_command_line;

	parse_early_param();

	init_mm.start_code = (unsigned long) _stext;
	init_mm.end_code   = (unsigned long) _etext;
	init_mm.end_data   = (unsigned long) _edata;
	init_mm.brk        = (unsigned long) _end;

	setup_bootmem();
	paging_init();
	unflatten_device_tree();
	swiotlb_init(1);
=======
	parse_dtb();
	setup_initial_init_mm(_stext, _etext, _edata, _end);

	*cmdline_p = boot_command_line;

	early_ioremap_setup();
	jump_label_init();
	parse_early_param();

	efi_init();
	paging_init();
#if IS_ENABLED(CONFIG_BUILTIN_DTB)
	unflatten_and_copy_device_tree();
#else
	if (early_init_dt_verify(__va(XIP_FIXUP(dtb_early_pa))))
		unflatten_device_tree();
	else
		pr_err("No DTB found in kernel mappings\n");
#endif
	misc_mem_init();

	init_resources();
	sbi_init();

#ifdef CONFIG_KASAN
	kasan_init();
#endif
>>>>>>> upstream/android-13

#ifdef CONFIG_SMP
	setup_smp();
#endif

<<<<<<< HEAD
#ifdef CONFIG_DUMMY_CONSOLE
	conswitchp = &dummy_con;
#endif

	riscv_fill_hwcap();
}

=======
	riscv_fill_hwcap();
}

static int __init topology_init(void)
{
	int i, ret;

	for_each_online_node(i)
		register_one_node(i);

	for_each_possible_cpu(i) {
		struct cpu *cpu = &per_cpu(cpu_devices, i);

		cpu->hotpluggable = cpu_has_hotplug(i);
		ret = register_cpu(cpu, i);
		if (unlikely(ret))
			pr_warn("Warning: %s: register_cpu %d failed (%d)\n",
			       __func__, i, ret);
	}

	return 0;
}
subsys_initcall(topology_init);

void free_initmem(void)
{
	if (IS_ENABLED(CONFIG_STRICT_KERNEL_RWX))
		set_kernel_memory(lm_alias(__init_begin), lm_alias(__init_end),
				  IS_ENABLED(CONFIG_64BIT) ?
					set_memory_rw : set_memory_rw_nx);

	free_initmem_default(POISON_FREE_INITMEM);
}
>>>>>>> upstream/android-13
