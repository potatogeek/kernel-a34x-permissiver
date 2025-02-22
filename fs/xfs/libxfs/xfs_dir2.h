<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2000-2001,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_DIR2_H__
#define __XFS_DIR2_H__

#include "xfs_da_format.h"
#include "xfs_da_btree.h"

struct xfs_da_args;
struct xfs_inode;
struct xfs_mount;
struct xfs_trans;
struct xfs_dir2_sf_hdr;
struct xfs_dir2_sf_entry;
struct xfs_dir2_data_hdr;
struct xfs_dir2_data_entry;
struct xfs_dir2_data_unused;
<<<<<<< HEAD
=======
struct xfs_dir3_icfree_hdr;
struct xfs_dir3_icleaf_hdr;
>>>>>>> upstream/android-13

extern struct xfs_name	xfs_name_dotdot;

/*
 * Convert inode mode to directory entry filetype
 */
extern unsigned char xfs_mode_to_ftype(int mode);

/*
<<<<<<< HEAD
 * directory operations vector for encode/decode routines
 */
struct xfs_dir_ops {
	int	(*sf_entsize)(struct xfs_dir2_sf_hdr *hdr, int len);
	struct xfs_dir2_sf_entry *
		(*sf_nextentry)(struct xfs_dir2_sf_hdr *hdr,
				struct xfs_dir2_sf_entry *sfep);
	uint8_t (*sf_get_ftype)(struct xfs_dir2_sf_entry *sfep);
	void	(*sf_put_ftype)(struct xfs_dir2_sf_entry *sfep,
				uint8_t ftype);
	xfs_ino_t (*sf_get_ino)(struct xfs_dir2_sf_hdr *hdr,
				struct xfs_dir2_sf_entry *sfep);
	void	(*sf_put_ino)(struct xfs_dir2_sf_hdr *hdr,
			      struct xfs_dir2_sf_entry *sfep,
			      xfs_ino_t ino);
	xfs_ino_t (*sf_get_parent_ino)(struct xfs_dir2_sf_hdr *hdr);
	void	(*sf_put_parent_ino)(struct xfs_dir2_sf_hdr *hdr,
				     xfs_ino_t ino);

	int	(*data_entsize)(int len);
	uint8_t (*data_get_ftype)(struct xfs_dir2_data_entry *dep);
	void	(*data_put_ftype)(struct xfs_dir2_data_entry *dep,
				uint8_t ftype);
	__be16 * (*data_entry_tag_p)(struct xfs_dir2_data_entry *dep);
	struct xfs_dir2_data_free *
		(*data_bestfree_p)(struct xfs_dir2_data_hdr *hdr);

	xfs_dir2_data_aoff_t data_dot_offset;
	xfs_dir2_data_aoff_t data_dotdot_offset;
	xfs_dir2_data_aoff_t data_first_offset;
	size_t	data_entry_offset;

	struct xfs_dir2_data_entry *
		(*data_dot_entry_p)(struct xfs_dir2_data_hdr *hdr);
	struct xfs_dir2_data_entry *
		(*data_dotdot_entry_p)(struct xfs_dir2_data_hdr *hdr);
	struct xfs_dir2_data_entry *
		(*data_first_entry_p)(struct xfs_dir2_data_hdr *hdr);
	struct xfs_dir2_data_entry *
		(*data_entry_p)(struct xfs_dir2_data_hdr *hdr);
	struct xfs_dir2_data_unused *
		(*data_unused_p)(struct xfs_dir2_data_hdr *hdr);

	int	leaf_hdr_size;
	void	(*leaf_hdr_to_disk)(struct xfs_dir2_leaf *to,
				    struct xfs_dir3_icleaf_hdr *from);
	void	(*leaf_hdr_from_disk)(struct xfs_dir3_icleaf_hdr *to,
				      struct xfs_dir2_leaf *from);
	int	(*leaf_max_ents)(struct xfs_da_geometry *geo);
	struct xfs_dir2_leaf_entry *
		(*leaf_ents_p)(struct xfs_dir2_leaf *lp);

	int	node_hdr_size;
	void	(*node_hdr_to_disk)(struct xfs_da_intnode *to,
				    struct xfs_da3_icnode_hdr *from);
	void	(*node_hdr_from_disk)(struct xfs_da3_icnode_hdr *to,
				      struct xfs_da_intnode *from);
	struct xfs_da_node_entry *
		(*node_tree_p)(struct xfs_da_intnode *dap);

	int	free_hdr_size;
	void	(*free_hdr_to_disk)(struct xfs_dir2_free *to,
				    struct xfs_dir3_icfree_hdr *from);
	void	(*free_hdr_from_disk)(struct xfs_dir3_icfree_hdr *to,
				      struct xfs_dir2_free *from);
	int	(*free_max_bests)(struct xfs_da_geometry *geo);
	__be16 * (*free_bests_p)(struct xfs_dir2_free *free);
	xfs_dir2_db_t (*db_to_fdb)(struct xfs_da_geometry *geo,
				   xfs_dir2_db_t db);
	int	(*db_to_fdindex)(struct xfs_da_geometry *geo,
				 xfs_dir2_db_t db);
};

extern const struct xfs_dir_ops *
	xfs_dir_get_ops(struct xfs_mount *mp, struct xfs_inode *dp);
extern const struct xfs_dir_ops *
	xfs_nondir_get_ops(struct xfs_mount *mp, struct xfs_inode *dp);

/*
=======
>>>>>>> upstream/android-13
 * Generic directory interface routines
 */
extern void xfs_dir_startup(void);
extern int xfs_da_mount(struct xfs_mount *mp);
extern void xfs_da_unmount(struct xfs_mount *mp);

extern int xfs_dir_isempty(struct xfs_inode *dp);
extern int xfs_dir_init(struct xfs_trans *tp, struct xfs_inode *dp,
				struct xfs_inode *pdp);
extern int xfs_dir_createname(struct xfs_trans *tp, struct xfs_inode *dp,
				struct xfs_name *name, xfs_ino_t inum,
				xfs_extlen_t tot);
extern int xfs_dir_lookup(struct xfs_trans *tp, struct xfs_inode *dp,
				struct xfs_name *name, xfs_ino_t *inum,
				struct xfs_name *ci_name);
extern int xfs_dir_removename(struct xfs_trans *tp, struct xfs_inode *dp,
				struct xfs_name *name, xfs_ino_t ino,
				xfs_extlen_t tot);
extern int xfs_dir_replace(struct xfs_trans *tp, struct xfs_inode *dp,
				struct xfs_name *name, xfs_ino_t inum,
				xfs_extlen_t tot);
extern int xfs_dir_canenter(struct xfs_trans *tp, struct xfs_inode *dp,
				struct xfs_name *name);

/*
 * Direct call from the bmap code, bypassing the generic directory layer.
 */
extern int xfs_dir2_sf_to_block(struct xfs_da_args *args);

/*
 * Interface routines used by userspace utilities
 */
extern int xfs_dir2_isblock(struct xfs_da_args *args, int *r);
extern int xfs_dir2_isleaf(struct xfs_da_args *args, int *r);
extern int xfs_dir2_shrink_inode(struct xfs_da_args *args, xfs_dir2_db_t db,
				struct xfs_buf *bp);

<<<<<<< HEAD
extern void xfs_dir2_data_freescan_int(struct xfs_da_geometry *geo,
		const struct xfs_dir_ops *ops,
		struct xfs_dir2_data_hdr *hdr, int *loghead);
extern void xfs_dir2_data_freescan(struct xfs_inode *dp,
=======
extern void xfs_dir2_data_freescan(struct xfs_mount *mp,
>>>>>>> upstream/android-13
		struct xfs_dir2_data_hdr *hdr, int *loghead);
extern void xfs_dir2_data_log_entry(struct xfs_da_args *args,
		struct xfs_buf *bp, struct xfs_dir2_data_entry *dep);
extern void xfs_dir2_data_log_header(struct xfs_da_args *args,
		struct xfs_buf *bp);
extern void xfs_dir2_data_log_unused(struct xfs_da_args *args,
		struct xfs_buf *bp, struct xfs_dir2_data_unused *dup);
extern void xfs_dir2_data_make_free(struct xfs_da_args *args,
		struct xfs_buf *bp, xfs_dir2_data_aoff_t offset,
		xfs_dir2_data_aoff_t len, int *needlogp, int *needscanp);
extern int xfs_dir2_data_use_free(struct xfs_da_args *args,
		struct xfs_buf *bp, struct xfs_dir2_data_unused *dup,
		xfs_dir2_data_aoff_t offset, xfs_dir2_data_aoff_t len,
		int *needlogp, int *needscanp);

extern struct xfs_dir2_data_free *xfs_dir2_data_freefind(
		struct xfs_dir2_data_hdr *hdr, struct xfs_dir2_data_free *bf,
		struct xfs_dir2_data_unused *dup);

extern int xfs_dir_ino_validate(struct xfs_mount *mp, xfs_ino_t ino);

extern const struct xfs_buf_ops xfs_dir3_block_buf_ops;
extern const struct xfs_buf_ops xfs_dir3_leafn_buf_ops;
extern const struct xfs_buf_ops xfs_dir3_leaf1_buf_ops;
extern const struct xfs_buf_ops xfs_dir3_free_buf_ops;
extern const struct xfs_buf_ops xfs_dir3_data_buf_ops;

/*
 * Directory offset/block conversion functions.
 *
 * DB blocks here are logical directory block numbers, not filesystem blocks.
 */

/*
 * Convert dataptr to byte in file space
 */
static inline xfs_dir2_off_t
xfs_dir2_dataptr_to_byte(xfs_dir2_dataptr_t dp)
{
	return (xfs_dir2_off_t)dp << XFS_DIR2_DATA_ALIGN_LOG;
}

/*
 * Convert byte in file space to dataptr.  It had better be aligned.
 */
static inline xfs_dir2_dataptr_t
xfs_dir2_byte_to_dataptr(xfs_dir2_off_t by)
{
	return (xfs_dir2_dataptr_t)(by >> XFS_DIR2_DATA_ALIGN_LOG);
}

/*
 * Convert byte in space to (DB) block
 */
static inline xfs_dir2_db_t
xfs_dir2_byte_to_db(struct xfs_da_geometry *geo, xfs_dir2_off_t by)
{
	return (xfs_dir2_db_t)(by >> geo->blklog);
}

/*
 * Convert dataptr to a block number
 */
static inline xfs_dir2_db_t
xfs_dir2_dataptr_to_db(struct xfs_da_geometry *geo, xfs_dir2_dataptr_t dp)
{
	return xfs_dir2_byte_to_db(geo, xfs_dir2_dataptr_to_byte(dp));
}

/*
 * Convert byte in space to offset in a block
 */
static inline xfs_dir2_data_aoff_t
xfs_dir2_byte_to_off(struct xfs_da_geometry *geo, xfs_dir2_off_t by)
{
	return (xfs_dir2_data_aoff_t)(by & (geo->blksize - 1));
}

/*
 * Convert dataptr to a byte offset in a block
 */
static inline xfs_dir2_data_aoff_t
xfs_dir2_dataptr_to_off(struct xfs_da_geometry *geo, xfs_dir2_dataptr_t dp)
{
	return xfs_dir2_byte_to_off(geo, xfs_dir2_dataptr_to_byte(dp));
}

/*
 * Convert block and offset to byte in space
 */
static inline xfs_dir2_off_t
xfs_dir2_db_off_to_byte(struct xfs_da_geometry *geo, xfs_dir2_db_t db,
			xfs_dir2_data_aoff_t o)
{
	return ((xfs_dir2_off_t)db << geo->blklog) + o;
}

/*
 * Convert block (DB) to block (dablk)
 */
static inline xfs_dablk_t
xfs_dir2_db_to_da(struct xfs_da_geometry *geo, xfs_dir2_db_t db)
{
	return (xfs_dablk_t)(db << (geo->blklog - geo->fsblog));
}

/*
 * Convert byte in space to (DA) block
 */
static inline xfs_dablk_t
xfs_dir2_byte_to_da(struct xfs_da_geometry *geo, xfs_dir2_off_t by)
{
	return xfs_dir2_db_to_da(geo, xfs_dir2_byte_to_db(geo, by));
}

/*
 * Convert block and offset to dataptr
 */
static inline xfs_dir2_dataptr_t
xfs_dir2_db_off_to_dataptr(struct xfs_da_geometry *geo, xfs_dir2_db_t db,
			   xfs_dir2_data_aoff_t o)
{
	return xfs_dir2_byte_to_dataptr(xfs_dir2_db_off_to_byte(geo, db, o));
}

/*
 * Convert block (dablk) to block (DB)
 */
static inline xfs_dir2_db_t
xfs_dir2_da_to_db(struct xfs_da_geometry *geo, xfs_dablk_t da)
{
	return (xfs_dir2_db_t)(da >> (geo->blklog - geo->fsblog));
}

/*
 * Convert block (dablk) to byte offset in space
 */
static inline xfs_dir2_off_t
xfs_dir2_da_to_byte(struct xfs_da_geometry *geo, xfs_dablk_t da)
{
	return xfs_dir2_db_off_to_byte(geo, xfs_dir2_da_to_db(geo, da), 0);
}

/*
 * Directory tail pointer accessor functions. Based on block geometry.
 */
static inline struct xfs_dir2_block_tail *
xfs_dir2_block_tail_p(struct xfs_da_geometry *geo, struct xfs_dir2_data_hdr *hdr)
{
	return ((struct xfs_dir2_block_tail *)
		((char *)hdr + geo->blksize)) - 1;
}

static inline struct xfs_dir2_leaf_tail *
xfs_dir2_leaf_tail_p(struct xfs_da_geometry *geo, struct xfs_dir2_leaf *lp)
{
	return (struct xfs_dir2_leaf_tail *)
		((char *)lp + geo->blksize -
		  sizeof(struct xfs_dir2_leaf_tail));
}

/*
 * The Linux API doesn't pass down the total size of the buffer
 * we read into down to the filesystem.  With the filldir concept
 * it's not needed for correct information, but the XFS dir2 leaf
 * code wants an estimate of the buffer size to calculate it's
 * readahead window and size the buffers used for mapping to
 * physical blocks.
 *
 * Try to give it an estimate that's good enough, maybe at some
 * point we can change the ->readdir prototype to include the
 * buffer size.  For now we use the current glibc buffer size.
 * musl libc hardcodes 2k and dietlibc uses PAGE_SIZE.
 */
#define XFS_READDIR_BUFSIZE	(32768)

unsigned char xfs_dir3_get_dtype(struct xfs_mount *mp, uint8_t filetype);
<<<<<<< HEAD
void *xfs_dir3_data_endp(struct xfs_da_geometry *geo,
		struct xfs_dir2_data_hdr *hdr);
=======
unsigned int xfs_dir3_data_end_offset(struct xfs_da_geometry *geo,
		struct xfs_dir2_data_hdr *hdr);
bool xfs_dir2_namecheck(const void *name, size_t length);
>>>>>>> upstream/android-13

#endif	/* __XFS_DIR2_H__ */
