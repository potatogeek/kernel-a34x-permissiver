// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2017-2018 Mellanox Technologies. All rights reserved.
 */

#include <rdma/rdma_cm.h>
#include <rdma/ib_verbs.h>
#include <rdma/restrack.h>
<<<<<<< HEAD
=======
#include <rdma/rdma_counter.h>
>>>>>>> upstream/android-13
#include <linux/mutex.h>
#include <linux/sched/task.h>
#include <linux/pid_namespace.h>

#include "cma_priv.h"
<<<<<<< HEAD

static int fill_res_noop(struct sk_buff *msg,
			 struct rdma_restrack_entry *entry)
{
	return 0;
}

void rdma_restrack_init(struct rdma_restrack_root *res)
{
	init_rwsem(&res->rwsem);
	res->fill_res_entry = fill_res_noop;
}

=======
#include "restrack.h"

/**
 * rdma_restrack_init() - initialize and allocate resource tracking
 * @dev:  IB device
 *
 * Return: 0 on success
 */
int rdma_restrack_init(struct ib_device *dev)
{
	struct rdma_restrack_root *rt;
	int i;

	dev->res = kcalloc(RDMA_RESTRACK_MAX, sizeof(*rt), GFP_KERNEL);
	if (!dev->res)
		return -ENOMEM;

	rt = dev->res;

	for (i = 0; i < RDMA_RESTRACK_MAX; i++)
		xa_init_flags(&rt[i].xa, XA_FLAGS_ALLOC);

	return 0;
}

>>>>>>> upstream/android-13
static const char *type2str(enum rdma_restrack_type type)
{
	static const char * const names[RDMA_RESTRACK_MAX] = {
		[RDMA_RESTRACK_PD] = "PD",
		[RDMA_RESTRACK_CQ] = "CQ",
		[RDMA_RESTRACK_QP] = "QP",
		[RDMA_RESTRACK_CM_ID] = "CM_ID",
		[RDMA_RESTRACK_MR] = "MR",
<<<<<<< HEAD
=======
		[RDMA_RESTRACK_CTX] = "CTX",
		[RDMA_RESTRACK_COUNTER] = "COUNTER",
		[RDMA_RESTRACK_SRQ] = "SRQ",
>>>>>>> upstream/android-13
	};

	return names[type];
};

<<<<<<< HEAD
void rdma_restrack_clean(struct rdma_restrack_root *res)
{
	struct rdma_restrack_entry *e;
	char buf[TASK_COMM_LEN];
	struct ib_device *dev;
	const char *owner;
	int bkt;

	if (hash_empty(res->hash))
		return;

	dev = container_of(res, struct ib_device, res);
	pr_err("restrack: %s", CUT_HERE);
	pr_err("restrack: BUG: RESTRACK detected leak of resources on %s\n",
	       dev->name);
	hash_for_each(res->hash, bkt, e, node) {
		if (rdma_is_kernel_res(e)) {
			owner = e->kern_name;
		} else {
			/*
			 * There is no need to call get_task_struct here,
			 * because we can be here only if there are more
			 * get_task_struct() call than put_task_struct().
			 */
			get_task_comm(buf, e->task);
			owner = buf;
		}

		pr_err("restrack: %s %s object allocated by %s is not freed\n",
		       rdma_is_kernel_res(e) ? "Kernel" : "User",
		       type2str(e->type), owner);
	}
	pr_err("restrack: %s", CUT_HERE);
}

int rdma_restrack_count(struct rdma_restrack_root *res,
			enum rdma_restrack_type type,
			struct pid_namespace *ns)
{
	struct rdma_restrack_entry *e;
	u32 cnt = 0;

	down_read(&res->rwsem);
	hash_for_each_possible(res->hash, e, node, type) {
		if (ns == &init_pid_ns ||
		    (!rdma_is_kernel_res(e) &&
		     ns == task_active_pid_ns(e->task)))
			cnt++;
	}
	up_read(&res->rwsem);
=======
/**
 * rdma_restrack_clean() - clean resource tracking
 * @dev:  IB device
 */
void rdma_restrack_clean(struct ib_device *dev)
{
	struct rdma_restrack_root *rt = dev->res;
	struct rdma_restrack_entry *e;
	char buf[TASK_COMM_LEN];
	bool found = false;
	const char *owner;
	int i;

	for (i = 0 ; i < RDMA_RESTRACK_MAX; i++) {
		struct xarray *xa = &dev->res[i].xa;

		if (!xa_empty(xa)) {
			unsigned long index;

			if (!found) {
				pr_err("restrack: %s", CUT_HERE);
				dev_err(&dev->dev, "BUG: RESTRACK detected leak of resources\n");
			}
			xa_for_each(xa, index, e) {
				if (rdma_is_kernel_res(e)) {
					owner = e->kern_name;
				} else {
					/*
					 * There is no need to call get_task_struct here,
					 * because we can be here only if there are more
					 * get_task_struct() call than put_task_struct().
					 */
					get_task_comm(buf, e->task);
					owner = buf;
				}

				pr_err("restrack: %s %s object allocated by %s is not freed\n",
				       rdma_is_kernel_res(e) ? "Kernel" :
							       "User",
				       type2str(e->type), owner);
			}
			found = true;
		}
		xa_destroy(xa);
	}
	if (found)
		pr_err("restrack: %s", CUT_HERE);

	kfree(rt);
}

/**
 * rdma_restrack_count() - the current usage of specific object
 * @dev:  IB device
 * @type: actual type of object to operate
 */
int rdma_restrack_count(struct ib_device *dev, enum rdma_restrack_type type)
{
	struct rdma_restrack_root *rt = &dev->res[type];
	struct rdma_restrack_entry *e;
	XA_STATE(xas, &rt->xa, 0);
	u32 cnt = 0;

	xa_lock(&rt->xa);
	xas_for_each(&xas, e, U32_MAX)
		cnt++;
	xa_unlock(&rt->xa);
>>>>>>> upstream/android-13
	return cnt;
}
EXPORT_SYMBOL(rdma_restrack_count);

<<<<<<< HEAD
static void set_kern_name(struct rdma_restrack_entry *res)
{
	struct ib_pd *pd;

	switch (res->type) {
	case RDMA_RESTRACK_QP:
		pd = container_of(res, struct ib_qp, res)->pd;
		if (!pd) {
			WARN_ONCE(true, "XRC QPs are not supported\n");
			/* Survive, despite the programmer's error */
			res->kern_name = " ";
		}
		break;
	case RDMA_RESTRACK_MR:
		pd = container_of(res, struct ib_mr, res)->pd;
		break;
	default:
		/* Other types set kern_name directly */
		pd = NULL;
		break;
	}

	if (pd)
		res->kern_name = pd->res.kern_name;
}

=======
>>>>>>> upstream/android-13
static struct ib_device *res_to_dev(struct rdma_restrack_entry *res)
{
	switch (res->type) {
	case RDMA_RESTRACK_PD:
		return container_of(res, struct ib_pd, res)->device;
	case RDMA_RESTRACK_CQ:
		return container_of(res, struct ib_cq, res)->device;
	case RDMA_RESTRACK_QP:
		return container_of(res, struct ib_qp, res)->device;
	case RDMA_RESTRACK_CM_ID:
		return container_of(res, struct rdma_id_private,
				    res)->id.device;
	case RDMA_RESTRACK_MR:
		return container_of(res, struct ib_mr, res)->device;
<<<<<<< HEAD
=======
	case RDMA_RESTRACK_CTX:
		return container_of(res, struct ib_ucontext, res)->device;
	case RDMA_RESTRACK_COUNTER:
		return container_of(res, struct rdma_counter, res)->device;
	case RDMA_RESTRACK_SRQ:
		return container_of(res, struct ib_srq, res)->device;
>>>>>>> upstream/android-13
	default:
		WARN_ONCE(true, "Wrong resource tracking type %u\n", res->type);
		return NULL;
	}
}

<<<<<<< HEAD
static bool res_is_user(struct rdma_restrack_entry *res)
{
	switch (res->type) {
	case RDMA_RESTRACK_PD:
		return container_of(res, struct ib_pd, res)->uobject;
	case RDMA_RESTRACK_CQ:
		return container_of(res, struct ib_cq, res)->uobject;
	case RDMA_RESTRACK_QP:
		return container_of(res, struct ib_qp, res)->uobject;
	case RDMA_RESTRACK_CM_ID:
		return !res->kern_name;
	case RDMA_RESTRACK_MR:
		return container_of(res, struct ib_mr, res)->pd->uobject;
	default:
		WARN_ONCE(true, "Wrong resource tracking type %u\n", res->type);
		return false;
	}
}

void rdma_restrack_add(struct rdma_restrack_entry *res)
{
	struct ib_device *dev = res_to_dev(res);
=======
/**
 * rdma_restrack_attach_task() - attach the task onto this resource,
 * valid for user space restrack entries.
 * @res:  resource entry
 * @task: the task to attach
 */
static void rdma_restrack_attach_task(struct rdma_restrack_entry *res,
				      struct task_struct *task)
{
	if (WARN_ON_ONCE(!task))
		return;

	if (res->task)
		put_task_struct(res->task);
	get_task_struct(task);
	res->task = task;
	res->user = true;
}

/**
 * rdma_restrack_set_name() - set the task for this resource
 * @res:  resource entry
 * @caller: kernel name, the current task will be used if the caller is NULL.
 */
void rdma_restrack_set_name(struct rdma_restrack_entry *res, const char *caller)
{
	if (caller) {
		res->kern_name = caller;
		return;
	}

	rdma_restrack_attach_task(res, current);
}
EXPORT_SYMBOL(rdma_restrack_set_name);

/**
 * rdma_restrack_parent_name() - set the restrack name properties based
 * on parent restrack
 * @dst: destination resource entry
 * @parent: parent resource entry
 */
void rdma_restrack_parent_name(struct rdma_restrack_entry *dst,
			       const struct rdma_restrack_entry *parent)
{
	if (rdma_is_kernel_res(parent))
		dst->kern_name = parent->kern_name;
	else
		rdma_restrack_attach_task(dst, parent->task);
}
EXPORT_SYMBOL(rdma_restrack_parent_name);

/**
 * rdma_restrack_new() - Initializes new restrack entry to allow _put() interface
 * to release memory in fully automatic way.
 * @res: Entry to initialize
 * @type: REstrack type
 */
void rdma_restrack_new(struct rdma_restrack_entry *res,
		       enum rdma_restrack_type type)
{
	kref_init(&res->kref);
	init_completion(&res->comp);
	res->type = type;
}
EXPORT_SYMBOL(rdma_restrack_new);

/**
 * rdma_restrack_add() - add object to the reource tracking database
 * @res:  resource entry
 */
void rdma_restrack_add(struct rdma_restrack_entry *res)
{
	struct ib_device *dev = res_to_dev(res);
	struct rdma_restrack_root *rt;
	int ret = 0;
>>>>>>> upstream/android-13

	if (!dev)
		return;

<<<<<<< HEAD
	if (res->type != RDMA_RESTRACK_CM_ID || !res_is_user(res))
		res->task = NULL;

	if (res_is_user(res)) {
		if (!res->task)
			rdma_restrack_set_task(res, current);
		res->kern_name = NULL;
	} else {
		set_kern_name(res);
	}

	kref_init(&res->kref);
	init_completion(&res->comp);
	res->valid = true;

	down_write(&dev->res.rwsem);
	hash_add(dev->res.hash, &res->node, res->type);
	up_write(&dev->res.rwsem);
=======
	if (res->no_track)
		goto out;

	rt = &dev->res[res->type];

	if (res->type == RDMA_RESTRACK_QP) {
		/* Special case to ensure that LQPN points to right QP */
		struct ib_qp *qp = container_of(res, struct ib_qp, res);

		WARN_ONCE(qp->qp_num >> 24 || qp->port >> 8,
			  "QP number 0x%0X and port 0x%0X", qp->qp_num,
			  qp->port);
		res->id = qp->qp_num;
		if (qp->qp_type == IB_QPT_SMI || qp->qp_type == IB_QPT_GSI)
			res->id |= qp->port << 24;
		ret = xa_insert(&rt->xa, res->id, res, GFP_KERNEL);
		if (ret)
			res->id = 0;
	} else if (res->type == RDMA_RESTRACK_COUNTER) {
		/* Special case to ensure that cntn points to right counter */
		struct rdma_counter *counter;

		counter = container_of(res, struct rdma_counter, res);
		ret = xa_insert(&rt->xa, counter->id, res, GFP_KERNEL);
		res->id = ret ? 0 : counter->id;
	} else {
		ret = xa_alloc_cyclic(&rt->xa, &res->id, res, xa_limit_32b,
				      &rt->next_id, GFP_KERNEL);
		ret = (ret < 0) ? ret : 0;
	}

out:
	if (!ret)
		res->valid = true;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(rdma_restrack_add);

int __must_check rdma_restrack_get(struct rdma_restrack_entry *res)
{
	return kref_get_unless_zero(&res->kref);
}
EXPORT_SYMBOL(rdma_restrack_get);

<<<<<<< HEAD
=======
/**
 * rdma_restrack_get_byid() - translate from ID to restrack object
 * @dev: IB device
 * @type: resource track type
 * @id: ID to take a look
 *
 * Return: Pointer to restrack entry or -ENOENT in case of error.
 */
struct rdma_restrack_entry *
rdma_restrack_get_byid(struct ib_device *dev,
		       enum rdma_restrack_type type, u32 id)
{
	struct rdma_restrack_root *rt = &dev->res[type];
	struct rdma_restrack_entry *res;

	xa_lock(&rt->xa);
	res = xa_load(&rt->xa, id);
	if (!res || !rdma_restrack_get(res))
		res = ERR_PTR(-ENOENT);
	xa_unlock(&rt->xa);

	return res;
}
EXPORT_SYMBOL(rdma_restrack_get_byid);

>>>>>>> upstream/android-13
static void restrack_release(struct kref *kref)
{
	struct rdma_restrack_entry *res;

	res = container_of(kref, struct rdma_restrack_entry, kref);
<<<<<<< HEAD
=======
	if (res->task) {
		put_task_struct(res->task);
		res->task = NULL;
	}
>>>>>>> upstream/android-13
	complete(&res->comp);
}

int rdma_restrack_put(struct rdma_restrack_entry *res)
{
	return kref_put(&res->kref, restrack_release);
}
EXPORT_SYMBOL(rdma_restrack_put);

<<<<<<< HEAD
void rdma_restrack_del(struct rdma_restrack_entry *res)
{
	struct ib_device *dev;

	if (!res->valid)
		goto out;

	dev = res_to_dev(res);
	if (!dev)
		return;

	rdma_restrack_put(res);

	wait_for_completion(&res->comp);

	down_write(&dev->res.rwsem);
	hash_del(&res->node);
	res->valid = false;
	up_write(&dev->res.rwsem);

out:
	if (res->task) {
		put_task_struct(res->task);
		res->task = NULL;
	}
=======
/**
 * rdma_restrack_del() - delete object from the reource tracking database
 * @res:  resource entry
 */
void rdma_restrack_del(struct rdma_restrack_entry *res)
{
	struct rdma_restrack_entry *old;
	struct rdma_restrack_root *rt;
	struct ib_device *dev;

	if (!res->valid) {
		if (res->task) {
			put_task_struct(res->task);
			res->task = NULL;
		}
		return;
	}

	if (res->no_track)
		goto out;

	dev = res_to_dev(res);
	if (WARN_ON(!dev))
		return;

	rt = &dev->res[res->type];

	old = xa_erase(&rt->xa, res->id);
	if (res->type == RDMA_RESTRACK_MR)
		return;
	WARN_ON(old != res);

out:
	res->valid = false;
	rdma_restrack_put(res);
	wait_for_completion(&res->comp);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(rdma_restrack_del);
