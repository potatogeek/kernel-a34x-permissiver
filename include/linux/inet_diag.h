/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _INET_DIAG_H_
#define _INET_DIAG_H_ 1

#include <net/netlink.h>
#include <uapi/linux/inet_diag.h>

struct inet_hashinfo;

struct inet_diag_handler {
	void		(*dump)(struct sk_buff *skb,
				struct netlink_callback *cb,
<<<<<<< HEAD
				const struct inet_diag_req_v2 *r,
				struct nlattr *bc);

	int		(*dump_one)(struct sk_buff *in_skb,
				    const struct nlmsghdr *nlh,
=======
				const struct inet_diag_req_v2 *r);

	int		(*dump_one)(struct netlink_callback *cb,
>>>>>>> upstream/android-13
				    const struct inet_diag_req_v2 *req);

	void		(*idiag_get_info)(struct sock *sk,
					  struct inet_diag_msg *r,
					  void *info);

	int		(*idiag_get_aux)(struct sock *sk,
					 bool net_admin,
					 struct sk_buff *skb);

	size_t		(*idiag_get_aux_size)(struct sock *sk,
					      bool net_admin);

	int		(*destroy)(struct sk_buff *in_skb,
				   const struct inet_diag_req_v2 *req);

	__u16		idiag_type;
	__u16		idiag_info_size;
};

<<<<<<< HEAD
struct inet_connection_sock;
int inet_sk_diag_fill(struct sock *sk, struct inet_connection_sock *icsk,
		      struct sk_buff *skb, const struct inet_diag_req_v2 *req,
		      struct user_namespace *user_ns,
		      u32 pid, u32 seq, u16 nlmsg_flags,
		      const struct nlmsghdr *unlh, bool net_admin);
void inet_diag_dump_icsk(struct inet_hashinfo *h, struct sk_buff *skb,
			 struct netlink_callback *cb,
			 const struct inet_diag_req_v2 *r,
			 struct nlattr *bc);
int inet_diag_dump_one_icsk(struct inet_hashinfo *hashinfo,
			    struct sk_buff *in_skb, const struct nlmsghdr *nlh,
=======
struct bpf_sk_storage_diag;
struct inet_diag_dump_data {
	struct nlattr *req_nlas[__INET_DIAG_REQ_MAX];
#define inet_diag_nla_bc req_nlas[INET_DIAG_REQ_BYTECODE]
#define inet_diag_nla_bpf_stgs req_nlas[INET_DIAG_REQ_SK_BPF_STORAGES]

	struct bpf_sk_storage_diag *bpf_stg_diag;
};

struct inet_connection_sock;
int inet_sk_diag_fill(struct sock *sk, struct inet_connection_sock *icsk,
		      struct sk_buff *skb, struct netlink_callback *cb,
		      const struct inet_diag_req_v2 *req,
		      u16 nlmsg_flags, bool net_admin);
void inet_diag_dump_icsk(struct inet_hashinfo *h, struct sk_buff *skb,
			 struct netlink_callback *cb,
			 const struct inet_diag_req_v2 *r);
int inet_diag_dump_one_icsk(struct inet_hashinfo *hashinfo,
			    struct netlink_callback *cb,
>>>>>>> upstream/android-13
			    const struct inet_diag_req_v2 *req);

struct sock *inet_diag_find_one_icsk(struct net *net,
				     struct inet_hashinfo *hashinfo,
				     const struct inet_diag_req_v2 *req);

int inet_diag_bc_sk(const struct nlattr *_bc, struct sock *sk);

void inet_diag_msg_common_fill(struct inet_diag_msg *r, struct sock *sk);

static inline size_t inet_diag_msg_attrs_size(void)
{
	return	  nla_total_size(1)  /* INET_DIAG_SHUTDOWN */
		+ nla_total_size(1)  /* INET_DIAG_TOS */
#if IS_ENABLED(CONFIG_IPV6)
		+ nla_total_size(1)  /* INET_DIAG_TCLASS */
		+ nla_total_size(1)  /* INET_DIAG_SKV6ONLY */
#endif
		+ nla_total_size(4)  /* INET_DIAG_MARK */
<<<<<<< HEAD
		+ nla_total_size(4); /* INET_DIAG_CLASS_ID */
=======
		+ nla_total_size(4)  /* INET_DIAG_CLASS_ID */
#ifdef CONFIG_SOCK_CGROUP_DATA
		+ nla_total_size_64bit(sizeof(u64))  /* INET_DIAG_CGROUP_ID */
#endif
		+ nla_total_size(sizeof(struct inet_diag_sockopt))
						     /* INET_DIAG_SOCKOPT */
		;
>>>>>>> upstream/android-13
}
int inet_diag_msg_attrs_fill(struct sock *sk, struct sk_buff *skb,
			     struct inet_diag_msg *r, int ext,
			     struct user_namespace *user_ns, bool net_admin);

extern int  inet_diag_register(const struct inet_diag_handler *handler);
extern void inet_diag_unregister(const struct inet_diag_handler *handler);
#endif /* _INET_DIAG_H_ */
