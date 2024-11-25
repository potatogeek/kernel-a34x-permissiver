// SPDX-License-Identifier: GPL-2.0


#ifndef __PDIC_SYSFS__
#define __PDIC_SYSFS__

extern const struct attribute_group pdic_sysfs_group;

enum {
	BUILT_IN = 0,
	UMS = 1,
	SPU = 2,
	SPU_VERIFICATION = 3,
	FWUP_CMD_MAX = 4,
};

static const int RO_PERM = 0444;
static const int WO_PERM = 0200;
static const int RW_PERM = 0644;

enum pdic_sysfs_property {
	PDIC_SYSFS_PROP_CHIP_NAME = 0,
	PDIC_SYSFS_PROP_CUR_VERSION,
	PDIC_SYSFS_PROP_SRC_VERSION,
	PDIC_SYSFS_PROP_LPM_MODE,
	PDIC_SYSFS_PROP_STATE,
	PDIC_SYSFS_PROP_RID,
	PDIC_SYSFS_PROP_CTRL_OPTION,
	PDIC_SYSFS_PROP_BOOTING_DRY,
	PDIC_SYSFS_PROP_FW_UPDATE,
	PDIC_SYSFS_PROP_FW_UPDATE_STATUS,
	PDIC_SYSFS_PROP_FW_WATER,
	PDIC_SYSFS_PROP_DEX_FAN_UVDM,
	PDIC_SYSFS_PROP_ACC_DEVICE_VERSION,
	PDIC_SYSFS_PROP_DEBUG_OPCODE,
	PDIC_SYSFS_PROP_CONTROL_GPIO,
	PDIC_SYSFS_PROP_USBPD_IDS,
	PDIC_SYSFS_PROP_USBPD_TYPE,	
	PDIC_SYSFS_PROP_CC_PIN_STATUS,
	PDIC_SYSFS_PROP_RAM_TEST,
	PDIC_SYSFS_PROP_SBU_ADC,
	PDIC_SYSFS_PROP_CC_ADC,
	PDIC_SYSFS_PROP_VSAFE0V_STATUS,
	PDIC_SYSFS_PROP_OVP_IC_SHUTDOWN,
	PDIC_SYSFS_PROP_HMD_POWER,
	PDIC_SYSFS_PROP_SET_WATER_THRESHOLD,
	PDIC_SYSFS_PROP_USBPD_WATER_CHECK,
	PDIC_SYSFS_PROP_15MODE_WATERTEST_TYPE,
	PDIC_SYSFS_PROP_VBUS_ADC,
	PDIC_SYSFS_PROP_USB_BOOT_MODE,
	PDIC_SYSFS_PROP_DP_SBU_SW_SEL,
	PDIC_SYSFS_PROP_NOVBUS_RP22K,
	PDIC_SYSFS_PROP_MAX_COUNT,
};
struct _pdic_data_t;
typedef struct _pdic_sysfs_property_t {
	enum pdic_sysfs_property *properties;
	size_t num_properties;
	int (*get_property)(struct _pdic_data_t *ppdic_data,
			     enum pdic_sysfs_property prop,
			     char *buf);
	ssize_t (*set_property)(struct _pdic_data_t *ppdic_data,
			     enum pdic_sysfs_property prop,
			     const char *buf,
				 size_t size);
	
	int (*property_is_writeable)(struct _pdic_data_t *ppdic_data,
				      enum pdic_sysfs_property prop);
	int (*property_is_writeonly)(struct _pdic_data_t *ppdic_data,
				      enum pdic_sysfs_property prop);
} pdic_sysfs_property_t, *ppdic_sysfs_property_t;

void pdic_sysfs_init_attrs(void);
#endif

