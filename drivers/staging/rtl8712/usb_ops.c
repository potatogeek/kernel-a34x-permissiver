<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/******************************************************************************
 * usb_ops.c
 *
 * Copyright(c) 2007 - 2010 Realtek Corporation. All rights reserved.
 * Linux device driver for RTL8192SU
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
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

#define _HCI_OPS_C_

#include "osdep_service.h"
#include "drv_types.h"
#include "osdep_intf.h"
#include "usb_ops.h"
#include "recv_osdep.h"

<<<<<<< HEAD
static u8 usb_read8(struct intf_hdl *pintfhdl, u32 addr)
=======
static u8 usb_read8(struct intf_hdl *intfhdl, u32 addr)
>>>>>>> upstream/android-13
{
	u8 request;
	u8 requesttype;
	u16 wvalue;
	u16 index;
	u16 len;
	__le32 data;
<<<<<<< HEAD
	struct intf_priv *pintfpriv = pintfhdl->pintfpriv;
=======
	struct intf_priv *intfpriv = intfhdl->pintfpriv;
>>>>>>> upstream/android-13

	request = 0x05;
	requesttype = 0x01; /* read_in */
	index = 0;
	wvalue = (u16)(addr & 0x0000ffff);
	len = 1;
<<<<<<< HEAD
	r8712_usbctrl_vendorreq(pintfpriv, request, wvalue, index, &data, len,
			  requesttype);
	return (u8)(le32_to_cpu(data) & 0x0ff);
}

static u16 usb_read16(struct intf_hdl *pintfhdl, u32 addr)
=======
	r8712_usbctrl_vendorreq(intfpriv, request, wvalue, index, &data, len,
				requesttype);
	return (u8)(le32_to_cpu(data) & 0x0ff);
}

static u16 usb_read16(struct intf_hdl *intfhdl, u32 addr)
>>>>>>> upstream/android-13
{
	u8 request;
	u8 requesttype;
	u16 wvalue;
	u16 index;
	u16 len;
	__le32 data;
<<<<<<< HEAD
	struct intf_priv *pintfpriv = pintfhdl->pintfpriv;
=======
	struct intf_priv *intfpriv = intfhdl->pintfpriv;
>>>>>>> upstream/android-13

	request = 0x05;
	requesttype = 0x01; /* read_in */
	index = 0;
	wvalue = (u16)(addr & 0x0000ffff);
	len = 2;
<<<<<<< HEAD
	r8712_usbctrl_vendorreq(pintfpriv, request, wvalue, index, &data, len,
			  requesttype);
	return (u16)(le32_to_cpu(data) & 0xffff);
}

static u32 usb_read32(struct intf_hdl *pintfhdl, u32 addr)
=======
	r8712_usbctrl_vendorreq(intfpriv, request, wvalue, index, &data, len,
				requesttype);
	return (u16)(le32_to_cpu(data) & 0xffff);
}

static u32 usb_read32(struct intf_hdl *intfhdl, u32 addr)
>>>>>>> upstream/android-13
{
	u8 request;
	u8 requesttype;
	u16 wvalue;
	u16 index;
	u16 len;
	__le32 data;
<<<<<<< HEAD
	struct intf_priv *pintfpriv = pintfhdl->pintfpriv;
=======
	struct intf_priv *intfpriv = intfhdl->pintfpriv;
>>>>>>> upstream/android-13

	request = 0x05;
	requesttype = 0x01; /* read_in */
	index = 0;
	wvalue = (u16)(addr & 0x0000ffff);
	len = 4;
<<<<<<< HEAD
	r8712_usbctrl_vendorreq(pintfpriv, request, wvalue, index, &data, len,
			  requesttype);
	return le32_to_cpu(data);
}

static void usb_write8(struct intf_hdl *pintfhdl, u32 addr, u8 val)
=======
	r8712_usbctrl_vendorreq(intfpriv, request, wvalue, index, &data, len,
				requesttype);
	return le32_to_cpu(data);
}

static void usb_write8(struct intf_hdl *intfhdl, u32 addr, u8 val)
>>>>>>> upstream/android-13
{
	u8 request;
	u8 requesttype;
	u16 wvalue;
	u16 index;
	u16 len;
	__le32 data;
<<<<<<< HEAD
	struct intf_priv *pintfpriv = pintfhdl->pintfpriv;
=======
	struct intf_priv *intfpriv = intfhdl->pintfpriv;
>>>>>>> upstream/android-13

	request = 0x05;
	requesttype = 0x00; /* write_out */
	index = 0;
	wvalue = (u16)(addr & 0x0000ffff);
	len = 1;
	data = cpu_to_le32((u32)val & 0x000000ff);
<<<<<<< HEAD
	r8712_usbctrl_vendorreq(pintfpriv, request, wvalue, index, &data, len,
			  requesttype);
}

static void usb_write16(struct intf_hdl *pintfhdl, u32 addr, u16 val)
=======
	r8712_usbctrl_vendorreq(intfpriv, request, wvalue, index, &data, len,
				requesttype);
}

static void usb_write16(struct intf_hdl *intfhdl, u32 addr, u16 val)
>>>>>>> upstream/android-13
{
	u8 request;
	u8 requesttype;
	u16 wvalue;
	u16 index;
	u16 len;
	__le32 data;
<<<<<<< HEAD
	struct intf_priv *pintfpriv = pintfhdl->pintfpriv;
=======
	struct intf_priv *intfpriv = intfhdl->pintfpriv;
>>>>>>> upstream/android-13

	request = 0x05;
	requesttype = 0x00; /* write_out */
	index = 0;
	wvalue = (u16)(addr & 0x0000ffff);
	len = 2;
	data = cpu_to_le32((u32)val & 0x0000ffff);
<<<<<<< HEAD
	r8712_usbctrl_vendorreq(pintfpriv, request, wvalue, index, &data, len,
			  requesttype);
}

static void usb_write32(struct intf_hdl *pintfhdl, u32 addr, u32 val)
=======
	r8712_usbctrl_vendorreq(intfpriv, request, wvalue, index, &data, len,
				requesttype);
}

static void usb_write32(struct intf_hdl *intfhdl, u32 addr, u32 val)
>>>>>>> upstream/android-13
{
	u8 request;
	u8 requesttype;
	u16 wvalue;
	u16 index;
	u16 len;
	__le32 data;
<<<<<<< HEAD
	struct intf_priv *pintfpriv = pintfhdl->pintfpriv;
=======
	struct intf_priv *intfpriv = intfhdl->pintfpriv;
>>>>>>> upstream/android-13

	request = 0x05;
	requesttype = 0x00; /* write_out */
	index = 0;
	wvalue = (u16)(addr & 0x0000ffff);
	len = 4;
	data = cpu_to_le32(val);
<<<<<<< HEAD
	r8712_usbctrl_vendorreq(pintfpriv, request, wvalue, index, &data, len,
			  requesttype);
}

void r8712_usb_set_intf_option(u32 *poption)
{
	*poption = ((*poption) | _INTF_ASYNC_);
=======
	r8712_usbctrl_vendorreq(intfpriv, request, wvalue, index, &data, len,
				requesttype);
}

void r8712_usb_set_intf_option(u32 *option)
{
	*option = ((*option) | _INTF_ASYNC_);
>>>>>>> upstream/android-13
}

static void usb_intf_hdl_init(u8 *priv)
{
}

static void usb_intf_hdl_unload(u8 *priv)
{
}

static void usb_intf_hdl_open(u8 *priv)
{
}

static void usb_intf_hdl_close(u8 *priv)
{
}

<<<<<<< HEAD
void r8712_usb_set_intf_funs(struct intf_hdl *pintf_hdl)
{
	pintf_hdl->intf_hdl_init = usb_intf_hdl_init;
	pintf_hdl->intf_hdl_unload = usb_intf_hdl_unload;
	pintf_hdl->intf_hdl_open = usb_intf_hdl_open;
	pintf_hdl->intf_hdl_close = usb_intf_hdl_close;
}

void r8712_usb_set_intf_ops(struct _io_ops	*pops)
{
	memset((u8 *)pops, 0, sizeof(struct _io_ops));
	pops->_read8 = usb_read8;
	pops->_read16 = usb_read16;
	pops->_read32 = usb_read32;
	pops->_read_port = r8712_usb_read_port;
	pops->_write8 = usb_write8;
	pops->_write16 = usb_write16;
	pops->_write32 = usb_write32;
	pops->_write_mem = r8712_usb_write_mem;
	pops->_write_port = r8712_usb_write_port;
=======
void r8712_usb_set_intf_funs(struct intf_hdl *intfhdl)
{
	intfhdl->intf_hdl_init = usb_intf_hdl_init;
	intfhdl->intf_hdl_unload = usb_intf_hdl_unload;
	intfhdl->intf_hdl_open = usb_intf_hdl_open;
	intfhdl->intf_hdl_close = usb_intf_hdl_close;
}

void r8712_usb_set_intf_ops(struct _io_ops *ops)
{
	memset((u8 *)ops, 0, sizeof(struct _io_ops));
	ops->_read8 = usb_read8;
	ops->_read16 = usb_read16;
	ops->_read32 = usb_read32;
	ops->_read_port = r8712_usb_read_port;
	ops->_write8 = usb_write8;
	ops->_write16 = usb_write16;
	ops->_write32 = usb_write32;
	ops->_write_mem = r8712_usb_write_mem;
	ops->_write_port = r8712_usb_write_port;
>>>>>>> upstream/android-13
}
