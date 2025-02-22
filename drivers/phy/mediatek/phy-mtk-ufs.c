// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2019 MediaTek Inc.
 * Author: Stanley Chu <stanley.chu@mediatek.com>
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>

/* mphy register and offsets */
#define MP_GLB_DIG_8C               0x008C
#define FRC_PLL_ISO_EN              BIT(8)
#define PLL_ISO_EN                  BIT(9)
#define FRC_FRC_PWR_ON              BIT(10)
#define PLL_PWR_ON                  BIT(11)

#define MP_LN_DIG_RX_9C             0xA09C
#define FSM_DIFZ_FRC                BIT(18)

#define MP_LN_DIG_RX_AC             0xA0AC
#define FRC_RX_SQ_EN                BIT(0)
#define RX_SQ_EN                    BIT(1)

#define MP_LN_RX_44                 0xB044
#define FRC_CDR_PWR_ON              BIT(17)
#define CDR_PWR_ON                  BIT(18)
#define FRC_CDR_ISO_EN              BIT(19)
#define CDR_ISO_EN                  BIT(20)

<<<<<<< HEAD
struct ufs_mtk_phy {
	struct device *dev;
	void __iomem *mmio;
	struct clk *mp_clk;
	struct clk *unipro_clk;
=======
#define UFSPHY_CLKS_CNT    2

struct ufs_mtk_phy {
	struct device *dev;
	void __iomem *mmio;
	struct clk_bulk_data clks[UFSPHY_CLKS_CNT];
>>>>>>> upstream/android-13
};

static inline u32 mphy_readl(struct ufs_mtk_phy *phy, u32 reg)
{
	return readl(phy->mmio + reg);
}

static inline void mphy_writel(struct ufs_mtk_phy *phy, u32 val, u32 reg)
{
	writel(val, phy->mmio + reg);
}

static void mphy_set_bit(struct ufs_mtk_phy *phy, u32 reg, u32 bit)
{
	u32 val;

	val = mphy_readl(phy, reg);
	val |= bit;
	mphy_writel(phy, val, reg);
}

static void mphy_clr_bit(struct ufs_mtk_phy *phy, u32 reg, u32 bit)
{
	u32 val;

	val = mphy_readl(phy, reg);
	val &= ~bit;
	mphy_writel(phy, val, reg);
}

static struct ufs_mtk_phy *get_ufs_mtk_phy(struct phy *generic_phy)
{
<<<<<<< HEAD
	return (struct ufs_mtk_phy *) phy_get_drvdata(generic_phy);
=======
	return (struct ufs_mtk_phy *)phy_get_drvdata(generic_phy);
>>>>>>> upstream/android-13
}

static int ufs_mtk_phy_clk_init(struct ufs_mtk_phy *phy)
{
	struct device *dev = phy->dev;
<<<<<<< HEAD

	phy->unipro_clk = devm_clk_get(dev, "unipro");
	if (IS_ERR(phy->unipro_clk)) {
		dev_info(dev, "unipro clock is not found, ignored.");
		phy->unipro_clk = NULL;
	}

	phy->mp_clk = devm_clk_get(dev, "mp");
	if (IS_ERR(phy->mp_clk)) {
		dev_info(dev, "mp clock is not found, ignored.");
		phy->mp_clk = NULL;
	}

	return 0;
=======
	struct clk_bulk_data *clks = phy->clks;

	clks[0].id = "unipro";
	clks[1].id = "mp";
	return devm_clk_bulk_get(dev, UFSPHY_CLKS_CNT, clks);
>>>>>>> upstream/android-13
}

static void ufs_mtk_phy_set_active(struct ufs_mtk_phy *phy)
{
	/* release DA_MP_PLL_PWR_ON */
	mphy_set_bit(phy, MP_GLB_DIG_8C, PLL_PWR_ON);
	mphy_clr_bit(phy, MP_GLB_DIG_8C, FRC_FRC_PWR_ON);

	/* release DA_MP_PLL_ISO_EN */
	mphy_clr_bit(phy, MP_GLB_DIG_8C, PLL_ISO_EN);
	mphy_clr_bit(phy, MP_GLB_DIG_8C, FRC_PLL_ISO_EN);

	/* release DA_MP_CDR_PWR_ON */
	mphy_set_bit(phy, MP_LN_RX_44, CDR_PWR_ON);
	mphy_clr_bit(phy, MP_LN_RX_44, FRC_CDR_PWR_ON);

	/* release DA_MP_CDR_ISO_EN */
	mphy_clr_bit(phy, MP_LN_RX_44, CDR_ISO_EN);
	mphy_clr_bit(phy, MP_LN_RX_44, FRC_CDR_ISO_EN);

	/* release DA_MP_RX0_SQ_EN */
	mphy_set_bit(phy, MP_LN_DIG_RX_AC, RX_SQ_EN);
	mphy_clr_bit(phy, MP_LN_DIG_RX_AC, FRC_RX_SQ_EN);

	/* delay 1us to wait DIFZ stable */
	udelay(1);

	/* release DIFZ */
	mphy_clr_bit(phy, MP_LN_DIG_RX_9C, FSM_DIFZ_FRC);
}

static void ufs_mtk_phy_set_deep_hibern(struct ufs_mtk_phy *phy)
{
	/* force DIFZ */
	mphy_set_bit(phy, MP_LN_DIG_RX_9C, FSM_DIFZ_FRC);

	/* force DA_MP_RX0_SQ_EN */
	mphy_set_bit(phy, MP_LN_DIG_RX_AC, FRC_RX_SQ_EN);
	mphy_clr_bit(phy, MP_LN_DIG_RX_AC, RX_SQ_EN);

	/* force DA_MP_CDR_ISO_EN */
	mphy_set_bit(phy, MP_LN_RX_44, FRC_CDR_ISO_EN);
	mphy_set_bit(phy, MP_LN_RX_44, CDR_ISO_EN);

	/* force DA_MP_CDR_PWR_ON */
	mphy_set_bit(phy, MP_LN_RX_44, FRC_CDR_PWR_ON);
	mphy_clr_bit(phy, MP_LN_RX_44, CDR_PWR_ON);

	/* force DA_MP_PLL_ISO_EN */
	mphy_set_bit(phy, MP_GLB_DIG_8C, FRC_PLL_ISO_EN);
	mphy_set_bit(phy, MP_GLB_DIG_8C, PLL_ISO_EN);

	/* force DA_MP_PLL_PWR_ON */
	mphy_set_bit(phy, MP_GLB_DIG_8C, FRC_FRC_PWR_ON);
	mphy_clr_bit(phy, MP_GLB_DIG_8C, PLL_PWR_ON);
}

<<<<<<< HEAD
#define ufs_mtk_phy_clk_prepare_enable(clk) \
	clk ? clk_prepare_enable(clk) : 0

#define ufs_mtk_phy_clk_disable_unprepare(clk) \
	clk ? clk_disable_unprepare(clk) : 0

=======
>>>>>>> upstream/android-13
static int ufs_mtk_phy_power_on(struct phy *generic_phy)
{
	struct ufs_mtk_phy *phy = get_ufs_mtk_phy(generic_phy);
	int ret;

<<<<<<< HEAD
	ret = ufs_mtk_phy_clk_prepare_enable(phy->unipro_clk);
	if (ret) {
		dev_err(phy->dev, "unipro_clk enable failed %d\n", ret);
		goto out;
	}

	ret = ufs_mtk_phy_clk_prepare_enable(phy->mp_clk);
	if (ret) {
		dev_err(phy->dev, "mp_clk enable failed %d\n", ret);
		goto out_unprepare_unipro_clk;
	}
=======
	ret = clk_bulk_prepare_enable(UFSPHY_CLKS_CNT, phy->clks);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	ufs_mtk_phy_set_active(phy);

	return 0;
<<<<<<< HEAD

out_unprepare_unipro_clk:
	ufs_mtk_phy_clk_disable_unprepare(phy->unipro_clk);
out:
	return ret;
=======
>>>>>>> upstream/android-13
}

static int ufs_mtk_phy_power_off(struct phy *generic_phy)
{
	struct ufs_mtk_phy *phy = get_ufs_mtk_phy(generic_phy);

	ufs_mtk_phy_set_deep_hibern(phy);

<<<<<<< HEAD
	ufs_mtk_phy_clk_disable_unprepare(phy->unipro_clk);
	ufs_mtk_phy_clk_disable_unprepare(phy->mp_clk);
=======
	clk_bulk_disable_unprepare(UFSPHY_CLKS_CNT, phy->clks);
>>>>>>> upstream/android-13

	return 0;
}

static const struct phy_ops ufs_mtk_phy_ops = {
	.power_on       = ufs_mtk_phy_power_on,
	.power_off      = ufs_mtk_phy_power_off,
	.owner          = THIS_MODULE,
};

static int ufs_mtk_phy_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct phy *generic_phy;
	struct phy_provider *phy_provider;
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	struct ufs_mtk_phy *phy;
	int ret;

	phy = devm_kzalloc(dev, sizeof(*phy), GFP_KERNEL);
	if (!phy)
		return -ENOMEM;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	phy->mmio = devm_ioremap_resource(dev, res);
=======
	phy->mmio = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(phy->mmio))
		return PTR_ERR(phy->mmio);

	phy->dev = dev;

	ret = ufs_mtk_phy_clk_init(phy);
<<<<<<< HEAD
	if (ret) {
		dev_err(dev, "%s mtk phy clk init fail: %d\n", __func__, ret);
		return ret;
	}

	generic_phy = devm_phy_create(dev, NULL, &ufs_mtk_phy_ops);
	if (IS_ERR(generic_phy)) {
		dev_err(dev, "%s mtk phy clk create fail: %d\n", __func__, PTR_ERR(generic_phy));
		return PTR_ERR(generic_phy);
	}
=======
	if (ret)
		return ret;

	generic_phy = devm_phy_create(dev, NULL, &ufs_mtk_phy_ops);
	if (IS_ERR(generic_phy))
		return PTR_ERR(generic_phy);
>>>>>>> upstream/android-13

	phy_set_drvdata(generic_phy, phy);

	phy_provider = devm_of_phy_provider_register(dev, of_phy_simple_xlate);
<<<<<<< HEAD
	if (IS_ERR(phy_provider)) {
		dev_err(dev, "%s mtk phy clk register fail: %d\n", __func__, PTR_ERR(phy_provider));
	}
=======

>>>>>>> upstream/android-13
	return PTR_ERR_OR_ZERO(phy_provider);
}

static const struct of_device_id ufs_mtk_phy_of_match[] = {
	{.compatible = "mediatek,mt8183-ufsphy"},
	{},
};
MODULE_DEVICE_TABLE(of, ufs_mtk_phy_of_match);

static struct platform_driver ufs_mtk_phy_driver = {
	.probe = ufs_mtk_phy_probe,
	.driver = {
		.of_match_table = ufs_mtk_phy_of_match,
		.name = "ufs_mtk_phy",
	},
};
module_platform_driver(ufs_mtk_phy_driver);

MODULE_DESCRIPTION("Universal Flash Storage (UFS) MediaTek MPHY");
<<<<<<< HEAD
MODULE_AUTHOR("Stanley Chu <stanley.chu@medaitek.com>");
=======
MODULE_AUTHOR("Stanley Chu <stanley.chu@mediatek.com>");
>>>>>>> upstream/android-13
MODULE_LICENSE("GPL v2");
