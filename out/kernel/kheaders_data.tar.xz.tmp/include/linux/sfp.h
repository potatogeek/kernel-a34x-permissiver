#ifndef LINUX_SFP_H
#define LINUX_SFP_H

#include <linux/phy.h>

struct sfp_eeprom_base {
	u8 phys_id;
	u8 phys_ext_id;
	u8 connector;
#if defined __BIG_ENDIAN_BITFIELD
	u8 e10g_base_er:1;
	u8 e10g_base_lrm:1;
	u8 e10g_base_lr:1;
	u8 e10g_base_sr:1;
	u8 if_1x_sx:1;
	u8 if_1x_lx:1;
	u8 if_1x_copper_active:1;
	u8 if_1x_copper_passive:1;

	u8 escon_mmf_1310_led:1;
	u8 escon_smf_1310_laser:1;
	u8 sonet_oc192_short_reach:1;
	u8 sonet_reach_bit1:1;
	u8 sonet_reach_bit2:1;
	u8 sonet_oc48_long_reach:1;
	u8 sonet_oc48_intermediate_reach:1;
	u8 sonet_oc48_short_reach:1;

	u8 unallocated_5_7:1;
	u8 sonet_oc12_smf_long_reach:1;
	u8 sonet_oc12_smf_intermediate_reach:1;
	u8 sonet_oc12_short_reach:1;
	u8 unallocated_5_3:1;
	u8 sonet_oc3_smf_long_reach:1;
	u8 sonet_oc3_smf_intermediate_reach:1;
	u8 sonet_oc3_short_reach:1;

	u8 e_base_px:1;
	u8 e_base_bx10:1;
	u8 e100_base_fx:1;
	u8 e100_base_lx:1;
	u8 e1000_base_t:1;
	u8 e1000_base_cx:1;
	u8 e1000_base_lx:1;
	u8 e1000_base_sx:1;

	u8 fc_ll_v:1;
	u8 fc_ll_s:1;
	u8 fc_ll_i:1;
	u8 fc_ll_l:1;
	u8 fc_ll_m:1;
	u8 fc_tech_sa:1;
	u8 fc_tech_lc:1;
	u8 fc_tech_electrical_inter_enclosure:1;

	u8 fc_tech_electrical_intra_enclosure:1;
	u8 fc_tech_sn:1;
	u8 fc_tech_sl:1;
	u8 fc_tech_ll:1;
	u8 sfp_ct_active:1;
	u8 sfp_ct_passive:1;
	u8 unallocated_8_1:1;
	u8 unallocated_8_0:1;

	u8 fc_media_tw:1;
	u8 fc_media_tp:1;
	u8 fc_media_mi:1;
	u8 fc_media_tv:1;
	u8 fc_media_m6:1;
	u8 fc_media_m5:1;
	u8 unallocated_9_1:1;
	u8 fc_media_sm:1;

	u8 fc_speed_1200:1;
	u8 fc_speed_800:1;
	u8 fc_speed_1600:1;
	u8 fc_speed_400:1;
	u8 fc_speed_3200:1;
	u8 fc_speed_200:1;
	u8 unallocated_10_1:1;
	u8 fc_speed_100:1;
#elif defined __LITTLE_ENDIAN_BITFIELD
	u8 if_1x_copper_passive:1;
	u8 if_1x_copper_active:1;
	u8 if_1x_lx:1;
	u8 if_1x_sx:1;
	u8 e10g_base_sr:1;
	u8 e10g_base_lr:1;
	u8 e10g_base_lrm:1;
	u8 e10g_base_er:1;

	u8 sonet_oc3_short_reach:1;
	u8 sonet_oc3_smf_intermediate_reach:1;
	u8 sonet_oc3_smf_long_reach:1;
	u8 unallocated_5_3:1;
	u8 sonet_oc12_short_reach:1;
	u8 sonet_oc12_smf_intermediate_reach:1;
	u8 sonet_oc12_smf_long_reach:1;
	u8 unallocated_5_7:1;

	u8 sonet_oc48_short_reach:1;
	u8 sonet_oc48_intermediate_reach:1;
	u8 sonet_oc48_long_reach:1;
	u8 sonet_reach_bit2:1;
	u8 sonet_reach_bit1:1;
	u8 sonet_oc192_short_reach:1;
	u8 escon_smf_1310_laser:1;
	u8 escon_mmf_1310_led:1;

	u8 e1000_base_sx:1;
	u8 e1000_base_lx:1;
	u8 e1000_base_cx:1;
	u8 e1000_base_t:1;
	u8 e100_base_lx:1;
	u8 e100_base_fx:1;
	u8 e_base_bx10:1;
	u8 e_base_px:1;

	u8 fc_tech_electrical_inter_enclosure:1;
	u8 fc_tech_lc:1;
	u8 fc_tech_sa:1;
	u8 fc_ll_m:1;
	u8 fc_ll_l:1;
	u8 fc_ll_i:1;
	u8 fc_ll_s:1;
	u8 fc_ll_v:1;

	u8 unallocated_8_0:1;
	u8 unallocated_8_1:1;
	u8 sfp_ct_passive:1;
	u8 sfp_ct_active:1;
	u8 fc_tech_ll:1;
	u8 fc_tech_sl:1;
	u8 fc_tech_sn:1;
	u8 fc_tech_electrical_intra_enclosure:1;

	u8 fc_media_sm:1;
	u8 unallocated_9_1:1;
	u8 fc_media_m5:1;
	u8 fc_media_m6:1;
	u8 fc_media_tv:1;
	u8 fc_media_mi:1;
	u8 fc_media_tp:1;
	u8 fc_media_tw:1;

	u8 fc_speed_100:1;
	u8 unallocated_10_1:1;
	u8 fc_speed_200:1;
	u8 fc_speed_3200:1;
	u8 fc_speed_400:1;
	u8 fc_speed_1600:1;
	u8 fc_speed_800:1;
	u8 fc_speed_1200:1;
#else
#error Unknown Endian
#endif
	u8 encoding;
	u8 br_nominal;
	u8 rate_id;
	u8 link_len[6];
	char vendor_name[16];
	u8 extended_cc;
	char vendor_oui[3];
	char vendor_pn[16];
	char vendor_rev[4];
	union {
		__be16 optical_wavelength;
		__be16 cable_compliance;
		struct {
#if defined __BIG_ENDIAN_BITFIELD
			u8 reserved60_2:6;
			u8 fc_pi_4_app_h:1;
			u8 sff8431_app_e:1;
			u8 reserved61:8;
#elif defined __LITTLE_ENDIAN_BITFIELD
			u8 sff8431_app_e:1;
			u8 fc_pi_4_app_h:1;
			u8 reserved60_2:6;
			u8 reserved61:8;
#else
#error Unknown Endian
#endif
		} __packed passive;
		struct {
#if defined __BIG_ENDIAN_BITFIELD
			u8 reserved60_4:4;
			u8 fc_pi_4_lim:1;
			u8 sff8431_lim:1;
			u8 fc_pi_4_app_h:1;
			u8 sff8431_app_e:1;
			u8 reserved61:8;
#elif defined __LITTLE_ENDIAN_BITFIELD
			u8 sff8431_app_e:1;
			u8 fc_pi_4_app_h:1;
			u8 sff8431_lim:1;
			u8 fc_pi_4_lim:1;
			u8 reserved60_4:4;
			u8 reserved61:8;
#else
#error Unknown Endian
#endif
		} __packed active;
	} __packed;
	u8 reserved62;
	u8 cc_base;
} __packed;

struct sfp_eeprom_ext {
	__be16 options;
	u8 br_max;
	u8 br_min;
	char vendor_sn[16];
	char datecode[8];
	u8 diagmon;
	u8 enhopts;
	u8 sff8472_compliance;
	u8 cc_ext;
} __packed;


struct sfp_eeprom_id {
	struct sfp_eeprom_base base;
	struct sfp_eeprom_ext ext;
} __packed;

struct sfp_diag {
	__be16 temp_high_alarm;
	__be16 temp_low_alarm;
	__be16 temp_high_warn;
	__be16 temp_low_warn;
	__be16 volt_high_alarm;
	__be16 volt_low_alarm;
	__be16 volt_high_warn;
	__be16 volt_low_warn;
	__be16 bias_high_alarm;
	__be16 bias_low_alarm;
	__be16 bias_high_warn;
	__be16 bias_low_warn;
	__be16 txpwr_high_alarm;
	__be16 txpwr_low_alarm;
	__be16 txpwr_high_warn;
	__be16 txpwr_low_warn;
	__be16 rxpwr_high_alarm;
	__be16 rxpwr_low_alarm;
	__be16 rxpwr_high_warn;
	__be16 rxpwr_low_warn;
	__be16 laser_temp_high_alarm;
	__be16 laser_temp_low_alarm;
	__be16 laser_temp_high_warn;
	__be16 laser_temp_low_warn;
	__be16 tec_cur_high_alarm;
	__be16 tec_cur_low_alarm;
	__be16 tec_cur_high_warn;
	__be16 tec_cur_low_warn;
	__be32 cal_rxpwr4;
	__be32 cal_rxpwr3;
	__be32 cal_rxpwr2;
	__be32 cal_rxpwr1;
	__be32 cal_rxpwr0;
	__be16 cal_txi_slope;
	__be16 cal_txi_offset;
	__be16 cal_txpwr_slope;
	__be16 cal_txpwr_offset;
	__be16 cal_t_slope;
	__be16 cal_t_offset;
	__be16 cal_v_slope;
	__be16 cal_v_offset;
} __packed;


enum {
	SFP_PHYS_ID			= 0x00,
	SFP_PHYS_EXT_ID			= 0x01,
	SFP_CONNECTOR			= 0x02,
	SFP_COMPLIANCE			= 0x03,
	SFP_ENCODING			= 0x0b,
	SFP_BR_NOMINAL			= 0x0c,
	SFP_RATE_ID			= 0x0d,
	SFP_LINK_LEN_SM_KM		= 0x0e,
	SFP_LINK_LEN_SM_100M		= 0x0f,
	SFP_LINK_LEN_50UM_OM2_10M	= 0x10,
	SFP_LINK_LEN_62_5UM_OM1_10M	= 0x11,
	SFP_LINK_LEN_COPPER_1M		= 0x12,
	SFP_LINK_LEN_50UM_OM4_10M	= 0x12,
	SFP_LINK_LEN_50UM_OM3_10M	= 0x13,
	SFP_VENDOR_NAME			= 0x14,
	SFP_VENDOR_OUI			= 0x25,
	SFP_VENDOR_PN			= 0x28,
	SFP_VENDOR_REV			= 0x38,
	SFP_OPTICAL_WAVELENGTH_MSB	= 0x3c,
	SFP_OPTICAL_WAVELENGTH_LSB	= 0x3d,
	SFP_CABLE_SPEC			= 0x3c,
	SFP_CC_BASE			= 0x3f,
	SFP_OPTIONS			= 0x40,	
	SFP_BR_MAX			= 0x42,
	SFP_BR_MIN			= 0x43,
	SFP_VENDOR_SN			= 0x44,
	SFP_DATECODE			= 0x54,
	SFP_DIAGMON			= 0x5c,
	SFP_ENHOPTS			= 0x5d,
	SFP_SFF8472_COMPLIANCE		= 0x5e,
	SFP_CC_EXT			= 0x5f,

	SFP_PHYS_ID_SFF			= 0x02,
	SFP_PHYS_ID_SFP			= 0x03,
	SFP_PHYS_EXT_ID_SFP		= 0x04,
	SFP_CONNECTOR_UNSPEC		= 0x00,
	
	SFP_CONNECTOR_SC		= 0x01,
	SFP_CONNECTOR_FIBERJACK		= 0x06,
	SFP_CONNECTOR_LC		= 0x07,
	SFP_CONNECTOR_MT_RJ		= 0x08,
	SFP_CONNECTOR_MU		= 0x09,
	SFP_CONNECTOR_SG		= 0x0a,
	SFP_CONNECTOR_OPTICAL_PIGTAIL	= 0x0b,
	SFP_CONNECTOR_MPO_1X12		= 0x0c,
	SFP_CONNECTOR_MPO_2X16		= 0x0d,
	SFP_CONNECTOR_HSSDC_II		= 0x20,
	SFP_CONNECTOR_COPPER_PIGTAIL	= 0x21,
	SFP_CONNECTOR_RJ45		= 0x22,
	SFP_CONNECTOR_NOSEPARATE	= 0x23,
	SFP_CONNECTOR_MXC_2X16		= 0x24,
	SFP_ENCODING_UNSPEC		= 0x00,
	SFP_ENCODING_8B10B		= 0x01,
	SFP_ENCODING_4B5B		= 0x02,
	SFP_ENCODING_NRZ		= 0x03,
	SFP_ENCODING_8472_MANCHESTER	= 0x04,
	SFP_ENCODING_8472_SONET		= 0x05,
	SFP_ENCODING_8472_64B66B	= 0x06,
	SFP_ENCODING_256B257B		= 0x07,
	SFP_ENCODING_PAM4		= 0x08,
	SFP_OPTIONS_HIGH_POWER_LEVEL	= BIT(13),
	SFP_OPTIONS_PAGING_A2		= BIT(12),
	SFP_OPTIONS_RETIMER		= BIT(11),
	SFP_OPTIONS_COOLED_XCVR		= BIT(10),
	SFP_OPTIONS_POWER_DECL		= BIT(9),
	SFP_OPTIONS_RX_LINEAR_OUT	= BIT(8),
	SFP_OPTIONS_RX_DECISION_THRESH	= BIT(7),
	SFP_OPTIONS_TUNABLE_TX		= BIT(6),
	SFP_OPTIONS_RATE_SELECT		= BIT(5),
	SFP_OPTIONS_TX_DISABLE		= BIT(4),
	SFP_OPTIONS_TX_FAULT		= BIT(3),
	SFP_OPTIONS_LOS_INVERTED	= BIT(2),
	SFP_OPTIONS_LOS_NORMAL		= BIT(1),
	SFP_DIAGMON_DDM			= BIT(6),
	SFP_DIAGMON_INT_CAL		= BIT(5),
	SFP_DIAGMON_EXT_CAL		= BIT(4),
	SFP_DIAGMON_RXPWR_AVG		= BIT(3),
	SFP_DIAGMON_ADDRMODE		= BIT(2),
	SFP_ENHOPTS_ALARMWARN		= BIT(7),
	SFP_ENHOPTS_SOFT_TX_DISABLE	= BIT(6),
	SFP_ENHOPTS_SOFT_TX_FAULT	= BIT(5),
	SFP_ENHOPTS_SOFT_RX_LOS		= BIT(4),
	SFP_ENHOPTS_SOFT_RATE_SELECT	= BIT(3),
	SFP_ENHOPTS_APP_SELECT_SFF8079	= BIT(2),
	SFP_ENHOPTS_SOFT_RATE_SFF8431	= BIT(1),
	SFP_SFF8472_COMPLIANCE_NONE	= 0x00,
	SFP_SFF8472_COMPLIANCE_REV9_3	= 0x01,
	SFP_SFF8472_COMPLIANCE_REV9_5	= 0x02,
	SFP_SFF8472_COMPLIANCE_REV10_2	= 0x03,
	SFP_SFF8472_COMPLIANCE_REV10_4	= 0x04,
	SFP_SFF8472_COMPLIANCE_REV11_0	= 0x05,
	SFP_SFF8472_COMPLIANCE_REV11_3	= 0x06,
	SFP_SFF8472_COMPLIANCE_REV11_4	= 0x07,
	SFP_SFF8472_COMPLIANCE_REV12_0	= 0x08,
};


enum {
	
	SFP_TEMP_HIGH_ALARM		= 0x00,
	SFP_TEMP_LOW_ALARM		= 0x02,
	SFP_TEMP_HIGH_WARN		= 0x04,
	SFP_TEMP_LOW_WARN		= 0x06,
	SFP_VOLT_HIGH_ALARM		= 0x08,
	SFP_VOLT_LOW_ALARM		= 0x0a,
	SFP_VOLT_HIGH_WARN		= 0x0c,
	SFP_VOLT_LOW_WARN		= 0x0e,
	SFP_BIAS_HIGH_ALARM		= 0x10,
	SFP_BIAS_LOW_ALARM		= 0x12,
	SFP_BIAS_HIGH_WARN		= 0x14,
	SFP_BIAS_LOW_WARN		= 0x16,
	SFP_TXPWR_HIGH_ALARM		= 0x18,
	SFP_TXPWR_LOW_ALARM		= 0x1a,
	SFP_TXPWR_HIGH_WARN		= 0x1c,
	SFP_TXPWR_LOW_WARN		= 0x1e,
	SFP_RXPWR_HIGH_ALARM		= 0x20,
	SFP_RXPWR_LOW_ALARM		= 0x22,
	SFP_RXPWR_HIGH_WARN		= 0x24,
	SFP_RXPWR_LOW_WARN		= 0x26,
	SFP_LASER_TEMP_HIGH_ALARM	= 0x28,
	SFP_LASER_TEMP_LOW_ALARM	= 0x2a,
	SFP_LASER_TEMP_HIGH_WARN	= 0x2c,
	SFP_LASER_TEMP_LOW_WARN		= 0x2e,
	SFP_TEC_CUR_HIGH_ALARM		= 0x30,
	SFP_TEC_CUR_LOW_ALARM		= 0x32,
	SFP_TEC_CUR_HIGH_WARN		= 0x34,
	SFP_TEC_CUR_LOW_WARN		= 0x36,
	SFP_CAL_RXPWR4			= 0x38,
	SFP_CAL_RXPWR3			= 0x3c,
	SFP_CAL_RXPWR2			= 0x40,
	SFP_CAL_RXPWR1			= 0x44,
	SFP_CAL_RXPWR0			= 0x48,
	SFP_CAL_TXI_SLOPE		= 0x4c,
	SFP_CAL_TXI_OFFSET		= 0x4e,
	SFP_CAL_TXPWR_SLOPE		= 0x50,
	SFP_CAL_TXPWR_OFFSET		= 0x52,
	SFP_CAL_T_SLOPE			= 0x54,
	SFP_CAL_T_OFFSET		= 0x56,
	SFP_CAL_V_SLOPE			= 0x58,
	SFP_CAL_V_OFFSET		= 0x5a,
	SFP_CHKSUM			= 0x5f,

	SFP_TEMP			= 0x60,
	SFP_VCC				= 0x62,
	SFP_TX_BIAS			= 0x64,
	SFP_TX_POWER			= 0x66,
	SFP_RX_POWER			= 0x68,
	SFP_LASER_TEMP			= 0x6a,
	SFP_TEC_CUR			= 0x6c,

	SFP_STATUS			= 0x6e,
	SFP_ALARM0			= 0x70,
	SFP_ALARM0_TEMP_HIGH		= BIT(7),
	SFP_ALARM0_TEMP_LOW		= BIT(6),
	SFP_ALARM0_VCC_HIGH		= BIT(5),
	SFP_ALARM0_VCC_LOW		= BIT(4),
	SFP_ALARM0_TX_BIAS_HIGH		= BIT(3),
	SFP_ALARM0_TX_BIAS_LOW		= BIT(2),
	SFP_ALARM0_TXPWR_HIGH		= BIT(1),
	SFP_ALARM0_TXPWR_LOW		= BIT(0),

	SFP_ALARM1			= 0x71,
	SFP_ALARM1_RXPWR_HIGH		= BIT(7),
	SFP_ALARM1_RXPWR_LOW		= BIT(6),

	SFP_WARN0			= 0x74,
	SFP_WARN0_TEMP_HIGH		= BIT(7),
	SFP_WARN0_TEMP_LOW		= BIT(6),
	SFP_WARN0_VCC_HIGH		= BIT(5),
	SFP_WARN0_VCC_LOW		= BIT(4),
	SFP_WARN0_TX_BIAS_HIGH		= BIT(3),
	SFP_WARN0_TX_BIAS_LOW		= BIT(2),
	SFP_WARN0_TXPWR_HIGH		= BIT(1),
	SFP_WARN0_TXPWR_LOW		= BIT(0),

	SFP_WARN1			= 0x75,
	SFP_WARN1_RXPWR_HIGH		= BIT(7),
	SFP_WARN1_RXPWR_LOW		= BIT(6),

	SFP_EXT_STATUS			= 0x76,
	SFP_VSL				= 0x78,
	SFP_PAGE			= 0x7f,
};

struct fwnode_handle;
struct ethtool_eeprom;
struct ethtool_modinfo;
struct net_device;
struct sfp_bus;


struct sfp_upstream_ops {
	int (*module_insert)(void *priv, const struct sfp_eeprom_id *id);
	void (*module_remove)(void *priv);
	void (*link_down)(void *priv);
	void (*link_up)(void *priv);
	int (*connect_phy)(void *priv, struct phy_device *);
	void (*disconnect_phy)(void *priv);
};

#if IS_ENABLED(CONFIG_SFP)
int sfp_parse_port(struct sfp_bus *bus, const struct sfp_eeprom_id *id,
		   unsigned long *support);
void sfp_parse_support(struct sfp_bus *bus, const struct sfp_eeprom_id *id,
		       unsigned long *support);
phy_interface_t sfp_select_interface(struct sfp_bus *bus,
				     const struct sfp_eeprom_id *id,
				     unsigned long *link_modes);

int sfp_get_module_info(struct sfp_bus *bus, struct ethtool_modinfo *modinfo);
int sfp_get_module_eeprom(struct sfp_bus *bus, struct ethtool_eeprom *ee,
			  u8 *data);
void sfp_upstream_start(struct sfp_bus *bus);
void sfp_upstream_stop(struct sfp_bus *bus);
struct sfp_bus *sfp_register_upstream(struct fwnode_handle *fwnode,
				      struct net_device *ndev, void *upstream,
				      const struct sfp_upstream_ops *ops);
void sfp_unregister_upstream(struct sfp_bus *bus);
#else
static inline int sfp_parse_port(struct sfp_bus *bus,
				 const struct sfp_eeprom_id *id,
				 unsigned long *support)
{
	return PORT_OTHER;
}

static inline void sfp_parse_support(struct sfp_bus *bus,
				     const struct sfp_eeprom_id *id,
				     unsigned long *support)
{
}

static inline phy_interface_t sfp_select_interface(struct sfp_bus *bus,
						   const struct sfp_eeprom_id *id,
						   unsigned long *link_modes)
{
	return PHY_INTERFACE_MODE_NA;
}

static inline int sfp_get_module_info(struct sfp_bus *bus,
				      struct ethtool_modinfo *modinfo)
{
	return -EOPNOTSUPP;
}

static inline int sfp_get_module_eeprom(struct sfp_bus *bus,
					struct ethtool_eeprom *ee, u8 *data)
{
	return -EOPNOTSUPP;
}

static inline void sfp_upstream_start(struct sfp_bus *bus)
{
}

static inline void sfp_upstream_stop(struct sfp_bus *bus)
{
}

static inline struct sfp_bus *sfp_register_upstream(
	struct fwnode_handle *fwnode,
	struct net_device *ndev, void *upstream,
	const struct sfp_upstream_ops *ops)
{
	return (struct sfp_bus *)-1;
}

static inline void sfp_unregister_upstream(struct sfp_bus *bus)
{
}
#endif

#endif
