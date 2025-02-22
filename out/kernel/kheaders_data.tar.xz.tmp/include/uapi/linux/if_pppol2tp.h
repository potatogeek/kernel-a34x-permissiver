/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */


#ifndef _UAPI__LINUX_IF_PPPOL2TP_H
#define _UAPI__LINUX_IF_PPPOL2TP_H

#include <linux/types.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/l2tp.h>


struct pppol2tp_addr {
	__kernel_pid_t	pid;		
	int	fd;			

	struct sockaddr_in addr;	

	__u16 s_tunnel, s_session;	
	__u16 d_tunnel, d_session;	
};


struct pppol2tpin6_addr {
	__kernel_pid_t	pid;		
	int	fd;			

	__u16 s_tunnel, s_session;	
	__u16 d_tunnel, d_session;	

	struct sockaddr_in6 addr;	
};


struct pppol2tpv3_addr {
	__kernel_pid_t	pid;		
	int	fd;			

	struct sockaddr_in addr;	

	__u32 s_tunnel, s_session;	
	__u32 d_tunnel, d_session;	
};

struct pppol2tpv3in6_addr {
	__kernel_pid_t	pid;		
	int	fd;			

	__u32 s_tunnel, s_session;	
	__u32 d_tunnel, d_session;	

	struct sockaddr_in6 addr;	
};


enum {
	PPPOL2TP_SO_DEBUG	= 1,
	PPPOL2TP_SO_RECVSEQ	= 2,
	PPPOL2TP_SO_SENDSEQ	= 3,
	PPPOL2TP_SO_LNSMODE	= 4,
	PPPOL2TP_SO_REORDERTO	= 5,
};


enum {
	PPPOL2TP_MSG_DEBUG	= L2TP_MSG_DEBUG,
	PPPOL2TP_MSG_CONTROL	= L2TP_MSG_CONTROL,
	PPPOL2TP_MSG_SEQ	= L2TP_MSG_SEQ,
	PPPOL2TP_MSG_DATA	= L2TP_MSG_DATA,
};



#endif 
