<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/****************************************************************************
 * Driver for Solarflare network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2005-2013 Solarflare Communications Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
=======
>>>>>>> upstream/android-13
 */

#include <linux/socket.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/prefetch.h>
#include <linux/moduleparam.h>
#include <linux/iommu.h>
#include <net/ip.h>
#include <net/checksum.h>
<<<<<<< HEAD
#include "net_driver.h"
#include "efx.h"
=======
#include <net/xdp.h>
#include <linux/bpf_trace.h>
#include "net_driver.h"
#include "efx.h"
#include "rx_common.h"
>>>>>>> upstream/android-13
#include "filter.h"
#include "nic.h"
#include "selftest.h"
#include "workarounds.h"

/* Preferred number of descriptors to fill at once */
#define EFX_RX_PREFERRED_BATCH 8U

<<<<<<< HEAD
/* Number of RX buffers to recycle pages for.  When creating the RX page recycle
 * ring, this number is divided by the number of buffers per page to calculate
 * the number of pages to store in the RX page recycle ring.
 */
#define EFX_RECYCLE_RING_SIZE_IOMMU 4096
#define EFX_RECYCLE_RING_SIZE_NOIOMMU (2 * EFX_RX_PREFERRED_BATCH)
=======
/* Maximum rx prefix used by any architecture. */
#define EFX_MAX_RX_PREFIX_SIZE 16
>>>>>>> upstream/android-13

/* Size of buffer allocated for skb header area. */
#define EFX_SKB_HEADERS  128u

<<<<<<< HEAD
/* This is the percentage fill level below which new RX descriptors
 * will be added to the RX descriptor ring.
 */
static unsigned int rx_refill_threshold;

=======
>>>>>>> upstream/android-13
/* Each packet can consume up to ceil(max_frame_len / buffer_size) buffers */
#define EFX_RX_MAX_FRAGS DIV_ROUND_UP(EFX_MAX_FRAME_LEN(EFX_MAX_MTU), \
				      EFX_RX_USR_BUF_SIZE)

<<<<<<< HEAD
/*
 * RX maximum head room required.
 *
 * This must be at least 1 to prevent overflow, plus one packet-worth
 * to allow pipelined receives.
 */
#define EFX_RXD_HEAD_ROOM (1 + EFX_RX_MAX_FRAGS)

static inline u8 *efx_rx_buf_va(struct efx_rx_buffer *buf)
{
	return page_address(buf->page) + buf->page_offset;
}

static inline u32 efx_rx_buf_hash(struct efx_nic *efx, const u8 *eh)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	return __le32_to_cpup((const __le32 *)(eh + efx->rx_packet_hash_offset));
#else
	const u8 *data = eh + efx->rx_packet_hash_offset;
	return (u32)data[0]	  |
	       (u32)data[1] << 8  |
	       (u32)data[2] << 16 |
	       (u32)data[3] << 24;
#endif
}

static inline struct efx_rx_buffer *
efx_rx_buf_next(struct efx_rx_queue *rx_queue, struct efx_rx_buffer *rx_buf)
{
	if (unlikely(rx_buf == efx_rx_buffer(rx_queue, rx_queue->ptr_mask)))
		return efx_rx_buffer(rx_queue, 0);
	else
		return rx_buf + 1;
}

static inline void efx_sync_rx_buffer(struct efx_nic *efx,
				      struct efx_rx_buffer *rx_buf,
				      unsigned int len)
{
	dma_sync_single_for_cpu(&efx->pci_dev->dev, rx_buf->dma_addr, len,
				DMA_FROM_DEVICE);
}

void efx_rx_config_page_split(struct efx_nic *efx)
{
	efx->rx_page_buf_step = ALIGN(efx->rx_dma_len + efx->rx_ip_align,
				      EFX_RX_BUF_ALIGNMENT);
	efx->rx_bufs_per_page = efx->rx_buffer_order ? 1 :
		((PAGE_SIZE - sizeof(struct efx_rx_page_state)) /
		 efx->rx_page_buf_step);
	efx->rx_buffer_truesize = (PAGE_SIZE << efx->rx_buffer_order) /
		efx->rx_bufs_per_page;
	efx->rx_pages_per_batch = DIV_ROUND_UP(EFX_RX_PREFERRED_BATCH,
					       efx->rx_bufs_per_page);
}

/* Check the RX page recycle ring for a page that can be reused. */
static struct page *efx_reuse_page(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	struct page *page;
	struct efx_rx_page_state *state;
	unsigned index;

	index = rx_queue->page_remove & rx_queue->page_ptr_mask;
	page = rx_queue->page_ring[index];
	if (page == NULL)
		return NULL;

	rx_queue->page_ring[index] = NULL;
	/* page_remove cannot exceed page_add. */
	if (rx_queue->page_remove != rx_queue->page_add)
		++rx_queue->page_remove;

	/* If page_count is 1 then we hold the only reference to this page. */
	if (page_count(page) == 1) {
		++rx_queue->page_recycle_count;
		return page;
	} else {
		state = page_address(page);
		dma_unmap_page(&efx->pci_dev->dev, state->dma_addr,
			       PAGE_SIZE << efx->rx_buffer_order,
			       DMA_FROM_DEVICE);
		put_page(page);
		++rx_queue->page_recycle_failed;
	}

	return NULL;
}

/**
 * efx_init_rx_buffers - create EFX_RX_BATCH page-based RX buffers
 *
 * @rx_queue:		Efx RX queue
 *
 * This allocates a batch of pages, maps them for DMA, and populates
 * struct efx_rx_buffers for each one. Return a negative error code or
 * 0 on success. If a single page can be used for multiple buffers,
 * then the page will either be inserted fully, or not at all.
 */
static int efx_init_rx_buffers(struct efx_rx_queue *rx_queue, bool atomic)
{
	struct efx_nic *efx = rx_queue->efx;
	struct efx_rx_buffer *rx_buf;
	struct page *page;
	unsigned int page_offset;
	struct efx_rx_page_state *state;
	dma_addr_t dma_addr;
	unsigned index, count;

	count = 0;
	do {
		page = efx_reuse_page(rx_queue);
		if (page == NULL) {
			page = alloc_pages(__GFP_COMP |
					   (atomic ? GFP_ATOMIC : GFP_KERNEL),
					   efx->rx_buffer_order);
			if (unlikely(page == NULL))
				return -ENOMEM;
			dma_addr =
				dma_map_page(&efx->pci_dev->dev, page, 0,
					     PAGE_SIZE << efx->rx_buffer_order,
					     DMA_FROM_DEVICE);
			if (unlikely(dma_mapping_error(&efx->pci_dev->dev,
						       dma_addr))) {
				__free_pages(page, efx->rx_buffer_order);
				return -EIO;
			}
			state = page_address(page);
			state->dma_addr = dma_addr;
		} else {
			state = page_address(page);
			dma_addr = state->dma_addr;
		}

		dma_addr += sizeof(struct efx_rx_page_state);
		page_offset = sizeof(struct efx_rx_page_state);

		do {
			index = rx_queue->added_count & rx_queue->ptr_mask;
			rx_buf = efx_rx_buffer(rx_queue, index);
			rx_buf->dma_addr = dma_addr + efx->rx_ip_align;
			rx_buf->page = page;
			rx_buf->page_offset = page_offset + efx->rx_ip_align;
			rx_buf->len = efx->rx_dma_len;
			rx_buf->flags = 0;
			++rx_queue->added_count;
			get_page(page);
			dma_addr += efx->rx_page_buf_step;
			page_offset += efx->rx_page_buf_step;
		} while (page_offset + efx->rx_page_buf_step <= PAGE_SIZE);

		rx_buf->flags = EFX_RX_BUF_LAST_IN_PAGE;
	} while (++count < efx->rx_pages_per_batch);

	return 0;
}

/* Unmap a DMA-mapped page.  This function is only called for the final RX
 * buffer in a page.
 */
static void efx_unmap_rx_buffer(struct efx_nic *efx,
				struct efx_rx_buffer *rx_buf)
{
	struct page *page = rx_buf->page;

	if (page) {
		struct efx_rx_page_state *state = page_address(page);
		dma_unmap_page(&efx->pci_dev->dev,
			       state->dma_addr,
			       PAGE_SIZE << efx->rx_buffer_order,
			       DMA_FROM_DEVICE);
	}
}

static void efx_free_rx_buffers(struct efx_rx_queue *rx_queue,
				struct efx_rx_buffer *rx_buf,
				unsigned int num_bufs)
{
	do {
		if (rx_buf->page) {
			put_page(rx_buf->page);
			rx_buf->page = NULL;
		}
		rx_buf = efx_rx_buf_next(rx_queue, rx_buf);
	} while (--num_bufs);
}

/* Attempt to recycle the page if there is an RX recycle ring; the page can
 * only be added if this is the final RX buffer, to prevent pages being used in
 * the descriptor ring and appearing in the recycle ring simultaneously.
 */
static void efx_recycle_rx_page(struct efx_channel *channel,
				struct efx_rx_buffer *rx_buf)
{
	struct page *page = rx_buf->page;
	struct efx_rx_queue *rx_queue = efx_channel_get_rx_queue(channel);
	struct efx_nic *efx = rx_queue->efx;
	unsigned index;

	/* Only recycle the page after processing the final buffer. */
	if (!(rx_buf->flags & EFX_RX_BUF_LAST_IN_PAGE))
		return;

	index = rx_queue->page_add & rx_queue->page_ptr_mask;
	if (rx_queue->page_ring[index] == NULL) {
		unsigned read_index = rx_queue->page_remove &
			rx_queue->page_ptr_mask;

		/* The next slot in the recycle ring is available, but
		 * increment page_remove if the read pointer currently
		 * points here.
		 */
		if (read_index == index)
			++rx_queue->page_remove;
		rx_queue->page_ring[index] = page;
		++rx_queue->page_add;
		return;
	}
	++rx_queue->page_recycle_full;
	efx_unmap_rx_buffer(efx, rx_buf);
	put_page(rx_buf->page);
}

static void efx_fini_rx_buffer(struct efx_rx_queue *rx_queue,
			       struct efx_rx_buffer *rx_buf)
{
	/* Release the page reference we hold for the buffer. */
	if (rx_buf->page)
		put_page(rx_buf->page);

	/* If this is the last buffer in a page, unmap and free it. */
	if (rx_buf->flags & EFX_RX_BUF_LAST_IN_PAGE) {
		efx_unmap_rx_buffer(rx_queue->efx, rx_buf);
		efx_free_rx_buffers(rx_queue, rx_buf, 1);
	}
	rx_buf->page = NULL;
}

/* Recycle the pages that are used by buffers that have just been received. */
static void efx_recycle_rx_pages(struct efx_channel *channel,
				 struct efx_rx_buffer *rx_buf,
				 unsigned int n_frags)
{
	struct efx_rx_queue *rx_queue = efx_channel_get_rx_queue(channel);

	do {
		efx_recycle_rx_page(channel, rx_buf);
		rx_buf = efx_rx_buf_next(rx_queue, rx_buf);
	} while (--n_frags);
}

static void efx_discard_rx_packet(struct efx_channel *channel,
				  struct efx_rx_buffer *rx_buf,
				  unsigned int n_frags)
{
	struct efx_rx_queue *rx_queue = efx_channel_get_rx_queue(channel);

	efx_recycle_rx_pages(channel, rx_buf, n_frags);

	efx_free_rx_buffers(rx_queue, rx_buf, n_frags);
}

/**
 * efx_fast_push_rx_descriptors - push new RX descriptors quickly
 * @rx_queue:		RX descriptor queue
 *
 * This will aim to fill the RX descriptor queue up to
 * @rx_queue->@max_fill. If there is insufficient atomic
 * memory to do so, a slow fill will be scheduled.
 *
 * The caller must provide serialisation (none is used here). In practise,
 * this means this function must run from the NAPI handler, or be called
 * when NAPI is disabled.
 */
void efx_fast_push_rx_descriptors(struct efx_rx_queue *rx_queue, bool atomic)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned int fill_level, batch_size;
	int space, rc = 0;

	if (!rx_queue->refill_enabled)
		return;

	/* Calculate current fill level, and exit if we don't need to fill */
	fill_level = (rx_queue->added_count - rx_queue->removed_count);
	EFX_WARN_ON_ONCE_PARANOID(fill_level > rx_queue->efx->rxq_entries);
	if (fill_level >= rx_queue->fast_fill_trigger)
		goto out;

	/* Record minimum fill level */
	if (unlikely(fill_level < rx_queue->min_fill)) {
		if (fill_level)
			rx_queue->min_fill = fill_level;
	}

	batch_size = efx->rx_pages_per_batch * efx->rx_bufs_per_page;
	space = rx_queue->max_fill - fill_level;
	EFX_WARN_ON_ONCE_PARANOID(space < batch_size);

	netif_vdbg(rx_queue->efx, rx_status, rx_queue->efx->net_dev,
		   "RX queue %d fast-filling descriptor ring from"
		   " level %d to level %d\n",
		   efx_rx_queue_index(rx_queue), fill_level,
		   rx_queue->max_fill);


	do {
		rc = efx_init_rx_buffers(rx_queue, atomic);
		if (unlikely(rc)) {
			/* Ensure that we don't leave the rx queue empty */
			if (rx_queue->added_count == rx_queue->removed_count)
				efx_schedule_slow_fill(rx_queue);
			goto out;
		}
	} while ((space -= batch_size) >= batch_size);

	netif_vdbg(rx_queue->efx, rx_status, rx_queue->efx->net_dev,
		   "RX queue %d fast-filled descriptor ring "
		   "to level %d\n", efx_rx_queue_index(rx_queue),
		   rx_queue->added_count - rx_queue->removed_count);

 out:
	if (rx_queue->notified_count != rx_queue->added_count)
		efx_nic_notify_rx_desc(rx_queue);
}

void efx_rx_slow_fill(struct timer_list *t)
{
	struct efx_rx_queue *rx_queue = from_timer(rx_queue, t, slow_fill);

	/* Post an event to cause NAPI to run and refill the queue */
	efx_nic_generate_fill_event(rx_queue);
	++rx_queue->slow_fill_count;
}

=======
>>>>>>> upstream/android-13
static void efx_rx_packet__check_len(struct efx_rx_queue *rx_queue,
				     struct efx_rx_buffer *rx_buf,
				     int len)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned max_len = rx_buf->len - efx->type->rx_buffer_padding;

	if (likely(len <= max_len))
		return;

	/* The packet must be discarded, but this is only a fatal error
	 * if the caller indicated it was
	 */
	rx_buf->flags |= EFX_RX_PKT_DISCARD;

	if (net_ratelimit())
		netif_err(efx, rx_err, efx->net_dev,
			  "RX queue %d overlength RX event (%#x > %#x)\n",
			  efx_rx_queue_index(rx_queue), len, max_len);

	efx_rx_queue_channel(rx_queue)->n_rx_overlength++;
}

<<<<<<< HEAD
/* Pass a received packet up through GRO.  GRO can handle pages
 * regardless of checksum state and skbs with a good checksum.
 */
static void
efx_rx_packet_gro(struct efx_channel *channel, struct efx_rx_buffer *rx_buf,
		  unsigned int n_frags, u8 *eh)
{
	struct napi_struct *napi = &channel->napi_str;
	gro_result_t gro_result;
	struct efx_nic *efx = channel->efx;
	struct sk_buff *skb;

	skb = napi_get_frags(napi);
	if (unlikely(!skb)) {
		struct efx_rx_queue *rx_queue;

		rx_queue = efx_channel_get_rx_queue(channel);
		efx_free_rx_buffers(rx_queue, rx_buf, n_frags);
		return;
	}

	if (efx->net_dev->features & NETIF_F_RXHASH)
		skb_set_hash(skb, efx_rx_buf_hash(efx, eh),
			     PKT_HASH_TYPE_L3);
	skb->ip_summed = ((rx_buf->flags & EFX_RX_PKT_CSUMMED) ?
			  CHECKSUM_UNNECESSARY : CHECKSUM_NONE);
	skb->csum_level = !!(rx_buf->flags & EFX_RX_PKT_CSUM_LEVEL);

	for (;;) {
		skb_fill_page_desc(skb, skb_shinfo(skb)->nr_frags,
				   rx_buf->page, rx_buf->page_offset,
				   rx_buf->len);
		rx_buf->page = NULL;
		skb->len += rx_buf->len;
		if (skb_shinfo(skb)->nr_frags == n_frags)
			break;

		rx_buf = efx_rx_buf_next(&channel->rx_queue, rx_buf);
	}

	skb->data_len = skb->len;
	skb->truesize += n_frags * efx->rx_buffer_truesize;

	skb_record_rx_queue(skb, channel->rx_queue.core_index);

	gro_result = napi_gro_frags(napi);
	if (gro_result != GRO_DROP)
		channel->irq_mod_score += 2;
}

=======
>>>>>>> upstream/android-13
/* Allocate and construct an SKB around page fragments */
static struct sk_buff *efx_rx_mk_skb(struct efx_channel *channel,
				     struct efx_rx_buffer *rx_buf,
				     unsigned int n_frags,
				     u8 *eh, int hdr_len)
{
	struct efx_nic *efx = channel->efx;
	struct sk_buff *skb;

	/* Allocate an SKB to store the headers */
	skb = netdev_alloc_skb(efx->net_dev,
			       efx->rx_ip_align + efx->rx_prefix_size +
			       hdr_len);
	if (unlikely(skb == NULL)) {
		atomic_inc(&efx->n_rx_noskb_drops);
		return NULL;
	}

	EFX_WARN_ON_ONCE_PARANOID(rx_buf->len < hdr_len);

	memcpy(skb->data + efx->rx_ip_align, eh - efx->rx_prefix_size,
	       efx->rx_prefix_size + hdr_len);
	skb_reserve(skb, efx->rx_ip_align + efx->rx_prefix_size);
	__skb_put(skb, hdr_len);

	/* Append the remaining page(s) onto the frag list */
	if (rx_buf->len > hdr_len) {
		rx_buf->page_offset += hdr_len;
		rx_buf->len -= hdr_len;

		for (;;) {
<<<<<<< HEAD
			skb_fill_page_desc(skb, skb_shinfo(skb)->nr_frags,
					   rx_buf->page, rx_buf->page_offset,
					   rx_buf->len);
			rx_buf->page = NULL;
			skb->len += rx_buf->len;
			skb->data_len += rx_buf->len;
=======
			skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags,
					rx_buf->page, rx_buf->page_offset,
					rx_buf->len, efx->rx_buffer_truesize);
			rx_buf->page = NULL;

>>>>>>> upstream/android-13
			if (skb_shinfo(skb)->nr_frags == n_frags)
				break;

			rx_buf = efx_rx_buf_next(&channel->rx_queue, rx_buf);
		}
	} else {
		__free_pages(rx_buf->page, efx->rx_buffer_order);
		rx_buf->page = NULL;
		n_frags = 0;
	}

<<<<<<< HEAD
	skb->truesize += n_frags * efx->rx_buffer_truesize;

=======
>>>>>>> upstream/android-13
	/* Move past the ethernet header */
	skb->protocol = eth_type_trans(skb, efx->net_dev);

	skb_mark_napi_id(skb, &channel->napi_str);

	return skb;
}

void efx_rx_packet(struct efx_rx_queue *rx_queue, unsigned int index,
		   unsigned int n_frags, unsigned int len, u16 flags)
{
	struct efx_nic *efx = rx_queue->efx;
	struct efx_channel *channel = efx_rx_queue_channel(rx_queue);
	struct efx_rx_buffer *rx_buf;

	rx_queue->rx_packets++;

	rx_buf = efx_rx_buffer(rx_queue, index);
	rx_buf->flags |= flags;

	/* Validate the number of fragments and completed length */
	if (n_frags == 1) {
		if (!(flags & EFX_RX_PKT_PREFIX_LEN))
			efx_rx_packet__check_len(rx_queue, rx_buf, len);
	} else if (unlikely(n_frags > EFX_RX_MAX_FRAGS) ||
		   unlikely(len <= (n_frags - 1) * efx->rx_dma_len) ||
		   unlikely(len > n_frags * efx->rx_dma_len) ||
		   unlikely(!efx->rx_scatter)) {
		/* If this isn't an explicit discard request, either
		 * the hardware or the driver is broken.
		 */
		WARN_ON(!(len == 0 && rx_buf->flags & EFX_RX_PKT_DISCARD));
		rx_buf->flags |= EFX_RX_PKT_DISCARD;
	}

	netif_vdbg(efx, rx_status, efx->net_dev,
		   "RX queue %d received ids %x-%x len %d %s%s\n",
		   efx_rx_queue_index(rx_queue), index,
		   (index + n_frags - 1) & rx_queue->ptr_mask, len,
		   (rx_buf->flags & EFX_RX_PKT_CSUMMED) ? " [SUMMED]" : "",
		   (rx_buf->flags & EFX_RX_PKT_DISCARD) ? " [DISCARD]" : "");

	/* Discard packet, if instructed to do so.  Process the
	 * previous receive first.
	 */
	if (unlikely(rx_buf->flags & EFX_RX_PKT_DISCARD)) {
		efx_rx_flush_packet(channel);
		efx_discard_rx_packet(channel, rx_buf, n_frags);
		return;
	}

	if (n_frags == 1 && !(flags & EFX_RX_PKT_PREFIX_LEN))
		rx_buf->len = len;

	/* Release and/or sync the DMA mapping - assumes all RX buffers
	 * consumed in-order per RX queue.
	 */
	efx_sync_rx_buffer(efx, rx_buf, rx_buf->len);

	/* Prefetch nice and early so data will (hopefully) be in cache by
	 * the time we look at it.
	 */
	prefetch(efx_rx_buf_va(rx_buf));

	rx_buf->page_offset += efx->rx_prefix_size;
	rx_buf->len -= efx->rx_prefix_size;

	if (n_frags > 1) {
		/* Release/sync DMA mapping for additional fragments.
		 * Fix length for last fragment.
		 */
		unsigned int tail_frags = n_frags - 1;

		for (;;) {
			rx_buf = efx_rx_buf_next(rx_queue, rx_buf);
			if (--tail_frags == 0)
				break;
			efx_sync_rx_buffer(efx, rx_buf, efx->rx_dma_len);
		}
		rx_buf->len = len - (n_frags - 1) * efx->rx_dma_len;
		efx_sync_rx_buffer(efx, rx_buf, rx_buf->len);
	}

	/* All fragments have been DMA-synced, so recycle pages. */
	rx_buf = efx_rx_buffer(rx_queue, index);
	efx_recycle_rx_pages(channel, rx_buf, n_frags);

	/* Pipeline receives so that we give time for packet headers to be
	 * prefetched into cache.
	 */
	efx_rx_flush_packet(channel);
	channel->rx_pkt_n_frags = n_frags;
	channel->rx_pkt_index = index;
}

static void efx_rx_deliver(struct efx_channel *channel, u8 *eh,
			   struct efx_rx_buffer *rx_buf,
			   unsigned int n_frags)
{
	struct sk_buff *skb;
	u16 hdr_len = min_t(u16, rx_buf->len, EFX_SKB_HEADERS);

	skb = efx_rx_mk_skb(channel, rx_buf, n_frags, eh, hdr_len);
	if (unlikely(skb == NULL)) {
		struct efx_rx_queue *rx_queue;

		rx_queue = efx_channel_get_rx_queue(channel);
		efx_free_rx_buffers(rx_queue, rx_buf, n_frags);
		return;
	}
	skb_record_rx_queue(skb, channel->rx_queue.core_index);

	/* Set the SKB flags */
	skb_checksum_none_assert(skb);
	if (likely(rx_buf->flags & EFX_RX_PKT_CSUMMED)) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		skb->csum_level = !!(rx_buf->flags & EFX_RX_PKT_CSUM_LEVEL);
	}

	efx_rx_skb_attach_timestamp(channel, skb);

	if (channel->type->receive_skb)
		if (channel->type->receive_skb(channel, skb))
			return;

	/* Pass the packet up */
	if (channel->rx_list != NULL)
		/* Add to list, will pass up later */
		list_add_tail(&skb->list, channel->rx_list);
	else
		/* No list, so pass it up now */
		netif_receive_skb(skb);
}

<<<<<<< HEAD
=======
/** efx_do_xdp: perform XDP processing on a received packet
 *
 * Returns true if packet should still be delivered.
 */
static bool efx_do_xdp(struct efx_nic *efx, struct efx_channel *channel,
		       struct efx_rx_buffer *rx_buf, u8 **ehp)
{
	u8 rx_prefix[EFX_MAX_RX_PREFIX_SIZE];
	struct efx_rx_queue *rx_queue;
	struct bpf_prog *xdp_prog;
	struct xdp_frame *xdpf;
	struct xdp_buff xdp;
	u32 xdp_act;
	s16 offset;
	int err;

	xdp_prog = rcu_dereference_bh(efx->xdp_prog);
	if (!xdp_prog)
		return true;

	rx_queue = efx_channel_get_rx_queue(channel);

	if (unlikely(channel->rx_pkt_n_frags > 1)) {
		/* We can't do XDP on fragmented packets - drop. */
		efx_free_rx_buffers(rx_queue, rx_buf,
				    channel->rx_pkt_n_frags);
		if (net_ratelimit())
			netif_err(efx, rx_err, efx->net_dev,
				  "XDP is not possible with multiple receive fragments (%d)\n",
				  channel->rx_pkt_n_frags);
		channel->n_rx_xdp_bad_drops++;
		return false;
	}

	dma_sync_single_for_cpu(&efx->pci_dev->dev, rx_buf->dma_addr,
				rx_buf->len, DMA_FROM_DEVICE);

	/* Save the rx prefix. */
	EFX_WARN_ON_PARANOID(efx->rx_prefix_size > EFX_MAX_RX_PREFIX_SIZE);
	memcpy(rx_prefix, *ehp - efx->rx_prefix_size,
	       efx->rx_prefix_size);

	xdp_init_buff(&xdp, efx->rx_page_buf_step, &rx_queue->xdp_rxq_info);
	/* No support yet for XDP metadata */
	xdp_prepare_buff(&xdp, *ehp - EFX_XDP_HEADROOM, EFX_XDP_HEADROOM,
			 rx_buf->len, false);

	xdp_act = bpf_prog_run_xdp(xdp_prog, &xdp);

	offset = (u8 *)xdp.data - *ehp;

	switch (xdp_act) {
	case XDP_PASS:
		/* Fix up rx prefix. */
		if (offset) {
			*ehp += offset;
			rx_buf->page_offset += offset;
			rx_buf->len -= offset;
			memcpy(*ehp - efx->rx_prefix_size, rx_prefix,
			       efx->rx_prefix_size);
		}
		break;

	case XDP_TX:
		/* Buffer ownership passes to tx on success. */
		xdpf = xdp_convert_buff_to_frame(&xdp);
		err = efx_xdp_tx_buffers(efx, 1, &xdpf, true);
		if (unlikely(err != 1)) {
			efx_free_rx_buffers(rx_queue, rx_buf, 1);
			if (net_ratelimit())
				netif_err(efx, rx_err, efx->net_dev,
					  "XDP TX failed (%d)\n", err);
			channel->n_rx_xdp_bad_drops++;
			trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		} else {
			channel->n_rx_xdp_tx++;
		}
		break;

	case XDP_REDIRECT:
		err = xdp_do_redirect(efx->net_dev, &xdp, xdp_prog);
		if (unlikely(err)) {
			efx_free_rx_buffers(rx_queue, rx_buf, 1);
			if (net_ratelimit())
				netif_err(efx, rx_err, efx->net_dev,
					  "XDP redirect failed (%d)\n", err);
			channel->n_rx_xdp_bad_drops++;
			trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		} else {
			channel->n_rx_xdp_redirect++;
		}
		break;

	default:
		bpf_warn_invalid_xdp_action(xdp_act);
		efx_free_rx_buffers(rx_queue, rx_buf, 1);
		channel->n_rx_xdp_bad_drops++;
		trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		break;

	case XDP_ABORTED:
		trace_xdp_exception(efx->net_dev, xdp_prog, xdp_act);
		fallthrough;
	case XDP_DROP:
		efx_free_rx_buffers(rx_queue, rx_buf, 1);
		channel->n_rx_xdp_drops++;
		break;
	}

	return xdp_act == XDP_PASS;
}

>>>>>>> upstream/android-13
/* Handle a received packet.  Second half: Touches packet payload. */
void __efx_rx_packet(struct efx_channel *channel)
{
	struct efx_nic *efx = channel->efx;
	struct efx_rx_buffer *rx_buf =
		efx_rx_buffer(&channel->rx_queue, channel->rx_pkt_index);
	u8 *eh = efx_rx_buf_va(rx_buf);

	/* Read length from the prefix if necessary.  This already
	 * excludes the length of the prefix itself.
	 */
	if (rx_buf->flags & EFX_RX_PKT_PREFIX_LEN)
		rx_buf->len = le16_to_cpup((__le16 *)
					   (eh + efx->rx_packet_len_offset));

	/* If we're in loopback test, then pass the packet directly to the
	 * loopback layer, and free the rx_buf here
	 */
	if (unlikely(efx->loopback_selftest)) {
		struct efx_rx_queue *rx_queue;

		efx_loopback_rx_packet(efx, eh, rx_buf->len);
		rx_queue = efx_channel_get_rx_queue(channel);
		efx_free_rx_buffers(rx_queue, rx_buf,
				    channel->rx_pkt_n_frags);
		goto out;
	}

<<<<<<< HEAD
=======
	if (!efx_do_xdp(efx, channel, rx_buf, &eh))
		goto out;

>>>>>>> upstream/android-13
	if (unlikely(!(efx->net_dev->features & NETIF_F_RXCSUM)))
		rx_buf->flags &= ~EFX_RX_PKT_CSUMMED;

	if ((rx_buf->flags & EFX_RX_PKT_TCP) && !channel->type->receive_skb)
<<<<<<< HEAD
		efx_rx_packet_gro(channel, rx_buf, channel->rx_pkt_n_frags, eh);
=======
		efx_rx_packet_gro(channel, rx_buf, channel->rx_pkt_n_frags, eh, 0);
>>>>>>> upstream/android-13
	else
		efx_rx_deliver(channel, eh, rx_buf, channel->rx_pkt_n_frags);
out:
	channel->rx_pkt_n_frags = 0;
}
<<<<<<< HEAD

int efx_probe_rx_queue(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned int entries;
	int rc;

	/* Create the smallest power-of-two aligned ring */
	entries = max(roundup_pow_of_two(efx->rxq_entries), EFX_MIN_DMAQ_SIZE);
	EFX_WARN_ON_PARANOID(entries > EFX_MAX_DMAQ_SIZE);
	rx_queue->ptr_mask = entries - 1;

	netif_dbg(efx, probe, efx->net_dev,
		  "creating RX queue %d size %#x mask %#x\n",
		  efx_rx_queue_index(rx_queue), efx->rxq_entries,
		  rx_queue->ptr_mask);

	/* Allocate RX buffers */
	rx_queue->buffer = kcalloc(entries, sizeof(*rx_queue->buffer),
				   GFP_KERNEL);
	if (!rx_queue->buffer)
		return -ENOMEM;

	rc = efx_nic_probe_rx(rx_queue);
	if (rc) {
		kfree(rx_queue->buffer);
		rx_queue->buffer = NULL;
	}

	return rc;
}

static void efx_init_rx_recycle_ring(struct efx_nic *efx,
				     struct efx_rx_queue *rx_queue)
{
	unsigned int bufs_in_recycle_ring, page_ring_size;

	/* Set the RX recycle ring size */
#ifdef CONFIG_PPC64
	bufs_in_recycle_ring = EFX_RECYCLE_RING_SIZE_IOMMU;
#else
	if (iommu_present(&pci_bus_type))
		bufs_in_recycle_ring = EFX_RECYCLE_RING_SIZE_IOMMU;
	else
		bufs_in_recycle_ring = EFX_RECYCLE_RING_SIZE_NOIOMMU;
#endif /* CONFIG_PPC64 */

	page_ring_size = roundup_pow_of_two(bufs_in_recycle_ring /
					    efx->rx_bufs_per_page);
	rx_queue->page_ring = kcalloc(page_ring_size,
				      sizeof(*rx_queue->page_ring), GFP_KERNEL);
	rx_queue->page_ptr_mask = page_ring_size - 1;
}

void efx_init_rx_queue(struct efx_rx_queue *rx_queue)
{
	struct efx_nic *efx = rx_queue->efx;
	unsigned int max_fill, trigger, max_trigger;

	netif_dbg(rx_queue->efx, drv, rx_queue->efx->net_dev,
		  "initialising RX queue %d\n", efx_rx_queue_index(rx_queue));

	/* Initialise ptr fields */
	rx_queue->added_count = 0;
	rx_queue->notified_count = 0;
	rx_queue->removed_count = 0;
	rx_queue->min_fill = -1U;
	efx_init_rx_recycle_ring(efx, rx_queue);

	rx_queue->page_remove = 0;
	rx_queue->page_add = rx_queue->page_ptr_mask + 1;
	rx_queue->page_recycle_count = 0;
	rx_queue->page_recycle_failed = 0;
	rx_queue->page_recycle_full = 0;

	/* Initialise limit fields */
	max_fill = efx->rxq_entries - EFX_RXD_HEAD_ROOM;
	max_trigger =
		max_fill - efx->rx_pages_per_batch * efx->rx_bufs_per_page;
	if (rx_refill_threshold != 0) {
		trigger = max_fill * min(rx_refill_threshold, 100U) / 100U;
		if (trigger > max_trigger)
			trigger = max_trigger;
	} else {
		trigger = max_trigger;
	}

	rx_queue->max_fill = max_fill;
	rx_queue->fast_fill_trigger = trigger;
	rx_queue->refill_enabled = true;

	/* Set up RX descriptor ring */
	efx_nic_init_rx(rx_queue);
}

void efx_fini_rx_queue(struct efx_rx_queue *rx_queue)
{
	int i;
	struct efx_nic *efx = rx_queue->efx;
	struct efx_rx_buffer *rx_buf;

	netif_dbg(rx_queue->efx, drv, rx_queue->efx->net_dev,
		  "shutting down RX queue %d\n", efx_rx_queue_index(rx_queue));

	del_timer_sync(&rx_queue->slow_fill);

	/* Release RX buffers from the current read ptr to the write ptr */
	if (rx_queue->buffer) {
		for (i = rx_queue->removed_count; i < rx_queue->added_count;
		     i++) {
			unsigned index = i & rx_queue->ptr_mask;
			rx_buf = efx_rx_buffer(rx_queue, index);
			efx_fini_rx_buffer(rx_queue, rx_buf);
		}
	}

	/* Unmap and release the pages in the recycle ring. Remove the ring. */
	for (i = 0; i <= rx_queue->page_ptr_mask; i++) {
		struct page *page = rx_queue->page_ring[i];
		struct efx_rx_page_state *state;

		if (page == NULL)
			continue;

		state = page_address(page);
		dma_unmap_page(&efx->pci_dev->dev, state->dma_addr,
			       PAGE_SIZE << efx->rx_buffer_order,
			       DMA_FROM_DEVICE);
		put_page(page);
	}
	kfree(rx_queue->page_ring);
	rx_queue->page_ring = NULL;
}

void efx_remove_rx_queue(struct efx_rx_queue *rx_queue)
{
	netif_dbg(rx_queue->efx, drv, rx_queue->efx->net_dev,
		  "destroying RX queue %d\n", efx_rx_queue_index(rx_queue));

	efx_nic_remove_rx(rx_queue);

	kfree(rx_queue->buffer);
	rx_queue->buffer = NULL;
}


module_param(rx_refill_threshold, uint, 0444);
MODULE_PARM_DESC(rx_refill_threshold,
		 "RX descriptor ring refill threshold (%)");

#ifdef CONFIG_RFS_ACCEL

static void efx_filter_rfs_work(struct work_struct *data)
{
	struct efx_async_filter_insertion *req = container_of(data, struct efx_async_filter_insertion,
							      work);
	struct efx_nic *efx = netdev_priv(req->net_dev);
	struct efx_channel *channel = efx_get_channel(efx, req->rxq_index);
	int slot_idx = req - efx->rps_slot;
	struct efx_arfs_rule *rule;
	u16 arfs_id = 0;
	int rc;

	rc = efx->type->filter_insert(efx, &req->spec, true);
	if (rc >= 0)
		rc %= efx->type->max_rx_ip_filters;
	if (efx->rps_hash_table) {
		spin_lock_bh(&efx->rps_hash_lock);
		rule = efx_rps_hash_find(efx, &req->spec);
		/* The rule might have already gone, if someone else's request
		 * for the same spec was already worked and then expired before
		 * we got around to our work.  In that case we have nothing
		 * tying us to an arfs_id, meaning that as soon as the filter
		 * is considered for expiry it will be removed.
		 */
		if (rule) {
			if (rc < 0)
				rule->filter_id = EFX_ARFS_FILTER_ID_ERROR;
			else
				rule->filter_id = rc;
			arfs_id = rule->arfs_id;
		}
		spin_unlock_bh(&efx->rps_hash_lock);
	}
	if (rc >= 0) {
		/* Remember this so we can check whether to expire the filter
		 * later.
		 */
		mutex_lock(&efx->rps_mutex);
		channel->rps_flow_id[rc] = req->flow_id;
		++channel->rfs_filters_added;
		mutex_unlock(&efx->rps_mutex);

		if (req->spec.ether_type == htons(ETH_P_IP))
			netif_info(efx, rx_status, efx->net_dev,
				   "steering %s %pI4:%u:%pI4:%u to queue %u [flow %u filter %d id %u]\n",
				   (req->spec.ip_proto == IPPROTO_TCP) ? "TCP" : "UDP",
				   req->spec.rem_host, ntohs(req->spec.rem_port),
				   req->spec.loc_host, ntohs(req->spec.loc_port),
				   req->rxq_index, req->flow_id, rc, arfs_id);
		else
			netif_info(efx, rx_status, efx->net_dev,
				   "steering %s [%pI6]:%u:[%pI6]:%u to queue %u [flow %u filter %d id %u]\n",
				   (req->spec.ip_proto == IPPROTO_TCP) ? "TCP" : "UDP",
				   req->spec.rem_host, ntohs(req->spec.rem_port),
				   req->spec.loc_host, ntohs(req->spec.loc_port),
				   req->rxq_index, req->flow_id, rc, arfs_id);
	}

	/* Release references */
	clear_bit(slot_idx, &efx->rps_slot_map);
	dev_put(req->net_dev);
}

int efx_filter_rfs(struct net_device *net_dev, const struct sk_buff *skb,
		   u16 rxq_index, u32 flow_id)
{
	struct efx_nic *efx = netdev_priv(net_dev);
	struct efx_async_filter_insertion *req;
	struct efx_arfs_rule *rule;
	struct flow_keys fk;
	int slot_idx;
	bool new;
	int rc;

	/* find a free slot */
	for (slot_idx = 0; slot_idx < EFX_RPS_MAX_IN_FLIGHT; slot_idx++)
		if (!test_and_set_bit(slot_idx, &efx->rps_slot_map))
			break;
	if (slot_idx >= EFX_RPS_MAX_IN_FLIGHT)
		return -EBUSY;

	if (flow_id == RPS_FLOW_ID_INVALID) {
		rc = -EINVAL;
		goto out_clear;
	}

	if (!skb_flow_dissect_flow_keys(skb, &fk, 0)) {
		rc = -EPROTONOSUPPORT;
		goto out_clear;
	}

	if (fk.basic.n_proto != htons(ETH_P_IP) && fk.basic.n_proto != htons(ETH_P_IPV6)) {
		rc = -EPROTONOSUPPORT;
		goto out_clear;
	}
	if (fk.control.flags & FLOW_DIS_IS_FRAGMENT) {
		rc = -EPROTONOSUPPORT;
		goto out_clear;
	}

	req = efx->rps_slot + slot_idx;
	efx_filter_init_rx(&req->spec, EFX_FILTER_PRI_HINT,
			   efx->rx_scatter ? EFX_FILTER_FLAG_RX_SCATTER : 0,
			   rxq_index);
	req->spec.match_flags =
		EFX_FILTER_MATCH_ETHER_TYPE | EFX_FILTER_MATCH_IP_PROTO |
		EFX_FILTER_MATCH_LOC_HOST | EFX_FILTER_MATCH_LOC_PORT |
		EFX_FILTER_MATCH_REM_HOST | EFX_FILTER_MATCH_REM_PORT;
	req->spec.ether_type = fk.basic.n_proto;
	req->spec.ip_proto = fk.basic.ip_proto;

	if (fk.basic.n_proto == htons(ETH_P_IP)) {
		req->spec.rem_host[0] = fk.addrs.v4addrs.src;
		req->spec.loc_host[0] = fk.addrs.v4addrs.dst;
	} else {
		memcpy(req->spec.rem_host, &fk.addrs.v6addrs.src,
		       sizeof(struct in6_addr));
		memcpy(req->spec.loc_host, &fk.addrs.v6addrs.dst,
		       sizeof(struct in6_addr));
	}

	req->spec.rem_port = fk.ports.src;
	req->spec.loc_port = fk.ports.dst;

	if (efx->rps_hash_table) {
		/* Add it to ARFS hash table */
		spin_lock(&efx->rps_hash_lock);
		rule = efx_rps_hash_add(efx, &req->spec, &new);
		if (!rule) {
			rc = -ENOMEM;
			goto out_unlock;
		}
		if (new)
			rule->arfs_id = efx->rps_next_id++ % RPS_NO_FILTER;
		rc = rule->arfs_id;
		/* Skip if existing or pending filter already does the right thing */
		if (!new && rule->rxq_index == rxq_index &&
		    rule->filter_id >= EFX_ARFS_FILTER_ID_PENDING)
			goto out_unlock;
		rule->rxq_index = rxq_index;
		rule->filter_id = EFX_ARFS_FILTER_ID_PENDING;
		spin_unlock(&efx->rps_hash_lock);
	} else {
		/* Without an ARFS hash table, we just use arfs_id 0 for all
		 * filters.  This means if multiple flows hash to the same
		 * flow_id, all but the most recently touched will be eligible
		 * for expiry.
		 */
		rc = 0;
	}

	/* Queue the request */
	dev_hold(req->net_dev = net_dev);
	INIT_WORK(&req->work, efx_filter_rfs_work);
	req->rxq_index = rxq_index;
	req->flow_id = flow_id;
	schedule_work(&req->work);
	return rc;
out_unlock:
	spin_unlock(&efx->rps_hash_lock);
out_clear:
	clear_bit(slot_idx, &efx->rps_slot_map);
	return rc;
}

bool __efx_filter_rfs_expire(struct efx_nic *efx, unsigned int quota)
{
	bool (*expire_one)(struct efx_nic *efx, u32 flow_id, unsigned int index);
	unsigned int channel_idx, index, size;
	u32 flow_id;

	if (!mutex_trylock(&efx->rps_mutex))
		return false;
	expire_one = efx->type->filter_rfs_expire_one;
	channel_idx = efx->rps_expire_channel;
	index = efx->rps_expire_index;
	size = efx->type->max_rx_ip_filters;
	while (quota--) {
		struct efx_channel *channel = efx_get_channel(efx, channel_idx);
		flow_id = channel->rps_flow_id[index];

		if (flow_id != RPS_FLOW_ID_INVALID &&
		    expire_one(efx, flow_id, index)) {
			netif_info(efx, rx_status, efx->net_dev,
				   "expired filter %d [queue %u flow %u]\n",
				   index, channel_idx, flow_id);
			channel->rps_flow_id[index] = RPS_FLOW_ID_INVALID;
		}
		if (++index == size) {
			if (++channel_idx == efx->n_channels)
				channel_idx = 0;
			index = 0;
		}
	}
	efx->rps_expire_channel = channel_idx;
	efx->rps_expire_index = index;

	mutex_unlock(&efx->rps_mutex);
	return true;
}

#endif /* CONFIG_RFS_ACCEL */

/**
 * efx_filter_is_mc_recipient - test whether spec is a multicast recipient
 * @spec: Specification to test
 *
 * Return: %true if the specification is a non-drop RX filter that
 * matches a local MAC address I/G bit value of 1 or matches a local
 * IPv4 or IPv6 address value in the respective multicast address
 * range.  Otherwise %false.
 */
bool efx_filter_is_mc_recipient(const struct efx_filter_spec *spec)
{
	if (!(spec->flags & EFX_FILTER_FLAG_RX) ||
	    spec->dmaq_id == EFX_FILTER_RX_DMAQ_ID_DROP)
		return false;

	if (spec->match_flags &
	    (EFX_FILTER_MATCH_LOC_MAC | EFX_FILTER_MATCH_LOC_MAC_IG) &&
	    is_multicast_ether_addr(spec->loc_mac))
		return true;

	if ((spec->match_flags &
	     (EFX_FILTER_MATCH_ETHER_TYPE | EFX_FILTER_MATCH_LOC_HOST)) ==
	    (EFX_FILTER_MATCH_ETHER_TYPE | EFX_FILTER_MATCH_LOC_HOST)) {
		if (spec->ether_type == htons(ETH_P_IP) &&
		    ipv4_is_multicast(spec->loc_host[0]))
			return true;
		if (spec->ether_type == htons(ETH_P_IPV6) &&
		    ((const u8 *)spec->loc_host)[0] == 0xff)
			return true;
	}

	return false;
}
=======
>>>>>>> upstream/android-13
