/*
 * Copyright (c) 2016 Hisilicon Limited.
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

#include <linux/platform_device.h>
#include <rdma/ib_umem.h>
<<<<<<< HEAD
#include "hns_roce_device.h"
#include "hns_roce_cmd.h"
#include "hns_roce_hem.h"
#include <rdma/hns-abi.h>
#include "hns_roce_common.h"

static void hns_roce_ib_cq_comp(struct hns_roce_cq *hr_cq)
{
	struct ib_cq *ibcq = &hr_cq->ib_cq;

	ibcq->comp_handler(ibcq, ibcq->cq_context);
}

static void hns_roce_ib_cq_event(struct hns_roce_cq *hr_cq,
				 enum hns_roce_event event_type)
{
	struct hns_roce_dev *hr_dev;
	struct ib_event event;
	struct ib_cq *ibcq;

	ibcq = &hr_cq->ib_cq;
	hr_dev = to_hr_dev(ibcq->device);

	if (event_type != HNS_ROCE_EVENT_TYPE_CQ_ID_INVALID &&
	    event_type != HNS_ROCE_EVENT_TYPE_CQ_ACCESS_ERROR &&
	    event_type != HNS_ROCE_EVENT_TYPE_CQ_OVERFLOW) {
		dev_err(hr_dev->dev,
			"hns_roce_ib: Unexpected event type 0x%x on CQ %06lx\n",
			event_type, hr_cq->cqn);
		return;
	}

	if (ibcq->event_handler) {
		event.device = ibcq->device;
		event.event = IB_EVENT_CQ_ERR;
		event.element.cq = ibcq;
		ibcq->event_handler(&event, ibcq->cq_context);
	}
}

static int hns_roce_sw2hw_cq(struct hns_roce_dev *dev,
			     struct hns_roce_cmd_mailbox *mailbox,
			     unsigned long cq_num)
{
	return hns_roce_cmd_mbox(dev, mailbox->dma, 0, cq_num, 0,
			    HNS_ROCE_CMD_SW2HW_CQ, HNS_ROCE_CMD_TIMEOUT_MSECS);
}

static int hns_roce_cq_alloc(struct hns_roce_dev *hr_dev, int nent,
			     struct hns_roce_mtt *hr_mtt,
			     struct hns_roce_uar *hr_uar,
			     struct hns_roce_cq *hr_cq, int vector)
{
	struct hns_roce_cmd_mailbox *mailbox;
	struct hns_roce_hem_table *mtt_table;
	struct hns_roce_cq_table *cq_table;
	struct device *dev = hr_dev->dev;
	dma_addr_t dma_handle;
	u64 *mtts;
	int ret;

	cq_table = &hr_dev->cq_table;

	/* Get the physical address of cq buf */
	if (hns_roce_check_whether_mhop(hr_dev, HEM_TYPE_CQE))
		mtt_table = &hr_dev->mr_table.mtt_cqe_table;
	else
		mtt_table = &hr_dev->mr_table.mtt_table;

	mtts = hns_roce_table_find(hr_dev, mtt_table,
				   hr_mtt->first_seg, &dma_handle);
	if (!mtts) {
		dev_err(dev, "CQ alloc.Failed to find cq buf addr.\n");
		return -EINVAL;
	}

	if (vector >= hr_dev->caps.num_comp_vectors) {
		dev_err(dev, "CQ alloc.Invalid vector.\n");
		return -EINVAL;
	}
	hr_cq->vector = vector;

	ret = hns_roce_bitmap_alloc(&cq_table->bitmap, &hr_cq->cqn);
	if (ret == -1) {
		dev_err(dev, "CQ alloc.Failed to alloc index.\n");
		return -ENOMEM;
	}

	/* Get CQC memory HEM(Hardware Entry Memory) table */
	ret = hns_roce_table_get(hr_dev, &cq_table->table, hr_cq->cqn);
	if (ret) {
		dev_err(dev, "CQ alloc.Failed to get context mem.\n");
		goto err_out;
	}

	/* The cq insert radix tree */
	spin_lock_irq(&cq_table->lock);
	/* Radix_tree: The associated pointer and long integer key value like */
	ret = radix_tree_insert(&cq_table->tree, hr_cq->cqn, hr_cq);
	spin_unlock_irq(&cq_table->lock);
	if (ret) {
		dev_err(dev, "CQ alloc.Failed to radix_tree_insert.\n");
=======
#include <rdma/uverbs_ioctl.h>
#include "hns_roce_device.h"
#include "hns_roce_cmd.h"
#include "hns_roce_hem.h"
#include "hns_roce_common.h"

static u8 get_least_load_bankid_for_cq(struct hns_roce_bank *bank)
{
	u32 least_load = bank[0].inuse;
	u8 bankid = 0;
	u32 bankcnt;
	u8 i;

	for (i = 1; i < HNS_ROCE_CQ_BANK_NUM; i++) {
		bankcnt = bank[i].inuse;
		if (bankcnt < least_load) {
			least_load = bankcnt;
			bankid = i;
		}
	}

	return bankid;
}

static int alloc_cqn(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq)
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;
	struct hns_roce_bank *bank;
	u8 bankid;
	int id;

	mutex_lock(&cq_table->bank_mutex);
	bankid = get_least_load_bankid_for_cq(cq_table->bank);
	bank = &cq_table->bank[bankid];

	id = ida_alloc_range(&bank->ida, bank->min, bank->max, GFP_KERNEL);
	if (id < 0) {
		mutex_unlock(&cq_table->bank_mutex);
		return id;
	}

	/* the lower 2 bits is bankid */
	hr_cq->cqn = (id << CQ_BANKID_SHIFT) | bankid;
	bank->inuse++;
	mutex_unlock(&cq_table->bank_mutex);

	return 0;
}

static inline u8 get_cq_bankid(unsigned long cqn)
{
	/* The lower 2 bits of CQN are used to hash to different banks */
	return (u8)(cqn & GENMASK(1, 0));
}

static void free_cqn(struct hns_roce_dev *hr_dev, unsigned long cqn)
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;
	struct hns_roce_bank *bank;

	bank = &cq_table->bank[get_cq_bankid(cqn)];

	ida_free(&bank->ida, cqn >> CQ_BANKID_SHIFT);

	mutex_lock(&cq_table->bank_mutex);
	bank->inuse--;
	mutex_unlock(&cq_table->bank_mutex);
}

static int alloc_cqc(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq)
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;
	struct ib_device *ibdev = &hr_dev->ib_dev;
	struct hns_roce_cmd_mailbox *mailbox;
	u64 mtts[MTT_MIN_COUNT] = { 0 };
	dma_addr_t dma_handle;
	int ret;

	ret = hns_roce_mtr_find(hr_dev, &hr_cq->mtr, 0, mtts, ARRAY_SIZE(mtts),
				&dma_handle);
	if (!ret) {
		ibdev_err(ibdev, "failed to find CQ mtr, ret = %d.\n", ret);
		return -EINVAL;
	}

	/* Get CQC memory HEM(Hardware Entry Memory) table */
	ret = hns_roce_table_get(hr_dev, &cq_table->table, hr_cq->cqn);
	if (ret) {
		ibdev_err(ibdev, "failed to get CQ(0x%lx) context, ret = %d.\n",
			  hr_cq->cqn, ret);
		goto err_out;
	}

	ret = xa_err(xa_store(&cq_table->array, hr_cq->cqn, hr_cq, GFP_KERNEL));
	if (ret) {
		ibdev_err(ibdev, "failed to xa_store CQ, ret = %d.\n", ret);
>>>>>>> upstream/android-13
		goto err_put;
	}

	/* Allocate mailbox memory */
	mailbox = hns_roce_alloc_cmd_mailbox(hr_dev);
	if (IS_ERR(mailbox)) {
		ret = PTR_ERR(mailbox);
<<<<<<< HEAD
		goto err_radix;
	}

	hr_dev->hw->write_cqc(hr_dev, hr_cq, mailbox->buf, mtts, dma_handle,
			      nent, vector);

	/* Send mailbox to hw */
	ret = hns_roce_sw2hw_cq(hr_dev, mailbox, hr_cq->cqn);
	hns_roce_free_cmd_mailbox(hr_dev, mailbox);
	if (ret) {
		dev_err(dev, "CQ alloc.Failed to cmd mailbox.\n");
		goto err_radix;
=======
		goto err_xa;
	}

	hr_dev->hw->write_cqc(hr_dev, hr_cq, mailbox->buf, mtts, dma_handle);

	/* Send mailbox to hw */
	ret = hns_roce_cmd_mbox(hr_dev, mailbox->dma, 0, hr_cq->cqn, 0,
			HNS_ROCE_CMD_CREATE_CQC, HNS_ROCE_CMD_TIMEOUT_MSECS);
	hns_roce_free_cmd_mailbox(hr_dev, mailbox);
	if (ret) {
		ibdev_err(ibdev,
			  "failed to send create cmd for CQ(0x%lx), ret = %d.\n",
			  hr_cq->cqn, ret);
		goto err_xa;
>>>>>>> upstream/android-13
	}

	hr_cq->cons_index = 0;
	hr_cq->arm_sn = 1;
<<<<<<< HEAD
	hr_cq->uar = hr_uar;

	atomic_set(&hr_cq->refcount, 1);
=======

	refcount_set(&hr_cq->refcount, 1);
>>>>>>> upstream/android-13
	init_completion(&hr_cq->free);

	return 0;

<<<<<<< HEAD
err_radix:
	spin_lock_irq(&cq_table->lock);
	radix_tree_delete(&cq_table->tree, hr_cq->cqn);
	spin_unlock_irq(&cq_table->lock);
=======
err_xa:
	xa_erase(&cq_table->array, hr_cq->cqn);
>>>>>>> upstream/android-13

err_put:
	hns_roce_table_put(hr_dev, &cq_table->table, hr_cq->cqn);

err_out:
<<<<<<< HEAD
	hns_roce_bitmap_free(&cq_table->bitmap, hr_cq->cqn, BITMAP_NO_RR);
	return ret;
}

static int hns_roce_hw2sw_cq(struct hns_roce_dev *dev,
			     struct hns_roce_cmd_mailbox *mailbox,
			     unsigned long cq_num)
{
	return hns_roce_cmd_mbox(dev, 0, mailbox ? mailbox->dma : 0, cq_num,
				 mailbox ? 0 : 1, HNS_ROCE_CMD_HW2SW_CQ,
				 HNS_ROCE_CMD_TIMEOUT_MSECS);
}

void hns_roce_free_cq(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq)
=======
	return ret;
}

static void free_cqc(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq)
>>>>>>> upstream/android-13
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;
	struct device *dev = hr_dev->dev;
	int ret;

<<<<<<< HEAD
	ret = hns_roce_hw2sw_cq(hr_dev, NULL, hr_cq->cqn);
	if (ret)
		dev_err(dev, "HW2SW_CQ failed (%d) for CQN %06lx\n", ret,
			hr_cq->cqn);

=======
	ret = hns_roce_cmd_mbox(hr_dev, 0, 0, hr_cq->cqn, 1,
				HNS_ROCE_CMD_DESTROY_CQC,
				HNS_ROCE_CMD_TIMEOUT_MSECS);
	if (ret)
		dev_err(dev, "DESTROY_CQ failed (%d) for CQN %06lx\n", ret,
			hr_cq->cqn);

	xa_erase(&cq_table->array, hr_cq->cqn);

>>>>>>> upstream/android-13
	/* Waiting interrupt process procedure carried out */
	synchronize_irq(hr_dev->eq_table.eq[hr_cq->vector].irq);

	/* wait for all interrupt processed */
<<<<<<< HEAD
	if (atomic_dec_and_test(&hr_cq->refcount))
		complete(&hr_cq->free);
	wait_for_completion(&hr_cq->free);

	spin_lock_irq(&cq_table->lock);
	radix_tree_delete(&cq_table->tree, hr_cq->cqn);
	spin_unlock_irq(&cq_table->lock);

	hns_roce_table_put(hr_dev, &cq_table->table, hr_cq->cqn);
	hns_roce_bitmap_free(&cq_table->bitmap, hr_cq->cqn, BITMAP_NO_RR);
}
EXPORT_SYMBOL_GPL(hns_roce_free_cq);

static int hns_roce_ib_get_cq_umem(struct hns_roce_dev *hr_dev,
				   struct ib_ucontext *context,
				   struct hns_roce_cq_buf *buf,
				   struct ib_umem **umem, u64 buf_addr, int cqe)
{
	int ret;
	u32 page_shift;
	u32 npages;

	*umem = ib_umem_get(context, buf_addr, cqe * hr_dev->caps.cq_entry_sz,
			    IB_ACCESS_LOCAL_WRITE, 1);
	if (IS_ERR(*umem))
		return PTR_ERR(*umem);

	if (hns_roce_check_whether_mhop(hr_dev, HEM_TYPE_CQE))
		buf->hr_mtt.mtt_type = MTT_TYPE_CQE;
	else
		buf->hr_mtt.mtt_type = MTT_TYPE_WQE;

	if (hr_dev->caps.cqe_buf_pg_sz) {
		npages = (ib_umem_page_count(*umem) +
			(1 << hr_dev->caps.cqe_buf_pg_sz) - 1) /
			(1 << hr_dev->caps.cqe_buf_pg_sz);
		page_shift = PAGE_SHIFT + hr_dev->caps.cqe_buf_pg_sz;
		ret = hns_roce_mtt_init(hr_dev, npages, page_shift,
					&buf->hr_mtt);
	} else {
		ret = hns_roce_mtt_init(hr_dev, ib_umem_page_count(*umem),
				(*umem)->page_shift,
				&buf->hr_mtt);
	}
	if (ret)
		goto err_buf;

	ret = hns_roce_ib_umem_write_mtt(hr_dev, &buf->hr_mtt, *umem);
	if (ret)
		goto err_mtt;

	return 0;

err_mtt:
	hns_roce_mtt_cleanup(hr_dev, &buf->hr_mtt);

err_buf:
	ib_umem_release(*umem);
	return ret;
}

static int hns_roce_ib_alloc_cq_buf(struct hns_roce_dev *hr_dev,
				    struct hns_roce_cq_buf *buf, u32 nent)
{
	int ret;
	u32 page_shift = PAGE_SHIFT + hr_dev->caps.cqe_buf_pg_sz;

	ret = hns_roce_buf_alloc(hr_dev, nent * hr_dev->caps.cq_entry_sz,
				 (1 << page_shift) * 2, &buf->hr_buf,
				 page_shift);
	if (ret)
		goto out;

	if (hns_roce_check_whether_mhop(hr_dev, HEM_TYPE_CQE))
		buf->hr_mtt.mtt_type = MTT_TYPE_CQE;
	else
		buf->hr_mtt.mtt_type = MTT_TYPE_WQE;

	ret = hns_roce_mtt_init(hr_dev, buf->hr_buf.npages,
				buf->hr_buf.page_shift, &buf->hr_mtt);
	if (ret)
		goto err_buf;

	ret = hns_roce_buf_write_mtt(hr_dev, &buf->hr_mtt, &buf->hr_buf);
	if (ret)
		goto err_mtt;

	return 0;

err_mtt:
	hns_roce_mtt_cleanup(hr_dev, &buf->hr_mtt);

err_buf:
	hns_roce_buf_free(hr_dev, nent * hr_dev->caps.cq_entry_sz,
			  &buf->hr_buf);
out:
	return ret;
}

static void hns_roce_ib_free_cq_buf(struct hns_roce_dev *hr_dev,
				    struct hns_roce_cq_buf *buf, int cqe)
{
	hns_roce_buf_free(hr_dev, (cqe + 1) * hr_dev->caps.cq_entry_sz,
			  &buf->hr_buf);
}

struct ib_cq *hns_roce_ib_create_cq(struct ib_device *ib_dev,
				    const struct ib_cq_init_attr *attr,
				    struct ib_ucontext *context,
				    struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_dev);
	struct device *dev = hr_dev->dev;
	struct hns_roce_ib_create_cq ucmd;
	struct hns_roce_ib_create_cq_resp resp = {};
	struct hns_roce_cq *hr_cq = NULL;
	struct hns_roce_uar *uar = NULL;
	int vector = attr->comp_vector;
	int cq_entries = attr->cqe;
	int ret;

	if (cq_entries < 1 || cq_entries > hr_dev->caps.max_cqes) {
		dev_err(dev, "Creat CQ failed. entries=%d, max=%d\n",
			cq_entries, hr_dev->caps.max_cqes);
		return ERR_PTR(-EINVAL);
	}

	hr_cq = kzalloc(sizeof(*hr_cq), GFP_KERNEL);
	if (!hr_cq)
		return ERR_PTR(-ENOMEM);

	if (hr_dev->caps.min_cqes)
		cq_entries = max(cq_entries, hr_dev->caps.min_cqes);

	cq_entries = roundup_pow_of_two((unsigned int)cq_entries);
	hr_cq->ib_cq.cqe = cq_entries - 1;
	spin_lock_init(&hr_cq->lock);

	if (context) {
		if (ib_copy_from_udata(&ucmd, udata, sizeof(ucmd))) {
			dev_err(dev, "Failed to copy_from_udata.\n");
			ret = -EFAULT;
			goto err_cq;
		}

		/* Get user space address, write it into mtt table */
		ret = hns_roce_ib_get_cq_umem(hr_dev, context, &hr_cq->hr_buf,
					      &hr_cq->umem, ucmd.buf_addr,
					      cq_entries);
		if (ret) {
			dev_err(dev, "Failed to get_cq_umem.\n");
			goto err_cq;
		}

		if ((hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_RECORD_DB) &&
		    (udata->outlen >= sizeof(resp))) {
			ret = hns_roce_db_map_user(to_hr_ucontext(context),
						   ucmd.db_addr, &hr_cq->db);
			if (ret) {
				dev_err(dev, "cq record doorbell map failed!\n");
				goto err_mtt;
			}
			hr_cq->db_en = 1;
			resp.cap_flags |= HNS_ROCE_SUPPORT_CQ_RECORD_DB;
		}

		/* Get user space parameters */
		uar = &to_hr_ucontext(context)->uar;
	} else {
		if (hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_RECORD_DB) {
			ret = hns_roce_alloc_db(hr_dev, &hr_cq->db, 1);
			if (ret)
				goto err_cq;

			hr_cq->set_ci_db = hr_cq->db.db_record;
			*hr_cq->set_ci_db = 0;
			hr_cq->db_en = 1;
		}

		/* Init mmt table and write buff address to mtt table */
		ret = hns_roce_ib_alloc_cq_buf(hr_dev, &hr_cq->hr_buf,
					       cq_entries);
		if (ret) {
			dev_err(dev, "Failed to alloc_cq_buf.\n");
			goto err_db;
		}

		uar = &hr_dev->priv_uar;
		hr_cq->cq_db_l = hr_dev->reg_base + hr_dev->odb_offset +
				DB_REG_OFFSET * uar->index;
	}

	/* Allocate cq index, fill cq_context */
	ret = hns_roce_cq_alloc(hr_dev, cq_entries, &hr_cq->hr_buf.hr_mtt, uar,
				hr_cq, vector);
	if (ret) {
		dev_err(dev, "Creat CQ .Failed to cq_alloc.\n");
		goto err_dbmap;
=======
	if (refcount_dec_and_test(&hr_cq->refcount))
		complete(&hr_cq->free);
	wait_for_completion(&hr_cq->free);

	hns_roce_table_put(hr_dev, &cq_table->table, hr_cq->cqn);
}

static int alloc_cq_buf(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq,
			struct ib_udata *udata, unsigned long addr)
{
	struct ib_device *ibdev = &hr_dev->ib_dev;
	struct hns_roce_buf_attr buf_attr = {};
	int ret;

	buf_attr.page_shift = hr_dev->caps.cqe_buf_pg_sz + PAGE_SHIFT;
	buf_attr.region[0].size = hr_cq->cq_depth * hr_cq->cqe_size;
	buf_attr.region[0].hopnum = hr_dev->caps.cqe_hop_num;
	buf_attr.region_count = 1;

	ret = hns_roce_mtr_create(hr_dev, &hr_cq->mtr, &buf_attr,
				  hr_dev->caps.cqe_ba_pg_sz + PAGE_SHIFT,
				  udata, addr);
	if (ret)
		ibdev_err(ibdev, "failed to alloc CQ mtr, ret = %d.\n", ret);

	return ret;
}

static void free_cq_buf(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq)
{
	hns_roce_mtr_destroy(hr_dev, &hr_cq->mtr);
}

static int alloc_cq_db(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq,
		       struct ib_udata *udata, unsigned long addr,
		       struct hns_roce_ib_create_cq_resp *resp)
{
	bool has_db = hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_CQ_RECORD_DB;
	struct hns_roce_ucontext *uctx;
	int err;

	if (udata) {
		if (has_db &&
		    udata->outlen >= offsetofend(typeof(*resp), cap_flags)) {
			uctx = rdma_udata_to_drv_context(udata,
					struct hns_roce_ucontext, ibucontext);
			err = hns_roce_db_map_user(uctx, addr, &hr_cq->db);
			if (err)
				return err;
			hr_cq->flags |= HNS_ROCE_CQ_FLAG_RECORD_DB;
			resp->cap_flags |= HNS_ROCE_CQ_FLAG_RECORD_DB;
		}
	} else {
		if (has_db) {
			err = hns_roce_alloc_db(hr_dev, &hr_cq->db, 1);
			if (err)
				return err;
			hr_cq->set_ci_db = hr_cq->db.db_record;
			*hr_cq->set_ci_db = 0;
			hr_cq->flags |= HNS_ROCE_CQ_FLAG_RECORD_DB;
		}
		hr_cq->db_reg = hr_dev->reg_base + hr_dev->odb_offset +
				DB_REG_OFFSET * hr_dev->priv_uar.index;
	}

	return 0;
}

static void free_cq_db(struct hns_roce_dev *hr_dev, struct hns_roce_cq *hr_cq,
		       struct ib_udata *udata)
{
	struct hns_roce_ucontext *uctx;

	if (!(hr_cq->flags & HNS_ROCE_CQ_FLAG_RECORD_DB))
		return;

	hr_cq->flags &= ~HNS_ROCE_CQ_FLAG_RECORD_DB;
	if (udata) {
		uctx = rdma_udata_to_drv_context(udata,
						 struct hns_roce_ucontext,
						 ibucontext);
		hns_roce_db_unmap_user(uctx, &hr_cq->db);
	} else {
		hns_roce_free_db(hr_dev, &hr_cq->db);
	}
}

static int verify_cq_create_attr(struct hns_roce_dev *hr_dev,
				 const struct ib_cq_init_attr *attr)
{
	struct ib_device *ibdev = &hr_dev->ib_dev;

	if (!attr->cqe || attr->cqe > hr_dev->caps.max_cqes) {
		ibdev_err(ibdev, "failed to check CQ count %u, max = %u.\n",
			  attr->cqe, hr_dev->caps.max_cqes);
		return -EINVAL;
	}

	if (attr->comp_vector >= hr_dev->caps.num_comp_vectors) {
		ibdev_err(ibdev, "failed to check CQ vector = %u, max = %d.\n",
			  attr->comp_vector, hr_dev->caps.num_comp_vectors);
		return -EINVAL;
	}

	return 0;
}

static int get_cq_ucmd(struct hns_roce_cq *hr_cq, struct ib_udata *udata,
		       struct hns_roce_ib_create_cq *ucmd)
{
	struct ib_device *ibdev = hr_cq->ib_cq.device;
	int ret;

	ret = ib_copy_from_udata(ucmd, udata, min(udata->inlen, sizeof(*ucmd)));
	if (ret) {
		ibdev_err(ibdev, "failed to copy CQ udata, ret = %d.\n", ret);
		return ret;
	}

	return 0;
}

static void set_cq_param(struct hns_roce_cq *hr_cq, u32 cq_entries, int vector,
			 struct hns_roce_ib_create_cq *ucmd)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(hr_cq->ib_cq.device);

	cq_entries = max(cq_entries, hr_dev->caps.min_cqes);
	cq_entries = roundup_pow_of_two(cq_entries);
	hr_cq->ib_cq.cqe = cq_entries - 1; /* used as cqe index */
	hr_cq->cq_depth = cq_entries;
	hr_cq->vector = vector;

	spin_lock_init(&hr_cq->lock);
	INIT_LIST_HEAD(&hr_cq->sq_list);
	INIT_LIST_HEAD(&hr_cq->rq_list);
}

static int set_cqe_size(struct hns_roce_cq *hr_cq, struct ib_udata *udata,
			struct hns_roce_ib_create_cq *ucmd)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(hr_cq->ib_cq.device);

	if (!udata) {
		hr_cq->cqe_size = hr_dev->caps.cqe_sz;
		return 0;
	}

	if (udata->inlen >= offsetofend(typeof(*ucmd), cqe_size)) {
		if (ucmd->cqe_size != HNS_ROCE_V2_CQE_SIZE &&
		    ucmd->cqe_size != HNS_ROCE_V3_CQE_SIZE) {
			ibdev_err(&hr_dev->ib_dev,
				  "invalid cqe size %u.\n", ucmd->cqe_size);
			return -EINVAL;
		}

		hr_cq->cqe_size = ucmd->cqe_size;
	} else {
		hr_cq->cqe_size = HNS_ROCE_V2_CQE_SIZE;
	}

	return 0;
}

int hns_roce_create_cq(struct ib_cq *ib_cq, const struct ib_cq_init_attr *attr,
		       struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_cq->device);
	struct hns_roce_ib_create_cq_resp resp = {};
	struct hns_roce_cq *hr_cq = to_hr_cq(ib_cq);
	struct ib_device *ibdev = &hr_dev->ib_dev;
	struct hns_roce_ib_create_cq ucmd = {};
	int ret;

	if (attr->flags)
		return -EOPNOTSUPP;

	ret = verify_cq_create_attr(hr_dev, attr);
	if (ret)
		return ret;

	if (udata) {
		ret = get_cq_ucmd(hr_cq, udata, &ucmd);
		if (ret)
			return ret;
	}

	set_cq_param(hr_cq, attr->cqe, attr->comp_vector, &ucmd);

	ret = set_cqe_size(hr_cq, udata, &ucmd);
	if (ret)
		return ret;

	ret = alloc_cq_buf(hr_dev, hr_cq, udata, ucmd.buf_addr);
	if (ret) {
		ibdev_err(ibdev, "failed to alloc CQ buf, ret = %d.\n", ret);
		return ret;
	}

	ret = alloc_cq_db(hr_dev, hr_cq, udata, ucmd.db_addr, &resp);
	if (ret) {
		ibdev_err(ibdev, "failed to alloc CQ db, ret = %d.\n", ret);
		goto err_cq_buf;
	}

	ret = alloc_cqn(hr_dev, hr_cq);
	if (ret) {
		ibdev_err(ibdev, "failed to alloc CQN, ret = %d.\n", ret);
		goto err_cq_db;
	}

	ret = alloc_cqc(hr_dev, hr_cq);
	if (ret) {
		ibdev_err(ibdev,
			  "failed to alloc CQ context, ret = %d.\n", ret);
		goto err_cqn;
>>>>>>> upstream/android-13
	}

	/*
	 * For the QP created by kernel space, tptr value should be initialized
	 * to zero; For the QP created by user space, it will cause synchronous
<<<<<<< HEAD
	 * problems if tptr is set to zero here, so we initialze it in user
	 * space.
	 */
	if (!context && hr_cq->tptr_addr)
		*hr_cq->tptr_addr = 0;

	/* Get created cq handler and carry out event */
	hr_cq->comp = hns_roce_ib_cq_comp;
	hr_cq->event = hns_roce_ib_cq_event;
	hr_cq->cq_depth = cq_entries;

	if (context) {
		resp.cqn = hr_cq->cqn;
		ret = ib_copy_to_udata(udata, &resp, sizeof(resp));
=======
	 * problems if tptr is set to zero here, so we initialize it in user
	 * space.
	 */
	if (!udata && hr_cq->tptr_addr)
		*hr_cq->tptr_addr = 0;

	if (udata) {
		resp.cqn = hr_cq->cqn;
		ret = ib_copy_to_udata(udata, &resp,
				       min(udata->outlen, sizeof(resp)));
>>>>>>> upstream/android-13
		if (ret)
			goto err_cqc;
	}

<<<<<<< HEAD
	return &hr_cq->ib_cq;

err_cqc:
	hns_roce_free_cq(hr_dev, hr_cq);

err_dbmap:
	if (context && (hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_RECORD_DB) &&
	    (udata->outlen >= sizeof(resp)))
		hns_roce_db_unmap_user(to_hr_ucontext(context),
				       &hr_cq->db);

err_mtt:
	hns_roce_mtt_cleanup(hr_dev, &hr_cq->hr_buf.hr_mtt);
	if (context)
		ib_umem_release(hr_cq->umem);
	else
		hns_roce_ib_free_cq_buf(hr_dev, &hr_cq->hr_buf,
					hr_cq->ib_cq.cqe);

err_db:
	if (!context && (hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_RECORD_DB))
		hns_roce_free_db(hr_dev, &hr_cq->db);

err_cq:
	kfree(hr_cq);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL_GPL(hns_roce_ib_create_cq);

int hns_roce_ib_destroy_cq(struct ib_cq *ib_cq)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_cq->device);
	struct hns_roce_cq *hr_cq = to_hr_cq(ib_cq);
	int ret = 0;

	if (hr_dev->hw->destroy_cq) {
		ret = hr_dev->hw->destroy_cq(ib_cq);
	} else {
		hns_roce_free_cq(hr_dev, hr_cq);
		hns_roce_mtt_cleanup(hr_dev, &hr_cq->hr_buf.hr_mtt);

		if (ib_cq->uobject) {
			ib_umem_release(hr_cq->umem);

			if (hr_cq->db_en == 1)
				hns_roce_db_unmap_user(
					to_hr_ucontext(ib_cq->uobject->context),
					&hr_cq->db);
		} else {
			/* Free the buff of stored cq */
			hns_roce_ib_free_cq_buf(hr_dev, &hr_cq->hr_buf,
						ib_cq->cqe);
			if (hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_RECORD_DB)
				hns_roce_free_db(hr_dev, &hr_cq->db);
		}

		kfree(hr_cq);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(hns_roce_ib_destroy_cq);

void hns_roce_cq_completion(struct hns_roce_dev *hr_dev, u32 cqn)
{
	struct device *dev = hr_dev->dev;
	struct hns_roce_cq *cq;

	cq = radix_tree_lookup(&hr_dev->cq_table.tree,
			       cqn & (hr_dev->caps.num_cqs - 1));
	if (!cq) {
		dev_warn(dev, "Completion event for bogus CQ 0x%08x\n", cqn);
		return;
	}

	++cq->arm_sn;
	cq->comp(cq);
}
EXPORT_SYMBOL_GPL(hns_roce_cq_completion);

void hns_roce_cq_event(struct hns_roce_dev *hr_dev, u32 cqn, int event_type)
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;
	struct device *dev = hr_dev->dev;
	struct hns_roce_cq *cq;

	cq = radix_tree_lookup(&cq_table->tree,
			       cqn & (hr_dev->caps.num_cqs - 1));
	if (cq)
		atomic_inc(&cq->refcount);

	if (!cq) {
		dev_warn(dev, "Async event for bogus CQ %08x\n", cqn);
		return;
	}

	cq->event(cq, (enum hns_roce_event)event_type);

	if (atomic_dec_and_test(&cq->refcount))
		complete(&cq->free);
}
EXPORT_SYMBOL_GPL(hns_roce_cq_event);

int hns_roce_init_cq_table(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;

	spin_lock_init(&cq_table->lock);
	INIT_RADIX_TREE(&cq_table->tree, GFP_ATOMIC);

	return hns_roce_bitmap_init(&cq_table->bitmap, hr_dev->caps.num_cqs,
				    hr_dev->caps.num_cqs - 1,
				    hr_dev->caps.reserved_cqs, 0);
=======
	return 0;

err_cqc:
	free_cqc(hr_dev, hr_cq);
err_cqn:
	free_cqn(hr_dev, hr_cq->cqn);
err_cq_db:
	free_cq_db(hr_dev, hr_cq, udata);
err_cq_buf:
	free_cq_buf(hr_dev, hr_cq);
	return ret;
}

int hns_roce_destroy_cq(struct ib_cq *ib_cq, struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_cq->device);
	struct hns_roce_cq *hr_cq = to_hr_cq(ib_cq);

	if (hr_dev->hw->destroy_cq)
		hr_dev->hw->destroy_cq(ib_cq, udata);

	free_cqc(hr_dev, hr_cq);
	free_cqn(hr_dev, hr_cq->cqn);
	free_cq_db(hr_dev, hr_cq, udata);
	free_cq_buf(hr_dev, hr_cq);

	return 0;
}

void hns_roce_cq_completion(struct hns_roce_dev *hr_dev, u32 cqn)
{
	struct hns_roce_cq *hr_cq;
	struct ib_cq *ibcq;

	hr_cq = xa_load(&hr_dev->cq_table.array,
			cqn & (hr_dev->caps.num_cqs - 1));
	if (!hr_cq) {
		dev_warn(hr_dev->dev, "Completion event for bogus CQ 0x%06x\n",
			 cqn);
		return;
	}

	++hr_cq->arm_sn;
	ibcq = &hr_cq->ib_cq;
	if (ibcq->comp_handler)
		ibcq->comp_handler(ibcq, ibcq->cq_context);
}

void hns_roce_cq_event(struct hns_roce_dev *hr_dev, u32 cqn, int event_type)
{
	struct device *dev = hr_dev->dev;
	struct hns_roce_cq *hr_cq;
	struct ib_event event;
	struct ib_cq *ibcq;

	hr_cq = xa_load(&hr_dev->cq_table.array,
			cqn & (hr_dev->caps.num_cqs - 1));
	if (!hr_cq) {
		dev_warn(dev, "Async event for bogus CQ 0x%06x\n", cqn);
		return;
	}

	if (event_type != HNS_ROCE_EVENT_TYPE_CQ_ID_INVALID &&
	    event_type != HNS_ROCE_EVENT_TYPE_CQ_ACCESS_ERROR &&
	    event_type != HNS_ROCE_EVENT_TYPE_CQ_OVERFLOW) {
		dev_err(dev, "Unexpected event type 0x%x on CQ 0x%06x\n",
			event_type, cqn);
		return;
	}

	refcount_inc(&hr_cq->refcount);

	ibcq = &hr_cq->ib_cq;
	if (ibcq->event_handler) {
		event.device = ibcq->device;
		event.element.cq = ibcq;
		event.event = IB_EVENT_CQ_ERR;
		ibcq->event_handler(&event, ibcq->cq_context);
	}

	if (refcount_dec_and_test(&hr_cq->refcount))
		complete(&hr_cq->free);
}

void hns_roce_init_cq_table(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_cq_table *cq_table = &hr_dev->cq_table;
	unsigned int reserved_from_bot;
	unsigned int i;

	mutex_init(&cq_table->bank_mutex);
	xa_init(&cq_table->array);

	reserved_from_bot = hr_dev->caps.reserved_cqs;

	for (i = 0; i < reserved_from_bot; i++) {
		cq_table->bank[get_cq_bankid(i)].inuse++;
		cq_table->bank[get_cq_bankid(i)].min++;
	}

	for (i = 0; i < HNS_ROCE_CQ_BANK_NUM; i++) {
		ida_init(&cq_table->bank[i].ida);
		cq_table->bank[i].max = hr_dev->caps.num_cqs /
					HNS_ROCE_CQ_BANK_NUM - 1;
	}
>>>>>>> upstream/android-13
}

void hns_roce_cleanup_cq_table(struct hns_roce_dev *hr_dev)
{
<<<<<<< HEAD
	hns_roce_bitmap_cleanup(&hr_dev->cq_table.bitmap);
=======
	int i;

	for (i = 0; i < HNS_ROCE_CQ_BANK_NUM; i++)
		ida_destroy(&hr_dev->cq_table.bank[i].ida);
>>>>>>> upstream/android-13
}
