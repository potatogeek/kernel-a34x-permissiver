/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPILINUX_PHONET_H
#define _UAPILINUX_PHONET_H

#include <linux/types.h>
#include <linux/socket.h>


#define PN_PROTO_TRANSPORT	0

#define PN_PROTO_PHONET		1

#define PN_PROTO_PIPE		2
#define PHONET_NPROTO		3


#define PNPIPE_ENCAP		1
#define PNPIPE_IFINDEX		2
#define PNPIPE_HANDLE		3
#define PNPIPE_INITSTATE	4

#define PNADDR_ANY		0
#define PNADDR_BROADCAST	0xFC
#define PNPORT_RESOURCE_ROUTING	0


#define PNPIPE_ENCAP_NONE	0
#define PNPIPE_ENCAP_IP		1


#define SIOCPNGETOBJECT		(SIOCPROTOPRIVATE + 0)
#define SIOCPNENABLEPIPE	(SIOCPROTOPRIVATE + 13)
#define SIOCPNADDRESOURCE	(SIOCPROTOPRIVATE + 14)
#define SIOCPNDELRESOURCE	(SIOCPROTOPRIVATE + 15)


struct phonethdr {
	__u8	pn_rdev;
	__u8	pn_sdev;
	__u8	pn_res;
	__be16	pn_length;
	__u8	pn_robj;
	__u8	pn_sobj;
} __attribute__((packed));


struct phonetmsg {
	__u8	pn_trans_id;	
	__u8	pn_msg_id;	
	union {
		struct {
			__u8	pn_submsg_id;	
			__u8	pn_data[5];
		} base;
		struct {
			__u16	pn_e_res_id;	
			__u8	pn_e_submsg_id;	
			__u8	pn_e_data[3];
		} ext;
	} pn_msg_u;
};
#define PN_COMMON_MESSAGE	0xF0
#define PN_COMMGR		0x10
#define PN_PREFIX		0xE0 
#define pn_submsg_id		pn_msg_u.base.pn_submsg_id
#define pn_e_submsg_id		pn_msg_u.ext.pn_e_submsg_id
#define pn_e_res_id		pn_msg_u.ext.pn_e_res_id
#define pn_data			pn_msg_u.base.pn_data
#define pn_e_data		pn_msg_u.ext.pn_e_data


#define PN_COMM_SERVICE_NOT_IDENTIFIED_RESP	0x01
#define PN_COMM_ISA_ENTITY_NOT_REACHABLE_RESP	0x14
#define pn_orig_msg_id		pn_data[0]
#define pn_status		pn_data[1]
#define pn_e_orig_msg_id	pn_e_data[0]
#define pn_e_status		pn_e_data[1]


struct sockaddr_pn {
	__kernel_sa_family_t spn_family;
	__u8 spn_obj;
	__u8 spn_dev;
	__u8 spn_resource;
	__u8 spn_zero[sizeof(struct sockaddr) - sizeof(__kernel_sa_family_t) - 3];
} __attribute__((packed));


#define PN_DEV_PC	0x10

static inline __u16 pn_object(__u8 addr, __u16 port)
{
	return (addr << 8) | (port & 0x3ff);
}

static inline __u8 pn_obj(__u16 handle)
{
	return handle & 0xff;
}

static inline __u8 pn_dev(__u16 handle)
{
	return handle >> 8;
}

static inline __u16 pn_port(__u16 handle)
{
	return handle & 0x3ff;
}

static inline __u8 pn_addr(__u16 handle)
{
	return (handle >> 8) & 0xfc;
}

static inline void pn_sockaddr_set_addr(struct sockaddr_pn *spn, __u8 addr)
{
	spn->spn_dev &= 0x03;
	spn->spn_dev |= addr & 0xfc;
}

static inline void pn_sockaddr_set_port(struct sockaddr_pn *spn, __u16 port)
{
	spn->spn_dev &= 0xfc;
	spn->spn_dev |= (port >> 8) & 0x03;
	spn->spn_obj = port & 0xff;
}

static inline void pn_sockaddr_set_object(struct sockaddr_pn *spn,
						__u16 handle)
{
	spn->spn_dev = pn_dev(handle);
	spn->spn_obj = pn_obj(handle);
}

static inline void pn_sockaddr_set_resource(struct sockaddr_pn *spn,
						__u8 resource)
{
	spn->spn_resource = resource;
}

static inline __u8 pn_sockaddr_get_addr(const struct sockaddr_pn *spn)
{
	return spn->spn_dev & 0xfc;
}

static inline __u16 pn_sockaddr_get_port(const struct sockaddr_pn *spn)
{
	return ((spn->spn_dev & 0x03) << 8) | spn->spn_obj;
}

static inline __u16 pn_sockaddr_get_object(const struct sockaddr_pn *spn)
{
	return pn_object(spn->spn_dev, spn->spn_obj);
}

static inline __u8 pn_sockaddr_get_resource(const struct sockaddr_pn *spn)
{
	return spn->spn_resource;
}



#endif 
