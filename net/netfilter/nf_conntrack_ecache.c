<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* Event cache for netfilter. */

/*
 * (C) 2005 Harald Welte <laforge@gnumonks.org>
 * (C) 2005 Patrick McHardy <kaber@trash.net>
 * (C) 2005-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2005 USAGI/WIDE Project <http://www.linux-ipv6.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/vmalloc.h>
#include <linux/stddef.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <linux/export.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
<<<<<<< HEAD
=======
#include <net/netfilter/nf_conntrack_ecache.h>
>>>>>>> upstream/android-13
#include <net/netfilter/nf_conntrack_extend.h>

static DEFINE_MUTEX(nf_ct_ecache_mutex);

#define ECACHE_RETRY_WAIT (HZ/10)
<<<<<<< HEAD
=======
#define ECACHE_STACK_ALLOC (256 / sizeof(void *))
>>>>>>> upstream/android-13

enum retry_state {
	STATE_CONGESTED,
	STATE_RESTART,
	STATE_DONE,
};

static enum retry_state ecache_work_evict_list(struct ct_pcpu *pcpu)
{
<<<<<<< HEAD
	struct nf_conn *refs[16];
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int evicted = 0;
	enum retry_state ret = STATE_DONE;
=======
	struct nf_conn *refs[ECACHE_STACK_ALLOC];
	enum retry_state ret = STATE_DONE;
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int evicted = 0;
>>>>>>> upstream/android-13

	spin_lock(&pcpu->lock);

	hlist_nulls_for_each_entry(h, n, &pcpu->dying, hnnode) {
		struct nf_conn *ct = nf_ct_tuplehash_to_ctrack(h);
		struct nf_conntrack_ecache *e;

		if (!nf_ct_is_confirmed(ct))
			continue;

<<<<<<< HEAD
=======
		/* This ecache access is safe because the ct is on the
		 * pcpu dying list and we hold the spinlock -- the entry
		 * cannot be free'd until after the lock is released.
		 *
		 * This is true even if ct has a refcount of 0: the
		 * cpu that is about to free the entry must remove it
		 * from the dying list and needs the lock to do so.
		 */
>>>>>>> upstream/android-13
		e = nf_ct_ecache_find(ct);
		if (!e || e->state != NFCT_ECACHE_DESTROY_FAIL)
			continue;

<<<<<<< HEAD
=======
		/* ct is in NFCT_ECACHE_DESTROY_FAIL state, this means
		 * the worker owns this entry: the ct will remain valid
		 * until the worker puts its ct reference.
		 */
>>>>>>> upstream/android-13
		if (nf_conntrack_event(IPCT_DESTROY, ct)) {
			ret = STATE_CONGESTED;
			break;
		}

		e->state = NFCT_ECACHE_DESTROY_SENT;
		refs[evicted] = ct;

		if (++evicted >= ARRAY_SIZE(refs)) {
			ret = STATE_RESTART;
			break;
		}
	}

	spin_unlock(&pcpu->lock);

	/* can't _put while holding lock */
	while (evicted)
		nf_ct_put(refs[--evicted]);

	return ret;
}

static void ecache_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct netns_ct *ctnet =
		container_of(work, struct netns_ct, ecache_dwork.work);
=======
	struct nf_conntrack_net *cnet = container_of(work, struct nf_conntrack_net, ecache_dwork.work);
	struct netns_ct *ctnet = cnet->ct_net;
>>>>>>> upstream/android-13
	int cpu, delay = -1;
	struct ct_pcpu *pcpu;

	local_bh_disable();

	for_each_possible_cpu(cpu) {
		enum retry_state ret;

		pcpu = per_cpu_ptr(ctnet->pcpu_lists, cpu);

		ret = ecache_work_evict_list(pcpu);

		switch (ret) {
		case STATE_CONGESTED:
			delay = ECACHE_RETRY_WAIT;
			goto out;
		case STATE_RESTART:
			delay = 0;
			break;
		case STATE_DONE:
			break;
		}
	}

 out:
	local_bh_enable();

	ctnet->ecache_dwork_pending = delay > 0;
	if (delay >= 0)
<<<<<<< HEAD
		schedule_delayed_work(&ctnet->ecache_dwork, delay);
}

int nf_conntrack_eventmask_report(unsigned int eventmask, struct nf_conn *ct,
				  u32 portid, int report)
{
	int ret = 0;
	struct net *net = nf_ct_net(ct);
	struct nf_ct_event_notifier *notify;
	struct nf_conntrack_ecache *e;

	rcu_read_lock();
	notify = rcu_dereference(net->ct.nf_conntrack_event_cb);
	if (!notify)
		goto out_unlock;

	e = nf_ct_ecache_find(ct);
	if (!e)
		goto out_unlock;

	if (nf_ct_is_confirmed(ct)) {
		struct nf_ct_event item = {
			.ct	= ct,
			.portid	= e->portid ? e->portid : portid,
			.report = report
		};
		/* This is a resent of a destroy event? If so, skip missed */
		unsigned long missed = e->portid ? 0 : e->missed;

		if (!((eventmask | missed) & e->ctmask))
			goto out_unlock;

		ret = notify->fcn(eventmask | missed, &item);
		if (unlikely(ret < 0 || missed)) {
			spin_lock_bh(&ct->lock);
			if (ret < 0) {
				/* This is a destroy event that has been
				 * triggered by a process, we store the PORTID
				 * to include it in the retransmission.
				 */
				if (eventmask & (1 << IPCT_DESTROY)) {
					if (e->portid == 0 && portid != 0)
						e->portid = portid;
					e->state = NFCT_ECACHE_DESTROY_FAIL;
				} else {
					e->missed |= eventmask;
				}
			} else {
				e->missed &= ~missed;
			}
			spin_unlock_bh(&ct->lock);
		}
	}
out_unlock:
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_eventmask_report);

/* deliver cached events and clear cache entry - must be called with locally
 * disabled softirqs */
void nf_ct_deliver_cached_events(struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
	unsigned long events, missed;
	struct nf_ct_event_notifier *notify;
	struct nf_conntrack_ecache *e;
	struct nf_ct_event item;
	int ret;

	rcu_read_lock();
	notify = rcu_dereference(net->ct.nf_conntrack_event_cb);
	if (notify == NULL)
		goto out_unlock;

	e = nf_ct_ecache_find(ct);
	if (e == NULL)
		goto out_unlock;

	events = xchg(&e->cache, 0);

	if (!nf_ct_is_confirmed(ct) || nf_ct_is_dying(ct))
		goto out_unlock;

	/* We make a copy of the missed event cache without taking
	 * the lock, thus we may send missed events twice. However,
	 * this does not harm and it happens very rarely. */
	missed = e->missed;

	if (!((events | missed) & e->ctmask))
		goto out_unlock;

	item.ct = ct;
	item.portid = 0;
	item.report = 0;

	ret = notify->fcn(events | missed, &item);

	if (likely(ret == 0 && !missed))
		goto out_unlock;
=======
		schedule_delayed_work(&cnet->ecache_dwork, delay);
}

static int __nf_conntrack_eventmask_report(struct nf_conntrack_ecache *e,
					   const unsigned int events,
					   const unsigned long missed,
					   const struct nf_ct_event *item)
{
	struct nf_conn *ct = item->ct;
	struct net *net = nf_ct_net(item->ct);
	struct nf_ct_event_notifier *notify;
	int ret;

	if (!((events | missed) & e->ctmask))
		return 0;

	rcu_read_lock();

	notify = rcu_dereference(net->ct.nf_conntrack_event_cb);
	if (!notify) {
		rcu_read_unlock();
		return 0;
	}

	ret = notify->ct_event(events | missed, item);
	rcu_read_unlock();

	if (likely(ret >= 0 && missed == 0))
		return 0;
>>>>>>> upstream/android-13

	spin_lock_bh(&ct->lock);
	if (ret < 0)
		e->missed |= events;
	else
		e->missed &= ~missed;
	spin_unlock_bh(&ct->lock);

<<<<<<< HEAD
out_unlock:
	rcu_read_unlock();
=======
	return ret;
}

int nf_conntrack_eventmask_report(unsigned int events, struct nf_conn *ct,
				  u32 portid, int report)
{
	struct nf_conntrack_ecache *e;
	struct nf_ct_event item;
	unsigned long missed;
	int ret;

	if (!nf_ct_is_confirmed(ct))
		return 0;

	e = nf_ct_ecache_find(ct);
	if (!e)
		return 0;

	memset(&item, 0, sizeof(item));

	item.ct = ct;
	item.portid = e->portid ? e->portid : portid;
	item.report = report;

	/* This is a resent of a destroy event? If so, skip missed */
	missed = e->portid ? 0 : e->missed;

	ret = __nf_conntrack_eventmask_report(e, events, missed, &item);
	if (unlikely(ret < 0 && (events & (1 << IPCT_DESTROY)))) {
		/* This is a destroy event that has been triggered by a process,
		 * we store the PORTID to include it in the retransmission.
		 */
		if (e->portid == 0 && portid != 0)
			e->portid = portid;
		e->state = NFCT_ECACHE_DESTROY_FAIL;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_eventmask_report);

/* deliver cached events and clear cache entry - must be called with locally
 * disabled softirqs */
void nf_ct_deliver_cached_events(struct nf_conn *ct)
{
	struct nf_conntrack_ecache *e;
	struct nf_ct_event item;
	unsigned long events;

	if (!nf_ct_is_confirmed(ct) || nf_ct_is_dying(ct))
		return;

	e = nf_ct_ecache_find(ct);
	if (e == NULL)
		return;

	events = xchg(&e->cache, 0);

	item.ct = ct;
	item.portid = 0;
	item.report = 0;

	/* We make a copy of the missed event cache without taking
	 * the lock, thus we may send missed events twice. However,
	 * this does not harm and it happens very rarely.
	 */
	__nf_conntrack_eventmask_report(e, events, e->missed, &item);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(nf_ct_deliver_cached_events);

void nf_ct_expect_event_report(enum ip_conntrack_expect_events event,
			       struct nf_conntrack_expect *exp,
			       u32 portid, int report)

{
	struct net *net = nf_ct_exp_net(exp);
<<<<<<< HEAD
	struct nf_exp_event_notifier *notify;
	struct nf_conntrack_ecache *e;

	rcu_read_lock();
	notify = rcu_dereference(net->ct.nf_expect_event_cb);
=======
	struct nf_ct_event_notifier *notify;
	struct nf_conntrack_ecache *e;

	rcu_read_lock();
	notify = rcu_dereference(net->ct.nf_conntrack_event_cb);
>>>>>>> upstream/android-13
	if (!notify)
		goto out_unlock;

	e = nf_ct_ecache_find(exp->master);
	if (!e)
		goto out_unlock;

	if (e->expmask & (1 << event)) {
		struct nf_exp_event item = {
			.exp	= exp,
			.portid	= portid,
			.report = report
		};
<<<<<<< HEAD
		notify->fcn(1 << event, &item);
=======
		notify->exp_event(1 << event, &item);
>>>>>>> upstream/android-13
	}
out_unlock:
	rcu_read_unlock();
}

<<<<<<< HEAD
int nf_conntrack_register_notifier(struct net *net,
				   struct nf_ct_event_notifier *new)
{
	int ret;
=======
void nf_conntrack_register_notifier(struct net *net,
				    const struct nf_ct_event_notifier *new)
{
>>>>>>> upstream/android-13
	struct nf_ct_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
	notify = rcu_dereference_protected(net->ct.nf_conntrack_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
<<<<<<< HEAD
	if (notify != NULL) {
		ret = -EBUSY;
		goto out_unlock;
	}
	rcu_assign_pointer(net->ct.nf_conntrack_event_cb, new);
	ret = 0;

out_unlock:
	mutex_unlock(&nf_ct_ecache_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_register_notifier);

void nf_conntrack_unregister_notifier(struct net *net,
				      struct nf_ct_event_notifier *new)
{
	struct nf_ct_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
	notify = rcu_dereference_protected(net->ct.nf_conntrack_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	BUG_ON(notify != new);
	RCU_INIT_POINTER(net->ct.nf_conntrack_event_cb, NULL);
	mutex_unlock(&nf_ct_ecache_mutex);
	/* synchronize_rcu() is called from ctnetlink_exit. */
}
EXPORT_SYMBOL_GPL(nf_conntrack_unregister_notifier);

int nf_ct_expect_register_notifier(struct net *net,
				   struct nf_exp_event_notifier *new)
{
	int ret;
	struct nf_exp_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
	notify = rcu_dereference_protected(net->ct.nf_expect_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	if (notify != NULL) {
		ret = -EBUSY;
		goto out_unlock;
	}
	rcu_assign_pointer(net->ct.nf_expect_event_cb, new);
	ret = 0;

out_unlock:
	mutex_unlock(&nf_ct_ecache_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_expect_register_notifier);

void nf_ct_expect_unregister_notifier(struct net *net,
				      struct nf_exp_event_notifier *new)
{
	struct nf_exp_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
	notify = rcu_dereference_protected(net->ct.nf_expect_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	BUG_ON(notify != new);
	RCU_INIT_POINTER(net->ct.nf_expect_event_cb, NULL);
	mutex_unlock(&nf_ct_ecache_mutex);
	/* synchronize_rcu() is called from ctnetlink_exit. */
}
EXPORT_SYMBOL_GPL(nf_ct_expect_unregister_notifier);
=======
	WARN_ON_ONCE(notify);
	rcu_assign_pointer(net->ct.nf_conntrack_event_cb, new);
	mutex_unlock(&nf_ct_ecache_mutex);
}
EXPORT_SYMBOL_GPL(nf_conntrack_register_notifier);

void nf_conntrack_unregister_notifier(struct net *net)
{
	mutex_lock(&nf_ct_ecache_mutex);
	RCU_INIT_POINTER(net->ct.nf_conntrack_event_cb, NULL);
	mutex_unlock(&nf_ct_ecache_mutex);
	/* synchronize_rcu() is called after netns pre_exit */
}
EXPORT_SYMBOL_GPL(nf_conntrack_unregister_notifier);

void nf_conntrack_ecache_work(struct net *net, enum nf_ct_ecache_state state)
{
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);

	if (state == NFCT_ECACHE_DESTROY_FAIL &&
	    !delayed_work_pending(&cnet->ecache_dwork)) {
		schedule_delayed_work(&cnet->ecache_dwork, HZ);
		net->ct.ecache_dwork_pending = true;
	} else if (state == NFCT_ECACHE_DESTROY_SENT) {
		net->ct.ecache_dwork_pending = false;
		mod_delayed_work(system_wq, &cnet->ecache_dwork, 0);
	}
}
>>>>>>> upstream/android-13

#define NF_CT_EVENTS_DEFAULT 1
static int nf_ct_events __read_mostly = NF_CT_EVENTS_DEFAULT;

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
static struct ctl_table event_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_events",
		.data		= &init_net.ct.sysctl_events,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{}
};
#endif /* CONFIG_SYSCTL */

=======
>>>>>>> upstream/android-13
static const struct nf_ct_ext_type event_extend = {
	.len	= sizeof(struct nf_conntrack_ecache),
	.align	= __alignof__(struct nf_conntrack_ecache),
	.id	= NF_CT_EXT_ECACHE,
};

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
static int nf_conntrack_event_init_sysctl(struct net *net)
{
	struct ctl_table *table;

	table = kmemdup(event_sysctl_table, sizeof(event_sysctl_table),
			GFP_KERNEL);
	if (!table)
		goto out;

	table[0].data = &net->ct.sysctl_events;

	/* Don't export sysctls to unprivileged users */
	if (net->user_ns != &init_user_ns)
		table[0].procname = NULL;

	net->ct.event_sysctl_header =
		register_net_sysctl(net, "net/netfilter", table);
	if (!net->ct.event_sysctl_header) {
		pr_err("can't register to sysctl\n");
		goto out_register;
	}
	return 0;

out_register:
	kfree(table);
out:
	return -ENOMEM;
}

static void nf_conntrack_event_fini_sysctl(struct net *net)
{
	struct ctl_table *table;

	table = net->ct.event_sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(net->ct.event_sysctl_header);
	kfree(table);
}
#else
static int nf_conntrack_event_init_sysctl(struct net *net)
{
	return 0;
}

static void nf_conntrack_event_fini_sysctl(struct net *net)
{
}
#endif /* CONFIG_SYSCTL */

int nf_conntrack_ecache_pernet_init(struct net *net)
{
	net->ct.sysctl_events = nf_ct_events;
	INIT_DELAYED_WORK(&net->ct.ecache_dwork, ecache_work);
	return nf_conntrack_event_init_sysctl(net);
=======
void nf_conntrack_ecache_pernet_init(struct net *net)
{
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);

	net->ct.sysctl_events = nf_ct_events;
	cnet->ct_net = &net->ct;
	INIT_DELAYED_WORK(&cnet->ecache_dwork, ecache_work);
>>>>>>> upstream/android-13
}

void nf_conntrack_ecache_pernet_fini(struct net *net)
{
<<<<<<< HEAD
	cancel_delayed_work_sync(&net->ct.ecache_dwork);
	nf_conntrack_event_fini_sysctl(net);
=======
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);

	cancel_delayed_work_sync(&cnet->ecache_dwork);
>>>>>>> upstream/android-13
}

int nf_conntrack_ecache_init(void)
{
	int ret = nf_ct_extend_register(&event_extend);
	if (ret < 0)
		pr_err("Unable to register event extension\n");

	BUILD_BUG_ON(__IPCT_MAX >= 16);	/* ctmask, missed use u16 */

	return ret;
}

void nf_conntrack_ecache_fini(void)
{
	nf_ct_extend_unregister(&event_extend);
}
