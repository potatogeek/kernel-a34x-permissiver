/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SOCKET_H
#define _LINUX_SOCKET_H


#include <asm/socket.h>			
#include <linux/sockios.h>		
#include <linux/uio.h>			
#include <linux/types.h>		
#include <linux/compiler.h>		
#include <uapi/linux/socket.h>

struct pid;
struct cred;

#define __sockaddr_check_size(size)	\
	BUILD_BUG_ON(((size) > sizeof(struct __kernel_sockaddr_storage)))

#ifdef CONFIG_PROC_FS
struct seq_file;
extern void socket_seq_show(struct seq_file *seq);
#endif

typedef __kernel_sa_family_t	sa_family_t;


 
struct sockaddr {
	sa_family_t	sa_family;	
	char		sa_data[14];	
};

struct linger {
	int		l_onoff;	
	int		l_linger;	
};

#define sockaddr_storage __kernel_sockaddr_storage


 
struct msghdr {
	void		*msg_name;	
	int		msg_namelen;	
	struct iov_iter	msg_iter;	
	void		*msg_control;	
	__kernel_size_t	msg_controllen;	
	unsigned int	msg_flags;	
	struct kiocb	*msg_iocb;	
};
 
struct user_msghdr {
	void		__user *msg_name;	
	int		msg_namelen;		
	struct iovec	__user *msg_iov;	
	__kernel_size_t	msg_iovlen;		
	void		__user *msg_control;	
	__kernel_size_t	msg_controllen;		
	unsigned int	msg_flags;		
};


struct mmsghdr {
	struct user_msghdr  msg_hdr;
	unsigned int        msg_len;
};



struct cmsghdr {
	__kernel_size_t	cmsg_len;	
        int		cmsg_level;	
        int		cmsg_type;	
};



#define __CMSG_NXTHDR(ctl, len, cmsg) __cmsg_nxthdr((ctl),(len),(cmsg))
#define CMSG_NXTHDR(mhdr, cmsg) cmsg_nxthdr((mhdr), (cmsg))

#define CMSG_ALIGN(len) ( ((len)+sizeof(long)-1) & ~(sizeof(long)-1) )

#define CMSG_DATA(cmsg)	((void *)((char *)(cmsg) + sizeof(struct cmsghdr)))
#define CMSG_SPACE(len) (sizeof(struct cmsghdr) + CMSG_ALIGN(len))
#define CMSG_LEN(len) (sizeof(struct cmsghdr) + (len))

#define __CMSG_FIRSTHDR(ctl,len) ((len) >= sizeof(struct cmsghdr) ? \
				  (struct cmsghdr *)(ctl) : \
				  (struct cmsghdr *)NULL)
#define CMSG_FIRSTHDR(msg)	__CMSG_FIRSTHDR((msg)->msg_control, (msg)->msg_controllen)
#define CMSG_OK(mhdr, cmsg) ((cmsg)->cmsg_len >= sizeof(struct cmsghdr) && \
			     (cmsg)->cmsg_len <= (unsigned long) \
			     ((mhdr)->msg_controllen - \
			      ((char *)(cmsg) - (char *)(mhdr)->msg_control)))
#define for_each_cmsghdr(cmsg, msg) \
	for (cmsg = CMSG_FIRSTHDR(msg); \
	     cmsg; \
	     cmsg = CMSG_NXTHDR(msg, cmsg))


 
static inline struct cmsghdr * __cmsg_nxthdr(void *__ctl, __kernel_size_t __size,
					       struct cmsghdr *__cmsg)
{
	struct cmsghdr * __ptr;

	__ptr = (struct cmsghdr*)(((unsigned char *) __cmsg) +  CMSG_ALIGN(__cmsg->cmsg_len));
	if ((unsigned long)((char*)(__ptr+1) - (char *) __ctl) > __size)
		return (struct cmsghdr *)0;

	return __ptr;
}

static inline struct cmsghdr * cmsg_nxthdr (struct msghdr *__msg, struct cmsghdr *__cmsg)
{
	return __cmsg_nxthdr(__msg->msg_control, __msg->msg_controllen, __cmsg);
}

static inline size_t msg_data_left(struct msghdr *msg)
{
	return iov_iter_count(&msg->msg_iter);
}



#define	SCM_RIGHTS	0x01		
#define SCM_CREDENTIALS 0x02		
#define SCM_SECURITY	0x03		

struct ucred {
	__u32	pid;
	__u32	uid;
	__u32	gid;
};


#define AF_UNSPEC	0
#define AF_UNIX		1	
#define AF_LOCAL	1	
#define AF_INET		2	
#define AF_AX25		3	
#define AF_IPX		4	
#define AF_APPLETALK	5	
#define AF_NETROM	6	
#define AF_BRIDGE	7	
#define AF_ATMPVC	8	
#define AF_X25		9	
#define AF_INET6	10	
#define AF_ROSE		11	
#define AF_DECnet	12	
#define AF_NETBEUI	13	
#define AF_SECURITY	14	
#define AF_KEY		15      
#define AF_NETLINK	16
#define AF_ROUTE	AF_NETLINK 
#define AF_PACKET	17	
#define AF_ASH		18	
#define AF_ECONET	19	
#define AF_ATMSVC	20	
#define AF_RDS		21	
#define AF_SNA		22	
#define AF_IRDA		23	
#define AF_PPPOX	24	
#define AF_WANPIPE	25	
#define AF_LLC		26	
#define AF_IB		27	
#define AF_MPLS		28	
#define AF_CAN		29	
#define AF_TIPC		30	
#define AF_BLUETOOTH	31	
#define AF_IUCV		32	
#define AF_RXRPC	33	
#define AF_ISDN		34	
#define AF_PHONET	35	
#define AF_IEEE802154	36	
#define AF_CAIF		37	
#define AF_ALG		38	
#define AF_NFC		39	
#define AF_VSOCK	40	
#define AF_KCM		41	
#define AF_QIPCRTR	42	
#define AF_SMC		43	
#define AF_XDP		44	

#define AF_MAX		45	


#define PF_UNSPEC	AF_UNSPEC
#define PF_UNIX		AF_UNIX
#define PF_LOCAL	AF_LOCAL
#define PF_INET		AF_INET
#define PF_AX25		AF_AX25
#define PF_IPX		AF_IPX
#define PF_APPLETALK	AF_APPLETALK
#define	PF_NETROM	AF_NETROM
#define PF_BRIDGE	AF_BRIDGE
#define PF_ATMPVC	AF_ATMPVC
#define PF_X25		AF_X25
#define PF_INET6	AF_INET6
#define PF_ROSE		AF_ROSE
#define PF_DECnet	AF_DECnet
#define PF_NETBEUI	AF_NETBEUI
#define PF_SECURITY	AF_SECURITY
#define PF_KEY		AF_KEY
#define PF_NETLINK	AF_NETLINK
#define PF_ROUTE	AF_ROUTE
#define PF_PACKET	AF_PACKET
#define PF_ASH		AF_ASH
#define PF_ECONET	AF_ECONET
#define PF_ATMSVC	AF_ATMSVC
#define PF_RDS		AF_RDS
#define PF_SNA		AF_SNA
#define PF_IRDA		AF_IRDA
#define PF_PPPOX	AF_PPPOX
#define PF_WANPIPE	AF_WANPIPE
#define PF_LLC		AF_LLC
#define PF_IB		AF_IB
#define PF_MPLS		AF_MPLS
#define PF_CAN		AF_CAN
#define PF_TIPC		AF_TIPC
#define PF_BLUETOOTH	AF_BLUETOOTH
#define PF_IUCV		AF_IUCV
#define PF_RXRPC	AF_RXRPC
#define PF_ISDN		AF_ISDN
#define PF_PHONET	AF_PHONET
#define PF_IEEE802154	AF_IEEE802154
#define PF_CAIF		AF_CAIF
#define PF_ALG		AF_ALG
#define PF_NFC		AF_NFC
#define PF_VSOCK	AF_VSOCK
#define PF_KCM		AF_KCM
#define PF_QIPCRTR	AF_QIPCRTR
#define PF_SMC		AF_SMC
#define PF_XDP		AF_XDP
#define PF_MAX		AF_MAX


#define SOMAXCONN	128


 
#define MSG_OOB		1
#define MSG_PEEK	2
#define MSG_DONTROUTE	4
#define MSG_TRYHARD     4       
#define MSG_CTRUNC	8
#define MSG_PROBE	0x10	
#define MSG_TRUNC	0x20
#define MSG_DONTWAIT	0x40	
#define MSG_EOR         0x80	
#define MSG_WAITALL	0x100	
#define MSG_FIN         0x200
#define MSG_SYN		0x400
#define MSG_CONFIRM	0x800	
#define MSG_RST		0x1000
#define MSG_ERRQUEUE	0x2000	
#define MSG_NOSIGNAL	0x4000	
#define MSG_MORE	0x8000	
#define MSG_WAITFORONE	0x10000	
#define MSG_SENDPAGE_NOTLAST 0x20000 
#define MSG_BATCH	0x40000 
#define MSG_EOF         MSG_FIN
#define MSG_NO_SHARED_FRAGS 0x80000 

#define MSG_ZEROCOPY	0x4000000	
#define MSG_FASTOPEN	0x20000000	
#define MSG_CMSG_CLOEXEC 0x40000000	
#if defined(CONFIG_COMPAT)
#define MSG_CMSG_COMPAT	0x80000000	
#else
#define MSG_CMSG_COMPAT	0		
#endif



#define SOL_IP		0

#define SOL_TCP		6
#define SOL_UDP		17
#define SOL_IPV6	41
#define SOL_ICMPV6	58
#define SOL_SCTP	132
#define SOL_UDPLITE	136     
#define SOL_RAW		255
#define SOL_IPX		256
#define SOL_AX25	257
#define SOL_ATALK	258
#define SOL_NETROM	259
#define SOL_ROSE	260
#define SOL_DECNET	261
#define	SOL_X25		262
#define SOL_PACKET	263
#define SOL_ATM		264	
#define SOL_AAL		265	
#define SOL_IRDA        266
#define SOL_NETBEUI	267
#define SOL_LLC		268
#define SOL_DCCP	269
#define SOL_NETLINK	270
#define SOL_TIPC	271
#define SOL_RXRPC	272
#define SOL_PPPOL2TP	273
#define SOL_BLUETOOTH	274
#define SOL_PNPIPE	275
#define SOL_RDS		276
#define SOL_IUCV	277
#define SOL_CAIF	278
#define SOL_ALG		279
#define SOL_NFC		280
#define SOL_KCM		281
#define SOL_TLS		282
#define SOL_XDP		283


#define IPX_TYPE	1

extern int move_addr_to_kernel(void __user *uaddr, int ulen, struct sockaddr_storage *kaddr);
extern int put_cmsg(struct msghdr*, int level, int type, int len, void *data);

struct timespec;


extern long __sys_recvmsg(int fd, struct user_msghdr __user *msg,
			  unsigned int flags, bool forbid_cmsg_compat);
extern long __sys_sendmsg(int fd, struct user_msghdr __user *msg,
			  unsigned int flags, bool forbid_cmsg_compat);
extern int __sys_recvmmsg(int fd, struct mmsghdr __user *mmsg, unsigned int vlen,
			  unsigned int flags, struct timespec *timeout);
extern int __sys_sendmmsg(int fd, struct mmsghdr __user *mmsg,
			  unsigned int vlen, unsigned int flags,
			  bool forbid_cmsg_compat);


extern int __sys_recvfrom(int fd, void __user *ubuf, size_t size,
			  unsigned int flags, struct sockaddr __user *addr,
			  int __user *addr_len);
extern int __sys_sendto(int fd, void __user *buff, size_t len,
			unsigned int flags, struct sockaddr __user *addr,
			int addr_len);
extern int __sys_accept4(int fd, struct sockaddr __user *upeer_sockaddr,
			 int __user *upeer_addrlen, int flags);
extern int __sys_socket(int family, int type, int protocol);
extern int __sys_bind(int fd, struct sockaddr __user *umyaddr, int addrlen);
extern int __sys_connect(int fd, struct sockaddr __user *uservaddr,
			 int addrlen);
extern int __sys_listen(int fd, int backlog);
extern int __sys_getsockname(int fd, struct sockaddr __user *usockaddr,
			     int __user *usockaddr_len);
extern int __sys_getpeername(int fd, struct sockaddr __user *usockaddr,
			     int __user *usockaddr_len);
extern int __sys_socketpair(int family, int type, int protocol,
			    int __user *usockvec);
extern int __sys_shutdown(int fd, int how);

extern struct ns_common *get_net_ns(struct ns_common *ns);
#endif 
