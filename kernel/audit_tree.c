// SPDX-License-Identifier: GPL-2.0
#include "audit.h"
#include <linux/fsnotify_backend.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/kthread.h>
#include <linux/refcount.h>
#include <linux/slab.h>

struct audit_tree;
struct audit_chunk;

struct audit_tree {
	refcount_t count;
	int goner;
	struct audit_chunk *root;
	struct list_head chunks;
	struct list_head rules;
	struct list_head list;
	struct list_head same_root;
	struct rcu_head head;
	char pathname[];
};

struct audit_chunk {
	struct list_head hash;
	unsigned long key;
<<<<<<< HEAD
	struct fsnotify_mark mark;
	struct list_head trees;		/* with root here */
	int dead;
=======
	struct fsnotify_mark *mark;
	struct list_head trees;		/* with root here */
>>>>>>> upstream/android-13
	int count;
	atomic_long_t refs;
	struct rcu_head head;
	struct node {
		struct list_head list;
		struct audit_tree *owner;
		unsigned index;		/* index; upper bit indicates 'will prune' */
	} owners[];
};

<<<<<<< HEAD
=======
struct audit_tree_mark {
	struct fsnotify_mark mark;
	struct audit_chunk *chunk;
};

>>>>>>> upstream/android-13
static LIST_HEAD(tree_list);
static LIST_HEAD(prune_list);
static struct task_struct *prune_thread;

/*
<<<<<<< HEAD
 * One struct chunk is attached to each inode of interest.
 * We replace struct chunk on tagging/untagging.
=======
 * One struct chunk is attached to each inode of interest through
 * audit_tree_mark (fsnotify mark). We replace struct chunk on tagging /
 * untagging, the mark is stable as long as there is chunk attached. The
 * association between mark and chunk is protected by hash_lock and
 * audit_tree_group->mark_mutex. Thus as long as we hold
 * audit_tree_group->mark_mutex and check that the mark is alive by
 * FSNOTIFY_MARK_FLAG_ATTACHED flag check, we are sure the mark points to
 * the current chunk.
 *
>>>>>>> upstream/android-13
 * Rules have pointer to struct audit_tree.
 * Rules have struct list_head rlist forming a list of rules over
 * the same tree.
 * References to struct chunk are collected at audit_inode{,_child}()
 * time and used in AUDIT_TREE rule matching.
 * These references are dropped at the same time we are calling
 * audit_free_names(), etc.
 *
 * Cyclic lists galore:
 * tree.chunks anchors chunk.owners[].list			hash_lock
 * tree.rules anchors rule.rlist				audit_filter_mutex
 * chunk.trees anchors tree.same_root				hash_lock
 * chunk.hash is a hash with middle bits of watch.inode as
 * a hash function.						RCU, hash_lock
 *
 * tree is refcounted; one reference for "some rules on rules_list refer to
 * it", one for each chunk with pointer to it.
 *
<<<<<<< HEAD
 * chunk is refcounted by embedded fsnotify_mark + .refs (non-zero refcount
 * of watch contributes 1 to .refs).
=======
 * chunk is refcounted by embedded .refs. Mark associated with the chunk holds
 * one chunk reference. This reference is dropped either when a mark is going
 * to be freed (corresponding inode goes away) or when chunk attached to the
 * mark gets replaced. This reference must be dropped using
 * audit_mark_put_chunk() to make sure the reference is dropped only after RCU
 * grace period as it protects RCU readers of the hash table.
>>>>>>> upstream/android-13
 *
 * node.index allows to get from node.list to containing chunk.
 * MSB of that sucker is stolen to mark taggings that we might have to
 * revert - several operations have very unpleasant cleanup logics and
 * that makes a difference.  Some.
 */

static struct fsnotify_group *audit_tree_group;
<<<<<<< HEAD
=======
static struct kmem_cache *audit_tree_mark_cachep __read_mostly;
>>>>>>> upstream/android-13

static struct audit_tree *alloc_tree(const char *s)
{
	struct audit_tree *tree;

	tree = kmalloc(sizeof(struct audit_tree) + strlen(s) + 1, GFP_KERNEL);
	if (tree) {
		refcount_set(&tree->count, 1);
		tree->goner = 0;
		INIT_LIST_HEAD(&tree->chunks);
		INIT_LIST_HEAD(&tree->rules);
		INIT_LIST_HEAD(&tree->list);
		INIT_LIST_HEAD(&tree->same_root);
		tree->root = NULL;
		strcpy(tree->pathname, s);
	}
	return tree;
}

static inline void get_tree(struct audit_tree *tree)
{
	refcount_inc(&tree->count);
}

static inline void put_tree(struct audit_tree *tree)
{
	if (refcount_dec_and_test(&tree->count))
		kfree_rcu(tree, head);
}

/* to avoid bringing the entire thing in audit.h */
const char *audit_tree_path(struct audit_tree *tree)
{
	return tree->pathname;
}

static void free_chunk(struct audit_chunk *chunk)
{
	int i;

	for (i = 0; i < chunk->count; i++) {
		if (chunk->owners[i].owner)
			put_tree(chunk->owners[i].owner);
	}
	kfree(chunk);
}

void audit_put_chunk(struct audit_chunk *chunk)
{
	if (atomic_long_dec_and_test(&chunk->refs))
		free_chunk(chunk);
}

static void __put_chunk(struct rcu_head *rcu)
{
	struct audit_chunk *chunk = container_of(rcu, struct audit_chunk, head);
	audit_put_chunk(chunk);
}

<<<<<<< HEAD
static void audit_tree_destroy_watch(struct fsnotify_mark *entry)
{
	struct audit_chunk *chunk = container_of(entry, struct audit_chunk, mark);
	call_rcu(&chunk->head, __put_chunk);
}

static struct audit_chunk *alloc_chunk(int count)
{
	struct audit_chunk *chunk;
	size_t size;
	int i;

	size = offsetof(struct audit_chunk, owners) + count * sizeof(struct node);
	chunk = kzalloc(size, GFP_KERNEL);
=======
/*
 * Drop reference to the chunk that was held by the mark. This is the reference
 * that gets dropped after we've removed the chunk from the hash table and we
 * use it to make sure chunk cannot be freed before RCU grace period expires.
 */
static void audit_mark_put_chunk(struct audit_chunk *chunk)
{
	call_rcu(&chunk->head, __put_chunk);
}

static inline struct audit_tree_mark *audit_mark(struct fsnotify_mark *mark)
{
	return container_of(mark, struct audit_tree_mark, mark);
}

static struct audit_chunk *mark_chunk(struct fsnotify_mark *mark)
{
	return audit_mark(mark)->chunk;
}

static void audit_tree_destroy_watch(struct fsnotify_mark *mark)
{
	kmem_cache_free(audit_tree_mark_cachep, audit_mark(mark));
}

static struct fsnotify_mark *alloc_mark(void)
{
	struct audit_tree_mark *amark;

	amark = kmem_cache_zalloc(audit_tree_mark_cachep, GFP_KERNEL);
	if (!amark)
		return NULL;
	fsnotify_init_mark(&amark->mark, audit_tree_group);
	amark->mark.mask = FS_IN_IGNORED;
	return &amark->mark;
}

static struct audit_chunk *alloc_chunk(int count)
{
	struct audit_chunk *chunk;
	int i;

	chunk = kzalloc(struct_size(chunk, owners, count), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!chunk)
		return NULL;

	INIT_LIST_HEAD(&chunk->hash);
	INIT_LIST_HEAD(&chunk->trees);
	chunk->count = count;
	atomic_long_set(&chunk->refs, 1);
	for (i = 0; i < count; i++) {
		INIT_LIST_HEAD(&chunk->owners[i].list);
		chunk->owners[i].index = i;
	}
<<<<<<< HEAD
	fsnotify_init_mark(&chunk->mark, audit_tree_group);
	chunk->mark.mask = FS_IN_IGNORED;
=======
>>>>>>> upstream/android-13
	return chunk;
}

enum {HASH_SIZE = 128};
static struct list_head chunk_hash_heads[HASH_SIZE];
static __cacheline_aligned_in_smp DEFINE_SPINLOCK(hash_lock);

/* Function to return search key in our hash from inode. */
static unsigned long inode_to_key(const struct inode *inode)
{
	/* Use address pointed to by connector->obj as the key */
	return (unsigned long)&inode->i_fsnotify_marks;
}

static inline struct list_head *chunk_hash(unsigned long key)
{
	unsigned long n = key / L1_CACHE_BYTES;
	return chunk_hash_heads + n % HASH_SIZE;
}

<<<<<<< HEAD
/* hash_lock & entry->lock is held by caller */
=======
/* hash_lock & mark->group->mark_mutex is held by caller */
>>>>>>> upstream/android-13
static void insert_hash(struct audit_chunk *chunk)
{
	struct list_head *list;

<<<<<<< HEAD
	if (!(chunk->mark.flags & FSNOTIFY_MARK_FLAG_ATTACHED))
		return;
=======
	/*
	 * Make sure chunk is fully initialized before making it visible in the
	 * hash. Pairs with a data dependency barrier in READ_ONCE() in
	 * audit_tree_lookup().
	 */
	smp_wmb();
>>>>>>> upstream/android-13
	WARN_ON_ONCE(!chunk->key);
	list = chunk_hash(chunk->key);
	list_add_rcu(&chunk->hash, list);
}

/* called under rcu_read_lock */
struct audit_chunk *audit_tree_lookup(const struct inode *inode)
{
	unsigned long key = inode_to_key(inode);
	struct list_head *list = chunk_hash(key);
	struct audit_chunk *p;

	list_for_each_entry_rcu(p, list, hash) {
<<<<<<< HEAD
		if (p->key == key) {
=======
		/*
		 * We use a data dependency barrier in READ_ONCE() to make sure
		 * the chunk we see is fully initialized.
		 */
		if (READ_ONCE(p->key) == key) {
>>>>>>> upstream/android-13
			atomic_long_inc(&p->refs);
			return p;
		}
	}
	return NULL;
}

bool audit_tree_match(struct audit_chunk *chunk, struct audit_tree *tree)
{
	int n;
	for (n = 0; n < chunk->count; n++)
		if (chunk->owners[n].owner == tree)
			return true;
	return false;
}

/* tagging and untagging inodes with trees */

static struct audit_chunk *find_chunk(struct node *p)
{
	int index = p->index & ~(1U<<31);
	p -= index;
	return container_of(p, struct audit_chunk, owners[0]);
}

<<<<<<< HEAD
static void untag_chunk(struct node *p)
{
	struct audit_chunk *chunk = find_chunk(p);
	struct fsnotify_mark *entry = &chunk->mark;
	struct audit_chunk *new = NULL;
	struct audit_tree *owner;
	int size = chunk->count - 1;
	int i, j;

	fsnotify_get_mark(entry);

	spin_unlock(&hash_lock);

	if (size)
		new = alloc_chunk(size);

	mutex_lock(&entry->group->mark_mutex);
	spin_lock(&entry->lock);
	/*
	 * mark_mutex protects mark from getting detached and thus also from
	 * mark->connector->obj getting NULL.
	 */
	if (chunk->dead || !(entry->flags & FSNOTIFY_MARK_FLAG_ATTACHED)) {
		spin_unlock(&entry->lock);
		mutex_unlock(&entry->group->mark_mutex);
		if (new)
			fsnotify_put_mark(&new->mark);
		goto out;
	}

	owner = p->owner;

	if (!size) {
		chunk->dead = 1;
		spin_lock(&hash_lock);
		list_del_init(&chunk->trees);
		if (owner->root == chunk)
			owner->root = NULL;
		list_del_init(&p->list);
		list_del_rcu(&chunk->hash);
		spin_unlock(&hash_lock);
		spin_unlock(&entry->lock);
		mutex_unlock(&entry->group->mark_mutex);
		fsnotify_destroy_mark(entry, audit_tree_group);
		goto out;
	}

	if (!new)
		goto Fallback;

	if (fsnotify_add_mark_locked(&new->mark, entry->connector->obj,
				     FSNOTIFY_OBJ_TYPE_INODE, 1)) {
		fsnotify_put_mark(&new->mark);
		goto Fallback;
	}

	chunk->dead = 1;
	spin_lock(&hash_lock);
	new->key = chunk->key;
	list_replace_init(&chunk->trees, &new->trees);
	if (owner->root == chunk) {
		list_del_init(&owner->same_root);
		owner->root = NULL;
	}

	for (i = j = 0; j <= size; i++, j++) {
		struct audit_tree *s;
		if (&chunk->owners[j] == p) {
			list_del_init(&p->list);
			i--;
			continue;
		}
		s = chunk->owners[j].owner;
		new->owners[i].owner = s;
		new->owners[i].index = chunk->owners[j].index - j + i;
		if (!s) /* result of earlier fallback */
			continue;
		get_tree(s);
		list_replace_init(&chunk->owners[j].list, &new->owners[i].list);
	}

	list_replace_rcu(&chunk->hash, &new->hash);
	list_for_each_entry(owner, &new->trees, same_root)
		owner->root = new;
	spin_unlock(&hash_lock);
	spin_unlock(&entry->lock);
	mutex_unlock(&entry->group->mark_mutex);
	fsnotify_destroy_mark(entry, audit_tree_group);
	fsnotify_put_mark(&new->mark);	/* drop initial reference */
	goto out;

Fallback:
	// do the best we can
	spin_lock(&hash_lock);
=======
static void replace_mark_chunk(struct fsnotify_mark *mark,
			       struct audit_chunk *chunk)
{
	struct audit_chunk *old;

	assert_spin_locked(&hash_lock);
	old = mark_chunk(mark);
	audit_mark(mark)->chunk = chunk;
	if (chunk)
		chunk->mark = mark;
	if (old)
		old->mark = NULL;
}

static void replace_chunk(struct audit_chunk *new, struct audit_chunk *old)
{
	struct audit_tree *owner;
	int i, j;

	new->key = old->key;
	list_splice_init(&old->trees, &new->trees);
	list_for_each_entry(owner, &new->trees, same_root)
		owner->root = new;
	for (i = j = 0; j < old->count; i++, j++) {
		if (!old->owners[j].owner) {
			i--;
			continue;
		}
		owner = old->owners[j].owner;
		new->owners[i].owner = owner;
		new->owners[i].index = old->owners[j].index - j + i;
		if (!owner) /* result of earlier fallback */
			continue;
		get_tree(owner);
		list_replace_init(&old->owners[j].list, &new->owners[i].list);
	}
	replace_mark_chunk(old->mark, new);
	/*
	 * Make sure chunk is fully initialized before making it visible in the
	 * hash. Pairs with a data dependency barrier in READ_ONCE() in
	 * audit_tree_lookup().
	 */
	smp_wmb();
	list_replace_rcu(&old->hash, &new->hash);
}

static void remove_chunk_node(struct audit_chunk *chunk, struct node *p)
{
	struct audit_tree *owner = p->owner;

>>>>>>> upstream/android-13
	if (owner->root == chunk) {
		list_del_init(&owner->same_root);
		owner->root = NULL;
	}
	list_del_init(&p->list);
	p->owner = NULL;
	put_tree(owner);
<<<<<<< HEAD
	spin_unlock(&hash_lock);
	spin_unlock(&entry->lock);
	mutex_unlock(&entry->group->mark_mutex);
out:
	fsnotify_put_mark(entry);
	spin_lock(&hash_lock);
}

static int create_chunk(struct inode *inode, struct audit_tree *tree)
{
	struct fsnotify_mark *entry;
	struct audit_chunk *chunk = alloc_chunk(1);
	if (!chunk)
		return -ENOMEM;

	entry = &chunk->mark;
	if (fsnotify_add_inode_mark(entry, inode, 0)) {
		fsnotify_put_mark(entry);
		return -ENOSPC;
	}

	spin_lock(&entry->lock);
	spin_lock(&hash_lock);
	if (tree->goner) {
		spin_unlock(&hash_lock);
		chunk->dead = 1;
		spin_unlock(&entry->lock);
		fsnotify_destroy_mark(entry, audit_tree_group);
		fsnotify_put_mark(entry);
		return 0;
	}
=======
}

static int chunk_count_trees(struct audit_chunk *chunk)
{
	int i;
	int ret = 0;

	for (i = 0; i < chunk->count; i++)
		if (chunk->owners[i].owner)
			ret++;
	return ret;
}

static void untag_chunk(struct audit_chunk *chunk, struct fsnotify_mark *mark)
{
	struct audit_chunk *new;
	int size;

	mutex_lock(&audit_tree_group->mark_mutex);
	/*
	 * mark_mutex stabilizes chunk attached to the mark so we can check
	 * whether it didn't change while we've dropped hash_lock.
	 */
	if (!(mark->flags & FSNOTIFY_MARK_FLAG_ATTACHED) ||
	    mark_chunk(mark) != chunk)
		goto out_mutex;

	size = chunk_count_trees(chunk);
	if (!size) {
		spin_lock(&hash_lock);
		list_del_init(&chunk->trees);
		list_del_rcu(&chunk->hash);
		replace_mark_chunk(mark, NULL);
		spin_unlock(&hash_lock);
		fsnotify_detach_mark(mark);
		mutex_unlock(&audit_tree_group->mark_mutex);
		audit_mark_put_chunk(chunk);
		fsnotify_free_mark(mark);
		return;
	}

	new = alloc_chunk(size);
	if (!new)
		goto out_mutex;

	spin_lock(&hash_lock);
	/*
	 * This has to go last when updating chunk as once replace_chunk() is
	 * called, new RCU readers can see the new chunk.
	 */
	replace_chunk(new, chunk);
	spin_unlock(&hash_lock);
	mutex_unlock(&audit_tree_group->mark_mutex);
	audit_mark_put_chunk(chunk);
	return;

out_mutex:
	mutex_unlock(&audit_tree_group->mark_mutex);
}

/* Call with group->mark_mutex held, releases it */
static int create_chunk(struct inode *inode, struct audit_tree *tree)
{
	struct fsnotify_mark *mark;
	struct audit_chunk *chunk = alloc_chunk(1);

	if (!chunk) {
		mutex_unlock(&audit_tree_group->mark_mutex);
		return -ENOMEM;
	}

	mark = alloc_mark();
	if (!mark) {
		mutex_unlock(&audit_tree_group->mark_mutex);
		kfree(chunk);
		return -ENOMEM;
	}

	if (fsnotify_add_inode_mark_locked(mark, inode, 0)) {
		mutex_unlock(&audit_tree_group->mark_mutex);
		fsnotify_put_mark(mark);
		kfree(chunk);
		return -ENOSPC;
	}

	spin_lock(&hash_lock);
	if (tree->goner) {
		spin_unlock(&hash_lock);
		fsnotify_detach_mark(mark);
		mutex_unlock(&audit_tree_group->mark_mutex);
		fsnotify_free_mark(mark);
		fsnotify_put_mark(mark);
		kfree(chunk);
		return 0;
	}
	replace_mark_chunk(mark, chunk);
>>>>>>> upstream/android-13
	chunk->owners[0].index = (1U << 31);
	chunk->owners[0].owner = tree;
	get_tree(tree);
	list_add(&chunk->owners[0].list, &tree->chunks);
	if (!tree->root) {
		tree->root = chunk;
		list_add(&tree->same_root, &chunk->trees);
	}
	chunk->key = inode_to_key(inode);
<<<<<<< HEAD
	insert_hash(chunk);
	spin_unlock(&hash_lock);
	spin_unlock(&entry->lock);
	fsnotify_put_mark(entry);	/* drop initial reference */
=======
	/*
	 * Inserting into the hash table has to go last as once we do that RCU
	 * readers can see the chunk.
	 */
	insert_hash(chunk);
	spin_unlock(&hash_lock);
	mutex_unlock(&audit_tree_group->mark_mutex);
	/*
	 * Drop our initial reference. When mark we point to is getting freed,
	 * we get notification through ->freeing_mark callback and cleanup
	 * chunk pointing to this mark.
	 */
	fsnotify_put_mark(mark);
>>>>>>> upstream/android-13
	return 0;
}

/* the first tagged inode becomes root of tree */
static int tag_chunk(struct inode *inode, struct audit_tree *tree)
{
<<<<<<< HEAD
	struct fsnotify_mark *old_entry, *chunk_entry;
	struct audit_tree *owner;
=======
	struct fsnotify_mark *mark;
>>>>>>> upstream/android-13
	struct audit_chunk *chunk, *old;
	struct node *p;
	int n;

<<<<<<< HEAD
	old_entry = fsnotify_find_mark(&inode->i_fsnotify_marks,
				       audit_tree_group);
	if (!old_entry)
		return create_chunk(inode, tree);

	old = container_of(old_entry, struct audit_chunk, mark);

	/* are we already there? */
	spin_lock(&hash_lock);
	for (n = 0; n < old->count; n++) {
		if (old->owners[n].owner == tree) {
			spin_unlock(&hash_lock);
			fsnotify_put_mark(old_entry);
=======
	mutex_lock(&audit_tree_group->mark_mutex);
	mark = fsnotify_find_mark(&inode->i_fsnotify_marks, audit_tree_group);
	if (!mark)
		return create_chunk(inode, tree);

	/*
	 * Found mark is guaranteed to be attached and mark_mutex protects mark
	 * from getting detached and thus it makes sure there is chunk attached
	 * to the mark.
	 */
	/* are we already there? */
	spin_lock(&hash_lock);
	old = mark_chunk(mark);
	for (n = 0; n < old->count; n++) {
		if (old->owners[n].owner == tree) {
			spin_unlock(&hash_lock);
			mutex_unlock(&audit_tree_group->mark_mutex);
			fsnotify_put_mark(mark);
>>>>>>> upstream/android-13
			return 0;
		}
	}
	spin_unlock(&hash_lock);

	chunk = alloc_chunk(old->count + 1);
	if (!chunk) {
<<<<<<< HEAD
		fsnotify_put_mark(old_entry);
		return -ENOMEM;
	}

	chunk_entry = &chunk->mark;

	mutex_lock(&old_entry->group->mark_mutex);
	spin_lock(&old_entry->lock);
	/*
	 * mark_mutex protects mark from getting detached and thus also from
	 * mark->connector->obj getting NULL.
	 */
	if (!(old_entry->flags & FSNOTIFY_MARK_FLAG_ATTACHED)) {
		/* old_entry is being shot, lets just lie */
		spin_unlock(&old_entry->lock);
		mutex_unlock(&old_entry->group->mark_mutex);
		fsnotify_put_mark(old_entry);
		fsnotify_put_mark(&chunk->mark);
		return -ENOENT;
	}

	if (fsnotify_add_mark_locked(chunk_entry, old_entry->connector->obj,
				     FSNOTIFY_OBJ_TYPE_INODE, 1)) {
		spin_unlock(&old_entry->lock);
		mutex_unlock(&old_entry->group->mark_mutex);
		fsnotify_put_mark(chunk_entry);
		fsnotify_put_mark(old_entry);
		return -ENOSPC;
	}

	/* even though we hold old_entry->lock, this is safe since chunk_entry->lock could NEVER have been grabbed before */
	spin_lock(&chunk_entry->lock);
	spin_lock(&hash_lock);

	/* we now hold old_entry->lock, chunk_entry->lock, and hash_lock */
	if (tree->goner) {
		spin_unlock(&hash_lock);
		chunk->dead = 1;
		spin_unlock(&chunk_entry->lock);
		spin_unlock(&old_entry->lock);
		mutex_unlock(&old_entry->group->mark_mutex);

		fsnotify_destroy_mark(chunk_entry, audit_tree_group);

		fsnotify_put_mark(chunk_entry);
		fsnotify_put_mark(old_entry);
		return 0;
	}
	chunk->key = old->key;
	list_replace_init(&old->trees, &chunk->trees);
	for (n = 0, p = chunk->owners; n < old->count; n++, p++) {
		struct audit_tree *s = old->owners[n].owner;
		p->owner = s;
		p->index = old->owners[n].index;
		if (!s) /* result of fallback in untag */
			continue;
		get_tree(s);
		list_replace_init(&old->owners[n].list, &p->list);
	}
=======
		mutex_unlock(&audit_tree_group->mark_mutex);
		fsnotify_put_mark(mark);
		return -ENOMEM;
	}

	spin_lock(&hash_lock);
	if (tree->goner) {
		spin_unlock(&hash_lock);
		mutex_unlock(&audit_tree_group->mark_mutex);
		fsnotify_put_mark(mark);
		kfree(chunk);
		return 0;
	}
	p = &chunk->owners[chunk->count - 1];
>>>>>>> upstream/android-13
	p->index = (chunk->count - 1) | (1U<<31);
	p->owner = tree;
	get_tree(tree);
	list_add(&p->list, &tree->chunks);
<<<<<<< HEAD
	list_replace_rcu(&old->hash, &chunk->hash);
	list_for_each_entry(owner, &chunk->trees, same_root)
		owner->root = chunk;
	old->dead = 1;
=======
>>>>>>> upstream/android-13
	if (!tree->root) {
		tree->root = chunk;
		list_add(&tree->same_root, &chunk->trees);
	}
<<<<<<< HEAD
	spin_unlock(&hash_lock);
	spin_unlock(&chunk_entry->lock);
	spin_unlock(&old_entry->lock);
	mutex_unlock(&old_entry->group->mark_mutex);
	fsnotify_destroy_mark(old_entry, audit_tree_group);
	fsnotify_put_mark(chunk_entry);	/* drop initial reference */
	fsnotify_put_mark(old_entry); /* pair to fsnotify_find mark_entry */
	return 0;
}

static void audit_tree_log_remove_rule(struct audit_krule *rule)
=======
	/*
	 * This has to go last when updating chunk as once replace_chunk() is
	 * called, new RCU readers can see the new chunk.
	 */
	replace_chunk(chunk, old);
	spin_unlock(&hash_lock);
	mutex_unlock(&audit_tree_group->mark_mutex);
	fsnotify_put_mark(mark); /* pair to fsnotify_find_mark */
	audit_mark_put_chunk(old);

	return 0;
}

static void audit_tree_log_remove_rule(struct audit_context *context,
				       struct audit_krule *rule)
>>>>>>> upstream/android-13
{
	struct audit_buffer *ab;

	if (!audit_enabled)
		return;
<<<<<<< HEAD
	ab = audit_log_start(NULL, GFP_KERNEL, AUDIT_CONFIG_CHANGE);
	if (unlikely(!ab))
		return;
	audit_log_format(ab, "op=remove_rule");
	audit_log_format(ab, " dir=");
=======
	ab = audit_log_start(context, GFP_KERNEL, AUDIT_CONFIG_CHANGE);
	if (unlikely(!ab))
		return;
	audit_log_format(ab, "op=remove_rule dir=");
>>>>>>> upstream/android-13
	audit_log_untrustedstring(ab, rule->tree->pathname);
	audit_log_key(ab, rule->filterkey);
	audit_log_format(ab, " list=%d res=1", rule->listnr);
	audit_log_end(ab);
}

<<<<<<< HEAD
static void kill_rules(struct audit_tree *tree)
=======
static void kill_rules(struct audit_context *context, struct audit_tree *tree)
>>>>>>> upstream/android-13
{
	struct audit_krule *rule, *next;
	struct audit_entry *entry;

	list_for_each_entry_safe(rule, next, &tree->rules, rlist) {
		entry = container_of(rule, struct audit_entry, rule);

		list_del_init(&rule->rlist);
		if (rule->tree) {
			/* not a half-baked one */
<<<<<<< HEAD
			audit_tree_log_remove_rule(rule);
=======
			audit_tree_log_remove_rule(context, rule);
>>>>>>> upstream/android-13
			if (entry->rule.exe)
				audit_remove_mark(entry->rule.exe);
			rule->tree = NULL;
			list_del_rcu(&entry->list);
			list_del(&entry->rule.list);
			call_rcu(&entry->rcu, audit_free_rule_rcu);
		}
	}
}

/*
<<<<<<< HEAD
 * finish killing struct audit_tree
 */
static void prune_one(struct audit_tree *victim)
=======
 * Remove tree from chunks. If 'tagged' is set, remove tree only from tagged
 * chunks. The function expects tagged chunks are all at the beginning of the
 * chunks list.
 */
static void prune_tree_chunks(struct audit_tree *victim, bool tagged)
>>>>>>> upstream/android-13
{
	spin_lock(&hash_lock);
	while (!list_empty(&victim->chunks)) {
		struct node *p;
<<<<<<< HEAD

		p = list_entry(victim->chunks.next, struct node, list);

		untag_chunk(p);
	}
	spin_unlock(&hash_lock);
=======
		struct audit_chunk *chunk;
		struct fsnotify_mark *mark;

		p = list_first_entry(&victim->chunks, struct node, list);
		/* have we run out of marked? */
		if (tagged && !(p->index & (1U<<31)))
			break;
		chunk = find_chunk(p);
		mark = chunk->mark;
		remove_chunk_node(chunk, p);
		/* Racing with audit_tree_freeing_mark()? */
		if (!mark)
			continue;
		fsnotify_get_mark(mark);
		spin_unlock(&hash_lock);

		untag_chunk(chunk, mark);
		fsnotify_put_mark(mark);

		spin_lock(&hash_lock);
	}
	spin_unlock(&hash_lock);
}

/*
 * finish killing struct audit_tree
 */
static void prune_one(struct audit_tree *victim)
{
	prune_tree_chunks(victim, false);
>>>>>>> upstream/android-13
	put_tree(victim);
}

/* trim the uncommitted chunks from tree */

static void trim_marked(struct audit_tree *tree)
{
	struct list_head *p, *q;
	spin_lock(&hash_lock);
	if (tree->goner) {
		spin_unlock(&hash_lock);
		return;
	}
	/* reorder */
	for (p = tree->chunks.next; p != &tree->chunks; p = q) {
		struct node *node = list_entry(p, struct node, list);
		q = p->next;
		if (node->index & (1U<<31)) {
			list_del_init(p);
			list_add(p, &tree->chunks);
		}
	}
<<<<<<< HEAD

	while (!list_empty(&tree->chunks)) {
		struct node *node;

		node = list_entry(tree->chunks.next, struct node, list);

		/* have we run out of marked? */
		if (!(node->index & (1U<<31)))
			break;

		untag_chunk(node);
	}
=======
	spin_unlock(&hash_lock);

	prune_tree_chunks(tree, true);

	spin_lock(&hash_lock);
>>>>>>> upstream/android-13
	if (!tree->root && !tree->goner) {
		tree->goner = 1;
		spin_unlock(&hash_lock);
		mutex_lock(&audit_filter_mutex);
<<<<<<< HEAD
		kill_rules(tree);
=======
		kill_rules(audit_context(), tree);
>>>>>>> upstream/android-13
		list_del_init(&tree->list);
		mutex_unlock(&audit_filter_mutex);
		prune_one(tree);
	} else {
		spin_unlock(&hash_lock);
	}
}

static void audit_schedule_prune(void);

/* called with audit_filter_mutex */
int audit_remove_tree_rule(struct audit_krule *rule)
{
	struct audit_tree *tree;
	tree = rule->tree;
	if (tree) {
		spin_lock(&hash_lock);
		list_del_init(&rule->rlist);
		if (list_empty(&tree->rules) && !tree->goner) {
			tree->root = NULL;
			list_del_init(&tree->same_root);
			tree->goner = 1;
			list_move(&tree->list, &prune_list);
			rule->tree = NULL;
			spin_unlock(&hash_lock);
			audit_schedule_prune();
			return 1;
		}
		rule->tree = NULL;
		spin_unlock(&hash_lock);
		return 1;
	}
	return 0;
}

static int compare_root(struct vfsmount *mnt, void *arg)
{
	return inode_to_key(d_backing_inode(mnt->mnt_root)) ==
	       (unsigned long)arg;
}

void audit_trim_trees(void)
{
	struct list_head cursor;

	mutex_lock(&audit_filter_mutex);
	list_add(&cursor, &tree_list);
	while (cursor.next != &tree_list) {
		struct audit_tree *tree;
		struct path path;
		struct vfsmount *root_mnt;
		struct node *node;
		int err;

		tree = container_of(cursor.next, struct audit_tree, list);
		get_tree(tree);
<<<<<<< HEAD
		list_del(&cursor);
		list_add(&cursor, &tree->list);
=======
		list_move(&cursor, &tree->list);
>>>>>>> upstream/android-13
		mutex_unlock(&audit_filter_mutex);

		err = kern_path(tree->pathname, 0, &path);
		if (err)
			goto skip_it;

		root_mnt = collect_mounts(&path);
		path_put(&path);
		if (IS_ERR(root_mnt))
			goto skip_it;

		spin_lock(&hash_lock);
		list_for_each_entry(node, &tree->chunks, list) {
			struct audit_chunk *chunk = find_chunk(node);
			/* this could be NULL if the watch is dying else where... */
			node->index |= 1U<<31;
			if (iterate_mounts(compare_root,
					   (void *)(chunk->key),
					   root_mnt))
				node->index &= ~(1U<<31);
		}
		spin_unlock(&hash_lock);
		trim_marked(tree);
		drop_collected_mounts(root_mnt);
skip_it:
		put_tree(tree);
		mutex_lock(&audit_filter_mutex);
	}
	list_del(&cursor);
	mutex_unlock(&audit_filter_mutex);
}

int audit_make_tree(struct audit_krule *rule, char *pathname, u32 op)
{

	if (pathname[0] != '/' ||
	    rule->listnr != AUDIT_FILTER_EXIT ||
	    op != Audit_equal ||
	    rule->inode_f || rule->watch || rule->tree)
		return -EINVAL;
	rule->tree = alloc_tree(pathname);
	if (!rule->tree)
		return -ENOMEM;
	return 0;
}

void audit_put_tree(struct audit_tree *tree)
{
	put_tree(tree);
}

static int tag_mount(struct vfsmount *mnt, void *arg)
{
	return tag_chunk(d_backing_inode(mnt->mnt_root), arg);
}

/*
 * That gets run when evict_chunk() ends up needing to kill audit_tree.
 * Runs from a separate thread.
 */
static int prune_tree_thread(void *unused)
{
	for (;;) {
		if (list_empty(&prune_list)) {
			set_current_state(TASK_INTERRUPTIBLE);
			schedule();
		}

		audit_ctl_lock();
		mutex_lock(&audit_filter_mutex);

		while (!list_empty(&prune_list)) {
			struct audit_tree *victim;

			victim = list_entry(prune_list.next,
					struct audit_tree, list);
			list_del_init(&victim->list);

			mutex_unlock(&audit_filter_mutex);

			prune_one(victim);

			mutex_lock(&audit_filter_mutex);
		}

		mutex_unlock(&audit_filter_mutex);
		audit_ctl_unlock();
	}
	return 0;
}

static int audit_launch_prune(void)
{
	if (prune_thread)
		return 0;
	prune_thread = kthread_run(prune_tree_thread, NULL,
				"audit_prune_tree");
	if (IS_ERR(prune_thread)) {
		pr_err("cannot start thread audit_prune_tree");
		prune_thread = NULL;
		return -ENOMEM;
	}
	return 0;
}

/* called with audit_filter_mutex */
int audit_add_tree_rule(struct audit_krule *rule)
{
	struct audit_tree *seed = rule->tree, *tree;
	struct path path;
	struct vfsmount *mnt;
	int err;

	rule->tree = NULL;
	list_for_each_entry(tree, &tree_list, list) {
		if (!strcmp(seed->pathname, tree->pathname)) {
			put_tree(seed);
			rule->tree = tree;
			list_add(&rule->rlist, &tree->rules);
			return 0;
		}
	}
	tree = seed;
	list_add(&tree->list, &tree_list);
	list_add(&rule->rlist, &tree->rules);
	/* do not set rule->tree yet */
	mutex_unlock(&audit_filter_mutex);

	if (unlikely(!prune_thread)) {
		err = audit_launch_prune();
		if (err)
			goto Err;
	}

	err = kern_path(tree->pathname, 0, &path);
	if (err)
		goto Err;
	mnt = collect_mounts(&path);
	path_put(&path);
	if (IS_ERR(mnt)) {
		err = PTR_ERR(mnt);
		goto Err;
	}

	get_tree(tree);
	err = iterate_mounts(tag_mount, tree, mnt);
	drop_collected_mounts(mnt);

	if (!err) {
		struct node *node;
		spin_lock(&hash_lock);
		list_for_each_entry(node, &tree->chunks, list)
			node->index &= ~(1U<<31);
		spin_unlock(&hash_lock);
	} else {
		trim_marked(tree);
		goto Err;
	}

	mutex_lock(&audit_filter_mutex);
	if (list_empty(&rule->rlist)) {
		put_tree(tree);
		return -ENOENT;
	}
	rule->tree = tree;
	put_tree(tree);

	return 0;
Err:
	mutex_lock(&audit_filter_mutex);
	list_del_init(&tree->list);
	list_del_init(&tree->rules);
	put_tree(tree);
	return err;
}

int audit_tag_tree(char *old, char *new)
{
	struct list_head cursor, barrier;
	int failed = 0;
	struct path path1, path2;
	struct vfsmount *tagged;
	int err;

	err = kern_path(new, 0, &path2);
	if (err)
		return err;
	tagged = collect_mounts(&path2);
	path_put(&path2);
	if (IS_ERR(tagged))
		return PTR_ERR(tagged);

	err = kern_path(old, 0, &path1);
	if (err) {
		drop_collected_mounts(tagged);
		return err;
	}

	mutex_lock(&audit_filter_mutex);
	list_add(&barrier, &tree_list);
	list_add(&cursor, &barrier);

	while (cursor.next != &tree_list) {
		struct audit_tree *tree;
		int good_one = 0;

		tree = container_of(cursor.next, struct audit_tree, list);
		get_tree(tree);
<<<<<<< HEAD
		list_del(&cursor);
		list_add(&cursor, &tree->list);
=======
		list_move(&cursor, &tree->list);
>>>>>>> upstream/android-13
		mutex_unlock(&audit_filter_mutex);

		err = kern_path(tree->pathname, 0, &path2);
		if (!err) {
			good_one = path_is_under(&path1, &path2);
			path_put(&path2);
		}

		if (!good_one) {
			put_tree(tree);
			mutex_lock(&audit_filter_mutex);
			continue;
		}

		failed = iterate_mounts(tag_mount, tree, tagged);
		if (failed) {
			put_tree(tree);
			mutex_lock(&audit_filter_mutex);
			break;
		}

		mutex_lock(&audit_filter_mutex);
		spin_lock(&hash_lock);
		if (!tree->goner) {
<<<<<<< HEAD
			list_del(&tree->list);
			list_add(&tree->list, &tree_list);
=======
			list_move(&tree->list, &tree_list);
>>>>>>> upstream/android-13
		}
		spin_unlock(&hash_lock);
		put_tree(tree);
	}

	while (barrier.prev != &tree_list) {
		struct audit_tree *tree;

		tree = container_of(barrier.prev, struct audit_tree, list);
		get_tree(tree);
<<<<<<< HEAD
		list_del(&tree->list);
		list_add(&tree->list, &barrier);
=======
		list_move(&tree->list, &barrier);
>>>>>>> upstream/android-13
		mutex_unlock(&audit_filter_mutex);

		if (!failed) {
			struct node *node;
			spin_lock(&hash_lock);
			list_for_each_entry(node, &tree->chunks, list)
				node->index &= ~(1U<<31);
			spin_unlock(&hash_lock);
		} else {
			trim_marked(tree);
		}

		put_tree(tree);
		mutex_lock(&audit_filter_mutex);
	}
	list_del(&barrier);
	list_del(&cursor);
	mutex_unlock(&audit_filter_mutex);
	path_put(&path1);
	drop_collected_mounts(tagged);
	return failed;
}


static void audit_schedule_prune(void)
{
	wake_up_process(prune_thread);
}

/*
 * ... and that one is done if evict_chunk() decides to delay until the end
 * of syscall.  Runs synchronously.
 */
<<<<<<< HEAD
void audit_kill_trees(struct list_head *list)
{
=======
void audit_kill_trees(struct audit_context *context)
{
	struct list_head *list = &context->killed_trees;

>>>>>>> upstream/android-13
	audit_ctl_lock();
	mutex_lock(&audit_filter_mutex);

	while (!list_empty(list)) {
		struct audit_tree *victim;

		victim = list_entry(list->next, struct audit_tree, list);
<<<<<<< HEAD
		kill_rules(victim);
=======
		kill_rules(context, victim);
>>>>>>> upstream/android-13
		list_del_init(&victim->list);

		mutex_unlock(&audit_filter_mutex);

		prune_one(victim);

		mutex_lock(&audit_filter_mutex);
	}

	mutex_unlock(&audit_filter_mutex);
	audit_ctl_unlock();
}

/*
 *  Here comes the stuff asynchronous to auditctl operations
 */

static void evict_chunk(struct audit_chunk *chunk)
{
	struct audit_tree *owner;
	struct list_head *postponed = audit_killed_trees();
	int need_prune = 0;
	int n;

<<<<<<< HEAD
	if (chunk->dead)
		return;

	chunk->dead = 1;
=======
>>>>>>> upstream/android-13
	mutex_lock(&audit_filter_mutex);
	spin_lock(&hash_lock);
	while (!list_empty(&chunk->trees)) {
		owner = list_entry(chunk->trees.next,
				   struct audit_tree, same_root);
		owner->goner = 1;
		owner->root = NULL;
		list_del_init(&owner->same_root);
		spin_unlock(&hash_lock);
		if (!postponed) {
<<<<<<< HEAD
			kill_rules(owner);
=======
			kill_rules(audit_context(), owner);
>>>>>>> upstream/android-13
			list_move(&owner->list, &prune_list);
			need_prune = 1;
		} else {
			list_move(&owner->list, postponed);
		}
		spin_lock(&hash_lock);
	}
	list_del_rcu(&chunk->hash);
	for (n = 0; n < chunk->count; n++)
		list_del_init(&chunk->owners[n].list);
	spin_unlock(&hash_lock);
	mutex_unlock(&audit_filter_mutex);
	if (need_prune)
		audit_schedule_prune();
}

<<<<<<< HEAD
static int audit_tree_handle_event(struct fsnotify_group *group,
				   struct inode *to_tell,
				   u32 mask, const void *data, int data_type,
				   const unsigned char *file_name, u32 cookie,
				   struct fsnotify_iter_info *iter_info)
=======
static int audit_tree_handle_event(struct fsnotify_mark *mark, u32 mask,
				   struct inode *inode, struct inode *dir,
				   const struct qstr *file_name, u32 cookie)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static void audit_tree_freeing_mark(struct fsnotify_mark *entry, struct fsnotify_group *group)
{
	struct audit_chunk *chunk = container_of(entry, struct audit_chunk, mark);

	evict_chunk(chunk);
=======
static void audit_tree_freeing_mark(struct fsnotify_mark *mark,
				    struct fsnotify_group *group)
{
	struct audit_chunk *chunk;

	mutex_lock(&mark->group->mark_mutex);
	spin_lock(&hash_lock);
	chunk = mark_chunk(mark);
	replace_mark_chunk(mark, NULL);
	spin_unlock(&hash_lock);
	mutex_unlock(&mark->group->mark_mutex);
	if (chunk) {
		evict_chunk(chunk);
		audit_mark_put_chunk(chunk);
	}
>>>>>>> upstream/android-13

	/*
	 * We are guaranteed to have at least one reference to the mark from
	 * either the inode or the caller of fsnotify_destroy_mark().
	 */
<<<<<<< HEAD
	BUG_ON(refcount_read(&entry->refcnt) < 1);
}

static const struct fsnotify_ops audit_tree_ops = {
	.handle_event = audit_tree_handle_event,
=======
	BUG_ON(refcount_read(&mark->refcnt) < 1);
}

static const struct fsnotify_ops audit_tree_ops = {
	.handle_inode_event = audit_tree_handle_event,
>>>>>>> upstream/android-13
	.freeing_mark = audit_tree_freeing_mark,
	.free_mark = audit_tree_destroy_watch,
};

static int __init audit_tree_init(void)
{
	int i;

<<<<<<< HEAD
=======
	audit_tree_mark_cachep = KMEM_CACHE(audit_tree_mark, SLAB_PANIC);

>>>>>>> upstream/android-13
	audit_tree_group = fsnotify_alloc_group(&audit_tree_ops);
	if (IS_ERR(audit_tree_group))
		audit_panic("cannot initialize fsnotify group for rectree watches");

	for (i = 0; i < HASH_SIZE; i++)
		INIT_LIST_HEAD(&chunk_hash_heads[i]);

	return 0;
}
__initcall(audit_tree_init);
