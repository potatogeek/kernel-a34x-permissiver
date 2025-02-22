/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LWTUNNEL_H_
#define _UAPI_LWTUNNEL_H_

#include <linux/types.h>

enum lwtunnel_encap_types {
	LWTUNNEL_ENCAP_NONE,
	LWTUNNEL_ENCAP_MPLS,
	LWTUNNEL_ENCAP_IP,
	LWTUNNEL_ENCAP_ILA,
	LWTUNNEL_ENCAP_IP6,
	LWTUNNEL_ENCAP_SEG6,
	LWTUNNEL_ENCAP_BPF,
	LWTUNNEL_ENCAP_SEG6_LOCAL,
	__LWTUNNEL_ENCAP_MAX,
};

#define LWTUNNEL_ENCAP_MAX (__LWTUNNEL_ENCAP_MAX - 1)

enum lwtunnel_ip_t {
	LWTUNNEL_IP_UNSPEC,
	LWTUNNEL_IP_ID,
	LWTUNNEL_IP_DST,
	LWTUNNEL_IP_SRC,
	LWTUNNEL_IP_TTL,
	LWTUNNEL_IP_TOS,
	LWTUNNEL_IP_FLAGS,
	LWTUNNEL_IP_PAD,
	__LWTUNNEL_IP_MAX,
};

#define LWTUNNEL_IP_MAX (__LWTUNNEL_IP_MAX - 1)

enum lwtunnel_ip6_t {
	LWTUNNEL_IP6_UNSPEC,
	LWTUNNEL_IP6_ID,
	LWTUNNEL_IP6_DST,
	LWTUNNEL_IP6_SRC,
	LWTUNNEL_IP6_HOPLIMIT,
	LWTUNNEL_IP6_TC,
	LWTUNNEL_IP6_FLAGS,
	LWTUNNEL_IP6_PAD,
	__LWTUNNEL_IP6_MAX,
};

#define LWTUNNEL_IP6_MAX (__LWTUNNEL_IP6_MAX - 1)

enum {
	LWT_BPF_PROG_UNSPEC,
	LWT_BPF_PROG_FD,
	LWT_BPF_PROG_NAME,
	__LWT_BPF_PROG_MAX,
};

#define LWT_BPF_PROG_MAX (__LWT_BPF_PROG_MAX - 1)

enum {
	LWT_BPF_UNSPEC,
	LWT_BPF_IN,
	LWT_BPF_OUT,
	LWT_BPF_XMIT,
	LWT_BPF_XMIT_HEADROOM,
	__LWT_BPF_MAX,
};

#define LWT_BPF_MAX (__LWT_BPF_MAX - 1)

#define LWT_BPF_MAX_HEADROOM 256

#endif 
