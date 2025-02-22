// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_QUOTA_H__
#define __XFS_QUOTA_H__

#include "xfs_quota_defs.h"

/*
 * Kernel only quota definitions and functions
 */

struct xfs_trans;
<<<<<<< HEAD
=======
struct xfs_buf;
>>>>>>> upstream/android-13

/*
 * This check is done typically without holding the inode lock;
 * that may seem racy, but it is harmless in the context that it is used.
 * The inode cannot go inactive as long a reference is kept, and
 * therefore if dquot(s) were attached, they'll stay consistent.
 * If, for example, the ownership of the inode changes while
 * we didn't have the inode locked, the appropriate dquot(s) will be
 * attached atomically.
 */
#define XFS_NOT_DQATTACHED(mp, ip) \
	((XFS_IS_UQUOTA_ON(mp) && (ip)->i_udquot == NULL) || \
	 (XFS_IS_GQUOTA_ON(mp) && (ip)->i_gdquot == NULL) || \
	 (XFS_IS_PQUOTA_ON(mp) && (ip)->i_pdquot == NULL))

#define XFS_QM_NEED_QUOTACHECK(mp) \
	((XFS_IS_UQUOTA_ON(mp) && \
		(mp->m_sb.sb_qflags & XFS_UQUOTA_CHKD) == 0) || \
	 (XFS_IS_GQUOTA_ON(mp) && \
		(mp->m_sb.sb_qflags & XFS_GQUOTA_CHKD) == 0) || \
	 (XFS_IS_PQUOTA_ON(mp) && \
		(mp->m_sb.sb_qflags & XFS_PQUOTA_CHKD) == 0))

static inline uint
xfs_quota_chkd_flag(
<<<<<<< HEAD
	uint		dqtype)
{
	switch (dqtype) {
	case XFS_DQ_USER:
		return XFS_UQUOTA_CHKD;
	case XFS_DQ_GROUP:
		return XFS_GQUOTA_CHKD;
	case XFS_DQ_PROJ:
=======
	xfs_dqtype_t		type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return XFS_UQUOTA_CHKD;
	case XFS_DQTYPE_GROUP:
		return XFS_GQUOTA_CHKD;
	case XFS_DQTYPE_PROJ:
>>>>>>> upstream/android-13
		return XFS_PQUOTA_CHKD;
	default:
		return 0;
	}
}

/*
 * The structure kept inside the xfs_trans_t keep track of dquot changes
 * within a transaction and apply them later.
 */
<<<<<<< HEAD
typedef struct xfs_dqtrx {
	struct xfs_dquot *qt_dquot;	  /* the dquot this refers to */
	ulong		qt_blk_res;	  /* blks reserved on a dquot */
	ulong		qt_ino_res;	  /* inode reserved on a dquot */
	ulong		qt_ino_res_used;  /* inodes used from the reservation */
	long		qt_bcount_delta;  /* dquot blk count changes */
	long		qt_delbcnt_delta; /* delayed dquot blk count changes */
	long		qt_icount_delta;  /* dquot inode count changes */
	ulong		qt_rtblk_res;	  /* # blks reserved on a dquot */
	ulong		qt_rtblk_res_used;/* # blks used from reservation */
	long		qt_rtbcount_delta;/* dquot realtime blk changes */
	long		qt_delrtb_delta;  /* delayed RT blk count changes */
} xfs_dqtrx_t;
=======
struct xfs_dqtrx {
	struct xfs_dquot *qt_dquot;	  /* the dquot this refers to */

	uint64_t	qt_blk_res;	  /* blks reserved on a dquot */
	int64_t		qt_bcount_delta;  /* dquot blk count changes */
	int64_t		qt_delbcnt_delta; /* delayed dquot blk count changes */

	uint64_t	qt_rtblk_res;	  /* # blks reserved on a dquot */
	uint64_t	qt_rtblk_res_used;/* # blks used from reservation */
	int64_t		qt_rtbcount_delta;/* dquot realtime blk changes */
	int64_t		qt_delrtb_delta;  /* delayed RT blk count changes */

	uint64_t	qt_ino_res;	  /* inode reserved on a dquot */
	uint64_t	qt_ino_res_used;  /* inodes used from the reservation */
	int64_t		qt_icount_delta;  /* dquot inode count changes */
};
>>>>>>> upstream/android-13

#ifdef CONFIG_XFS_QUOTA
extern void xfs_trans_dup_dqinfo(struct xfs_trans *, struct xfs_trans *);
extern void xfs_trans_free_dqinfo(struct xfs_trans *);
extern void xfs_trans_mod_dquot_byino(struct xfs_trans *, struct xfs_inode *,
<<<<<<< HEAD
		uint, long);
extern void xfs_trans_apply_dquot_deltas(struct xfs_trans *);
extern void xfs_trans_unreserve_and_mod_dquots(struct xfs_trans *);
extern int xfs_trans_reserve_quota_nblks(struct xfs_trans *,
		struct xfs_inode *, long, long, uint);
extern int xfs_trans_reserve_quota_bydquots(struct xfs_trans *,
		struct xfs_mount *, struct xfs_dquot *,
		struct xfs_dquot *, struct xfs_dquot *, long, long, uint);

extern int xfs_qm_vop_dqalloc(struct xfs_inode *, xfs_dqid_t, xfs_dqid_t,
=======
		uint, int64_t);
extern void xfs_trans_apply_dquot_deltas(struct xfs_trans *);
extern void xfs_trans_unreserve_and_mod_dquots(struct xfs_trans *);
int xfs_trans_reserve_quota_nblks(struct xfs_trans *tp, struct xfs_inode *ip,
		int64_t dblocks, int64_t rblocks, bool force);
extern int xfs_trans_reserve_quota_bydquots(struct xfs_trans *,
		struct xfs_mount *, struct xfs_dquot *,
		struct xfs_dquot *, struct xfs_dquot *, int64_t, long, uint);
int xfs_trans_reserve_quota_icreate(struct xfs_trans *tp,
		struct xfs_dquot *udqp, struct xfs_dquot *gdqp,
		struct xfs_dquot *pdqp, int64_t dblocks);

extern int xfs_qm_vop_dqalloc(struct xfs_inode *, kuid_t, kgid_t,
>>>>>>> upstream/android-13
		prid_t, uint, struct xfs_dquot **, struct xfs_dquot **,
		struct xfs_dquot **);
extern void xfs_qm_vop_create_dqattach(struct xfs_trans *, struct xfs_inode *,
		struct xfs_dquot *, struct xfs_dquot *, struct xfs_dquot *);
extern int xfs_qm_vop_rename_dqattach(struct xfs_inode **);
extern struct xfs_dquot *xfs_qm_vop_chown(struct xfs_trans *,
		struct xfs_inode *, struct xfs_dquot **, struct xfs_dquot *);
<<<<<<< HEAD
extern int xfs_qm_vop_chown_reserve(struct xfs_trans *, struct xfs_inode *,
		struct xfs_dquot *, struct xfs_dquot *,
		struct xfs_dquot *, uint);
=======
>>>>>>> upstream/android-13
extern int xfs_qm_dqattach(struct xfs_inode *);
extern int xfs_qm_dqattach_locked(struct xfs_inode *ip, bool doalloc);
extern void xfs_qm_dqdetach(struct xfs_inode *);
extern void xfs_qm_dqrele(struct xfs_dquot *);
extern void xfs_qm_statvfs(struct xfs_inode *, struct kstatfs *);
extern int xfs_qm_newmount(struct xfs_mount *, uint *, uint *);
extern void xfs_qm_mount_quotas(struct xfs_mount *);
extern void xfs_qm_unmount(struct xfs_mount *);
extern void xfs_qm_unmount_quotas(struct xfs_mount *);

<<<<<<< HEAD
#else
static inline int
xfs_qm_vop_dqalloc(struct xfs_inode *ip, xfs_dqid_t uid, xfs_dqid_t gid,
=======
static inline int
xfs_quota_reserve_blkres(struct xfs_inode *ip, int64_t blocks)
{
	return xfs_trans_reserve_quota_nblks(NULL, ip, blocks, 0, false);
}
bool xfs_inode_near_dquot_enforcement(struct xfs_inode *ip, xfs_dqtype_t type);
#else
static inline int
xfs_qm_vop_dqalloc(struct xfs_inode *ip, kuid_t kuid, kgid_t kgid,
>>>>>>> upstream/android-13
		prid_t prid, uint flags, struct xfs_dquot **udqp,
		struct xfs_dquot **gdqp, struct xfs_dquot **pdqp)
{
	*udqp = NULL;
	*gdqp = NULL;
	*pdqp = NULL;
	return 0;
}
#define xfs_trans_dup_dqinfo(tp, tp2)
#define xfs_trans_free_dqinfo(tp)
<<<<<<< HEAD
#define xfs_trans_mod_dquot_byino(tp, ip, fields, delta)
#define xfs_trans_apply_dquot_deltas(tp)
#define xfs_trans_unreserve_and_mod_dquots(tp)
static inline int xfs_trans_reserve_quota_nblks(struct xfs_trans *tp,
		struct xfs_inode *ip, long nblks, long ninos, uint flags)
=======
#define xfs_trans_mod_dquot_byino(tp, ip, fields, delta) do { } while (0)
#define xfs_trans_apply_dquot_deltas(tp)
#define xfs_trans_unreserve_and_mod_dquots(tp)
static inline int xfs_trans_reserve_quota_nblks(struct xfs_trans *tp,
		struct xfs_inode *ip, int64_t dblocks, int64_t rblocks,
		bool force)
>>>>>>> upstream/android-13
{
	return 0;
}
static inline int xfs_trans_reserve_quota_bydquots(struct xfs_trans *tp,
		struct xfs_mount *mp, struct xfs_dquot *udqp,
		struct xfs_dquot *gdqp, struct xfs_dquot *pdqp,
<<<<<<< HEAD
		long nblks, long nions, uint flags)
{
	return 0;
}
#define xfs_qm_vop_create_dqattach(tp, ip, u, g, p)
#define xfs_qm_vop_rename_dqattach(it)					(0)
#define xfs_qm_vop_chown(tp, ip, old, new)				(NULL)
#define xfs_qm_vop_chown_reserve(tp, ip, u, g, p, fl)			(0)
#define xfs_qm_dqattach(ip)						(0)
#define xfs_qm_dqattach_locked(ip, fl)					(0)
#define xfs_qm_dqdetach(ip)
#define xfs_qm_dqrele(d)
#define xfs_qm_statvfs(ip, s)
=======
		int64_t nblks, long nions, uint flags)
{
	return 0;
}

static inline int
xfs_quota_reserve_blkres(struct xfs_inode *ip, int64_t blocks)
{
	return 0;
}

static inline int
xfs_trans_reserve_quota_icreate(struct xfs_trans *tp, struct xfs_dquot *udqp,
		struct xfs_dquot *gdqp, struct xfs_dquot *pdqp, int64_t dblocks)
{
	return 0;
}

#define xfs_qm_vop_create_dqattach(tp, ip, u, g, p)
#define xfs_qm_vop_rename_dqattach(it)					(0)
#define xfs_qm_vop_chown(tp, ip, old, new)				(NULL)
#define xfs_qm_dqattach(ip)						(0)
#define xfs_qm_dqattach_locked(ip, fl)					(0)
#define xfs_qm_dqdetach(ip)
#define xfs_qm_dqrele(d)			do { (d) = (d); } while(0)
#define xfs_qm_statvfs(ip, s)			do { } while(0)
>>>>>>> upstream/android-13
#define xfs_qm_newmount(mp, a, b)					(0)
#define xfs_qm_mount_quotas(mp)
#define xfs_qm_unmount(mp)
#define xfs_qm_unmount_quotas(mp)
<<<<<<< HEAD
#endif /* CONFIG_XFS_QUOTA */

#define xfs_trans_unreserve_quota_nblks(tp, ip, nblks, ninos, flags) \
	xfs_trans_reserve_quota_nblks(tp, ip, -(nblks), -(ninos), flags)
#define xfs_trans_reserve_quota(tp, mp, ud, gd, pd, nb, ni, f) \
	xfs_trans_reserve_quota_bydquots(tp, mp, ud, gd, pd, nb, ni, \
				f | XFS_QMOPT_RES_REGBLKS)
=======
#define xfs_inode_near_dquot_enforcement(ip, type)			(false)
#endif /* CONFIG_XFS_QUOTA */

static inline int
xfs_quota_unreserve_blkres(struct xfs_inode *ip, int64_t blocks)
{
	return xfs_quota_reserve_blkres(ip, -blocks);
}
>>>>>>> upstream/android-13

extern int xfs_mount_reset_sbqflags(struct xfs_mount *);

#endif	/* __XFS_QUOTA_H__ */
