<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Pluggable TCP upper layer protocol support.
 *
 * Copyright (c) 2016-2017, Mellanox Technologies. All rights reserved.
 * Copyright (c) 2016-2017, Dave Watson <davejwatson@fb.com>. All rights reserved.
 *
 */

<<<<<<< HEAD
#include<linux/module.h>
=======
#include <linux/module.h>
>>>>>>> upstream/android-13
#include <linux/mm.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/gfp.h>
#include <net/tcp.h>

static DEFINE_SPINLOCK(tcp_ulp_list_lock);
static LIST_HEAD(tcp_ulp_list);

/* Simple linear search, don't expect many entries! */
static struct tcp_ulp_ops *tcp_ulp_find(const char *name)
{
	struct tcp_ulp_ops *e;

<<<<<<< HEAD
	list_for_each_entry_rcu(e, &tcp_ulp_list, list) {
=======
	list_for_each_entry_rcu(e, &tcp_ulp_list, list,
				lockdep_is_held(&tcp_ulp_list_lock)) {
>>>>>>> upstream/android-13
		if (strcmp(e->name, name) == 0)
			return e;
	}

	return NULL;
}

<<<<<<< HEAD
static struct tcp_ulp_ops *tcp_ulp_find_id(const int ulp)
{
	struct tcp_ulp_ops *e;

	list_for_each_entry_rcu(e, &tcp_ulp_list, list) {
		if (e->uid == ulp)
			return e;
	}

	return NULL;
}

=======
>>>>>>> upstream/android-13
static const struct tcp_ulp_ops *__tcp_ulp_find_autoload(const char *name)
{
	const struct tcp_ulp_ops *ulp = NULL;

	rcu_read_lock();
	ulp = tcp_ulp_find(name);

#ifdef CONFIG_MODULES
	if (!ulp && capable(CAP_NET_ADMIN)) {
		rcu_read_unlock();
		request_module("tcp-ulp-%s", name);
		rcu_read_lock();
		ulp = tcp_ulp_find(name);
	}
#endif
	if (!ulp || !try_module_get(ulp->owner))
		ulp = NULL;

	rcu_read_unlock();
	return ulp;
}

<<<<<<< HEAD
static const struct tcp_ulp_ops *__tcp_ulp_lookup(const int uid)
{
	const struct tcp_ulp_ops *ulp;

	rcu_read_lock();
	ulp = tcp_ulp_find_id(uid);
	if (!ulp || !try_module_get(ulp->owner))
		ulp = NULL;
	rcu_read_unlock();
	return ulp;
}

=======
>>>>>>> upstream/android-13
/* Attach new upper layer protocol to the list
 * of available protocols.
 */
int tcp_register_ulp(struct tcp_ulp_ops *ulp)
{
	int ret = 0;

	spin_lock(&tcp_ulp_list_lock);
	if (tcp_ulp_find(ulp->name))
		ret = -EEXIST;
	else
		list_add_tail_rcu(&ulp->list, &tcp_ulp_list);
	spin_unlock(&tcp_ulp_list_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(tcp_register_ulp);

void tcp_unregister_ulp(struct tcp_ulp_ops *ulp)
{
	spin_lock(&tcp_ulp_list_lock);
	list_del_rcu(&ulp->list);
	spin_unlock(&tcp_ulp_list_lock);

	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(tcp_unregister_ulp);

/* Build string with list of available upper layer protocl values */
void tcp_get_available_ulp(char *buf, size_t maxlen)
{
	struct tcp_ulp_ops *ulp_ops;
	size_t offs = 0;

	*buf = '\0';
	rcu_read_lock();
	list_for_each_entry_rcu(ulp_ops, &tcp_ulp_list, list) {
		offs += snprintf(buf + offs, maxlen - offs,
				 "%s%s",
				 offs == 0 ? "" : " ", ulp_ops->name);
<<<<<<< HEAD
=======

		if (WARN_ON_ONCE(offs >= maxlen))
			break;
>>>>>>> upstream/android-13
	}
	rcu_read_unlock();
}

<<<<<<< HEAD
=======
void tcp_update_ulp(struct sock *sk, struct proto *proto,
		    void (*write_space)(struct sock *sk))
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_ulp_ops->update)
		icsk->icsk_ulp_ops->update(sk, proto, write_space);
}

>>>>>>> upstream/android-13
void tcp_cleanup_ulp(struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

<<<<<<< HEAD
=======
	/* No sock_owned_by_me() check here as at the time the
	 * stack calls this function, the socket is dead and
	 * about to be destroyed.
	 */
>>>>>>> upstream/android-13
	if (!icsk->icsk_ulp_ops)
		return;

	if (icsk->icsk_ulp_ops->release)
		icsk->icsk_ulp_ops->release(sk);
	module_put(icsk->icsk_ulp_ops->owner);

	icsk->icsk_ulp_ops = NULL;
}

<<<<<<< HEAD
/* Change upper layer protocol for socket */
int tcp_set_ulp(struct sock *sk, const char *name)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	const struct tcp_ulp_ops *ulp_ops;
	int err = 0;

	if (icsk->icsk_ulp_ops)
		return -EEXIST;
=======
static int __tcp_set_ulp(struct sock *sk, const struct tcp_ulp_ops *ulp_ops)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	int err;

	err = -EEXIST;
	if (icsk->icsk_ulp_ops)
		goto out_err;

	err = ulp_ops->init(sk);
	if (err)
		goto out_err;

	icsk->icsk_ulp_ops = ulp_ops;
	return 0;
out_err:
	module_put(ulp_ops->owner);
	return err;
}

int tcp_set_ulp(struct sock *sk, const char *name)
{
	const struct tcp_ulp_ops *ulp_ops;

	sock_owned_by_me(sk);
>>>>>>> upstream/android-13

	ulp_ops = __tcp_ulp_find_autoload(name);
	if (!ulp_ops)
		return -ENOENT;

<<<<<<< HEAD
	if (!ulp_ops->user_visible) {
		module_put(ulp_ops->owner);
		return -ENOENT;
	}

	err = ulp_ops->init(sk);
	if (err) {
		module_put(ulp_ops->owner);
		return err;
	}

	icsk->icsk_ulp_ops = ulp_ops;
	return 0;
}

int tcp_set_ulp_id(struct sock *sk, int ulp)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	const struct tcp_ulp_ops *ulp_ops;
	int err;

	if (icsk->icsk_ulp_ops)
		return -EEXIST;

	ulp_ops = __tcp_ulp_lookup(ulp);
	if (!ulp_ops)
		return -ENOENT;

	err = ulp_ops->init(sk);
	if (err) {
		module_put(ulp_ops->owner);
		return err;
	}

	icsk->icsk_ulp_ops = ulp_ops;
	return 0;
=======
	return __tcp_set_ulp(sk, ulp_ops);
>>>>>>> upstream/android-13
}
