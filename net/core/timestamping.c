<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * PTP 1588 clock support - support for timestamping in PHY devices
 *
 * Copyright (C) 2010 OMICRON electronics GmbH
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
 */
#include <linux/errqueue.h>
#include <linux/phy.h>
#include <linux/ptp_classify.h>
#include <linux/skbuff.h>
#include <linux/export.h>

static unsigned int classify(const struct sk_buff *skb)
{
	if (likely(skb->dev && skb->dev->phydev &&
<<<<<<< HEAD
		   skb->dev->phydev->drv))
=======
		   skb->dev->phydev->mii_ts))
>>>>>>> upstream/android-13
		return ptp_classify_raw(skb);
	else
		return PTP_CLASS_NONE;
}

void skb_clone_tx_timestamp(struct sk_buff *skb)
{
<<<<<<< HEAD
	struct phy_device *phydev;
=======
	struct mii_timestamper *mii_ts;
>>>>>>> upstream/android-13
	struct sk_buff *clone;
	unsigned int type;

	if (!skb->sk)
		return;

	type = classify(skb);
	if (type == PTP_CLASS_NONE)
		return;

<<<<<<< HEAD
	phydev = skb->dev->phydev;
	if (likely(phydev->drv->txtstamp)) {
		clone = skb_clone_sk(skb);
		if (!clone)
			return;
		phydev->drv->txtstamp(phydev, clone, type);
=======
	mii_ts = skb->dev->phydev->mii_ts;
	if (likely(mii_ts->txtstamp)) {
		clone = skb_clone_sk(skb);
		if (!clone)
			return;
		mii_ts->txtstamp(mii_ts, clone, type);
>>>>>>> upstream/android-13
	}
}
EXPORT_SYMBOL_GPL(skb_clone_tx_timestamp);

bool skb_defer_rx_timestamp(struct sk_buff *skb)
{
<<<<<<< HEAD
	struct phy_device *phydev;
	unsigned int type;

	if (!skb->dev || !skb->dev->phydev || !skb->dev->phydev->drv)
=======
	struct mii_timestamper *mii_ts;
	unsigned int type;

	if (!skb->dev || !skb->dev->phydev || !skb->dev->phydev->mii_ts)
>>>>>>> upstream/android-13
		return false;

	if (skb_headroom(skb) < ETH_HLEN)
		return false;

	__skb_push(skb, ETH_HLEN);

	type = ptp_classify_raw(skb);

	__skb_pull(skb, ETH_HLEN);

	if (type == PTP_CLASS_NONE)
		return false;

<<<<<<< HEAD
	phydev = skb->dev->phydev;
	if (likely(phydev->drv->rxtstamp))
		return phydev->drv->rxtstamp(phydev, skb, type);
=======
	mii_ts = skb->dev->phydev->mii_ts;
	if (likely(mii_ts->rxtstamp))
		return mii_ts->rxtstamp(mii_ts, skb, type);
>>>>>>> upstream/android-13

	return false;
}
EXPORT_SYMBOL_GPL(skb_defer_rx_timestamp);
