// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * PCIe host controller driver for Samsung EXYNOS SoCs
 *
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Author: Jingoo Han <jg1.han@samsung.com>
=======
 * PCIe host controller driver for Samsung Exynos SoCs
 *
 * Copyright (C) 2013-2020 Samsung Electronics Co., Ltd.
 *		https://www.samsung.com
 *
 * Author: Jingoo Han <jg1.han@samsung.com>
 *	   Jaehoon Chung <jh80.chung@samsung.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/gpio.h>
=======
>>>>>>> upstream/android-13
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of_device.h>
<<<<<<< HEAD
#include <linux/of_gpio.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/phy/phy.h>
#include <linux/resource.h>
#include <linux/signal.h>
#include <linux/types.h>
=======
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/phy/phy.h>
#include <linux/regulator/consumer.h>
>>>>>>> upstream/android-13

#include "pcie-designware.h"

#define to_exynos_pcie(x)	dev_get_drvdata((x)->dev)

/* PCIe ELBI registers */
#define PCIE_IRQ_PULSE			0x000
#define IRQ_INTA_ASSERT			BIT(0)
#define IRQ_INTB_ASSERT			BIT(2)
#define IRQ_INTC_ASSERT			BIT(4)
#define IRQ_INTD_ASSERT			BIT(6)
#define PCIE_IRQ_LEVEL			0x004
#define PCIE_IRQ_SPECIAL		0x008
#define PCIE_IRQ_EN_PULSE		0x00c
#define PCIE_IRQ_EN_LEVEL		0x010
<<<<<<< HEAD
#define IRQ_MSI_ENABLE			BIT(2)
#define PCIE_IRQ_EN_SPECIAL		0x014
#define PCIE_PWR_RESET			0x018
=======
#define PCIE_IRQ_EN_SPECIAL		0x014
#define PCIE_SW_WAKE			0x018
#define PCIE_BUS_EN			BIT(1)
>>>>>>> upstream/android-13
#define PCIE_CORE_RESET			0x01c
#define PCIE_CORE_RESET_ENABLE		BIT(0)
#define PCIE_STICKY_RESET		0x020
#define PCIE_NONSTICKY_RESET		0x024
#define PCIE_APP_INIT_RESET		0x028
#define PCIE_APP_LTSSM_ENABLE		0x02c
<<<<<<< HEAD
#define PCIE_ELBI_RDLH_LINKUP		0x064
=======
#define PCIE_ELBI_RDLH_LINKUP		0x074
#define PCIE_ELBI_XMLH_LINKUP		BIT(4)
>>>>>>> upstream/android-13
#define PCIE_ELBI_LTSSM_ENABLE		0x1
#define PCIE_ELBI_SLV_AWMISC		0x11c
#define PCIE_ELBI_SLV_ARMISC		0x120
#define PCIE_ELBI_SLV_DBI_ENABLE	BIT(21)

<<<<<<< HEAD
struct exynos_pcie_mem_res {
	void __iomem *elbi_base;   /* DT 0th resource: PCIe CTRL */
};

struct exynos_pcie_clk_res {
	struct clk *clk;
	struct clk *bus_clk;
};

struct exynos_pcie {
	struct dw_pcie			*pci;
	struct exynos_pcie_mem_res	*mem_res;
	struct exynos_pcie_clk_res	*clk_res;
	const struct exynos_pcie_ops	*ops;
	int				reset_gpio;

	struct phy			*phy;
};

struct exynos_pcie_ops {
	int (*get_mem_resources)(struct platform_device *pdev,
			struct exynos_pcie *ep);
	int (*get_clk_resources)(struct exynos_pcie *ep);
	int (*init_clk_resources)(struct exynos_pcie *ep);
	void (*deinit_clk_resources)(struct exynos_pcie *ep);
};

static int exynos5440_pcie_get_mem_resources(struct platform_device *pdev,
					     struct exynos_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct device *dev = pci->dev;
	struct resource *res;

	ep->mem_res = devm_kzalloc(dev, sizeof(*ep->mem_res), GFP_KERNEL);
	if (!ep->mem_res)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ep->mem_res->elbi_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(ep->mem_res->elbi_base))
		return PTR_ERR(ep->mem_res->elbi_base);

	return 0;
}

static int exynos5440_pcie_get_clk_resources(struct exynos_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct device *dev = pci->dev;

	ep->clk_res = devm_kzalloc(dev, sizeof(*ep->clk_res), GFP_KERNEL);
	if (!ep->clk_res)
		return -ENOMEM;

	ep->clk_res->clk = devm_clk_get(dev, "pcie");
	if (IS_ERR(ep->clk_res->clk)) {
		dev_err(dev, "Failed to get pcie rc clock\n");
		return PTR_ERR(ep->clk_res->clk);
	}

	ep->clk_res->bus_clk = devm_clk_get(dev, "pcie_bus");
	if (IS_ERR(ep->clk_res->bus_clk)) {
		dev_err(dev, "Failed to get pcie bus clock\n");
		return PTR_ERR(ep->clk_res->bus_clk);
	}

	return 0;
}

static int exynos5440_pcie_init_clk_resources(struct exynos_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct device *dev = pci->dev;
	int ret;

	ret = clk_prepare_enable(ep->clk_res->clk);
=======
struct exynos_pcie {
	struct dw_pcie			pci;
	void __iomem			*elbi_base;
	struct clk			*clk;
	struct clk			*bus_clk;
	struct phy			*phy;
	struct regulator_bulk_data	supplies[2];
};

static int exynos_pcie_init_clk_resources(struct exynos_pcie *ep)
{
	struct device *dev = ep->pci.dev;
	int ret;

	ret = clk_prepare_enable(ep->clk);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "cannot enable pcie rc clock");
		return ret;
	}

<<<<<<< HEAD
	ret = clk_prepare_enable(ep->clk_res->bus_clk);
=======
	ret = clk_prepare_enable(ep->bus_clk);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "cannot enable pcie bus clock");
		goto err_bus_clk;
	}

	return 0;

err_bus_clk:
<<<<<<< HEAD
	clk_disable_unprepare(ep->clk_res->clk);
=======
	clk_disable_unprepare(ep->clk);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void exynos5440_pcie_deinit_clk_resources(struct exynos_pcie *ep)
{
	clk_disable_unprepare(ep->clk_res->bus_clk);
	clk_disable_unprepare(ep->clk_res->clk);
}

static const struct exynos_pcie_ops exynos5440_pcie_ops = {
	.get_mem_resources	= exynos5440_pcie_get_mem_resources,
	.get_clk_resources	= exynos5440_pcie_get_clk_resources,
	.init_clk_resources	= exynos5440_pcie_init_clk_resources,
	.deinit_clk_resources	= exynos5440_pcie_deinit_clk_resources,
};

=======
static void exynos_pcie_deinit_clk_resources(struct exynos_pcie *ep)
{
	clk_disable_unprepare(ep->bus_clk);
	clk_disable_unprepare(ep->clk);
}

>>>>>>> upstream/android-13
static void exynos_pcie_writel(void __iomem *base, u32 val, u32 reg)
{
	writel(val, base + reg);
}

static u32 exynos_pcie_readl(void __iomem *base, u32 reg)
{
	return readl(base + reg);
}

static void exynos_pcie_sideband_dbi_w_mode(struct exynos_pcie *ep, bool on)
{
	u32 val;

<<<<<<< HEAD
	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_ELBI_SLV_AWMISC);
=======
	val = exynos_pcie_readl(ep->elbi_base, PCIE_ELBI_SLV_AWMISC);
>>>>>>> upstream/android-13
	if (on)
		val |= PCIE_ELBI_SLV_DBI_ENABLE;
	else
		val &= ~PCIE_ELBI_SLV_DBI_ENABLE;
<<<<<<< HEAD
	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_ELBI_SLV_AWMISC);
=======
	exynos_pcie_writel(ep->elbi_base, val, PCIE_ELBI_SLV_AWMISC);
>>>>>>> upstream/android-13
}

static void exynos_pcie_sideband_dbi_r_mode(struct exynos_pcie *ep, bool on)
{
	u32 val;

<<<<<<< HEAD
	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_ELBI_SLV_ARMISC);
=======
	val = exynos_pcie_readl(ep->elbi_base, PCIE_ELBI_SLV_ARMISC);
>>>>>>> upstream/android-13
	if (on)
		val |= PCIE_ELBI_SLV_DBI_ENABLE;
	else
		val &= ~PCIE_ELBI_SLV_DBI_ENABLE;
<<<<<<< HEAD
	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_ELBI_SLV_ARMISC);
=======
	exynos_pcie_writel(ep->elbi_base, val, PCIE_ELBI_SLV_ARMISC);
>>>>>>> upstream/android-13
}

static void exynos_pcie_assert_core_reset(struct exynos_pcie *ep)
{
	u32 val;

<<<<<<< HEAD
	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_CORE_RESET);
	val &= ~PCIE_CORE_RESET_ENABLE;
	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_CORE_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 0, PCIE_PWR_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 0, PCIE_STICKY_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 0, PCIE_NONSTICKY_RESET);
=======
	val = exynos_pcie_readl(ep->elbi_base, PCIE_CORE_RESET);
	val &= ~PCIE_CORE_RESET_ENABLE;
	exynos_pcie_writel(ep->elbi_base, val, PCIE_CORE_RESET);
	exynos_pcie_writel(ep->elbi_base, 0, PCIE_STICKY_RESET);
	exynos_pcie_writel(ep->elbi_base, 0, PCIE_NONSTICKY_RESET);
>>>>>>> upstream/android-13
}

static void exynos_pcie_deassert_core_reset(struct exynos_pcie *ep)
{
	u32 val;

<<<<<<< HEAD
	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_CORE_RESET);
	val |= PCIE_CORE_RESET_ENABLE;

	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_CORE_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 1, PCIE_STICKY_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 1, PCIE_NONSTICKY_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 1, PCIE_APP_INIT_RESET);
	exynos_pcie_writel(ep->mem_res->elbi_base, 0, PCIE_APP_INIT_RESET);
}

static void exynos_pcie_assert_reset(struct exynos_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct device *dev = pci->dev;

	if (ep->reset_gpio >= 0)
		devm_gpio_request_one(dev, ep->reset_gpio,
				GPIOF_OUT_INIT_HIGH, "RESET");
}

static int exynos_pcie_establish_link(struct exynos_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct pcie_port *pp = &pci->pp;
	struct device *dev = pci->dev;

	if (dw_pcie_link_up(pci)) {
		dev_err(dev, "Link already up\n");
		return 0;
	}

	exynos_pcie_assert_core_reset(ep);

	phy_reset(ep->phy);

	exynos_pcie_writel(ep->mem_res->elbi_base, 1,
			PCIE_PWR_RESET);

	phy_power_on(ep->phy);
	phy_init(ep->phy);

	exynos_pcie_deassert_core_reset(ep);
	dw_pcie_setup_rc(pp);
	exynos_pcie_assert_reset(ep);

	/* assert LTSSM enable */
	exynos_pcie_writel(ep->mem_res->elbi_base, PCIE_ELBI_LTSSM_ENABLE,
			  PCIE_APP_LTSSM_ENABLE);

	/* check if the link is up or not */
	if (!dw_pcie_wait_for_link(pci))
		return 0;

	phy_power_off(ep->phy);
	return -ETIMEDOUT;
=======
	val = exynos_pcie_readl(ep->elbi_base, PCIE_CORE_RESET);
	val |= PCIE_CORE_RESET_ENABLE;

	exynos_pcie_writel(ep->elbi_base, val, PCIE_CORE_RESET);
	exynos_pcie_writel(ep->elbi_base, 1, PCIE_STICKY_RESET);
	exynos_pcie_writel(ep->elbi_base, 1, PCIE_NONSTICKY_RESET);
	exynos_pcie_writel(ep->elbi_base, 1, PCIE_APP_INIT_RESET);
	exynos_pcie_writel(ep->elbi_base, 0, PCIE_APP_INIT_RESET);
}

static int exynos_pcie_start_link(struct dw_pcie *pci)
{
	struct exynos_pcie *ep = to_exynos_pcie(pci);
	u32 val;

	val = exynos_pcie_readl(ep->elbi_base, PCIE_SW_WAKE);
	val &= ~PCIE_BUS_EN;
	exynos_pcie_writel(ep->elbi_base, val, PCIE_SW_WAKE);

	/* assert LTSSM enable */
	exynos_pcie_writel(ep->elbi_base, PCIE_ELBI_LTSSM_ENABLE,
			  PCIE_APP_LTSSM_ENABLE);
	return 0;
>>>>>>> upstream/android-13
}

static void exynos_pcie_clear_irq_pulse(struct exynos_pcie *ep)
{
<<<<<<< HEAD
	u32 val;

	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_IRQ_PULSE);
	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_IRQ_PULSE);
}

static void exynos_pcie_enable_irq_pulse(struct exynos_pcie *ep)
{
	u32 val;

	/* enable INTX interrupt */
	val = IRQ_INTA_ASSERT | IRQ_INTB_ASSERT |
		IRQ_INTC_ASSERT | IRQ_INTD_ASSERT;
	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_IRQ_EN_PULSE);
=======
	u32 val = exynos_pcie_readl(ep->elbi_base, PCIE_IRQ_PULSE);

	exynos_pcie_writel(ep->elbi_base, val, PCIE_IRQ_PULSE);
>>>>>>> upstream/android-13
}

static irqreturn_t exynos_pcie_irq_handler(int irq, void *arg)
{
	struct exynos_pcie *ep = arg;

	exynos_pcie_clear_irq_pulse(ep);
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void exynos_pcie_msi_init(struct exynos_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct pcie_port *pp = &pci->pp;
	u32 val;

	dw_pcie_msi_init(pp);

	/* enable MSI interrupt */
	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_IRQ_EN_LEVEL);
	val |= IRQ_MSI_ENABLE;
	exynos_pcie_writel(ep->mem_res->elbi_base, val, PCIE_IRQ_EN_LEVEL);
}

static void exynos_pcie_enable_interrupts(struct exynos_pcie *ep)
{
	exynos_pcie_enable_irq_pulse(ep);

	if (IS_ENABLED(CONFIG_PCI_MSI))
		exynos_pcie_msi_init(ep);
=======
static void exynos_pcie_enable_irq_pulse(struct exynos_pcie *ep)
{
	u32 val = IRQ_INTA_ASSERT | IRQ_INTB_ASSERT |
		  IRQ_INTC_ASSERT | IRQ_INTD_ASSERT;

	exynos_pcie_writel(ep->elbi_base, val, PCIE_IRQ_EN_PULSE);
	exynos_pcie_writel(ep->elbi_base, 0, PCIE_IRQ_EN_LEVEL);
	exynos_pcie_writel(ep->elbi_base, 0, PCIE_IRQ_EN_SPECIAL);
>>>>>>> upstream/android-13
}

static u32 exynos_pcie_read_dbi(struct dw_pcie *pci, void __iomem *base,
				u32 reg, size_t size)
{
	struct exynos_pcie *ep = to_exynos_pcie(pci);
	u32 val;

	exynos_pcie_sideband_dbi_r_mode(ep, true);
	dw_pcie_read(base + reg, size, &val);
	exynos_pcie_sideband_dbi_r_mode(ep, false);
	return val;
}

static void exynos_pcie_write_dbi(struct dw_pcie *pci, void __iomem *base,
				  u32 reg, size_t size, u32 val)
{
	struct exynos_pcie *ep = to_exynos_pcie(pci);

	exynos_pcie_sideband_dbi_w_mode(ep, true);
	dw_pcie_write(base + reg, size, val);
	exynos_pcie_sideband_dbi_w_mode(ep, false);
}

<<<<<<< HEAD
static int exynos_pcie_rd_own_conf(struct pcie_port *pp, int where, int size,
				u32 *val)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct exynos_pcie *ep = to_exynos_pcie(pci);
	int ret;

	exynos_pcie_sideband_dbi_r_mode(ep, true);
	ret = dw_pcie_read(pci->dbi_base + where, size, val);
	exynos_pcie_sideband_dbi_r_mode(ep, false);
	return ret;
}

static int exynos_pcie_wr_own_conf(struct pcie_port *pp, int where, int size,
				u32 val)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct exynos_pcie *ep = to_exynos_pcie(pci);
	int ret;

	exynos_pcie_sideband_dbi_w_mode(ep, true);
	ret = dw_pcie_write(pci->dbi_base + where, size, val);
	exynos_pcie_sideband_dbi_w_mode(ep, false);
	return ret;
}

static int exynos_pcie_link_up(struct dw_pcie *pci)
{
	struct exynos_pcie *ep = to_exynos_pcie(pci);
	u32 val;

	val = exynos_pcie_readl(ep->mem_res->elbi_base, PCIE_ELBI_RDLH_LINKUP);
	if (val == PCIE_ELBI_LTSSM_ENABLE)
		return 1;

	return 0;
=======
static int exynos_pcie_rd_own_conf(struct pci_bus *bus, unsigned int devfn,
				   int where, int size, u32 *val)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(bus->sysdata);

	if (PCI_SLOT(devfn)) {
		*val = ~0;
		return PCIBIOS_DEVICE_NOT_FOUND;
	}

	*val = dw_pcie_read_dbi(pci, where, size);
	return PCIBIOS_SUCCESSFUL;
}

static int exynos_pcie_wr_own_conf(struct pci_bus *bus, unsigned int devfn,
				   int where, int size, u32 val)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(bus->sysdata);

	if (PCI_SLOT(devfn))
		return PCIBIOS_DEVICE_NOT_FOUND;

	dw_pcie_write_dbi(pci, where, size, val);
	return PCIBIOS_SUCCESSFUL;
}

static struct pci_ops exynos_pci_ops = {
	.read = exynos_pcie_rd_own_conf,
	.write = exynos_pcie_wr_own_conf,
};

static int exynos_pcie_link_up(struct dw_pcie *pci)
{
	struct exynos_pcie *ep = to_exynos_pcie(pci);
	u32 val = exynos_pcie_readl(ep->elbi_base, PCIE_ELBI_RDLH_LINKUP);

	return (val & PCIE_ELBI_XMLH_LINKUP);
>>>>>>> upstream/android-13
}

static int exynos_pcie_host_init(struct pcie_port *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct exynos_pcie *ep = to_exynos_pcie(pci);

<<<<<<< HEAD
	exynos_pcie_establish_link(ep);
	exynos_pcie_enable_interrupts(ep);
=======
	pp->bridge->ops = &exynos_pci_ops;

	exynos_pcie_assert_core_reset(ep);

	phy_reset(ep->phy);
	phy_power_on(ep->phy);
	phy_init(ep->phy);

	exynos_pcie_deassert_core_reset(ep);
	exynos_pcie_enable_irq_pulse(ep);
>>>>>>> upstream/android-13

	return 0;
}

static const struct dw_pcie_host_ops exynos_pcie_host_ops = {
<<<<<<< HEAD
	.rd_own_conf = exynos_pcie_rd_own_conf,
	.wr_own_conf = exynos_pcie_wr_own_conf,
	.host_init = exynos_pcie_host_init,
};

static int __init exynos_add_pcie_port(struct exynos_pcie *ep,
				       struct platform_device *pdev)
{
	struct dw_pcie *pci = ep->pci;
=======
	.host_init = exynos_pcie_host_init,
};

static int exynos_add_pcie_port(struct exynos_pcie *ep,
				       struct platform_device *pdev)
{
	struct dw_pcie *pci = &ep->pci;
>>>>>>> upstream/android-13
	struct pcie_port *pp = &pci->pp;
	struct device *dev = &pdev->dev;
	int ret;

<<<<<<< HEAD
	pp->irq = platform_get_irq(pdev, 1);
	if (pp->irq < 0) {
		dev_err(dev, "failed to get irq\n");
		return pp->irq;
	}
	ret = devm_request_irq(dev, pp->irq, exynos_pcie_irq_handler,
				IRQF_SHARED, "exynos-pcie", ep);
=======
	pp->irq = platform_get_irq(pdev, 0);
	if (pp->irq < 0)
		return pp->irq;

	ret = devm_request_irq(dev, pp->irq, exynos_pcie_irq_handler,
			       IRQF_SHARED, "exynos-pcie", ep);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "failed to request irq\n");
		return ret;
	}

<<<<<<< HEAD
	if (IS_ENABLED(CONFIG_PCI_MSI)) {
		pp->msi_irq = platform_get_irq(pdev, 0);
		if (pp->msi_irq < 0) {
			dev_err(dev, "failed to get msi irq\n");
			return pp->msi_irq;
		}
	}

	pp->ops = &exynos_pcie_host_ops;
=======
	pp->ops = &exynos_pcie_host_ops;
	pp->msi_irq = -ENODEV;
>>>>>>> upstream/android-13

	ret = dw_pcie_host_init(pp);
	if (ret) {
		dev_err(dev, "failed to initialize host\n");
		return ret;
	}

	return 0;
}

static const struct dw_pcie_ops dw_pcie_ops = {
	.read_dbi = exynos_pcie_read_dbi,
	.write_dbi = exynos_pcie_write_dbi,
	.link_up = exynos_pcie_link_up,
<<<<<<< HEAD
};

static int __init exynos_pcie_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci;
=======
	.start_link = exynos_pcie_start_link,
};

static int exynos_pcie_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
>>>>>>> upstream/android-13
	struct exynos_pcie *ep;
	struct device_node *np = dev->of_node;
	int ret;

	ep = devm_kzalloc(dev, sizeof(*ep), GFP_KERNEL);
	if (!ep)
		return -ENOMEM;

<<<<<<< HEAD
	pci = devm_kzalloc(dev, sizeof(*pci), GFP_KERNEL);
	if (!pci)
		return -ENOMEM;

	pci->dev = dev;
	pci->ops = &dw_pcie_ops;

	ep->pci = pci;
	ep->ops = (const struct exynos_pcie_ops *)
		of_device_get_match_data(dev);

	ep->reset_gpio = of_get_named_gpio(np, "reset-gpio", 0);

	ep->phy = devm_of_phy_get(dev, np, NULL);
	if (IS_ERR(ep->phy)) {
		if (PTR_ERR(ep->phy) != -ENODEV)
			return PTR_ERR(ep->phy);

		ep->phy = NULL;
	}

	if (ep->ops && ep->ops->get_mem_resources) {
		ret = ep->ops->get_mem_resources(pdev, ep);
		if (ret)
			return ret;
	}

	if (ep->ops && ep->ops->get_clk_resources &&
			ep->ops->init_clk_resources) {
		ret = ep->ops->get_clk_resources(ep);
		if (ret)
			return ret;
		ret = ep->ops->init_clk_resources(ep);
		if (ret)
			return ret;
	}
=======
	ep->pci.dev = dev;
	ep->pci.ops = &dw_pcie_ops;

	ep->phy = devm_of_phy_get(dev, np, NULL);
	if (IS_ERR(ep->phy))
		return PTR_ERR(ep->phy);

	/* External Local Bus interface (ELBI) registers */
	ep->elbi_base = devm_platform_ioremap_resource_byname(pdev, "elbi");
	if (IS_ERR(ep->elbi_base))
		return PTR_ERR(ep->elbi_base);

	ep->clk = devm_clk_get(dev, "pcie");
	if (IS_ERR(ep->clk)) {
		dev_err(dev, "Failed to get pcie rc clock\n");
		return PTR_ERR(ep->clk);
	}

	ep->bus_clk = devm_clk_get(dev, "pcie_bus");
	if (IS_ERR(ep->bus_clk)) {
		dev_err(dev, "Failed to get pcie bus clock\n");
		return PTR_ERR(ep->bus_clk);
	}

	ep->supplies[0].supply = "vdd18";
	ep->supplies[1].supply = "vdd10";
	ret = devm_regulator_bulk_get(dev, ARRAY_SIZE(ep->supplies),
				      ep->supplies);
	if (ret)
		return ret;

	ret = exynos_pcie_init_clk_resources(ep);
	if (ret)
		return ret;

	ret = regulator_bulk_enable(ARRAY_SIZE(ep->supplies), ep->supplies);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, ep);

	ret = exynos_add_pcie_port(ep, pdev);
	if (ret < 0)
		goto fail_probe;

	return 0;

fail_probe:
	phy_exit(ep->phy);
<<<<<<< HEAD

	if (ep->ops && ep->ops->deinit_clk_resources)
		ep->ops->deinit_clk_resources(ep);
=======
	exynos_pcie_deinit_clk_resources(ep);
	regulator_bulk_disable(ARRAY_SIZE(ep->supplies), ep->supplies);

>>>>>>> upstream/android-13
	return ret;
}

static int __exit exynos_pcie_remove(struct platform_device *pdev)
{
	struct exynos_pcie *ep = platform_get_drvdata(pdev);

<<<<<<< HEAD
	if (ep->ops && ep->ops->deinit_clk_resources)
		ep->ops->deinit_clk_resources(ep);
=======
	dw_pcie_host_deinit(&ep->pci.pp);
	exynos_pcie_assert_core_reset(ep);
	phy_power_off(ep->phy);
	phy_exit(ep->phy);
	exynos_pcie_deinit_clk_resources(ep);
	regulator_bulk_disable(ARRAY_SIZE(ep->supplies), ep->supplies);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static const struct of_device_id exynos_pcie_of_match[] = {
	{
		.compatible = "samsung,exynos5440-pcie",
		.data = &exynos5440_pcie_ops
	},
	{},
};

static struct platform_driver exynos_pcie_driver = {
=======
static int __maybe_unused exynos_pcie_suspend_noirq(struct device *dev)
{
	struct exynos_pcie *ep = dev_get_drvdata(dev);

	exynos_pcie_assert_core_reset(ep);
	phy_power_off(ep->phy);
	phy_exit(ep->phy);
	regulator_bulk_disable(ARRAY_SIZE(ep->supplies), ep->supplies);

	return 0;
}

static int __maybe_unused exynos_pcie_resume_noirq(struct device *dev)
{
	struct exynos_pcie *ep = dev_get_drvdata(dev);
	struct dw_pcie *pci = &ep->pci;
	struct pcie_port *pp = &pci->pp;
	int ret;

	ret = regulator_bulk_enable(ARRAY_SIZE(ep->supplies), ep->supplies);
	if (ret)
		return ret;

	/* exynos_pcie_host_init controls ep->phy */
	exynos_pcie_host_init(pp);
	dw_pcie_setup_rc(pp);
	exynos_pcie_start_link(pci);
	return dw_pcie_wait_for_link(pci);
}

static const struct dev_pm_ops exynos_pcie_pm_ops = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(exynos_pcie_suspend_noirq,
				      exynos_pcie_resume_noirq)
};

static const struct of_device_id exynos_pcie_of_match[] = {
	{ .compatible = "samsung,exynos5433-pcie", },
	{ },
};

static struct platform_driver exynos_pcie_driver = {
	.probe		= exynos_pcie_probe,
>>>>>>> upstream/android-13
	.remove		= __exit_p(exynos_pcie_remove),
	.driver = {
		.name	= "exynos-pcie",
		.of_match_table = exynos_pcie_of_match,
<<<<<<< HEAD
	},
};

/* Exynos PCIe driver does not allow module unload */

static int __init exynos_pcie_init(void)
{
	return platform_driver_probe(&exynos_pcie_driver, exynos_pcie_probe);
}
subsys_initcall(exynos_pcie_init);
=======
		.pm		= &exynos_pcie_pm_ops,
	},
};
module_platform_driver(exynos_pcie_driver);
MODULE_LICENSE("GPL v2");
MODULE_DEVICE_TABLE(of, exynos_pcie_of_match);
>>>>>>> upstream/android-13
