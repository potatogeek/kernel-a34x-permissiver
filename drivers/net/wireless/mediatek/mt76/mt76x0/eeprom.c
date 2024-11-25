<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2014 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2015 Jakub Kicinski <kubakici@wp.pl>
 * Copyright (C) 2018 Stanislaw Gruszka <stf_xl@wp.pl>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

=======
 */

#include <linux/module.h>
>>>>>>> upstream/android-13
#include <linux/of.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/etherdevice.h>
#include <asm/unaligned.h>
#include "mt76x0.h"
#include "eeprom.h"
<<<<<<< HEAD

static bool
field_valid(u8 val)
{
	return val != 0xff;
}

static s8
field_validate(u8 val)
{
	if (!field_valid(val))
		return 0;

	return val;
}

static inline int
sign_extend(u32 val, unsigned int size)
{
	bool sign = val & BIT(size - 1);

	val &= BIT(size - 1) - 1;

	return sign ? val : -val;
}

static int
mt76x0_efuse_read(struct mt76x0_dev *dev, u16 addr, u8 *data,
		   enum mt76x0_eeprom_access_modes mode)
{
	u32 val;
	int i;

	val = mt76_rr(dev, MT_EFUSE_CTRL);
	val &= ~(MT_EFUSE_CTRL_AIN |
		 MT_EFUSE_CTRL_MODE);
	val |= FIELD_PREP(MT_EFUSE_CTRL_AIN, addr & ~0xf) |
	       FIELD_PREP(MT_EFUSE_CTRL_MODE, mode) |
	       MT_EFUSE_CTRL_KICK;
	mt76_wr(dev, MT_EFUSE_CTRL, val);

	if (!mt76_poll(dev, MT_EFUSE_CTRL, MT_EFUSE_CTRL_KICK, 0, 1000))
		return -ETIMEDOUT;

	val = mt76_rr(dev, MT_EFUSE_CTRL);
	if ((val & MT_EFUSE_CTRL_AOUT) == MT_EFUSE_CTRL_AOUT) {
		/* Parts of eeprom not in the usage map (0x80-0xc0,0xf0)
		 * will not return valid data but it's ok.
		 */
		memset(data, 0xff, 16);
		return 0;
	}

	for (i = 0; i < 4; i++) {
		val = mt76_rr(dev, MT_EFUSE_DATA(i));
		put_unaligned_le32(val, data + 4 * i);
	}

	return 0;
}

#define MT_MAP_READS	DIV_ROUND_UP(MT_EFUSE_USAGE_MAP_SIZE, 16)
static int
mt76x0_efuse_physical_size_check(struct mt76x0_dev *dev)
=======
#include "../mt76x02_phy.h"

#define MT_MAP_READS	DIV_ROUND_UP(MT_EFUSE_USAGE_MAP_SIZE, 16)
static int
mt76x0_efuse_physical_size_check(struct mt76x02_dev *dev)
>>>>>>> upstream/android-13
{
	u8 data[MT_MAP_READS * 16];
	int ret, i;
	u32 start = 0, end = 0, cnt_free;

<<<<<<< HEAD
	for (i = 0; i < MT_MAP_READS; i++) {
		ret = mt76x0_efuse_read(dev, MT_EE_USAGE_MAP_START + i * 16,
					 data + i * 16, MT_EE_PHYSICAL_READ);
		if (ret)
			return ret;
	}
=======
	ret = mt76x02_get_efuse_data(dev, MT_EE_USAGE_MAP_START, data,
				     sizeof(data), MT_EE_PHYSICAL_READ);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	for (i = 0; i < MT_EFUSE_USAGE_MAP_SIZE; i++)
		if (!data[i]) {
			if (!start)
				start = MT_EE_USAGE_MAP_START + i;
			end = MT_EE_USAGE_MAP_START + i;
		}
	cnt_free = end - start + 1;

	if (MT_EFUSE_USAGE_MAP_SIZE - cnt_free < 5) {
<<<<<<< HEAD
		dev_err(dev->mt76.dev, "Error: your device needs default EEPROM file and this driver doesn't support it!\n");
=======
		dev_err(dev->mt76.dev,
			"driver does not support default EEPROM\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static void
mt76x0_set_chip_cap(struct mt76x0_dev *dev, u8 *eeprom)
{
	enum mt76x2_board_type { BOARD_TYPE_2GHZ = 1, BOARD_TYPE_5GHZ = 2 };
	u16 nic_conf0 = get_unaligned_le16(eeprom + MT_EE_NIC_CONF_0);
	u16 nic_conf1 = get_unaligned_le16(eeprom + MT_EE_NIC_CONF_1);

	dev_dbg(dev->mt76.dev, "NIC_CONF0: %04x NIC_CONF1: %04x\n", nic_conf0, nic_conf1);

	switch (FIELD_GET(MT_EE_NIC_CONF_0_BOARD_TYPE, nic_conf0)) {
	case BOARD_TYPE_5GHZ:
		dev->ee->has_5ghz = true;
		break;
	case BOARD_TYPE_2GHZ:
		dev->ee->has_2ghz = true;
		break;
	default:
		dev->ee->has_2ghz = true;
		dev->ee->has_5ghz = true;
		break;
	}

	dev_dbg(dev->mt76.dev, "Has 2GHZ %d 5GHZ %d\n", dev->ee->has_2ghz, dev->ee->has_5ghz);

	if (!field_valid(nic_conf1 & 0xff))
		nic_conf1 &= 0xff00;

	if (nic_conf1 & MT_EE_NIC_CONF_1_HW_RF_CTRL)
		dev_err(dev->mt76.dev,
			"Error: this driver does not support HW RF ctrl\n");

	if (!field_valid(nic_conf0 >> 8))
=======
static void mt76x0_set_chip_cap(struct mt76x02_dev *dev)
{
	u16 nic_conf0 = mt76x02_eeprom_get(dev, MT_EE_NIC_CONF_0);
	u16 nic_conf1 = mt76x02_eeprom_get(dev, MT_EE_NIC_CONF_1);

	mt76x02_eeprom_parse_hw_cap(dev);
	dev_dbg(dev->mt76.dev, "2GHz %d 5GHz %d\n",
		dev->mphy.cap.has_2ghz, dev->mphy.cap.has_5ghz);

	if (dev->no_2ghz) {
		dev->mphy.cap.has_2ghz = false;
		dev_dbg(dev->mt76.dev, "mask out 2GHz support\n");
	}

	if (is_mt7630(dev)) {
		dev->mphy.cap.has_5ghz = false;
		dev_dbg(dev->mt76.dev, "mask out 5GHz support\n");
	}

	if (!mt76x02_field_valid(nic_conf1 & 0xff))
		nic_conf1 &= 0xff00;

	if (nic_conf1 & MT_EE_NIC_CONF_1_HW_RF_CTRL)
		dev_dbg(dev->mt76.dev,
			"driver does not support HW RF ctrl\n");

	if (!mt76x02_field_valid(nic_conf0 >> 8))
>>>>>>> upstream/android-13
		return;

	if (FIELD_GET(MT_EE_NIC_CONF_0_RX_PATH, nic_conf0) > 1 ||
	    FIELD_GET(MT_EE_NIC_CONF_0_TX_PATH, nic_conf0) > 1)
<<<<<<< HEAD
		dev_err(dev->mt76.dev,
			"Error: device has more than 1 RX/TX stream!\n");

	dev->ee->pa_type = FIELD_GET(MT_EE_NIC_CONF_0_PA_TYPE, nic_conf0);
	dev_dbg(dev->mt76.dev, "PA Type %d\n", dev->ee->pa_type);
}

static int
mt76x0_set_macaddr(struct mt76x0_dev *dev, const u8 *eeprom)
{
	const void *src = eeprom + MT_EE_MAC_ADDR;

	ether_addr_copy(dev->macaddr, src);

	if (!is_valid_ether_addr(dev->macaddr)) {
		eth_random_addr(dev->macaddr);
		dev_info(dev->mt76.dev,
			 "Invalid MAC address, using random address %pM\n",
			 dev->macaddr);
	}

	mt76_wr(dev, MT_MAC_ADDR_DW0, get_unaligned_le32(dev->macaddr));
	mt76_wr(dev, MT_MAC_ADDR_DW1, get_unaligned_le16(dev->macaddr + 4) |
		FIELD_PREP(MT_MAC_ADDR_DW1_U2ME_MASK, 0xff));
=======
		dev_err(dev->mt76.dev, "invalid tx-rx stream\n");
}

static void mt76x0_set_temp_offset(struct mt76x02_dev *dev)
{
	u8 val;

	val = mt76x02_eeprom_get(dev, MT_EE_2G_TARGET_POWER) >> 8;
	if (mt76x02_field_valid(val))
		dev->cal.rx.temp_offset = mt76x02_sign_extend(val, 8);
	else
		dev->cal.rx.temp_offset = -10;
}

static void mt76x0_set_freq_offset(struct mt76x02_dev *dev)
{
	struct mt76x02_rx_freq_cal *caldata = &dev->cal.rx;
	u8 val;

	val = mt76x02_eeprom_get(dev, MT_EE_FREQ_OFFSET);
	if (!mt76x02_field_valid(val))
		val = 0;
	caldata->freq_offset = val;

	val = mt76x02_eeprom_get(dev, MT_EE_TSSI_BOUND4) >> 8;
	if (!mt76x02_field_valid(val))
		val = 0;

	caldata->freq_offset -= mt76x02_sign_extend(val, 8);
}

void mt76x0_read_rx_gain(struct mt76x02_dev *dev)
{
	struct ieee80211_channel *chan = dev->mphy.chandef.chan;
	struct mt76x02_rx_freq_cal *caldata = &dev->cal.rx;
	s8 val, lna_5g[3], lna_2g;
	u16 rssi_offset;
	int i;

	mt76x02_get_rx_gain(dev, chan->band, &rssi_offset, &lna_2g, lna_5g);
	caldata->lna_gain = mt76x02_get_lna_gain(dev, &lna_2g, lna_5g, chan);

	for (i = 0; i < ARRAY_SIZE(caldata->rssi_offset); i++) {
		val = rssi_offset >> (8 * i);
		if (val < -10 || val > 10)
			val = 0;

		caldata->rssi_offset[i] = val;
	}
}

static s8 mt76x0_get_delta(struct mt76x02_dev *dev)
{
	struct cfg80211_chan_def *chandef = &dev->mphy.chandef;
	u8 val;

	if (chandef->width == NL80211_CHAN_WIDTH_80) {
		val = mt76x02_eeprom_get(dev, MT_EE_5G_TARGET_POWER) >> 8;
	} else if (chandef->width == NL80211_CHAN_WIDTH_40) {
		u16 data;

		data = mt76x02_eeprom_get(dev, MT_EE_TX_POWER_DELTA_BW40);
		if (chandef->chan->band == NL80211_BAND_5GHZ)
			val = data >> 8;
		else
			val = data;
	} else {
		return 0;
	}

	return mt76x02_rate_power_val(val);
}

void mt76x0_get_tx_power_per_rate(struct mt76x02_dev *dev,
				  struct ieee80211_channel *chan,
				  struct mt76_rate_power *t)
{
	bool is_2ghz = chan->band == NL80211_BAND_2GHZ;
	u16 val, addr;
	s8 delta;

	memset(t, 0, sizeof(*t));

	/* cck 1M, 2M, 5.5M, 11M */
	val = mt76x02_eeprom_get(dev, MT_EE_TX_POWER_BYRATE_BASE);
	t->cck[0] = t->cck[1] = s6_to_s8(val);
	t->cck[2] = t->cck[3] = s6_to_s8(val >> 8);

	/* ofdm 6M, 9M, 12M, 18M */
	addr = is_2ghz ? MT_EE_TX_POWER_BYRATE_BASE + 2 : 0x120;
	val = mt76x02_eeprom_get(dev, addr);
	t->ofdm[0] = t->ofdm[1] = s6_to_s8(val);
	t->ofdm[2] = t->ofdm[3] = s6_to_s8(val >> 8);

	/* ofdm 24M, 36M, 48M, 54M */
	addr = is_2ghz ? MT_EE_TX_POWER_BYRATE_BASE + 4 : 0x122;
	val = mt76x02_eeprom_get(dev, addr);
	t->ofdm[4] = t->ofdm[5] = s6_to_s8(val);
	t->ofdm[6] = t->ofdm[7] = s6_to_s8(val >> 8);

	/* ht-vht mcs 1ss 0, 1, 2, 3 */
	addr = is_2ghz ? MT_EE_TX_POWER_BYRATE_BASE + 6 : 0x124;
	val = mt76x02_eeprom_get(dev, addr);
	t->ht[0] = t->ht[1] = t->vht[0] = t->vht[1] = s6_to_s8(val);
	t->ht[2] = t->ht[3] = t->vht[2] = t->vht[3] = s6_to_s8(val >> 8);

	/* ht-vht mcs 1ss 4, 5, 6 */
	addr = is_2ghz ? MT_EE_TX_POWER_BYRATE_BASE + 8 : 0x126;
	val = mt76x02_eeprom_get(dev, addr);
	t->ht[4] = t->ht[5] = t->vht[4] = t->vht[5] = s6_to_s8(val);
	t->ht[6] = t->ht[7] = t->vht[6] = t->vht[7] = s6_to_s8(val >> 8);

	/* ht-vht mcs 1ss 0, 1, 2, 3 stbc */
	addr = is_2ghz ? MT_EE_TX_POWER_BYRATE_BASE + 14 : 0xec;
	val = mt76x02_eeprom_get(dev, addr);
	t->stbc[0] = t->stbc[1] = s6_to_s8(val);
	t->stbc[2] = t->stbc[3] = s6_to_s8(val >> 8);

	/* ht-vht mcs 1ss 4, 5, 6 stbc */
	addr = is_2ghz ? MT_EE_TX_POWER_BYRATE_BASE + 16 : 0xee;
	val = mt76x02_eeprom_get(dev, addr);
	t->stbc[4] = t->stbc[5] = s6_to_s8(val);
	t->stbc[6] = t->stbc[7] = s6_to_s8(val >> 8);

	/* vht mcs 8, 9 5GHz */
	val = mt76x02_eeprom_get(dev, 0x132);
	t->vht[8] = s6_to_s8(val);
	t->vht[9] = s6_to_s8(val >> 8);

	delta = mt76x0_tssi_enabled(dev) ? 0 : mt76x0_get_delta(dev);
	mt76x02_add_rate_power_offset(t, delta);
}

void mt76x0_get_power_info(struct mt76x02_dev *dev,
			   struct ieee80211_channel *chan, s8 *tp)
{
	static const struct mt76x0_chan_map {
		u8 chan;
		u8 offset;
	} chan_map[] = {
		{   2,  0 }, {   4,  2 }, {   6,  4 }, {   8,  6 },
		{  10,  8 }, {  12, 10 }, {  14, 12 }, {  38,  0 },
		{  44,  2 }, {  48,  4 }, {  54,  6 }, {  60,  8 },
		{  64, 10 }, { 102, 12 }, { 108, 14 }, { 112, 16 },
		{ 118, 18 }, { 124, 20 }, { 128, 22 }, { 134, 24 },
		{ 140, 26 }, { 151, 28 }, { 157, 30 }, { 161, 32 },
		{ 167, 34 }, { 171, 36 }, { 175, 38 },
	};
	u8 offset, addr;
	int i, idx = 0;
	u16 data;

	if (mt76x0_tssi_enabled(dev)) {
		s8 target_power;

		if (chan->band == NL80211_BAND_5GHZ)
			data = mt76x02_eeprom_get(dev, MT_EE_5G_TARGET_POWER);
		else
			data = mt76x02_eeprom_get(dev, MT_EE_2G_TARGET_POWER);
		target_power = (data & 0xff) - dev->mt76.rate_power.ofdm[7];
		*tp = target_power + mt76x0_get_delta(dev);

		return;
	}

	for (i = 0; i < ARRAY_SIZE(chan_map); i++) {
		if (chan->hw_value <= chan_map[i].chan) {
			idx = (chan->hw_value == chan_map[i].chan);
			offset = chan_map[i].offset;
			break;
		}
	}
	if (i == ARRAY_SIZE(chan_map))
		offset = chan_map[0].offset;

	if (chan->band == NL80211_BAND_2GHZ) {
		addr = MT_EE_TX_POWER_DELTA_BW80 + offset;
	} else {
		switch (chan->hw_value) {
		case 42:
			offset = 2;
			break;
		case 58:
			offset = 8;
			break;
		case 106:
			offset = 14;
			break;
		case 122:
			offset = 20;
			break;
		case 155:
			offset = 30;
			break;
		default:
			break;
		}
		addr = MT_EE_TX_POWER_0_GRP4_TSSI_SLOPE + 2 + offset;
	}

	data = mt76x02_eeprom_get(dev, addr);
	*tp = data >> (8 * idx);
	if (*tp < 0 || *tp > 0x3f)
		*tp = 5;
}

static int mt76x0_check_eeprom(struct mt76x02_dev *dev)
{
	u16 val;

	val = get_unaligned_le16(dev->mt76.eeprom.data);
	if (!val)
		val = get_unaligned_le16(dev->mt76.eeprom.data +
					 MT_EE_PCI_ID);

	switch (val) {
	case 0x7650:
	case 0x7610:
		return 0;
	default:
		dev_err(dev->mt76.dev, "EEPROM data check failed: %04x\n",
			val);
		return -EINVAL;
	}
}

static int mt76x0_load_eeprom(struct mt76x02_dev *dev)
{
	int found;

	found = mt76_eeprom_init(&dev->mt76, MT76X0_EEPROM_SIZE);
	if (found < 0)
		return found;

	if (found && !mt76x0_check_eeprom(dev))
		return 0;

	found = mt76x0_efuse_physical_size_check(dev);
	if (found < 0)
		return found;

	return mt76x02_get_efuse_data(dev, 0, dev->mt76.eeprom.data,
				      MT76X0_EEPROM_SIZE, MT_EE_READ);
}

int mt76x0_eeprom_init(struct mt76x02_dev *dev)
{
	u8 version, fae;
	u16 data;
	int err;

	err = mt76x0_load_eeprom(dev);
	if (err < 0)
		return err;

	data = mt76x02_eeprom_get(dev, MT_EE_VERSION);
	version = data >> 8;
	fae = data;

	if (version > MT76X0U_EE_MAX_VER)
		dev_warn(dev->mt76.dev,
			 "Warning: unsupported EEPROM version %02hhx\n",
			 version);
	dev_info(dev->mt76.dev, "EEPROM ver:%02hhx fae:%02hhx\n",
		 version, fae);

	memcpy(dev->mphy.macaddr, (u8 *)dev->mt76.eeprom.data + MT_EE_MAC_ADDR,
	       ETH_ALEN);
	mt76_eeprom_override(&dev->mphy);
	mt76x02_mac_setaddr(dev, dev->mphy.macaddr);

	mt76x0_set_chip_cap(dev);
	mt76x0_set_freq_offset(dev);
	mt76x0_set_temp_offset(dev);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void
mt76x0_set_temp_offset(struct mt76x0_dev *dev, u8 *eeprom)
{
	u8 temp = eeprom[MT_EE_TEMP_OFFSET];

	if (field_valid(temp))
		dev->ee->temp_off = sign_extend(temp, 8);
	else
		dev->ee->temp_off = -10;
}

static void
mt76x0_set_country_reg(struct mt76x0_dev *dev, u8 *eeprom)
{
	/* Note: - region 31 is not valid for mt76x0 (see rtmp_init.c)
	 *	 - comments in rtmp_def.h are incorrect (see rt_channel.c)
	 */
	static const struct reg_channel_bounds chan_bounds[] = {
		/* EEPROM country regions 0 - 7 */
		{  1, 11 },	{  1, 13 },	{ 10,  2 },	{ 10,  4 },
		{ 14,  1 },	{  1, 14 },	{  3,  7 },	{  5,  9 },
		/* EEPROM country regions 32 - 33 */
		{  1, 11 },	{  1, 14 }
	};
	u8 val = eeprom[MT_EE_COUNTRY_REGION_2GHZ];
	int idx = -1;

	dev_dbg(dev->mt76.dev, "REG 2GHZ %u REG 5GHZ %u\n", val, eeprom[MT_EE_COUNTRY_REGION_5GHZ]);
	if (val < 8)
		idx = val;
	if (val > 31 && val < 33)
		idx = val - 32 + 8;

	if (idx != -1)
		dev_info(dev->mt76.dev,
			 "EEPROM country region %02hhx (channels %hhd-%hhd)\n",
			 val, chan_bounds[idx].start,
			 chan_bounds[idx].start + chan_bounds[idx].num - 1);
	else
		idx = 5; /* channels 1 - 14 */

	dev->ee->reg = chan_bounds[idx];

	/* TODO: country region 33 is special - phy should be set to B-mode
	 *	 before entering channel 14 (see sta/connect.c)
	 */
}

static void
mt76x0_set_rf_freq_off(struct mt76x0_dev *dev, u8 *eeprom)
{
	u8 comp;

	dev->ee->rf_freq_off = field_validate(eeprom[MT_EE_FREQ_OFFSET]);
	comp = field_validate(eeprom[MT_EE_FREQ_OFFSET_COMPENSATION]);

	if (comp & BIT(7))
		dev->ee->rf_freq_off -= comp & 0x7f;
	else
		dev->ee->rf_freq_off += comp;
}

static void
mt76x0_set_lna_gain(struct mt76x0_dev *dev, u8 *eeprom)
{
	u8 gain;

	dev->ee->lna_gain_2ghz = eeprom[MT_EE_LNA_GAIN_2GHZ];
	dev->ee->lna_gain_5ghz[0] = eeprom[MT_EE_LNA_GAIN_5GHZ_0];

	gain = eeprom[MT_EE_LNA_GAIN_5GHZ_1];
	if (gain == 0xff || gain == 0)
		dev->ee->lna_gain_5ghz[1] = dev->ee->lna_gain_5ghz[0];
	else
		dev->ee->lna_gain_5ghz[1] = gain;

	gain = eeprom[MT_EE_LNA_GAIN_5GHZ_2];
	if (gain == 0xff || gain == 0)
		dev->ee->lna_gain_5ghz[2] = dev->ee->lna_gain_5ghz[0];
	else
		dev->ee->lna_gain_5ghz[2] = gain;
}

static void
mt76x0_set_rssi_offset(struct mt76x0_dev *dev, u8 *eeprom)
{
	int i;
	s8 *rssi_offset = dev->ee->rssi_offset_2ghz;

	for (i = 0; i < 2; i++) {
		rssi_offset[i] = eeprom[MT_EE_RSSI_OFFSET + i];

		if (rssi_offset[i] < -10 || rssi_offset[i] > 10) {
			dev_warn(dev->mt76.dev,
				 "Warning: EEPROM RSSI is invalid %02hhx\n",
				 rssi_offset[i]);
			rssi_offset[i] = 0;
		}
	}

	rssi_offset = dev->ee->rssi_offset_5ghz;

	for (i = 0; i < 3; i++) {
		rssi_offset[i] = eeprom[MT_EE_RSSI_OFFSET_5GHZ + i];

		if (rssi_offset[i] < -10 || rssi_offset[i] > 10) {
			dev_warn(dev->mt76.dev,
				 "Warning: EEPROM RSSI is invalid %02hhx\n",
				 rssi_offset[i]);
			rssi_offset[i] = 0;
		}
	}
}

static u32
calc_bw40_power_rate(u32 value, int delta)
{
	u32 ret = 0;
	int i, tmp;

	for (i = 0; i < 4; i++) {
		tmp = s6_to_int((value >> i*8) & 0xff) + delta;
		ret |= (u32)(int_to_s6(tmp)) << i*8;
	}

	return ret;
}

static s8
get_delta(u8 val)
{
	s8 ret;

	if (!field_valid(val) || !(val & BIT(7)))
		return 0;

	ret = val & 0x1f;
	if (ret > 8)
		ret = 8;
	if (val & BIT(6))
		ret = -ret;

	return ret;
}

static void
mt76x0_set_tx_power_per_rate(struct mt76x0_dev *dev, u8 *eeprom)
{
	s8 bw40_delta_2g, bw40_delta_5g;
	u32 val;
	int i;

	bw40_delta_2g = get_delta(eeprom[MT_EE_TX_POWER_DELTA_BW40]);
	bw40_delta_5g = get_delta(eeprom[MT_EE_TX_POWER_DELTA_BW40 + 1]);

	for (i = 0; i < 5; i++) {
		val = get_unaligned_le32(eeprom + MT_EE_TX_POWER_BYRATE(i));

		/* Skip last 16 bits. */
		if (i == 4)
			val &= 0x0000ffff;

		dev->ee->tx_pwr_cfg_2g[i][0] = val;
		dev->ee->tx_pwr_cfg_2g[i][1] = calc_bw40_power_rate(val, bw40_delta_2g);
	}

	/* Reading per rate tx power for 5 GHz band is a bit more complex. Note
	 * we mix 16 bit and 32 bit reads and sometimes do shifts.
	 */
	val = get_unaligned_le16(eeprom + 0x120);
	val <<= 16;
	dev->ee->tx_pwr_cfg_5g[0][0] = val;
	dev->ee->tx_pwr_cfg_5g[0][1] = calc_bw40_power_rate(val, bw40_delta_5g);

	val = get_unaligned_le32(eeprom + 0x122);
	dev->ee->tx_pwr_cfg_5g[1][0] = val;
	dev->ee->tx_pwr_cfg_5g[1][1] = calc_bw40_power_rate(val, bw40_delta_5g);

	val = get_unaligned_le16(eeprom + 0x126);
	dev->ee->tx_pwr_cfg_5g[2][0] = val;
	dev->ee->tx_pwr_cfg_5g[2][1] = calc_bw40_power_rate(val, bw40_delta_5g);

	val = get_unaligned_le16(eeprom + 0xec);
	val <<= 16;
	dev->ee->tx_pwr_cfg_5g[3][0] = val;
	dev->ee->tx_pwr_cfg_5g[3][1] = calc_bw40_power_rate(val, bw40_delta_5g);

	val = get_unaligned_le16(eeprom + 0xee);
	dev->ee->tx_pwr_cfg_5g[4][0] = val;
	dev->ee->tx_pwr_cfg_5g[4][1] = calc_bw40_power_rate(val, bw40_delta_5g);
}

static void
mt76x0_set_tx_power_per_chan(struct mt76x0_dev *dev, u8 *eeprom)
{
	int i;
	u8 tx_pwr;

	for (i = 0; i < 14; i++) {
		tx_pwr = eeprom[MT_EE_TX_POWER_OFFSET_2GHZ + i];
		if (tx_pwr <= 0x3f && tx_pwr > 0)
			dev->ee->tx_pwr_per_chan[i] = tx_pwr;
		else
			dev->ee->tx_pwr_per_chan[i] = 5;
	}

	for (i = 0; i < 40; i++) {
		tx_pwr = eeprom[MT_EE_TX_POWER_OFFSET_5GHZ + i];
		if (tx_pwr <= 0x3f && tx_pwr > 0)
			dev->ee->tx_pwr_per_chan[14 + i] = tx_pwr;
		else
			dev->ee->tx_pwr_per_chan[14 + i] = 5;
	}

	dev->ee->tx_pwr_per_chan[54] = dev->ee->tx_pwr_per_chan[22];
	dev->ee->tx_pwr_per_chan[55] = dev->ee->tx_pwr_per_chan[28];
	dev->ee->tx_pwr_per_chan[56] = dev->ee->tx_pwr_per_chan[34];
	dev->ee->tx_pwr_per_chan[57] = dev->ee->tx_pwr_per_chan[44];
}

int
mt76x0_eeprom_init(struct mt76x0_dev *dev)
{
	u8 *eeprom;
	int i, ret;

	ret = mt76x0_efuse_physical_size_check(dev);
	if (ret)
		return ret;

	dev->ee = devm_kzalloc(dev->mt76.dev, sizeof(*dev->ee), GFP_KERNEL);
	if (!dev->ee)
		return -ENOMEM;

	eeprom = kmalloc(MT76X0_EEPROM_SIZE, GFP_KERNEL);
	if (!eeprom)
		return -ENOMEM;

	for (i = 0; i + 16 <= MT76X0_EEPROM_SIZE; i += 16) {
		ret = mt76x0_efuse_read(dev, i, eeprom + i, MT_EE_READ);
		if (ret)
			goto out;
	}

	if (eeprom[MT_EE_VERSION_EE] > MT76X0U_EE_MAX_VER)
		dev_warn(dev->mt76.dev,
			 "Warning: unsupported EEPROM version %02hhx\n",
			 eeprom[MT_EE_VERSION_EE]);
	dev_info(dev->mt76.dev, "EEPROM ver:%02hhx fae:%02hhx\n",
		 eeprom[MT_EE_VERSION_EE], eeprom[MT_EE_VERSION_FAE]);

	mt76x0_set_macaddr(dev, eeprom);
	mt76x0_set_chip_cap(dev, eeprom);
	mt76x0_set_country_reg(dev, eeprom);
	mt76x0_set_rf_freq_off(dev, eeprom);
	mt76x0_set_temp_offset(dev, eeprom);
	mt76x0_set_lna_gain(dev, eeprom);
	mt76x0_set_rssi_offset(dev, eeprom);
	dev->chainmask = 0x0101;

	mt76x0_set_tx_power_per_rate(dev, eeprom);
	mt76x0_set_tx_power_per_chan(dev, eeprom);

out:
	kfree(eeprom);
	return ret;
}
=======
MODULE_LICENSE("Dual BSD/GPL");
>>>>>>> upstream/android-13
