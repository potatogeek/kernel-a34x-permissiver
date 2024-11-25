<<<<<<< HEAD
/*
 * Copyright (c) 2013-2015, Mellanox Technologies. All rights reserved.
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

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/mlx5/driver.h>
#include <linux/mlx5/cmd.h>
=======
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2013-2021, Mellanox Technologies inc.  All rights reserved.
 */

#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <linux/module.h>
#include <linux/mlx5/driver.h>
#include <linux/mlx5/vport.h>
#include <linux/mlx5/eq.h>
>>>>>>> upstream/android-13
#ifdef CONFIG_RFS_ACCEL
#include <linux/cpu_rmap.h>
#endif
#include "mlx5_core.h"
<<<<<<< HEAD
=======
#include "lib/eq.h"
>>>>>>> upstream/android-13
#include "fpga/core.h"
#include "eswitch.h"
#include "lib/clock.h"
#include "diag/fw_tracer.h"
<<<<<<< HEAD

enum {
	MLX5_EQE_SIZE		= sizeof(struct mlx5_eqe),
=======
#include "mlx5_irq.h"

enum {
>>>>>>> upstream/android-13
	MLX5_EQE_OWNER_INIT_VAL	= 0x1,
};

enum {
	MLX5_EQ_STATE_ARMED		= 0x9,
	MLX5_EQ_STATE_FIRED		= 0xa,
	MLX5_EQ_STATE_ALWAYS_ARMED	= 0xb,
};

enum {
<<<<<<< HEAD
	MLX5_NUM_SPARE_EQE	= 0x80,
	MLX5_NUM_ASYNC_EQE	= 0x1000,
	MLX5_NUM_CMD_EQE	= 32,
	MLX5_NUM_PF_DRAIN	= 64,
};

enum {
	MLX5_EQ_DOORBEL_OFFSET	= 0x40,
=======
	MLX5_EQ_DOORBEL_OFFSET	= 0x40,
};

/* budget must be smaller than MLX5_NUM_SPARE_EQE to guarantee that we update
 * the ci before we polled all the entries in the EQ. MLX5_NUM_SPARE_EQE is
 * used to set the EQ size, budget must be smaller than the EQ size.
 */
enum {
	MLX5_EQ_POLLING_BUDGET	= 128,
};

static_assert(MLX5_EQ_POLLING_BUDGET <= MLX5_NUM_SPARE_EQE);

struct mlx5_eq_table {
	struct list_head        comp_eqs_list;
	struct mlx5_eq_async    pages_eq;
	struct mlx5_eq_async    cmd_eq;
	struct mlx5_eq_async    async_eq;

	struct atomic_notifier_head nh[MLX5_EVENT_TYPE_MAX];

	/* Since CQ DB is stored in async_eq */
	struct mlx5_nb          cq_err_nb;

	struct mutex            lock; /* sync async eqs creations */
	int			num_comp_eqs;
	struct mlx5_irq_table	*irq_table;
#ifdef CONFIG_RFS_ACCEL
	struct cpu_rmap		*rmap;
#endif
>>>>>>> upstream/android-13
};

#define MLX5_ASYNC_EVENT_MASK ((1ull << MLX5_EVENT_TYPE_PATH_MIG)	    | \
			       (1ull << MLX5_EVENT_TYPE_COMM_EST)	    | \
			       (1ull << MLX5_EVENT_TYPE_SQ_DRAINED)	    | \
			       (1ull << MLX5_EVENT_TYPE_CQ_ERROR)	    | \
			       (1ull << MLX5_EVENT_TYPE_WQ_CATAS_ERROR)	    | \
			       (1ull << MLX5_EVENT_TYPE_PATH_MIG_FAILED)    | \
			       (1ull << MLX5_EVENT_TYPE_WQ_INVAL_REQ_ERROR) | \
			       (1ull << MLX5_EVENT_TYPE_WQ_ACCESS_ERROR)    | \
			       (1ull << MLX5_EVENT_TYPE_PORT_CHANGE)	    | \
			       (1ull << MLX5_EVENT_TYPE_SRQ_CATAS_ERROR)    | \
			       (1ull << MLX5_EVENT_TYPE_SRQ_LAST_WQE)	    | \
			       (1ull << MLX5_EVENT_TYPE_SRQ_RQ_LIMIT))

<<<<<<< HEAD
struct map_eq_in {
	u64	mask;
	u32	reserved;
	u32	unmap_eqn;
};

struct cre_des_eq {
	u8	reserved[15];
	u8	eqn;
};

static int mlx5_cmd_destroy_eq(struct mlx5_core_dev *dev, u8 eqn)
{
	u32 out[MLX5_ST_SZ_DW(destroy_eq_out)] = {0};
	u32 in[MLX5_ST_SZ_DW(destroy_eq_in)]   = {0};

	MLX5_SET(destroy_eq_in, in, opcode, MLX5_CMD_OP_DESTROY_EQ);
	MLX5_SET(destroy_eq_in, in, eq_number, eqn);
	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
}

static struct mlx5_eqe *get_eqe(struct mlx5_eq *eq, u32 entry)
{
	return mlx5_buf_offset(&eq->buf, entry * MLX5_EQE_SIZE);
}

static struct mlx5_eqe *next_eqe_sw(struct mlx5_eq *eq)
{
	struct mlx5_eqe *eqe = get_eqe(eq, eq->cons_index & (eq->nent - 1));

	return ((eqe->owner & 1) ^ !!(eq->cons_index & eq->nent)) ? NULL : eqe;
}

static const char *eqe_type_str(u8 type)
{
	switch (type) {
	case MLX5_EVENT_TYPE_COMP:
		return "MLX5_EVENT_TYPE_COMP";
	case MLX5_EVENT_TYPE_PATH_MIG:
		return "MLX5_EVENT_TYPE_PATH_MIG";
	case MLX5_EVENT_TYPE_COMM_EST:
		return "MLX5_EVENT_TYPE_COMM_EST";
	case MLX5_EVENT_TYPE_SQ_DRAINED:
		return "MLX5_EVENT_TYPE_SQ_DRAINED";
	case MLX5_EVENT_TYPE_SRQ_LAST_WQE:
		return "MLX5_EVENT_TYPE_SRQ_LAST_WQE";
	case MLX5_EVENT_TYPE_SRQ_RQ_LIMIT:
		return "MLX5_EVENT_TYPE_SRQ_RQ_LIMIT";
	case MLX5_EVENT_TYPE_CQ_ERROR:
		return "MLX5_EVENT_TYPE_CQ_ERROR";
	case MLX5_EVENT_TYPE_WQ_CATAS_ERROR:
		return "MLX5_EVENT_TYPE_WQ_CATAS_ERROR";
	case MLX5_EVENT_TYPE_PATH_MIG_FAILED:
		return "MLX5_EVENT_TYPE_PATH_MIG_FAILED";
	case MLX5_EVENT_TYPE_WQ_INVAL_REQ_ERROR:
		return "MLX5_EVENT_TYPE_WQ_INVAL_REQ_ERROR";
	case MLX5_EVENT_TYPE_WQ_ACCESS_ERROR:
		return "MLX5_EVENT_TYPE_WQ_ACCESS_ERROR";
	case MLX5_EVENT_TYPE_SRQ_CATAS_ERROR:
		return "MLX5_EVENT_TYPE_SRQ_CATAS_ERROR";
	case MLX5_EVENT_TYPE_INTERNAL_ERROR:
		return "MLX5_EVENT_TYPE_INTERNAL_ERROR";
	case MLX5_EVENT_TYPE_PORT_CHANGE:
		return "MLX5_EVENT_TYPE_PORT_CHANGE";
	case MLX5_EVENT_TYPE_GPIO_EVENT:
		return "MLX5_EVENT_TYPE_GPIO_EVENT";
	case MLX5_EVENT_TYPE_PORT_MODULE_EVENT:
		return "MLX5_EVENT_TYPE_PORT_MODULE_EVENT";
	case MLX5_EVENT_TYPE_TEMP_WARN_EVENT:
		return "MLX5_EVENT_TYPE_TEMP_WARN_EVENT";
	case MLX5_EVENT_TYPE_REMOTE_CONFIG:
		return "MLX5_EVENT_TYPE_REMOTE_CONFIG";
	case MLX5_EVENT_TYPE_DB_BF_CONGESTION:
		return "MLX5_EVENT_TYPE_DB_BF_CONGESTION";
	case MLX5_EVENT_TYPE_STALL_EVENT:
		return "MLX5_EVENT_TYPE_STALL_EVENT";
	case MLX5_EVENT_TYPE_CMD:
		return "MLX5_EVENT_TYPE_CMD";
	case MLX5_EVENT_TYPE_PAGE_REQUEST:
		return "MLX5_EVENT_TYPE_PAGE_REQUEST";
	case MLX5_EVENT_TYPE_PAGE_FAULT:
		return "MLX5_EVENT_TYPE_PAGE_FAULT";
	case MLX5_EVENT_TYPE_PPS_EVENT:
		return "MLX5_EVENT_TYPE_PPS_EVENT";
	case MLX5_EVENT_TYPE_NIC_VPORT_CHANGE:
		return "MLX5_EVENT_TYPE_NIC_VPORT_CHANGE";
	case MLX5_EVENT_TYPE_FPGA_ERROR:
		return "MLX5_EVENT_TYPE_FPGA_ERROR";
	case MLX5_EVENT_TYPE_FPGA_QP_ERROR:
		return "MLX5_EVENT_TYPE_FPGA_QP_ERROR";
	case MLX5_EVENT_TYPE_GENERAL_EVENT:
		return "MLX5_EVENT_TYPE_GENERAL_EVENT";
	case MLX5_EVENT_TYPE_DEVICE_TRACER:
		return "MLX5_EVENT_TYPE_DEVICE_TRACER";
	default:
		return "Unrecognized event";
	}
}

static enum mlx5_dev_event port_subtype_event(u8 subtype)
{
	switch (subtype) {
	case MLX5_PORT_CHANGE_SUBTYPE_DOWN:
		return MLX5_DEV_EVENT_PORT_DOWN;
	case MLX5_PORT_CHANGE_SUBTYPE_ACTIVE:
		return MLX5_DEV_EVENT_PORT_UP;
	case MLX5_PORT_CHANGE_SUBTYPE_INITIALIZED:
		return MLX5_DEV_EVENT_PORT_INITIALIZED;
	case MLX5_PORT_CHANGE_SUBTYPE_LID:
		return MLX5_DEV_EVENT_LID_CHANGE;
	case MLX5_PORT_CHANGE_SUBTYPE_PKEY:
		return MLX5_DEV_EVENT_PKEY_CHANGE;
	case MLX5_PORT_CHANGE_SUBTYPE_GUID:
		return MLX5_DEV_EVENT_GUID_CHANGE;
	case MLX5_PORT_CHANGE_SUBTYPE_CLIENT_REREG:
		return MLX5_DEV_EVENT_CLIENT_REREG;
	}
	return -1;
}

static void eq_update_ci(struct mlx5_eq *eq, int arm)
{
	__be32 __iomem *addr = eq->doorbell + (arm ? 0 : 2);
	u32 val = (eq->cons_index & 0xffffff) | (eq->eqn << 24);

	__raw_writel((__force u32)cpu_to_be32(val), addr);
	/* We still want ordering, just not swabbing, so add a barrier */
	mb();
}

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
static void eqe_pf_action(struct work_struct *work)
{
	struct mlx5_pagefault *pfault = container_of(work,
						     struct mlx5_pagefault,
						     work);
	struct mlx5_eq *eq = pfault->eq;

	mlx5_core_page_fault(eq->dev, pfault);
	mempool_free(pfault, eq->pf_ctx.pool);
}

static void eq_pf_process(struct mlx5_eq *eq)
{
	struct mlx5_core_dev *dev = eq->dev;
	struct mlx5_eqe_page_fault *pf_eqe;
	struct mlx5_pagefault *pfault;
	struct mlx5_eqe *eqe;
	int set_ci = 0;

	while ((eqe = next_eqe_sw(eq))) {
		pfault = mempool_alloc(eq->pf_ctx.pool, GFP_ATOMIC);
		if (!pfault) {
			schedule_work(&eq->pf_ctx.work);
			break;
		}

		dma_rmb();
		pf_eqe = &eqe->data.page_fault;
		pfault->event_subtype = eqe->sub_type;
		pfault->bytes_committed = be32_to_cpu(pf_eqe->bytes_committed);

		mlx5_core_dbg(dev,
			      "PAGE_FAULT: subtype: 0x%02x, bytes_committed: 0x%06x\n",
			      eqe->sub_type, pfault->bytes_committed);

		switch (eqe->sub_type) {
		case MLX5_PFAULT_SUBTYPE_RDMA:
			/* RDMA based event */
			pfault->type =
				be32_to_cpu(pf_eqe->rdma.pftype_token) >> 24;
			pfault->token =
				be32_to_cpu(pf_eqe->rdma.pftype_token) &
				MLX5_24BIT_MASK;
			pfault->rdma.r_key =
				be32_to_cpu(pf_eqe->rdma.r_key);
			pfault->rdma.packet_size =
				be16_to_cpu(pf_eqe->rdma.packet_length);
			pfault->rdma.rdma_op_len =
				be32_to_cpu(pf_eqe->rdma.rdma_op_len);
			pfault->rdma.rdma_va =
				be64_to_cpu(pf_eqe->rdma.rdma_va);
			mlx5_core_dbg(dev,
				      "PAGE_FAULT: type:0x%x, token: 0x%06x, r_key: 0x%08x\n",
				      pfault->type, pfault->token,
				      pfault->rdma.r_key);
			mlx5_core_dbg(dev,
				      "PAGE_FAULT: rdma_op_len: 0x%08x, rdma_va: 0x%016llx\n",
				      pfault->rdma.rdma_op_len,
				      pfault->rdma.rdma_va);
			break;

		case MLX5_PFAULT_SUBTYPE_WQE:
			/* WQE based event */
			pfault->type =
				(be32_to_cpu(pf_eqe->wqe.pftype_wq) >> 24) & 0x7;
			pfault->token =
				be32_to_cpu(pf_eqe->wqe.token);
			pfault->wqe.wq_num =
				be32_to_cpu(pf_eqe->wqe.pftype_wq) &
				MLX5_24BIT_MASK;
			pfault->wqe.wqe_index =
				be16_to_cpu(pf_eqe->wqe.wqe_index);
			pfault->wqe.packet_size =
				be16_to_cpu(pf_eqe->wqe.packet_length);
			mlx5_core_dbg(dev,
				      "PAGE_FAULT: type:0x%x, token: 0x%06x, wq_num: 0x%06x, wqe_index: 0x%04x\n",
				      pfault->type, pfault->token,
				      pfault->wqe.wq_num,
				      pfault->wqe.wqe_index);
			break;

		default:
			mlx5_core_warn(dev,
				       "Unsupported page fault event sub-type: 0x%02hhx\n",
				       eqe->sub_type);
			/* Unsupported page faults should still be
			 * resolved by the page fault handler
			 */
		}

		pfault->eq = eq;
		INIT_WORK(&pfault->work, eqe_pf_action);
		queue_work(eq->pf_ctx.wq, &pfault->work);

		++eq->cons_index;
		++set_ci;

		if (unlikely(set_ci >= MLX5_NUM_SPARE_EQE)) {
			eq_update_ci(eq, 0);
			set_ci = 0;
		}
	}

	eq_update_ci(eq, 1);
}

static irqreturn_t mlx5_eq_pf_int(int irq, void *eq_ptr)
{
	struct mlx5_eq *eq = eq_ptr;
	unsigned long flags;

	if (spin_trylock_irqsave(&eq->pf_ctx.lock, flags)) {
		eq_pf_process(eq);
		spin_unlock_irqrestore(&eq->pf_ctx.lock, flags);
	} else {
		schedule_work(&eq->pf_ctx.work);
	}

	return IRQ_HANDLED;
}

/* mempool_refill() was proposed but unfortunately wasn't accepted
 * http://lkml.iu.edu/hypermail/linux/kernel/1512.1/05073.html
 * Chip workaround.
 */
static void mempool_refill(mempool_t *pool)
{
	while (pool->curr_nr < pool->min_nr)
		mempool_free(mempool_alloc(pool, GFP_KERNEL), pool);
}

static void eq_pf_action(struct work_struct *work)
{
	struct mlx5_eq *eq = container_of(work, struct mlx5_eq, pf_ctx.work);

	mempool_refill(eq->pf_ctx.pool);

	spin_lock_irq(&eq->pf_ctx.lock);
	eq_pf_process(eq);
	spin_unlock_irq(&eq->pf_ctx.lock);
}

static int init_pf_ctx(struct mlx5_eq_pagefault *pf_ctx, const char *name)
{
	spin_lock_init(&pf_ctx->lock);
	INIT_WORK(&pf_ctx->work, eq_pf_action);

	pf_ctx->wq = alloc_ordered_workqueue(name,
					     WQ_MEM_RECLAIM);
	if (!pf_ctx->wq)
		return -ENOMEM;

	pf_ctx->pool = mempool_create_kmalloc_pool
		(MLX5_NUM_PF_DRAIN, sizeof(struct mlx5_pagefault));
	if (!pf_ctx->pool)
		goto err_wq;

	return 0;
err_wq:
	destroy_workqueue(pf_ctx->wq);
	return -ENOMEM;
}

int mlx5_core_page_fault_resume(struct mlx5_core_dev *dev, u32 token,
				u32 wq_num, u8 type, int error)
{
	u32 out[MLX5_ST_SZ_DW(page_fault_resume_out)] = {0};
	u32 in[MLX5_ST_SZ_DW(page_fault_resume_in)]   = {0};

	MLX5_SET(page_fault_resume_in, in, opcode,
		 MLX5_CMD_OP_PAGE_FAULT_RESUME);
	MLX5_SET(page_fault_resume_in, in, error, !!error);
	MLX5_SET(page_fault_resume_in, in, page_fault_type, type);
	MLX5_SET(page_fault_resume_in, in, wq_number, wq_num);
	MLX5_SET(page_fault_resume_in, in, token, token);

	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
}
EXPORT_SYMBOL_GPL(mlx5_core_page_fault_resume);
#endif

static void general_event_handler(struct mlx5_core_dev *dev,
				  struct mlx5_eqe *eqe)
{
	switch (eqe->sub_type) {
	case MLX5_GENERAL_SUBTYPE_DELAY_DROP_TIMEOUT:
		if (dev->event)
			dev->event(dev, MLX5_DEV_EVENT_DELAY_DROP_TIMEOUT, 0);
		break;
	default:
		mlx5_core_dbg(dev, "General event with unrecognized subtype: sub_type %d\n",
			      eqe->sub_type);
	}
}

static void mlx5_temp_warning_event(struct mlx5_core_dev *dev,
				    struct mlx5_eqe *eqe)
{
	u64 value_lsb;
	u64 value_msb;

	value_lsb = be64_to_cpu(eqe->data.temp_warning.sensor_warning_lsb);
	value_msb = be64_to_cpu(eqe->data.temp_warning.sensor_warning_msb);

	mlx5_core_warn(dev,
		       "High temperature on sensors with bit set %llx %llx",
		       value_msb, value_lsb);
=======
static int mlx5_cmd_destroy_eq(struct mlx5_core_dev *dev, u8 eqn)
{
	u32 in[MLX5_ST_SZ_DW(destroy_eq_in)] = {};

	MLX5_SET(destroy_eq_in, in, opcode, MLX5_CMD_OP_DESTROY_EQ);
	MLX5_SET(destroy_eq_in, in, eq_number, eqn);
	return mlx5_cmd_exec_in(dev, destroy_eq, in);
>>>>>>> upstream/android-13
}

/* caller must eventually call mlx5_cq_put on the returned cq */
static struct mlx5_core_cq *mlx5_eq_cq_get(struct mlx5_eq *eq, u32 cqn)
{
	struct mlx5_cq_table *table = &eq->cq_table;
	struct mlx5_core_cq *cq = NULL;

<<<<<<< HEAD
	spin_lock(&table->lock);
	cq = radix_tree_lookup(&table->tree, cqn);
	if (likely(cq))
		mlx5_cq_hold(cq);
	spin_unlock(&table->lock);
=======
	rcu_read_lock();
	cq = radix_tree_lookup(&table->tree, cqn);
	if (likely(cq))
		mlx5_cq_hold(cq);
	rcu_read_unlock();
>>>>>>> upstream/android-13

	return cq;
}

<<<<<<< HEAD
static void mlx5_eq_cq_completion(struct mlx5_eq *eq, u32 cqn)
{
	struct mlx5_core_cq *cq = mlx5_eq_cq_get(eq, cqn);

	if (unlikely(!cq)) {
		mlx5_core_warn(eq->dev, "Completion event for bogus CQ 0x%x\n", cqn);
		return;
	}

	++cq->arm_sn;

	cq->comp(cq);

	mlx5_cq_put(cq);
}

static void mlx5_eq_cq_event(struct mlx5_eq *eq, u32 cqn, int event_type)
{
	struct mlx5_core_cq *cq = mlx5_eq_cq_get(eq, cqn);

	if (unlikely(!cq)) {
		mlx5_core_warn(eq->dev, "Async event for bogus CQ 0x%x\n", cqn);
		return;
	}

	cq->event(cq, event_type);

	mlx5_cq_put(cq);
}

static irqreturn_t mlx5_eq_int(int irq, void *eq_ptr)
{
	struct mlx5_eq *eq = eq_ptr;
	struct mlx5_core_dev *dev = eq->dev;
	struct mlx5_eqe *eqe;
	int set_ci = 0;
	u32 cqn = -1;
	u32 rsn;
	u8 port;

	while ((eqe = next_eqe_sw(eq))) {
		/*
		 * Make sure we read EQ entry contents after we've
		 * checked the ownership bit.
		 */
		dma_rmb();

		mlx5_core_dbg(eq->dev, "eqn %d, eqe type %s\n",
			      eq->eqn, eqe_type_str(eqe->type));
		switch (eqe->type) {
		case MLX5_EVENT_TYPE_COMP:
			cqn = be32_to_cpu(eqe->data.comp.cqn) & 0xffffff;
			mlx5_eq_cq_completion(eq, cqn);
			break;
		case MLX5_EVENT_TYPE_DCT_DRAINED:
			rsn = be32_to_cpu(eqe->data.dct.dctn) & 0xffffff;
			rsn |= (MLX5_RES_DCT << MLX5_USER_INDEX_LEN);
			mlx5_rsc_event(dev, rsn, eqe->type);
			break;
		case MLX5_EVENT_TYPE_PATH_MIG:
		case MLX5_EVENT_TYPE_COMM_EST:
		case MLX5_EVENT_TYPE_SQ_DRAINED:
		case MLX5_EVENT_TYPE_SRQ_LAST_WQE:
		case MLX5_EVENT_TYPE_WQ_CATAS_ERROR:
		case MLX5_EVENT_TYPE_PATH_MIG_FAILED:
		case MLX5_EVENT_TYPE_WQ_INVAL_REQ_ERROR:
		case MLX5_EVENT_TYPE_WQ_ACCESS_ERROR:
			rsn = be32_to_cpu(eqe->data.qp_srq.qp_srq_n) & 0xffffff;
			rsn |= (eqe->data.qp_srq.type << MLX5_USER_INDEX_LEN);
			mlx5_core_dbg(dev, "event %s(%d) arrived on resource 0x%x\n",
				      eqe_type_str(eqe->type), eqe->type, rsn);
			mlx5_rsc_event(dev, rsn, eqe->type);
			break;

		case MLX5_EVENT_TYPE_SRQ_RQ_LIMIT:
		case MLX5_EVENT_TYPE_SRQ_CATAS_ERROR:
			rsn = be32_to_cpu(eqe->data.qp_srq.qp_srq_n) & 0xffffff;
			mlx5_core_dbg(dev, "SRQ event %s(%d): srqn 0x%x\n",
				      eqe_type_str(eqe->type), eqe->type, rsn);
			mlx5_srq_event(dev, rsn, eqe->type);
			break;

		case MLX5_EVENT_TYPE_CMD:
			mlx5_cmd_comp_handler(dev, be32_to_cpu(eqe->data.cmd.vector), false);
			break;

		case MLX5_EVENT_TYPE_PORT_CHANGE:
			port = (eqe->data.port.port >> 4) & 0xf;
			switch (eqe->sub_type) {
			case MLX5_PORT_CHANGE_SUBTYPE_DOWN:
			case MLX5_PORT_CHANGE_SUBTYPE_ACTIVE:
			case MLX5_PORT_CHANGE_SUBTYPE_LID:
			case MLX5_PORT_CHANGE_SUBTYPE_PKEY:
			case MLX5_PORT_CHANGE_SUBTYPE_GUID:
			case MLX5_PORT_CHANGE_SUBTYPE_CLIENT_REREG:
			case MLX5_PORT_CHANGE_SUBTYPE_INITIALIZED:
				if (dev->event)
					dev->event(dev, port_subtype_event(eqe->sub_type),
						   (unsigned long)port);
				break;
			default:
				mlx5_core_warn(dev, "Port event with unrecognized subtype: port %d, sub_type %d\n",
					       port, eqe->sub_type);
			}
			break;
		case MLX5_EVENT_TYPE_CQ_ERROR:
			cqn = be32_to_cpu(eqe->data.cq_err.cqn) & 0xffffff;
			mlx5_core_warn(dev, "CQ error on CQN 0x%x, syndrome 0x%x\n",
				       cqn, eqe->data.cq_err.syndrome);
			mlx5_eq_cq_event(eq, cqn, eqe->type);
			break;

		case MLX5_EVENT_TYPE_PAGE_REQUEST:
			{
				u16 func_id = be16_to_cpu(eqe->data.req_pages.func_id);
				s32 npages = be32_to_cpu(eqe->data.req_pages.num_pages);

				mlx5_core_dbg(dev, "page request for func 0x%x, npages %d\n",
					      func_id, npages);
				mlx5_core_req_pages_handler(dev, func_id, npages);
			}
			break;

		case MLX5_EVENT_TYPE_NIC_VPORT_CHANGE:
			mlx5_eswitch_vport_event(dev->priv.eswitch, eqe);
			break;

		case MLX5_EVENT_TYPE_PORT_MODULE_EVENT:
			mlx5_port_module_event(dev, eqe);
			break;

		case MLX5_EVENT_TYPE_PPS_EVENT:
			mlx5_pps_event(dev, eqe);
			break;

		case MLX5_EVENT_TYPE_FPGA_ERROR:
		case MLX5_EVENT_TYPE_FPGA_QP_ERROR:
			mlx5_fpga_event(dev, eqe->type, &eqe->data.raw);
			break;

		case MLX5_EVENT_TYPE_TEMP_WARN_EVENT:
			mlx5_temp_warning_event(dev, eqe);
			break;

		case MLX5_EVENT_TYPE_GENERAL_EVENT:
			general_event_handler(dev, eqe);
			break;

		case MLX5_EVENT_TYPE_DEVICE_TRACER:
			mlx5_fw_tracer_event(dev, eqe);
			break;

		default:
			mlx5_core_warn(dev, "Unhandled event 0x%x on EQ 0x%x\n",
				       eqe->type, eq->eqn);
			break;
		}

		++eq->cons_index;
		++set_ci;

		/* The HCA will think the queue has overflowed if we
		 * don't tell it we've been processing events.  We
		 * create our EQs with MLX5_NUM_SPARE_EQE extra
		 * entries, so we must update our consumer index at
		 * least that often.
		 */
		if (unlikely(set_ci >= MLX5_NUM_SPARE_EQE)) {
			eq_update_ci(eq, 0);
			set_ci = 0;
		}
	}

	eq_update_ci(eq, 1);

	if (cqn != -1)
		tasklet_schedule(&eq->tasklet_ctx.task);

	return IRQ_HANDLED;
=======
static int mlx5_eq_comp_int(struct notifier_block *nb,
			    __always_unused unsigned long action,
			    __always_unused void *data)
{
	struct mlx5_eq_comp *eq_comp =
		container_of(nb, struct mlx5_eq_comp, irq_nb);
	struct mlx5_eq *eq = &eq_comp->core;
	struct mlx5_eqe *eqe;
	int num_eqes = 0;
	u32 cqn = -1;

	eqe = next_eqe_sw(eq);
	if (!eqe)
		goto out;

	do {
		struct mlx5_core_cq *cq;

		/* Make sure we read EQ entry contents after we've
		 * checked the ownership bit.
		 */
		dma_rmb();
		/* Assume (eqe->type) is always MLX5_EVENT_TYPE_COMP */
		cqn = be32_to_cpu(eqe->data.comp.cqn) & 0xffffff;

		cq = mlx5_eq_cq_get(eq, cqn);
		if (likely(cq)) {
			++cq->arm_sn;
			cq->comp(cq, eqe);
			mlx5_cq_put(cq);
		} else {
			dev_dbg_ratelimited(eq->dev->device,
					    "Completion event for bogus CQ 0x%x\n", cqn);
		}

		++eq->cons_index;

	} while ((++num_eqes < MLX5_EQ_POLLING_BUDGET) && (eqe = next_eqe_sw(eq)));

out:
	eq_update_ci(eq, 1);

	if (cqn != -1)
		tasklet_schedule(&eq_comp->tasklet_ctx.task);

	return 0;
>>>>>>> upstream/android-13
}

/* Some architectures don't latch interrupts when they are disabled, so using
 * mlx5_eq_poll_irq_disabled could end up losing interrupts while trying to
 * avoid losing them.  It is not recommended to use it, unless this is the last
 * resort.
 */
<<<<<<< HEAD
u32 mlx5_eq_poll_irq_disabled(struct mlx5_eq *eq)
{
	u32 count_eqe;

	disable_irq(eq->irqn);
	count_eqe = eq->cons_index;
	mlx5_eq_int(eq->irqn, eq);
	count_eqe = eq->cons_index - count_eqe;
	enable_irq(eq->irqn);
=======
u32 mlx5_eq_poll_irq_disabled(struct mlx5_eq_comp *eq)
{
	u32 count_eqe;

	disable_irq(eq->core.irqn);
	count_eqe = eq->core.cons_index;
	mlx5_eq_comp_int(&eq->irq_nb, 0, NULL);
	count_eqe = eq->core.cons_index - count_eqe;
	enable_irq(eq->core.irqn);
>>>>>>> upstream/android-13

	return count_eqe;
}

<<<<<<< HEAD
=======
static void mlx5_eq_async_int_lock(struct mlx5_eq_async *eq, bool recovery,
				   unsigned long *flags)
	__acquires(&eq->lock)
{
	if (!recovery)
		spin_lock(&eq->lock);
	else
		spin_lock_irqsave(&eq->lock, *flags);
}

static void mlx5_eq_async_int_unlock(struct mlx5_eq_async *eq, bool recovery,
				     unsigned long *flags)
	__releases(&eq->lock)
{
	if (!recovery)
		spin_unlock(&eq->lock);
	else
		spin_unlock_irqrestore(&eq->lock, *flags);
}

enum async_eq_nb_action {
	ASYNC_EQ_IRQ_HANDLER = 0,
	ASYNC_EQ_RECOVER = 1,
};

static int mlx5_eq_async_int(struct notifier_block *nb,
			     unsigned long action, void *data)
{
	struct mlx5_eq_async *eq_async =
		container_of(nb, struct mlx5_eq_async, irq_nb);
	struct mlx5_eq *eq = &eq_async->core;
	struct mlx5_eq_table *eqt;
	struct mlx5_core_dev *dev;
	struct mlx5_eqe *eqe;
	unsigned long flags;
	int num_eqes = 0;
	bool recovery;

	dev = eq->dev;
	eqt = dev->priv.eq_table;

	recovery = action == ASYNC_EQ_RECOVER;
	mlx5_eq_async_int_lock(eq_async, recovery, &flags);

	eqe = next_eqe_sw(eq);
	if (!eqe)
		goto out;

	do {
		/*
		 * Make sure we read EQ entry contents after we've
		 * checked the ownership bit.
		 */
		dma_rmb();

		atomic_notifier_call_chain(&eqt->nh[eqe->type], eqe->type, eqe);
		atomic_notifier_call_chain(&eqt->nh[MLX5_EVENT_TYPE_NOTIFY_ANY], eqe->type, eqe);

		++eq->cons_index;

	} while ((++num_eqes < MLX5_EQ_POLLING_BUDGET) && (eqe = next_eqe_sw(eq)));

out:
	eq_update_ci(eq, 1);
	mlx5_eq_async_int_unlock(eq_async, recovery, &flags);

	return unlikely(recovery) ? num_eqes : 0;
}

void mlx5_cmd_eq_recover(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_async *eq = &dev->priv.eq_table->cmd_eq;
	int eqes;

	eqes = mlx5_eq_async_int(&eq->irq_nb, ASYNC_EQ_RECOVER, NULL);
	if (eqes)
		mlx5_core_warn(dev, "Recovered %d EQEs on cmd_eq\n", eqes);
}

>>>>>>> upstream/android-13
static void init_eq_buf(struct mlx5_eq *eq)
{
	struct mlx5_eqe *eqe;
	int i;

<<<<<<< HEAD
	for (i = 0; i < eq->nent; i++) {
=======
	for (i = 0; i < eq_get_size(eq); i++) {
>>>>>>> upstream/android-13
		eqe = get_eqe(eq, i);
		eqe->owner = MLX5_EQE_OWNER_INIT_VAL;
	}
}

<<<<<<< HEAD
int mlx5_create_map_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq, u8 vecidx,
		       int nent, u64 mask, const char *name,
		       enum mlx5_eq_type type)
{
	struct mlx5_cq_table *cq_table = &eq->cq_table;
	u32 out[MLX5_ST_SZ_DW(create_eq_out)] = {0};
	struct mlx5_priv *priv = &dev->priv;
	irq_handler_t handler;
=======
static int
create_map_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq,
	      struct mlx5_eq_param *param)
{
	u8 log_eq_size = order_base_2(param->nent + MLX5_NUM_SPARE_EQE);
	struct mlx5_cq_table *cq_table = &eq->cq_table;
	u32 out[MLX5_ST_SZ_DW(create_eq_out)] = {0};
	u8 log_eq_stride = ilog2(MLX5_EQE_SIZE);
	struct mlx5_priv *priv = &dev->priv;
	u16 vecidx = param->irq_index;
>>>>>>> upstream/android-13
	__be64 *pas;
	void *eqc;
	int inlen;
	u32 *in;
	int err;
<<<<<<< HEAD
=======
	int i;
>>>>>>> upstream/android-13

	/* Init CQ table */
	memset(cq_table, 0, sizeof(*cq_table));
	spin_lock_init(&cq_table->lock);
	INIT_RADIX_TREE(&cq_table->tree, GFP_ATOMIC);

<<<<<<< HEAD
	eq->type = type;
	eq->nent = roundup_pow_of_two(nent + MLX5_NUM_SPARE_EQE);
	eq->cons_index = 0;
	err = mlx5_buf_alloc(dev, eq->nent * MLX5_EQE_SIZE, &eq->buf);
	if (err)
		return err;

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	if (type == MLX5_EQ_TYPE_PF)
		handler = mlx5_eq_pf_int;
	else
#endif
		handler = mlx5_eq_int;

	init_eq_buf(eq);

	inlen = MLX5_ST_SZ_BYTES(create_eq_in) +
		MLX5_FLD_SZ_BYTES(create_eq_in, pas[0]) * eq->buf.npages;
=======
	eq->cons_index = 0;

	err = mlx5_frag_buf_alloc_node(dev, wq_get_byte_sz(log_eq_size, log_eq_stride),
				       &eq->frag_buf, dev->priv.numa_node);
	if (err)
		return err;

	mlx5_init_fbc(eq->frag_buf.frags, log_eq_stride, log_eq_size, &eq->fbc);
	init_eq_buf(eq);

	eq->irq = mlx5_irq_request(dev, vecidx, param->affinity);
	if (IS_ERR(eq->irq)) {
		err = PTR_ERR(eq->irq);
		goto err_buf;
	}

	vecidx = mlx5_irq_get_index(eq->irq);
	inlen = MLX5_ST_SZ_BYTES(create_eq_in) +
		MLX5_FLD_SZ_BYTES(create_eq_in, pas[0]) * eq->frag_buf.npages;
>>>>>>> upstream/android-13

	in = kvzalloc(inlen, GFP_KERNEL);
	if (!in) {
		err = -ENOMEM;
<<<<<<< HEAD
		goto err_buf;
	}

	pas = (__be64 *)MLX5_ADDR_OF(create_eq_in, in, pas);
	mlx5_fill_page_array(&eq->buf, pas);

	MLX5_SET(create_eq_in, in, opcode, MLX5_CMD_OP_CREATE_EQ);
	MLX5_SET64(create_eq_in, in, event_bitmask, mask);

	eqc = MLX5_ADDR_OF(create_eq_in, in, eq_context_entry);
	MLX5_SET(eqc, eqc, log_eq_size, ilog2(eq->nent));
	MLX5_SET(eqc, eqc, uar_page, priv->uar->index);
	MLX5_SET(eqc, eqc, intr, vecidx);
	MLX5_SET(eqc, eqc, log_page_size,
		 eq->buf.page_shift - MLX5_ADAPTER_PAGE_SHIFT);
=======
		goto err_irq;
	}

	pas = (__be64 *)MLX5_ADDR_OF(create_eq_in, in, pas);
	mlx5_fill_page_frag_array(&eq->frag_buf, pas);

	MLX5_SET(create_eq_in, in, opcode, MLX5_CMD_OP_CREATE_EQ);
	if (!param->mask[0] && MLX5_CAP_GEN(dev, log_max_uctx))
		MLX5_SET(create_eq_in, in, uid, MLX5_SHARED_RESOURCE_UID);

	for (i = 0; i < 4; i++)
		MLX5_ARRAY_SET64(create_eq_in, in, event_bitmask, i,
				 param->mask[i]);

	eqc = MLX5_ADDR_OF(create_eq_in, in, eq_context_entry);
	MLX5_SET(eqc, eqc, log_eq_size, eq->fbc.log_sz);
	MLX5_SET(eqc, eqc, uar_page, priv->uar->index);
	MLX5_SET(eqc, eqc, intr, vecidx);
	MLX5_SET(eqc, eqc, log_page_size,
		 eq->frag_buf.page_shift - MLX5_ADAPTER_PAGE_SHIFT);
>>>>>>> upstream/android-13

	err = mlx5_cmd_exec(dev, in, inlen, out, sizeof(out));
	if (err)
		goto err_in;

<<<<<<< HEAD
	snprintf(priv->irq_info[vecidx].name, MLX5_MAX_IRQ_NAME, "%s@pci:%s",
		 name, pci_name(dev->pdev));

=======
	eq->vecidx = vecidx;
>>>>>>> upstream/android-13
	eq->eqn = MLX5_GET(create_eq_out, out, eq_number);
	eq->irqn = pci_irq_vector(dev->pdev, vecidx);
	eq->dev = dev;
	eq->doorbell = priv->uar->map + MLX5_EQ_DOORBEL_OFFSET;
<<<<<<< HEAD
	err = request_irq(eq->irqn, handler, 0,
			  priv->irq_info[vecidx].name, eq);
	if (err)
		goto err_eq;

	err = mlx5_debug_eq_add(dev, eq);
	if (err)
		goto err_irq;

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	if (type == MLX5_EQ_TYPE_PF) {
		err = init_pf_ctx(&eq->pf_ctx, name);
		if (err)
			goto err_irq;
	} else
#endif
	{
		INIT_LIST_HEAD(&eq->tasklet_ctx.list);
		INIT_LIST_HEAD(&eq->tasklet_ctx.process_list);
		spin_lock_init(&eq->tasklet_ctx.lock);
		tasklet_init(&eq->tasklet_ctx.task, mlx5_cq_tasklet_cb,
			     (unsigned long)&eq->tasklet_ctx);
	}

	/* EQs are created in ARMED state
	 */
	eq_update_ci(eq, 1);
=======

	err = mlx5_debug_eq_add(dev, eq);
	if (err)
		goto err_eq;
>>>>>>> upstream/android-13

	kvfree(in);
	return 0;

<<<<<<< HEAD
err_irq:
	free_irq(eq->irqn, eq);

=======
>>>>>>> upstream/android-13
err_eq:
	mlx5_cmd_destroy_eq(dev, eq->eqn);

err_in:
	kvfree(in);

<<<<<<< HEAD
err_buf:
	mlx5_buf_free(dev, &eq->buf);
	return err;
}

int mlx5_destroy_unmap_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq)
=======
err_irq:
	mlx5_irq_release(eq->irq);
err_buf:
	mlx5_frag_buf_free(dev, &eq->frag_buf);
	return err;
}

/**
 * mlx5_eq_enable - Enable EQ for receiving EQEs
 * @dev : Device which owns the eq
 * @eq  : EQ to enable
 * @nb  : Notifier call block
 *
 * Must be called after EQ is created in device.
 *
 * @return: 0 if no error
 */
int mlx5_eq_enable(struct mlx5_core_dev *dev, struct mlx5_eq *eq,
		   struct notifier_block *nb)
{
	int err;

	err = mlx5_irq_attach_nb(eq->irq, nb);
	if (!err)
		eq_update_ci(eq, 1);

	return err;
}
EXPORT_SYMBOL(mlx5_eq_enable);

/**
 * mlx5_eq_disable - Disable EQ for receiving EQEs
 * @dev : Device which owns the eq
 * @eq  : EQ to disable
 * @nb  : Notifier call block
 *
 * Must be called before EQ is destroyed.
 */
void mlx5_eq_disable(struct mlx5_core_dev *dev, struct mlx5_eq *eq,
		     struct notifier_block *nb)
{
	mlx5_irq_detach_nb(eq->irq, nb);
}
EXPORT_SYMBOL(mlx5_eq_disable);

static int destroy_unmap_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq)
>>>>>>> upstream/android-13
{
	int err;

	mlx5_debug_eq_remove(dev, eq);
<<<<<<< HEAD
	free_irq(eq->irqn, eq);
=======

>>>>>>> upstream/android-13
	err = mlx5_cmd_destroy_eq(dev, eq->eqn);
	if (err)
		mlx5_core_warn(dev, "failed to destroy a previously created eq: eqn %d\n",
			       eq->eqn);
<<<<<<< HEAD
	synchronize_irq(eq->irqn);

	if (eq->type == MLX5_EQ_TYPE_COMP) {
		tasklet_disable(&eq->tasklet_ctx.task);
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	} else if (eq->type == MLX5_EQ_TYPE_PF) {
		cancel_work_sync(&eq->pf_ctx.work);
		destroy_workqueue(eq->pf_ctx.wq);
		mempool_destroy(eq->pf_ctx.pool);
#endif
	}
	mlx5_buf_free(dev, &eq->buf);

=======
	mlx5_irq_release(eq->irq);

	mlx5_frag_buf_free(dev, &eq->frag_buf);
>>>>>>> upstream/android-13
	return err;
}

int mlx5_eq_add_cq(struct mlx5_eq *eq, struct mlx5_core_cq *cq)
{
	struct mlx5_cq_table *table = &eq->cq_table;
	int err;

<<<<<<< HEAD
	spin_lock_irq(&table->lock);
	err = radix_tree_insert(&table->tree, cq->cqn, cq);
	spin_unlock_irq(&table->lock);
=======
	spin_lock(&table->lock);
	err = radix_tree_insert(&table->tree, cq->cqn, cq);
	spin_unlock(&table->lock);
>>>>>>> upstream/android-13

	return err;
}

<<<<<<< HEAD
int mlx5_eq_del_cq(struct mlx5_eq *eq, struct mlx5_core_cq *cq)
=======
void mlx5_eq_del_cq(struct mlx5_eq *eq, struct mlx5_core_cq *cq)
>>>>>>> upstream/android-13
{
	struct mlx5_cq_table *table = &eq->cq_table;
	struct mlx5_core_cq *tmp;

<<<<<<< HEAD
	spin_lock_irq(&table->lock);
	tmp = radix_tree_delete(&table->tree, cq->cqn);
	spin_unlock_irq(&table->lock);

	if (!tmp) {
		mlx5_core_warn(eq->dev, "cq 0x%x not found in eq 0x%x tree\n", eq->eqn, cq->cqn);
		return -ENOENT;
	}

	if (tmp != cq) {
		mlx5_core_warn(eq->dev, "corruption on cqn 0x%x in eq 0x%x\n", eq->eqn, cq->cqn);
		return -EINVAL;
	}

	return 0;
}

int mlx5_eq_init(struct mlx5_core_dev *dev)
{
	int err;

	spin_lock_init(&dev->priv.eq_table.lock);

	err = mlx5_eq_debugfs_init(dev);

	return err;
}

void mlx5_eq_cleanup(struct mlx5_core_dev *dev)
{
	mlx5_eq_debugfs_cleanup(dev);
}

int mlx5_start_eqs(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *table = &dev->priv.eq_table;
	u64 async_event_mask = MLX5_ASYNC_EVENT_MASK;
	int err;
=======
	spin_lock(&table->lock);
	tmp = radix_tree_delete(&table->tree, cq->cqn);
	spin_unlock(&table->lock);

	if (!tmp) {
		mlx5_core_dbg(eq->dev, "cq 0x%x not found in eq 0x%x tree\n",
			      eq->eqn, cq->cqn);
		return;
	}

	if (tmp != cq)
		mlx5_core_dbg(eq->dev, "corruption on cqn 0x%x in eq 0x%x\n",
			      eq->eqn, cq->cqn);
}

int mlx5_eq_table_init(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *eq_table;
	int i;

	eq_table = kvzalloc(sizeof(*eq_table), GFP_KERNEL);
	if (!eq_table)
		return -ENOMEM;

	dev->priv.eq_table = eq_table;

	mlx5_eq_debugfs_init(dev);

	mutex_init(&eq_table->lock);
	for (i = 0; i < MLX5_EVENT_TYPE_MAX; i++)
		ATOMIC_INIT_NOTIFIER_HEAD(&eq_table->nh[i]);

	eq_table->irq_table = mlx5_irq_table_get(dev);
	return 0;
}

void mlx5_eq_table_cleanup(struct mlx5_core_dev *dev)
{
	mlx5_eq_debugfs_cleanup(dev);
	kvfree(dev->priv.eq_table);
}

/* Async EQs */

static int create_async_eq(struct mlx5_core_dev *dev,
			   struct mlx5_eq *eq, struct mlx5_eq_param *param)
{
	struct mlx5_eq_table *eq_table = dev->priv.eq_table;
	int err;

	mutex_lock(&eq_table->lock);
	err = create_map_eq(dev, eq, param);
	mutex_unlock(&eq_table->lock);
	return err;
}

static int destroy_async_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq)
{
	struct mlx5_eq_table *eq_table = dev->priv.eq_table;
	int err;

	mutex_lock(&eq_table->lock);
	err = destroy_unmap_eq(dev, eq);
	mutex_unlock(&eq_table->lock);
	return err;
}

static int cq_err_event_notifier(struct notifier_block *nb,
				 unsigned long type, void *data)
{
	struct mlx5_eq_table *eqt;
	struct mlx5_core_cq *cq;
	struct mlx5_eqe *eqe;
	struct mlx5_eq *eq;
	u32 cqn;

	/* type == MLX5_EVENT_TYPE_CQ_ERROR */

	eqt = mlx5_nb_cof(nb, struct mlx5_eq_table, cq_err_nb);
	eq  = &eqt->async_eq.core;
	eqe = data;

	cqn = be32_to_cpu(eqe->data.cq_err.cqn) & 0xffffff;
	mlx5_core_warn(eq->dev, "CQ error on CQN 0x%x, syndrome 0x%x\n",
		       cqn, eqe->data.cq_err.syndrome);

	cq = mlx5_eq_cq_get(eq, cqn);
	if (unlikely(!cq)) {
		mlx5_core_warn(eq->dev, "Async event for bogus CQ 0x%x\n", cqn);
		return NOTIFY_OK;
	}

	if (cq->event)
		cq->event(cq, type);

	mlx5_cq_put(cq);

	return NOTIFY_OK;
}

static void gather_user_async_events(struct mlx5_core_dev *dev, u64 mask[4])
{
	__be64 *user_unaffiliated_events;
	__be64 *user_affiliated_events;
	int i;

	user_affiliated_events =
		MLX5_CAP_DEV_EVENT(dev, user_affiliated_events);
	user_unaffiliated_events =
		MLX5_CAP_DEV_EVENT(dev, user_unaffiliated_events);

	for (i = 0; i < 4; i++)
		mask[i] |= be64_to_cpu(user_affiliated_events[i] |
				       user_unaffiliated_events[i]);
}

static void gather_async_events_mask(struct mlx5_core_dev *dev, u64 mask[4])
{
	u64 async_event_mask = MLX5_ASYNC_EVENT_MASK;
>>>>>>> upstream/android-13

	if (MLX5_VPORT_MANAGER(dev))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_NIC_VPORT_CHANGE);

<<<<<<< HEAD
	if (MLX5_CAP_GEN(dev, port_type) == MLX5_CAP_PORT_TYPE_ETH &&
	    MLX5_CAP_GEN(dev, general_notification_event))
=======
	if (MLX5_CAP_GEN(dev, general_notification_event))
>>>>>>> upstream/android-13
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_GENERAL_EVENT);

	if (MLX5_CAP_GEN(dev, port_module_event))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_PORT_MODULE_EVENT);
	else
		mlx5_core_dbg(dev, "port_module_event is not set\n");

	if (MLX5_PPS_CAP(dev))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_PPS_EVENT);

	if (MLX5_CAP_GEN(dev, fpga))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_FPGA_ERROR) |
				    (1ull << MLX5_EVENT_TYPE_FPGA_QP_ERROR);
	if (MLX5_CAP_GEN_MAX(dev, dct))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_DCT_DRAINED);

	if (MLX5_CAP_GEN(dev, temp_warn_event))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_TEMP_WARN_EVENT);

	if (MLX5_CAP_MCAM_REG(dev, tracer_registers))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_DEVICE_TRACER);

<<<<<<< HEAD
	err = mlx5_create_map_eq(dev, &table->cmd_eq, MLX5_EQ_VEC_CMD,
				 MLX5_NUM_CMD_EQE, 1ull << MLX5_EVENT_TYPE_CMD,
				 "mlx5_cmd_eq", MLX5_EQ_TYPE_ASYNC);
	if (err) {
		mlx5_core_warn(dev, "failed to create cmd EQ %d\n", err);
		return err;
	}

	mlx5_cmd_use_events(dev);

	err = mlx5_create_map_eq(dev, &table->async_eq, MLX5_EQ_VEC_ASYNC,
				 MLX5_NUM_ASYNC_EQE, async_event_mask,
				 "mlx5_async_eq", MLX5_EQ_TYPE_ASYNC);
	if (err) {
		mlx5_core_warn(dev, "failed to create async EQ %d\n", err);
		goto err1;
	}

	err = mlx5_create_map_eq(dev, &table->pages_eq,
				 MLX5_EQ_VEC_PAGES,
				 /* TODO: sriov max_vf + */ 1,
				 1 << MLX5_EVENT_TYPE_PAGE_REQUEST, "mlx5_pages_eq",
				 MLX5_EQ_TYPE_ASYNC);
	if (err) {
		mlx5_core_warn(dev, "failed to create pages EQ %d\n", err);
		goto err2;
	}

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	if (MLX5_CAP_GEN(dev, pg)) {
		err = mlx5_create_map_eq(dev, &table->pfault_eq,
					 MLX5_EQ_VEC_PFAULT,
					 MLX5_NUM_ASYNC_EQE,
					 1 << MLX5_EVENT_TYPE_PAGE_FAULT,
					 "mlx5_page_fault_eq",
					 MLX5_EQ_TYPE_PF);
		if (err) {
			mlx5_core_warn(dev, "failed to create page fault EQ %d\n",
				       err);
			goto err3;
=======
	if (MLX5_CAP_GEN(dev, max_num_of_monitor_counters))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_MONITOR_COUNTER);

	if (mlx5_eswitch_is_funcs_handler(dev))
		async_event_mask |=
			(1ull << MLX5_EVENT_TYPE_ESW_FUNCTIONS_CHANGED);

	if (MLX5_CAP_GEN_MAX(dev, vhca_state))
		async_event_mask |= (1ull << MLX5_EVENT_TYPE_VHCA_STATE_CHANGE);

	mask[0] = async_event_mask;

	if (MLX5_CAP_GEN(dev, event_cap))
		gather_user_async_events(dev, mask);
}

static int
setup_async_eq(struct mlx5_core_dev *dev, struct mlx5_eq_async *eq,
	       struct mlx5_eq_param *param, const char *name)
{
	int err;

	eq->irq_nb.notifier_call = mlx5_eq_async_int;
	spin_lock_init(&eq->lock);
	if (!zalloc_cpumask_var(&param->affinity, GFP_KERNEL))
		return -ENOMEM;

	err = create_async_eq(dev, &eq->core, param);
	free_cpumask_var(param->affinity);
	if (err) {
		mlx5_core_warn(dev, "failed to create %s EQ %d\n", name, err);
		return err;
	}
	err = mlx5_eq_enable(dev, &eq->core, &eq->irq_nb);
	if (err) {
		mlx5_core_warn(dev, "failed to enable %s EQ %d\n", name, err);
		destroy_async_eq(dev, &eq->core);
	}
	return err;
}

static void cleanup_async_eq(struct mlx5_core_dev *dev,
			     struct mlx5_eq_async *eq, const char *name)
{
	int err;

	mlx5_eq_disable(dev, &eq->core, &eq->irq_nb);
	err = destroy_async_eq(dev, &eq->core);
	if (err)
		mlx5_core_err(dev, "failed to destroy %s eq, err(%d)\n",
			      name, err);
}

static int create_async_eqs(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;
	struct mlx5_eq_param param = {};
	int err;

	MLX5_NB_INIT(&table->cq_err_nb, cq_err_event_notifier, CQ_ERROR);
	mlx5_eq_notifier_register(dev, &table->cq_err_nb);

	param = (struct mlx5_eq_param) {
		.nent = MLX5_NUM_CMD_EQE,
		.mask[0] = 1ull << MLX5_EVENT_TYPE_CMD,
	};
	mlx5_cmd_allowed_opcode(dev, MLX5_CMD_OP_CREATE_EQ);
	err = setup_async_eq(dev, &table->cmd_eq, &param, "cmd");
	if (err)
		goto err1;

	mlx5_cmd_use_events(dev);
	mlx5_cmd_allowed_opcode(dev, CMD_ALLOWED_OPCODE_ALL);

	param = (struct mlx5_eq_param) {
		.nent = MLX5_NUM_ASYNC_EQE,
	};

	gather_async_events_mask(dev, param.mask);
	err = setup_async_eq(dev, &table->async_eq, &param, "async");
	if (err)
		goto err2;

	param = (struct mlx5_eq_param) {
		.nent = /* TODO: sriov max_vf + */ 1,
		.mask[0] = 1ull << MLX5_EVENT_TYPE_PAGE_REQUEST,
	};

	err = setup_async_eq(dev, &table->pages_eq, &param, "pages");
	if (err)
		goto err3;

	return 0;

err3:
	cleanup_async_eq(dev, &table->async_eq, "async");
err2:
	mlx5_cmd_use_polling(dev);
	cleanup_async_eq(dev, &table->cmd_eq, "cmd");
err1:
	mlx5_cmd_allowed_opcode(dev, CMD_ALLOWED_OPCODE_ALL);
	mlx5_eq_notifier_unregister(dev, &table->cq_err_nb);
	return err;
}

static void destroy_async_eqs(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;

	cleanup_async_eq(dev, &table->pages_eq, "pages");
	cleanup_async_eq(dev, &table->async_eq, "async");
	mlx5_cmd_allowed_opcode(dev, MLX5_CMD_OP_DESTROY_EQ);
	mlx5_cmd_use_polling(dev);
	cleanup_async_eq(dev, &table->cmd_eq, "cmd");
	mlx5_cmd_allowed_opcode(dev, CMD_ALLOWED_OPCODE_ALL);
	mlx5_eq_notifier_unregister(dev, &table->cq_err_nb);
}

struct mlx5_eq *mlx5_get_async_eq(struct mlx5_core_dev *dev)
{
	return &dev->priv.eq_table->async_eq.core;
}

void mlx5_eq_synchronize_async_irq(struct mlx5_core_dev *dev)
{
	synchronize_irq(dev->priv.eq_table->async_eq.core.irqn);
}

void mlx5_eq_synchronize_cmd_irq(struct mlx5_core_dev *dev)
{
	synchronize_irq(dev->priv.eq_table->cmd_eq.core.irqn);
}

/* Generic EQ API for mlx5_core consumers
 * Needed For RDMA ODP EQ for now
 */
struct mlx5_eq *
mlx5_eq_create_generic(struct mlx5_core_dev *dev,
		       struct mlx5_eq_param *param)
{
	struct mlx5_eq *eq = kvzalloc(sizeof(*eq), GFP_KERNEL);
	int err;

	if (!cpumask_available(param->affinity))
		return ERR_PTR(-EINVAL);

	if (!eq)
		return ERR_PTR(-ENOMEM);

	err = create_async_eq(dev, eq, param);
	if (err) {
		kvfree(eq);
		eq = ERR_PTR(err);
	}

	return eq;
}
EXPORT_SYMBOL(mlx5_eq_create_generic);

int mlx5_eq_destroy_generic(struct mlx5_core_dev *dev, struct mlx5_eq *eq)
{
	int err;

	if (IS_ERR(eq))
		return -EINVAL;

	err = destroy_async_eq(dev, eq);
	if (err)
		goto out;

	kvfree(eq);
out:
	return err;
}
EXPORT_SYMBOL(mlx5_eq_destroy_generic);

struct mlx5_eqe *mlx5_eq_get_eqe(struct mlx5_eq *eq, u32 cc)
{
	u32 ci = eq->cons_index + cc;
	u32 nent = eq_get_size(eq);
	struct mlx5_eqe *eqe;

	eqe = get_eqe(eq, ci & (nent - 1));
	eqe = ((eqe->owner & 1) ^ !!(ci & nent)) ? NULL : eqe;
	/* Make sure we read EQ entry contents after we've
	 * checked the ownership bit.
	 */
	if (eqe)
		dma_rmb();

	return eqe;
}
EXPORT_SYMBOL(mlx5_eq_get_eqe);

void mlx5_eq_update_ci(struct mlx5_eq *eq, u32 cc, bool arm)
{
	__be32 __iomem *addr = eq->doorbell + (arm ? 0 : 2);
	u32 val;

	eq->cons_index += cc;
	val = (eq->cons_index & 0xffffff) | (eq->eqn << 24);

	__raw_writel((__force u32)cpu_to_be32(val), addr);
	/* We still want ordering, just not swabbing, so add a barrier */
	wmb();
}
EXPORT_SYMBOL(mlx5_eq_update_ci);

static void destroy_comp_eqs(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;
	struct mlx5_eq_comp *eq, *n;

	list_for_each_entry_safe(eq, n, &table->comp_eqs_list, list) {
		list_del(&eq->list);
		mlx5_eq_disable(dev, &eq->core, &eq->irq_nb);
		if (destroy_unmap_eq(dev, &eq->core))
			mlx5_core_warn(dev, "failed to destroy comp EQ 0x%x\n",
				       eq->core.eqn);
		tasklet_disable(&eq->tasklet_ctx.task);
		kfree(eq);
	}
}

static int create_comp_eqs(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;
	struct mlx5_eq_comp *eq;
	int ncomp_eqs;
	int nent;
	int err;
	int i;

	INIT_LIST_HEAD(&table->comp_eqs_list);
	ncomp_eqs = table->num_comp_eqs;
	nent = MLX5_COMP_EQ_SIZE;
	for (i = 0; i < ncomp_eqs; i++) {
		int vecidx = i + MLX5_IRQ_VEC_COMP_BASE;
		struct mlx5_eq_param param = {};

		eq = kzalloc(sizeof(*eq), GFP_KERNEL);
		if (!eq) {
			err = -ENOMEM;
			goto clean;
		}

		INIT_LIST_HEAD(&eq->tasklet_ctx.list);
		INIT_LIST_HEAD(&eq->tasklet_ctx.process_list);
		spin_lock_init(&eq->tasklet_ctx.lock);
		tasklet_setup(&eq->tasklet_ctx.task, mlx5_cq_tasklet_cb);

		eq->irq_nb.notifier_call = mlx5_eq_comp_int;
		param = (struct mlx5_eq_param) {
			.irq_index = vecidx,
			.nent = nent,
		};

		if (!zalloc_cpumask_var(&param.affinity, GFP_KERNEL)) {
			err = -ENOMEM;
			goto clean_eq;
		}
		cpumask_set_cpu(cpumask_local_spread(i, dev->priv.numa_node),
				param.affinity);
		err = create_map_eq(dev, &eq->core, &param);
		free_cpumask_var(param.affinity);
		if (err)
			goto clean_eq;
		err = mlx5_eq_enable(dev, &eq->core, &eq->irq_nb);
		if (err) {
			destroy_unmap_eq(dev, &eq->core);
			goto clean_eq;
		}

		mlx5_core_dbg(dev, "allocated completion EQN %d\n", eq->core.eqn);
		/* add tail, to keep the list ordered, for mlx5_vector2eqn to work */
		list_add_tail(&eq->list, &table->comp_eqs_list);
	}

	return 0;
clean_eq:
	kfree(eq);
clean:
	destroy_comp_eqs(dev);
	return err;
}

static int vector2eqnirqn(struct mlx5_core_dev *dev, int vector, int *eqn,
			  unsigned int *irqn)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;
	struct mlx5_eq_comp *eq, *n;
	int err = -ENOENT;
	int i = 0;

	list_for_each_entry_safe(eq, n, &table->comp_eqs_list, list) {
		if (i++ == vector) {
			if (irqn)
				*irqn = eq->core.irqn;
			if (eqn)
				*eqn = eq->core.eqn;
			err = 0;
			break;
>>>>>>> upstream/android-13
		}
	}

	return err;
<<<<<<< HEAD
err3:
	mlx5_destroy_unmap_eq(dev, &table->pages_eq);
#else
	return err;
#endif

err2:
	mlx5_destroy_unmap_eq(dev, &table->async_eq);

err1:
	mlx5_cmd_use_polling(dev);
	mlx5_destroy_unmap_eq(dev, &table->cmd_eq);
	return err;
}

void mlx5_stop_eqs(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *table = &dev->priv.eq_table;
	int err;

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	if (MLX5_CAP_GEN(dev, pg)) {
		err = mlx5_destroy_unmap_eq(dev, &table->pfault_eq);
		if (err)
			mlx5_core_err(dev, "failed to destroy page fault eq, err(%d)\n",
				      err);
	}
#endif

	err = mlx5_destroy_unmap_eq(dev, &table->pages_eq);
	if (err)
		mlx5_core_err(dev, "failed to destroy pages eq, err(%d)\n",
			      err);

	err = mlx5_destroy_unmap_eq(dev, &table->async_eq);
	if (err)
		mlx5_core_err(dev, "failed to destroy async eq, err(%d)\n",
			      err);
	mlx5_cmd_use_polling(dev);

	err = mlx5_destroy_unmap_eq(dev, &table->cmd_eq);
	if (err)
		mlx5_core_err(dev, "failed to destroy command eq, err(%d)\n",
			      err);
}

int mlx5_core_eq_query(struct mlx5_core_dev *dev, struct mlx5_eq *eq,
		       u32 *out, int outlen)
{
	u32 in[MLX5_ST_SZ_DW(query_eq_in)] = {0};

	MLX5_SET(query_eq_in, in, opcode, MLX5_CMD_OP_QUERY_EQ);
	MLX5_SET(query_eq_in, in, eq_number, eq->eqn);
	return mlx5_cmd_exec(dev, in, sizeof(in), out, outlen);
=======
}

int mlx5_vector2eqn(struct mlx5_core_dev *dev, int vector, int *eqn)
{
	return vector2eqnirqn(dev, vector, eqn, NULL);
}
EXPORT_SYMBOL(mlx5_vector2eqn);

int mlx5_vector2irqn(struct mlx5_core_dev *dev, int vector, unsigned int *irqn)
{
	return vector2eqnirqn(dev, vector, NULL, irqn);
}

unsigned int mlx5_comp_vectors_count(struct mlx5_core_dev *dev)
{
	return dev->priv.eq_table->num_comp_eqs;
}
EXPORT_SYMBOL(mlx5_comp_vectors_count);

struct cpumask *
mlx5_comp_irq_get_affinity_mask(struct mlx5_core_dev *dev, int vector)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;
	struct mlx5_eq_comp *eq, *n;
	int i = 0;

	list_for_each_entry_safe(eq, n, &table->comp_eqs_list, list) {
		if (i++ == vector)
			break;
	}

	return mlx5_irq_get_affinity_mask(eq->core.irq);
}
EXPORT_SYMBOL(mlx5_comp_irq_get_affinity_mask);

#ifdef CONFIG_RFS_ACCEL
struct cpu_rmap *mlx5_eq_table_get_rmap(struct mlx5_core_dev *dev)
{
	return dev->priv.eq_table->rmap;
}
#endif

struct mlx5_eq_comp *mlx5_eqn2comp_eq(struct mlx5_core_dev *dev, int eqn)
{
	struct mlx5_eq_table *table = dev->priv.eq_table;
	struct mlx5_eq_comp *eq;

	list_for_each_entry(eq, &table->comp_eqs_list, list) {
		if (eq->core.eqn == eqn)
			return eq;
	}

	return ERR_PTR(-ENOENT);
}

static void clear_rmap(struct mlx5_core_dev *dev)
{
#ifdef CONFIG_RFS_ACCEL
	struct mlx5_eq_table *eq_table = dev->priv.eq_table;

	free_irq_cpu_rmap(eq_table->rmap);
#endif
}

static int set_rmap(struct mlx5_core_dev *mdev)
{
	int err = 0;
#ifdef CONFIG_RFS_ACCEL
	struct mlx5_eq_table *eq_table = mdev->priv.eq_table;
	int vecidx;

	eq_table->rmap = alloc_irq_cpu_rmap(eq_table->num_comp_eqs);
	if (!eq_table->rmap) {
		err = -ENOMEM;
		mlx5_core_err(mdev, "Failed to allocate cpu_rmap. err %d", err);
		goto err_out;
	}

	vecidx = MLX5_IRQ_VEC_COMP_BASE;
	for (; vecidx < eq_table->num_comp_eqs + MLX5_IRQ_VEC_COMP_BASE;
	     vecidx++) {
		err = irq_cpu_rmap_add(eq_table->rmap,
				       pci_irq_vector(mdev->pdev, vecidx));
		if (err) {
			mlx5_core_err(mdev, "irq_cpu_rmap_add failed. err %d",
				      err);
			goto err_irq_cpu_rmap_add;
		}
	}
	return 0;

err_irq_cpu_rmap_add:
	clear_rmap(mdev);
err_out:
#endif
	return err;
>>>>>>> upstream/android-13
}

/* This function should only be called after mlx5_cmd_force_teardown_hca */
void mlx5_core_eq_free_irqs(struct mlx5_core_dev *dev)
{
<<<<<<< HEAD
	struct mlx5_eq_table *table = &dev->priv.eq_table;
	struct mlx5_eq *eq;

#ifdef CONFIG_RFS_ACCEL
	if (dev->rmap) {
		free_irq_cpu_rmap(dev->rmap);
		dev->rmap = NULL;
	}
#endif
	list_for_each_entry(eq, &table->comp_eqs_list, list)
		free_irq(eq->irqn, eq);

	free_irq(table->pages_eq.irqn, &table->pages_eq);
	free_irq(table->async_eq.irqn, &table->async_eq);
	free_irq(table->cmd_eq.irqn, &table->cmd_eq);
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	if (MLX5_CAP_GEN(dev, pg))
		free_irq(table->pfault_eq.irqn, &table->pfault_eq);
#endif
	pci_free_irq_vectors(dev->pdev);
}
=======
	struct mlx5_eq_table *table = dev->priv.eq_table;

	mutex_lock(&table->lock); /* sync with create/destroy_async_eq */
	if (!mlx5_core_is_sf(dev))
		clear_rmap(dev);
	mlx5_irq_table_destroy(dev);
	mutex_unlock(&table->lock);
}

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
#define MLX5_MAX_ASYNC_EQS 4
#else
#define MLX5_MAX_ASYNC_EQS 3
#endif

int mlx5_eq_table_create(struct mlx5_core_dev *dev)
{
	struct mlx5_eq_table *eq_table = dev->priv.eq_table;
	int num_eqs = MLX5_CAP_GEN(dev, max_num_eqs) ?
		      MLX5_CAP_GEN(dev, max_num_eqs) :
		      1 << MLX5_CAP_GEN(dev, log_max_eq);
	int max_eqs_sf;
	int err;

	eq_table->num_comp_eqs =
		min_t(int,
		      mlx5_irq_table_get_num_comp(eq_table->irq_table),
		      num_eqs - MLX5_MAX_ASYNC_EQS);
	if (mlx5_core_is_sf(dev)) {
		max_eqs_sf = min_t(int, MLX5_COMP_EQS_PER_SF,
				   mlx5_irq_table_get_sfs_vec(eq_table->irq_table));
		eq_table->num_comp_eqs = min_t(int, eq_table->num_comp_eqs,
					       max_eqs_sf);
	}

	err = create_async_eqs(dev);
	if (err) {
		mlx5_core_err(dev, "Failed to create async EQs\n");
		goto err_async_eqs;
	}

	if (!mlx5_core_is_sf(dev)) {
		/* rmap is a mapping between irq number and queue number.
		 * each irq can be assign only to a single rmap.
		 * since SFs share IRQs, rmap mapping cannot function correctly
		 * for irqs that are shared for different core/netdev RX rings.
		 * Hence we don't allow netdev rmap for SFs
		 */
		err = set_rmap(dev);
		if (err)
			goto err_rmap;
	}

	err = create_comp_eqs(dev);
	if (err) {
		mlx5_core_err(dev, "Failed to create completion EQs\n");
		goto err_comp_eqs;
	}

	return 0;
err_comp_eqs:
	if (!mlx5_core_is_sf(dev))
		clear_rmap(dev);
err_rmap:
	destroy_async_eqs(dev);
err_async_eqs:
	return err;
}

void mlx5_eq_table_destroy(struct mlx5_core_dev *dev)
{
	if (!mlx5_core_is_sf(dev))
		clear_rmap(dev);
	destroy_comp_eqs(dev);
	destroy_async_eqs(dev);
}

int mlx5_eq_notifier_register(struct mlx5_core_dev *dev, struct mlx5_nb *nb)
{
	struct mlx5_eq_table *eqt = dev->priv.eq_table;

	return atomic_notifier_chain_register(&eqt->nh[nb->event_type], &nb->nb);
}
EXPORT_SYMBOL(mlx5_eq_notifier_register);

int mlx5_eq_notifier_unregister(struct mlx5_core_dev *dev, struct mlx5_nb *nb)
{
	struct mlx5_eq_table *eqt = dev->priv.eq_table;

	return atomic_notifier_chain_unregister(&eqt->nh[nb->event_type], &nb->nb);
}
EXPORT_SYMBOL(mlx5_eq_notifier_unregister);
>>>>>>> upstream/android-13
