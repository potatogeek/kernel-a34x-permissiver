

#ifndef _UVERBS_IOCTL_
#define _UVERBS_IOCTL_

#include <rdma/uverbs_types.h>
#include <linux/uaccess.h>
#include <rdma/rdma_user_ioctl.h>
#include <rdma/ib_user_ioctl_verbs.h>
#include <rdma/ib_user_ioctl_cmds.h>



enum uverbs_attr_type {
	UVERBS_ATTR_TYPE_NA,
	UVERBS_ATTR_TYPE_PTR_IN,
	UVERBS_ATTR_TYPE_PTR_OUT,
	UVERBS_ATTR_TYPE_IDR,
	UVERBS_ATTR_TYPE_FD,
	UVERBS_ATTR_TYPE_ENUM_IN,
};

enum uverbs_obj_access {
	UVERBS_ACCESS_READ,
	UVERBS_ACCESS_WRITE,
	UVERBS_ACCESS_NEW,
	UVERBS_ACCESS_DESTROY
};



struct uverbs_attr_spec {
	u8 type;

	
	u8 zero_trailing:1;
	
	u8 alloc_and_copy:1;
	u8 mandatory:1;

	union {
		struct {
			
			u16 len;
			
			u16 min_len;
		} ptr;

		struct {
			
			u16 obj_type;
			u8 access;
		} obj;

		struct {
			u8 num_elems;
		} enum_def;
	} u;

	
	union {
		struct {
			
			const struct uverbs_attr_spec *ids;
		} enum_def;
	} u2;
};


enum uapi_radix_data {
	UVERBS_API_NS_FLAG = 1U << UVERBS_ID_NS_SHIFT,

	UVERBS_API_ATTR_KEY_BITS = 6,
	UVERBS_API_ATTR_KEY_MASK = GENMASK(UVERBS_API_ATTR_KEY_BITS - 1, 0),
	UVERBS_API_ATTR_BKEY_LEN = (1 << UVERBS_API_ATTR_KEY_BITS) - 1,

	UVERBS_API_METHOD_KEY_BITS = 5,
	UVERBS_API_METHOD_KEY_SHIFT = UVERBS_API_ATTR_KEY_BITS,
	UVERBS_API_METHOD_KEY_NUM_CORE = 24,
	UVERBS_API_METHOD_KEY_NUM_DRIVER = (1 << UVERBS_API_METHOD_KEY_BITS) -
					   UVERBS_API_METHOD_KEY_NUM_CORE,
	UVERBS_API_METHOD_KEY_MASK = GENMASK(
		UVERBS_API_METHOD_KEY_BITS + UVERBS_API_METHOD_KEY_SHIFT - 1,
		UVERBS_API_METHOD_KEY_SHIFT),

	UVERBS_API_OBJ_KEY_BITS = 5,
	UVERBS_API_OBJ_KEY_SHIFT =
		UVERBS_API_METHOD_KEY_BITS + UVERBS_API_METHOD_KEY_SHIFT,
	UVERBS_API_OBJ_KEY_NUM_CORE = 24,
	UVERBS_API_OBJ_KEY_NUM_DRIVER =
		(1 << UVERBS_API_OBJ_KEY_BITS) - UVERBS_API_OBJ_KEY_NUM_CORE,
	UVERBS_API_OBJ_KEY_MASK = GENMASK(31, UVERBS_API_OBJ_KEY_SHIFT),

	
	UVERBS_API_KEY_ERR = 0xFFFFFFFF,
};

static inline __attribute_const__ u32 uapi_key_obj(u32 id)
{
	if (id & UVERBS_API_NS_FLAG) {
		id &= ~UVERBS_API_NS_FLAG;
		if (id >= UVERBS_API_OBJ_KEY_NUM_DRIVER)
			return UVERBS_API_KEY_ERR;
		id = id + UVERBS_API_OBJ_KEY_NUM_CORE;
	} else {
		if (id >= UVERBS_API_OBJ_KEY_NUM_CORE)
			return UVERBS_API_KEY_ERR;
	}

	return id << UVERBS_API_OBJ_KEY_SHIFT;
}

static inline __attribute_const__ bool uapi_key_is_object(u32 key)
{
	return (key & ~UVERBS_API_OBJ_KEY_MASK) == 0;
}

static inline __attribute_const__ u32 uapi_key_ioctl_method(u32 id)
{
	if (id & UVERBS_API_NS_FLAG) {
		id &= ~UVERBS_API_NS_FLAG;
		if (id >= UVERBS_API_METHOD_KEY_NUM_DRIVER)
			return UVERBS_API_KEY_ERR;
		id = id + UVERBS_API_METHOD_KEY_NUM_CORE;
	} else {
		id++;
		if (id >= UVERBS_API_METHOD_KEY_NUM_CORE)
			return UVERBS_API_KEY_ERR;
	}

	return id << UVERBS_API_METHOD_KEY_SHIFT;
}

static inline __attribute_const__ u32 uapi_key_attr_to_method(u32 attr_key)
{
	return attr_key &
	       (UVERBS_API_OBJ_KEY_MASK | UVERBS_API_METHOD_KEY_MASK);
}

static inline __attribute_const__ bool uapi_key_is_ioctl_method(u32 key)
{
	return (key & UVERBS_API_METHOD_KEY_MASK) != 0 &&
	       (key & UVERBS_API_ATTR_KEY_MASK) == 0;
}

static inline __attribute_const__ u32 uapi_key_attrs_start(u32 ioctl_method_key)
{
	
	return ioctl_method_key + 1;
}

static inline __attribute_const__ u32 uapi_key_attr(u32 id)
{
	
	if (id & UVERBS_API_NS_FLAG) {
		id &= ~UVERBS_API_NS_FLAG;
		id++;
		if (id >= 1 << (UVERBS_API_ATTR_KEY_BITS - 1))
			return UVERBS_API_KEY_ERR;
		id = (id << 1) | 0;
	} else {
		if (id >= 1 << (UVERBS_API_ATTR_KEY_BITS - 1))
			return UVERBS_API_KEY_ERR;
		id = (id << 1) | 1;
	}

	return id;
}

static inline __attribute_const__ bool uapi_key_is_attr(u32 key)
{
	return (key & UVERBS_API_METHOD_KEY_MASK) != 0 &&
	       (key & UVERBS_API_ATTR_KEY_MASK) != 0;
}


static inline __attribute_const__ u32 uapi_bkey_attr(u32 attr_key)
{
	return attr_key - 1;
}



struct uverbs_attr_def {
	u16                           id;
	struct uverbs_attr_spec       attr;
};

struct uverbs_method_def {
	u16                                  id;
	
	u32				     flags;
	size_t				     num_attrs;
	const struct uverbs_attr_def * const (*attrs)[];
	int (*handler)(struct ib_uverbs_file *ufile,
		       struct uverbs_attr_bundle *ctx);
};

struct uverbs_object_def {
	u16					 id;
	const struct uverbs_obj_type	        *type_attrs;
	size_t				         num_methods;
	const struct uverbs_method_def * const (*methods)[];
};

struct uverbs_object_tree_def {
	size_t					 num_objects;
	const struct uverbs_object_def * const (*objects)[];
};



#define UVERBS_ATTR_SIZE(_min_len, _len)			\
	.u.ptr.min_len = _min_len, .u.ptr.len = _len

#define UVERBS_ATTR_NO_DATA() UVERBS_ATTR_SIZE(0, 0)


#define UVERBS_ATTR_TYPE(_type)					\
	.u.ptr.min_len = sizeof(_type), .u.ptr.len = sizeof(_type)

#define UVERBS_ATTR_STRUCT(_type, _last)                                       \
	.zero_trailing = 1,                                                    \
	UVERBS_ATTR_SIZE(((uintptr_t)(&((_type *)0)->_last + 1)),              \
			 sizeof(_type))

#define UVERBS_ATTR_MIN_SIZE(_min_len) UVERBS_ATTR_SIZE(_min_len, USHRT_MAX)


#define UA_ALLOC_AND_COPY .alloc_and_copy = 1
#define UA_MANDATORY .mandatory = 1
#define UA_OPTIONAL .mandatory = 0

#define UVERBS_ATTR_IDR(_attr_id, _idr_type, _access, ...)                     \
	(&(const struct uverbs_attr_def){                                      \
		.id = _attr_id,                                                \
		.attr = { .type = UVERBS_ATTR_TYPE_IDR,                        \
			  .u.obj.obj_type = _idr_type,                         \
			  .u.obj.access = _access,                             \
			  __VA_ARGS__ } })

#define UVERBS_ATTR_FD(_attr_id, _fd_type, _access, ...)                       \
	(&(const struct uverbs_attr_def){                                      \
		.id = (_attr_id) +                                             \
		      BUILD_BUG_ON_ZERO((_access) != UVERBS_ACCESS_NEW &&      \
					(_access) != UVERBS_ACCESS_READ),      \
		.attr = { .type = UVERBS_ATTR_TYPE_FD,                         \
			  .u.obj.obj_type = _fd_type,                          \
			  .u.obj.access = _access,                             \
			  __VA_ARGS__ } })

#define UVERBS_ATTR_PTR_IN(_attr_id, _type, ...)                               \
	(&(const struct uverbs_attr_def){                                      \
		.id = _attr_id,                                                \
		.attr = { .type = UVERBS_ATTR_TYPE_PTR_IN,                     \
			  _type,                                               \
			  __VA_ARGS__ } })

#define UVERBS_ATTR_PTR_OUT(_attr_id, _type, ...)                              \
	(&(const struct uverbs_attr_def){                                      \
		.id = _attr_id,                                                \
		.attr = { .type = UVERBS_ATTR_TYPE_PTR_OUT,                    \
			  _type,                                               \
			  __VA_ARGS__ } })


#define UVERBS_ATTR_ENUM_IN(_attr_id, _enum_arr, ...)                          \
	(&(const struct uverbs_attr_def){                                      \
		.id = _attr_id,                                                \
		.attr = { .type = UVERBS_ATTR_TYPE_ENUM_IN,                    \
			  .u2.enum_def.ids = _enum_arr,                        \
			  .u.enum_def.num_elems = ARRAY_SIZE(_enum_arr),       \
			  __VA_ARGS__ },                                       \
	})


#define UVERBS_ATTR_FLAGS_IN(_attr_id, _enum_type, ...)                        \
	UVERBS_ATTR_PTR_IN(                                                    \
		_attr_id,                                                      \
		UVERBS_ATTR_SIZE(sizeof(u32) + BUILD_BUG_ON_ZERO(              \
						       !sizeof(_enum_type *)), \
				 sizeof(u64)),                                 \
		__VA_ARGS__)


#define UVERBS_ATTR_UHW()                                                      \
	UVERBS_ATTR_PTR_IN(UVERBS_ATTR_UHW_IN,                                 \
			   UVERBS_ATTR_MIN_SIZE(0),			       \
			   UA_OPTIONAL),				       \
	UVERBS_ATTR_PTR_OUT(UVERBS_ATTR_UHW_OUT,                               \
			    UVERBS_ATTR_MIN_SIZE(0),			       \
			    UA_OPTIONAL)



#define DECLARE_UVERBS_OBJECT_TREE(_name, ...)                                 \
	static const struct uverbs_object_def *const _name##_ptr[] = {         \
		__VA_ARGS__,                                                   \
	};                                                                     \
	static const struct uverbs_object_tree_def _name = {                   \
		.num_objects = ARRAY_SIZE(_name##_ptr),                        \
		.objects = &_name##_ptr,                                       \
	}




struct uverbs_ptr_attr {
	
	union {
		void *ptr;
		u64 data;
	};
	u16		len;
	u16		uattr_idx;
	u8		enum_id;
};

struct uverbs_obj_attr {
	struct ib_uobject		*uobject;
	const struct uverbs_api_attr	*attr_elm;
};

struct uverbs_attr {
	union {
		struct uverbs_ptr_attr	ptr_attr;
		struct uverbs_obj_attr	obj_attr;
	};
};

struct uverbs_attr_bundle {
	struct ib_uverbs_file *ufile;
	DECLARE_BITMAP(attr_present, UVERBS_API_ATTR_BKEY_LEN);
	struct uverbs_attr attrs[];
};

static inline bool uverbs_attr_is_valid(const struct uverbs_attr_bundle *attrs_bundle,
					unsigned int idx)
{
	return test_bit(uapi_bkey_attr(uapi_key_attr(idx)),
			attrs_bundle->attr_present);
}

#define IS_UVERBS_COPY_ERR(_ret)		((_ret) && (_ret) != -ENOENT)

static inline const struct uverbs_attr *uverbs_attr_get(const struct uverbs_attr_bundle *attrs_bundle,
							u16 idx)
{
	if (!uverbs_attr_is_valid(attrs_bundle, idx))
		return ERR_PTR(-ENOENT);

	return &attrs_bundle->attrs[uapi_bkey_attr(uapi_key_attr(idx))];
}

static inline int uverbs_attr_get_enum_id(const struct uverbs_attr_bundle *attrs_bundle,
					  u16 idx)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);

	if (IS_ERR(attr))
		return PTR_ERR(attr);

	return attr->ptr_attr.enum_id;
}

static inline void *uverbs_attr_get_obj(const struct uverbs_attr_bundle *attrs_bundle,
					u16 idx)
{
	const struct uverbs_attr *attr;

	attr = uverbs_attr_get(attrs_bundle, idx);
	if (IS_ERR(attr))
		return ERR_CAST(attr);

	return attr->obj_attr.uobject->object;
}

static inline struct ib_uobject *uverbs_attr_get_uobject(const struct uverbs_attr_bundle *attrs_bundle,
							 u16 idx)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);

	if (IS_ERR(attr))
		return ERR_CAST(attr);

	return attr->obj_attr.uobject;
}

static inline int
uverbs_attr_get_len(const struct uverbs_attr_bundle *attrs_bundle, u16 idx)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);

	if (IS_ERR(attr))
		return PTR_ERR(attr);

	return attr->ptr_attr.len;
}

static inline bool uverbs_attr_ptr_is_inline(const struct uverbs_attr *attr)
{
	return attr->ptr_attr.len <= sizeof(attr->ptr_attr.data);
}

static inline void *uverbs_attr_get_alloced_ptr(
	const struct uverbs_attr_bundle *attrs_bundle, u16 idx)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);

	if (IS_ERR(attr))
		return (void *)attr;

	return uverbs_attr_ptr_is_inline(attr) ? (void *)&attr->ptr_attr.data :
						 attr->ptr_attr.ptr;
}

static inline int _uverbs_copy_from(void *to,
				    const struct uverbs_attr_bundle *attrs_bundle,
				    size_t idx,
				    size_t size)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);

	if (IS_ERR(attr))
		return PTR_ERR(attr);

	
	if (unlikely(size < attr->ptr_attr.len))
		return -EINVAL;

	if (uverbs_attr_ptr_is_inline(attr))
		memcpy(to, &attr->ptr_attr.data, attr->ptr_attr.len);
	else if (copy_from_user(to, u64_to_user_ptr(attr->ptr_attr.data),
				attr->ptr_attr.len))
		return -EFAULT;

	return 0;
}

static inline int _uverbs_copy_from_or_zero(void *to,
					    const struct uverbs_attr_bundle *attrs_bundle,
					    size_t idx,
					    size_t size)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);
	size_t min_size;

	if (IS_ERR(attr))
		return PTR_ERR(attr);

	min_size = min_t(size_t, size, attr->ptr_attr.len);

	if (uverbs_attr_ptr_is_inline(attr))
		memcpy(to, &attr->ptr_attr.data, min_size);
	else if (copy_from_user(to, u64_to_user_ptr(attr->ptr_attr.data),
				min_size))
		return -EFAULT;

	if (size > min_size)
		memset(to + min_size, 0, size - min_size);

	return 0;
}

#define uverbs_copy_from(to, attrs_bundle, idx)				      \
	_uverbs_copy_from(to, attrs_bundle, idx, sizeof(*to))

#define uverbs_copy_from_or_zero(to, attrs_bundle, idx)			      \
	_uverbs_copy_from_or_zero(to, attrs_bundle, idx, sizeof(*to))

#if IS_ENABLED(CONFIG_INFINIBAND_USER_ACCESS)
int uverbs_get_flags64(u64 *to, const struct uverbs_attr_bundle *attrs_bundle,
		       size_t idx, u64 allowed_bits);
int uverbs_get_flags32(u32 *to, const struct uverbs_attr_bundle *attrs_bundle,
		       size_t idx, u64 allowed_bits);
int uverbs_copy_to(const struct uverbs_attr_bundle *attrs_bundle, size_t idx,
		   const void *from, size_t size);
__malloc void *_uverbs_alloc(struct uverbs_attr_bundle *bundle, size_t size,
			     gfp_t flags);

static inline __malloc void *uverbs_alloc(struct uverbs_attr_bundle *bundle,
					  size_t size)
{
	return _uverbs_alloc(bundle, size, GFP_KERNEL);
}

static inline __malloc void *uverbs_zalloc(struct uverbs_attr_bundle *bundle,
					   size_t size)
{
	return _uverbs_alloc(bundle, size, GFP_KERNEL | __GFP_ZERO);
}
#else
static inline int
uverbs_get_flags64(u64 *to, const struct uverbs_attr_bundle *attrs_bundle,
		   size_t idx, u64 allowed_bits)
{
	return -EINVAL;
}
static inline int
uverbs_get_flags32(u32 *to, const struct uverbs_attr_bundle *attrs_bundle,
		   size_t idx, u64 allowed_bits)
{
	return -EINVAL;
}
static inline int uverbs_copy_to(const struct uverbs_attr_bundle *attrs_bundle,
				 size_t idx, const void *from, size_t size)
{
	return -EINVAL;
}
static inline __malloc void *uverbs_alloc(struct uverbs_attr_bundle *bundle,
					  size_t size)
{
	return ERR_PTR(-EINVAL);
}
static inline __malloc void *uverbs_zalloc(struct uverbs_attr_bundle *bundle,
					   size_t size)
{
	return ERR_PTR(-EINVAL);
}
#endif

#endif
