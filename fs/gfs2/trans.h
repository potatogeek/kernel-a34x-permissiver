<<<<<<< HEAD
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef __TRANS_DOT_H__
#define __TRANS_DOT_H__

#include <linux/buffer_head.h>
struct gfs2_sbd;
struct gfs2_rgrpd;
struct gfs2_glock;

#define RES_DINODE	1
#define RES_INDIRECT	1
#define RES_JDATA	1
#define RES_DATA	1
#define RES_LEAF	1
#define RES_RG_HDR	1
#define RES_RG_BIT	2
#define RES_EATTR	1
#define RES_STATFS	1
#define RES_QUOTA	2

/* reserve either the number of blocks to be allocated plus the rg header
 * block, or all of the blocks in the rg, whichever is smaller */
static inline unsigned int gfs2_rg_blocks(const struct gfs2_inode *ip, unsigned requested)
{
<<<<<<< HEAD
	struct gfs2_rgrpd *rgd = ip->i_res.rs_rbm.rgd;
=======
	struct gfs2_rgrpd *rgd = ip->i_res.rs_rgd;
>>>>>>> upstream/android-13

	if (requested < rgd->rd_length)
		return requested + 1;
	return rgd->rd_length;
}

<<<<<<< HEAD
=======
extern int __gfs2_trans_begin(struct gfs2_trans *tr, struct gfs2_sbd *sdp,
			      unsigned int blocks, unsigned int revokes,
			      unsigned long ip);
>>>>>>> upstream/android-13
extern int gfs2_trans_begin(struct gfs2_sbd *sdp, unsigned int blocks,
			    unsigned int revokes);

extern void gfs2_trans_end(struct gfs2_sbd *sdp);
extern void gfs2_trans_add_data(struct gfs2_glock *gl, struct buffer_head *bh);
extern void gfs2_trans_add_meta(struct gfs2_glock *gl, struct buffer_head *bh);
extern void gfs2_trans_add_revoke(struct gfs2_sbd *sdp, struct gfs2_bufdata *bd);
<<<<<<< HEAD
extern void gfs2_trans_add_unrevoke(struct gfs2_sbd *sdp, u64 blkno, unsigned int len);
=======
extern void gfs2_trans_remove_revoke(struct gfs2_sbd *sdp, u64 blkno, unsigned int len);
extern void gfs2_trans_free(struct gfs2_sbd *sdp, struct gfs2_trans *tr);
>>>>>>> upstream/android-13

#endif /* __TRANS_DOT_H__ */
