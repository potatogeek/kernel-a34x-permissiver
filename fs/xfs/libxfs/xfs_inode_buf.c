// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * All Rights Reserved.
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
#include "xfs_inode.h"
#include "xfs_errortag.h"
#include "xfs_error.h"
#include "xfs_cksum.h"
=======
#include "xfs_inode.h"
#include "xfs_errortag.h"
#include "xfs_error.h"
>>>>>>> upstream/android-13
#include "xfs_icache.h"
#include "xfs_trans.h"
#include "xfs_ialloc.h"
#include "xfs_dir2.h"

#include <linux/iversion.h>

/*
<<<<<<< HEAD
 * Check that none of the inode's in the buffer have a next
 * unlinked field of 0.
 */
#if defined(DEBUG)
void
xfs_inobp_check(
	xfs_mount_t	*mp,
	xfs_buf_t	*bp)
{
	int		i;
	int		j;
	xfs_dinode_t	*dip;

	j = mp->m_inode_cluster_size >> mp->m_sb.sb_inodelog;

	for (i = 0; i < j; i++) {
		dip = xfs_buf_offset(bp, i * mp->m_sb.sb_inodesize);
		if (!dip->di_next_unlinked)  {
			xfs_alert(mp,
	"Detected bogus zero next_unlinked field in inode %d buffer 0x%llx.",
				i, (long long)bp->b_bn);
		}
	}
}
#endif

bool
xfs_dinode_good_version(
	struct xfs_mount *mp,
	__u8		version)
{
	if (xfs_sb_version_hascrc(&mp->m_sb))
		return version == 3;

	return version == 1 || version == 2;
}

/*
=======
>>>>>>> upstream/android-13
 * If we are doing readahead on an inode buffer, we might be in log recovery
 * reading an inode allocation buffer that hasn't yet been replayed, and hence
 * has not had the inode cores stamped into it. Hence for readahead, the buffer
 * may be potentially invalid.
 *
 * If the readahead buffer is invalid, we need to mark it with an error and
 * clear the DONE status of the buffer so that a followup read will re-read it
 * from disk. We don't report the error otherwise to avoid warnings during log
<<<<<<< HEAD
 * recovery and we don't get unnecssary panics on debug kernels. We use EIO here
 * because all we want to do is say readahead failed; there is no-one to report
 * the error to, so this will distinguish it from a non-ra verifier failure.
 * Changes to this readahead error behavour also need to be reflected in
=======
 * recovery and we don't get unnecessary panics on debug kernels. We use EIO here
 * because all we want to do is say readahead failed; there is no-one to report
 * the error to, so this will distinguish it from a non-ra verifier failure.
 * Changes to this readahead error behaviour also need to be reflected in
>>>>>>> upstream/android-13
 * xfs_dquot_buf_readahead_verify().
 */
static void
xfs_inode_buf_verify(
	struct xfs_buf	*bp,
	bool		readahead)
{
<<<<<<< HEAD
	struct xfs_mount *mp = bp->b_target->bt_mount;
=======
	struct xfs_mount *mp = bp->b_mount;
>>>>>>> upstream/android-13
	xfs_agnumber_t	agno;
	int		i;
	int		ni;

	/*
	 * Validate the magic number and version of every inode in the buffer
	 */
<<<<<<< HEAD
	agno = xfs_daddr_to_agno(mp, XFS_BUF_ADDR(bp));
=======
	agno = xfs_daddr_to_agno(mp, xfs_buf_daddr(bp));
>>>>>>> upstream/android-13
	ni = XFS_BB_TO_FSB(mp, bp->b_length) * mp->m_sb.sb_inopblock;
	for (i = 0; i < ni; i++) {
		int		di_ok;
		xfs_dinode_t	*dip;
		xfs_agino_t	unlinked_ino;

		dip = xfs_buf_offset(bp, (i << mp->m_sb.sb_inodelog));
		unlinked_ino = be32_to_cpu(dip->di_next_unlinked);
<<<<<<< HEAD
		di_ok = dip->di_magic == cpu_to_be16(XFS_DINODE_MAGIC) &&
			xfs_dinode_good_version(mp, dip->di_version) &&
			(unlinked_ino == NULLAGINO ||
			 xfs_verify_agino(mp, agno, unlinked_ino));
=======
		di_ok = xfs_verify_magic16(bp, dip->di_magic) &&
			xfs_dinode_good_version(mp, dip->di_version) &&
			xfs_verify_agino_or_null(mp, agno, unlinked_ino);
>>>>>>> upstream/android-13
		if (unlikely(XFS_TEST_ERROR(!di_ok, mp,
						XFS_ERRTAG_ITOBP_INOTOBP))) {
			if (readahead) {
				bp->b_flags &= ~XBF_DONE;
				xfs_buf_ioerror(bp, -EIO);
				return;
			}

#ifdef DEBUG
			xfs_alert(mp,
				"bad inode magic/vsn daddr %lld #%d (magic=%x)",
<<<<<<< HEAD
				(unsigned long long)bp->b_bn, i,
=======
				(unsigned long long)xfs_buf_daddr(bp), i,
>>>>>>> upstream/android-13
				be16_to_cpu(dip->di_magic));
#endif
			xfs_buf_verifier_error(bp, -EFSCORRUPTED,
					__func__, dip, sizeof(*dip),
					NULL);
			return;
		}
	}
}


static void
xfs_inode_buf_read_verify(
	struct xfs_buf	*bp)
{
	xfs_inode_buf_verify(bp, false);
}

static void
xfs_inode_buf_readahead_verify(
	struct xfs_buf	*bp)
{
	xfs_inode_buf_verify(bp, true);
}

static void
xfs_inode_buf_write_verify(
	struct xfs_buf	*bp)
{
	xfs_inode_buf_verify(bp, false);
}

const struct xfs_buf_ops xfs_inode_buf_ops = {
	.name = "xfs_inode",
<<<<<<< HEAD
=======
	.magic16 = { cpu_to_be16(XFS_DINODE_MAGIC),
		     cpu_to_be16(XFS_DINODE_MAGIC) },
>>>>>>> upstream/android-13
	.verify_read = xfs_inode_buf_read_verify,
	.verify_write = xfs_inode_buf_write_verify,
};

const struct xfs_buf_ops xfs_inode_buf_ra_ops = {
<<<<<<< HEAD
	.name = "xxfs_inode_ra",
=======
	.name = "xfs_inode_ra",
	.magic16 = { cpu_to_be16(XFS_DINODE_MAGIC),
		     cpu_to_be16(XFS_DINODE_MAGIC) },
>>>>>>> upstream/android-13
	.verify_read = xfs_inode_buf_readahead_verify,
	.verify_write = xfs_inode_buf_write_verify,
};


/*
 * This routine is called to map an inode to the buffer containing the on-disk
 * version of the inode.  It returns a pointer to the buffer containing the
<<<<<<< HEAD
 * on-disk inode in the bpp parameter, and in the dipp parameter it returns a
 * pointer to the on-disk inode within that buffer.
 *
 * If a non-zero error is returned, then the contents of bpp and dipp are
 * undefined.
=======
 * on-disk inode in the bpp parameter.
>>>>>>> upstream/android-13
 */
int
xfs_imap_to_bp(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_imap		*imap,
<<<<<<< HEAD
	struct xfs_dinode       **dipp,
	struct xfs_buf		**bpp,
	uint			buf_flags,
	uint			iget_flags)
{
	struct xfs_buf		*bp;
	int			error;

	buf_flags |= XBF_UNMAPPED;
	error = xfs_trans_read_buf(mp, tp, mp->m_ddev_targp, imap->im_blkno,
				   (int)imap->im_len, buf_flags, &bp,
				   &xfs_inode_buf_ops);
	if (error) {
		if (error == -EAGAIN) {
			ASSERT(buf_flags & XBF_TRYLOCK);
			return error;
		}
		xfs_warn(mp, "%s: xfs_trans_read_buf() returned error %d.",
			__func__, error);
		return error;
	}

	*bpp = bp;
	*dipp = xfs_buf_offset(bp, imap->im_boffset);
	return 0;
}

void
=======
	struct xfs_buf		**bpp)
{
	return xfs_trans_read_buf(mp, tp, mp->m_ddev_targp, imap->im_blkno,
				   imap->im_len, XBF_UNMAPPED, bpp,
				   &xfs_inode_buf_ops);
}

static inline struct timespec64 xfs_inode_decode_bigtime(uint64_t ts)
{
	struct timespec64	tv;
	uint32_t		n;

	tv.tv_sec = xfs_bigtime_to_unix(div_u64_rem(ts, NSEC_PER_SEC, &n));
	tv.tv_nsec = n;

	return tv;
}

/* Convert an ondisk timestamp to an incore timestamp. */
struct timespec64
xfs_inode_from_disk_ts(
	struct xfs_dinode		*dip,
	const xfs_timestamp_t		ts)
{
	struct timespec64		tv;
	struct xfs_legacy_timestamp	*lts;

	if (xfs_dinode_has_bigtime(dip))
		return xfs_inode_decode_bigtime(be64_to_cpu(ts));

	lts = (struct xfs_legacy_timestamp *)&ts;
	tv.tv_sec = (int)be32_to_cpu(lts->t_sec);
	tv.tv_nsec = (int)be32_to_cpu(lts->t_nsec);

	return tv;
}

int
>>>>>>> upstream/android-13
xfs_inode_from_disk(
	struct xfs_inode	*ip,
	struct xfs_dinode	*from)
{
<<<<<<< HEAD
	struct xfs_icdinode	*to = &ip->i_d;
	struct inode		*inode = VFS_I(ip);

=======
	struct inode		*inode = VFS_I(ip);
	int			error;
	xfs_failaddr_t		fa;

	ASSERT(ip->i_cowfp == NULL);
	ASSERT(ip->i_afp == NULL);

	fa = xfs_dinode_verify(ip->i_mount, ip->i_ino, from);
	if (fa) {
		xfs_inode_verifier_error(ip, -EFSCORRUPTED, "dinode", from,
				sizeof(*from), fa);
		return -EFSCORRUPTED;
	}

	/*
	 * First get the permanent information that is needed to allocate an
	 * inode. If the inode is unused, mode is zero and we shouldn't mess
	 * with the uninitialized part of it.
	 */
	if (!xfs_has_v3inodes(ip->i_mount))
		ip->i_flushiter = be16_to_cpu(from->di_flushiter);
	inode->i_generation = be32_to_cpu(from->di_gen);
	inode->i_mode = be16_to_cpu(from->di_mode);
	if (!inode->i_mode)
		return 0;
>>>>>>> upstream/android-13

	/*
	 * Convert v1 inodes immediately to v2 inode format as this is the
	 * minimum inode version format we support in the rest of the code.
<<<<<<< HEAD
	 */
	to->di_version = from->di_version;
	if (to->di_version == 1) {
		set_nlink(inode, be16_to_cpu(from->di_onlink));
		to->di_projid_lo = 0;
		to->di_projid_hi = 0;
		to->di_version = 2;
	} else {
		set_nlink(inode, be32_to_cpu(from->di_nlink));
		to->di_projid_lo = be16_to_cpu(from->di_projid_lo);
		to->di_projid_hi = be16_to_cpu(from->di_projid_hi);
	}

	to->di_format = from->di_format;
	to->di_uid = be32_to_cpu(from->di_uid);
	to->di_gid = be32_to_cpu(from->di_gid);
	to->di_flushiter = be16_to_cpu(from->di_flushiter);
=======
	 * They will also be unconditionally written back to disk as v2 inodes.
	 */
	if (unlikely(from->di_version == 1)) {
		set_nlink(inode, be16_to_cpu(from->di_onlink));
		ip->i_projid = 0;
	} else {
		set_nlink(inode, be32_to_cpu(from->di_nlink));
		ip->i_projid = (prid_t)be16_to_cpu(from->di_projid_hi) << 16 |
					be16_to_cpu(from->di_projid_lo);
	}

	i_uid_write(inode, be32_to_cpu(from->di_uid));
	i_gid_write(inode, be32_to_cpu(from->di_gid));
>>>>>>> upstream/android-13

	/*
	 * Time is signed, so need to convert to signed 32 bit before
	 * storing in inode timestamp which may be 64 bit. Otherwise
	 * a time before epoch is converted to a time long after epoch
	 * on 64 bit systems.
	 */
<<<<<<< HEAD
	inode->i_atime.tv_sec = (int)be32_to_cpu(from->di_atime.t_sec);
	inode->i_atime.tv_nsec = (int)be32_to_cpu(from->di_atime.t_nsec);
	inode->i_mtime.tv_sec = (int)be32_to_cpu(from->di_mtime.t_sec);
	inode->i_mtime.tv_nsec = (int)be32_to_cpu(from->di_mtime.t_nsec);
	inode->i_ctime.tv_sec = (int)be32_to_cpu(from->di_ctime.t_sec);
	inode->i_ctime.tv_nsec = (int)be32_to_cpu(from->di_ctime.t_nsec);
	inode->i_generation = be32_to_cpu(from->di_gen);
	inode->i_mode = be16_to_cpu(from->di_mode);

	to->di_size = be64_to_cpu(from->di_size);
	to->di_nblocks = be64_to_cpu(from->di_nblocks);
	to->di_extsize = be32_to_cpu(from->di_extsize);
	to->di_nextents = be32_to_cpu(from->di_nextents);
	to->di_anextents = be16_to_cpu(from->di_anextents);
	to->di_forkoff = from->di_forkoff;
	to->di_aformat	= from->di_aformat;
	to->di_dmevmask	= be32_to_cpu(from->di_dmevmask);
	to->di_dmstate	= be16_to_cpu(from->di_dmstate);
	to->di_flags	= be16_to_cpu(from->di_flags);

	if (to->di_version == 3) {
		inode_set_iversion_queried(inode,
					   be64_to_cpu(from->di_changecount));
		to->di_crtime.t_sec = be32_to_cpu(from->di_crtime.t_sec);
		to->di_crtime.t_nsec = be32_to_cpu(from->di_crtime.t_nsec);
		to->di_flags2 = be64_to_cpu(from->di_flags2);
		to->di_cowextsize = be32_to_cpu(from->di_cowextsize);
	}
=======
	inode->i_atime = xfs_inode_from_disk_ts(from, from->di_atime);
	inode->i_mtime = xfs_inode_from_disk_ts(from, from->di_mtime);
	inode->i_ctime = xfs_inode_from_disk_ts(from, from->di_ctime);

	ip->i_disk_size = be64_to_cpu(from->di_size);
	ip->i_nblocks = be64_to_cpu(from->di_nblocks);
	ip->i_extsize = be32_to_cpu(from->di_extsize);
	ip->i_forkoff = from->di_forkoff;
	ip->i_diflags	= be16_to_cpu(from->di_flags);

	if (from->di_dmevmask || from->di_dmstate)
		xfs_iflags_set(ip, XFS_IPRESERVE_DM_FIELDS);

	if (xfs_has_v3inodes(ip->i_mount)) {
		inode_set_iversion_queried(inode,
					   be64_to_cpu(from->di_changecount));
		ip->i_crtime = xfs_inode_from_disk_ts(from, from->di_crtime);
		ip->i_diflags2 = be64_to_cpu(from->di_flags2);
		ip->i_cowextsize = be32_to_cpu(from->di_cowextsize);
	}

	error = xfs_iformat_data_fork(ip, from);
	if (error)
		return error;
	if (from->di_forkoff) {
		error = xfs_iformat_attr_fork(ip, from);
		if (error)
			goto out_destroy_data_fork;
	}
	if (xfs_is_reflink_inode(ip))
		xfs_ifork_init_cow(ip);
	return 0;

out_destroy_data_fork:
	xfs_idestroy_fork(&ip->i_df);
	return error;
}

/* Convert an incore timestamp to an ondisk timestamp. */
static inline xfs_timestamp_t
xfs_inode_to_disk_ts(
	struct xfs_inode		*ip,
	const struct timespec64		tv)
{
	struct xfs_legacy_timestamp	*lts;
	xfs_timestamp_t			ts;

	if (xfs_inode_has_bigtime(ip))
		return cpu_to_be64(xfs_inode_encode_bigtime(tv));

	lts = (struct xfs_legacy_timestamp *)&ts;
	lts->t_sec = cpu_to_be32(tv.tv_sec);
	lts->t_nsec = cpu_to_be32(tv.tv_nsec);

	return ts;
>>>>>>> upstream/android-13
}

void
xfs_inode_to_disk(
	struct xfs_inode	*ip,
	struct xfs_dinode	*to,
	xfs_lsn_t		lsn)
{
<<<<<<< HEAD
	struct xfs_icdinode	*from = &ip->i_d;
=======
>>>>>>> upstream/android-13
	struct inode		*inode = VFS_I(ip);

	to->di_magic = cpu_to_be16(XFS_DINODE_MAGIC);
	to->di_onlink = 0;

<<<<<<< HEAD
	to->di_version = from->di_version;
	to->di_format = from->di_format;
	to->di_uid = cpu_to_be32(from->di_uid);
	to->di_gid = cpu_to_be32(from->di_gid);
	to->di_projid_lo = cpu_to_be16(from->di_projid_lo);
	to->di_projid_hi = cpu_to_be16(from->di_projid_hi);

	memset(to->di_pad, 0, sizeof(to->di_pad));
	to->di_atime.t_sec = cpu_to_be32(inode->i_atime.tv_sec);
	to->di_atime.t_nsec = cpu_to_be32(inode->i_atime.tv_nsec);
	to->di_mtime.t_sec = cpu_to_be32(inode->i_mtime.tv_sec);
	to->di_mtime.t_nsec = cpu_to_be32(inode->i_mtime.tv_nsec);
	to->di_ctime.t_sec = cpu_to_be32(inode->i_ctime.tv_sec);
	to->di_ctime.t_nsec = cpu_to_be32(inode->i_ctime.tv_nsec);
=======
	to->di_format = xfs_ifork_format(&ip->i_df);
	to->di_uid = cpu_to_be32(i_uid_read(inode));
	to->di_gid = cpu_to_be32(i_gid_read(inode));
	to->di_projid_lo = cpu_to_be16(ip->i_projid & 0xffff);
	to->di_projid_hi = cpu_to_be16(ip->i_projid >> 16);

	memset(to->di_pad, 0, sizeof(to->di_pad));
	to->di_atime = xfs_inode_to_disk_ts(ip, inode->i_atime);
	to->di_mtime = xfs_inode_to_disk_ts(ip, inode->i_mtime);
	to->di_ctime = xfs_inode_to_disk_ts(ip, inode->i_ctime);
>>>>>>> upstream/android-13
	to->di_nlink = cpu_to_be32(inode->i_nlink);
	to->di_gen = cpu_to_be32(inode->i_generation);
	to->di_mode = cpu_to_be16(inode->i_mode);

<<<<<<< HEAD
	to->di_size = cpu_to_be64(from->di_size);
	to->di_nblocks = cpu_to_be64(from->di_nblocks);
	to->di_extsize = cpu_to_be32(from->di_extsize);
	to->di_nextents = cpu_to_be32(from->di_nextents);
	to->di_anextents = cpu_to_be16(from->di_anextents);
	to->di_forkoff = from->di_forkoff;
	to->di_aformat = from->di_aformat;
	to->di_dmevmask = cpu_to_be32(from->di_dmevmask);
	to->di_dmstate = cpu_to_be16(from->di_dmstate);
	to->di_flags = cpu_to_be16(from->di_flags);

	if (from->di_version == 3) {
		to->di_changecount = cpu_to_be64(inode_peek_iversion(inode));
		to->di_crtime.t_sec = cpu_to_be32(from->di_crtime.t_sec);
		to->di_crtime.t_nsec = cpu_to_be32(from->di_crtime.t_nsec);
		to->di_flags2 = cpu_to_be64(from->di_flags2);
		to->di_cowextsize = cpu_to_be32(from->di_cowextsize);
=======
	to->di_size = cpu_to_be64(ip->i_disk_size);
	to->di_nblocks = cpu_to_be64(ip->i_nblocks);
	to->di_extsize = cpu_to_be32(ip->i_extsize);
	to->di_nextents = cpu_to_be32(xfs_ifork_nextents(&ip->i_df));
	to->di_anextents = cpu_to_be16(xfs_ifork_nextents(ip->i_afp));
	to->di_forkoff = ip->i_forkoff;
	to->di_aformat = xfs_ifork_format(ip->i_afp);
	to->di_flags = cpu_to_be16(ip->i_diflags);

	if (xfs_has_v3inodes(ip->i_mount)) {
		to->di_version = 3;
		to->di_changecount = cpu_to_be64(inode_peek_iversion(inode));
		to->di_crtime = xfs_inode_to_disk_ts(ip, ip->i_crtime);
		to->di_flags2 = cpu_to_be64(ip->i_diflags2);
		to->di_cowextsize = cpu_to_be32(ip->i_cowextsize);
>>>>>>> upstream/android-13
		to->di_ino = cpu_to_be64(ip->i_ino);
		to->di_lsn = cpu_to_be64(lsn);
		memset(to->di_pad2, 0, sizeof(to->di_pad2));
		uuid_copy(&to->di_uuid, &ip->i_mount->m_sb.sb_meta_uuid);
		to->di_flushiter = 0;
	} else {
<<<<<<< HEAD
		to->di_flushiter = cpu_to_be16(from->di_flushiter);
	}
}

void
xfs_log_dinode_to_disk(
	struct xfs_log_dinode	*from,
	struct xfs_dinode	*to)
{
	to->di_magic = cpu_to_be16(from->di_magic);
	to->di_mode = cpu_to_be16(from->di_mode);
	to->di_version = from->di_version;
	to->di_format = from->di_format;
	to->di_onlink = 0;
	to->di_uid = cpu_to_be32(from->di_uid);
	to->di_gid = cpu_to_be32(from->di_gid);
	to->di_nlink = cpu_to_be32(from->di_nlink);
	to->di_projid_lo = cpu_to_be16(from->di_projid_lo);
	to->di_projid_hi = cpu_to_be16(from->di_projid_hi);
	memcpy(to->di_pad, from->di_pad, sizeof(to->di_pad));

	to->di_atime.t_sec = cpu_to_be32(from->di_atime.t_sec);
	to->di_atime.t_nsec = cpu_to_be32(from->di_atime.t_nsec);
	to->di_mtime.t_sec = cpu_to_be32(from->di_mtime.t_sec);
	to->di_mtime.t_nsec = cpu_to_be32(from->di_mtime.t_nsec);
	to->di_ctime.t_sec = cpu_to_be32(from->di_ctime.t_sec);
	to->di_ctime.t_nsec = cpu_to_be32(from->di_ctime.t_nsec);

	to->di_size = cpu_to_be64(from->di_size);
	to->di_nblocks = cpu_to_be64(from->di_nblocks);
	to->di_extsize = cpu_to_be32(from->di_extsize);
	to->di_nextents = cpu_to_be32(from->di_nextents);
	to->di_anextents = cpu_to_be16(from->di_anextents);
	to->di_forkoff = from->di_forkoff;
	to->di_aformat = from->di_aformat;
	to->di_dmevmask = cpu_to_be32(from->di_dmevmask);
	to->di_dmstate = cpu_to_be16(from->di_dmstate);
	to->di_flags = cpu_to_be16(from->di_flags);
	to->di_gen = cpu_to_be32(from->di_gen);

	if (from->di_version == 3) {
		to->di_changecount = cpu_to_be64(from->di_changecount);
		to->di_crtime.t_sec = cpu_to_be32(from->di_crtime.t_sec);
		to->di_crtime.t_nsec = cpu_to_be32(from->di_crtime.t_nsec);
		to->di_flags2 = cpu_to_be64(from->di_flags2);
		to->di_cowextsize = cpu_to_be32(from->di_cowextsize);
		to->di_ino = cpu_to_be64(from->di_ino);
		to->di_lsn = cpu_to_be64(from->di_lsn);
		memcpy(to->di_pad2, from->di_pad2, sizeof(to->di_pad2));
		uuid_copy(&to->di_uuid, &from->di_uuid);
		to->di_flushiter = 0;
	} else {
		to->di_flushiter = cpu_to_be16(from->di_flushiter);
=======
		to->di_version = 2;
		to->di_flushiter = cpu_to_be16(ip->i_flushiter);
>>>>>>> upstream/android-13
	}
}

static xfs_failaddr_t
xfs_dinode_verify_fork(
	struct xfs_dinode	*dip,
	struct xfs_mount	*mp,
	int			whichfork)
{
	uint32_t		di_nextents = XFS_DFORK_NEXTENTS(dip, whichfork);

	switch (XFS_DFORK_FORMAT(dip, whichfork)) {
	case XFS_DINODE_FMT_LOCAL:
		/*
		 * no local regular files yet
		 */
		if (whichfork == XFS_DATA_FORK) {
			if (S_ISREG(be16_to_cpu(dip->di_mode)))
				return __this_address;
			if (be64_to_cpu(dip->di_size) >
					XFS_DFORK_SIZE(dip, mp, whichfork))
				return __this_address;
		}
		if (di_nextents)
			return __this_address;
		break;
	case XFS_DINODE_FMT_EXTENTS:
		if (di_nextents > XFS_DFORK_MAXEXT(dip, mp, whichfork))
			return __this_address;
		break;
	case XFS_DINODE_FMT_BTREE:
		if (whichfork == XFS_ATTR_FORK) {
			if (di_nextents > MAXAEXTNUM)
				return __this_address;
		} else if (di_nextents > MAXEXTNUM) {
			return __this_address;
		}
		break;
	default:
		return __this_address;
	}
	return NULL;
}

static xfs_failaddr_t
xfs_dinode_verify_forkoff(
	struct xfs_dinode	*dip,
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	if (!XFS_DFORK_Q(dip))
=======
	if (!dip->di_forkoff)
>>>>>>> upstream/android-13
		return NULL;

	switch (dip->di_format)  {
	case XFS_DINODE_FMT_DEV:
		if (dip->di_forkoff != (roundup(sizeof(xfs_dev_t), 8) >> 3))
			return __this_address;
		break;
	case XFS_DINODE_FMT_LOCAL:	/* fall through ... */
	case XFS_DINODE_FMT_EXTENTS:    /* fall through ... */
	case XFS_DINODE_FMT_BTREE:
<<<<<<< HEAD
		if (dip->di_forkoff >= (XFS_LITINO(mp, dip->di_version) >> 3))
=======
		if (dip->di_forkoff >= (XFS_LITINO(mp) >> 3))
>>>>>>> upstream/android-13
			return __this_address;
		break;
	default:
		return __this_address;
	}
	return NULL;
}

xfs_failaddr_t
xfs_dinode_verify(
	struct xfs_mount	*mp,
	xfs_ino_t		ino,
	struct xfs_dinode	*dip)
{
	xfs_failaddr_t		fa;
	uint16_t		mode;
	uint16_t		flags;
	uint64_t		flags2;
	uint64_t		di_size;

	if (dip->di_magic != cpu_to_be16(XFS_DINODE_MAGIC))
		return __this_address;

	/* Verify v3 integrity information first */
	if (dip->di_version >= 3) {
<<<<<<< HEAD
		if (!xfs_sb_version_hascrc(&mp->m_sb))
=======
		if (!xfs_has_v3inodes(mp))
>>>>>>> upstream/android-13
			return __this_address;
		if (!xfs_verify_cksum((char *)dip, mp->m_sb.sb_inodesize,
				      XFS_DINODE_CRC_OFF))
			return __this_address;
		if (be64_to_cpu(dip->di_ino) != ino)
			return __this_address;
		if (!uuid_equal(&dip->di_uuid, &mp->m_sb.sb_meta_uuid))
			return __this_address;
	}

	/* don't allow invalid i_size */
	di_size = be64_to_cpu(dip->di_size);
	if (di_size & (1ULL << 63))
		return __this_address;

	mode = be16_to_cpu(dip->di_mode);
	if (mode && xfs_mode_to_ftype(mode) == XFS_DIR3_FT_UNKNOWN)
		return __this_address;

	/* No zero-length symlinks/dirs. */
	if ((S_ISLNK(mode) || S_ISDIR(mode)) && di_size == 0)
		return __this_address;

	/* Fork checks carried over from xfs_iformat_fork */
	if (mode &&
	    be32_to_cpu(dip->di_nextents) + be16_to_cpu(dip->di_anextents) >
			be64_to_cpu(dip->di_nblocks))
		return __this_address;

	if (mode && XFS_DFORK_BOFF(dip) > mp->m_sb.sb_inodesize)
		return __this_address;

	flags = be16_to_cpu(dip->di_flags);

	if (mode && (flags & XFS_DIFLAG_REALTIME) && !mp->m_rtdev_targp)
		return __this_address;

	/* check for illegal values of forkoff */
	fa = xfs_dinode_verify_forkoff(dip, mp);
	if (fa)
		return fa;

	/* Do we have appropriate data fork formats for the mode? */
	switch (mode & S_IFMT) {
	case S_IFIFO:
	case S_IFCHR:
	case S_IFBLK:
	case S_IFSOCK:
		if (dip->di_format != XFS_DINODE_FMT_DEV)
			return __this_address;
		break;
	case S_IFREG:
	case S_IFLNK:
	case S_IFDIR:
		fa = xfs_dinode_verify_fork(dip, mp, XFS_DATA_FORK);
		if (fa)
			return fa;
		break;
	case 0:
		/* Uninitialized inode ok. */
		break;
	default:
		return __this_address;
	}

<<<<<<< HEAD
	if (XFS_DFORK_Q(dip)) {
=======
	if (dip->di_forkoff) {
>>>>>>> upstream/android-13
		fa = xfs_dinode_verify_fork(dip, mp, XFS_ATTR_FORK);
		if (fa)
			return fa;
	} else {
		/*
		 * If there is no fork offset, this may be a freshly-made inode
		 * in a new disk cluster, in which case di_aformat is zeroed.
		 * Otherwise, such an inode must be in EXTENTS format; this goes
		 * for freed inodes as well.
		 */
		switch (dip->di_aformat) {
		case 0:
		case XFS_DINODE_FMT_EXTENTS:
			break;
		default:
			return __this_address;
		}
		if (dip->di_anextents)
			return __this_address;
	}

	/* extent size hint validation */
	fa = xfs_inode_validate_extsize(mp, be32_to_cpu(dip->di_extsize),
			mode, flags);
	if (fa)
		return fa;

	/* only version 3 or greater inodes are extensively verified here */
	if (dip->di_version < 3)
		return NULL;

	flags2 = be64_to_cpu(dip->di_flags2);

	/* don't allow reflink/cowextsize if we don't have reflink */
	if ((flags2 & (XFS_DIFLAG2_REFLINK | XFS_DIFLAG2_COWEXTSIZE)) &&
<<<<<<< HEAD
	     !xfs_sb_version_hasreflink(&mp->m_sb))
=======
	     !xfs_has_reflink(mp))
>>>>>>> upstream/android-13
		return __this_address;

	/* only regular files get reflink */
	if ((flags2 & XFS_DIFLAG2_REFLINK) && (mode & S_IFMT) != S_IFREG)
		return __this_address;

	/* don't let reflink and realtime mix */
	if ((flags2 & XFS_DIFLAG2_REFLINK) && (flags & XFS_DIFLAG_REALTIME))
		return __this_address;

<<<<<<< HEAD
	/* don't let reflink and dax mix */
	if ((flags2 & XFS_DIFLAG2_REFLINK) && (flags2 & XFS_DIFLAG2_DAX))
		return __this_address;

=======
>>>>>>> upstream/android-13
	/* COW extent size hint validation */
	fa = xfs_inode_validate_cowextsize(mp, be32_to_cpu(dip->di_cowextsize),
			mode, flags, flags2);
	if (fa)
		return fa;

<<<<<<< HEAD
=======
	/* bigtime iflag can only happen on bigtime filesystems */
	if (xfs_dinode_has_bigtime(dip) &&
	    !xfs_has_bigtime(mp))
		return __this_address;

>>>>>>> upstream/android-13
	return NULL;
}

void
xfs_dinode_calc_crc(
	struct xfs_mount	*mp,
	struct xfs_dinode	*dip)
{
	uint32_t		crc;

	if (dip->di_version < 3)
		return;

<<<<<<< HEAD
	ASSERT(xfs_sb_version_hascrc(&mp->m_sb));
=======
	ASSERT(xfs_has_crc(mp));
>>>>>>> upstream/android-13
	crc = xfs_start_cksum_update((char *)dip, mp->m_sb.sb_inodesize,
			      XFS_DINODE_CRC_OFF);
	dip->di_crc = xfs_end_cksum(crc);
}

/*
<<<<<<< HEAD
 * Read the disk inode attributes into the in-core inode structure.
 *
 * For version 5 superblocks, if we are initialising a new inode and we are not
 * utilising the XFS_MOUNT_IKEEP inode cluster mode, we can simple build the new
 * inode core with a random generation number. If we are keeping inodes around,
 * we need to read the inode cluster to get the existing generation number off
 * disk. Further, if we are using version 4 superblocks (i.e. v1/v2 inode
 * format) then log recovery is dependent on the di_flushiter field being
 * initialised from the current on-disk value and hence we must also read the
 * inode off disk.
 */
int
xfs_iread(
	xfs_mount_t	*mp,
	xfs_trans_t	*tp,
	xfs_inode_t	*ip,
	uint		iget_flags)
{
	xfs_buf_t	*bp;
	xfs_dinode_t	*dip;
	xfs_failaddr_t	fa;
	int		error;

	/*
	 * Fill in the location information in the in-core inode.
	 */
	error = xfs_imap(mp, tp, ip->i_ino, &ip->i_imap, iget_flags);
	if (error)
		return error;

	/* shortcut IO on inode allocation if possible */
	if ((iget_flags & XFS_IGET_CREATE) &&
	    xfs_sb_version_hascrc(&mp->m_sb) &&
	    !(mp->m_flags & XFS_MOUNT_IKEEP)) {
		/* initialise the on-disk inode core */
		memset(&ip->i_d, 0, sizeof(ip->i_d));
		VFS_I(ip)->i_generation = prandom_u32();
		ip->i_d.di_version = 3;
		return 0;
	}

	/*
	 * Get pointers to the on-disk inode and the buffer containing it.
	 */
	error = xfs_imap_to_bp(mp, tp, &ip->i_imap, &dip, &bp, 0, iget_flags);
	if (error)
		return error;

	/* even unallocated inodes are verified */
	fa = xfs_dinode_verify(mp, ip->i_ino, dip);
	if (fa) {
		xfs_inode_verifier_error(ip, -EFSCORRUPTED, "dinode", dip,
				sizeof(*dip), fa);
		error = -EFSCORRUPTED;
		goto out_brelse;
	}

	/*
	 * If the on-disk inode is already linked to a directory
	 * entry, copy all of the inode into the in-core inode.
	 * xfs_iformat_fork() handles copying in the inode format
	 * specific information.
	 * Otherwise, just get the truly permanent information.
	 */
	if (dip->di_mode) {
		xfs_inode_from_disk(ip, dip);
		error = xfs_iformat_fork(ip, dip);
		if (error)  {
#ifdef DEBUG
			xfs_alert(mp, "%s: xfs_iformat() returned error %d",
				__func__, error);
#endif /* DEBUG */
			goto out_brelse;
		}
	} else {
		/*
		 * Partial initialisation of the in-core inode. Just the bits
		 * that xfs_ialloc won't overwrite or relies on being correct.
		 */
		ip->i_d.di_version = dip->di_version;
		VFS_I(ip)->i_generation = be32_to_cpu(dip->di_gen);
		ip->i_d.di_flushiter = be16_to_cpu(dip->di_flushiter);

		/*
		 * Make sure to pull in the mode here as well in
		 * case the inode is released without being used.
		 * This ensures that xfs_inactive() will see that
		 * the inode is already free and not try to mess
		 * with the uninitialized part of it.
		 */
		VFS_I(ip)->i_mode = 0;
	}

	ASSERT(ip->i_d.di_version >= 2);
	ip->i_delayed_blks = 0;

	/*
	 * Mark the buffer containing the inode as something to keep
	 * around for a while.  This helps to keep recently accessed
	 * meta-data in-core longer.
	 */
	xfs_buf_set_ref(bp, XFS_INO_REF);

	/*
	 * Use xfs_trans_brelse() to release the buffer containing the on-disk
	 * inode, because it was acquired with xfs_trans_read_buf() in
	 * xfs_imap_to_bp() above.  If tp is NULL, this is just a normal
	 * brelse().  If we're within a transaction, then xfs_trans_brelse()
	 * will only release the buffer if it is not dirty within the
	 * transaction.  It will be OK to release the buffer in this case,
	 * because inodes on disk are never destroyed and we will be locking the
	 * new in-core inode before putting it in the cache where other
	 * processes can find it.  Thus we don't have to worry about the inode
	 * being changed just because we released the buffer.
	 */
 out_brelse:
	xfs_trans_brelse(tp, bp);
	return error;
}

/*
 * Validate di_extsize hint.
 *
 * The rules are documented at xfs_ioctl_setattr_check_extsize().
 * These functions must be kept in sync with each other.
=======
 * Validate di_extsize hint.
 *
 * 1. Extent size hint is only valid for directories and regular files.
 * 2. FS_XFLAG_EXTSIZE is only valid for regular files.
 * 3. FS_XFLAG_EXTSZINHERIT is only valid for directories.
 * 4. Hint cannot be larger than MAXTEXTLEN.
 * 5. Can be changed on directories at any time.
 * 6. Hint value of 0 turns off hints, clears inode flags.
 * 7. Extent size must be a multiple of the appropriate block size.
 *    For realtime files, this is the rt extent size.
 * 8. For non-realtime files, the extent size hint must be limited
 *    to half the AG size to avoid alignment extending the extent beyond the
 *    limits of the AG.
>>>>>>> upstream/android-13
 */
xfs_failaddr_t
xfs_inode_validate_extsize(
	struct xfs_mount		*mp,
	uint32_t			extsize,
	uint16_t			mode,
	uint16_t			flags)
{
	bool				rt_flag;
	bool				hint_flag;
	bool				inherit_flag;
	uint32_t			extsize_bytes;
	uint32_t			blocksize_bytes;

	rt_flag = (flags & XFS_DIFLAG_REALTIME);
	hint_flag = (flags & XFS_DIFLAG_EXTSIZE);
	inherit_flag = (flags & XFS_DIFLAG_EXTSZINHERIT);
	extsize_bytes = XFS_FSB_TO_B(mp, extsize);

<<<<<<< HEAD
	if (rt_flag)
		blocksize_bytes = mp->m_sb.sb_rextsize << mp->m_sb.sb_blocklog;
=======
	/*
	 * This comment describes a historic gap in this verifier function.
	 *
	 * For a directory with both RTINHERIT and EXTSZINHERIT flags set, this
	 * function has never checked that the extent size hint is an integer
	 * multiple of the realtime extent size.  Since we allow users to set
	 * this combination  on non-rt filesystems /and/ to change the rt
	 * extent size when adding a rt device to a filesystem, the net effect
	 * is that users can configure a filesystem anticipating one rt
	 * geometry and change their minds later.  Directories do not use the
	 * extent size hint, so this is harmless for them.
	 *
	 * If a directory with a misaligned extent size hint is allowed to
	 * propagate that hint into a new regular realtime file, the result
	 * is that the inode cluster buffer verifier will trigger a corruption
	 * shutdown the next time it is run, because the verifier has always
	 * enforced the alignment rule for regular files.
	 *
	 * Because we allow administrators to set a new rt extent size when
	 * adding a rt section, we cannot add a check to this verifier because
	 * that will result a new source of directory corruption errors when
	 * reading an existing filesystem.  Instead, we rely on callers to
	 * decide when alignment checks are appropriate, and fix things up as
	 * needed.
	 */

	if (rt_flag)
		blocksize_bytes = XFS_FSB_TO_B(mp, mp->m_sb.sb_rextsize);
>>>>>>> upstream/android-13
	else
		blocksize_bytes = mp->m_sb.sb_blocksize;

	if ((hint_flag || inherit_flag) && !(S_ISDIR(mode) || S_ISREG(mode)))
		return __this_address;

	if (hint_flag && !S_ISREG(mode))
		return __this_address;

	if (inherit_flag && !S_ISDIR(mode))
		return __this_address;

	if ((hint_flag || inherit_flag) && extsize == 0)
		return __this_address;

	/* free inodes get flags set to zero but extsize remains */
	if (mode && !(hint_flag || inherit_flag) && extsize != 0)
		return __this_address;

	if (extsize_bytes % blocksize_bytes)
		return __this_address;

	if (extsize > MAXEXTLEN)
		return __this_address;

	if (!rt_flag && extsize > mp->m_sb.sb_agblocks / 2)
		return __this_address;

	return NULL;
}

/*
 * Validate di_cowextsize hint.
 *
<<<<<<< HEAD
 * The rules are documented at xfs_ioctl_setattr_check_cowextsize().
 * These functions must be kept in sync with each other.
=======
 * 1. CoW extent size hint can only be set if reflink is enabled on the fs.
 *    The inode does not have to have any shared blocks, but it must be a v3.
 * 2. FS_XFLAG_COWEXTSIZE is only valid for directories and regular files;
 *    for a directory, the hint is propagated to new files.
 * 3. Can be changed on files & directories at any time.
 * 4. Hint value of 0 turns off hints, clears inode flags.
 * 5. Extent size must be a multiple of the appropriate block size.
 * 6. The extent size hint must be limited to half the AG size to avoid
 *    alignment extending the extent beyond the limits of the AG.
>>>>>>> upstream/android-13
 */
xfs_failaddr_t
xfs_inode_validate_cowextsize(
	struct xfs_mount		*mp,
	uint32_t			cowextsize,
	uint16_t			mode,
	uint16_t			flags,
	uint64_t			flags2)
{
	bool				rt_flag;
	bool				hint_flag;
	uint32_t			cowextsize_bytes;

	rt_flag = (flags & XFS_DIFLAG_REALTIME);
	hint_flag = (flags2 & XFS_DIFLAG2_COWEXTSIZE);
	cowextsize_bytes = XFS_FSB_TO_B(mp, cowextsize);

<<<<<<< HEAD
	if (hint_flag && !xfs_sb_version_hasreflink(&mp->m_sb))
=======
	if (hint_flag && !xfs_has_reflink(mp))
>>>>>>> upstream/android-13
		return __this_address;

	if (hint_flag && !(S_ISDIR(mode) || S_ISREG(mode)))
		return __this_address;

	if (hint_flag && cowextsize == 0)
		return __this_address;

	/* free inodes get flags set to zero but cowextsize remains */
	if (mode && !hint_flag && cowextsize != 0)
		return __this_address;

	if (hint_flag && rt_flag)
		return __this_address;

	if (cowextsize_bytes % mp->m_sb.sb_blocksize)
		return __this_address;

	if (cowextsize > MAXEXTLEN)
		return __this_address;

	if (cowextsize > mp->m_sb.sb_agblocks / 2)
		return __this_address;

	return NULL;
}
