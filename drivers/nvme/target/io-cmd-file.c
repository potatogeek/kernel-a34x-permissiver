// SPDX-License-Identifier: GPL-2.0
/*
 * NVMe Over Fabrics Target File I/O commands implementation.
 * Copyright (c) 2017-2018 Western Digital Corporation or its
 * affiliates.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/uio.h>
#include <linux/falloc.h>
#include <linux/file.h>
<<<<<<< HEAD
=======
#include <linux/fs.h>
>>>>>>> upstream/android-13
#include "nvmet.h"

#define NVMET_MAX_MPOOL_BVEC		16
#define NVMET_MIN_MPOOL_OBJ		16

<<<<<<< HEAD
=======
int nvmet_file_ns_revalidate(struct nvmet_ns *ns)
{
	struct kstat stat;
	int ret;

	ret = vfs_getattr(&ns->file->f_path, &stat, STATX_SIZE,
			  AT_STATX_FORCE_SYNC);
	if (!ret)
		ns->size = stat.size;
	return ret;
}

>>>>>>> upstream/android-13
void nvmet_file_ns_disable(struct nvmet_ns *ns)
{
	if (ns->file) {
		if (ns->buffered_io)
			flush_workqueue(buffered_io_wq);
		mempool_destroy(ns->bvec_pool);
		ns->bvec_pool = NULL;
		kmem_cache_destroy(ns->bvec_cache);
		ns->bvec_cache = NULL;
		fput(ns->file);
		ns->file = NULL;
	}
}

int nvmet_file_ns_enable(struct nvmet_ns *ns)
{
	int flags = O_RDWR | O_LARGEFILE;
<<<<<<< HEAD
	struct kstat stat;
=======
>>>>>>> upstream/android-13
	int ret;

	if (!ns->buffered_io)
		flags |= O_DIRECT;

	ns->file = filp_open(ns->device_path, flags, 0);
	if (IS_ERR(ns->file)) {
<<<<<<< HEAD
		pr_err("failed to open file %s: (%ld)\n",
				ns->device_path, PTR_ERR(ns->file));
		return PTR_ERR(ns->file);
	}

	ret = vfs_getattr(&ns->file->f_path,
			&stat, STATX_SIZE, AT_STATX_FORCE_SYNC);
	if (ret)
		goto err;

	ns->size = stat.size;
	ns->blksize_shift = file_inode(ns->file)->i_blkbits;
=======
		ret = PTR_ERR(ns->file);
		pr_err("failed to open file %s: (%d)\n",
			ns->device_path, ret);
		ns->file = NULL;
		return ret;
	}

	ret = nvmet_file_ns_revalidate(ns);
	if (ret)
		goto err;

	/*
	 * i_blkbits can be greater than the universally accepted upper bound,
	 * so make sure we export a sane namespace lba_shift.
	 */
	ns->blksize_shift = min_t(u8,
			file_inode(ns->file)->i_blkbits, 12);
>>>>>>> upstream/android-13

	ns->bvec_cache = kmem_cache_create("nvmet-bvec",
			NVMET_MAX_MPOOL_BVEC * sizeof(struct bio_vec),
			0, SLAB_HWCACHE_ALIGN, NULL);
	if (!ns->bvec_cache) {
		ret = -ENOMEM;
		goto err;
	}

	ns->bvec_pool = mempool_create(NVMET_MIN_MPOOL_OBJ, mempool_alloc_slab,
			mempool_free_slab, ns->bvec_cache);

	if (!ns->bvec_pool) {
		ret = -ENOMEM;
		goto err;
	}

	return ret;
err:
	ns->size = 0;
	ns->blksize_shift = 0;
	nvmet_file_ns_disable(ns);
	return ret;
}

<<<<<<< HEAD
static void nvmet_file_init_bvec(struct bio_vec *bv, struct sg_page_iter *iter)
{
	bv->bv_page = sg_page_iter_page(iter);
	bv->bv_offset = iter->sg->offset;
	bv->bv_len = PAGE_SIZE - iter->sg->offset;
}

static ssize_t nvmet_file_submit_bvec(struct nvmet_req *req, loff_t pos,
		unsigned long nr_segs, size_t count)
=======
static void nvmet_file_init_bvec(struct bio_vec *bv, struct scatterlist *sg)
{
	bv->bv_page = sg_page(sg);
	bv->bv_offset = sg->offset;
	bv->bv_len = sg->length;
}

static ssize_t nvmet_file_submit_bvec(struct nvmet_req *req, loff_t pos,
		unsigned long nr_segs, size_t count, int ki_flags)
>>>>>>> upstream/android-13
{
	struct kiocb *iocb = &req->f.iocb;
	ssize_t (*call_iter)(struct kiocb *iocb, struct iov_iter *iter);
	struct iov_iter iter;
<<<<<<< HEAD
	int ki_flags = 0, rw;
	ssize_t ret;

	if (req->cmd->rw.opcode == nvme_cmd_write) {
		if (req->cmd->rw.control & cpu_to_le16(NVME_RW_FUA))
			ki_flags = IOCB_DSYNC;
=======
	int rw;

	if (req->cmd->rw.opcode == nvme_cmd_write) {
		if (req->cmd->rw.control & cpu_to_le16(NVME_RW_FUA))
			ki_flags |= IOCB_DSYNC;
>>>>>>> upstream/android-13
		call_iter = req->ns->file->f_op->write_iter;
		rw = WRITE;
	} else {
		call_iter = req->ns->file->f_op->read_iter;
		rw = READ;
	}

<<<<<<< HEAD
	iov_iter_bvec(&iter, ITER_BVEC | rw, req->f.bvec, nr_segs, count);
=======
	iov_iter_bvec(&iter, rw, req->f.bvec, nr_segs, count);
>>>>>>> upstream/android-13

	iocb->ki_pos = pos;
	iocb->ki_filp = req->ns->file;
	iocb->ki_flags = ki_flags | iocb_flags(req->ns->file);

<<<<<<< HEAD
	ret = call_iter(iocb, &iter);

	if (ret != -EIOCBQUEUED && iocb->ki_complete)
		iocb->ki_complete(iocb, ret, 0);

	return ret;
=======
	return call_iter(iocb, &iter);
>>>>>>> upstream/android-13
}

static void nvmet_file_io_done(struct kiocb *iocb, long ret, long ret2)
{
	struct nvmet_req *req = container_of(iocb, struct nvmet_req, f.iocb);
<<<<<<< HEAD
=======
	u16 status = NVME_SC_SUCCESS;
>>>>>>> upstream/android-13

	if (req->f.bvec != req->inline_bvec) {
		if (likely(req->f.mpool_alloc == false))
			kfree(req->f.bvec);
		else
			mempool_free(req->f.bvec, req->ns->bvec_pool);
	}

<<<<<<< HEAD
	nvmet_req_complete(req, ret != req->data_len ?
			NVME_SC_INTERNAL | NVME_SC_DNR : 0);
}

static void nvmet_file_execute_rw(struct nvmet_req *req)
{
	ssize_t nr_bvec = DIV_ROUND_UP(req->data_len, PAGE_SIZE);
	struct sg_page_iter sg_pg_iter;
=======
	if (unlikely(ret != req->transfer_len))
		status = errno_to_nvme_status(req, ret);
	nvmet_req_complete(req, status);
}

static bool nvmet_file_execute_io(struct nvmet_req *req, int ki_flags)
{
	ssize_t nr_bvec = req->sg_cnt;
>>>>>>> upstream/android-13
	unsigned long bv_cnt = 0;
	bool is_sync = false;
	size_t len = 0, total_len = 0;
	ssize_t ret = 0;
	loff_t pos;
<<<<<<< HEAD
=======
	int i;
	struct scatterlist *sg;

	if (req->f.mpool_alloc && nr_bvec > NVMET_MAX_MPOOL_BVEC)
		is_sync = true;

	pos = le64_to_cpu(req->cmd->rw.slba) << req->ns->blksize_shift;
	if (unlikely(pos + req->transfer_len > req->ns->size)) {
		nvmet_req_complete(req, errno_to_nvme_status(req, -ENOSPC));
		return true;
	}

	memset(&req->f.iocb, 0, sizeof(struct kiocb));
	for_each_sg(req->sg, sg, req->sg_cnt, i) {
		nvmet_file_init_bvec(&req->f.bvec[bv_cnt], sg);
		len += req->f.bvec[bv_cnt].bv_len;
		total_len += req->f.bvec[bv_cnt].bv_len;
		bv_cnt++;

		WARN_ON_ONCE((nr_bvec - 1) < 0);

		if (unlikely(is_sync) &&
		    (nr_bvec - 1 == 0 || bv_cnt == NVMET_MAX_MPOOL_BVEC)) {
			ret = nvmet_file_submit_bvec(req, pos, bv_cnt, len, 0);
			if (ret < 0)
				goto complete;

			pos += len;
			bv_cnt = 0;
			len = 0;
		}
		nr_bvec--;
	}

	if (WARN_ON_ONCE(total_len != req->transfer_len)) {
		ret = -EIO;
		goto complete;
	}

	if (unlikely(is_sync)) {
		ret = total_len;
		goto complete;
	}

	/*
	 * A NULL ki_complete ask for synchronous execution, which we want
	 * for the IOCB_NOWAIT case.
	 */
	if (!(ki_flags & IOCB_NOWAIT))
		req->f.iocb.ki_complete = nvmet_file_io_done;

	ret = nvmet_file_submit_bvec(req, pos, bv_cnt, total_len, ki_flags);

	switch (ret) {
	case -EIOCBQUEUED:
		return true;
	case -EAGAIN:
		if (WARN_ON_ONCE(!(ki_flags & IOCB_NOWAIT)))
			goto complete;
		return false;
	case -EOPNOTSUPP:
		/*
		 * For file systems returning error -EOPNOTSUPP, handle
		 * IOCB_NOWAIT error case separately and retry without
		 * IOCB_NOWAIT.
		 */
		if ((ki_flags & IOCB_NOWAIT))
			return false;
		break;
	}

complete:
	nvmet_file_io_done(&req->f.iocb, ret, 0);
	return true;
}

static void nvmet_file_buffered_io_work(struct work_struct *w)
{
	struct nvmet_req *req = container_of(w, struct nvmet_req, f.work);

	nvmet_file_execute_io(req, 0);
}

static void nvmet_file_submit_buffered_io(struct nvmet_req *req)
{
	INIT_WORK(&req->f.work, nvmet_file_buffered_io_work);
	queue_work(buffered_io_wq, &req->f.work);
}

static void nvmet_file_execute_rw(struct nvmet_req *req)
{
	ssize_t nr_bvec = req->sg_cnt;

	if (!nvmet_check_transfer_len(req, nvmet_rw_data_len(req)))
		return;
>>>>>>> upstream/android-13

	if (!req->sg_cnt || !nr_bvec) {
		nvmet_req_complete(req, 0);
		return;
	}

<<<<<<< HEAD
	pos = le64_to_cpu(req->cmd->rw.slba) << req->ns->blksize_shift;
	if (unlikely(pos + req->data_len > req->ns->size)) {
		nvmet_req_complete(req, NVME_SC_LBA_RANGE | NVME_SC_DNR);
		return;
	}

=======
>>>>>>> upstream/android-13
	if (nr_bvec > NVMET_MAX_INLINE_BIOVEC)
		req->f.bvec = kmalloc_array(nr_bvec, sizeof(struct bio_vec),
				GFP_KERNEL);
	else
		req->f.bvec = req->inline_bvec;

<<<<<<< HEAD
	req->f.mpool_alloc = false;
=======
>>>>>>> upstream/android-13
	if (unlikely(!req->f.bvec)) {
		/* fallback under memory pressure */
		req->f.bvec = mempool_alloc(req->ns->bvec_pool, GFP_KERNEL);
		req->f.mpool_alloc = true;
<<<<<<< HEAD
		if (nr_bvec > NVMET_MAX_MPOOL_BVEC)
			is_sync = true;
	}

	memset(&req->f.iocb, 0, sizeof(struct kiocb));
	for_each_sg_page(req->sg, &sg_pg_iter, req->sg_cnt, 0) {
		nvmet_file_init_bvec(&req->f.bvec[bv_cnt], &sg_pg_iter);
		len += req->f.bvec[bv_cnt].bv_len;
		total_len += req->f.bvec[bv_cnt].bv_len;
		bv_cnt++;

		WARN_ON_ONCE((nr_bvec - 1) < 0);

		if (unlikely(is_sync) &&
		    (nr_bvec - 1 == 0 || bv_cnt == NVMET_MAX_MPOOL_BVEC)) {
			ret = nvmet_file_submit_bvec(req, pos, bv_cnt, len);
			if (ret < 0)
				goto out;
			pos += len;
			bv_cnt = 0;
			len = 0;
		}
		nr_bvec--;
	}

	if (WARN_ON_ONCE(total_len != req->data_len))
		ret = -EIO;
out:
	if (unlikely(is_sync || ret)) {
		nvmet_file_io_done(&req->f.iocb, ret < 0 ? ret : total_len, 0);
		return;
	}
	req->f.iocb.ki_complete = nvmet_file_io_done;
	nvmet_file_submit_bvec(req, pos, bv_cnt, total_len);
}

static void nvmet_file_buffered_io_work(struct work_struct *w)
{
	struct nvmet_req *req = container_of(w, struct nvmet_req, f.work);

	nvmet_file_execute_rw(req);
}

static void nvmet_file_execute_rw_buffered_io(struct nvmet_req *req)
{
	INIT_WORK(&req->f.work, nvmet_file_buffered_io_work);
	queue_work(buffered_io_wq, &req->f.work);
=======
	} else
		req->f.mpool_alloc = false;

	if (req->ns->buffered_io) {
		if (likely(!req->f.mpool_alloc) &&
		    (req->ns->file->f_mode & FMODE_NOWAIT) &&
		    nvmet_file_execute_io(req, IOCB_NOWAIT))
			return;
		nvmet_file_submit_buffered_io(req);
	} else
		nvmet_file_execute_io(req, 0);
>>>>>>> upstream/android-13
}

u16 nvmet_file_flush(struct nvmet_req *req)
{
<<<<<<< HEAD
	if (vfs_fsync(req->ns->file, 1) < 0)
		return NVME_SC_INTERNAL | NVME_SC_DNR;
	return 0;
=======
	return errno_to_nvme_status(req, vfs_fsync(req->ns->file, 1));
>>>>>>> upstream/android-13
}

static void nvmet_file_flush_work(struct work_struct *w)
{
	struct nvmet_req *req = container_of(w, struct nvmet_req, f.work);

	nvmet_req_complete(req, nvmet_file_flush(req));
}

static void nvmet_file_execute_flush(struct nvmet_req *req)
{
<<<<<<< HEAD
=======
	if (!nvmet_check_transfer_len(req, 0))
		return;
>>>>>>> upstream/android-13
	INIT_WORK(&req->f.work, nvmet_file_flush_work);
	schedule_work(&req->f.work);
}

static void nvmet_file_execute_discard(struct nvmet_req *req)
{
	int mode = FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE;
	struct nvme_dsm_range range;
	loff_t offset, len;
<<<<<<< HEAD
	u16 ret;
	int i;

	for (i = 0; i <= le32_to_cpu(req->cmd->dsm.nr); i++) {
		ret = nvmet_copy_from_sgl(req, i * sizeof(range), &range,
					sizeof(range));
		if (ret)
=======
	u16 status = 0;
	int ret;
	int i;

	for (i = 0; i <= le32_to_cpu(req->cmd->dsm.nr); i++) {
		status = nvmet_copy_from_sgl(req, i * sizeof(range), &range,
					sizeof(range));
		if (status)
>>>>>>> upstream/android-13
			break;

		offset = le64_to_cpu(range.slba) << req->ns->blksize_shift;
		len = le32_to_cpu(range.nlb);
		len <<= req->ns->blksize_shift;
		if (offset + len > req->ns->size) {
<<<<<<< HEAD
			ret = NVME_SC_LBA_RANGE | NVME_SC_DNR;
			break;
		}

		if (vfs_fallocate(req->ns->file, mode, offset, len)) {
			ret = NVME_SC_INTERNAL | NVME_SC_DNR;
=======
			req->error_slba = le64_to_cpu(range.slba);
			status = errno_to_nvme_status(req, -ENOSPC);
			break;
		}

		ret = vfs_fallocate(req->ns->file, mode, offset, len);
		if (ret && ret != -EOPNOTSUPP) {
			req->error_slba = le64_to_cpu(range.slba);
			status = errno_to_nvme_status(req, ret);
>>>>>>> upstream/android-13
			break;
		}
	}

<<<<<<< HEAD
	nvmet_req_complete(req, ret);
=======
	nvmet_req_complete(req, status);
>>>>>>> upstream/android-13
}

static void nvmet_file_dsm_work(struct work_struct *w)
{
	struct nvmet_req *req = container_of(w, struct nvmet_req, f.work);

	switch (le32_to_cpu(req->cmd->dsm.attributes)) {
	case NVME_DSMGMT_AD:
		nvmet_file_execute_discard(req);
		return;
	case NVME_DSMGMT_IDR:
	case NVME_DSMGMT_IDW:
	default:
		/* Not supported yet */
		nvmet_req_complete(req, 0);
		return;
	}
}

static void nvmet_file_execute_dsm(struct nvmet_req *req)
{
<<<<<<< HEAD
=======
	if (!nvmet_check_data_len_lte(req, nvmet_dsm_len(req)))
		return;
>>>>>>> upstream/android-13
	INIT_WORK(&req->f.work, nvmet_file_dsm_work);
	schedule_work(&req->f.work);
}

static void nvmet_file_write_zeroes_work(struct work_struct *w)
{
	struct nvmet_req *req = container_of(w, struct nvmet_req, f.work);
	struct nvme_write_zeroes_cmd *write_zeroes = &req->cmd->write_zeroes;
	int mode = FALLOC_FL_ZERO_RANGE | FALLOC_FL_KEEP_SIZE;
	loff_t offset;
	loff_t len;
	int ret;

	offset = le64_to_cpu(write_zeroes->slba) << req->ns->blksize_shift;
	len = (((sector_t)le16_to_cpu(write_zeroes->length) + 1) <<
			req->ns->blksize_shift);

	if (unlikely(offset + len > req->ns->size)) {
<<<<<<< HEAD
		nvmet_req_complete(req, NVME_SC_LBA_RANGE | NVME_SC_DNR);
=======
		nvmet_req_complete(req, errno_to_nvme_status(req, -ENOSPC));
>>>>>>> upstream/android-13
		return;
	}

	ret = vfs_fallocate(req->ns->file, mode, offset, len);
<<<<<<< HEAD
	nvmet_req_complete(req, ret < 0 ? NVME_SC_INTERNAL | NVME_SC_DNR : 0);
=======
	nvmet_req_complete(req, ret < 0 ? errno_to_nvme_status(req, ret) : 0);
>>>>>>> upstream/android-13
}

static void nvmet_file_execute_write_zeroes(struct nvmet_req *req)
{
<<<<<<< HEAD
=======
	if (!nvmet_check_transfer_len(req, 0))
		return;
>>>>>>> upstream/android-13
	INIT_WORK(&req->f.work, nvmet_file_write_zeroes_work);
	schedule_work(&req->f.work);
}

u16 nvmet_file_parse_io_cmd(struct nvmet_req *req)
{
<<<<<<< HEAD
	struct nvme_command *cmd = req->cmd;

	switch (cmd->common.opcode) {
	case nvme_cmd_read:
	case nvme_cmd_write:
		if (req->ns->buffered_io)
			req->execute = nvmet_file_execute_rw_buffered_io;
		else
			req->execute = nvmet_file_execute_rw;
		req->data_len = nvmet_rw_len(req);
		return 0;
	case nvme_cmd_flush:
		req->execute = nvmet_file_execute_flush;
		req->data_len = 0;
		return 0;
	case nvme_cmd_dsm:
		req->execute = nvmet_file_execute_dsm;
		req->data_len = (le32_to_cpu(cmd->dsm.nr) + 1) *
			sizeof(struct nvme_dsm_range);
		return 0;
	case nvme_cmd_write_zeroes:
		req->execute = nvmet_file_execute_write_zeroes;
		req->data_len = 0;
		return 0;
	default:
		pr_err("unhandled cmd for file ns %d on qid %d\n",
				cmd->common.opcode, req->sq->qid);
		return NVME_SC_INVALID_OPCODE | NVME_SC_DNR;
=======
	switch (req->cmd->common.opcode) {
	case nvme_cmd_read:
	case nvme_cmd_write:
		req->execute = nvmet_file_execute_rw;
		return 0;
	case nvme_cmd_flush:
		req->execute = nvmet_file_execute_flush;
		return 0;
	case nvme_cmd_dsm:
		req->execute = nvmet_file_execute_dsm;
		return 0;
	case nvme_cmd_write_zeroes:
		req->execute = nvmet_file_execute_write_zeroes;
		return 0;
	default:
		return nvmet_report_invalid_opcode(req);
>>>>>>> upstream/android-13
	}
}
