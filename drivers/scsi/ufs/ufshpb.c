// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * Copyright (C) 2017-2018 Samsung Electronics Co., Ltd.
 */

#include "ufshcd.h"
#include "ufshpb.h"

#define UFSHCD_REQ_SENSE_SIZE	18

/*
 * define global constants
 */
static int sects_per_blk_shift;
static int bits_per_dword_shift;
static int bits_per_dword_mask;
static int bits_per_byte_shift;

static int ufshpb_create_sysfs(struct ufsf_feature *ufsf,
			       struct ufshpb_lu *hpb);
static int ufshpb_remove_sysfs(struct ufshpb_lu *hpb);

static inline void
=======
 * Universal Flash Storage Host Performance Booster
 *
 * Copyright (C) 2017-2021 Samsung Electronics Co., Ltd.
 *
 * Authors:
 *	Yongmyung Lee <ymhungry.lee@samsung.com>
 *	Jinyoung Choi <j-young.choi@samsung.com>
 */

#include <asm/unaligned.h>
#include <linux/async.h>

#include "ufshcd.h"
#include "ufshpb.h"
#include "../sd.h"

#define ACTIVATION_THRESHOLD 8 /* 8 IOs */
#define READ_TO_MS 1000
#define READ_TO_EXPIRIES 100
#define POLLING_INTERVAL_MS 200
#define THROTTLE_MAP_REQ_DEFAULT 1

/* memory management */
static struct kmem_cache *ufshpb_mctx_cache;
static mempool_t *ufshpb_mctx_pool;
static mempool_t *ufshpb_page_pool;
/* A cache size of 2MB can cache ppn in the 1GB range. */
static unsigned int ufshpb_host_map_kbytes = 2048;
static int tot_active_srgn_pages;

static struct workqueue_struct *ufshpb_wq;

static void ufshpb_update_active_info(struct ufshpb_lu *hpb, int rgn_idx,
				      int srgn_idx);

bool ufshpb_is_allowed(struct ufs_hba *hba)
{
	return !(hba->ufshpb_dev.hpb_disabled);
}

/* HPB version 1.0 is called as legacy version. */
bool ufshpb_is_legacy(struct ufs_hba *hba)
{
	return hba->ufshpb_dev.is_legacy;
}

static struct ufshpb_lu *ufshpb_get_hpb_data(struct scsi_device *sdev)
{
	return sdev->hostdata;
}

static int ufshpb_get_state(struct ufshpb_lu *hpb)
{
	return atomic_read(&hpb->hpb_state);
}

static void ufshpb_set_state(struct ufshpb_lu *hpb, int state)
{
	atomic_set(&hpb->hpb_state, state);
}

static int ufshpb_is_valid_srgn(struct ufshpb_region *rgn,
				struct ufshpb_subregion *srgn)
{
	return rgn->rgn_state != HPB_RGN_INACTIVE &&
		srgn->srgn_state == HPB_SRGN_VALID;
}

static bool ufshpb_is_read_cmd(struct scsi_cmnd *cmd)
{
	return req_op(scsi_cmd_to_rq(cmd)) == REQ_OP_READ;
}

static bool ufshpb_is_write_or_discard(struct scsi_cmnd *cmd)
{
	return op_is_write(req_op(scsi_cmd_to_rq(cmd))) ||
	       op_is_discard(req_op(scsi_cmd_to_rq(cmd)));
}

static bool ufshpb_is_supported_chunk(struct ufshpb_lu *hpb, int transfer_len)
{
	return transfer_len <= hpb->pre_req_max_tr_len;
}

static bool ufshpb_is_general_lun(int lun)
{
	return lun < UFS_UPIU_MAX_UNIT_NUM_ID;
}

static bool ufshpb_is_pinned_region(struct ufshpb_lu *hpb, int rgn_idx)
{
	if (hpb->lu_pinned_end != PINNED_NOT_SET &&
	    rgn_idx >= hpb->lu_pinned_start &&
	    rgn_idx <= hpb->lu_pinned_end)
		return true;

	return false;
}

static void ufshpb_kick_map_work(struct ufshpb_lu *hpb)
{
	bool ret = false;
	unsigned long flags;

	if (ufshpb_get_state(hpb) != HPB_PRESENT)
		return;

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	if (!list_empty(&hpb->lh_inact_rgn) || !list_empty(&hpb->lh_act_srgn))
		ret = true;
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

	if (ret)
		queue_work(ufshpb_wq, &hpb->map_work);
}

static bool ufshpb_is_hpb_rsp_valid(struct ufs_hba *hba,
				    struct ufshcd_lrb *lrbp,
				    struct utp_hpb_rsp *rsp_field)
{
	/* Check HPB_UPDATE_ALERT */
	if (!(lrbp->ucd_rsp_ptr->header.dword_2 &
	      UPIU_HEADER_DWORD(0, 2, 0, 0)))
		return false;

	if (be16_to_cpu(rsp_field->sense_data_len) != DEV_SENSE_SEG_LEN ||
	    rsp_field->desc_type != DEV_DES_TYPE ||
	    rsp_field->additional_len != DEV_ADDITIONAL_LEN ||
	    rsp_field->active_rgn_cnt > MAX_ACTIVE_NUM ||
	    rsp_field->inactive_rgn_cnt > MAX_INACTIVE_NUM ||
	    rsp_field->hpb_op == HPB_RSP_NONE ||
	    (rsp_field->hpb_op == HPB_RSP_REQ_REGION_UPDATE &&
	     !rsp_field->active_rgn_cnt && !rsp_field->inactive_rgn_cnt))
		return false;

	if (!ufshpb_is_general_lun(rsp_field->lun)) {
		dev_warn(hba->dev, "ufshpb: lun(%d) not supported\n",
			 lrbp->lun);
		return false;
	}

	return true;
}

static void ufshpb_iterate_rgn(struct ufshpb_lu *hpb, int rgn_idx, int srgn_idx,
			       int srgn_offset, int cnt, bool set_dirty)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn, *prev_srgn = NULL;
	int set_bit_len;
	int bitmap_len;
	unsigned long flags;

next_srgn:
	rgn = hpb->rgn_tbl + rgn_idx;
	srgn = rgn->srgn_tbl + srgn_idx;

	if (likely(!srgn->is_last))
		bitmap_len = hpb->entries_per_srgn;
	else
		bitmap_len = hpb->last_srgn_entries;

	if ((srgn_offset + cnt) > bitmap_len)
		set_bit_len = bitmap_len - srgn_offset;
	else
		set_bit_len = cnt;

	spin_lock_irqsave(&hpb->rgn_state_lock, flags);
	if (rgn->rgn_state != HPB_RGN_INACTIVE) {
		if (set_dirty) {
			if (srgn->srgn_state == HPB_SRGN_VALID)
				bitmap_set(srgn->mctx->ppn_dirty, srgn_offset,
					   set_bit_len);
		} else if (hpb->is_hcm) {
			 /* rewind the read timer for lru regions */
			rgn->read_timeout = ktime_add_ms(ktime_get(),
					rgn->hpb->params.read_timeout_ms);
			rgn->read_timeout_expiries =
				rgn->hpb->params.read_timeout_expiries;
		}
	}
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);

	if (hpb->is_hcm && prev_srgn != srgn) {
		bool activate = false;

		spin_lock(&rgn->rgn_lock);
		if (set_dirty) {
			rgn->reads -= srgn->reads;
			srgn->reads = 0;
			set_bit(RGN_FLAG_DIRTY, &rgn->rgn_flags);
		} else {
			srgn->reads++;
			rgn->reads++;
			if (srgn->reads == hpb->params.activation_thld)
				activate = true;
		}
		spin_unlock(&rgn->rgn_lock);

		if (activate ||
		    test_and_clear_bit(RGN_FLAG_UPDATE, &rgn->rgn_flags)) {
			spin_lock_irqsave(&hpb->rsp_list_lock, flags);
			ufshpb_update_active_info(hpb, rgn_idx, srgn_idx);
			spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
			dev_dbg(&hpb->sdev_ufs_lu->sdev_dev,
				"activate region %d-%d\n", rgn_idx, srgn_idx);
		}

		prev_srgn = srgn;
	}

	srgn_offset = 0;
	if (++srgn_idx == hpb->srgns_per_rgn) {
		srgn_idx = 0;
		rgn_idx++;
	}

	cnt -= set_bit_len;
	if (cnt > 0)
		goto next_srgn;
}

static bool ufshpb_test_ppn_dirty(struct ufshpb_lu *hpb, int rgn_idx,
				  int srgn_idx, int srgn_offset, int cnt)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	int bitmap_len;
	int bit_len;

next_srgn:
	rgn = hpb->rgn_tbl + rgn_idx;
	srgn = rgn->srgn_tbl + srgn_idx;

	if (likely(!srgn->is_last))
		bitmap_len = hpb->entries_per_srgn;
	else
		bitmap_len = hpb->last_srgn_entries;

	if (!ufshpb_is_valid_srgn(rgn, srgn))
		return true;

	/*
	 * If the region state is active, mctx must be allocated.
	 * In this case, check whether the region is evicted or
	 * mctx allocation fail.
	 */
	if (unlikely(!srgn->mctx)) {
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			"no mctx in region %d subregion %d.\n",
			srgn->rgn_idx, srgn->srgn_idx);
		return true;
	}

	if ((srgn_offset + cnt) > bitmap_len)
		bit_len = bitmap_len - srgn_offset;
	else
		bit_len = cnt;

	if (find_next_bit(srgn->mctx->ppn_dirty, bit_len + srgn_offset,
			  srgn_offset) < bit_len + srgn_offset)
		return true;

	srgn_offset = 0;
	if (++srgn_idx == hpb->srgns_per_rgn) {
		srgn_idx = 0;
		rgn_idx++;
	}

	cnt -= bit_len;
	if (cnt > 0)
		goto next_srgn;

	return false;
}

static inline bool is_rgn_dirty(struct ufshpb_region *rgn)
{
	return test_bit(RGN_FLAG_DIRTY, &rgn->rgn_flags);
}

static int ufshpb_fill_ppn_from_page(struct ufshpb_lu *hpb,
				     struct ufshpb_map_ctx *mctx, int pos,
				     int len, __be64 *ppn_buf)
{
	struct page *page;
	int index, offset;
	int copied;

	index = pos / (PAGE_SIZE / HPB_ENTRY_SIZE);
	offset = pos % (PAGE_SIZE / HPB_ENTRY_SIZE);

	if ((offset + len) <= (PAGE_SIZE / HPB_ENTRY_SIZE))
		copied = len;
	else
		copied = (PAGE_SIZE / HPB_ENTRY_SIZE) - offset;

	page = mctx->m_page[index];
	if (unlikely(!page)) {
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			"error. cannot find page in mctx\n");
		return -ENOMEM;
	}

	memcpy(ppn_buf, page_address(page) + (offset * HPB_ENTRY_SIZE),
	       copied * HPB_ENTRY_SIZE);

	return copied;
}

static void
>>>>>>> upstream/android-13
ufshpb_get_pos_from_lpn(struct ufshpb_lu *hpb, unsigned long lpn, int *rgn_idx,
			int *srgn_idx, int *offset)
{
	int rgn_offset;

	*rgn_idx = lpn >> hpb->entries_per_rgn_shift;
	rgn_offset = lpn & hpb->entries_per_rgn_mask;
	*srgn_idx = rgn_offset >> hpb->entries_per_srgn_shift;
	*offset = rgn_offset & hpb->entries_per_srgn_mask;
}

<<<<<<< HEAD
inline int ufshpb_valid_srgn(struct ufshpb_region *rgn,
			     struct ufshpb_subregion *srgn)
{
	return rgn->rgn_state != HPBREGION_INACTIVE &&
		srgn->srgn_state == HPBSUBREGION_CLEAN;
}

/* Must be held hpb_lock  */
static bool ufshpb_ppn_dirty_check(struct ufshpb_lu *hpb, unsigned long lpn,
				   int transfer_len)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	unsigned long cur_lpn = lpn;
	int rgn_idx, srgn_idx, srgn_offset, find_size;
	int scan_cnt = transfer_len;

	do {
		ufshpb_get_pos_from_lpn(hpb, cur_lpn, &rgn_idx, &srgn_idx,
					&srgn_offset);
		rgn = hpb->rgn_tbl + rgn_idx;
		srgn = rgn->srgn_tbl + srgn_idx;

		if (!ufshpb_valid_srgn(rgn, srgn))
			return true;

		if (!srgn->mctx || !srgn->mctx->ppn_dirty)
			return true;

		if (hpb->entries_per_srgn < srgn_offset + scan_cnt) {
			find_size = hpb->entries_per_srgn - srgn_offset;
			scan_cnt -= find_size;
		} else {
			find_size = srgn_offset + scan_cnt;
			scan_cnt = 0;
		}

		srgn_offset =
			find_next_bit((unsigned long *)srgn->mctx->ppn_dirty,
				      hpb->entries_per_srgn, srgn_offset);

		if (srgn_offset < hpb->entries_per_srgn)
			return srgn_offset < find_size;

		cur_lpn += find_size;
	} while (scan_cnt);

	return false;
}

static void ufshpb_set_read16_cmd(struct ufshpb_lu *hpb,
				  struct ufshcd_lrb *lrbp,
				  unsigned long long ppn,
				  unsigned int transfer_len)
{
	unsigned char *cdb = lrbp->cmd->cmnd;

	cdb[0] = READ_16;
	cdb[2] = lrbp->cmd->cmnd[2];
	cdb[3] = lrbp->cmd->cmnd[3];
	cdb[4] = lrbp->cmd->cmnd[4];
	cdb[5] = lrbp->cmd->cmnd[5];
	cdb[6] = GET_BYTE_7(ppn);
	cdb[7] = GET_BYTE_6(ppn);
	cdb[8] = GET_BYTE_5(ppn);
	cdb[9] = GET_BYTE_4(ppn);
	cdb[10] = GET_BYTE_3(ppn);
	cdb[11] = GET_BYTE_2(ppn);
	cdb[12] = GET_BYTE_1(ppn);
	cdb[13] = GET_BYTE_0(ppn);

	if (lrbp->hpb_ctx_id < MAX_HPB_CONTEXT_ID)
		cdb[14] = (1 << 7) | lrbp->hpb_ctx_id;
	else
		cdb[14] = UFSHPB_GROUP_NUMBER;

	cdb[15] = transfer_len;

	lrbp->cmd->cmd_len = MAX_CDB_SIZE;
}

/* called with hpb_lock (irq) */
static inline void
ufshpb_set_dirty_bits(struct ufshpb_lu *hpb, struct ufshpb_region *rgn,
		      struct ufshpb_subregion *srgn, int dword, int offset,
		      unsigned int cnt)
{
	const unsigned long mask = ((1UL << cnt) - 1) & 0xffffffff;

	if (rgn->rgn_state == HPBREGION_INACTIVE)
		return;

	BUG_ON(!srgn->mctx);
	srgn->mctx->ppn_dirty[dword] |= (mask << offset);
}

static inline void ufshpb_get_bit_offset(struct ufshpb_lu *hpb, int srgn_offset,
					 int *dword, int *offset)
{
	*dword = srgn_offset >> bits_per_dword_shift;
	*offset = srgn_offset & bits_per_dword_mask;
}

static void ufshpb_set_dirty(struct ufshpb_lu *hpb, struct ufshcd_lrb *lrbp,
			     int rgn_idx, int srgn_idx, int srgn_offset)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	int cnt, bit_cnt, bit_dword, bit_offset;

	cnt = blk_rq_sectors(lrbp->cmd->request) >> sects_per_blk_shift;
	ufshpb_get_bit_offset(hpb, srgn_offset, &bit_dword, &bit_offset);

	do {
		bit_cnt = min(cnt, BITS_PER_DWORD - bit_offset);

		rgn = hpb->rgn_tbl + rgn_idx;
		srgn = rgn->srgn_tbl + srgn_idx;

		ufshpb_set_dirty_bits(hpb, rgn, srgn, bit_dword, bit_offset,
				      bit_cnt);

		bit_offset = 0;
		bit_dword++;

		if (bit_dword == hpb->dwords_per_srgn) {
			bit_dword = 0;
			srgn_idx++;

			if (srgn_idx == hpb->srgns_per_rgn) {
				srgn_idx = 0;
				rgn_idx++;
			}
		}
		cnt -= bit_cnt;
	} while (cnt);

	BUG_ON(cnt < 0);
}

static inline bool ufshpb_is_read_cmd(struct scsi_cmnd *cmd)
{
	if (cmd->cmnd[0] == READ_10 || cmd->cmnd[0] == READ_16)
		return true;

	return false;
}

static inline bool ufshpb_is_write_discard_lrbp(struct ufshcd_lrb *lrbp)
{
	if (lrbp->cmd->cmnd[0] == WRITE_10 || lrbp->cmd->cmnd[0] == WRITE_16 ||
	    lrbp->cmd->cmnd[0] == UNMAP)
		return true;

	return false;
}

static unsigned long long ufshpb_get_ppn(struct ufshpb_map_ctx *mctx, int pos,
					 int *error)
{
	unsigned long long *ppn_table;
	struct page *page = NULL;
	int index, offset;

	index = pos / HPB_ENTREIS_PER_OS_PAGE;
	offset = pos % HPB_ENTREIS_PER_OS_PAGE;

	page = mctx->m_page[index];
	if (!page) {
		*error = -ENOMEM;
		ERR_MSG("mctx %p cannot get m_page", mctx);
		return 0;
	}

	ppn_table = page_address(page);
	if (!ppn_table) {
		*error = -ENOMEM;
		ERR_MSG("mctx %p cannot get ppn_table vm", mctx);
		return 0;
	}

	return ppn_table[offset];
}

static inline int ufshpb_lu_get(struct ufshpb_lu *hpb)
{
	if (!hpb || hpb->ufsf->ufshpb_state != HPB_PRESENT)
		return -ENODEV;

	kref_get(&hpb->ufsf->ufshpb_kref);
	return 0;
}

static inline void ufshpb_schedule_error_handler(struct kref *kref)
{
	struct ufsf_feature *ufsf;

	ufsf = container_of(kref, struct ufsf_feature, ufshpb_kref);
	schedule_work(&ufsf->ufshpb_eh_work);
}

static inline void ufshpb_lu_put(struct ufshpb_lu *hpb)
{
	kref_put(&hpb->ufsf->ufshpb_kref, ufshpb_schedule_error_handler);
}

static void ufshpb_failed(struct ufshpb_lu *hpb, const char *f)
{
	ERR_MSG("ufshpb_driver failed. function (%s)", f);
	hpb->ufsf->ufshpb_state = HPB_FAILED;
	ufshpb_lu_put(hpb);
}

static inline void ufshpb_put_pre_req(struct ufshpb_lu *hpb,
				      struct ufshpb_req *pre_req)
{
	list_add_tail(&pre_req->list_req, &hpb->lh_pre_req_free);
	hpb->num_inflight_pre_req--;
}

static struct ufshpb_req *ufshpb_get_pre_req(struct ufshpb_lu *hpb)
{
	struct ufshpb_req *pre_req;

	if (hpb->num_inflight_pre_req >= hpb->throttle_pre_req) {
		HPB_DEBUG(hpb, "pre_req throttle. inflight %d throttle %d",
			  hpb->num_inflight_pre_req, hpb->throttle_pre_req);
		return NULL;
	}

	pre_req = list_first_entry_or_null(&hpb->lh_pre_req_free,
					   struct ufshpb_req,
					   list_req);
	if (!pre_req) {
		HPB_DEBUG(hpb, "There is no pre_req");
		return NULL;
	}

	list_del_init(&pre_req->list_req);
	hpb->num_inflight_pre_req++;

	return pre_req;
}

static void ufshpb_pre_req_compl_fn(struct request *req, blk_status_t error)
{
	struct ufshpb_req *pre_req = (struct ufshpb_req *)req->end_io_data;
	struct ufshpb_lu *hpb = pre_req->hpb;
	unsigned long flags;
	struct scsi_sense_hdr sshdr;

	if (error) {
		ERR_MSG("error number %d", error);
		scsi_normalize_sense(pre_req->sense, SCSI_SENSE_BUFFERSIZE,
				     &sshdr);
		ERR_MSG("code %x sense_key %x asc %x ascq %x",
			sshdr.response_code,
			sshdr.sense_key, sshdr.asc, sshdr.ascq);
		ERR_MSG("byte4 %x byte5 %x byte6 %x additional_len %x",
			sshdr.byte4, sshdr.byte5,
			sshdr.byte6, sshdr.additional_length);
	}

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	ufshpb_put_pre_req(pre_req->hpb, pre_req);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);

	ufshpb_lu_put(pre_req->hpb);
}

static int ufshpb_prep_entry(struct ufshpb_req *pre_req,
			     struct page *page)
{
	struct ufshpb_lu *hpb = pre_req->hpb;
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	unsigned long long *addr;
	unsigned long long entry_ppn = 0;
	unsigned long lpn = pre_req->wb.lpn;
	int rgn_idx, srgn_idx, srgn_offset;
	int i, error = 0;
	unsigned long flags;

	addr = page_address(page);

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	for (i = 0; i < pre_req->wb.len; i++, lpn++) {
		ufshpb_get_pos_from_lpn(hpb, lpn, &rgn_idx, &srgn_idx,
					&srgn_offset);

		rgn = hpb->rgn_tbl + rgn_idx;
		srgn = rgn->srgn_tbl + srgn_idx;

		if (!ufshpb_valid_srgn(rgn, srgn))
			goto mctx_error;

		if (!srgn->mctx)
			goto mctx_error;

		entry_ppn = ufshpb_get_ppn(srgn->mctx, srgn_offset, &error);
		if (error)
			goto mctx_error;

		addr[i] = entry_ppn;
	}
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return 0;
mctx_error:
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return -ENOMEM;
}

static int ufshpb_pre_req_add_bio_page(struct request_queue *q,
				       struct ufshpb_req *pre_req)
{
	struct page *page = pre_req->wb.m_page;
	struct bio *bio = pre_req->bio;
	int ret;

	BUG_ON(!page);

	bio_reset(bio);

	ret = ufshpb_prep_entry(pre_req, page);
	if (ret)
		return ret;

	ret = bio_add_pc_page(q, bio, page, OS_PAGE_SIZE, 0);
	if (ret != OS_PAGE_SIZE) {
		ERR_MSG("bio_add_pc_page fail: %d", ret);
		return -ENOMEM;
	}

	return 0;
}

static void ufshpb_init_cmd_errh(struct scsi_cmnd *cmd)
{
	cmd->serial_number = 0;
	scsi_set_resid(cmd, 0);
	memset(cmd->sense_buffer, 0, SCSI_SENSE_BUFFERSIZE);
	if (cmd->cmd_len == 0)
		cmd->cmd_len = scsi_command_size(cmd->cmnd);
}

static void ufshpb_pre_req_done(struct scsi_cmnd *cmd)
{
	blk_complete_request(cmd->request);
}

static inline unsigned long ufshpb_get_lpn(struct request *rq)
{
	return blk_rq_pos(rq) / SECTORS_PER_BLOCK;
}

static inline unsigned int ufshpb_get_len(struct request *rq)
{
	return blk_rq_sectors(rq) / SECTORS_PER_BLOCK;
}

static inline unsigned int ufshpb_is_unaligned(struct request *rq)
{
	return blk_rq_sectors(rq) % SECTORS_PER_BLOCK;
}

static inline int ufshpb_issue_ctx_id_ticket(struct ufshpb_lu *hpb)
{
	int hpb_ctx_id;

	hpb->ctx_id_ticket++;
	if (hpb->ctx_id_ticket >= MAX_HPB_CONTEXT_ID)
		hpb->ctx_id_ticket = 0;
	hpb_ctx_id = hpb->ctx_id_ticket;

	return hpb_ctx_id;
}

static inline void ufshpb_set_write_buf_cmd(unsigned char *cdb,
					    unsigned long lpn, unsigned int len,
					    int hpb_ctx_id)
{
	int len_byte = len * HPB_ENTRY_SIZE;

	cdb[0] = UFSHPB_WRITE_BUFFER;
	cdb[1] = UFSHPB_WRITE_BUFFER_ID;
	cdb[2] = GET_BYTE_3(lpn);
	cdb[3] = GET_BYTE_2(lpn);
	cdb[4] = GET_BYTE_1(lpn);
	cdb[5] = GET_BYTE_0(lpn);
	cdb[6] = (1 << 7) | hpb_ctx_id;
	cdb[7] = GET_BYTE_1(len_byte);
	cdb[8] = GET_BYTE_0(len_byte);
	cdb[9] = 0x00;	/* Control = 0x00 */
}

static void ufshpb_mimic_scsi_release_buffers(struct scsi_cmnd *cmd)
{
	if (cmd->sdb.table.nents)
		sg_free_table_chained(&cmd->sdb.table, false);

	memset(&cmd->sdb, 0, sizeof(cmd->sdb));

	if (scsi_prot_sg_count(cmd))
		sg_free_table_chained(&cmd->prot_sdb->table, false);
}

static inline void ufshpb_mimic_scsi_dispatch_cmd(struct scsi_cmnd *cmd)
{
	atomic_inc(&cmd->device->iorequest_cnt);

	scsi_log_send(cmd);

	cmd->scsi_done = ufshpb_pre_req_done;
}

static int ufshpb_mimic_scsi_request_fn(struct ufshpb_lu *hpb,
					struct request *req)
{
	struct request_queue *q = req->q;
	struct scsi_device *sdev = q->queuedata;
	struct Scsi_Host *shost = sdev->host;
	struct scsi_cmnd *cmd;
	unsigned long flags;
	unsigned int busy;
	int ret = 0;

	spin_lock_irqsave(q->queue_lock, flags);
	req->rq_flags |= RQF_STARTED;

	ret = q->prep_rq_fn(q, req);
	if (unlikely(ret != BLKPREP_OK)) {
		HPB_DEBUG(hpb, "scsi_prep_fn is fail");
		ret = -EIO;
		goto prep_err;
	}
	cmd = req->special;
	if (unlikely(!cmd))
		BUG();

	busy = atomic_inc_return(&sdev->device_busy) - 1;
	if (busy >= sdev->queue_depth) {
		ret = -EAGAIN;
		goto finish_cmd;
	}

	/* lh_pre_req_free list is dummy head for blk_dequeue_request() */
	list_add_tail(&req->queuelist, &hpb->lh_pre_req_dummy);
	ret = blk_queue_start_tag(q, req);
	if (ret) {
		list_del_init(&req->queuelist);
		ret = -EAGAIN;
		goto finish_cmd;
	}
	spin_unlock_irqrestore(q->queue_lock, flags);

	/*
	 * UFS device has multi luns, so starget is not used.
	 * In case of UFS, starget->can_queue <= 0.
	 */
	if (unlikely(scsi_target(sdev)->can_queue > 0))
		atomic_inc(&scsi_target(sdev)->target_busy);
	atomic_inc(&shost->host_busy);

	ufshpb_init_cmd_errh(cmd);

	ufshpb_mimic_scsi_dispatch_cmd(cmd);

	return ret;
finish_cmd:
	ufshpb_mimic_scsi_release_buffers(cmd);
	scsi_put_command(cmd);
	put_device(&sdev->sdev_gendev);
	req->special = NULL;
	atomic_dec(&sdev->device_busy);
prep_err:
	spin_unlock_irqrestore(q->queue_lock, flags);
	return ret;
}

static int ufshpb_set_pre_req(struct ufshpb_lu *hpb, struct scsi_cmnd *cmd,
			      struct ufshpb_req *pre_req, int hpb_ctx_id)
{
	struct scsi_device *sdev = cmd->device;
	struct request_queue *q = sdev->request_queue;
	struct request *req;
	struct scsi_request *rq;
	struct scsi_cmnd *scmd;
	struct bio *bio = pre_req->bio;
	int ret = 0;

	pre_req->hpb = hpb;
	pre_req->wb.lpn = ufshpb_get_lpn(cmd->request);
	pre_req->wb.len = ufshpb_get_len(cmd->request);

	ret = ufshpb_pre_req_add_bio_page(q, pre_req);
	if (ret)
		return ret;

	req = pre_req->req;

	/*
	 * blk_init_rl() -> alloc_request_size().
	 * q->init_rq_fn = scsi_old_init_rq behavior.
	 */
	scmd = (struct scsi_cmnd *)(req + 1);
	memset(scmd, 0, sizeof(*scmd));
	scmd->sense_buffer = pre_req->sense;
	scmd->req.sense = scmd->sense_buffer;

	/* blk_get_request behavior */
	blk_rq_init(q, req);
	q->initialize_rq_fn(req);

	/* 1. request setup */
	blk_rq_append_bio(req, &bio);
	req->cmd_flags = REQ_OP_WRITE | REQ_SYNC | REQ_OP_SCSI_OUT;
	req->rq_flags = RQF_QUIET | RQF_PREEMPT;
	req->timeout = msecs_to_jiffies(30000);
	req->end_io_data = (void *)pre_req;
	req->end_io = ufshpb_pre_req_compl_fn;

	/* 2. scsi_request setup */
	rq = scsi_req(req);
	ufshpb_set_write_buf_cmd(rq->cmd, pre_req->wb.lpn, pre_req->wb.len,
				 hpb_ctx_id);
	rq->cmd_len = scsi_command_size(rq->cmd);

	ret = ufshpb_mimic_scsi_request_fn(hpb, req);

	return ret;
}

static inline bool ufshpb_is_support_chunk(int transfer_len)
{
	return transfer_len <= HPB_MULTI_CHUNK_HIGH;
}

static int ufshpb_check_pre_req_cond(struct ufshpb_lu *hpb,
				     struct scsi_cmnd *cmd)
{
	struct request *rq = cmd->request;
	unsigned long flags;
	unsigned int transfer_len;
	unsigned int lpn;

	if (!ufshpb_is_read_cmd(cmd))
		return -EINVAL;

	if (ufshpb_is_unaligned(rq))
		return -EINVAL;

	transfer_len = ufshpb_get_len(rq);
	if (!transfer_len)
		return -EINVAL;

	if (!ufshpb_is_support_chunk(transfer_len))
		return -EINVAL;

	/*
	 * WRITE_BUFFER CMD support 36K (len=9) ~ 512K (len=128) default.
	 * it is possible to change range of transfer_len through sysfs.
	 */
	if (transfer_len < hpb->pre_req_min_tr_len ||
	    transfer_len > hpb->pre_req_max_tr_len)
		return -EINVAL;

	/*
	 * When the request is from ioctrl, the address in rq will be -1.
	 * At this case, the ufshpb_ppn_dirty_check() will check the address
	 * with 0xFFFFFFFF_FFFFFFFF (64bit) or 0xFFFFFFFF(32bit). This will
	 * cause unexpected behavier. So skip here.
	 */
	if ((long)blk_rq_pos(cmd->request) == -1)
		return -EINVAL;

	lpn = ufshpb_get_lpn(cmd->request);

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	if (ufshpb_ppn_dirty_check(hpb, lpn, transfer_len)) {
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);
		return -EINVAL;
	}
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);

	return 0;
}

void ufshpb_end_pre_req(struct ufsf_feature *ufsf, struct request *req)
{
	struct scsi_cmnd *scmd = (struct scsi_cmnd *)(req + 1);

	set_host_byte(scmd, DID_OK);

	scmd->scsi_done(scmd);
}

int ufshpb_prepare_pre_req(struct ufsf_feature *ufsf, struct scsi_cmnd *cmd,
			   u8 lun)
{
	struct ufs_hba *hba = ufsf->hba;
	struct ufshpb_lu *hpb;
	struct ufshpb_req *pre_req;
	struct ufshcd_lrb *add_lrbp;
	struct ufshcd_lrb *orig_lrbp = &hba->lrb[cmd->request->tag];
	struct scsi_cmnd *pre_cmd;
	unsigned long flags;
	int add_tag, hpb_ctx_id;
	int ret = 0;

	/* WKLU could not be HPB-LU */
	if (!ufsf_is_valid_lun(lun))
		return -ENODEV;

	hpb = ufsf->ufshpb_lup[lun];
	ret = ufshpb_lu_get(hpb);
	if (ret)
		return ret;

	if (hpb->force_disable) {
		ret = -ENODEV;
		goto put_hpb;
	}

	ret = ufshpb_check_pre_req_cond(hpb, cmd);
	if (ret)
		goto put_hpb;

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	pre_req = ufshpb_get_pre_req(hpb);
	if (!pre_req) {
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);
		ret = -ENOMEM;
		goto put_hpb;
	}

	hpb_ctx_id = ufshpb_issue_ctx_id_ticket(hpb);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);

	ret = ufshpb_set_pre_req(hpb, cmd, pre_req, hpb_ctx_id);
	if (ret)
		goto put_pre_req;

	add_tag = pre_req->req->tag;
	if (test_and_set_bit_lock(add_tag, &hba->lrb_in_use)) {
		ufshpb_end_pre_req(ufsf, pre_req->req);
		return -EIO;
	}

	add_lrbp = &hba->lrb[add_tag];
	WARN_ON(add_lrbp->cmd);

	pre_cmd = pre_req->req->special;
	add_lrbp->cmd = pre_cmd;
	add_lrbp->sense_bufflen = UFSHCD_REQ_SENSE_SIZE;
	add_lrbp->sense_buffer = pre_cmd->sense_buffer;
	add_lrbp->task_tag = add_tag;
	add_lrbp->lun = lun;
	add_lrbp->intr_cmd = !ufshcd_is_intr_aggr_allowed(hba) ? true : false;
	add_lrbp->req_abort_skip = false;

	/* MTK patch: reset crypto_enable */
	add_lrbp->crypto_enable = false;

	orig_lrbp->hpb_ctx_id = hpb_ctx_id;

	return add_tag;
put_pre_req:
	spin_lock_irqsave(&hpb->hpb_lock, flags);
	ufshpb_put_pre_req(hpb, pre_req);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
put_hpb:
	ufshpb_lu_put(hpb);
	return ret;
}

int ufshpb_prepare_add_lrbp(struct ufsf_feature *ufsf, int add_tag)
{
	struct ufs_hba *hba = ufsf->hba;
	struct ufshcd_lrb *add_lrbp;
	struct scsi_cmnd *pre_cmd;
	int err = 0;

	add_lrbp = &hba->lrb[add_tag];

	pre_cmd = add_lrbp->cmd;

	err = ufshcd_hold(hba, true);
	if (err)
		goto hold_err;

	ufshcd_comp_scsi_upiu(hba, add_lrbp);

	err = ufshcd_map_sg(hba, add_lrbp);
	if (err)
		goto map_err;

	return 0;
map_err:
	ufshcd_release(hba);
hold_err:
	add_lrbp->cmd = NULL;
	clear_bit_unlock(add_tag, &hba->lrb_in_use);
	ufsf_hpb_end_pre_req(&hba->ufsf, pre_cmd->request);
	return -EIO;
}

/* routine : READ10 -> HPB_READ  */
void ufshpb_prep_fn(struct ufsf_feature *ufsf, struct ufshcd_lrb *lrbp)
{
	struct ufshpb_lu *hpb;
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	struct request *rq;
	unsigned long long ppn = 0;
	unsigned long lpn, flags;
	int transfer_len = TRANSFER_LEN;
	int rgn_idx, srgn_idx, srgn_offset, ret, error = 0;

	/* WKLU could not be HPB-LU */
	if (!lrbp || !ufsf_is_valid_lun(lrbp->lun))
		return;

	if (!ufshpb_is_write_discard_lrbp(lrbp) &&
	    !ufshpb_is_read_cmd(lrbp->cmd))
		return;

	rq = lrbp->cmd->request;
	hpb = ufsf->ufshpb_lup[lrbp->lun];
	ret = ufshpb_lu_get(hpb);
	if (ret)
		return;

	if (hpb->force_disable) {
		if (ufshpb_is_read_cmd(lrbp->cmd))
			TMSG(ufsf, hpb->lun, "%llu + %u READ_10",
			     (unsigned long long) blk_rq_pos(rq),
			     (unsigned int) blk_rq_sectors(rq));
		goto put_hpb;
	}

	/*
	 * When the request is from ioctrl, the address in rq will be -1.
	 * At this case, the ufshpb_ppn_dirty_check() will check the address
	 * with 0xFFFFFFFF_FFFFFFFF (64bit) or 0xFFFFFFFF(32bit). This will
	 * cause unexpected behavier. So skip here.
	 */
	if ((long)blk_rq_pos(rq) == -1)
		goto put_hpb;

	lpn = ufshpb_get_lpn(rq);
=======
static void
ufshpb_set_hpb_read_to_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp,
			    __be64 ppn, u8 transfer_len)
{
	unsigned char *cdb = lrbp->cmd->cmnd;
	__be64 ppn_tmp = ppn;
	cdb[0] = UFSHPB_READ;

	if (hba->dev_quirks & UFS_DEVICE_QUIRK_SWAP_L2P_ENTRY_FOR_HPB_READ)
		ppn_tmp = swab64(ppn);

	/* ppn value is stored as big-endian in the host memory */
	memcpy(&cdb[6], &ppn_tmp, sizeof(__be64));
	cdb[14] = transfer_len;
	cdb[15] = 0;

	lrbp->cmd->cmd_len = UFS_CDB_SIZE;
}

/*
 * This function will set up HPB read command using host-side L2P map data.
 */
int ufshpb_prep(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufshpb_lu *hpb;
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	struct scsi_cmnd *cmd = lrbp->cmd;
	u32 lpn;
	__be64 ppn;
	unsigned long flags;
	int transfer_len, rgn_idx, srgn_idx, srgn_offset;
	int err = 0;

	hpb = ufshpb_get_hpb_data(cmd->device);
	if (!hpb)
		return -ENODEV;

	if (ufshpb_get_state(hpb) == HPB_INIT)
		return -ENODEV;

	if (ufshpb_get_state(hpb) != HPB_PRESENT) {
		dev_notice(&hpb->sdev_ufs_lu->sdev_dev,
			   "%s: ufshpb state is not PRESENT", __func__);
		return -ENODEV;
	}

	if (blk_rq_is_passthrough(scsi_cmd_to_rq(cmd)) ||
	    (!ufshpb_is_write_or_discard(cmd) &&
	     !ufshpb_is_read_cmd(cmd)))
		return 0;

	transfer_len = sectors_to_logical(cmd->device,
					  blk_rq_sectors(scsi_cmd_to_rq(cmd)));
	if (unlikely(!transfer_len))
		return 0;

	lpn = sectors_to_logical(cmd->device, blk_rq_pos(scsi_cmd_to_rq(cmd)));
>>>>>>> upstream/android-13
	ufshpb_get_pos_from_lpn(hpb, lpn, &rgn_idx, &srgn_idx, &srgn_offset);
	rgn = hpb->rgn_tbl + rgn_idx;
	srgn = rgn->srgn_tbl + srgn_idx;

<<<<<<< HEAD
	/*
	 * If cmd type is WRITE, bitmap set to dirty.
	 */
	if (ufshpb_is_write_discard_lrbp(lrbp)) {
		spin_lock_irqsave(&hpb->hpb_lock, flags);
		if (rgn->rgn_state == HPBREGION_INACTIVE) {
			spin_unlock_irqrestore(&hpb->hpb_lock, flags);
			goto put_hpb;
		}
		ufshpb_set_dirty(hpb, lrbp, rgn_idx, srgn_idx, srgn_offset);
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);
		goto put_hpb;
	}

	if (!ufshpb_is_read_cmd(lrbp->cmd))
		goto put_hpb;

	if (ufshpb_is_unaligned(rq)) {
		TMSG_CMD(hpb, "READ_10 not aligned 4KB", rq, rgn_idx, srgn_idx);
		goto put_hpb;
	}

	transfer_len = ufshpb_get_len(rq);
	if (!transfer_len)
		goto put_hpb;

	if (!ufshpb_is_support_chunk(transfer_len)) {
		TMSG_CMD(hpb, "READ_10 doesn't support chunk size",
			 rq, rgn_idx, srgn_idx);
		goto put_hpb;
	}

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	if (ufshpb_ppn_dirty_check(hpb, lpn, transfer_len)) {
		atomic64_inc(&hpb->miss);
		TMSG_CMD(hpb, "READ_10 E_D", rq, rgn_idx, srgn_idx);
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);
		goto put_hpb;
	}

	ppn = ufshpb_get_ppn(srgn->mctx, srgn_offset, &error);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	if (error) {
		ERR_MSG("get_ppn failed.. err %d region %d subregion %d",
			error, rgn_idx, srgn_idx);
		ufshpb_lu_put(hpb);
		goto wakeup_ee_worker;
	}

	ufshpb_set_read16_cmd(hpb, lrbp, ppn, transfer_len);
	TMSG(ufsf, hpb->lun, "%llu + %u HPB_READ %d - %d context_id %d",
	     (unsigned long long) blk_rq_pos(lrbp->cmd->request),
	     (unsigned int) blk_rq_sectors(lrbp->cmd->request), rgn_idx,
	     srgn_idx, lrbp->hpb_ctx_id);

	atomic64_inc(&hpb->hit);
put_hpb:
	ufshpb_lu_put(hpb);
	return;
wakeup_ee_worker:
	ufshpb_failed(hpb, __func__);
}

static inline void ufshpb_put_map_req(struct ufshpb_lu *hpb,
				      struct ufshpb_req *map_req)
{
	list_add_tail(&map_req->list_req, &hpb->lh_map_req_free);
	hpb->num_inflight_map_req--;
}

static struct ufshpb_req *ufshpb_get_map_req(struct ufshpb_lu *hpb)
{
	struct ufshpb_req *map_req;

	if (hpb->num_inflight_map_req >= hpb->throttle_map_req) {
		HPB_DEBUG(hpb, "map_req throttle. inflight %d throttle %d",
			  hpb->num_inflight_map_req, hpb->throttle_map_req);
		return NULL;
	}

	map_req = list_first_entry_or_null(&hpb->lh_map_req_free,
					   struct ufshpb_req, list_req);
	if (!map_req) {
		HPB_DEBUG(hpb, "There is no map_req");
		return NULL;
	}

	list_del_init(&map_req->list_req);
	hpb->num_inflight_map_req++;
=======
	/* If command type is WRITE or DISCARD, set bitmap as drity */
	if (ufshpb_is_write_or_discard(cmd)) {
		ufshpb_iterate_rgn(hpb, rgn_idx, srgn_idx, srgn_offset,
				   transfer_len, true);
		return 0;
	}

	if (!ufshpb_is_supported_chunk(hpb, transfer_len))
		return 0;

	if (hpb->is_hcm) {
		/*
		 * in host control mode, reads are the main source for
		 * activation trials.
		 */
		ufshpb_iterate_rgn(hpb, rgn_idx, srgn_idx, srgn_offset,
				   transfer_len, false);

		/* keep those counters normalized */
		if (rgn->reads > hpb->entries_per_srgn)
			schedule_work(&hpb->ufshpb_normalization_work);
	}

	spin_lock_irqsave(&hpb->rgn_state_lock, flags);
	if (ufshpb_test_ppn_dirty(hpb, rgn_idx, srgn_idx, srgn_offset,
				   transfer_len)) {
		hpb->stats.miss_cnt++;
		spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
		return 0;
	}

	err = ufshpb_fill_ppn_from_page(hpb, srgn->mctx, srgn_offset, 1, &ppn);
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
	if (unlikely(err < 0)) {
		/*
		 * In this case, the region state is active,
		 * but the ppn table is not allocated.
		 * Make sure that ppn table must be allocated on
		 * active state.
		 */
		dev_err(hba->dev, "get ppn failed. err %d\n", err);
		return err;
	}

	ufshpb_set_hpb_read_to_upiu(hba, lrbp, ppn, transfer_len);

	hpb->stats.hit_cnt++;
	return 0;
}

static struct ufshpb_req *ufshpb_get_req(struct ufshpb_lu *hpb,
					 int rgn_idx, enum req_opf dir,
					 bool atomic)
{
	struct ufshpb_req *rq;
	struct request *req;
	int retries = HPB_MAP_REQ_RETRIES;

	rq = kmem_cache_alloc(hpb->map_req_cache, GFP_KERNEL);
	if (!rq)
		return NULL;

retry:
	req = blk_get_request(hpb->sdev_ufs_lu->request_queue, dir,
			      BLK_MQ_REQ_NOWAIT);

	if (!atomic && (PTR_ERR(req) == -EWOULDBLOCK) && (--retries > 0)) {
		usleep_range(3000, 3100);
		goto retry;
	}

	if (IS_ERR(req))
		goto free_rq;

	rq->hpb = hpb;
	rq->req = req;
	rq->rb.rgn_idx = rgn_idx;

	return rq;

free_rq:
	kmem_cache_free(hpb->map_req_cache, rq);
	return NULL;
}

static void ufshpb_put_req(struct ufshpb_lu *hpb, struct ufshpb_req *rq)
{
	blk_put_request(rq->req);
	kmem_cache_free(hpb->map_req_cache, rq);
}

static struct ufshpb_req *ufshpb_get_map_req(struct ufshpb_lu *hpb,
					     struct ufshpb_subregion *srgn)
{
	struct ufshpb_req *map_req;
	struct bio *bio;
	unsigned long flags;

	if (hpb->is_hcm &&
	    hpb->num_inflight_map_req >= hpb->params.inflight_map_req) {
		dev_info(&hpb->sdev_ufs_lu->sdev_dev,
			 "map_req throttle. inflight %d throttle %d",
			 hpb->num_inflight_map_req,
			 hpb->params.inflight_map_req);
		return NULL;
	}

	map_req = ufshpb_get_req(hpb, srgn->rgn_idx, REQ_OP_DRV_IN, false);
	if (!map_req)
		return NULL;

	bio = bio_alloc(GFP_KERNEL, hpb->pages_per_srgn);
	if (!bio) {
		ufshpb_put_req(hpb, map_req);
		return NULL;
	}

	map_req->bio = bio;

	map_req->rb.srgn_idx = srgn->srgn_idx;
	map_req->rb.mctx = srgn->mctx;

	spin_lock_irqsave(&hpb->param_lock, flags);
	hpb->num_inflight_map_req++;
	spin_unlock_irqrestore(&hpb->param_lock, flags);
>>>>>>> upstream/android-13

	return map_req;
}

<<<<<<< HEAD
static int ufshpb_clean_dirty_bitmap(struct ufshpb_lu *hpb,
				     struct ufshpb_subregion *srgn)
{
	struct ufshpb_region *rgn;

	BUG_ON(!srgn->mctx);

	rgn = hpb->rgn_tbl + srgn->rgn_idx;

	if (rgn->rgn_state == HPBREGION_INACTIVE) {
		HPB_DEBUG(hpb, "%d - %d evicted", srgn->rgn_idx,
			  srgn->srgn_idx);
		return -EINVAL;
	}

	memset(srgn->mctx->ppn_dirty, 0x00,
	       hpb->entries_per_srgn >> bits_per_byte_shift);
=======
static void ufshpb_put_map_req(struct ufshpb_lu *hpb,
			       struct ufshpb_req *map_req)
{
	unsigned long flags;

	bio_put(map_req->bio);
	ufshpb_put_req(hpb, map_req);

	spin_lock_irqsave(&hpb->param_lock, flags);
	hpb->num_inflight_map_req--;
	spin_unlock_irqrestore(&hpb->param_lock, flags);
}

static int ufshpb_clear_dirty_bitmap(struct ufshpb_lu *hpb,
				     struct ufshpb_subregion *srgn)
{
	struct ufshpb_region *rgn;
	u32 num_entries = hpb->entries_per_srgn;

	if (!srgn->mctx) {
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			"no mctx in region %d subregion %d.\n",
			srgn->rgn_idx, srgn->srgn_idx);
		return -1;
	}

	if (unlikely(srgn->is_last))
		num_entries = hpb->last_srgn_entries;

	bitmap_zero(srgn->mctx->ppn_dirty, num_entries);

	rgn = hpb->rgn_tbl + srgn->rgn_idx;
	clear_bit(RGN_FLAG_DIRTY, &rgn->rgn_flags);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void ufshpb_clean_active_subregion(struct ufshpb_lu *hpb,
					  struct ufshpb_subregion *srgn)
{
	struct ufshpb_region *rgn;

	BUG_ON(!srgn->mctx);

	rgn = hpb->rgn_tbl + srgn->rgn_idx;

	if (rgn->rgn_state == HPBREGION_INACTIVE) {
		HPB_DEBUG(hpb, "%d - %d evicted", srgn->rgn_idx,
			  srgn->srgn_idx);
		return;
	}
	srgn->srgn_state = HPBSUBREGION_CLEAN;
}

static void ufshpb_error_active_subregion(struct ufshpb_lu *hpb,
					  struct ufshpb_subregion *srgn)
{
	struct ufshpb_region *rgn;

	BUG_ON(!srgn->mctx);

	rgn = hpb->rgn_tbl + srgn->rgn_idx;

	if (rgn->rgn_state == HPBREGION_INACTIVE) {
		ERR_MSG("%d - %d evicted", srgn->rgn_idx, srgn->srgn_idx);
		return;
	}
	srgn->srgn_state = HPBSUBREGION_DIRTY;
}

static void ufshpb_check_ppn(struct ufshpb_lu *hpb, int rgn_idx, int srgn_idx,
			     struct ufshpb_map_ctx *mctx, const char *str)
{
	int error = 0;
	unsigned long long val[2];

	BUG_ON(!mctx);

	val[0] = ufshpb_get_ppn(mctx, 0, &error);
	if (!error)
		val[1] = ufshpb_get_ppn(mctx, hpb->entries_per_srgn - 1,
					&error);
	if (error)
		val[0] = val[1] = 0;

	HPB_DEBUG(hpb, "%s READ BUFFER %d - %d ( %llx ~ %llx )", str, rgn_idx,
		  srgn_idx, val[0], val[1]);
}

static void ufshpb_map_compl_process(struct ufshpb_req *map_req)
{
	struct ufshpb_lu *hpb = map_req->hpb;
	struct ufshpb_subregion *srgn;
	unsigned long flags;

	srgn = hpb->rgn_tbl[map_req->rb.rgn_idx].srgn_tbl +
		map_req->rb.srgn_idx;

	if (hpb->debug)
		ufshpb_check_ppn(hpb, srgn->rgn_idx, srgn->srgn_idx, srgn->mctx,
				 "COMPL");

	TMSG(hpb->ufsf, hpb->lun, "Noti: C RB %d - %d", map_req->rb.rgn_idx,
	     map_req->rb.srgn_idx);

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	ufshpb_clean_active_subregion(hpb, srgn);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
}

=======
>>>>>>> upstream/android-13
static void ufshpb_update_active_info(struct ufshpb_lu *hpb, int rgn_idx,
				      int srgn_idx)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;

	rgn = hpb->rgn_tbl + rgn_idx;
	srgn = rgn->srgn_tbl + srgn_idx;

	list_del_init(&rgn->list_inact_rgn);

	if (list_empty(&srgn->list_act_srgn))
		list_add_tail(&srgn->list_act_srgn, &hpb->lh_act_srgn);
<<<<<<< HEAD
=======

	hpb->stats.rb_active_cnt++;
>>>>>>> upstream/android-13
}

static void ufshpb_update_inactive_info(struct ufshpb_lu *hpb, int rgn_idx)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	rgn = hpb->rgn_tbl + rgn_idx;

<<<<<<< HEAD
	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		srgn = rgn->srgn_tbl + srgn_idx;

		list_del_init(&srgn->list_act_srgn);
	}

	if (list_empty(&rgn->list_inact_rgn))
		list_add_tail(&rgn->list_inact_rgn, &hpb->lh_inact_rgn);
}

static int ufshpb_map_req_error(struct ufshpb_req *map_req)
{
	struct ufshpb_lu *hpb = map_req->hpb;
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	struct scsi_sense_hdr sshdr;
	unsigned long flags;

	rgn = hpb->rgn_tbl + map_req->rb.rgn_idx;
	srgn = rgn->srgn_tbl + map_req->rb.srgn_idx;

	scsi_normalize_sense(map_req->sense, SCSI_SENSE_BUFFERSIZE, &sshdr);

	ERR_MSG("code %x sense_key %x asc %x ascq %x", sshdr.response_code,
		sshdr.sense_key, sshdr.asc, sshdr.ascq);
	ERR_MSG("byte4 %x byte5 %x byte6 %x additional_len %x", sshdr.byte4,
		sshdr.byte5, sshdr.byte6, sshdr.additional_length);

	if (sshdr.sense_key != ILLEGAL_REQUEST)
		return 0;

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	if (rgn->rgn_state == HPBREGION_PINNED) {
		if (sshdr.asc == 0x06 && sshdr.ascq == 0x01) {
			HPB_DEBUG(hpb, "retry pinned rb %d - %d",
				  map_req->rb.rgn_idx, map_req->rb.srgn_idx);

			list_add_tail(&map_req->list_req,
				      &hpb->lh_map_req_retry);
			spin_unlock_irqrestore(&hpb->hpb_lock, flags);

			schedule_delayed_work(&hpb->ufshpb_retry_work,
					      msecs_to_jiffies(RETRY_DELAY_MS));
			return -EAGAIN;
		}
		HPB_DEBUG(hpb, "pinned rb %d - %d(dirty)",
			  map_req->rb.rgn_idx, map_req->rb.srgn_idx);

		ufshpb_error_active_subregion(hpb, srgn);
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	} else {
		ufshpb_error_active_subregion(hpb, srgn);

		spin_unlock_irqrestore(&hpb->hpb_lock, flags);

		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
		ufshpb_update_inactive_info(hpb, map_req->rb.rgn_idx);
		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

		HPB_DEBUG(hpb, "Non-pinned rb %d will be inactive",
			  map_req->rb.rgn_idx);

		schedule_work(&hpb->ufshpb_task_workq);
	}

	return 0;
}

static inline void ufshpb_mimic_blk_pm_put_request(struct request *rq)
{
	if (rq->q->dev && !(rq->rq_flags & RQF_PM) && !--rq->q->nr_pending)
		pm_runtime_mark_last_busy(rq->q->dev);
}

/* routine : map_req compl */
=======
	for_each_sub_region(rgn, srgn_idx, srgn)
		list_del_init(&srgn->list_act_srgn);

	if (list_empty(&rgn->list_inact_rgn))
		list_add_tail(&rgn->list_inact_rgn, &hpb->lh_inact_rgn);

	hpb->stats.rb_inactive_cnt++;
}

static void ufshpb_activate_subregion(struct ufshpb_lu *hpb,
				      struct ufshpb_subregion *srgn)
{
	struct ufshpb_region *rgn;

	/*
	 * If there is no mctx in subregion
	 * after I/O progress for HPB_READ_BUFFER, the region to which the
	 * subregion belongs was evicted.
	 * Make sure the region must not evict in I/O progress
	 */
	if (!srgn->mctx) {
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			"no mctx in region %d subregion %d.\n",
			srgn->rgn_idx, srgn->srgn_idx);
		srgn->srgn_state = HPB_SRGN_INVALID;
		return;
	}

	rgn = hpb->rgn_tbl + srgn->rgn_idx;

	if (unlikely(rgn->rgn_state == HPB_RGN_INACTIVE)) {
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			"region %d subregion %d evicted\n",
			srgn->rgn_idx, srgn->srgn_idx);
		srgn->srgn_state = HPB_SRGN_INVALID;
		return;
	}
	srgn->srgn_state = HPB_SRGN_VALID;
}

static void ufshpb_umap_req_compl_fn(struct request *req, blk_status_t error)
{
	struct ufshpb_req *umap_req = (struct ufshpb_req *)req->end_io_data;

	ufshpb_put_req(umap_req->hpb, umap_req);
}

>>>>>>> upstream/android-13
static void ufshpb_map_req_compl_fn(struct request *req, blk_status_t error)
{
	struct ufshpb_req *map_req = (struct ufshpb_req *) req->end_io_data;
	struct ufshpb_lu *hpb = map_req->hpb;
<<<<<<< HEAD
	unsigned long flags;
	int ret;

#ifdef CONFIG_PM
	ufshpb_mimic_blk_pm_put_request(req);
#endif
	if (hpb->ufsf->ufshpb_state != HPB_PRESENT)
		goto free_map_req;

	if (error) {
		ERR_MSG("COMP_NOTI: RB number %d ( %d - %d )", error,
			map_req->rb.rgn_idx, map_req->rb.srgn_idx);

		ret = ufshpb_map_req_error(map_req);
		if (ret)
			goto retry_map_req;
	} else
		ufshpb_map_compl_process(map_req);

free_map_req:
	spin_lock_irqsave(&hpb->hpb_lock, flags);
	ufshpb_put_map_req(map_req->hpb, map_req);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
retry_map_req:
	scsi_device_put(hpb->ufsf->sdev_ufs_lu[hpb->lun]);
	ufshpb_lu_put(hpb);
}

static inline int ufshpb_get_scsi_device(struct ufs_hba *hba,
					 struct scsi_device *sdev)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(hba->host->host_lock, flags);
	ret = scsi_device_get(sdev);
	if (!ret && !scsi_device_online(sdev)) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		scsi_device_put(sdev);
		WARNING_MSG("scsi_device_get failed.. ret %d", ret);
		return -ENODEV;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	return 0;
}

static int ufshpb_execute_dev_ctx_req(struct ufshpb_lu *hpb, unsigned char *cdb,
				      void *buf, int len)
{
	struct scsi_sense_hdr sshdr;
	struct scsi_device *sdev;
	struct ufsf_feature *ufsf = hpb->ufsf;
	int ret = 0;

	sdev = ufsf->sdev_ufs_lu[hpb->lun];
	if (!sdev) {
		WARNING_MSG("cannot find scsi_device");
		return -ENODEV;
	}

	ret = ufshpb_get_scsi_device(ufsf->hba, sdev);
	if (ret)
		return ret;

	ufsf->issue_ioctl = true;

	ret = scsi_execute(sdev, cdb, DMA_FROM_DEVICE, buf, len, NULL, &sshdr,
			   msecs_to_jiffies(30000), 3, 0, 0, NULL);

	ufsf->issue_ioctl = false;

	scsi_device_put(sdev);

	return ret;
}

static inline void ufshpb_set_read_dev_ctx(unsigned char *cdb, int lba, int len)
{
	cdb[0] = READ_10;
	cdb[1] = 0x02;
	cdb[2] = GET_BYTE_3(lba);
	cdb[3] = GET_BYTE_2(lba);
	cdb[4] = GET_BYTE_1(lba);
	cdb[5] = GET_BYTE_0(lba);
	cdb[6] = GET_BYTE_2(len);
	cdb[7] = GET_BYTE_1(len);
	cdb[8] = GET_BYTE_0(len);
}

int ufshpb_issue_req_dev_ctx(struct ufshpb_lu *hpb, unsigned char *buf,
			     int buf_len)
{
	unsigned char cdb[10] = { 0 };
	int cmd_len = buf_len >> OS_PAGE_SHIFT;
	int ret = 0;

	ufshpb_set_read_dev_ctx(cdb, READ10_DEBUG_LBA, cmd_len);

	ret = ufshpb_execute_dev_ctx_req(hpb, cdb, buf, buf_len);

	if (ret < 0)
		ERR_MSG("failed with err %d", ret);

	return ret;
}

static inline void ufshpb_set_read_buf_cmd(unsigned char *cdb, int rgn_idx,
					   int srgn_idx, int srgn_mem_size)
{
	cdb[0] = UFSHPB_READ_BUFFER;
	cdb[1] = UFSHPB_READ_BUFFER_ID;
	cdb[2] = GET_BYTE_1(rgn_idx);
	cdb[3] = GET_BYTE_0(rgn_idx);
	cdb[4] = GET_BYTE_1(srgn_idx);
	cdb[5] = GET_BYTE_0(srgn_idx);
	cdb[6] = GET_BYTE_2(srgn_mem_size);
	cdb[7] = GET_BYTE_1(srgn_mem_size);
	cdb[8] = GET_BYTE_0(srgn_mem_size);
	cdb[9] = 0x00;
}

static int ufshpb_map_req_add_bio_page(struct ufshpb_lu *hpb,
				       struct request_queue *q, struct bio *bio,
				       struct ufshpb_map_ctx *mctx)
{
	struct page *page = NULL;
	int i, ret = 0;

	bio_reset(bio);

	for (i = 0; i < hpb->mpages_per_srgn; i++) {
		/* virt_to_page(p + (OS_PAGE_SIZE * i)); */
		page = mctx->m_page[i];
		if (!page)
			return -ENOMEM;

		ret = bio_add_pc_page(q, bio, page, hpb->mpage_bytes, 0);

		if (ret != hpb->mpage_bytes) {
			ERR_MSG("bio_add_pc_page fail: %d", ret);
			return -ENOMEM;
		}
	}

	return 0;
}

static int ufshpb_execute_map_req(struct ufshpb_lu *hpb,
				  struct scsi_device *sdev,
				  struct ufshpb_req *map_req)
{
	struct request_queue *q = sdev->request_queue;
	struct request *req;
	struct scsi_request *rq;
	struct scsi_cmnd *scmd;
	struct bio *bio = map_req->bio;
	int ret;

	ret = ufshpb_map_req_add_bio_page(hpb, q, bio, map_req->rb.mctx);
	if (ret)
		return ret;

	req = map_req->req;

	/*
	 * blk_init_rl() -> alloc_request_size().
	 * q->init_rq_fn = scsi_old_init_rq behavior.
	 */
	scmd = (struct scsi_cmnd *)(req + 1);
	memset(scmd, 0, sizeof(*scmd));
	scmd->sense_buffer = map_req->sense;
	scmd->req.sense = scmd->sense_buffer;

	/* blk_get_request behavior */
	blk_rq_init(q, req);
	q->initialize_rq_fn(req);

	/* 1. request setup */
	blk_rq_append_bio(req, &bio); /* req->__data_len is setted */
	req->cmd_flags = REQ_OP_READ | REQ_OP_SCSI_IN;
	req->rq_flags = RQF_QUIET | RQF_PREEMPT;
	req->timeout = msecs_to_jiffies(30000);
	req->end_io_data = (void *)map_req;

	/* 2. scsi_request setup */
	rq = scsi_req(req);
	ufshpb_set_read_buf_cmd(rq->cmd, map_req->rb.rgn_idx,
				map_req->rb.srgn_idx, hpb->srgn_mem_size);
	rq->cmd_len = scsi_command_size(rq->cmd);

	if (hpb->debug)
		ufshpb_check_ppn(hpb, map_req->rb.rgn_idx, map_req->rb.srgn_idx,
				 map_req->rb.mctx, "ISSUE");

	TMSG(hpb->ufsf, hpb->lun, "Noti: I RB %d - %d", map_req->rb.rgn_idx,
	     map_req->rb.srgn_idx);

	blk_execute_rq_nowait(q, NULL, req, 1, ufshpb_map_req_compl_fn);

	atomic64_inc(&hpb->map_req_cnt);
=======
	struct ufshpb_subregion *srgn;
	unsigned long flags;

	srgn = hpb->rgn_tbl[map_req->rb.rgn_idx].srgn_tbl +
		map_req->rb.srgn_idx;

	ufshpb_clear_dirty_bitmap(hpb, srgn);
	spin_lock_irqsave(&hpb->rgn_state_lock, flags);
	ufshpb_activate_subregion(hpb, srgn);
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);

	ufshpb_put_map_req(map_req->hpb, map_req);
}

static void ufshpb_set_unmap_cmd(unsigned char *cdb, struct ufshpb_region *rgn)
{
	cdb[0] = UFSHPB_WRITE_BUFFER;
	cdb[1] = rgn ? UFSHPB_WRITE_BUFFER_INACT_SINGLE_ID :
			  UFSHPB_WRITE_BUFFER_INACT_ALL_ID;
	if (rgn)
		put_unaligned_be16(rgn->rgn_idx, &cdb[2]);
	cdb[9] = 0x00;
}

static void ufshpb_set_read_buf_cmd(unsigned char *cdb, int rgn_idx,
				    int srgn_idx, int srgn_mem_size)
{
	cdb[0] = UFSHPB_READ_BUFFER;
	cdb[1] = UFSHPB_READ_BUFFER_ID;

	put_unaligned_be16(rgn_idx, &cdb[2]);
	put_unaligned_be16(srgn_idx, &cdb[4]);
	put_unaligned_be24(srgn_mem_size, &cdb[6]);

	cdb[9] = 0x00;
}

static void ufshpb_execute_umap_req(struct ufshpb_lu *hpb,
				   struct ufshpb_req *umap_req,
				   struct ufshpb_region *rgn)
{
	struct request *req;
	struct scsi_request *rq;

	req = umap_req->req;
	req->timeout = 0;
	req->end_io_data = (void *)umap_req;
	rq = scsi_req(req);
	ufshpb_set_unmap_cmd(rq->cmd, rgn);
	rq->cmd_len = HPB_WRITE_BUFFER_CMD_LENGTH;

	blk_execute_rq_nowait(NULL, req, 1, ufshpb_umap_req_compl_fn);

	hpb->stats.umap_req_cnt++;
}

static int ufshpb_execute_map_req(struct ufshpb_lu *hpb,
				  struct ufshpb_req *map_req, bool last)
{
	struct request_queue *q;
	struct request *req;
	struct scsi_request *rq;
	int mem_size = hpb->srgn_mem_size;
	int ret = 0;
	int i;

	q = hpb->sdev_ufs_lu->request_queue;
	for (i = 0; i < hpb->pages_per_srgn; i++) {
		ret = bio_add_pc_page(q, map_req->bio, map_req->rb.mctx->m_page[i],
				      PAGE_SIZE, 0);
		if (ret != PAGE_SIZE) {
			dev_err(&hpb->sdev_ufs_lu->sdev_dev,
				   "bio_add_pc_page fail %d - %d\n",
				   map_req->rb.rgn_idx, map_req->rb.srgn_idx);
			return ret;
		}
	}

	req = map_req->req;

	blk_rq_append_bio(req, map_req->bio);

	req->end_io_data = map_req;

	rq = scsi_req(req);

	if (unlikely(last))
		mem_size = hpb->last_srgn_entries * HPB_ENTRY_SIZE;

	ufshpb_set_read_buf_cmd(rq->cmd, map_req->rb.rgn_idx,
				map_req->rb.srgn_idx, mem_size);
	rq->cmd_len = HPB_READ_BUFFER_CMD_LENGTH;

	blk_execute_rq_nowait(NULL, req, 1, ufshpb_map_req_compl_fn);

	hpb->stats.map_req_cnt++;
	return 0;
}

static struct ufshpb_map_ctx *ufshpb_get_map_ctx(struct ufshpb_lu *hpb,
						 bool last)
{
	struct ufshpb_map_ctx *mctx;
	u32 num_entries = hpb->entries_per_srgn;
	int i, j;

	mctx = mempool_alloc(ufshpb_mctx_pool, GFP_KERNEL);
	if (!mctx)
		return NULL;

	mctx->m_page = kmem_cache_alloc(hpb->m_page_cache, GFP_KERNEL);
	if (!mctx->m_page)
		goto release_mctx;

	if (unlikely(last))
		num_entries = hpb->last_srgn_entries;

	mctx->ppn_dirty = bitmap_zalloc(num_entries, GFP_KERNEL);
	if (!mctx->ppn_dirty)
		goto release_m_page;

	for (i = 0; i < hpb->pages_per_srgn; i++) {
		mctx->m_page[i] = mempool_alloc(ufshpb_page_pool, GFP_KERNEL);
		if (!mctx->m_page[i]) {
			for (j = 0; j < i; j++)
				mempool_free(mctx->m_page[j], ufshpb_page_pool);
			goto release_ppn_dirty;
		}
		clear_page(page_address(mctx->m_page[i]));
	}

	return mctx;

release_ppn_dirty:
	bitmap_free(mctx->ppn_dirty);
release_m_page:
	kmem_cache_free(hpb->m_page_cache, mctx->m_page);
release_mctx:
	mempool_free(mctx, ufshpb_mctx_pool);
	return NULL;
}

static void ufshpb_put_map_ctx(struct ufshpb_lu *hpb,
			       struct ufshpb_map_ctx *mctx)
{
	int i;

	for (i = 0; i < hpb->pages_per_srgn; i++)
		mempool_free(mctx->m_page[i], ufshpb_page_pool);

	bitmap_free(mctx->ppn_dirty);
	kmem_cache_free(hpb->m_page_cache, mctx->m_page);
	mempool_free(mctx, ufshpb_mctx_pool);
}

static int ufshpb_check_srgns_issue_state(struct ufshpb_lu *hpb,
					  struct ufshpb_region *rgn)
{
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	for_each_sub_region(rgn, srgn_idx, srgn)
		if (srgn->srgn_state == HPB_SRGN_ISSUED)
			return -EPERM;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int ufshpb_issue_map_req(struct ufshpb_lu *hpb,
				struct ufshpb_req *map_req)
{
	struct scsi_device *sdev;
	struct ufsf_feature *ufsf = hpb->ufsf;
	int ret = 0;

	sdev = ufsf->sdev_ufs_lu[hpb->lun];
	if (!sdev) {
		WARNING_MSG("cannot find scsi_device");
		return -ENODEV;
	}

	ret = ufshpb_get_scsi_device(ufsf->hba, sdev);
	if (ret)
		return ret;

	ret = ufshpb_execute_map_req(hpb, sdev, map_req);
	if (ret)
		scsi_device_put(sdev);

	return ret;
}

static inline void ufshpb_set_map_req(struct ufshpb_lu *hpb, int rgn_idx,
				      int srgn_idx, struct ufshpb_map_ctx *mctx,
				      struct ufshpb_req *map_req)
{
	map_req->hpb = hpb;
	map_req->rb.rgn_idx = rgn_idx;
	map_req->rb.srgn_idx = srgn_idx;
	map_req->rb.mctx = mctx;
	map_req->rb.lun = hpb->lun;
}

static struct ufshpb_map_ctx *ufshpb_get_map_ctx(struct ufshpb_lu *hpb,
						 int *err)
{
	struct ufshpb_map_ctx *mctx;

	mctx = list_first_entry_or_null(&hpb->lh_map_ctx_free,
					struct ufshpb_map_ctx, list_table);
	if (mctx) {
		list_del_init(&mctx->list_table);
		hpb->debug_free_table--;
		return mctx;
	}
	*err = -ENOMEM;
	return NULL;
}

static inline void ufshpb_add_lru_info(struct victim_select_info *lru_info,
				       struct ufshpb_region *rgn)
{
	rgn->rgn_state = HPBREGION_ACTIVE;
	list_add_tail(&rgn->list_lru_rgn, &lru_info->lh_lru_rgn);
	atomic64_inc(&lru_info->active_cnt);
}

static inline int ufshpb_add_region(struct ufshpb_lu *hpb,
				    struct ufshpb_region *rgn)
{
	struct victim_select_info *lru_info;
	int srgn_idx;
	int err = 0;

	lru_info = &hpb->lru_info;

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		struct ufshpb_subregion *srgn;

		srgn = rgn->srgn_tbl + srgn_idx;

		srgn->mctx = ufshpb_get_map_ctx(hpb, &err);
		if (!srgn->mctx) {
			HPB_DEBUG(hpb, "get mctx err %d srgn %d free_table %d",
				  err, srgn_idx, hpb->debug_free_table);
			goto out;
		}

		srgn->srgn_state = HPBSUBREGION_DIRTY;
	}
	HPB_DEBUG(hpb, "\x1b[44m\x1b[32m E->active region: %d \x1b[0m",
		  rgn->rgn_idx);
	TMSG(hpb->ufsf, hpb->lun, "Noti: ACT RG: %d", rgn->rgn_idx);

	ufshpb_add_lru_info(lru_info, rgn);
out:
	return err;
}

static inline void ufshpb_put_map_ctx(struct ufshpb_lu *hpb,
				      struct ufshpb_map_ctx *mctx)
{
	list_add(&mctx->list_table, &hpb->lh_map_ctx_free);
	hpb->debug_free_table++;
}

static inline void ufshpb_purge_active_subregion(struct ufshpb_lu *hpb,
						 struct ufshpb_subregion *srgn,
						 int state)
{
	if (state == HPBSUBREGION_UNUSED) {
		ufshpb_put_map_ctx(hpb, srgn->mctx);
		srgn->mctx = NULL;
	}

	srgn->srgn_state = state;
}

static inline void ufshpb_cleanup_lru_info(struct victim_select_info *lru_info,
					   struct ufshpb_region *rgn)
{
	list_del_init(&rgn->list_lru_rgn);
	rgn->rgn_state = HPBREGION_INACTIVE;
	atomic64_dec(&lru_info->active_cnt);
}

static void __ufshpb_evict_region(struct ufshpb_lu *hpb,
				  struct ufshpb_region *rgn)
{
	struct victim_select_info *lru_info;
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	lru_info = &hpb->lru_info;

	HPB_DEBUG(hpb, "\x1b[41m\x1b[33m C->EVICT region: %d \x1b[0m",
		  rgn->rgn_idx);
	TMSG(hpb->ufsf, hpb->lun, "Noti: EVIC RG: %d", rgn->rgn_idx);

	ufshpb_cleanup_lru_info(lru_info, rgn);

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		srgn = rgn->srgn_tbl + srgn_idx;

		ufshpb_purge_active_subregion(hpb, srgn, HPBSUBREGION_UNUSED);
=======
static void ufshpb_read_to_handler(struct work_struct *work)
{
	struct ufshpb_lu *hpb = container_of(work, struct ufshpb_lu,
					     ufshpb_read_to_work.work);
	struct victim_select_info *lru_info = &hpb->lru_info;
	struct ufshpb_region *rgn, *next_rgn;
	unsigned long flags;
	unsigned int poll;
	LIST_HEAD(expired_list);

	if (test_and_set_bit(TIMEOUT_WORK_RUNNING, &hpb->work_data_bits))
		return;

	spin_lock_irqsave(&hpb->rgn_state_lock, flags);

	list_for_each_entry_safe(rgn, next_rgn, &lru_info->lh_lru_rgn,
				 list_lru_rgn) {
		bool timedout = ktime_after(ktime_get(), rgn->read_timeout);

		if (timedout) {
			rgn->read_timeout_expiries--;
			if (is_rgn_dirty(rgn) ||
			    rgn->read_timeout_expiries == 0)
				list_add(&rgn->list_expired_rgn, &expired_list);
			else
				rgn->read_timeout = ktime_add_ms(ktime_get(),
						hpb->params.read_timeout_ms);
		}
	}

	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);

	list_for_each_entry_safe(rgn, next_rgn, &expired_list,
				 list_expired_rgn) {
		list_del_init(&rgn->list_expired_rgn);
		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
		ufshpb_update_inactive_info(hpb, rgn->rgn_idx);
		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
	}

	ufshpb_kick_map_work(hpb);

	clear_bit(TIMEOUT_WORK_RUNNING, &hpb->work_data_bits);

	poll = hpb->params.timeout_polling_interval_ms;
	schedule_delayed_work(&hpb->ufshpb_read_to_work,
			      msecs_to_jiffies(poll));
}

static void ufshpb_add_lru_info(struct victim_select_info *lru_info,
				struct ufshpb_region *rgn)
{
	rgn->rgn_state = HPB_RGN_ACTIVE;
	list_add_tail(&rgn->list_lru_rgn, &lru_info->lh_lru_rgn);
	atomic_inc(&lru_info->active_cnt);
	if (rgn->hpb->is_hcm) {
		rgn->read_timeout =
			ktime_add_ms(ktime_get(),
				     rgn->hpb->params.read_timeout_ms);
		rgn->read_timeout_expiries =
			rgn->hpb->params.read_timeout_expiries;
>>>>>>> upstream/android-13
	}
}

static void ufshpb_hit_lru_info(struct victim_select_info *lru_info,
				struct ufshpb_region *rgn)
{
<<<<<<< HEAD
	switch (lru_info->selection_type) {
	case LRU:
		list_move_tail(&rgn->list_lru_rgn, &lru_info->lh_lru_rgn);
		break;
	default:
		break;
	}
}

/*
 *  Must be held hpb_lock before call this func.
 */
static int ufshpb_check_issue_state_srgns(struct ufshpb_lu *hpb,
					  struct ufshpb_region *rgn)
{
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		srgn  = rgn->srgn_tbl + srgn_idx;

		if (srgn->srgn_state == HPBSUBREGION_ISSUED)
			return -EPERM;
	}
	return 0;
=======
	list_move_tail(&rgn->list_lru_rgn, &lru_info->lh_lru_rgn);
>>>>>>> upstream/android-13
}

static struct ufshpb_region *ufshpb_victim_lru_info(struct ufshpb_lu *hpb)
{
	struct victim_select_info *lru_info = &hpb->lru_info;
<<<<<<< HEAD
	struct ufshpb_region *rgn;
	struct ufshpb_region *victim_rgn = NULL;

	switch (lru_info->selection_type) {
	case LRU:
		list_for_each_entry(rgn, &lru_info->lh_lru_rgn, list_lru_rgn) {
			if (!rgn)
				break;

			if (ufshpb_check_issue_state_srgns(hpb, rgn))
				continue;

			victim_rgn = rgn;
			break;
		}
		break;
	default:
		break;
	}

	return victim_rgn;
}

static int ufshpb_evict_region(struct ufshpb_lu *hpb, struct ufshpb_region *rgn)
{
	unsigned long flags;

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	if (rgn->rgn_state == HPBREGION_PINNED) {
		/*
		 * Pinned active-block should not drop-out.
		 * But if so, it would treat error as critical,
		 * and it will run ufshpb_eh_work
		 */
		WARNING_MSG("pinned active-block drop-out error");
=======
	struct ufshpb_region *rgn, *victim_rgn = NULL;

	list_for_each_entry(rgn, &lru_info->lh_lru_rgn, list_lru_rgn) {
		if (ufshpb_check_srgns_issue_state(hpb, rgn))
			continue;

		/*
		 * in host control mode, verify that the exiting region
		 * has fewer reads
		 */
		if (hpb->is_hcm &&
		    rgn->reads > hpb->params.eviction_thld_exit)
			continue;

		victim_rgn = rgn;
		break;
	}

	if (!victim_rgn)
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			"%s: no region allocated\n",
			__func__);

	return victim_rgn;
}

static void ufshpb_cleanup_lru_info(struct victim_select_info *lru_info,
				    struct ufshpb_region *rgn)
{
	list_del_init(&rgn->list_lru_rgn);
	rgn->rgn_state = HPB_RGN_INACTIVE;
	atomic_dec(&lru_info->active_cnt);
}

static void ufshpb_purge_active_subregion(struct ufshpb_lu *hpb,
					  struct ufshpb_subregion *srgn)
{
	if (srgn->srgn_state != HPB_SRGN_UNUSED) {
		ufshpb_put_map_ctx(hpb, srgn->mctx);
		srgn->srgn_state = HPB_SRGN_UNUSED;
		srgn->mctx = NULL;
	}
}

static int ufshpb_issue_umap_req(struct ufshpb_lu *hpb,
				 struct ufshpb_region *rgn,
				 bool atomic)
{
	struct ufshpb_req *umap_req;
	int rgn_idx = rgn ? rgn->rgn_idx : 0;

	umap_req = ufshpb_get_req(hpb, rgn_idx, REQ_OP_DRV_OUT, atomic);
	if (!umap_req)
		return -ENOMEM;

	ufshpb_execute_umap_req(hpb, umap_req, rgn);

	return 0;
}

static int ufshpb_issue_umap_single_req(struct ufshpb_lu *hpb,
					struct ufshpb_region *rgn)
{
	return ufshpb_issue_umap_req(hpb, rgn, true);
}

static int ufshpb_issue_umap_all_req(struct ufshpb_lu *hpb)
{
	return ufshpb_issue_umap_req(hpb, NULL, false);
}

static void __ufshpb_evict_region(struct ufshpb_lu *hpb,
				 struct ufshpb_region *rgn)
{
	struct victim_select_info *lru_info;
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	lru_info = &hpb->lru_info;

	dev_dbg(&hpb->sdev_ufs_lu->sdev_dev, "evict region %d\n", rgn->rgn_idx);

	ufshpb_cleanup_lru_info(lru_info, rgn);

	for_each_sub_region(rgn, srgn_idx, srgn)
		ufshpb_purge_active_subregion(hpb, srgn);
}

static int ufshpb_evict_region(struct ufshpb_lu *hpb, struct ufshpb_region *rgn)
{
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&hpb->rgn_state_lock, flags);
	if (rgn->rgn_state == HPB_RGN_PINNED) {
		dev_warn(&hpb->sdev_ufs_lu->sdev_dev,
			 "pinned region cannot drop-out. region %d\n",
			 rgn->rgn_idx);
>>>>>>> upstream/android-13
		goto out;
	}

	if (!list_empty(&rgn->list_lru_rgn)) {
<<<<<<< HEAD
		if (ufshpb_check_issue_state_srgns(hpb, rgn))
			goto evict_fail;
=======
		if (ufshpb_check_srgns_issue_state(hpb, rgn)) {
			ret = -EBUSY;
			goto out;
		}

		if (hpb->is_hcm) {
			spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
			ret = ufshpb_issue_umap_single_req(hpb, rgn);
			spin_lock_irqsave(&hpb->rgn_state_lock, flags);
			if (ret)
				goto out;
		}
>>>>>>> upstream/android-13

		__ufshpb_evict_region(hpb, rgn);
	}
out:
<<<<<<< HEAD
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return 0;
evict_fail:
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return -EPERM;
}

static inline struct
ufshpb_rsp_field *ufshpb_get_hpb_rsp(struct ufshcd_lrb *lrbp)
{
	return (struct ufshpb_rsp_field *)&lrbp->ucd_rsp_ptr->sr.sense_data_len;
}

static int ufshpb_prepare_map_req(struct ufshpb_lu *hpb,
				  struct ufshpb_subregion *srgn)
{
	struct ufshpb_req *map_req;
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&hpb->hpb_lock, flags);

	if (srgn->srgn_state == HPBSUBREGION_ISSUED) {
		ret = -EAGAIN;
		goto unlock_out;
	}

	map_req = ufshpb_get_map_req(hpb);
	if (!map_req) {
		ret = -ENOMEM;
		goto unlock_out;
	}

	srgn->srgn_state = HPBSUBREGION_ISSUED;

	ret = ufshpb_clean_dirty_bitmap(hpb, srgn);
	if (ret) {
		ufshpb_put_map_req(hpb, map_req);
		goto unlock_out;
	}
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);

	ufshpb_set_map_req(hpb, srgn->rgn_idx, srgn->srgn_idx,
			   srgn->mctx, map_req);

	ret = ufshpb_lu_get(hpb);
	if (ret) {
		WARNING_MSG("warning: ufshpb_lu_get failed.. %d", ret);
		spin_lock_irqsave(&hpb->hpb_lock, flags);
		ufshpb_put_map_req(hpb, map_req);
		goto unlock_out;
	}

	ret = ufshpb_issue_map_req(hpb, map_req);
	if (ret) {
		ERR_MSG("issue map_req failed. [%d-%d] err %d",
			srgn->rgn_idx, srgn->srgn_idx, ret);
		ufshpb_lu_put(hpb);
		goto wakeup_ee_worker;
	}
	return ret;
unlock_out:
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return ret;
wakeup_ee_worker:
	ufshpb_failed(hpb, __func__);
	return ret;
}

static int ufshpb_load_region(struct ufshpb_lu *hpb, struct ufshpb_region *rgn)
{
	struct ufshpb_region *victim_rgn;
=======
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
	return ret;
}

static int ufshpb_issue_map_req(struct ufshpb_lu *hpb,
				struct ufshpb_region *rgn,
				struct ufshpb_subregion *srgn)
{
	struct ufshpb_req *map_req;
	unsigned long flags;
	int ret;
	int err = -EAGAIN;
	bool alloc_required = false;
	enum HPB_SRGN_STATE state = HPB_SRGN_INVALID;

	spin_lock_irqsave(&hpb->rgn_state_lock, flags);

	if (ufshpb_get_state(hpb) != HPB_PRESENT) {
		dev_notice(&hpb->sdev_ufs_lu->sdev_dev,
			   "%s: ufshpb state is not PRESENT\n", __func__);
		goto unlock_out;
	}

	if ((rgn->rgn_state == HPB_RGN_INACTIVE) &&
	    (srgn->srgn_state == HPB_SRGN_INVALID)) {
		err = 0;
		goto unlock_out;
	}

	if (srgn->srgn_state == HPB_SRGN_UNUSED)
		alloc_required = true;

	/*
	 * If the subregion is already ISSUED state,
	 * a specific event (e.g., GC or wear-leveling, etc.) occurs in
	 * the device and HPB response for map loading is received.
	 * In this case, after finishing the HPB_READ_BUFFER,
	 * the next HPB_READ_BUFFER is performed again to obtain the latest
	 * map data.
	 */
	if (srgn->srgn_state == HPB_SRGN_ISSUED)
		goto unlock_out;

	srgn->srgn_state = HPB_SRGN_ISSUED;
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);

	if (alloc_required) {
		srgn->mctx = ufshpb_get_map_ctx(hpb, srgn->is_last);
		if (!srgn->mctx) {
			dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			    "get map_ctx failed. region %d - %d\n",
			    rgn->rgn_idx, srgn->srgn_idx);
			state = HPB_SRGN_UNUSED;
			goto change_srgn_state;
		}
	}

	map_req = ufshpb_get_map_req(hpb, srgn);
	if (!map_req)
		goto change_srgn_state;


	ret = ufshpb_execute_map_req(hpb, map_req, srgn->is_last);
	if (ret) {
		dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			   "%s: issue map_req failed: %d, region %d - %d\n",
			   __func__, ret, srgn->rgn_idx, srgn->srgn_idx);
		goto free_map_req;
	}
	return 0;

free_map_req:
	ufshpb_put_map_req(hpb, map_req);
change_srgn_state:
	spin_lock_irqsave(&hpb->rgn_state_lock, flags);
	srgn->srgn_state = state;
unlock_out:
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
	return err;
}

static int ufshpb_add_region(struct ufshpb_lu *hpb, struct ufshpb_region *rgn)
{
	struct ufshpb_region *victim_rgn = NULL;
>>>>>>> upstream/android-13
	struct victim_select_info *lru_info = &hpb->lru_info;
	unsigned long flags;
	int ret = 0;

<<<<<<< HEAD
	/*
	 * if already region is added to lru_list,
	 * just initiate the information of lru.
	 * because the region already has the map ctx.
	 * (!list_empty(&rgn->list_region) == region->state=active...)
	 */
	spin_lock_irqsave(&hpb->hpb_lock, flags);
=======
	spin_lock_irqsave(&hpb->rgn_state_lock, flags);
	/*
	 * If region belongs to lru_list, just move the region
	 * to the front of lru list because the state of the region
	 * is already active-state.
	 */
>>>>>>> upstream/android-13
	if (!list_empty(&rgn->list_lru_rgn)) {
		ufshpb_hit_lru_info(lru_info, rgn);
		goto out;
	}

<<<<<<< HEAD
	if (rgn->rgn_state == HPBREGION_INACTIVE) {
		if (atomic64_read(&lru_info->active_cnt)
		    == lru_info->max_lru_active_cnt) {
			victim_rgn = ufshpb_victim_lru_info(hpb);
			if (!victim_rgn) {
				HPB_DEBUG(hpb, "UFSHPB victim_rgn is NULL");
				ret = -ENOMEM;
				goto out;
			}
			TMSG(hpb->ufsf, hpb->lun, "Noti: VT RG %d",
			     victim_rgn->rgn_idx);
			HPB_DEBUG(hpb, "LRU MAX(=%ld). victim choose %d",
				  atomic64_read(&lru_info->active_cnt),
				  victim_rgn->rgn_idx);
=======
	if (rgn->rgn_state == HPB_RGN_INACTIVE) {
		if (atomic_read(&lru_info->active_cnt) ==
		    lru_info->max_lru_active_cnt) {
			/*
			 * If the maximum number of active regions
			 * is exceeded, evict the least recently used region.
			 * This case may occur when the device responds
			 * to the eviction information late.
			 * It is okay to evict the least recently used region,
			 * because the device could detect this region
			 * by not issuing HPB_READ
			 *
			 * in host control mode, verify that the entering
			 * region has enough reads
			 */
			if (hpb->is_hcm &&
			    rgn->reads < hpb->params.eviction_thld_enter) {
				ret = -EACCES;
				goto out;
			}

			victim_rgn = ufshpb_victim_lru_info(hpb);
			if (!victim_rgn) {
				dev_warn(&hpb->sdev_ufs_lu->sdev_dev,
				    "cannot get victim region %s\n",
				    hpb->is_hcm ? "" : "error");
				ret = -ENOMEM;
				goto out;
			}

			dev_dbg(&hpb->sdev_ufs_lu->sdev_dev,
				"LRU full (%d), choose victim %d\n",
				atomic_read(&lru_info->active_cnt),
				victim_rgn->rgn_idx);

			if (hpb->is_hcm) {
				spin_unlock_irqrestore(&hpb->rgn_state_lock,
						       flags);
				ret = ufshpb_issue_umap_single_req(hpb,
								victim_rgn);
				spin_lock_irqsave(&hpb->rgn_state_lock,
						  flags);
				if (ret)
					goto out;
			}
>>>>>>> upstream/android-13

			__ufshpb_evict_region(hpb, victim_rgn);
		}

<<<<<<< HEAD
		ret = ufshpb_add_region(hpb, rgn);
		if (ret) {
			ERR_MSG("UFSHPB memory allocation failed");
			spin_unlock_irqrestore(&hpb->hpb_lock, flags);
			goto wake_up_ee_worker;
		}
	}
out:
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return ret;
wake_up_ee_worker:
	ufshpb_failed(hpb, __func__);
=======
		/*
		 * When a region is added to lru_info list_head,
		 * it is guaranteed that the subregion has been
		 * assigned all mctx. If failed, try to receive mctx again
		 * without being added to lru_info list_head
		 */
		ufshpb_add_lru_info(lru_info, rgn);
	}
out:
	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
>>>>>>> upstream/android-13
	return ret;
}

static void ufshpb_rsp_req_region_update(struct ufshpb_lu *hpb,
<<<<<<< HEAD
					 struct ufshpb_rsp_field *rsp_field)
{
	int num, rgn_idx, srgn_idx;

	/*
	 * If active rgn = inactive rgn, choose inactive rgn.
	 * So, active process -> inactive process
	 */
	spin_lock(&hpb->rsp_list_lock);
	for (num = 0; num < rsp_field->active_rgn_cnt; num++) {
		rgn_idx = be16_to_cpu(rsp_field->hpb_active_field[num].active_rgn);
		srgn_idx = be16_to_cpu(rsp_field->hpb_active_field[num].active_srgn);

		HPB_DEBUG(hpb, "act num: %d, region: %d, subregion: %d",
			  num + 1, rgn_idx, srgn_idx);
		ufshpb_update_active_info(hpb, rgn_idx, srgn_idx);
		atomic64_inc(&hpb->rb_active_cnt);
	}

	for (num = 0; num < rsp_field->inactive_rgn_cnt; num++) {
		rgn_idx = be16_to_cpu(rsp_field->hpb_inactive_field[num]);
		HPB_DEBUG(hpb, "inact num: %d, region: %d", num + 1, rgn_idx);
		ufshpb_update_inactive_info(hpb, rgn_idx);
		atomic64_inc(&hpb->rb_inactive_cnt);
	}
	spin_unlock(&hpb->rsp_list_lock);

	TMSG(hpb->ufsf, hpb->lun, "Noti: #ACT %u, #INACT %u",
	     rsp_field->active_rgn_cnt, rsp_field->inactive_rgn_cnt);

	schedule_work(&hpb->ufshpb_task_workq);
}

static inline int ufshpb_may_field_valid(struct ufshcd_lrb *lrbp,
					 struct ufshpb_rsp_field *rsp_field)
{
	if (be16_to_cpu(rsp_field->sense_data_len) != DEV_SENSE_SEG_LEN ||
	    rsp_field->desc_type != DEV_DES_TYPE ||
	    rsp_field->additional_len != DEV_ADDITIONAL_LEN ||
	    rsp_field->hpb_type == HPB_RSP_NONE ||
	    rsp_field->active_rgn_cnt > MAX_ACTIVE_NUM ||
	    rsp_field->inactive_rgn_cnt > MAX_INACTIVE_NUM ||
	    (!rsp_field->active_rgn_cnt && !rsp_field->inactive_rgn_cnt))
		return -EINVAL;

	if (!ufsf_is_valid_lun(lrbp->lun)) {
		ERR_MSG("LU(%d) is not supported", lrbp->lun);
		return -EINVAL;
	}

	return 0;
}

static bool ufshpb_is_empty_rsp_lists(struct ufshpb_lu *hpb)
{
	bool ret = true;
	unsigned long flags;

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	if (!list_empty(&hpb->lh_inact_rgn) || !list_empty(&hpb->lh_act_srgn))
		ret = false;
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

	return ret;
}

/* routine : isr (ufs) */
void ufshpb_rsp_upiu(struct ufsf_feature *ufsf, struct ufshcd_lrb *lrbp)
{
	struct ufshpb_lu *hpb;
	struct ufshpb_rsp_field *rsp_field;
	struct ufshpb_rsp_field sense_data;
	int data_seg_len, ret;
=======
					 struct utp_hpb_rsp *rsp_field)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	int i, rgn_i, srgn_i;

	BUILD_BUG_ON(sizeof(struct ufshpb_active_field) != HPB_ACT_FIELD_SIZE);
	/*
	 * If the active region and the inactive region are the same,
	 * we will inactivate this region.
	 * The device could check this (region inactivated) and
	 * will response the proper active region information
	 */
	for (i = 0; i < rsp_field->active_rgn_cnt; i++) {
		rgn_i =
			be16_to_cpu(rsp_field->hpb_active_field[i].active_rgn);
		srgn_i =
			be16_to_cpu(rsp_field->hpb_active_field[i].active_srgn);

		rgn = hpb->rgn_tbl + rgn_i;
		if (hpb->is_hcm &&
		    (rgn->rgn_state != HPB_RGN_ACTIVE || is_rgn_dirty(rgn))) {
			/*
			 * in host control mode, subregion activation
			 * recommendations are only allowed to active regions.
			 * Also, ignore recommendations for dirty regions - the
			 * host will make decisions concerning those by himself
			 */
			continue;
		}

		dev_dbg(&hpb->sdev_ufs_lu->sdev_dev,
			"activate(%d) region %d - %d\n", i, rgn_i, srgn_i);

		spin_lock(&hpb->rsp_list_lock);
		ufshpb_update_active_info(hpb, rgn_i, srgn_i);
		spin_unlock(&hpb->rsp_list_lock);

		srgn = rgn->srgn_tbl + srgn_i;

		/* blocking HPB_READ */
		spin_lock(&hpb->rgn_state_lock);
		if (srgn->srgn_state == HPB_SRGN_VALID)
			srgn->srgn_state = HPB_SRGN_INVALID;
		spin_unlock(&hpb->rgn_state_lock);
	}

	if (hpb->is_hcm) {
		/*
		 * in host control mode the device is not allowed to inactivate
		 * regions
		 */
		goto out;
	}

	for (i = 0; i < rsp_field->inactive_rgn_cnt; i++) {
		rgn_i = be16_to_cpu(rsp_field->hpb_inactive_field[i]);
		dev_dbg(&hpb->sdev_ufs_lu->sdev_dev,
			"inactivate(%d) region %d\n", i, rgn_i);

		spin_lock(&hpb->rsp_list_lock);
		ufshpb_update_inactive_info(hpb, rgn_i);
		spin_unlock(&hpb->rsp_list_lock);

		rgn = hpb->rgn_tbl + rgn_i;

		spin_lock(&hpb->rgn_state_lock);
		if (rgn->rgn_state != HPB_RGN_INACTIVE) {
			for (srgn_i = 0; srgn_i < rgn->srgn_cnt; srgn_i++) {
				srgn = rgn->srgn_tbl + srgn_i;
				if (srgn->srgn_state == HPB_SRGN_VALID)
					srgn->srgn_state = HPB_SRGN_INVALID;
			}
		}
		spin_unlock(&hpb->rgn_state_lock);

	}

out:
	dev_dbg(&hpb->sdev_ufs_lu->sdev_dev, "Noti: #ACT %u #INACT %u\n",
		rsp_field->active_rgn_cnt, rsp_field->inactive_rgn_cnt);

	if (ufshpb_get_state(hpb) == HPB_PRESENT)
		queue_work(ufshpb_wq, &hpb->map_work);
}

static void ufshpb_dev_reset_handler(struct ufshpb_lu *hpb)
{
	struct victim_select_info *lru_info = &hpb->lru_info;
	struct ufshpb_region *rgn;
	unsigned long flags;

	spin_lock_irqsave(&hpb->rgn_state_lock, flags);

	list_for_each_entry(rgn, &lru_info->lh_lru_rgn, list_lru_rgn)
		set_bit(RGN_FLAG_UPDATE, &rgn->rgn_flags);

	spin_unlock_irqrestore(&hpb->rgn_state_lock, flags);
}

/*
 * This function will parse recommended active subregion information in sense
 * data field of response UPIU with SAM_STAT_GOOD state.
 */
void ufshpb_rsp_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(lrbp->cmd->device);
	struct utp_hpb_rsp *rsp_field = &lrbp->ucd_rsp_ptr->hr;
	int data_seg_len;

	if (unlikely(lrbp->lun != rsp_field->lun)) {
		struct scsi_device *sdev;
		bool found = false;

		__shost_for_each_device(sdev, hba->host) {
			hpb = ufshpb_get_hpb_data(sdev);

			if (!hpb)
				continue;

			if (rsp_field->lun == hpb->lun) {
				found = true;
				break;
			}
		}

		if (!found)
			return;
	}

	if (!hpb)
		return;

	if (ufshpb_get_state(hpb) == HPB_INIT)
		return;

	if ((ufshpb_get_state(hpb) != HPB_PRESENT) &&
	    (ufshpb_get_state(hpb) != HPB_SUSPEND)) {
		dev_notice(&hpb->sdev_ufs_lu->sdev_dev,
			   "%s: ufshpb state is not PRESENT/SUSPEND\n",
			   __func__);
		return;
	}
>>>>>>> upstream/android-13

	data_seg_len = be32_to_cpu(lrbp->ucd_rsp_ptr->header.dword_2)
		& MASK_RSP_UPIU_DATA_SEG_LEN;

<<<<<<< HEAD
	if (!data_seg_len) {
		bool do_workq = false;

		if (!ufsf_is_valid_lun(lrbp->lun))
			return;

		hpb = ufsf->ufshpb_lup[lrbp->lun];
		ret = ufshpb_lu_get(hpb);
		if (ret)
			return;

		do_workq = !ufshpb_is_empty_rsp_lists(hpb);
		if (do_workq)
			schedule_work(&hpb->ufshpb_task_workq);

		goto put_hpb;
	}

	memcpy(&sense_data, &lrbp->ucd_rsp_ptr->sr.sense_data_len,
		sizeof(struct ufshpb_rsp_field));
	rsp_field = &sense_data;

	if (ufshpb_may_field_valid(lrbp, rsp_field)) {
		WARN_ON(rsp_field->additional_len != DEV_ADDITIONAL_LEN);
		return;
	}

	hpb = ufsf->ufshpb_lup[lrbp->lun];
	ret = ufshpb_lu_get(hpb);
	if (ret) {
		WARNING_MSG("warning: ufshpb_lu_get failed %d..", ret);
		return;
	}

	if (hpb->force_map_req_disable)
		goto put_hpb;

	HPB_DEBUG(hpb, "**** HPB Noti %u LUN %u Seg-Len %u, #ACT %u, #INACT %u",
		  rsp_field->hpb_type, lrbp->lun,
		  be32_to_cpu(lrbp->ucd_rsp_ptr->header.dword_2) &
		  MASK_RSP_UPIU_DATA_SEG_LEN, rsp_field->active_rgn_cnt,
		  rsp_field->inactive_rgn_cnt);
	atomic64_inc(&hpb->rb_noti_cnt);

	switch (rsp_field->hpb_type) {
	case HPB_RSP_REQ_REGION_UPDATE:
		WARN_ON(data_seg_len != DEV_DATA_SEG_LEN);
		ufshpb_rsp_req_region_update(hpb, rsp_field);
		goto put_hpb;
	default:
		HPB_DEBUG(hpb, "hpb_type is not available : %d",
			  rsp_field->hpb_type);
		goto put_hpb;
	}

put_hpb:
	ufshpb_lu_put(hpb);
}

static int ufshpb_execute_map_req_wait(struct ufshpb_lu *hpb,
				       unsigned char *cmd,
				       struct ufshpb_subregion *srgn)
{
	struct ufsf_feature *ufsf = hpb->ufsf;
	struct scsi_device *sdev;
	struct request_queue *q;
	struct request *req;
	struct scsi_request *rq;
	struct bio *bio;
	struct scsi_sense_hdr sshdr = {0};
	unsigned long flags;
	int ret = 0;

	sdev = ufsf->sdev_ufs_lu[hpb->lun];
	if (!sdev) {
		WARNING_MSG("cannot find scsi_device");
		return -ENODEV;
	}

	q = sdev->request_queue;

	ret = ufshpb_get_scsi_device(ufsf->hba, sdev);
	if (ret)
		return ret;

	req = blk_get_request(q, REQ_OP_SCSI_IN, 0);
	if (IS_ERR(req)) {
		WARNING_MSG("cannot get request");
		ret = -EIO;
		goto sdev_put_out;
	}

	bio = bio_kmalloc(GFP_KERNEL, hpb->mpages_per_srgn);
	if (!bio) {
		ret = -ENOMEM;
		goto req_put_out;
	}

	ret = ufshpb_map_req_add_bio_page(hpb, q, bio, srgn->mctx);
	if (ret)
		goto mem_free_out;

	/* 1. request setup*/
	blk_rq_append_bio(req, &bio); /* req->__data_len */
	req->timeout = msecs_to_jiffies(30000);
	req->cmd_flags |= REQ_OP_READ;
	req->rq_flags |= RQF_QUIET | RQF_PREEMPT;

	/* 2. scsi_request setup */
	rq = scsi_req(req);
	rq->cmd_len = scsi_command_size(cmd);
	memcpy(rq->cmd, cmd, rq->cmd_len);

	blk_execute_rq(q, NULL, req, 1);
	if (rq->result) {
		ret = -EIO;
		scsi_normalize_sense(rq->sense, SCSI_SENSE_BUFFERSIZE, &sshdr);
		ERR_MSG("code %x sense_key %x asc %x ascq %x",
			sshdr.response_code, sshdr.sense_key, sshdr.asc,
			sshdr.ascq);
		ERR_MSG("byte4 %x byte5 %x byte6 %x additional_len %x",
			sshdr.byte4, sshdr.byte5, sshdr.byte6,
			sshdr.additional_length);
		spin_lock_irqsave(&hpb->hpb_lock, flags);
		ufshpb_error_active_subregion(hpb, srgn);
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	}
mem_free_out:
	bio_put(bio);
req_put_out:
	blk_put_request(req);
sdev_put_out:
	scsi_device_put(sdev);
	return ret;
}

static int ufshpb_issue_map_req_from_list(struct ufshpb_lu *hpb)
{
	struct ufshpb_subregion *srgn;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);

	while ((srgn = list_first_entry_or_null(&hpb->lh_pinned_srgn,
						struct ufshpb_subregion,
						list_act_srgn))) {
		unsigned char cmd[10] = { 0 };

		list_del_init(&srgn->list_act_srgn);
		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

		ufshpb_set_read_buf_cmd(cmd, srgn->rgn_idx, srgn->srgn_idx,
					hpb->srgn_mem_size);

		if (hpb->debug)
			ufshpb_check_ppn(hpb, srgn->rgn_idx, srgn->srgn_idx,
					 srgn->mctx, "ISSUE");

		TMSG(hpb->ufsf, hpb->lun, "Noti: I RB %d - %d",
		     srgn->rgn_idx, srgn->srgn_idx);

		ret = ufshpb_execute_map_req_wait(hpb, cmd, srgn);
		if (ret < 0) {
			ERR_MSG("region %d sub %d failed with err %d",
				srgn->rgn_idx, srgn->srgn_idx, ret);
			spin_lock_irqsave(&hpb->rsp_list_lock, flags);
			if (list_empty(&srgn->list_act_srgn))
				list_add(&srgn->list_act_srgn,
					 &hpb->lh_pinned_srgn);
			continue;
		}

		TMSG(hpb->ufsf, hpb->lun, "Noti: C RB %d - %d",
		     srgn->rgn_idx, srgn->srgn_idx);

		if (hpb->debug)
			ufshpb_check_ppn(hpb, srgn->rgn_idx, srgn->srgn_idx,
					 srgn->mctx, "COMPL");

		spin_lock_irqsave(&hpb->hpb_lock, flags);
		ufshpb_clean_active_subregion(hpb, srgn);
		spin_unlock_irqrestore(&hpb->hpb_lock, flags);

		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	}

	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

	return 0;
}

static void ufshpb_work_handler(struct work_struct *work)
{
	struct ufshpb_lu *hpb;
	int ret;

	hpb = container_of(work, struct ufshpb_lu, ufshpb_work);
	HPB_DEBUG(hpb, "worker start for pinned region");

	if (!list_empty(&hpb->lh_pinned_srgn)) {
		ret = ufshpb_issue_map_req_from_list(hpb);
		/*
		 * if its function failed at init time,
		 * ufshpb-device will request map-req,
		 * so it is not critical-error, and just finish work-handler
		 */
		if (ret)
			HPB_DEBUG(hpb, "failed map-issue. ret %d", ret);
	}

	HPB_DEBUG(hpb, "worker end");
}

static int ufshpb_check_pm(struct ufshpb_lu *hpb)
{
	struct ufs_hba *hba = hpb->ufsf->hba;

	if (hba->pm_op_in_progress ||
	    hba->curr_dev_pwr_mode != UFS_ACTIVE_PWR_MODE) {
		INFO_MSG("hba current power state %d pm_progress %d",
			 hba->curr_dev_pwr_mode,
			 hba->pm_op_in_progress);
		return -ENODEV;
	}
	return 0;
}

static void ufshpb_retry_work_handler(struct work_struct *work)
{
	struct ufshpb_lu *hpb;
	struct delayed_work *dwork = to_delayed_work(work);
	struct ufshpb_req *map_req, *next;
	unsigned long flags;
	int ret = 0;

	LIST_HEAD(retry_list);

	hpb = container_of(dwork, struct ufshpb_lu, ufshpb_retry_work);

	if (ufshpb_check_pm(hpb))
		return;

	ret = ufshpb_lu_get(hpb);
	if (ret) {
		WARNING_MSG("warning: ufshpb_lu_get failed %d..", ret);
		return;
	}

	HPB_DEBUG(hpb, "retry worker start");

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	list_splice_init(&hpb->lh_map_req_retry, &retry_list);
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);

	list_for_each_entry_safe(map_req, next, &retry_list, list_req) {
		list_del_init(&map_req->list_req);

		ret = ufshpb_lu_get(hpb);
		if (ret) {
			WARNING_MSG("warning: ufshpb_lu_get failed %d..", ret);
			spin_lock_irqsave(&hpb->hpb_lock, flags);
			ufshpb_put_map_req(hpb, map_req);
			spin_unlock_irqrestore(&hpb->hpb_lock, flags);
			continue;
		}

		ret = ufshpb_issue_map_req(hpb, map_req);
		if (ret) {
			ERR_MSG("issue map_req failed. [%d-%d] err %d",
				map_req->rb.rgn_idx, map_req->rb.srgn_idx, ret);
			ufshpb_lu_put(hpb);
			goto wakeup_ee_worker;
		}
	}
	HPB_DEBUG(hpb, "worker end");
	ufshpb_lu_put(hpb);
	return;
wakeup_ee_worker:
	ufshpb_lu_put(hpb);
	ufshpb_failed(hpb, __func__);
}

static void ufshpb_add_starved_list(struct ufshpb_lu *hpb,
				    struct ufshpb_region *rgn,
				    struct list_head *starved_list)
{
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	if (!list_empty(&rgn->list_inact_rgn))
		return;

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		srgn = rgn->srgn_tbl + srgn_idx;

		if (!list_empty(&srgn->list_act_srgn))
			return;
	}

	list_add_tail(&rgn->list_inact_rgn, starved_list);
}

static void ufshpb_run_inactive_region_list(struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn;
	unsigned long flags;
	int ret;
	LIST_HEAD(starved_list);

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	while ((rgn = list_first_entry_or_null(&hpb->lh_inact_rgn,
					       struct ufshpb_region,
					       list_inact_rgn))) {
		list_del_init(&rgn->list_inact_rgn);
		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

		ret = ufshpb_evict_region(hpb, rgn);
		if (ret) {
			spin_lock_irqsave(&hpb->rsp_list_lock, flags);
			ufshpb_add_starved_list(hpb, rgn, &starved_list);
			spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
		}

		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	}

	list_splice(&starved_list, &hpb->lh_inact_rgn);
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
=======
	/* To flush remained rsp_list, we queue the map_work task */
	if (!data_seg_len) {
		if (!ufshpb_is_general_lun(hpb->lun))
			return;

		ufshpb_kick_map_work(hpb);
		return;
	}

	BUILD_BUG_ON(sizeof(struct utp_hpb_rsp) != UTP_HPB_RSP_SIZE);

	if (!ufshpb_is_hpb_rsp_valid(hba, lrbp, rsp_field))
		return;

	hpb->stats.rb_noti_cnt++;

	switch (rsp_field->hpb_op) {
	case HPB_RSP_REQ_REGION_UPDATE:
		if (data_seg_len != DEV_DATA_SEG_LEN)
			dev_warn(&hpb->sdev_ufs_lu->sdev_dev,
				 "%s: data seg length is not same.\n",
				 __func__);
		ufshpb_rsp_req_region_update(hpb, rsp_field);
		break;
	case HPB_RSP_DEV_RESET:
		dev_warn(&hpb->sdev_ufs_lu->sdev_dev,
			 "UFS device lost HPB information during PM.\n");

		if (hpb->is_hcm) {
			struct scsi_device *sdev;

			__shost_for_each_device(sdev, hba->host) {
				struct ufshpb_lu *h = sdev->hostdata;

				if (h)
					ufshpb_dev_reset_handler(h);
			}
		}

		break;
	default:
		dev_notice(&hpb->sdev_ufs_lu->sdev_dev,
			   "hpb_op is not available: %d\n",
			   rsp_field->hpb_op);
		break;
	}
>>>>>>> upstream/android-13
}

static void ufshpb_add_active_list(struct ufshpb_lu *hpb,
				   struct ufshpb_region *rgn,
				   struct ufshpb_subregion *srgn)
{
	if (!list_empty(&rgn->list_inact_rgn))
		return;

	if (!list_empty(&srgn->list_act_srgn)) {
		list_move(&srgn->list_act_srgn, &hpb->lh_act_srgn);
		return;
	}

	list_add(&srgn->list_act_srgn, &hpb->lh_act_srgn);
}

<<<<<<< HEAD
=======
static void ufshpb_add_pending_evict_list(struct ufshpb_lu *hpb,
					  struct ufshpb_region *rgn,
					  struct list_head *pending_list)
{
	struct ufshpb_subregion *srgn;
	int srgn_idx;

	if (!list_empty(&rgn->list_inact_rgn))
		return;

	for_each_sub_region(rgn, srgn_idx, srgn)
		if (!list_empty(&srgn->list_act_srgn))
			return;

	list_add_tail(&rgn->list_inact_rgn, pending_list);
}

>>>>>>> upstream/android-13
static void ufshpb_run_active_subregion_list(struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	while ((srgn = list_first_entry_or_null(&hpb->lh_act_srgn,
						struct ufshpb_subregion,
						list_act_srgn))) {
<<<<<<< HEAD
		list_del_init(&srgn->list_act_srgn);

		if (hpb->force_map_req_disable) {
			HPB_DEBUG(hpb, "map_req disabled");
			continue;
		}

		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

		rgn = hpb->rgn_tbl + srgn->rgn_idx;

		ret = ufshpb_load_region(hpb, rgn);
		if (ret)
			break;

		ret = ufshpb_prepare_map_req(hpb, srgn);
		if (ret)
			break;

		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	}

	if (ret) {
		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
		ufshpb_add_active_list(hpb, rgn, srgn);
	}
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
}

static void ufshpb_task_workq_fn(struct work_struct *work)
{
	struct ufshpb_lu *hpb;
	int ret;

	hpb = container_of(work, struct ufshpb_lu, ufshpb_task_workq);
	ret = ufshpb_lu_get(hpb);
	if (ret) {
		WARNING_MSG("warning: ufshpb_lu_get failed %d..", ret);
=======
		if (ufshpb_get_state(hpb) == HPB_SUSPEND)
			break;

		list_del_init(&srgn->list_act_srgn);
		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

		rgn = hpb->rgn_tbl + srgn->rgn_idx;
		ret = ufshpb_add_region(hpb, rgn);
		if (ret)
			goto active_failed;

		ret = ufshpb_issue_map_req(hpb, rgn, srgn);
		if (ret) {
			dev_err(&hpb->sdev_ufs_lu->sdev_dev,
			    "issue map_req failed. ret %d, region %d - %d\n",
			    ret, rgn->rgn_idx, srgn->srgn_idx);
			goto active_failed;
		}
		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	}
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
	return;

active_failed:
	dev_err(&hpb->sdev_ufs_lu->sdev_dev, "failed to activate region %d - %d, will retry\n",
		   rgn->rgn_idx, srgn->srgn_idx);
	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	ufshpb_add_active_list(hpb, rgn, srgn);
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
}

static void ufshpb_run_inactive_region_list(struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn;
	unsigned long flags;
	int ret;
	LIST_HEAD(pending_list);

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	while ((rgn = list_first_entry_or_null(&hpb->lh_inact_rgn,
					       struct ufshpb_region,
					       list_inact_rgn))) {
		if (ufshpb_get_state(hpb) == HPB_SUSPEND)
			break;

		list_del_init(&rgn->list_inact_rgn);
		spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);

		ret = ufshpb_evict_region(hpb, rgn);
		if (ret) {
			spin_lock_irqsave(&hpb->rsp_list_lock, flags);
			ufshpb_add_pending_evict_list(hpb, rgn, &pending_list);
			spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
		}

		spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	}

	list_splice(&pending_list, &hpb->lh_inact_rgn);
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
}

static void ufshpb_normalization_work_handler(struct work_struct *work)
{
	struct ufshpb_lu *hpb = container_of(work, struct ufshpb_lu,
					     ufshpb_normalization_work);
	int rgn_idx;
	u8 factor = hpb->params.normalization_factor;

	for (rgn_idx = 0; rgn_idx < hpb->rgns_per_lu; rgn_idx++) {
		struct ufshpb_region *rgn = hpb->rgn_tbl + rgn_idx;
		int srgn_idx;

		spin_lock(&rgn->rgn_lock);
		rgn->reads = 0;
		for (srgn_idx = 0; srgn_idx < hpb->srgns_per_rgn; srgn_idx++) {
			struct ufshpb_subregion *srgn = rgn->srgn_tbl + srgn_idx;

			srgn->reads >>= factor;
			rgn->reads += srgn->reads;
		}
		spin_unlock(&rgn->rgn_lock);

		if (rgn->rgn_state != HPB_RGN_ACTIVE || rgn->reads)
			continue;

		/* if region is active but has no reads - inactivate it */
		spin_lock(&hpb->rsp_list_lock);
		ufshpb_update_inactive_info(hpb, rgn->rgn_idx);
		spin_unlock(&hpb->rsp_list_lock);
	}
}

static void ufshpb_map_work_handler(struct work_struct *work)
{
	struct ufshpb_lu *hpb = container_of(work, struct ufshpb_lu, map_work);

	if (ufshpb_get_state(hpb) != HPB_PRESENT) {
		dev_notice(&hpb->sdev_ufs_lu->sdev_dev,
			   "%s: ufshpb state is not PRESENT\n", __func__);
>>>>>>> upstream/android-13
		return;
	}

	ufshpb_run_inactive_region_list(hpb);
	ufshpb_run_active_subregion_list(hpb);
<<<<<<< HEAD

	ufshpb_lu_put(hpb);
}

static void ufshpb_init_constant(void)
{
	sects_per_blk_shift = ffs(BLOCK) - ffs(SECTOR);
	INIT_INFO("sects_per_blk_shift: %u %u", sects_per_blk_shift,
		  ffs(SECTORS_PER_BLOCK) - 1);

	bits_per_dword_shift = ffs(BITS_PER_DWORD) - 1;
	bits_per_dword_mask = BITS_PER_DWORD - 1;
	INIT_INFO("bits_per_dword %u shift %u mask 0x%X", BITS_PER_DWORD,
		  bits_per_dword_shift, bits_per_dword_mask);

	bits_per_byte_shift = ffs(BITS_PER_BYTE) - 1;
	INIT_INFO("bits_per_byte %u shift %u", BITS_PER_BYTE,
		  bits_per_byte_shift);
}

static inline void ufshpb_map_req_mempool_remove(struct ufshpb_lu *hpb)
{
	int i;

	for (i = 0; i < hpb->qd; i++) {
		kfree(hpb->map_req[i].req);
		bio_put(hpb->map_req[i].bio);
	}

	kfree(hpb->map_req);
}

static inline void ufshpb_pre_req_mempool_remove(struct ufshpb_lu *hpb)
{
	int i;

	for (i = 0; i < hpb->qd; i++) {
		kfree(hpb->pre_req[i].req);
		bio_put(hpb->pre_req[i].bio);
		__free_page(hpb->pre_req[i].wb.m_page);
	}

	kfree(hpb->pre_req);
}

static void ufshpb_table_mempool_remove(struct ufshpb_lu *hpb)
{
	struct ufshpb_map_ctx *mctx, *next;
	int i;

	/*
	 * the mctx in the lh_map_ctx_free has been allocated completely.
	 */
	list_for_each_entry_safe(mctx, next, &hpb->lh_map_ctx_free,
				 list_table) {
		for (i = 0; i < hpb->mpages_per_srgn; i++)
			__free_page(mctx->m_page[i]);

		vfree(mctx->ppn_dirty);
		kfree(mctx->m_page);
		kfree(mctx);
		hpb->alloc_mctx--;
	}
=======
>>>>>>> upstream/android-13
}

/*
 * this function doesn't need to hold lock due to be called in init.
<<<<<<< HEAD
 * (hpb_lock, rsp_list_lock, etc..)
 */
static int ufshpb_init_pinned_active_region(struct ufshpb_lu *hpb,
					    struct ufshpb_region *rgn)
{
	struct ufshpb_subregion *srgn;
	int srgn_idx, j;
	int err = 0;

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		srgn = rgn->srgn_tbl + srgn_idx;

		srgn->mctx = ufshpb_get_map_ctx(hpb, &err);
		if (err) {
			ERR_MSG("get mctx err %d srgn %d free_table %d",
				err, srgn_idx, hpb->debug_free_table);
			goto release;
		}

		srgn->srgn_state = HPBSUBREGION_ISSUED;
		ufshpb_clean_dirty_bitmap(hpb, srgn);
		list_add_tail(&srgn->list_act_srgn, &hpb->lh_pinned_srgn);
	}

	rgn->rgn_state = HPBREGION_PINNED;

	return 0;

release:
	for (j = 0; j < srgn_idx; j++) {
		srgn = rgn->srgn_tbl + j;
		ufshpb_put_map_ctx(hpb, srgn->mctx);
	}

	return err;
}

static inline bool ufshpb_is_pinned_region(struct ufshpb_lu *hpb, int rgn_idx)
{
	if (hpb->lu_pinned_end_offset != -1 &&
	    rgn_idx >= hpb->lu_pinned_rgn_startidx &&
	    rgn_idx <= hpb->lu_pinned_end_offset)
		return true;

	return false;
}

static inline void ufshpb_init_jobs(struct ufshpb_lu *hpb)
{
	INIT_WORK(&hpb->ufshpb_work, ufshpb_work_handler);
	INIT_DELAYED_WORK(&hpb->ufshpb_retry_work, ufshpb_retry_work_handler);
	INIT_WORK(&hpb->ufshpb_task_workq, ufshpb_task_workq_fn);
}

static inline void ufshpb_cancel_jobs(struct ufshpb_lu *hpb)
{
	cancel_work_sync(&hpb->ufshpb_work);
	cancel_delayed_work_sync(&hpb->ufshpb_retry_work);
	cancel_work_sync(&hpb->ufshpb_task_workq);
}

static void ufshpb_init_subregion_tbl(struct ufshpb_lu *hpb,
				      struct ufshpb_region *rgn)
{
	int srgn_idx;

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		struct ufshpb_subregion *srgn = rgn->srgn_tbl + srgn_idx;

=======
 * (rgn_state_lock, rsp_list_lock, etc..)
 */
static int ufshpb_init_pinned_active_region(struct ufs_hba *hba,
					    struct ufshpb_lu *hpb,
					    struct ufshpb_region *rgn)
{
	struct ufshpb_subregion *srgn;
	int srgn_idx, i;
	int err = 0;

	for_each_sub_region(rgn, srgn_idx, srgn) {
		srgn->mctx = ufshpb_get_map_ctx(hpb, srgn->is_last);
		srgn->srgn_state = HPB_SRGN_INVALID;
		if (!srgn->mctx) {
			err = -ENOMEM;
			dev_err(hba->dev,
				"alloc mctx for pinned region failed\n");
			goto release;
		}

		list_add_tail(&srgn->list_act_srgn, &hpb->lh_act_srgn);
	}

	rgn->rgn_state = HPB_RGN_PINNED;
	return 0;

release:
	for (i = 0; i < srgn_idx; i++) {
		srgn = rgn->srgn_tbl + i;
		ufshpb_put_map_ctx(hpb, srgn->mctx);
	}
	return err;
}

static void ufshpb_init_subregion_tbl(struct ufshpb_lu *hpb,
				      struct ufshpb_region *rgn, bool last)
{
	int srgn_idx;
	struct ufshpb_subregion *srgn;

	for_each_sub_region(rgn, srgn_idx, srgn) {
>>>>>>> upstream/android-13
		INIT_LIST_HEAD(&srgn->list_act_srgn);

		srgn->rgn_idx = rgn->rgn_idx;
		srgn->srgn_idx = srgn_idx;
<<<<<<< HEAD
		srgn->srgn_state = HPBSUBREGION_UNUSED;
	}
}

static inline int ufshpb_alloc_subregion_tbl(struct ufshpb_lu *hpb,
					     struct ufshpb_region *rgn,
					     int srgn_cnt)
{
	rgn->srgn_tbl =
		kzalloc(sizeof(struct ufshpb_subregion) * srgn_cnt, GFP_KERNEL);
=======
		srgn->srgn_state = HPB_SRGN_UNUSED;
	}

	if (unlikely(last && hpb->last_srgn_entries))
		srgn->is_last = true;
}

static int ufshpb_alloc_subregion_tbl(struct ufshpb_lu *hpb,
				      struct ufshpb_region *rgn, int srgn_cnt)
{
	rgn->srgn_tbl = kvcalloc(srgn_cnt, sizeof(struct ufshpb_subregion),
				 GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!rgn->srgn_tbl)
		return -ENOMEM;

	rgn->srgn_cnt = srgn_cnt;
	return 0;
}

<<<<<<< HEAD
static int ufshpb_table_mempool_init(struct ufshpb_lu *hpb)
{
	struct ufshpb_map_ctx *mctx = NULL;
	int i, j, k;

	INIT_LIST_HEAD(&hpb->lh_map_ctx_free);

	hpb->alloc_mctx = hpb->lu_max_active_rgns * hpb->srgns_per_rgn;

	for (i = 0; i < hpb->alloc_mctx; i++) {
		mctx = kmalloc(sizeof(struct ufshpb_map_ctx), GFP_KERNEL);
		if (!mctx)
			goto release_mem;

		mctx->m_page =
			kzalloc(sizeof(struct page *) * hpb->mpages_per_srgn,
				GFP_KERNEL);
		if (!mctx->m_page)
			goto release_mem;

		mctx->ppn_dirty =
			vzalloc(hpb->entries_per_srgn >> bits_per_byte_shift);
		if (!mctx->ppn_dirty)
			goto release_mem;

		for (j = 0; j < hpb->mpages_per_srgn; j++) {
			mctx->m_page[j] = alloc_page(GFP_KERNEL | __GFP_ZERO);
			if (!mctx->m_page[j]) {
				for (k = 0; k < j; k++)
					kfree(mctx->m_page[k]);
				goto release_mem;
			}
		}

		INIT_LIST_HEAD(&mctx->list_table);
		list_add(&mctx->list_table, &hpb->lh_map_ctx_free);

		hpb->debug_free_table++;
	}

	INIT_INFO("The number of mctx = %d. debug_free_table %d",
		  hpb->alloc_mctx, hpb->debug_free_table);
	return 0;
release_mem:
	/*
	 * mctxs already added in lh_map_ctx_free will be removed
	 * in the caller function.
	 */
	if (mctx) {
		kfree(mctx->m_page);
		vfree(mctx->ppn_dirty);
		kfree(mctx);
	}
	return -ENOMEM;
}

static int
ufshpb_map_req_mempool_init(struct ufshpb_lu *hpb)
{
	struct scsi_device *sdev;
	struct request_queue *q;
	struct ufshpb_req *map_req = NULL;
	int qd = hpb->qd;
	int i, j;

	sdev = hpb->ufsf->sdev_ufs_lu[hpb->lun];
	q = sdev->request_queue;

	INIT_LIST_HEAD(&hpb->lh_map_req_free);
	INIT_LIST_HEAD(&hpb->lh_map_req_retry);

	hpb->map_req = kzalloc(sizeof(struct ufshpb_req) * qd, GFP_KERNEL);
	if (!hpb->map_req)
		goto release_mem;

	/*
	 * q->cmd_size: sizeof(struct scsi_cmd) + shost->hostt->cmd_size
	 */
	for (i = 0; i < qd; i++) {
		map_req = hpb->map_req + i;
		INIT_LIST_HEAD(&map_req->list_req);
		map_req->req = kzalloc(sizeof(struct request) + q->cmd_size,
				       GFP_KERNEL);
		if (!map_req->req) {
			for (j = 0; j < i; j++)
				kfree(hpb->map_req[j].req);
			goto release_mem;
		}

		map_req->bio = bio_kmalloc(GFP_KERNEL, hpb->mpages_per_srgn);
		if (!map_req->bio) {
			kfree(hpb->map_req[i].req);
			for (j = 0; j < i; j++) {
				kfree(hpb->map_req[j].req);
				bio_put(hpb->map_req[j].bio);
			}
			goto release_mem;
		}
		list_add_tail(&map_req->list_req, &hpb->lh_map_req_free);
	}

	return 0;
release_mem:
	kfree(hpb->map_req);
	return -ENOMEM;
}

static int
ufshpb_pre_req_mempool_init(struct ufshpb_lu *hpb)
{
	struct scsi_device *sdev;
	struct request_queue *q;
	struct ufshpb_req *pre_req = NULL;
	int qd = hpb->qd;
	int i, j;

	INIT_LIST_HEAD(&hpb->lh_pre_req_free);
	INIT_LIST_HEAD(&hpb->lh_pre_req_dummy);

	sdev = hpb->ufsf->sdev_ufs_lu[hpb->lun];
	q = sdev->request_queue;

	hpb->pre_req = kzalloc(sizeof(struct ufshpb_req) * qd, GFP_KERNEL);
	if (!hpb->pre_req)
		goto release_mem;

	/*
	 * q->cmd_size: sizeof(struct scsi_cmd) + shost->hostt->cmd_size
	 */
	for (i = 0; i < qd; i++) {
		pre_req = hpb->pre_req + i;
		INIT_LIST_HEAD(&pre_req->list_req);
		pre_req->req = kzalloc(sizeof(struct request) + q->cmd_size,
				       GFP_KERNEL);
		if (!pre_req->req) {
			for (j = 0; j < i; j++)
				kfree(hpb->pre_req[j].req);
			goto release_mem;
		}

		pre_req->bio = bio_kmalloc(GFP_KERNEL, 1);
		if (!pre_req->bio) {
			kfree(hpb->pre_req[i].req);
			for (j = 0; j < i; j++) {
				kfree(hpb->pre_req[j].req);
				bio_put(hpb->pre_req[j].bio);
			}
			goto release_mem;
		}

		pre_req->wb.m_page = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!pre_req->wb.m_page) {
			kfree(hpb->pre_req[i].req);
			bio_put(hpb->pre_req[i].bio);
			for (j = 0; j < i; j++) {
				kfree(hpb->pre_req[j].req);
				bio_put(hpb->pre_req[j].bio);
				__free_page(hpb->pre_req[j].wb.m_page);
			}
			goto release_mem;
		}
		list_add_tail(&pre_req->list_req, &hpb->lh_pre_req_free);
	}

	return 0;
release_mem:
	kfree(hpb->pre_req);
	return -ENOMEM;
}

static void ufshpb_find_lu_qd(struct ufshpb_lu *hpb)
{
	struct scsi_device *sdev;
	struct ufs_hba *hba;

	sdev = hpb->ufsf->sdev_ufs_lu[hpb->lun];
	hba = hpb->ufsf->hba;

	/*
	 * ufshcd_slave_alloc(sdev) -> ufshcd_set_queue_depth(sdev)
	 * a lu-queue-depth compared with lu_info and hba->nutrs
	 * is selected in ufshcd_set_queue_depth()
	 */
	hpb->qd = sdev->queue_depth;
	INIT_INFO("lu %d queue_depth %d", hpb->lun, hpb->qd);
	if (!hpb->qd) {
		hpb->qd = hba->nutrs;
		INIT_INFO("lu_queue_depth is 0. we use device's queue info.");
		INIT_INFO("hba->nutrs = %d", hba->nutrs);
	}

	hpb->throttle_map_req = hpb->qd;
	hpb->throttle_pre_req = hpb->qd;
	hpb->num_inflight_map_req = 0;
	hpb->num_inflight_pre_req = 0;
}

static void ufshpb_init_lu_constant(struct ufshpb_dev_info *hpb_dev_info,
				    struct ufshpb_lu *hpb)
{
	unsigned long long rgn_unit_size, rgn_mem_size;
	int entries_per_rgn;

	hpb->debug = false;

	ufshpb_find_lu_qd(hpb);

	/* for pre_req */
	hpb->pre_req_min_tr_len = HPB_MULTI_CHUNK_LOW;
	hpb->pre_req_max_tr_len = HPB_MULTI_CHUNK_HIGH;
	hpb->ctx_id_ticket = 0;

	/*	From descriptors	*/
	rgn_unit_size = (unsigned long long)
		SECTOR * (0x01 << hpb_dev_info->hpb_rgn_size);
	rgn_mem_size = rgn_unit_size / BLOCK * HPB_ENTRY_SIZE;

	hpb->srgn_unit_size = (unsigned long long)
		SECTOR * (0x01 << hpb_dev_info->hpb_srgn_size);
	hpb->srgn_mem_size =
		hpb->srgn_unit_size / BLOCK * HPB_ENTRY_SIZE;

	hpb->hpb_ver = hpb_dev_info->hpb_ver;

	/* relation : lu <-> region <-> sub region <-> entry */
	entries_per_rgn = rgn_mem_size / HPB_ENTRY_SIZE;
	hpb->entries_per_srgn = hpb->srgn_mem_size / HPB_ENTRY_SIZE;
#if BITS_PER_LONG == 32
	hpb->srgns_per_rgn = div_u64(rgn_mem_size, hpb->srgn_mem_size);

	/*
	 * regions_per_lu = (lu_num_blocks * 4096) / region_unit_size
	 *	          = (lu_num_blocks * HPB_ENTRY_SIZE) / region_mem_size
	 */
	hpb->rgns_per_lu =
		div_u64(((unsigned long long)hpb->lu_num_blocks
		 + (rgn_mem_size / HPB_ENTRY_SIZE) - 1),
		 (rgn_mem_size / HPB_ENTRY_SIZE));
	hpb->srgns_per_lu =
		div_u64(((unsigned long long)hpb->lu_num_blocks
		 + (hpb->srgn_mem_size / HPB_ENTRY_SIZE) - 1),
		(hpb->srgn_mem_size / HPB_ENTRY_SIZE));
#else
	hpb->srgns_per_rgn = rgn_mem_size / hpb->srgn_mem_size;

	/*
	 * regions_per_lu = (lu_num_blocks * 4096) / region_unit_size
	 *	          = (lu_num_blocks * HPB_ENTRY_SIZE) / region_mem_size
	 */
	hpb->rgns_per_lu =
		((unsigned long long)hpb->lu_num_blocks
		 + (rgn_mem_size / HPB_ENTRY_SIZE) - 1)
		/ (rgn_mem_size / HPB_ENTRY_SIZE);
	hpb->srgns_per_lu =
		((unsigned long long)hpb->lu_num_blocks
		 + (hpb->srgn_mem_size / HPB_ENTRY_SIZE) - 1)
		/ (hpb->srgn_mem_size / HPB_ENTRY_SIZE);
#endif

	/* mempool info */
	hpb->mpage_bytes = OS_PAGE_SIZE;
	hpb->mpages_per_srgn = hpb->srgn_mem_size / hpb->mpage_bytes;

	/* Bitmask Info. */
	hpb->dwords_per_srgn = hpb->entries_per_srgn / BITS_PER_DWORD;
	hpb->entries_per_rgn_shift = ffs(entries_per_rgn) - 1;
	hpb->entries_per_rgn_mask = entries_per_rgn - 1;
	hpb->entries_per_srgn_shift = ffs(hpb->entries_per_srgn) - 1;
	hpb->entries_per_srgn_mask = hpb->entries_per_srgn - 1;

	INIT_INFO("===== From Device Descriptor! =====");
	INIT_INFO("hpb_region_size = %d, hpb_subregion_size = %d",
		  hpb_dev_info->hpb_rgn_size,
		  hpb_dev_info->hpb_srgn_size);
	INIT_INFO("=====   Constant Values(LU)   =====");
	INIT_INFO("region_unit_size = %lld, region_mem_size %lld",
		  rgn_unit_size, rgn_mem_size);
	INIT_INFO("subregion_unit_size = %lld, subregion_mem_size %d",
		  hpb->srgn_unit_size, hpb->srgn_mem_size);

	INIT_INFO("lu_num_blocks = %d", hpb->lu_num_blocks);
	INIT_INFO("regions_per_lu = %d, subregions_per_lu = %d",
		  hpb->rgns_per_lu, hpb->srgns_per_lu);

	INIT_INFO("subregions_per_region = %d", hpb->srgns_per_rgn);
	INIT_INFO("entries_per_region %u shift %u mask 0x%X",
		  entries_per_rgn, hpb->entries_per_rgn_shift,
		  hpb->entries_per_rgn_mask);
	INIT_INFO("entries_per_subregion %u shift %u mask 0x%X",
		  hpb->entries_per_srgn, hpb->entries_per_srgn_shift,
		  hpb->entries_per_srgn_mask);
	INIT_INFO("mpages_per_subregion : %d", hpb->mpages_per_srgn);
	INIT_INFO("===================================\n");
}

static int ufshpb_lu_hpb_init(struct ufsf_feature *ufsf, u8 lun)
{
	struct ufshpb_lu *hpb = ufsf->ufshpb_lup[lun];
	struct ufshpb_region *rgn_table, *rgn;
	struct ufshpb_subregion *srgn;
	int rgn_idx, srgn_idx, total_srgn_cnt, srgn_cnt, i, ret = 0;
	bool do_work_handler = false;

	ufshpb_init_lu_constant(&ufsf->hpb_dev_info, hpb);

	rgn_table = kzalloc(sizeof(struct ufshpb_region) * hpb->rgns_per_lu,
			    GFP_KERNEL);
	if (!rgn_table) {
		ret = -ENOMEM;
		goto out;
	}

	INIT_INFO("active_region_table bytes: %lu",
		  (sizeof(struct ufshpb_region) * hpb->rgns_per_lu));

	hpb->rgn_tbl = rgn_table;

	spin_lock_init(&hpb->hpb_lock);
	spin_lock_init(&hpb->rsp_list_lock);

	/* init lru information */
	INIT_LIST_HEAD(&hpb->lru_info.lh_lru_rgn);
	hpb->lru_info.selection_type = LRU;

	INIT_LIST_HEAD(&hpb->lh_pinned_srgn);
	INIT_LIST_HEAD(&hpb->lh_act_srgn);
	INIT_LIST_HEAD(&hpb->lh_inact_rgn);

	INIT_LIST_HEAD(&hpb->lh_map_ctx_free);

	ufshpb_init_jobs(hpb);

	ret = ufshpb_map_req_mempool_init(hpb);
	if (ret) {
		ERR_MSG("map_req_mempool init fail!");
		goto release_rgn_table;
	}

	ret = ufshpb_pre_req_mempool_init(hpb);
	if (ret) {
		ERR_MSG("pre_req_mempool init fail!");
		goto release_map_req_mempool;
	}

	ret = ufshpb_table_mempool_init(hpb);
	if (ret) {
		ERR_MSG("ppn table mempool init fail!");
		ufshpb_table_mempool_remove(hpb);
		goto release_pre_req_mempool;
	}

	total_srgn_cnt = hpb->srgns_per_lu;
	INIT_INFO("total_subregion_count: %d", total_srgn_cnt);
	for (rgn_idx = 0, srgn_cnt = 0; rgn_idx < hpb->rgns_per_lu;
	     rgn_idx++, total_srgn_cnt -= srgn_cnt) {
		rgn = rgn_table + rgn_idx;
		rgn->rgn_idx = rgn_idx;

		INIT_LIST_HEAD(&rgn->list_inact_rgn);

		/* init lru region information*/
		INIT_LIST_HEAD(&rgn->list_lru_rgn);

		srgn_cnt = min(total_srgn_cnt, hpb->srgns_per_rgn);

		ret = ufshpb_alloc_subregion_tbl(hpb, rgn, srgn_cnt);
		if (ret)
			goto release_srgns;
		ufshpb_init_subregion_tbl(hpb, rgn);

		if (ufshpb_is_pinned_region(hpb, rgn_idx)) {
			ret = ufshpb_init_pinned_active_region(hpb, rgn);
			if (ret)
				goto release_srgns;

			do_work_handler = true;
		} else {
			rgn->rgn_state = HPBREGION_INACTIVE;
		}
	}

	if (total_srgn_cnt != 0) {
		ERR_MSG("error total_subregion_count: %d",
			total_srgn_cnt);
		goto release_srgns;
	}

	if (do_work_handler)
		schedule_work(&hpb->ufshpb_work);

	/*
	 * even if creating sysfs failed, ufshpb could run normally.
	 * so we don't deal with error handling
	 */
	ufshpb_create_sysfs(ufsf, hpb);

	return 0;
release_srgns:
	for (i = 0; i < rgn_idx; i++) {
		rgn = rgn_table + i;
		if (rgn->srgn_tbl) {
			for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt;
			     srgn_idx++) {
				srgn = rgn->srgn_tbl + srgn_idx;
				if (srgn->mctx)
					ufshpb_put_map_ctx(hpb, srgn->mctx);
			}
			kfree(rgn->srgn_tbl);
		}
	}

	ufshpb_table_mempool_remove(hpb);
release_pre_req_mempool:
	ufshpb_pre_req_mempool_remove(hpb);
release_map_req_mempool:
	ufshpb_map_req_mempool_remove(hpb);
release_rgn_table:
	kfree(rgn_table);
out:
	return ret;
}

static inline int ufshpb_version_check(struct ufshpb_dev_info *hpb_dev_info)
{
	if (hpb_dev_info->hpb_ver < UFSHPB_VER) {
		INIT_INFO("Driver = %.2x %.2x, Device = %.2x %.2x",
			  GET_BYTE_1(UFSHPB_VER), GET_BYTE_0(UFSHPB_VER),
			  GET_BYTE_1(hpb_dev_info->hpb_ver),
			  GET_BYTE_0(hpb_dev_info->hpb_ver));
		return -ENODEV;
	}
	return 0;
}

void ufshpb_get_dev_info(struct ufshpb_dev_info *hpb_dev_info, u8 *desc_buf)
{
	int ret;

	hpb_dev_info->hpb_device = false;

	if (desc_buf[DEVICE_DESC_PARAM_FEAT_SUP] & UFS_FEATURE_SUPPORT_HPB_BIT)
		INIT_INFO("bUFSFeaturesSupport: HPB is set");
	else {
		INIT_INFO("bUFSFeaturesSupport: HPB not support");
		return;
	}

	hpb_dev_info->hpb_ver = LI_EN_16(desc_buf + DEVICE_DESC_PARAM_HPB_VER);

	ret = ufshpb_version_check(hpb_dev_info);
	if (!ret)
		hpb_dev_info->hpb_device = true;
}

void ufshpb_get_geo_info(struct ufshpb_dev_info *hpb_dev_info, u8 *geo_buf)
{
	hpb_dev_info->hpb_number_lu = geo_buf[GEOMETRY_DESC_HPB_NUMBER_LU];
	if (hpb_dev_info->hpb_number_lu == 0) {
		ERR_MSG("Don't have a lu for hpb.");
		hpb_dev_info->hpb_device = false;
		return;
	}

	hpb_dev_info->hpb_rgn_size = geo_buf[GEOMETRY_DESC_HPB_REGION_SIZE];
	hpb_dev_info->hpb_srgn_size = geo_buf[GEOMETRY_DESC_HPB_SUBREGION_SIZE];
	hpb_dev_info->hpb_device_max_active_rgns =
		LI_EN_16(geo_buf + GEOMETRY_DESC_HPB_DEVICE_MAX_ACTIVE_REGIONS);

	INIT_INFO("[48] bHPBRegionSiz %u", hpb_dev_info->hpb_rgn_size);
	INIT_INFO("[49] bHPBNumberLU %u", hpb_dev_info->hpb_number_lu);
	INIT_INFO("[4A] bHPBSubRegionSize %u", hpb_dev_info->hpb_srgn_size);
	INIT_INFO("[4B:4C] wDeviceMaxActiveHPBRegions %u",
		  hpb_dev_info->hpb_device_max_active_rgns);

	ufshpb_init_constant();
}

int ufshpb_get_lu_info(struct ufsf_feature *ufsf, u8 lun, u8 *unit_buf)
{
	struct ufsf_lu_desc lu_desc;
	struct ufshpb_lu *hpb;

	lu_desc.lu_enable = unit_buf[UNIT_DESC_PARAM_LU_ENABLE];
	lu_desc.lu_queue_depth = unit_buf[UNIT_DESC_PARAM_LU_Q_DEPTH];
	lu_desc.lu_logblk_size = unit_buf[UNIT_DESC_PARAM_LOGICAL_BLK_SIZE];
	lu_desc.lu_logblk_cnt =
		LI_EN_64(unit_buf + UNIT_DESC_PARAM_LOGICAL_BLK_COUNT);
	lu_desc.lu_max_active_hpb_rgns =
		LI_EN_16(unit_buf + UNIT_DESC_HPB_LU_MAX_ACTIVE_REGIONS);
	lu_desc.lu_hpb_pinned_rgn_startidx =
		LI_EN_16(unit_buf + UNIT_DESC_HPB_LU_PIN_REGION_START_OFFSET);
	lu_desc.lu_num_hpb_pinned_rgns =
		LI_EN_16(unit_buf + UNIT_DESC_HPB_LU_NUM_PIN_REGIONS);

	if (lu_desc.lu_num_hpb_pinned_rgns > 0) {
		lu_desc.lu_hpb_pinned_end_offset =
			lu_desc.lu_hpb_pinned_rgn_startidx +
			lu_desc.lu_num_hpb_pinned_rgns - 1;
	} else
		lu_desc.lu_hpb_pinned_end_offset = -1;

	INIT_INFO("LUN(%d) [0A] bLogicalBlockSize %d",
		  lun, lu_desc.lu_logblk_size);
	INIT_INFO("LUN(%d) [0B] qLogicalBlockCount %llu",
		  lun, lu_desc.lu_logblk_cnt);
	INIT_INFO("LUN(%d) [03] bLuEnable %d", lun, lu_desc.lu_enable);
	INIT_INFO("LUN(%d) [06] bLuQueueDepth %d", lun, lu_desc.lu_queue_depth);
	INIT_INFO("LUN(%d) [23:24] wLUMaxActiveHPBRegions %d",
		  lun, lu_desc.lu_max_active_hpb_rgns);
	INIT_INFO("LUN(%d) [25:26] wHPBPinnedRegionStartIdx %d",
		  lun, lu_desc.lu_hpb_pinned_rgn_startidx);
	INIT_INFO("LUN(%d) [27:28] wNumHPBPinnedRegions %d",
		  lun, lu_desc.lu_num_hpb_pinned_rgns);
	INIT_INFO("LUN(%d) PINNED Start %d End %d",
		  lun, lu_desc.lu_hpb_pinned_rgn_startidx,
		  lu_desc.lu_hpb_pinned_end_offset);

	ufsf->ufshpb_lup[lun] = NULL;

	if (lu_desc.lu_enable == 0x02) {
		ufsf->ufshpb_lup[lun] = kzalloc(sizeof(struct ufshpb_lu),
						GFP_KERNEL);
		if (!ufsf->ufshpb_lup[lun])
			return -ENOMEM;

		hpb = ufsf->ufshpb_lup[lun];
		hpb->ufsf = ufsf;
		hpb->lun = lun;
		hpb->lu_num_blocks = lu_desc.lu_logblk_cnt;
		hpb->lu_max_active_rgns = lu_desc.lu_max_active_hpb_rgns;
		hpb->lru_info.max_lru_active_cnt =
			lu_desc.lu_max_active_hpb_rgns -
			lu_desc.lu_num_hpb_pinned_rgns;
		hpb->lu_pinned_rgn_startidx =
			lu_desc.lu_hpb_pinned_rgn_startidx;
		hpb->lu_pinned_end_offset = lu_desc.lu_hpb_pinned_end_offset;
	} else {
		INIT_INFO("===== LU %d is hpb-disabled.", lun);
		return -ENODEV;
	}

	return 0;
}

static void ufshpb_error_handler(struct work_struct *work)
{
	struct ufsf_feature *ufsf;

	ufsf = container_of(work, struct ufsf_feature, ufshpb_eh_work);

	WARNING_MSG("driver has failed. but UFSHCD can run without UFSHPB");
	WARNING_MSG("UFSHPB will be removed from the kernel");

	ufshpb_release(ufsf, HPB_FAILED);
}

static int ufshpb_init(struct ufsf_feature *ufsf)
{
	int lun, ret;
	int hpb_enabled_lun = 0;

	seq_scan_lu(lun) {
		if (!ufsf->ufshpb_lup[lun])
			continue;

		/*
		 * HPB need info about request queue in order to issue
		 * RB-CMD for pinned region.
		 */
		if (!ufsf->sdev_ufs_lu[lun]) {
			WARNING_MSG("warn: lun %d don't have scsi_device", lun);
			continue;
		}

		ret = ufshpb_lu_hpb_init(ufsf, lun);
		if (ret) {
			if (ret == -ENODEV)
				continue;
			else
				goto out_free_mem;
		}
		hpb_enabled_lun++;
	}

	if (hpb_enabled_lun == 0) {
		ERR_MSG("No UFSHPB LU to init");
		ret = -ENODEV;
		goto out_free_mem;
	}

	INIT_WORK(&ufsf->ufshpb_reset_work, ufshpb_reset_handler);
	INIT_WORK(&ufsf->ufshpb_eh_work, ufshpb_error_handler);

	kref_init(&ufsf->ufshpb_kref);
	ufsf->ufshpb_state = HPB_PRESENT;
	ufsf->issue_ioctl = false;

	seq_scan_lu(lun)
		if (ufsf->ufshpb_lup[lun])
			INFO_MSG("UFSHPB LU %d working", lun);

	return 0;
out_free_mem:
	seq_scan_lu(lun)
		kfree(ufsf->ufshpb_lup[lun]);

	ufsf->ufshpb_state = HPB_NOT_SUPPORTED;
	return ret;
}

static void ufshpb_purge_active_region(struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	unsigned long flags;
	int rgn_idx, srgn_idx, state;

	RELEASE_INFO("Start");
	spin_lock_irqsave(&hpb->hpb_lock, flags);
	for (rgn_idx = 0; rgn_idx < hpb->rgns_per_lu; rgn_idx++) {
		rgn = hpb->rgn_tbl + rgn_idx;

		if (rgn->rgn_state == HPBREGION_INACTIVE)
			continue;

		if (rgn->rgn_state == HPBREGION_PINNED)
			state = HPBSUBREGION_DIRTY;
		else if (rgn->rgn_state == HPBREGION_ACTIVE) {
			state = HPBSUBREGION_UNUSED;
			ufshpb_cleanup_lru_info(&hpb->lru_info, rgn);
		} else
			continue;

		for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
			srgn = rgn->srgn_tbl + srgn_idx;

			ufshpb_purge_active_subregion(hpb, srgn, state);
		}
	}
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);

	RELEASE_INFO("END");
}

static void ufshpb_drop_retry_list(struct ufshpb_lu *hpb)
{
	struct ufshpb_req *map_req, *next;
	unsigned long flags;

	if (list_empty(&hpb->lh_map_req_retry))
		return;

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	list_for_each_entry_safe(map_req, next, &hpb->lh_map_req_retry,
				 list_req) {
		INFO_MSG("drop map_req %p ( %d - %d )", map_req,
			 map_req->rb.rgn_idx, map_req->rb.srgn_idx);

		list_del_init(&map_req->list_req);

		ufshpb_put_map_req(hpb, map_req);
	}
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
}

static void ufshpb_drop_rsp_lists(struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn, *next_rgn;
	struct ufshpb_subregion *srgn, *next_srgn;
	unsigned long flags;

	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	list_for_each_entry_safe(rgn, next_rgn, &hpb->lh_inact_rgn,
				 list_inact_rgn) {
		list_del_init(&rgn->list_inact_rgn);
	}

	list_for_each_entry_safe(srgn, next_srgn, &hpb->lh_act_srgn,
				 list_act_srgn) {
		list_del_init(&srgn->list_act_srgn);
	}
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
}

=======
static void ufshpb_lu_parameter_init(struct ufs_hba *hba,
				     struct ufshpb_lu *hpb,
				     struct ufshpb_dev_info *hpb_dev_info,
				     struct ufshpb_lu_info *hpb_lu_info)
{
	u32 entries_per_rgn;
	u64 rgn_mem_size, tmp;

	if (ufshpb_is_legacy(hba))
		hpb->pre_req_max_tr_len = HPB_LEGACY_CHUNK_HIGH;
	else
		hpb->pre_req_max_tr_len = hpb_dev_info->max_hpb_single_cmd;

	hpb->lu_pinned_start = hpb_lu_info->pinned_start;
	hpb->lu_pinned_end = hpb_lu_info->num_pinned ?
		(hpb_lu_info->pinned_start + hpb_lu_info->num_pinned - 1)
		: PINNED_NOT_SET;
	hpb->lru_info.max_lru_active_cnt =
		hpb_lu_info->max_active_rgns - hpb_lu_info->num_pinned;

	rgn_mem_size = (1ULL << hpb_dev_info->rgn_size) * HPB_RGN_SIZE_UNIT
			* HPB_ENTRY_SIZE;
	do_div(rgn_mem_size, HPB_ENTRY_BLOCK_SIZE);
	hpb->srgn_mem_size = (1ULL << hpb_dev_info->srgn_size)
		* HPB_RGN_SIZE_UNIT / HPB_ENTRY_BLOCK_SIZE * HPB_ENTRY_SIZE;

	tmp = rgn_mem_size;
	do_div(tmp, HPB_ENTRY_SIZE);
	entries_per_rgn = (u32)tmp;
	hpb->entries_per_rgn_shift = ilog2(entries_per_rgn);
	hpb->entries_per_rgn_mask = entries_per_rgn - 1;

	hpb->entries_per_srgn = hpb->srgn_mem_size / HPB_ENTRY_SIZE;
	hpb->entries_per_srgn_shift = ilog2(hpb->entries_per_srgn);
	hpb->entries_per_srgn_mask = hpb->entries_per_srgn - 1;

	tmp = rgn_mem_size;
	do_div(tmp, hpb->srgn_mem_size);
	hpb->srgns_per_rgn = (int)tmp;

	hpb->rgns_per_lu = DIV_ROUND_UP(hpb_lu_info->num_blocks,
				entries_per_rgn);
	hpb->srgns_per_lu = DIV_ROUND_UP(hpb_lu_info->num_blocks,
				(hpb->srgn_mem_size / HPB_ENTRY_SIZE));
	hpb->last_srgn_entries = hpb_lu_info->num_blocks
				 % (hpb->srgn_mem_size / HPB_ENTRY_SIZE);

	hpb->pages_per_srgn = DIV_ROUND_UP(hpb->srgn_mem_size, PAGE_SIZE);

	if (hpb_dev_info->control_mode == HPB_HOST_CONTROL)
		hpb->is_hcm = true;
}

static int ufshpb_alloc_region_tbl(struct ufs_hba *hba, struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn_table, *rgn;
	int rgn_idx, i;
	int ret = 0;

	rgn_table = kvcalloc(hpb->rgns_per_lu, sizeof(struct ufshpb_region),
			    GFP_KERNEL);
	if (!rgn_table)
		return -ENOMEM;

	for (rgn_idx = 0; rgn_idx < hpb->rgns_per_lu; rgn_idx++) {
		int srgn_cnt = hpb->srgns_per_rgn;
		bool last_srgn = false;

		rgn = rgn_table + rgn_idx;
		rgn->rgn_idx = rgn_idx;

		spin_lock_init(&rgn->rgn_lock);

		INIT_LIST_HEAD(&rgn->list_inact_rgn);
		INIT_LIST_HEAD(&rgn->list_lru_rgn);
		INIT_LIST_HEAD(&rgn->list_expired_rgn);

		if (rgn_idx == hpb->rgns_per_lu - 1) {
			srgn_cnt = ((hpb->srgns_per_lu - 1) %
				    hpb->srgns_per_rgn) + 1;
			last_srgn = true;
		}

		ret = ufshpb_alloc_subregion_tbl(hpb, rgn, srgn_cnt);
		if (ret)
			goto release_srgn_table;
		ufshpb_init_subregion_tbl(hpb, rgn, last_srgn);

		if (ufshpb_is_pinned_region(hpb, rgn_idx)) {
			ret = ufshpb_init_pinned_active_region(hba, hpb, rgn);
			if (ret)
				goto release_srgn_table;
		} else {
			rgn->rgn_state = HPB_RGN_INACTIVE;
		}

		rgn->rgn_flags = 0;
		rgn->hpb = hpb;
	}

	hpb->rgn_tbl = rgn_table;

	return 0;

release_srgn_table:
	for (i = 0; i <= rgn_idx; i++)
		kvfree(rgn_table[i].srgn_tbl);

	kvfree(rgn_table);
	return ret;
}

>>>>>>> upstream/android-13
static void ufshpb_destroy_subregion_tbl(struct ufshpb_lu *hpb,
					 struct ufshpb_region *rgn)
{
	int srgn_idx;
<<<<<<< HEAD

	for (srgn_idx = 0; srgn_idx < rgn->srgn_cnt; srgn_idx++) {
		struct ufshpb_subregion *srgn;

		srgn = rgn->srgn_tbl + srgn_idx;
		srgn->srgn_state = HPBSUBREGION_UNUSED;

		ufshpb_put_map_ctx(hpb, srgn->mctx);
	}

	kfree(rgn->srgn_tbl);
=======
	struct ufshpb_subregion *srgn;

	for_each_sub_region(rgn, srgn_idx, srgn)
		if (srgn->srgn_state != HPB_SRGN_UNUSED) {
			srgn->srgn_state = HPB_SRGN_UNUSED;
			ufshpb_put_map_ctx(hpb, srgn->mctx);
		}
>>>>>>> upstream/android-13
}

static void ufshpb_destroy_region_tbl(struct ufshpb_lu *hpb)
{
	int rgn_idx;

<<<<<<< HEAD
	RELEASE_INFO("Start");

=======
>>>>>>> upstream/android-13
	for (rgn_idx = 0; rgn_idx < hpb->rgns_per_lu; rgn_idx++) {
		struct ufshpb_region *rgn;

		rgn = hpb->rgn_tbl + rgn_idx;
<<<<<<< HEAD
		if (rgn->rgn_state == HPBREGION_PINNED ||
		    rgn->rgn_state == HPBREGION_ACTIVE) {
			rgn->rgn_state = HPBREGION_INACTIVE;

			ufshpb_destroy_subregion_tbl(hpb, rgn);
		}
	}

	ufshpb_table_mempool_remove(hpb);
	kfree(hpb->rgn_tbl);

	RELEASE_INFO("End");
}

void ufshpb_release(struct ufsf_feature *ufsf, int state)
{
	struct ufshpb_lu *hpb;
	int lun;

	RELEASE_INFO("start release");
	ufsf->ufshpb_state = HPB_FAILED;

	RELEASE_INFO("kref count %d",
		     atomic_read(&ufsf->ufshpb_kref.refcount.refs));

	seq_scan_lu(lun) {
		hpb = ufsf->ufshpb_lup[lun];

		RELEASE_INFO("lun %d %p", lun, hpb);

		ufsf->ufshpb_lup[lun] = NULL;

		if (!hpb)
			continue;

		ufshpb_cancel_jobs(hpb);

		ufshpb_destroy_region_tbl(hpb);
		if (hpb->alloc_mctx != 0)
			WARNING_MSG("warning: alloc_mctx %d", hpb->alloc_mctx);

		ufshpb_map_req_mempool_remove(hpb);

		ufshpb_pre_req_mempool_remove(hpb);

		ufshpb_remove_sysfs(hpb);

		kfree(hpb);
	}

	ufsf->ufshpb_state = state;

	RELEASE_INFO("end release");
}

static void ufshpb_reset(struct ufsf_feature *ufsf)
{
	struct ufshpb_lu *hpb;
	int lun;

	seq_scan_lu(lun) {
		hpb = ufsf->ufshpb_lup[lun];
		if (hpb) {
			INFO_MSG("UFSHPB lun %d reset", lun);
			ufshpb_cancel_jobs(hpb);
			ufshpb_drop_retry_list(hpb);
			ufshpb_drop_rsp_lists(hpb);
			ufshpb_purge_active_region(hpb);
		}
	}

	ufsf->ufshpb_state = HPB_PRESENT;
}

static inline int ufshpb_wait_kref_init_value(struct ufsf_feature *ufsf)
{
	return (atomic_read(&ufsf->ufshpb_kref.refcount.refs) == 1);
}

void ufshpb_reset_handler(struct work_struct *work)
{
	struct ufsf_feature *ufsf;
	int ret;

	ufsf = container_of(work, struct ufsf_feature, ufshpb_reset_work);

	init_waitqueue_head(&ufsf->wait_hpb);

	ret = wait_event_timeout(ufsf->wait_hpb,
				 ufshpb_wait_kref_init_value(ufsf),
				 msecs_to_jiffies(15000));
	if (ret == 0)
		ERR_MSG("UFSHPB kref is not init_value(=1). kref count = %d",
			atomic_read(&ufsf->ufshpb_kref.refcount.refs));

	INIT_INFO("HPB_RESET_START");

	ufshpb_reset(ufsf);
}

static inline int ufshpb_probe_lun_done(struct ufsf_feature *ufsf)
{
	return (ufsf->num_lu == ufsf->slave_conf_cnt);
}

void ufshpb_init_handler(struct work_struct *work)
{
	struct ufsf_feature *ufsf;
	int ret;

	ufsf = container_of(work, struct ufsf_feature, ufshpb_init_work);

	init_waitqueue_head(&ufsf->wait_hpb);

	ret = wait_event_timeout(ufsf->wait_hpb,
				 ufshpb_probe_lun_done(ufsf),
				 msecs_to_jiffies(10000));
	if (ret == 0)
		ERR_MSG("Probing LU is not fully complete.");

	INIT_INFO("HPB_INIT_START");

	ret = ufshpb_init(ufsf);
	if (ret)
		ERR_MSG("UFSHPB driver init failed. err %d", ret);
}

void ufshpb_suspend(struct ufsf_feature *ufsf)
{
	struct ufshpb_lu *hpb;
	int lun;

	seq_scan_lu(lun) {
		hpb = ufsf->ufshpb_lup[lun];
		if (hpb) {
			INFO_MSG("ufshpb_lu %d goto suspend", lun);
			ufshpb_cancel_jobs(hpb);
		}
	}
}

void ufshpb_resume(struct ufsf_feature *ufsf)
{
	struct ufshpb_lu *hpb;
	int lun;

	seq_scan_lu(lun) {
		hpb = ufsf->ufshpb_lup[lun];
		if (hpb) {
			bool do_workq = false;
			bool do_retry_work = false;

			do_workq = !ufshpb_is_empty_rsp_lists(hpb);
			do_retry_work =
				!list_empty_careful(&hpb->lh_map_req_retry);

			INFO_MSG("ufshpb_lu %d resume. do_workq %d retry %d",
				 lun, do_workq, do_retry_work);

			if (do_workq)
				schedule_work(&hpb->ufshpb_task_workq);
			if (do_retry_work)
				schedule_delayed_work(&hpb->ufshpb_retry_work,
						      msecs_to_jiffies(100));
		}
	}
=======
		if (rgn->rgn_state != HPB_RGN_INACTIVE) {
			rgn->rgn_state = HPB_RGN_INACTIVE;

			ufshpb_destroy_subregion_tbl(hpb, rgn);
		}

		kvfree(rgn->srgn_tbl);
	}

	kvfree(hpb->rgn_tbl);
}

/* SYSFS functions */
#define ufshpb_sysfs_attr_show_func(__name)				\
static ssize_t __name##_show(struct device *dev,			\
	struct device_attribute *attr, char *buf)			\
{									\
	struct scsi_device *sdev = to_scsi_device(dev);			\
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);		\
									\
	if (!hpb)							\
		return -ENODEV;						\
									\
	return sysfs_emit(buf, "%llu\n", hpb->stats.__name);		\
}									\
\
static DEVICE_ATTR_RO(__name)

ufshpb_sysfs_attr_show_func(hit_cnt);
ufshpb_sysfs_attr_show_func(miss_cnt);
ufshpb_sysfs_attr_show_func(rb_noti_cnt);
ufshpb_sysfs_attr_show_func(rb_active_cnt);
ufshpb_sysfs_attr_show_func(rb_inactive_cnt);
ufshpb_sysfs_attr_show_func(map_req_cnt);
ufshpb_sysfs_attr_show_func(umap_req_cnt);

static struct attribute *hpb_dev_stat_attrs[] = {
	&dev_attr_hit_cnt.attr,
	&dev_attr_miss_cnt.attr,
	&dev_attr_rb_noti_cnt.attr,
	&dev_attr_rb_active_cnt.attr,
	&dev_attr_rb_inactive_cnt.attr,
	&dev_attr_map_req_cnt.attr,
	&dev_attr_umap_req_cnt.attr,
	NULL,
};

struct attribute_group ufs_sysfs_hpb_stat_group = {
	.name = "hpb_stats",
	.attrs = hpb_dev_stat_attrs,
};

/* SYSFS functions */
#define ufshpb_sysfs_param_show_func(__name)				\
static ssize_t __name##_show(struct device *dev,			\
	struct device_attribute *attr, char *buf)			\
{									\
	struct scsi_device *sdev = to_scsi_device(dev);			\
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);		\
									\
	if (!hpb)							\
		return -ENODEV;						\
									\
	return sysfs_emit(buf, "%d\n", hpb->params.__name);		\
}

ufshpb_sysfs_param_show_func(requeue_timeout_ms);
static ssize_t
requeue_timeout_ms_store(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val < 0)
		return -EINVAL;

	hpb->params.requeue_timeout_ms = val;

	return count;
}
static DEVICE_ATTR_RW(requeue_timeout_ms);

ufshpb_sysfs_param_show_func(activation_thld);
static ssize_t
activation_thld_store(struct device *dev, struct device_attribute *attr,
		      const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val <= 0)
		return -EINVAL;

	hpb->params.activation_thld = val;

	return count;
}
static DEVICE_ATTR_RW(activation_thld);

ufshpb_sysfs_param_show_func(normalization_factor);
static ssize_t
normalization_factor_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val <= 0 || val > ilog2(hpb->entries_per_srgn))
		return -EINVAL;

	hpb->params.normalization_factor = val;

	return count;
}
static DEVICE_ATTR_RW(normalization_factor);

ufshpb_sysfs_param_show_func(eviction_thld_enter);
static ssize_t
eviction_thld_enter_store(struct device *dev, struct device_attribute *attr,
			  const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val <= hpb->params.eviction_thld_exit)
		return -EINVAL;

	hpb->params.eviction_thld_enter = val;

	return count;
}
static DEVICE_ATTR_RW(eviction_thld_enter);

ufshpb_sysfs_param_show_func(eviction_thld_exit);
static ssize_t
eviction_thld_exit_store(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val <= hpb->params.activation_thld)
		return -EINVAL;

	hpb->params.eviction_thld_exit = val;

	return count;
}
static DEVICE_ATTR_RW(eviction_thld_exit);

ufshpb_sysfs_param_show_func(read_timeout_ms);
static ssize_t
read_timeout_ms_store(struct device *dev, struct device_attribute *attr,
		      const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	/* read_timeout >> timeout_polling_interval */
	if (val < hpb->params.timeout_polling_interval_ms * 2)
		return -EINVAL;

	hpb->params.read_timeout_ms = val;

	return count;
}
static DEVICE_ATTR_RW(read_timeout_ms);

ufshpb_sysfs_param_show_func(read_timeout_expiries);
static ssize_t
read_timeout_expiries_store(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val <= 0)
		return -EINVAL;

	hpb->params.read_timeout_expiries = val;

	return count;
}
static DEVICE_ATTR_RW(read_timeout_expiries);

ufshpb_sysfs_param_show_func(timeout_polling_interval_ms);
static ssize_t
timeout_polling_interval_ms_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	/* timeout_polling_interval << read_timeout */
	if (val <= 0 || val > hpb->params.read_timeout_ms / 2)
		return -EINVAL;

	hpb->params.timeout_polling_interval_ms = val;

	return count;
}
static DEVICE_ATTR_RW(timeout_polling_interval_ms);

ufshpb_sysfs_param_show_func(inflight_map_req);
static ssize_t inflight_map_req_store(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf, size_t count)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);
	int val;

	if (!hpb)
		return -ENODEV;

	if (!hpb->is_hcm)
		return -EOPNOTSUPP;

	if (kstrtouint(buf, 0, &val))
		return -EINVAL;

	if (val <= 0 || val > hpb->sdev_ufs_lu->queue_depth - 1)
		return -EINVAL;

	hpb->params.inflight_map_req = val;

	return count;
}
static DEVICE_ATTR_RW(inflight_map_req);

static void ufshpb_hcm_param_init(struct ufshpb_lu *hpb)
{
	hpb->params.activation_thld = ACTIVATION_THRESHOLD;
	hpb->params.normalization_factor = 1;
	hpb->params.eviction_thld_enter = (ACTIVATION_THRESHOLD << 5);
	hpb->params.eviction_thld_exit = (ACTIVATION_THRESHOLD << 4);
	hpb->params.read_timeout_ms = READ_TO_MS;
	hpb->params.read_timeout_expiries = READ_TO_EXPIRIES;
	hpb->params.timeout_polling_interval_ms = POLLING_INTERVAL_MS;
	hpb->params.inflight_map_req = THROTTLE_MAP_REQ_DEFAULT;
}

static struct attribute *hpb_dev_param_attrs[] = {
	&dev_attr_requeue_timeout_ms.attr,
	&dev_attr_activation_thld.attr,
	&dev_attr_normalization_factor.attr,
	&dev_attr_eviction_thld_enter.attr,
	&dev_attr_eviction_thld_exit.attr,
	&dev_attr_read_timeout_ms.attr,
	&dev_attr_read_timeout_expiries.attr,
	&dev_attr_timeout_polling_interval_ms.attr,
	&dev_attr_inflight_map_req.attr,
	NULL,
};

struct attribute_group ufs_sysfs_hpb_param_group = {
	.name = "hpb_params",
	.attrs = hpb_dev_param_attrs,
};

static int ufshpb_pre_req_mempool_init(struct ufshpb_lu *hpb)
{
	struct ufshpb_req *pre_req = NULL, *t;
	int qd = hpb->sdev_ufs_lu->queue_depth / 2;
	int i;

	INIT_LIST_HEAD(&hpb->lh_pre_req_free);

	hpb->pre_req = kcalloc(qd, sizeof(struct ufshpb_req), GFP_KERNEL);
	hpb->throttle_pre_req = qd;
	hpb->num_inflight_pre_req = 0;

	if (!hpb->pre_req)
		goto release_mem;

	for (i = 0; i < qd; i++) {
		pre_req = hpb->pre_req + i;
		INIT_LIST_HEAD(&pre_req->list_req);
		pre_req->req = NULL;

		pre_req->bio = bio_alloc(GFP_KERNEL, 1);
		if (!pre_req->bio)
			goto release_mem;

		pre_req->wb.m_page = alloc_page(GFP_KERNEL | __GFP_ZERO);
		if (!pre_req->wb.m_page) {
			bio_put(pre_req->bio);
			goto release_mem;
		}

		list_add_tail(&pre_req->list_req, &hpb->lh_pre_req_free);
	}

	return 0;
release_mem:
	list_for_each_entry_safe(pre_req, t, &hpb->lh_pre_req_free, list_req) {
		list_del_init(&pre_req->list_req);
		bio_put(pre_req->bio);
		__free_page(pre_req->wb.m_page);
	}

	kfree(hpb->pre_req);
	return -ENOMEM;
}

static void ufshpb_pre_req_mempool_destroy(struct ufshpb_lu *hpb)
{
	struct ufshpb_req *pre_req = NULL;
	int i;

	for (i = 0; i < hpb->throttle_pre_req; i++) {
		pre_req = hpb->pre_req + i;
		bio_put(hpb->pre_req[i].bio);
		if (!pre_req->wb.m_page)
			__free_page(hpb->pre_req[i].wb.m_page);
		list_del_init(&pre_req->list_req);
	}

	kfree(hpb->pre_req);
>>>>>>> upstream/android-13
}

static void ufshpb_stat_init(struct ufshpb_lu *hpb)
{
<<<<<<< HEAD
	atomic64_set(&hpb->hit, 0);
	atomic64_set(&hpb->miss, 0);
	atomic64_set(&hpb->rb_noti_cnt, 0);
	atomic64_set(&hpb->rb_active_cnt, 0);
	atomic64_set(&hpb->rb_inactive_cnt, 0);
	atomic64_set(&hpb->map_req_cnt, 0);
	atomic64_set(&hpb->pre_req_cnt, 0);
}

/* SYSFS functions */
static ssize_t ufshpb_sysfs_prep_disable_show(struct ufshpb_lu *hpb, char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "force_hpb_read_disable %d\n",
		       hpb->force_disable);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_prep_disable_store(struct ufshpb_lu *hpb,
					       const char *buf, size_t cnt)
{
	unsigned long value = 0;

	if (kstrtoul(buf, 0, &value))
		return -EINVAL;

	if (value > 1)
		return -EINVAL;

	if (value == 1)
		hpb->force_disable = true;
	else if (value == 0)
		hpb->force_disable = false;

	SYSFS_INFO("force_hpb_read_disable %d", hpb->force_disable);

	return cnt;
}

static ssize_t ufshpb_sysfs_map_disable_show(struct ufshpb_lu *hpb, char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "force_map_req_disable %d\n",
		       hpb->force_map_req_disable);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_map_disable_store(struct ufshpb_lu *hpb,
					      const char *buf, size_t cnt)
{
	unsigned long value = 0;

	if (kstrtoul(buf, 0, &value))
		return -EINVAL;

	if (value > 1)
		return -EINVAL;

	if (value == 1)
		hpb->force_map_req_disable = true;
	else if (value == 0)
		hpb->force_map_req_disable = false;

	SYSFS_INFO("force_map_req_disable %d", hpb->force_map_req_disable);

	return cnt;
}

static ssize_t ufshpb_sysfs_throttle_map_req_show(struct ufshpb_lu *hpb,
						  char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "throttle_map_req %d\n",
		       hpb->throttle_map_req);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_throttle_map_req_store(struct ufshpb_lu *hpb,
						   const char *buf, size_t cnt)
{
	unsigned long throttle_map_req = 0;

	if (kstrtoul(buf, 0, &throttle_map_req))
		return -EINVAL;

	hpb->throttle_map_req = (int)throttle_map_req;

	SYSFS_INFO("throttle_map_req %d", hpb->throttle_map_req);

	return cnt;
}

static ssize_t ufshpb_sysfs_throttle_pre_req_show(struct ufshpb_lu *hpb,
						  char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "throttle_pre_req %d\n",
		       hpb->throttle_pre_req);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_throttle_pre_req_store(struct ufshpb_lu *hpb,
						   const char *buf, size_t cnt)
{
	unsigned long throttle_pre_req = 0;

	if (kstrtoul(buf, 0, &throttle_pre_req))
		return -EINVAL;

	hpb->throttle_pre_req = (int)throttle_pre_req;

	SYSFS_INFO("throttle_pre_req %d", hpb->throttle_pre_req);

	return cnt;
}

static ssize_t ufshpb_sysfs_pre_req_min_tr_len_show(struct ufshpb_lu *hpb,
						    char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "%d", hpb->pre_req_min_tr_len);

	if (ret < 0)
		return ret;

	SYSFS_INFO("pre_req min transfer len %d", hpb->pre_req_min_tr_len);

	return ret;
}

static ssize_t ufshpb_sysfs_pre_req_min_tr_len_store(struct ufshpb_lu *hpb,
						     const char *buf,
						     size_t count)
{
	unsigned long val = 0;

	if (kstrtoul(buf, 0, &val))
		return -EINVAL;

	if (val < 0)
		val = 0;

	if (hpb->pre_req_max_tr_len < val || val < HPB_MULTI_CHUNK_LOW)
		SYSFS_INFO("value is wrong. pre_req transfer len %d ~ %d\n",
			   HPB_MULTI_CHUNK_LOW, hpb->pre_req_max_tr_len);
	else
		hpb->pre_req_min_tr_len = val;

	SYSFS_INFO("pre_req min transfer len %d", hpb->pre_req_min_tr_len);

	return count;
}

static ssize_t ufshpb_sysfs_pre_req_max_tr_len_show(struct ufshpb_lu *hpb,
						    char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "%d", hpb->pre_req_max_tr_len);

	if (ret < 0)
		return ret;

	SYSFS_INFO("pre_req max transfer len %d", hpb->pre_req_max_tr_len);

	return ret;
}

static ssize_t ufshpb_sysfs_pre_req_max_tr_len_store(struct ufshpb_lu *hpb,
						     const char *buf,
						     size_t count)
{
	unsigned long val = 0;

	if (kstrtoul(buf, 0, &val))
		return -EINVAL;

	if (hpb->pre_req_min_tr_len > val || val > HPB_MULTI_CHUNK_HIGH)
		SYSFS_INFO("value is wrong. pre_req transfer len %d ~ %d\n",
			   hpb->pre_req_min_tr_len, HPB_MULTI_CHUNK_HIGH);
	else
		hpb->pre_req_max_tr_len = val;

	SYSFS_INFO("pre_req max transfer len %d", hpb->pre_req_max_tr_len);

	return count;
}

static ssize_t ufshpb_sysfs_debug_show(struct ufshpb_lu *hpb, char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE, "debug %d\n", hpb->debug);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_debug_store(struct ufshpb_lu *hpb,
					const char *buf, size_t cnt)
{
	unsigned long debug = 0;

	if (kstrtoul(buf, 0, &debug))
		return -EINVAL;

	if (debug >= 1)
		hpb->debug = 1;
	else
		hpb->debug = 0;

	SYSFS_INFO("debug %d", hpb->debug);

	return cnt;
}

static ssize_t ufshpb_sysfs_version_show(struct ufshpb_lu *hpb, char *buf)
{
	int ret;

	ret = snprintf(buf, PAGE_SIZE,
		       "HPB version %.2x %.2x D/D version %.2x %.2x\n",
		       GET_BYTE_1(hpb->hpb_ver), GET_BYTE_0(hpb->hpb_ver),
		       GET_BYTE_1(UFSHPB_DD_VER), GET_BYTE_0(UFSHPB_DD_VER));

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_hit_show(struct ufshpb_lu *hpb, char *buf)
{
	long long hit_cnt;
	int ret;

	hit_cnt = atomic64_read(&hpb->hit);

	ret = snprintf(buf, PAGE_SIZE, "hit_count %lld\n", hit_cnt);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_miss_show(struct ufshpb_lu *hpb, char *buf)
{
	long long miss_cnt;
	int ret;

	miss_cnt = atomic64_read(&hpb->miss);

	ret = snprintf(buf, PAGE_SIZE, "miss_count %lld\n", miss_cnt);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_map_req_show(struct ufshpb_lu *hpb, char *buf)
{
	long long rb_noti_cnt, rb_active_cnt, rb_inactive_cnt, map_req_cnt;
	int ret;

	rb_noti_cnt = atomic64_read(&hpb->rb_noti_cnt);
	rb_active_cnt = atomic64_read(&hpb->rb_active_cnt);
	rb_inactive_cnt = atomic64_read(&hpb->rb_inactive_cnt);
	map_req_cnt = atomic64_read(&hpb->map_req_cnt);

	ret = snprintf(buf, PAGE_SIZE,
		       "rb_noti %lld ACT %lld INACT %lld map_req_count %lld\n",
		       rb_noti_cnt, rb_active_cnt, rb_inactive_cnt,
		       map_req_cnt);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_pre_req_show(struct ufshpb_lu *hpb, char *buf)
{
	long long pre_req_cnt;
	int ret;

	pre_req_cnt = atomic64_read(&hpb->pre_req_cnt);

	ret = snprintf(buf, PAGE_SIZE, "pre_req_count %lld\n", pre_req_cnt);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_region_stat_show(struct ufshpb_lu *hpb, char *buf)
{
	int ret, pin_cnt = 0, act_cnt = 0, inact_cnt = 0, rgn_idx;
	enum HPBREGION_STATE state;

	for (rgn_idx = 0; rgn_idx < hpb->rgns_per_lu; rgn_idx++) {
		state = hpb->rgn_tbl[rgn_idx].rgn_state;
		if (state == HPBREGION_PINNED)
			pin_cnt++;
		else if (state == HPBREGION_ACTIVE)
			act_cnt++;
		else if (state == HPBREGION_INACTIVE)
			inact_cnt++;
	}

	ret = snprintf(buf, PAGE_SIZE,
		       "Total %d pinned %d active %d inactive %d\n",
		       hpb->rgns_per_lu, pin_cnt, act_cnt, inact_cnt);

	if (ret < 0)
		return ret;

	SYSFS_INFO("%s", buf);

	return ret;
}

static ssize_t ufshpb_sysfs_count_reset_store(struct ufshpb_lu *hpb,
					      const char *buf, size_t cnt)
{
	unsigned long debug;

	if (kstrtoul(buf, 0, &debug))
		return -EINVAL;

	SYSFS_INFO("Stat Init");

	ufshpb_stat_init(hpb);

	return cnt;
}

static ssize_t ufshpb_sysfs_info_lba_store(struct ufshpb_lu *hpb,
					   const char *buf, size_t cnt)
{
	struct ufshpb_region *rgn;
	struct ufshpb_subregion *srgn;
	unsigned long long ppn = 0;
	unsigned long value = 0, lpn, flags;
	int rgn_idx = 0, srgn_idx = 0, srgn_offset = 0, error = 0;

	if (kstrtoul(buf, 0, &value)) {
		ERR_MSG("kstrtoul error");
		return -EINVAL;
	}

	if (value > hpb->lu_num_blocks * SECTORS_PER_BLOCK) {
		ERR_MSG("value %lu > lu_num_blocks %d error",
			value, hpb->lu_num_blocks);
		return -EINVAL;
	}

	lpn = value / SECTORS_PER_BLOCK;

	ufshpb_get_pos_from_lpn(hpb, lpn, &rgn_idx, &srgn_idx, &srgn_offset);

	rgn = hpb->rgn_tbl + rgn_idx;
	srgn = rgn->srgn_tbl + srgn_idx;

	spin_lock_irqsave(&hpb->hpb_lock, flags);
	SYSFS_INFO("lba %lu lpn %lu region %d state %d subregion %d state %d",
		   value, lpn, rgn_idx, rgn->rgn_state, srgn_idx,
		   srgn->srgn_state);

	if (!ufshpb_valid_srgn(rgn, srgn)) {
		SYSFS_INFO("[region %d subregion %d] has not valid hpb info.",
			   rgn_idx, srgn_idx);
		goto out;
	}

	if (!srgn->mctx) {
		SYSFS_INFO("mctx is NULL");
		goto out;
	}

	ppn = ufshpb_get_ppn(srgn->mctx, srgn_offset, &error);
	if (error) {
		SYSFS_INFO("getting ppn is fail from a page.");
		goto out;
	}

	SYSFS_INFO("ppn %llx is_dirty %d", ppn,
		   ufshpb_ppn_dirty_check(hpb, lpn, 1));
out:
	spin_unlock_irqrestore(&hpb->hpb_lock, flags);
	return cnt;
}

static ssize_t ufshpb_sysfs_info_region_store(struct ufshpb_lu *hpb,
					      const char *buf, size_t cnt)
{
	unsigned long rgn_idx = 0;
	int srgn_idx;

	if (kstrtoul(buf, 0, &rgn_idx))
		return -EINVAL;

	if (rgn_idx >= hpb->rgns_per_lu)
		ERR_MSG("error region %ld max %d", rgn_idx, hpb->rgns_per_lu);
	else {
		SYSFS_INFO("(region state : PINNED=%d ACTIVE=%d INACTIVE=%d)",
			   HPBREGION_PINNED, HPBREGION_ACTIVE,
			   HPBREGION_INACTIVE);

		SYSFS_INFO("region %ld state %d", rgn_idx,
			   hpb->rgn_tbl[rgn_idx].rgn_state);

		for (srgn_idx = 0; srgn_idx < hpb->rgn_tbl[rgn_idx].srgn_cnt;
		     srgn_idx++) {
			SYSFS_INFO("--- subregion %d state %d", srgn_idx,
				   hpb->rgn_tbl[rgn_idx].srgn_tbl[srgn_idx].srgn_state);
		}
	}

	return cnt;
}

static ssize_t ufshpb_sysfs_ufshpb_release_store(struct ufshpb_lu *hpb,
						 const char *buf, size_t cnt)
{
	unsigned long value = 0;

	SYSFS_INFO("start release function");

	if (kstrtoul(buf, 0, &value)) {
		ERR_MSG("kstrtoul error");
		return -EINVAL;
	}

	if (value == 0xab) {
		SYSFS_INFO("magic number %lu release start", value);
		goto err_out;
	} else
		SYSFS_INFO("wrong magic number %lu", value);

	return cnt;
err_out:
	SYSFS_INFO("ref_cnt %d",
		   atomic_read(&hpb->ufsf->ufshpb_kref.refcount.refs));
	ufshpb_failed(hpb, __func__);

	return cnt;
}

static struct ufshpb_sysfs_entry ufshpb_sysfs_entries[] = {
	__ATTR(hpb_read_disable, 0644,
	       ufshpb_sysfs_prep_disable_show, ufshpb_sysfs_prep_disable_store),
	__ATTR(map_cmd_disable, 0644,
	       ufshpb_sysfs_map_disable_show, ufshpb_sysfs_map_disable_store),
	__ATTR(throttle_map_req, 0644,
	       ufshpb_sysfs_throttle_map_req_show,
	       ufshpb_sysfs_throttle_map_req_store),
	__ATTR(throttle_pre_req, 0644,
	       ufshpb_sysfs_throttle_pre_req_show,
	       ufshpb_sysfs_throttle_pre_req_store),
	__ATTR(pre_req_min_tr_len, 0644,
	       ufshpb_sysfs_pre_req_min_tr_len_show,
	       ufshpb_sysfs_pre_req_min_tr_len_store),
	__ATTR(pre_req_max_tr_len, 0644,
	       ufshpb_sysfs_pre_req_max_tr_len_show,
	       ufshpb_sysfs_pre_req_max_tr_len_store),
	__ATTR(debug, 0644,
	       ufshpb_sysfs_debug_show, ufshpb_sysfs_debug_store),
	__ATTR(hpb_version, 0444, ufshpb_sysfs_version_show, NULL),
	__ATTR(hit_count, 0444, ufshpb_sysfs_hit_show, NULL),
	__ATTR(miss_count, 0444, ufshpb_sysfs_miss_show, NULL),
	__ATTR(map_req_count, 0444, ufshpb_sysfs_map_req_show, NULL),
	__ATTR(pre_req_count, 0444, ufshpb_sysfs_pre_req_show, NULL),
	__ATTR(region_stat_count, 0444, ufshpb_sysfs_region_stat_show, NULL),
	__ATTR(count_reset, 0200, NULL, ufshpb_sysfs_count_reset_store),
	__ATTR(get_info_from_lba, 0200, NULL, ufshpb_sysfs_info_lba_store),
	__ATTR(get_info_from_region, 0200, NULL,
	       ufshpb_sysfs_info_region_store),
	__ATTR(release, 0200, NULL, ufshpb_sysfs_ufshpb_release_store),
	__ATTR_NULL
};

static ssize_t ufshpb_attr_show(struct kobject *kobj, struct attribute *attr,
				char *page)
{
	struct ufshpb_sysfs_entry *entry;
	struct ufshpb_lu *hpb;
	ssize_t error;

	entry = container_of(attr, struct ufshpb_sysfs_entry, attr);
	hpb = container_of(kobj, struct ufshpb_lu, kobj);

	if (!entry->show)
		return -EIO;

	mutex_lock(&hpb->sysfs_lock);
	error = entry->show(hpb, page);
	mutex_unlock(&hpb->sysfs_lock);
	return error;
}

static ssize_t ufshpb_attr_store(struct kobject *kobj, struct attribute *attr,
				 const char *page, size_t len)
{
	struct ufshpb_sysfs_entry *entry;
	struct ufshpb_lu *hpb;
	ssize_t error;

	entry = container_of(attr, struct ufshpb_sysfs_entry, attr);
	hpb = container_of(kobj, struct ufshpb_lu, kobj);

	if (!entry->store)
		return -EIO;

	mutex_lock(&hpb->sysfs_lock);
	error = entry->store(hpb, page, len);
	mutex_unlock(&hpb->sysfs_lock);
	return error;
}

static const struct sysfs_ops ufshpb_sysfs_ops = {
	.show = ufshpb_attr_show,
	.store = ufshpb_attr_store,
};

static struct kobj_type ufshpb_ktype = {
	.sysfs_ops = &ufshpb_sysfs_ops,
	.release = NULL,
};

static int ufshpb_create_sysfs(struct ufsf_feature *ufsf, struct ufshpb_lu *hpb)
{
	struct device *dev = ufsf->hba->dev;
	struct ufshpb_sysfs_entry *entry;
	int err;

	hpb->sysfs_entries = ufshpb_sysfs_entries;

	ufshpb_stat_init(hpb);

	kobject_init(&hpb->kobj, &ufshpb_ktype);
	mutex_init(&hpb->sysfs_lock);

	INIT_INFO("ufshpb creates sysfs lu %d %p dev->kobj %p", hpb->lun,
		  &hpb->kobj, &dev->kobj);

	err = kobject_add(&hpb->kobj, kobject_get(&dev->kobj),
			  "ufshpb_lu%d", hpb->lun);
	if (!err) {
		for (entry = hpb->sysfs_entries; entry->attr.name != NULL;
		     entry++) {
			INIT_INFO("ufshpb_lu%d sysfs attr creates: %s",
				  hpb->lun, entry->attr.name);
			if (sysfs_create_file(&hpb->kobj, &entry->attr))
				break;
		}
		INIT_INFO("ufshpb_lu%d sysfs adds uevent", hpb->lun);
		kobject_uevent(&hpb->kobj, KOBJ_ADD);
	}

	return err;
}

static int ufshpb_remove_sysfs(struct ufshpb_lu *hpb)
{
	struct ufshpb_sysfs_entry *entry;

	for (entry = hpb->sysfs_entries; entry->attr.name != NULL;
	     entry++) {
		INIT_INFO("ufshpb_lu%d sysfs attr removes: %s",
			  hpb->lun, entry->attr.name);
		sysfs_remove_file(&hpb->kobj, &entry->attr);
	}
	kobject_uevent(&hpb->kobj, KOBJ_REMOVE);

	INIT_INFO("ufshpb removes sysfs lu %d %p ", hpb->lun, &hpb->kobj);
	kobject_del(&hpb->kobj);

	return 0;
}
=======
	hpb->stats.hit_cnt = 0;
	hpb->stats.miss_cnt = 0;
	hpb->stats.rb_noti_cnt = 0;
	hpb->stats.rb_active_cnt = 0;
	hpb->stats.rb_inactive_cnt = 0;
	hpb->stats.map_req_cnt = 0;
	hpb->stats.umap_req_cnt = 0;
}

static void ufshpb_param_init(struct ufshpb_lu *hpb)
{
	hpb->params.requeue_timeout_ms = HPB_REQUEUE_TIME_MS;
	if (hpb->is_hcm)
		ufshpb_hcm_param_init(hpb);
}

static int ufshpb_lu_hpb_init(struct ufs_hba *hba, struct ufshpb_lu *hpb)
{
	int ret;

	spin_lock_init(&hpb->rgn_state_lock);
	spin_lock_init(&hpb->rsp_list_lock);
	spin_lock_init(&hpb->param_lock);

	INIT_LIST_HEAD(&hpb->lru_info.lh_lru_rgn);
	INIT_LIST_HEAD(&hpb->lh_act_srgn);
	INIT_LIST_HEAD(&hpb->lh_inact_rgn);
	INIT_LIST_HEAD(&hpb->list_hpb_lu);

	INIT_WORK(&hpb->map_work, ufshpb_map_work_handler);
	if (hpb->is_hcm) {
		INIT_WORK(&hpb->ufshpb_normalization_work,
			  ufshpb_normalization_work_handler);
		INIT_DELAYED_WORK(&hpb->ufshpb_read_to_work,
				  ufshpb_read_to_handler);
	}

	hpb->map_req_cache = kmem_cache_create("ufshpb_req_cache",
			  sizeof(struct ufshpb_req), 0, 0, NULL);
	if (!hpb->map_req_cache) {
		dev_err(hba->dev, "ufshpb(%d) ufshpb_req_cache create fail",
			hpb->lun);
		return -ENOMEM;
	}

	hpb->m_page_cache = kmem_cache_create("ufshpb_m_page_cache",
			  sizeof(struct page *) * hpb->pages_per_srgn,
			  0, 0, NULL);
	if (!hpb->m_page_cache) {
		dev_err(hba->dev, "ufshpb(%d) ufshpb_m_page_cache create fail",
			hpb->lun);
		ret = -ENOMEM;
		goto release_req_cache;
	}

	ret = ufshpb_pre_req_mempool_init(hpb);
	if (ret) {
		dev_err(hba->dev, "ufshpb(%d) pre_req_mempool init fail",
			hpb->lun);
		goto release_m_page_cache;
	}

	ret = ufshpb_alloc_region_tbl(hba, hpb);
	if (ret)
		goto release_pre_req_mempool;

	ufshpb_stat_init(hpb);
	ufshpb_param_init(hpb);

	if (hpb->is_hcm) {
		unsigned int poll;

		poll = hpb->params.timeout_polling_interval_ms;
		schedule_delayed_work(&hpb->ufshpb_read_to_work,
				      msecs_to_jiffies(poll));
	}

	return 0;

release_pre_req_mempool:
	ufshpb_pre_req_mempool_destroy(hpb);
release_m_page_cache:
	kmem_cache_destroy(hpb->m_page_cache);
release_req_cache:
	kmem_cache_destroy(hpb->map_req_cache);
	return ret;
}

static struct ufshpb_lu *
ufshpb_alloc_hpb_lu(struct ufs_hba *hba, struct scsi_device *sdev,
		    struct ufshpb_dev_info *hpb_dev_info,
		    struct ufshpb_lu_info *hpb_lu_info)
{
	struct ufshpb_lu *hpb;
	int ret;

	hpb = kzalloc(sizeof(struct ufshpb_lu), GFP_KERNEL);
	if (!hpb)
		return NULL;

	hpb->lun = sdev->lun;
	hpb->sdev_ufs_lu = sdev;

	ufshpb_lu_parameter_init(hba, hpb, hpb_dev_info, hpb_lu_info);

	ret = ufshpb_lu_hpb_init(hba, hpb);
	if (ret) {
		dev_err(hba->dev, "hpb lu init failed. ret %d", ret);
		goto release_hpb;
	}

	sdev->hostdata = hpb;
	return hpb;

release_hpb:
	kfree(hpb);
	return NULL;
}

static void ufshpb_discard_rsp_lists(struct ufshpb_lu *hpb)
{
	struct ufshpb_region *rgn, *next_rgn;
	struct ufshpb_subregion *srgn, *next_srgn;
	unsigned long flags;

	/*
	 * If the device reset occurred, the remaining HPB region information
	 * may be stale. Therefore, by discarding the lists of HPB response
	 * that remained after reset, we prevent unnecessary work.
	 */
	spin_lock_irqsave(&hpb->rsp_list_lock, flags);
	list_for_each_entry_safe(rgn, next_rgn, &hpb->lh_inact_rgn,
				 list_inact_rgn)
		list_del_init(&rgn->list_inact_rgn);

	list_for_each_entry_safe(srgn, next_srgn, &hpb->lh_act_srgn,
				 list_act_srgn)
		list_del_init(&srgn->list_act_srgn);
	spin_unlock_irqrestore(&hpb->rsp_list_lock, flags);
}

static void ufshpb_cancel_jobs(struct ufshpb_lu *hpb)
{
	if (hpb->is_hcm) {
		cancel_delayed_work_sync(&hpb->ufshpb_read_to_work);
		cancel_work_sync(&hpb->ufshpb_normalization_work);
	}
	cancel_work_sync(&hpb->map_work);
}

static bool ufshpb_check_hpb_reset_query(struct ufs_hba *hba)
{
	int err = 0;
	bool flag_res = true;
	int try;

	/* wait for the device to complete HPB reset query */
	for (try = 0; try < HPB_RESET_REQ_RETRIES; try++) {
		dev_dbg(hba->dev,
			"%s start flag reset polling %d times\n",
			__func__, try);

		/* Poll fHpbReset flag to be cleared */
		err = ufshcd_query_flag(hba, UPIU_QUERY_OPCODE_READ_FLAG,
				QUERY_FLAG_IDN_HPB_RESET, 0, &flag_res);

		if (err) {
			dev_err(hba->dev,
				"%s reading fHpbReset flag failed with error %d\n",
				__func__, err);
			return flag_res;
		}

		if (!flag_res)
			goto out;

		usleep_range(1000, 1100);
	}
	if (flag_res) {
		dev_err(hba->dev,
			"%s fHpbReset was not cleared by the device\n",
			__func__);
	}
out:
	return flag_res;
}

void ufshpb_reset(struct ufs_hba *hba)
{
	struct ufshpb_lu *hpb;
	struct scsi_device *sdev;

	shost_for_each_device(sdev, hba->host) {
		hpb = ufshpb_get_hpb_data(sdev);
		if (!hpb)
			continue;

		if (ufshpb_get_state(hpb) != HPB_RESET)
			continue;

		ufshpb_set_state(hpb, HPB_PRESENT);
	}
}

void ufshpb_reset_host(struct ufs_hba *hba)
{
	struct ufshpb_lu *hpb;
	struct scsi_device *sdev;

	shost_for_each_device(sdev, hba->host) {
		hpb = ufshpb_get_hpb_data(sdev);
		if (!hpb)
			continue;

		if (ufshpb_get_state(hpb) != HPB_PRESENT)
			continue;
		ufshpb_set_state(hpb, HPB_RESET);
		ufshpb_cancel_jobs(hpb);
		ufshpb_discard_rsp_lists(hpb);
	}
}

void ufshpb_suspend(struct ufs_hba *hba)
{
	struct ufshpb_lu *hpb;
	struct scsi_device *sdev;

	shost_for_each_device(sdev, hba->host) {
		hpb = ufshpb_get_hpb_data(sdev);
		if (!hpb)
			continue;

		if (ufshpb_get_state(hpb) != HPB_PRESENT)
			continue;
		ufshpb_set_state(hpb, HPB_SUSPEND);
		ufshpb_cancel_jobs(hpb);
	}
}

void ufshpb_resume(struct ufs_hba *hba)
{
	struct ufshpb_lu *hpb;
	struct scsi_device *sdev;

	shost_for_each_device(sdev, hba->host) {
		hpb = ufshpb_get_hpb_data(sdev);
		if (!hpb)
			continue;

		if ((ufshpb_get_state(hpb) != HPB_PRESENT) &&
		    (ufshpb_get_state(hpb) != HPB_SUSPEND))
			continue;
		ufshpb_set_state(hpb, HPB_PRESENT);
		ufshpb_kick_map_work(hpb);
		if (hpb->is_hcm) {
			unsigned int poll =
				hpb->params.timeout_polling_interval_ms;

			schedule_delayed_work(&hpb->ufshpb_read_to_work,
				msecs_to_jiffies(poll));
		}
	}
}

static int ufshpb_get_lu_info(struct ufs_hba *hba, int lun,
			      struct ufshpb_lu_info *hpb_lu_info)
{
	u16 max_active_rgns;
	u8 lu_enable;
	int size;
	int ret;
	char desc_buf[QUERY_DESC_MAX_SIZE];

	ufshcd_map_desc_id_to_length(hba, QUERY_DESC_IDN_UNIT, &size);

	ufshcd_rpm_get_sync(hba);
	ret = ufshcd_query_descriptor_retry(hba, UPIU_QUERY_OPCODE_READ_DESC,
					    QUERY_DESC_IDN_UNIT, lun, 0,
					    desc_buf, &size);
	ufshcd_rpm_put_sync(hba);

	if (ret) {
		dev_err(hba->dev,
			"%s: idn: %d lun: %d  query request failed",
			__func__, QUERY_DESC_IDN_UNIT, lun);
		return ret;
	}

	lu_enable = desc_buf[UNIT_DESC_PARAM_LU_ENABLE];
	if (lu_enable != LU_ENABLED_HPB_FUNC)
		return -ENODEV;

	max_active_rgns = get_unaligned_be16(
			desc_buf + UNIT_DESC_PARAM_HPB_LU_MAX_ACTIVE_RGNS);
	if (!max_active_rgns) {
		dev_err(hba->dev,
			"lun %d wrong number of max active regions\n", lun);
		return -ENODEV;
	}

	hpb_lu_info->num_blocks = get_unaligned_be64(
			desc_buf + UNIT_DESC_PARAM_LOGICAL_BLK_COUNT);
	hpb_lu_info->pinned_start = get_unaligned_be16(
			desc_buf + UNIT_DESC_PARAM_HPB_PIN_RGN_START_OFF);
	hpb_lu_info->num_pinned = get_unaligned_be16(
			desc_buf + UNIT_DESC_PARAM_HPB_NUM_PIN_RGNS);
	hpb_lu_info->max_active_rgns = max_active_rgns;

	return 0;
}

void ufshpb_destroy_lu(struct ufs_hba *hba, struct scsi_device *sdev)
{
	struct ufshpb_lu *hpb = ufshpb_get_hpb_data(sdev);

	if (!hpb)
		return;

	ufshpb_set_state(hpb, HPB_FAILED);

	sdev = hpb->sdev_ufs_lu;
	sdev->hostdata = NULL;

	ufshpb_cancel_jobs(hpb);

	ufshpb_pre_req_mempool_destroy(hpb);
	ufshpb_destroy_region_tbl(hpb);

	kmem_cache_destroy(hpb->map_req_cache);
	kmem_cache_destroy(hpb->m_page_cache);

	list_del_init(&hpb->list_hpb_lu);

	kfree(hpb);
}

static void ufshpb_hpb_lu_prepared(struct ufs_hba *hba)
{
	int pool_size;
	struct ufshpb_lu *hpb;
	struct scsi_device *sdev;
	bool init_success;

	if (tot_active_srgn_pages == 0) {
		ufshpb_remove(hba);
		return;
	}

	init_success = !ufshpb_check_hpb_reset_query(hba);

	pool_size = PAGE_ALIGN(ufshpb_host_map_kbytes * 1024) / PAGE_SIZE;
	if (pool_size > tot_active_srgn_pages) {
		mempool_resize(ufshpb_mctx_pool, tot_active_srgn_pages);
		mempool_resize(ufshpb_page_pool, tot_active_srgn_pages);
	}

	shost_for_each_device(sdev, hba->host) {
		hpb = ufshpb_get_hpb_data(sdev);
		if (!hpb)
			continue;

		if (init_success) {
			ufshpb_set_state(hpb, HPB_PRESENT);
			if ((hpb->lu_pinned_end - hpb->lu_pinned_start) > 0)
				queue_work(ufshpb_wq, &hpb->map_work);
			if (!hpb->is_hcm)
				ufshpb_issue_umap_all_req(hpb);
		} else {
			dev_err(hba->dev, "destroy HPB lu %d\n", hpb->lun);
			ufshpb_destroy_lu(hba, sdev);
		}
	}

	if (!init_success)
		ufshpb_remove(hba);
}

void ufshpb_init_hpb_lu(struct ufs_hba *hba, struct scsi_device *sdev)
{
	struct ufshpb_lu *hpb;
	int ret;
	struct ufshpb_lu_info hpb_lu_info = { 0 };
	int lun = sdev->lun;

	if (lun >= hba->dev_info.max_lu_supported)
		goto out;

	ret = ufshpb_get_lu_info(hba, lun, &hpb_lu_info);
	if (ret)
		goto out;

	hpb = ufshpb_alloc_hpb_lu(hba, sdev, &hba->ufshpb_dev,
				  &hpb_lu_info);
	if (!hpb)
		goto out;

	tot_active_srgn_pages += hpb_lu_info.max_active_rgns *
			hpb->srgns_per_rgn * hpb->pages_per_srgn;

out:
	/* All LUs are initialized */
	if (atomic_dec_and_test(&hba->ufshpb_dev.slave_conf_cnt))
		ufshpb_hpb_lu_prepared(hba);
}

static int ufshpb_init_mem_wq(struct ufs_hba *hba)
{
	int ret;
	unsigned int pool_size;

	ufshpb_mctx_cache = kmem_cache_create("ufshpb_mctx_cache",
					sizeof(struct ufshpb_map_ctx),
					0, 0, NULL);
	if (!ufshpb_mctx_cache) {
		dev_err(hba->dev, "ufshpb: cannot init mctx cache\n");
		return -ENOMEM;
	}

	pool_size = PAGE_ALIGN(ufshpb_host_map_kbytes * 1024) / PAGE_SIZE;
	dev_info(hba->dev, "%s:%d ufshpb_host_map_kbytes %u pool_size %u\n",
	       __func__, __LINE__, ufshpb_host_map_kbytes, pool_size);

	ufshpb_mctx_pool = mempool_create_slab_pool(pool_size,
						    ufshpb_mctx_cache);
	if (!ufshpb_mctx_pool) {
		dev_err(hba->dev, "ufshpb: cannot init mctx pool\n");
		ret = -ENOMEM;
		goto release_mctx_cache;
	}

	ufshpb_page_pool = mempool_create_page_pool(pool_size, 0);
	if (!ufshpb_page_pool) {
		dev_err(hba->dev, "ufshpb: cannot init page pool\n");
		ret = -ENOMEM;
		goto release_mctx_pool;
	}

	ufshpb_wq = alloc_workqueue("ufshpb-wq",
					WQ_UNBOUND | WQ_MEM_RECLAIM, 0);
	if (!ufshpb_wq) {
		dev_err(hba->dev, "ufshpb: alloc workqueue failed\n");
		ret = -ENOMEM;
		goto release_page_pool;
	}

	return 0;

release_page_pool:
	mempool_destroy(ufshpb_page_pool);
release_mctx_pool:
	mempool_destroy(ufshpb_mctx_pool);
release_mctx_cache:
	kmem_cache_destroy(ufshpb_mctx_cache);
	return ret;
}

void ufshpb_get_geo_info(struct ufs_hba *hba, u8 *geo_buf)
{
	struct ufshpb_dev_info *hpb_info = &hba->ufshpb_dev;
	int max_active_rgns = 0;
	int hpb_num_lu;

	hpb_num_lu = geo_buf[GEOMETRY_DESC_PARAM_HPB_NUMBER_LU];
	if (hpb_num_lu == 0) {
		dev_err(hba->dev, "No HPB LU supported\n");
		hpb_info->hpb_disabled = true;
		return;
	}

	hpb_info->rgn_size = geo_buf[GEOMETRY_DESC_PARAM_HPB_REGION_SIZE];
	hpb_info->srgn_size = geo_buf[GEOMETRY_DESC_PARAM_HPB_SUBREGION_SIZE];
	max_active_rgns = get_unaligned_be16(geo_buf +
			  GEOMETRY_DESC_PARAM_HPB_MAX_ACTIVE_REGS);

	if (hpb_info->rgn_size == 0 || hpb_info->srgn_size == 0 ||
	    max_active_rgns == 0) {
		dev_err(hba->dev, "No HPB supported device\n");
		hpb_info->hpb_disabled = true;
		return;
	}
}

void ufshpb_get_dev_info(struct ufs_hba *hba, u8 *desc_buf)
{
	struct ufshpb_dev_info *hpb_dev_info = &hba->ufshpb_dev;
	int version, ret;
	int max_single_cmd;

	hpb_dev_info->control_mode = desc_buf[DEVICE_DESC_PARAM_HPB_CONTROL];

	version = get_unaligned_be16(desc_buf + DEVICE_DESC_PARAM_HPB_VER);
	if ((version != HPB_SUPPORT_VERSION) &&
	    (version != HPB_SUPPORT_LEGACY_VERSION)) {
		dev_err(hba->dev, "%s: HPB %x version is not supported.\n",
			__func__, version);
		hpb_dev_info->hpb_disabled = true;
		return;
	}

	if (version == HPB_SUPPORT_LEGACY_VERSION)
		hpb_dev_info->is_legacy = true;

	/*
	 * Get the number of user logical unit to check whether all
	 * scsi_device finish initialization
	 */
	hpb_dev_info->num_lu = desc_buf[DEVICE_DESC_PARAM_NUM_LU];

	if (hpb_dev_info->is_legacy)
		return;

	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
		QUERY_ATTR_IDN_MAX_HPB_SINGLE_CMD, 0, 0, &max_single_cmd);

	if (ret)
		hpb_dev_info->max_hpb_single_cmd = HPB_LEGACY_CHUNK_HIGH;
	else
		hpb_dev_info->max_hpb_single_cmd = min(max_single_cmd + 1, HPB_MULTI_CHUNK_HIGH);
}

void ufshpb_init(struct ufs_hba *hba)
{
	struct ufshpb_dev_info *hpb_dev_info = &hba->ufshpb_dev;
	int try;
	int ret;

	if (!ufshpb_is_allowed(hba) || !hba->dev_info.hpb_enabled)
		return;

	if (ufshpb_init_mem_wq(hba)) {
		hpb_dev_info->hpb_disabled = true;
		return;
	}

	atomic_set(&hpb_dev_info->slave_conf_cnt, hpb_dev_info->num_lu);
	tot_active_srgn_pages = 0;
	/* issue HPB reset query */
	for (try = 0; try < HPB_RESET_REQ_RETRIES; try++) {
		ret = ufshcd_query_flag(hba, UPIU_QUERY_OPCODE_SET_FLAG,
					QUERY_FLAG_IDN_HPB_RESET, 0, NULL);
		if (!ret)
			break;
	}
}

void ufshpb_remove(struct ufs_hba *hba)
{
	mempool_destroy(ufshpb_page_pool);
	mempool_destroy(ufshpb_mctx_pool);
	kmem_cache_destroy(ufshpb_mctx_cache);

	destroy_workqueue(ufshpb_wq);
}

module_param(ufshpb_host_map_kbytes, uint, 0644);
MODULE_PARM_DESC(ufshpb_host_map_kbytes,
	"ufshpb host mapping memory kilo-bytes for ufshpb memory-pool");
>>>>>>> upstream/android-13
