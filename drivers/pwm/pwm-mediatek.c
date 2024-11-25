<<<<<<< HEAD
/*
 * Mediatek Pulse Width Modulator driver
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * MediaTek Pulse Width Modulator driver
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2015 John Crispin <blogic@openwrt.org>
 * Copyright (C) 2017 Zhi Mao <zhi.mao@mediatek.com>
 *
<<<<<<< HEAD
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
=======
>>>>>>> upstream/android-13
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/types.h>

/* PWM registers and bits definitions */
#define PWMCON			0x00
#define PWMHDUR			0x04
#define PWMLDUR			0x08
#define PWMGDUR			0x0c
#define PWMWAVENUM		0x28
#define PWMDWIDTH		0x2c
#define PWM45DWIDTH_FIXUP	0x30
#define PWMTHRES		0x30
#define PWM45THRES_FIXUP	0x34
<<<<<<< HEAD

#define PWM_CLK_DIV_MAX		7

enum {
	MTK_CLK_MAIN = 0,
	MTK_CLK_TOP,
	MTK_CLK_PWM1,
	MTK_CLK_PWM2,
	MTK_CLK_PWM3,
	MTK_CLK_PWM4,
	MTK_CLK_PWM5,
	MTK_CLK_PWM6,
	MTK_CLK_PWM7,
	MTK_CLK_PWM8,
	MTK_CLK_MAX,
};

static const char * const mtk_pwm_clk_name[MTK_CLK_MAX] = {
	"main", "top", "pwm1", "pwm2", "pwm3", "pwm4", "pwm5", "pwm6", "pwm7",
	"pwm8"
};

struct mtk_pwm_platform_data {
	unsigned int num_pwms;
	bool pwm45_fixup;
	bool has_clks;
};

/**
 * struct mtk_pwm_chip - struct representing PWM chip
 * @chip: linux PWM chip representation
 * @regs: base address of PWM chip
 * @clks: list of clocks
 */
struct mtk_pwm_chip {
	struct pwm_chip chip;
	void __iomem *regs;
	struct clk *clks[MTK_CLK_MAX];
	const struct mtk_pwm_platform_data *soc;
};

static const unsigned int mtk_pwm_reg_offset[] = {
	0x0010, 0x0050, 0x0090, 0x00d0, 0x0110, 0x0150, 0x0190, 0x0220
};

static inline struct mtk_pwm_chip *to_mtk_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct mtk_pwm_chip, chip);
}

static int mtk_pwm_clk_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct mtk_pwm_chip *pc = to_mtk_pwm_chip(chip);
	int ret;

	if (!pc->soc->has_clks)
		return 0;

	ret = clk_prepare_enable(pc->clks[MTK_CLK_TOP]);
	if (ret < 0)
		return ret;

	ret = clk_prepare_enable(pc->clks[MTK_CLK_MAIN]);
	if (ret < 0)
		goto disable_clk_top;

	ret = clk_prepare_enable(pc->clks[MTK_CLK_PWM1 + pwm->hwpwm]);
=======
#define PWM_CK_26M_SEL		0x210

#define PWM_CLK_DIV_MAX		7

struct pwm_mediatek_of_data {
	unsigned int num_pwms;
	bool pwm45_fixup;
	bool has_ck_26m_sel;
};

/**
 * struct pwm_mediatek_chip - struct representing PWM chip
 * @chip: linux PWM chip representation
 * @regs: base address of PWM chip
 * @clk_top: the top clock generator
 * @clk_main: the clock used by PWM core
 * @clk_pwms: the clock used by each PWM channel
 * @clk_freq: the fix clock frequency of legacy MIPS SoC
 * @soc: pointer to chip's platform data
 */
struct pwm_mediatek_chip {
	struct pwm_chip chip;
	void __iomem *regs;
	struct clk *clk_top;
	struct clk *clk_main;
	struct clk **clk_pwms;
	const struct pwm_mediatek_of_data *soc;
};

static const unsigned int pwm_mediatek_reg_offset[] = {
	0x0010, 0x0050, 0x0090, 0x00d0, 0x0110, 0x0150, 0x0190, 0x0220
};

static inline struct pwm_mediatek_chip *
to_pwm_mediatek_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct pwm_mediatek_chip, chip);
}

static int pwm_mediatek_clk_enable(struct pwm_chip *chip,
				   struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
	int ret;

	ret = clk_prepare_enable(pc->clk_top);
	if (ret < 0)
		return ret;

	ret = clk_prepare_enable(pc->clk_main);
	if (ret < 0)
		goto disable_clk_top;

	ret = clk_prepare_enable(pc->clk_pwms[pwm->hwpwm]);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto disable_clk_main;

	return 0;

disable_clk_main:
<<<<<<< HEAD
	clk_disable_unprepare(pc->clks[MTK_CLK_MAIN]);
disable_clk_top:
	clk_disable_unprepare(pc->clks[MTK_CLK_TOP]);
=======
	clk_disable_unprepare(pc->clk_main);
disable_clk_top:
	clk_disable_unprepare(pc->clk_top);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void mtk_pwm_clk_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct mtk_pwm_chip *pc = to_mtk_pwm_chip(chip);

	if (!pc->soc->has_clks)
		return;

	clk_disable_unprepare(pc->clks[MTK_CLK_PWM1 + pwm->hwpwm]);
	clk_disable_unprepare(pc->clks[MTK_CLK_MAIN]);
	clk_disable_unprepare(pc->clks[MTK_CLK_TOP]);
}

static inline u32 mtk_pwm_readl(struct mtk_pwm_chip *chip, unsigned int num,
				unsigned int offset)
{
	return readl(chip->regs + mtk_pwm_reg_offset[num] + offset);
}

static inline void mtk_pwm_writel(struct mtk_pwm_chip *chip,
				  unsigned int num, unsigned int offset,
				  u32 value)
{
	writel(value, chip->regs + mtk_pwm_reg_offset[num] + offset);
}

static int mtk_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			  int duty_ns, int period_ns)
{
	struct mtk_pwm_chip *pc = to_mtk_pwm_chip(chip);
	struct clk *clk = pc->clks[MTK_CLK_PWM1 + pwm->hwpwm];
=======
static void pwm_mediatek_clk_disable(struct pwm_chip *chip,
				     struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);

	clk_disable_unprepare(pc->clk_pwms[pwm->hwpwm]);
	clk_disable_unprepare(pc->clk_main);
	clk_disable_unprepare(pc->clk_top);
}

static inline void pwm_mediatek_writel(struct pwm_mediatek_chip *chip,
				       unsigned int num, unsigned int offset,
				       u32 value)
{
	writel(value, chip->regs + pwm_mediatek_reg_offset[num] + offset);
}

static int pwm_mediatek_config(struct pwm_chip *chip, struct pwm_device *pwm,
			       int duty_ns, int period_ns)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
>>>>>>> upstream/android-13
	u32 clkdiv = 0, cnt_period, cnt_duty, reg_width = PWMDWIDTH,
	    reg_thres = PWMTHRES;
	u64 resolution;
	int ret;

<<<<<<< HEAD
	ret = mtk_pwm_clk_enable(chip, pwm);
	if (ret < 0)
		return ret;

	/* Using resolution in picosecond gets accuracy higher */
	resolution = (u64)NSEC_PER_SEC * 1000;
	do_div(resolution, clk_get_rate(clk));
=======
	ret = pwm_mediatek_clk_enable(chip, pwm);

	if (ret < 0)
		return ret;

	/* Make sure we use the bus clock and not the 26MHz clock */
	if (pc->soc->has_ck_26m_sel)
		writel(0, pc->regs + PWM_CK_26M_SEL);

	/* Using resolution in picosecond gets accuracy higher */
	resolution = (u64)NSEC_PER_SEC * 1000;
	do_div(resolution, clk_get_rate(pc->clk_pwms[pwm->hwpwm]));
>>>>>>> upstream/android-13

	cnt_period = DIV_ROUND_CLOSEST_ULL((u64)period_ns * 1000, resolution);
	while (cnt_period > 8191) {
		resolution *= 2;
		clkdiv++;
		cnt_period = DIV_ROUND_CLOSEST_ULL((u64)period_ns * 1000,
						   resolution);
	}

	if (clkdiv > PWM_CLK_DIV_MAX) {
<<<<<<< HEAD
		mtk_pwm_clk_disable(chip, pwm);
=======
		pwm_mediatek_clk_disable(chip, pwm);
>>>>>>> upstream/android-13
		dev_err(chip->dev, "period %d not supported\n", period_ns);
		return -EINVAL;
	}

	if (pc->soc->pwm45_fixup && pwm->hwpwm > 2) {
		/*
		 * PWM[4,5] has distinct offset for PWMDWIDTH and PWMTHRES
		 * from the other PWMs on MT7623.
		 */
		reg_width = PWM45DWIDTH_FIXUP;
		reg_thres = PWM45THRES_FIXUP;
	}

	cnt_duty = DIV_ROUND_CLOSEST_ULL((u64)duty_ns * 1000, resolution);
<<<<<<< HEAD
	mtk_pwm_writel(pc, pwm->hwpwm, PWMCON, BIT(15) | clkdiv);
	mtk_pwm_writel(pc, pwm->hwpwm, reg_width, cnt_period);
	mtk_pwm_writel(pc, pwm->hwpwm, reg_thres, cnt_duty);

	mtk_pwm_clk_disable(chip, pwm);
=======
	pwm_mediatek_writel(pc, pwm->hwpwm, PWMCON, BIT(15) | clkdiv);
	pwm_mediatek_writel(pc, pwm->hwpwm, reg_width, cnt_period);
	pwm_mediatek_writel(pc, pwm->hwpwm, reg_thres, cnt_duty);

	pwm_mediatek_clk_disable(chip, pwm);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int mtk_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct mtk_pwm_chip *pc = to_mtk_pwm_chip(chip);
	u32 value;
	int ret;

	ret = mtk_pwm_clk_enable(chip, pwm);
=======
static int pwm_mediatek_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
	u32 value;
	int ret;

	ret = pwm_mediatek_clk_enable(chip, pwm);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	value = readl(pc->regs);
	value |= BIT(pwm->hwpwm);
	writel(value, pc->regs);

	return 0;
}

<<<<<<< HEAD
static void mtk_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct mtk_pwm_chip *pc = to_mtk_pwm_chip(chip);
=======
static void pwm_mediatek_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
>>>>>>> upstream/android-13
	u32 value;

	value = readl(pc->regs);
	value &= ~BIT(pwm->hwpwm);
	writel(value, pc->regs);

<<<<<<< HEAD
	mtk_pwm_clk_disable(chip, pwm);
}

static const struct pwm_ops mtk_pwm_ops = {
	.config = mtk_pwm_config,
	.enable = mtk_pwm_enable,
	.disable = mtk_pwm_disable,
	.owner = THIS_MODULE,
};

static int mtk_pwm_probe(struct platform_device *pdev)
{
	const struct mtk_pwm_platform_data *data;
	struct mtk_pwm_chip *pc;
	struct resource *res;
=======
	pwm_mediatek_clk_disable(chip, pwm);
}

static const struct pwm_ops pwm_mediatek_ops = {
	.config = pwm_mediatek_config,
	.enable = pwm_mediatek_enable,
	.disable = pwm_mediatek_disable,
	.owner = THIS_MODULE,
};

static int pwm_mediatek_probe(struct platform_device *pdev)
{
	struct pwm_mediatek_chip *pc;
>>>>>>> upstream/android-13
	unsigned int i;
	int ret;

	pc = devm_kzalloc(&pdev->dev, sizeof(*pc), GFP_KERNEL);
	if (!pc)
		return -ENOMEM;

<<<<<<< HEAD
	data = of_device_get_match_data(&pdev->dev);
	if (data == NULL)
		return -EINVAL;
	pc->soc = data;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	pc->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(pc->regs))
		return PTR_ERR(pc->regs);

	for (i = 0; i < data->num_pwms + 2 && pc->soc->has_clks; i++) {
		pc->clks[i] = devm_clk_get(&pdev->dev, mtk_pwm_clk_name[i]);
		if (IS_ERR(pc->clks[i])) {
			dev_err(&pdev->dev, "clock: %s fail: %ld\n",
				mtk_pwm_clk_name[i], PTR_ERR(pc->clks[i]));
			return PTR_ERR(pc->clks[i]);
		}
	}

	platform_set_drvdata(pdev, pc);

	pc->chip.dev = &pdev->dev;
	pc->chip.ops = &mtk_pwm_ops;
	pc->chip.base = -1;
	pc->chip.npwm = data->num_pwms;

	ret = pwmchip_add(&pc->chip);
=======
	pc->soc = of_device_get_match_data(&pdev->dev);

	pc->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(pc->regs))
		return PTR_ERR(pc->regs);

	pc->clk_pwms = devm_kcalloc(&pdev->dev, pc->soc->num_pwms,
				    sizeof(*pc->clk_pwms), GFP_KERNEL);
	if (!pc->clk_pwms)
		return -ENOMEM;

	pc->clk_top = devm_clk_get(&pdev->dev, "top");
	if (IS_ERR(pc->clk_top)) {
		dev_err(&pdev->dev, "clock: top fail: %ld\n",
			PTR_ERR(pc->clk_top));
		return PTR_ERR(pc->clk_top);
	}

	pc->clk_main = devm_clk_get(&pdev->dev, "main");
	if (IS_ERR(pc->clk_main)) {
		dev_err(&pdev->dev, "clock: main fail: %ld\n",
			PTR_ERR(pc->clk_main));
		return PTR_ERR(pc->clk_main);
	}

	for (i = 0; i < pc->soc->num_pwms; i++) {
		char name[8];

		snprintf(name, sizeof(name), "pwm%d", i + 1);

		pc->clk_pwms[i] = devm_clk_get(&pdev->dev, name);
		if (IS_ERR(pc->clk_pwms[i])) {
			dev_err(&pdev->dev, "clock: %s fail: %ld\n",
				name, PTR_ERR(pc->clk_pwms[i]));
			return PTR_ERR(pc->clk_pwms[i]);
		}
	}

	pc->chip.dev = &pdev->dev;
	pc->chip.ops = &pwm_mediatek_ops;
	pc->chip.npwm = pc->soc->num_pwms;

	ret = devm_pwmchip_add(&pdev->dev, &pc->chip);
>>>>>>> upstream/android-13
	if (ret < 0) {
		dev_err(&pdev->dev, "pwmchip_add() failed: %d\n", ret);
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int mtk_pwm_remove(struct platform_device *pdev)
{
	struct mtk_pwm_chip *pc = platform_get_drvdata(pdev);

	return pwmchip_remove(&pc->chip);
}

static const struct mtk_pwm_platform_data mt2712_pwm_data = {
	.num_pwms = 8,
	.pwm45_fixup = false,
	.has_clks = true,
};

static const struct mtk_pwm_platform_data mt7622_pwm_data = {
	.num_pwms = 6,
	.pwm45_fixup = false,
	.has_clks = true,
};

static const struct mtk_pwm_platform_data mt7623_pwm_data = {
	.num_pwms = 5,
	.pwm45_fixup = true,
	.has_clks = true,
};

static const struct mtk_pwm_platform_data mt7628_pwm_data = {
	.num_pwms = 4,
	.pwm45_fixup = true,
	.has_clks = false,
};

static const struct of_device_id mtk_pwm_of_match[] = {
=======
static const struct pwm_mediatek_of_data mt2712_pwm_data = {
	.num_pwms = 8,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
};

static const struct pwm_mediatek_of_data mt7622_pwm_data = {
	.num_pwms = 6,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
};

static const struct pwm_mediatek_of_data mt7623_pwm_data = {
	.num_pwms = 5,
	.pwm45_fixup = true,
	.has_ck_26m_sel = false,
};

static const struct pwm_mediatek_of_data mt7628_pwm_data = {
	.num_pwms = 4,
	.pwm45_fixup = true,
	.has_ck_26m_sel = false,
};

static const struct pwm_mediatek_of_data mt7629_pwm_data = {
	.num_pwms = 1,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
};

static const struct pwm_mediatek_of_data mt8183_pwm_data = {
	.num_pwms = 4,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
};

static const struct pwm_mediatek_of_data mt8516_pwm_data = {
	.num_pwms = 5,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
};

static const struct of_device_id pwm_mediatek_of_match[] = {
>>>>>>> upstream/android-13
	{ .compatible = "mediatek,mt2712-pwm", .data = &mt2712_pwm_data },
	{ .compatible = "mediatek,mt7622-pwm", .data = &mt7622_pwm_data },
	{ .compatible = "mediatek,mt7623-pwm", .data = &mt7623_pwm_data },
	{ .compatible = "mediatek,mt7628-pwm", .data = &mt7628_pwm_data },
<<<<<<< HEAD
	{ },
};
MODULE_DEVICE_TABLE(of, mtk_pwm_of_match);

static struct platform_driver mtk_pwm_driver = {
	.driver = {
		.name = "mtk-pwm",
		.of_match_table = mtk_pwm_of_match,
	},
	.probe = mtk_pwm_probe,
	.remove = mtk_pwm_remove,
};
module_platform_driver(mtk_pwm_driver);

MODULE_AUTHOR("John Crispin <blogic@openwrt.org>");
MODULE_LICENSE("GPL");
=======
	{ .compatible = "mediatek,mt7629-pwm", .data = &mt7629_pwm_data },
	{ .compatible = "mediatek,mt8183-pwm", .data = &mt8183_pwm_data },
	{ .compatible = "mediatek,mt8516-pwm", .data = &mt8516_pwm_data },
	{ },
};
MODULE_DEVICE_TABLE(of, pwm_mediatek_of_match);

static struct platform_driver pwm_mediatek_driver = {
	.driver = {
		.name = "pwm-mediatek",
		.of_match_table = pwm_mediatek_of_match,
	},
	.probe = pwm_mediatek_probe,
};
module_platform_driver(pwm_mediatek_driver);

MODULE_AUTHOR("John Crispin <blogic@openwrt.org>");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
