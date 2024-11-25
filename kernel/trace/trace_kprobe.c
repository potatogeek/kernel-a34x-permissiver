// SPDX-License-Identifier: GPL-2.0
/*
 * Kprobes-based tracing events
 *
 * Created by Masami Hiramatsu <mhiramat@redhat.com>
 *
 */
#define pr_fmt(fmt)	"trace_kprobe: " fmt

<<<<<<< HEAD
=======
#include <linux/security.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/rculist.h>
#include <linux/error-injection.h>

<<<<<<< HEAD
#include "trace_kprobe_selftest.h"
#include "trace_probe.h"
=======
#include <asm/setup.h>  /* for COMMAND_LINE_SIZE */

#include "trace_dynevent.h"
#include "trace_kprobe_selftest.h"
#include "trace_probe.h"
#include "trace_probe_tmpl.h"
>>>>>>> upstream/android-13

#define KPROBE_EVENT_SYSTEM "kprobes"
#define KRETPROBE_MAXACTIVE_MAX 4096

<<<<<<< HEAD
/**
 * Kprobe event core functions
 */
struct trace_kprobe {
	struct list_head	list;
=======
/* Kprobe early definition from command line */
static char kprobe_boot_events_buf[COMMAND_LINE_SIZE] __initdata;

static int __init set_kprobe_boot_events(char *str)
{
	strlcpy(kprobe_boot_events_buf, str, COMMAND_LINE_SIZE);
	disable_tracing_selftest("running kprobe events");

	return 1;
}
__setup("kprobe_event=", set_kprobe_boot_events);

static int trace_kprobe_create(const char *raw_command);
static int trace_kprobe_show(struct seq_file *m, struct dyn_event *ev);
static int trace_kprobe_release(struct dyn_event *ev);
static bool trace_kprobe_is_busy(struct dyn_event *ev);
static bool trace_kprobe_match(const char *system, const char *event,
			int argc, const char **argv, struct dyn_event *ev);

static struct dyn_event_operations trace_kprobe_ops = {
	.create = trace_kprobe_create,
	.show = trace_kprobe_show,
	.is_busy = trace_kprobe_is_busy,
	.free = trace_kprobe_release,
	.match = trace_kprobe_match,
};

/*
 * Kprobe event core functions
 */
struct trace_kprobe {
	struct dyn_event	devent;
>>>>>>> upstream/android-13
	struct kretprobe	rp;	/* Use rp.kp for kprobe use */
	unsigned long __percpu *nhit;
	const char		*symbol;	/* symbol name */
	struct trace_probe	tp;
};

<<<<<<< HEAD
#define SIZEOF_TRACE_KPROBE(n)				\
	(offsetof(struct trace_kprobe, tp.args) +	\
	(sizeof(struct probe_arg) * (n)))
=======
static bool is_trace_kprobe(struct dyn_event *ev)
{
	return ev->ops == &trace_kprobe_ops;
}

static struct trace_kprobe *to_trace_kprobe(struct dyn_event *ev)
{
	return container_of(ev, struct trace_kprobe, devent);
}

/**
 * for_each_trace_kprobe - iterate over the trace_kprobe list
 * @pos:	the struct trace_kprobe * for each entry
 * @dpos:	the struct dyn_event * to use as a loop cursor
 */
#define for_each_trace_kprobe(pos, dpos)	\
	for_each_dyn_event(dpos)		\
		if (is_trace_kprobe(dpos) && (pos = to_trace_kprobe(dpos)))
>>>>>>> upstream/android-13

static nokprobe_inline bool trace_kprobe_is_return(struct trace_kprobe *tk)
{
	return tk->rp.handler != NULL;
}

static nokprobe_inline const char *trace_kprobe_symbol(struct trace_kprobe *tk)
{
	return tk->symbol ? tk->symbol : "unknown";
}

static nokprobe_inline unsigned long trace_kprobe_offset(struct trace_kprobe *tk)
{
	return tk->rp.kp.offset;
}

static nokprobe_inline bool trace_kprobe_has_gone(struct trace_kprobe *tk)
{
	return !!(kprobe_gone(&tk->rp.kp));
}

static nokprobe_inline bool trace_kprobe_within_module(struct trace_kprobe *tk,
						 struct module *mod)
{
<<<<<<< HEAD
	int len = strlen(mod->name);
	const char *name = trace_kprobe_symbol(tk);
	return strncmp(mod->name, name, len) == 0 && name[len] == ':';
=======
	int len = strlen(module_name(mod));
	const char *name = trace_kprobe_symbol(tk);

	return strncmp(module_name(mod), name, len) == 0 && name[len] == ':';
>>>>>>> upstream/android-13
}

static nokprobe_inline bool trace_kprobe_module_exist(struct trace_kprobe *tk)
{
	char *p;
	bool ret;

	if (!tk->symbol)
		return false;
	p = strchr(tk->symbol, ':');
	if (!p)
		return true;
	*p = '\0';
<<<<<<< HEAD
	mutex_lock(&module_mutex);
	ret = !!find_module(tk->symbol);
	mutex_unlock(&module_mutex);
=======
	rcu_read_lock_sched();
	ret = !!find_module(tk->symbol);
	rcu_read_unlock_sched();
>>>>>>> upstream/android-13
	*p = ':';

	return ret;
}

<<<<<<< HEAD
=======
static bool trace_kprobe_is_busy(struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);

	return trace_probe_is_enabled(&tk->tp);
}

static bool trace_kprobe_match_command_head(struct trace_kprobe *tk,
					    int argc, const char **argv)
{
	char buf[MAX_ARGSTR_LEN + 1];

	if (!argc)
		return true;

	if (!tk->symbol)
		snprintf(buf, sizeof(buf), "0x%p", tk->rp.kp.addr);
	else if (tk->rp.kp.offset)
		snprintf(buf, sizeof(buf), "%s+%u",
			 trace_kprobe_symbol(tk), tk->rp.kp.offset);
	else
		snprintf(buf, sizeof(buf), "%s", trace_kprobe_symbol(tk));
	if (strcmp(buf, argv[0]))
		return false;
	argc--; argv++;

	return trace_probe_match_command_args(&tk->tp, argc, argv);
}

static bool trace_kprobe_match(const char *system, const char *event,
			int argc, const char **argv, struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);

	return strcmp(trace_probe_name(&tk->tp), event) == 0 &&
	    (!system || strcmp(trace_probe_group_name(&tk->tp), system) == 0) &&
	    trace_kprobe_match_command_head(tk, argc, argv);
}

>>>>>>> upstream/android-13
static nokprobe_inline unsigned long trace_kprobe_nhit(struct trace_kprobe *tk)
{
	unsigned long nhit = 0;
	int cpu;

	for_each_possible_cpu(cpu)
		nhit += *per_cpu_ptr(tk->nhit, cpu);

	return nhit;
}

<<<<<<< HEAD
=======
static nokprobe_inline bool trace_kprobe_is_registered(struct trace_kprobe *tk)
{
	return !(list_empty(&tk->rp.kp.list) &&
		 hlist_unhashed(&tk->rp.kp.hlist));
}

>>>>>>> upstream/android-13
/* Return 0 if it fails to find the symbol address */
static nokprobe_inline
unsigned long trace_kprobe_address(struct trace_kprobe *tk)
{
	unsigned long addr;

	if (tk->symbol) {
		addr = (unsigned long)
			kallsyms_lookup_name(trace_kprobe_symbol(tk));
		if (addr)
			addr += tk->rp.kp.offset;
	} else {
		addr = (unsigned long)tk->rp.kp.addr;
	}
	return addr;
}

<<<<<<< HEAD
bool trace_kprobe_on_func_entry(struct trace_event_call *call)
{
	struct trace_kprobe *tk = (struct trace_kprobe *)call->data;

	return (kprobe_on_func_entry(tk->rp.kp.addr,
			tk->rp.kp.addr ? NULL : tk->rp.kp.symbol_name,
			tk->rp.kp.addr ? 0 : tk->rp.kp.offset) == 0);
=======
static nokprobe_inline struct trace_kprobe *
trace_kprobe_primary_from_call(struct trace_event_call *call)
{
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(call);
	if (WARN_ON_ONCE(!tp))
		return NULL;

	return container_of(tp, struct trace_kprobe, tp);
}

bool trace_kprobe_on_func_entry(struct trace_event_call *call)
{
	struct trace_kprobe *tk = trace_kprobe_primary_from_call(call);

	return tk ? (kprobe_on_func_entry(tk->rp.kp.addr,
			tk->rp.kp.addr ? NULL : tk->rp.kp.symbol_name,
			tk->rp.kp.addr ? 0 : tk->rp.kp.offset) == 0) : false;
>>>>>>> upstream/android-13
}

bool trace_kprobe_error_injectable(struct trace_event_call *call)
{
<<<<<<< HEAD
	struct trace_kprobe *tk = (struct trace_kprobe *)call->data;

	return within_error_injection_list(trace_kprobe_address(tk));
=======
	struct trace_kprobe *tk = trace_kprobe_primary_from_call(call);

	return tk ? within_error_injection_list(trace_kprobe_address(tk)) :
	       false;
>>>>>>> upstream/android-13
}

static int register_kprobe_event(struct trace_kprobe *tk);
static int unregister_kprobe_event(struct trace_kprobe *tk);

<<<<<<< HEAD
static DEFINE_MUTEX(probe_lock);
static LIST_HEAD(probe_list);

=======
>>>>>>> upstream/android-13
static int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs);
static int kretprobe_dispatcher(struct kretprobe_instance *ri,
				struct pt_regs *regs);

<<<<<<< HEAD
/* Memory fetching by symbol */
struct symbol_cache {
	char		*symbol;
	long		offset;
	unsigned long	addr;
};

unsigned long update_symbol_cache(struct symbol_cache *sc)
{
	sc->addr = (unsigned long)kallsyms_lookup_name(sc->symbol);

	if (sc->addr)
		sc->addr += sc->offset;

	return sc->addr;
}

void free_symbol_cache(struct symbol_cache *sc)
{
	kfree(sc->symbol);
	kfree(sc);
}

struct symbol_cache *alloc_symbol_cache(const char *sym, long offset)
{
	struct symbol_cache *sc;

	if (!sym || strlen(sym) == 0)
		return NULL;

	sc = kzalloc(sizeof(struct symbol_cache), GFP_KERNEL);
	if (!sc)
		return NULL;

	sc->symbol = kstrdup(sym, GFP_KERNEL);
	if (!sc->symbol) {
		kfree(sc);
		return NULL;
	}
	sc->offset = offset;
	update_symbol_cache(sc);

	return sc;
}

/*
 * Kprobes-specific fetch functions
 */
#define DEFINE_FETCH_stack(type)					\
static void FETCH_FUNC_NAME(stack, type)(struct pt_regs *regs,		\
					  void *offset, void *dest)	\
{									\
	*(type *)dest = (type)regs_get_kernel_stack_nth(regs,		\
				(unsigned int)((unsigned long)offset));	\
}									\
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(stack, type));

DEFINE_BASIC_FETCH_FUNCS(stack)
/* No string on the stack entry */
#define fetch_stack_string	NULL
#define fetch_stack_string_size	NULL

#define DEFINE_FETCH_memory(type)					\
static void FETCH_FUNC_NAME(memory, type)(struct pt_regs *regs,		\
					  void *addr, void *dest)	\
{									\
	type retval;							\
	if (probe_kernel_address(addr, retval))				\
		*(type *)dest = 0;					\
	else								\
		*(type *)dest = retval;					\
}									\
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(memory, type));

DEFINE_BASIC_FETCH_FUNCS(memory)
/*
 * Fetch a null-terminated string. Caller MUST set *(u32 *)dest with max
 * length and relative data location.
 */
static void FETCH_FUNC_NAME(memory, string)(struct pt_regs *regs,
					    void *addr, void *dest)
{
	int maxlen = get_rloc_len(*(u32 *)dest);
	u8 *dst = get_rloc_data(dest);
	long ret;

	if (!maxlen)
		return;

	/*
	 * Try to get string again, since the string can be changed while
	 * probing.
	 */
	ret = strncpy_from_unsafe(dst, addr, maxlen);

	if (ret < 0) {	/* Failed to fetch string */
		dst[0] = '\0';
		*(u32 *)dest = make_data_rloc(0, get_rloc_offs(*(u32 *)dest));
	} else {
		*(u32 *)dest = make_data_rloc(ret, get_rloc_offs(*(u32 *)dest));
	}
}
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(memory, string));

/* Return the length of string -- including null terminal byte */
static void FETCH_FUNC_NAME(memory, string_size)(struct pt_regs *regs,
						 void *addr, void *dest)
{
	mm_segment_t old_fs;
	int ret, len = 0;
	u8 c;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	pagefault_disable();

	do {
		ret = __copy_from_user_inatomic(&c, (u8 *)addr + len, 1);
		len++;
	} while (c && ret == 0 && len < MAX_STRING_SIZE);

	pagefault_enable();
	set_fs(old_fs);

	if (ret < 0)	/* Failed to check the length */
		*(u32 *)dest = 0;
	else
		*(u32 *)dest = len;
}
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(memory, string_size));

#define DEFINE_FETCH_symbol(type)					\
void FETCH_FUNC_NAME(symbol, type)(struct pt_regs *regs, void *data, void *dest)\
{									\
	struct symbol_cache *sc = data;					\
	if (sc->addr)							\
		fetch_memory_##type(regs, (void *)sc->addr, dest);	\
	else								\
		*(type *)dest = 0;					\
}									\
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(symbol, type));

DEFINE_BASIC_FETCH_FUNCS(symbol)
DEFINE_FETCH_symbol(string)
DEFINE_FETCH_symbol(string_size)

/* kprobes don't support file_offset fetch methods */
#define fetch_file_offset_u8		NULL
#define fetch_file_offset_u16		NULL
#define fetch_file_offset_u32		NULL
#define fetch_file_offset_u64		NULL
#define fetch_file_offset_string	NULL
#define fetch_file_offset_string_size	NULL

/* Fetch type information table */
static const struct fetch_type kprobes_fetch_type_table[] = {
	/* Special types */
	[FETCH_TYPE_STRING] = __ASSIGN_FETCH_TYPE("string", string, string,
					sizeof(u32), 1, "__data_loc char[]"),
	[FETCH_TYPE_STRSIZE] = __ASSIGN_FETCH_TYPE("string_size", u32,
					string_size, sizeof(u32), 0, "u32"),
	/* Basic types */
	ASSIGN_FETCH_TYPE(u8,  u8,  0),
	ASSIGN_FETCH_TYPE(u16, u16, 0),
	ASSIGN_FETCH_TYPE(u32, u32, 0),
	ASSIGN_FETCH_TYPE(u64, u64, 0),
	ASSIGN_FETCH_TYPE(s8,  u8,  1),
	ASSIGN_FETCH_TYPE(s16, u16, 1),
	ASSIGN_FETCH_TYPE(s32, u32, 1),
	ASSIGN_FETCH_TYPE(s64, u64, 1),
	ASSIGN_FETCH_TYPE_ALIAS(x8,  u8,  u8,  0),
	ASSIGN_FETCH_TYPE_ALIAS(x16, u16, u16, 0),
	ASSIGN_FETCH_TYPE_ALIAS(x32, u32, u32, 0),
	ASSIGN_FETCH_TYPE_ALIAS(x64, u64, u64, 0),

	ASSIGN_FETCH_TYPE_END
};
=======
static void free_trace_kprobe(struct trace_kprobe *tk)
{
	if (tk) {
		trace_probe_cleanup(&tk->tp);
		kfree(tk->symbol);
		free_percpu(tk->nhit);
		kfree(tk);
	}
}
>>>>>>> upstream/android-13

/*
 * Allocate new trace_probe and initialize it (including kprobes).
 */
static struct trace_kprobe *alloc_trace_kprobe(const char *group,
					     const char *event,
					     void *addr,
					     const char *symbol,
					     unsigned long offs,
					     int maxactive,
					     int nargs, bool is_return)
{
	struct trace_kprobe *tk;
	int ret = -ENOMEM;

<<<<<<< HEAD
	tk = kzalloc(SIZEOF_TRACE_KPROBE(nargs), GFP_KERNEL);
=======
	tk = kzalloc(struct_size(tk, tp.args, nargs), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!tk)
		return ERR_PTR(ret);

	tk->nhit = alloc_percpu(unsigned long);
	if (!tk->nhit)
		goto error;

	if (symbol) {
		tk->symbol = kstrdup(symbol, GFP_KERNEL);
		if (!tk->symbol)
			goto error;
		tk->rp.kp.symbol_name = tk->symbol;
		tk->rp.kp.offset = offs;
	} else
		tk->rp.kp.addr = addr;

	if (is_return)
		tk->rp.handler = kretprobe_dispatcher;
	else
		tk->rp.kp.pre_handler = kprobe_dispatcher;

	tk->rp.maxactive = maxactive;
<<<<<<< HEAD

	if (!event || !is_good_name(event)) {
		ret = -EINVAL;
		goto error;
	}

	tk->tp.call.class = &tk->tp.class;
	tk->tp.call.name = kstrdup(event, GFP_KERNEL);
	if (!tk->tp.call.name)
		goto error;

	if (!group || !is_good_name(group)) {
		ret = -EINVAL;
		goto error;
	}

	tk->tp.class.system = kstrdup(group, GFP_KERNEL);
	if (!tk->tp.class.system)
		goto error;

	INIT_LIST_HEAD(&tk->list);
	INIT_LIST_HEAD(&tk->tp.files);
	return tk;
error:
	kfree(tk->tp.call.name);
	kfree(tk->symbol);
	free_percpu(tk->nhit);
	kfree(tk);
	return ERR_PTR(ret);
}

static void free_trace_kprobe(struct trace_kprobe *tk)
{
	int i;

	for (i = 0; i < tk->tp.nr_args; i++)
		traceprobe_free_probe_arg(&tk->tp.args[i]);

	kfree(tk->tp.call.class->system);
	kfree(tk->tp.call.name);
	kfree(tk->symbol);
	free_percpu(tk->nhit);
	kfree(tk);
}

static struct trace_kprobe *find_trace_kprobe(const char *event,
					      const char *group)
{
	struct trace_kprobe *tk;

	list_for_each_entry(tk, &probe_list, list)
		if (strcmp(trace_event_name(&tk->tp.call), event) == 0 &&
		    strcmp(tk->tp.call.class->system, group) == 0)
=======
	INIT_HLIST_NODE(&tk->rp.kp.hlist);
	INIT_LIST_HEAD(&tk->rp.kp.list);

	ret = trace_probe_init(&tk->tp, event, group, false);
	if (ret < 0)
		goto error;

	dyn_event_init(&tk->devent, &trace_kprobe_ops);
	return tk;
error:
	free_trace_kprobe(tk);
	return ERR_PTR(ret);
}

static struct trace_kprobe *find_trace_kprobe(const char *event,
					      const char *group)
{
	struct dyn_event *pos;
	struct trace_kprobe *tk;

	for_each_trace_kprobe(tk, pos)
		if (strcmp(trace_probe_name(&tk->tp), event) == 0 &&
		    strcmp(trace_probe_group_name(&tk->tp), group) == 0)
>>>>>>> upstream/android-13
			return tk;
	return NULL;
}

static inline int __enable_trace_kprobe(struct trace_kprobe *tk)
{
	int ret = 0;

<<<<<<< HEAD
	if (trace_probe_is_registered(&tk->tp) && !trace_kprobe_has_gone(tk)) {
=======
	if (trace_kprobe_is_registered(tk) && !trace_kprobe_has_gone(tk)) {
>>>>>>> upstream/android-13
		if (trace_kprobe_is_return(tk))
			ret = enable_kretprobe(&tk->rp);
		else
			ret = enable_kprobe(&tk->rp.kp);
	}

	return ret;
}

<<<<<<< HEAD
=======
static void __disable_trace_kprobe(struct trace_probe *tp)
{
	struct trace_probe *pos;
	struct trace_kprobe *tk;

	list_for_each_entry(pos, trace_probe_probe_list(tp), list) {
		tk = container_of(pos, struct trace_kprobe, tp);
		if (!trace_kprobe_is_registered(tk))
			continue;
		if (trace_kprobe_is_return(tk))
			disable_kretprobe(&tk->rp);
		else
			disable_kprobe(&tk->rp.kp);
	}
}

>>>>>>> upstream/android-13
/*
 * Enable trace_probe
 * if the file is NULL, enable "perf" handler, or enable "trace" handler.
 */
<<<<<<< HEAD
static int
enable_trace_kprobe(struct trace_kprobe *tk, struct trace_event_file *file)
{
	struct event_file_link *link;
	int ret = 0;

	if (file) {
		link = kmalloc(sizeof(*link), GFP_KERNEL);
		if (!link) {
			ret = -ENOMEM;
			goto out;
		}

		link->file = file;
		list_add_tail_rcu(&link->list, &tk->tp.files);

		tk->tp.flags |= TP_FLAG_TRACE;
		ret = __enable_trace_kprobe(tk);
		if (ret) {
			list_del_rcu(&link->list);
			kfree(link);
			tk->tp.flags &= ~TP_FLAG_TRACE;
		}

	} else {
		tk->tp.flags |= TP_FLAG_PROFILE;
		ret = __enable_trace_kprobe(tk);
		if (ret)
			tk->tp.flags &= ~TP_FLAG_PROFILE;
	}
 out:
=======
static int enable_trace_kprobe(struct trace_event_call *call,
				struct trace_event_file *file)
{
	struct trace_probe *pos, *tp;
	struct trace_kprobe *tk;
	bool enabled;
	int ret = 0;

	tp = trace_probe_primary_from_call(call);
	if (WARN_ON_ONCE(!tp))
		return -ENODEV;
	enabled = trace_probe_is_enabled(tp);

	/* This also changes "enabled" state */
	if (file) {
		ret = trace_probe_add_file(tp, file);
		if (ret)
			return ret;
	} else
		trace_probe_set_flag(tp, TP_FLAG_PROFILE);

	if (enabled)
		return 0;

	list_for_each_entry(pos, trace_probe_probe_list(tp), list) {
		tk = container_of(pos, struct trace_kprobe, tp);
		if (trace_kprobe_has_gone(tk))
			continue;
		ret = __enable_trace_kprobe(tk);
		if (ret)
			break;
		enabled = true;
	}

	if (ret) {
		/* Failed to enable one of them. Roll back all */
		if (enabled)
			__disable_trace_kprobe(tp);
		if (file)
			trace_probe_remove_file(tp, file);
		else
			trace_probe_clear_flag(tp, TP_FLAG_PROFILE);
	}

>>>>>>> upstream/android-13
	return ret;
}

/*
 * Disable trace_probe
 * if the file is NULL, disable "perf" handler, or disable "trace" handler.
 */
<<<<<<< HEAD
static int
disable_trace_kprobe(struct trace_kprobe *tk, struct trace_event_file *file)
{
	struct event_file_link *link = NULL;
	int wait = 0;
	int ret = 0;

	if (file) {
		link = find_event_file_link(&tk->tp, file);
		if (!link) {
			ret = -EINVAL;
			goto out;
		}

		list_del_rcu(&link->list);
		wait = 1;
		if (!list_empty(&tk->tp.files))
			goto out;

		tk->tp.flags &= ~TP_FLAG_TRACE;
	} else
		tk->tp.flags &= ~TP_FLAG_PROFILE;

	if (!trace_probe_is_enabled(&tk->tp) && trace_probe_is_registered(&tk->tp)) {
		if (trace_kprobe_is_return(tk))
			disable_kretprobe(&tk->rp);
		else
			disable_kprobe(&tk->rp.kp);
		wait = 1;
	}

	/*
	 * if tk is not added to any list, it must be a local trace_kprobe
	 * created with perf_event_open. We don't need to wait for these
	 * trace_kprobes
	 */
	if (list_empty(&tk->list))
		wait = 0;
 out:
	if (wait) {
		/*
		 * Synchronize with kprobe_trace_func/kretprobe_trace_func
		 * to ensure disabled (all running handlers are finished).
		 * This is not only for kfree(), but also the caller,
		 * trace_remove_event_call() supposes it for releasing
		 * event_call related objects, which will be accessed in
		 * the kprobe_trace_func/kretprobe_trace_func.
		 */
		synchronize_sched();
		kfree(link);	/* Ignored if link == NULL */
	}

	return ret;
=======
static int disable_trace_kprobe(struct trace_event_call *call,
				struct trace_event_file *file)
{
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(call);
	if (WARN_ON_ONCE(!tp))
		return -ENODEV;

	if (file) {
		if (!trace_probe_get_file_link(tp, file))
			return -ENOENT;
		if (!trace_probe_has_single_file(tp))
			goto out;
		trace_probe_clear_flag(tp, TP_FLAG_TRACE);
	} else
		trace_probe_clear_flag(tp, TP_FLAG_PROFILE);

	if (!trace_probe_is_enabled(tp))
		__disable_trace_kprobe(tp);

 out:
	if (file)
		/*
		 * Synchronization is done in below function. For perf event,
		 * file == NULL and perf_trace_event_unreg() calls
		 * tracepoint_synchronize_unregister() to ensure synchronize
		 * event. We don't need to care about it.
		 */
		trace_probe_remove_file(tp, file);

	return 0;
>>>>>>> upstream/android-13
}

#if defined(CONFIG_DYNAMIC_FTRACE) && \
	!defined(CONFIG_KPROBE_EVENTS_ON_NOTRACE)
static bool __within_notrace_func(unsigned long addr)
{
	unsigned long offset, size;

	if (!addr || !kallsyms_lookup_size_offset(addr, &size, &offset))
		return false;

	/* Get the entry address of the target function */
	addr -= offset;

	/*
	 * Since ftrace_location_range() does inclusive range check, we need
	 * to subtract 1 byte from the end address.
	 */
	return !ftrace_location_range(addr, addr + size - 1);
}

static bool within_notrace_func(struct trace_kprobe *tk)
{
	unsigned long addr = trace_kprobe_address(tk);
	char symname[KSYM_NAME_LEN], *p;

	if (!__within_notrace_func(addr))
		return false;

	/* Check if the address is on a suffixed-symbol */
	if (!lookup_symbol_name(addr, symname)) {
		p = strchr(symname, '.');
		if (!p)
			return true;
		*p = '\0';
		addr = (unsigned long)kprobe_lookup_name(symname, 0);
		if (addr)
			return __within_notrace_func(addr);
	}

	return true;
}
#else
#define within_notrace_func(tk)	(false)
#endif

/* Internal register function - just handle k*probes and flags */
static int __register_trace_kprobe(struct trace_kprobe *tk)
{
	int i, ret;

<<<<<<< HEAD
	if (trace_probe_is_registered(&tk->tp))
=======
	ret = security_locked_down(LOCKDOWN_KPROBES);
	if (ret)
		return ret;

	if (trace_kprobe_is_registered(tk))
>>>>>>> upstream/android-13
		return -EINVAL;

	if (within_notrace_func(tk)) {
		pr_warn("Could not probe notrace function %s\n",
			trace_kprobe_symbol(tk));
		return -EINVAL;
	}

<<<<<<< HEAD
	for (i = 0; i < tk->tp.nr_args; i++)
		traceprobe_update_arg(&tk->tp.args[i]);
=======
	for (i = 0; i < tk->tp.nr_args; i++) {
		ret = traceprobe_update_arg(&tk->tp.args[i]);
		if (ret)
			return ret;
	}
>>>>>>> upstream/android-13

	/* Set/clear disabled flag according to tp->flag */
	if (trace_probe_is_enabled(&tk->tp))
		tk->rp.kp.flags &= ~KPROBE_FLAG_DISABLED;
	else
		tk->rp.kp.flags |= KPROBE_FLAG_DISABLED;

	if (trace_kprobe_is_return(tk))
		ret = register_kretprobe(&tk->rp);
	else
		ret = register_kprobe(&tk->rp.kp);

<<<<<<< HEAD
	if (ret == 0) {
		tk->tp.flags |= TP_FLAG_REGISTERED;
	} else if (ret == -EILSEQ) {
		pr_warn("Probing address(0x%p) is not an instruction boundary.\n",
			tk->rp.kp.addr);
		ret = -EINVAL;
	}
=======
>>>>>>> upstream/android-13
	return ret;
}

/* Internal unregister function - just handle k*probes and flags */
static void __unregister_trace_kprobe(struct trace_kprobe *tk)
{
<<<<<<< HEAD
	if (trace_probe_is_registered(&tk->tp)) {
=======
	if (trace_kprobe_is_registered(tk)) {
>>>>>>> upstream/android-13
		if (trace_kprobe_is_return(tk))
			unregister_kretprobe(&tk->rp);
		else
			unregister_kprobe(&tk->rp.kp);
<<<<<<< HEAD
		tk->tp.flags &= ~TP_FLAG_REGISTERED;
		/* Cleanup kprobe for reuse */
=======
		/* Cleanup kprobe for reuse and mark it unregistered */
		INIT_HLIST_NODE(&tk->rp.kp.hlist);
		INIT_LIST_HEAD(&tk->rp.kp.list);
>>>>>>> upstream/android-13
		if (tk->rp.kp.symbol_name)
			tk->rp.kp.addr = NULL;
	}
}

<<<<<<< HEAD
/* Unregister a trace_probe and probe_event: call with locking probe_lock */
static int unregister_trace_kprobe(struct trace_kprobe *tk)
{
=======
/* Unregister a trace_probe and probe_event */
static int unregister_trace_kprobe(struct trace_kprobe *tk)
{
	/* If other probes are on the event, just unregister kprobe */
	if (trace_probe_has_sibling(&tk->tp))
		goto unreg;

>>>>>>> upstream/android-13
	/* Enabled event can not be unregistered */
	if (trace_probe_is_enabled(&tk->tp))
		return -EBUSY;

<<<<<<< HEAD
=======
	/* If there's a reference to the dynamic event */
	if (trace_event_dyn_busy(trace_probe_event_call(&tk->tp)))
		return -EBUSY;

>>>>>>> upstream/android-13
	/* Will fail if probe is being used by ftrace or perf */
	if (unregister_kprobe_event(tk))
		return -EBUSY;

<<<<<<< HEAD
	__unregister_trace_kprobe(tk);
	list_del(&tk->list);
=======
unreg:
	__unregister_trace_kprobe(tk);
	dyn_event_remove(&tk->devent);
	trace_probe_unlink(&tk->tp);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
=======
static bool trace_kprobe_has_same_kprobe(struct trace_kprobe *orig,
					 struct trace_kprobe *comp)
{
	struct trace_probe_event *tpe = orig->tp.event;
	struct trace_probe *pos;
	int i;

	list_for_each_entry(pos, &tpe->probes, list) {
		orig = container_of(pos, struct trace_kprobe, tp);
		if (strcmp(trace_kprobe_symbol(orig),
			   trace_kprobe_symbol(comp)) ||
		    trace_kprobe_offset(orig) != trace_kprobe_offset(comp))
			continue;

		/*
		 * trace_probe_compare_arg_type() ensured that nr_args and
		 * each argument name and type are same. Let's compare comm.
		 */
		for (i = 0; i < orig->tp.nr_args; i++) {
			if (strcmp(orig->tp.args[i].comm,
				   comp->tp.args[i].comm))
				break;
		}

		if (i == orig->tp.nr_args)
			return true;
	}

	return false;
}

static int append_trace_kprobe(struct trace_kprobe *tk, struct trace_kprobe *to)
{
	int ret;

	ret = trace_probe_compare_arg_type(&tk->tp, &to->tp);
	if (ret) {
		/* Note that argument starts index = 2 */
		trace_probe_log_set_index(ret + 1);
		trace_probe_log_err(0, DIFF_ARG_TYPE);
		return -EEXIST;
	}
	if (trace_kprobe_has_same_kprobe(to, tk)) {
		trace_probe_log_set_index(0);
		trace_probe_log_err(0, SAME_PROBE);
		return -EEXIST;
	}

	/* Append to existing event */
	ret = trace_probe_append(&tk->tp, &to->tp);
	if (ret)
		return ret;

	/* Register k*probe */
	ret = __register_trace_kprobe(tk);
	if (ret == -ENOENT && !trace_kprobe_module_exist(tk)) {
		pr_warn("This probe might be able to register after target module is loaded. Continue.\n");
		ret = 0;
	}

	if (ret)
		trace_probe_unlink(&tk->tp);
	else
		dyn_event_add(&tk->devent, trace_probe_event_call(&tk->tp));

	return ret;
}

>>>>>>> upstream/android-13
/* Register a trace_probe and probe_event */
static int register_trace_kprobe(struct trace_kprobe *tk)
{
	struct trace_kprobe *old_tk;
	int ret;

<<<<<<< HEAD
	mutex_lock(&probe_lock);

	/* Delete old (same name) event if exist */
	old_tk = find_trace_kprobe(trace_event_name(&tk->tp.call),
			tk->tp.call.class->system);
	if (old_tk) {
		ret = unregister_trace_kprobe(old_tk);
		if (ret < 0)
			goto end;
		free_trace_kprobe(old_tk);
=======
	mutex_lock(&event_mutex);

	old_tk = find_trace_kprobe(trace_probe_name(&tk->tp),
				   trace_probe_group_name(&tk->tp));
	if (old_tk) {
		if (trace_kprobe_is_return(tk) != trace_kprobe_is_return(old_tk)) {
			trace_probe_log_set_index(0);
			trace_probe_log_err(0, DIFF_PROBE_TYPE);
			ret = -EEXIST;
		} else {
			ret = append_trace_kprobe(tk, old_tk);
		}
		goto end;
>>>>>>> upstream/android-13
	}

	/* Register new event */
	ret = register_kprobe_event(tk);
	if (ret) {
<<<<<<< HEAD
		pr_warn("Failed to register probe event(%d)\n", ret);
=======
		if (ret == -EEXIST) {
			trace_probe_log_set_index(0);
			trace_probe_log_err(0, EVENT_EXIST);
		} else
			pr_warn("Failed to register probe event(%d)\n", ret);
>>>>>>> upstream/android-13
		goto end;
	}

	/* Register k*probe */
	ret = __register_trace_kprobe(tk);
	if (ret == -ENOENT && !trace_kprobe_module_exist(tk)) {
		pr_warn("This probe might be able to register after target module is loaded. Continue.\n");
		ret = 0;
	}

	if (ret < 0)
		unregister_kprobe_event(tk);
	else
<<<<<<< HEAD
		list_add_tail(&tk->list, &probe_list);

end:
	mutex_unlock(&probe_lock);
=======
		dyn_event_add(&tk->devent, trace_probe_event_call(&tk->tp));

end:
	mutex_unlock(&event_mutex);
>>>>>>> upstream/android-13
	return ret;
}

/* Module notifier call back, checking event on the module */
static int trace_kprobe_module_callback(struct notifier_block *nb,
				       unsigned long val, void *data)
{
	struct module *mod = data;
<<<<<<< HEAD
=======
	struct dyn_event *pos;
>>>>>>> upstream/android-13
	struct trace_kprobe *tk;
	int ret;

	if (val != MODULE_STATE_COMING)
		return NOTIFY_DONE;

	/* Update probes on coming module */
<<<<<<< HEAD
	mutex_lock(&probe_lock);
	list_for_each_entry(tk, &probe_list, list) {
=======
	mutex_lock(&event_mutex);
	for_each_trace_kprobe(tk, pos) {
>>>>>>> upstream/android-13
		if (trace_kprobe_within_module(tk, mod)) {
			/* Don't need to check busy - this should have gone. */
			__unregister_trace_kprobe(tk);
			ret = __register_trace_kprobe(tk);
			if (ret)
				pr_warn("Failed to re-register probe %s on %s: %d\n",
<<<<<<< HEAD
					trace_event_name(&tk->tp.call),
					mod->name, ret);
		}
	}
	mutex_unlock(&probe_lock);
=======
					trace_probe_name(&tk->tp),
					module_name(mod), ret);
		}
	}
	mutex_unlock(&event_mutex);
>>>>>>> upstream/android-13

	return NOTIFY_DONE;
}

static struct notifier_block trace_kprobe_module_nb = {
	.notifier_call = trace_kprobe_module_callback,
	.priority = 1	/* Invoked after kprobe module callback */
};

<<<<<<< HEAD
/* Convert certain expected symbols into '_' when generating event names */
static inline void sanitize_event_name(char *name)
{
	while (*name++ != '\0')
		if (*name == ':' || *name == '.')
			*name = '_';
}

static int create_trace_kprobe(int argc, char **argv)
=======
static int __trace_kprobe_create(int argc, const char *argv[])
>>>>>>> upstream/android-13
{
	/*
	 * Argument syntax:
	 *  - Add kprobe:
	 *      p[:[GRP/]EVENT] [MOD:]KSYM[+OFFS]|KADDR [FETCHARGS]
	 *  - Add kretprobe:
	 *      r[MAXACTIVE][:[GRP/]EVENT] [MOD:]KSYM[+0] [FETCHARGS]
<<<<<<< HEAD
=======
	 *    Or
	 *      p:[GRP/]EVENT] [MOD:]KSYM[+0]%return [FETCHARGS]
	 *
>>>>>>> upstream/android-13
	 * Fetch args:
	 *  $retval	: fetch return value
	 *  $stack	: fetch stack address
	 *  $stackN	: fetch Nth of stack (N:0-)
	 *  $comm       : fetch current task comm
	 *  @ADDR	: fetch memory at ADDR (ADDR should be in kernel)
	 *  @SYM[+|-offs] : fetch memory at SYM +|- offs (SYM is a data symbol)
	 *  %REG	: fetch register REG
	 * Dereferencing memory fetch:
	 *  +|-offs(ARG) : fetch memory at ARG +|- offs address.
	 * Alias name of args:
	 *  NAME=FETCHARG : set NAME as alias of FETCHARG.
	 * Type of args:
	 *  FETCHARG:TYPE : use TYPE instead of unsigned long.
	 */
<<<<<<< HEAD
	struct trace_kprobe *tk;
	int i, ret = 0;
	bool is_return = false, is_delete = false;
	char *symbol = NULL, *event = NULL, *group = NULL;
	int maxactive = 0;
	char *arg;
	long offset = 0;
	void *addr = NULL;
	char buf[MAX_EVENT_NAME_LEN];

	/* argc must be >= 1 */
	if (argv[0][0] == 'p')
		is_return = false;
	else if (argv[0][0] == 'r')
		is_return = true;
	else if (argv[0][0] == '-')
		is_delete = true;
	else {
		pr_info("Probe definition must be started with 'p', 'r' or"
			" '-'.\n");
		return -EINVAL;
	}

	event = strchr(&argv[0][1], ':');
	if (event) {
		event[0] = '\0';
		event++;
	}
	if (is_return && isdigit(argv[0][1])) {
		ret = kstrtouint(&argv[0][1], 0, &maxactive);
		if (ret) {
			pr_info("Failed to parse maxactive.\n");
			return ret;
=======
	struct trace_kprobe *tk = NULL;
	int i, len, ret = 0;
	bool is_return = false;
	char *symbol = NULL, *tmp = NULL;
	const char *event = NULL, *group = KPROBE_EVENT_SYSTEM;
	enum probe_print_type ptype;
	int maxactive = 0;
	long offset = 0;
	void *addr = NULL;
	char buf[MAX_EVENT_NAME_LEN];
	unsigned int flags = TPARG_FL_KERNEL;

	switch (argv[0][0]) {
	case 'r':
		is_return = true;
		break;
	case 'p':
		break;
	default:
		return -ECANCELED;
	}
	if (argc < 2)
		return -ECANCELED;

	trace_probe_log_init("trace_kprobe", argc, argv);

	event = strchr(&argv[0][1], ':');
	if (event)
		event++;

	if (isdigit(argv[0][1])) {
		if (!is_return) {
			trace_probe_log_err(1, MAXACT_NO_KPROBE);
			goto parse_error;
		}
		if (event)
			len = event - &argv[0][1] - 1;
		else
			len = strlen(&argv[0][1]);
		if (len > MAX_EVENT_NAME_LEN - 1) {
			trace_probe_log_err(1, BAD_MAXACT);
			goto parse_error;
		}
		memcpy(buf, &argv[0][1], len);
		buf[len] = '\0';
		ret = kstrtouint(buf, 0, &maxactive);
		if (ret || !maxactive) {
			trace_probe_log_err(1, BAD_MAXACT);
			goto parse_error;
>>>>>>> upstream/android-13
		}
		/* kretprobes instances are iterated over via a list. The
		 * maximum should stay reasonable.
		 */
		if (maxactive > KRETPROBE_MAXACTIVE_MAX) {
<<<<<<< HEAD
			pr_info("Maxactive is too big (%d > %d).\n",
				maxactive, KRETPROBE_MAXACTIVE_MAX);
			return -E2BIG;
		}
	}

	if (event) {
		if (strchr(event, '/')) {
			group = event;
			event = strchr(group, '/') + 1;
			event[-1] = '\0';
			if (strlen(group) == 0) {
				pr_info("Group name is not specified\n");
				return -EINVAL;
			}
		}
		if (strlen(event) == 0) {
			pr_info("Event name is not specified\n");
			return -EINVAL;
		}
	}
	if (!group)
		group = KPROBE_EVENT_SYSTEM;

	if (is_delete) {
		if (!event) {
			pr_info("Delete command needs an event name.\n");
			return -EINVAL;
		}
		mutex_lock(&probe_lock);
		tk = find_trace_kprobe(event, group);
		if (!tk) {
			mutex_unlock(&probe_lock);
			pr_info("Event %s/%s doesn't exist.\n", group, event);
			return -ENOENT;
		}
		/* delete an event */
		ret = unregister_trace_kprobe(tk);
		if (ret == 0)
			free_trace_kprobe(tk);
		mutex_unlock(&probe_lock);
		return ret;
	}

	if (argc < 2) {
		pr_info("Probe point is not specified.\n");
		return -EINVAL;
	}

	/* try to parse an address. if that fails, try to read the
	 * input as a symbol. */
	if (kstrtoul(argv[1], 0, (unsigned long *)&addr)) {
		/* a symbol specified */
		symbol = argv[1];
		/* TODO: support .init module functions */
		ret = traceprobe_split_symbol_offset(symbol, &offset);
		if (ret || offset < 0 || offset > UINT_MAX) {
			pr_info("Failed to parse either an address or a symbol.\n");
			return ret;
		}
		if (offset && is_return &&
		    !kprobe_on_func_entry(NULL, symbol, offset)) {
			pr_info("Given offset is not valid for return probe.\n");
			return -EINVAL;
		}
	}
	argc -= 2; argv += 2;

	/* setup a probe */
	if (!event) {
=======
			trace_probe_log_err(1, MAXACT_TOO_BIG);
			goto parse_error;
		}
	}

	/* try to parse an address. if that fails, try to read the
	 * input as a symbol. */
	if (kstrtoul(argv[1], 0, (unsigned long *)&addr)) {
		trace_probe_log_set_index(1);
		/* Check whether uprobe event specified */
		if (strchr(argv[1], '/') && strchr(argv[1], ':')) {
			ret = -ECANCELED;
			goto error;
		}
		/* a symbol specified */
		symbol = kstrdup(argv[1], GFP_KERNEL);
		if (!symbol)
			return -ENOMEM;

		tmp = strchr(symbol, '%');
		if (tmp) {
			if (!strcmp(tmp, "%return")) {
				*tmp = '\0';
				is_return = true;
			} else {
				trace_probe_log_err(tmp - symbol, BAD_ADDR_SUFFIX);
				goto parse_error;
			}
		}

		/* TODO: support .init module functions */
		ret = traceprobe_split_symbol_offset(symbol, &offset);
		if (ret || offset < 0 || offset > UINT_MAX) {
			trace_probe_log_err(0, BAD_PROBE_ADDR);
			goto parse_error;
		}
		if (is_return)
			flags |= TPARG_FL_RETURN;
		ret = kprobe_on_func_entry(NULL, symbol, offset);
		if (ret == 0)
			flags |= TPARG_FL_FENTRY;
		/* Defer the ENOENT case until register kprobe */
		if (ret == -EINVAL && is_return) {
			trace_probe_log_err(0, BAD_RETPROBE);
			goto parse_error;
		}
	}

	trace_probe_log_set_index(0);
	if (event) {
		ret = traceprobe_parse_event_name(&event, &group, buf,
						  event - argv[0]);
		if (ret)
			goto parse_error;
	} else {
>>>>>>> upstream/android-13
		/* Make a new event name */
		if (symbol)
			snprintf(buf, MAX_EVENT_NAME_LEN, "%c_%s_%ld",
				 is_return ? 'r' : 'p', symbol, offset);
		else
			snprintf(buf, MAX_EVENT_NAME_LEN, "%c_0x%p",
				 is_return ? 'r' : 'p', addr);
		sanitize_event_name(buf);
		event = buf;
	}
<<<<<<< HEAD
	tk = alloc_trace_kprobe(group, event, addr, symbol, offset, maxactive,
			       argc, is_return);
	if (IS_ERR(tk)) {
		pr_info("Failed to allocate trace_probe.(%d)\n",
			(int)PTR_ERR(tk));
		return PTR_ERR(tk);
	}

	/* parse arguments */
	ret = 0;
	for (i = 0; i < argc && i < MAX_TRACE_ARGS; i++) {
		struct probe_arg *parg = &tk->tp.args[i];

		/* Increment count for freeing args in error case */
		tk->tp.nr_args++;

		/* Parse argument name */
		arg = strchr(argv[i], '=');
		if (arg) {
			*arg++ = '\0';
			parg->name = kstrdup(argv[i], GFP_KERNEL);
		} else {
			arg = argv[i];
			/* If argument name is omitted, set "argN" */
			snprintf(buf, MAX_EVENT_NAME_LEN, "arg%d", i + 1);
			parg->name = kstrdup(buf, GFP_KERNEL);
		}

		if (!parg->name) {
			pr_info("Failed to allocate argument[%d] name.\n", i);
			ret = -ENOMEM;
			goto error;
		}

		if (!is_good_name(parg->name)) {
			pr_info("Invalid argument[%d] name: %s\n",
				i, parg->name);
			ret = -EINVAL;
			goto error;
		}

		if (traceprobe_conflict_field_name(parg->name,
							tk->tp.args, i)) {
			pr_info("Argument[%d] name '%s' conflicts with "
				"another field.\n", i, argv[i]);
			ret = -EINVAL;
			goto error;
		}

		/* Parse fetch argument */
		ret = traceprobe_parse_probe_arg(arg, &tk->tp.size, parg,
						is_return, true,
						kprobes_fetch_type_table);
		if (ret) {
			pr_info("Parse error at argument[%d]. (%d)\n", i, ret);
			goto error;
		}
	}

	ret = register_trace_kprobe(tk);
	if (ret)
		goto error;
	return 0;

error:
	free_trace_kprobe(tk);
	return ret;
}

static int release_all_trace_kprobes(void)
{
	struct trace_kprobe *tk;
	int ret = 0;

	mutex_lock(&probe_lock);
	/* Ensure no probe is in use. */
	list_for_each_entry(tk, &probe_list, list)
		if (trace_probe_is_enabled(&tk->tp)) {
			ret = -EBUSY;
			goto end;
		}
	/* TODO: Use batch unregistration */
	while (!list_empty(&probe_list)) {
		tk = list_entry(probe_list.next, struct trace_kprobe, list);
		ret = unregister_trace_kprobe(tk);
		if (ret)
			goto end;
		free_trace_kprobe(tk);
	}

end:
	mutex_unlock(&probe_lock);

	return ret;
}

/* Probes listing interfaces */
static void *probes_seq_start(struct seq_file *m, loff_t *pos)
{
	mutex_lock(&probe_lock);
	return seq_list_start(&probe_list, *pos);
}

static void *probes_seq_next(struct seq_file *m, void *v, loff_t *pos)
{
	return seq_list_next(v, &probe_list, pos);
}

static void probes_seq_stop(struct seq_file *m, void *v)
{
	mutex_unlock(&probe_lock);
}

static int probes_seq_show(struct seq_file *m, void *v)
{
	struct trace_kprobe *tk = v;
=======

	/* setup a probe */
	tk = alloc_trace_kprobe(group, event, addr, symbol, offset, maxactive,
			       argc - 2, is_return);
	if (IS_ERR(tk)) {
		ret = PTR_ERR(tk);
		/* This must return -ENOMEM, else there is a bug */
		WARN_ON_ONCE(ret != -ENOMEM);
		goto out;	/* We know tk is not allocated */
	}
	argc -= 2; argv += 2;

	/* parse arguments */
	for (i = 0; i < argc && i < MAX_TRACE_ARGS; i++) {
		trace_probe_log_set_index(i + 2);
		ret = traceprobe_parse_probe_arg(&tk->tp, i, argv[i], flags);
		if (ret)
			goto error;	/* This can be -ENOMEM */
	}

	ptype = is_return ? PROBE_PRINT_RETURN : PROBE_PRINT_NORMAL;
	ret = traceprobe_set_print_fmt(&tk->tp, ptype);
	if (ret < 0)
		goto error;

	ret = register_trace_kprobe(tk);
	if (ret) {
		trace_probe_log_set_index(1);
		if (ret == -EILSEQ)
			trace_probe_log_err(0, BAD_INSN_BNDRY);
		else if (ret == -ENOENT)
			trace_probe_log_err(0, BAD_PROBE_ADDR);
		else if (ret != -ENOMEM && ret != -EEXIST)
			trace_probe_log_err(0, FAIL_REG_PROBE);
		goto error;
	}

out:
	trace_probe_log_clear();
	kfree(symbol);
	return ret;

parse_error:
	ret = -EINVAL;
error:
	free_trace_kprobe(tk);
	goto out;
}

static int trace_kprobe_create(const char *raw_command)
{
	return trace_probe_create(raw_command, __trace_kprobe_create);
}

static int create_or_delete_trace_kprobe(const char *raw_command)
{
	int ret;

	if (raw_command[0] == '-')
		return dyn_event_release(raw_command, &trace_kprobe_ops);

	ret = trace_kprobe_create(raw_command);
	return ret == -ECANCELED ? -EINVAL : ret;
}

static int trace_kprobe_run_command(struct dynevent_cmd *cmd)
{
	return create_or_delete_trace_kprobe(cmd->seq.buffer);
}

/**
 * kprobe_event_cmd_init - Initialize a kprobe event command object
 * @cmd: A pointer to the dynevent_cmd struct representing the new event
 * @buf: A pointer to the buffer used to build the command
 * @maxlen: The length of the buffer passed in @buf
 *
 * Initialize a synthetic event command object.  Use this before
 * calling any of the other kprobe_event functions.
 */
void kprobe_event_cmd_init(struct dynevent_cmd *cmd, char *buf, int maxlen)
{
	dynevent_cmd_init(cmd, buf, maxlen, DYNEVENT_TYPE_KPROBE,
			  trace_kprobe_run_command);
}
EXPORT_SYMBOL_GPL(kprobe_event_cmd_init);

/**
 * __kprobe_event_gen_cmd_start - Generate a kprobe event command from arg list
 * @cmd: A pointer to the dynevent_cmd struct representing the new event
 * @name: The name of the kprobe event
 * @loc: The location of the kprobe event
 * @kretprobe: Is this a return probe?
 * @args: Variable number of arg (pairs), one pair for each field
 *
 * NOTE: Users normally won't want to call this function directly, but
 * rather use the kprobe_event_gen_cmd_start() wrapper, which automatically
 * adds a NULL to the end of the arg list.  If this function is used
 * directly, make sure the last arg in the variable arg list is NULL.
 *
 * Generate a kprobe event command to be executed by
 * kprobe_event_gen_cmd_end().  This function can be used to generate the
 * complete command or only the first part of it; in the latter case,
 * kprobe_event_add_fields() can be used to add more fields following this.
 *
 * Unlikely the synth_event_gen_cmd_start(), @loc must be specified. This
 * returns -EINVAL if @loc == NULL.
 *
 * Return: 0 if successful, error otherwise.
 */
int __kprobe_event_gen_cmd_start(struct dynevent_cmd *cmd, bool kretprobe,
				 const char *name, const char *loc, ...)
{
	char buf[MAX_EVENT_NAME_LEN];
	struct dynevent_arg arg;
	va_list args;
	int ret;

	if (cmd->type != DYNEVENT_TYPE_KPROBE)
		return -EINVAL;

	if (!loc)
		return -EINVAL;

	if (kretprobe)
		snprintf(buf, MAX_EVENT_NAME_LEN, "r:kprobes/%s", name);
	else
		snprintf(buf, MAX_EVENT_NAME_LEN, "p:kprobes/%s", name);

	ret = dynevent_str_add(cmd, buf);
	if (ret)
		return ret;

	dynevent_arg_init(&arg, 0);
	arg.str = loc;
	ret = dynevent_arg_add(cmd, &arg, NULL);
	if (ret)
		return ret;

	va_start(args, loc);
	for (;;) {
		const char *field;

		field = va_arg(args, const char *);
		if (!field)
			break;

		if (++cmd->n_fields > MAX_TRACE_ARGS) {
			ret = -EINVAL;
			break;
		}

		arg.str = field;
		ret = dynevent_arg_add(cmd, &arg, NULL);
		if (ret)
			break;
	}
	va_end(args);

	return ret;
}
EXPORT_SYMBOL_GPL(__kprobe_event_gen_cmd_start);

/**
 * __kprobe_event_add_fields - Add probe fields to a kprobe command from arg list
 * @cmd: A pointer to the dynevent_cmd struct representing the new event
 * @args: Variable number of arg (pairs), one pair for each field
 *
 * NOTE: Users normally won't want to call this function directly, but
 * rather use the kprobe_event_add_fields() wrapper, which
 * automatically adds a NULL to the end of the arg list.  If this
 * function is used directly, make sure the last arg in the variable
 * arg list is NULL.
 *
 * Add probe fields to an existing kprobe command using a variable
 * list of args.  Fields are added in the same order they're listed.
 *
 * Return: 0 if successful, error otherwise.
 */
int __kprobe_event_add_fields(struct dynevent_cmd *cmd, ...)
{
	struct dynevent_arg arg;
	va_list args;
	int ret = 0;

	if (cmd->type != DYNEVENT_TYPE_KPROBE)
		return -EINVAL;

	dynevent_arg_init(&arg, 0);

	va_start(args, cmd);
	for (;;) {
		const char *field;

		field = va_arg(args, const char *);
		if (!field)
			break;

		if (++cmd->n_fields > MAX_TRACE_ARGS) {
			ret = -EINVAL;
			break;
		}

		arg.str = field;
		ret = dynevent_arg_add(cmd, &arg, NULL);
		if (ret)
			break;
	}
	va_end(args);

	return ret;
}
EXPORT_SYMBOL_GPL(__kprobe_event_add_fields);

/**
 * kprobe_event_delete - Delete a kprobe event
 * @name: The name of the kprobe event to delete
 *
 * Delete a kprobe event with the give @name from kernel code rather
 * than directly from the command line.
 *
 * Return: 0 if successful, error otherwise.
 */
int kprobe_event_delete(const char *name)
{
	char buf[MAX_EVENT_NAME_LEN];

	snprintf(buf, MAX_EVENT_NAME_LEN, "-:%s", name);

	return create_or_delete_trace_kprobe(buf);
}
EXPORT_SYMBOL_GPL(kprobe_event_delete);

static int trace_kprobe_release(struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);
	int ret = unregister_trace_kprobe(tk);

	if (!ret)
		free_trace_kprobe(tk);
	return ret;
}

static int trace_kprobe_show(struct seq_file *m, struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);
>>>>>>> upstream/android-13
	int i;

	seq_putc(m, trace_kprobe_is_return(tk) ? 'r' : 'p');
	if (trace_kprobe_is_return(tk) && tk->rp.maxactive)
		seq_printf(m, "%d", tk->rp.maxactive);
<<<<<<< HEAD
	seq_printf(m, ":%s/%s", tk->tp.call.class->system,
			trace_event_name(&tk->tp.call));
=======
	seq_printf(m, ":%s/%s", trace_probe_group_name(&tk->tp),
				trace_probe_name(&tk->tp));
>>>>>>> upstream/android-13

	if (!tk->symbol)
		seq_printf(m, " 0x%p", tk->rp.kp.addr);
	else if (tk->rp.kp.offset)
		seq_printf(m, " %s+%u", trace_kprobe_symbol(tk),
			   tk->rp.kp.offset);
	else
		seq_printf(m, " %s", trace_kprobe_symbol(tk));

	for (i = 0; i < tk->tp.nr_args; i++)
		seq_printf(m, " %s=%s", tk->tp.args[i].name, tk->tp.args[i].comm);
	seq_putc(m, '\n');

	return 0;
}

<<<<<<< HEAD
static const struct seq_operations probes_seq_op = {
	.start  = probes_seq_start,
	.next   = probes_seq_next,
	.stop   = probes_seq_stop,
=======
static int probes_seq_show(struct seq_file *m, void *v)
{
	struct dyn_event *ev = v;

	if (!is_trace_kprobe(ev))
		return 0;

	return trace_kprobe_show(m, ev);
}

static const struct seq_operations probes_seq_op = {
	.start  = dyn_event_seq_start,
	.next   = dyn_event_seq_next,
	.stop   = dyn_event_seq_stop,
>>>>>>> upstream/android-13
	.show   = probes_seq_show
};

static int probes_open(struct inode *inode, struct file *file)
{
	int ret;

<<<<<<< HEAD
	if ((file->f_mode & FMODE_WRITE) && (file->f_flags & O_TRUNC)) {
		ret = release_all_trace_kprobes();
=======
	ret = security_locked_down(LOCKDOWN_TRACEFS);
	if (ret)
		return ret;

	if ((file->f_mode & FMODE_WRITE) && (file->f_flags & O_TRUNC)) {
		ret = dyn_events_release_all(&trace_kprobe_ops);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;
	}

	return seq_open(file, &probes_seq_op);
}

static ssize_t probes_write(struct file *file, const char __user *buffer,
			    size_t count, loff_t *ppos)
{
	return trace_parse_run_command(file, buffer, count, ppos,
<<<<<<< HEAD
				       create_trace_kprobe);
=======
				       create_or_delete_trace_kprobe);
>>>>>>> upstream/android-13
}

static const struct file_operations kprobe_events_ops = {
	.owner          = THIS_MODULE,
	.open           = probes_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
	.write		= probes_write,
};

/* Probes profiling interfaces */
static int probes_profile_seq_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
	struct trace_kprobe *tk = v;

	seq_printf(m, "  %-44s %15lu %15lu\n",
		   trace_event_name(&tk->tp.call),
		   trace_kprobe_nhit(tk),
		   tk->rp.kp.nmissed);
=======
	struct dyn_event *ev = v;
	struct trace_kprobe *tk;
	unsigned long nmissed;

	if (!is_trace_kprobe(ev))
		return 0;

	tk = to_trace_kprobe(ev);
	nmissed = trace_kprobe_is_return(tk) ?
		tk->rp.kp.nmissed + tk->rp.nmissed : tk->rp.kp.nmissed;
	seq_printf(m, "  %-44s %15lu %15lu\n",
		   trace_probe_name(&tk->tp),
		   trace_kprobe_nhit(tk),
		   nmissed);
>>>>>>> upstream/android-13

	return 0;
}

static const struct seq_operations profile_seq_op = {
<<<<<<< HEAD
	.start  = probes_seq_start,
	.next   = probes_seq_next,
	.stop   = probes_seq_stop,
=======
	.start  = dyn_event_seq_start,
	.next   = dyn_event_seq_next,
	.stop   = dyn_event_seq_stop,
>>>>>>> upstream/android-13
	.show   = probes_profile_seq_show
};

static int profile_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
=======
	int ret;

	ret = security_locked_down(LOCKDOWN_TRACEFS);
	if (ret)
		return ret;

>>>>>>> upstream/android-13
	return seq_open(file, &profile_seq_op);
}

static const struct file_operations kprobe_profile_ops = {
	.owner          = THIS_MODULE,
	.open           = profile_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
};

<<<<<<< HEAD
=======
/* Kprobe specific fetch functions */

/* Return the length of string -- including null terminal byte */
static nokprobe_inline int
fetch_store_strlen_user(unsigned long addr)
{
	const void __user *uaddr =  (__force const void __user *)addr;

	return strnlen_user_nofault(uaddr, MAX_STRING_SIZE);
}

/* Return the length of string -- including null terminal byte */
static nokprobe_inline int
fetch_store_strlen(unsigned long addr)
{
	int ret, len = 0;
	u8 c;

#ifdef CONFIG_ARCH_HAS_NON_OVERLAPPING_ADDRESS_SPACE
	if (addr < TASK_SIZE)
		return fetch_store_strlen_user(addr);
#endif

	do {
		ret = copy_from_kernel_nofault(&c, (u8 *)addr + len, 1);
		len++;
	} while (c && ret == 0 && len < MAX_STRING_SIZE);

	return (ret < 0) ? ret : len;
}

/*
 * Fetch a null-terminated string from user. Caller MUST set *(u32 *)buf
 * with max length and relative data location.
 */
static nokprobe_inline int
fetch_store_string_user(unsigned long addr, void *dest, void *base)
{
	const void __user *uaddr =  (__force const void __user *)addr;
	int maxlen = get_loc_len(*(u32 *)dest);
	void *__dest;
	long ret;

	if (unlikely(!maxlen))
		return -ENOMEM;

	__dest = get_loc_data(dest, base);

	ret = strncpy_from_user_nofault(__dest, uaddr, maxlen);
	if (ret >= 0)
		*(u32 *)dest = make_data_loc(ret, __dest - base);

	return ret;
}

/*
 * Fetch a null-terminated string. Caller MUST set *(u32 *)buf with max
 * length and relative data location.
 */
static nokprobe_inline int
fetch_store_string(unsigned long addr, void *dest, void *base)
{
	int maxlen = get_loc_len(*(u32 *)dest);
	void *__dest;
	long ret;

#ifdef CONFIG_ARCH_HAS_NON_OVERLAPPING_ADDRESS_SPACE
	if ((unsigned long)addr < TASK_SIZE)
		return fetch_store_string_user(addr, dest, base);
#endif

	if (unlikely(!maxlen))
		return -ENOMEM;

	__dest = get_loc_data(dest, base);

	/*
	 * Try to get string again, since the string can be changed while
	 * probing.
	 */
	ret = strncpy_from_kernel_nofault(__dest, (void *)addr, maxlen);
	if (ret >= 0)
		*(u32 *)dest = make_data_loc(ret, __dest - base);

	return ret;
}

static nokprobe_inline int
probe_mem_read_user(void *dest, void *src, size_t size)
{
	const void __user *uaddr =  (__force const void __user *)src;

	return copy_from_user_nofault(dest, uaddr, size);
}

static nokprobe_inline int
probe_mem_read(void *dest, void *src, size_t size)
{
#ifdef CONFIG_ARCH_HAS_NON_OVERLAPPING_ADDRESS_SPACE
	if ((unsigned long)src < TASK_SIZE)
		return probe_mem_read_user(dest, src, size);
#endif
	return copy_from_kernel_nofault(dest, src, size);
}

/* Note that we don't verify it, since the code does not come from user space */
static int
process_fetch_insn(struct fetch_insn *code, void *rec, void *dest,
		   void *base)
{
	struct pt_regs *regs = rec;
	unsigned long val;

retry:
	/* 1st stage: get value from context */
	switch (code->op) {
	case FETCH_OP_REG:
		val = regs_get_register(regs, code->param);
		break;
	case FETCH_OP_STACK:
		val = regs_get_kernel_stack_nth(regs, code->param);
		break;
	case FETCH_OP_STACKP:
		val = kernel_stack_pointer(regs);
		break;
	case FETCH_OP_RETVAL:
		val = regs_return_value(regs);
		break;
	case FETCH_OP_IMM:
		val = code->immediate;
		break;
	case FETCH_OP_COMM:
		val = (unsigned long)current->comm;
		break;
	case FETCH_OP_DATA:
		val = (unsigned long)code->data;
		break;
#ifdef CONFIG_HAVE_FUNCTION_ARG_ACCESS_API
	case FETCH_OP_ARG:
		val = regs_get_kernel_argument(regs, code->param);
		break;
#endif
	case FETCH_NOP_SYMBOL:	/* Ignore a place holder */
		code++;
		goto retry;
	default:
		return -EILSEQ;
	}
	code++;

	return process_fetch_insn_bottom(code, val, dest, base);
}
NOKPROBE_SYMBOL(process_fetch_insn)

>>>>>>> upstream/android-13
/* Kprobe handler */
static nokprobe_inline void
__kprobe_trace_func(struct trace_kprobe *tk, struct pt_regs *regs,
		    struct trace_event_file *trace_file)
{
	struct kprobe_trace_entry_head *entry;
<<<<<<< HEAD
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size, dsize, pc;
	unsigned long irq_flags;
	struct trace_event_call *call = &tk->tp.call;
=======
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
	struct trace_event_buffer fbuffer;
	int dsize;
>>>>>>> upstream/android-13

	WARN_ON(call != trace_file->event_call);

	if (trace_trigger_soft_disabled(trace_file))
		return;

<<<<<<< HEAD
	local_save_flags(irq_flags);
	pc = preempt_count();

	dsize = __get_data_size(&tk->tp, regs);
	size = sizeof(*entry) + tk->tp.size + dsize;

	event = trace_event_buffer_lock_reserve(&buffer, trace_file,
						call->event.type,
						size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->ip = (unsigned long)tk->rp.kp.addr;
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);

	event_trigger_unlock_commit_regs(trace_file, buffer, event,
					 entry, irq_flags, pc, regs);
=======
	fbuffer.trace_ctx = tracing_gen_ctx();
	fbuffer.trace_file = trace_file;

	dsize = __get_data_size(&tk->tp, regs);

	fbuffer.event =
		trace_event_buffer_lock_reserve(&fbuffer.buffer, trace_file,
					call->event.type,
					sizeof(*entry) + tk->tp.size + dsize,
					fbuffer.trace_ctx);
	if (!fbuffer.event)
		return;

	fbuffer.regs = regs;
	entry = fbuffer.entry = ring_buffer_event_data(fbuffer.event);
	entry->ip = (unsigned long)tk->rp.kp.addr;
	store_trace_args(&entry[1], &tk->tp, regs, sizeof(*entry), dsize);

	trace_event_buffer_commit(&fbuffer);
>>>>>>> upstream/android-13
}

static void
kprobe_trace_func(struct trace_kprobe *tk, struct pt_regs *regs)
{
	struct event_file_link *link;

<<<<<<< HEAD
	list_for_each_entry_rcu(link, &tk->tp.files, list)
=======
	trace_probe_for_each_link_rcu(link, &tk->tp)
>>>>>>> upstream/android-13
		__kprobe_trace_func(tk, regs, link->file);
}
NOKPROBE_SYMBOL(kprobe_trace_func);

/* Kretprobe handler */
static nokprobe_inline void
__kretprobe_trace_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		       struct pt_regs *regs,
		       struct trace_event_file *trace_file)
{
	struct kretprobe_trace_entry_head *entry;
<<<<<<< HEAD
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size, pc, dsize;
	unsigned long irq_flags;
	struct trace_event_call *call = &tk->tp.call;
=======
	struct trace_event_buffer fbuffer;
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
	int dsize;
>>>>>>> upstream/android-13

	WARN_ON(call != trace_file->event_call);

	if (trace_trigger_soft_disabled(trace_file))
		return;

<<<<<<< HEAD
	local_save_flags(irq_flags);
	pc = preempt_count();

	dsize = __get_data_size(&tk->tp, regs);
	size = sizeof(*entry) + tk->tp.size + dsize;

	event = trace_event_buffer_lock_reserve(&buffer, trace_file,
						call->event.type,
						size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);

	event_trigger_unlock_commit_regs(trace_file, buffer, event,
					 entry, irq_flags, pc, regs);
=======
	fbuffer.trace_ctx = tracing_gen_ctx();
	fbuffer.trace_file = trace_file;

	dsize = __get_data_size(&tk->tp, regs);
	fbuffer.event =
		trace_event_buffer_lock_reserve(&fbuffer.buffer, trace_file,
					call->event.type,
					sizeof(*entry) + tk->tp.size + dsize,
					fbuffer.trace_ctx);
	if (!fbuffer.event)
		return;

	fbuffer.regs = regs;
	entry = fbuffer.entry = ring_buffer_event_data(fbuffer.event);
	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(&entry[1], &tk->tp, regs, sizeof(*entry), dsize);

	trace_event_buffer_commit(&fbuffer);
>>>>>>> upstream/android-13
}

static void
kretprobe_trace_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		     struct pt_regs *regs)
{
	struct event_file_link *link;

<<<<<<< HEAD
	list_for_each_entry_rcu(link, &tk->tp.files, list)
=======
	trace_probe_for_each_link_rcu(link, &tk->tp)
>>>>>>> upstream/android-13
		__kretprobe_trace_func(tk, ri, regs, link->file);
}
NOKPROBE_SYMBOL(kretprobe_trace_func);

/* Event entry printers */
static enum print_line_t
print_kprobe_event(struct trace_iterator *iter, int flags,
		   struct trace_event *event)
{
	struct kprobe_trace_entry_head *field;
	struct trace_seq *s = &iter->seq;
	struct trace_probe *tp;
<<<<<<< HEAD
	u8 *data;
	int i;

	field = (struct kprobe_trace_entry_head *)iter->ent;
	tp = container_of(event, struct trace_probe, call.event);

	trace_seq_printf(s, "%s: (", trace_event_name(&tp->call));
=======

	field = (struct kprobe_trace_entry_head *)iter->ent;
	tp = trace_probe_primary_from_call(
		container_of(event, struct trace_event_call, event));
	if (WARN_ON_ONCE(!tp))
		goto out;

	trace_seq_printf(s, "%s: (", trace_probe_name(tp));
>>>>>>> upstream/android-13

	if (!seq_print_ip_sym(s, field->ip, flags | TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_putc(s, ')');

<<<<<<< HEAD
	data = (u8 *)&field[1];
	for (i = 0; i < tp->nr_args; i++)
		if (!tp->args[i].type->print(s, tp->args[i].name,
					     data + tp->args[i].offset, field))
			goto out;
=======
	if (print_probe_args(s, tp->args, tp->nr_args,
			     (u8 *)&field[1], field) < 0)
		goto out;
>>>>>>> upstream/android-13

	trace_seq_putc(s, '\n');
 out:
	return trace_handle_return(s);
}

static enum print_line_t
print_kretprobe_event(struct trace_iterator *iter, int flags,
		      struct trace_event *event)
{
	struct kretprobe_trace_entry_head *field;
	struct trace_seq *s = &iter->seq;
	struct trace_probe *tp;
<<<<<<< HEAD
	u8 *data;
	int i;

	field = (struct kretprobe_trace_entry_head *)iter->ent;
	tp = container_of(event, struct trace_probe, call.event);

	trace_seq_printf(s, "%s: (", trace_event_name(&tp->call));
=======

	field = (struct kretprobe_trace_entry_head *)iter->ent;
	tp = trace_probe_primary_from_call(
		container_of(event, struct trace_event_call, event));
	if (WARN_ON_ONCE(!tp))
		goto out;

	trace_seq_printf(s, "%s: (", trace_probe_name(tp));
>>>>>>> upstream/android-13

	if (!seq_print_ip_sym(s, field->ret_ip, flags | TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_puts(s, " <- ");

	if (!seq_print_ip_sym(s, field->func, flags & ~TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_putc(s, ')');

<<<<<<< HEAD
	data = (u8 *)&field[1];
	for (i = 0; i < tp->nr_args; i++)
		if (!tp->args[i].type->print(s, tp->args[i].name,
					     data + tp->args[i].offset, field))
			goto out;
=======
	if (print_probe_args(s, tp->args, tp->nr_args,
			     (u8 *)&field[1], field) < 0)
		goto out;
>>>>>>> upstream/android-13

	trace_seq_putc(s, '\n');

 out:
	return trace_handle_return(s);
}


static int kprobe_event_define_fields(struct trace_event_call *event_call)
{
<<<<<<< HEAD
	int ret, i;
	struct kprobe_trace_entry_head field;
	struct trace_kprobe *tk = (struct trace_kprobe *)event_call->data;

	DEFINE_FIELD(unsigned long, ip, FIELD_STRING_IP, 0);
	/* Set argument names as fields */
	for (i = 0; i < tk->tp.nr_args; i++) {
		struct probe_arg *parg = &tk->tp.args[i];

		ret = trace_define_field(event_call, parg->type->fmttype,
					 parg->name,
					 sizeof(field) + parg->offset,
					 parg->type->size,
					 parg->type->is_signed,
					 FILTER_OTHER);
		if (ret)
			return ret;
	}
	return 0;
=======
	int ret;
	struct kprobe_trace_entry_head field;
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(event_call);
	if (WARN_ON_ONCE(!tp))
		return -ENOENT;

	DEFINE_FIELD(unsigned long, ip, FIELD_STRING_IP, 0);

	return traceprobe_define_arg_fields(event_call, sizeof(field), tp);
>>>>>>> upstream/android-13
}

static int kretprobe_event_define_fields(struct trace_event_call *event_call)
{
<<<<<<< HEAD
	int ret, i;
	struct kretprobe_trace_entry_head field;
	struct trace_kprobe *tk = (struct trace_kprobe *)event_call->data;

	DEFINE_FIELD(unsigned long, func, FIELD_STRING_FUNC, 0);
	DEFINE_FIELD(unsigned long, ret_ip, FIELD_STRING_RETIP, 0);
	/* Set argument names as fields */
	for (i = 0; i < tk->tp.nr_args; i++) {
		struct probe_arg *parg = &tk->tp.args[i];

		ret = trace_define_field(event_call, parg->type->fmttype,
					 parg->name,
					 sizeof(field) + parg->offset,
					 parg->type->size,
					 parg->type->is_signed,
					 FILTER_OTHER);
		if (ret)
			return ret;
	}
	return 0;
=======
	int ret;
	struct kretprobe_trace_entry_head field;
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(event_call);
	if (WARN_ON_ONCE(!tp))
		return -ENOENT;

	DEFINE_FIELD(unsigned long, func, FIELD_STRING_FUNC, 0);
	DEFINE_FIELD(unsigned long, ret_ip, FIELD_STRING_RETIP, 0);

	return traceprobe_define_arg_fields(event_call, sizeof(field), tp);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PERF_EVENTS

/* Kprobe profile handler */
static int
kprobe_perf_func(struct trace_kprobe *tk, struct pt_regs *regs)
{
<<<<<<< HEAD
	struct trace_event_call *call = &tk->tp.call;
=======
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
>>>>>>> upstream/android-13
	struct kprobe_trace_entry_head *entry;
	struct hlist_head *head;
	int size, __size, dsize;
	int rctx;

	if (bpf_prog_array_valid(call)) {
		unsigned long orig_ip = instruction_pointer(regs);
		int ret;

		ret = trace_call_bpf(call, regs);

		/*
		 * We need to check and see if we modified the pc of the
		 * pt_regs, and if so return 1 so that we don't do the
		 * single stepping.
		 */
		if (orig_ip != instruction_pointer(regs))
			return 1;
		if (!ret)
			return 0;
	}

	head = this_cpu_ptr(call->perf_events);
	if (hlist_empty(head))
		return 0;

	dsize = __get_data_size(&tk->tp, regs);
	__size = sizeof(*entry) + tk->tp.size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

	entry = perf_trace_buf_alloc(size, NULL, &rctx);
	if (!entry)
		return 0;

	entry->ip = (unsigned long)tk->rp.kp.addr;
	memset(&entry[1], 0, dsize);
<<<<<<< HEAD
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);
=======
	store_trace_args(&entry[1], &tk->tp, regs, sizeof(*entry), dsize);
>>>>>>> upstream/android-13
	perf_trace_buf_submit(entry, size, rctx, call->event.type, 1, regs,
			      head, NULL);
	return 0;
}
NOKPROBE_SYMBOL(kprobe_perf_func);

/* Kretprobe profile handler */
static void
kretprobe_perf_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		    struct pt_regs *regs)
{
<<<<<<< HEAD
	struct trace_event_call *call = &tk->tp.call;
=======
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
>>>>>>> upstream/android-13
	struct kretprobe_trace_entry_head *entry;
	struct hlist_head *head;
	int size, __size, dsize;
	int rctx;

	if (bpf_prog_array_valid(call) && !trace_call_bpf(call, regs))
		return;

	head = this_cpu_ptr(call->perf_events);
	if (hlist_empty(head))
		return;

	dsize = __get_data_size(&tk->tp, regs);
	__size = sizeof(*entry) + tk->tp.size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

	entry = perf_trace_buf_alloc(size, NULL, &rctx);
	if (!entry)
		return;

	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
<<<<<<< HEAD
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);
=======
	store_trace_args(&entry[1], &tk->tp, regs, sizeof(*entry), dsize);
>>>>>>> upstream/android-13
	perf_trace_buf_submit(entry, size, rctx, call->event.type, 1, regs,
			      head, NULL);
}
NOKPROBE_SYMBOL(kretprobe_perf_func);

int bpf_get_kprobe_info(const struct perf_event *event, u32 *fd_type,
			const char **symbol, u64 *probe_offset,
			u64 *probe_addr, bool perf_type_tracepoint)
{
	const char *pevent = trace_event_name(event->tp_event);
	const char *group = event->tp_event->class->system;
	struct trace_kprobe *tk;

	if (perf_type_tracepoint)
		tk = find_trace_kprobe(pevent, group);
	else
<<<<<<< HEAD
		tk = event->tp_event->data;
=======
		tk = trace_kprobe_primary_from_call(event->tp_event);
>>>>>>> upstream/android-13
	if (!tk)
		return -EINVAL;

	*fd_type = trace_kprobe_is_return(tk) ? BPF_FD_TYPE_KRETPROBE
					      : BPF_FD_TYPE_KPROBE;
	if (tk->symbol) {
		*symbol = tk->symbol;
		*probe_offset = tk->rp.kp.offset;
		*probe_addr = 0;
	} else {
		*symbol = NULL;
		*probe_offset = 0;
		*probe_addr = (unsigned long)tk->rp.kp.addr;
	}
	return 0;
}
#endif	/* CONFIG_PERF_EVENTS */

/*
 * called by perf_trace_init() or __ftrace_set_clr_event() under event_mutex.
 *
 * kprobe_trace_self_tests_init() does enable_trace_probe/disable_trace_probe
 * lockless, but we can't race with this __init function.
 */
static int kprobe_register(struct trace_event_call *event,
			   enum trace_reg type, void *data)
{
<<<<<<< HEAD
	struct trace_kprobe *tk = (struct trace_kprobe *)event->data;
=======
>>>>>>> upstream/android-13
	struct trace_event_file *file = data;

	switch (type) {
	case TRACE_REG_REGISTER:
<<<<<<< HEAD
		return enable_trace_kprobe(tk, file);
	case TRACE_REG_UNREGISTER:
		return disable_trace_kprobe(tk, file);

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return enable_trace_kprobe(tk, NULL);
	case TRACE_REG_PERF_UNREGISTER:
		return disable_trace_kprobe(tk, NULL);
=======
		return enable_trace_kprobe(event, file);
	case TRACE_REG_UNREGISTER:
		return disable_trace_kprobe(event, file);

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return enable_trace_kprobe(event, NULL);
	case TRACE_REG_PERF_UNREGISTER:
		return disable_trace_kprobe(event, NULL);
>>>>>>> upstream/android-13
	case TRACE_REG_PERF_OPEN:
	case TRACE_REG_PERF_CLOSE:
	case TRACE_REG_PERF_ADD:
	case TRACE_REG_PERF_DEL:
		return 0;
#endif
	}
	return 0;
}

static int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs)
{
	struct trace_kprobe *tk = container_of(kp, struct trace_kprobe, rp.kp);
	int ret = 0;

	raw_cpu_inc(*tk->nhit);

<<<<<<< HEAD
	if (tk->tp.flags & TP_FLAG_TRACE)
		kprobe_trace_func(tk, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tk->tp.flags & TP_FLAG_PROFILE)
=======
	if (trace_probe_test_flag(&tk->tp, TP_FLAG_TRACE))
		kprobe_trace_func(tk, regs);
#ifdef CONFIG_PERF_EVENTS
	if (trace_probe_test_flag(&tk->tp, TP_FLAG_PROFILE))
>>>>>>> upstream/android-13
		ret = kprobe_perf_func(tk, regs);
#endif
	return ret;
}
NOKPROBE_SYMBOL(kprobe_dispatcher);

static int
kretprobe_dispatcher(struct kretprobe_instance *ri, struct pt_regs *regs)
{
<<<<<<< HEAD
	struct trace_kprobe *tk = container_of(ri->rp, struct trace_kprobe, rp);

	raw_cpu_inc(*tk->nhit);

	if (tk->tp.flags & TP_FLAG_TRACE)
		kretprobe_trace_func(tk, ri, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tk->tp.flags & TP_FLAG_PROFILE)
		kretprobe_perf_func(tk, ri, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
=======
	struct kretprobe *rp = get_kretprobe(ri);
	struct trace_kprobe *tk = container_of(rp, struct trace_kprobe, rp);

	raw_cpu_inc(*tk->nhit);

	if (trace_probe_test_flag(&tk->tp, TP_FLAG_TRACE))
		kretprobe_trace_func(tk, ri, regs);
#ifdef CONFIG_PERF_EVENTS
	if (trace_probe_test_flag(&tk->tp, TP_FLAG_PROFILE))
		kretprobe_perf_func(tk, ri, regs);
#endif
	return 0;	/* We don't tweak kernel, so just return 0 */
>>>>>>> upstream/android-13
}
NOKPROBE_SYMBOL(kretprobe_dispatcher);

static struct trace_event_functions kretprobe_funcs = {
	.trace		= print_kretprobe_event
};

static struct trace_event_functions kprobe_funcs = {
	.trace		= print_kprobe_event
};

<<<<<<< HEAD
static inline void init_trace_event_call(struct trace_kprobe *tk,
					 struct trace_event_call *call)
{
	INIT_LIST_HEAD(&call->class->fields);
	if (trace_kprobe_is_return(tk)) {
		call->event.funcs = &kretprobe_funcs;
		call->class->define_fields = kretprobe_event_define_fields;
	} else {
		call->event.funcs = &kprobe_funcs;
		call->class->define_fields = kprobe_event_define_fields;
=======
static struct trace_event_fields kretprobe_fields_array[] = {
	{ .type = TRACE_FUNCTION_TYPE,
	  .define_fields = kretprobe_event_define_fields },
	{}
};

static struct trace_event_fields kprobe_fields_array[] = {
	{ .type = TRACE_FUNCTION_TYPE,
	  .define_fields = kprobe_event_define_fields },
	{}
};

static inline void init_trace_event_call(struct trace_kprobe *tk)
{
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);

	if (trace_kprobe_is_return(tk)) {
		call->event.funcs = &kretprobe_funcs;
		call->class->fields_array = kretprobe_fields_array;
	} else {
		call->event.funcs = &kprobe_funcs;
		call->class->fields_array = kprobe_fields_array;
>>>>>>> upstream/android-13
	}

	call->flags = TRACE_EVENT_FL_KPROBE;
	call->class->reg = kprobe_register;
<<<<<<< HEAD
	call->data = tk;
=======
>>>>>>> upstream/android-13
}

static int register_kprobe_event(struct trace_kprobe *tk)
{
<<<<<<< HEAD
	struct trace_event_call *call = &tk->tp.call;
	int ret = 0;

	init_trace_event_call(tk, call);

	if (set_print_fmt(&tk->tp, trace_kprobe_is_return(tk)) < 0)
		return -ENOMEM;
	ret = register_trace_event(&call->event);
	if (!ret) {
		kfree(call->print_fmt);
		return -ENODEV;
	}
	ret = trace_add_event_call(call);
	if (ret) {
		pr_info("Failed to register kprobe event: %s\n",
			trace_event_name(call));
		kfree(call->print_fmt);
		unregister_trace_event(&call->event);
	}
	return ret;
=======
	init_trace_event_call(tk);

	return trace_probe_register_event_call(&tk->tp);
>>>>>>> upstream/android-13
}

static int unregister_kprobe_event(struct trace_kprobe *tk)
{
<<<<<<< HEAD
	int ret;

	/* tp->event is unregistered in trace_remove_event_call() */
	ret = trace_remove_event_call(&tk->tp.call);
	if (!ret)
		kfree(tk->tp.call.print_fmt);
	return ret;
=======
	return trace_probe_unregister_event_call(&tk->tp);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PERF_EVENTS
/* create a trace_kprobe, but don't add it to global lists */
struct trace_event_call *
create_local_trace_kprobe(char *func, void *addr, unsigned long offs,
			  bool is_return)
{
<<<<<<< HEAD
=======
	enum probe_print_type ptype;
>>>>>>> upstream/android-13
	struct trace_kprobe *tk;
	int ret;
	char *event;

	/*
<<<<<<< HEAD
	 * local trace_kprobes are not added to probe_list, so they are never
=======
	 * local trace_kprobes are not added to dyn_event, so they are never
>>>>>>> upstream/android-13
	 * searched in find_trace_kprobe(). Therefore, there is no concern of
	 * duplicated name here.
	 */
	event = func ? func : "DUMMY_EVENT";

	tk = alloc_trace_kprobe(KPROBE_EVENT_SYSTEM, event, (void *)addr, func,
				offs, 0 /* maxactive */, 0 /* nargs */,
				is_return);

	if (IS_ERR(tk)) {
		pr_info("Failed to allocate trace_probe.(%d)\n",
			(int)PTR_ERR(tk));
		return ERR_CAST(tk);
	}

<<<<<<< HEAD
	init_trace_event_call(tk, &tk->tp.call);

	if (set_print_fmt(&tk->tp, trace_kprobe_is_return(tk)) < 0) {
=======
	init_trace_event_call(tk);

	ptype = trace_kprobe_is_return(tk) ?
		PROBE_PRINT_RETURN : PROBE_PRINT_NORMAL;
	if (traceprobe_set_print_fmt(&tk->tp, ptype) < 0) {
>>>>>>> upstream/android-13
		ret = -ENOMEM;
		goto error;
	}

	ret = __register_trace_kprobe(tk);
<<<<<<< HEAD
	if (ret < 0) {
		kfree(tk->tp.call.print_fmt);
		goto error;
	}

	return &tk->tp.call;
=======
	if (ret < 0)
		goto error;

	return trace_probe_event_call(&tk->tp);
>>>>>>> upstream/android-13
error:
	free_trace_kprobe(tk);
	return ERR_PTR(ret);
}

void destroy_local_trace_kprobe(struct trace_event_call *event_call)
{
	struct trace_kprobe *tk;

<<<<<<< HEAD
	tk = container_of(event_call, struct trace_kprobe, tp.call);
=======
	tk = trace_kprobe_primary_from_call(event_call);
	if (unlikely(!tk))
		return;
>>>>>>> upstream/android-13

	if (trace_probe_is_enabled(&tk->tp)) {
		WARN_ON(1);
		return;
	}

	__unregister_trace_kprobe(tk);

<<<<<<< HEAD
	kfree(tk->tp.call.print_fmt);
=======
>>>>>>> upstream/android-13
	free_trace_kprobe(tk);
}
#endif /* CONFIG_PERF_EVENTS */

<<<<<<< HEAD
/* Make a tracefs interface for controlling probe points */
static __init int init_kprobe_trace(void)
{
	struct dentry *d_tracer;
	struct dentry *entry;
=======
static __init void enable_boot_kprobe_events(void)
{
	struct trace_array *tr = top_trace_array();
	struct trace_event_file *file;
	struct trace_kprobe *tk;
	struct dyn_event *pos;

	mutex_lock(&event_mutex);
	for_each_trace_kprobe(tk, pos) {
		list_for_each_entry(file, &tr->events, list)
			if (file->event_call == trace_probe_event_call(&tk->tp))
				trace_event_enable_disable(file, 1, 0);
	}
	mutex_unlock(&event_mutex);
}

static __init void setup_boot_kprobe_events(void)
{
	char *p, *cmd = kprobe_boot_events_buf;
	int ret;

	strreplace(kprobe_boot_events_buf, ',', ' ');

	while (cmd && *cmd != '\0') {
		p = strchr(cmd, ';');
		if (p)
			*p++ = '\0';

		ret = create_or_delete_trace_kprobe(cmd);
		if (ret)
			pr_warn("Failed to add event(%d): %s\n", ret, cmd);

		cmd = p;
	}

	enable_boot_kprobe_events();
}

/*
 * Register dynevent at core_initcall. This allows kernel to setup kprobe
 * events in postcore_initcall without tracefs.
 */
static __init int init_kprobe_trace_early(void)
{
	int ret;

	ret = dyn_event_register(&trace_kprobe_ops);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	if (register_module_notifier(&trace_kprobe_module_nb))
		return -EINVAL;

<<<<<<< HEAD
	d_tracer = tracing_init_dentry();
	if (IS_ERR(d_tracer))
		return 0;

	entry = tracefs_create_file("kprobe_events", 0644, d_tracer,
=======
	return 0;
}
core_initcall(init_kprobe_trace_early);

/* Make a tracefs interface for controlling probe points */
static __init int init_kprobe_trace(void)
{
	int ret;
	struct dentry *entry;

	ret = tracing_init_dentry();
	if (ret)
		return 0;

	entry = tracefs_create_file("kprobe_events", 0644, NULL,
>>>>>>> upstream/android-13
				    NULL, &kprobe_events_ops);

	/* Event list interface */
	if (!entry)
		pr_warn("Could not create tracefs 'kprobe_events' entry\n");

	/* Profile interface */
<<<<<<< HEAD
	entry = tracefs_create_file("kprobe_profile", 0444, d_tracer,
=======
	entry = tracefs_create_file("kprobe_profile", 0444, NULL,
>>>>>>> upstream/android-13
				    NULL, &kprobe_profile_ops);

	if (!entry)
		pr_warn("Could not create tracefs 'kprobe_profile' entry\n");
<<<<<<< HEAD
=======

	setup_boot_kprobe_events();

>>>>>>> upstream/android-13
	return 0;
}
fs_initcall(init_kprobe_trace);


#ifdef CONFIG_FTRACE_STARTUP_TEST
static __init struct trace_event_file *
find_trace_probe_file(struct trace_kprobe *tk, struct trace_array *tr)
{
	struct trace_event_file *file;

	list_for_each_entry(file, &tr->events, list)
<<<<<<< HEAD
		if (file->event_call == &tk->tp.call)
=======
		if (file->event_call == trace_probe_event_call(&tk->tp))
>>>>>>> upstream/android-13
			return file;

	return NULL;
}

/*
 * Nobody but us can call enable_trace_kprobe/disable_trace_kprobe at this
 * stage, we can do this lockless.
 */
static __init int kprobe_trace_self_tests_init(void)
{
	int ret, warn = 0;
	int (*target)(int, int, int, int, int, int);
	struct trace_kprobe *tk;
	struct trace_event_file *file;

	if (tracing_is_disabled())
		return -ENODEV;

<<<<<<< HEAD
=======
	if (tracing_selftest_disabled)
		return 0;

>>>>>>> upstream/android-13
	target = kprobe_trace_selftest_target;

	pr_info("Testing kprobe tracing: ");

<<<<<<< HEAD
	ret = trace_run_command("p:testprobe kprobe_trace_selftest_target "
				"$stack $stack0 +0($stack)",
				create_trace_kprobe);
=======
	ret = create_or_delete_trace_kprobe("p:testprobe kprobe_trace_selftest_target $stack $stack0 +0($stack)");
>>>>>>> upstream/android-13
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on probing function entry.\n");
		warn++;
	} else {
		/* Enable trace point */
		tk = find_trace_kprobe("testprobe", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tk == NULL)) {
			pr_warn("error on getting new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tk, top_trace_array());
			if (WARN_ON_ONCE(file == NULL)) {
				pr_warn("error on getting probe file.\n");
				warn++;
			} else
<<<<<<< HEAD
				enable_trace_kprobe(tk, file);
		}
	}

	ret = trace_run_command("r:testprobe2 kprobe_trace_selftest_target "
				"$retval", create_trace_kprobe);
=======
				enable_trace_kprobe(
					trace_probe_event_call(&tk->tp), file);
		}
	}

	ret = create_or_delete_trace_kprobe("r:testprobe2 kprobe_trace_selftest_target $retval");
>>>>>>> upstream/android-13
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on probing function return.\n");
		warn++;
	} else {
		/* Enable trace point */
		tk = find_trace_kprobe("testprobe2", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tk == NULL)) {
			pr_warn("error on getting 2nd new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tk, top_trace_array());
			if (WARN_ON_ONCE(file == NULL)) {
				pr_warn("error on getting probe file.\n");
				warn++;
			} else
<<<<<<< HEAD
				enable_trace_kprobe(tk, file);
=======
				enable_trace_kprobe(
					trace_probe_event_call(&tk->tp), file);
>>>>>>> upstream/android-13
		}
	}

	if (warn)
		goto end;

	ret = target(1, 2, 3, 4, 5, 6);

	/*
	 * Not expecting an error here, the check is only to prevent the
	 * optimizer from removing the call to target() as otherwise there
	 * are no side-effects and the call is never performed.
	 */
	if (ret != 21)
		warn++;

	/* Disable trace points before removing it */
	tk = find_trace_kprobe("testprobe", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tk == NULL)) {
		pr_warn("error on getting test probe.\n");
		warn++;
	} else {
		if (trace_kprobe_nhit(tk) != 1) {
			pr_warn("incorrect number of testprobe hits\n");
			warn++;
		}

		file = find_trace_probe_file(tk, top_trace_array());
		if (WARN_ON_ONCE(file == NULL)) {
			pr_warn("error on getting probe file.\n");
			warn++;
		} else
<<<<<<< HEAD
			disable_trace_kprobe(tk, file);
=======
			disable_trace_kprobe(
				trace_probe_event_call(&tk->tp), file);
>>>>>>> upstream/android-13
	}

	tk = find_trace_kprobe("testprobe2", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tk == NULL)) {
		pr_warn("error on getting 2nd test probe.\n");
		warn++;
	} else {
		if (trace_kprobe_nhit(tk) != 1) {
			pr_warn("incorrect number of testprobe2 hits\n");
			warn++;
		}

		file = find_trace_probe_file(tk, top_trace_array());
		if (WARN_ON_ONCE(file == NULL)) {
			pr_warn("error on getting probe file.\n");
			warn++;
		} else
<<<<<<< HEAD
			disable_trace_kprobe(tk, file);
	}

	ret = trace_run_command("-:testprobe", create_trace_kprobe);
=======
			disable_trace_kprobe(
				trace_probe_event_call(&tk->tp), file);
	}

	ret = create_or_delete_trace_kprobe("-:testprobe");
>>>>>>> upstream/android-13
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on deleting a probe.\n");
		warn++;
	}

<<<<<<< HEAD
	ret = trace_run_command("-:testprobe2", create_trace_kprobe);
=======
	ret = create_or_delete_trace_kprobe("-:testprobe2");
>>>>>>> upstream/android-13
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on deleting a probe.\n");
		warn++;
	}

end:
<<<<<<< HEAD
	release_all_trace_kprobes();
=======
	ret = dyn_events_release_all(&trace_kprobe_ops);
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on cleaning up probes.\n");
		warn++;
	}
>>>>>>> upstream/android-13
	/*
	 * Wait for the optimizer work to finish. Otherwise it might fiddle
	 * with probes in already freed __init text.
	 */
	wait_for_kprobe_optimizer();
	if (warn)
		pr_cont("NG: Some tests are failed. Please check them.\n");
	else
		pr_cont("OK\n");
	return 0;
}

late_initcall(kprobe_trace_self_tests_init);

#endif
