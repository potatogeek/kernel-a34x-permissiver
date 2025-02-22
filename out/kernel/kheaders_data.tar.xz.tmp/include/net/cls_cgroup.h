

#ifndef _NET_CLS_CGROUP_H
#define _NET_CLS_CGROUP_H

#include <linux/cgroup.h>
#include <linux/hardirq.h>
#include <linux/rcupdate.h>
#include <net/sock.h>
#include <net/inet_sock.h>

#ifdef CONFIG_CGROUP_NET_CLASSID
struct cgroup_cls_state {
	struct cgroup_subsys_state css;
	u32 classid;
};

struct cgroup_cls_state *task_cls_state(struct task_struct *p);

static inline u32 task_cls_classid(struct task_struct *p)
{
	u32 classid;

	if (in_interrupt())
		return 0;

	rcu_read_lock();
	classid = container_of(task_css(p, net_cls_cgrp_id),
			       struct cgroup_cls_state, css)->classid;
	rcu_read_unlock();

	return classid;
}

static inline void sock_update_classid(struct sock_cgroup_data *skcd)
{
	u32 classid;

	classid = task_cls_classid(current);
	sock_cgroup_set_classid(skcd, classid);
}

static inline u32 task_get_classid(const struct sk_buff *skb)
{
	u32 classid = task_cls_state(current)->classid;

	
	if (in_serving_softirq()) {
		struct sock *sk = skb_to_full_sk(skb);

		
		if (!sk || !sk_fullsock(sk))
			return 0;

		classid = sock_cgroup_classid(&sk->sk_cgrp_data);
	}

	return classid;
}
#else 
static inline void sock_update_classid(struct sock_cgroup_data *skcd)
{
}

static inline u32 task_get_classid(const struct sk_buff *skb)
{
	return 0;
}
#endif 
#endif  
