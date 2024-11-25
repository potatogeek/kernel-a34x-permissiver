<<<<<<< HEAD
/*
 * Copyright (c) 2014,2017 Qualcomm Atheros, Inc.
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
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
 * Copyright (c) 2014,2017 Qualcomm Atheros, Inc.
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/etherdevice.h>
#include <linux/pci.h>
#include <linux/rtnetlink.h>
#include <net/cfg80211.h>

#include "wil6210.h"

<<<<<<< HEAD
static int wil_ethtoolops_begin(struct net_device *ndev)
{
	struct wil6210_priv *wil = ndev_to_wil(ndev);

	mutex_lock(&wil->mutex);

	wil_dbg_misc(wil, "ethtoolops_begin\n");

	return 0;
}

static void wil_ethtoolops_complete(struct net_device *ndev)
{
	struct wil6210_priv *wil = ndev_to_wil(ndev);

	wil_dbg_misc(wil, "ethtoolops_complete\n");

	mutex_unlock(&wil->mutex);
}

static int wil_ethtoolops_get_coalesce(struct net_device *ndev,
				       struct ethtool_coalesce *cp)
=======
static int
wil_ethtoolops_get_coalesce(struct net_device *ndev,
			    struct ethtool_coalesce *cp,
			    struct kernel_ethtool_coalesce *kernel_coal,
			    struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	struct wil6210_priv *wil = ndev_to_wil(ndev);
	u32 tx_itr_en, tx_itr_val = 0;
	u32 rx_itr_en, rx_itr_val = 0;
	int ret;

<<<<<<< HEAD
=======
	mutex_lock(&wil->mutex);
>>>>>>> upstream/android-13
	wil_dbg_misc(wil, "ethtoolops_get_coalesce\n");

	ret = wil_pm_runtime_get(wil);
	if (ret < 0)
<<<<<<< HEAD
		return ret;
=======
		goto out;
>>>>>>> upstream/android-13

	tx_itr_en = wil_r(wil, RGF_DMA_ITR_TX_CNT_CTL);
	if (tx_itr_en & BIT_DMA_ITR_TX_CNT_CTL_EN)
		tx_itr_val = wil_r(wil, RGF_DMA_ITR_TX_CNT_TRSH);

	rx_itr_en = wil_r(wil, RGF_DMA_ITR_RX_CNT_CTL);
	if (rx_itr_en & BIT_DMA_ITR_RX_CNT_CTL_EN)
		rx_itr_val = wil_r(wil, RGF_DMA_ITR_RX_CNT_TRSH);

	wil_pm_runtime_put(wil);

	cp->tx_coalesce_usecs = tx_itr_val;
	cp->rx_coalesce_usecs = rx_itr_val;
<<<<<<< HEAD
	return 0;
}

static int wil_ethtoolops_set_coalesce(struct net_device *ndev,
				       struct ethtool_coalesce *cp)
=======
	ret = 0;

out:
	mutex_unlock(&wil->mutex);
	return ret;
}

static int
wil_ethtoolops_set_coalesce(struct net_device *ndev,
			    struct ethtool_coalesce *cp,
			    struct kernel_ethtool_coalesce *kernel_coal,
			    struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	struct wil6210_priv *wil = ndev_to_wil(ndev);
	struct wireless_dev *wdev = ndev->ieee80211_ptr;
	int ret;

<<<<<<< HEAD
=======
	mutex_lock(&wil->mutex);
>>>>>>> upstream/android-13
	wil_dbg_misc(wil, "ethtoolops_set_coalesce: rx %d usec, tx %d usec\n",
		     cp->rx_coalesce_usecs, cp->tx_coalesce_usecs);

	if (wdev->iftype == NL80211_IFTYPE_MONITOR) {
		wil_dbg_misc(wil, "No IRQ coalescing in monitor mode\n");
<<<<<<< HEAD
		return -EINVAL;
=======
		ret = -EINVAL;
		goto out;
>>>>>>> upstream/android-13
	}

	/* only @rx_coalesce_usecs and @tx_coalesce_usecs supported,
	 * ignore other parameters
	 */

	if (cp->rx_coalesce_usecs > WIL6210_ITR_TRSH_MAX ||
	    cp->tx_coalesce_usecs > WIL6210_ITR_TRSH_MAX)
		goto out_bad;

	wil->tx_max_burst_duration = cp->tx_coalesce_usecs;
	wil->rx_max_burst_duration = cp->rx_coalesce_usecs;

	ret = wil_pm_runtime_get(wil);
	if (ret < 0)
<<<<<<< HEAD
		return ret;
=======
		goto out;
>>>>>>> upstream/android-13

	wil->txrx_ops.configure_interrupt_moderation(wil);

	wil_pm_runtime_put(wil);
<<<<<<< HEAD

	return 0;
=======
	ret = 0;

out:
	mutex_unlock(&wil->mutex);
	return ret;
>>>>>>> upstream/android-13

out_bad:
	wil_dbg_misc(wil, "Unsupported coalescing params. Raw command:\n");
	print_hex_dump_debug("DBG[MISC] coal ", DUMP_PREFIX_OFFSET, 16, 4,
			     cp, sizeof(*cp), false);
<<<<<<< HEAD
=======
	mutex_unlock(&wil->mutex);
>>>>>>> upstream/android-13
	return -EINVAL;
}

static const struct ethtool_ops wil_ethtool_ops = {
<<<<<<< HEAD
	.begin		= wil_ethtoolops_begin,
	.complete	= wil_ethtoolops_complete,
=======
	.supported_coalesce_params = ETHTOOL_COALESCE_USECS,
>>>>>>> upstream/android-13
	.get_drvinfo	= cfg80211_get_drvinfo,
	.get_coalesce	= wil_ethtoolops_get_coalesce,
	.set_coalesce	= wil_ethtoolops_set_coalesce,
};

void wil_set_ethtoolops(struct net_device *ndev)
{
	ndev->ethtool_ops = &wil_ethtool_ops;
}
