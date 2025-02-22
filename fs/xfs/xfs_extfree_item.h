// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef	__XFS_EXTFREE_ITEM_H__
#define	__XFS_EXTFREE_ITEM_H__

/* kernel only EFI/EFD definitions */

struct xfs_mount;
struct kmem_zone;

/*
 * Max number of extents in fast allocation path.
 */
#define	XFS_EFI_MAX_FAST_EXTENTS	16

/*
<<<<<<< HEAD
 * Define EFI flag bits. Manipulated by set/clear/test_bit operators.
 */
#define	XFS_EFI_RECOVERED	1

/*
=======
>>>>>>> upstream/android-13
 * This is the "extent free intention" log item.  It is used to log the fact
 * that some extents need to be free.  It is used in conjunction with the
 * "extent free done" log item described below.
 *
 * The EFI is reference counted so that it is not freed prior to both the EFI
 * and EFD being committed and unpinned. This ensures the EFI is inserted into
 * the AIL even in the event of out of order EFI/EFD processing. In other words,
 * an EFI is born with two references:
 *
 * 	1.) an EFI held reference to track EFI AIL insertion
 * 	2.) an EFD held reference to track EFD commit
 *
 * On allocation, both references are the responsibility of the caller. Once the
 * EFI is added to and dirtied in a transaction, ownership of reference one
 * transfers to the transaction. The reference is dropped once the EFI is
 * inserted to the AIL or in the event of failure along the way (e.g., commit
 * failure, log I/O error, etc.). Note that the caller remains responsible for
 * the EFD reference under all circumstances to this point. The caller has no
 * means to detect failure once the transaction is committed, however.
 * Therefore, an EFD is required after this point, even in the event of
 * unrelated failure.
 *
 * Once an EFD is allocated and dirtied in a transaction, reference two
 * transfers to the transaction. The EFD reference is dropped once it reaches
 * the unpin handler. Similar to the EFI, the reference also drops in the event
 * of commit failure or log I/O errors. Note that the EFD is not inserted in the
 * AIL, so at this point both the EFI and EFD are freed.
 */
<<<<<<< HEAD
typedef struct xfs_efi_log_item {
	xfs_log_item_t		efi_item;
	atomic_t		efi_refcount;
	atomic_t		efi_next_extent;
	unsigned long		efi_flags;	/* misc flags */
	xfs_efi_log_format_t	efi_format;
} xfs_efi_log_item_t;
=======
struct xfs_efi_log_item {
	struct xfs_log_item	efi_item;
	atomic_t		efi_refcount;
	atomic_t		efi_next_extent;
	xfs_efi_log_format_t	efi_format;
};
>>>>>>> upstream/android-13

/*
 * This is the "extent free done" log item.  It is used to log
 * the fact that some extents earlier mentioned in an efi item
 * have been freed.
 */
<<<<<<< HEAD
typedef struct xfs_efd_log_item {
	xfs_log_item_t		efd_item;
	xfs_efi_log_item_t	*efd_efip;
	uint			efd_next_extent;
	xfs_efd_log_format_t	efd_format;
} xfs_efd_log_item_t;
=======
struct xfs_efd_log_item {
	struct xfs_log_item	efd_item;
	struct xfs_efi_log_item *efd_efip;
	uint			efd_next_extent;
	xfs_efd_log_format_t	efd_format;
};
>>>>>>> upstream/android-13

/*
 * Max number of extents in fast allocation path.
 */
#define	XFS_EFD_MAX_FAST_EXTENTS	16

extern struct kmem_zone	*xfs_efi_zone;
extern struct kmem_zone	*xfs_efd_zone;

<<<<<<< HEAD
xfs_efi_log_item_t	*xfs_efi_init(struct xfs_mount *, uint);
xfs_efd_log_item_t	*xfs_efd_init(struct xfs_mount *, xfs_efi_log_item_t *,
				      uint);
int			xfs_efi_copy_format(xfs_log_iovec_t *buf,
					    xfs_efi_log_format_t *dst_efi_fmt);
void			xfs_efi_item_free(xfs_efi_log_item_t *);
void			xfs_efi_release(struct xfs_efi_log_item *);

int			xfs_efi_recover(struct xfs_mount *mp,
					struct xfs_efi_log_item *efip);

=======
>>>>>>> upstream/android-13
#endif	/* __XFS_EXTFREE_ITEM_H__ */
