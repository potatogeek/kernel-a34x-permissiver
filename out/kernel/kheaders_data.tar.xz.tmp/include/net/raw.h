
#ifndef _RAW_H
#define _RAW_H


#include <net/protocol.h>
#include <linux/icmp.h>

extern struct proto raw_prot;

extern struct raw_hashinfo raw_v4_hashinfo;
struct sock *__raw_v4_lookup(struct net *net, struct sock *sk,
			     unsigned short num, __be32 raddr,
			     __be32 laddr, int dif, int sdif);

int raw_abort(struct sock *sk, int err);
void raw_icmp_error(struct sk_buff *, int, u32);
int raw_local_deliver(struct sk_buff *, int);

int raw_rcv(struct sock *, struct sk_buff *);

#define RAW_HTABLE_SIZE	MAX_INET_PROTOS

struct raw_hashinfo {
	rwlock_t lock;
	struct hlist_head ht[RAW_HTABLE_SIZE];
};

#ifdef CONFIG_PROC_FS
int raw_proc_init(void);
void raw_proc_exit(void);

struct raw_iter_state {
	struct seq_net_private p;
	int bucket;
};

static inline struct raw_iter_state *raw_seq_private(struct seq_file *seq)
{
	return seq->private;
}
void *raw_seq_start(struct seq_file *seq, loff_t *pos);
void *raw_seq_next(struct seq_file *seq, void *v, loff_t *pos);
void raw_seq_stop(struct seq_file *seq, void *v);
#endif

int raw_hash_sk(struct sock *sk);
void raw_unhash_sk(struct sock *sk);

struct raw_sock {
	
	struct inet_sock   inet;
	struct icmp_filter filter;
	u32		   ipmr_table;
};

static inline struct raw_sock *raw_sk(const struct sock *sk)
{
	return (struct raw_sock *)sk;
}

#endif	
