// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
<<<<<<< HEAD
#define _XMIT_OSDEP_C_

=======
>>>>>>> upstream/android-13
#include <drv_types.h>
#include <rtw_debug.h>


uint rtw_remainder_len(struct pkt_file *pfile)
{
	return (pfile->buf_len - ((SIZE_PTR)(pfile->cur_addr) - (SIZE_PTR)(pfile->buf_start)));
}

<<<<<<< HEAD
void _rtw_open_pktfile (_pkt *pktptr, struct pkt_file *pfile)
=======
void _rtw_open_pktfile(struct sk_buff *pktptr, struct pkt_file *pfile)
>>>>>>> upstream/android-13
{
	pfile->pkt = pktptr;
	pfile->cur_addr = pfile->buf_start = pktptr->data;
	pfile->pkt_len = pfile->buf_len = pktptr->len;

<<<<<<< HEAD
	pfile->cur_buffer = pfile->buf_start ;
}

uint _rtw_pktfile_read (struct pkt_file *pfile, u8 *rmem, uint rlen)
=======
	pfile->cur_buffer = pfile->buf_start;
}

uint _rtw_pktfile_read(struct pkt_file *pfile, u8 *rmem, uint rlen)
>>>>>>> upstream/android-13
{
	uint	len = 0;

	len =  rtw_remainder_len(pfile);
	len = (rlen > len) ? len : rlen;

	if (rmem)
<<<<<<< HEAD
		skb_copy_bits(pfile->pkt, pfile->buf_len-pfile->pkt_len, rmem, len);
=======
		skb_copy_bits(pfile->pkt, pfile->buf_len - pfile->pkt_len, rmem, len);
>>>>>>> upstream/android-13

	pfile->cur_addr += len;
	pfile->pkt_len -= len;
	return len;
}

<<<<<<< HEAD
sint rtw_endofpktfile(struct pkt_file *pfile)
=======
signed int rtw_endofpktfile(struct pkt_file *pfile)
>>>>>>> upstream/android-13
{
	if (pfile->pkt_len == 0)
		return true;
	return false;
}

int rtw_os_xmit_resource_alloc(struct adapter *padapter, struct xmit_buf *pxmitbuf, u32 alloc_sz, u8 flag)
{
	if (alloc_sz > 0) {
		pxmitbuf->pallocated_buf = rtw_zmalloc(alloc_sz);
<<<<<<< HEAD
		if (pxmitbuf->pallocated_buf == NULL)
=======
		if (!pxmitbuf->pallocated_buf)
>>>>>>> upstream/android-13
			return _FAIL;

		pxmitbuf->pbuf = (u8 *)N_BYTE_ALIGMENT((SIZE_PTR)(pxmitbuf->pallocated_buf), XMITBUF_ALIGN_SZ);
	}

	return _SUCCESS;
}

void rtw_os_xmit_resource_free(struct adapter *padapter, struct xmit_buf *pxmitbuf, u32 free_sz, u8 flag)
{
	if (free_sz > 0)
		kfree(pxmitbuf->pallocated_buf);
}

<<<<<<< HEAD
#define WMM_XMIT_THRESHOLD	(NR_XMITFRAME*2/5)

void rtw_os_pkt_complete(struct adapter *padapter, _pkt *pkt)
=======
#define WMM_XMIT_THRESHOLD	(NR_XMITFRAME * 2 / 5)

void rtw_os_pkt_complete(struct adapter *padapter, struct sk_buff *pkt)
>>>>>>> upstream/android-13
{
	u16 queue;
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;

	queue = skb_get_queue_mapping(pkt);
	if (padapter->registrypriv.wifi_spec) {
		if (__netif_subqueue_stopped(padapter->pnetdev, queue) &&
		    (pxmitpriv->hwxmits[queue].accnt < WMM_XMIT_THRESHOLD))
			netif_wake_subqueue(padapter->pnetdev, queue);
	} else {
		if (__netif_subqueue_stopped(padapter->pnetdev, queue))
			netif_wake_subqueue(padapter->pnetdev, queue);
	}

	dev_kfree_skb_any(pkt);
}

void rtw_os_xmit_complete(struct adapter *padapter, struct xmit_frame *pxframe)
{
	if (pxframe->pkt)
		rtw_os_pkt_complete(padapter, pxframe->pkt);

	pxframe->pkt = NULL;
}

void rtw_os_xmit_schedule(struct adapter *padapter)
{
	struct adapter *pri_adapter = padapter;

	if (!padapter)
		return;

	if (!list_empty(&padapter->xmitpriv.pending_xmitbuf_queue.queue))
<<<<<<< HEAD
		up(&pri_adapter->xmitpriv.xmit_sema);
}

static void rtw_check_xmit_resource(struct adapter *padapter, _pkt *pkt)
=======
		complete(&pri_adapter->xmitpriv.xmit_comp);
}

static void rtw_check_xmit_resource(struct adapter *padapter, struct sk_buff *pkt)
>>>>>>> upstream/android-13
{
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	u16 queue;

	queue = skb_get_queue_mapping(pkt);
	if (padapter->registrypriv.wifi_spec) {
		/* No free space for Tx, tx_worker is too slow */
<<<<<<< HEAD
		if (pxmitpriv->hwxmits[queue].accnt > WMM_XMIT_THRESHOLD) {
			/* DBG_871X("%s(): stop netif_subqueue[%d]\n", __func__, queue); */
			netif_stop_subqueue(padapter->pnetdev, queue);
		}
=======
		if (pxmitpriv->hwxmits[queue].accnt > WMM_XMIT_THRESHOLD)
			netif_stop_subqueue(padapter->pnetdev, queue);
>>>>>>> upstream/android-13
	} else {
		if (pxmitpriv->free_xmitframe_cnt <= 4) {
			if (!netif_tx_queue_stopped(netdev_get_tx_queue(padapter->pnetdev, queue)))
				netif_stop_subqueue(padapter->pnetdev, queue);
		}
	}
}

static int rtw_mlcst2unicst(struct adapter *padapter, struct sk_buff *skb)
{
	struct	sta_priv *pstapriv = &padapter->stapriv;
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	struct list_head	*phead, *plist;
	struct sk_buff *newskb;
	struct sta_info *psta = NULL;
	u8 chk_alive_num = 0;
	char chk_alive_list[NUM_STA];
	u8 bc_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	u8 null_addr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	int i;
	s32	res;

<<<<<<< HEAD
	DBG_COUNTER(padapter->tx_logs.os_tx_m2u);

	spin_lock_bh(&pstapriv->asoc_list_lock);
	phead = &pstapriv->asoc_list;
	plist = get_next(phead);

	/* free sta asoc_queue */
	while (phead != plist) {
		int stainfo_offset;
		psta = LIST_CONTAINOR(plist, struct sta_info, asoc_list);
		plist = get_next(plist);
=======
	spin_lock_bh(&pstapriv->asoc_list_lock);
	phead = &pstapriv->asoc_list;
	/* free sta asoc_queue */
	list_for_each(plist, phead) {
		int stainfo_offset;

		psta = list_entry(plist, struct sta_info, asoc_list);
>>>>>>> upstream/android-13

		stainfo_offset = rtw_stainfo_offset(pstapriv, psta);
		if (stainfo_offset_valid(stainfo_offset)) {
			chk_alive_list[chk_alive_num++] = stainfo_offset;
		}
	}
	spin_unlock_bh(&pstapriv->asoc_list_lock);

	for (i = 0; i < chk_alive_num; i++) {
		psta = rtw_get_stainfo_by_offset(pstapriv, chk_alive_list[i]);
<<<<<<< HEAD
		if (!(psta->state & _FW_LINKED)) {
			DBG_COUNTER(padapter->tx_logs.os_tx_m2u_ignore_fw_linked);
			continue;
		}
=======
		if (!(psta->state & _FW_LINKED))
			continue;
>>>>>>> upstream/android-13

		/* avoid come from STA1 and send back STA1 */
		if (!memcmp(psta->hwaddr, &skb->data[6], 6) ||
		    !memcmp(psta->hwaddr, null_addr, 6) ||
<<<<<<< HEAD
		    !memcmp(psta->hwaddr, bc_addr, 6)) {
			DBG_COUNTER(padapter->tx_logs.os_tx_m2u_ignore_self);
			continue;
		}

		DBG_COUNTER(padapter->tx_logs.os_tx_m2u_entry);
=======
		    !memcmp(psta->hwaddr, bc_addr, 6))
			continue;
>>>>>>> upstream/android-13

		newskb = rtw_skb_copy(skb);

		if (newskb) {
			memcpy(newskb->data, psta->hwaddr, 6);
			res = rtw_xmit(padapter, &newskb);
			if (res < 0) {
<<<<<<< HEAD
				DBG_COUNTER(padapter->tx_logs.os_tx_m2u_entry_err_xmit);
				DBG_871X("%s()-%d: rtw_xmit() return error!\n", __func__, __LINE__);
=======
>>>>>>> upstream/android-13
				pxmitpriv->tx_drop++;
				dev_kfree_skb_any(newskb);
			}
		} else {
<<<<<<< HEAD
			DBG_COUNTER(padapter->tx_logs.os_tx_m2u_entry_err_skb);
			DBG_871X("%s-%d: rtw_skb_copy() failed!\n", __func__, __LINE__);
=======
>>>>>>> upstream/android-13
			pxmitpriv->tx_drop++;
			/* dev_kfree_skb_any(skb); */
			return false;	/*  Caller shall tx this multicast frame via normal way. */
		}
	}

	dev_kfree_skb_any(skb);
	return true;
}

<<<<<<< HEAD
int _rtw_xmit_entry(_pkt *pkt, _nic_hdl pnetdev)
{
	struct adapter *padapter = (struct adapter *)rtw_netdev_priv(pnetdev);
=======
int _rtw_xmit_entry(struct sk_buff *pkt, struct net_device *pnetdev)
{
	struct adapter *padapter = rtw_netdev_priv(pnetdev);
>>>>>>> upstream/android-13
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;
	s32 res = 0;

<<<<<<< HEAD
	DBG_COUNTER(padapter->tx_logs.os_tx);
	RT_TRACE(_module_rtl871x_mlme_c_, _drv_info_, ("+xmit_enry\n"));

	if (rtw_if_up(padapter) == false) {
		DBG_COUNTER(padapter->tx_logs.os_tx_err_up);
		RT_TRACE(_module_xmit_osdep_c_, _drv_err_, ("rtw_xmit_entry: rtw_if_up fail\n"));
		#ifdef DBG_TX_DROP_FRAME
		DBG_871X("DBG_TX_DROP_FRAME %s if_up fail\n", __func__);
		#endif
		goto drop_packet;
	}
=======
	if (rtw_if_up(padapter) == false)
		goto drop_packet;
>>>>>>> upstream/android-13

	rtw_check_xmit_resource(padapter, pkt);

	if (!rtw_mc2u_disable
		&& check_fwstate(pmlmepriv, WIFI_AP_STATE) == true
		&& (IP_MCAST_MAC(pkt->data)
			|| ICMPV6_MCAST_MAC(pkt->data)
<<<<<<< HEAD
			#ifdef CONFIG_TX_BCAST2UNI
			|| is_broadcast_mac_addr(pkt->data)
			#endif
			)
		&& padapter->registrypriv.wifi_spec == 0) {
		if (pxmitpriv->free_xmitframe_cnt > (NR_XMITFRAME/4)) {
			res = rtw_mlcst2unicst(padapter, pkt);
			if (res == true)
				goto exit;
		} else {
			/* DBG_871X("Stop M2U(%d, %d)! ", pxmitpriv->free_xmitframe_cnt, pxmitpriv->free_xmitbuf_cnt); */
			/* DBG_871X("!m2u); */
			DBG_COUNTER(padapter->tx_logs.os_tx_m2u_stop);
=======
			)
		&& padapter->registrypriv.wifi_spec == 0) {
		if (pxmitpriv->free_xmitframe_cnt > (NR_XMITFRAME / 4)) {
			res = rtw_mlcst2unicst(padapter, pkt);
			if (res)
				goto exit;
>>>>>>> upstream/android-13
		}
	}

	res = rtw_xmit(padapter, &pkt);
<<<<<<< HEAD
	if (res < 0) {
		#ifdef DBG_TX_DROP_FRAME
		DBG_871X("DBG_TX_DROP_FRAME %s rtw_xmit fail\n", __func__);
		#endif
		goto drop_packet;
	}

	RT_TRACE(_module_xmit_osdep_c_, _drv_info_, ("rtw_xmit_entry: tx_pkts =%d\n", (u32)pxmitpriv->tx_pkts));
=======
	if (res < 0)
		goto drop_packet;

>>>>>>> upstream/android-13
	goto exit;

drop_packet:
	pxmitpriv->tx_drop++;
	dev_kfree_skb_any(pkt);
<<<<<<< HEAD
	RT_TRACE(_module_xmit_osdep_c_, _drv_notice_, ("rtw_xmit_entry: drop, tx_drop =%d\n", (u32)pxmitpriv->tx_drop));
=======
>>>>>>> upstream/android-13

exit:
	return 0;
}

<<<<<<< HEAD
int rtw_xmit_entry(_pkt *pkt, _nic_hdl pnetdev)
{
	int ret = 0;

	if (pkt) {
		rtw_mstat_update(MSTAT_TYPE_SKB, MSTAT_ALLOC_SUCCESS, pkt->truesize);
		ret = _rtw_xmit_entry(pkt, pnetdev);
	}
=======
int rtw_xmit_entry(struct sk_buff *pkt, struct net_device *pnetdev)
{
	int ret = 0;

	if (pkt)
		ret = _rtw_xmit_entry(pkt, pnetdev);
>>>>>>> upstream/android-13

	return ret;
}
