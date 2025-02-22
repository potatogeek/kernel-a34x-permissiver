/*
 * Copyright (c) 2013-2015, Mellanox Technologies, Ltd.  All rights reserved.
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

#include <linux/mlx5/driver.h>
#include "wq.h"
#include "mlx5_core.h"

<<<<<<< HEAD
u32 mlx5_wq_cyc_get_size(struct mlx5_wq_cyc *wq)
{
	return (u32)wq->fbc.sz_m1 + 1;
}

u32 mlx5_cqwq_get_size(struct mlx5_cqwq *wq)
{
	return wq->fbc.sz_m1 + 1;
}

u32 mlx5_wq_ll_get_size(struct mlx5_wq_ll *wq)
{
	return (u32)wq->fbc.sz_m1 + 1;
}

static u32 mlx5_wq_cyc_get_byte_size(struct mlx5_wq_cyc *wq)
{
	return mlx5_wq_cyc_get_size(wq) << wq->fbc.log_stride;
}

static u32 mlx5_wq_qp_get_byte_size(struct mlx5_wq_qp *wq)
{
	return mlx5_wq_cyc_get_byte_size(&wq->rq) +
	       mlx5_wq_cyc_get_byte_size(&wq->sq);
}

static u32 mlx5_cqwq_get_byte_size(struct mlx5_cqwq *wq)
{
	return mlx5_cqwq_get_size(wq) << wq->fbc.log_stride;
}

static u32 mlx5_wq_ll_get_byte_size(struct mlx5_wq_ll *wq)
{
	return mlx5_wq_ll_get_size(wq) << wq->fbc.log_stride;
}

=======
>>>>>>> upstream/android-13
int mlx5_wq_cyc_create(struct mlx5_core_dev *mdev, struct mlx5_wq_param *param,
		       void *wqc, struct mlx5_wq_cyc *wq,
		       struct mlx5_wq_ctrl *wq_ctrl)
{
<<<<<<< HEAD
	struct mlx5_frag_buf_ctrl *fbc = &wq->fbc;
	int err;

	mlx5_fill_fbc(MLX5_GET(wq, wqc, log_wq_stride),
		      MLX5_GET(wq, wqc, log_wq_sz),
		      fbc);
	wq->sz    = wq->fbc.sz_m1 + 1;

=======
	u8 log_wq_stride = MLX5_GET(wq, wqc, log_wq_stride);
	u8 log_wq_sz     = MLX5_GET(wq, wqc, log_wq_sz);
	struct mlx5_frag_buf_ctrl *fbc = &wq->fbc;
	int err;

>>>>>>> upstream/android-13
	err = mlx5_db_alloc_node(mdev, &wq_ctrl->db, param->db_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_db_alloc_node() failed, %d\n", err);
		return err;
	}

<<<<<<< HEAD
	err = mlx5_frag_buf_alloc_node(mdev, mlx5_wq_cyc_get_byte_size(wq),
=======
	wq->db  = wq_ctrl->db.db;

	err = mlx5_frag_buf_alloc_node(mdev, wq_get_byte_sz(log_wq_sz, log_wq_stride),
>>>>>>> upstream/android-13
				       &wq_ctrl->buf, param->buf_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_frag_buf_alloc_node() failed, %d\n", err);
		goto err_db_free;
	}

<<<<<<< HEAD
	fbc->frag_buf = wq_ctrl->buf;
	wq->db  = wq_ctrl->db.db;
=======
	mlx5_init_fbc(wq_ctrl->buf.frags, log_wq_stride, log_wq_sz, fbc);
	wq->sz = mlx5_wq_cyc_get_size(wq);
>>>>>>> upstream/android-13

	wq_ctrl->mdev = mdev;

	return 0;

err_db_free:
	mlx5_db_free(mdev, &wq_ctrl->db);

	return err;
}

<<<<<<< HEAD
static void mlx5_qp_set_frag_buf(struct mlx5_frag_buf *buf,
				 struct mlx5_wq_qp *qp)
{
	struct mlx5_frag_buf_ctrl *sq_fbc;
	struct mlx5_frag_buf *rqb, *sqb;

	rqb  = &qp->rq.fbc.frag_buf;
	*rqb = *buf;
	rqb->size   = mlx5_wq_cyc_get_byte_size(&qp->rq);
	rqb->npages = DIV_ROUND_UP(rqb->size, PAGE_SIZE);

	sq_fbc = &qp->sq.fbc;
	sqb    = &sq_fbc->frag_buf;
	*sqb   = *buf;
	sqb->size   = mlx5_wq_cyc_get_byte_size(&qp->sq);
	sqb->npages = DIV_ROUND_UP(sqb->size, PAGE_SIZE);
	sqb->frags += rqb->npages; /* first part is for the rq */
	if (sq_fbc->strides_offset)
		sqb->frags--;
=======
void mlx5_wq_cyc_wqe_dump(struct mlx5_wq_cyc *wq, u16 ix, u8 nstrides)
{
	size_t len;
	void *wqe;

	if (!net_ratelimit())
		return;

	nstrides = max_t(u8, nstrides, 1);

	len = nstrides << wq->fbc.log_stride;
	wqe = mlx5_wq_cyc_get_wqe(wq, ix);

	pr_info("WQE DUMP: WQ size %d WQ cur size %d, WQE index 0x%x, len: %zu\n",
		mlx5_wq_cyc_get_size(wq), wq->cur_sz, ix, len);
	print_hex_dump(KERN_WARNING, "", DUMP_PREFIX_OFFSET, 16, 1, wqe, len, false);
}

void mlx5_wq_cyc_reset(struct mlx5_wq_cyc *wq)
{
	wq->wqe_ctr = 0;
	wq->cur_sz = 0;
	mlx5_wq_cyc_update_db_record(wq);
>>>>>>> upstream/android-13
}

int mlx5_wq_qp_create(struct mlx5_core_dev *mdev, struct mlx5_wq_param *param,
		      void *qpc, struct mlx5_wq_qp *wq,
		      struct mlx5_wq_ctrl *wq_ctrl)
{
<<<<<<< HEAD
	u16 sq_strides_offset;
	u32 rq_pg_remainder;
	int err;

	mlx5_fill_fbc(MLX5_GET(qpc, qpc, log_rq_stride) + 4,
		      MLX5_GET(qpc, qpc, log_rq_size),
		      &wq->rq.fbc);

	rq_pg_remainder   = mlx5_wq_cyc_get_byte_size(&wq->rq) % PAGE_SIZE;
	sq_strides_offset = rq_pg_remainder / MLX5_SEND_WQE_BB;

	mlx5_fill_fbc_offset(ilog2(MLX5_SEND_WQE_BB),
			     MLX5_GET(qpc, qpc, log_sq_size),
			     sq_strides_offset,
			     &wq->sq.fbc);
=======
	u8 log_rq_stride = MLX5_GET(qpc, qpc, log_rq_stride) + 4;
	u8 log_rq_sz     = MLX5_GET(qpc, qpc, log_rq_size);
	u8 log_sq_stride = ilog2(MLX5_SEND_WQE_BB);
	u8 log_sq_sz     = MLX5_GET(qpc, qpc, log_sq_size);

	u32 rq_byte_size;
	int err;


>>>>>>> upstream/android-13

	err = mlx5_db_alloc_node(mdev, &wq_ctrl->db, param->db_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_db_alloc_node() failed, %d\n", err);
		return err;
	}

<<<<<<< HEAD
	err = mlx5_frag_buf_alloc_node(mdev, mlx5_wq_qp_get_byte_size(wq),
=======
	err = mlx5_frag_buf_alloc_node(mdev,
				       wq_get_byte_sz(log_rq_sz, log_rq_stride) +
				       wq_get_byte_sz(log_sq_sz, log_sq_stride),
>>>>>>> upstream/android-13
				       &wq_ctrl->buf, param->buf_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_frag_buf_alloc_node() failed, %d\n", err);
		goto err_db_free;
	}

<<<<<<< HEAD
	mlx5_qp_set_frag_buf(&wq_ctrl->buf, wq);
=======
	mlx5_init_fbc(wq_ctrl->buf.frags, log_rq_stride, log_rq_sz, &wq->rq.fbc);

	rq_byte_size = wq_get_byte_sz(log_rq_sz, log_rq_stride);

	if (rq_byte_size < PAGE_SIZE) {
		/* SQ starts within the same page of the RQ */
		u16 sq_strides_offset = rq_byte_size / MLX5_SEND_WQE_BB;

		mlx5_init_fbc_offset(wq_ctrl->buf.frags,
				     log_sq_stride, log_sq_sz, sq_strides_offset,
				     &wq->sq.fbc);
	} else {
		u16 rq_npages = rq_byte_size >> PAGE_SHIFT;

		mlx5_init_fbc(wq_ctrl->buf.frags + rq_npages,
			      log_sq_stride, log_sq_sz, &wq->sq.fbc);
	}
>>>>>>> upstream/android-13

	wq->rq.db  = &wq_ctrl->db.db[MLX5_RCV_DBR];
	wq->sq.db  = &wq_ctrl->db.db[MLX5_SND_DBR];

	wq_ctrl->mdev = mdev;

	return 0;

err_db_free:
	mlx5_db_free(mdev, &wq_ctrl->db);

	return err;
}

int mlx5_cqwq_create(struct mlx5_core_dev *mdev, struct mlx5_wq_param *param,
		     void *cqc, struct mlx5_cqwq *wq,
		     struct mlx5_wq_ctrl *wq_ctrl)
{
<<<<<<< HEAD
	int err;

	mlx5_core_init_cq_frag_buf(&wq->fbc, cqc);

=======
	/* CQE_STRIDE_128 and CQE_STRIDE_128_PAD both mean 128B stride */
	u8 log_wq_stride = MLX5_GET(cqc, cqc, cqe_sz) == CQE_STRIDE_64 ? 6 : 7;
	u8 log_wq_sz     = MLX5_GET(cqc, cqc, log_cq_size);
	int err;

>>>>>>> upstream/android-13
	err = mlx5_db_alloc_node(mdev, &wq_ctrl->db, param->db_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_db_alloc_node() failed, %d\n", err);
		return err;
	}

<<<<<<< HEAD
	err = mlx5_frag_buf_alloc_node(mdev, mlx5_cqwq_get_byte_size(wq),
=======
	wq->db  = wq_ctrl->db.db;

	err = mlx5_frag_buf_alloc_node(mdev, wq_get_byte_sz(log_wq_sz, log_wq_stride),
>>>>>>> upstream/android-13
				       &wq_ctrl->buf,
				       param->buf_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_frag_buf_alloc_node() failed, %d\n",
			       err);
		goto err_db_free;
	}

<<<<<<< HEAD
	wq->fbc.frag_buf = wq_ctrl->buf;
	wq->db  = wq_ctrl->db.db;
=======
	mlx5_init_fbc(wq_ctrl->buf.frags, log_wq_stride, log_wq_sz, &wq->fbc);
>>>>>>> upstream/android-13

	wq_ctrl->mdev = mdev;

	return 0;

err_db_free:
	mlx5_db_free(mdev, &wq_ctrl->db);

	return err;
}

<<<<<<< HEAD
=======
static void mlx5_wq_ll_init_list(struct mlx5_wq_ll *wq)
{
	struct mlx5_wqe_srq_next_seg *next_seg;
	int i;

	for (i = 0; i < wq->fbc.sz_m1; i++) {
		next_seg = mlx5_wq_ll_get_wqe(wq, i);
		next_seg->next_wqe_index = cpu_to_be16(i + 1);
	}
	next_seg = mlx5_wq_ll_get_wqe(wq, i);
	wq->tail_next = &next_seg->next_wqe_index;
}

>>>>>>> upstream/android-13
int mlx5_wq_ll_create(struct mlx5_core_dev *mdev, struct mlx5_wq_param *param,
		      void *wqc, struct mlx5_wq_ll *wq,
		      struct mlx5_wq_ctrl *wq_ctrl)
{
<<<<<<< HEAD
	struct mlx5_frag_buf_ctrl *fbc = &wq->fbc;
	struct mlx5_wqe_srq_next_seg *next_seg;
	int err;
	int i;

	mlx5_fill_fbc(MLX5_GET(wq, wqc, log_wq_stride),
		      MLX5_GET(wq, wqc, log_wq_sz),
		      fbc);
=======
	u8 log_wq_stride = MLX5_GET(wq, wqc, log_wq_stride);
	u8 log_wq_sz     = MLX5_GET(wq, wqc, log_wq_sz);
	struct mlx5_frag_buf_ctrl *fbc = &wq->fbc;
	int err;
>>>>>>> upstream/android-13

	err = mlx5_db_alloc_node(mdev, &wq_ctrl->db, param->db_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_db_alloc_node() failed, %d\n", err);
		return err;
	}

<<<<<<< HEAD
	err = mlx5_frag_buf_alloc_node(mdev, mlx5_wq_ll_get_byte_size(wq),
=======
	wq->db  = wq_ctrl->db.db;

	err = mlx5_frag_buf_alloc_node(mdev, wq_get_byte_sz(log_wq_sz, log_wq_stride),
>>>>>>> upstream/android-13
				       &wq_ctrl->buf, param->buf_numa_node);
	if (err) {
		mlx5_core_warn(mdev, "mlx5_frag_buf_alloc_node() failed, %d\n", err);
		goto err_db_free;
	}

<<<<<<< HEAD
	wq->fbc.frag_buf = wq_ctrl->buf;
	wq->db  = wq_ctrl->db.db;

	for (i = 0; i < fbc->sz_m1; i++) {
		next_seg = mlx5_wq_ll_get_wqe(wq, i);
		next_seg->next_wqe_index = cpu_to_be16(i + 1);
	}
	next_seg = mlx5_wq_ll_get_wqe(wq, i);
	wq->tail_next = &next_seg->next_wqe_index;

=======
	mlx5_init_fbc(wq_ctrl->buf.frags, log_wq_stride, log_wq_sz, fbc);

	mlx5_wq_ll_init_list(wq);
>>>>>>> upstream/android-13
	wq_ctrl->mdev = mdev;

	return 0;

err_db_free:
	mlx5_db_free(mdev, &wq_ctrl->db);

	return err;
}

<<<<<<< HEAD
=======
void mlx5_wq_ll_reset(struct mlx5_wq_ll *wq)
{
	wq->head = 0;
	wq->wqe_ctr = 0;
	wq->cur_sz = 0;
	mlx5_wq_ll_init_list(wq);
	mlx5_wq_ll_update_db_record(wq);
}

>>>>>>> upstream/android-13
void mlx5_wq_destroy(struct mlx5_wq_ctrl *wq_ctrl)
{
	mlx5_frag_buf_free(wq_ctrl->mdev, &wq_ctrl->buf);
	mlx5_db_free(wq_ctrl->mdev, &wq_ctrl->db);
}

