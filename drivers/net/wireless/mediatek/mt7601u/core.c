<<<<<<< HEAD
/*
 * Copyright (C) 2014 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2015 Jakub Kicinski <kubakici@wp.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2014 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2015 Jakub Kicinski <kubakici@wp.pl>
>>>>>>> upstream/android-13
 */

#include "mt7601u.h"

int mt7601u_wait_asic_ready(struct mt7601u_dev *dev)
{
	int i = 100;
	u32 val;

	do {
		if (test_bit(MT7601U_STATE_REMOVED, &dev->state))
			return -EIO;

		val = mt7601u_rr(dev, MT_MAC_CSR0);
		if (val && ~val)
			return 0;

		udelay(10);
	} while (i--);

	return -EIO;
}

bool mt76_poll(struct mt7601u_dev *dev, u32 offset, u32 mask, u32 val,
	       int timeout)
{
	u32 cur;

	timeout /= 10;
	do {
		if (test_bit(MT7601U_STATE_REMOVED, &dev->state))
			return false;

		cur = mt7601u_rr(dev, offset) & mask;
		if (cur == val)
			return true;

		udelay(10);
	} while (timeout-- > 0);

	dev_err(dev->dev, "Error: Time out with reg %08x\n", offset);

	return false;
}

bool mt76_poll_msec(struct mt7601u_dev *dev, u32 offset, u32 mask, u32 val,
		    int timeout)
{
	u32 cur;

	timeout /= 10;
	do {
		if (test_bit(MT7601U_STATE_REMOVED, &dev->state))
			return false;

		cur = mt7601u_rr(dev, offset) & mask;
		if (cur == val)
			return true;

		msleep(10);
	} while (timeout-- > 0);

	dev_err(dev->dev, "Error: Time out with reg %08x\n", offset);

	return false;
}
