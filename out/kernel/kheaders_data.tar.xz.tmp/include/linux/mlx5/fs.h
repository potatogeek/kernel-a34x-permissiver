

#ifndef _MLX5_FS_
#define _MLX5_FS_

#include <linux/mlx5/driver.h>
#include <linux/mlx5/mlx5_ifc.h>

#define MLX5_FS_DEFAULT_FLOW_TAG 0x0

enum {
	MLX5_FLOW_CONTEXT_ACTION_FWD_NEXT_PRIO	= 1 << 16,
	MLX5_FLOW_CONTEXT_ACTION_ENCRYPT	= 1 << 17,
	MLX5_FLOW_CONTEXT_ACTION_DECRYPT	= 1 << 18,
};

enum {
	MLX5_FLOW_TABLE_TUNNEL_EN = BIT(0),
};

#define LEFTOVERS_RULE_NUM	 2
static inline void build_leftovers_ft_param(int *priority,
					    int *n_ent,
					    int *n_grp)
{
	*priority = 0; 
	*n_ent = LEFTOVERS_RULE_NUM;
	*n_grp = LEFTOVERS_RULE_NUM;
}

enum mlx5_flow_namespace_type {
	MLX5_FLOW_NAMESPACE_BYPASS,
	MLX5_FLOW_NAMESPACE_LAG,
	MLX5_FLOW_NAMESPACE_OFFLOADS,
	MLX5_FLOW_NAMESPACE_ETHTOOL,
	MLX5_FLOW_NAMESPACE_KERNEL,
	MLX5_FLOW_NAMESPACE_LEFTOVERS,
	MLX5_FLOW_NAMESPACE_ANCHOR,
	MLX5_FLOW_NAMESPACE_FDB,
	MLX5_FLOW_NAMESPACE_ESW_EGRESS,
	MLX5_FLOW_NAMESPACE_ESW_INGRESS,
	MLX5_FLOW_NAMESPACE_SNIFFER_RX,
	MLX5_FLOW_NAMESPACE_SNIFFER_TX,
	MLX5_FLOW_NAMESPACE_EGRESS,
};

struct mlx5_flow_table;
struct mlx5_flow_group;
struct mlx5_flow_namespace;
struct mlx5_flow_handle;

struct mlx5_flow_spec {
	u8   match_criteria_enable;
	u32  match_criteria[MLX5_ST_SZ_DW(fte_match_param)];
	u32  match_value[MLX5_ST_SZ_DW(fte_match_param)];
};

struct mlx5_flow_destination {
	enum mlx5_flow_destination_type	type;
	union {
		u32			tir_num;
		u32			ft_num;
		struct mlx5_flow_table	*ft;
		struct mlx5_fc		*counter;
		struct {
			u16		num;
			u16		vhca_id;
			bool		vhca_id_valid;
		} vport;
	};
};

struct mlx5_flow_namespace *
mlx5_get_flow_namespace(struct mlx5_core_dev *dev,
			enum mlx5_flow_namespace_type type);
struct mlx5_flow_namespace *
mlx5_get_flow_vport_acl_namespace(struct mlx5_core_dev *dev,
				  enum mlx5_flow_namespace_type type,
				  int vport);

struct mlx5_flow_table *
mlx5_create_auto_grouped_flow_table(struct mlx5_flow_namespace *ns,
				    int prio,
				    int num_flow_table_entries,
				    int max_num_groups,
				    u32 level,
				    u32 flags);

struct mlx5_flow_table_attr {
	int prio;
	int max_fte;
	u32 level;
	u32 flags;
};

struct mlx5_flow_table *
mlx5_create_flow_table(struct mlx5_flow_namespace *ns,
		       struct mlx5_flow_table_attr *ft_attr);

struct mlx5_flow_table *
mlx5_create_vport_flow_table(struct mlx5_flow_namespace *ns,
			     int prio,
			     int num_flow_table_entries,
			     u32 level, u16 vport);
struct mlx5_flow_table *mlx5_create_lag_demux_flow_table(
					       struct mlx5_flow_namespace *ns,
					       int prio, u32 level);
int mlx5_destroy_flow_table(struct mlx5_flow_table *ft);


struct mlx5_flow_group *
mlx5_create_flow_group(struct mlx5_flow_table *ft, u32 *in);
void mlx5_destroy_flow_group(struct mlx5_flow_group *fg);

struct mlx5_fs_vlan {
        u16 ethtype;
        u16 vid;
        u8  prio;
};

#define MLX5_FS_VLAN_DEPTH	2

struct mlx5_flow_act {
	u32 action;
	bool has_flow_tag;
	u32 flow_tag;
	u32 encap_id;
	u32 modify_id;
	uintptr_t esp_id;
	struct mlx5_fs_vlan vlan[MLX5_FS_VLAN_DEPTH];
	struct ib_counters *counters;
};

#define MLX5_DECLARE_FLOW_ACT(name) \
	struct mlx5_flow_act name = {MLX5_FLOW_CONTEXT_ACTION_FWD_DEST,\
				     MLX5_FS_DEFAULT_FLOW_TAG, 0, 0}


struct mlx5_flow_handle *
mlx5_add_flow_rules(struct mlx5_flow_table *ft,
		    struct mlx5_flow_spec *spec,
		    struct mlx5_flow_act *flow_act,
		    struct mlx5_flow_destination *dest,
		    int num_dest);
void mlx5_del_flow_rules(struct mlx5_flow_handle *fr);

int mlx5_modify_rule_destination(struct mlx5_flow_handle *handler,
				 struct mlx5_flow_destination *new_dest,
				 struct mlx5_flow_destination *old_dest);

struct mlx5_fc *mlx5_flow_rule_counter(struct mlx5_flow_handle *handler);
struct mlx5_fc *mlx5_fc_create(struct mlx5_core_dev *dev, bool aging);
void mlx5_fc_destroy(struct mlx5_core_dev *dev, struct mlx5_fc *counter);
u64 mlx5_fc_query_lastuse(struct mlx5_fc *counter);
void mlx5_fc_query_cached(struct mlx5_fc *counter,
			  u64 *bytes, u64 *packets, u64 *lastuse);
int mlx5_fc_query(struct mlx5_core_dev *dev, struct mlx5_fc *counter,
		  u64 *packets, u64 *bytes);

int mlx5_fs_add_rx_underlay_qpn(struct mlx5_core_dev *dev, u32 underlay_qpn);
int mlx5_fs_remove_rx_underlay_qpn(struct mlx5_core_dev *dev, u32 underlay_qpn);

#endif
