// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
<<<<<<< HEAD
 * File: rxtx.c
 *
=======
>>>>>>> upstream/android-13
 * Purpose: handle WMAC/802.3/802.11 rx & tx functions
 *
 * Author: Lyndon Chen
 *
 * Date: May 20, 2003
 *
 * Functions:
 *      vnt_generate_tx_parameter - Generate tx dma required parameter.
<<<<<<< HEAD
 *      vnt_get_duration_le - get tx data required duration
 *      vnt_get_rtscts_duration_le- get rtx/cts required duration
 *      vnt_get_rtscts_rsvtime_le- get rts/cts reserved time
=======
>>>>>>> upstream/android-13
 *      vnt_get_rsvtime- get frame reserved time
 *      vnt_fill_cts_head- fulfill CTS ctl header
 *
 * Revision History:
 *
 */

#include <linux/etherdevice.h>
#include "device.h"
#include "rxtx.h"
#include "card.h"
#include "mac.h"
#include "rf.h"
#include "usbpipe.h"

static const u16 vnt_time_stampoff[2][MAX_RATE] = {
	/* Long Preamble */
	{384, 288, 226, 209, 54, 43, 37, 31, 28, 25, 24, 23},

	/* Short Preamble */
	{384, 192, 130, 113, 54, 43, 37, 31, 28, 25, 24, 23},
};

<<<<<<< HEAD
static const u16 vnt_fb_opt0[2][5] = {
	{RATE_12M, RATE_18M, RATE_24M, RATE_36M, RATE_48M}, /* fallback_rate0 */
	{RATE_12M, RATE_12M, RATE_18M, RATE_24M, RATE_36M}, /* fallback_rate1 */
};

static const u16 vnt_fb_opt1[2][5] = {
	{RATE_12M, RATE_18M, RATE_24M, RATE_24M, RATE_36M}, /* fallback_rate0 */
	{RATE_6M,  RATE_6M,  RATE_12M, RATE_12M, RATE_18M}, /* fallback_rate1 */
};

#define RTSDUR_BB       0
#define RTSDUR_BA       1
#define RTSDUR_AA       2
#define CTSDUR_BA       3
#define RTSDUR_BA_F0    4
#define RTSDUR_AA_F0    5
#define RTSDUR_BA_F1    6
#define RTSDUR_AA_F1    7
#define CTSDUR_BA_F0    8
#define CTSDUR_BA_F1    9
#define DATADUR_B       10
#define DATADUR_A       11
#define DATADUR_A_F0    12
#define DATADUR_A_F1    13
=======
#define DATADUR_B       10
#define DATADUR_A       11

static const u8 vnt_phy_signal[] = {
	0x00,	/* RATE_1M  */
	0x01,	/* RATE_2M  */
	0x02,	/* RATE_5M  */
	0x03,	/* RATE_11M */
	0x8b,	/* RATE_6M  */
	0x8f,	/* RATE_9M  */
	0x8a,	/* RATE_12M */
	0x8e,	/* RATE_18M */
	0x89,	/* RATE_24M */
	0x8d,	/* RATE_36M */
	0x88,	/* RATE_48M */
	0x8c	/* RATE_54M */
};
>>>>>>> upstream/android-13

static struct vnt_usb_send_context
	*vnt_get_free_context(struct vnt_private *priv)
{
	struct vnt_usb_send_context *context = NULL;
	int ii;

	dev_dbg(&priv->usb->dev, "%s\n", __func__);

	for (ii = 0; ii < priv->num_tx_context; ii++) {
		if (!priv->tx_context[ii])
			return NULL;

		context = priv->tx_context[ii];
		if (!context->in_use) {
			context->in_use = true;
<<<<<<< HEAD
			memset(context->data, 0,
			       MAX_TOTAL_SIZE_WITH_ALL_HEADERS);

			context->hdr = NULL;

=======
>>>>>>> upstream/android-13
			return context;
		}
	}

	if (ii == priv->num_tx_context) {
		dev_dbg(&priv->usb->dev, "%s No Free Tx Context\n", __func__);

		ieee80211_stop_queues(priv->hw);
	}

	return NULL;
}

<<<<<<< HEAD
=======
/* Get Length, Service, and Signal fields of Phy for Tx */
static void vnt_get_phy_field(struct vnt_private *priv, u32 frame_length,
			      u16 tx_rate, u8 pkt_type,
			      struct vnt_phy_field *phy)
{
	u32 bit_count;
	u32 count = 0;
	u32 tmp;
	int ext_bit;
	int i;
	u8 mask = 0;
	u8 preamble_type = priv->preamble_type;

	bit_count = frame_length * 8;
	ext_bit = false;

	switch (tx_rate) {
	case RATE_1M:
		count = bit_count;
		break;
	case RATE_2M:
		count = bit_count / 2;
		break;
	case RATE_5M:
		count = DIV_ROUND_UP(bit_count * 10, 55);
		break;
	case RATE_11M:
		count = bit_count / 11;
		tmp = count * 11;

		if (tmp != bit_count) {
			count++;

			if ((bit_count - tmp) <= 3)
				ext_bit = true;
		}

		break;
	}

	if (tx_rate > RATE_11M) {
		if (pkt_type == PK_TYPE_11A)
			mask = BIT(4);
	} else if (tx_rate > RATE_1M) {
		if (preamble_type == PREAMBLE_SHORT)
			mask = BIT(3);
	}

	i = tx_rate > RATE_54M ? RATE_54M : tx_rate;
	phy->signal = vnt_phy_signal[i] | mask;
	phy->service = 0x00;

	if (pkt_type == PK_TYPE_11B) {
		if (ext_bit)
			phy->service |= 0x80;
		phy->len = cpu_to_le16((u16)count);
	} else {
		phy->len = cpu_to_le16((u16)frame_length);
	}
}

>>>>>>> upstream/android-13
static __le16 vnt_time_stamp_off(struct vnt_private *priv, u16 rate)
{
	return cpu_to_le16(vnt_time_stampoff[priv->preamble_type % 2]
							[rate % MAX_RATE]);
}

<<<<<<< HEAD
static u32 vnt_get_rsvtime(struct vnt_private *priv, u8 pkt_type,
			   u32 frame_length, u16 rate, int need_ack)
{
	u32 data_time, ack_time;

	data_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
				       frame_length, rate);

	if (pkt_type == PK_TYPE_11B)
		ack_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, (u16)priv->top_cck_basic_rate);
	else
		ack_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, (u16)priv->top_ofdm_basic_rate);

	if (need_ack)
		return data_time + priv->sifs + ack_time;

	return data_time;
}

static __le16 vnt_rxtx_rsvtime_le16(struct vnt_private *priv, u8 pkt_type,
				    u32 frame_length, u16 rate, int need_ack)
{
	return cpu_to_le16((u16)vnt_get_rsvtime(priv, pkt_type,
		frame_length, rate, need_ack));
}

static __le16 vnt_get_rtscts_rsvtime_le(struct vnt_private *priv, u8 rsv_type,
					u8 pkt_type, u32 frame_length,
					u16 current_rate)
{
	u32 rrv_time, rts_time, cts_time, ack_time, data_time;

	rrv_time = 0;
	rts_time = 0;
	cts_time = 0;
	ack_time = 0;

	data_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
				       frame_length, current_rate);

	if (rsv_type == 0) {
		rts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      20, priv->top_cck_basic_rate);
		ack_time = vnt_get_frame_time(priv->preamble_type,
					      pkt_type, 14,
					      priv->top_cck_basic_rate);
		cts_time = ack_time;

	} else if (rsv_type == 1) {
		rts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      20, priv->top_cck_basic_rate);
		cts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, priv->top_cck_basic_rate);
		ack_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, priv->top_ofdm_basic_rate);
	} else if (rsv_type == 2) {
		rts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      20, priv->top_ofdm_basic_rate);
		ack_time = vnt_get_frame_time(priv->preamble_type,
					      pkt_type, 14,
					      priv->top_ofdm_basic_rate);
		cts_time = ack_time;

	} else if (rsv_type == 3) {
		cts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, priv->top_cck_basic_rate);
		ack_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, priv->top_ofdm_basic_rate);

		rrv_time = cts_time + ack_time + data_time + 2 * priv->sifs;

		return cpu_to_le16((u16)rrv_time);
	}

	rrv_time = rts_time + cts_time + ack_time + data_time + 3 * priv->sifs;

	return cpu_to_le16((u16)rrv_time);
}

static __le16 vnt_get_duration_le(struct vnt_private *priv, u8 pkt_type,
				  int need_ack)
{
	u32 ack_time = 0;

	if (need_ack) {
		if (pkt_type == PK_TYPE_11B)
			ack_time = vnt_get_frame_time(priv->preamble_type,
						      pkt_type, 14,
						      priv->top_cck_basic_rate);
		else
			ack_time = vnt_get_frame_time(priv->preamble_type,
						      pkt_type, 14,
						      priv->top_ofdm_basic_rate);

		return cpu_to_le16((u16)(priv->sifs + ack_time));
	}

	return 0;
}

static __le16 vnt_get_rtscts_duration_le(struct vnt_usb_send_context *context,
					 u8 dur_type, u8 pkt_type, u16 rate)
{
	struct vnt_private *priv = context->priv;
	u32 cts_time = 0, dur_time = 0;
	u32 frame_length = context->frame_len;
	u8 need_ack = context->need_ack;

	switch (dur_type) {
	case RTSDUR_BB:
	case RTSDUR_BA:
	case RTSDUR_BA_F0:
	case RTSDUR_BA_F1:
		cts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, priv->top_cck_basic_rate);
		dur_time = cts_time + 2 * priv->sifs +
			vnt_get_rsvtime(priv, pkt_type,
					frame_length, rate, need_ack);
		break;

	case RTSDUR_AA:
	case RTSDUR_AA_F0:
	case RTSDUR_AA_F1:
		cts_time = vnt_get_frame_time(priv->preamble_type, pkt_type,
					      14, priv->top_ofdm_basic_rate);
		dur_time = cts_time + 2 * priv->sifs +
			vnt_get_rsvtime(priv, pkt_type,
					frame_length, rate, need_ack);
		break;

	case CTSDUR_BA:
	case CTSDUR_BA_F0:
	case CTSDUR_BA_F1:
		dur_time = priv->sifs + vnt_get_rsvtime(priv,
				pkt_type, frame_length, rate, need_ack);
		break;

	default:
		break;
	}

	return cpu_to_le16((u16)dur_time);
}

static u16 vnt_mac_hdr_pos(struct vnt_usb_send_context *tx_context,
			   struct ieee80211_hdr *hdr)
{
	u8 *head = tx_context->data + offsetof(struct vnt_tx_buffer, fifo_head);
	u8 *hdr_pos = (u8 *)hdr;

	tx_context->hdr = hdr;
	if (!tx_context->hdr)
		return 0;

	return (u16)(hdr_pos - head);
}

static u16 vnt_rxtx_datahead_g(struct vnt_usb_send_context *tx_context,
			       struct vnt_tx_datahead_g *buf)
=======
static __le16 vnt_rxtx_rsvtime_le16(struct vnt_usb_send_context *context)
{
	struct vnt_private *priv = context->priv;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(context->skb);
	struct ieee80211_rate *rate = ieee80211_get_tx_rate(priv->hw, info);

	return ieee80211_generic_frame_duration(priv->hw,
						 info->control.vif, info->band,
						 context->frame_len,
						 rate);
}

static __le16 vnt_get_rts_duration(struct vnt_usb_send_context *context)
{
	struct vnt_private *priv = context->priv;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(context->skb);

	return ieee80211_rts_duration(priv->hw, priv->vif,
				      context->frame_len, info);
}

static __le16 vnt_get_cts_duration(struct vnt_usb_send_context *context)
{
	struct vnt_private *priv = context->priv;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(context->skb);

	return ieee80211_ctstoself_duration(priv->hw, priv->vif,
					    context->frame_len, info);
}

static void vnt_rxtx_datahead_g(struct vnt_usb_send_context *tx_context,
				struct vnt_tx_datahead_g *buf)
>>>>>>> upstream/android-13
{
	struct vnt_private *priv = tx_context->priv;
	struct ieee80211_hdr *hdr =
				(struct ieee80211_hdr *)tx_context->skb->data;
	u32 frame_len = tx_context->frame_len;
	u16 rate = tx_context->tx_rate;
<<<<<<< HEAD
	u8 need_ack = tx_context->need_ack;
=======
>>>>>>> upstream/android-13

	/* Get SignalField,ServiceField,Length */
	vnt_get_phy_field(priv, frame_len, rate, tx_context->pkt_type, &buf->a);
	vnt_get_phy_field(priv, frame_len, priv->top_cck_basic_rate,
			  PK_TYPE_11B, &buf->b);

	/* Get Duration and TimeStamp */
<<<<<<< HEAD
	if (ieee80211_is_nullfunc(hdr->frame_control)) {
		buf->duration_a = hdr->duration_id;
		buf->duration_b = hdr->duration_id;
	} else {
		buf->duration_a = vnt_get_duration_le(priv,
						tx_context->pkt_type, need_ack);
		buf->duration_b = vnt_get_duration_le(priv,
							PK_TYPE_11B, need_ack);
	}

	buf->time_stamp_off_a = vnt_time_stamp_off(priv, rate);
	buf->time_stamp_off_b = vnt_time_stamp_off(priv,
					priv->top_cck_basic_rate);

	tx_context->tx_hdr_size = vnt_mac_hdr_pos(tx_context, &buf->hdr);

	return le16_to_cpu(buf->duration_a);
}

static u16 vnt_rxtx_datahead_g_fb(struct vnt_usb_send_context *tx_context,
				  struct vnt_tx_datahead_g_fb *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u32 frame_len = tx_context->frame_len;
	u16 rate = tx_context->tx_rate;
	u8 need_ack = tx_context->need_ack;

	/* Get SignalField,ServiceField,Length */
	vnt_get_phy_field(priv, frame_len, rate, tx_context->pkt_type, &buf->a);

	vnt_get_phy_field(priv, frame_len, priv->top_cck_basic_rate,
			  PK_TYPE_11B, &buf->b);

	/* Get Duration and TimeStamp */
	buf->duration_a = vnt_get_duration_le(priv, tx_context->pkt_type,
					      need_ack);
	buf->duration_b = vnt_get_duration_le(priv, PK_TYPE_11B, need_ack);

	buf->duration_a_f0 = vnt_get_duration_le(priv, tx_context->pkt_type,
						 need_ack);
	buf->duration_a_f1 = vnt_get_duration_le(priv, tx_context->pkt_type,
						 need_ack);

	buf->time_stamp_off_a = vnt_time_stamp_off(priv, rate);
	buf->time_stamp_off_b = vnt_time_stamp_off(priv,
						priv->top_cck_basic_rate);

	tx_context->tx_hdr_size = vnt_mac_hdr_pos(tx_context, &buf->hdr);

	return le16_to_cpu(buf->duration_a);
}

static u16 vnt_rxtx_datahead_a_fb(struct vnt_usb_send_context *tx_context,
				  struct vnt_tx_datahead_a_fb *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u16 rate = tx_context->tx_rate;
	u8 pkt_type = tx_context->pkt_type;
	u8 need_ack = tx_context->need_ack;
	u32 frame_len = tx_context->frame_len;

	/* Get SignalField,ServiceField,Length */
	vnt_get_phy_field(priv, frame_len, rate, pkt_type, &buf->a);
	/* Get Duration and TimeStampOff */
	buf->duration = vnt_get_duration_le(priv, pkt_type, need_ack);

	buf->duration_f0 = vnt_get_duration_le(priv, pkt_type, need_ack);
	buf->duration_f1 = vnt_get_duration_le(priv, pkt_type, need_ack);

	buf->time_stamp_off = vnt_time_stamp_off(priv, rate);

	tx_context->tx_hdr_size = vnt_mac_hdr_pos(tx_context, &buf->hdr);

	return le16_to_cpu(buf->duration);
}

static u16 vnt_rxtx_datahead_ab(struct vnt_usb_send_context *tx_context,
				struct vnt_tx_datahead_ab *buf)
=======
	buf->duration_a = hdr->duration_id;
	buf->duration_b = hdr->duration_id;
	buf->time_stamp_off_a = vnt_time_stamp_off(priv, rate);
	buf->time_stamp_off_b = vnt_time_stamp_off(priv,
						   priv->top_cck_basic_rate);
}

static void vnt_rxtx_datahead_ab(struct vnt_usb_send_context *tx_context,
				 struct vnt_tx_datahead_ab *buf)
>>>>>>> upstream/android-13
{
	struct vnt_private *priv = tx_context->priv;
	struct ieee80211_hdr *hdr =
				(struct ieee80211_hdr *)tx_context->skb->data;
	u32 frame_len = tx_context->frame_len;
	u16 rate = tx_context->tx_rate;
<<<<<<< HEAD
	u8 need_ack = tx_context->need_ack;
=======
>>>>>>> upstream/android-13

	/* Get SignalField,ServiceField,Length */
	vnt_get_phy_field(priv, frame_len, rate,
			  tx_context->pkt_type, &buf->ab);

	/* Get Duration and TimeStampOff */
<<<<<<< HEAD
	if (ieee80211_is_nullfunc(hdr->frame_control)) {
		buf->duration = hdr->duration_id;
	} else {
		buf->duration = vnt_get_duration_le(priv, tx_context->pkt_type,
						    need_ack);
	}

	buf->time_stamp_off = vnt_time_stamp_off(priv, rate);

	tx_context->tx_hdr_size = vnt_mac_hdr_pos(tx_context, &buf->hdr);

	return le16_to_cpu(buf->duration);
}

static int vnt_fill_ieee80211_rts(struct vnt_usb_send_context *tx_context,
				  struct ieee80211_rts *rts, __le16 duration)
=======
	buf->duration = hdr->duration_id;
	buf->time_stamp_off = vnt_time_stamp_off(priv, rate);
}

static void vnt_fill_ieee80211_rts(struct vnt_usb_send_context *tx_context,
				   struct ieee80211_rts *rts, __le16 duration)
>>>>>>> upstream/android-13
{
	struct ieee80211_hdr *hdr =
				(struct ieee80211_hdr *)tx_context->skb->data;

	rts->duration = duration;
	rts->frame_control =
		cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_RTS);

	ether_addr_copy(rts->ra, hdr->addr1);
	ether_addr_copy(rts->ta, hdr->addr2);
<<<<<<< HEAD

	return 0;
}

static u16 vnt_rxtx_rts_g_head(struct vnt_usb_send_context *tx_context,
			       struct vnt_rts_g *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u16 rts_frame_len = 20;
	u16 current_rate = tx_context->tx_rate;

	vnt_get_phy_field(priv, rts_frame_len, priv->top_cck_basic_rate,
			  PK_TYPE_11B, &buf->b);
	vnt_get_phy_field(priv, rts_frame_len, priv->top_ofdm_basic_rate,
			  tx_context->pkt_type, &buf->a);

	buf->duration_bb = vnt_get_rtscts_duration_le(tx_context, RTSDUR_BB,
						      PK_TYPE_11B,
						      priv->top_cck_basic_rate);
	buf->duration_aa = vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA,
						      tx_context->pkt_type,
						      current_rate);
	buf->duration_ba = vnt_get_rtscts_duration_le(tx_context, RTSDUR_BA,
						      tx_context->pkt_type,
						      current_rate);

	vnt_fill_ieee80211_rts(tx_context, &buf->data, buf->duration_aa);

	return vnt_rxtx_datahead_g(tx_context, &buf->data_head);
}

static u16 vnt_rxtx_rts_g_fb_head(struct vnt_usb_send_context *tx_context,
				  struct vnt_rts_g_fb *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u16 current_rate = tx_context->tx_rate;
	u16 rts_frame_len = 20;
=======
}

static void vnt_rxtx_rts_g_head(struct vnt_usb_send_context *tx_context,
				struct vnt_rts_g *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u16 rts_frame_len = 20;
>>>>>>> upstream/android-13

	vnt_get_phy_field(priv, rts_frame_len, priv->top_cck_basic_rate,
			  PK_TYPE_11B, &buf->b);
	vnt_get_phy_field(priv, rts_frame_len, priv->top_ofdm_basic_rate,
			  tx_context->pkt_type, &buf->a);

<<<<<<< HEAD
	buf->duration_bb = vnt_get_rtscts_duration_le(tx_context, RTSDUR_BB,
						      PK_TYPE_11B,
						      priv->top_cck_basic_rate);
	buf->duration_aa = vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA,
						      tx_context->pkt_type,
						      current_rate);
	buf->duration_ba = vnt_get_rtscts_duration_le(tx_context, RTSDUR_BA,
						      tx_context->pkt_type,
						      current_rate);

	buf->rts_duration_ba_f0 =
		vnt_get_rtscts_duration_le(tx_context, RTSDUR_BA_F0,
					   tx_context->pkt_type,
					   priv->tx_rate_fb0);
	buf->rts_duration_aa_f0 =
		vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA_F0,
					   tx_context->pkt_type,
					   priv->tx_rate_fb0);
	buf->rts_duration_ba_f1 =
		vnt_get_rtscts_duration_le(tx_context, RTSDUR_BA_F1,
					   tx_context->pkt_type,
					   priv->tx_rate_fb1);
	buf->rts_duration_aa_f1 =
		vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA_F1,
					   tx_context->pkt_type,
					   priv->tx_rate_fb1);

	vnt_fill_ieee80211_rts(tx_context, &buf->data, buf->duration_aa);

	return vnt_rxtx_datahead_g_fb(tx_context, &buf->data_head);
}

static u16 vnt_rxtx_rts_ab_head(struct vnt_usb_send_context *tx_context,
				struct vnt_rts_ab *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u16 current_rate = tx_context->tx_rate;
=======
	buf->duration_bb = vnt_get_rts_duration(tx_context);
	buf->duration_aa = buf->duration_bb;
	buf->duration_ba = buf->duration_bb;

	vnt_fill_ieee80211_rts(tx_context, &buf->data, buf->duration_aa);

	vnt_rxtx_datahead_g(tx_context, &buf->data_head);
}

static void vnt_rxtx_rts_ab_head(struct vnt_usb_send_context *tx_context,
				 struct vnt_rts_ab *buf)
{
	struct vnt_private *priv = tx_context->priv;
>>>>>>> upstream/android-13
	u16 rts_frame_len = 20;

	vnt_get_phy_field(priv, rts_frame_len, priv->top_ofdm_basic_rate,
			  tx_context->pkt_type, &buf->ab);

<<<<<<< HEAD
	buf->duration = vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA,
						   tx_context->pkt_type,
						   current_rate);

	vnt_fill_ieee80211_rts(tx_context, &buf->data, buf->duration);

	return vnt_rxtx_datahead_ab(tx_context, &buf->data_head);
}

static u16 vnt_rxtx_rts_a_fb_head(struct vnt_usb_send_context *tx_context,
				  struct vnt_rts_a_fb *buf)
{
	struct vnt_private *priv = tx_context->priv;
	u16 current_rate = tx_context->tx_rate;
	u16 rts_frame_len = 20;

	vnt_get_phy_field(priv, rts_frame_len, priv->top_ofdm_basic_rate,
			  tx_context->pkt_type, &buf->a);

	buf->duration = vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA,
						   tx_context->pkt_type,
						   current_rate);

	buf->rts_duration_f0 =
		vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA_F0,
					   tx_context->pkt_type,
					   priv->tx_rate_fb0);

	buf->rts_duration_f1 =
		vnt_get_rtscts_duration_le(tx_context, RTSDUR_AA_F1,
					   tx_context->pkt_type,
					   priv->tx_rate_fb1);

	vnt_fill_ieee80211_rts(tx_context, &buf->data, buf->duration);

	return vnt_rxtx_datahead_a_fb(tx_context, &buf->data_head);
}

static u16 vnt_fill_cts_fb_head(struct vnt_usb_send_context *tx_context,
				union vnt_tx_data_head *head)
{
	struct vnt_private *priv = tx_context->priv;
	struct vnt_cts_fb *buf = &head->cts_g_fb;
	u32 cts_frame_len = 14;
	u16 current_rate = tx_context->tx_rate;

	/* Get SignalField,ServiceField,Length */
	vnt_get_phy_field(priv, cts_frame_len, priv->top_cck_basic_rate,
			  PK_TYPE_11B, &buf->b);

	buf->duration_ba =
		vnt_get_rtscts_duration_le(tx_context, CTSDUR_BA,
					   tx_context->pkt_type,
					   current_rate);
	/* Get CTSDuration_ba_f0 */
	buf->cts_duration_ba_f0 =
		vnt_get_rtscts_duration_le(tx_context, CTSDUR_BA_F0,
					   tx_context->pkt_type,
					   priv->tx_rate_fb0);
	/* Get CTSDuration_ba_f1 */
	buf->cts_duration_ba_f1 =
		vnt_get_rtscts_duration_le(tx_context, CTSDUR_BA_F1,
					   tx_context->pkt_type,
					   priv->tx_rate_fb1);
	/* Get CTS Frame body */
	buf->data.duration = buf->duration_ba;
	buf->data.frame_control =
		cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_CTS);

	ether_addr_copy(buf->data.ra, priv->current_net_addr);

	return vnt_rxtx_datahead_g_fb(tx_context, &buf->data_head);
}

static u16 vnt_fill_cts_head(struct vnt_usb_send_context *tx_context,
			     union vnt_tx_data_head *head)
=======
	buf->duration = vnt_get_rts_duration(tx_context);

	vnt_fill_ieee80211_rts(tx_context, &buf->data, buf->duration);

	vnt_rxtx_datahead_ab(tx_context, &buf->data_head);
}

static void vnt_fill_cts_head(struct vnt_usb_send_context *tx_context,
			      union vnt_tx_data_head *head)
>>>>>>> upstream/android-13
{
	struct vnt_private *priv = tx_context->priv;
	struct vnt_cts *buf = &head->cts_g;
	u32 cts_frame_len = 14;
<<<<<<< HEAD
	u16 current_rate = tx_context->tx_rate;
=======
>>>>>>> upstream/android-13

	/* Get SignalField,ServiceField,Length */
	vnt_get_phy_field(priv, cts_frame_len, priv->top_cck_basic_rate,
			  PK_TYPE_11B, &buf->b);
	/* Get CTSDuration_ba */
<<<<<<< HEAD
	buf->duration_ba =
		vnt_get_rtscts_duration_le(tx_context, CTSDUR_BA,
					   tx_context->pkt_type,
					   current_rate);
=======
	buf->duration_ba = vnt_get_cts_duration(tx_context);
>>>>>>> upstream/android-13
	/*Get CTS Frame body*/
	buf->data.duration = buf->duration_ba;
	buf->data.frame_control =
		cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_CTS);

	ether_addr_copy(buf->data.ra, priv->current_net_addr);

<<<<<<< HEAD
	return vnt_rxtx_datahead_g(tx_context, &buf->data_head);
}

static u16 vnt_rxtx_rts(struct vnt_usb_send_context *tx_context,
			union vnt_tx_head *tx_head, bool need_mic)
{
	struct vnt_private *priv = tx_context->priv;
	struct vnt_rrv_time_rts *buf = &tx_head->tx_rts.rts;
	union vnt_tx_data_head *head = &tx_head->tx_rts.tx.head;
	u32 frame_len = tx_context->frame_len;
	u16 current_rate = tx_context->tx_rate;
	u8 need_ack = tx_context->need_ack;

	buf->rts_rrv_time_aa = vnt_get_rtscts_rsvtime_le(priv, 2,
			tx_context->pkt_type, frame_len, current_rate);
	buf->rts_rrv_time_ba = vnt_get_rtscts_rsvtime_le(priv, 1,
			tx_context->pkt_type, frame_len, current_rate);
	buf->rts_rrv_time_bb = vnt_get_rtscts_rsvtime_le(priv, 0,
			tx_context->pkt_type, frame_len, current_rate);

	buf->rrv_time_a = vnt_rxtx_rsvtime_le16(priv, tx_context->pkt_type,
						frame_len, current_rate,
						need_ack);
	buf->rrv_time_b = vnt_rxtx_rsvtime_le16(priv, PK_TYPE_11B, frame_len,
					priv->top_cck_basic_rate, need_ack);

	if (need_mic)
		head = &tx_head->tx_rts.tx.mic.head;

	if (tx_context->fb_option)
		return vnt_rxtx_rts_g_fb_head(tx_context, &head->rts_g_fb);

	return vnt_rxtx_rts_g_head(tx_context, &head->rts_g);
}

static u16 vnt_rxtx_cts(struct vnt_usb_send_context *tx_context,
			union vnt_tx_head *tx_head, bool need_mic)
{
	struct vnt_private *priv = tx_context->priv;
	struct vnt_rrv_time_cts *buf = &tx_head->tx_cts.cts;
	union vnt_tx_data_head *head = &tx_head->tx_cts.tx.head;
	u32 frame_len = tx_context->frame_len;
	u16 current_rate = tx_context->tx_rate;
	u8 need_ack = tx_context->need_ack;

	buf->rrv_time_a = vnt_rxtx_rsvtime_le16(priv, tx_context->pkt_type,
					frame_len, current_rate, need_ack);
	buf->rrv_time_b = vnt_rxtx_rsvtime_le16(priv, PK_TYPE_11B,
				frame_len, priv->top_cck_basic_rate, need_ack);

	buf->cts_rrv_time_ba = vnt_get_rtscts_rsvtime_le(priv, 3,
			tx_context->pkt_type, frame_len, current_rate);

	if (need_mic)
		head = &tx_head->tx_cts.tx.mic.head;

	/* Fill CTS */
	if (tx_context->fb_option)
		return vnt_fill_cts_fb_head(tx_context, head);

	return vnt_fill_cts_head(tx_context, head);
}

static u16 vnt_rxtx_ab(struct vnt_usb_send_context *tx_context,
		       union vnt_tx_head *tx_head, bool need_rts, bool need_mic)
{
	struct vnt_private *priv = tx_context->priv;
	struct vnt_rrv_time_ab *buf = &tx_head->tx_ab.ab;
	union vnt_tx_data_head *head = &tx_head->tx_ab.tx.head;
	u32 frame_len = tx_context->frame_len;
	u16 current_rate = tx_context->tx_rate;
	u8 need_ack = tx_context->need_ack;

	buf->rrv_time = vnt_rxtx_rsvtime_le16(priv, tx_context->pkt_type,
			frame_len, current_rate, need_ack);

	if (need_mic)
		head = &tx_head->tx_ab.tx.mic.head;

	if (need_rts) {
		if (tx_context->pkt_type == PK_TYPE_11B)
			buf->rts_rrv_time = vnt_get_rtscts_rsvtime_le(priv, 0,
				tx_context->pkt_type, frame_len, current_rate);
		else /* PK_TYPE_11A */
			buf->rts_rrv_time = vnt_get_rtscts_rsvtime_le(priv, 2,
				tx_context->pkt_type, frame_len, current_rate);

		if (tx_context->fb_option &&
		    tx_context->pkt_type == PK_TYPE_11A)
			return vnt_rxtx_rts_a_fb_head(tx_context,
						      &head->rts_a_fb);

		return vnt_rxtx_rts_ab_head(tx_context, &head->rts_ab);
	}

	if (tx_context->pkt_type == PK_TYPE_11A)
		return vnt_rxtx_datahead_a_fb(tx_context,
					      &head->data_head_a_fb);

	return vnt_rxtx_datahead_ab(tx_context, &head->data_head_ab);
}

static u16 vnt_generate_tx_parameter(struct vnt_usb_send_context *tx_context,
				     struct vnt_tx_buffer *tx_buffer,
				     struct vnt_mic_hdr **mic_hdr, u32 need_mic,
				     bool need_rts)
{
	if (tx_context->pkt_type == PK_TYPE_11GB ||
	    tx_context->pkt_type == PK_TYPE_11GA) {
		if (need_rts) {
			if (need_mic)
				*mic_hdr =
					&tx_buffer->tx_head.tx_rts.tx.mic.hdr;

			return vnt_rxtx_rts(tx_context, &tx_buffer->tx_head,
					    need_mic);
		}

		if (need_mic)
			*mic_hdr = &tx_buffer->tx_head.tx_cts.tx.mic.hdr;

		return vnt_rxtx_cts(tx_context, &tx_buffer->tx_head, need_mic);
	}

	if (need_mic)
		*mic_hdr = &tx_buffer->tx_head.tx_ab.tx.mic.hdr;

	return vnt_rxtx_ab(tx_context, &tx_buffer->tx_head, need_rts, need_mic);
}

static void vnt_fill_txkey(struct vnt_usb_send_context *tx_context,
			   u8 *key_buffer, struct ieee80211_key_conf *tx_key,
			   struct sk_buff *skb, u16 payload_len,
			   struct vnt_mic_hdr *mic_hdr)
{
	struct ieee80211_hdr *hdr = tx_context->hdr;
	u64 pn64;
=======
	vnt_rxtx_datahead_g(tx_context, &buf->data_head);
}

/* returns true if mic_hdr is needed */
static bool vnt_fill_txkey(struct vnt_tx_buffer *tx_buffer, struct sk_buff *skb)
{
	struct vnt_tx_fifo_head *fifo = &tx_buffer->fifo_head;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_key_conf *tx_key = info->control.hw_key;
	struct vnt_mic_hdr *mic_hdr;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	u64 pn64;
	u16 payload_len = skb->len;
>>>>>>> upstream/android-13
	u8 *iv = ((u8 *)hdr + ieee80211_get_hdrlen_from_skb(skb));

	/* strip header and icv len from payload */
	payload_len -= ieee80211_get_hdrlen_from_skb(skb);
	payload_len -= tx_key->icv_len;

	switch (tx_key->cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
<<<<<<< HEAD
		memcpy(key_buffer, iv, 3);
		memcpy(key_buffer + 3, tx_key->key, tx_key->keylen);

		if (tx_key->keylen == WLAN_KEY_LEN_WEP40) {
			memcpy(key_buffer + 8, iv, 3);
			memcpy(key_buffer + 11,
			       tx_key->key, WLAN_KEY_LEN_WEP40);
		}

		break;
	case WLAN_CIPHER_SUITE_TKIP:
		ieee80211_get_tkip_p2k(tx_key, skb, key_buffer);

		break;
	case WLAN_CIPHER_SUITE_CCMP:

		if (!mic_hdr)
			return;
=======
		memcpy(fifo->tx_key, iv, 3);
		memcpy(fifo->tx_key + 3, tx_key->key, tx_key->keylen);

		if (tx_key->keylen == WLAN_KEY_LEN_WEP40) {
			memcpy(fifo->tx_key + 8, iv, 3);
			memcpy(fifo->tx_key + 11,
			       tx_key->key, WLAN_KEY_LEN_WEP40);
		}

		fifo->frag_ctl |= cpu_to_le16(FRAGCTL_LEGACY);
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		ieee80211_get_tkip_p2k(tx_key, skb, fifo->tx_key);

		fifo->frag_ctl |= cpu_to_le16(FRAGCTL_TKIP);
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		if (info->control.use_cts_prot) {
			if (info->control.use_rts)
				mic_hdr = &tx_buffer->tx_head.tx_rts.tx.mic.hdr;
			else
				mic_hdr = &tx_buffer->tx_head.tx_cts.tx.mic.hdr;
		} else {
			mic_hdr = &tx_buffer->tx_head.tx_ab.tx.mic.hdr;
		}
>>>>>>> upstream/android-13

		mic_hdr->id = 0x59;
		mic_hdr->payload_len = cpu_to_be16(payload_len);
		ether_addr_copy(mic_hdr->mic_addr2, hdr->addr2);

		pn64 = atomic64_read(&tx_key->tx_pn);
		mic_hdr->ccmp_pn[5] = pn64;
		mic_hdr->ccmp_pn[4] = pn64 >> 8;
		mic_hdr->ccmp_pn[3] = pn64 >> 16;
		mic_hdr->ccmp_pn[2] = pn64 >> 24;
		mic_hdr->ccmp_pn[1] = pn64 >> 32;
		mic_hdr->ccmp_pn[0] = pn64 >> 40;

		if (ieee80211_has_a4(hdr->frame_control))
			mic_hdr->hlen = cpu_to_be16(28);
		else
			mic_hdr->hlen = cpu_to_be16(22);

		ether_addr_copy(mic_hdr->addr1, hdr->addr1);
		ether_addr_copy(mic_hdr->addr2, hdr->addr2);
		ether_addr_copy(mic_hdr->addr3, hdr->addr3);

		mic_hdr->frame_control = cpu_to_le16(
			le16_to_cpu(hdr->frame_control) & 0xc78f);
		mic_hdr->seq_ctrl = cpu_to_le16(
				le16_to_cpu(hdr->seq_ctrl) & 0xf);

		if (ieee80211_has_a4(hdr->frame_control))
			ether_addr_copy(mic_hdr->addr4, hdr->addr4);

<<<<<<< HEAD
		memcpy(key_buffer, tx_key->key, WLAN_KEY_LEN_CCMP);

		break;
	default:
		break;
	}
=======
		memcpy(fifo->tx_key, tx_key->key, WLAN_KEY_LEN_CCMP);

		fifo->frag_ctl |= cpu_to_le16(FRAGCTL_AES);
		return true;
	default:
		break;
	}

	return false;
}

static void vnt_rxtx_rts(struct vnt_usb_send_context *tx_context)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx_context->skb);
	struct vnt_tx_buffer *tx_buffer = tx_context->tx_buffer;
	union vnt_tx_head *tx_head = &tx_buffer->tx_head;
	struct vnt_rrv_time_rts *buf = &tx_head->tx_rts.rts;
	union vnt_tx_data_head *head = &tx_head->tx_rts.tx.head;

	buf->rts_rrv_time_aa = vnt_get_rts_duration(tx_context);
	buf->rts_rrv_time_ba = buf->rts_rrv_time_aa;
	buf->rts_rrv_time_bb = buf->rts_rrv_time_aa;

	buf->rrv_time_a = vnt_rxtx_rsvtime_le16(tx_context);
	buf->rrv_time_b = buf->rrv_time_a;

	if (info->control.hw_key) {
		if (vnt_fill_txkey(tx_buffer, tx_context->skb))
			head = &tx_head->tx_rts.tx.mic.head;
	}

	vnt_rxtx_rts_g_head(tx_context, &head->rts_g);
}

static void vnt_rxtx_cts(struct vnt_usb_send_context *tx_context)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx_context->skb);
	struct vnt_tx_buffer *tx_buffer = tx_context->tx_buffer;
	union vnt_tx_head *tx_head = &tx_buffer->tx_head;
	struct vnt_rrv_time_cts *buf = &tx_head->tx_cts.cts;
	union vnt_tx_data_head *head = &tx_head->tx_cts.tx.head;

	buf->rrv_time_a = vnt_rxtx_rsvtime_le16(tx_context);
	buf->rrv_time_b = buf->rrv_time_a;

	buf->cts_rrv_time_ba = vnt_get_cts_duration(tx_context);

	if (info->control.hw_key) {
		if (vnt_fill_txkey(tx_buffer, tx_context->skb))
			head = &tx_head->tx_cts.tx.mic.head;
	}

	vnt_fill_cts_head(tx_context, head);
}

static void vnt_rxtx_ab(struct vnt_usb_send_context *tx_context)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx_context->skb);
	struct vnt_tx_buffer *tx_buffer = tx_context->tx_buffer;
	union vnt_tx_head *tx_head = &tx_buffer->tx_head;
	struct vnt_rrv_time_ab *buf = &tx_head->tx_ab.ab;
	union vnt_tx_data_head *head = &tx_head->tx_ab.tx.head;

	buf->rrv_time = vnt_rxtx_rsvtime_le16(tx_context);

	if (info->control.hw_key) {
		if (vnt_fill_txkey(tx_buffer, tx_context->skb))
			head = &tx_head->tx_ab.tx.mic.head;
	}

	if (info->control.use_rts) {
		buf->rts_rrv_time = vnt_get_rts_duration(tx_context);

		vnt_rxtx_rts_ab_head(tx_context, &head->rts_ab);

		return;
	}

	vnt_rxtx_datahead_ab(tx_context, &head->data_head_ab);
}

static void vnt_generate_tx_parameter(struct vnt_usb_send_context *tx_context)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(tx_context->skb);

	if (info->control.use_cts_prot) {
		if (info->control.use_rts) {
			vnt_rxtx_rts(tx_context);

			return;
		}

		vnt_rxtx_cts(tx_context);

		return;
	}

	vnt_rxtx_ab(tx_context);
}

static u16 vnt_get_hdr_size(struct ieee80211_tx_info *info)
{
	u16 size = sizeof(struct vnt_tx_datahead_ab);

	if (info->control.use_cts_prot) {
		if (info->control.use_rts)
			size = sizeof(struct vnt_rts_g);
		else
			size = sizeof(struct vnt_cts);
	} else if (info->control.use_rts) {
		size = sizeof(struct vnt_rts_ab);
	}

	if (info->control.hw_key) {
		if (info->control.hw_key->cipher == WLAN_CIPHER_SUITE_CCMP)
			size += sizeof(struct vnt_mic_hdr);
	}

	/* Get rrv_time header */
	if (info->control.use_cts_prot) {
		if (info->control.use_rts)
			size += sizeof(struct vnt_rrv_time_rts);
		else
			size += sizeof(struct vnt_rrv_time_cts);
	} else {
		size += sizeof(struct vnt_rrv_time_ab);
	}

	size += sizeof(struct vnt_tx_fifo_head);

	return size;
>>>>>>> upstream/android-13
}

int vnt_tx_packet(struct vnt_private *priv, struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_tx_rate *tx_rate = &info->control.rates[0];
	struct ieee80211_rate *rate;
<<<<<<< HEAD
	struct ieee80211_key_conf *tx_key;
	struct ieee80211_hdr *hdr;
	struct vnt_mic_hdr *mic_hdr = NULL;
=======
	struct ieee80211_hdr *hdr;
>>>>>>> upstream/android-13
	struct vnt_tx_buffer *tx_buffer;
	struct vnt_tx_fifo_head *tx_buffer_head;
	struct vnt_usb_send_context *tx_context;
	unsigned long flags;
<<<<<<< HEAD
	u16 tx_bytes, tx_header_size, tx_body_size, current_rate, duration_id;
	u8 pkt_type, fb_option = AUTO_FB_NONE;
	bool need_rts = false;
	bool need_mic = false;
=======
	u8 pkt_type;
>>>>>>> upstream/android-13

	hdr = (struct ieee80211_hdr *)(skb->data);

	rate = ieee80211_get_tx_rate(priv->hw, info);

<<<<<<< HEAD
	current_rate = rate->hw_value;
	if (priv->current_rate != current_rate &&
	    !(priv->hw->conf.flags & IEEE80211_CONF_OFFCHANNEL)) {
		priv->current_rate = current_rate;
		vnt_schedule_command(priv, WLAN_CMD_SETPOWER);
	}

	if (current_rate > RATE_11M) {
=======
	if (rate->hw_value > RATE_11M) {
>>>>>>> upstream/android-13
		if (info->band == NL80211_BAND_5GHZ) {
			pkt_type = PK_TYPE_11A;
		} else {
			if (tx_rate->flags & IEEE80211_TX_RC_USE_CTS_PROTECT) {
				if (priv->basic_rates & VNT_B_RATES)
					pkt_type = PK_TYPE_11GB;
				else
					pkt_type = PK_TYPE_11GA;
			} else {
				pkt_type = PK_TYPE_11A;
			}
		}
	} else {
		pkt_type = PK_TYPE_11B;
	}

	spin_lock_irqsave(&priv->lock, flags);

	tx_context = vnt_get_free_context(priv);
	if (!tx_context) {
		dev_dbg(&priv->usb->dev, "%s No free context\n", __func__);
		spin_unlock_irqrestore(&priv->lock, flags);
		return -ENOMEM;
	}

<<<<<<< HEAD
	tx_context->skb = skb;
	tx_context->pkt_type = pkt_type;
	tx_context->need_ack = false;
	tx_context->frame_len = skb->len + 4;
	tx_context->tx_rate = current_rate;

	spin_unlock_irqrestore(&priv->lock, flags);

	tx_buffer = (struct vnt_tx_buffer *)tx_context->data;
	tx_buffer_head = &tx_buffer->fifo_head;
	tx_body_size = skb->len;
=======
	tx_context->pkt_type = pkt_type;
	tx_context->frame_len = skb->len + 4;
	tx_context->tx_rate =  rate->hw_value;

	spin_unlock_irqrestore(&priv->lock, flags);

	tx_context->skb = skb_clone(skb, GFP_ATOMIC);
	if (!tx_context->skb) {
		tx_context->in_use = false;
		return -ENOMEM;
	}

	tx_buffer = skb_push(skb, vnt_get_hdr_size(info));
	tx_context->tx_buffer = tx_buffer;
	tx_buffer_head = &tx_buffer->fifo_head;

	tx_context->type = CONTEXT_DATA_PACKET;
>>>>>>> upstream/android-13

	/*Set fifo controls */
	if (pkt_type == PK_TYPE_11A)
		tx_buffer_head->fifo_ctl = 0;
	else if (pkt_type == PK_TYPE_11B)
		tx_buffer_head->fifo_ctl = cpu_to_le16(FIFOCTL_11B);
	else if (pkt_type == PK_TYPE_11GB)
		tx_buffer_head->fifo_ctl = cpu_to_le16(FIFOCTL_11GB);
	else if (pkt_type == PK_TYPE_11GA)
		tx_buffer_head->fifo_ctl = cpu_to_le16(FIFOCTL_11GA);

	if (!ieee80211_is_data(hdr->frame_control)) {
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_GENINT |
							FIFOCTL_ISDMA0);
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_TMOEN);

		tx_buffer_head->time_stamp =
			cpu_to_le16(DEFAULT_MGN_LIFETIME_RES_64us);
	} else {
		tx_buffer_head->time_stamp =
			cpu_to_le16(DEFAULT_MSDU_LIFETIME_RES_64us);
	}

<<<<<<< HEAD
	if (!(info->flags & IEEE80211_TX_CTL_NO_ACK)) {
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_NEEDACK);
		tx_context->need_ack = true;
	}
=======
	if (!(info->flags & IEEE80211_TX_CTL_NO_ACK))
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_NEEDACK);
>>>>>>> upstream/android-13

	if (ieee80211_has_retry(hdr->frame_control))
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_LRETRY);

<<<<<<< HEAD
	if (tx_rate->flags & IEEE80211_TX_RC_USE_SHORT_PREAMBLE)
		priv->preamble_type = PREAMBLE_SHORT;
	else
		priv->preamble_type = PREAMBLE_LONG;

	if (tx_rate->flags & IEEE80211_TX_RC_USE_RTS_CTS) {
		need_rts = true;
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_RTS);
	}
=======
	if (info->control.use_rts)
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_RTS);
>>>>>>> upstream/android-13

	if (ieee80211_has_a4(hdr->frame_control))
		tx_buffer_head->fifo_ctl |= cpu_to_le16(FIFOCTL_LHEAD);

	tx_buffer_head->frag_ctl =
<<<<<<< HEAD
			cpu_to_le16(ieee80211_get_hdrlen_from_skb(skb) << 10);

	if (info->control.hw_key) {
		tx_key = info->control.hw_key;
		switch (info->control.hw_key->cipher) {
		case WLAN_CIPHER_SUITE_WEP40:
		case WLAN_CIPHER_SUITE_WEP104:
			tx_buffer_head->frag_ctl |= cpu_to_le16(FRAGCTL_LEGACY);
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			tx_buffer_head->frag_ctl |= cpu_to_le16(FRAGCTL_TKIP);
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			tx_buffer_head->frag_ctl |= cpu_to_le16(FRAGCTL_AES);
			need_mic = true;
		default:
			break;
		}
		tx_context->frame_len += tx_key->icv_len;
	}

	tx_buffer_head->current_rate = cpu_to_le16(current_rate);

	/* legacy rates TODO use ieee80211_tx_rate */
	if (current_rate >= RATE_18M && ieee80211_is_data(hdr->frame_control)) {
		if (priv->auto_fb_ctrl == AUTO_FB_0) {
			tx_buffer_head->fifo_ctl |=
						cpu_to_le16(FIFOCTL_AUTO_FB_0);

			priv->tx_rate_fb0 =
				vnt_fb_opt0[FB_RATE0][current_rate - RATE_18M];
			priv->tx_rate_fb1 =
				vnt_fb_opt0[FB_RATE1][current_rate - RATE_18M];

			fb_option = AUTO_FB_0;
		} else if (priv->auto_fb_ctrl == AUTO_FB_1) {
			tx_buffer_head->fifo_ctl |=
						cpu_to_le16(FIFOCTL_AUTO_FB_1);

			priv->tx_rate_fb0 =
				vnt_fb_opt1[FB_RATE0][current_rate - RATE_18M];
			priv->tx_rate_fb1 =
				vnt_fb_opt1[FB_RATE1][current_rate - RATE_18M];

			fb_option = AUTO_FB_1;
		}
	}

	tx_context->fb_option = fb_option;

	duration_id = vnt_generate_tx_parameter(tx_context, tx_buffer, &mic_hdr,
						need_mic, need_rts);

	tx_header_size = tx_context->tx_hdr_size;
	if (!tx_header_size) {
		tx_context->in_use = false;
		return -ENOMEM;
	}

	tx_buffer_head->frag_ctl |= cpu_to_le16(FRAGCTL_NONFRAG);

	tx_bytes = tx_header_size + tx_body_size;

	memcpy(tx_context->hdr, skb->data, tx_body_size);

	hdr->duration_id = cpu_to_le16(duration_id);

	if (info->control.hw_key) {
		tx_key = info->control.hw_key;
		if (tx_key->keylen > 0)
			vnt_fill_txkey(tx_context, tx_buffer_head->tx_key,
				       tx_key, skb, tx_body_size, mic_hdr);
	}

	priv->seq_counter = (le16_to_cpu(hdr->seq_ctrl) &
						IEEE80211_SCTL_SEQ) >> 4;

	tx_buffer->tx_byte_count = cpu_to_le16(tx_bytes);
	tx_buffer->pkt_no = tx_context->pkt_no;
	tx_buffer->type = 0x00;

	tx_bytes += 4;

	tx_context->type = CONTEXT_DATA_PACKET;
	tx_context->buf_len = tx_bytes;

	spin_lock_irqsave(&priv->lock, flags);

	if (vnt_tx_context(priv, tx_context) != STATUS_PENDING) {
=======
			cpu_to_le16(ieee80211_hdrlen(hdr->frame_control) << 10);

	if (info->control.hw_key)
		tx_context->frame_len += info->control.hw_key->icv_len;

	tx_buffer_head->current_rate = cpu_to_le16(rate->hw_value);

	vnt_generate_tx_parameter(tx_context);

	tx_buffer_head->frag_ctl |= cpu_to_le16(FRAGCTL_NONFRAG);

	priv->seq_counter = (le16_to_cpu(hdr->seq_ctrl) &
						IEEE80211_SCTL_SEQ) >> 4;

	spin_lock_irqsave(&priv->lock, flags);

	if (vnt_tx_context(priv, tx_context, skb)) {
		dev_kfree_skb(tx_context->skb);
>>>>>>> upstream/android-13
		spin_unlock_irqrestore(&priv->lock, flags);
		return -EIO;
	}

<<<<<<< HEAD
=======
	dev_kfree_skb(skb);

>>>>>>> upstream/android-13
	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}

static int vnt_beacon_xmit(struct vnt_private *priv, struct sk_buff *skb)
{
<<<<<<< HEAD
	struct vnt_beacon_buffer *beacon_buffer;
=======
>>>>>>> upstream/android-13
	struct vnt_tx_short_buf_head *short_head;
	struct ieee80211_tx_info *info;
	struct vnt_usb_send_context *context;
	struct ieee80211_mgmt *mgmt_hdr;
	unsigned long flags;
	u32 frame_size = skb->len + 4;
<<<<<<< HEAD
	u16 current_rate, count;
=======
	u16 current_rate;
>>>>>>> upstream/android-13

	spin_lock_irqsave(&priv->lock, flags);

	context = vnt_get_free_context(priv);
	if (!context) {
		dev_dbg(&priv->usb->dev, "%s No free context!\n", __func__);
		spin_unlock_irqrestore(&priv->lock, flags);
		return -ENOMEM;
	}

	context->skb = skb;

	spin_unlock_irqrestore(&priv->lock, flags);

<<<<<<< HEAD
	beacon_buffer = (struct vnt_beacon_buffer *)&context->data[0];
	short_head = &beacon_buffer->short_head;
=======
	mgmt_hdr = (struct ieee80211_mgmt *)skb->data;
	short_head = skb_push(skb, sizeof(*short_head));
>>>>>>> upstream/android-13

	if (priv->bb_type == BB_TYPE_11A) {
		current_rate = RATE_6M;

		/* Get SignalField,ServiceField,Length */
		vnt_get_phy_field(priv, frame_size, current_rate,
				  PK_TYPE_11A, &short_head->ab);

<<<<<<< HEAD
		/* Get Duration and TimeStampOff */
		short_head->duration = vnt_get_duration_le(priv,
							   PK_TYPE_11A, false);
=======
		/* Get TimeStampOff */
>>>>>>> upstream/android-13
		short_head->time_stamp_off =
				vnt_time_stamp_off(priv, current_rate);
	} else {
		current_rate = RATE_1M;
		short_head->fifo_ctl |= cpu_to_le16(FIFOCTL_11B);

		/* Get SignalField,ServiceField,Length */
		vnt_get_phy_field(priv, frame_size, current_rate,
				  PK_TYPE_11B, &short_head->ab);

<<<<<<< HEAD
		/* Get Duration and TimeStampOff */
		short_head->duration = vnt_get_duration_le(priv,
						PK_TYPE_11B, false);
=======
		/* Get TimeStampOff */
>>>>>>> upstream/android-13
		short_head->time_stamp_off =
			vnt_time_stamp_off(priv, current_rate);
	}

<<<<<<< HEAD
	/* Generate Beacon Header */
	mgmt_hdr = &beacon_buffer->mgmt_hdr;
	memcpy(mgmt_hdr, skb->data, skb->len);
=======
	/* Get Duration */
	short_head->duration = mgmt_hdr->duration;
>>>>>>> upstream/android-13

	/* time stamp always 0 */
	mgmt_hdr->u.beacon.timestamp = 0;

	info = IEEE80211_SKB_CB(skb);
	if (info->flags & IEEE80211_TX_CTL_ASSIGN_SEQ) {
		struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)mgmt_hdr;

		hdr->duration_id = 0;
		hdr->seq_ctrl = cpu_to_le16(priv->seq_counter << 4);
	}

	priv->seq_counter++;
	if (priv->seq_counter > 0x0fff)
		priv->seq_counter = 0;

<<<<<<< HEAD
	count = sizeof(struct vnt_tx_short_buf_head) + skb->len;

	beacon_buffer->tx_byte_count = cpu_to_le16(count);
	beacon_buffer->pkt_no = context->pkt_no;
	beacon_buffer->type = 0x01;

	context->type = CONTEXT_BEACON_PACKET;
	context->buf_len = count + 4; /* USB header */

	spin_lock_irqsave(&priv->lock, flags);

	if (vnt_tx_context(priv, context) != STATUS_PENDING)
=======
	context->type = CONTEXT_BEACON_PACKET;

	spin_lock_irqsave(&priv->lock, flags);

	if (vnt_tx_context(priv, context, skb))
>>>>>>> upstream/android-13
		ieee80211_free_txskb(priv->hw, context->skb);

	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}

int vnt_beacon_make(struct vnt_private *priv, struct ieee80211_vif *vif)
{
	struct sk_buff *beacon;

	beacon = ieee80211_beacon_get(priv->hw, vif);
	if (!beacon)
		return -ENOMEM;

	if (vnt_beacon_xmit(priv, beacon)) {
		ieee80211_free_txskb(priv->hw, beacon);
		return -ENODEV;
	}

	return 0;
}

int vnt_beacon_enable(struct vnt_private *priv, struct ieee80211_vif *vif,
		      struct ieee80211_bss_conf *conf)
{
	vnt_mac_reg_bits_off(priv, MAC_REG_TCR, TCR_AUTOBCNTX);

	vnt_mac_reg_bits_off(priv, MAC_REG_TFTCTL, TFTCTL_TSFCNTREN);

	vnt_mac_set_beacon_interval(priv, conf->beacon_int);

	vnt_clear_current_tsf(priv);

	vnt_mac_reg_bits_on(priv, MAC_REG_TFTCTL, TFTCTL_TSFCNTREN);

	vnt_reset_next_tbtt(priv, conf->beacon_int);

	return vnt_beacon_make(priv, vif);
}
