<<<<<<< HEAD
/*
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
>>>>>>> upstream/android-13
 */

#ifndef _WME_H
#define _WME_H

#include <linux/netdevice.h>
#include "ieee80211_i.h"

u16 ieee80211_select_queue_80211(struct ieee80211_sub_if_data *sdata,
				 struct sk_buff *skb,
				 struct ieee80211_hdr *hdr);
<<<<<<< HEAD
=======
u16 __ieee80211_select_queue(struct ieee80211_sub_if_data *sdata,
			     struct sta_info *sta, struct sk_buff *skb);
>>>>>>> upstream/android-13
u16 ieee80211_select_queue(struct ieee80211_sub_if_data *sdata,
			   struct sk_buff *skb);
void ieee80211_set_qos_hdr(struct ieee80211_sub_if_data *sdata,
			   struct sk_buff *skb);

#endif /* _WME_H */
