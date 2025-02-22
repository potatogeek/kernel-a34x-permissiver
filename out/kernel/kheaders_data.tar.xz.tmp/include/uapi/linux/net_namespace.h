/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef _UAPI_LINUX_NET_NAMESPACE_H_
#define _UAPI_LINUX_NET_NAMESPACE_H_


enum {
	NETNSA_NONE,
#define NETNSA_NSID_NOT_ASSIGNED -1
	NETNSA_NSID,
	NETNSA_PID,
	NETNSA_FD,
	__NETNSA_MAX,
};

#define NETNSA_MAX		(__NETNSA_MAX - 1)

#endif 
