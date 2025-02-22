<<<<<<< HEAD
/*
 * Copyright(c) 2016 Intel Corporation.
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
=======
// SPDX-License-Identifier: GPL-2.0 or BSD-3-Clause
/*
 * Copyright(c) 2016 Intel Corporation.
>>>>>>> upstream/android-13
 */

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD

#include "srq.h"
#include "vt.h"

=======
#include <rdma/uverbs_ioctl.h>

#include "srq.h"
#include "vt.h"
#include "qp.h"
>>>>>>> upstream/android-13
/**
 * rvt_driver_srq_init - init srq resources on a per driver basis
 * @rdi: rvt dev structure
 *
 * Do any initialization needed when a driver registers with rdmavt.
 */
void rvt_driver_srq_init(struct rvt_dev_info *rdi)
{
	spin_lock_init(&rdi->n_srqs_lock);
	rdi->n_srqs_allocated = 0;
}

/**
 * rvt_create_srq - create a shared receive queue
<<<<<<< HEAD
 * @ibpd: the protection domain of the SRQ to create
 * @srq_init_attr: the attributes of the SRQ
 * @udata: data from libibverbs when creating a user SRQ
 *
 * Return: Allocated srq object
 */
struct ib_srq *rvt_create_srq(struct ib_pd *ibpd,
			      struct ib_srq_init_attr *srq_init_attr,
			      struct ib_udata *udata)
{
	struct rvt_dev_info *dev = ib_to_rvt(ibpd->device);
	struct rvt_srq *srq;
	u32 sz;
	struct ib_srq *ret;

	if (srq_init_attr->srq_type != IB_SRQT_BASIC)
		return ERR_PTR(-EOPNOTSUPP);
=======
 * @ibsrq: the protection domain of the SRQ to create
 * @srq_init_attr: the attributes of the SRQ
 * @udata: data from libibverbs when creating a user SRQ
 *
 * Return: 0 on success
 */
int rvt_create_srq(struct ib_srq *ibsrq, struct ib_srq_init_attr *srq_init_attr,
		   struct ib_udata *udata)
{
	struct rvt_dev_info *dev = ib_to_rvt(ibsrq->device);
	struct rvt_srq *srq = ibsrq_to_rvtsrq(ibsrq);
	u32 sz;
	int ret;

	if (srq_init_attr->srq_type != IB_SRQT_BASIC)
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13

	if (srq_init_attr->attr.max_sge == 0 ||
	    srq_init_attr->attr.max_sge > dev->dparms.props.max_srq_sge ||
	    srq_init_attr->attr.max_wr == 0 ||
	    srq_init_attr->attr.max_wr > dev->dparms.props.max_srq_wr)
<<<<<<< HEAD
		return ERR_PTR(-EINVAL);

	srq = kzalloc_node(sizeof(*srq), GFP_KERNEL, dev->dparms.node);
	if (!srq)
		return ERR_PTR(-ENOMEM);
=======
		return -EINVAL;
>>>>>>> upstream/android-13

	/*
	 * Need to use vmalloc() if we want to support large #s of entries.
	 */
	srq->rq.size = srq_init_attr->attr.max_wr + 1;
	srq->rq.max_sge = srq_init_attr->attr.max_sge;
	sz = sizeof(struct ib_sge) * srq->rq.max_sge +
		sizeof(struct rvt_rwqe);
<<<<<<< HEAD
	srq->rq.wq = udata ?
		vmalloc_user(sizeof(struct rvt_rwq) + srq->rq.size * sz) :
		vzalloc_node(sizeof(struct rvt_rwq) + srq->rq.size * sz,
			     dev->dparms.node);
	if (!srq->rq.wq) {
		ret = ERR_PTR(-ENOMEM);
=======
	if (rvt_alloc_rq(&srq->rq, srq->rq.size * sz,
			 dev->dparms.node, udata)) {
		ret = -ENOMEM;
>>>>>>> upstream/android-13
		goto bail_srq;
	}

	/*
	 * Return the address of the RWQ as the offset to mmap.
	 * See rvt_mmap() for details.
	 */
	if (udata && udata->outlen >= sizeof(__u64)) {
<<<<<<< HEAD
		int err;
		u32 s = sizeof(struct rvt_rwq) + srq->rq.size * sz;

		srq->ip =
		    rvt_create_mmap_info(dev, s, ibpd->uobject->context,
					 srq->rq.wq);
		if (!srq->ip) {
			ret = ERR_PTR(-ENOMEM);
			goto bail_wq;
		}

		err = ib_copy_to_udata(udata, &srq->ip->offset,
				       sizeof(srq->ip->offset));
		if (err) {
			ret = ERR_PTR(err);
			goto bail_ip;
		}
=======
		u32 s = sizeof(struct rvt_rwq) + srq->rq.size * sz;

		srq->ip = rvt_create_mmap_info(dev, s, udata, srq->rq.wq);
		if (IS_ERR(srq->ip)) {
			ret = PTR_ERR(srq->ip);
			goto bail_wq;
		}

		ret = ib_copy_to_udata(udata, &srq->ip->offset,
				       sizeof(srq->ip->offset));
		if (ret)
			goto bail_ip;
>>>>>>> upstream/android-13
	}

	/*
	 * ib_create_srq() will initialize srq->ibsrq.
	 */
	spin_lock_init(&srq->rq.lock);
	srq->limit = srq_init_attr->attr.srq_limit;

	spin_lock(&dev->n_srqs_lock);
	if (dev->n_srqs_allocated == dev->dparms.props.max_srq) {
		spin_unlock(&dev->n_srqs_lock);
<<<<<<< HEAD
		ret = ERR_PTR(-ENOMEM);
=======
		ret = -ENOMEM;
>>>>>>> upstream/android-13
		goto bail_ip;
	}

	dev->n_srqs_allocated++;
	spin_unlock(&dev->n_srqs_lock);

	if (srq->ip) {
		spin_lock_irq(&dev->pending_lock);
		list_add(&srq->ip->pending_mmaps, &dev->pending_mmaps);
		spin_unlock_irq(&dev->pending_lock);
	}

<<<<<<< HEAD
	return &srq->ibsrq;
=======
	return 0;
>>>>>>> upstream/android-13

bail_ip:
	kfree(srq->ip);
bail_wq:
<<<<<<< HEAD
	vfree(srq->rq.wq);
bail_srq:
	kfree(srq);
=======
	rvt_free_rq(&srq->rq);
bail_srq:
>>>>>>> upstream/android-13
	return ret;
}

/**
 * rvt_modify_srq - modify a shared receive queue
 * @ibsrq: the SRQ to modify
 * @attr: the new attributes of the SRQ
 * @attr_mask: indicates which attributes to modify
 * @udata: user data for libibverbs.so
 *
 * Return: 0 on success
 */
int rvt_modify_srq(struct ib_srq *ibsrq, struct ib_srq_attr *attr,
		   enum ib_srq_attr_mask attr_mask,
		   struct ib_udata *udata)
{
	struct rvt_srq *srq = ibsrq_to_rvtsrq(ibsrq);
	struct rvt_dev_info *dev = ib_to_rvt(ibsrq->device);
<<<<<<< HEAD
	struct rvt_rwq *wq;
	int ret = 0;

	if (attr_mask & IB_SRQ_MAX_WR) {
		struct rvt_rwq *owq;
=======
	struct rvt_rq tmp_rq = {};
	int ret = 0;

	if (attr_mask & IB_SRQ_MAX_WR) {
		struct rvt_krwq *okwq = NULL;
		struct rvt_rwq *owq = NULL;
>>>>>>> upstream/android-13
		struct rvt_rwqe *p;
		u32 sz, size, n, head, tail;

		/* Check that the requested sizes are below the limits. */
		if ((attr->max_wr > dev->dparms.props.max_srq_wr) ||
		    ((attr_mask & IB_SRQ_LIMIT) ?
		     attr->srq_limit : srq->limit) > attr->max_wr)
			return -EINVAL;
<<<<<<< HEAD

		sz = sizeof(struct rvt_rwqe) +
			srq->rq.max_sge * sizeof(struct ib_sge);
		size = attr->max_wr + 1;
		wq = udata ?
			vmalloc_user(sizeof(struct rvt_rwq) + size * sz) :
			vzalloc_node(sizeof(struct rvt_rwq) + size * sz,
				     dev->dparms.node);
		if (!wq)
			return -ENOMEM;

=======
		sz = sizeof(struct rvt_rwqe) +
			srq->rq.max_sge * sizeof(struct ib_sge);
		size = attr->max_wr + 1;
		if (rvt_alloc_rq(&tmp_rq, size * sz, dev->dparms.node,
				 udata))
			return -ENOMEM;
>>>>>>> upstream/android-13
		/* Check that we can write the offset to mmap. */
		if (udata && udata->inlen >= sizeof(__u64)) {
			__u64 offset_addr;
			__u64 offset = 0;

			ret = ib_copy_from_udata(&offset_addr, udata,
						 sizeof(offset_addr));
			if (ret)
				goto bail_free;
			udata->outbuf = (void __user *)
					(unsigned long)offset_addr;
			ret = ib_copy_to_udata(udata, &offset,
					       sizeof(offset));
			if (ret)
				goto bail_free;
		}

<<<<<<< HEAD
		spin_lock_irq(&srq->rq.lock);
=======
		spin_lock_irq(&srq->rq.kwq->c_lock);
>>>>>>> upstream/android-13
		/*
		 * validate head and tail pointer values and compute
		 * the number of remaining WQEs.
		 */
<<<<<<< HEAD
		owq = srq->rq.wq;
		head = owq->head;
		tail = owq->tail;
=======
		if (udata) {
			owq = srq->rq.wq;
			head = RDMA_READ_UAPI_ATOMIC(owq->head);
			tail = RDMA_READ_UAPI_ATOMIC(owq->tail);
		} else {
			okwq = srq->rq.kwq;
			head = okwq->head;
			tail = okwq->tail;
		}
>>>>>>> upstream/android-13
		if (head >= srq->rq.size || tail >= srq->rq.size) {
			ret = -EINVAL;
			goto bail_unlock;
		}
		n = head;
		if (n < tail)
			n += srq->rq.size - tail;
		else
			n -= tail;
		if (size <= n) {
			ret = -EINVAL;
			goto bail_unlock;
		}
		n = 0;
<<<<<<< HEAD
		p = wq->wq;
=======
		p = tmp_rq.kwq->curr_wq;
>>>>>>> upstream/android-13
		while (tail != head) {
			struct rvt_rwqe *wqe;
			int i;

			wqe = rvt_get_rwqe_ptr(&srq->rq, tail);
			p->wr_id = wqe->wr_id;
			p->num_sge = wqe->num_sge;
			for (i = 0; i < wqe->num_sge; i++)
				p->sg_list[i] = wqe->sg_list[i];
			n++;
			p = (struct rvt_rwqe *)((char *)p + sz);
			if (++tail >= srq->rq.size)
				tail = 0;
		}
<<<<<<< HEAD
		srq->rq.wq = wq;
		srq->rq.size = size;
		wq->head = n;
		wq->tail = 0;
		if (attr_mask & IB_SRQ_LIMIT)
			srq->limit = attr->srq_limit;
		spin_unlock_irq(&srq->rq.lock);

		vfree(owq);
=======
		srq->rq.kwq = tmp_rq.kwq;
		if (udata) {
			srq->rq.wq = tmp_rq.wq;
			RDMA_WRITE_UAPI_ATOMIC(tmp_rq.wq->head, n);
			RDMA_WRITE_UAPI_ATOMIC(tmp_rq.wq->tail, 0);
		} else {
			tmp_rq.kwq->head = n;
			tmp_rq.kwq->tail = 0;
		}
		srq->rq.size = size;
		if (attr_mask & IB_SRQ_LIMIT)
			srq->limit = attr->srq_limit;
		spin_unlock_irq(&srq->rq.kwq->c_lock);

		vfree(owq);
		kvfree(okwq);
>>>>>>> upstream/android-13

		if (srq->ip) {
			struct rvt_mmap_info *ip = srq->ip;
			struct rvt_dev_info *dev = ib_to_rvt(srq->ibsrq.device);
			u32 s = sizeof(struct rvt_rwq) + size * sz;

<<<<<<< HEAD
			rvt_update_mmap_info(dev, ip, s, wq);
=======
			rvt_update_mmap_info(dev, ip, s, tmp_rq.wq);
>>>>>>> upstream/android-13

			/*
			 * Return the offset to mmap.
			 * See rvt_mmap() for details.
			 */
			if (udata && udata->inlen >= sizeof(__u64)) {
				ret = ib_copy_to_udata(udata, &ip->offset,
						       sizeof(ip->offset));
				if (ret)
					return ret;
			}

			/*
			 * Put user mapping info onto the pending list
			 * unless it already is on the list.
			 */
			spin_lock_irq(&dev->pending_lock);
			if (list_empty(&ip->pending_mmaps))
				list_add(&ip->pending_mmaps,
					 &dev->pending_mmaps);
			spin_unlock_irq(&dev->pending_lock);
		}
	} else if (attr_mask & IB_SRQ_LIMIT) {
<<<<<<< HEAD
		spin_lock_irq(&srq->rq.lock);
=======
		spin_lock_irq(&srq->rq.kwq->c_lock);
>>>>>>> upstream/android-13
		if (attr->srq_limit >= srq->rq.size)
			ret = -EINVAL;
		else
			srq->limit = attr->srq_limit;
<<<<<<< HEAD
		spin_unlock_irq(&srq->rq.lock);
=======
		spin_unlock_irq(&srq->rq.kwq->c_lock);
>>>>>>> upstream/android-13
	}
	return ret;

bail_unlock:
<<<<<<< HEAD
	spin_unlock_irq(&srq->rq.lock);
bail_free:
	vfree(wq);
	return ret;
}

/** rvt_query_srq - query srq data
=======
	spin_unlock_irq(&srq->rq.kwq->c_lock);
bail_free:
	rvt_free_rq(&tmp_rq);
	return ret;
}

/**
 * rvt_query_srq - query srq data
>>>>>>> upstream/android-13
 * @ibsrq: srq to query
 * @attr: return info in attr
 *
 * Return: always 0
 */
int rvt_query_srq(struct ib_srq *ibsrq, struct ib_srq_attr *attr)
{
	struct rvt_srq *srq = ibsrq_to_rvtsrq(ibsrq);

	attr->max_wr = srq->rq.size - 1;
	attr->max_sge = srq->rq.max_sge;
	attr->srq_limit = srq->limit;
	return 0;
}

/**
 * rvt_destroy_srq - destory an srq
 * @ibsrq: srq object to destroy
<<<<<<< HEAD
 *
 * Return always 0
 */
int rvt_destroy_srq(struct ib_srq *ibsrq)
=======
 * @udata: user data for libibverbs.so
 */
int rvt_destroy_srq(struct ib_srq *ibsrq, struct ib_udata *udata)
>>>>>>> upstream/android-13
{
	struct rvt_srq *srq = ibsrq_to_rvtsrq(ibsrq);
	struct rvt_dev_info *dev = ib_to_rvt(ibsrq->device);

	spin_lock(&dev->n_srqs_lock);
	dev->n_srqs_allocated--;
	spin_unlock(&dev->n_srqs_lock);
	if (srq->ip)
		kref_put(&srq->ip->ref, rvt_release_mmap_info);
<<<<<<< HEAD
	else
		vfree(srq->rq.wq);
	kfree(srq);

=======
	kvfree(srq->rq.kwq);
>>>>>>> upstream/android-13
	return 0;
}
