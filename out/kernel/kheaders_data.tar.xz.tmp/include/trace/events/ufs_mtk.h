/* SPDX-License-Identifier: GPL-2.0 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM ufs_mtk

#if !defined(_TRACE_EVENT_UFS_MTK_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_EVENT_UFS_MTK_H

#include <linux/tracepoint.h>

TRACE_EVENT(ufs_mtk_event,
	TP_PROTO(unsigned int type, unsigned int data),
	TP_ARGS(type, data),

	TP_STRUCT__entry(
		__field(unsigned int, type)
		__field(unsigned int, data)
	),

	TP_fast_assign(
		__entry->type = type;
		__entry->data = data;
	),

	TP_printk("ufs:event=%u data=%u",
		  __entry->type, __entry->data)
	);
#endif

#include <trace/define_trace.h>

