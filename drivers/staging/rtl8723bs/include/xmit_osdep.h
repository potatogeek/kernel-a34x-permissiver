/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __XMIT_OSDEP_H_
#define __XMIT_OSDEP_H_


struct pkt_file {
<<<<<<< HEAD
	_pkt *pkt;
	__kernel_size_t pkt_len;	 /* the remainder length of the open_file */
	_buffer *cur_buffer;
=======
	struct sk_buff *pkt;
	__kernel_size_t pkt_len;	 /* the remainder length of the open_file */
	unsigned char *cur_buffer;
>>>>>>> upstream/android-13
	u8 *buf_start;
	u8 *cur_addr;
	__kernel_size_t buf_len;
};

#define NR_XMITFRAME	256

struct xmit_priv;
struct pkt_attrib;
struct sta_xmit_priv;
struct xmit_frame;
struct xmit_buf;

<<<<<<< HEAD
extern int _rtw_xmit_entry(_pkt *pkt, _nic_hdl pnetdev);
extern int rtw_xmit_entry(_pkt *pkt, _nic_hdl pnetdev);
=======
extern int _rtw_xmit_entry(struct sk_buff *pkt, struct net_device *pnetdev);
extern int rtw_xmit_entry(struct sk_buff *pkt, struct net_device *pnetdev);
>>>>>>> upstream/android-13

void rtw_os_xmit_schedule(struct adapter *padapter);

int rtw_os_xmit_resource_alloc(struct adapter *padapter, struct xmit_buf *pxmitbuf, u32 alloc_sz, u8 flag);
void rtw_os_xmit_resource_free(struct adapter *padapter, struct xmit_buf *pxmitbuf, u32 free_sz, u8 flag);

extern uint rtw_remainder_len(struct pkt_file *pfile);
<<<<<<< HEAD
extern void _rtw_open_pktfile(_pkt *pkt, struct pkt_file *pfile);
extern uint _rtw_pktfile_read (struct pkt_file *pfile, u8 *rmem, uint rlen);
extern sint rtw_endofpktfile (struct pkt_file *pfile);

extern void rtw_os_pkt_complete(struct adapter *padapter, _pkt *pkt);
=======
extern void _rtw_open_pktfile(struct sk_buff *pkt, struct pkt_file *pfile);
extern uint _rtw_pktfile_read(struct pkt_file *pfile, u8 *rmem, uint rlen);
extern signed int rtw_endofpktfile(struct pkt_file *pfile);

extern void rtw_os_pkt_complete(struct adapter *padapter, struct sk_buff *pkt);
>>>>>>> upstream/android-13
extern void rtw_os_xmit_complete(struct adapter *padapter, struct xmit_frame *pxframe);

#endif /* __XMIT_OSDEP_H_ */
