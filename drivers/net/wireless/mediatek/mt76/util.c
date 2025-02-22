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

#include <linux/module.h>
#include "mt76.h"

bool __mt76_poll(struct mt76_dev *dev, u32 offset, u32 mask, u32 val,
		 int timeout)
{
	u32 cur;

	timeout /= 10;
	do {
<<<<<<< HEAD
		cur = dev->bus->rr(dev, offset) & mask;
=======
		cur = __mt76_rr(dev, offset) & mask;
>>>>>>> upstream/android-13
		if (cur == val)
			return true;

		udelay(10);
	} while (timeout-- > 0);

	return false;
}
EXPORT_SYMBOL_GPL(__mt76_poll);

bool __mt76_poll_msec(struct mt76_dev *dev, u32 offset, u32 mask, u32 val,
		      int timeout)
{
	u32 cur;

	timeout /= 10;
	do {
<<<<<<< HEAD
		cur = dev->bus->rr(dev, offset) & mask;
=======
		cur = __mt76_rr(dev, offset) & mask;
>>>>>>> upstream/android-13
		if (cur == val)
			return true;

		usleep_range(10000, 20000);
	} while (timeout-- > 0);

	return false;
}
EXPORT_SYMBOL_GPL(__mt76_poll_msec);

<<<<<<< HEAD
int mt76_wcid_alloc(unsigned long *mask, int size)
{
	int i, idx = 0, cur;

	for (i = 0; i < size / BITS_PER_LONG; i++) {
=======
int mt76_wcid_alloc(u32 *mask, int size)
{
	int i, idx = 0, cur;

	for (i = 0; i < DIV_ROUND_UP(size, 32); i++) {
>>>>>>> upstream/android-13
		idx = ffs(~mask[i]);
		if (!idx)
			continue;

		idx--;
<<<<<<< HEAD
		cur = i * BITS_PER_LONG + idx;
=======
		cur = i * 32 + idx;
>>>>>>> upstream/android-13
		if (cur >= size)
			break;

		mask[i] |= BIT(idx);
		return cur;
	}

	return -1;
}
EXPORT_SYMBOL_GPL(mt76_wcid_alloc);

<<<<<<< HEAD
=======
int mt76_get_min_avg_rssi(struct mt76_dev *dev, bool ext_phy)
{
	struct mt76_wcid *wcid;
	int i, j, min_rssi = 0;
	s8 cur_rssi;

	local_bh_disable();
	rcu_read_lock();

	for (i = 0; i < ARRAY_SIZE(dev->wcid_mask); i++) {
		u32 mask = dev->wcid_mask[i];
		u32 phy_mask = dev->wcid_phy_mask[i];

		if (!mask)
			continue;

		for (j = i * 32; mask; j++, mask >>= 1, phy_mask >>= 1) {
			if (!(mask & 1))
				continue;

			if (!!(phy_mask & 1) != ext_phy)
				continue;

			wcid = rcu_dereference(dev->wcid[j]);
			if (!wcid)
				continue;

			spin_lock(&dev->rx_lock);
			if (wcid->inactive_count++ < 5)
				cur_rssi = -ewma_signal_read(&wcid->rssi);
			else
				cur_rssi = 0;
			spin_unlock(&dev->rx_lock);

			if (cur_rssi < min_rssi)
				min_rssi = cur_rssi;
		}
	}

	rcu_read_unlock();
	local_bh_enable();

	return min_rssi;
}
EXPORT_SYMBOL_GPL(mt76_get_min_avg_rssi);

int __mt76_worker_fn(void *ptr)
{
	struct mt76_worker *w = ptr;

	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);

		if (kthread_should_park()) {
			kthread_parkme();
			continue;
		}

		if (!test_and_clear_bit(MT76_WORKER_SCHEDULED, &w->state)) {
			schedule();
			continue;
		}

		set_bit(MT76_WORKER_RUNNING, &w->state);
		set_current_state(TASK_RUNNING);
		w->fn(w);
		cond_resched();
		clear_bit(MT76_WORKER_RUNNING, &w->state);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(__mt76_worker_fn);

>>>>>>> upstream/android-13
MODULE_LICENSE("Dual BSD/GPL");
