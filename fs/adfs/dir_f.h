<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/fs/adfs/dir_f.h
 *
 *  Copyright (C) 1999 Russell King
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 *  Structures of directories on the F format disk
 */
#ifndef ADFS_DIR_F_H
#define ADFS_DIR_F_H

/*
 * Directory header
 */
struct adfs_dirheader {
<<<<<<< HEAD
	unsigned char startmasseq;
	unsigned char startname[4];
};
=======
	__u8 startmasseq;
	__u8 startname[4];
} __attribute__((packed));
>>>>>>> upstream/android-13

#define ADFS_NEWDIR_SIZE	2048
#define ADFS_NUM_DIR_ENTRIES	77

/*
 * Directory entries
 */
struct adfs_direntry {
#define ADFS_F_NAME_LEN 10
	char dirobname[ADFS_F_NAME_LEN];
	__u8 dirload[4];
	__u8 direxec[4];
	__u8 dirlen[4];
	__u8 dirinddiscadd[3];
	__u8 newdiratts;
<<<<<<< HEAD
};
=======
} __attribute__((packed));
>>>>>>> upstream/android-13

/*
 * Directory tail
 */
<<<<<<< HEAD
union adfs_dirtail {
	struct {
		unsigned char dirlastmask;
		char dirname[10];
		unsigned char dirparent[3];
		char dirtitle[19];
		unsigned char reserved[14];
		unsigned char endmasseq;
		unsigned char endname[4];
		unsigned char dircheckbyte;
	} old;
	struct {
		unsigned char dirlastmask;
		unsigned char reserved[2];
		unsigned char dirparent[3];
		char dirtitle[19];
		char dirname[10];
		unsigned char endmasseq;
		unsigned char endname[4];
		unsigned char dircheckbyte;
	} new;
=======
struct adfs_olddirtail {
	__u8 dirlastmask;
	char dirname[10];
	__u8 dirparent[3];
	char dirtitle[19];
	__u8 reserved[14];
	__u8 endmasseq;
	__u8 endname[4];
	__u8 dircheckbyte;
} __attribute__((packed));

struct adfs_newdirtail {
	__u8 dirlastmask;
	__u8 reserved[2];
	__u8 dirparent[3];
	char dirtitle[19];
	char dirname[10];
	__u8 endmasseq;
	__u8 endname[4];
	__u8 dircheckbyte;
} __attribute__((packed));

union adfs_dirtail {
	struct adfs_olddirtail old;
	struct adfs_newdirtail new;
>>>>>>> upstream/android-13
};

#endif
