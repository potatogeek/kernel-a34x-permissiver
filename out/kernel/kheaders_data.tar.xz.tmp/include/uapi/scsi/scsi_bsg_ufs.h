/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef SCSI_BSG_UFS_H
#define SCSI_BSG_UFS_H

#include <linux/types.h>


#define UFS_CDB_SIZE	16
#define UPIU_TRANSACTION_UIC_CMD 0x1F

#define UIC_CMD_SIZE (sizeof(__u32) * 4)


struct utp_upiu_header {
	__be32 dword_0;
	__be32 dword_1;
	__be32 dword_2;
};


struct utp_upiu_query {
	__u8 opcode;
	__u8 idn;
	__u8 index;
	__u8 selector;
	__be16 reserved_osf;
	__be16 length;
	__be32 value;
	__be32 reserved[2];
};


struct utp_upiu_cmd {
	__be32 exp_data_transfer_len;
	__u8 cdb[UFS_CDB_SIZE];
};


struct utp_upiu_req {
	struct utp_upiu_header header;
	union {
		struct utp_upiu_cmd		sc;
		struct utp_upiu_query		qr;
		struct utp_upiu_query		tr;
		
		struct utp_upiu_query		uc;
	};
};


struct ufs_bsg_request {
	__u32 msgcode;
	struct utp_upiu_req upiu_req;
};


struct ufs_bsg_reply {
	
	__u32 result;

	
	__u32 reply_payload_rcv_len;

	struct utp_upiu_req upiu_rsp;
};
#endif 
