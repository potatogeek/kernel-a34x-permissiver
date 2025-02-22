/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __XEN_PUBLIC_IO_BLKIF_H__
#define __XEN_PUBLIC_IO_BLKIF_H__

#include <xen/interface/io/ring.h>
#include <xen/interface/grant_table.h>



typedef uint16_t blkif_vdev_t;
typedef uint64_t blkif_sector_t;




#define BLKIF_OP_READ              0
#define BLKIF_OP_WRITE             1

#define BLKIF_OP_WRITE_BARRIER     2


#define BLKIF_OP_FLUSH_DISKCACHE   3


#define BLKIF_OP_DISCARD           5


#define BLKIF_OP_INDIRECT          6


#define BLKIF_MAX_SEGMENTS_PER_REQUEST 11

#define BLKIF_MAX_INDIRECT_PAGES_PER_REQUEST 8

struct blkif_request_segment {
		grant_ref_t gref;        
		
		
		uint8_t     first_sect, last_sect;
};

struct blkif_request_rw {
	uint8_t        nr_segments;  
	blkif_vdev_t   handle;       
#ifndef CONFIG_X86_32
	uint32_t       _pad1;	     
#endif
	uint64_t       id;           
	blkif_sector_t sector_number;
	struct blkif_request_segment seg[BLKIF_MAX_SEGMENTS_PER_REQUEST];
} __attribute__((__packed__));

struct blkif_request_discard {
	uint8_t        flag;         
#define BLKIF_DISCARD_SECURE (1<<0)  
	blkif_vdev_t   _pad1;        
#ifndef CONFIG_X86_32
	uint32_t       _pad2;        
#endif
	uint64_t       id;           
	blkif_sector_t sector_number;
	uint64_t       nr_sectors;
	uint8_t        _pad3;
} __attribute__((__packed__));

struct blkif_request_other {
	uint8_t      _pad1;
	blkif_vdev_t _pad2;        
#ifndef CONFIG_X86_32
	uint32_t     _pad3;        
#endif
	uint64_t     id;           
} __attribute__((__packed__));

struct blkif_request_indirect {
	uint8_t        indirect_op;
	uint16_t       nr_segments;
#ifndef CONFIG_X86_32
	uint32_t       _pad1;        
#endif
	uint64_t       id;
	blkif_sector_t sector_number;
	blkif_vdev_t   handle;
	uint16_t       _pad2;
	grant_ref_t    indirect_grefs[BLKIF_MAX_INDIRECT_PAGES_PER_REQUEST];
#ifndef CONFIG_X86_32
	uint32_t      _pad3;         
#else
	uint64_t      _pad3;         
#endif
} __attribute__((__packed__));

struct blkif_request {
	uint8_t        operation;    
	union {
		struct blkif_request_rw rw;
		struct blkif_request_discard discard;
		struct blkif_request_other other;
		struct blkif_request_indirect indirect;
	} u;
} __attribute__((__packed__));

struct blkif_response {
	uint64_t        id;              
	uint8_t         operation;       
	int16_t         status;          
};


 
#define BLKIF_RSP_EOPNOTSUPP  -2
 
#define BLKIF_RSP_ERROR       -1
 
#define BLKIF_RSP_OKAY         0



DEFINE_RING_TYPES(blkif, struct blkif_request, struct blkif_response);

#define VDISK_CDROM        0x1
#define VDISK_REMOVABLE    0x2
#define VDISK_READONLY     0x4


#define XEN_IDE0_MAJOR	3
#define XEN_IDE1_MAJOR	22
#define XEN_SCSI_DISK0_MAJOR	8
#define XEN_SCSI_DISK1_MAJOR	65
#define XEN_SCSI_DISK2_MAJOR	66
#define XEN_SCSI_DISK3_MAJOR	67
#define XEN_SCSI_DISK4_MAJOR	68
#define XEN_SCSI_DISK5_MAJOR	69
#define XEN_SCSI_DISK6_MAJOR	70
#define XEN_SCSI_DISK7_MAJOR	71
#define XEN_SCSI_DISK8_MAJOR	128
#define XEN_SCSI_DISK9_MAJOR	129
#define XEN_SCSI_DISK10_MAJOR	130
#define XEN_SCSI_DISK11_MAJOR	131
#define XEN_SCSI_DISK12_MAJOR	132
#define XEN_SCSI_DISK13_MAJOR	133
#define XEN_SCSI_DISK14_MAJOR	134
#define XEN_SCSI_DISK15_MAJOR	135

#endif 
