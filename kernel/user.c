<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * The "user cache".
 *
 * (C) Copyright 1991-2000 Linus Torvalds
 *
 * We have a per-user structure to keep track of how many
 * processes, files etc the user has claimed, in order to be
 * able to have per-user limits for system resources. 
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/key.h>
#include <linux/sched/user.h>
#include <linux/interrupt.h>
#include <linux/export.h>
#include <linux/user_namespace.h>
<<<<<<< HEAD
#include <linux/proc_fs.h>
#include <linux/proc_ns.h>

=======
#include <linux/proc_ns.h>

#include <trace/hooks/user.h>

>>>>>>> upstream/android-13
/*
 * userns count is 1 for root user, 1 for init_uts_ns,
 * and 1 for... ?
 */
struct user_namespace init_user_ns = {
	.uid_map = {
		.nr_extents = 1,
		{
			.extent[0] = {
				.first = 0,
				.lower_first = 0,
				.count = 4294967295U,
			},
		},
	},
	.gid_map = {
		.nr_extents = 1,
		{
			.extent[0] = {
				.first = 0,
				.lower_first = 0,
				.count = 4294967295U,
			},
		},
	},
	.projid_map = {
		.nr_extents = 1,
		{
			.extent[0] = {
				.first = 0,
				.lower_first = 0,
				.count = 4294967295U,
			},
		},
	},
<<<<<<< HEAD
	.count = ATOMIC_INIT(3),
=======
	.ns.count = REFCOUNT_INIT(3),
>>>>>>> upstream/android-13
	.owner = GLOBAL_ROOT_UID,
	.group = GLOBAL_ROOT_GID,
	.ns.inum = PROC_USER_INIT_INO,
#ifdef CONFIG_USER_NS
	.ns.ops = &userns_operations,
#endif
	.flags = USERNS_INIT_FLAGS,
<<<<<<< HEAD
#ifdef CONFIG_PERSISTENT_KEYRINGS
	.persistent_keyring_register_sem =
	__RWSEM_INITIALIZER(init_user_ns.persistent_keyring_register_sem),
=======
#ifdef CONFIG_KEYS
	.keyring_name_list = LIST_HEAD_INIT(init_user_ns.keyring_name_list),
	.keyring_sem = __RWSEM_INITIALIZER(init_user_ns.keyring_sem),
>>>>>>> upstream/android-13
#endif
};
EXPORT_SYMBOL_GPL(init_user_ns);

/*
 * UID task count cache, to get fast user lookup in "alloc_uid"
 * when changing user ID's (ie setuid() and friends).
 */

#define UIDHASH_BITS	(CONFIG_BASE_SMALL ? 3 : 7)
#define UIDHASH_SZ	(1 << UIDHASH_BITS)
#define UIDHASH_MASK		(UIDHASH_SZ - 1)
#define __uidhashfn(uid)	(((uid >> UIDHASH_BITS) + uid) & UIDHASH_MASK)
#define uidhashentry(uid)	(uidhash_table + __uidhashfn((__kuid_val(uid))))

static struct kmem_cache *uid_cachep;
<<<<<<< HEAD
struct hlist_head uidhash_table[UIDHASH_SZ];
=======
static struct hlist_head uidhash_table[UIDHASH_SZ];
>>>>>>> upstream/android-13

/*
 * The uidhash_lock is mostly taken from process context, but it is
 * occasionally also taken from softirq/tasklet context, when
 * task-structs get RCU-freed. Hence all locking must be softirq-safe.
 * But free_uid() is also called with local interrupts disabled, and running
 * local_bh_enable() with local interrupts disabled is an error - we'll run
 * softirq callbacks, and they can unconditionally enable interrupts, and
 * the caller of free_uid() didn't expect that..
 */
static DEFINE_SPINLOCK(uidhash_lock);

/* root_user.__count is 1, for init task cred */
<<<<<<< HEAD
struct user_struct root_user = {
	.__count	= REFCOUNT_INIT(1),
	.processes	= ATOMIC_INIT(1),
	.sigpending	= ATOMIC_INIT(0),
	.locked_shm     = 0,
	.uid		= GLOBAL_ROOT_UID,
	.ratelimit	= RATELIMIT_STATE_INIT(root_user.ratelimit, 0, 0),
};
=======
struct ext_user_struct ext_root_user = {
	.user.__count	= REFCOUNT_INIT(1),
	.user.uid	= GLOBAL_ROOT_UID,
	.user.ratelimit	= RATELIMIT_STATE_INIT(root_user.ratelimit, 0, 0),
};
struct user_struct *root_user = &ext_root_user.user;
>>>>>>> upstream/android-13

/*
 * These routines must be called with the uidhash spinlock held!
 */
static void uid_hash_insert(struct user_struct *up, struct hlist_head *hashent)
{
	hlist_add_head(&up->uidhash_node, hashent);
}

static void uid_hash_remove(struct user_struct *up)
{
	hlist_del_init(&up->uidhash_node);
}

static struct user_struct *uid_hash_find(kuid_t uid, struct hlist_head *hashent)
{
	struct user_struct *user;

	hlist_for_each_entry(user, hashent, uidhash_node) {
		if (uid_eq(user->uid, uid)) {
			refcount_inc(&user->__count);
			return user;
		}
	}

	return NULL;
}

<<<<<<< HEAD
=======
static int user_epoll_alloc(struct user_struct *up)
{
#ifdef CONFIG_EPOLL
	return percpu_counter_init(&up->epoll_watches, 0, GFP_KERNEL);
#else
	return 0;
#endif
}

static void user_epoll_free(struct user_struct *up)
{
#ifdef CONFIG_EPOLL
	percpu_counter_destroy(&up->epoll_watches);
#endif
}

>>>>>>> upstream/android-13
/* IRQs are disabled and uidhash_lock is held upon function entry.
 * IRQ state (as stored in flags) is restored and uidhash_lock released
 * upon function exit.
 */
static void free_user(struct user_struct *up, unsigned long flags)
	__releases(&uidhash_lock)
{
<<<<<<< HEAD
	uid_hash_remove(up);
	spin_unlock_irqrestore(&uidhash_lock, flags);
	key_put(up->uid_keyring);
	key_put(up->session_keyring);
	kmem_cache_free(uid_cachep, up);
=======
	struct ext_user_struct *ext_user;

	ext_user = container_of(up, struct ext_user_struct, user);
	trace_android_vh_free_user(up);
	uid_hash_remove(up);
	spin_unlock_irqrestore(&uidhash_lock, flags);
	user_epoll_free(up);
	kmem_cache_free(uid_cachep, ext_user);
>>>>>>> upstream/android-13
}

/*
 * Locate the user_struct for the passed UID.  If found, take a ref on it.  The
 * caller must undo that ref with free_uid().
 *
 * If the user_struct could not be found, return NULL.
 */
struct user_struct *find_user(kuid_t uid)
{
	struct user_struct *ret;
	unsigned long flags;

	spin_lock_irqsave(&uidhash_lock, flags);
	ret = uid_hash_find(uid, uidhashentry(uid));
	spin_unlock_irqrestore(&uidhash_lock, flags);
	return ret;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(find_user);
>>>>>>> upstream/android-13

void free_uid(struct user_struct *up)
{
	unsigned long flags;

	if (!up)
		return;

	if (refcount_dec_and_lock_irqsave(&up->__count, &uidhash_lock, &flags))
		free_user(up, flags);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(free_uid);
>>>>>>> upstream/android-13

struct user_struct *alloc_uid(kuid_t uid)
{
	struct hlist_head *hashent = uidhashentry(uid);
	struct user_struct *up, *new;
<<<<<<< HEAD
=======
	struct ext_user_struct *ext_user;
>>>>>>> upstream/android-13

	spin_lock_irq(&uidhash_lock);
	up = uid_hash_find(uid, hashent);
	spin_unlock_irq(&uidhash_lock);

	if (!up) {
<<<<<<< HEAD
		new = kmem_cache_zalloc(uid_cachep, GFP_KERNEL);
		if (!new)
			goto out_unlock;

		new->uid = uid;
		refcount_set(&new->__count, 1);
=======
		ext_user = kmem_cache_zalloc(uid_cachep, GFP_KERNEL);
		if (!ext_user)
			return NULL;

		new = &ext_user->user;
		new->uid = uid;
		refcount_set(&new->__count, 1);
		trace_android_vh_alloc_uid(new);
		if (user_epoll_alloc(new)) {
			kmem_cache_free(uid_cachep, new);
			return NULL;
		}
>>>>>>> upstream/android-13
		ratelimit_state_init(&new->ratelimit, HZ, 100);
		ratelimit_set_flags(&new->ratelimit, RATELIMIT_MSG_ON_RELEASE);

		/*
		 * Before adding this, check whether we raced
		 * on adding the same user already..
		 */
		spin_lock_irq(&uidhash_lock);
		up = uid_hash_find(uid, hashent);
		if (up) {
<<<<<<< HEAD
			key_put(new->uid_keyring);
			key_put(new->session_keyring);
=======
			user_epoll_free(new);
>>>>>>> upstream/android-13
			kmem_cache_free(uid_cachep, new);
		} else {
			uid_hash_insert(new, hashent);
			up = new;
		}
		spin_unlock_irq(&uidhash_lock);
	}
<<<<<<< HEAD
	proc_register_uid(uid);

	return up;

out_unlock:
	return NULL;
=======

	return up;
>>>>>>> upstream/android-13
}

static int __init uid_cache_init(void)
{
	int n;

<<<<<<< HEAD
	uid_cachep = kmem_cache_create("uid_cache", sizeof(struct user_struct),
=======
	uid_cachep = kmem_cache_create("uid_cache", sizeof(struct ext_user_struct),
>>>>>>> upstream/android-13
			0, SLAB_HWCACHE_ALIGN|SLAB_PANIC, NULL);

	for(n = 0; n < UIDHASH_SZ; ++n)
		INIT_HLIST_HEAD(uidhash_table + n);

<<<<<<< HEAD
	/* Insert the root user immediately (init already runs as root) */
	spin_lock_irq(&uidhash_lock);
	uid_hash_insert(&root_user, uidhashentry(GLOBAL_ROOT_UID));
	spin_unlock_irq(&uidhash_lock);
	proc_register_uid(GLOBAL_ROOT_UID);
=======
	if (user_epoll_alloc(root_user))
		panic("root_user epoll percpu counter alloc failed");

	/* Insert the root user immediately (init already runs as root) */
	spin_lock_irq(&uidhash_lock);
	uid_hash_insert(root_user, uidhashentry(GLOBAL_ROOT_UID));
	spin_unlock_irq(&uidhash_lock);
>>>>>>> upstream/android-13

	return 0;
}
subsys_initcall(uid_cache_init);
