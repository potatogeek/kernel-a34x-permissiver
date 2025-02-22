// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 *******************************************************************************/
<<<<<<< HEAD
#define _SDIO_OPS_LINUX_C_
=======
>>>>>>> upstream/android-13

#include <drv_types.h>
#include <rtw_debug.h>

static bool rtw_sdio_claim_host_needed(struct sdio_func *func)
{
	struct dvobj_priv *dvobj = sdio_get_drvdata(func);
<<<<<<< HEAD
	PSDIO_DATA sdio_data = &dvobj->intf_data;
=======
	struct sdio_data *sdio_data = &dvobj->intf_data;
>>>>>>> upstream/android-13

	if (sdio_data->sys_sdio_irq_thd && sdio_data->sys_sdio_irq_thd == current)
		return false;
	return true;
}

inline void rtw_sdio_set_irq_thd(struct dvobj_priv *dvobj, void *thd_hdl)
{
<<<<<<< HEAD
	PSDIO_DATA sdio_data = &dvobj->intf_data;
=======
	struct sdio_data *sdio_data = &dvobj->intf_data;
>>>>>>> upstream/android-13

	sdio_data->sys_sdio_irq_thd = thd_hdl;
}

<<<<<<< HEAD
u8 sd_f0_read8(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u8 v = 0;
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return v;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_f0_readb(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x\n", __func__, *err, addr);
	return v;
}

=======
>>>>>>> upstream/android-13
/*
 * Return:
 *0		Success
 *others	Fail
 */
s32 _sd_cmd52_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	int err = 0, i;
	struct sdio_func *func;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;
>>>>>>> upstream/android-13

	func = psdio->func;

	for (i = 0; i < cnt; i++) {
<<<<<<< HEAD
		pdata[i] = sdio_readb(func, addr+i, &err);
		if (err) {
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x\n", __func__, err, addr+i);
			break;
		}
=======
		pdata[i] = sdio_readb(func, addr + i, &err);
		if (err)
			break;
>>>>>>> upstream/android-13
	}
	return err;
}

/*
 * Return:
 *0		Success
 *others	Fail
 */
s32 sd_cmd52_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	int err = 0;
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_cmd52_read(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);
	return err;
}

/*
 * Return:
 *0		Success
 *others	Fail
 */
s32 _sd_cmd52_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	int err = 0, i;
	struct sdio_func *func;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;
>>>>>>> upstream/android-13

	func = psdio->func;

	for (i = 0; i < cnt; i++) {
<<<<<<< HEAD
		sdio_writeb(func, pdata[i], addr+i, &err);
		if (err) {
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x val = 0x%02x\n", __func__, err, addr+i, pdata[i]);
			break;
		}
=======
		sdio_writeb(func, pdata[i], addr + i, &err);
		if (err)
			break;
>>>>>>> upstream/android-13
	}
	return err;
}

/*
 * Return:
 *0		Success
 *others	Fail
 */
s32 sd_cmd52_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	int err = 0;
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_cmd52_write(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);
	return err;
}

u8 sd_read8(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	u8 v = 0;
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return v;
	}
=======
	if (padapter->bSurpriseRemoved)
		return v;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_readb(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);
<<<<<<< HEAD
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x\n", __func__, *err, addr);
=======
>>>>>>> upstream/android-13
	return v;
}

u32 sd_read32(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13
	u32 v = 0;
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return v;
	}
=======
	if (padapter->bSurpriseRemoved)
		return v;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_readl(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);

<<<<<<< HEAD
	if (err && *err)
	{
		int i;

		DBG_871X(KERN_ERR "%s: (%d) addr = 0x%05x, val = 0x%x\n", __func__, *err, addr, v);

		*err = 0;
		for (i = 0; i < SD_IO_TRY_CNT; i++)
		{
			if (claim_needed) sdio_claim_host(func);
			v = sdio_readl(func, addr, err);
			if (claim_needed) sdio_release_host(func);
=======
	if (err && *err) {
		int i;

		*err = 0;
		for (i = 0; i < SD_IO_TRY_CNT; i++) {
			if (claim_needed)
				sdio_claim_host(func);
			v = sdio_readl(func, addr, err);
			if (claim_needed)
				sdio_release_host(func);
>>>>>>> upstream/android-13

			if (*err == 0) {
				rtw_reset_continual_io_error(psdiodev);
				break;
			} else {
<<<<<<< HEAD
				DBG_871X(KERN_ERR "%s: (%d) addr = 0x%05x, val = 0x%x, try_cnt =%d\n", __func__, *err, addr, v, i);
				if ((-ESHUTDOWN == *err) || (-ENODEV == *err)) {
					padapter->bSurpriseRemoved = true;
				}
=======
				if ((-ESHUTDOWN == *err) || (-ENODEV == *err))
					padapter->bSurpriseRemoved = true;
>>>>>>> upstream/android-13

				if (rtw_inc_and_chk_continual_io_error(psdiodev) == true) {
					padapter->bSurpriseRemoved = true;
					break;
				}
			}
		}
<<<<<<< HEAD

		if (i == SD_IO_TRY_CNT)
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x, val = 0x%x, try_cnt =%d\n", __func__, *err, addr, v, i);
		else
			DBG_871X(KERN_ERR "%s: (%d) addr = 0x%05x, val = 0x%x, try_cnt =%d\n", __func__, *err, addr, v, i);

=======
>>>>>>> upstream/android-13
	}
	return  v;
}

void sd_write8(struct intf_hdl *pintfhdl, u32 addr, u8 v, s32 *err)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return;
	}
=======
	if (padapter->bSurpriseRemoved)
		return;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	sdio_writeb(func, v, addr, err);
	if (claim_needed)
		sdio_release_host(func);
<<<<<<< HEAD
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x val = 0x%02x\n", __func__, *err, addr, v);
=======
>>>>>>> upstream/android-13
}

void sd_write32(struct intf_hdl *pintfhdl, u32 addr, u32 v, s32 *err)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13
	struct sdio_func *func;
	bool claim_needed;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return;
	}
=======
	if (padapter->bSurpriseRemoved)
		return;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	sdio_writel(func, v, addr, err);
	if (claim_needed)
		sdio_release_host(func);

<<<<<<< HEAD
	if (err && *err)
	{
		int i;

		DBG_871X(KERN_ERR "%s: (%d) addr = 0x%05x val = 0x%08x\n", __func__, *err, addr, v);

		*err = 0;
		for (i = 0; i < SD_IO_TRY_CNT; i++)
		{
			if (claim_needed) sdio_claim_host(func);
			sdio_writel(func, v, addr, err);
			if (claim_needed) sdio_release_host(func);
=======
	if (err && *err) {
		int i;

		*err = 0;
		for (i = 0; i < SD_IO_TRY_CNT; i++) {
			if (claim_needed)
				sdio_claim_host(func);
			sdio_writel(func, v, addr, err);
			if (claim_needed)
				sdio_release_host(func);
>>>>>>> upstream/android-13
			if (*err == 0) {
				rtw_reset_continual_io_error(psdiodev);
				break;
			} else {
<<<<<<< HEAD
				DBG_871X(KERN_ERR "%s: (%d) addr = 0x%05x, val = 0x%x, try_cnt =%d\n", __func__, *err, addr, v, i);
				if ((-ESHUTDOWN == *err) || (-ENODEV == *err)) {
					padapter->bSurpriseRemoved = true;
				}
=======
				if ((-ESHUTDOWN == *err) || (-ENODEV == *err))
					padapter->bSurpriseRemoved = true;
>>>>>>> upstream/android-13

				if (rtw_inc_and_chk_continual_io_error(psdiodev) == true) {
					padapter->bSurpriseRemoved = true;
					break;
				}
			}
		}

<<<<<<< HEAD
		if (i == SD_IO_TRY_CNT)
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x val = 0x%08x, try_cnt =%d\n", __func__, *err, addr, v, i);
		else
			DBG_871X(KERN_ERR "%s: (%d) addr = 0x%05x val = 0x%08x, try_cnt =%d\n", __func__, *err, addr, v, i);
=======
>>>>>>> upstream/android-13
	}
}

/*
 * Use CMD53 to read data from SDIO device.
 * This function MUST be called after sdio_claim_host() or
 * in SDIO ISR(host had been claimed).
 *
 * Parameters:
 *psdio	pointer of SDIO_DATA
 *addr	address to read
 *cnt		amount to read
 *pdata	pointer to put data, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *0		Success
 *others	Fail
 */
s32 _sd_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	int err = -EPERM;
	struct sdio_func *func;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}

	func = psdio->func;

	if (unlikely((cnt == 1) || (cnt == 2)))
	{
		int i;
		u8 *pbuf = pdata;

		for (i = 0; i < cnt; i++)
		{
			*(pbuf+i) = sdio_readb(func, addr+i, &err);

			if (err) {
				DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x\n", __func__, err, addr);
				break;
			}
=======
	if (padapter->bSurpriseRemoved)
		return err;

	func = psdio->func;

	if (unlikely((cnt == 1) || (cnt == 2))) {
		int i;
		u8 *pbuf = pdata;

		for (i = 0; i < cnt; i++) {
			*(pbuf + i) = sdio_readb(func, addr + i, &err);

			if (err)
				break;
>>>>>>> upstream/android-13
		}
		return err;
	}

	err = sdio_memcpy_fromio(func, pdata, addr, cnt);
<<<<<<< HEAD
	if (err) {
		DBG_871X(KERN_ERR "%s: FAIL(%d)! ADDR =%#x Size =%d\n", __func__, err, addr, cnt);
	}
=======

>>>>>>> upstream/android-13
	return err;
}

/*
 * Use CMD53 to read data from SDIO device.
 *
 * Parameters:
 *psdio	pointer of SDIO_DATA
 *addr	address to read
 *cnt		amount to read
 *pdata	pointer to put data, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *0		Success
 *others	Fail
 */
s32 sd_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	struct sdio_func *func;
	bool claim_needed;
	s32 err = -EPERM;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;

>>>>>>> upstream/android-13
	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_read(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);
	return err;
}

/*
 * Use CMD53 to write data to SDIO device.
 * This function MUST be called after sdio_claim_host() or
 * in SDIO ISR(host had been claimed).
 *
 * Parameters:
 *psdio	pointer of SDIO_DATA
 *addr	address to write
 *cnt		amount to write
 *pdata	data pointer, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *0		Success
 *others	Fail
 */
s32 _sd_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13

	struct sdio_func *func;
	u32 size;
	s32 err =  -EPERM;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;
>>>>>>> upstream/android-13

	func = psdio->func;
/*	size = sdio_align_size(func, cnt); */

<<<<<<< HEAD
	if (unlikely((cnt == 1) || (cnt == 2)))
	{
		int i;
		u8 *pbuf = pdata;

		for (i = 0; i < cnt; i++)
		{
			sdio_writeb(func, *(pbuf+i), addr+i, &err);
			if (err) {
				DBG_871X(KERN_ERR "%s: FAIL!(%d) addr = 0x%05x val = 0x%02x\n", __func__, err, addr, *(pbuf+i));
				break;
			}
=======
	if (unlikely((cnt == 1) || (cnt == 2))) {
		int i;
		u8 *pbuf = pdata;

		for (i = 0; i < cnt; i++) {
			sdio_writeb(func, *(pbuf + i), addr + i, &err);
			if (err)
				break;
>>>>>>> upstream/android-13
		}

		return err;
	}

	size = cnt;
	err = sdio_memcpy_toio(func, addr, pdata, size);
<<<<<<< HEAD
	if (err) {
		DBG_871X(KERN_ERR "%s: FAIL(%d)! ADDR =%#x Size =%d(%d)\n", __func__, err, addr, cnt, size);
	}
=======

>>>>>>> upstream/android-13
	return err;
}

/*
 * Use CMD53 to write data to SDIO device.
 *
 * Parameters:
 *  psdio	pointer of SDIO_DATA
 *  addr	address to write
 *  cnt		amount to write
 *  pdata	data pointer, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *  0		Success
 *  others	Fail
 */
s32 sd_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	struct adapter *padapter;
	struct dvobj_priv *psdiodev;
<<<<<<< HEAD
	PSDIO_DATA psdio;
=======
	struct sdio_data *psdio;
>>>>>>> upstream/android-13
	struct sdio_func *func;
	bool claim_needed;
	s32 err =  -EPERM;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved) {
		/* DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n", __func__); */
		return err;
	}
=======
	if (padapter->bSurpriseRemoved)
		return err;
>>>>>>> upstream/android-13

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_write(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);
	return err;
}
