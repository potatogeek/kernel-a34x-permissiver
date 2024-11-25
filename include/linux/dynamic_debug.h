/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _DYNAMIC_DEBUG_H
#define _DYNAMIC_DEBUG_H

#if defined(CONFIG_JUMP_LABEL)
#include <linux/jump_label.h>
#endif

/*
 * An instance of this structure is created in a special
 * ELF section at every dynamic debug callsite.  At runtime,
 * the special section is treated as an array of these.
 */
struct _ddebug {
	/*
	 * These fields are used to drive the user interface
	 * for selecting and displaying debug callsites.
	 */
	const char *modname;
	const char *function;
	const char *filename;
	const char *format;
	unsigned int lineno:18;
	/*
	 * The flags field controls the behaviour at the callsite.
	 * The bits here are changed dynamically when the user
	 * writes commands to <debugfs>/dynamic_debug/control
	 */
#define _DPRINTK_FLAGS_NONE	0
#define _DPRINTK_FLAGS_PRINT	(1<<0) /* printk() a message using the format */
#define _DPRINTK_FLAGS_INCL_MODNAME	(1<<1)
#define _DPRINTK_FLAGS_INCL_FUNCNAME	(1<<2)
#define _DPRINTK_FLAGS_INCL_LINENO	(1<<3)
#define _DPRINTK_FLAGS_INCL_TID		(1<<4)
<<<<<<< HEAD
=======

#define _DPRINTK_FLAGS_INCL_ANY		\
	(_DPRINTK_FLAGS_INCL_MODNAME | _DPRINTK_FLAGS_INCL_FUNCNAME |\
	 _DPRINTK_FLAGS_INCL_LINENO  | _DPRINTK_FLAGS_INCL_TID)

>>>>>>> upstream/android-13
#if defined DEBUG
#define _DPRINTK_FLAGS_DEFAULT _DPRINTK_FLAGS_PRINT
#else
#define _DPRINTK_FLAGS_DEFAULT 0
#endif
	unsigned int flags:8;
#ifdef CONFIG_JUMP_LABEL
	union {
		struct static_key_true dd_key_true;
		struct static_key_false dd_key_false;
	} key;
#endif
} __attribute__((aligned(8)));


<<<<<<< HEAD
int ddebug_add_module(struct _ddebug *tab, unsigned int n,
				const char *modname);

#if defined(CONFIG_DYNAMIC_DEBUG)
extern int ddebug_remove_module(const char *mod_name);
extern __printf(2, 3)
#ifdef CONFIG_LOG_TOO_MUCH_WARNING
void __dynamic_pr_emerg(struct _ddebug *descriptor, const char *fmt, ...);
void __dynamic_pr_alert(struct _ddebug *descriptor, const char *fmt, ...);
void __dynamic_pr_crit(struct _ddebug *descriptor, const char *fmt, ...);
void __dynamic_pr_err(struct _ddebug *descriptor, const char *fmt, ...);
void __dynamic_pr_warn(struct _ddebug *descriptor, const char *fmt, ...);
void __dynamic_pr_notice(struct _ddebug *descriptor, const char *fmt, ...);
void __dynamic_pr_info(struct _ddebug *descriptor, const char *fmt, ...);
#endif
=======

#if defined(CONFIG_DYNAMIC_DEBUG_CORE)

/* exported for module authors to exercise >control */
int dynamic_debug_exec_queries(const char *query, const char *modname);

int ddebug_add_module(struct _ddebug *tab, unsigned int n,
				const char *modname);
extern int ddebug_remove_module(const char *mod_name);
extern __printf(2, 3)
>>>>>>> upstream/android-13
void __dynamic_pr_debug(struct _ddebug *descriptor, const char *fmt, ...);

extern int ddebug_dyndbg_module_param_cb(char *param, char *val,
					const char *modname);

struct device;

extern __printf(3, 4)
void __dynamic_dev_dbg(struct _ddebug *descriptor, const struct device *dev,
		       const char *fmt, ...);

struct net_device;

extern __printf(3, 4)
void __dynamic_netdev_dbg(struct _ddebug *descriptor,
			  const struct net_device *dev,
			  const char *fmt, ...);

<<<<<<< HEAD
#define DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, key, init)	\
	static struct _ddebug  __aligned(8)			\
	__attribute__((section("__verbose"))) name = {		\
=======
struct ib_device;

extern __printf(3, 4)
void __dynamic_ibdev_dbg(struct _ddebug *descriptor,
			 const struct ib_device *ibdev,
			 const char *fmt, ...);

#define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt)		\
	static struct _ddebug  __aligned(8)			\
	__section("__dyndbg") name = {				\
>>>>>>> upstream/android-13
		.modname = KBUILD_MODNAME,			\
		.function = __func__,				\
		.filename = __FILE__,				\
		.format = (fmt),				\
		.lineno = __LINE__,				\
		.flags = _DPRINTK_FLAGS_DEFAULT,		\
<<<<<<< HEAD
		dd_key_init(key, init)				\
=======
		_DPRINTK_KEY_INIT				\
>>>>>>> upstream/android-13
	}

#ifdef CONFIG_JUMP_LABEL

<<<<<<< HEAD
#define dd_key_init(key, init) key = (init)

#ifdef DEBUG
#define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt) \
	DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, .key.dd_key_true, \
					  (STATIC_KEY_TRUE_INIT))
=======
#ifdef DEBUG

#define _DPRINTK_KEY_INIT .key.dd_key_true = (STATIC_KEY_TRUE_INIT)
>>>>>>> upstream/android-13

#define DYNAMIC_DEBUG_BRANCH(descriptor) \
	static_branch_likely(&descriptor.key.dd_key_true)
#else
<<<<<<< HEAD
#define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt) \
	DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, .key.dd_key_false, \
					  (STATIC_KEY_FALSE_INIT))
=======
#define _DPRINTK_KEY_INIT .key.dd_key_false = (STATIC_KEY_FALSE_INIT)
>>>>>>> upstream/android-13

#define DYNAMIC_DEBUG_BRANCH(descriptor) \
	static_branch_unlikely(&descriptor.key.dd_key_false)
#endif

<<<<<<< HEAD
#else

#define dd_key_init(key, init)

#define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt) \
	DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, 0, 0)
=======
#else /* !CONFIG_JUMP_LABEL */

#define _DPRINTK_KEY_INIT
>>>>>>> upstream/android-13

#ifdef DEBUG
#define DYNAMIC_DEBUG_BRANCH(descriptor) \
	likely(descriptor.flags & _DPRINTK_FLAGS_PRINT)
#else
#define DYNAMIC_DEBUG_BRANCH(descriptor) \
	unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)
#endif

<<<<<<< HEAD
#endif

#ifdef CONFIG_LOG_TOO_MUCH_WARNING
#define dynamic_pr_emerg(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_emerg(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_EMERG KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})

#define dynamic_pr_alert(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_alert(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_ALERT KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})

#define dynamic_pr_crit(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_crit(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_CRIT KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})

#define dynamic_pr_err(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_err(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_ERR KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})

#define dynamic_pr_warn(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_warn(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_WARNING KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})

#define dynamic_pr_notice(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_notice(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_NOTICE KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})

#define dynamic_pr_info(fmt, ...)                \
({ \
	static bool __print_once __read_mostly; \
	if (!__print_once) { \
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, KLOG_MODNAME fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true; \
		__dynamic_pr_info(&descriptor, pr_fmt(fmt), \
			##__VA_ARGS__);      \
	}   else \
		printk(KERN_INFO KLOG_MODNAME pr_fmt(fmt), ##__VA_ARGS__); \
})
#endif

#define dynamic_pr_debug(fmt, ...)				\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
		__dynamic_pr_debug(&descriptor, pr_fmt(fmt),	\
				   ##__VA_ARGS__);		\
} while (0)

#define dynamic_dev_dbg(dev, fmt, ...)				\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
		__dynamic_dev_dbg(&descriptor, dev, fmt,	\
				  ##__VA_ARGS__);		\
} while (0)

#define dynamic_netdev_dbg(dev, fmt, ...)			\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
		__dynamic_netdev_dbg(&descriptor, dev, fmt,	\
				     ##__VA_ARGS__);		\
} while (0)

#define dynamic_hex_dump(prefix_str, prefix_type, rowsize,	\
			 groupsize, buf, len, ascii)		\
do {								\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor,		\
		__builtin_constant_p(prefix_str) ? prefix_str : "hexdump");\
	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
		print_hex_dump(KERN_DEBUG, prefix_str,		\
			       prefix_type, rowsize, groupsize,	\
			       buf, len, ascii);		\
} while (0)

#else

#include <linux/string.h>
#include <linux/errno.h>
=======
#endif /* CONFIG_JUMP_LABEL */

#define __dynamic_func_call(id, fmt, func, ...) do {	\
	DEFINE_DYNAMIC_DEBUG_METADATA(id, fmt);		\
	if (DYNAMIC_DEBUG_BRANCH(id))			\
		func(&id, ##__VA_ARGS__);		\
} while (0)

#define __dynamic_func_call_no_desc(id, fmt, func, ...) do {	\
	DEFINE_DYNAMIC_DEBUG_METADATA(id, fmt);			\
	if (DYNAMIC_DEBUG_BRANCH(id))				\
		func(__VA_ARGS__);				\
} while (0)

/*
 * "Factory macro" for generating a call to func, guarded by a
 * DYNAMIC_DEBUG_BRANCH. The dynamic debug descriptor will be
 * initialized using the fmt argument. The function will be called with
 * the address of the descriptor as first argument, followed by all
 * the varargs. Note that fmt is repeated in invocations of this
 * macro.
 */
#define _dynamic_func_call(fmt, func, ...)				\
	__dynamic_func_call(__UNIQUE_ID(ddebug), fmt, func, ##__VA_ARGS__)
/*
 * A variant that does the same, except that the descriptor is not
 * passed as the first argument to the function; it is only called
 * with precisely the macro's varargs.
 */
#define _dynamic_func_call_no_desc(fmt, func, ...)	\
	__dynamic_func_call_no_desc(__UNIQUE_ID(ddebug), fmt, func, ##__VA_ARGS__)

#define dynamic_pr_debug(fmt, ...)				\
	_dynamic_func_call(fmt,	__dynamic_pr_debug,		\
			   pr_fmt(fmt), ##__VA_ARGS__)

#define dynamic_dev_dbg(dev, fmt, ...)				\
	_dynamic_func_call(fmt,__dynamic_dev_dbg, 		\
			   dev, fmt, ##__VA_ARGS__)

#define dynamic_netdev_dbg(dev, fmt, ...)			\
	_dynamic_func_call(fmt, __dynamic_netdev_dbg,		\
			   dev, fmt, ##__VA_ARGS__)

#define dynamic_ibdev_dbg(dev, fmt, ...)			\
	_dynamic_func_call(fmt, __dynamic_ibdev_dbg,		\
			   dev, fmt, ##__VA_ARGS__)

#define dynamic_hex_dump(prefix_str, prefix_type, rowsize,		\
			 groupsize, buf, len, ascii)			\
	_dynamic_func_call_no_desc(__builtin_constant_p(prefix_str) ? prefix_str : "hexdump", \
				   print_hex_dump,			\
				   KERN_DEBUG, prefix_str, prefix_type,	\
				   rowsize, groupsize, buf, len, ascii)

#else /* !CONFIG_DYNAMIC_DEBUG_CORE */

#include <linux/string.h>
#include <linux/errno.h>
#include <linux/printk.h>

static inline int ddebug_add_module(struct _ddebug *tab, unsigned int n,
				    const char *modname)
{
	return 0;
}
>>>>>>> upstream/android-13

static inline int ddebug_remove_module(const char *mod)
{
	return 0;
}

static inline int ddebug_dyndbg_module_param_cb(char *param, char *val,
						const char *modname)
{
	if (strstr(param, "dyndbg")) {
		/* avoid pr_warn(), which wants pr_fmt() fully defined */
		printk(KERN_WARNING "dyndbg param is supported only in "
			"CONFIG_DYNAMIC_DEBUG builds\n");
		return 0; /* allow and ignore */
	}
	return -EINVAL;
}

#define dynamic_pr_debug(fmt, ...)					\
	do { if (0) printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__); } while (0)
#define dynamic_dev_dbg(dev, fmt, ...)					\
	do { if (0) dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__); } while (0)
<<<<<<< HEAD
#endif
=======
#define dynamic_hex_dump(prefix_str, prefix_type, rowsize,		\
			 groupsize, buf, len, ascii)			\
	do { if (0)							\
		print_hex_dump(KERN_DEBUG, prefix_str, prefix_type,	\
				rowsize, groupsize, buf, len, ascii);	\
	} while (0)

static inline int dynamic_debug_exec_queries(const char *query, const char *modname)
{
	pr_warn("kernel not built with CONFIG_DYNAMIC_DEBUG_CORE\n");
	return 0;
}

#endif /* !CONFIG_DYNAMIC_DEBUG_CORE */
>>>>>>> upstream/android-13

#endif
