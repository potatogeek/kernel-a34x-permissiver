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
#include "trace.h"

static u32 mt76_mmio_rr(struct mt76_dev *dev, u32 offset)
{
	u32 val;

<<<<<<< HEAD
	val = ioread32(dev->regs + offset);
=======
	val = readl(dev->mmio.regs + offset);
>>>>>>> upstream/android-13
	trace_reg_rr(dev, offset, val);

	return val;
}

static void mt76_mmio_wr(struct mt76_dev *dev, u32 offset, u32 val)
{
	trace_reg_wr(dev, offset, val);
<<<<<<< HEAD
	iowrite32(val, dev->regs + offset);
=======
	writel(val, dev->mmio.regs + offset);
>>>>>>> upstream/android-13
}

static u32 mt76_mmio_rmw(struct mt76_dev *dev, u32 offset, u32 mask, u32 val)
{
	val |= mt76_mmio_rr(dev, offset) & ~mask;
	mt76_mmio_wr(dev, offset, val);
	return val;
}

<<<<<<< HEAD
static void mt76_mmio_copy(struct mt76_dev *dev, u32 offset, const void *data,
			   int len)
{
	__iowrite32_copy(dev->regs + offset, data, len >> 2);
}

=======
static void mt76_mmio_write_copy(struct mt76_dev *dev, u32 offset,
				 const void *data, int len)
{
	__iowrite32_copy(dev->mmio.regs + offset, data, DIV_ROUND_UP(len, 4));
}

static void mt76_mmio_read_copy(struct mt76_dev *dev, u32 offset,
				void *data, int len)
{
	__ioread32_copy(data, dev->mmio.regs + offset, DIV_ROUND_UP(len, 4));
}

static int mt76_mmio_wr_rp(struct mt76_dev *dev, u32 base,
			   const struct mt76_reg_pair *data, int len)
{
	while (len > 0) {
		mt76_mmio_wr(dev, data->reg, data->value);
		data++;
		len--;
	}

	return 0;
}

static int mt76_mmio_rd_rp(struct mt76_dev *dev, u32 base,
			   struct mt76_reg_pair *data, int len)
{
	while (len > 0) {
		data->value = mt76_mmio_rr(dev, data->reg);
		data++;
		len--;
	}

	return 0;
}

void mt76_set_irq_mask(struct mt76_dev *dev, u32 addr,
		       u32 clear, u32 set)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->mmio.irq_lock, flags);
	dev->mmio.irqmask &= ~clear;
	dev->mmio.irqmask |= set;
	if (addr)
		mt76_mmio_wr(dev, addr, dev->mmio.irqmask);
	spin_unlock_irqrestore(&dev->mmio.irq_lock, flags);
}
EXPORT_SYMBOL_GPL(mt76_set_irq_mask);

>>>>>>> upstream/android-13
void mt76_mmio_init(struct mt76_dev *dev, void __iomem *regs)
{
	static const struct mt76_bus_ops mt76_mmio_ops = {
		.rr = mt76_mmio_rr,
		.rmw = mt76_mmio_rmw,
		.wr = mt76_mmio_wr,
<<<<<<< HEAD
		.copy = mt76_mmio_copy,
	};

	dev->bus = &mt76_mmio_ops;
	dev->regs = regs;
=======
		.write_copy = mt76_mmio_write_copy,
		.read_copy = mt76_mmio_read_copy,
		.wr_rp = mt76_mmio_wr_rp,
		.rd_rp = mt76_mmio_rd_rp,
		.type = MT76_BUS_MMIO,
	};

	dev->bus = &mt76_mmio_ops;
	dev->mmio.regs = regs;

	spin_lock_init(&dev->mmio.irq_lock);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mt76_mmio_init);
