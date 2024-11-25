/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  linux/include/linux/sunrpc/xprtsock.h
 *
 *  Declarations for the RPC transport socket provider.
 */

#ifndef _LINUX_SUNRPC_XPRTSOCK_H
#define _LINUX_SUNRPC_XPRTSOCK_H

<<<<<<< HEAD
#ifdef __KERNEL__

int		init_socket_xprt(void);
void		cleanup_socket_xprt(void);
=======
int		init_socket_xprt(void);
void		cleanup_socket_xprt(void);
unsigned short	get_srcport(struct rpc_xprt *);
>>>>>>> upstream/android-13

#define RPC_MIN_RESVPORT	(1U)
#define RPC_MAX_RESVPORT	(65535U)
#define RPC_DEF_MIN_RESVPORT	(665U)
#define RPC_DEF_MAX_RESVPORT	(1023U)

struct sock_xprt {
	struct rpc_xprt		xprt;

	/*
	 * Network layer
	 */
	struct socket *		sock;
	struct sock *		inet;
<<<<<<< HEAD
=======
	struct file *		file;
>>>>>>> upstream/android-13

	/*
	 * State of TCP reply receive
	 */
<<<<<<< HEAD
	__be32			tcp_fraghdr,
				tcp_xid,
				tcp_calldir;

	u32			tcp_offset,
				tcp_reclen;

	unsigned long		tcp_copied,
				tcp_flags;
=======
	struct {
		struct {
			__be32	fraghdr,
				xid,
				calldir;
		} __attribute__((packed));

		u32		offset,
				len;

		unsigned long	copied;
	} recv;

	/*
	 * State of TCP transmit queue
	 */
	struct {
		u32		offset;
	} xmit;
>>>>>>> upstream/android-13

	/*
	 * Connection of transports
	 */
	unsigned long		sock_state;
	struct delayed_work	connect_worker;
<<<<<<< HEAD
=======
	struct work_struct	error_worker;
>>>>>>> upstream/android-13
	struct work_struct	recv_worker;
	struct mutex		recv_mutex;
	struct sockaddr_storage	srcaddr;
	unsigned short		srcport;
<<<<<<< HEAD
=======
	int			xprt_err;
>>>>>>> upstream/android-13

	/*
	 * UDP socket buffer size parameters
	 */
	size_t			rcvsize,
				sndsize;

	struct rpc_timeout	tcp_timeout;

	/*
	 * Saved socket callback addresses
	 */
	void			(*old_data_ready)(struct sock *);
	void			(*old_state_change)(struct sock *);
	void			(*old_write_space)(struct sock *);
	void			(*old_error_report)(struct sock *);
};

/*
<<<<<<< HEAD
 * TCP receive state flags
 */
#define TCP_RCV_LAST_FRAG	(1UL << 0)
#define TCP_RCV_COPY_FRAGHDR	(1UL << 1)
#define TCP_RCV_COPY_XID	(1UL << 2)
#define TCP_RCV_COPY_DATA	(1UL << 3)
#define TCP_RCV_READ_CALLDIR	(1UL << 4)
#define TCP_RCV_COPY_CALLDIR	(1UL << 5)

/*
 * TCP RPC flags
 */
#define TCP_RPC_REPLY		(1UL << 6)

#define XPRT_SOCK_CONNECTING	1U
#define XPRT_SOCK_DATA_READY	(2)
#define XPRT_SOCK_UPD_TIMEOUT	(3)

#endif /* __KERNEL__ */
=======
 * TCP RPC flags
 */
#define XPRT_SOCK_CONNECTING	1U
#define XPRT_SOCK_DATA_READY	(2)
#define XPRT_SOCK_UPD_TIMEOUT	(3)
#define XPRT_SOCK_WAKE_ERROR	(4)
#define XPRT_SOCK_WAKE_WRITE	(5)
#define XPRT_SOCK_WAKE_PENDING	(6)
#define XPRT_SOCK_WAKE_DISCONNECT	(7)
#define XPRT_SOCK_CONNECT_SENT	(8)
>>>>>>> upstream/android-13

#endif /* _LINUX_SUNRPC_XPRTSOCK_H */
