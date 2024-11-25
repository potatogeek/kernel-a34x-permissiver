<<<<<<< HEAD
/*
 * Copyright (C) 2016 Felix Fietkau <nbd@nbd.name>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
=======
// SPDX-License-Identifier: ISC
/*
 * Copyright (C) 2016 Felix Fietkau <nbd@nbd.name>
>>>>>>> upstream/android-13
 */

#include <linux/dma-mapping.h>
#include "mt76.h"
#include "dma.h"

<<<<<<< HEAD
#define DMA_DUMMY_TXWI	((void *) ~0)

static int
mt76_dma_alloc_queue(struct mt76_dev *dev, struct mt76_queue *q)
{
	int size;
	int i;

	spin_lock_init(&q->lock);
	INIT_LIST_HEAD(&q->swq);
=======
static struct mt76_txwi_cache *
mt76_alloc_txwi(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t;
	dma_addr_t addr;
	u8 *txwi;
	int size;

	size = L1_CACHE_ALIGN(dev->drv->txwi_size + sizeof(*t));
	txwi = devm_kzalloc(dev->dev, size, GFP_ATOMIC);
	if (!txwi)
		return NULL;

	addr = dma_map_single(dev->dev, txwi, dev->drv->txwi_size,
			      DMA_TO_DEVICE);
	t = (struct mt76_txwi_cache *)(txwi + dev->drv->txwi_size);
	t->dma_addr = addr;

	return t;
}

static struct mt76_txwi_cache *
__mt76_get_txwi(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t = NULL;

	spin_lock(&dev->lock);
	if (!list_empty(&dev->txwi_cache)) {
		t = list_first_entry(&dev->txwi_cache, struct mt76_txwi_cache,
				     list);
		list_del(&t->list);
	}
	spin_unlock(&dev->lock);

	return t;
}

static struct mt76_txwi_cache *
mt76_get_txwi(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t = __mt76_get_txwi(dev);

	if (t)
		return t;

	return mt76_alloc_txwi(dev);
}

void
mt76_put_txwi(struct mt76_dev *dev, struct mt76_txwi_cache *t)
{
	if (!t)
		return;

	spin_lock(&dev->lock);
	list_add(&t->list, &dev->txwi_cache);
	spin_unlock(&dev->lock);
}
EXPORT_SYMBOL_GPL(mt76_put_txwi);

static void
mt76_free_pending_txwi(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t;

	local_bh_disable();
	while ((t = __mt76_get_txwi(dev)) != NULL)
		dma_unmap_single(dev->dev, t->dma_addr, dev->drv->txwi_size,
				 DMA_TO_DEVICE);
	local_bh_enable();
}

static void
mt76_dma_sync_idx(struct mt76_dev *dev, struct mt76_queue *q)
{
	writel(q->desc_dma, &q->regs->desc_base);
	writel(q->ndesc, &q->regs->ring_size);
	q->head = readl(&q->regs->dma_idx);
	q->tail = q->head;
}

static void
mt76_dma_queue_reset(struct mt76_dev *dev, struct mt76_queue *q)
{
	int i;

	if (!q)
		return;

	/* clear descriptors */
	for (i = 0; i < q->ndesc; i++)
		q->desc[i].ctrl = cpu_to_le32(MT_DMA_CTL_DMA_DONE);

	writel(0, &q->regs->cpu_idx);
	writel(0, &q->regs->dma_idx);
	mt76_dma_sync_idx(dev, q);
}

static int
mt76_dma_alloc_queue(struct mt76_dev *dev, struct mt76_queue *q,
		     int idx, int n_desc, int bufsize,
		     u32 ring_base)
{
	int size;

	spin_lock_init(&q->lock);
	spin_lock_init(&q->cleanup_lock);

	q->regs = dev->mmio.regs + ring_base + idx * MT_RING_SIZE;
	q->ndesc = n_desc;
	q->buf_size = bufsize;
	q->hw_idx = idx;
>>>>>>> upstream/android-13

	size = q->ndesc * sizeof(struct mt76_desc);
	q->desc = dmam_alloc_coherent(dev->dev, size, &q->desc_dma, GFP_KERNEL);
	if (!q->desc)
		return -ENOMEM;

	size = q->ndesc * sizeof(*q->entry);
	q->entry = devm_kzalloc(dev->dev, size, GFP_KERNEL);
	if (!q->entry)
		return -ENOMEM;

<<<<<<< HEAD
	/* clear descriptors */
	for (i = 0; i < q->ndesc; i++)
		q->desc[i].ctrl = cpu_to_le32(MT_DMA_CTL_DMA_DONE);

	iowrite32(q->desc_dma, &q->regs->desc_base);
	iowrite32(0, &q->regs->cpu_idx);
	iowrite32(0, &q->regs->dma_idx);
	iowrite32(q->ndesc, &q->regs->ring_size);
=======
	mt76_dma_queue_reset(dev, q);
>>>>>>> upstream/android-13

	return 0;
}

static int
mt76_dma_add_buf(struct mt76_dev *dev, struct mt76_queue *q,
		 struct mt76_queue_buf *buf, int nbufs, u32 info,
		 struct sk_buff *skb, void *txwi)
{
<<<<<<< HEAD
=======
	struct mt76_queue_entry *entry;
>>>>>>> upstream/android-13
	struct mt76_desc *desc;
	u32 ctrl;
	int i, idx = -1;

<<<<<<< HEAD
	if (txwi)
		q->entry[q->head].txwi = DMA_DUMMY_TXWI;
=======
	if (txwi) {
		q->entry[q->head].txwi = DMA_DUMMY_DATA;
		q->entry[q->head].skip_buf0 = true;
	}
>>>>>>> upstream/android-13

	for (i = 0; i < nbufs; i += 2, buf += 2) {
		u32 buf0 = buf[0].addr, buf1 = 0;

<<<<<<< HEAD
		ctrl = FIELD_PREP(MT_DMA_CTL_SD_LEN0, buf[0].len);
		if (i < nbufs - 1) {
			buf1 = buf[1].addr;
			ctrl |= FIELD_PREP(MT_DMA_CTL_SD_LEN1, buf[1].len);
=======
		idx = q->head;
		q->head = (q->head + 1) % q->ndesc;

		desc = &q->desc[idx];
		entry = &q->entry[idx];

		if (buf[0].skip_unmap)
			entry->skip_buf0 = true;
		entry->skip_buf1 = i == nbufs - 1;

		entry->dma_addr[0] = buf[0].addr;
		entry->dma_len[0] = buf[0].len;

		ctrl = FIELD_PREP(MT_DMA_CTL_SD_LEN0, buf[0].len);
		if (i < nbufs - 1) {
			entry->dma_addr[1] = buf[1].addr;
			entry->dma_len[1] = buf[1].len;
			buf1 = buf[1].addr;
			ctrl |= FIELD_PREP(MT_DMA_CTL_SD_LEN1, buf[1].len);
			if (buf[1].skip_unmap)
				entry->skip_buf1 = true;
>>>>>>> upstream/android-13
		}

		if (i == nbufs - 1)
			ctrl |= MT_DMA_CTL_LAST_SEC0;
		else if (i == nbufs - 2)
			ctrl |= MT_DMA_CTL_LAST_SEC1;

<<<<<<< HEAD
		idx = q->head;
		q->head = (q->head + 1) % q->ndesc;

		desc = &q->desc[idx];

=======
>>>>>>> upstream/android-13
		WRITE_ONCE(desc->buf0, cpu_to_le32(buf0));
		WRITE_ONCE(desc->buf1, cpu_to_le32(buf1));
		WRITE_ONCE(desc->info, cpu_to_le32(info));
		WRITE_ONCE(desc->ctrl, cpu_to_le32(ctrl));

		q->queued++;
	}

	q->entry[idx].txwi = txwi;
	q->entry[idx].skb = skb;
<<<<<<< HEAD
=======
	q->entry[idx].wcid = 0xffff;
>>>>>>> upstream/android-13

	return idx;
}

static void
mt76_dma_tx_cleanup_idx(struct mt76_dev *dev, struct mt76_queue *q, int idx,
			struct mt76_queue_entry *prev_e)
{
	struct mt76_queue_entry *e = &q->entry[idx];
<<<<<<< HEAD
	__le32 __ctrl = READ_ONCE(q->desc[idx].ctrl);
	u32 ctrl = le32_to_cpu(__ctrl);

	if (!e->txwi || !e->skb) {
		__le32 addr = READ_ONCE(q->desc[idx].buf0);
		u32 len = FIELD_GET(MT_DMA_CTL_SD_LEN0, ctrl);

		dma_unmap_single(dev->dev, le32_to_cpu(addr), len,
				 DMA_TO_DEVICE);
	}

	if (!(ctrl & MT_DMA_CTL_LAST_SEC0)) {
		__le32 addr = READ_ONCE(q->desc[idx].buf1);
		u32 len = FIELD_GET(MT_DMA_CTL_SD_LEN1, ctrl);

		dma_unmap_single(dev->dev, le32_to_cpu(addr), len,
				 DMA_TO_DEVICE);
	}

	if (e->txwi == DMA_DUMMY_TXWI)
		e->txwi = NULL;

=======

	if (!e->skip_buf0)
		dma_unmap_single(dev->dev, e->dma_addr[0], e->dma_len[0],
				 DMA_TO_DEVICE);

	if (!e->skip_buf1)
		dma_unmap_single(dev->dev, e->dma_addr[1], e->dma_len[1],
				 DMA_TO_DEVICE);

	if (e->txwi == DMA_DUMMY_DATA)
		e->txwi = NULL;

	if (e->skb == DMA_DUMMY_DATA)
		e->skb = NULL;

>>>>>>> upstream/android-13
	*prev_e = *e;
	memset(e, 0, sizeof(*e));
}

static void
<<<<<<< HEAD
mt76_dma_sync_idx(struct mt76_dev *dev, struct mt76_queue *q)
{
	q->head = ioread32(&q->regs->dma_idx);
	q->tail = q->head;
	iowrite32(q->head, &q->regs->cpu_idx);
}

static void
mt76_dma_tx_cleanup(struct mt76_dev *dev, enum mt76_txq_id qid, bool flush)
{
	struct mt76_queue *q = &dev->q_tx[qid];
	struct mt76_queue_entry entry;
	bool wake = false;
	int last;

	if (!q->ndesc)
		return;

	spin_lock_bh(&q->lock);
	if (flush)
		last = -1;
	else
		last = ioread32(&q->regs->dma_idx);

	while (q->queued && q->tail != last) {
		mt76_dma_tx_cleanup_idx(dev, q, q->tail, &entry);
		if (entry.schedule)
			q->swq_queued--;

		if (entry.skb)
			dev->drv->tx_complete_skb(dev, q, &entry, flush);

		if (entry.txwi) {
			mt76_put_txwi(dev, entry.txwi);
			wake = true;
		}

		q->tail = (q->tail + 1) % q->ndesc;
		q->queued--;

		if (!flush && q->tail == last)
			last = ioread32(&q->regs->dma_idx);
	}

	if (!flush)
		mt76_txq_schedule(dev, q);
	else
		mt76_dma_sync_idx(dev, q);

	wake = wake && qid < IEEE80211_NUM_ACS && q->queued < q->ndesc - 8;

	if (!q->queued)
		wake_up(&dev->tx_wait);

	spin_unlock_bh(&q->lock);

	if (wake)
		ieee80211_wake_queue(dev->hw, qid);
=======
mt76_dma_kick_queue(struct mt76_dev *dev, struct mt76_queue *q)
{
	wmb();
	writel(q->head, &q->regs->cpu_idx);
}

static void
mt76_dma_tx_cleanup(struct mt76_dev *dev, struct mt76_queue *q, bool flush)
{
	struct mt76_queue_entry entry;
	int last;

	if (!q)
		return;

	spin_lock_bh(&q->cleanup_lock);
	if (flush)
		last = -1;
	else
		last = readl(&q->regs->dma_idx);

	while (q->queued > 0 && q->tail != last) {
		mt76_dma_tx_cleanup_idx(dev, q, q->tail, &entry);
		mt76_queue_tx_complete(dev, q, &entry);

		if (entry.txwi) {
			if (!(dev->drv->drv_flags & MT_DRV_TXWI_NO_FREE))
				mt76_put_txwi(dev, entry.txwi);
		}

		if (!flush && q->tail == last)
			last = readl(&q->regs->dma_idx);

	}
	spin_unlock_bh(&q->cleanup_lock);

	if (flush) {
		spin_lock_bh(&q->lock);
		mt76_dma_sync_idx(dev, q);
		mt76_dma_kick_queue(dev, q);
		spin_unlock_bh(&q->lock);
	}

	if (!q->queued)
		wake_up(&dev->tx_wait);
>>>>>>> upstream/android-13
}

static void *
mt76_dma_get_buf(struct mt76_dev *dev, struct mt76_queue *q, int idx,
		 int *len, u32 *info, bool *more)
{
	struct mt76_queue_entry *e = &q->entry[idx];
	struct mt76_desc *desc = &q->desc[idx];
	dma_addr_t buf_addr;
	void *buf = e->buf;
	int buf_len = SKB_WITH_OVERHEAD(q->buf_size);

<<<<<<< HEAD
	buf_addr = le32_to_cpu(READ_ONCE(desc->buf0));
=======
	buf_addr = e->dma_addr[0];
>>>>>>> upstream/android-13
	if (len) {
		u32 ctl = le32_to_cpu(READ_ONCE(desc->ctrl));
		*len = FIELD_GET(MT_DMA_CTL_SD_LEN0, ctl);
		*more = !(ctl & MT_DMA_CTL_LAST_SEC0);
	}

	if (info)
		*info = le32_to_cpu(desc->info);

	dma_unmap_single(dev->dev, buf_addr, buf_len, DMA_FROM_DEVICE);
	e->buf = NULL;

	return buf;
}

static void *
mt76_dma_dequeue(struct mt76_dev *dev, struct mt76_queue *q, bool flush,
		 int *len, u32 *info, bool *more)
{
	int idx = q->tail;

	*more = false;
	if (!q->queued)
		return NULL;

<<<<<<< HEAD
	if (!flush && !(q->desc[idx].ctrl & cpu_to_le32(MT_DMA_CTL_DMA_DONE)))
=======
	if (flush)
		q->desc[idx].ctrl |= cpu_to_le32(MT_DMA_CTL_DMA_DONE);
	else if (!(q->desc[idx].ctrl & cpu_to_le32(MT_DMA_CTL_DMA_DONE)))
>>>>>>> upstream/android-13
		return NULL;

	q->tail = (q->tail + 1) % q->ndesc;
	q->queued--;

	return mt76_dma_get_buf(dev, q, idx, len, info, more);
}

<<<<<<< HEAD
static void
mt76_dma_kick_queue(struct mt76_dev *dev, struct mt76_queue *q)
{
	iowrite32(q->head, &q->regs->cpu_idx);
}

int mt76_dma_tx_queue_skb(struct mt76_dev *dev, struct mt76_queue *q,
			  struct sk_buff *skb, struct mt76_wcid *wcid,
			  struct ieee80211_sta *sta)
{
	struct mt76_queue_entry e;
	struct mt76_txwi_cache *t;
	struct mt76_queue_buf buf[32];
	struct sk_buff *iter;
	dma_addr_t addr;
	int len;
	u32 tx_info = 0;
	int n, ret;

	t = mt76_get_txwi(dev);
	if (!t) {
		ieee80211_free_txskb(dev->hw, skb);
		return -ENOMEM;
	}

	dma_sync_single_for_cpu(dev->dev, t->dma_addr, sizeof(t->txwi),
				DMA_TO_DEVICE);
	ret = dev->drv->tx_prepare_skb(dev, &t->txwi, skb, q, wcid, sta,
				       &tx_info);
	dma_sync_single_for_device(dev->dev, t->dma_addr, sizeof(t->txwi),
				   DMA_TO_DEVICE);
	if (ret < 0)
		goto free;

	len = skb->len - skb->data_len;
	addr = dma_map_single(dev->dev, skb->data, len, DMA_TO_DEVICE);
	if (dma_mapping_error(dev->dev, addr)) {
		ret = -ENOMEM;
		goto free;
	}

	n = 0;
	buf[n].addr = t->dma_addr;
	buf[n++].len = dev->drv->txwi_size;
	buf[n].addr = addr;
	buf[n++].len = len;

	skb_walk_frags(skb, iter) {
		if (n == ARRAY_SIZE(buf))
=======
static int
mt76_dma_tx_queue_skb_raw(struct mt76_dev *dev, struct mt76_queue *q,
			  struct sk_buff *skb, u32 tx_info)
{
	struct mt76_queue_buf buf = {};
	dma_addr_t addr;

	if (q->queued + 1 >= q->ndesc - 1)
		goto error;

	addr = dma_map_single(dev->dev, skb->data, skb->len,
			      DMA_TO_DEVICE);
	if (unlikely(dma_mapping_error(dev->dev, addr)))
		goto error;

	buf.addr = addr;
	buf.len = skb->len;

	spin_lock_bh(&q->lock);
	mt76_dma_add_buf(dev, q, &buf, 1, tx_info, skb, NULL);
	mt76_dma_kick_queue(dev, q);
	spin_unlock_bh(&q->lock);

	return 0;

error:
	dev_kfree_skb(skb);
	return -ENOMEM;
}

static int
mt76_dma_tx_queue_skb(struct mt76_dev *dev, struct mt76_queue *q,
		      struct sk_buff *skb, struct mt76_wcid *wcid,
		      struct ieee80211_sta *sta)
{
	struct ieee80211_tx_status status = {
		.sta = sta,
	};
	struct mt76_tx_info tx_info = {
		.skb = skb,
	};
	struct ieee80211_hw *hw;
	int len, n = 0, ret = -ENOMEM;
	struct mt76_txwi_cache *t;
	struct sk_buff *iter;
	dma_addr_t addr;
	u8 *txwi;

	t = mt76_get_txwi(dev);
	if (!t)
		goto free_skb;

	txwi = mt76_get_txwi_ptr(dev, t);

	skb->prev = skb->next = NULL;
	if (dev->drv->drv_flags & MT_DRV_TX_ALIGNED4_SKBS)
		mt76_insert_hdr_pad(skb);

	len = skb_headlen(skb);
	addr = dma_map_single(dev->dev, skb->data, len, DMA_TO_DEVICE);
	if (unlikely(dma_mapping_error(dev->dev, addr)))
		goto free;

	tx_info.buf[n].addr = t->dma_addr;
	tx_info.buf[n++].len = dev->drv->txwi_size;
	tx_info.buf[n].addr = addr;
	tx_info.buf[n++].len = len;

	skb_walk_frags(skb, iter) {
		if (n == ARRAY_SIZE(tx_info.buf))
>>>>>>> upstream/android-13
			goto unmap;

		addr = dma_map_single(dev->dev, iter->data, iter->len,
				      DMA_TO_DEVICE);
<<<<<<< HEAD
		if (dma_mapping_error(dev->dev, addr))
			goto unmap;

		buf[n].addr = addr;
		buf[n++].len = iter->len;
	}

	if (q->queued + (n + 1) / 2 >= q->ndesc - 1)
		goto unmap;

	return dev->queue_ops->add_buf(dev, q, buf, n, tx_info, skb, t);

unmap:
	ret = -ENOMEM;
	for (n--; n > 0; n--)
		dma_unmap_single(dev->dev, buf[n].addr, buf[n].len,
				 DMA_TO_DEVICE);

free:
	e.skb = skb;
	e.txwi = t;
	dev->drv->tx_complete_skb(dev, q, &e, true);
	mt76_put_txwi(dev, t);
	return ret;
}
EXPORT_SYMBOL_GPL(mt76_dma_tx_queue_skb);

static int
mt76_dma_rx_fill(struct mt76_dev *dev, struct mt76_queue *q, bool napi)
=======
		if (unlikely(dma_mapping_error(dev->dev, addr)))
			goto unmap;

		tx_info.buf[n].addr = addr;
		tx_info.buf[n++].len = iter->len;
	}
	tx_info.nbuf = n;

	if (q->queued + (tx_info.nbuf + 1) / 2 >= q->ndesc - 1) {
		ret = -ENOMEM;
		goto unmap;
	}

	dma_sync_single_for_cpu(dev->dev, t->dma_addr, dev->drv->txwi_size,
				DMA_TO_DEVICE);
	ret = dev->drv->tx_prepare_skb(dev, txwi, q->qid, wcid, sta, &tx_info);
	dma_sync_single_for_device(dev->dev, t->dma_addr, dev->drv->txwi_size,
				   DMA_TO_DEVICE);
	if (ret < 0)
		goto unmap;

	return mt76_dma_add_buf(dev, q, tx_info.buf, tx_info.nbuf,
				tx_info.info, tx_info.skb, t);

unmap:
	for (n--; n > 0; n--)
		dma_unmap_single(dev->dev, tx_info.buf[n].addr,
				 tx_info.buf[n].len, DMA_TO_DEVICE);

free:
#ifdef CONFIG_NL80211_TESTMODE
	/* fix tx_done accounting on queue overflow */
	if (mt76_is_testmode_skb(dev, skb, &hw)) {
		struct mt76_phy *phy = hw->priv;

		if (tx_info.skb == phy->test.tx_skb)
			phy->test.tx_done--;
	}
#endif

	mt76_put_txwi(dev, t);

free_skb:
	status.skb = tx_info.skb;
	hw = mt76_tx_status_get_hw(dev, tx_info.skb);
	ieee80211_tx_status_ext(hw, &status);

	return ret;
}

static int
mt76_dma_rx_fill(struct mt76_dev *dev, struct mt76_queue *q)
>>>>>>> upstream/android-13
{
	dma_addr_t addr;
	void *buf;
	int frames = 0;
	int len = SKB_WITH_OVERHEAD(q->buf_size);
	int offset = q->buf_offset;
<<<<<<< HEAD
	int idx;
	void *(*alloc)(unsigned int fragsz);

	if (napi)
		alloc = napi_alloc_frag;
	else
		alloc = netdev_alloc_frag;
=======
>>>>>>> upstream/android-13

	spin_lock_bh(&q->lock);

	while (q->queued < q->ndesc - 1) {
		struct mt76_queue_buf qbuf;

<<<<<<< HEAD
		buf = alloc(q->buf_size);
=======
		buf = page_frag_alloc(&q->rx_page, q->buf_size, GFP_ATOMIC);
>>>>>>> upstream/android-13
		if (!buf)
			break;

		addr = dma_map_single(dev->dev, buf, len, DMA_FROM_DEVICE);
<<<<<<< HEAD
		if (dma_mapping_error(dev->dev, addr)) {
=======
		if (unlikely(dma_mapping_error(dev->dev, addr))) {
>>>>>>> upstream/android-13
			skb_free_frag(buf);
			break;
		}

		qbuf.addr = addr + offset;
		qbuf.len = len - offset;
<<<<<<< HEAD
		idx = mt76_dma_add_buf(dev, q, &qbuf, 1, 0, buf, NULL);
=======
		qbuf.skip_unmap = false;
		mt76_dma_add_buf(dev, q, &qbuf, 1, 0, buf, NULL);
>>>>>>> upstream/android-13
		frames++;
	}

	if (frames)
		mt76_dma_kick_queue(dev, q);

	spin_unlock_bh(&q->lock);

	return frames;
}

static void
mt76_dma_rx_cleanup(struct mt76_dev *dev, struct mt76_queue *q)
{
<<<<<<< HEAD
=======
	struct page *page;
>>>>>>> upstream/android-13
	void *buf;
	bool more;

	spin_lock_bh(&q->lock);
	do {
		buf = mt76_dma_dequeue(dev, q, true, NULL, NULL, &more);
		if (!buf)
			break;

		skb_free_frag(buf);
	} while (1);
	spin_unlock_bh(&q->lock);
<<<<<<< HEAD
=======

	if (!q->rx_page.va)
		return;

	page = virt_to_page(q->rx_page.va);
	__page_frag_cache_drain(page, q->rx_page.pagecnt_bias);
	memset(&q->rx_page, 0, sizeof(q->rx_page));
>>>>>>> upstream/android-13
}

static void
mt76_dma_rx_reset(struct mt76_dev *dev, enum mt76_rxq_id qid)
{
	struct mt76_queue *q = &dev->q_rx[qid];
	int i;

	for (i = 0; i < q->ndesc; i++)
<<<<<<< HEAD
		q->desc[i].ctrl &= ~cpu_to_le32(MT_DMA_CTL_DMA_DONE);

	mt76_dma_rx_cleanup(dev, q);
	mt76_dma_sync_idx(dev, q);
	mt76_dma_rx_fill(dev, q, false);
=======
		q->desc[i].ctrl = cpu_to_le32(MT_DMA_CTL_DMA_DONE);

	mt76_dma_rx_cleanup(dev, q);
	mt76_dma_sync_idx(dev, q);
	mt76_dma_rx_fill(dev, q);

	if (!q->rx_head)
		return;

	dev_kfree_skb(q->rx_head);
	q->rx_head = NULL;
>>>>>>> upstream/android-13
}

static void
mt76_add_fragment(struct mt76_dev *dev, struct mt76_queue *q, void *data,
		  int len, bool more)
{
	struct sk_buff *skb = q->rx_head;
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	int nr_frags = shinfo->nr_frags;

	if (nr_frags < ARRAY_SIZE(shinfo->frags)) {
		struct page *page = virt_to_head_page(data);
		int offset = data - page_address(page) + q->buf_offset;

		skb_add_rx_frag(skb, nr_frags, page, offset, len, q->buf_size);
	} else {
		skb_free_frag(data);
	}

	if (more)
		return;

	q->rx_head = NULL;
	if (nr_frags < ARRAY_SIZE(shinfo->frags))
		dev->drv->rx_skb(dev, q - dev->q_rx, skb);
	else
		dev_kfree_skb(skb);
}

static int
mt76_dma_rx_process(struct mt76_dev *dev, struct mt76_queue *q, int budget)
{
<<<<<<< HEAD
	struct sk_buff *skb;
	unsigned char *data;
	int len;
	int done = 0;
=======
	int len, data_len, done = 0;
	struct sk_buff *skb;
	unsigned char *data;
>>>>>>> upstream/android-13
	bool more;

	while (done < budget) {
		u32 info;

		data = mt76_dma_dequeue(dev, q, false, &len, &info, &more);
		if (!data)
			break;

<<<<<<< HEAD
=======
		if (q->rx_head)
			data_len = q->buf_size;
		else
			data_len = SKB_WITH_OVERHEAD(q->buf_size);

		if (data_len < len + q->buf_offset) {
			dev_kfree_skb(q->rx_head);
			q->rx_head = NULL;

			skb_free_frag(data);
			continue;
		}

>>>>>>> upstream/android-13
		if (q->rx_head) {
			mt76_add_fragment(dev, q, data, len, more);
			continue;
		}

		skb = build_skb(data, q->buf_size);
		if (!skb) {
			skb_free_frag(data);
			continue;
		}
<<<<<<< HEAD

		skb_reserve(skb, q->buf_offset);
		if (skb->tail + len > skb->end) {
			dev_kfree_skb(skb);
			continue;
		}

		if (q == &dev->q_rx[MT_RXQ_MCU]) {
			u32 *rxfce = (u32 *) skb->cb;
=======
		skb_reserve(skb, q->buf_offset);

		if (q == &dev->q_rx[MT_RXQ_MCU]) {
			u32 *rxfce = (u32 *)skb->cb;
>>>>>>> upstream/android-13
			*rxfce = info;
		}

		__skb_put(skb, len);
		done++;

		if (more) {
			q->rx_head = skb;
			continue;
		}

		dev->drv->rx_skb(dev, q - dev->q_rx, skb);
	}

<<<<<<< HEAD
	mt76_dma_rx_fill(dev, q, true);
	return done;
}

static int
mt76_dma_rx_poll(struct napi_struct *napi, int budget)
=======
	mt76_dma_rx_fill(dev, q);
	return done;
}

int mt76_dma_rx_poll(struct napi_struct *napi, int budget)
>>>>>>> upstream/android-13
{
	struct mt76_dev *dev;
	int qid, done = 0, cur;

	dev = container_of(napi->dev, struct mt76_dev, napi_dev);
	qid = napi - dev->napi;

	rcu_read_lock();

	do {
		cur = mt76_dma_rx_process(dev, &dev->q_rx[qid], budget - done);
		mt76_rx_poll_complete(dev, qid, napi);
		done += cur;
	} while (cur && done < budget);

	rcu_read_unlock();

<<<<<<< HEAD
	if (done < budget) {
		napi_complete(napi);
		dev->drv->rx_poll_complete(dev, qid);
	}

	return done;
}

static int
mt76_dma_init(struct mt76_dev *dev)
=======
	if (done < budget && napi_complete(napi))
		dev->drv->rx_poll_complete(dev, qid);

	return done;
}
EXPORT_SYMBOL_GPL(mt76_dma_rx_poll);

static int
mt76_dma_init(struct mt76_dev *dev,
	      int (*poll)(struct napi_struct *napi, int budget))
>>>>>>> upstream/android-13
{
	int i;

	init_dummy_netdev(&dev->napi_dev);
<<<<<<< HEAD

	for (i = 0; i < ARRAY_SIZE(dev->q_rx); i++) {
		netif_napi_add(&dev->napi_dev, &dev->napi[i], mt76_dma_rx_poll,
			       64);
		mt76_dma_rx_fill(dev, &dev->q_rx[i], false);
		skb_queue_head_init(&dev->rx_skb[i]);
=======
	init_dummy_netdev(&dev->tx_napi_dev);
	snprintf(dev->napi_dev.name, sizeof(dev->napi_dev.name), "%s",
		 wiphy_name(dev->hw->wiphy));
	dev->napi_dev.threaded = 1;

	mt76_for_each_q_rx(dev, i) {
		netif_napi_add(&dev->napi_dev, &dev->napi[i], poll, 64);
		mt76_dma_rx_fill(dev, &dev->q_rx[i]);
>>>>>>> upstream/android-13
		napi_enable(&dev->napi[i]);
	}

	return 0;
}

static const struct mt76_queue_ops mt76_dma_ops = {
	.init = mt76_dma_init,
	.alloc = mt76_dma_alloc_queue,
<<<<<<< HEAD
	.add_buf = mt76_dma_add_buf,
	.tx_queue_skb = mt76_dma_tx_queue_skb,
	.tx_cleanup = mt76_dma_tx_cleanup,
=======
	.reset_q = mt76_dma_queue_reset,
	.tx_queue_skb_raw = mt76_dma_tx_queue_skb_raw,
	.tx_queue_skb = mt76_dma_tx_queue_skb,
	.tx_cleanup = mt76_dma_tx_cleanup,
	.rx_cleanup = mt76_dma_rx_cleanup,
>>>>>>> upstream/android-13
	.rx_reset = mt76_dma_rx_reset,
	.kick = mt76_dma_kick_queue,
};

<<<<<<< HEAD
int mt76_dma_attach(struct mt76_dev *dev)
{
	dev->queue_ops = &mt76_dma_ops;
	return 0;
=======
void mt76_dma_attach(struct mt76_dev *dev)
{
	dev->queue_ops = &mt76_dma_ops;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mt76_dma_attach);

void mt76_dma_cleanup(struct mt76_dev *dev)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(dev->q_tx); i++)
		mt76_dma_tx_cleanup(dev, i, true);

	for (i = 0; i < ARRAY_SIZE(dev->q_rx); i++) {
		netif_napi_del(&dev->napi[i]);
		mt76_dma_rx_cleanup(dev, &dev->q_rx[i]);
	}
=======
	mt76_worker_disable(&dev->tx_worker);
	netif_napi_del(&dev->tx_napi);

	for (i = 0; i < ARRAY_SIZE(dev->phy.q_tx); i++) {
		mt76_dma_tx_cleanup(dev, dev->phy.q_tx[i], true);
		if (dev->phy2)
			mt76_dma_tx_cleanup(dev, dev->phy2->q_tx[i], true);
	}

	for (i = 0; i < ARRAY_SIZE(dev->q_mcu); i++)
		mt76_dma_tx_cleanup(dev, dev->q_mcu[i], true);

	mt76_for_each_q_rx(dev, i) {
		netif_napi_del(&dev->napi[i]);
		mt76_dma_rx_cleanup(dev, &dev->q_rx[i]);
	}

	mt76_free_pending_txwi(dev);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mt76_dma_cleanup);
