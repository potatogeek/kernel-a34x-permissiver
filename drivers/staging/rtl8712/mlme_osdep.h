<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/******************************************************************************
 *
 * Copyright(c) 2007 - 2010 Realtek Corporation. All rights reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
=======
>>>>>>> upstream/android-13
 * Modifications for inclusion into the Linux staging tree are
 * Copyright(c) 2010 Larry Finger. All rights reserved.
 *
 * Contact information:
 * WLAN FAE <wlanfae@realtek.com>
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 ******************************************************************************/
#ifndef	__MLME_OSDEP_H_
#define __MLME_OSDEP_H_

#include "osdep_service.h"
#include "drv_types.h"

void r8712_init_mlme_timer(struct _adapter *padapter);
void r8712_os_indicate_disconnect(struct _adapter *adapter);
void r8712_os_indicate_connect(struct _adapter *adapter);
void r8712_report_sec_ie(struct _adapter *adapter, u8 authmode, u8 *sec_ie);
int r8712_recv_indicatepkts_in_order(struct _adapter *adapter,
<<<<<<< HEAD
				struct recv_reorder_ctrl *precvreorder_ctrl,
				int bforced);
=======
				     struct recv_reorder_ctrl *precvreorder_ctrl,
				     int bforced);
>>>>>>> upstream/android-13
void r8712_indicate_wx_assoc_event(struct _adapter *padapter);
void r8712_indicate_wx_disassoc_event(struct _adapter *padapter);

#endif	/*_MLME_OSDEP_H_*/

