/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _FS_CEPH_OSDMAP_H
#define _FS_CEPH_OSDMAP_H

#include <linux/rbtree.h>
#include <linux/ceph/types.h>
#include <linux/ceph/decode.h>
#include <linux/crush/crush.h>


struct ceph_pg {
	uint64_t pool;
	uint32_t seed;
};

#define CEPH_SPG_NOSHARD	-1

struct ceph_spg {
	struct ceph_pg pgid;
	s8 shard;
};

int ceph_pg_compare(const struct ceph_pg *lhs, const struct ceph_pg *rhs);
int ceph_spg_compare(const struct ceph_spg *lhs, const struct ceph_spg *rhs);

#define CEPH_POOL_FLAG_HASHPSPOOL	(1ULL << 0) 
#define CEPH_POOL_FLAG_FULL		(1ULL << 1) 
#define CEPH_POOL_FLAG_FULL_QUOTA	(1ULL << 10) 
#define CEPH_POOL_FLAG_NEARFULL		(1ULL << 11) 

struct ceph_pg_pool_info {
	struct rb_node node;
	s64 id;
	u8 type; 
	u8 size;
	u8 min_size;
	u8 crush_ruleset;
	u8 object_hash;
	u32 last_force_request_resend;
	u32 pg_num, pgp_num;
	int pg_num_mask, pgp_num_mask;
	s64 read_tier;
	s64 write_tier; 
	u64 flags; 
	char *name;

	bool was_full;  
};

static inline bool ceph_can_shift_osds(struct ceph_pg_pool_info *pool)
{
	switch (pool->type) {
	case CEPH_POOL_TYPE_REP:
		return true;
	case CEPH_POOL_TYPE_EC:
		return false;
	default:
		BUG();
	}
}

struct ceph_object_locator {
	s64 pool;
	struct ceph_string *pool_ns;
};

static inline void ceph_oloc_init(struct ceph_object_locator *oloc)
{
	oloc->pool = -1;
	oloc->pool_ns = NULL;
}

static inline bool ceph_oloc_empty(const struct ceph_object_locator *oloc)
{
	return oloc->pool == -1;
}

void ceph_oloc_copy(struct ceph_object_locator *dest,
		    const struct ceph_object_locator *src);
void ceph_oloc_destroy(struct ceph_object_locator *oloc);


#define CEPH_OID_INLINE_LEN 52


struct ceph_object_id {
	char *name;
	char inline_name[CEPH_OID_INLINE_LEN];
	int name_len;
};

static inline void ceph_oid_init(struct ceph_object_id *oid)
{
	oid->name = oid->inline_name;
	oid->name_len = 0;
}

#define CEPH_OID_INIT_ONSTACK(oid)					\
    ({ ceph_oid_init(&oid); oid; })
#define CEPH_DEFINE_OID_ONSTACK(oid)					\
	struct ceph_object_id oid = CEPH_OID_INIT_ONSTACK(oid)

static inline bool ceph_oid_empty(const struct ceph_object_id *oid)
{
	return oid->name == oid->inline_name && !oid->name_len;
}

void ceph_oid_copy(struct ceph_object_id *dest,
		   const struct ceph_object_id *src);
__printf(2, 3)
void ceph_oid_printf(struct ceph_object_id *oid, const char *fmt, ...);
__printf(3, 4)
int ceph_oid_aprintf(struct ceph_object_id *oid, gfp_t gfp,
		     const char *fmt, ...);
void ceph_oid_destroy(struct ceph_object_id *oid);

struct ceph_pg_mapping {
	struct rb_node node;
	struct ceph_pg pgid;

	union {
		struct {
			int len;
			int osds[];
		} pg_temp, pg_upmap;
		struct {
			int osd;
		} primary_temp;
		struct {
			int len;
			int from_to[][2];
		} pg_upmap_items;
	};
};

struct ceph_osdmap {
	struct ceph_fsid fsid;
	u32 epoch;
	struct ceph_timespec created, modified;

	u32 flags;         

	u32 max_osd;       
	u32 *osd_state;    
	u32 *osd_weight;   
	struct ceph_entity_addr *osd_addr;

	struct rb_root pg_temp;
	struct rb_root primary_temp;

	
	struct rb_root pg_upmap;	
	struct rb_root pg_upmap_items;	

	u32 *osd_primary_affinity;

	struct rb_root pg_pools;
	u32 pool_max;

	
	struct crush_map *crush;

	struct mutex crush_workspace_mutex;
	void *crush_workspace;
};

static inline bool ceph_osd_exists(struct ceph_osdmap *map, int osd)
{
	return osd >= 0 && osd < map->max_osd &&
	       (map->osd_state[osd] & CEPH_OSD_EXISTS);
}

static inline bool ceph_osd_is_up(struct ceph_osdmap *map, int osd)
{
	return ceph_osd_exists(map, osd) &&
	       (map->osd_state[osd] & CEPH_OSD_UP);
}

static inline bool ceph_osd_is_down(struct ceph_osdmap *map, int osd)
{
	return !ceph_osd_is_up(map, osd);
}

char *ceph_osdmap_state_str(char *str, int len, u32 state);
extern u32 ceph_get_primary_affinity(struct ceph_osdmap *map, int osd);

static inline struct ceph_entity_addr *ceph_osd_addr(struct ceph_osdmap *map,
						     int osd)
{
	if (osd >= map->max_osd)
		return NULL;
	return &map->osd_addr[osd];
}

#define CEPH_PGID_ENCODING_LEN		(1 + 8 + 4 + 4)

static inline int ceph_decode_pgid(void **p, void *end, struct ceph_pg *pgid)
{
	__u8 version;

	if (!ceph_has_room(p, end, CEPH_PGID_ENCODING_LEN)) {
		pr_warn("incomplete pg encoding\n");
		return -EINVAL;
	}
	version = ceph_decode_8(p);
	if (version > 1) {
		pr_warn("do not understand pg encoding %d > 1\n",
			(int)version);
		return -EINVAL;
	}

	pgid->pool = ceph_decode_64(p);
	pgid->seed = ceph_decode_32(p);
	*p += 4;	

	return 0;
}

struct ceph_osdmap *ceph_osdmap_alloc(void);
extern struct ceph_osdmap *ceph_osdmap_decode(void **p, void *end);
struct ceph_osdmap *osdmap_apply_incremental(void **p, void *end,
					     struct ceph_osdmap *map);
extern void ceph_osdmap_destroy(struct ceph_osdmap *map);

struct ceph_osds {
	int osds[CEPH_PG_MAX_SIZE];
	int size;
	int primary; 
};

static inline void ceph_osds_init(struct ceph_osds *set)
{
	set->size = 0;
	set->primary = -1;
}

void ceph_osds_copy(struct ceph_osds *dest, const struct ceph_osds *src);

bool ceph_pg_is_split(const struct ceph_pg *pgid, u32 old_pg_num,
		      u32 new_pg_num);
bool ceph_is_new_interval(const struct ceph_osds *old_acting,
			  const struct ceph_osds *new_acting,
			  const struct ceph_osds *old_up,
			  const struct ceph_osds *new_up,
			  int old_size,
			  int new_size,
			  int old_min_size,
			  int new_min_size,
			  u32 old_pg_num,
			  u32 new_pg_num,
			  bool old_sort_bitwise,
			  bool new_sort_bitwise,
			  bool old_recovery_deletes,
			  bool new_recovery_deletes,
			  const struct ceph_pg *pgid);
bool ceph_osds_changed(const struct ceph_osds *old_acting,
		       const struct ceph_osds *new_acting,
		       bool any_change);

void __ceph_object_locator_to_pg(struct ceph_pg_pool_info *pi,
				 const struct ceph_object_id *oid,
				 const struct ceph_object_locator *oloc,
				 struct ceph_pg *raw_pgid);
int ceph_object_locator_to_pg(struct ceph_osdmap *osdmap,
			      const struct ceph_object_id *oid,
			      const struct ceph_object_locator *oloc,
			      struct ceph_pg *raw_pgid);

void ceph_pg_to_up_acting_osds(struct ceph_osdmap *osdmap,
			       struct ceph_pg_pool_info *pi,
			       const struct ceph_pg *raw_pgid,
			       struct ceph_osds *up,
			       struct ceph_osds *acting);
bool ceph_pg_to_primary_shard(struct ceph_osdmap *osdmap,
			      struct ceph_pg_pool_info *pi,
			      const struct ceph_pg *raw_pgid,
			      struct ceph_spg *spgid);
int ceph_pg_to_acting_primary(struct ceph_osdmap *osdmap,
			      const struct ceph_pg *raw_pgid);

extern struct ceph_pg_pool_info *ceph_pg_pool_by_id(struct ceph_osdmap *map,
						    u64 id);

extern const char *ceph_pg_pool_name_by_id(struct ceph_osdmap *map, u64 id);
extern int ceph_pg_poolid_by_name(struct ceph_osdmap *map, const char *name);
u64 ceph_pg_pool_flags(struct ceph_osdmap *map, u64 id);

#endif
