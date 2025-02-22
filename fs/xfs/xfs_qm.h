// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_QM_H__
#define __XFS_QM_H__

#include "xfs_dquot_item.h"
#include "xfs_dquot.h"

struct xfs_inode;

extern struct kmem_zone	*xfs_qm_dqtrxzone;

/*
 * Number of bmaps that we ask from bmapi when doing a quotacheck.
 * We make this restriction to keep the memory usage to a minimum.
 */
#define XFS_DQITER_MAP_SIZE	10

#define XFS_IS_DQUOT_UNINITIALIZED(dqp) ( \
<<<<<<< HEAD
	!dqp->q_core.d_blk_hardlimit && \
	!dqp->q_core.d_blk_softlimit && \
	!dqp->q_core.d_rtb_hardlimit && \
	!dqp->q_core.d_rtb_softlimit && \
	!dqp->q_core.d_ino_hardlimit && \
	!dqp->q_core.d_ino_softlimit && \
	!dqp->q_core.d_bcount && \
	!dqp->q_core.d_rtbcount && \
	!dqp->q_core.d_icount)

/*
 * This defines the unit of allocation of dquots.
 * Currently, it is just one file system block, and a 4K blk contains 30
 * (136 * 30 = 4080) dquots. It's probably not worth trying to make
 * this more dynamic.
 * XXXsup However, if this number is changed, we have to make sure that we don't
 * implicitly assume that we do allocations in chunks of a single filesystem
 * block in the dquot/xqm code.
 */
#define XFS_DQUOT_CLUSTER_SIZE_FSB	(xfs_filblks_t)1

struct xfs_def_quota {
	xfs_qcnt_t       bhardlimit;     /* default data blk hard limit */
	xfs_qcnt_t       bsoftlimit;	 /* default data blk soft limit */
	xfs_qcnt_t       ihardlimit;	 /* default inode count hard limit */
	xfs_qcnt_t       isoftlimit;	 /* default inode count soft limit */
	xfs_qcnt_t	 rtbhardlimit;   /* default realtime blk hard limit */
	xfs_qcnt_t	 rtbsoftlimit;   /* default realtime blk soft limit */
=======
	!dqp->q_blk.hardlimit && \
	!dqp->q_blk.softlimit && \
	!dqp->q_rtb.hardlimit && \
	!dqp->q_rtb.softlimit && \
	!dqp->q_ino.hardlimit && \
	!dqp->q_ino.softlimit && \
	!dqp->q_blk.count && \
	!dqp->q_rtb.count && \
	!dqp->q_ino.count)

struct xfs_quota_limits {
	xfs_qcnt_t		hard;	/* default hard limit */
	xfs_qcnt_t		soft;	/* default soft limit */
	time64_t		time;	/* limit for timers */
	xfs_qwarncnt_t		warn;	/* limit for warnings */
};

/* Defaults for each quota type: time limits, warn limits, usage limits */
struct xfs_def_quota {
	struct xfs_quota_limits	blk;
	struct xfs_quota_limits	ino;
	struct xfs_quota_limits	rtb;
>>>>>>> upstream/android-13
};

/*
 * Various quota information for individual filesystems.
 * The mount structure keeps a pointer to this.
 */
<<<<<<< HEAD
typedef struct xfs_quotainfo {
	struct radix_tree_root qi_uquota_tree;
	struct radix_tree_root qi_gquota_tree;
	struct radix_tree_root qi_pquota_tree;
	struct mutex qi_tree_lock;
	struct xfs_inode	*qi_uquotaip;	/* user quota inode */
	struct xfs_inode	*qi_gquotaip;	/* group quota inode */
	struct xfs_inode	*qi_pquotaip;	/* project quota inode */
	struct list_lru	 qi_lru;
	int		 qi_dquots;
	time_t		 qi_btimelimit;	 /* limit for blks timer */
	time_t		 qi_itimelimit;	 /* limit for inodes timer */
	time_t		 qi_rtbtimelimit;/* limit for rt blks timer */
	xfs_qwarncnt_t	 qi_bwarnlimit;	 /* limit for blks warnings */
	xfs_qwarncnt_t	 qi_iwarnlimit;	 /* limit for inodes warnings */
	xfs_qwarncnt_t	 qi_rtbwarnlimit;/* limit for rt blks warnings */
	struct mutex	 qi_quotaofflock;/* to serialize quotaoff */
	xfs_filblks_t	 qi_dqchunklen;	 /* # BBs in a chunk of dqs */
	uint		 qi_dqperchunk;	 /* # ondisk dqs in above chunk */
	struct xfs_def_quota	qi_usr_default;
	struct xfs_def_quota	qi_grp_default;
	struct xfs_def_quota	qi_prj_default;
	struct shrinker  qi_shrinker;
} xfs_quotainfo_t;
=======
struct xfs_quotainfo {
	struct radix_tree_root	qi_uquota_tree;
	struct radix_tree_root	qi_gquota_tree;
	struct radix_tree_root	qi_pquota_tree;
	struct mutex		qi_tree_lock;
	struct xfs_inode	*qi_uquotaip;	/* user quota inode */
	struct xfs_inode	*qi_gquotaip;	/* group quota inode */
	struct xfs_inode	*qi_pquotaip;	/* project quota inode */
	struct list_lru		qi_lru;
	int			qi_dquots;
	struct mutex		qi_quotaofflock;/* to serialize quotaoff */
	xfs_filblks_t		qi_dqchunklen;	/* # BBs in a chunk of dqs */
	uint			qi_dqperchunk;	/* # ondisk dq in above chunk */
	struct xfs_def_quota	qi_usr_default;
	struct xfs_def_quota	qi_grp_default;
	struct xfs_def_quota	qi_prj_default;
	struct shrinker		qi_shrinker;

	/* Minimum and maximum quota expiration timestamp values. */
	time64_t		qi_expiry_min;
	time64_t		qi_expiry_max;
};
>>>>>>> upstream/android-13

static inline struct radix_tree_root *
xfs_dquot_tree(
	struct xfs_quotainfo	*qi,
<<<<<<< HEAD
	int			type)
{
	switch (type) {
	case XFS_DQ_USER:
		return &qi->qi_uquota_tree;
	case XFS_DQ_GROUP:
		return &qi->qi_gquota_tree;
	case XFS_DQ_PROJ:
=======
	xfs_dqtype_t		type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return &qi->qi_uquota_tree;
	case XFS_DQTYPE_GROUP:
		return &qi->qi_gquota_tree;
	case XFS_DQTYPE_PROJ:
>>>>>>> upstream/android-13
		return &qi->qi_pquota_tree;
	default:
		ASSERT(0);
	}
	return NULL;
}

static inline struct xfs_inode *
<<<<<<< HEAD
xfs_quota_inode(xfs_mount_t *mp, uint dq_flags)
{
	switch (dq_flags & XFS_DQ_ALLTYPES) {
	case XFS_DQ_USER:
		return mp->m_quotainfo->qi_uquotaip;
	case XFS_DQ_GROUP:
		return mp->m_quotainfo->qi_gquotaip;
	case XFS_DQ_PROJ:
=======
xfs_quota_inode(struct xfs_mount *mp, xfs_dqtype_t type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return mp->m_quotainfo->qi_uquotaip;
	case XFS_DQTYPE_GROUP:
		return mp->m_quotainfo->qi_gquotaip;
	case XFS_DQTYPE_PROJ:
>>>>>>> upstream/android-13
		return mp->m_quotainfo->qi_pquotaip;
	default:
		ASSERT(0);
	}
	return NULL;
}

<<<<<<< HEAD
extern void	xfs_trans_mod_dquot(struct xfs_trans *,
					struct xfs_dquot *, uint, long);
extern int	xfs_trans_reserve_quota_bydquots(struct xfs_trans *,
			struct xfs_mount *, struct xfs_dquot *,
			struct xfs_dquot *, struct xfs_dquot *,
			long, long, uint);
=======
extern void	xfs_trans_mod_dquot(struct xfs_trans *tp, struct xfs_dquot *dqp,
				    uint field, int64_t delta);
>>>>>>> upstream/android-13
extern void	xfs_trans_dqjoin(struct xfs_trans *, struct xfs_dquot *);
extern void	xfs_trans_log_dquot(struct xfs_trans *, struct xfs_dquot *);

/*
 * We keep the usr, grp, and prj dquots separately so that locking will be
 * easier to do at commit time. All transactions that we know of at this point
 * affect no more than two dquots of one type. Hence, the TRANS_MAXDQS value.
 */
enum {
	XFS_QM_TRANS_USR = 0,
	XFS_QM_TRANS_GRP,
	XFS_QM_TRANS_PRJ,
	XFS_QM_TRANS_DQTYPES
};
#define XFS_QM_TRANS_MAXDQS		2
struct xfs_dquot_acct {
	struct xfs_dqtrx	dqs[XFS_QM_TRANS_DQTYPES][XFS_QM_TRANS_MAXDQS];
};

/*
 * Users are allowed to have a usage exceeding their softlimit for
 * a period this long.
 */
#define XFS_QM_BTIMELIMIT	(7 * 24*60*60)          /* 1 week */
#define XFS_QM_RTBTIMELIMIT	(7 * 24*60*60)          /* 1 week */
#define XFS_QM_ITIMELIMIT	(7 * 24*60*60)          /* 1 week */

#define XFS_QM_BWARNLIMIT	5
#define XFS_QM_IWARNLIMIT	5
#define XFS_QM_RTBWARNLIMIT	5

extern void		xfs_qm_destroy_quotainfo(struct xfs_mount *);

<<<<<<< HEAD
/* dquot stuff */
extern void		xfs_qm_dqpurge_all(struct xfs_mount *, uint);
extern void		xfs_qm_dqrele_all_inodes(struct xfs_mount *, uint);

/* quota ops */
extern int		xfs_qm_scall_trunc_qfiles(struct xfs_mount *, uint);
extern int		xfs_qm_scall_getquota(struct xfs_mount *, xfs_dqid_t,
					uint, struct qc_dqblk *);
extern int		xfs_qm_scall_getquota_next(struct xfs_mount *,
					xfs_dqid_t *, uint, struct qc_dqblk *);
extern int		xfs_qm_scall_setqlim(struct xfs_mount *, xfs_dqid_t, uint,
					struct qc_dqblk *);
=======
/* quota ops */
extern int		xfs_qm_scall_trunc_qfiles(struct xfs_mount *, uint);
extern int		xfs_qm_scall_getquota(struct xfs_mount *mp,
					xfs_dqid_t id,
					xfs_dqtype_t type,
					struct qc_dqblk *dst);
extern int		xfs_qm_scall_getquota_next(struct xfs_mount *mp,
					xfs_dqid_t *id,
					xfs_dqtype_t type,
					struct qc_dqblk *dst);
extern int		xfs_qm_scall_setqlim(struct xfs_mount *mp,
					xfs_dqid_t id,
					xfs_dqtype_t type,
					struct qc_dqblk *newlim);
>>>>>>> upstream/android-13
extern int		xfs_qm_scall_quotaon(struct xfs_mount *, uint);
extern int		xfs_qm_scall_quotaoff(struct xfs_mount *, uint);

static inline struct xfs_def_quota *
<<<<<<< HEAD
xfs_get_defquota(struct xfs_dquot *dqp, struct xfs_quotainfo *qi)
{
	struct xfs_def_quota *defq;

	if (XFS_QM_ISUDQ(dqp))
		defq = &qi->qi_usr_default;
	else if (XFS_QM_ISGDQ(dqp))
		defq = &qi->qi_grp_default;
	else {
		ASSERT(XFS_QM_ISPDQ(dqp));
		defq = &qi->qi_prj_default;
	}
	return defq;
=======
xfs_get_defquota(struct xfs_quotainfo *qi, xfs_dqtype_t type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return &qi->qi_usr_default;
	case XFS_DQTYPE_GROUP:
		return &qi->qi_grp_default;
	case XFS_DQTYPE_PROJ:
		return &qi->qi_prj_default;
	default:
		ASSERT(0);
		return NULL;
	}
>>>>>>> upstream/android-13
}

#endif /* __XFS_QM_H__ */
