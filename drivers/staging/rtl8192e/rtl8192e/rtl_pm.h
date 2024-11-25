<<<<<<< HEAD
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 *****************************************************************************/

=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
>>>>>>> upstream/android-13
#ifndef R8192E_PM_H
#define R8192E_PM_H

#include <linux/types.h>
#include <linux/pci.h>

<<<<<<< HEAD
int rtl92e_suspend(struct pci_dev *dev, pm_message_t state);
int rtl92e_resume(struct pci_dev *dev);
=======
int rtl92e_suspend(struct device *dev_d);
int rtl92e_resume(struct device *dev_d);
>>>>>>> upstream/android-13

#endif
