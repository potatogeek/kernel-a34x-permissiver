// SPDX-License-Identifier: GPL-2.0
/*
 * Dynamic function tracer architecture backend.
 *
 * Copyright IBM Corp. 2009,2014
 *
 *   Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>,
 *		Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#include <linux/moduleloader.h>
#include <linux/hardirq.h>
#include <linux/uaccess.h>
#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kprobes.h>
#include <trace/syscall.h>
#include <asm/asm-offsets.h>
#include <asm/cacheflush.h>
<<<<<<< HEAD
#include <asm/set_memory.h>
#include "entry.h"

/*
 * The mcount code looks like this:
 *	stg	%r14,8(%r15)		# offset 0
 *	larl	%r1,<&counter>		# offset 6
 *	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
 * Total length is 24 bytes. Only the first instruction will be patched
 * by ftrace_make_call / ftrace_make_nop.
 * The enabled ftrace code block looks like this:
 * >	brasl	%r0,ftrace_caller	# offset 0
 *	larl	%r1,<&counter>		# offset 6
 *	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
=======
#include <asm/ftrace.lds.h>
#include <asm/nospec-branch.h>
#include <asm/set_memory.h>
#include "entry.h"
#include "ftrace.h"

/*
 * To generate function prologue either gcc's hotpatch feature (since gcc 4.8)
 * or a combination of -pg -mrecord-mcount -mnop-mcount -mfentry flags
 * (since gcc 9 / clang 10) is used.
 * In both cases the original and also the disabled function prologue contains
 * only a single six byte instruction and looks like this:
 * >	brcl	0,0			# offset 0
 * To enable ftrace the code gets patched like above and afterwards looks
 * like this:
 * >	brasl	%r0,ftrace_caller	# offset 0
 *
 * The instruction will be patched by ftrace_make_call / ftrace_make_nop.
>>>>>>> upstream/android-13
 * The ftrace function gets called with a non-standard C function call ABI
 * where r0 contains the return address. It is also expected that the called
 * function only clobbers r0 and r1, but restores r2-r15.
 * For module code we can't directly jump to ftrace caller, but need a
 * trampoline (ftrace_plt), which clobbers also r1.
<<<<<<< HEAD
 * The return point of the ftrace function has offset 24, so execution
 * continues behind the mcount block.
 * The disabled ftrace code block looks like this:
 * >	jg	.+24			# offset 0
 *	larl	%r1,<&counter>		# offset 6
 *	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
 * The jg instruction branches to offset 24 to skip as many instructions
 * as possible.
 * In case we use gcc's hotpatch feature the original and also the disabled
 * function prologue contains only a single six byte instruction and looks
 * like this:
 * >	brcl	0,0			# offset 0
 * To enable ftrace the code gets patched like above and afterwards looks
 * like this:
 * >	brasl	%r0,ftrace_caller	# offset 0
 */

unsigned long ftrace_plt;

static inline void ftrace_generate_orig_insn(struct ftrace_insn *insn)
{
#if defined(CC_USING_HOTPATCH) || defined(CC_USING_NOP_MCOUNT)
	/* brcl 0,0 */
	insn->opc = 0xc004;
	insn->disp = 0;
#else
	/* stg r14,8(r15) */
	insn->opc = 0xe3e0;
	insn->disp = 0xf0080024;
#endif
}

static inline int is_kprobe_on_ftrace(struct ftrace_insn *insn)
{
#ifdef CONFIG_KPROBES
	if (insn->opc == BREAKPOINT_INSTRUCTION)
		return 1;
#endif
	return 0;
}

static inline void ftrace_generate_kprobe_nop_insn(struct ftrace_insn *insn)
{
#ifdef CONFIG_KPROBES
	insn->opc = BREAKPOINT_INSTRUCTION;
	insn->disp = KPROBE_ON_FTRACE_NOP;
#endif
}

static inline void ftrace_generate_kprobe_call_insn(struct ftrace_insn *insn)
{
#ifdef CONFIG_KPROBES
	insn->opc = BREAKPOINT_INSTRUCTION;
	insn->disp = KPROBE_ON_FTRACE_CALL;
#endif
}

=======
 */

void *ftrace_func __read_mostly = ftrace_stub;
struct ftrace_insn {
	u16 opc;
	s32 disp;
} __packed;

asm(
	"	.align 16\n"
	"ftrace_shared_hotpatch_trampoline_br:\n"
	"	lmg	%r0,%r1,2(%r1)\n"
	"	br	%r1\n"
	"ftrace_shared_hotpatch_trampoline_br_end:\n"
);

#ifdef CONFIG_EXPOLINE
asm(
	"	.align 16\n"
	"ftrace_shared_hotpatch_trampoline_ex:\n"
	"	lmg	%r0,%r1,2(%r1)\n"
	"	ex	%r0," __stringify(__LC_BR_R1) "(%r0)\n"
	"	j	.\n"
	"ftrace_shared_hotpatch_trampoline_ex_end:\n"
);

asm(
	"	.align 16\n"
	"ftrace_shared_hotpatch_trampoline_exrl:\n"
	"	lmg	%r0,%r1,2(%r1)\n"
	"	.insn	ril,0xc60000000000,%r0,0f\n" /* exrl */
	"	j	.\n"
	"0:	br	%r1\n"
	"ftrace_shared_hotpatch_trampoline_exrl_end:\n"
);
#endif /* CONFIG_EXPOLINE */

#ifdef CONFIG_MODULES
static char *ftrace_plt;

asm(
	"	.data\n"
	"ftrace_plt_template:\n"
	"	basr	%r1,%r0\n"
	"	lg	%r1,0f-.(%r1)\n"
	"	br	%r1\n"
	"0:	.quad	ftrace_caller\n"
	"ftrace_plt_template_end:\n"
	"	.previous\n"
);
#endif /* CONFIG_MODULES */

static const char *ftrace_shared_hotpatch_trampoline(const char **end)
{
	const char *tstart, *tend;

	tstart = ftrace_shared_hotpatch_trampoline_br;
	tend = ftrace_shared_hotpatch_trampoline_br_end;
#ifdef CONFIG_EXPOLINE
	if (!nospec_disable) {
		tstart = ftrace_shared_hotpatch_trampoline_ex;
		tend = ftrace_shared_hotpatch_trampoline_ex_end;
		if (test_facility(35)) { /* exrl */
			tstart = ftrace_shared_hotpatch_trampoline_exrl;
			tend = ftrace_shared_hotpatch_trampoline_exrl_end;
		}
	}
#endif /* CONFIG_EXPOLINE */
	if (end)
		*end = tend;
	return tstart;
}

bool ftrace_need_init_nop(void)
{
	return ftrace_shared_hotpatch_trampoline(NULL);
}

int ftrace_init_nop(struct module *mod, struct dyn_ftrace *rec)
{
	static struct ftrace_hotpatch_trampoline *next_vmlinux_trampoline =
		__ftrace_hotpatch_trampolines_start;
	static const char orig[6] = { 0xc0, 0x04, 0x00, 0x00, 0x00, 0x00 };
	static struct ftrace_hotpatch_trampoline *trampoline;
	struct ftrace_hotpatch_trampoline **next_trampoline;
	struct ftrace_hotpatch_trampoline *trampolines_end;
	struct ftrace_hotpatch_trampoline tmp;
	struct ftrace_insn *insn;
	const char *shared;
	s32 disp;

	BUILD_BUG_ON(sizeof(struct ftrace_hotpatch_trampoline) !=
		     SIZEOF_FTRACE_HOTPATCH_TRAMPOLINE);

	next_trampoline = &next_vmlinux_trampoline;
	trampolines_end = __ftrace_hotpatch_trampolines_end;
	shared = ftrace_shared_hotpatch_trampoline(NULL);
#ifdef CONFIG_MODULES
	if (mod) {
		next_trampoline = &mod->arch.next_trampoline;
		trampolines_end = mod->arch.trampolines_end;
		shared = ftrace_plt;
	}
#endif

	if (WARN_ON_ONCE(*next_trampoline >= trampolines_end))
		return -ENOMEM;
	trampoline = (*next_trampoline)++;

	/* Check for the compiler-generated fentry nop (brcl 0, .). */
	if (WARN_ON_ONCE(memcmp((const void *)rec->ip, &orig, sizeof(orig))))
		return -EINVAL;

	/* Generate the trampoline. */
	tmp.brasl_opc = 0xc015; /* brasl %r1, shared */
	tmp.brasl_disp = (shared - (const char *)&trampoline->brasl_opc) / 2;
	tmp.interceptor = FTRACE_ADDR;
	tmp.rest_of_intercepted_function = rec->ip + sizeof(struct ftrace_insn);
	s390_kernel_write(trampoline, &tmp, sizeof(tmp));

	/* Generate a jump to the trampoline. */
	disp = ((char *)trampoline - (char *)rec->ip) / 2;
	insn = (struct ftrace_insn *)rec->ip;
	s390_kernel_write(&insn->disp, &disp, sizeof(disp));

	return 0;
}

>>>>>>> upstream/android-13
int ftrace_modify_call(struct dyn_ftrace *rec, unsigned long old_addr,
		       unsigned long addr)
{
	return 0;
}

<<<<<<< HEAD
=======
static void ftrace_generate_nop_insn(struct ftrace_insn *insn)
{
	/* brcl 0,0 */
	insn->opc = 0xc004;
	insn->disp = 0;
}

static void ftrace_generate_call_insn(struct ftrace_insn *insn,
				      unsigned long ip)
{
	unsigned long target;

	/* brasl r0,ftrace_caller */
	target = FTRACE_ADDR;
#ifdef CONFIG_MODULES
	if (is_module_addr((void *)ip))
		target = (unsigned long)ftrace_plt;
#endif /* CONFIG_MODULES */
	insn->opc = 0xc005;
	insn->disp = (target - ip) / 2;
}

static void brcl_disable(void *brcl)
{
	u8 op = 0x04; /* set mask field to zero */

	s390_kernel_write((char *)brcl + 1, &op, sizeof(op));
}

>>>>>>> upstream/android-13
int ftrace_make_nop(struct module *mod, struct dyn_ftrace *rec,
		    unsigned long addr)
{
	struct ftrace_insn orig, new, old;

<<<<<<< HEAD
	if (probe_kernel_read(&old, (void *) rec->ip, sizeof(old)))
		return -EFAULT;
	if (addr == MCOUNT_ADDR) {
		/* Initial code replacement */
		ftrace_generate_orig_insn(&orig);
		ftrace_generate_nop_insn(&new);
	} else if (is_kprobe_on_ftrace(&old)) {
		/*
		 * If we find a breakpoint instruction, a kprobe has been
		 * placed at the beginning of the function. We write the
		 * constant KPROBE_ON_FTRACE_NOP into the remaining four
		 * bytes of the original instruction so that the kprobes
		 * handler can execute a nop, if it reaches this breakpoint.
		 */
		ftrace_generate_kprobe_call_insn(&orig);
		ftrace_generate_kprobe_nop_insn(&new);
	} else {
		/* Replace ftrace call with a nop. */
		ftrace_generate_call_insn(&orig, rec->ip);
		ftrace_generate_nop_insn(&new);
	}
=======
	if (ftrace_shared_hotpatch_trampoline(NULL)) {
		brcl_disable((void *)rec->ip);
		return 0;
	}

	if (copy_from_kernel_nofault(&old, (void *) rec->ip, sizeof(old)))
		return -EFAULT;
	/* Replace ftrace call with a nop. */
	ftrace_generate_call_insn(&orig, rec->ip);
	ftrace_generate_nop_insn(&new);

>>>>>>> upstream/android-13
	/* Verify that the to be replaced code matches what we expect. */
	if (memcmp(&orig, &old, sizeof(old)))
		return -EINVAL;
	s390_kernel_write((void *) rec->ip, &new, sizeof(new));
	return 0;
}

<<<<<<< HEAD
=======
static void brcl_enable(void *brcl)
{
	u8 op = 0xf4; /* set mask field to all ones */

	s390_kernel_write((char *)brcl + 1, &op, sizeof(op));
}

>>>>>>> upstream/android-13
int ftrace_make_call(struct dyn_ftrace *rec, unsigned long addr)
{
	struct ftrace_insn orig, new, old;

<<<<<<< HEAD
	if (probe_kernel_read(&old, (void *) rec->ip, sizeof(old)))
		return -EFAULT;
	if (is_kprobe_on_ftrace(&old)) {
		/*
		 * If we find a breakpoint instruction, a kprobe has been
		 * placed at the beginning of the function. We write the
		 * constant KPROBE_ON_FTRACE_CALL into the remaining four
		 * bytes of the original instruction so that the kprobes
		 * handler can execute a brasl if it reaches this breakpoint.
		 */
		ftrace_generate_kprobe_nop_insn(&orig);
		ftrace_generate_kprobe_call_insn(&new);
	} else {
		/* Replace nop with an ftrace call. */
		ftrace_generate_nop_insn(&orig);
		ftrace_generate_call_insn(&new, rec->ip);
	}
=======
	if (ftrace_shared_hotpatch_trampoline(NULL)) {
		brcl_enable((void *)rec->ip);
		return 0;
	}

	if (copy_from_kernel_nofault(&old, (void *) rec->ip, sizeof(old)))
		return -EFAULT;
	/* Replace nop with an ftrace call. */
	ftrace_generate_nop_insn(&orig);
	ftrace_generate_call_insn(&new, rec->ip);

>>>>>>> upstream/android-13
	/* Verify that the to be replaced code matches what we expect. */
	if (memcmp(&orig, &old, sizeof(old)))
		return -EINVAL;
	s390_kernel_write((void *) rec->ip, &new, sizeof(new));
	return 0;
}

int ftrace_update_ftrace_func(ftrace_func_t func)
{
<<<<<<< HEAD
=======
	ftrace_func = func;
>>>>>>> upstream/android-13
	return 0;
}

int __init ftrace_dyn_arch_init(void)
{
	return 0;
}

<<<<<<< HEAD
=======
void arch_ftrace_update_code(int command)
{
	if (ftrace_shared_hotpatch_trampoline(NULL))
		ftrace_modify_all_code(command);
	else
		ftrace_run_stop_machine(command);
}

static void __ftrace_sync(void *dummy)
{
}

int ftrace_arch_code_modify_post_process(void)
{
	if (ftrace_shared_hotpatch_trampoline(NULL)) {
		/* Send SIGP to the other CPUs, so they see the new code. */
		smp_call_function(__ftrace_sync, NULL, 1);
	}
	return 0;
}

>>>>>>> upstream/android-13
#ifdef CONFIG_MODULES

static int __init ftrace_plt_init(void)
{
<<<<<<< HEAD
	unsigned int *ip;

	ftrace_plt = (unsigned long) module_alloc(PAGE_SIZE);
	if (!ftrace_plt)
		panic("cannot allocate ftrace plt\n");
	ip = (unsigned int *) ftrace_plt;
	ip[0] = 0x0d10e310; /* basr 1,0; lg 1,10(1); br 1 */
	ip[1] = 0x100a0004;
	ip[2] = 0x07f10000;
	ip[3] = FTRACE_ADDR >> 32;
	ip[4] = FTRACE_ADDR & 0xffffffff;
	set_memory_ro(ftrace_plt, 1);
=======
	const char *start, *end;

	ftrace_plt = module_alloc(PAGE_SIZE);
	if (!ftrace_plt)
		panic("cannot allocate ftrace plt\n");

	start = ftrace_shared_hotpatch_trampoline(&end);
	if (!start) {
		start = ftrace_plt_template;
		end = ftrace_plt_template_end;
	}
	memcpy(ftrace_plt, start, end - start);
	set_memory_ro((unsigned long)ftrace_plt, 1);
>>>>>>> upstream/android-13
	return 0;
}
device_initcall(ftrace_plt_init);

#endif /* CONFIG_MODULES */

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
/*
 * Hook the return address and push it in the stack of return addresses
 * in current thread info.
 */
<<<<<<< HEAD
unsigned long prepare_ftrace_return(unsigned long parent, unsigned long ip)
=======
unsigned long prepare_ftrace_return(unsigned long ra, unsigned long sp,
				    unsigned long ip)
>>>>>>> upstream/android-13
{
	if (unlikely(ftrace_graph_is_dead()))
		goto out;
	if (unlikely(atomic_read(&current->tracing_graph_pause)))
		goto out;
	ip -= MCOUNT_INSN_SIZE;
<<<<<<< HEAD
	if (!function_graph_enter(parent, ip, 0, NULL))
		parent = (unsigned long) return_to_handler;
out:
	return parent;
=======
	if (!function_graph_enter(ra, ip, 0, (void *) sp))
		ra = (unsigned long) return_to_handler;
out:
	return ra;
>>>>>>> upstream/android-13
}
NOKPROBE_SYMBOL(prepare_ftrace_return);

/*
 * Patch the kernel code at ftrace_graph_caller location. The instruction
 * there is branch relative on condition. To enable the ftrace graph code
 * block, we simply patch the mask field of the instruction to zero and
 * turn the instruction into a nop.
 * To disable the ftrace graph code the mask field will be patched to
 * all ones, which turns the instruction into an unconditional branch.
 */
int ftrace_enable_ftrace_graph_caller(void)
{
<<<<<<< HEAD
	u8 op = 0x04; /* set mask field to zero */

	s390_kernel_write(__va(ftrace_graph_caller)+1, &op, sizeof(op));
=======
	brcl_disable(ftrace_graph_caller);
>>>>>>> upstream/android-13
	return 0;
}

int ftrace_disable_ftrace_graph_caller(void)
{
<<<<<<< HEAD
	u8 op = 0xf4; /* set mask field to all ones */

	s390_kernel_write(__va(ftrace_graph_caller)+1, &op, sizeof(op));
=======
	brcl_enable(ftrace_graph_caller);
>>>>>>> upstream/android-13
	return 0;
}

#endif /* CONFIG_FUNCTION_GRAPH_TRACER */
<<<<<<< HEAD
=======

#ifdef CONFIG_KPROBES_ON_FTRACE
void kprobe_ftrace_handler(unsigned long ip, unsigned long parent_ip,
		struct ftrace_ops *ops, struct ftrace_regs *fregs)
{
	struct kprobe_ctlblk *kcb;
	struct pt_regs *regs;
	struct kprobe *p;
	int bit;

	bit = ftrace_test_recursion_trylock(ip, parent_ip);
	if (bit < 0)
		return;

	regs = ftrace_get_regs(fregs);
	preempt_disable_notrace();
	p = get_kprobe((kprobe_opcode_t *)ip);
	if (unlikely(!p) || kprobe_disabled(p))
		goto out;

	if (kprobe_running()) {
		kprobes_inc_nmissed_count(p);
		goto out;
	}

	__this_cpu_write(current_kprobe, p);

	kcb = get_kprobe_ctlblk();
	kcb->kprobe_status = KPROBE_HIT_ACTIVE;

	instruction_pointer_set(regs, ip);

	if (!p->pre_handler || !p->pre_handler(p, regs)) {

		instruction_pointer_set(regs, ip + MCOUNT_INSN_SIZE);

		if (unlikely(p->post_handler)) {
			kcb->kprobe_status = KPROBE_HIT_SSDONE;
			p->post_handler(p, regs, 0);
		}
	}
	__this_cpu_write(current_kprobe, NULL);
out:
	preempt_enable_notrace();
	ftrace_test_recursion_unlock(bit);
}
NOKPROBE_SYMBOL(kprobe_ftrace_handler);

int arch_prepare_kprobe_ftrace(struct kprobe *p)
{
	p->ainsn.insn = NULL;
	return 0;
}
#endif
>>>>>>> upstream/android-13
