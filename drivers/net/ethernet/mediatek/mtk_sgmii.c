// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Sean Wang <sean.wang@mediatek.com>
=======
// Copyright (c) 2018-2019 MediaTek Inc.

/* A library for MediaTek SGMII circuit
 *
 * Author: Sean Wang <sean.wang@mediatek.com>
 *
>>>>>>> upstream/android-13
 */

#include <linux/mfd/syscon.h>
#include <linux/of.h>
#include <linux/regmap.h>

#include "mtk_eth_soc.h"

int mtk_sgmii_init(struct mtk_sgmii *ss, struct device_node *r, u32 ana_rgc3)
{
	struct device_node *np;
<<<<<<< HEAD
	const char *str;
	int i, err;
=======
	int i;
>>>>>>> upstream/android-13

	ss->ana_rgc3 = ana_rgc3;

	for (i = 0; i < MTK_MAX_DEVS; i++) {
		np = of_parse_phandle(r, "mediatek,sgmiisys", i);
		if (!np)
			break;

		ss->regmap[i] = syscon_node_to_regmap(np);
<<<<<<< HEAD
		if (IS_ERR(ss->regmap[i]))
			return PTR_ERR(ss->regmap[i]);

		err = of_property_read_string(np, "mediatek,physpeed", &str);
		if (err)
			return err;

		if (!strcmp(str, "2500"))
			ss->flags[i] |= MTK_SGMII_PHYSPEED_2500;
		else if (!strcmp(str, "1000"))
			ss->flags[i] |= MTK_SGMII_PHYSPEED_1000;
		else if (!strcmp(str, "auto"))
			ss->flags[i] |= MTK_SGMII_PHYSPEED_AN;
		else
			return -EINVAL;
=======
		of_node_put(np);
		if (IS_ERR(ss->regmap[i]))
			return PTR_ERR(ss->regmap[i]);
>>>>>>> upstream/android-13
	}

	return 0;
}

int mtk_sgmii_setup_mode_an(struct mtk_sgmii *ss, int id)
{
	unsigned int val;

	if (!ss->regmap[id])
		return -EINVAL;

	/* Setup the link timer and QPHY power up inside SGMIISYS */
	regmap_write(ss->regmap[id], SGMSYS_PCS_LINK_TIMER,
		     SGMII_LINK_TIMER_DEFAULT);

	regmap_read(ss->regmap[id], SGMSYS_SGMII_MODE, &val);
	val |= SGMII_REMOTE_FAULT_DIS;
	regmap_write(ss->regmap[id], SGMSYS_SGMII_MODE, val);

	regmap_read(ss->regmap[id], SGMSYS_PCS_CONTROL_1, &val);
	val |= SGMII_AN_RESTART;
	regmap_write(ss->regmap[id], SGMSYS_PCS_CONTROL_1, val);

	regmap_read(ss->regmap[id], SGMSYS_QPHY_PWR_STATE_CTRL, &val);
	val &= ~SGMII_PHYA_PWD;
	regmap_write(ss->regmap[id], SGMSYS_QPHY_PWR_STATE_CTRL, val);

	return 0;
}

<<<<<<< HEAD
int mtk_sgmii_setup_mode_force(struct mtk_sgmii *ss, int id)
{
	unsigned int val;
	int mode;
=======
int mtk_sgmii_setup_mode_force(struct mtk_sgmii *ss, int id,
			       const struct phylink_link_state *state)
{
	unsigned int val;
>>>>>>> upstream/android-13

	if (!ss->regmap[id])
		return -EINVAL;

	regmap_read(ss->regmap[id], ss->ana_rgc3, &val);
<<<<<<< HEAD
	val &= ~GENMASK(3, 2);
	mode = ss->flags[id] & MTK_SGMII_PHYSPEED_MASK;
	val |= (mode == MTK_SGMII_PHYSPEED_1000) ? 0 : BIT(2);
	regmap_write(ss->regmap[id], ss->ana_rgc3, val);

	/* disable SGMII AN */
	regmap_read(ss->regmap[id], SGMSYS_PCS_CONTROL_1, &val);
	val &= ~BIT(12);
	regmap_write(ss->regmap[id], SGMSYS_PCS_CONTROL_1, val);

	/* SGMII force mode setting */
	val = 0x31120019;
=======
	val &= ~RG_PHY_SPEED_MASK;
	if (state->interface == PHY_INTERFACE_MODE_2500BASEX)
		val |= RG_PHY_SPEED_3_125G;
	regmap_write(ss->regmap[id], ss->ana_rgc3, val);

	/* Disable SGMII AN */
	regmap_read(ss->regmap[id], SGMSYS_PCS_CONTROL_1, &val);
	val &= ~SGMII_AN_ENABLE;
	regmap_write(ss->regmap[id], SGMSYS_PCS_CONTROL_1, val);

	/* SGMII force mode setting */
	regmap_read(ss->regmap[id], SGMSYS_SGMII_MODE, &val);
	val &= ~SGMII_IF_MODE_MASK;

	switch (state->speed) {
	case SPEED_10:
		val |= SGMII_SPEED_10;
		break;
	case SPEED_100:
		val |= SGMII_SPEED_100;
		break;
	case SPEED_2500:
	case SPEED_1000:
		val |= SGMII_SPEED_1000;
		break;
	}

	if (state->duplex == DUPLEX_FULL)
		val |= SGMII_DUPLEX_FULL;

>>>>>>> upstream/android-13
	regmap_write(ss->regmap[id], SGMSYS_SGMII_MODE, val);

	/* Release PHYA power down state */
	regmap_read(ss->regmap[id], SGMSYS_QPHY_PWR_STATE_CTRL, &val);
	val &= ~SGMII_PHYA_PWD;
	regmap_write(ss->regmap[id], SGMSYS_QPHY_PWR_STATE_CTRL, val);

	return 0;
}
<<<<<<< HEAD
=======

void mtk_sgmii_restart_an(struct mtk_eth *eth, int mac_id)
{
	struct mtk_sgmii *ss = eth->sgmii;
	unsigned int val, sid;

	/* Decide how GMAC and SGMIISYS be mapped */
	sid = (MTK_HAS_CAPS(eth->soc->caps, MTK_SHARED_SGMII)) ?
	       0 : mac_id;

	if (!ss->regmap[sid])
		return;

	regmap_read(ss->regmap[sid], SGMSYS_PCS_CONTROL_1, &val);
	val |= SGMII_AN_RESTART;
	regmap_write(ss->regmap[sid], SGMSYS_PCS_CONTROL_1, val);
}
>>>>>>> upstream/android-13
