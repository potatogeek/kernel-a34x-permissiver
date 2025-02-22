/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR Linux-OpenIB) */


#ifndef IB_USER_IOCTL_VERBS_H
#define IB_USER_IOCTL_VERBS_H

#include <linux/types.h>

#ifndef RDMA_UAPI_PTR
#define RDMA_UAPI_PTR(_type, _name)	__aligned_u64 _name
#endif

enum ib_uverbs_access_flags {
	IB_UVERBS_ACCESS_LOCAL_WRITE = 1 << 0,
	IB_UVERBS_ACCESS_REMOTE_WRITE = 1 << 1,
	IB_UVERBS_ACCESS_REMOTE_READ = 1 << 2,
	IB_UVERBS_ACCESS_REMOTE_ATOMIC = 1 << 3,
	IB_UVERBS_ACCESS_MW_BIND = 1 << 4,
	IB_UVERBS_ACCESS_ZERO_BASED = 1 << 5,
	IB_UVERBS_ACCESS_ON_DEMAND = 1 << 6,
	IB_UVERBS_ACCESS_HUGETLB = 1 << 7,
};

enum ib_uverbs_query_port_cap_flags {
	IB_UVERBS_PCF_SM = 1 << 1,
	IB_UVERBS_PCF_NOTICE_SUP = 1 << 2,
	IB_UVERBS_PCF_TRAP_SUP = 1 << 3,
	IB_UVERBS_PCF_OPT_IPD_SUP = 1 << 4,
	IB_UVERBS_PCF_AUTO_MIGR_SUP = 1 << 5,
	IB_UVERBS_PCF_SL_MAP_SUP = 1 << 6,
	IB_UVERBS_PCF_MKEY_NVRAM = 1 << 7,
	IB_UVERBS_PCF_PKEY_NVRAM = 1 << 8,
	IB_UVERBS_PCF_LED_INFO_SUP = 1 << 9,
	IB_UVERBS_PCF_SM_DISABLED = 1 << 10,
	IB_UVERBS_PCF_SYS_IMAGE_GUID_SUP = 1 << 11,
	IB_UVERBS_PCF_PKEY_SW_EXT_PORT_TRAP_SUP = 1 << 12,
	IB_UVERBS_PCF_EXTENDED_SPEEDS_SUP = 1 << 14,
	IB_UVERBS_PCF_CM_SUP = 1 << 16,
	IB_UVERBS_PCF_SNMP_TUNNEL_SUP = 1 << 17,
	IB_UVERBS_PCF_REINIT_SUP = 1 << 18,
	IB_UVERBS_PCF_DEVICE_MGMT_SUP = 1 << 19,
	IB_UVERBS_PCF_VENDOR_CLASS_SUP = 1 << 20,
	IB_UVERBS_PCF_DR_NOTICE_SUP = 1 << 21,
	IB_UVERBS_PCF_CAP_MASK_NOTICE_SUP = 1 << 22,
	IB_UVERBS_PCF_BOOT_MGMT_SUP = 1 << 23,
	IB_UVERBS_PCF_LINK_LATENCY_SUP = 1 << 24,
	IB_UVERBS_PCF_CLIENT_REG_SUP = 1 << 25,
	
	IB_UVERBS_PCF_LINK_SPEED_WIDTH_TABLE_SUP = 1 << 27,
	IB_UVERBS_PCF_VENDOR_SPECIFIC_MADS_TABLE_SUP = 1 << 28,
	IB_UVERBS_PCF_MCAST_PKEY_TRAP_SUPPRESSION_SUP = 1 << 29,
	IB_UVERBS_PCF_MCAST_FDB_TOP_SUP = 1 << 30,
	IB_UVERBS_PCF_HIERARCHY_INFO_SUP = 1ULL << 31,

	
	IB_UVERBS_PCF_IP_BASED_GIDS = 1 << 26,
};

enum ib_uverbs_query_port_flags {
	IB_UVERBS_QPF_GRH_REQUIRED = 1 << 0,
};

enum ib_uverbs_flow_action_esp_keymat {
	IB_UVERBS_FLOW_ACTION_ESP_KEYMAT_AES_GCM,
};

enum ib_uverbs_flow_action_esp_keymat_aes_gcm_iv_algo {
	IB_UVERBS_FLOW_ACTION_IV_ALGO_SEQ,
};

struct ib_uverbs_flow_action_esp_keymat_aes_gcm {
	__aligned_u64	iv;
	__u32		iv_algo; 

	__u32		salt;
	__u32		icv_len;

	__u32		key_len;
	__u32		aes_key[256 / 32];
};

enum ib_uverbs_flow_action_esp_replay {
	IB_UVERBS_FLOW_ACTION_ESP_REPLAY_NONE,
	IB_UVERBS_FLOW_ACTION_ESP_REPLAY_BMP,
};

struct ib_uverbs_flow_action_esp_replay_bmp {
	__u32	size;
};

enum ib_uverbs_flow_action_esp_flags {
	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_INLINE_CRYPTO	= 0UL << 0,	
	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_FULL_OFFLOAD	= 1UL << 0,

	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_TUNNEL		= 0UL << 1,	
	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_TRANSPORT	= 1UL << 1,

	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_DECRYPT		= 0UL << 2,	
	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ENCRYPT		= 1UL << 2,

	IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW	= 1UL << 3,
};

struct ib_uverbs_flow_action_esp_encap {
	
	RDMA_UAPI_PTR(void *, val_ptr); 
	RDMA_UAPI_PTR(struct ib_uverbs_flow_action_esp_encap *, next_ptr);
	__u16	len;		
	__u16	type;		
};

struct ib_uverbs_flow_action_esp {
	__u32		spi;
	__u32		seq;
	__u32		tfc_pad;
	__u32		flags;
	__aligned_u64	hard_limit_pkts;
};

enum ib_uverbs_read_counters_flags {
	
	IB_UVERBS_READ_COUNTERS_PREFER_CACHED = 1 << 0,
};

#endif
