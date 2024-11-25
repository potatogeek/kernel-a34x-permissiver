/*
 * Kernel Debugger Architecture Independent Stack Traceback
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1999-2004 Silicon Graphics, Inc.  All Rights Reserved.
 * Copyright (c) 2009 Wind River Systems, Inc.  All Rights Reserved.
 */

#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/sched/debug.h>
#include <linux/kdb.h>
#include <linux/nmi.h>
#include "kdb_private.h"


static void kdb_show_stack(struct task_struct *p, void *addr)
{
<<<<<<< HEAD
	int old_lvl = console_loglevel;
	console_loglevel = CONSOLE_LOGLEVEL_MOTORMOUTH;
	kdb_trap_printk++;
	kdb_set_current_task(p);
	if (addr) {
		show_stack((struct task_struct *)p, addr);
	} else if (kdb_current_regs) {
#ifdef CONFIG_X86
		show_stack(p, &kdb_current_regs->sp);
#else
		show_stack(p, NULL);
#endif
	} else {
		show_stack(p, NULL);
	}
	console_loglevel = old_lvl;
=======
	kdb_trap_printk++;

	if (!addr && kdb_task_has_cpu(p)) {
		int old_lvl = console_loglevel;

		console_loglevel = CONSOLE_LOGLEVEL_MOTORMOUTH;
		kdb_dump_stack_on_cpu(kdb_process_cpu(p));
		console_loglevel = old_lvl;
	} else {
		show_stack(p, addr, KERN_EMERG);
	}

>>>>>>> upstream/android-13
	kdb_trap_printk--;
}

/*
 * kdb_bt
 *
 *	This function implements the 'bt' command.  Print a stack
 *	traceback.
 *
 *	bt [<address-expression>]	(addr-exp is for alternate stacks)
 *	btp <pid>			Kernel stack for <pid>
 *	btt <address-expression>	Kernel stack for task structure at
 *					<address-expression>
<<<<<<< HEAD
 *	bta [DRSTCZEUIMA]		All useful processes, optionally
=======
 *	bta [state_chars>|A]		All useful processes, optionally
>>>>>>> upstream/android-13
 *					filtered by state
 *	btc [<cpu>]			The current process on one cpu,
 *					default is all cpus
 *
 *	bt <address-expression> refers to a address on the stack, that location
 *	is assumed to contain a return address.
 *
 *	btt <address-expression> refers to the address of a struct task.
 *
 * Inputs:
 *	argc	argument count
 *	argv	argument vector
 * Outputs:
 *	None.
 * Returns:
 *	zero for success, a kdb diagnostic if error
 * Locking:
 *	none.
 * Remarks:
 *	Backtrack works best when the code uses frame pointers.  But even
 *	without frame pointers we should get a reasonable trace.
 *
 *	mds comes in handy when examining the stack to do a manual traceback or
 *	to get a starting point for bt <address-expression>.
 */

static int
<<<<<<< HEAD
kdb_bt1(struct task_struct *p, unsigned long mask,
	int argcount, int btaprompt)
{
	char buffer[2];
	if (kdb_getarea(buffer[0], (unsigned long)p) ||
	    kdb_getarea(buffer[0], (unsigned long)(p+1)-1))
=======
kdb_bt1(struct task_struct *p, const char *mask, bool btaprompt)
{
	char ch;

	if (kdb_getarea(ch, (unsigned long)p) ||
	    kdb_getarea(ch, (unsigned long)(p+1)-1))
>>>>>>> upstream/android-13
		return KDB_BADADDR;
	if (!kdb_task_state(p, mask))
		return 0;
	kdb_printf("Stack traceback for pid %d\n", p->pid);
	kdb_ps1(p);
	kdb_show_stack(p, NULL);
	if (btaprompt) {
<<<<<<< HEAD
		kdb_getstr(buffer, sizeof(buffer),
			   "Enter <q> to end, <cr> to continue:");
		if (buffer[0] == 'q') {
			kdb_printf("\n");
			return 1;
		}
=======
		kdb_printf("Enter <q> to end, <cr> or <space> to continue:");
		do {
			ch = kdb_getchar();
		} while (!strchr("\r\n q", ch));
		kdb_printf("\n");

		/* reset the pager */
		kdb_nextline = 1;

		if (ch == 'q')
			return 1;
>>>>>>> upstream/android-13
	}
	touch_nmi_watchdog();
	return 0;
}

<<<<<<< HEAD
=======
static void
kdb_bt_cpu(unsigned long cpu)
{
	struct task_struct *kdb_tsk;

	if (cpu >= num_possible_cpus() || !cpu_online(cpu)) {
		kdb_printf("WARNING: no process for cpu %ld\n", cpu);
		return;
	}

	/* If a CPU failed to round up we could be here */
	kdb_tsk = KDB_TSK(cpu);
	if (!kdb_tsk) {
		kdb_printf("WARNING: no task for cpu %ld\n", cpu);
		return;
	}

	kdb_bt1(kdb_tsk, "A", false);
}

>>>>>>> upstream/android-13
int
kdb_bt(int argc, const char **argv)
{
	int diag;
<<<<<<< HEAD
	int argcount = 5;
=======
>>>>>>> upstream/android-13
	int btaprompt = 1;
	int nextarg;
	unsigned long addr;
	long offset;

	/* Prompt after each proc in bta */
	kdbgetintenv("BTAPROMPT", &btaprompt);

	if (strcmp(argv[0], "bta") == 0) {
		struct task_struct *g, *p;
		unsigned long cpu;
<<<<<<< HEAD
		unsigned long mask = kdb_task_state_string(argc ? argv[1] :
							   NULL);
=======
		const char *mask = argc ? argv[1] : kdbgetenv("PS");

>>>>>>> upstream/android-13
		if (argc == 0)
			kdb_ps_suppressed();
		/* Run the active tasks first */
		for_each_online_cpu(cpu) {
			p = kdb_curr_task(cpu);
<<<<<<< HEAD
			if (kdb_bt1(p, mask, argcount, btaprompt))
				return 0;
		}
		/* Now the inactive tasks */
		kdb_do_each_thread(g, p) {
=======
			if (kdb_bt1(p, mask, btaprompt))
				return 0;
		}
		/* Now the inactive tasks */
		for_each_process_thread(g, p) {
>>>>>>> upstream/android-13
			if (KDB_FLAG(CMD_INTERRUPT))
				return 0;
			if (task_curr(p))
				continue;
<<<<<<< HEAD
			if (kdb_bt1(p, mask, argcount, btaprompt))
				return 0;
		} kdb_while_each_thread(g, p);
=======
			if (kdb_bt1(p, mask, btaprompt))
				return 0;
		}
>>>>>>> upstream/android-13
	} else if (strcmp(argv[0], "btp") == 0) {
		struct task_struct *p;
		unsigned long pid;
		if (argc != 1)
			return KDB_ARGCOUNT;
		diag = kdbgetularg((char *)argv[1], &pid);
		if (diag)
			return diag;
		p = find_task_by_pid_ns(pid, &init_pid_ns);
<<<<<<< HEAD
		if (p) {
			kdb_set_current_task(p);
			return kdb_bt1(p, ~0UL, argcount, 0);
		}
=======
		if (p)
			return kdb_bt1(p, "A", false);
>>>>>>> upstream/android-13
		kdb_printf("No process with pid == %ld found\n", pid);
		return 0;
	} else if (strcmp(argv[0], "btt") == 0) {
		if (argc != 1)
			return KDB_ARGCOUNT;
		diag = kdbgetularg((char *)argv[1], &addr);
		if (diag)
			return diag;
<<<<<<< HEAD
		kdb_set_current_task((struct task_struct *)addr);
		return kdb_bt1((struct task_struct *)addr, ~0UL, argcount, 0);
	} else if (strcmp(argv[0], "btc") == 0) {
		unsigned long cpu = ~0;
		struct task_struct *save_current_task = kdb_current_task;
		char buf[80];
=======
		return kdb_bt1((struct task_struct *)addr, "A", false);
	} else if (strcmp(argv[0], "btc") == 0) {
		unsigned long cpu = ~0;
>>>>>>> upstream/android-13
		if (argc > 1)
			return KDB_ARGCOUNT;
		if (argc == 1) {
			diag = kdbgetularg((char *)argv[1], &cpu);
			if (diag)
				return diag;
		}
<<<<<<< HEAD
		/* Recursive use of kdb_parse, do not use argv after
		 * this point */
		argv = NULL;
		if (cpu != ~0) {
			if (cpu >= num_possible_cpus() || !cpu_online(cpu)) {
				kdb_printf("no process for cpu %ld\n", cpu);
				return 0;
			}
			sprintf(buf, "btt 0x%px\n", KDB_TSK(cpu));
			kdb_parse(buf);
			return 0;
		}
		kdb_printf("btc: cpu status: ");
		kdb_parse("cpu\n");
		for_each_online_cpu(cpu) {
			void *kdb_tsk = KDB_TSK(cpu);

			/* If a CPU failed to round up we could be here */
			if (!kdb_tsk) {
				kdb_printf("WARNING: no task for cpu %ld\n",
					   cpu);
				continue;
			}

			sprintf(buf, "btt 0x%px\n", kdb_tsk);
			kdb_parse(buf);
			touch_nmi_watchdog();
		}
		kdb_set_current_task(save_current_task);
=======
		if (cpu != ~0) {
			kdb_bt_cpu(cpu);
		} else {
			/*
			 * Recursive use of kdb_parse, do not use argv after
			 * this point.
			 */
			argv = NULL;
			kdb_printf("btc: cpu status: ");
			kdb_parse("cpu\n");
			for_each_online_cpu(cpu) {
				kdb_bt_cpu(cpu);
				touch_nmi_watchdog();
			}
		}
>>>>>>> upstream/android-13
		return 0;
	} else {
		if (argc) {
			nextarg = 1;
			diag = kdbgetaddrarg(argc, argv, &nextarg, &addr,
					     &offset, NULL);
			if (diag)
				return diag;
			kdb_show_stack(kdb_current_task, (void *)addr);
			return 0;
		} else {
<<<<<<< HEAD
			return kdb_bt1(kdb_current_task, ~0UL, argcount, 0);
=======
			return kdb_bt1(kdb_current_task, "A", false);
>>>>>>> upstream/android-13
		}
	}

	/* NOTREACHED */
	return 0;
}
