<<<<<<< HEAD
/*
 * Copyright (C) 2010 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2010 Felix Fietkau <nbd@openwrt.org>
>>>>>>> upstream/android-13
 */
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/debugfs.h>
#include <linux/ieee80211.h>
#include <linux/export.h>
#include <net/mac80211.h>
<<<<<<< HEAD
#include "rc80211_minstrel.h"
#include "rc80211_minstrel_ht.h"

=======
#include "rc80211_minstrel_ht.h"

struct minstrel_debugfs_info {
	size_t len;
	char buf[];
};

static ssize_t
minstrel_stats_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
	struct minstrel_debugfs_info *ms;

	ms = file->private_data;
	return simple_read_from_buffer(buf, len, ppos, ms->buf, ms->len);
}

static int
minstrel_stats_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

static bool
minstrel_ht_is_sample_rate(struct minstrel_ht_sta *mi, int idx)
{
	int type, i;

	for (type = 0; type < ARRAY_SIZE(mi->sample); type++)
		for (i = 0; i < MINSTREL_SAMPLE_RATES; i++)
			if (mi->sample[type].cur_sample_rates[i] == idx)
				return true;
	return false;
}

>>>>>>> upstream/android-13
static char *
minstrel_ht_stats_dump(struct minstrel_ht_sta *mi, int i, char *p)
{
	const struct mcs_group *mg;
	unsigned int j, tp_max, tp_avg, eprob, tx_time;
	char htmode = '2';
	char gimode = 'L';
	u32 gflags;

	if (!mi->supported[i])
		return p;

	mg = &minstrel_mcs_groups[i];
	gflags = mg->flags;

	if (gflags & IEEE80211_TX_RC_40_MHZ_WIDTH)
		htmode = '4';
	else if (gflags & IEEE80211_TX_RC_80_MHZ_WIDTH)
		htmode = '8';
	if (gflags & IEEE80211_TX_RC_SHORT_GI)
		gimode = 'S';

	for (j = 0; j < MCS_GROUP_RATES; j++) {
		struct minstrel_rate_stats *mrs = &mi->groups[i].rates[j];
<<<<<<< HEAD
		static const int bitrates[4] = { 10, 20, 55, 110 };
		int idx = i * MCS_GROUP_RATES + j;
		unsigned int prob_ewmsd;
=======
		int idx = MI_RATE(i, j);
		unsigned int duration;
>>>>>>> upstream/android-13

		if (!(mi->supported[i] & BIT(j)))
			continue;

		if (gflags & IEEE80211_TX_RC_MCS) {
			p += sprintf(p, "HT%c0  ", htmode);
			p += sprintf(p, "%cGI  ", gimode);
			p += sprintf(p, "%d  ", mg->streams);
		} else if (gflags & IEEE80211_TX_RC_VHT_MCS) {
			p += sprintf(p, "VHT%c0 ", htmode);
			p += sprintf(p, "%cGI ", gimode);
			p += sprintf(p, "%d  ", mg->streams);
<<<<<<< HEAD
=======
		} else if (i == MINSTREL_OFDM_GROUP) {
			p += sprintf(p, "OFDM       ");
			p += sprintf(p, "1 ");
>>>>>>> upstream/android-13
		} else {
			p += sprintf(p, "CCK    ");
			p += sprintf(p, "%cP  ", j < 4 ? 'L' : 'S');
			p += sprintf(p, "1 ");
		}

		*(p++) = (idx == mi->max_tp_rate[0]) ? 'A' : ' ';
		*(p++) = (idx == mi->max_tp_rate[1]) ? 'B' : ' ';
		*(p++) = (idx == mi->max_tp_rate[2]) ? 'C' : ' ';
		*(p++) = (idx == mi->max_tp_rate[3]) ? 'D' : ' ';
		*(p++) = (idx == mi->max_prob_rate) ? 'P' : ' ';
<<<<<<< HEAD
=======
		*(p++) = minstrel_ht_is_sample_rate(mi, idx) ? 'S' : ' ';
>>>>>>> upstream/android-13

		if (gflags & IEEE80211_TX_RC_MCS) {
			p += sprintf(p, "  MCS%-2u", (mg->streams - 1) * 8 + j);
		} else if (gflags & IEEE80211_TX_RC_VHT_MCS) {
			p += sprintf(p, "  MCS%-1u/%1u", j, mg->streams);
		} else {
<<<<<<< HEAD
			int r = bitrates[j % 4];
=======
			int r;

			if (i == MINSTREL_OFDM_GROUP)
				r = minstrel_ofdm_bitrates[j % 8];
			else
				r = minstrel_cck_bitrates[j % 4];
>>>>>>> upstream/android-13

			p += sprintf(p, "   %2u.%1uM", r / 10, r % 10);
		}

		p += sprintf(p, "  %3u  ", idx);

		/* tx_time[rate(i)] in usec */
<<<<<<< HEAD
		tx_time = DIV_ROUND_CLOSEST(mg->duration[j], 1000);
		p += sprintf(p, "%6u  ", tx_time);

		tp_max = minstrel_ht_get_tp_avg(mi, i, j, MINSTREL_FRAC(100, 100));
		tp_avg = minstrel_ht_get_tp_avg(mi, i, j, mrs->prob_ewma);
		eprob = MINSTREL_TRUNC(mrs->prob_ewma * 1000);
		prob_ewmsd = minstrel_get_ewmsd10(mrs);

		p += sprintf(p, "%4u.%1u    %4u.%1u     %3u.%1u    %3u.%1u"
=======
		duration = mg->duration[j];
		duration <<= mg->shift;
		tx_time = DIV_ROUND_CLOSEST(duration, 1000);
		p += sprintf(p, "%6u  ", tx_time);

		tp_max = minstrel_ht_get_tp_avg(mi, i, j, MINSTREL_FRAC(100, 100));
		tp_avg = minstrel_ht_get_tp_avg(mi, i, j, mrs->prob_avg);
		eprob = MINSTREL_TRUNC(mrs->prob_avg * 1000);

		p += sprintf(p, "%4u.%1u    %4u.%1u     %3u.%1u"
>>>>>>> upstream/android-13
				"     %3u   %3u %-3u   "
				"%9llu   %-9llu\n",
				tp_max / 10, tp_max % 10,
				tp_avg / 10, tp_avg % 10,
				eprob / 10, eprob % 10,
<<<<<<< HEAD
				prob_ewmsd / 10, prob_ewmsd % 10,
=======
>>>>>>> upstream/android-13
				mrs->retry_count,
				mrs->last_success,
				mrs->last_attempts,
				(unsigned long long)mrs->succ_hist,
				(unsigned long long)mrs->att_hist);
	}

	return p;
}

static int
minstrel_ht_stats_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct minstrel_ht_sta_priv *msp = inode->i_private;
	struct minstrel_ht_sta *mi = &msp->ht;
	struct minstrel_debugfs_info *ms;
	unsigned int i;
	int ret;
	char *p;

	if (!msp->is_ht) {
		inode->i_private = &msp->legacy;
		ret = minstrel_stats_open(inode, file);
		inode->i_private = msp;
		return ret;
	}

=======
	struct minstrel_ht_sta *mi = inode->i_private;
	struct minstrel_debugfs_info *ms;
	unsigned int i;
	char *p;

>>>>>>> upstream/android-13
	ms = kmalloc(32768, GFP_KERNEL);
	if (!ms)
		return -ENOMEM;

	file->private_data = ms;
	p = ms->buf;

	p += sprintf(p, "\n");
	p += sprintf(p,
<<<<<<< HEAD
		     "              best   ____________rate__________    ________statistics________    _____last____    ______sum-of________\n");
	p += sprintf(p,
		     "mode guard #  rate  [name   idx airtime  max_tp]  [avg(tp) avg(prob) sd(prob)]  [retry|suc|att]  [#success | #attempts]\n");
=======
		     "              best    ____________rate__________    ____statistics___    _____last____    ______sum-of________\n");
	p += sprintf(p,
		     "mode guard #  rate   [name   idx airtime  max_tp]  [avg(tp) avg(prob)]  [retry|suc|att]  [#success | #attempts]\n");
>>>>>>> upstream/android-13

	p = minstrel_ht_stats_dump(mi, MINSTREL_CCK_GROUP, p);
	for (i = 0; i < MINSTREL_CCK_GROUP; i++)
		p = minstrel_ht_stats_dump(mi, i, p);
	for (i++; i < ARRAY_SIZE(mi->groups); i++)
		p = minstrel_ht_stats_dump(mi, i, p);

	p += sprintf(p, "\nTotal packet count::    ideal %d      "
			"lookaround %d\n",
			max(0, (int) mi->total_packets - (int) mi->sample_packets),
			mi->sample_packets);
<<<<<<< HEAD
	p += sprintf(p, "Average # of aggregated frames per A-MPDU: %d.%d\n",
		MINSTREL_TRUNC(mi->avg_ampdu_len),
		MINSTREL_TRUNC(mi->avg_ampdu_len * 10) % 10);
=======
	if (mi->avg_ampdu_len)
		p += sprintf(p, "Average # of aggregated frames per A-MPDU: %d.%d\n",
			MINSTREL_TRUNC(mi->avg_ampdu_len),
			MINSTREL_TRUNC(mi->avg_ampdu_len * 10) % 10);
>>>>>>> upstream/android-13
	ms->len = p - ms->buf;
	WARN_ON(ms->len + sizeof(*ms) > 32768);

	return nonseekable_open(inode, file);
}

static const struct file_operations minstrel_ht_stat_fops = {
	.owner = THIS_MODULE,
	.open = minstrel_ht_stats_open,
	.read = minstrel_stats_read,
	.release = minstrel_stats_release,
	.llseek = no_llseek,
};

static char *
minstrel_ht_stats_csv_dump(struct minstrel_ht_sta *mi, int i, char *p)
{
	const struct mcs_group *mg;
	unsigned int j, tp_max, tp_avg, eprob, tx_time;
	char htmode = '2';
	char gimode = 'L';
	u32 gflags;

	if (!mi->supported[i])
		return p;

	mg = &minstrel_mcs_groups[i];
	gflags = mg->flags;

	if (gflags & IEEE80211_TX_RC_40_MHZ_WIDTH)
		htmode = '4';
	else if (gflags & IEEE80211_TX_RC_80_MHZ_WIDTH)
		htmode = '8';
	if (gflags & IEEE80211_TX_RC_SHORT_GI)
		gimode = 'S';

	for (j = 0; j < MCS_GROUP_RATES; j++) {
		struct minstrel_rate_stats *mrs = &mi->groups[i].rates[j];
<<<<<<< HEAD
		static const int bitrates[4] = { 10, 20, 55, 110 };
		int idx = i * MCS_GROUP_RATES + j;
		unsigned int prob_ewmsd;
=======
		int idx = MI_RATE(i, j);
		unsigned int duration;
>>>>>>> upstream/android-13

		if (!(mi->supported[i] & BIT(j)))
			continue;

		if (gflags & IEEE80211_TX_RC_MCS) {
			p += sprintf(p, "HT%c0,", htmode);
			p += sprintf(p, "%cGI,", gimode);
			p += sprintf(p, "%d,", mg->streams);
		} else if (gflags & IEEE80211_TX_RC_VHT_MCS) {
			p += sprintf(p, "VHT%c0,", htmode);
			p += sprintf(p, "%cGI,", gimode);
			p += sprintf(p, "%d,", mg->streams);
<<<<<<< HEAD
=======
		} else if (i == MINSTREL_OFDM_GROUP) {
			p += sprintf(p, "OFDM,,1,");
>>>>>>> upstream/android-13
		} else {
			p += sprintf(p, "CCK,");
			p += sprintf(p, "%cP,", j < 4 ? 'L' : 'S');
			p += sprintf(p, "1,");
		}

		p += sprintf(p, "%s" ,((idx == mi->max_tp_rate[0]) ? "A" : ""));
		p += sprintf(p, "%s" ,((idx == mi->max_tp_rate[1]) ? "B" : ""));
		p += sprintf(p, "%s" ,((idx == mi->max_tp_rate[2]) ? "C" : ""));
		p += sprintf(p, "%s" ,((idx == mi->max_tp_rate[3]) ? "D" : ""));
		p += sprintf(p, "%s" ,((idx == mi->max_prob_rate) ? "P" : ""));
<<<<<<< HEAD
=======
		p += sprintf(p, "%s", (minstrel_ht_is_sample_rate(mi, idx) ? "S" : ""));
>>>>>>> upstream/android-13

		if (gflags & IEEE80211_TX_RC_MCS) {
			p += sprintf(p, ",MCS%-2u,", (mg->streams - 1) * 8 + j);
		} else if (gflags & IEEE80211_TX_RC_VHT_MCS) {
			p += sprintf(p, ",MCS%-1u/%1u,", j, mg->streams);
		} else {
<<<<<<< HEAD
			int r = bitrates[j % 4];
=======
			int r;

			if (i == MINSTREL_OFDM_GROUP)
				r = minstrel_ofdm_bitrates[j % 8];
			else
				r = minstrel_cck_bitrates[j % 4];

>>>>>>> upstream/android-13
			p += sprintf(p, ",%2u.%1uM,", r / 10, r % 10);
		}

		p += sprintf(p, "%u,", idx);
<<<<<<< HEAD
		tx_time = DIV_ROUND_CLOSEST(mg->duration[j], 1000);
		p += sprintf(p, "%u,", tx_time);

		tp_max = minstrel_ht_get_tp_avg(mi, i, j, MINSTREL_FRAC(100, 100));
		tp_avg = minstrel_ht_get_tp_avg(mi, i, j, mrs->prob_ewma);
		eprob = MINSTREL_TRUNC(mrs->prob_ewma * 1000);
		prob_ewmsd = minstrel_get_ewmsd10(mrs);

		p += sprintf(p, "%u.%u,%u.%u,%u.%u,%u.%u,%u,%u,"
=======

		duration = mg->duration[j];
		duration <<= mg->shift;
		tx_time = DIV_ROUND_CLOSEST(duration, 1000);
		p += sprintf(p, "%u,", tx_time);

		tp_max = minstrel_ht_get_tp_avg(mi, i, j, MINSTREL_FRAC(100, 100));
		tp_avg = minstrel_ht_get_tp_avg(mi, i, j, mrs->prob_avg);
		eprob = MINSTREL_TRUNC(mrs->prob_avg * 1000);

		p += sprintf(p, "%u.%u,%u.%u,%u.%u,%u,%u,"
>>>>>>> upstream/android-13
				"%u,%llu,%llu,",
				tp_max / 10, tp_max % 10,
				tp_avg / 10, tp_avg % 10,
				eprob / 10, eprob % 10,
<<<<<<< HEAD
				prob_ewmsd / 10, prob_ewmsd % 10,
=======
>>>>>>> upstream/android-13
				mrs->retry_count,
				mrs->last_success,
				mrs->last_attempts,
				(unsigned long long)mrs->succ_hist,
				(unsigned long long)mrs->att_hist);
		p += sprintf(p, "%d,%d,%d.%d\n",
				max(0, (int) mi->total_packets -
				(int) mi->sample_packets),
				mi->sample_packets,
				MINSTREL_TRUNC(mi->avg_ampdu_len),
				MINSTREL_TRUNC(mi->avg_ampdu_len * 10) % 10);
	}

	return p;
}

static int
minstrel_ht_stats_csv_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct minstrel_ht_sta_priv *msp = inode->i_private;
	struct minstrel_ht_sta *mi = &msp->ht;
	struct minstrel_debugfs_info *ms;
	unsigned int i;
	int ret;
	char *p;

	if (!msp->is_ht) {
		inode->i_private = &msp->legacy;
		ret = minstrel_stats_csv_open(inode, file);
		inode->i_private = msp;
		return ret;
	}

	ms = kmalloc(32768, GFP_KERNEL);

=======
	struct minstrel_ht_sta *mi = inode->i_private;
	struct minstrel_debugfs_info *ms;
	unsigned int i;
	char *p;

	ms = kmalloc(32768, GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!ms)
		return -ENOMEM;

	file->private_data = ms;

	p = ms->buf;

	p = minstrel_ht_stats_csv_dump(mi, MINSTREL_CCK_GROUP, p);
	for (i = 0; i < MINSTREL_CCK_GROUP; i++)
		p = minstrel_ht_stats_csv_dump(mi, i, p);
	for (i++; i < ARRAY_SIZE(mi->groups); i++)
		p = minstrel_ht_stats_csv_dump(mi, i, p);

	ms->len = p - ms->buf;
	WARN_ON(ms->len + sizeof(*ms) > 32768);

	return nonseekable_open(inode, file);
}

static const struct file_operations minstrel_ht_stat_csv_fops = {
	.owner = THIS_MODULE,
	.open = minstrel_ht_stats_csv_open,
	.read = minstrel_stats_read,
	.release = minstrel_stats_release,
	.llseek = no_llseek,
};

void
minstrel_ht_add_sta_debugfs(void *priv, void *priv_sta, struct dentry *dir)
{
<<<<<<< HEAD
	struct minstrel_ht_sta_priv *msp = priv_sta;

	msp->dbg_stats = debugfs_create_file("rc_stats", 0444, dir, msp,
					     &minstrel_ht_stat_fops);
	msp->dbg_stats_csv = debugfs_create_file("rc_stats_csv", 0444, dir, msp,
						 &minstrel_ht_stat_csv_fops);
}

void
minstrel_ht_remove_sta_debugfs(void *priv, void *priv_sta)
{
	struct minstrel_ht_sta_priv *msp = priv_sta;

	debugfs_remove(msp->dbg_stats);
	debugfs_remove(msp->dbg_stats_csv);
=======
	debugfs_create_file("rc_stats", 0444, dir, priv_sta,
			    &minstrel_ht_stat_fops);
	debugfs_create_file("rc_stats_csv", 0444, dir, priv_sta,
			    &minstrel_ht_stat_csv_fops);
>>>>>>> upstream/android-13
}
