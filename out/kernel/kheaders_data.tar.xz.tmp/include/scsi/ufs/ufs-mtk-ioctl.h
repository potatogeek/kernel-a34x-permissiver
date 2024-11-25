/* SPDX-License-Identifier: GPL-2.0 */


#ifndef UFS_MTK_IOCTL_H__
#define UFS_MTK_IOCTL_H__

#include <linux/types.h>


#define UFS_IOCTL_QUERY         0x5388  
#define UFS_IOCTL_FFU           0x5389  
#define UFS_IOCTL_GET_FW_VER    0x5390  
#define UFS_IOCTL_RPMB          0x5391  

#if defined(CONFIG_SCSI_UFS_FEATURE)
#define UFSFEATURE_QUERY_OPCODE		0x5500
#endif

#define UFS_IOCTL_FFU_MAX_FW_SIZE_BYTES             (512L * 1024)
#define UFS_IOCTL_FFU_MAX_FW_VER_BYTES              (4)
#define UFS_IOCTL_FFU_MAX_FW_VER_STRING_DESCR_BYTES	(10)

struct ufs_ioctl_query_data {
	
	__u32 opcode;
	
	__u8 idn;
	__u8 idx;
	
	__u16 buf_byte;
	
	__u8 *buf_ptr;
};

struct ufs_ioctl_query_data_hpb {
	
	__u32 opcode;
	
	__u8 idn;
	
	__u16 buf_size;
	
	__u8 buffer[0];
};


struct ufs_ioctl_ffu_data {
	
	__u32 buf_byte;

	
	__u8 *buf_ptr;
};


struct ufs_ioctl_query_fw_ver_data {
	
	__u16 buf_byte;

	
	__u8 *buf_ptr;
};

#endif 

