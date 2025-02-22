#include <linux/rtnetlink.h>
#include <linux/notifier.h>
#include <linux/rcupdate.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <net/net_namespace.h>
<<<<<<< HEAD
#include <net/fib_notifier.h>

static ATOMIC_NOTIFIER_HEAD(fib_chain);

int call_fib_notifier(struct notifier_block *nb, struct net *net,
=======
#include <net/netns/generic.h>
#include <net/fib_notifier.h>

static unsigned int fib_notifier_net_id;

struct fib_notifier_net {
	struct list_head fib_notifier_ops;
	struct atomic_notifier_head fib_chain;
};

int call_fib_notifier(struct notifier_block *nb,
>>>>>>> upstream/android-13
		      enum fib_event_type event_type,
		      struct fib_notifier_info *info)
{
	int err;

<<<<<<< HEAD
	info->net = net;
=======
>>>>>>> upstream/android-13
	err = nb->notifier_call(nb, event_type, info);
	return notifier_to_errno(err);
}
EXPORT_SYMBOL(call_fib_notifier);

int call_fib_notifiers(struct net *net, enum fib_event_type event_type,
		       struct fib_notifier_info *info)
{
<<<<<<< HEAD
	int err;

	info->net = net;
	err = atomic_notifier_call_chain(&fib_chain, event_type, info);
=======
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);
	int err;

	err = atomic_notifier_call_chain(&fn_net->fib_chain, event_type, info);
>>>>>>> upstream/android-13
	return notifier_to_errno(err);
}
EXPORT_SYMBOL(call_fib_notifiers);

<<<<<<< HEAD
static unsigned int fib_seq_sum(void)
{
	struct fib_notifier_ops *ops;
	unsigned int fib_seq = 0;
	struct net *net;

	rtnl_lock();
	down_read(&net_rwsem);
	for_each_net(net) {
		rcu_read_lock();
		list_for_each_entry_rcu(ops, &net->fib_notifier_ops, list) {
			if (!try_module_get(ops->owner))
				continue;
			fib_seq += ops->fib_seq_read(net);
			module_put(ops->owner);
		}
		rcu_read_unlock();
	}
	up_read(&net_rwsem);
=======
static unsigned int fib_seq_sum(struct net *net)
{
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);
	struct fib_notifier_ops *ops;
	unsigned int fib_seq = 0;

	rtnl_lock();
	rcu_read_lock();
	list_for_each_entry_rcu(ops, &fn_net->fib_notifier_ops, list) {
		if (!try_module_get(ops->owner))
			continue;
		fib_seq += ops->fib_seq_read(net);
		module_put(ops->owner);
	}
	rcu_read_unlock();
>>>>>>> upstream/android-13
	rtnl_unlock();

	return fib_seq;
}

<<<<<<< HEAD
static int fib_net_dump(struct net *net, struct notifier_block *nb)
{
	struct fib_notifier_ops *ops;

	list_for_each_entry_rcu(ops, &net->fib_notifier_ops, list) {
		int err;

		if (!try_module_get(ops->owner))
			continue;
		err = ops->fib_dump(net, nb);
		module_put(ops->owner);
		if (err)
			return err;
	}

	return 0;
}

static bool fib_dump_is_consistent(struct notifier_block *nb,
				   void (*cb)(struct notifier_block *nb),
				   unsigned int fib_seq)
{
	atomic_notifier_chain_register(&fib_chain, nb);
	if (fib_seq == fib_seq_sum())
		return true;
	atomic_notifier_chain_unregister(&fib_chain, nb);
=======
static int fib_net_dump(struct net *net, struct notifier_block *nb,
			struct netlink_ext_ack *extack)
{
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);
	struct fib_notifier_ops *ops;
	int err = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(ops, &fn_net->fib_notifier_ops, list) {
		if (!try_module_get(ops->owner))
			continue;
		err = ops->fib_dump(net, nb, extack);
		module_put(ops->owner);
		if (err)
			goto unlock;
	}

unlock:
	rcu_read_unlock();

	return err;
}

static bool fib_dump_is_consistent(struct net *net, struct notifier_block *nb,
				   void (*cb)(struct notifier_block *nb),
				   unsigned int fib_seq)
{
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);

	atomic_notifier_chain_register(&fn_net->fib_chain, nb);
	if (fib_seq == fib_seq_sum(net))
		return true;
	atomic_notifier_chain_unregister(&fn_net->fib_chain, nb);
>>>>>>> upstream/android-13
	if (cb)
		cb(nb);
	return false;
}

#define FIB_DUMP_MAX_RETRIES 5
<<<<<<< HEAD
int register_fib_notifier(struct notifier_block *nb,
			  void (*cb)(struct notifier_block *nb))
=======
int register_fib_notifier(struct net *net, struct notifier_block *nb,
			  void (*cb)(struct notifier_block *nb),
			  struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	int retries = 0;
	int err;

	do {
<<<<<<< HEAD
		unsigned int fib_seq = fib_seq_sum();
		struct net *net;

		rcu_read_lock();
		for_each_net_rcu(net) {
			err = fib_net_dump(net, nb);
			if (err)
				goto err_fib_net_dump;
		}
		rcu_read_unlock();

		if (fib_dump_is_consistent(nb, cb, fib_seq))
=======
		unsigned int fib_seq = fib_seq_sum(net);

		err = fib_net_dump(net, nb, extack);
		if (err)
			return err;

		if (fib_dump_is_consistent(net, nb, cb, fib_seq))
>>>>>>> upstream/android-13
			return 0;
	} while (++retries < FIB_DUMP_MAX_RETRIES);

	return -EBUSY;
<<<<<<< HEAD

err_fib_net_dump:
	rcu_read_unlock();
	return err;
}
EXPORT_SYMBOL(register_fib_notifier);

int unregister_fib_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&fib_chain, nb);
=======
}
EXPORT_SYMBOL(register_fib_notifier);

int unregister_fib_notifier(struct net *net, struct notifier_block *nb)
{
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);

	return atomic_notifier_chain_unregister(&fn_net->fib_chain, nb);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(unregister_fib_notifier);

static int __fib_notifier_ops_register(struct fib_notifier_ops *ops,
				       struct net *net)
{
<<<<<<< HEAD
	struct fib_notifier_ops *o;

	list_for_each_entry(o, &net->fib_notifier_ops, list)
		if (ops->family == o->family)
			return -EEXIST;
	list_add_tail_rcu(&ops->list, &net->fib_notifier_ops);
=======
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);
	struct fib_notifier_ops *o;

	list_for_each_entry(o, &fn_net->fib_notifier_ops, list)
		if (ops->family == o->family)
			return -EEXIST;
	list_add_tail_rcu(&ops->list, &fn_net->fib_notifier_ops);
>>>>>>> upstream/android-13
	return 0;
}

struct fib_notifier_ops *
fib_notifier_ops_register(const struct fib_notifier_ops *tmpl, struct net *net)
{
	struct fib_notifier_ops *ops;
	int err;

	ops = kmemdup(tmpl, sizeof(*ops), GFP_KERNEL);
	if (!ops)
		return ERR_PTR(-ENOMEM);

	err = __fib_notifier_ops_register(ops, net);
	if (err)
		goto err_register;

	return ops;

err_register:
	kfree(ops);
	return ERR_PTR(err);
}
EXPORT_SYMBOL(fib_notifier_ops_register);

void fib_notifier_ops_unregister(struct fib_notifier_ops *ops)
{
	list_del_rcu(&ops->list);
	kfree_rcu(ops, rcu);
}
EXPORT_SYMBOL(fib_notifier_ops_unregister);

static int __net_init fib_notifier_net_init(struct net *net)
{
<<<<<<< HEAD
	INIT_LIST_HEAD(&net->fib_notifier_ops);
=======
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);

	INIT_LIST_HEAD(&fn_net->fib_notifier_ops);
	ATOMIC_INIT_NOTIFIER_HEAD(&fn_net->fib_chain);
>>>>>>> upstream/android-13
	return 0;
}

static void __net_exit fib_notifier_net_exit(struct net *net)
{
<<<<<<< HEAD
	WARN_ON_ONCE(!list_empty(&net->fib_notifier_ops));
=======
	struct fib_notifier_net *fn_net = net_generic(net, fib_notifier_net_id);

	WARN_ON_ONCE(!list_empty(&fn_net->fib_notifier_ops));
>>>>>>> upstream/android-13
}

static struct pernet_operations fib_notifier_net_ops = {
	.init = fib_notifier_net_init,
	.exit = fib_notifier_net_exit,
<<<<<<< HEAD
=======
	.id = &fib_notifier_net_id,
	.size = sizeof(struct fib_notifier_net),
>>>>>>> upstream/android-13
};

static int __init fib_notifier_init(void)
{
	return register_pernet_subsys(&fib_notifier_net_ops);
}

subsys_initcall(fib_notifier_init);
