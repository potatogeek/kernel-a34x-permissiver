// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
/*

The purpose of rtw_io.c

a. provides the API

b. provides the protocol engine

c. provides the software interface between caller and the hardware interface


Compiler Flag Option:

1. CONFIG_SDIO_HCI:
    a. USE_SYNC_IRP:  Only sync operations are provided.
    b. USE_ASYNC_IRP:Both sync/async operations are provided.

jackson@realtek.com.tw

*/

<<<<<<< HEAD
#define _RTW_IO_C_

#include <drv_types.h>
#include <rtw_debug.h>

#define rtw_le16_to_cpu(val)		val
#define rtw_le32_to_cpu(val)		val
#define rtw_cpu_to_le16(val)		val
#define rtw_cpu_to_le32(val)		val

u8 _rtw_read8(struct adapter *adapter, u32 addr)
{
	u8 r_val;
=======
#include <drv_types.h>
#include <rtw_debug.h>

u8 rtw_read8(struct adapter *adapter, u32 addr)
{
>>>>>>> upstream/android-13
	/* struct	io_queue	*pio_queue = (struct io_queue *)adapter->pio_queue; */
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
	u8 (*_read8)(struct intf_hdl *pintfhdl, u32 addr);

	_read8 = pintfhdl->io_ops._read8;

<<<<<<< HEAD
	r_val = _read8(pintfhdl, addr);
	return r_val;
}

u16 _rtw_read16(struct adapter *adapter, u32 addr)
{
	u16 r_val;
=======
	return _read8(pintfhdl, addr);
}

u16 rtw_read16(struct adapter *adapter, u32 addr)
{
>>>>>>> upstream/android-13
	/* struct	io_queue	*pio_queue = (struct io_queue *)adapter->pio_queue; */
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
	u16 (*_read16)(struct intf_hdl *pintfhdl, u32 addr);

	_read16 = pintfhdl->io_ops._read16;

<<<<<<< HEAD
	r_val = _read16(pintfhdl, addr);
	return rtw_le16_to_cpu(r_val);
}

u32 _rtw_read32(struct adapter *adapter, u32 addr)
{
	u32 r_val;
=======
	return _read16(pintfhdl, addr);
}

u32 rtw_read32(struct adapter *adapter, u32 addr)
{
>>>>>>> upstream/android-13
	/* struct	io_queue	*pio_queue = (struct io_queue *)adapter->pio_queue; */
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
	u32 (*_read32)(struct intf_hdl *pintfhdl, u32 addr);

	_read32 = pintfhdl->io_ops._read32;

<<<<<<< HEAD
	r_val = _read32(pintfhdl, addr);
	return rtw_le32_to_cpu(r_val);

}

int _rtw_write8(struct adapter *adapter, u32 addr, u8 val)
=======
	return _read32(pintfhdl, addr);

}

int rtw_write8(struct adapter *adapter, u32 addr, u8 val)
>>>>>>> upstream/android-13
{
	/* struct	io_queue	*pio_queue = (struct io_queue *)adapter->pio_queue; */
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
	int (*_write8)(struct intf_hdl *pintfhdl, u32 addr, u8 val);
	int ret;

	_write8 = pintfhdl->io_ops._write8;

	ret = _write8(pintfhdl, addr, val);

	return RTW_STATUS_CODE(ret);
}
<<<<<<< HEAD
int _rtw_write16(struct adapter *adapter, u32 addr, u16 val)
=======
int rtw_write16(struct adapter *adapter, u32 addr, u16 val)
>>>>>>> upstream/android-13
{
	/* struct	io_queue	*pio_queue = (struct io_queue *)adapter->pio_queue; */
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
	int (*_write16)(struct intf_hdl *pintfhdl, u32 addr, u16 val);
	int ret;

	_write16 = pintfhdl->io_ops._write16;

	ret = _write16(pintfhdl, addr, val);
	return RTW_STATUS_CODE(ret);
}
<<<<<<< HEAD
int _rtw_write32(struct adapter *adapter, u32 addr, u32 val)
=======
int rtw_write32(struct adapter *adapter, u32 addr, u32 val)
>>>>>>> upstream/android-13
{
	/* struct	io_queue	*pio_queue = (struct io_queue *)adapter->pio_queue; */
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
	int (*_write32)(struct intf_hdl *pintfhdl, u32 addr, u32 val);
	int ret;

	_write32 = pintfhdl->io_ops._write32;

	ret = _write32(pintfhdl, addr, val);

	return RTW_STATUS_CODE(ret);
}

<<<<<<< HEAD
u8 _rtw_sd_f0_read8(struct adapter *adapter, u32 addr)
{
	u8 r_val = 0x00;
	struct io_priv *pio_priv = &adapter->iopriv;
	struct intf_hdl *pintfhdl = &(pio_priv->intf);
	u8 (*_sd_f0_read8)(struct intf_hdl *pintfhdl, u32 addr);

	_sd_f0_read8 = pintfhdl->io_ops._sd_f0_read8;

	if (_sd_f0_read8)
		r_val = _sd_f0_read8(pintfhdl, addr);
	else
		DBG_871X_LEVEL(_drv_warning_, FUNC_ADPT_FMT" _sd_f0_read8 callback is NULL\n", FUNC_ADPT_ARG(adapter));

	return r_val;
}

u32 _rtw_write_port(struct adapter *adapter, u32 addr, u32 cnt, u8 *pmem)
=======
u32 rtw_write_port(struct adapter *adapter, u32 addr, u32 cnt, u8 *pmem)
>>>>>>> upstream/android-13
{
	u32 (*_write_port)(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pmem);
	struct io_priv *pio_priv = &adapter->iopriv;
	struct	intf_hdl		*pintfhdl = &(pio_priv->intf);
<<<<<<< HEAD
	u32 ret = _SUCCESS;

	_write_port = pintfhdl->io_ops._write_port;

	ret = _write_port(pintfhdl, addr, cnt, pmem);

	return ret;
=======

	_write_port = pintfhdl->io_ops._write_port;

	return _write_port(pintfhdl, addr, cnt, pmem);
>>>>>>> upstream/android-13
}

int rtw_init_io_priv(struct adapter *padapter, void (*set_intf_ops)(struct adapter *padapter, struct _io_ops *pops))
{
	struct io_priv *piopriv = &padapter->iopriv;
	struct intf_hdl *pintf = &piopriv->intf;

<<<<<<< HEAD
	if (set_intf_ops == NULL)
=======
	if (!set_intf_ops)
>>>>>>> upstream/android-13
		return _FAIL;

	piopriv->padapter = padapter;
	pintf->padapter = padapter;
	pintf->pintf_dev = adapter_to_dvobj(padapter);

	set_intf_ops(padapter, &pintf->io_ops);

	return _SUCCESS;
}

/*
* Increase and check if the continual_io_error of this @param dvobjprive is larger than MAX_CONTINUAL_IO_ERR
* @return true:
* @return false:
*/
int rtw_inc_and_chk_continual_io_error(struct dvobj_priv *dvobj)
{
	int ret = false;
	int value = atomic_inc_return(&dvobj->continual_io_error);
<<<<<<< HEAD
	if (value > MAX_CONTINUAL_IO_ERR) {
		DBG_871X("[dvobj:%p][ERROR] continual_io_error:%d > %d\n", dvobj, value, MAX_CONTINUAL_IO_ERR);
		ret = true;
	} else {
		/* DBG_871X("[dvobj:%p] continual_io_error:%d\n", dvobj, value); */
	}
=======
	if (value > MAX_CONTINUAL_IO_ERR)
		ret = true;

>>>>>>> upstream/android-13
	return ret;
}

/*
* Set the continual_io_error of this @param dvobjprive to 0
*/
void rtw_reset_continual_io_error(struct dvobj_priv *dvobj)
{
	atomic_set(&dvobj->continual_io_error, 0);
}
