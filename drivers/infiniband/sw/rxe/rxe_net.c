<<<<<<< HEAD
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
=======
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <net/udp_tunnel.h>
#include <net/sch_generic.h>
#include <linux/netfilter.h>
#include <rdma/ib_addr.h>

#include "rxe.h"
#include "rxe_net.h"
#include "rxe_loc.h"

<<<<<<< HEAD
static LIST_HEAD(rxe_dev_list);
static DEFINE_SPINLOCK(dev_list_lock); /* spinlock for device list */

struct rxe_dev *net_to_rxe(struct net_device *ndev)
{
	struct rxe_dev *rxe;
	struct rxe_dev *found = NULL;

	spin_lock_bh(&dev_list_lock);
	list_for_each_entry(rxe, &rxe_dev_list, list) {
		if (rxe->ndev == ndev) {
			found = rxe;
			break;
		}
	}
	spin_unlock_bh(&dev_list_lock);

	return found;
}

struct rxe_dev *get_rxe_by_name(const char *name)
{
	struct rxe_dev *rxe;
	struct rxe_dev *found = NULL;

	spin_lock_bh(&dev_list_lock);
	list_for_each_entry(rxe, &rxe_dev_list, list) {
		if (!strcmp(name, rxe->ib_dev.name)) {
			found = rxe;
			break;
		}
	}
	spin_unlock_bh(&dev_list_lock);
	return found;
}


static struct rxe_recv_sockets recv_sockets;

struct device *rxe_dma_device(struct rxe_dev *rxe)
{
	struct net_device *ndev;

	ndev = rxe->ndev;

	if (is_vlan_dev(ndev))
		ndev = vlan_dev_real_dev(ndev);

	return ndev->dev.parent;
}

=======
static struct rxe_recv_sockets recv_sockets;

>>>>>>> upstream/android-13
int rxe_mcast_add(struct rxe_dev *rxe, union ib_gid *mgid)
{
	int err;
	unsigned char ll_addr[ETH_ALEN];

	ipv6_eth_mc_map((struct in6_addr *)mgid->raw, ll_addr);
	err = dev_mc_add(rxe->ndev, ll_addr);

	return err;
}

int rxe_mcast_delete(struct rxe_dev *rxe, union ib_gid *mgid)
{
	int err;
	unsigned char ll_addr[ETH_ALEN];

	ipv6_eth_mc_map((struct in6_addr *)mgid->raw, ll_addr);
	err = dev_mc_del(rxe->ndev, ll_addr);

	return err;
}

static struct dst_entry *rxe_find_route4(struct net_device *ndev,
				  struct in_addr *saddr,
				  struct in_addr *daddr)
{
	struct rtable *rt;
	struct flowi4 fl = { { 0 } };

	memset(&fl, 0, sizeof(fl));
	fl.flowi4_oif = ndev->ifindex;
	memcpy(&fl.saddr, saddr, sizeof(*saddr));
	memcpy(&fl.daddr, daddr, sizeof(*daddr));
	fl.flowi4_proto = IPPROTO_UDP;

	rt = ip_route_output_key(&init_net, &fl);
	if (IS_ERR(rt)) {
		pr_err_ratelimited("no route to %pI4\n", &daddr->s_addr);
		return NULL;
	}

	return &rt->dst;
}

#if IS_ENABLED(CONFIG_IPV6)
static struct dst_entry *rxe_find_route6(struct net_device *ndev,
					 struct in6_addr *saddr,
					 struct in6_addr *daddr)
{
	struct dst_entry *ndst;
	struct flowi6 fl6 = { { 0 } };

	memset(&fl6, 0, sizeof(fl6));
	fl6.flowi6_oif = ndev->ifindex;
	memcpy(&fl6.saddr, saddr, sizeof(*saddr));
	memcpy(&fl6.daddr, daddr, sizeof(*daddr));
	fl6.flowi6_proto = IPPROTO_UDP;

	ndst = ipv6_stub->ipv6_dst_lookup_flow(sock_net(recv_sockets.sk6->sk),
					       recv_sockets.sk6->sk, &fl6,
					       NULL);
<<<<<<< HEAD
	if (unlikely(IS_ERR(ndst))) {
=======
	if (IS_ERR(ndst)) {
>>>>>>> upstream/android-13
		pr_err_ratelimited("no route to %pI6\n", daddr);
		return NULL;
	}

	if (unlikely(ndst->error)) {
		pr_err("no route to %pI6\n", daddr);
		goto put;
	}

	return ndst;
put:
	dst_release(ndst);
	return NULL;
}

#else

static struct dst_entry *rxe_find_route6(struct net_device *ndev,
					 struct in6_addr *saddr,
					 struct in6_addr *daddr)
{
	return NULL;
}

#endif

<<<<<<< HEAD
static struct dst_entry *rxe_find_route(struct rxe_dev *rxe,
					struct rxe_qp *qp,
					struct rxe_av *av)
{
	const struct ib_gid_attr *attr;
	struct dst_entry *dst = NULL;
	struct net_device *ndev;

	attr = rdma_get_gid_attr(&rxe->ib_dev, qp->attr.port_num,
				 av->grh.sgid_index);
	if (IS_ERR(attr))
		return NULL;
	ndev = attr->ndev;
=======
static struct dst_entry *rxe_find_route(struct net_device *ndev,
					struct rxe_qp *qp,
					struct rxe_av *av)
{
	struct dst_entry *dst = NULL;
>>>>>>> upstream/android-13

	if (qp_type(qp) == IB_QPT_RC)
		dst = sk_dst_get(qp->sk->sk);

	if (!dst || !dst_check(dst, qp->dst_cookie)) {
		if (dst)
			dst_release(dst);

<<<<<<< HEAD
		if (av->network_type == RDMA_NETWORK_IPV4) {
=======
		if (av->network_type == RXE_NETWORK_TYPE_IPV4) {
>>>>>>> upstream/android-13
			struct in_addr *saddr;
			struct in_addr *daddr;

			saddr = &av->sgid_addr._sockaddr_in.sin_addr;
			daddr = &av->dgid_addr._sockaddr_in.sin_addr;
			dst = rxe_find_route4(ndev, saddr, daddr);
<<<<<<< HEAD
		} else if (av->network_type == RDMA_NETWORK_IPV6) {
=======
		} else if (av->network_type == RXE_NETWORK_TYPE_IPV6) {
>>>>>>> upstream/android-13
			struct in6_addr *saddr6;
			struct in6_addr *daddr6;

			saddr6 = &av->sgid_addr._sockaddr_in6.sin6_addr;
			daddr6 = &av->dgid_addr._sockaddr_in6.sin6_addr;
			dst = rxe_find_route6(ndev, saddr6, daddr6);
#if IS_ENABLED(CONFIG_IPV6)
			if (dst)
				qp->dst_cookie =
					rt6_get_cookie((struct rt6_info *)dst);
#endif
		}

		if (dst && (qp_type(qp) == IB_QPT_RC)) {
			dst_hold(dst);
			sk_dst_set(qp->sk->sk, dst);
		}
	}
<<<<<<< HEAD
	rdma_put_gid_attr(attr);
=======
>>>>>>> upstream/android-13
	return dst;
}

static int rxe_udp_encap_recv(struct sock *sk, struct sk_buff *skb)
{
	struct udphdr *udph;
<<<<<<< HEAD
	struct net_device *ndev = skb->dev;
	struct net_device *rdev = ndev;
	struct rxe_dev *rxe = net_to_rxe(ndev);
	struct rxe_pkt_info *pkt = SKB_TO_PKT(skb);

	if (!rxe && is_vlan_dev(rdev)) {
		rdev = vlan_dev_real_dev(ndev);
		rxe = net_to_rxe(rdev);
	}
=======
	struct rxe_dev *rxe;
	struct net_device *ndev = skb->dev;
	struct rxe_pkt_info *pkt = SKB_TO_PKT(skb);

	/* takes a reference on rxe->ib_dev
	 * drop when skb is freed
	 */
	rxe = rxe_get_dev_from_net(ndev);
	if (!rxe && is_vlan_dev(ndev))
		rxe = rxe_get_dev_from_net(vlan_dev_real_dev(ndev));
>>>>>>> upstream/android-13
	if (!rxe)
		goto drop;

	if (skb_linearize(skb)) {
		pr_err("skb_linearize failed\n");
<<<<<<< HEAD
=======
		ib_device_put(&rxe->ib_dev);
>>>>>>> upstream/android-13
		goto drop;
	}

	udph = udp_hdr(skb);
	pkt->rxe = rxe;
	pkt->port_num = 1;
	pkt->hdr = (u8 *)(udph + 1);
	pkt->mask = RXE_GRH_MASK;
	pkt->paylen = be16_to_cpu(udph->len) - sizeof(*udph);

	rxe_rcv(skb);

	return 0;
drop:
	kfree_skb(skb);

	return 0;
}

static struct socket *rxe_setup_udp_tunnel(struct net *net, __be16 port,
					   bool ipv6)
{
	int err;
	struct socket *sock;
	struct udp_port_cfg udp_cfg = { };
	struct udp_tunnel_sock_cfg tnl_cfg = { };

	if (ipv6) {
		udp_cfg.family = AF_INET6;
		udp_cfg.ipv6_v6only = 1;
	} else {
		udp_cfg.family = AF_INET;
	}

	udp_cfg.local_udp_port = port;

	/* Create UDP socket */
	err = udp_sock_create(net, &udp_cfg, &sock);
<<<<<<< HEAD
	if (err < 0) {
		pr_err("failed to create udp socket. err = %d\n", err);
		return ERR_PTR(err);
	}
=======
	if (err < 0)
		return ERR_PTR(err);
>>>>>>> upstream/android-13

	tnl_cfg.encap_type = 1;
	tnl_cfg.encap_rcv = rxe_udp_encap_recv;

	/* Setup UDP tunnel */
	setup_udp_tunnel_sock(net, sock, &tnl_cfg);

	return sock;
}

static void rxe_release_udp_tunnel(struct socket *sk)
{
	if (sk)
		udp_tunnel_sock_release(sk);
}

static void prepare_udp_hdr(struct sk_buff *skb, __be16 src_port,
			    __be16 dst_port)
{
	struct udphdr *udph;

	__skb_push(skb, sizeof(*udph));
	skb_reset_transport_header(skb);
	udph = udp_hdr(skb);

	udph->dest = dst_port;
	udph->source = src_port;
	udph->len = htons(skb->len);
	udph->check = 0;
}

static void prepare_ipv4_hdr(struct dst_entry *dst, struct sk_buff *skb,
			     __be32 saddr, __be32 daddr, __u8 proto,
			     __u8 tos, __u8 ttl, __be16 df, bool xnet)
{
	struct iphdr *iph;

	skb_scrub_packet(skb, xnet);

	skb_clear_hash(skb);
	skb_dst_set(skb, dst_clone(dst));
	memset(IPCB(skb), 0, sizeof(*IPCB(skb)));

	skb_push(skb, sizeof(struct iphdr));
	skb_reset_network_header(skb);

	iph = ip_hdr(skb);

	iph->version	=	IPVERSION;
	iph->ihl	=	sizeof(struct iphdr) >> 2;
<<<<<<< HEAD
=======
	iph->tot_len	=	htons(skb->len);
>>>>>>> upstream/android-13
	iph->frag_off	=	df;
	iph->protocol	=	proto;
	iph->tos	=	tos;
	iph->daddr	=	daddr;
	iph->saddr	=	saddr;
	iph->ttl	=	ttl;
	__ip_select_ident(dev_net(dst->dev), iph,
			  skb_shinfo(skb)->gso_segs ?: 1);
<<<<<<< HEAD
	iph->tot_len = htons(skb->len);
	ip_send_check(iph);
=======
>>>>>>> upstream/android-13
}

static void prepare_ipv6_hdr(struct dst_entry *dst, struct sk_buff *skb,
			     struct in6_addr *saddr, struct in6_addr *daddr,
			     __u8 proto, __u8 prio, __u8 ttl)
{
	struct ipv6hdr *ip6h;

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED
			    | IPSKB_REROUTED);
	skb_dst_set(skb, dst_clone(dst));

	__skb_push(skb, sizeof(*ip6h));
	skb_reset_network_header(skb);
	ip6h		  = ipv6_hdr(skb);
	ip6_flow_hdr(ip6h, prio, htonl(0));
	ip6h->payload_len = htons(skb->len);
	ip6h->nexthdr     = proto;
	ip6h->hop_limit   = ttl;
	ip6h->daddr	  = *daddr;
	ip6h->saddr	  = *saddr;
	ip6h->payload_len = htons(skb->len - sizeof(*ip6h));
}

<<<<<<< HEAD
static int prepare4(struct rxe_dev *rxe, struct rxe_pkt_info *pkt,
		    struct sk_buff *skb, struct rxe_av *av)
=======
static int prepare4(struct rxe_pkt_info *pkt, struct sk_buff *skb)
>>>>>>> upstream/android-13
{
	struct rxe_qp *qp = pkt->qp;
	struct dst_entry *dst;
	bool xnet = false;
	__be16 df = htons(IP_DF);
<<<<<<< HEAD
	struct in_addr *saddr = &av->sgid_addr._sockaddr_in.sin_addr;
	struct in_addr *daddr = &av->dgid_addr._sockaddr_in.sin_addr;

	dst = rxe_find_route(rxe, qp, av);
=======
	struct rxe_av *av = rxe_get_av(pkt);
	struct in_addr *saddr = &av->sgid_addr._sockaddr_in.sin_addr;
	struct in_addr *daddr = &av->dgid_addr._sockaddr_in.sin_addr;

	dst = rxe_find_route(skb->dev, qp, av);
>>>>>>> upstream/android-13
	if (!dst) {
		pr_err("Host not reachable\n");
		return -EHOSTUNREACH;
	}

<<<<<<< HEAD
	if (!memcmp(saddr, daddr, sizeof(*daddr)))
		pkt->mask |= RXE_LOOPBACK_MASK;

	prepare_udp_hdr(skb, htons(RXE_ROCE_V2_SPORT),
			htons(ROCE_V2_UDP_DPORT));
=======
	prepare_udp_hdr(skb, cpu_to_be16(qp->src_port),
			cpu_to_be16(ROCE_V2_UDP_DPORT));
>>>>>>> upstream/android-13

	prepare_ipv4_hdr(dst, skb, saddr->s_addr, daddr->s_addr, IPPROTO_UDP,
			 av->grh.traffic_class, av->grh.hop_limit, df, xnet);

	dst_release(dst);
	return 0;
}

<<<<<<< HEAD
static int prepare6(struct rxe_dev *rxe, struct rxe_pkt_info *pkt,
		    struct sk_buff *skb, struct rxe_av *av)
{
	struct rxe_qp *qp = pkt->qp;
	struct dst_entry *dst;
	struct in6_addr *saddr = &av->sgid_addr._sockaddr_in6.sin6_addr;
	struct in6_addr *daddr = &av->dgid_addr._sockaddr_in6.sin6_addr;

	dst = rxe_find_route(rxe, qp, av);
=======
static int prepare6(struct rxe_pkt_info *pkt, struct sk_buff *skb)
{
	struct rxe_qp *qp = pkt->qp;
	struct dst_entry *dst;
	struct rxe_av *av = rxe_get_av(pkt);
	struct in6_addr *saddr = &av->sgid_addr._sockaddr_in6.sin6_addr;
	struct in6_addr *daddr = &av->dgid_addr._sockaddr_in6.sin6_addr;

	dst = rxe_find_route(skb->dev, qp, av);
>>>>>>> upstream/android-13
	if (!dst) {
		pr_err("Host not reachable\n");
		return -EHOSTUNREACH;
	}

<<<<<<< HEAD
	if (!memcmp(saddr, daddr, sizeof(*daddr)))
		pkt->mask |= RXE_LOOPBACK_MASK;

	prepare_udp_hdr(skb, htons(RXE_ROCE_V2_SPORT),
			htons(ROCE_V2_UDP_DPORT));
=======
	prepare_udp_hdr(skb, cpu_to_be16(qp->src_port),
			cpu_to_be16(ROCE_V2_UDP_DPORT));
>>>>>>> upstream/android-13

	prepare_ipv6_hdr(dst, skb, saddr, daddr, IPPROTO_UDP,
			 av->grh.traffic_class,
			 av->grh.hop_limit);

	dst_release(dst);
	return 0;
}

<<<<<<< HEAD
int rxe_prepare(struct rxe_dev *rxe, struct rxe_pkt_info *pkt,
		struct sk_buff *skb, u32 *crc)
{
	int err = 0;
	struct rxe_av *av = rxe_get_av(pkt);

	if (av->network_type == RDMA_NETWORK_IPV4)
		err = prepare4(rxe, pkt, skb, av);
	else if (av->network_type == RDMA_NETWORK_IPV6)
		err = prepare6(rxe, pkt, skb, av);

	*crc = rxe_icrc_hdr(pkt, skb);
=======
int rxe_prepare(struct rxe_pkt_info *pkt, struct sk_buff *skb)
{
	int err = 0;

	if (skb->protocol == htons(ETH_P_IP))
		err = prepare4(pkt, skb);
	else if (skb->protocol == htons(ETH_P_IPV6))
		err = prepare6(pkt, skb);

	if (ether_addr_equal(skb->dev->dev_addr, rxe_get_av(pkt)->dmac))
		pkt->mask |= RXE_LOOPBACK_MASK;
>>>>>>> upstream/android-13

	return err;
}

static void rxe_skb_tx_dtor(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;
	struct rxe_qp *qp = sk->sk_user_data;
	int skb_out = atomic_dec_return(&qp->skb_out);

	if (unlikely(qp->need_req_skb &&
		     skb_out < RXE_INFLIGHT_SKBS_PER_QP_LOW))
		rxe_run_task(&qp->req.task, 1);

	rxe_drop_ref(qp);
}

<<<<<<< HEAD
int rxe_send(struct rxe_pkt_info *pkt, struct sk_buff *skb)
{
	struct rxe_av *av;
	int err;

	av = rxe_get_av(pkt);

=======
static int rxe_send(struct sk_buff *skb, struct rxe_pkt_info *pkt)
{
	int err;

>>>>>>> upstream/android-13
	skb->destructor = rxe_skb_tx_dtor;
	skb->sk = pkt->qp->sk->sk;

	rxe_add_ref(pkt->qp);
	atomic_inc(&pkt->qp->skb_out);

<<<<<<< HEAD
	if (av->network_type == RDMA_NETWORK_IPV4) {
		err = ip_local_out(dev_net(skb_dst(skb)->dev), skb->sk, skb);
	} else if (av->network_type == RDMA_NETWORK_IPV6) {
		err = ip6_local_out(dev_net(skb_dst(skb)->dev), skb->sk, skb);
	} else {
		pr_err("Unknown layer 3 protocol: %d\n", av->network_type);
=======
	if (skb->protocol == htons(ETH_P_IP)) {
		err = ip_local_out(dev_net(skb_dst(skb)->dev), skb->sk, skb);
	} else if (skb->protocol == htons(ETH_P_IPV6)) {
		err = ip6_local_out(dev_net(skb_dst(skb)->dev), skb->sk, skb);
	} else {
		pr_err("Unknown layer 3 protocol: %d\n", skb->protocol);
>>>>>>> upstream/android-13
		atomic_dec(&pkt->qp->skb_out);
		rxe_drop_ref(pkt->qp);
		kfree_skb(skb);
		return -EINVAL;
	}

	if (unlikely(net_xmit_eval(err))) {
		pr_debug("error sending packet: %d\n", err);
		return -EAGAIN;
	}

	return 0;
}

<<<<<<< HEAD
void rxe_loopback(struct sk_buff *skb)
{
=======
/* fix up a send packet to match the packets
 * received from UDP before looping them back
 */
static int rxe_loopback(struct sk_buff *skb, struct rxe_pkt_info *pkt)
{
	memcpy(SKB_TO_PKT(skb), pkt, sizeof(*pkt));

>>>>>>> upstream/android-13
	if (skb->protocol == htons(ETH_P_IP))
		skb_pull(skb, sizeof(struct iphdr));
	else
		skb_pull(skb, sizeof(struct ipv6hdr));

<<<<<<< HEAD
	rxe_rcv(skb);
}

static inline int addr_same(struct rxe_dev *rxe, struct rxe_av *av)
{
	return rxe->port.port_guid == av->grh.dgid.global.interface_id;
=======
	if (WARN_ON(!ib_device_try_get(&pkt->rxe->ib_dev))) {
		kfree_skb(skb);
		return -EIO;
	}

	rxe_rcv(skb);

	return 0;
}

int rxe_xmit_packet(struct rxe_qp *qp, struct rxe_pkt_info *pkt,
		    struct sk_buff *skb)
{
	int err;
	int is_request = pkt->mask & RXE_REQ_MASK;
	struct rxe_dev *rxe = to_rdev(qp->ibqp.device);

	if ((is_request && (qp->req.state != QP_STATE_READY)) ||
	    (!is_request && (qp->resp.state != QP_STATE_READY))) {
		pr_info("Packet dropped. QP is not in ready state\n");
		goto drop;
	}

	rxe_icrc_generate(skb, pkt);

	if (pkt->mask & RXE_LOOPBACK_MASK)
		err = rxe_loopback(skb, pkt);
	else
		err = rxe_send(skb, pkt);
	if (err) {
		rxe->xmit_errors++;
		rxe_counter_inc(rxe, RXE_CNT_SEND_ERR);
		return err;
	}

	if ((qp_type(qp) != IB_QPT_RC) &&
	    (pkt->mask & RXE_END_MASK)) {
		pkt->wqe->state = wqe_state_done;
		rxe_run_task(&qp->comp.task, 1);
	}

	rxe_counter_inc(rxe, RXE_CNT_SENT_PKTS);
	goto done;

drop:
	kfree_skb(skb);
	err = 0;
done:
	return err;
>>>>>>> upstream/android-13
}

struct sk_buff *rxe_init_packet(struct rxe_dev *rxe, struct rxe_av *av,
				int paylen, struct rxe_pkt_info *pkt)
{
	unsigned int hdr_len;
<<<<<<< HEAD
	struct sk_buff *skb;
=======
	struct sk_buff *skb = NULL;
>>>>>>> upstream/android-13
	struct net_device *ndev;
	const struct ib_gid_attr *attr;
	const int port_num = 1;

	attr = rdma_get_gid_attr(&rxe->ib_dev, port_num, av->grh.sgid_index);
	if (IS_ERR(attr))
		return NULL;
<<<<<<< HEAD
	ndev = attr->ndev;

	if (av->network_type == RDMA_NETWORK_IPV4)
=======

	if (av->network_type == RXE_NETWORK_TYPE_IPV4)
>>>>>>> upstream/android-13
		hdr_len = ETH_HLEN + sizeof(struct udphdr) +
			sizeof(struct iphdr);
	else
		hdr_len = ETH_HLEN + sizeof(struct udphdr) +
			sizeof(struct ipv6hdr);

<<<<<<< HEAD
	skb = alloc_skb(paylen + hdr_len + LL_RESERVED_SPACE(ndev),
			GFP_ATOMIC);

	if (unlikely(!skb))
		goto out;
=======
	rcu_read_lock();
	ndev = rdma_read_gid_attr_ndev_rcu(attr);
	if (IS_ERR(ndev)) {
		rcu_read_unlock();
		goto out;
	}
	skb = alloc_skb(paylen + hdr_len + LL_RESERVED_SPACE(ndev),
			GFP_ATOMIC);

	if (unlikely(!skb)) {
		rcu_read_unlock();
		goto out;
	}
>>>>>>> upstream/android-13

	skb_reserve(skb, hdr_len + LL_RESERVED_SPACE(ndev));

	/* FIXME: hold reference to this netdev until life of this skb. */
	skb->dev	= ndev;
<<<<<<< HEAD
	if (av->network_type == RDMA_NETWORK_IPV4)
=======
	rcu_read_unlock();

	if (av->network_type == RXE_NETWORK_TYPE_IPV4)
>>>>>>> upstream/android-13
		skb->protocol = htons(ETH_P_IP);
	else
		skb->protocol = htons(ETH_P_IPV6);

	pkt->rxe	= rxe;
	pkt->port_num	= port_num;
<<<<<<< HEAD
	pkt->hdr	= skb_put_zero(skb, paylen);
=======
	pkt->hdr	= skb_put(skb, paylen);
>>>>>>> upstream/android-13
	pkt->mask	|= RXE_GRH_MASK;

out:
	rdma_put_gid_attr(attr);
	return skb;
}

/*
 * this is required by rxe_cfg to match rxe devices in
 * /sys/class/infiniband up with their underlying ethernet devices
 */
const char *rxe_parent_name(struct rxe_dev *rxe, unsigned int port_num)
{
	return rxe->ndev->name;
}

<<<<<<< HEAD
enum rdma_link_layer rxe_link_layer(struct rxe_dev *rxe, unsigned int port_num)
{
	return IB_LINK_LAYER_ETHERNET;
}

struct rxe_dev *rxe_net_add(struct net_device *ndev)
=======
int rxe_net_add(const char *ibdev_name, struct net_device *ndev)
>>>>>>> upstream/android-13
{
	int err;
	struct rxe_dev *rxe = NULL;

<<<<<<< HEAD
	rxe = (struct rxe_dev *)ib_alloc_device(sizeof(*rxe));
	if (!rxe)
		return NULL;

	rxe->ndev = ndev;

	err = rxe_add(rxe, ndev->mtu);
	if (err) {
		ib_dealloc_device(&rxe->ib_dev);
		return NULL;
	}

	spin_lock_bh(&dev_list_lock);
	list_add_tail(&rxe->list, &rxe_dev_list);
	spin_unlock_bh(&dev_list_lock);
	return rxe;
}

void rxe_remove_all(void)
{
	spin_lock_bh(&dev_list_lock);
	while (!list_empty(&rxe_dev_list)) {
		struct rxe_dev *rxe =
			list_first_entry(&rxe_dev_list, struct rxe_dev, list);

		list_del(&rxe->list);
		spin_unlock_bh(&dev_list_lock);
		rxe_remove(rxe);
		spin_lock_bh(&dev_list_lock);
	}
	spin_unlock_bh(&dev_list_lock);
=======
	rxe = ib_alloc_device(rxe_dev, ib_dev);
	if (!rxe)
		return -ENOMEM;

	rxe->ndev = ndev;

	err = rxe_add(rxe, ndev->mtu, ibdev_name);
	if (err) {
		ib_dealloc_device(&rxe->ib_dev);
		return err;
	}

	return 0;
>>>>>>> upstream/android-13
}

static void rxe_port_event(struct rxe_dev *rxe,
			   enum ib_event_type event)
{
	struct ib_event ev;

	ev.device = &rxe->ib_dev;
	ev.element.port_num = 1;
	ev.event = event;

	ib_dispatch_event(&ev);
}

/* Caller must hold net_info_lock */
void rxe_port_up(struct rxe_dev *rxe)
{
	struct rxe_port *port;

	port = &rxe->port;
	port->attr.state = IB_PORT_ACTIVE;
<<<<<<< HEAD
	port->attr.phys_state = IB_PHYS_STATE_LINK_UP;

	rxe_port_event(rxe, IB_EVENT_PORT_ACTIVE);
	pr_info("set %s active\n", rxe->ib_dev.name);
=======

	rxe_port_event(rxe, IB_EVENT_PORT_ACTIVE);
	dev_info(&rxe->ib_dev.dev, "set active\n");
>>>>>>> upstream/android-13
}

/* Caller must hold net_info_lock */
void rxe_port_down(struct rxe_dev *rxe)
{
	struct rxe_port *port;

	port = &rxe->port;
	port->attr.state = IB_PORT_DOWN;
<<<<<<< HEAD
	port->attr.phys_state = IB_PHYS_STATE_LINK_DOWN;

	rxe_port_event(rxe, IB_EVENT_PORT_ERR);
	pr_info("set %s down\n", rxe->ib_dev.name);
=======

	rxe_port_event(rxe, IB_EVENT_PORT_ERR);
	rxe_counter_inc(rxe, RXE_CNT_LINK_DOWNED);
	dev_info(&rxe->ib_dev.dev, "set down\n");
}

void rxe_set_port_state(struct rxe_dev *rxe)
{
	if (netif_running(rxe->ndev) && netif_carrier_ok(rxe->ndev))
		rxe_port_up(rxe);
	else
		rxe_port_down(rxe);
>>>>>>> upstream/android-13
}

static int rxe_notify(struct notifier_block *not_blk,
		      unsigned long event,
		      void *arg)
{
	struct net_device *ndev = netdev_notifier_info_to_dev(arg);
<<<<<<< HEAD
	struct rxe_dev *rxe = net_to_rxe(ndev);

	if (!rxe)
		goto out;

	switch (event) {
	case NETDEV_UNREGISTER:
		list_del(&rxe->list);
		rxe_remove(rxe);
=======
	struct rxe_dev *rxe = rxe_get_dev_from_net(ndev);

	if (!rxe)
		return NOTIFY_OK;

	switch (event) {
	case NETDEV_UNREGISTER:
		ib_unregister_device_queued(&rxe->ib_dev);
>>>>>>> upstream/android-13
		break;
	case NETDEV_UP:
		rxe_port_up(rxe);
		break;
	case NETDEV_DOWN:
		rxe_port_down(rxe);
		break;
	case NETDEV_CHANGEMTU:
		pr_info("%s changed mtu to %d\n", ndev->name, ndev->mtu);
		rxe_set_mtu(rxe, ndev->mtu);
		break;
	case NETDEV_CHANGE:
<<<<<<< HEAD
		if (netif_running(ndev) && netif_carrier_ok(ndev))
			rxe_port_up(rxe);
		else
			rxe_port_down(rxe);
=======
		rxe_set_port_state(rxe);
>>>>>>> upstream/android-13
		break;
	case NETDEV_REBOOT:
	case NETDEV_GOING_DOWN:
	case NETDEV_CHANGEADDR:
	case NETDEV_CHANGENAME:
	case NETDEV_FEAT_CHANGE:
	default:
		pr_info("ignoring netdev event = %ld for %s\n",
			event, ndev->name);
		break;
	}
<<<<<<< HEAD
out:
=======

	ib_device_put(&rxe->ib_dev);
>>>>>>> upstream/android-13
	return NOTIFY_OK;
}

static struct notifier_block rxe_net_notifier = {
	.notifier_call = rxe_notify,
};

static int rxe_net_ipv4_init(void)
{
	recv_sockets.sk4 = rxe_setup_udp_tunnel(&init_net,
				htons(ROCE_V2_UDP_DPORT), false);
	if (IS_ERR(recv_sockets.sk4)) {
		recv_sockets.sk4 = NULL;
		pr_err("Failed to create IPv4 UDP tunnel\n");
		return -1;
	}

	return 0;
}

static int rxe_net_ipv6_init(void)
{
#if IS_ENABLED(CONFIG_IPV6)

	recv_sockets.sk6 = rxe_setup_udp_tunnel(&init_net,
						htons(ROCE_V2_UDP_DPORT), true);
<<<<<<< HEAD
=======
	if (PTR_ERR(recv_sockets.sk6) == -EAFNOSUPPORT) {
		recv_sockets.sk6 = NULL;
		pr_warn("IPv6 is not supported, can not create a UDPv6 socket\n");
		return 0;
	}

>>>>>>> upstream/android-13
	if (IS_ERR(recv_sockets.sk6)) {
		recv_sockets.sk6 = NULL;
		pr_err("Failed to create IPv6 UDP tunnel\n");
		return -1;
	}
#endif
	return 0;
}

void rxe_net_exit(void)
{
	rxe_release_udp_tunnel(recv_sockets.sk6);
	rxe_release_udp_tunnel(recv_sockets.sk4);
	unregister_netdevice_notifier(&rxe_net_notifier);
}

int rxe_net_init(void)
{
	int err;

	recv_sockets.sk6 = NULL;

	err = rxe_net_ipv4_init();
	if (err)
		return err;
	err = rxe_net_ipv6_init();
	if (err)
		goto err_out;
	err = register_netdevice_notifier(&rxe_net_notifier);
	if (err) {
		pr_err("Failed to register netdev notifier\n");
		goto err_out;
	}
	return 0;
err_out:
	rxe_net_exit();
	return err;
}
