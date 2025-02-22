/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __LINUX_MII_H__
#define __LINUX_MII_H__


#include <linux/if.h>
#include <uapi/linux/mii.h>

struct ethtool_cmd;

struct mii_if_info {
	int phy_id;
	int advertising;
	int phy_id_mask;
	int reg_num_mask;

	unsigned int full_duplex : 1;	
	unsigned int force_media : 1;	
	unsigned int supports_gmii : 1; 

	struct net_device *dev;
	int (*mdio_read) (struct net_device *dev, int phy_id, int location);
	void (*mdio_write) (struct net_device *dev, int phy_id, int location, int val);
};

extern int mii_link_ok (struct mii_if_info *mii);
extern int mii_nway_restart (struct mii_if_info *mii);
extern int mii_ethtool_gset(struct mii_if_info *mii, struct ethtool_cmd *ecmd);
extern void mii_ethtool_get_link_ksettings(
	struct mii_if_info *mii, struct ethtool_link_ksettings *cmd);
extern int mii_ethtool_sset(struct mii_if_info *mii, struct ethtool_cmd *ecmd);
extern int mii_ethtool_set_link_ksettings(
	struct mii_if_info *mii, const struct ethtool_link_ksettings *cmd);
extern int mii_check_gmii_support(struct mii_if_info *mii);
extern void mii_check_link (struct mii_if_info *mii);
extern unsigned int mii_check_media (struct mii_if_info *mii,
				     unsigned int ok_to_print,
				     unsigned int init_media);
extern int generic_mii_ioctl(struct mii_if_info *mii_if,
			     struct mii_ioctl_data *mii_data, int cmd,
			     unsigned int *duplex_changed);


static inline struct mii_ioctl_data *if_mii(struct ifreq *rq)
{
	return (struct mii_ioctl_data *) &rq->ifr_ifru;
}


static inline unsigned int mii_nway_result (unsigned int negotiated)
{
	unsigned int ret;

	if (negotiated & LPA_100FULL)
		ret = LPA_100FULL;
	else if (negotiated & LPA_100BASE4)
		ret = LPA_100BASE4;
	else if (negotiated & LPA_100HALF)
		ret = LPA_100HALF;
	else if (negotiated & LPA_10FULL)
		ret = LPA_10FULL;
	else
		ret = LPA_10HALF;

	return ret;
}


static inline unsigned int mii_duplex (unsigned int duplex_lock,
				       unsigned int negotiated)
{
	if (duplex_lock)
		return 1;
	if (mii_nway_result(negotiated) & LPA_DUPLEX)
		return 1;
	return 0;
}


static inline u32 ethtool_adv_to_mii_adv_t(u32 ethadv)
{
	u32 result = 0;

	if (ethadv & ADVERTISED_10baseT_Half)
		result |= ADVERTISE_10HALF;
	if (ethadv & ADVERTISED_10baseT_Full)
		result |= ADVERTISE_10FULL;
	if (ethadv & ADVERTISED_100baseT_Half)
		result |= ADVERTISE_100HALF;
	if (ethadv & ADVERTISED_100baseT_Full)
		result |= ADVERTISE_100FULL;
	if (ethadv & ADVERTISED_Pause)
		result |= ADVERTISE_PAUSE_CAP;
	if (ethadv & ADVERTISED_Asym_Pause)
		result |= ADVERTISE_PAUSE_ASYM;

	return result;
}


static inline u32 mii_adv_to_ethtool_adv_t(u32 adv)
{
	u32 result = 0;

	if (adv & ADVERTISE_10HALF)
		result |= ADVERTISED_10baseT_Half;
	if (adv & ADVERTISE_10FULL)
		result |= ADVERTISED_10baseT_Full;
	if (adv & ADVERTISE_100HALF)
		result |= ADVERTISED_100baseT_Half;
	if (adv & ADVERTISE_100FULL)
		result |= ADVERTISED_100baseT_Full;
	if (adv & ADVERTISE_PAUSE_CAP)
		result |= ADVERTISED_Pause;
	if (adv & ADVERTISE_PAUSE_ASYM)
		result |= ADVERTISED_Asym_Pause;

	return result;
}


static inline u32 ethtool_adv_to_mii_ctrl1000_t(u32 ethadv)
{
	u32 result = 0;

	if (ethadv & ADVERTISED_1000baseT_Half)
		result |= ADVERTISE_1000HALF;
	if (ethadv & ADVERTISED_1000baseT_Full)
		result |= ADVERTISE_1000FULL;

	return result;
}


static inline u32 mii_ctrl1000_to_ethtool_adv_t(u32 adv)
{
	u32 result = 0;

	if (adv & ADVERTISE_1000HALF)
		result |= ADVERTISED_1000baseT_Half;
	if (adv & ADVERTISE_1000FULL)
		result |= ADVERTISED_1000baseT_Full;

	return result;
}


static inline u32 mii_lpa_to_ethtool_lpa_t(u32 lpa)
{
	u32 result = 0;

	if (lpa & LPA_LPACK)
		result |= ADVERTISED_Autoneg;

	return result | mii_adv_to_ethtool_adv_t(lpa);
}


static inline u32 mii_stat1000_to_ethtool_lpa_t(u32 lpa)
{
	u32 result = 0;

	if (lpa & LPA_1000HALF)
		result |= ADVERTISED_1000baseT_Half;
	if (lpa & LPA_1000FULL)
		result |= ADVERTISED_1000baseT_Full;

	return result;
}


static inline u32 ethtool_adv_to_mii_adv_x(u32 ethadv)
{
	u32 result = 0;

	if (ethadv & ADVERTISED_1000baseT_Half)
		result |= ADVERTISE_1000XHALF;
	if (ethadv & ADVERTISED_1000baseT_Full)
		result |= ADVERTISE_1000XFULL;
	if (ethadv & ADVERTISED_Pause)
		result |= ADVERTISE_1000XPAUSE;
	if (ethadv & ADVERTISED_Asym_Pause)
		result |= ADVERTISE_1000XPSE_ASYM;

	return result;
}


static inline u32 mii_adv_to_ethtool_adv_x(u32 adv)
{
	u32 result = 0;

	if (adv & ADVERTISE_1000XHALF)
		result |= ADVERTISED_1000baseT_Half;
	if (adv & ADVERTISE_1000XFULL)
		result |= ADVERTISED_1000baseT_Full;
	if (adv & ADVERTISE_1000XPAUSE)
		result |= ADVERTISED_Pause;
	if (adv & ADVERTISE_1000XPSE_ASYM)
		result |= ADVERTISED_Asym_Pause;

	return result;
}


static inline u32 mii_lpa_to_ethtool_lpa_x(u32 lpa)
{
	u32 result = 0;

	if (lpa & LPA_LPACK)
		result |= ADVERTISED_Autoneg;

	return result | mii_adv_to_ethtool_adv_x(lpa);
}


static inline u16 mii_advertise_flowctrl(int cap)
{
	u16 adv = 0;

	if (cap & FLOW_CTRL_RX)
		adv = ADVERTISE_PAUSE_CAP | ADVERTISE_PAUSE_ASYM;
	if (cap & FLOW_CTRL_TX)
		adv ^= ADVERTISE_PAUSE_ASYM;

	return adv;
}


static inline u8 mii_resolve_flowctrl_fdx(u16 lcladv, u16 rmtadv)
{
	u8 cap = 0;

	if (lcladv & rmtadv & ADVERTISE_PAUSE_CAP) {
		cap = FLOW_CTRL_TX | FLOW_CTRL_RX;
	} else if (lcladv & rmtadv & ADVERTISE_PAUSE_ASYM) {
		if (lcladv & ADVERTISE_PAUSE_CAP)
			cap = FLOW_CTRL_RX;
		else if (rmtadv & ADVERTISE_PAUSE_CAP)
			cap = FLOW_CTRL_TX;
	}

	return cap;
}

#endif 
