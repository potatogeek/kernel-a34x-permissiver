/*
 * Copyright (c) 2017, Mellanox Technologies inc.  All rights reserved.
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
 */

#include <rdma/uverbs_std_types.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_verbs.h>
#include <linux/bug.h>
#include <linux/file.h>
#include <rdma/restrack.h>
#include "rdma_core.h"
#include "uverbs.h"

static int uverbs_free_ah(struct ib_uobject *uobject,
<<<<<<< HEAD
			  enum rdma_remove_reason why)
{
	return rdma_destroy_ah((struct ib_ah *)uobject->object);
}

static int uverbs_free_flow(struct ib_uobject *uobject,
			    enum rdma_remove_reason why)
=======
			  enum rdma_remove_reason why,
			  struct uverbs_attr_bundle *attrs)
{
	return rdma_destroy_ah_user((struct ib_ah *)uobject->object,
				    RDMA_DESTROY_AH_SLEEPABLE,
				    &attrs->driver_udata);
}

static int uverbs_free_flow(struct ib_uobject *uobject,
			    enum rdma_remove_reason why,
			    struct uverbs_attr_bundle *attrs)
>>>>>>> upstream/android-13
{
	struct ib_flow *flow = (struct ib_flow *)uobject->object;
	struct ib_uflow_object *uflow =
		container_of(uobject, struct ib_uflow_object, uobject);
	struct ib_qp *qp = flow->qp;
	int ret;

<<<<<<< HEAD
	ret = flow->device->destroy_flow(flow);
=======
	ret = flow->device->ops.destroy_flow(flow);
>>>>>>> upstream/android-13
	if (!ret) {
		if (qp)
			atomic_dec(&qp->usecnt);
		ib_uverbs_flow_resources_free(uflow->resources);
	}

	return ret;
}

static int uverbs_free_mw(struct ib_uobject *uobject,
<<<<<<< HEAD
			  enum rdma_remove_reason why)
=======
			  enum rdma_remove_reason why,
			  struct uverbs_attr_bundle *attrs)
>>>>>>> upstream/android-13
{
	return uverbs_dealloc_mw((struct ib_mw *)uobject->object);
}

<<<<<<< HEAD
static int uverbs_free_qp(struct ib_uobject *uobject,
			  enum rdma_remove_reason why)
{
	struct ib_qp *qp = uobject->object;
	struct ib_uqp_object *uqp =
		container_of(uobject, struct ib_uqp_object, uevent.uobject);
	int ret;

	/*
	 * If this is a user triggered destroy then do not allow destruction
	 * until the user cleans up all the mcast bindings. Unlike in other
	 * places we forcibly clean up the mcast attachments for !DESTROY
	 * because the mcast attaches are not ubojects and will not be
	 * destroyed by anything else during cleanup processing.
	 */
	if (why == RDMA_REMOVE_DESTROY) {
		if (!list_empty(&uqp->mcast_list))
			return -EBUSY;
	} else if (qp == qp->real_qp) {
		ib_uverbs_detach_umcast(qp, uqp);
	}

	ret = ib_destroy_qp(qp);
	if (ib_is_destroy_retryable(ret, why, uobject))
		return ret;

	if (uqp->uxrcd)
		atomic_dec(&uqp->uxrcd->refcnt);

	ib_uverbs_release_uevent(uobject->context->ufile, &uqp->uevent);
	return ret;
}

static int uverbs_free_rwq_ind_tbl(struct ib_uobject *uobject,
				   enum rdma_remove_reason why)
{
	struct ib_rwq_ind_table *rwq_ind_tbl = uobject->object;
	struct ib_wq **ind_tbl = rwq_ind_tbl->ind_tbl;
	int ret;

	ret = ib_destroy_rwq_ind_table(rwq_ind_tbl);
	if (ib_is_destroy_retryable(ret, why, uobject))
		return ret;

	kfree(ind_tbl);
	return ret;
}

static int uverbs_free_wq(struct ib_uobject *uobject,
			  enum rdma_remove_reason why)
{
	struct ib_wq *wq = uobject->object;
	struct ib_uwq_object *uwq =
		container_of(uobject, struct ib_uwq_object, uevent.uobject);
	int ret;

	ret = ib_destroy_wq(wq);
	if (ib_is_destroy_retryable(ret, why, uobject))
		return ret;

	ib_uverbs_release_uevent(uobject->context->ufile, &uwq->uevent);
	return ret;
}

static int uverbs_free_srq(struct ib_uobject *uobject,
			   enum rdma_remove_reason why)
{
	struct ib_srq *srq = uobject->object;
	struct ib_uevent_object *uevent =
		container_of(uobject, struct ib_uevent_object, uobject);
	enum ib_srq_type  srq_type = srq->srq_type;
	int ret;

	ret = ib_destroy_srq(srq);
	if (ib_is_destroy_retryable(ret, why, uobject))
		return ret;

	if (srq_type == IB_SRQT_XRC) {
		struct ib_usrq_object *us =
			container_of(uevent, struct ib_usrq_object, uevent);

		atomic_dec(&us->uxrcd->refcnt);
	}

	ib_uverbs_release_uevent(uobject->context->ufile, uevent);
	return ret;
}

static int uverbs_free_xrcd(struct ib_uobject *uobject,
			    enum rdma_remove_reason why)
=======
static int uverbs_free_rwq_ind_tbl(struct ib_uobject *uobject,
				   enum rdma_remove_reason why,
				   struct uverbs_attr_bundle *attrs)
{
	struct ib_rwq_ind_table *rwq_ind_tbl = uobject->object;
	struct ib_wq **ind_tbl = rwq_ind_tbl->ind_tbl;
	u32 table_size = (1 << rwq_ind_tbl->log_ind_tbl_size);
	int ret, i;

	if (atomic_read(&rwq_ind_tbl->usecnt))
		return -EBUSY;

	ret = rwq_ind_tbl->device->ops.destroy_rwq_ind_table(rwq_ind_tbl);
	if (ret)
		return ret;

	for (i = 0; i < table_size; i++)
		atomic_dec(&ind_tbl[i]->usecnt);

	kfree(rwq_ind_tbl);
	kfree(ind_tbl);
	return 0;
}

static int uverbs_free_xrcd(struct ib_uobject *uobject,
			    enum rdma_remove_reason why,
			    struct uverbs_attr_bundle *attrs)
>>>>>>> upstream/android-13
{
	struct ib_xrcd *xrcd = uobject->object;
	struct ib_uxrcd_object *uxrcd =
		container_of(uobject, struct ib_uxrcd_object, uobject);
	int ret;

<<<<<<< HEAD
	ret = ib_destroy_usecnt(&uxrcd->refcnt, why, uobject);
	if (ret)
		return ret;

	mutex_lock(&uobject->context->ufile->device->xrcd_tree_mutex);
	ret = ib_uverbs_dealloc_xrcd(uobject, xrcd, why);
	mutex_unlock(&uobject->context->ufile->device->xrcd_tree_mutex);
=======
	if (atomic_read(&uxrcd->refcnt))
		return -EBUSY;

	mutex_lock(&attrs->ufile->device->xrcd_tree_mutex);
	ret = ib_uverbs_dealloc_xrcd(uobject, xrcd, why, attrs);
	mutex_unlock(&attrs->ufile->device->xrcd_tree_mutex);
>>>>>>> upstream/android-13

	return ret;
}

static int uverbs_free_pd(struct ib_uobject *uobject,
<<<<<<< HEAD
			  enum rdma_remove_reason why)
{
	struct ib_pd *pd = uobject->object;
	int ret;

	ret = ib_destroy_usecnt(&pd->usecnt, why, uobject);
	if (ret)
		return ret;

	ib_dealloc_pd((struct ib_pd *)uobject->object);
	return 0;
}

static int uverbs_hot_unplug_completion_event_file(struct ib_uobject *uobj,
						   enum rdma_remove_reason why)
{
	struct ib_uverbs_completion_event_file *comp_event_file =
		container_of(uobj, struct ib_uverbs_completion_event_file,
			     uobj);
	struct ib_uverbs_event_queue *event_queue = &comp_event_file->ev_queue;

	spin_lock_irq(&event_queue->lock);
	event_queue->is_closed = 1;
	spin_unlock_irq(&event_queue->lock);

	if (why == RDMA_REMOVE_DRIVER_REMOVE) {
		wake_up_interruptible(&event_queue->poll_wait);
		kill_fasync(&event_queue->async_queue, SIGIO, POLL_IN);
	}
	return 0;
};

int uverbs_destroy_def_handler(struct ib_uverbs_file *file,
			       struct uverbs_attr_bundle *attrs)
=======
			  enum rdma_remove_reason why,
			  struct uverbs_attr_bundle *attrs)
{
	struct ib_pd *pd = uobject->object;

	if (atomic_read(&pd->usecnt))
		return -EBUSY;

	return ib_dealloc_pd_user(pd, &attrs->driver_udata);
}

void ib_uverbs_free_event_queue(struct ib_uverbs_event_queue *event_queue)
{
	struct ib_uverbs_event *entry, *tmp;

	spin_lock_irq(&event_queue->lock);
	/*
	 * The user must ensure that no new items are added to the event_list
	 * once is_closed is set.
	 */
	event_queue->is_closed = 1;
	spin_unlock_irq(&event_queue->lock);
	wake_up_interruptible(&event_queue->poll_wait);
	kill_fasync(&event_queue->async_queue, SIGIO, POLL_IN);

	spin_lock_irq(&event_queue->lock);
	list_for_each_entry_safe(entry, tmp, &event_queue->event_list, list) {
		if (entry->counter)
			list_del(&entry->obj_list);
		list_del(&entry->list);
		kfree(entry);
	}
	spin_unlock_irq(&event_queue->lock);
}

static void
uverbs_completion_event_file_destroy_uobj(struct ib_uobject *uobj,
					  enum rdma_remove_reason why)
{
	struct ib_uverbs_completion_event_file *file =
		container_of(uobj, struct ib_uverbs_completion_event_file,
			     uobj);

	ib_uverbs_free_event_queue(&file->ev_queue);
}

int uverbs_destroy_def_handler(struct uverbs_attr_bundle *attrs)
>>>>>>> upstream/android-13
{
	return 0;
}
EXPORT_SYMBOL(uverbs_destroy_def_handler);

DECLARE_UVERBS_NAMED_OBJECT(
	UVERBS_OBJECT_COMP_CHANNEL,
	UVERBS_TYPE_ALLOC_FD(sizeof(struct ib_uverbs_completion_event_file),
<<<<<<< HEAD
			     uverbs_hot_unplug_completion_event_file,
=======
			     uverbs_completion_event_file_destroy_uobj,
>>>>>>> upstream/android-13
			     &uverbs_event_fops,
			     "[infinibandevent]",
			     O_RDONLY));

<<<<<<< HEAD
DECLARE_UVERBS_NAMED_OBJECT(
	UVERBS_OBJECT_QP,
	UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_uqp_object), uverbs_free_qp));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_MW,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_mw));

DECLARE_UVERBS_NAMED_OBJECT(
	UVERBS_OBJECT_SRQ,
	UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_usrq_object),
				 uverbs_free_srq));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_AH,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_ah));
=======
DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	UVERBS_METHOD_MW_DESTROY,
	UVERBS_ATTR_IDR(UVERBS_ATTR_DESTROY_MW_HANDLE,
			UVERBS_OBJECT_MW,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_MW,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_mw),
			    &UVERBS_METHOD(UVERBS_METHOD_MW_DESTROY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	UVERBS_METHOD_AH_DESTROY,
	UVERBS_ATTR_IDR(UVERBS_ATTR_DESTROY_AH_HANDLE,
			UVERBS_OBJECT_AH,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_AH,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_ah),
			    &UVERBS_METHOD(UVERBS_METHOD_AH_DESTROY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	UVERBS_METHOD_FLOW_DESTROY,
	UVERBS_ATTR_IDR(UVERBS_ATTR_DESTROY_FLOW_HANDLE,
			UVERBS_OBJECT_FLOW,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));
>>>>>>> upstream/android-13

DECLARE_UVERBS_NAMED_OBJECT(
	UVERBS_OBJECT_FLOW,
	UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_uflow_object),
<<<<<<< HEAD
				 uverbs_free_flow));

DECLARE_UVERBS_NAMED_OBJECT(
	UVERBS_OBJECT_WQ,
	UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_uwq_object), uverbs_free_wq));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_RWQ_IND_TBL,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_rwq_ind_tbl));
=======
				 uverbs_free_flow),
			    &UVERBS_METHOD(UVERBS_METHOD_FLOW_DESTROY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	UVERBS_METHOD_RWQ_IND_TBL_DESTROY,
	UVERBS_ATTR_IDR(UVERBS_ATTR_DESTROY_RWQ_IND_TBL_HANDLE,
			UVERBS_OBJECT_RWQ_IND_TBL,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_RWQ_IND_TBL,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_rwq_ind_tbl),
			    &UVERBS_METHOD(UVERBS_METHOD_RWQ_IND_TBL_DESTROY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	UVERBS_METHOD_XRCD_DESTROY,
	UVERBS_ATTR_IDR(UVERBS_ATTR_DESTROY_XRCD_HANDLE,
			UVERBS_OBJECT_XRCD,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));
>>>>>>> upstream/android-13

DECLARE_UVERBS_NAMED_OBJECT(
	UVERBS_OBJECT_XRCD,
	UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_uxrcd_object),
<<<<<<< HEAD
				 uverbs_free_xrcd));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_PD,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_pd));

DECLARE_UVERBS_GLOBAL_METHODS(UVERBS_OBJECT_DEVICE);

DECLARE_UVERBS_OBJECT_TREE(uverbs_default_objects,
			   &UVERBS_OBJECT(UVERBS_OBJECT_DEVICE),
			   &UVERBS_OBJECT(UVERBS_OBJECT_PD),
			   &UVERBS_OBJECT(UVERBS_OBJECT_MR),
			   &UVERBS_OBJECT(UVERBS_OBJECT_COMP_CHANNEL),
			   &UVERBS_OBJECT(UVERBS_OBJECT_CQ),
			   &UVERBS_OBJECT(UVERBS_OBJECT_QP),
			   &UVERBS_OBJECT(UVERBS_OBJECT_AH),
			   &UVERBS_OBJECT(UVERBS_OBJECT_MW),
			   &UVERBS_OBJECT(UVERBS_OBJECT_SRQ),
			   &UVERBS_OBJECT(UVERBS_OBJECT_FLOW),
			   &UVERBS_OBJECT(UVERBS_OBJECT_WQ),
			   &UVERBS_OBJECT(UVERBS_OBJECT_RWQ_IND_TBL),
			   &UVERBS_OBJECT(UVERBS_OBJECT_XRCD),
			   &UVERBS_OBJECT(UVERBS_OBJECT_FLOW_ACTION),
			   &UVERBS_OBJECT(UVERBS_OBJECT_DM),
			   &UVERBS_OBJECT(UVERBS_OBJECT_COUNTERS));

const struct uverbs_object_tree_def *uverbs_default_get_objects(void)
{
	return &uverbs_default_objects;
}
=======
				 uverbs_free_xrcd),
			    &UVERBS_METHOD(UVERBS_METHOD_XRCD_DESTROY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	UVERBS_METHOD_PD_DESTROY,
	UVERBS_ATTR_IDR(UVERBS_ATTR_DESTROY_PD_HANDLE,
			UVERBS_OBJECT_PD,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));

DECLARE_UVERBS_NAMED_OBJECT(UVERBS_OBJECT_PD,
			    UVERBS_TYPE_ALLOC_IDR(uverbs_free_pd),
			    &UVERBS_METHOD(UVERBS_METHOD_PD_DESTROY));

const struct uapi_definition uverbs_def_obj_intf[] = {
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(UVERBS_OBJECT_PD,
				      UAPI_DEF_OBJ_NEEDS_FN(dealloc_pd)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(UVERBS_OBJECT_COMP_CHANNEL,
				      UAPI_DEF_OBJ_NEEDS_FN(dealloc_pd)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(UVERBS_OBJECT_AH,
				      UAPI_DEF_OBJ_NEEDS_FN(destroy_ah)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(UVERBS_OBJECT_MW,
				      UAPI_DEF_OBJ_NEEDS_FN(dealloc_mw)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(UVERBS_OBJECT_FLOW,
				      UAPI_DEF_OBJ_NEEDS_FN(destroy_flow)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(
		UVERBS_OBJECT_RWQ_IND_TBL,
		UAPI_DEF_OBJ_NEEDS_FN(destroy_rwq_ind_table)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(UVERBS_OBJECT_XRCD,
				      UAPI_DEF_OBJ_NEEDS_FN(dealloc_xrcd)),
	{}
};
>>>>>>> upstream/android-13
