// SPDX-License-Identifier: GPL-2.0
/*
 * Provide a default dump_stack() function for architectures
 * which don't implement their own.
 */

#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/buildid.h>
>>>>>>> upstream/android-13
#include <linux/export.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/smp.h>
#include <linux/atomic.h>
#include <linux/kexec.h>
#include <linux/utsname.h>
<<<<<<< HEAD
=======
#include <linux/stop_machine.h>
>>>>>>> upstream/android-13

static char dump_stack_arch_desc_str[128];

/**
 * dump_stack_set_arch_desc - set arch-specific str to show with task dumps
 * @fmt: printf-style format string
 * @...: arguments for the format string
 *
 * The configured string will be printed right after utsname during task
 * dumps.  Usually used to add arch-specific system identifiers.  If an
 * arch wants to make use of such an ID string, it should initialize this
 * as soon as possible during boot.
 */
void __init dump_stack_set_arch_desc(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(dump_stack_arch_desc_str, sizeof(dump_stack_arch_desc_str),
		  fmt, args);
	va_end(args);
}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_STACKTRACE_BUILD_ID)
#define BUILD_ID_FMT " %20phN"
#define BUILD_ID_VAL vmlinux_build_id
#else
#define BUILD_ID_FMT "%s"
#define BUILD_ID_VAL ""
#endif

>>>>>>> upstream/android-13
/**
 * dump_stack_print_info - print generic debug info for dump_stack()
 * @log_lvl: log level
 *
 * Arch-specific dump_stack() implementations can use this function to
 * print out the same debug information as the generic dump_stack().
 */
void dump_stack_print_info(const char *log_lvl)
{
<<<<<<< HEAD
	printk("%sCPU: %d PID: %d Comm: %.20s %s%s %s %.*s\n",
=======
	printk("%sCPU: %d PID: %d Comm: %.20s %s%s %s %.*s" BUILD_ID_FMT "\n",
>>>>>>> upstream/android-13
	       log_lvl, raw_smp_processor_id(), current->pid, current->comm,
	       kexec_crash_loaded() ? "Kdump: loaded " : "",
	       print_tainted(),
	       init_utsname()->release,
	       (int)strcspn(init_utsname()->version, " "),
<<<<<<< HEAD
	       init_utsname()->version);
=======
	       init_utsname()->version, BUILD_ID_VAL);
>>>>>>> upstream/android-13

	if (dump_stack_arch_desc_str[0] != '\0')
		printk("%sHardware name: %s\n",
		       log_lvl, dump_stack_arch_desc_str);

	print_worker_info(log_lvl, current);
<<<<<<< HEAD
=======
	print_stop_info(log_lvl, current);
>>>>>>> upstream/android-13
}

/**
 * show_regs_print_info - print generic debug info for show_regs()
 * @log_lvl: log level
 *
 * show_regs() implementations can use this function to print out generic
 * debug information.
 */
void show_regs_print_info(const char *log_lvl)
{
	dump_stack_print_info(log_lvl);
}

<<<<<<< HEAD
static void __dump_stack(void)
{
	dump_stack_print_info(KERN_DEFAULT);
	show_stack(NULL, NULL);
}

/**
 * dump_stack - dump the current task information and its stack trace
 *
 * Architectures can override this implementation by implementing its own.
 */
#ifdef CONFIG_SMP
static atomic_t dump_lock = ATOMIC_INIT(-1);

asmlinkage __visible void dump_stack(void)
{
	unsigned long flags;
	int was_locked;
	int old;
	int cpu;
=======
static void __dump_stack(const char *log_lvl)
{
	dump_stack_print_info(log_lvl);
	show_stack(NULL, NULL, log_lvl);
}

/**
 * dump_stack_lvl - dump the current task information and its stack trace
 * @log_lvl: log level
 *
 * Architectures can override this implementation by implementing its own.
 */
asmlinkage __visible void dump_stack_lvl(const char *log_lvl)
{
	unsigned long flags;
>>>>>>> upstream/android-13

	/*
	 * Permit this cpu to perform nested stack dumps while serialising
	 * against other CPUs
	 */
<<<<<<< HEAD
retry:
	local_irq_save(flags);
	cpu = smp_processor_id();
	old = atomic_cmpxchg(&dump_lock, -1, cpu);
	if (old == -1) {
		was_locked = 0;
	} else if (old == cpu) {
		was_locked = 1;
	} else {
		local_irq_restore(flags);
		/*
		 * Wait for the lock to release before jumping to
		 * atomic_cmpxchg() in order to mitigate the thundering herd
		 * problem.
		 */
		do { cpu_relax(); } while (atomic_read(&dump_lock) != -1);
		goto retry;
	}

	__dump_stack();

	if (!was_locked)
		atomic_set(&dump_lock, -1);

	local_irq_restore(flags);
}
#else
asmlinkage __visible void dump_stack(void)
{
	__dump_stack();
}
#endif
=======
	printk_cpu_lock_irqsave(flags);
	__dump_stack(log_lvl);
	printk_cpu_unlock_irqrestore(flags);
}
EXPORT_SYMBOL(dump_stack_lvl);

asmlinkage __visible void dump_stack(void)
{
	dump_stack_lvl(KERN_DEFAULT);
}
>>>>>>> upstream/android-13
EXPORT_SYMBOL(dump_stack);
