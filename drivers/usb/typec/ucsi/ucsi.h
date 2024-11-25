/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DRIVER_USB_TYPEC_UCSI_H
#define __DRIVER_USB_TYPEC_UCSI_H

#include <linux/bitops.h>
#include <linux/device.h>
<<<<<<< HEAD
#include <linux/types.h>

/* -------------------------------------------------------------------------- */

/* Command Status and Connector Change Indication (CCI) data structure */
struct ucsi_cci {
	u8:1; /* reserved */
	u8 connector_change:7;
	u8 data_length;
	u16:9; /* reserved */
	u16 not_supported:1;
	u16 cancel_complete:1;
	u16 reset_complete:1;
	u16 busy:1;
	u16 ack_complete:1;
	u16 error:1;
	u16 cmd_complete:1;
} __packed;

/* Default fields in CONTROL data structure */
struct ucsi_command {
	u8 cmd;
	u8 length;
	u64 data:48;
} __packed;

/* ACK Command structure */
struct ucsi_ack_cmd {
	u8 cmd;
	u8 length;
	u8 cci_ack:1;
	u8 cmd_ack:1;
	u8:6; /* reserved */
} __packed;

/* Connector Reset Command structure */
struct ucsi_con_rst {
	u8 cmd;
	u8 length;
	u8 con_num:7;
	u8 hard_reset:1;
} __packed;

/* Set USB Operation Mode Command structure */
struct ucsi_uor_cmd {
	u8 cmd;
	u8 length;
	u16 con_num:7;
	u16 role:3;
#define UCSI_UOR_ROLE_DFP			BIT(0)
#define UCSI_UOR_ROLE_UFP			BIT(1)
#define UCSI_UOR_ROLE_DRP			BIT(2)
	u16:6; /* reserved */
} __packed;

struct ucsi_control {
	union {
		u64 raw_cmd;
		struct ucsi_command cmd;
		struct ucsi_uor_cmd uor;
		struct ucsi_ack_cmd ack;
		struct ucsi_con_rst con_rst;
	};
};

#define __UCSI_CMD(_ctrl_, _cmd_)					\
{									\
	(_ctrl_).raw_cmd = 0;						\
	(_ctrl_).cmd.cmd = _cmd_;					\
}

/* Helper for preparing ucsi_control for CONNECTOR_RESET command. */
#define UCSI_CMD_CONNECTOR_RESET(_ctrl_, _con_, _hard_)			\
{									\
	__UCSI_CMD(_ctrl_, UCSI_CONNECTOR_RESET)			\
	(_ctrl_).con_rst.con_num = (_con_)->num;			\
	(_ctrl_).con_rst.hard_reset = _hard_;				\
}

/* Helper for preparing ucsi_control for ACK_CC_CI command. */
#define UCSI_CMD_ACK(_ctrl_, _ack_)					\
{									\
	__UCSI_CMD(_ctrl_, UCSI_ACK_CC_CI)				\
	(_ctrl_).ack.cci_ack = ((_ack_) == UCSI_ACK_EVENT);		\
	(_ctrl_).ack.cmd_ack = ((_ack_) == UCSI_ACK_CMD);		\
}

/* Helper for preparing ucsi_control for SET_NOTIFY_ENABLE command. */
#define UCSI_CMD_SET_NTFY_ENABLE(_ctrl_, _ntfys_)			\
{									\
	__UCSI_CMD(_ctrl_, UCSI_SET_NOTIFICATION_ENABLE)		\
	(_ctrl_).cmd.data = _ntfys_;					\
}

/* Helper for preparing ucsi_control for GET_CAPABILITY command. */
#define UCSI_CMD_GET_CAPABILITY(_ctrl_)					\
{									\
	__UCSI_CMD(_ctrl_, UCSI_GET_CAPABILITY)				\
}

/* Helper for preparing ucsi_control for GET_CONNECTOR_CAPABILITY command. */
#define UCSI_CMD_GET_CONNECTOR_CAPABILITY(_ctrl_, _con_)		\
{									\
	__UCSI_CMD(_ctrl_, UCSI_GET_CONNECTOR_CAPABILITY)		\
	(_ctrl_).cmd.data = _con_;					\
}

/* Helper for preparing ucsi_control for GET_CONNECTOR_STATUS command. */
#define UCSI_CMD_GET_CONNECTOR_STATUS(_ctrl_, _con_)			\
{									\
	__UCSI_CMD(_ctrl_, UCSI_GET_CONNECTOR_STATUS)			\
	(_ctrl_).cmd.data = _con_;					\
}

#define __UCSI_ROLE(_ctrl_, _cmd_, _con_num_)				\
{									\
	__UCSI_CMD(_ctrl_, _cmd_)					\
	(_ctrl_).uor.con_num = _con_num_;				\
	(_ctrl_).uor.role = UCSI_UOR_ROLE_DRP;				\
}

/* Helper for preparing ucsi_control for SET_UOR command. */
#define UCSI_CMD_SET_UOR(_ctrl_, _con_, _role_)				\
{									\
	__UCSI_ROLE(_ctrl_, UCSI_SET_UOR, (_con_)->num)		\
	(_ctrl_).uor.role |= (_role_) == TYPEC_HOST ? UCSI_UOR_ROLE_DFP : \
			  UCSI_UOR_ROLE_UFP;				\
}

/* Helper for preparing ucsi_control for SET_PDR command. */
#define UCSI_CMD_SET_PDR(_ctrl_, _con_, _role_)			\
{									\
	__UCSI_ROLE(_ctrl_, UCSI_SET_PDR, (_con_)->num)		\
	(_ctrl_).uor.role |= (_role_) == TYPEC_SOURCE ? UCSI_UOR_ROLE_DFP : \
			UCSI_UOR_ROLE_UFP;				\
}
=======
#include <linux/power_supply.h>
#include <linux/types.h>
#include <linux/usb/typec.h>
#include <linux/usb/pd.h>
#include <linux/usb/role.h>

/* -------------------------------------------------------------------------- */

struct ucsi;
struct ucsi_altmode;

/* UCSI offsets (Bytes) */
#define UCSI_VERSION			0
#define UCSI_CCI			4
#define UCSI_CONTROL			8
#define UCSI_MESSAGE_IN			16
#define UCSI_MESSAGE_OUT		32

/* Command Status and Connector Change Indication (CCI) bits */
#define UCSI_CCI_CONNECTOR(_c_)		(((_c_) & GENMASK(7, 1)) >> 1)
#define UCSI_CCI_LENGTH(_c_)		(((_c_) & GENMASK(15, 8)) >> 8)
#define UCSI_CCI_NOT_SUPPORTED		BIT(25)
#define UCSI_CCI_CANCEL_COMPLETE	BIT(26)
#define UCSI_CCI_RESET_COMPLETE		BIT(27)
#define UCSI_CCI_BUSY			BIT(28)
#define UCSI_CCI_ACK_COMPLETE		BIT(29)
#define UCSI_CCI_ERROR			BIT(30)
#define UCSI_CCI_COMMAND_COMPLETE	BIT(31)

/**
 * struct ucsi_operations - UCSI I/O operations
 * @read: Read operation
 * @sync_write: Blocking write operation
 * @async_write: Non-blocking write operation
 * @update_altmodes: Squashes duplicate DP altmodes
 *
 * Read and write routines for UCSI interface. @sync_write must wait for the
 * Command Completion Event from the PPM before returning, and @async_write must
 * return immediately after sending the data to the PPM.
 */
struct ucsi_operations {
	int (*read)(struct ucsi *ucsi, unsigned int offset,
		    void *val, size_t val_len);
	int (*sync_write)(struct ucsi *ucsi, unsigned int offset,
			  const void *val, size_t val_len);
	int (*async_write)(struct ucsi *ucsi, unsigned int offset,
			   const void *val, size_t val_len);
	bool (*update_altmodes)(struct ucsi *ucsi, struct ucsi_altmode *orig,
				struct ucsi_altmode *updated);
};

struct ucsi *ucsi_create(struct device *dev, const struct ucsi_operations *ops);
void ucsi_destroy(struct ucsi *ucsi);
int ucsi_register(struct ucsi *ucsi);
void ucsi_unregister(struct ucsi *ucsi);
void *ucsi_get_drvdata(struct ucsi *ucsi);
void ucsi_set_drvdata(struct ucsi *ucsi, void *data);

void ucsi_connector_change(struct ucsi *ucsi, u8 num);

/* -------------------------------------------------------------------------- */
>>>>>>> upstream/android-13

/* Commands */
#define UCSI_PPM_RESET			0x01
#define UCSI_CANCEL			0x02
#define UCSI_CONNECTOR_RESET		0x03
#define UCSI_ACK_CC_CI			0x04
#define UCSI_SET_NOTIFICATION_ENABLE	0x05
#define UCSI_GET_CAPABILITY		0x06
#define UCSI_GET_CONNECTOR_CAPABILITY	0x07
#define UCSI_SET_UOM			0x08
#define UCSI_SET_UOR			0x09
#define UCSI_SET_PDM			0x0a
#define UCSI_SET_PDR			0x0b
#define UCSI_GET_ALTERNATE_MODES	0x0c
#define UCSI_GET_CAM_SUPPORTED		0x0d
#define UCSI_GET_CURRENT_CAM		0x0e
#define UCSI_SET_NEW_CAM		0x0f
#define UCSI_GET_PDOS			0x10
#define UCSI_GET_CABLE_PROPERTY		0x11
#define UCSI_GET_CONNECTOR_STATUS	0x12
#define UCSI_GET_ERROR_STATUS		0x13

<<<<<<< HEAD
/* ACK_CC_CI commands */
#define UCSI_ACK_EVENT			1
#define UCSI_ACK_CMD			2

/* Bits for SET_NOTIFICATION_ENABLE command */
#define UCSI_ENABLE_NTFY_CMD_COMPLETE		BIT(0)
#define UCSI_ENABLE_NTFY_EXT_PWR_SRC_CHANGE	BIT(1)
#define UCSI_ENABLE_NTFY_PWR_OPMODE_CHANGE	BIT(2)
#define UCSI_ENABLE_NTFY_CAP_CHANGE		BIT(5)
#define UCSI_ENABLE_NTFY_PWR_LEVEL_CHANGE	BIT(6)
#define UCSI_ENABLE_NTFY_PD_RESET_COMPLETE	BIT(7)
#define UCSI_ENABLE_NTFY_CAM_CHANGE		BIT(8)
#define UCSI_ENABLE_NTFY_BAT_STATUS_CHANGE	BIT(9)
#define UCSI_ENABLE_NTFY_PARTNER_CHANGE		BIT(11)
#define UCSI_ENABLE_NTFY_PWR_DIR_CHANGE		BIT(12)
#define UCSI_ENABLE_NTFY_CONNECTOR_CHANGE	BIT(14)
#define UCSI_ENABLE_NTFY_ERROR			BIT(15)
#define UCSI_ENABLE_NTFY_ALL			0xdbe7
=======
#define UCSI_CONNECTOR_NUMBER(_num_)		((u64)(_num_) << 16)
#define UCSI_COMMAND(_cmd_)			((_cmd_) & 0xff)

/* CONNECTOR_RESET command bits */
#define UCSI_CONNECTOR_RESET_HARD		BIT(23) /* Deprecated in v1.1 */

/* ACK_CC_CI bits */
#define UCSI_ACK_CONNECTOR_CHANGE		BIT(16)
#define UCSI_ACK_COMMAND_COMPLETE		BIT(17)

/* SET_NOTIFICATION_ENABLE command bits */
#define UCSI_ENABLE_NTFY_CMD_COMPLETE		BIT(16)
#define UCSI_ENABLE_NTFY_EXT_PWR_SRC_CHANGE	BIT(17)
#define UCSI_ENABLE_NTFY_PWR_OPMODE_CHANGE	BIT(18)
#define UCSI_ENABLE_NTFY_CAP_CHANGE		BIT(21)
#define UCSI_ENABLE_NTFY_PWR_LEVEL_CHANGE	BIT(22)
#define UCSI_ENABLE_NTFY_PD_RESET_COMPLETE	BIT(23)
#define UCSI_ENABLE_NTFY_CAM_CHANGE		BIT(24)
#define UCSI_ENABLE_NTFY_BAT_STATUS_CHANGE	BIT(25)
#define UCSI_ENABLE_NTFY_PARTNER_CHANGE		BIT(27)
#define UCSI_ENABLE_NTFY_PWR_DIR_CHANGE		BIT(28)
#define UCSI_ENABLE_NTFY_CONNECTOR_CHANGE	BIT(30)
#define UCSI_ENABLE_NTFY_ERROR			BIT(31)
#define UCSI_ENABLE_NTFY_ALL			0xdbe70000

/* SET_UOR command bits */
#define UCSI_SET_UOR_ROLE(_r_)		(((_r_) == TYPEC_HOST ? 1 : 2) << 23)
#define UCSI_SET_UOR_ACCEPT_ROLE_SWAPS		BIT(25)

/* SET_PDF command bits */
#define UCSI_SET_PDR_ROLE(_r_)		(((_r_) == TYPEC_SOURCE ? 1 : 2) << 23)
#define UCSI_SET_PDR_ACCEPT_ROLE_SWAPS		BIT(25)

/* GET_ALTERNATE_MODES command bits */
#define UCSI_ALTMODE_RECIPIENT(_r_)		(((_r_) >> 16) & 0x7)
#define UCSI_GET_ALTMODE_RECIPIENT(_r_)		((u64)(_r_) << 16)
#define   UCSI_RECIPIENT_CON			0
#define   UCSI_RECIPIENT_SOP			1
#define   UCSI_RECIPIENT_SOP_P			2
#define   UCSI_RECIPIENT_SOP_PP			3
#define UCSI_GET_ALTMODE_CONNECTOR_NUMBER(_r_)	((u64)(_r_) << 24)
#define UCSI_ALTMODE_OFFSET(_r_)		(((_r_) >> 32) & 0xff)
#define UCSI_GET_ALTMODE_OFFSET(_r_)		((u64)(_r_) << 32)
#define UCSI_GET_ALTMODE_NUM_ALTMODES(_r_)	((u64)(_r_) << 40)

/* GET_PDOS command bits */
#define UCSI_GET_PDOS_PARTNER_PDO(_r_)		((u64)(_r_) << 23)
#define UCSI_GET_PDOS_PDO_OFFSET(_r_)		((u64)(_r_) << 24)
#define UCSI_GET_PDOS_NUM_PDOS(_r_)		((u64)(_r_) << 32)
#define UCSI_MAX_PDOS				(4)
#define UCSI_GET_PDOS_SRC_PDOS			((u64)1 << 34)

/* -------------------------------------------------------------------------- */
>>>>>>> upstream/android-13

/* Error information returned by PPM in response to GET_ERROR_STATUS command. */
#define UCSI_ERROR_UNREGONIZED_CMD		BIT(0)
#define UCSI_ERROR_INVALID_CON_NUM		BIT(1)
#define UCSI_ERROR_INVALID_CMD_ARGUMENT		BIT(2)
#define UCSI_ERROR_INCOMPATIBLE_PARTNER		BIT(3)
#define UCSI_ERROR_CC_COMMUNICATION_ERR		BIT(4)
#define UCSI_ERROR_DEAD_BATTERY			BIT(5)
#define UCSI_ERROR_CONTRACT_NEGOTIATION_FAIL	BIT(6)
<<<<<<< HEAD
=======
#define UCSI_ERROR_OVERCURRENT			BIT(7)
#define UCSI_ERROR_UNDEFINED			BIT(8)
#define UCSI_ERROR_PARTNER_REJECTED_SWAP	BIT(9)
#define UCSI_ERROR_HARD_RESET			BIT(10)
#define UCSI_ERROR_PPM_POLICY_CONFLICT		BIT(11)
#define UCSI_ERROR_SWAP_REJECTED		BIT(12)

#define UCSI_SET_NEW_CAM_ENTER(x)		(((x) >> 23) & 0x1)
#define UCSI_SET_NEW_CAM_GET_AM(x)		(((x) >> 24) & 0xff)
#define UCSI_SET_NEW_CAM_AM_MASK		(0xff << 24)
#define UCSI_SET_NEW_CAM_SET_AM(x)		(((x) & 0xff) << 24)
#define UCSI_CMD_CONNECTOR_MASK			(0x7)
>>>>>>> upstream/android-13

/* Data structure filled by PPM in response to GET_CAPABILITY command. */
struct ucsi_capability {
	u32 attributes;
#define UCSI_CAP_ATTR_DISABLE_STATE		BIT(0)
#define UCSI_CAP_ATTR_BATTERY_CHARGING		BIT(1)
#define UCSI_CAP_ATTR_USB_PD			BIT(2)
#define UCSI_CAP_ATTR_TYPEC_CURRENT		BIT(6)
#define UCSI_CAP_ATTR_POWER_AC_SUPPLY		BIT(8)
#define UCSI_CAP_ATTR_POWER_OTHER		BIT(10)
#define UCSI_CAP_ATTR_POWER_VBUS		BIT(14)
<<<<<<< HEAD
	u32 num_connectors:8;
	u32 features:24;
=======
	u8 num_connectors;
	u8 features;
>>>>>>> upstream/android-13
#define UCSI_CAP_SET_UOM			BIT(0)
#define UCSI_CAP_SET_PDM			BIT(1)
#define UCSI_CAP_ALT_MODE_DETAILS		BIT(2)
#define UCSI_CAP_ALT_MODE_OVERRIDE		BIT(3)
#define UCSI_CAP_PDO_DETAILS			BIT(4)
#define UCSI_CAP_CABLE_DETAILS			BIT(5)
#define UCSI_CAP_EXT_SUPPLY_NOTIFICATIONS	BIT(6)
#define UCSI_CAP_PD_RESET			BIT(7)
<<<<<<< HEAD
	u8 num_alt_modes;
	u8 reserved;
=======
	u16 reserved_1;
	u8 num_alt_modes;
	u8 reserved_2;
>>>>>>> upstream/android-13
	u16 bc_version;
	u16 pd_version;
	u16 typec_version;
} __packed;

/* Data structure filled by PPM in response to GET_CONNECTOR_CAPABILITY cmd. */
struct ucsi_connector_capability {
	u8 op_mode;
#define UCSI_CONCAP_OPMODE_DFP			BIT(0)
#define UCSI_CONCAP_OPMODE_UFP			BIT(1)
#define UCSI_CONCAP_OPMODE_DRP			BIT(2)
#define UCSI_CONCAP_OPMODE_AUDIO_ACCESSORY	BIT(3)
#define UCSI_CONCAP_OPMODE_DEBUG_ACCESSORY	BIT(4)
#define UCSI_CONCAP_OPMODE_USB2			BIT(5)
#define UCSI_CONCAP_OPMODE_USB3			BIT(6)
#define UCSI_CONCAP_OPMODE_ALT_MODE		BIT(7)
<<<<<<< HEAD
	u8 provider:1;
	u8 consumer:1;
	u8:6; /* reserved */
=======
	u8 flags;
#define UCSI_CONCAP_FLAG_PROVIDER		BIT(0)
#define UCSI_CONCAP_FLAG_CONSUMER		BIT(1)
>>>>>>> upstream/android-13
} __packed;

struct ucsi_altmode {
	u16 svid;
	u32 mid;
} __packed;

/* Data structure filled by PPM in response to GET_CABLE_PROPERTY command. */
struct ucsi_cable_property {
	u16 speed_supported;
	u8 current_capability;
<<<<<<< HEAD
	u8 vbus_in_cable:1;
	u8 active_cable:1;
	u8 directionality:1;
	u8 plug_type:2;
#define UCSI_CABLE_PROPERTY_PLUG_TYPE_A		0
#define UCSI_CABLE_PROPERTY_PLUG_TYPE_B		1
#define UCSI_CABLE_PROPERTY_PLUG_TYPE_C		2
#define UCSI_CABLE_PROPERTY_PLUG_OTHER		3
	u8 mode_support:1;
	u8:2; /* reserved */
	u8 latency:4;
	u8:4; /* reserved */
=======
	u8 flags;
#define UCSI_CABLE_PROP_FLAG_VBUS_IN_CABLE	BIT(0)
#define UCSI_CABLE_PROP_FLAG_ACTIVE_CABLE	BIT(1)
#define UCSI_CABLE_PROP_FLAG_DIRECTIONALITY	BIT(2)
#define UCSI_CABLE_PROP_FLAG_PLUG_TYPE(_f_)	((_f_) & GENMASK(3, 0))
#define   UCSI_CABLE_PROPERTY_PLUG_TYPE_A	0
#define   UCSI_CABLE_PROPERTY_PLUG_TYPE_B	1
#define   UCSI_CABLE_PROPERTY_PLUG_TYPE_C	2
#define   UCSI_CABLE_PROPERTY_PLUG_OTHER	3
#define UCSI_CABLE_PROP_MODE_SUPPORT		BIT(5)
	u8 latency;
>>>>>>> upstream/android-13
} __packed;

/* Data structure filled by PPM in response to GET_CONNECTOR_STATUS command. */
struct ucsi_connector_status {
	u16 change;
#define UCSI_CONSTAT_EXT_SUPPLY_CHANGE		BIT(1)
#define UCSI_CONSTAT_POWER_OPMODE_CHANGE	BIT(2)
#define UCSI_CONSTAT_PDOS_CHANGE		BIT(5)
#define UCSI_CONSTAT_POWER_LEVEL_CHANGE		BIT(6)
#define UCSI_CONSTAT_PD_RESET_COMPLETE		BIT(7)
#define UCSI_CONSTAT_CAM_CHANGE			BIT(8)
#define UCSI_CONSTAT_BC_CHANGE			BIT(9)
#define UCSI_CONSTAT_PARTNER_CHANGE		BIT(11)
#define UCSI_CONSTAT_POWER_DIR_CHANGE		BIT(12)
#define UCSI_CONSTAT_CONNECT_CHANGE		BIT(14)
#define UCSI_CONSTAT_ERROR			BIT(15)
<<<<<<< HEAD
	u16 pwr_op_mode:3;
#define UCSI_CONSTAT_PWR_OPMODE_NONE		0
#define UCSI_CONSTAT_PWR_OPMODE_DEFAULT		1
#define UCSI_CONSTAT_PWR_OPMODE_BC		2
#define UCSI_CONSTAT_PWR_OPMODE_PD		3
#define UCSI_CONSTAT_PWR_OPMODE_TYPEC1_5	4
#define UCSI_CONSTAT_PWR_OPMODE_TYPEC3_0	5
	u16 connected:1;
	u16 pwr_dir:1;
	u16 partner_flags:8;
#define UCSI_CONSTAT_PARTNER_FLAG_USB		BIT(0)
#define UCSI_CONSTAT_PARTNER_FLAG_ALT_MODE	BIT(1)
	u16 partner_type:3;
#define UCSI_CONSTAT_PARTNER_TYPE_DFP		1
#define UCSI_CONSTAT_PARTNER_TYPE_UFP		2
#define UCSI_CONSTAT_PARTNER_TYPE_CABLE		3 /* Powered Cable */
#define UCSI_CONSTAT_PARTNER_TYPE_CABLE_AND_UFP	4 /* Powered Cable */
#define UCSI_CONSTAT_PARTNER_TYPE_DEBUG		5
#define UCSI_CONSTAT_PARTNER_TYPE_AUDIO		6
	u32 request_data_obj;
	u8 bc_status:2;
#define UCSI_CONSTAT_BC_NOT_CHARGING		0
#define UCSI_CONSTAT_BC_NOMINAL_CHARGING	1
#define UCSI_CONSTAT_BC_SLOW_CHARGING		2
#define UCSI_CONSTAT_BC_TRICKLE_CHARGING	3
	u8 provider_cap_limit_reason:4;
#define UCSI_CONSTAT_CAP_PWR_LOWERED		0
#define UCSI_CONSTAT_CAP_PWR_BUDGET_LIMIT	1
	u8:2; /* reserved */
=======
	u16 flags;
#define UCSI_CONSTAT_PWR_OPMODE(_f_)		((_f_) & GENMASK(2, 0))
#define   UCSI_CONSTAT_PWR_OPMODE_NONE		0
#define   UCSI_CONSTAT_PWR_OPMODE_DEFAULT	1
#define   UCSI_CONSTAT_PWR_OPMODE_BC		2
#define   UCSI_CONSTAT_PWR_OPMODE_PD		3
#define   UCSI_CONSTAT_PWR_OPMODE_TYPEC1_5	4
#define   UCSI_CONSTAT_PWR_OPMODE_TYPEC3_0	5
#define UCSI_CONSTAT_CONNECTED			BIT(3)
#define UCSI_CONSTAT_PWR_DIR			BIT(4)
#define UCSI_CONSTAT_PARTNER_FLAGS(_f_)		(((_f_) & GENMASK(12, 5)) >> 5)
#define   UCSI_CONSTAT_PARTNER_FLAG_USB		1
#define   UCSI_CONSTAT_PARTNER_FLAG_ALT_MODE	2
#define UCSI_CONSTAT_PARTNER_TYPE(_f_)		(((_f_) & GENMASK(15, 13)) >> 13)
#define   UCSI_CONSTAT_PARTNER_TYPE_DFP		1
#define   UCSI_CONSTAT_PARTNER_TYPE_UFP		2
#define   UCSI_CONSTAT_PARTNER_TYPE_CABLE	3 /* Powered Cable */
#define   UCSI_CONSTAT_PARTNER_TYPE_CABLE_AND_UFP	4 /* Powered Cable */
#define   UCSI_CONSTAT_PARTNER_TYPE_DEBUG	5
#define   UCSI_CONSTAT_PARTNER_TYPE_AUDIO	6
	u32 request_data_obj;
	u8 pwr_status;
#define UCSI_CONSTAT_BC_STATUS(_p_)		((_p_) & GENMASK(2, 0))
#define   UCSI_CONSTAT_BC_NOT_CHARGING		0
#define   UCSI_CONSTAT_BC_NOMINAL_CHARGING	1
#define   UCSI_CONSTAT_BC_SLOW_CHARGING		2
#define   UCSI_CONSTAT_BC_TRICKLE_CHARGING	3
#define UCSI_CONSTAT_PROVIDER_CAP_LIMIT(_p_)	(((_p_) & GENMASK(6, 3)) >> 3)
#define   UCSI_CONSTAT_CAP_PWR_LOWERED		0
#define   UCSI_CONSTAT_CAP_PWR_BUDGET_LIMIT	1
>>>>>>> upstream/android-13
} __packed;

/* -------------------------------------------------------------------------- */

<<<<<<< HEAD
struct ucsi;

struct ucsi_data {
	u16 version;
	u16 reserved;
	union {
		u32 raw_cci;
		struct ucsi_cci cci;
	};
	struct ucsi_control ctrl;
	u32 message_in[4];
	u32 message_out[4];
} __packed;

/*
 * struct ucsi_ppm - Interface to UCSI Platform Policy Manager
 * @data: memory location to the UCSI data structures
 * @cmd: UCSI command execution routine
 * @sync: Refresh UCSI mailbox (the data structures)
 */
struct ucsi_ppm {
	struct ucsi_data *data;
	int (*cmd)(struct ucsi_ppm *, struct ucsi_control *);
	int (*sync)(struct ucsi_ppm *);
};

struct ucsi *ucsi_register_ppm(struct device *dev, struct ucsi_ppm *ppm);
void ucsi_unregister_ppm(struct ucsi *ucsi);
void ucsi_notify(struct ucsi *ucsi);
=======
struct ucsi {
	u16 version;
	struct device *dev;
	struct driver_data *driver_data;

	const struct ucsi_operations *ops;

	struct ucsi_capability cap;
	struct ucsi_connector *connector;

	struct work_struct work;

	/* PPM Communication lock */
	struct mutex ppm_lock;

	/* The latest "Notification Enable" bits (SET_NOTIFICATION_ENABLE) */
	u64 ntfy;

	/* PPM communication flags */
	unsigned long flags;
#define EVENT_PENDING	0
#define COMMAND_PENDING	1
#define ACK_PENDING	2
#define EVENT_PROCESSING	3
};

#define UCSI_MAX_SVID		5
#define UCSI_MAX_ALTMODES	(UCSI_MAX_SVID * 6)

#define UCSI_TYPEC_VSAFE5V	5000
#define UCSI_TYPEC_1_5_CURRENT	1500
#define UCSI_TYPEC_3_0_CURRENT	3000

struct ucsi_connector {
	int num;

	struct ucsi *ucsi;
	struct mutex lock; /* port lock */
	struct work_struct work;
	struct completion complete;

	struct typec_port *port;
	struct typec_partner *partner;

	struct typec_altmode *port_altmode[UCSI_MAX_ALTMODES];
	struct typec_altmode *partner_altmode[UCSI_MAX_ALTMODES];

	struct typec_capability typec_cap;

	u16 unprocessed_changes;
	struct ucsi_connector_status status;
	struct ucsi_connector_capability cap;
	struct power_supply *psy;
	struct power_supply_desc psy_desc;
	u32 rdo;
	u32 src_pdos[PDO_MAX_OBJECTS];
	int num_pdos;

	struct usb_role_switch *usb_role_sw;
};

int ucsi_send_command(struct ucsi *ucsi, u64 command,
		      void *retval, size_t size);

void ucsi_altmode_update_active(struct ucsi_connector *con);
int ucsi_resume(struct ucsi *ucsi);

#if IS_ENABLED(CONFIG_POWER_SUPPLY)
int ucsi_register_port_psy(struct ucsi_connector *con);
void ucsi_unregister_port_psy(struct ucsi_connector *con);
void ucsi_port_psy_changed(struct ucsi_connector *con);
#else
static inline int ucsi_register_port_psy(struct ucsi_connector *con) { return 0; }
static inline void ucsi_unregister_port_psy(struct ucsi_connector *con) { }
static inline void ucsi_port_psy_changed(struct ucsi_connector *con) { }
#endif /* CONFIG_POWER_SUPPLY */

#if IS_ENABLED(CONFIG_TYPEC_DP_ALTMODE)
struct typec_altmode *
ucsi_register_displayport(struct ucsi_connector *con,
			  bool override, int offset,
			  struct typec_altmode_desc *desc);

void ucsi_displayport_remove_partner(struct typec_altmode *adev);

#else
static inline struct typec_altmode *
ucsi_register_displayport(struct ucsi_connector *con,
			  bool override, int offset,
			  struct typec_altmode_desc *desc)
{
	return NULL;
}

static inline void
ucsi_displayport_remove_partner(struct typec_altmode *adev) { }
#endif /* CONFIG_TYPEC_DP_ALTMODE */

/*
 * NVIDIA VirtualLink (svid 0x955) has two altmode. VirtualLink
 * DP mode with vdo=0x1 and NVIDIA test mode with vdo=0x3
 */
#define USB_TYPEC_NVIDIA_VLINK_DP_VDO	0x1
#define USB_TYPEC_NVIDIA_VLINK_DBG_VDO	0x3
>>>>>>> upstream/android-13

#endif /* __DRIVER_USB_TYPEC_UCSI_H */
