/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_H323_H
#define _NF_CONNTRACK_H323_H

#ifdef __KERNEL__

#include <linux/netfilter/nf_conntrack_h323_asn1.h>

#define RAS_PORT 1719
#define Q931_PORT 1720
#define H323_RTP_CHANNEL_MAX 4	


struct nf_ct_h323_master {

	
	__be16 sig_port[IP_CT_DIR_MAX];

	
	__be16 rtp_port[H323_RTP_CHANNEL_MAX][IP_CT_DIR_MAX];

	union {
		
		u_int32_t timeout;

		
		u_int16_t tpkt_len[IP_CT_DIR_MAX];
	};
};

struct nf_conn;

int get_h225_addr(struct nf_conn *ct, unsigned char *data,
		  TransportAddress *taddr, union nf_inet_addr *addr,
		  __be16 *port);
void nf_conntrack_h245_expect(struct nf_conn *new,
			      struct nf_conntrack_expect *this);
void nf_conntrack_q931_expect(struct nf_conn *new,
			      struct nf_conntrack_expect *this);
extern int (*set_h245_addr_hook) (struct sk_buff *skb, unsigned int protoff,
				  unsigned char **data, int dataoff,
				  H245_TransportAddress *taddr,
				  union nf_inet_addr *addr,
				  __be16 port);
extern int (*set_h225_addr_hook) (struct sk_buff *skb, unsigned int protoff,
				  unsigned char **data, int dataoff,
				  TransportAddress *taddr,
				  union nf_inet_addr *addr,
				  __be16 port);
extern int (*set_sig_addr_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned int protoff, unsigned char **data,
				 TransportAddress *taddr, int count);
extern int (*set_ras_addr_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned int protoff, unsigned char **data,
				 TransportAddress *taddr, int count);
extern int (*nat_rtp_rtcp_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned int protoff, unsigned char **data,
				 int dataoff,
				 H245_TransportAddress *taddr,
				 __be16 port, __be16 rtp_port,
				 struct nf_conntrack_expect *rtp_exp,
				 struct nf_conntrack_expect *rtcp_exp);
extern int (*nat_t120_hook) (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned int protoff,
			     unsigned char **data, int dataoff,
			     H245_TransportAddress *taddr, __be16 port,
			     struct nf_conntrack_expect *exp);
extern int (*nat_h245_hook) (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned int protoff,
			     unsigned char **data, int dataoff,
			     TransportAddress *taddr, __be16 port,
			     struct nf_conntrack_expect *exp);
extern int (*nat_callforwarding_hook) (struct sk_buff *skb,
				       struct nf_conn *ct,
				       enum ip_conntrack_info ctinfo,
				       unsigned int protoff,
				       unsigned char **data, int dataoff,
				       TransportAddress *taddr,
				       __be16 port,
				       struct nf_conntrack_expect *exp);
extern int (*nat_q931_hook) (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned int protoff,
			     unsigned char **data, TransportAddress *taddr,
			     int idx, __be16 port,
			     struct nf_conntrack_expect *exp);

#endif

#endif
