<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*******************************************************************************
  STMMAC Ethernet Driver -- MDIO bus implementation
  Provides Bus interface for MII registers

  Copyright (C) 2007-2009  STMicroelectronics Ltd

<<<<<<< HEAD
  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".
=======
>>>>>>> upstream/android-13

  Author: Carl Shaw <carl.shaw@st.com>
  Maintainer: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

<<<<<<< HEAD
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/mii.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_mdio.h>
#include <linux/phy.h>
=======
#include <linux/gpio/consumer.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/mii.h>
#include <linux/of_mdio.h>
#include <linux/pm_runtime.h>
#include <linux/phy.h>
#include <linux/property.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>

#include "dwxgmac2.h"
#include "stmmac.h"

#define MII_BUSY 0x00000001
#define MII_WRITE 0x00000002
<<<<<<< HEAD

/* GMAC4 defines */
#define MII_GMAC4_GOC_SHIFT		2
#define MII_GMAC4_WRITE			(1 << MII_GMAC4_GOC_SHIFT)
#define MII_GMAC4_READ			(3 << MII_GMAC4_GOC_SHIFT)
=======
#define MII_DATA_MASK GENMASK(15, 0)

/* GMAC4 defines */
#define MII_GMAC4_GOC_SHIFT		2
#define MII_GMAC4_REG_ADDR_SHIFT	16
#define MII_GMAC4_WRITE			(1 << MII_GMAC4_GOC_SHIFT)
#define MII_GMAC4_READ			(3 << MII_GMAC4_GOC_SHIFT)
#define MII_GMAC4_C45E			BIT(1)
>>>>>>> upstream/android-13

/* XGMAC defines */
#define MII_XGMAC_SADDR			BIT(18)
#define MII_XGMAC_CMD_SHIFT		16
#define MII_XGMAC_WRITE			(1 << MII_XGMAC_CMD_SHIFT)
#define MII_XGMAC_READ			(3 << MII_XGMAC_CMD_SHIFT)
#define MII_XGMAC_BUSY			BIT(22)
#define MII_XGMAC_MAX_C22ADDR		3
#define MII_XGMAC_C22P_MASK		GENMASK(MII_XGMAC_MAX_C22ADDR, 0)
<<<<<<< HEAD
=======
#define MII_XGMAC_PA_SHIFT		16
#define MII_XGMAC_DA_SHIFT		21

static int stmmac_xgmac2_c45_format(struct stmmac_priv *priv, int phyaddr,
				    int phyreg, u32 *hw_addr)
{
	u32 tmp;

	/* Set port as Clause 45 */
	tmp = readl(priv->ioaddr + XGMAC_MDIO_C22P);
	tmp &= ~BIT(phyaddr);
	writel(tmp, priv->ioaddr + XGMAC_MDIO_C22P);

	*hw_addr = (phyaddr << MII_XGMAC_PA_SHIFT) | (phyreg & 0xffff);
	*hw_addr |= (phyreg >> MII_DEVADDR_C45_SHIFT) << MII_XGMAC_DA_SHIFT;
	return 0;
}
>>>>>>> upstream/android-13

static int stmmac_xgmac2_c22_format(struct stmmac_priv *priv, int phyaddr,
				    int phyreg, u32 *hw_addr)
{
<<<<<<< HEAD
	unsigned int mii_data = priv->hw->mii.data;
=======
>>>>>>> upstream/android-13
	u32 tmp;

	/* HW does not support C22 addr >= 4 */
	if (phyaddr > MII_XGMAC_MAX_C22ADDR)
		return -ENODEV;
<<<<<<< HEAD
	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
			       !(tmp & MII_XGMAC_BUSY), 100, 10000))
		return -EBUSY;
=======
>>>>>>> upstream/android-13

	/* Set port as Clause 22 */
	tmp = readl(priv->ioaddr + XGMAC_MDIO_C22P);
	tmp &= ~MII_XGMAC_C22P_MASK;
	tmp |= BIT(phyaddr);
	writel(tmp, priv->ioaddr + XGMAC_MDIO_C22P);

<<<<<<< HEAD
	*hw_addr = (phyaddr << 16) | (phyreg & 0x1f);
=======
	*hw_addr = (phyaddr << MII_XGMAC_PA_SHIFT) | (phyreg & 0x1f);
>>>>>>> upstream/android-13
	return 0;
}

static int stmmac_xgmac2_mdio_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 tmp, addr, value = MII_XGMAC_BUSY;
	int ret;

<<<<<<< HEAD
	if (phyreg & MII_ADDR_C45) {
		return -EOPNOTSUPP;
	} else {
		ret = stmmac_xgmac2_c22_format(priv, phyaddr, phyreg, &addr);
		if (ret)
			return ret;
=======
	ret = pm_runtime_get_sync(priv->device);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->device);
		return ret;
	}

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
			       !(tmp & MII_XGMAC_BUSY), 100, 10000)) {
		ret = -EBUSY;
		goto err_disable_clks;
	}

	if (phyreg & MII_ADDR_C45) {
		phyreg &= ~MII_ADDR_C45;

		ret = stmmac_xgmac2_c45_format(priv, phyaddr, phyreg, &addr);
		if (ret)
			goto err_disable_clks;
	} else {
		ret = stmmac_xgmac2_c22_format(priv, phyaddr, phyreg, &addr);
		if (ret)
			goto err_disable_clks;

		value |= MII_XGMAC_SADDR;
>>>>>>> upstream/android-13
	}

	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
<<<<<<< HEAD
	value |= MII_XGMAC_SADDR | MII_XGMAC_READ;

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
			       !(tmp & MII_XGMAC_BUSY), 100, 10000))
		return -EBUSY;
=======
	value |= MII_XGMAC_READ;

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
			       !(tmp & MII_XGMAC_BUSY), 100, 10000)) {
		ret = -EBUSY;
		goto err_disable_clks;
	}
>>>>>>> upstream/android-13

	/* Set the MII address register to read */
	writel(addr, priv->ioaddr + mii_address);
	writel(value, priv->ioaddr + mii_data);

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
<<<<<<< HEAD
			       !(tmp & MII_XGMAC_BUSY), 100, 10000))
		return -EBUSY;

	/* Read the data from the MII data register */
	return readl(priv->ioaddr + mii_data) & GENMASK(15, 0);
=======
			       !(tmp & MII_XGMAC_BUSY), 100, 10000)) {
		ret = -EBUSY;
		goto err_disable_clks;
	}

	/* Read the data from the MII data register */
	ret = (int)readl(priv->ioaddr + mii_data) & GENMASK(15, 0);

err_disable_clks:
	pm_runtime_put(priv->device);

	return ret;
>>>>>>> upstream/android-13
}

static int stmmac_xgmac2_mdio_write(struct mii_bus *bus, int phyaddr,
				    int phyreg, u16 phydata)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 addr, tmp, value = MII_XGMAC_BUSY;
	int ret;

<<<<<<< HEAD
	if (phyreg & MII_ADDR_C45) {
		return -EOPNOTSUPP;
	} else {
		ret = stmmac_xgmac2_c22_format(priv, phyaddr, phyreg, &addr);
		if (ret)
			return ret;
=======
	ret = pm_runtime_get_sync(priv->device);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->device);
		return ret;
	}

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
			       !(tmp & MII_XGMAC_BUSY), 100, 10000)) {
		ret = -EBUSY;
		goto err_disable_clks;
	}

	if (phyreg & MII_ADDR_C45) {
		phyreg &= ~MII_ADDR_C45;

		ret = stmmac_xgmac2_c45_format(priv, phyaddr, phyreg, &addr);
		if (ret)
			goto err_disable_clks;
	} else {
		ret = stmmac_xgmac2_c22_format(priv, phyaddr, phyreg, &addr);
		if (ret)
			goto err_disable_clks;

		value |= MII_XGMAC_SADDR;
>>>>>>> upstream/android-13
	}

	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
<<<<<<< HEAD
	value |= phydata | MII_XGMAC_SADDR;
=======
	value |= phydata;
>>>>>>> upstream/android-13
	value |= MII_XGMAC_WRITE;

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_data, tmp,
<<<<<<< HEAD
			       !(tmp & MII_XGMAC_BUSY), 100, 10000))
		return -EBUSY;
=======
			       !(tmp & MII_XGMAC_BUSY), 100, 10000)) {
		ret = -EBUSY;
		goto err_disable_clks;
	}
>>>>>>> upstream/android-13

	/* Set the MII address register to write */
	writel(addr, priv->ioaddr + mii_address);
	writel(value, priv->ioaddr + mii_data);

	/* Wait until any existing MII operation is complete */
<<<<<<< HEAD
	return readl_poll_timeout(priv->ioaddr + mii_data, tmp,
				  !(tmp & MII_XGMAC_BUSY), 100, 10000);
}

/**
 * stmmac_c22_read
 * @bus: points to the mii_bus structure
 * @phyaddr: clause 22 phy address
 * @phyreg: clause 22 phy register
 * Description: it reads data from the MII register follow clause 22.
 * For the 7111 GMAC, we must set the bit 0 in the MII address register while
 * accessing the PHY registers.
 * Fortunately, it seems this has no drawback for the 7109 MAC.
 */
static int stmmac_c22_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 v;
	int data;
	u32 value = MII_BUSY;

	value |= (phyaddr << priv->hw->mii.addr_shift)
		& priv->hw->mii.addr_mask;
	value |= (phyreg << priv->hw->mii.reg_shift) & priv->hw->mii.reg_mask;
	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
	if (priv->plat->has_gmac4)
		value |= MII_GMAC4_READ;

	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000))
		return -EBUSY;

	writel(value, priv->ioaddr + mii_address);

	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000))
		return -EBUSY;

	/* Read the data from the MII data register */
	data = (int)readl(priv->ioaddr + mii_data);

	return data;
}

/**
 * stmmac_c22_write
 * @bus: points to the mii_bus structure
 * @phyaddr: clause-22 phy address
 * @phyreg: clause-22 phy register
 * @phydata: clause-22 phy data
 * Description: it writes the data into the MII register follow clause 22.
 */
static int stmmac_c22_write(struct mii_bus *bus, int phyaddr, int phyreg,
			    u16 phydata)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 v;
	u32 value = MII_BUSY;

	value |= (phyaddr << priv->hw->mii.addr_shift)
		& priv->hw->mii.addr_mask;
	value |= (phyreg << priv->hw->mii.reg_shift) & priv->hw->mii.reg_mask;

	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
	if (priv->plat->has_gmac4)
		value |= MII_GMAC4_WRITE;
	else
		value |= MII_WRITE;

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000))
		return -EBUSY;

	/* Set the MII address register to write */
	writel(phydata, priv->ioaddr + mii_data);
	writel(value, priv->ioaddr + mii_address);

	/* Wait until any existing MII operation is complete */
	return readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
				  100, 10000);
}

/**
 * stmmac_c45_read
 * @bus: points to the mii_bus structure
 * @phyaddr: clause-45 phy address
 * @devad: clause-45 device address
 * @prtad: clause-45 register address
 * @phydata: phy data
 * Description: it reads the data from the  MII register follow clause 45.
 */
static int stmmac_c45_read(struct mii_bus *bus, int phyaddr,
			   int devad, int prtad)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 v, value;
	int data;

	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000))
		return -EBUSY;

	value = 0;
	value |= (prtad << priv->hw->mii.cl45_reg_shift)
			& priv->hw->mii.cl45_reg_mask;
	writel(value, priv->ioaddr + mii_data);

	/* delay 2ms to avoid error value of get_phy_c45_devs_in_pkg */
	mdelay(2);

	value = MII_BUSY;
	value |= (phyaddr << priv->hw->mii.addr_shift)
		& priv->hw->mii.addr_mask;
	value |= (devad << priv->hw->mii.reg_shift) & priv->hw->mii.reg_mask;
	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
	if (priv->plat->has_gmac4) {
		value |= MII_GMAC4_READ;
		value |= priv->hw->mii.cl45_en;
	}
	writel(value, priv->ioaddr + mii_address);

	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000))
		return -EBUSY;

	/* Read the data from the MII data register */
	data = (int)(readl(priv->ioaddr + mii_data) & 0xffff);

	return data;
}

/**
 * stmmac_c45_write
 * @bus: points to the mii_bus structure
 * @phyaddr: clause-45 phy address
 * @devad: clause-45 device address
 * @prtad: clause-45 register address
 * @phydata: phy data
 * Description: it writes the data into the MII register follow clause 45.
 */
static int stmmac_c45_write(struct mii_bus *bus, int phyaddr, int devad,
			    int prtad, u16 phydata)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 v, value;

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000))
		return -EBUSY;

	value = phydata;
	value |= (prtad << priv->hw->mii.cl45_reg_shift) &
		 priv->hw->mii.cl45_reg_mask;
	writel(value, priv->ioaddr + mii_data);

	mdelay(2);

	value = MII_BUSY;
	value |= (phyaddr << priv->hw->mii.addr_shift) &
		 priv->hw->mii.addr_mask;
	value |= (devad << priv->hw->mii.reg_shift) & priv->hw->mii.reg_mask;
	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift) &
		 priv->hw->mii.clk_csr_mask;

	if (priv->plat->has_gmac4) {
		value |= MII_GMAC4_WRITE;
		value |= priv->hw->mii.cl45_en;
	}
	writel(value, priv->ioaddr + mii_address);

	/* Wait until any existing MII operation is complete */
	return readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
				  100, 10000);
=======
	ret = readl_poll_timeout(priv->ioaddr + mii_data, tmp,
				 !(tmp & MII_XGMAC_BUSY), 100, 10000);

err_disable_clks:
	pm_runtime_put(priv->device);

	return ret;
>>>>>>> upstream/android-13
}

/**
 * stmmac_mdio_read
 * @bus: points to the mii_bus structure
 * @phyaddr: MII addr
 * @phyreg: MII reg
 * Description: it reads data from the MII register from within the phy device.
<<<<<<< HEAD
 */
static int stmmac_mdio_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	if (phyreg & MII_ADDR_C45) {
		int devad, prtad;

		devad = (phyreg >> 16) & 0x1f;
		prtad = phyreg & 0xffff;
		return stmmac_c45_read(bus, phyaddr, devad, prtad);
	} else {
		return stmmac_c22_read(bus, phyaddr, phyreg);
	}
=======
 * For the 7111 GMAC, we must set the bit 0 in the MII address register while
 * accessing the PHY registers.
 * Fortunately, it seems this has no drawback for the 7109 MAC.
 */
static int stmmac_mdio_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	u32 value = MII_BUSY;
	int data = 0;
	u32 v;

	data = pm_runtime_get_sync(priv->device);
	if (data < 0) {
		pm_runtime_put_noidle(priv->device);
		return data;
	}

	value |= (phyaddr << priv->hw->mii.addr_shift)
		& priv->hw->mii.addr_mask;
	value |= (phyreg << priv->hw->mii.reg_shift) & priv->hw->mii.reg_mask;
	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
	if (priv->plat->has_gmac4) {
		value |= MII_GMAC4_READ;
		if (phyreg & MII_ADDR_C45) {
			value |= MII_GMAC4_C45E;
			value &= ~priv->hw->mii.reg_mask;
			value |= ((phyreg >> MII_DEVADDR_C45_SHIFT) <<
			       priv->hw->mii.reg_shift) &
			       priv->hw->mii.reg_mask;

			data |= (phyreg & MII_REGADDR_C45_MASK) <<
				MII_GMAC4_REG_ADDR_SHIFT;
		}
	}

	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000)) {
		data = -EBUSY;
		goto err_disable_clks;
	}

	writel(data, priv->ioaddr + mii_data);
	writel(value, priv->ioaddr + mii_address);

	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000)) {
		data = -EBUSY;
		goto err_disable_clks;
	}

	/* Read the data from the MII data register */
	data = (int)readl(priv->ioaddr + mii_data) & MII_DATA_MASK;

err_disable_clks:
	pm_runtime_put(priv->device);

	return data;
>>>>>>> upstream/android-13
}

/**
 * stmmac_mdio_write
 * @bus: points to the mii_bus structure
 * @phyaddr: MII addr
 * @phyreg: MII reg
 * @phydata: phy data
 * Description: it writes the data into the MII register from within the device.
 */
static int stmmac_mdio_write(struct mii_bus *bus, int phyaddr, int phyreg,
			     u16 phydata)
{
<<<<<<< HEAD
	if (phyreg & MII_ADDR_C45) {
		int devad, prtad;

		devad = (phyreg >> 16) & 0x1f;
		prtad = phyreg & 0xffff;
		return stmmac_c45_write(bus, phyaddr, devad, prtad, phydata);
	} else {
		return stmmac_c22_write(bus, phyaddr, phyreg, phydata);
	}
=======
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
	unsigned int mii_data = priv->hw->mii.data;
	int ret, data = phydata;
	u32 value = MII_BUSY;
	u32 v;

	ret = pm_runtime_get_sync(priv->device);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->device);
		return ret;
	}

	value |= (phyaddr << priv->hw->mii.addr_shift)
		& priv->hw->mii.addr_mask;
	value |= (phyreg << priv->hw->mii.reg_shift) & priv->hw->mii.reg_mask;

	value |= (priv->clk_csr << priv->hw->mii.clk_csr_shift)
		& priv->hw->mii.clk_csr_mask;
	if (priv->plat->has_gmac4) {
		value |= MII_GMAC4_WRITE;
		if (phyreg & MII_ADDR_C45) {
			value |= MII_GMAC4_C45E;
			value &= ~priv->hw->mii.reg_mask;
			value |= ((phyreg >> MII_DEVADDR_C45_SHIFT) <<
			       priv->hw->mii.reg_shift) &
			       priv->hw->mii.reg_mask;

			data |= (phyreg & MII_REGADDR_C45_MASK) <<
				MII_GMAC4_REG_ADDR_SHIFT;
		}
	} else {
		value |= MII_WRITE;
	}

	/* Wait until any existing MII operation is complete */
	if (readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
			       100, 10000)) {
		ret = -EBUSY;
		goto err_disable_clks;
	}

	/* Set the MII address register to write */
	writel(data, priv->ioaddr + mii_data);
	writel(value, priv->ioaddr + mii_address);

	/* Wait until any existing MII operation is complete */
	ret = readl_poll_timeout(priv->ioaddr + mii_address, v, !(v & MII_BUSY),
				 100, 10000);

err_disable_clks:
	pm_runtime_put(priv->device);

	return ret;
>>>>>>> upstream/android-13
}

/**
 * stmmac_mdio_reset
 * @bus: points to the mii_bus structure
 * Description: reset the MII bus
 */
int stmmac_mdio_reset(struct mii_bus *bus)
{
#if IS_ENABLED(CONFIG_STMMAC_PLATFORM)
	struct net_device *ndev = bus->priv;
	struct stmmac_priv *priv = netdev_priv(ndev);
	unsigned int mii_address = priv->hw->mii.addr;
<<<<<<< HEAD
	struct stmmac_mdio_bus_data *data = priv->plat->mdio_bus_data;

#ifdef CONFIG_OF
	if (priv->device->of_node) {
		if (data->reset_gpio < 0) {
			struct device_node *np = priv->device->of_node;

			if (!np)
				return 0;

			data->reset_gpio = of_get_named_gpio(np,
						"snps,reset-gpio", 0);
			if (data->reset_gpio < 0)
				return 0;

			data->active_low = of_property_read_bool(np,
						"snps,reset-active-low");
			of_property_read_u32_array(np,
				"snps,reset-delays-us", data->delays, 3);

			if (devm_gpio_request(priv->device, data->reset_gpio,
					      "mdio-reset"))
				return 0;
		}

		gpio_direction_output(data->reset_gpio,
				      data->active_low ? 1 : 0);
		if (data->delays[0])
			msleep(DIV_ROUND_UP(data->delays[0], 1000));

		gpio_set_value(data->reset_gpio, data->active_low ? 0 : 1);
		if (data->delays[1])
			msleep(DIV_ROUND_UP(data->delays[1], 1000));

		gpio_set_value(data->reset_gpio, data->active_low ? 1 : 0);
		if (data->delays[2])
			msleep(DIV_ROUND_UP(data->delays[2], 1000));
	}
#endif

	if (data->phy_reset) {
		netdev_dbg(ndev, "stmmac_mdio_reset: calling phy_reset\n");
		data->phy_reset(priv->plat->bsp_priv);
	}

=======

#ifdef CONFIG_OF
	if (priv->device->of_node) {
		struct gpio_desc *reset_gpio;
		u32 delays[3] = { 0, 0, 0 };

		reset_gpio = devm_gpiod_get_optional(priv->device,
						     "snps,reset",
						     GPIOD_OUT_LOW);
		if (IS_ERR(reset_gpio))
			return PTR_ERR(reset_gpio);

		device_property_read_u32_array(priv->device,
					       "snps,reset-delays-us",
					       delays, ARRAY_SIZE(delays));

		if (delays[0])
			msleep(DIV_ROUND_UP(delays[0], 1000));

		gpiod_set_value_cansleep(reset_gpio, 1);
		if (delays[1])
			msleep(DIV_ROUND_UP(delays[1], 1000));

		gpiod_set_value_cansleep(reset_gpio, 0);
		if (delays[2])
			msleep(DIV_ROUND_UP(delays[2], 1000));
	}
#endif

>>>>>>> upstream/android-13
	/* This is a workaround for problems with the STE101P PHY.
	 * It doesn't complete its reset until at least one clock cycle
	 * on MDC, so perform a dummy mdio read. To be updated for GMAC4
	 * if needed.
	 */
	if (!priv->plat->has_gmac4)
		writel(0, priv->ioaddr + mii_address);
#endif
	return 0;
}

<<<<<<< HEAD
=======
int stmmac_xpcs_setup(struct mii_bus *bus)
{
	struct net_device *ndev = bus->priv;
	struct mdio_device *mdiodev;
	struct stmmac_priv *priv;
	struct dw_xpcs *xpcs;
	int mode, addr;

	priv = netdev_priv(ndev);
	mode = priv->plat->phy_interface;

	/* Try to probe the XPCS by scanning all addresses. */
	for (addr = 0; addr < PHY_MAX_ADDR; addr++) {
		mdiodev = mdio_device_create(bus, addr);
		if (IS_ERR(mdiodev))
			continue;

		xpcs = xpcs_create(mdiodev, mode);
		if (IS_ERR_OR_NULL(xpcs)) {
			mdio_device_free(mdiodev);
			continue;
		}

		priv->hw->xpcs = xpcs;
		break;
	}

	if (!priv->hw->xpcs) {
		dev_warn(priv->device, "No xPCS found\n");
		return -ENODEV;
	}

	return 0;
}

>>>>>>> upstream/android-13
/**
 * stmmac_mdio_register
 * @ndev: net device structure
 * Description: it registers the MII bus
 */
int stmmac_mdio_register(struct net_device *ndev)
{
	int err = 0;
	struct mii_bus *new_bus;
	struct stmmac_priv *priv = netdev_priv(ndev);
	struct stmmac_mdio_bus_data *mdio_bus_data = priv->plat->mdio_bus_data;
	struct device_node *mdio_node = priv->plat->mdio_node;
	struct device *dev = ndev->dev.parent;
	int addr, found, max_addr;

	if (!mdio_bus_data)
		return 0;

	new_bus = mdiobus_alloc();
	if (!new_bus)
		return -ENOMEM;

	if (mdio_bus_data->irqs)
		memcpy(new_bus->irq, mdio_bus_data->irqs, sizeof(new_bus->irq));

<<<<<<< HEAD
#ifdef CONFIG_OF
	if (priv->device->of_node)
		mdio_bus_data->reset_gpio = -1;
#endif

	new_bus->name = "stmmac";

=======
	new_bus->name = "stmmac";

	if (priv->plat->has_gmac4)
		new_bus->probe_capabilities = MDIOBUS_C22_C45;

>>>>>>> upstream/android-13
	if (priv->plat->has_xgmac) {
		new_bus->read = &stmmac_xgmac2_mdio_read;
		new_bus->write = &stmmac_xgmac2_mdio_write;

		/* Right now only C22 phys are supported */
		max_addr = MII_XGMAC_MAX_C22ADDR + 1;

		/* Check if DT specified an unsupported phy addr */
		if (priv->plat->phy_addr > MII_XGMAC_MAX_C22ADDR)
			dev_err(dev, "Unsupported phy_addr (max=%d)\n",
					MII_XGMAC_MAX_C22ADDR);
	} else {
		new_bus->read = &stmmac_mdio_read;
		new_bus->write = &stmmac_mdio_write;
		max_addr = PHY_MAX_ADDR;
	}

<<<<<<< HEAD
	new_bus->reset = &stmmac_mdio_reset;
=======
	if (mdio_bus_data->needs_reset)
		new_bus->reset = &stmmac_mdio_reset;

>>>>>>> upstream/android-13
	snprintf(new_bus->id, MII_BUS_ID_SIZE, "%s-%x",
		 new_bus->name, priv->plat->bus_id);
	new_bus->priv = ndev;
	new_bus->phy_mask = mdio_bus_data->phy_mask;
	new_bus->parent = priv->device;

	err = of_mdiobus_register(new_bus, mdio_node);
	if (err != 0) {
		dev_err(dev, "Cannot register the MDIO bus\n");
		goto bus_register_fail;
	}

<<<<<<< HEAD
=======
	/* Looks like we need a dummy read for XGMAC only and C45 PHYs */
	if (priv->plat->has_xgmac)
		stmmac_xgmac2_mdio_read(new_bus, 0, MII_ADDR_C45);

>>>>>>> upstream/android-13
	if (priv->plat->phy_node || mdio_node)
		goto bus_register_done;

	found = 0;
	for (addr = 0; addr < max_addr; addr++) {
		struct phy_device *phydev = mdiobus_get_phy(new_bus, addr);

		if (!phydev)
			continue;

		/*
		 * If an IRQ was provided to be assigned after
		 * the bus probe, do it here.
		 */
		if (!mdio_bus_data->irqs &&
		    (mdio_bus_data->probed_phy_irq > 0)) {
			new_bus->irq[addr] = mdio_bus_data->probed_phy_irq;
			phydev->irq = mdio_bus_data->probed_phy_irq;
		}

		/*
		 * If we're going to bind the MAC to this PHY bus,
		 * and no PHY number was provided to the MAC,
		 * use the one probed here.
		 */
		if (priv->plat->phy_addr == -1)
			priv->plat->phy_addr = addr;

		phy_attached_info(phydev);
		found = 1;
	}

	if (!found && !mdio_node) {
		dev_warn(dev, "No PHY found\n");
<<<<<<< HEAD
		mdiobus_unregister(new_bus);
		mdiobus_free(new_bus);
		return -ENODEV;
=======
		err = -ENODEV;
		goto no_phy_found;
>>>>>>> upstream/android-13
	}

bus_register_done:
	priv->mii = new_bus;

	return 0;

<<<<<<< HEAD
=======
no_phy_found:
	mdiobus_unregister(new_bus);
>>>>>>> upstream/android-13
bus_register_fail:
	mdiobus_free(new_bus);
	return err;
}

/**
 * stmmac_mdio_unregister
 * @ndev: net device structure
 * Description: it unregisters the MII bus
 */
int stmmac_mdio_unregister(struct net_device *ndev)
{
	struct stmmac_priv *priv = netdev_priv(ndev);

	if (!priv->mii)
		return 0;

<<<<<<< HEAD
=======
	if (priv->hw->xpcs) {
		mdio_device_free(priv->hw->xpcs->mdiodev);
		xpcs_destroy(priv->hw->xpcs);
	}

>>>>>>> upstream/android-13
	mdiobus_unregister(priv->mii);
	priv->mii->priv = NULL;
	mdiobus_free(priv->mii);
	priv->mii = NULL;

	return 0;
}
