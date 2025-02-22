/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TRACE_H
#define _LINUX_TRACE_H

#ifdef CONFIG_TRACING
<<<<<<< HEAD
=======

#define TRACE_EXPORT_FUNCTION	BIT(0)
#define TRACE_EXPORT_EVENT	BIT(1)
#define TRACE_EXPORT_MARKER	BIT(2)

>>>>>>> upstream/android-13
/*
 * The trace export - an export of Ftrace output. The trace_export
 * can process traces and export them to a registered destination as
 * an addition to the current only output of Ftrace - i.e. ring buffer.
 *
 * If you want traces to be sent to some other place rather than ring
 * buffer only, just need to register a new trace_export and implement
 * its own .write() function for writing traces to the storage.
 *
 * next		- pointer to the next trace_export
 * write	- copy traces which have been delt with ->commit() to
 *		  the destination
<<<<<<< HEAD
=======
 * flags	- which ftrace to be exported
>>>>>>> upstream/android-13
 */
struct trace_export {
	struct trace_export __rcu	*next;
	void (*write)(struct trace_export *, const void *, unsigned int);
<<<<<<< HEAD
=======
	int flags;
>>>>>>> upstream/android-13
};

int register_ftrace_export(struct trace_export *export);
int unregister_ftrace_export(struct trace_export *export);

<<<<<<< HEAD
=======
struct trace_array;

void trace_printk_init_buffers(void);
__printf(3, 4)
int trace_array_printk(struct trace_array *tr, unsigned long ip,
		       const char *fmt, ...);
int trace_array_init_printk(struct trace_array *tr);
void trace_array_put(struct trace_array *tr);
struct trace_array *trace_array_get_by_name(const char *name);
int trace_array_destroy(struct trace_array *tr);

/* For osnoise tracer */
int osnoise_arch_register(void);
void osnoise_arch_unregister(void);
void osnoise_trace_irq_entry(int id);
void osnoise_trace_irq_exit(int id, const char *desc);

>>>>>>> upstream/android-13
#endif	/* CONFIG_TRACING */

#endif	/* _LINUX_TRACE_H */
