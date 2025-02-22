/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2014 Anna Schumaker.
 *
 * NFSv3-specific filesystem definitions and declarations
 */
#ifndef __LINUX_FS_NFS_NFS3_FS_H
#define __LINUX_FS_NFS_NFS3_FS_H

/*
 * nfs3acl.c
 */
#ifdef CONFIG_NFS_V3_ACL
<<<<<<< HEAD
extern struct posix_acl *nfs3_get_acl(struct inode *inode, int type);
extern int nfs3_set_acl(struct inode *inode, struct posix_acl *acl, int type);
=======
extern struct posix_acl *nfs3_get_acl(struct inode *inode, int type, bool rcu);
extern int nfs3_set_acl(struct user_namespace *mnt_userns, struct inode *inode,
			struct posix_acl *acl, int type);
>>>>>>> upstream/android-13
extern int nfs3_proc_setacls(struct inode *inode, struct posix_acl *acl,
		struct posix_acl *dfacl);
extern ssize_t nfs3_listxattr(struct dentry *, char *, size_t);
extern const struct xattr_handler *nfs3_xattr_handlers[];
#else
static inline int nfs3_proc_setacls(struct inode *inode, struct posix_acl *acl,
		struct posix_acl *dfacl)
{
	return 0;
}
#define nfs3_listxattr NULL
#endif /* CONFIG_NFS_V3_ACL */

/* nfs3client.c */
<<<<<<< HEAD
struct nfs_server *nfs3_create_server(struct nfs_mount_info *, struct nfs_subversion *);
=======
struct nfs_server *nfs3_create_server(struct fs_context *);
>>>>>>> upstream/android-13
struct nfs_server *nfs3_clone_server(struct nfs_server *, struct nfs_fh *,
				     struct nfs_fattr *, rpc_authflavor_t);

/* nfs3super.c */
extern struct nfs_subversion nfs_v3;

#endif /* __LINUX_FS_NFS_NFS3_FS_H */
