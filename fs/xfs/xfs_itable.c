// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_ialloc.h"
#include "xfs_ialloc_btree.h"
<<<<<<< HEAD
#include "xfs_itable.h"
#include "xfs_error.h"
#include "xfs_trace.h"
#include "xfs_icache.h"

/*
 * Return stat information for one inode.
 * Return 0 if ok, else errno.
 */
int
xfs_bulkstat_one_int(
	struct xfs_mount	*mp,		/* mount point for filesystem */
	xfs_ino_t		ino,		/* inode to get data for */
	void __user		*buffer,	/* buffer to place output in */
	int			ubsize,		/* size of buffer */
	bulkstat_one_fmt_pf	formatter,	/* formatter, copy to user */
	int			*ubused,	/* bytes used by me */
	int			*stat)		/* BULKSTAT_RV_... */
{
	struct xfs_icdinode	*dic;		/* dinode core info pointer */
	struct xfs_inode	*ip;		/* incore inode pointer */
	struct inode		*inode;
	struct xfs_bstat	*buf;		/* return buffer */
	int			error = 0;	/* error value */

	*stat = BULKSTAT_RV_NOTHING;

	if (!buffer || xfs_internal_inum(mp, ino))
		return -EINVAL;

	buf = kmem_zalloc(sizeof(*buf), KM_SLEEP | KM_MAYFAIL);
	if (!buf)
		return -ENOMEM;

	error = xfs_iget(mp, NULL, ino,
			 (XFS_IGET_DONTCACHE | XFS_IGET_UNTRUSTED),
			 XFS_ILOCK_SHARED, &ip);
	if (error)
		goto out_free;
=======
#include "xfs_iwalk.h"
#include "xfs_itable.h"
#include "xfs_error.h"
#include "xfs_icache.h"
#include "xfs_health.h"
#include "xfs_trans.h"

/*
 * Bulk Stat
 * =========
 *
 * Use the inode walking functions to fill out struct xfs_bulkstat for every
 * allocated inode, then pass the stat information to some externally provided
 * iteration function.
 */

struct xfs_bstat_chunk {
	bulkstat_one_fmt_pf	formatter;
	struct xfs_ibulk	*breq;
	struct xfs_bulkstat	*buf;
};

/*
 * Fill out the bulkstat info for a single inode and report it somewhere.
 *
 * bc->breq->lastino is effectively the inode cursor as we walk through the
 * filesystem.  Therefore, we update it any time we need to move the cursor
 * forward, regardless of whether or not we're sending any bstat information
 * back to userspace.  If the inode is internal metadata or, has been freed
 * out from under us, we just simply keep going.
 *
 * However, if any other type of error happens we want to stop right where we
 * are so that userspace will call back with exact number of the bad inode and
 * we can send back an error code.
 *
 * Note that if the formatter tells us there's no space left in the buffer we
 * move the cursor forward and abort the walk.
 */
STATIC int
xfs_bulkstat_one_int(
	struct xfs_mount	*mp,
	struct user_namespace	*mnt_userns,
	struct xfs_trans	*tp,
	xfs_ino_t		ino,
	struct xfs_bstat_chunk	*bc)
{
	struct user_namespace	*sb_userns = mp->m_super->s_user_ns;
	struct xfs_inode	*ip;		/* incore inode pointer */
	struct inode		*inode;
	struct xfs_bulkstat	*buf = bc->buf;
	int			error = -EINVAL;

	if (xfs_internal_inum(mp, ino))
		goto out_advance;

	error = xfs_iget(mp, tp, ino,
			 (XFS_IGET_DONTCACHE | XFS_IGET_UNTRUSTED),
			 XFS_ILOCK_SHARED, &ip);
	if (error == -ENOENT || error == -EINVAL)
		goto out_advance;
	if (error)
		goto out;
>>>>>>> upstream/android-13

	ASSERT(ip != NULL);
	ASSERT(ip->i_imap.im_blkno != 0);
	inode = VFS_I(ip);

<<<<<<< HEAD
	dic = &ip->i_d;

	/* xfs_iget returns the following without needing
	 * further change.
	 */
	buf->bs_projid_lo = dic->di_projid_lo;
	buf->bs_projid_hi = dic->di_projid_hi;
	buf->bs_ino = ino;
	buf->bs_uid = dic->di_uid;
	buf->bs_gid = dic->di_gid;
	buf->bs_size = dic->di_size;

	buf->bs_nlink = inode->i_nlink;
	buf->bs_atime.tv_sec = inode->i_atime.tv_sec;
	buf->bs_atime.tv_nsec = inode->i_atime.tv_nsec;
	buf->bs_mtime.tv_sec = inode->i_mtime.tv_sec;
	buf->bs_mtime.tv_nsec = inode->i_mtime.tv_nsec;
	buf->bs_ctime.tv_sec = inode->i_ctime.tv_sec;
	buf->bs_ctime.tv_nsec = inode->i_ctime.tv_nsec;
=======
	/* xfs_iget returns the following without needing
	 * further change.
	 */
	buf->bs_projectid = ip->i_projid;
	buf->bs_ino = ino;
	buf->bs_uid = from_kuid(sb_userns, i_uid_into_mnt(mnt_userns, inode));
	buf->bs_gid = from_kgid(sb_userns, i_gid_into_mnt(mnt_userns, inode));
	buf->bs_size = ip->i_disk_size;

	buf->bs_nlink = inode->i_nlink;
	buf->bs_atime = inode->i_atime.tv_sec;
	buf->bs_atime_nsec = inode->i_atime.tv_nsec;
	buf->bs_mtime = inode->i_mtime.tv_sec;
	buf->bs_mtime_nsec = inode->i_mtime.tv_nsec;
	buf->bs_ctime = inode->i_ctime.tv_sec;
	buf->bs_ctime_nsec = inode->i_ctime.tv_nsec;
>>>>>>> upstream/android-13
	buf->bs_gen = inode->i_generation;
	buf->bs_mode = inode->i_mode;

	buf->bs_xflags = xfs_ip2xflags(ip);
<<<<<<< HEAD
	buf->bs_extsize = dic->di_extsize << mp->m_sb.sb_blocklog;
	buf->bs_extents = dic->di_nextents;
	memset(buf->bs_pad, 0, sizeof(buf->bs_pad));
	buf->bs_dmevmask = dic->di_dmevmask;
	buf->bs_dmstate = dic->di_dmstate;
	buf->bs_aextents = dic->di_anextents;
	buf->bs_forkoff = XFS_IFORK_BOFF(ip);

	if (dic->di_version == 3) {
		if (dic->di_flags2 & XFS_DIFLAG2_COWEXTSIZE)
			buf->bs_cowextsize = dic->di_cowextsize <<
					mp->m_sb.sb_blocklog;
	}

	switch (dic->di_format) {
=======
	buf->bs_extsize_blks = ip->i_extsize;
	buf->bs_extents = xfs_ifork_nextents(&ip->i_df);
	xfs_bulkstat_health(ip, buf);
	buf->bs_aextents = xfs_ifork_nextents(ip->i_afp);
	buf->bs_forkoff = XFS_IFORK_BOFF(ip);
	buf->bs_version = XFS_BULKSTAT_VERSION_V5;

	if (xfs_has_v3inodes(mp)) {
		buf->bs_btime = ip->i_crtime.tv_sec;
		buf->bs_btime_nsec = ip->i_crtime.tv_nsec;
		if (ip->i_diflags2 & XFS_DIFLAG2_COWEXTSIZE)
			buf->bs_cowextsize_blks = ip->i_cowextsize;
	}

	switch (ip->i_df.if_format) {
>>>>>>> upstream/android-13
	case XFS_DINODE_FMT_DEV:
		buf->bs_rdev = sysv_encode_dev(inode->i_rdev);
		buf->bs_blksize = BLKDEV_IOSIZE;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_LOCAL:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_EXTENTS:
	case XFS_DINODE_FMT_BTREE:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
<<<<<<< HEAD
		buf->bs_blocks = dic->di_nblocks + ip->i_delayed_blks;
=======
		buf->bs_blocks = ip->i_nblocks + ip->i_delayed_blks;
>>>>>>> upstream/android-13
		break;
	}
	xfs_iunlock(ip, XFS_ILOCK_SHARED);
	xfs_irele(ip);

<<<<<<< HEAD
	error = formatter(buffer, ubsize, ubused, buf);
	if (!error)
		*stat = BULKSTAT_RV_DIDONE;

 out_free:
	kmem_free(buf);
	return error;
}

/* Return 0 on success or positive error */
STATIC int
xfs_bulkstat_one_fmt(
	void			__user *ubuffer,
	int			ubsize,
	int			*ubused,
	const xfs_bstat_t	*buffer)
{
	if (ubsize < sizeof(*buffer))
		return -ENOMEM;
	if (copy_to_user(ubuffer, buffer, sizeof(*buffer)))
		return -EFAULT;
	if (ubused)
		*ubused = sizeof(*buffer);
	return 0;
}

int
xfs_bulkstat_one(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	ino,		/* inode number to get data for */
	void		__user *buffer,	/* buffer to place output in */
	int		ubsize,		/* size of buffer */
	int		*ubused,	/* bytes used by me */
	int		*stat)		/* BULKSTAT_RV_... */
{
	return xfs_bulkstat_one_int(mp, ino, buffer, ubsize,
				    xfs_bulkstat_one_fmt, ubused, stat);
}

/*
 * Loop over all clusters in a chunk for a given incore inode allocation btree
 * record.  Do a readahead if there are any allocated inodes in that cluster.
 */
STATIC void
xfs_bulkstat_ichunk_ra(
	struct xfs_mount		*mp,
	xfs_agnumber_t			agno,
	struct xfs_inobt_rec_incore	*irec)
{
	xfs_agblock_t			agbno;
	struct blk_plug			plug;
	int				blks_per_cluster;
	int				inodes_per_cluster;
	int				i;	/* inode chunk index */

	agbno = XFS_AGINO_TO_AGBNO(mp, irec->ir_startino);
	blks_per_cluster = xfs_icluster_size_fsb(mp);
	inodes_per_cluster = blks_per_cluster << mp->m_sb.sb_inopblog;

	blk_start_plug(&plug);
	for (i = 0; i < XFS_INODES_PER_CHUNK;
	     i += inodes_per_cluster, agbno += blks_per_cluster) {
		if (xfs_inobt_maskn(i, inodes_per_cluster) & ~irec->ir_free) {
			xfs_btree_reada_bufs(mp, agno, agbno, blks_per_cluster,
					     &xfs_inode_buf_ops);
		}
	}
	blk_finish_plug(&plug);
}

/*
 * Lookup the inode chunk that the given inode lives in and then get the record
 * if we found the chunk.  If the inode was not the last in the chunk and there
 * are some left allocated, update the data for the pointed-to record as well as
 * return the count of grabbed inodes.
 */
STATIC int
xfs_bulkstat_grab_ichunk(
	struct xfs_btree_cur		*cur,	/* btree cursor */
	xfs_agino_t			agino,	/* starting inode of chunk */
	int				*icount,/* return # of inodes grabbed */
	struct xfs_inobt_rec_incore	*irec)	/* btree record */
{
	int				idx;	/* index into inode chunk */
	int				stat;
	int				error = 0;

	/* Lookup the inode chunk that this inode lives in */
	error = xfs_inobt_lookup(cur, agino, XFS_LOOKUP_LE, &stat);
	if (error)
		return error;
	if (!stat) {
		*icount = 0;
		return error;
	}

	/* Get the record, should always work */
	error = xfs_inobt_get_rec(cur, irec, &stat);
	if (error)
		return error;
	XFS_WANT_CORRUPTED_RETURN(cur->bc_mp, stat == 1);

	/* Check if the record contains the inode in request */
	if (irec->ir_startino + XFS_INODES_PER_CHUNK <= agino) {
		*icount = 0;
		return 0;
	}

	idx = agino - irec->ir_startino + 1;
	if (idx < XFS_INODES_PER_CHUNK &&
	    (xfs_inobt_maskn(idx, XFS_INODES_PER_CHUNK - idx) & ~irec->ir_free)) {
		int	i;

		/* We got a right chunk with some left inodes allocated at it.
		 * Grab the chunk record.  Mark all the uninteresting inodes
		 * free -- because they're before our start point.
		 */
		for (i = 0; i < idx; i++) {
			if (XFS_INOBT_MASK(i) & ~irec->ir_free)
				irec->ir_freecount++;
		}

		irec->ir_free |= xfs_inobt_maskn(0, idx);
		*icount = irec->ir_count - irec->ir_freecount;
	}

	return 0;
}

#define XFS_BULKSTAT_UBLEFT(ubleft)	((ubleft) >= statstruct_size)

struct xfs_bulkstat_agichunk {
	char		__user **ac_ubuffer;/* pointer into user's buffer */
	int		ac_ubleft;	/* bytes left in user's buffer */
	int		ac_ubelem;	/* spaces used in user's buffer */
};

/*
 * Process inodes in chunk with a pointer to a formatter function
 * that will iget the inode and fill in the appropriate structure.
 */
static int
xfs_bulkstat_ag_ichunk(
	struct xfs_mount		*mp,
	xfs_agnumber_t			agno,
	struct xfs_inobt_rec_incore	*irbp,
	bulkstat_one_pf			formatter,
	size_t				statstruct_size,
	struct xfs_bulkstat_agichunk	*acp,
	xfs_agino_t			*last_agino)
{
	char				__user **ubufp = acp->ac_ubuffer;
	int				chunkidx;
	int				error = 0;
	xfs_agino_t			agino = irbp->ir_startino;

	for (chunkidx = 0; chunkidx < XFS_INODES_PER_CHUNK;
	     chunkidx++, agino++) {
		int		fmterror;
		int		ubused;

		/* inode won't fit in buffer, we are done */
		if (acp->ac_ubleft < statstruct_size)
			break;

		/* Skip if this inode is free */
		if (XFS_INOBT_MASK(chunkidx) & irbp->ir_free)
			continue;

		/* Get the inode and fill in a single buffer */
		ubused = statstruct_size;
		error = formatter(mp, XFS_AGINO_TO_INO(mp, agno, agino),
				  *ubufp, acp->ac_ubleft, &ubused, &fmterror);

		if (fmterror == BULKSTAT_RV_GIVEUP ||
		    (error && error != -ENOENT && error != -EINVAL)) {
			acp->ac_ubleft = 0;
			ASSERT(error);
			break;
		}

		/* be careful not to leak error if at end of chunk */
		if (fmterror == BULKSTAT_RV_NOTHING || error) {
			error = 0;
			continue;
		}

		*ubufp += ubused;
		acp->ac_ubleft -= ubused;
		acp->ac_ubelem++;
	}

	/*
	 * Post-update *last_agino. At this point, agino will always point one
	 * inode past the last inode we processed successfully. Hence we
	 * substract that inode when setting the *last_agino cursor so that we
	 * return the correct cookie to userspace. On the next bulkstat call,
	 * the inode under the lastino cookie will be skipped as we have already
	 * processed it here.
	 */
	*last_agino = agino - 1;
=======
	error = bc->formatter(bc->breq, buf);
	if (error == -ECANCELED)
		goto out_advance;
	if (error)
		goto out;

out_advance:
	/*
	 * Advance the cursor to the inode that comes after the one we just
	 * looked at.  We want the caller to move along if the bulkstat
	 * information was copied successfully; if we tried to grab the inode
	 * but it's no longer allocated; or if it's internal metadata.
	 */
	bc->breq->startino = ino + 1;
out:
	return error;
}

/* Bulkstat a single inode. */
int
xfs_bulkstat_one(
	struct xfs_ibulk	*breq,
	bulkstat_one_fmt_pf	formatter)
{
	struct xfs_bstat_chunk	bc = {
		.formatter	= formatter,
		.breq		= breq,
	};
	struct xfs_trans	*tp;
	int			error;

	if (breq->mnt_userns != &init_user_ns) {
		xfs_warn_ratelimited(breq->mp,
			"bulkstat not supported inside of idmapped mounts.");
		return -EINVAL;
	}

	ASSERT(breq->icount == 1);

	bc.buf = kmem_zalloc(sizeof(struct xfs_bulkstat),
			KM_MAYFAIL);
	if (!bc.buf)
		return -ENOMEM;

	/*
	 * Grab an empty transaction so that we can use its recursive buffer
	 * locking abilities to detect cycles in the inobt without deadlocking.
	 */
	error = xfs_trans_alloc_empty(breq->mp, &tp);
	if (error)
		goto out;

	error = xfs_bulkstat_one_int(breq->mp, breq->mnt_userns, tp,
			breq->startino, &bc);
	xfs_trans_cancel(tp);
out:
	kmem_free(bc.buf);

	/*
	 * If we reported one inode to userspace then we abort because we hit
	 * the end of the buffer.  Don't leak that back to userspace.
	 */
	if (error == -ECANCELED)
		error = 0;
>>>>>>> upstream/android-13

	return error;
}

<<<<<<< HEAD
/*
 * Return stat information in bulk (by-inode) for the filesystem.
 */
int					/* error status */
xfs_bulkstat(
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_ino_t		*lastinop, /* last inode returned */
	int			*ubcountp, /* size of buffer/count returned */
	bulkstat_one_pf		formatter, /* func that'd fill a single buf */
	size_t			statstruct_size, /* sizeof struct filling */
	char			__user *ubuffer, /* buffer with inode stats */
	int			*done)	/* 1 if there are more stats to get */
{
	xfs_buf_t		*agbp;	/* agi header buffer */
	xfs_agino_t		agino;	/* inode # in allocation group */
	xfs_agnumber_t		agno;	/* allocation group number */
	xfs_btree_cur_t		*cur;	/* btree cursor for ialloc btree */
	xfs_inobt_rec_incore_t	*irbuf;	/* start of irec buffer */
	int			nirbuf;	/* size of irbuf */
	int			ubcount; /* size of user's buffer */
	struct xfs_bulkstat_agichunk ac;
	int			error = 0;

	/*
	 * Get the last inode value, see if there's nothing to do.
	 */
	agno = XFS_INO_TO_AGNO(mp, *lastinop);
	agino = XFS_INO_TO_AGINO(mp, *lastinop);
	if (agno >= mp->m_sb.sb_agcount ||
	    *lastinop != XFS_AGINO_TO_INO(mp, agno, agino)) {
		*done = 1;
		*ubcountp = 0;
		return 0;
	}

	ubcount = *ubcountp; /* statstruct's */
	ac.ac_ubuffer = &ubuffer;
	ac.ac_ubleft = ubcount * statstruct_size; /* bytes */;
	ac.ac_ubelem = 0;

	*ubcountp = 0;
	*done = 0;

	irbuf = kmem_zalloc_large(PAGE_SIZE * 4, KM_SLEEP);
	if (!irbuf)
		return -ENOMEM;
	nirbuf = (PAGE_SIZE * 4) / sizeof(*irbuf);

	/*
	 * Loop over the allocation groups, starting from the last
	 * inode returned; 0 means start of the allocation group.
	 */
	while (agno < mp->m_sb.sb_agcount) {
		struct xfs_inobt_rec_incore	*irbp = irbuf;
		struct xfs_inobt_rec_incore	*irbufend = irbuf + nirbuf;
		bool				end_of_ag = false;
		int				icount = 0;
		int				stat;

		error = xfs_ialloc_read_agi(mp, NULL, agno, &agbp);
		if (error)
			break;
		/*
		 * Allocate and initialize a btree cursor for ialloc btree.
		 */
		cur = xfs_inobt_init_cursor(mp, NULL, agbp, agno,
					    XFS_BTNUM_INO);
		if (agino > 0) {
			/*
			 * In the middle of an allocation group, we need to get
			 * the remainder of the chunk we're in.
			 */
			struct xfs_inobt_rec_incore	r;

			error = xfs_bulkstat_grab_ichunk(cur, agino, &icount, &r);
			if (error)
				goto del_cursor;
			if (icount) {
				irbp->ir_startino = r.ir_startino;
				irbp->ir_holemask = r.ir_holemask;
				irbp->ir_count = r.ir_count;
				irbp->ir_freecount = r.ir_freecount;
				irbp->ir_free = r.ir_free;
				irbp++;
			}
			/* Increment to the next record */
			error = xfs_btree_increment(cur, 0, &stat);
		} else {
			/* Start of ag.  Lookup the first inode chunk */
			error = xfs_inobt_lookup(cur, 0, XFS_LOOKUP_GE, &stat);
		}
		if (error || stat == 0) {
			end_of_ag = true;
			goto del_cursor;
		}

		/*
		 * Loop through inode btree records in this ag,
		 * until we run out of inodes or space in the buffer.
		 */
		while (irbp < irbufend && icount < ubcount) {
			struct xfs_inobt_rec_incore	r;

			error = xfs_inobt_get_rec(cur, &r, &stat);
			if (error || stat == 0) {
				end_of_ag = true;
				goto del_cursor;
			}

			/*
			 * If this chunk has any allocated inodes, save it.
			 * Also start read-ahead now for this chunk.
			 */
			if (r.ir_freecount < r.ir_count) {
				xfs_bulkstat_ichunk_ra(mp, agno, &r);
				irbp->ir_startino = r.ir_startino;
				irbp->ir_holemask = r.ir_holemask;
				irbp->ir_count = r.ir_count;
				irbp->ir_freecount = r.ir_freecount;
				irbp->ir_free = r.ir_free;
				irbp++;
				icount += r.ir_count - r.ir_freecount;
			}
			error = xfs_btree_increment(cur, 0, &stat);
			if (error || stat == 0) {
				end_of_ag = true;
				goto del_cursor;
			}
			cond_resched();
		}

		/*
		 * Drop the btree buffers and the agi buffer as we can't hold any
		 * of the locks these represent when calling iget. If there is a
		 * pending error, then we are done.
		 */
del_cursor:
		xfs_btree_del_cursor(cur, error);
		xfs_buf_relse(agbp);
		if (error)
			break;
		/*
		 * Now format all the good inodes into the user's buffer. The
		 * call to xfs_bulkstat_ag_ichunk() sets up the agino pointer
		 * for the next loop iteration.
		 */
		irbufend = irbp;
		for (irbp = irbuf;
		     irbp < irbufend && ac.ac_ubleft >= statstruct_size;
		     irbp++) {
			error = xfs_bulkstat_ag_ichunk(mp, agno, irbp,
					formatter, statstruct_size, &ac,
					&agino);
			if (error)
				break;

			cond_resched();
		}

		/*
		 * If we've run out of space or had a formatting error, we
		 * are now done
		 */
		if (ac.ac_ubleft < statstruct_size || error)
			break;

		if (end_of_ag) {
			agno++;
			agino = 0;
		}
	}
	/*
	 * Done, we're either out of filesystem or space to put the data.
	 */
	kmem_free(irbuf);
	*ubcountp = ac.ac_ubelem;
=======
static int
xfs_bulkstat_iwalk(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	xfs_ino_t		ino,
	void			*data)
{
	struct xfs_bstat_chunk	*bc = data;
	int			error;

	error = xfs_bulkstat_one_int(mp, bc->breq->mnt_userns, tp, ino, data);
	/* bulkstat just skips over missing inodes */
	if (error == -ENOENT || error == -EINVAL)
		return 0;
	return error;
}

/*
 * Check the incoming lastino parameter.
 *
 * We allow any inode value that could map to physical space inside the
 * filesystem because if there are no inodes there, bulkstat moves on to the
 * next chunk.  In other words, the magic agino value of zero takes us to the
 * first chunk in the AG, and an agino value past the end of the AG takes us to
 * the first chunk in the next AG.
 *
 * Therefore we can end early if the requested inode is beyond the end of the
 * filesystem or doesn't map properly.
 */
static inline bool
xfs_bulkstat_already_done(
	struct xfs_mount	*mp,
	xfs_ino_t		startino)
{
	xfs_agnumber_t		agno = XFS_INO_TO_AGNO(mp, startino);
	xfs_agino_t		agino = XFS_INO_TO_AGINO(mp, startino);

	return agno >= mp->m_sb.sb_agcount ||
	       startino != XFS_AGINO_TO_INO(mp, agno, agino);
}

/* Return stat information in bulk (by-inode) for the filesystem. */
int
xfs_bulkstat(
	struct xfs_ibulk	*breq,
	bulkstat_one_fmt_pf	formatter)
{
	struct xfs_bstat_chunk	bc = {
		.formatter	= formatter,
		.breq		= breq,
	};
	struct xfs_trans	*tp;
	int			error;

	if (breq->mnt_userns != &init_user_ns) {
		xfs_warn_ratelimited(breq->mp,
			"bulkstat not supported inside of idmapped mounts.");
		return -EINVAL;
	}
	if (xfs_bulkstat_already_done(breq->mp, breq->startino))
		return 0;

	bc.buf = kmem_zalloc(sizeof(struct xfs_bulkstat),
			KM_MAYFAIL);
	if (!bc.buf)
		return -ENOMEM;

	/*
	 * Grab an empty transaction so that we can use its recursive buffer
	 * locking abilities to detect cycles in the inobt without deadlocking.
	 */
	error = xfs_trans_alloc_empty(breq->mp, &tp);
	if (error)
		goto out;

	error = xfs_iwalk(breq->mp, tp, breq->startino, breq->flags,
			xfs_bulkstat_iwalk, breq->icount, &bc);
	xfs_trans_cancel(tp);
out:
	kmem_free(bc.buf);
>>>>>>> upstream/android-13

	/*
	 * We found some inodes, so clear the error status and return them.
	 * The lastino pointer will point directly at the inode that triggered
	 * any error that occurred, so on the next call the error will be
	 * triggered again and propagated to userspace as there will be no
	 * formatted inodes in the buffer.
	 */
<<<<<<< HEAD
	if (ac.ac_ubelem)
		error = 0;

	/*
	 * If we ran out of filesystem, lastino will point off the end of
	 * the filesystem so the next call will return immediately.
	 */
	*lastinop = XFS_AGINO_TO_INO(mp, agno, agino);
	if (agno >= mp->m_sb.sb_agcount)
		*done = 1;

	return error;
}

int
xfs_inumbers_fmt(
	void			__user *ubuffer, /* buffer to write to */
	const struct xfs_inogrp	*buffer,	/* buffer to read from */
	long			count,		/* # of elements to read */
	long			*written)	/* # of bytes written */
{
	if (copy_to_user(ubuffer, buffer, count * sizeof(*buffer)))
		return -EFAULT;
	*written = count * sizeof(*buffer);
	return 0;
=======
	if (breq->ocount > 0)
		error = 0;

	return error;
}

/* Convert bulkstat (v5) to bstat (v1). */
void
xfs_bulkstat_to_bstat(
	struct xfs_mount		*mp,
	struct xfs_bstat		*bs1,
	const struct xfs_bulkstat	*bstat)
{
	/* memset is needed here because of padding holes in the structure. */
	memset(bs1, 0, sizeof(struct xfs_bstat));
	bs1->bs_ino = bstat->bs_ino;
	bs1->bs_mode = bstat->bs_mode;
	bs1->bs_nlink = bstat->bs_nlink;
	bs1->bs_uid = bstat->bs_uid;
	bs1->bs_gid = bstat->bs_gid;
	bs1->bs_rdev = bstat->bs_rdev;
	bs1->bs_blksize = bstat->bs_blksize;
	bs1->bs_size = bstat->bs_size;
	bs1->bs_atime.tv_sec = bstat->bs_atime;
	bs1->bs_mtime.tv_sec = bstat->bs_mtime;
	bs1->bs_ctime.tv_sec = bstat->bs_ctime;
	bs1->bs_atime.tv_nsec = bstat->bs_atime_nsec;
	bs1->bs_mtime.tv_nsec = bstat->bs_mtime_nsec;
	bs1->bs_ctime.tv_nsec = bstat->bs_ctime_nsec;
	bs1->bs_blocks = bstat->bs_blocks;
	bs1->bs_xflags = bstat->bs_xflags;
	bs1->bs_extsize = XFS_FSB_TO_B(mp, bstat->bs_extsize_blks);
	bs1->bs_extents = bstat->bs_extents;
	bs1->bs_gen = bstat->bs_gen;
	bs1->bs_projid_lo = bstat->bs_projectid & 0xFFFF;
	bs1->bs_forkoff = bstat->bs_forkoff;
	bs1->bs_projid_hi = bstat->bs_projectid >> 16;
	bs1->bs_sick = bstat->bs_sick;
	bs1->bs_checked = bstat->bs_checked;
	bs1->bs_cowextsize = XFS_FSB_TO_B(mp, bstat->bs_cowextsize_blks);
	bs1->bs_dmevmask = 0;
	bs1->bs_dmstate = 0;
	bs1->bs_aextents = bstat->bs_aextents;
}

struct xfs_inumbers_chunk {
	inumbers_fmt_pf		formatter;
	struct xfs_ibulk	*breq;
};

/*
 * INUMBERS
 * ========
 * This is how we export inode btree records to userspace, so that XFS tools
 * can figure out where inodes are allocated.
 */

/*
 * Format the inode group structure and report it somewhere.
 *
 * Similar to xfs_bulkstat_one_int, lastino is the inode cursor as we walk
 * through the filesystem so we move it forward unless there was a runtime
 * error.  If the formatter tells us the buffer is now full we also move the
 * cursor forward and abort the walk.
 */
STATIC int
xfs_inumbers_walk(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	xfs_agnumber_t		agno,
	const struct xfs_inobt_rec_incore *irec,
	void			*data)
{
	struct xfs_inumbers	inogrp = {
		.xi_startino	= XFS_AGINO_TO_INO(mp, agno, irec->ir_startino),
		.xi_alloccount	= irec->ir_count - irec->ir_freecount,
		.xi_allocmask	= ~irec->ir_free,
		.xi_version	= XFS_INUMBERS_VERSION_V5,
	};
	struct xfs_inumbers_chunk *ic = data;
	int			error;

	error = ic->formatter(ic->breq, &inogrp);
	if (error && error != -ECANCELED)
		return error;

	ic->breq->startino = XFS_AGINO_TO_INO(mp, agno, irec->ir_startino) +
			XFS_INODES_PER_CHUNK;
	return error;
>>>>>>> upstream/android-13
}

/*
 * Return inode number table for the filesystem.
 */
<<<<<<< HEAD
int					/* error status */
xfs_inumbers(
	struct xfs_mount	*mp,/* mount point for filesystem */
	xfs_ino_t		*lastino,/* last inode returned */
	int			*count,/* size of buffer/count returned */
	void			__user *ubuffer,/* buffer with inode descriptions */
	inumbers_fmt_pf		formatter)
{
	xfs_agnumber_t		agno = XFS_INO_TO_AGNO(mp, *lastino);
	xfs_agino_t		agino = XFS_INO_TO_AGINO(mp, *lastino);
	struct xfs_btree_cur	*cur = NULL;
	struct xfs_buf		*agbp = NULL;
	struct xfs_inogrp	*buffer;
	int			bcount;
	int			left = *count;
	int			bufidx = 0;
	int			error = 0;

	*count = 0;
	if (agno >= mp->m_sb.sb_agcount ||
	    *lastino != XFS_AGINO_TO_INO(mp, agno, agino))
		return error;

	bcount = min(left, (int)(PAGE_SIZE / sizeof(*buffer)));
	buffer = kmem_zalloc(bcount * sizeof(*buffer), KM_SLEEP);
	do {
		struct xfs_inobt_rec_incore	r;
		int				stat;

		if (!agbp) {
			error = xfs_ialloc_read_agi(mp, NULL, agno, &agbp);
			if (error)
				break;

			cur = xfs_inobt_init_cursor(mp, NULL, agbp, agno,
						    XFS_BTNUM_INO);
			error = xfs_inobt_lookup(cur, agino, XFS_LOOKUP_GE,
						 &stat);
			if (error)
				break;
			if (!stat)
				goto next_ag;
		}

		error = xfs_inobt_get_rec(cur, &r, &stat);
		if (error)
			break;
		if (!stat)
			goto next_ag;

		agino = r.ir_startino + XFS_INODES_PER_CHUNK - 1;
		buffer[bufidx].xi_startino =
			XFS_AGINO_TO_INO(mp, agno, r.ir_startino);
		buffer[bufidx].xi_alloccount = r.ir_count - r.ir_freecount;
		buffer[bufidx].xi_allocmask = ~r.ir_free;
		if (++bufidx == bcount) {
			long	written;

			error = formatter(ubuffer, buffer, bufidx, &written);
			if (error)
				break;
			ubuffer += written;
			*count += bufidx;
			bufidx = 0;
		}
		if (!--left)
			break;

		error = xfs_btree_increment(cur, 0, &stat);
		if (error)
			break;
		if (stat)
			continue;

next_ag:
		xfs_btree_del_cursor(cur, XFS_BTREE_ERROR);
		cur = NULL;
		xfs_buf_relse(agbp);
		agbp = NULL;
		agino = 0;
		agno++;
	} while (agno < mp->m_sb.sb_agcount);

	if (!error) {
		if (bufidx) {
			long	written;

			error = formatter(ubuffer, buffer, bufidx, &written);
			if (!error)
				*count += bufidx;
		}
		*lastino = XFS_AGINO_TO_INO(mp, agno, agino);
	}

	kmem_free(buffer);
	if (cur)
		xfs_btree_del_cursor(cur, error);
	if (agbp)
		xfs_buf_relse(agbp);

	return error;
}
=======
int
xfs_inumbers(
	struct xfs_ibulk	*breq,
	inumbers_fmt_pf		formatter)
{
	struct xfs_inumbers_chunk ic = {
		.formatter	= formatter,
		.breq		= breq,
	};
	struct xfs_trans	*tp;
	int			error = 0;

	if (xfs_bulkstat_already_done(breq->mp, breq->startino))
		return 0;

	/*
	 * Grab an empty transaction so that we can use its recursive buffer
	 * locking abilities to detect cycles in the inobt without deadlocking.
	 */
	error = xfs_trans_alloc_empty(breq->mp, &tp);
	if (error)
		goto out;

	error = xfs_inobt_walk(breq->mp, tp, breq->startino, breq->flags,
			xfs_inumbers_walk, breq->icount, &ic);
	xfs_trans_cancel(tp);
out:

	/*
	 * We found some inode groups, so clear the error status and return
	 * them.  The lastino pointer will point directly at the inode that
	 * triggered any error that occurred, so on the next call the error
	 * will be triggered again and propagated to userspace as there will be
	 * no formatted inode groups in the buffer.
	 */
	if (breq->ocount > 0)
		error = 0;

	return error;
}

/* Convert an inumbers (v5) struct to a inogrp (v1) struct. */
void
xfs_inumbers_to_inogrp(
	struct xfs_inogrp		*ig1,
	const struct xfs_inumbers	*ig)
{
	/* memset is needed here because of padding holes in the structure. */
	memset(ig1, 0, sizeof(struct xfs_inogrp));
	ig1->xi_startino = ig->xi_startino;
	ig1->xi_alloccount = ig->xi_alloccount;
	ig1->xi_allocmask = ig->xi_allocmask;
}
>>>>>>> upstream/android-13
