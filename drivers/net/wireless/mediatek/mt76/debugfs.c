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

static int
mt76_reg_set(void *data, u64 val)
{
	struct mt76_dev *dev = data;

<<<<<<< HEAD
	dev->bus->wr(dev, dev->debugfs_reg, val);
=======
	__mt76_wr(dev, dev->debugfs_reg, val);
>>>>>>> upstream/android-13
	return 0;
}

static int
mt76_reg_get(void *data, u64 *val)
{
	struct mt76_dev *dev = data;

<<<<<<< HEAD
	*val = dev->bus->rr(dev, dev->debugfs_reg);
=======
	*val = __mt76_rr(dev, dev->debugfs_reg);
>>>>>>> upstream/android-13
	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(fops_regval, mt76_reg_get, mt76_reg_set,
			 "0x%08llx\n");

static int
<<<<<<< HEAD
mt76_queues_read(struct seq_file *s, void *data)
=======
mt76_napi_threaded_set(void *data, u64 val)
{
	struct mt76_dev *dev = data;

	if (!mt76_is_mmio(dev))
		return -EOPNOTSUPP;

	if (dev->napi_dev.threaded != val)
		return dev_set_threaded(&dev->napi_dev, val);

	return 0;
}

static int
mt76_napi_threaded_get(void *data, u64 *val)
{
	struct mt76_dev *dev = data;

	*val = dev->napi_dev.threaded;
	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(fops_napi_threaded, mt76_napi_threaded_get,
			 mt76_napi_threaded_set, "%llu\n");

int mt76_queues_read(struct seq_file *s, void *data)
>>>>>>> upstream/android-13
{
	struct mt76_dev *dev = dev_get_drvdata(s->private);
	int i;

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(dev->q_tx); i++) {
		struct mt76_queue *q = &dev->q_tx[i];

		if (!q->ndesc)
			continue;

		seq_printf(s,
			   "%d:	queued=%d head=%d tail=%d swq_queued=%d\n",
			   i, q->queued, q->head, q->tail, q->swq_queued);
=======
	for (i = 0; i < ARRAY_SIZE(dev->phy.q_tx); i++) {
		struct mt76_queue *q = dev->phy.q_tx[i];

		if (!q)
			continue;

		seq_printf(s,
			   "%d:	queued=%d head=%d tail=%d\n",
			   i, q->queued, q->head, q->tail);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(mt76_queues_read);

static int mt76_rx_queues_read(struct seq_file *s, void *data)
{
	struct mt76_dev *dev = dev_get_drvdata(s->private);
	int i, queued;

	mt76_for_each_q_rx(dev, i) {
		struct mt76_queue *q = &dev->q_rx[i];

		queued = mt76_is_usb(dev) ? q->ndesc - q->queued : q->queued;
		seq_printf(s, "%d:	queued=%d head=%d tail=%d\n",
			   i, queued, q->head, q->tail);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
struct dentry *mt76_register_debugfs(struct mt76_dev *dev)
{
=======
void mt76_seq_puts_array(struct seq_file *file, const char *str,
			 s8 *val, int len)
{
	int i;

	seq_printf(file, "%10s:", str);
	for (i = 0; i < len; i++)
		seq_printf(file, " %2d", val[i]);
	seq_puts(file, "\n");
}
EXPORT_SYMBOL_GPL(mt76_seq_puts_array);

static int mt76_read_rate_txpower(struct seq_file *s, void *data)
{
	struct mt76_dev *dev = dev_get_drvdata(s->private);

	mt76_seq_puts_array(s, "CCK", dev->rate_power.cck,
			    ARRAY_SIZE(dev->rate_power.cck));
	mt76_seq_puts_array(s, "OFDM", dev->rate_power.ofdm,
			    ARRAY_SIZE(dev->rate_power.ofdm));
	mt76_seq_puts_array(s, "STBC", dev->rate_power.stbc,
			    ARRAY_SIZE(dev->rate_power.stbc));
	mt76_seq_puts_array(s, "HT", dev->rate_power.ht,
			    ARRAY_SIZE(dev->rate_power.ht));
	mt76_seq_puts_array(s, "VHT", dev->rate_power.vht,
			    ARRAY_SIZE(dev->rate_power.vht));
	return 0;
}

struct dentry *
mt76_register_debugfs_fops(struct mt76_dev *dev,
			   const struct file_operations *ops)
{
	const struct file_operations *fops = ops ? ops : &fops_regval;
>>>>>>> upstream/android-13
	struct dentry *dir;

	dir = debugfs_create_dir("mt76", dev->hw->wiphy->debugfsdir);
	if (!dir)
		return NULL;

	debugfs_create_u8("led_pin", 0600, dir, &dev->led_pin);
	debugfs_create_u32("regidx", 0600, dir, &dev->debugfs_reg);
<<<<<<< HEAD
	debugfs_create_file_unsafe("regval", 0600, dir, dev,
				   &fops_regval);
	debugfs_create_blob("eeprom", 0400, dir, &dev->eeprom);
	if (dev->otp.data)
		debugfs_create_blob("otp", 0400, dir, &dev->otp);
	debugfs_create_devm_seqfile(dev->dev, "queues", dir, mt76_queues_read);

	return dir;
}
EXPORT_SYMBOL_GPL(mt76_register_debugfs);
=======
	debugfs_create_file_unsafe("regval", 0600, dir, dev, fops);
	debugfs_create_file_unsafe("napi_threaded", 0600, dir, dev,
				   &fops_napi_threaded);
	debugfs_create_blob("eeprom", 0400, dir, &dev->eeprom);
	if (dev->otp.data)
		debugfs_create_blob("otp", 0400, dir, &dev->otp);
	debugfs_create_devm_seqfile(dev->dev, "rate_txpower", dir,
				    mt76_read_rate_txpower);
	debugfs_create_devm_seqfile(dev->dev, "rx-queues", dir,
				    mt76_rx_queues_read);

	return dir;
}
EXPORT_SYMBOL_GPL(mt76_register_debugfs_fops);
>>>>>>> upstream/android-13
