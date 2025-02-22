<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * Broadcom BCM7xxx internal transceivers support.
 *
 * Copyright (C) 2014-2017 Broadcom
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/phy.h>
#include <linux/delay.h>
#include "bcm-phy-lib.h"
#include <linux/bitops.h>
#include <linux/brcmphy.h>
<<<<<<< HEAD
=======
#include <linux/clk.h>
>>>>>>> upstream/android-13
#include <linux/mdio.h>

/* Broadcom BCM7xxx internal PHY registers */

/* EPHY only register definitions */
#define MII_BCM7XXX_100TX_AUX_CTL	0x10
#define MII_BCM7XXX_100TX_FALSE_CAR	0x13
#define MII_BCM7XXX_100TX_DISC		0x14
#define MII_BCM7XXX_AUX_MODE		0x1d
#define  MII_BCM7XXX_64CLK_MDIO		BIT(12)
#define MII_BCM7XXX_TEST		0x1f
#define  MII_BCM7XXX_SHD_MODE_2		BIT(2)
#define MII_BCM7XXX_SHD_2_ADDR_CTRL	0xe
#define MII_BCM7XXX_SHD_2_CTRL_STAT	0xf
#define MII_BCM7XXX_SHD_2_BIAS_TRIM	0x1a
<<<<<<< HEAD
#define MII_BCM7XXX_SHD_3_AN_EEE_ADV	0x3
=======
#define MII_BCM7XXX_SHD_3_PCS_CTRL	0x0
#define MII_BCM7XXX_SHD_3_PCS_STATUS	0x1
#define MII_BCM7XXX_SHD_3_EEE_CAP	0x2
#define MII_BCM7XXX_SHD_3_AN_EEE_ADV	0x3
#define MII_BCM7XXX_SHD_3_EEE_LP	0x4
#define MII_BCM7XXX_SHD_3_EEE_WK_ERR	0x5
>>>>>>> upstream/android-13
#define MII_BCM7XXX_SHD_3_PCS_CTRL_2	0x6
#define  MII_BCM7XXX_PCS_CTRL_2_DEF	0x4400
#define MII_BCM7XXX_SHD_3_AN_STAT	0xb
#define  MII_BCM7XXX_AN_NULL_MSG_EN	BIT(0)
#define  MII_BCM7XXX_AN_EEE_EN		BIT(1)
#define MII_BCM7XXX_SHD_3_EEE_THRESH	0xe
#define  MII_BCM7XXX_EEE_THRESH_DEF	0x50
#define MII_BCM7XXX_SHD_3_TL4		0x23
#define  MII_BCM7XXX_TL4_RST_MSK	(BIT(2) | BIT(1))

<<<<<<< HEAD
/* 28nm only register definitions */
#define MISC_ADDR(base, channel)	base, channel

#define DSP_TAP10			MISC_ADDR(0x0a, 0)
#define PLL_PLLCTRL_1			MISC_ADDR(0x32, 1)
#define PLL_PLLCTRL_2			MISC_ADDR(0x32, 2)
#define PLL_PLLCTRL_4			MISC_ADDR(0x33, 0)

#define AFE_RXCONFIG_0			MISC_ADDR(0x38, 0)
#define AFE_RXCONFIG_1			MISC_ADDR(0x38, 1)
#define AFE_RXCONFIG_2			MISC_ADDR(0x38, 2)
#define AFE_RX_LP_COUNTER		MISC_ADDR(0x38, 3)
#define AFE_TX_CONFIG			MISC_ADDR(0x39, 0)
#define AFE_VDCA_ICTRL_0		MISC_ADDR(0x39, 1)
#define AFE_VDAC_OTHERS_0		MISC_ADDR(0x39, 3)
#define AFE_HPF_TRIM_OTHERS		MISC_ADDR(0x3a, 0)

struct bcm7xxx_phy_priv {
	u64	*stats;
};

static void r_rc_cal_reset(struct phy_device *phydev)
{
	/* Reset R_CAL/RC_CAL Engine */
	bcm_phy_write_exp_sel(phydev, 0x00b0, 0x0010);

	/* Disable Reset R_AL/RC_CAL Engine */
	bcm_phy_write_exp_sel(phydev, 0x00b0, 0x0000);
}

static int bcm7xxx_28nm_b0_afe_config_init(struct phy_device *phydev)
{
	/* Increase VCO range to prevent unlocking problem of PLL at low
	 * temp
	 */
	bcm_phy_write_misc(phydev, PLL_PLLCTRL_1, 0x0048);

	/* Change Ki to 011 */
	bcm_phy_write_misc(phydev, PLL_PLLCTRL_2, 0x021b);

	/* Disable loading of TVCO buffer to bandgap, set bandgap trim
	 * to 111
	 */
	bcm_phy_write_misc(phydev, PLL_PLLCTRL_4, 0x0e20);

	/* Adjust bias current trim by -3 */
	bcm_phy_write_misc(phydev, DSP_TAP10, 0x690b);

	/* Switch to CORE_BASE1E */
	phy_write(phydev, MII_BRCM_CORE_BASE1E, 0xd);

	r_rc_cal_reset(phydev);

	/* write AFE_RXCONFIG_0 */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_0, 0xeb19);

	/* write AFE_RXCONFIG_1 */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_1, 0x9a3f);

	/* write AFE_RX_LP_COUNTER */
	bcm_phy_write_misc(phydev, AFE_RX_LP_COUNTER, 0x7fc0);

	/* write AFE_HPF_TRIM_OTHERS */
	bcm_phy_write_misc(phydev, AFE_HPF_TRIM_OTHERS, 0x000b);

	/* write AFTE_TX_CONFIG */
	bcm_phy_write_misc(phydev, AFE_TX_CONFIG, 0x0800);

	return 0;
}

=======
struct bcm7xxx_phy_priv {
	u64	*stats;
	struct clk *clk;
};

>>>>>>> upstream/android-13
static int bcm7xxx_28nm_d0_afe_config_init(struct phy_device *phydev)
{
	/* AFE_RXCONFIG_0 */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_0, 0xeb15);

	/* AFE_RXCONFIG_1 */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_1, 0x9b2f);

	/* AFE_RXCONFIG_2, set rCal offset for HT=0 code and LT=-2 code */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_2, 0x2003);

	/* AFE_RX_LP_COUNTER, set RX bandwidth to maximum */
	bcm_phy_write_misc(phydev, AFE_RX_LP_COUNTER, 0x7fc0);

	/* AFE_TX_CONFIG, set 100BT Cfeed=011 to improve rise/fall time */
	bcm_phy_write_misc(phydev, AFE_TX_CONFIG, 0x431);

	/* AFE_VDCA_ICTRL_0, set Iq=1101 instead of 0111 for AB symmetry */
	bcm_phy_write_misc(phydev, AFE_VDCA_ICTRL_0, 0xa7da);

	/* AFE_VDAC_OTHERS_0, set 1000BT Cidac=010 for all ports */
	bcm_phy_write_misc(phydev, AFE_VDAC_OTHERS_0, 0xa020);

	/* AFE_HPF_TRIM_OTHERS, set 100Tx/10BT to -4.5% swing and set rCal
	 * offset for HT=0 code
	 */
	bcm_phy_write_misc(phydev, AFE_HPF_TRIM_OTHERS, 0x00e3);

	/* CORE_BASE1E, force trim to overwrite and set I_ext trim to 0000 */
	phy_write(phydev, MII_BRCM_CORE_BASE1E, 0x0010);

	/* DSP_TAP10, adjust bias current trim (+0% swing, +0 tick) */
	bcm_phy_write_misc(phydev, DSP_TAP10, 0x011b);

	/* Reset R_CAL/RC_CAL engine */
<<<<<<< HEAD
	r_rc_cal_reset(phydev);
=======
	bcm_phy_r_rc_cal_reset(phydev);
>>>>>>> upstream/android-13

	return 0;
}

static int bcm7xxx_28nm_e0_plus_afe_config_init(struct phy_device *phydev)
{
	/* AFE_RXCONFIG_1, provide more margin for INL/DNL measurement */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_1, 0x9b2f);

	/* AFE_TX_CONFIG, set 100BT Cfeed=011 to improve rise/fall time */
	bcm_phy_write_misc(phydev, AFE_TX_CONFIG, 0x431);

	/* AFE_VDCA_ICTRL_0, set Iq=1101 instead of 0111 for AB symmetry */
	bcm_phy_write_misc(phydev, AFE_VDCA_ICTRL_0, 0xa7da);

	/* AFE_HPF_TRIM_OTHERS, set 100Tx/10BT to -4.5% swing and set rCal
	 * offset for HT=0 code
	 */
	bcm_phy_write_misc(phydev, AFE_HPF_TRIM_OTHERS, 0x00e3);

	/* CORE_BASE1E, force trim to overwrite and set I_ext trim to 0000 */
	phy_write(phydev, MII_BRCM_CORE_BASE1E, 0x0010);

	/* DSP_TAP10, adjust bias current trim (+0% swing, +0 tick) */
	bcm_phy_write_misc(phydev, DSP_TAP10, 0x011b);

	/* Reset R_CAL/RC_CAL engine */
<<<<<<< HEAD
	r_rc_cal_reset(phydev);
=======
	bcm_phy_r_rc_cal_reset(phydev);
>>>>>>> upstream/android-13

	return 0;
}

static int bcm7xxx_28nm_a0_patch_afe_config_init(struct phy_device *phydev)
{
	/* +1 RC_CAL codes for RL centering for both LT and HT conditions */
	bcm_phy_write_misc(phydev, AFE_RXCONFIG_2, 0xd003);

	/* Cut master bias current by 2% to compensate for RC_CAL offset */
	bcm_phy_write_misc(phydev, DSP_TAP10, 0x791b);

	/* Improve hybrid leakage */
	bcm_phy_write_misc(phydev, AFE_HPF_TRIM_OTHERS, 0x10e3);

	/* Change rx_on_tune 8 to 0xf */
	bcm_phy_write_misc(phydev, 0x21, 0x2, 0x87f6);

	/* Change 100Tx EEE bandwidth */
	bcm_phy_write_misc(phydev, 0x22, 0x2, 0x017d);

	/* Enable ffe zero detection for Vitesse interoperability */
	bcm_phy_write_misc(phydev, 0x26, 0x2, 0x0015);

<<<<<<< HEAD
	r_rc_cal_reset(phydev);
=======
	bcm_phy_r_rc_cal_reset(phydev);
>>>>>>> upstream/android-13

	return 0;
}

static int bcm7xxx_28nm_config_init(struct phy_device *phydev)
{
	u8 rev = PHY_BRCM_7XXX_REV(phydev->dev_flags);
	u8 patch = PHY_BRCM_7XXX_PATCH(phydev->dev_flags);
	u8 count;
	int ret = 0;

	/* Newer devices have moved the revision information back into a
	 * standard location in MII_PHYS_ID[23]
	 */
	if (rev == 0)
		rev = phydev->phy_id & ~phydev->drv->phy_id_mask;

	pr_info_once("%s: %s PHY revision: 0x%02x, patch: %d\n",
		     phydev_name(phydev), phydev->drv->name, rev, patch);

	/* Dummy read to a register to workaround an issue upon reset where the
	 * internal inverter may not allow the first MDIO transaction to pass
	 * the MDIO management controller and make us return 0xffff for such
	 * reads.
	 */
	phy_read(phydev, MII_BMSR);

	switch (rev) {
	case 0xa0:
	case 0xb0:
<<<<<<< HEAD
		ret = bcm7xxx_28nm_b0_afe_config_init(phydev);
=======
		ret = bcm_phy_28nm_a0b0_afe_config_init(phydev);
>>>>>>> upstream/android-13
		break;
	case 0xd0:
		ret = bcm7xxx_28nm_d0_afe_config_init(phydev);
		break;
	case 0xe0:
	case 0xf0:
	/* Rev G0 introduces a roll over */
	case 0x10:
		ret = bcm7xxx_28nm_e0_plus_afe_config_init(phydev);
		break;
	case 0x01:
		ret = bcm7xxx_28nm_a0_patch_afe_config_init(phydev);
		break;
	default:
		break;
	}

	if (ret)
		return ret;

<<<<<<< HEAD
=======
	ret =  bcm_phy_enable_jumbo(phydev);
	if (ret)
		return ret;

>>>>>>> upstream/android-13
	ret = bcm_phy_downshift_get(phydev, &count);
	if (ret)
		return ret;

	/* Only enable EEE if Wirespeed/downshift is disabled */
	ret = bcm_phy_set_eee(phydev, count == DOWNSHIFT_DEV_DISABLE);
	if (ret)
		return ret;

	return bcm_phy_enable_apd(phydev, true);
}

static int bcm7xxx_28nm_resume(struct phy_device *phydev)
{
	int ret;

	/* Re-apply workarounds coming out suspend/resume */
	ret = bcm7xxx_28nm_config_init(phydev);
	if (ret)
		return ret;

	/* 28nm Gigabit PHYs come out of reset without any half-duplex
	 * or "hub" compliant advertised mode, fix that. This does not
	 * cause any problems with the PHY library since genphy_config_aneg()
	 * gracefully handles auto-negotiated and forced modes.
	 */
	return genphy_config_aneg(phydev);
}

<<<<<<< HEAD
static int phy_set_clr_bits(struct phy_device *dev, int location,
					int set_mask, int clr_mask)
{
	int v, ret;

	v = phy_read(dev, location);
=======
static int __phy_set_clr_bits(struct phy_device *dev, int location,
			      int set_mask, int clr_mask)
{
	int v, ret;

	v = __phy_read(dev, location);
>>>>>>> upstream/android-13
	if (v < 0)
		return v;

	v &= ~clr_mask;
	v |= set_mask;

<<<<<<< HEAD
	ret = phy_write(dev, location, v);
=======
	ret = __phy_write(dev, location, v);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	return v;
}

<<<<<<< HEAD
=======
static int phy_set_clr_bits(struct phy_device *dev, int location,
			    int set_mask, int clr_mask)
{
	int ret;

	mutex_lock(&dev->mdio.bus->mdio_lock);
	ret = __phy_set_clr_bits(dev, location, set_mask, clr_mask);
	mutex_unlock(&dev->mdio.bus->mdio_lock);

	return ret;
}

>>>>>>> upstream/android-13
static int bcm7xxx_28nm_ephy_01_afe_config_init(struct phy_device *phydev)
{
	int ret;

	/* set shadow mode 2 */
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_TEST,
			       MII_BCM7XXX_SHD_MODE_2, 0);
	if (ret < 0)
		return ret;

	/* Set current trim values INT_trim = -1, Ext_trim =0 */
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_BIAS_TRIM, 0x3BE0);
	if (ret < 0)
		goto reset_shadow_mode;

	/* Cal reset */
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL,
			MII_BCM7XXX_SHD_3_TL4);
	if (ret < 0)
		goto reset_shadow_mode;
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT,
			       MII_BCM7XXX_TL4_RST_MSK, 0);
	if (ret < 0)
		goto reset_shadow_mode;

	/* Cal reset disable */
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL,
			MII_BCM7XXX_SHD_3_TL4);
	if (ret < 0)
		goto reset_shadow_mode;
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT,
			       0, MII_BCM7XXX_TL4_RST_MSK);
	if (ret < 0)
		goto reset_shadow_mode;

reset_shadow_mode:
	/* reset shadow mode 2 */
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_TEST, 0,
			       MII_BCM7XXX_SHD_MODE_2);
	if (ret < 0)
		return ret;

	return 0;
}

/* The 28nm EPHY does not support Clause 45 (MMD) used by bcm-phy-lib */
static int bcm7xxx_28nm_ephy_apd_enable(struct phy_device *phydev)
{
	int ret;

	/* set shadow mode 1 */
	ret = phy_set_clr_bits(phydev, MII_BRCM_FET_BRCMTEST,
			       MII_BRCM_FET_BT_SRE, 0);
	if (ret < 0)
		return ret;

	/* Enable auto-power down */
	ret = phy_set_clr_bits(phydev, MII_BRCM_FET_SHDW_AUXSTAT2,
			       MII_BRCM_FET_SHDW_AS2_APDE, 0);
	if (ret < 0)
		return ret;

	/* reset shadow mode 1 */
	ret = phy_set_clr_bits(phydev, MII_BRCM_FET_BRCMTEST, 0,
			       MII_BRCM_FET_BT_SRE);
	if (ret < 0)
		return ret;

	return 0;
}

static int bcm7xxx_28nm_ephy_eee_enable(struct phy_device *phydev)
{
	int ret;

	/* set shadow mode 2 */
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_TEST,
			       MII_BCM7XXX_SHD_MODE_2, 0);
	if (ret < 0)
		return ret;

	/* Advertise supported modes */
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL,
			MII_BCM7XXX_SHD_3_AN_EEE_ADV);
	if (ret < 0)
		goto reset_shadow_mode;
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT,
			MDIO_EEE_100TX);
	if (ret < 0)
		goto reset_shadow_mode;

	/* Restore Defaults */
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL,
			MII_BCM7XXX_SHD_3_PCS_CTRL_2);
	if (ret < 0)
		goto reset_shadow_mode;
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT,
			MII_BCM7XXX_PCS_CTRL_2_DEF);
	if (ret < 0)
		goto reset_shadow_mode;

	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL,
			MII_BCM7XXX_SHD_3_EEE_THRESH);
	if (ret < 0)
		goto reset_shadow_mode;
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT,
			MII_BCM7XXX_EEE_THRESH_DEF);
	if (ret < 0)
		goto reset_shadow_mode;

	/* Enable EEE autonegotiation */
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL,
			MII_BCM7XXX_SHD_3_AN_STAT);
	if (ret < 0)
		goto reset_shadow_mode;
	ret = phy_write(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT,
			(MII_BCM7XXX_AN_NULL_MSG_EN | MII_BCM7XXX_AN_EEE_EN));
	if (ret < 0)
		goto reset_shadow_mode;

reset_shadow_mode:
	/* reset shadow mode 2 */
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_TEST, 0,
			       MII_BCM7XXX_SHD_MODE_2);
	if (ret < 0)
		return ret;

	/* Restart autoneg */
	phy_write(phydev, MII_BMCR,
		  (BMCR_SPEED100 | BMCR_ANENABLE | BMCR_ANRESTART));

	return 0;
}

static int bcm7xxx_28nm_ephy_config_init(struct phy_device *phydev)
{
	u8 rev = phydev->phy_id & ~phydev->drv->phy_id_mask;
	int ret = 0;

	pr_info_once("%s: %s PHY revision: 0x%02x\n",
		     phydev_name(phydev), phydev->drv->name, rev);

	/* Dummy read to a register to workaround a possible issue upon reset
	 * where the internal inverter may not allow the first MDIO transaction
	 * to pass the MDIO management controller and make us return 0xffff for
	 * such reads.
	 */
	phy_read(phydev, MII_BMSR);

	/* Apply AFE software work-around if necessary */
	if (rev == 0x01) {
		ret = bcm7xxx_28nm_ephy_01_afe_config_init(phydev);
		if (ret)
			return ret;
	}

	ret = bcm7xxx_28nm_ephy_eee_enable(phydev);
	if (ret)
		return ret;

	return bcm7xxx_28nm_ephy_apd_enable(phydev);
}

<<<<<<< HEAD
=======
#define MII_BCM7XXX_REG_INVALID	0xff

static u8 bcm7xxx_28nm_ephy_regnum_to_shd(u16 regnum)
{
	switch (regnum) {
	case MDIO_CTRL1:
		return MII_BCM7XXX_SHD_3_PCS_CTRL;
	case MDIO_STAT1:
		return MII_BCM7XXX_SHD_3_PCS_STATUS;
	case MDIO_PCS_EEE_ABLE:
		return MII_BCM7XXX_SHD_3_EEE_CAP;
	case MDIO_AN_EEE_ADV:
		return MII_BCM7XXX_SHD_3_AN_EEE_ADV;
	case MDIO_AN_EEE_LPABLE:
		return MII_BCM7XXX_SHD_3_EEE_LP;
	case MDIO_PCS_EEE_WK_ERR:
		return MII_BCM7XXX_SHD_3_EEE_WK_ERR;
	default:
		return MII_BCM7XXX_REG_INVALID;
	}
}

static bool bcm7xxx_28nm_ephy_dev_valid(int devnum)
{
	return devnum == MDIO_MMD_AN || devnum == MDIO_MMD_PCS;
}

static int bcm7xxx_28nm_ephy_read_mmd(struct phy_device *phydev,
				      int devnum, u16 regnum)
{
	u8 shd = bcm7xxx_28nm_ephy_regnum_to_shd(regnum);
	int ret;

	if (!bcm7xxx_28nm_ephy_dev_valid(devnum) ||
	    shd == MII_BCM7XXX_REG_INVALID)
		return -EOPNOTSUPP;

	/* set shadow mode 2 */
	ret = __phy_set_clr_bits(phydev, MII_BCM7XXX_TEST,
				 MII_BCM7XXX_SHD_MODE_2, 0);
	if (ret < 0)
		return ret;

	/* Access the desired shadow register address */
	ret = __phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL, shd);
	if (ret < 0)
		goto reset_shadow_mode;

	ret = __phy_read(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT);

reset_shadow_mode:
	/* reset shadow mode 2 */
	__phy_set_clr_bits(phydev, MII_BCM7XXX_TEST, 0,
			   MII_BCM7XXX_SHD_MODE_2);
	return ret;
}

static int bcm7xxx_28nm_ephy_write_mmd(struct phy_device *phydev,
				       int devnum, u16 regnum, u16 val)
{
	u8 shd = bcm7xxx_28nm_ephy_regnum_to_shd(regnum);
	int ret;

	if (!bcm7xxx_28nm_ephy_dev_valid(devnum) ||
	    shd == MII_BCM7XXX_REG_INVALID)
		return -EOPNOTSUPP;

	/* set shadow mode 2 */
	ret = __phy_set_clr_bits(phydev, MII_BCM7XXX_TEST,
				 MII_BCM7XXX_SHD_MODE_2, 0);
	if (ret < 0)
		return ret;

	/* Access the desired shadow register address */
	ret = __phy_write(phydev, MII_BCM7XXX_SHD_2_ADDR_CTRL, shd);
	if (ret < 0)
		goto reset_shadow_mode;

	/* Write the desired value in the shadow register */
	__phy_write(phydev, MII_BCM7XXX_SHD_2_CTRL_STAT, val);

reset_shadow_mode:
	/* reset shadow mode 2 */
	return __phy_set_clr_bits(phydev, MII_BCM7XXX_TEST, 0,
				  MII_BCM7XXX_SHD_MODE_2);
}

>>>>>>> upstream/android-13
static int bcm7xxx_28nm_ephy_resume(struct phy_device *phydev)
{
	int ret;

	/* Re-apply workarounds coming out suspend/resume */
	ret = bcm7xxx_28nm_ephy_config_init(phydev);
	if (ret)
		return ret;

	return genphy_config_aneg(phydev);
}

static int bcm7xxx_config_init(struct phy_device *phydev)
{
	int ret;

	/* Enable 64 clock MDIO */
	phy_write(phydev, MII_BCM7XXX_AUX_MODE, MII_BCM7XXX_64CLK_MDIO);
	phy_read(phydev, MII_BCM7XXX_AUX_MODE);

	/* set shadow mode 2 */
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_TEST,
			MII_BCM7XXX_SHD_MODE_2, MII_BCM7XXX_SHD_MODE_2);
	if (ret < 0)
		return ret;

	/* set iddq_clkbias */
	phy_write(phydev, MII_BCM7XXX_100TX_DISC, 0x0F00);
	udelay(10);

	/* reset iddq_clkbias */
	phy_write(phydev, MII_BCM7XXX_100TX_DISC, 0x0C00);

	phy_write(phydev, MII_BCM7XXX_100TX_FALSE_CAR, 0x7555);

	/* reset shadow mode 2 */
	ret = phy_set_clr_bits(phydev, MII_BCM7XXX_TEST, 0, MII_BCM7XXX_SHD_MODE_2);
	if (ret < 0)
		return ret;

	return 0;
}

/* Workaround for putting the PHY in IDDQ mode, required
 * for all BCM7XXX 40nm and 65nm PHYs
 */
static int bcm7xxx_suspend(struct phy_device *phydev)
{
	int ret;
	static const struct bcm7xxx_regs {
		int reg;
		u16 value;
	} bcm7xxx_suspend_cfg[] = {
		{ MII_BCM7XXX_TEST, 0x008b },
		{ MII_BCM7XXX_100TX_AUX_CTL, 0x01c0 },
		{ MII_BCM7XXX_100TX_DISC, 0x7000 },
		{ MII_BCM7XXX_TEST, 0x000f },
		{ MII_BCM7XXX_100TX_AUX_CTL, 0x20d0 },
		{ MII_BCM7XXX_TEST, 0x000b },
	};
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(bcm7xxx_suspend_cfg); i++) {
		ret = phy_write(phydev,
				bcm7xxx_suspend_cfg[i].reg,
				bcm7xxx_suspend_cfg[i].value);
		if (ret)
			return ret;
	}

	return 0;
}

static int bcm7xxx_28nm_get_tunable(struct phy_device *phydev,
				    struct ethtool_tunable *tuna,
				    void *data)
{
	switch (tuna->id) {
	case ETHTOOL_PHY_DOWNSHIFT:
		return bcm_phy_downshift_get(phydev, (u8 *)data);
	default:
		return -EOPNOTSUPP;
	}
}

static int bcm7xxx_28nm_set_tunable(struct phy_device *phydev,
				    struct ethtool_tunable *tuna,
				    const void *data)
{
	u8 count = *(u8 *)data;
	int ret;

	switch (tuna->id) {
	case ETHTOOL_PHY_DOWNSHIFT:
		ret = bcm_phy_downshift_set(phydev, count);
		break;
	default:
		return -EOPNOTSUPP;
	}

	if (ret)
		return ret;

	/* Disable EEE advertisement since this prevents the PHY
	 * from successfully linking up, trigger auto-negotiation restart
	 * to let the MAC decide what to do.
	 */
	ret = bcm_phy_set_eee(phydev, count == DOWNSHIFT_DEV_DISABLE);
	if (ret)
		return ret;

	return genphy_restart_aneg(phydev);
}

static void bcm7xxx_28nm_get_phy_stats(struct phy_device *phydev,
				       struct ethtool_stats *stats, u64 *data)
{
	struct bcm7xxx_phy_priv *priv = phydev->priv;

	bcm_phy_get_stats(phydev, priv->stats, stats, data);
}

static int bcm7xxx_28nm_probe(struct phy_device *phydev)
{
	struct bcm7xxx_phy_priv *priv;
<<<<<<< HEAD
=======
	int ret = 0;
>>>>>>> upstream/android-13

	priv = devm_kzalloc(&phydev->mdio.dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	phydev->priv = priv;

	priv->stats = devm_kcalloc(&phydev->mdio.dev,
				   bcm_phy_get_sset_count(phydev), sizeof(u64),
				   GFP_KERNEL);
	if (!priv->stats)
		return -ENOMEM;

<<<<<<< HEAD
	return 0;
=======
	priv->clk = devm_clk_get_optional(&phydev->mdio.dev, NULL);
	if (IS_ERR(priv->clk))
		return PTR_ERR(priv->clk);

	ret = clk_prepare_enable(priv->clk);
	if (ret)
		return ret;

	/* Dummy read to a register to workaround an issue upon reset where the
	 * internal inverter may not allow the first MDIO transaction to pass
	 * the MDIO management controller and make us return 0xffff for such
	 * reads. This is needed to ensure that any subsequent reads to the
	 * PHY will succeed.
	 */
	phy_read(phydev, MII_BMSR);

	return ret;
}

static void bcm7xxx_28nm_remove(struct phy_device *phydev)
{
	struct bcm7xxx_phy_priv *priv = phydev->priv;

	clk_disable_unprepare(priv->clk);
>>>>>>> upstream/android-13
}

#define BCM7XXX_28NM_GPHY(_oui, _name)					\
{									\
	.phy_id		= (_oui),					\
	.phy_id_mask	= 0xfffffff0,					\
	.name		= _name,					\
<<<<<<< HEAD
	.features	= PHY_GBIT_FEATURES,				\
=======
	/* PHY_GBIT_FEATURES */						\
>>>>>>> upstream/android-13
	.flags		= PHY_IS_INTERNAL,				\
	.config_init	= bcm7xxx_28nm_config_init,			\
	.resume		= bcm7xxx_28nm_resume,				\
	.get_tunable	= bcm7xxx_28nm_get_tunable,			\
	.set_tunable	= bcm7xxx_28nm_set_tunable,			\
	.get_sset_count	= bcm_phy_get_sset_count,			\
	.get_strings	= bcm_phy_get_strings,				\
	.get_stats	= bcm7xxx_28nm_get_phy_stats,			\
	.probe		= bcm7xxx_28nm_probe,				\
<<<<<<< HEAD
=======
	.remove		= bcm7xxx_28nm_remove,				\
>>>>>>> upstream/android-13
}

#define BCM7XXX_28NM_EPHY(_oui, _name)					\
{									\
	.phy_id		= (_oui),					\
	.phy_id_mask	= 0xfffffff0,					\
	.name		= _name,					\
<<<<<<< HEAD
	.features	= PHY_BASIC_FEATURES,				\
=======
	/* PHY_BASIC_FEATURES */					\
>>>>>>> upstream/android-13
	.flags		= PHY_IS_INTERNAL,				\
	.config_init	= bcm7xxx_28nm_ephy_config_init,		\
	.resume		= bcm7xxx_28nm_ephy_resume,			\
	.get_sset_count	= bcm_phy_get_sset_count,			\
	.get_strings	= bcm_phy_get_strings,				\
	.get_stats	= bcm7xxx_28nm_get_phy_stats,			\
	.probe		= bcm7xxx_28nm_probe,				\
<<<<<<< HEAD
=======
	.remove		= bcm7xxx_28nm_remove,				\
	.read_mmd	= bcm7xxx_28nm_ephy_read_mmd,			\
	.write_mmd	= bcm7xxx_28nm_ephy_write_mmd,			\
>>>>>>> upstream/android-13
}

#define BCM7XXX_40NM_EPHY(_oui, _name)					\
{									\
	.phy_id         = (_oui),					\
	.phy_id_mask    = 0xfffffff0,					\
	.name           = _name,					\
<<<<<<< HEAD
	.features       = PHY_BASIC_FEATURES,				\
=======
	/* PHY_BASIC_FEATURES */					\
>>>>>>> upstream/android-13
	.flags          = PHY_IS_INTERNAL,				\
	.soft_reset	= genphy_soft_reset,				\
	.config_init    = bcm7xxx_config_init,				\
	.suspend        = bcm7xxx_suspend,				\
	.resume         = bcm7xxx_config_init,				\
}

static struct phy_driver bcm7xxx_driver[] = {
<<<<<<< HEAD
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7250, "Broadcom BCM7250"),
=======
	BCM7XXX_28NM_EPHY(PHY_ID_BCM72113, "Broadcom BCM72113"),
	BCM7XXX_28NM_EPHY(PHY_ID_BCM72116, "Broadcom BCM72116"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7250, "Broadcom BCM7250"),
	BCM7XXX_28NM_EPHY(PHY_ID_BCM7255, "Broadcom BCM7255"),
>>>>>>> upstream/android-13
	BCM7XXX_28NM_EPHY(PHY_ID_BCM7260, "Broadcom BCM7260"),
	BCM7XXX_28NM_EPHY(PHY_ID_BCM7268, "Broadcom BCM7268"),
	BCM7XXX_28NM_EPHY(PHY_ID_BCM7271, "Broadcom BCM7271"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7278, "Broadcom BCM7278"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7364, "Broadcom BCM7364"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7366, "Broadcom BCM7366"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM74371, "Broadcom BCM74371"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7439, "Broadcom BCM7439"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7439_2, "Broadcom BCM7439 (2)"),
	BCM7XXX_28NM_GPHY(PHY_ID_BCM7445, "Broadcom BCM7445"),
<<<<<<< HEAD
	BCM7XXX_28NM_GPHY(PHY_ID_BCM_OMEGA, "Broadcom Omega Combo GPHY"),
=======
>>>>>>> upstream/android-13
	BCM7XXX_40NM_EPHY(PHY_ID_BCM7346, "Broadcom BCM7346"),
	BCM7XXX_40NM_EPHY(PHY_ID_BCM7362, "Broadcom BCM7362"),
	BCM7XXX_40NM_EPHY(PHY_ID_BCM7425, "Broadcom BCM7425"),
	BCM7XXX_40NM_EPHY(PHY_ID_BCM7429, "Broadcom BCM7429"),
	BCM7XXX_40NM_EPHY(PHY_ID_BCM7435, "Broadcom BCM7435"),
};

static struct mdio_device_id __maybe_unused bcm7xxx_tbl[] = {
<<<<<<< HEAD
	{ PHY_ID_BCM7250, 0xfffffff0, },
=======
	{ PHY_ID_BCM72113, 0xfffffff0 },
	{ PHY_ID_BCM72116, 0xfffffff0, },
	{ PHY_ID_BCM7250, 0xfffffff0, },
	{ PHY_ID_BCM7255, 0xfffffff0, },
>>>>>>> upstream/android-13
	{ PHY_ID_BCM7260, 0xfffffff0, },
	{ PHY_ID_BCM7268, 0xfffffff0, },
	{ PHY_ID_BCM7271, 0xfffffff0, },
	{ PHY_ID_BCM7278, 0xfffffff0, },
	{ PHY_ID_BCM7364, 0xfffffff0, },
	{ PHY_ID_BCM7366, 0xfffffff0, },
	{ PHY_ID_BCM7346, 0xfffffff0, },
	{ PHY_ID_BCM7362, 0xfffffff0, },
	{ PHY_ID_BCM7425, 0xfffffff0, },
	{ PHY_ID_BCM7429, 0xfffffff0, },
	{ PHY_ID_BCM74371, 0xfffffff0, },
	{ PHY_ID_BCM7439, 0xfffffff0, },
	{ PHY_ID_BCM7435, 0xfffffff0, },
	{ PHY_ID_BCM7445, 0xfffffff0, },
	{ }
};

module_phy_driver(bcm7xxx_driver);

MODULE_DEVICE_TABLE(mdio, bcm7xxx_tbl);

MODULE_DESCRIPTION("Broadcom BCM7xxx internal PHY driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Broadcom Corporation");
