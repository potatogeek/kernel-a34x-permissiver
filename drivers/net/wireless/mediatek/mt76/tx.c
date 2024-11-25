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

#include "mt76.h"

<<<<<<< HEAD
static struct mt76_txwi_cache *
mt76_alloc_txwi(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t;
	dma_addr_t addr;
	int size;

	size = (sizeof(*t) + L1_CACHE_BYTES - 1) & ~(L1_CACHE_BYTES - 1);
	t = devm_kzalloc(dev->dev, size, GFP_ATOMIC);
	if (!t)
		return NULL;

	addr = dma_map_single(dev->dev, &t->txwi, sizeof(t->txwi),
			      DMA_TO_DEVICE);
	t->dma_addr = addr;

	return t;
}

static struct mt76_txwi_cache *
__mt76_get_txwi(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t = NULL;

	spin_lock_bh(&dev->lock);
	if (!list_empty(&dev->txwi_cache)) {
		t = list_first_entry(&dev->txwi_cache, struct mt76_txwi_cache,
				     list);
		list_del(&t->list);
	}
	spin_unlock_bh(&dev->lock);

	return t;
}

struct mt76_txwi_cache *
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

	spin_lock_bh(&dev->lock);
	list_add(&t->list, &dev->txwi_cache);
	spin_unlock_bh(&dev->lock);
}

void mt76_tx_free(struct mt76_dev *dev)
{
	struct mt76_txwi_cache *t;

	while ((t = __mt76_get_txwi(dev)) != NULL)
		dma_unmap_single(dev->dev, t->dma_addr, sizeof(t->txwi),
				 DMA_TO_DEVICE);
}

=======
>>>>>>> upstream/android-13
static int
mt76_txq_get_qid(struct ieee80211_txq *txq)
{
	if (!txq->sta)
		return MT_TXQ_BE;

	return txq->ac;
}

void
<<<<<<< HEAD
mt76_tx(struct mt76_dev *dev, struct ieee80211_sta *sta,
	struct mt76_wcid *wcid, struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct mt76_queue *q;
	int qid = skb_get_queue_mapping(skb);
=======
mt76_tx_check_agg_ssn(struct ieee80211_sta *sta, struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	struct ieee80211_txq *txq;
	struct mt76_txq *mtxq;
	u8 tid;

	if (!sta || !ieee80211_is_data_qos(hdr->frame_control) ||
	    !ieee80211_is_data_present(hdr->frame_control))
		return;

	tid = skb->priority & IEEE80211_QOS_CTL_TAG1D_MASK;
	txq = sta->txq[tid];
	mtxq = (struct mt76_txq *)txq->drv_priv;
	if (!mtxq->aggr)
		return;

	mtxq->agg_ssn = le16_to_cpu(hdr->seq_ctrl) + 0x10;
}
EXPORT_SYMBOL_GPL(mt76_tx_check_agg_ssn);

void
mt76_tx_status_lock(struct mt76_dev *dev, struct sk_buff_head *list)
		   __acquires(&dev->status_list.lock)
{
	__skb_queue_head_init(list);
	spin_lock_bh(&dev->status_list.lock);
}
EXPORT_SYMBOL_GPL(mt76_tx_status_lock);

void
mt76_tx_status_unlock(struct mt76_dev *dev, struct sk_buff_head *list)
		      __releases(&dev->status_list.lock)
{
	struct ieee80211_hw *hw;
	struct sk_buff *skb;

	spin_unlock_bh(&dev->status_list.lock);

	rcu_read_lock();
	while ((skb = __skb_dequeue(list)) != NULL) {
		struct ieee80211_tx_status status = {
			.skb = skb,
			.info = IEEE80211_SKB_CB(skb),
		};
		struct mt76_tx_cb *cb = mt76_tx_skb_cb(skb);
		struct mt76_wcid *wcid;

		wcid = rcu_dereference(dev->wcid[cb->wcid]);
		if (wcid)
			status.sta = wcid_to_sta(wcid);

		hw = mt76_tx_status_get_hw(dev, skb);
		ieee80211_tx_status_ext(hw, &status);
	}
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(mt76_tx_status_unlock);

static void
__mt76_tx_status_skb_done(struct mt76_dev *dev, struct sk_buff *skb, u8 flags,
			  struct sk_buff_head *list)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct mt76_tx_cb *cb = mt76_tx_skb_cb(skb);
	u8 done = MT_TX_CB_DMA_DONE | MT_TX_CB_TXS_DONE;

	flags |= cb->flags;
	cb->flags = flags;

	if ((flags & done) != done)
		return;

	__skb_unlink(skb, &dev->status_list);

	/* Tx status can be unreliable. if it fails, mark the frame as ACKed */
	if (flags & MT_TX_CB_TXS_FAILED) {
		info->status.rates[0].count = 0;
		info->status.rates[0].idx = -1;
		info->flags |= IEEE80211_TX_STAT_ACK;
	}

	__skb_queue_tail(list, skb);
}

void
mt76_tx_status_skb_done(struct mt76_dev *dev, struct sk_buff *skb,
			struct sk_buff_head *list)
{
	__mt76_tx_status_skb_done(dev, skb, MT_TX_CB_TXS_DONE, list);
}
EXPORT_SYMBOL_GPL(mt76_tx_status_skb_done);

int
mt76_tx_status_skb_add(struct mt76_dev *dev, struct mt76_wcid *wcid,
		       struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct mt76_tx_cb *cb = mt76_tx_skb_cb(skb);
	int pid;

	if (!wcid)
		return MT_PACKET_ID_NO_ACK;

	if (info->flags & IEEE80211_TX_CTL_NO_ACK)
		return MT_PACKET_ID_NO_ACK;

	if (!(info->flags & (IEEE80211_TX_CTL_REQ_TX_STATUS |
			     IEEE80211_TX_CTL_RATE_CTRL_PROBE)))
		return MT_PACKET_ID_NO_SKB;

	spin_lock_bh(&dev->status_list.lock);

	memset(cb, 0, sizeof(*cb));
	pid = mt76_get_next_pkt_id(wcid);
	cb->wcid = wcid->idx;
	cb->pktid = pid;
	cb->jiffies = jiffies;

	__skb_queue_tail(&dev->status_list, skb);
	spin_unlock_bh(&dev->status_list.lock);

	return pid;
}
EXPORT_SYMBOL_GPL(mt76_tx_status_skb_add);

struct sk_buff *
mt76_tx_status_skb_get(struct mt76_dev *dev, struct mt76_wcid *wcid, int pktid,
		       struct sk_buff_head *list)
{
	struct sk_buff *skb, *tmp;

	skb_queue_walk_safe(&dev->status_list, skb, tmp) {
		struct mt76_tx_cb *cb = mt76_tx_skb_cb(skb);

		if (wcid && cb->wcid != wcid->idx)
			continue;

		if (cb->pktid == pktid)
			return skb;

		if (pktid >= 0 && !time_after(jiffies, cb->jiffies +
					      MT_TX_STATUS_SKB_TIMEOUT))
			continue;

		__mt76_tx_status_skb_done(dev, skb, MT_TX_CB_TXS_FAILED |
						    MT_TX_CB_TXS_DONE, list);
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(mt76_tx_status_skb_get);

void
mt76_tx_status_check(struct mt76_dev *dev, struct mt76_wcid *wcid, bool flush)
{
	struct sk_buff_head list;

	mt76_tx_status_lock(dev, &list);
	mt76_tx_status_skb_get(dev, wcid, flush ? -1 : 0, &list);
	mt76_tx_status_unlock(dev, &list);
}
EXPORT_SYMBOL_GPL(mt76_tx_status_check);

static void
mt76_tx_check_non_aql(struct mt76_dev *dev, struct mt76_wcid *wcid,
		      struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	int pending;

	if (!wcid || info->tx_time_est)
		return;

	pending = atomic_dec_return(&wcid->non_aql_packets);
	if (pending < 0)
		atomic_cmpxchg(&wcid->non_aql_packets, pending, 0);
}

void __mt76_tx_complete_skb(struct mt76_dev *dev, u16 wcid_idx, struct sk_buff *skb,
			    struct list_head *free_list)
{
	struct ieee80211_tx_status status = {
		.skb = skb,
		.free_list = free_list,
	};
	struct mt76_wcid *wcid = NULL;
	struct ieee80211_hw *hw;
	struct sk_buff_head list;

	rcu_read_lock();

	if (wcid_idx < ARRAY_SIZE(dev->wcid))
		wcid = rcu_dereference(dev->wcid[wcid_idx]);

	mt76_tx_check_non_aql(dev, wcid, skb);

#ifdef CONFIG_NL80211_TESTMODE
	if (mt76_is_testmode_skb(dev, skb, &hw)) {
		struct mt76_phy *phy = hw->priv;

		if (skb == phy->test.tx_skb)
			phy->test.tx_done++;
		if (phy->test.tx_queued == phy->test.tx_done)
			wake_up(&dev->tx_wait);

		dev_kfree_skb_any(skb);
		goto out;
	}
#endif

	if (!skb->prev) {
		hw = mt76_tx_status_get_hw(dev, skb);
		status.sta = wcid_to_sta(wcid);
		ieee80211_tx_status_ext(hw, &status);
		goto out;
	}

	mt76_tx_status_lock(dev, &list);
	__mt76_tx_status_skb_done(dev, skb, MT_TX_CB_DMA_DONE, &list);
	mt76_tx_status_unlock(dev, &list);

out:
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(__mt76_tx_complete_skb);

static int
__mt76_tx_queue_skb(struct mt76_phy *phy, int qid, struct sk_buff *skb,
		    struct mt76_wcid *wcid, struct ieee80211_sta *sta,
		    bool *stop)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct mt76_queue *q = phy->q_tx[qid];
	struct mt76_dev *dev = phy->dev;
	bool non_aql;
	int pending;
	int idx;

	non_aql = !info->tx_time_est;
	idx = dev->queue_ops->tx_queue_skb(dev, q, skb, wcid, sta);
	if (idx < 0 || !sta)
		return idx;

	wcid = (struct mt76_wcid *)sta->drv_priv;
	q->entry[idx].wcid = wcid->idx;

	if (!non_aql)
		return idx;

	pending = atomic_inc_return(&wcid->non_aql_packets);
	if (stop && pending >= MT_MAX_NON_AQL_PKT)
		*stop = true;

	return idx;
}

void
mt76_tx(struct mt76_phy *phy, struct ieee80211_sta *sta,
	struct mt76_wcid *wcid, struct sk_buff *skb)
{
	struct mt76_dev *dev = phy->dev;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	struct mt76_queue *q;
	int qid = skb_get_queue_mapping(skb);
	bool ext_phy = phy != &dev->phy;

	if (mt76_testmode_enabled(phy)) {
		ieee80211_free_txskb(phy->hw, skb);
		return;
	}
>>>>>>> upstream/android-13

	if (WARN_ON(qid >= MT_TXQ_PSD)) {
		qid = MT_TXQ_BE;
		skb_set_queue_mapping(skb, qid);
	}

<<<<<<< HEAD
	if (!wcid->tx_rate_set)
		ieee80211_get_tx_rates(info->control.vif, sta, skb,
				       info->control.rates, 1);

	q = &dev->q_tx[qid];

	spin_lock_bh(&q->lock);
	dev->queue_ops->tx_queue_skb(dev, q, skb, wcid, sta);
	dev->queue_ops->kick(dev, q);

	if (q->queued > q->ndesc - 8)
		ieee80211_stop_queue(dev->hw, skb_get_queue_mapping(skb));
=======
	if ((dev->drv->drv_flags & MT_DRV_HW_MGMT_TXQ) &&
	    !(info->flags & IEEE80211_TX_CTL_HW_80211_ENCAP) &&
	    !ieee80211_is_data(hdr->frame_control) &&
	    !ieee80211_is_bufferable_mmpdu(hdr->frame_control)) {
		qid = MT_TXQ_PSD;
		skb_set_queue_mapping(skb, qid);
	}

	if (wcid && !(wcid->tx_info & MT_WCID_TX_INFO_SET))
		ieee80211_get_tx_rates(info->control.vif, sta, skb,
				       info->control.rates, 1);

	if (ext_phy)
		info->hw_queue |= MT_TX_HW_QUEUE_EXT_PHY;

	q = phy->q_tx[qid];

	spin_lock_bh(&q->lock);
	__mt76_tx_queue_skb(phy, qid, skb, wcid, sta, NULL);
	dev->queue_ops->kick(dev, q);
>>>>>>> upstream/android-13
	spin_unlock_bh(&q->lock);
}
EXPORT_SYMBOL_GPL(mt76_tx);

static struct sk_buff *
<<<<<<< HEAD
mt76_txq_dequeue(struct mt76_dev *dev, struct mt76_txq *mtxq, bool ps)
{
	struct ieee80211_txq *txq = mtxq_to_txq(mtxq);
	struct sk_buff *skb;

	skb = skb_dequeue(&mtxq->retry_q);
	if (skb) {
		u8 tid = skb->priority & IEEE80211_QOS_CTL_TID_MASK;

		if (ps && skb_queue_empty(&mtxq->retry_q))
			ieee80211_sta_set_buffered(txq->sta, tid, false);

		return skb;
	}

	skb = ieee80211_tx_dequeue(dev->hw, txq);
	if (!skb)
		return NULL;

=======
mt76_txq_dequeue(struct mt76_phy *phy, struct mt76_txq *mtxq)
{
	struct ieee80211_txq *txq = mtxq_to_txq(mtxq);
	struct ieee80211_tx_info *info;
	bool ext_phy = phy != &phy->dev->phy;
	struct sk_buff *skb;

	skb = ieee80211_tx_dequeue(phy->hw, txq);
	if (!skb)
		return NULL;

	info = IEEE80211_SKB_CB(skb);
	if (ext_phy)
		info->hw_queue |= MT_TX_HW_QUEUE_EXT_PHY;

>>>>>>> upstream/android-13
	return skb;
}

static void
<<<<<<< HEAD
mt76_check_agg_ssn(struct mt76_txq *mtxq, struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;

	if (!ieee80211_is_data_qos(hdr->frame_control) ||
	    !ieee80211_is_data_present(hdr->frame_control))
		return;

	mtxq->agg_ssn = le16_to_cpu(hdr->seq_ctrl) + 0x10;
}

static void
mt76_queue_ps_skb(struct mt76_dev *dev, struct ieee80211_sta *sta,
		  struct sk_buff *skb, bool last)
{
	struct mt76_wcid *wcid = (struct mt76_wcid *) sta->drv_priv;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct mt76_queue *hwq = &dev->q_tx[MT_TXQ_PSD];

	info->control.flags |= IEEE80211_TX_CTRL_PS_RESPONSE;
	if (last)
		info->flags |= IEEE80211_TX_STATUS_EOSP;

	mt76_skb_set_moredata(skb, !last);
	dev->queue_ops->tx_queue_skb(dev, hwq, skb, wcid, sta);
=======
mt76_queue_ps_skb(struct mt76_phy *phy, struct ieee80211_sta *sta,
		  struct sk_buff *skb, bool last)
{
	struct mt76_wcid *wcid = (struct mt76_wcid *)sta->drv_priv;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);

	info->control.flags |= IEEE80211_TX_CTRL_PS_RESPONSE;
	if (last)
		info->flags |= IEEE80211_TX_STATUS_EOSP |
			       IEEE80211_TX_CTL_REQ_TX_STATUS;

	mt76_skb_set_moredata(skb, !last);
	__mt76_tx_queue_skb(phy, MT_TXQ_PSD, skb, wcid, sta, NULL);
>>>>>>> upstream/android-13
}

void
mt76_release_buffered_frames(struct ieee80211_hw *hw, struct ieee80211_sta *sta,
			     u16 tids, int nframes,
			     enum ieee80211_frame_release_type reason,
			     bool more_data)
{
<<<<<<< HEAD
	struct mt76_dev *dev = hw->priv;
	struct sk_buff *last_skb = NULL;
	struct mt76_queue *hwq = &dev->q_tx[MT_TXQ_PSD];
=======
	struct mt76_phy *phy = hw->priv;
	struct mt76_dev *dev = phy->dev;
	struct sk_buff *last_skb = NULL;
	struct mt76_queue *hwq = phy->q_tx[MT_TXQ_PSD];
>>>>>>> upstream/android-13
	int i;

	spin_lock_bh(&hwq->lock);
	for (i = 0; tids && nframes; i++, tids >>= 1) {
		struct ieee80211_txq *txq = sta->txq[i];
<<<<<<< HEAD
		struct mt76_txq *mtxq = (struct mt76_txq *) txq->drv_priv;
=======
		struct mt76_txq *mtxq = (struct mt76_txq *)txq->drv_priv;
>>>>>>> upstream/android-13
		struct sk_buff *skb;

		if (!(tids & 1))
			continue;

		do {
<<<<<<< HEAD
			skb = mt76_txq_dequeue(dev, mtxq, true);
			if (!skb)
				break;

			if (mtxq->aggr)
				mt76_check_agg_ssn(mtxq, skb);

			nframes--;
			if (last_skb)
				mt76_queue_ps_skb(dev, sta, last_skb, false);
=======
			skb = mt76_txq_dequeue(phy, mtxq);
			if (!skb)
				break;

			nframes--;
			if (last_skb)
				mt76_queue_ps_skb(phy, sta, last_skb, false);
>>>>>>> upstream/android-13

			last_skb = skb;
		} while (nframes);
	}

	if (last_skb) {
<<<<<<< HEAD
		mt76_queue_ps_skb(dev, sta, last_skb, true);
		dev->queue_ops->kick(dev, hwq);
	}
=======
		mt76_queue_ps_skb(phy, sta, last_skb, true);
		dev->queue_ops->kick(dev, hwq);
	} else {
		ieee80211_sta_eosp(sta);
	}

>>>>>>> upstream/android-13
	spin_unlock_bh(&hwq->lock);
}
EXPORT_SYMBOL_GPL(mt76_release_buffered_frames);

<<<<<<< HEAD
static int
mt76_txq_send_burst(struct mt76_dev *dev, struct mt76_queue *hwq,
		    struct mt76_txq *mtxq, bool *empty)
{
	struct ieee80211_txq *txq = mtxq_to_txq(mtxq);
	struct ieee80211_tx_info *info;
	struct mt76_wcid *wcid = mtxq->wcid;
	struct sk_buff *skb;
	int n_frames = 1, limit;
	struct ieee80211_tx_rate tx_rate;
	bool ampdu;
	bool probe;
	int idx;

	skb = mt76_txq_dequeue(dev, mtxq, false);
	if (!skb) {
		*empty = true;
		return 0;
	}

	info = IEEE80211_SKB_CB(skb);
	if (!wcid->tx_rate_set)
		ieee80211_get_tx_rates(txq->vif, txq->sta, skb,
				       info->control.rates, 1);
	tx_rate = info->control.rates[0];

	probe = (info->flags & IEEE80211_TX_CTL_RATE_CTRL_PROBE);
	ampdu = IEEE80211_SKB_CB(skb)->flags & IEEE80211_TX_CTL_AMPDU;
	limit = ampdu ? 16 : 3;

	if (ampdu)
		mt76_check_agg_ssn(mtxq, skb);

	idx = dev->queue_ops->tx_queue_skb(dev, hwq, skb, wcid, txq->sta);

=======
static bool
mt76_txq_stopped(struct mt76_queue *q)
{
	return q->stopped || q->blocked ||
	       q->queued + MT_TXQ_FREE_THR >= q->ndesc;
}

static int
mt76_txq_send_burst(struct mt76_phy *phy, struct mt76_queue *q,
		    struct mt76_txq *mtxq)
{
	struct mt76_dev *dev = phy->dev;
	struct ieee80211_txq *txq = mtxq_to_txq(mtxq);
	enum mt76_txq_id qid = mt76_txq_get_qid(txq);
	struct mt76_wcid *wcid = mtxq->wcid;
	struct ieee80211_tx_info *info;
	struct sk_buff *skb;
	int n_frames = 1;
	bool stop = false;
	int idx;

	if (test_bit(MT_WCID_FLAG_PS, &wcid->flags))
		return 0;

	if (atomic_read(&wcid->non_aql_packets) >= MT_MAX_NON_AQL_PKT)
		return 0;

	skb = mt76_txq_dequeue(phy, mtxq);
	if (!skb)
		return 0;

	info = IEEE80211_SKB_CB(skb);
	if (!(wcid->tx_info & MT_WCID_TX_INFO_SET))
		ieee80211_get_tx_rates(txq->vif, txq->sta, skb,
				       info->control.rates, 1);

	idx = __mt76_tx_queue_skb(phy, qid, skb, wcid, txq->sta, &stop);
>>>>>>> upstream/android-13
	if (idx < 0)
		return idx;

	do {
<<<<<<< HEAD
		bool cur_ampdu;

		if (probe)
			break;

		if (test_bit(MT76_OFFCHANNEL, &dev->state) ||
		    test_bit(MT76_RESET, &dev->state))
			return -EBUSY;

		skb = mt76_txq_dequeue(dev, mtxq, false);
		if (!skb) {
			*empty = true;
			break;
		}

		info = IEEE80211_SKB_CB(skb);
		cur_ampdu = info->flags & IEEE80211_TX_CTL_AMPDU;

		if (ampdu != cur_ampdu ||
		    (info->flags & IEEE80211_TX_CTL_RATE_CTRL_PROBE)) {
			skb_queue_tail(&mtxq->retry_q, skb);
			break;
		}

		info->control.rates[0] = tx_rate;

		if (cur_ampdu)
			mt76_check_agg_ssn(mtxq, skb);

		idx = dev->queue_ops->tx_queue_skb(dev, hwq, skb, wcid,
						   txq->sta);
		if (idx < 0)
			return idx;

		n_frames++;
	} while (n_frames < limit);

	if (!probe) {
		hwq->swq_queued++;
		hwq->entry[idx].schedule = true;
	}

	dev->queue_ops->kick(dev, hwq);
=======
		if (test_bit(MT76_RESET, &phy->state))
			return -EBUSY;

		if (stop || mt76_txq_stopped(q))
			break;

		skb = mt76_txq_dequeue(phy, mtxq);
		if (!skb)
			break;

		info = IEEE80211_SKB_CB(skb);
		if (!(wcid->tx_info & MT_WCID_TX_INFO_SET))
			ieee80211_get_tx_rates(txq->vif, txq->sta, skb,
					       info->control.rates, 1);

		idx = __mt76_tx_queue_skb(phy, qid, skb, wcid, txq->sta, &stop);
		if (idx < 0)
			break;

		n_frames++;
	} while (1);

	dev->queue_ops->kick(dev, q);
>>>>>>> upstream/android-13

	return n_frames;
}

static int
<<<<<<< HEAD
mt76_txq_schedule_list(struct mt76_dev *dev, struct mt76_queue *hwq)
{
	struct mt76_txq *mtxq, *mtxq_last;
	int len = 0;

restart:
	mtxq_last = list_last_entry(&hwq->swq, struct mt76_txq, list);
	while (!list_empty(&hwq->swq)) {
		bool empty = false;
		int cur;

		if (test_bit(MT76_OFFCHANNEL, &dev->state) ||
		    test_bit(MT76_RESET, &dev->state))
			return -EBUSY;

		mtxq = list_first_entry(&hwq->swq, struct mt76_txq, list);
=======
mt76_txq_schedule_list(struct mt76_phy *phy, enum mt76_txq_id qid)
{
	struct mt76_queue *q = phy->q_tx[qid];
	struct mt76_dev *dev = phy->dev;
	struct ieee80211_txq *txq;
	struct mt76_txq *mtxq;
	struct mt76_wcid *wcid;
	int ret = 0;

	while (1) {
		int n_frames = 0;

		if (test_bit(MT76_RESET, &phy->state))
			return -EBUSY;

		if (dev->queue_ops->tx_cleanup &&
		    q->queued + 2 * MT_TXQ_FREE_THR >= q->ndesc) {
			dev->queue_ops->tx_cleanup(dev, q, false);
		}

		txq = ieee80211_next_txq(phy->hw, qid);
		if (!txq)
			break;

		mtxq = (struct mt76_txq *)txq->drv_priv;
		wcid = mtxq->wcid;
		if (wcid && test_bit(MT_WCID_FLAG_PS, &wcid->flags))
			continue;

		spin_lock_bh(&q->lock);

>>>>>>> upstream/android-13
		if (mtxq->send_bar && mtxq->aggr) {
			struct ieee80211_txq *txq = mtxq_to_txq(mtxq);
			struct ieee80211_sta *sta = txq->sta;
			struct ieee80211_vif *vif = txq->vif;
			u16 agg_ssn = mtxq->agg_ssn;
			u8 tid = txq->tid;

			mtxq->send_bar = false;
<<<<<<< HEAD
			spin_unlock_bh(&hwq->lock);
			ieee80211_send_bar(vif, sta->addr, tid, agg_ssn);
			spin_lock_bh(&hwq->lock);
			goto restart;
		}

		list_del_init(&mtxq->list);

		cur = mt76_txq_send_burst(dev, hwq, mtxq, &empty);
		if (!empty)
			list_add_tail(&mtxq->list, &hwq->swq);

		if (cur < 0)
			return cur;

		len += cur;

		if (mtxq == mtxq_last)
			break;
	}

	return len;
}

void mt76_txq_schedule(struct mt76_dev *dev, struct mt76_queue *hwq)
{
	int len;

	rcu_read_lock();
	do {
		if (hwq->swq_queued >= 4 || list_empty(&hwq->swq))
			break;

		len = mt76_txq_schedule_list(dev, hwq);
	} while (len > 0);
=======
			spin_unlock_bh(&q->lock);
			ieee80211_send_bar(vif, sta->addr, tid, agg_ssn);
			spin_lock_bh(&q->lock);
		}

		if (!mt76_txq_stopped(q))
			n_frames = mt76_txq_send_burst(phy, q, mtxq);

		spin_unlock_bh(&q->lock);

		ieee80211_return_txq(phy->hw, txq, false);

		if (unlikely(n_frames < 0))
			return n_frames;

		ret += n_frames;
	}

	return ret;
}

void mt76_txq_schedule(struct mt76_phy *phy, enum mt76_txq_id qid)
{
	int len;

	if (qid >= 4)
		return;

	rcu_read_lock();

	do {
		ieee80211_txq_schedule_start(phy->hw, qid);
		len = mt76_txq_schedule_list(phy, qid);
		ieee80211_txq_schedule_end(phy->hw, qid);
	} while (len > 0);

>>>>>>> upstream/android-13
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(mt76_txq_schedule);

<<<<<<< HEAD
void mt76_txq_schedule_all(struct mt76_dev *dev)
{
	int i;

	for (i = 0; i <= MT_TXQ_BK; i++) {
		struct mt76_queue *q = &dev->q_tx[i];

		spin_lock_bh(&q->lock);
		mt76_txq_schedule(dev, q);
		spin_unlock_bh(&q->lock);
	}
}
EXPORT_SYMBOL_GPL(mt76_txq_schedule_all);

void mt76_stop_tx_queues(struct mt76_dev *dev, struct ieee80211_sta *sta,
=======
void mt76_txq_schedule_all(struct mt76_phy *phy)
{
	int i;

	for (i = 0; i <= MT_TXQ_BK; i++)
		mt76_txq_schedule(phy, i);
}
EXPORT_SYMBOL_GPL(mt76_txq_schedule_all);

void mt76_tx_worker_run(struct mt76_dev *dev)
{
	mt76_txq_schedule_all(&dev->phy);
	if (dev->phy2)
		mt76_txq_schedule_all(dev->phy2);

#ifdef CONFIG_NL80211_TESTMODE
	if (dev->phy.test.tx_pending)
		mt76_testmode_tx_pending(&dev->phy);
	if (dev->phy2 && dev->phy2->test.tx_pending)
		mt76_testmode_tx_pending(dev->phy2);
#endif
}
EXPORT_SYMBOL_GPL(mt76_tx_worker_run);

void mt76_tx_worker(struct mt76_worker *w)
{
	struct mt76_dev *dev = container_of(w, struct mt76_dev, tx_worker);

	mt76_tx_worker_run(dev);
}

void mt76_stop_tx_queues(struct mt76_phy *phy, struct ieee80211_sta *sta,
>>>>>>> upstream/android-13
			 bool send_bar)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(sta->txq); i++) {
		struct ieee80211_txq *txq = sta->txq[i];
<<<<<<< HEAD
=======
		struct mt76_queue *hwq;
>>>>>>> upstream/android-13
		struct mt76_txq *mtxq;

		if (!txq)
			continue;

<<<<<<< HEAD
		mtxq = (struct mt76_txq *)txq->drv_priv;

		spin_lock_bh(&mtxq->hwq->lock);
		mtxq->send_bar = mtxq->aggr && send_bar;
		if (!list_empty(&mtxq->list))
			list_del_init(&mtxq->list);
		spin_unlock_bh(&mtxq->hwq->lock);
=======
		hwq = phy->q_tx[mt76_txq_get_qid(txq)];
		mtxq = (struct mt76_txq *)txq->drv_priv;

		spin_lock_bh(&hwq->lock);
		mtxq->send_bar = mtxq->aggr && send_bar;
		spin_unlock_bh(&hwq->lock);
>>>>>>> upstream/android-13
	}
}
EXPORT_SYMBOL_GPL(mt76_stop_tx_queues);

void mt76_wake_tx_queue(struct ieee80211_hw *hw, struct ieee80211_txq *txq)
{
<<<<<<< HEAD
	struct mt76_dev *dev = hw->priv;
	struct mt76_txq *mtxq = (struct mt76_txq *) txq->drv_priv;
	struct mt76_queue *hwq = mtxq->hwq;

	spin_lock_bh(&hwq->lock);
	if (list_empty(&mtxq->list))
		list_add_tail(&mtxq->list, &hwq->swq);
	mt76_txq_schedule(dev, hwq);
	spin_unlock_bh(&hwq->lock);
}
EXPORT_SYMBOL_GPL(mt76_wake_tx_queue);

void mt76_txq_remove(struct mt76_dev *dev, struct ieee80211_txq *txq)
{
	struct mt76_txq *mtxq;
	struct mt76_queue *hwq;
	struct sk_buff *skb;

	if (!txq)
		return;

	mtxq = (struct mt76_txq *) txq->drv_priv;
	hwq = mtxq->hwq;

	spin_lock_bh(&hwq->lock);
	if (!list_empty(&mtxq->list))
		list_del(&mtxq->list);
	spin_unlock_bh(&hwq->lock);

	while ((skb = skb_dequeue(&mtxq->retry_q)) != NULL)
		ieee80211_free_txskb(dev->hw, skb);
}
EXPORT_SYMBOL_GPL(mt76_txq_remove);

void mt76_txq_init(struct mt76_dev *dev, struct ieee80211_txq *txq)
{
	struct mt76_txq *mtxq = (struct mt76_txq *) txq->drv_priv;

	INIT_LIST_HEAD(&mtxq->list);
	skb_queue_head_init(&mtxq->retry_q);

	mtxq->hwq = &dev->q_tx[mt76_txq_get_qid(txq)];
}
EXPORT_SYMBOL_GPL(mt76_txq_init);
=======
	struct mt76_phy *phy = hw->priv;
	struct mt76_dev *dev = phy->dev;

	if (!test_bit(MT76_STATE_RUNNING, &phy->state))
		return;

	mt76_worker_schedule(&dev->tx_worker);
}
EXPORT_SYMBOL_GPL(mt76_wake_tx_queue);

u8 mt76_ac_to_hwq(u8 ac)
{
	static const u8 wmm_queue_map[] = {
		[IEEE80211_AC_BE] = 0,
		[IEEE80211_AC_BK] = 1,
		[IEEE80211_AC_VI] = 2,
		[IEEE80211_AC_VO] = 3,
	};

	if (WARN_ON(ac >= IEEE80211_NUM_ACS))
		return 0;

	return wmm_queue_map[ac];
}
EXPORT_SYMBOL_GPL(mt76_ac_to_hwq);

int mt76_skb_adjust_pad(struct sk_buff *skb, int pad)
{
	struct sk_buff *iter, *last = skb;

	/* First packet of a A-MSDU burst keeps track of the whole burst
	 * length, need to update length of it and the last packet.
	 */
	skb_walk_frags(skb, iter) {
		last = iter;
		if (!iter->next) {
			skb->data_len += pad;
			skb->len += pad;
			break;
		}
	}

	if (skb_pad(last, pad))
		return -ENOMEM;

	__skb_put(last, pad);

	return 0;
}
EXPORT_SYMBOL_GPL(mt76_skb_adjust_pad);

void mt76_queue_tx_complete(struct mt76_dev *dev, struct mt76_queue *q,
			    struct mt76_queue_entry *e)
{
	if (e->skb)
		dev->drv->tx_complete_skb(dev, e);

	spin_lock_bh(&q->lock);
	q->tail = (q->tail + 1) % q->ndesc;
	q->queued--;
	spin_unlock_bh(&q->lock);
}
EXPORT_SYMBOL_GPL(mt76_queue_tx_complete);

void __mt76_set_tx_blocked(struct mt76_dev *dev, bool blocked)
{
	struct mt76_phy *phy = &dev->phy, *phy2 = dev->phy2;
	struct mt76_queue *q, *q2 = NULL;

	q = phy->q_tx[0];
	if (blocked == q->blocked)
		return;

	q->blocked = blocked;
	if (phy2) {
		q2 = phy2->q_tx[0];
		q2->blocked = blocked;
	}

	if (!blocked)
		mt76_worker_schedule(&dev->tx_worker);
}
EXPORT_SYMBOL_GPL(__mt76_set_tx_blocked);

int mt76_token_consume(struct mt76_dev *dev, struct mt76_txwi_cache **ptxwi)
{
	int token;

	spin_lock_bh(&dev->token_lock);

	token = idr_alloc(&dev->token, *ptxwi, 0, dev->drv->token_size,
			  GFP_ATOMIC);
	if (token >= 0)
		dev->token_count++;

	if (dev->token_count >= dev->drv->token_size - MT76_TOKEN_FREE_THR)
		__mt76_set_tx_blocked(dev, true);

	spin_unlock_bh(&dev->token_lock);

	return token;
}
EXPORT_SYMBOL_GPL(mt76_token_consume);

struct mt76_txwi_cache *
mt76_token_release(struct mt76_dev *dev, int token, bool *wake)
{
	struct mt76_txwi_cache *txwi;

	spin_lock_bh(&dev->token_lock);

	txwi = idr_remove(&dev->token, token);
	if (txwi)
		dev->token_count--;

	if (dev->token_count < dev->drv->token_size - MT76_TOKEN_FREE_THR &&
	    dev->phy.q_tx[0]->blocked)
		*wake = true;

	spin_unlock_bh(&dev->token_lock);

	return txwi;
}
EXPORT_SYMBOL_GPL(mt76_token_release);
>>>>>>> upstream/android-13
