/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _FS_CEPH_OSD_CLIENT_H
#define _FS_CEPH_OSD_CLIENT_H

#include <linux/bitrev.h>
#include <linux/completion.h>
#include <linux/kref.h>
#include <linux/mempool.h>
#include <linux/rbtree.h>
#include <linux/refcount.h>

#include <linux/ceph/types.h>
#include <linux/ceph/osdmap.h>
#include <linux/ceph/messenger.h>
#include <linux/ceph/msgpool.h>
#include <linux/ceph/auth.h>
#include <linux/ceph/pagelist.h>

struct ceph_msg;
struct ceph_snap_context;
struct ceph_osd_request;
struct ceph_osd_client;


typedef void (*ceph_osdc_callback_t)(struct ceph_osd_request *);

#define CEPH_HOMELESS_OSD	-1


struct ceph_osd {
	refcount_t o_ref;
	struct ceph_osd_client *o_osdc;
	int o_osd;
	int o_incarnation;
	struct rb_node o_node;
	struct ceph_connection o_con;
	struct rb_root o_requests;
	struct rb_root o_linger_requests;
	struct rb_root o_backoff_mappings;
	struct rb_root o_backoffs_by_id;
	struct list_head o_osd_lru;
	struct ceph_auth_handshake o_auth;
	unsigned long lru_ttl;
	struct list_head o_keepalive_item;
	struct mutex lock;
};

#define CEPH_OSD_SLAB_OPS	2
#define CEPH_OSD_MAX_OPS	16

enum ceph_osd_data_type {
	CEPH_OSD_DATA_TYPE_NONE = 0,
	CEPH_OSD_DATA_TYPE_PAGES,
	CEPH_OSD_DATA_TYPE_PAGELIST,
#ifdef CONFIG_BLOCK
	CEPH_OSD_DATA_TYPE_BIO,
#endif 
	CEPH_OSD_DATA_TYPE_BVECS,
};

struct ceph_osd_data {
	enum ceph_osd_data_type	type;
	union {
		struct {
			struct page	**pages;
			u64		length;
			u32		alignment;
			bool		pages_from_pool;
			bool		own_pages;
		};
		struct ceph_pagelist	*pagelist;
#ifdef CONFIG_BLOCK
		struct {
			struct ceph_bio_iter	bio_pos;
			u32			bio_length;
		};
#endif 
		struct {
			struct ceph_bvec_iter	bvec_pos;
			u32			num_bvecs;
		};
	};
};

struct ceph_osd_req_op {
	u16 op;           
	u32 flags;        
	u32 indata_len;   
	u32 outdata_len;  
	s32 rval;

	union {
		struct ceph_osd_data raw_data_in;
		struct {
			u64 offset, length;
			u64 truncate_size;
			u32 truncate_seq;
			struct ceph_osd_data osd_data;
		} extent;
		struct {
			u32 name_len;
			u32 value_len;
			__u8 cmp_op;       
			__u8 cmp_mode;     
			struct ceph_osd_data osd_data;
		} xattr;
		struct {
			const char *class_name;
			const char *method_name;
			struct ceph_osd_data request_info;
			struct ceph_osd_data request_data;
			struct ceph_osd_data response_data;
			__u8 class_len;
			__u8 method_len;
			u32 indata_len;
		} cls;
		struct {
			u64 cookie;
			__u8 op;           
			u32 gen;
		} watch;
		struct {
			struct ceph_osd_data request_data;
		} notify_ack;
		struct {
			u64 cookie;
			struct ceph_osd_data request_data;
			struct ceph_osd_data response_data;
		} notify;
		struct {
			struct ceph_osd_data response_data;
		} list_watchers;
		struct {
			u64 expected_object_size;
			u64 expected_write_size;
		} alloc_hint;
	};
};

struct ceph_osd_request_target {
	struct ceph_object_id base_oid;
	struct ceph_object_locator base_oloc;
	struct ceph_object_id target_oid;
	struct ceph_object_locator target_oloc;

	struct ceph_pg pgid;               
	struct ceph_spg spgid;             
	u32 pg_num;
	u32 pg_num_mask;
	struct ceph_osds acting;
	struct ceph_osds up;
	int size;
	int min_size;
	bool sort_bitwise;
	bool recovery_deletes;

	unsigned int flags;                
	bool paused;

	u32 epoch;
	u32 last_force_resend;

	int osd;
};


struct ceph_osd_request {
	u64             r_tid;              
	struct rb_node  r_node;
	struct rb_node  r_mc_node;          
	struct work_struct r_complete_work;
	struct ceph_osd *r_osd;

	struct ceph_osd_request_target r_t;
#define r_base_oid	r_t.base_oid
#define r_base_oloc	r_t.base_oloc
#define r_flags		r_t.flags

	struct ceph_msg  *r_request, *r_reply;
	u32               r_sent;      

	
	unsigned int		r_num_ops;

	int               r_result;

	struct ceph_osd_client *r_osdc;
	struct kref       r_kref;
	bool              r_mempool;
	struct completion r_completion;       
	ceph_osdc_callback_t r_callback;
	struct list_head  r_unsafe_item;

	struct inode *r_inode;         	      
	void *r_priv;			      

	
	u64 r_snapid;                         
	struct ceph_snap_context *r_snapc;    
	struct timespec64 r_mtime;            
	u64 r_data_offset;                    
	bool r_linger;                        

	
	unsigned long r_stamp;                
	unsigned long r_start_stamp;          
	int r_attempts;
	u32 r_map_dne_bound;

	struct ceph_osd_req_op r_ops[];
};

struct ceph_request_redirect {
	struct ceph_object_locator oloc;
};


struct ceph_osd_reqid {
	struct ceph_entity_name name;
	__le64 tid;
	__le32 inc;
} __packed;

struct ceph_blkin_trace_info {
	__le64 trace_id;
	__le64 span_id;
	__le64 parent_span_id;
} __packed;

typedef void (*rados_watchcb2_t)(void *arg, u64 notify_id, u64 cookie,
				 u64 notifier_id, void *data, size_t data_len);
typedef void (*rados_watcherrcb_t)(void *arg, u64 cookie, int err);

struct ceph_osd_linger_request {
	struct ceph_osd_client *osdc;
	u64 linger_id;
	bool committed;
	bool is_watch;                  

	struct ceph_osd *osd;
	struct ceph_osd_request *reg_req;
	struct ceph_osd_request *ping_req;
	unsigned long ping_sent;
	unsigned long watch_valid_thru;
	struct list_head pending_lworks;

	struct ceph_osd_request_target t;
	u32 map_dne_bound;

	struct timespec64 mtime;

	struct kref kref;
	struct mutex lock;
	struct rb_node node;            
	struct rb_node osdc_node;       
	struct rb_node mc_node;         
	struct list_head scan_item;

	struct completion reg_commit_wait;
	struct completion notify_finish_wait;
	int reg_commit_error;
	int notify_finish_error;
	int last_error;

	u32 register_gen;
	u64 notify_id;

	rados_watchcb2_t wcb;
	rados_watcherrcb_t errcb;
	void *data;

	struct page ***preply_pages;
	size_t *preply_len;
};

struct ceph_watch_item {
	struct ceph_entity_name name;
	u64 cookie;
	struct ceph_entity_addr addr;
};

struct ceph_spg_mapping {
	struct rb_node node;
	struct ceph_spg spgid;

	struct rb_root backoffs;
};

struct ceph_hobject_id {
	void *key;
	size_t key_len;
	void *oid;
	size_t oid_len;
	u64 snapid;
	u32 hash;
	u8 is_max;
	void *nspace;
	size_t nspace_len;
	s64 pool;

	
	u32 hash_reverse_bits;
};

static inline void ceph_hoid_build_hash_cache(struct ceph_hobject_id *hoid)
{
	hoid->hash_reverse_bits = bitrev32(hoid->hash);
}


struct ceph_osd_backoff {
	struct rb_node spg_node;
	struct rb_node id_node;

	struct ceph_spg spgid;
	u64 id;
	struct ceph_hobject_id *begin;
	struct ceph_hobject_id *end;
};

#define CEPH_LINGER_ID_START	0xffff000000000000ULL

struct ceph_osd_client {
	struct ceph_client     *client;

	struct ceph_osdmap     *osdmap;       
	struct rw_semaphore    lock;

	struct rb_root         osds;          
	struct list_head       osd_lru;       
	spinlock_t             osd_lru_lock;
	u32		       epoch_barrier;
	struct ceph_osd        homeless_osd;
	atomic64_t             last_tid;      
	u64                    last_linger_id;
	struct rb_root         linger_requests; 
	struct rb_root         map_checks;
	struct rb_root         linger_map_checks;
	atomic_t               num_requests;
	atomic_t               num_homeless;
	bool                   abort_on_full; 
	int                    abort_err;
	struct delayed_work    timeout_work;
	struct delayed_work    osds_timeout_work;
#ifdef CONFIG_DEBUG_FS
	struct dentry 	       *debugfs_file;
#endif

	mempool_t              *req_mempool;

	struct ceph_msgpool	msgpool_op;
	struct ceph_msgpool	msgpool_op_reply;

	struct workqueue_struct	*notify_wq;
	struct workqueue_struct	*completion_wq;
};

static inline bool ceph_osdmap_flag(struct ceph_osd_client *osdc, int flag)
{
	return osdc->osdmap->flags & flag;
}

extern int ceph_osdc_setup(void);
extern void ceph_osdc_cleanup(void);

extern int ceph_osdc_init(struct ceph_osd_client *osdc,
			  struct ceph_client *client);
extern void ceph_osdc_stop(struct ceph_osd_client *osdc);

extern void ceph_osdc_handle_reply(struct ceph_osd_client *osdc,
				   struct ceph_msg *msg);
extern void ceph_osdc_handle_map(struct ceph_osd_client *osdc,
				 struct ceph_msg *msg);
void ceph_osdc_update_epoch_barrier(struct ceph_osd_client *osdc, u32 eb);
void ceph_osdc_abort_requests(struct ceph_osd_client *osdc, int err);

extern void osd_req_op_init(struct ceph_osd_request *osd_req,
			    unsigned int which, u16 opcode, u32 flags);

extern void osd_req_op_raw_data_in_pages(struct ceph_osd_request *,
					unsigned int which,
					struct page **pages, u64 length,
					u32 alignment, bool pages_from_pool,
					bool own_pages);

extern void osd_req_op_extent_init(struct ceph_osd_request *osd_req,
					unsigned int which, u16 opcode,
					u64 offset, u64 length,
					u64 truncate_size, u32 truncate_seq);
extern void osd_req_op_extent_update(struct ceph_osd_request *osd_req,
					unsigned int which, u64 length);
extern void osd_req_op_extent_dup_last(struct ceph_osd_request *osd_req,
				       unsigned int which, u64 offset_inc);

extern struct ceph_osd_data *osd_req_op_extent_osd_data(
					struct ceph_osd_request *osd_req,
					unsigned int which);

extern void osd_req_op_extent_osd_data_pages(struct ceph_osd_request *,
					unsigned int which,
					struct page **pages, u64 length,
					u32 alignment, bool pages_from_pool,
					bool own_pages);
extern void osd_req_op_extent_osd_data_pagelist(struct ceph_osd_request *,
					unsigned int which,
					struct ceph_pagelist *pagelist);
#ifdef CONFIG_BLOCK
void osd_req_op_extent_osd_data_bio(struct ceph_osd_request *osd_req,
				    unsigned int which,
				    struct ceph_bio_iter *bio_pos,
				    u32 bio_length);
#endif 
void osd_req_op_extent_osd_data_bvecs(struct ceph_osd_request *osd_req,
				      unsigned int which,
				      struct bio_vec *bvecs, u32 num_bvecs,
				      u32 bytes);
void osd_req_op_extent_osd_data_bvec_pos(struct ceph_osd_request *osd_req,
					 unsigned int which,
					 struct ceph_bvec_iter *bvec_pos);

extern void osd_req_op_cls_request_data_pagelist(struct ceph_osd_request *,
					unsigned int which,
					struct ceph_pagelist *pagelist);
extern void osd_req_op_cls_request_data_pages(struct ceph_osd_request *,
					unsigned int which,
					struct page **pages, u64 length,
					u32 alignment, bool pages_from_pool,
					bool own_pages);
void osd_req_op_cls_request_data_bvecs(struct ceph_osd_request *osd_req,
				       unsigned int which,
				       struct bio_vec *bvecs, u32 num_bvecs,
				       u32 bytes);
extern void osd_req_op_cls_response_data_pages(struct ceph_osd_request *,
					unsigned int which,
					struct page **pages, u64 length,
					u32 alignment, bool pages_from_pool,
					bool own_pages);
extern int osd_req_op_cls_init(struct ceph_osd_request *osd_req,
					unsigned int which, u16 opcode,
					const char *class, const char *method);
extern int osd_req_op_xattr_init(struct ceph_osd_request *osd_req, unsigned int which,
				 u16 opcode, const char *name, const void *value,
				 size_t size, u8 cmp_op, u8 cmp_mode);
extern void osd_req_op_alloc_hint_init(struct ceph_osd_request *osd_req,
				       unsigned int which,
				       u64 expected_object_size,
				       u64 expected_write_size);

extern struct ceph_osd_request *ceph_osdc_alloc_request(struct ceph_osd_client *osdc,
					       struct ceph_snap_context *snapc,
					       unsigned int num_ops,
					       bool use_mempool,
					       gfp_t gfp_flags);
int ceph_osdc_alloc_messages(struct ceph_osd_request *req, gfp_t gfp);

extern struct ceph_osd_request *ceph_osdc_new_request(struct ceph_osd_client *,
				      struct ceph_file_layout *layout,
				      struct ceph_vino vino,
				      u64 offset, u64 *len,
				      unsigned int which, int num_ops,
				      int opcode, int flags,
				      struct ceph_snap_context *snapc,
				      u32 truncate_seq, u64 truncate_size,
				      bool use_mempool);

extern void ceph_osdc_get_request(struct ceph_osd_request *req);
extern void ceph_osdc_put_request(struct ceph_osd_request *req);

extern int ceph_osdc_start_request(struct ceph_osd_client *osdc,
				   struct ceph_osd_request *req,
				   bool nofail);
extern void ceph_osdc_cancel_request(struct ceph_osd_request *req);
extern int ceph_osdc_wait_request(struct ceph_osd_client *osdc,
				  struct ceph_osd_request *req);
extern void ceph_osdc_sync(struct ceph_osd_client *osdc);

extern void ceph_osdc_flush_notifies(struct ceph_osd_client *osdc);
void ceph_osdc_maybe_request_map(struct ceph_osd_client *osdc);

int ceph_osdc_call(struct ceph_osd_client *osdc,
		   struct ceph_object_id *oid,
		   struct ceph_object_locator *oloc,
		   const char *class, const char *method,
		   unsigned int flags,
		   struct page *req_page, size_t req_len,
		   struct page *resp_page, size_t *resp_len);

extern int ceph_osdc_readpages(struct ceph_osd_client *osdc,
			       struct ceph_vino vino,
			       struct ceph_file_layout *layout,
			       u64 off, u64 *plen,
			       u32 truncate_seq, u64 truncate_size,
			       struct page **pages, int nr_pages,
			       int page_align);

extern int ceph_osdc_writepages(struct ceph_osd_client *osdc,
				struct ceph_vino vino,
				struct ceph_file_layout *layout,
				struct ceph_snap_context *sc,
				u64 off, u64 len,
				u32 truncate_seq, u64 truncate_size,
				struct timespec64 *mtime,
				struct page **pages, int nr_pages);


struct ceph_osd_linger_request *
ceph_osdc_watch(struct ceph_osd_client *osdc,
		struct ceph_object_id *oid,
		struct ceph_object_locator *oloc,
		rados_watchcb2_t wcb,
		rados_watcherrcb_t errcb,
		void *data);
int ceph_osdc_unwatch(struct ceph_osd_client *osdc,
		      struct ceph_osd_linger_request *lreq);

int ceph_osdc_notify_ack(struct ceph_osd_client *osdc,
			 struct ceph_object_id *oid,
			 struct ceph_object_locator *oloc,
			 u64 notify_id,
			 u64 cookie,
			 void *payload,
			 u32 payload_len);
int ceph_osdc_notify(struct ceph_osd_client *osdc,
		     struct ceph_object_id *oid,
		     struct ceph_object_locator *oloc,
		     void *payload,
		     u32 payload_len,
		     u32 timeout,
		     struct page ***preply_pages,
		     size_t *preply_len);
int ceph_osdc_watch_check(struct ceph_osd_client *osdc,
			  struct ceph_osd_linger_request *lreq);
int ceph_osdc_list_watchers(struct ceph_osd_client *osdc,
			    struct ceph_object_id *oid,
			    struct ceph_object_locator *oloc,
			    struct ceph_watch_item **watchers,
			    u32 *num_watchers);
#endif

