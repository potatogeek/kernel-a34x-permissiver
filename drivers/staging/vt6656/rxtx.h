<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0+
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
<<<<<<< HEAD
 * File: rxtx.h
 *
=======
>>>>>>> upstream/android-13
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Jun. 27, 2002
 *
 */

#ifndef __RXTX_H__
#define __RXTX_H__

#include "device.h"
#include "wcmd.h"
#include "baseband.h"

#define DEFAULT_MGN_LIFETIME_RES_64us	125  /* 64us */
#define DEFAULT_MSDU_LIFETIME_RES_64us  8000

<<<<<<< HEAD
=======
/* Length, Service, and Signal fields of Phy for Tx */
struct vnt_phy_field {
	u8 signal;
	u8 service;
	__le16 len;
} __packed;

>>>>>>> upstream/android-13
/* MIC HDR data header */
struct vnt_mic_hdr {
	u8 id;
	u8 tx_priority;
	u8 mic_addr2[6];
	u8 ccmp_pn[IEEE80211_CCMP_PN_LEN];
	__be16 payload_len;
	__be16 hlen;
	__le16 frame_control;
	u8 addr1[6];
	u8 addr2[6];
	u8 addr3[6];
	__le16 seq_ctrl;
	u8 addr4[6];
	u16 packing; /* packing to 48 bytes */
} __packed;

/* RsvTime buffer header */
struct vnt_rrv_time_rts {
	__le16 rts_rrv_time_ba;
	__le16 rts_rrv_time_aa;
	__le16 rts_rrv_time_bb;
	u16 wReserved;
	__le16 rrv_time_b;
	__le16 rrv_time_a;
} __packed;

struct vnt_rrv_time_cts {
	__le16 cts_rrv_time_ba;
	u16 wReserved;
	__le16 rrv_time_b;
	__le16 rrv_time_a;
} __packed;

struct vnt_rrv_time_ab {
	__le16 rts_rrv_time;
	__le16 rrv_time;
} __packed;

/* TX data header */
struct vnt_tx_datahead_g {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_b;
	__le16 duration_a;
	__le16 time_stamp_off_b;
	__le16 time_stamp_off_a;
<<<<<<< HEAD
	struct ieee80211_hdr hdr;
} __packed;

struct vnt_tx_datahead_g_fb {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_b;
	__le16 duration_a;
	__le16 duration_a_f0;
	__le16 duration_a_f1;
	__le16 time_stamp_off_b;
	__le16 time_stamp_off_a;
	struct ieee80211_hdr hdr;
=======
>>>>>>> upstream/android-13
} __packed;

struct vnt_tx_datahead_ab {
	struct vnt_phy_field ab;
	__le16 duration;
	__le16 time_stamp_off;
<<<<<<< HEAD
	struct ieee80211_hdr hdr;
} __packed;

struct vnt_tx_datahead_a_fb {
	struct vnt_phy_field a;
	__le16 duration;
	__le16 time_stamp_off;
	__le16 duration_f0;
	__le16 duration_f1;
	struct ieee80211_hdr hdr;
=======
>>>>>>> upstream/android-13
} __packed;

/* RTS buffer header */
struct vnt_rts_g {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_ba;
	__le16 duration_aa;
	__le16 duration_bb;
	u16 wReserved;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_g data_head;
<<<<<<< HEAD
} __packed;

struct vnt_rts_g_fb {
	struct vnt_phy_field b;
	struct vnt_phy_field a;
	__le16 duration_ba;
	__le16 duration_aa;
	__le16 duration_bb;
	u16 wReserved;
	__le16 rts_duration_ba_f0;
	__le16 rts_duration_aa_f0;
	__le16 rts_duration_ba_f1;
	__le16 rts_duration_aa_f1;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_g_fb data_head;
} __packed;
=======
} __packed __aligned(2);
>>>>>>> upstream/android-13

struct vnt_rts_ab {
	struct vnt_phy_field ab;
	__le16 duration;
	u16 wReserved;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_ab data_head;
<<<<<<< HEAD
} __packed;

struct vnt_rts_a_fb {
	struct vnt_phy_field a;
	__le16 duration;
	u16 wReserved;
	__le16 rts_duration_f0;
	__le16 rts_duration_f1;
	struct ieee80211_rts data;
	struct vnt_tx_datahead_a_fb data_head;
} __packed;
=======
} __packed __aligned(2);
>>>>>>> upstream/android-13

/* CTS buffer header */
struct vnt_cts {
	struct vnt_phy_field b;
	__le16 duration_ba;
	u16 wReserved;
	struct ieee80211_cts data;
	u16 reserved2;
	struct vnt_tx_datahead_g data_head;
<<<<<<< HEAD
} __packed;

struct vnt_cts_fb {
	struct vnt_phy_field b;
	__le16 duration_ba;
	u16 wReserved;
	__le16 cts_duration_ba_f0;
	__le16 cts_duration_ba_f1;
	struct ieee80211_cts data;
	u16 reserved2;
	struct vnt_tx_datahead_g_fb data_head;
} __packed;
=======
} __packed __aligned(2);
>>>>>>> upstream/android-13

union vnt_tx_data_head {
	/* rts g */
	struct vnt_rts_g rts_g;
<<<<<<< HEAD
	struct vnt_rts_g_fb rts_g_fb;
	/* rts a/b */
	struct vnt_rts_ab rts_ab;
	struct vnt_rts_a_fb rts_a_fb;
	/* cts g */
	struct vnt_cts cts_g;
	struct vnt_cts_fb cts_g_fb;
	/* no rts/cts */
	struct vnt_tx_datahead_a_fb data_head_a_fb;
=======
	/* rts a/b */
	struct vnt_rts_ab rts_ab;
	/* cts g */
	struct vnt_cts cts_g;
	/* no rts/cts */
>>>>>>> upstream/android-13
	struct vnt_tx_datahead_ab data_head_ab;
};

struct vnt_tx_mic_hdr {
	struct vnt_mic_hdr hdr;
	union vnt_tx_data_head head;
} __packed;

union vnt_tx {
	struct vnt_tx_mic_hdr mic;
	union vnt_tx_data_head head;
};

union vnt_tx_head {
	struct {
		struct vnt_rrv_time_rts rts;
		union vnt_tx tx;
	} __packed tx_rts;
	struct {
		struct vnt_rrv_time_cts cts;
		union vnt_tx tx;
	} __packed tx_cts;
	struct {
		struct vnt_rrv_time_ab ab;
		union vnt_tx tx;
	} __packed tx_ab;
};

struct vnt_tx_fifo_head {
	u8 tx_key[WLAN_KEY_LEN_CCMP];
	__le16 fifo_ctl;
	__le16 time_stamp;
	__le16 frag_ctl;
	__le16 current_rate;
} __packed;

struct vnt_tx_buffer {
<<<<<<< HEAD
	u8 type;
	u8 pkt_no;
	__le16 tx_byte_count;
=======
>>>>>>> upstream/android-13
	struct vnt_tx_fifo_head fifo_head;
	union vnt_tx_head tx_head;
} __packed;

struct vnt_tx_short_buf_head {
	__le16 fifo_ctl;
	u16 time_stamp;
	struct vnt_phy_field ab;
	__le16 duration;
	__le16 time_stamp_off;
} __packed;

<<<<<<< HEAD
struct vnt_beacon_buffer {
	u8 type;
	u8 pkt_no;
	__le16 tx_byte_count;
	struct vnt_tx_short_buf_head short_head;
	struct ieee80211_mgmt mgmt_hdr;
} __packed;

=======
>>>>>>> upstream/android-13
int vnt_tx_packet(struct vnt_private *priv, struct sk_buff *skb);
int vnt_beacon_make(struct vnt_private *priv, struct ieee80211_vif *vif);
int vnt_beacon_enable(struct vnt_private *priv, struct ieee80211_vif *vif,
		      struct ieee80211_bss_conf *conf);

#endif /* __RXTX_H__ */
