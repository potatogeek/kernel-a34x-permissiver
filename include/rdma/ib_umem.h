<<<<<<< HEAD
/*
 * Copyright (c) 2007 Cisco Systems.  All rights reserved.
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
=======
/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/*
 * Copyright (c) 2007 Cisco Systems.  All rights reserved.
 * Copyright (c) 2020 Intel Corporation.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef IB_UMEM_H
#define IB_UMEM_H

#include <linux/list.h>
#include <linux/scatterlist.h>
#include <linux/workqueue.h>
<<<<<<< HEAD

struct ib_ucontext;
struct ib_umem_odp;

struct ib_umem {
	struct ib_ucontext     *context;
	size_t			length;
	unsigned long		address;
	int			page_shift;
	int                     writable;
	int                     hugetlb;
	struct work_struct	work;
	struct mm_struct       *mm;
	unsigned long		diff;
	struct ib_umem_odp     *odp_data;
	struct sg_table sg_head;
	int             nmap;
	int             npages;
};

/* Returns the offset of the umem start relative to the first page. */
static inline int ib_umem_offset(struct ib_umem *umem)
{
	return umem->address & (BIT(umem->page_shift) - 1);
}

/* Returns the first page of an ODP umem. */
static inline unsigned long ib_umem_start(struct ib_umem *umem)
{
	return umem->address - ib_umem_offset(umem);
}

/* Returns the address of the page after the last one of an ODP umem. */
static inline unsigned long ib_umem_end(struct ib_umem *umem)
{
	return ALIGN(umem->address + umem->length, BIT(umem->page_shift));
=======
#include <rdma/ib_verbs.h>

struct ib_ucontext;
struct ib_umem_odp;
struct dma_buf_attach_ops;

struct ib_umem {
	struct ib_device       *ibdev;
	struct mm_struct       *owning_mm;
	u64 iova;
	size_t			length;
	unsigned long		address;
	u32 writable : 1;
	u32 is_odp : 1;
	u32 is_dmabuf : 1;
	struct work_struct	work;
	struct sg_append_table sgt_append;
};

struct ib_umem_dmabuf {
	struct ib_umem umem;
	struct dma_buf_attachment *attach;
	struct sg_table *sgt;
	struct scatterlist *first_sg;
	struct scatterlist *last_sg;
	unsigned long first_sg_offset;
	unsigned long last_sg_trim;
	void *private;
};

static inline struct ib_umem_dmabuf *to_ib_umem_dmabuf(struct ib_umem *umem)
{
	return container_of(umem, struct ib_umem_dmabuf, umem);
}

/* Returns the offset of the umem start relative to the first page. */
static inline int ib_umem_offset(struct ib_umem *umem)
{
	return umem->address & ~PAGE_MASK;
}

static inline unsigned long ib_umem_dma_offset(struct ib_umem *umem,
					       unsigned long pgsz)
{
	return (sg_dma_address(umem->sgt_append.sgt.sgl) + ib_umem_offset(umem)) &
	       (pgsz - 1);
}

static inline size_t ib_umem_num_dma_blocks(struct ib_umem *umem,
					    unsigned long pgsz)
{
	return (size_t)((ALIGN(umem->iova + umem->length, pgsz) -
			 ALIGN_DOWN(umem->iova, pgsz))) /
	       pgsz;
>>>>>>> upstream/android-13
}

static inline size_t ib_umem_num_pages(struct ib_umem *umem)
{
<<<<<<< HEAD
	return (ib_umem_end(umem) - ib_umem_start(umem)) >> umem->page_shift;
}

#ifdef CONFIG_INFINIBAND_USER_MEM

struct ib_umem *ib_umem_get(struct ib_ucontext *context, unsigned long addr,
			    size_t size, int access, int dmasync);
void ib_umem_release(struct ib_umem *umem);
int ib_umem_page_count(struct ib_umem *umem);
int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      size_t length);
=======
	return ib_umem_num_dma_blocks(umem, PAGE_SIZE);
}

static inline void __rdma_umem_block_iter_start(struct ib_block_iter *biter,
						struct ib_umem *umem,
						unsigned long pgsz)
{
	__rdma_block_iter_start(biter, umem->sgt_append.sgt.sgl,
				umem->sgt_append.sgt.nents, pgsz);
}

/**
 * rdma_umem_for_each_dma_block - iterate over contiguous DMA blocks of the umem
 * @umem: umem to iterate over
 * @pgsz: Page size to split the list into
 *
 * pgsz must be <= PAGE_SIZE or computed by ib_umem_find_best_pgsz(). The
 * returned DMA blocks will be aligned to pgsz and span the range:
 * ALIGN_DOWN(umem->address, pgsz) to ALIGN(umem->address + umem->length, pgsz)
 *
 * Performs exactly ib_umem_num_dma_blocks() iterations.
 */
#define rdma_umem_for_each_dma_block(umem, biter, pgsz)                        \
	for (__rdma_umem_block_iter_start(biter, umem, pgsz);                  \
	     __rdma_block_iter_next(biter);)

#ifdef CONFIG_INFINIBAND_USER_MEM

struct ib_umem *ib_umem_get(struct ib_device *device, unsigned long addr,
			    size_t size, int access);
void ib_umem_release(struct ib_umem *umem);
int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      size_t length);
unsigned long ib_umem_find_best_pgsz(struct ib_umem *umem,
				     unsigned long pgsz_bitmap,
				     unsigned long virt);

/**
 * ib_umem_find_best_pgoff - Find best HW page size
 *
 * @umem: umem struct
 * @pgsz_bitmap bitmap of HW supported page sizes
 * @pgoff_bitmask: Mask of bits that can be represented with an offset
 *
 * This is very similar to ib_umem_find_best_pgsz() except instead of accepting
 * an IOVA it accepts a bitmask specifying what address bits can be represented
 * with a page offset.
 *
 * For instance if the HW has multiple page sizes, requires 64 byte alignemnt,
 * and can support aligned offsets up to 4032 then pgoff_bitmask would be
 * "111111000000".
 *
 * If the pgoff_bitmask requires either alignment in the low bit or an
 * unavailable page size for the high bits, this function returns 0.
 */
static inline unsigned long ib_umem_find_best_pgoff(struct ib_umem *umem,
						    unsigned long pgsz_bitmap,
						    u64 pgoff_bitmask)
{
	struct scatterlist *sg = umem->sgt_append.sgt.sgl;
	dma_addr_t dma_addr;

	dma_addr = sg_dma_address(sg) + (umem->address & ~PAGE_MASK);
	return ib_umem_find_best_pgsz(umem, pgsz_bitmap,
				      dma_addr & pgoff_bitmask);
}

struct ib_umem_dmabuf *ib_umem_dmabuf_get(struct ib_device *device,
					  unsigned long offset, size_t size,
					  int fd, int access,
					  const struct dma_buf_attach_ops *ops);
int ib_umem_dmabuf_map_pages(struct ib_umem_dmabuf *umem_dmabuf);
void ib_umem_dmabuf_unmap_pages(struct ib_umem_dmabuf *umem_dmabuf);
void ib_umem_dmabuf_release(struct ib_umem_dmabuf *umem_dmabuf);
>>>>>>> upstream/android-13

#else /* CONFIG_INFINIBAND_USER_MEM */

#include <linux/err.h>

<<<<<<< HEAD
static inline struct ib_umem *ib_umem_get(struct ib_ucontext *context,
					  unsigned long addr, size_t size,
					  int access, int dmasync) {
	return ERR_PTR(-EINVAL);
}
static inline void ib_umem_release(struct ib_umem *umem) { }
static inline int ib_umem_page_count(struct ib_umem *umem) { return 0; }
static inline int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      		    size_t length) {
	return -EINVAL;
}
#endif /* CONFIG_INFINIBAND_USER_MEM */

=======
static inline struct ib_umem *ib_umem_get(struct ib_device *device,
					  unsigned long addr, size_t size,
					  int access)
{
	return ERR_PTR(-EOPNOTSUPP);
}
static inline void ib_umem_release(struct ib_umem *umem) { }
static inline int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      		    size_t length) {
	return -EOPNOTSUPP;
}
static inline unsigned long ib_umem_find_best_pgsz(struct ib_umem *umem,
						   unsigned long pgsz_bitmap,
						   unsigned long virt)
{
	return 0;
}
static inline unsigned long ib_umem_find_best_pgoff(struct ib_umem *umem,
						    unsigned long pgsz_bitmap,
						    u64 pgoff_bitmask)
{
	return 0;
}
static inline
struct ib_umem_dmabuf *ib_umem_dmabuf_get(struct ib_device *device,
					  unsigned long offset,
					  size_t size, int fd,
					  int access,
					  struct dma_buf_attach_ops *ops)
{
	return ERR_PTR(-EOPNOTSUPP);
}
static inline int ib_umem_dmabuf_map_pages(struct ib_umem_dmabuf *umem_dmabuf)
{
	return -EOPNOTSUPP;
}
static inline void ib_umem_dmabuf_unmap_pages(struct ib_umem_dmabuf *umem_dmabuf) { }
static inline void ib_umem_dmabuf_release(struct ib_umem_dmabuf *umem_dmabuf) { }

#endif /* CONFIG_INFINIBAND_USER_MEM */
>>>>>>> upstream/android-13
#endif /* IB_UMEM_H */
