<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/**************************************************************************
 * Copyright (c) 2011, Intel Corporation.
 * All Rights Reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 **************************************************************************/

#include <drm/drmP.h>
#include "psb_drv.h"
=======
 **************************************************************************/

#include "psb_drv.h"
#include "gma_device.h"
>>>>>>> upstream/android-13

void gma_get_core_freq(struct drm_device *dev)
{
	uint32_t clock;
<<<<<<< HEAD
	struct pci_dev *pci_root =
		pci_get_domain_bus_and_slot(pci_domain_nr(dev->pdev->bus),
=======
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	struct pci_dev *pci_root =
		pci_get_domain_bus_and_slot(pci_domain_nr(pdev->bus),
>>>>>>> upstream/android-13
					    0, 0);
	struct drm_psb_private *dev_priv = dev->dev_private;

	/*pci_write_config_dword(pci_root, 0xD4, 0x00C32004);*/
	/*pci_write_config_dword(pci_root, 0xD0, 0xE0033000);*/

	pci_write_config_dword(pci_root, 0xD0, 0xD0050300);
	pci_read_config_dword(pci_root, 0xD4, &clock);
	pci_dev_put(pci_root);

	switch (clock & 0x07) {
	case 0:
		dev_priv->core_freq = 100;
		break;
	case 1:
		dev_priv->core_freq = 133;
		break;
	case 2:
		dev_priv->core_freq = 150;
		break;
	case 3:
		dev_priv->core_freq = 178;
		break;
	case 4:
		dev_priv->core_freq = 200;
		break;
	case 5:
	case 6:
	case 7:
		dev_priv->core_freq = 266;
		break;
	default:
		dev_priv->core_freq = 0;
	}
}
