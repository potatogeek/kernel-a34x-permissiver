/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */


#ifndef __MTD_ABI_H__
#define __MTD_ABI_H__

#include <linux/types.h>

struct erase_info_user {
	__u32 start;
	__u32 length;
};

struct erase_info_user64 {
	__u64 start;
	__u64 length;
};

struct mtd_oob_buf {
	__u32 start;
	__u32 length;
	unsigned char __user *ptr;
};

struct mtd_oob_buf64 {
	__u64 start;
	__u32 pad;
	__u32 length;
	__u64 usr_ptr;
};


enum {
	MTD_OPS_PLACE_OOB = 0,
	MTD_OPS_AUTO_OOB = 1,
	MTD_OPS_RAW = 2,
};


struct mtd_write_req {
	__u64 start;
	__u64 len;
	__u64 ooblen;
	__u64 usr_data;
	__u64 usr_oob;
	__u8 mode;
	__u8 padding[7];
};

#define MTD_ABSENT		0
#define MTD_RAM			1
#define MTD_ROM			2
#define MTD_NORFLASH		3
#define MTD_NANDFLASH		4	
#define MTD_DATAFLASH		6
#define MTD_UBIVOLUME		7
#define MTD_MLCNANDFLASH	8	

#define MTD_WRITEABLE		0x400	
#define MTD_BIT_WRITEABLE	0x800	
#define MTD_NO_ERASE		0x1000	
#define MTD_POWERUP_LOCK	0x2000	


#define MTD_CAP_ROM		0
#define MTD_CAP_RAM		(MTD_WRITEABLE | MTD_BIT_WRITEABLE | MTD_NO_ERASE)
#define MTD_CAP_NORFLASH	(MTD_WRITEABLE | MTD_BIT_WRITEABLE)
#define MTD_CAP_NANDFLASH	(MTD_WRITEABLE)
#define MTD_CAP_NVRAM		(MTD_WRITEABLE | MTD_BIT_WRITEABLE | MTD_NO_ERASE)


#define MTD_NANDECC_OFF		0	// Switch off ECC (Not recommended)
#define MTD_NANDECC_PLACE	1	// Use the given placement in the structure (YAFFS1 legacy mode)
#define MTD_NANDECC_AUTOPLACE	2	// Use the default placement scheme
#define MTD_NANDECC_PLACEONLY	3	// Use the given placement in the structure (Do not store ecc result on read)
#define MTD_NANDECC_AUTOPL_USR 	4	// Use the given autoplacement scheme rather than using the default


#define MTD_OTP_OFF		0
#define MTD_OTP_FACTORY		1
#define MTD_OTP_USER		2

struct mtd_info_user {
	__u8 type;
	__u32 flags;
	__u32 size;	
	__u32 erasesize;
	__u32 writesize;
	__u32 oobsize;	
	__u64 padding;	
};

struct region_info_user {
	__u32 offset;		
	__u32 erasesize;	
	__u32 numblocks;	
	__u32 regionindex;
};

struct otp_info {
	__u32 start;
	__u32 length;
	__u32 locked;
};




#define MEMGETINFO		_IOR('M', 1, struct mtd_info_user)

#define MEMERASE		_IOW('M', 2, struct erase_info_user)

#define MEMWRITEOOB		_IOWR('M', 3, struct mtd_oob_buf)

#define MEMREADOOB		_IOWR('M', 4, struct mtd_oob_buf)

#define MEMLOCK			_IOW('M', 5, struct erase_info_user)

#define MEMUNLOCK		_IOW('M', 6, struct erase_info_user)

#define MEMGETREGIONCOUNT	_IOR('M', 7, int)

#define MEMGETREGIONINFO	_IOWR('M', 8, struct region_info_user)

#define MEMGETOOBSEL		_IOR('M', 10, struct nand_oobinfo)

#define MEMGETBADBLOCK		_IOW('M', 11, __kernel_loff_t)

#define MEMSETBADBLOCK		_IOW('M', 12, __kernel_loff_t)

#define OTPSELECT		_IOR('M', 13, int)

#define OTPGETREGIONCOUNT	_IOW('M', 14, int)

#define OTPGETREGIONINFO	_IOW('M', 15, struct otp_info)

#define OTPLOCK			_IOR('M', 16, struct otp_info)

#define ECCGETLAYOUT		_IOR('M', 17, struct nand_ecclayout_user)

#define ECCGETSTATS		_IOR('M', 18, struct mtd_ecc_stats)

#define MTDFILEMODE		_IO('M', 19)

#define MEMERASE64		_IOW('M', 20, struct erase_info_user64)

#define MEMWRITEOOB64		_IOWR('M', 21, struct mtd_oob_buf64)

#define MEMREADOOB64		_IOWR('M', 22, struct mtd_oob_buf64)

#define MEMISLOCKED		_IOR('M', 23, struct erase_info_user)

#define MEMWRITE		_IOWR('M', 24, struct mtd_write_req)


struct nand_oobinfo {
	__u32 useecc;
	__u32 eccbytes;
	__u32 oobfree[8][2];
	__u32 eccpos[32];
};

struct nand_oobfree {
	__u32 offset;
	__u32 length;
};

#define MTD_MAX_OOBFREE_ENTRIES	8
#define MTD_MAX_ECCPOS_ENTRIES	64

struct nand_ecclayout_user {
	__u32 eccbytes;
	__u32 eccpos[MTD_MAX_ECCPOS_ENTRIES];
	__u32 oobavail;
	struct nand_oobfree oobfree[MTD_MAX_OOBFREE_ENTRIES];
};


struct mtd_ecc_stats {
	__u32 corrected;
	__u32 failed;
	__u32 badblocks;
	__u32 bbtblocks;
};


enum mtd_file_modes {
	MTD_FILE_MODE_NORMAL = MTD_OTP_OFF,
	MTD_FILE_MODE_OTP_FACTORY = MTD_OTP_FACTORY,
	MTD_FILE_MODE_OTP_USER = MTD_OTP_USER,
	MTD_FILE_MODE_RAW,
};

static inline int mtd_type_is_nand_user(const struct mtd_info_user *mtd)
{
	return mtd->type == MTD_NANDFLASH || mtd->type == MTD_MLCNANDFLASH;
}

#endif 
