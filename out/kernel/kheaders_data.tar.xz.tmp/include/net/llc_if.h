#ifndef LLC_IF_H
#define LLC_IF_H



#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/llc.h>
#include <linux/etherdevice.h>
#include <net/llc.h>

#define LLC_DATAUNIT_PRIM	1
#define LLC_CONN_PRIM		2
#define LLC_DATA_PRIM		3
#define LLC_DISC_PRIM		4
#define LLC_RESET_PRIM		5
#define LLC_FLOWCONTROL_PRIM	6 
#define LLC_DISABLE_PRIM	7
#define LLC_XID_PRIM		8
#define LLC_TEST_PRIM		9
#define LLC_SAP_ACTIVATION     10
#define LLC_SAP_DEACTIVATION   11

#define LLC_NBR_PRIMITIVES     11

#define LLC_IND			1
#define LLC_CONFIRM		2


#define LLC_PRIM_TYPE_REQ	1
#define LLC_PRIM_TYPE_IND	2
#define LLC_PRIM_TYPE_RESP	3
#define LLC_PRIM_TYPE_CONFIRM	4


#define LLC_RESET_REASON_REMOTE	1
#define LLC_RESET_REASON_LOCAL	2


#define LLC_DISC_REASON_RX_DM_RSP_PDU	0
#define LLC_DISC_REASON_RX_DISC_CMD_PDU	1
#define LLC_DISC_REASON_ACK_TMR_EXP	2


#define LLC_STATUS_CONN		0 
#define LLC_STATUS_DISC		1 
#define LLC_STATUS_FAILED	2 
#define LLC_STATUS_IMPOSSIBLE	3 
#define LLC_STATUS_RECEIVED	4 
#define LLC_STATUS_REMOTE_BUSY	5 
#define LLC_STATUS_REFUSE	6 
#define LLC_STATUS_CONFLICT	7 
#define LLC_STATUS_RESET_DONE	8 

int llc_establish_connection(struct sock *sk, u8 *lmac, u8 *dmac, u8 dsap);
int llc_build_and_send_pkt(struct sock *sk, struct sk_buff *skb);
int llc_send_disc(struct sock *sk);
#endif 
