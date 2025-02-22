<<<<<<< HEAD
/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
>>>>>>> upstream/android-13
 * blockcheck.h
 *
 * Checksum and ECC codes for the OCFS2 userspace library.
 *
 * Copyright (C) 2004, 2008 Oracle.  All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License, version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef OCFS2_BLOCKCHECK_H
#define OCFS2_BLOCKCHECK_H


/* Count errors and error correction from blockcheck.c */
struct ocfs2_blockcheck_stats {
	spinlock_t b_lock;
	u64 b_check_count;	/* Number of blocks we've checked */
	u64 b_failure_count;	/* Number of failed checksums */
	u64 b_recover_count;	/* Number of blocks fixed by ecc */

	/*
	 * debugfs entries, used if this is passed to
	 * ocfs2_blockcheck_stats_debugfs_install()
	 */
	struct dentry *b_debug_dir;	/* Parent of the debugfs  files */
<<<<<<< HEAD
	struct dentry *b_debug_check;	/* Exposes b_check_count */
	struct dentry *b_debug_failure;	/* Exposes b_failure_count */
	struct dentry *b_debug_recover;	/* Exposes b_recover_count */
=======
>>>>>>> upstream/android-13
};


/* High level block API */
void ocfs2_compute_meta_ecc(struct super_block *sb, void *data,
			    struct ocfs2_block_check *bc);
int ocfs2_validate_meta_ecc(struct super_block *sb, void *data,
			    struct ocfs2_block_check *bc);
void ocfs2_compute_meta_ecc_bhs(struct super_block *sb,
				struct buffer_head **bhs, int nr,
				struct ocfs2_block_check *bc);
int ocfs2_validate_meta_ecc_bhs(struct super_block *sb,
				struct buffer_head **bhs, int nr,
				struct ocfs2_block_check *bc);

/* Lower level API */
void ocfs2_block_check_compute(void *data, size_t blocksize,
			       struct ocfs2_block_check *bc);
int ocfs2_block_check_validate(void *data, size_t blocksize,
			       struct ocfs2_block_check *bc,
			       struct ocfs2_blockcheck_stats *stats);
void ocfs2_block_check_compute_bhs(struct buffer_head **bhs, int nr,
				   struct ocfs2_block_check *bc);
int ocfs2_block_check_validate_bhs(struct buffer_head **bhs, int nr,
				   struct ocfs2_block_check *bc,
				   struct ocfs2_blockcheck_stats *stats);

/* Debug Initialization */
<<<<<<< HEAD
int ocfs2_blockcheck_stats_debugfs_install(struct ocfs2_blockcheck_stats *stats,
					   struct dentry *parent);
=======
void ocfs2_blockcheck_stats_debugfs_install(struct ocfs2_blockcheck_stats *stats,
					    struct dentry *parent);
>>>>>>> upstream/android-13
void ocfs2_blockcheck_stats_debugfs_remove(struct ocfs2_blockcheck_stats *stats);

/*
 * Hamming code functions
 */

/*
 * Encoding hamming code parity bits for a buffer.
 *
 * This is the low level encoder function.  It can be called across
 * multiple hunks just like the crc32 code.  'd' is the number of bits
 * _in_this_hunk_.  nr is the bit offset of this hunk.  So, if you had
 * two 512B buffers, you would do it like so:
 *
 * parity = ocfs2_hamming_encode(0, buf1, 512 * 8, 0);
 * parity = ocfs2_hamming_encode(parity, buf2, 512 * 8, 512 * 8);
 *
 * If you just have one buffer, use ocfs2_hamming_encode_block().
 */
u32 ocfs2_hamming_encode(u32 parity, void *data, unsigned int d,
			 unsigned int nr);
/*
 * Fix a buffer with a bit error.  The 'fix' is the original parity
 * xor'd with the parity calculated now.
 *
 * Like ocfs2_hamming_encode(), this can handle hunks.  nr is the bit
 * offset of the current hunk.  If bit to be fixed is not part of the
 * current hunk, this does nothing.
 *
 * If you only have one buffer, use ocfs2_hamming_fix_block().
 */
void ocfs2_hamming_fix(void *data, unsigned int d, unsigned int nr,
		       unsigned int fix);

/* Convenience wrappers for a single buffer of data */
extern u32 ocfs2_hamming_encode_block(void *data, unsigned int blocksize);
extern void ocfs2_hamming_fix_block(void *data, unsigned int blocksize,
				    unsigned int fix);
#endif
