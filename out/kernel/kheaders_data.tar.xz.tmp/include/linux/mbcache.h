/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MBCACHE_H
#define _LINUX_MBCACHE_H

#include <linux/hash.h>
#include <linux/list_bl.h>
#include <linux/list.h>
#include <linux/atomic.h>
#include <linux/fs.h>

struct mb_cache;

struct mb_cache_entry {
	
	struct list_head	e_list;
	
	struct hlist_bl_node	e_hash_list;
	atomic_t		e_refcnt;
	
	u32			e_key;
	u32			e_referenced:1;
	u32			e_reusable:1;
	
	u64			e_value;
};

struct mb_cache *mb_cache_create(int bucket_bits);
void mb_cache_destroy(struct mb_cache *cache);

int mb_cache_entry_create(struct mb_cache *cache, gfp_t mask, u32 key,
			  u64 value, bool reusable);
void __mb_cache_entry_free(struct mb_cache_entry *entry);
static inline int mb_cache_entry_put(struct mb_cache *cache,
				     struct mb_cache_entry *entry)
{
	if (!atomic_dec_and_test(&entry->e_refcnt))
		return 0;
	__mb_cache_entry_free(entry);
	return 1;
}

void mb_cache_entry_delete(struct mb_cache *cache, u32 key, u64 value);
struct mb_cache_entry *mb_cache_entry_get(struct mb_cache *cache, u32 key,
					  u64 value);
struct mb_cache_entry *mb_cache_entry_find_first(struct mb_cache *cache,
						 u32 key);
struct mb_cache_entry *mb_cache_entry_find_next(struct mb_cache *cache,
						struct mb_cache_entry *entry);
void mb_cache_entry_touch(struct mb_cache *cache,
			  struct mb_cache_entry *entry);

#endif	
