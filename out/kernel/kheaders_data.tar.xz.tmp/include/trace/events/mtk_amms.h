/* SPDX-License-Identifier: GPL-2.0 */


#undef TRACE_SYSTEM
#define TRACE_SYSTEM mtk_amms

#if !defined(_TRACE_MTK_AMMS_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_MTK_AMMS_H

#include <linux/types.h>
#include <linux/tracepoint.h>



DECLARE_EVENT_CLASS(amms_event,

	TP_PROTO(unsigned int amms_seq_id),

	TP_ARGS(amms_seq_id),

	TP_STRUCT__entry(
		__field(unsigned int,	amms_seq_id)
	),

	TP_fast_assign(
		__entry->amms_seq_id = amms_seq_id;
	),

	TP_printk("%u", __entry->amms_seq_id)
);

DEFINE_EVENT(amms_event, amms_event_alloc_success,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_start_iso,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_start_migrate,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);
DEFINE_EVENT(amms_event, amms_event_before_drain_all,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_after_drain_all,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_cma_alloc_failed,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_alloc_timeout,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_receive_irq,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_free,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);

DEFINE_EVENT(amms_event, amms_event_cma_alloc_end,

	TP_PROTO(unsigned int seq_id),

	TP_ARGS(seq_id)
);


TRACE_EVENT(amms_event_cma_alloc,

	TP_PROTO(unsigned int seq_id, int used_count, int count),

	TP_ARGS(seq_id, used_count, count),

	TP_STRUCT__entry(
		__field(unsigned int, seq_id)
		__field(int, used_count)
		__field(int, count)
	),

	TP_fast_assign(
		__entry->seq_id = seq_id;
		__entry->used_count = used_count;
		__entry->count = count;
	),

	TP_printk("seq_id %u page_count %d page_used %d",
		__entry->seq_id, __entry->count, __entry->used_count)
);


#endif 


#include <trace/define_trace.h>
