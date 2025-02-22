

#ifndef __MLX5_PORT_H__
#define __MLX5_PORT_H__

#include <linux/mlx5/driver.h>

enum mlx5_beacon_duration {
	MLX5_BEACON_DURATION_OFF = 0x0,
	MLX5_BEACON_DURATION_INF = 0xffff,
};

enum mlx5_module_id {
	MLX5_MODULE_ID_SFP              = 0x3,
	MLX5_MODULE_ID_QSFP             = 0xC,
	MLX5_MODULE_ID_QSFP_PLUS        = 0xD,
	MLX5_MODULE_ID_QSFP28           = 0x11,
};

enum mlx5_an_status {
	MLX5_AN_UNAVAILABLE = 0,
	MLX5_AN_COMPLETE    = 1,
	MLX5_AN_FAILED      = 2,
	MLX5_AN_LINK_UP     = 3,
	MLX5_AN_LINK_DOWN   = 4,
};

#define MLX5_EEPROM_MAX_BYTES			32
#define MLX5_EEPROM_IDENTIFIER_BYTE_MASK	0x000000ff
#define MLX5_I2C_ADDR_LOW		0x50
#define MLX5_I2C_ADDR_HIGH		0x51
#define MLX5_EEPROM_PAGE_LENGTH		256

enum mlx5e_link_mode {
	MLX5E_1000BASE_CX_SGMII	 = 0,
	MLX5E_1000BASE_KX	 = 1,
	MLX5E_10GBASE_CX4	 = 2,
	MLX5E_10GBASE_KX4	 = 3,
	MLX5E_10GBASE_KR	 = 4,
	MLX5E_20GBASE_KR2	 = 5,
	MLX5E_40GBASE_CR4	 = 6,
	MLX5E_40GBASE_KR4	 = 7,
	MLX5E_56GBASE_R4	 = 8,
	MLX5E_10GBASE_CR	 = 12,
	MLX5E_10GBASE_SR	 = 13,
	MLX5E_10GBASE_ER	 = 14,
	MLX5E_40GBASE_SR4	 = 15,
	MLX5E_40GBASE_LR4	 = 16,
	MLX5E_50GBASE_SR2	 = 18,
	MLX5E_100GBASE_CR4	 = 20,
	MLX5E_100GBASE_SR4	 = 21,
	MLX5E_100GBASE_KR4	 = 22,
	MLX5E_100GBASE_LR4	 = 23,
	MLX5E_100BASE_TX	 = 24,
	MLX5E_1000BASE_T	 = 25,
	MLX5E_10GBASE_T		 = 26,
	MLX5E_25GBASE_CR	 = 27,
	MLX5E_25GBASE_KR	 = 28,
	MLX5E_25GBASE_SR	 = 29,
	MLX5E_50GBASE_CR2	 = 30,
	MLX5E_50GBASE_KR2	 = 31,
	MLX5E_LINK_MODES_NUMBER,
};

enum mlx5e_connector_type {
	MLX5E_PORT_UNKNOWN	= 0,
	MLX5E_PORT_NONE			= 1,
	MLX5E_PORT_TP			= 2,
	MLX5E_PORT_AUI			= 3,
	MLX5E_PORT_BNC			= 4,
	MLX5E_PORT_MII			= 5,
	MLX5E_PORT_FIBRE		= 6,
	MLX5E_PORT_DA			= 7,
	MLX5E_PORT_OTHER		= 8,
	MLX5E_CONNECTOR_TYPE_NUMBER,
};

#define MLX5E_PROT_MASK(link_mode) (1 << link_mode)

#define PORT_MODULE_EVENT_MODULE_STATUS_MASK 0xF
#define PORT_MODULE_EVENT_ERROR_TYPE_MASK         0xF

int mlx5_set_port_caps(struct mlx5_core_dev *dev, u8 port_num, u32 caps);
int mlx5_query_port_ptys(struct mlx5_core_dev *dev, u32 *ptys,
			 int ptys_size, int proto_mask, u8 local_port);
int mlx5_query_port_proto_cap(struct mlx5_core_dev *dev,
			      u32 *proto_cap, int proto_mask);
int mlx5_query_port_proto_admin(struct mlx5_core_dev *dev,
				u32 *proto_admin, int proto_mask);
int mlx5_query_port_link_width_oper(struct mlx5_core_dev *dev,
				    u8 *link_width_oper, u8 local_port);
int mlx5_query_port_ib_proto_oper(struct mlx5_core_dev *dev,
				  u8 *proto_oper, u8 local_port);
int mlx5_query_port_eth_proto_oper(struct mlx5_core_dev *dev,
				   u32 *proto_oper, u8 local_port);
int mlx5_set_port_ptys(struct mlx5_core_dev *dev, bool an_disable,
		       u32 proto_admin, int proto_mask);
void mlx5_toggle_port_link(struct mlx5_core_dev *dev);
int mlx5_set_port_admin_status(struct mlx5_core_dev *dev,
			       enum mlx5_port_status status);
int mlx5_query_port_admin_status(struct mlx5_core_dev *dev,
				 enum mlx5_port_status *status);
int mlx5_set_port_beacon(struct mlx5_core_dev *dev, u16 beacon_duration);
void mlx5_query_port_autoneg(struct mlx5_core_dev *dev, int proto_mask,
			     u8 *an_status,
			     u8 *an_disable_cap, u8 *an_disable_admin);

int mlx5_set_port_mtu(struct mlx5_core_dev *dev, u16 mtu, u8 port);
void mlx5_query_port_max_mtu(struct mlx5_core_dev *dev, u16 *max_mtu, u8 port);
void mlx5_query_port_oper_mtu(struct mlx5_core_dev *dev, u16 *oper_mtu,
			      u8 port);

int mlx5_query_port_vl_hw_cap(struct mlx5_core_dev *dev,
			      u8 *vl_hw_cap, u8 local_port);

int mlx5_set_port_pause(struct mlx5_core_dev *dev, u32 rx_pause, u32 tx_pause);
int mlx5_query_port_pause(struct mlx5_core_dev *dev,
			  u32 *rx_pause, u32 *tx_pause);

int mlx5_set_port_pfc(struct mlx5_core_dev *dev, u8 pfc_en_tx, u8 pfc_en_rx);
int mlx5_query_port_pfc(struct mlx5_core_dev *dev, u8 *pfc_en_tx,
			u8 *pfc_en_rx);

int mlx5_set_port_stall_watermark(struct mlx5_core_dev *dev,
				  u16 stall_critical_watermark,
				  u16 stall_minor_watermark);
int mlx5_query_port_stall_watermark(struct mlx5_core_dev *dev,
				    u16 *stall_critical_watermark, u16 *stall_minor_watermark);

int mlx5_max_tc(struct mlx5_core_dev *mdev);

int mlx5_set_port_prio_tc(struct mlx5_core_dev *mdev, u8 *prio_tc);
int mlx5_query_port_prio_tc(struct mlx5_core_dev *mdev,
			    u8 prio, u8 *tc);
int mlx5_set_port_tc_group(struct mlx5_core_dev *mdev, u8 *tc_group);
int mlx5_query_port_tc_group(struct mlx5_core_dev *mdev,
			     u8 tc, u8 *tc_group);
int mlx5_set_port_tc_bw_alloc(struct mlx5_core_dev *mdev, u8 *tc_bw);
int mlx5_query_port_tc_bw_alloc(struct mlx5_core_dev *mdev,
				u8 tc, u8 *bw_pct);
int mlx5_modify_port_ets_rate_limit(struct mlx5_core_dev *mdev,
				    u8 *max_bw_value,
				    u8 *max_bw_unit);
int mlx5_query_port_ets_rate_limit(struct mlx5_core_dev *mdev,
				   u8 *max_bw_value,
				   u8 *max_bw_unit);
int mlx5_set_port_wol(struct mlx5_core_dev *mdev, u8 wol_mode);
int mlx5_query_port_wol(struct mlx5_core_dev *mdev, u8 *wol_mode);

int mlx5_set_port_fcs(struct mlx5_core_dev *mdev, u8 enable);
void mlx5_query_port_fcs(struct mlx5_core_dev *mdev, bool *supported,
			 bool *enabled);
int mlx5_query_module_eeprom(struct mlx5_core_dev *dev,
			     u16 offset, u16 size, u8 *data);

int mlx5_query_port_dcbx_param(struct mlx5_core_dev *mdev, u32 *out);
int mlx5_set_port_dcbx_param(struct mlx5_core_dev *mdev, u32 *in);

int mlx5_set_trust_state(struct mlx5_core_dev *mdev, u8 trust_state);
int mlx5_query_trust_state(struct mlx5_core_dev *mdev, u8 *trust_state);
int mlx5_set_dscp2prio(struct mlx5_core_dev *mdev, u8 dscp, u8 prio);
int mlx5_query_dscp2prio(struct mlx5_core_dev *mdev, u8 *dscp2prio);
#endif 
