

#ifndef DRM_PRINT_H_
#define DRM_PRINT_H_

#include <linux/compiler.h>
#include <linux/printk.h>
#include <linux/seq_file.h>
#include <linux/device.h>




struct drm_printer {
	
	void (*printfn)(struct drm_printer *p, struct va_format *vaf);
	void (*puts)(struct drm_printer *p, const char *str);
	void *arg;
	const char *prefix;
};

void __drm_printfn_coredump(struct drm_printer *p, struct va_format *vaf);
void __drm_puts_coredump(struct drm_printer *p, const char *str);
void __drm_printfn_seq_file(struct drm_printer *p, struct va_format *vaf);
void __drm_puts_seq_file(struct drm_printer *p, const char *str);
void __drm_printfn_info(struct drm_printer *p, struct va_format *vaf);
void __drm_printfn_debug(struct drm_printer *p, struct va_format *vaf);

__printf(2, 3)
void drm_printf(struct drm_printer *p, const char *f, ...);
void drm_puts(struct drm_printer *p, const char *str);

__printf(2, 0)

static inline void
drm_vprintf(struct drm_printer *p, const char *fmt, va_list *va)
{
	struct va_format vaf = { .fmt = fmt, .va = va };

	p->printfn(p, &vaf);
}


#define drm_printf_indent(printer, indent, fmt, ...) \
	drm_printf((printer), "%.*s" fmt, (indent), "\t\t\t\t\tX", ##__VA_ARGS__)


struct drm_print_iterator {
	void *data;
	ssize_t start;
	ssize_t remain;
	
	ssize_t offset;
};


static inline struct drm_printer
drm_coredump_printer(struct drm_print_iterator *iter)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_coredump,
		.puts = __drm_puts_coredump,
		.arg = iter,
	};

	
	iter->offset = 0;

	return p;
}


static inline struct drm_printer drm_seq_file_printer(struct seq_file *f)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_seq_file,
		.puts = __drm_puts_seq_file,
		.arg = f,
	};
	return p;
}


static inline struct drm_printer drm_info_printer(struct device *dev)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_info,
		.arg = dev,
	};
	return p;
}


static inline struct drm_printer drm_debug_printer(const char *prefix)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_debug,
		.prefix = prefix
	};
	return p;
}


#define DRM_UT_NONE		0x00
#define DRM_UT_CORE		0x01
#define DRM_UT_DRIVER		0x02
#define DRM_UT_KMS		0x04
#define DRM_UT_PRIME		0x08
#define DRM_UT_ATOMIC		0x10
#define DRM_UT_VBL		0x20
#define DRM_UT_STATE		0x40
#define DRM_UT_LEASE		0x80
#define DRM_UT_DP		0x100

__printf(3, 4)
void drm_dev_printk(const struct device *dev, const char *level,
		    const char *format, ...);
__printf(3, 4)
void drm_dev_dbg(const struct device *dev, unsigned int category,
		 const char *format, ...);

__printf(2, 3)
void drm_dbg(unsigned int category, const char *format, ...);
__printf(1, 2)
void drm_err(const char *format, ...);



#define _DRM_PRINTK(once, level, fmt, ...)				\
	printk##once(KERN_##level "[" DRM_NAME "] " fmt, ##__VA_ARGS__)

#define DRM_INFO(fmt, ...)						\
	_DRM_PRINTK(, INFO, fmt, ##__VA_ARGS__)
#define DRM_NOTE(fmt, ...)						\
	_DRM_PRINTK(, NOTICE, fmt, ##__VA_ARGS__)
#define DRM_WARN(fmt, ...)						\
	_DRM_PRINTK(, WARNING, fmt, ##__VA_ARGS__)

#define DRM_INFO_ONCE(fmt, ...)						\
	_DRM_PRINTK(_once, INFO, fmt, ##__VA_ARGS__)
#define DRM_NOTE_ONCE(fmt, ...)						\
	_DRM_PRINTK(_once, NOTICE, fmt, ##__VA_ARGS__)
#define DRM_WARN_ONCE(fmt, ...)						\
	_DRM_PRINTK(_once, WARNING, fmt, ##__VA_ARGS__)


#define DRM_DEV_ERROR(dev, fmt, ...)					\
	drm_dev_printk(dev, KERN_ERR, "*ERROR* " fmt, ##__VA_ARGS__)
#define DRM_ERROR(fmt, ...)						\
	drm_err(fmt, ##__VA_ARGS__)


#define DRM_DEV_ERROR_RATELIMITED(dev, fmt, ...)			\
({									\
	static DEFINE_RATELIMIT_STATE(_rs,				\
				      DEFAULT_RATELIMIT_INTERVAL,	\
				      DEFAULT_RATELIMIT_BURST);		\
									\
	if (__ratelimit(&_rs))						\
		DRM_DEV_ERROR(dev, fmt, ##__VA_ARGS__);			\
})
#define DRM_ERROR_RATELIMITED(fmt, ...)					\
	DRM_DEV_ERROR_RATELIMITED(NULL, fmt, ##__VA_ARGS__)

#define DRM_DEV_INFO(dev, fmt, ...)					\
	drm_dev_printk(dev, KERN_INFO, fmt, ##__VA_ARGS__)

#define DRM_DEV_INFO_ONCE(dev, fmt, ...)				\
({									\
	static bool __print_once __read_mostly;				\
	if (!__print_once) {						\
		__print_once = true;					\
		DRM_DEV_INFO(dev, fmt, ##__VA_ARGS__);			\
	}								\
})


#define DRM_DEV_DEBUG(dev, fmt, ...)					\
	drm_dev_dbg(dev, DRM_UT_CORE, fmt, ##__VA_ARGS__)
#define DRM_DEBUG(fmt, ...)						\
	drm_dbg(DRM_UT_CORE, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_DRIVER(dev, fmt, ...)				\
	drm_dev_dbg(dev, DRM_UT_DRIVER,	fmt, ##__VA_ARGS__)
#define DRM_DEBUG_DRIVER(fmt, ...)					\
	drm_dbg(DRM_UT_DRIVER, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_KMS(dev, fmt, ...)				\
	drm_dev_dbg(dev, DRM_UT_KMS, fmt, ##__VA_ARGS__)
#define DRM_DEBUG_KMS(fmt, ...)						\
	drm_dbg(DRM_UT_KMS, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_PRIME(dev, fmt, ...)				\
	drm_dev_dbg(dev, DRM_UT_PRIME, fmt, ##__VA_ARGS__)
#define DRM_DEBUG_PRIME(fmt, ...)					\
	drm_dbg(DRM_UT_PRIME, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_ATOMIC(dev, fmt, ...)				\
	drm_dev_dbg(dev, DRM_UT_ATOMIC,	fmt, ##__VA_ARGS__)
#define DRM_DEBUG_ATOMIC(fmt, ...)					\
	drm_dbg(DRM_UT_ATOMIC, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_VBL(dev, fmt, ...)				\
	drm_dev_dbg(dev, DRM_UT_VBL, fmt, ##__VA_ARGS__)
#define DRM_DEBUG_VBL(fmt, ...)						\
	drm_dbg(DRM_UT_VBL, fmt, ##__VA_ARGS__)

#define DRM_DEBUG_LEASE(fmt, ...)					\
	drm_dbg(DRM_UT_LEASE, fmt, ##__VA_ARGS__)

#define	DRM_DEV_DEBUG_DP(dev, fmt, ...)					\
	drm_dev_dbg(dev, DRM_UT_DP, fmt, ## __VA_ARGS__)
#define DRM_DEBUG_DP(dev, fmt, ...)					\
	drm_dbg(DRM_UT_DP, fmt, ## __VA_ARGS__)

#define _DRM_DEV_DEFINE_DEBUG_RATELIMITED(dev, category, fmt, ...)	\
({									\
	static DEFINE_RATELIMIT_STATE(_rs,				\
				      DEFAULT_RATELIMIT_INTERVAL,	\
				      DEFAULT_RATELIMIT_BURST);		\
	if (__ratelimit(&_rs))						\
		drm_dev_dbg(dev, category, fmt, ##__VA_ARGS__);		\
})


#define DRM_DEV_DEBUG_RATELIMITED(dev, fmt, ...)			\
	_DEV_DRM_DEFINE_DEBUG_RATELIMITED(dev, DRM_UT_CORE,		\
					  fmt, ##__VA_ARGS__)
#define DRM_DEBUG_RATELIMITED(fmt, ...)					\
	DRM_DEV_DEBUG_RATELIMITED(NULL, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_DRIVER_RATELIMITED(dev, fmt, ...)			\
	_DRM_DEV_DEFINE_DEBUG_RATELIMITED(dev, DRM_UT_DRIVER,		\
					  fmt, ##__VA_ARGS__)
#define DRM_DEBUG_DRIVER_RATELIMITED(fmt, ...)				\
	DRM_DEV_DEBUG_DRIVER_RATELIMITED(NULL, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_KMS_RATELIMITED(dev, fmt, ...)			\
	_DRM_DEV_DEFINE_DEBUG_RATELIMITED(dev, DRM_UT_KMS,		\
					  fmt, ##__VA_ARGS__)
#define DRM_DEBUG_KMS_RATELIMITED(fmt, ...)				\
	DRM_DEV_DEBUG_KMS_RATELIMITED(NULL, fmt, ##__VA_ARGS__)

#define DRM_DEV_DEBUG_PRIME_RATELIMITED(dev, fmt, ...)			\
	_DRM_DEV_DEFINE_DEBUG_RATELIMITED(dev, DRM_UT_PRIME,		\
					  fmt, ##__VA_ARGS__)
#define DRM_DEBUG_PRIME_RATELIMITED(fmt, ...)				\
	DRM_DEV_DEBUG_PRIME_RATELIMITED(NULL, fmt, ##__VA_ARGS__)

#endif 
