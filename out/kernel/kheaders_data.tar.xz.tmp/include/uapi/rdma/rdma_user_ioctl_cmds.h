

#ifndef RDMA_USER_IOCTL_CMDS_H
#define RDMA_USER_IOCTL_CMDS_H

#include <linux/types.h>
#include <linux/ioctl.h>


#define RDMA_IOCTL_MAGIC	0x1b
#define RDMA_VERBS_IOCTL \
	_IOWR(RDMA_IOCTL_MAGIC, 1, struct ib_uverbs_ioctl_hdr)

enum {
	
	UVERBS_ATTR_F_MANDATORY = 1U << 0,
	
	UVERBS_ATTR_F_VALID_OUTPUT = 1U << 1,
};

struct ib_uverbs_attr {
	__u16 attr_id;		
	__u16 len;		
	__u16 flags;		
	union {
		struct {
			__u8 elem_id;
			__u8 reserved;
		} enum_data;
		__u16 reserved;
	} attr_data;
	union {
		
		__aligned_u64 data;
		
		__s64 data_s64;
	};
};

struct ib_uverbs_ioctl_hdr {
	__u16 length;
	__u16 object_id;
	__u16 method_id;
	__u16 num_attrs;
	__aligned_u64 reserved1;
	__u32 driver_id;
	__u32 reserved2;
	struct ib_uverbs_attr  attrs[0];
};

enum rdma_driver_id {
	RDMA_DRIVER_UNKNOWN,
	RDMA_DRIVER_MLX5,
	RDMA_DRIVER_MLX4,
	RDMA_DRIVER_CXGB3,
	RDMA_DRIVER_CXGB4,
	RDMA_DRIVER_MTHCA,
	RDMA_DRIVER_BNXT_RE,
	RDMA_DRIVER_OCRDMA,
	RDMA_DRIVER_NES,
	RDMA_DRIVER_I40IW,
	RDMA_DRIVER_VMW_PVRDMA,
	RDMA_DRIVER_QEDR,
	RDMA_DRIVER_HNS,
	RDMA_DRIVER_USNIC,
	RDMA_DRIVER_RXE,
	RDMA_DRIVER_HFI1,
	RDMA_DRIVER_QIB,
};

#endif
