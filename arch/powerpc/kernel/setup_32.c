<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Common prep/pmac/chrp boot and setup code.
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/reboot.h>
#include <linux/delay.h>
#include <linux/initrd.h>
#include <linux/tty.h>
#include <linux/seq_file.h>
#include <linux/root_dev.h>
#include <linux/cpu.h>
#include <linux/console.h>
#include <linux/memblock.h>
#include <linux/export.h>
<<<<<<< HEAD
=======
#include <linux/nvram.h>
#include <linux/pgtable.h>
>>>>>>> upstream/android-13

#include <asm/io.h>
#include <asm/prom.h>
#include <asm/processor.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/setup.h>
#include <asm/smp.h>
#include <asm/elf.h>
#include <asm/cputable.h>
#include <asm/bootx.h>
#include <asm/btext.h>
#include <asm/machdep.h>
#include <linux/uaccess.h>
#include <asm/pmac_feature.h>
#include <asm/sections.h>
#include <asm/nvram.h>
#include <asm/xmon.h>
#include <asm/time.h>
#include <asm/serial.h>
#include <asm/udbg.h>
#include <asm/code-patching.h>
#include <asm/cpu_has_feature.h>
#include <asm/asm-prototypes.h>
#include <asm/kdump.h>
#include <asm/feature-fixups.h>
<<<<<<< HEAD
=======
#include <asm/early_ioremap.h>
>>>>>>> upstream/android-13

#include "setup.h"

#define DBG(fmt...)

extern void bootx_init(unsigned long r4, unsigned long phys);

int boot_cpuid_phys;
EXPORT_SYMBOL_GPL(boot_cpuid_phys);

int smp_hw_index[NR_CPUS];
EXPORT_SYMBOL(smp_hw_index);

<<<<<<< HEAD
unsigned long ISA_DMA_THRESHOLD;
unsigned int DMA_MODE_READ;
unsigned int DMA_MODE_WRITE;

EXPORT_SYMBOL(ISA_DMA_THRESHOLD);
=======
unsigned int DMA_MODE_READ;
unsigned int DMA_MODE_WRITE;

>>>>>>> upstream/android-13
EXPORT_SYMBOL(DMA_MODE_READ);
EXPORT_SYMBOL(DMA_MODE_WRITE);

/*
<<<<<<< HEAD
 * We're called here very early in the boot.
 *
 * Note that the kernel may be running at an address which is different
 * from the address that it was linked at, so we must use RELOC/PTRRELOC
 * to access static data (including strings).  -- paulus
 */
notrace unsigned long __init early_init(unsigned long dt_ptr)
{
	unsigned long offset = reloc_offset();

	/* First zero the BSS -- use memset_io, some platforms don't have
	 * caches on yet */
	memset_io((void __iomem *)PTRRELOC(&__bss_start), 0,
			__bss_stop - __bss_start);

	/*
	 * Identify the CPU type and fix up code sections
	 * that depend on which cpu we have.
	 */
	identify_cpu(offset, mfspr(SPRN_PVR));

	apply_feature_fixups();

	return KERNELBASE + offset;
}


/*
=======
>>>>>>> upstream/android-13
 * This is run before start_kernel(), the kernel has been relocated
 * and we are running with enough of the MMU enabled to have our
 * proper kernel virtual addresses
 *
 * We do the initial parsing of the flat device-tree and prepares
 * for the MMU to be fully initialized.
 */
notrace void __init machine_init(u64 dt_ptr)
{
<<<<<<< HEAD
	unsigned int *addr = (unsigned int *)((unsigned long)&patch__memset_nocache +
					       patch__memset_nocache);
	unsigned long insn;
=======
	u32 *addr = (u32 *)patch_site_addr(&patch__memset_nocache);
	struct ppc_inst insn;
>>>>>>> upstream/android-13

	/* Configure static keys first, now that we're relocated. */
	setup_feature_keys();

<<<<<<< HEAD
	/* Enable early debugging if any specified (see udbg.h) */
	udbg_early_init();

	patch_instruction_site(&patch__memcpy_nocache, PPC_INST_NOP);

	insn = create_cond_branch(addr, branch_target(addr), 0x820000);
=======
	early_ioremap_init();

	/* Enable early debugging if any specified (see udbg.h) */
	udbg_early_init();

	patch_instruction_site(&patch__memcpy_nocache, ppc_inst(PPC_RAW_NOP()));

	create_cond_branch(&insn, addr, branch_target(addr), 0x820000);
>>>>>>> upstream/android-13
	patch_instruction(addr, insn);	/* replace b by bne cr0 */

	/* Do some early initialization based on the flat device tree */
	early_init_devtree(__va(dt_ptr));

	early_init_mmu();

	setup_kdump_trampoline();
}

/* Checks "l2cr=xxxx" command-line option */
static int __init ppc_setup_l2cr(char *str)
{
	if (cpu_has_feature(CPU_FTR_L2CR)) {
		unsigned long val = simple_strtoul(str, NULL, 0);
		printk(KERN_INFO "l2cr set to %lx\n", val);
		_set_L2CR(0);		/* force invalidate by disable cache */
		_set_L2CR(val);		/* and enable it */
	}
	return 1;
}
__setup("l2cr=", ppc_setup_l2cr);

/* Checks "l3cr=xxxx" command-line option */
static int __init ppc_setup_l3cr(char *str)
{
	if (cpu_has_feature(CPU_FTR_L3CR)) {
		unsigned long val = simple_strtoul(str, NULL, 0);
		printk(KERN_INFO "l3cr set to %lx\n", val);
		_set_L3CR(val);		/* and enable it */
	}
	return 1;
}
__setup("l3cr=", ppc_setup_l3cr);

<<<<<<< HEAD
#ifdef CONFIG_GENERIC_NVRAM

/* Generic nvram hooks used by drivers/char/gen_nvram.c */
unsigned char nvram_read_byte(int addr)
{
	if (ppc_md.nvram_read_val)
		return ppc_md.nvram_read_val(addr);
	return 0xff;
}
EXPORT_SYMBOL(nvram_read_byte);

void nvram_write_byte(unsigned char val, int addr)
{
	if (ppc_md.nvram_write_val)
		ppc_md.nvram_write_val(addr, val);
}
EXPORT_SYMBOL(nvram_write_byte);

ssize_t nvram_get_size(void)
{
	if (ppc_md.nvram_size)
		return ppc_md.nvram_size();
	return -1;
}
EXPORT_SYMBOL(nvram_get_size);

void nvram_sync(void)
{
	if (ppc_md.nvram_sync)
		ppc_md.nvram_sync();
}
EXPORT_SYMBOL(nvram_sync);

#endif /* CONFIG_NVRAM */

=======
>>>>>>> upstream/android-13
static int __init ppc_init(void)
{
	/* clear the progress line */
	if (ppc_md.progress)
		ppc_md.progress("             ", 0xffff);

	/* call platform init */
	if (ppc_md.init != NULL) {
		ppc_md.init();
	}
	return 0;
}
arch_initcall(ppc_init);

<<<<<<< HEAD
=======
static void *__init alloc_stack(void)
{
	void *ptr = memblock_alloc(THREAD_SIZE, THREAD_ALIGN);

	if (!ptr)
		panic("cannot allocate %d bytes for stack at %pS\n",
		      THREAD_SIZE, (void *)_RET_IP_);

	return ptr;
}

>>>>>>> upstream/android-13
void __init irqstack_early_init(void)
{
	unsigned int i;

<<<<<<< HEAD
	/* interrupt stacks must be in lowmem, we get that for free on ppc32
	 * as the memblock is limited to lowmem by default */
	for_each_possible_cpu(i) {
		softirq_ctx[i] = (struct thread_info *)
			__va(memblock_alloc(THREAD_SIZE, THREAD_SIZE));
		hardirq_ctx[i] = (struct thread_info *)
			__va(memblock_alloc(THREAD_SIZE, THREAD_SIZE));
	}
}

=======
	if (IS_ENABLED(CONFIG_VMAP_STACK))
		return;

	/* interrupt stacks must be in lowmem, we get that for free on ppc32
	 * as the memblock is limited to lowmem by default */
	for_each_possible_cpu(i) {
		softirq_ctx[i] = alloc_stack();
		hardirq_ctx[i] = alloc_stack();
	}
}

#ifdef CONFIG_VMAP_STACK
void *emergency_ctx[NR_CPUS] __ro_after_init = {[0] = &init_stack};

void __init emergency_stack_init(void)
{
	unsigned int i;

	for_each_possible_cpu(i)
		emergency_ctx[i] = alloc_stack();
}
#endif

>>>>>>> upstream/android-13
#if defined(CONFIG_BOOKE) || defined(CONFIG_40x)
void __init exc_lvl_early_init(void)
{
	unsigned int i, hw_cpu;

	/* interrupt stacks must be in lowmem, we get that for free on ppc32
	 * as the memblock is limited to lowmem by MEMBLOCK_REAL_LIMIT */
	for_each_possible_cpu(i) {
#ifdef CONFIG_SMP
		hw_cpu = get_hard_smp_processor_id(i);
#else
		hw_cpu = 0;
#endif

<<<<<<< HEAD
		critirq_ctx[hw_cpu] = (struct thread_info *)
			__va(memblock_alloc(THREAD_SIZE, THREAD_SIZE));
#ifdef CONFIG_BOOKE
		dbgirq_ctx[hw_cpu] = (struct thread_info *)
			__va(memblock_alloc(THREAD_SIZE, THREAD_SIZE));
		mcheckirq_ctx[hw_cpu] = (struct thread_info *)
			__va(memblock_alloc(THREAD_SIZE, THREAD_SIZE));
=======
		critirq_ctx[hw_cpu] = alloc_stack();
#ifdef CONFIG_BOOKE
		dbgirq_ctx[hw_cpu] = alloc_stack();
		mcheckirq_ctx[hw_cpu] = alloc_stack();
>>>>>>> upstream/android-13
#endif
	}
}
#endif

void __init setup_power_save(void)
{
<<<<<<< HEAD
#ifdef CONFIG_6xx
=======
#ifdef CONFIG_PPC_BOOK3S_32
>>>>>>> upstream/android-13
	if (cpu_has_feature(CPU_FTR_CAN_DOZE) ||
	    cpu_has_feature(CPU_FTR_CAN_NAP))
		ppc_md.power_save = ppc6xx_idle;
#endif

#ifdef CONFIG_E500
	if (cpu_has_feature(CPU_FTR_CAN_DOZE) ||
	    cpu_has_feature(CPU_FTR_CAN_NAP))
		ppc_md.power_save = e500_idle;
#endif
}

__init void initialize_cache_info(void)
{
	/*
	 * Set cache line size based on type of cpu as a default.
	 * Systems with OF can look in the properties on the cpu node(s)
	 * for a possibly more accurate value.
	 */
	dcache_bsize = cur_cpu_spec->dcache_bsize;
	icache_bsize = cur_cpu_spec->icache_bsize;
<<<<<<< HEAD
	ucache_bsize = 0;
	if (cpu_has_feature(CPU_FTR_UNIFIED_ID_CACHE))
		ucache_bsize = icache_bsize = dcache_bsize;
=======
>>>>>>> upstream/android-13
}
