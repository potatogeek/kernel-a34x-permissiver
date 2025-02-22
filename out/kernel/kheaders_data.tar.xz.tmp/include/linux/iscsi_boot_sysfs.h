
#ifndef _ISCSI_BOOT_SYSFS_
#define _ISCSI_BOOT_SYSFS_


enum iscsi_boot_eth_properties_enum {
	ISCSI_BOOT_ETH_INDEX,
	ISCSI_BOOT_ETH_FLAGS,
	ISCSI_BOOT_ETH_IP_ADDR,
	ISCSI_BOOT_ETH_PREFIX_LEN,
	ISCSI_BOOT_ETH_SUBNET_MASK,
	ISCSI_BOOT_ETH_ORIGIN,
	ISCSI_BOOT_ETH_GATEWAY,
	ISCSI_BOOT_ETH_PRIMARY_DNS,
	ISCSI_BOOT_ETH_SECONDARY_DNS,
	ISCSI_BOOT_ETH_DHCP,
	ISCSI_BOOT_ETH_VLAN,
	ISCSI_BOOT_ETH_MAC,
	
	ISCSI_BOOT_ETH_HOSTNAME,
	ISCSI_BOOT_ETH_END_MARKER,
};

enum iscsi_boot_tgt_properties_enum {
	ISCSI_BOOT_TGT_INDEX,
	ISCSI_BOOT_TGT_FLAGS,
	ISCSI_BOOT_TGT_IP_ADDR,
	ISCSI_BOOT_TGT_PORT,
	ISCSI_BOOT_TGT_LUN,
	ISCSI_BOOT_TGT_CHAP_TYPE,
	ISCSI_BOOT_TGT_NIC_ASSOC,
	ISCSI_BOOT_TGT_NAME,
	ISCSI_BOOT_TGT_CHAP_NAME,
	ISCSI_BOOT_TGT_CHAP_SECRET,
	ISCSI_BOOT_TGT_REV_CHAP_NAME,
	ISCSI_BOOT_TGT_REV_CHAP_SECRET,
	ISCSI_BOOT_TGT_END_MARKER,
};

enum iscsi_boot_initiator_properties_enum {
	ISCSI_BOOT_INI_INDEX,
	ISCSI_BOOT_INI_FLAGS,
	ISCSI_BOOT_INI_ISNS_SERVER,
	ISCSI_BOOT_INI_SLP_SERVER,
	ISCSI_BOOT_INI_PRI_RADIUS_SERVER,
	ISCSI_BOOT_INI_SEC_RADIUS_SERVER,
	ISCSI_BOOT_INI_INITIATOR_NAME,
	ISCSI_BOOT_INI_END_MARKER,
};

enum iscsi_boot_acpitbl_properties_enum {
	ISCSI_BOOT_ACPITBL_SIGNATURE,
	ISCSI_BOOT_ACPITBL_OEM_ID,
	ISCSI_BOOT_ACPITBL_OEM_TABLE_ID,
};

struct attribute_group;

struct iscsi_boot_kobj {
	struct kobject kobj;
	struct attribute_group *attr_group;
	struct list_head list;

	
	void *data;
	
	ssize_t (*show) (void *data, int type, char *buf);

	
	umode_t (*is_visible) (void *data, int type);

	
	void (*release) (void *data);
};

struct iscsi_boot_kset {
	struct list_head kobj_list;
	struct kset *kset;
};

struct iscsi_boot_kobj *
iscsi_boot_create_initiator(struct iscsi_boot_kset *boot_kset, int index,
			    void *data,
			    ssize_t (*show) (void *data, int type, char *buf),
			    umode_t (*is_visible) (void *data, int type),
			    void (*release) (void *data));

struct iscsi_boot_kobj *
iscsi_boot_create_ethernet(struct iscsi_boot_kset *boot_kset, int index,
			   void *data,
			   ssize_t (*show) (void *data, int type, char *buf),
			   umode_t (*is_visible) (void *data, int type),
			   void (*release) (void *data));
struct iscsi_boot_kobj *
iscsi_boot_create_target(struct iscsi_boot_kset *boot_kset, int index,
			 void *data,
			 ssize_t (*show) (void *data, int type, char *buf),
			 umode_t (*is_visible) (void *data, int type),
			 void (*release) (void *data));

struct iscsi_boot_kobj *
iscsi_boot_create_acpitbl(struct iscsi_boot_kset *boot_kset, int index,
			  void *data,
			  ssize_t (*show)(void *data, int type, char *buf),
			  umode_t (*is_visible)(void *data, int type),
			  void (*release)(void *data));

struct iscsi_boot_kset *iscsi_boot_create_kset(const char *set_name);
struct iscsi_boot_kset *iscsi_boot_create_host_kset(unsigned int hostno);
void iscsi_boot_destroy_kset(struct iscsi_boot_kset *boot_kset);

#endif
