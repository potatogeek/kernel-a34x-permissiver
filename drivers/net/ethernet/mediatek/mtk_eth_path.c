// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Sean Wang <sean.wang@mediatek.com>
=======
// Copyright (c) 2018-2019 MediaTek Inc.

/* A library for configuring path from GMAC/GDM to target PHY
 *
 * Author: Sean Wang <sean.wang@mediatek.com>
 *
>>>>>>> upstream/android-13
 */

#include <linux/phy.h>
#include <linux/regmap.h>

#include "mtk_eth_soc.h"

struct mtk_eth_muxc {
<<<<<<< HEAD
	int (*set_path)(struct mtk_eth *eth, int path);
};

static const char * const mtk_eth_mux_name[] = {
	"mux_gdm1_to_gmac1_esw", "mux_gmac2_gmac0_to_gephy",
	"mux_u3_gmac2_to_qphy", "mux_gmac1_gmac2_to_sgmii_rgmii",
	"mux_gmac12_to_gephy_sgmii",
};

static const char * const mtk_eth_path_name[] = {
	"gmac1_rgmii", "gmac1_trgmii", "gmac1_sgmii", "gmac2_rgmii",
	"gmac2_sgmii", "gmac2_gephy", "gdm1_esw",
};

static int set_mux_gdm1_to_gmac1_esw(struct mtk_eth *eth, int path)
{
	u32 val, mask, set;
	bool updated = true;
=======
	const char	*name;
	int		cap_bit;
	int		(*set_path)(struct mtk_eth *eth, int path);
};

static const char *mtk_eth_path_name(int path)
{
	switch (path) {
	case MTK_ETH_PATH_GMAC1_RGMII:
		return "gmac1_rgmii";
	case MTK_ETH_PATH_GMAC1_TRGMII:
		return "gmac1_trgmii";
	case MTK_ETH_PATH_GMAC1_SGMII:
		return "gmac1_sgmii";
	case MTK_ETH_PATH_GMAC2_RGMII:
		return "gmac2_rgmii";
	case MTK_ETH_PATH_GMAC2_SGMII:
		return "gmac2_sgmii";
	case MTK_ETH_PATH_GMAC2_GEPHY:
		return "gmac2_gephy";
	case MTK_ETH_PATH_GDM1_ESW:
		return "gdm1_esw";
	default:
		return "unknown path";
	}
}

static int set_mux_gdm1_to_gmac1_esw(struct mtk_eth *eth, int path)
{
	bool updated = true;
	u32 val, mask, set;
>>>>>>> upstream/android-13

	switch (path) {
	case MTK_ETH_PATH_GMAC1_SGMII:
		mask = ~(u32)MTK_MUX_TO_ESW;
		set = 0;
		break;
	case MTK_ETH_PATH_GDM1_ESW:
		mask = ~(u32)MTK_MUX_TO_ESW;
		set = MTK_MUX_TO_ESW;
		break;
	default:
		updated = false;
		break;
<<<<<<< HEAD
	};
=======
	}
>>>>>>> upstream/android-13

	if (updated) {
		val = mtk_r32(eth, MTK_MAC_MISC);
		val = (val & mask) | set;
		mtk_w32(eth, val, MTK_MAC_MISC);
	}

<<<<<<< HEAD
	dev_info(eth->dev, "path %s in %s updated = %d\n",
		 mtk_eth_path_name[path], __func__, updated);
=======
	dev_dbg(eth->dev, "path %s in %s updated = %d\n",
		mtk_eth_path_name(path), __func__, updated);
>>>>>>> upstream/android-13

	return 0;
}

static int set_mux_gmac2_gmac0_to_gephy(struct mtk_eth *eth, int path)
{
	unsigned int val = 0;
	bool updated = true;

	switch (path) {
	case MTK_ETH_PATH_GMAC2_GEPHY:
		val = ~(u32)GEPHY_MAC_SEL;
		break;
	default:
		updated = false;
		break;
	}

	if (updated)
		regmap_update_bits(eth->infra, INFRA_MISC2, GEPHY_MAC_SEL, val);

<<<<<<< HEAD
	dev_info(eth->dev, "path %s in %s updated = %d\n",
		 mtk_eth_path_name[path], __func__, updated);
=======
	dev_dbg(eth->dev, "path %s in %s updated = %d\n",
		mtk_eth_path_name(path), __func__, updated);
>>>>>>> upstream/android-13

	return 0;
}

static int set_mux_u3_gmac2_to_qphy(struct mtk_eth *eth, int path)
{
	unsigned int val = 0;
	bool updated = true;

	switch (path) {
	case MTK_ETH_PATH_GMAC2_SGMII:
		val = CO_QPHY_SEL;
		break;
	default:
		updated = false;
		break;
	}

	if (updated)
		regmap_update_bits(eth->infra, INFRA_MISC2, CO_QPHY_SEL, val);

<<<<<<< HEAD
	dev_info(eth->dev, "path %s in %s updated = %d\n",
		 mtk_eth_path_name[path], __func__, updated);
=======
	dev_dbg(eth->dev, "path %s in %s updated = %d\n",
		mtk_eth_path_name(path), __func__, updated);
>>>>>>> upstream/android-13

	return 0;
}

static int set_mux_gmac1_gmac2_to_sgmii_rgmii(struct mtk_eth *eth, int path)
{
	unsigned int val = 0;
	bool updated = true;

	switch (path) {
	case MTK_ETH_PATH_GMAC1_SGMII:
		val = SYSCFG0_SGMII_GMAC1;
		break;
	case MTK_ETH_PATH_GMAC2_SGMII:
		val = SYSCFG0_SGMII_GMAC2;
		break;
	case MTK_ETH_PATH_GMAC1_RGMII:
	case MTK_ETH_PATH_GMAC2_RGMII:
		regmap_read(eth->ethsys, ETHSYS_SYSCFG0, &val);
		val &= SYSCFG0_SGMII_MASK;

		if ((path == MTK_GMAC1_RGMII && val == SYSCFG0_SGMII_GMAC1) ||
		    (path == MTK_GMAC2_RGMII && val == SYSCFG0_SGMII_GMAC2))
			val = 0;
		else
			updated = false;
		break;
	default:
		updated = false;
		break;
<<<<<<< HEAD
	};
=======
	}
>>>>>>> upstream/android-13

	if (updated)
		regmap_update_bits(eth->ethsys, ETHSYS_SYSCFG0,
				   SYSCFG0_SGMII_MASK, val);

<<<<<<< HEAD
	dev_info(eth->dev, "path %s in %s updated = %d\n",
		 mtk_eth_path_name[path], __func__, updated);
=======
	dev_dbg(eth->dev, "path %s in %s updated = %d\n",
		mtk_eth_path_name(path), __func__, updated);
>>>>>>> upstream/android-13

	return 0;
}

static int set_mux_gmac12_to_gephy_sgmii(struct mtk_eth *eth, int path)
{
	unsigned int val = 0;
	bool updated = true;

	regmap_read(eth->ethsys, ETHSYS_SYSCFG0, &val);

	switch (path) {
	case MTK_ETH_PATH_GMAC1_SGMII:
		val |= SYSCFG0_SGMII_GMAC1_V2;
		break;
	case MTK_ETH_PATH_GMAC2_GEPHY:
		val &= ~(u32)SYSCFG0_SGMII_GMAC2_V2;
		break;
	case MTK_ETH_PATH_GMAC2_SGMII:
		val |= SYSCFG0_SGMII_GMAC2_V2;
		break;
	default:
		updated = false;
<<<<<<< HEAD
	};
=======
	}
>>>>>>> upstream/android-13

	if (updated)
		regmap_update_bits(eth->ethsys, ETHSYS_SYSCFG0,
				   SYSCFG0_SGMII_MASK, val);

<<<<<<< HEAD
	if (!updated)
		dev_info(eth->dev, "path %s no needs updatiion in %s\n",
			 mtk_eth_path_name[path], __func__);

	dev_info(eth->dev, "path %s in %s updated = %d\n",
		 mtk_eth_path_name[path], __func__, updated);
=======
	dev_dbg(eth->dev, "path %s in %s updated = %d\n",
		mtk_eth_path_name(path), __func__, updated);
>>>>>>> upstream/android-13

	return 0;
}

static const struct mtk_eth_muxc mtk_eth_muxc[] = {
<<<<<<< HEAD
	{ .set_path = set_mux_gdm1_to_gmac1_esw, },
	{ .set_path = set_mux_gmac2_gmac0_to_gephy, },
	{ .set_path = set_mux_u3_gmac2_to_qphy, },
	{ .set_path = set_mux_gmac1_gmac2_to_sgmii_rgmii, },
	{ .set_path = set_mux_gmac12_to_gephy_sgmii, }
=======
	{
		.name = "mux_gdm1_to_gmac1_esw",
		.cap_bit = MTK_ETH_MUX_GDM1_TO_GMAC1_ESW,
		.set_path = set_mux_gdm1_to_gmac1_esw,
	}, {
		.name = "mux_gmac2_gmac0_to_gephy",
		.cap_bit = MTK_ETH_MUX_GMAC2_GMAC0_TO_GEPHY,
		.set_path = set_mux_gmac2_gmac0_to_gephy,
	}, {
		.name = "mux_u3_gmac2_to_qphy",
		.cap_bit = MTK_ETH_MUX_U3_GMAC2_TO_QPHY,
		.set_path = set_mux_u3_gmac2_to_qphy,
	}, {
		.name = "mux_gmac1_gmac2_to_sgmii_rgmii",
		.cap_bit = MTK_ETH_MUX_GMAC1_GMAC2_TO_SGMII_RGMII,
		.set_path = set_mux_gmac1_gmac2_to_sgmii_rgmii,
	}, {
		.name = "mux_gmac12_to_gephy_sgmii",
		.cap_bit = MTK_ETH_MUX_GMAC12_TO_GEPHY_SGMII,
		.set_path = set_mux_gmac12_to_gephy_sgmii,
	},
>>>>>>> upstream/android-13
};

static int mtk_eth_mux_setup(struct mtk_eth *eth, int path)
{
	int i, err = 0;

<<<<<<< HEAD
	if (!MTK_HAS_CAPS(eth->soc->caps, MTK_PATH_BIT(path))) {
		dev_info(eth->dev, "path %s isn't support on the SoC\n",
			 mtk_eth_path_name[path]);
=======
	if (!MTK_HAS_CAPS(eth->soc->caps, path)) {
		dev_err(eth->dev, "path %s isn't support on the SoC\n",
			mtk_eth_path_name(path));
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	if (!MTK_HAS_CAPS(eth->soc->caps, MTK_MUX))
		return 0;

	/* Setup MUX in path fabric */
<<<<<<< HEAD
	for (i = 0; i < MTK_ETH_MUX_MAX; i++) {
		if (MTK_HAS_CAPS(eth->soc->caps, MTK_MUX_BIT(i))) {
=======
	for (i = 0; i < ARRAY_SIZE(mtk_eth_muxc); i++) {
		if (MTK_HAS_CAPS(eth->soc->caps, mtk_eth_muxc[i].cap_bit)) {
>>>>>>> upstream/android-13
			err = mtk_eth_muxc[i].set_path(eth, path);
			if (err)
				goto out;
		} else {
<<<<<<< HEAD
			dev_info(eth->dev, "mux %s isn't present on the SoC\n",
				 mtk_eth_mux_name[i]);
=======
			dev_dbg(eth->dev, "mux %s isn't present on the SoC\n",
				mtk_eth_muxc[i].name);
>>>>>>> upstream/android-13
		}
	}

out:
	return err;
}

<<<<<<< HEAD
static int mtk_gmac_sgmii_path_setup(struct mtk_eth *eth, int mac_id)
{
	unsigned int val = 0;
	int sid, err, path;
=======
int mtk_gmac_sgmii_path_setup(struct mtk_eth *eth, int mac_id)
{
	int path;
>>>>>>> upstream/android-13

	path = (mac_id == 0) ?  MTK_ETH_PATH_GMAC1_SGMII :
				MTK_ETH_PATH_GMAC2_SGMII;

	/* Setup proper MUXes along the path */
<<<<<<< HEAD
	err = mtk_eth_mux_setup(eth, path);
	if (err)
		return err;

	/* The path GMAC to SGMII will be enabled once the SGMIISYS is being
	 * setup done.
	 */
	regmap_read(eth->ethsys, ETHSYS_SYSCFG0, &val);

	regmap_update_bits(eth->ethsys, ETHSYS_SYSCFG0,
			   SYSCFG0_SGMII_MASK, ~(u32)SYSCFG0_SGMII_MASK);

	/* Decide how GMAC and SGMIISYS be mapped */
	sid = (MTK_HAS_CAPS(eth->soc->caps, MTK_SHARED_SGMII)) ? 0 : mac_id;

	/* Setup SGMIISYS with the determined property */
	if (MTK_HAS_FLAGS(eth->sgmii->flags[sid], MTK_SGMII_PHYSPEED_AN))
		err = mtk_sgmii_setup_mode_an(eth->sgmii, sid);
	else
		err = mtk_sgmii_setup_mode_force(eth->sgmii, sid);

	if (err)
		return err;

	regmap_update_bits(eth->ethsys, ETHSYS_SYSCFG0,
			   SYSCFG0_SGMII_MASK, val);

	return 0;
}

static int mtk_gmac_gephy_path_setup(struct mtk_eth *eth, int mac_id)
{
	int err, path = 0;
=======
	return mtk_eth_mux_setup(eth, path);
}

int mtk_gmac_gephy_path_setup(struct mtk_eth *eth, int mac_id)
{
	int path = 0;
>>>>>>> upstream/android-13

	if (mac_id == 1)
		path = MTK_ETH_PATH_GMAC2_GEPHY;

	if (!path)
		return -EINVAL;

	/* Setup proper MUXes along the path */
<<<<<<< HEAD
	err = mtk_eth_mux_setup(eth, path);
	if (err)
		return err;

	return 0;
}

static int mtk_gmac_rgmii_path_setup(struct mtk_eth *eth, int mac_id)
{
	int err, path;
=======
	return mtk_eth_mux_setup(eth, path);
}

int mtk_gmac_rgmii_path_setup(struct mtk_eth *eth, int mac_id)
{
	int path;
>>>>>>> upstream/android-13

	path = (mac_id == 0) ?  MTK_ETH_PATH_GMAC1_RGMII :
				MTK_ETH_PATH_GMAC2_RGMII;

	/* Setup proper MUXes along the path */
<<<<<<< HEAD
	err = mtk_eth_mux_setup(eth, path);
	if (err)
		return err;

	return 0;
}

int mtk_setup_hw_path(struct mtk_eth *eth, int mac_id, int phymode)
{
	int err;

	switch (phymode) {
	case PHY_INTERFACE_MODE_TRGMII:
	case PHY_INTERFACE_MODE_RGMII_TXID:
	case PHY_INTERFACE_MODE_RGMII_RXID:
	case PHY_INTERFACE_MODE_RGMII_ID:
	case PHY_INTERFACE_MODE_RGMII:
	case PHY_INTERFACE_MODE_MII:
	case PHY_INTERFACE_MODE_REVMII:
	case PHY_INTERFACE_MODE_RMII:
		if (MTK_HAS_CAPS(eth->soc->caps, MTK_RGMII)) {
			err = mtk_gmac_rgmii_path_setup(eth, mac_id);
			if (err)
				return err;
		}
		break;
	case PHY_INTERFACE_MODE_SGMII:
		if (MTK_HAS_CAPS(eth->soc->caps, MTK_SGMII)) {
			err = mtk_gmac_sgmii_path_setup(eth, mac_id);
			if (err)
				return err;
		}
		break;
	case PHY_INTERFACE_MODE_GMII:
		if (MTK_HAS_CAPS(eth->soc->caps, MTK_GEPHY)) {
			err = mtk_gmac_gephy_path_setup(eth, mac_id);
			if (err)
				return err;
		}
		break;
	default:
		break;
	}

	return 0;
}
=======
	return mtk_eth_mux_setup(eth, path);
}

>>>>>>> upstream/android-13
