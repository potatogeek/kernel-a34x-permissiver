<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/* Texas Instruments Ethernet Switch Driver
 *
 * Copyright (C) 2013 Texas Instruments
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */
#ifndef __CPSW_H__
#define __CPSW_H__

#include <linux/if_ether.h>
#include <linux/phy.h>

#define mac_hi(mac)	(((mac)[0] << 0) | ((mac)[1] << 8) |	\
			 ((mac)[2] << 16) | ((mac)[3] << 24))
#define mac_lo(mac)	(((mac)[4] << 0) | ((mac)[5] << 8))

<<<<<<< HEAD
void cpsw_phy_sel(struct device *dev, phy_interface_t phy_mode, int slave);
=======
#if IS_ENABLED(CONFIG_TI_CPSW_PHY_SEL)
void cpsw_phy_sel(struct device *dev, phy_interface_t phy_mode, int slave);
#else
static inline
void cpsw_phy_sel(struct device *dev, phy_interface_t phy_mode, int slave)
{}
#endif
>>>>>>> upstream/android-13
int ti_cm_get_macid(struct device *dev, int slave, u8 *mac_addr);

#endif /* __CPSW_H__ */
