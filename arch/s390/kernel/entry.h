/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ENTRY_H
#define _ENTRY_H

#include <linux/percpu.h>
#include <linux/types.h>
#include <linux/signal.h>
#include <asm/ptrace.h>
#include <asm/idle.h>

extern void *restart_stack;
<<<<<<< HEAD
extern unsigned long suspend_zero_pages;
=======
>>>>>>> upstream/android-13

void system_call(void);
void pgm_check_handler(void);
void ext_int_handler(void);
void io_int_handler(void);
void mcck_int_handler(void);
void restart_int_handler(void);
<<<<<<< HEAD
void restart_call_handler(void);

asmlinkage long do_syscall_trace_enter(struct pt_regs *regs);
asmlinkage void do_syscall_trace_exit(struct pt_regs *regs);

void do_protection_exception(struct pt_regs *regs);
void do_dat_exception(struct pt_regs *regs);

void addressing_exception(struct pt_regs *regs);
void data_exception(struct pt_regs *regs);
void default_trap_handler(struct pt_regs *regs);
void divide_exception(struct pt_regs *regs);
void execute_exception(struct pt_regs *regs);
void hfp_divide_exception(struct pt_regs *regs);
void hfp_overflow_exception(struct pt_regs *regs);
void hfp_significance_exception(struct pt_regs *regs);
void hfp_sqrt_exception(struct pt_regs *regs);
void hfp_underflow_exception(struct pt_regs *regs);
void illegal_op(struct pt_regs *regs);
void operand_exception(struct pt_regs *regs);
void overflow_exception(struct pt_regs *regs);
void privileged_op(struct pt_regs *regs);
void space_switch_exception(struct pt_regs *regs);
void special_op_exception(struct pt_regs *regs);
void specification_exception(struct pt_regs *regs);
void transaction_exception(struct pt_regs *regs);
void translation_exception(struct pt_regs *regs);
void vector_exception(struct pt_regs *regs);

void do_per_trap(struct pt_regs *regs);
void do_report_trap(struct pt_regs *regs, int si_signo, int si_code, char *str);
void syscall_trace(struct pt_regs *regs, int entryexit);
void kernel_stack_overflow(struct pt_regs * regs);
void do_signal(struct pt_regs *regs);
void handle_signal32(struct ksignal *ksig, sigset_t *oldset,
		     struct pt_regs *regs);
void do_notify_resume(struct pt_regs *regs);

void __init init_IRQ(void);
void do_IRQ(struct pt_regs *regs, int irq);
void do_restart(void);
void __init startup_init_nobss(void);
=======

void __ret_from_fork(struct task_struct *prev, struct pt_regs *regs);
void __do_pgm_check(struct pt_regs *regs);
void __do_syscall(struct pt_regs *regs, int per_trap);

void do_protection_exception(struct pt_regs *regs);
void do_dat_exception(struct pt_regs *regs);
void do_secure_storage_access(struct pt_regs *regs);
void do_non_secure_storage_access(struct pt_regs *regs);
void do_secure_storage_violation(struct pt_regs *regs);
void do_report_trap(struct pt_regs *regs, int si_signo, int si_code, char *str);
void kernel_stack_overflow(struct pt_regs * regs);
void handle_signal32(struct ksignal *ksig, sigset_t *oldset,
		     struct pt_regs *regs);

void __init init_IRQ(void);
void do_io_irq(struct pt_regs *regs);
void do_ext_irq(struct pt_regs *regs);
void do_restart(void *arg);
>>>>>>> upstream/android-13
void __init startup_init(void);
void die(struct pt_regs *regs, const char *str);
int setup_profiling_timer(unsigned int multiplier);
void __init time_init(void);
<<<<<<< HEAD
int pfn_is_nosave(unsigned long);
void s390_early_resume(void);
unsigned long prepare_ftrace_return(unsigned long parent, unsigned long ip);
=======
unsigned long prepare_ftrace_return(unsigned long parent, unsigned long sp, unsigned long ip);
>>>>>>> upstream/android-13

struct s390_mmap_arg_struct;
struct fadvise64_64_args;
struct old_sigaction;

long sys_rt_sigreturn(void);
long sys_sigreturn(void);

long sys_s390_personality(unsigned int personality);
long sys_s390_runtime_instr(int command, int signum);
long sys_s390_guarded_storage(int command, struct gs_cb __user *);
long sys_s390_pci_mmio_write(unsigned long, const void __user *, size_t);
long sys_s390_pci_mmio_read(unsigned long, void __user *, size_t);
long sys_s390_sthyi(unsigned long function_code, void __user *buffer, u64 __user *return_code, unsigned long flags);

DECLARE_PER_CPU(u64, mt_cycles[8]);

<<<<<<< HEAD
void gs_load_bc_cb(struct pt_regs *regs);
void set_fs_fixup(void);
=======
unsigned long stack_alloc(void);
void stack_free(unsigned long stack);

extern char kprobes_insn_page[];

extern char _samode31[], _eamode31[];
extern char _stext_amode31[], _etext_amode31[];
extern struct exception_table_entry _start_amode31_ex_table[];
extern struct exception_table_entry _stop_amode31_ex_table[];

#define __amode31_data __section(".amode31.data")
#define __amode31_ref __section(".amode31.refs")
extern long _start_amode31_refs[], _end_amode31_refs[];
>>>>>>> upstream/android-13

#endif /* _ENTRY_H */
