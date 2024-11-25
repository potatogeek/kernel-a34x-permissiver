<<<<<<< HEAD
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
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
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
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
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/dma-mapping.h>
#include <net/addrconf.h>
<<<<<<< HEAD
=======
#include <rdma/uverbs_ioctl.h>
>>>>>>> upstream/android-13
#include "rxe.h"
#include "rxe_loc.h"
#include "rxe_queue.h"
#include "rxe_hw_counters.h"

static int rxe_query_device(struct ib_device *dev,
			    struct ib_device_attr *attr,
			    struct ib_udata *uhw)
{
	struct rxe_dev *rxe = to_rdev(dev);

	if (uhw->inlen || uhw->outlen)
		return -EINVAL;

	*attr = rxe->attr;
	return 0;
}

static int rxe_query_port(struct ib_device *dev,
<<<<<<< HEAD
			  u8 port_num, struct ib_port_attr *attr)
{
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_port *port;
	int rc = -EINVAL;

	if (unlikely(port_num != 1)) {
		pr_warn("invalid port_number %d\n", port_num);
		goto out;
	}
=======
			  u32 port_num, struct ib_port_attr *attr)
{
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_port *port;
	int rc;
>>>>>>> upstream/android-13

	port = &rxe->port;

	/* *attr being zeroed by the caller, avoid zeroing it here */
	*attr = port->attr;

	mutex_lock(&rxe->usdev_lock);
	rc = ib_get_eth_speed(dev, port_num, &attr->active_speed,
			      &attr->active_width);
<<<<<<< HEAD
	mutex_unlock(&rxe->usdev_lock);

out:
	return rc;
}

static struct net_device *rxe_get_netdev(struct ib_device *device,
					 u8 port_num)
{
	struct rxe_dev *rxe = to_rdev(device);

	if (rxe->ndev) {
		dev_hold(rxe->ndev);
		return rxe->ndev;
	}

	return NULL;
}

static int rxe_query_pkey(struct ib_device *device,
			  u8 port_num, u16 index, u16 *pkey)
{
	struct rxe_dev *rxe = to_rdev(device);
	struct rxe_port *port;

	if (unlikely(port_num != 1)) {
		dev_warn(device->dev.parent, "invalid port_num = %d\n",
			 port_num);
		goto err1;
	}

	port = &rxe->port;

	if (unlikely(index >= port->attr.pkey_tbl_len)) {
		dev_warn(device->dev.parent, "invalid index = %d\n",
			 index);
		goto err1;
	}

	*pkey = port->pkey_tbl[index];
	return 0;

err1:
	return -EINVAL;
=======

	if (attr->state == IB_PORT_ACTIVE)
		attr->phys_state = IB_PORT_PHYS_STATE_LINK_UP;
	else if (dev_get_flags(rxe->ndev) & IFF_UP)
		attr->phys_state = IB_PORT_PHYS_STATE_POLLING;
	else
		attr->phys_state = IB_PORT_PHYS_STATE_DISABLED;

	mutex_unlock(&rxe->usdev_lock);

	return rc;
}

static int rxe_query_pkey(struct ib_device *device,
			  u32 port_num, u16 index, u16 *pkey)
{
	if (index > 0)
		return -EINVAL;

	*pkey = IB_DEFAULT_PKEY_FULL;
	return 0;
>>>>>>> upstream/android-13
}

static int rxe_modify_device(struct ib_device *dev,
			     int mask, struct ib_device_modify *attr)
{
	struct rxe_dev *rxe = to_rdev(dev);

<<<<<<< HEAD
=======
	if (mask & ~(IB_DEVICE_MODIFY_SYS_IMAGE_GUID |
		     IB_DEVICE_MODIFY_NODE_DESC))
		return -EOPNOTSUPP;

>>>>>>> upstream/android-13
	if (mask & IB_DEVICE_MODIFY_SYS_IMAGE_GUID)
		rxe->attr.sys_image_guid = cpu_to_be64(attr->sys_image_guid);

	if (mask & IB_DEVICE_MODIFY_NODE_DESC) {
		memcpy(rxe->ib_dev.node_desc,
		       attr->node_desc, sizeof(rxe->ib_dev.node_desc));
	}

	return 0;
}

static int rxe_modify_port(struct ib_device *dev,
<<<<<<< HEAD
			   u8 port_num, int mask, struct ib_port_modify *attr)
=======
			   u32 port_num, int mask, struct ib_port_modify *attr)
>>>>>>> upstream/android-13
{
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_port *port;

<<<<<<< HEAD
	if (unlikely(port_num != 1)) {
		pr_warn("invalid port_num = %d\n", port_num);
		goto err1;
	}

=======
>>>>>>> upstream/android-13
	port = &rxe->port;

	port->attr.port_cap_flags |= attr->set_port_cap_mask;
	port->attr.port_cap_flags &= ~attr->clr_port_cap_mask;

	if (mask & IB_PORT_RESET_QKEY_CNTR)
		port->attr.qkey_viol_cntr = 0;

	return 0;
<<<<<<< HEAD

err1:
	return -EINVAL;
}

static enum rdma_link_layer rxe_get_link_layer(struct ib_device *dev,
					       u8 port_num)
{
	struct rxe_dev *rxe = to_rdev(dev);

	return rxe_link_layer(rxe, port_num);
}

static struct ib_ucontext *rxe_alloc_ucontext(struct ib_device *dev,
					      struct ib_udata *udata)
{
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_ucontext *uc;

	uc = rxe_alloc(&rxe->uc_pool);
	return uc ? &uc->ibuc : ERR_PTR(-ENOMEM);
}

static int rxe_dealloc_ucontext(struct ib_ucontext *ibuc)
=======
}

static enum rdma_link_layer rxe_get_link_layer(struct ib_device *dev,
					       u32 port_num)
{
	return IB_LINK_LAYER_ETHERNET;
}

static int rxe_alloc_ucontext(struct ib_ucontext *ibuc, struct ib_udata *udata)
{
	struct rxe_dev *rxe = to_rdev(ibuc->device);
	struct rxe_ucontext *uc = to_ruc(ibuc);

	return rxe_add_to_pool(&rxe->uc_pool, uc);
}

static void rxe_dealloc_ucontext(struct ib_ucontext *ibuc)
>>>>>>> upstream/android-13
{
	struct rxe_ucontext *uc = to_ruc(ibuc);

	rxe_drop_ref(uc);
<<<<<<< HEAD
	return 0;
}

static int rxe_port_immutable(struct ib_device *dev, u8 port_num,
=======
}

static int rxe_port_immutable(struct ib_device *dev, u32 port_num,
>>>>>>> upstream/android-13
			      struct ib_port_immutable *immutable)
{
	int err;
	struct ib_port_attr attr;

	immutable->core_cap_flags = RDMA_CORE_PORT_IBA_ROCE_UDP_ENCAP;

	err = ib_query_port(dev, port_num, &attr);
	if (err)
		return err;

	immutable->pkey_tbl_len = attr.pkey_tbl_len;
	immutable->gid_tbl_len = attr.gid_tbl_len;
	immutable->max_mad_size = IB_MGMT_MAD_SIZE;

	return 0;
}

<<<<<<< HEAD
static struct ib_pd *rxe_alloc_pd(struct ib_device *dev,
				  struct ib_ucontext *context,
				  struct ib_udata *udata)
{
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_pd *pd;

	pd = rxe_alloc(&rxe->pd_pool);
	return pd ? &pd->ibpd : ERR_PTR(-ENOMEM);
}

static int rxe_dealloc_pd(struct ib_pd *ibpd)
=======
static int rxe_alloc_pd(struct ib_pd *ibpd, struct ib_udata *udata)
{
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);

	return rxe_add_to_pool(&rxe->pd_pool, pd);
}

static int rxe_dealloc_pd(struct ib_pd *ibpd, struct ib_udata *udata)
>>>>>>> upstream/android-13
{
	struct rxe_pd *pd = to_rpd(ibpd);

	rxe_drop_ref(pd);
	return 0;
}

<<<<<<< HEAD
static void rxe_init_av(struct rxe_dev *rxe, struct rdma_ah_attr *attr,
			struct rxe_av *av)
{
	rxe_av_from_attr(rdma_ah_get_port_num(attr), av, attr);
	rxe_av_fill_ip_info(av, attr);
}

static struct ib_ah *rxe_create_ah(struct ib_pd *ibpd,
				   struct rdma_ah_attr *attr,
				   struct ib_udata *udata)

{
	int err;
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);
	struct rxe_ah *ah;

	err = rxe_av_chk_attr(rxe, attr);
	if (err)
		return ERR_PTR(err);

	ah = rxe_alloc(&rxe->ah_pool);
	if (!ah)
		return ERR_PTR(-ENOMEM);

	rxe_add_ref(pd);
	ah->pd = pd;

	rxe_init_av(rxe, attr, &ah->av);
	return &ah->ibah;
=======
static int rxe_create_ah(struct ib_ah *ibah,
			 struct rdma_ah_init_attr *init_attr,
			 struct ib_udata *udata)

{
	int err;
	struct rxe_dev *rxe = to_rdev(ibah->device);
	struct rxe_ah *ah = to_rah(ibah);

	err = rxe_av_chk_attr(rxe, init_attr->ah_attr);
	if (err)
		return err;

	err = rxe_add_to_pool(&rxe->ah_pool, ah);
	if (err)
		return err;

	rxe_init_av(init_attr->ah_attr, &ah->av);
	return 0;
>>>>>>> upstream/android-13
}

static int rxe_modify_ah(struct ib_ah *ibah, struct rdma_ah_attr *attr)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibah->device);
	struct rxe_ah *ah = to_rah(ibah);

	err = rxe_av_chk_attr(rxe, attr);
	if (err)
		return err;

<<<<<<< HEAD
	rxe_init_av(rxe, attr, &ah->av);
=======
	rxe_init_av(attr, &ah->av);
>>>>>>> upstream/android-13
	return 0;
}

static int rxe_query_ah(struct ib_ah *ibah, struct rdma_ah_attr *attr)
{
	struct rxe_ah *ah = to_rah(ibah);

	memset(attr, 0, sizeof(*attr));
	attr->type = ibah->type;
	rxe_av_to_attr(&ah->av, attr);
	return 0;
}

<<<<<<< HEAD
static int rxe_destroy_ah(struct ib_ah *ibah)
{
	struct rxe_ah *ah = to_rah(ibah);

	rxe_drop_ref(ah->pd);
=======
static int rxe_destroy_ah(struct ib_ah *ibah, u32 flags)
{
	struct rxe_ah *ah = to_rah(ibah);

>>>>>>> upstream/android-13
	rxe_drop_ref(ah);
	return 0;
}

static int post_one_recv(struct rxe_rq *rq, const struct ib_recv_wr *ibwr)
{
	int err;
	int i;
	u32 length;
	struct rxe_recv_wqe *recv_wqe;
	int num_sge = ibwr->num_sge;
<<<<<<< HEAD

	if (unlikely(queue_full(rq->queue))) {
=======
	int full;

	if (rq->is_user)
		full = queue_full(rq->queue, QUEUE_TYPE_FROM_USER);
	else
		full = queue_full(rq->queue, QUEUE_TYPE_KERNEL);

	if (unlikely(full)) {
>>>>>>> upstream/android-13
		err = -ENOMEM;
		goto err1;
	}

	if (unlikely(num_sge > rq->max_sge)) {
		err = -EINVAL;
		goto err1;
	}

	length = 0;
	for (i = 0; i < num_sge; i++)
		length += ibwr->sg_list[i].length;

<<<<<<< HEAD
	recv_wqe = producer_addr(rq->queue);
=======
	if (rq->is_user)
		recv_wqe = producer_addr(rq->queue, QUEUE_TYPE_FROM_USER);
	else
		recv_wqe = producer_addr(rq->queue, QUEUE_TYPE_KERNEL);

>>>>>>> upstream/android-13
	recv_wqe->wr_id = ibwr->wr_id;
	recv_wqe->num_sge = num_sge;

	memcpy(recv_wqe->dma.sge, ibwr->sg_list,
	       num_sge * sizeof(struct ib_sge));

	recv_wqe->dma.length		= length;
	recv_wqe->dma.resid		= length;
	recv_wqe->dma.num_sge		= num_sge;
	recv_wqe->dma.cur_sge		= 0;
	recv_wqe->dma.sge_offset	= 0;

<<<<<<< HEAD
	/* make sure all changes to the work queue are written before we
	 * update the producer pointer
	 */
	smp_wmb();

	advance_producer(rq->queue);
=======
	if (rq->is_user)
		advance_producer(rq->queue, QUEUE_TYPE_FROM_USER);
	else
		advance_producer(rq->queue, QUEUE_TYPE_KERNEL);

>>>>>>> upstream/android-13
	return 0;

err1:
	return err;
}

<<<<<<< HEAD
static struct ib_srq *rxe_create_srq(struct ib_pd *ibpd,
				     struct ib_srq_init_attr *init,
				     struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);
	struct rxe_srq *srq;
	struct ib_ucontext *context = udata ? ibpd->uobject->context : NULL;
	struct rxe_create_srq_resp __user *uresp = NULL;

	if (udata) {
		if (udata->outlen < sizeof(*uresp))
			return ERR_PTR(-EINVAL);
		uresp = udata->outbuf;
=======
static int rxe_create_srq(struct ib_srq *ibsrq, struct ib_srq_init_attr *init,
			  struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibsrq->device);
	struct rxe_pd *pd = to_rpd(ibsrq->pd);
	struct rxe_srq *srq = to_rsrq(ibsrq);
	struct rxe_create_srq_resp __user *uresp = NULL;

	if (init->srq_type != IB_SRQT_BASIC)
		return -EOPNOTSUPP;

	if (udata) {
		if (udata->outlen < sizeof(*uresp))
			return -EINVAL;
		uresp = udata->outbuf;
		srq->is_user = true;
	} else {
		srq->is_user = false;
>>>>>>> upstream/android-13
	}

	err = rxe_srq_chk_attr(rxe, NULL, &init->attr, IB_SRQ_INIT_MASK);
	if (err)
		goto err1;

<<<<<<< HEAD
	srq = rxe_alloc(&rxe->srq_pool);
	if (!srq) {
		err = -ENOMEM;
		goto err1;
	}

	rxe_add_index(srq);
	rxe_add_ref(pd);
	srq->pd = pd;

	err = rxe_srq_from_init(rxe, srq, init, context, uresp);
	if (err)
		goto err2;

	return &srq->ibsrq;

err2:
	rxe_drop_ref(pd);
	rxe_drop_index(srq);
	rxe_drop_ref(srq);
err1:
	return ERR_PTR(err);
=======
	err = rxe_add_to_pool(&rxe->srq_pool, srq);
	if (err)
		goto err1;

	rxe_add_ref(pd);
	srq->pd = pd;

	err = rxe_srq_from_init(rxe, srq, init, udata, uresp);
	if (err)
		goto err2;

	return 0;

err2:
	rxe_drop_ref(pd);
	rxe_drop_ref(srq);
err1:
	return err;
>>>>>>> upstream/android-13
}

static int rxe_modify_srq(struct ib_srq *ibsrq, struct ib_srq_attr *attr,
			  enum ib_srq_attr_mask mask,
			  struct ib_udata *udata)
{
	int err;
	struct rxe_srq *srq = to_rsrq(ibsrq);
	struct rxe_dev *rxe = to_rdev(ibsrq->device);
	struct rxe_modify_srq_cmd ucmd = {};

	if (udata) {
		if (udata->inlen < sizeof(ucmd))
			return -EINVAL;

		err = ib_copy_from_udata(&ucmd, udata, sizeof(ucmd));
		if (err)
			return err;
	}

	err = rxe_srq_chk_attr(rxe, srq, attr, mask);
	if (err)
		goto err1;

<<<<<<< HEAD
	err = rxe_srq_from_attr(rxe, srq, attr, mask, &ucmd);
=======
	err = rxe_srq_from_attr(rxe, srq, attr, mask, &ucmd, udata);
>>>>>>> upstream/android-13
	if (err)
		goto err1;

	return 0;

err1:
	return err;
}

static int rxe_query_srq(struct ib_srq *ibsrq, struct ib_srq_attr *attr)
{
	struct rxe_srq *srq = to_rsrq(ibsrq);

	if (srq->error)
		return -EINVAL;

	attr->max_wr = srq->rq.queue->buf->index_mask;
	attr->max_sge = srq->rq.max_sge;
	attr->srq_limit = srq->limit;
	return 0;
}

<<<<<<< HEAD
static int rxe_destroy_srq(struct ib_srq *ibsrq)
=======
static int rxe_destroy_srq(struct ib_srq *ibsrq, struct ib_udata *udata)
>>>>>>> upstream/android-13
{
	struct rxe_srq *srq = to_rsrq(ibsrq);

	if (srq->rq.queue)
		rxe_queue_cleanup(srq->rq.queue);

	rxe_drop_ref(srq->pd);
<<<<<<< HEAD
	rxe_drop_index(srq);
	rxe_drop_ref(srq);

=======
	rxe_drop_ref(srq);
>>>>>>> upstream/android-13
	return 0;
}

static int rxe_post_srq_recv(struct ib_srq *ibsrq, const struct ib_recv_wr *wr,
			     const struct ib_recv_wr **bad_wr)
{
	int err = 0;
	unsigned long flags;
	struct rxe_srq *srq = to_rsrq(ibsrq);

	spin_lock_irqsave(&srq->rq.producer_lock, flags);

	while (wr) {
		err = post_one_recv(&srq->rq, wr);
		if (unlikely(err))
			break;
		wr = wr->next;
	}

	spin_unlock_irqrestore(&srq->rq.producer_lock, flags);

	if (err)
		*bad_wr = wr;

	return err;
}

<<<<<<< HEAD
static struct ib_qp *rxe_create_qp(struct ib_pd *ibpd,
				   struct ib_qp_init_attr *init,
				   struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);
	struct rxe_qp *qp;
=======
static int rxe_create_qp(struct ib_qp *ibqp, struct ib_qp_init_attr *init,
			 struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibqp->device);
	struct rxe_pd *pd = to_rpd(ibqp->pd);
	struct rxe_qp *qp = to_rqp(ibqp);
>>>>>>> upstream/android-13
	struct rxe_create_qp_resp __user *uresp = NULL;

	if (udata) {
		if (udata->outlen < sizeof(*uresp))
<<<<<<< HEAD
			return ERR_PTR(-EINVAL);
		uresp = udata->outbuf;
	}

	err = rxe_qp_chk_init(rxe, init);
	if (err)
		goto err1;

	qp = rxe_alloc(&rxe->qp_pool);
	if (!qp) {
		err = -ENOMEM;
		goto err1;
	}

	if (udata) {
		if (udata->inlen) {
			err = -EINVAL;
			goto err2;
		}
		qp->is_user = 1;
	}

	rxe_add_index(qp);

	err = rxe_qp_from_init(rxe, qp, pd, init, uresp, ibpd);
	if (err)
		goto err3;

	return &qp->ibqp;

err3:
	rxe_drop_index(qp);
err2:
	rxe_drop_ref(qp);
err1:
	return ERR_PTR(err);
=======
			return -EINVAL;
		uresp = udata->outbuf;
	}

	if (init->create_flags)
		return -EOPNOTSUPP;

	err = rxe_qp_chk_init(rxe, init);
	if (err)
		return err;

	if (udata) {
		if (udata->inlen)
			return -EINVAL;

		qp->is_user = true;
	} else {
		qp->is_user = false;
	}

	err = rxe_add_to_pool(&rxe->qp_pool, qp);
	if (err)
		return err;

	rxe_add_index(qp);
	err = rxe_qp_from_init(rxe, qp, pd, init, uresp, ibqp->pd, udata);
	if (err)
		goto qp_init;

	return 0;

qp_init:
	rxe_drop_index(qp);
	rxe_drop_ref(qp);
	return err;
>>>>>>> upstream/android-13
}

static int rxe_modify_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
			 int mask, struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibqp->device);
	struct rxe_qp *qp = to_rqp(ibqp);

<<<<<<< HEAD
=======
	if (mask & ~IB_QP_ATTR_STANDARD_BITS)
		return -EOPNOTSUPP;

>>>>>>> upstream/android-13
	err = rxe_qp_chk_attr(rxe, qp, attr, mask);
	if (err)
		goto err1;

	err = rxe_qp_from_attr(qp, attr, mask, udata);
	if (err)
		goto err1;

	return 0;

err1:
	return err;
}

static int rxe_query_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
			int mask, struct ib_qp_init_attr *init)
{
	struct rxe_qp *qp = to_rqp(ibqp);

	rxe_qp_to_init(qp, init);
	rxe_qp_to_attr(qp, attr, mask);

	return 0;
}

<<<<<<< HEAD
static int rxe_destroy_qp(struct ib_qp *ibqp)
=======
static int rxe_destroy_qp(struct ib_qp *ibqp, struct ib_udata *udata)
>>>>>>> upstream/android-13
{
	struct rxe_qp *qp = to_rqp(ibqp);

	rxe_qp_destroy(qp);
	rxe_drop_index(qp);
	rxe_drop_ref(qp);
	return 0;
}

static int validate_send_wr(struct rxe_qp *qp, const struct ib_send_wr *ibwr,
			    unsigned int mask, unsigned int length)
{
	int num_sge = ibwr->num_sge;
	struct rxe_sq *sq = &qp->sq;

	if (unlikely(num_sge > sq->max_sge))
		goto err1;

	if (unlikely(mask & WR_ATOMIC_MASK)) {
		if (length < 8)
			goto err1;

		if (atomic_wr(ibwr)->remote_addr & 0x7)
			goto err1;
	}

	if (unlikely((ibwr->send_flags & IB_SEND_INLINE) &&
		     (length > sq->max_inline)))
		goto err1;

	return 0;

err1:
	return -EINVAL;
}

static void init_send_wr(struct rxe_qp *qp, struct rxe_send_wr *wr,
			 const struct ib_send_wr *ibwr)
{
	wr->wr_id = ibwr->wr_id;
	wr->num_sge = ibwr->num_sge;
	wr->opcode = ibwr->opcode;
	wr->send_flags = ibwr->send_flags;

	if (qp_type(qp) == IB_QPT_UD ||
	    qp_type(qp) == IB_QPT_SMI ||
	    qp_type(qp) == IB_QPT_GSI) {
		wr->wr.ud.remote_qpn = ud_wr(ibwr)->remote_qpn;
		wr->wr.ud.remote_qkey = ud_wr(ibwr)->remote_qkey;
		if (qp_type(qp) == IB_QPT_GSI)
			wr->wr.ud.pkey_index = ud_wr(ibwr)->pkey_index;
		if (wr->opcode == IB_WR_SEND_WITH_IMM)
			wr->ex.imm_data = ibwr->ex.imm_data;
	} else {
		switch (wr->opcode) {
		case IB_WR_RDMA_WRITE_WITH_IMM:
			wr->ex.imm_data = ibwr->ex.imm_data;
<<<<<<< HEAD
			/* fall through */
=======
			fallthrough;
>>>>>>> upstream/android-13
		case IB_WR_RDMA_READ:
		case IB_WR_RDMA_WRITE:
			wr->wr.rdma.remote_addr = rdma_wr(ibwr)->remote_addr;
			wr->wr.rdma.rkey	= rdma_wr(ibwr)->rkey;
			break;
		case IB_WR_SEND_WITH_IMM:
			wr->ex.imm_data = ibwr->ex.imm_data;
			break;
		case IB_WR_SEND_WITH_INV:
			wr->ex.invalidate_rkey = ibwr->ex.invalidate_rkey;
			break;
		case IB_WR_ATOMIC_CMP_AND_SWP:
		case IB_WR_ATOMIC_FETCH_AND_ADD:
			wr->wr.atomic.remote_addr =
				atomic_wr(ibwr)->remote_addr;
			wr->wr.atomic.compare_add =
				atomic_wr(ibwr)->compare_add;
			wr->wr.atomic.swap = atomic_wr(ibwr)->swap;
			wr->wr.atomic.rkey = atomic_wr(ibwr)->rkey;
			break;
		case IB_WR_LOCAL_INV:
			wr->ex.invalidate_rkey = ibwr->ex.invalidate_rkey;
		break;
		case IB_WR_REG_MR:
			wr->wr.reg.mr = reg_wr(ibwr)->mr;
			wr->wr.reg.key = reg_wr(ibwr)->key;
			wr->wr.reg.access = reg_wr(ibwr)->access;
		break;
		default:
			break;
		}
	}
}

<<<<<<< HEAD
static int init_send_wqe(struct rxe_qp *qp, const struct ib_send_wr *ibwr,
=======
static void copy_inline_data_to_wqe(struct rxe_send_wqe *wqe,
				    const struct ib_send_wr *ibwr)
{
	struct ib_sge *sge = ibwr->sg_list;
	u8 *p = wqe->dma.inline_data;
	int i;

	for (i = 0; i < ibwr->num_sge; i++, sge++) {
		memcpy(p, (void *)(uintptr_t)sge->addr, sge->length);
		p += sge->length;
	}
}

static void init_send_wqe(struct rxe_qp *qp, const struct ib_send_wr *ibwr,
>>>>>>> upstream/android-13
			 unsigned int mask, unsigned int length,
			 struct rxe_send_wqe *wqe)
{
	int num_sge = ibwr->num_sge;
<<<<<<< HEAD
	struct ib_sge *sge;
	int i;
	u8 *p;

	init_send_wr(qp, &wqe->wr, ibwr);

=======

	init_send_wr(qp, &wqe->wr, ibwr);

	/* local operation */
	if (unlikely(mask & WR_LOCAL_OP_MASK)) {
		wqe->mask = mask;
		wqe->state = wqe_state_posted;
		return;
	}

>>>>>>> upstream/android-13
	if (qp_type(qp) == IB_QPT_UD ||
	    qp_type(qp) == IB_QPT_SMI ||
	    qp_type(qp) == IB_QPT_GSI)
		memcpy(&wqe->av, &to_rah(ud_wr(ibwr)->ah)->av, sizeof(wqe->av));

<<<<<<< HEAD
	if (unlikely(ibwr->send_flags & IB_SEND_INLINE)) {
		p = wqe->dma.inline_data;

		sge = ibwr->sg_list;
		for (i = 0; i < num_sge; i++, sge++) {
			memcpy(p, (void *)(uintptr_t)sge->addr,
					sge->length);

			p += sge->length;
		}
	} else if (mask & WR_REG_MASK) {
		wqe->mask = mask;
		wqe->state = wqe_state_posted;
		return 0;
	} else
=======
	if (unlikely(ibwr->send_flags & IB_SEND_INLINE))
		copy_inline_data_to_wqe(wqe, ibwr);
	else
>>>>>>> upstream/android-13
		memcpy(wqe->dma.sge, ibwr->sg_list,
		       num_sge * sizeof(struct ib_sge));

	wqe->iova = mask & WR_ATOMIC_MASK ? atomic_wr(ibwr)->remote_addr :
		mask & WR_READ_OR_WRITE_MASK ? rdma_wr(ibwr)->remote_addr : 0;
	wqe->mask		= mask;
	wqe->dma.length		= length;
	wqe->dma.resid		= length;
	wqe->dma.num_sge	= num_sge;
	wqe->dma.cur_sge	= 0;
	wqe->dma.sge_offset	= 0;
	wqe->state		= wqe_state_posted;
	wqe->ssn		= atomic_add_return(1, &qp->ssn);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static int post_one_send(struct rxe_qp *qp, const struct ib_send_wr *ibwr,
			 unsigned int mask, u32 length)
{
	int err;
	struct rxe_sq *sq = &qp->sq;
	struct rxe_send_wqe *send_wqe;
	unsigned long flags;
<<<<<<< HEAD
=======
	int full;
>>>>>>> upstream/android-13

	err = validate_send_wr(qp, ibwr, mask, length);
	if (err)
		return err;

	spin_lock_irqsave(&qp->sq.sq_lock, flags);

<<<<<<< HEAD
	if (unlikely(queue_full(sq->queue))) {
		err = -ENOMEM;
		goto err1;
	}

	send_wqe = producer_addr(sq->queue);

	err = init_send_wqe(qp, ibwr, mask, length, send_wqe);
	if (unlikely(err))
		goto err1;

	/*
	 * make sure all changes to the work queue are
	 * written before we update the producer pointer
	 */
	smp_wmb();

	advance_producer(sq->queue);
	spin_unlock_irqrestore(&qp->sq.sq_lock, flags);

	return 0;

err1:
	spin_unlock_irqrestore(&qp->sq.sq_lock, flags);
	return err;
=======
	if (qp->is_user)
		full = queue_full(sq->queue, QUEUE_TYPE_FROM_USER);
	else
		full = queue_full(sq->queue, QUEUE_TYPE_KERNEL);

	if (unlikely(full)) {
		spin_unlock_irqrestore(&qp->sq.sq_lock, flags);
		return -ENOMEM;
	}

	if (qp->is_user)
		send_wqe = producer_addr(sq->queue, QUEUE_TYPE_FROM_USER);
	else
		send_wqe = producer_addr(sq->queue, QUEUE_TYPE_KERNEL);

	init_send_wqe(qp, ibwr, mask, length, send_wqe);

	if (qp->is_user)
		advance_producer(sq->queue, QUEUE_TYPE_FROM_USER);
	else
		advance_producer(sq->queue, QUEUE_TYPE_KERNEL);

	spin_unlock_irqrestore(&qp->sq.sq_lock, flags);

	return 0;
>>>>>>> upstream/android-13
}

static int rxe_post_send_kernel(struct rxe_qp *qp, const struct ib_send_wr *wr,
				const struct ib_send_wr **bad_wr)
{
	int err = 0;
	unsigned int mask;
	unsigned int length = 0;
	int i;
	struct ib_send_wr *next;

	while (wr) {
		mask = wr_opcode_mask(wr->opcode, qp);
		if (unlikely(!mask)) {
			err = -EINVAL;
			*bad_wr = wr;
			break;
		}

		if (unlikely((wr->send_flags & IB_SEND_INLINE) &&
			     !(mask & WR_INLINE_MASK))) {
			err = -EINVAL;
			*bad_wr = wr;
			break;
		}

		next = wr->next;

		length = 0;
		for (i = 0; i < wr->num_sge; i++)
			length += wr->sg_list[i].length;

		err = post_one_send(qp, wr, mask, length);

		if (err) {
			*bad_wr = wr;
			break;
		}
		wr = next;
	}

	rxe_run_task(&qp->req.task, 1);
	if (unlikely(qp->req.state == QP_STATE_ERROR))
		rxe_run_task(&qp->comp.task, 1);

	return err;
}

static int rxe_post_send(struct ib_qp *ibqp, const struct ib_send_wr *wr,
			 const struct ib_send_wr **bad_wr)
{
	struct rxe_qp *qp = to_rqp(ibqp);

	if (unlikely(!qp->valid)) {
		*bad_wr = wr;
		return -EINVAL;
	}

	if (unlikely(qp->req.state < QP_STATE_READY)) {
		*bad_wr = wr;
		return -EINVAL;
	}

	if (qp->is_user) {
		/* Utilize process context to do protocol processing */
		rxe_run_task(&qp->req.task, 0);
		return 0;
	} else
		return rxe_post_send_kernel(qp, wr, bad_wr);
}

static int rxe_post_recv(struct ib_qp *ibqp, const struct ib_recv_wr *wr,
			 const struct ib_recv_wr **bad_wr)
{
	int err = 0;
	struct rxe_qp *qp = to_rqp(ibqp);
	struct rxe_rq *rq = &qp->rq;
	unsigned long flags;

	if (unlikely((qp_state(qp) < IB_QPS_INIT) || !qp->valid)) {
		*bad_wr = wr;
		err = -EINVAL;
		goto err1;
	}

	if (unlikely(qp->srq)) {
		*bad_wr = wr;
		err = -EINVAL;
		goto err1;
	}

	spin_lock_irqsave(&rq->producer_lock, flags);

	while (wr) {
		err = post_one_recv(rq, wr);
		if (unlikely(err)) {
			*bad_wr = wr;
			break;
		}
		wr = wr->next;
	}

	spin_unlock_irqrestore(&rq->producer_lock, flags);

	if (qp->resp.state == QP_STATE_ERROR)
		rxe_run_task(&qp->resp.task, 1);

err1:
	return err;
}

<<<<<<< HEAD
static struct ib_cq *rxe_create_cq(struct ib_device *dev,
				   const struct ib_cq_init_attr *attr,
				   struct ib_ucontext *context,
				   struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_cq *cq;
=======
static int rxe_create_cq(struct ib_cq *ibcq, const struct ib_cq_init_attr *attr,
			 struct ib_udata *udata)
{
	int err;
	struct ib_device *dev = ibcq->device;
	struct rxe_dev *rxe = to_rdev(dev);
	struct rxe_cq *cq = to_rcq(ibcq);
>>>>>>> upstream/android-13
	struct rxe_create_cq_resp __user *uresp = NULL;

	if (udata) {
		if (udata->outlen < sizeof(*uresp))
<<<<<<< HEAD
			return ERR_PTR(-EINVAL);
=======
			return -EINVAL;
>>>>>>> upstream/android-13
		uresp = udata->outbuf;
	}

	if (attr->flags)
<<<<<<< HEAD
		return ERR_PTR(-EINVAL);

	err = rxe_cq_chk_attr(rxe, NULL, attr->cqe, attr->comp_vector);
	if (err)
		goto err1;

	cq = rxe_alloc(&rxe->cq_pool);
	if (!cq) {
		err = -ENOMEM;
		goto err1;
	}

	err = rxe_cq_from_init(rxe, cq, attr->cqe, attr->comp_vector,
			       context, uresp);
	if (err)
		goto err2;

	return &cq->ibcq;

err2:
	rxe_drop_ref(cq);
err1:
	return ERR_PTR(err);
}

static int rxe_destroy_cq(struct ib_cq *ibcq)
=======
		return -EOPNOTSUPP;

	err = rxe_cq_chk_attr(rxe, NULL, attr->cqe, attr->comp_vector);
	if (err)
		return err;

	err = rxe_cq_from_init(rxe, cq, attr->cqe, attr->comp_vector, udata,
			       uresp);
	if (err)
		return err;

	return rxe_add_to_pool(&rxe->cq_pool, cq);
}

static int rxe_destroy_cq(struct ib_cq *ibcq, struct ib_udata *udata)
>>>>>>> upstream/android-13
{
	struct rxe_cq *cq = to_rcq(ibcq);

	rxe_cq_disable(cq);

	rxe_drop_ref(cq);
	return 0;
}

static int rxe_resize_cq(struct ib_cq *ibcq, int cqe, struct ib_udata *udata)
{
	int err;
	struct rxe_cq *cq = to_rcq(ibcq);
	struct rxe_dev *rxe = to_rdev(ibcq->device);
	struct rxe_resize_cq_resp __user *uresp = NULL;

	if (udata) {
		if (udata->outlen < sizeof(*uresp))
			return -EINVAL;
		uresp = udata->outbuf;
	}

	err = rxe_cq_chk_attr(rxe, cq, cqe, 0);
	if (err)
		goto err1;

<<<<<<< HEAD
	err = rxe_cq_resize_queue(cq, cqe, uresp);
=======
	err = rxe_cq_resize_queue(cq, cqe, uresp, udata);
>>>>>>> upstream/android-13
	if (err)
		goto err1;

	return 0;

err1:
	return err;
}

static int rxe_poll_cq(struct ib_cq *ibcq, int num_entries, struct ib_wc *wc)
{
	int i;
	struct rxe_cq *cq = to_rcq(ibcq);
	struct rxe_cqe *cqe;
	unsigned long flags;

	spin_lock_irqsave(&cq->cq_lock, flags);
	for (i = 0; i < num_entries; i++) {
<<<<<<< HEAD
		cqe = queue_head(cq->queue);
=======
		if (cq->is_user)
			cqe = queue_head(cq->queue, QUEUE_TYPE_TO_USER);
		else
			cqe = queue_head(cq->queue, QUEUE_TYPE_KERNEL);
>>>>>>> upstream/android-13
		if (!cqe)
			break;

		memcpy(wc++, &cqe->ibwc, sizeof(*wc));
<<<<<<< HEAD
		advance_consumer(cq->queue);
=======
		if (cq->is_user)
			advance_consumer(cq->queue, QUEUE_TYPE_TO_USER);
		else
			advance_consumer(cq->queue, QUEUE_TYPE_KERNEL);
>>>>>>> upstream/android-13
	}
	spin_unlock_irqrestore(&cq->cq_lock, flags);

	return i;
}

static int rxe_peek_cq(struct ib_cq *ibcq, int wc_cnt)
{
	struct rxe_cq *cq = to_rcq(ibcq);
<<<<<<< HEAD
	int count = queue_count(cq->queue);
=======
	int count;

	if (cq->is_user)
		count = queue_count(cq->queue, QUEUE_TYPE_TO_USER);
	else
		count = queue_count(cq->queue, QUEUE_TYPE_KERNEL);
>>>>>>> upstream/android-13

	return (count > wc_cnt) ? wc_cnt : count;
}

static int rxe_req_notify_cq(struct ib_cq *ibcq, enum ib_cq_notify_flags flags)
{
	struct rxe_cq *cq = to_rcq(ibcq);
	unsigned long irq_flags;
	int ret = 0;
<<<<<<< HEAD
=======
	int empty;
>>>>>>> upstream/android-13

	spin_lock_irqsave(&cq->cq_lock, irq_flags);
	if (cq->notify != IB_CQ_NEXT_COMP)
		cq->notify = flags & IB_CQ_SOLICITED_MASK;

<<<<<<< HEAD
	if ((flags & IB_CQ_REPORT_MISSED_EVENTS) && !queue_empty(cq->queue))
=======
	if (cq->is_user)
		empty = queue_empty(cq->queue, QUEUE_TYPE_TO_USER);
	else
		empty = queue_empty(cq->queue, QUEUE_TYPE_KERNEL);

	if ((flags & IB_CQ_REPORT_MISSED_EVENTS) && !empty)
>>>>>>> upstream/android-13
		ret = 1;

	spin_unlock_irqrestore(&cq->cq_lock, irq_flags);

	return ret;
}

static struct ib_mr *rxe_get_dma_mr(struct ib_pd *ibpd, int access)
{
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);
<<<<<<< HEAD
	struct rxe_mem *mr;
	int err;

	mr = rxe_alloc(&rxe->mr_pool);
	if (!mr) {
		err = -ENOMEM;
		goto err1;
	}

	rxe_add_index(mr);

	rxe_add_ref(pd);

	err = rxe_mem_init_dma(pd, access, mr);
	if (err)
		goto err2;

	return &mr->ibmr;

err2:
	rxe_drop_ref(pd);
	rxe_drop_index(mr);
	rxe_drop_ref(mr);
err1:
	return ERR_PTR(err);
=======
	struct rxe_mr *mr;

	mr = rxe_alloc(&rxe->mr_pool);
	if (!mr)
		return ERR_PTR(-ENOMEM);

	rxe_add_index(mr);
	rxe_add_ref(pd);
	rxe_mr_init_dma(pd, access, mr);

	return &mr->ibmr;
>>>>>>> upstream/android-13
}

static struct ib_mr *rxe_reg_user_mr(struct ib_pd *ibpd,
				     u64 start,
				     u64 length,
				     u64 iova,
				     int access, struct ib_udata *udata)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);
<<<<<<< HEAD
	struct rxe_mem *mr;
=======
	struct rxe_mr *mr;
>>>>>>> upstream/android-13

	mr = rxe_alloc(&rxe->mr_pool);
	if (!mr) {
		err = -ENOMEM;
		goto err2;
	}

	rxe_add_index(mr);

	rxe_add_ref(pd);

<<<<<<< HEAD
	err = rxe_mem_init_user(pd, start, length, iova,
				access, udata, mr);
=======
	err = rxe_mr_init_user(pd, start, length, iova, access, mr);
>>>>>>> upstream/android-13
	if (err)
		goto err3;

	return &mr->ibmr;

err3:
	rxe_drop_ref(pd);
	rxe_drop_index(mr);
	rxe_drop_ref(mr);
err2:
	return ERR_PTR(err);
}

<<<<<<< HEAD
static int rxe_dereg_mr(struct ib_mr *ibmr)
{
	struct rxe_mem *mr = to_rmr(ibmr);

	mr->state = RXE_MEM_STATE_ZOMBIE;
	rxe_drop_ref(mr->pd);
	rxe_drop_index(mr);
	rxe_drop_ref(mr);
	return 0;
}

static struct ib_mr *rxe_alloc_mr(struct ib_pd *ibpd,
				  enum ib_mr_type mr_type,
=======
static struct ib_mr *rxe_alloc_mr(struct ib_pd *ibpd, enum ib_mr_type mr_type,
>>>>>>> upstream/android-13
				  u32 max_num_sg)
{
	struct rxe_dev *rxe = to_rdev(ibpd->device);
	struct rxe_pd *pd = to_rpd(ibpd);
<<<<<<< HEAD
	struct rxe_mem *mr;
=======
	struct rxe_mr *mr;
>>>>>>> upstream/android-13
	int err;

	if (mr_type != IB_MR_TYPE_MEM_REG)
		return ERR_PTR(-EINVAL);

	mr = rxe_alloc(&rxe->mr_pool);
	if (!mr) {
		err = -ENOMEM;
		goto err1;
	}

	rxe_add_index(mr);

	rxe_add_ref(pd);

<<<<<<< HEAD
	err = rxe_mem_init_fast(pd, max_num_sg, mr);
=======
	err = rxe_mr_init_fast(pd, max_num_sg, mr);
>>>>>>> upstream/android-13
	if (err)
		goto err2;

	return &mr->ibmr;

err2:
	rxe_drop_ref(pd);
	rxe_drop_index(mr);
	rxe_drop_ref(mr);
err1:
	return ERR_PTR(err);
}

static int rxe_set_page(struct ib_mr *ibmr, u64 addr)
{
<<<<<<< HEAD
	struct rxe_mem *mr = to_rmr(ibmr);
=======
	struct rxe_mr *mr = to_rmr(ibmr);
>>>>>>> upstream/android-13
	struct rxe_map *map;
	struct rxe_phys_buf *buf;

	if (unlikely(mr->nbuf == mr->num_buf))
		return -ENOMEM;

	map = mr->map[mr->nbuf / RXE_BUF_PER_MAP];
	buf = &map->buf[mr->nbuf % RXE_BUF_PER_MAP];

	buf->addr = addr;
	buf->size = ibmr->page_size;
	mr->nbuf++;

	return 0;
}

static int rxe_map_mr_sg(struct ib_mr *ibmr, struct scatterlist *sg,
			 int sg_nents, unsigned int *sg_offset)
{
<<<<<<< HEAD
	struct rxe_mem *mr = to_rmr(ibmr);
=======
	struct rxe_mr *mr = to_rmr(ibmr);
>>>>>>> upstream/android-13
	int n;

	mr->nbuf = 0;

	n = ib_sg_to_pages(ibmr, sg, sg_nents, sg_offset, rxe_set_page);

	mr->va = ibmr->iova;
	mr->iova = ibmr->iova;
	mr->length = ibmr->length;
	mr->page_shift = ilog2(ibmr->page_size);
	mr->page_mask = ibmr->page_size - 1;
	mr->offset = mr->iova & mr->page_mask;

	return n;
}

static int rxe_attach_mcast(struct ib_qp *ibqp, union ib_gid *mgid, u16 mlid)
{
	int err;
	struct rxe_dev *rxe = to_rdev(ibqp->device);
	struct rxe_qp *qp = to_rqp(ibqp);
	struct rxe_mc_grp *grp;

	/* takes a ref on grp if successful */
	err = rxe_mcast_get_grp(rxe, mgid, &grp);
	if (err)
		return err;

	err = rxe_mcast_add_grp_elem(rxe, qp, grp);

	rxe_drop_ref(grp);
	return err;
}

static int rxe_detach_mcast(struct ib_qp *ibqp, union ib_gid *mgid, u16 mlid)
{
	struct rxe_dev *rxe = to_rdev(ibqp->device);
	struct rxe_qp *qp = to_rqp(ibqp);

	return rxe_mcast_drop_grp_elem(rxe, qp, mgid);
}

static ssize_t parent_show(struct device *device,
			   struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct rxe_dev *rxe = container_of(device, struct rxe_dev,
					   ib_dev.dev);

	return scnprintf(buf, PAGE_SIZE, "%s\n", rxe_parent_name(rxe, 1));
=======
	struct rxe_dev *rxe =
		rdma_device_to_drv_device(device, struct rxe_dev, ib_dev);

	return sysfs_emit(buf, "%s\n", rxe_parent_name(rxe, 1));
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(parent);

<<<<<<< HEAD
static struct device_attribute *rxe_dev_attributes[] = {
	&dev_attr_parent,
};

int rxe_register_device(struct rxe_dev *rxe)
{
	int err;
	int i;
	struct ib_device *dev = &rxe->ib_dev;
	struct crypto_shash *tfm;

	strlcpy(dev->name, "rxe%d", IB_DEVICE_NAME_MAX);
	strlcpy(dev->node_desc, "rxe", sizeof(dev->node_desc));

	dev->owner = THIS_MODULE;
	dev->node_type = RDMA_NODE_IB_CA;
	dev->phys_port_cnt = 1;
	dev->num_comp_vectors = num_possible_cpus();
	dev->dev.parent = rxe_dma_device(rxe);
	dev->local_dma_lkey = 0;
	addrconf_addr_eui48((unsigned char *)&dev->node_guid,
			    rxe->ndev->dev_addr);
	dev->dev.dma_ops = &dma_virt_ops;
	dma_coerce_mask_and_coherent(&dev->dev,
				     dma_get_required_mask(&dev->dev));

	dev->uverbs_abi_ver = RXE_UVERBS_ABI_VERSION;
	dev->uverbs_cmd_mask = BIT_ULL(IB_USER_VERBS_CMD_GET_CONTEXT)
	    | BIT_ULL(IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL)
	    | BIT_ULL(IB_USER_VERBS_CMD_QUERY_DEVICE)
	    | BIT_ULL(IB_USER_VERBS_CMD_QUERY_PORT)
	    | BIT_ULL(IB_USER_VERBS_CMD_ALLOC_PD)
	    | BIT_ULL(IB_USER_VERBS_CMD_DEALLOC_PD)
	    | BIT_ULL(IB_USER_VERBS_CMD_CREATE_SRQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_MODIFY_SRQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_QUERY_SRQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_DESTROY_SRQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_POST_SRQ_RECV)
	    | BIT_ULL(IB_USER_VERBS_CMD_CREATE_QP)
	    | BIT_ULL(IB_USER_VERBS_CMD_MODIFY_QP)
	    | BIT_ULL(IB_USER_VERBS_CMD_QUERY_QP)
	    | BIT_ULL(IB_USER_VERBS_CMD_DESTROY_QP)
	    | BIT_ULL(IB_USER_VERBS_CMD_POST_SEND)
	    | BIT_ULL(IB_USER_VERBS_CMD_POST_RECV)
	    | BIT_ULL(IB_USER_VERBS_CMD_CREATE_CQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_RESIZE_CQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_DESTROY_CQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_POLL_CQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_PEEK_CQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_REQ_NOTIFY_CQ)
	    | BIT_ULL(IB_USER_VERBS_CMD_REG_MR)
	    | BIT_ULL(IB_USER_VERBS_CMD_DEREG_MR)
	    | BIT_ULL(IB_USER_VERBS_CMD_CREATE_AH)
	    | BIT_ULL(IB_USER_VERBS_CMD_MODIFY_AH)
	    | BIT_ULL(IB_USER_VERBS_CMD_QUERY_AH)
	    | BIT_ULL(IB_USER_VERBS_CMD_DESTROY_AH)
	    | BIT_ULL(IB_USER_VERBS_CMD_ATTACH_MCAST)
	    | BIT_ULL(IB_USER_VERBS_CMD_DETACH_MCAST)
	    ;

	dev->query_device = rxe_query_device;
	dev->modify_device = rxe_modify_device;
	dev->query_port = rxe_query_port;
	dev->modify_port = rxe_modify_port;
	dev->get_link_layer = rxe_get_link_layer;
	dev->get_netdev = rxe_get_netdev;
	dev->query_pkey = rxe_query_pkey;
	dev->alloc_ucontext = rxe_alloc_ucontext;
	dev->dealloc_ucontext = rxe_dealloc_ucontext;
	dev->mmap = rxe_mmap;
	dev->get_port_immutable = rxe_port_immutable;
	dev->alloc_pd = rxe_alloc_pd;
	dev->dealloc_pd = rxe_dealloc_pd;
	dev->create_ah = rxe_create_ah;
	dev->modify_ah = rxe_modify_ah;
	dev->query_ah = rxe_query_ah;
	dev->destroy_ah = rxe_destroy_ah;
	dev->create_srq = rxe_create_srq;
	dev->modify_srq = rxe_modify_srq;
	dev->query_srq = rxe_query_srq;
	dev->destroy_srq = rxe_destroy_srq;
	dev->post_srq_recv = rxe_post_srq_recv;
	dev->create_qp = rxe_create_qp;
	dev->modify_qp = rxe_modify_qp;
	dev->query_qp = rxe_query_qp;
	dev->destroy_qp = rxe_destroy_qp;
	dev->post_send = rxe_post_send;
	dev->post_recv = rxe_post_recv;
	dev->create_cq = rxe_create_cq;
	dev->destroy_cq = rxe_destroy_cq;
	dev->resize_cq = rxe_resize_cq;
	dev->poll_cq = rxe_poll_cq;
	dev->peek_cq = rxe_peek_cq;
	dev->req_notify_cq = rxe_req_notify_cq;
	dev->get_dma_mr = rxe_get_dma_mr;
	dev->reg_user_mr = rxe_reg_user_mr;
	dev->dereg_mr = rxe_dereg_mr;
	dev->alloc_mr = rxe_alloc_mr;
	dev->map_mr_sg = rxe_map_mr_sg;
	dev->attach_mcast = rxe_attach_mcast;
	dev->detach_mcast = rxe_detach_mcast;
	dev->get_hw_stats = rxe_ib_get_hw_stats;
	dev->alloc_hw_stats = rxe_ib_alloc_hw_stats;

	tfm = crypto_alloc_shash("crc32", 0, 0);
	if (IS_ERR(tfm)) {
		pr_err("failed to allocate crc algorithm err:%ld\n",
		       PTR_ERR(tfm));
		return PTR_ERR(tfm);
	}
	rxe->tfm = tfm;

	dev->driver_id = RDMA_DRIVER_RXE;
	err = ib_register_device(dev, NULL);
	if (err) {
		pr_warn("%s failed with error %d\n", __func__, err);
		goto err1;
	}

	for (i = 0; i < ARRAY_SIZE(rxe_dev_attributes); ++i) {
		err = device_create_file(&dev->dev, rxe_dev_attributes[i]);
		if (err) {
			pr_warn("%s failed with error %d for attr number %d\n",
				__func__, err, i);
			goto err2;
		}
	}

	return 0;

err2:
	ib_unregister_device(dev);
err1:
	crypto_free_shash(rxe->tfm);

	return err;
}

int rxe_unregister_device(struct rxe_dev *rxe)
{
	int i;
	struct ib_device *dev = &rxe->ib_dev;

	for (i = 0; i < ARRAY_SIZE(rxe_dev_attributes); ++i)
		device_remove_file(&dev->dev, rxe_dev_attributes[i]);

	ib_unregister_device(dev);

	return 0;
}
=======
static struct attribute *rxe_dev_attributes[] = {
	&dev_attr_parent.attr,
	NULL
};

static const struct attribute_group rxe_attr_group = {
	.attrs = rxe_dev_attributes,
};

static int rxe_enable_driver(struct ib_device *ib_dev)
{
	struct rxe_dev *rxe = container_of(ib_dev, struct rxe_dev, ib_dev);

	rxe_set_port_state(rxe);
	dev_info(&rxe->ib_dev.dev, "added %s\n", netdev_name(rxe->ndev));
	return 0;
}

static const struct ib_device_ops rxe_dev_ops = {
	.owner = THIS_MODULE,
	.driver_id = RDMA_DRIVER_RXE,
	.uverbs_abi_ver = RXE_UVERBS_ABI_VERSION,

	.alloc_hw_port_stats = rxe_ib_alloc_hw_port_stats,
	.alloc_mr = rxe_alloc_mr,
	.alloc_mw = rxe_alloc_mw,
	.alloc_pd = rxe_alloc_pd,
	.alloc_ucontext = rxe_alloc_ucontext,
	.attach_mcast = rxe_attach_mcast,
	.create_ah = rxe_create_ah,
	.create_cq = rxe_create_cq,
	.create_qp = rxe_create_qp,
	.create_srq = rxe_create_srq,
	.create_user_ah = rxe_create_ah,
	.dealloc_driver = rxe_dealloc,
	.dealloc_mw = rxe_dealloc_mw,
	.dealloc_pd = rxe_dealloc_pd,
	.dealloc_ucontext = rxe_dealloc_ucontext,
	.dereg_mr = rxe_dereg_mr,
	.destroy_ah = rxe_destroy_ah,
	.destroy_cq = rxe_destroy_cq,
	.destroy_qp = rxe_destroy_qp,
	.destroy_srq = rxe_destroy_srq,
	.detach_mcast = rxe_detach_mcast,
	.device_group = &rxe_attr_group,
	.enable_driver = rxe_enable_driver,
	.get_dma_mr = rxe_get_dma_mr,
	.get_hw_stats = rxe_ib_get_hw_stats,
	.get_link_layer = rxe_get_link_layer,
	.get_port_immutable = rxe_port_immutable,
	.map_mr_sg = rxe_map_mr_sg,
	.mmap = rxe_mmap,
	.modify_ah = rxe_modify_ah,
	.modify_device = rxe_modify_device,
	.modify_port = rxe_modify_port,
	.modify_qp = rxe_modify_qp,
	.modify_srq = rxe_modify_srq,
	.peek_cq = rxe_peek_cq,
	.poll_cq = rxe_poll_cq,
	.post_recv = rxe_post_recv,
	.post_send = rxe_post_send,
	.post_srq_recv = rxe_post_srq_recv,
	.query_ah = rxe_query_ah,
	.query_device = rxe_query_device,
	.query_pkey = rxe_query_pkey,
	.query_port = rxe_query_port,
	.query_qp = rxe_query_qp,
	.query_srq = rxe_query_srq,
	.reg_user_mr = rxe_reg_user_mr,
	.req_notify_cq = rxe_req_notify_cq,
	.resize_cq = rxe_resize_cq,

	INIT_RDMA_OBJ_SIZE(ib_ah, rxe_ah, ibah),
	INIT_RDMA_OBJ_SIZE(ib_cq, rxe_cq, ibcq),
	INIT_RDMA_OBJ_SIZE(ib_pd, rxe_pd, ibpd),
	INIT_RDMA_OBJ_SIZE(ib_qp, rxe_qp, ibqp),
	INIT_RDMA_OBJ_SIZE(ib_srq, rxe_srq, ibsrq),
	INIT_RDMA_OBJ_SIZE(ib_ucontext, rxe_ucontext, ibuc),
	INIT_RDMA_OBJ_SIZE(ib_mw, rxe_mw, ibmw),
};

int rxe_register_device(struct rxe_dev *rxe, const char *ibdev_name)
{
	int err;
	struct ib_device *dev = &rxe->ib_dev;

	strscpy(dev->node_desc, "rxe", sizeof(dev->node_desc));

	dev->node_type = RDMA_NODE_IB_CA;
	dev->phys_port_cnt = 1;
	dev->num_comp_vectors = num_possible_cpus();
	dev->local_dma_lkey = 0;
	addrconf_addr_eui48((unsigned char *)&dev->node_guid,
			    rxe->ndev->dev_addr);

	dev->uverbs_cmd_mask |= BIT_ULL(IB_USER_VERBS_CMD_POST_SEND) |
				BIT_ULL(IB_USER_VERBS_CMD_REQ_NOTIFY_CQ);

	ib_set_device_ops(dev, &rxe_dev_ops);
	err = ib_device_set_netdev(&rxe->ib_dev, rxe->ndev, 1);
	if (err)
		return err;

	err = rxe_icrc_init(rxe);
	if (err)
		return err;

	err = ib_register_device(dev, ibdev_name, NULL);
	if (err)
		pr_warn("%s failed with error %d\n", __func__, err);

	/*
	 * Note that rxe may be invalid at this point if another thread
	 * unregistered it.
	 */
	return err;
}
>>>>>>> upstream/android-13
