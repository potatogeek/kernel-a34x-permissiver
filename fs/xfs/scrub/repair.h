// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018 Oracle.  All Rights Reserved.
 * Author: Darrick J. Wong <darrick.wong@oracle.com>
 */
#ifndef __XFS_SCRUB_REPAIR_H__
#define __XFS_SCRUB_REPAIR_H__

<<<<<<< HEAD
=======
#include "xfs_quota_defs.h"

>>>>>>> upstream/android-13
static inline int xrep_notsupported(struct xfs_scrub *sc)
{
	return -EOPNOTSUPP;
}

#ifdef CONFIG_XFS_ONLINE_REPAIR

/* Repair helpers */

<<<<<<< HEAD
int xrep_attempt(struct xfs_inode *ip, struct xfs_scrub *sc, bool *fixed);
=======
int xrep_attempt(struct xfs_scrub *sc);
>>>>>>> upstream/android-13
void xrep_failure(struct xfs_mount *mp);
int xrep_roll_ag_trans(struct xfs_scrub *sc);
bool xrep_ag_has_space(struct xfs_perag *pag, xfs_extlen_t nr_blocks,
		enum xfs_ag_resv_type type);
xfs_extlen_t xrep_calc_ag_resblks(struct xfs_scrub *sc);
<<<<<<< HEAD
int xrep_alloc_ag_block(struct xfs_scrub *sc, struct xfs_owner_info *oinfo,
		xfs_fsblock_t *fsbno, enum xfs_ag_resv_type resv);
=======
int xrep_alloc_ag_block(struct xfs_scrub *sc,
		const struct xfs_owner_info *oinfo, xfs_fsblock_t *fsbno,
		enum xfs_ag_resv_type resv);
>>>>>>> upstream/android-13
int xrep_init_btblock(struct xfs_scrub *sc, xfs_fsblock_t fsb,
		struct xfs_buf **bpp, xfs_btnum_t btnum,
		const struct xfs_buf_ops *ops);

<<<<<<< HEAD
struct xfs_bitmap;

int xrep_fix_freelist(struct xfs_scrub *sc, bool can_shrink);
int xrep_invalidate_blocks(struct xfs_scrub *sc, struct xfs_bitmap *btlist);
int xrep_reap_extents(struct xfs_scrub *sc, struct xfs_bitmap *exlist,
		struct xfs_owner_info *oinfo, enum xfs_ag_resv_type type);
=======
struct xbitmap;

int xrep_fix_freelist(struct xfs_scrub *sc, bool can_shrink);
int xrep_invalidate_blocks(struct xfs_scrub *sc, struct xbitmap *btlist);
int xrep_reap_extents(struct xfs_scrub *sc, struct xbitmap *exlist,
		const struct xfs_owner_info *oinfo, enum xfs_ag_resv_type type);
>>>>>>> upstream/android-13

struct xrep_find_ag_btree {
	/* in: rmap owner of the btree we're looking for */
	uint64_t			rmap_owner;

	/* in: buffer ops */
	const struct xfs_buf_ops	*buf_ops;

<<<<<<< HEAD
	/* in: magic number of the btree */
	uint32_t			magic;

=======
>>>>>>> upstream/android-13
	/* out: the highest btree block found and the tree height */
	xfs_agblock_t			root;
	unsigned int			height;
};

int xrep_find_ag_btree_roots(struct xfs_scrub *sc, struct xfs_buf *agf_bp,
		struct xrep_find_ag_btree *btree_info, struct xfs_buf *agfl_bp);
<<<<<<< HEAD
void xrep_force_quotacheck(struct xfs_scrub *sc, uint dqtype);
=======
void xrep_force_quotacheck(struct xfs_scrub *sc, xfs_dqtype_t type);
>>>>>>> upstream/android-13
int xrep_ino_dqattach(struct xfs_scrub *sc);

/* Metadata repairers */

int xrep_probe(struct xfs_scrub *sc);
int xrep_superblock(struct xfs_scrub *sc);
int xrep_agf(struct xfs_scrub *sc);
int xrep_agfl(struct xfs_scrub *sc);
int xrep_agi(struct xfs_scrub *sc);

#else

<<<<<<< HEAD
static inline int xrep_attempt(
	struct xfs_inode	*ip,
	struct xfs_scrub	*sc,
	bool			*fixed)
=======
static inline int
xrep_attempt(
	struct xfs_scrub	*sc)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

static inline void xrep_failure(struct xfs_mount *mp) {}

static inline xfs_extlen_t
xrep_calc_ag_resblks(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	ASSERT(!(sc->sm->sm_flags & XFS_SCRUB_IFLAG_REPAIR));
=======
>>>>>>> upstream/android-13
	return 0;
}

#define xrep_probe			xrep_notsupported
#define xrep_superblock			xrep_notsupported
#define xrep_agf			xrep_notsupported
#define xrep_agfl			xrep_notsupported
#define xrep_agi			xrep_notsupported

#endif /* CONFIG_XFS_ONLINE_REPAIR */

#endif	/* __XFS_SCRUB_REPAIR_H__ */
