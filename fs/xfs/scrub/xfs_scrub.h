// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017 Oracle.  All Rights Reserved.
 * Author: Darrick J. Wong <darrick.wong@oracle.com>
 */
#ifndef __XFS_SCRUB_H__
#define __XFS_SCRUB_H__

#ifndef CONFIG_XFS_ONLINE_SCRUB
<<<<<<< HEAD
# define xfs_scrub_metadata(ip, sm)	(-ENOTTY)
#else
int xfs_scrub_metadata(struct xfs_inode *ip, struct xfs_scrub_metadata *sm);
=======
# define xfs_scrub_metadata(file, sm)	(-ENOTTY)
#else
int xfs_scrub_metadata(struct file *file, struct xfs_scrub_metadata *sm);
>>>>>>> upstream/android-13
#endif /* CONFIG_XFS_ONLINE_SCRUB */

#endif	/* __XFS_SCRUB_H__ */
