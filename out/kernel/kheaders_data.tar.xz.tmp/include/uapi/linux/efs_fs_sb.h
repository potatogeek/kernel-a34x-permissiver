/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef __EFS_FS_SB_H__
#define __EFS_FS_SB_H__

#include <linux/types.h>
#include <linux/magic.h>


#define EFS_MAGIC	0x072959
#define EFS_NEWMAGIC	0x07295a

#define IS_EFS_MAGIC(x)	((x == EFS_MAGIC) || (x == EFS_NEWMAGIC))

#define EFS_SUPER		1
#define EFS_ROOTINODE		2


struct efs_super {
	__be32		fs_size;        
	__be32		fs_firstcg;     
	__be32		fs_cgfsize;     
	__be16		fs_cgisize;     
	__be16		fs_sectors;     
	__be16		fs_heads;       
	__be16		fs_ncg;         
	__be16		fs_dirty;       
	__be32		fs_time;        
	__be32		fs_magic;       
	char		fs_fname[6];    
	char		fs_fpack[6];    
	__be32		fs_bmsize;      
	__be32		fs_tfree;       
	__be32		fs_tinode;      
	__be32		fs_bmblock;     
	__be32		fs_replsb;      
	__be32		fs_lastialloc;  
	char		fs_spare[20];   
	__be32		fs_checksum;    
};


struct efs_sb_info {
	__u32	fs_magic;	
	__u32	fs_start;	
	__u32	first_block;	
	__u32	total_blocks;	
	__u32	group_size;	 
	__u32	data_free;	
	__u32	inode_free;	
	__u16	inode_blocks;	
	__u16	total_groups;	
};

#endif 

