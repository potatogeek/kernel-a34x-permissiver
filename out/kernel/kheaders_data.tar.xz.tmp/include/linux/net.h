
#ifndef _LINUX_NET_H
#define _LINUX_NET_H

#include <linux/stringify.h>
#include <linux/random.h>
#include <linux/wait.h>
#include <linux/fcntl.h>	
#include <linux/rcupdate.h>
#include <linux/once.h>
#include <linux/fs.h>
#include <linux/android_kabi.h>

#include <uapi/linux/net.h>

struct poll_table_struct;
struct pipe_inode_info;
struct inode;
struct file;
struct net;


#define SOCKWQ_ASYNC_NOSPACE	0
#define SOCKWQ_ASYNC_WAITDATA	1
#define SOCK_NOSPACE		2
#define SOCK_PASSCRED		3
#define SOCK_PASSSEC		4

#ifndef ARCH_HAS_SOCKET_TYPES

enum sock_type {
	SOCK_STREAM	= 1,
	SOCK_DGRAM	= 2,
	SOCK_RAW	= 3,
	SOCK_RDM	= 4,
	SOCK_SEQPACKET	= 5,
	SOCK_DCCP	= 6,
	SOCK_PACKET	= 10,
};

#define SOCK_MAX (SOCK_PACKET + 1)

#define SOCK_TYPE_MASK 0xf


#define SOCK_CLOEXEC	O_CLOEXEC
#ifndef SOCK_NONBLOCK
#define SOCK_NONBLOCK	O_NONBLOCK
#endif

#endif 

enum sock_shutdown_cmd {
	SHUT_RD,
	SHUT_WR,
	SHUT_RDWR,
};

struct socket_wq {
	
	wait_queue_head_t	wait;
	struct fasync_struct	*fasync_list;
	unsigned long		flags; 
	struct rcu_head		rcu;
} ____cacheline_aligned_in_smp;


struct socket {
	socket_state		state;

	short			type;

	unsigned long		flags;

	struct socket_wq	*wq;

	struct file		*file;
	struct sock		*sk;
	const struct proto_ops	*ops;
};

struct vm_area_struct;
struct page;
struct sockaddr;
struct msghdr;
struct module;
struct sk_buff;
typedef int (*sk_read_actor_t)(read_descriptor_t *, struct sk_buff *,
			       unsigned int, size_t);

struct proto_ops {
	int		family;
	struct module	*owner;
	int		(*release)   (struct socket *sock);
	int		(*bind)	     (struct socket *sock,
				      struct sockaddr *myaddr,
				      int sockaddr_len);
	int		(*connect)   (struct socket *sock,
				      struct sockaddr *vaddr,
				      int sockaddr_len, int flags);
	int		(*socketpair)(struct socket *sock1,
				      struct socket *sock2);
	int		(*accept)    (struct socket *sock,
				      struct socket *newsock, int flags, bool kern);
	int		(*getname)   (struct socket *sock,
				      struct sockaddr *addr,
				      int peer);
	__poll_t	(*poll)	     (struct file *file, struct socket *sock,
				      struct poll_table_struct *wait);
	int		(*ioctl)     (struct socket *sock, unsigned int cmd,
				      unsigned long arg);
#ifdef CONFIG_COMPAT
	int	 	(*compat_ioctl) (struct socket *sock, unsigned int cmd,
				      unsigned long arg);
#endif
	int		(*listen)    (struct socket *sock, int len);
	int		(*shutdown)  (struct socket *sock, int flags);
	int		(*setsockopt)(struct socket *sock, int level,
				      int optname, char __user *optval, unsigned int optlen);
	int		(*getsockopt)(struct socket *sock, int level,
				      int optname, char __user *optval, int __user *optlen);
#ifdef CONFIG_COMPAT
	int		(*compat_setsockopt)(struct socket *sock, int level,
				      int optname, char __user *optval, unsigned int optlen);
	int		(*compat_getsockopt)(struct socket *sock, int level,
				      int optname, char __user *optval, int __user *optlen);
#endif
	int		(*sendmsg)   (struct socket *sock, struct msghdr *m,
				      size_t total_len);
	
	int		(*recvmsg)   (struct socket *sock, struct msghdr *m,
				      size_t total_len, int flags);
	int		(*mmap)	     (struct file *file, struct socket *sock,
				      struct vm_area_struct * vma);
	ssize_t		(*sendpage)  (struct socket *sock, struct page *page,
				      int offset, size_t size, int flags);
	ssize_t 	(*splice_read)(struct socket *sock,  loff_t *ppos,
				       struct pipe_inode_info *pipe, size_t len, unsigned int flags);
	int		(*set_peek_off)(struct sock *sk, int val);
	int		(*peek_len)(struct socket *sock);

	
	int		(*read_sock)(struct sock *sk, read_descriptor_t *desc,
				     sk_read_actor_t recv_actor);
	int		(*sendpage_locked)(struct sock *sk, struct page *page,
					   int offset, size_t size, int flags);
	int		(*sendmsg_locked)(struct sock *sk, struct msghdr *msg,
					  size_t size);
	int		(*set_rcvlowat)(struct sock *sk, int val);

	ANDROID_KABI_RESERVE(1);
	ANDROID_KABI_RESERVE(2);
	ANDROID_KABI_RESERVE(3);
	ANDROID_KABI_RESERVE(4);
};

#define DECLARE_SOCKADDR(type, dst, src)	\
	type dst = ({ __sockaddr_check_size(sizeof(*dst)); (type) src; })

struct net_proto_family {
	int		family;
	int		(*create)(struct net *net, struct socket *sock,
				  int protocol, int kern);
	struct module	*owner;
};

struct iovec;
struct kvec;

enum {
	SOCK_WAKE_IO,
	SOCK_WAKE_WAITD,
	SOCK_WAKE_SPACE,
	SOCK_WAKE_URG,
};

int sock_wake_async(struct socket_wq *sk_wq, int how, int band);
int sock_register(const struct net_proto_family *fam);
void sock_unregister(int family);
bool sock_is_registered(int family);
int __sock_create(struct net *net, int family, int type, int proto,
		  struct socket **res, int kern);
int sock_create(int family, int type, int proto, struct socket **res);
int sock_create_kern(struct net *net, int family, int type, int proto, struct socket **res);
int sock_create_lite(int family, int type, int proto, struct socket **res);
struct socket *sock_alloc(void);
void sock_release(struct socket *sock);
int sock_sendmsg(struct socket *sock, struct msghdr *msg);
int sock_recvmsg(struct socket *sock, struct msghdr *msg, int flags);
struct file *sock_alloc_file(struct socket *sock, int flags, const char *dname);
struct socket *sockfd_lookup(int fd, int *err);
struct socket *sock_from_file(struct file *file, int *err);
#define		     sockfd_put(sock) fput(sock->file)
int net_ratelimit(void);

#define net_ratelimited_function(function, ...)			\
do {								\
	if (net_ratelimit())					\
		function(__VA_ARGS__);				\
} while (0)

#define net_emerg_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_emerg, fmt, ##__VA_ARGS__)
#define net_alert_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_alert, fmt, ##__VA_ARGS__)
#define net_crit_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_crit, fmt, ##__VA_ARGS__)
#define net_err_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_err, fmt, ##__VA_ARGS__)
#define net_notice_ratelimited(fmt, ...)			\
	net_ratelimited_function(pr_notice, fmt, ##__VA_ARGS__)
#define net_warn_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_warn, fmt, ##__VA_ARGS__)
#define net_info_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_info, fmt, ##__VA_ARGS__)
#if defined(CONFIG_DYNAMIC_DEBUG)
#define net_dbg_ratelimited(fmt, ...)					\
do {									\
	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);			\
	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT) &&	\
	    net_ratelimit())						\
		__dynamic_pr_debug(&descriptor, pr_fmt(fmt),		\
		                   ##__VA_ARGS__);			\
} while (0)
#elif defined(DEBUG)
#define net_dbg_ratelimited(fmt, ...)				\
	net_ratelimited_function(pr_debug, fmt, ##__VA_ARGS__)
#else
#define net_dbg_ratelimited(fmt, ...)				\
	do {							\
		if (0)						\
			no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__); \
	} while (0)
#endif

#define net_get_random_once(buf, nbytes)			\
	get_random_once((buf), (nbytes))
#define net_get_random_once_wait(buf, nbytes)			\
	get_random_once_wait((buf), (nbytes))

int kernel_sendmsg(struct socket *sock, struct msghdr *msg, struct kvec *vec,
		   size_t num, size_t len);
int kernel_sendmsg_locked(struct sock *sk, struct msghdr *msg,
			  struct kvec *vec, size_t num, size_t len);
int kernel_recvmsg(struct socket *sock, struct msghdr *msg, struct kvec *vec,
		   size_t num, size_t len, int flags);

int kernel_bind(struct socket *sock, struct sockaddr *addr, int addrlen);
int kernel_listen(struct socket *sock, int backlog);
int kernel_accept(struct socket *sock, struct socket **newsock, int flags);
int kernel_connect(struct socket *sock, struct sockaddr *addr, int addrlen,
		   int flags);
int kernel_getsockname(struct socket *sock, struct sockaddr *addr);
int kernel_getpeername(struct socket *sock, struct sockaddr *addr);
int kernel_getsockopt(struct socket *sock, int level, int optname, char *optval,
		      int *optlen);
int kernel_setsockopt(struct socket *sock, int level, int optname, char *optval,
		      unsigned int optlen);
int kernel_sendpage(struct socket *sock, struct page *page, int offset,
		    size_t size, int flags);
int kernel_sendpage_locked(struct sock *sk, struct page *page, int offset,
			   size_t size, int flags);
int kernel_sock_shutdown(struct socket *sock, enum sock_shutdown_cmd how);


u32 kernel_sock_ip_overhead(struct sock *sk);

#define MODULE_ALIAS_NETPROTO(proto) \
	MODULE_ALIAS("net-pf-" __stringify(proto))

#define MODULE_ALIAS_NET_PF_PROTO(pf, proto) \
	MODULE_ALIAS("net-pf-" __stringify(pf) "-proto-" __stringify(proto))

#define MODULE_ALIAS_NET_PF_PROTO_TYPE(pf, proto, type) \
	MODULE_ALIAS("net-pf-" __stringify(pf) "-proto-" __stringify(proto) \
		     "-type-" __stringify(type))

#define MODULE_ALIAS_NET_PF_PROTO_NAME(pf, proto, name) \
	MODULE_ALIAS("net-pf-" __stringify(pf) "-proto-" __stringify(proto) \
		     name)
#endif	
