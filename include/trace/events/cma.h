/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM cma

#if !defined(_TRACE_CMA_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_CMA_H

#include <linux/types.h>
#include <linux/tracepoint.h>

<<<<<<< HEAD
TRACE_EVENT(cma_alloc,

	TP_PROTO(unsigned long pfn, const struct page *page,
		 unsigned int count, unsigned int align),

	TP_ARGS(pfn, page, count, align),

	TP_STRUCT__entry(
		__field(unsigned long, pfn)
		__field(const struct page *, page)
		__field(unsigned int, count)
=======
DECLARE_EVENT_CLASS(cma_alloc_class,

	TP_PROTO(const char *name, unsigned long pfn, const struct page *page,
		 unsigned long count, unsigned int align),

	TP_ARGS(name, pfn, page, count, align),

	TP_STRUCT__entry(
		__string(name, name)
		__field(unsigned long, pfn)
		__field(const struct page *, page)
		__field(unsigned long, count)
>>>>>>> upstream/android-13
		__field(unsigned int, align)
	),

	TP_fast_assign(
<<<<<<< HEAD
=======
		__assign_str(name, name);
>>>>>>> upstream/android-13
		__entry->pfn = pfn;
		__entry->page = page;
		__entry->count = count;
		__entry->align = align;
	),

<<<<<<< HEAD
	TP_printk("pfn=%lx page=%p count=%u align=%u",
=======
	TP_printk("name=%s pfn=0x%lx page=%p count=%lu align=%u",
		  __get_str(name),
>>>>>>> upstream/android-13
		  __entry->pfn,
		  __entry->page,
		  __entry->count,
		  __entry->align)
);

TRACE_EVENT(cma_release,

<<<<<<< HEAD
	TP_PROTO(unsigned long pfn, const struct page *page,
		 unsigned int count),

	TP_ARGS(pfn, page, count),

	TP_STRUCT__entry(
		__field(unsigned long, pfn)
		__field(const struct page *, page)
		__field(unsigned int, count)
	),

	TP_fast_assign(
=======
	TP_PROTO(const char *name, unsigned long pfn, const struct page *page,
		 unsigned long count),

	TP_ARGS(name, pfn, page, count),

	TP_STRUCT__entry(
		__string(name, name)
		__field(unsigned long, pfn)
		__field(const struct page *, page)
		__field(unsigned long, count)
	),

	TP_fast_assign(
		__assign_str(name, name);
>>>>>>> upstream/android-13
		__entry->pfn = pfn;
		__entry->page = page;
		__entry->count = count;
	),

<<<<<<< HEAD
	TP_printk("pfn=%lx page=%p count=%u",
=======
	TP_printk("name=%s pfn=0x%lx page=%p count=%lu",
		  __get_str(name),
>>>>>>> upstream/android-13
		  __entry->pfn,
		  __entry->page,
		  __entry->count)
);

<<<<<<< HEAD
=======
TRACE_EVENT(cma_alloc_start,

	TP_PROTO(const char *name, unsigned long count, unsigned int align),

	TP_ARGS(name, count, align),

	TP_STRUCT__entry(
		__string(name, name)
		__field(unsigned long, count)
		__field(unsigned int, align)
	),

	TP_fast_assign(
		__assign_str(name, name);
		__entry->count = count;
		__entry->align = align;
	),

	TP_printk("name=%s count=%lu align=%u",
		  __get_str(name),
		  __entry->count,
		  __entry->align)
);

DEFINE_EVENT(cma_alloc_class, cma_alloc_finish,

	TP_PROTO(const char *name, unsigned long pfn, const struct page *page,
		 unsigned long count, unsigned int align),

	TP_ARGS(name, pfn, page, count, align)
);

DEFINE_EVENT(cma_alloc_class, cma_alloc_busy_retry,

	TP_PROTO(const char *name, unsigned long pfn, const struct page *page,
		 unsigned long count, unsigned int align),

	TP_ARGS(name, pfn, page, count, align)
);

>>>>>>> upstream/android-13
#endif /* _TRACE_CMA_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
