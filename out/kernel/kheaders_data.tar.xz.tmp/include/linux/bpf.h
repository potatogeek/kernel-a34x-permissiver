
#ifndef _LINUX_BPF_H
#define _LINUX_BPF_H 1

#include <uapi/linux/bpf.h>

#include <linux/workqueue.h>
#include <linux/file.h>
#include <linux/percpu.h>
#include <linux/err.h>
#include <linux/rbtree_latch.h>
#include <linux/numa.h>
#include <linux/wait.h>

struct bpf_verifier_env;
struct perf_event;
struct bpf_prog;
struct bpf_map;
struct sock;
struct seq_file;
struct btf_type;


struct bpf_map_ops {
	
	int (*map_alloc_check)(union bpf_attr *attr);
	struct bpf_map *(*map_alloc)(union bpf_attr *attr);
	void (*map_release)(struct bpf_map *map, struct file *map_file);
	void (*map_free)(struct bpf_map *map);
	int (*map_get_next_key)(struct bpf_map *map, void *key, void *next_key);
	void (*map_release_uref)(struct bpf_map *map);
	void *(*map_lookup_elem_sys_only)(struct bpf_map *map, void *key);

	
	void *(*map_lookup_elem)(struct bpf_map *map, void *key);
	int (*map_update_elem)(struct bpf_map *map, void *key, void *value, u64 flags);
	int (*map_delete_elem)(struct bpf_map *map, void *key);

	
	void *(*map_fd_get_ptr)(struct bpf_map *map, struct file *map_file,
				int fd);
	void (*map_fd_put_ptr)(void *ptr);
	u32 (*map_gen_lookup)(struct bpf_map *map, struct bpf_insn *insn_buf);
	u32 (*map_fd_sys_lookup_elem)(void *ptr);
	void (*map_seq_show_elem)(struct bpf_map *map, void *key,
				  struct seq_file *m);
	int (*map_check_btf)(const struct bpf_map *map,
			     const struct btf_type *key_type,
			     const struct btf_type *value_type);
};

struct bpf_map {
	
	const struct bpf_map_ops *ops ____cacheline_aligned;
	struct bpf_map *inner_map_meta;
#ifdef CONFIG_SECURITY
	void *security;
#endif
	enum bpf_map_type map_type;
	u32 key_size;
	u32 value_size;
	u32 max_entries;
	u32 map_flags;
	u32 pages;
	u32 id;
	int numa_node;
	u32 btf_key_type_id;
	u32 btf_value_type_id;
	struct btf *btf;
	bool unpriv_array;
	

	
	struct user_struct *user ____cacheline_aligned;
	atomic_t refcnt;
	atomic_t usercnt;
	struct work_struct work;
	char name[BPF_OBJ_NAME_LEN];
};

struct bpf_offload_dev;
struct bpf_offloaded_map;

struct bpf_map_dev_ops {
	int (*map_get_next_key)(struct bpf_offloaded_map *map,
				void *key, void *next_key);
	int (*map_lookup_elem)(struct bpf_offloaded_map *map,
			       void *key, void *value);
	int (*map_update_elem)(struct bpf_offloaded_map *map,
			       void *key, void *value, u64 flags);
	int (*map_delete_elem)(struct bpf_offloaded_map *map, void *key);
};

struct bpf_offloaded_map {
	struct bpf_map map;
	struct net_device *netdev;
	const struct bpf_map_dev_ops *dev_ops;
	void *dev_priv;
	struct list_head offloads;
};

static inline struct bpf_offloaded_map *map_to_offmap(struct bpf_map *map)
{
	return container_of(map, struct bpf_offloaded_map, map);
}

static inline bool bpf_map_offload_neutral(const struct bpf_map *map)
{
	return map->map_type == BPF_MAP_TYPE_PERF_EVENT_ARRAY;
}

static inline bool bpf_map_support_seq_show(const struct bpf_map *map)
{
	return map->btf && map->ops->map_seq_show_elem;
}

int map_check_no_btf(const struct bpf_map *map,
		     const struct btf_type *key_type,
		     const struct btf_type *value_type);

extern const struct bpf_map_ops bpf_map_offload_ops;


enum bpf_arg_type {
	ARG_DONTCARE = 0,	

	
	ARG_CONST_MAP_PTR,	
	ARG_PTR_TO_MAP_KEY,	
	ARG_PTR_TO_MAP_VALUE,	

	
	ARG_PTR_TO_MEM,		
	ARG_PTR_TO_MEM_OR_NULL, 
	ARG_PTR_TO_UNINIT_MEM,	

	ARG_CONST_SIZE,		
	ARG_CONST_SIZE_OR_ZERO,	

	ARG_PTR_TO_CTX,		
	ARG_ANYTHING,		
};


enum bpf_return_type {
	RET_INTEGER,			
	RET_VOID,			
	RET_PTR_TO_MAP_VALUE,		
	RET_PTR_TO_MAP_VALUE_OR_NULL,	
};


struct bpf_func_proto {
	u64 (*func)(u64 r1, u64 r2, u64 r3, u64 r4, u64 r5);
	bool gpl_only;
	bool pkt_access;
	enum bpf_return_type ret_type;
	enum bpf_arg_type arg1_type;
	enum bpf_arg_type arg2_type;
	enum bpf_arg_type arg3_type;
	enum bpf_arg_type arg4_type;
	enum bpf_arg_type arg5_type;
};


struct bpf_context;

enum bpf_access_type {
	BPF_READ = 1,
	BPF_WRITE = 2
};



enum bpf_reg_type {
	NOT_INIT = 0,		 
	SCALAR_VALUE,		 
	PTR_TO_CTX,		 
	CONST_PTR_TO_MAP,	 
	PTR_TO_MAP_VALUE,	 
	PTR_TO_MAP_VALUE_OR_NULL,
	PTR_TO_STACK,		 
	PTR_TO_PACKET_META,	 
	PTR_TO_PACKET,		 
	PTR_TO_PACKET_END,	 
};


struct bpf_insn_access_aux {
	enum bpf_reg_type reg_type;
	int ctx_field_size;
};

static inline void
bpf_ctx_record_field_size(struct bpf_insn_access_aux *aux, u32 size)
{
	aux->ctx_field_size = size;
}

struct bpf_prog_ops {
	int (*test_run)(struct bpf_prog *prog, const union bpf_attr *kattr,
			union bpf_attr __user *uattr);
};

struct bpf_verifier_ops {
	
	const struct bpf_func_proto *
	(*get_func_proto)(enum bpf_func_id func_id,
			  const struct bpf_prog *prog);

	
	bool (*is_valid_access)(int off, int size, enum bpf_access_type type,
				const struct bpf_prog *prog,
				struct bpf_insn_access_aux *info);
	int (*gen_prologue)(struct bpf_insn *insn, bool direct_write,
			    const struct bpf_prog *prog);
	int (*gen_ld_abs)(const struct bpf_insn *orig,
			  struct bpf_insn *insn_buf);
	u32 (*convert_ctx_access)(enum bpf_access_type type,
				  const struct bpf_insn *src,
				  struct bpf_insn *dst,
				  struct bpf_prog *prog, u32 *target_size);
};

struct bpf_prog_offload_ops {
	int (*insn_hook)(struct bpf_verifier_env *env,
			 int insn_idx, int prev_insn_idx);
};

struct bpf_prog_offload {
	struct bpf_prog		*prog;
	struct net_device	*netdev;
	void			*dev_priv;
	struct list_head	offloads;
	bool			dev_state;
	const struct bpf_prog_offload_ops *dev_ops;
	void			*jited_image;
	u32			jited_len;
};

struct bpf_prog_aux {
	atomic_t refcnt;
	u32 used_map_cnt;
	u32 max_ctx_offset;
	u32 stack_depth;
	u32 id;
	u32 func_cnt;
	bool offload_requested;
	struct bpf_prog **func;
	void *jit_data; 
	struct latch_tree_node ksym_tnode;
	struct list_head ksym_lnode;
	const struct bpf_prog_ops *ops;
	struct bpf_map **used_maps;
	struct bpf_prog *prog;
	struct user_struct *user;
	u64 load_time; 
	struct bpf_map *cgroup_storage;
	char name[BPF_OBJ_NAME_LEN];
#ifdef CONFIG_SECURITY
	void *security;
#endif
	struct bpf_prog_offload *offload;
	union {
		struct work_struct work;
		struct rcu_head	rcu;
	};
};

struct bpf_array {
	struct bpf_map map;
	u32 elem_size;
	u32 index_mask;
	
	enum bpf_prog_type owner_prog_type;
	bool owner_jited;
	union {
		char value[0] __aligned(8);
		void *ptrs[0] __aligned(8);
		void __percpu *pptrs[0] __aligned(8);
	};
};

#define MAX_TAIL_CALL_CNT 32

struct bpf_event_entry {
	struct perf_event *event;
	struct file *perf_file;
	struct file *map_file;
	struct rcu_head rcu;
};

bool bpf_prog_array_compatible(struct bpf_array *array, const struct bpf_prog *fp);
int bpf_prog_calc_tag(struct bpf_prog *fp);

const struct bpf_func_proto *bpf_get_trace_printk_proto(void);

typedef unsigned long (*bpf_ctx_copy_t)(void *dst, const void *src,
					unsigned long off, unsigned long len);

u64 bpf_event_output(struct bpf_map *map, u64 flags, void *meta, u64 meta_size,
		     void *ctx, u64 ctx_size, bpf_ctx_copy_t ctx_copy);

int bpf_prog_test_run_xdp(struct bpf_prog *prog, const union bpf_attr *kattr,
			  union bpf_attr __user *uattr);
int bpf_prog_test_run_skb(struct bpf_prog *prog, const union bpf_attr *kattr,
			  union bpf_attr __user *uattr);


struct bpf_prog_array_item {
	struct bpf_prog *prog;
	struct bpf_cgroup_storage *cgroup_storage;
};

struct bpf_prog_array {
	struct rcu_head rcu;
	struct bpf_prog_array_item items[0];
};

struct bpf_prog_array *bpf_prog_array_alloc(u32 prog_cnt, gfp_t flags);
void bpf_prog_array_free(struct bpf_prog_array __rcu *progs);
int bpf_prog_array_length(struct bpf_prog_array __rcu *progs);
int bpf_prog_array_copy_to_user(struct bpf_prog_array __rcu *progs,
				__u32 __user *prog_ids, u32 cnt);

void bpf_prog_array_delete_safe(struct bpf_prog_array __rcu *progs,
				struct bpf_prog *old_prog);
int bpf_prog_array_copy_info(struct bpf_prog_array __rcu *array,
			     u32 *prog_ids, u32 request_cnt,
			     u32 *prog_cnt);
int bpf_prog_array_copy(struct bpf_prog_array __rcu *old_array,
			struct bpf_prog *exclude_prog,
			struct bpf_prog *include_prog,
			struct bpf_prog_array **new_array);

#define __BPF_PROG_RUN_ARRAY(array, ctx, func, check_non_null, set_cg_storage) \
	({						\
		struct bpf_prog_array_item *_item;	\
		struct bpf_prog *_prog;			\
		struct bpf_prog_array *_array;		\
		u32 _ret = 1;				\
		preempt_disable();			\
		rcu_read_lock();			\
		_array = rcu_dereference(array);	\
		if (unlikely(check_non_null && !_array))\
			goto _out;			\
		_item = &_array->items[0];		\
		while ((_prog = READ_ONCE(_item->prog))) {		\
			if (set_cg_storage)		\
				bpf_cgroup_storage_set(_item->cgroup_storage);	\
			_ret &= func(_prog, ctx);	\
			_item++;			\
		}					\
_out:							\
		rcu_read_unlock();			\
		preempt_enable();			\
		_ret;					\
	 })

#define BPF_PROG_RUN_ARRAY(array, ctx, func)		\
	__BPF_PROG_RUN_ARRAY(array, ctx, func, false, true)

#define BPF_PROG_RUN_ARRAY_CHECK(array, ctx, func)	\
	__BPF_PROG_RUN_ARRAY(array, ctx, func, true, false)

#ifdef CONFIG_BPF_SYSCALL
DECLARE_PER_CPU(int, bpf_prog_active);

extern const struct file_operations bpf_map_fops;
extern const struct file_operations bpf_prog_fops;

#define BPF_PROG_TYPE(_id, _name) \
	extern const struct bpf_prog_ops _name ## _prog_ops; \
	extern const struct bpf_verifier_ops _name ## _verifier_ops;
#define BPF_MAP_TYPE(_id, _ops) \
	extern const struct bpf_map_ops _ops;
#include <linux/bpf_types.h>
#undef BPF_PROG_TYPE
#undef BPF_MAP_TYPE

extern const struct bpf_prog_ops bpf_offload_prog_ops;
extern const struct bpf_verifier_ops tc_cls_act_analyzer_ops;
extern const struct bpf_verifier_ops xdp_analyzer_ops;

struct bpf_prog *bpf_prog_get(u32 ufd);
struct bpf_prog *bpf_prog_get_type_dev(u32 ufd, enum bpf_prog_type type,
				       bool attach_drv);
struct bpf_prog * __must_check bpf_prog_add(struct bpf_prog *prog, int i);
void bpf_prog_sub(struct bpf_prog *prog, int i);
struct bpf_prog * __must_check bpf_prog_inc(struct bpf_prog *prog);
struct bpf_prog * __must_check bpf_prog_inc_not_zero(struct bpf_prog *prog);
void bpf_prog_put(struct bpf_prog *prog);
int __bpf_prog_charge(struct user_struct *user, u32 pages);
void __bpf_prog_uncharge(struct user_struct *user, u32 pages);

void bpf_prog_free_id(struct bpf_prog *prog, bool do_idr_lock);
void bpf_map_free_id(struct bpf_map *map, bool do_idr_lock);

struct bpf_map *bpf_map_get_with_uref(u32 ufd);
struct bpf_map *__bpf_map_get(struct fd f);
struct bpf_map * __must_check bpf_map_inc(struct bpf_map *map, bool uref);
void bpf_map_put_with_uref(struct bpf_map *map);
void bpf_map_put(struct bpf_map *map);
int bpf_map_precharge_memlock(u32 pages);
int bpf_map_charge_memlock(struct bpf_map *map, u32 pages);
void bpf_map_uncharge_memlock(struct bpf_map *map, u32 pages);
void *bpf_map_area_alloc(size_t size, int numa_node);
void bpf_map_area_free(void *base);
void bpf_map_init_from_attr(struct bpf_map *map, union bpf_attr *attr);

extern int sysctl_unprivileged_bpf_disabled;

int bpf_map_new_fd(struct bpf_map *map, int flags);
int bpf_prog_new_fd(struct bpf_prog *prog);

int bpf_obj_pin_user(u32 ufd, const char __user *pathname);
int bpf_obj_get_user(const char __user *pathname, int flags);

int bpf_percpu_hash_copy(struct bpf_map *map, void *key, void *value);
int bpf_percpu_array_copy(struct bpf_map *map, void *key, void *value);
int bpf_percpu_hash_update(struct bpf_map *map, void *key, void *value,
			   u64 flags);
int bpf_percpu_array_update(struct bpf_map *map, void *key, void *value,
			    u64 flags);

int bpf_stackmap_copy(struct bpf_map *map, void *key, void *value);

int bpf_fd_array_map_update_elem(struct bpf_map *map, struct file *map_file,
				 void *key, void *value, u64 map_flags);
int bpf_fd_array_map_lookup_elem(struct bpf_map *map, void *key, u32 *value);
int bpf_fd_htab_map_update_elem(struct bpf_map *map, struct file *map_file,
				void *key, void *value, u64 map_flags);
int bpf_fd_htab_map_lookup_elem(struct bpf_map *map, void *key, u32 *value);

int bpf_get_file_flag(int flags);
int bpf_check_uarg_tail_zero(void __user *uaddr, size_t expected_size,
			     size_t actual_size);


static inline void bpf_long_memcpy(void *dst, const void *src, u32 size)
{
	const long *lsrc = src;
	long *ldst = dst;

	size /= sizeof(long);
	while (size--)
		*ldst++ = *lsrc++;
}


int bpf_check(struct bpf_prog **fp, union bpf_attr *attr);
void bpf_patch_call_args(struct bpf_insn *insn, u32 stack_depth);


struct xdp_buff;
struct sk_buff;

struct bpf_dtab_netdev *__dev_map_lookup_elem(struct bpf_map *map, u32 key);
void __dev_map_insert_ctx(struct bpf_map *map, u32 index);
void __dev_map_flush(struct bpf_map *map);
int dev_map_enqueue(struct bpf_dtab_netdev *dst, struct xdp_buff *xdp,
		    struct net_device *dev_rx);
int dev_map_generic_redirect(struct bpf_dtab_netdev *dst, struct sk_buff *skb,
			     struct bpf_prog *xdp_prog);

struct bpf_cpu_map_entry *__cpu_map_lookup_elem(struct bpf_map *map, u32 key);
void __cpu_map_insert_ctx(struct bpf_map *map, u32 index);
void __cpu_map_flush(struct bpf_map *map);
int cpu_map_enqueue(struct bpf_cpu_map_entry *rcpu, struct xdp_buff *xdp,
		    struct net_device *dev_rx);


static inline int bpf_map_attr_numa_node(const union bpf_attr *attr)
{
	return (attr->map_flags & BPF_F_NUMA_NODE) ?
		attr->numa_node : NUMA_NO_NODE;
}

struct bpf_prog *bpf_prog_get_type_path(const char *name, enum bpf_prog_type type);
int array_map_alloc_check(union bpf_attr *attr);

#else 
static inline struct bpf_prog *bpf_prog_get(u32 ufd)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline struct bpf_prog *bpf_prog_get_type_dev(u32 ufd,
						     enum bpf_prog_type type,
						     bool attach_drv)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline struct bpf_prog * __must_check bpf_prog_add(struct bpf_prog *prog,
							  int i)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void bpf_prog_sub(struct bpf_prog *prog, int i)
{
}

static inline void bpf_prog_put(struct bpf_prog *prog)
{
}

static inline struct bpf_prog * __must_check bpf_prog_inc(struct bpf_prog *prog)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline struct bpf_prog *__must_check
bpf_prog_inc_not_zero(struct bpf_prog *prog)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline int __bpf_prog_charge(struct user_struct *user, u32 pages)
{
	return 0;
}

static inline void __bpf_prog_uncharge(struct user_struct *user, u32 pages)
{
}

static inline int bpf_obj_get_user(const char __user *pathname, int flags)
{
	return -EOPNOTSUPP;
}

static inline struct net_device  *__dev_map_lookup_elem(struct bpf_map *map,
						       u32 key)
{
	return NULL;
}

static inline void __dev_map_insert_ctx(struct bpf_map *map, u32 index)
{
}

static inline void __dev_map_flush(struct bpf_map *map)
{
}

struct xdp_buff;
struct bpf_dtab_netdev;

static inline
int dev_map_enqueue(struct bpf_dtab_netdev *dst, struct xdp_buff *xdp,
		    struct net_device *dev_rx)
{
	return 0;
}

struct sk_buff;

static inline int dev_map_generic_redirect(struct bpf_dtab_netdev *dst,
					   struct sk_buff *skb,
					   struct bpf_prog *xdp_prog)
{
	return 0;
}

static inline
struct bpf_cpu_map_entry *__cpu_map_lookup_elem(struct bpf_map *map, u32 key)
{
	return NULL;
}

static inline void __cpu_map_insert_ctx(struct bpf_map *map, u32 index)
{
}

static inline void __cpu_map_flush(struct bpf_map *map)
{
}

static inline int cpu_map_enqueue(struct bpf_cpu_map_entry *rcpu,
				  struct xdp_buff *xdp,
				  struct net_device *dev_rx)
{
	return 0;
}

static inline struct bpf_prog *bpf_prog_get_type_path(const char *name,
				enum bpf_prog_type type)
{
	return ERR_PTR(-EOPNOTSUPP);
}
#endif 

static inline struct bpf_prog *bpf_prog_get_type(u32 ufd,
						 enum bpf_prog_type type)
{
	return bpf_prog_get_type_dev(ufd, type, false);
}

bool bpf_prog_get_ok(struct bpf_prog *, enum bpf_prog_type *, bool);

int bpf_prog_offload_compile(struct bpf_prog *prog);
void bpf_prog_offload_destroy(struct bpf_prog *prog);
int bpf_prog_offload_info_fill(struct bpf_prog_info *info,
			       struct bpf_prog *prog);

int bpf_map_offload_info_fill(struct bpf_map_info *info, struct bpf_map *map);

int bpf_map_offload_lookup_elem(struct bpf_map *map, void *key, void *value);
int bpf_map_offload_update_elem(struct bpf_map *map,
				void *key, void *value, u64 flags);
int bpf_map_offload_delete_elem(struct bpf_map *map, void *key);
int bpf_map_offload_get_next_key(struct bpf_map *map,
				 void *key, void *next_key);

bool bpf_offload_prog_map_match(struct bpf_prog *prog, struct bpf_map *map);

struct bpf_offload_dev *bpf_offload_dev_create(void);
void bpf_offload_dev_destroy(struct bpf_offload_dev *offdev);
int bpf_offload_dev_netdev_register(struct bpf_offload_dev *offdev,
				    struct net_device *netdev);
void bpf_offload_dev_netdev_unregister(struct bpf_offload_dev *offdev,
				       struct net_device *netdev);
bool bpf_offload_dev_match(struct bpf_prog *prog, struct net_device *netdev);

#if defined(CONFIG_NET) && defined(CONFIG_BPF_SYSCALL)
int bpf_prog_offload_init(struct bpf_prog *prog, union bpf_attr *attr);

static inline bool bpf_prog_is_dev_bound(const struct bpf_prog_aux *aux)
{
	return aux->offload_requested;
}

static inline bool bpf_map_is_dev_bound(struct bpf_map *map)
{
	return unlikely(map->ops == &bpf_map_offload_ops);
}

struct bpf_map *bpf_map_offload_map_alloc(union bpf_attr *attr);
void bpf_map_offload_map_free(struct bpf_map *map);
#else
static inline int bpf_prog_offload_init(struct bpf_prog *prog,
					union bpf_attr *attr)
{
	return -EOPNOTSUPP;
}

static inline bool bpf_prog_is_dev_bound(struct bpf_prog_aux *aux)
{
	return false;
}

static inline bool bpf_map_is_dev_bound(struct bpf_map *map)
{
	return false;
}

static inline struct bpf_map *bpf_map_offload_map_alloc(union bpf_attr *attr)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void bpf_map_offload_map_free(struct bpf_map *map)
{
}
#endif 

#if defined(CONFIG_STREAM_PARSER) && defined(CONFIG_BPF_SYSCALL) && defined(CONFIG_INET)
struct sock  *__sock_map_lookup_elem(struct bpf_map *map, u32 key);
struct sock  *__sock_hash_lookup_elem(struct bpf_map *map, void *key);
int sock_map_prog(struct bpf_map *map, struct bpf_prog *prog, u32 type);
int sockmap_get_from_fd(const union bpf_attr *attr, int type,
			struct bpf_prog *prog);
#else
static inline struct sock  *__sock_map_lookup_elem(struct bpf_map *map, u32 key)
{
	return NULL;
}

static inline struct sock  *__sock_hash_lookup_elem(struct bpf_map *map,
						    void *key)
{
	return NULL;
}

static inline int sock_map_prog(struct bpf_map *map,
				struct bpf_prog *prog,
				u32 type)
{
	return -EOPNOTSUPP;
}

static inline int sockmap_get_from_fd(const union bpf_attr *attr, int type,
				      struct bpf_prog *prog)
{
	return -EINVAL;
}
#endif

#if defined(CONFIG_XDP_SOCKETS)
struct xdp_sock;
struct xdp_sock *__xsk_map_lookup_elem(struct bpf_map *map, u32 key);
int __xsk_map_redirect(struct bpf_map *map, struct xdp_buff *xdp,
		       struct xdp_sock *xs);
void __xsk_map_flush(struct bpf_map *map);
#else
struct xdp_sock;
static inline struct xdp_sock *__xsk_map_lookup_elem(struct bpf_map *map,
						     u32 key)
{
	return NULL;
}

static inline int __xsk_map_redirect(struct bpf_map *map, struct xdp_buff *xdp,
				     struct xdp_sock *xs)
{
	return -EOPNOTSUPP;
}

static inline void __xsk_map_flush(struct bpf_map *map)
{
}
#endif

#if defined(CONFIG_INET) && defined(CONFIG_BPF_SYSCALL)
void bpf_sk_reuseport_detach(struct sock *sk);
int bpf_fd_reuseport_array_lookup_elem(struct bpf_map *map, void *key,
				       void *value);
int bpf_fd_reuseport_array_update_elem(struct bpf_map *map, void *key,
				       void *value, u64 map_flags);
#else
static inline void bpf_sk_reuseport_detach(struct sock *sk)
{
}

#ifdef CONFIG_BPF_SYSCALL
static inline int bpf_fd_reuseport_array_lookup_elem(struct bpf_map *map,
						     void *key, void *value)
{
	return -EOPNOTSUPP;
}

static inline int bpf_fd_reuseport_array_update_elem(struct bpf_map *map,
						     void *key, void *value,
						     u64 map_flags)
{
	return -EOPNOTSUPP;
}
#endif 
#endif 


extern const struct bpf_func_proto bpf_map_lookup_elem_proto;
extern const struct bpf_func_proto bpf_map_update_elem_proto;
extern const struct bpf_func_proto bpf_map_delete_elem_proto;

extern const struct bpf_func_proto bpf_get_prandom_u32_proto;
extern const struct bpf_func_proto bpf_get_smp_processor_id_proto;
extern const struct bpf_func_proto bpf_get_numa_node_id_proto;
extern const struct bpf_func_proto bpf_tail_call_proto;
extern const struct bpf_func_proto bpf_ktime_get_ns_proto;
extern const struct bpf_func_proto bpf_ktime_get_boot_ns_proto;
extern const struct bpf_func_proto bpf_get_current_pid_tgid_proto;
extern const struct bpf_func_proto bpf_get_current_uid_gid_proto;
extern const struct bpf_func_proto bpf_get_current_comm_proto;
extern const struct bpf_func_proto bpf_get_stackid_proto;
extern const struct bpf_func_proto bpf_get_stack_proto;
extern const struct bpf_func_proto bpf_sock_map_update_proto;
extern const struct bpf_func_proto bpf_sock_hash_update_proto;
extern const struct bpf_func_proto bpf_get_current_cgroup_id_proto;

extern const struct bpf_func_proto bpf_get_local_storage_proto;


void bpf_user_rnd_init_once(void);
u64 bpf_user_rnd_u32(u64 r1, u64 r2, u64 r3, u64 r4, u64 r5);

#endif 
