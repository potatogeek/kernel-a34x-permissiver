
#ifndef _NET_DEVLINK_H_
#define _NET_DEVLINK_H_

#include <linux/device.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/list.h>
#include <linux/netdevice.h>
#include <net/net_namespace.h>
#include <uapi/linux/devlink.h>

struct devlink_ops;

struct devlink {
	struct list_head list;
	struct list_head port_list;
	struct list_head sb_list;
	struct list_head dpipe_table_list;
	struct list_head resource_list;
	struct list_head param_list;
	struct list_head region_list;
	u32 snapshot_id;
	struct devlink_dpipe_headers *dpipe_headers;
	const struct devlink_ops *ops;
	struct device *dev;
	possible_net_t _net;
	struct mutex lock;
	char priv[0] __aligned(NETDEV_ALIGN);
};

struct devlink_port_attrs {
	bool set;
	enum devlink_port_flavour flavour;
	u32 port_number; 
	bool split;
	u32 split_subport_number;
};

struct devlink_port {
	struct list_head list;
	struct devlink *devlink;
	unsigned index;
	bool registered;
	enum devlink_port_type type;
	enum devlink_port_type desired_type;
	void *type_dev;
	struct devlink_port_attrs attrs;
};

struct devlink_sb_pool_info {
	enum devlink_sb_pool_type pool_type;
	u32 size;
	enum devlink_sb_threshold_type threshold_type;
};


struct devlink_dpipe_field {
	const char *name;
	unsigned int id;
	unsigned int bitwidth;
	enum devlink_dpipe_field_mapping_type mapping_type;
};


struct devlink_dpipe_header {
	const char *name;
	unsigned int id;
	struct devlink_dpipe_field *fields;
	unsigned int fields_count;
	bool global;
};


struct devlink_dpipe_match {
	enum devlink_dpipe_match_type type;
	unsigned int header_index;
	struct devlink_dpipe_header *header;
	unsigned int field_id;
};


struct devlink_dpipe_action {
	enum devlink_dpipe_action_type type;
	unsigned int header_index;
	struct devlink_dpipe_header *header;
	unsigned int field_id;
};


struct devlink_dpipe_value {
	union {
		struct devlink_dpipe_action *action;
		struct devlink_dpipe_match *match;
	};
	unsigned int mapping_value;
	bool mapping_valid;
	unsigned int value_size;
	void *value;
	void *mask;
};


struct devlink_dpipe_entry {
	u64 index;
	struct devlink_dpipe_value *match_values;
	unsigned int match_values_count;
	struct devlink_dpipe_value *action_values;
	unsigned int action_values_count;
	u64 counter;
	bool counter_valid;
};


struct devlink_dpipe_dump_ctx {
	struct genl_info *info;
	enum devlink_command cmd;
	struct sk_buff *skb;
	struct nlattr *nest;
	void *hdr;
};

struct devlink_dpipe_table_ops;


struct devlink_dpipe_table {
	void *priv;
	struct list_head list;
	const char *name;
	bool counters_enabled;
	bool counter_control_extern;
	bool resource_valid;
	u64 resource_id;
	u64 resource_units;
	struct devlink_dpipe_table_ops *table_ops;
	struct rcu_head rcu;
};


struct devlink_dpipe_table_ops {
	int (*actions_dump)(void *priv, struct sk_buff *skb);
	int (*matches_dump)(void *priv, struct sk_buff *skb);
	int (*entries_dump)(void *priv, bool counters_enabled,
			    struct devlink_dpipe_dump_ctx *dump_ctx);
	int (*counters_set_update)(void *priv, bool enable);
	u64 (*size_get)(void *priv);
};


struct devlink_dpipe_headers {
	struct devlink_dpipe_header **headers;
	unsigned int headers_count;
};


struct devlink_resource_size_params {
	u64 size_min;
	u64 size_max;
	u64 size_granularity;
	enum devlink_resource_unit unit;
};

static inline void
devlink_resource_size_params_init(struct devlink_resource_size_params *size_params,
				  u64 size_min, u64 size_max,
				  u64 size_granularity,
				  enum devlink_resource_unit unit)
{
	size_params->size_min = size_min;
	size_params->size_max = size_max;
	size_params->size_granularity = size_granularity;
	size_params->unit = unit;
}

typedef u64 devlink_resource_occ_get_t(void *priv);


struct devlink_resource {
	const char *name;
	u64 id;
	u64 size;
	u64 size_new;
	bool size_valid;
	struct devlink_resource *parent;
	struct devlink_resource_size_params size_params;
	struct list_head list;
	struct list_head resource_list;
	devlink_resource_occ_get_t *occ_get;
	void *occ_get_priv;
};

#define DEVLINK_RESOURCE_ID_PARENT_TOP 0

#define __DEVLINK_PARAM_MAX_STRING_VALUE 32
enum devlink_param_type {
	DEVLINK_PARAM_TYPE_U8,
	DEVLINK_PARAM_TYPE_U16,
	DEVLINK_PARAM_TYPE_U32,
	DEVLINK_PARAM_TYPE_STRING,
	DEVLINK_PARAM_TYPE_BOOL,
};

union devlink_param_value {
	u8 vu8;
	u16 vu16;
	u32 vu32;
	char vstr[__DEVLINK_PARAM_MAX_STRING_VALUE];
	bool vbool;
};

struct devlink_param_gset_ctx {
	union devlink_param_value val;
	enum devlink_param_cmode cmode;
};


struct devlink_param {
	u32 id;
	const char *name;
	bool generic;
	enum devlink_param_type type;
	unsigned long supported_cmodes;
	int (*get)(struct devlink *devlink, u32 id,
		   struct devlink_param_gset_ctx *ctx);
	int (*set)(struct devlink *devlink, u32 id,
		   struct devlink_param_gset_ctx *ctx);
	int (*validate)(struct devlink *devlink, u32 id,
			union devlink_param_value val,
			struct netlink_ext_ack *extack);
};

struct devlink_param_item {
	struct list_head list;
	const struct devlink_param *param;
	union devlink_param_value driverinit_value;
	bool driverinit_value_valid;
};

enum devlink_param_generic_id {
	DEVLINK_PARAM_GENERIC_ID_INT_ERR_RESET,
	DEVLINK_PARAM_GENERIC_ID_MAX_MACS,
	DEVLINK_PARAM_GENERIC_ID_ENABLE_SRIOV,
	DEVLINK_PARAM_GENERIC_ID_REGION_SNAPSHOT,

	
	__DEVLINK_PARAM_GENERIC_ID_MAX,
	DEVLINK_PARAM_GENERIC_ID_MAX = __DEVLINK_PARAM_GENERIC_ID_MAX - 1,
};

#define DEVLINK_PARAM_GENERIC_INT_ERR_RESET_NAME "internal_error_reset"
#define DEVLINK_PARAM_GENERIC_INT_ERR_RESET_TYPE DEVLINK_PARAM_TYPE_BOOL

#define DEVLINK_PARAM_GENERIC_MAX_MACS_NAME "max_macs"
#define DEVLINK_PARAM_GENERIC_MAX_MACS_TYPE DEVLINK_PARAM_TYPE_U32

#define DEVLINK_PARAM_GENERIC_ENABLE_SRIOV_NAME "enable_sriov"
#define DEVLINK_PARAM_GENERIC_ENABLE_SRIOV_TYPE DEVLINK_PARAM_TYPE_BOOL

#define DEVLINK_PARAM_GENERIC_REGION_SNAPSHOT_NAME "region_snapshot_enable"
#define DEVLINK_PARAM_GENERIC_REGION_SNAPSHOT_TYPE DEVLINK_PARAM_TYPE_BOOL

#define DEVLINK_PARAM_GENERIC(_id, _cmodes, _get, _set, _validate)	\
{									\
	.id = DEVLINK_PARAM_GENERIC_ID_##_id,				\
	.name = DEVLINK_PARAM_GENERIC_##_id##_NAME,			\
	.type = DEVLINK_PARAM_GENERIC_##_id##_TYPE,			\
	.generic = true,						\
	.supported_cmodes = _cmodes,					\
	.get = _get,							\
	.set = _set,							\
	.validate = _validate,						\
}

#define DEVLINK_PARAM_DRIVER(_id, _name, _type, _cmodes, _get, _set, _validate)	\
{									\
	.id = _id,							\
	.name = _name,							\
	.type = _type,							\
	.supported_cmodes = _cmodes,					\
	.get = _get,							\
	.set = _set,							\
	.validate = _validate,						\
}

struct devlink_region;

typedef void devlink_snapshot_data_dest_t(const void *data);

struct devlink_ops {
	int (*reload)(struct devlink *devlink, struct netlink_ext_ack *extack);
	int (*port_type_set)(struct devlink_port *devlink_port,
			     enum devlink_port_type port_type);
	int (*port_split)(struct devlink *devlink, unsigned int port_index,
			  unsigned int count, struct netlink_ext_ack *extack);
	int (*port_unsplit)(struct devlink *devlink, unsigned int port_index,
			    struct netlink_ext_ack *extack);
	int (*sb_pool_get)(struct devlink *devlink, unsigned int sb_index,
			   u16 pool_index,
			   struct devlink_sb_pool_info *pool_info);
	int (*sb_pool_set)(struct devlink *devlink, unsigned int sb_index,
			   u16 pool_index, u32 size,
			   enum devlink_sb_threshold_type threshold_type);
	int (*sb_port_pool_get)(struct devlink_port *devlink_port,
				unsigned int sb_index, u16 pool_index,
				u32 *p_threshold);
	int (*sb_port_pool_set)(struct devlink_port *devlink_port,
				unsigned int sb_index, u16 pool_index,
				u32 threshold);
	int (*sb_tc_pool_bind_get)(struct devlink_port *devlink_port,
				   unsigned int sb_index,
				   u16 tc_index,
				   enum devlink_sb_pool_type pool_type,
				   u16 *p_pool_index, u32 *p_threshold);
	int (*sb_tc_pool_bind_set)(struct devlink_port *devlink_port,
				   unsigned int sb_index,
				   u16 tc_index,
				   enum devlink_sb_pool_type pool_type,
				   u16 pool_index, u32 threshold);
	int (*sb_occ_snapshot)(struct devlink *devlink,
			       unsigned int sb_index);
	int (*sb_occ_max_clear)(struct devlink *devlink,
				unsigned int sb_index);
	int (*sb_occ_port_pool_get)(struct devlink_port *devlink_port,
				    unsigned int sb_index, u16 pool_index,
				    u32 *p_cur, u32 *p_max);
	int (*sb_occ_tc_port_bind_get)(struct devlink_port *devlink_port,
				       unsigned int sb_index,
				       u16 tc_index,
				       enum devlink_sb_pool_type pool_type,
				       u32 *p_cur, u32 *p_max);

	int (*eswitch_mode_get)(struct devlink *devlink, u16 *p_mode);
	int (*eswitch_mode_set)(struct devlink *devlink, u16 mode);
	int (*eswitch_inline_mode_get)(struct devlink *devlink, u8 *p_inline_mode);
	int (*eswitch_inline_mode_set)(struct devlink *devlink, u8 inline_mode);
	int (*eswitch_encap_mode_get)(struct devlink *devlink, u8 *p_encap_mode);
	int (*eswitch_encap_mode_set)(struct devlink *devlink, u8 encap_mode);
};

static inline void *devlink_priv(struct devlink *devlink)
{
	BUG_ON(!devlink);
	return &devlink->priv;
}

static inline struct devlink *priv_to_devlink(void *priv)
{
	BUG_ON(!priv);
	return container_of(priv, struct devlink, priv);
}

struct ib_device;

#if IS_ENABLED(CONFIG_NET_DEVLINK)

struct devlink *devlink_alloc(const struct devlink_ops *ops, size_t priv_size);
int devlink_register(struct devlink *devlink, struct device *dev);
void devlink_unregister(struct devlink *devlink);
void devlink_free(struct devlink *devlink);
int devlink_port_register(struct devlink *devlink,
			  struct devlink_port *devlink_port,
			  unsigned int port_index);
void devlink_port_unregister(struct devlink_port *devlink_port);
void devlink_port_type_eth_set(struct devlink_port *devlink_port,
			       struct net_device *netdev);
void devlink_port_type_ib_set(struct devlink_port *devlink_port,
			      struct ib_device *ibdev);
void devlink_port_type_clear(struct devlink_port *devlink_port);
void devlink_port_attrs_set(struct devlink_port *devlink_port,
			    enum devlink_port_flavour flavour,
			    u32 port_number, bool split,
			    u32 split_subport_number);
int devlink_port_get_phys_port_name(struct devlink_port *devlink_port,
				    char *name, size_t len);
int devlink_sb_register(struct devlink *devlink, unsigned int sb_index,
			u32 size, u16 ingress_pools_count,
			u16 egress_pools_count, u16 ingress_tc_count,
			u16 egress_tc_count);
void devlink_sb_unregister(struct devlink *devlink, unsigned int sb_index);
int devlink_dpipe_table_register(struct devlink *devlink,
				 const char *table_name,
				 struct devlink_dpipe_table_ops *table_ops,
				 void *priv, bool counter_control_extern);
void devlink_dpipe_table_unregister(struct devlink *devlink,
				    const char *table_name);
int devlink_dpipe_headers_register(struct devlink *devlink,
				   struct devlink_dpipe_headers *dpipe_headers);
void devlink_dpipe_headers_unregister(struct devlink *devlink);
bool devlink_dpipe_table_counter_enabled(struct devlink *devlink,
					 const char *table_name);
int devlink_dpipe_entry_ctx_prepare(struct devlink_dpipe_dump_ctx *dump_ctx);
int devlink_dpipe_entry_ctx_append(struct devlink_dpipe_dump_ctx *dump_ctx,
				   struct devlink_dpipe_entry *entry);
int devlink_dpipe_entry_ctx_close(struct devlink_dpipe_dump_ctx *dump_ctx);
void devlink_dpipe_entry_clear(struct devlink_dpipe_entry *entry);
int devlink_dpipe_action_put(struct sk_buff *skb,
			     struct devlink_dpipe_action *action);
int devlink_dpipe_match_put(struct sk_buff *skb,
			    struct devlink_dpipe_match *match);
extern struct devlink_dpipe_header devlink_dpipe_header_ethernet;
extern struct devlink_dpipe_header devlink_dpipe_header_ipv4;
extern struct devlink_dpipe_header devlink_dpipe_header_ipv6;

int devlink_resource_register(struct devlink *devlink,
			      const char *resource_name,
			      u64 resource_size,
			      u64 resource_id,
			      u64 parent_resource_id,
			      const struct devlink_resource_size_params *size_params);
void devlink_resources_unregister(struct devlink *devlink,
				  struct devlink_resource *resource);
int devlink_resource_size_get(struct devlink *devlink,
			      u64 resource_id,
			      u64 *p_resource_size);
int devlink_dpipe_table_resource_set(struct devlink *devlink,
				     const char *table_name, u64 resource_id,
				     u64 resource_units);
void devlink_resource_occ_get_register(struct devlink *devlink,
				       u64 resource_id,
				       devlink_resource_occ_get_t *occ_get,
				       void *occ_get_priv);
void devlink_resource_occ_get_unregister(struct devlink *devlink,
					 u64 resource_id);
int devlink_params_register(struct devlink *devlink,
			    const struct devlink_param *params,
			    size_t params_count);
void devlink_params_unregister(struct devlink *devlink,
			       const struct devlink_param *params,
			       size_t params_count);
int devlink_param_driverinit_value_get(struct devlink *devlink, u32 param_id,
				       union devlink_param_value *init_val);
int devlink_param_driverinit_value_set(struct devlink *devlink, u32 param_id,
				       union devlink_param_value init_val);
void devlink_param_value_changed(struct devlink *devlink, u32 param_id);
void devlink_param_value_str_fill(union devlink_param_value *dst_val,
				  const char *src);
struct devlink_region *devlink_region_create(struct devlink *devlink,
					     const char *region_name,
					     u32 region_max_snapshots,
					     u64 region_size);
void devlink_region_destroy(struct devlink_region *region);
u32 devlink_region_shapshot_id_get(struct devlink *devlink);
int devlink_region_snapshot_create(struct devlink_region *region, u64 data_len,
				   u8 *data, u32 snapshot_id,
				   devlink_snapshot_data_dest_t *data_destructor);

#else

static inline struct devlink *devlink_alloc(const struct devlink_ops *ops,
					    size_t priv_size)
{
	return kzalloc(sizeof(struct devlink) + priv_size, GFP_KERNEL);
}

static inline int devlink_register(struct devlink *devlink, struct device *dev)
{
	return 0;
}

static inline void devlink_unregister(struct devlink *devlink)
{
}

static inline void devlink_free(struct devlink *devlink)
{
	kfree(devlink);
}

static inline int devlink_port_register(struct devlink *devlink,
					struct devlink_port *devlink_port,
					unsigned int port_index)
{
	return 0;
}

static inline void devlink_port_unregister(struct devlink_port *devlink_port)
{
}

static inline void devlink_port_type_eth_set(struct devlink_port *devlink_port,
					     struct net_device *netdev)
{
}

static inline void devlink_port_type_ib_set(struct devlink_port *devlink_port,
					    struct ib_device *ibdev)
{
}

static inline void devlink_port_type_clear(struct devlink_port *devlink_port)
{
}

static inline void devlink_port_attrs_set(struct devlink_port *devlink_port,
					  enum devlink_port_flavour flavour,
					  u32 port_number, bool split,
					  u32 split_subport_number)
{
}

static inline int
devlink_port_get_phys_port_name(struct devlink_port *devlink_port,
				char *name, size_t len)
{
	return -EOPNOTSUPP;
}

static inline int devlink_sb_register(struct devlink *devlink,
				      unsigned int sb_index, u32 size,
				      u16 ingress_pools_count,
				      u16 egress_pools_count,
				      u16 ingress_tc_count,
				      u16 egress_tc_count)
{
	return 0;
}

static inline void devlink_sb_unregister(struct devlink *devlink,
					 unsigned int sb_index)
{
}

static inline int
devlink_dpipe_table_register(struct devlink *devlink,
			     const char *table_name,
			     struct devlink_dpipe_table_ops *table_ops,
			     void *priv, bool counter_control_extern)
{
	return 0;
}

static inline void devlink_dpipe_table_unregister(struct devlink *devlink,
						  const char *table_name)
{
}

static inline int devlink_dpipe_headers_register(struct devlink *devlink,
						 struct devlink_dpipe_headers *
						 dpipe_headers)
{
	return 0;
}

static inline void devlink_dpipe_headers_unregister(struct devlink *devlink)
{
}

static inline bool devlink_dpipe_table_counter_enabled(struct devlink *devlink,
						       const char *table_name)
{
	return false;
}

static inline int
devlink_dpipe_entry_ctx_prepare(struct devlink_dpipe_dump_ctx *dump_ctx)
{
	return 0;
}

static inline int
devlink_dpipe_entry_ctx_append(struct devlink_dpipe_dump_ctx *dump_ctx,
			       struct devlink_dpipe_entry *entry)
{
	return 0;
}

static inline int
devlink_dpipe_entry_ctx_close(struct devlink_dpipe_dump_ctx *dump_ctx)
{
	return 0;
}

static inline void
devlink_dpipe_entry_clear(struct devlink_dpipe_entry *entry)
{
}

static inline int
devlink_dpipe_action_put(struct sk_buff *skb,
			 struct devlink_dpipe_action *action)
{
	return 0;
}

static inline int
devlink_dpipe_match_put(struct sk_buff *skb,
			struct devlink_dpipe_match *match)
{
	return 0;
}

static inline int
devlink_resource_register(struct devlink *devlink,
			  const char *resource_name,
			  u64 resource_size,
			  u64 resource_id,
			  u64 parent_resource_id,
			  const struct devlink_resource_size_params *size_params)
{
	return 0;
}

static inline void
devlink_resources_unregister(struct devlink *devlink,
			     struct devlink_resource *resource)
{
}

static inline int
devlink_resource_size_get(struct devlink *devlink, u64 resource_id,
			  u64 *p_resource_size)
{
	return -EOPNOTSUPP;
}

static inline int
devlink_dpipe_table_resource_set(struct devlink *devlink,
				 const char *table_name, u64 resource_id,
				 u64 resource_units)
{
	return -EOPNOTSUPP;
}

static inline void
devlink_resource_occ_get_register(struct devlink *devlink,
				  u64 resource_id,
				  devlink_resource_occ_get_t *occ_get,
				  void *occ_get_priv)
{
}

static inline void
devlink_resource_occ_get_unregister(struct devlink *devlink,
				    u64 resource_id)
{
}

static inline int
devlink_params_register(struct devlink *devlink,
			const struct devlink_param *params,
			size_t params_count)
{
	return 0;
}

static inline void
devlink_params_unregister(struct devlink *devlink,
			  const struct devlink_param *params,
			  size_t params_count)
{

}

static inline int
devlink_param_driverinit_value_get(struct devlink *devlink, u32 param_id,
				   union devlink_param_value *init_val)
{
	return -EOPNOTSUPP;
}

static inline int
devlink_param_driverinit_value_set(struct devlink *devlink, u32 param_id,
				   union devlink_param_value init_val)
{
	return -EOPNOTSUPP;
}

static inline void
devlink_param_value_changed(struct devlink *devlink, u32 param_id)
{
}

static inline void
devlink_param_value_str_fill(union devlink_param_value *dst_val,
			     const char *src)
{
}

static inline struct devlink_region *
devlink_region_create(struct devlink *devlink,
		      const char *region_name,
		      u32 region_max_snapshots,
		      u64 region_size)
{
	return NULL;
}

static inline void
devlink_region_destroy(struct devlink_region *region)
{
}

static inline u32
devlink_region_shapshot_id_get(struct devlink *devlink)
{
	return 0;
}

static inline int
devlink_region_snapshot_create(struct devlink_region *region, u64 data_len,
			       u8 *data, u32 snapshot_id,
			       devlink_snapshot_data_dest_t *data_destructor)
{
	return 0;
}

#endif

#endif 
