/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _SPRD_DMA_H_
#define _SPRD_DMA_H_

#define SPRD_DMA_REQ_SHIFT 16
#define SPRD_DMA_FLAGS(req_mode, int_type) \
	((req_mode) << SPRD_DMA_REQ_SHIFT | (int_type))


enum sprd_dma_req_mode {
	SPRD_DMA_FRAG_REQ,
	SPRD_DMA_BLK_REQ,
	SPRD_DMA_TRANS_REQ,
	SPRD_DMA_LIST_REQ,
};


enum sprd_dma_int_type {
	SPRD_DMA_NO_INT,
	SPRD_DMA_FRAG_INT,
	SPRD_DMA_BLK_INT,
	SPRD_DMA_BLK_FRAG_INT,
	SPRD_DMA_TRANS_INT,
	SPRD_DMA_TRANS_FRAG_INT,
	SPRD_DMA_TRANS_BLK_INT,
	SPRD_DMA_LIST_INT,
	SPRD_DMA_CFGERR_INT,
};

#endif
