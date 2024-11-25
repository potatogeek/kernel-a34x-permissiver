<<<<<<< HEAD
/*  Paravirtualization interfaces
    Copyright (C) 2006 Rusty Russell IBM Corporation

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*  Paravirtualization interfaces
    Copyright (C) 2006 Rusty Russell IBM Corporation

>>>>>>> upstream/android-13

    2007 - x86_64 support added by Glauber de Oliveira Costa, Red Hat Inc
*/

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/efi.h>
#include <linux/bcd.h>
#include <linux/highmem.h>
#include <linux/kprobes.h>
<<<<<<< HEAD
=======
#include <linux/pgtable.h>
#include <linux/static_call.h>
>>>>>>> upstream/android-13

#include <asm/bug.h>
#include <asm/paravirt.h>
#include <asm/debugreg.h>
#include <asm/desc.h>
#include <asm/setup.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/time.h>
#include <asm/pgalloc.h>
#include <asm/irq.h>
#include <asm/delay.h>
#include <asm/fixmap.h>
#include <asm/apic.h>
#include <asm/tlbflush.h>
#include <asm/timer.h>
#include <asm/special_insns.h>
#include <asm/tlb.h>
<<<<<<< HEAD
=======
#include <asm/io_bitmap.h>
>>>>>>> upstream/android-13

/*
 * nop stub, which must not clobber anything *including the stack* to
 * avoid confusing the entry prologues.
 */
extern void _paravirt_nop(void);
asm (".pushsection .entry.text, \"ax\"\n"
     ".global _paravirt_nop\n"
     "_paravirt_nop:\n\t"
<<<<<<< HEAD
     "ret\n\t"
=======
     ASM_RET
>>>>>>> upstream/android-13
     ".size _paravirt_nop, . - _paravirt_nop\n\t"
     ".type _paravirt_nop, @function\n\t"
     ".popsection");

<<<<<<< HEAD
/* identity function, which can be inlined */
u32 notrace _paravirt_ident_32(u32 x)
{
	return x;
}

u64 notrace _paravirt_ident_64(u64 x)
{
	return x;
}

=======
>>>>>>> upstream/android-13
void __init default_banner(void)
{
	printk(KERN_INFO "Booting paravirtualized kernel on %s\n",
	       pv_info.name);
}

/* Undefined instruction for dealing with missing ops pointers. */
<<<<<<< HEAD
static const unsigned char ud2a[] = { 0x0f, 0x0b };
=======
static void paravirt_BUG(void)
{
	BUG();
}
>>>>>>> upstream/android-13

struct branch {
	unsigned char opcode;
	u32 delta;
} __attribute__((packed));

<<<<<<< HEAD
unsigned paravirt_patch_call(void *insnbuf,
			     const void *target, u16 tgt_clobbers,
			     unsigned long addr, u16 site_clobbers,
			     unsigned len)
{
	struct branch *b = insnbuf;
	unsigned long delta = (unsigned long)target - (addr+5);

	if (len < 5) {
#ifdef CONFIG_RETPOLINE
		WARN_ONCE(1, "Failing to patch indirect CALL in %ps\n", (void *)addr);
#endif
		return len;	/* call too long for patch site */
=======
static unsigned paravirt_patch_call(void *insn_buff, const void *target,
				    unsigned long addr, unsigned len)
{
	const int call_len = 5;
	struct branch *b = insn_buff;
	unsigned long delta = (unsigned long)target - (addr+call_len);

	if (len < call_len) {
		pr_warn("paravirt: Failed to patch indirect CALL at %ps\n", (void *)addr);
		/* Kernel might not be viable if patching fails, bail out: */
		BUG_ON(1);
>>>>>>> upstream/android-13
	}

	b->opcode = 0xe8; /* call */
	b->delta = delta;
<<<<<<< HEAD
	BUILD_BUG_ON(sizeof(*b) != 5);

	return 5;
}

unsigned paravirt_patch_jmp(void *insnbuf, const void *target,
			    unsigned long addr, unsigned len)
{
	struct branch *b = insnbuf;
	unsigned long delta = (unsigned long)target - (addr+5);

	if (len < 5) {
#ifdef CONFIG_RETPOLINE
		WARN_ONCE(1, "Failing to patch indirect JMP in %ps\n", (void *)addr);
#endif
		return len;	/* call too long for patch site */
	}

	b->opcode = 0xe9;	/* jmp */
	b->delta = delta;

	return 5;
}
=======
	BUILD_BUG_ON(sizeof(*b) != call_len);

	return call_len;
}

#ifdef CONFIG_PARAVIRT_XXL
/* identity function, which can be inlined */
u64 notrace _paravirt_ident_64(u64 x)
{
	return x;
}
#endif
>>>>>>> upstream/android-13

DEFINE_STATIC_KEY_TRUE(virt_spin_lock_key);

void __init native_pv_lock_init(void)
{
<<<<<<< HEAD
	if (!static_cpu_has(X86_FEATURE_HYPERVISOR))
		static_branch_disable(&virt_spin_lock_key);
}

/*
 * Neat trick to map patch type back to the call within the
 * corresponding structure.
 */
static void *get_call_destination(u8 type)
{
	struct paravirt_patch_template tmpl = {
		.pv_init_ops = pv_init_ops,
		.pv_time_ops = pv_time_ops,
		.pv_cpu_ops = pv_cpu_ops,
		.pv_irq_ops = pv_irq_ops,
		.pv_mmu_ops = pv_mmu_ops,
#ifdef CONFIG_PARAVIRT_SPINLOCKS
		.pv_lock_ops = pv_lock_ops,
#endif
	};
	return *((void **)&tmpl + type);
}

unsigned paravirt_patch_default(u8 type, u16 clobbers, void *insnbuf,
				unsigned long addr, unsigned len)
{
	void *opfunc = get_call_destination(type);
	unsigned ret;

	if (opfunc == NULL)
		/* If there's no function, patch it with a ud2a (BUG) */
		ret = paravirt_patch_insns(insnbuf, len, ud2a, ud2a+sizeof(ud2a));
	else if (opfunc == _paravirt_nop)
		ret = 0;

	/* identity functions just return their single argument */
	else if (opfunc == _paravirt_ident_32)
		ret = paravirt_patch_ident_32(insnbuf, len);
	else if (opfunc == _paravirt_ident_64)
		ret = paravirt_patch_ident_64(insnbuf, len);

	else if (type == PARAVIRT_PATCH(pv_cpu_ops.iret) ||
		 type == PARAVIRT_PATCH(pv_cpu_ops.usergs_sysret64))
		/* If operation requires a jmp, then jmp */
		ret = paravirt_patch_jmp(insnbuf, opfunc, addr, len);
	else
		/* Otherwise call the function; assume target could
		   clobber any caller-save reg */
		ret = paravirt_patch_call(insnbuf, opfunc, CLBR_ANY,
					  addr, clobbers, len);
=======
	if (!boot_cpu_has(X86_FEATURE_HYPERVISOR))
		static_branch_disable(&virt_spin_lock_key);
}

unsigned int paravirt_patch(u8 type, void *insn_buff, unsigned long addr,
			    unsigned int len)
{
	/*
	 * Neat trick to map patch type back to the call within the
	 * corresponding structure.
	 */
	void *opfunc = *((void **)&pv_ops + type);
	unsigned ret;

	if (opfunc == NULL)
		/* If there's no function, patch it with paravirt_BUG() */
		ret = paravirt_patch_call(insn_buff, paravirt_BUG, addr, len);
	else if (opfunc == _paravirt_nop)
		ret = 0;
	else
		/* Otherwise call the function. */
		ret = paravirt_patch_call(insn_buff, opfunc, addr, len);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
unsigned paravirt_patch_insns(void *insnbuf, unsigned len,
			      const char *start, const char *end)
{
	unsigned insn_len = end - start;

	if (insn_len > len || start == NULL)
		insn_len = len;
	else
		memcpy(insnbuf, start, insn_len);

	return insn_len;
}

static void native_flush_tlb(void)
{
	__native_flush_tlb();
}

/*
 * Global pages have to be flushed a bit differently. Not a real
 * performance problem because this does not happen often.
 */
static void native_flush_tlb_global(void)
{
	__native_flush_tlb_global();
}

static void native_flush_tlb_one_user(unsigned long addr)
{
	__native_flush_tlb_one_user(addr);
}

=======
>>>>>>> upstream/android-13
struct static_key paravirt_steal_enabled;
struct static_key paravirt_steal_rq_enabled;

static u64 native_steal_clock(int cpu)
{
	return 0;
}

<<<<<<< HEAD
/* These are in entry.S */
extern void native_iret(void);
extern void native_usergs_sysret64(void);
=======
DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
DEFINE_STATIC_CALL(pv_sched_clock, native_sched_clock);

void paravirt_set_sched_clock(u64 (*func)(void))
{
	static_call_update(pv_sched_clock, func);
}

/* These are in entry.S */
extern void native_iret(void);
>>>>>>> upstream/android-13

static struct resource reserve_ioports = {
	.start = 0,
	.end = IO_SPACE_LIMIT,
	.name = "paravirt-ioport",
	.flags = IORESOURCE_IO | IORESOURCE_BUSY,
};

/*
 * Reserve the whole legacy IO space to prevent any legacy drivers
 * from wasting time probing for their hardware.  This is a fairly
 * brute-force approach to disabling all non-virtual drivers.
 *
 * Note that this must be called very early to have any effect.
 */
int paravirt_disable_iospace(void)
{
	return request_resource(&ioport_resource, &reserve_ioports);
}

static DEFINE_PER_CPU(enum paravirt_lazy_mode, paravirt_lazy_mode) = PARAVIRT_LAZY_NONE;

static inline void enter_lazy(enum paravirt_lazy_mode mode)
{
	BUG_ON(this_cpu_read(paravirt_lazy_mode) != PARAVIRT_LAZY_NONE);

	this_cpu_write(paravirt_lazy_mode, mode);
}

static void leave_lazy(enum paravirt_lazy_mode mode)
{
	BUG_ON(this_cpu_read(paravirt_lazy_mode) != mode);

	this_cpu_write(paravirt_lazy_mode, PARAVIRT_LAZY_NONE);
}

void paravirt_enter_lazy_mmu(void)
{
	enter_lazy(PARAVIRT_LAZY_MMU);
}

void paravirt_leave_lazy_mmu(void)
{
	leave_lazy(PARAVIRT_LAZY_MMU);
}

void paravirt_flush_lazy_mmu(void)
{
	preempt_disable();

	if (paravirt_get_lazy_mode() == PARAVIRT_LAZY_MMU) {
		arch_leave_lazy_mmu_mode();
		arch_enter_lazy_mmu_mode();
	}

	preempt_enable();
}

<<<<<<< HEAD
=======
#ifdef CONFIG_PARAVIRT_XXL
>>>>>>> upstream/android-13
void paravirt_start_context_switch(struct task_struct *prev)
{
	BUG_ON(preemptible());

	if (this_cpu_read(paravirt_lazy_mode) == PARAVIRT_LAZY_MMU) {
		arch_leave_lazy_mmu_mode();
		set_ti_thread_flag(task_thread_info(prev), TIF_LAZY_MMU_UPDATES);
	}
	enter_lazy(PARAVIRT_LAZY_CPU);
}

void paravirt_end_context_switch(struct task_struct *next)
{
	BUG_ON(preemptible());

	leave_lazy(PARAVIRT_LAZY_CPU);

	if (test_and_clear_ti_thread_flag(task_thread_info(next), TIF_LAZY_MMU_UPDATES))
		arch_enter_lazy_mmu_mode();
}
<<<<<<< HEAD
=======
#endif
>>>>>>> upstream/android-13

enum paravirt_lazy_mode paravirt_get_lazy_mode(void)
{
	if (in_interrupt())
		return PARAVIRT_LAZY_NONE;

	return this_cpu_read(paravirt_lazy_mode);
}

struct pv_info pv_info = {
	.name = "bare hardware",
<<<<<<< HEAD
	.kernel_rpl = 0,
	.shared_kernel_pmd = 1,	/* Only used when CONFIG_X86_PAE is set */

#ifdef CONFIG_X86_64
=======
#ifdef CONFIG_PARAVIRT_XXL
>>>>>>> upstream/android-13
	.extra_user_64bit_cs = __USER_CS,
#endif
};

<<<<<<< HEAD
struct pv_init_ops pv_init_ops = {
	.patch = native_patch,
};

struct pv_time_ops pv_time_ops = {
	.sched_clock = native_sched_clock,
	.steal_clock = native_steal_clock,
};

__visible struct pv_irq_ops pv_irq_ops = {
	.save_fl = __PV_IS_CALLEE_SAVE(native_save_fl),
	.restore_fl = __PV_IS_CALLEE_SAVE(native_restore_fl),
	.irq_disable = __PV_IS_CALLEE_SAVE(native_irq_disable),
	.irq_enable = __PV_IS_CALLEE_SAVE(native_irq_enable),
	.safe_halt = native_safe_halt,
	.halt = native_halt,
};

__visible struct pv_cpu_ops pv_cpu_ops = {
	.cpuid = native_cpuid,
	.get_debugreg = native_get_debugreg,
	.set_debugreg = native_set_debugreg,
	.read_cr0 = native_read_cr0,
	.write_cr0 = native_write_cr0,
	.write_cr4 = native_write_cr4,
#ifdef CONFIG_X86_64
	.read_cr8 = native_read_cr8,
	.write_cr8 = native_write_cr8,
#endif
	.wbinvd = native_wbinvd,
	.read_msr = native_read_msr,
	.write_msr = native_write_msr,
	.read_msr_safe = native_read_msr_safe,
	.write_msr_safe = native_write_msr_safe,
	.read_pmc = native_read_pmc,
	.load_tr_desc = native_load_tr_desc,
	.set_ldt = native_set_ldt,
	.load_gdt = native_load_gdt,
	.load_idt = native_load_idt,
	.store_tr = native_store_tr,
	.load_tls = native_load_tls,
#ifdef CONFIG_X86_64
	.load_gs_index = native_load_gs_index,
#endif
	.write_ldt_entry = native_write_ldt_entry,
	.write_gdt_entry = native_write_gdt_entry,
	.write_idt_entry = native_write_idt_entry,

	.alloc_ldt = paravirt_nop,
	.free_ldt = paravirt_nop,

	.load_sp0 = native_load_sp0,

#ifdef CONFIG_X86_64
	.usergs_sysret64 = native_usergs_sysret64,
#endif
	.iret = native_iret,
	.swapgs = native_swapgs,

	.set_iopl_mask = native_set_iopl_mask,
	.io_delay = native_io_delay,

	.start_context_switch = paravirt_nop,
	.end_context_switch = paravirt_nop,
};

=======
/* 64-bit pagetable entries */
#define PTE_IDENT	__PV_IS_CALLEE_SAVE(_paravirt_ident_64)

struct paravirt_patch_template pv_ops = {
	/* Cpu ops. */
	.cpu.io_delay		= native_io_delay,

#ifdef CONFIG_PARAVIRT_XXL
	.cpu.cpuid		= native_cpuid,
	.cpu.get_debugreg	= native_get_debugreg,
	.cpu.set_debugreg	= native_set_debugreg,
	.cpu.read_cr0		= native_read_cr0,
	.cpu.write_cr0		= native_write_cr0,
	.cpu.write_cr4		= native_write_cr4,
	.cpu.wbinvd		= native_wbinvd,
	.cpu.read_msr		= native_read_msr,
	.cpu.write_msr		= native_write_msr,
	.cpu.read_msr_safe	= native_read_msr_safe,
	.cpu.write_msr_safe	= native_write_msr_safe,
	.cpu.read_pmc		= native_read_pmc,
	.cpu.load_tr_desc	= native_load_tr_desc,
	.cpu.set_ldt		= native_set_ldt,
	.cpu.load_gdt		= native_load_gdt,
	.cpu.load_idt		= native_load_idt,
	.cpu.store_tr		= native_store_tr,
	.cpu.load_tls		= native_load_tls,
	.cpu.load_gs_index	= native_load_gs_index,
	.cpu.write_ldt_entry	= native_write_ldt_entry,
	.cpu.write_gdt_entry	= native_write_gdt_entry,
	.cpu.write_idt_entry	= native_write_idt_entry,

	.cpu.alloc_ldt		= paravirt_nop,
	.cpu.free_ldt		= paravirt_nop,

	.cpu.load_sp0		= native_load_sp0,

#ifdef CONFIG_X86_IOPL_IOPERM
	.cpu.invalidate_io_bitmap	= native_tss_invalidate_io_bitmap,
	.cpu.update_io_bitmap		= native_tss_update_io_bitmap,
#endif

	.cpu.start_context_switch	= paravirt_nop,
	.cpu.end_context_switch		= paravirt_nop,

	/* Irq ops. */
	.irq.save_fl		= __PV_IS_CALLEE_SAVE(native_save_fl),
	.irq.irq_disable	= __PV_IS_CALLEE_SAVE(native_irq_disable),
	.irq.irq_enable		= __PV_IS_CALLEE_SAVE(native_irq_enable),
	.irq.safe_halt		= native_safe_halt,
	.irq.halt		= native_halt,
#endif /* CONFIG_PARAVIRT_XXL */

	/* Mmu ops. */
	.mmu.flush_tlb_user	= native_flush_tlb_local,
	.mmu.flush_tlb_kernel	= native_flush_tlb_global,
	.mmu.flush_tlb_one_user	= native_flush_tlb_one_user,
	.mmu.flush_tlb_multi	= native_flush_tlb_multi,
	.mmu.tlb_remove_table	=
			(void (*)(struct mmu_gather *, void *))tlb_remove_page,

	.mmu.exit_mmap		= paravirt_nop,

#ifdef CONFIG_PARAVIRT_XXL
	.mmu.read_cr2		= __PV_IS_CALLEE_SAVE(native_read_cr2),
	.mmu.write_cr2		= native_write_cr2,
	.mmu.read_cr3		= __native_read_cr3,
	.mmu.write_cr3		= native_write_cr3,

	.mmu.pgd_alloc		= __paravirt_pgd_alloc,
	.mmu.pgd_free		= paravirt_nop,

	.mmu.alloc_pte		= paravirt_nop,
	.mmu.alloc_pmd		= paravirt_nop,
	.mmu.alloc_pud		= paravirt_nop,
	.mmu.alloc_p4d		= paravirt_nop,
	.mmu.release_pte	= paravirt_nop,
	.mmu.release_pmd	= paravirt_nop,
	.mmu.release_pud	= paravirt_nop,
	.mmu.release_p4d	= paravirt_nop,

	.mmu.set_pte		= native_set_pte,
	.mmu.set_pmd		= native_set_pmd,

	.mmu.ptep_modify_prot_start	= __ptep_modify_prot_start,
	.mmu.ptep_modify_prot_commit	= __ptep_modify_prot_commit,

	.mmu.set_pud		= native_set_pud,

	.mmu.pmd_val		= PTE_IDENT,
	.mmu.make_pmd		= PTE_IDENT,

	.mmu.pud_val		= PTE_IDENT,
	.mmu.make_pud		= PTE_IDENT,

	.mmu.set_p4d		= native_set_p4d,

#if CONFIG_PGTABLE_LEVELS >= 5
	.mmu.p4d_val		= PTE_IDENT,
	.mmu.make_p4d		= PTE_IDENT,

	.mmu.set_pgd		= native_set_pgd,
#endif /* CONFIG_PGTABLE_LEVELS >= 5 */

	.mmu.pte_val		= PTE_IDENT,
	.mmu.pgd_val		= PTE_IDENT,

	.mmu.make_pte		= PTE_IDENT,
	.mmu.make_pgd		= PTE_IDENT,

	.mmu.dup_mmap		= paravirt_nop,
	.mmu.activate_mm	= paravirt_nop,

	.mmu.lazy_mode = {
		.enter		= paravirt_nop,
		.leave		= paravirt_nop,
		.flush		= paravirt_nop,
	},

	.mmu.set_fixmap		= native_set_fixmap,
#endif /* CONFIG_PARAVIRT_XXL */

#if defined(CONFIG_PARAVIRT_SPINLOCKS)
	/* Lock ops. */
#ifdef CONFIG_SMP
	.lock.queued_spin_lock_slowpath	= native_queued_spin_lock_slowpath,
	.lock.queued_spin_unlock	=
				PV_CALLEE_SAVE(__native_queued_spin_unlock),
	.lock.wait			= paravirt_nop,
	.lock.kick			= paravirt_nop,
	.lock.vcpu_is_preempted		=
				PV_CALLEE_SAVE(__native_vcpu_is_preempted),
#endif /* SMP */
#endif
};

#ifdef CONFIG_PARAVIRT_XXL
>>>>>>> upstream/android-13
/* At this point, native_get/set_debugreg has real function entries */
NOKPROBE_SYMBOL(native_get_debugreg);
NOKPROBE_SYMBOL(native_set_debugreg);
NOKPROBE_SYMBOL(native_load_idt);

<<<<<<< HEAD
#if defined(CONFIG_X86_32) && !defined(CONFIG_X86_PAE)
/* 32-bit pagetable entries */
#define PTE_IDENT	__PV_IS_CALLEE_SAVE(_paravirt_ident_32)
#else
/* 64-bit pagetable entries */
#define PTE_IDENT	__PV_IS_CALLEE_SAVE(_paravirt_ident_64)
#endif

struct pv_mmu_ops pv_mmu_ops __ro_after_init = {

	.read_cr2 = native_read_cr2,
	.write_cr2 = native_write_cr2,
	.read_cr3 = __native_read_cr3,
	.write_cr3 = native_write_cr3,

	.flush_tlb_user = native_flush_tlb,
	.flush_tlb_kernel = native_flush_tlb_global,
	.flush_tlb_one_user = native_flush_tlb_one_user,
	.flush_tlb_others = native_flush_tlb_others,
	.tlb_remove_table = (void (*)(struct mmu_gather *, void *))tlb_remove_page,

	.pgd_alloc = __paravirt_pgd_alloc,
	.pgd_free = paravirt_nop,

	.alloc_pte = paravirt_nop,
	.alloc_pmd = paravirt_nop,
	.alloc_pud = paravirt_nop,
	.alloc_p4d = paravirt_nop,
	.release_pte = paravirt_nop,
	.release_pmd = paravirt_nop,
	.release_pud = paravirt_nop,
	.release_p4d = paravirt_nop,

	.set_pte = native_set_pte,
	.set_pte_at = native_set_pte_at,
	.set_pmd = native_set_pmd,

	.ptep_modify_prot_start = __ptep_modify_prot_start,
	.ptep_modify_prot_commit = __ptep_modify_prot_commit,

#if CONFIG_PGTABLE_LEVELS >= 3
#ifdef CONFIG_X86_PAE
	.set_pte_atomic = native_set_pte_atomic,
	.pte_clear = native_pte_clear,
	.pmd_clear = native_pmd_clear,
#endif
	.set_pud = native_set_pud,

	.pmd_val = PTE_IDENT,
	.make_pmd = PTE_IDENT,

#if CONFIG_PGTABLE_LEVELS >= 4
	.pud_val = PTE_IDENT,
	.make_pud = PTE_IDENT,

	.set_p4d = native_set_p4d,

#if CONFIG_PGTABLE_LEVELS >= 5
	.p4d_val = PTE_IDENT,
	.make_p4d = PTE_IDENT,

	.set_pgd = native_set_pgd,
#endif /* CONFIG_PGTABLE_LEVELS >= 5 */
#endif /* CONFIG_PGTABLE_LEVELS >= 4 */
#endif /* CONFIG_PGTABLE_LEVELS >= 3 */

	.pte_val = PTE_IDENT,
	.pgd_val = PTE_IDENT,

	.make_pte = PTE_IDENT,
	.make_pgd = PTE_IDENT,

	.dup_mmap = paravirt_nop,
	.exit_mmap = paravirt_nop,
	.activate_mm = paravirt_nop,

	.lazy_mode = {
		.enter = paravirt_nop,
		.leave = paravirt_nop,
		.flush = paravirt_nop,
	},

	.set_fixmap = native_set_fixmap,
};

EXPORT_SYMBOL_GPL(pv_time_ops);
EXPORT_SYMBOL    (pv_cpu_ops);
EXPORT_SYMBOL    (pv_mmu_ops);
EXPORT_SYMBOL_GPL(pv_info);
EXPORT_SYMBOL    (pv_irq_ops);
=======
void (*paravirt_iret)(void) = native_iret;
#endif

EXPORT_SYMBOL(pv_ops);
EXPORT_SYMBOL_GPL(pv_info);
>>>>>>> upstream/android-13
