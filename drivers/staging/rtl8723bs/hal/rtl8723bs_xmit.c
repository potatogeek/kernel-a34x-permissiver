// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
<<<<<<< HEAD
#define _RTL8723BS_XMIT_C_
=======
>>>>>>> upstream/android-13

#include <drv_types.h>
#include <rtw_debug.h>
#include <rtl8723b_hal.h>

static u8 rtw_sdio_wait_enough_TxOQT_space(struct adapter *padapter, u8 agg_num)
{
	u32 n = 0;
	struct hal_com_data *pHalData = GET_HAL_DATA(padapter);

	while (pHalData->SdioTxOQTFreeSpace < agg_num) {
		if (
<<<<<<< HEAD
			(padapter->bSurpriseRemoved == true) ||
			(padapter->bDriverStopped == true)
		) {
			DBG_871X("%s: bSurpriseRemoved or bDriverStopped (wait TxOQT)\n", __func__);
			return false;
		}
=======
			(padapter->bSurpriseRemoved) ||
			(padapter->bDriverStopped)
		)
			return false;
>>>>>>> upstream/android-13

		HalQueryTxOQTBufferStatus8723BSdio(padapter);

		if ((++n % 60) == 0) {
<<<<<<< HEAD
			if ((n % 300) == 0) {
				DBG_871X("%s(%d): QOT free space(%d), agg_num: %d\n",
				__func__, n, pHalData->SdioTxOQTFreeSpace, agg_num);
			}
=======
>>>>>>> upstream/android-13
			msleep(1);
			/* yield(); */
		}
	}

	pHalData->SdioTxOQTFreeSpace -= agg_num;

	/* if (n > 1) */
	/* 	++priv->pshare->nr_out_of_txoqt_space; */

	return true;
}

static s32 rtl8723_dequeue_writeport(struct adapter *padapter)
{
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(padapter);
	struct xmit_buf *pxmitbuf;
	struct adapter *pri_padapter = padapter;
	s32 ret = 0;
	u8 PageIdx = 0;
	u32 deviceId;
	u8 bUpdatePageNum = false;

	ret = ret || check_fwstate(pmlmepriv, _FW_UNDER_SURVEY);

<<<<<<< HEAD
	if (true == ret)
=======
	if (ret)
>>>>>>> upstream/android-13
		pxmitbuf = dequeue_pending_xmitbuf_under_survey(pxmitpriv);
	else
		pxmitbuf = dequeue_pending_xmitbuf(pxmitpriv);

<<<<<<< HEAD
	if (pxmitbuf == NULL)
=======
	if (!pxmitbuf)
>>>>>>> upstream/android-13
		return true;

	deviceId = ffaddr2deviceId(pdvobjpriv, pxmitbuf->ff_hwaddr);

	/*  translate fifo addr to queue index */
	switch (deviceId) {
	case WLAN_TX_HIQ_DEVICE_ID:
		PageIdx = HI_QUEUE_IDX;
		break;

	case WLAN_TX_MIQ_DEVICE_ID:
		PageIdx = MID_QUEUE_IDX;
		break;

	case WLAN_TX_LOQ_DEVICE_ID:
		PageIdx = LOW_QUEUE_IDX;
		break;
	}

query_free_page:
	/*  check if hardware tx fifo page is enough */
<<<<<<< HEAD
	if (false == rtw_hal_sdio_query_tx_freepage(pri_padapter, PageIdx, pxmitbuf->pg_num)) {
=======
	if (!rtw_hal_sdio_query_tx_freepage(pri_padapter, PageIdx, pxmitbuf->pg_num)) {
>>>>>>> upstream/android-13
		if (!bUpdatePageNum) {
			/*  Total number of page is NOT available, so update current FIFO status */
			HalQueryTxBufferStatus8723BSdio(padapter);
			bUpdatePageNum = true;
			goto query_free_page;
		} else {
			bUpdatePageNum = false;
			enqueue_pending_xmitbuf_to_head(pxmitpriv, pxmitbuf);
			return true;
		}
	}

	if (
<<<<<<< HEAD
		(padapter->bSurpriseRemoved == true) ||
		(padapter->bDriverStopped == true)
	) {
		RT_TRACE(
			_module_hal_xmit_c_,
			_drv_notice_,
			("%s: bSurpriseRemoved(write port)\n", __func__)
		);
		goto free_xmitbuf;
	}
=======
		(padapter->bSurpriseRemoved) ||
		(padapter->bDriverStopped)
	)
		goto free_xmitbuf;
>>>>>>> upstream/android-13

	if (rtw_sdio_wait_enough_TxOQT_space(padapter, pxmitbuf->agg_num) == false)
		goto free_xmitbuf;

	traffic_check_for_leave_lps(padapter, true, pxmitbuf->agg_num);

	rtw_write_port(padapter, deviceId, pxmitbuf->len, (u8 *)pxmitbuf);

	rtw_hal_sdio_update_tx_freepage(pri_padapter, PageIdx, pxmitbuf->pg_num);

free_xmitbuf:
	/* rtw_free_xmitframe(pxmitpriv, pframe); */
	/* pxmitbuf->priv_data = NULL; */
	rtw_free_xmitbuf(pxmitpriv, pxmitbuf);

<<<<<<< HEAD
#ifdef CONFIG_SDIO_TX_TASKLET
	tasklet_hi_schedule(&pxmitpriv->xmit_tasklet);
#endif

=======
>>>>>>> upstream/android-13
	return _FAIL;
}

/*
 * Description
 *Transmit xmitbuf to hardware tx fifo
 *
 * Return
 *_SUCCESS	ok
 *_FAIL		something error
 */
s32 rtl8723bs_xmit_buf_handler(struct adapter *padapter)
{
	struct xmit_priv *pxmitpriv;
	u8 queue_empty, queue_pending;
	s32 ret;


	pxmitpriv = &padapter->xmitpriv;

<<<<<<< HEAD
	if (down_interruptible(&pxmitpriv->xmit_sema)) {
		DBG_871X_LEVEL(_drv_emerg_, "%s: down SdioXmitBufSema fail!\n", __func__);
		return _FAIL;
	}

	ret = (padapter->bDriverStopped == true) || (padapter->bSurpriseRemoved == true);
	if (ret) {
		RT_TRACE(
			_module_hal_xmit_c_,
			_drv_err_,
			(
				"%s: bDriverStopped(%d) bSurpriseRemoved(%d)!\n",
				__func__,
				padapter->bDriverStopped,
				padapter->bSurpriseRemoved
			)
		);
		return _FAIL;
	}

	queue_pending = check_pending_xmitbuf(pxmitpriv);

	if (queue_pending == false)
=======
	if (wait_for_completion_interruptible(&pxmitpriv->xmit_comp)) {
		netdev_emerg(padapter->pnetdev,
			     "%s: down SdioXmitBufSema fail!\n", __func__);
		return _FAIL;
	}

	ret = (padapter->bDriverStopped) || (padapter->bSurpriseRemoved);
	if (ret)
		return _FAIL;

	queue_pending = check_pending_xmitbuf(pxmitpriv);

	if (!queue_pending)
>>>>>>> upstream/android-13
		return _SUCCESS;

	ret = rtw_register_tx_alive(padapter);
	if (ret != _SUCCESS) {
		return _SUCCESS;
	}

	do {
		queue_empty = rtl8723_dequeue_writeport(padapter);
/* 	dump secondary adapter xmitbuf */
	} while (!queue_empty);

	rtw_unregister_tx_alive(padapter);

	return _SUCCESS;
}

/*
 * Description:
 *Aggregation packets and send to hardware
 *
 * Return:
 *0	Success
 *-1	Hardware resource(TX FIFO) not ready
 *-2	Software resource(xmitbuf) not ready
 */
static s32 xmit_xmitframes(struct adapter *padapter, struct xmit_priv *pxmitpriv)
{
	s32 err, ret;
	u32 k = 0;
	struct hw_xmit *hwxmits, *phwxmit;
<<<<<<< HEAD
	u8 no_res, idx, hwentry;
	struct tx_servq *ptxservq;
	struct list_head *sta_plist, *sta_phead, *frame_plist, *frame_phead;
=======
	u8 idx, hwentry;
	struct tx_servq *ptxservq;
	struct list_head *sta_plist, *sta_phead, *frame_plist, *frame_phead, *tmp;
>>>>>>> upstream/android-13
	struct xmit_frame *pxmitframe;
	struct __queue *pframe_queue;
	struct xmit_buf *pxmitbuf;
	u32 txlen, max_xmit_len;
	u8 txdesc_size = TXDESC_SIZE;
	int inx[4];

	err = 0;
<<<<<<< HEAD
	no_res = false;
=======
>>>>>>> upstream/android-13
	hwxmits = pxmitpriv->hwxmits;
	hwentry = pxmitpriv->hwxmit_entry;
	ptxservq = NULL;
	pxmitframe = NULL;
	pframe_queue = NULL;
	pxmitbuf = NULL;

	if (padapter->registrypriv.wifi_spec == 1) {
		for (idx = 0; idx < 4; idx++)
			inx[idx] = pxmitpriv->wmm_para_seq[idx];
	} else {
		inx[0] = 0;
		inx[1] = 1;
		inx[2] = 2;
		inx[3] = 3;
	}

	/*  0(VO), 1(VI), 2(BE), 3(BK) */
	for (idx = 0; idx < hwentry; idx++) {
		phwxmit = hwxmits + inx[idx];

		if (
<<<<<<< HEAD
			(check_pending_xmitbuf(pxmitpriv) == true) &&
			(padapter->mlmepriv.LinkDetectInfo.bHigherBusyTxTraffic == true)
=======
			(check_pending_xmitbuf(pxmitpriv)) &&
			(padapter->mlmepriv.LinkDetectInfo.bHigherBusyTxTraffic)
>>>>>>> upstream/android-13
		) {
			if ((phwxmit->accnt > 0) && (phwxmit->accnt < 5)) {
				err = -2;
				break;
			}
		}

		max_xmit_len = rtw_hal_get_sdio_tx_max_length(padapter, inx[idx]);

		spin_lock_bh(&pxmitpriv->lock);

		sta_phead = get_list_head(phwxmit->sta_queue);
<<<<<<< HEAD
		sta_plist = get_next(sta_phead);
		/* because stop_sta_xmit may delete sta_plist at any time */
		/* so we should add lock here, or while loop can not exit */
		while (sta_phead != sta_plist) {
			ptxservq = LIST_CONTAINOR(sta_plist, struct tx_servq, tx_pending);
			sta_plist = get_next(sta_plist);

#ifdef DBG_XMIT_BUF
			DBG_871X(
				"%s idx:%d hwxmit_pkt_num:%d ptxservq_pkt_num:%d\n",
				__func__,
				idx,
				phwxmit->accnt,
				ptxservq->qcnt
			);
			DBG_871X(
				"%s free_xmit_extbuf_cnt =%d free_xmitbuf_cnt =%d free_xmitframe_cnt =%d\n",
				__func__,
				pxmitpriv->free_xmit_extbuf_cnt,
				pxmitpriv->free_xmitbuf_cnt,
				pxmitpriv->free_xmitframe_cnt
			);
#endif
=======
		/* because stop_sta_xmit may delete sta_plist at any time */
		/* so we should add lock here, or while loop can not exit */
		list_for_each_safe(sta_plist, tmp, sta_phead) {
			ptxservq = list_entry(sta_plist, struct tx_servq,
					      tx_pending);

>>>>>>> upstream/android-13
			pframe_queue = &ptxservq->sta_pending;

			frame_phead = get_list_head(pframe_queue);

			while (list_empty(frame_phead) == false) {
				frame_plist = get_next(frame_phead);
<<<<<<< HEAD
				pxmitframe = LIST_CONTAINOR(frame_plist, struct xmit_frame, list);

				/*  check xmit_buf size enough or not */
				txlen = txdesc_size + rtw_wlan_pkt_size(pxmitframe);
				if (
					(NULL == pxmitbuf) ||
					((_RND(pxmitbuf->len, 8) + txlen) > max_xmit_len) ||
					(k >= (rtw_hal_sdio_max_txoqt_free_space(padapter)-1))
=======
				pxmitframe = container_of(frame_plist, struct xmit_frame, list);

				/*  check xmit_buf size enough or not */
				txlen = txdesc_size + rtw_wlan_pkt_size(pxmitframe);
				if (!pxmitbuf ||
					((_RND(pxmitbuf->len, 8) + txlen) > max_xmit_len) ||
					(k >= (rtw_hal_sdio_max_txoqt_free_space(padapter) - 1))
>>>>>>> upstream/android-13
				) {
					if (pxmitbuf) {
						/* pxmitbuf->priv_data will be NULL, and will crash here */
						if (pxmitbuf->len > 0 &&
						    pxmitbuf->priv_data) {
							struct xmit_frame *pframe;
							pframe = (struct xmit_frame *)pxmitbuf->priv_data;
							pframe->agg_num = k;
							pxmitbuf->agg_num = k;
							rtl8723b_update_txdesc(pframe, pframe->buf_addr);
							rtw_free_xmitframe(pxmitpriv, pframe);
							pxmitbuf->priv_data = NULL;
							enqueue_pending_xmitbuf(pxmitpriv, pxmitbuf);
							/* can not yield under lock */
							/* yield(); */
						} else
							rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
					}

					pxmitbuf = rtw_alloc_xmitbuf(pxmitpriv);
<<<<<<< HEAD
					if (pxmitbuf == NULL) {
#ifdef DBG_XMIT_BUF
						DBG_871X_LEVEL(_drv_err_, "%s: xmit_buf is not enough!\n", __func__);
#endif
						err = -2;
						up(&(pxmitpriv->xmit_sema));
=======
					if (!pxmitbuf) {
#ifdef DBG_XMIT_BUF
						netdev_err(padapter->pnetdev,
							   "%s: xmit_buf is not enough!\n",
							   __func__);
#endif
						err = -2;
						complete(&(pxmitpriv->xmit_comp));
>>>>>>> upstream/android-13
						break;
					}
					k = 0;
				}

				/*  ok to send, remove frame from queue */
<<<<<<< HEAD
				if (check_fwstate(&padapter->mlmepriv, WIFI_AP_STATE) == true) {
					if (
						(pxmitframe->attrib.psta->state & WIFI_SLEEP_STATE) &&
						(pxmitframe->attrib.triggered == 0)
					) {
						DBG_871X(
							"%s: one not triggered pkt in queue when this STA sleep,"
							" break and goto next sta\n",
							__func__
						);
						break;
					}
				}
=======
				if (check_fwstate(&padapter->mlmepriv, WIFI_AP_STATE) == true)
					if (
						(pxmitframe->attrib.psta->state & WIFI_SLEEP_STATE) &&
						(pxmitframe->attrib.triggered == 0)
					)
						break;
>>>>>>> upstream/android-13

				list_del_init(&pxmitframe->list);
				ptxservq->qcnt--;
				phwxmit->accnt--;

				if (k == 0) {
					pxmitbuf->ff_hwaddr = rtw_get_ff_hwaddr(pxmitframe);
					pxmitbuf->priv_data = (u8 *)pxmitframe;
				}

				/*  coalesce the xmitframe to xmitbuf */
				pxmitframe->pxmitbuf = pxmitbuf;
				pxmitframe->buf_addr = pxmitbuf->ptail;

				ret = rtw_xmitframe_coalesce(padapter, pxmitframe->pkt, pxmitframe);
				if (ret == _FAIL) {
<<<<<<< HEAD
					DBG_871X_LEVEL(_drv_err_, "%s: coalesce FAIL!", __func__);
=======
					netdev_err(padapter->pnetdev,
						   "%s: coalesce FAIL!",
						   __func__);
>>>>>>> upstream/android-13
					/*  Todo: error handler */
				} else {
					k++;
					if (k != 1)
						rtl8723b_update_txdesc(pxmitframe, pxmitframe->buf_addr);
					rtw_count_tx_stats(padapter, pxmitframe, pxmitframe->attrib.last_txcmdsz);

					txlen = txdesc_size + pxmitframe->attrib.last_txcmdsz;
<<<<<<< HEAD
					pxmitframe->pg_num = (txlen + 127)/128;
					pxmitbuf->pg_num += (txlen + 127)/128;
=======
					pxmitframe->pg_num = (txlen + 127) / 128;
					pxmitbuf->pg_num += (txlen + 127) / 128;
>>>>>>> upstream/android-13
				    /* if (k != 1) */
					/* 	((struct xmit_frame*)pxmitbuf->priv_data)->pg_num += pxmitframe->pg_num; */
					pxmitbuf->ptail += _RND(txlen, 8); /*  round to 8 bytes alignment */
					pxmitbuf->len = _RND(pxmitbuf->len, 8) + txlen;
				}

				if (k != 1)
					rtw_free_xmitframe(pxmitpriv, pxmitframe);
				pxmitframe = NULL;
			}

			if (list_empty(&pframe_queue->queue))
				list_del_init(&ptxservq->tx_pending);

			if (err)
				break;
		}
		spin_unlock_bh(&pxmitpriv->lock);

		/*  dump xmit_buf to hw tx fifo */
		if (pxmitbuf) {
<<<<<<< HEAD
			RT_TRACE(_module_hal_xmit_c_, _drv_info_, ("pxmitbuf->len =%d enqueue\n", pxmitbuf->len));

=======
>>>>>>> upstream/android-13
			if (pxmitbuf->len > 0) {
				struct xmit_frame *pframe;
				pframe = (struct xmit_frame *)pxmitbuf->priv_data;
				pframe->agg_num = k;
				pxmitbuf->agg_num = k;
				rtl8723b_update_txdesc(pframe, pframe->buf_addr);
				rtw_free_xmitframe(pxmitpriv, pframe);
				pxmitbuf->priv_data = NULL;
				enqueue_pending_xmitbuf(pxmitpriv, pxmitbuf);
				yield();
			} else
				rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
			pxmitbuf = NULL;
		}

		if (err)
			break;
	}

	return err;
}

/*
 * Description
 *Transmit xmitframe from queue
 *
 * Return
 *_SUCCESS	ok
 *_FAIL		something error
 */
static s32 rtl8723bs_xmit_handler(struct adapter *padapter)
{
	struct xmit_priv *pxmitpriv;
	s32 ret;


	pxmitpriv = &padapter->xmitpriv;

<<<<<<< HEAD
	if (down_interruptible(&pxmitpriv->SdioXmitSema)) {
		DBG_871X_LEVEL(_drv_emerg_, "%s: down sema fail!\n", __func__);
=======
	if (wait_for_completion_interruptible(&pxmitpriv->SdioXmitStart)) {
		netdev_emerg(padapter->pnetdev, "%s: SdioXmitStart fail!\n",
			     __func__);
>>>>>>> upstream/android-13
		return _FAIL;
	}

next:
	if (
<<<<<<< HEAD
		(padapter->bDriverStopped == true) ||
		(padapter->bSurpriseRemoved == true)
	) {
		RT_TRACE(
			_module_hal_xmit_c_,
			_drv_notice_,
			(
				"%s: bDriverStopped(%d) bSurpriseRemoved(%d)\n",
				__func__,
				padapter->bDriverStopped,
				padapter->bSurpriseRemoved
			)
		);
		return _FAIL;
	}
=======
		(padapter->bDriverStopped) ||
		(padapter->bSurpriseRemoved)
	)
		return _FAIL;
>>>>>>> upstream/android-13

	spin_lock_bh(&pxmitpriv->lock);
	ret = rtw_txframes_pending(padapter);
	spin_unlock_bh(&pxmitpriv->lock);
	if (ret == 0) {
		return _SUCCESS;
	}

	/*  dequeue frame and write to hardware */

	ret = xmit_xmitframes(padapter, pxmitpriv);
	if (ret == -2) {
		/* here sleep 1ms will cause big TP loss of TX */
		/* from 50+ to 40+ */
		if (padapter->registrypriv.wifi_spec)
			msleep(1);
		else
			yield();
		goto next;
	}

	spin_lock_bh(&pxmitpriv->lock);
	ret = rtw_txframes_pending(padapter);
	spin_unlock_bh(&pxmitpriv->lock);
	if (ret == 1) {
		goto next;
	}

	return _SUCCESS;
}

int rtl8723bs_xmit_thread(void *context)
{
	s32 ret;
	struct adapter *padapter;
	struct xmit_priv *pxmitpriv;
	u8 thread_name[20];

	ret = _SUCCESS;
	padapter = context;
	pxmitpriv = &padapter->xmitpriv;

<<<<<<< HEAD
	rtw_sprintf(thread_name, 20, "RTWHALXT-" ADPT_FMT, ADPT_ARG(padapter));
	thread_enter(thread_name);

	DBG_871X("start "FUNC_ADPT_FMT"\n", FUNC_ADPT_ARG(padapter));

	/*  For now, no one would down sema to check thread is running, */
	/*  so mark this temporary, Lucas@20130820 */
/* 	up(&pxmitpriv->SdioXmitTerminateSema); */

=======
	rtw_sprintf(thread_name, 20, "RTWHALXT-%s", ADPT_ARG(padapter));
	thread_enter(thread_name);

>>>>>>> upstream/android-13
	do {
		ret = rtl8723bs_xmit_handler(padapter);
		if (signal_pending(current)) {
			flush_signals(current);
		}
	} while (_SUCCESS == ret);

<<<<<<< HEAD
	up(&pxmitpriv->SdioXmitTerminateSema);

	RT_TRACE(_module_hal_xmit_c_, _drv_notice_, ("-%s\n", __func__));
=======
	complete(&pxmitpriv->SdioXmitTerminate);
>>>>>>> upstream/android-13

	thread_exit();
}

s32 rtl8723bs_mgnt_xmit(
	struct adapter *padapter, struct xmit_frame *pmgntframe
)
{
	s32 ret = _SUCCESS;
	struct pkt_attrib *pattrib;
	struct xmit_buf *pxmitbuf;
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(padapter);
	u8 *pframe = (u8 *)(pmgntframe->buf_addr) + TXDESC_OFFSET;
	u8 txdesc_size = TXDESC_SIZE;

<<<<<<< HEAD
	RT_TRACE(_module_hal_xmit_c_, _drv_info_, ("+%s\n", __func__));

=======
>>>>>>> upstream/android-13
	pattrib = &pmgntframe->attrib;
	pxmitbuf = pmgntframe->pxmitbuf;

	rtl8723b_update_txdesc(pmgntframe, pmgntframe->buf_addr);

	pxmitbuf->len = txdesc_size + pattrib->last_txcmdsz;
<<<<<<< HEAD
	pxmitbuf->pg_num = (pxmitbuf->len + 127)/128; /*  128 is tx page size */
=======
	pxmitbuf->pg_num = (pxmitbuf->len + 127) / 128; /*  128 is tx page size */
>>>>>>> upstream/android-13
	pxmitbuf->ptail = pmgntframe->buf_addr + pxmitbuf->len;
	pxmitbuf->ff_hwaddr = rtw_get_ff_hwaddr(pmgntframe);

	rtw_count_tx_stats(padapter, pmgntframe, pattrib->last_txcmdsz);

	rtw_free_xmitframe(pxmitpriv, pmgntframe);

	pxmitbuf->priv_data = NULL;

	if (GetFrameSubType(pframe) == WIFI_BEACON) { /* dump beacon directly */
		ret = rtw_write_port(padapter, pdvobjpriv->Queue2Pipe[pxmitbuf->ff_hwaddr], pxmitbuf->len, (u8 *)pxmitbuf);
		if (ret != _SUCCESS)
			rtw_sctx_done_err(&pxmitbuf->sctx, RTW_SCTX_DONE_WRITE_PORT_ERR);

		rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
	} else
		enqueue_pending_xmitbuf(pxmitpriv, pxmitbuf);

	return ret;
}

/*
 * Description:
 *Handle xmitframe(packet) come from rtw_xmit()
 *
 * Return:
 *true	dump packet directly ok
 *false	enqueue, temporary can't transmit packets to hardware
 */
s32 rtl8723bs_hal_xmit(
	struct adapter *padapter, struct xmit_frame *pxmitframe
)
{
	struct xmit_priv *pxmitpriv;
	s32 err;


	pxmitframe->attrib.qsel = pxmitframe->attrib.priority;
	pxmitpriv = &padapter->xmitpriv;

	if (
		(pxmitframe->frame_tag == DATA_FRAMETAG) &&
		(pxmitframe->attrib.ether_type != 0x0806) &&
		(pxmitframe->attrib.ether_type != 0x888e) &&
		(pxmitframe->attrib.dhcp_pkt != 1)
	) {
<<<<<<< HEAD
		if (padapter->mlmepriv.LinkDetectInfo.bBusyTraffic == true)
=======
		if (padapter->mlmepriv.LinkDetectInfo.bBusyTraffic)
>>>>>>> upstream/android-13
			rtw_issue_addbareq_cmd(padapter, pxmitframe);
	}

	spin_lock_bh(&pxmitpriv->lock);
	err = rtw_xmitframe_enqueue(padapter, pxmitframe);
	spin_unlock_bh(&pxmitpriv->lock);
	if (err != _SUCCESS) {
<<<<<<< HEAD
		RT_TRACE(_module_hal_xmit_c_, _drv_err_, ("rtl8723bs_hal_xmit: enqueue xmitframe fail\n"));
=======
>>>>>>> upstream/android-13
		rtw_free_xmitframe(pxmitpriv, pxmitframe);

		pxmitpriv->tx_drop++;
		return true;
	}

<<<<<<< HEAD
	up(&pxmitpriv->SdioXmitSema);
=======
	complete(&pxmitpriv->SdioXmitStart);
>>>>>>> upstream/android-13

	return false;
}

s32	rtl8723bs_hal_xmitframe_enqueue(
	struct adapter *padapter, struct xmit_frame *pxmitframe
)
{
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	s32 err;

	err = rtw_xmitframe_enqueue(padapter, pxmitframe);
	if (err != _SUCCESS) {
		rtw_free_xmitframe(pxmitpriv, pxmitframe);

		pxmitpriv->tx_drop++;
	} else {
<<<<<<< HEAD
#ifdef CONFIG_SDIO_TX_TASKLET
		tasklet_hi_schedule(&pxmitpriv->xmit_tasklet);
#else
		up(&pxmitpriv->SdioXmitSema);
#endif
=======
		complete(&pxmitpriv->SdioXmitStart);
>>>>>>> upstream/android-13
	}

	return err;

}

/*
 * Return
 *_SUCCESS	start thread ok
 *_FAIL		start thread fail
 *
 */
s32 rtl8723bs_init_xmit_priv(struct adapter *padapter)
{
	struct xmit_priv *xmitpriv = &padapter->xmitpriv;
	struct hal_com_data *phal;


	phal = GET_HAL_DATA(padapter);

	spin_lock_init(&phal->SdioTxFIFOFreePageLock);
<<<<<<< HEAD
	sema_init(&xmitpriv->SdioXmitSema, 0);
	sema_init(&xmitpriv->SdioXmitTerminateSema, 0);
=======
	init_completion(&xmitpriv->SdioXmitStart);
	init_completion(&xmitpriv->SdioXmitTerminate);
>>>>>>> upstream/android-13

	return _SUCCESS;
}

void rtl8723bs_free_xmit_priv(struct adapter *padapter)
{
<<<<<<< HEAD
	struct hal_com_data *phal;
=======
>>>>>>> upstream/android-13
	struct xmit_priv *pxmitpriv;
	struct xmit_buf *pxmitbuf;
	struct __queue *pqueue;
	struct list_head *plist, *phead;
	struct list_head tmplist;


<<<<<<< HEAD
	phal = GET_HAL_DATA(padapter);
=======
>>>>>>> upstream/android-13
	pxmitpriv = &padapter->xmitpriv;
	pqueue = &pxmitpriv->pending_xmitbuf_queue;
	phead = get_list_head(pqueue);
	INIT_LIST_HEAD(&tmplist);

	spin_lock_bh(&pqueue->lock);
	if (!list_empty(&pqueue->queue)) {
		/*  Insert tmplist to end of queue, and delete phead */
		/*  then tmplist become head of queue. */
		list_add_tail(&tmplist, phead);
		list_del_init(phead);
	}
	spin_unlock_bh(&pqueue->lock);

	phead = &tmplist;
	while (list_empty(phead) == false) {
		plist = get_next(phead);
		list_del_init(plist);

<<<<<<< HEAD
		pxmitbuf = LIST_CONTAINOR(plist, struct xmit_buf, list);
=======
		pxmitbuf = container_of(plist, struct xmit_buf, list);
>>>>>>> upstream/android-13
		rtw_free_xmitframe(pxmitpriv, (struct xmit_frame *)pxmitbuf->priv_data);
		pxmitbuf->priv_data = NULL;
		rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
	}
}
