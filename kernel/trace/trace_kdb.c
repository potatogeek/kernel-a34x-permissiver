// SPDX-License-Identifier: GPL-2.0
/*
 * kdb helper for dumping the ftrace buffer
 *
 * Copyright (C) 2010 Jason Wessel <jason.wessel@windriver.com>
 *
 * ftrace_dump_buf based on ftrace_dump:
 * Copyright (C) 2007-2008 Steven Rostedt <srostedt@redhat.com>
 * Copyright (C) 2008 Ingo Molnar <mingo@redhat.com>
 *
 */
#include <linux/init.h>
#include <linux/kgdb.h>
#include <linux/kdb.h>
#include <linux/ftrace.h>

#include "trace.h"
#include "trace_output.h"

<<<<<<< HEAD
static void ftrace_dump_buf(int skip_lines, long cpu_file)
{
	/* use static because iter can be a bit big for the stack */
	static struct trace_iterator iter;
	static struct ring_buffer_iter *buffer_iter[CONFIG_NR_CPUS];
=======
static struct trace_iterator iter;
static struct ring_buffer_iter *buffer_iter[CONFIG_NR_CPUS];

static void ftrace_dump_buf(int skip_entries, long cpu_file)
{
>>>>>>> upstream/android-13
	struct trace_array *tr;
	unsigned int old_userobj;
	int cnt = 0, cpu;

<<<<<<< HEAD
	trace_init_global_iter(&iter);
	iter.buffer_iter = buffer_iter;
	tr = iter.tr;

	for_each_tracing_cpu(cpu) {
		atomic_inc(&per_cpu_ptr(iter.trace_buffer->data, cpu)->disabled);
	}

=======
	tr = iter.tr;

>>>>>>> upstream/android-13
	old_userobj = tr->trace_flags;

	/* don't look at user memory in panic mode */
	tr->trace_flags &= ~TRACE_ITER_SYM_USEROBJ;

	kdb_printf("Dumping ftrace buffer:\n");
<<<<<<< HEAD
=======
	if (skip_entries)
		kdb_printf("(skipping %d entries)\n", skip_entries);
>>>>>>> upstream/android-13

	trace_iterator_reset(&iter);
	iter.iter_flags |= TRACE_FILE_LAT_FMT;

	if (cpu_file == RING_BUFFER_ALL_CPUS) {
		for_each_tracing_cpu(cpu) {
			iter.buffer_iter[cpu] =
<<<<<<< HEAD
			ring_buffer_read_prepare(iter.trace_buffer->buffer,
=======
			ring_buffer_read_prepare(iter.array_buffer->buffer,
>>>>>>> upstream/android-13
						 cpu, GFP_ATOMIC);
			ring_buffer_read_start(iter.buffer_iter[cpu]);
			tracing_iter_reset(&iter, cpu);
		}
	} else {
		iter.cpu_file = cpu_file;
		iter.buffer_iter[cpu_file] =
<<<<<<< HEAD
			ring_buffer_read_prepare(iter.trace_buffer->buffer,
=======
			ring_buffer_read_prepare(iter.array_buffer->buffer,
>>>>>>> upstream/android-13
						 cpu_file, GFP_ATOMIC);
		ring_buffer_read_start(iter.buffer_iter[cpu_file]);
		tracing_iter_reset(&iter, cpu_file);
	}

	while (trace_find_next_entry_inc(&iter)) {
		if (!cnt)
			kdb_printf("---------------------------------\n");
		cnt++;

<<<<<<< HEAD
		if (!skip_lines) {
			print_trace_line(&iter);
			trace_printk_seq(&iter.seq);
		} else {
			skip_lines--;
=======
		if (!skip_entries) {
			print_trace_line(&iter);
			trace_printk_seq(&iter.seq);
		} else {
			skip_entries--;
>>>>>>> upstream/android-13
		}

		if (KDB_FLAG(CMD_INTERRUPT))
			goto out;
	}

	if (!cnt)
		kdb_printf("   (ftrace buffer empty)\n");
	else
		kdb_printf("---------------------------------\n");

out:
	tr->trace_flags = old_userobj;

	for_each_tracing_cpu(cpu) {
<<<<<<< HEAD
		atomic_dec(&per_cpu_ptr(iter.trace_buffer->data, cpu)->disabled);
	}

	for_each_tracing_cpu(cpu) {
=======
>>>>>>> upstream/android-13
		if (iter.buffer_iter[cpu]) {
			ring_buffer_read_finish(iter.buffer_iter[cpu]);
			iter.buffer_iter[cpu] = NULL;
		}
	}
}

/*
 * kdb_ftdump - Dump the ftrace log buffer
 */
static int kdb_ftdump(int argc, const char **argv)
{
<<<<<<< HEAD
	int skip_lines = 0;
	long cpu_file;
	char *cp;
=======
	int skip_entries = 0;
	long cpu_file;
	char *cp;
	int cnt;
	int cpu;
>>>>>>> upstream/android-13

	if (argc > 2)
		return KDB_ARGCOUNT;

	if (argc) {
<<<<<<< HEAD
		skip_lines = simple_strtol(argv[1], &cp, 0);
		if (*cp)
			skip_lines = 0;
=======
		skip_entries = simple_strtol(argv[1], &cp, 0);
		if (*cp)
			skip_entries = 0;
>>>>>>> upstream/android-13
	}

	if (argc == 2) {
		cpu_file = simple_strtol(argv[2], &cp, 0);
		if (*cp || cpu_file >= NR_CPUS || cpu_file < 0 ||
		    !cpu_online(cpu_file))
			return KDB_BADINT;
	} else {
		cpu_file = RING_BUFFER_ALL_CPUS;
	}

	kdb_trap_printk++;
<<<<<<< HEAD
	ftrace_dump_buf(skip_lines, cpu_file);
=======

	trace_init_global_iter(&iter);
	iter.buffer_iter = buffer_iter;

	for_each_tracing_cpu(cpu) {
		atomic_inc(&per_cpu_ptr(iter.array_buffer->data, cpu)->disabled);
	}

	/* A negative skip_entries means skip all but the last entries */
	if (skip_entries < 0) {
		if (cpu_file == RING_BUFFER_ALL_CPUS)
			cnt = trace_total_entries(NULL);
		else
			cnt = trace_total_entries_cpu(NULL, cpu_file);
		skip_entries = max(cnt + skip_entries, 0);
	}

	ftrace_dump_buf(skip_entries, cpu_file);

	for_each_tracing_cpu(cpu) {
		atomic_dec(&per_cpu_ptr(iter.array_buffer->data, cpu)->disabled);
	}

>>>>>>> upstream/android-13
	kdb_trap_printk--;

	return 0;
}

<<<<<<< HEAD
static __init int kdb_ftrace_register(void)
{
	kdb_register_flags("ftdump", kdb_ftdump, "[skip_#lines] [cpu]",
			    "Dump ftrace log", 0, KDB_ENABLE_ALWAYS_SAFE);
=======
static kdbtab_t ftdump_cmd = {
	.name = "ftdump",
	.func = kdb_ftdump,
	.usage = "[skip_#entries] [cpu]",
	.help = "Dump ftrace log; -skip dumps last #entries",
	.flags = KDB_ENABLE_ALWAYS_SAFE,
};

static __init int kdb_ftrace_register(void)
{
	kdb_register(&ftdump_cmd);
>>>>>>> upstream/android-13
	return 0;
}

late_initcall(kdb_ftrace_register);
