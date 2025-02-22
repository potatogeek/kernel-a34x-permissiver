/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_SED_OPAL_H
#define _UAPI_SED_OPAL_H

#include <linux/types.h>

#define OPAL_KEY_MAX 256
#define OPAL_MAX_LRS 9

enum opal_mbr {
	OPAL_MBR_ENABLE = 0x0,
	OPAL_MBR_DISABLE = 0x01,
};

enum opal_user {
	OPAL_ADMIN1 = 0x0,
	OPAL_USER1 = 0x01,
	OPAL_USER2 = 0x02,
	OPAL_USER3 = 0x03,
	OPAL_USER4 = 0x04,
	OPAL_USER5 = 0x05,
	OPAL_USER6 = 0x06,
	OPAL_USER7 = 0x07,
	OPAL_USER8 = 0x08,
	OPAL_USER9 = 0x09,
};

enum opal_lock_state {
	OPAL_RO = 0x01, 
	OPAL_RW = 0x02, 
	OPAL_LK = 0x04, 
};

struct opal_key {
	__u8 lr;
	__u8 key_len;
	__u8 __align[6];
	__u8 key[OPAL_KEY_MAX];
};

struct opal_lr_act {
	struct opal_key key;
	__u32 sum;
	__u8    num_lrs;
	__u8 lr[OPAL_MAX_LRS];
	__u8 align[2]; 
};

struct opal_session_info {
	__u32 sum;
	__u32 who;
	struct opal_key opal_key;
};

struct opal_user_lr_setup {
	__u64 range_start;
	__u64 range_length;
	__u32 RLE; 
	__u32 WLE; 
	struct opal_session_info session;
};

struct opal_lock_unlock {
	struct opal_session_info session;
	__u32 l_state;
	__u8 __align[4];
};

struct opal_new_pw {
	struct opal_session_info session;

	
	struct opal_session_info new_user_pw;
};

struct opal_mbr_data {
	struct opal_key key;
	__u8 enable_disable;
	__u8 __align[7];
};

#define IOC_OPAL_SAVE		    _IOW('p', 220, struct opal_lock_unlock)
#define IOC_OPAL_LOCK_UNLOCK	    _IOW('p', 221, struct opal_lock_unlock)
#define IOC_OPAL_TAKE_OWNERSHIP	    _IOW('p', 222, struct opal_key)
#define IOC_OPAL_ACTIVATE_LSP       _IOW('p', 223, struct opal_lr_act)
#define IOC_OPAL_SET_PW             _IOW('p', 224, struct opal_new_pw)
#define IOC_OPAL_ACTIVATE_USR       _IOW('p', 225, struct opal_session_info)
#define IOC_OPAL_REVERT_TPR         _IOW('p', 226, struct opal_key)
#define IOC_OPAL_LR_SETUP           _IOW('p', 227, struct opal_user_lr_setup)
#define IOC_OPAL_ADD_USR_TO_LR      _IOW('p', 228, struct opal_lock_unlock)
#define IOC_OPAL_ENABLE_DISABLE_MBR _IOW('p', 229, struct opal_mbr_data)
#define IOC_OPAL_ERASE_LR           _IOW('p', 230, struct opal_session_info)
#define IOC_OPAL_SECURE_ERASE_LR    _IOW('p', 231, struct opal_session_info)

#endif 
