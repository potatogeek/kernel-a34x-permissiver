/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright IBM Corp. 2000, 2009
 * Author(s): Utz Bacher <utz.bacher@de.ibm.com>
 *	      Jan Glauber <jang@linux.vnet.ibm.com>
 */
#ifndef _CIO_QDIO_H
#define _CIO_QDIO_H

#include <asm/page.h>
#include <asm/schid.h>
#include <asm/debug.h>
#include "chsc.h"

#define QDIO_BUSY_BIT_PATIENCE		(100 << 12)	/* 100 microseconds */
#define QDIO_BUSY_BIT_RETRY_DELAY	10		/* 10 milliseconds */
#define QDIO_BUSY_BIT_RETRIES		1000		/* = 10s retry time */
<<<<<<< HEAD
#define QDIO_INPUT_THRESHOLD		(500 << 12)	/* 500 microseconds */
=======
>>>>>>> upstream/android-13

enum qdio_irq_states {
	QDIO_IRQ_STATE_INACTIVE,
	QDIO_IRQ_STATE_ESTABLISHED,
	QDIO_IRQ_STATE_ACTIVE,
	QDIO_IRQ_STATE_STOPPED,
	QDIO_IRQ_STATE_CLEANUP,
	QDIO_IRQ_STATE_ERR,
	NR_QDIO_IRQ_STATES,
};

/* used as intparm in do_IO */
#define QDIO_DOING_ESTABLISH	1
#define QDIO_DOING_ACTIVATE	2
#define QDIO_DOING_CLEANUP	3

#define SLSB_STATE_NOT_INIT	0x0
#define SLSB_STATE_EMPTY	0x1
#define SLSB_STATE_PRIMED	0x2
#define SLSB_STATE_PENDING	0x3
#define SLSB_STATE_HALTED	0xe
#define SLSB_STATE_ERROR	0xf
#define SLSB_TYPE_INPUT		0x0
#define SLSB_TYPE_OUTPUT	0x20
#define SLSB_OWNER_PROG		0x80
#define SLSB_OWNER_CU		0x40

#define SLSB_P_INPUT_NOT_INIT	\
	(SLSB_OWNER_PROG | SLSB_TYPE_INPUT | SLSB_STATE_NOT_INIT)  /* 0x80 */
#define SLSB_P_INPUT_ACK	\
	(SLSB_OWNER_PROG | SLSB_TYPE_INPUT | SLSB_STATE_EMPTY)	   /* 0x81 */
#define SLSB_CU_INPUT_EMPTY	\
	(SLSB_OWNER_CU | SLSB_TYPE_INPUT | SLSB_STATE_EMPTY)	   /* 0x41 */
#define SLSB_P_INPUT_PRIMED	\
	(SLSB_OWNER_PROG | SLSB_TYPE_INPUT | SLSB_STATE_PRIMED)	   /* 0x82 */
#define SLSB_P_INPUT_HALTED	\
	(SLSB_OWNER_PROG | SLSB_TYPE_INPUT | SLSB_STATE_HALTED)	   /* 0x8e */
#define SLSB_P_INPUT_ERROR	\
	(SLSB_OWNER_PROG | SLSB_TYPE_INPUT | SLSB_STATE_ERROR)	   /* 0x8f */
#define SLSB_P_OUTPUT_NOT_INIT	\
	(SLSB_OWNER_PROG | SLSB_TYPE_OUTPUT | SLSB_STATE_NOT_INIT) /* 0xa0 */
#define SLSB_P_OUTPUT_EMPTY	\
	(SLSB_OWNER_PROG | SLSB_TYPE_OUTPUT | SLSB_STATE_EMPTY)	   /* 0xa1 */
#define SLSB_P_OUTPUT_PENDING \
	(SLSB_OWNER_PROG | SLSB_TYPE_OUTPUT | SLSB_STATE_PENDING)  /* 0xa3 */
#define SLSB_CU_OUTPUT_PRIMED	\
	(SLSB_OWNER_CU | SLSB_TYPE_OUTPUT | SLSB_STATE_PRIMED)	   /* 0x62 */
#define SLSB_P_OUTPUT_HALTED	\
	(SLSB_OWNER_PROG | SLSB_TYPE_OUTPUT | SLSB_STATE_HALTED)   /* 0xae */
#define SLSB_P_OUTPUT_ERROR	\
	(SLSB_OWNER_PROG | SLSB_TYPE_OUTPUT | SLSB_STATE_ERROR)	   /* 0xaf */

#define SLSB_ERROR_DURING_LOOKUP  0xff

/* additional CIWs returned by extended Sense-ID */
#define CIW_TYPE_EQUEUE			0x3 /* establish QDIO queues */
#define CIW_TYPE_AQUEUE			0x4 /* activate QDIO queues */

/* flags for st qdio sch data */
#define CHSC_FLAG_QDIO_CAPABILITY	0x80
#define CHSC_FLAG_VALIDITY		0x40

/* SIGA flags */
#define QDIO_SIGA_WRITE		0x00
#define QDIO_SIGA_READ		0x01
#define QDIO_SIGA_SYNC		0x02
<<<<<<< HEAD
=======
#define QDIO_SIGA_WRITEM	0x03
>>>>>>> upstream/android-13
#define QDIO_SIGA_WRITEQ	0x04
#define QDIO_SIGA_QEBSM_FLAG	0x80

static inline int do_sqbs(u64 token, unsigned char state, int queue,
			  int *start, int *count)
{
<<<<<<< HEAD
	register unsigned long _ccq asm ("0") = *count;
	register unsigned long _token asm ("1") = token;
	unsigned long _queuestart = ((unsigned long)queue << 32) | *start;

	asm volatile(
		"	.insn	rsy,0xeb000000008A,%1,0,0(%2)"
		: "+d" (_ccq), "+d" (_queuestart)
		: "d" ((unsigned long)state), "d" (_token)
		: "memory", "cc");
=======
	unsigned long _queuestart = ((unsigned long)queue << 32) | *start;
	unsigned long _ccq = *count;

	asm volatile(
		"	lgr	1,%[token]\n"
		"	.insn	rsy,0xeb000000008a,%[qs],%[ccq],0(%[state])"
		: [ccq] "+&d" (_ccq), [qs] "+&d" (_queuestart)
		: [state] "d" ((unsigned long)state), [token] "d" (token)
		: "memory", "cc", "1");
>>>>>>> upstream/android-13
	*count = _ccq & 0xff;
	*start = _queuestart & 0xff;

	return (_ccq >> 32) & 0xff;
}

static inline int do_eqbs(u64 token, unsigned char *state, int queue,
			  int *start, int *count, int ack)
{
<<<<<<< HEAD
	register unsigned long _ccq asm ("0") = *count;
	register unsigned long _token asm ("1") = token;
	unsigned long _queuestart = ((unsigned long)queue << 32) | *start;
	unsigned long _state = (unsigned long)ack << 63;

	asm volatile(
		"	.insn	rrf,0xB99c0000,%1,%2,0,0"
		: "+d" (_ccq), "+d" (_queuestart), "+d" (_state)
		: "d" (_token)
		: "memory", "cc");
=======
	unsigned long _queuestart = ((unsigned long)queue << 32) | *start;
	unsigned long _state = (unsigned long)ack << 63;
	unsigned long _ccq = *count;

	asm volatile(
		"	lgr	1,%[token]\n"
		"	.insn	rrf,0xb99c0000,%[qs],%[state],%[ccq],0"
		: [ccq] "+&d" (_ccq), [qs] "+&d" (_queuestart),
		  [state] "+&d" (_state)
		: [token] "d" (token)
		: "memory", "cc", "1");
>>>>>>> upstream/android-13
	*count = _ccq & 0xff;
	*start = _queuestart & 0xff;
	*state = _state & 0xff;

	return (_ccq >> 32) & 0xff;
}

struct qdio_irq;

<<<<<<< HEAD
struct siga_flag {
	u8 input:1;
	u8 output:1;
	u8 sync:1;
	u8 sync_after_ai:1;
	u8 sync_out_after_pci:1;
	u8:3;
} __attribute__ ((packed));

struct qdio_dev_perf_stat {
	unsigned int adapter_int;
	unsigned int qdio_int;
	unsigned int pci_request_int;

	unsigned int tasklet_inbound;
	unsigned int tasklet_inbound_resched;
	unsigned int tasklet_inbound_resched2;
	unsigned int tasklet_outbound;
=======
struct qdio_dev_perf_stat {
	unsigned int adapter_int;
	unsigned int qdio_int;
>>>>>>> upstream/android-13

	unsigned int siga_read;
	unsigned int siga_write;
	unsigned int siga_sync;

	unsigned int inbound_call;
<<<<<<< HEAD
	unsigned int inbound_handler;
	unsigned int stop_polling;
	unsigned int inbound_queue_full;
	unsigned int outbound_call;
	unsigned int outbound_handler;
=======
	unsigned int stop_polling;
	unsigned int inbound_queue_full;
	unsigned int outbound_call;
>>>>>>> upstream/android-13
	unsigned int outbound_queue_full;
	unsigned int fast_requeue;
	unsigned int target_full;
	unsigned int eqbs;
	unsigned int eqbs_partial;
	unsigned int sqbs;
	unsigned int sqbs_partial;
	unsigned int int_discarded;
} ____cacheline_aligned;

struct qdio_queue_perf_stat {
<<<<<<< HEAD
	/*
	 * Sorted into order-2 buckets: 1, 2-3, 4-7, ... 64-127, 128.
	 * Since max. 127 SBALs are scanned reuse entry for 128 as queue full
	 * aka 127 SBALs found.
	 */
=======
	/* Sorted into order-2 buckets: 1, 2-3, 4-7, ... 64-127, 128. */
>>>>>>> upstream/android-13
	unsigned int nr_sbals[8];
	unsigned int nr_sbal_error;
	unsigned int nr_sbal_nop;
	unsigned int nr_sbal_total;
};

<<<<<<< HEAD
enum qdio_queue_irq_states {
	QDIO_QUEUE_IRQS_DISABLED,
};

struct qdio_input_q {
	/* input buffer acknowledgement flag */
	int polling;
	/* first ACK'ed buffer */
	int ack_start;
	/* how much sbals are acknowledged with qebsm */
	int ack_count;
	/* last time of noticing incoming data */
	u64 timestamp;
	/* upper-layer polling flag */
	unsigned long queue_irq_state;
	/* callback to start upper-layer polling */
	void (*queue_start_poll) (struct ccw_device *, int, unsigned long);
};

struct qdio_output_q {
	/* PCIs are enabled for the queue */
	int pci_out_enabled;
	/* cq: use asynchronous output buffers */
	int use_cq;
	/* cq: aobs used for particual SBAL */
	struct qaob **aobs;
	/* cq: sbal state related to asynchronous operation */
	struct qdio_outbuf_state *sbal_state;
	/* timer to check for more outbound work */
	struct timer_list timer;
	/* used SBALs before tasklet schedule */
	int scan_threshold;
=======
enum qdio_irq_poll_states {
	QDIO_IRQ_DISABLED,
};

struct qdio_input_q {
	/* Batch of SBALs that we processed while polling the queue: */
	unsigned int batch_start;
	unsigned int batch_count;
};

struct qdio_output_q {
>>>>>>> upstream/android-13
};

/*
 * Note on cache alignment: grouped slsb and write mostly data at the beginning
 * sbal[] is read-only and starts on a new cacheline followed by read mostly.
 */
struct qdio_q {
	struct slsb slsb;

	union {
		struct qdio_input_q in;
		struct qdio_output_q out;
	} u;

	/*
	 * inbound: next buffer the program should check for
	 * outbound: next buffer to check if adapter processed it
	 */
	int first_to_check;

<<<<<<< HEAD
	/* first_to_check of the last time */
	int last_move;

	/* beginning position for calling the program */
	int first_to_kick;

	/* number of buffers in use by the adapter */
	atomic_t nr_buf_used;

	/* error condition during a data transfer */
	unsigned int qdio_error;

	/* last scan of the queue */
	u64 timestamp;

	struct tasklet_struct tasklet;
=======
	/* number of buffers in use by the adapter */
	atomic_t nr_buf_used;

	/* last scan of the queue */
	u64 timestamp;

>>>>>>> upstream/android-13
	struct qdio_queue_perf_stat q_stats;

	struct qdio_buffer *sbal[QDIO_MAX_BUFFERS_PER_Q] ____cacheline_aligned;

	/* queue number */
	int nr;

	/* bitmask of queue number */
	int mask;

	/* input or output queue */
	int is_input_q;

<<<<<<< HEAD
	/* list of thinint input queues */
	struct list_head entry;

	/* upper-layer program handler */
	qdio_handler_t (*handler);

	struct dentry *debugfs_q;
=======
	/* upper-layer program handler */
	qdio_handler_t (*handler);

>>>>>>> upstream/android-13
	struct qdio_irq *irq_ptr;
	struct sl *sl;
	/*
	 * A page is allocated under this pointer and used for slib and sl.
	 * slib is 2048 bytes big and sl points to offset PAGE_SIZE / 2.
	 */
	struct slib *slib;
} __attribute__ ((aligned(256)));

struct qdio_irq {
	struct qib qib;
	u32 *dsci;		/* address of device state change indicator */
	struct ccw_device *cdev;
<<<<<<< HEAD
	struct dentry *debugfs_dev;
	struct dentry *debugfs_perf;
=======
	struct list_head entry;		/* list of thinint devices */
	struct dentry *debugfs_dev;
	u64 last_data_irq_time;
>>>>>>> upstream/android-13

	unsigned long int_parm;
	struct subchannel_id schid;
	unsigned long sch_token;	/* QEBSM facility */

	enum qdio_irq_states state;
<<<<<<< HEAD

	struct siga_flag siga_flag;	/* siga sync information from qdioac */
=======
	u8 qdioac1;
>>>>>>> upstream/android-13

	int nr_input_qs;
	int nr_output_qs;

	struct ccw1 ccw;
	struct ciw equeue;
	struct ciw aqueue;

	struct qdio_ssqd_desc ssqd_desc;
	void (*orig_handler) (struct ccw_device *, unsigned long, struct irb *);

	int perf_stat_enabled;

	struct qdr *qdr;
	unsigned long chsc_page;

	struct qdio_q *input_qs[QDIO_MAX_QUEUES_PER_IRQ];
	struct qdio_q *output_qs[QDIO_MAX_QUEUES_PER_IRQ];
<<<<<<< HEAD
=======
	unsigned int max_input_qs;
	unsigned int max_output_qs;

	void (*irq_poll)(struct ccw_device *cdev, unsigned long data);
	unsigned long poll_state;
>>>>>>> upstream/android-13

	debug_info_t *debug_area;
	struct mutex setup_mutex;
	struct qdio_dev_perf_stat perf_stat;
};

/* helper functions */
#define queue_type(q)	q->irq_ptr->qib.qfmt
#define SCH_NO(q)	(q->irq_ptr->schid.sch_no)

#define is_thinint_irq(irq) \
	(irq->qib.qfmt == QDIO_IQDIO_QFMT || \
	 css_general_characteristics.aif_osa)

#define qperf(__qdev, __attr)	((__qdev)->perf_stat.(__attr))

<<<<<<< HEAD
#define qperf_inc(__q, __attr)						\
({									\
	struct qdio_irq *qdev = (__q)->irq_ptr;				\
=======
#define QDIO_PERF_STAT_INC(__irq, __attr)				\
({									\
	struct qdio_irq *qdev = __irq;					\
>>>>>>> upstream/android-13
	if (qdev->perf_stat_enabled)					\
		(qdev->perf_stat.__attr)++;				\
})

<<<<<<< HEAD
=======
#define qperf_inc(__q, __attr)	QDIO_PERF_STAT_INC((__q)->irq_ptr, __attr)

>>>>>>> upstream/android-13
static inline void account_sbals_error(struct qdio_q *q, int count)
{
	q->q_stats.nr_sbal_error += count;
	q->q_stats.nr_sbal_total += count;
}

/* the highest iqdio queue is used for multicast */
static inline int multicast_outbound(struct qdio_q *q)
{
	return (q->irq_ptr->nr_output_qs > 1) &&
	       (q->nr == q->irq_ptr->nr_output_qs - 1);
}

<<<<<<< HEAD
#define pci_out_supported(q) \
	(q->irq_ptr->qib.ac & QIB_AC_OUTBOUND_PCI_SUPPORTED)
#define is_qebsm(q)			(q->irq_ptr->sch_token != 0)

#define need_siga_in(q)			(q->irq_ptr->siga_flag.input)
#define need_siga_out(q)		(q->irq_ptr->siga_flag.output)
#define need_siga_sync(q)		(unlikely(q->irq_ptr->siga_flag.sync))
#define need_siga_sync_after_ai(q)	\
	(unlikely(q->irq_ptr->siga_flag.sync_after_ai))
#define need_siga_sync_out_after_pci(q)	\
	(unlikely(q->irq_ptr->siga_flag.sync_out_after_pci))
=======
static inline void qdio_deliver_irq(struct qdio_irq *irq)
{
	if (!test_and_set_bit(QDIO_IRQ_DISABLED, &irq->poll_state))
		irq->irq_poll(irq->cdev, irq->int_parm);
	else
		QDIO_PERF_STAT_INC(irq, int_discarded);
}

#define pci_out_supported(irq) ((irq)->qib.ac & QIB_AC_OUTBOUND_PCI_SUPPORTED)
#define is_qebsm(q)			(q->irq_ptr->sch_token != 0)

#define qdio_need_siga_in(irq)		((irq)->qdioac1 & AC1_SIGA_INPUT_NEEDED)
#define qdio_need_siga_out(irq)		((irq)->qdioac1 & AC1_SIGA_OUTPUT_NEEDED)
#define qdio_need_siga_sync(irq)	(unlikely((irq)->qdioac1 & AC1_SIGA_SYNC_NEEDED))
>>>>>>> upstream/android-13

#define for_each_input_queue(irq_ptr, q, i)		\
	for (i = 0; i < irq_ptr->nr_input_qs &&		\
		({ q = irq_ptr->input_qs[i]; 1; }); i++)
#define for_each_output_queue(irq_ptr, q, i)		\
	for (i = 0; i < irq_ptr->nr_output_qs &&	\
		({ q = irq_ptr->output_qs[i]; 1; }); i++)

<<<<<<< HEAD
#define prev_buf(bufnr)	\
	((bufnr + QDIO_MAX_BUFFERS_MASK) & QDIO_MAX_BUFFERS_MASK)
#define next_buf(bufnr)	\
	((bufnr + 1) & QDIO_MAX_BUFFERS_MASK)
#define add_buf(bufnr, inc) \
	((bufnr + inc) & QDIO_MAX_BUFFERS_MASK)
#define sub_buf(bufnr, dec) \
	((bufnr - dec) & QDIO_MAX_BUFFERS_MASK)

#define queue_irqs_enabled(q)			\
	(test_bit(QDIO_QUEUE_IRQS_DISABLED, &q->u.in.queue_irq_state) == 0)
#define queue_irqs_disabled(q)			\
	(test_bit(QDIO_QUEUE_IRQS_DISABLED, &q->u.in.queue_irq_state) != 0)
=======
#define add_buf(bufnr, inc)	QDIO_BUFNR((bufnr) + (inc))
#define next_buf(bufnr)		add_buf(bufnr, 1)
#define sub_buf(bufnr, dec)	QDIO_BUFNR((bufnr) - (dec))
#define prev_buf(bufnr)		sub_buf(bufnr, 1)
>>>>>>> upstream/android-13

extern u64 last_ai_time;

/* prototypes for thin interrupt */
int qdio_establish_thinint(struct qdio_irq *irq_ptr);
void qdio_shutdown_thinint(struct qdio_irq *irq_ptr);
<<<<<<< HEAD
void tiqdio_add_input_queues(struct qdio_irq *irq_ptr);
void tiqdio_remove_input_queues(struct qdio_irq *irq_ptr);
void tiqdio_inbound_processing(unsigned long q);
int tiqdio_allocate_memory(void);
void tiqdio_free_memory(void);
int tiqdio_register_thinints(void);
void tiqdio_unregister_thinints(void);
void clear_nonshared_ind(struct qdio_irq *);
int test_nonshared_ind(struct qdio_irq *);

/* prototypes for setup */
void qdio_inbound_processing(unsigned long data);
void qdio_outbound_processing(unsigned long data);
void qdio_outbound_timer(struct timer_list *t);
=======
int qdio_thinint_init(void);
void qdio_thinint_exit(void);
int test_nonshared_ind(struct qdio_irq *);

/* prototypes for setup */
>>>>>>> upstream/android-13
void qdio_int_handler(struct ccw_device *cdev, unsigned long intparm,
		      struct irb *irb);
int qdio_allocate_qs(struct qdio_irq *irq_ptr, int nr_input_qs,
		     int nr_output_qs);
void qdio_setup_ssqd_info(struct qdio_irq *irq_ptr);
int qdio_setup_get_ssqd(struct qdio_irq *irq_ptr,
			struct subchannel_id *schid,
			struct qdio_ssqd_desc *data);
<<<<<<< HEAD
int qdio_setup_irq(struct qdio_initialize *init_data);
void qdio_print_subchannel_info(struct qdio_irq *irq_ptr,
				struct ccw_device *cdev);
void qdio_release_memory(struct qdio_irq *irq_ptr);
int qdio_setup_create_sysfs(struct ccw_device *cdev);
void qdio_setup_destroy_sysfs(struct ccw_device *cdev);
int qdio_setup_init(void);
void qdio_setup_exit(void);
int qdio_enable_async_operation(struct qdio_output_q *q);
void qdio_disable_async_operation(struct qdio_output_q *q);
struct qaob *qdio_allocate_aob(void);
=======
int qdio_setup_irq(struct qdio_irq *irq_ptr, struct qdio_initialize *init_data);
void qdio_shutdown_irq(struct qdio_irq *irq);
void qdio_print_subchannel_info(struct qdio_irq *irq_ptr);
void qdio_free_queues(struct qdio_irq *irq_ptr);
int qdio_setup_init(void);
void qdio_setup_exit(void);
>>>>>>> upstream/android-13

int debug_get_buf_state(struct qdio_q *q, unsigned int bufnr,
			unsigned char *state);
#endif /* _CIO_QDIO_H */
