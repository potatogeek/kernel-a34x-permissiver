/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef IF_CAIF_H_
#define IF_CAIF_H_
#include <linux/sockios.h>
#include <linux/types.h>
#include <linux/socket.h>


enum ifla_caif {
	__IFLA_CAIF_UNSPEC,
	IFLA_CAIF_IPV4_CONNID,
	IFLA_CAIF_IPV6_CONNID,
	IFLA_CAIF_LOOPBACK,
	__IFLA_CAIF_MAX
};
#define	IFLA_CAIF_MAX (__IFLA_CAIF_MAX-1)

#endif 
