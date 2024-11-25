<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2004-2007 Intel Corporation.  All rights reserved.
 * Copyright (c) 2004 Topspin Corporation.  All rights reserved.
 * Copyright (c) 2004, 2005 Voltaire Corporation.  All rights reserved.
 * Copyright (c) 2005 Sun Microsystems, Inc. All rights reserved.
<<<<<<< HEAD
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
=======
 * Copyright (c) 2019, Mellanox Technologies inc.  All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/completion.h>
#include <linux/dma-mapping.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/idr.h>
#include <linux/interrupt.h>
#include <linux/random.h>
#include <linux/rbtree.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/workqueue.h>
#include <linux/kdev_t.h>
#include <linux/etherdevice.h>

#include <rdma/ib_cache.h>
#include <rdma/ib_cm.h>
<<<<<<< HEAD
#include "cm_msgs.h"
=======
#include <rdma/ib_sysfs.h>
#include "cm_msgs.h"
#include "core_priv.h"
#include "cm_trace.h"
>>>>>>> upstream/android-13

MODULE_AUTHOR("Sean Hefty");
MODULE_DESCRIPTION("InfiniBand CM");
MODULE_LICENSE("Dual BSD/GPL");

static const char * const ibcm_rej_reason_strs[] = {
	[IB_CM_REJ_NO_QP]			= "no QP",
	[IB_CM_REJ_NO_EEC]			= "no EEC",
	[IB_CM_REJ_NO_RESOURCES]		= "no resources",
	[IB_CM_REJ_TIMEOUT]			= "timeout",
	[IB_CM_REJ_UNSUPPORTED]			= "unsupported",
	[IB_CM_REJ_INVALID_COMM_ID]		= "invalid comm ID",
	[IB_CM_REJ_INVALID_COMM_INSTANCE]	= "invalid comm instance",
	[IB_CM_REJ_INVALID_SERVICE_ID]		= "invalid service ID",
	[IB_CM_REJ_INVALID_TRANSPORT_TYPE]	= "invalid transport type",
	[IB_CM_REJ_STALE_CONN]			= "stale conn",
	[IB_CM_REJ_RDC_NOT_EXIST]		= "RDC not exist",
	[IB_CM_REJ_INVALID_GID]			= "invalid GID",
	[IB_CM_REJ_INVALID_LID]			= "invalid LID",
	[IB_CM_REJ_INVALID_SL]			= "invalid SL",
	[IB_CM_REJ_INVALID_TRAFFIC_CLASS]	= "invalid traffic class",
	[IB_CM_REJ_INVALID_HOP_LIMIT]		= "invalid hop limit",
	[IB_CM_REJ_INVALID_PACKET_RATE]		= "invalid packet rate",
	[IB_CM_REJ_INVALID_ALT_GID]		= "invalid alt GID",
	[IB_CM_REJ_INVALID_ALT_LID]		= "invalid alt LID",
	[IB_CM_REJ_INVALID_ALT_SL]		= "invalid alt SL",
	[IB_CM_REJ_INVALID_ALT_TRAFFIC_CLASS]	= "invalid alt traffic class",
	[IB_CM_REJ_INVALID_ALT_HOP_LIMIT]	= "invalid alt hop limit",
	[IB_CM_REJ_INVALID_ALT_PACKET_RATE]	= "invalid alt packet rate",
	[IB_CM_REJ_PORT_CM_REDIRECT]		= "port CM redirect",
	[IB_CM_REJ_PORT_REDIRECT]		= "port redirect",
	[IB_CM_REJ_INVALID_MTU]			= "invalid MTU",
	[IB_CM_REJ_INSUFFICIENT_RESP_RESOURCES]	= "insufficient resp resources",
	[IB_CM_REJ_CONSUMER_DEFINED]		= "consumer defined",
	[IB_CM_REJ_INVALID_RNR_RETRY]		= "invalid RNR retry",
	[IB_CM_REJ_DUPLICATE_LOCAL_COMM_ID]	= "duplicate local comm ID",
	[IB_CM_REJ_INVALID_CLASS_VERSION]	= "invalid class version",
	[IB_CM_REJ_INVALID_FLOW_LABEL]		= "invalid flow label",
	[IB_CM_REJ_INVALID_ALT_FLOW_LABEL]	= "invalid alt flow label",
<<<<<<< HEAD
=======
	[IB_CM_REJ_VENDOR_OPTION_NOT_SUPPORTED] =
		"vendor option is not supported",
>>>>>>> upstream/android-13
};

const char *__attribute_const__ ibcm_reject_msg(int reason)
{
	size_t index = reason;

	if (index < ARRAY_SIZE(ibcm_rej_reason_strs) &&
	    ibcm_rej_reason_strs[index])
		return ibcm_rej_reason_strs[index];
	else
		return "unrecognized reason";
}
EXPORT_SYMBOL(ibcm_reject_msg);

<<<<<<< HEAD
static void cm_add_one(struct ib_device *device);
static void cm_remove_one(struct ib_device *device, void *client_data);
=======
struct cm_id_private;
struct cm_work;
static int cm_add_one(struct ib_device *device);
static void cm_remove_one(struct ib_device *device, void *client_data);
static void cm_process_work(struct cm_id_private *cm_id_priv,
			    struct cm_work *work);
static int cm_send_sidr_rep_locked(struct cm_id_private *cm_id_priv,
				   struct ib_cm_sidr_rep_param *param);
static int cm_send_dreq_locked(struct cm_id_private *cm_id_priv,
			       const void *private_data, u8 private_data_len);
static int cm_send_drep_locked(struct cm_id_private *cm_id_priv,
			       void *private_data, u8 private_data_len);
static int cm_send_rej_locked(struct cm_id_private *cm_id_priv,
			      enum ib_cm_rej_reason reason, void *ari,
			      u8 ari_length, const void *private_data,
			      u8 private_data_len);
>>>>>>> upstream/android-13

static struct ib_client cm_client = {
	.name   = "cm",
	.add    = cm_add_one,
	.remove = cm_remove_one
};

static struct ib_cm {
	spinlock_t lock;
	struct list_head device_list;
	rwlock_t device_lock;
	struct rb_root listen_service_table;
	u64 listen_service_id;
	/* struct rb_root peer_service_table; todo: fix peer to peer */
	struct rb_root remote_qp_table;
	struct rb_root remote_id_table;
	struct rb_root remote_sidr_table;
<<<<<<< HEAD
	struct idr local_id_table;
	__be32 random_id_operand;
	struct list_head timewait_list;
	struct workqueue_struct *wq;
	/* Sync on cm change port state */
	spinlock_t state_lock;
=======
	struct xarray local_id_table;
	u32 local_id_next;
	__be32 random_id_operand;
	struct list_head timewait_list;
	struct workqueue_struct *wq;
>>>>>>> upstream/android-13
} cm;

/* Counter indexes ordered by attribute ID */
enum {
	CM_REQ_COUNTER,
	CM_MRA_COUNTER,
	CM_REJ_COUNTER,
	CM_REP_COUNTER,
	CM_RTU_COUNTER,
	CM_DREQ_COUNTER,
	CM_DREP_COUNTER,
	CM_SIDR_REQ_COUNTER,
	CM_SIDR_REP_COUNTER,
	CM_LAP_COUNTER,
	CM_APR_COUNTER,
	CM_ATTR_COUNT,
	CM_ATTR_ID_OFFSET = 0x0010,
};

enum {
	CM_XMIT,
	CM_XMIT_RETRIES,
	CM_RECV,
	CM_RECV_DUPLICATES,
	CM_COUNTER_GROUPS
};

<<<<<<< HEAD
static char const counter_group_names[CM_COUNTER_GROUPS]
				     [sizeof("cm_rx_duplicates")] = {
	"cm_tx_msgs", "cm_tx_retries",
	"cm_rx_msgs", "cm_rx_duplicates"
};

struct cm_counter_group {
	struct kobject obj;
	atomic_long_t counter[CM_ATTR_COUNT];
};

struct cm_counter_attribute {
	struct attribute attr;
	int index;
};

#define CM_COUNTER_ATTR(_name, _index) \
struct cm_counter_attribute cm_##_name##_counter_attr = { \
	.attr = { .name = __stringify(_name), .mode = 0444 }, \
	.index = _index \
}

static CM_COUNTER_ATTR(req, CM_REQ_COUNTER);
static CM_COUNTER_ATTR(mra, CM_MRA_COUNTER);
static CM_COUNTER_ATTR(rej, CM_REJ_COUNTER);
static CM_COUNTER_ATTR(rep, CM_REP_COUNTER);
static CM_COUNTER_ATTR(rtu, CM_RTU_COUNTER);
static CM_COUNTER_ATTR(dreq, CM_DREQ_COUNTER);
static CM_COUNTER_ATTR(drep, CM_DREP_COUNTER);
static CM_COUNTER_ATTR(sidr_req, CM_SIDR_REQ_COUNTER);
static CM_COUNTER_ATTR(sidr_rep, CM_SIDR_REP_COUNTER);
static CM_COUNTER_ATTR(lap, CM_LAP_COUNTER);
static CM_COUNTER_ATTR(apr, CM_APR_COUNTER);

static struct attribute *cm_counter_default_attrs[] = {
	&cm_req_counter_attr.attr,
	&cm_mra_counter_attr.attr,
	&cm_rej_counter_attr.attr,
	&cm_rep_counter_attr.attr,
	&cm_rtu_counter_attr.attr,
	&cm_dreq_counter_attr.attr,
	&cm_drep_counter_attr.attr,
	&cm_sidr_req_counter_attr.attr,
	&cm_sidr_rep_counter_attr.attr,
	&cm_lap_counter_attr.attr,
	&cm_apr_counter_attr.attr,
	NULL
=======
struct cm_counter_attribute {
	struct ib_port_attribute attr;
	unsigned short group;
	unsigned short index;
>>>>>>> upstream/android-13
};

struct cm_port {
	struct cm_device *cm_dev;
	struct ib_mad_agent *mad_agent;
<<<<<<< HEAD
	struct kobject port_obj;
	u8 port_num;
	struct list_head cm_priv_prim_list;
	struct list_head cm_priv_altr_list;
	struct cm_counter_group counter_group[CM_COUNTER_GROUPS];
};

struct cm_device {
	struct list_head list;
	struct ib_device *ib_device;
	struct device *device;
	u8 ack_delay;
	int going_down;
	struct cm_port *port[0];
=======
	u32 port_num;
	atomic_long_t counters[CM_COUNTER_GROUPS][CM_ATTR_COUNT];
};

struct cm_device {
	struct kref kref;
	struct list_head list;
	spinlock_t mad_agent_lock;
	struct ib_device *ib_device;
	u8 ack_delay;
	int going_down;
	struct cm_port *port[];
>>>>>>> upstream/android-13
};

struct cm_av {
	struct cm_port *port;
<<<<<<< HEAD
	union ib_gid dgid;
=======
>>>>>>> upstream/android-13
	struct rdma_ah_attr ah_attr;
	u16 pkey_index;
	u8 timeout;
};

struct cm_work {
	struct delayed_work work;
	struct list_head list;
	struct cm_port *port;
	struct ib_mad_recv_wc *mad_recv_wc;	/* Received MADs */
	__be32 local_id;			/* Established / timewait */
	__be32 remote_id;
	struct ib_cm_event cm_event;
<<<<<<< HEAD
	struct sa_path_rec path[0];
};

struct cm_timewait_info {
	struct cm_work work;			/* Must be first. */
=======
	struct sa_path_rec path[];
};

struct cm_timewait_info {
	struct cm_work work;
>>>>>>> upstream/android-13
	struct list_head list;
	struct rb_node remote_qp_node;
	struct rb_node remote_id_node;
	__be64 remote_ca_guid;
	__be32 remote_qpn;
	u8 inserted_remote_qp;
	u8 inserted_remote_id;
};

struct cm_id_private {
	struct ib_cm_id	id;

	struct rb_node service_node;
	struct rb_node sidr_id_node;
<<<<<<< HEAD
	spinlock_t lock;	/* Do not acquire inside cm.lock */
	struct completion comp;
	atomic_t refcount;
	/* Number of clients sharing this ib_cm_id. Only valid for listeners.
	 * Protected by the cm.lock spinlock. */
	int listen_sharecount;
=======
	u32 sidr_slid;
	spinlock_t lock;	/* Do not acquire inside cm.lock */
	struct completion comp;
	refcount_t refcount;
	/* Number of clients sharing this ib_cm_id. Only valid for listeners.
	 * Protected by the cm.lock spinlock.
	 */
	int listen_sharecount;
	struct rcu_head rcu;
>>>>>>> upstream/android-13

	struct ib_mad_send_buf *msg;
	struct cm_timewait_info *timewait_info;
	/* todo: use alternate port on send failure */
	struct cm_av av;
	struct cm_av alt_av;

	void *private_data;
	__be64 tid;
	__be32 local_qpn;
	__be32 remote_qpn;
	enum ib_qp_type qp_type;
	__be32 sq_psn;
	__be32 rq_psn;
	int timeout_ms;
	enum ib_mtu path_mtu;
	__be16 pkey;
	u8 private_data_len;
	u8 max_cm_retries;
<<<<<<< HEAD
	u8 peer_to_peer;
=======
>>>>>>> upstream/android-13
	u8 responder_resources;
	u8 initiator_depth;
	u8 retry_count;
	u8 rnr_retry_count;
	u8 service_timeout;
	u8 target_ack_delay;

<<<<<<< HEAD
	struct list_head prim_list;
	struct list_head altr_list;
	/* Indicates that the send port mad is registered and av is set */
	int prim_send_port_not_ready;
	int altr_send_port_not_ready;

	struct list_head work_list;
	atomic_t work_count;
};

=======
	struct list_head work_list;
	atomic_t work_count;

	struct rdma_ucm_ece ece;
};

static void cm_dev_release(struct kref *kref)
{
	struct cm_device *cm_dev = container_of(kref, struct cm_device, kref);
	u32 i;

	rdma_for_each_port(cm_dev->ib_device, i)
		kfree(cm_dev->port[i - 1]);

	kfree(cm_dev);
}

static void cm_device_put(struct cm_device *cm_dev)
{
	kref_put(&cm_dev->kref, cm_dev_release);
}

>>>>>>> upstream/android-13
static void cm_work_handler(struct work_struct *work);

static inline void cm_deref_id(struct cm_id_private *cm_id_priv)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&cm_id_priv->refcount))
		complete(&cm_id_priv->comp);
}

static int cm_alloc_msg(struct cm_id_private *cm_id_priv,
			struct ib_mad_send_buf **msg)
=======
	if (refcount_dec_and_test(&cm_id_priv->refcount))
		complete(&cm_id_priv->comp);
}

static struct ib_mad_send_buf *cm_alloc_msg(struct cm_id_private *cm_id_priv)
>>>>>>> upstream/android-13
{
	struct ib_mad_agent *mad_agent;
	struct ib_mad_send_buf *m;
	struct ib_ah *ah;
<<<<<<< HEAD
	struct cm_av *av;
	unsigned long flags, flags2;
	int ret = 0;

	/* don't let the port to be released till the agent is down */
	spin_lock_irqsave(&cm.state_lock, flags2);
	spin_lock_irqsave(&cm.lock, flags);
	if (!cm_id_priv->prim_send_port_not_ready)
		av = &cm_id_priv->av;
	else if (!cm_id_priv->altr_send_port_not_ready &&
		 (cm_id_priv->alt_av.port))
		av = &cm_id_priv->alt_av;
	else {
		pr_info("%s: not valid CM id\n", __func__);
		ret = -ENODEV;
		spin_unlock_irqrestore(&cm.lock, flags);
		goto out;
	}
	spin_unlock_irqrestore(&cm.lock, flags);
	/* Make sure the port haven't released the mad yet */
	mad_agent = cm_id_priv->av.port->mad_agent;
	if (!mad_agent) {
		pr_info("%s: not a valid MAD agent\n", __func__);
		ret = -ENODEV;
		goto out;
	}
	ah = rdma_create_ah(mad_agent->qp->pd, &av->ah_attr);
	if (IS_ERR(ah)) {
		ret = PTR_ERR(ah);
=======

	lockdep_assert_held(&cm_id_priv->lock);

	if (!cm_id_priv->av.port)
		return ERR_PTR(-EINVAL);

	spin_lock(&cm_id_priv->av.port->cm_dev->mad_agent_lock);
	mad_agent = cm_id_priv->av.port->mad_agent;
	if (!mad_agent) {
		m = ERR_PTR(-EINVAL);
		goto out;
	}

	ah = rdma_create_ah(mad_agent->qp->pd, &cm_id_priv->av.ah_attr, 0);
	if (IS_ERR(ah)) {
		m = ERR_CAST(ah);
>>>>>>> upstream/android-13
		goto out;
	}

	m = ib_create_send_mad(mad_agent, cm_id_priv->id.remote_cm_qpn,
<<<<<<< HEAD
			       av->pkey_index,
=======
			       cm_id_priv->av.pkey_index,
>>>>>>> upstream/android-13
			       0, IB_MGMT_MAD_HDR, IB_MGMT_MAD_DATA,
			       GFP_ATOMIC,
			       IB_MGMT_BASE_VERSION);
	if (IS_ERR(m)) {
<<<<<<< HEAD
		rdma_destroy_ah(ah);
		ret = PTR_ERR(m);
=======
		rdma_destroy_ah(ah, 0);
>>>>>>> upstream/android-13
		goto out;
	}

	/* Timeout set by caller if response is expected. */
	m->ah = ah;
	m->retries = cm_id_priv->max_cm_retries;

<<<<<<< HEAD
	atomic_inc(&cm_id_priv->refcount);
	m->context[0] = cm_id_priv;
	*msg = m;

out:
	spin_unlock_irqrestore(&cm.state_lock, flags2);
	return ret;
=======
	refcount_inc(&cm_id_priv->refcount);
	m->context[0] = cm_id_priv;

out:
	spin_unlock(&cm_id_priv->av.port->cm_dev->mad_agent_lock);
	return m;
}

static void cm_free_msg(struct ib_mad_send_buf *msg)
{
	struct cm_id_private *cm_id_priv = msg->context[0];

	if (msg->ah)
		rdma_destroy_ah(msg->ah, 0);
	cm_deref_id(cm_id_priv);
	ib_free_send_mad(msg);
}

static struct ib_mad_send_buf *
cm_alloc_priv_msg(struct cm_id_private *cm_id_priv)
{
	struct ib_mad_send_buf *msg;

	lockdep_assert_held(&cm_id_priv->lock);

	msg = cm_alloc_msg(cm_id_priv);
	if (IS_ERR(msg))
		return msg;
	cm_id_priv->msg = msg;
	return msg;
}

static void cm_free_priv_msg(struct ib_mad_send_buf *msg)
{
	struct cm_id_private *cm_id_priv = msg->context[0];

	lockdep_assert_held(&cm_id_priv->lock);

	if (!WARN_ON(cm_id_priv->msg != msg))
		cm_id_priv->msg = NULL;

	if (msg->ah)
		rdma_destroy_ah(msg->ah, 0);
	cm_deref_id(cm_id_priv);
	ib_free_send_mad(msg);
>>>>>>> upstream/android-13
}

static struct ib_mad_send_buf *cm_alloc_response_msg_no_ah(struct cm_port *port,
							   struct ib_mad_recv_wc *mad_recv_wc)
{
	return ib_create_send_mad(port->mad_agent, 1, mad_recv_wc->wc->pkey_index,
				  0, IB_MGMT_MAD_HDR, IB_MGMT_MAD_DATA,
				  GFP_ATOMIC,
				  IB_MGMT_BASE_VERSION);
}

static int cm_create_response_msg_ah(struct cm_port *port,
				     struct ib_mad_recv_wc *mad_recv_wc,
				     struct ib_mad_send_buf *msg)
{
	struct ib_ah *ah;

	ah = ib_create_ah_from_wc(port->mad_agent->qp->pd, mad_recv_wc->wc,
				  mad_recv_wc->recv_buf.grh, port->port_num);
	if (IS_ERR(ah))
		return PTR_ERR(ah);

	msg->ah = ah;
	return 0;
}

<<<<<<< HEAD
static void cm_free_msg(struct ib_mad_send_buf *msg)
{
	if (msg->ah)
		rdma_destroy_ah(msg->ah);
	if (msg->context[0])
		cm_deref_id(msg->context[0]);
	ib_free_send_mad(msg);
}

=======
>>>>>>> upstream/android-13
static int cm_alloc_response_msg(struct cm_port *port,
				 struct ib_mad_recv_wc *mad_recv_wc,
				 struct ib_mad_send_buf **msg)
{
	struct ib_mad_send_buf *m;
	int ret;

	m = cm_alloc_response_msg_no_ah(port, mad_recv_wc);
	if (IS_ERR(m))
		return PTR_ERR(m);

	ret = cm_create_response_msg_ah(port, mad_recv_wc, m);
	if (ret) {
<<<<<<< HEAD
		cm_free_msg(m);
=======
		ib_free_send_mad(m);
>>>>>>> upstream/android-13
		return ret;
	}

	*msg = m;
	return 0;
}

<<<<<<< HEAD
static void * cm_copy_private_data(const void *private_data,
				   u8 private_data_len)
=======
static void cm_free_response_msg(struct ib_mad_send_buf *msg)
{
	if (msg->ah)
		rdma_destroy_ah(msg->ah, 0);
	ib_free_send_mad(msg);
}

static void *cm_copy_private_data(const void *private_data, u8 private_data_len)
>>>>>>> upstream/android-13
{
	void *data;

	if (!private_data || !private_data_len)
		return NULL;

	data = kmemdup(private_data, private_data_len, GFP_KERNEL);
	if (!data)
		return ERR_PTR(-ENOMEM);

	return data;
}

static void cm_set_private_data(struct cm_id_private *cm_id_priv,
				 void *private_data, u8 private_data_len)
{
	if (cm_id_priv->private_data && cm_id_priv->private_data_len)
		kfree(cm_id_priv->private_data);

	cm_id_priv->private_data = private_data;
	cm_id_priv->private_data_len = private_data_len;
}

<<<<<<< HEAD
static int cm_init_av_for_lap(struct cm_port *port, struct ib_wc *wc,
			      struct ib_grh *grh, struct cm_av *av)
{
	struct rdma_ah_attr new_ah_attr;
	int ret;

	av->port = port;
	av->pkey_index = wc->pkey_index;

	/*
	 * av->ah_attr might be initialized based on past wc during incoming
	 * connect request or while sending out connect request. So initialize
	 * a new ah_attr on stack. If initialization fails, old ah_attr is
	 * used for sending any responses. If initialization is successful,
	 * than new ah_attr is used by overwriting old one.
	 */
	ret = ib_init_ah_attr_from_wc(port->cm_dev->ib_device,
				      port->port_num, wc,
				      grh, &new_ah_attr);
	if (ret)
		return ret;

	rdma_move_ah_attr(&av->ah_attr, &new_ah_attr);
	return 0;
=======
static void cm_set_av_port(struct cm_av *av, struct cm_port *port)
{
	struct cm_port *old_port = av->port;

	if (old_port == port)
		return;

	av->port = port;
	if (old_port)
		cm_device_put(old_port->cm_dev);
	if (port)
		kref_get(&port->cm_dev->kref);
}

static void cm_init_av_for_lap(struct cm_port *port, struct ib_wc *wc,
			       struct rdma_ah_attr *ah_attr, struct cm_av *av)
{
	cm_set_av_port(av, port);
	av->pkey_index = wc->pkey_index;
	rdma_move_ah_attr(&av->ah_attr, ah_attr);
>>>>>>> upstream/android-13
}

static int cm_init_av_for_response(struct cm_port *port, struct ib_wc *wc,
				   struct ib_grh *grh, struct cm_av *av)
{
<<<<<<< HEAD
	av->port = port;
=======
	cm_set_av_port(av, port);
>>>>>>> upstream/android-13
	av->pkey_index = wc->pkey_index;
	return ib_init_ah_attr_from_wc(port->cm_dev->ib_device,
				       port->port_num, wc,
				       grh, &av->ah_attr);
}

<<<<<<< HEAD
static int add_cm_id_to_port_list(struct cm_id_private *cm_id_priv,
				  struct cm_av *av,
				  struct cm_port *port)
{
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&cm.lock, flags);

	if (&cm_id_priv->av == av)
		list_add_tail(&cm_id_priv->prim_list, &port->cm_priv_prim_list);
	else if (&cm_id_priv->alt_av == av)
		list_add_tail(&cm_id_priv->altr_list, &port->cm_priv_altr_list);
	else
		ret = -EINVAL;

	spin_unlock_irqrestore(&cm.lock, flags);
	return ret;
}

=======
>>>>>>> upstream/android-13
static struct cm_port *
get_cm_port_from_path(struct sa_path_rec *path, const struct ib_gid_attr *attr)
{
	struct cm_device *cm_dev;
	struct cm_port *port = NULL;
	unsigned long flags;

	if (attr) {
		read_lock_irqsave(&cm.device_lock, flags);
		list_for_each_entry(cm_dev, &cm.device_list, list) {
			if (cm_dev->ib_device == attr->device) {
				port = cm_dev->port[attr->port_num - 1];
				break;
			}
		}
		read_unlock_irqrestore(&cm.device_lock, flags);
	} else {
		/* SGID attribute can be NULL in following
		 * conditions.
		 * (a) Alternative path
		 * (b) IB link layer without GRH
		 * (c) LAP send messages
		 */
		read_lock_irqsave(&cm.device_lock, flags);
		list_for_each_entry(cm_dev, &cm.device_list, list) {
			attr = rdma_find_gid(cm_dev->ib_device,
					     &path->sgid,
					     sa_conv_pathrec_to_gid_type(path),
					     NULL);
			if (!IS_ERR(attr)) {
				port = cm_dev->port[attr->port_num - 1];
				break;
			}
		}
		read_unlock_irqrestore(&cm.device_lock, flags);
		if (port)
			rdma_put_gid_attr(attr);
	}
	return port;
}

static int cm_init_av_by_path(struct sa_path_rec *path,
			      const struct ib_gid_attr *sgid_attr,
<<<<<<< HEAD
			      struct cm_av *av,
			      struct cm_id_private *cm_id_priv)
=======
			      struct cm_av *av)
>>>>>>> upstream/android-13
{
	struct rdma_ah_attr new_ah_attr;
	struct cm_device *cm_dev;
	struct cm_port *port;
	int ret;

	port = get_cm_port_from_path(path, sgid_attr);
	if (!port)
		return -EINVAL;
	cm_dev = port->cm_dev;

	ret = ib_find_cached_pkey(cm_dev->ib_device, port->port_num,
				  be16_to_cpu(path->pkey), &av->pkey_index);
	if (ret)
		return ret;

<<<<<<< HEAD
	av->port = port;
=======
	cm_set_av_port(av, port);
>>>>>>> upstream/android-13

	/*
	 * av->ah_attr might be initialized based on wc or during
	 * request processing time which might have reference to sgid_attr.
	 * So initialize a new ah_attr on stack.
	 * If initialization fails, old ah_attr is used for sending any
	 * responses. If initialization is successful, than new ah_attr
	 * is used by overwriting the old one. So that right ah_attr
	 * can be used to return an error response.
	 */
	ret = ib_init_ah_attr_from_path(cm_dev->ib_device, port->port_num, path,
					&new_ah_attr, sgid_attr);
	if (ret)
		return ret;

	av->timeout = path->packet_life_time + 1;
<<<<<<< HEAD

	ret = add_cm_id_to_port_list(cm_id_priv, av, port);
	if (ret) {
		rdma_destroy_ah_attr(&new_ah_attr);
		return ret;
	}
=======
>>>>>>> upstream/android-13
	rdma_move_ah_attr(&av->ah_attr, &new_ah_attr);
	return 0;
}

<<<<<<< HEAD
static int cm_alloc_id(struct cm_id_private *cm_id_priv)
{
	unsigned long flags;
	int id;

	idr_preload(GFP_KERNEL);
	spin_lock_irqsave(&cm.lock, flags);

	id = idr_alloc_cyclic(&cm.local_id_table, cm_id_priv, 0, 0, GFP_NOWAIT);

	spin_unlock_irqrestore(&cm.lock, flags);
	idr_preload_end();

	cm_id_priv->id.local_id = (__force __be32)id ^ cm.random_id_operand;
	return id < 0 ? id : 0;
}

static void cm_free_id(__be32 local_id)
{
	spin_lock_irq(&cm.lock);
	idr_remove(&cm.local_id_table,
		   (__force int) (local_id ^ cm.random_id_operand));
	spin_unlock_irq(&cm.lock);
}

static struct cm_id_private * cm_get_id(__be32 local_id, __be32 remote_id)
{
	struct cm_id_private *cm_id_priv;

	cm_id_priv = idr_find(&cm.local_id_table,
			      (__force int) (local_id ^ cm.random_id_operand));
	if (cm_id_priv) {
		if (cm_id_priv->id.remote_id == remote_id)
			atomic_inc(&cm_id_priv->refcount);
		else
			cm_id_priv = NULL;
	}

	return cm_id_priv;
}

static struct cm_id_private * cm_acquire_id(__be32 local_id, __be32 remote_id)
{
	struct cm_id_private *cm_id_priv;

	spin_lock_irq(&cm.lock);
	cm_id_priv = cm_get_id(local_id, remote_id);
	spin_unlock_irq(&cm.lock);
=======
/* Move av created by cm_init_av_by_path(), so av.dgid is not moved */
static void cm_move_av_from_path(struct cm_av *dest, struct cm_av *src)
{
	cm_set_av_port(dest, src->port);
	cm_set_av_port(src, NULL);
	dest->pkey_index = src->pkey_index;
	rdma_move_ah_attr(&dest->ah_attr, &src->ah_attr);
	dest->timeout = src->timeout;
}

static void cm_destroy_av(struct cm_av *av)
{
	rdma_destroy_ah_attr(&av->ah_attr);
	cm_set_av_port(av, NULL);
}

static u32 cm_local_id(__be32 local_id)
{
	return (__force u32) (local_id ^ cm.random_id_operand);
}

static struct cm_id_private *cm_acquire_id(__be32 local_id, __be32 remote_id)
{
	struct cm_id_private *cm_id_priv;

	rcu_read_lock();
	cm_id_priv = xa_load(&cm.local_id_table, cm_local_id(local_id));
	if (!cm_id_priv || cm_id_priv->id.remote_id != remote_id ||
	    !refcount_inc_not_zero(&cm_id_priv->refcount))
		cm_id_priv = NULL;
	rcu_read_unlock();
>>>>>>> upstream/android-13

	return cm_id_priv;
}

/*
 * Trivial helpers to strip endian annotation and compare; the
 * endianness doesn't actually matter since we just need a stable
 * order for the RB tree.
 */
static int be32_lt(__be32 a, __be32 b)
{
	return (__force u32) a < (__force u32) b;
}

static int be32_gt(__be32 a, __be32 b)
{
	return (__force u32) a > (__force u32) b;
}

static int be64_lt(__be64 a, __be64 b)
{
	return (__force u64) a < (__force u64) b;
}

static int be64_gt(__be64 a, __be64 b)
{
	return (__force u64) a > (__force u64) b;
}

<<<<<<< HEAD
static struct cm_id_private * cm_insert_listen(struct cm_id_private *cm_id_priv)
=======
/*
 * Inserts a new cm_id_priv into the listen_service_table. Returns cm_id_priv
 * if the new ID was inserted, NULL if it could not be inserted due to a
 * collision, or the existing cm_id_priv ready for shared usage.
 */
static struct cm_id_private *cm_insert_listen(struct cm_id_private *cm_id_priv,
					      ib_cm_handler shared_handler)
>>>>>>> upstream/android-13
{
	struct rb_node **link = &cm.listen_service_table.rb_node;
	struct rb_node *parent = NULL;
	struct cm_id_private *cur_cm_id_priv;
	__be64 service_id = cm_id_priv->id.service_id;
	__be64 service_mask = cm_id_priv->id.service_mask;
<<<<<<< HEAD

=======
	unsigned long flags;

	spin_lock_irqsave(&cm.lock, flags);
>>>>>>> upstream/android-13
	while (*link) {
		parent = *link;
		cur_cm_id_priv = rb_entry(parent, struct cm_id_private,
					  service_node);
		if ((cur_cm_id_priv->id.service_mask & service_id) ==
		    (service_mask & cur_cm_id_priv->id.service_id) &&
<<<<<<< HEAD
		    (cm_id_priv->id.device == cur_cm_id_priv->id.device))
			return cur_cm_id_priv;
=======
		    (cm_id_priv->id.device == cur_cm_id_priv->id.device)) {
			/*
			 * Sharing an ib_cm_id with different handlers is not
			 * supported
			 */
			if (cur_cm_id_priv->id.cm_handler != shared_handler ||
			    cur_cm_id_priv->id.context ||
			    WARN_ON(!cur_cm_id_priv->id.cm_handler)) {
				spin_unlock_irqrestore(&cm.lock, flags);
				return NULL;
			}
			refcount_inc(&cur_cm_id_priv->refcount);
			cur_cm_id_priv->listen_sharecount++;
			spin_unlock_irqrestore(&cm.lock, flags);
			return cur_cm_id_priv;
		}
>>>>>>> upstream/android-13

		if (cm_id_priv->id.device < cur_cm_id_priv->id.device)
			link = &(*link)->rb_left;
		else if (cm_id_priv->id.device > cur_cm_id_priv->id.device)
			link = &(*link)->rb_right;
		else if (be64_lt(service_id, cur_cm_id_priv->id.service_id))
			link = &(*link)->rb_left;
		else if (be64_gt(service_id, cur_cm_id_priv->id.service_id))
			link = &(*link)->rb_right;
		else
			link = &(*link)->rb_right;
	}
<<<<<<< HEAD
	rb_link_node(&cm_id_priv->service_node, parent, link);
	rb_insert_color(&cm_id_priv->service_node, &cm.listen_service_table);
	return NULL;
}

static struct cm_id_private * cm_find_listen(struct ib_device *device,
					     __be64 service_id)
=======
	cm_id_priv->listen_sharecount++;
	rb_link_node(&cm_id_priv->service_node, parent, link);
	rb_insert_color(&cm_id_priv->service_node, &cm.listen_service_table);
	spin_unlock_irqrestore(&cm.lock, flags);
	return cm_id_priv;
}

static struct cm_id_private *cm_find_listen(struct ib_device *device,
					    __be64 service_id)
>>>>>>> upstream/android-13
{
	struct rb_node *node = cm.listen_service_table.rb_node;
	struct cm_id_private *cm_id_priv;

	while (node) {
		cm_id_priv = rb_entry(node, struct cm_id_private, service_node);
		if ((cm_id_priv->id.service_mask & service_id) ==
		     cm_id_priv->id.service_id &&
<<<<<<< HEAD
		    (cm_id_priv->id.device == device))
			return cm_id_priv;

=======
		    (cm_id_priv->id.device == device)) {
			refcount_inc(&cm_id_priv->refcount);
			return cm_id_priv;
		}
>>>>>>> upstream/android-13
		if (device < cm_id_priv->id.device)
			node = node->rb_left;
		else if (device > cm_id_priv->id.device)
			node = node->rb_right;
		else if (be64_lt(service_id, cm_id_priv->id.service_id))
			node = node->rb_left;
		else if (be64_gt(service_id, cm_id_priv->id.service_id))
			node = node->rb_right;
		else
			node = node->rb_right;
	}
	return NULL;
}

<<<<<<< HEAD
static struct cm_timewait_info * cm_insert_remote_id(struct cm_timewait_info
						     *timewait_info)
=======
static struct cm_timewait_info *
cm_insert_remote_id(struct cm_timewait_info *timewait_info)
>>>>>>> upstream/android-13
{
	struct rb_node **link = &cm.remote_id_table.rb_node;
	struct rb_node *parent = NULL;
	struct cm_timewait_info *cur_timewait_info;
	__be64 remote_ca_guid = timewait_info->remote_ca_guid;
	__be32 remote_id = timewait_info->work.remote_id;

	while (*link) {
		parent = *link;
		cur_timewait_info = rb_entry(parent, struct cm_timewait_info,
					     remote_id_node);
		if (be32_lt(remote_id, cur_timewait_info->work.remote_id))
			link = &(*link)->rb_left;
		else if (be32_gt(remote_id, cur_timewait_info->work.remote_id))
			link = &(*link)->rb_right;
		else if (be64_lt(remote_ca_guid, cur_timewait_info->remote_ca_guid))
			link = &(*link)->rb_left;
		else if (be64_gt(remote_ca_guid, cur_timewait_info->remote_ca_guid))
			link = &(*link)->rb_right;
		else
			return cur_timewait_info;
	}
	timewait_info->inserted_remote_id = 1;
	rb_link_node(&timewait_info->remote_id_node, parent, link);
	rb_insert_color(&timewait_info->remote_id_node, &cm.remote_id_table);
	return NULL;
}

<<<<<<< HEAD
static struct cm_timewait_info * cm_find_remote_id(__be64 remote_ca_guid,
						   __be32 remote_id)
{
	struct rb_node *node = cm.remote_id_table.rb_node;
	struct cm_timewait_info *timewait_info;

=======
static struct cm_id_private *cm_find_remote_id(__be64 remote_ca_guid,
					       __be32 remote_id)
{
	struct rb_node *node = cm.remote_id_table.rb_node;
	struct cm_timewait_info *timewait_info;
	struct cm_id_private *res = NULL;

	spin_lock_irq(&cm.lock);
>>>>>>> upstream/android-13
	while (node) {
		timewait_info = rb_entry(node, struct cm_timewait_info,
					 remote_id_node);
		if (be32_lt(remote_id, timewait_info->work.remote_id))
			node = node->rb_left;
		else if (be32_gt(remote_id, timewait_info->work.remote_id))
			node = node->rb_right;
		else if (be64_lt(remote_ca_guid, timewait_info->remote_ca_guid))
			node = node->rb_left;
		else if (be64_gt(remote_ca_guid, timewait_info->remote_ca_guid))
			node = node->rb_right;
<<<<<<< HEAD
		else
			return timewait_info;
	}
	return NULL;
}

static struct cm_timewait_info * cm_insert_remote_qpn(struct cm_timewait_info
						      *timewait_info)
=======
		else {
			res = cm_acquire_id(timewait_info->work.local_id,
					     timewait_info->work.remote_id);
			break;
		}
	}
	spin_unlock_irq(&cm.lock);
	return res;
}

static struct cm_timewait_info *
cm_insert_remote_qpn(struct cm_timewait_info *timewait_info)
>>>>>>> upstream/android-13
{
	struct rb_node **link = &cm.remote_qp_table.rb_node;
	struct rb_node *parent = NULL;
	struct cm_timewait_info *cur_timewait_info;
	__be64 remote_ca_guid = timewait_info->remote_ca_guid;
	__be32 remote_qpn = timewait_info->remote_qpn;

	while (*link) {
		parent = *link;
		cur_timewait_info = rb_entry(parent, struct cm_timewait_info,
					     remote_qp_node);
		if (be32_lt(remote_qpn, cur_timewait_info->remote_qpn))
			link = &(*link)->rb_left;
		else if (be32_gt(remote_qpn, cur_timewait_info->remote_qpn))
			link = &(*link)->rb_right;
		else if (be64_lt(remote_ca_guid, cur_timewait_info->remote_ca_guid))
			link = &(*link)->rb_left;
		else if (be64_gt(remote_ca_guid, cur_timewait_info->remote_ca_guid))
			link = &(*link)->rb_right;
		else
			return cur_timewait_info;
	}
	timewait_info->inserted_remote_qp = 1;
	rb_link_node(&timewait_info->remote_qp_node, parent, link);
	rb_insert_color(&timewait_info->remote_qp_node, &cm.remote_qp_table);
	return NULL;
}

<<<<<<< HEAD
static struct cm_id_private * cm_insert_remote_sidr(struct cm_id_private
						    *cm_id_priv)
=======
static struct cm_id_private *
cm_insert_remote_sidr(struct cm_id_private *cm_id_priv)
>>>>>>> upstream/android-13
{
	struct rb_node **link = &cm.remote_sidr_table.rb_node;
	struct rb_node *parent = NULL;
	struct cm_id_private *cur_cm_id_priv;
<<<<<<< HEAD
	union ib_gid *port_gid = &cm_id_priv->av.dgid;
=======
>>>>>>> upstream/android-13
	__be32 remote_id = cm_id_priv->id.remote_id;

	while (*link) {
		parent = *link;
		cur_cm_id_priv = rb_entry(parent, struct cm_id_private,
					  sidr_id_node);
		if (be32_lt(remote_id, cur_cm_id_priv->id.remote_id))
			link = &(*link)->rb_left;
		else if (be32_gt(remote_id, cur_cm_id_priv->id.remote_id))
			link = &(*link)->rb_right;
		else {
<<<<<<< HEAD
			int cmp;
			cmp = memcmp(port_gid, &cur_cm_id_priv->av.dgid,
				     sizeof *port_gid);
			if (cmp < 0)
				link = &(*link)->rb_left;
			else if (cmp > 0)
=======
			if (cur_cm_id_priv->sidr_slid < cm_id_priv->sidr_slid)
				link = &(*link)->rb_left;
			else if (cur_cm_id_priv->sidr_slid > cm_id_priv->sidr_slid)
>>>>>>> upstream/android-13
				link = &(*link)->rb_right;
			else
				return cur_cm_id_priv;
		}
	}
	rb_link_node(&cm_id_priv->sidr_id_node, parent, link);
	rb_insert_color(&cm_id_priv->sidr_id_node, &cm.remote_sidr_table);
	return NULL;
}

<<<<<<< HEAD
static void cm_reject_sidr_req(struct cm_id_private *cm_id_priv,
			       enum ib_cm_sidr_status status)
{
	struct ib_cm_sidr_rep_param param;

	memset(&param, 0, sizeof param);
	param.status = status;
	ib_send_cm_sidr_rep(&cm_id_priv->id, &param);
}

struct ib_cm_id *ib_create_cm_id(struct ib_device *device,
				 ib_cm_handler cm_handler,
				 void *context)
{
	struct cm_id_private *cm_id_priv;
=======
static struct cm_id_private *cm_alloc_id_priv(struct ib_device *device,
					      ib_cm_handler cm_handler,
					      void *context)
{
	struct cm_id_private *cm_id_priv;
	u32 id;
>>>>>>> upstream/android-13
	int ret;

	cm_id_priv = kzalloc(sizeof *cm_id_priv, GFP_KERNEL);
	if (!cm_id_priv)
		return ERR_PTR(-ENOMEM);

	cm_id_priv->id.state = IB_CM_IDLE;
	cm_id_priv->id.device = device;
	cm_id_priv->id.cm_handler = cm_handler;
	cm_id_priv->id.context = context;
	cm_id_priv->id.remote_cm_qpn = 1;
<<<<<<< HEAD
	ret = cm_alloc_id(cm_id_priv);
	if (ret)
		goto error;

	spin_lock_init(&cm_id_priv->lock);
	init_completion(&cm_id_priv->comp);
	INIT_LIST_HEAD(&cm_id_priv->work_list);
	INIT_LIST_HEAD(&cm_id_priv->prim_list);
	INIT_LIST_HEAD(&cm_id_priv->altr_list);
	atomic_set(&cm_id_priv->work_count, -1);
	atomic_set(&cm_id_priv->refcount, 1);
	return &cm_id_priv->id;

error:
	kfree(cm_id_priv);
	return ERR_PTR(-ENOMEM);
}
EXPORT_SYMBOL(ib_create_cm_id);

static struct cm_work * cm_dequeue_work(struct cm_id_private *cm_id_priv)
=======

	RB_CLEAR_NODE(&cm_id_priv->service_node);
	RB_CLEAR_NODE(&cm_id_priv->sidr_id_node);
	spin_lock_init(&cm_id_priv->lock);
	init_completion(&cm_id_priv->comp);
	INIT_LIST_HEAD(&cm_id_priv->work_list);
	atomic_set(&cm_id_priv->work_count, -1);
	refcount_set(&cm_id_priv->refcount, 1);

	ret = xa_alloc_cyclic(&cm.local_id_table, &id, NULL, xa_limit_32b,
			      &cm.local_id_next, GFP_KERNEL);
	if (ret < 0)
		goto error;
	cm_id_priv->id.local_id = (__force __be32)id ^ cm.random_id_operand;

	return cm_id_priv;

error:
	kfree(cm_id_priv);
	return ERR_PTR(ret);
}

/*
 * Make the ID visible to the MAD handlers and other threads that use the
 * xarray.
 */
static void cm_finalize_id(struct cm_id_private *cm_id_priv)
{
	xa_store(&cm.local_id_table, cm_local_id(cm_id_priv->id.local_id),
		 cm_id_priv, GFP_ATOMIC);
}

struct ib_cm_id *ib_create_cm_id(struct ib_device *device,
				 ib_cm_handler cm_handler,
				 void *context)
{
	struct cm_id_private *cm_id_priv;

	cm_id_priv = cm_alloc_id_priv(device, cm_handler, context);
	if (IS_ERR(cm_id_priv))
		return ERR_CAST(cm_id_priv);

	cm_finalize_id(cm_id_priv);
	return &cm_id_priv->id;
}
EXPORT_SYMBOL(ib_create_cm_id);

static struct cm_work *cm_dequeue_work(struct cm_id_private *cm_id_priv)
>>>>>>> upstream/android-13
{
	struct cm_work *work;

	if (list_empty(&cm_id_priv->work_list))
		return NULL;

	work = list_entry(cm_id_priv->work_list.next, struct cm_work, list);
	list_del(&work->list);
	return work;
}

static void cm_free_work(struct cm_work *work)
{
	if (work->mad_recv_wc)
		ib_free_recv_mad(work->mad_recv_wc);
	kfree(work);
}

<<<<<<< HEAD
=======
static void cm_queue_work_unlock(struct cm_id_private *cm_id_priv,
				 struct cm_work *work)
	__releases(&cm_id_priv->lock)
{
	bool immediate;

	/*
	 * To deliver the event to the user callback we have the drop the
	 * spinlock, however, we need to ensure that the user callback is single
	 * threaded and receives events in the temporal order. If there are
	 * already events being processed then thread new events onto a list,
	 * the thread currently processing will pick them up.
	 */
	immediate = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!immediate) {
		list_add_tail(&work->list, &cm_id_priv->work_list);
		/*
		 * This routine always consumes incoming reference. Once queued
		 * to the work_list then a reference is held by the thread
		 * currently running cm_process_work() and this reference is not
		 * needed.
		 */
		cm_deref_id(cm_id_priv);
	}
	spin_unlock_irq(&cm_id_priv->lock);

	if (immediate)
		cm_process_work(cm_id_priv, work);
}

>>>>>>> upstream/android-13
static inline int cm_convert_to_ms(int iba_time)
{
	/* approximate conversion to ms from 4.096us x 2^iba_time */
	return 1 << max(iba_time - 8, 0);
}

/*
 * calculate: 4.096x2^ack_timeout = 4.096x2^ack_delay + 2x4.096x2^life_time
 * Because of how ack_timeout is stored, adding one doubles the timeout.
 * To avoid large timeouts, select the max(ack_delay, life_time + 1), and
 * increment it (round up) only if the other is within 50%.
 */
static u8 cm_ack_timeout(u8 ca_ack_delay, u8 packet_life_time)
{
	int ack_timeout = packet_life_time + 1;

	if (ack_timeout >= ca_ack_delay)
		ack_timeout += (ca_ack_delay >= (ack_timeout - 1));
	else
		ack_timeout = ca_ack_delay +
			      (ack_timeout >= (ca_ack_delay - 1));

	return min(31, ack_timeout);
}

<<<<<<< HEAD
static void cm_cleanup_timewait(struct cm_timewait_info *timewait_info)
{
=======
static void cm_remove_remote(struct cm_id_private *cm_id_priv)
{
	struct cm_timewait_info *timewait_info = cm_id_priv->timewait_info;

>>>>>>> upstream/android-13
	if (timewait_info->inserted_remote_id) {
		rb_erase(&timewait_info->remote_id_node, &cm.remote_id_table);
		timewait_info->inserted_remote_id = 0;
	}

	if (timewait_info->inserted_remote_qp) {
		rb_erase(&timewait_info->remote_qp_node, &cm.remote_qp_table);
		timewait_info->inserted_remote_qp = 0;
	}
}

<<<<<<< HEAD
static struct cm_timewait_info * cm_create_timewait_info(__be32 local_id)
=======
static struct cm_timewait_info *cm_create_timewait_info(__be32 local_id)
>>>>>>> upstream/android-13
{
	struct cm_timewait_info *timewait_info;

	timewait_info = kzalloc(sizeof *timewait_info, GFP_KERNEL);
	if (!timewait_info)
		return ERR_PTR(-ENOMEM);

	timewait_info->work.local_id = local_id;
	INIT_DELAYED_WORK(&timewait_info->work.work, cm_work_handler);
	timewait_info->work.cm_event.event = IB_CM_TIMEWAIT_EXIT;
	return timewait_info;
}

static void cm_enter_timewait(struct cm_id_private *cm_id_priv)
{
	int wait_time;
	unsigned long flags;
	struct cm_device *cm_dev;

<<<<<<< HEAD
=======
	lockdep_assert_held(&cm_id_priv->lock);

>>>>>>> upstream/android-13
	cm_dev = ib_get_client_data(cm_id_priv->id.device, &cm_client);
	if (!cm_dev)
		return;

	spin_lock_irqsave(&cm.lock, flags);
<<<<<<< HEAD
	cm_cleanup_timewait(cm_id_priv->timewait_info);
=======
	cm_remove_remote(cm_id_priv);
>>>>>>> upstream/android-13
	list_add_tail(&cm_id_priv->timewait_info->list, &cm.timewait_list);
	spin_unlock_irqrestore(&cm.lock, flags);

	/*
	 * The cm_id could be destroyed by the user before we exit timewait.
	 * To protect against this, we search for the cm_id after exiting
	 * timewait before notifying the user that we've exited timewait.
	 */
	cm_id_priv->id.state = IB_CM_TIMEWAIT;
	wait_time = cm_convert_to_ms(cm_id_priv->av.timeout);

	/* Check if the device started its remove_one */
	spin_lock_irqsave(&cm.lock, flags);
	if (!cm_dev->going_down)
		queue_delayed_work(cm.wq, &cm_id_priv->timewait_info->work.work,
				   msecs_to_jiffies(wait_time));
	spin_unlock_irqrestore(&cm.lock, flags);

<<<<<<< HEAD
=======
	/*
	 * The timewait_info is converted into a work and gets freed during
	 * cm_free_work() in cm_timewait_handler().
	 */
	BUILD_BUG_ON(offsetof(struct cm_timewait_info, work) != 0);
>>>>>>> upstream/android-13
	cm_id_priv->timewait_info = NULL;
}

static void cm_reset_to_idle(struct cm_id_private *cm_id_priv)
{
	unsigned long flags;

<<<<<<< HEAD
	cm_id_priv->id.state = IB_CM_IDLE;
	if (cm_id_priv->timewait_info) {
		spin_lock_irqsave(&cm.lock, flags);
		cm_cleanup_timewait(cm_id_priv->timewait_info);
=======
	lockdep_assert_held(&cm_id_priv->lock);

	cm_id_priv->id.state = IB_CM_IDLE;
	if (cm_id_priv->timewait_info) {
		spin_lock_irqsave(&cm.lock, flags);
		cm_remove_remote(cm_id_priv);
>>>>>>> upstream/android-13
		spin_unlock_irqrestore(&cm.lock, flags);
		kfree(cm_id_priv->timewait_info);
		cm_id_priv->timewait_info = NULL;
	}
}

static void cm_destroy_id(struct ib_cm_id *cm_id, int err)
{
	struct cm_id_private *cm_id_priv;
	struct cm_work *work;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
<<<<<<< HEAD
retest:
	spin_lock_irq(&cm_id_priv->lock);
	switch (cm_id->state) {
	case IB_CM_LISTEN:
		spin_unlock_irq(&cm_id_priv->lock);

		spin_lock_irq(&cm.lock);
		if (--cm_id_priv->listen_sharecount > 0) {
			/* The id is still shared. */
			cm_deref_id(cm_id_priv);
			spin_unlock_irq(&cm.lock);
			return;
		}
		rb_erase(&cm_id_priv->service_node, &cm.listen_service_table);
		spin_unlock_irq(&cm.lock);
		break;
	case IB_CM_SIDR_REQ_SENT:
		cm_id->state = IB_CM_IDLE;
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		spin_unlock_irq(&cm_id_priv->lock);
		break;
	case IB_CM_SIDR_REQ_RCVD:
		spin_unlock_irq(&cm_id_priv->lock);
		cm_reject_sidr_req(cm_id_priv, IB_SIDR_REJECT);
		spin_lock_irq(&cm.lock);
		if (!RB_EMPTY_NODE(&cm_id_priv->sidr_id_node))
			rb_erase(&cm_id_priv->sidr_id_node,
				 &cm.remote_sidr_table);
		spin_unlock_irq(&cm.lock);
		break;
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		spin_unlock_irq(&cm_id_priv->lock);
		ib_send_cm_rej(cm_id, IB_CM_REJ_TIMEOUT,
			       &cm_id_priv->id.device->node_guid,
			       sizeof cm_id_priv->id.device->node_guid,
			       NULL, 0);
=======
	spin_lock_irq(&cm_id_priv->lock);
retest:
	switch (cm_id->state) {
	case IB_CM_LISTEN:
		spin_lock(&cm.lock);
		if (--cm_id_priv->listen_sharecount > 0) {
			/* The id is still shared. */
			WARN_ON(refcount_read(&cm_id_priv->refcount) == 1);
			spin_unlock(&cm.lock);
			spin_unlock_irq(&cm_id_priv->lock);
			cm_deref_id(cm_id_priv);
			return;
		}
		cm_id->state = IB_CM_IDLE;
		rb_erase(&cm_id_priv->service_node, &cm.listen_service_table);
		RB_CLEAR_NODE(&cm_id_priv->service_node);
		spin_unlock(&cm.lock);
		break;
	case IB_CM_SIDR_REQ_SENT:
		cm_id->state = IB_CM_IDLE;
		ib_cancel_mad(cm_id_priv->msg);
		break;
	case IB_CM_SIDR_REQ_RCVD:
		cm_send_sidr_rep_locked(cm_id_priv,
					&(struct ib_cm_sidr_rep_param){
						.status = IB_SIDR_REJECT });
		/* cm_send_sidr_rep_locked will not move to IDLE if it fails */
		cm_id->state = IB_CM_IDLE;
		break;
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
		ib_cancel_mad(cm_id_priv->msg);
		cm_send_rej_locked(cm_id_priv, IB_CM_REJ_TIMEOUT,
				   &cm_id_priv->id.device->node_guid,
				   sizeof(cm_id_priv->id.device->node_guid),
				   NULL, 0);
>>>>>>> upstream/android-13
		break;
	case IB_CM_REQ_RCVD:
		if (err == -ENOMEM) {
			/* Do not reject to allow future retries. */
			cm_reset_to_idle(cm_id_priv);
<<<<<<< HEAD
			spin_unlock_irq(&cm_id_priv->lock);
		} else {
			spin_unlock_irq(&cm_id_priv->lock);
			ib_send_cm_rej(cm_id, IB_CM_REJ_CONSUMER_DEFINED,
				       NULL, 0, NULL, 0);
=======
		} else {
			cm_send_rej_locked(cm_id_priv,
					   IB_CM_REJ_CONSUMER_DEFINED, NULL, 0,
					   NULL, 0);
>>>>>>> upstream/android-13
		}
		break;
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
<<<<<<< HEAD
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		/* Fall through */
	case IB_CM_MRA_REQ_SENT:
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
		spin_unlock_irq(&cm_id_priv->lock);
		ib_send_cm_rej(cm_id, IB_CM_REJ_CONSUMER_DEFINED,
			       NULL, 0, NULL, 0);
		break;
	case IB_CM_ESTABLISHED:
		spin_unlock_irq(&cm_id_priv->lock);
		if (cm_id_priv->qp_type == IB_QPT_XRC_TGT)
			break;
		ib_send_cm_dreq(cm_id, NULL, 0);
		goto retest;
	case IB_CM_DREQ_SENT:
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		cm_enter_timewait(cm_id_priv);
		spin_unlock_irq(&cm_id_priv->lock);
		break;
	case IB_CM_DREQ_RCVD:
		spin_unlock_irq(&cm_id_priv->lock);
		ib_send_cm_drep(cm_id, NULL, 0);
		break;
	default:
		spin_unlock_irq(&cm_id_priv->lock);
		break;
	}

	spin_lock_irq(&cm_id_priv->lock);
	spin_lock(&cm.lock);
	/* Required for cleanup paths related cm_req_handler() */
	if (cm_id_priv->timewait_info) {
		cm_cleanup_timewait(cm_id_priv->timewait_info);
		kfree(cm_id_priv->timewait_info);
		cm_id_priv->timewait_info = NULL;
	}
	if (!list_empty(&cm_id_priv->altr_list) &&
	    (!cm_id_priv->altr_send_port_not_ready))
		list_del(&cm_id_priv->altr_list);
	if (!list_empty(&cm_id_priv->prim_list) &&
	    (!cm_id_priv->prim_send_port_not_ready))
		list_del(&cm_id_priv->prim_list);
	spin_unlock(&cm.lock);
	spin_unlock_irq(&cm_id_priv->lock);

	cm_free_id(cm_id->local_id);
=======
		ib_cancel_mad(cm_id_priv->msg);
		cm_send_rej_locked(cm_id_priv, IB_CM_REJ_CONSUMER_DEFINED, NULL,
				   0, NULL, 0);
		goto retest;
	case IB_CM_MRA_REQ_SENT:
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
		cm_send_rej_locked(cm_id_priv, IB_CM_REJ_CONSUMER_DEFINED, NULL,
				   0, NULL, 0);
		break;
	case IB_CM_ESTABLISHED:
		if (cm_id_priv->qp_type == IB_QPT_XRC_TGT) {
			cm_id->state = IB_CM_IDLE;
			break;
		}
		cm_send_dreq_locked(cm_id_priv, NULL, 0);
		goto retest;
	case IB_CM_DREQ_SENT:
		ib_cancel_mad(cm_id_priv->msg);
		cm_enter_timewait(cm_id_priv);
		goto retest;
	case IB_CM_DREQ_RCVD:
		cm_send_drep_locked(cm_id_priv, NULL, 0);
		WARN_ON(cm_id->state != IB_CM_TIMEWAIT);
		goto retest;
	case IB_CM_TIMEWAIT:
		/*
		 * The cm_acquire_id in cm_timewait_handler will stop working
		 * once we do xa_erase below, so just move to idle here for
		 * consistency.
		 */
		cm_id->state = IB_CM_IDLE;
		break;
	case IB_CM_IDLE:
		break;
	}
	WARN_ON(cm_id->state != IB_CM_IDLE);

	spin_lock(&cm.lock);
	/* Required for cleanup paths related cm_req_handler() */
	if (cm_id_priv->timewait_info) {
		cm_remove_remote(cm_id_priv);
		kfree(cm_id_priv->timewait_info);
		cm_id_priv->timewait_info = NULL;
	}

	WARN_ON(cm_id_priv->listen_sharecount);
	WARN_ON(!RB_EMPTY_NODE(&cm_id_priv->service_node));
	if (!RB_EMPTY_NODE(&cm_id_priv->sidr_id_node))
		rb_erase(&cm_id_priv->sidr_id_node, &cm.remote_sidr_table);
	spin_unlock(&cm.lock);
	spin_unlock_irq(&cm_id_priv->lock);

	xa_erase(&cm.local_id_table, cm_local_id(cm_id->local_id));
>>>>>>> upstream/android-13
	cm_deref_id(cm_id_priv);
	wait_for_completion(&cm_id_priv->comp);
	while ((work = cm_dequeue_work(cm_id_priv)) != NULL)
		cm_free_work(work);

<<<<<<< HEAD
	rdma_destroy_ah_attr(&cm_id_priv->av.ah_attr);
	rdma_destroy_ah_attr(&cm_id_priv->alt_av.ah_attr);
	kfree(cm_id_priv->private_data);
	kfree(cm_id_priv);
=======
	cm_destroy_av(&cm_id_priv->av);
	cm_destroy_av(&cm_id_priv->alt_av);
	kfree(cm_id_priv->private_data);
	kfree_rcu(cm_id_priv, rcu);
>>>>>>> upstream/android-13
}

void ib_destroy_cm_id(struct ib_cm_id *cm_id)
{
	cm_destroy_id(cm_id, 0);
}
EXPORT_SYMBOL(ib_destroy_cm_id);

<<<<<<< HEAD
/**
 * __ib_cm_listen - Initiates listening on the specified service ID for
=======
static int cm_init_listen(struct cm_id_private *cm_id_priv, __be64 service_id,
			  __be64 service_mask)
{
	service_mask = service_mask ? service_mask : ~cpu_to_be64(0);
	service_id &= service_mask;
	if ((service_id & IB_SERVICE_ID_AGN_MASK) == IB_CM_ASSIGN_SERVICE_ID &&
	    (service_id != IB_CM_ASSIGN_SERVICE_ID))
		return -EINVAL;

	if (service_id == IB_CM_ASSIGN_SERVICE_ID) {
		cm_id_priv->id.service_id = cpu_to_be64(cm.listen_service_id++);
		cm_id_priv->id.service_mask = ~cpu_to_be64(0);
	} else {
		cm_id_priv->id.service_id = service_id;
		cm_id_priv->id.service_mask = service_mask;
	}
	return 0;
}

/**
 * ib_cm_listen - Initiates listening on the specified service ID for
>>>>>>> upstream/android-13
 *   connection and service ID resolution requests.
 * @cm_id: Connection identifier associated with the listen request.
 * @service_id: Service identifier matched against incoming connection
 *   and service ID resolution requests.  The service ID should be specified
 *   network-byte order.  If set to IB_CM_ASSIGN_SERVICE_ID, the CM will
 *   assign a service ID to the caller.
 * @service_mask: Mask applied to service ID used to listen across a
 *   range of service IDs.  If set to 0, the service ID is matched
 *   exactly.  This parameter is ignored if %service_id is set to
 *   IB_CM_ASSIGN_SERVICE_ID.
 */
<<<<<<< HEAD
static int __ib_cm_listen(struct ib_cm_id *cm_id, __be64 service_id,
			  __be64 service_mask)
{
	struct cm_id_private *cm_id_priv, *cur_cm_id_priv;
	int ret = 0;

	service_mask = service_mask ? service_mask : ~cpu_to_be64(0);
	service_id &= service_mask;
	if ((service_id & IB_SERVICE_ID_AGN_MASK) == IB_CM_ASSIGN_SERVICE_ID &&
	    (service_id != IB_CM_ASSIGN_SERVICE_ID))
		return -EINVAL;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	if (cm_id->state != IB_CM_IDLE)
		return -EINVAL;

	cm_id->state = IB_CM_LISTEN;
	++cm_id_priv->listen_sharecount;

	if (service_id == IB_CM_ASSIGN_SERVICE_ID) {
		cm_id->service_id = cpu_to_be64(cm.listen_service_id++);
		cm_id->service_mask = ~cpu_to_be64(0);
	} else {
		cm_id->service_id = service_id;
		cm_id->service_mask = service_mask;
	}
	cur_cm_id_priv = cm_insert_listen(cm_id_priv);

	if (cur_cm_id_priv) {
		cm_id->state = IB_CM_IDLE;
		--cm_id_priv->listen_sharecount;
		ret = -EBUSY;
	}
	return ret;
}

int ib_cm_listen(struct ib_cm_id *cm_id, __be64 service_id, __be64 service_mask)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm.lock, flags);
	ret = __ib_cm_listen(cm_id, service_id, service_mask);
	spin_unlock_irqrestore(&cm.lock, flags);

=======
int ib_cm_listen(struct ib_cm_id *cm_id, __be64 service_id, __be64 service_mask)
{
	struct cm_id_private *cm_id_priv =
		container_of(cm_id, struct cm_id_private, id);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id_priv->id.state != IB_CM_IDLE) {
		ret = -EINVAL;
		goto out;
	}

	ret = cm_init_listen(cm_id_priv, service_id, service_mask);
	if (ret)
		goto out;

	if (!cm_insert_listen(cm_id_priv, NULL)) {
		ret = -EBUSY;
		goto out;
	}

	cm_id_priv->id.state = IB_CM_LISTEN;
	ret = 0;

out:
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_cm_listen);

/**
<<<<<<< HEAD
 * Create a new listening ib_cm_id and listen on the given service ID.
=======
 * ib_cm_insert_listen - Create a new listening ib_cm_id and listen on
 *			 the given service ID.
>>>>>>> upstream/android-13
 *
 * If there's an existing ID listening on that same device and service ID,
 * return it.
 *
 * @device: Device associated with the cm_id.  All related communication will
 * be associated with the specified device.
 * @cm_handler: Callback invoked to notify the user of CM events.
 * @service_id: Service identifier matched against incoming connection
 *   and service ID resolution requests.  The service ID should be specified
 *   network-byte order.  If set to IB_CM_ASSIGN_SERVICE_ID, the CM will
 *   assign a service ID to the caller.
 *
 * Callers should call ib_destroy_cm_id when done with the listener ID.
 */
struct ib_cm_id *ib_cm_insert_listen(struct ib_device *device,
				     ib_cm_handler cm_handler,
				     __be64 service_id)
{
<<<<<<< HEAD
	struct cm_id_private *cm_id_priv;
	struct ib_cm_id *cm_id;
	unsigned long flags;
	int err = 0;

	/* Create an ID in advance, since the creation may sleep */
	cm_id = ib_create_cm_id(device, cm_handler, NULL);
	if (IS_ERR(cm_id))
		return cm_id;

	spin_lock_irqsave(&cm.lock, flags);

	if (service_id == IB_CM_ASSIGN_SERVICE_ID)
		goto new_id;

	/* Find an existing ID */
	cm_id_priv = cm_find_listen(device, service_id);
	if (cm_id_priv) {
		if (cm_id->cm_handler != cm_handler || cm_id->context) {
			/* Sharing an ib_cm_id with different handlers is not
			 * supported */
			spin_unlock_irqrestore(&cm.lock, flags);
			ib_destroy_cm_id(cm_id);
			return ERR_PTR(-EINVAL);
		}
		atomic_inc(&cm_id_priv->refcount);
		++cm_id_priv->listen_sharecount;
		spin_unlock_irqrestore(&cm.lock, flags);

		ib_destroy_cm_id(cm_id);
		cm_id = &cm_id_priv->id;
		return cm_id;
	}

new_id:
	/* Use newly created ID */
	err = __ib_cm_listen(cm_id, service_id, 0);

	spin_unlock_irqrestore(&cm.lock, flags);

	if (err) {
		ib_destroy_cm_id(cm_id);
		return ERR_PTR(err);
	}
	return cm_id;
=======
	struct cm_id_private *listen_id_priv;
	struct cm_id_private *cm_id_priv;
	int err = 0;

	/* Create an ID in advance, since the creation may sleep */
	cm_id_priv = cm_alloc_id_priv(device, cm_handler, NULL);
	if (IS_ERR(cm_id_priv))
		return ERR_CAST(cm_id_priv);

	err = cm_init_listen(cm_id_priv, service_id, 0);
	if (err)
		return ERR_PTR(err);

	spin_lock_irq(&cm_id_priv->lock);
	listen_id_priv = cm_insert_listen(cm_id_priv, cm_handler);
	if (listen_id_priv != cm_id_priv) {
		spin_unlock_irq(&cm_id_priv->lock);
		ib_destroy_cm_id(&cm_id_priv->id);
		if (!listen_id_priv)
			return ERR_PTR(-EINVAL);
		return &listen_id_priv->id;
	}
	cm_id_priv->id.state = IB_CM_LISTEN;
	spin_unlock_irq(&cm_id_priv->lock);

	/*
	 * A listen ID does not need to be in the xarray since it does not
	 * receive mads, is not placed in the remote_id or remote_qpn rbtree,
	 * and does not enter timewait.
	 */

	return &cm_id_priv->id;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(ib_cm_insert_listen);

static __be64 cm_form_tid(struct cm_id_private *cm_id_priv)
{
<<<<<<< HEAD
	u64 hi_tid, low_tid;

	hi_tid   = ((u64) cm_id_priv->av.port->mad_agent->hi_tid) << 32;
	low_tid  = (u64)cm_id_priv->id.local_id;
=======
	u64 hi_tid = 0, low_tid;

	lockdep_assert_held(&cm_id_priv->lock);

	low_tid = (u64)cm_id_priv->id.local_id;
	if (!cm_id_priv->av.port)
		return cpu_to_be64(low_tid);

	spin_lock(&cm_id_priv->av.port->cm_dev->mad_agent_lock);
	if (cm_id_priv->av.port->mad_agent)
		hi_tid = ((u64)cm_id_priv->av.port->mad_agent->hi_tid) << 32;
	spin_unlock(&cm_id_priv->av.port->cm_dev->mad_agent_lock);
>>>>>>> upstream/android-13
	return cpu_to_be64(hi_tid | low_tid);
}

static void cm_format_mad_hdr(struct ib_mad_hdr *hdr,
			      __be16 attr_id, __be64 tid)
{
	hdr->base_version  = IB_MGMT_BASE_VERSION;
	hdr->mgmt_class	   = IB_MGMT_CLASS_CM;
	hdr->class_version = IB_CM_CLASS_VERSION;
	hdr->method	   = IB_MGMT_METHOD_SEND;
	hdr->attr_id	   = attr_id;
	hdr->tid	   = tid;
}

<<<<<<< HEAD
=======
static void cm_format_mad_ece_hdr(struct ib_mad_hdr *hdr, __be16 attr_id,
				  __be64 tid, u32 attr_mod)
{
	cm_format_mad_hdr(hdr, attr_id, tid);
	hdr->attr_mod = cpu_to_be32(attr_mod);
}

>>>>>>> upstream/android-13
static void cm_format_req(struct cm_req_msg *req_msg,
			  struct cm_id_private *cm_id_priv,
			  struct ib_cm_req_param *param)
{
	struct sa_path_rec *pri_path = param->primary_path;
	struct sa_path_rec *alt_path = param->alternate_path;
	bool pri_ext = false;

	if (pri_path->rec_type == SA_PATH_REC_TYPE_OPA)
		pri_ext = opa_is_extended_lid(pri_path->opa.dlid,
					      pri_path->opa.slid);

<<<<<<< HEAD
	cm_format_mad_hdr(&req_msg->hdr, CM_REQ_ATTR_ID,
			  cm_form_tid(cm_id_priv));

	req_msg->local_comm_id = cm_id_priv->id.local_id;
	req_msg->service_id = param->service_id;
	req_msg->local_ca_guid = cm_id_priv->id.device->node_guid;
	cm_req_set_local_qpn(req_msg, cpu_to_be32(param->qp_num));
	cm_req_set_init_depth(req_msg, param->initiator_depth);
	cm_req_set_remote_resp_timeout(req_msg,
				       param->remote_cm_response_timeout);
	cm_req_set_qp_type(req_msg, param->qp_type);
	cm_req_set_flow_ctrl(req_msg, param->flow_control);
	cm_req_set_starting_psn(req_msg, cpu_to_be32(param->starting_psn));
	cm_req_set_local_resp_timeout(req_msg,
				      param->local_cm_response_timeout);
	req_msg->pkey = param->primary_path->pkey;
	cm_req_set_path_mtu(req_msg, param->primary_path->mtu);
	cm_req_set_max_cm_retries(req_msg, param->max_cm_retries);

	if (param->qp_type != IB_QPT_XRC_INI) {
		cm_req_set_resp_res(req_msg, param->responder_resources);
		cm_req_set_retry_count(req_msg, param->retry_count);
		cm_req_set_rnr_retry_count(req_msg, param->rnr_retry_count);
		cm_req_set_srq(req_msg, param->srq);
	}

	req_msg->primary_local_gid = pri_path->sgid;
	req_msg->primary_remote_gid = pri_path->dgid;
	if (pri_ext) {
		req_msg->primary_local_gid.global.interface_id
			= OPA_MAKE_ID(be32_to_cpu(pri_path->opa.slid));
		req_msg->primary_remote_gid.global.interface_id
			= OPA_MAKE_ID(be32_to_cpu(pri_path->opa.dlid));
	}
	if (pri_path->hop_limit <= 1) {
		req_msg->primary_local_lid = pri_ext ? 0 :
			htons(ntohl(sa_path_get_slid(pri_path)));
		req_msg->primary_remote_lid = pri_ext ? 0 :
			htons(ntohl(sa_path_get_dlid(pri_path)));
	} else {
		/* Work-around until there's a way to obtain remote LID info */
		req_msg->primary_local_lid = IB_LID_PERMISSIVE;
		req_msg->primary_remote_lid = IB_LID_PERMISSIVE;
	}
	cm_req_set_primary_flow_label(req_msg, pri_path->flow_label);
	cm_req_set_primary_packet_rate(req_msg, pri_path->rate);
	req_msg->primary_traffic_class = pri_path->traffic_class;
	req_msg->primary_hop_limit = pri_path->hop_limit;
	cm_req_set_primary_sl(req_msg, pri_path->sl);
	cm_req_set_primary_subnet_local(req_msg, (pri_path->hop_limit <= 1));
	cm_req_set_primary_local_ack_timeout(req_msg,
=======
	cm_format_mad_ece_hdr(&req_msg->hdr, CM_REQ_ATTR_ID,
			      cm_form_tid(cm_id_priv), param->ece.attr_mod);

	IBA_SET(CM_REQ_LOCAL_COMM_ID, req_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_REQ_SERVICE_ID, req_msg, be64_to_cpu(param->service_id));
	IBA_SET(CM_REQ_LOCAL_CA_GUID, req_msg,
		be64_to_cpu(cm_id_priv->id.device->node_guid));
	IBA_SET(CM_REQ_LOCAL_QPN, req_msg, param->qp_num);
	IBA_SET(CM_REQ_INITIATOR_DEPTH, req_msg, param->initiator_depth);
	IBA_SET(CM_REQ_REMOTE_CM_RESPONSE_TIMEOUT, req_msg,
		param->remote_cm_response_timeout);
	cm_req_set_qp_type(req_msg, param->qp_type);
	IBA_SET(CM_REQ_END_TO_END_FLOW_CONTROL, req_msg, param->flow_control);
	IBA_SET(CM_REQ_STARTING_PSN, req_msg, param->starting_psn);
	IBA_SET(CM_REQ_LOCAL_CM_RESPONSE_TIMEOUT, req_msg,
		param->local_cm_response_timeout);
	IBA_SET(CM_REQ_PARTITION_KEY, req_msg,
		be16_to_cpu(param->primary_path->pkey));
	IBA_SET(CM_REQ_PATH_PACKET_PAYLOAD_MTU, req_msg,
		param->primary_path->mtu);
	IBA_SET(CM_REQ_MAX_CM_RETRIES, req_msg, param->max_cm_retries);

	if (param->qp_type != IB_QPT_XRC_INI) {
		IBA_SET(CM_REQ_RESPONDER_RESOURCES, req_msg,
			param->responder_resources);
		IBA_SET(CM_REQ_RETRY_COUNT, req_msg, param->retry_count);
		IBA_SET(CM_REQ_RNR_RETRY_COUNT, req_msg,
			param->rnr_retry_count);
		IBA_SET(CM_REQ_SRQ, req_msg, param->srq);
	}

	*IBA_GET_MEM_PTR(CM_REQ_PRIMARY_LOCAL_PORT_GID, req_msg) =
		pri_path->sgid;
	*IBA_GET_MEM_PTR(CM_REQ_PRIMARY_REMOTE_PORT_GID, req_msg) =
		pri_path->dgid;
	if (pri_ext) {
		IBA_GET_MEM_PTR(CM_REQ_PRIMARY_LOCAL_PORT_GID, req_msg)
			->global.interface_id =
			OPA_MAKE_ID(be32_to_cpu(pri_path->opa.slid));
		IBA_GET_MEM_PTR(CM_REQ_PRIMARY_REMOTE_PORT_GID, req_msg)
			->global.interface_id =
			OPA_MAKE_ID(be32_to_cpu(pri_path->opa.dlid));
	}
	if (pri_path->hop_limit <= 1) {
		IBA_SET(CM_REQ_PRIMARY_LOCAL_PORT_LID, req_msg,
			be16_to_cpu(pri_ext ? 0 :
					      htons(ntohl(sa_path_get_slid(
						      pri_path)))));
		IBA_SET(CM_REQ_PRIMARY_REMOTE_PORT_LID, req_msg,
			be16_to_cpu(pri_ext ? 0 :
					      htons(ntohl(sa_path_get_dlid(
						      pri_path)))));
	} else {
		/* Work-around until there's a way to obtain remote LID info */
		IBA_SET(CM_REQ_PRIMARY_LOCAL_PORT_LID, req_msg,
			be16_to_cpu(IB_LID_PERMISSIVE));
		IBA_SET(CM_REQ_PRIMARY_REMOTE_PORT_LID, req_msg,
			be16_to_cpu(IB_LID_PERMISSIVE));
	}
	IBA_SET(CM_REQ_PRIMARY_FLOW_LABEL, req_msg,
		be32_to_cpu(pri_path->flow_label));
	IBA_SET(CM_REQ_PRIMARY_PACKET_RATE, req_msg, pri_path->rate);
	IBA_SET(CM_REQ_PRIMARY_TRAFFIC_CLASS, req_msg, pri_path->traffic_class);
	IBA_SET(CM_REQ_PRIMARY_HOP_LIMIT, req_msg, pri_path->hop_limit);
	IBA_SET(CM_REQ_PRIMARY_SL, req_msg, pri_path->sl);
	IBA_SET(CM_REQ_PRIMARY_SUBNET_LOCAL, req_msg,
		(pri_path->hop_limit <= 1));
	IBA_SET(CM_REQ_PRIMARY_LOCAL_ACK_TIMEOUT, req_msg,
>>>>>>> upstream/android-13
		cm_ack_timeout(cm_id_priv->av.port->cm_dev->ack_delay,
			       pri_path->packet_life_time));

	if (alt_path) {
		bool alt_ext = false;

		if (alt_path->rec_type == SA_PATH_REC_TYPE_OPA)
			alt_ext = opa_is_extended_lid(alt_path->opa.dlid,
						      alt_path->opa.slid);

<<<<<<< HEAD
		req_msg->alt_local_gid = alt_path->sgid;
		req_msg->alt_remote_gid = alt_path->dgid;
		if (alt_ext) {
			req_msg->alt_local_gid.global.interface_id
				= OPA_MAKE_ID(be32_to_cpu(alt_path->opa.slid));
			req_msg->alt_remote_gid.global.interface_id
				= OPA_MAKE_ID(be32_to_cpu(alt_path->opa.dlid));
		}
		if (alt_path->hop_limit <= 1) {
			req_msg->alt_local_lid = alt_ext ? 0 :
				htons(ntohl(sa_path_get_slid(alt_path)));
			req_msg->alt_remote_lid = alt_ext ? 0 :
				htons(ntohl(sa_path_get_dlid(alt_path)));
		} else {
			req_msg->alt_local_lid = IB_LID_PERMISSIVE;
			req_msg->alt_remote_lid = IB_LID_PERMISSIVE;
		}
		cm_req_set_alt_flow_label(req_msg,
					  alt_path->flow_label);
		cm_req_set_alt_packet_rate(req_msg, alt_path->rate);
		req_msg->alt_traffic_class = alt_path->traffic_class;
		req_msg->alt_hop_limit = alt_path->hop_limit;
		cm_req_set_alt_sl(req_msg, alt_path->sl);
		cm_req_set_alt_subnet_local(req_msg, (alt_path->hop_limit <= 1));
		cm_req_set_alt_local_ack_timeout(req_msg,
			cm_ack_timeout(cm_id_priv->av.port->cm_dev->ack_delay,
				       alt_path->packet_life_time));
	}

	if (param->private_data && param->private_data_len)
		memcpy(req_msg->private_data, param->private_data,
		       param->private_data_len);
=======
		*IBA_GET_MEM_PTR(CM_REQ_ALTERNATE_LOCAL_PORT_GID, req_msg) =
			alt_path->sgid;
		*IBA_GET_MEM_PTR(CM_REQ_ALTERNATE_REMOTE_PORT_GID, req_msg) =
			alt_path->dgid;
		if (alt_ext) {
			IBA_GET_MEM_PTR(CM_REQ_ALTERNATE_LOCAL_PORT_GID,
					req_msg)
				->global.interface_id =
				OPA_MAKE_ID(be32_to_cpu(alt_path->opa.slid));
			IBA_GET_MEM_PTR(CM_REQ_ALTERNATE_REMOTE_PORT_GID,
					req_msg)
				->global.interface_id =
				OPA_MAKE_ID(be32_to_cpu(alt_path->opa.dlid));
		}
		if (alt_path->hop_limit <= 1) {
			IBA_SET(CM_REQ_ALTERNATE_LOCAL_PORT_LID, req_msg,
				be16_to_cpu(
					alt_ext ? 0 :
						  htons(ntohl(sa_path_get_slid(
							  alt_path)))));
			IBA_SET(CM_REQ_ALTERNATE_REMOTE_PORT_LID, req_msg,
				be16_to_cpu(
					alt_ext ? 0 :
						  htons(ntohl(sa_path_get_dlid(
							  alt_path)))));
		} else {
			IBA_SET(CM_REQ_ALTERNATE_LOCAL_PORT_LID, req_msg,
				be16_to_cpu(IB_LID_PERMISSIVE));
			IBA_SET(CM_REQ_ALTERNATE_REMOTE_PORT_LID, req_msg,
				be16_to_cpu(IB_LID_PERMISSIVE));
		}
		IBA_SET(CM_REQ_ALTERNATE_FLOW_LABEL, req_msg,
			be32_to_cpu(alt_path->flow_label));
		IBA_SET(CM_REQ_ALTERNATE_PACKET_RATE, req_msg, alt_path->rate);
		IBA_SET(CM_REQ_ALTERNATE_TRAFFIC_CLASS, req_msg,
			alt_path->traffic_class);
		IBA_SET(CM_REQ_ALTERNATE_HOP_LIMIT, req_msg,
			alt_path->hop_limit);
		IBA_SET(CM_REQ_ALTERNATE_SL, req_msg, alt_path->sl);
		IBA_SET(CM_REQ_ALTERNATE_SUBNET_LOCAL, req_msg,
			(alt_path->hop_limit <= 1));
		IBA_SET(CM_REQ_ALTERNATE_LOCAL_ACK_TIMEOUT, req_msg,
			cm_ack_timeout(cm_id_priv->av.port->cm_dev->ack_delay,
				       alt_path->packet_life_time));
	}
	IBA_SET(CM_REQ_VENDOR_ID, req_msg, param->ece.vendor_id);

	if (param->private_data && param->private_data_len)
		IBA_SET_MEM(CM_REQ_PRIVATE_DATA, req_msg, param->private_data,
			    param->private_data_len);
>>>>>>> upstream/android-13
}

static int cm_validate_req_param(struct ib_cm_req_param *param)
{
<<<<<<< HEAD
	/* peer-to-peer not supported */
	if (param->peer_to_peer)
		return -EINVAL;

=======
>>>>>>> upstream/android-13
	if (!param->primary_path)
		return -EINVAL;

	if (param->qp_type != IB_QPT_RC && param->qp_type != IB_QPT_UC &&
	    param->qp_type != IB_QPT_XRC_INI)
		return -EINVAL;

	if (param->private_data &&
	    param->private_data_len > IB_CM_REQ_PRIVATE_DATA_SIZE)
		return -EINVAL;

	if (param->alternate_path &&
	    (param->alternate_path->pkey != param->primary_path->pkey ||
	     param->alternate_path->mtu != param->primary_path->mtu))
		return -EINVAL;

	return 0;
}

int ib_send_cm_req(struct ib_cm_id *cm_id,
		   struct ib_cm_req_param *param)
{
<<<<<<< HEAD
	struct cm_id_private *cm_id_priv;
=======
	struct cm_av av = {}, alt_av = {};
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
>>>>>>> upstream/android-13
	struct cm_req_msg *req_msg;
	unsigned long flags;
	int ret;

	ret = cm_validate_req_param(param);
	if (ret)
		return ret;

	/* Verify that we're not in timewait. */
	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_IDLE || WARN_ON(cm_id_priv->timewait_info)) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
<<<<<<< HEAD
		ret = -EINVAL;
		goto out;
=======
		return -EINVAL;
>>>>>>> upstream/android-13
	}
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);

	cm_id_priv->timewait_info = cm_create_timewait_info(cm_id_priv->
							    id.local_id);
	if (IS_ERR(cm_id_priv->timewait_info)) {
		ret = PTR_ERR(cm_id_priv->timewait_info);
		cm_id_priv->timewait_info = NULL;
<<<<<<< HEAD
		goto out;
	}

	ret = cm_init_av_by_path(param->primary_path,
				 param->ppath_sgid_attr, &cm_id_priv->av,
				 cm_id_priv);
	if (ret)
		goto out;
	if (param->alternate_path) {
		ret = cm_init_av_by_path(param->alternate_path, NULL,
					 &cm_id_priv->alt_av, cm_id_priv);
		if (ret)
			goto out;
=======
		return ret;
	}

	ret = cm_init_av_by_path(param->primary_path,
				 param->ppath_sgid_attr, &av);
	if (ret)
		return ret;
	if (param->alternate_path) {
		ret = cm_init_av_by_path(param->alternate_path, NULL,
					 &alt_av);
		if (ret) {
			cm_destroy_av(&av);
			return ret;
		}
>>>>>>> upstream/android-13
	}
	cm_id->service_id = param->service_id;
	cm_id->service_mask = ~cpu_to_be64(0);
	cm_id_priv->timeout_ms = cm_convert_to_ms(
				    param->primary_path->packet_life_time) * 2 +
				 cm_convert_to_ms(
				    param->remote_cm_response_timeout);
	cm_id_priv->max_cm_retries = param->max_cm_retries;
	cm_id_priv->initiator_depth = param->initiator_depth;
	cm_id_priv->responder_resources = param->responder_resources;
	cm_id_priv->retry_count = param->retry_count;
	cm_id_priv->path_mtu = param->primary_path->mtu;
	cm_id_priv->pkey = param->primary_path->pkey;
	cm_id_priv->qp_type = param->qp_type;

<<<<<<< HEAD
	ret = cm_alloc_msg(cm_id_priv, &cm_id_priv->msg);
	if (ret)
		goto out;

	req_msg = (struct cm_req_msg *) cm_id_priv->msg->mad;
	cm_format_req(req_msg, cm_id_priv, param);
	cm_id_priv->tid = req_msg->hdr.tid;
	cm_id_priv->msg->timeout_ms = cm_id_priv->timeout_ms;
	cm_id_priv->msg->context[1] = (void *) (unsigned long) IB_CM_REQ_SENT;

	cm_id_priv->local_qpn = cm_req_get_local_qpn(req_msg);
	cm_id_priv->rq_psn = cm_req_get_starting_psn(req_msg);

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	ret = ib_post_send_mad(cm_id_priv->msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		goto error2;
	}
=======
	spin_lock_irqsave(&cm_id_priv->lock, flags);

	cm_move_av_from_path(&cm_id_priv->av, &av);
	if (param->alternate_path)
		cm_move_av_from_path(&cm_id_priv->alt_av, &alt_av);

	msg = cm_alloc_priv_msg(cm_id_priv);
	if (IS_ERR(msg)) {
		ret = PTR_ERR(msg);
		goto out_unlock;
	}

	req_msg = (struct cm_req_msg *)msg->mad;
	cm_format_req(req_msg, cm_id_priv, param);
	cm_id_priv->tid = req_msg->hdr.tid;
	msg->timeout_ms = cm_id_priv->timeout_ms;
	msg->context[1] = (void *)(unsigned long)IB_CM_REQ_SENT;

	cm_id_priv->local_qpn = cpu_to_be32(IBA_GET(CM_REQ_LOCAL_QPN, req_msg));
	cm_id_priv->rq_psn = cpu_to_be32(IBA_GET(CM_REQ_STARTING_PSN, req_msg));

	trace_icm_send_req(&cm_id_priv->id);
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		goto out_free;
>>>>>>> upstream/android-13
	BUG_ON(cm_id->state != IB_CM_IDLE);
	cm_id->state = IB_CM_REQ_SENT;
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return 0;
<<<<<<< HEAD

error2:	cm_free_msg(cm_id_priv->msg);
out:	return ret;
=======
out_free:
	cm_free_priv_msg(msg);
out_unlock:
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return ret;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(ib_send_cm_req);

static int cm_issue_rej(struct cm_port *port,
			struct ib_mad_recv_wc *mad_recv_wc,
			enum ib_cm_rej_reason reason,
			enum cm_msg_response msg_rejected,
			void *ari, u8 ari_length)
{
	struct ib_mad_send_buf *msg = NULL;
	struct cm_rej_msg *rej_msg, *rcv_msg;
	int ret;

	ret = cm_alloc_response_msg(port, mad_recv_wc, &msg);
	if (ret)
		return ret;

	/* We just need common CM header information.  Cast to any message. */
	rcv_msg = (struct cm_rej_msg *) mad_recv_wc->recv_buf.mad;
	rej_msg = (struct cm_rej_msg *) msg->mad;

	cm_format_mad_hdr(&rej_msg->hdr, CM_REJ_ATTR_ID, rcv_msg->hdr.tid);
<<<<<<< HEAD
	rej_msg->remote_comm_id = rcv_msg->local_comm_id;
	rej_msg->local_comm_id = rcv_msg->remote_comm_id;
	cm_rej_set_msg_rejected(rej_msg, msg_rejected);
	rej_msg->reason = cpu_to_be16(reason);

	if (ari && ari_length) {
		cm_rej_set_reject_info_len(rej_msg, ari_length);
		memcpy(rej_msg->ari, ari, ari_length);
	}

	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		cm_free_msg(msg);
=======
	IBA_SET(CM_REJ_REMOTE_COMM_ID, rej_msg,
		IBA_GET(CM_REJ_LOCAL_COMM_ID, rcv_msg));
	IBA_SET(CM_REJ_LOCAL_COMM_ID, rej_msg,
		IBA_GET(CM_REJ_REMOTE_COMM_ID, rcv_msg));
	IBA_SET(CM_REJ_MESSAGE_REJECTED, rej_msg, msg_rejected);
	IBA_SET(CM_REJ_REASON, rej_msg, reason);

	if (ari && ari_length) {
		IBA_SET(CM_REJ_REJECTED_INFO_LENGTH, rej_msg, ari_length);
		IBA_SET_MEM(CM_REJ_ARI, rej_msg, ari, ari_length);
	}

	trace_icm_issue_rej(
		IBA_GET(CM_REJ_LOCAL_COMM_ID, rcv_msg),
		IBA_GET(CM_REJ_REMOTE_COMM_ID, rcv_msg));
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		cm_free_response_msg(msg);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static inline int cm_is_active_peer(__be64 local_ca_guid, __be64 remote_ca_guid,
				    __be32 local_qpn, __be32 remote_qpn)
{
	return (be64_to_cpu(local_ca_guid) > be64_to_cpu(remote_ca_guid) ||
		((local_ca_guid == remote_ca_guid) &&
		 (be32_to_cpu(local_qpn) > be32_to_cpu(remote_qpn))));
}

static bool cm_req_has_alt_path(struct cm_req_msg *req_msg)
{
	return ((req_msg->alt_local_lid) ||
		(ib_is_opa_gid(&req_msg->alt_local_gid)));
}

static void cm_path_set_rec_type(struct ib_device *ib_device, u8 port_num,
=======
static bool cm_req_has_alt_path(struct cm_req_msg *req_msg)
{
	return ((cpu_to_be16(
			IBA_GET(CM_REQ_ALTERNATE_LOCAL_PORT_LID, req_msg))) ||
		(ib_is_opa_gid(IBA_GET_MEM_PTR(CM_REQ_ALTERNATE_LOCAL_PORT_GID,
					       req_msg))));
}

static void cm_path_set_rec_type(struct ib_device *ib_device, u32 port_num,
>>>>>>> upstream/android-13
				 struct sa_path_rec *path, union ib_gid *gid)
{
	if (ib_is_opa_gid(gid) && rdma_cap_opa_ah(ib_device, port_num))
		path->rec_type = SA_PATH_REC_TYPE_OPA;
	else
		path->rec_type = SA_PATH_REC_TYPE_IB;
}

static void cm_format_path_lid_from_req(struct cm_req_msg *req_msg,
					struct sa_path_rec *primary_path,
					struct sa_path_rec *alt_path)
{
	u32 lid;

	if (primary_path->rec_type != SA_PATH_REC_TYPE_OPA) {
		sa_path_set_dlid(primary_path,
<<<<<<< HEAD
				 ntohs(req_msg->primary_local_lid));
		sa_path_set_slid(primary_path,
				 ntohs(req_msg->primary_remote_lid));
	} else {
		lid = opa_get_lid_from_gid(&req_msg->primary_local_gid);
		sa_path_set_dlid(primary_path, lid);

		lid = opa_get_lid_from_gid(&req_msg->primary_remote_gid);
=======
				 IBA_GET(CM_REQ_PRIMARY_LOCAL_PORT_LID,
					 req_msg));
		sa_path_set_slid(primary_path,
				 IBA_GET(CM_REQ_PRIMARY_REMOTE_PORT_LID,
					 req_msg));
	} else {
		lid = opa_get_lid_from_gid(IBA_GET_MEM_PTR(
			CM_REQ_PRIMARY_LOCAL_PORT_GID, req_msg));
		sa_path_set_dlid(primary_path, lid);

		lid = opa_get_lid_from_gid(IBA_GET_MEM_PTR(
			CM_REQ_PRIMARY_REMOTE_PORT_GID, req_msg));
>>>>>>> upstream/android-13
		sa_path_set_slid(primary_path, lid);
	}

	if (!cm_req_has_alt_path(req_msg))
		return;

	if (alt_path->rec_type != SA_PATH_REC_TYPE_OPA) {
<<<<<<< HEAD
		sa_path_set_dlid(alt_path, ntohs(req_msg->alt_local_lid));
		sa_path_set_slid(alt_path, ntohs(req_msg->alt_remote_lid));
	} else {
		lid = opa_get_lid_from_gid(&req_msg->alt_local_gid);
		sa_path_set_dlid(alt_path, lid);

		lid = opa_get_lid_from_gid(&req_msg->alt_remote_gid);
=======
		sa_path_set_dlid(alt_path,
				 IBA_GET(CM_REQ_ALTERNATE_LOCAL_PORT_LID,
					 req_msg));
		sa_path_set_slid(alt_path,
				 IBA_GET(CM_REQ_ALTERNATE_REMOTE_PORT_LID,
					 req_msg));
	} else {
		lid = opa_get_lid_from_gid(IBA_GET_MEM_PTR(
			CM_REQ_ALTERNATE_LOCAL_PORT_GID, req_msg));
		sa_path_set_dlid(alt_path, lid);

		lid = opa_get_lid_from_gid(IBA_GET_MEM_PTR(
			CM_REQ_ALTERNATE_REMOTE_PORT_GID, req_msg));
>>>>>>> upstream/android-13
		sa_path_set_slid(alt_path, lid);
	}
}

static void cm_format_paths_from_req(struct cm_req_msg *req_msg,
				     struct sa_path_rec *primary_path,
				     struct sa_path_rec *alt_path)
{
<<<<<<< HEAD
	primary_path->dgid = req_msg->primary_local_gid;
	primary_path->sgid = req_msg->primary_remote_gid;
	primary_path->flow_label = cm_req_get_primary_flow_label(req_msg);
	primary_path->hop_limit = req_msg->primary_hop_limit;
	primary_path->traffic_class = req_msg->primary_traffic_class;
	primary_path->reversible = 1;
	primary_path->pkey = req_msg->pkey;
	primary_path->sl = cm_req_get_primary_sl(req_msg);
	primary_path->mtu_selector = IB_SA_EQ;
	primary_path->mtu = cm_req_get_path_mtu(req_msg);
	primary_path->rate_selector = IB_SA_EQ;
	primary_path->rate = cm_req_get_primary_packet_rate(req_msg);
	primary_path->packet_life_time_selector = IB_SA_EQ;
	primary_path->packet_life_time =
		cm_req_get_primary_local_ack_timeout(req_msg);
	primary_path->packet_life_time -= (primary_path->packet_life_time > 0);
	primary_path->service_id = req_msg->service_id;
=======
	primary_path->dgid =
		*IBA_GET_MEM_PTR(CM_REQ_PRIMARY_LOCAL_PORT_GID, req_msg);
	primary_path->sgid =
		*IBA_GET_MEM_PTR(CM_REQ_PRIMARY_REMOTE_PORT_GID, req_msg);
	primary_path->flow_label =
		cpu_to_be32(IBA_GET(CM_REQ_PRIMARY_FLOW_LABEL, req_msg));
	primary_path->hop_limit = IBA_GET(CM_REQ_PRIMARY_HOP_LIMIT, req_msg);
	primary_path->traffic_class =
		IBA_GET(CM_REQ_PRIMARY_TRAFFIC_CLASS, req_msg);
	primary_path->reversible = 1;
	primary_path->pkey =
		cpu_to_be16(IBA_GET(CM_REQ_PARTITION_KEY, req_msg));
	primary_path->sl = IBA_GET(CM_REQ_PRIMARY_SL, req_msg);
	primary_path->mtu_selector = IB_SA_EQ;
	primary_path->mtu = IBA_GET(CM_REQ_PATH_PACKET_PAYLOAD_MTU, req_msg);
	primary_path->rate_selector = IB_SA_EQ;
	primary_path->rate = IBA_GET(CM_REQ_PRIMARY_PACKET_RATE, req_msg);
	primary_path->packet_life_time_selector = IB_SA_EQ;
	primary_path->packet_life_time =
		IBA_GET(CM_REQ_PRIMARY_LOCAL_ACK_TIMEOUT, req_msg);
	primary_path->packet_life_time -= (primary_path->packet_life_time > 0);
	primary_path->service_id =
		cpu_to_be64(IBA_GET(CM_REQ_SERVICE_ID, req_msg));
>>>>>>> upstream/android-13
	if (sa_path_is_roce(primary_path))
		primary_path->roce.route_resolved = false;

	if (cm_req_has_alt_path(req_msg)) {
<<<<<<< HEAD
		alt_path->dgid = req_msg->alt_local_gid;
		alt_path->sgid = req_msg->alt_remote_gid;
		alt_path->flow_label = cm_req_get_alt_flow_label(req_msg);
		alt_path->hop_limit = req_msg->alt_hop_limit;
		alt_path->traffic_class = req_msg->alt_traffic_class;
		alt_path->reversible = 1;
		alt_path->pkey = req_msg->pkey;
		alt_path->sl = cm_req_get_alt_sl(req_msg);
		alt_path->mtu_selector = IB_SA_EQ;
		alt_path->mtu = cm_req_get_path_mtu(req_msg);
		alt_path->rate_selector = IB_SA_EQ;
		alt_path->rate = cm_req_get_alt_packet_rate(req_msg);
		alt_path->packet_life_time_selector = IB_SA_EQ;
		alt_path->packet_life_time =
			cm_req_get_alt_local_ack_timeout(req_msg);
		alt_path->packet_life_time -= (alt_path->packet_life_time > 0);
		alt_path->service_id = req_msg->service_id;
=======
		alt_path->dgid = *IBA_GET_MEM_PTR(
			CM_REQ_ALTERNATE_LOCAL_PORT_GID, req_msg);
		alt_path->sgid = *IBA_GET_MEM_PTR(
			CM_REQ_ALTERNATE_REMOTE_PORT_GID, req_msg);
		alt_path->flow_label = cpu_to_be32(
			IBA_GET(CM_REQ_ALTERNATE_FLOW_LABEL, req_msg));
		alt_path->hop_limit =
			IBA_GET(CM_REQ_ALTERNATE_HOP_LIMIT, req_msg);
		alt_path->traffic_class =
			IBA_GET(CM_REQ_ALTERNATE_TRAFFIC_CLASS, req_msg);
		alt_path->reversible = 1;
		alt_path->pkey =
			cpu_to_be16(IBA_GET(CM_REQ_PARTITION_KEY, req_msg));
		alt_path->sl = IBA_GET(CM_REQ_ALTERNATE_SL, req_msg);
		alt_path->mtu_selector = IB_SA_EQ;
		alt_path->mtu =
			IBA_GET(CM_REQ_PATH_PACKET_PAYLOAD_MTU, req_msg);
		alt_path->rate_selector = IB_SA_EQ;
		alt_path->rate = IBA_GET(CM_REQ_ALTERNATE_PACKET_RATE, req_msg);
		alt_path->packet_life_time_selector = IB_SA_EQ;
		alt_path->packet_life_time =
			IBA_GET(CM_REQ_ALTERNATE_LOCAL_ACK_TIMEOUT, req_msg);
		alt_path->packet_life_time -= (alt_path->packet_life_time > 0);
		alt_path->service_id =
			cpu_to_be64(IBA_GET(CM_REQ_SERVICE_ID, req_msg));
>>>>>>> upstream/android-13

		if (sa_path_is_roce(alt_path))
			alt_path->roce.route_resolved = false;
	}
	cm_format_path_lid_from_req(req_msg, primary_path, alt_path);
}

static u16 cm_get_bth_pkey(struct cm_work *work)
{
	struct ib_device *ib_dev = work->port->cm_dev->ib_device;
<<<<<<< HEAD
	u8 port_num = work->port->port_num;
=======
	u32 port_num = work->port->port_num;
>>>>>>> upstream/android-13
	u16 pkey_index = work->mad_recv_wc->wc->pkey_index;
	u16 pkey;
	int ret;

	ret = ib_get_cached_pkey(ib_dev, port_num, pkey_index, &pkey);
	if (ret) {
<<<<<<< HEAD
		dev_warn_ratelimited(&ib_dev->dev, "ib_cm: Couldn't retrieve pkey for incoming request (port %d, pkey index %d). %d\n",
=======
		dev_warn_ratelimited(&ib_dev->dev, "ib_cm: Couldn't retrieve pkey for incoming request (port %u, pkey index %u). %d\n",
>>>>>>> upstream/android-13
				     port_num, pkey_index, ret);
		return 0;
	}

	return pkey;
}

/**
<<<<<<< HEAD
 * Convert OPA SGID to IB SGID
=======
 * cm_opa_to_ib_sgid - Convert OPA SGID to IB SGID
>>>>>>> upstream/android-13
 * ULPs (such as IPoIB) do not understand OPA GIDs and will
 * reject them as the local_gid will not match the sgid. Therefore,
 * change the pathrec's SGID to an IB SGID.
 *
 * @work: Work completion
 * @path: Path record
 */
static void cm_opa_to_ib_sgid(struct cm_work *work,
			      struct sa_path_rec *path)
{
	struct ib_device *dev = work->port->cm_dev->ib_device;
<<<<<<< HEAD
	u8 port_num = work->port->port_num;
=======
	u32 port_num = work->port->port_num;
>>>>>>> upstream/android-13

	if (rdma_cap_opa_ah(dev, port_num) &&
	    (ib_is_opa_gid(&path->sgid))) {
		union ib_gid sgid;

		if (rdma_query_gid(dev, port_num, 0, &sgid)) {
			dev_warn(&dev->dev,
				 "Error updating sgid in CM request\n");
			return;
		}

		path->sgid = sgid;
	}
}

static void cm_format_req_event(struct cm_work *work,
				struct cm_id_private *cm_id_priv,
				struct ib_cm_id *listen_id)
{
	struct cm_req_msg *req_msg;
	struct ib_cm_req_event_param *param;

	req_msg = (struct cm_req_msg *)work->mad_recv_wc->recv_buf.mad;
	param = &work->cm_event.param.req_rcvd;
	param->listen_id = listen_id;
	param->bth_pkey = cm_get_bth_pkey(work);
	param->port = cm_id_priv->av.port->port_num;
	param->primary_path = &work->path[0];
	cm_opa_to_ib_sgid(work, param->primary_path);
	if (cm_req_has_alt_path(req_msg)) {
		param->alternate_path = &work->path[1];
		cm_opa_to_ib_sgid(work, param->alternate_path);
	} else {
		param->alternate_path = NULL;
	}
<<<<<<< HEAD
	param->remote_ca_guid = req_msg->local_ca_guid;
	param->remote_qkey = be32_to_cpu(req_msg->local_qkey);
	param->remote_qpn = be32_to_cpu(cm_req_get_local_qpn(req_msg));
	param->qp_type = cm_req_get_qp_type(req_msg);
	param->starting_psn = be32_to_cpu(cm_req_get_starting_psn(req_msg));
	param->responder_resources = cm_req_get_init_depth(req_msg);
	param->initiator_depth = cm_req_get_resp_res(req_msg);
	param->local_cm_response_timeout =
					cm_req_get_remote_resp_timeout(req_msg);
	param->flow_control = cm_req_get_flow_ctrl(req_msg);
	param->remote_cm_response_timeout =
					cm_req_get_local_resp_timeout(req_msg);
	param->retry_count = cm_req_get_retry_count(req_msg);
	param->rnr_retry_count = cm_req_get_rnr_retry_count(req_msg);
	param->srq = cm_req_get_srq(req_msg);
	param->ppath_sgid_attr = cm_id_priv->av.ah_attr.grh.sgid_attr;
	work->cm_event.private_data = &req_msg->private_data;
=======
	param->remote_ca_guid =
		cpu_to_be64(IBA_GET(CM_REQ_LOCAL_CA_GUID, req_msg));
	param->remote_qkey = IBA_GET(CM_REQ_LOCAL_Q_KEY, req_msg);
	param->remote_qpn = IBA_GET(CM_REQ_LOCAL_QPN, req_msg);
	param->qp_type = cm_req_get_qp_type(req_msg);
	param->starting_psn = IBA_GET(CM_REQ_STARTING_PSN, req_msg);
	param->responder_resources = IBA_GET(CM_REQ_INITIATOR_DEPTH, req_msg);
	param->initiator_depth = IBA_GET(CM_REQ_RESPONDER_RESOURCES, req_msg);
	param->local_cm_response_timeout =
		IBA_GET(CM_REQ_REMOTE_CM_RESPONSE_TIMEOUT, req_msg);
	param->flow_control = IBA_GET(CM_REQ_END_TO_END_FLOW_CONTROL, req_msg);
	param->remote_cm_response_timeout =
		IBA_GET(CM_REQ_LOCAL_CM_RESPONSE_TIMEOUT, req_msg);
	param->retry_count = IBA_GET(CM_REQ_RETRY_COUNT, req_msg);
	param->rnr_retry_count = IBA_GET(CM_REQ_RNR_RETRY_COUNT, req_msg);
	param->srq = IBA_GET(CM_REQ_SRQ, req_msg);
	param->ppath_sgid_attr = cm_id_priv->av.ah_attr.grh.sgid_attr;
	param->ece.vendor_id = IBA_GET(CM_REQ_VENDOR_ID, req_msg);
	param->ece.attr_mod = be32_to_cpu(req_msg->hdr.attr_mod);

	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_REQ_PRIVATE_DATA, req_msg);
>>>>>>> upstream/android-13
}

static void cm_process_work(struct cm_id_private *cm_id_priv,
			    struct cm_work *work)
{
	int ret;

	/* We will typically only have the current event to report. */
	ret = cm_id_priv->id.cm_handler(&cm_id_priv->id, &work->cm_event);
	cm_free_work(work);

	while (!ret && !atomic_add_negative(-1, &cm_id_priv->work_count)) {
		spin_lock_irq(&cm_id_priv->lock);
		work = cm_dequeue_work(cm_id_priv);
		spin_unlock_irq(&cm_id_priv->lock);
		if (!work)
			return;

		ret = cm_id_priv->id.cm_handler(&cm_id_priv->id,
						&work->cm_event);
		cm_free_work(work);
	}
	cm_deref_id(cm_id_priv);
	if (ret)
		cm_destroy_id(&cm_id_priv->id, ret);
}

static void cm_format_mra(struct cm_mra_msg *mra_msg,
			  struct cm_id_private *cm_id_priv,
			  enum cm_msg_response msg_mraed, u8 service_timeout,
			  const void *private_data, u8 private_data_len)
{
	cm_format_mad_hdr(&mra_msg->hdr, CM_MRA_ATTR_ID, cm_id_priv->tid);
<<<<<<< HEAD
	cm_mra_set_msg_mraed(mra_msg, msg_mraed);
	mra_msg->local_comm_id = cm_id_priv->id.local_id;
	mra_msg->remote_comm_id = cm_id_priv->id.remote_id;
	cm_mra_set_service_timeout(mra_msg, service_timeout);

	if (private_data && private_data_len)
		memcpy(mra_msg->private_data, private_data, private_data_len);
=======
	IBA_SET(CM_MRA_MESSAGE_MRAED, mra_msg, msg_mraed);
	IBA_SET(CM_MRA_LOCAL_COMM_ID, mra_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_MRA_REMOTE_COMM_ID, mra_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));
	IBA_SET(CM_MRA_SERVICE_TIMEOUT, mra_msg, service_timeout);

	if (private_data && private_data_len)
		IBA_SET_MEM(CM_MRA_PRIVATE_DATA, mra_msg, private_data,
			    private_data_len);
>>>>>>> upstream/android-13
}

static void cm_format_rej(struct cm_rej_msg *rej_msg,
			  struct cm_id_private *cm_id_priv,
<<<<<<< HEAD
			  enum ib_cm_rej_reason reason,
			  void *ari,
			  u8 ari_length,
			  const void *private_data,
			  u8 private_data_len)
{
	cm_format_mad_hdr(&rej_msg->hdr, CM_REJ_ATTR_ID, cm_id_priv->tid);
	rej_msg->remote_comm_id = cm_id_priv->id.remote_id;

	switch(cm_id_priv->id.state) {
	case IB_CM_REQ_RCVD:
		rej_msg->local_comm_id = 0;
		cm_rej_set_msg_rejected(rej_msg, CM_MSG_RESPONSE_REQ);
		break;
	case IB_CM_MRA_REQ_SENT:
		rej_msg->local_comm_id = cm_id_priv->id.local_id;
		cm_rej_set_msg_rejected(rej_msg, CM_MSG_RESPONSE_REQ);
		break;
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
		rej_msg->local_comm_id = cm_id_priv->id.local_id;
		cm_rej_set_msg_rejected(rej_msg, CM_MSG_RESPONSE_REP);
		break;
	default:
		rej_msg->local_comm_id = cm_id_priv->id.local_id;
		cm_rej_set_msg_rejected(rej_msg, CM_MSG_RESPONSE_OTHER);
		break;
	}

	rej_msg->reason = cpu_to_be16(reason);
	if (ari && ari_length) {
		cm_rej_set_reject_info_len(rej_msg, ari_length);
		memcpy(rej_msg->ari, ari, ari_length);
	}

	if (private_data && private_data_len)
		memcpy(rej_msg->private_data, private_data, private_data_len);
=======
			  enum ib_cm_rej_reason reason, void *ari,
			  u8 ari_length, const void *private_data,
			  u8 private_data_len, enum ib_cm_state state)
{
	lockdep_assert_held(&cm_id_priv->lock);

	cm_format_mad_hdr(&rej_msg->hdr, CM_REJ_ATTR_ID, cm_id_priv->tid);
	IBA_SET(CM_REJ_REMOTE_COMM_ID, rej_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));

	switch (state) {
	case IB_CM_REQ_RCVD:
		IBA_SET(CM_REJ_LOCAL_COMM_ID, rej_msg, be32_to_cpu(0));
		IBA_SET(CM_REJ_MESSAGE_REJECTED, rej_msg, CM_MSG_RESPONSE_REQ);
		break;
	case IB_CM_MRA_REQ_SENT:
		IBA_SET(CM_REJ_LOCAL_COMM_ID, rej_msg,
			be32_to_cpu(cm_id_priv->id.local_id));
		IBA_SET(CM_REJ_MESSAGE_REJECTED, rej_msg, CM_MSG_RESPONSE_REQ);
		break;
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
		IBA_SET(CM_REJ_LOCAL_COMM_ID, rej_msg,
			be32_to_cpu(cm_id_priv->id.local_id));
		IBA_SET(CM_REJ_MESSAGE_REJECTED, rej_msg, CM_MSG_RESPONSE_REP);
		break;
	default:
		IBA_SET(CM_REJ_LOCAL_COMM_ID, rej_msg,
			be32_to_cpu(cm_id_priv->id.local_id));
		IBA_SET(CM_REJ_MESSAGE_REJECTED, rej_msg,
			CM_MSG_RESPONSE_OTHER);
		break;
	}

	IBA_SET(CM_REJ_REASON, rej_msg, reason);
	if (ari && ari_length) {
		IBA_SET(CM_REJ_REJECTED_INFO_LENGTH, rej_msg, ari_length);
		IBA_SET_MEM(CM_REJ_ARI, rej_msg, ari, ari_length);
	}

	if (private_data && private_data_len)
		IBA_SET_MEM(CM_REJ_PRIVATE_DATA, rej_msg, private_data,
			    private_data_len);
>>>>>>> upstream/android-13
}

static void cm_dup_req_handler(struct cm_work *work,
			       struct cm_id_private *cm_id_priv)
{
	struct ib_mad_send_buf *msg = NULL;
	int ret;

<<<<<<< HEAD
	atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
			counter[CM_REQ_COUNTER]);

	/* Quick state check to discard duplicate REQs. */
	if (cm_id_priv->id.state == IB_CM_REQ_RCVD)
		return;
=======
	atomic_long_inc(
		&work->port->counters[CM_RECV_DUPLICATES][CM_REQ_COUNTER]);

	/* Quick state check to discard duplicate REQs. */
	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state == IB_CM_REQ_RCVD) {
		spin_unlock_irq(&cm_id_priv->lock);
		return;
	}
	spin_unlock_irq(&cm_id_priv->lock);
>>>>>>> upstream/android-13

	ret = cm_alloc_response_msg(work->port, work->mad_recv_wc, &msg);
	if (ret)
		return;

	spin_lock_irq(&cm_id_priv->lock);
	switch (cm_id_priv->id.state) {
	case IB_CM_MRA_REQ_SENT:
		cm_format_mra((struct cm_mra_msg *) msg->mad, cm_id_priv,
			      CM_MSG_RESPONSE_REQ, cm_id_priv->service_timeout,
			      cm_id_priv->private_data,
			      cm_id_priv->private_data_len);
		break;
	case IB_CM_TIMEWAIT:
<<<<<<< HEAD
		cm_format_rej((struct cm_rej_msg *) msg->mad, cm_id_priv,
			      IB_CM_REJ_STALE_CONN, NULL, 0, NULL, 0);
=======
		cm_format_rej((struct cm_rej_msg *)msg->mad, cm_id_priv,
			      IB_CM_REJ_STALE_CONN, NULL, 0, NULL, 0,
			      IB_CM_TIMEWAIT);
>>>>>>> upstream/android-13
		break;
	default:
		goto unlock;
	}
	spin_unlock_irq(&cm_id_priv->lock);

<<<<<<< HEAD
=======
	trace_icm_send_dup_req(&cm_id_priv->id);
>>>>>>> upstream/android-13
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		goto free;
	return;

unlock:	spin_unlock_irq(&cm_id_priv->lock);
<<<<<<< HEAD
free:	cm_free_msg(msg);
}

static struct cm_id_private * cm_match_req(struct cm_work *work,
					   struct cm_id_private *cm_id_priv)
=======
free:	cm_free_response_msg(msg);
}

static struct cm_id_private *cm_match_req(struct cm_work *work,
					  struct cm_id_private *cm_id_priv)
>>>>>>> upstream/android-13
{
	struct cm_id_private *listen_cm_id_priv, *cur_cm_id_priv;
	struct cm_timewait_info *timewait_info;
	struct cm_req_msg *req_msg;
<<<<<<< HEAD
	struct ib_cm_id *cm_id;
=======
>>>>>>> upstream/android-13

	req_msg = (struct cm_req_msg *)work->mad_recv_wc->recv_buf.mad;

	/* Check for possible duplicate REQ. */
	spin_lock_irq(&cm.lock);
	timewait_info = cm_insert_remote_id(cm_id_priv->timewait_info);
	if (timewait_info) {
<<<<<<< HEAD
		cur_cm_id_priv = cm_get_id(timewait_info->work.local_id,
=======
		cur_cm_id_priv = cm_acquire_id(timewait_info->work.local_id,
>>>>>>> upstream/android-13
					   timewait_info->work.remote_id);
		spin_unlock_irq(&cm.lock);
		if (cur_cm_id_priv) {
			cm_dup_req_handler(work, cur_cm_id_priv);
			cm_deref_id(cur_cm_id_priv);
		}
		return NULL;
	}

	/* Check for stale connections. */
	timewait_info = cm_insert_remote_qpn(cm_id_priv->timewait_info);
	if (timewait_info) {
<<<<<<< HEAD
		cm_cleanup_timewait(cm_id_priv->timewait_info);
		cur_cm_id_priv = cm_get_id(timewait_info->work.local_id,
=======
		cm_remove_remote(cm_id_priv);
		cur_cm_id_priv = cm_acquire_id(timewait_info->work.local_id,
>>>>>>> upstream/android-13
					   timewait_info->work.remote_id);

		spin_unlock_irq(&cm.lock);
		cm_issue_rej(work->port, work->mad_recv_wc,
			     IB_CM_REJ_STALE_CONN, CM_MSG_RESPONSE_REQ,
			     NULL, 0);
		if (cur_cm_id_priv) {
<<<<<<< HEAD
			cm_id = &cur_cm_id_priv->id;
			ib_send_cm_dreq(cm_id, NULL, 0);
=======
			ib_send_cm_dreq(&cur_cm_id_priv->id, NULL, 0);
>>>>>>> upstream/android-13
			cm_deref_id(cur_cm_id_priv);
		}
		return NULL;
	}

	/* Find matching listen request. */
<<<<<<< HEAD
	listen_cm_id_priv = cm_find_listen(cm_id_priv->id.device,
					   req_msg->service_id);
	if (!listen_cm_id_priv) {
		cm_cleanup_timewait(cm_id_priv->timewait_info);
=======
	listen_cm_id_priv = cm_find_listen(
		cm_id_priv->id.device,
		cpu_to_be64(IBA_GET(CM_REQ_SERVICE_ID, req_msg)));
	if (!listen_cm_id_priv) {
		cm_remove_remote(cm_id_priv);
>>>>>>> upstream/android-13
		spin_unlock_irq(&cm.lock);
		cm_issue_rej(work->port, work->mad_recv_wc,
			     IB_CM_REJ_INVALID_SERVICE_ID, CM_MSG_RESPONSE_REQ,
			     NULL, 0);
<<<<<<< HEAD
		goto out;
	}
	atomic_inc(&listen_cm_id_priv->refcount);
	atomic_inc(&cm_id_priv->refcount);
	cm_id_priv->id.state = IB_CM_REQ_RCVD;
	atomic_inc(&cm_id_priv->work_count);
	spin_unlock_irq(&cm.lock);
out:
=======
		return NULL;
	}
	spin_unlock_irq(&cm.lock);
>>>>>>> upstream/android-13
	return listen_cm_id_priv;
}

/*
 * Work-around for inter-subnet connections.  If the LIDs are permissive,
 * we need to override the LID/SL data in the REQ with the LID information
 * in the work completion.
 */
static void cm_process_routed_req(struct cm_req_msg *req_msg, struct ib_wc *wc)
{
<<<<<<< HEAD
	if (!cm_req_get_primary_subnet_local(req_msg)) {
		if (req_msg->primary_local_lid == IB_LID_PERMISSIVE) {
			req_msg->primary_local_lid = ib_lid_be16(wc->slid);
			cm_req_set_primary_sl(req_msg, wc->sl);
		}

		if (req_msg->primary_remote_lid == IB_LID_PERMISSIVE)
			req_msg->primary_remote_lid = cpu_to_be16(wc->dlid_path_bits);
	}

	if (!cm_req_get_alt_subnet_local(req_msg)) {
		if (req_msg->alt_local_lid == IB_LID_PERMISSIVE) {
			req_msg->alt_local_lid = ib_lid_be16(wc->slid);
			cm_req_set_alt_sl(req_msg, wc->sl);
		}

		if (req_msg->alt_remote_lid == IB_LID_PERMISSIVE)
			req_msg->alt_remote_lid = cpu_to_be16(wc->dlid_path_bits);
=======
	if (!IBA_GET(CM_REQ_PRIMARY_SUBNET_LOCAL, req_msg)) {
		if (cpu_to_be16(IBA_GET(CM_REQ_PRIMARY_LOCAL_PORT_LID,
					req_msg)) == IB_LID_PERMISSIVE) {
			IBA_SET(CM_REQ_PRIMARY_LOCAL_PORT_LID, req_msg,
				be16_to_cpu(ib_lid_be16(wc->slid)));
			IBA_SET(CM_REQ_PRIMARY_SL, req_msg, wc->sl);
		}

		if (cpu_to_be16(IBA_GET(CM_REQ_PRIMARY_REMOTE_PORT_LID,
					req_msg)) == IB_LID_PERMISSIVE)
			IBA_SET(CM_REQ_PRIMARY_REMOTE_PORT_LID, req_msg,
				wc->dlid_path_bits);
	}

	if (!IBA_GET(CM_REQ_ALTERNATE_SUBNET_LOCAL, req_msg)) {
		if (cpu_to_be16(IBA_GET(CM_REQ_ALTERNATE_LOCAL_PORT_LID,
					req_msg)) == IB_LID_PERMISSIVE) {
			IBA_SET(CM_REQ_ALTERNATE_LOCAL_PORT_LID, req_msg,
				be16_to_cpu(ib_lid_be16(wc->slid)));
			IBA_SET(CM_REQ_ALTERNATE_SL, req_msg, wc->sl);
		}

		if (cpu_to_be16(IBA_GET(CM_REQ_ALTERNATE_REMOTE_PORT_LID,
					req_msg)) == IB_LID_PERMISSIVE)
			IBA_SET(CM_REQ_ALTERNATE_REMOTE_PORT_LID, req_msg,
				wc->dlid_path_bits);
>>>>>>> upstream/android-13
	}
}

static int cm_req_handler(struct cm_work *work)
{
<<<<<<< HEAD
	struct ib_cm_id *cm_id;
=======
>>>>>>> upstream/android-13
	struct cm_id_private *cm_id_priv, *listen_cm_id_priv;
	struct cm_req_msg *req_msg;
	const struct ib_global_route *grh;
	const struct ib_gid_attr *gid_attr;
	int ret;

	req_msg = (struct cm_req_msg *)work->mad_recv_wc->recv_buf.mad;

<<<<<<< HEAD
	cm_id = ib_create_cm_id(work->port->cm_dev->ib_device, NULL, NULL);
	if (IS_ERR(cm_id))
		return PTR_ERR(cm_id);

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	cm_id_priv->id.remote_id = req_msg->local_comm_id;
=======
	cm_id_priv =
		cm_alloc_id_priv(work->port->cm_dev->ib_device, NULL, NULL);
	if (IS_ERR(cm_id_priv))
		return PTR_ERR(cm_id_priv);

	cm_id_priv->id.remote_id =
		cpu_to_be32(IBA_GET(CM_REQ_LOCAL_COMM_ID, req_msg));
	cm_id_priv->id.service_id =
		cpu_to_be64(IBA_GET(CM_REQ_SERVICE_ID, req_msg));
	cm_id_priv->id.service_mask = ~cpu_to_be64(0);
	cm_id_priv->tid = req_msg->hdr.tid;
	cm_id_priv->timeout_ms = cm_convert_to_ms(
		IBA_GET(CM_REQ_LOCAL_CM_RESPONSE_TIMEOUT, req_msg));
	cm_id_priv->max_cm_retries = IBA_GET(CM_REQ_MAX_CM_RETRIES, req_msg);
	cm_id_priv->remote_qpn =
		cpu_to_be32(IBA_GET(CM_REQ_LOCAL_QPN, req_msg));
	cm_id_priv->initiator_depth =
		IBA_GET(CM_REQ_RESPONDER_RESOURCES, req_msg);
	cm_id_priv->responder_resources =
		IBA_GET(CM_REQ_INITIATOR_DEPTH, req_msg);
	cm_id_priv->path_mtu = IBA_GET(CM_REQ_PATH_PACKET_PAYLOAD_MTU, req_msg);
	cm_id_priv->pkey = cpu_to_be16(IBA_GET(CM_REQ_PARTITION_KEY, req_msg));
	cm_id_priv->sq_psn = cpu_to_be32(IBA_GET(CM_REQ_STARTING_PSN, req_msg));
	cm_id_priv->retry_count = IBA_GET(CM_REQ_RETRY_COUNT, req_msg);
	cm_id_priv->rnr_retry_count = IBA_GET(CM_REQ_RNR_RETRY_COUNT, req_msg);
	cm_id_priv->qp_type = cm_req_get_qp_type(req_msg);

>>>>>>> upstream/android-13
	ret = cm_init_av_for_response(work->port, work->mad_recv_wc->wc,
				      work->mad_recv_wc->recv_buf.grh,
				      &cm_id_priv->av);
	if (ret)
		goto destroy;
	cm_id_priv->timewait_info = cm_create_timewait_info(cm_id_priv->
							    id.local_id);
	if (IS_ERR(cm_id_priv->timewait_info)) {
		ret = PTR_ERR(cm_id_priv->timewait_info);
		cm_id_priv->timewait_info = NULL;
		goto destroy;
	}
<<<<<<< HEAD
	cm_id_priv->timewait_info->work.remote_id = req_msg->local_comm_id;
	cm_id_priv->timewait_info->remote_ca_guid = req_msg->local_ca_guid;
	cm_id_priv->timewait_info->remote_qpn = cm_req_get_local_qpn(req_msg);

	listen_cm_id_priv = cm_match_req(work, cm_id_priv);
	if (!listen_cm_id_priv) {
		pr_debug("%s: local_id %d, no listen_cm_id_priv\n", __func__,
			 be32_to_cpu(cm_id->local_id));
=======
	cm_id_priv->timewait_info->work.remote_id = cm_id_priv->id.remote_id;
	cm_id_priv->timewait_info->remote_ca_guid =
		cpu_to_be64(IBA_GET(CM_REQ_LOCAL_CA_GUID, req_msg));
	cm_id_priv->timewait_info->remote_qpn = cm_id_priv->remote_qpn;

	/*
	 * Note that the ID pointer is not in the xarray at this point,
	 * so this set is only visible to the local thread.
	 */
	cm_id_priv->id.state = IB_CM_REQ_RCVD;

	listen_cm_id_priv = cm_match_req(work, cm_id_priv);
	if (!listen_cm_id_priv) {
		trace_icm_no_listener_err(&cm_id_priv->id);
		cm_id_priv->id.state = IB_CM_IDLE;
>>>>>>> upstream/android-13
		ret = -EINVAL;
		goto destroy;
	}

<<<<<<< HEAD
	cm_id_priv->id.cm_handler = listen_cm_id_priv->id.cm_handler;
	cm_id_priv->id.context = listen_cm_id_priv->id.context;
	cm_id_priv->id.service_id = req_msg->service_id;
	cm_id_priv->id.service_mask = ~cpu_to_be64(0);

	cm_process_routed_req(req_msg, work->mad_recv_wc->wc);

=======
>>>>>>> upstream/android-13
	memset(&work->path[0], 0, sizeof(work->path[0]));
	if (cm_req_has_alt_path(req_msg))
		memset(&work->path[1], 0, sizeof(work->path[1]));
	grh = rdma_ah_read_grh(&cm_id_priv->av.ah_attr);
	gid_attr = grh->sgid_attr;

<<<<<<< HEAD
	if (gid_attr && gid_attr->ndev) {
		work->path[0].rec_type =
			sa_conv_gid_to_pathrec_type(gid_attr->gid_type);
	} else {
		/* If no GID attribute or ndev is null, it is not RoCE. */
		cm_path_set_rec_type(work->port->cm_dev->ib_device,
				     work->port->port_num,
				     &work->path[0],
				     &req_msg->primary_local_gid);
=======
	if (cm_id_priv->av.ah_attr.type == RDMA_AH_ATTR_TYPE_ROCE) {
		work->path[0].rec_type =
			sa_conv_gid_to_pathrec_type(gid_attr->gid_type);
	} else {
		cm_process_routed_req(req_msg, work->mad_recv_wc->wc);
		cm_path_set_rec_type(
			work->port->cm_dev->ib_device, work->port->port_num,
			&work->path[0],
			IBA_GET_MEM_PTR(CM_REQ_PRIMARY_LOCAL_PORT_GID,
					req_msg));
>>>>>>> upstream/android-13
	}
	if (cm_req_has_alt_path(req_msg))
		work->path[1].rec_type = work->path[0].rec_type;
	cm_format_paths_from_req(req_msg, &work->path[0],
				 &work->path[1]);
	if (cm_id_priv->av.ah_attr.type == RDMA_AH_ATTR_TYPE_ROCE)
		sa_path_set_dmac(&work->path[0],
				 cm_id_priv->av.ah_attr.roce.dmac);
	work->path[0].hop_limit = grh->hop_limit;
<<<<<<< HEAD
	ret = cm_init_av_by_path(&work->path[0], gid_attr, &cm_id_priv->av,
				 cm_id_priv);
=======

	/* This destroy call is needed to pair with cm_init_av_for_response */
	cm_destroy_av(&cm_id_priv->av);
	ret = cm_init_av_by_path(&work->path[0], gid_attr, &cm_id_priv->av);
>>>>>>> upstream/android-13
	if (ret) {
		int err;

		err = rdma_query_gid(work->port->cm_dev->ib_device,
				     work->port->port_num, 0,
				     &work->path[0].sgid);
		if (err)
<<<<<<< HEAD
			ib_send_cm_rej(cm_id, IB_CM_REJ_INVALID_GID,
				       NULL, 0, NULL, 0);
		else
			ib_send_cm_rej(cm_id, IB_CM_REJ_INVALID_GID,
=======
			ib_send_cm_rej(&cm_id_priv->id, IB_CM_REJ_INVALID_GID,
				       NULL, 0, NULL, 0);
		else
			ib_send_cm_rej(&cm_id_priv->id, IB_CM_REJ_INVALID_GID,
>>>>>>> upstream/android-13
				       &work->path[0].sgid,
				       sizeof(work->path[0].sgid),
				       NULL, 0);
		goto rejected;
	}
	if (cm_req_has_alt_path(req_msg)) {
		ret = cm_init_av_by_path(&work->path[1], NULL,
<<<<<<< HEAD
					 &cm_id_priv->alt_av, cm_id_priv);
		if (ret) {
			ib_send_cm_rej(cm_id, IB_CM_REJ_INVALID_ALT_GID,
=======
					 &cm_id_priv->alt_av);
		if (ret) {
			ib_send_cm_rej(&cm_id_priv->id,
				       IB_CM_REJ_INVALID_ALT_GID,
>>>>>>> upstream/android-13
				       &work->path[0].sgid,
				       sizeof(work->path[0].sgid), NULL, 0);
			goto rejected;
		}
	}
<<<<<<< HEAD
	cm_id_priv->tid = req_msg->hdr.tid;
	cm_id_priv->timeout_ms = cm_convert_to_ms(
					cm_req_get_local_resp_timeout(req_msg));
	cm_id_priv->max_cm_retries = cm_req_get_max_cm_retries(req_msg);
	cm_id_priv->remote_qpn = cm_req_get_local_qpn(req_msg);
	cm_id_priv->initiator_depth = cm_req_get_resp_res(req_msg);
	cm_id_priv->responder_resources = cm_req_get_init_depth(req_msg);
	cm_id_priv->path_mtu = cm_req_get_path_mtu(req_msg);
	cm_id_priv->pkey = req_msg->pkey;
	cm_id_priv->sq_psn = cm_req_get_starting_psn(req_msg);
	cm_id_priv->retry_count = cm_req_get_retry_count(req_msg);
	cm_id_priv->rnr_retry_count = cm_req_get_rnr_retry_count(req_msg);
	cm_id_priv->qp_type = cm_req_get_qp_type(req_msg);

	cm_format_req_event(work, cm_id_priv, &listen_cm_id_priv->id);
	cm_process_work(cm_id_priv, work);
=======

	cm_id_priv->id.cm_handler = listen_cm_id_priv->id.cm_handler;
	cm_id_priv->id.context = listen_cm_id_priv->id.context;
	cm_format_req_event(work, cm_id_priv, &listen_cm_id_priv->id);

	/* Now MAD handlers can see the new ID */
	spin_lock_irq(&cm_id_priv->lock);
	cm_finalize_id(cm_id_priv);

	/* Refcount belongs to the event, pairs with cm_process_work() */
	refcount_inc(&cm_id_priv->refcount);
	cm_queue_work_unlock(cm_id_priv, work);
	/*
	 * Since this ID was just created and was not made visible to other MAD
	 * handlers until the cm_finalize_id() above we know that the
	 * cm_process_work() will deliver the event and the listen_cm_id
	 * embedded in the event can be derefed here.
	 */
>>>>>>> upstream/android-13
	cm_deref_id(listen_cm_id_priv);
	return 0;

rejected:
<<<<<<< HEAD
	atomic_dec(&cm_id_priv->refcount);
	cm_deref_id(listen_cm_id_priv);
destroy:
	ib_destroy_cm_id(cm_id);
=======
	cm_deref_id(listen_cm_id_priv);
destroy:
	ib_destroy_cm_id(&cm_id_priv->id);
>>>>>>> upstream/android-13
	return ret;
}

static void cm_format_rep(struct cm_rep_msg *rep_msg,
			  struct cm_id_private *cm_id_priv,
			  struct ib_cm_rep_param *param)
{
<<<<<<< HEAD
	cm_format_mad_hdr(&rep_msg->hdr, CM_REP_ATTR_ID, cm_id_priv->tid);
	rep_msg->local_comm_id = cm_id_priv->id.local_id;
	rep_msg->remote_comm_id = cm_id_priv->id.remote_id;
	cm_rep_set_starting_psn(rep_msg, cpu_to_be32(param->starting_psn));
	rep_msg->resp_resources = param->responder_resources;
	cm_rep_set_target_ack_delay(rep_msg,
				    cm_id_priv->av.port->cm_dev->ack_delay);
	cm_rep_set_failover(rep_msg, param->failover_accepted);
	cm_rep_set_rnr_retry_count(rep_msg, param->rnr_retry_count);
	rep_msg->local_ca_guid = cm_id_priv->id.device->node_guid;

	if (cm_id_priv->qp_type != IB_QPT_XRC_TGT) {
		rep_msg->initiator_depth = param->initiator_depth;
		cm_rep_set_flow_ctrl(rep_msg, param->flow_control);
		cm_rep_set_srq(rep_msg, param->srq);
		cm_rep_set_local_qpn(rep_msg, cpu_to_be32(param->qp_num));
	} else {
		cm_rep_set_srq(rep_msg, 1);
		cm_rep_set_local_eecn(rep_msg, cpu_to_be32(param->qp_num));
	}

	if (param->private_data && param->private_data_len)
		memcpy(rep_msg->private_data, param->private_data,
		       param->private_data_len);
=======
	cm_format_mad_ece_hdr(&rep_msg->hdr, CM_REP_ATTR_ID, cm_id_priv->tid,
			      param->ece.attr_mod);
	IBA_SET(CM_REP_LOCAL_COMM_ID, rep_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_REP_REMOTE_COMM_ID, rep_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));
	IBA_SET(CM_REP_STARTING_PSN, rep_msg, param->starting_psn);
	IBA_SET(CM_REP_RESPONDER_RESOURCES, rep_msg,
		param->responder_resources);
	IBA_SET(CM_REP_TARGET_ACK_DELAY, rep_msg,
		cm_id_priv->av.port->cm_dev->ack_delay);
	IBA_SET(CM_REP_FAILOVER_ACCEPTED, rep_msg, param->failover_accepted);
	IBA_SET(CM_REP_RNR_RETRY_COUNT, rep_msg, param->rnr_retry_count);
	IBA_SET(CM_REP_LOCAL_CA_GUID, rep_msg,
		be64_to_cpu(cm_id_priv->id.device->node_guid));

	if (cm_id_priv->qp_type != IB_QPT_XRC_TGT) {
		IBA_SET(CM_REP_INITIATOR_DEPTH, rep_msg,
			param->initiator_depth);
		IBA_SET(CM_REP_END_TO_END_FLOW_CONTROL, rep_msg,
			param->flow_control);
		IBA_SET(CM_REP_SRQ, rep_msg, param->srq);
		IBA_SET(CM_REP_LOCAL_QPN, rep_msg, param->qp_num);
	} else {
		IBA_SET(CM_REP_SRQ, rep_msg, 1);
		IBA_SET(CM_REP_LOCAL_EE_CONTEXT_NUMBER, rep_msg, param->qp_num);
	}

	IBA_SET(CM_REP_VENDOR_ID_L, rep_msg, param->ece.vendor_id);
	IBA_SET(CM_REP_VENDOR_ID_M, rep_msg, param->ece.vendor_id >> 8);
	IBA_SET(CM_REP_VENDOR_ID_H, rep_msg, param->ece.vendor_id >> 16);

	if (param->private_data && param->private_data_len)
		IBA_SET_MEM(CM_REP_PRIVATE_DATA, rep_msg, param->private_data,
			    param->private_data_len);
>>>>>>> upstream/android-13
}

int ib_send_cm_rep(struct ib_cm_id *cm_id,
		   struct ib_cm_rep_param *param)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	struct cm_rep_msg *rep_msg;
	unsigned long flags;
	int ret;

	if (param->private_data &&
	    param->private_data_len > IB_CM_REP_PRIVATE_DATA_SIZE)
		return -EINVAL;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_REQ_RCVD &&
	    cm_id->state != IB_CM_MRA_REQ_SENT) {
<<<<<<< HEAD
		pr_debug("%s: local_comm_id %d, cm_id->state: %d\n", __func__,
			 be32_to_cpu(cm_id_priv->id.local_id), cm_id->state);
=======
		trace_icm_send_rep_err(cm_id_priv->id.local_id, cm_id->state);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto out;
=======
	msg = cm_alloc_priv_msg(cm_id_priv);
	if (IS_ERR(msg)) {
		ret = PTR_ERR(msg);
		goto out;
	}
>>>>>>> upstream/android-13

	rep_msg = (struct cm_rep_msg *) msg->mad;
	cm_format_rep(rep_msg, cm_id_priv, param);
	msg->timeout_ms = cm_id_priv->timeout_ms;
	msg->context[1] = (void *) (unsigned long) IB_CM_REP_SENT;

<<<<<<< HEAD
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		return ret;
	}

	cm_id->state = IB_CM_REP_SENT;
	cm_id_priv->msg = msg;
	cm_id_priv->initiator_depth = param->initiator_depth;
	cm_id_priv->responder_resources = param->responder_resources;
	cm_id_priv->rq_psn = cm_rep_get_starting_psn(rep_msg);
	cm_id_priv->local_qpn = cpu_to_be32(param->qp_num & 0xFFFFFF);

out:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
=======
	trace_icm_send_rep(cm_id);
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		goto out_free;

	cm_id->state = IB_CM_REP_SENT;
	cm_id_priv->initiator_depth = param->initiator_depth;
	cm_id_priv->responder_resources = param->responder_resources;
	cm_id_priv->rq_psn = cpu_to_be32(IBA_GET(CM_REP_STARTING_PSN, rep_msg));
	WARN_ONCE(param->qp_num & 0xFF000000,
		  "IBTA declares QPN to be 24 bits, but it is 0x%X\n",
		  param->qp_num);
	cm_id_priv->local_qpn = cpu_to_be32(param->qp_num & 0xFFFFFF);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return 0;

out_free:
	cm_free_priv_msg(msg);
out:
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_rep);

static void cm_format_rtu(struct cm_rtu_msg *rtu_msg,
			  struct cm_id_private *cm_id_priv,
			  const void *private_data,
			  u8 private_data_len)
{
	cm_format_mad_hdr(&rtu_msg->hdr, CM_RTU_ATTR_ID, cm_id_priv->tid);
<<<<<<< HEAD
	rtu_msg->local_comm_id = cm_id_priv->id.local_id;
	rtu_msg->remote_comm_id = cm_id_priv->id.remote_id;

	if (private_data && private_data_len)
		memcpy(rtu_msg->private_data, private_data, private_data_len);
=======
	IBA_SET(CM_RTU_LOCAL_COMM_ID, rtu_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_RTU_REMOTE_COMM_ID, rtu_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));

	if (private_data && private_data_len)
		IBA_SET_MEM(CM_RTU_PRIVATE_DATA, rtu_msg, private_data,
			    private_data_len);
>>>>>>> upstream/android-13
}

int ib_send_cm_rtu(struct ib_cm_id *cm_id,
		   const void *private_data,
		   u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	void *data;
	int ret;

	if (private_data && private_data_len > IB_CM_RTU_PRIVATE_DATA_SIZE)
		return -EINVAL;

	data = cm_copy_private_data(private_data, private_data_len);
	if (IS_ERR(data))
		return PTR_ERR(data);

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_REP_RCVD &&
	    cm_id->state != IB_CM_MRA_REP_SENT) {
<<<<<<< HEAD
		pr_debug("%s: local_id %d, cm_id->state %d\n", __func__,
			 be32_to_cpu(cm_id->local_id), cm_id->state);
=======
		trace_icm_send_cm_rtu_err(cm_id);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		goto error;
	}

<<<<<<< HEAD
	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto error;
=======
	msg = cm_alloc_msg(cm_id_priv);
	if (IS_ERR(msg)) {
		ret = PTR_ERR(msg);
		goto error;
	}
>>>>>>> upstream/android-13

	cm_format_rtu((struct cm_rtu_msg *) msg->mad, cm_id_priv,
		      private_data, private_data_len);

<<<<<<< HEAD
=======
	trace_icm_send_rtu(cm_id);
>>>>>>> upstream/android-13
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		kfree(data);
		return ret;
	}

	cm_id->state = IB_CM_ESTABLISHED;
	cm_set_private_data(cm_id_priv, data, private_data_len);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return 0;

error:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	kfree(data);
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_rtu);

static void cm_format_rep_event(struct cm_work *work, enum ib_qp_type qp_type)
{
	struct cm_rep_msg *rep_msg;
	struct ib_cm_rep_event_param *param;

	rep_msg = (struct cm_rep_msg *)work->mad_recv_wc->recv_buf.mad;
	param = &work->cm_event.param.rep_rcvd;
<<<<<<< HEAD
	param->remote_ca_guid = rep_msg->local_ca_guid;
	param->remote_qkey = be32_to_cpu(rep_msg->local_qkey);
	param->remote_qpn = be32_to_cpu(cm_rep_get_qpn(rep_msg, qp_type));
	param->starting_psn = be32_to_cpu(cm_rep_get_starting_psn(rep_msg));
	param->responder_resources = rep_msg->initiator_depth;
	param->initiator_depth = rep_msg->resp_resources;
	param->target_ack_delay = cm_rep_get_target_ack_delay(rep_msg);
	param->failover_accepted = cm_rep_get_failover(rep_msg);
	param->flow_control = cm_rep_get_flow_ctrl(rep_msg);
	param->rnr_retry_count = cm_rep_get_rnr_retry_count(rep_msg);
	param->srq = cm_rep_get_srq(rep_msg);
	work->cm_event.private_data = &rep_msg->private_data;
=======
	param->remote_ca_guid =
		cpu_to_be64(IBA_GET(CM_REP_LOCAL_CA_GUID, rep_msg));
	param->remote_qkey = IBA_GET(CM_REP_LOCAL_Q_KEY, rep_msg);
	param->remote_qpn = be32_to_cpu(cm_rep_get_qpn(rep_msg, qp_type));
	param->starting_psn = IBA_GET(CM_REP_STARTING_PSN, rep_msg);
	param->responder_resources = IBA_GET(CM_REP_INITIATOR_DEPTH, rep_msg);
	param->initiator_depth = IBA_GET(CM_REP_RESPONDER_RESOURCES, rep_msg);
	param->target_ack_delay = IBA_GET(CM_REP_TARGET_ACK_DELAY, rep_msg);
	param->failover_accepted = IBA_GET(CM_REP_FAILOVER_ACCEPTED, rep_msg);
	param->flow_control = IBA_GET(CM_REP_END_TO_END_FLOW_CONTROL, rep_msg);
	param->rnr_retry_count = IBA_GET(CM_REP_RNR_RETRY_COUNT, rep_msg);
	param->srq = IBA_GET(CM_REP_SRQ, rep_msg);
	param->ece.vendor_id = IBA_GET(CM_REP_VENDOR_ID_H, rep_msg) << 16;
	param->ece.vendor_id |= IBA_GET(CM_REP_VENDOR_ID_M, rep_msg) << 8;
	param->ece.vendor_id |= IBA_GET(CM_REP_VENDOR_ID_L, rep_msg);
	param->ece.attr_mod = be32_to_cpu(rep_msg->hdr.attr_mod);

	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_REP_PRIVATE_DATA, rep_msg);
>>>>>>> upstream/android-13
}

static void cm_dup_rep_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_rep_msg *rep_msg;
	struct ib_mad_send_buf *msg = NULL;
	int ret;

	rep_msg = (struct cm_rep_msg *) work->mad_recv_wc->recv_buf.mad;
<<<<<<< HEAD
	cm_id_priv = cm_acquire_id(rep_msg->remote_comm_id,
				   rep_msg->local_comm_id);
	if (!cm_id_priv)
		return;

	atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
			counter[CM_REP_COUNTER]);
=======
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_REP_REMOTE_COMM_ID, rep_msg)),
		cpu_to_be32(IBA_GET(CM_REP_LOCAL_COMM_ID, rep_msg)));
	if (!cm_id_priv)
		return;

	atomic_long_inc(
		&work->port->counters[CM_RECV_DUPLICATES][CM_REP_COUNTER]);
>>>>>>> upstream/android-13
	ret = cm_alloc_response_msg(work->port, work->mad_recv_wc, &msg);
	if (ret)
		goto deref;

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state == IB_CM_ESTABLISHED)
		cm_format_rtu((struct cm_rtu_msg *) msg->mad, cm_id_priv,
			      cm_id_priv->private_data,
			      cm_id_priv->private_data_len);
	else if (cm_id_priv->id.state == IB_CM_MRA_REP_SENT)
		cm_format_mra((struct cm_mra_msg *) msg->mad, cm_id_priv,
			      CM_MSG_RESPONSE_REP, cm_id_priv->service_timeout,
			      cm_id_priv->private_data,
			      cm_id_priv->private_data_len);
	else
		goto unlock;
	spin_unlock_irq(&cm_id_priv->lock);

<<<<<<< HEAD
=======
	trace_icm_send_dup_rep(&cm_id_priv->id);
>>>>>>> upstream/android-13
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		goto free;
	goto deref;

unlock:	spin_unlock_irq(&cm_id_priv->lock);
<<<<<<< HEAD
free:	cm_free_msg(msg);
=======
free:	cm_free_response_msg(msg);
>>>>>>> upstream/android-13
deref:	cm_deref_id(cm_id_priv);
}

static int cm_rep_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_rep_msg *rep_msg;
	int ret;
	struct cm_id_private *cur_cm_id_priv;
<<<<<<< HEAD
	struct ib_cm_id *cm_id;
	struct cm_timewait_info *timewait_info;

	rep_msg = (struct cm_rep_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(rep_msg->remote_comm_id, 0);
	if (!cm_id_priv) {
		cm_dup_rep_handler(work);
		pr_debug("%s: remote_comm_id %d, no cm_id_priv\n", __func__,
			 be32_to_cpu(rep_msg->remote_comm_id));
=======
	struct cm_timewait_info *timewait_info;

	rep_msg = (struct cm_rep_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_REP_REMOTE_COMM_ID, rep_msg)), 0);
	if (!cm_id_priv) {
		cm_dup_rep_handler(work);
		trace_icm_remote_no_priv_err(
			 IBA_GET(CM_REP_REMOTE_COMM_ID, rep_msg));
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	cm_format_rep_event(work, cm_id_priv->qp_type);

	spin_lock_irq(&cm_id_priv->lock);
	switch (cm_id_priv->id.state) {
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
		break;
	default:
<<<<<<< HEAD
		spin_unlock_irq(&cm_id_priv->lock);
		ret = -EINVAL;
		pr_debug("%s: cm_id_priv->id.state: %d, local_comm_id %d, remote_comm_id %d\n",
			 __func__, cm_id_priv->id.state,
			 be32_to_cpu(rep_msg->local_comm_id),
			 be32_to_cpu(rep_msg->remote_comm_id));
		goto error;
	}

	cm_id_priv->timewait_info->work.remote_id = rep_msg->local_comm_id;
	cm_id_priv->timewait_info->remote_ca_guid = rep_msg->local_ca_guid;
=======
		ret = -EINVAL;
		trace_icm_rep_unknown_err(
			IBA_GET(CM_REP_LOCAL_COMM_ID, rep_msg),
			IBA_GET(CM_REP_REMOTE_COMM_ID, rep_msg),
			cm_id_priv->id.state);
		spin_unlock_irq(&cm_id_priv->lock);
		goto error;
	}

	cm_id_priv->timewait_info->work.remote_id =
		cpu_to_be32(IBA_GET(CM_REP_LOCAL_COMM_ID, rep_msg));
	cm_id_priv->timewait_info->remote_ca_guid =
		cpu_to_be64(IBA_GET(CM_REP_LOCAL_CA_GUID, rep_msg));
>>>>>>> upstream/android-13
	cm_id_priv->timewait_info->remote_qpn = cm_rep_get_qpn(rep_msg, cm_id_priv->qp_type);

	spin_lock(&cm.lock);
	/* Check for duplicate REP. */
	if (cm_insert_remote_id(cm_id_priv->timewait_info)) {
		spin_unlock(&cm.lock);
		spin_unlock_irq(&cm_id_priv->lock);
		ret = -EINVAL;
<<<<<<< HEAD
		pr_debug("%s: Failed to insert remote id %d\n", __func__,
			 be32_to_cpu(rep_msg->remote_comm_id));
=======
		trace_icm_insert_failed_err(
			 IBA_GET(CM_REP_REMOTE_COMM_ID, rep_msg));
>>>>>>> upstream/android-13
		goto error;
	}
	/* Check for a stale connection. */
	timewait_info = cm_insert_remote_qpn(cm_id_priv->timewait_info);
	if (timewait_info) {
<<<<<<< HEAD
		rb_erase(&cm_id_priv->timewait_info->remote_id_node,
			 &cm.remote_id_table);
		cm_id_priv->timewait_info->inserted_remote_id = 0;
		cur_cm_id_priv = cm_get_id(timewait_info->work.local_id,
=======
		cm_remove_remote(cm_id_priv);
		cur_cm_id_priv = cm_acquire_id(timewait_info->work.local_id,
>>>>>>> upstream/android-13
					   timewait_info->work.remote_id);

		spin_unlock(&cm.lock);
		spin_unlock_irq(&cm_id_priv->lock);
		cm_issue_rej(work->port, work->mad_recv_wc,
			     IB_CM_REJ_STALE_CONN, CM_MSG_RESPONSE_REP,
			     NULL, 0);
		ret = -EINVAL;
<<<<<<< HEAD
		pr_debug("%s: Stale connection. local_comm_id %d, remote_comm_id %d\n",
			 __func__, be32_to_cpu(rep_msg->local_comm_id),
			 be32_to_cpu(rep_msg->remote_comm_id));

		if (cur_cm_id_priv) {
			cm_id = &cur_cm_id_priv->id;
			ib_send_cm_dreq(cm_id, NULL, 0);
=======
		trace_icm_staleconn_err(
			IBA_GET(CM_REP_LOCAL_COMM_ID, rep_msg),
			IBA_GET(CM_REP_REMOTE_COMM_ID, rep_msg));

		if (cur_cm_id_priv) {
			ib_send_cm_dreq(&cur_cm_id_priv->id, NULL, 0);
>>>>>>> upstream/android-13
			cm_deref_id(cur_cm_id_priv);
		}

		goto error;
	}
	spin_unlock(&cm.lock);

	cm_id_priv->id.state = IB_CM_REP_RCVD;
<<<<<<< HEAD
	cm_id_priv->id.remote_id = rep_msg->local_comm_id;
	cm_id_priv->remote_qpn = cm_rep_get_qpn(rep_msg, cm_id_priv->qp_type);
	cm_id_priv->initiator_depth = rep_msg->resp_resources;
	cm_id_priv->responder_resources = rep_msg->initiator_depth;
	cm_id_priv->sq_psn = cm_rep_get_starting_psn(rep_msg);
	cm_id_priv->rnr_retry_count = cm_rep_get_rnr_retry_count(rep_msg);
	cm_id_priv->target_ack_delay = cm_rep_get_target_ack_delay(rep_msg);
=======
	cm_id_priv->id.remote_id =
		cpu_to_be32(IBA_GET(CM_REP_LOCAL_COMM_ID, rep_msg));
	cm_id_priv->remote_qpn = cm_rep_get_qpn(rep_msg, cm_id_priv->qp_type);
	cm_id_priv->initiator_depth =
		IBA_GET(CM_REP_RESPONDER_RESOURCES, rep_msg);
	cm_id_priv->responder_resources =
		IBA_GET(CM_REP_INITIATOR_DEPTH, rep_msg);
	cm_id_priv->sq_psn = cpu_to_be32(IBA_GET(CM_REP_STARTING_PSN, rep_msg));
	cm_id_priv->rnr_retry_count = IBA_GET(CM_REP_RNR_RETRY_COUNT, rep_msg);
	cm_id_priv->target_ack_delay =
		IBA_GET(CM_REP_TARGET_ACK_DELAY, rep_msg);
>>>>>>> upstream/android-13
	cm_id_priv->av.timeout =
			cm_ack_timeout(cm_id_priv->target_ack_delay,
				       cm_id_priv->av.timeout - 1);
	cm_id_priv->alt_av.timeout =
			cm_ack_timeout(cm_id_priv->target_ack_delay,
				       cm_id_priv->alt_av.timeout - 1);

<<<<<<< HEAD
	/* todo: handle peer_to_peer */

	ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	ib_cancel_mad(cm_id_priv->msg);
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;

error:
	cm_deref_id(cm_id_priv);
	return ret;
}

static int cm_establish_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> upstream/android-13

	/* See comment in cm_establish about lookup. */
	cm_id_priv = cm_acquire_id(work->local_id, work->remote_id);
	if (!cm_id_priv)
		return -EINVAL;

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state != IB_CM_ESTABLISHED) {
		spin_unlock_irq(&cm_id_priv->lock);
		goto out;
	}

<<<<<<< HEAD
	ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	ib_cancel_mad(cm_id_priv->msg);
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

static int cm_rtu_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_rtu_msg *rtu_msg;
<<<<<<< HEAD
	int ret;

	rtu_msg = (struct cm_rtu_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(rtu_msg->remote_comm_id,
				   rtu_msg->local_comm_id);
	if (!cm_id_priv)
		return -EINVAL;

	work->cm_event.private_data = &rtu_msg->private_data;
=======

	rtu_msg = (struct cm_rtu_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_RTU_REMOTE_COMM_ID, rtu_msg)),
		cpu_to_be32(IBA_GET(CM_RTU_LOCAL_COMM_ID, rtu_msg)));
	if (!cm_id_priv)
		return -EINVAL;

	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_RTU_PRIVATE_DATA, rtu_msg);
>>>>>>> upstream/android-13

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state != IB_CM_REP_SENT &&
	    cm_id_priv->id.state != IB_CM_MRA_REP_RCVD) {
		spin_unlock_irq(&cm_id_priv->lock);
<<<<<<< HEAD
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_RTU_COUNTER]);
=======
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_RTU_COUNTER]);
>>>>>>> upstream/android-13
		goto out;
	}
	cm_id_priv->id.state = IB_CM_ESTABLISHED;

<<<<<<< HEAD
	ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	ib_cancel_mad(cm_id_priv->msg);
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

static void cm_format_dreq(struct cm_dreq_msg *dreq_msg,
			  struct cm_id_private *cm_id_priv,
			  const void *private_data,
			  u8 private_data_len)
{
	cm_format_mad_hdr(&dreq_msg->hdr, CM_DREQ_ATTR_ID,
			  cm_form_tid(cm_id_priv));
<<<<<<< HEAD
	dreq_msg->local_comm_id = cm_id_priv->id.local_id;
	dreq_msg->remote_comm_id = cm_id_priv->id.remote_id;
	cm_dreq_set_remote_qpn(dreq_msg, cm_id_priv->remote_qpn);

	if (private_data && private_data_len)
		memcpy(dreq_msg->private_data, private_data, private_data_len);
}

int ib_send_cm_dreq(struct ib_cm_id *cm_id,
		    const void *private_data,
		    u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	int ret;

	if (private_data && private_data_len > IB_CM_DREQ_PRIVATE_DATA_SIZE)
		return -EINVAL;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_ESTABLISHED) {
		pr_debug("%s: local_id %d, cm_id->state: %d\n", __func__,
			 be32_to_cpu(cm_id->local_id), cm_id->state);
		ret = -EINVAL;
		goto out;
	}

	if (cm_id->lap_state == IB_CM_LAP_SENT ||
	    cm_id->lap_state == IB_CM_MRA_LAP_RCVD)
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);

	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret) {
		cm_enter_timewait(cm_id_priv);
		goto out;
=======
	IBA_SET(CM_DREQ_LOCAL_COMM_ID, dreq_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_DREQ_REMOTE_COMM_ID, dreq_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));
	IBA_SET(CM_DREQ_REMOTE_QPN_EECN, dreq_msg,
		be32_to_cpu(cm_id_priv->remote_qpn));

	if (private_data && private_data_len)
		IBA_SET_MEM(CM_DREQ_PRIVATE_DATA, dreq_msg, private_data,
			    private_data_len);
}

static int cm_send_dreq_locked(struct cm_id_private *cm_id_priv,
			       const void *private_data, u8 private_data_len)
{
	struct ib_mad_send_buf *msg;
	int ret;

	lockdep_assert_held(&cm_id_priv->lock);

	if (private_data && private_data_len > IB_CM_DREQ_PRIVATE_DATA_SIZE)
		return -EINVAL;

	if (cm_id_priv->id.state != IB_CM_ESTABLISHED) {
		trace_icm_dreq_skipped(&cm_id_priv->id);
		return -EINVAL;
	}

	if (cm_id_priv->id.lap_state == IB_CM_LAP_SENT ||
	    cm_id_priv->id.lap_state == IB_CM_MRA_LAP_RCVD)
		ib_cancel_mad(cm_id_priv->msg);

	msg = cm_alloc_priv_msg(cm_id_priv);
	if (IS_ERR(msg)) {
		cm_enter_timewait(cm_id_priv);
		return PTR_ERR(msg);
>>>>>>> upstream/android-13
	}

	cm_format_dreq((struct cm_dreq_msg *) msg->mad, cm_id_priv,
		       private_data, private_data_len);
	msg->timeout_ms = cm_id_priv->timeout_ms;
	msg->context[1] = (void *) (unsigned long) IB_CM_DREQ_SENT;

<<<<<<< HEAD
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		cm_enter_timewait(cm_id_priv);
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		return ret;
	}

	cm_id->state = IB_CM_DREQ_SENT;
	cm_id_priv->msg = msg;
out:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
=======
	trace_icm_send_dreq(&cm_id_priv->id);
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		cm_enter_timewait(cm_id_priv);
		cm_free_priv_msg(msg);
		return ret;
	}

	cm_id_priv->id.state = IB_CM_DREQ_SENT;
	return 0;
}

int ib_send_cm_dreq(struct ib_cm_id *cm_id, const void *private_data,
		    u8 private_data_len)
{
	struct cm_id_private *cm_id_priv =
		container_of(cm_id, struct cm_id_private, id);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	ret = cm_send_dreq_locked(cm_id_priv, private_data, private_data_len);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_dreq);

static void cm_format_drep(struct cm_drep_msg *drep_msg,
			  struct cm_id_private *cm_id_priv,
			  const void *private_data,
			  u8 private_data_len)
{
	cm_format_mad_hdr(&drep_msg->hdr, CM_DREP_ATTR_ID, cm_id_priv->tid);
<<<<<<< HEAD
	drep_msg->local_comm_id = cm_id_priv->id.local_id;
	drep_msg->remote_comm_id = cm_id_priv->id.remote_id;

	if (private_data && private_data_len)
		memcpy(drep_msg->private_data, private_data, private_data_len);
}

int ib_send_cm_drep(struct ib_cm_id *cm_id,
		    const void *private_data,
		    u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	void *data;
	int ret;

	if (private_data && private_data_len > IB_CM_DREP_PRIVATE_DATA_SIZE)
		return -EINVAL;

=======
	IBA_SET(CM_DREP_LOCAL_COMM_ID, drep_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_DREP_REMOTE_COMM_ID, drep_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));

	if (private_data && private_data_len)
		IBA_SET_MEM(CM_DREP_PRIVATE_DATA, drep_msg, private_data,
			    private_data_len);
}

static int cm_send_drep_locked(struct cm_id_private *cm_id_priv,
			       void *private_data, u8 private_data_len)
{
	struct ib_mad_send_buf *msg;
	int ret;

	lockdep_assert_held(&cm_id_priv->lock);

	if (private_data && private_data_len > IB_CM_DREP_PRIVATE_DATA_SIZE)
		return -EINVAL;

	if (cm_id_priv->id.state != IB_CM_DREQ_RCVD) {
		trace_icm_send_drep_err(&cm_id_priv->id);
		kfree(private_data);
		return -EINVAL;
	}

	cm_set_private_data(cm_id_priv, private_data, private_data_len);
	cm_enter_timewait(cm_id_priv);

	msg = cm_alloc_msg(cm_id_priv);
	if (IS_ERR(msg))
		return PTR_ERR(msg);

	cm_format_drep((struct cm_drep_msg *) msg->mad, cm_id_priv,
		       private_data, private_data_len);

	trace_icm_send_drep(&cm_id_priv->id);
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		cm_free_msg(msg);
		return ret;
	}
	return 0;
}

int ib_send_cm_drep(struct ib_cm_id *cm_id, const void *private_data,
		    u8 private_data_len)
{
	struct cm_id_private *cm_id_priv =
		container_of(cm_id, struct cm_id_private, id);
	unsigned long flags;
	void *data;
	int ret;

>>>>>>> upstream/android-13
	data = cm_copy_private_data(private_data, private_data_len);
	if (IS_ERR(data))
		return PTR_ERR(data);

<<<<<<< HEAD
	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_DREQ_RCVD) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		kfree(data);
		pr_debug("%s: local_id %d, cm_idcm_id->state(%d) != IB_CM_DREQ_RCVD\n",
			 __func__, be32_to_cpu(cm_id->local_id), cm_id->state);
		return -EINVAL;
	}

	cm_set_private_data(cm_id_priv, data, private_data_len);
	cm_enter_timewait(cm_id_priv);

	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto out;

	cm_format_drep((struct cm_drep_msg *) msg->mad, cm_id_priv,
		       private_data, private_data_len);

	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		return ret;
	}

out:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
=======
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	ret = cm_send_drep_locked(cm_id_priv, data, private_data_len);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_drep);

static int cm_issue_drep(struct cm_port *port,
			 struct ib_mad_recv_wc *mad_recv_wc)
{
	struct ib_mad_send_buf *msg = NULL;
	struct cm_dreq_msg *dreq_msg;
	struct cm_drep_msg *drep_msg;
	int ret;

	ret = cm_alloc_response_msg(port, mad_recv_wc, &msg);
	if (ret)
		return ret;

	dreq_msg = (struct cm_dreq_msg *) mad_recv_wc->recv_buf.mad;
	drep_msg = (struct cm_drep_msg *) msg->mad;

	cm_format_mad_hdr(&drep_msg->hdr, CM_DREP_ATTR_ID, dreq_msg->hdr.tid);
<<<<<<< HEAD
	drep_msg->remote_comm_id = dreq_msg->local_comm_id;
	drep_msg->local_comm_id = dreq_msg->remote_comm_id;

	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		cm_free_msg(msg);
=======
	IBA_SET(CM_DREP_REMOTE_COMM_ID, drep_msg,
		IBA_GET(CM_DREQ_LOCAL_COMM_ID, dreq_msg));
	IBA_SET(CM_DREP_LOCAL_COMM_ID, drep_msg,
		IBA_GET(CM_DREQ_REMOTE_COMM_ID, dreq_msg));

	trace_icm_issue_drep(
		IBA_GET(CM_DREQ_LOCAL_COMM_ID, dreq_msg),
		IBA_GET(CM_DREQ_REMOTE_COMM_ID, dreq_msg));
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		cm_free_response_msg(msg);
>>>>>>> upstream/android-13

	return ret;
}

static int cm_dreq_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_dreq_msg *dreq_msg;
	struct ib_mad_send_buf *msg = NULL;
<<<<<<< HEAD
	int ret;

	dreq_msg = (struct cm_dreq_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(dreq_msg->remote_comm_id,
				   dreq_msg->local_comm_id);
	if (!cm_id_priv) {
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_DREQ_COUNTER]);
		cm_issue_drep(work->port, work->mad_recv_wc);
		pr_debug("%s: no cm_id_priv, local_comm_id %d, remote_comm_id %d\n",
			 __func__, be32_to_cpu(dreq_msg->local_comm_id),
			 be32_to_cpu(dreq_msg->remote_comm_id));
		return -EINVAL;
	}

	work->cm_event.private_data = &dreq_msg->private_data;

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->local_qpn != cm_dreq_get_remote_qpn(dreq_msg))
=======

	dreq_msg = (struct cm_dreq_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_DREQ_REMOTE_COMM_ID, dreq_msg)),
		cpu_to_be32(IBA_GET(CM_DREQ_LOCAL_COMM_ID, dreq_msg)));
	if (!cm_id_priv) {
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_DREQ_COUNTER]);
		cm_issue_drep(work->port, work->mad_recv_wc);
		trace_icm_no_priv_err(
			IBA_GET(CM_DREQ_LOCAL_COMM_ID, dreq_msg),
			IBA_GET(CM_DREQ_REMOTE_COMM_ID, dreq_msg));
		return -EINVAL;
	}

	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_DREQ_PRIVATE_DATA, dreq_msg);

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->local_qpn !=
	    cpu_to_be32(IBA_GET(CM_DREQ_REMOTE_QPN_EECN, dreq_msg)))
>>>>>>> upstream/android-13
		goto unlock;

	switch (cm_id_priv->id.state) {
	case IB_CM_REP_SENT:
	case IB_CM_DREQ_SENT:
<<<<<<< HEAD
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
=======
	case IB_CM_MRA_REP_RCVD:
		ib_cancel_mad(cm_id_priv->msg);
>>>>>>> upstream/android-13
		break;
	case IB_CM_ESTABLISHED:
		if (cm_id_priv->id.lap_state == IB_CM_LAP_SENT ||
		    cm_id_priv->id.lap_state == IB_CM_MRA_LAP_RCVD)
<<<<<<< HEAD
			ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		break;
	case IB_CM_MRA_REP_RCVD:
		break;
	case IB_CM_TIMEWAIT:
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_DREQ_COUNTER]);
=======
			ib_cancel_mad(cm_id_priv->msg);
		break;
	case IB_CM_TIMEWAIT:
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_DREQ_COUNTER]);
>>>>>>> upstream/android-13
		msg = cm_alloc_response_msg_no_ah(work->port, work->mad_recv_wc);
		if (IS_ERR(msg))
			goto unlock;

		cm_format_drep((struct cm_drep_msg *) msg->mad, cm_id_priv,
			       cm_id_priv->private_data,
			       cm_id_priv->private_data_len);
		spin_unlock_irq(&cm_id_priv->lock);

		if (cm_create_response_msg_ah(work->port, work->mad_recv_wc, msg) ||
		    ib_post_send_mad(msg, NULL))
<<<<<<< HEAD
			cm_free_msg(msg);
		goto deref;
	case IB_CM_DREQ_RCVD:
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_DREQ_COUNTER]);
		goto unlock;
	default:
		pr_debug("%s: local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
=======
			cm_free_response_msg(msg);
		goto deref;
	case IB_CM_DREQ_RCVD:
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_DREQ_COUNTER]);
		goto unlock;
	default:
		trace_icm_dreq_unknown_err(&cm_id_priv->id);
>>>>>>> upstream/android-13
		goto unlock;
	}
	cm_id_priv->id.state = IB_CM_DREQ_RCVD;
	cm_id_priv->tid = dreq_msg->hdr.tid;
<<<<<<< HEAD
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;

unlock:	spin_unlock_irq(&cm_id_priv->lock);
deref:	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

static int cm_drep_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_drep_msg *drep_msg;
<<<<<<< HEAD
	int ret;

	drep_msg = (struct cm_drep_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(drep_msg->remote_comm_id,
				   drep_msg->local_comm_id);
	if (!cm_id_priv)
		return -EINVAL;

	work->cm_event.private_data = &drep_msg->private_data;
=======

	drep_msg = (struct cm_drep_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_DREP_REMOTE_COMM_ID, drep_msg)),
		cpu_to_be32(IBA_GET(CM_DREP_LOCAL_COMM_ID, drep_msg)));
	if (!cm_id_priv)
		return -EINVAL;

	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_DREP_PRIVATE_DATA, drep_msg);
>>>>>>> upstream/android-13

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state != IB_CM_DREQ_SENT &&
	    cm_id_priv->id.state != IB_CM_DREQ_RCVD) {
		spin_unlock_irq(&cm_id_priv->lock);
		goto out;
	}
	cm_enter_timewait(cm_id_priv);

<<<<<<< HEAD
	ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	ib_cancel_mad(cm_id_priv->msg);
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

<<<<<<< HEAD
int ib_send_cm_rej(struct ib_cm_id *cm_id,
		   enum ib_cm_rej_reason reason,
		   void *ari,
		   u8 ari_length,
		   const void *private_data,
		   u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	int ret;

=======
static int cm_send_rej_locked(struct cm_id_private *cm_id_priv,
			      enum ib_cm_rej_reason reason, void *ari,
			      u8 ari_length, const void *private_data,
			      u8 private_data_len)
{
	enum ib_cm_state state = cm_id_priv->id.state;
	struct ib_mad_send_buf *msg;
	int ret;

	lockdep_assert_held(&cm_id_priv->lock);

>>>>>>> upstream/android-13
	if ((private_data && private_data_len > IB_CM_REJ_PRIVATE_DATA_SIZE) ||
	    (ari && ari_length > IB_CM_REJ_ARI_LENGTH))
		return -EINVAL;

<<<<<<< HEAD
	cm_id_priv = container_of(cm_id, struct cm_id_private, id);

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	switch (cm_id->state) {
=======
	switch (state) {
>>>>>>> upstream/android-13
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
	case IB_CM_REQ_RCVD:
	case IB_CM_MRA_REQ_SENT:
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
<<<<<<< HEAD
		ret = cm_alloc_msg(cm_id_priv, &msg);
		if (!ret)
			cm_format_rej((struct cm_rej_msg *) msg->mad,
				      cm_id_priv, reason, ari, ari_length,
				      private_data, private_data_len);

		cm_reset_to_idle(cm_id_priv);
		break;
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
		ret = cm_alloc_msg(cm_id_priv, &msg);
		if (!ret)
			cm_format_rej((struct cm_rej_msg *) msg->mad,
				      cm_id_priv, reason, ari, ari_length,
				      private_data, private_data_len);

		cm_enter_timewait(cm_id_priv);
		break;
	default:
		pr_debug("%s: local_id %d, cm_id->state: %d\n", __func__,
			 be32_to_cpu(cm_id_priv->id.local_id), cm_id->state);
		ret = -EINVAL;
		goto out;
	}

	if (ret)
		goto out;

	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		cm_free_msg(msg);

out:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
=======
		cm_reset_to_idle(cm_id_priv);
		msg = cm_alloc_msg(cm_id_priv);
		if (IS_ERR(msg))
			return PTR_ERR(msg);
		cm_format_rej((struct cm_rej_msg *)msg->mad, cm_id_priv, reason,
			      ari, ari_length, private_data, private_data_len,
			      state);
		break;
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
		cm_enter_timewait(cm_id_priv);
		msg = cm_alloc_msg(cm_id_priv);
		if (IS_ERR(msg))
			return PTR_ERR(msg);
		cm_format_rej((struct cm_rej_msg *)msg->mad, cm_id_priv, reason,
			      ari, ari_length, private_data, private_data_len,
			      state);
		break;
	default:
		trace_icm_send_unknown_rej_err(&cm_id_priv->id);
		return -EINVAL;
	}

	trace_icm_send_rej(&cm_id_priv->id, reason);
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		cm_free_msg(msg);
		return ret;
	}

	return 0;
}

int ib_send_cm_rej(struct ib_cm_id *cm_id, enum ib_cm_rej_reason reason,
		   void *ari, u8 ari_length, const void *private_data,
		   u8 private_data_len)
{
	struct cm_id_private *cm_id_priv =
		container_of(cm_id, struct cm_id_private, id);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	ret = cm_send_rej_locked(cm_id_priv, reason, ari, ari_length,
				 private_data, private_data_len);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_rej);

static void cm_format_rej_event(struct cm_work *work)
{
	struct cm_rej_msg *rej_msg;
	struct ib_cm_rej_event_param *param;

	rej_msg = (struct cm_rej_msg *)work->mad_recv_wc->recv_buf.mad;
	param = &work->cm_event.param.rej_rcvd;
<<<<<<< HEAD
	param->ari = rej_msg->ari;
	param->ari_length = cm_rej_get_reject_info_len(rej_msg);
	param->reason = __be16_to_cpu(rej_msg->reason);
	work->cm_event.private_data = &rej_msg->private_data;
}

static struct cm_id_private * cm_acquire_rejected_id(struct cm_rej_msg *rej_msg)
{
	struct cm_timewait_info *timewait_info;
	struct cm_id_private *cm_id_priv;
	__be32 remote_id;

	remote_id = rej_msg->local_comm_id;

	if (__be16_to_cpu(rej_msg->reason) == IB_CM_REJ_TIMEOUT) {
		spin_lock_irq(&cm.lock);
		timewait_info = cm_find_remote_id( *((__be64 *) rej_msg->ari),
						  remote_id);
		if (!timewait_info) {
			spin_unlock_irq(&cm.lock);
			return NULL;
		}
		cm_id_priv = idr_find(&cm.local_id_table, (__force int)
				      (timewait_info->work.local_id ^
				       cm.random_id_operand));
		if (cm_id_priv) {
			if (cm_id_priv->id.remote_id == remote_id)
				atomic_inc(&cm_id_priv->refcount);
			else
				cm_id_priv = NULL;
		}
		spin_unlock_irq(&cm.lock);
	} else if (cm_rej_get_msg_rejected(rej_msg) == CM_MSG_RESPONSE_REQ)
		cm_id_priv = cm_acquire_id(rej_msg->remote_comm_id, 0);
	else
		cm_id_priv = cm_acquire_id(rej_msg->remote_comm_id, remote_id);
=======
	param->ari = IBA_GET_MEM_PTR(CM_REJ_ARI, rej_msg);
	param->ari_length = IBA_GET(CM_REJ_REJECTED_INFO_LENGTH, rej_msg);
	param->reason = IBA_GET(CM_REJ_REASON, rej_msg);
	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_REJ_PRIVATE_DATA, rej_msg);
}

static struct cm_id_private *cm_acquire_rejected_id(struct cm_rej_msg *rej_msg)
{
	struct cm_id_private *cm_id_priv;
	__be32 remote_id;

	remote_id = cpu_to_be32(IBA_GET(CM_REJ_LOCAL_COMM_ID, rej_msg));

	if (IBA_GET(CM_REJ_REASON, rej_msg) == IB_CM_REJ_TIMEOUT) {
		cm_id_priv = cm_find_remote_id(
			*((__be64 *)IBA_GET_MEM_PTR(CM_REJ_ARI, rej_msg)),
			remote_id);
	} else if (IBA_GET(CM_REJ_MESSAGE_REJECTED, rej_msg) ==
		   CM_MSG_RESPONSE_REQ)
		cm_id_priv = cm_acquire_id(
			cpu_to_be32(IBA_GET(CM_REJ_REMOTE_COMM_ID, rej_msg)),
			0);
	else
		cm_id_priv = cm_acquire_id(
			cpu_to_be32(IBA_GET(CM_REJ_REMOTE_COMM_ID, rej_msg)),
			remote_id);
>>>>>>> upstream/android-13

	return cm_id_priv;
}

static int cm_rej_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_rej_msg *rej_msg;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> upstream/android-13

	rej_msg = (struct cm_rej_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_rejected_id(rej_msg);
	if (!cm_id_priv)
		return -EINVAL;

	cm_format_rej_event(work);

	spin_lock_irq(&cm_id_priv->lock);
	switch (cm_id_priv->id.state) {
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
<<<<<<< HEAD
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		/* fall through */
	case IB_CM_REQ_RCVD:
	case IB_CM_MRA_REQ_SENT:
		if (__be16_to_cpu(rej_msg->reason) == IB_CM_REJ_STALE_CONN)
=======
		ib_cancel_mad(cm_id_priv->msg);
		fallthrough;
	case IB_CM_REQ_RCVD:
	case IB_CM_MRA_REQ_SENT:
		if (IBA_GET(CM_REJ_REASON, rej_msg) == IB_CM_REJ_STALE_CONN)
>>>>>>> upstream/android-13
			cm_enter_timewait(cm_id_priv);
		else
			cm_reset_to_idle(cm_id_priv);
		break;
	case IB_CM_DREQ_SENT:
<<<<<<< HEAD
		ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
		/* fall through */
=======
		ib_cancel_mad(cm_id_priv->msg);
		fallthrough;
>>>>>>> upstream/android-13
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
		cm_enter_timewait(cm_id_priv);
		break;
	case IB_CM_ESTABLISHED:
		if (cm_id_priv->id.lap_state == IB_CM_LAP_UNINIT ||
		    cm_id_priv->id.lap_state == IB_CM_LAP_SENT) {
			if (cm_id_priv->id.lap_state == IB_CM_LAP_SENT)
<<<<<<< HEAD
				ib_cancel_mad(cm_id_priv->av.port->mad_agent,
					      cm_id_priv->msg);
			cm_enter_timewait(cm_id_priv);
			break;
		}
		/* fall through */
	default:
		spin_unlock_irq(&cm_id_priv->lock);
		pr_debug("%s: local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
		ret = -EINVAL;
		goto out;
	}

	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
				ib_cancel_mad(cm_id_priv->msg);
			cm_enter_timewait(cm_id_priv);
			break;
		}
		fallthrough;
	default:
		trace_icm_rej_unknown_err(&cm_id_priv->id);
		spin_unlock_irq(&cm_id_priv->lock);
		goto out;
	}

	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

int ib_send_cm_mra(struct ib_cm_id *cm_id,
		   u8 service_timeout,
		   const void *private_data,
		   u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	enum ib_cm_state cm_state;
	enum ib_cm_lap_state lap_state;
	enum cm_msg_response msg_response;
	void *data;
	unsigned long flags;
	int ret;

	if (private_data && private_data_len > IB_CM_MRA_PRIVATE_DATA_SIZE)
		return -EINVAL;

	data = cm_copy_private_data(private_data, private_data_len);
	if (IS_ERR(data))
		return PTR_ERR(data);

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);

	spin_lock_irqsave(&cm_id_priv->lock, flags);
<<<<<<< HEAD
	switch(cm_id_priv->id.state) {
=======
	switch (cm_id_priv->id.state) {
>>>>>>> upstream/android-13
	case IB_CM_REQ_RCVD:
		cm_state = IB_CM_MRA_REQ_SENT;
		lap_state = cm_id->lap_state;
		msg_response = CM_MSG_RESPONSE_REQ;
		break;
	case IB_CM_REP_RCVD:
		cm_state = IB_CM_MRA_REP_SENT;
		lap_state = cm_id->lap_state;
		msg_response = CM_MSG_RESPONSE_REP;
		break;
	case IB_CM_ESTABLISHED:
		if (cm_id->lap_state == IB_CM_LAP_RCVD) {
			cm_state = cm_id->state;
			lap_state = IB_CM_MRA_LAP_SENT;
			msg_response = CM_MSG_RESPONSE_OTHER;
			break;
		}
<<<<<<< HEAD
		/* fall through */
	default:
		pr_debug("%s: local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
		ret = -EINVAL;
		goto error1;
	}

	if (!(service_timeout & IB_CM_MRA_FLAG_DELAY)) {
		ret = cm_alloc_msg(cm_id_priv, &msg);
		if (ret)
			goto error1;
=======
		fallthrough;
	default:
		trace_icm_send_mra_unknown_err(&cm_id_priv->id);
		ret = -EINVAL;
		goto error_unlock;
	}

	if (!(service_timeout & IB_CM_MRA_FLAG_DELAY)) {
		msg = cm_alloc_msg(cm_id_priv);
		if (IS_ERR(msg)) {
			ret = PTR_ERR(msg);
			goto error_unlock;
		}
>>>>>>> upstream/android-13

		cm_format_mra((struct cm_mra_msg *) msg->mad, cm_id_priv,
			      msg_response, service_timeout,
			      private_data, private_data_len);
<<<<<<< HEAD
		ret = ib_post_send_mad(msg, NULL);
		if (ret)
			goto error2;
=======
		trace_icm_send_mra(cm_id);
		ret = ib_post_send_mad(msg, NULL);
		if (ret)
			goto error_free_msg;
>>>>>>> upstream/android-13
	}

	cm_id->state = cm_state;
	cm_id->lap_state = lap_state;
	cm_id_priv->service_timeout = service_timeout;
	cm_set_private_data(cm_id_priv, data, private_data_len);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return 0;

<<<<<<< HEAD
error1:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	kfree(data);
	return ret;

error2:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	kfree(data);
	cm_free_msg(msg);
=======
error_free_msg:
	cm_free_msg(msg);
error_unlock:
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	kfree(data);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_mra);

<<<<<<< HEAD
static struct cm_id_private * cm_acquire_mraed_id(struct cm_mra_msg *mra_msg)
{
	switch (cm_mra_get_msg_mraed(mra_msg)) {
	case CM_MSG_RESPONSE_REQ:
		return cm_acquire_id(mra_msg->remote_comm_id, 0);
	case CM_MSG_RESPONSE_REP:
	case CM_MSG_RESPONSE_OTHER:
		return cm_acquire_id(mra_msg->remote_comm_id,
				     mra_msg->local_comm_id);
=======
static struct cm_id_private *cm_acquire_mraed_id(struct cm_mra_msg *mra_msg)
{
	switch (IBA_GET(CM_MRA_MESSAGE_MRAED, mra_msg)) {
	case CM_MSG_RESPONSE_REQ:
		return cm_acquire_id(
			cpu_to_be32(IBA_GET(CM_MRA_REMOTE_COMM_ID, mra_msg)),
			0);
	case CM_MSG_RESPONSE_REP:
	case CM_MSG_RESPONSE_OTHER:
		return cm_acquire_id(
			cpu_to_be32(IBA_GET(CM_MRA_REMOTE_COMM_ID, mra_msg)),
			cpu_to_be32(IBA_GET(CM_MRA_LOCAL_COMM_ID, mra_msg)));
>>>>>>> upstream/android-13
	default:
		return NULL;
	}
}

static int cm_mra_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_mra_msg *mra_msg;
<<<<<<< HEAD
	int timeout, ret;
=======
	int timeout;
>>>>>>> upstream/android-13

	mra_msg = (struct cm_mra_msg *)work->mad_recv_wc->recv_buf.mad;
	cm_id_priv = cm_acquire_mraed_id(mra_msg);
	if (!cm_id_priv)
		return -EINVAL;

<<<<<<< HEAD
	work->cm_event.private_data = &mra_msg->private_data;
	work->cm_event.param.mra_rcvd.service_timeout =
					cm_mra_get_service_timeout(mra_msg);
	timeout = cm_convert_to_ms(cm_mra_get_service_timeout(mra_msg)) +
=======
	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_MRA_PRIVATE_DATA, mra_msg);
	work->cm_event.param.mra_rcvd.service_timeout =
		IBA_GET(CM_MRA_SERVICE_TIMEOUT, mra_msg);
	timeout = cm_convert_to_ms(IBA_GET(CM_MRA_SERVICE_TIMEOUT, mra_msg)) +
>>>>>>> upstream/android-13
		  cm_convert_to_ms(cm_id_priv->av.timeout);

	spin_lock_irq(&cm_id_priv->lock);
	switch (cm_id_priv->id.state) {
	case IB_CM_REQ_SENT:
<<<<<<< HEAD
		if (cm_mra_get_msg_mraed(mra_msg) != CM_MSG_RESPONSE_REQ ||
		    ib_modify_mad(cm_id_priv->av.port->mad_agent,
				  cm_id_priv->msg, timeout))
=======
		if (IBA_GET(CM_MRA_MESSAGE_MRAED, mra_msg) !=
			    CM_MSG_RESPONSE_REQ ||
		    ib_modify_mad(cm_id_priv->msg, timeout))
>>>>>>> upstream/android-13
			goto out;
		cm_id_priv->id.state = IB_CM_MRA_REQ_RCVD;
		break;
	case IB_CM_REP_SENT:
<<<<<<< HEAD
		if (cm_mra_get_msg_mraed(mra_msg) != CM_MSG_RESPONSE_REP ||
		    ib_modify_mad(cm_id_priv->av.port->mad_agent,
				  cm_id_priv->msg, timeout))
=======
		if (IBA_GET(CM_MRA_MESSAGE_MRAED, mra_msg) !=
			    CM_MSG_RESPONSE_REP ||
		    ib_modify_mad(cm_id_priv->msg, timeout))
>>>>>>> upstream/android-13
			goto out;
		cm_id_priv->id.state = IB_CM_MRA_REP_RCVD;
		break;
	case IB_CM_ESTABLISHED:
<<<<<<< HEAD
		if (cm_mra_get_msg_mraed(mra_msg) != CM_MSG_RESPONSE_OTHER ||
		    cm_id_priv->id.lap_state != IB_CM_LAP_SENT ||
		    ib_modify_mad(cm_id_priv->av.port->mad_agent,
				  cm_id_priv->msg, timeout)) {
			if (cm_id_priv->id.lap_state == IB_CM_MRA_LAP_RCVD)
				atomic_long_inc(&work->port->
						counter_group[CM_RECV_DUPLICATES].
						counter[CM_MRA_COUNTER]);
=======
		if (IBA_GET(CM_MRA_MESSAGE_MRAED, mra_msg) !=
			    CM_MSG_RESPONSE_OTHER ||
		    cm_id_priv->id.lap_state != IB_CM_LAP_SENT ||
		    ib_modify_mad(cm_id_priv->msg, timeout)) {
			if (cm_id_priv->id.lap_state == IB_CM_MRA_LAP_RCVD)
				atomic_long_inc(
					&work->port->counters[CM_RECV_DUPLICATES]
							     [CM_MRA_COUNTER]);
>>>>>>> upstream/android-13
			goto out;
		}
		cm_id_priv->id.lap_state = IB_CM_MRA_LAP_RCVD;
		break;
	case IB_CM_MRA_REQ_RCVD:
	case IB_CM_MRA_REP_RCVD:
<<<<<<< HEAD
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_MRA_COUNTER]);
		/* fall through */
	default:
		pr_debug("%s local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
=======
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_MRA_COUNTER]);
		fallthrough;
	default:
		trace_icm_mra_unknown_err(&cm_id_priv->id);
>>>>>>> upstream/android-13
		goto out;
	}

	cm_id_priv->msg->context[1] = (void *) (unsigned long)
				      cm_id_priv->id.state;
<<<<<<< HEAD
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	spin_unlock_irq(&cm_id_priv->lock);
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

<<<<<<< HEAD
static void cm_format_lap(struct cm_lap_msg *lap_msg,
			  struct cm_id_private *cm_id_priv,
			  struct sa_path_rec *alternate_path,
			  const void *private_data,
			  u8 private_data_len)
{
	bool alt_ext = false;

	if (alternate_path->rec_type == SA_PATH_REC_TYPE_OPA)
		alt_ext = opa_is_extended_lid(alternate_path->opa.dlid,
					      alternate_path->opa.slid);
	cm_format_mad_hdr(&lap_msg->hdr, CM_LAP_ATTR_ID,
			  cm_form_tid(cm_id_priv));
	lap_msg->local_comm_id = cm_id_priv->id.local_id;
	lap_msg->remote_comm_id = cm_id_priv->id.remote_id;
	cm_lap_set_remote_qpn(lap_msg, cm_id_priv->remote_qpn);
	/* todo: need remote CM response timeout */
	cm_lap_set_remote_resp_timeout(lap_msg, 0x1F);
	lap_msg->alt_local_lid =
		htons(ntohl(sa_path_get_slid(alternate_path)));
	lap_msg->alt_remote_lid =
		htons(ntohl(sa_path_get_dlid(alternate_path)));
	lap_msg->alt_local_gid = alternate_path->sgid;
	lap_msg->alt_remote_gid = alternate_path->dgid;
	if (alt_ext) {
		lap_msg->alt_local_gid.global.interface_id
			= OPA_MAKE_ID(be32_to_cpu(alternate_path->opa.slid));
		lap_msg->alt_remote_gid.global.interface_id
			= OPA_MAKE_ID(be32_to_cpu(alternate_path->opa.dlid));
	}
	cm_lap_set_flow_label(lap_msg, alternate_path->flow_label);
	cm_lap_set_traffic_class(lap_msg, alternate_path->traffic_class);
	lap_msg->alt_hop_limit = alternate_path->hop_limit;
	cm_lap_set_packet_rate(lap_msg, alternate_path->rate);
	cm_lap_set_sl(lap_msg, alternate_path->sl);
	cm_lap_set_subnet_local(lap_msg, 1); /* local only... */
	cm_lap_set_local_ack_timeout(lap_msg,
		cm_ack_timeout(cm_id_priv->av.port->cm_dev->ack_delay,
			       alternate_path->packet_life_time));

	if (private_data && private_data_len)
		memcpy(lap_msg->private_data, private_data, private_data_len);
}

int ib_send_cm_lap(struct ib_cm_id *cm_id,
		   struct sa_path_rec *alternate_path,
		   const void *private_data,
		   u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	int ret;

	if (private_data && private_data_len > IB_CM_LAP_PRIVATE_DATA_SIZE)
		return -EINVAL;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_ESTABLISHED ||
	    (cm_id->lap_state != IB_CM_LAP_UNINIT &&
	     cm_id->lap_state != IB_CM_LAP_IDLE)) {
		ret = -EINVAL;
		goto out;
	}

	ret = cm_init_av_by_path(alternate_path, NULL, &cm_id_priv->alt_av,
				 cm_id_priv);
	if (ret)
		goto out;
	cm_id_priv->alt_av.timeout =
			cm_ack_timeout(cm_id_priv->target_ack_delay,
				       cm_id_priv->alt_av.timeout - 1);

	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto out;

	cm_format_lap((struct cm_lap_msg *) msg->mad, cm_id_priv,
		      alternate_path, private_data, private_data_len);
	msg->timeout_ms = cm_id_priv->timeout_ms;
	msg->context[1] = (void *) (unsigned long) IB_CM_ESTABLISHED;

	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		return ret;
	}

	cm_id->lap_state = IB_CM_LAP_SENT;
	cm_id_priv->msg = msg;

out:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_lap);

=======
>>>>>>> upstream/android-13
static void cm_format_path_lid_from_lap(struct cm_lap_msg *lap_msg,
					struct sa_path_rec *path)
{
	u32 lid;

	if (path->rec_type != SA_PATH_REC_TYPE_OPA) {
<<<<<<< HEAD
		sa_path_set_dlid(path, ntohs(lap_msg->alt_local_lid));
		sa_path_set_slid(path, ntohs(lap_msg->alt_remote_lid));
	} else {
		lid = opa_get_lid_from_gid(&lap_msg->alt_local_gid);
		sa_path_set_dlid(path, lid);

		lid = opa_get_lid_from_gid(&lap_msg->alt_remote_gid);
=======
		sa_path_set_dlid(path, IBA_GET(CM_LAP_ALTERNATE_LOCAL_PORT_LID,
					       lap_msg));
		sa_path_set_slid(path, IBA_GET(CM_LAP_ALTERNATE_REMOTE_PORT_LID,
					       lap_msg));
	} else {
		lid = opa_get_lid_from_gid(IBA_GET_MEM_PTR(
			CM_LAP_ALTERNATE_LOCAL_PORT_GID, lap_msg));
		sa_path_set_dlid(path, lid);

		lid = opa_get_lid_from_gid(IBA_GET_MEM_PTR(
			CM_LAP_ALTERNATE_REMOTE_PORT_GID, lap_msg));
>>>>>>> upstream/android-13
		sa_path_set_slid(path, lid);
	}
}

static void cm_format_path_from_lap(struct cm_id_private *cm_id_priv,
				    struct sa_path_rec *path,
				    struct cm_lap_msg *lap_msg)
{
<<<<<<< HEAD
	path->dgid = lap_msg->alt_local_gid;
	path->sgid = lap_msg->alt_remote_gid;
	path->flow_label = cm_lap_get_flow_label(lap_msg);
	path->hop_limit = lap_msg->alt_hop_limit;
	path->traffic_class = cm_lap_get_traffic_class(lap_msg);
	path->reversible = 1;
	path->pkey = cm_id_priv->pkey;
	path->sl = cm_lap_get_sl(lap_msg);
	path->mtu_selector = IB_SA_EQ;
	path->mtu = cm_id_priv->path_mtu;
	path->rate_selector = IB_SA_EQ;
	path->rate = cm_lap_get_packet_rate(lap_msg);
	path->packet_life_time_selector = IB_SA_EQ;
	path->packet_life_time = cm_lap_get_local_ack_timeout(lap_msg);
=======
	path->dgid = *IBA_GET_MEM_PTR(CM_LAP_ALTERNATE_LOCAL_PORT_GID, lap_msg);
	path->sgid =
		*IBA_GET_MEM_PTR(CM_LAP_ALTERNATE_REMOTE_PORT_GID, lap_msg);
	path->flow_label =
		cpu_to_be32(IBA_GET(CM_LAP_ALTERNATE_FLOW_LABEL, lap_msg));
	path->hop_limit = IBA_GET(CM_LAP_ALTERNATE_HOP_LIMIT, lap_msg);
	path->traffic_class = IBA_GET(CM_LAP_ALTERNATE_TRAFFIC_CLASS, lap_msg);
	path->reversible = 1;
	path->pkey = cm_id_priv->pkey;
	path->sl = IBA_GET(CM_LAP_ALTERNATE_SL, lap_msg);
	path->mtu_selector = IB_SA_EQ;
	path->mtu = cm_id_priv->path_mtu;
	path->rate_selector = IB_SA_EQ;
	path->rate = IBA_GET(CM_LAP_ALTERNATE_PACKET_RATE, lap_msg);
	path->packet_life_time_selector = IB_SA_EQ;
	path->packet_life_time =
		IBA_GET(CM_LAP_ALTERNATE_LOCAL_ACK_TIMEOUT, lap_msg);
>>>>>>> upstream/android-13
	path->packet_life_time -= (path->packet_life_time > 0);
	cm_format_path_lid_from_lap(lap_msg, path);
}

static int cm_lap_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_lap_msg *lap_msg;
	struct ib_cm_lap_event_param *param;
	struct ib_mad_send_buf *msg = NULL;
<<<<<<< HEAD
=======
	struct rdma_ah_attr ah_attr;
	struct cm_av alt_av = {};
>>>>>>> upstream/android-13
	int ret;

	/* Currently Alternate path messages are not supported for
	 * RoCE link layer.
	 */
	if (rdma_protocol_roce(work->port->cm_dev->ib_device,
			       work->port->port_num))
		return -EINVAL;

	/* todo: verify LAP request and send reject APR if invalid. */
	lap_msg = (struct cm_lap_msg *)work->mad_recv_wc->recv_buf.mad;
<<<<<<< HEAD
	cm_id_priv = cm_acquire_id(lap_msg->remote_comm_id,
				   lap_msg->local_comm_id);
=======
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_LAP_REMOTE_COMM_ID, lap_msg)),
		cpu_to_be32(IBA_GET(CM_LAP_LOCAL_COMM_ID, lap_msg)));
>>>>>>> upstream/android-13
	if (!cm_id_priv)
		return -EINVAL;

	param = &work->cm_event.param.lap_rcvd;
	memset(&work->path[0], 0, sizeof(work->path[1]));
	cm_path_set_rec_type(work->port->cm_dev->ib_device,
<<<<<<< HEAD
			     work->port->port_num,
			     &work->path[0],
			     &lap_msg->alt_local_gid);
	param->alternate_path = &work->path[0];
	cm_format_path_from_lap(cm_id_priv, param->alternate_path, lap_msg);
	work->cm_event.private_data = &lap_msg->private_data;

	spin_lock_irq(&cm_id_priv->lock);
=======
			     work->port->port_num, &work->path[0],
			     IBA_GET_MEM_PTR(CM_LAP_ALTERNATE_LOCAL_PORT_GID,
					     lap_msg));
	param->alternate_path = &work->path[0];
	cm_format_path_from_lap(cm_id_priv, param->alternate_path, lap_msg);
	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_LAP_PRIVATE_DATA, lap_msg);

	ret = ib_init_ah_attr_from_wc(work->port->cm_dev->ib_device,
				      work->port->port_num,
				      work->mad_recv_wc->wc,
				      work->mad_recv_wc->recv_buf.grh,
				      &ah_attr);
	if (ret)
		goto deref;

	ret = cm_init_av_by_path(param->alternate_path, NULL, &alt_av);
	if (ret) {
		rdma_destroy_ah_attr(&ah_attr);
		goto deref;
	}

	spin_lock_irq(&cm_id_priv->lock);
	cm_init_av_for_lap(work->port, work->mad_recv_wc->wc,
			   &ah_attr, &cm_id_priv->av);
	cm_move_av_from_path(&cm_id_priv->alt_av, &alt_av);

>>>>>>> upstream/android-13
	if (cm_id_priv->id.state != IB_CM_ESTABLISHED)
		goto unlock;

	switch (cm_id_priv->id.lap_state) {
	case IB_CM_LAP_UNINIT:
	case IB_CM_LAP_IDLE:
		break;
	case IB_CM_MRA_LAP_SENT:
<<<<<<< HEAD
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_LAP_COUNTER]);
=======
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_LAP_COUNTER]);
>>>>>>> upstream/android-13
		msg = cm_alloc_response_msg_no_ah(work->port, work->mad_recv_wc);
		if (IS_ERR(msg))
			goto unlock;

		cm_format_mra((struct cm_mra_msg *) msg->mad, cm_id_priv,
			      CM_MSG_RESPONSE_OTHER,
			      cm_id_priv->service_timeout,
			      cm_id_priv->private_data,
			      cm_id_priv->private_data_len);
		spin_unlock_irq(&cm_id_priv->lock);

		if (cm_create_response_msg_ah(work->port, work->mad_recv_wc, msg) ||
		    ib_post_send_mad(msg, NULL))
<<<<<<< HEAD
			cm_free_msg(msg);
		goto deref;
	case IB_CM_LAP_RCVD:
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_LAP_COUNTER]);
=======
			cm_free_response_msg(msg);
		goto deref;
	case IB_CM_LAP_RCVD:
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_LAP_COUNTER]);
>>>>>>> upstream/android-13
		goto unlock;
	default:
		goto unlock;
	}

<<<<<<< HEAD
	ret = cm_init_av_for_lap(work->port, work->mad_recv_wc->wc,
				 work->mad_recv_wc->recv_buf.grh,
				 &cm_id_priv->av);
	if (ret)
		goto unlock;

	ret = cm_init_av_by_path(param->alternate_path, NULL,
				 &cm_id_priv->alt_av, cm_id_priv);
	if (ret)
		goto unlock;

	cm_id_priv->id.lap_state = IB_CM_LAP_RCVD;
	cm_id_priv->tid = lap_msg->hdr.tid;
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	cm_id_priv->id.lap_state = IB_CM_LAP_RCVD;
	cm_id_priv->tid = lap_msg->hdr.tid;
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;

unlock:	spin_unlock_irq(&cm_id_priv->lock);
deref:	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

<<<<<<< HEAD
static void cm_format_apr(struct cm_apr_msg *apr_msg,
			  struct cm_id_private *cm_id_priv,
			  enum ib_cm_apr_status status,
			  void *info,
			  u8 info_length,
			  const void *private_data,
			  u8 private_data_len)
{
	cm_format_mad_hdr(&apr_msg->hdr, CM_APR_ATTR_ID, cm_id_priv->tid);
	apr_msg->local_comm_id = cm_id_priv->id.local_id;
	apr_msg->remote_comm_id = cm_id_priv->id.remote_id;
	apr_msg->ap_status = (u8) status;

	if (info && info_length) {
		apr_msg->info_length = info_length;
		memcpy(apr_msg->info, info, info_length);
	}

	if (private_data && private_data_len)
		memcpy(apr_msg->private_data, private_data, private_data_len);
}

int ib_send_cm_apr(struct ib_cm_id *cm_id,
		   enum ib_cm_apr_status status,
		   void *info,
		   u8 info_length,
		   const void *private_data,
		   u8 private_data_len)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	int ret;

	if ((private_data && private_data_len > IB_CM_APR_PRIVATE_DATA_SIZE) ||
	    (info && info_length > IB_CM_APR_INFO_LENGTH))
		return -EINVAL;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_ESTABLISHED ||
	    (cm_id->lap_state != IB_CM_LAP_RCVD &&
	     cm_id->lap_state != IB_CM_MRA_LAP_SENT)) {
		ret = -EINVAL;
		goto out;
	}

	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto out;

	cm_format_apr((struct cm_apr_msg *) msg->mad, cm_id_priv, status,
		      info, info_length, private_data, private_data_len);
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		return ret;
	}

	cm_id->lap_state = IB_CM_LAP_IDLE;
out:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_apr);

=======
>>>>>>> upstream/android-13
static int cm_apr_handler(struct cm_work *work)
{
	struct cm_id_private *cm_id_priv;
	struct cm_apr_msg *apr_msg;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> upstream/android-13

	/* Currently Alternate path messages are not supported for
	 * RoCE link layer.
	 */
	if (rdma_protocol_roce(work->port->cm_dev->ib_device,
			       work->port->port_num))
		return -EINVAL;

	apr_msg = (struct cm_apr_msg *)work->mad_recv_wc->recv_buf.mad;
<<<<<<< HEAD
	cm_id_priv = cm_acquire_id(apr_msg->remote_comm_id,
				   apr_msg->local_comm_id);
	if (!cm_id_priv)
		return -EINVAL; /* Unmatched reply. */

	work->cm_event.param.apr_rcvd.ap_status = apr_msg->ap_status;
	work->cm_event.param.apr_rcvd.apr_info = &apr_msg->info;
	work->cm_event.param.apr_rcvd.info_len = apr_msg->info_length;
	work->cm_event.private_data = &apr_msg->private_data;
=======
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_APR_REMOTE_COMM_ID, apr_msg)),
		cpu_to_be32(IBA_GET(CM_APR_LOCAL_COMM_ID, apr_msg)));
	if (!cm_id_priv)
		return -EINVAL; /* Unmatched reply. */

	work->cm_event.param.apr_rcvd.ap_status =
		IBA_GET(CM_APR_AR_STATUS, apr_msg);
	work->cm_event.param.apr_rcvd.apr_info =
		IBA_GET_MEM_PTR(CM_APR_ADDITIONAL_INFORMATION, apr_msg);
	work->cm_event.param.apr_rcvd.info_len =
		IBA_GET(CM_APR_ADDITIONAL_INFORMATION_LENGTH, apr_msg);
	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_APR_PRIVATE_DATA, apr_msg);
>>>>>>> upstream/android-13

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state != IB_CM_ESTABLISHED ||
	    (cm_id_priv->id.lap_state != IB_CM_LAP_SENT &&
	     cm_id_priv->id.lap_state != IB_CM_MRA_LAP_RCVD)) {
		spin_unlock_irq(&cm_id_priv->lock);
		goto out;
	}
	cm_id_priv->id.lap_state = IB_CM_LAP_IDLE;
<<<<<<< HEAD
	ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
	cm_id_priv->msg = NULL;

	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	ib_cancel_mad(cm_id_priv->msg);
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

static int cm_timewait_handler(struct cm_work *work)
{
	struct cm_timewait_info *timewait_info;
	struct cm_id_private *cm_id_priv;
<<<<<<< HEAD
	int ret;

	timewait_info = (struct cm_timewait_info *)work;
=======

	timewait_info = container_of(work, struct cm_timewait_info, work);
>>>>>>> upstream/android-13
	spin_lock_irq(&cm.lock);
	list_del(&timewait_info->list);
	spin_unlock_irq(&cm.lock);

	cm_id_priv = cm_acquire_id(timewait_info->work.local_id,
				   timewait_info->work.remote_id);
	if (!cm_id_priv)
		return -EINVAL;

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state != IB_CM_TIMEWAIT ||
	    cm_id_priv->remote_qpn != timewait_info->remote_qpn) {
		spin_unlock_irq(&cm_id_priv->lock);
		goto out;
	}
	cm_id_priv->id.state = IB_CM_IDLE;
<<<<<<< HEAD
	ret = atomic_inc_and_test(&cm_id_priv->work_count);
	if (!ret)
		list_add_tail(&work->list, &cm_id_priv->work_list);
	spin_unlock_irq(&cm_id_priv->lock);

	if (ret)
		cm_process_work(cm_id_priv, work);
	else
		cm_deref_id(cm_id_priv);
=======
	cm_queue_work_unlock(cm_id_priv, work);
>>>>>>> upstream/android-13
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

static void cm_format_sidr_req(struct cm_sidr_req_msg *sidr_req_msg,
			       struct cm_id_private *cm_id_priv,
			       struct ib_cm_sidr_req_param *param)
{
	cm_format_mad_hdr(&sidr_req_msg->hdr, CM_SIDR_REQ_ATTR_ID,
			  cm_form_tid(cm_id_priv));
<<<<<<< HEAD
	sidr_req_msg->request_id = cm_id_priv->id.local_id;
	sidr_req_msg->pkey = param->path->pkey;
	sidr_req_msg->service_id = param->service_id;

	if (param->private_data && param->private_data_len)
		memcpy(sidr_req_msg->private_data, param->private_data,
		       param->private_data_len);
=======
	IBA_SET(CM_SIDR_REQ_REQUESTID, sidr_req_msg,
		be32_to_cpu(cm_id_priv->id.local_id));
	IBA_SET(CM_SIDR_REQ_PARTITION_KEY, sidr_req_msg,
		be16_to_cpu(param->path->pkey));
	IBA_SET(CM_SIDR_REQ_SERVICEID, sidr_req_msg,
		be64_to_cpu(param->service_id));

	if (param->private_data && param->private_data_len)
		IBA_SET_MEM(CM_SIDR_REQ_PRIVATE_DATA, sidr_req_msg,
			    param->private_data, param->private_data_len);
>>>>>>> upstream/android-13
}

int ib_send_cm_sidr_req(struct ib_cm_id *cm_id,
			struct ib_cm_sidr_req_param *param)
{
	struct cm_id_private *cm_id_priv;
	struct ib_mad_send_buf *msg;
<<<<<<< HEAD
=======
	struct cm_av av = {};
>>>>>>> upstream/android-13
	unsigned long flags;
	int ret;

	if (!param->path || (param->private_data &&
	     param->private_data_len > IB_CM_SIDR_REQ_PRIVATE_DATA_SIZE))
		return -EINVAL;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
<<<<<<< HEAD
	ret = cm_init_av_by_path(param->path, param->sgid_attr,
				 &cm_id_priv->av,
				 cm_id_priv);
	if (ret)
		goto out;

=======
	ret = cm_init_av_by_path(param->path, param->sgid_attr, &av);
	if (ret)
		return ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	cm_move_av_from_path(&cm_id_priv->av, &av);
>>>>>>> upstream/android-13
	cm_id->service_id = param->service_id;
	cm_id->service_mask = ~cpu_to_be64(0);
	cm_id_priv->timeout_ms = param->timeout_ms;
	cm_id_priv->max_cm_retries = param->max_cm_retries;
<<<<<<< HEAD
	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto out;

	cm_format_sidr_req((struct cm_sidr_req_msg *) msg->mad, cm_id_priv,
			   param);
	msg->timeout_ms = cm_id_priv->timeout_ms;
	msg->context[1] = (void *) (unsigned long) IB_CM_SIDR_REQ_SENT;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state == IB_CM_IDLE)
		ret = ib_post_send_mad(msg, NULL);
	else
		ret = -EINVAL;

	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		goto out;
	}
	cm_id->state = IB_CM_SIDR_REQ_SENT;
	cm_id_priv->msg = msg;
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
out:
=======
	if (cm_id->state != IB_CM_IDLE) {
		ret = -EINVAL;
		goto out_unlock;
	}

	msg = cm_alloc_priv_msg(cm_id_priv);
	if (IS_ERR(msg)) {
		ret = PTR_ERR(msg);
		goto out_unlock;
	}

	cm_format_sidr_req((struct cm_sidr_req_msg *)msg->mad, cm_id_priv,
			   param);
	msg->timeout_ms = cm_id_priv->timeout_ms;
	msg->context[1] = (void *)(unsigned long)IB_CM_SIDR_REQ_SENT;

	trace_icm_send_sidr_req(&cm_id_priv->id);
	ret = ib_post_send_mad(msg, NULL);
	if (ret)
		goto out_free;
	cm_id->state = IB_CM_SIDR_REQ_SENT;
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return 0;
out_free:
	cm_free_priv_msg(msg);
out_unlock:
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_sidr_req);

static void cm_format_sidr_req_event(struct cm_work *work,
				     const struct cm_id_private *rx_cm_id,
				     struct ib_cm_id *listen_id)
{
	struct cm_sidr_req_msg *sidr_req_msg;
	struct ib_cm_sidr_req_event_param *param;

	sidr_req_msg = (struct cm_sidr_req_msg *)
				work->mad_recv_wc->recv_buf.mad;
	param = &work->cm_event.param.sidr_req_rcvd;
<<<<<<< HEAD
	param->pkey = __be16_to_cpu(sidr_req_msg->pkey);
	param->listen_id = listen_id;
	param->service_id = sidr_req_msg->service_id;
	param->bth_pkey = cm_get_bth_pkey(work);
	param->port = work->port->port_num;
	param->sgid_attr = rx_cm_id->av.ah_attr.grh.sgid_attr;
	work->cm_event.private_data = &sidr_req_msg->private_data;
=======
	param->pkey = IBA_GET(CM_SIDR_REQ_PARTITION_KEY, sidr_req_msg);
	param->listen_id = listen_id;
	param->service_id =
		cpu_to_be64(IBA_GET(CM_SIDR_REQ_SERVICEID, sidr_req_msg));
	param->bth_pkey = cm_get_bth_pkey(work);
	param->port = work->port->port_num;
	param->sgid_attr = rx_cm_id->av.ah_attr.grh.sgid_attr;
	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_SIDR_REQ_PRIVATE_DATA, sidr_req_msg);
>>>>>>> upstream/android-13
}

static int cm_sidr_req_handler(struct cm_work *work)
{
<<<<<<< HEAD
	struct ib_cm_id *cm_id;
	struct cm_id_private *cm_id_priv, *cur_cm_id_priv;
=======
	struct cm_id_private *cm_id_priv, *listen_cm_id_priv;
>>>>>>> upstream/android-13
	struct cm_sidr_req_msg *sidr_req_msg;
	struct ib_wc *wc;
	int ret;

<<<<<<< HEAD
	cm_id = ib_create_cm_id(work->port->cm_dev->ib_device, NULL, NULL);
	if (IS_ERR(cm_id))
		return PTR_ERR(cm_id);
	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
=======
	cm_id_priv =
		cm_alloc_id_priv(work->port->cm_dev->ib_device, NULL, NULL);
	if (IS_ERR(cm_id_priv))
		return PTR_ERR(cm_id_priv);
>>>>>>> upstream/android-13

	/* Record SGID/SLID and request ID for lookup. */
	sidr_req_msg = (struct cm_sidr_req_msg *)
				work->mad_recv_wc->recv_buf.mad;
<<<<<<< HEAD
	wc = work->mad_recv_wc->wc;
	cm_id_priv->av.dgid.global.subnet_prefix = cpu_to_be64(wc->slid);
	cm_id_priv->av.dgid.global.interface_id = 0;
=======

	cm_id_priv->id.remote_id =
		cpu_to_be32(IBA_GET(CM_SIDR_REQ_REQUESTID, sidr_req_msg));
	cm_id_priv->id.service_id =
		cpu_to_be64(IBA_GET(CM_SIDR_REQ_SERVICEID, sidr_req_msg));
	cm_id_priv->id.service_mask = ~cpu_to_be64(0);
	cm_id_priv->tid = sidr_req_msg->hdr.tid;

	wc = work->mad_recv_wc->wc;
	cm_id_priv->sidr_slid = wc->slid;
>>>>>>> upstream/android-13
	ret = cm_init_av_for_response(work->port, work->mad_recv_wc->wc,
				      work->mad_recv_wc->recv_buf.grh,
				      &cm_id_priv->av);
	if (ret)
		goto out;

<<<<<<< HEAD
	cm_id_priv->id.remote_id = sidr_req_msg->request_id;
	cm_id_priv->tid = sidr_req_msg->hdr.tid;
	atomic_inc(&cm_id_priv->work_count);

	spin_lock_irq(&cm.lock);
	cur_cm_id_priv = cm_insert_remote_sidr(cm_id_priv);
	if (cur_cm_id_priv) {
		spin_unlock_irq(&cm.lock);
		atomic_long_inc(&work->port->counter_group[CM_RECV_DUPLICATES].
				counter[CM_SIDR_REQ_COUNTER]);
		goto out; /* Duplicate message. */
	}
	cm_id_priv->id.state = IB_CM_SIDR_REQ_RCVD;
	cur_cm_id_priv = cm_find_listen(cm_id->device,
					sidr_req_msg->service_id);
	if (!cur_cm_id_priv) {
		spin_unlock_irq(&cm.lock);
		cm_reject_sidr_req(cm_id_priv, IB_SIDR_UNSUPPORTED);
		goto out; /* No match. */
	}
	atomic_inc(&cur_cm_id_priv->refcount);
	atomic_inc(&cm_id_priv->refcount);
	spin_unlock_irq(&cm.lock);

	cm_id_priv->id.cm_handler = cur_cm_id_priv->id.cm_handler;
	cm_id_priv->id.context = cur_cm_id_priv->id.context;
	cm_id_priv->id.service_id = sidr_req_msg->service_id;
	cm_id_priv->id.service_mask = ~cpu_to_be64(0);

	cm_format_sidr_req_event(work, cm_id_priv, &cur_cm_id_priv->id);
	cm_process_work(cm_id_priv, work);
	cm_deref_id(cur_cm_id_priv);
=======
	spin_lock_irq(&cm.lock);
	listen_cm_id_priv = cm_insert_remote_sidr(cm_id_priv);
	if (listen_cm_id_priv) {
		spin_unlock_irq(&cm.lock);
		atomic_long_inc(&work->port->counters[CM_RECV_DUPLICATES]
						     [CM_SIDR_REQ_COUNTER]);
		goto out; /* Duplicate message. */
	}
	cm_id_priv->id.state = IB_CM_SIDR_REQ_RCVD;
	listen_cm_id_priv = cm_find_listen(cm_id_priv->id.device,
					   cm_id_priv->id.service_id);
	if (!listen_cm_id_priv) {
		spin_unlock_irq(&cm.lock);
		ib_send_cm_sidr_rep(&cm_id_priv->id,
				    &(struct ib_cm_sidr_rep_param){
					    .status = IB_SIDR_UNSUPPORTED });
		goto out; /* No match. */
	}
	spin_unlock_irq(&cm.lock);

	cm_id_priv->id.cm_handler = listen_cm_id_priv->id.cm_handler;
	cm_id_priv->id.context = listen_cm_id_priv->id.context;

	/*
	 * A SIDR ID does not need to be in the xarray since it does not receive
	 * mads, is not placed in the remote_id or remote_qpn rbtree, and does
	 * not enter timewait.
	 */

	cm_format_sidr_req_event(work, cm_id_priv, &listen_cm_id_priv->id);
	ret = cm_id_priv->id.cm_handler(&cm_id_priv->id, &work->cm_event);
	cm_free_work(work);
	/*
	 * A pointer to the listen_cm_id is held in the event, so this deref
	 * must be after the event is delivered above.
	 */
	cm_deref_id(listen_cm_id_priv);
	if (ret)
		cm_destroy_id(&cm_id_priv->id, ret);
>>>>>>> upstream/android-13
	return 0;
out:
	ib_destroy_cm_id(&cm_id_priv->id);
	return -EINVAL;
}

static void cm_format_sidr_rep(struct cm_sidr_rep_msg *sidr_rep_msg,
			       struct cm_id_private *cm_id_priv,
			       struct ib_cm_sidr_rep_param *param)
{
<<<<<<< HEAD
	cm_format_mad_hdr(&sidr_rep_msg->hdr, CM_SIDR_REP_ATTR_ID,
			  cm_id_priv->tid);
	sidr_rep_msg->request_id = cm_id_priv->id.remote_id;
	sidr_rep_msg->status = param->status;
	cm_sidr_rep_set_qpn(sidr_rep_msg, cpu_to_be32(param->qp_num));
	sidr_rep_msg->service_id = cm_id_priv->id.service_id;
	sidr_rep_msg->qkey = cpu_to_be32(param->qkey);

	if (param->info && param->info_length)
		memcpy(sidr_rep_msg->info, param->info, param->info_length);

	if (param->private_data && param->private_data_len)
		memcpy(sidr_rep_msg->private_data, param->private_data,
		       param->private_data_len);
}

int ib_send_cm_sidr_rep(struct ib_cm_id *cm_id,
			struct ib_cm_sidr_rep_param *param)
{
	struct cm_id_private *cm_id_priv;
=======
	cm_format_mad_ece_hdr(&sidr_rep_msg->hdr, CM_SIDR_REP_ATTR_ID,
			      cm_id_priv->tid, param->ece.attr_mod);
	IBA_SET(CM_SIDR_REP_REQUESTID, sidr_rep_msg,
		be32_to_cpu(cm_id_priv->id.remote_id));
	IBA_SET(CM_SIDR_REP_STATUS, sidr_rep_msg, param->status);
	IBA_SET(CM_SIDR_REP_QPN, sidr_rep_msg, param->qp_num);
	IBA_SET(CM_SIDR_REP_SERVICEID, sidr_rep_msg,
		be64_to_cpu(cm_id_priv->id.service_id));
	IBA_SET(CM_SIDR_REP_Q_KEY, sidr_rep_msg, param->qkey);
	IBA_SET(CM_SIDR_REP_VENDOR_ID_L, sidr_rep_msg,
		param->ece.vendor_id & 0xFF);
	IBA_SET(CM_SIDR_REP_VENDOR_ID_H, sidr_rep_msg,
		(param->ece.vendor_id >> 8) & 0xFF);

	if (param->info && param->info_length)
		IBA_SET_MEM(CM_SIDR_REP_ADDITIONAL_INFORMATION, sidr_rep_msg,
			    param->info, param->info_length);

	if (param->private_data && param->private_data_len)
		IBA_SET_MEM(CM_SIDR_REP_PRIVATE_DATA, sidr_rep_msg,
			    param->private_data, param->private_data_len);
}

static int cm_send_sidr_rep_locked(struct cm_id_private *cm_id_priv,
				   struct ib_cm_sidr_rep_param *param)
{
>>>>>>> upstream/android-13
	struct ib_mad_send_buf *msg;
	unsigned long flags;
	int ret;

<<<<<<< HEAD
=======
	lockdep_assert_held(&cm_id_priv->lock);

>>>>>>> upstream/android-13
	if ((param->info && param->info_length > IB_CM_SIDR_REP_INFO_LENGTH) ||
	    (param->private_data &&
	     param->private_data_len > IB_CM_SIDR_REP_PRIVATE_DATA_SIZE))
		return -EINVAL;

<<<<<<< HEAD
	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state != IB_CM_SIDR_REQ_RCVD) {
		ret = -EINVAL;
		goto error;
	}

	ret = cm_alloc_msg(cm_id_priv, &msg);
	if (ret)
		goto error;

	cm_format_sidr_rep((struct cm_sidr_rep_msg *) msg->mad, cm_id_priv,
			   param);
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		spin_unlock_irqrestore(&cm_id_priv->lock, flags);
		cm_free_msg(msg);
		return ret;
	}
	cm_id->state = IB_CM_IDLE;
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);

=======
	if (cm_id_priv->id.state != IB_CM_SIDR_REQ_RCVD)
		return -EINVAL;

	msg = cm_alloc_msg(cm_id_priv);
	if (IS_ERR(msg))
		return PTR_ERR(msg);

	cm_format_sidr_rep((struct cm_sidr_rep_msg *) msg->mad, cm_id_priv,
			   param);
	trace_icm_send_sidr_rep(&cm_id_priv->id);
	ret = ib_post_send_mad(msg, NULL);
	if (ret) {
		cm_free_msg(msg);
		return ret;
	}
	cm_id_priv->id.state = IB_CM_IDLE;
>>>>>>> upstream/android-13
	spin_lock_irqsave(&cm.lock, flags);
	if (!RB_EMPTY_NODE(&cm_id_priv->sidr_id_node)) {
		rb_erase(&cm_id_priv->sidr_id_node, &cm.remote_sidr_table);
		RB_CLEAR_NODE(&cm_id_priv->sidr_id_node);
	}
	spin_unlock_irqrestore(&cm.lock, flags);
	return 0;
<<<<<<< HEAD

error:	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
=======
}

int ib_send_cm_sidr_rep(struct ib_cm_id *cm_id,
			struct ib_cm_sidr_rep_param *param)
{
	struct cm_id_private *cm_id_priv =
		container_of(cm_id, struct cm_id_private, id);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	ret = cm_send_sidr_rep_locked(cm_id_priv, param);
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ib_send_cm_sidr_rep);

static void cm_format_sidr_rep_event(struct cm_work *work,
				     const struct cm_id_private *cm_id_priv)
{
	struct cm_sidr_rep_msg *sidr_rep_msg;
	struct ib_cm_sidr_rep_event_param *param;

	sidr_rep_msg = (struct cm_sidr_rep_msg *)
				work->mad_recv_wc->recv_buf.mad;
	param = &work->cm_event.param.sidr_rep_rcvd;
<<<<<<< HEAD
	param->status = sidr_rep_msg->status;
	param->qkey = be32_to_cpu(sidr_rep_msg->qkey);
	param->qpn = be32_to_cpu(cm_sidr_rep_get_qpn(sidr_rep_msg));
	param->info = &sidr_rep_msg->info;
	param->info_len = sidr_rep_msg->info_length;
	param->sgid_attr = cm_id_priv->av.ah_attr.grh.sgid_attr;
	work->cm_event.private_data = &sidr_rep_msg->private_data;
=======
	param->status = IBA_GET(CM_SIDR_REP_STATUS, sidr_rep_msg);
	param->qkey = IBA_GET(CM_SIDR_REP_Q_KEY, sidr_rep_msg);
	param->qpn = IBA_GET(CM_SIDR_REP_QPN, sidr_rep_msg);
	param->info = IBA_GET_MEM_PTR(CM_SIDR_REP_ADDITIONAL_INFORMATION,
				      sidr_rep_msg);
	param->info_len = IBA_GET(CM_SIDR_REP_ADDITIONAL_INFORMATION_LENGTH,
				  sidr_rep_msg);
	param->sgid_attr = cm_id_priv->av.ah_attr.grh.sgid_attr;
	work->cm_event.private_data =
		IBA_GET_MEM_PTR(CM_SIDR_REP_PRIVATE_DATA, sidr_rep_msg);
>>>>>>> upstream/android-13
}

static int cm_sidr_rep_handler(struct cm_work *work)
{
	struct cm_sidr_rep_msg *sidr_rep_msg;
	struct cm_id_private *cm_id_priv;

	sidr_rep_msg = (struct cm_sidr_rep_msg *)
				work->mad_recv_wc->recv_buf.mad;
<<<<<<< HEAD
	cm_id_priv = cm_acquire_id(sidr_rep_msg->request_id, 0);
=======
	cm_id_priv = cm_acquire_id(
		cpu_to_be32(IBA_GET(CM_SIDR_REP_REQUESTID, sidr_rep_msg)), 0);
>>>>>>> upstream/android-13
	if (!cm_id_priv)
		return -EINVAL; /* Unmatched reply. */

	spin_lock_irq(&cm_id_priv->lock);
	if (cm_id_priv->id.state != IB_CM_SIDR_REQ_SENT) {
		spin_unlock_irq(&cm_id_priv->lock);
		goto out;
	}
	cm_id_priv->id.state = IB_CM_IDLE;
<<<<<<< HEAD
	ib_cancel_mad(cm_id_priv->av.port->mad_agent, cm_id_priv->msg);
=======
	ib_cancel_mad(cm_id_priv->msg);
>>>>>>> upstream/android-13
	spin_unlock_irq(&cm_id_priv->lock);

	cm_format_sidr_rep_event(work, cm_id_priv);
	cm_process_work(cm_id_priv, work);
	return 0;
out:
	cm_deref_id(cm_id_priv);
	return -EINVAL;
}

<<<<<<< HEAD
static void cm_process_send_error(struct ib_mad_send_buf *msg,
				  enum ib_wc_status wc_status)
{
	struct cm_id_private *cm_id_priv;
	struct ib_cm_event cm_event;
	enum ib_cm_state state;
	int ret;

	memset(&cm_event, 0, sizeof cm_event);
	cm_id_priv = msg->context[0];

	/* Discard old sends or ones without a response. */
	spin_lock_irq(&cm_id_priv->lock);
	state = (enum ib_cm_state) (unsigned long) msg->context[1];
	if (msg != cm_id_priv->msg || state != cm_id_priv->id.state)
		goto discard;

	pr_debug_ratelimited("CM: failed sending MAD in state %d. (%s)\n",
			     state, ib_wc_status_msg(wc_status));
=======
static void cm_process_send_error(struct cm_id_private *cm_id_priv,
				  struct ib_mad_send_buf *msg,
				  enum ib_cm_state state,
				  enum ib_wc_status wc_status)
{
	struct ib_cm_event cm_event = {};
	int ret;

	/* Discard old sends or ones without a response. */
	spin_lock_irq(&cm_id_priv->lock);
	if (msg != cm_id_priv->msg) {
		spin_unlock_irq(&cm_id_priv->lock);
		cm_free_msg(msg);
		return;
	}
	cm_free_priv_msg(msg);

	if (state != cm_id_priv->id.state || wc_status == IB_WC_SUCCESS ||
	    wc_status == IB_WC_WR_FLUSH_ERR)
		goto out_unlock;

	trace_icm_mad_send_err(state, wc_status);
>>>>>>> upstream/android-13
	switch (state) {
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
		cm_reset_to_idle(cm_id_priv);
		cm_event.event = IB_CM_REQ_ERROR;
		break;
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
		cm_reset_to_idle(cm_id_priv);
		cm_event.event = IB_CM_REP_ERROR;
		break;
	case IB_CM_DREQ_SENT:
		cm_enter_timewait(cm_id_priv);
		cm_event.event = IB_CM_DREQ_ERROR;
		break;
	case IB_CM_SIDR_REQ_SENT:
		cm_id_priv->id.state = IB_CM_IDLE;
		cm_event.event = IB_CM_SIDR_REQ_ERROR;
		break;
	default:
<<<<<<< HEAD
		goto discard;
=======
		goto out_unlock;
>>>>>>> upstream/android-13
	}
	spin_unlock_irq(&cm_id_priv->lock);
	cm_event.param.send_status = wc_status;

	/* No other events can occur on the cm_id at this point. */
	ret = cm_id_priv->id.cm_handler(&cm_id_priv->id, &cm_event);
<<<<<<< HEAD
	cm_free_msg(msg);
	if (ret)
		ib_destroy_cm_id(&cm_id_priv->id);
	return;
discard:
	spin_unlock_irq(&cm_id_priv->lock);
	cm_free_msg(msg);
=======
	if (ret)
		ib_destroy_cm_id(&cm_id_priv->id);
	return;
out_unlock:
	spin_unlock_irq(&cm_id_priv->lock);
>>>>>>> upstream/android-13
}

static void cm_send_handler(struct ib_mad_agent *mad_agent,
			    struct ib_mad_send_wc *mad_send_wc)
{
	struct ib_mad_send_buf *msg = mad_send_wc->send_buf;
<<<<<<< HEAD
=======
	struct cm_id_private *cm_id_priv = msg->context[0];
	enum ib_cm_state state =
		(enum ib_cm_state)(unsigned long)msg->context[1];
>>>>>>> upstream/android-13
	struct cm_port *port;
	u16 attr_index;

	port = mad_agent->context;
	attr_index = be16_to_cpu(((struct ib_mad_hdr *)
				  msg->mad)->attr_id) - CM_ATTR_ID_OFFSET;

	/*
	 * If the send was in response to a received message (context[0] is not
	 * set to a cm_id), and is not a REJ, then it is a send that was
	 * manually retried.
	 */
<<<<<<< HEAD
	if (!msg->context[0] && (attr_index != CM_REJ_COUNTER))
		msg->retries = 1;

	atomic_long_add(1 + msg->retries,
			&port->counter_group[CM_XMIT].counter[attr_index]);
	if (msg->retries)
		atomic_long_add(msg->retries,
				&port->counter_group[CM_XMIT_RETRIES].
				counter[attr_index]);

	switch (mad_send_wc->status) {
	case IB_WC_SUCCESS:
	case IB_WC_WR_FLUSH_ERR:
		cm_free_msg(msg);
		break;
	default:
		if (msg->context[0] && msg->context[1])
			cm_process_send_error(msg, mad_send_wc->status);
		else
			cm_free_msg(msg);
		break;
	}
=======
	if (!cm_id_priv && (attr_index != CM_REJ_COUNTER))
		msg->retries = 1;

	atomic_long_add(1 + msg->retries, &port->counters[CM_XMIT][attr_index]);
	if (msg->retries)
		atomic_long_add(msg->retries,
				&port->counters[CM_XMIT_RETRIES][attr_index]);

	if (cm_id_priv)
		cm_process_send_error(cm_id_priv, msg, state,
				      mad_send_wc->status);
	else
		cm_free_response_msg(msg);
>>>>>>> upstream/android-13
}

static void cm_work_handler(struct work_struct *_work)
{
	struct cm_work *work = container_of(_work, struct cm_work, work.work);
	int ret;

	switch (work->cm_event.event) {
	case IB_CM_REQ_RECEIVED:
		ret = cm_req_handler(work);
		break;
	case IB_CM_MRA_RECEIVED:
		ret = cm_mra_handler(work);
		break;
	case IB_CM_REJ_RECEIVED:
		ret = cm_rej_handler(work);
		break;
	case IB_CM_REP_RECEIVED:
		ret = cm_rep_handler(work);
		break;
	case IB_CM_RTU_RECEIVED:
		ret = cm_rtu_handler(work);
		break;
	case IB_CM_USER_ESTABLISHED:
		ret = cm_establish_handler(work);
		break;
	case IB_CM_DREQ_RECEIVED:
		ret = cm_dreq_handler(work);
		break;
	case IB_CM_DREP_RECEIVED:
		ret = cm_drep_handler(work);
		break;
	case IB_CM_SIDR_REQ_RECEIVED:
		ret = cm_sidr_req_handler(work);
		break;
	case IB_CM_SIDR_REP_RECEIVED:
		ret = cm_sidr_rep_handler(work);
		break;
	case IB_CM_LAP_RECEIVED:
		ret = cm_lap_handler(work);
		break;
	case IB_CM_APR_RECEIVED:
		ret = cm_apr_handler(work);
		break;
	case IB_CM_TIMEWAIT_EXIT:
		ret = cm_timewait_handler(work);
		break;
	default:
<<<<<<< HEAD
		pr_debug("cm_event.event: 0x%x\n", work->cm_event.event);
=======
		trace_icm_handler_err(work->cm_event.event);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		break;
	}
	if (ret)
		cm_free_work(work);
}

static int cm_establish(struct ib_cm_id *cm_id)
{
	struct cm_id_private *cm_id_priv;
	struct cm_work *work;
	unsigned long flags;
	int ret = 0;
	struct cm_device *cm_dev;

	cm_dev = ib_get_client_data(cm_id->device, &cm_client);
	if (!cm_dev)
		return -ENODEV;

	work = kmalloc(sizeof *work, GFP_ATOMIC);
	if (!work)
		return -ENOMEM;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
<<<<<<< HEAD
	switch (cm_id->state)
	{
=======
	switch (cm_id->state) {
>>>>>>> upstream/android-13
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
		cm_id->state = IB_CM_ESTABLISHED;
		break;
	case IB_CM_ESTABLISHED:
		ret = -EISCONN;
		break;
	default:
<<<<<<< HEAD
		pr_debug("%s: local_id %d, cm_id->state: %d\n", __func__,
			 be32_to_cpu(cm_id->local_id), cm_id->state);
=======
		trace_icm_establish_err(cm_id);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		break;
	}
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);

	if (ret) {
		kfree(work);
		goto out;
	}

	/*
	 * The CM worker thread may try to destroy the cm_id before it
	 * can execute this work item.  To prevent potential deadlock,
	 * we need to find the cm_id once we're in the context of the
	 * worker thread, rather than holding a reference on it.
	 */
	INIT_DELAYED_WORK(&work->work, cm_work_handler);
	work->local_id = cm_id->local_id;
	work->remote_id = cm_id->remote_id;
	work->mad_recv_wc = NULL;
	work->cm_event.event = IB_CM_USER_ESTABLISHED;

	/* Check if the device started its remove_one */
	spin_lock_irqsave(&cm.lock, flags);
	if (!cm_dev->going_down) {
		queue_delayed_work(cm.wq, &work->work, 0);
	} else {
		kfree(work);
		ret = -ENODEV;
	}
	spin_unlock_irqrestore(&cm.lock, flags);

out:
	return ret;
}

static int cm_migrate(struct ib_cm_id *cm_id)
{
	struct cm_id_private *cm_id_priv;
<<<<<<< HEAD
	struct cm_av tmp_av;
	unsigned long flags;
	int tmp_send_port_not_ready;
=======
	unsigned long flags;
>>>>>>> upstream/android-13
	int ret = 0;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	spin_lock_irqsave(&cm_id_priv->lock, flags);
	if (cm_id->state == IB_CM_ESTABLISHED &&
	    (cm_id->lap_state == IB_CM_LAP_UNINIT ||
	     cm_id->lap_state == IB_CM_LAP_IDLE)) {
		cm_id->lap_state = IB_CM_LAP_IDLE;
<<<<<<< HEAD
		/* Swap address vector */
		tmp_av = cm_id_priv->av;
		cm_id_priv->av = cm_id_priv->alt_av;
		cm_id_priv->alt_av = tmp_av;
		/* Swap port send ready state */
		tmp_send_port_not_ready = cm_id_priv->prim_send_port_not_ready;
		cm_id_priv->prim_send_port_not_ready = cm_id_priv->altr_send_port_not_ready;
		cm_id_priv->altr_send_port_not_ready = tmp_send_port_not_ready;
=======
		cm_id_priv->av = cm_id_priv->alt_av;
>>>>>>> upstream/android-13
	} else
		ret = -EINVAL;
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);

	return ret;
}

int ib_cm_notify(struct ib_cm_id *cm_id, enum ib_event_type event)
{
	int ret;

	switch (event) {
	case IB_EVENT_COMM_EST:
		ret = cm_establish(cm_id);
		break;
	case IB_EVENT_PATH_MIG:
		ret = cm_migrate(cm_id);
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}
EXPORT_SYMBOL(ib_cm_notify);

static void cm_recv_handler(struct ib_mad_agent *mad_agent,
			    struct ib_mad_send_buf *send_buf,
			    struct ib_mad_recv_wc *mad_recv_wc)
{
	struct cm_port *port = mad_agent->context;
	struct cm_work *work;
	enum ib_cm_event_type event;
	bool alt_path = false;
	u16 attr_id;
	int paths = 0;
	int going_down = 0;

	switch (mad_recv_wc->recv_buf.mad->mad_hdr.attr_id) {
	case CM_REQ_ATTR_ID:
		alt_path = cm_req_has_alt_path((struct cm_req_msg *)
						mad_recv_wc->recv_buf.mad);
		paths = 1 + (alt_path != 0);
		event = IB_CM_REQ_RECEIVED;
		break;
	case CM_MRA_ATTR_ID:
		event = IB_CM_MRA_RECEIVED;
		break;
	case CM_REJ_ATTR_ID:
		event = IB_CM_REJ_RECEIVED;
		break;
	case CM_REP_ATTR_ID:
		event = IB_CM_REP_RECEIVED;
		break;
	case CM_RTU_ATTR_ID:
		event = IB_CM_RTU_RECEIVED;
		break;
	case CM_DREQ_ATTR_ID:
		event = IB_CM_DREQ_RECEIVED;
		break;
	case CM_DREP_ATTR_ID:
		event = IB_CM_DREP_RECEIVED;
		break;
	case CM_SIDR_REQ_ATTR_ID:
		event = IB_CM_SIDR_REQ_RECEIVED;
		break;
	case CM_SIDR_REP_ATTR_ID:
		event = IB_CM_SIDR_REP_RECEIVED;
		break;
	case CM_LAP_ATTR_ID:
		paths = 1;
		event = IB_CM_LAP_RECEIVED;
		break;
	case CM_APR_ATTR_ID:
		event = IB_CM_APR_RECEIVED;
		break;
	default:
		ib_free_recv_mad(mad_recv_wc);
		return;
	}

	attr_id = be16_to_cpu(mad_recv_wc->recv_buf.mad->mad_hdr.attr_id);
<<<<<<< HEAD
	atomic_long_inc(&port->counter_group[CM_RECV].
			counter[attr_id - CM_ATTR_ID_OFFSET]);

	work = kmalloc(sizeof(*work) + sizeof(struct sa_path_rec) * paths,
		       GFP_KERNEL);
=======
	atomic_long_inc(&port->counters[CM_RECV][attr_id - CM_ATTR_ID_OFFSET]);

	work = kmalloc(struct_size(work, path, paths), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!work) {
		ib_free_recv_mad(mad_recv_wc);
		return;
	}

	INIT_DELAYED_WORK(&work->work, cm_work_handler);
	work->cm_event.event = event;
	work->mad_recv_wc = mad_recv_wc;
	work->port = port;

	/* Check if the device started its remove_one */
	spin_lock_irq(&cm.lock);
	if (!port->cm_dev->going_down)
		queue_delayed_work(cm.wq, &work->work, 0);
	else
		going_down = 1;
	spin_unlock_irq(&cm.lock);

	if (going_down) {
		kfree(work);
		ib_free_recv_mad(mad_recv_wc);
	}
}

static int cm_init_qp_init_attr(struct cm_id_private *cm_id_priv,
				struct ib_qp_attr *qp_attr,
				int *qp_attr_mask)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	switch (cm_id_priv->id.state) {
	case IB_CM_REQ_SENT:
	case IB_CM_MRA_REQ_RCVD:
	case IB_CM_REQ_RCVD:
	case IB_CM_MRA_REQ_SENT:
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
	case IB_CM_ESTABLISHED:
		*qp_attr_mask = IB_QP_STATE | IB_QP_ACCESS_FLAGS |
				IB_QP_PKEY_INDEX | IB_QP_PORT;
		qp_attr->qp_access_flags = IB_ACCESS_REMOTE_WRITE;
		if (cm_id_priv->responder_resources)
			qp_attr->qp_access_flags |= IB_ACCESS_REMOTE_READ |
						    IB_ACCESS_REMOTE_ATOMIC;
		qp_attr->pkey_index = cm_id_priv->av.pkey_index;
<<<<<<< HEAD
		qp_attr->port_num = cm_id_priv->av.port->port_num;
		ret = 0;
		break;
	default:
		pr_debug("%s: local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
=======
		if (cm_id_priv->av.port)
			qp_attr->port_num = cm_id_priv->av.port->port_num;
		ret = 0;
		break;
	default:
		trace_icm_qp_init_err(&cm_id_priv->id);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		break;
	}
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return ret;
}

static int cm_init_qp_rtr_attr(struct cm_id_private *cm_id_priv,
			       struct ib_qp_attr *qp_attr,
			       int *qp_attr_mask)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	switch (cm_id_priv->id.state) {
	case IB_CM_REQ_RCVD:
	case IB_CM_MRA_REQ_SENT:
	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
	case IB_CM_ESTABLISHED:
		*qp_attr_mask = IB_QP_STATE | IB_QP_AV | IB_QP_PATH_MTU |
				IB_QP_DEST_QPN | IB_QP_RQ_PSN;
		qp_attr->ah_attr = cm_id_priv->av.ah_attr;
		qp_attr->path_mtu = cm_id_priv->path_mtu;
		qp_attr->dest_qp_num = be32_to_cpu(cm_id_priv->remote_qpn);
		qp_attr->rq_psn = be32_to_cpu(cm_id_priv->rq_psn);
		if (cm_id_priv->qp_type == IB_QPT_RC ||
		    cm_id_priv->qp_type == IB_QPT_XRC_TGT) {
			*qp_attr_mask |= IB_QP_MAX_DEST_RD_ATOMIC |
					 IB_QP_MIN_RNR_TIMER;
			qp_attr->max_dest_rd_atomic =
					cm_id_priv->responder_resources;
			qp_attr->min_rnr_timer = 0;
		}
<<<<<<< HEAD
		if (rdma_ah_get_dlid(&cm_id_priv->alt_av.ah_attr)) {
=======
		if (rdma_ah_get_dlid(&cm_id_priv->alt_av.ah_attr) &&
		    cm_id_priv->alt_av.port) {
>>>>>>> upstream/android-13
			*qp_attr_mask |= IB_QP_ALT_PATH;
			qp_attr->alt_port_num = cm_id_priv->alt_av.port->port_num;
			qp_attr->alt_pkey_index = cm_id_priv->alt_av.pkey_index;
			qp_attr->alt_timeout = cm_id_priv->alt_av.timeout;
			qp_attr->alt_ah_attr = cm_id_priv->alt_av.ah_attr;
		}
		ret = 0;
		break;
	default:
<<<<<<< HEAD
		pr_debug("%s: local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
=======
		trace_icm_qp_rtr_err(&cm_id_priv->id);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		break;
	}
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return ret;
}

static int cm_init_qp_rts_attr(struct cm_id_private *cm_id_priv,
			       struct ib_qp_attr *qp_attr,
			       int *qp_attr_mask)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cm_id_priv->lock, flags);
	switch (cm_id_priv->id.state) {
	/* Allow transition to RTS before sending REP */
	case IB_CM_REQ_RCVD:
	case IB_CM_MRA_REQ_SENT:

	case IB_CM_REP_RCVD:
	case IB_CM_MRA_REP_SENT:
	case IB_CM_REP_SENT:
	case IB_CM_MRA_REP_RCVD:
	case IB_CM_ESTABLISHED:
		if (cm_id_priv->id.lap_state == IB_CM_LAP_UNINIT) {
			*qp_attr_mask = IB_QP_STATE | IB_QP_SQ_PSN;
			qp_attr->sq_psn = be32_to_cpu(cm_id_priv->sq_psn);
			switch (cm_id_priv->qp_type) {
			case IB_QPT_RC:
			case IB_QPT_XRC_INI:
				*qp_attr_mask |= IB_QP_RETRY_CNT | IB_QP_RNR_RETRY |
						 IB_QP_MAX_QP_RD_ATOMIC;
				qp_attr->retry_cnt = cm_id_priv->retry_count;
				qp_attr->rnr_retry = cm_id_priv->rnr_retry_count;
				qp_attr->max_rd_atomic = cm_id_priv->initiator_depth;
<<<<<<< HEAD
				/* fall through */
=======
				fallthrough;
>>>>>>> upstream/android-13
			case IB_QPT_XRC_TGT:
				*qp_attr_mask |= IB_QP_TIMEOUT;
				qp_attr->timeout = cm_id_priv->av.timeout;
				break;
			default:
				break;
			}
			if (rdma_ah_get_dlid(&cm_id_priv->alt_av.ah_attr)) {
				*qp_attr_mask |= IB_QP_PATH_MIG_STATE;
				qp_attr->path_mig_state = IB_MIG_REARM;
			}
		} else {
			*qp_attr_mask = IB_QP_ALT_PATH | IB_QP_PATH_MIG_STATE;
<<<<<<< HEAD
			qp_attr->alt_port_num = cm_id_priv->alt_av.port->port_num;
=======
			if (cm_id_priv->alt_av.port)
				qp_attr->alt_port_num =
					cm_id_priv->alt_av.port->port_num;
>>>>>>> upstream/android-13
			qp_attr->alt_pkey_index = cm_id_priv->alt_av.pkey_index;
			qp_attr->alt_timeout = cm_id_priv->alt_av.timeout;
			qp_attr->alt_ah_attr = cm_id_priv->alt_av.ah_attr;
			qp_attr->path_mig_state = IB_MIG_REARM;
		}
		ret = 0;
		break;
	default:
<<<<<<< HEAD
		pr_debug("%s: local_id %d, cm_id_priv->id.state: %d\n",
			 __func__, be32_to_cpu(cm_id_priv->id.local_id),
			 cm_id_priv->id.state);
=======
		trace_icm_qp_rts_err(&cm_id_priv->id);
>>>>>>> upstream/android-13
		ret = -EINVAL;
		break;
	}
	spin_unlock_irqrestore(&cm_id_priv->lock, flags);
	return ret;
}

int ib_cm_init_qp_attr(struct ib_cm_id *cm_id,
		       struct ib_qp_attr *qp_attr,
		       int *qp_attr_mask)
{
	struct cm_id_private *cm_id_priv;
	int ret;

	cm_id_priv = container_of(cm_id, struct cm_id_private, id);
	switch (qp_attr->qp_state) {
	case IB_QPS_INIT:
		ret = cm_init_qp_init_attr(cm_id_priv, qp_attr, qp_attr_mask);
		break;
	case IB_QPS_RTR:
		ret = cm_init_qp_rtr_attr(cm_id_priv, qp_attr, qp_attr_mask);
		break;
	case IB_QPS_RTS:
		ret = cm_init_qp_rts_attr(cm_id_priv, qp_attr, qp_attr_mask);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}
EXPORT_SYMBOL(ib_cm_init_qp_attr);

<<<<<<< HEAD
static ssize_t cm_show_counter(struct kobject *obj, struct attribute *attr,
			       char *buf)
{
	struct cm_counter_group *group;
	struct cm_counter_attribute *cm_attr;

	group = container_of(obj, struct cm_counter_group, obj);
	cm_attr = container_of(attr, struct cm_counter_attribute, attr);

	return sprintf(buf, "%ld\n",
		       atomic_long_read(&group->counter[cm_attr->index]));
}

static const struct sysfs_ops cm_counter_ops = {
	.show = cm_show_counter
};

static struct kobj_type cm_counter_obj_type = {
	.sysfs_ops = &cm_counter_ops,
	.default_attrs = cm_counter_default_attrs
};

static void cm_release_port_obj(struct kobject *obj)
{
	struct cm_port *cm_port;

	cm_port = container_of(obj, struct cm_port, port_obj);
	kfree(cm_port);
}

static struct kobj_type cm_port_obj_type = {
	.release = cm_release_port_obj
};

static char *cm_devnode(struct device *dev, umode_t *mode)
{
	if (mode)
		*mode = 0666;
	return kasprintf(GFP_KERNEL, "infiniband/%s", dev_name(dev));
}

struct class cm_class = {
	.owner   = THIS_MODULE,
	.name    = "infiniband_cm",
	.devnode = cm_devnode,
};
EXPORT_SYMBOL(cm_class);

static int cm_create_port_fs(struct cm_port *port)
{
	int i, ret;

	ret = kobject_init_and_add(&port->port_obj, &cm_port_obj_type,
				   &port->cm_dev->device->kobj,
				   "%d", port->port_num);
	if (ret) {
		kfree(port);
		return ret;
	}

	for (i = 0; i < CM_COUNTER_GROUPS; i++) {
		ret = kobject_init_and_add(&port->counter_group[i].obj,
					   &cm_counter_obj_type,
					   &port->port_obj,
					   "%s", counter_group_names[i]);
		if (ret)
			goto error;
	}

	return 0;

error:
	while (i--)
		kobject_put(&port->counter_group[i].obj);
	kobject_put(&port->port_obj);
	return ret;

}

static void cm_remove_port_fs(struct cm_port *port)
{
	int i;

	for (i = 0; i < CM_COUNTER_GROUPS; i++)
		kobject_put(&port->counter_group[i].obj);

	kobject_put(&port->port_obj);
}

static void cm_add_one(struct ib_device *ib_device)
=======
static ssize_t cm_show_counter(struct ib_device *ibdev, u32 port_num,
			       struct ib_port_attribute *attr, char *buf)
{
	struct cm_counter_attribute *cm_attr =
		container_of(attr, struct cm_counter_attribute, attr);
	struct cm_device *cm_dev = ib_get_client_data(ibdev, &cm_client);

	if (WARN_ON(!cm_dev))
		return -EINVAL;

	return sysfs_emit(
		buf, "%ld\n",
		atomic_long_read(
			&cm_dev->port[port_num - 1]
				 ->counters[cm_attr->group][cm_attr->index]));
}

#define CM_COUNTER_ATTR(_name, _group, _index)                                 \
	{                                                                      \
		.attr = __ATTR(_name, 0444, cm_show_counter, NULL),            \
		.group = _group, .index = _index                               \
	}

#define CM_COUNTER_GROUP(_group, _name)                                        \
	static struct cm_counter_attribute cm_counter_attr_##_group[] = {      \
		CM_COUNTER_ATTR(req, _group, CM_REQ_COUNTER),                  \
		CM_COUNTER_ATTR(mra, _group, CM_MRA_COUNTER),                  \
		CM_COUNTER_ATTR(rej, _group, CM_REJ_COUNTER),                  \
		CM_COUNTER_ATTR(rep, _group, CM_REP_COUNTER),                  \
		CM_COUNTER_ATTR(rtu, _group, CM_RTU_COUNTER),                  \
		CM_COUNTER_ATTR(dreq, _group, CM_DREQ_COUNTER),                \
		CM_COUNTER_ATTR(drep, _group, CM_DREP_COUNTER),                \
		CM_COUNTER_ATTR(sidr_req, _group, CM_SIDR_REQ_COUNTER),        \
		CM_COUNTER_ATTR(sidr_rep, _group, CM_SIDR_REP_COUNTER),        \
		CM_COUNTER_ATTR(lap, _group, CM_LAP_COUNTER),                  \
		CM_COUNTER_ATTR(apr, _group, CM_APR_COUNTER),                  \
	};                                                                     \
	static struct attribute *cm_counter_attrs_##_group[] = {               \
		&cm_counter_attr_##_group[0].attr.attr,                        \
		&cm_counter_attr_##_group[1].attr.attr,                        \
		&cm_counter_attr_##_group[2].attr.attr,                        \
		&cm_counter_attr_##_group[3].attr.attr,                        \
		&cm_counter_attr_##_group[4].attr.attr,                        \
		&cm_counter_attr_##_group[5].attr.attr,                        \
		&cm_counter_attr_##_group[6].attr.attr,                        \
		&cm_counter_attr_##_group[7].attr.attr,                        \
		&cm_counter_attr_##_group[8].attr.attr,                        \
		&cm_counter_attr_##_group[9].attr.attr,                        \
		&cm_counter_attr_##_group[10].attr.attr,                       \
		NULL,                                                          \
	};                                                                     \
	static const struct attribute_group cm_counter_group_##_group = {      \
		.name = _name,                                                 \
		.attrs = cm_counter_attrs_##_group,                            \
	};

CM_COUNTER_GROUP(CM_XMIT, "cm_tx_msgs")
CM_COUNTER_GROUP(CM_XMIT_RETRIES, "cm_tx_retries")
CM_COUNTER_GROUP(CM_RECV, "cm_rx_msgs")
CM_COUNTER_GROUP(CM_RECV_DUPLICATES, "cm_rx_duplicates")

static const struct attribute_group *cm_counter_groups[] = {
	&cm_counter_group_CM_XMIT,
	&cm_counter_group_CM_XMIT_RETRIES,
	&cm_counter_group_CM_RECV,
	&cm_counter_group_CM_RECV_DUPLICATES,
	NULL,
};

static int cm_add_one(struct ib_device *ib_device)
>>>>>>> upstream/android-13
{
	struct cm_device *cm_dev;
	struct cm_port *port;
	struct ib_mad_reg_req reg_req = {
		.mgmt_class = IB_MGMT_CLASS_CM,
		.mgmt_class_version = IB_CM_CLASS_VERSION,
	};
	struct ib_port_modify port_modify = {
		.set_port_cap_mask = IB_PORT_CM_SUP
	};
	unsigned long flags;
	int ret;
	int count = 0;
<<<<<<< HEAD
	u8 i;
=======
	u32 i;
>>>>>>> upstream/android-13

	cm_dev = kzalloc(struct_size(cm_dev, port, ib_device->phys_port_cnt),
			 GFP_KERNEL);
	if (!cm_dev)
<<<<<<< HEAD
		return;

	cm_dev->ib_device = ib_device;
	cm_dev->ack_delay = ib_device->attrs.local_ca_ack_delay;
	cm_dev->going_down = 0;
	cm_dev->device = device_create(&cm_class, &ib_device->dev,
				       MKDEV(0, 0), NULL,
				       "%s", ib_device->name);
	if (IS_ERR(cm_dev->device)) {
		kfree(cm_dev);
		return;
	}

	set_bit(IB_MGMT_METHOD_SEND, reg_req.method_mask);
	for (i = 1; i <= ib_device->phys_port_cnt; i++) {
=======
		return -ENOMEM;

	kref_init(&cm_dev->kref);
	spin_lock_init(&cm_dev->mad_agent_lock);
	cm_dev->ib_device = ib_device;
	cm_dev->ack_delay = ib_device->attrs.local_ca_ack_delay;
	cm_dev->going_down = 0;

	ib_set_client_data(ib_device, &cm_client, cm_dev);

	set_bit(IB_MGMT_METHOD_SEND, reg_req.method_mask);
	rdma_for_each_port (ib_device, i) {
>>>>>>> upstream/android-13
		if (!rdma_cap_ib_cm(ib_device, i))
			continue;

		port = kzalloc(sizeof *port, GFP_KERNEL);
<<<<<<< HEAD
		if (!port)
			goto error1;
=======
		if (!port) {
			ret = -ENOMEM;
			goto error1;
		}
>>>>>>> upstream/android-13

		cm_dev->port[i-1] = port;
		port->cm_dev = cm_dev;
		port->port_num = i;

<<<<<<< HEAD
		INIT_LIST_HEAD(&port->cm_priv_prim_list);
		INIT_LIST_HEAD(&port->cm_priv_altr_list);

		ret = cm_create_port_fs(port);
=======
		ret = ib_port_register_client_groups(ib_device, i,
						     cm_counter_groups);
>>>>>>> upstream/android-13
		if (ret)
			goto error1;

		port->mad_agent = ib_register_mad_agent(ib_device, i,
							IB_QPT_GSI,
							&reg_req,
							0,
							cm_send_handler,
							cm_recv_handler,
							port,
							0);
<<<<<<< HEAD
		if (IS_ERR(port->mad_agent))
			goto error2;
=======
		if (IS_ERR(port->mad_agent)) {
			ret = PTR_ERR(port->mad_agent);
			goto error2;
		}
>>>>>>> upstream/android-13

		ret = ib_modify_port(ib_device, i, 0, &port_modify);
		if (ret)
			goto error3;

		count++;
	}

<<<<<<< HEAD
	if (!count)
		goto free;

	ib_set_client_data(ib_device, &cm_client, cm_dev);
=======
	if (!count) {
		ret = -EOPNOTSUPP;
		goto free;
	}
>>>>>>> upstream/android-13

	write_lock_irqsave(&cm.device_lock, flags);
	list_add_tail(&cm_dev->list, &cm.device_list);
	write_unlock_irqrestore(&cm.device_lock, flags);
<<<<<<< HEAD
	return;
=======
	return 0;
>>>>>>> upstream/android-13

error3:
	ib_unregister_mad_agent(port->mad_agent);
error2:
<<<<<<< HEAD
	cm_remove_port_fs(port);
=======
	ib_port_unregister_client_groups(ib_device, i, cm_counter_groups);
>>>>>>> upstream/android-13
error1:
	port_modify.set_port_cap_mask = 0;
	port_modify.clr_port_cap_mask = IB_PORT_CM_SUP;
	while (--i) {
		if (!rdma_cap_ib_cm(ib_device, i))
			continue;

		port = cm_dev->port[i-1];
		ib_modify_port(ib_device, port->port_num, 0, &port_modify);
		ib_unregister_mad_agent(port->mad_agent);
<<<<<<< HEAD
		cm_remove_port_fs(port);
	}
free:
	device_unregister(cm_dev->device);
	kfree(cm_dev);
=======
		ib_port_unregister_client_groups(ib_device, i,
						 cm_counter_groups);
	}
free:
	cm_device_put(cm_dev);
	return ret;
>>>>>>> upstream/android-13
}

static void cm_remove_one(struct ib_device *ib_device, void *client_data)
{
	struct cm_device *cm_dev = client_data;
	struct cm_port *port;
<<<<<<< HEAD
	struct cm_id_private *cm_id_priv;
	struct ib_mad_agent *cur_mad_agent;
=======
>>>>>>> upstream/android-13
	struct ib_port_modify port_modify = {
		.clr_port_cap_mask = IB_PORT_CM_SUP
	};
	unsigned long flags;
<<<<<<< HEAD
	int i;

	if (!cm_dev)
		return;
=======
	u32 i;
>>>>>>> upstream/android-13

	write_lock_irqsave(&cm.device_lock, flags);
	list_del(&cm_dev->list);
	write_unlock_irqrestore(&cm.device_lock, flags);

	spin_lock_irq(&cm.lock);
	cm_dev->going_down = 1;
	spin_unlock_irq(&cm.lock);

<<<<<<< HEAD
	for (i = 1; i <= ib_device->phys_port_cnt; i++) {
=======
	rdma_for_each_port (ib_device, i) {
		struct ib_mad_agent *mad_agent;

>>>>>>> upstream/android-13
		if (!rdma_cap_ib_cm(ib_device, i))
			continue;

		port = cm_dev->port[i-1];
<<<<<<< HEAD
		ib_modify_port(ib_device, port->port_num, 0, &port_modify);
		/* Mark all the cm_id's as not valid */
		spin_lock_irq(&cm.lock);
		list_for_each_entry(cm_id_priv, &port->cm_priv_altr_list, altr_list)
			cm_id_priv->altr_send_port_not_ready = 1;
		list_for_each_entry(cm_id_priv, &port->cm_priv_prim_list, prim_list)
			cm_id_priv->prim_send_port_not_ready = 1;
		spin_unlock_irq(&cm.lock);
=======
		mad_agent = port->mad_agent;
		ib_modify_port(ib_device, port->port_num, 0, &port_modify);
>>>>>>> upstream/android-13
		/*
		 * We flush the queue here after the going_down set, this
		 * verify that no new works will be queued in the recv handler,
		 * after that we can call the unregister_mad_agent
		 */
		flush_workqueue(cm.wq);
<<<<<<< HEAD
		spin_lock_irq(&cm.state_lock);
		cur_mad_agent = port->mad_agent;
		port->mad_agent = NULL;
		spin_unlock_irq(&cm.state_lock);
		ib_unregister_mad_agent(cur_mad_agent);
		cm_remove_port_fs(port);
	}

	device_unregister(cm_dev->device);
	kfree(cm_dev);
=======
		/*
		 * The above ensures no call paths from the work are running,
		 * the remaining paths all take the mad_agent_lock.
		 */
		spin_lock(&cm_dev->mad_agent_lock);
		port->mad_agent = NULL;
		spin_unlock(&cm_dev->mad_agent_lock);
		ib_unregister_mad_agent(mad_agent);
		ib_port_unregister_client_groups(ib_device, i,
						 cm_counter_groups);
	}

	cm_device_put(cm_dev);
>>>>>>> upstream/android-13
}

static int __init ib_cm_init(void)
{
	int ret;

<<<<<<< HEAD
	memset(&cm, 0, sizeof cm);
	INIT_LIST_HEAD(&cm.device_list);
	rwlock_init(&cm.device_lock);
	spin_lock_init(&cm.lock);
	spin_lock_init(&cm.state_lock);
=======
	INIT_LIST_HEAD(&cm.device_list);
	rwlock_init(&cm.device_lock);
	spin_lock_init(&cm.lock);
>>>>>>> upstream/android-13
	cm.listen_service_table = RB_ROOT;
	cm.listen_service_id = be64_to_cpu(IB_CM_ASSIGN_SERVICE_ID);
	cm.remote_id_table = RB_ROOT;
	cm.remote_qp_table = RB_ROOT;
	cm.remote_sidr_table = RB_ROOT;
<<<<<<< HEAD
	idr_init(&cm.local_id_table);
	get_random_bytes(&cm.random_id_operand, sizeof cm.random_id_operand);
	INIT_LIST_HEAD(&cm.timewait_list);

	ret = class_register(&cm_class);
	if (ret) {
		ret = -ENOMEM;
		goto error1;
	}

=======
	xa_init_flags(&cm.local_id_table, XA_FLAGS_ALLOC);
	get_random_bytes(&cm.random_id_operand, sizeof cm.random_id_operand);
	INIT_LIST_HEAD(&cm.timewait_list);

>>>>>>> upstream/android-13
	cm.wq = alloc_workqueue("ib_cm", 0, 1);
	if (!cm.wq) {
		ret = -ENOMEM;
		goto error2;
	}

	ret = ib_register_client(&cm_client);
	if (ret)
		goto error3;

	return 0;
error3:
	destroy_workqueue(cm.wq);
error2:
<<<<<<< HEAD
	class_unregister(&cm_class);
error1:
	idr_destroy(&cm.local_id_table);
=======
>>>>>>> upstream/android-13
	return ret;
}

static void __exit ib_cm_cleanup(void)
{
	struct cm_timewait_info *timewait_info, *tmp;

	spin_lock_irq(&cm.lock);
	list_for_each_entry(timewait_info, &cm.timewait_list, list)
		cancel_delayed_work(&timewait_info->work.work);
	spin_unlock_irq(&cm.lock);

	ib_unregister_client(&cm_client);
	destroy_workqueue(cm.wq);

	list_for_each_entry_safe(timewait_info, tmp, &cm.timewait_list, list) {
		list_del(&timewait_info->list);
		kfree(timewait_info);
	}

<<<<<<< HEAD
	class_unregister(&cm_class);
	idr_destroy(&cm.local_id_table);
=======
	WARN_ON(!xa_empty(&cm.local_id_table));
>>>>>>> upstream/android-13
}

module_init(ib_cm_init);
module_exit(ib_cm_cleanup);
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
