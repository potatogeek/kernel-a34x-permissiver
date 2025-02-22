// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/* Copyright (C) 2012-2018 ARM Limited or its affiliates. */
=======
/* Copyright (C) 2012-2019 ARM Limited (or its affiliates). */
>>>>>>> upstream/android-13

#include "cc_driver.h"
#include "cc_sram_mgr.h"

/**
<<<<<<< HEAD
 * struct cc_sram_ctx -Internal RAM context manager
 * @sram_free_offset:   the offset to the non-allocated area
 */
struct cc_sram_ctx {
	cc_sram_addr_t sram_free_offset;
};

/**
 * cc_sram_mgr_fini() - Cleanup SRAM pool.
 *
 * @drvdata: Associated device driver context
 */
void cc_sram_mgr_fini(struct cc_drvdata *drvdata)
{
	/* Free "this" context */
	kfree(drvdata->sram_mgr_handle);
}

/**
=======
>>>>>>> upstream/android-13
 * cc_sram_mgr_init() - Initializes SRAM pool.
 *      The pool starts right at the beginning of SRAM.
 *      Returns zero for success, negative value otherwise.
 *
 * @drvdata: Associated device driver context
<<<<<<< HEAD
 */
int cc_sram_mgr_init(struct cc_drvdata *drvdata)
{
	struct cc_sram_ctx *ctx;
	dma_addr_t start = 0;
=======
 *
 * Return:
 * 0 for success, negative error code for failure.
 */
int cc_sram_mgr_init(struct cc_drvdata *drvdata)
{
	u32 start = 0;
>>>>>>> upstream/android-13
	struct device *dev = drvdata_to_dev(drvdata);

	if (drvdata->hw_rev < CC_HW_REV_712) {
		/* Pool starts after ROM bytes */
<<<<<<< HEAD
		start = (dma_addr_t)cc_ioread(drvdata,
					      CC_REG(HOST_SEP_SRAM_THRESHOLD));

		if ((start & 0x3) != 0) {
			dev_err(dev, "Invalid SRAM offset %pad\n", &start);
=======
		start = cc_ioread(drvdata, CC_REG(HOST_SEP_SRAM_THRESHOLD));
		if ((start & 0x3) != 0) {
			dev_err(dev, "Invalid SRAM offset 0x%x\n", start);
>>>>>>> upstream/android-13
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	/* Allocate "this" context */
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);

	if (!ctx)
		return -ENOMEM;

	ctx->sram_free_offset = start;
	drvdata->sram_mgr_handle = ctx;

	return 0;
}

/*!
 * Allocated buffer from SRAM pool.
 * Note: Caller is responsible to free the LAST allocated buffer.
 * This function does not taking care of any fragmentation may occur
 * by the order of calls to alloc/free.
 *
 * \param drvdata
 * \param size The requested bytes to allocate
 */
cc_sram_addr_t cc_sram_alloc(struct cc_drvdata *drvdata, u32 size)
{
	struct cc_sram_ctx *smgr_ctx = drvdata->sram_mgr_handle;
	struct device *dev = drvdata_to_dev(drvdata);
	cc_sram_addr_t p;
=======
	drvdata->sram_free_offset = start;
	return 0;
}

/**
 * cc_sram_alloc() - Allocate buffer from SRAM pool.
 *
 * @drvdata: Associated device driver context
 * @size: The requested numer of bytes to allocate
 *
 * Return:
 * Address offset in SRAM or NULL_SRAM_ADDR for failure.
 */
u32 cc_sram_alloc(struct cc_drvdata *drvdata, u32 size)
{
	struct device *dev = drvdata_to_dev(drvdata);
	u32 p;
>>>>>>> upstream/android-13

	if ((size & 0x3)) {
		dev_err(dev, "Requested buffer size (%u) is not multiple of 4",
			size);
		return NULL_SRAM_ADDR;
	}
<<<<<<< HEAD
	if (size > (CC_CC_SRAM_SIZE - smgr_ctx->sram_free_offset)) {
		dev_err(dev, "Not enough space to allocate %u B (at offset %llu)\n",
			size, smgr_ctx->sram_free_offset);
		return NULL_SRAM_ADDR;
	}

	p = smgr_ctx->sram_free_offset;
	smgr_ctx->sram_free_offset += size;
	dev_dbg(dev, "Allocated %u B @ %u\n", size, (unsigned int)p);
=======
	if (size > (CC_CC_SRAM_SIZE - drvdata->sram_free_offset)) {
		dev_err(dev, "Not enough space to allocate %u B (at offset %u)\n",
			size, drvdata->sram_free_offset);
		return NULL_SRAM_ADDR;
	}

	p = drvdata->sram_free_offset;
	drvdata->sram_free_offset += size;
	dev_dbg(dev, "Allocated %u B @ %u\n", size, p);
>>>>>>> upstream/android-13
	return p;
}

/**
 * cc_set_sram_desc() - Create const descriptors sequence to
 *	set values in given array into SRAM.
 * Note: each const value can't exceed word size.
 *
 * @src:	  A pointer to array of words to set as consts.
 * @dst:	  The target SRAM buffer to set into
<<<<<<< HEAD
 * @nelements:	  The number of words in "src" array
 * @seq:	  A pointer to the given IN/OUT descriptor sequence
 * @seq_len:	  A pointer to the given IN/OUT sequence length
 */
void cc_set_sram_desc(const u32 *src, cc_sram_addr_t dst,
		      unsigned int nelement, struct cc_hw_desc *seq,
		      unsigned int *seq_len)
=======
 * @nelement:	  The number of words in "src" array
 * @seq:	  A pointer to the given IN/OUT descriptor sequence
 * @seq_len:	  A pointer to the given IN/OUT sequence length
 */
void cc_set_sram_desc(const u32 *src, u32 dst, unsigned int nelement,
		      struct cc_hw_desc *seq, unsigned int *seq_len)
>>>>>>> upstream/android-13
{
	u32 i;
	unsigned int idx = *seq_len;

	for (i = 0; i < nelement; i++, idx++) {
		hw_desc_init(&seq[idx]);
		set_din_const(&seq[idx], src[i], sizeof(u32));
		set_dout_sram(&seq[idx], dst + (i * sizeof(u32)), sizeof(u32));
		set_flow_mode(&seq[idx], BYPASS);
	}

	*seq_len = idx;
}
