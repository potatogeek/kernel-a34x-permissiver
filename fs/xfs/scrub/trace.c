// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017 Oracle.  All Rights Reserved.
 * Author: Darrick J. Wong <darrick.wong@oracle.com>
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
<<<<<<< HEAD
#include "xfs_defer.h"
#include "xfs_da_format.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_trans.h"
#include "xfs_bit.h"
#include "scrub/xfs_scrub.h"
#include "scrub/scrub.h"
#include "scrub/common.h"
=======
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "scrub/scrub.h"
#include "xfs_ag.h"
>>>>>>> upstream/android-13

/* Figure out which block the btree cursor was pointing to. */
static inline xfs_fsblock_t
xchk_btree_cur_fsbno(
	struct xfs_btree_cur	*cur,
	int			level)
{
	if (level < cur->bc_nlevels && cur->bc_bufs[level])
<<<<<<< HEAD
		return XFS_DADDR_TO_FSB(cur->bc_mp, cur->bc_bufs[level]->b_bn);
	else if (level == cur->bc_nlevels - 1 &&
		 cur->bc_flags & XFS_BTREE_LONG_PTRS)
		return XFS_INO_TO_FSB(cur->bc_mp, cur->bc_private.b.ip->i_ino);
	else if (!(cur->bc_flags & XFS_BTREE_LONG_PTRS))
		return XFS_AGB_TO_FSB(cur->bc_mp, cur->bc_private.a.agno, 0);
=======
		return XFS_DADDR_TO_FSB(cur->bc_mp,
				xfs_buf_daddr(cur->bc_bufs[level]));
	if (level == cur->bc_nlevels - 1 && cur->bc_flags & XFS_BTREE_LONG_PTRS)
		return XFS_INO_TO_FSB(cur->bc_mp, cur->bc_ino.ip->i_ino);
	if (!(cur->bc_flags & XFS_BTREE_LONG_PTRS))
		return XFS_AGB_TO_FSB(cur->bc_mp, cur->bc_ag.pag->pag_agno, 0);
>>>>>>> upstream/android-13
	return NULLFSBLOCK;
}

/*
 * We include this last to have the helpers above available for the trace
 * event implementations.
 */
#define CREATE_TRACE_POINTS
#include "scrub/trace.h"
