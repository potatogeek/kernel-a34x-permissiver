/*
 * Broadcom NetXtreme-E RoCE driver.
 *
 * Copyright (c) 2016 - 2017, Broadcom. All rights reserved.  The term
 * Broadcom refers to Broadcom Limited and/or its subsidiaries.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Description: RDMA Controller HW interface
 */
<<<<<<< HEAD
=======

#define dev_fmt(fmt) "QPLIB: " fmt

>>>>>>> upstream/android-13
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/pci.h>
#include <linux/prefetch.h>
#include <linux/delay.h>

#include "roce_hsi.h"
#include "qplib_res.h"
#include "qplib_rcfw.h"
#include "qplib_sp.h"
#include "qplib_fp.h"

<<<<<<< HEAD
static void bnxt_qplib_service_creq(unsigned long data);
=======
static void bnxt_qplib_service_creq(struct tasklet_struct *t);
>>>>>>> upstream/android-13

/* Hardware communication channel */
static int __wait_for_resp(struct bnxt_qplib_rcfw *rcfw, u16 cookie)
{
<<<<<<< HEAD
	u16 cbit;
	int rc;

	cbit = cookie % RCFW_MAX_OUTSTANDING_CMD;
	rc = wait_event_timeout(rcfw->waitq,
				!test_bit(cbit, rcfw->cmdq_bitmap),
=======
	struct bnxt_qplib_cmdq_ctx *cmdq;
	u16 cbit;
	int rc;

	cmdq = &rcfw->cmdq;
	cbit = cookie % rcfw->cmdq_depth;
	rc = wait_event_timeout(cmdq->waitq,
				!test_bit(cbit, cmdq->cmdq_bitmap),
>>>>>>> upstream/android-13
				msecs_to_jiffies(RCFW_CMD_WAIT_TIME_MS));
	return rc ? 0 : -ETIMEDOUT;
};

static int __block_for_resp(struct bnxt_qplib_rcfw *rcfw, u16 cookie)
{
	u32 count = RCFW_BLOCKED_CMD_WAIT_COUNT;
<<<<<<< HEAD
	u16 cbit;

	cbit = cookie % RCFW_MAX_OUTSTANDING_CMD;
	if (!test_bit(cbit, rcfw->cmdq_bitmap))
		goto done;
	do {
		mdelay(1); /* 1m sec */
		bnxt_qplib_service_creq((unsigned long)rcfw);
	} while (test_bit(cbit, rcfw->cmdq_bitmap) && --count);
=======
	struct bnxt_qplib_cmdq_ctx *cmdq;
	u16 cbit;

	cmdq = &rcfw->cmdq;
	cbit = cookie % rcfw->cmdq_depth;
	if (!test_bit(cbit, cmdq->cmdq_bitmap))
		goto done;
	do {
		mdelay(1); /* 1m sec */
		bnxt_qplib_service_creq(&rcfw->creq.creq_tasklet);
	} while (test_bit(cbit, cmdq->cmdq_bitmap) && --count);
>>>>>>> upstream/android-13
done:
	return count ? 0 : -ETIMEDOUT;
};

static int __send_message(struct bnxt_qplib_rcfw *rcfw, struct cmdq_base *req,
			  struct creq_base *resp, void *sb, u8 is_block)
{
<<<<<<< HEAD
	struct bnxt_qplib_cmdqe *cmdqe, **cmdq_ptr;
	struct bnxt_qplib_hwq *cmdq = &rcfw->cmdq;
	struct bnxt_qplib_crsq *crsqe;
	u32 sw_prod, cmdq_prod;
=======
	struct bnxt_qplib_cmdq_ctx *cmdq = &rcfw->cmdq;
	struct bnxt_qplib_hwq *hwq = &cmdq->hwq;
	struct bnxt_qplib_crsqe *crsqe;
	struct bnxt_qplib_cmdqe *cmdqe;
	u32 sw_prod, cmdq_prod;
	struct pci_dev *pdev;
>>>>>>> upstream/android-13
	unsigned long flags;
	u32 size, opcode;
	u16 cookie, cbit;
	u8 *preq;

<<<<<<< HEAD
	opcode = req->opcode;
	if (!test_bit(FIRMWARE_INITIALIZED_FLAG, &rcfw->flags) &&
	    (opcode != CMDQ_BASE_OPCODE_QUERY_FUNC &&
	     opcode != CMDQ_BASE_OPCODE_INITIALIZE_FW &&
	     opcode != CMDQ_BASE_OPCODE_QUERY_VERSION)) {
		dev_err(&rcfw->pdev->dev,
			"QPLIB: RCFW not initialized, reject opcode 0x%x",
			opcode);
		return -EINVAL;
	}

	if (test_bit(FIRMWARE_INITIALIZED_FLAG, &rcfw->flags) &&
	    opcode == CMDQ_BASE_OPCODE_INITIALIZE_FW) {
		dev_err(&rcfw->pdev->dev, "QPLIB: RCFW already initialized!");
		return -EINVAL;
	}

	if (test_bit(FIRMWARE_TIMED_OUT, &rcfw->flags))
=======
	pdev = rcfw->pdev;

	opcode = req->opcode;
	if (!test_bit(FIRMWARE_INITIALIZED_FLAG, &cmdq->flags) &&
	    (opcode != CMDQ_BASE_OPCODE_QUERY_FUNC &&
	     opcode != CMDQ_BASE_OPCODE_INITIALIZE_FW &&
	     opcode != CMDQ_BASE_OPCODE_QUERY_VERSION)) {
		dev_err(&pdev->dev,
			"RCFW not initialized, reject opcode 0x%x\n", opcode);
		return -EINVAL;
	}

	if (test_bit(FIRMWARE_INITIALIZED_FLAG, &cmdq->flags) &&
	    opcode == CMDQ_BASE_OPCODE_INITIALIZE_FW) {
		dev_err(&pdev->dev, "RCFW already initialized!\n");
		return -EINVAL;
	}

	if (test_bit(FIRMWARE_TIMED_OUT, &cmdq->flags))
>>>>>>> upstream/android-13
		return -ETIMEDOUT;

	/* Cmdq are in 16-byte units, each request can consume 1 or more
	 * cmdqe
	 */
<<<<<<< HEAD
	spin_lock_irqsave(&cmdq->lock, flags);
	if (req->cmd_size >= HWQ_FREE_SLOTS(cmdq)) {
		dev_err(&rcfw->pdev->dev, "QPLIB: RCFW: CMDQ is full!");
		spin_unlock_irqrestore(&cmdq->lock, flags);
=======
	spin_lock_irqsave(&hwq->lock, flags);
	if (req->cmd_size >= HWQ_FREE_SLOTS(hwq)) {
		dev_err(&pdev->dev, "RCFW: CMDQ is full!\n");
		spin_unlock_irqrestore(&hwq->lock, flags);
>>>>>>> upstream/android-13
		return -EAGAIN;
	}


<<<<<<< HEAD
	cookie = rcfw->seq_num & RCFW_MAX_COOKIE_VALUE;
	cbit = cookie % RCFW_MAX_OUTSTANDING_CMD;
	if (is_block)
		cookie |= RCFW_CMD_IS_BLOCKING;

	set_bit(cbit, rcfw->cmdq_bitmap);
	req->cookie = cpu_to_le16(cookie);
	crsqe = &rcfw->crsqe_tbl[cbit];
	if (crsqe->resp) {
		spin_unlock_irqrestore(&cmdq->lock, flags);
		return -EBUSY;
	}
=======
	cookie = cmdq->seq_num & RCFW_MAX_COOKIE_VALUE;
	cbit = cookie % rcfw->cmdq_depth;
	if (is_block)
		cookie |= RCFW_CMD_IS_BLOCKING;

	set_bit(cbit, cmdq->cmdq_bitmap);
	req->cookie = cpu_to_le16(cookie);
	crsqe = &rcfw->crsqe_tbl[cbit];
	if (crsqe->resp) {
		spin_unlock_irqrestore(&hwq->lock, flags);
		return -EBUSY;
	}

	size = req->cmd_size;
	/* change the cmd_size to the number of 16byte cmdq unit.
	 * req->cmd_size is modified here
	 */
	bnxt_qplib_set_cmd_slots(req);

>>>>>>> upstream/android-13
	memset(resp, 0, sizeof(*resp));
	crsqe->resp = (struct creq_qp_event *)resp;
	crsqe->resp->cookie = req->cookie;
	crsqe->req_size = req->cmd_size;
	if (req->resp_size && sb) {
		struct bnxt_qplib_rcfw_sbuf *sbuf = sb;

		req->resp_addr = cpu_to_le64(sbuf->dma_addr);
		req->resp_size = (sbuf->size + BNXT_QPLIB_CMDQE_UNITS - 1) /
				  BNXT_QPLIB_CMDQE_UNITS;
	}

<<<<<<< HEAD
	cmdq_ptr = (struct bnxt_qplib_cmdqe **)cmdq->pbl_ptr;
	preq = (u8 *)req;
	size = req->cmd_size * BNXT_QPLIB_CMDQE_UNITS;
	do {
		/* Locate the next cmdq slot */
		sw_prod = HWQ_CMP(cmdq->prod, cmdq);
		cmdqe = &cmdq_ptr[get_cmdq_pg(sw_prod)][get_cmdq_idx(sw_prod)];
		if (!cmdqe) {
			dev_err(&rcfw->pdev->dev,
				"QPLIB: RCFW request failed with no cmdqe!");
=======
	preq = (u8 *)req;
	do {
		/* Locate the next cmdq slot */
		sw_prod = HWQ_CMP(hwq->prod, hwq);
		cmdqe = bnxt_qplib_get_qe(hwq, sw_prod, NULL);
		if (!cmdqe) {
			dev_err(&pdev->dev,
				"RCFW request failed with no cmdqe!\n");
>>>>>>> upstream/android-13
			goto done;
		}
		/* Copy a segment of the req cmd to the cmdq */
		memset(cmdqe, 0, sizeof(*cmdqe));
		memcpy(cmdqe, preq, min_t(u32, size, sizeof(*cmdqe)));
		preq += min_t(u32, size, sizeof(*cmdqe));
		size -= min_t(u32, size, sizeof(*cmdqe));
<<<<<<< HEAD
		cmdq->prod++;
		rcfw->seq_num++;
	} while (size > 0);

	rcfw->seq_num++;

	cmdq_prod = cmdq->prod;
	if (test_bit(FIRMWARE_FIRST_FLAG, &rcfw->flags)) {
=======
		hwq->prod++;
	} while (size > 0);
	cmdq->seq_num++;

	cmdq_prod = hwq->prod;
	if (test_bit(FIRMWARE_FIRST_FLAG, &cmdq->flags)) {
>>>>>>> upstream/android-13
		/* The very first doorbell write
		 * is required to set this flag
		 * which prompts the FW to reset
		 * its internal pointers
		 */
		cmdq_prod |= BIT(FIRMWARE_FIRST_FLAG);
<<<<<<< HEAD
		clear_bit(FIRMWARE_FIRST_FLAG, &rcfw->flags);
=======
		clear_bit(FIRMWARE_FIRST_FLAG, &cmdq->flags);
>>>>>>> upstream/android-13
	}

	/* ring CMDQ DB */
	wmb();
<<<<<<< HEAD
	writel(cmdq_prod, rcfw->cmdq_bar_reg_iomem +
	       rcfw->cmdq_bar_reg_prod_off);
	writel(RCFW_CMDQ_TRIG_VAL, rcfw->cmdq_bar_reg_iomem +
	       rcfw->cmdq_bar_reg_trig_off);
done:
	spin_unlock_irqrestore(&cmdq->lock, flags);
=======
	writel(cmdq_prod, cmdq->cmdq_mbox.prod);
	writel(RCFW_CMDQ_TRIG_VAL, cmdq->cmdq_mbox.db);
done:
	spin_unlock_irqrestore(&hwq->lock, flags);
>>>>>>> upstream/android-13
	/* Return the CREQ response pointer */
	return 0;
}

int bnxt_qplib_rcfw_send_message(struct bnxt_qplib_rcfw *rcfw,
				 struct cmdq_base *req,
				 struct creq_base *resp,
				 void *sb, u8 is_block)
{
	struct creq_qp_event *evnt = (struct creq_qp_event *)resp;
	u16 cookie;
	u8 opcode, retry_cnt = 0xFF;
	int rc = 0;

<<<<<<< HEAD
=======
	/* Prevent posting if f/w is not in a state to process */
	if (test_bit(ERR_DEVICE_DETACHED, &rcfw->cmdq.flags))
		return 0;

>>>>>>> upstream/android-13
	do {
		opcode = req->opcode;
		rc = __send_message(rcfw, req, resp, sb, is_block);
		cookie = le16_to_cpu(req->cookie) & RCFW_MAX_COOKIE_VALUE;
		if (!rc)
			break;

		if (!retry_cnt || (rc != -EAGAIN && rc != -EBUSY)) {
			/* send failed */
<<<<<<< HEAD
			dev_err(&rcfw->pdev->dev, "QPLIB: cmdq[%#x]=%#x send failed",
=======
			dev_err(&rcfw->pdev->dev, "cmdq[%#x]=%#x send failed\n",
>>>>>>> upstream/android-13
				cookie, opcode);
			return rc;
		}
		is_block ? mdelay(1) : usleep_range(500, 1000);

	} while (retry_cnt--);

	if (is_block)
		rc = __block_for_resp(rcfw, cookie);
	else
		rc = __wait_for_resp(rcfw, cookie);
	if (rc) {
		/* timed out */
<<<<<<< HEAD
		dev_err(&rcfw->pdev->dev, "QPLIB: cmdq[%#x]=%#x timedout (%d)msec",
			cookie, opcode, RCFW_CMD_WAIT_TIME_MS);
		set_bit(FIRMWARE_TIMED_OUT, &rcfw->flags);
=======
		dev_err(&rcfw->pdev->dev, "cmdq[%#x]=%#x timedout (%d)msec\n",
			cookie, opcode, RCFW_CMD_WAIT_TIME_MS);
		set_bit(FIRMWARE_TIMED_OUT, &rcfw->cmdq.flags);
>>>>>>> upstream/android-13
		return rc;
	}

	if (evnt->status) {
		/* failed with status */
<<<<<<< HEAD
		dev_err(&rcfw->pdev->dev, "QPLIB: cmdq[%#x]=%#x status %#x",
=======
		dev_err(&rcfw->pdev->dev, "cmdq[%#x]=%#x status %#x\n",
>>>>>>> upstream/android-13
			cookie, opcode, evnt->status);
		rc = -EFAULT;
	}

	return rc;
}
/* Completions */
static int bnxt_qplib_process_func_event(struct bnxt_qplib_rcfw *rcfw,
					 struct creq_func_event *func_event)
{
<<<<<<< HEAD
=======
	int rc;

>>>>>>> upstream/android-13
	switch (func_event->event) {
	case CREQ_FUNC_EVENT_EVENT_TX_WQE_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_TX_DATA_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_RX_WQE_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_RX_DATA_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_CQ_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_TQM_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_CFCQ_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_CFCS_ERROR:
		/* SRQ ctx error, call srq_handler??
		 * But there's no SRQ handle!
		 */
		break;
	case CREQ_FUNC_EVENT_EVENT_CFCC_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_CFCM_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_TIM_ERROR:
		break;
	case CREQ_FUNC_EVENT_EVENT_VF_COMM_REQUEST:
		break;
	case CREQ_FUNC_EVENT_EVENT_RESOURCE_EXHAUSTED:
		break;
	default:
		return -EINVAL;
	}
<<<<<<< HEAD
	return 0;
=======

	rc = rcfw->creq.aeq_handler(rcfw, (void *)func_event, NULL);
	return rc;
>>>>>>> upstream/android-13
}

static int bnxt_qplib_process_qp_event(struct bnxt_qplib_rcfw *rcfw,
				       struct creq_qp_event *qp_event)
{
<<<<<<< HEAD
	struct bnxt_qplib_hwq *cmdq = &rcfw->cmdq;
	struct creq_qp_error_notification *err_event;
	struct bnxt_qplib_crsq *crsqe;
	unsigned long flags;
	struct bnxt_qplib_qp *qp;
	u16 cbit, blocked = 0;
	u16 cookie;
	__le16  mcookie;
	u32 qp_id;

=======
	struct creq_qp_error_notification *err_event;
	struct bnxt_qplib_hwq *hwq = &rcfw->cmdq.hwq;
	struct bnxt_qplib_crsqe *crsqe;
	struct bnxt_qplib_qp *qp;
	u16 cbit, blocked = 0;
	struct pci_dev *pdev;
	unsigned long flags;
	__le16  mcookie;
	u16 cookie;
	int rc = 0;
	u32 qp_id, tbl_indx;

	pdev = rcfw->pdev;
>>>>>>> upstream/android-13
	switch (qp_event->event) {
	case CREQ_QP_EVENT_EVENT_QP_ERROR_NOTIFICATION:
		err_event = (struct creq_qp_error_notification *)qp_event;
		qp_id = le32_to_cpu(err_event->xid);
<<<<<<< HEAD
		qp = rcfw->qp_tbl[qp_id].qp_handle;
		dev_dbg(&rcfw->pdev->dev,
			"QPLIB: Received QP error notification");
		dev_dbg(&rcfw->pdev->dev,
			"QPLIB: qpid 0x%x, req_err=0x%x, resp_err=0x%x\n",
=======
		tbl_indx = map_qp_id_to_tbl_indx(qp_id, rcfw);
		qp = rcfw->qp_tbl[tbl_indx].qp_handle;
		dev_dbg(&pdev->dev, "Received QP error notification\n");
		dev_dbg(&pdev->dev,
			"qpid 0x%x, req_err=0x%x, resp_err=0x%x\n",
>>>>>>> upstream/android-13
			qp_id, err_event->req_err_state_reason,
			err_event->res_err_state_reason);
		if (!qp)
			break;
		bnxt_qplib_mark_qp_error(qp);
<<<<<<< HEAD
		rcfw->aeq_handler(rcfw, qp_event, qp);
=======
		rc = rcfw->creq.aeq_handler(rcfw, qp_event, qp);
>>>>>>> upstream/android-13
		break;
	default:
		/*
		 * Command Response
		 * cmdq->lock needs to be acquired to synchronie
		 * the command send and completion reaping. This function
		 * is always called with creq->lock held. Using
		 * the nested variant of spin_lock.
		 *
		 */

<<<<<<< HEAD
		spin_lock_irqsave_nested(&cmdq->lock, flags,
=======
		spin_lock_irqsave_nested(&hwq->lock, flags,
>>>>>>> upstream/android-13
					 SINGLE_DEPTH_NESTING);
		cookie = le16_to_cpu(qp_event->cookie);
		mcookie = qp_event->cookie;
		blocked = cookie & RCFW_CMD_IS_BLOCKING;
		cookie &= RCFW_MAX_COOKIE_VALUE;
<<<<<<< HEAD
		cbit = cookie % RCFW_MAX_OUTSTANDING_CMD;
=======
		cbit = cookie % rcfw->cmdq_depth;
>>>>>>> upstream/android-13
		crsqe = &rcfw->crsqe_tbl[cbit];
		if (crsqe->resp &&
		    crsqe->resp->cookie  == mcookie) {
			memcpy(crsqe->resp, qp_event, sizeof(*qp_event));
			crsqe->resp = NULL;
		} else {
<<<<<<< HEAD
			dev_err(&rcfw->pdev->dev,
				"QPLIB: CMD %s resp->cookie = %#x, evnt->cookie = %#x",
				crsqe->resp ? "mismatch" : "collision",
				crsqe->resp ? crsqe->resp->cookie : 0, mcookie);
		}
		if (!test_and_clear_bit(cbit, rcfw->cmdq_bitmap))
			dev_warn(&rcfw->pdev->dev,
				 "QPLIB: CMD bit %d was not requested", cbit);
		cmdq->cons += crsqe->req_size;
		crsqe->req_size = 0;

		if (!blocked)
			wake_up(&rcfw->waitq);
		spin_unlock_irqrestore(&cmdq->lock, flags);
	}
	return 0;
}

/* SP - CREQ Completion handlers */
static void bnxt_qplib_service_creq(unsigned long data)
{
	struct bnxt_qplib_rcfw *rcfw = (struct bnxt_qplib_rcfw *)data;
	struct bnxt_qplib_hwq *creq = &rcfw->creq;
	struct creq_base *creqe, **creq_ptr;
	u32 sw_cons, raw_cons;
	unsigned long flags;
	u32 type, budget = CREQ_ENTRY_POLL_BUDGET;

	/* Service the CREQ until budget is over */
	spin_lock_irqsave(&creq->lock, flags);
	raw_cons = creq->cons;
	while (budget > 0) {
		sw_cons = HWQ_CMP(raw_cons, creq);
		creq_ptr = (struct creq_base **)creq->pbl_ptr;
		creqe = &creq_ptr[get_creq_pg(sw_cons)][get_creq_idx(sw_cons)];
		if (!CREQ_CMP_VALID(creqe, raw_cons, creq->max_elements))
=======
			if (crsqe->resp && crsqe->resp->cookie)
				dev_err(&pdev->dev,
					"CMD %s cookie sent=%#x, recd=%#x\n",
					crsqe->resp ? "mismatch" : "collision",
					crsqe->resp ? crsqe->resp->cookie : 0,
					mcookie);
		}
		if (!test_and_clear_bit(cbit, rcfw->cmdq.cmdq_bitmap))
			dev_warn(&pdev->dev,
				 "CMD bit %d was not requested\n", cbit);
		hwq->cons += crsqe->req_size;
		crsqe->req_size = 0;

		if (!blocked)
			wake_up(&rcfw->cmdq.waitq);
		spin_unlock_irqrestore(&hwq->lock, flags);
	}
	return rc;
}

/* SP - CREQ Completion handlers */
static void bnxt_qplib_service_creq(struct tasklet_struct *t)
{
	struct bnxt_qplib_rcfw *rcfw = from_tasklet(rcfw, t, creq.creq_tasklet);
	struct bnxt_qplib_creq_ctx *creq = &rcfw->creq;
	u32 type, budget = CREQ_ENTRY_POLL_BUDGET;
	struct bnxt_qplib_hwq *hwq = &creq->hwq;
	struct creq_base *creqe;
	u32 sw_cons, raw_cons;
	unsigned long flags;

	/* Service the CREQ until budget is over */
	spin_lock_irqsave(&hwq->lock, flags);
	raw_cons = hwq->cons;
	while (budget > 0) {
		sw_cons = HWQ_CMP(raw_cons, hwq);
		creqe = bnxt_qplib_get_qe(hwq, sw_cons, NULL);
		if (!CREQ_CMP_VALID(creqe, raw_cons, hwq->max_elements))
>>>>>>> upstream/android-13
			break;
		/* The valid test of the entry must be done first before
		 * reading any further.
		 */
		dma_rmb();

		type = creqe->type & CREQ_BASE_TYPE_MASK;
		switch (type) {
		case CREQ_BASE_TYPE_QP_EVENT:
			bnxt_qplib_process_qp_event
				(rcfw, (struct creq_qp_event *)creqe);
<<<<<<< HEAD
			rcfw->creq_qp_event_processed++;
=======
			creq->stats.creq_qp_event_processed++;
>>>>>>> upstream/android-13
			break;
		case CREQ_BASE_TYPE_FUNC_EVENT:
			if (!bnxt_qplib_process_func_event
			    (rcfw, (struct creq_func_event *)creqe))
<<<<<<< HEAD
				rcfw->creq_func_event_processed++;
			else
				dev_warn
				(&rcfw->pdev->dev, "QPLIB:aeqe:%#x Not handled",
				 type);
			break;
		default:
			dev_warn(&rcfw->pdev->dev, "QPLIB: creqe with ");
			dev_warn(&rcfw->pdev->dev,
				 "QPLIB: op_event = 0x%x not handled", type);
=======
				creq->stats.creq_func_event_processed++;
			else
				dev_warn(&rcfw->pdev->dev,
					 "aeqe:%#x Not handled\n", type);
			break;
		default:
			if (type != ASYNC_EVENT_CMPL_TYPE_HWRM_ASYNC_EVENT)
				dev_warn(&rcfw->pdev->dev,
					 "creqe with event 0x%x not handled\n",
					 type);
>>>>>>> upstream/android-13
			break;
		}
		raw_cons++;
		budget--;
	}

<<<<<<< HEAD
	if (creq->cons != raw_cons) {
		creq->cons = raw_cons;
		CREQ_DB_REARM(rcfw->creq_bar_reg_iomem, raw_cons,
			      creq->max_elements);
	}
	spin_unlock_irqrestore(&creq->lock, flags);
=======
	if (hwq->cons != raw_cons) {
		hwq->cons = raw_cons;
		bnxt_qplib_ring_nq_db(&creq->creq_db.dbinfo,
				      rcfw->res->cctx, true);
	}
	spin_unlock_irqrestore(&hwq->lock, flags);
>>>>>>> upstream/android-13
}

static irqreturn_t bnxt_qplib_creq_irq(int irq, void *dev_instance)
{
	struct bnxt_qplib_rcfw *rcfw = dev_instance;
<<<<<<< HEAD
	struct bnxt_qplib_hwq *creq = &rcfw->creq;
	struct creq_base **creq_ptr;
	u32 sw_cons;

	/* Prefetch the CREQ element */
	sw_cons = HWQ_CMP(creq->cons, creq);
	creq_ptr = (struct creq_base **)rcfw->creq.pbl_ptr;
	prefetch(&creq_ptr[get_creq_pg(sw_cons)][get_creq_idx(sw_cons)]);

	tasklet_schedule(&rcfw->worker);
=======
	struct bnxt_qplib_creq_ctx *creq;
	struct bnxt_qplib_hwq *hwq;
	u32 sw_cons;

	creq = &rcfw->creq;
	hwq = &creq->hwq;
	/* Prefetch the CREQ element */
	sw_cons = HWQ_CMP(hwq->cons, hwq);
	prefetch(bnxt_qplib_get_qe(hwq, sw_cons, NULL));

	tasklet_schedule(&creq->creq_tasklet);
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

/* RCFW */
int bnxt_qplib_deinit_rcfw(struct bnxt_qplib_rcfw *rcfw)
{
	struct cmdq_deinitialize_fw req;
	struct creq_deinitialize_fw_resp resp;
	u16 cmd_flags = 0;
	int rc;

	RCFW_CMD_PREP(req, DEINITIALIZE_FW, cmd_flags);
	rc = bnxt_qplib_rcfw_send_message(rcfw, (void *)&req, (void *)&resp,
					  NULL, 0);
	if (rc)
		return rc;

<<<<<<< HEAD
	clear_bit(FIRMWARE_INITIALIZED_FLAG, &rcfw->flags);
	return 0;
}

static int __get_pbl_pg_idx(struct bnxt_qplib_pbl *pbl)
{
	return (pbl->pg_size == ROCE_PG_SIZE_4K ?
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_4K :
		pbl->pg_size == ROCE_PG_SIZE_8K ?
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_8K :
		pbl->pg_size == ROCE_PG_SIZE_64K ?
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_64K :
		pbl->pg_size == ROCE_PG_SIZE_2M ?
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_2M :
		pbl->pg_size == ROCE_PG_SIZE_8M ?
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_8M :
		pbl->pg_size == ROCE_PG_SIZE_1G ?
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_1G :
				      CMDQ_INITIALIZE_FW_QPC_PG_SIZE_PG_4K);
}

int bnxt_qplib_init_rcfw(struct bnxt_qplib_rcfw *rcfw,
			 struct bnxt_qplib_ctx *ctx, int is_virtfn)
{
	struct cmdq_initialize_fw req;
	struct creq_initialize_fw_resp resp;
	u16 cmd_flags = 0, level;
=======
	clear_bit(FIRMWARE_INITIALIZED_FLAG, &rcfw->cmdq.flags);
	return 0;
}

int bnxt_qplib_init_rcfw(struct bnxt_qplib_rcfw *rcfw,
			 struct bnxt_qplib_ctx *ctx, int is_virtfn)
{
	struct creq_initialize_fw_resp resp;
	struct cmdq_initialize_fw req;
	u16 cmd_flags = 0;
	u8 pgsz, lvl;
>>>>>>> upstream/android-13
	int rc;

	RCFW_CMD_PREP(req, INITIALIZE_FW, cmd_flags);
	/* Supply (log-base-2-of-host-page-size - base-page-shift)
	 * to bono to adjust the doorbell page sizes.
	 */
	req.log2_dbr_pg_size = cpu_to_le16(PAGE_SHIFT -
					   RCFW_DBR_BASE_PAGE_SHIFT);
	/*
<<<<<<< HEAD
	 * VFs need not setup the HW context area, PF
=======
	 * Gen P5 devices doesn't require this allocation
	 * as the L2 driver does the same for RoCE also.
	 * Also, VFs need not setup the HW context area, PF
>>>>>>> upstream/android-13
	 * shall setup this area for VF. Skipping the
	 * HW programming
	 */
	if (is_virtfn)
		goto skip_ctx_setup;
<<<<<<< HEAD

	level = ctx->qpc_tbl.level;
	req.qpc_pg_size_qpc_lvl = (level << CMDQ_INITIALIZE_FW_QPC_LVL_SFT) |
				__get_pbl_pg_idx(&ctx->qpc_tbl.pbl[level]);
	level = ctx->mrw_tbl.level;
	req.mrw_pg_size_mrw_lvl = (level << CMDQ_INITIALIZE_FW_MRW_LVL_SFT) |
				__get_pbl_pg_idx(&ctx->mrw_tbl.pbl[level]);
	level = ctx->srqc_tbl.level;
	req.srq_pg_size_srq_lvl = (level << CMDQ_INITIALIZE_FW_SRQ_LVL_SFT) |
				__get_pbl_pg_idx(&ctx->srqc_tbl.pbl[level]);
	level = ctx->cq_tbl.level;
	req.cq_pg_size_cq_lvl = (level << CMDQ_INITIALIZE_FW_CQ_LVL_SFT) |
				__get_pbl_pg_idx(&ctx->cq_tbl.pbl[level]);
	level = ctx->srqc_tbl.level;
	req.srq_pg_size_srq_lvl = (level << CMDQ_INITIALIZE_FW_SRQ_LVL_SFT) |
				__get_pbl_pg_idx(&ctx->srqc_tbl.pbl[level]);
	level = ctx->cq_tbl.level;
	req.cq_pg_size_cq_lvl = (level << CMDQ_INITIALIZE_FW_CQ_LVL_SFT) |
				__get_pbl_pg_idx(&ctx->cq_tbl.pbl[level]);
	level = ctx->tim_tbl.level;
	req.tim_pg_size_tim_lvl = (level << CMDQ_INITIALIZE_FW_TIM_LVL_SFT) |
				  __get_pbl_pg_idx(&ctx->tim_tbl.pbl[level]);
	level = ctx->tqm_pde_level;
	req.tqm_pg_size_tqm_lvl = (level << CMDQ_INITIALIZE_FW_TQM_LVL_SFT) |
				  __get_pbl_pg_idx(&ctx->tqm_pde.pbl[level]);

=======
	if (bnxt_qplib_is_chip_gen_p5(rcfw->res->cctx))
		goto config_vf_res;

	lvl = ctx->qpc_tbl.level;
	pgsz = bnxt_qplib_base_pg_size(&ctx->qpc_tbl);
	req.qpc_pg_size_qpc_lvl = (pgsz << CMDQ_INITIALIZE_FW_QPC_PG_SIZE_SFT) |
				   lvl;
	lvl = ctx->mrw_tbl.level;
	pgsz = bnxt_qplib_base_pg_size(&ctx->mrw_tbl);
	req.mrw_pg_size_mrw_lvl = (pgsz << CMDQ_INITIALIZE_FW_QPC_PG_SIZE_SFT) |
				   lvl;
	lvl = ctx->srqc_tbl.level;
	pgsz = bnxt_qplib_base_pg_size(&ctx->srqc_tbl);
	req.srq_pg_size_srq_lvl = (pgsz << CMDQ_INITIALIZE_FW_QPC_PG_SIZE_SFT) |
				   lvl;
	lvl = ctx->cq_tbl.level;
	pgsz = bnxt_qplib_base_pg_size(&ctx->cq_tbl);
	req.cq_pg_size_cq_lvl = (pgsz << CMDQ_INITIALIZE_FW_QPC_PG_SIZE_SFT) |
				 lvl;
	lvl = ctx->tim_tbl.level;
	pgsz = bnxt_qplib_base_pg_size(&ctx->tim_tbl);
	req.tim_pg_size_tim_lvl = (pgsz << CMDQ_INITIALIZE_FW_QPC_PG_SIZE_SFT) |
				   lvl;
	lvl = ctx->tqm_ctx.pde.level;
	pgsz = bnxt_qplib_base_pg_size(&ctx->tqm_ctx.pde);
	req.tqm_pg_size_tqm_lvl = (pgsz << CMDQ_INITIALIZE_FW_QPC_PG_SIZE_SFT) |
				   lvl;
>>>>>>> upstream/android-13
	req.qpc_page_dir =
		cpu_to_le64(ctx->qpc_tbl.pbl[PBL_LVL_0].pg_map_arr[0]);
	req.mrw_page_dir =
		cpu_to_le64(ctx->mrw_tbl.pbl[PBL_LVL_0].pg_map_arr[0]);
	req.srq_page_dir =
		cpu_to_le64(ctx->srqc_tbl.pbl[PBL_LVL_0].pg_map_arr[0]);
	req.cq_page_dir =
		cpu_to_le64(ctx->cq_tbl.pbl[PBL_LVL_0].pg_map_arr[0]);
	req.tim_page_dir =
		cpu_to_le64(ctx->tim_tbl.pbl[PBL_LVL_0].pg_map_arr[0]);
	req.tqm_page_dir =
<<<<<<< HEAD
		cpu_to_le64(ctx->tqm_pde.pbl[PBL_LVL_0].pg_map_arr[0]);
=======
		cpu_to_le64(ctx->tqm_ctx.pde.pbl[PBL_LVL_0].pg_map_arr[0]);
>>>>>>> upstream/android-13

	req.number_of_qp = cpu_to_le32(ctx->qpc_tbl.max_elements);
	req.number_of_mrw = cpu_to_le32(ctx->mrw_tbl.max_elements);
	req.number_of_srq = cpu_to_le32(ctx->srqc_tbl.max_elements);
	req.number_of_cq = cpu_to_le32(ctx->cq_tbl.max_elements);

<<<<<<< HEAD
=======
config_vf_res:
>>>>>>> upstream/android-13
	req.max_qp_per_vf = cpu_to_le32(ctx->vf_res.max_qp_per_vf);
	req.max_mrw_per_vf = cpu_to_le32(ctx->vf_res.max_mrw_per_vf);
	req.max_srq_per_vf = cpu_to_le32(ctx->vf_res.max_srq_per_vf);
	req.max_cq_per_vf = cpu_to_le32(ctx->vf_res.max_cq_per_vf);
	req.max_gid_per_vf = cpu_to_le32(ctx->vf_res.max_gid_per_vf);

skip_ctx_setup:
	req.stat_ctx_id = cpu_to_le32(ctx->stats.fw_id);
	rc = bnxt_qplib_rcfw_send_message(rcfw, (void *)&req, (void *)&resp,
					  NULL, 0);
	if (rc)
		return rc;
<<<<<<< HEAD
	set_bit(FIRMWARE_INITIALIZED_FLAG, &rcfw->flags);
=======
	set_bit(FIRMWARE_INITIALIZED_FLAG, &rcfw->cmdq.flags);
>>>>>>> upstream/android-13
	return 0;
}

void bnxt_qplib_free_rcfw_channel(struct bnxt_qplib_rcfw *rcfw)
{
<<<<<<< HEAD
	kfree(rcfw->qp_tbl);
	kfree(rcfw->crsqe_tbl);
	bnxt_qplib_free_hwq(rcfw->pdev, &rcfw->cmdq);
	bnxt_qplib_free_hwq(rcfw->pdev, &rcfw->creq);
	rcfw->pdev = NULL;
}

int bnxt_qplib_alloc_rcfw_channel(struct pci_dev *pdev,
				  struct bnxt_qplib_rcfw *rcfw,
				  int qp_tbl_sz)
{
	rcfw->pdev = pdev;
	rcfw->creq.max_elements = BNXT_QPLIB_CREQE_MAX_CNT;
	if (bnxt_qplib_alloc_init_hwq(rcfw->pdev, &rcfw->creq, NULL, 0,
				      &rcfw->creq.max_elements,
				      BNXT_QPLIB_CREQE_UNITS, 0, PAGE_SIZE,
				      HWQ_TYPE_L2_CMPL)) {
		dev_err(&rcfw->pdev->dev,
			"QPLIB: HW channel CREQ allocation failed");
		goto fail;
	}
	rcfw->cmdq.max_elements = BNXT_QPLIB_CMDQE_MAX_CNT;
	if (bnxt_qplib_alloc_init_hwq(rcfw->pdev, &rcfw->cmdq, NULL, 0,
				      &rcfw->cmdq.max_elements,
				      BNXT_QPLIB_CMDQE_UNITS, 0, PAGE_SIZE,
				      HWQ_TYPE_CTX)) {
		dev_err(&rcfw->pdev->dev,
			"QPLIB: HW channel CMDQ allocation failed");
		goto fail;
	}

	rcfw->crsqe_tbl = kcalloc(rcfw->cmdq.max_elements,
=======
	kfree(rcfw->cmdq.cmdq_bitmap);
	kfree(rcfw->qp_tbl);
	kfree(rcfw->crsqe_tbl);
	bnxt_qplib_free_hwq(rcfw->res, &rcfw->cmdq.hwq);
	bnxt_qplib_free_hwq(rcfw->res, &rcfw->creq.hwq);
	rcfw->pdev = NULL;
}

int bnxt_qplib_alloc_rcfw_channel(struct bnxt_qplib_res *res,
				  struct bnxt_qplib_rcfw *rcfw,
				  struct bnxt_qplib_ctx *ctx,
				  int qp_tbl_sz)
{
	struct bnxt_qplib_hwq_attr hwq_attr = {};
	struct bnxt_qplib_sg_info sginfo = {};
	struct bnxt_qplib_cmdq_ctx *cmdq;
	struct bnxt_qplib_creq_ctx *creq;
	u32 bmap_size = 0;

	rcfw->pdev = res->pdev;
	cmdq = &rcfw->cmdq;
	creq = &rcfw->creq;
	rcfw->res = res;

	sginfo.pgsize = PAGE_SIZE;
	sginfo.pgshft = PAGE_SHIFT;

	hwq_attr.sginfo = &sginfo;
	hwq_attr.res = rcfw->res;
	hwq_attr.depth = BNXT_QPLIB_CREQE_MAX_CNT;
	hwq_attr.stride = BNXT_QPLIB_CREQE_UNITS;
	hwq_attr.type = bnxt_qplib_get_hwq_type(res);

	if (bnxt_qplib_alloc_init_hwq(&creq->hwq, &hwq_attr)) {
		dev_err(&rcfw->pdev->dev,
			"HW channel CREQ allocation failed\n");
		goto fail;
	}
	if (ctx->hwrm_intf_ver < HWRM_VERSION_RCFW_CMDQ_DEPTH_CHECK)
		rcfw->cmdq_depth = BNXT_QPLIB_CMDQE_MAX_CNT_256;
	else
		rcfw->cmdq_depth = BNXT_QPLIB_CMDQE_MAX_CNT_8192;

	sginfo.pgsize = bnxt_qplib_cmdqe_page_size(rcfw->cmdq_depth);
	hwq_attr.depth = rcfw->cmdq_depth;
	hwq_attr.stride = BNXT_QPLIB_CMDQE_UNITS;
	hwq_attr.type = HWQ_TYPE_CTX;
	if (bnxt_qplib_alloc_init_hwq(&cmdq->hwq, &hwq_attr)) {
		dev_err(&rcfw->pdev->dev,
			"HW channel CMDQ allocation failed\n");
		goto fail;
	}

	rcfw->crsqe_tbl = kcalloc(cmdq->hwq.max_elements,
>>>>>>> upstream/android-13
				  sizeof(*rcfw->crsqe_tbl), GFP_KERNEL);
	if (!rcfw->crsqe_tbl)
		goto fail;

<<<<<<< HEAD
	rcfw->qp_tbl_size = qp_tbl_sz;
	rcfw->qp_tbl = kcalloc(qp_tbl_sz, sizeof(struct bnxt_qplib_qp_node),
=======
	bmap_size = BITS_TO_LONGS(rcfw->cmdq_depth) * sizeof(unsigned long);
	cmdq->cmdq_bitmap = kzalloc(bmap_size, GFP_KERNEL);
	if (!cmdq->cmdq_bitmap)
		goto fail;

	/* Allocate one extra to hold the QP1 entries */
	rcfw->qp_tbl_size = qp_tbl_sz + 1;
	rcfw->qp_tbl = kcalloc(rcfw->qp_tbl_size, sizeof(struct bnxt_qplib_qp_node),
>>>>>>> upstream/android-13
			       GFP_KERNEL);
	if (!rcfw->qp_tbl)
		goto fail;

	return 0;

fail:
	bnxt_qplib_free_rcfw_channel(rcfw);
	return -ENOMEM;
}

void bnxt_qplib_rcfw_stop_irq(struct bnxt_qplib_rcfw *rcfw, bool kill)
{
<<<<<<< HEAD
	tasklet_disable(&rcfw->worker);
	/* Mask h/w interrupts */
	CREQ_DB(rcfw->creq_bar_reg_iomem, rcfw->creq.cons,
		rcfw->creq.max_elements);
	/* Sync with last running IRQ-handler */
	synchronize_irq(rcfw->vector);
	if (kill)
		tasklet_kill(&rcfw->worker);

	if (rcfw->requested) {
		free_irq(rcfw->vector, rcfw);
		rcfw->requested = false;
=======
	struct bnxt_qplib_creq_ctx *creq;

	creq = &rcfw->creq;
	tasklet_disable(&creq->creq_tasklet);
	/* Mask h/w interrupts */
	bnxt_qplib_ring_nq_db(&creq->creq_db.dbinfo, rcfw->res->cctx, false);
	/* Sync with last running IRQ-handler */
	synchronize_irq(creq->msix_vec);
	if (kill)
		tasklet_kill(&creq->creq_tasklet);

	if (creq->requested) {
		free_irq(creq->msix_vec, rcfw);
		creq->requested = false;
>>>>>>> upstream/android-13
	}
}

void bnxt_qplib_disable_rcfw_channel(struct bnxt_qplib_rcfw *rcfw)
{
<<<<<<< HEAD
	unsigned long indx;

	bnxt_qplib_rcfw_stop_irq(rcfw, true);

	iounmap(rcfw->cmdq_bar_reg_iomem);
	iounmap(rcfw->creq_bar_reg_iomem);

	indx = find_first_bit(rcfw->cmdq_bitmap, rcfw->bmap_size);
	if (indx != rcfw->bmap_size)
		dev_err(&rcfw->pdev->dev,
			"QPLIB: disabling RCFW with pending cmd-bit %lx", indx);
	kfree(rcfw->cmdq_bitmap);
	rcfw->bmap_size = 0;

	rcfw->cmdq_bar_reg_iomem = NULL;
	rcfw->creq_bar_reg_iomem = NULL;
	rcfw->aeq_handler = NULL;
	rcfw->vector = 0;
=======
	struct bnxt_qplib_creq_ctx *creq;
	struct bnxt_qplib_cmdq_ctx *cmdq;
	unsigned long indx;

	creq = &rcfw->creq;
	cmdq = &rcfw->cmdq;
	/* Make sure the HW channel is stopped! */
	bnxt_qplib_rcfw_stop_irq(rcfw, true);

	iounmap(cmdq->cmdq_mbox.reg.bar_reg);
	iounmap(creq->creq_db.reg.bar_reg);

	indx = find_first_bit(cmdq->cmdq_bitmap, rcfw->cmdq_depth);
	if (indx != rcfw->cmdq_depth)
		dev_err(&rcfw->pdev->dev,
			"disabling RCFW with pending cmd-bit %lx\n", indx);

	cmdq->cmdq_mbox.reg.bar_reg = NULL;
	creq->creq_db.reg.bar_reg = NULL;
	creq->aeq_handler = NULL;
	creq->msix_vec = 0;
>>>>>>> upstream/android-13
}

int bnxt_qplib_rcfw_start_irq(struct bnxt_qplib_rcfw *rcfw, int msix_vector,
			      bool need_init)
{
<<<<<<< HEAD
	int rc;

	if (rcfw->requested)
		return -EFAULT;

	rcfw->vector = msix_vector;
	if (need_init)
		tasklet_init(&rcfw->worker,
			     bnxt_qplib_service_creq, (unsigned long)rcfw);
	else
		tasklet_enable(&rcfw->worker);
	rc = request_irq(rcfw->vector, bnxt_qplib_creq_irq, 0,
			 "bnxt_qplib_creq", rcfw);
	if (rc)
		return rc;
	rcfw->requested = true;
	CREQ_DB_REARM(rcfw->creq_bar_reg_iomem, rcfw->creq.cons,
		      rcfw->creq.max_elements);
=======
	struct bnxt_qplib_creq_ctx *creq;
	int rc;

	creq = &rcfw->creq;

	if (creq->requested)
		return -EFAULT;

	creq->msix_vec = msix_vector;
	if (need_init)
		tasklet_setup(&creq->creq_tasklet, bnxt_qplib_service_creq);
	else
		tasklet_enable(&creq->creq_tasklet);
	rc = request_irq(creq->msix_vec, bnxt_qplib_creq_irq, 0,
			 "bnxt_qplib_creq", rcfw);
	if (rc)
		return rc;
	creq->requested = true;

	bnxt_qplib_ring_nq_db(&creq->creq_db.dbinfo, rcfw->res->cctx, true);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
int bnxt_qplib_enable_rcfw_channel(struct pci_dev *pdev,
				   struct bnxt_qplib_rcfw *rcfw,
				   int msix_vector,
				   int cp_bar_reg_off, int virt_fn,
				   int (*aeq_handler)(struct bnxt_qplib_rcfw *,
						      void *, void *))
{
	resource_size_t res_base;
	struct cmdq_init init;
	u16 bmap_size;
	int rc;

	/* General */
	rcfw->seq_num = 0;
	set_bit(FIRMWARE_FIRST_FLAG, &rcfw->flags);
	bmap_size = BITS_TO_LONGS(RCFW_MAX_OUTSTANDING_CMD *
				  sizeof(unsigned long));
	rcfw->cmdq_bitmap = kzalloc(bmap_size, GFP_KERNEL);
	if (!rcfw->cmdq_bitmap)
		return -ENOMEM;
	rcfw->bmap_size = bmap_size;

	/* CMDQ */
	rcfw->cmdq_bar_reg = RCFW_COMM_PCI_BAR_REGION;
	res_base = pci_resource_start(pdev, rcfw->cmdq_bar_reg);
	if (!res_base)
		return -ENOMEM;

	rcfw->cmdq_bar_reg_iomem = ioremap_nocache(res_base +
					      RCFW_COMM_BASE_OFFSET,
					      RCFW_COMM_SIZE);
	if (!rcfw->cmdq_bar_reg_iomem) {
		dev_err(&rcfw->pdev->dev,
			"QPLIB: CMDQ BAR region %d mapping failed",
			rcfw->cmdq_bar_reg);
		return -ENOMEM;
	}

	rcfw->cmdq_bar_reg_prod_off = virt_fn ? RCFW_VF_COMM_PROD_OFFSET :
					RCFW_PF_COMM_PROD_OFFSET;

	rcfw->cmdq_bar_reg_trig_off = RCFW_COMM_TRIG_OFFSET;

	/* CREQ */
	rcfw->creq_bar_reg = RCFW_COMM_CONS_PCI_BAR_REGION;
	res_base = pci_resource_start(pdev, rcfw->creq_bar_reg);
	if (!res_base)
		dev_err(&rcfw->pdev->dev,
			"QPLIB: CREQ BAR region %d resc start is 0!",
			rcfw->creq_bar_reg);
	rcfw->creq_bar_reg_iomem = ioremap_nocache(res_base + cp_bar_reg_off,
						   4);
	if (!rcfw->creq_bar_reg_iomem) {
		dev_err(&rcfw->pdev->dev,
			"QPLIB: CREQ BAR region %d mapping failed",
			rcfw->creq_bar_reg);
		iounmap(rcfw->cmdq_bar_reg_iomem);
		rcfw->cmdq_bar_reg_iomem = NULL;
		return -ENOMEM;
	}
	rcfw->creq_qp_event_processed = 0;
	rcfw->creq_func_event_processed = 0;

	if (aeq_handler)
		rcfw->aeq_handler = aeq_handler;
	init_waitqueue_head(&rcfw->waitq);
=======
static int bnxt_qplib_map_cmdq_mbox(struct bnxt_qplib_rcfw *rcfw, bool is_vf)
{
	struct bnxt_qplib_cmdq_mbox *mbox;
	resource_size_t bar_reg;
	struct pci_dev *pdev;
	u16 prod_offt;
	int rc = 0;

	pdev = rcfw->pdev;
	mbox = &rcfw->cmdq.cmdq_mbox;

	mbox->reg.bar_id = RCFW_COMM_PCI_BAR_REGION;
	mbox->reg.len = RCFW_COMM_SIZE;
	mbox->reg.bar_base = pci_resource_start(pdev, mbox->reg.bar_id);
	if (!mbox->reg.bar_base) {
		dev_err(&pdev->dev,
			"QPLIB: CMDQ BAR region %d resc start is 0!\n",
			mbox->reg.bar_id);
		return -ENOMEM;
	}

	bar_reg = mbox->reg.bar_base + RCFW_COMM_BASE_OFFSET;
	mbox->reg.len = RCFW_COMM_SIZE;
	mbox->reg.bar_reg = ioremap(bar_reg, mbox->reg.len);
	if (!mbox->reg.bar_reg) {
		dev_err(&pdev->dev,
			"QPLIB: CMDQ BAR region %d mapping failed\n",
			mbox->reg.bar_id);
		return -ENOMEM;
	}

	prod_offt = is_vf ? RCFW_VF_COMM_PROD_OFFSET :
			    RCFW_PF_COMM_PROD_OFFSET;
	mbox->prod = (void  __iomem *)(mbox->reg.bar_reg + prod_offt);
	mbox->db = (void __iomem *)(mbox->reg.bar_reg + RCFW_COMM_TRIG_OFFSET);
	return rc;
}

static int bnxt_qplib_map_creq_db(struct bnxt_qplib_rcfw *rcfw, u32 reg_offt)
{
	struct bnxt_qplib_creq_db *creq_db;
	resource_size_t bar_reg;
	struct pci_dev *pdev;

	pdev = rcfw->pdev;
	creq_db = &rcfw->creq.creq_db;

	creq_db->reg.bar_id = RCFW_COMM_CONS_PCI_BAR_REGION;
	creq_db->reg.bar_base = pci_resource_start(pdev, creq_db->reg.bar_id);
	if (!creq_db->reg.bar_id)
		dev_err(&pdev->dev,
			"QPLIB: CREQ BAR region %d resc start is 0!",
			creq_db->reg.bar_id);

	bar_reg = creq_db->reg.bar_base + reg_offt;
	/* Unconditionally map 8 bytes to support 57500 series */
	creq_db->reg.len = 8;
	creq_db->reg.bar_reg = ioremap(bar_reg, creq_db->reg.len);
	if (!creq_db->reg.bar_reg) {
		dev_err(&pdev->dev,
			"QPLIB: CREQ BAR region %d mapping failed",
			creq_db->reg.bar_id);
		return -ENOMEM;
	}
	creq_db->dbinfo.db = creq_db->reg.bar_reg;
	creq_db->dbinfo.hwq = &rcfw->creq.hwq;
	creq_db->dbinfo.xid = rcfw->creq.ring_id;
	return 0;
}

static void bnxt_qplib_start_rcfw(struct bnxt_qplib_rcfw *rcfw)
{
	struct bnxt_qplib_cmdq_ctx *cmdq;
	struct bnxt_qplib_creq_ctx *creq;
	struct bnxt_qplib_cmdq_mbox *mbox;
	struct cmdq_init init = {0};

	cmdq = &rcfw->cmdq;
	creq = &rcfw->creq;
	mbox = &cmdq->cmdq_mbox;

	init.cmdq_pbl = cpu_to_le64(cmdq->hwq.pbl[PBL_LVL_0].pg_map_arr[0]);
	init.cmdq_size_cmdq_lvl =
			cpu_to_le16(((rcfw->cmdq_depth <<
				      CMDQ_INIT_CMDQ_SIZE_SFT) &
				    CMDQ_INIT_CMDQ_SIZE_MASK) |
				    ((cmdq->hwq.level <<
				      CMDQ_INIT_CMDQ_LVL_SFT) &
				    CMDQ_INIT_CMDQ_LVL_MASK));
	init.creq_ring_id = cpu_to_le16(creq->ring_id);
	/* Write to the Bono mailbox register */
	__iowrite32_copy(mbox->reg.bar_reg, &init, sizeof(init) / 4);
}

int bnxt_qplib_enable_rcfw_channel(struct bnxt_qplib_rcfw *rcfw,
				   int msix_vector,
				   int cp_bar_reg_off, int virt_fn,
				   aeq_handler_t aeq_handler)
{
	struct bnxt_qplib_cmdq_ctx *cmdq;
	struct bnxt_qplib_creq_ctx *creq;
	int rc;

	cmdq = &rcfw->cmdq;
	creq = &rcfw->creq;

	/* Clear to defaults */

	cmdq->seq_num = 0;
	set_bit(FIRMWARE_FIRST_FLAG, &cmdq->flags);
	init_waitqueue_head(&cmdq->waitq);

	creq->stats.creq_qp_event_processed = 0;
	creq->stats.creq_func_event_processed = 0;
	creq->aeq_handler = aeq_handler;

	rc = bnxt_qplib_map_cmdq_mbox(rcfw, virt_fn);
	if (rc)
		return rc;

	rc = bnxt_qplib_map_creq_db(rcfw, cp_bar_reg_off);
	if (rc)
		return rc;
>>>>>>> upstream/android-13

	rc = bnxt_qplib_rcfw_start_irq(rcfw, msix_vector, true);
	if (rc) {
		dev_err(&rcfw->pdev->dev,
<<<<<<< HEAD
			"QPLIB: Failed to request IRQ for CREQ rc = 0x%x", rc);
=======
			"Failed to request IRQ for CREQ rc = 0x%x\n", rc);
>>>>>>> upstream/android-13
		bnxt_qplib_disable_rcfw_channel(rcfw);
		return rc;
	}

<<<<<<< HEAD
	init.cmdq_pbl = cpu_to_le64(rcfw->cmdq.pbl[PBL_LVL_0].pg_map_arr[0]);
	init.cmdq_size_cmdq_lvl = cpu_to_le16(
		((BNXT_QPLIB_CMDQE_MAX_CNT << CMDQ_INIT_CMDQ_SIZE_SFT) &
		 CMDQ_INIT_CMDQ_SIZE_MASK) |
		((rcfw->cmdq.level << CMDQ_INIT_CMDQ_LVL_SFT) &
		 CMDQ_INIT_CMDQ_LVL_MASK));
	init.creq_ring_id = cpu_to_le16(rcfw->creq_ring_id);

	/* Write to the Bono mailbox register */
	__iowrite32_copy(rcfw->cmdq_bar_reg_iomem, &init, sizeof(init) / 4);
=======
	bnxt_qplib_start_rcfw(rcfw);

>>>>>>> upstream/android-13
	return 0;
}

struct bnxt_qplib_rcfw_sbuf *bnxt_qplib_rcfw_alloc_sbuf(
		struct bnxt_qplib_rcfw *rcfw,
		u32 size)
{
	struct bnxt_qplib_rcfw_sbuf *sbuf;

	sbuf = kzalloc(sizeof(*sbuf), GFP_ATOMIC);
	if (!sbuf)
		return NULL;

	sbuf->size = size;
<<<<<<< HEAD
	sbuf->sb = dma_zalloc_coherent(&rcfw->pdev->dev, sbuf->size,
				       &sbuf->dma_addr, GFP_ATOMIC);
=======
	sbuf->sb = dma_alloc_coherent(&rcfw->pdev->dev, sbuf->size,
				      &sbuf->dma_addr, GFP_ATOMIC);
>>>>>>> upstream/android-13
	if (!sbuf->sb)
		goto bail;

	return sbuf;
bail:
	kfree(sbuf);
	return NULL;
}

void bnxt_qplib_rcfw_free_sbuf(struct bnxt_qplib_rcfw *rcfw,
			       struct bnxt_qplib_rcfw_sbuf *sbuf)
{
	if (sbuf->sb)
		dma_free_coherent(&rcfw->pdev->dev, sbuf->size,
				  sbuf->sb, sbuf->dma_addr);
	kfree(sbuf);
}
