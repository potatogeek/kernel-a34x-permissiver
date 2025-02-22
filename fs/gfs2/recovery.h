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

#ifndef __RECOVERY_DOT_H__
#define __RECOVERY_DOT_H__

#include "incore.h"

extern struct workqueue_struct *gfs_recovery_wq;

<<<<<<< HEAD
static inline void gfs2_replay_incr_blk(struct gfs2_jdesc *jd, unsigned int *blk)
=======
static inline void gfs2_replay_incr_blk(struct gfs2_jdesc *jd, u32 *blk)
>>>>>>> upstream/android-13
{
	if (++*blk == jd->jd_blocks)
	        *blk = 0;
}

extern int gfs2_replay_read_block(struct gfs2_jdesc *jd, unsigned int blk,
			   struct buffer_head **bh);

extern int gfs2_revoke_add(struct gfs2_jdesc *jd, u64 blkno, unsigned int where);
extern int gfs2_revoke_check(struct gfs2_jdesc *jd, u64 blkno, unsigned int where);
extern void gfs2_revoke_clean(struct gfs2_jdesc *jd);

<<<<<<< HEAD
extern int gfs2_find_jhead(struct gfs2_jdesc *jd,
		    struct gfs2_log_header_host *head);
extern int gfs2_recover_journal(struct gfs2_jdesc *gfs2_jd, bool wait);
extern void gfs2_recover_func(struct work_struct *work);
=======
extern int gfs2_recover_journal(struct gfs2_jdesc *gfs2_jd, bool wait);
extern void gfs2_recover_func(struct work_struct *work);
extern int __get_log_header(struct gfs2_sbd *sdp,
			    const struct gfs2_log_header *lh, unsigned int blkno,
			    struct gfs2_log_header_host *head);
>>>>>>> upstream/android-13

#endif /* __RECOVERY_DOT_H__ */

