/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __RECV_OSDEP_H_
#define __RECV_OSDEP_H_


<<<<<<< HEAD
extern sint _rtw_init_recv_priv(struct recv_priv *precvpriv, struct adapter *padapter);
extern void _rtw_free_recv_priv (struct recv_priv *precvpriv);
=======
extern signed int _rtw_init_recv_priv(struct recv_priv *precvpriv, struct adapter *padapter);
extern void _rtw_free_recv_priv(struct recv_priv *precvpriv);
>>>>>>> upstream/android-13


extern s32  rtw_recv_entry(union recv_frame *precv_frame);
extern int rtw_recv_indicatepkt(struct adapter *adapter, union recv_frame *precv_frame);
<<<<<<< HEAD
extern void rtw_recv_returnpacket(_nic_hdl cnxt, _pkt *preturnedpkt);
=======
extern void rtw_recv_returnpacket(struct net_device *cnxt, struct sk_buff *preturnedpkt);
>>>>>>> upstream/android-13

extern void rtw_handle_tkip_mic_err(struct adapter *padapter, u8 bgroup);

int	rtw_init_recv_priv(struct recv_priv *precvpriv, struct adapter *padapter);
<<<<<<< HEAD
void rtw_free_recv_priv (struct recv_priv *precvpriv);


int rtw_os_recv_resource_alloc(struct adapter *padapter, union recv_frame *precvframe);
=======
void rtw_free_recv_priv(struct recv_priv *precvpriv);


void rtw_os_recv_resource_alloc(struct adapter *padapter, union recv_frame *precvframe);
>>>>>>> upstream/android-13
void rtw_os_recv_resource_free(struct recv_priv *precvpriv);


void rtw_os_free_recvframe(union recv_frame *precvframe);


<<<<<<< HEAD
int rtw_os_recvbuf_resource_free(struct adapter *padapter, struct recv_buf *precvbuf);

_pkt *rtw_os_alloc_msdu_pkt(union recv_frame *prframe, u16 nSubframe_Length, u8 *pdata);
void rtw_os_recv_indicate_pkt(struct adapter *padapter, _pkt *pkt, struct rx_pkt_attrib *pattrib);
=======
void rtw_os_recvbuf_resource_free(struct adapter *padapter, struct recv_buf *precvbuf);

struct sk_buff *rtw_os_alloc_msdu_pkt(union recv_frame *prframe, u16 nSubframe_Length, u8 *pdata);
void rtw_os_recv_indicate_pkt(struct adapter *padapter, struct sk_buff *pkt, struct rx_pkt_attrib *pattrib);
>>>>>>> upstream/android-13

void rtw_init_recv_timer(struct recv_reorder_ctrl *preorder_ctrl);


#endif /*  */
