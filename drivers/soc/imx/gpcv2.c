<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * Copyright 2017 Impinj, Inc
 * Author: Andrey Smirnov <andrew.smirnov@gmail.com>
 *
 * Based on the code of analogus driver:
 *
 * Copyright 2015-2017 Pengutronix, Lucas Stach <kernel@pengutronix.de>
<<<<<<< HEAD
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/platform_device.h>
#include <linux/pm_domain.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <dt-bindings/power/imx7-power.h>

#define GPC_LPCR_A7_BSC			0x000

#define GPC_PGC_CPU_MAPPING		0x0ec
#define USB_HSIC_PHY_A7_DOMAIN		BIT(6)
#define USB_OTG2_PHY_A7_DOMAIN		BIT(5)
#define USB_OTG1_PHY_A7_DOMAIN		BIT(4)
#define PCIE_PHY_A7_DOMAIN		BIT(3)
#define MIPI_PHY_A7_DOMAIN		BIT(2)

#define GPC_PU_PGC_SW_PUP_REQ		0x0f8
#define GPC_PU_PGC_SW_PDN_REQ		0x104
#define USB_HSIC_PHY_SW_Pxx_REQ		BIT(4)
#define USB_OTG2_PHY_SW_Pxx_REQ		BIT(3)
#define USB_OTG1_PHY_SW_Pxx_REQ		BIT(2)
#define PCIE_PHY_SW_Pxx_REQ		BIT(1)
#define MIPI_PHY_SW_Pxx_REQ		BIT(0)

#define GPC_M4_PU_PDN_FLG		0x1bc

=======
 */

#include <linux/clk.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_domain.h>
#include <linux/pm_runtime.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/reset.h>
#include <linux/sizes.h>
#include <dt-bindings/power/imx7-power.h>
#include <dt-bindings/power/imx8mq-power.h>
#include <dt-bindings/power/imx8mm-power.h>
#include <dt-bindings/power/imx8mn-power.h>

#define GPC_LPCR_A_CORE_BSC			0x000

#define GPC_PGC_CPU_MAPPING		0x0ec

#define IMX7_USB_HSIC_PHY_A_CORE_DOMAIN		BIT(6)
#define IMX7_USB_OTG2_PHY_A_CORE_DOMAIN		BIT(5)
#define IMX7_USB_OTG1_PHY_A_CORE_DOMAIN		BIT(4)
#define IMX7_PCIE_PHY_A_CORE_DOMAIN		BIT(3)
#define IMX7_MIPI_PHY_A_CORE_DOMAIN		BIT(2)

#define IMX8M_PCIE2_A53_DOMAIN			BIT(15)
#define IMX8M_MIPI_CSI2_A53_DOMAIN		BIT(14)
#define IMX8M_MIPI_CSI1_A53_DOMAIN		BIT(13)
#define IMX8M_DISP_A53_DOMAIN			BIT(12)
#define IMX8M_HDMI_A53_DOMAIN			BIT(11)
#define IMX8M_VPU_A53_DOMAIN			BIT(10)
#define IMX8M_GPU_A53_DOMAIN			BIT(9)
#define IMX8M_DDR2_A53_DOMAIN			BIT(8)
#define IMX8M_DDR1_A53_DOMAIN			BIT(7)
#define IMX8M_OTG2_A53_DOMAIN			BIT(5)
#define IMX8M_OTG1_A53_DOMAIN			BIT(4)
#define IMX8M_PCIE1_A53_DOMAIN			BIT(3)
#define IMX8M_MIPI_A53_DOMAIN			BIT(2)

#define IMX8MM_VPUH1_A53_DOMAIN			BIT(15)
#define IMX8MM_VPUG2_A53_DOMAIN			BIT(14)
#define IMX8MM_VPUG1_A53_DOMAIN			BIT(13)
#define IMX8MM_DISPMIX_A53_DOMAIN		BIT(12)
#define IMX8MM_VPUMIX_A53_DOMAIN		BIT(10)
#define IMX8MM_GPUMIX_A53_DOMAIN		BIT(9)
#define IMX8MM_GPU_A53_DOMAIN			(BIT(8) | BIT(11))
#define IMX8MM_DDR1_A53_DOMAIN			BIT(7)
#define IMX8MM_OTG2_A53_DOMAIN			BIT(5)
#define IMX8MM_OTG1_A53_DOMAIN			BIT(4)
#define IMX8MM_PCIE_A53_DOMAIN			BIT(3)
#define IMX8MM_MIPI_A53_DOMAIN			BIT(2)

#define IMX8MN_DISPMIX_A53_DOMAIN		BIT(12)
#define IMX8MN_GPUMIX_A53_DOMAIN		BIT(9)
#define IMX8MN_DDR1_A53_DOMAIN		BIT(7)
#define IMX8MN_OTG1_A53_DOMAIN		BIT(4)
#define IMX8MN_MIPI_A53_DOMAIN		BIT(2)

#define GPC_PU_PGC_SW_PUP_REQ		0x0f8
#define GPC_PU_PGC_SW_PDN_REQ		0x104

#define IMX7_USB_HSIC_PHY_SW_Pxx_REQ		BIT(4)
#define IMX7_USB_OTG2_PHY_SW_Pxx_REQ		BIT(3)
#define IMX7_USB_OTG1_PHY_SW_Pxx_REQ		BIT(2)
#define IMX7_PCIE_PHY_SW_Pxx_REQ		BIT(1)
#define IMX7_MIPI_PHY_SW_Pxx_REQ		BIT(0)

#define IMX8M_PCIE2_SW_Pxx_REQ			BIT(13)
#define IMX8M_MIPI_CSI2_SW_Pxx_REQ		BIT(12)
#define IMX8M_MIPI_CSI1_SW_Pxx_REQ		BIT(11)
#define IMX8M_DISP_SW_Pxx_REQ			BIT(10)
#define IMX8M_HDMI_SW_Pxx_REQ			BIT(9)
#define IMX8M_VPU_SW_Pxx_REQ			BIT(8)
#define IMX8M_GPU_SW_Pxx_REQ			BIT(7)
#define IMX8M_DDR2_SW_Pxx_REQ			BIT(6)
#define IMX8M_DDR1_SW_Pxx_REQ			BIT(5)
#define IMX8M_OTG2_SW_Pxx_REQ			BIT(3)
#define IMX8M_OTG1_SW_Pxx_REQ			BIT(2)
#define IMX8M_PCIE1_SW_Pxx_REQ			BIT(1)
#define IMX8M_MIPI_SW_Pxx_REQ			BIT(0)

#define IMX8MM_VPUH1_SW_Pxx_REQ			BIT(13)
#define IMX8MM_VPUG2_SW_Pxx_REQ			BIT(12)
#define IMX8MM_VPUG1_SW_Pxx_REQ			BIT(11)
#define IMX8MM_DISPMIX_SW_Pxx_REQ		BIT(10)
#define IMX8MM_VPUMIX_SW_Pxx_REQ		BIT(8)
#define IMX8MM_GPUMIX_SW_Pxx_REQ		BIT(7)
#define IMX8MM_GPU_SW_Pxx_REQ			(BIT(6) | BIT(9))
#define IMX8MM_DDR1_SW_Pxx_REQ			BIT(5)
#define IMX8MM_OTG2_SW_Pxx_REQ			BIT(3)
#define IMX8MM_OTG1_SW_Pxx_REQ			BIT(2)
#define IMX8MM_PCIE_SW_Pxx_REQ			BIT(1)
#define IMX8MM_MIPI_SW_Pxx_REQ			BIT(0)

#define IMX8MN_DISPMIX_SW_Pxx_REQ		BIT(10)
#define IMX8MN_GPUMIX_SW_Pxx_REQ		BIT(7)
#define IMX8MN_DDR1_SW_Pxx_REQ		BIT(5)
#define IMX8MN_OTG1_SW_Pxx_REQ		BIT(2)
#define IMX8MN_MIPI_SW_Pxx_REQ		BIT(0)

#define GPC_M4_PU_PDN_FLG		0x1bc

#define GPC_PU_PWRHSK			0x1fc

#define IMX8M_GPU_HSK_PWRDNACKN			BIT(26)
#define IMX8M_VPU_HSK_PWRDNACKN			BIT(25)
#define IMX8M_DISP_HSK_PWRDNACKN		BIT(24)
#define IMX8M_GPU_HSK_PWRDNREQN			BIT(6)
#define IMX8M_VPU_HSK_PWRDNREQN			BIT(5)
#define IMX8M_DISP_HSK_PWRDNREQN		BIT(4)


#define IMX8MM_GPUMIX_HSK_PWRDNACKN		BIT(29)
#define IMX8MM_GPU_HSK_PWRDNACKN		(BIT(27) | BIT(28))
#define IMX8MM_VPUMIX_HSK_PWRDNACKN		BIT(26)
#define IMX8MM_DISPMIX_HSK_PWRDNACKN		BIT(25)
#define IMX8MM_HSIO_HSK_PWRDNACKN		(BIT(23) | BIT(24))
#define IMX8MM_GPUMIX_HSK_PWRDNREQN		BIT(11)
#define IMX8MM_GPU_HSK_PWRDNREQN		(BIT(9) | BIT(10))
#define IMX8MM_VPUMIX_HSK_PWRDNREQN		BIT(8)
#define IMX8MM_DISPMIX_HSK_PWRDNREQN		BIT(7)
#define IMX8MM_HSIO_HSK_PWRDNREQN		(BIT(5) | BIT(6))

#define IMX8MN_GPUMIX_HSK_PWRDNACKN		(BIT(29) | BIT(27))
#define IMX8MN_DISPMIX_HSK_PWRDNACKN		BIT(25)
#define IMX8MN_HSIO_HSK_PWRDNACKN		BIT(23)
#define IMX8MN_GPUMIX_HSK_PWRDNREQN		(BIT(11) | BIT(9))
#define IMX8MN_DISPMIX_HSK_PWRDNREQN		BIT(7)
#define IMX8MN_HSIO_HSK_PWRDNREQN		BIT(5)

>>>>>>> upstream/android-13
/*
 * The PGC offset values in Reference Manual
 * (Rev. 1, 01/2018 and the older ones) GPC chapter's
 * GPC_PGC memory map are incorrect, below offset
 * values are from design RTL.
 */
<<<<<<< HEAD
#define PGC_MIPI			16
#define PGC_PCIE			17
#define PGC_USB_HSIC			20
=======
#define IMX7_PGC_MIPI			16
#define IMX7_PGC_PCIE			17
#define IMX7_PGC_USB_HSIC		20

#define IMX8M_PGC_MIPI			16
#define IMX8M_PGC_PCIE1			17
#define IMX8M_PGC_OTG1			18
#define IMX8M_PGC_OTG2			19
#define IMX8M_PGC_DDR1			21
#define IMX8M_PGC_GPU			23
#define IMX8M_PGC_VPU			24
#define IMX8M_PGC_DISP			26
#define IMX8M_PGC_MIPI_CSI1		27
#define IMX8M_PGC_MIPI_CSI2		28
#define IMX8M_PGC_PCIE2			29

#define IMX8MM_PGC_MIPI			16
#define IMX8MM_PGC_PCIE			17
#define IMX8MM_PGC_OTG1			18
#define IMX8MM_PGC_OTG2			19
#define IMX8MM_PGC_DDR1			21
#define IMX8MM_PGC_GPU2D		22
#define IMX8MM_PGC_GPUMIX		23
#define IMX8MM_PGC_VPUMIX		24
#define IMX8MM_PGC_GPU3D		25
#define IMX8MM_PGC_DISPMIX		26
#define IMX8MM_PGC_VPUG1		27
#define IMX8MM_PGC_VPUG2		28
#define IMX8MM_PGC_VPUH1		29

#define IMX8MN_PGC_MIPI		16
#define IMX8MN_PGC_OTG1		18
#define IMX8MN_PGC_DDR1		21
#define IMX8MN_PGC_GPUMIX		23
#define IMX8MN_PGC_DISPMIX		26

>>>>>>> upstream/android-13
#define GPC_PGC_CTRL(n)			(0x800 + (n) * 0x40)
#define GPC_PGC_SR(n)			(GPC_PGC_CTRL(n) + 0xc)

#define GPC_PGC_CTRL_PCR		BIT(0)

<<<<<<< HEAD
struct imx7_pgc_domain {
	struct generic_pm_domain genpd;
	struct regmap *regmap;
	struct regulator *regulator;
=======
struct imx_pgc_domain {
	struct generic_pm_domain genpd;
	struct regmap *regmap;
	struct regulator *regulator;
	struct reset_control *reset;
	struct clk_bulk_data *clks;
	int num_clks;
>>>>>>> upstream/android-13

	unsigned int pgc;

	const struct {
		u32 pxx;
		u32 map;
<<<<<<< HEAD
=======
		u32 hskreq;
		u32 hskack;
>>>>>>> upstream/android-13
	} bits;

	const int voltage;
	struct device *dev;
};

<<<<<<< HEAD
static int imx7_gpc_pu_pgc_sw_pxx_req(struct generic_pm_domain *genpd,
				      bool on)
{
	struct imx7_pgc_domain *domain = container_of(genpd,
						      struct imx7_pgc_domain,
						      genpd);
	unsigned int offset = on ?
		GPC_PU_PGC_SW_PUP_REQ : GPC_PU_PGC_SW_PDN_REQ;
	const bool enable_power_control = !on;
	const bool has_regulator = !IS_ERR(domain->regulator);
	unsigned long deadline;
	int ret = 0;

	regmap_update_bits(domain->regmap, GPC_PGC_CPU_MAPPING,
			   domain->bits.map, domain->bits.map);

	if (has_regulator && on) {
		ret = regulator_enable(domain->regulator);
		if (ret) {
			dev_err(domain->dev, "failed to enable regulator\n");
			goto unmap;
		}
	}

	if (enable_power_control)
		regmap_update_bits(domain->regmap, GPC_PGC_CTRL(domain->pgc),
				   GPC_PGC_CTRL_PCR, GPC_PGC_CTRL_PCR);

	regmap_update_bits(domain->regmap, offset,
			   domain->bits.pxx, domain->bits.pxx);

	/*
	 * As per "5.5.9.4 Example Code 4" in IMX7DRM.pdf wait
	 * for PUP_REQ/PDN_REQ bit to be cleared
	 */
	deadline = jiffies + msecs_to_jiffies(1);
	while (true) {
		u32 pxx_req;

		regmap_read(domain->regmap, offset, &pxx_req);

		if (!(pxx_req & domain->bits.pxx))
			break;

		if (time_after(jiffies, deadline)) {
			dev_err(domain->dev, "falied to command PGC\n");
			ret = -ETIMEDOUT;
			/*
			 * If we were in a process of enabling a
			 * domain and failed we might as well disable
			 * the regulator we just enabled. And if it
			 * was the opposite situation and we failed to
			 * power down -- keep the regulator on
			 */
			on = !on;
			break;
		}

		cpu_relax();
	}

	if (enable_power_control)
		regmap_update_bits(domain->regmap, GPC_PGC_CTRL(domain->pgc),
				   GPC_PGC_CTRL_PCR, 0);

	if (has_regulator && !on) {
		int err;

		err = regulator_disable(domain->regulator);
		if (err)
			dev_err(domain->dev,
				"failed to disable regulator: %d\n", ret);
		/* Preserve earlier error code */
		ret = ret ?: err;
	}
unmap:
	regmap_update_bits(domain->regmap, GPC_PGC_CPU_MAPPING,
			   domain->bits.map, 0);
	return ret;
}

static int imx7_gpc_pu_pgc_sw_pup_req(struct generic_pm_domain *genpd)
{
	return imx7_gpc_pu_pgc_sw_pxx_req(genpd, true);
}

static int imx7_gpc_pu_pgc_sw_pdn_req(struct generic_pm_domain *genpd)
{
	return imx7_gpc_pu_pgc_sw_pxx_req(genpd, false);
}

static const struct imx7_pgc_domain imx7_pgc_domains[] = {
=======
struct imx_pgc_domain_data {
	const struct imx_pgc_domain *domains;
	size_t domains_num;
	const struct regmap_access_table *reg_access_table;
};

static inline struct imx_pgc_domain *
to_imx_pgc_domain(struct generic_pm_domain *genpd)
{
	return container_of(genpd, struct imx_pgc_domain, genpd);
}

static int imx_pgc_power_up(struct generic_pm_domain *genpd)
{
	struct imx_pgc_domain *domain = to_imx_pgc_domain(genpd);
	u32 reg_val;
	int ret;

	ret = pm_runtime_get_sync(domain->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(domain->dev);
		return ret;
	}

	if (!IS_ERR(domain->regulator)) {
		ret = regulator_enable(domain->regulator);
		if (ret) {
			dev_err(domain->dev, "failed to enable regulator\n");
			goto out_put_pm;
		}
	}

	/* Enable reset clocks for all devices in the domain */
	ret = clk_bulk_prepare_enable(domain->num_clks, domain->clks);
	if (ret) {
		dev_err(domain->dev, "failed to enable reset clocks\n");
		goto out_regulator_disable;
	}

	reset_control_assert(domain->reset);

	if (domain->bits.pxx) {
		/* request the domain to power up */
		regmap_update_bits(domain->regmap, GPC_PU_PGC_SW_PUP_REQ,
				   domain->bits.pxx, domain->bits.pxx);
		/*
		 * As per "5.5.9.4 Example Code 4" in IMX7DRM.pdf wait
		 * for PUP_REQ/PDN_REQ bit to be cleared
		 */
		ret = regmap_read_poll_timeout(domain->regmap,
					       GPC_PU_PGC_SW_PUP_REQ, reg_val,
					       !(reg_val & domain->bits.pxx),
					       0, USEC_PER_MSEC);
		if (ret) {
			dev_err(domain->dev, "failed to command PGC\n");
			goto out_clk_disable;
		}

		/* disable power control */
		regmap_clear_bits(domain->regmap, GPC_PGC_CTRL(domain->pgc),
				  GPC_PGC_CTRL_PCR);
	}

	/* delay for reset to propagate */
	udelay(5);

	reset_control_deassert(domain->reset);

	/* request the ADB400 to power up */
	if (domain->bits.hskreq) {
		regmap_update_bits(domain->regmap, GPC_PU_PWRHSK,
				   domain->bits.hskreq, domain->bits.hskreq);

		/*
		 * ret = regmap_read_poll_timeout(domain->regmap, GPC_PU_PWRHSK, reg_val,
		 *				  (reg_val & domain->bits.hskack), 0,
		 *				  USEC_PER_MSEC);
		 * Technically we need the commented code to wait handshake. But that needs
		 * the BLK-CTL module BUS clk-en bit being set.
		 *
		 * There is a separate BLK-CTL module and we will have such a driver for it,
		 * that driver will set the BUS clk-en bit and handshake will be triggered
		 * automatically there. Just add a delay and suppose the handshake finish
		 * after that.
		 */
	}

	/* Disable reset clocks for all devices in the domain */
	clk_bulk_disable_unprepare(domain->num_clks, domain->clks);

	return 0;

out_clk_disable:
	clk_bulk_disable_unprepare(domain->num_clks, domain->clks);
out_regulator_disable:
	if (!IS_ERR(domain->regulator))
		regulator_disable(domain->regulator);
out_put_pm:
	pm_runtime_put(domain->dev);

	return ret;
}

static int imx_pgc_power_down(struct generic_pm_domain *genpd)
{
	struct imx_pgc_domain *domain = to_imx_pgc_domain(genpd);
	u32 reg_val;
	int ret;

	/* Enable reset clocks for all devices in the domain */
	ret = clk_bulk_prepare_enable(domain->num_clks, domain->clks);
	if (ret) {
		dev_err(domain->dev, "failed to enable reset clocks\n");
		return ret;
	}

	/* request the ADB400 to power down */
	if (domain->bits.hskreq) {
		regmap_clear_bits(domain->regmap, GPC_PU_PWRHSK,
				  domain->bits.hskreq);

		ret = regmap_read_poll_timeout(domain->regmap, GPC_PU_PWRHSK,
					       reg_val,
					       !(reg_val & domain->bits.hskack),
					       0, USEC_PER_MSEC);
		if (ret) {
			dev_err(domain->dev, "failed to power down ADB400\n");
			goto out_clk_disable;
		}
	}

	if (domain->bits.pxx) {
		/* enable power control */
		regmap_update_bits(domain->regmap, GPC_PGC_CTRL(domain->pgc),
				   GPC_PGC_CTRL_PCR, GPC_PGC_CTRL_PCR);

		/* request the domain to power down */
		regmap_update_bits(domain->regmap, GPC_PU_PGC_SW_PDN_REQ,
				   domain->bits.pxx, domain->bits.pxx);
		/*
		 * As per "5.5.9.4 Example Code 4" in IMX7DRM.pdf wait
		 * for PUP_REQ/PDN_REQ bit to be cleared
		 */
		ret = regmap_read_poll_timeout(domain->regmap,
					       GPC_PU_PGC_SW_PDN_REQ, reg_val,
					       !(reg_val & domain->bits.pxx),
					       0, USEC_PER_MSEC);
		if (ret) {
			dev_err(domain->dev, "failed to command PGC\n");
			goto out_clk_disable;
		}
	}

	/* Disable reset clocks for all devices in the domain */
	clk_bulk_disable_unprepare(domain->num_clks, domain->clks);

	if (!IS_ERR(domain->regulator)) {
		ret = regulator_disable(domain->regulator);
		if (ret) {
			dev_err(domain->dev, "failed to disable regulator\n");
			return ret;
		}
	}

	pm_runtime_put_sync_suspend(domain->dev);

	return 0;

out_clk_disable:
	clk_bulk_disable_unprepare(domain->num_clks, domain->clks);

	return ret;
}

static const struct imx_pgc_domain imx7_pgc_domains[] = {
>>>>>>> upstream/android-13
	[IMX7_POWER_DOMAIN_MIPI_PHY] = {
		.genpd = {
			.name      = "mipi-phy",
		},
		.bits  = {
<<<<<<< HEAD
			.pxx = MIPI_PHY_SW_Pxx_REQ,
			.map = MIPI_PHY_A7_DOMAIN,
		},
		.voltage   = 1000000,
		.pgc	   = PGC_MIPI,
=======
			.pxx = IMX7_MIPI_PHY_SW_Pxx_REQ,
			.map = IMX7_MIPI_PHY_A_CORE_DOMAIN,
		},
		.voltage   = 1000000,
		.pgc	   = IMX7_PGC_MIPI,
>>>>>>> upstream/android-13
	},

	[IMX7_POWER_DOMAIN_PCIE_PHY] = {
		.genpd = {
			.name      = "pcie-phy",
		},
		.bits  = {
<<<<<<< HEAD
			.pxx = PCIE_PHY_SW_Pxx_REQ,
			.map = PCIE_PHY_A7_DOMAIN,
		},
		.voltage   = 1000000,
		.pgc	   = PGC_PCIE,
=======
			.pxx = IMX7_PCIE_PHY_SW_Pxx_REQ,
			.map = IMX7_PCIE_PHY_A_CORE_DOMAIN,
		},
		.voltage   = 1000000,
		.pgc	   = IMX7_PGC_PCIE,
>>>>>>> upstream/android-13
	},

	[IMX7_POWER_DOMAIN_USB_HSIC_PHY] = {
		.genpd = {
			.name      = "usb-hsic-phy",
		},
		.bits  = {
<<<<<<< HEAD
			.pxx = USB_HSIC_PHY_SW_Pxx_REQ,
			.map = USB_HSIC_PHY_A7_DOMAIN,
		},
		.voltage   = 1200000,
		.pgc	   = PGC_USB_HSIC,
	},
};

static int imx7_pgc_domain_probe(struct platform_device *pdev)
{
	struct imx7_pgc_domain *domain = pdev->dev.platform_data;
=======
			.pxx = IMX7_USB_HSIC_PHY_SW_Pxx_REQ,
			.map = IMX7_USB_HSIC_PHY_A_CORE_DOMAIN,
		},
		.voltage   = 1200000,
		.pgc	   = IMX7_PGC_USB_HSIC,
	},
};

static const struct regmap_range imx7_yes_ranges[] = {
		regmap_reg_range(GPC_LPCR_A_CORE_BSC,
				 GPC_M4_PU_PDN_FLG),
		regmap_reg_range(GPC_PGC_CTRL(IMX7_PGC_MIPI),
				 GPC_PGC_SR(IMX7_PGC_MIPI)),
		regmap_reg_range(GPC_PGC_CTRL(IMX7_PGC_PCIE),
				 GPC_PGC_SR(IMX7_PGC_PCIE)),
		regmap_reg_range(GPC_PGC_CTRL(IMX7_PGC_USB_HSIC),
				 GPC_PGC_SR(IMX7_PGC_USB_HSIC)),
};

static const struct regmap_access_table imx7_access_table = {
	.yes_ranges	= imx7_yes_ranges,
	.n_yes_ranges	= ARRAY_SIZE(imx7_yes_ranges),
};

static const struct imx_pgc_domain_data imx7_pgc_domain_data = {
	.domains = imx7_pgc_domains,
	.domains_num = ARRAY_SIZE(imx7_pgc_domains),
	.reg_access_table = &imx7_access_table,
};

static const struct imx_pgc_domain imx8m_pgc_domains[] = {
	[IMX8M_POWER_DOMAIN_MIPI] = {
		.genpd = {
			.name      = "mipi",
		},
		.bits  = {
			.pxx = IMX8M_MIPI_SW_Pxx_REQ,
			.map = IMX8M_MIPI_A53_DOMAIN,
		},
		.pgc	   = IMX8M_PGC_MIPI,
	},

	[IMX8M_POWER_DOMAIN_PCIE1] = {
		.genpd = {
			.name = "pcie1",
		},
		.bits  = {
			.pxx = IMX8M_PCIE1_SW_Pxx_REQ,
			.map = IMX8M_PCIE1_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_PCIE1,
	},

	[IMX8M_POWER_DOMAIN_USB_OTG1] = {
		.genpd = {
			.name = "usb-otg1",
		},
		.bits  = {
			.pxx = IMX8M_OTG1_SW_Pxx_REQ,
			.map = IMX8M_OTG1_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_OTG1,
	},

	[IMX8M_POWER_DOMAIN_USB_OTG2] = {
		.genpd = {
			.name = "usb-otg2",
		},
		.bits  = {
			.pxx = IMX8M_OTG2_SW_Pxx_REQ,
			.map = IMX8M_OTG2_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_OTG2,
	},

	[IMX8M_POWER_DOMAIN_DDR1] = {
		.genpd = {
			.name = "ddr1",
		},
		.bits  = {
			.pxx = IMX8M_DDR1_SW_Pxx_REQ,
			.map = IMX8M_DDR2_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_DDR1,
	},

	[IMX8M_POWER_DOMAIN_GPU] = {
		.genpd = {
			.name = "gpu",
		},
		.bits  = {
			.pxx = IMX8M_GPU_SW_Pxx_REQ,
			.map = IMX8M_GPU_A53_DOMAIN,
			.hskreq = IMX8M_GPU_HSK_PWRDNREQN,
			.hskack = IMX8M_GPU_HSK_PWRDNACKN,
		},
		.pgc   = IMX8M_PGC_GPU,
	},

	[IMX8M_POWER_DOMAIN_VPU] = {
		.genpd = {
			.name = "vpu",
		},
		.bits  = {
			.pxx = IMX8M_VPU_SW_Pxx_REQ,
			.map = IMX8M_VPU_A53_DOMAIN,
			.hskreq = IMX8M_VPU_HSK_PWRDNREQN,
			.hskack = IMX8M_VPU_HSK_PWRDNACKN,
		},
		.pgc   = IMX8M_PGC_VPU,
	},

	[IMX8M_POWER_DOMAIN_DISP] = {
		.genpd = {
			.name = "disp",
		},
		.bits  = {
			.pxx = IMX8M_DISP_SW_Pxx_REQ,
			.map = IMX8M_DISP_A53_DOMAIN,
			.hskreq = IMX8M_DISP_HSK_PWRDNREQN,
			.hskack = IMX8M_DISP_HSK_PWRDNACKN,
		},
		.pgc   = IMX8M_PGC_DISP,
	},

	[IMX8M_POWER_DOMAIN_MIPI_CSI1] = {
		.genpd = {
			.name = "mipi-csi1",
		},
		.bits  = {
			.pxx = IMX8M_MIPI_CSI1_SW_Pxx_REQ,
			.map = IMX8M_MIPI_CSI1_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_MIPI_CSI1,
	},

	[IMX8M_POWER_DOMAIN_MIPI_CSI2] = {
		.genpd = {
			.name = "mipi-csi2",
		},
		.bits  = {
			.pxx = IMX8M_MIPI_CSI2_SW_Pxx_REQ,
			.map = IMX8M_MIPI_CSI2_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_MIPI_CSI2,
	},

	[IMX8M_POWER_DOMAIN_PCIE2] = {
		.genpd = {
			.name = "pcie2",
		},
		.bits  = {
			.pxx = IMX8M_PCIE2_SW_Pxx_REQ,
			.map = IMX8M_PCIE2_A53_DOMAIN,
		},
		.pgc   = IMX8M_PGC_PCIE2,
	},
};

static const struct regmap_range imx8m_yes_ranges[] = {
		regmap_reg_range(GPC_LPCR_A_CORE_BSC,
				 GPC_PU_PWRHSK),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_MIPI),
				 GPC_PGC_SR(IMX8M_PGC_MIPI)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_PCIE1),
				 GPC_PGC_SR(IMX8M_PGC_PCIE1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_OTG1),
				 GPC_PGC_SR(IMX8M_PGC_OTG1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_OTG2),
				 GPC_PGC_SR(IMX8M_PGC_OTG2)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_DDR1),
				 GPC_PGC_SR(IMX8M_PGC_DDR1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_GPU),
				 GPC_PGC_SR(IMX8M_PGC_GPU)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_VPU),
				 GPC_PGC_SR(IMX8M_PGC_VPU)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_DISP),
				 GPC_PGC_SR(IMX8M_PGC_DISP)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_MIPI_CSI1),
				 GPC_PGC_SR(IMX8M_PGC_MIPI_CSI1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_MIPI_CSI2),
				 GPC_PGC_SR(IMX8M_PGC_MIPI_CSI2)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8M_PGC_PCIE2),
				 GPC_PGC_SR(IMX8M_PGC_PCIE2)),
};

static const struct regmap_access_table imx8m_access_table = {
	.yes_ranges	= imx8m_yes_ranges,
	.n_yes_ranges	= ARRAY_SIZE(imx8m_yes_ranges),
};

static const struct imx_pgc_domain_data imx8m_pgc_domain_data = {
	.domains = imx8m_pgc_domains,
	.domains_num = ARRAY_SIZE(imx8m_pgc_domains),
	.reg_access_table = &imx8m_access_table,
};

static const struct imx_pgc_domain imx8mm_pgc_domains[] = {
	[IMX8MM_POWER_DOMAIN_HSIOMIX] = {
		.genpd = {
			.name = "hsiomix",
		},
		.bits  = {
			.pxx = 0, /* no power sequence control */
			.map = 0, /* no power sequence control */
			.hskreq = IMX8MM_HSIO_HSK_PWRDNREQN,
			.hskack = IMX8MM_HSIO_HSK_PWRDNACKN,
		},
	},

	[IMX8MM_POWER_DOMAIN_PCIE] = {
		.genpd = {
			.name = "pcie",
		},
		.bits  = {
			.pxx = IMX8MM_PCIE_SW_Pxx_REQ,
			.map = IMX8MM_PCIE_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_PCIE,
	},

	[IMX8MM_POWER_DOMAIN_OTG1] = {
		.genpd = {
			.name = "usb-otg1",
		},
		.bits  = {
			.pxx = IMX8MM_OTG1_SW_Pxx_REQ,
			.map = IMX8MM_OTG1_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_OTG1,
	},

	[IMX8MM_POWER_DOMAIN_OTG2] = {
		.genpd = {
			.name = "usb-otg2",
		},
		.bits  = {
			.pxx = IMX8MM_OTG2_SW_Pxx_REQ,
			.map = IMX8MM_OTG2_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_OTG2,
	},

	[IMX8MM_POWER_DOMAIN_GPUMIX] = {
		.genpd = {
			.name = "gpumix",
		},
		.bits  = {
			.pxx = IMX8MM_GPUMIX_SW_Pxx_REQ,
			.map = IMX8MM_GPUMIX_A53_DOMAIN,
			.hskreq = IMX8MM_GPUMIX_HSK_PWRDNREQN,
			.hskack = IMX8MM_GPUMIX_HSK_PWRDNACKN,
		},
		.pgc   = IMX8MM_PGC_GPUMIX,
	},

	[IMX8MM_POWER_DOMAIN_GPU] = {
		.genpd = {
			.name = "gpu",
		},
		.bits  = {
			.pxx = IMX8MM_GPU_SW_Pxx_REQ,
			.map = IMX8MM_GPU_A53_DOMAIN,
			.hskreq = IMX8MM_GPU_HSK_PWRDNREQN,
			.hskack = IMX8MM_GPU_HSK_PWRDNACKN,
		},
		.pgc   = IMX8MM_PGC_GPU2D,
	},

	[IMX8MM_POWER_DOMAIN_VPUMIX] = {
		.genpd = {
			.name = "vpumix",
		},
		.bits  = {
			.pxx = IMX8MM_VPUMIX_SW_Pxx_REQ,
			.map = IMX8MM_VPUMIX_A53_DOMAIN,
			.hskreq = IMX8MM_VPUMIX_HSK_PWRDNREQN,
			.hskack = IMX8MM_VPUMIX_HSK_PWRDNACKN,
		},
		.pgc   = IMX8MM_PGC_VPUMIX,
	},

	[IMX8MM_POWER_DOMAIN_VPUG1] = {
		.genpd = {
			.name = "vpu-g1",
		},
		.bits  = {
			.pxx = IMX8MM_VPUG1_SW_Pxx_REQ,
			.map = IMX8MM_VPUG1_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_VPUG1,
	},

	[IMX8MM_POWER_DOMAIN_VPUG2] = {
		.genpd = {
			.name = "vpu-g2",
		},
		.bits  = {
			.pxx = IMX8MM_VPUG2_SW_Pxx_REQ,
			.map = IMX8MM_VPUG2_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_VPUG2,
	},

	[IMX8MM_POWER_DOMAIN_VPUH1] = {
		.genpd = {
			.name = "vpu-h1",
		},
		.bits  = {
			.pxx = IMX8MM_VPUH1_SW_Pxx_REQ,
			.map = IMX8MM_VPUH1_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_VPUH1,
	},

	[IMX8MM_POWER_DOMAIN_DISPMIX] = {
		.genpd = {
			.name = "dispmix",
		},
		.bits  = {
			.pxx = IMX8MM_DISPMIX_SW_Pxx_REQ,
			.map = IMX8MM_DISPMIX_A53_DOMAIN,
			.hskreq = IMX8MM_DISPMIX_HSK_PWRDNREQN,
			.hskack = IMX8MM_DISPMIX_HSK_PWRDNACKN,
		},
		.pgc   = IMX8MM_PGC_DISPMIX,
	},

	[IMX8MM_POWER_DOMAIN_MIPI] = {
		.genpd = {
			.name = "mipi",
		},
		.bits  = {
			.pxx = IMX8MM_MIPI_SW_Pxx_REQ,
			.map = IMX8MM_MIPI_A53_DOMAIN,
		},
		.pgc   = IMX8MM_PGC_MIPI,
	},
};

static const struct regmap_range imx8mm_yes_ranges[] = {
		regmap_reg_range(GPC_LPCR_A_CORE_BSC,
				 GPC_PU_PWRHSK),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_MIPI),
				 GPC_PGC_SR(IMX8MM_PGC_MIPI)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_PCIE),
				 GPC_PGC_SR(IMX8MM_PGC_PCIE)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_OTG1),
				 GPC_PGC_SR(IMX8MM_PGC_OTG1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_OTG2),
				 GPC_PGC_SR(IMX8MM_PGC_OTG2)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_DDR1),
				 GPC_PGC_SR(IMX8MM_PGC_DDR1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_GPU2D),
				 GPC_PGC_SR(IMX8MM_PGC_GPU2D)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_GPUMIX),
				 GPC_PGC_SR(IMX8MM_PGC_GPUMIX)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_VPUMIX),
				 GPC_PGC_SR(IMX8MM_PGC_VPUMIX)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_GPU3D),
				 GPC_PGC_SR(IMX8MM_PGC_GPU3D)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_DISPMIX),
				 GPC_PGC_SR(IMX8MM_PGC_DISPMIX)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_VPUG1),
				 GPC_PGC_SR(IMX8MM_PGC_VPUG1)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_VPUG2),
				 GPC_PGC_SR(IMX8MM_PGC_VPUG2)),
		regmap_reg_range(GPC_PGC_CTRL(IMX8MM_PGC_VPUH1),
				 GPC_PGC_SR(IMX8MM_PGC_VPUH1)),
};

static const struct regmap_access_table imx8mm_access_table = {
	.yes_ranges	= imx8mm_yes_ranges,
	.n_yes_ranges	= ARRAY_SIZE(imx8mm_yes_ranges),
};

static const struct imx_pgc_domain_data imx8mm_pgc_domain_data = {
	.domains = imx8mm_pgc_domains,
	.domains_num = ARRAY_SIZE(imx8mm_pgc_domains),
	.reg_access_table = &imx8mm_access_table,
};

static const struct imx_pgc_domain imx8mn_pgc_domains[] = {
	[IMX8MN_POWER_DOMAIN_HSIOMIX] = {
		.genpd = {
			.name = "hsiomix",
		},
		.bits  = {
			.pxx = 0, /* no power sequence control */
			.map = 0, /* no power sequence control */
			.hskreq = IMX8MN_HSIO_HSK_PWRDNREQN,
			.hskack = IMX8MN_HSIO_HSK_PWRDNACKN,
		},
	},

	[IMX8MN_POWER_DOMAIN_OTG1] = {
		.genpd = {
			.name = "usb-otg1",
		},
		.bits  = {
			.pxx = IMX8MN_OTG1_SW_Pxx_REQ,
			.map = IMX8MN_OTG1_A53_DOMAIN,
		},
		.pgc   = IMX8MN_PGC_OTG1,
	},

	[IMX8MN_POWER_DOMAIN_GPUMIX] = {
		.genpd = {
			.name = "gpumix",
		},
		.bits  = {
			.pxx = IMX8MN_GPUMIX_SW_Pxx_REQ,
			.map = IMX8MN_GPUMIX_A53_DOMAIN,
			.hskreq = IMX8MN_GPUMIX_HSK_PWRDNREQN,
			.hskack = IMX8MN_GPUMIX_HSK_PWRDNACKN,
		},
		.pgc   = IMX8MN_PGC_GPUMIX,
	},
};

static const struct regmap_range imx8mn_yes_ranges[] = {
	regmap_reg_range(GPC_LPCR_A_CORE_BSC,
			 GPC_PU_PWRHSK),
	regmap_reg_range(GPC_PGC_CTRL(IMX8MN_PGC_MIPI),
			 GPC_PGC_SR(IMX8MN_PGC_MIPI)),
	regmap_reg_range(GPC_PGC_CTRL(IMX8MN_PGC_OTG1),
			 GPC_PGC_SR(IMX8MN_PGC_OTG1)),
	regmap_reg_range(GPC_PGC_CTRL(IMX8MN_PGC_DDR1),
			 GPC_PGC_SR(IMX8MN_PGC_DDR1)),
	regmap_reg_range(GPC_PGC_CTRL(IMX8MN_PGC_GPUMIX),
			 GPC_PGC_SR(IMX8MN_PGC_GPUMIX)),
	regmap_reg_range(GPC_PGC_CTRL(IMX8MN_PGC_DISPMIX),
			 GPC_PGC_SR(IMX8MN_PGC_DISPMIX)),
};

static const struct regmap_access_table imx8mn_access_table = {
	.yes_ranges	= imx8mn_yes_ranges,
	.n_yes_ranges	= ARRAY_SIZE(imx8mn_yes_ranges),
};

static const struct imx_pgc_domain_data imx8mn_pgc_domain_data = {
	.domains = imx8mn_pgc_domains,
	.domains_num = ARRAY_SIZE(imx8mn_pgc_domains),
	.reg_access_table = &imx8mn_access_table,
};

static int imx_pgc_domain_probe(struct platform_device *pdev)
{
	struct imx_pgc_domain *domain = pdev->dev.platform_data;
>>>>>>> upstream/android-13
	int ret;

	domain->dev = &pdev->dev;

	domain->regulator = devm_regulator_get_optional(domain->dev, "power");
	if (IS_ERR(domain->regulator)) {
<<<<<<< HEAD
		if (PTR_ERR(domain->regulator) != -ENODEV) {
			if (PTR_ERR(domain->regulator) != -EPROBE_DEFER)
				dev_err(domain->dev, "Failed to get domain's regulator\n");
			return PTR_ERR(domain->regulator);
		}
	} else {
=======
		if (PTR_ERR(domain->regulator) != -ENODEV)
			return dev_err_probe(domain->dev, PTR_ERR(domain->regulator),
					     "Failed to get domain's regulator\n");
	} else if (domain->voltage) {
>>>>>>> upstream/android-13
		regulator_set_voltage(domain->regulator,
				      domain->voltage, domain->voltage);
	}

<<<<<<< HEAD
	ret = pm_genpd_init(&domain->genpd, NULL, true);
	if (ret) {
		dev_err(domain->dev, "Failed to init power domain\n");
		return ret;
=======
	domain->num_clks = devm_clk_bulk_get_all(domain->dev, &domain->clks);
	if (domain->num_clks < 0)
		return dev_err_probe(domain->dev, domain->num_clks,
				     "Failed to get domain's clocks\n");

	domain->reset = devm_reset_control_array_get_optional_exclusive(domain->dev);
	if (IS_ERR(domain->reset))
		return dev_err_probe(domain->dev, PTR_ERR(domain->reset),
				     "Failed to get domain's resets\n");

	pm_runtime_enable(domain->dev);

	if (domain->bits.map)
		regmap_update_bits(domain->regmap, GPC_PGC_CPU_MAPPING,
				   domain->bits.map, domain->bits.map);

	ret = pm_genpd_init(&domain->genpd, NULL, true);
	if (ret) {
		dev_err(domain->dev, "Failed to init power domain\n");
		goto out_domain_unmap;
>>>>>>> upstream/android-13
	}

	ret = of_genpd_add_provider_simple(domain->dev->of_node,
					   &domain->genpd);
	if (ret) {
		dev_err(domain->dev, "Failed to add genpd provider\n");
<<<<<<< HEAD
		pm_genpd_remove(&domain->genpd);
	}

	return ret;
}

static int imx7_pgc_domain_remove(struct platform_device *pdev)
{
	struct imx7_pgc_domain *domain = pdev->dev.platform_data;
=======
		goto out_genpd_remove;
	}

	return 0;

out_genpd_remove:
	pm_genpd_remove(&domain->genpd);
out_domain_unmap:
	if (domain->bits.map)
		regmap_update_bits(domain->regmap, GPC_PGC_CPU_MAPPING,
				   domain->bits.map, 0);
	pm_runtime_disable(domain->dev);

	return ret;
}

static int imx_pgc_domain_remove(struct platform_device *pdev)
{
	struct imx_pgc_domain *domain = pdev->dev.platform_data;
>>>>>>> upstream/android-13

	of_genpd_del_provider(domain->dev->of_node);
	pm_genpd_remove(&domain->genpd);

<<<<<<< HEAD
	return 0;
}

static const struct platform_device_id imx7_pgc_domain_id[] = {
	{ "imx7-pgc-domain", },
	{ },
};

static struct platform_driver imx7_pgc_domain_driver = {
	.driver = {
		.name = "imx7-pgc",
	},
	.probe    = imx7_pgc_domain_probe,
	.remove   = imx7_pgc_domain_remove,
	.id_table = imx7_pgc_domain_id,
};
builtin_platform_driver(imx7_pgc_domain_driver)

static int imx_gpcv2_probe(struct platform_device *pdev)
{
	static const struct regmap_range yes_ranges[] = {
		regmap_reg_range(GPC_LPCR_A7_BSC,
				 GPC_M4_PU_PDN_FLG),
		regmap_reg_range(GPC_PGC_CTRL(PGC_MIPI),
				 GPC_PGC_SR(PGC_MIPI)),
		regmap_reg_range(GPC_PGC_CTRL(PGC_PCIE),
				 GPC_PGC_SR(PGC_PCIE)),
		regmap_reg_range(GPC_PGC_CTRL(PGC_USB_HSIC),
				 GPC_PGC_SR(PGC_USB_HSIC)),
	};
	static const struct regmap_access_table access_table = {
		.yes_ranges	= yes_ranges,
		.n_yes_ranges	= ARRAY_SIZE(yes_ranges),
	};
	static const struct regmap_config regmap_config = {
		.reg_bits	= 32,
		.val_bits	= 32,
		.reg_stride	= 4,
		.rd_table	= &access_table,
		.wr_table	= &access_table,
=======
	if (domain->bits.map)
		regmap_update_bits(domain->regmap, GPC_PGC_CPU_MAPPING,
				   domain->bits.map, 0);

	pm_runtime_disable(domain->dev);

	return 0;
}

static const struct platform_device_id imx_pgc_domain_id[] = {
	{ "imx-pgc-domain", },
	{ },
};

static struct platform_driver imx_pgc_domain_driver = {
	.driver = {
		.name = "imx-pgc",
	},
	.probe    = imx_pgc_domain_probe,
	.remove   = imx_pgc_domain_remove,
	.id_table = imx_pgc_domain_id,
};
builtin_platform_driver(imx_pgc_domain_driver)

static int imx_gpcv2_probe(struct platform_device *pdev)
{
	const struct imx_pgc_domain_data *domain_data =
			of_device_get_match_data(&pdev->dev);

	struct regmap_config regmap_config = {
		.reg_bits	= 32,
		.val_bits	= 32,
		.reg_stride	= 4,
		.rd_table	= domain_data->reg_access_table,
		.wr_table	= domain_data->reg_access_table,
>>>>>>> upstream/android-13
		.max_register   = SZ_4K,
	};
	struct device *dev = &pdev->dev;
	struct device_node *pgc_np, *np;
	struct regmap *regmap;
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	void __iomem *base;
	int ret;

	pgc_np = of_get_child_by_name(dev->of_node, "pgc");
	if (!pgc_np) {
		dev_err(dev, "No power domains specified in DT\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	res  = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(dev, res);
=======
	base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(base))
		return PTR_ERR(base);

	regmap = devm_regmap_init_mmio(dev, base, &regmap_config);
	if (IS_ERR(regmap)) {
		ret = PTR_ERR(regmap);
		dev_err(dev, "failed to init regmap (%d)\n", ret);
		return ret;
	}

	for_each_child_of_node(pgc_np, np) {
		struct platform_device *pd_pdev;
<<<<<<< HEAD
		struct imx7_pgc_domain *domain;
=======
		struct imx_pgc_domain *domain;
>>>>>>> upstream/android-13
		u32 domain_index;

		ret = of_property_read_u32(np, "reg", &domain_index);
		if (ret) {
			dev_err(dev, "Failed to read 'reg' property\n");
			of_node_put(np);
			return ret;
		}

<<<<<<< HEAD
		if (domain_index >= ARRAY_SIZE(imx7_pgc_domains)) {
=======
		if (domain_index >= domain_data->domains_num) {
>>>>>>> upstream/android-13
			dev_warn(dev,
				 "Domain index %d is out of bounds\n",
				 domain_index);
			continue;
		}

<<<<<<< HEAD
		pd_pdev = platform_device_alloc("imx7-pgc-domain",
=======
		pd_pdev = platform_device_alloc("imx-pgc-domain",
>>>>>>> upstream/android-13
						domain_index);
		if (!pd_pdev) {
			dev_err(dev, "Failed to allocate platform device\n");
			of_node_put(np);
			return -ENOMEM;
		}

		ret = platform_device_add_data(pd_pdev,
<<<<<<< HEAD
					       &imx7_pgc_domains[domain_index],
					       sizeof(imx7_pgc_domains[domain_index]));
=======
					       &domain_data->domains[domain_index],
					       sizeof(domain_data->domains[domain_index]));
>>>>>>> upstream/android-13
		if (ret) {
			platform_device_put(pd_pdev);
			of_node_put(np);
			return ret;
		}

		domain = pd_pdev->dev.platform_data;
		domain->regmap = regmap;
<<<<<<< HEAD
		domain->genpd.power_on  = imx7_gpc_pu_pgc_sw_pup_req;
		domain->genpd.power_off = imx7_gpc_pu_pgc_sw_pdn_req;
=======
		domain->genpd.power_on  = imx_pgc_power_up;
		domain->genpd.power_off = imx_pgc_power_down;
>>>>>>> upstream/android-13

		pd_pdev->dev.parent = dev;
		pd_pdev->dev.of_node = np;

		ret = platform_device_add(pd_pdev);
		if (ret) {
			platform_device_put(pd_pdev);
			of_node_put(np);
			return ret;
		}
	}

	return 0;
}

static const struct of_device_id imx_gpcv2_dt_ids[] = {
<<<<<<< HEAD
	{ .compatible = "fsl,imx7d-gpc" },
=======
	{ .compatible = "fsl,imx7d-gpc", .data = &imx7_pgc_domain_data, },
	{ .compatible = "fsl,imx8mm-gpc", .data = &imx8mm_pgc_domain_data, },
	{ .compatible = "fsl,imx8mn-gpc", .data = &imx8mn_pgc_domain_data, },
	{ .compatible = "fsl,imx8mq-gpc", .data = &imx8m_pgc_domain_data, },
>>>>>>> upstream/android-13
	{ }
};

static struct platform_driver imx_gpc_driver = {
	.driver = {
		.name = "imx-gpcv2",
		.of_match_table = imx_gpcv2_dt_ids,
	},
	.probe = imx_gpcv2_probe,
};
builtin_platform_driver(imx_gpc_driver)
