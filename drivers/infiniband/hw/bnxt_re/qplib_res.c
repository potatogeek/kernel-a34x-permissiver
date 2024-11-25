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
 * Description: QPLib resource manager
 */

<<<<<<< HEAD
=======
#define dev_fmt(fmt) "QPLIB: " fmt

>>>>>>> upstream/android-13
#include <linux/spinlock.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/inetdevice.h>
#include <linux/dma-mapping.h>
#include <linux/if_vlan.h>
<<<<<<< HEAD
=======
#include <linux/vmalloc.h>
#include <rdma/ib_verbs.h>
#include <rdma/ib_umem.h>

>>>>>>> upstream/android-13
#include "roce_hsi.h"
#include "qplib_res.h"
#include "qplib_sp.h"
#include "qplib_rcfw.h"

static void bnxt_qplib_free_stats_ctx(struct pci_dev *pdev,
				      struct bnxt_qplib_stats *stats);
static int bnxt_qplib_alloc_stats_ctx(struct pci_dev *pdev,
<<<<<<< HEAD
				      struct bnxt_qplib_stats *stats);

/* PBL */
static void __free_pbl(struct pci_dev *pdev, struct bnxt_qplib_pbl *pbl,
		       bool is_umem)
{
=======
				      struct bnxt_qplib_chip_ctx *cctx,
				      struct bnxt_qplib_stats *stats);

/* PBL */
static void __free_pbl(struct bnxt_qplib_res *res, struct bnxt_qplib_pbl *pbl,
		       bool is_umem)
{
	struct pci_dev *pdev = res->pdev;
>>>>>>> upstream/android-13
	int i;

	if (!is_umem) {
		for (i = 0; i < pbl->pg_count; i++) {
			if (pbl->pg_arr[i])
				dma_free_coherent(&pdev->dev, pbl->pg_size,
						  (void *)((unsigned long)
						   pbl->pg_arr[i] &
						  PAGE_MASK),
						  pbl->pg_map_arr[i]);
			else
				dev_warn(&pdev->dev,
<<<<<<< HEAD
					 "QPLIB: PBL free pg_arr[%d] empty?!",
					 i);
			pbl->pg_arr[i] = NULL;
		}
	}
	kfree(pbl->pg_arr);
	pbl->pg_arr = NULL;
	kfree(pbl->pg_map_arr);
=======
					 "PBL free pg_arr[%d] empty?!\n", i);
			pbl->pg_arr[i] = NULL;
		}
	}
	vfree(pbl->pg_arr);
	pbl->pg_arr = NULL;
	vfree(pbl->pg_map_arr);
>>>>>>> upstream/android-13
	pbl->pg_map_arr = NULL;
	pbl->pg_count = 0;
	pbl->pg_size = 0;
}

<<<<<<< HEAD
static int __alloc_pbl(struct pci_dev *pdev, struct bnxt_qplib_pbl *pbl,
		       struct scatterlist *sghead, u32 pages, u32 pg_size)
{
	struct scatterlist *sg;
	bool is_umem = false;
	int i;

	/* page ptr arrays */
	pbl->pg_arr = kcalloc(pages, sizeof(void *), GFP_KERNEL);
	if (!pbl->pg_arr)
		return -ENOMEM;

	pbl->pg_map_arr = kcalloc(pages, sizeof(dma_addr_t), GFP_KERNEL);
	if (!pbl->pg_map_arr) {
		kfree(pbl->pg_arr);
=======
static void bnxt_qplib_fill_user_dma_pages(struct bnxt_qplib_pbl *pbl,
					   struct bnxt_qplib_sg_info *sginfo)
{
	struct ib_block_iter biter;
	int i = 0;

	rdma_umem_for_each_dma_block(sginfo->umem, &biter, sginfo->pgsize) {
		pbl->pg_map_arr[i] = rdma_block_iter_dma_address(&biter);
		pbl->pg_arr[i] = NULL;
		pbl->pg_count++;
		i++;
	}
}

static int __alloc_pbl(struct bnxt_qplib_res *res,
		       struct bnxt_qplib_pbl *pbl,
		       struct bnxt_qplib_sg_info *sginfo)
{
	struct pci_dev *pdev = res->pdev;
	bool is_umem = false;
	u32 pages;
	int i;

	if (sginfo->nopte)
		return 0;
	if (sginfo->umem)
		pages = ib_umem_num_dma_blocks(sginfo->umem, sginfo->pgsize);
	else
		pages = sginfo->npages;
	/* page ptr arrays */
	pbl->pg_arr = vmalloc(pages * sizeof(void *));
	if (!pbl->pg_arr)
		return -ENOMEM;

	pbl->pg_map_arr = vmalloc(pages * sizeof(dma_addr_t));
	if (!pbl->pg_map_arr) {
		vfree(pbl->pg_arr);
>>>>>>> upstream/android-13
		pbl->pg_arr = NULL;
		return -ENOMEM;
	}
	pbl->pg_count = 0;
<<<<<<< HEAD
	pbl->pg_size = pg_size;

	if (!sghead) {
		for (i = 0; i < pages; i++) {
			pbl->pg_arr[i] = dma_zalloc_coherent(&pdev->dev,
							     pbl->pg_size,
							     &pbl->pg_map_arr[i],
							     GFP_KERNEL);
=======
	pbl->pg_size = sginfo->pgsize;

	if (!sginfo->umem) {
		for (i = 0; i < pages; i++) {
			pbl->pg_arr[i] = dma_alloc_coherent(&pdev->dev,
							    pbl->pg_size,
							    &pbl->pg_map_arr[i],
							    GFP_KERNEL);
>>>>>>> upstream/android-13
			if (!pbl->pg_arr[i])
				goto fail;
			pbl->pg_count++;
		}
	} else {
<<<<<<< HEAD
		i = 0;
		is_umem = true;
		for_each_sg(sghead, sg, pages, i) {
			pbl->pg_map_arr[i] = sg_dma_address(sg);
			pbl->pg_arr[i] = sg_virt(sg);
			if (!pbl->pg_arr[i])
				goto fail;

			pbl->pg_count++;
		}
	}

	return 0;

fail:
	__free_pbl(pdev, pbl, is_umem);
=======
		is_umem = true;
		bnxt_qplib_fill_user_dma_pages(pbl, sginfo);
	}

	return 0;
fail:
	__free_pbl(res, pbl, is_umem);
>>>>>>> upstream/android-13
	return -ENOMEM;
}

/* HWQ */
<<<<<<< HEAD
void bnxt_qplib_free_hwq(struct pci_dev *pdev, struct bnxt_qplib_hwq *hwq)
=======
void bnxt_qplib_free_hwq(struct bnxt_qplib_res *res,
			 struct bnxt_qplib_hwq *hwq)
>>>>>>> upstream/android-13
{
	int i;

	if (!hwq->max_elements)
		return;
	if (hwq->level >= PBL_LVL_MAX)
		return;

	for (i = 0; i < hwq->level + 1; i++) {
		if (i == hwq->level)
<<<<<<< HEAD
			__free_pbl(pdev, &hwq->pbl[i], hwq->is_user);
		else
			__free_pbl(pdev, &hwq->pbl[i], false);
=======
			__free_pbl(res, &hwq->pbl[i], hwq->is_user);
		else
			__free_pbl(res, &hwq->pbl[i], false);
>>>>>>> upstream/android-13
	}

	hwq->level = PBL_LVL_MAX;
	hwq->max_elements = 0;
	hwq->element_size = 0;
	hwq->prod = 0;
	hwq->cons = 0;
	hwq->cp_bit = 0;
}

/* All HWQs are power of 2 in size */
<<<<<<< HEAD
int bnxt_qplib_alloc_init_hwq(struct pci_dev *pdev, struct bnxt_qplib_hwq *hwq,
			      struct scatterlist *sghead, int nmap,
			      u32 *elements, u32 element_size, u32 aux,
			      u32 pg_size, enum bnxt_qplib_hwq_type hwq_type)
{
	u32 pages, slots, size, aux_pages = 0, aux_size = 0;
	dma_addr_t *src_phys_ptr, **dst_virt_ptr;
	int i, rc;

	hwq->level = PBL_LVL_MAX;

	slots = roundup_pow_of_two(*elements);
	if (aux) {
		aux_size = roundup_pow_of_two(aux);
		aux_pages = (slots * aux_size) / pg_size;
		if ((slots * aux_size) % pg_size)
			aux_pages++;
	}
	size = roundup_pow_of_two(element_size);

	if (!sghead) {
		hwq->is_user = false;
		pages = (slots * size) / pg_size + aux_pages;
		if ((slots * size) % pg_size)
			pages++;
		if (!pages)
			return -EINVAL;
	} else {
		hwq->is_user = true;
		pages = nmap;
	}

	/* Alloc the 1st memory block; can be a PDL/PTL/PBL */
	if (sghead && (pages == MAX_PBL_LVL_0_PGS))
		rc = __alloc_pbl(pdev, &hwq->pbl[PBL_LVL_0], sghead,
				 pages, pg_size);
	else
		rc = __alloc_pbl(pdev, &hwq->pbl[PBL_LVL_0], NULL, 1, pg_size);
	if (rc)
		goto fail;

	hwq->level = PBL_LVL_0;

	if (pages > MAX_PBL_LVL_0_PGS) {
		if (pages > MAX_PBL_LVL_1_PGS) {
			/* 2 levels of indirection */
			rc = __alloc_pbl(pdev, &hwq->pbl[PBL_LVL_1], NULL,
					 MAX_PBL_LVL_1_PGS_FOR_LVL_2, pg_size);
			if (rc)
				goto fail;
			/* Fill in lvl0 PBL */
			dst_virt_ptr =
				(dma_addr_t **)hwq->pbl[PBL_LVL_0].pg_arr;
			src_phys_ptr = hwq->pbl[PBL_LVL_1].pg_map_arr;
			for (i = 0; i < hwq->pbl[PBL_LVL_1].pg_count; i++)
				dst_virt_ptr[PTR_PG(i)][PTR_IDX(i)] =
					src_phys_ptr[i] | PTU_PDE_VALID;
			hwq->level = PBL_LVL_1;

			rc = __alloc_pbl(pdev, &hwq->pbl[PBL_LVL_2], sghead,
					 pages, pg_size);
			if (rc)
				goto fail;

			/* Fill in lvl1 PBL */
=======

int bnxt_qplib_alloc_init_hwq(struct bnxt_qplib_hwq *hwq,
			      struct bnxt_qplib_hwq_attr *hwq_attr)
{
	u32 npages, aux_slots, pg_size, aux_pages = 0, aux_size = 0;
	struct bnxt_qplib_sg_info sginfo = {};
	u32 depth, stride, npbl, npde;
	dma_addr_t *src_phys_ptr, **dst_virt_ptr;
	struct bnxt_qplib_res *res;
	struct pci_dev *pdev;
	int i, rc, lvl;

	res = hwq_attr->res;
	pdev = res->pdev;
	pg_size = hwq_attr->sginfo->pgsize;
	hwq->level = PBL_LVL_MAX;

	depth = roundup_pow_of_two(hwq_attr->depth);
	stride = roundup_pow_of_two(hwq_attr->stride);
	if (hwq_attr->aux_depth) {
		aux_slots = hwq_attr->aux_depth;
		aux_size = roundup_pow_of_two(hwq_attr->aux_stride);
		aux_pages = (aux_slots * aux_size) / pg_size;
		if ((aux_slots * aux_size) % pg_size)
			aux_pages++;
	}

	if (!hwq_attr->sginfo->umem) {
		hwq->is_user = false;
		npages = (depth * stride) / pg_size + aux_pages;
		if ((depth * stride) % pg_size)
			npages++;
		if (!npages)
			return -EINVAL;
		hwq_attr->sginfo->npages = npages;
	} else {
		unsigned long sginfo_num_pages = ib_umem_num_dma_blocks(
			hwq_attr->sginfo->umem, hwq_attr->sginfo->pgsize);

		hwq->is_user = true;
		npages = sginfo_num_pages;
		npages = (npages * PAGE_SIZE) /
			  BIT_ULL(hwq_attr->sginfo->pgshft);
		if ((sginfo_num_pages * PAGE_SIZE) %
		     BIT_ULL(hwq_attr->sginfo->pgshft))
			if (!npages)
				npages++;
	}

	if (npages == MAX_PBL_LVL_0_PGS) {
		/* This request is Level 0, map PTE */
		rc = __alloc_pbl(res, &hwq->pbl[PBL_LVL_0], hwq_attr->sginfo);
		if (rc)
			goto fail;
		hwq->level = PBL_LVL_0;
	}

	if (npages > MAX_PBL_LVL_0_PGS) {
		if (npages > MAX_PBL_LVL_1_PGS) {
			u32 flag = (hwq_attr->type == HWQ_TYPE_L2_CMPL) ?
				    0 : PTU_PTE_VALID;
			/* 2 levels of indirection */
			npbl = npages >> MAX_PBL_LVL_1_PGS_SHIFT;
			if (npages % BIT(MAX_PBL_LVL_1_PGS_SHIFT))
				npbl++;
			npde = npbl >> MAX_PDL_LVL_SHIFT;
			if (npbl % BIT(MAX_PDL_LVL_SHIFT))
				npde++;
			/* Alloc PDE pages */
			sginfo.pgsize = npde * pg_size;
			sginfo.npages = 1;
			rc = __alloc_pbl(res, &hwq->pbl[PBL_LVL_0], &sginfo);

			/* Alloc PBL pages */
			sginfo.npages = npbl;
			sginfo.pgsize = PAGE_SIZE;
			rc = __alloc_pbl(res, &hwq->pbl[PBL_LVL_1], &sginfo);
			if (rc)
				goto fail;
			/* Fill PDL with PBL page pointers */
			dst_virt_ptr =
				(dma_addr_t **)hwq->pbl[PBL_LVL_0].pg_arr;
			src_phys_ptr = hwq->pbl[PBL_LVL_1].pg_map_arr;
			if (hwq_attr->type == HWQ_TYPE_MR) {
			/* For MR it is expected that we supply only 1 contigous
			 * page i.e only 1 entry in the PDL that will contain
			 * all the PBLs for the user supplied memory region
			 */
				for (i = 0; i < hwq->pbl[PBL_LVL_1].pg_count;
				     i++)
					dst_virt_ptr[0][i] = src_phys_ptr[i] |
						flag;
			} else {
				for (i = 0; i < hwq->pbl[PBL_LVL_1].pg_count;
				     i++)
					dst_virt_ptr[PTR_PG(i)][PTR_IDX(i)] =
						src_phys_ptr[i] |
						PTU_PDE_VALID;
			}
			/* Alloc or init PTEs */
			rc = __alloc_pbl(res, &hwq->pbl[PBL_LVL_2],
					 hwq_attr->sginfo);
			if (rc)
				goto fail;
			hwq->level = PBL_LVL_2;
			if (hwq_attr->sginfo->nopte)
				goto done;
			/* Fill PBLs with PTE pointers */
>>>>>>> upstream/android-13
			dst_virt_ptr =
				(dma_addr_t **)hwq->pbl[PBL_LVL_1].pg_arr;
			src_phys_ptr = hwq->pbl[PBL_LVL_2].pg_map_arr;
			for (i = 0; i < hwq->pbl[PBL_LVL_2].pg_count; i++) {
				dst_virt_ptr[PTR_PG(i)][PTR_IDX(i)] =
					src_phys_ptr[i] | PTU_PTE_VALID;
			}
<<<<<<< HEAD
			if (hwq_type == HWQ_TYPE_QUEUE) {
=======
			if (hwq_attr->type == HWQ_TYPE_QUEUE) {
>>>>>>> upstream/android-13
				/* Find the last pg of the size */
				i = hwq->pbl[PBL_LVL_2].pg_count;
				dst_virt_ptr[PTR_PG(i - 1)][PTR_IDX(i - 1)] |=
								  PTU_PTE_LAST;
				if (i > 1)
					dst_virt_ptr[PTR_PG(i - 2)]
						    [PTR_IDX(i - 2)] |=
						    PTU_PTE_NEXT_TO_LAST;
			}
<<<<<<< HEAD
			hwq->level = PBL_LVL_2;
		} else {
			u32 flag = hwq_type == HWQ_TYPE_L2_CMPL ? 0 :
						PTU_PTE_VALID;

			/* 1 level of indirection */
			rc = __alloc_pbl(pdev, &hwq->pbl[PBL_LVL_1], sghead,
					 pages, pg_size);
			if (rc)
				goto fail;
			/* Fill in lvl0 PBL */
			dst_virt_ptr =
				(dma_addr_t **)hwq->pbl[PBL_LVL_0].pg_arr;
			src_phys_ptr = hwq->pbl[PBL_LVL_1].pg_map_arr;
			for (i = 0; i < hwq->pbl[PBL_LVL_1].pg_count; i++) {
				dst_virt_ptr[PTR_PG(i)][PTR_IDX(i)] =
					src_phys_ptr[i] | flag;
			}
			if (hwq_type == HWQ_TYPE_QUEUE) {
=======
		} else { /* pages < 512 npbl = 1, npde = 0 */
			u32 flag = (hwq_attr->type == HWQ_TYPE_L2_CMPL) ?
				    0 : PTU_PTE_VALID;

			/* 1 level of indirection */
			npbl = npages >> MAX_PBL_LVL_1_PGS_SHIFT;
			if (npages % BIT(MAX_PBL_LVL_1_PGS_SHIFT))
				npbl++;
			sginfo.npages = npbl;
			sginfo.pgsize = PAGE_SIZE;
			/* Alloc PBL page */
			rc = __alloc_pbl(res, &hwq->pbl[PBL_LVL_0], &sginfo);
			if (rc)
				goto fail;
			/* Alloc or init  PTEs */
			rc = __alloc_pbl(res, &hwq->pbl[PBL_LVL_1],
					 hwq_attr->sginfo);
			if (rc)
				goto fail;
			hwq->level = PBL_LVL_1;
			if (hwq_attr->sginfo->nopte)
				goto done;
			/* Fill PBL with PTE pointers */
			dst_virt_ptr =
				(dma_addr_t **)hwq->pbl[PBL_LVL_0].pg_arr;
			src_phys_ptr = hwq->pbl[PBL_LVL_1].pg_map_arr;
			for (i = 0; i < hwq->pbl[PBL_LVL_1].pg_count; i++)
				dst_virt_ptr[PTR_PG(i)][PTR_IDX(i)] =
					src_phys_ptr[i] | flag;
			if (hwq_attr->type == HWQ_TYPE_QUEUE) {
>>>>>>> upstream/android-13
				/* Find the last pg of the size */
				i = hwq->pbl[PBL_LVL_1].pg_count;
				dst_virt_ptr[PTR_PG(i - 1)][PTR_IDX(i - 1)] |=
								  PTU_PTE_LAST;
				if (i > 1)
					dst_virt_ptr[PTR_PG(i - 2)]
						    [PTR_IDX(i - 2)] |=
						    PTU_PTE_NEXT_TO_LAST;
			}
<<<<<<< HEAD
			hwq->level = PBL_LVL_1;
		}
	}
	hwq->pdev = pdev;
	spin_lock_init(&hwq->lock);
	hwq->prod = 0;
	hwq->cons = 0;
	*elements = hwq->max_elements = slots;
	hwq->element_size = size;

	/* For direct access to the elements */
	hwq->pbl_ptr = hwq->pbl[hwq->level].pg_arr;
	hwq->pbl_dma_ptr = hwq->pbl[hwq->level].pg_map_arr;

	return 0;

fail:
	bnxt_qplib_free_hwq(pdev, hwq);
=======
		}
	}
done:
	hwq->prod = 0;
	hwq->cons = 0;
	hwq->pdev = pdev;
	hwq->depth = hwq_attr->depth;
	hwq->max_elements = depth;
	hwq->element_size = stride;
	hwq->qe_ppg = pg_size / stride;
	/* For direct access to the elements */
	lvl = hwq->level;
	if (hwq_attr->sginfo->nopte && hwq->level)
		lvl = hwq->level - 1;
	hwq->pbl_ptr = hwq->pbl[lvl].pg_arr;
	hwq->pbl_dma_ptr = hwq->pbl[lvl].pg_map_arr;
	spin_lock_init(&hwq->lock);

	return 0;
fail:
	bnxt_qplib_free_hwq(res, hwq);
>>>>>>> upstream/android-13
	return -ENOMEM;
}

/* Context Tables */
<<<<<<< HEAD
void bnxt_qplib_free_ctx(struct pci_dev *pdev,
=======
void bnxt_qplib_free_ctx(struct bnxt_qplib_res *res,
>>>>>>> upstream/android-13
			 struct bnxt_qplib_ctx *ctx)
{
	int i;

<<<<<<< HEAD
	bnxt_qplib_free_hwq(pdev, &ctx->qpc_tbl);
	bnxt_qplib_free_hwq(pdev, &ctx->mrw_tbl);
	bnxt_qplib_free_hwq(pdev, &ctx->srqc_tbl);
	bnxt_qplib_free_hwq(pdev, &ctx->cq_tbl);
	bnxt_qplib_free_hwq(pdev, &ctx->tim_tbl);
	for (i = 0; i < MAX_TQM_ALLOC_REQ; i++)
		bnxt_qplib_free_hwq(pdev, &ctx->tqm_tbl[i]);
	bnxt_qplib_free_hwq(pdev, &ctx->tqm_pde);
	bnxt_qplib_free_stats_ctx(pdev, &ctx->stats);
=======
	bnxt_qplib_free_hwq(res, &ctx->qpc_tbl);
	bnxt_qplib_free_hwq(res, &ctx->mrw_tbl);
	bnxt_qplib_free_hwq(res, &ctx->srqc_tbl);
	bnxt_qplib_free_hwq(res, &ctx->cq_tbl);
	bnxt_qplib_free_hwq(res, &ctx->tim_tbl);
	for (i = 0; i < MAX_TQM_ALLOC_REQ; i++)
		bnxt_qplib_free_hwq(res, &ctx->tqm_ctx.qtbl[i]);
	/* restore original pde level before destroy */
	ctx->tqm_ctx.pde.level = ctx->tqm_ctx.pde_level;
	bnxt_qplib_free_hwq(res, &ctx->tqm_ctx.pde);
	bnxt_qplib_free_stats_ctx(res->pdev, &ctx->stats);
}

static int bnxt_qplib_alloc_tqm_rings(struct bnxt_qplib_res *res,
				      struct bnxt_qplib_ctx *ctx)
{
	struct bnxt_qplib_hwq_attr hwq_attr = {};
	struct bnxt_qplib_sg_info sginfo = {};
	struct bnxt_qplib_tqm_ctx *tqmctx;
	int rc = 0;
	int i;

	tqmctx = &ctx->tqm_ctx;

	sginfo.pgsize = PAGE_SIZE;
	sginfo.pgshft = PAGE_SHIFT;
	hwq_attr.sginfo = &sginfo;
	hwq_attr.res = res;
	hwq_attr.type = HWQ_TYPE_CTX;
	hwq_attr.depth = 512;
	hwq_attr.stride = sizeof(u64);
	/* Alloc pdl buffer */
	rc = bnxt_qplib_alloc_init_hwq(&tqmctx->pde, &hwq_attr);
	if (rc)
		goto out;
	/* Save original pdl level */
	tqmctx->pde_level = tqmctx->pde.level;

	hwq_attr.stride = 1;
	for (i = 0; i < MAX_TQM_ALLOC_REQ; i++) {
		if (!tqmctx->qcount[i])
			continue;
		hwq_attr.depth = ctx->qpc_count * tqmctx->qcount[i];
		rc = bnxt_qplib_alloc_init_hwq(&tqmctx->qtbl[i], &hwq_attr);
		if (rc)
			goto out;
	}
out:
	return rc;
}

static void bnxt_qplib_map_tqm_pgtbl(struct bnxt_qplib_tqm_ctx *ctx)
{
	struct bnxt_qplib_hwq *tbl;
	dma_addr_t *dma_ptr;
	__le64 **pbl_ptr, *ptr;
	int i, j, k;
	int fnz_idx = -1;
	int pg_count;

	pbl_ptr = (__le64 **)ctx->pde.pbl_ptr;

	for (i = 0, j = 0; i < MAX_TQM_ALLOC_REQ;
	     i++, j += MAX_TQM_ALLOC_BLK_SIZE) {
		tbl = &ctx->qtbl[i];
		if (!tbl->max_elements)
			continue;
		if (fnz_idx == -1)
			fnz_idx = i; /* first non-zero index */
		switch (tbl->level) {
		case PBL_LVL_2:
			pg_count = tbl->pbl[PBL_LVL_1].pg_count;
			for (k = 0; k < pg_count; k++) {
				ptr = &pbl_ptr[PTR_PG(j + k)][PTR_IDX(j + k)];
				dma_ptr = &tbl->pbl[PBL_LVL_1].pg_map_arr[k];
				*ptr = cpu_to_le64(*dma_ptr | PTU_PTE_VALID);
			}
			break;
		case PBL_LVL_1:
		case PBL_LVL_0:
		default:
			ptr = &pbl_ptr[PTR_PG(j)][PTR_IDX(j)];
			*ptr = cpu_to_le64(tbl->pbl[PBL_LVL_0].pg_map_arr[0] |
					   PTU_PTE_VALID);
			break;
		}
	}
	if (fnz_idx == -1)
		fnz_idx = 0;
	/* update pde level as per page table programming */
	ctx->pde.level = (ctx->qtbl[fnz_idx].level == PBL_LVL_2) ? PBL_LVL_2 :
			  ctx->qtbl[fnz_idx].level + 1;
}

static int bnxt_qplib_setup_tqm_rings(struct bnxt_qplib_res *res,
				      struct bnxt_qplib_ctx *ctx)
{
	int rc = 0;

	rc = bnxt_qplib_alloc_tqm_rings(res, ctx);
	if (rc)
		goto fail;

	bnxt_qplib_map_tqm_pgtbl(&ctx->tqm_ctx);
fail:
	return rc;
>>>>>>> upstream/android-13
}

/*
 * Routine: bnxt_qplib_alloc_ctx
 * Description:
 *     Context tables are memories which are used by the chip fw.
 *     The 6 tables defined are:
 *             QPC ctx - holds QP states
 *             MRW ctx - holds memory region and window
 *             SRQ ctx - holds shared RQ states
 *             CQ ctx - holds completion queue states
 *             TQM ctx - holds Tx Queue Manager context
 *             TIM ctx - holds timer context
 *     Depending on the size of the tbl requested, either a 1 Page Buffer List
 *     or a 1-to-2-stage indirection Page Directory List + 1 PBL is used
 *     instead.
 *     Table might be employed as follows:
 *             For 0      < ctx size <= 1 PAGE, 0 level of ind is used
 *             For 1 PAGE < ctx size <= 512 entries size, 1 level of ind is used
 *             For 512    < ctx size <= MAX, 2 levels of ind is used
 * Returns:
 *     0 if success, else -ERRORS
 */
<<<<<<< HEAD
int bnxt_qplib_alloc_ctx(struct pci_dev *pdev,
			 struct bnxt_qplib_ctx *ctx,
			 bool virt_fn)
{
	int i, j, k, rc = 0;
	int fnz_idx = -1;
	__le64 **pbl_ptr;

	if (virt_fn)
		goto stats_alloc;

	/* QPC Tables */
	ctx->qpc_tbl.max_elements = ctx->qpc_count;
	rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->qpc_tbl, NULL, 0,
				       &ctx->qpc_tbl.max_elements,
				       BNXT_QPLIB_MAX_QP_CTX_ENTRY_SIZE, 0,
				       PAGE_SIZE, HWQ_TYPE_CTX);
=======
int bnxt_qplib_alloc_ctx(struct bnxt_qplib_res *res,
			 struct bnxt_qplib_ctx *ctx,
			 bool virt_fn, bool is_p5)
{
	struct bnxt_qplib_hwq_attr hwq_attr = {};
	struct bnxt_qplib_sg_info sginfo = {};
	int rc = 0;

	if (virt_fn || is_p5)
		goto stats_alloc;

	/* QPC Tables */
	sginfo.pgsize = PAGE_SIZE;
	sginfo.pgshft = PAGE_SHIFT;
	hwq_attr.sginfo = &sginfo;

	hwq_attr.res = res;
	hwq_attr.depth = ctx->qpc_count;
	hwq_attr.stride = BNXT_QPLIB_MAX_QP_CTX_ENTRY_SIZE;
	hwq_attr.type = HWQ_TYPE_CTX;
	rc = bnxt_qplib_alloc_init_hwq(&ctx->qpc_tbl, &hwq_attr);
>>>>>>> upstream/android-13
	if (rc)
		goto fail;

	/* MRW Tables */
<<<<<<< HEAD
	ctx->mrw_tbl.max_elements = ctx->mrw_count;
	rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->mrw_tbl, NULL, 0,
				       &ctx->mrw_tbl.max_elements,
				       BNXT_QPLIB_MAX_MRW_CTX_ENTRY_SIZE, 0,
				       PAGE_SIZE, HWQ_TYPE_CTX);
=======
	hwq_attr.depth = ctx->mrw_count;
	hwq_attr.stride = BNXT_QPLIB_MAX_MRW_CTX_ENTRY_SIZE;
	rc = bnxt_qplib_alloc_init_hwq(&ctx->mrw_tbl, &hwq_attr);
>>>>>>> upstream/android-13
	if (rc)
		goto fail;

	/* SRQ Tables */
<<<<<<< HEAD
	ctx->srqc_tbl.max_elements = ctx->srqc_count;
	rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->srqc_tbl, NULL, 0,
				       &ctx->srqc_tbl.max_elements,
				       BNXT_QPLIB_MAX_SRQ_CTX_ENTRY_SIZE, 0,
				       PAGE_SIZE, HWQ_TYPE_CTX);
=======
	hwq_attr.depth = ctx->srqc_count;
	hwq_attr.stride = BNXT_QPLIB_MAX_SRQ_CTX_ENTRY_SIZE;
	rc = bnxt_qplib_alloc_init_hwq(&ctx->srqc_tbl, &hwq_attr);
>>>>>>> upstream/android-13
	if (rc)
		goto fail;

	/* CQ Tables */
<<<<<<< HEAD
	ctx->cq_tbl.max_elements = ctx->cq_count;
	rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->cq_tbl, NULL, 0,
				       &ctx->cq_tbl.max_elements,
				       BNXT_QPLIB_MAX_CQ_CTX_ENTRY_SIZE, 0,
				       PAGE_SIZE, HWQ_TYPE_CTX);
=======
	hwq_attr.depth = ctx->cq_count;
	hwq_attr.stride = BNXT_QPLIB_MAX_CQ_CTX_ENTRY_SIZE;
	rc = bnxt_qplib_alloc_init_hwq(&ctx->cq_tbl, &hwq_attr);
>>>>>>> upstream/android-13
	if (rc)
		goto fail;

	/* TQM Buffer */
<<<<<<< HEAD
	ctx->tqm_pde.max_elements = 512;
	rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->tqm_pde, NULL, 0,
				       &ctx->tqm_pde.max_elements, sizeof(u64),
				       0, PAGE_SIZE, HWQ_TYPE_CTX);
	if (rc)
		goto fail;

	for (i = 0; i < MAX_TQM_ALLOC_REQ; i++) {
		if (!ctx->tqm_count[i])
			continue;
		ctx->tqm_tbl[i].max_elements = ctx->qpc_count *
					       ctx->tqm_count[i];
		rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->tqm_tbl[i], NULL, 0,
					       &ctx->tqm_tbl[i].max_elements, 1,
					       0, PAGE_SIZE, HWQ_TYPE_CTX);
		if (rc)
			goto fail;
	}
	pbl_ptr = (__le64 **)ctx->tqm_pde.pbl_ptr;
	for (i = 0, j = 0; i < MAX_TQM_ALLOC_REQ;
	     i++, j += MAX_TQM_ALLOC_BLK_SIZE) {
		if (!ctx->tqm_tbl[i].max_elements)
			continue;
		if (fnz_idx == -1)
			fnz_idx = i;
		switch (ctx->tqm_tbl[i].level) {
		case PBL_LVL_2:
			for (k = 0; k < ctx->tqm_tbl[i].pbl[PBL_LVL_1].pg_count;
			     k++)
				pbl_ptr[PTR_PG(j + k)][PTR_IDX(j + k)] =
				  cpu_to_le64(
				    ctx->tqm_tbl[i].pbl[PBL_LVL_1].pg_map_arr[k]
				    | PTU_PTE_VALID);
			break;
		case PBL_LVL_1:
		case PBL_LVL_0:
		default:
			pbl_ptr[PTR_PG(j)][PTR_IDX(j)] = cpu_to_le64(
				ctx->tqm_tbl[i].pbl[PBL_LVL_0].pg_map_arr[0] |
				PTU_PTE_VALID);
			break;
		}
	}
	if (fnz_idx == -1)
		fnz_idx = 0;
	ctx->tqm_pde_level = ctx->tqm_tbl[fnz_idx].level == PBL_LVL_2 ?
			     PBL_LVL_2 : ctx->tqm_tbl[fnz_idx].level + 1;

	/* TIM Buffer */
	ctx->tim_tbl.max_elements = ctx->qpc_count * 16;
	rc = bnxt_qplib_alloc_init_hwq(pdev, &ctx->tim_tbl, NULL, 0,
				       &ctx->tim_tbl.max_elements, 1,
				       0, PAGE_SIZE, HWQ_TYPE_CTX);
	if (rc)
		goto fail;

stats_alloc:
	/* Stats */
	rc = bnxt_qplib_alloc_stats_ctx(pdev, &ctx->stats);
=======
	rc = bnxt_qplib_setup_tqm_rings(res, ctx);
	if (rc)
		goto fail;
	/* TIM Buffer */
	ctx->tim_tbl.max_elements = ctx->qpc_count * 16;
	hwq_attr.depth = ctx->qpc_count * 16;
	hwq_attr.stride = 1;
	rc = bnxt_qplib_alloc_init_hwq(&ctx->tim_tbl, &hwq_attr);
	if (rc)
		goto fail;
stats_alloc:
	/* Stats */
	rc = bnxt_qplib_alloc_stats_ctx(res->pdev, res->cctx, &ctx->stats);
>>>>>>> upstream/android-13
	if (rc)
		goto fail;

	return 0;

fail:
<<<<<<< HEAD
	bnxt_qplib_free_ctx(pdev, ctx);
=======
	bnxt_qplib_free_ctx(res, ctx);
>>>>>>> upstream/android-13
	return rc;
}

/* GUID */
void bnxt_qplib_get_guid(u8 *dev_addr, u8 *guid)
{
	u8 mac[ETH_ALEN];

	/* MAC-48 to EUI-64 mapping */
	memcpy(mac, dev_addr, ETH_ALEN);
	guid[0] = mac[0] ^ 2;
	guid[1] = mac[1];
	guid[2] = mac[2];
	guid[3] = 0xff;
	guid[4] = 0xfe;
	guid[5] = mac[3];
	guid[6] = mac[4];
	guid[7] = mac[5];
}

static void bnxt_qplib_free_sgid_tbl(struct bnxt_qplib_res *res,
				     struct bnxt_qplib_sgid_tbl *sgid_tbl)
{
	kfree(sgid_tbl->tbl);
	kfree(sgid_tbl->hw_id);
	kfree(sgid_tbl->ctx);
	kfree(sgid_tbl->vlan);
	sgid_tbl->tbl = NULL;
	sgid_tbl->hw_id = NULL;
	sgid_tbl->ctx = NULL;
	sgid_tbl->vlan = NULL;
	sgid_tbl->max = 0;
	sgid_tbl->active = 0;
}

static int bnxt_qplib_alloc_sgid_tbl(struct bnxt_qplib_res *res,
				     struct bnxt_qplib_sgid_tbl *sgid_tbl,
				     u16 max)
{
<<<<<<< HEAD
	sgid_tbl->tbl = kcalloc(max, sizeof(struct bnxt_qplib_gid), GFP_KERNEL);
=======
	sgid_tbl->tbl = kcalloc(max, sizeof(*sgid_tbl->tbl), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!sgid_tbl->tbl)
		return -ENOMEM;

	sgid_tbl->hw_id = kcalloc(max, sizeof(u16), GFP_KERNEL);
	if (!sgid_tbl->hw_id)
		goto out_free1;

	sgid_tbl->ctx = kcalloc(max, sizeof(void *), GFP_KERNEL);
	if (!sgid_tbl->ctx)
		goto out_free2;

	sgid_tbl->vlan = kcalloc(max, sizeof(u8), GFP_KERNEL);
	if (!sgid_tbl->vlan)
		goto out_free3;

	sgid_tbl->max = max;
	return 0;
out_free3:
	kfree(sgid_tbl->ctx);
	sgid_tbl->ctx = NULL;
out_free2:
	kfree(sgid_tbl->hw_id);
	sgid_tbl->hw_id = NULL;
out_free1:
	kfree(sgid_tbl->tbl);
	sgid_tbl->tbl = NULL;
	return -ENOMEM;
};

static void bnxt_qplib_cleanup_sgid_tbl(struct bnxt_qplib_res *res,
					struct bnxt_qplib_sgid_tbl *sgid_tbl)
{
	int i;

	for (i = 0; i < sgid_tbl->max; i++) {
		if (memcmp(&sgid_tbl->tbl[i], &bnxt_qplib_gid_zero,
			   sizeof(bnxt_qplib_gid_zero)))
<<<<<<< HEAD
			bnxt_qplib_del_sgid(sgid_tbl, &sgid_tbl->tbl[i], true);
	}
	memset(sgid_tbl->tbl, 0, sizeof(struct bnxt_qplib_gid) * sgid_tbl->max);
=======
			bnxt_qplib_del_sgid(sgid_tbl, &sgid_tbl->tbl[i].gid,
					    sgid_tbl->tbl[i].vlan_id, true);
	}
	memset(sgid_tbl->tbl, 0, sizeof(*sgid_tbl->tbl) * sgid_tbl->max);
>>>>>>> upstream/android-13
	memset(sgid_tbl->hw_id, -1, sizeof(u16) * sgid_tbl->max);
	memset(sgid_tbl->vlan, 0, sizeof(u8) * sgid_tbl->max);
	sgid_tbl->active = 0;
}

static void bnxt_qplib_init_sgid_tbl(struct bnxt_qplib_sgid_tbl *sgid_tbl,
				     struct net_device *netdev)
{
<<<<<<< HEAD
	memset(sgid_tbl->tbl, 0, sizeof(struct bnxt_qplib_gid) * sgid_tbl->max);
=======
	u32 i;

	for (i = 0; i < sgid_tbl->max; i++)
		sgid_tbl->tbl[i].vlan_id = 0xffff;

>>>>>>> upstream/android-13
	memset(sgid_tbl->hw_id, -1, sizeof(u16) * sgid_tbl->max);
}

static void bnxt_qplib_free_pkey_tbl(struct bnxt_qplib_res *res,
				     struct bnxt_qplib_pkey_tbl *pkey_tbl)
{
	if (!pkey_tbl->tbl)
<<<<<<< HEAD
		dev_dbg(&res->pdev->dev, "QPLIB: PKEY tbl not present");
=======
		dev_dbg(&res->pdev->dev, "PKEY tbl not present\n");
>>>>>>> upstream/android-13
	else
		kfree(pkey_tbl->tbl);

	pkey_tbl->tbl = NULL;
	pkey_tbl->max = 0;
	pkey_tbl->active = 0;
}

static int bnxt_qplib_alloc_pkey_tbl(struct bnxt_qplib_res *res,
				     struct bnxt_qplib_pkey_tbl *pkey_tbl,
				     u16 max)
{
	pkey_tbl->tbl = kcalloc(max, sizeof(u16), GFP_KERNEL);
	if (!pkey_tbl->tbl)
		return -ENOMEM;

	pkey_tbl->max = max;
	return 0;
};

/* PDs */
int bnxt_qplib_alloc_pd(struct bnxt_qplib_pd_tbl *pdt, struct bnxt_qplib_pd *pd)
{
	u32 bit_num;

	bit_num = find_first_bit(pdt->tbl, pdt->max);
	if (bit_num == pdt->max)
		return -ENOMEM;

	/* Found unused PD */
	clear_bit(bit_num, pdt->tbl);
	pd->id = bit_num;
	return 0;
}

int bnxt_qplib_dealloc_pd(struct bnxt_qplib_res *res,
			  struct bnxt_qplib_pd_tbl *pdt,
			  struct bnxt_qplib_pd *pd)
{
	if (test_and_set_bit(pd->id, pdt->tbl)) {
<<<<<<< HEAD
		dev_warn(&res->pdev->dev, "Freeing an unused PD? pdn = %d",
=======
		dev_warn(&res->pdev->dev, "Freeing an unused PD? pdn = %d\n",
>>>>>>> upstream/android-13
			 pd->id);
		return -EINVAL;
	}
	pd->id = 0;
	return 0;
}

static void bnxt_qplib_free_pd_tbl(struct bnxt_qplib_pd_tbl *pdt)
{
	kfree(pdt->tbl);
	pdt->tbl = NULL;
	pdt->max = 0;
}

static int bnxt_qplib_alloc_pd_tbl(struct bnxt_qplib_res *res,
				   struct bnxt_qplib_pd_tbl *pdt,
				   u32 max)
{
	u32 bytes;

	bytes = max >> 3;
	if (!bytes)
		bytes = 1;
	pdt->tbl = kmalloc(bytes, GFP_KERNEL);
	if (!pdt->tbl)
		return -ENOMEM;

	pdt->max = max;
	memset((u8 *)pdt->tbl, 0xFF, bytes);

	return 0;
}

/* DPIs */
int bnxt_qplib_alloc_dpi(struct bnxt_qplib_dpi_tbl *dpit,
			 struct bnxt_qplib_dpi     *dpi,
			 void                      *app)
{
	u32 bit_num;

	bit_num = find_first_bit(dpit->tbl, dpit->max);
	if (bit_num == dpit->max)
		return -ENOMEM;

	/* Found unused DPI */
	clear_bit(bit_num, dpit->tbl);
	dpit->app_tbl[bit_num] = app;

	dpi->dpi = bit_num;
	dpi->dbr = dpit->dbr_bar_reg_iomem + (bit_num * PAGE_SIZE);
	dpi->umdbr = dpit->unmapped_dbr + (bit_num * PAGE_SIZE);

	return 0;
}

int bnxt_qplib_dealloc_dpi(struct bnxt_qplib_res *res,
			   struct bnxt_qplib_dpi_tbl *dpit,
			   struct bnxt_qplib_dpi     *dpi)
{
	if (dpi->dpi >= dpit->max) {
<<<<<<< HEAD
		dev_warn(&res->pdev->dev, "Invalid DPI? dpi = %d", dpi->dpi);
		return -EINVAL;
	}
	if (test_and_set_bit(dpi->dpi, dpit->tbl)) {
		dev_warn(&res->pdev->dev, "Freeing an unused DPI? dpi = %d",
=======
		dev_warn(&res->pdev->dev, "Invalid DPI? dpi = %d\n", dpi->dpi);
		return -EINVAL;
	}
	if (test_and_set_bit(dpi->dpi, dpit->tbl)) {
		dev_warn(&res->pdev->dev, "Freeing an unused DPI? dpi = %d\n",
>>>>>>> upstream/android-13
			 dpi->dpi);
		return -EINVAL;
	}
	if (dpit->app_tbl)
		dpit->app_tbl[dpi->dpi] = NULL;
	memset(dpi, 0, sizeof(*dpi));

	return 0;
}

static void bnxt_qplib_free_dpi_tbl(struct bnxt_qplib_res     *res,
				    struct bnxt_qplib_dpi_tbl *dpit)
{
	kfree(dpit->tbl);
	kfree(dpit->app_tbl);
	if (dpit->dbr_bar_reg_iomem)
		pci_iounmap(res->pdev, dpit->dbr_bar_reg_iomem);
	memset(dpit, 0, sizeof(*dpit));
}

static int bnxt_qplib_alloc_dpi_tbl(struct bnxt_qplib_res     *res,
				    struct bnxt_qplib_dpi_tbl *dpit,
				    u32                       dbr_offset)
{
	u32 dbr_bar_reg = RCFW_DBR_PCI_BAR_REGION;
	resource_size_t bar_reg_base;
	u32 dbr_len, bytes;

	if (dpit->dbr_bar_reg_iomem) {
<<<<<<< HEAD
		dev_err(&res->pdev->dev,
			"QPLIB: DBR BAR region %d already mapped", dbr_bar_reg);
=======
		dev_err(&res->pdev->dev, "DBR BAR region %d already mapped\n",
			dbr_bar_reg);
>>>>>>> upstream/android-13
		return -EALREADY;
	}

	bar_reg_base = pci_resource_start(res->pdev, dbr_bar_reg);
	if (!bar_reg_base) {
<<<<<<< HEAD
		dev_err(&res->pdev->dev,
			"QPLIB: BAR region %d resc start failed", dbr_bar_reg);
=======
		dev_err(&res->pdev->dev, "BAR region %d resc start failed\n",
			dbr_bar_reg);
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	dbr_len = pci_resource_len(res->pdev, dbr_bar_reg) - dbr_offset;
	if (!dbr_len || ((dbr_len & (PAGE_SIZE - 1)) != 0)) {
<<<<<<< HEAD
		dev_err(&res->pdev->dev, "QPLIB: Invalid DBR length %d",
			dbr_len);
		return -ENOMEM;
	}

	dpit->dbr_bar_reg_iomem = ioremap_nocache(bar_reg_base + dbr_offset,
						  dbr_len);
	if (!dpit->dbr_bar_reg_iomem) {
		dev_err(&res->pdev->dev,
			"QPLIB: FP: DBR BAR region %d mapping failed",
			dbr_bar_reg);
=======
		dev_err(&res->pdev->dev, "Invalid DBR length %d\n", dbr_len);
		return -ENOMEM;
	}

	dpit->dbr_bar_reg_iomem = ioremap(bar_reg_base + dbr_offset,
						  dbr_len);
	if (!dpit->dbr_bar_reg_iomem) {
		dev_err(&res->pdev->dev,
			"FP: DBR BAR region %d mapping failed\n", dbr_bar_reg);
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	dpit->unmapped_dbr = bar_reg_base + dbr_offset;
	dpit->max = dbr_len / PAGE_SIZE;

	dpit->app_tbl = kcalloc(dpit->max, sizeof(void *), GFP_KERNEL);
	if (!dpit->app_tbl)
		goto unmap_io;

	bytes = dpit->max >> 3;
	if (!bytes)
		bytes = 1;

	dpit->tbl = kmalloc(bytes, GFP_KERNEL);
	if (!dpit->tbl) {
		kfree(dpit->app_tbl);
		dpit->app_tbl = NULL;
		goto unmap_io;
	}

	memset((u8 *)dpit->tbl, 0xFF, bytes);

	return 0;

unmap_io:
	pci_iounmap(res->pdev, dpit->dbr_bar_reg_iomem);
	dpit->dbr_bar_reg_iomem = NULL;
	return -ENOMEM;
}

/* PKEYs */
static void bnxt_qplib_cleanup_pkey_tbl(struct bnxt_qplib_pkey_tbl *pkey_tbl)
{
	memset(pkey_tbl->tbl, 0, sizeof(u16) * pkey_tbl->max);
	pkey_tbl->active = 0;
}

static void bnxt_qplib_init_pkey_tbl(struct bnxt_qplib_res *res,
				     struct bnxt_qplib_pkey_tbl *pkey_tbl)
{
	u16 pkey = 0xFFFF;

	memset(pkey_tbl->tbl, 0, sizeof(u16) * pkey_tbl->max);

	/* pkey default = 0xFFFF */
	bnxt_qplib_add_pkey(res, pkey_tbl, &pkey, false);
}

/* Stats */
static void bnxt_qplib_free_stats_ctx(struct pci_dev *pdev,
				      struct bnxt_qplib_stats *stats)
{
	if (stats->dma) {
		dma_free_coherent(&pdev->dev, stats->size,
				  stats->dma, stats->dma_map);
	}
	memset(stats, 0, sizeof(*stats));
	stats->fw_id = -1;
}

static int bnxt_qplib_alloc_stats_ctx(struct pci_dev *pdev,
<<<<<<< HEAD
=======
				      struct bnxt_qplib_chip_ctx *cctx,
>>>>>>> upstream/android-13
				      struct bnxt_qplib_stats *stats)
{
	memset(stats, 0, sizeof(*stats));
	stats->fw_id = -1;
<<<<<<< HEAD
	stats->size = sizeof(struct ctx_hw_stats);
	stats->dma = dma_alloc_coherent(&pdev->dev, stats->size,
					&stats->dma_map, GFP_KERNEL);
	if (!stats->dma) {
		dev_err(&pdev->dev, "QPLIB: Stats DMA allocation failed");
=======
	stats->size = cctx->hw_stats_size;
	stats->dma = dma_alloc_coherent(&pdev->dev, stats->size,
					&stats->dma_map, GFP_KERNEL);
	if (!stats->dma) {
		dev_err(&pdev->dev, "Stats DMA allocation failed\n");
>>>>>>> upstream/android-13
		return -ENOMEM;
	}
	return 0;
}

void bnxt_qplib_cleanup_res(struct bnxt_qplib_res *res)
{
	bnxt_qplib_cleanup_pkey_tbl(&res->pkey_tbl);
	bnxt_qplib_cleanup_sgid_tbl(res, &res->sgid_tbl);
}

int bnxt_qplib_init_res(struct bnxt_qplib_res *res)
{
	bnxt_qplib_init_sgid_tbl(&res->sgid_tbl, res->netdev);
	bnxt_qplib_init_pkey_tbl(res, &res->pkey_tbl);

	return 0;
}

void bnxt_qplib_free_res(struct bnxt_qplib_res *res)
{
	bnxt_qplib_free_pkey_tbl(res, &res->pkey_tbl);
	bnxt_qplib_free_sgid_tbl(res, &res->sgid_tbl);
	bnxt_qplib_free_pd_tbl(&res->pd_tbl);
	bnxt_qplib_free_dpi_tbl(res, &res->dpi_tbl);
<<<<<<< HEAD

	res->netdev = NULL;
	res->pdev = NULL;
=======
>>>>>>> upstream/android-13
}

int bnxt_qplib_alloc_res(struct bnxt_qplib_res *res, struct pci_dev *pdev,
			 struct net_device *netdev,
			 struct bnxt_qplib_dev_attr *dev_attr)
{
	int rc = 0;

	res->pdev = pdev;
	res->netdev = netdev;

	rc = bnxt_qplib_alloc_sgid_tbl(res, &res->sgid_tbl, dev_attr->max_sgid);
	if (rc)
		goto fail;

	rc = bnxt_qplib_alloc_pkey_tbl(res, &res->pkey_tbl, dev_attr->max_pkey);
	if (rc)
		goto fail;

	rc = bnxt_qplib_alloc_pd_tbl(res, &res->pd_tbl, dev_attr->max_pd);
	if (rc)
		goto fail;

	rc = bnxt_qplib_alloc_dpi_tbl(res, &res->dpi_tbl, dev_attr->l2_db_size);
	if (rc)
		goto fail;

	return 0;
fail:
	bnxt_qplib_free_res(res);
	return rc;
}
<<<<<<< HEAD
=======

int bnxt_qplib_determine_atomics(struct pci_dev *dev)
{
	int comp;
	u16 ctl2;

	comp = pci_enable_atomic_ops_to_root(dev,
					     PCI_EXP_DEVCAP2_ATOMIC_COMP32);
	if (comp)
		return -EOPNOTSUPP;
	comp = pci_enable_atomic_ops_to_root(dev,
					     PCI_EXP_DEVCAP2_ATOMIC_COMP64);
	if (comp)
		return -EOPNOTSUPP;
	pcie_capability_read_word(dev, PCI_EXP_DEVCTL2, &ctl2);
	return !(ctl2 & PCI_EXP_DEVCTL2_ATOMIC_REQ);
}
>>>>>>> upstream/android-13
