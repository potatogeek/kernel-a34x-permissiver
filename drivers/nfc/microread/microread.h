<<<<<<< HEAD
/*
 * Copyright (C) 2011 - 2012  Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2011 - 2012  Intel Corporation. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef __LOCAL_MICROREAD_H_
#define __LOCAL_MICROREAD_H_

#include <net/nfc/hci.h>

#define DRIVER_DESC "NFC driver for microread"

<<<<<<< HEAD
int microread_probe(void *phy_id, struct nfc_phy_ops *phy_ops, char *llc_name,
		    int phy_headroom, int phy_tailroom, int phy_payload,
		    struct nfc_hci_dev **hdev);
=======
int microread_probe(void *phy_id, const struct nfc_phy_ops *phy_ops,
		    const char *llc_name, int phy_headroom, int phy_tailroom,
		    int phy_payload, struct nfc_hci_dev **hdev);
>>>>>>> upstream/android-13

void microread_remove(struct nfc_hci_dev *hdev);

#endif /* __LOCAL_MICROREAD_H_ */
