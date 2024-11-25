// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2017 Marvell
 *
 * Antoine Tenart <antoine.tenart@free-electrons.com>
 */

#include <linux/dma-mapping.h>
#include <linux/spinlock.h>

#include "safexcel.h"

int safexcel_init_ring_descriptors(struct safexcel_crypto_priv *priv,
				   struct safexcel_desc_ring *cdr,
				   struct safexcel_desc_ring *rdr)
{
<<<<<<< HEAD
	cdr->offset = sizeof(u32) * priv->config.cd_offset;
=======
	int i;
	struct safexcel_command_desc *cdesc;
	dma_addr_t atok;

	/* Actual command descriptor ring */
	cdr->offset = priv->config.cd_offset;
>>>>>>> upstream/android-13
	cdr->base = dmam_alloc_coherent(priv->dev,
					cdr->offset * EIP197_DEFAULT_RING_SIZE,
					&cdr->base_dma, GFP_KERNEL);
	if (!cdr->base)
		return -ENOMEM;
	cdr->write = cdr->base;
	cdr->base_end = cdr->base + cdr->offset * (EIP197_DEFAULT_RING_SIZE - 1);
	cdr->read = cdr->base;

<<<<<<< HEAD
	rdr->offset = sizeof(u32) * priv->config.rd_offset;
=======
	/* Command descriptor shadow ring for storing additional token data */
	cdr->shoffset = priv->config.cdsh_offset;
	cdr->shbase = dmam_alloc_coherent(priv->dev,
					  cdr->shoffset *
					  EIP197_DEFAULT_RING_SIZE,
					  &cdr->shbase_dma, GFP_KERNEL);
	if (!cdr->shbase)
		return -ENOMEM;
	cdr->shwrite = cdr->shbase;
	cdr->shbase_end = cdr->shbase + cdr->shoffset *
					(EIP197_DEFAULT_RING_SIZE - 1);

	/*
	 * Populate command descriptors with physical pointers to shadow descs.
	 * Note that we only need to do this once if we don't overwrite them.
	 */
	cdesc = cdr->base;
	atok = cdr->shbase_dma;
	for (i = 0; i < EIP197_DEFAULT_RING_SIZE; i++) {
		cdesc->atok_lo = lower_32_bits(atok);
		cdesc->atok_hi = upper_32_bits(atok);
		cdesc = (void *)cdesc + cdr->offset;
		atok += cdr->shoffset;
	}

	rdr->offset = priv->config.rd_offset;
	/* Use shoffset for result token offset here */
	rdr->shoffset = priv->config.res_offset;
>>>>>>> upstream/android-13
	rdr->base = dmam_alloc_coherent(priv->dev,
					rdr->offset * EIP197_DEFAULT_RING_SIZE,
					&rdr->base_dma, GFP_KERNEL);
	if (!rdr->base)
		return -ENOMEM;
	rdr->write = rdr->base;
	rdr->base_end = rdr->base + rdr->offset  * (EIP197_DEFAULT_RING_SIZE - 1);
	rdr->read = rdr->base;

	return 0;
}

inline int safexcel_select_ring(struct safexcel_crypto_priv *priv)
{
	return (atomic_inc_return(&priv->ring_used) % priv->config.rings);
}

<<<<<<< HEAD
static void *safexcel_ring_next_wptr(struct safexcel_crypto_priv *priv,
				     struct safexcel_desc_ring *ring)
{
	void *ptr = ring->write;

=======
static void *safexcel_ring_next_cwptr(struct safexcel_crypto_priv *priv,
				     struct safexcel_desc_ring *ring,
				     bool first,
				     struct safexcel_token **atoken)
{
	void *ptr = ring->write;

	if (first)
		*atoken = ring->shwrite;

	if ((ring->write == ring->read - ring->offset) ||
	    (ring->read == ring->base && ring->write == ring->base_end))
		return ERR_PTR(-ENOMEM);

	if (ring->write == ring->base_end) {
		ring->write = ring->base;
		ring->shwrite = ring->shbase;
	} else {
		ring->write += ring->offset;
		ring->shwrite += ring->shoffset;
	}

	return ptr;
}

static void *safexcel_ring_next_rwptr(struct safexcel_crypto_priv *priv,
				     struct safexcel_desc_ring *ring,
				     struct result_data_desc **rtoken)
{
	void *ptr = ring->write;

	/* Result token at relative offset shoffset */
	*rtoken = ring->write + ring->shoffset;

>>>>>>> upstream/android-13
	if ((ring->write == ring->read - ring->offset) ||
	    (ring->read == ring->base && ring->write == ring->base_end))
		return ERR_PTR(-ENOMEM);

	if (ring->write == ring->base_end)
		ring->write = ring->base;
	else
		ring->write += ring->offset;

	return ptr;
}

void *safexcel_ring_next_rptr(struct safexcel_crypto_priv *priv,
			      struct safexcel_desc_ring *ring)
{
	void *ptr = ring->read;

	if (ring->write == ring->read)
		return ERR_PTR(-ENOENT);

	if (ring->read == ring->base_end)
		ring->read = ring->base;
	else
		ring->read += ring->offset;

	return ptr;
}

inline void *safexcel_ring_curr_rptr(struct safexcel_crypto_priv *priv,
				     int ring)
{
	struct safexcel_desc_ring *rdr = &priv->ring[ring].rdr;

	return rdr->read;
}

inline int safexcel_ring_first_rdr_index(struct safexcel_crypto_priv *priv,
					 int ring)
{
	struct safexcel_desc_ring *rdr = &priv->ring[ring].rdr;

	return (rdr->read - rdr->base) / rdr->offset;
}

inline int safexcel_ring_rdr_rdesc_index(struct safexcel_crypto_priv *priv,
					 int ring,
					 struct safexcel_result_desc *rdesc)
{
	struct safexcel_desc_ring *rdr = &priv->ring[ring].rdr;

	return ((void *)rdesc - rdr->base) / rdr->offset;
}

void safexcel_ring_rollback_wptr(struct safexcel_crypto_priv *priv,
				 struct safexcel_desc_ring *ring)
{
	if (ring->write == ring->read)
		return;

<<<<<<< HEAD
	if (ring->write == ring->base)
		ring->write = ring->base_end;
	else
		ring->write -= ring->offset;
=======
	if (ring->write == ring->base) {
		ring->write = ring->base_end;
		ring->shwrite = ring->shbase_end;
	} else {
		ring->write -= ring->offset;
		ring->shwrite -= ring->shoffset;
	}
>>>>>>> upstream/android-13
}

struct safexcel_command_desc *safexcel_add_cdesc(struct safexcel_crypto_priv *priv,
						 int ring_id,
						 bool first, bool last,
						 dma_addr_t data, u32 data_len,
						 u32 full_data_len,
<<<<<<< HEAD
						 dma_addr_t context) {
	struct safexcel_command_desc *cdesc;
	int i;

	cdesc = safexcel_ring_next_wptr(priv, &priv->ring[ring_id].cdr);
	if (IS_ERR(cdesc))
		return cdesc;

	memset(cdesc, 0, sizeof(struct safexcel_command_desc));

	cdesc->first_seg = first;
	cdesc->last_seg = last;
	cdesc->particle_size = data_len;
	cdesc->data_lo = lower_32_bits(data);
	cdesc->data_hi = upper_32_bits(data);

	if (first && context) {
		struct safexcel_token *token =
			(struct safexcel_token *)cdesc->control_data.token;

		cdesc->control_data.packet_length = full_data_len;
		cdesc->control_data.options = EIP197_OPTION_MAGIC_VALUE |
					      EIP197_OPTION_64BIT_CTX |
					      EIP197_OPTION_CTX_CTRL_IN_CMD;
		cdesc->control_data.context_lo =
			(lower_32_bits(context) & GENMASK(31, 2)) >> 2;
		cdesc->control_data.context_hi = upper_32_bits(context);

		/* TODO: large xform HMAC with SHA-384/512 uses refresh = 3 */
		cdesc->control_data.refresh = 2;

		for (i = 0; i < EIP197_MAX_TOKENS; i++)
			eip197_noop_token(&token[i]);
=======
						 dma_addr_t context,
						 struct safexcel_token **atoken)
{
	struct safexcel_command_desc *cdesc;

	cdesc = safexcel_ring_next_cwptr(priv, &priv->ring[ring_id].cdr,
					 first, atoken);
	if (IS_ERR(cdesc))
		return cdesc;

	cdesc->particle_size = data_len;
	cdesc->rsvd0 = 0;
	cdesc->last_seg = last;
	cdesc->first_seg = first;
	cdesc->additional_cdata_size = 0;
	cdesc->rsvd1 = 0;
	cdesc->data_lo = lower_32_bits(data);
	cdesc->data_hi = upper_32_bits(data);

	if (first) {
		/*
		 * Note that the length here MUST be >0 or else the EIP(1)97
		 * may hang. Newer EIP197 firmware actually incorporates this
		 * fix already, but that doesn't help the EIP97 and we may
		 * also be running older firmware.
		 */
		cdesc->control_data.packet_length = full_data_len ?: 1;
		cdesc->control_data.options = EIP197_OPTION_MAGIC_VALUE |
					      EIP197_OPTION_64BIT_CTX |
					      EIP197_OPTION_CTX_CTRL_IN_CMD |
					      EIP197_OPTION_RC_AUTO;
		cdesc->control_data.type = EIP197_TYPE_BCLA;
		cdesc->control_data.context_lo = lower_32_bits(context) |
						 EIP197_CONTEXT_SMALL;
		cdesc->control_data.context_hi = upper_32_bits(context);
>>>>>>> upstream/android-13
	}

	return cdesc;
}

struct safexcel_result_desc *safexcel_add_rdesc(struct safexcel_crypto_priv *priv,
						int ring_id,
						bool first, bool last,
						dma_addr_t data, u32 len)
{
	struct safexcel_result_desc *rdesc;
<<<<<<< HEAD

	rdesc = safexcel_ring_next_wptr(priv, &priv->ring[ring_id].rdr);
	if (IS_ERR(rdesc))
		return rdesc;

	memset(rdesc, 0, sizeof(struct safexcel_result_desc));

	rdesc->first_seg = first;
	rdesc->last_seg = last;
	rdesc->particle_size = len;
	rdesc->data_lo = lower_32_bits(data);
	rdesc->data_hi = upper_32_bits(data);

=======
	struct result_data_desc *rtoken;

	rdesc = safexcel_ring_next_rwptr(priv, &priv->ring[ring_id].rdr,
					 &rtoken);
	if (IS_ERR(rdesc))
		return rdesc;

	rdesc->particle_size = len;
	rdesc->rsvd0 = 0;
	rdesc->descriptor_overflow = 1; /* assume error */
	rdesc->buffer_overflow = 1;     /* assume error */
	rdesc->last_seg = last;
	rdesc->first_seg = first;
	rdesc->result_size = EIP197_RD64_RESULT_SIZE;
	rdesc->rsvd1 = 0;
	rdesc->data_lo = lower_32_bits(data);
	rdesc->data_hi = upper_32_bits(data);

	/* Clear length in result token */
	rtoken->packet_length = 0;
	/* Assume errors - HW will clear if not the case */
	rtoken->error_code = 0x7fff;

>>>>>>> upstream/android-13
	return rdesc;
}
