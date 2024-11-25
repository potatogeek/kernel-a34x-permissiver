

#ifndef _UVERBS_TYPES_
#define _UVERBS_TYPES_

#include <linux/kernel.h>
#include <rdma/ib_verbs.h>

struct uverbs_obj_type;
struct uverbs_api_object;

enum rdma_lookup_mode {
	UVERBS_LOOKUP_READ,
	UVERBS_LOOKUP_WRITE,
	
	UVERBS_LOOKUP_DESTROY,
};


struct uverbs_obj_type_class {
	struct ib_uobject *(*alloc_begin)(const struct uverbs_api_object *obj,
					  struct ib_uverbs_file *ufile);
	
	int (*alloc_commit)(struct ib_uobject *uobj);
	
	void (*alloc_abort)(struct ib_uobject *uobj);

	struct ib_uobject *(*lookup_get)(const struct uverbs_api_object *obj,
					 struct ib_uverbs_file *ufile, s64 id,
					 enum rdma_lookup_mode mode);
	void (*lookup_put)(struct ib_uobject *uobj, enum rdma_lookup_mode mode);
	
	int __must_check (*destroy_hw)(struct ib_uobject *uobj,
				       enum rdma_remove_reason why);
	void (*remove_handle)(struct ib_uobject *uobj);
	u8    needs_kfree_rcu;
};

struct uverbs_obj_type {
	const struct uverbs_obj_type_class * const type_class;
	size_t	     obj_size;
};



struct uverbs_obj_idr_type {
	
	struct uverbs_obj_type  type;

	
	int __must_check (*destroy_object)(struct ib_uobject *uobj,
					   enum rdma_remove_reason why);
};

struct ib_uobject *rdma_lookup_get_uobject(const struct uverbs_api_object *obj,
					   struct ib_uverbs_file *ufile, s64 id,
					   enum rdma_lookup_mode mode);
void rdma_lookup_put_uobject(struct ib_uobject *uobj,
			     enum rdma_lookup_mode mode);
struct ib_uobject *rdma_alloc_begin_uobject(const struct uverbs_api_object *obj,
					    struct ib_uverbs_file *ufile);
void rdma_alloc_abort_uobject(struct ib_uobject *uobj);
int __must_check rdma_alloc_commit_uobject(struct ib_uobject *uobj);

struct uverbs_obj_fd_type {
	
	struct uverbs_obj_type  type;
	int (*context_closed)(struct ib_uobject *uobj,
			      enum rdma_remove_reason why);
	const struct file_operations	*fops;
	const char			*name;
	int				flags;
};

extern const struct uverbs_obj_type_class uverbs_idr_class;
extern const struct uverbs_obj_type_class uverbs_fd_class;

#define UVERBS_BUILD_BUG_ON(cond) (sizeof(char[1 - 2 * !!(cond)]) -	\
				   sizeof(char))
#define UVERBS_TYPE_ALLOC_FD(_obj_size, _context_closed, _fops, _name, _flags)\
	((&((const struct uverbs_obj_fd_type)				\
	 {.type = {							\
		.type_class = &uverbs_fd_class,				\
		.obj_size = (_obj_size) +				\
			UVERBS_BUILD_BUG_ON((_obj_size) <               \
					    sizeof(struct ib_uobject)), \
	 },								\
	 .context_closed = _context_closed,				\
	 .fops = _fops,							\
	 .name = _name,							\
	 .flags = _flags}))->type)
#define UVERBS_TYPE_ALLOC_IDR_SZ(_size, _destroy_object)	\
	((&((const struct uverbs_obj_idr_type)				\
	 {.type = {							\
		.type_class = &uverbs_idr_class,			\
		.obj_size = (_size) +					\
			UVERBS_BUILD_BUG_ON((_size) <			\
					    sizeof(struct ib_uobject))	\
	 },								\
	 .destroy_object = _destroy_object,}))->type)
#define UVERBS_TYPE_ALLOC_IDR(_destroy_object)			\
	 UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_uobject),	\
				  _destroy_object)

#endif
