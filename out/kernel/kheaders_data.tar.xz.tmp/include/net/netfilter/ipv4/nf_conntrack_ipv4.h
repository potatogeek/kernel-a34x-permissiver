/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _NF_CONNTRACK_IPV4_H
#define _NF_CONNTRACK_IPV4_H

extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp4;
extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4;
extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_icmp;
#ifdef CONFIG_NF_CT_PROTO_DCCP
extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_dccp4;
#endif
#ifdef CONFIG_NF_CT_PROTO_SCTP
extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_sctp4;
#endif
#ifdef CONFIG_NF_CT_PROTO_UDPLITE
extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_udplite4;
#endif

int nf_conntrack_ipv4_compat_init(void);
void nf_conntrack_ipv4_compat_fini(void);

#endif 
