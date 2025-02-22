// SPDX-License-Identifier: GPL-2.0
/*
 *    Copyright IBM Corp. 2007, 2009
 *    Author(s): Utz Bacher <utz.bacher@de.ibm.com>,
 *		 Frank Pavlic <fpavlic@de.ibm.com>,
 *		 Thomas Spatzier <tspat@de.ibm.com>,
 *		 Frank Blaschka <frank.blaschka@de.ibm.com>
 */

#define KMSG_COMPONENT "qeth"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/compat.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/mii.h>
=======
#include <linux/log2.h>
#include <linux/io.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/mii.h>
#include <linux/mm.h>
>>>>>>> upstream/android-13
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/if_vlan.h>
#include <linux/netdevice.h>
#include <linux/netdev_features.h>
<<<<<<< HEAD
=======
#include <linux/rcutree.h>
>>>>>>> upstream/android-13
#include <linux/skbuff.h>
#include <linux/vmalloc.h>

#include <net/iucv/af_iucv.h>
#include <net/dsfield.h>
<<<<<<< HEAD

#include <asm/ebcdic.h>
#include <asm/chpid.h>
#include <asm/io.h>
=======
#include <net/sock.h>

#include <asm/ebcdic.h>
#include <asm/chpid.h>
>>>>>>> upstream/android-13
#include <asm/sysinfo.h>
#include <asm/diag.h>
#include <asm/cio.h>
#include <asm/ccwdev.h>
#include <asm/cpcmd.h>

#include "qeth_core.h"

struct qeth_dbf_info qeth_dbf[QETH_DBF_INFOS] = {
	/* define dbf - Name, Pages, Areas, Maxlen, Level, View, Handle */
	/*                   N  P  A    M  L  V                      H  */
	[QETH_DBF_SETUP] = {"qeth_setup",
				8, 1,   8, 5, &debug_hex_ascii_view, NULL},
	[QETH_DBF_MSG]	 = {"qeth_msg", 8, 1, 11 * sizeof(long), 3,
			    &debug_sprintf_view, NULL},
	[QETH_DBF_CTRL]  = {"qeth_control",
		8, 1, QETH_DBF_CTRL_LEN, 5, &debug_hex_ascii_view, NULL},
};
EXPORT_SYMBOL_GPL(qeth_dbf);

<<<<<<< HEAD
struct qeth_card_list_struct qeth_core_card_list;
EXPORT_SYMBOL_GPL(qeth_core_card_list);
struct kmem_cache *qeth_core_header_cache;
EXPORT_SYMBOL_GPL(qeth_core_header_cache);
static struct kmem_cache *qeth_qdio_outbuf_cache;

static struct device *qeth_core_root_dev;
static struct lock_class_key qdio_out_skb_queue_key;
static struct mutex qeth_mod_mutex;

static void qeth_send_control_data_cb(struct qeth_channel *,
			struct qeth_cmd_buffer *);
static struct qeth_cmd_buffer *qeth_get_buffer(struct qeth_channel *);
static void qeth_free_buffer_pool(struct qeth_card *);
static int qeth_qdio_establish(struct qeth_card *);
static void qeth_free_qdio_buffers(struct qeth_card *);
static void qeth_notify_skbs(struct qeth_qdio_out_q *queue,
		struct qeth_qdio_out_buffer *buf,
		enum iucv_tx_notify notification);
static void qeth_release_skbs(struct qeth_qdio_out_buffer *buf);
static int qeth_init_qdio_out_buf(struct qeth_qdio_out_q *, int);

struct workqueue_struct *qeth_wq;
EXPORT_SYMBOL_GPL(qeth_wq);

int qeth_card_hw_is_reachable(struct qeth_card *card)
{
	return (card->state == CARD_STATE_SOFTSETUP) ||
		(card->state == CARD_STATE_UP);
}
EXPORT_SYMBOL_GPL(qeth_card_hw_is_reachable);

static void qeth_close_dev_handler(struct work_struct *work)
{
	struct qeth_card *card;

	card = container_of(work, struct qeth_card, close_dev_work);
	QETH_CARD_TEXT(card, 2, "cldevhdl");
	rtnl_lock();
	dev_close(card->dev);
	rtnl_unlock();
	ccwgroup_set_offline(card->gdev);
}

void qeth_close_dev(struct qeth_card *card)
{
	QETH_CARD_TEXT(card, 2, "cldevsubm");
	queue_work(qeth_wq, &card->close_dev_work);
}
EXPORT_SYMBOL_GPL(qeth_close_dev);

static const char *qeth_get_cardname(struct qeth_card *card)
{
	if (card->info.guestlan) {
=======
static struct kmem_cache *qeth_core_header_cache;
static struct kmem_cache *qeth_qdio_outbuf_cache;

static struct device *qeth_core_root_dev;
static struct dentry *qeth_debugfs_root;
static struct lock_class_key qdio_out_skb_queue_key;

static void qeth_issue_next_read_cb(struct qeth_card *card,
				    struct qeth_cmd_buffer *iob,
				    unsigned int data_length);
static int qeth_qdio_establish(struct qeth_card *);
static void qeth_free_qdio_queues(struct qeth_card *card);

static const char *qeth_get_cardname(struct qeth_card *card)
{
	if (IS_VM_NIC(card)) {
>>>>>>> upstream/android-13
		switch (card->info.type) {
		case QETH_CARD_TYPE_OSD:
			return " Virtual NIC QDIO";
		case QETH_CARD_TYPE_IQD:
			return " Virtual NIC Hiper";
		case QETH_CARD_TYPE_OSM:
			return " Virtual NIC QDIO - OSM";
		case QETH_CARD_TYPE_OSX:
			return " Virtual NIC QDIO - OSX";
		default:
			return " unknown";
		}
	} else {
		switch (card->info.type) {
		case QETH_CARD_TYPE_OSD:
			return " OSD Express";
		case QETH_CARD_TYPE_IQD:
			return " HiperSockets";
<<<<<<< HEAD
		case QETH_CARD_TYPE_OSN:
			return " OSN QDIO";
=======
>>>>>>> upstream/android-13
		case QETH_CARD_TYPE_OSM:
			return " OSM QDIO";
		case QETH_CARD_TYPE_OSX:
			return " OSX QDIO";
		default:
			return " unknown";
		}
	}
	return " n/a";
}

/* max length to be returned: 14 */
const char *qeth_get_cardname_short(struct qeth_card *card)
{
<<<<<<< HEAD
	if (card->info.guestlan) {
=======
	if (IS_VM_NIC(card)) {
>>>>>>> upstream/android-13
		switch (card->info.type) {
		case QETH_CARD_TYPE_OSD:
			return "Virt.NIC QDIO";
		case QETH_CARD_TYPE_IQD:
			return "Virt.NIC Hiper";
		case QETH_CARD_TYPE_OSM:
			return "Virt.NIC OSM";
		case QETH_CARD_TYPE_OSX:
			return "Virt.NIC OSX";
		default:
			return "unknown";
		}
	} else {
		switch (card->info.type) {
		case QETH_CARD_TYPE_OSD:
			switch (card->info.link_type) {
			case QETH_LINK_TYPE_FAST_ETH:
				return "OSD_100";
			case QETH_LINK_TYPE_HSTR:
				return "HSTR";
			case QETH_LINK_TYPE_GBIT_ETH:
				return "OSD_1000";
			case QETH_LINK_TYPE_10GBIT_ETH:
				return "OSD_10GIG";
<<<<<<< HEAD
=======
			case QETH_LINK_TYPE_25GBIT_ETH:
				return "OSD_25GIG";
>>>>>>> upstream/android-13
			case QETH_LINK_TYPE_LANE_ETH100:
				return "OSD_FE_LANE";
			case QETH_LINK_TYPE_LANE_TR:
				return "OSD_TR_LANE";
			case QETH_LINK_TYPE_LANE_ETH1000:
				return "OSD_GbE_LANE";
			case QETH_LINK_TYPE_LANE:
				return "OSD_ATM_LANE";
			default:
				return "OSD_Express";
			}
		case QETH_CARD_TYPE_IQD:
			return "HiperSockets";
<<<<<<< HEAD
		case QETH_CARD_TYPE_OSN:
			return "OSN";
=======
>>>>>>> upstream/android-13
		case QETH_CARD_TYPE_OSM:
			return "OSM_1000";
		case QETH_CARD_TYPE_OSX:
			return "OSX_10GIG";
		default:
			return "unknown";
		}
	}
	return "n/a";
}

<<<<<<< HEAD
void qeth_set_recovery_task(struct qeth_card *card)
{
	card->recovery_task = current;
}
EXPORT_SYMBOL_GPL(qeth_set_recovery_task);

void qeth_clear_recovery_task(struct qeth_card *card)
{
	card->recovery_task = NULL;
}
EXPORT_SYMBOL_GPL(qeth_clear_recovery_task);

static bool qeth_is_recovery_task(const struct qeth_card *card)
{
	return card->recovery_task == current;
}

=======
>>>>>>> upstream/android-13
void qeth_set_allowed_threads(struct qeth_card *card, unsigned long threads,
			 int clear_start_mask)
{
	unsigned long flags;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	card->thread_allowed_mask = threads;
	if (clear_start_mask)
		card->thread_start_mask &= threads;
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	wake_up(&card->wait_q);
}
EXPORT_SYMBOL_GPL(qeth_set_allowed_threads);

int qeth_threads_running(struct qeth_card *card, unsigned long threads)
{
	unsigned long flags;
	int rc = 0;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	rc = (card->thread_running_mask & threads);
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_threads_running);

<<<<<<< HEAD
int qeth_wait_for_threads(struct qeth_card *card, unsigned long threads)
{
	if (qeth_is_recovery_task(card))
		return 0;
	return wait_event_interruptible(card->wait_q,
			qeth_threads_running(card, threads) == 0);
}
EXPORT_SYMBOL_GPL(qeth_wait_for_threads);

void qeth_clear_working_pool_list(struct qeth_card *card)
{
	struct qeth_buffer_pool_entry *pool_entry, *tmp;

	QETH_CARD_TEXT(card, 5, "clwrklst");
	list_for_each_entry_safe(pool_entry, tmp,
			    &card->qdio.in_buf_pool.entry_list, list){
			list_del(&pool_entry->list);
	}
}
EXPORT_SYMBOL_GPL(qeth_clear_working_pool_list);

static int qeth_alloc_buffer_pool(struct qeth_card *card)
{
	struct qeth_buffer_pool_entry *pool_entry;
	void *ptr;
	int i, j;

	QETH_CARD_TEXT(card, 5, "alocpool");
	for (i = 0; i < card->qdio.init_pool.buf_count; ++i) {
		pool_entry = kzalloc(sizeof(*pool_entry), GFP_KERNEL);
		if (!pool_entry) {
			qeth_free_buffer_pool(card);
			return -ENOMEM;
		}
		for (j = 0; j < QETH_MAX_BUFFER_ELEMENTS(card); ++j) {
			ptr = (void *) __get_free_page(GFP_KERNEL);
			if (!ptr) {
				while (j > 0)
					free_page((unsigned long)
						  pool_entry->elements[--j]);
				kfree(pool_entry);
				qeth_free_buffer_pool(card);
				return -ENOMEM;
			}
			pool_entry->elements[j] = ptr;
		}
		list_add(&pool_entry->init_list,
			 &card->qdio.init_pool.entry_list);
=======
static void qeth_clear_working_pool_list(struct qeth_card *card)
{
	struct qeth_buffer_pool_entry *pool_entry, *tmp;
	struct qeth_qdio_q *queue = card->qdio.in_q;
	unsigned int i;

	QETH_CARD_TEXT(card, 5, "clwrklst");
	list_for_each_entry_safe(pool_entry, tmp,
				 &card->qdio.in_buf_pool.entry_list, list)
		list_del(&pool_entry->list);

	if (!queue)
		return;

	for (i = 0; i < ARRAY_SIZE(queue->bufs); i++)
		queue->bufs[i].pool_entry = NULL;
}

static void qeth_free_pool_entry(struct qeth_buffer_pool_entry *entry)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(entry->elements); i++) {
		if (entry->elements[i])
			__free_page(entry->elements[i]);
	}

	kfree(entry);
}

static void qeth_free_buffer_pool(struct qeth_card *card)
{
	struct qeth_buffer_pool_entry *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &card->qdio.init_pool.entry_list,
				 init_list) {
		list_del(&entry->init_list);
		qeth_free_pool_entry(entry);
	}
}

static struct qeth_buffer_pool_entry *qeth_alloc_pool_entry(unsigned int pages)
{
	struct qeth_buffer_pool_entry *entry;
	unsigned int i;

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return NULL;

	for (i = 0; i < pages; i++) {
		entry->elements[i] = __dev_alloc_page(GFP_KERNEL);

		if (!entry->elements[i]) {
			qeth_free_pool_entry(entry);
			return NULL;
		}
	}

	return entry;
}

static int qeth_alloc_buffer_pool(struct qeth_card *card)
{
	unsigned int buf_elements = QETH_MAX_BUFFER_ELEMENTS(card);
	unsigned int i;

	QETH_CARD_TEXT(card, 5, "alocpool");
	for (i = 0; i < card->qdio.init_pool.buf_count; ++i) {
		struct qeth_buffer_pool_entry *entry;

		entry = qeth_alloc_pool_entry(buf_elements);
		if (!entry) {
			qeth_free_buffer_pool(card);
			return -ENOMEM;
		}

		list_add(&entry->init_list, &card->qdio.init_pool.entry_list);
>>>>>>> upstream/android-13
	}
	return 0;
}

<<<<<<< HEAD
int qeth_realloc_buffer_pool(struct qeth_card *card, int bufcnt)
{
	QETH_CARD_TEXT(card, 2, "realcbp");

	if ((card->state != CARD_STATE_DOWN) &&
	    (card->state != CARD_STATE_RECOVER))
		return -EPERM;

	/* TODO: steel/add buffers from/to a running card's buffer pool (?) */
	qeth_clear_working_pool_list(card);
	qeth_free_buffer_pool(card);
	card->qdio.in_buf_pool.buf_count = bufcnt;
	card->qdio.init_pool.buf_count = bufcnt;
	return qeth_alloc_buffer_pool(card);
}
EXPORT_SYMBOL_GPL(qeth_realloc_buffer_pool);
=======
int qeth_resize_buffer_pool(struct qeth_card *card, unsigned int count)
{
	unsigned int buf_elements = QETH_MAX_BUFFER_ELEMENTS(card);
	struct qeth_qdio_buffer_pool *pool = &card->qdio.init_pool;
	struct qeth_buffer_pool_entry *entry, *tmp;
	int delta = count - pool->buf_count;
	LIST_HEAD(entries);

	QETH_CARD_TEXT(card, 2, "realcbp");

	/* Defer until queue is allocated: */
	if (!card->qdio.in_q)
		goto out;

	/* Remove entries from the pool: */
	while (delta < 0) {
		entry = list_first_entry(&pool->entry_list,
					 struct qeth_buffer_pool_entry,
					 init_list);
		list_del(&entry->init_list);
		qeth_free_pool_entry(entry);

		delta++;
	}

	/* Allocate additional entries: */
	while (delta > 0) {
		entry = qeth_alloc_pool_entry(buf_elements);
		if (!entry) {
			list_for_each_entry_safe(entry, tmp, &entries,
						 init_list) {
				list_del(&entry->init_list);
				qeth_free_pool_entry(entry);
			}

			return -ENOMEM;
		}

		list_add(&entry->init_list, &entries);

		delta--;
	}

	list_splice(&entries, &pool->entry_list);

out:
	card->qdio.in_buf_pool.buf_count = count;
	pool->buf_count = count;
	return 0;
}
EXPORT_SYMBOL_GPL(qeth_resize_buffer_pool);
>>>>>>> upstream/android-13

static void qeth_free_qdio_queue(struct qeth_qdio_q *q)
{
	if (!q)
		return;

	qdio_free_buffers(q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q);
	kfree(q);
}

static struct qeth_qdio_q *qeth_alloc_qdio_queue(void)
{
	struct qeth_qdio_q *q = kzalloc(sizeof(*q), GFP_KERNEL);
	int i;

	if (!q)
		return NULL;

	if (qdio_alloc_buffers(q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q)) {
		kfree(q);
		return NULL;
	}

	for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; ++i)
		q->bufs[i].buffer = q->qdio_bufs[i];

	QETH_DBF_HEX(SETUP, 2, &q, sizeof(void *));
	return q;
}

static int qeth_cq_init(struct qeth_card *card)
{
	int rc;

	if (card->options.cq == QETH_CQ_ENABLED) {
<<<<<<< HEAD
		QETH_DBF_TEXT(SETUP, 2, "cqinit");
=======
		QETH_CARD_TEXT(card, 2, "cqinit");
>>>>>>> upstream/android-13
		qdio_reset_buffers(card->qdio.c_q->qdio_bufs,
				   QDIO_MAX_BUFFERS_PER_Q);
		card->qdio.c_q->next_buf_to_init = 127;
		rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT,
<<<<<<< HEAD
			     card->qdio.no_in_queues - 1, 0,
			     127);
		if (rc) {
			QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
=======
			     card->qdio.no_in_queues - 1, 0, 127, NULL);
		if (rc) {
			QETH_CARD_TEXT_(card, 2, "1err%d", rc);
>>>>>>> upstream/android-13
			goto out;
		}
	}
	rc = 0;
out:
	return rc;
}

static int qeth_alloc_cq(struct qeth_card *card)
{
<<<<<<< HEAD
	int rc;

	if (card->options.cq == QETH_CQ_ENABLED) {
		int i;
		struct qdio_outbuf_state *outbuf_states;

		QETH_DBF_TEXT(SETUP, 2, "cqon");
		card->qdio.c_q = qeth_alloc_qdio_queue();
		if (!card->qdio.c_q) {
			rc = -1;
			goto kmsg_out;
		}
		card->qdio.no_in_queues = 2;
		card->qdio.out_bufstates =
			kcalloc(card->qdio.no_out_queues *
					QDIO_MAX_BUFFERS_PER_Q,
				sizeof(struct qdio_outbuf_state),
				GFP_KERNEL);
		outbuf_states = card->qdio.out_bufstates;
		if (outbuf_states == NULL) {
			rc = -1;
			goto free_cq_out;
		}
		for (i = 0; i < card->qdio.no_out_queues; ++i) {
			card->qdio.out_qs[i]->bufstates = outbuf_states;
			outbuf_states += QDIO_MAX_BUFFERS_PER_Q;
		}
	} else {
		QETH_DBF_TEXT(SETUP, 2, "nocq");
		card->qdio.c_q = NULL;
		card->qdio.no_in_queues = 1;
	}
	QETH_DBF_TEXT_(SETUP, 2, "iqc%d", card->qdio.no_in_queues);
	rc = 0;
out:
	return rc;
free_cq_out:
	qeth_free_qdio_queue(card->qdio.c_q);
	card->qdio.c_q = NULL;
kmsg_out:
	dev_err(&card->gdev->dev, "Failed to create completion queue\n");
	goto out;
=======
	if (card->options.cq == QETH_CQ_ENABLED) {
		QETH_CARD_TEXT(card, 2, "cqon");
		card->qdio.c_q = qeth_alloc_qdio_queue();
		if (!card->qdio.c_q) {
			dev_err(&card->gdev->dev, "Failed to create completion queue\n");
			return -ENOMEM;
		}

		card->qdio.no_in_queues = 2;
	} else {
		QETH_CARD_TEXT(card, 2, "nocq");
		card->qdio.c_q = NULL;
		card->qdio.no_in_queues = 1;
	}
	QETH_CARD_TEXT_(card, 2, "iqc%d", card->qdio.no_in_queues);
	return 0;
>>>>>>> upstream/android-13
}

static void qeth_free_cq(struct qeth_card *card)
{
	if (card->qdio.c_q) {
		--card->qdio.no_in_queues;
		qeth_free_qdio_queue(card->qdio.c_q);
		card->qdio.c_q = NULL;
	}
<<<<<<< HEAD
	kfree(card->qdio.out_bufstates);
	card->qdio.out_bufstates = NULL;
=======
>>>>>>> upstream/android-13
}

static enum iucv_tx_notify qeth_compute_cq_notification(int sbalf15,
							int delayed)
{
	enum iucv_tx_notify n;

	switch (sbalf15) {
	case 0:
		n = delayed ? TX_NOTIFY_DELAYED_OK : TX_NOTIFY_OK;
		break;
	case 4:
	case 16:
	case 17:
	case 18:
		n = delayed ? TX_NOTIFY_DELAYED_UNREACHABLE :
			TX_NOTIFY_UNREACHABLE;
		break;
	default:
		n = delayed ? TX_NOTIFY_DELAYED_GENERALERROR :
			TX_NOTIFY_GENERALERROR;
		break;
	}

	return n;
}

<<<<<<< HEAD
static void qeth_cleanup_handled_pending(struct qeth_qdio_out_q *q, int bidx,
					 int forced_cleanup)
{
	if (q->card->options.cq != QETH_CQ_ENABLED)
		return;

	if (q->bufs[bidx]->next_pending != NULL) {
		struct qeth_qdio_out_buffer *head = q->bufs[bidx];
		struct qeth_qdio_out_buffer *c = q->bufs[bidx]->next_pending;

		while (c) {
			if (forced_cleanup ||
			    atomic_read(&c->state) ==
			      QETH_QDIO_BUF_HANDLED_DELAYED) {
				struct qeth_qdio_out_buffer *f = c;
				QETH_CARD_TEXT(f->q->card, 5, "fp");
				QETH_CARD_TEXT_(f->q->card, 5, "%lx", (long) f);
				/* release here to avoid interleaving between
				   outbound tasklet and inbound tasklet
				   regarding notifications and lifecycle */
				qeth_release_skbs(c);

				c = f->next_pending;
				WARN_ON_ONCE(head->next_pending != f);
				head->next_pending = c;
				kmem_cache_free(qeth_qdio_outbuf_cache, f);
			} else {
				head = c;
				c = c->next_pending;
			}

		}
	}
}


static void qeth_qdio_handle_aob(struct qeth_card *card,
				 unsigned long phys_aob_addr)
{
	struct qaob *aob;
	struct qeth_qdio_out_buffer *buffer;
	enum iucv_tx_notify notification;
	unsigned int i;

	aob = (struct qaob *) phys_to_virt(phys_aob_addr);
	QETH_CARD_TEXT(card, 5, "haob");
	QETH_CARD_TEXT_(card, 5, "%lx", phys_aob_addr);
	buffer = (struct qeth_qdio_out_buffer *) aob->user1;
	QETH_CARD_TEXT_(card, 5, "%lx", aob->user1);

	if (atomic_cmpxchg(&buffer->state, QETH_QDIO_BUF_PRIMED,
			   QETH_QDIO_BUF_IN_CQ) == QETH_QDIO_BUF_PRIMED) {
		notification = TX_NOTIFY_OK;
	} else {
		WARN_ON_ONCE(atomic_read(&buffer->state) !=
							QETH_QDIO_BUF_PENDING);
		atomic_set(&buffer->state, QETH_QDIO_BUF_IN_CQ);
		notification = TX_NOTIFY_DELAYED_OK;
	}

	if (aob->aorc != 0)  {
		QETH_CARD_TEXT_(card, 2, "aorc%02X", aob->aorc);
		notification = qeth_compute_cq_notification(aob->aorc, 1);
	}
	qeth_notify_skbs(buffer->q, buffer, notification);

	/* Free dangling allocations. The attached skbs are handled by
	 * qeth_cleanup_handled_pending().
	 */
	for (i = 0;
	     i < aob->sb_count && i < QETH_MAX_BUFFER_ELEMENTS(card);
	     i++) {
		if (aob->sba[i] && buffer->is_header[i])
			kmem_cache_free(qeth_core_header_cache,
					(void *) aob->sba[i]);
	}
	atomic_set(&buffer->state, QETH_QDIO_BUF_HANDLED_DELAYED);

	qdio_release_aob(aob);
}

static inline int qeth_is_cq(struct qeth_card *card, unsigned int queue)
{
	return card->options.cq == QETH_CQ_ENABLED &&
	    card->qdio.c_q != NULL &&
	    queue != 0 &&
	    queue == card->qdio.no_in_queues - 1;
}

static void qeth_setup_ccw(struct ccw1 *ccw, u8 cmd_code, u32 len, void *data)
{
	ccw->cmd_code = cmd_code;
	ccw->flags = CCW_FLAG_SLI;
=======
static void qeth_put_cmd(struct qeth_cmd_buffer *iob)
{
	if (refcount_dec_and_test(&iob->ref_count)) {
		kfree(iob->data);
		kfree(iob);
	}
}
static void qeth_setup_ccw(struct ccw1 *ccw, u8 cmd_code, u8 flags, u32 len,
			   void *data)
{
	ccw->cmd_code = cmd_code;
	ccw->flags = flags | CCW_FLAG_SLI;
>>>>>>> upstream/android-13
	ccw->count = len;
	ccw->cda = (__u32) __pa(data);
}

static int __qeth_issue_next_read(struct qeth_card *card)
{
<<<<<<< HEAD
	struct qeth_channel *channel = &card->read;
	struct qeth_cmd_buffer *iob;
=======
	struct qeth_cmd_buffer *iob = card->read_cmd;
	struct qeth_channel *channel = iob->channel;
	struct ccw1 *ccw = __ccw_from_cmd(iob);
>>>>>>> upstream/android-13
	int rc;

	QETH_CARD_TEXT(card, 5, "issnxrd");
	if (channel->state != CH_STATE_UP)
		return -EIO;
<<<<<<< HEAD
	iob = qeth_get_buffer(channel);
	if (!iob) {
		dev_warn(&card->gdev->dev, "The qeth device driver "
			"failed to recover an error on the device\n");
		QETH_DBF_MESSAGE(2, "%s issue_next_read failed: no iob "
			"available\n", dev_name(&card->gdev->dev));
		return -ENOMEM;
	}
	qeth_setup_ccw(channel->ccw, CCW_CMD_READ, QETH_BUFSIZE, iob->data);
	QETH_CARD_TEXT(card, 6, "noirqpnd");
	rc = ccw_device_start(channel->ccwdev, channel->ccw,
			      (addr_t) iob, 0, 0);
	if (rc) {
		QETH_DBF_MESSAGE(2, "%s error in starting next read ccw! "
			"rc=%i\n", dev_name(&card->gdev->dev), rc);
		atomic_set(&channel->irq_pending, 0);
		qeth_release_buffer(channel, iob);
		card->read_or_write_problem = 1;
		qeth_schedule_recovery(card);
		wake_up(&card->wait_q);
=======

	memset(iob->data, 0, iob->length);
	qeth_setup_ccw(ccw, CCW_CMD_READ, 0, iob->length, iob->data);
	iob->callback = qeth_issue_next_read_cb;
	/* keep the cmd alive after completion: */
	qeth_get_cmd(iob);

	QETH_CARD_TEXT(card, 6, "noirqpnd");
	rc = ccw_device_start(channel->ccwdev, ccw, (addr_t) iob, 0, 0);
	if (!rc) {
		channel->active_cmd = iob;
	} else {
		QETH_DBF_MESSAGE(2, "error %i on device %x when starting next read ccw!\n",
				 rc, CARD_DEVID(card));
		qeth_unlock_channel(card, channel);
		qeth_put_cmd(iob);
		card->read_or_write_problem = 1;
		qeth_schedule_recovery(card);
>>>>>>> upstream/android-13
	}
	return rc;
}

static int qeth_issue_next_read(struct qeth_card *card)
{
	int ret;

	spin_lock_irq(get_ccwdev_lock(CARD_RDEV(card)));
	ret = __qeth_issue_next_read(card);
	spin_unlock_irq(get_ccwdev_lock(CARD_RDEV(card)));

	return ret;
}

<<<<<<< HEAD
static struct qeth_reply *qeth_alloc_reply(struct qeth_card *card)
{
	struct qeth_reply *reply;

	reply = kzalloc(sizeof(struct qeth_reply), GFP_ATOMIC);
	if (reply) {
		refcount_set(&reply->refcnt, 1);
		atomic_set(&reply->received, 0);
		reply->card = card;
	}
	return reply;
}

static void qeth_get_reply(struct qeth_reply *reply)
{
	refcount_inc(&reply->refcnt);
}

static void qeth_put_reply(struct qeth_reply *reply)
{
	if (refcount_dec_and_test(&reply->refcnt))
		kfree(reply);
}

=======
static void qeth_enqueue_cmd(struct qeth_card *card,
			     struct qeth_cmd_buffer *iob)
{
	spin_lock_irq(&card->lock);
	list_add_tail(&iob->list_entry, &card->cmd_waiter_list);
	spin_unlock_irq(&card->lock);
}

static void qeth_dequeue_cmd(struct qeth_card *card,
			     struct qeth_cmd_buffer *iob)
{
	spin_lock_irq(&card->lock);
	list_del(&iob->list_entry);
	spin_unlock_irq(&card->lock);
}

static void qeth_notify_cmd(struct qeth_cmd_buffer *iob, int reason)
{
	iob->rc = reason;
	complete(&iob->done);
}

static void qeth_flush_local_addrs4(struct qeth_card *card)
{
	struct qeth_local_addr *addr;
	struct hlist_node *tmp;
	unsigned int i;

	spin_lock_irq(&card->local_addrs4_lock);
	hash_for_each_safe(card->local_addrs4, i, tmp, addr, hnode) {
		hash_del_rcu(&addr->hnode);
		kfree_rcu(addr, rcu);
	}
	spin_unlock_irq(&card->local_addrs4_lock);
}

static void qeth_flush_local_addrs6(struct qeth_card *card)
{
	struct qeth_local_addr *addr;
	struct hlist_node *tmp;
	unsigned int i;

	spin_lock_irq(&card->local_addrs6_lock);
	hash_for_each_safe(card->local_addrs6, i, tmp, addr, hnode) {
		hash_del_rcu(&addr->hnode);
		kfree_rcu(addr, rcu);
	}
	spin_unlock_irq(&card->local_addrs6_lock);
}

static void qeth_flush_local_addrs(struct qeth_card *card)
{
	qeth_flush_local_addrs4(card);
	qeth_flush_local_addrs6(card);
}

static void qeth_add_local_addrs4(struct qeth_card *card,
				  struct qeth_ipacmd_local_addrs4 *cmd)
{
	unsigned int i;

	if (cmd->addr_length !=
	    sizeof_field(struct qeth_ipacmd_local_addr4, addr)) {
		dev_err_ratelimited(&card->gdev->dev,
				    "Dropped IPv4 ADD LOCAL ADDR event with bad length %u\n",
				    cmd->addr_length);
		return;
	}

	spin_lock(&card->local_addrs4_lock);
	for (i = 0; i < cmd->count; i++) {
		unsigned int key = ipv4_addr_hash(cmd->addrs[i].addr);
		struct qeth_local_addr *addr;
		bool duplicate = false;

		hash_for_each_possible(card->local_addrs4, addr, hnode, key) {
			if (addr->addr.s6_addr32[3] == cmd->addrs[i].addr) {
				duplicate = true;
				break;
			}
		}

		if (duplicate)
			continue;

		addr = kmalloc(sizeof(*addr), GFP_ATOMIC);
		if (!addr) {
			dev_err(&card->gdev->dev,
				"Failed to allocate local addr object. Traffic to %pI4 might suffer.\n",
				&cmd->addrs[i].addr);
			continue;
		}

		ipv6_addr_set(&addr->addr, 0, 0, 0, cmd->addrs[i].addr);
		hash_add_rcu(card->local_addrs4, &addr->hnode, key);
	}
	spin_unlock(&card->local_addrs4_lock);
}

static void qeth_add_local_addrs6(struct qeth_card *card,
				  struct qeth_ipacmd_local_addrs6 *cmd)
{
	unsigned int i;

	if (cmd->addr_length !=
	    sizeof_field(struct qeth_ipacmd_local_addr6, addr)) {
		dev_err_ratelimited(&card->gdev->dev,
				    "Dropped IPv6 ADD LOCAL ADDR event with bad length %u\n",
				    cmd->addr_length);
		return;
	}

	spin_lock(&card->local_addrs6_lock);
	for (i = 0; i < cmd->count; i++) {
		u32 key = ipv6_addr_hash(&cmd->addrs[i].addr);
		struct qeth_local_addr *addr;
		bool duplicate = false;

		hash_for_each_possible(card->local_addrs6, addr, hnode, key) {
			if (ipv6_addr_equal(&addr->addr, &cmd->addrs[i].addr)) {
				duplicate = true;
				break;
			}
		}

		if (duplicate)
			continue;

		addr = kmalloc(sizeof(*addr), GFP_ATOMIC);
		if (!addr) {
			dev_err(&card->gdev->dev,
				"Failed to allocate local addr object. Traffic to %pI6c might suffer.\n",
				&cmd->addrs[i].addr);
			continue;
		}

		addr->addr = cmd->addrs[i].addr;
		hash_add_rcu(card->local_addrs6, &addr->hnode, key);
	}
	spin_unlock(&card->local_addrs6_lock);
}

static void qeth_del_local_addrs4(struct qeth_card *card,
				  struct qeth_ipacmd_local_addrs4 *cmd)
{
	unsigned int i;

	if (cmd->addr_length !=
	    sizeof_field(struct qeth_ipacmd_local_addr4, addr)) {
		dev_err_ratelimited(&card->gdev->dev,
				    "Dropped IPv4 DEL LOCAL ADDR event with bad length %u\n",
				    cmd->addr_length);
		return;
	}

	spin_lock(&card->local_addrs4_lock);
	for (i = 0; i < cmd->count; i++) {
		struct qeth_ipacmd_local_addr4 *addr = &cmd->addrs[i];
		unsigned int key = ipv4_addr_hash(addr->addr);
		struct qeth_local_addr *tmp;

		hash_for_each_possible(card->local_addrs4, tmp, hnode, key) {
			if (tmp->addr.s6_addr32[3] == addr->addr) {
				hash_del_rcu(&tmp->hnode);
				kfree_rcu(tmp, rcu);
				break;
			}
		}
	}
	spin_unlock(&card->local_addrs4_lock);
}

static void qeth_del_local_addrs6(struct qeth_card *card,
				  struct qeth_ipacmd_local_addrs6 *cmd)
{
	unsigned int i;

	if (cmd->addr_length !=
	    sizeof_field(struct qeth_ipacmd_local_addr6, addr)) {
		dev_err_ratelimited(&card->gdev->dev,
				    "Dropped IPv6 DEL LOCAL ADDR event with bad length %u\n",
				    cmd->addr_length);
		return;
	}

	spin_lock(&card->local_addrs6_lock);
	for (i = 0; i < cmd->count; i++) {
		struct qeth_ipacmd_local_addr6 *addr = &cmd->addrs[i];
		u32 key = ipv6_addr_hash(&addr->addr);
		struct qeth_local_addr *tmp;

		hash_for_each_possible(card->local_addrs6, tmp, hnode, key) {
			if (ipv6_addr_equal(&tmp->addr, &addr->addr)) {
				hash_del_rcu(&tmp->hnode);
				kfree_rcu(tmp, rcu);
				break;
			}
		}
	}
	spin_unlock(&card->local_addrs6_lock);
}

static bool qeth_next_hop_is_local_v4(struct qeth_card *card,
				      struct sk_buff *skb)
{
	struct qeth_local_addr *tmp;
	bool is_local = false;
	unsigned int key;
	__be32 next_hop;

	if (hash_empty(card->local_addrs4))
		return false;

	rcu_read_lock();
	next_hop = qeth_next_hop_v4_rcu(skb,
					qeth_dst_check_rcu(skb, htons(ETH_P_IP)));
	key = ipv4_addr_hash(next_hop);

	hash_for_each_possible_rcu(card->local_addrs4, tmp, hnode, key) {
		if (tmp->addr.s6_addr32[3] == next_hop) {
			is_local = true;
			break;
		}
	}
	rcu_read_unlock();

	return is_local;
}

static bool qeth_next_hop_is_local_v6(struct qeth_card *card,
				      struct sk_buff *skb)
{
	struct qeth_local_addr *tmp;
	struct in6_addr *next_hop;
	bool is_local = false;
	u32 key;

	if (hash_empty(card->local_addrs6))
		return false;

	rcu_read_lock();
	next_hop = qeth_next_hop_v6_rcu(skb,
					qeth_dst_check_rcu(skb, htons(ETH_P_IPV6)));
	key = ipv6_addr_hash(next_hop);

	hash_for_each_possible_rcu(card->local_addrs6, tmp, hnode, key) {
		if (ipv6_addr_equal(&tmp->addr, next_hop)) {
			is_local = true;
			break;
		}
	}
	rcu_read_unlock();

	return is_local;
}

static int qeth_debugfs_local_addr_show(struct seq_file *m, void *v)
{
	struct qeth_card *card = m->private;
	struct qeth_local_addr *tmp;
	unsigned int i;

	rcu_read_lock();
	hash_for_each_rcu(card->local_addrs4, i, tmp, hnode)
		seq_printf(m, "%pI4\n", &tmp->addr.s6_addr32[3]);
	hash_for_each_rcu(card->local_addrs6, i, tmp, hnode)
		seq_printf(m, "%pI6c\n", &tmp->addr);
	rcu_read_unlock();

	return 0;
}

DEFINE_SHOW_ATTRIBUTE(qeth_debugfs_local_addr);

>>>>>>> upstream/android-13
static void qeth_issue_ipa_msg(struct qeth_ipa_cmd *cmd, int rc,
		struct qeth_card *card)
{
	const char *ipa_name;
	int com = cmd->hdr.command;
<<<<<<< HEAD
	ipa_name = qeth_get_ipa_cmd_name(com);
	if (rc)
		QETH_DBF_MESSAGE(2, "IPA: %s(x%X) for %s/%s returned "
				"x%X \"%s\"\n",
				ipa_name, com, dev_name(&card->gdev->dev),
				QETH_CARD_IFNAME(card), rc,
				qeth_get_ipa_msg(rc));
	else
		QETH_DBF_MESSAGE(5, "IPA: %s(x%X) for %s/%s succeeded\n",
				ipa_name, com, dev_name(&card->gdev->dev),
				QETH_CARD_IFNAME(card));
}

static struct qeth_ipa_cmd *qeth_check_ipa_data(struct qeth_card *card,
		struct qeth_cmd_buffer *iob)
{
	struct qeth_ipa_cmd *cmd = NULL;

	QETH_CARD_TEXT(card, 5, "chkipad");
	if (IS_IPA(iob->data)) {
		cmd = (struct qeth_ipa_cmd *) PDU_ENCAPSULATION(iob->data);
		if (IS_IPA_REPLY(cmd)) {
			if (cmd->hdr.command != IPA_CMD_SETCCID &&
			    cmd->hdr.command != IPA_CMD_DELCCID &&
			    cmd->hdr.command != IPA_CMD_MODCCID &&
			    cmd->hdr.command != IPA_CMD_SET_DIAG_ASS)
				qeth_issue_ipa_msg(cmd,
						cmd->hdr.return_code, card);
			return cmd;
		} else {
			switch (cmd->hdr.command) {
			case IPA_CMD_STOPLAN:
				if (cmd->hdr.return_code ==
						IPA_RC_VEPA_TO_VEB_TRANSITION) {
					dev_err(&card->gdev->dev,
					   "Interface %s is down because the "
					   "adjacent port is no longer in "
					   "reflective relay mode\n",
					   QETH_CARD_IFNAME(card));
					qeth_close_dev(card);
				} else {
					dev_warn(&card->gdev->dev,
					   "The link for interface %s on CHPID"
					   " 0x%X failed\n",
					   QETH_CARD_IFNAME(card),
					   card->info.chpid);
					qeth_issue_ipa_msg(cmd,
						cmd->hdr.return_code, card);
				}
				card->lan_online = 0;
				netif_carrier_off(card->dev);
				return NULL;
			case IPA_CMD_STARTLAN:
				dev_info(&card->gdev->dev,
					   "The link for %s on CHPID 0x%X has"
					   " been restored\n",
					   QETH_CARD_IFNAME(card),
					   card->info.chpid);
				netif_carrier_on(card->dev);
				card->lan_online = 1;
				if (card->info.hwtrap)
					card->info.hwtrap = 2;
				qeth_schedule_recovery(card);
				return NULL;
			case IPA_CMD_SETBRIDGEPORT_IQD:
			case IPA_CMD_SETBRIDGEPORT_OSA:
			case IPA_CMD_ADDRESS_CHANGE_NOTIF:
				if (card->discipline->control_event_handler
								(card, cmd))
					return cmd;
				else
					return NULL;
			case IPA_CMD_MODCCID:
				return cmd;
			case IPA_CMD_REGISTER_LOCAL_ADDR:
				QETH_CARD_TEXT(card, 3, "irla");
				break;
			case IPA_CMD_UNREGISTER_LOCAL_ADDR:
				QETH_CARD_TEXT(card, 3, "urla");
				break;
			default:
				QETH_DBF_MESSAGE(2, "Received data is IPA "
					   "but not a reply!\n");
				break;
			}
		}
	}
	return cmd;
}

void qeth_clear_ipacmd_list(struct qeth_card *card)
{
	struct qeth_reply *reply, *r;
=======

	ipa_name = qeth_get_ipa_cmd_name(com);

	if (rc)
		QETH_DBF_MESSAGE(2, "IPA: %s(%#x) for device %x returned %#x \"%s\"\n",
				 ipa_name, com, CARD_DEVID(card), rc,
				 qeth_get_ipa_msg(rc));
	else
		QETH_DBF_MESSAGE(5, "IPA: %s(%#x) for device %x succeeded\n",
				 ipa_name, com, CARD_DEVID(card));
}

static struct qeth_ipa_cmd *qeth_check_ipa_data(struct qeth_card *card,
						struct qeth_ipa_cmd *cmd)
{
	QETH_CARD_TEXT(card, 5, "chkipad");

	if (IS_IPA_REPLY(cmd)) {
		if (cmd->hdr.command != IPA_CMD_SET_DIAG_ASS)
			qeth_issue_ipa_msg(cmd, cmd->hdr.return_code, card);
		return cmd;
	}

	/* handle unsolicited event: */
	switch (cmd->hdr.command) {
	case IPA_CMD_STOPLAN:
		if (cmd->hdr.return_code == IPA_RC_VEPA_TO_VEB_TRANSITION) {
			dev_err(&card->gdev->dev,
				"Adjacent port of interface %s is no longer in reflective relay mode, trigger recovery\n",
				netdev_name(card->dev));
			/* Set offline, then probably fail to set online: */
			qeth_schedule_recovery(card);
		} else {
			/* stay online for subsequent STARTLAN */
			dev_warn(&card->gdev->dev,
				 "The link for interface %s on CHPID 0x%X failed\n",
				 netdev_name(card->dev), card->info.chpid);
			qeth_issue_ipa_msg(cmd, cmd->hdr.return_code, card);
			netif_carrier_off(card->dev);
		}
		return NULL;
	case IPA_CMD_STARTLAN:
		dev_info(&card->gdev->dev,
			 "The link for %s on CHPID 0x%X has been restored\n",
			 netdev_name(card->dev), card->info.chpid);
		if (card->info.hwtrap)
			card->info.hwtrap = 2;
		qeth_schedule_recovery(card);
		return NULL;
	case IPA_CMD_SETBRIDGEPORT_IQD:
	case IPA_CMD_SETBRIDGEPORT_OSA:
	case IPA_CMD_ADDRESS_CHANGE_NOTIF:
		if (card->discipline->control_event_handler(card, cmd))
			return cmd;
		return NULL;
	case IPA_CMD_REGISTER_LOCAL_ADDR:
		if (cmd->hdr.prot_version == QETH_PROT_IPV4)
			qeth_add_local_addrs4(card, &cmd->data.local_addrs4);
		else if (cmd->hdr.prot_version == QETH_PROT_IPV6)
			qeth_add_local_addrs6(card, &cmd->data.local_addrs6);

		QETH_CARD_TEXT(card, 3, "irla");
		return NULL;
	case IPA_CMD_UNREGISTER_LOCAL_ADDR:
		if (cmd->hdr.prot_version == QETH_PROT_IPV4)
			qeth_del_local_addrs4(card, &cmd->data.local_addrs4);
		else if (cmd->hdr.prot_version == QETH_PROT_IPV6)
			qeth_del_local_addrs6(card, &cmd->data.local_addrs6);

		QETH_CARD_TEXT(card, 3, "urla");
		return NULL;
	default:
		QETH_DBF_MESSAGE(2, "Received data is IPA but not a reply!\n");
		return cmd;
	}
}

static void qeth_clear_ipacmd_list(struct qeth_card *card)
{
	struct qeth_cmd_buffer *iob;
>>>>>>> upstream/android-13
	unsigned long flags;

	QETH_CARD_TEXT(card, 4, "clipalst");

	spin_lock_irqsave(&card->lock, flags);
<<<<<<< HEAD
	list_for_each_entry_safe(reply, r, &card->cmd_waiter_list, list) {
		qeth_get_reply(reply);
		reply->rc = -EIO;
		atomic_inc(&reply->received);
		list_del_init(&reply->list);
		wake_up(&reply->wait_q);
		qeth_put_reply(reply);
	}
	spin_unlock_irqrestore(&card->lock, flags);
}
EXPORT_SYMBOL_GPL(qeth_clear_ipacmd_list);
=======
	list_for_each_entry(iob, &card->cmd_waiter_list, list_entry)
		qeth_notify_cmd(iob, -ECANCELED);
	spin_unlock_irqrestore(&card->lock, flags);
}
>>>>>>> upstream/android-13

static int qeth_check_idx_response(struct qeth_card *card,
	unsigned char *buffer)
{
<<<<<<< HEAD
	if (!buffer)
		return 0;

	QETH_DBF_HEX(CTRL, 2, buffer, QETH_DBF_CTRL_LEN);
	if ((buffer[2] & 0xc0) == 0xc0) {
		QETH_DBF_MESSAGE(2, "received an IDX TERMINATE with cause code %#02x\n",
				 buffer[4]);
		QETH_CARD_TEXT(card, 2, "ckidxres");
		QETH_CARD_TEXT(card, 2, " idxterm");
		QETH_CARD_TEXT_(card, 2, "  rc%d", -EIO);
		if (buffer[4] == 0xf6) {
			dev_err(&card->gdev->dev,
			"The qeth device is not configured "
			"for the OSI layer required by z/VM\n");
			return -EPERM;
=======
	QETH_DBF_HEX(CTRL, 2, buffer, QETH_DBF_CTRL_LEN);
	if ((buffer[2] & QETH_IDX_TERMINATE_MASK) == QETH_IDX_TERMINATE) {
		QETH_DBF_MESSAGE(2, "received an IDX TERMINATE with cause code %#04x\n",
				 buffer[4]);
		QETH_CARD_TEXT(card, 2, "ckidxres");
		QETH_CARD_TEXT(card, 2, " idxterm");
		QETH_CARD_TEXT_(card, 2, "rc%x", buffer[4]);
		if (buffer[4] == QETH_IDX_TERM_BAD_TRANSPORT ||
		    buffer[4] == QETH_IDX_TERM_BAD_TRANSPORT_VM) {
			dev_err(&card->gdev->dev,
				"The device does not support the configured transport mode\n");
			return -EPROTONOSUPPORT;
>>>>>>> upstream/android-13
		}
		return -EIO;
	}
	return 0;
}

<<<<<<< HEAD
static struct qeth_card *CARD_FROM_CDEV(struct ccw_device *cdev)
{
	struct qeth_card *card = dev_get_drvdata(&((struct ccwgroup_device *)
		dev_get_drvdata(&cdev->dev))->dev);
	return card;
}

static struct qeth_cmd_buffer *__qeth_get_buffer(struct qeth_channel *channel)
{
	__u8 index;

	QETH_CARD_TEXT(CARD_FROM_CDEV(channel->ccwdev), 6, "getbuff");
	index = channel->io_buf_no;
	do {
		if (channel->iob[index].state == BUF_STATE_FREE) {
			channel->iob[index].state = BUF_STATE_LOCKED;
			channel->io_buf_no = (channel->io_buf_no + 1) %
				QETH_CMD_BUFFER_NO;
			memset(channel->iob[index].data, 0, QETH_BUFSIZE);
			return channel->iob + index;
		}
		index = (index + 1) % QETH_CMD_BUFFER_NO;
	} while (index != channel->io_buf_no);

	return NULL;
}

void qeth_release_buffer(struct qeth_channel *channel,
		struct qeth_cmd_buffer *iob)
{
	unsigned long flags;

	QETH_CARD_TEXT(CARD_FROM_CDEV(channel->ccwdev), 6, "relbuff");
	spin_lock_irqsave(&channel->iob_lock, flags);
	memset(iob->data, 0, QETH_BUFSIZE);
	iob->state = BUF_STATE_FREE;
	iob->callback = qeth_send_control_data_cb;
	iob->rc = 0;
	spin_unlock_irqrestore(&channel->iob_lock, flags);
	wake_up(&channel->wait_q);
}
EXPORT_SYMBOL_GPL(qeth_release_buffer);

static struct qeth_cmd_buffer *qeth_get_buffer(struct qeth_channel *channel)
{
	struct qeth_cmd_buffer *buffer = NULL;
	unsigned long flags;

	spin_lock_irqsave(&channel->iob_lock, flags);
	buffer = __qeth_get_buffer(channel);
	spin_unlock_irqrestore(&channel->iob_lock, flags);
	return buffer;
}

struct qeth_cmd_buffer *qeth_wait_for_buffer(struct qeth_channel *channel)
{
	struct qeth_cmd_buffer *buffer;
	wait_event(channel->wait_q,
		   ((buffer = qeth_get_buffer(channel)) != NULL));
	return buffer;
}
EXPORT_SYMBOL_GPL(qeth_wait_for_buffer);

void qeth_clear_cmd_buffers(struct qeth_channel *channel)
{
	int cnt;

	for (cnt = 0; cnt < QETH_CMD_BUFFER_NO; cnt++)
		qeth_release_buffer(channel, &channel->iob[cnt]);
	channel->io_buf_no = 0;
}
EXPORT_SYMBOL_GPL(qeth_clear_cmd_buffers);

static void qeth_send_control_data_cb(struct qeth_channel *channel,
		  struct qeth_cmd_buffer *iob)
{
	struct qeth_card *card;
	struct qeth_reply *reply, *r;
	struct qeth_ipa_cmd *cmd;
	unsigned long flags;
	int keep_reply;
	int rc = 0;

	card = CARD_FROM_CDEV(channel->ccwdev);
=======
static void qeth_release_buffer_cb(struct qeth_card *card,
				   struct qeth_cmd_buffer *iob,
				   unsigned int data_length)
{
	qeth_put_cmd(iob);
}

static void qeth_cancel_cmd(struct qeth_cmd_buffer *iob, int rc)
{
	qeth_notify_cmd(iob, rc);
	qeth_put_cmd(iob);
}

static struct qeth_cmd_buffer *qeth_alloc_cmd(struct qeth_channel *channel,
					      unsigned int length,
					      unsigned int ccws, long timeout)
{
	struct qeth_cmd_buffer *iob;

	if (length > QETH_BUFSIZE)
		return NULL;

	iob = kzalloc(sizeof(*iob), GFP_KERNEL);
	if (!iob)
		return NULL;

	iob->data = kzalloc(ALIGN(length, 8) + ccws * sizeof(struct ccw1),
			    GFP_KERNEL | GFP_DMA);
	if (!iob->data) {
		kfree(iob);
		return NULL;
	}

	init_completion(&iob->done);
	spin_lock_init(&iob->lock);
	refcount_set(&iob->ref_count, 1);
	iob->channel = channel;
	iob->timeout = timeout;
	iob->length = length;
	return iob;
}

static void qeth_issue_next_read_cb(struct qeth_card *card,
				    struct qeth_cmd_buffer *iob,
				    unsigned int data_length)
{
	struct qeth_cmd_buffer *request = NULL;
	struct qeth_ipa_cmd *cmd = NULL;
	struct qeth_reply *reply = NULL;
	struct qeth_cmd_buffer *tmp;
	unsigned long flags;
	int rc = 0;

>>>>>>> upstream/android-13
	QETH_CARD_TEXT(card, 4, "sndctlcb");
	rc = qeth_check_idx_response(card, iob->data);
	switch (rc) {
	case 0:
		break;
	case -EIO:
<<<<<<< HEAD
		qeth_clear_ipacmd_list(card);
		qeth_schedule_recovery(card);
		/* fall through */
	default:
		goto out;
	}

	cmd = qeth_check_ipa_data(card, iob);
	if ((cmd == NULL) && (card->state != CARD_STATE_DOWN))
		goto out;
	/*in case of OSN : check if cmd is set */
	if (card->info.type == QETH_CARD_TYPE_OSN &&
	    cmd &&
	    cmd->hdr.command != IPA_CMD_STARTLAN &&
	    card->osn_info.assist_cb != NULL) {
		card->osn_info.assist_cb(card->dev, cmd);
		goto out;
	}

	spin_lock_irqsave(&card->lock, flags);
	list_for_each_entry_safe(reply, r, &card->cmd_waiter_list, list) {
		if ((reply->seqno == QETH_IDX_COMMAND_SEQNO) ||
		    ((cmd) && (reply->seqno == cmd->hdr.seqno))) {
			qeth_get_reply(reply);
			list_del_init(&reply->list);
			spin_unlock_irqrestore(&card->lock, flags);
			keep_reply = 0;
			if (reply->callback != NULL) {
				if (cmd) {
					reply->offset = (__u16)((char *)cmd -
							(char *)iob->data);
					keep_reply = reply->callback(card,
							reply,
							(unsigned long)cmd);
				} else
					keep_reply = reply->callback(card,
							reply,
							(unsigned long)iob);
			}
			if (cmd)
				reply->rc = (u16) cmd->hdr.return_code;
			else if (iob->rc)
				reply->rc = iob->rc;
			if (keep_reply) {
				spin_lock_irqsave(&card->lock, flags);
				list_add_tail(&reply->list,
					      &card->cmd_waiter_list);
				spin_unlock_irqrestore(&card->lock, flags);
			} else {
				atomic_inc(&reply->received);
				wake_up(&reply->wait_q);
			}
			qeth_put_reply(reply);
			goto out;
		}
	}
	spin_unlock_irqrestore(&card->lock, flags);
=======
		qeth_schedule_recovery(card);
		fallthrough;
	default:
		qeth_clear_ipacmd_list(card);
		goto err_idx;
	}

	cmd = __ipa_reply(iob);
	if (cmd) {
		cmd = qeth_check_ipa_data(card, cmd);
		if (!cmd)
			goto out;
	}

	/* match against pending cmd requests */
	spin_lock_irqsave(&card->lock, flags);
	list_for_each_entry(tmp, &card->cmd_waiter_list, list_entry) {
		if (tmp->match && tmp->match(tmp, iob)) {
			request = tmp;
			/* take the object outside the lock */
			qeth_get_cmd(request);
			break;
		}
	}
	spin_unlock_irqrestore(&card->lock, flags);

	if (!request)
		goto out;

	reply = &request->reply;
	if (!reply->callback) {
		rc = 0;
		goto no_callback;
	}

	spin_lock_irqsave(&request->lock, flags);
	if (request->rc)
		/* Bail out when the requestor has already left: */
		rc = request->rc;
	else
		rc = reply->callback(card, reply, cmd ? (unsigned long)cmd :
							(unsigned long)iob);
	spin_unlock_irqrestore(&request->lock, flags);

no_callback:
	if (rc <= 0)
		qeth_notify_cmd(request, rc);
	qeth_put_cmd(request);
>>>>>>> upstream/android-13
out:
	memcpy(&card->seqno.pdu_hdr_ack,
		QETH_PDU_HEADER_SEQ_NO(iob->data),
		QETH_SEQ_NO_LENGTH);
<<<<<<< HEAD
	qeth_release_buffer(channel, iob);
=======
	__qeth_issue_next_read(card);
err_idx:
	qeth_put_cmd(iob);
>>>>>>> upstream/android-13
}

static int qeth_set_thread_start_bit(struct qeth_card *card,
		unsigned long thread)
{
	unsigned long flags;
<<<<<<< HEAD

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	if (!(card->thread_allowed_mask & thread) ||
	      (card->thread_start_mask & thread)) {
		spin_unlock_irqrestore(&card->thread_mask_lock, flags);
		return -EPERM;
	}
	card->thread_start_mask |= thread;
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	return 0;
}

void qeth_clear_thread_start_bit(struct qeth_card *card, unsigned long thread)
=======
	int rc = 0;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	if (!(card->thread_allowed_mask & thread))
		rc = -EPERM;
	else if (card->thread_start_mask & thread)
		rc = -EBUSY;
	else
		card->thread_start_mask |= thread;
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);

	return rc;
}

static void qeth_clear_thread_start_bit(struct qeth_card *card,
					unsigned long thread)
>>>>>>> upstream/android-13
{
	unsigned long flags;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	card->thread_start_mask &= ~thread;
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	wake_up(&card->wait_q);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_clear_thread_start_bit);

void qeth_clear_thread_running_bit(struct qeth_card *card, unsigned long thread)
=======

static void qeth_clear_thread_running_bit(struct qeth_card *card,
					  unsigned long thread)
>>>>>>> upstream/android-13
{
	unsigned long flags;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	card->thread_running_mask &= ~thread;
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	wake_up_all(&card->wait_q);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_clear_thread_running_bit);
=======
>>>>>>> upstream/android-13

static int __qeth_do_run_thread(struct qeth_card *card, unsigned long thread)
{
	unsigned long flags;
	int rc = 0;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	if (card->thread_start_mask & thread) {
		if ((card->thread_allowed_mask & thread) &&
		    !(card->thread_running_mask & thread)) {
			rc = 1;
			card->thread_start_mask &= ~thread;
			card->thread_running_mask |= thread;
		} else
			rc = -EPERM;
	}
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	return rc;
}

<<<<<<< HEAD
int qeth_do_run_thread(struct qeth_card *card, unsigned long thread)
=======
static int qeth_do_run_thread(struct qeth_card *card, unsigned long thread)
>>>>>>> upstream/android-13
{
	int rc = 0;

	wait_event(card->wait_q,
		   (rc = __qeth_do_run_thread(card, thread)) >= 0);
	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_do_run_thread);

void qeth_schedule_recovery(struct qeth_card *card)
{
	QETH_CARD_TEXT(card, 2, "startrec");
	if (qeth_set_thread_start_bit(card, QETH_RECOVER_THREAD) == 0)
		schedule_work(&card->kernel_thread_starter);
}
EXPORT_SYMBOL_GPL(qeth_schedule_recovery);

static int qeth_get_problem(struct ccw_device *cdev, struct irb *irb)
{
	int dstat, cstat;
	char *sense;
	struct qeth_card *card;
=======

int qeth_schedule_recovery(struct qeth_card *card)
{
	int rc;

	QETH_CARD_TEXT(card, 2, "startrec");

	rc = qeth_set_thread_start_bit(card, QETH_RECOVER_THREAD);
	if (!rc)
		schedule_work(&card->kernel_thread_starter);

	return rc;
}

static int qeth_get_problem(struct qeth_card *card, struct ccw_device *cdev,
			    struct irb *irb)
{
	int dstat, cstat;
	char *sense;
>>>>>>> upstream/android-13

	sense = (char *) irb->ecw;
	cstat = irb->scsw.cmd.cstat;
	dstat = irb->scsw.cmd.dstat;
<<<<<<< HEAD
	card = CARD_FROM_CDEV(cdev);
=======
>>>>>>> upstream/android-13

	if (cstat & (SCHN_STAT_CHN_CTRL_CHK | SCHN_STAT_INTF_CTRL_CHK |
		     SCHN_STAT_CHN_DATA_CHK | SCHN_STAT_CHAIN_CHECK |
		     SCHN_STAT_PROT_CHECK | SCHN_STAT_PROG_CHECK)) {
		QETH_CARD_TEXT(card, 2, "CGENCHK");
		dev_warn(&cdev->dev, "The qeth device driver "
			"failed to recover an error on the device\n");
<<<<<<< HEAD
		QETH_DBF_MESSAGE(2, "%s check on device dstat=x%x, cstat=x%x\n",
			dev_name(&cdev->dev), dstat, cstat);
		print_hex_dump(KERN_WARNING, "qeth: irb ", DUMP_PREFIX_OFFSET,
				16, 1, irb, 64, 1);
		return 1;
=======
		QETH_DBF_MESSAGE(2, "check on channel %x with dstat=%#x, cstat=%#x\n",
				 CCW_DEVID(cdev), dstat, cstat);
		print_hex_dump(KERN_WARNING, "qeth: irb ", DUMP_PREFIX_OFFSET,
				16, 1, irb, 64, 1);
		return -EIO;
>>>>>>> upstream/android-13
	}

	if (dstat & DEV_STAT_UNIT_CHECK) {
		if (sense[SENSE_RESETTING_EVENT_BYTE] &
		    SENSE_RESETTING_EVENT_FLAG) {
			QETH_CARD_TEXT(card, 2, "REVIND");
<<<<<<< HEAD
			return 1;
=======
			return -EIO;
>>>>>>> upstream/android-13
		}
		if (sense[SENSE_COMMAND_REJECT_BYTE] &
		    SENSE_COMMAND_REJECT_FLAG) {
			QETH_CARD_TEXT(card, 2, "CMDREJi");
<<<<<<< HEAD
			return 1;
		}
		if ((sense[2] == 0xaf) && (sense[3] == 0xfe)) {
			QETH_CARD_TEXT(card, 2, "AFFE");
			return 1;
=======
			return -EIO;
		}
		if ((sense[2] == 0xaf) && (sense[3] == 0xfe)) {
			QETH_CARD_TEXT(card, 2, "AFFE");
			return -EIO;
>>>>>>> upstream/android-13
		}
		if ((!sense[0]) && (!sense[1]) && (!sense[2]) && (!sense[3])) {
			QETH_CARD_TEXT(card, 2, "ZEROSEN");
			return 0;
		}
		QETH_CARD_TEXT(card, 2, "DGENCHK");
<<<<<<< HEAD
			return 1;
=======
		return -EIO;
>>>>>>> upstream/android-13
	}
	return 0;
}

<<<<<<< HEAD
static long __qeth_check_irb_error(struct ccw_device *cdev,
		unsigned long intparm, struct irb *irb)
{
	struct qeth_card *card;

	card = CARD_FROM_CDEV(cdev);

	if (!card || !IS_ERR(irb))
=======
static int qeth_check_irb_error(struct qeth_card *card, struct ccw_device *cdev,
				struct irb *irb)
{
	if (!IS_ERR(irb))
>>>>>>> upstream/android-13
		return 0;

	switch (PTR_ERR(irb)) {
	case -EIO:
<<<<<<< HEAD
		QETH_DBF_MESSAGE(2, "%s i/o-error on device\n",
			dev_name(&cdev->dev));
		QETH_CARD_TEXT(card, 2, "ckirberr");
		QETH_CARD_TEXT_(card, 2, "  rc%d", -EIO);
		break;
=======
		QETH_DBF_MESSAGE(2, "i/o-error on channel %x\n",
				 CCW_DEVID(cdev));
		QETH_CARD_TEXT(card, 2, "ckirberr");
		QETH_CARD_TEXT_(card, 2, "  rc%d", -EIO);
		return -EIO;
>>>>>>> upstream/android-13
	case -ETIMEDOUT:
		dev_warn(&cdev->dev, "A hardware operation timed out"
			" on the device\n");
		QETH_CARD_TEXT(card, 2, "ckirberr");
		QETH_CARD_TEXT_(card, 2, "  rc%d", -ETIMEDOUT);
<<<<<<< HEAD
		if (intparm == QETH_RCD_PARM) {
			if (card->data.ccwdev == cdev) {
				card->data.state = CH_STATE_DOWN;
				wake_up(&card->wait_q);
			}
		}
		break;
	default:
		QETH_DBF_MESSAGE(2, "%s unknown error %ld on device\n",
			dev_name(&cdev->dev), PTR_ERR(irb));
		QETH_CARD_TEXT(card, 2, "ckirberr");
		QETH_CARD_TEXT(card, 2, "  rc???");
	}
	return PTR_ERR(irb);
=======
		return -ETIMEDOUT;
	default:
		QETH_DBF_MESSAGE(2, "unknown error %ld on channel %x\n",
				 PTR_ERR(irb), CCW_DEVID(cdev));
		QETH_CARD_TEXT(card, 2, "ckirberr");
		QETH_CARD_TEXT(card, 2, "  rc???");
		return PTR_ERR(irb);
	}
>>>>>>> upstream/android-13
}

static void qeth_irq(struct ccw_device *cdev, unsigned long intparm,
		struct irb *irb)
{
	int rc;
	int cstat, dstat;
	struct qeth_cmd_buffer *iob = NULL;
<<<<<<< HEAD
	struct qeth_channel *channel;
	struct qeth_card *card;

	card = CARD_FROM_CDEV(cdev);
	if (!card)
		return;
=======
	struct ccwgroup_device *gdev;
	struct qeth_channel *channel;
	struct qeth_card *card;

	/* while we hold the ccwdev lock, this stays valid: */
	gdev = dev_get_drvdata(&cdev->dev);
	card = dev_get_drvdata(&gdev->dev);
>>>>>>> upstream/android-13

	QETH_CARD_TEXT(card, 5, "irq");

	if (card->read.ccwdev == cdev) {
		channel = &card->read;
		QETH_CARD_TEXT(card, 5, "read");
	} else if (card->write.ccwdev == cdev) {
		channel = &card->write;
		QETH_CARD_TEXT(card, 5, "write");
	} else {
		channel = &card->data;
		QETH_CARD_TEXT(card, 5, "data");
	}

<<<<<<< HEAD
	if (qeth_intparm_is_iob(intparm))
		iob = (struct qeth_cmd_buffer *) __va((addr_t)intparm);

	if (__qeth_check_irb_error(cdev, intparm, irb)) {
		/* IO was terminated, free its resources. */
		if (iob)
			qeth_release_buffer(iob->channel, iob);
		atomic_set(&channel->irq_pending, 0);
		wake_up(&card->wait_q);
		return;
	}

	atomic_set(&channel->irq_pending, 0);

	if (irb->scsw.cmd.fctl & (SCSW_FCTL_CLEAR_FUNC))
		channel->state = CH_STATE_STOPPED;

	if (irb->scsw.cmd.fctl & (SCSW_FCTL_HALT_FUNC))
		channel->state = CH_STATE_HALTED;

	/*let's wake up immediately on data channel*/
	if ((channel == &card->data) && (intparm != 0) &&
	    (intparm != QETH_RCD_PARM))
		goto out;

	if (intparm == QETH_CLEAR_CHANNEL_PARM) {
		QETH_CARD_TEXT(card, 6, "clrchpar");
		/* we don't have to handle this further */
		intparm = 0;
	}
	if (intparm == QETH_HALT_CHANNEL_PARM) {
		QETH_CARD_TEXT(card, 6, "hltchpar");
		/* we don't have to handle this further */
		intparm = 0;
=======
	if (intparm == 0) {
		QETH_CARD_TEXT(card, 5, "irqunsol");
	} else if ((addr_t)intparm != (addr_t)channel->active_cmd) {
		QETH_CARD_TEXT(card, 5, "irqunexp");

		dev_err(&cdev->dev,
			"Received IRQ with intparm %lx, expected %px\n",
			intparm, channel->active_cmd);
		if (channel->active_cmd)
			qeth_cancel_cmd(channel->active_cmd, -EIO);
	} else {
		iob = (struct qeth_cmd_buffer *) (addr_t)intparm;
	}

	qeth_unlock_channel(card, channel);

	rc = qeth_check_irb_error(card, cdev, irb);
	if (rc) {
		/* IO was terminated, free its resources. */
		if (iob)
			qeth_cancel_cmd(iob, rc);
		return;
	}

	if (irb->scsw.cmd.fctl & SCSW_FCTL_CLEAR_FUNC) {
		channel->state = CH_STATE_STOPPED;
		wake_up(&card->wait_q);
	}

	if (irb->scsw.cmd.fctl & SCSW_FCTL_HALT_FUNC) {
		channel->state = CH_STATE_HALTED;
		wake_up(&card->wait_q);
	}

	if (iob && (irb->scsw.cmd.fctl & (SCSW_FCTL_CLEAR_FUNC |
					  SCSW_FCTL_HALT_FUNC))) {
		qeth_cancel_cmd(iob, -ECANCELED);
		iob = NULL;
>>>>>>> upstream/android-13
	}

	cstat = irb->scsw.cmd.cstat;
	dstat = irb->scsw.cmd.dstat;

	if ((dstat & DEV_STAT_UNIT_EXCEP) ||
	    (dstat & DEV_STAT_UNIT_CHECK) ||
	    (cstat)) {
		if (irb->esw.esw0.erw.cons) {
			dev_warn(&channel->ccwdev->dev,
				"The qeth device driver failed to recover "
				"an error on the device\n");
<<<<<<< HEAD
			QETH_DBF_MESSAGE(2, "%s sense data available. cstat "
				"0x%X dstat 0x%X\n",
				dev_name(&channel->ccwdev->dev), cstat, dstat);
=======
			QETH_DBF_MESSAGE(2, "sense data available on channel %x: cstat %#X dstat %#X\n",
					 CCW_DEVID(channel->ccwdev), cstat,
					 dstat);
>>>>>>> upstream/android-13
			print_hex_dump(KERN_WARNING, "qeth: irb ",
				DUMP_PREFIX_OFFSET, 16, 1, irb, 32, 1);
			print_hex_dump(KERN_WARNING, "qeth: sense data ",
				DUMP_PREFIX_OFFSET, 16, 1, irb->ecw, 32, 1);
		}
<<<<<<< HEAD
		if (intparm == QETH_RCD_PARM) {
			channel->state = CH_STATE_DOWN;
			goto out;
		}
		rc = qeth_get_problem(cdev, irb);
		if (rc) {
			card->read_or_write_problem = 1;
			if (iob)
				qeth_release_buffer(iob->channel, iob);
			qeth_clear_ipacmd_list(card);
			qeth_schedule_recovery(card);
			goto out;
		}
	}

	if (intparm == QETH_RCD_PARM) {
		channel->state = CH_STATE_RCD_DONE;
		goto out;
	}
	if (channel == &card->data)
		return;
	if (channel == &card->read &&
	    channel->state == CH_STATE_UP)
		__qeth_issue_next_read(card);

	if (iob && iob->callback)
		iob->callback(iob->channel, iob);

out:
	wake_up(&card->wait_q);
	return;
=======

		rc = qeth_get_problem(card, cdev, irb);
		if (rc) {
			card->read_or_write_problem = 1;
			if (iob)
				qeth_cancel_cmd(iob, rc);
			qeth_clear_ipacmd_list(card);
			qeth_schedule_recovery(card);
			return;
		}
	}

	if (iob) {
		/* sanity check: */
		if (irb->scsw.cmd.count > iob->length) {
			qeth_cancel_cmd(iob, -EIO);
			return;
		}
		if (iob->callback)
			iob->callback(card, iob,
				      iob->length - irb->scsw.cmd.count);
	}
>>>>>>> upstream/android-13
}

static void qeth_notify_skbs(struct qeth_qdio_out_q *q,
		struct qeth_qdio_out_buffer *buf,
		enum iucv_tx_notify notification)
{
	struct sk_buff *skb;

<<<<<<< HEAD
	if (skb_queue_empty(&buf->skb_list))
		goto out;
	skb = skb_peek(&buf->skb_list);
	while (skb) {
		QETH_CARD_TEXT_(q->card, 5, "skbn%d", notification);
		QETH_CARD_TEXT_(q->card, 5, "%lx", (long) skb);
		if (be16_to_cpu(skb->protocol) == ETH_P_AF_IUCV) {
			if (skb->sk) {
				struct iucv_sock *iucv = iucv_sk(skb->sk);
				iucv->sk_txnotify(skb, notification);
			}
		}
		if (skb_queue_is_last(&buf->skb_list, skb))
			skb = NULL;
		else
			skb = skb_queue_next(&buf->skb_list, skb);
	}
out:
	return;
}

static void qeth_release_skbs(struct qeth_qdio_out_buffer *buf)
{
	struct sk_buff *skb;
	struct iucv_sock *iucv;
	int notify_general_error = 0;

	if (atomic_read(&buf->state) == QETH_QDIO_BUF_PENDING)
		notify_general_error = 1;

	/* release may never happen from within CQ tasklet scope */
	WARN_ON_ONCE(atomic_read(&buf->state) == QETH_QDIO_BUF_IN_CQ);

	skb = skb_dequeue(&buf->skb_list);
	while (skb) {
		QETH_CARD_TEXT(buf->q->card, 5, "skbr");
		QETH_CARD_TEXT_(buf->q->card, 5, "%lx", (long) skb);
		if (notify_general_error &&
		    be16_to_cpu(skb->protocol) == ETH_P_AF_IUCV) {
			if (skb->sk) {
				iucv = iucv_sk(skb->sk);
				iucv->sk_txnotify(skb, TX_NOTIFY_GENERALERROR);
			}
		}
		refcount_dec(&skb->users);
		dev_kfree_skb_any(skb);
		skb = skb_dequeue(&buf->skb_list);
=======
	skb_queue_walk(&buf->skb_list, skb) {
		struct sock *sk = skb->sk;

		QETH_CARD_TEXT_(q->card, 5, "skbn%d", notification);
		QETH_CARD_TEXT_(q->card, 5, "%lx", (long) skb);
		if (sk && sk->sk_family == PF_IUCV)
			iucv_sk(sk)->sk_txnotify(sk, notification);
	}
}

static void qeth_tx_complete_buf(struct qeth_qdio_out_q *queue,
				 struct qeth_qdio_out_buffer *buf, bool error,
				 int budget)
{
	struct sk_buff *skb;

	/* Empty buffer? */
	if (buf->next_element_to_fill == 0)
		return;

	QETH_TXQ_STAT_INC(queue, bufs);
	QETH_TXQ_STAT_ADD(queue, buf_elements, buf->next_element_to_fill);
	if (error) {
		QETH_TXQ_STAT_ADD(queue, tx_errors, buf->frames);
	} else {
		QETH_TXQ_STAT_ADD(queue, tx_packets, buf->frames);
		QETH_TXQ_STAT_ADD(queue, tx_bytes, buf->bytes);
	}

	while ((skb = __skb_dequeue(&buf->skb_list)) != NULL) {
		unsigned int bytes = qdisc_pkt_len(skb);
		bool is_tso = skb_is_gso(skb);
		unsigned int packets;

		packets = is_tso ? skb_shinfo(skb)->gso_segs : 1;
		if (!error) {
			if (skb->ip_summed == CHECKSUM_PARTIAL)
				QETH_TXQ_STAT_ADD(queue, skbs_csum, packets);
			if (skb_is_nonlinear(skb))
				QETH_TXQ_STAT_INC(queue, skbs_sg);
			if (is_tso) {
				QETH_TXQ_STAT_INC(queue, skbs_tso);
				QETH_TXQ_STAT_ADD(queue, tso_bytes, bytes);
			}
		}

		napi_consume_skb(skb, budget);
>>>>>>> upstream/android-13
	}
}

static void qeth_clear_output_buffer(struct qeth_qdio_out_q *queue,
<<<<<<< HEAD
				     struct qeth_qdio_out_buffer *buf)
=======
				     struct qeth_qdio_out_buffer *buf,
				     bool error, int budget)
>>>>>>> upstream/android-13
{
	int i;

	/* is PCI flag set on buffer? */
<<<<<<< HEAD
	if (buf->buffer->element[0].sflags & SBAL_SFLAGS0_PCI_REQ)
		atomic_dec(&queue->set_pci_flags_count);

	qeth_release_skbs(buf);

	for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(queue->card); ++i) {
		if (buf->buffer->element[i].addr && buf->is_header[i])
			kmem_cache_free(qeth_core_header_cache,
				buf->buffer->element[i].addr);
		buf->is_header[i] = 0;
	}

	qeth_scrub_qdio_buffer(buf->buffer,
			       QETH_MAX_BUFFER_ELEMENTS(queue->card));
	buf->next_element_to_fill = 0;
	atomic_set(&buf->state, QETH_QDIO_BUF_EMPTY);
}

static void qeth_clear_outq_buffers(struct qeth_qdio_out_q *q, int free)
{
	int j;

	for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; ++j) {
		if (!q->bufs[j])
			continue;
		qeth_cleanup_handled_pending(q, j, 1);
		qeth_clear_output_buffer(q, q->bufs[j]);
		if (free) {
			kmem_cache_free(qeth_qdio_outbuf_cache, q->bufs[j]);
=======
	if (buf->buffer->element[0].sflags & SBAL_SFLAGS0_PCI_REQ) {
		atomic_dec(&queue->set_pci_flags_count);
		QETH_TXQ_STAT_INC(queue, completion_irq);
	}

	qeth_tx_complete_buf(queue, buf, error, budget);

	for (i = 0; i < queue->max_elements; ++i) {
		void *data = phys_to_virt(buf->buffer->element[i].addr);

		if (__test_and_clear_bit(i, buf->from_kmem_cache) && data)
			kmem_cache_free(qeth_core_header_cache, data);
	}

	qeth_scrub_qdio_buffer(buf->buffer, queue->max_elements);
	buf->next_element_to_fill = 0;
	buf->frames = 0;
	buf->bytes = 0;
	atomic_set(&buf->state, QETH_QDIO_BUF_EMPTY);
}

static void qeth_free_out_buf(struct qeth_qdio_out_buffer *buf)
{
	if (buf->aob)
		qdio_release_aob(buf->aob);
	kmem_cache_free(qeth_qdio_outbuf_cache, buf);
}

static void qeth_tx_complete_pending_bufs(struct qeth_card *card,
					  struct qeth_qdio_out_q *queue,
					  bool drain, int budget)
{
	struct qeth_qdio_out_buffer *buf, *tmp;

	list_for_each_entry_safe(buf, tmp, &queue->pending_bufs, list_entry) {
		struct qeth_qaob_priv1 *priv;
		struct qaob *aob = buf->aob;
		enum iucv_tx_notify notify;
		unsigned int i;

		priv = (struct qeth_qaob_priv1 *)&aob->user1;
		if (drain || READ_ONCE(priv->state) == QETH_QAOB_DONE) {
			QETH_CARD_TEXT(card, 5, "fp");
			QETH_CARD_TEXT_(card, 5, "%lx", (long) buf);

			notify = drain ? TX_NOTIFY_GENERALERROR :
					 qeth_compute_cq_notification(aob->aorc, 1);
			qeth_notify_skbs(queue, buf, notify);
			qeth_tx_complete_buf(queue, buf, drain, budget);

			for (i = 0;
			     i < aob->sb_count && i < queue->max_elements;
			     i++) {
				void *data = phys_to_virt(aob->sba[i]);

				if (test_bit(i, buf->from_kmem_cache) && data)
					kmem_cache_free(qeth_core_header_cache,
							data);
			}

			list_del(&buf->list_entry);
			qeth_free_out_buf(buf);
		}
	}
}

static void qeth_drain_output_queue(struct qeth_qdio_out_q *q, bool free)
{
	int j;

	qeth_tx_complete_pending_bufs(q->card, q, true, 0);

	for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; ++j) {
		if (!q->bufs[j])
			continue;

		qeth_clear_output_buffer(q, q->bufs[j], true, 0);
		if (free) {
			qeth_free_out_buf(q->bufs[j]);
>>>>>>> upstream/android-13
			q->bufs[j] = NULL;
		}
	}
}

<<<<<<< HEAD
void qeth_clear_qdio_buffers(struct qeth_card *card)
=======
static void qeth_drain_output_queues(struct qeth_card *card)
>>>>>>> upstream/android-13
{
	int i;

	QETH_CARD_TEXT(card, 2, "clearqdbf");
	/* clear outbound buffers to free skbs */
	for (i = 0; i < card->qdio.no_out_queues; ++i) {
<<<<<<< HEAD
		if (card->qdio.out_qs[i]) {
			qeth_clear_outq_buffers(card->qdio.out_qs[i], 0);
		}
	}
}
EXPORT_SYMBOL_GPL(qeth_clear_qdio_buffers);

static void qeth_free_buffer_pool(struct qeth_card *card)
{
	struct qeth_buffer_pool_entry *pool_entry, *tmp;
	int i = 0;
	list_for_each_entry_safe(pool_entry, tmp,
				 &card->qdio.init_pool.entry_list, init_list){
		for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i)
			free_page((unsigned long)pool_entry->elements[i]);
		list_del(&pool_entry->init_list);
		kfree(pool_entry);
	}
}

static void qeth_clean_channel(struct qeth_channel *channel)
{
	struct ccw_device *cdev = channel->ccwdev;
	int cnt;

	QETH_DBF_TEXT(SETUP, 2, "freech");

	spin_lock_irq(get_ccwdev_lock(cdev));
	cdev->handler = NULL;
	spin_unlock_irq(get_ccwdev_lock(cdev));

	for (cnt = 0; cnt < QETH_CMD_BUFFER_NO; cnt++)
		kfree(channel->iob[cnt].data);
	kfree(channel->ccw);
}

static int qeth_setup_channel(struct qeth_channel *channel, bool alloc_buffers)
{
	struct ccw_device *cdev = channel->ccwdev;
	int cnt;

	QETH_DBF_TEXT(SETUP, 2, "setupch");

	channel->ccw = kmalloc(sizeof(struct ccw1), GFP_KERNEL | GFP_DMA);
	if (!channel->ccw)
		return -ENOMEM;
	channel->state = CH_STATE_DOWN;
	atomic_set(&channel->irq_pending, 0);
	init_waitqueue_head(&channel->wait_q);

	spin_lock_irq(get_ccwdev_lock(cdev));
	cdev->handler = qeth_irq;
	spin_unlock_irq(get_ccwdev_lock(cdev));

	if (!alloc_buffers)
		return 0;

	for (cnt = 0; cnt < QETH_CMD_BUFFER_NO; cnt++) {
		channel->iob[cnt].data =
			kzalloc(QETH_BUFSIZE, GFP_DMA|GFP_KERNEL);
		if (channel->iob[cnt].data == NULL)
			break;
		channel->iob[cnt].state = BUF_STATE_FREE;
		channel->iob[cnt].channel = channel;
		channel->iob[cnt].callback = qeth_send_control_data_cb;
		channel->iob[cnt].rc = 0;
	}
	if (cnt < QETH_CMD_BUFFER_NO) {
		qeth_clean_channel(channel);
		return -ENOMEM;
	}
	channel->io_buf_no = 0;
	spin_lock_init(&channel->iob_lock);

	return 0;
}

static void qeth_set_single_write_queues(struct qeth_card *card)
{
	if ((atomic_read(&card->qdio.state) != QETH_QDIO_UNINITIALIZED) &&
	    (card->qdio.no_out_queues == 4))
		qeth_free_qdio_buffers(card);

	card->qdio.no_out_queues = 1;
	if (card->qdio.default_out_queue != 0)
		dev_info(&card->gdev->dev, "Priority Queueing not supported\n");

	card->qdio.default_out_queue = 0;
}

static void qeth_set_multiple_write_queues(struct qeth_card *card)
{
	if ((atomic_read(&card->qdio.state) != QETH_QDIO_UNINITIALIZED) &&
	    (card->qdio.no_out_queues == 1)) {
		qeth_free_qdio_buffers(card);
		card->qdio.default_out_queue = 2;
	}
	card->qdio.no_out_queues = 4;
=======
		if (card->qdio.out_qs[i])
			qeth_drain_output_queue(card->qdio.out_qs[i], false);
	}
}

static void qeth_osa_set_output_queues(struct qeth_card *card, bool single)
{
	unsigned int max = single ? 1 : card->dev->num_tx_queues;

	if (card->qdio.no_out_queues == max)
		return;

	if (atomic_read(&card->qdio.state) != QETH_QDIO_UNINITIALIZED)
		qeth_free_qdio_queues(card);

	if (max == 1 && card->qdio.do_prio_queueing != QETH_PRIOQ_DEFAULT)
		dev_info(&card->gdev->dev, "Priority Queueing not supported\n");

	card->qdio.no_out_queues = max;
>>>>>>> upstream/android-13
}

static int qeth_update_from_chp_desc(struct qeth_card *card)
{
	struct ccw_device *ccwdev;
	struct channel_path_desc_fmt0 *chp_dsc;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "chp_desc");
=======
	QETH_CARD_TEXT(card, 2, "chp_desc");
>>>>>>> upstream/android-13

	ccwdev = card->data.ccwdev;
	chp_dsc = ccw_device_get_chp_desc(ccwdev, 0);
	if (!chp_dsc)
		return -ENOMEM;

	card->info.func_level = 0x4100 + chp_dsc->desc;
<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_IQD)
		goto out;

	/* CHPP field bit 6 == 1 -> single queue */
	if ((chp_dsc->chpp & 0x02) == 0x02)
		qeth_set_single_write_queues(card);
	else
		qeth_set_multiple_write_queues(card);
out:
	kfree(chp_dsc);
	QETH_DBF_TEXT_(SETUP, 2, "nr:%x", card->qdio.no_out_queues);
	QETH_DBF_TEXT_(SETUP, 2, "lvl:%02x", card->info.func_level);
=======

	if (IS_OSD(card) || IS_OSX(card))
		/* CHPP field bit 6 == 1 -> single queue */
		qeth_osa_set_output_queues(card, chp_dsc->chpp & 0x02);

	kfree(chp_dsc);
	QETH_CARD_TEXT_(card, 2, "nr:%x", card->qdio.no_out_queues);
	QETH_CARD_TEXT_(card, 2, "lvl:%02x", card->info.func_level);
>>>>>>> upstream/android-13
	return 0;
}

static void qeth_init_qdio_info(struct qeth_card *card)
{
<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 4, "intqdinf");
	atomic_set(&card->qdio.state, QETH_QDIO_UNINITIALIZED);
	card->qdio.do_prio_queueing = QETH_PRIOQ_DEFAULT;
	card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	card->qdio.no_out_queues = QETH_MAX_QUEUES;
=======
	QETH_CARD_TEXT(card, 4, "intqdinf");
	atomic_set(&card->qdio.state, QETH_QDIO_UNINITIALIZED);
	card->qdio.do_prio_queueing = QETH_PRIOQ_DEFAULT;
	card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
>>>>>>> upstream/android-13

	/* inbound */
	card->qdio.no_in_queues = 1;
	card->qdio.in_buf_size = QETH_IN_BUF_SIZE_DEFAULT;
<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_IQD)
=======
	if (IS_IQD(card))
>>>>>>> upstream/android-13
		card->qdio.init_pool.buf_count = QETH_IN_BUF_COUNT_HSDEFAULT;
	else
		card->qdio.init_pool.buf_count = QETH_IN_BUF_COUNT_DEFAULT;
	card->qdio.in_buf_pool.buf_count = card->qdio.init_pool.buf_count;
	INIT_LIST_HEAD(&card->qdio.in_buf_pool.entry_list);
	INIT_LIST_HEAD(&card->qdio.init_pool.entry_list);
}

static void qeth_set_initial_options(struct qeth_card *card)
{
	card->options.route4.type = NO_ROUTER;
	card->options.route6.type = NO_ROUTER;
<<<<<<< HEAD
	card->options.rx_sg_cb = QETH_RX_SG_CB;
	card->options.isolation = ISOLATION_MODE_NONE;
	card->options.cq = QETH_CQ_DISABLED;
=======
	card->options.isolation = ISOLATION_MODE_NONE;
	card->options.cq = QETH_CQ_DISABLED;
	card->options.layer = QETH_DISCIPLINE_UNDETERMINED;
>>>>>>> upstream/android-13
}

static int qeth_do_start_thread(struct qeth_card *card, unsigned long thread)
{
	unsigned long flags;
	int rc = 0;

	spin_lock_irqsave(&card->thread_mask_lock, flags);
	QETH_CARD_TEXT_(card, 4, "  %02x%02x%02x",
			(u8) card->thread_start_mask,
			(u8) card->thread_allowed_mask,
			(u8) card->thread_running_mask);
	rc = (card->thread_start_mask & thread);
	spin_unlock_irqrestore(&card->thread_mask_lock, flags);
	return rc;
}

<<<<<<< HEAD
=======
static int qeth_do_reset(void *data);
>>>>>>> upstream/android-13
static void qeth_start_kernel_thread(struct work_struct *work)
{
	struct task_struct *ts;
	struct qeth_card *card = container_of(work, struct qeth_card,
					kernel_thread_starter);
<<<<<<< HEAD
	QETH_CARD_TEXT(card , 2, "strthrd");
=======
	QETH_CARD_TEXT(card, 2, "strthrd");
>>>>>>> upstream/android-13

	if (card->read.state != CH_STATE_UP &&
	    card->write.state != CH_STATE_UP)
		return;
	if (qeth_do_start_thread(card, QETH_RECOVER_THREAD)) {
<<<<<<< HEAD
		ts = kthread_run(card->discipline->recover, (void *)card,
				"qeth_recover");
=======
		ts = kthread_run(qeth_do_reset, card, "qeth_recover");
>>>>>>> upstream/android-13
		if (IS_ERR(ts)) {
			qeth_clear_thread_start_bit(card, QETH_RECOVER_THREAD);
			qeth_clear_thread_running_bit(card,
				QETH_RECOVER_THREAD);
		}
	}
}

static void qeth_buffer_reclaim_work(struct work_struct *);
static void qeth_setup_card(struct qeth_card *card)
{
<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "setupcrd");
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));

	card->info.type = CARD_RDEV(card)->id.driver_info;
	card->state = CARD_STATE_DOWN;
	spin_lock_init(&card->mclock);
	spin_lock_init(&card->lock);
	spin_lock_init(&card->ip_lock);
	spin_lock_init(&card->thread_mask_lock);
	mutex_init(&card->conf_mutex);
	mutex_init(&card->discipline_mutex);
	mutex_init(&card->vid_list_mutex);
=======
	QETH_CARD_TEXT(card, 2, "setupcrd");

	card->info.type = CARD_RDEV(card)->id.driver_info;
	card->state = CARD_STATE_DOWN;
	spin_lock_init(&card->lock);
	spin_lock_init(&card->thread_mask_lock);
	mutex_init(&card->conf_mutex);
	mutex_init(&card->discipline_mutex);
>>>>>>> upstream/android-13
	INIT_WORK(&card->kernel_thread_starter, qeth_start_kernel_thread);
	INIT_LIST_HEAD(&card->cmd_waiter_list);
	init_waitqueue_head(&card->wait_q);
	qeth_set_initial_options(card);
	/* IP address takeover */
	INIT_LIST_HEAD(&card->ipato.entries);
	qeth_init_qdio_info(card);
	INIT_DELAYED_WORK(&card->buffer_reclaim_work, qeth_buffer_reclaim_work);
<<<<<<< HEAD
	INIT_WORK(&card->close_dev_work, qeth_close_dev_handler);
=======
	hash_init(card->rx_mode_addrs);
	hash_init(card->local_addrs4);
	hash_init(card->local_addrs6);
	spin_lock_init(&card->local_addrs4_lock);
	spin_lock_init(&card->local_addrs6_lock);
>>>>>>> upstream/android-13
}

static void qeth_core_sl_print(struct seq_file *m, struct service_level *slr)
{
	struct qeth_card *card = container_of(slr, struct qeth_card,
					qeth_service_level);
	if (card->info.mcl_level[0])
		seq_printf(m, "qeth: %s firmware level %s\n",
			CARD_BUS_ID(card), card->info.mcl_level);
}

static struct qeth_card *qeth_alloc_card(struct ccwgroup_device *gdev)
{
	struct qeth_card *card;

	QETH_DBF_TEXT(SETUP, 2, "alloccrd");
	card = kzalloc(sizeof(*card), GFP_KERNEL);
	if (!card)
		goto out;
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));

	card->gdev = gdev;
<<<<<<< HEAD
	CARD_RDEV(card) = gdev->cdev[0];
	CARD_WDEV(card) = gdev->cdev[1];
	CARD_DDEV(card) = gdev->cdev[2];
	if (qeth_setup_channel(&card->read, true))
		goto out_ip;
	if (qeth_setup_channel(&card->write, true))
		goto out_channel;
	if (qeth_setup_channel(&card->data, false))
		goto out_data;
	card->options.layer2 = -1;
=======
	dev_set_drvdata(&gdev->dev, card);
	CARD_RDEV(card) = gdev->cdev[0];
	CARD_WDEV(card) = gdev->cdev[1];
	CARD_DDEV(card) = gdev->cdev[2];

	card->event_wq = alloc_ordered_workqueue("%s_event", 0,
						 dev_name(&gdev->dev));
	if (!card->event_wq)
		goto out_wq;

	card->read_cmd = qeth_alloc_cmd(&card->read, QETH_BUFSIZE, 1, 0);
	if (!card->read_cmd)
		goto out_read_cmd;

	card->debugfs = debugfs_create_dir(dev_name(&gdev->dev),
					   qeth_debugfs_root);
	debugfs_create_file("local_addrs", 0400, card->debugfs, card,
			    &qeth_debugfs_local_addr_fops);

>>>>>>> upstream/android-13
	card->qeth_service_level.seq_print = qeth_core_sl_print;
	register_service_level(&card->qeth_service_level);
	return card;

<<<<<<< HEAD
out_data:
	qeth_clean_channel(&card->write);
out_channel:
	qeth_clean_channel(&card->read);
out_ip:
=======
out_read_cmd:
	destroy_workqueue(card->event_wq);
out_wq:
	dev_set_drvdata(&gdev->dev, NULL);
>>>>>>> upstream/android-13
	kfree(card);
out:
	return NULL;
}

<<<<<<< HEAD
static int qeth_clear_channel(struct qeth_channel *channel)
{
	unsigned long flags;
	struct qeth_card *card;
	int rc;

	card = CARD_FROM_CDEV(channel->ccwdev);
	QETH_CARD_TEXT(card, 3, "clearch");
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	rc = ccw_device_clear(channel->ccwdev, QETH_CLEAR_CHANNEL_PARM);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);
=======
static int qeth_clear_channel(struct qeth_card *card,
			      struct qeth_channel *channel)
{
	int rc;

	QETH_CARD_TEXT(card, 3, "clearch");
	spin_lock_irq(get_ccwdev_lock(channel->ccwdev));
	rc = ccw_device_clear(channel->ccwdev, (addr_t)channel->active_cmd);
	spin_unlock_irq(get_ccwdev_lock(channel->ccwdev));
>>>>>>> upstream/android-13

	if (rc)
		return rc;
	rc = wait_event_interruptible_timeout(card->wait_q,
			channel->state == CH_STATE_STOPPED, QETH_TIMEOUT);
	if (rc == -ERESTARTSYS)
		return rc;
	if (channel->state != CH_STATE_STOPPED)
		return -ETIME;
	channel->state = CH_STATE_DOWN;
	return 0;
}

<<<<<<< HEAD
static int qeth_halt_channel(struct qeth_channel *channel)
{
	unsigned long flags;
	struct qeth_card *card;
	int rc;

	card = CARD_FROM_CDEV(channel->ccwdev);
	QETH_CARD_TEXT(card, 3, "haltch");
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	rc = ccw_device_halt(channel->ccwdev, QETH_HALT_CHANNEL_PARM);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);
=======
static int qeth_halt_channel(struct qeth_card *card,
			     struct qeth_channel *channel)
{
	int rc;

	QETH_CARD_TEXT(card, 3, "haltch");
	spin_lock_irq(get_ccwdev_lock(channel->ccwdev));
	rc = ccw_device_halt(channel->ccwdev, (addr_t)channel->active_cmd);
	spin_unlock_irq(get_ccwdev_lock(channel->ccwdev));
>>>>>>> upstream/android-13

	if (rc)
		return rc;
	rc = wait_event_interruptible_timeout(card->wait_q,
			channel->state == CH_STATE_HALTED, QETH_TIMEOUT);
	if (rc == -ERESTARTSYS)
		return rc;
	if (channel->state != CH_STATE_HALTED)
		return -ETIME;
	return 0;
}

<<<<<<< HEAD
=======
static int qeth_stop_channel(struct qeth_channel *channel)
{
	struct ccw_device *cdev = channel->ccwdev;
	int rc;

	rc = ccw_device_set_offline(cdev);

	spin_lock_irq(get_ccwdev_lock(cdev));
	if (channel->active_cmd)
		dev_err(&cdev->dev, "Stopped channel while cmd %px was still active\n",
			channel->active_cmd);

	cdev->handler = NULL;
	spin_unlock_irq(get_ccwdev_lock(cdev));

	return rc;
}

static int qeth_start_channel(struct qeth_channel *channel)
{
	struct ccw_device *cdev = channel->ccwdev;
	int rc;

	channel->state = CH_STATE_DOWN;
	xchg(&channel->active_cmd, NULL);

	spin_lock_irq(get_ccwdev_lock(cdev));
	cdev->handler = qeth_irq;
	spin_unlock_irq(get_ccwdev_lock(cdev));

	rc = ccw_device_set_online(cdev);
	if (rc)
		goto err;

	return 0;

err:
	spin_lock_irq(get_ccwdev_lock(cdev));
	cdev->handler = NULL;
	spin_unlock_irq(get_ccwdev_lock(cdev));
	return rc;
}

>>>>>>> upstream/android-13
static int qeth_halt_channels(struct qeth_card *card)
{
	int rc1 = 0, rc2 = 0, rc3 = 0;

	QETH_CARD_TEXT(card, 3, "haltchs");
<<<<<<< HEAD
	rc1 = qeth_halt_channel(&card->read);
	rc2 = qeth_halt_channel(&card->write);
	rc3 = qeth_halt_channel(&card->data);
=======
	rc1 = qeth_halt_channel(card, &card->read);
	rc2 = qeth_halt_channel(card, &card->write);
	rc3 = qeth_halt_channel(card, &card->data);
>>>>>>> upstream/android-13
	if (rc1)
		return rc1;
	if (rc2)
		return rc2;
	return rc3;
}

static int qeth_clear_channels(struct qeth_card *card)
{
	int rc1 = 0, rc2 = 0, rc3 = 0;

	QETH_CARD_TEXT(card, 3, "clearchs");
<<<<<<< HEAD
	rc1 = qeth_clear_channel(&card->read);
	rc2 = qeth_clear_channel(&card->write);
	rc3 = qeth_clear_channel(&card->data);
=======
	rc1 = qeth_clear_channel(card, &card->read);
	rc2 = qeth_clear_channel(card, &card->write);
	rc3 = qeth_clear_channel(card, &card->data);
>>>>>>> upstream/android-13
	if (rc1)
		return rc1;
	if (rc2)
		return rc2;
	return rc3;
}

static int qeth_clear_halt_card(struct qeth_card *card, int halt)
{
	int rc = 0;

	QETH_CARD_TEXT(card, 3, "clhacrd");

	if (halt)
		rc = qeth_halt_channels(card);
	if (rc)
		return rc;
	return qeth_clear_channels(card);
}

<<<<<<< HEAD
int qeth_qdio_clear_card(struct qeth_card *card, int use_halt)
=======
static int qeth_qdio_clear_card(struct qeth_card *card, int use_halt)
>>>>>>> upstream/android-13
{
	int rc = 0;

	QETH_CARD_TEXT(card, 3, "qdioclr");
	switch (atomic_cmpxchg(&card->qdio.state, QETH_QDIO_ESTABLISHED,
		QETH_QDIO_CLEANING)) {
	case QETH_QDIO_ESTABLISHED:
<<<<<<< HEAD
		if (card->info.type == QETH_CARD_TYPE_IQD)
=======
		if (IS_IQD(card))
>>>>>>> upstream/android-13
			rc = qdio_shutdown(CARD_DDEV(card),
				QDIO_FLAG_CLEANUP_USING_HALT);
		else
			rc = qdio_shutdown(CARD_DDEV(card),
				QDIO_FLAG_CLEANUP_USING_CLEAR);
		if (rc)
			QETH_CARD_TEXT_(card, 3, "1err%d", rc);
		atomic_set(&card->qdio.state, QETH_QDIO_ALLOCATED);
		break;
	case QETH_QDIO_CLEANING:
		return rc;
	default:
		break;
	}
	rc = qeth_clear_halt_card(card, use_halt);
	if (rc)
		QETH_CARD_TEXT_(card, 3, "2err%d", rc);
<<<<<<< HEAD
	card->state = CARD_STATE_DOWN;
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_qdio_clear_card);

static int qeth_read_conf_data(struct qeth_card *card, void **buffer,
			       int *length)
{
	struct ciw *ciw;
	char *rcd_buf;
	int ret;
	struct qeth_channel *channel = &card->data;
	unsigned long flags;

	/*
	 * scan for RCD command in extended SenseID data
	 */
	ciw = ccw_device_get_ciw(channel->ccwdev, CIW_TYPE_RCD);
	if (!ciw || ciw->cmd == 0)
		return -EOPNOTSUPP;
	rcd_buf = kzalloc(ciw->count, GFP_KERNEL | GFP_DMA);
	if (!rcd_buf)
		return -ENOMEM;

	qeth_setup_ccw(channel->ccw, ciw->cmd, ciw->count, rcd_buf);
	channel->state = CH_STATE_RCD;
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	ret = ccw_device_start_timeout(channel->ccwdev, channel->ccw,
				       QETH_RCD_PARM, LPM_ANYPATH, 0,
				       QETH_RCD_TIMEOUT);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);
	if (!ret)
		wait_event(card->wait_q,
			   (channel->state == CH_STATE_RCD_DONE ||
			    channel->state == CH_STATE_DOWN));
	if (channel->state == CH_STATE_DOWN)
		ret = -EIO;
	else
		channel->state = CH_STATE_DOWN;
	if (ret) {
		kfree(rcd_buf);
		*buffer = NULL;
		*length = 0;
	} else {
		*length = ciw->count;
		*buffer = rcd_buf;
	}
	return ret;
}

static void qeth_configure_unitaddr(struct qeth_card *card, char *prcd)
{
	QETH_DBF_TEXT(SETUP, 2, "cfgunit");
	card->info.chpid = prcd[30];
	card->info.unit_addr2 = prcd[31];
	card->info.cula = prcd[63];
	card->info.guestlan = ((prcd[0x10] == _ascebc['V']) &&
			       (prcd[0x11] == _ascebc['M']));
}
=======
	return rc;
}
>>>>>>> upstream/android-13

static enum qeth_discipline_id qeth_vm_detect_layer(struct qeth_card *card)
{
	enum qeth_discipline_id disc = QETH_DISCIPLINE_UNDETERMINED;
	struct diag26c_vnic_resp *response = NULL;
	struct diag26c_vnic_req *request = NULL;
	struct ccw_dev_id id;
	char userid[80];
	int rc = 0;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "vmlayer");
=======
	QETH_CARD_TEXT(card, 2, "vmlayer");
>>>>>>> upstream/android-13

	cpcmd("QUERY USERID", userid, sizeof(userid), &rc);
	if (rc)
		goto out;

	request = kzalloc(sizeof(*request), GFP_KERNEL | GFP_DMA);
	response = kzalloc(sizeof(*response), GFP_KERNEL | GFP_DMA);
	if (!request || !response) {
		rc = -ENOMEM;
		goto out;
	}

	ccw_device_get_id(CARD_RDEV(card), &id);
	request->resp_buf_len = sizeof(*response);
	request->resp_version = DIAG26C_VERSION6_VM65918;
	request->req_format = DIAG26C_VNIC_INFO;
	ASCEBC(userid, 8);
	memcpy(&request->sys_name, userid, 8);
	request->devno = id.devno;

	QETH_DBF_HEX(CTRL, 2, request, sizeof(*request));
	rc = diag26c(request, response, DIAG26C_PORT_VNIC);
	QETH_DBF_HEX(CTRL, 2, request, sizeof(*request));
	if (rc)
		goto out;
	QETH_DBF_HEX(CTRL, 2, response, sizeof(*response));

	if (request->resp_buf_len < sizeof(*response) ||
	    response->version != request->resp_version) {
		rc = -EIO;
		goto out;
	}

	if (response->protocol == VNIC_INFO_PROT_L2)
		disc = QETH_DISCIPLINE_LAYER2;
	else if (response->protocol == VNIC_INFO_PROT_L3)
		disc = QETH_DISCIPLINE_LAYER3;

out:
	kfree(response);
	kfree(request);
	if (rc)
<<<<<<< HEAD
		QETH_DBF_TEXT_(SETUP, 2, "err%x", rc);
=======
		QETH_CARD_TEXT_(card, 2, "err%x", rc);
>>>>>>> upstream/android-13
	return disc;
}

/* Determine whether the device requires a specific layer discipline */
static enum qeth_discipline_id qeth_enforce_discipline(struct qeth_card *card)
{
	enum qeth_discipline_id disc = QETH_DISCIPLINE_UNDETERMINED;

<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_OSM ||
	    card->info.type == QETH_CARD_TYPE_OSN)
		disc = QETH_DISCIPLINE_LAYER2;
	else if (card->info.guestlan)
		disc = (card->info.type == QETH_CARD_TYPE_IQD) ?
				QETH_DISCIPLINE_LAYER3 :
				qeth_vm_detect_layer(card);

	switch (disc) {
	case QETH_DISCIPLINE_LAYER2:
		QETH_DBF_TEXT(SETUP, 3, "force l2");
		break;
	case QETH_DISCIPLINE_LAYER3:
		QETH_DBF_TEXT(SETUP, 3, "force l3");
		break;
	default:
		QETH_DBF_TEXT(SETUP, 3, "force no");
=======
	if (IS_OSM(card))
		disc = QETH_DISCIPLINE_LAYER2;
	else if (IS_VM_NIC(card))
		disc = IS_IQD(card) ? QETH_DISCIPLINE_LAYER3 :
				      qeth_vm_detect_layer(card);

	switch (disc) {
	case QETH_DISCIPLINE_LAYER2:
		QETH_CARD_TEXT(card, 3, "force l2");
		break;
	case QETH_DISCIPLINE_LAYER3:
		QETH_CARD_TEXT(card, 3, "force l3");
		break;
	default:
		QETH_CARD_TEXT(card, 3, "force no");
>>>>>>> upstream/android-13
	}

	return disc;
}

<<<<<<< HEAD
static void qeth_configure_blkt_default(struct qeth_card *card, char *prcd)
{
	QETH_DBF_TEXT(SETUP, 2, "cfgblkt");

	if (prcd[74] == 0xF0 && prcd[75] == 0xF0 &&
	    prcd[76] >= 0xF1 && prcd[76] <= 0xF4) {
=======
static void qeth_set_blkt_defaults(struct qeth_card *card)
{
	QETH_CARD_TEXT(card, 2, "cfgblkt");

	if (card->info.use_v1_blkt) {
>>>>>>> upstream/android-13
		card->info.blkt.time_total = 0;
		card->info.blkt.inter_packet = 0;
		card->info.blkt.inter_packet_jumbo = 0;
	} else {
		card->info.blkt.time_total = 250;
		card->info.blkt.inter_packet = 5;
		card->info.blkt.inter_packet_jumbo = 15;
	}
}

<<<<<<< HEAD
static void qeth_init_tokens(struct qeth_card *card)
{
=======
static void qeth_idx_init(struct qeth_card *card)
{
	memset(&card->seqno, 0, sizeof(card->seqno));

>>>>>>> upstream/android-13
	card->token.issuer_rm_w = 0x00010103UL;
	card->token.cm_filter_w = 0x00010108UL;
	card->token.cm_connection_w = 0x0001010aUL;
	card->token.ulp_filter_w = 0x0001010bUL;
	card->token.ulp_connection_w = 0x0001010dUL;
<<<<<<< HEAD
}

static void qeth_init_func_level(struct qeth_card *card)
{
=======

>>>>>>> upstream/android-13
	switch (card->info.type) {
	case QETH_CARD_TYPE_IQD:
		card->info.func_level =	QETH_IDX_FUNC_LEVEL_IQD;
		break;
	case QETH_CARD_TYPE_OSD:
<<<<<<< HEAD
	case QETH_CARD_TYPE_OSN:
=======
>>>>>>> upstream/android-13
		card->info.func_level = QETH_IDX_FUNC_LEVEL_OSD;
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
static int qeth_idx_activate_get_answer(struct qeth_channel *channel,
		void (*idx_reply_cb)(struct qeth_channel *,
			struct qeth_cmd_buffer *))
{
	struct qeth_cmd_buffer *iob;
	unsigned long flags;
	int rc;
	struct qeth_card *card;

	QETH_DBF_TEXT(SETUP, 2, "idxanswr");
	card = CARD_FROM_CDEV(channel->ccwdev);
	iob = qeth_get_buffer(channel);
	if (!iob)
		return -ENOMEM;
	iob->callback = idx_reply_cb;
	qeth_setup_ccw(channel->ccw, CCW_CMD_READ, QETH_BUFSIZE, iob->data);

	wait_event(card->wait_q,
		   atomic_cmpxchg(&channel->irq_pending, 0, 1) == 0);
	QETH_DBF_TEXT(SETUP, 6, "noirqpnd");
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	rc = ccw_device_start_timeout(channel->ccwdev, channel->ccw,
				      (addr_t) iob, 0, 0, QETH_TIMEOUT);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);

	if (rc) {
		QETH_DBF_MESSAGE(2, "Error2 in activating channel rc=%d\n", rc);
		QETH_DBF_TEXT_(SETUP, 2, "2err%d", rc);
		atomic_set(&channel->irq_pending, 0);
		qeth_release_buffer(channel, iob);
		wake_up(&card->wait_q);
		return rc;
	}
	rc = wait_event_interruptible_timeout(card->wait_q,
			 channel->state == CH_STATE_UP, QETH_TIMEOUT);
	if (rc == -ERESTARTSYS)
		return rc;
	if (channel->state != CH_STATE_UP) {
		rc = -ETIME;
		QETH_DBF_TEXT_(SETUP, 2, "3err%d", rc);
	} else
		rc = 0;
	return rc;
}

static int qeth_idx_activate_channel(struct qeth_channel *channel,
		void (*idx_reply_cb)(struct qeth_channel *,
			struct qeth_cmd_buffer *))
{
	struct qeth_card *card;
	struct qeth_cmd_buffer *iob;
	unsigned long flags;
	__u16 temp;
	__u8 tmp;
	int rc;
	struct ccw_dev_id temp_devid;

	card = CARD_FROM_CDEV(channel->ccwdev);

	QETH_DBF_TEXT(SETUP, 2, "idxactch");

	iob = qeth_get_buffer(channel);
	if (!iob)
		return -ENOMEM;
	iob->callback = idx_reply_cb;
	qeth_setup_ccw(channel->ccw, CCW_CMD_WRITE, IDX_ACTIVATE_SIZE,
		       iob->data);
	if (channel == &card->write) {
		memcpy(iob->data, IDX_ACTIVATE_WRITE, IDX_ACTIVATE_SIZE);
		memcpy(QETH_TRANSPORT_HEADER_SEQ_NO(iob->data),
		       &card->seqno.trans_hdr, QETH_SEQ_NO_LENGTH);
		card->seqno.trans_hdr++;
	} else {
		memcpy(iob->data, IDX_ACTIVATE_READ, IDX_ACTIVATE_SIZE);
		memcpy(QETH_TRANSPORT_HEADER_SEQ_NO(iob->data),
		       &card->seqno.trans_hdr, QETH_SEQ_NO_LENGTH);
	}
	tmp = ((u8)card->dev->dev_port) | 0x80;
	memcpy(QETH_IDX_ACT_PNO(iob->data), &tmp, 1);
	memcpy(QETH_IDX_ACT_ISSUER_RM_TOKEN(iob->data),
	       &card->token.issuer_rm_w, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_IDX_ACT_FUNC_LEVEL(iob->data),
	       &card->info.func_level, sizeof(__u16));
	ccw_device_get_id(CARD_DDEV(card), &temp_devid);
	memcpy(QETH_IDX_ACT_QDIO_DEV_CUA(iob->data), &temp_devid.devno, 2);
	temp = (card->info.cula << 8) + card->info.unit_addr2;
	memcpy(QETH_IDX_ACT_QDIO_DEV_REALADDR(iob->data), &temp, 2);

	wait_event(card->wait_q,
		   atomic_cmpxchg(&channel->irq_pending, 0, 1) == 0);
	QETH_DBF_TEXT(SETUP, 6, "noirqpnd");
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	rc = ccw_device_start_timeout(channel->ccwdev, channel->ccw,
				      (addr_t) iob, 0, 0, QETH_TIMEOUT);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);

	if (rc) {
		QETH_DBF_MESSAGE(2, "Error1 in activating channel. rc=%d\n",
			rc);
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
		atomic_set(&channel->irq_pending, 0);
		qeth_release_buffer(channel, iob);
		wake_up(&card->wait_q);
		return rc;
	}
	rc = wait_event_interruptible_timeout(card->wait_q,
			channel->state == CH_STATE_ACTIVATING, QETH_TIMEOUT);
	if (rc == -ERESTARTSYS)
		return rc;
	if (channel->state != CH_STATE_ACTIVATING) {
		dev_warn(&channel->ccwdev->dev, "The qeth device driver"
			" failed to recover an error on the device\n");
		QETH_DBF_MESSAGE(2, "%s IDX activate timed out\n",
			dev_name(&channel->ccwdev->dev));
		QETH_DBF_TEXT_(SETUP, 2, "2err%d", -ETIME);
		return -ETIME;
	}
	return qeth_idx_activate_get_answer(channel, idx_reply_cb);
=======
static void qeth_idx_finalize_cmd(struct qeth_card *card,
				  struct qeth_cmd_buffer *iob)
{
	memcpy(QETH_TRANSPORT_HEADER_SEQ_NO(iob->data), &card->seqno.trans_hdr,
	       QETH_SEQ_NO_LENGTH);
	if (iob->channel == &card->write)
		card->seqno.trans_hdr++;
>>>>>>> upstream/android-13
}

static int qeth_peer_func_level(int level)
{
	if ((level & 0xff) == 8)
		return (level & 0xff) + 0x400;
	if (((level >> 8) & 3) == 1)
		return (level & 0xff) + 0x200;
	return level;
}

<<<<<<< HEAD
static void qeth_idx_write_cb(struct qeth_channel *channel,
		struct qeth_cmd_buffer *iob)
{
	struct qeth_card *card;
	__u16 temp;

	QETH_DBF_TEXT(SETUP , 2, "idxwrcb");

	if (channel->state == CH_STATE_DOWN) {
		channel->state = CH_STATE_ACTIVATING;
		goto out;
	}
	card = CARD_FROM_CDEV(channel->ccwdev);

	if (!(QETH_IS_IDX_ACT_POS_REPLY(iob->data))) {
		if (QETH_IDX_ACT_CAUSE_CODE(iob->data) == QETH_IDX_ACT_ERR_EXCL)
			dev_err(&channel->ccwdev->dev,
				"The adapter is used exclusively by another "
				"host\n");
		else
			QETH_DBF_MESSAGE(2, "%s IDX_ACTIVATE on write channel:"
				" negative reply\n",
				dev_name(&channel->ccwdev->dev));
		goto out;
	}
	memcpy(&temp, QETH_IDX_ACT_FUNC_LEVEL(iob->data), 2);
	if ((temp & ~0x0100) != qeth_peer_func_level(card->info.func_level)) {
		QETH_DBF_MESSAGE(2, "%s IDX_ACTIVATE on write channel: "
			"function level mismatch (sent: 0x%x, received: "
			"0x%x)\n", dev_name(&channel->ccwdev->dev),
			card->info.func_level, temp);
		goto out;
	}
	channel->state = CH_STATE_UP;
out:
	qeth_release_buffer(channel, iob);
}

static void qeth_idx_read_cb(struct qeth_channel *channel,
		struct qeth_cmd_buffer *iob)
{
	struct qeth_card *card;
	__u16 temp;

	QETH_DBF_TEXT(SETUP , 2, "idxrdcb");
	if (channel->state == CH_STATE_DOWN) {
		channel->state = CH_STATE_ACTIVATING;
		goto out;
	}

	card = CARD_FROM_CDEV(channel->ccwdev);
	if (qeth_check_idx_response(card, iob->data))
			goto out;

	if (!(QETH_IS_IDX_ACT_POS_REPLY(iob->data))) {
		switch (QETH_IDX_ACT_CAUSE_CODE(iob->data)) {
		case QETH_IDX_ACT_ERR_EXCL:
			dev_err(&channel->ccwdev->dev,
				"The adapter is used exclusively by another "
				"host\n");
			break;
		case QETH_IDX_ACT_ERR_AUTH:
		case QETH_IDX_ACT_ERR_AUTH_USER:
			dev_err(&channel->ccwdev->dev,
				"Setting the device online failed because of "
				"insufficient authorization\n");
			break;
		default:
			QETH_DBF_MESSAGE(2, "%s IDX_ACTIVATE on read channel:"
				" negative reply\n",
				dev_name(&channel->ccwdev->dev));
		}
		QETH_CARD_TEXT_(card, 2, "idxread%c",
			QETH_IDX_ACT_CAUSE_CODE(iob->data));
		goto out;
	}

	memcpy(&temp, QETH_IDX_ACT_FUNC_LEVEL(iob->data), 2);
	if (temp != qeth_peer_func_level(card->info.func_level)) {
		QETH_DBF_MESSAGE(2, "%s IDX_ACTIVATE on read channel: function "
			"level mismatch (sent: 0x%x, received: 0x%x)\n",
			dev_name(&channel->ccwdev->dev),
			card->info.func_level, temp);
		goto out;
	}
	memcpy(&card->token.issuer_rm_r,
	       QETH_IDX_ACT_ISSUER_RM_TOKEN(iob->data),
	       QETH_MPC_TOKEN_LENGTH);
	memcpy(&card->info.mcl_level[0],
	       QETH_IDX_REPLY_LEVEL(iob->data), QETH_MCL_LENGTH);
	channel->state = CH_STATE_UP;
out:
	qeth_release_buffer(channel, iob);
}

void qeth_prepare_control_data(struct qeth_card *card, int len,
		struct qeth_cmd_buffer *iob)
{
	qeth_setup_ccw(iob->channel->ccw, CCW_CMD_WRITE, len, iob->data);
	iob->callback = qeth_release_buffer;

	memcpy(QETH_TRANSPORT_HEADER_SEQ_NO(iob->data),
	       &card->seqno.trans_hdr, QETH_SEQ_NO_LENGTH);
	card->seqno.trans_hdr++;
=======
static void qeth_mpc_finalize_cmd(struct qeth_card *card,
				  struct qeth_cmd_buffer *iob)
{
	qeth_idx_finalize_cmd(card, iob);

>>>>>>> upstream/android-13
	memcpy(QETH_PDU_HEADER_SEQ_NO(iob->data),
	       &card->seqno.pdu_hdr, QETH_SEQ_NO_LENGTH);
	card->seqno.pdu_hdr++;
	memcpy(QETH_PDU_HEADER_ACK_SEQ_NO(iob->data),
	       &card->seqno.pdu_hdr_ack, QETH_SEQ_NO_LENGTH);
<<<<<<< HEAD
	QETH_DBF_HEX(CTRL, 2, iob->data, QETH_DBF_CTRL_LEN);
}
EXPORT_SYMBOL_GPL(qeth_prepare_control_data);
=======

	iob->callback = qeth_release_buffer_cb;
}

static bool qeth_mpc_match_reply(struct qeth_cmd_buffer *iob,
				 struct qeth_cmd_buffer *reply)
{
	/* MPC cmds are issued strictly in sequence. */
	return !IS_IPA(reply->data);
}

static struct qeth_cmd_buffer *qeth_mpc_alloc_cmd(struct qeth_card *card,
						  const void *data,
						  unsigned int data_length)
{
	struct qeth_cmd_buffer *iob;

	iob = qeth_alloc_cmd(&card->write, data_length, 1, QETH_TIMEOUT);
	if (!iob)
		return NULL;

	memcpy(iob->data, data, data_length);
	qeth_setup_ccw(__ccw_from_cmd(iob), CCW_CMD_WRITE, 0, data_length,
		       iob->data);
	iob->finalize = qeth_mpc_finalize_cmd;
	iob->match = qeth_mpc_match_reply;
	return iob;
}
>>>>>>> upstream/android-13

/**
 * qeth_send_control_data() -	send control command to the card
 * @card:			qeth_card structure pointer
<<<<<<< HEAD
 * @len:			size of the command buffer
=======
>>>>>>> upstream/android-13
 * @iob:			qeth_cmd_buffer pointer
 * @reply_cb:			callback function pointer
 * @cb_card:			pointer to the qeth_card structure
 * @cb_reply:			pointer to the qeth_reply structure
 * @cb_cmd:			pointer to the original iob for non-IPA
 *				commands, or to the qeth_ipa_cmd structure
 *				for the IPA commands.
 * @reply_param:		private pointer passed to the callback
 *
<<<<<<< HEAD
 * Returns the value of the `return_code' field of the response
 * block returned from the hardware, or other error indication.
 * Value of zero indicates successful execution of the command.
 *
 * Callback function gets called one or more times, with cb_cmd
 * pointing to the response returned by the hardware. Callback
 * function must return non-zero if more reply blocks are expected,
 * and zero if the last or only reply block is received. Callback
 * function can get the value of the reply_param pointer from the
 * field 'param' of the structure qeth_reply.
 */

int qeth_send_control_data(struct qeth_card *card, int len,
		struct qeth_cmd_buffer *iob,
		int (*reply_cb)(struct qeth_card *cb_card,
				struct qeth_reply *cb_reply,
				unsigned long cb_cmd),
		void *reply_param)
{
	struct qeth_channel *channel = iob->channel;
	int rc;
	unsigned long flags;
	struct qeth_reply *reply = NULL;
	unsigned long timeout, event_timeout;
	struct qeth_ipa_cmd *cmd = NULL;

	QETH_CARD_TEXT(card, 2, "sendctl");

	if (card->read_or_write_problem) {
		qeth_release_buffer(channel, iob);
		return -EIO;
	}
	reply = qeth_alloc_reply(card);
	if (!reply) {
		qeth_release_buffer(channel, iob);
		return -ENOMEM;
	}
	reply->callback = reply_cb;
	reply->param = reply_param;

	init_waitqueue_head(&reply->wait_q);

	while (atomic_cmpxchg(&channel->irq_pending, 0, 1)) ;

	if (IS_IPA(iob->data)) {
		cmd = __ipa_cmd(iob);
		cmd->hdr.seqno = card->seqno.ipa++;
		reply->seqno = cmd->hdr.seqno;
		event_timeout = QETH_IPA_TIMEOUT;
	} else {
		reply->seqno = QETH_IDX_COMMAND_SEQNO;
		event_timeout = QETH_TIMEOUT;
	}
	qeth_prepare_control_data(card, len, iob);

	spin_lock_irqsave(&card->lock, flags);
	list_add_tail(&reply->list, &card->cmd_waiter_list);
	spin_unlock_irqrestore(&card->lock, flags);

	timeout = jiffies + event_timeout;

	QETH_CARD_TEXT(card, 6, "noirqpnd");
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	rc = ccw_device_start_timeout(channel->ccwdev, channel->ccw,
				      (addr_t) iob, 0, 0, event_timeout);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);
	if (rc) {
		QETH_DBF_MESSAGE(2, "%s qeth_send_control_data: "
			"ccw_device_start rc = %i\n",
			dev_name(&channel->ccwdev->dev), rc);
		QETH_CARD_TEXT_(card, 2, " err%d", rc);
		spin_lock_irqsave(&card->lock, flags);
		list_del_init(&reply->list);
		qeth_put_reply(reply);
		spin_unlock_irqrestore(&card->lock, flags);
		qeth_release_buffer(channel, iob);
		atomic_set(&channel->irq_pending, 0);
		wake_up(&card->wait_q);
		return rc;
	}

	/* we have only one long running ipassist, since we can ensure
	   process context of this command we can sleep */
	if (cmd && cmd->hdr.command == IPA_CMD_SETIP &&
	    cmd->hdr.prot_version == QETH_PROT_IPV4) {
		if (!wait_event_timeout(reply->wait_q,
		    atomic_read(&reply->received), event_timeout))
			goto time_err;
	} else {
		while (!atomic_read(&reply->received)) {
			if (time_after(jiffies, timeout))
				goto time_err;
			cpu_relax();
		}
	}

	rc = reply->rc;
	qeth_put_reply(reply);
	return rc;

time_err:
	reply->rc = -ETIME;
	spin_lock_irqsave(&reply->card->lock, flags);
	list_del_init(&reply->list);
	spin_unlock_irqrestore(&reply->card->lock, flags);
	atomic_inc(&reply->received);
	rc = reply->rc;
	qeth_put_reply(reply);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_send_control_data);
=======
 * Callback function gets called one or more times, with cb_cmd
 * pointing to the response returned by the hardware. Callback
 * function must return
 *   > 0 if more reply blocks are expected,
 *     0 if the last or only reply block is received, and
 *   < 0 on error.
 * Callback function can get the value of the reply_param pointer from the
 * field 'param' of the structure qeth_reply.
 */

static int qeth_send_control_data(struct qeth_card *card,
				  struct qeth_cmd_buffer *iob,
				  int (*reply_cb)(struct qeth_card *cb_card,
						  struct qeth_reply *cb_reply,
						  unsigned long cb_cmd),
				  void *reply_param)
{
	struct qeth_channel *channel = iob->channel;
	struct qeth_reply *reply = &iob->reply;
	long timeout = iob->timeout;
	int rc;

	QETH_CARD_TEXT(card, 2, "sendctl");

	reply->callback = reply_cb;
	reply->param = reply_param;

	timeout = wait_event_interruptible_timeout(card->wait_q,
						   qeth_trylock_channel(channel, iob),
						   timeout);
	if (timeout <= 0) {
		qeth_put_cmd(iob);
		return (timeout == -ERESTARTSYS) ? -EINTR : -ETIME;
	}

	if (iob->finalize)
		iob->finalize(card, iob);
	QETH_DBF_HEX(CTRL, 2, iob->data, min(iob->length, QETH_DBF_CTRL_LEN));

	qeth_enqueue_cmd(card, iob);

	/* This pairs with iob->callback, and keeps the iob alive after IO: */
	qeth_get_cmd(iob);

	QETH_CARD_TEXT(card, 6, "noirqpnd");
	spin_lock_irq(get_ccwdev_lock(channel->ccwdev));
	rc = ccw_device_start_timeout(channel->ccwdev, __ccw_from_cmd(iob),
				      (addr_t) iob, 0, 0, timeout);
	spin_unlock_irq(get_ccwdev_lock(channel->ccwdev));
	if (rc) {
		QETH_DBF_MESSAGE(2, "qeth_send_control_data on device %x: ccw_device_start rc = %i\n",
				 CARD_DEVID(card), rc);
		QETH_CARD_TEXT_(card, 2, " err%d", rc);
		qeth_dequeue_cmd(card, iob);
		qeth_put_cmd(iob);
		qeth_unlock_channel(card, channel);
		goto out;
	}

	timeout = wait_for_completion_interruptible_timeout(&iob->done,
							    timeout);
	if (timeout <= 0)
		rc = (timeout == -ERESTARTSYS) ? -EINTR : -ETIME;

	qeth_dequeue_cmd(card, iob);

	if (reply_cb) {
		/* Wait until the callback for a late reply has completed: */
		spin_lock_irq(&iob->lock);
		if (rc)
			/* Zap any callback that's still pending: */
			iob->rc = rc;
		spin_unlock_irq(&iob->lock);
	}

	if (!rc)
		rc = iob->rc;

out:
	qeth_put_cmd(iob);
	return rc;
}

struct qeth_node_desc {
	struct node_descriptor nd1;
	struct node_descriptor nd2;
	struct node_descriptor nd3;
};

static void qeth_read_conf_data_cb(struct qeth_card *card,
				   struct qeth_cmd_buffer *iob,
				   unsigned int data_length)
{
	struct qeth_node_desc *nd = (struct qeth_node_desc *) iob->data;
	int rc = 0;
	u8 *tag;

	QETH_CARD_TEXT(card, 2, "cfgunit");

	if (data_length < sizeof(*nd)) {
		rc = -EINVAL;
		goto out;
	}

	card->info.is_vm_nic = nd->nd1.plant[0] == _ascebc['V'] &&
			       nd->nd1.plant[1] == _ascebc['M'];
	tag = (u8 *)&nd->nd1.tag;
	card->info.chpid = tag[0];
	card->info.unit_addr2 = tag[1];

	tag = (u8 *)&nd->nd2.tag;
	card->info.cula = tag[1];

	card->info.use_v1_blkt = nd->nd3.model[0] == 0xF0 &&
				 nd->nd3.model[1] == 0xF0 &&
				 nd->nd3.model[2] >= 0xF1 &&
				 nd->nd3.model[2] <= 0xF4;

out:
	qeth_notify_cmd(iob, rc);
	qeth_put_cmd(iob);
}

static int qeth_read_conf_data(struct qeth_card *card)
{
	struct qeth_channel *channel = &card->data;
	struct qeth_cmd_buffer *iob;
	struct ciw *ciw;

	/* scan for RCD command in extended SenseID data */
	ciw = ccw_device_get_ciw(channel->ccwdev, CIW_TYPE_RCD);
	if (!ciw || ciw->cmd == 0)
		return -EOPNOTSUPP;
	if (ciw->count < sizeof(struct qeth_node_desc))
		return -EINVAL;

	iob = qeth_alloc_cmd(channel, ciw->count, 1, QETH_RCD_TIMEOUT);
	if (!iob)
		return -ENOMEM;

	iob->callback = qeth_read_conf_data_cb;
	qeth_setup_ccw(__ccw_from_cmd(iob), ciw->cmd, 0, iob->length,
		       iob->data);

	return qeth_send_control_data(card, iob, NULL, NULL);
}

static int qeth_idx_check_activate_response(struct qeth_card *card,
					    struct qeth_channel *channel,
					    struct qeth_cmd_buffer *iob)
{
	int rc;

	rc = qeth_check_idx_response(card, iob->data);
	if (rc)
		return rc;

	if (QETH_IS_IDX_ACT_POS_REPLY(iob->data))
		return 0;

	/* negative reply: */
	QETH_CARD_TEXT_(card, 2, "idxneg%c",
			QETH_IDX_ACT_CAUSE_CODE(iob->data));

	switch (QETH_IDX_ACT_CAUSE_CODE(iob->data)) {
	case QETH_IDX_ACT_ERR_EXCL:
		dev_err(&channel->ccwdev->dev,
			"The adapter is used exclusively by another host\n");
		return -EBUSY;
	case QETH_IDX_ACT_ERR_AUTH:
	case QETH_IDX_ACT_ERR_AUTH_USER:
		dev_err(&channel->ccwdev->dev,
			"Setting the device online failed because of insufficient authorization\n");
		return -EPERM;
	default:
		QETH_DBF_MESSAGE(2, "IDX_ACTIVATE on channel %x: negative reply\n",
				 CCW_DEVID(channel->ccwdev));
		return -EIO;
	}
}

static void qeth_idx_activate_read_channel_cb(struct qeth_card *card,
					      struct qeth_cmd_buffer *iob,
					      unsigned int data_length)
{
	struct qeth_channel *channel = iob->channel;
	u16 peer_level;
	int rc;

	QETH_CARD_TEXT(card, 2, "idxrdcb");

	rc = qeth_idx_check_activate_response(card, channel, iob);
	if (rc)
		goto out;

	memcpy(&peer_level, QETH_IDX_ACT_FUNC_LEVEL(iob->data), 2);
	if (peer_level != qeth_peer_func_level(card->info.func_level)) {
		QETH_DBF_MESSAGE(2, "IDX_ACTIVATE on channel %x: function level mismatch (sent: %#x, received: %#x)\n",
				 CCW_DEVID(channel->ccwdev),
				 card->info.func_level, peer_level);
		rc = -EINVAL;
		goto out;
	}

	memcpy(&card->token.issuer_rm_r,
	       QETH_IDX_ACT_ISSUER_RM_TOKEN(iob->data),
	       QETH_MPC_TOKEN_LENGTH);
	memcpy(&card->info.mcl_level[0],
	       QETH_IDX_REPLY_LEVEL(iob->data), QETH_MCL_LENGTH);

out:
	qeth_notify_cmd(iob, rc);
	qeth_put_cmd(iob);
}

static void qeth_idx_activate_write_channel_cb(struct qeth_card *card,
					       struct qeth_cmd_buffer *iob,
					       unsigned int data_length)
{
	struct qeth_channel *channel = iob->channel;
	u16 peer_level;
	int rc;

	QETH_CARD_TEXT(card, 2, "idxwrcb");

	rc = qeth_idx_check_activate_response(card, channel, iob);
	if (rc)
		goto out;

	memcpy(&peer_level, QETH_IDX_ACT_FUNC_LEVEL(iob->data), 2);
	if ((peer_level & ~0x0100) !=
	    qeth_peer_func_level(card->info.func_level)) {
		QETH_DBF_MESSAGE(2, "IDX_ACTIVATE on channel %x: function level mismatch (sent: %#x, received: %#x)\n",
				 CCW_DEVID(channel->ccwdev),
				 card->info.func_level, peer_level);
		rc = -EINVAL;
	}

out:
	qeth_notify_cmd(iob, rc);
	qeth_put_cmd(iob);
}

static void qeth_idx_setup_activate_cmd(struct qeth_card *card,
					struct qeth_cmd_buffer *iob)
{
	u16 addr = (card->info.cula << 8) + card->info.unit_addr2;
	u8 port = ((u8)card->dev->dev_port) | 0x80;
	struct ccw1 *ccw = __ccw_from_cmd(iob);

	qeth_setup_ccw(&ccw[0], CCW_CMD_WRITE, CCW_FLAG_CC, IDX_ACTIVATE_SIZE,
		       iob->data);
	qeth_setup_ccw(&ccw[1], CCW_CMD_READ, 0, iob->length, iob->data);
	iob->finalize = qeth_idx_finalize_cmd;

	port |= QETH_IDX_ACT_INVAL_FRAME;
	memcpy(QETH_IDX_ACT_PNO(iob->data), &port, 1);
	memcpy(QETH_IDX_ACT_ISSUER_RM_TOKEN(iob->data),
	       &card->token.issuer_rm_w, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_IDX_ACT_FUNC_LEVEL(iob->data),
	       &card->info.func_level, 2);
	memcpy(QETH_IDX_ACT_QDIO_DEV_CUA(iob->data), &card->info.ddev_devno, 2);
	memcpy(QETH_IDX_ACT_QDIO_DEV_REALADDR(iob->data), &addr, 2);
}

static int qeth_idx_activate_read_channel(struct qeth_card *card)
{
	struct qeth_channel *channel = &card->read;
	struct qeth_cmd_buffer *iob;
	int rc;

	QETH_CARD_TEXT(card, 2, "idxread");

	iob = qeth_alloc_cmd(channel, QETH_BUFSIZE, 2, QETH_TIMEOUT);
	if (!iob)
		return -ENOMEM;

	memcpy(iob->data, IDX_ACTIVATE_READ, IDX_ACTIVATE_SIZE);
	qeth_idx_setup_activate_cmd(card, iob);
	iob->callback = qeth_idx_activate_read_channel_cb;

	rc = qeth_send_control_data(card, iob, NULL, NULL);
	if (rc)
		return rc;

	channel->state = CH_STATE_UP;
	return 0;
}

static int qeth_idx_activate_write_channel(struct qeth_card *card)
{
	struct qeth_channel *channel = &card->write;
	struct qeth_cmd_buffer *iob;
	int rc;

	QETH_CARD_TEXT(card, 2, "idxwrite");

	iob = qeth_alloc_cmd(channel, QETH_BUFSIZE, 2, QETH_TIMEOUT);
	if (!iob)
		return -ENOMEM;

	memcpy(iob->data, IDX_ACTIVATE_WRITE, IDX_ACTIVATE_SIZE);
	qeth_idx_setup_activate_cmd(card, iob);
	iob->callback = qeth_idx_activate_write_channel_cb;

	rc = qeth_send_control_data(card, iob, NULL, NULL);
	if (rc)
		return rc;

	channel->state = CH_STATE_UP;
	return 0;
}
>>>>>>> upstream/android-13

static int qeth_cm_enable_cb(struct qeth_card *card, struct qeth_reply *reply,
		unsigned long data)
{
	struct qeth_cmd_buffer *iob;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "cmenblcb");
=======
	QETH_CARD_TEXT(card, 2, "cmenblcb");
>>>>>>> upstream/android-13

	iob = (struct qeth_cmd_buffer *) data;
	memcpy(&card->token.cm_filter_r,
	       QETH_CM_ENABLE_RESP_FILTER_TOKEN(iob->data),
	       QETH_MPC_TOKEN_LENGTH);
<<<<<<< HEAD
	QETH_DBF_TEXT_(SETUP, 2, "  rc%d", iob->rc);
=======
>>>>>>> upstream/android-13
	return 0;
}

static int qeth_cm_enable(struct qeth_card *card)
{
<<<<<<< HEAD
	int rc;
	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "cmenable");

	iob = qeth_wait_for_buffer(&card->write);
	memcpy(iob->data, CM_ENABLE, CM_ENABLE_SIZE);
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "cmenable");

	iob = qeth_mpc_alloc_cmd(card, CM_ENABLE, CM_ENABLE_SIZE);
	if (!iob)
		return -ENOMEM;

>>>>>>> upstream/android-13
	memcpy(QETH_CM_ENABLE_ISSUER_RM_TOKEN(iob->data),
	       &card->token.issuer_rm_r, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_CM_ENABLE_FILTER_TOKEN(iob->data),
	       &card->token.cm_filter_w, QETH_MPC_TOKEN_LENGTH);

<<<<<<< HEAD
	rc = qeth_send_control_data(card, CM_ENABLE_SIZE, iob,
				    qeth_cm_enable_cb, NULL);
	return rc;
=======
	return qeth_send_control_data(card, iob, qeth_cm_enable_cb, NULL);
>>>>>>> upstream/android-13
}

static int qeth_cm_setup_cb(struct qeth_card *card, struct qeth_reply *reply,
		unsigned long data)
{
<<<<<<< HEAD

	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "cmsetpcb");
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "cmsetpcb");
>>>>>>> upstream/android-13

	iob = (struct qeth_cmd_buffer *) data;
	memcpy(&card->token.cm_connection_r,
	       QETH_CM_SETUP_RESP_DEST_ADDR(iob->data),
	       QETH_MPC_TOKEN_LENGTH);
<<<<<<< HEAD
	QETH_DBF_TEXT_(SETUP, 2, "  rc%d", iob->rc);
=======
>>>>>>> upstream/android-13
	return 0;
}

static int qeth_cm_setup(struct qeth_card *card)
{
<<<<<<< HEAD
	int rc;
	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "cmsetup");

	iob = qeth_wait_for_buffer(&card->write);
	memcpy(iob->data, CM_SETUP, CM_SETUP_SIZE);
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "cmsetup");

	iob = qeth_mpc_alloc_cmd(card, CM_SETUP, CM_SETUP_SIZE);
	if (!iob)
		return -ENOMEM;

>>>>>>> upstream/android-13
	memcpy(QETH_CM_SETUP_DEST_ADDR(iob->data),
	       &card->token.issuer_rm_r, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_CM_SETUP_CONNECTION_TOKEN(iob->data),
	       &card->token.cm_connection_w, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_CM_SETUP_FILTER_TOKEN(iob->data),
	       &card->token.cm_filter_r, QETH_MPC_TOKEN_LENGTH);
<<<<<<< HEAD
	rc = qeth_send_control_data(card, CM_SETUP_SIZE, iob,
				    qeth_cm_setup_cb, NULL);
	return rc;

=======
	return qeth_send_control_data(card, iob, qeth_cm_setup_cb, NULL);
}

static bool qeth_is_supported_link_type(struct qeth_card *card, u8 link_type)
{
	if (link_type == QETH_LINK_TYPE_LANE_TR ||
	    link_type == QETH_LINK_TYPE_HSTR) {
		dev_err(&card->gdev->dev, "Unsupported Token Ring device\n");
		return false;
	}

	return true;
>>>>>>> upstream/android-13
}

static int qeth_update_max_mtu(struct qeth_card *card, unsigned int max_mtu)
{
	struct net_device *dev = card->dev;
	unsigned int new_mtu;

	if (!max_mtu) {
		/* IQD needs accurate max MTU to set up its RX buffers: */
		if (IS_IQD(card))
			return -EINVAL;
		/* tolerate quirky HW: */
		max_mtu = ETH_MAX_MTU;
	}

	rtnl_lock();
	if (IS_IQD(card)) {
		/* move any device with default MTU to new max MTU: */
		new_mtu = (dev->mtu == dev->max_mtu) ? max_mtu : dev->mtu;

		/* adjust RX buffer size to new max MTU: */
		card->qdio.in_buf_size = max_mtu + 2 * PAGE_SIZE;
		if (dev->max_mtu && dev->max_mtu != max_mtu)
<<<<<<< HEAD
			qeth_free_qdio_buffers(card);
=======
			qeth_free_qdio_queues(card);
>>>>>>> upstream/android-13
	} else {
		if (dev->mtu)
			new_mtu = dev->mtu;
		/* default MTUs for first setup: */
<<<<<<< HEAD
		else if (card->options.layer2)
=======
		else if (IS_LAYER2(card))
>>>>>>> upstream/android-13
			new_mtu = ETH_DATA_LEN;
		else
			new_mtu = ETH_DATA_LEN - 8; /* allow for LLC + SNAP */
	}

	dev->max_mtu = max_mtu;
	dev->mtu = min(new_mtu, max_mtu);
	rtnl_unlock();
	return 0;
}

static int qeth_get_mtu_outof_framesize(int framesize)
{
	switch (framesize) {
	case 0x4000:
		return 8192;
	case 0x6000:
		return 16384;
	case 0xa000:
		return 32768;
	case 0xffff:
		return 57344;
	default:
		return 0;
	}
}

static int qeth_ulp_enable_cb(struct qeth_card *card, struct qeth_reply *reply,
		unsigned long data)
{
<<<<<<< HEAD

	__u16 mtu, framesize;
	__u16 len;
	__u8 link_type;
	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "ulpenacb");
=======
	__u16 mtu, framesize;
	__u16 len;
	struct qeth_cmd_buffer *iob;
	u8 link_type = 0;

	QETH_CARD_TEXT(card, 2, "ulpenacb");
>>>>>>> upstream/android-13

	iob = (struct qeth_cmd_buffer *) data;
	memcpy(&card->token.ulp_filter_r,
	       QETH_ULP_ENABLE_RESP_FILTER_TOKEN(iob->data),
	       QETH_MPC_TOKEN_LENGTH);
<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_IQD) {
=======
	if (IS_IQD(card)) {
>>>>>>> upstream/android-13
		memcpy(&framesize, QETH_ULP_ENABLE_RESP_MAX_MTU(iob->data), 2);
		mtu = qeth_get_mtu_outof_framesize(framesize);
	} else {
		mtu = *(__u16 *)QETH_ULP_ENABLE_RESP_MAX_MTU(iob->data);
	}
	*(u16 *)reply->param = mtu;

	memcpy(&len, QETH_ULP_ENABLE_RESP_DIFINFO_LEN(iob->data), 2);
	if (len >= QETH_MPC_DIFINFO_LEN_INDICATES_LINK_TYPE) {
		memcpy(&link_type,
		       QETH_ULP_ENABLE_RESP_LINK_TYPE(iob->data), 1);
<<<<<<< HEAD
		card->info.link_type = link_type;
	} else
		card->info.link_type = 0;
	QETH_DBF_TEXT_(SETUP, 2, "link%d", card->info.link_type);
	QETH_DBF_TEXT_(SETUP, 2, "  rc%d", iob->rc);
=======
		if (!qeth_is_supported_link_type(card, link_type))
			return -EPROTONOSUPPORT;
	}

	card->info.link_type = link_type;
	QETH_CARD_TEXT_(card, 2, "link%d", card->info.link_type);
>>>>>>> upstream/android-13
	return 0;
}

static u8 qeth_mpc_select_prot_type(struct qeth_card *card)
{
<<<<<<< HEAD
	if (IS_OSN(card))
		return QETH_PROT_OSN2;
	return (card->options.layer2 == 1) ? QETH_PROT_LAYER2 : QETH_PROT_TCPIP;
=======
	return IS_LAYER2(card) ? QETH_MPC_PROT_L2 : QETH_MPC_PROT_L3;
>>>>>>> upstream/android-13
}

static int qeth_ulp_enable(struct qeth_card *card)
{
	u8 prot_type = qeth_mpc_select_prot_type(card);
	struct qeth_cmd_buffer *iob;
	u16 max_mtu;
	int rc;

<<<<<<< HEAD
	/*FIXME: trace view callbacks*/
	QETH_DBF_TEXT(SETUP, 2, "ulpenabl");

	iob = qeth_wait_for_buffer(&card->write);
	memcpy(iob->data, ULP_ENABLE, ULP_ENABLE_SIZE);
=======
	QETH_CARD_TEXT(card, 2, "ulpenabl");

	iob = qeth_mpc_alloc_cmd(card, ULP_ENABLE, ULP_ENABLE_SIZE);
	if (!iob)
		return -ENOMEM;
>>>>>>> upstream/android-13

	*(QETH_ULP_ENABLE_LINKNUM(iob->data)) = (u8) card->dev->dev_port;
	memcpy(QETH_ULP_ENABLE_PROT_TYPE(iob->data), &prot_type, 1);
	memcpy(QETH_ULP_ENABLE_DEST_ADDR(iob->data),
	       &card->token.cm_connection_r, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_ULP_ENABLE_FILTER_TOKEN(iob->data),
	       &card->token.ulp_filter_w, QETH_MPC_TOKEN_LENGTH);
<<<<<<< HEAD
	rc = qeth_send_control_data(card, ULP_ENABLE_SIZE, iob,
				    qeth_ulp_enable_cb, &max_mtu);
=======
	rc = qeth_send_control_data(card, iob, qeth_ulp_enable_cb, &max_mtu);
>>>>>>> upstream/android-13
	if (rc)
		return rc;
	return qeth_update_max_mtu(card, max_mtu);
}

static int qeth_ulp_setup_cb(struct qeth_card *card, struct qeth_reply *reply,
		unsigned long data)
{
	struct qeth_cmd_buffer *iob;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "ulpstpcb");
=======
	QETH_CARD_TEXT(card, 2, "ulpstpcb");
>>>>>>> upstream/android-13

	iob = (struct qeth_cmd_buffer *) data;
	memcpy(&card->token.ulp_connection_r,
	       QETH_ULP_SETUP_RESP_CONNECTION_TOKEN(iob->data),
	       QETH_MPC_TOKEN_LENGTH);
	if (!strncmp("00S", QETH_ULP_SETUP_RESP_CONNECTION_TOKEN(iob->data),
		     3)) {
<<<<<<< HEAD
		QETH_DBF_TEXT(SETUP, 2, "olmlimit");
		dev_err(&card->gdev->dev, "A connection could not be "
			"established because of an OLM limit\n");
		iob->rc = -EMLINK;
	}
	QETH_DBF_TEXT_(SETUP, 2, "  rc%d", iob->rc);
=======
		QETH_CARD_TEXT(card, 2, "olmlimit");
		dev_err(&card->gdev->dev, "A connection could not be "
			"established because of an OLM limit\n");
		return -EMLINK;
	}
>>>>>>> upstream/android-13
	return 0;
}

static int qeth_ulp_setup(struct qeth_card *card)
{
<<<<<<< HEAD
	int rc;
	__u16 temp;
	struct qeth_cmd_buffer *iob;
	struct ccw_dev_id dev_id;

	QETH_DBF_TEXT(SETUP, 2, "ulpsetup");

	iob = qeth_wait_for_buffer(&card->write);
	memcpy(iob->data, ULP_SETUP, ULP_SETUP_SIZE);
=======
	__u16 temp;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "ulpsetup");

	iob = qeth_mpc_alloc_cmd(card, ULP_SETUP, ULP_SETUP_SIZE);
	if (!iob)
		return -ENOMEM;
>>>>>>> upstream/android-13

	memcpy(QETH_ULP_SETUP_DEST_ADDR(iob->data),
	       &card->token.cm_connection_r, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_ULP_SETUP_CONNECTION_TOKEN(iob->data),
	       &card->token.ulp_connection_w, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_ULP_SETUP_FILTER_TOKEN(iob->data),
	       &card->token.ulp_filter_r, QETH_MPC_TOKEN_LENGTH);

<<<<<<< HEAD
	ccw_device_get_id(CARD_DDEV(card), &dev_id);
	memcpy(QETH_ULP_SETUP_CUA(iob->data), &dev_id.devno, 2);
	temp = (card->info.cula << 8) + card->info.unit_addr2;
	memcpy(QETH_ULP_SETUP_REAL_DEVADDR(iob->data), &temp, 2);
	rc = qeth_send_control_data(card, ULP_SETUP_SIZE, iob,
				    qeth_ulp_setup_cb, NULL);
	return rc;
}

static int qeth_init_qdio_out_buf(struct qeth_qdio_out_q *q, int bidx)
{
	struct qeth_qdio_out_buffer *newbuf;

	newbuf = kmem_cache_zalloc(qeth_qdio_outbuf_cache, GFP_ATOMIC);
=======
	memcpy(QETH_ULP_SETUP_CUA(iob->data), &card->info.ddev_devno, 2);
	temp = (card->info.cula << 8) + card->info.unit_addr2;
	memcpy(QETH_ULP_SETUP_REAL_DEVADDR(iob->data), &temp, 2);
	return qeth_send_control_data(card, iob, qeth_ulp_setup_cb, NULL);
}

static int qeth_alloc_out_buf(struct qeth_qdio_out_q *q, unsigned int bidx,
			      gfp_t gfp)
{
	struct qeth_qdio_out_buffer *newbuf;

	newbuf = kmem_cache_zalloc(qeth_qdio_outbuf_cache, gfp);
>>>>>>> upstream/android-13
	if (!newbuf)
		return -ENOMEM;

	newbuf->buffer = q->qdio_bufs[bidx];
	skb_queue_head_init(&newbuf->skb_list);
	lockdep_set_class(&newbuf->skb_list.lock, &qdio_out_skb_queue_key);
<<<<<<< HEAD
	newbuf->q = q;
	newbuf->next_pending = q->bufs[bidx];
=======
>>>>>>> upstream/android-13
	atomic_set(&newbuf->state, QETH_QDIO_BUF_EMPTY);
	q->bufs[bidx] = newbuf;
	return 0;
}

static void qeth_free_output_queue(struct qeth_qdio_out_q *q)
{
	if (!q)
		return;

<<<<<<< HEAD
	qeth_clear_outq_buffers(q, 1);
=======
	qeth_drain_output_queue(q, true);
>>>>>>> upstream/android-13
	qdio_free_buffers(q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q);
	kfree(q);
}

<<<<<<< HEAD
static struct qeth_qdio_out_q *qeth_alloc_qdio_out_buf(void)
{
	struct qeth_qdio_out_q *q = kzalloc(sizeof(*q), GFP_KERNEL);
=======
static struct qeth_qdio_out_q *qeth_alloc_output_queue(void)
{
	struct qeth_qdio_out_q *q = kzalloc(sizeof(*q), GFP_KERNEL);
	unsigned int i;
>>>>>>> upstream/android-13

	if (!q)
		return NULL;

<<<<<<< HEAD
	if (qdio_alloc_buffers(q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q)) {
		kfree(q);
		return NULL;
	}
	return q;
}

static int qeth_alloc_qdio_buffers(struct qeth_card *card)
{
	int i, j;

	QETH_DBF_TEXT(SETUP, 2, "allcqdbf");
=======
	if (qdio_alloc_buffers(q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q))
		goto err_qdio_bufs;

	for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; i++) {
		if (qeth_alloc_out_buf(q, i, GFP_KERNEL))
			goto err_out_bufs;
	}

	return q;

err_out_bufs:
	while (i > 0)
		qeth_free_out_buf(q->bufs[--i]);
	qdio_free_buffers(q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q);
err_qdio_bufs:
	kfree(q);
	return NULL;
}

static void qeth_tx_completion_timer(struct timer_list *timer)
{
	struct qeth_qdio_out_q *queue = from_timer(queue, timer, timer);

	napi_schedule(&queue->napi);
	QETH_TXQ_STAT_INC(queue, completion_timer);
}

static int qeth_alloc_qdio_queues(struct qeth_card *card)
{
	unsigned int i;

	QETH_CARD_TEXT(card, 2, "allcqdbf");
>>>>>>> upstream/android-13

	if (atomic_cmpxchg(&card->qdio.state, QETH_QDIO_UNINITIALIZED,
		QETH_QDIO_ALLOCATED) != QETH_QDIO_UNINITIALIZED)
		return 0;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "inq");
=======
	QETH_CARD_TEXT(card, 2, "inq");
>>>>>>> upstream/android-13
	card->qdio.in_q = qeth_alloc_qdio_queue();
	if (!card->qdio.in_q)
		goto out_nomem;

	/* inbound buffer pool */
	if (qeth_alloc_buffer_pool(card))
		goto out_freeinq;

	/* outbound */
<<<<<<< HEAD
	card->qdio.out_qs =
		kcalloc(card->qdio.no_out_queues,
			sizeof(struct qeth_qdio_out_q *),
			GFP_KERNEL);
	if (!card->qdio.out_qs)
		goto out_freepool;
	for (i = 0; i < card->qdio.no_out_queues; ++i) {
		card->qdio.out_qs[i] = qeth_alloc_qdio_out_buf();
		if (!card->qdio.out_qs[i])
			goto out_freeoutq;
		QETH_DBF_TEXT_(SETUP, 2, "outq %i", i);
		QETH_DBF_HEX(SETUP, 2, &card->qdio.out_qs[i], sizeof(void *));
		card->qdio.out_qs[i]->queue_no = i;
		/* give outbound qeth_qdio_buffers their qdio_buffers */
		for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; ++j) {
			WARN_ON(card->qdio.out_qs[i]->bufs[j] != NULL);
			if (qeth_init_qdio_out_buf(card->qdio.out_qs[i], j))
				goto out_freeoutqbufs;
		}
=======
	for (i = 0; i < card->qdio.no_out_queues; ++i) {
		struct qeth_qdio_out_q *queue;

		queue = qeth_alloc_output_queue();
		if (!queue)
			goto out_freeoutq;
		QETH_CARD_TEXT_(card, 2, "outq %i", i);
		QETH_CARD_HEX(card, 2, &queue, sizeof(void *));
		card->qdio.out_qs[i] = queue;
		queue->card = card;
		queue->queue_no = i;
		INIT_LIST_HEAD(&queue->pending_bufs);
		spin_lock_init(&queue->lock);
		timer_setup(&queue->timer, qeth_tx_completion_timer, 0);
		if (IS_IQD(card)) {
			queue->coalesce_usecs = QETH_TX_COALESCE_USECS;
			queue->max_coalesced_frames = QETH_TX_MAX_COALESCED_FRAMES;
			queue->rescan_usecs = QETH_TX_TIMER_USECS;
		} else {
			queue->coalesce_usecs = USEC_PER_SEC;
			queue->max_coalesced_frames = 0;
			queue->rescan_usecs = 10 * USEC_PER_SEC;
		}
		queue->priority = QETH_QIB_PQUE_PRIO_DEFAULT;
>>>>>>> upstream/android-13
	}

	/* completion */
	if (qeth_alloc_cq(card))
		goto out_freeoutq;

	return 0;

<<<<<<< HEAD
out_freeoutqbufs:
	while (j > 0) {
		--j;
		kmem_cache_free(qeth_qdio_outbuf_cache,
				card->qdio.out_qs[i]->bufs[j]);
		card->qdio.out_qs[i]->bufs[j] = NULL;
	}
out_freeoutq:
	while (i > 0)
		qeth_free_output_queue(card->qdio.out_qs[--i]);
	kfree(card->qdio.out_qs);
	card->qdio.out_qs = NULL;
out_freepool:
=======
out_freeoutq:
	while (i > 0) {
		qeth_free_output_queue(card->qdio.out_qs[--i]);
		card->qdio.out_qs[i] = NULL;
	}
>>>>>>> upstream/android-13
	qeth_free_buffer_pool(card);
out_freeinq:
	qeth_free_qdio_queue(card->qdio.in_q);
	card->qdio.in_q = NULL;
out_nomem:
	atomic_set(&card->qdio.state, QETH_QDIO_UNINITIALIZED);
	return -ENOMEM;
}

<<<<<<< HEAD
static void qeth_free_qdio_buffers(struct qeth_card *card)
=======
static void qeth_free_qdio_queues(struct qeth_card *card)
>>>>>>> upstream/android-13
{
	int i, j;

	if (atomic_xchg(&card->qdio.state, QETH_QDIO_UNINITIALIZED) ==
		QETH_QDIO_UNINITIALIZED)
		return;

	qeth_free_cq(card);
<<<<<<< HEAD
	cancel_delayed_work_sync(&card->buffer_reclaim_work);
=======
>>>>>>> upstream/android-13
	for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; ++j) {
		if (card->qdio.in_q->bufs[j].rx_skb)
			dev_kfree_skb_any(card->qdio.in_q->bufs[j].rx_skb);
	}
	qeth_free_qdio_queue(card->qdio.in_q);
	card->qdio.in_q = NULL;
	/* inbound buffer pool */
	qeth_free_buffer_pool(card);
	/* free outbound qdio_qs */
<<<<<<< HEAD
	if (card->qdio.out_qs) {
		for (i = 0; i < card->qdio.no_out_queues; i++)
			qeth_free_output_queue(card->qdio.out_qs[i]);
		kfree(card->qdio.out_qs);
		card->qdio.out_qs = NULL;
	}
}

static void qeth_create_qib_param_field(struct qeth_card *card,
		char *param_field)
{

	param_field[0] = _ascebc['P'];
	param_field[1] = _ascebc['C'];
	param_field[2] = _ascebc['I'];
	param_field[3] = _ascebc['T'];
	*((unsigned int *) (&param_field[4])) = QETH_PCI_THRESHOLD_A(card);
	*((unsigned int *) (&param_field[8])) = QETH_PCI_THRESHOLD_B(card);
	*((unsigned int *) (&param_field[12])) = QETH_PCI_TIMER_VALUE(card);
}

static void qeth_create_qib_param_field_blkt(struct qeth_card *card,
		char *param_field)
{
	param_field[16] = _ascebc['B'];
	param_field[17] = _ascebc['L'];
	param_field[18] = _ascebc['K'];
	param_field[19] = _ascebc['T'];
	*((unsigned int *) (&param_field[20])) = card->info.blkt.time_total;
	*((unsigned int *) (&param_field[24])) = card->info.blkt.inter_packet;
	*((unsigned int *) (&param_field[28])) =
		card->info.blkt.inter_packet_jumbo;
=======
	for (i = 0; i < card->qdio.no_out_queues; i++) {
		qeth_free_output_queue(card->qdio.out_qs[i]);
		card->qdio.out_qs[i] = NULL;
	}
}

static void qeth_fill_qib_parms(struct qeth_card *card,
				struct qeth_qib_parms *parms)
{
	struct qeth_qdio_out_q *queue;
	unsigned int i;

	parms->pcit_magic[0] = 'P';
	parms->pcit_magic[1] = 'C';
	parms->pcit_magic[2] = 'I';
	parms->pcit_magic[3] = 'T';
	ASCEBC(parms->pcit_magic, sizeof(parms->pcit_magic));
	parms->pcit_a = QETH_PCI_THRESHOLD_A(card);
	parms->pcit_b = QETH_PCI_THRESHOLD_B(card);
	parms->pcit_c = QETH_PCI_TIMER_VALUE(card);

	parms->blkt_magic[0] = 'B';
	parms->blkt_magic[1] = 'L';
	parms->blkt_magic[2] = 'K';
	parms->blkt_magic[3] = 'T';
	ASCEBC(parms->blkt_magic, sizeof(parms->blkt_magic));
	parms->blkt_total = card->info.blkt.time_total;
	parms->blkt_inter_packet = card->info.blkt.inter_packet;
	parms->blkt_inter_packet_jumbo = card->info.blkt.inter_packet_jumbo;

	/* Prio-queueing implicitly uses the default priorities: */
	if (qeth_uses_tx_prio_queueing(card) || card->qdio.no_out_queues == 1)
		return;

	parms->pque_magic[0] = 'P';
	parms->pque_magic[1] = 'Q';
	parms->pque_magic[2] = 'U';
	parms->pque_magic[3] = 'E';
	ASCEBC(parms->pque_magic, sizeof(parms->pque_magic));
	parms->pque_order = QETH_QIB_PQUE_ORDER_RR;
	parms->pque_units = QETH_QIB_PQUE_UNITS_SBAL;

	qeth_for_each_output_queue(card, queue, i)
		parms->pque_priority[i] = queue->priority;
>>>>>>> upstream/android-13
}

static int qeth_qdio_activate(struct qeth_card *card)
{
<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 3, "qdioact");
=======
	QETH_CARD_TEXT(card, 3, "qdioact");
>>>>>>> upstream/android-13
	return qdio_activate(CARD_DDEV(card));
}

static int qeth_dm_act(struct qeth_card *card)
{
<<<<<<< HEAD
	int rc;
	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "dmact");

	iob = qeth_wait_for_buffer(&card->write);
	memcpy(iob->data, DM_ACT, DM_ACT_SIZE);
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "dmact");

	iob = qeth_mpc_alloc_cmd(card, DM_ACT, DM_ACT_SIZE);
	if (!iob)
		return -ENOMEM;
>>>>>>> upstream/android-13

	memcpy(QETH_DM_ACT_DEST_ADDR(iob->data),
	       &card->token.cm_connection_r, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_DM_ACT_CONNECTION_TOKEN(iob->data),
	       &card->token.ulp_connection_r, QETH_MPC_TOKEN_LENGTH);
<<<<<<< HEAD
	rc = qeth_send_control_data(card, DM_ACT_SIZE, iob, NULL, NULL);
	return rc;
=======
	return qeth_send_control_data(card, iob, NULL, NULL);
>>>>>>> upstream/android-13
}

static int qeth_mpc_initialize(struct qeth_card *card)
{
	int rc;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "mpcinit");

	rc = qeth_issue_next_read(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
=======
	QETH_CARD_TEXT(card, 2, "mpcinit");

	rc = qeth_issue_next_read(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "1err%d", rc);
>>>>>>> upstream/android-13
		return rc;
	}
	rc = qeth_cm_enable(card);
	if (rc) {
<<<<<<< HEAD
		QETH_DBF_TEXT_(SETUP, 2, "2err%d", rc);
		goto out_qdio;
	}
	rc = qeth_cm_setup(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "3err%d", rc);
		goto out_qdio;
	}
	rc = qeth_ulp_enable(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "4err%d", rc);
		goto out_qdio;
	}
	rc = qeth_ulp_setup(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "5err%d", rc);
		goto out_qdio;
	}
	rc = qeth_alloc_qdio_buffers(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "5err%d", rc);
		goto out_qdio;
	}
	rc = qeth_qdio_establish(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "6err%d", rc);
		qeth_free_qdio_buffers(card);
		goto out_qdio;
	}
	rc = qeth_qdio_activate(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "7err%d", rc);
		goto out_qdio;
	}
	rc = qeth_dm_act(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "8err%d", rc);
		goto out_qdio;
	}

	return 0;
out_qdio:
	qeth_qdio_clear_card(card, card->info.type != QETH_CARD_TYPE_IQD);
	qdio_free(CARD_DDEV(card));
	return rc;
}

void qeth_print_status_message(struct qeth_card *card)
=======
		QETH_CARD_TEXT_(card, 2, "2err%d", rc);
		return rc;
	}
	rc = qeth_cm_setup(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "3err%d", rc);
		return rc;
	}
	rc = qeth_ulp_enable(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "4err%d", rc);
		return rc;
	}
	rc = qeth_ulp_setup(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "5err%d", rc);
		return rc;
	}
	rc = qeth_alloc_qdio_queues(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "5err%d", rc);
		return rc;
	}
	rc = qeth_qdio_establish(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "6err%d", rc);
		qeth_free_qdio_queues(card);
		return rc;
	}
	rc = qeth_qdio_activate(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "7err%d", rc);
		return rc;
	}
	rc = qeth_dm_act(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "8err%d", rc);
		return rc;
	}

	return 0;
}

static void qeth_print_status_message(struct qeth_card *card)
>>>>>>> upstream/android-13
{
	switch (card->info.type) {
	case QETH_CARD_TYPE_OSD:
	case QETH_CARD_TYPE_OSM:
	case QETH_CARD_TYPE_OSX:
		/* VM will use a non-zero first character
		 * to indicate a HiperSockets like reporting
		 * of the level OSA sets the first character to zero
		 * */
		if (!card->info.mcl_level[0]) {
			sprintf(card->info.mcl_level, "%02x%02x",
				card->info.mcl_level[2],
				card->info.mcl_level[3]);
			break;
		}
<<<<<<< HEAD
		/* fallthrough */
	case QETH_CARD_TYPE_IQD:
		if ((card->info.guestlan) ||
		    (card->info.mcl_level[0] & 0x80)) {
=======
		fallthrough;
	case QETH_CARD_TYPE_IQD:
		if (IS_VM_NIC(card) || (card->info.mcl_level[0] & 0x80)) {
>>>>>>> upstream/android-13
			card->info.mcl_level[0] = (char) _ebcasc[(__u8)
				card->info.mcl_level[0]];
			card->info.mcl_level[1] = (char) _ebcasc[(__u8)
				card->info.mcl_level[1]];
			card->info.mcl_level[2] = (char) _ebcasc[(__u8)
				card->info.mcl_level[2]];
			card->info.mcl_level[3] = (char) _ebcasc[(__u8)
				card->info.mcl_level[3]];
			card->info.mcl_level[QETH_MCL_LENGTH] = 0;
		}
		break;
	default:
		memset(&card->info.mcl_level[0], 0, QETH_MCL_LENGTH + 1);
	}
	dev_info(&card->gdev->dev,
		 "Device is a%s card%s%s%s\nwith link type %s.\n",
		 qeth_get_cardname(card),
		 (card->info.mcl_level[0]) ? " (level: " : "",
		 (card->info.mcl_level[0]) ? card->info.mcl_level : "",
		 (card->info.mcl_level[0]) ? ")" : "",
		 qeth_get_cardname_short(card));
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_print_status_message);
=======
>>>>>>> upstream/android-13

static void qeth_initialize_working_pool_list(struct qeth_card *card)
{
	struct qeth_buffer_pool_entry *entry;

	QETH_CARD_TEXT(card, 5, "inwrklst");

	list_for_each_entry(entry,
			    &card->qdio.init_pool.entry_list, init_list) {
		qeth_put_buffer_pool_entry(card, entry);
	}
}

static struct qeth_buffer_pool_entry *qeth_find_free_buffer_pool_entry(
					struct qeth_card *card)
{
<<<<<<< HEAD
	struct list_head *plh;
	struct qeth_buffer_pool_entry *entry;
	int i, free;
	struct page *page;
=======
	struct qeth_buffer_pool_entry *entry;
	int i, free;
>>>>>>> upstream/android-13

	if (list_empty(&card->qdio.in_buf_pool.entry_list))
		return NULL;

<<<<<<< HEAD
	list_for_each(plh, &card->qdio.in_buf_pool.entry_list) {
		entry = list_entry(plh, struct qeth_buffer_pool_entry, list);
		free = 1;
		for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i) {
			if (page_count(virt_to_page(entry->elements[i])) > 1) {
=======
	list_for_each_entry(entry, &card->qdio.in_buf_pool.entry_list, list) {
		free = 1;
		for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i) {
			if (page_count(entry->elements[i]) > 1) {
>>>>>>> upstream/android-13
				free = 0;
				break;
			}
		}
		if (free) {
			list_del_init(&entry->list);
			return entry;
		}
	}

	/* no free buffer in pool so take first one and swap pages */
<<<<<<< HEAD
	entry = list_entry(card->qdio.in_buf_pool.entry_list.next,
			struct qeth_buffer_pool_entry, list);
	for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i) {
		if (page_count(virt_to_page(entry->elements[i])) > 1) {
			page = alloc_page(GFP_ATOMIC);
			if (!page) {
				return NULL;
			} else {
				free_page((unsigned long)entry->elements[i]);
				entry->elements[i] = page_address(page);
				if (card->options.performance_stats)
					card->perf_stats.sg_alloc_page_rx++;
			}
=======
	entry = list_first_entry(&card->qdio.in_buf_pool.entry_list,
				 struct qeth_buffer_pool_entry, list);
	for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i) {
		if (page_count(entry->elements[i]) > 1) {
			struct page *page = dev_alloc_page();

			if (!page)
				return NULL;

			__free_page(entry->elements[i]);
			entry->elements[i] = page;
			QETH_CARD_STAT_INC(card, rx_sg_alloc_page);
>>>>>>> upstream/android-13
		}
	}
	list_del_init(&entry->list);
	return entry;
}

static int qeth_init_input_buffer(struct qeth_card *card,
		struct qeth_qdio_buffer *buf)
{
<<<<<<< HEAD
	struct qeth_buffer_pool_entry *pool_entry;
=======
	struct qeth_buffer_pool_entry *pool_entry = buf->pool_entry;
>>>>>>> upstream/android-13
	int i;

	if ((card->options.cq == QETH_CQ_ENABLED) && (!buf->rx_skb)) {
		buf->rx_skb = netdev_alloc_skb(card->dev,
<<<<<<< HEAD
					       QETH_RX_PULL_LEN + ETH_HLEN);
=======
					       ETH_HLEN +
					       sizeof(struct ipv6hdr));
>>>>>>> upstream/android-13
		if (!buf->rx_skb)
			return -ENOMEM;
	}

<<<<<<< HEAD
	pool_entry = qeth_find_free_buffer_pool_entry(card);
	if (!pool_entry)
		return -ENOBUFS;
=======
	if (!pool_entry) {
		pool_entry = qeth_find_free_buffer_pool_entry(card);
		if (!pool_entry)
			return -ENOBUFS;

		buf->pool_entry = pool_entry;
	}
>>>>>>> upstream/android-13

	/*
	 * since the buffer is accessed only from the input_tasklet
	 * there shouldn't be a need to synchronize; also, since we use
	 * the QETH_IN_BUF_REQUEUE_THRESHOLD we should never run  out off
	 * buffers
	 */
<<<<<<< HEAD

	buf->pool_entry = pool_entry;
	for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i) {
		buf->buffer->element[i].length = PAGE_SIZE;
		buf->buffer->element[i].addr =  pool_entry->elements[i];
=======
	for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(card); ++i) {
		buf->buffer->element[i].length = PAGE_SIZE;
		buf->buffer->element[i].addr =
			page_to_phys(pool_entry->elements[i]);
>>>>>>> upstream/android-13
		if (i == QETH_MAX_BUFFER_ELEMENTS(card) - 1)
			buf->buffer->element[i].eflags = SBAL_EFLAGS_LAST_ENTRY;
		else
			buf->buffer->element[i].eflags = 0;
		buf->buffer->element[i].sflags = 0;
	}
	return 0;
}

<<<<<<< HEAD
int qeth_init_qdio_queues(struct qeth_card *card)
{
	int i, j;
	int rc;

	QETH_DBF_TEXT(SETUP, 2, "initqdqs");
=======
static unsigned int qeth_tx_select_bulk_max(struct qeth_card *card,
					    struct qeth_qdio_out_q *queue)
{
	if (!IS_IQD(card) ||
	    qeth_iqd_is_mcast_queue(card, queue) ||
	    card->options.cq == QETH_CQ_ENABLED ||
	    qdio_get_ssqd_desc(CARD_DDEV(card), &card->ssqd))
		return 1;

	return card->ssqd.mmwc ? card->ssqd.mmwc : 1;
}

static int qeth_init_qdio_queues(struct qeth_card *card)
{
	unsigned int rx_bufs = card->qdio.in_buf_pool.buf_count;
	unsigned int i;
	int rc;

	QETH_CARD_TEXT(card, 2, "initqdqs");
>>>>>>> upstream/android-13

	/* inbound queue */
	qdio_reset_buffers(card->qdio.in_q->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q);
	memset(&card->rx, 0, sizeof(struct qeth_rx));

	qeth_initialize_working_pool_list(card);
	/*give only as many buffers to hardware as we have buffer pool entries*/
<<<<<<< HEAD
	for (i = 0; i < card->qdio.in_buf_pool.buf_count - 1; i++) {
=======
	for (i = 0; i < rx_bufs; i++) {
>>>>>>> upstream/android-13
		rc = qeth_init_input_buffer(card, &card->qdio.in_q->bufs[i]);
		if (rc)
			return rc;
	}

<<<<<<< HEAD
	card->qdio.in_q->next_buf_to_init =
		card->qdio.in_buf_pool.buf_count - 1;
	rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT, 0, 0,
		     card->qdio.in_buf_pool.buf_count - 1);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
=======
	card->qdio.in_q->next_buf_to_init = QDIO_BUFNR(rx_bufs);
	rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT, 0, 0, rx_bufs,
		     NULL);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "1err%d", rc);
>>>>>>> upstream/android-13
		return rc;
	}

	/* completion */
	rc = qeth_cq_init(card);
	if (rc) {
		return rc;
	}

	/* outbound queue */
	for (i = 0; i < card->qdio.no_out_queues; ++i) {
<<<<<<< HEAD
		qdio_reset_buffers(card->qdio.out_qs[i]->qdio_bufs,
				   QDIO_MAX_BUFFERS_PER_Q);
		for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; ++j) {
			qeth_clear_output_buffer(card->qdio.out_qs[i],
						 card->qdio.out_qs[i]->bufs[j]);
		}
		card->qdio.out_qs[i]->card = card;
		card->qdio.out_qs[i]->next_buf_to_fill = 0;
		card->qdio.out_qs[i]->do_pack = 0;
		atomic_set(&card->qdio.out_qs[i]->used_buffers, 0);
		atomic_set(&card->qdio.out_qs[i]->set_pci_flags_count, 0);
		atomic_set(&card->qdio.out_qs[i]->state,
			   QETH_OUT_Q_UNLOCKED);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(qeth_init_qdio_queues);

static __u8 qeth_get_ipa_adp_type(enum qeth_link_types link_type)
{
	switch (link_type) {
	case QETH_LINK_TYPE_HSTR:
		return 2;
	default:
		return 1;
	}
}

static void qeth_fill_ipacmd_header(struct qeth_card *card,
		struct qeth_ipa_cmd *cmd, __u8 command,
		enum qeth_prot_versions prot)
{
	memset(cmd, 0, sizeof(struct qeth_ipa_cmd));
	cmd->hdr.command = command;
	cmd->hdr.initiator = IPA_CMD_INITIATOR_HOST;
	/* cmd->hdr.seqno is set by qeth_send_control_data() */
	cmd->hdr.adapter_type = qeth_get_ipa_adp_type(card->info.link_type);
	cmd->hdr.rel_adapter_no = (u8) card->dev->dev_port;
	if (card->options.layer2)
		cmd->hdr.prim_version_no = 2;
	else
		cmd->hdr.prim_version_no = 1;
	cmd->hdr.param_count = 1;
	cmd->hdr.prot_version = prot;
	cmd->hdr.ipa_supported = 0;
	cmd->hdr.ipa_enabled = 0;
}

struct qeth_cmd_buffer *qeth_get_ipacmd_buffer(struct qeth_card *card,
		enum qeth_ipa_cmds ipacmd, enum qeth_prot_versions prot)
{
	struct qeth_cmd_buffer *iob;

	iob = qeth_get_buffer(&card->write);
	if (iob) {
		qeth_fill_ipacmd_header(card, __ipa_cmd(iob), ipacmd, prot);
	} else {
		dev_warn(&card->gdev->dev,
			 "The qeth driver ran out of channel command buffers\n");
		QETH_DBF_MESSAGE(1, "%s The qeth driver ran out of channel command buffers",
				 dev_name(&card->gdev->dev));
	}

	return iob;
}
EXPORT_SYMBOL_GPL(qeth_get_ipacmd_buffer);

void qeth_prepare_ipa_cmd(struct qeth_card *card, struct qeth_cmd_buffer *iob)
{
	u8 prot_type = qeth_mpc_select_prot_type(card);

	memcpy(iob->data, IPA_PDU_HEADER, IPA_PDU_HEADER_SIZE);
	memcpy(QETH_IPA_CMD_PROT_TYPE(iob->data), &prot_type, 1);
	memcpy(QETH_IPA_CMD_DEST_ADDR(iob->data),
	       &card->token.ulp_connection_r, QETH_MPC_TOKEN_LENGTH);
}
EXPORT_SYMBOL_GPL(qeth_prepare_ipa_cmd);
=======
		struct qeth_qdio_out_q *queue = card->qdio.out_qs[i];

		qdio_reset_buffers(queue->qdio_bufs, QDIO_MAX_BUFFERS_PER_Q);
		queue->max_elements = QETH_MAX_BUFFER_ELEMENTS(card);
		queue->next_buf_to_fill = 0;
		queue->do_pack = 0;
		queue->prev_hdr = NULL;
		queue->coalesced_frames = 0;
		queue->bulk_start = 0;
		queue->bulk_count = 0;
		queue->bulk_max = qeth_tx_select_bulk_max(card, queue);
		atomic_set(&queue->used_buffers, 0);
		atomic_set(&queue->set_pci_flags_count, 0);
		netdev_tx_reset_queue(netdev_get_tx_queue(card->dev, i));
	}
	return 0;
}

static void qeth_ipa_finalize_cmd(struct qeth_card *card,
				  struct qeth_cmd_buffer *iob)
{
	qeth_mpc_finalize_cmd(card, iob);

	/* override with IPA-specific values: */
	__ipa_cmd(iob)->hdr.seqno = card->seqno.ipa++;
}

static void qeth_prepare_ipa_cmd(struct qeth_card *card,
				 struct qeth_cmd_buffer *iob, u16 cmd_length)
{
	u8 prot_type = qeth_mpc_select_prot_type(card);
	u16 total_length = iob->length;

	qeth_setup_ccw(__ccw_from_cmd(iob), CCW_CMD_WRITE, 0, total_length,
		       iob->data);
	iob->finalize = qeth_ipa_finalize_cmd;

	memcpy(iob->data, IPA_PDU_HEADER, IPA_PDU_HEADER_SIZE);
	memcpy(QETH_IPA_PDU_LEN_TOTAL(iob->data), &total_length, 2);
	memcpy(QETH_IPA_CMD_PROT_TYPE(iob->data), &prot_type, 1);
	memcpy(QETH_IPA_PDU_LEN_PDU1(iob->data), &cmd_length, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU2(iob->data), &cmd_length, 2);
	memcpy(QETH_IPA_CMD_DEST_ADDR(iob->data),
	       &card->token.ulp_connection_r, QETH_MPC_TOKEN_LENGTH);
	memcpy(QETH_IPA_PDU_LEN_PDU3(iob->data), &cmd_length, 2);
}

static bool qeth_ipa_match_reply(struct qeth_cmd_buffer *iob,
				 struct qeth_cmd_buffer *reply)
{
	struct qeth_ipa_cmd *ipa_reply = __ipa_reply(reply);

	return ipa_reply && (__ipa_cmd(iob)->hdr.seqno == ipa_reply->hdr.seqno);
}

struct qeth_cmd_buffer *qeth_ipa_alloc_cmd(struct qeth_card *card,
					   enum qeth_ipa_cmds cmd_code,
					   enum qeth_prot_versions prot,
					   unsigned int data_length)
{
	struct qeth_cmd_buffer *iob;
	struct qeth_ipacmd_hdr *hdr;

	data_length += offsetof(struct qeth_ipa_cmd, data);
	iob = qeth_alloc_cmd(&card->write, IPA_PDU_HEADER_SIZE + data_length, 1,
			     QETH_IPA_TIMEOUT);
	if (!iob)
		return NULL;

	qeth_prepare_ipa_cmd(card, iob, data_length);
	iob->match = qeth_ipa_match_reply;

	hdr = &__ipa_cmd(iob)->hdr;
	hdr->command = cmd_code;
	hdr->initiator = IPA_CMD_INITIATOR_HOST;
	/* hdr->seqno is set by qeth_send_control_data() */
	hdr->adapter_type = QETH_LINK_TYPE_FAST_ETH;
	hdr->rel_adapter_no = (u8) card->dev->dev_port;
	hdr->prim_version_no = IS_LAYER2(card) ? 2 : 1;
	hdr->param_count = 1;
	hdr->prot_version = prot;
	return iob;
}
EXPORT_SYMBOL_GPL(qeth_ipa_alloc_cmd);

static int qeth_send_ipa_cmd_cb(struct qeth_card *card,
				struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;

	return (cmd->hdr.return_code) ? -EIO : 0;
}
>>>>>>> upstream/android-13

/**
 * qeth_send_ipa_cmd() - send an IPA command
 *
 * See qeth_send_control_data() for explanation of the arguments.
 */

int qeth_send_ipa_cmd(struct qeth_card *card, struct qeth_cmd_buffer *iob,
		int (*reply_cb)(struct qeth_card *, struct qeth_reply*,
			unsigned long),
		void *reply_param)
{
	int rc;

	QETH_CARD_TEXT(card, 4, "sendipa");
<<<<<<< HEAD
	qeth_prepare_ipa_cmd(card, iob);
	rc = qeth_send_control_data(card, IPA_CMD_LENGTH,
						iob, reply_cb, reply_param);
=======

	if (card->read_or_write_problem) {
		qeth_put_cmd(iob);
		return -EIO;
	}

	if (reply_cb == NULL)
		reply_cb = qeth_send_ipa_cmd_cb;
	rc = qeth_send_control_data(card, iob, reply_cb, reply_param);
>>>>>>> upstream/android-13
	if (rc == -ETIME) {
		qeth_clear_ipacmd_list(card);
		qeth_schedule_recovery(card);
	}
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_send_ipa_cmd);

<<<<<<< HEAD
static int qeth_send_startlan(struct qeth_card *card)
{
	int rc;
	struct qeth_cmd_buffer *iob;

	QETH_DBF_TEXT(SETUP, 2, "strtlan");

	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_STARTLAN, 0);
	if (!iob)
		return -ENOMEM;
	rc = qeth_send_ipa_cmd(card, iob, NULL, NULL);
	return rc;
=======
static int qeth_send_startlan_cb(struct qeth_card *card,
				 struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;

	if (cmd->hdr.return_code == IPA_RC_LAN_OFFLINE)
		return -ENETDOWN;

	return (cmd->hdr.return_code) ? -EIO : 0;
}

static int qeth_send_startlan(struct qeth_card *card)
{
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "strtlan");

	iob = qeth_ipa_alloc_cmd(card, IPA_CMD_STARTLAN, QETH_PROT_NONE, 0);
	if (!iob)
		return -ENOMEM;
	return qeth_send_ipa_cmd(card, iob, qeth_send_startlan_cb, NULL);
>>>>>>> upstream/android-13
}

static int qeth_setadpparms_inspect_rc(struct qeth_ipa_cmd *cmd)
{
	if (!cmd->hdr.return_code)
		cmd->hdr.return_code =
			cmd->data.setadapterparms.hdr.return_code;
	return cmd->hdr.return_code;
}

static int qeth_query_setadapterparms_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
<<<<<<< HEAD

	QETH_CARD_TEXT(card, 3, "quyadpcb");
	if (qeth_setadpparms_inspect_rc(cmd))
		return 0;

	if (cmd->data.setadapterparms.data.query_cmds_supp.lan_type & 0x7f) {
		card->info.link_type =
		      cmd->data.setadapterparms.data.query_cmds_supp.lan_type;
		QETH_DBF_TEXT_(SETUP, 2, "lnk %d", card->info.link_type);
	}
	card->options.adp.supported_funcs =
		cmd->data.setadapterparms.data.query_cmds_supp.supported_cmds;
=======
	struct qeth_query_cmds_supp *query_cmd;

	QETH_CARD_TEXT(card, 3, "quyadpcb");
	if (qeth_setadpparms_inspect_rc(cmd))
		return -EIO;

	query_cmd = &cmd->data.setadapterparms.data.query_cmds_supp;
	if (query_cmd->lan_type & 0x7f) {
		if (!qeth_is_supported_link_type(card, query_cmd->lan_type))
			return -EPROTONOSUPPORT;

		card->info.link_type = query_cmd->lan_type;
		QETH_CARD_TEXT_(card, 2, "lnk %d", card->info.link_type);
	}

	card->options.adp.supported = query_cmd->supported_cmds;
>>>>>>> upstream/android-13
	return 0;
}

static struct qeth_cmd_buffer *qeth_get_adapter_cmd(struct qeth_card *card,
<<<<<<< HEAD
		__u32 command, __u32 cmdlen)
{
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;

	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_SETADAPTERPARMS,
				     QETH_PROT_IPV4);
	if (iob) {
		cmd = __ipa_cmd(iob);
		cmd->data.setadapterparms.hdr.cmdlength = cmdlen;
		cmd->data.setadapterparms.hdr.command_code = command;
		cmd->data.setadapterparms.hdr.used_total = 1;
		cmd->data.setadapterparms.hdr.seq_no = 1;
	}

=======
						    enum qeth_ipa_setadp_cmd adp_cmd,
						    unsigned int data_length)
{
	struct qeth_ipacmd_setadpparms_hdr *hdr;
	struct qeth_cmd_buffer *iob;

	iob = qeth_ipa_alloc_cmd(card, IPA_CMD_SETADAPTERPARMS, QETH_PROT_IPV4,
				 data_length +
				 offsetof(struct qeth_ipacmd_setadpparms,
					  data));
	if (!iob)
		return NULL;

	hdr = &__ipa_cmd(iob)->data.setadapterparms.hdr;
	hdr->cmdlength = sizeof(*hdr) + data_length;
	hdr->command_code = adp_cmd;
	hdr->used_total = 1;
	hdr->seq_no = 1;
>>>>>>> upstream/android-13
	return iob;
}

static int qeth_query_setadapterparms(struct qeth_card *card)
{
	int rc;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 3, "queryadp");
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_COMMANDS_SUPPORTED,
<<<<<<< HEAD
				   sizeof(struct qeth_ipacmd_setadpparms));
=======
				   SETADP_DATA_SIZEOF(query_cmds_supp));
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	rc = qeth_send_ipa_cmd(card, iob, qeth_query_setadapterparms_cb, NULL);
	return rc;
}

static int qeth_query_ipassists_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "qipasscb");
=======
	QETH_CARD_TEXT(card, 2, "qipasscb");
>>>>>>> upstream/android-13

	cmd = (struct qeth_ipa_cmd *) data;

	switch (cmd->hdr.return_code) {
<<<<<<< HEAD
	case IPA_RC_NOTSUPP:
	case IPA_RC_L2_UNSUPPORTED_CMD:
		QETH_DBF_TEXT(SETUP, 2, "ipaunsup");
		card->options.ipa4.supported_funcs |= IPA_SETADAPTERPARMS;
		card->options.ipa6.supported_funcs |= IPA_SETADAPTERPARMS;
		return -0;
	default:
		if (cmd->hdr.return_code) {
			QETH_DBF_MESSAGE(1, "%s IPA_CMD_QIPASSIST: Unhandled "
						"rc=%d\n",
						dev_name(&card->gdev->dev),
						cmd->hdr.return_code);
			return 0;
		}
	}

	if (cmd->hdr.prot_version == QETH_PROT_IPV4) {
		card->options.ipa4.supported_funcs = cmd->hdr.ipa_supported;
		card->options.ipa4.enabled_funcs = cmd->hdr.ipa_enabled;
	} else if (cmd->hdr.prot_version == QETH_PROT_IPV6) {
		card->options.ipa6.supported_funcs = cmd->hdr.ipa_supported;
		card->options.ipa6.enabled_funcs = cmd->hdr.ipa_enabled;
	} else
		QETH_DBF_MESSAGE(1, "%s IPA_CMD_QIPASSIST: Flawed LIC detected"
					"\n", dev_name(&card->gdev->dev));
=======
	case IPA_RC_SUCCESS:
		break;
	case IPA_RC_NOTSUPP:
	case IPA_RC_L2_UNSUPPORTED_CMD:
		QETH_CARD_TEXT(card, 2, "ipaunsup");
		card->options.ipa4.supported |= IPA_SETADAPTERPARMS;
		card->options.ipa6.supported |= IPA_SETADAPTERPARMS;
		return -EOPNOTSUPP;
	default:
		QETH_DBF_MESSAGE(1, "IPA_CMD_QIPASSIST on device %x: Unhandled rc=%#x\n",
				 CARD_DEVID(card), cmd->hdr.return_code);
		return -EIO;
	}

	if (cmd->hdr.prot_version == QETH_PROT_IPV4)
		card->options.ipa4 = cmd->hdr.assists;
	else if (cmd->hdr.prot_version == QETH_PROT_IPV6)
		card->options.ipa6 = cmd->hdr.assists;
	else
		QETH_DBF_MESSAGE(1, "IPA_CMD_QIPASSIST on device %x: Flawed LIC detected\n",
				 CARD_DEVID(card));
>>>>>>> upstream/android-13
	return 0;
}

static int qeth_query_ipassists(struct qeth_card *card,
				enum qeth_prot_versions prot)
{
	int rc;
	struct qeth_cmd_buffer *iob;

<<<<<<< HEAD
	QETH_DBF_TEXT_(SETUP, 2, "qipassi%i", prot);
	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_QIPASSIST, prot);
=======
	QETH_CARD_TEXT_(card, 2, "qipassi%i", prot);
	iob = qeth_ipa_alloc_cmd(card, IPA_CMD_QIPASSIST, prot, 0);
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	rc = qeth_send_ipa_cmd(card, iob, qeth_query_ipassists_cb, NULL);
	return rc;
}

static int qeth_query_switch_attributes_cb(struct qeth_card *card,
				struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_query_switch_attributes *attrs;
	struct qeth_switch_info *sw_info;

	QETH_CARD_TEXT(card, 2, "qswiatcb");
	if (qeth_setadpparms_inspect_rc(cmd))
<<<<<<< HEAD
		return 0;
=======
		return -EIO;
>>>>>>> upstream/android-13

	sw_info = (struct qeth_switch_info *)reply->param;
	attrs = &cmd->data.setadapterparms.data.query_switch_attributes;
	sw_info->capabilities = attrs->capabilities;
	sw_info->settings = attrs->settings;
	QETH_CARD_TEXT_(card, 2, "%04x%04x", sw_info->capabilities,
			sw_info->settings);
	return 0;
}

int qeth_query_switch_attributes(struct qeth_card *card,
				 struct qeth_switch_info *sw_info)
{
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "qswiattr");
	if (!qeth_adp_supported(card, IPA_SETADP_QUERY_SWITCH_ATTRIBUTES))
		return -EOPNOTSUPP;
	if (!netif_carrier_ok(card->dev))
		return -ENOMEDIUM;
<<<<<<< HEAD
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_SWITCH_ATTRIBUTES,
				sizeof(struct qeth_ipacmd_setadpparms_hdr));
=======
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_SWITCH_ATTRIBUTES, 0);
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	return qeth_send_ipa_cmd(card, iob,
				qeth_query_switch_attributes_cb, sw_info);
}

<<<<<<< HEAD
static int qeth_query_setdiagass_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd;
	__u16 rc;

	cmd = (struct qeth_ipa_cmd *)data;
	rc = cmd->hdr.return_code;
	if (rc)
		QETH_CARD_TEXT_(card, 2, "diagq:%x", rc);
	else
		card->info.diagass_support = cmd->data.diagass.ext;
=======
struct qeth_cmd_buffer *qeth_get_diag_cmd(struct qeth_card *card,
					  enum qeth_diags_cmds sub_cmd,
					  unsigned int data_length)
{
	struct qeth_ipacmd_diagass *cmd;
	struct qeth_cmd_buffer *iob;

	iob = qeth_ipa_alloc_cmd(card, IPA_CMD_SET_DIAG_ASS, QETH_PROT_NONE,
				 DIAG_HDR_LEN + data_length);
	if (!iob)
		return NULL;

	cmd = &__ipa_cmd(iob)->data.diagass;
	cmd->subcmd_len = DIAG_SUB_HDR_LEN + data_length;
	cmd->subcmd = sub_cmd;
	return iob;
}
EXPORT_SYMBOL_GPL(qeth_get_diag_cmd);

static int qeth_query_setdiagass_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	u16 rc = cmd->hdr.return_code;

	if (rc) {
		QETH_CARD_TEXT_(card, 2, "diagq:%x", rc);
		return -EIO;
	}

	card->info.diagass_support = cmd->data.diagass.ext;
>>>>>>> upstream/android-13
	return 0;
}

static int qeth_query_setdiagass(struct qeth_card *card)
{
	struct qeth_cmd_buffer *iob;
<<<<<<< HEAD
	struct qeth_ipa_cmd    *cmd;

	QETH_DBF_TEXT(SETUP, 2, "qdiagass");
	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_SET_DIAG_ASS, 0);
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
	cmd->data.diagass.subcmd_len = 16;
	cmd->data.diagass.subcmd = QETH_DIAGS_CMD_QUERY;
=======

	QETH_CARD_TEXT(card, 2, "qdiagass");
	iob = qeth_get_diag_cmd(card, QETH_DIAGS_CMD_QUERY, 0);
	if (!iob)
		return -ENOMEM;
>>>>>>> upstream/android-13
	return qeth_send_ipa_cmd(card, iob, qeth_query_setdiagass_cb, NULL);
}

static void qeth_get_trap_id(struct qeth_card *card, struct qeth_trap_id *tid)
{
	unsigned long info = get_zeroed_page(GFP_KERNEL);
	struct sysinfo_2_2_2 *info222 = (struct sysinfo_2_2_2 *)info;
	struct sysinfo_3_2_2 *info322 = (struct sysinfo_3_2_2 *)info;
	struct ccw_dev_id ccwid;
	int level;

	tid->chpid = card->info.chpid;
	ccw_device_get_id(CARD_RDEV(card), &ccwid);
	tid->ssid = ccwid.ssid;
	tid->devno = ccwid.devno;
	if (!info)
		return;
	level = stsi(NULL, 0, 0, 0);
	if ((level >= 2) && (stsi(info222, 2, 2, 2) == 0))
		tid->lparnr = info222->lpar_number;
	if ((level >= 3) && (stsi(info322, 3, 2, 2) == 0)) {
		EBCASC(info322->vm[0].name, sizeof(info322->vm[0].name));
		memcpy(tid->vmname, info322->vm[0].name, sizeof(tid->vmname));
	}
	free_page(info);
<<<<<<< HEAD
	return;
=======
>>>>>>> upstream/android-13
}

static int qeth_hw_trap_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
<<<<<<< HEAD
	struct qeth_ipa_cmd *cmd;
	__u16 rc;

	cmd = (struct qeth_ipa_cmd *)data;
	rc = cmd->hdr.return_code;
	if (rc)
		QETH_CARD_TEXT_(card, 2, "trapc:%x", rc);
=======
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	u16 rc = cmd->hdr.return_code;

	if (rc) {
		QETH_CARD_TEXT_(card, 2, "trapc:%x", rc);
		return -EIO;
	}
>>>>>>> upstream/android-13
	return 0;
}

int qeth_hw_trap(struct qeth_card *card, enum qeth_diags_trap_action action)
{
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "diagtrap");
	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_SET_DIAG_ASS, 0);
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
	cmd->data.diagass.subcmd_len = 80;
	cmd->data.diagass.subcmd = QETH_DIAGS_CMD_TRAP;
=======
	QETH_CARD_TEXT(card, 2, "diagtrap");
	iob = qeth_get_diag_cmd(card, QETH_DIAGS_CMD_TRAP, 64);
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
>>>>>>> upstream/android-13
	cmd->data.diagass.type = 1;
	cmd->data.diagass.action = action;
	switch (action) {
	case QETH_DIAGS_TRAP_ARM:
		cmd->data.diagass.options = 0x0003;
		cmd->data.diagass.ext = 0x00010000 +
			sizeof(struct qeth_trap_id);
		qeth_get_trap_id(card,
			(struct qeth_trap_id *)cmd->data.diagass.cdata);
		break;
	case QETH_DIAGS_TRAP_DISARM:
		cmd->data.diagass.options = 0x0001;
		break;
	case QETH_DIAGS_TRAP_CAPTURE:
		break;
	}
	return qeth_send_ipa_cmd(card, iob, qeth_hw_trap_cb, NULL);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_hw_trap);
=======
>>>>>>> upstream/android-13

static int qeth_check_qdio_errors(struct qeth_card *card,
				  struct qdio_buffer *buf,
				  unsigned int qdio_error,
				  const char *dbftext)
{
	if (qdio_error) {
		QETH_CARD_TEXT(card, 2, dbftext);
		QETH_CARD_TEXT_(card, 2, " F15=%02X",
			       buf->element[15].sflags);
		QETH_CARD_TEXT_(card, 2, " F14=%02X",
			       buf->element[14].sflags);
		QETH_CARD_TEXT_(card, 2, " qerr=%X", qdio_error);
		if ((buf->element[15].sflags) == 0x12) {
<<<<<<< HEAD
			card->stats.rx_dropped++;
=======
			QETH_CARD_STAT_INC(card, rx_fifo_errors);
>>>>>>> upstream/android-13
			return 0;
		} else
			return 1;
	}
	return 0;
}

<<<<<<< HEAD
static void qeth_queue_input_buffer(struct qeth_card *card, int index)
{
	struct qeth_qdio_q *queue = card->qdio.in_q;
	struct list_head *lh;
	int count;
=======
static unsigned int qeth_rx_refill_queue(struct qeth_card *card,
					 unsigned int count)
{
	struct qeth_qdio_q *queue = card->qdio.in_q;
	struct list_head *lh;
>>>>>>> upstream/android-13
	int i;
	int rc;
	int newcount = 0;

<<<<<<< HEAD
	count = (index < queue->next_buf_to_init)?
		card->qdio.in_buf_pool.buf_count -
		(queue->next_buf_to_init - index) :
		card->qdio.in_buf_pool.buf_count -
		(queue->next_buf_to_init + QDIO_MAX_BUFFERS_PER_Q - index);
=======
>>>>>>> upstream/android-13
	/* only requeue at a certain threshold to avoid SIGAs */
	if (count >= QETH_IN_BUF_REQUEUE_THRESHOLD(card)) {
		for (i = queue->next_buf_to_init;
		     i < queue->next_buf_to_init + count; ++i) {
			if (qeth_init_input_buffer(card,
<<<<<<< HEAD
				&queue->bufs[i % QDIO_MAX_BUFFERS_PER_Q])) {
=======
				&queue->bufs[QDIO_BUFNR(i)])) {
>>>>>>> upstream/android-13
				break;
			} else {
				newcount++;
			}
		}

		if (newcount < count) {
			/* we are in memory shortage so we switch back to
			   traditional skb allocation and drop packages */
			atomic_set(&card->force_alloc_skb, 3);
			count = newcount;
		} else {
			atomic_add_unless(&card->force_alloc_skb, -1, 0);
		}

		if (!count) {
			i = 0;
			list_for_each(lh, &card->qdio.in_buf_pool.entry_list)
				i++;
			if (i == card->qdio.in_buf_pool.buf_count) {
				QETH_CARD_TEXT(card, 2, "qsarbw");
<<<<<<< HEAD
				card->reclaim_index = index;
=======
>>>>>>> upstream/android-13
				schedule_delayed_work(
					&card->buffer_reclaim_work,
					QETH_RECLAIM_WORK_TIME);
			}
<<<<<<< HEAD
			return;
		}

		/*
		 * according to old code it should be avoided to requeue all
		 * 128 buffers in order to benefit from PCI avoidance.
		 * this function keeps at least one buffer (the buffer at
		 * 'index') un-requeued -> this buffer is the first buffer that
		 * will be requeued the next time
		 */
		if (card->options.performance_stats) {
			card->perf_stats.inbound_do_qdio_cnt++;
			card->perf_stats.inbound_do_qdio_start_time =
				qeth_get_micros();
		}
		rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT, 0,
			     queue->next_buf_to_init, count);
		if (card->options.performance_stats)
			card->perf_stats.inbound_do_qdio_time +=
				qeth_get_micros() -
				card->perf_stats.inbound_do_qdio_start_time;
		if (rc) {
			QETH_CARD_TEXT(card, 2, "qinberr");
		}
		queue->next_buf_to_init = (queue->next_buf_to_init + count) %
					  QDIO_MAX_BUFFERS_PER_Q;
	}
=======
			return 0;
		}

		rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT, 0,
			     queue->next_buf_to_init, count, NULL);
		if (rc) {
			QETH_CARD_TEXT(card, 2, "qinberr");
		}
		queue->next_buf_to_init = QDIO_BUFNR(queue->next_buf_to_init +
						     count);
		return count;
	}

	return 0;
>>>>>>> upstream/android-13
}

static void qeth_buffer_reclaim_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct qeth_card *card = container_of(work, struct qeth_card,
		buffer_reclaim_work.work);

	QETH_CARD_TEXT_(card, 2, "brw:%x", card->reclaim_index);
	qeth_queue_input_buffer(card, card->reclaim_index);
=======
	struct qeth_card *card = container_of(to_delayed_work(work),
					      struct qeth_card,
					      buffer_reclaim_work);

	local_bh_disable();
	napi_schedule(&card->napi);
	/* kick-start the NAPI softirq: */
	local_bh_enable();
>>>>>>> upstream/android-13
}

static void qeth_handle_send_error(struct qeth_card *card,
		struct qeth_qdio_out_buffer *buffer, unsigned int qdio_err)
{
	int sbalf15 = buffer->buffer->element[15].sflags;

	QETH_CARD_TEXT(card, 6, "hdsnderr");
<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_IQD) {
		if (sbalf15 == 0) {
			qdio_err = 0;
		} else {
			qdio_err = 1;
		}
	}
=======
>>>>>>> upstream/android-13
	qeth_check_qdio_errors(card, buffer->buffer, qdio_err, "qouterr");

	if (!qdio_err)
		return;

	if ((sbalf15 >= 15) && (sbalf15 <= 31))
		return;

	QETH_CARD_TEXT(card, 1, "lnkfail");
	QETH_CARD_TEXT_(card, 1, "%04x %02x",
		       (u16)qdio_err, (u8)sbalf15);
}

/**
 * qeth_prep_flush_pack_buffer - Prepares flushing of a packing buffer.
 * @queue: queue to check for packing buffer
 *
 * Returns number of buffers that were prepared for flush.
 */
static int qeth_prep_flush_pack_buffer(struct qeth_qdio_out_q *queue)
{
	struct qeth_qdio_out_buffer *buffer;

	buffer = queue->bufs[queue->next_buf_to_fill];
	if ((atomic_read(&buffer->state) == QETH_QDIO_BUF_EMPTY) &&
	    (buffer->next_element_to_fill > 0)) {
		/* it's a packing buffer */
		atomic_set(&buffer->state, QETH_QDIO_BUF_PRIMED);
		queue->next_buf_to_fill =
<<<<<<< HEAD
			(queue->next_buf_to_fill + 1) % QDIO_MAX_BUFFERS_PER_Q;
=======
			QDIO_BUFNR(queue->next_buf_to_fill + 1);
>>>>>>> upstream/android-13
		return 1;
	}
	return 0;
}

/*
 * Switched to packing state if the number of used buffers on a queue
 * reaches a certain limit.
 */
static void qeth_switch_to_packing_if_needed(struct qeth_qdio_out_q *queue)
{
	if (!queue->do_pack) {
		if (atomic_read(&queue->used_buffers)
		    >= QETH_HIGH_WATERMARK_PACK){
			/* switch non-PACKING -> PACKING */
			QETH_CARD_TEXT(queue->card, 6, "np->pack");
<<<<<<< HEAD
			if (queue->card->options.performance_stats)
				queue->card->perf_stats.sc_dp_p++;
=======
			QETH_TXQ_STAT_INC(queue, packing_mode_switch);
>>>>>>> upstream/android-13
			queue->do_pack = 1;
		}
	}
}

/*
 * Switches from packing to non-packing mode. If there is a packing
 * buffer on the queue this buffer will be prepared to be flushed.
 * In that case 1 is returned to inform the caller. If no buffer
 * has to be flushed, zero is returned.
 */
static int qeth_switch_to_nonpacking_if_needed(struct qeth_qdio_out_q *queue)
{
	if (queue->do_pack) {
		if (atomic_read(&queue->used_buffers)
		    <= QETH_LOW_WATERMARK_PACK) {
			/* switch PACKING -> non-PACKING */
			QETH_CARD_TEXT(queue->card, 6, "pack->np");
<<<<<<< HEAD
			if (queue->card->options.performance_stats)
				queue->card->perf_stats.sc_p_dp++;
=======
			QETH_TXQ_STAT_INC(queue, packing_mode_switch);
>>>>>>> upstream/android-13
			queue->do_pack = 0;
			return qeth_prep_flush_pack_buffer(queue);
		}
	}
	return 0;
}

static void qeth_flush_buffers(struct qeth_qdio_out_q *queue, int index,
			       int count)
{
<<<<<<< HEAD
	struct qeth_qdio_out_buffer *buf;
	int rc;
	int i;
	unsigned int qdio_flags;

	for (i = index; i < index + count; ++i) {
		int bidx = i % QDIO_MAX_BUFFERS_PER_Q;
		buf = queue->bufs[bidx];
		buf->buffer->element[buf->next_element_to_fill - 1].eflags |=
				SBAL_EFLAGS_LAST_ENTRY;

		if (queue->bufstates)
			queue->bufstates[bidx].user = buf;

		if (queue->card->info.type == QETH_CARD_TYPE_IQD)
			continue;

=======
	struct qeth_qdio_out_buffer *buf = queue->bufs[index];
	struct qeth_card *card = queue->card;
	unsigned int frames, usecs;
	struct qaob *aob = NULL;
	int rc;
	int i;

	for (i = index; i < index + count; ++i) {
		unsigned int bidx = QDIO_BUFNR(i);
		struct sk_buff *skb;

		buf = queue->bufs[bidx];
		buf->buffer->element[buf->next_element_to_fill - 1].eflags |=
				SBAL_EFLAGS_LAST_ENTRY;
		queue->coalesced_frames += buf->frames;

		if (IS_IQD(card)) {
			skb_queue_walk(&buf->skb_list, skb)
				skb_tx_timestamp(skb);
		}
	}

	if (IS_IQD(card)) {
		if (card->options.cq == QETH_CQ_ENABLED &&
		    !qeth_iqd_is_mcast_queue(card, queue) &&
		    count == 1) {
			if (!buf->aob)
				buf->aob = qdio_allocate_aob();
			if (buf->aob) {
				struct qeth_qaob_priv1 *priv;

				aob = buf->aob;
				priv = (struct qeth_qaob_priv1 *)&aob->user1;
				priv->state = QETH_QAOB_ISSUED;
				priv->queue_no = queue->queue_no;
			}
		}
	} else {
>>>>>>> upstream/android-13
		if (!queue->do_pack) {
			if ((atomic_read(&queue->used_buffers) >=
				(QETH_HIGH_WATERMARK_PACK -
				 QETH_WATERMARK_PACK_FUZZ)) &&
			    !atomic_read(&queue->set_pci_flags_count)) {
				/* it's likely that we'll go to packing
				 * mode soon */
				atomic_inc(&queue->set_pci_flags_count);
				buf->buffer->element[0].sflags |= SBAL_SFLAGS0_PCI_REQ;
			}
		} else {
			if (!atomic_read(&queue->set_pci_flags_count)) {
				/*
				 * there's no outstanding PCI any more, so we
				 * have to request a PCI to be sure the the PCI
				 * will wake at some time in the future then we
				 * can flush packed buffers that might still be
				 * hanging around, which can happen if no
				 * further send was requested by the stack
				 */
				atomic_inc(&queue->set_pci_flags_count);
				buf->buffer->element[0].sflags |= SBAL_SFLAGS0_PCI_REQ;
			}
		}
	}

<<<<<<< HEAD
	netif_trans_update(queue->card->dev);
	if (queue->card->options.performance_stats) {
		queue->card->perf_stats.outbound_do_qdio_cnt++;
		queue->card->perf_stats.outbound_do_qdio_start_time =
			qeth_get_micros();
	}
	qdio_flags = QDIO_FLAG_SYNC_OUTPUT;
	if (atomic_read(&queue->set_pci_flags_count))
		qdio_flags |= QDIO_FLAG_PCI_OUT;
	atomic_add(count, &queue->used_buffers);

	rc = do_QDIO(CARD_DDEV(queue->card), qdio_flags,
		     queue->queue_no, index, count);
	if (queue->card->options.performance_stats)
		queue->card->perf_stats.outbound_do_qdio_time +=
			qeth_get_micros() -
			queue->card->perf_stats.outbound_do_qdio_start_time;
	if (rc) {
		queue->card->stats.tx_errors += count;
		/* ignore temporary SIGA errors without busy condition */
		if (rc == -ENOBUFS)
			return;
=======
	QETH_TXQ_STAT_INC(queue, doorbell);
	rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_OUTPUT, queue->queue_no,
		     index, count, aob);

	switch (rc) {
	case 0:
	case -ENOBUFS:
		/* ignore temporary SIGA errors without busy condition */

		/* Fake the TX completion interrupt: */
		frames = READ_ONCE(queue->max_coalesced_frames);
		usecs = READ_ONCE(queue->coalesce_usecs);

		if (frames && queue->coalesced_frames >= frames) {
			napi_schedule(&queue->napi);
			queue->coalesced_frames = 0;
			QETH_TXQ_STAT_INC(queue, coal_frames);
		} else if (qeth_use_tx_irqs(card) &&
			   atomic_read(&queue->used_buffers) >= 32) {
			/* Old behaviour carried over from the qdio layer: */
			napi_schedule(&queue->napi);
			QETH_TXQ_STAT_INC(queue, coal_frames);
		} else if (usecs) {
			qeth_tx_arm_timer(queue, usecs);
		}

		break;
	default:
>>>>>>> upstream/android-13
		QETH_CARD_TEXT(queue->card, 2, "flushbuf");
		QETH_CARD_TEXT_(queue->card, 2, " q%d", queue->queue_no);
		QETH_CARD_TEXT_(queue->card, 2, " idx%d", index);
		QETH_CARD_TEXT_(queue->card, 2, " c%d", count);
		QETH_CARD_TEXT_(queue->card, 2, " err%d", rc);

		/* this must not happen under normal circumstances. if it
		 * happens something is really wrong -> recover */
		qeth_schedule_recovery(queue->card);
<<<<<<< HEAD
		return;
	}
	if (queue->card->options.performance_stats)
		queue->card->perf_stats.bufs_sent += count;
=======
	}
}

static void qeth_flush_queue(struct qeth_qdio_out_q *queue)
{
	qeth_flush_buffers(queue, queue->bulk_start, queue->bulk_count);

	queue->bulk_start = QDIO_BUFNR(queue->bulk_start + queue->bulk_count);
	queue->prev_hdr = NULL;
	queue->bulk_count = 0;
>>>>>>> upstream/android-13
}

static void qeth_check_outbound_queue(struct qeth_qdio_out_q *queue)
{
<<<<<<< HEAD
	int index;
	int flush_cnt = 0;
	int q_was_packing = 0;

=======
>>>>>>> upstream/android-13
	/*
	 * check if weed have to switch to non-packing mode or if
	 * we have to get a pci flag out on the queue
	 */
	if ((atomic_read(&queue->used_buffers) <= QETH_LOW_WATERMARK_PACK) ||
	    !atomic_read(&queue->set_pci_flags_count)) {
<<<<<<< HEAD
		if (atomic_xchg(&queue->state, QETH_OUT_Q_LOCKED_FLUSH) ==
				QETH_OUT_Q_UNLOCKED) {
			/*
			 * If we get in here, there was no action in
			 * do_send_packet. So, we check if there is a
			 * packing buffer to be flushed here.
			 */
			netif_stop_queue(queue->card->dev);
			index = queue->next_buf_to_fill;
			q_was_packing = queue->do_pack;
			/* queue->do_pack may change */
			barrier();
			flush_cnt += qeth_switch_to_nonpacking_if_needed(queue);
			if (!flush_cnt &&
			    !atomic_read(&queue->set_pci_flags_count))
				flush_cnt += qeth_prep_flush_pack_buffer(queue);
			if (queue->card->options.performance_stats &&
			    q_was_packing)
				queue->card->perf_stats.bufs_sent_pack +=
					flush_cnt;
			if (flush_cnt)
				qeth_flush_buffers(queue, index, flush_cnt);
			atomic_set(&queue->state, QETH_OUT_Q_UNLOCKED);
		}
	}
}

static void qeth_qdio_start_poll(struct ccw_device *ccwdev, int queue,
				 unsigned long card_ptr)
{
	struct qeth_card *card = (struct qeth_card *)card_ptr;

	if (card->dev->flags & IFF_UP)
		napi_schedule(&card->napi);
=======
		unsigned int index, flush_cnt;
		bool q_was_packing;

		spin_lock(&queue->lock);

		index = queue->next_buf_to_fill;
		q_was_packing = queue->do_pack;

		flush_cnt = qeth_switch_to_nonpacking_if_needed(queue);
		if (!flush_cnt && !atomic_read(&queue->set_pci_flags_count))
			flush_cnt = qeth_prep_flush_pack_buffer(queue);

		if (flush_cnt) {
			qeth_flush_buffers(queue, index, flush_cnt);
			if (q_was_packing)
				QETH_TXQ_STAT_ADD(queue, bufs_pack, flush_cnt);
		}

		spin_unlock(&queue->lock);
	}
}

static void qeth_qdio_poll(struct ccw_device *cdev, unsigned long card_ptr)
{
	struct qeth_card *card = (struct qeth_card *)card_ptr;

	napi_schedule_irqoff(&card->napi);
>>>>>>> upstream/android-13
}

int qeth_configure_cq(struct qeth_card *card, enum qeth_cq cq)
{
	int rc;

	if (card->options.cq ==  QETH_CQ_NOTAVAILABLE) {
		rc = -1;
		goto out;
	} else {
		if (card->options.cq == cq) {
			rc = 0;
			goto out;
		}

<<<<<<< HEAD
		if (card->state != CARD_STATE_DOWN &&
		    card->state != CARD_STATE_RECOVER) {
			rc = -1;
			goto out;
		}

		qeth_free_qdio_buffers(card);
=======
		qeth_free_qdio_queues(card);
>>>>>>> upstream/android-13
		card->options.cq = cq;
		rc = 0;
	}
out:
	return rc;

}
EXPORT_SYMBOL_GPL(qeth_configure_cq);

<<<<<<< HEAD
=======
static void qeth_qdio_handle_aob(struct qeth_card *card, struct qaob *aob)
{
	struct qeth_qaob_priv1 *priv = (struct qeth_qaob_priv1 *)&aob->user1;
	unsigned int queue_no = priv->queue_no;

	BUILD_BUG_ON(sizeof(*priv) > ARRAY_SIZE(aob->user1));

	if (xchg(&priv->state, QETH_QAOB_DONE) == QETH_QAOB_PENDING &&
	    queue_no < card->qdio.no_out_queues)
		napi_schedule(&card->qdio.out_qs[queue_no]->napi);
}

>>>>>>> upstream/android-13
static void qeth_qdio_cq_handler(struct qeth_card *card, unsigned int qdio_err,
				 unsigned int queue, int first_element,
				 int count)
{
	struct qeth_qdio_q *cq = card->qdio.c_q;
	int i;
	int rc;

<<<<<<< HEAD
	if (!qeth_is_cq(card, queue))
		goto out;

=======
>>>>>>> upstream/android-13
	QETH_CARD_TEXT_(card, 5, "qcqhe%d", first_element);
	QETH_CARD_TEXT_(card, 5, "qcqhc%d", count);
	QETH_CARD_TEXT_(card, 5, "qcqherr%d", qdio_err);

	if (qdio_err) {
<<<<<<< HEAD
		netif_stop_queue(card->dev);
		qeth_schedule_recovery(card);
		goto out;
	}

	if (card->options.performance_stats) {
		card->perf_stats.cq_cnt++;
		card->perf_stats.cq_start_time = qeth_get_micros();
	}

	for (i = first_element; i < first_element + count; ++i) {
		int bidx = i % QDIO_MAX_BUFFERS_PER_Q;
		struct qdio_buffer *buffer = cq->qdio_bufs[bidx];
=======
		netif_tx_stop_all_queues(card->dev);
		qeth_schedule_recovery(card);
		return;
	}

	for (i = first_element; i < first_element + count; ++i) {
		struct qdio_buffer *buffer = cq->qdio_bufs[QDIO_BUFNR(i)];
>>>>>>> upstream/android-13
		int e = 0;

		while ((e < QDIO_MAX_ELEMENTS_PER_BUFFER) &&
		       buffer->element[e].addr) {
<<<<<<< HEAD
			unsigned long phys_aob_addr;

			phys_aob_addr = (unsigned long) buffer->element[e].addr;
			qeth_qdio_handle_aob(card, phys_aob_addr);
=======
			unsigned long phys_aob_addr = buffer->element[e].addr;

			qeth_qdio_handle_aob(card, phys_to_virt(phys_aob_addr));
>>>>>>> upstream/android-13
			++e;
		}
		qeth_scrub_qdio_buffer(buffer, QDIO_MAX_ELEMENTS_PER_BUFFER);
	}
	rc = do_QDIO(CARD_DDEV(card), QDIO_FLAG_SYNC_INPUT, queue,
<<<<<<< HEAD
		    card->qdio.c_q->next_buf_to_init,
		    count);
=======
		     cq->next_buf_to_init, count, NULL);
>>>>>>> upstream/android-13
	if (rc) {
		dev_warn(&card->gdev->dev,
			"QDIO reported an error, rc=%i\n", rc);
		QETH_CARD_TEXT(card, 2, "qcqherr");
	}
<<<<<<< HEAD
	card->qdio.c_q->next_buf_to_init = (card->qdio.c_q->next_buf_to_init
				   + count) % QDIO_MAX_BUFFERS_PER_Q;

	netif_wake_queue(card->dev);

	if (card->options.performance_stats) {
		int delta_t = qeth_get_micros();
		delta_t -= card->perf_stats.cq_start_time;
		card->perf_stats.cq_time += delta_t;
	}
out:
	return;
=======

	cq->next_buf_to_init = QDIO_BUFNR(cq->next_buf_to_init + count);
>>>>>>> upstream/android-13
}

static void qeth_qdio_input_handler(struct ccw_device *ccwdev,
				    unsigned int qdio_err, int queue,
				    int first_elem, int count,
				    unsigned long card_ptr)
{
	struct qeth_card *card = (struct qeth_card *)card_ptr;

	QETH_CARD_TEXT_(card, 2, "qihq%d", queue);
	QETH_CARD_TEXT_(card, 2, "qiec%d", qdio_err);

<<<<<<< HEAD
	if (qeth_is_cq(card, queue))
		qeth_qdio_cq_handler(card, qdio_err, queue, first_elem, count);
	else if (qdio_err)
=======
	if (qdio_err)
>>>>>>> upstream/android-13
		qeth_schedule_recovery(card);
}

static void qeth_qdio_output_handler(struct ccw_device *ccwdev,
				     unsigned int qdio_error, int __queue,
				     int first_element, int count,
				     unsigned long card_ptr)
{
	struct qeth_card *card        = (struct qeth_card *) card_ptr;
<<<<<<< HEAD
	struct qeth_qdio_out_q *queue = card->qdio.out_qs[__queue];
	struct qeth_qdio_out_buffer *buffer;
	int i;

	QETH_CARD_TEXT(card, 6, "qdouhdl");
	if (qdio_error & QDIO_ERROR_FATAL) {
		QETH_CARD_TEXT(card, 2, "achkcond");
		netif_stop_queue(card->dev);
		qeth_schedule_recovery(card);
		return;
	}
	if (card->options.performance_stats) {
		card->perf_stats.outbound_handler_cnt++;
		card->perf_stats.outbound_handler_start_time =
			qeth_get_micros();
	}
	for (i = first_element; i < (first_element + count); ++i) {
		int bidx = i % QDIO_MAX_BUFFERS_PER_Q;
		buffer = queue->bufs[bidx];
		qeth_handle_send_error(card, buffer, qdio_error);

		if (queue->bufstates &&
		    (queue->bufstates[bidx].flags &
		     QDIO_OUTBUF_STATE_FLAG_PENDING) != 0) {
			WARN_ON_ONCE(card->options.cq != QETH_CQ_ENABLED);

			if (atomic_cmpxchg(&buffer->state,
					   QETH_QDIO_BUF_PRIMED,
					   QETH_QDIO_BUF_PENDING) ==
				QETH_QDIO_BUF_PRIMED) {
				qeth_notify_skbs(queue, buffer,
						 TX_NOTIFY_PENDING);
			}
			QETH_CARD_TEXT_(queue->card, 5, "pel%d", bidx);

			/* prepare the queue slot for re-use: */
			qeth_scrub_qdio_buffer(buffer->buffer,
					       QETH_MAX_BUFFER_ELEMENTS(card));
			if (qeth_init_qdio_out_buf(queue, bidx)) {
				QETH_CARD_TEXT(card, 2, "outofbuf");
				qeth_schedule_recovery(card);
			}
		} else {
			if (card->options.cq == QETH_CQ_ENABLED) {
				enum iucv_tx_notify n;

				n = qeth_compute_cq_notification(
					buffer->buffer->element[15].sflags, 0);
				qeth_notify_skbs(queue, buffer, n);
			}

			qeth_clear_output_buffer(queue, buffer);
		}
		qeth_cleanup_handled_pending(queue, bidx, 0);
	}
	atomic_sub(count, &queue->used_buffers);
	/* check if we need to do something on this outbound queue */
	if (card->info.type != QETH_CARD_TYPE_IQD)
		qeth_check_outbound_queue(queue);

	netif_wake_queue(queue->card->dev);
	if (card->options.performance_stats)
		card->perf_stats.outbound_handler_time += qeth_get_micros() -
			card->perf_stats.outbound_handler_start_time;
}

/* We cannot use outbound queue 3 for unicast packets on HiperSockets */
static inline int qeth_cut_iqd_prio(struct qeth_card *card, int queue_num)
{
	if ((card->info.type == QETH_CARD_TYPE_IQD) && (queue_num == 3))
		return 2;
	return queue_num;
=======

	QETH_CARD_TEXT(card, 2, "achkcond");
	netif_tx_stop_all_queues(card->dev);
	qeth_schedule_recovery(card);
>>>>>>> upstream/android-13
}

/**
 * Note: Function assumes that we have 4 outbound queues.
 */
<<<<<<< HEAD
int qeth_get_priority_queue(struct qeth_card *card, struct sk_buff *skb,
			    int ipv)
{
	__be16 *tci;
=======
int qeth_get_priority_queue(struct qeth_card *card, struct sk_buff *skb)
{
	struct vlan_ethhdr *veth = vlan_eth_hdr(skb);
>>>>>>> upstream/android-13
	u8 tos;

	switch (card->qdio.do_prio_queueing) {
	case QETH_PRIO_Q_ING_TOS:
	case QETH_PRIO_Q_ING_PREC:
<<<<<<< HEAD
		switch (ipv) {
		case 4:
			tos = ipv4_get_dsfield(ip_hdr(skb));
			break;
		case 6:
=======
		switch (vlan_get_protocol(skb)) {
		case htons(ETH_P_IP):
			tos = ipv4_get_dsfield(ip_hdr(skb));
			break;
		case htons(ETH_P_IPV6):
>>>>>>> upstream/android-13
			tos = ipv6_get_dsfield(ipv6_hdr(skb));
			break;
		default:
			return card->qdio.default_out_queue;
		}
		if (card->qdio.do_prio_queueing == QETH_PRIO_Q_ING_PREC)
<<<<<<< HEAD
			return qeth_cut_iqd_prio(card, ~tos >> 6 & 3);
		if (tos & IPTOS_MINCOST)
			return qeth_cut_iqd_prio(card, 3);
=======
			return ~tos >> 6 & 3;
		if (tos & IPTOS_MINCOST)
			return 3;
>>>>>>> upstream/android-13
		if (tos & IPTOS_RELIABILITY)
			return 2;
		if (tos & IPTOS_THROUGHPUT)
			return 1;
		if (tos & IPTOS_LOWDELAY)
			return 0;
		break;
	case QETH_PRIO_Q_ING_SKB:
		if (skb->priority > 5)
			return 0;
<<<<<<< HEAD
		return qeth_cut_iqd_prio(card, ~skb->priority >> 1 & 3);
	case QETH_PRIO_Q_ING_VLAN:
		tci = &((struct ethhdr *)skb->data)->h_proto;
		if (be16_to_cpu(*tci) == ETH_P_8021Q)
			return qeth_cut_iqd_prio(card,
			~be16_to_cpu(*(tci + 1)) >> (VLAN_PRIO_SHIFT + 1) & 3);
		break;
=======
		return ~skb->priority >> 1 & 3;
	case QETH_PRIO_Q_ING_VLAN:
		if (veth->h_vlan_proto == htons(ETH_P_8021Q))
			return ~ntohs(veth->h_vlan_TCI) >>
			       (VLAN_PRIO_SHIFT + 1) & 3;
		break;
	case QETH_PRIO_Q_ING_FIXED:
		return card->qdio.default_out_queue;
>>>>>>> upstream/android-13
	default:
		break;
	}
	return card->qdio.default_out_queue;
}
EXPORT_SYMBOL_GPL(qeth_get_priority_queue);

/**
 * qeth_get_elements_for_frags() -	find number of SBALEs for skb frags.
 * @skb:				SKB address
 *
 * Returns the number of pages, and thus QDIO buffer elements, needed to cover
 * fragmented part of the SKB. Returns zero for linear SKB.
 */
<<<<<<< HEAD
int qeth_get_elements_for_frags(struct sk_buff *skb)
=======
static int qeth_get_elements_for_frags(struct sk_buff *skb)
>>>>>>> upstream/android-13
{
	int cnt, elements = 0;

	for (cnt = 0; cnt < skb_shinfo(skb)->nr_frags; cnt++) {
<<<<<<< HEAD
		struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[cnt];
=======
		skb_frag_t *frag = &skb_shinfo(skb)->frags[cnt];
>>>>>>> upstream/android-13

		elements += qeth_get_elements_for_range(
			(addr_t)skb_frag_address(frag),
			(addr_t)skb_frag_address(frag) + skb_frag_size(frag));
	}
	return elements;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_get_elements_for_frags);

static unsigned int qeth_count_elements(struct sk_buff *skb, int data_offset)
=======

/**
 * qeth_count_elements() -	Counts the number of QDIO buffer elements needed
 *				to transmit an skb.
 * @skb:			the skb to operate on.
 * @data_offset:		skip this part of the skb's linear data
 *
 * Returns the number of pages, and thus QDIO buffer elements, needed to map the
 * skb's data (both its linear part and paged fragments).
 */
static unsigned int qeth_count_elements(struct sk_buff *skb,
					unsigned int data_offset)
>>>>>>> upstream/android-13
{
	unsigned int elements = qeth_get_elements_for_frags(skb);
	addr_t end = (addr_t)skb->data + skb_headlen(skb);
	addr_t start = (addr_t)skb->data + data_offset;

	if (start != end)
		elements += qeth_get_elements_for_range(start, end);
	return elements;
}

<<<<<<< HEAD
/**
 * qeth_get_elements_no() -	find number of SBALEs for skb data, inc. frags.
 * @card:			qeth card structure, to check max. elems.
 * @skb:			SKB address
 * @extra_elems:		extra elems needed, to check against max.
 * @data_offset:		range starts at skb->data + data_offset
 *
 * Returns the number of pages, and thus QDIO buffer elements, needed to cover
 * skb data, including linear part and fragments. Checks if the result plus
 * extra_elems fits under the limit for the card. Returns 0 if it does not.
 * Note: extra_elems is not included in the returned result.
 */
int qeth_get_elements_no(struct qeth_card *card,
		     struct sk_buff *skb, int extra_elems, int data_offset)
{
	int elements = qeth_count_elements(skb, data_offset);

	if ((elements + extra_elems) > QETH_MAX_BUFFER_ELEMENTS(card)) {
		QETH_DBF_MESSAGE(2, "Invalid size of IP packet "
			"(Number=%d / Length=%d). Discarded.\n",
			elements + extra_elems, skb->len);
		return 0;
	}
	return elements;
}
EXPORT_SYMBOL_GPL(qeth_get_elements_no);

int qeth_hdr_chk_and_bounce(struct sk_buff *skb, struct qeth_hdr **hdr, int len)
{
	int hroom, inpage, rest;

	if (((unsigned long)skb->data & PAGE_MASK) !=
	    (((unsigned long)skb->data + len - 1) & PAGE_MASK)) {
		hroom = skb_headroom(skb);
		inpage = PAGE_SIZE - ((unsigned long) skb->data % PAGE_SIZE);
		rest = len - inpage;
		if (rest > hroom)
			return 1;
		memmove(skb->data - rest, skb->data, skb_headlen(skb));
		skb->data -= rest;
		skb->tail -= rest;
		*hdr = (struct qeth_hdr *)skb->data;
		QETH_DBF_MESSAGE(2, "skb bounce len: %d rest: %d\n", len, rest);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(qeth_hdr_chk_and_bounce);
=======
#define QETH_HDR_CACHE_OBJ_SIZE		(sizeof(struct qeth_hdr_tso) + \
					 MAX_TCP_HEADER)
>>>>>>> upstream/android-13

/**
 * qeth_add_hw_header() - add a HW header to an skb.
 * @skb: skb that the HW header should be added to.
 * @hdr: double pointer to a qeth_hdr. When returning with >= 0,
 *	 it contains a valid pointer to a qeth_hdr.
 * @hdr_len: length of the HW header.
 * @proto_len: length of protocol headers that need to be in same page as the
 *	       HW header.
 *
 * Returns the pushed length. If the header can't be pushed on
 * (eg. because it would cross a page boundary), it is allocated from
 * the cache instead and 0 is returned.
 * The number of needed buffer elements is returned in @elements.
 * Error to create the hdr is indicated by returning with < 0.
 */
<<<<<<< HEAD
int qeth_add_hw_header(struct qeth_card *card, struct sk_buff *skb,
		       struct qeth_hdr **hdr, unsigned int hdr_len,
		       unsigned int proto_len, unsigned int *elements)
{
	const unsigned int max_elements = QETH_MAX_BUFFER_ELEMENTS(card);
	const unsigned int contiguous = proto_len ? proto_len : 1;
=======
static int qeth_add_hw_header(struct qeth_qdio_out_q *queue,
			      struct sk_buff *skb, struct qeth_hdr **hdr,
			      unsigned int hdr_len, unsigned int proto_len,
			      unsigned int *elements)
{
	gfp_t gfp = GFP_ATOMIC | (skb_pfmemalloc(skb) ? __GFP_MEMALLOC : 0);
	const unsigned int contiguous = proto_len ? proto_len : 1;
	const unsigned int max_elements = queue->max_elements;
>>>>>>> upstream/android-13
	unsigned int __elements;
	addr_t start, end;
	bool push_ok;
	int rc;

check_layout:
	start = (addr_t)skb->data - hdr_len;
	end = (addr_t)skb->data;

	if (qeth_get_elements_for_range(start, end + contiguous) == 1) {
		/* Push HW header into same page as first protocol header. */
		push_ok = true;
<<<<<<< HEAD
		__elements = qeth_count_elements(skb, 0);
	} else if (!proto_len && qeth_get_elements_for_range(start, end) == 1) {
		/* Push HW header into a new page. */
=======
		/* ... but TSO always needs a separate element for headers: */
		if (skb_is_gso(skb))
			__elements = 1 + qeth_count_elements(skb, proto_len);
		else
			__elements = qeth_count_elements(skb, 0);
	} else if (!proto_len && PAGE_ALIGNED(skb->data)) {
		/* Push HW header into preceding page, flush with skb->data. */
>>>>>>> upstream/android-13
		push_ok = true;
		__elements = 1 + qeth_count_elements(skb, 0);
	} else {
		/* Use header cache, copy protocol headers up. */
		push_ok = false;
		__elements = 1 + qeth_count_elements(skb, proto_len);
	}

	/* Compress skb to fit into one IO buffer: */
	if (__elements > max_elements) {
		if (!skb_is_nonlinear(skb)) {
			/* Drop it, no easy way of shrinking it further. */
			QETH_DBF_MESSAGE(2, "Dropped an oversized skb (Max Elements=%u / Actual=%u / Length=%u).\n",
					 max_elements, __elements, skb->len);
			return -E2BIG;
		}

		rc = skb_linearize(skb);
<<<<<<< HEAD
		if (card->options.performance_stats) {
			if (rc)
				card->perf_stats.tx_linfail++;
			else
				card->perf_stats.tx_lin++;
		}
		if (rc)
			return rc;

=======
		if (rc) {
			QETH_TXQ_STAT_INC(queue, skbs_linearized_fail);
			return rc;
		}

		QETH_TXQ_STAT_INC(queue, skbs_linearized);
>>>>>>> upstream/android-13
		/* Linearization changed the layout, re-evaluate: */
		goto check_layout;
	}

	*elements = __elements;
	/* Add the header: */
	if (push_ok) {
		*hdr = skb_push(skb, hdr_len);
		return hdr_len;
	}
<<<<<<< HEAD
	/* fall back */
	*hdr = kmem_cache_alloc(qeth_core_header_cache, GFP_ATOMIC);
=======

	/* Fall back to cache element with known-good alignment: */
	if (hdr_len + proto_len > QETH_HDR_CACHE_OBJ_SIZE)
		return -E2BIG;
	*hdr = kmem_cache_alloc(qeth_core_header_cache, gfp);
>>>>>>> upstream/android-13
	if (!*hdr)
		return -ENOMEM;
	/* Copy protocol headers behind HW header: */
	skb_copy_from_linear_data(skb, ((char *)*hdr) + hdr_len, proto_len);
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_add_hw_header);

static void __qeth_fill_buffer(struct sk_buff *skb,
			       struct qeth_qdio_out_buffer *buf,
			       bool is_first_elem, unsigned int offset)
=======

static bool qeth_iqd_may_bulk(struct qeth_qdio_out_q *queue,
			      struct sk_buff *curr_skb,
			      struct qeth_hdr *curr_hdr)
{
	struct qeth_qdio_out_buffer *buffer = queue->bufs[queue->bulk_start];
	struct qeth_hdr *prev_hdr = queue->prev_hdr;

	if (!prev_hdr)
		return true;

	/* All packets must have the same target: */
	if (curr_hdr->hdr.l2.id == QETH_HEADER_TYPE_LAYER2) {
		struct sk_buff *prev_skb = skb_peek(&buffer->skb_list);

		return ether_addr_equal(eth_hdr(prev_skb)->h_dest,
					eth_hdr(curr_skb)->h_dest) &&
		       qeth_l2_same_vlan(&prev_hdr->hdr.l2, &curr_hdr->hdr.l2);
	}

	return qeth_l3_same_next_hop(&prev_hdr->hdr.l3, &curr_hdr->hdr.l3) &&
	       qeth_l3_iqd_same_vlan(&prev_hdr->hdr.l3, &curr_hdr->hdr.l3);
}

/**
 * qeth_fill_buffer() - map skb into an output buffer
 * @buf:	buffer to transport the skb
 * @skb:	skb to map into the buffer
 * @hdr:	qeth_hdr for this skb. Either at skb->data, or allocated
 *		from qeth_core_header_cache.
 * @offset:	when mapping the skb, start at skb->data + offset
 * @hd_len:	if > 0, build a dedicated header element of this size
 */
static unsigned int qeth_fill_buffer(struct qeth_qdio_out_buffer *buf,
				     struct sk_buff *skb, struct qeth_hdr *hdr,
				     unsigned int offset, unsigned int hd_len)
>>>>>>> upstream/android-13
{
	struct qdio_buffer *buffer = buf->buffer;
	int element = buf->next_element_to_fill;
	int length = skb_headlen(skb) - offset;
	char *data = skb->data + offset;
<<<<<<< HEAD
	int length_here, cnt;

	/* map linear part into buffer element(s) */
	while (length > 0) {
		/* length_here is the remaining amount of data in this page */
		length_here = PAGE_SIZE - ((unsigned long) data % PAGE_SIZE);
		if (length < length_here)
			length_here = length;

		buffer->element[element].addr = data;
		buffer->element[element].length = length_here;
		length -= length_here;
=======
	unsigned int elem_length, cnt;
	bool is_first_elem = true;

	__skb_queue_tail(&buf->skb_list, skb);

	/* build dedicated element for HW Header */
	if (hd_len) {
		is_first_elem = false;

		buffer->element[element].addr = virt_to_phys(hdr);
		buffer->element[element].length = hd_len;
		buffer->element[element].eflags = SBAL_EFLAGS_FIRST_FRAG;

		/* HW header is allocated from cache: */
		if ((void *)hdr != skb->data)
			__set_bit(element, buf->from_kmem_cache);
		/* HW header was pushed and is contiguous with linear part: */
		else if (length > 0 && !PAGE_ALIGNED(data) &&
			 (data == (char *)hdr + hd_len))
			buffer->element[element].eflags |=
				SBAL_EFLAGS_CONTIGUOUS;

		element++;
	}

	/* map linear part into buffer element(s) */
	while (length > 0) {
		elem_length = min_t(unsigned int, length,
				    PAGE_SIZE - offset_in_page(data));

		buffer->element[element].addr = virt_to_phys(data);
		buffer->element[element].length = elem_length;
		length -= elem_length;
>>>>>>> upstream/android-13
		if (is_first_elem) {
			is_first_elem = false;
			if (length || skb_is_nonlinear(skb))
				/* skb needs additional elements */
				buffer->element[element].eflags =
					SBAL_EFLAGS_FIRST_FRAG;
			else
				buffer->element[element].eflags = 0;
		} else {
			buffer->element[element].eflags =
				SBAL_EFLAGS_MIDDLE_FRAG;
		}
<<<<<<< HEAD
		data += length_here;
=======

		data += elem_length;
>>>>>>> upstream/android-13
		element++;
	}

	/* map page frags into buffer element(s) */
	for (cnt = 0; cnt < skb_shinfo(skb)->nr_frags; cnt++) {
		skb_frag_t *frag = &skb_shinfo(skb)->frags[cnt];

		data = skb_frag_address(frag);
		length = skb_frag_size(frag);
		while (length > 0) {
<<<<<<< HEAD
			length_here = PAGE_SIZE -
				((unsigned long) data % PAGE_SIZE);
			if (length < length_here)
				length_here = length;

			buffer->element[element].addr = data;
			buffer->element[element].length = length_here;
			buffer->element[element].eflags =
				SBAL_EFLAGS_MIDDLE_FRAG;
			length -= length_here;
			data += length_here;
=======
			elem_length = min_t(unsigned int, length,
					    PAGE_SIZE - offset_in_page(data));

			buffer->element[element].addr = virt_to_phys(data);
			buffer->element[element].length = elem_length;
			buffer->element[element].eflags =
				SBAL_EFLAGS_MIDDLE_FRAG;

			length -= elem_length;
			data += elem_length;
>>>>>>> upstream/android-13
			element++;
		}
	}

	if (buffer->element[element - 1].eflags)
		buffer->element[element - 1].eflags = SBAL_EFLAGS_LAST_FRAG;
	buf->next_element_to_fill = element;
<<<<<<< HEAD
}

/**
 * qeth_fill_buffer() - map skb into an output buffer
 * @queue:	QDIO queue to submit the buffer on
 * @buf:	buffer to transport the skb
 * @skb:	skb to map into the buffer
 * @hdr:	qeth_hdr for this skb. Either at skb->data, or allocated
 *		from qeth_core_header_cache.
 * @offset:	when mapping the skb, start at skb->data + offset
 * @hd_len:	if > 0, build a dedicated header element of this size
 */
static int qeth_fill_buffer(struct qeth_qdio_out_q *queue,
			    struct qeth_qdio_out_buffer *buf,
			    struct sk_buff *skb, struct qeth_hdr *hdr,
			    unsigned int offset, unsigned int hd_len)
{
	struct qdio_buffer *buffer = buf->buffer;
	bool is_first_elem = true;
	int flush_cnt = 0;

	refcount_inc(&skb->users);
	skb_queue_tail(&buf->skb_list, skb);

	/* build dedicated header element */
	if (hd_len) {
		int element = buf->next_element_to_fill;
		is_first_elem = false;

		buffer->element[element].addr = hdr;
		buffer->element[element].length = hd_len;
		buffer->element[element].eflags = SBAL_EFLAGS_FIRST_FRAG;
		/* remember to free cache-allocated qeth_hdr: */
		buf->is_header[element] = ((void *)hdr != skb->data);
		buf->next_element_to_fill++;
	}

	__qeth_fill_buffer(skb, buf, is_first_elem, offset);

	if (!queue->do_pack) {
		QETH_CARD_TEXT(queue->card, 6, "fillbfnp");
		/* set state to PRIMED -> will be flushed */
		atomic_set(&buf->state, QETH_QDIO_BUF_PRIMED);
		flush_cnt = 1;
	} else {
		QETH_CARD_TEXT(queue->card, 6, "fillbfpa");
		if (queue->card->options.performance_stats)
			queue->card->perf_stats.skbs_sent_pack++;
		if (buf->next_element_to_fill >=
				QETH_MAX_BUFFER_ELEMENTS(queue->card)) {
			/*
			 * packed buffer if full -> set state PRIMED
			 * -> will be flushed
			 */
			atomic_set(&buf->state, QETH_QDIO_BUF_PRIMED);
			flush_cnt = 1;
		}
	}
	return flush_cnt;
}

int qeth_do_send_packet_fast(struct qeth_qdio_out_q *queue, struct sk_buff *skb,
			     struct qeth_hdr *hdr, unsigned int offset,
			     unsigned int hd_len)
{
	int index = queue->next_buf_to_fill;
	struct qeth_qdio_out_buffer *buffer = queue->bufs[index];

	/*
	 * check if buffer is empty to make sure that we do not 'overtake'
	 * ourselves and try to fill a buffer that is already primed
	 */
	if (atomic_read(&buffer->state) != QETH_QDIO_BUF_EMPTY)
		return -EBUSY;
	queue->next_buf_to_fill = (index + 1) % QDIO_MAX_BUFFERS_PER_Q;
	qeth_fill_buffer(queue, buffer, skb, hdr, offset, hd_len);
	qeth_flush_buffers(queue, index, 1);
	return 0;
}
EXPORT_SYMBOL_GPL(qeth_do_send_packet_fast);

int qeth_do_send_packet(struct qeth_card *card, struct qeth_qdio_out_q *queue,
			struct sk_buff *skb, struct qeth_hdr *hdr,
			unsigned int offset, unsigned int hd_len,
			int elements_needed)
{
	struct qeth_qdio_out_buffer *buffer;
	int start_index;
	int flush_count = 0;
	int do_pack = 0;
	int tmp;
	int rc = 0;

	/* spin until we get the queue ... */
	while (atomic_cmpxchg(&queue->state, QETH_OUT_Q_UNLOCKED,
			      QETH_OUT_Q_LOCKED) != QETH_OUT_Q_UNLOCKED);
	start_index = queue->next_buf_to_fill;
	buffer = queue->bufs[queue->next_buf_to_fill];
	/*
	 * check if buffer is empty to make sure that we do not 'overtake'
	 * ourselves and try to fill a buffer that is already primed
	 */
	if (atomic_read(&buffer->state) != QETH_QDIO_BUF_EMPTY) {
		atomic_set(&queue->state, QETH_OUT_Q_UNLOCKED);
		return -EBUSY;
	}
=======
	return element;
}

static int __qeth_xmit(struct qeth_card *card, struct qeth_qdio_out_q *queue,
		       struct sk_buff *skb, unsigned int elements,
		       struct qeth_hdr *hdr, unsigned int offset,
		       unsigned int hd_len)
{
	unsigned int bytes = qdisc_pkt_len(skb);
	struct qeth_qdio_out_buffer *buffer;
	unsigned int next_element;
	struct netdev_queue *txq;
	bool stopped = false;
	bool flush;

	buffer = queue->bufs[QDIO_BUFNR(queue->bulk_start + queue->bulk_count)];
	txq = netdev_get_tx_queue(card->dev, skb_get_queue_mapping(skb));

	/* Just a sanity check, the wake/stop logic should ensure that we always
	 * get a free buffer.
	 */
	if (atomic_read(&buffer->state) != QETH_QDIO_BUF_EMPTY)
		return -EBUSY;

	flush = !qeth_iqd_may_bulk(queue, skb, hdr);

	if (flush ||
	    (buffer->next_element_to_fill + elements > queue->max_elements)) {
		if (buffer->next_element_to_fill > 0) {
			atomic_set(&buffer->state, QETH_QDIO_BUF_PRIMED);
			queue->bulk_count++;
		}

		if (queue->bulk_count >= queue->bulk_max)
			flush = true;

		if (flush)
			qeth_flush_queue(queue);

		buffer = queue->bufs[QDIO_BUFNR(queue->bulk_start +
						queue->bulk_count)];

		/* Sanity-check again: */
		if (atomic_read(&buffer->state) != QETH_QDIO_BUF_EMPTY)
			return -EBUSY;
	}

	if (buffer->next_element_to_fill == 0 &&
	    atomic_inc_return(&queue->used_buffers) >= QDIO_MAX_BUFFERS_PER_Q) {
		/* If a TX completion happens right _here_ and misses to wake
		 * the txq, then our re-check below will catch the race.
		 */
		QETH_TXQ_STAT_INC(queue, stopped);
		netif_tx_stop_queue(txq);
		stopped = true;
	}

	next_element = qeth_fill_buffer(buffer, skb, hdr, offset, hd_len);
	buffer->bytes += bytes;
	buffer->frames += skb_is_gso(skb) ? skb_shinfo(skb)->gso_segs : 1;
	queue->prev_hdr = hdr;

	flush = __netdev_tx_sent_queue(txq, bytes,
				       !stopped && netdev_xmit_more());

	if (flush || next_element >= queue->max_elements) {
		atomic_set(&buffer->state, QETH_QDIO_BUF_PRIMED);
		queue->bulk_count++;

		if (queue->bulk_count >= queue->bulk_max)
			flush = true;

		if (flush)
			qeth_flush_queue(queue);
	}

	if (stopped && !qeth_out_queue_is_full(queue))
		netif_tx_start_queue(txq);
	return 0;
}

static int qeth_do_send_packet(struct qeth_card *card,
			       struct qeth_qdio_out_q *queue,
			       struct sk_buff *skb, struct qeth_hdr *hdr,
			       unsigned int offset, unsigned int hd_len,
			       unsigned int elements_needed)
{
	unsigned int start_index = queue->next_buf_to_fill;
	struct qeth_qdio_out_buffer *buffer;
	unsigned int next_element;
	struct netdev_queue *txq;
	bool stopped = false;
	int flush_count = 0;
	int do_pack = 0;
	int rc = 0;

	buffer = queue->bufs[queue->next_buf_to_fill];

	/* Just a sanity check, the wake/stop logic should ensure that we always
	 * get a free buffer.
	 */
	if (atomic_read(&buffer->state) != QETH_QDIO_BUF_EMPTY)
		return -EBUSY;

	txq = netdev_get_tx_queue(card->dev, skb_get_queue_mapping(skb));

>>>>>>> upstream/android-13
	/* check if we need to switch packing state of this queue */
	qeth_switch_to_packing_if_needed(queue);
	if (queue->do_pack) {
		do_pack = 1;
		/* does packet fit in current buffer? */
<<<<<<< HEAD
		if ((QETH_MAX_BUFFER_ELEMENTS(card) -
		    buffer->next_element_to_fill) < elements_needed) {
=======
		if (buffer->next_element_to_fill + elements_needed >
		    queue->max_elements) {
>>>>>>> upstream/android-13
			/* ... no -> set state PRIMED */
			atomic_set(&buffer->state, QETH_QDIO_BUF_PRIMED);
			flush_count++;
			queue->next_buf_to_fill =
<<<<<<< HEAD
				(queue->next_buf_to_fill + 1) %
				QDIO_MAX_BUFFERS_PER_Q;
			buffer = queue->bufs[queue->next_buf_to_fill];
			/* we did a step forward, so check buffer state
			 * again */
=======
				QDIO_BUFNR(queue->next_buf_to_fill + 1);
			buffer = queue->bufs[queue->next_buf_to_fill];

			/* We stepped forward, so sanity-check again: */
>>>>>>> upstream/android-13
			if (atomic_read(&buffer->state) !=
			    QETH_QDIO_BUF_EMPTY) {
				qeth_flush_buffers(queue, start_index,
							   flush_count);
<<<<<<< HEAD
				atomic_set(&queue->state,
						QETH_OUT_Q_UNLOCKED);
=======
>>>>>>> upstream/android-13
				rc = -EBUSY;
				goto out;
			}
		}
	}
<<<<<<< HEAD
	tmp = qeth_fill_buffer(queue, buffer, skb, hdr, offset, hd_len);
	queue->next_buf_to_fill = (queue->next_buf_to_fill + tmp) %
				  QDIO_MAX_BUFFERS_PER_Q;
	flush_count += tmp;
	if (flush_count)
		qeth_flush_buffers(queue, start_index, flush_count);
	else if (!atomic_read(&queue->set_pci_flags_count))
		atomic_xchg(&queue->state, QETH_OUT_Q_LOCKED_FLUSH);
	/*
	 * queue->state will go from LOCKED -> UNLOCKED or from
	 * LOCKED_FLUSH -> LOCKED if output_handler wanted to 'notify' us
	 * (switch packing state or flush buffer to get another pci flag out).
	 * In that case we will enter this loop
	 */
	while (atomic_dec_return(&queue->state)) {
		start_index = queue->next_buf_to_fill;
		/* check if we can go back to non-packing state */
		tmp = qeth_switch_to_nonpacking_if_needed(queue);
		/*
		 * check if we need to flush a packing buffer to get a pci
		 * flag out on the queue
		 */
		if (!tmp && !atomic_read(&queue->set_pci_flags_count))
			tmp = qeth_prep_flush_pack_buffer(queue);
		if (tmp) {
			qeth_flush_buffers(queue, start_index, tmp);
			flush_count += tmp;
		}
	}
out:
	/* at this point the queue is UNLOCKED again */
	if (queue->card->options.performance_stats && do_pack)
		queue->card->perf_stats.bufs_sent_pack += flush_count;

	return rc;
}
EXPORT_SYMBOL_GPL(qeth_do_send_packet);
=======

	if (buffer->next_element_to_fill == 0 &&
	    atomic_inc_return(&queue->used_buffers) >= QDIO_MAX_BUFFERS_PER_Q) {
		/* If a TX completion happens right _here_ and misses to wake
		 * the txq, then our re-check below will catch the race.
		 */
		QETH_TXQ_STAT_INC(queue, stopped);
		netif_tx_stop_queue(txq);
		stopped = true;
	}

	next_element = qeth_fill_buffer(buffer, skb, hdr, offset, hd_len);
	buffer->bytes += qdisc_pkt_len(skb);
	buffer->frames += skb_is_gso(skb) ? skb_shinfo(skb)->gso_segs : 1;

	if (queue->do_pack)
		QETH_TXQ_STAT_INC(queue, skbs_pack);
	if (!queue->do_pack || stopped || next_element >= queue->max_elements) {
		flush_count++;
		atomic_set(&buffer->state, QETH_QDIO_BUF_PRIMED);
		queue->next_buf_to_fill =
				QDIO_BUFNR(queue->next_buf_to_fill + 1);
	}

	if (flush_count)
		qeth_flush_buffers(queue, start_index, flush_count);

out:
	if (do_pack)
		QETH_TXQ_STAT_ADD(queue, bufs_pack, flush_count);

	if (stopped && !qeth_out_queue_is_full(queue))
		netif_tx_start_queue(txq);
	return rc;
}

static void qeth_fill_tso_ext(struct qeth_hdr_tso *hdr,
			      unsigned int payload_len, struct sk_buff *skb,
			      unsigned int proto_len)
{
	struct qeth_hdr_ext_tso *ext = &hdr->ext;

	ext->hdr_tot_len = sizeof(*ext);
	ext->imb_hdr_no = 1;
	ext->hdr_type = 1;
	ext->hdr_version = 1;
	ext->hdr_len = 28;
	ext->payload_len = payload_len;
	ext->mss = skb_shinfo(skb)->gso_size;
	ext->dg_hdr_len = proto_len;
}

int qeth_xmit(struct qeth_card *card, struct sk_buff *skb,
	      struct qeth_qdio_out_q *queue, __be16 proto,
	      void (*fill_header)(struct qeth_qdio_out_q *queue,
				  struct qeth_hdr *hdr, struct sk_buff *skb,
				  __be16 proto, unsigned int data_len))
{
	unsigned int proto_len, hw_hdr_len;
	unsigned int frame_len = skb->len;
	bool is_tso = skb_is_gso(skb);
	unsigned int data_offset = 0;
	struct qeth_hdr *hdr = NULL;
	unsigned int hd_len = 0;
	unsigned int elements;
	int push_len, rc;

	if (is_tso) {
		hw_hdr_len = sizeof(struct qeth_hdr_tso);
		proto_len = skb_transport_offset(skb) + tcp_hdrlen(skb);
	} else {
		hw_hdr_len = sizeof(struct qeth_hdr);
		proto_len = (IS_IQD(card) && IS_LAYER2(card)) ? ETH_HLEN : 0;
	}

	rc = skb_cow_head(skb, hw_hdr_len);
	if (rc)
		return rc;

	push_len = qeth_add_hw_header(queue, skb, &hdr, hw_hdr_len, proto_len,
				      &elements);
	if (push_len < 0)
		return push_len;
	if (is_tso || !push_len) {
		/* HW header needs its own buffer element. */
		hd_len = hw_hdr_len + proto_len;
		data_offset = push_len + proto_len;
	}
	memset(hdr, 0, hw_hdr_len);
	fill_header(queue, hdr, skb, proto, frame_len);
	if (is_tso)
		qeth_fill_tso_ext((struct qeth_hdr_tso *) hdr,
				  frame_len - proto_len, skb, proto_len);

	if (IS_IQD(card)) {
		rc = __qeth_xmit(card, queue, skb, elements, hdr, data_offset,
				 hd_len);
	} else {
		/* TODO: drop skb_orphan() once TX completion is fast enough */
		skb_orphan(skb);
		spin_lock(&queue->lock);
		rc = qeth_do_send_packet(card, queue, skb, hdr, data_offset,
					 hd_len, elements);
		spin_unlock(&queue->lock);
	}

	if (rc && !push_len)
		kmem_cache_free(qeth_core_header_cache, hdr);

	return rc;
}
EXPORT_SYMBOL_GPL(qeth_xmit);
>>>>>>> upstream/android-13

static int qeth_setadp_promisc_mode_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_ipacmd_setadpparms *setparms;

	QETH_CARD_TEXT(card, 4, "prmadpcb");

	setparms = &(cmd->data.setadapterparms);
	if (qeth_setadpparms_inspect_rc(cmd)) {
		QETH_CARD_TEXT_(card, 4, "prmrc%x", cmd->hdr.return_code);
		setparms->data.mode = SET_PROMISC_MODE_OFF;
	}
	card->info.promisc_mode = setparms->data.mode;
<<<<<<< HEAD
	return 0;
}

void qeth_setadp_promisc_mode(struct qeth_card *card)
{
	enum qeth_ipa_promisc_modes mode;
	struct net_device *dev = card->dev;
=======
	return (cmd->hdr.return_code) ? -EIO : 0;
}

void qeth_setadp_promisc_mode(struct qeth_card *card, bool enable)
{
	enum qeth_ipa_promisc_modes mode = enable ? SET_PROMISC_MODE_ON :
						    SET_PROMISC_MODE_OFF;
>>>>>>> upstream/android-13
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;

	QETH_CARD_TEXT(card, 4, "setprom");
<<<<<<< HEAD

	if (((dev->flags & IFF_PROMISC) &&
	     (card->info.promisc_mode == SET_PROMISC_MODE_ON)) ||
	    (!(dev->flags & IFF_PROMISC) &&
	     (card->info.promisc_mode == SET_PROMISC_MODE_OFF)))
		return;
	mode = SET_PROMISC_MODE_OFF;
	if (dev->flags & IFF_PROMISC)
		mode = SET_PROMISC_MODE_ON;
	QETH_CARD_TEXT_(card, 4, "mode:%x", mode);

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_PROMISC_MODE,
			sizeof(struct qeth_ipacmd_setadpparms_hdr) + 8);
=======
	QETH_CARD_TEXT_(card, 4, "mode:%x", mode);

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_PROMISC_MODE,
				   SETADP_DATA_SIZEOF(mode));
>>>>>>> upstream/android-13
	if (!iob)
		return;
	cmd = __ipa_cmd(iob);
	cmd->data.setadapterparms.data.mode = mode;
	qeth_send_ipa_cmd(card, iob, qeth_setadp_promisc_mode_cb, NULL);
}
EXPORT_SYMBOL_GPL(qeth_setadp_promisc_mode);

<<<<<<< HEAD
struct net_device_stats *qeth_get_stats(struct net_device *dev)
{
	struct qeth_card *card;

	card = dev->ml_priv;

	QETH_CARD_TEXT(card, 5, "getstat");

	return &card->stats;
}
EXPORT_SYMBOL_GPL(qeth_get_stats);

=======
>>>>>>> upstream/android-13
static int qeth_setadpparms_change_macaddr_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
<<<<<<< HEAD

	QETH_CARD_TEXT(card, 4, "chgmaccb");
	if (qeth_setadpparms_inspect_rc(cmd))
		return 0;

	if (!card->options.layer2 ||
	    !(card->info.mac_bits & QETH_LAYER2_MAC_READ)) {
		ether_addr_copy(card->dev->dev_addr,
				cmd->data.setadapterparms.data.change_addr.addr);
		card->info.mac_bits |= QETH_LAYER2_MAC_READ;
	}
=======
	struct qeth_ipacmd_setadpparms *adp_cmd;

	QETH_CARD_TEXT(card, 4, "chgmaccb");
	if (qeth_setadpparms_inspect_rc(cmd))
		return -EIO;

	adp_cmd = &cmd->data.setadapterparms;
	if (!is_valid_ether_addr(adp_cmd->data.change_addr.addr))
		return -EADDRNOTAVAIL;

	if (IS_LAYER2(card) && IS_OSD(card) && !IS_VM_NIC(card) &&
	    !(adp_cmd->hdr.flags & QETH_SETADP_FLAGS_VIRTUAL_MAC))
		return -EADDRNOTAVAIL;

	ether_addr_copy(card->dev->dev_addr, adp_cmd->data.change_addr.addr);
>>>>>>> upstream/android-13
	return 0;
}

int qeth_setadpparms_change_macaddr(struct qeth_card *card)
{
	int rc;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;

	QETH_CARD_TEXT(card, 4, "chgmac");

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_ALTER_MAC_ADDRESS,
<<<<<<< HEAD
				   sizeof(struct qeth_ipacmd_setadpparms_hdr) +
				   sizeof(struct qeth_change_addr));
=======
				   SETADP_DATA_SIZEOF(change_addr));
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
	cmd->data.setadapterparms.data.change_addr.cmd = CHANGE_ADDR_READ_MAC;
	cmd->data.setadapterparms.data.change_addr.addr_size = ETH_ALEN;
	ether_addr_copy(cmd->data.setadapterparms.data.change_addr.addr,
			card->dev->dev_addr);
	rc = qeth_send_ipa_cmd(card, iob, qeth_setadpparms_change_macaddr_cb,
			       NULL);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_setadpparms_change_macaddr);

static int qeth_setadpparms_set_access_ctrl_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_set_access_ctrl *access_ctrl_req;
<<<<<<< HEAD
	int fallback = *(int *)reply->param;

	QETH_CARD_TEXT(card, 4, "setaccb");
	if (cmd->hdr.return_code)
		return 0;
	qeth_setadpparms_inspect_rc(cmd);

	access_ctrl_req = &cmd->data.setadapterparms.data.set_access_ctrl;
	QETH_DBF_TEXT_(SETUP, 2, "setaccb");
	QETH_DBF_TEXT_(SETUP, 2, "%s", card->gdev->dev.kobj.name);
	QETH_DBF_TEXT_(SETUP, 2, "rc=%d",
		cmd->data.setadapterparms.hdr.return_code);
	if (cmd->data.setadapterparms.hdr.return_code !=
						SET_ACCESS_CTRL_RC_SUCCESS)
		QETH_DBF_MESSAGE(3, "ERR:SET_ACCESS_CTRL(%s,%d)==%d\n",
				card->gdev->dev.kobj.name,
				access_ctrl_req->subcmd_code,
				cmd->data.setadapterparms.hdr.return_code);
	switch (cmd->data.setadapterparms.hdr.return_code) {
	case SET_ACCESS_CTRL_RC_SUCCESS:
		if (card->options.isolation == ISOLATION_MODE_NONE) {
			dev_info(&card->gdev->dev,
			    "QDIO data connection isolation is deactivated\n");
		} else {
			dev_info(&card->gdev->dev,
			    "QDIO data connection isolation is activated\n");
		}
		break;
	case SET_ACCESS_CTRL_RC_ALREADY_NOT_ISOLATED:
		QETH_DBF_MESSAGE(2, "%s QDIO data connection isolation already "
				"deactivated\n", dev_name(&card->gdev->dev));
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	case SET_ACCESS_CTRL_RC_ALREADY_ISOLATED:
		QETH_DBF_MESSAGE(2, "%s QDIO data connection isolation already"
				" activated\n", dev_name(&card->gdev->dev));
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	case SET_ACCESS_CTRL_RC_NOT_SUPPORTED:
		dev_err(&card->gdev->dev, "Adapter does not "
			"support QDIO data connection isolation\n");
		break;
=======

	QETH_CARD_TEXT(card, 4, "setaccb");

	access_ctrl_req = &cmd->data.setadapterparms.data.set_access_ctrl;
	QETH_CARD_TEXT_(card, 2, "rc=%d",
			cmd->data.setadapterparms.hdr.return_code);
	if (cmd->data.setadapterparms.hdr.return_code !=
						SET_ACCESS_CTRL_RC_SUCCESS)
		QETH_DBF_MESSAGE(3, "ERR:SET_ACCESS_CTRL(%#x) on device %x: %#x\n",
				 access_ctrl_req->subcmd_code, CARD_DEVID(card),
				 cmd->data.setadapterparms.hdr.return_code);
	switch (qeth_setadpparms_inspect_rc(cmd)) {
	case SET_ACCESS_CTRL_RC_SUCCESS:
		if (access_ctrl_req->subcmd_code == ISOLATION_MODE_NONE)
			dev_info(&card->gdev->dev,
			    "QDIO data connection isolation is deactivated\n");
		else
			dev_info(&card->gdev->dev,
			    "QDIO data connection isolation is activated\n");
		return 0;
	case SET_ACCESS_CTRL_RC_ALREADY_NOT_ISOLATED:
		QETH_DBF_MESSAGE(2, "QDIO data connection isolation on device %x already deactivated\n",
				 CARD_DEVID(card));
		return 0;
	case SET_ACCESS_CTRL_RC_ALREADY_ISOLATED:
		QETH_DBF_MESSAGE(2, "QDIO data connection isolation on device %x already activated\n",
				 CARD_DEVID(card));
		return 0;
	case SET_ACCESS_CTRL_RC_NOT_SUPPORTED:
		dev_err(&card->gdev->dev, "Adapter does not "
			"support QDIO data connection isolation\n");
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13
	case SET_ACCESS_CTRL_RC_NONE_SHARED_ADAPTER:
		dev_err(&card->gdev->dev,
			"Adapter is dedicated. "
			"QDIO data connection isolation not supported\n");
<<<<<<< HEAD
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	case SET_ACCESS_CTRL_RC_ACTIVE_CHECKSUM_OFF:
		dev_err(&card->gdev->dev,
			"TSO does not permit QDIO data connection isolation\n");
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	case SET_ACCESS_CTRL_RC_REFLREL_UNSUPPORTED:
		dev_err(&card->gdev->dev, "The adjacent switch port does not "
			"support reflective relay mode\n");
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	case SET_ACCESS_CTRL_RC_REFLREL_FAILED:
		dev_err(&card->gdev->dev, "The reflective relay mode cannot be "
					"enabled at the adjacent switch port");
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	case SET_ACCESS_CTRL_RC_REFLREL_DEACT_FAILED:
		dev_warn(&card->gdev->dev, "Turning off reflective relay mode "
					"at the adjacent switch failed\n");
		break;
	default:
		/* this should never happen */
		if (fallback)
			card->options.isolation = card->options.prev_isolation;
		break;
	}
	return 0;
}

static int qeth_setadpparms_set_access_ctrl(struct qeth_card *card,
		enum qeth_ipa_isolation_modes isolation, int fallback)
=======
		return -EOPNOTSUPP;
	case SET_ACCESS_CTRL_RC_ACTIVE_CHECKSUM_OFF:
		dev_err(&card->gdev->dev,
			"TSO does not permit QDIO data connection isolation\n");
		return -EPERM;
	case SET_ACCESS_CTRL_RC_REFLREL_UNSUPPORTED:
		dev_err(&card->gdev->dev, "The adjacent switch port does not "
			"support reflective relay mode\n");
		return -EOPNOTSUPP;
	case SET_ACCESS_CTRL_RC_REFLREL_FAILED:
		dev_err(&card->gdev->dev, "The reflective relay mode cannot be "
					"enabled at the adjacent switch port");
		return -EREMOTEIO;
	case SET_ACCESS_CTRL_RC_REFLREL_DEACT_FAILED:
		dev_warn(&card->gdev->dev, "Turning off reflective relay mode "
					"at the adjacent switch failed\n");
		/* benign error while disabling ISOLATION_MODE_FWD */
		return 0;
	default:
		return -EIO;
	}
}

int qeth_setadpparms_set_access_ctrl(struct qeth_card *card,
				     enum qeth_ipa_isolation_modes mode)
>>>>>>> upstream/android-13
{
	int rc;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;
	struct qeth_set_access_ctrl *access_ctrl_req;

	QETH_CARD_TEXT(card, 4, "setacctl");

<<<<<<< HEAD
	QETH_DBF_TEXT_(SETUP, 2, "setacctl");
	QETH_DBF_TEXT_(SETUP, 2, "%s", card->gdev->dev.kobj.name);

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_ACCESS_CONTROL,
				   sizeof(struct qeth_ipacmd_setadpparms_hdr) +
				   sizeof(struct qeth_set_access_ctrl));
=======
	if (!qeth_adp_supported(card, IPA_SETADP_SET_ACCESS_CONTROL)) {
		dev_err(&card->gdev->dev,
			"Adapter does not support QDIO data connection isolation\n");
		return -EOPNOTSUPP;
	}

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_ACCESS_CONTROL,
				   SETADP_DATA_SIZEOF(set_access_ctrl));
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
	access_ctrl_req = &cmd->data.setadapterparms.data.set_access_ctrl;
<<<<<<< HEAD
	access_ctrl_req->subcmd_code = isolation;

	rc = qeth_send_ipa_cmd(card, iob, qeth_setadpparms_set_access_ctrl_cb,
			       &fallback);
	QETH_DBF_TEXT_(SETUP, 2, "rc=%d", rc);
	return rc;
}

int qeth_set_access_ctrl_online(struct qeth_card *card, int fallback)
{
	int rc = 0;

	QETH_CARD_TEXT(card, 4, "setactlo");

	if ((card->info.type == QETH_CARD_TYPE_OSD ||
	     card->info.type == QETH_CARD_TYPE_OSX) &&
	     qeth_adp_supported(card, IPA_SETADP_SET_ACCESS_CONTROL)) {
		rc = qeth_setadpparms_set_access_ctrl(card,
			card->options.isolation, fallback);
		if (rc) {
			QETH_DBF_MESSAGE(3,
				"IPA(SET_ACCESS_CTRL,%s,%d) sent failed\n",
				card->gdev->dev.kobj.name,
				rc);
			rc = -EOPNOTSUPP;
		}
	} else if (card->options.isolation != ISOLATION_MODE_NONE) {
		card->options.isolation = ISOLATION_MODE_NONE;

		dev_err(&card->gdev->dev, "Adapter does not "
			"support QDIO data connection isolation\n");
		rc = -EOPNOTSUPP;
	}
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_set_access_ctrl_online);

void qeth_tx_timeout(struct net_device *dev)
=======
	access_ctrl_req->subcmd_code = mode;

	rc = qeth_send_ipa_cmd(card, iob, qeth_setadpparms_set_access_ctrl_cb,
			       NULL);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "rc=%d", rc);
		QETH_DBF_MESSAGE(3, "IPA(SET_ACCESS_CTRL(%d) on device %x: sent failed\n",
				 rc, CARD_DEVID(card));
	}

	return rc;
}

void qeth_tx_timeout(struct net_device *dev, unsigned int txqueue)
>>>>>>> upstream/android-13
{
	struct qeth_card *card;

	card = dev->ml_priv;
	QETH_CARD_TEXT(card, 4, "txtimeo");
<<<<<<< HEAD
	card->stats.tx_errors++;
=======
>>>>>>> upstream/android-13
	qeth_schedule_recovery(card);
}
EXPORT_SYMBOL_GPL(qeth_tx_timeout);

static int qeth_mdio_read(struct net_device *dev, int phy_id, int regnum)
{
	struct qeth_card *card = dev->ml_priv;
	int rc = 0;

	switch (regnum) {
	case MII_BMCR: /* Basic mode control register */
		rc = BMCR_FULLDPLX;
		if ((card->info.link_type != QETH_LINK_TYPE_GBIT_ETH) &&
<<<<<<< HEAD
		    (card->info.link_type != QETH_LINK_TYPE_OSN) &&
		    (card->info.link_type != QETH_LINK_TYPE_10GBIT_ETH))
=======
		    (card->info.link_type != QETH_LINK_TYPE_10GBIT_ETH) &&
		    (card->info.link_type != QETH_LINK_TYPE_25GBIT_ETH))
>>>>>>> upstream/android-13
			rc |= BMCR_SPEED100;
		break;
	case MII_BMSR: /* Basic mode status register */
		rc = BMSR_ERCAP | BMSR_ANEGCOMPLETE | BMSR_LSTATUS |
		     BMSR_10HALF | BMSR_10FULL | BMSR_100HALF | BMSR_100FULL |
		     BMSR_100BASE4;
		break;
	case MII_PHYSID1: /* PHYS ID 1 */
		rc = (dev->dev_addr[0] << 16) | (dev->dev_addr[1] << 8) |
		     dev->dev_addr[2];
		rc = (rc >> 5) & 0xFFFF;
		break;
	case MII_PHYSID2: /* PHYS ID 2 */
		rc = (dev->dev_addr[2] << 10) & 0xFFFF;
		break;
	case MII_ADVERTISE: /* Advertisement control reg */
		rc = ADVERTISE_ALL;
		break;
	case MII_LPA: /* Link partner ability reg */
		rc = LPA_10HALF | LPA_10FULL | LPA_100HALF | LPA_100FULL |
		     LPA_100BASE4 | LPA_LPACK;
		break;
	case MII_EXPANSION: /* Expansion register */
		break;
	case MII_DCOUNTER: /* disconnect counter */
		break;
	case MII_FCSCOUNTER: /* false carrier counter */
		break;
	case MII_NWAYTEST: /* N-way auto-neg test register */
		break;
	case MII_RERRCOUNTER: /* rx error counter */
<<<<<<< HEAD
		rc = card->stats.rx_errors;
=======
		rc = card->stats.rx_length_errors +
		     card->stats.rx_frame_errors +
		     card->stats.rx_fifo_errors;
>>>>>>> upstream/android-13
		break;
	case MII_SREVISION: /* silicon revision */
		break;
	case MII_RESV1: /* reserved 1 */
		break;
	case MII_LBRERROR: /* loopback, rx, bypass error */
		break;
	case MII_PHYADDR: /* physical address */
		break;
	case MII_RESV2: /* reserved 2 */
		break;
	case MII_TPISTATUS: /* TPI status for 10mbps */
		break;
	case MII_NCONFIG: /* network interface config */
		break;
	default:
		break;
	}
	return rc;
}

<<<<<<< HEAD
static int qeth_send_ipa_snmp_cmd(struct qeth_card *card,
		struct qeth_cmd_buffer *iob, int len,
		int (*reply_cb)(struct qeth_card *, struct qeth_reply *,
			unsigned long),
		void *reply_param)
{
	u16 s1, s2;

	QETH_CARD_TEXT(card, 4, "sendsnmp");

	memcpy(iob->data, IPA_PDU_HEADER, IPA_PDU_HEADER_SIZE);
	memcpy(QETH_IPA_CMD_DEST_ADDR(iob->data),
	       &card->token.ulp_connection_r, QETH_MPC_TOKEN_LENGTH);
	/* adjust PDU length fields in IPA_PDU_HEADER */
	s1 = (u32) IPA_PDU_HEADER_SIZE + len;
	s2 = (u32) len;
	memcpy(QETH_IPA_PDU_LEN_TOTAL(iob->data), &s1, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU1(iob->data), &s2, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU2(iob->data), &s2, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU3(iob->data), &s2, 2);
	return qeth_send_control_data(card, IPA_PDU_HEADER_SIZE + len, iob,
				      reply_cb, reply_param);
}

static int qeth_snmp_command_cb(struct qeth_card *card,
		struct qeth_reply *reply, unsigned long sdata)
{
	struct qeth_ipa_cmd *cmd;
	struct qeth_arp_query_info *qinfo;
	unsigned char *data;
	void *snmp_data;
	__u16 data_len;

	QETH_CARD_TEXT(card, 3, "snpcmdcb");

	cmd = (struct qeth_ipa_cmd *) sdata;
	data = (unsigned char *)((char *)cmd - reply->offset);
	qinfo = (struct qeth_arp_query_info *) reply->param;

	if (cmd->hdr.return_code) {
		QETH_CARD_TEXT_(card, 4, "scer1%x", cmd->hdr.return_code);
		return 0;
=======
static int qeth_snmp_command_cb(struct qeth_card *card,
				struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_arp_query_info *qinfo = reply->param;
	struct qeth_ipacmd_setadpparms *adp_cmd;
	unsigned int data_len;
	void *snmp_data;

	QETH_CARD_TEXT(card, 3, "snpcmdcb");

	if (cmd->hdr.return_code) {
		QETH_CARD_TEXT_(card, 4, "scer1%x", cmd->hdr.return_code);
		return -EIO;
>>>>>>> upstream/android-13
	}
	if (cmd->data.setadapterparms.hdr.return_code) {
		cmd->hdr.return_code =
			cmd->data.setadapterparms.hdr.return_code;
		QETH_CARD_TEXT_(card, 4, "scer2%x", cmd->hdr.return_code);
<<<<<<< HEAD
		return 0;
	}
	data_len = *((__u16 *)QETH_IPA_PDU_LEN_PDU1(data));
	if (cmd->data.setadapterparms.hdr.seq_no == 1) {
		snmp_data = &cmd->data.setadapterparms.data.snmp;
		data_len -= offsetof(struct qeth_ipa_cmd,
				     data.setadapterparms.data.snmp);
	} else {
		snmp_data = &cmd->data.setadapterparms.data.snmp.request;
		data_len -= offsetof(struct qeth_ipa_cmd,
				     data.setadapterparms.data.snmp.request);
=======
		return -EIO;
	}

	adp_cmd = &cmd->data.setadapterparms;
	data_len = adp_cmd->hdr.cmdlength - sizeof(adp_cmd->hdr);
	if (adp_cmd->hdr.seq_no == 1) {
		snmp_data = &adp_cmd->data.snmp;
	} else {
		snmp_data = &adp_cmd->data.snmp.request;
		data_len -= offsetof(struct qeth_snmp_cmd, request);
>>>>>>> upstream/android-13
	}

	/* check if there is enough room in userspace */
	if ((qinfo->udata_len - qinfo->udata_offset) < data_len) {
<<<<<<< HEAD
		QETH_CARD_TEXT_(card, 4, "scer3%i", -ENOMEM);
		cmd->hdr.return_code = IPA_RC_ENOMEM;
		return 0;
	}
	QETH_CARD_TEXT_(card, 4, "snore%i",
		       cmd->data.setadapterparms.hdr.used_total);
	QETH_CARD_TEXT_(card, 4, "sseqn%i",
		cmd->data.setadapterparms.hdr.seq_no);
=======
		QETH_CARD_TEXT_(card, 4, "scer3%i", -ENOSPC);
		return -ENOSPC;
	}
	QETH_CARD_TEXT_(card, 4, "snore%i",
			cmd->data.setadapterparms.hdr.used_total);
	QETH_CARD_TEXT_(card, 4, "sseqn%i",
			cmd->data.setadapterparms.hdr.seq_no);
>>>>>>> upstream/android-13
	/*copy entries to user buffer*/
	memcpy(qinfo->udata + qinfo->udata_offset, snmp_data, data_len);
	qinfo->udata_offset += data_len;

<<<<<<< HEAD
	/* check if all replies received ... */
		QETH_CARD_TEXT_(card, 4, "srtot%i",
			       cmd->data.setadapterparms.hdr.used_total);
		QETH_CARD_TEXT_(card, 4, "srseq%i",
			       cmd->data.setadapterparms.hdr.seq_no);
=======
>>>>>>> upstream/android-13
	if (cmd->data.setadapterparms.hdr.seq_no <
	    cmd->data.setadapterparms.hdr.used_total)
		return 1;
	return 0;
}

static int qeth_snmp_command(struct qeth_card *card, char __user *udata)
{
<<<<<<< HEAD
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;
	struct qeth_snmp_ureq *ureq;
=======
	struct qeth_snmp_ureq __user *ureq;
	struct qeth_cmd_buffer *iob;
>>>>>>> upstream/android-13
	unsigned int req_len;
	struct qeth_arp_query_info qinfo = {0, };
	int rc = 0;

	QETH_CARD_TEXT(card, 3, "snmpcmd");

<<<<<<< HEAD
	if (card->info.guestlan)
		return -EOPNOTSUPP;

	if ((!qeth_adp_supported(card, IPA_SETADP_SET_SNMP_CONTROL)) &&
	    (!card->options.layer2)) {
		return -EOPNOTSUPP;
	}
	/* skip 4 bytes (data_len struct member) to get req_len */
	if (copy_from_user(&req_len, udata + sizeof(int), sizeof(int)))
		return -EFAULT;
	if (req_len > (QETH_BUFSIZE - IPA_PDU_HEADER_SIZE -
		       sizeof(struct qeth_ipacmd_hdr) -
		       sizeof(struct qeth_ipacmd_setadpparms_hdr)))
		return -EINVAL;
	ureq = memdup_user(udata, req_len + sizeof(struct qeth_snmp_ureq_hdr));
	if (IS_ERR(ureq)) {
		QETH_CARD_TEXT(card, 2, "snmpnome");
		return PTR_ERR(ureq);
	}
	qinfo.udata_len = ureq->hdr.data_len;
	qinfo.udata = kzalloc(qinfo.udata_len, GFP_KERNEL);
	if (!qinfo.udata) {
		kfree(ureq);
=======
	if (IS_VM_NIC(card))
		return -EOPNOTSUPP;

	if ((!qeth_adp_supported(card, IPA_SETADP_SET_SNMP_CONTROL)) &&
	    IS_LAYER3(card))
		return -EOPNOTSUPP;

	ureq = (struct qeth_snmp_ureq __user *) udata;
	if (get_user(qinfo.udata_len, &ureq->hdr.data_len) ||
	    get_user(req_len, &ureq->hdr.req_len))
		return -EFAULT;

	/* Sanitize user input, to avoid overflows in iob size calculation: */
	if (req_len > QETH_BUFSIZE)
		return -EINVAL;

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_SNMP_CONTROL, req_len);
	if (!iob)
		return -ENOMEM;

	if (copy_from_user(&__ipa_cmd(iob)->data.setadapterparms.data.snmp,
			   &ureq->cmd, req_len)) {
		qeth_put_cmd(iob);
		return -EFAULT;
	}

	qinfo.udata = kzalloc(qinfo.udata_len, GFP_KERNEL);
	if (!qinfo.udata) {
		qeth_put_cmd(iob);
>>>>>>> upstream/android-13
		return -ENOMEM;
	}
	qinfo.udata_offset = sizeof(struct qeth_snmp_ureq_hdr);

<<<<<<< HEAD
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_SNMP_CONTROL,
				   QETH_SNMP_SETADP_CMDLENGTH + req_len);
	if (!iob) {
		rc = -ENOMEM;
		goto out;
	}
	cmd = __ipa_cmd(iob);
	memcpy(&cmd->data.setadapterparms.data.snmp, &ureq->cmd, req_len);
	rc = qeth_send_ipa_snmp_cmd(card, iob, QETH_SETADP_BASE_LEN + req_len,
				    qeth_snmp_command_cb, (void *)&qinfo);
	if (rc)
		QETH_DBF_MESSAGE(2, "SNMP command failed on %s: (0x%x)\n",
			   QETH_CARD_IFNAME(card), rc);
=======
	rc = qeth_send_ipa_cmd(card, iob, qeth_snmp_command_cb, &qinfo);
	if (rc)
		QETH_DBF_MESSAGE(2, "SNMP command failed on device %x: (%#x)\n",
				 CARD_DEVID(card), rc);
>>>>>>> upstream/android-13
	else {
		if (copy_to_user(udata, qinfo.udata, qinfo.udata_len))
			rc = -EFAULT;
	}
<<<<<<< HEAD
out:
	kfree(ureq);
=======

>>>>>>> upstream/android-13
	kfree(qinfo.udata);
	return rc;
}

static int qeth_setadpparms_query_oat_cb(struct qeth_card *card,
<<<<<<< HEAD
		struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *)data;
	struct qeth_qoat_priv *priv;
	char *resdata;
=======
					 struct qeth_reply *reply,
					 unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *)data;
	struct qeth_qoat_priv *priv = reply->param;
>>>>>>> upstream/android-13
	int resdatalen;

	QETH_CARD_TEXT(card, 3, "qoatcb");
	if (qeth_setadpparms_inspect_rc(cmd))
<<<<<<< HEAD
		return 0;

	priv = (struct qeth_qoat_priv *)reply->param;
	resdatalen = cmd->data.setadapterparms.hdr.cmdlength;
	resdata = (char *)data + 28;

	if (resdatalen > (priv->buffer_len - priv->response_len)) {
		cmd->hdr.return_code = IPA_RC_FFFF;
		return 0;
	}

	memcpy((priv->buffer + priv->response_len), resdata,
		resdatalen);
=======
		return -EIO;

	resdatalen = cmd->data.setadapterparms.hdr.cmdlength;

	if (resdatalen > (priv->buffer_len - priv->response_len))
		return -ENOSPC;

	memcpy(priv->buffer + priv->response_len,
	       &cmd->data.setadapterparms.hdr, resdatalen);
>>>>>>> upstream/android-13
	priv->response_len += resdatalen;

	if (cmd->data.setadapterparms.hdr.seq_no <
	    cmd->data.setadapterparms.hdr.used_total)
		return 1;
	return 0;
}

static int qeth_query_oat_command(struct qeth_card *card, char __user *udata)
{
	int rc = 0;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;
	struct qeth_query_oat *oat_req;
	struct qeth_query_oat_data oat_data;
	struct qeth_qoat_priv priv;
	void __user *tmp;

	QETH_CARD_TEXT(card, 3, "qoatcmd");

<<<<<<< HEAD
	if (!qeth_adp_supported(card, IPA_SETADP_QUERY_OAT)) {
		rc = -EOPNOTSUPP;
		goto out;
	}

	if (copy_from_user(&oat_data, udata,
	    sizeof(struct qeth_query_oat_data))) {
			rc = -EFAULT;
			goto out;
	}
=======
	if (!qeth_adp_supported(card, IPA_SETADP_QUERY_OAT))
		return -EOPNOTSUPP;

	if (copy_from_user(&oat_data, udata, sizeof(oat_data)))
		return -EFAULT;
>>>>>>> upstream/android-13

	priv.buffer_len = oat_data.buffer_len;
	priv.response_len = 0;
	priv.buffer = vzalloc(oat_data.buffer_len);
<<<<<<< HEAD
	if (!priv.buffer) {
		rc = -ENOMEM;
		goto out;
	}

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_OAT,
				   sizeof(struct qeth_ipacmd_setadpparms_hdr) +
				   sizeof(struct qeth_query_oat));
=======
	if (!priv.buffer)
		return -ENOMEM;

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_OAT,
				   SETADP_DATA_SIZEOF(query_oat));
>>>>>>> upstream/android-13
	if (!iob) {
		rc = -ENOMEM;
		goto out_free;
	}
	cmd = __ipa_cmd(iob);
	oat_req = &cmd->data.setadapterparms.data.query_oat;
	oat_req->subcmd_code = oat_data.command;

<<<<<<< HEAD
	rc = qeth_send_ipa_cmd(card, iob, qeth_setadpparms_query_oat_cb,
			       &priv);
	if (!rc) {
		if (is_compat_task())
			tmp = compat_ptr(oat_data.ptr);
		else
			tmp = (void __user *)(unsigned long)oat_data.ptr;

		if (copy_to_user(tmp, priv.buffer,
		    priv.response_len)) {
			rc = -EFAULT;
			goto out_free;
		}

		oat_data.response_len = priv.response_len;

		if (copy_to_user(udata, &oat_data,
		    sizeof(struct qeth_query_oat_data)))
			rc = -EFAULT;
	} else
		if (rc == IPA_RC_FFFF)
			rc = -EFAULT;

out_free:
	vfree(priv.buffer);
out:
=======
	rc = qeth_send_ipa_cmd(card, iob, qeth_setadpparms_query_oat_cb, &priv);
	if (!rc) {
		tmp = is_compat_task() ? compat_ptr(oat_data.ptr) :
					 u64_to_user_ptr(oat_data.ptr);
		oat_data.response_len = priv.response_len;

		if (copy_to_user(tmp, priv.buffer, priv.response_len) ||
		    copy_to_user(udata, &oat_data, sizeof(oat_data)))
			rc = -EFAULT;
	}

out_free:
	vfree(priv.buffer);
>>>>>>> upstream/android-13
	return rc;
}

static int qeth_query_card_info_cb(struct qeth_card *card,
				   struct qeth_reply *reply, unsigned long data)
{
<<<<<<< HEAD
	struct carrier_info *carrier_info = (struct carrier_info *)reply->param;
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *)data;
=======
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *)data;
	struct qeth_link_info *link_info = reply->param;
>>>>>>> upstream/android-13
	struct qeth_query_card_info *card_info;

	QETH_CARD_TEXT(card, 2, "qcrdincb");
	if (qeth_setadpparms_inspect_rc(cmd))
<<<<<<< HEAD
		return 0;

	card_info = &cmd->data.setadapterparms.data.card_info;
	carrier_info->card_type = card_info->card_type;
	carrier_info->port_mode = card_info->port_mode;
	carrier_info->port_speed = card_info->port_speed;
	return 0;
}

static int qeth_query_card_info(struct qeth_card *card,
				struct carrier_info *carrier_info)
=======
		return -EIO;

	card_info = &cmd->data.setadapterparms.data.card_info;
	netdev_dbg(card->dev,
		   "card info: card_type=0x%02x, port_mode=0x%04x, port_speed=0x%08x\n",
		   card_info->card_type, card_info->port_mode,
		   card_info->port_speed);

	switch (card_info->port_mode) {
	case CARD_INFO_PORTM_FULLDUPLEX:
		link_info->duplex = DUPLEX_FULL;
		break;
	case CARD_INFO_PORTM_HALFDUPLEX:
		link_info->duplex = DUPLEX_HALF;
		break;
	default:
		link_info->duplex = DUPLEX_UNKNOWN;
	}

	switch (card_info->card_type) {
	case CARD_INFO_TYPE_1G_COPPER_A:
	case CARD_INFO_TYPE_1G_COPPER_B:
		link_info->speed = SPEED_1000;
		link_info->port = PORT_TP;
		break;
	case CARD_INFO_TYPE_1G_FIBRE_A:
	case CARD_INFO_TYPE_1G_FIBRE_B:
		link_info->speed = SPEED_1000;
		link_info->port = PORT_FIBRE;
		break;
	case CARD_INFO_TYPE_10G_FIBRE_A:
	case CARD_INFO_TYPE_10G_FIBRE_B:
		link_info->speed = SPEED_10000;
		link_info->port = PORT_FIBRE;
		break;
	default:
		switch (card_info->port_speed) {
		case CARD_INFO_PORTS_10M:
			link_info->speed = SPEED_10;
			break;
		case CARD_INFO_PORTS_100M:
			link_info->speed = SPEED_100;
			break;
		case CARD_INFO_PORTS_1G:
			link_info->speed = SPEED_1000;
			break;
		case CARD_INFO_PORTS_10G:
			link_info->speed = SPEED_10000;
			break;
		case CARD_INFO_PORTS_25G:
			link_info->speed = SPEED_25000;
			break;
		default:
			link_info->speed = SPEED_UNKNOWN;
		}

		link_info->port = PORT_OTHER;
	}

	return 0;
}

int qeth_query_card_info(struct qeth_card *card,
			 struct qeth_link_info *link_info)
>>>>>>> upstream/android-13
{
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "qcrdinfo");
	if (!qeth_adp_supported(card, IPA_SETADP_QUERY_CARD_INFO))
		return -EOPNOTSUPP;
<<<<<<< HEAD
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_CARD_INFO,
		sizeof(struct qeth_ipacmd_setadpparms_hdr));
	if (!iob)
		return -ENOMEM;
	return qeth_send_ipa_cmd(card, iob, qeth_query_card_info_cb,
					(void *)carrier_info);
=======
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_CARD_INFO, 0);
	if (!iob)
		return -ENOMEM;

	return qeth_send_ipa_cmd(card, iob, qeth_query_card_info_cb, link_info);
}

static int qeth_init_link_info_oat_cb(struct qeth_card *card,
				      struct qeth_reply *reply_priv,
				      unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *)data;
	struct qeth_link_info *link_info = reply_priv->param;
	struct qeth_query_oat_physical_if *phys_if;
	struct qeth_query_oat_reply *reply;

	if (qeth_setadpparms_inspect_rc(cmd))
		return -EIO;

	/* Multi-part reply is unexpected, don't bother: */
	if (cmd->data.setadapterparms.hdr.used_total > 1)
		return -EINVAL;

	/* Expect the reply to start with phys_if data: */
	reply = &cmd->data.setadapterparms.data.query_oat.reply[0];
	if (reply->type != QETH_QOAT_REPLY_TYPE_PHYS_IF ||
	    reply->length < sizeof(*reply))
		return -EINVAL;

	phys_if = &reply->phys_if;

	switch (phys_if->speed_duplex) {
	case QETH_QOAT_PHYS_SPEED_10M_HALF:
		link_info->speed = SPEED_10;
		link_info->duplex = DUPLEX_HALF;
		break;
	case QETH_QOAT_PHYS_SPEED_10M_FULL:
		link_info->speed = SPEED_10;
		link_info->duplex = DUPLEX_FULL;
		break;
	case QETH_QOAT_PHYS_SPEED_100M_HALF:
		link_info->speed = SPEED_100;
		link_info->duplex = DUPLEX_HALF;
		break;
	case QETH_QOAT_PHYS_SPEED_100M_FULL:
		link_info->speed = SPEED_100;
		link_info->duplex = DUPLEX_FULL;
		break;
	case QETH_QOAT_PHYS_SPEED_1000M_HALF:
		link_info->speed = SPEED_1000;
		link_info->duplex = DUPLEX_HALF;
		break;
	case QETH_QOAT_PHYS_SPEED_1000M_FULL:
		link_info->speed = SPEED_1000;
		link_info->duplex = DUPLEX_FULL;
		break;
	case QETH_QOAT_PHYS_SPEED_10G_FULL:
		link_info->speed = SPEED_10000;
		link_info->duplex = DUPLEX_FULL;
		break;
	case QETH_QOAT_PHYS_SPEED_25G_FULL:
		link_info->speed = SPEED_25000;
		link_info->duplex = DUPLEX_FULL;
		break;
	case QETH_QOAT_PHYS_SPEED_UNKNOWN:
	default:
		link_info->speed = SPEED_UNKNOWN;
		link_info->duplex = DUPLEX_UNKNOWN;
		break;
	}

	switch (phys_if->media_type) {
	case QETH_QOAT_PHYS_MEDIA_COPPER:
		link_info->port = PORT_TP;
		link_info->link_mode = QETH_LINK_MODE_UNKNOWN;
		break;
	case QETH_QOAT_PHYS_MEDIA_FIBRE_SHORT:
		link_info->port = PORT_FIBRE;
		link_info->link_mode = QETH_LINK_MODE_FIBRE_SHORT;
		break;
	case QETH_QOAT_PHYS_MEDIA_FIBRE_LONG:
		link_info->port = PORT_FIBRE;
		link_info->link_mode = QETH_LINK_MODE_FIBRE_LONG;
		break;
	default:
		link_info->port = PORT_OTHER;
		link_info->link_mode = QETH_LINK_MODE_UNKNOWN;
		break;
	}

	return 0;
}

static void qeth_init_link_info(struct qeth_card *card)
{
	card->info.link_info.duplex = DUPLEX_FULL;

	if (IS_IQD(card) || IS_VM_NIC(card)) {
		card->info.link_info.speed = SPEED_10000;
		card->info.link_info.port = PORT_FIBRE;
		card->info.link_info.link_mode = QETH_LINK_MODE_FIBRE_SHORT;
	} else {
		switch (card->info.link_type) {
		case QETH_LINK_TYPE_FAST_ETH:
		case QETH_LINK_TYPE_LANE_ETH100:
			card->info.link_info.speed = SPEED_100;
			card->info.link_info.port = PORT_TP;
			break;
		case QETH_LINK_TYPE_GBIT_ETH:
		case QETH_LINK_TYPE_LANE_ETH1000:
			card->info.link_info.speed = SPEED_1000;
			card->info.link_info.port = PORT_FIBRE;
			break;
		case QETH_LINK_TYPE_10GBIT_ETH:
			card->info.link_info.speed = SPEED_10000;
			card->info.link_info.port = PORT_FIBRE;
			break;
		case QETH_LINK_TYPE_25GBIT_ETH:
			card->info.link_info.speed = SPEED_25000;
			card->info.link_info.port = PORT_FIBRE;
			break;
		default:
			dev_info(&card->gdev->dev, "Unknown link type %x\n",
				 card->info.link_type);
			card->info.link_info.speed = SPEED_UNKNOWN;
			card->info.link_info.port = PORT_OTHER;
		}

		card->info.link_info.link_mode = QETH_LINK_MODE_UNKNOWN;
	}

	/* Get more accurate data via QUERY OAT: */
	if (qeth_adp_supported(card, IPA_SETADP_QUERY_OAT)) {
		struct qeth_link_info link_info;
		struct qeth_cmd_buffer *iob;

		iob = qeth_get_adapter_cmd(card, IPA_SETADP_QUERY_OAT,
					   SETADP_DATA_SIZEOF(query_oat));
		if (iob) {
			struct qeth_ipa_cmd *cmd = __ipa_cmd(iob);
			struct qeth_query_oat *oat_req;

			oat_req = &cmd->data.setadapterparms.data.query_oat;
			oat_req->subcmd_code = QETH_QOAT_SCOPE_INTERFACE;

			if (!qeth_send_ipa_cmd(card, iob,
					       qeth_init_link_info_oat_cb,
					       &link_info)) {
				if (link_info.speed != SPEED_UNKNOWN)
					card->info.link_info.speed = link_info.speed;
				if (link_info.duplex != DUPLEX_UNKNOWN)
					card->info.link_info.duplex = link_info.duplex;
				if (link_info.port != PORT_OTHER)
					card->info.link_info.port = link_info.port;
				if (link_info.link_mode != QETH_LINK_MODE_UNKNOWN)
					card->info.link_info.link_mode = link_info.link_mode;
			}
		}
	}
>>>>>>> upstream/android-13
}

/**
 * qeth_vm_request_mac() - Request a hypervisor-managed MAC address
 * @card: pointer to a qeth_card
 *
 * Returns
 *	0, if a MAC address has been set for the card's netdevice
 *	a return code, for various error conditions
 */
int qeth_vm_request_mac(struct qeth_card *card)
{
	struct diag26c_mac_resp *response;
	struct diag26c_mac_req *request;
<<<<<<< HEAD
	struct ccw_dev_id id;
	int rc;

	QETH_DBF_TEXT(SETUP, 2, "vmreqmac");
=======
	int rc;

	QETH_CARD_TEXT(card, 2, "vmreqmac");
>>>>>>> upstream/android-13

	request = kzalloc(sizeof(*request), GFP_KERNEL | GFP_DMA);
	response = kzalloc(sizeof(*response), GFP_KERNEL | GFP_DMA);
	if (!request || !response) {
		rc = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	ccw_device_get_id(CARD_DDEV(card), &id);
	request->resp_buf_len = sizeof(*response);
	request->resp_version = DIAG26C_VERSION2;
	request->op_code = DIAG26C_GET_MAC;
	request->devno = id.devno;
=======
	request->resp_buf_len = sizeof(*response);
	request->resp_version = DIAG26C_VERSION2;
	request->op_code = DIAG26C_GET_MAC;
	request->devno = card->info.ddev_devno;
>>>>>>> upstream/android-13

	QETH_DBF_HEX(CTRL, 2, request, sizeof(*request));
	rc = diag26c(request, response, DIAG26C_MAC_SERVICES);
	QETH_DBF_HEX(CTRL, 2, request, sizeof(*request));
	if (rc)
		goto out;
	QETH_DBF_HEX(CTRL, 2, response, sizeof(*response));

	if (request->resp_buf_len < sizeof(*response) ||
	    response->version != request->resp_version) {
		rc = -EIO;
<<<<<<< HEAD
		QETH_DBF_TEXT(SETUP, 2, "badresp");
		QETH_DBF_HEX(SETUP, 2, &request->resp_buf_len,
			     sizeof(request->resp_buf_len));
	} else if (!is_valid_ether_addr(response->mac)) {
		rc = -EINVAL;
		QETH_DBF_TEXT(SETUP, 2, "badmac");
		QETH_DBF_HEX(SETUP, 2, response->mac, ETH_ALEN);
=======
		QETH_CARD_TEXT(card, 2, "badresp");
		QETH_CARD_HEX(card, 2, &request->resp_buf_len,
			      sizeof(request->resp_buf_len));
	} else if (!is_valid_ether_addr(response->mac)) {
		rc = -EINVAL;
		QETH_CARD_TEXT(card, 2, "badmac");
		QETH_CARD_HEX(card, 2, response->mac, ETH_ALEN);
>>>>>>> upstream/android-13
	} else {
		ether_addr_copy(card->dev->dev_addr, response->mac);
	}

out:
	kfree(response);
	kfree(request);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_vm_request_mac);

<<<<<<< HEAD
static int qeth_get_qdio_q_format(struct qeth_card *card)
{
	if (card->info.type == QETH_CARD_TYPE_IQD)
		return QDIO_IQDIO_QFMT;
	else
		return QDIO_QETH_QFMT;
}

static void qeth_determine_capabilities(struct qeth_card *card)
{
	int rc;
	int length;
	char *prcd;
	struct ccw_device *ddev;
	int ddev_offline = 0;

	QETH_DBF_TEXT(SETUP, 2, "detcapab");
	ddev = CARD_DDEV(card);
	if (!ddev->online) {
		ddev_offline = 1;
		rc = ccw_device_set_online(ddev);
		if (rc) {
			QETH_DBF_TEXT_(SETUP, 2, "3err%d", rc);
=======
static void qeth_determine_capabilities(struct qeth_card *card)
{
	struct qeth_channel *channel = &card->data;
	struct ccw_device *ddev = channel->ccwdev;
	int rc;
	int ddev_offline = 0;

	QETH_CARD_TEXT(card, 2, "detcapab");
	if (!ddev->online) {
		ddev_offline = 1;
		rc = qeth_start_channel(channel);
		if (rc) {
			QETH_CARD_TEXT_(card, 2, "3err%d", rc);
>>>>>>> upstream/android-13
			goto out;
		}
	}

<<<<<<< HEAD
	rc = qeth_read_conf_data(card, (void **) &prcd, &length);
	if (rc) {
		QETH_DBF_MESSAGE(2, "%s qeth_read_conf_data returned %i\n",
			dev_name(&card->gdev->dev), rc);
		QETH_DBF_TEXT_(SETUP, 2, "5err%d", rc);
		goto out_offline;
	}
	qeth_configure_unitaddr(card, prcd);
	if (ddev_offline)
		qeth_configure_blkt_default(card, prcd);
	kfree(prcd);

	rc = qdio_get_ssqd_desc(ddev, &card->ssqd);
	if (rc)
		QETH_DBF_TEXT_(SETUP, 2, "6err%d", rc);

	QETH_DBF_TEXT_(SETUP, 2, "qfmt%d", card->ssqd.qfmt);
	QETH_DBF_TEXT_(SETUP, 2, "ac1:%02x", card->ssqd.qdioac1);
	QETH_DBF_TEXT_(SETUP, 2, "ac2:%04x", card->ssqd.qdioac2);
	QETH_DBF_TEXT_(SETUP, 2, "ac3:%04x", card->ssqd.qdioac3);
	QETH_DBF_TEXT_(SETUP, 2, "icnt%d", card->ssqd.icnt);
=======
	rc = qeth_read_conf_data(card);
	if (rc) {
		QETH_DBF_MESSAGE(2, "qeth_read_conf_data on device %x returned %i\n",
				 CARD_DEVID(card), rc);
		QETH_CARD_TEXT_(card, 2, "5err%d", rc);
		goto out_offline;
	}

	rc = qdio_get_ssqd_desc(ddev, &card->ssqd);
	if (rc)
		QETH_CARD_TEXT_(card, 2, "6err%d", rc);

	QETH_CARD_TEXT_(card, 2, "qfmt%d", card->ssqd.qfmt);
	QETH_CARD_TEXT_(card, 2, "ac1:%02x", card->ssqd.qdioac1);
	QETH_CARD_TEXT_(card, 2, "ac2:%04x", card->ssqd.qdioac2);
	QETH_CARD_TEXT_(card, 2, "ac3:%04x", card->ssqd.qdioac3);
	QETH_CARD_TEXT_(card, 2, "icnt%d", card->ssqd.icnt);
>>>>>>> upstream/android-13
	if (!((card->ssqd.qfmt != QDIO_IQDIO_QFMT) ||
	    ((card->ssqd.qdioac1 & CHSC_AC1_INITIATE_INPUTQ) == 0) ||
	    ((card->ssqd.qdioac3 & CHSC_AC3_FORMAT2_CQ_AVAILABLE) == 0))) {
		dev_info(&card->gdev->dev,
			"Completion Queueing supported\n");
	} else {
		card->options.cq = QETH_CQ_NOTAVAILABLE;
	}

<<<<<<< HEAD

out_offline:
	if (ddev_offline == 1)
		ccw_device_set_offline(ddev);
=======
out_offline:
	if (ddev_offline == 1)
		qeth_stop_channel(channel);
>>>>>>> upstream/android-13
out:
	return;
}

<<<<<<< HEAD
static void qeth_qdio_establish_cq(struct qeth_card *card,
				   struct qdio_buffer **in_sbal_ptrs,
				   void (**queue_start_poll)
					(struct ccw_device *, int,
					 unsigned long))
{
	int i;

	if (card->options.cq == QETH_CQ_ENABLED) {
		int offset = QDIO_MAX_BUFFERS_PER_Q *
			     (card->qdio.no_in_queues - 1);

		for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; i++)
			in_sbal_ptrs[offset + i] =
				card->qdio.c_q->bufs[i].buffer;

		queue_start_poll[card->qdio.no_in_queues - 1] = NULL;
	}
=======
static void qeth_read_ccw_conf_data(struct qeth_card *card)
{
	struct qeth_card_info *info = &card->info;
	struct ccw_device *cdev = CARD_DDEV(card);
	struct ccw_dev_id dev_id;

	QETH_CARD_TEXT(card, 2, "ccwconfd");
	ccw_device_get_id(cdev, &dev_id);

	info->ddev_devno = dev_id.devno;
	info->ids_valid = !ccw_device_get_cssid(cdev, &info->cssid) &&
			  !ccw_device_get_iid(cdev, &info->iid) &&
			  !ccw_device_get_chid(cdev, 0, &info->chid);
	info->ssid = dev_id.ssid;

	dev_info(&card->gdev->dev, "CHID: %x CHPID: %x\n",
		 info->chid, info->chpid);

	QETH_CARD_TEXT_(card, 3, "devn%x", info->ddev_devno);
	QETH_CARD_TEXT_(card, 3, "cssid:%x", info->cssid);
	QETH_CARD_TEXT_(card, 3, "iid:%x", info->iid);
	QETH_CARD_TEXT_(card, 3, "ssid:%x", info->ssid);
	QETH_CARD_TEXT_(card, 3, "chpid:%x", info->chpid);
	QETH_CARD_TEXT_(card, 3, "chid:%x", info->chid);
	QETH_CARD_TEXT_(card, 3, "idval%x", info->ids_valid);
>>>>>>> upstream/android-13
}

static int qeth_qdio_establish(struct qeth_card *card)
{
<<<<<<< HEAD
	struct qdio_initialize init_data;
	char *qib_param_field;
	struct qdio_buffer **in_sbal_ptrs;
	void (**queue_start_poll) (struct ccw_device *, int, unsigned long);
	struct qdio_buffer **out_sbal_ptrs;
	int i, j, k;
	int rc = 0;

	QETH_DBF_TEXT(SETUP, 2, "qdioest");

	qib_param_field = kzalloc(QDIO_MAX_BUFFERS_PER_Q,
				  GFP_KERNEL);
	if (!qib_param_field) {
		rc =  -ENOMEM;
		goto out_free_nothing;
	}

	qeth_create_qib_param_field(card, qib_param_field);
	qeth_create_qib_param_field_blkt(card, qib_param_field);

	in_sbal_ptrs = kcalloc(card->qdio.no_in_queues * QDIO_MAX_BUFFERS_PER_Q,
			       sizeof(void *),
			       GFP_KERNEL);
	if (!in_sbal_ptrs) {
		rc = -ENOMEM;
		goto out_free_qib_param;
	}

	for (i = 0; i < QDIO_MAX_BUFFERS_PER_Q; i++)
		in_sbal_ptrs[i] = card->qdio.in_q->bufs[i].buffer;

	queue_start_poll = kcalloc(card->qdio.no_in_queues, sizeof(void *),
				   GFP_KERNEL);
	if (!queue_start_poll) {
		rc = -ENOMEM;
		goto out_free_in_sbals;
	}
	for (i = 0; i < card->qdio.no_in_queues; ++i)
		queue_start_poll[i] = qeth_qdio_start_poll;

	qeth_qdio_establish_cq(card, in_sbal_ptrs, queue_start_poll);

	out_sbal_ptrs =
		kcalloc(card->qdio.no_out_queues * QDIO_MAX_BUFFERS_PER_Q,
			sizeof(void *),
			GFP_KERNEL);
	if (!out_sbal_ptrs) {
		rc = -ENOMEM;
		goto out_free_queue_start_poll;
	}

	for (i = 0, k = 0; i < card->qdio.no_out_queues; ++i)
		for (j = 0; j < QDIO_MAX_BUFFERS_PER_Q; j++, k++)
			out_sbal_ptrs[k] =
				card->qdio.out_qs[i]->bufs[j]->buffer;

	memset(&init_data, 0, sizeof(struct qdio_initialize));
	init_data.cdev                   = CARD_DDEV(card);
	init_data.q_format               = qeth_get_qdio_q_format(card);
	init_data.qib_param_field_format = 0;
	init_data.qib_param_field        = qib_param_field;
=======
	struct qdio_buffer **out_sbal_ptrs[QETH_MAX_OUT_QUEUES];
	struct qdio_buffer **in_sbal_ptrs[QETH_MAX_IN_QUEUES];
	struct qeth_qib_parms *qib_parms = NULL;
	struct qdio_initialize init_data;
	unsigned int i;
	int rc = 0;

	QETH_CARD_TEXT(card, 2, "qdioest");

	if (!IS_IQD(card) && !IS_VM_NIC(card)) {
		qib_parms = kzalloc(sizeof_field(struct qib, parm), GFP_KERNEL);
		if (!qib_parms)
			return -ENOMEM;

		qeth_fill_qib_parms(card, qib_parms);
	}

	in_sbal_ptrs[0] = card->qdio.in_q->qdio_bufs;
	if (card->options.cq == QETH_CQ_ENABLED)
		in_sbal_ptrs[1] = card->qdio.c_q->qdio_bufs;

	for (i = 0; i < card->qdio.no_out_queues; i++)
		out_sbal_ptrs[i] = card->qdio.out_qs[i]->qdio_bufs;

	memset(&init_data, 0, sizeof(struct qdio_initialize));
	init_data.q_format		 = IS_IQD(card) ? QDIO_IQDIO_QFMT :
							  QDIO_QETH_QFMT;
	init_data.qib_param_field_format = 0;
	init_data.qib_param_field	 = (void *)qib_parms;
>>>>>>> upstream/android-13
	init_data.no_input_qs            = card->qdio.no_in_queues;
	init_data.no_output_qs           = card->qdio.no_out_queues;
	init_data.input_handler		 = qeth_qdio_input_handler;
	init_data.output_handler	 = qeth_qdio_output_handler;
<<<<<<< HEAD
	init_data.queue_start_poll_array = queue_start_poll;
	init_data.int_parm               = (unsigned long) card;
	init_data.input_sbal_addr_array  = (void **) in_sbal_ptrs;
	init_data.output_sbal_addr_array = (void **) out_sbal_ptrs;
	init_data.output_sbal_state_array = card->qdio.out_bufstates;
	init_data.scan_threshold =
		(card->info.type == QETH_CARD_TYPE_IQD) ? 1 : 32;

	if (atomic_cmpxchg(&card->qdio.state, QETH_QDIO_ALLOCATED,
		QETH_QDIO_ESTABLISHED) == QETH_QDIO_ALLOCATED) {
		rc = qdio_allocate(&init_data);
=======
	init_data.irq_poll		 = qeth_qdio_poll;
	init_data.int_parm               = (unsigned long) card;
	init_data.input_sbal_addr_array  = in_sbal_ptrs;
	init_data.output_sbal_addr_array = out_sbal_ptrs;

	if (atomic_cmpxchg(&card->qdio.state, QETH_QDIO_ALLOCATED,
		QETH_QDIO_ESTABLISHED) == QETH_QDIO_ALLOCATED) {
		rc = qdio_allocate(CARD_DDEV(card), init_data.no_input_qs,
				   init_data.no_output_qs);
>>>>>>> upstream/android-13
		if (rc) {
			atomic_set(&card->qdio.state, QETH_QDIO_ALLOCATED);
			goto out;
		}
<<<<<<< HEAD
		rc = qdio_establish(&init_data);
=======
		rc = qdio_establish(CARD_DDEV(card), &init_data);
>>>>>>> upstream/android-13
		if (rc) {
			atomic_set(&card->qdio.state, QETH_QDIO_ALLOCATED);
			qdio_free(CARD_DDEV(card));
		}
	}

	switch (card->options.cq) {
	case QETH_CQ_ENABLED:
		dev_info(&card->gdev->dev, "Completion Queue support enabled");
		break;
	case QETH_CQ_DISABLED:
		dev_info(&card->gdev->dev, "Completion Queue support disabled");
		break;
	default:
		break;
	}
<<<<<<< HEAD
out:
	kfree(out_sbal_ptrs);
out_free_queue_start_poll:
	kfree(queue_start_poll);
out_free_in_sbals:
	kfree(in_sbal_ptrs);
out_free_qib_param:
	kfree(qib_param_field);
out_free_nothing:
=======

out:
	kfree(qib_parms);
>>>>>>> upstream/android-13
	return rc;
}

static void qeth_core_free_card(struct qeth_card *card)
{
<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "freecrd");
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));
	qeth_clean_channel(&card->read);
	qeth_clean_channel(&card->write);
	qeth_clean_channel(&card->data);
	qeth_free_qdio_buffers(card);
	unregister_service_level(&card->qeth_service_level);
	kfree(card);
}

void qeth_trace_features(struct qeth_card *card)
=======
	QETH_CARD_TEXT(card, 2, "freecrd");

	unregister_service_level(&card->qeth_service_level);
	debugfs_remove_recursive(card->debugfs);
	qeth_put_cmd(card->read_cmd);
	destroy_workqueue(card->event_wq);
	dev_set_drvdata(&card->gdev->dev, NULL);
	kfree(card);
}

static void qeth_trace_features(struct qeth_card *card)
>>>>>>> upstream/android-13
{
	QETH_CARD_TEXT(card, 2, "features");
	QETH_CARD_HEX(card, 2, &card->options.ipa4, sizeof(card->options.ipa4));
	QETH_CARD_HEX(card, 2, &card->options.ipa6, sizeof(card->options.ipa6));
	QETH_CARD_HEX(card, 2, &card->options.adp, sizeof(card->options.adp));
	QETH_CARD_HEX(card, 2, &card->info.diagass_support,
		      sizeof(card->info.diagass_support));
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(qeth_trace_features);
=======
>>>>>>> upstream/android-13

static struct ccw_device_id qeth_ids[] = {
	{CCW_DEVICE_DEVTYPE(0x1731, 0x01, 0x1732, 0x01),
					.driver_info = QETH_CARD_TYPE_OSD},
	{CCW_DEVICE_DEVTYPE(0x1731, 0x05, 0x1732, 0x05),
					.driver_info = QETH_CARD_TYPE_IQD},
<<<<<<< HEAD
	{CCW_DEVICE_DEVTYPE(0x1731, 0x06, 0x1732, 0x06),
					.driver_info = QETH_CARD_TYPE_OSN},
	{CCW_DEVICE_DEVTYPE(0x1731, 0x02, 0x1732, 0x03),
					.driver_info = QETH_CARD_TYPE_OSM},
	{CCW_DEVICE_DEVTYPE(0x1731, 0x02, 0x1732, 0x02),
					.driver_info = QETH_CARD_TYPE_OSX},
=======
	{CCW_DEVICE_DEVTYPE(0x1731, 0x02, 0x1732, 0x03),
					.driver_info = QETH_CARD_TYPE_OSM},
#ifdef CONFIG_QETH_OSX
	{CCW_DEVICE_DEVTYPE(0x1731, 0x02, 0x1732, 0x02),
					.driver_info = QETH_CARD_TYPE_OSX},
#endif
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(ccw, qeth_ids);

static struct ccw_driver qeth_ccw_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "qeth",
	},
	.ids = qeth_ids,
	.probe = ccwgroup_probe_ccwdev,
	.remove = ccwgroup_remove_ccwdev,
};

<<<<<<< HEAD
int qeth_core_hardsetup_card(struct qeth_card *card)
=======
static int qeth_hardsetup_card(struct qeth_card *card, bool *carrier_ok)
>>>>>>> upstream/android-13
{
	int retries = 3;
	int rc;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "hrdsetup");
=======
	QETH_CARD_TEXT(card, 2, "hrdsetup");
>>>>>>> upstream/android-13
	atomic_set(&card->force_alloc_skb, 0);
	rc = qeth_update_from_chp_desc(card);
	if (rc)
		return rc;
retry:
	if (retries < 3)
<<<<<<< HEAD
		QETH_DBF_MESSAGE(2, "%s Retrying to do IDX activates.\n",
			dev_name(&card->gdev->dev));
	rc = qeth_qdio_clear_card(card, card->info.type != QETH_CARD_TYPE_IQD);
	ccw_device_set_offline(CARD_DDEV(card));
	ccw_device_set_offline(CARD_WDEV(card));
	ccw_device_set_offline(CARD_RDEV(card));
	qdio_free(CARD_DDEV(card));
	rc = ccw_device_set_online(CARD_RDEV(card));
	if (rc)
		goto retriable;
	rc = ccw_device_set_online(CARD_WDEV(card));
	if (rc)
		goto retriable;
	rc = ccw_device_set_online(CARD_DDEV(card));
=======
		QETH_DBF_MESSAGE(2, "Retrying to do IDX activates on device %x.\n",
				 CARD_DEVID(card));
	rc = qeth_qdio_clear_card(card, !IS_IQD(card));
	qeth_stop_channel(&card->data);
	qeth_stop_channel(&card->write);
	qeth_stop_channel(&card->read);
	qdio_free(CARD_DDEV(card));

	rc = qeth_start_channel(&card->read);
	if (rc)
		goto retriable;
	rc = qeth_start_channel(&card->write);
	if (rc)
		goto retriable;
	rc = qeth_start_channel(&card->data);
>>>>>>> upstream/android-13
	if (rc)
		goto retriable;
retriable:
	if (rc == -ERESTARTSYS) {
<<<<<<< HEAD
		QETH_DBF_TEXT(SETUP, 2, "break1");
		return rc;
	} else if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
=======
		QETH_CARD_TEXT(card, 2, "break1");
		return rc;
	} else if (rc) {
		QETH_CARD_TEXT_(card, 2, "1err%d", rc);
>>>>>>> upstream/android-13
		if (--retries < 0)
			goto out;
		else
			goto retry;
	}
<<<<<<< HEAD
	qeth_determine_capabilities(card);
	qeth_init_tokens(card);
	qeth_init_func_level(card);
	rc = qeth_idx_activate_channel(&card->read, qeth_idx_read_cb);
	if (rc == -ERESTARTSYS) {
		QETH_DBF_TEXT(SETUP, 2, "break2");
		return rc;
	} else if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "3err%d", rc);
=======

	qeth_determine_capabilities(card);
	qeth_read_ccw_conf_data(card);
	qeth_idx_init(card);

	rc = qeth_idx_activate_read_channel(card);
	if (rc == -EINTR) {
		QETH_CARD_TEXT(card, 2, "break2");
		return rc;
	} else if (rc) {
		QETH_CARD_TEXT_(card, 2, "3err%d", rc);
>>>>>>> upstream/android-13
		if (--retries < 0)
			goto out;
		else
			goto retry;
	}
<<<<<<< HEAD
	rc = qeth_idx_activate_channel(&card->write, qeth_idx_write_cb);
	if (rc == -ERESTARTSYS) {
		QETH_DBF_TEXT(SETUP, 2, "break3");
		return rc;
	} else if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "4err%d", rc);
=======

	rc = qeth_idx_activate_write_channel(card);
	if (rc == -EINTR) {
		QETH_CARD_TEXT(card, 2, "break3");
		return rc;
	} else if (rc) {
		QETH_CARD_TEXT_(card, 2, "4err%d", rc);
>>>>>>> upstream/android-13
		if (--retries < 0)
			goto out;
		else
			goto retry;
	}
	card->read_or_write_problem = 0;
	rc = qeth_mpc_initialize(card);
	if (rc) {
<<<<<<< HEAD
		QETH_DBF_TEXT_(SETUP, 2, "5err%d", rc);
=======
		QETH_CARD_TEXT_(card, 2, "5err%d", rc);
>>>>>>> upstream/android-13
		goto out;
	}

	rc = qeth_send_startlan(card);
	if (rc) {
<<<<<<< HEAD
		QETH_DBF_TEXT_(SETUP, 2, "6err%d", rc);
		if (rc == IPA_RC_LAN_OFFLINE) {
			dev_warn(&card->gdev->dev,
				"The LAN is offline\n");
			card->lan_online = 0;
		} else {
			rc = -ENODEV;
			goto out;
		}
	} else
		card->lan_online = 1;

	card->options.ipa4.supported_funcs = 0;
	card->options.ipa6.supported_funcs = 0;
	card->options.adp.supported_funcs = 0;
=======
		QETH_CARD_TEXT_(card, 2, "6err%d", rc);
		if (rc == -ENETDOWN) {
			dev_warn(&card->gdev->dev, "The LAN is offline\n");
			*carrier_ok = false;
		} else {
			goto out;
		}
	} else {
		*carrier_ok = true;
	}

	card->options.ipa4.supported = 0;
	card->options.ipa6.supported = 0;
	card->options.adp.supported = 0;
>>>>>>> upstream/android-13
	card->options.sbp.supported_funcs = 0;
	card->info.diagass_support = 0;
	rc = qeth_query_ipassists(card, QETH_PROT_IPV4);
	if (rc == -ENOMEM)
		goto out;
	if (qeth_is_supported(card, IPA_IPV6)) {
		rc = qeth_query_ipassists(card, QETH_PROT_IPV6);
		if (rc == -ENOMEM)
			goto out;
	}
	if (qeth_is_supported(card, IPA_SETADAPTERPARMS)) {
		rc = qeth_query_setadapterparms(card);
		if (rc < 0) {
<<<<<<< HEAD
			QETH_DBF_TEXT_(SETUP, 2, "7err%d", rc);
=======
			QETH_CARD_TEXT_(card, 2, "7err%d", rc);
>>>>>>> upstream/android-13
			goto out;
		}
	}
	if (qeth_adp_supported(card, IPA_SETADP_SET_DIAG_ASSIST)) {
		rc = qeth_query_setdiagass(card);
<<<<<<< HEAD
		if (rc < 0) {
			QETH_DBF_TEXT_(SETUP, 2, "8err%d", rc);
			goto out;
		}
	}
=======
		if (rc)
			QETH_CARD_TEXT_(card, 2, "8err%d", rc);
	}

	qeth_trace_features(card);

	if (!qeth_is_diagass_supported(card, QETH_DIAGS_CMD_TRAP) ||
	    (card->info.hwtrap && qeth_hw_trap(card, QETH_DIAGS_TRAP_ARM)))
		card->info.hwtrap = 0;

	if (card->options.isolation != ISOLATION_MODE_NONE) {
		rc = qeth_setadpparms_set_access_ctrl(card,
						      card->options.isolation);
		if (rc)
			goto out;
	}

	qeth_init_link_info(card);

	rc = qeth_init_qdio_queues(card);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "9err%d", rc);
		goto out;
	}

>>>>>>> upstream/android-13
	return 0;
out:
	dev_warn(&card->gdev->dev, "The qeth device driver failed to recover "
		"an error on the device\n");
<<<<<<< HEAD
	QETH_DBF_MESSAGE(2, "%s Initialization in hardsetup failed! rc=%d\n",
		dev_name(&card->gdev->dev), rc);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_core_hardsetup_card);

static void qeth_create_skb_frag(struct qdio_buffer_element *element,
				 struct sk_buff *skb, int offset, int data_len)
{
	struct page *page = virt_to_page(element->addr);
	unsigned int next_frag;

	/* first fill the linear space */
	if (!skb->len) {
		unsigned int linear = min(data_len, skb_tailroom(skb));

		skb_put_data(skb, element->addr + offset, linear);
		data_len -= linear;
		if (!data_len)
			return;
		offset += linear;
		/* fall through to add page frag for remaining data */
	}

	next_frag = skb_shinfo(skb)->nr_frags;
	get_page(page);
	skb_add_rx_frag(skb, next_frag, page, offset, data_len, data_len);
=======
	QETH_DBF_MESSAGE(2, "Initialization for device %x failed in hardsetup! rc=%d\n",
			 CARD_DEVID(card), rc);
	return rc;
}

static int qeth_set_online(struct qeth_card *card,
			   const struct qeth_discipline *disc)
{
	bool carrier_ok;
	int rc;

	mutex_lock(&card->conf_mutex);
	QETH_CARD_TEXT(card, 2, "setonlin");

	rc = qeth_hardsetup_card(card, &carrier_ok);
	if (rc) {
		QETH_CARD_TEXT_(card, 2, "2err%04x", rc);
		rc = -ENODEV;
		goto err_hardsetup;
	}

	qeth_print_status_message(card);

	if (card->dev->reg_state != NETREG_REGISTERED)
		/* no need for locking / error handling at this early stage: */
		qeth_set_real_num_tx_queues(card, qeth_tx_actual_queues(card));

	rc = disc->set_online(card, carrier_ok);
	if (rc)
		goto err_online;

	/* let user_space know that device is online */
	kobject_uevent(&card->gdev->dev.kobj, KOBJ_CHANGE);

	mutex_unlock(&card->conf_mutex);
	return 0;

err_online:
err_hardsetup:
	qeth_qdio_clear_card(card, 0);
	qeth_clear_working_pool_list(card);
	qeth_flush_local_addrs(card);

	qeth_stop_channel(&card->data);
	qeth_stop_channel(&card->write);
	qeth_stop_channel(&card->read);
	qdio_free(CARD_DDEV(card));

	mutex_unlock(&card->conf_mutex);
	return rc;
}

int qeth_set_offline(struct qeth_card *card, const struct qeth_discipline *disc,
		     bool resetting)
{
	int rc, rc2, rc3;

	mutex_lock(&card->conf_mutex);
	QETH_CARD_TEXT(card, 3, "setoffl");

	if ((!resetting && card->info.hwtrap) || card->info.hwtrap == 2) {
		qeth_hw_trap(card, QETH_DIAGS_TRAP_DISARM);
		card->info.hwtrap = 1;
	}

	/* cancel any stalled cmd that might block the rtnl: */
	qeth_clear_ipacmd_list(card);

	rtnl_lock();
	card->info.open_when_online = card->dev->flags & IFF_UP;
	dev_close(card->dev);
	netif_device_detach(card->dev);
	netif_carrier_off(card->dev);
	rtnl_unlock();

	cancel_work_sync(&card->rx_mode_work);

	disc->set_offline(card);

	qeth_qdio_clear_card(card, 0);
	qeth_drain_output_queues(card);
	qeth_clear_working_pool_list(card);
	qeth_flush_local_addrs(card);
	card->info.promisc_mode = 0;

	rc  = qeth_stop_channel(&card->data);
	rc2 = qeth_stop_channel(&card->write);
	rc3 = qeth_stop_channel(&card->read);
	if (!rc)
		rc = (rc2) ? rc2 : rc3;
	if (rc)
		QETH_CARD_TEXT_(card, 2, "1err%d", rc);
	qdio_free(CARD_DDEV(card));

	/* let user_space know that device is offline */
	kobject_uevent(&card->gdev->dev.kobj, KOBJ_CHANGE);

	mutex_unlock(&card->conf_mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(qeth_set_offline);

static int qeth_do_reset(void *data)
{
	const struct qeth_discipline *disc;
	struct qeth_card *card = data;
	int rc;

	/* Lock-free, other users will block until we are done. */
	disc = card->discipline;

	QETH_CARD_TEXT(card, 2, "recover1");
	if (!qeth_do_run_thread(card, QETH_RECOVER_THREAD))
		return 0;
	QETH_CARD_TEXT(card, 2, "recover2");
	dev_warn(&card->gdev->dev,
		 "A recovery process has been started for the device\n");

	qeth_set_offline(card, disc, true);
	rc = qeth_set_online(card, disc);
	if (!rc) {
		dev_info(&card->gdev->dev,
			 "Device successfully recovered!\n");
	} else {
		qeth_set_offline(card, disc, true);
		ccwgroup_set_offline(card->gdev, false);
		dev_warn(&card->gdev->dev,
			 "The qeth device driver failed to recover an error on the device\n");
	}
	qeth_clear_thread_start_bit(card, QETH_RECOVER_THREAD);
	qeth_clear_thread_running_bit(card, QETH_RECOVER_THREAD);
	return 0;
}

#if IS_ENABLED(CONFIG_QETH_L3)
static void qeth_l3_rebuild_skb(struct qeth_card *card, struct sk_buff *skb,
				struct qeth_hdr *hdr)
{
	struct af_iucv_trans_hdr *iucv = (struct af_iucv_trans_hdr *) skb->data;
	struct qeth_hdr_layer3 *l3_hdr = &hdr->hdr.l3;
	struct net_device *dev = skb->dev;

	if (IS_IQD(card) && iucv->magic == ETH_P_AF_IUCV) {
		dev_hard_header(skb, dev, ETH_P_AF_IUCV, dev->dev_addr,
				"FAKELL", skb->len);
		return;
	}

	if (!(l3_hdr->flags & QETH_HDR_PASSTHRU)) {
		u16 prot = (l3_hdr->flags & QETH_HDR_IPV6) ? ETH_P_IPV6 :
							     ETH_P_IP;
		unsigned char tg_addr[ETH_ALEN];

		skb_reset_network_header(skb);
		switch (l3_hdr->flags & QETH_HDR_CAST_MASK) {
		case QETH_CAST_MULTICAST:
			if (prot == ETH_P_IP)
				ip_eth_mc_map(ip_hdr(skb)->daddr, tg_addr);
			else
				ipv6_eth_mc_map(&ipv6_hdr(skb)->daddr, tg_addr);
			QETH_CARD_STAT_INC(card, rx_multicast);
			break;
		case QETH_CAST_BROADCAST:
			ether_addr_copy(tg_addr, dev->broadcast);
			QETH_CARD_STAT_INC(card, rx_multicast);
			break;
		default:
			if (card->options.sniffer)
				skb->pkt_type = PACKET_OTHERHOST;
			ether_addr_copy(tg_addr, dev->dev_addr);
		}

		if (l3_hdr->ext_flags & QETH_HDR_EXT_SRC_MAC_ADDR)
			dev_hard_header(skb, dev, prot, tg_addr,
					&l3_hdr->next_hop.rx.src_mac, skb->len);
		else
			dev_hard_header(skb, dev, prot, tg_addr, "FAKELL",
					skb->len);
	}

	/* copy VLAN tag from hdr into skb */
	if (!card->options.sniffer &&
	    (l3_hdr->ext_flags & (QETH_HDR_EXT_VLAN_FRAME |
				  QETH_HDR_EXT_INCLUDE_VLAN_TAG))) {
		u16 tag = (l3_hdr->ext_flags & QETH_HDR_EXT_VLAN_FRAME) ?
				l3_hdr->vlan_id :
				l3_hdr->next_hop.rx.vlan_id;

		__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), tag);
	}
}
#endif

static void qeth_receive_skb(struct qeth_card *card, struct sk_buff *skb,
			     struct qeth_hdr *hdr, bool uses_frags)
{
	struct napi_struct *napi = &card->napi;
	bool is_cso;

	switch (hdr->hdr.l2.id) {
#if IS_ENABLED(CONFIG_QETH_L3)
	case QETH_HEADER_TYPE_LAYER3:
		qeth_l3_rebuild_skb(card, skb, hdr);
		is_cso = hdr->hdr.l3.ext_flags & QETH_HDR_EXT_CSUM_TRANSP_REQ;
		break;
#endif
	case QETH_HEADER_TYPE_LAYER2:
		is_cso = hdr->hdr.l2.flags[1] & QETH_HDR_EXT_CSUM_TRANSP_REQ;
		break;
	default:
		/* never happens */
		if (uses_frags)
			napi_free_frags(napi);
		else
			dev_kfree_skb_any(skb);
		return;
	}

	if (is_cso && (card->dev->features & NETIF_F_RXCSUM)) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		QETH_CARD_STAT_INC(card, rx_skb_csum);
	} else {
		skb->ip_summed = CHECKSUM_NONE;
	}

	QETH_CARD_STAT_ADD(card, rx_bytes, skb->len);
	QETH_CARD_STAT_INC(card, rx_packets);
	if (skb_is_nonlinear(skb)) {
		QETH_CARD_STAT_INC(card, rx_sg_skbs);
		QETH_CARD_STAT_ADD(card, rx_sg_frags,
				   skb_shinfo(skb)->nr_frags);
	}

	if (uses_frags) {
		napi_gro_frags(napi);
	} else {
		skb->protocol = eth_type_trans(skb, skb->dev);
		napi_gro_receive(napi, skb);
	}
}

static void qeth_create_skb_frag(struct sk_buff *skb, char *data, int data_len)
{
	struct page *page = virt_to_page(data);
	unsigned int next_frag;

	next_frag = skb_shinfo(skb)->nr_frags;
	get_page(page);
	skb_add_rx_frag(skb, next_frag, page, offset_in_page(data), data_len,
			data_len);
>>>>>>> upstream/android-13
}

static inline int qeth_is_last_sbale(struct qdio_buffer_element *sbale)
{
	return (sbale->eflags & SBAL_EFLAGS_LAST_ENTRY);
}

<<<<<<< HEAD
struct sk_buff *qeth_core_get_next_skb(struct qeth_card *card,
		struct qeth_qdio_buffer *qethbuffer,
		struct qdio_buffer_element **__element, int *__offset,
		struct qeth_hdr **hdr)
{
	struct qdio_buffer_element *element = *__element;
	struct qdio_buffer *buffer = qethbuffer->buffer;
	int offset = *__offset;
	struct sk_buff *skb;
	int skb_len = 0;
	void *data_ptr;
	int data_len;
	int headroom = 0;
	int use_rx_sg = 0;

	/* qeth_hdr must not cross element boundaries */
	while (element->length < offset + sizeof(struct qeth_hdr)) {
		if (qeth_is_last_sbale(element))
			return NULL;
		element++;
		offset = 0;
	}
	*hdr = element->addr + offset;

	offset += sizeof(struct qeth_hdr);
	switch ((*hdr)->hdr.l2.id) {
	case QETH_HEADER_TYPE_LAYER2:
		skb_len = (*hdr)->hdr.l2.pkt_length;
		break;
	case QETH_HEADER_TYPE_LAYER3:
		skb_len = (*hdr)->hdr.l3.length;
		headroom = ETH_HLEN;
		break;
	case QETH_HEADER_TYPE_OSN:
		skb_len = (*hdr)->hdr.osn.pdu_length;
		headroom = sizeof(struct qeth_hdr);
		break;
	default:
		break;
	}

	if (!skb_len)
		return NULL;

	if (((skb_len >= card->options.rx_sg_cb) &&
	     (!(card->info.type == QETH_CARD_TYPE_OSN)) &&
	     (!atomic_read(&card->force_alloc_skb))) ||
	    (card->options.cq == QETH_CQ_ENABLED))
		use_rx_sg = 1;

	if (use_rx_sg && qethbuffer->rx_skb) {
		/* QETH_CQ_ENABLED only: */
		skb = qethbuffer->rx_skb;
		qethbuffer->rx_skb = NULL;
	} else {
		unsigned int linear = (use_rx_sg) ? QETH_RX_PULL_LEN : skb_len;

		skb = napi_alloc_skb(&card->napi, linear + headroom);
	}
	if (!skb)
		goto no_mem;
	if (headroom)
		skb_reserve(skb, headroom);

	data_ptr = element->addr + offset;
	while (skb_len) {
		data_len = min(skb_len, (int)(element->length - offset));
		if (data_len) {
			if (use_rx_sg)
				qeth_create_skb_frag(element, skb, offset,
						     data_len);
			else
				skb_put_data(skb, data_ptr, data_len);
		}
		skb_len -= data_len;
=======
static int qeth_extract_skb(struct qeth_card *card,
			    struct qeth_qdio_buffer *qethbuffer, u8 *element_no,
			    int *__offset)
{
	struct qeth_priv *priv = netdev_priv(card->dev);
	struct qdio_buffer *buffer = qethbuffer->buffer;
	struct napi_struct *napi = &card->napi;
	struct qdio_buffer_element *element;
	unsigned int linear_len = 0;
	bool uses_frags = false;
	int offset = *__offset;
	bool use_rx_sg = false;
	unsigned int headroom;
	struct qeth_hdr *hdr;
	struct sk_buff *skb;
	int skb_len = 0;

	element = &buffer->element[*element_no];

next_packet:
	/* qeth_hdr must not cross element boundaries */
	while (element->length < offset + sizeof(struct qeth_hdr)) {
		if (qeth_is_last_sbale(element))
			return -ENODATA;
		element++;
		offset = 0;
	}

	hdr = phys_to_virt(element->addr) + offset;
	offset += sizeof(*hdr);
	skb = NULL;

	switch (hdr->hdr.l2.id) {
	case QETH_HEADER_TYPE_LAYER2:
		skb_len = hdr->hdr.l2.pkt_length;
		linear_len = ETH_HLEN;
		headroom = 0;
		break;
	case QETH_HEADER_TYPE_LAYER3:
		skb_len = hdr->hdr.l3.length;
		if (!IS_LAYER3(card)) {
			QETH_CARD_STAT_INC(card, rx_dropped_notsupp);
			goto walk_packet;
		}

		if (hdr->hdr.l3.flags & QETH_HDR_PASSTHRU) {
			linear_len = ETH_HLEN;
			headroom = 0;
			break;
		}

		if (hdr->hdr.l3.flags & QETH_HDR_IPV6)
			linear_len = sizeof(struct ipv6hdr);
		else
			linear_len = sizeof(struct iphdr);
		headroom = ETH_HLEN;
		break;
	default:
		if (hdr->hdr.l2.id & QETH_HEADER_MASK_INVAL)
			QETH_CARD_STAT_INC(card, rx_frame_errors);
		else
			QETH_CARD_STAT_INC(card, rx_dropped_notsupp);

		/* Can't determine packet length, drop the whole buffer. */
		return -EPROTONOSUPPORT;
	}

	if (skb_len < linear_len) {
		QETH_CARD_STAT_INC(card, rx_dropped_runt);
		goto walk_packet;
	}

	use_rx_sg = (card->options.cq == QETH_CQ_ENABLED) ||
		    (skb_len > READ_ONCE(priv->rx_copybreak) &&
		     !atomic_read(&card->force_alloc_skb));

	if (use_rx_sg) {
		/* QETH_CQ_ENABLED only: */
		if (qethbuffer->rx_skb &&
		    skb_tailroom(qethbuffer->rx_skb) >= linear_len + headroom) {
			skb = qethbuffer->rx_skb;
			qethbuffer->rx_skb = NULL;
			goto use_skb;
		}

		skb = napi_get_frags(napi);
		if (!skb) {
			/* -ENOMEM, no point in falling back further. */
			QETH_CARD_STAT_INC(card, rx_dropped_nomem);
			goto walk_packet;
		}

		if (skb_tailroom(skb) >= linear_len + headroom) {
			uses_frags = true;
			goto use_skb;
		}

		netdev_info_once(card->dev,
				 "Insufficient linear space in NAPI frags skb, need %u but have %u\n",
				 linear_len + headroom, skb_tailroom(skb));
		/* Shouldn't happen. Don't optimize, fall back to linear skb. */
	}

	linear_len = skb_len;
	skb = napi_alloc_skb(napi, linear_len + headroom);
	if (!skb) {
		QETH_CARD_STAT_INC(card, rx_dropped_nomem);
		goto walk_packet;
	}

use_skb:
	if (headroom)
		skb_reserve(skb, headroom);
walk_packet:
	while (skb_len) {
		int data_len = min(skb_len, (int)(element->length - offset));
		char *data = phys_to_virt(element->addr) + offset;

		skb_len -= data_len;
		offset += data_len;

		/* Extract data from current element: */
		if (skb && data_len) {
			if (linear_len) {
				unsigned int copy_len;

				copy_len = min_t(unsigned int, linear_len,
						 data_len);

				skb_put_data(skb, data, copy_len);
				linear_len -= copy_len;
				data_len -= copy_len;
				data += copy_len;
			}

			if (data_len)
				qeth_create_skb_frag(skb, data, data_len);
		}

		/* Step forward to next element: */
>>>>>>> upstream/android-13
		if (skb_len) {
			if (qeth_is_last_sbale(element)) {
				QETH_CARD_TEXT(card, 4, "unexeob");
				QETH_CARD_HEX(card, 2, buffer, sizeof(void *));
<<<<<<< HEAD
				dev_kfree_skb_any(skb);
				card->stats.rx_errors++;
				return NULL;
			}
			element++;
			offset = 0;
			data_ptr = element->addr;
		} else {
			offset += data_len;
		}
	}
	*__element = element;
	*__offset = offset;
	if (use_rx_sg && card->options.performance_stats) {
		card->perf_stats.sg_skbs_rx++;
		card->perf_stats.sg_frags_rx += skb_shinfo(skb)->nr_frags;
	}
	return skb;
no_mem:
	if (net_ratelimit()) {
		QETH_CARD_TEXT(card, 2, "noskbmem");
	}
	card->stats.rx_dropped++;
	return NULL;
}
EXPORT_SYMBOL_GPL(qeth_core_get_next_skb);

int qeth_poll(struct napi_struct *napi, int budget)
{
	struct qeth_card *card = container_of(napi, struct qeth_card, napi);
	int work_done = 0;
	struct qeth_qdio_buffer *buffer;
	int done;
	int new_budget = budget;

	if (card->options.performance_stats) {
		card->perf_stats.inbound_cnt++;
		card->perf_stats.inbound_start_time = qeth_get_micros();
	}

	while (1) {
		if (!card->rx.b_count) {
			card->rx.qdio_err = 0;
			card->rx.b_count = qdio_get_next_buffers(
				card->data.ccwdev, 0, &card->rx.b_index,
				&card->rx.qdio_err);
=======
				if (skb) {
					if (uses_frags)
						napi_free_frags(napi);
					else
						dev_kfree_skb_any(skb);
					QETH_CARD_STAT_INC(card,
							   rx_length_errors);
				}
				return -EMSGSIZE;
			}
			element++;
			offset = 0;
		}
	}

	/* This packet was skipped, go get another one: */
	if (!skb)
		goto next_packet;

	*element_no = element - &buffer->element[0];
	*__offset = offset;

	qeth_receive_skb(card, skb, hdr, uses_frags);
	return 0;
}

static unsigned int qeth_extract_skbs(struct qeth_card *card, int budget,
				      struct qeth_qdio_buffer *buf, bool *done)
{
	unsigned int work_done = 0;

	while (budget) {
		if (qeth_extract_skb(card, buf, &card->rx.buf_element,
				     &card->rx.e_offset)) {
			*done = true;
			break;
		}

		work_done++;
		budget--;
	}

	return work_done;
}

static unsigned int qeth_rx_poll(struct qeth_card *card, int budget)
{
	struct qeth_rx *ctx = &card->rx;
	unsigned int work_done = 0;

	while (budget > 0) {
		struct qeth_qdio_buffer *buffer;
		unsigned int skbs_done = 0;
		bool done = false;

		/* Fetch completed RX buffers: */
		if (!card->rx.b_count) {
			card->rx.qdio_err = 0;
			card->rx.b_count = qdio_inspect_queue(CARD_DDEV(card),
							      0, true,
							      &card->rx.b_index,
							      &card->rx.qdio_err);
>>>>>>> upstream/android-13
			if (card->rx.b_count <= 0) {
				card->rx.b_count = 0;
				break;
			}
<<<<<<< HEAD
			card->rx.b_element =
				&card->qdio.in_q->bufs[card->rx.b_index]
				.buffer->element[0];
			card->rx.e_offset = 0;
		}

		while (card->rx.b_count) {
			buffer = &card->qdio.in_q->bufs[card->rx.b_index];
			if (!(card->rx.qdio_err &&
			    qeth_check_qdio_errors(card, buffer->buffer,
			    card->rx.qdio_err, "qinerr")))
				work_done +=
					card->discipline->process_rx_buffer(
						card, new_budget, &done);
			else
				done = 1;

			if (done) {
				if (card->options.performance_stats)
					card->perf_stats.bufs_rec++;
				qeth_put_buffer_pool_entry(card,
					buffer->pool_entry);
				qeth_queue_input_buffer(card, card->rx.b_index);
				card->rx.b_count--;
				if (card->rx.b_count) {
					card->rx.b_index =
						(card->rx.b_index + 1) %
						QDIO_MAX_BUFFERS_PER_Q;
					card->rx.b_element =
						&card->qdio.in_q
						->bufs[card->rx.b_index]
						.buffer->element[0];
					card->rx.e_offset = 0;
				}
			}

			if (work_done >= budget)
				goto out;
			else
				new_budget = budget - work_done;
		}
	}

	napi_complete_done(napi, work_done);
	if (qdio_start_irq(card->data.ccwdev, 0))
		napi_schedule(&card->napi);
out:
	if (card->options.performance_stats)
		card->perf_stats.inbound_time += qeth_get_micros() -
			card->perf_stats.inbound_start_time;
=======
		}

		/* Process one completed RX buffer: */
		buffer = &card->qdio.in_q->bufs[card->rx.b_index];
		if (!(card->rx.qdio_err &&
		      qeth_check_qdio_errors(card, buffer->buffer,
					     card->rx.qdio_err, "qinerr")))
			skbs_done = qeth_extract_skbs(card, budget, buffer,
						      &done);
		else
			done = true;

		work_done += skbs_done;
		budget -= skbs_done;

		if (done) {
			QETH_CARD_STAT_INC(card, rx_bufs);
			qeth_put_buffer_pool_entry(card, buffer->pool_entry);
			buffer->pool_entry = NULL;
			card->rx.b_count--;
			ctx->bufs_refill++;
			ctx->bufs_refill -= qeth_rx_refill_queue(card,
								 ctx->bufs_refill);

			/* Step forward to next buffer: */
			card->rx.b_index = QDIO_BUFNR(card->rx.b_index + 1);
			card->rx.buf_element = 0;
			card->rx.e_offset = 0;
		}
	}

	return work_done;
}

static void qeth_cq_poll(struct qeth_card *card)
{
	unsigned int work_done = 0;

	while (work_done < QDIO_MAX_BUFFERS_PER_Q) {
		unsigned int start, error;
		int completed;

		completed = qdio_inspect_queue(CARD_DDEV(card), 1, true, &start,
					       &error);
		if (completed <= 0)
			return;

		qeth_qdio_cq_handler(card, error, 1, start, completed);
		work_done += completed;
	}
}

int qeth_poll(struct napi_struct *napi, int budget)
{
	struct qeth_card *card = container_of(napi, struct qeth_card, napi);
	unsigned int work_done;

	work_done = qeth_rx_poll(card, budget);

	if (qeth_use_tx_irqs(card)) {
		struct qeth_qdio_out_q *queue;
		unsigned int i;

		qeth_for_each_output_queue(card, queue, i) {
			if (!qeth_out_queue_is_empty(queue))
				napi_schedule(&queue->napi);
		}
	}

	if (card->options.cq == QETH_CQ_ENABLED)
		qeth_cq_poll(card);

	if (budget) {
		struct qeth_rx *ctx = &card->rx;

		/* Process any substantial refill backlog: */
		ctx->bufs_refill -= qeth_rx_refill_queue(card, ctx->bufs_refill);

		/* Exhausted the RX budget. Keep IRQ disabled, we get called again. */
		if (work_done >= budget)
			return work_done;
	}

	if (napi_complete_done(napi, work_done) &&
	    qdio_start_irq(CARD_DDEV(card)))
		napi_schedule(napi);

>>>>>>> upstream/android-13
	return work_done;
}
EXPORT_SYMBOL_GPL(qeth_poll);

<<<<<<< HEAD
=======
static void qeth_iqd_tx_complete(struct qeth_qdio_out_q *queue,
				 unsigned int bidx, unsigned int qdio_error,
				 int budget)
{
	struct qeth_qdio_out_buffer *buffer = queue->bufs[bidx];
	u8 sflags = buffer->buffer->element[15].sflags;
	struct qeth_card *card = queue->card;
	bool error = !!qdio_error;

	if (qdio_error == QDIO_ERROR_SLSB_PENDING) {
		struct qaob *aob = buffer->aob;
		struct qeth_qaob_priv1 *priv;
		enum iucv_tx_notify notify;

		if (!aob) {
			netdev_WARN_ONCE(card->dev,
					 "Pending TX buffer %#x without QAOB on TX queue %u\n",
					 bidx, queue->queue_no);
			qeth_schedule_recovery(card);
			return;
		}

		QETH_CARD_TEXT_(card, 5, "pel%u", bidx);

		priv = (struct qeth_qaob_priv1 *)&aob->user1;
		/* QAOB hasn't completed yet: */
		if (xchg(&priv->state, QETH_QAOB_PENDING) != QETH_QAOB_DONE) {
			qeth_notify_skbs(queue, buffer, TX_NOTIFY_PENDING);

			/* Prepare the queue slot for immediate re-use: */
			qeth_scrub_qdio_buffer(buffer->buffer, queue->max_elements);
			if (qeth_alloc_out_buf(queue, bidx, GFP_ATOMIC)) {
				QETH_CARD_TEXT(card, 2, "outofbuf");
				qeth_schedule_recovery(card);
			}

			list_add(&buffer->list_entry, &queue->pending_bufs);
			/* Skip clearing the buffer: */
			return;
		}

		/* QAOB already completed: */
		notify = qeth_compute_cq_notification(aob->aorc, 0);
		qeth_notify_skbs(queue, buffer, notify);
		error = !!aob->aorc;
		memset(aob, 0, sizeof(*aob));
	} else if (card->options.cq == QETH_CQ_ENABLED) {
		qeth_notify_skbs(queue, buffer,
				 qeth_compute_cq_notification(sflags, 0));
	}

	qeth_clear_output_buffer(queue, buffer, error, budget);
}

static int qeth_tx_poll(struct napi_struct *napi, int budget)
{
	struct qeth_qdio_out_q *queue = qeth_napi_to_out_queue(napi);
	unsigned int queue_no = queue->queue_no;
	struct qeth_card *card = queue->card;
	struct net_device *dev = card->dev;
	unsigned int work_done = 0;
	struct netdev_queue *txq;

	if (IS_IQD(card))
		txq = netdev_get_tx_queue(dev, qeth_iqd_translate_txq(dev, queue_no));
	else
		txq = netdev_get_tx_queue(dev, queue_no);

	while (1) {
		unsigned int start, error, i;
		unsigned int packets = 0;
		unsigned int bytes = 0;
		int completed;

		qeth_tx_complete_pending_bufs(card, queue, false, budget);

		if (qeth_out_queue_is_empty(queue)) {
			napi_complete(napi);
			return 0;
		}

		/* Give the CPU a breather: */
		if (work_done >= QDIO_MAX_BUFFERS_PER_Q) {
			QETH_TXQ_STAT_INC(queue, completion_yield);
			if (napi_complete_done(napi, 0))
				napi_schedule(napi);
			return 0;
		}

		completed = qdio_inspect_queue(CARD_DDEV(card), queue_no, false,
					       &start, &error);
		if (completed <= 0) {
			/* Ensure we see TX completion for pending work: */
			if (napi_complete_done(napi, 0) &&
			    !atomic_read(&queue->set_pci_flags_count))
				qeth_tx_arm_timer(queue, queue->rescan_usecs);
			return 0;
		}

		for (i = start; i < start + completed; i++) {
			struct qeth_qdio_out_buffer *buffer;
			unsigned int bidx = QDIO_BUFNR(i);

			buffer = queue->bufs[bidx];
			packets += buffer->frames;
			bytes += buffer->bytes;

			qeth_handle_send_error(card, buffer, error);
			if (IS_IQD(card))
				qeth_iqd_tx_complete(queue, bidx, error, budget);
			else
				qeth_clear_output_buffer(queue, buffer, error,
							 budget);
		}

		atomic_sub(completed, &queue->used_buffers);
		work_done += completed;
		if (IS_IQD(card))
			netdev_tx_completed_queue(txq, packets, bytes);
		else
			qeth_check_outbound_queue(queue);

		/* xmit may have observed the full-condition, but not yet
		 * stopped the txq. In which case the code below won't trigger.
		 * So before returning, xmit will re-check the txq's fill level
		 * and wake it up if needed.
		 */
		if (netif_tx_queue_stopped(txq) &&
		    !qeth_out_queue_is_full(queue))
			netif_tx_wake_queue(txq);
	}
}

>>>>>>> upstream/android-13
static int qeth_setassparms_inspect_rc(struct qeth_ipa_cmd *cmd)
{
	if (!cmd->hdr.return_code)
		cmd->hdr.return_code = cmd->data.setassparms.hdr.return_code;
	return cmd->hdr.return_code;
}

<<<<<<< HEAD
int qeth_setassparms_cb(struct qeth_card *card,
			struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd;

	QETH_CARD_TEXT(card, 4, "defadpcb");

	cmd = (struct qeth_ipa_cmd *) data;
	if (cmd->hdr.return_code == 0) {
		cmd->hdr.return_code = cmd->data.setassparms.hdr.return_code;
		if (cmd->hdr.prot_version == QETH_PROT_IPV4)
			card->options.ipa4.enabled_funcs = cmd->hdr.ipa_enabled;
		if (cmd->hdr.prot_version == QETH_PROT_IPV6)
			card->options.ipa6.enabled_funcs = cmd->hdr.ipa_enabled;
	}
=======
static int qeth_setassparms_get_caps_cb(struct qeth_card *card,
					struct qeth_reply *reply,
					unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_ipa_caps *caps = reply->param;

	if (qeth_setassparms_inspect_rc(cmd))
		return -EIO;

	caps->supported = cmd->data.setassparms.data.caps.supported;
	caps->enabled = cmd->data.setassparms.data.caps.enabled;
	return 0;
}

int qeth_setassparms_cb(struct qeth_card *card,
			struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;

	QETH_CARD_TEXT(card, 4, "defadpcb");

	if (cmd->hdr.return_code)
		return -EIO;

	cmd->hdr.return_code = cmd->data.setassparms.hdr.return_code;
	if (cmd->hdr.prot_version == QETH_PROT_IPV4)
		card->options.ipa4.enabled = cmd->hdr.assists.enabled;
	if (cmd->hdr.prot_version == QETH_PROT_IPV6)
		card->options.ipa6.enabled = cmd->hdr.assists.enabled;
>>>>>>> upstream/android-13
	return 0;
}
EXPORT_SYMBOL_GPL(qeth_setassparms_cb);

struct qeth_cmd_buffer *qeth_get_setassparms_cmd(struct qeth_card *card,
						 enum qeth_ipa_funcs ipa_func,
<<<<<<< HEAD
						 __u16 cmd_code, __u16 len,
						 enum qeth_prot_versions prot)
{
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;

	QETH_CARD_TEXT(card, 4, "getasscm");
	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_SETASSPARMS, prot);

	if (iob) {
		cmd = __ipa_cmd(iob);
		cmd->data.setassparms.hdr.assist_no = ipa_func;
		cmd->data.setassparms.hdr.length = 8 + len;
		cmd->data.setassparms.hdr.command_code = cmd_code;
		cmd->data.setassparms.hdr.return_code = 0;
		cmd->data.setassparms.hdr.seq_no = 0;
	}

=======
						 u16 cmd_code,
						 unsigned int data_length,
						 enum qeth_prot_versions prot)
{
	struct qeth_ipacmd_setassparms *setassparms;
	struct qeth_ipacmd_setassparms_hdr *hdr;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 4, "getasscm");
	iob = qeth_ipa_alloc_cmd(card, IPA_CMD_SETASSPARMS, prot,
				 data_length +
				 offsetof(struct qeth_ipacmd_setassparms,
					  data));
	if (!iob)
		return NULL;

	setassparms = &__ipa_cmd(iob)->data.setassparms;
	setassparms->assist_no = ipa_func;

	hdr = &setassparms->hdr;
	hdr->length = sizeof(*hdr) + data_length;
	hdr->command_code = cmd_code;
>>>>>>> upstream/android-13
	return iob;
}
EXPORT_SYMBOL_GPL(qeth_get_setassparms_cmd);

<<<<<<< HEAD
int qeth_send_setassparms(struct qeth_card *card,
			  struct qeth_cmd_buffer *iob, __u16 len, long data,
			  int (*reply_cb)(struct qeth_card *,
					  struct qeth_reply *, unsigned long),
			  void *reply_param)
{
	int rc;
	struct qeth_ipa_cmd *cmd;

	QETH_CARD_TEXT(card, 4, "sendassp");

	cmd = __ipa_cmd(iob);
	if (len <= sizeof(__u32))
		cmd->data.setassparms.data.flags_32bit = (__u32) data;
	else   /* (len > sizeof(__u32)) */
		memcpy(&cmd->data.setassparms.data, (void *) data, len);

	rc = qeth_send_ipa_cmd(card, iob, reply_cb, reply_param);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_send_setassparms);

int qeth_send_simple_setassparms_prot(struct qeth_card *card,
				      enum qeth_ipa_funcs ipa_func,
				      u16 cmd_code, long data,
				      enum qeth_prot_versions prot)
{
	int rc;
	int length = 0;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT_(card, 4, "simassp%i", prot);
	if (data)
		length = sizeof(__u32);
	iob = qeth_get_setassparms_cmd(card, ipa_func, cmd_code, length, prot);
	if (!iob)
		return -ENOMEM;
	rc = qeth_send_setassparms(card, iob, length, data,
				   qeth_setassparms_cb, NULL);
	return rc;
=======
int qeth_send_simple_setassparms_prot(struct qeth_card *card,
				      enum qeth_ipa_funcs ipa_func,
				      u16 cmd_code, u32 *data,
				      enum qeth_prot_versions prot)
{
	unsigned int length = data ? SETASS_DATA_SIZEOF(flags_32bit) : 0;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT_(card, 4, "simassp%i", prot);
	iob = qeth_get_setassparms_cmd(card, ipa_func, cmd_code, length, prot);
	if (!iob)
		return -ENOMEM;

	if (data)
		__ipa_cmd(iob)->data.setassparms.data.flags_32bit = *data;
	return qeth_send_ipa_cmd(card, iob, qeth_setassparms_cb, NULL);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(qeth_send_simple_setassparms_prot);

static void qeth_unregister_dbf_views(void)
{
	int x;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	for (x = 0; x < QETH_DBF_INFOS; x++) {
		debug_unregister(qeth_dbf[x].id);
		qeth_dbf[x].id = NULL;
	}
}

void qeth_dbf_longtext(debug_info_t *id, int level, char *fmt, ...)
{
	char dbf_txt_buf[32];
	va_list args;

	if (!debug_level_enabled(id, level))
		return;
	va_start(args, fmt);
	vsnprintf(dbf_txt_buf, sizeof(dbf_txt_buf), fmt, args);
	va_end(args);
	debug_text_event(id, level, dbf_txt_buf);
}
EXPORT_SYMBOL_GPL(qeth_dbf_longtext);

static int qeth_register_dbf_views(void)
{
	int ret;
	int x;

	for (x = 0; x < QETH_DBF_INFOS; x++) {
		/* register the areas */
		qeth_dbf[x].id = debug_register(qeth_dbf[x].name,
						qeth_dbf[x].pages,
						qeth_dbf[x].areas,
						qeth_dbf[x].len);
		if (qeth_dbf[x].id == NULL) {
			qeth_unregister_dbf_views();
			return -ENOMEM;
		}

		/* register a view */
		ret = debug_register_view(qeth_dbf[x].id, qeth_dbf[x].view);
		if (ret) {
			qeth_unregister_dbf_views();
			return ret;
		}

		/* set a passing level */
		debug_set_level(qeth_dbf[x].id, qeth_dbf[x].level);
	}

	return 0;
}

<<<<<<< HEAD
int qeth_core_load_discipline(struct qeth_card *card,
		enum qeth_discipline_id discipline)
{
	int rc = 0;
=======
static DEFINE_MUTEX(qeth_mod_mutex);	/* for synchronized module loading */

int qeth_setup_discipline(struct qeth_card *card,
			  enum qeth_discipline_id discipline)
{
	int rc;
>>>>>>> upstream/android-13

	mutex_lock(&qeth_mod_mutex);
	switch (discipline) {
	case QETH_DISCIPLINE_LAYER3:
		card->discipline = try_then_request_module(
			symbol_get(qeth_l3_discipline), "qeth_l3");
		break;
	case QETH_DISCIPLINE_LAYER2:
		card->discipline = try_then_request_module(
			symbol_get(qeth_l2_discipline), "qeth_l2");
		break;
	default:
		break;
	}
<<<<<<< HEAD
=======
	mutex_unlock(&qeth_mod_mutex);
>>>>>>> upstream/android-13

	if (!card->discipline) {
		dev_err(&card->gdev->dev, "There is no kernel module to "
			"support discipline %d\n", discipline);
<<<<<<< HEAD
		rc = -EINVAL;
	}
	mutex_unlock(&qeth_mod_mutex);
	return rc;
}

void qeth_core_free_discipline(struct qeth_card *card)
{
	if (card->options.layer2)
		symbol_put(qeth_l2_discipline);
	else
		symbol_put(qeth_l3_discipline);
	card->discipline = NULL;
}

const struct device_type qeth_generic_devtype = {
	.name = "qeth_generic",
	.groups = qeth_generic_attr_groups,
};
EXPORT_SYMBOL_GPL(qeth_generic_devtype);

static const struct device_type qeth_osn_devtype = {
	.name = "qeth_osn",
	.groups = qeth_osn_attr_groups,
=======
		return -EINVAL;
	}

	rc = card->discipline->setup(card->gdev);
	if (rc) {
		if (discipline == QETH_DISCIPLINE_LAYER2)
			symbol_put(qeth_l2_discipline);
		else
			symbol_put(qeth_l3_discipline);
		card->discipline = NULL;

		return rc;
	}

	card->options.layer = discipline;
	return 0;
}

void qeth_remove_discipline(struct qeth_card *card)
{
	card->discipline->remove(card->gdev);

	if (IS_LAYER2(card))
		symbol_put(qeth_l2_discipline);
	else
		symbol_put(qeth_l3_discipline);
	card->options.layer = QETH_DISCIPLINE_UNDETERMINED;
	card->discipline = NULL;
}

static const struct device_type qeth_generic_devtype = {
	.name = "qeth_generic",
>>>>>>> upstream/android-13
};

#define DBF_NAME_LEN	20

struct qeth_dbf_entry {
	char dbf_name[DBF_NAME_LEN];
	debug_info_t *dbf_info;
	struct list_head dbf_list;
};

static LIST_HEAD(qeth_dbf_list);
static DEFINE_MUTEX(qeth_dbf_list_mutex);

static debug_info_t *qeth_get_dbf_entry(char *name)
{
	struct qeth_dbf_entry *entry;
	debug_info_t *rc = NULL;

	mutex_lock(&qeth_dbf_list_mutex);
	list_for_each_entry(entry, &qeth_dbf_list, dbf_list) {
		if (strcmp(entry->dbf_name, name) == 0) {
			rc = entry->dbf_info;
			break;
		}
	}
	mutex_unlock(&qeth_dbf_list_mutex);
	return rc;
}

static int qeth_add_dbf_entry(struct qeth_card *card, char *name)
{
	struct qeth_dbf_entry *new_entry;

	card->debug = debug_register(name, 2, 1, 8);
	if (!card->debug) {
		QETH_DBF_TEXT_(SETUP, 2, "%s", "qcdbf");
		goto err;
	}
	if (debug_register_view(card->debug, &debug_hex_ascii_view))
		goto err_dbg;
	new_entry = kzalloc(sizeof(struct qeth_dbf_entry), GFP_KERNEL);
	if (!new_entry)
		goto err_dbg;
	strncpy(new_entry->dbf_name, name, DBF_NAME_LEN);
	new_entry->dbf_info = card->debug;
	mutex_lock(&qeth_dbf_list_mutex);
	list_add(&new_entry->dbf_list, &qeth_dbf_list);
	mutex_unlock(&qeth_dbf_list_mutex);

	return 0;

err_dbg:
	debug_unregister(card->debug);
err:
	return -ENOMEM;
}

static void qeth_clear_dbf_list(void)
{
	struct qeth_dbf_entry *entry, *tmp;

	mutex_lock(&qeth_dbf_list_mutex);
	list_for_each_entry_safe(entry, tmp, &qeth_dbf_list, dbf_list) {
		list_del(&entry->dbf_list);
		debug_unregister(entry->dbf_info);
		kfree(entry);
	}
	mutex_unlock(&qeth_dbf_list_mutex);
}

static struct net_device *qeth_alloc_netdev(struct qeth_card *card)
{
	struct net_device *dev;
<<<<<<< HEAD

	switch (card->info.type) {
	case QETH_CARD_TYPE_IQD:
		dev = alloc_netdev(0, "hsi%d", NET_NAME_UNKNOWN, ether_setup);
		break;
	case QETH_CARD_TYPE_OSN:
		dev = alloc_netdev(0, "osn%d", NET_NAME_UNKNOWN, ether_setup);
		break;
	default:
		dev = alloc_etherdev(0);
=======
	struct qeth_priv *priv;

	switch (card->info.type) {
	case QETH_CARD_TYPE_IQD:
		dev = alloc_netdev_mqs(sizeof(*priv), "hsi%d", NET_NAME_UNKNOWN,
				       ether_setup, QETH_MAX_OUT_QUEUES, 1);
		break;
	case QETH_CARD_TYPE_OSM:
		dev = alloc_etherdev(sizeof(*priv));
		break;
	default:
		dev = alloc_etherdev_mqs(sizeof(*priv), QETH_MAX_OUT_QUEUES, 1);
>>>>>>> upstream/android-13
	}

	if (!dev)
		return NULL;

<<<<<<< HEAD
	dev->ml_priv = card;
	dev->watchdog_timeo = QETH_TX_TIMEOUT;
	dev->min_mtu = IS_OSN(card) ? 64 : 576;
=======
	priv = netdev_priv(dev);
	priv->rx_copybreak = QETH_RX_COPYBREAK;
	priv->tx_wanted_queues = IS_IQD(card) ? QETH_IQD_MIN_TXQ : 1;

	dev->ml_priv = card;
	dev->watchdog_timeo = QETH_TX_TIMEOUT;
	dev->min_mtu = 576;
>>>>>>> upstream/android-13
	 /* initialized when device first goes online: */
	dev->max_mtu = 0;
	dev->mtu = 0;
	SET_NETDEV_DEV(dev, &card->gdev->dev);
	netif_carrier_off(dev);

<<<<<<< HEAD
	if (!IS_OSN(card)) {
		dev->priv_flags &= ~IFF_TX_SKB_SHARING;
		dev->hw_features |= NETIF_F_SG;
		dev->vlan_features |= NETIF_F_SG;
		if (IS_IQD(card))
			dev->features |= NETIF_F_SG;
	}
=======
	dev->ethtool_ops = &qeth_ethtool_ops;
	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
	dev->hw_features |= NETIF_F_SG;
	dev->vlan_features |= NETIF_F_SG;
	if (IS_IQD(card))
		dev->features |= NETIF_F_SG;
>>>>>>> upstream/android-13

	return dev;
}

struct net_device *qeth_clone_netdev(struct net_device *orig)
{
	struct net_device *clone = qeth_alloc_netdev(orig->ml_priv);

	if (!clone)
		return NULL;

	clone->dev_port = orig->dev_port;
	return clone;
}

static int qeth_core_probe_device(struct ccwgroup_device *gdev)
{
	struct qeth_card *card;
	struct device *dev;
	int rc;
	enum qeth_discipline_id enforced_disc;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> upstream/android-13
	char dbf_name[DBF_NAME_LEN];

	QETH_DBF_TEXT(SETUP, 2, "probedev");

	dev = &gdev->dev;
	if (!get_device(dev))
		return -ENODEV;

	QETH_DBF_TEXT_(SETUP, 2, "%s", dev_name(&gdev->dev));

	card = qeth_alloc_card(gdev);
	if (!card) {
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", -ENOMEM);
		rc = -ENOMEM;
		goto err_dev;
	}

	snprintf(dbf_name, sizeof(dbf_name), "qeth_card_%s",
		dev_name(&gdev->dev));
	card->debug = qeth_get_dbf_entry(dbf_name);
	if (!card->debug) {
		rc = qeth_add_dbf_entry(card, dbf_name);
		if (rc)
			goto err_card;
	}

<<<<<<< HEAD
	dev_set_drvdata(&gdev->dev, card);
	qeth_setup_card(card);
	rc = qeth_update_from_chp_desc(card);
	if (rc)
		goto err_chp_desc;

=======
	qeth_setup_card(card);
>>>>>>> upstream/android-13
	card->dev = qeth_alloc_netdev(card);
	if (!card->dev) {
		rc = -ENOMEM;
		goto err_card;
	}

	qeth_determine_capabilities(card);
<<<<<<< HEAD
=======
	qeth_set_blkt_defaults(card);

	card->qdio.no_out_queues = card->dev->num_tx_queues;
	rc = qeth_update_from_chp_desc(card);
	if (rc)
		goto err_chp_desc;

	gdev->dev.groups = qeth_dev_groups;

>>>>>>> upstream/android-13
	enforced_disc = qeth_enforce_discipline(card);
	switch (enforced_disc) {
	case QETH_DISCIPLINE_UNDETERMINED:
		gdev->dev.type = &qeth_generic_devtype;
		break;
	default:
		card->info.layer_enforced = true;
<<<<<<< HEAD
		rc = qeth_core_load_discipline(card, enforced_disc);
		if (rc)
			goto err_load;

		gdev->dev.type = (card->info.type != QETH_CARD_TYPE_OSN)
					? card->discipline->devtype
					: &qeth_osn_devtype;
		rc = card->discipline->setup(card->gdev);
		if (rc)
			goto err_disc;
		break;
	}

	write_lock_irqsave(&qeth_core_card_list.rwlock, flags);
	list_add_tail(&card->list, &qeth_core_card_list.list);
	write_unlock_irqrestore(&qeth_core_card_list.rwlock, flags);
	return 0;

err_disc:
	qeth_core_free_discipline(card);
err_load:
	free_netdev(card->dev);
err_chp_desc:
=======
		/* It's so early that we don't need the discipline_mutex yet. */
		rc = qeth_setup_discipline(card, enforced_disc);
		if (rc)
			goto err_setup_disc;

		break;
	}

	return 0;

err_setup_disc:
err_chp_desc:
	free_netdev(card->dev);
>>>>>>> upstream/android-13
err_card:
	qeth_core_free_card(card);
err_dev:
	put_device(dev);
	return rc;
}

static void qeth_core_remove_device(struct ccwgroup_device *gdev)
{
<<<<<<< HEAD
	unsigned long flags;
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);

	QETH_DBF_TEXT(SETUP, 2, "removedv");

	if (card->discipline) {
		card->discipline->remove(gdev);
		qeth_core_free_discipline(card);
	}

	write_lock_irqsave(&qeth_core_card_list.rwlock, flags);
	list_del(&card->list);
	write_unlock_irqrestore(&qeth_core_card_list.rwlock, flags);
	free_netdev(card->dev);
	qeth_core_free_card(card);
	dev_set_drvdata(&gdev->dev, NULL);
=======
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);

	QETH_CARD_TEXT(card, 2, "removedv");

	mutex_lock(&card->discipline_mutex);
	if (card->discipline)
		qeth_remove_discipline(card);
	mutex_unlock(&card->discipline_mutex);

	qeth_free_qdio_queues(card);

	free_netdev(card->dev);
	qeth_core_free_card(card);
>>>>>>> upstream/android-13
	put_device(&gdev->dev);
}

static int qeth_core_set_online(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	int rc = 0;
	enum qeth_discipline_id def_discipline;

<<<<<<< HEAD
	if (!card->discipline) {
		if (card->info.type == QETH_CARD_TYPE_IQD)
			def_discipline = QETH_DISCIPLINE_LAYER3;
		else
			def_discipline = QETH_DISCIPLINE_LAYER2;
		rc = qeth_core_load_discipline(card, def_discipline);
		if (rc)
			goto err;
		rc = card->discipline->setup(card->gdev);
		if (rc) {
			qeth_core_free_discipline(card);
			goto err;
		}
	}
	rc = card->discipline->set_online(gdev);
err:
=======
	mutex_lock(&card->discipline_mutex);
	if (!card->discipline) {
		def_discipline = IS_IQD(card) ? QETH_DISCIPLINE_LAYER3 :
						QETH_DISCIPLINE_LAYER2;
		rc = qeth_setup_discipline(card, def_discipline);
		if (rc)
			goto err;
	}

	rc = qeth_set_online(card, card->discipline);

err:
	mutex_unlock(&card->discipline_mutex);
>>>>>>> upstream/android-13
	return rc;
}

static int qeth_core_set_offline(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
<<<<<<< HEAD
	return card->discipline->set_offline(gdev);
=======
	int rc;

	mutex_lock(&card->discipline_mutex);
	rc = qeth_set_offline(card, card->discipline, false);
	mutex_unlock(&card->discipline_mutex);

	return rc;
>>>>>>> upstream/android-13
}

static void qeth_core_shutdown(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	qeth_set_allowed_threads(card, 0, 1);
	if ((gdev->state == CCWGROUP_ONLINE) && card->info.hwtrap)
		qeth_hw_trap(card, QETH_DIAGS_TRAP_DISARM);
	qeth_qdio_clear_card(card, 0);
<<<<<<< HEAD
	qeth_clear_qdio_buffers(card);
	qdio_free(CARD_DDEV(card));
}

static int qeth_core_freeze(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	if (card->discipline && card->discipline->freeze)
		return card->discipline->freeze(gdev);
	return 0;
}

static int qeth_core_thaw(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	if (card->discipline && card->discipline->thaw)
		return card->discipline->thaw(gdev);
	return 0;
}

static int qeth_core_restore(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	if (card->discipline && card->discipline->restore)
		return card->discipline->restore(gdev);
	return 0;
}

=======
	qeth_drain_output_queues(card);
	qdio_free(CARD_DDEV(card));
}

>>>>>>> upstream/android-13
static ssize_t group_store(struct device_driver *ddrv, const char *buf,
			   size_t count)
{
	int err;

	err = ccwgroup_create_dev(qeth_core_root_dev, to_ccwgroupdrv(ddrv), 3,
				  buf);

	return err ? err : count;
}
static DRIVER_ATTR_WO(group);

static struct attribute *qeth_drv_attrs[] = {
	&driver_attr_group.attr,
	NULL,
};
static struct attribute_group qeth_drv_attr_group = {
	.attrs = qeth_drv_attrs,
};
static const struct attribute_group *qeth_drv_attr_groups[] = {
	&qeth_drv_attr_group,
	NULL,
};

static struct ccwgroup_driver qeth_core_ccwgroup_driver = {
	.driver = {
		.groups = qeth_drv_attr_groups,
		.owner = THIS_MODULE,
		.name = "qeth",
	},
	.ccw_driver = &qeth_ccw_driver,
	.setup = qeth_core_probe_device,
	.remove = qeth_core_remove_device,
	.set_online = qeth_core_set_online,
	.set_offline = qeth_core_set_offline,
	.shutdown = qeth_core_shutdown,
<<<<<<< HEAD
	.prepare = NULL,
	.complete = NULL,
	.freeze = qeth_core_freeze,
	.thaw = qeth_core_thaw,
	.restore = qeth_core_restore,
};

=======
};

int qeth_siocdevprivate(struct net_device *dev, struct ifreq *rq, void __user *data, int cmd)
{
	struct qeth_card *card = dev->ml_priv;
	int rc = 0;

	switch (cmd) {
	case SIOC_QETH_ADP_SET_SNMP_CONTROL:
		rc = qeth_snmp_command(card, data);
		break;
	case SIOC_QETH_GET_CARD_TYPE:
		if ((IS_OSD(card) || IS_OSM(card) || IS_OSX(card)) &&
		    !IS_VM_NIC(card))
			return 1;
		return 0;
	case SIOC_QETH_QUERY_OAT:
		rc = qeth_query_oat_command(card, data);
		break;
	default:
		if (card->discipline->do_ioctl)
			rc = card->discipline->do_ioctl(dev, rq, data, cmd);
		else
			rc = -EOPNOTSUPP;
	}
	if (rc)
		QETH_CARD_TEXT_(card, 2, "ioce%x", rc);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_siocdevprivate);

>>>>>>> upstream/android-13
int qeth_do_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct qeth_card *card = dev->ml_priv;
	struct mii_ioctl_data *mii_data;
	int rc = 0;

<<<<<<< HEAD
	if (!card)
		return -ENODEV;

	if (!qeth_card_hw_is_reachable(card))
		return -ENODEV;

	if (card->info.type == QETH_CARD_TYPE_OSN)
		return -EPERM;

	switch (cmd) {
	case SIOC_QETH_ADP_SET_SNMP_CONTROL:
		rc = qeth_snmp_command(card, rq->ifr_ifru.ifru_data);
		break;
	case SIOC_QETH_GET_CARD_TYPE:
		if ((card->info.type == QETH_CARD_TYPE_OSD ||
		     card->info.type == QETH_CARD_TYPE_OSM ||
		     card->info.type == QETH_CARD_TYPE_OSX) &&
		    !card->info.guestlan)
			return 1;
		else
			return 0;
=======
	switch (cmd) {
>>>>>>> upstream/android-13
	case SIOCGMIIPHY:
		mii_data = if_mii(rq);
		mii_data->phy_id = 0;
		break;
	case SIOCGMIIREG:
		mii_data = if_mii(rq);
		if (mii_data->phy_id != 0)
			rc = -EINVAL;
		else
			mii_data->val_out = qeth_mdio_read(dev,
				mii_data->phy_id, mii_data->reg_num);
		break;
<<<<<<< HEAD
	case SIOC_QETH_QUERY_OAT:
		rc = qeth_query_oat_command(card, rq->ifr_ifru.ifru_data);
		break;
	default:
		if (card->discipline->do_ioctl)
			rc = card->discipline->do_ioctl(dev, rq, cmd);
		else
			rc = -EOPNOTSUPP;
=======
	default:
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13
	}
	if (rc)
		QETH_CARD_TEXT_(card, 2, "ioce%x", rc);
	return rc;
}
EXPORT_SYMBOL_GPL(qeth_do_ioctl);

<<<<<<< HEAD
static struct {
	const char str[ETH_GSTRING_LEN];
} qeth_ethtool_stats_keys[] = {
/*  0 */{"rx skbs"},
	{"rx buffers"},
	{"tx skbs"},
	{"tx buffers"},
	{"tx skbs no packing"},
	{"tx buffers no packing"},
	{"tx skbs packing"},
	{"tx buffers packing"},
	{"tx sg skbs"},
	{"tx buffer elements"},
/* 10 */{"rx sg skbs"},
	{"rx sg frags"},
	{"rx sg page allocs"},
	{"tx large kbytes"},
	{"tx large count"},
	{"tx pk state ch n->p"},
	{"tx pk state ch p->n"},
	{"tx pk watermark low"},
	{"tx pk watermark high"},
	{"queue 0 buffer usage"},
/* 20 */{"queue 1 buffer usage"},
	{"queue 2 buffer usage"},
	{"queue 3 buffer usage"},
	{"rx poll time"},
	{"rx poll count"},
	{"rx do_QDIO time"},
	{"rx do_QDIO count"},
	{"tx handler time"},
	{"tx handler count"},
	{"tx time"},
/* 30 */{"tx count"},
	{"tx do_QDIO time"},
	{"tx do_QDIO count"},
	{"tx csum"},
	{"tx lin"},
	{"tx linfail"},
	{"cq handler count"},
	{"cq handler time"},
	{"rx csum"}
};

int qeth_core_get_sset_count(struct net_device *dev, int stringset)
{
	switch (stringset) {
	case ETH_SS_STATS:
		return (sizeof(qeth_ethtool_stats_keys) / ETH_GSTRING_LEN);
	default:
		return -EINVAL;
	}
}
EXPORT_SYMBOL_GPL(qeth_core_get_sset_count);

void qeth_core_get_ethtool_stats(struct net_device *dev,
		struct ethtool_stats *stats, u64 *data)
{
	struct qeth_card *card = dev->ml_priv;
	data[0] = card->stats.rx_packets -
				card->perf_stats.initial_rx_packets;
	data[1] = card->perf_stats.bufs_rec;
	data[2] = card->stats.tx_packets -
				card->perf_stats.initial_tx_packets;
	data[3] = card->perf_stats.bufs_sent;
	data[4] = card->stats.tx_packets - card->perf_stats.initial_tx_packets
			- card->perf_stats.skbs_sent_pack;
	data[5] = card->perf_stats.bufs_sent - card->perf_stats.bufs_sent_pack;
	data[6] = card->perf_stats.skbs_sent_pack;
	data[7] = card->perf_stats.bufs_sent_pack;
	data[8] = card->perf_stats.sg_skbs_sent;
	data[9] = card->perf_stats.buf_elements_sent;
	data[10] = card->perf_stats.sg_skbs_rx;
	data[11] = card->perf_stats.sg_frags_rx;
	data[12] = card->perf_stats.sg_alloc_page_rx;
	data[13] = (card->perf_stats.large_send_bytes >> 10);
	data[14] = card->perf_stats.large_send_cnt;
	data[15] = card->perf_stats.sc_dp_p;
	data[16] = card->perf_stats.sc_p_dp;
	data[17] = QETH_LOW_WATERMARK_PACK;
	data[18] = QETH_HIGH_WATERMARK_PACK;
	data[19] = atomic_read(&card->qdio.out_qs[0]->used_buffers);
	data[20] = (card->qdio.no_out_queues > 1) ?
			atomic_read(&card->qdio.out_qs[1]->used_buffers) : 0;
	data[21] = (card->qdio.no_out_queues > 2) ?
			atomic_read(&card->qdio.out_qs[2]->used_buffers) : 0;
	data[22] = (card->qdio.no_out_queues > 3) ?
			atomic_read(&card->qdio.out_qs[3]->used_buffers) : 0;
	data[23] = card->perf_stats.inbound_time;
	data[24] = card->perf_stats.inbound_cnt;
	data[25] = card->perf_stats.inbound_do_qdio_time;
	data[26] = card->perf_stats.inbound_do_qdio_cnt;
	data[27] = card->perf_stats.outbound_handler_time;
	data[28] = card->perf_stats.outbound_handler_cnt;
	data[29] = card->perf_stats.outbound_time;
	data[30] = card->perf_stats.outbound_cnt;
	data[31] = card->perf_stats.outbound_do_qdio_time;
	data[32] = card->perf_stats.outbound_do_qdio_cnt;
	data[33] = card->perf_stats.tx_csum;
	data[34] = card->perf_stats.tx_lin;
	data[35] = card->perf_stats.tx_linfail;
	data[36] = card->perf_stats.cq_cnt;
	data[37] = card->perf_stats.cq_time;
	data[38] = card->perf_stats.rx_csum;
}
EXPORT_SYMBOL_GPL(qeth_core_get_ethtool_stats);

void qeth_core_get_strings(struct net_device *dev, u32 stringset, u8 *data)
{
	switch (stringset) {
	case ETH_SS_STATS:
		memcpy(data, &qeth_ethtool_stats_keys,
			sizeof(qeth_ethtool_stats_keys));
		break;
	default:
		WARN_ON(1);
		break;
	}
}
EXPORT_SYMBOL_GPL(qeth_core_get_strings);

void qeth_core_get_drvinfo(struct net_device *dev,
		struct ethtool_drvinfo *info)
{
	struct qeth_card *card = dev->ml_priv;

	strlcpy(info->driver, card->options.layer2 ? "qeth_l2" : "qeth_l3",
		sizeof(info->driver));
	strlcpy(info->version, "1.0", sizeof(info->version));
	strlcpy(info->fw_version, card->info.mcl_level,
		sizeof(info->fw_version));
	snprintf(info->bus_info, sizeof(info->bus_info), "%s/%s/%s",
		 CARD_RDEV_ID(card), CARD_WDEV_ID(card), CARD_DDEV_ID(card));
}
EXPORT_SYMBOL_GPL(qeth_core_get_drvinfo);

/* Helper function to fill 'advertising' and 'supported' which are the same. */
/* Autoneg and full-duplex are supported and advertised unconditionally.     */
/* Always advertise and support all speeds up to specified, and only one     */
/* specified port type.							     */
static void qeth_set_cmd_adv_sup(struct ethtool_link_ksettings *cmd,
				int maxspeed, int porttype)
{
	ethtool_link_ksettings_zero_link_mode(cmd, supported);
	ethtool_link_ksettings_zero_link_mode(cmd, advertising);
	ethtool_link_ksettings_zero_link_mode(cmd, lp_advertising);

	ethtool_link_ksettings_add_link_mode(cmd, supported, Autoneg);
	ethtool_link_ksettings_add_link_mode(cmd, advertising, Autoneg);

	switch (porttype) {
	case PORT_TP:
		ethtool_link_ksettings_add_link_mode(cmd, supported, TP);
		ethtool_link_ksettings_add_link_mode(cmd, advertising, TP);
		break;
	case PORT_FIBRE:
		ethtool_link_ksettings_add_link_mode(cmd, supported, FIBRE);
		ethtool_link_ksettings_add_link_mode(cmd, advertising, FIBRE);
		break;
	default:
		ethtool_link_ksettings_add_link_mode(cmd, supported, TP);
		ethtool_link_ksettings_add_link_mode(cmd, advertising, TP);
		WARN_ON_ONCE(1);
	}

	/* fallthrough from high to low, to select all legal speeds: */
	switch (maxspeed) {
	case SPEED_10000:
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     10000baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     10000baseT_Full);
	case SPEED_1000:
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     1000baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     1000baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     1000baseT_Half);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     1000baseT_Half);
	case SPEED_100:
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     100baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     100baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     100baseT_Half);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     100baseT_Half);
	case SPEED_10:
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     10baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     10baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     10baseT_Half);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     10baseT_Half);
		/* end fallthrough */
		break;
	default:
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     10baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     10baseT_Full);
		ethtool_link_ksettings_add_link_mode(cmd, supported,
						     10baseT_Half);
		ethtool_link_ksettings_add_link_mode(cmd, advertising,
						     10baseT_Half);
		WARN_ON_ONCE(1);
	}
}

int qeth_core_ethtool_get_link_ksettings(struct net_device *netdev,
		struct ethtool_link_ksettings *cmd)
{
	struct qeth_card *card = netdev->ml_priv;
	enum qeth_link_types link_type;
	struct carrier_info carrier_info;
	int rc;

	if ((card->info.type == QETH_CARD_TYPE_IQD) || (card->info.guestlan))
		link_type = QETH_LINK_TYPE_10GBIT_ETH;
	else
		link_type = card->info.link_type;

	cmd->base.duplex = DUPLEX_FULL;
	cmd->base.autoneg = AUTONEG_ENABLE;
	cmd->base.phy_address = 0;
	cmd->base.mdio_support = 0;
	cmd->base.eth_tp_mdix = ETH_TP_MDI_INVALID;
	cmd->base.eth_tp_mdix_ctrl = ETH_TP_MDI_INVALID;

	switch (link_type) {
	case QETH_LINK_TYPE_FAST_ETH:
	case QETH_LINK_TYPE_LANE_ETH100:
		cmd->base.speed = SPEED_100;
		cmd->base.port = PORT_TP;
		break;
	case QETH_LINK_TYPE_GBIT_ETH:
	case QETH_LINK_TYPE_LANE_ETH1000:
		cmd->base.speed = SPEED_1000;
		cmd->base.port = PORT_FIBRE;
		break;
	case QETH_LINK_TYPE_10GBIT_ETH:
		cmd->base.speed = SPEED_10000;
		cmd->base.port = PORT_FIBRE;
		break;
	default:
		cmd->base.speed = SPEED_10;
		cmd->base.port = PORT_TP;
	}
	qeth_set_cmd_adv_sup(cmd, cmd->base.speed, cmd->base.port);

	/* Check if we can obtain more accurate information.	 */
	/* If QUERY_CARD_INFO command is not supported or fails, */
	/* just return the heuristics that was filled above.	 */
	if (!qeth_card_hw_is_reachable(card))
		return -ENODEV;
	rc = qeth_query_card_info(card, &carrier_info);
	if (rc == -EOPNOTSUPP) /* for old hardware, return heuristic */
		return 0;
	if (rc) /* report error from the hardware operation */
		return rc;
	/* on success, fill in the information got from the hardware */

	netdev_dbg(netdev,
	"card info: card_type=0x%02x, port_mode=0x%04x, port_speed=0x%08x\n",
			carrier_info.card_type,
			carrier_info.port_mode,
			carrier_info.port_speed);

	/* Update attributes for which we've obtained more authoritative */
	/* information, leave the rest the way they where filled above.  */
	switch (carrier_info.card_type) {
	case CARD_INFO_TYPE_1G_COPPER_A:
	case CARD_INFO_TYPE_1G_COPPER_B:
		cmd->base.port = PORT_TP;
		qeth_set_cmd_adv_sup(cmd, SPEED_1000, cmd->base.port);
		break;
	case CARD_INFO_TYPE_1G_FIBRE_A:
	case CARD_INFO_TYPE_1G_FIBRE_B:
		cmd->base.port = PORT_FIBRE;
		qeth_set_cmd_adv_sup(cmd, SPEED_1000, cmd->base.port);
		break;
	case CARD_INFO_TYPE_10G_FIBRE_A:
	case CARD_INFO_TYPE_10G_FIBRE_B:
		cmd->base.port = PORT_FIBRE;
		qeth_set_cmd_adv_sup(cmd, SPEED_10000, cmd->base.port);
		break;
	}

	switch (carrier_info.port_mode) {
	case CARD_INFO_PORTM_FULLDUPLEX:
		cmd->base.duplex = DUPLEX_FULL;
		break;
	case CARD_INFO_PORTM_HALFDUPLEX:
		cmd->base.duplex = DUPLEX_HALF;
		break;
	}

	switch (carrier_info.port_speed) {
	case CARD_INFO_PORTS_10M:
		cmd->base.speed = SPEED_10;
		break;
	case CARD_INFO_PORTS_100M:
		cmd->base.speed = SPEED_100;
		break;
	case CARD_INFO_PORTS_1G:
		cmd->base.speed = SPEED_1000;
		break;
	case CARD_INFO_PORTS_10G:
		cmd->base.speed = SPEED_10000;
		break;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(qeth_core_ethtool_get_link_ksettings);

/* Callback to handle checksum offload command reply from OSA card.
 * Verify that required features have been enabled on the card.
 * Return error in hdr->return_code as this value is checked by caller.
 *
 * Always returns zero to indicate no further messages from the OSA card.
 */
static int qeth_ipa_checksum_run_cmd_cb(struct qeth_card *card,
					struct qeth_reply *reply,
					unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_checksum_cmd *chksum_cb =
				(struct qeth_checksum_cmd *)reply->param;

	QETH_CARD_TEXT(card, 4, "chkdoccb");
	if (qeth_setassparms_inspect_rc(cmd))
		return 0;

	memset(chksum_cb, 0, sizeof(*chksum_cb));
	if (cmd->data.setassparms.hdr.command_code == IPA_CMD_ASS_START) {
		chksum_cb->supported =
				cmd->data.setassparms.data.chksum.supported;
		QETH_CARD_TEXT_(card, 3, "strt:%x", chksum_cb->supported);
	}
	if (cmd->data.setassparms.hdr.command_code == IPA_CMD_ASS_ENABLE) {
		chksum_cb->supported =
				cmd->data.setassparms.data.chksum.supported;
		chksum_cb->enabled =
				cmd->data.setassparms.data.chksum.enabled;
		QETH_CARD_TEXT_(card, 3, "supp:%x", chksum_cb->supported);
		QETH_CARD_TEXT_(card, 3, "enab:%x", chksum_cb->enabled);
	}
	return 0;
}

/* Send command to OSA card and check results. */
static int qeth_ipa_checksum_run_cmd(struct qeth_card *card,
				     enum qeth_ipa_funcs ipa_func,
				     __u16 cmd_code, long data,
				     struct qeth_checksum_cmd *chksum_cb,
				     enum qeth_prot_versions prot)
{
	struct qeth_cmd_buffer *iob;
	int rc = -ENOMEM;

	QETH_CARD_TEXT(card, 4, "chkdocmd");
	iob = qeth_get_setassparms_cmd(card, ipa_func, cmd_code,
				       sizeof(__u32), prot);
	if (iob)
		rc = qeth_send_setassparms(card, iob, sizeof(__u32), data,
					   qeth_ipa_checksum_run_cmd_cb,
					   chksum_cb);
	return rc;
}

static int qeth_send_checksum_on(struct qeth_card *card, int cstype,
				 enum qeth_prot_versions prot)
{
	u32 required_features = QETH_IPA_CHECKSUM_UDP | QETH_IPA_CHECKSUM_TCP;
	struct qeth_checksum_cmd chksum_cb;
	int rc;

	if (prot == QETH_PROT_IPV4)
		required_features |= QETH_IPA_CHECKSUM_IP_HDR;
	rc = qeth_ipa_checksum_run_cmd(card, cstype, IPA_CMD_ASS_START, 0,
				       &chksum_cb, prot);
	if (!rc) {
		if ((required_features & chksum_cb.supported) !=
		    required_features)
			rc = -EIO;
		else if (!(QETH_IPA_CHECKSUM_LP2LP & chksum_cb.supported) &&
			 cstype == IPA_INBOUND_CHECKSUM)
			dev_warn(&card->gdev->dev,
				 "Hardware checksumming is performed only if %s and its peer use different OSA Express 3 ports\n",
				 QETH_CARD_IFNAME(card));
	}
	if (rc) {
		qeth_send_simple_setassparms_prot(card, cstype,
						  IPA_CMD_ASS_STOP, 0, prot);
		dev_warn(&card->gdev->dev,
			 "Starting HW IPv%d checksumming for %s failed, using SW checksumming\n",
			 prot, QETH_CARD_IFNAME(card));
		return rc;
	}
	rc = qeth_ipa_checksum_run_cmd(card, cstype, IPA_CMD_ASS_ENABLE,
				       chksum_cb.supported, &chksum_cb,
				       prot);
	if (!rc) {
		if ((required_features & chksum_cb.enabled) !=
		    required_features)
			rc = -EIO;
	}
	if (rc) {
		qeth_send_simple_setassparms_prot(card, cstype,
						  IPA_CMD_ASS_STOP, 0, prot);
		dev_warn(&card->gdev->dev,
			 "Enabling HW IPv%d checksumming for %s failed, using SW checksumming\n",
			 prot, QETH_CARD_IFNAME(card));
		return rc;
=======
static int qeth_start_csum_cb(struct qeth_card *card, struct qeth_reply *reply,
			      unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	u32 *features = reply->param;

	if (qeth_setassparms_inspect_rc(cmd))
		return -EIO;

	*features = cmd->data.setassparms.data.flags_32bit;
	return 0;
}

static int qeth_set_csum_off(struct qeth_card *card, enum qeth_ipa_funcs cstype,
			     enum qeth_prot_versions prot)
{
	return qeth_send_simple_setassparms_prot(card, cstype, IPA_CMD_ASS_STOP,
						 NULL, prot);
}

static int qeth_set_csum_on(struct qeth_card *card, enum qeth_ipa_funcs cstype,
			    enum qeth_prot_versions prot, u8 *lp2lp)
{
	u32 required_features = QETH_IPA_CHECKSUM_UDP | QETH_IPA_CHECKSUM_TCP;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_caps caps;
	u32 features;
	int rc;

	/* some L3 HW requires combined L3+L4 csum offload: */
	if (IS_LAYER3(card) && prot == QETH_PROT_IPV4 &&
	    cstype == IPA_OUTBOUND_CHECKSUM)
		required_features |= QETH_IPA_CHECKSUM_IP_HDR;

	iob = qeth_get_setassparms_cmd(card, cstype, IPA_CMD_ASS_START, 0,
				       prot);
	if (!iob)
		return -ENOMEM;

	rc = qeth_send_ipa_cmd(card, iob, qeth_start_csum_cb, &features);
	if (rc)
		return rc;

	if ((required_features & features) != required_features) {
		qeth_set_csum_off(card, cstype, prot);
		return -EOPNOTSUPP;
	}

	iob = qeth_get_setassparms_cmd(card, cstype, IPA_CMD_ASS_ENABLE,
				       SETASS_DATA_SIZEOF(flags_32bit),
				       prot);
	if (!iob) {
		qeth_set_csum_off(card, cstype, prot);
		return -ENOMEM;
	}

	if (features & QETH_IPA_CHECKSUM_LP2LP)
		required_features |= QETH_IPA_CHECKSUM_LP2LP;
	__ipa_cmd(iob)->data.setassparms.data.flags_32bit = required_features;
	rc = qeth_send_ipa_cmd(card, iob, qeth_setassparms_get_caps_cb, &caps);
	if (rc) {
		qeth_set_csum_off(card, cstype, prot);
		return rc;
	}

	if (!qeth_ipa_caps_supported(&caps, required_features) ||
	    !qeth_ipa_caps_enabled(&caps, required_features)) {
		qeth_set_csum_off(card, cstype, prot);
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13
	}

	dev_info(&card->gdev->dev, "HW Checksumming (%sbound IPv%d) enabled\n",
		 cstype == IPA_INBOUND_CHECKSUM ? "in" : "out", prot);
<<<<<<< HEAD
=======

	if (lp2lp)
		*lp2lp = qeth_ipa_caps_enabled(&caps, QETH_IPA_CHECKSUM_LP2LP);

>>>>>>> upstream/android-13
	return 0;
}

static int qeth_set_ipa_csum(struct qeth_card *card, bool on, int cstype,
<<<<<<< HEAD
			     enum qeth_prot_versions prot)
{
	int rc = (on) ? qeth_send_checksum_on(card, cstype, prot)
		      : qeth_send_simple_setassparms_prot(card, cstype,
							  IPA_CMD_ASS_STOP, 0,
							  prot);
	return rc ? -EIO : 0;
}

static int qeth_set_ipa_tso(struct qeth_card *card, int on)
{
	int rc;

	QETH_CARD_TEXT(card, 3, "sttso");

	if (on) {
		rc = qeth_send_simple_setassparms(card, IPA_OUTBOUND_TSO,
						  IPA_CMD_ASS_START, 0);
		if (rc) {
			dev_warn(&card->gdev->dev,
				 "Starting outbound TCP segmentation offload for %s failed\n",
				 QETH_CARD_IFNAME(card));
			return -EIO;
		}
		dev_info(&card->gdev->dev, "Outbound TSO enabled\n");
	} else {
		rc = qeth_send_simple_setassparms(card, IPA_OUTBOUND_TSO,
						  IPA_CMD_ASS_STOP, 0);
	}
	return rc;
=======
			     enum qeth_prot_versions prot, u8 *lp2lp)
{
	return on ? qeth_set_csum_on(card, cstype, prot, lp2lp) :
		    qeth_set_csum_off(card, cstype, prot);
}

static int qeth_start_tso_cb(struct qeth_card *card, struct qeth_reply *reply,
			     unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_tso_start_data *tso_data = reply->param;

	if (qeth_setassparms_inspect_rc(cmd))
		return -EIO;

	tso_data->mss = cmd->data.setassparms.data.tso.mss;
	tso_data->supported = cmd->data.setassparms.data.tso.supported;
	return 0;
}

static int qeth_set_tso_off(struct qeth_card *card,
			    enum qeth_prot_versions prot)
{
	return qeth_send_simple_setassparms_prot(card, IPA_OUTBOUND_TSO,
						 IPA_CMD_ASS_STOP, NULL, prot);
}

static int qeth_set_tso_on(struct qeth_card *card,
			   enum qeth_prot_versions prot)
{
	struct qeth_tso_start_data tso_data;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_caps caps;
	int rc;

	iob = qeth_get_setassparms_cmd(card, IPA_OUTBOUND_TSO,
				       IPA_CMD_ASS_START, 0, prot);
	if (!iob)
		return -ENOMEM;

	rc = qeth_send_ipa_cmd(card, iob, qeth_start_tso_cb, &tso_data);
	if (rc)
		return rc;

	if (!tso_data.mss || !(tso_data.supported & QETH_IPA_LARGE_SEND_TCP)) {
		qeth_set_tso_off(card, prot);
		return -EOPNOTSUPP;
	}

	iob = qeth_get_setassparms_cmd(card, IPA_OUTBOUND_TSO,
				       IPA_CMD_ASS_ENABLE,
				       SETASS_DATA_SIZEOF(caps), prot);
	if (!iob) {
		qeth_set_tso_off(card, prot);
		return -ENOMEM;
	}

	/* enable TSO capability */
	__ipa_cmd(iob)->data.setassparms.data.caps.enabled =
		QETH_IPA_LARGE_SEND_TCP;
	rc = qeth_send_ipa_cmd(card, iob, qeth_setassparms_get_caps_cb, &caps);
	if (rc) {
		qeth_set_tso_off(card, prot);
		return rc;
	}

	if (!qeth_ipa_caps_supported(&caps, QETH_IPA_LARGE_SEND_TCP) ||
	    !qeth_ipa_caps_enabled(&caps, QETH_IPA_LARGE_SEND_TCP)) {
		qeth_set_tso_off(card, prot);
		return -EOPNOTSUPP;
	}

	dev_info(&card->gdev->dev, "TSOv%u enabled (MSS: %u)\n", prot,
		 tso_data.mss);
	return 0;
}

static int qeth_set_ipa_tso(struct qeth_card *card, bool on,
			    enum qeth_prot_versions prot)
{
	return on ? qeth_set_tso_on(card, prot) : qeth_set_tso_off(card, prot);
>>>>>>> upstream/android-13
}

static int qeth_set_ipa_rx_csum(struct qeth_card *card, bool on)
{
	int rc_ipv4 = (on) ? -EOPNOTSUPP : 0;
	int rc_ipv6;

	if (qeth_is_supported(card, IPA_INBOUND_CHECKSUM))
		rc_ipv4 = qeth_set_ipa_csum(card, on, IPA_INBOUND_CHECKSUM,
<<<<<<< HEAD
					    QETH_PROT_IPV4);
=======
					    QETH_PROT_IPV4, NULL);
>>>>>>> upstream/android-13
	if (!qeth_is_supported6(card, IPA_INBOUND_CHECKSUM_V6))
		/* no/one Offload Assist available, so the rc is trivial */
		return rc_ipv4;

	rc_ipv6 = qeth_set_ipa_csum(card, on, IPA_INBOUND_CHECKSUM,
<<<<<<< HEAD
				    QETH_PROT_IPV6);
=======
				    QETH_PROT_IPV6, NULL);
>>>>>>> upstream/android-13

	if (on)
		/* enable: success if any Assist is active */
		return (rc_ipv6) ? rc_ipv4 : 0;

	/* disable: failure if any Assist is still active */
	return (rc_ipv6) ? rc_ipv6 : rc_ipv4;
}

<<<<<<< HEAD
#define QETH_HW_FEATURES (NETIF_F_RXCSUM | NETIF_F_IP_CSUM | NETIF_F_TSO | \
			  NETIF_F_IPV6_CSUM)
=======
>>>>>>> upstream/android-13
/**
 * qeth_enable_hw_features() - (Re-)Enable HW functions for device features
 * @dev:	a net_device
 */
void qeth_enable_hw_features(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;
	netdev_features_t features;

<<<<<<< HEAD
	rtnl_lock();
	features = dev->features;
	/* force-off any feature that needs an IPA sequence.
	 * netdev_update_features() will restart them.
	 */
	dev->features &= ~QETH_HW_FEATURES;
=======
	features = dev->features;
	/* force-off any feature that might need an IPA sequence.
	 * netdev_update_features() will restart them.
	 */
	dev->features &= ~dev->hw_features;
	/* toggle VLAN filter, so that VIDs are re-programmed: */
	if (IS_LAYER2(card) && IS_VM_NIC(card)) {
		dev->features &= ~NETIF_F_HW_VLAN_CTAG_FILTER;
		dev->wanted_features |= NETIF_F_HW_VLAN_CTAG_FILTER;
	}
>>>>>>> upstream/android-13
	netdev_update_features(dev);
	if (features != dev->features)
		dev_warn(&card->gdev->dev,
			 "Device recovery failed to restore all offload features\n");
<<<<<<< HEAD
	rtnl_unlock();
}
EXPORT_SYMBOL_GPL(qeth_enable_hw_features);

=======
}
EXPORT_SYMBOL_GPL(qeth_enable_hw_features);

static void qeth_check_restricted_features(struct qeth_card *card,
					   netdev_features_t changed,
					   netdev_features_t actual)
{
	netdev_features_t ipv6_features = NETIF_F_TSO6;
	netdev_features_t ipv4_features = NETIF_F_TSO;

	if (!card->info.has_lp2lp_cso_v6)
		ipv6_features |= NETIF_F_IPV6_CSUM;
	if (!card->info.has_lp2lp_cso_v4)
		ipv4_features |= NETIF_F_IP_CSUM;

	if ((changed & ipv6_features) && !(actual & ipv6_features))
		qeth_flush_local_addrs6(card);
	if ((changed & ipv4_features) && !(actual & ipv4_features))
		qeth_flush_local_addrs4(card);
}

>>>>>>> upstream/android-13
int qeth_set_features(struct net_device *dev, netdev_features_t features)
{
	struct qeth_card *card = dev->ml_priv;
	netdev_features_t changed = dev->features ^ features;
	int rc = 0;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "setfeat");
	QETH_DBF_HEX(SETUP, 2, &features, sizeof(features));

	if ((changed & NETIF_F_IP_CSUM)) {
		rc = qeth_set_ipa_csum(card, features & NETIF_F_IP_CSUM,
				       IPA_OUTBOUND_CHECKSUM, QETH_PROT_IPV4);
=======
	QETH_CARD_TEXT(card, 2, "setfeat");
	QETH_CARD_HEX(card, 2, &features, sizeof(features));

	if ((changed & NETIF_F_IP_CSUM)) {
		rc = qeth_set_ipa_csum(card, features & NETIF_F_IP_CSUM,
				       IPA_OUTBOUND_CHECKSUM, QETH_PROT_IPV4,
				       &card->info.has_lp2lp_cso_v4);
>>>>>>> upstream/android-13
		if (rc)
			changed ^= NETIF_F_IP_CSUM;
	}
	if (changed & NETIF_F_IPV6_CSUM) {
		rc = qeth_set_ipa_csum(card, features & NETIF_F_IPV6_CSUM,
<<<<<<< HEAD
				       IPA_OUTBOUND_CHECKSUM, QETH_PROT_IPV6);
=======
				       IPA_OUTBOUND_CHECKSUM, QETH_PROT_IPV6,
				       &card->info.has_lp2lp_cso_v6);
>>>>>>> upstream/android-13
		if (rc)
			changed ^= NETIF_F_IPV6_CSUM;
	}
	if (changed & NETIF_F_RXCSUM) {
		rc = qeth_set_ipa_rx_csum(card, features & NETIF_F_RXCSUM);
		if (rc)
			changed ^= NETIF_F_RXCSUM;
	}
<<<<<<< HEAD
	if ((changed & NETIF_F_TSO)) {
		rc = qeth_set_ipa_tso(card, features & NETIF_F_TSO ? 1 : 0);
		if (rc)
			changed ^= NETIF_F_TSO;
	}
=======
	if (changed & NETIF_F_TSO) {
		rc = qeth_set_ipa_tso(card, features & NETIF_F_TSO,
				      QETH_PROT_IPV4);
		if (rc)
			changed ^= NETIF_F_TSO;
	}
	if (changed & NETIF_F_TSO6) {
		rc = qeth_set_ipa_tso(card, features & NETIF_F_TSO6,
				      QETH_PROT_IPV6);
		if (rc)
			changed ^= NETIF_F_TSO6;
	}

	qeth_check_restricted_features(card, dev->features ^ features,
				       dev->features ^ changed);
>>>>>>> upstream/android-13

	/* everything changed successfully? */
	if ((dev->features ^ features) == changed)
		return 0;
	/* something went wrong. save changed features and return error */
	dev->features ^= changed;
	return -EIO;
}
EXPORT_SYMBOL_GPL(qeth_set_features);

netdev_features_t qeth_fix_features(struct net_device *dev,
				    netdev_features_t features)
{
	struct qeth_card *card = dev->ml_priv;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "fixfeat");
=======
	QETH_CARD_TEXT(card, 2, "fixfeat");
>>>>>>> upstream/android-13
	if (!qeth_is_supported(card, IPA_OUTBOUND_CHECKSUM))
		features &= ~NETIF_F_IP_CSUM;
	if (!qeth_is_supported6(card, IPA_OUTBOUND_CHECKSUM_V6))
		features &= ~NETIF_F_IPV6_CSUM;
	if (!qeth_is_supported(card, IPA_INBOUND_CHECKSUM) &&
	    !qeth_is_supported6(card, IPA_INBOUND_CHECKSUM_V6))
		features &= ~NETIF_F_RXCSUM;
	if (!qeth_is_supported(card, IPA_OUTBOUND_TSO))
		features &= ~NETIF_F_TSO;
<<<<<<< HEAD
	/* if the card isn't up, remove features that require hw changes */
	if (card->state == CARD_STATE_DOWN ||
	    card->state == CARD_STATE_RECOVER)
		features &= ~QETH_HW_FEATURES;
	QETH_DBF_HEX(SETUP, 2, &features, sizeof(features));
=======
	if (!qeth_is_supported6(card, IPA_OUTBOUND_TSO))
		features &= ~NETIF_F_TSO6;

	QETH_CARD_HEX(card, 2, &features, sizeof(features));
>>>>>>> upstream/android-13
	return features;
}
EXPORT_SYMBOL_GPL(qeth_fix_features);

netdev_features_t qeth_features_check(struct sk_buff *skb,
				      struct net_device *dev,
				      netdev_features_t features)
{
<<<<<<< HEAD
=======
	struct qeth_card *card = dev->ml_priv;

	/* Traffic with local next-hop is not eligible for some offloads: */
	if (skb->ip_summed == CHECKSUM_PARTIAL &&
	    READ_ONCE(card->options.isolation) != ISOLATION_MODE_FWD) {
		netdev_features_t restricted = 0;

		if (skb_is_gso(skb) && !netif_needs_gso(skb, features))
			restricted |= NETIF_F_ALL_TSO;

		switch (vlan_get_protocol(skb)) {
		case htons(ETH_P_IP):
			if (!card->info.has_lp2lp_cso_v4)
				restricted |= NETIF_F_IP_CSUM;

			if (restricted && qeth_next_hop_is_local_v4(card, skb))
				features &= ~restricted;
			break;
		case htons(ETH_P_IPV6):
			if (!card->info.has_lp2lp_cso_v6)
				restricted |= NETIF_F_IPV6_CSUM;

			if (restricted && qeth_next_hop_is_local_v6(card, skb))
				features &= ~restricted;
			break;
		default:
			break;
		}
	}

>>>>>>> upstream/android-13
	/* GSO segmentation builds skbs with
	 *	a (small) linear part for the headers, and
	 *	page frags for the data.
	 * Compared to a linear skb, the header-only part consumes an
	 * additional buffer element. This reduces buffer utilization, and
	 * hurts throughput. So compress small segments into one element.
	 */
	if (netif_needs_gso(skb, features)) {
		/* match skb_segment(): */
		unsigned int doffset = skb->data - skb_mac_header(skb);
		unsigned int hsize = skb_shinfo(skb)->gso_size;
		unsigned int hroom = skb_headroom(skb);

		/* linearize only if resulting skb allocations are order-0: */
		if (SKB_DATA_ALIGN(hroom + doffset + hsize) <= SKB_MAX_HEAD(0))
			features &= ~NETIF_F_SG;
	}

	return vlan_features_check(skb, features);
}
EXPORT_SYMBOL_GPL(qeth_features_check);

<<<<<<< HEAD
=======
void qeth_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats)
{
	struct qeth_card *card = dev->ml_priv;
	struct qeth_qdio_out_q *queue;
	unsigned int i;

	QETH_CARD_TEXT(card, 5, "getstat");

	stats->rx_packets = card->stats.rx_packets;
	stats->rx_bytes = card->stats.rx_bytes;
	stats->rx_errors = card->stats.rx_length_errors +
			   card->stats.rx_frame_errors +
			   card->stats.rx_fifo_errors;
	stats->rx_dropped = card->stats.rx_dropped_nomem +
			    card->stats.rx_dropped_notsupp +
			    card->stats.rx_dropped_runt;
	stats->multicast = card->stats.rx_multicast;
	stats->rx_length_errors = card->stats.rx_length_errors;
	stats->rx_frame_errors = card->stats.rx_frame_errors;
	stats->rx_fifo_errors = card->stats.rx_fifo_errors;

	for (i = 0; i < card->qdio.no_out_queues; i++) {
		queue = card->qdio.out_qs[i];

		stats->tx_packets += queue->stats.tx_packets;
		stats->tx_bytes += queue->stats.tx_bytes;
		stats->tx_errors += queue->stats.tx_errors;
		stats->tx_dropped += queue->stats.tx_dropped;
	}
}
EXPORT_SYMBOL_GPL(qeth_get_stats64);

#define TC_IQD_UCAST   0
static void qeth_iqd_set_prio_tc_map(struct net_device *dev,
				     unsigned int ucast_txqs)
{
	unsigned int prio;

	/* IQD requires mcast traffic to be placed on a dedicated queue, and
	 * qeth_iqd_select_queue() deals with this.
	 * For unicast traffic, we defer the queue selection to the stack.
	 * By installing a trivial prio map that spans over only the unicast
	 * queues, we can encourage the stack to spread the ucast traffic evenly
	 * without selecting the mcast queue.
	 */

	/* One traffic class, spanning over all active ucast queues: */
	netdev_set_num_tc(dev, 1);
	netdev_set_tc_queue(dev, TC_IQD_UCAST, ucast_txqs,
			    QETH_IQD_MIN_UCAST_TXQ);

	/* Map all priorities to this traffic class: */
	for (prio = 0; prio <= TC_BITMASK; prio++)
		netdev_set_prio_tc_map(dev, prio, TC_IQD_UCAST);
}

int qeth_set_real_num_tx_queues(struct qeth_card *card, unsigned int count)
{
	struct net_device *dev = card->dev;
	int rc;

	/* Per netif_setup_tc(), adjust the mapping first: */
	if (IS_IQD(card))
		qeth_iqd_set_prio_tc_map(dev, count - 1);

	rc = netif_set_real_num_tx_queues(dev, count);

	if (rc && IS_IQD(card))
		qeth_iqd_set_prio_tc_map(dev, dev->real_num_tx_queues - 1);

	return rc;
}
EXPORT_SYMBOL_GPL(qeth_set_real_num_tx_queues);

u16 qeth_iqd_select_queue(struct net_device *dev, struct sk_buff *skb,
			  u8 cast_type, struct net_device *sb_dev)
{
	u16 txq;

	if (cast_type != RTN_UNICAST)
		return QETH_IQD_MCAST_TXQ;
	if (dev->real_num_tx_queues == QETH_IQD_MIN_TXQ)
		return QETH_IQD_MIN_UCAST_TXQ;

	txq = netdev_pick_tx(dev, skb, sb_dev);
	return (txq == QETH_IQD_MCAST_TXQ) ? QETH_IQD_MIN_UCAST_TXQ : txq;
}
EXPORT_SYMBOL_GPL(qeth_iqd_select_queue);

int qeth_open(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;
	struct qeth_qdio_out_q *queue;
	unsigned int i;

	QETH_CARD_TEXT(card, 4, "qethopen");

	card->data.state = CH_STATE_UP;
	netif_tx_start_all_queues(dev);

	local_bh_disable();
	qeth_for_each_output_queue(card, queue, i) {
		netif_tx_napi_add(dev, &queue->napi, qeth_tx_poll,
				  QETH_NAPI_WEIGHT);
		napi_enable(&queue->napi);
		napi_schedule(&queue->napi);
	}

	napi_enable(&card->napi);
	napi_schedule(&card->napi);
	/* kick-start the NAPI softirq: */
	local_bh_enable();

	return 0;
}
EXPORT_SYMBOL_GPL(qeth_open);

int qeth_stop(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;
	struct qeth_qdio_out_q *queue;
	unsigned int i;

	QETH_CARD_TEXT(card, 4, "qethstop");

	napi_disable(&card->napi);
	cancel_delayed_work_sync(&card->buffer_reclaim_work);
	qdio_stop_irq(CARD_DDEV(card));

	/* Quiesce the NAPI instances: */
	qeth_for_each_output_queue(card, queue, i)
		napi_disable(&queue->napi);

	/* Stop .ndo_start_xmit, might still access queue->napi. */
	netif_tx_disable(dev);

	qeth_for_each_output_queue(card, queue, i) {
		del_timer_sync(&queue->timer);
		/* Queues may get re-allocated, so remove the NAPIs. */
		netif_napi_del(&queue->napi);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(qeth_stop);

>>>>>>> upstream/android-13
static int __init qeth_core_init(void)
{
	int rc;

	pr_info("loading core functions\n");
<<<<<<< HEAD
	INIT_LIST_HEAD(&qeth_core_card_list.list);
	INIT_LIST_HEAD(&qeth_dbf_list);
	rwlock_init(&qeth_core_card_list.rwlock);
	mutex_init(&qeth_mod_mutex);

	qeth_wq = create_singlethread_workqueue("qeth_wq");
	if (!qeth_wq) {
		rc = -ENOMEM;
		goto out_err;
	}
=======

	qeth_debugfs_root = debugfs_create_dir("qeth", NULL);
>>>>>>> upstream/android-13

	rc = qeth_register_dbf_views();
	if (rc)
		goto dbf_err;
	qeth_core_root_dev = root_device_register("qeth");
	rc = PTR_ERR_OR_ZERO(qeth_core_root_dev);
	if (rc)
		goto register_err;
<<<<<<< HEAD
	qeth_core_header_cache = kmem_cache_create("qeth_hdr",
			sizeof(struct qeth_hdr) + ETH_HLEN, 64, 0, NULL);
=======
	qeth_core_header_cache =
		kmem_cache_create("qeth_hdr", QETH_HDR_CACHE_OBJ_SIZE,
				  roundup_pow_of_two(QETH_HDR_CACHE_OBJ_SIZE),
				  0, NULL);
>>>>>>> upstream/android-13
	if (!qeth_core_header_cache) {
		rc = -ENOMEM;
		goto slab_err;
	}
	qeth_qdio_outbuf_cache = kmem_cache_create("qeth_buf",
			sizeof(struct qeth_qdio_out_buffer), 0, 0, NULL);
	if (!qeth_qdio_outbuf_cache) {
		rc = -ENOMEM;
		goto cqslab_err;
	}
	rc = ccw_driver_register(&qeth_ccw_driver);
	if (rc)
		goto ccw_err;
	rc = ccwgroup_driver_register(&qeth_core_ccwgroup_driver);
	if (rc)
		goto ccwgroup_err;

	return 0;

ccwgroup_err:
	ccw_driver_unregister(&qeth_ccw_driver);
ccw_err:
	kmem_cache_destroy(qeth_qdio_outbuf_cache);
cqslab_err:
	kmem_cache_destroy(qeth_core_header_cache);
slab_err:
	root_device_unregister(qeth_core_root_dev);
register_err:
	qeth_unregister_dbf_views();
dbf_err:
<<<<<<< HEAD
	destroy_workqueue(qeth_wq);
out_err:
=======
	debugfs_remove_recursive(qeth_debugfs_root);
>>>>>>> upstream/android-13
	pr_err("Initializing the qeth device driver failed\n");
	return rc;
}

static void __exit qeth_core_exit(void)
{
	qeth_clear_dbf_list();
<<<<<<< HEAD
	destroy_workqueue(qeth_wq);
=======
>>>>>>> upstream/android-13
	ccwgroup_driver_unregister(&qeth_core_ccwgroup_driver);
	ccw_driver_unregister(&qeth_ccw_driver);
	kmem_cache_destroy(qeth_qdio_outbuf_cache);
	kmem_cache_destroy(qeth_core_header_cache);
	root_device_unregister(qeth_core_root_dev);
	qeth_unregister_dbf_views();
<<<<<<< HEAD
=======
	debugfs_remove_recursive(qeth_debugfs_root);
>>>>>>> upstream/android-13
	pr_info("core functions removed\n");
}

module_init(qeth_core_init);
module_exit(qeth_core_exit);
MODULE_AUTHOR("Frank Blaschka <frank.blaschka@de.ibm.com>");
MODULE_DESCRIPTION("qeth core functions");
MODULE_LICENSE("GPL");
