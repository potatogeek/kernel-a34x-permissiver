<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *   Contains the CIFS DFS referral mounting routines used for handling
 *   traversal via DFS junction point
 *
 *   Copyright (c) 2007 Igor Mammedov
 *   Copyright (C) International Business Machines  Corp., 2008
 *   Author(s): Igor Mammedov (niallain@gmail.com)
 *		Steve French (sfrench@us.ibm.com)
<<<<<<< HEAD
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version
 *   2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/dcache.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/vfs.h>
#include <linux/fs.h>
#include <linux/inet.h>
#include "cifsglob.h"
#include "cifsproto.h"
#include "cifsfs.h"
#include "dns_resolve.h"
#include "cifs_debug.h"
#include "cifs_unicode.h"
<<<<<<< HEAD
=======
#include "dfs_cache.h"
#include "fs_context.h"
>>>>>>> upstream/android-13

static LIST_HEAD(cifs_dfs_automount_list);

static void cifs_dfs_expire_automounts(struct work_struct *work);
static DECLARE_DELAYED_WORK(cifs_dfs_automount_task,
			    cifs_dfs_expire_automounts);
static int cifs_dfs_mountpoint_expiry_timeout = 500 * HZ;

static void cifs_dfs_expire_automounts(struct work_struct *work)
{
	struct list_head *list = &cifs_dfs_automount_list;

	mark_mounts_for_expiry(list);
	if (!list_empty(list))
		schedule_delayed_work(&cifs_dfs_automount_task,
				      cifs_dfs_mountpoint_expiry_timeout);
}

void cifs_dfs_release_automount_timer(void)
{
	BUG_ON(!list_empty(&cifs_dfs_automount_list));
	cancel_delayed_work_sync(&cifs_dfs_automount_task);
}

/**
 * cifs_build_devname - build a devicename from a UNC and optional prepath
 * @nodename:	pointer to UNC string
 * @prepath:	pointer to prefixpath (or NULL if there isn't one)
 *
 * Build a new cifs devicename after chasing a DFS referral. Allocate a buffer
 * big enough to hold the final thing. Copy the UNC from the nodename, and
 * concatenate the prepath onto the end of it if there is one.
 *
 * Returns pointer to the built string, or a ERR_PTR. Caller is responsible
 * for freeing the returned string.
 */
static char *
cifs_build_devname(char *nodename, const char *prepath)
{
	size_t pplen;
	size_t unclen;
	char *dev;
	char *pos;

	/* skip over any preceding delimiters */
	nodename += strspn(nodename, "\\");
	if (!*nodename)
		return ERR_PTR(-EINVAL);

	/* get length of UNC and set pos to last char */
	unclen = strlen(nodename);
	pos = nodename + unclen - 1;

	/* trim off any trailing delimiters */
	while (*pos == '\\') {
		--pos;
		--unclen;
	}

	/* allocate a buffer:
	 * +2 for preceding "//"
	 * +1 for delimiter between UNC and prepath
	 * +1 for trailing NULL
	 */
	pplen = prepath ? strlen(prepath) : 0;
	dev = kmalloc(2 + unclen + 1 + pplen + 1, GFP_KERNEL);
	if (!dev)
		return ERR_PTR(-ENOMEM);

	pos = dev;
	/* add the initial "//" */
	*pos = '/';
	++pos;
	*pos = '/';
	++pos;

	/* copy in the UNC portion from referral */
	memcpy(pos, nodename, unclen);
	pos += unclen;

	/* copy the prefixpath remainder (if there is one) */
	if (pplen) {
		*pos = '/';
		++pos;
		memcpy(pos, prepath, pplen);
		pos += pplen;
	}

	/* NULL terminator */
	*pos = '\0';

	convert_delimiter(dev, '/');
	return dev;
}


/**
<<<<<<< HEAD
 * cifs_compose_mount_options	-	creates mount options for refferral
 * @sb_mountdata:	parent/root DFS mount options (template)
 * @fullpath:		full path in UNC format
 * @ref:		server's referral
 * @devname:		pointer for saving device name
 *
=======
 * cifs_compose_mount_options	-	creates mount options for referral
 * @sb_mountdata:	parent/root DFS mount options (template)
 * @fullpath:		full path in UNC format
 * @ref:		optional server's referral
 * @devname:		return the built cifs device name if passed pointer not NULL
>>>>>>> upstream/android-13
 * creates mount options for submount based on template options sb_mountdata
 * and replacing unc,ip,prefixpath options with ones we've got form ref_unc.
 *
 * Returns: pointer to new mount options or ERR_PTR.
<<<<<<< HEAD
 * Caller is responcible for freeing retunrned value if it is not error.
 */
char *cifs_compose_mount_options(const char *sb_mountdata,
				   const char *fullpath,
				   const struct dfs_info3_param *ref,
				   char **devname)
{
	int rc;
=======
 * Caller is responsible for freeing returned value if it is not error.
 */
char *cifs_compose_mount_options(const char *sb_mountdata,
				 const char *fullpath,
				 const struct dfs_info3_param *ref,
				 char **devname)
{
	int rc;
	char *name;
>>>>>>> upstream/android-13
	char *mountdata = NULL;
	const char *prepath = NULL;
	int md_len;
	char *tkn_e;
	char *srvIP = NULL;
	char sep = ',';
	int off, noff;

	if (sb_mountdata == NULL)
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	if (strlen(fullpath) - ref->path_consumed) {
		prepath = fullpath + ref->path_consumed;
		/* skip initial delimiter */
		if (*prepath == '/' || *prepath == '\\')
			prepath++;
	}

	*devname = cifs_build_devname(ref->node_name, prepath);
	if (IS_ERR(*devname)) {
		rc = PTR_ERR(*devname);
		*devname = NULL;
		goto compose_mount_options_err;
	}

	rc = dns_resolve_server_name_to_ip(*devname, &srvIP);
	if (rc < 0) {
		cifs_dbg(FYI, "%s: Failed to resolve server part of %s to IP: %d\n",
			 __func__, *devname, rc);
=======
	if (ref) {
		if (WARN_ON_ONCE(!ref->node_name || ref->path_consumed < 0))
			return ERR_PTR(-EINVAL);

		if (strlen(fullpath) - ref->path_consumed) {
			prepath = fullpath + ref->path_consumed;
			/* skip initial delimiter */
			if (*prepath == '/' || *prepath == '\\')
				prepath++;
		}

		name = cifs_build_devname(ref->node_name, prepath);
		if (IS_ERR(name)) {
			rc = PTR_ERR(name);
			name = NULL;
			goto compose_mount_options_err;
		}
	} else {
		name = cifs_build_devname((char *)fullpath, NULL);
		if (IS_ERR(name)) {
			rc = PTR_ERR(name);
			name = NULL;
			goto compose_mount_options_err;
		}
	}

	rc = dns_resolve_server_name_to_ip(name, &srvIP, NULL);
	if (rc < 0) {
		cifs_dbg(FYI, "%s: Failed to resolve server part of %s to IP: %d\n",
			 __func__, name, rc);
>>>>>>> upstream/android-13
		goto compose_mount_options_err;
	}

	/*
	 * In most cases, we'll be building a shorter string than the original,
	 * but we do have to assume that the address in the ip= option may be
	 * much longer than the original. Add the max length of an address
	 * string to the length of the original string to allow for worst case.
	 */
	md_len = strlen(sb_mountdata) + INET6_ADDRSTRLEN;
	mountdata = kzalloc(md_len + sizeof("ip=") + 1, GFP_KERNEL);
	if (mountdata == NULL) {
		rc = -ENOMEM;
		goto compose_mount_options_err;
	}

	/* copy all options except of unc,ip,prefixpath */
	off = 0;
	if (strncmp(sb_mountdata, "sep=", 4) == 0) {
			sep = sb_mountdata[4];
			strncpy(mountdata, sb_mountdata, 5);
			off += 5;
	}

	do {
		tkn_e = strchr(sb_mountdata + off, sep);
		if (tkn_e == NULL)
			noff = strlen(sb_mountdata + off);
		else
			noff = tkn_e - (sb_mountdata + off) + 1;

<<<<<<< HEAD
=======
		if (strncasecmp(sb_mountdata + off, "cruid=", 6) == 0) {
			off += noff;
			continue;
		}
>>>>>>> upstream/android-13
		if (strncasecmp(sb_mountdata + off, "unc=", 4) == 0) {
			off += noff;
			continue;
		}
		if (strncasecmp(sb_mountdata + off, "ip=", 3) == 0) {
			off += noff;
			continue;
		}
		if (strncasecmp(sb_mountdata + off, "prefixpath=", 11) == 0) {
			off += noff;
			continue;
		}
		strncat(mountdata, sb_mountdata + off, noff);
		off += noff;
	} while (tkn_e);
	strcat(mountdata, sb_mountdata + off);
	mountdata[md_len] = '\0';

	/* copy new IP and ref share name */
	if (mountdata[strlen(mountdata) - 1] != sep)
		strncat(mountdata, &sep, 1);
	strcat(mountdata, "ip=");
	strcat(mountdata, srvIP);

<<<<<<< HEAD
=======
	if (devname)
		*devname = name;
	else
		kfree(name);

>>>>>>> upstream/android-13
	/*cifs_dbg(FYI, "%s: parent mountdata: %s\n", __func__, sb_mountdata);*/
	/*cifs_dbg(FYI, "%s: submount mountdata: %s\n", __func__, mountdata );*/

compose_mount_options_out:
	kfree(srvIP);
	return mountdata;

compose_mount_options_err:
	kfree(mountdata);
	mountdata = ERR_PTR(rc);
<<<<<<< HEAD
	kfree(*devname);
	*devname = NULL;
=======
	kfree(name);
>>>>>>> upstream/android-13
	goto compose_mount_options_out;
}

/**
<<<<<<< HEAD
 * cifs_dfs_do_refmount - mounts specified path using provided refferal
 * @cifs_sb:		parent/root superblock
 * @fullpath:		full path in UNC format
 * @ref:		server's referral
 */
static struct vfsmount *cifs_dfs_do_refmount(struct dentry *mntpt,
		struct cifs_sb_info *cifs_sb,
		const char *fullpath, const struct dfs_info3_param *ref)
{
	struct vfsmount *mnt;
	char *mountdata;
	char *devname = NULL;

	/* strip first '\' from fullpath */
	mountdata = cifs_compose_mount_options(cifs_sb->mountdata,
			fullpath + 1, ref, &devname);

	if (IS_ERR(mountdata))
		return (struct vfsmount *)mountdata;
=======
 * cifs_dfs_do_mount - mounts specified path using DFS full path
 *
 * Always pass down @fullpath to smb3_do_mount() so we can use the root server
 * to perform failover in case we failed to connect to the first target in the
 * referral.
 *
 * @mntpt:		directory entry for the path we are trying to automount
 * @cifs_sb:		parent/root superblock
 * @fullpath:		full path in UNC format
 */
static struct vfsmount *cifs_dfs_do_mount(struct dentry *mntpt,
					  struct cifs_sb_info *cifs_sb,
					  const char *fullpath)
{
	struct vfsmount *mnt;
	char *mountdata;
	char *devname;

	devname = kstrdup(fullpath, GFP_KERNEL);
	if (!devname)
		return ERR_PTR(-ENOMEM);

	convert_delimiter(devname, '/');

	/* TODO: change to call fs_context_for_mount(), fill in context directly, call fc_mount */

	/* See afs_mntpt_do_automount in fs/afs/mntpt.c for an example */

	/* strip first '\' from fullpath */
	mountdata = cifs_compose_mount_options(cifs_sb->ctx->mount_options,
					       fullpath + 1, NULL, NULL);
	if (IS_ERR(mountdata)) {
		kfree(devname);
		return (struct vfsmount *)mountdata;
	}
>>>>>>> upstream/android-13

	mnt = vfs_submount(mntpt, &cifs_fs_type, devname, mountdata);
	kfree(mountdata);
	kfree(devname);
	return mnt;
<<<<<<< HEAD

}

static void dump_referral(const struct dfs_info3_param *ref)
{
	cifs_dbg(FYI, "DFS: ref path: %s\n", ref->path_name);
	cifs_dbg(FYI, "DFS: node path: %s\n", ref->node_name);
	cifs_dbg(FYI, "DFS: fl: %d, srv_type: %d\n",
		 ref->flags, ref->server_type);
	cifs_dbg(FYI, "DFS: ref_flags: %d, path_consumed: %d\n",
		 ref->ref_flag, ref->path_consumed);
=======
>>>>>>> upstream/android-13
}

/*
 * Create a vfsmount that we can automount
 */
static struct vfsmount *cifs_dfs_do_automount(struct dentry *mntpt)
{
<<<<<<< HEAD
	struct dfs_info3_param *referrals = NULL;
	unsigned int num_referrals = 0;
	struct cifs_sb_info *cifs_sb;
	struct cifs_ses *ses;
	char *full_path;
	unsigned int xid;
	int i;
	int rc;
	struct vfsmount *mnt;
	struct tcon_link *tlink;
=======
	struct cifs_sb_info *cifs_sb;
	struct cifs_ses *ses;
	struct cifs_tcon *tcon;
	void *page;
	char *full_path, *root_path;
	unsigned int xid;
	int rc;
	struct vfsmount *mnt;
>>>>>>> upstream/android-13

	cifs_dbg(FYI, "in %s\n", __func__);
	BUG_ON(IS_ROOT(mntpt));

	/*
	 * The MSDFS spec states that paths in DFS referral requests and
	 * responses must be prefixed by a single '\' character instead of
	 * the double backslashes usually used in the UNC. This function
	 * gives us the latter, so we must adjust the result.
	 */
	mnt = ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	/* always use tree name prefix */
	full_path = build_path_from_dentry_optional_prefix(mntpt, true);
	if (full_path == NULL)
		goto cdda_exit;

	cifs_sb = CIFS_SB(mntpt->d_sb);
	tlink = cifs_sb_tlink(cifs_sb);
	if (IS_ERR(tlink)) {
		mnt = ERR_CAST(tlink);
		goto free_full_path;
	}
	ses = tlink_tcon(tlink)->ses;

	xid = get_xid();
	rc = get_dfs_path(xid, ses, full_path + 1, cifs_sb->local_nls,
		&num_referrals, &referrals,
		cifs_remap(cifs_sb));
	free_xid(xid);

	cifs_put_tlink(tlink);

	mnt = ERR_PTR(-ENOENT);
	for (i = 0; i < num_referrals; i++) {
		int len;
		dump_referral(referrals + i);
		/* connect to a node */
		len = strlen(referrals[i].node_name);
		if (len < 2) {
			cifs_dbg(VFS, "%s: Net Address path too short: %s\n",
				 __func__, referrals[i].node_name);
			mnt = ERR_PTR(-EINVAL);
			break;
		}
		mnt = cifs_dfs_do_refmount(mntpt, cifs_sb,
				full_path, referrals + i);
		cifs_dbg(FYI, "%s: cifs_dfs_do_refmount:%s , mnt:%p\n",
			 __func__, referrals[i].node_name, mnt);
		if (!IS_ERR(mnt))
			goto success;
	}

	/* no valid submounts were found; return error from get_dfs_path() by
	 * preference */
	if (rc != 0)
		mnt = ERR_PTR(rc);

success:
	free_dfs_info_array(referrals, num_referrals);
free_full_path:
	kfree(full_path);
=======
	cifs_sb = CIFS_SB(mntpt->d_sb);
	if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_DFS) {
		mnt = ERR_PTR(-EREMOTE);
		goto cdda_exit;
	}

	page = alloc_dentry_path();
	/* always use tree name prefix */
	full_path = build_path_from_dentry_optional_prefix(mntpt, page, true);
	if (IS_ERR(full_path)) {
		mnt = ERR_CAST(full_path);
		goto free_full_path;
	}

	convert_delimiter(full_path, '\\');

	cifs_dbg(FYI, "%s: full_path: %s\n", __func__, full_path);

	if (!cifs_sb_master_tlink(cifs_sb)) {
		cifs_dbg(FYI, "%s: master tlink is NULL\n", __func__);
		goto free_full_path;
	}

	tcon = cifs_sb_master_tcon(cifs_sb);
	if (!tcon) {
		cifs_dbg(FYI, "%s: master tcon is NULL\n", __func__);
		goto free_full_path;
	}

	root_path = kstrdup(tcon->treeName, GFP_KERNEL);
	if (!root_path) {
		mnt = ERR_PTR(-ENOMEM);
		goto free_full_path;
	}
	cifs_dbg(FYI, "%s: root path: %s\n", __func__, root_path);

	ses = tcon->ses;
	xid = get_xid();

	/*
	 * If DFS root has been expired, then unconditionally fetch it again to
	 * refresh DFS referral cache.
	 */
	rc = dfs_cache_find(xid, ses, cifs_sb->local_nls, cifs_remap(cifs_sb),
			    root_path + 1, NULL, NULL);
	if (!rc) {
		rc = dfs_cache_find(xid, ses, cifs_sb->local_nls,
				    cifs_remap(cifs_sb), full_path + 1,
				    NULL, NULL);
	}

	free_xid(xid);

	if (rc) {
		mnt = ERR_PTR(rc);
		goto free_root_path;
	}
	/*
	 * OK - we were able to get and cache a referral for @full_path.
	 *
	 * Now, pass it down to cifs_mount() and it will retry every available
	 * node server in case of failures - no need to do it here.
	 */
	mnt = cifs_dfs_do_mount(mntpt, cifs_sb, full_path);
	cifs_dbg(FYI, "%s: cifs_dfs_do_mount:%s , mnt:%p\n", __func__,
		 full_path + 1, mnt);

free_root_path:
	kfree(root_path);
free_full_path:
	free_dentry_path(page);
>>>>>>> upstream/android-13
cdda_exit:
	cifs_dbg(FYI, "leaving %s\n" , __func__);
	return mnt;
}

/*
 * Attempt to automount the referral
 */
struct vfsmount *cifs_dfs_d_automount(struct path *path)
{
	struct vfsmount *newmnt;

	cifs_dbg(FYI, "in %s\n", __func__);

	newmnt = cifs_dfs_do_automount(path->dentry);
	if (IS_ERR(newmnt)) {
		cifs_dbg(FYI, "leaving %s [automount failed]\n" , __func__);
		return newmnt;
	}

	mntget(newmnt); /* prevent immediate expiration */
	mnt_set_expiry(newmnt, &cifs_dfs_automount_list);
	schedule_delayed_work(&cifs_dfs_automount_task,
			      cifs_dfs_mountpoint_expiry_timeout);
	cifs_dbg(FYI, "leaving %s [ok]\n" , __func__);
	return newmnt;
}

const struct inode_operations cifs_dfs_referral_inode_operations = {
};
