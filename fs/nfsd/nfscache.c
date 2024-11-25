// SPDX-License-Identifier: GPL-2.0
/*
 * Request reply cache. This is currently a global cache, but this may
 * change in the future and be a per-client cache.
 *
 * This code is heavily inspired by the 44BSD implementation, although
 * it does things a bit differently.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

<<<<<<< HEAD
=======
#include <linux/sunrpc/svc_xprt.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/sunrpc/addr.h>
#include <linux/highmem.h>
#include <linux/log2.h>
#include <linux/hash.h>
#include <net/checksum.h>

#include "nfsd.h"
#include "cache.h"
<<<<<<< HEAD

#define NFSDDBG_FACILITY	NFSDDBG_REPCACHE
=======
#include "trace.h"
>>>>>>> upstream/android-13

/*
 * We use this value to determine the number of hash buckets from the max
 * cache size, the idea being that when the cache is at its maximum number
 * of entries, then this should be the average number of entries per bucket.
 */
#define TARGET_BUCKET_SIZE	64

struct nfsd_drc_bucket {
<<<<<<< HEAD
=======
	struct rb_root rb_head;
>>>>>>> upstream/android-13
	struct list_head lru_head;
	spinlock_t cache_lock;
};

<<<<<<< HEAD
static struct nfsd_drc_bucket	*drc_hashtbl;
static struct kmem_cache	*drc_slab;

/* max number of entries allowed in the cache */
static unsigned int		max_drc_entries;

/* number of significant bits in the hash value */
static unsigned int		maskbits;
static unsigned int		drc_hashsize;

/*
 * Stats and other tracking of on the duplicate reply cache. All of these and
 * the "rc" fields in nfsdstats are protected by the cache_lock
 */

/* total number of entries */
static atomic_t			num_drc_entries;

/* cache misses due only to checksum comparison failures */
static unsigned int		payload_misses;

/* amount of memory (in bytes) currently consumed by the DRC */
static unsigned int		drc_mem_usage;

/* longest hash chain seen */
static unsigned int		longest_chain;

/* size of cache when we saw the longest hash chain */
static unsigned int		longest_chain_cachesize;

=======
static struct kmem_cache	*drc_slab;

>>>>>>> upstream/android-13
static int	nfsd_cache_append(struct svc_rqst *rqstp, struct kvec *vec);
static unsigned long nfsd_reply_cache_count(struct shrinker *shrink,
					    struct shrink_control *sc);
static unsigned long nfsd_reply_cache_scan(struct shrinker *shrink,
					   struct shrink_control *sc);

<<<<<<< HEAD
static struct shrinker nfsd_reply_cache_shrinker = {
	.scan_objects = nfsd_reply_cache_scan,
	.count_objects = nfsd_reply_cache_count,
	.seeks	= 1,
};

=======
>>>>>>> upstream/android-13
/*
 * Put a cap on the size of the DRC based on the amount of available
 * low memory in the machine.
 *
 *  64MB:    8192
 * 128MB:   11585
 * 256MB:   16384
 * 512MB:   23170
 *   1GB:   32768
 *   2GB:   46340
 *   4GB:   65536
 *   8GB:   92681
 *  16GB:  131072
 *
 * ...with a hard cap of 256k entries. In the worst case, each entry will be
 * ~1k, so the above numbers should give a rough max of the amount of memory
 * used in k.
<<<<<<< HEAD
=======
 *
 * XXX: these limits are per-container, so memory used will increase
 * linearly with number of containers.  Maybe that's OK.
>>>>>>> upstream/android-13
 */
static unsigned int
nfsd_cache_size_limit(void)
{
	unsigned int limit;
<<<<<<< HEAD
	unsigned long low_pages = totalram_pages - totalhigh_pages;
=======
	unsigned long low_pages = totalram_pages() - totalhigh_pages();
>>>>>>> upstream/android-13

	limit = (16 * int_sqrt(low_pages)) << (PAGE_SHIFT-10);
	return min_t(unsigned int, limit, 256*1024);
}

/*
 * Compute the number of hash buckets we need. Divide the max cachesize by
 * the "target" max bucket size, and round up to next power of two.
 */
static unsigned int
nfsd_hashsize(unsigned int limit)
{
	return roundup_pow_of_two(limit / TARGET_BUCKET_SIZE);
}

static u32
<<<<<<< HEAD
nfsd_cache_hash(__be32 xid)
{
	return hash_32(be32_to_cpu(xid), maskbits);
}

static struct svc_cacherep *
nfsd_reply_cache_alloc(void)
=======
nfsd_cache_hash(__be32 xid, struct nfsd_net *nn)
{
	return hash_32(be32_to_cpu(xid), nn->maskbits);
}

static struct svc_cacherep *
nfsd_reply_cache_alloc(struct svc_rqst *rqstp, __wsum csum,
			struct nfsd_net *nn)
>>>>>>> upstream/android-13
{
	struct svc_cacherep	*rp;

	rp = kmem_cache_alloc(drc_slab, GFP_KERNEL);
	if (rp) {
		rp->c_state = RC_UNUSED;
		rp->c_type = RC_NOCACHE;
<<<<<<< HEAD
		INIT_LIST_HEAD(&rp->c_lru);
=======
		RB_CLEAR_NODE(&rp->c_node);
		INIT_LIST_HEAD(&rp->c_lru);

		memset(&rp->c_key, 0, sizeof(rp->c_key));
		rp->c_key.k_xid = rqstp->rq_xid;
		rp->c_key.k_proc = rqstp->rq_proc;
		rpc_copy_addr((struct sockaddr *)&rp->c_key.k_addr, svc_addr(rqstp));
		rpc_set_port((struct sockaddr *)&rp->c_key.k_addr, rpc_get_port(svc_addr(rqstp)));
		rp->c_key.k_prot = rqstp->rq_prot;
		rp->c_key.k_vers = rqstp->rq_vers;
		rp->c_key.k_len = rqstp->rq_arg.len;
		rp->c_key.k_csum = csum;
>>>>>>> upstream/android-13
	}
	return rp;
}

static void
<<<<<<< HEAD
nfsd_reply_cache_free_locked(struct svc_cacherep *rp)
{
	if (rp->c_type == RC_REPLBUFF && rp->c_replvec.iov_base) {
		drc_mem_usage -= rp->c_replvec.iov_len;
		kfree(rp->c_replvec.iov_base);
	}
	list_del(&rp->c_lru);
	atomic_dec(&num_drc_entries);
	drc_mem_usage -= sizeof(*rp);
=======
nfsd_reply_cache_free_locked(struct nfsd_drc_bucket *b, struct svc_cacherep *rp,
				struct nfsd_net *nn)
{
	if (rp->c_type == RC_REPLBUFF && rp->c_replvec.iov_base) {
		nfsd_stats_drc_mem_usage_sub(nn, rp->c_replvec.iov_len);
		kfree(rp->c_replvec.iov_base);
	}
	if (rp->c_state != RC_UNUSED) {
		rb_erase(&rp->c_node, &b->rb_head);
		list_del(&rp->c_lru);
		atomic_dec(&nn->num_drc_entries);
		nfsd_stats_drc_mem_usage_sub(nn, sizeof(*rp));
	}
>>>>>>> upstream/android-13
	kmem_cache_free(drc_slab, rp);
}

static void
<<<<<<< HEAD
nfsd_reply_cache_free(struct nfsd_drc_bucket *b, struct svc_cacherep *rp)
{
	spin_lock(&b->cache_lock);
	nfsd_reply_cache_free_locked(rp);
	spin_unlock(&b->cache_lock);
}

int nfsd_reply_cache_init(void)
=======
nfsd_reply_cache_free(struct nfsd_drc_bucket *b, struct svc_cacherep *rp,
			struct nfsd_net *nn)
{
	spin_lock(&b->cache_lock);
	nfsd_reply_cache_free_locked(b, rp, nn);
	spin_unlock(&b->cache_lock);
}

int nfsd_drc_slab_create(void)
{
	drc_slab = kmem_cache_create("nfsd_drc",
				sizeof(struct svc_cacherep), 0, 0, NULL);
	return drc_slab ? 0: -ENOMEM;
}

void nfsd_drc_slab_free(void)
{
	kmem_cache_destroy(drc_slab);
}

static int nfsd_reply_cache_stats_init(struct nfsd_net *nn)
{
	return nfsd_percpu_counters_init(nn->counter, NFSD_NET_COUNTERS_NUM);
}

static void nfsd_reply_cache_stats_destroy(struct nfsd_net *nn)
{
	nfsd_percpu_counters_destroy(nn->counter, NFSD_NET_COUNTERS_NUM);
}

int nfsd_reply_cache_init(struct nfsd_net *nn)
>>>>>>> upstream/android-13
{
	unsigned int hashsize;
	unsigned int i;
	int status = 0;

<<<<<<< HEAD
	max_drc_entries = nfsd_cache_size_limit();
	atomic_set(&num_drc_entries, 0);
	hashsize = nfsd_hashsize(max_drc_entries);
	maskbits = ilog2(hashsize);

	status = register_shrinker(&nfsd_reply_cache_shrinker);
	if (status)
		return status;

	drc_slab = kmem_cache_create("nfsd_drc", sizeof(struct svc_cacherep),
					0, 0, NULL);
	if (!drc_slab)
		goto out_nomem;

	drc_hashtbl = kcalloc(hashsize, sizeof(*drc_hashtbl), GFP_KERNEL);
	if (!drc_hashtbl) {
		drc_hashtbl = vzalloc(array_size(hashsize,
						 sizeof(*drc_hashtbl)));
		if (!drc_hashtbl)
			goto out_nomem;
	}

	for (i = 0; i < hashsize; i++) {
		INIT_LIST_HEAD(&drc_hashtbl[i].lru_head);
		spin_lock_init(&drc_hashtbl[i].cache_lock);
	}
	drc_hashsize = hashsize;

	return 0;
out_nomem:
	printk(KERN_ERR "nfsd: failed to allocate reply cache\n");
	nfsd_reply_cache_shutdown();
	return -ENOMEM;
}

void nfsd_reply_cache_shutdown(void)
=======
	nn->max_drc_entries = nfsd_cache_size_limit();
	atomic_set(&nn->num_drc_entries, 0);
	hashsize = nfsd_hashsize(nn->max_drc_entries);
	nn->maskbits = ilog2(hashsize);

	status = nfsd_reply_cache_stats_init(nn);
	if (status)
		goto out_nomem;

	nn->nfsd_reply_cache_shrinker.scan_objects = nfsd_reply_cache_scan;
	nn->nfsd_reply_cache_shrinker.count_objects = nfsd_reply_cache_count;
	nn->nfsd_reply_cache_shrinker.seeks = 1;
	status = register_shrinker(&nn->nfsd_reply_cache_shrinker);
	if (status)
		goto out_stats_destroy;

	nn->drc_hashtbl = kvzalloc(array_size(hashsize,
				sizeof(*nn->drc_hashtbl)), GFP_KERNEL);
	if (!nn->drc_hashtbl)
		goto out_shrinker;

	for (i = 0; i < hashsize; i++) {
		INIT_LIST_HEAD(&nn->drc_hashtbl[i].lru_head);
		spin_lock_init(&nn->drc_hashtbl[i].cache_lock);
	}
	nn->drc_hashsize = hashsize;

	return 0;
out_shrinker:
	unregister_shrinker(&nn->nfsd_reply_cache_shrinker);
out_stats_destroy:
	nfsd_reply_cache_stats_destroy(nn);
out_nomem:
	printk(KERN_ERR "nfsd: failed to allocate reply cache\n");
	return -ENOMEM;
}

void nfsd_reply_cache_shutdown(struct nfsd_net *nn)
>>>>>>> upstream/android-13
{
	struct svc_cacherep	*rp;
	unsigned int i;

<<<<<<< HEAD
	unregister_shrinker(&nfsd_reply_cache_shrinker);

	for (i = 0; i < drc_hashsize; i++) {
		struct list_head *head = &drc_hashtbl[i].lru_head;
		while (!list_empty(head)) {
			rp = list_first_entry(head, struct svc_cacherep, c_lru);
			nfsd_reply_cache_free_locked(rp);
		}
	}

	kvfree(drc_hashtbl);
	drc_hashtbl = NULL;
	drc_hashsize = 0;

	kmem_cache_destroy(drc_slab);
	drc_slab = NULL;
=======
	nfsd_reply_cache_stats_destroy(nn);
	unregister_shrinker(&nn->nfsd_reply_cache_shrinker);

	for (i = 0; i < nn->drc_hashsize; i++) {
		struct list_head *head = &nn->drc_hashtbl[i].lru_head;
		while (!list_empty(head)) {
			rp = list_first_entry(head, struct svc_cacherep, c_lru);
			nfsd_reply_cache_free_locked(&nn->drc_hashtbl[i],
									rp, nn);
		}
	}

	kvfree(nn->drc_hashtbl);
	nn->drc_hashtbl = NULL;
	nn->drc_hashsize = 0;

>>>>>>> upstream/android-13
}

/*
 * Move cache entry to end of LRU list, and queue the cleaner to run if it's
 * not already scheduled.
 */
static void
lru_put_end(struct nfsd_drc_bucket *b, struct svc_cacherep *rp)
{
	rp->c_timestamp = jiffies;
	list_move_tail(&rp->c_lru, &b->lru_head);
}

static long
<<<<<<< HEAD
prune_bucket(struct nfsd_drc_bucket *b)
=======
prune_bucket(struct nfsd_drc_bucket *b, struct nfsd_net *nn)
>>>>>>> upstream/android-13
{
	struct svc_cacherep *rp, *tmp;
	long freed = 0;

	list_for_each_entry_safe(rp, tmp, &b->lru_head, c_lru) {
		/*
		 * Don't free entries attached to calls that are still
		 * in-progress, but do keep scanning the list.
		 */
		if (rp->c_state == RC_INPROG)
			continue;
<<<<<<< HEAD
		if (atomic_read(&num_drc_entries) <= max_drc_entries &&
		    time_before(jiffies, rp->c_timestamp + RC_EXPIRE))
			break;
		nfsd_reply_cache_free_locked(rp);
=======
		if (atomic_read(&nn->num_drc_entries) <= nn->max_drc_entries &&
		    time_before(jiffies, rp->c_timestamp + RC_EXPIRE))
			break;
		nfsd_reply_cache_free_locked(b, rp, nn);
>>>>>>> upstream/android-13
		freed++;
	}
	return freed;
}

/*
 * Walk the LRU list and prune off entries that are older than RC_EXPIRE.
 * Also prune the oldest ones when the total exceeds the max number of entries.
 */
static long
<<<<<<< HEAD
prune_cache_entries(void)
=======
prune_cache_entries(struct nfsd_net *nn)
>>>>>>> upstream/android-13
{
	unsigned int i;
	long freed = 0;

<<<<<<< HEAD
	for (i = 0; i < drc_hashsize; i++) {
		struct nfsd_drc_bucket *b = &drc_hashtbl[i];
=======
	for (i = 0; i < nn->drc_hashsize; i++) {
		struct nfsd_drc_bucket *b = &nn->drc_hashtbl[i];
>>>>>>> upstream/android-13

		if (list_empty(&b->lru_head))
			continue;
		spin_lock(&b->cache_lock);
<<<<<<< HEAD
		freed += prune_bucket(b);
=======
		freed += prune_bucket(b, nn);
>>>>>>> upstream/android-13
		spin_unlock(&b->cache_lock);
	}
	return freed;
}

static unsigned long
nfsd_reply_cache_count(struct shrinker *shrink, struct shrink_control *sc)
{
<<<<<<< HEAD
	return atomic_read(&num_drc_entries);
=======
	struct nfsd_net *nn = container_of(shrink,
				struct nfsd_net, nfsd_reply_cache_shrinker);

	return atomic_read(&nn->num_drc_entries);
>>>>>>> upstream/android-13
}

static unsigned long
nfsd_reply_cache_scan(struct shrinker *shrink, struct shrink_control *sc)
{
<<<<<<< HEAD
	return prune_cache_entries();
=======
	struct nfsd_net *nn = container_of(shrink,
				struct nfsd_net, nfsd_reply_cache_shrinker);

	return prune_cache_entries(nn);
>>>>>>> upstream/android-13
}
/*
 * Walk an xdr_buf and get a CRC for at most the first RC_CSUMLEN bytes
 */
static __wsum
nfsd_cache_csum(struct svc_rqst *rqstp)
{
	int idx;
	unsigned int base;
	__wsum csum;
	struct xdr_buf *buf = &rqstp->rq_arg;
	const unsigned char *p = buf->head[0].iov_base;
	size_t csum_len = min_t(size_t, buf->head[0].iov_len + buf->page_len,
				RC_CSUMLEN);
	size_t len = min(buf->head[0].iov_len, csum_len);

	/* rq_arg.head first */
	csum = csum_partial(p, len, 0);
	csum_len -= len;

	/* Continue into page array */
	idx = buf->page_base / PAGE_SIZE;
	base = buf->page_base & ~PAGE_MASK;
	while (csum_len) {
		p = page_address(buf->pages[idx]) + base;
		len = min_t(size_t, PAGE_SIZE - base, csum_len);
		csum = csum_partial(p, len, csum);
		csum_len -= len;
		base = 0;
		++idx;
	}
	return csum;
}

<<<<<<< HEAD
static bool
nfsd_cache_match(struct svc_rqst *rqstp, __wsum csum, struct svc_cacherep *rp)
{
	/* Check RPC XID first */
	if (rqstp->rq_xid != rp->c_xid)
		return false;
	/* compare checksum of NFS data */
	if (csum != rp->c_csum) {
		++payload_misses;
		return false;
	}

	/* Other discriminators */
	if (rqstp->rq_proc != rp->c_proc ||
	    rqstp->rq_prot != rp->c_prot ||
	    rqstp->rq_vers != rp->c_vers ||
	    rqstp->rq_arg.len != rp->c_len ||
	    !rpc_cmp_addr(svc_addr(rqstp), (struct sockaddr *)&rp->c_addr) ||
	    rpc_get_port(svc_addr(rqstp)) != rpc_get_port((struct sockaddr *)&rp->c_addr))
		return false;

	return true;
=======
static int
nfsd_cache_key_cmp(const struct svc_cacherep *key,
			const struct svc_cacherep *rp, struct nfsd_net *nn)
{
	if (key->c_key.k_xid == rp->c_key.k_xid &&
	    key->c_key.k_csum != rp->c_key.k_csum) {
		nfsd_stats_payload_misses_inc(nn);
		trace_nfsd_drc_mismatch(nn, key, rp);
	}

	return memcmp(&key->c_key, &rp->c_key, sizeof(key->c_key));
>>>>>>> upstream/android-13
}

/*
 * Search the request hash for an entry that matches the given rqstp.
 * Must be called with cache_lock held. Returns the found entry or
<<<<<<< HEAD
 * NULL on failure.
 */
static struct svc_cacherep *
nfsd_cache_search(struct nfsd_drc_bucket *b, struct svc_rqst *rqstp,
		__wsum csum)
{
	struct svc_cacherep	*rp, *ret = NULL;
	struct list_head 	*rh = &b->lru_head;
	unsigned int		entries = 0;

	list_for_each_entry(rp, rh, c_lru) {
		++entries;
		if (nfsd_cache_match(rqstp, csum, rp)) {
			ret = rp;
			break;
		}
	}

	/* tally hash chain length stats */
	if (entries > longest_chain) {
		longest_chain = entries;
		longest_chain_cachesize = atomic_read(&num_drc_entries);
	} else if (entries == longest_chain) {
		/* prefer to keep the smallest cachesize possible here */
		longest_chain_cachesize = min_t(unsigned int,
				longest_chain_cachesize,
				atomic_read(&num_drc_entries));
	}

	return ret;
}

/*
=======
 * inserts an empty key on failure.
 */
static struct svc_cacherep *
nfsd_cache_insert(struct nfsd_drc_bucket *b, struct svc_cacherep *key,
			struct nfsd_net *nn)
{
	struct svc_cacherep	*rp, *ret = key;
	struct rb_node		**p = &b->rb_head.rb_node,
				*parent = NULL;
	unsigned int		entries = 0;
	int cmp;

	while (*p != NULL) {
		++entries;
		parent = *p;
		rp = rb_entry(parent, struct svc_cacherep, c_node);

		cmp = nfsd_cache_key_cmp(key, rp, nn);
		if (cmp < 0)
			p = &parent->rb_left;
		else if (cmp > 0)
			p = &parent->rb_right;
		else {
			ret = rp;
			goto out;
		}
	}
	rb_link_node(&key->c_node, parent, p);
	rb_insert_color(&key->c_node, &b->rb_head);
out:
	/* tally hash chain length stats */
	if (entries > nn->longest_chain) {
		nn->longest_chain = entries;
		nn->longest_chain_cachesize = atomic_read(&nn->num_drc_entries);
	} else if (entries == nn->longest_chain) {
		/* prefer to keep the smallest cachesize possible here */
		nn->longest_chain_cachesize = min_t(unsigned int,
				nn->longest_chain_cachesize,
				atomic_read(&nn->num_drc_entries));
	}

	lru_put_end(b, ret);
	return ret;
}

/**
 * nfsd_cache_lookup - Find an entry in the duplicate reply cache
 * @rqstp: Incoming Call to find
 *
>>>>>>> upstream/android-13
 * Try to find an entry matching the current call in the cache. When none
 * is found, we try to grab the oldest expired entry off the LRU list. If
 * a suitable one isn't there, then drop the cache_lock and allocate a
 * new one, then search again in case one got inserted while this thread
 * didn't hold the lock.
<<<<<<< HEAD
 */
int
nfsd_cache_lookup(struct svc_rqst *rqstp)
{
	struct svc_cacherep	*rp, *found;
	__be32			xid = rqstp->rq_xid;
	u32			proto =  rqstp->rq_prot,
				vers = rqstp->rq_vers,
				proc = rqstp->rq_proc;
	__wsum			csum;
	u32 hash = nfsd_cache_hash(xid);
	struct nfsd_drc_bucket *b = &drc_hashtbl[hash];
=======
 *
 * Return values:
 *   %RC_DOIT: Process the request normally
 *   %RC_REPLY: Reply from cache
 *   %RC_DROPIT: Do not process the request further
 */
int nfsd_cache_lookup(struct svc_rqst *rqstp)
{
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
	struct svc_cacherep	*rp, *found;
	__be32			xid = rqstp->rq_xid;
	__wsum			csum;
	u32 hash = nfsd_cache_hash(xid, nn);
	struct nfsd_drc_bucket *b = &nn->drc_hashtbl[hash];
>>>>>>> upstream/android-13
	int type = rqstp->rq_cachetype;
	int rtn = RC_DOIT;

	rqstp->rq_cacherep = NULL;
	if (type == RC_NOCACHE) {
<<<<<<< HEAD
		nfsdstats.rcnocache++;
		return rtn;
=======
		nfsd_stats_rc_nocache_inc();
		goto out;
>>>>>>> upstream/android-13
	}

	csum = nfsd_cache_csum(rqstp);

	/*
	 * Since the common case is a cache miss followed by an insert,
	 * preallocate an entry.
	 */
<<<<<<< HEAD
	rp = nfsd_reply_cache_alloc();
	spin_lock(&b->cache_lock);
	if (likely(rp)) {
		atomic_inc(&num_drc_entries);
		drc_mem_usage += sizeof(*rp);
	}

	/* go ahead and prune the cache */
	prune_bucket(b);

	found = nfsd_cache_search(b, rqstp, csum);
	if (found) {
		if (likely(rp))
			nfsd_reply_cache_free_locked(rp);
=======
	rp = nfsd_reply_cache_alloc(rqstp, csum, nn);
	if (!rp)
		goto out;

	spin_lock(&b->cache_lock);
	found = nfsd_cache_insert(b, rp, nn);
	if (found != rp) {
		nfsd_reply_cache_free_locked(NULL, rp, nn);
>>>>>>> upstream/android-13
		rp = found;
		goto found_entry;
	}

<<<<<<< HEAD
	if (!rp) {
		dprintk("nfsd: unable to allocate DRC entry!\n");
		goto out;
	}

	nfsdstats.rcmisses++;
	rqstp->rq_cacherep = rp;
	rp->c_state = RC_INPROG;
	rp->c_xid = xid;
	rp->c_proc = proc;
	rpc_copy_addr((struct sockaddr *)&rp->c_addr, svc_addr(rqstp));
	rpc_set_port((struct sockaddr *)&rp->c_addr, rpc_get_port(svc_addr(rqstp)));
	rp->c_prot = proto;
	rp->c_vers = vers;
	rp->c_len = rqstp->rq_arg.len;
	rp->c_csum = csum;

	lru_put_end(b, rp);

	/* release any buffer */
	if (rp->c_type == RC_REPLBUFF) {
		drc_mem_usage -= rp->c_replvec.iov_len;
		kfree(rp->c_replvec.iov_base);
		rp->c_replvec.iov_base = NULL;
	}
	rp->c_type = RC_NOCACHE;
 out:
	spin_unlock(&b->cache_lock);
	return rtn;

found_entry:
	nfsdstats.rchits++;
	/* We found a matching entry which is either in progress or done. */
	lru_put_end(b, rp);

	rtn = RC_DROPIT;
	/* Request being processed */
	if (rp->c_state == RC_INPROG)
		goto out;
=======
	nfsd_stats_rc_misses_inc();
	rqstp->rq_cacherep = rp;
	rp->c_state = RC_INPROG;

	atomic_inc(&nn->num_drc_entries);
	nfsd_stats_drc_mem_usage_add(nn, sizeof(*rp));

	/* go ahead and prune the cache */
	prune_bucket(b, nn);

out_unlock:
	spin_unlock(&b->cache_lock);
out:
	return rtn;

found_entry:
	/* We found a matching entry which is either in progress or done. */
	nfsd_stats_rc_hits_inc();
	rtn = RC_DROPIT;

	/* Request being processed */
	if (rp->c_state == RC_INPROG)
		goto out_trace;
>>>>>>> upstream/android-13

	/* From the hall of fame of impractical attacks:
	 * Is this a user who tries to snoop on the cache? */
	rtn = RC_DOIT;
	if (!test_bit(RQ_SECURE, &rqstp->rq_flags) && rp->c_secure)
<<<<<<< HEAD
		goto out;
=======
		goto out_trace;
>>>>>>> upstream/android-13

	/* Compose RPC reply header */
	switch (rp->c_type) {
	case RC_NOCACHE:
		break;
	case RC_REPLSTAT:
		svc_putu32(&rqstp->rq_res.head[0], rp->c_replstat);
		rtn = RC_REPLY;
		break;
	case RC_REPLBUFF:
		if (!nfsd_cache_append(rqstp, &rp->c_replvec))
<<<<<<< HEAD
			goto out;	/* should not happen */
		rtn = RC_REPLY;
		break;
	default:
		printk(KERN_WARNING "nfsd: bad repcache type %d\n", rp->c_type);
		nfsd_reply_cache_free_locked(rp);
	}

	goto out;
}

/*
 * Update a cache entry. This is called from nfsd_dispatch when
 * the procedure has been executed and the complete reply is in
 * rqstp->rq_res.
=======
			goto out_unlock; /* should not happen */
		rtn = RC_REPLY;
		break;
	default:
		WARN_ONCE(1, "nfsd: bad repcache type %d\n", rp->c_type);
	}

out_trace:
	trace_nfsd_drc_found(nn, rqstp, rtn);
	goto out_unlock;
}

/**
 * nfsd_cache_update - Update an entry in the duplicate reply cache.
 * @rqstp: svc_rqst with a finished Reply
 * @cachetype: which cache to update
 * @statp: Reply's status code
 *
 * This is called from nfsd_dispatch when the procedure has been
 * executed and the complete reply is in rqstp->rq_res.
>>>>>>> upstream/android-13
 *
 * We're copying around data here rather than swapping buffers because
 * the toplevel loop requires max-sized buffers, which would be a waste
 * of memory for a cache with a max reply size of 100 bytes (diropokres).
 *
 * If we should start to use different types of cache entries tailored
 * specifically for attrstat and fh's, we may save even more space.
 *
 * Also note that a cachetype of RC_NOCACHE can legally be passed when
 * nfsd failed to encode a reply that otherwise would have been cached.
 * In this case, nfsd_cache_update is called with statp == NULL.
 */
<<<<<<< HEAD
void
nfsd_cache_update(struct svc_rqst *rqstp, int cachetype, __be32 *statp)
{
=======
void nfsd_cache_update(struct svc_rqst *rqstp, int cachetype, __be32 *statp)
{
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
>>>>>>> upstream/android-13
	struct svc_cacherep *rp = rqstp->rq_cacherep;
	struct kvec	*resv = &rqstp->rq_res.head[0], *cachv;
	u32		hash;
	struct nfsd_drc_bucket *b;
	int		len;
	size_t		bufsize = 0;

	if (!rp)
		return;

<<<<<<< HEAD
	hash = nfsd_cache_hash(rp->c_xid);
	b = &drc_hashtbl[hash];
=======
	hash = nfsd_cache_hash(rp->c_key.k_xid, nn);
	b = &nn->drc_hashtbl[hash];
>>>>>>> upstream/android-13

	len = resv->iov_len - ((char*)statp - (char*)resv->iov_base);
	len >>= 2;

	/* Don't cache excessive amounts of data and XDR failures */
	if (!statp || len > (256 >> 2)) {
<<<<<<< HEAD
		nfsd_reply_cache_free(b, rp);
=======
		nfsd_reply_cache_free(b, rp, nn);
>>>>>>> upstream/android-13
		return;
	}

	switch (cachetype) {
	case RC_REPLSTAT:
		if (len != 1)
			printk("nfsd: RC_REPLSTAT/reply len %d!\n",len);
		rp->c_replstat = *statp;
		break;
	case RC_REPLBUFF:
		cachv = &rp->c_replvec;
		bufsize = len << 2;
		cachv->iov_base = kmalloc(bufsize, GFP_KERNEL);
		if (!cachv->iov_base) {
<<<<<<< HEAD
			nfsd_reply_cache_free(b, rp);
=======
			nfsd_reply_cache_free(b, rp, nn);
>>>>>>> upstream/android-13
			return;
		}
		cachv->iov_len = bufsize;
		memcpy(cachv->iov_base, statp, bufsize);
		break;
	case RC_NOCACHE:
<<<<<<< HEAD
		nfsd_reply_cache_free(b, rp);
		return;
	}
	spin_lock(&b->cache_lock);
	drc_mem_usage += bufsize;
=======
		nfsd_reply_cache_free(b, rp, nn);
		return;
	}
	spin_lock(&b->cache_lock);
	nfsd_stats_drc_mem_usage_add(nn, bufsize);
>>>>>>> upstream/android-13
	lru_put_end(b, rp);
	rp->c_secure = test_bit(RQ_SECURE, &rqstp->rq_flags);
	rp->c_type = cachetype;
	rp->c_state = RC_DONE;
	spin_unlock(&b->cache_lock);
	return;
}

/*
 * Copy cached reply to current reply buffer. Should always fit.
 * FIXME as reply is in a page, we should just attach the page, and
 * keep a refcount....
 */
static int
nfsd_cache_append(struct svc_rqst *rqstp, struct kvec *data)
{
	struct kvec	*vec = &rqstp->rq_res.head[0];

	if (vec->iov_len + data->iov_len > PAGE_SIZE) {
		printk(KERN_WARNING "nfsd: cached reply too large (%zd).\n",
				data->iov_len);
		return 0;
	}
	memcpy((char*)vec->iov_base + vec->iov_len, data->iov_base, data->iov_len);
	vec->iov_len += data->iov_len;
	return 1;
}

/*
 * Note that fields may be added, removed or reordered in the future. Programs
 * scraping this file for info should test the labels to ensure they're
 * getting the correct field.
 */
static int nfsd_reply_cache_stats_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
	seq_printf(m, "max entries:           %u\n", max_drc_entries);
	seq_printf(m, "num entries:           %u\n",
			atomic_read(&num_drc_entries));
	seq_printf(m, "hash buckets:          %u\n", 1 << maskbits);
	seq_printf(m, "mem usage:             %u\n", drc_mem_usage);
	seq_printf(m, "cache hits:            %u\n", nfsdstats.rchits);
	seq_printf(m, "cache misses:          %u\n", nfsdstats.rcmisses);
	seq_printf(m, "not cached:            %u\n", nfsdstats.rcnocache);
	seq_printf(m, "payload misses:        %u\n", payload_misses);
	seq_printf(m, "longest chain len:     %u\n", longest_chain);
	seq_printf(m, "cachesize at longest:  %u\n", longest_chain_cachesize);
=======
	struct nfsd_net *nn = m->private;

	seq_printf(m, "max entries:           %u\n", nn->max_drc_entries);
	seq_printf(m, "num entries:           %u\n",
		   atomic_read(&nn->num_drc_entries));
	seq_printf(m, "hash buckets:          %u\n", 1 << nn->maskbits);
	seq_printf(m, "mem usage:             %lld\n",
		   percpu_counter_sum_positive(&nn->counter[NFSD_NET_DRC_MEM_USAGE]));
	seq_printf(m, "cache hits:            %lld\n",
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_RC_HITS]));
	seq_printf(m, "cache misses:          %lld\n",
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_RC_MISSES]));
	seq_printf(m, "not cached:            %lld\n",
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_RC_NOCACHE]));
	seq_printf(m, "payload misses:        %lld\n",
		   percpu_counter_sum_positive(&nn->counter[NFSD_NET_PAYLOAD_MISSES]));
	seq_printf(m, "longest chain len:     %u\n", nn->longest_chain);
	seq_printf(m, "cachesize at longest:  %u\n", nn->longest_chain_cachesize);
>>>>>>> upstream/android-13
	return 0;
}

int nfsd_reply_cache_stats_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	return single_open(file, nfsd_reply_cache_stats_show, NULL);
=======
	struct nfsd_net *nn = net_generic(file_inode(file)->i_sb->s_fs_info,
								nfsd_net_id);

	return single_open(file, nfsd_reply_cache_stats_show, nn);
>>>>>>> upstream/android-13
}
