// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
=======
/*
 * Parts of this file are
 * Copyright (C) 2022 Intel Corporation
 */
>>>>>>> upstream/android-13
#include <linux/ieee80211.h>
#include <linux/export.h>
#include <net/cfg80211.h>
#include "nl80211.h"
#include "core.h"
#include "rdev-ops.h"


<<<<<<< HEAD
int __cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, bool notify)
=======
static int ___cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
			       struct net_device *dev, unsigned int link_id,
			       bool notify)
>>>>>>> upstream/android-13
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (!rdev->ops->stop_ap)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (!wdev->beacon_interval)
		return -ENOENT;

	err = rdev_stop_ap(rdev, dev);
	if (!err) {
		wdev->conn_owner_nlportid = 0;
		wdev->beacon_interval = 0;
		memset(&wdev->chandef, 0, sizeof(wdev->chandef));
		wdev->ssid_len = 0;
=======
	if (!wdev->links[link_id].ap.beacon_interval)
		return -ENOENT;

	err = rdev_stop_ap(rdev, dev, link_id);
	if (!err) {
		wdev->conn_owner_nlportid = 0;
		wdev->links[link_id].ap.beacon_interval = 0;
		memset(&wdev->links[link_id].ap.chandef, 0,
		       sizeof(wdev->links[link_id].ap.chandef));
		wdev->u.ap.ssid_len = 0;
>>>>>>> upstream/android-13
		rdev_set_qos_map(rdev, dev, NULL);
		if (notify)
			nl80211_send_ap_stopped(wdev);

		/* Should we apply the grace period during beaconing interface
		 * shutdown also?
		 */
		cfg80211_sched_dfs_chan_update(rdev);
	}

	schedule_work(&cfg80211_disconnect_work);

	return err;
}

<<<<<<< HEAD
int cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		     struct net_device *dev, bool notify)
=======
int __cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, int link_id,
		       bool notify)
{
	unsigned int link;
	int ret = 0;

	if (link_id >= 0)
		return ___cfg80211_stop_ap(rdev, dev, link_id, notify);

	for_each_valid_link(dev->ieee80211_ptr, link) {
		int ret1 = ___cfg80211_stop_ap(rdev, dev, link, notify);

		if (ret1)
			ret = ret1;
		/* try the next one also if one errored */
	}

	return ret;
}

int cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		     struct net_device *dev, int link_id,
		     bool notify)
>>>>>>> upstream/android-13
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

	wdev_lock(wdev);
<<<<<<< HEAD
	err = __cfg80211_stop_ap(rdev, dev, notify);
=======
	err = __cfg80211_stop_ap(rdev, dev, link_id, notify);
>>>>>>> upstream/android-13
	wdev_unlock(wdev);

	return err;
}
