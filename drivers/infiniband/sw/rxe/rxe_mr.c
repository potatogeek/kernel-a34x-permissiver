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

#include "rxe.h"
#include "rxe_loc.h"

<<<<<<< HEAD
/*
 * lfsr (linear feedback shift register) with period 255
 */
static u8 rxe_get_key(void)
{
	static u32 key = 1;

	key = key << 1;

	key |= (0 != (key & 0x100)) ^ (0 != (key & 0x10))
		^ (0 != (key & 0x80)) ^ (0 != (key & 0x40));

	key &= 0xff;
=======
/* Return a random 8 bit key value that is
 * different than the last_key. Set last_key to -1
 * if this is the first key for an MR or MW
 */
u8 rxe_get_next_key(u32 last_key)
{
	u8 key;

	do {
		get_random_bytes(&key, 1);
	} while (key == last_key);
>>>>>>> upstream/android-13

	return key;
}

<<<<<<< HEAD
int mem_check_range(struct rxe_mem *mem, u64 iova, size_t length)
{
	switch (mem->type) {
	case RXE_MEM_TYPE_DMA:
		return 0;

	case RXE_MEM_TYPE_MR:
	case RXE_MEM_TYPE_FMR:
		if (iova < mem->iova ||
		    length > mem->length ||
		    iova > mem->iova + mem->length - length)
=======
int mr_check_range(struct rxe_mr *mr, u64 iova, size_t length)
{
	switch (mr->type) {
	case RXE_MR_TYPE_DMA:
		return 0;

	case RXE_MR_TYPE_MR:
		if (iova < mr->iova || length > mr->length ||
		    iova > mr->iova + mr->length - length)
>>>>>>> upstream/android-13
			return -EFAULT;
		return 0;

	default:
		return -EFAULT;
	}
}

#define IB_ACCESS_REMOTE	(IB_ACCESS_REMOTE_READ		\
				| IB_ACCESS_REMOTE_WRITE	\
				| IB_ACCESS_REMOTE_ATOMIC)

<<<<<<< HEAD
static void rxe_mem_init(int access, struct rxe_mem *mem)
{
	u32 lkey = mem->pelem.index << 8 | rxe_get_key();
	u32 rkey = (access & IB_ACCESS_REMOTE) ? lkey : 0;

	if (mem->pelem.pool->type == RXE_TYPE_MR) {
		mem->ibmr.lkey		= lkey;
		mem->ibmr.rkey		= rkey;
	}

	mem->lkey		= lkey;
	mem->rkey		= rkey;
	mem->state		= RXE_MEM_STATE_INVALID;
	mem->type		= RXE_MEM_TYPE_NONE;
	mem->map_shift		= ilog2(RXE_BUF_PER_MAP);
}

void rxe_mem_cleanup(struct rxe_pool_entry *arg)
{
	struct rxe_mem *mem = container_of(arg, typeof(*mem), pelem);
	int i;

	if (mem->umem)
		ib_umem_release(mem->umem);

	if (mem->map) {
		for (i = 0; i < mem->num_map; i++)
			kfree(mem->map[i]);

		kfree(mem->map);
	}
}

static int rxe_mem_alloc(struct rxe_mem *mem, int num_buf)
{
	int i;
	int num_map;
	struct rxe_map **map = mem->map;

	num_map = (num_buf + RXE_BUF_PER_MAP - 1) / RXE_BUF_PER_MAP;

	mem->map = kmalloc_array(num_map, sizeof(*map), GFP_KERNEL);
	if (!mem->map)
		goto err1;

	for (i = 0; i < num_map; i++) {
		mem->map[i] = kmalloc(sizeof(**map), GFP_KERNEL);
		if (!mem->map[i])
=======
static void rxe_mr_init(int access, struct rxe_mr *mr)
{
	u32 lkey = mr->pelem.index << 8 | rxe_get_next_key(-1);
	u32 rkey = (access & IB_ACCESS_REMOTE) ? lkey : 0;

	/* set ibmr->l/rkey and also copy into private l/rkey
	 * for user MRs these will always be the same
	 * for cases where caller 'owns' the key portion
	 * they may be different until REG_MR WQE is executed.
	 */
	mr->lkey = mr->ibmr.lkey = lkey;
	mr->rkey = mr->ibmr.rkey = rkey;

	mr->state = RXE_MR_STATE_INVALID;
	mr->type = RXE_MR_TYPE_NONE;
	mr->map_shift = ilog2(RXE_BUF_PER_MAP);
}

static int rxe_mr_alloc(struct rxe_mr *mr, int num_buf)
{
	int i;
	int num_map;
	struct rxe_map **map = mr->map;

	num_map = (num_buf + RXE_BUF_PER_MAP - 1) / RXE_BUF_PER_MAP;

	mr->map = kmalloc_array(num_map, sizeof(*map), GFP_KERNEL);
	if (!mr->map)
		goto err1;

	for (i = 0; i < num_map; i++) {
		mr->map[i] = kmalloc(sizeof(**map), GFP_KERNEL);
		if (!mr->map[i])
>>>>>>> upstream/android-13
			goto err2;
	}

	BUILD_BUG_ON(!is_power_of_2(RXE_BUF_PER_MAP));

<<<<<<< HEAD
	mem->map_shift	= ilog2(RXE_BUF_PER_MAP);
	mem->map_mask	= RXE_BUF_PER_MAP - 1;

	mem->num_buf = num_buf;
	mem->num_map = num_map;
	mem->max_buf = num_map * RXE_BUF_PER_MAP;
=======
	mr->map_shift = ilog2(RXE_BUF_PER_MAP);
	mr->map_mask = RXE_BUF_PER_MAP - 1;

	mr->num_buf = num_buf;
	mr->num_map = num_map;
	mr->max_buf = num_map * RXE_BUF_PER_MAP;
>>>>>>> upstream/android-13

	return 0;

err2:
	for (i--; i >= 0; i--)
<<<<<<< HEAD
		kfree(mem->map[i]);

	kfree(mem->map);
=======
		kfree(mr->map[i]);

	kfree(mr->map);
>>>>>>> upstream/android-13
err1:
	return -ENOMEM;
}

<<<<<<< HEAD
int rxe_mem_init_dma(struct rxe_pd *pd,
		     int access, struct rxe_mem *mem)
{
	rxe_mem_init(access, mem);

	mem->pd			= pd;
	mem->access		= access;
	mem->state		= RXE_MEM_STATE_VALID;
	mem->type		= RXE_MEM_TYPE_DMA;

	return 0;
}

int rxe_mem_init_user(struct rxe_pd *pd, u64 start,
		      u64 length, u64 iova, int access, struct ib_udata *udata,
		      struct rxe_mem *mem)
{
	int			entry;
	struct rxe_map		**map;
	struct rxe_phys_buf	*buf = NULL;
	struct ib_umem		*umem;
	struct scatterlist	*sg;
	int			num_buf;
	void			*vaddr;
	int err;

	umem = ib_umem_get(pd->ibpd.uobject->context, start, length, access, 0);
	if (IS_ERR(umem)) {
		pr_warn("err %d from rxe_umem_get\n",
			(int)PTR_ERR(umem));
		err = -EINVAL;
		goto err1;
	}

	mem->umem = umem;
	num_buf = umem->nmap;

	rxe_mem_init(access, mem);

	err = rxe_mem_alloc(mem, num_buf);
	if (err) {
		pr_warn("err %d from rxe_mem_alloc\n", err);
		ib_umem_release(umem);
		goto err1;
	}

	mem->page_shift		= umem->page_shift;
	mem->page_mask		= BIT(umem->page_shift) - 1;

	num_buf			= 0;
	map			= mem->map;
	if (length > 0) {
		buf = map[0]->buf;

		for_each_sg(umem->sg_head.sgl, sg, umem->nmap, entry) {
			vaddr = page_address(sg_page(sg));
			if (!vaddr) {
				pr_warn("null vaddr\n");
				ib_umem_release(umem);
				err = -ENOMEM;
				goto err1;
			}

			buf->addr = (uintptr_t)vaddr;
			buf->size = BIT(umem->page_shift);
			num_buf++;
			buf++;

=======
void rxe_mr_init_dma(struct rxe_pd *pd, int access, struct rxe_mr *mr)
{
	rxe_mr_init(access, mr);

	mr->ibmr.pd = &pd->ibpd;
	mr->access = access;
	mr->state = RXE_MR_STATE_VALID;
	mr->type = RXE_MR_TYPE_DMA;
}

int rxe_mr_init_user(struct rxe_pd *pd, u64 start, u64 length, u64 iova,
		     int access, struct rxe_mr *mr)
{
	struct rxe_map		**map;
	struct rxe_phys_buf	*buf = NULL;
	struct ib_umem		*umem;
	struct sg_page_iter	sg_iter;
	int			num_buf;
	void			*vaddr;
	int err;
	int i;

	umem = ib_umem_get(pd->ibpd.device, start, length, access);
	if (IS_ERR(umem)) {
		pr_warn("%s: Unable to pin memory region err = %d\n",
			__func__, (int)PTR_ERR(umem));
		err = PTR_ERR(umem);
		goto err_out;
	}

	num_buf = ib_umem_num_pages(umem);

	rxe_mr_init(access, mr);

	err = rxe_mr_alloc(mr, num_buf);
	if (err) {
		pr_warn("%s: Unable to allocate memory for map\n",
				__func__);
		goto err_release_umem;
	}

	mr->page_shift = PAGE_SHIFT;
	mr->page_mask = PAGE_SIZE - 1;

	num_buf			= 0;
	map = mr->map;
	if (length > 0) {
		buf = map[0]->buf;

		for_each_sgtable_page (&umem->sgt_append.sgt, &sg_iter, 0) {
>>>>>>> upstream/android-13
			if (num_buf >= RXE_BUF_PER_MAP) {
				map++;
				buf = map[0]->buf;
				num_buf = 0;
			}
<<<<<<< HEAD
		}
	}

	mem->pd			= pd;
	mem->umem		= umem;
	mem->access		= access;
	mem->length		= length;
	mem->iova		= iova;
	mem->va			= start;
	mem->offset		= ib_umem_offset(umem);
	mem->state		= RXE_MEM_STATE_VALID;
	mem->type		= RXE_MEM_TYPE_MR;

	return 0;

err1:
	return err;
}

int rxe_mem_init_fast(struct rxe_pd *pd,
		      int max_pages, struct rxe_mem *mem)
{
	int err;

	rxe_mem_init(0, mem);

	/* In fastreg, we also set the rkey */
	mem->ibmr.rkey = mem->ibmr.lkey;

	err = rxe_mem_alloc(mem, max_pages);
	if (err)
		goto err1;

	mem->pd			= pd;
	mem->max_buf		= max_pages;
	mem->state		= RXE_MEM_STATE_FREE;
	mem->type		= RXE_MEM_TYPE_MR;
=======

			vaddr = page_address(sg_page_iter_page(&sg_iter));
			if (!vaddr) {
				pr_warn("%s: Unable to get virtual address\n",
						__func__);
				err = -ENOMEM;
				goto err_cleanup_map;
			}

			buf->addr = (uintptr_t)vaddr;
			buf->size = PAGE_SIZE;
			num_buf++;
			buf++;

		}
	}

	mr->ibmr.pd = &pd->ibpd;
	mr->umem = umem;
	mr->access = access;
	mr->length = length;
	mr->iova = iova;
	mr->va = start;
	mr->offset = ib_umem_offset(umem);
	mr->state = RXE_MR_STATE_VALID;
	mr->type = RXE_MR_TYPE_MR;

	return 0;

err_cleanup_map:
	for (i = 0; i < mr->num_map; i++)
		kfree(mr->map[i]);
	kfree(mr->map);
err_release_umem:
	ib_umem_release(umem);
err_out:
	return err;
}

int rxe_mr_init_fast(struct rxe_pd *pd, int max_pages, struct rxe_mr *mr)
{
	int err;

	/* always allow remote access for FMRs */
	rxe_mr_init(IB_ACCESS_REMOTE, mr);

	err = rxe_mr_alloc(mr, max_pages);
	if (err)
		goto err1;

	mr->ibmr.pd = &pd->ibpd;
	mr->max_buf = max_pages;
	mr->state = RXE_MR_STATE_FREE;
	mr->type = RXE_MR_TYPE_MR;
>>>>>>> upstream/android-13

	return 0;

err1:
	return err;
}

<<<<<<< HEAD
static void lookup_iova(
	struct rxe_mem	*mem,
	u64			iova,
	int			*m_out,
	int			*n_out,
	size_t			*offset_out)
{
	size_t			offset = iova - mem->iova + mem->offset;
=======
static void lookup_iova(struct rxe_mr *mr, u64 iova, int *m_out, int *n_out,
			size_t *offset_out)
{
	size_t offset = iova - mr->iova + mr->offset;
>>>>>>> upstream/android-13
	int			map_index;
	int			buf_index;
	u64			length;

<<<<<<< HEAD
	if (likely(mem->page_shift)) {
		*offset_out = offset & mem->page_mask;
		offset >>= mem->page_shift;
		*n_out = offset & mem->map_mask;
		*m_out = offset >> mem->map_shift;
=======
	if (likely(mr->page_shift)) {
		*offset_out = offset & mr->page_mask;
		offset >>= mr->page_shift;
		*n_out = offset & mr->map_mask;
		*m_out = offset >> mr->map_shift;
>>>>>>> upstream/android-13
	} else {
		map_index = 0;
		buf_index = 0;

<<<<<<< HEAD
		length = mem->map[map_index]->buf[buf_index].size;
=======
		length = mr->map[map_index]->buf[buf_index].size;
>>>>>>> upstream/android-13

		while (offset >= length) {
			offset -= length;
			buf_index++;

			if (buf_index == RXE_BUF_PER_MAP) {
				map_index++;
				buf_index = 0;
			}
<<<<<<< HEAD
			length = mem->map[map_index]->buf[buf_index].size;
=======
			length = mr->map[map_index]->buf[buf_index].size;
>>>>>>> upstream/android-13
		}

		*m_out = map_index;
		*n_out = buf_index;
		*offset_out = offset;
	}
}

<<<<<<< HEAD
void *iova_to_vaddr(struct rxe_mem *mem, u64 iova, int length)
=======
void *iova_to_vaddr(struct rxe_mr *mr, u64 iova, int length)
>>>>>>> upstream/android-13
{
	size_t offset;
	int m, n;
	void *addr;

<<<<<<< HEAD
	if (mem->state != RXE_MEM_STATE_VALID) {
		pr_warn("mem not in valid state\n");
=======
	if (mr->state != RXE_MR_STATE_VALID) {
		pr_warn("mr not in valid state\n");
>>>>>>> upstream/android-13
		addr = NULL;
		goto out;
	}

<<<<<<< HEAD
	if (!mem->map) {
=======
	if (!mr->map) {
>>>>>>> upstream/android-13
		addr = (void *)(uintptr_t)iova;
		goto out;
	}

<<<<<<< HEAD
	if (mem_check_range(mem, iova, length)) {
=======
	if (mr_check_range(mr, iova, length)) {
>>>>>>> upstream/android-13
		pr_warn("range violation\n");
		addr = NULL;
		goto out;
	}

<<<<<<< HEAD
	lookup_iova(mem, iova, &m, &n, &offset);

	if (offset + length > mem->map[m]->buf[n].size) {
=======
	lookup_iova(mr, iova, &m, &n, &offset);

	if (offset + length > mr->map[m]->buf[n].size) {
>>>>>>> upstream/android-13
		pr_warn("crosses page boundary\n");
		addr = NULL;
		goto out;
	}

<<<<<<< HEAD
	addr = (void *)(uintptr_t)mem->map[m]->buf[n].addr + offset;
=======
	addr = (void *)(uintptr_t)mr->map[m]->buf[n].addr + offset;
>>>>>>> upstream/android-13

out:
	return addr;
}

/* copy data from a range (vaddr, vaddr+length-1) to or from
<<<<<<< HEAD
 * a mem object starting at iova. Compute incremental value of
 * crc32 if crcp is not zero. caller must hold a reference to mem
 */
int rxe_mem_copy(struct rxe_mem *mem, u64 iova, void *addr, int length,
		 enum copy_direction dir, u32 *crcp)
=======
 * a mr object starting at iova.
 */
int rxe_mr_copy(struct rxe_mr *mr, u64 iova, void *addr, int length,
		enum rxe_mr_copy_dir dir)
>>>>>>> upstream/android-13
{
	int			err;
	int			bytes;
	u8			*va;
	struct rxe_map		**map;
	struct rxe_phys_buf	*buf;
	int			m;
	int			i;
	size_t			offset;
<<<<<<< HEAD
	u32			crc = crcp ? (*crcp) : 0;
=======
>>>>>>> upstream/android-13

	if (length == 0)
		return 0;

<<<<<<< HEAD
	if (mem->type == RXE_MEM_TYPE_DMA) {
		u8 *src, *dest;

		src  = (dir == to_mem_obj) ?
			addr : ((void *)(uintptr_t)iova);

		dest = (dir == to_mem_obj) ?
			((void *)(uintptr_t)iova) : addr;

		memcpy(dest, src, length);

		if (crcp)
			*crcp = rxe_crc32(to_rdev(mem->pd->ibpd.device),
					*crcp, dest, length);

		return 0;
	}

	WARN_ON_ONCE(!mem->map);

	err = mem_check_range(mem, iova, length);
=======
	if (mr->type == RXE_MR_TYPE_DMA) {
		u8 *src, *dest;

		src = (dir == RXE_TO_MR_OBJ) ? addr : ((void *)(uintptr_t)iova);

		dest = (dir == RXE_TO_MR_OBJ) ? ((void *)(uintptr_t)iova) : addr;

		memcpy(dest, src, length);

		return 0;
	}

	WARN_ON_ONCE(!mr->map);

	err = mr_check_range(mr, iova, length);
>>>>>>> upstream/android-13
	if (err) {
		err = -EFAULT;
		goto err1;
	}

<<<<<<< HEAD
	lookup_iova(mem, iova, &m, &i, &offset);

	map	= mem->map + m;
=======
	lookup_iova(mr, iova, &m, &i, &offset);

	map = mr->map + m;
>>>>>>> upstream/android-13
	buf	= map[0]->buf + i;

	while (length > 0) {
		u8 *src, *dest;

		va	= (u8 *)(uintptr_t)buf->addr + offset;
<<<<<<< HEAD
		src  = (dir == to_mem_obj) ? addr : va;
		dest = (dir == to_mem_obj) ? va : addr;
=======
		src = (dir == RXE_TO_MR_OBJ) ? addr : va;
		dest = (dir == RXE_TO_MR_OBJ) ? va : addr;
>>>>>>> upstream/android-13

		bytes	= buf->size - offset;

		if (bytes > length)
			bytes = length;

		memcpy(dest, src, bytes);

<<<<<<< HEAD
		if (crcp)
			crc = rxe_crc32(to_rdev(mem->pd->ibpd.device),
					crc, dest, bytes);

=======
>>>>>>> upstream/android-13
		length	-= bytes;
		addr	+= bytes;

		offset	= 0;
		buf++;
		i++;

		if (i == RXE_BUF_PER_MAP) {
			i = 0;
			map++;
			buf = map[0]->buf;
		}
	}

<<<<<<< HEAD
	if (crcp)
		*crcp = crc;

=======
>>>>>>> upstream/android-13
	return 0;

err1:
	return err;
}

/* copy data in or out of a wqe, i.e. sg list
 * under the control of a dma descriptor
 */
int copy_data(
	struct rxe_pd		*pd,
	int			access,
	struct rxe_dma_info	*dma,
	void			*addr,
	int			length,
<<<<<<< HEAD
	enum copy_direction	dir,
	u32			*crcp)
=======
	enum rxe_mr_copy_dir	dir)
>>>>>>> upstream/android-13
{
	int			bytes;
	struct rxe_sge		*sge	= &dma->sge[dma->cur_sge];
	int			offset	= dma->sge_offset;
	int			resid	= dma->resid;
<<<<<<< HEAD
	struct rxe_mem		*mem	= NULL;
=======
	struct rxe_mr		*mr	= NULL;
>>>>>>> upstream/android-13
	u64			iova;
	int			err;

	if (length == 0)
		return 0;

	if (length > resid) {
		err = -EINVAL;
		goto err2;
	}

	if (sge->length && (offset < sge->length)) {
<<<<<<< HEAD
		mem = lookup_mem(pd, access, sge->lkey, lookup_local);
		if (!mem) {
=======
		mr = lookup_mr(pd, access, sge->lkey, RXE_LOOKUP_LOCAL);
		if (!mr) {
>>>>>>> upstream/android-13
			err = -EINVAL;
			goto err1;
		}
	}

	while (length > 0) {
		bytes = length;

		if (offset >= sge->length) {
<<<<<<< HEAD
			if (mem) {
				rxe_drop_ref(mem);
				mem = NULL;
=======
			if (mr) {
				rxe_drop_ref(mr);
				mr = NULL;
>>>>>>> upstream/android-13
			}
			sge++;
			dma->cur_sge++;
			offset = 0;

			if (dma->cur_sge >= dma->num_sge) {
				err = -ENOSPC;
				goto err2;
			}

			if (sge->length) {
<<<<<<< HEAD
				mem = lookup_mem(pd, access, sge->lkey,
						 lookup_local);
				if (!mem) {
=======
				mr = lookup_mr(pd, access, sge->lkey,
					       RXE_LOOKUP_LOCAL);
				if (!mr) {
>>>>>>> upstream/android-13
					err = -EINVAL;
					goto err1;
				}
			} else {
				continue;
			}
		}

		if (bytes > sge->length - offset)
			bytes = sge->length - offset;

		if (bytes > 0) {
			iova = sge->addr + offset;

<<<<<<< HEAD
			err = rxe_mem_copy(mem, iova, addr, bytes, dir, crcp);
=======
			err = rxe_mr_copy(mr, iova, addr, bytes, dir);
>>>>>>> upstream/android-13
			if (err)
				goto err2;

			offset	+= bytes;
			resid	-= bytes;
			length	-= bytes;
			addr	+= bytes;
		}
	}

	dma->sge_offset = offset;
	dma->resid	= resid;

<<<<<<< HEAD
	if (mem)
		rxe_drop_ref(mem);
=======
	if (mr)
		rxe_drop_ref(mr);
>>>>>>> upstream/android-13

	return 0;

err2:
<<<<<<< HEAD
	if (mem)
		rxe_drop_ref(mem);
=======
	if (mr)
		rxe_drop_ref(mr);
>>>>>>> upstream/android-13
err1:
	return err;
}

int advance_dma_data(struct rxe_dma_info *dma, unsigned int length)
{
	struct rxe_sge		*sge	= &dma->sge[dma->cur_sge];
	int			offset	= dma->sge_offset;
	int			resid	= dma->resid;

	while (length) {
		unsigned int bytes;

		if (offset >= sge->length) {
			sge++;
			dma->cur_sge++;
			offset = 0;
			if (dma->cur_sge >= dma->num_sge)
				return -ENOSPC;
		}

		bytes = length;

		if (bytes > sge->length - offset)
			bytes = sge->length - offset;

		offset	+= bytes;
		resid	-= bytes;
		length	-= bytes;
	}

	dma->sge_offset = offset;
	dma->resid	= resid;

	return 0;
}

<<<<<<< HEAD
/* (1) find the mem (mr or mw) corresponding to lkey/rkey
 *     depending on lookup_type
 * (2) verify that the (qp) pd matches the mem pd
 * (3) verify that the mem can support the requested access
 * (4) verify that mem state is valid
 */
struct rxe_mem *lookup_mem(struct rxe_pd *pd, int access, u32 key,
			   enum lookup_type type)
{
	struct rxe_mem *mem;
	struct rxe_dev *rxe = to_rdev(pd->ibpd.device);
	int index = key >> 8;

	if (index >= RXE_MIN_MR_INDEX && index <= RXE_MAX_MR_INDEX) {
		mem = rxe_pool_get_index(&rxe->mr_pool, index);
		if (!mem)
			goto err1;
	} else {
		goto err1;
	}

	if ((type == lookup_local && mem->lkey != key) ||
	    (type == lookup_remote && mem->rkey != key))
		goto err2;

	if (mem->pd != pd)
		goto err2;

	if (access && !(access & mem->access))
		goto err2;

	if (mem->state != RXE_MEM_STATE_VALID)
		goto err2;

	return mem;

err2:
	rxe_drop_ref(mem);
err1:
	return NULL;
}

int rxe_mem_map_pages(struct rxe_dev *rxe, struct rxe_mem *mem,
		      u64 *page, int num_pages, u64 iova)
{
	int i;
	int num_buf;
	int err;
	struct rxe_map **map;
	struct rxe_phys_buf *buf;
	int page_size;

	if (num_pages > mem->max_buf) {
		err = -EINVAL;
		goto err1;
	}

	num_buf		= 0;
	page_size	= 1 << mem->page_shift;
	map		= mem->map;
	buf		= map[0]->buf;

	for (i = 0; i < num_pages; i++) {
		buf->addr = *page++;
		buf->size = page_size;
		buf++;
		num_buf++;

		if (num_buf == RXE_BUF_PER_MAP) {
			map++;
			buf = map[0]->buf;
			num_buf = 0;
		}
	}

	mem->iova	= iova;
	mem->va		= iova;
	mem->length	= num_pages << mem->page_shift;
	mem->state	= RXE_MEM_STATE_VALID;

	return 0;

err1:
	return err;
=======
/* (1) find the mr corresponding to lkey/rkey
 *     depending on lookup_type
 * (2) verify that the (qp) pd matches the mr pd
 * (3) verify that the mr can support the requested access
 * (4) verify that mr state is valid
 */
struct rxe_mr *lookup_mr(struct rxe_pd *pd, int access, u32 key,
			 enum rxe_mr_lookup_type type)
{
	struct rxe_mr *mr;
	struct rxe_dev *rxe = to_rdev(pd->ibpd.device);
	int index = key >> 8;

	mr = rxe_pool_get_index(&rxe->mr_pool, index);
	if (!mr)
		return NULL;

	if (unlikely((type == RXE_LOOKUP_LOCAL && mr->lkey != key) ||
		     (type == RXE_LOOKUP_REMOTE && mr->rkey != key) ||
		     mr_pd(mr) != pd || (access && !(access & mr->access)) ||
		     mr->state != RXE_MR_STATE_VALID)) {
		rxe_drop_ref(mr);
		mr = NULL;
	}

	return mr;
}

int rxe_invalidate_mr(struct rxe_qp *qp, u32 rkey)
{
	struct rxe_dev *rxe = to_rdev(qp->ibqp.device);
	struct rxe_mr *mr;
	int ret;

	mr = rxe_pool_get_index(&rxe->mr_pool, rkey >> 8);
	if (!mr) {
		pr_err("%s: No MR for rkey %#x\n", __func__, rkey);
		ret = -EINVAL;
		goto err;
	}

	if (rkey != mr->rkey) {
		pr_err("%s: rkey (%#x) doesn't match mr->rkey (%#x)\n",
			__func__, rkey, mr->rkey);
		ret = -EINVAL;
		goto err_drop_ref;
	}

	if (atomic_read(&mr->num_mw) > 0) {
		pr_warn("%s: Attempt to invalidate an MR while bound to MWs\n",
			__func__);
		ret = -EINVAL;
		goto err_drop_ref;
	}

	mr->state = RXE_MR_STATE_FREE;
	ret = 0;

err_drop_ref:
	rxe_drop_ref(mr);
err:
	return ret;
}

/* user can (re)register fast MR by executing a REG_MR WQE.
 * user is expected to hold a reference on the ib mr until the
 * WQE completes.
 * Once a fast MR is created this is the only way to change the
 * private keys. It is the responsibility of the user to maintain
 * the ib mr keys in sync with rxe mr keys.
 */
int rxe_reg_fast_mr(struct rxe_qp *qp, struct rxe_send_wqe *wqe)
{
	struct rxe_mr *mr = to_rmr(wqe->wr.wr.reg.mr);
	u32 key = wqe->wr.wr.reg.key;
	u32 access = wqe->wr.wr.reg.access;

	/* user can only register MR in free state */
	if (unlikely(mr->state != RXE_MR_STATE_FREE)) {
		pr_warn("%s: mr->lkey = 0x%x not free\n",
			__func__, mr->lkey);
		return -EINVAL;
	}

	/* user can only register mr with qp in same protection domain */
	if (unlikely(qp->ibqp.pd != mr->ibmr.pd)) {
		pr_warn("%s: qp->pd and mr->pd don't match\n",
			__func__);
		return -EINVAL;
	}

	/* user is only allowed to change key portion of l/rkey */
	if (unlikely((mr->lkey & ~0xff) != (key & ~0xff))) {
		pr_warn("%s: key = 0x%x has wrong index mr->lkey = 0x%x\n",
			__func__, key, mr->lkey);
		return -EINVAL;
	}

	mr->access = access;
	mr->lkey = key;
	mr->rkey = (access & IB_ACCESS_REMOTE) ? key : 0;
	mr->iova = wqe->wr.wr.reg.mr->iova;
	mr->state = RXE_MR_STATE_VALID;

	return 0;
}

int rxe_dereg_mr(struct ib_mr *ibmr, struct ib_udata *udata)
{
	struct rxe_mr *mr = to_rmr(ibmr);

	if (atomic_read(&mr->num_mw) > 0) {
		pr_warn("%s: Attempt to deregister an MR while bound to MWs\n",
			__func__);
		return -EINVAL;
	}

	mr->state = RXE_MR_STATE_ZOMBIE;
	rxe_drop_ref(mr_pd(mr));
	rxe_drop_index(mr);
	rxe_drop_ref(mr);

	return 0;
}

void rxe_mr_cleanup(struct rxe_pool_entry *arg)
{
	struct rxe_mr *mr = container_of(arg, typeof(*mr), pelem);
	int i;

	ib_umem_release(mr->umem);

	if (mr->map) {
		for (i = 0; i < mr->num_map; i++)
			kfree(mr->map[i]);

		kfree(mr->map);
	}
>>>>>>> upstream/android-13
}
