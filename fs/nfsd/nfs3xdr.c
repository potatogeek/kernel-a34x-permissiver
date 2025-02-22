// SPDX-License-Identifier: GPL-2.0
/*
 * XDR support for nfsd/protocol version 3.
 *
 * Copyright (C) 1995, 1996, 1997 Olaf Kirch <okir@monad.swb.de>
 *
 * 2003-08-09 Jamie Lokier: Use htonl() for nanoseconds, not htons()!
 */

#include <linux/namei.h>
#include <linux/sunrpc/svc_xprt.h>
#include "xdr3.h"
#include "auth.h"
#include "netns.h"
#include "vfs.h"

<<<<<<< HEAD
#define NFSDDBG_FACILITY		NFSDDBG_XDR

=======
/*
 * Force construction of an empty post-op attr
 */
static const struct svc_fh nfs3svc_null_fh = {
	.fh_no_wcc	= true,
};

/*
 * time_delta. {1, 0} means the server is accurate only
 * to the nearest second.
 */
static const struct timespec64 nfs3svc_time_delta = {
	.tv_sec		= 1,
	.tv_nsec	= 0,
};
>>>>>>> upstream/android-13

/*
 * Mapping of S_IF* types to NFS file types
 */
<<<<<<< HEAD
static u32	nfs3_ftypes[] = {
=======
static const u32 nfs3_ftypes[] = {
>>>>>>> upstream/android-13
	NF3NON,  NF3FIFO, NF3CHR, NF3BAD,
	NF3DIR,  NF3BAD,  NF3BLK, NF3BAD,
	NF3REG,  NF3BAD,  NF3LNK, NF3BAD,
	NF3SOCK, NF3BAD,  NF3LNK, NF3BAD,
};

<<<<<<< HEAD
/*
 * XDR functions for basic NFS types
 */
static __be32 *
encode_time3(__be32 *p, struct timespec *time)
{
	*p++ = htonl((u32) time->tv_sec); *p++ = htonl(time->tv_nsec);
	return p;
}

static __be32 *
decode_time3(__be32 *p, struct timespec *time)
{
	time->tv_sec = ntohl(*p++);
	time->tv_nsec = ntohl(*p++);
	return p;
}

static __be32 *
decode_fh(__be32 *p, struct svc_fh *fhp)
{
	unsigned int size;
	fh_init(fhp, NFS3_FHSIZE);
	size = ntohl(*p++);
	if (size > NFS3_FHSIZE)
		return NULL;

	memcpy(&fhp->fh_handle.fh_base, p, size);
	fhp->fh_handle.fh_size = size;
	return p + XDR_QUADLEN(size);
}

/* Helper function for NFSv3 ACL code */
__be32 *nfs3svc_decode_fh(__be32 *p, struct svc_fh *fhp)
{
	return decode_fh(p, fhp);
}

static __be32 *
encode_fh(__be32 *p, struct svc_fh *fhp)
{
	unsigned int size = fhp->fh_handle.fh_size;
	*p++ = htonl(size);
	if (size) p[XDR_QUADLEN(size)-1]=0;
	memcpy(p, &fhp->fh_handle.fh_base, size);
	return p + XDR_QUADLEN(size);
}

/*
 * Decode a file name and make sure that the path contains
 * no slashes or null bytes.
 */
static __be32 *
decode_filename(__be32 *p, char **namp, unsigned int *lenp)
{
	char		*name;
	unsigned int	i;

	if ((p = xdr_decode_string_inplace(p, namp, lenp, NFS3_MAXNAMLEN)) != NULL) {
		for (i = 0, name = *namp; i < *lenp; i++, name++) {
			if (*name == '\0' || *name == '/')
				return NULL;
		}
	}

	return p;
}

static __be32 *
decode_sattr3(__be32 *p, struct iattr *iap)
{
	u32	tmp;

	iap->ia_valid = 0;

	if (*p++) {
		iap->ia_valid |= ATTR_MODE;
		iap->ia_mode = ntohl(*p++);
	}
	if (*p++) {
		iap->ia_uid = make_kuid(&init_user_ns, ntohl(*p++));
		if (uid_valid(iap->ia_uid))
			iap->ia_valid |= ATTR_UID;
	}
	if (*p++) {
		iap->ia_gid = make_kgid(&init_user_ns, ntohl(*p++));
		if (gid_valid(iap->ia_gid))
			iap->ia_valid |= ATTR_GID;
	}
	if (*p++) {
		u64	newsize;

		iap->ia_valid |= ATTR_SIZE;
		p = xdr_decode_hyper(p, &newsize);
		iap->ia_size = min_t(u64, newsize, NFS_OFFSET_MAX);
	}
	if ((tmp = ntohl(*p++)) == 1) {	/* set to server time */
		iap->ia_valid |= ATTR_ATIME;
	} else if (tmp == 2) {		/* set to client time */
		iap->ia_valid |= ATTR_ATIME | ATTR_ATIME_SET;
		iap->ia_atime.tv_sec = ntohl(*p++);
		iap->ia_atime.tv_nsec = ntohl(*p++);
	}
	if ((tmp = ntohl(*p++)) == 1) {	/* set to server time */
		iap->ia_valid |= ATTR_MTIME;
	} else if (tmp == 2) {		/* set to client time */
		iap->ia_valid |= ATTR_MTIME | ATTR_MTIME_SET;
		iap->ia_mtime.tv_sec = ntohl(*p++);
		iap->ia_mtime.tv_nsec = ntohl(*p++);
	}
	return p;
}

static __be32 *encode_fsid(__be32 *p, struct svc_fh *fhp)
{
	u64 f;
	switch(fsid_source(fhp)) {
	default:
	case FSIDSOURCE_DEV:
		p = xdr_encode_hyper(p, (u64)huge_encode_dev
				     (fhp->fh_dentry->d_sb->s_dev));
		break;
	case FSIDSOURCE_FSID:
		p = xdr_encode_hyper(p, (u64) fhp->fh_export->ex_fsid);
		break;
	case FSIDSOURCE_UUID:
		f = ((u64*)fhp->fh_export->ex_uuid)[0];
		f ^= ((u64*)fhp->fh_export->ex_uuid)[1];
		p = xdr_encode_hyper(p, f);
		break;
	}
	return p;
}

static __be32 *
encode_fattr3(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp,
	      struct kstat *stat)
{
	struct timespec ts;
	*p++ = htonl(nfs3_ftypes[(stat->mode & S_IFMT) >> 12]);
	*p++ = htonl((u32) (stat->mode & S_IALLUGO));
	*p++ = htonl((u32) stat->nlink);
	*p++ = htonl((u32) from_kuid(&init_user_ns, stat->uid));
	*p++ = htonl((u32) from_kgid(&init_user_ns, stat->gid));
	if (S_ISLNK(stat->mode) && stat->size > NFS3_MAXPATHLEN) {
		p = xdr_encode_hyper(p, (u64) NFS3_MAXPATHLEN);
	} else {
		p = xdr_encode_hyper(p, (u64) stat->size);
	}
	p = xdr_encode_hyper(p, ((u64)stat->blocks) << 9);
	*p++ = htonl((u32) MAJOR(stat->rdev));
	*p++ = htonl((u32) MINOR(stat->rdev));
	p = encode_fsid(p, fhp);
	p = xdr_encode_hyper(p, stat->ino);
	ts = timespec64_to_timespec(stat->atime);
	p = encode_time3(p, &ts);
	ts = timespec64_to_timespec(stat->mtime);
	p = encode_time3(p, &ts);
	ts = timespec64_to_timespec(stat->ctime);
	p = encode_time3(p, &ts);

	return p;
}

static __be32 *
encode_saved_post_attr(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp)
{
	/* Attributes to follow */
	*p++ = xdr_one;
	return encode_fattr3(rqstp, p, fhp, &fhp->fh_post_attr);
}

/*
 * Encode post-operation attributes.
 * The inode may be NULL if the call failed because of a stale file
 * handle. In this case, no attributes are returned.
 */
static __be32 *
encode_post_op_attr(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp)
{
	struct dentry *dentry = fhp->fh_dentry;
	if (dentry && d_really_is_positive(dentry)) {
	        __be32 err;
		struct kstat stat;

		err = fh_getattr(fhp, &stat);
		if (!err) {
			*p++ = xdr_one;		/* attributes follow */
			lease_get_mtime(d_inode(dentry), &stat.mtime);
			return encode_fattr3(rqstp, p, fhp, &stat);
		}
	}
	*p++ = xdr_zero;
	return p;
}

/* Helper for NFSv3 ACLs */
__be32 *
nfs3svc_encode_post_op_attr(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp)
{
	return encode_post_op_attr(rqstp, p, fhp);
}

/*
 * Enocde weak cache consistency data
 */
static __be32 *
encode_wcc_data(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp)
{
	struct dentry	*dentry = fhp->fh_dentry;

	if (dentry && d_really_is_positive(dentry) && fhp->fh_post_saved) {
		if (fhp->fh_pre_saved) {
			*p++ = xdr_one;
			p = xdr_encode_hyper(p, (u64) fhp->fh_pre_size);
			p = encode_time3(p, &fhp->fh_pre_mtime);
			p = encode_time3(p, &fhp->fh_pre_ctime);
		} else {
			*p++ = xdr_zero;
		}
		return encode_saved_post_attr(rqstp, p, fhp);
	}
	/* no pre- or post-attrs */
	*p++ = xdr_zero;
	return encode_post_op_attr(rqstp, p, fhp);
=======

/*
 * Basic NFSv3 data types (RFC 1813 Sections 2.5 and 2.6)
 */

static __be32 *
encode_nfstime3(__be32 *p, const struct timespec64 *time)
{
	*p++ = cpu_to_be32((u32)time->tv_sec);
	*p++ = cpu_to_be32(time->tv_nsec);

	return p;
}

static bool
svcxdr_decode_nfstime3(struct xdr_stream *xdr, struct timespec64 *timep)
{
	__be32 *p;

	p = xdr_inline_decode(xdr, XDR_UNIT * 2);
	if (!p)
		return false;
	timep->tv_sec = be32_to_cpup(p++);
	timep->tv_nsec = be32_to_cpup(p);

	return true;
}

/**
 * svcxdr_decode_nfs_fh3 - Decode an NFSv3 file handle
 * @xdr: XDR stream positioned at an undecoded NFSv3 FH
 * @fhp: OUT: filled-in server file handle
 *
 * Return values:
 *  %false: The encoded file handle was not valid
 *  %true: @fhp has been initialized
 */
bool
svcxdr_decode_nfs_fh3(struct xdr_stream *xdr, struct svc_fh *fhp)
{
	__be32 *p;
	u32 size;

	if (xdr_stream_decode_u32(xdr, &size) < 0)
		return false;
	if (size == 0 || size > NFS3_FHSIZE)
		return false;
	p = xdr_inline_decode(xdr, size);
	if (!p)
		return false;
	fh_init(fhp, NFS3_FHSIZE);
	fhp->fh_handle.fh_size = size;
	memcpy(&fhp->fh_handle.fh_base, p, size);

	return true;
}

/**
 * svcxdr_encode_nfsstat3 - Encode an NFSv3 status code
 * @xdr: XDR stream
 * @status: status value to encode
 *
 * Return values:
 *   %false: Send buffer space was exhausted
 *   %true: Success
 */
bool
svcxdr_encode_nfsstat3(struct xdr_stream *xdr, __be32 status)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, sizeof(status));
	if (!p)
		return false;
	*p = status;

	return true;
}

static bool
svcxdr_encode_nfs_fh3(struct xdr_stream *xdr, const struct svc_fh *fhp)
{
	u32 size = fhp->fh_handle.fh_size;
	__be32 *p;

	p = xdr_reserve_space(xdr, XDR_UNIT + size);
	if (!p)
		return false;
	*p++ = cpu_to_be32(size);
	if (size)
		p[XDR_QUADLEN(size) - 1] = 0;
	memcpy(p, &fhp->fh_handle.fh_base, size);

	return true;
}

static bool
svcxdr_encode_post_op_fh3(struct xdr_stream *xdr, const struct svc_fh *fhp)
{
	if (xdr_stream_encode_item_present(xdr) < 0)
		return false;
	if (!svcxdr_encode_nfs_fh3(xdr, fhp))
		return false;

	return true;
}

static bool
svcxdr_encode_cookieverf3(struct xdr_stream *xdr, const __be32 *verf)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, NFS3_COOKIEVERFSIZE);
	if (!p)
		return false;
	memcpy(p, verf, NFS3_COOKIEVERFSIZE);

	return true;
}

static bool
svcxdr_encode_writeverf3(struct xdr_stream *xdr, const __be32 *verf)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, NFS3_WRITEVERFSIZE);
	if (!p)
		return false;
	memcpy(p, verf, NFS3_WRITEVERFSIZE);

	return true;
}

static bool
svcxdr_decode_filename3(struct xdr_stream *xdr, char **name, unsigned int *len)
{
	u32 size, i;
	__be32 *p;
	char *c;

	if (xdr_stream_decode_u32(xdr, &size) < 0)
		return false;
	if (size == 0 || size > NFS3_MAXNAMLEN)
		return false;
	p = xdr_inline_decode(xdr, size);
	if (!p)
		return false;

	*len = size;
	*name = (char *)p;
	for (i = 0, c = *name; i < size; i++, c++) {
		if (*c == '\0' || *c == '/')
			return false;
	}

	return true;
}

static bool
svcxdr_decode_diropargs3(struct xdr_stream *xdr, struct svc_fh *fhp,
			 char **name, unsigned int *len)
{
	return svcxdr_decode_nfs_fh3(xdr, fhp) &&
		svcxdr_decode_filename3(xdr, name, len);
}

static bool
svcxdr_decode_sattr3(struct svc_rqst *rqstp, struct xdr_stream *xdr,
		     struct iattr *iap)
{
	u32 set_it;

	iap->ia_valid = 0;

	if (xdr_stream_decode_bool(xdr, &set_it) < 0)
		return false;
	if (set_it) {
		u32 mode;

		if (xdr_stream_decode_u32(xdr, &mode) < 0)
			return false;
		iap->ia_valid |= ATTR_MODE;
		iap->ia_mode = mode;
	}
	if (xdr_stream_decode_bool(xdr, &set_it) < 0)
		return false;
	if (set_it) {
		u32 uid;

		if (xdr_stream_decode_u32(xdr, &uid) < 0)
			return false;
		iap->ia_uid = make_kuid(nfsd_user_namespace(rqstp), uid);
		if (uid_valid(iap->ia_uid))
			iap->ia_valid |= ATTR_UID;
	}
	if (xdr_stream_decode_bool(xdr, &set_it) < 0)
		return false;
	if (set_it) {
		u32 gid;

		if (xdr_stream_decode_u32(xdr, &gid) < 0)
			return false;
		iap->ia_gid = make_kgid(nfsd_user_namespace(rqstp), gid);
		if (gid_valid(iap->ia_gid))
			iap->ia_valid |= ATTR_GID;
	}
	if (xdr_stream_decode_bool(xdr, &set_it) < 0)
		return false;
	if (set_it) {
		u64 newsize;

		if (xdr_stream_decode_u64(xdr, &newsize) < 0)
			return false;
		iap->ia_valid |= ATTR_SIZE;
		iap->ia_size = newsize;
	}
	if (xdr_stream_decode_u32(xdr, &set_it) < 0)
		return false;
	switch (set_it) {
	case DONT_CHANGE:
		break;
	case SET_TO_SERVER_TIME:
		iap->ia_valid |= ATTR_ATIME;
		break;
	case SET_TO_CLIENT_TIME:
		if (!svcxdr_decode_nfstime3(xdr, &iap->ia_atime))
			return false;
		iap->ia_valid |= ATTR_ATIME | ATTR_ATIME_SET;
		break;
	default:
		return false;
	}
	if (xdr_stream_decode_u32(xdr, &set_it) < 0)
		return false;
	switch (set_it) {
	case DONT_CHANGE:
		break;
	case SET_TO_SERVER_TIME:
		iap->ia_valid |= ATTR_MTIME;
		break;
	case SET_TO_CLIENT_TIME:
		if (!svcxdr_decode_nfstime3(xdr, &iap->ia_mtime))
			return false;
		iap->ia_valid |= ATTR_MTIME | ATTR_MTIME_SET;
		break;
	default:
		return false;
	}

	return true;
}

static bool
svcxdr_decode_sattrguard3(struct xdr_stream *xdr, struct nfsd3_sattrargs *args)
{
	__be32 *p;
	u32 check;

	if (xdr_stream_decode_bool(xdr, &check) < 0)
		return false;
	if (check) {
		p = xdr_inline_decode(xdr, XDR_UNIT * 2);
		if (!p)
			return false;
		args->check_guard = 1;
		args->guardtime = be32_to_cpup(p);
	} else
		args->check_guard = 0;

	return true;
}

static bool
svcxdr_decode_specdata3(struct xdr_stream *xdr, struct nfsd3_mknodargs *args)
{
	__be32 *p;

	p = xdr_inline_decode(xdr, XDR_UNIT * 2);
	if (!p)
		return false;
	args->major = be32_to_cpup(p++);
	args->minor = be32_to_cpup(p);

	return true;
}

static bool
svcxdr_decode_devicedata3(struct svc_rqst *rqstp, struct xdr_stream *xdr,
			  struct nfsd3_mknodargs *args)
{
	return svcxdr_decode_sattr3(rqstp, xdr, &args->attrs) &&
		svcxdr_decode_specdata3(xdr, args);
}

static bool
svcxdr_encode_fattr3(struct svc_rqst *rqstp, struct xdr_stream *xdr,
		     const struct svc_fh *fhp, const struct kstat *stat)
{
	struct user_namespace *userns = nfsd_user_namespace(rqstp);
	__be32 *p;
	u64 fsid;

	p = xdr_reserve_space(xdr, XDR_UNIT * 21);
	if (!p)
		return false;

	*p++ = cpu_to_be32(nfs3_ftypes[(stat->mode & S_IFMT) >> 12]);
	*p++ = cpu_to_be32((u32)(stat->mode & S_IALLUGO));
	*p++ = cpu_to_be32((u32)stat->nlink);
	*p++ = cpu_to_be32((u32)from_kuid_munged(userns, stat->uid));
	*p++ = cpu_to_be32((u32)from_kgid_munged(userns, stat->gid));
	if (S_ISLNK(stat->mode) && stat->size > NFS3_MAXPATHLEN)
		p = xdr_encode_hyper(p, (u64)NFS3_MAXPATHLEN);
	else
		p = xdr_encode_hyper(p, (u64)stat->size);

	/* used */
	p = xdr_encode_hyper(p, ((u64)stat->blocks) << 9);

	/* rdev */
	*p++ = cpu_to_be32((u32)MAJOR(stat->rdev));
	*p++ = cpu_to_be32((u32)MINOR(stat->rdev));

	switch(fsid_source(fhp)) {
	case FSIDSOURCE_FSID:
		fsid = (u64)fhp->fh_export->ex_fsid;
		break;
	case FSIDSOURCE_UUID:
		fsid = ((u64 *)fhp->fh_export->ex_uuid)[0];
		fsid ^= ((u64 *)fhp->fh_export->ex_uuid)[1];
		break;
	default:
		fsid = (u64)huge_encode_dev(fhp->fh_dentry->d_sb->s_dev);
	}
	p = xdr_encode_hyper(p, fsid);

	/* fileid */
	p = xdr_encode_hyper(p, stat->ino);

	p = encode_nfstime3(p, &stat->atime);
	p = encode_nfstime3(p, &stat->mtime);
	encode_nfstime3(p, &stat->ctime);

	return true;
}

static bool
svcxdr_encode_wcc_attr(struct xdr_stream *xdr, const struct svc_fh *fhp)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, XDR_UNIT * 6);
	if (!p)
		return false;
	p = xdr_encode_hyper(p, (u64)fhp->fh_pre_size);
	p = encode_nfstime3(p, &fhp->fh_pre_mtime);
	encode_nfstime3(p, &fhp->fh_pre_ctime);

	return true;
}

static bool
svcxdr_encode_pre_op_attr(struct xdr_stream *xdr, const struct svc_fh *fhp)
{
	if (!fhp->fh_pre_saved) {
		if (xdr_stream_encode_item_absent(xdr) < 0)
			return false;
		return true;
	}

	if (xdr_stream_encode_item_present(xdr) < 0)
		return false;
	return svcxdr_encode_wcc_attr(xdr, fhp);
}

/**
 * svcxdr_encode_post_op_attr - Encode NFSv3 post-op attributes
 * @rqstp: Context of a completed RPC transaction
 * @xdr: XDR stream
 * @fhp: File handle to encode
 *
 * Return values:
 *   %false: Send buffer space was exhausted
 *   %true: Success
 */
bool
svcxdr_encode_post_op_attr(struct svc_rqst *rqstp, struct xdr_stream *xdr,
			   const struct svc_fh *fhp)
{
	struct dentry *dentry = fhp->fh_dentry;
	struct kstat stat;

	/*
	 * The inode may be NULL if the call failed because of a
	 * stale file handle. In this case, no attributes are
	 * returned.
	 */
	if (fhp->fh_no_wcc || !dentry || !d_really_is_positive(dentry))
		goto no_post_op_attrs;
	if (fh_getattr(fhp, &stat) != nfs_ok)
		goto no_post_op_attrs;

	if (xdr_stream_encode_item_present(xdr) < 0)
		return false;
	lease_get_mtime(d_inode(dentry), &stat.mtime);
	if (!svcxdr_encode_fattr3(rqstp, xdr, fhp, &stat))
		return false;

	return true;

no_post_op_attrs:
	return xdr_stream_encode_item_absent(xdr) > 0;
}

/*
 * Encode weak cache consistency data
 */
static bool
svcxdr_encode_wcc_data(struct svc_rqst *rqstp, struct xdr_stream *xdr,
		       const struct svc_fh *fhp)
{
	struct dentry *dentry = fhp->fh_dentry;

	if (!dentry || !d_really_is_positive(dentry) || !fhp->fh_post_saved)
		goto neither;

	/* before */
	if (!svcxdr_encode_pre_op_attr(xdr, fhp))
		return false;

	/* after */
	if (xdr_stream_encode_item_present(xdr) < 0)
		return false;
	if (!svcxdr_encode_fattr3(rqstp, xdr, fhp, &fhp->fh_post_attr))
		return false;

	return true;

neither:
	if (xdr_stream_encode_item_absent(xdr) < 0)
		return false;
	if (!svcxdr_encode_post_op_attr(rqstp, xdr, fhp))
		return false;

	return true;
>>>>>>> upstream/android-13
}

/*
 * Fill in the pre_op attr for the wcc data
 */
void fill_pre_wcc(struct svc_fh *fhp)
{
	struct inode    *inode;
	struct kstat	stat;
<<<<<<< HEAD
	__be32 err;

	if (fhp->fh_pre_saved)
		return;

=======
	bool v4 = (fhp->fh_maxsize == NFS4_FHSIZE);
	__be32 err;

	if (fhp->fh_no_wcc || fhp->fh_pre_saved)
		return;
>>>>>>> upstream/android-13
	inode = d_inode(fhp->fh_dentry);
	err = fh_getattr(fhp, &stat);
	if (err) {
		/* Grab the times from inode anyway */
		stat.mtime = inode->i_mtime;
		stat.ctime = inode->i_ctime;
		stat.size  = inode->i_size;
	}
<<<<<<< HEAD

	fhp->fh_pre_mtime = timespec64_to_timespec(stat.mtime);
	fhp->fh_pre_ctime = timespec64_to_timespec(stat.ctime);
	fhp->fh_pre_size  = stat.size;
	fhp->fh_pre_change = nfsd4_change_attribute(&stat, inode);
=======
	if (v4)
		fhp->fh_pre_change = nfsd4_change_attribute(&stat, inode);

	fhp->fh_pre_mtime = stat.mtime;
	fhp->fh_pre_ctime = stat.ctime;
	fhp->fh_pre_size  = stat.size;
>>>>>>> upstream/android-13
	fhp->fh_pre_saved = true;
}

/*
 * Fill in the post_op attr for the wcc data
 */
void fill_post_wcc(struct svc_fh *fhp)
{
<<<<<<< HEAD
	__be32 err;

=======
	bool v4 = (fhp->fh_maxsize == NFS4_FHSIZE);
	struct inode *inode = d_inode(fhp->fh_dentry);
	__be32 err;

	if (fhp->fh_no_wcc)
		return;

>>>>>>> upstream/android-13
	if (fhp->fh_post_saved)
		printk("nfsd: inode locked twice during operation.\n");

	err = fh_getattr(fhp, &fhp->fh_post_attr);
<<<<<<< HEAD
	fhp->fh_post_change = nfsd4_change_attribute(&fhp->fh_post_attr,
						     d_inode(fhp->fh_dentry));
	if (err) {
		fhp->fh_post_saved = false;
		/* Grab the ctime anyway - set_change_info might use it */
		fhp->fh_post_attr.ctime = d_inode(fhp->fh_dentry)->i_ctime;
	} else
		fhp->fh_post_saved = true;
=======
	if (err) {
		fhp->fh_post_saved = false;
		fhp->fh_post_attr.ctime = inode->i_ctime;
	} else
		fhp->fh_post_saved = true;
	if (v4)
		fhp->fh_post_change =
			nfsd4_change_attribute(&fhp->fh_post_attr, inode);
>>>>>>> upstream/android-13
}

/*
 * XDR decode functions
 */
<<<<<<< HEAD
int
nfs3svc_decode_fhandle(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd_fhandle *args = rqstp->rq_argp;

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	return xdr_argsize_check(rqstp, p);
=======

int
nfs3svc_decode_fhandleargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd_fhandle *args = rqstp->rq_argp;

	return svcxdr_decode_nfs_fh3(xdr, &args->fh);
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_sattrargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_sattrargs *args = rqstp->rq_argp;

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = decode_sattr3(p, &args->attrs);

	if ((args->check_guard = ntohl(*p++)) != 0) { 
		struct timespec time; 
		p = decode_time3(p, &time);
		args->guardtime = time.tv_sec;
	}

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_sattrargs *args = rqstp->rq_argp;

	return svcxdr_decode_nfs_fh3(xdr, &args->fh) &&
		svcxdr_decode_sattr3(rqstp, xdr, &args->attrs) &&
		svcxdr_decode_sattrguard3(xdr, args);
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_diropargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_diropargs *args = rqstp->rq_argp;

	if (!(p = decode_fh(p, &args->fh))
	 || !(p = decode_filename(p, &args->name, &args->len)))
		return 0;

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_diropargs *args = rqstp->rq_argp;

	return svcxdr_decode_diropargs3(xdr, &args->fh, &args->name, &args->len);
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_accessargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_accessargs *args = rqstp->rq_argp;

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	args->access = ntohl(*p++);

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_accessargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->access) < 0)
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_readargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_readargs *args = rqstp->rq_argp;
	unsigned int len;
	int v;
	u32 max_blocksize = svc_max_payload(rqstp);

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = xdr_decode_hyper(p, &args->offset);

	args->count = ntohl(*p++);
	len = min(args->count, max_blocksize);

	/* set up the kvec */
	v=0;
	while (len > 0) {
		struct page *p = *(rqstp->rq_next_page++);

		rqstp->rq_vec[v].iov_base = page_address(p);
		rqstp->rq_vec[v].iov_len = min_t(unsigned int, len, PAGE_SIZE);
		len -= rqstp->rq_vec[v].iov_len;
		v++;
	}
	args->vlen = v;
	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_readargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u64(xdr, &args->offset) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->count) < 0)
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_writeargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_writeargs *args = rqstp->rq_argp;
	unsigned int len, hdr, dlen;
	u32 max_blocksize = svc_max_payload(rqstp);
	struct kvec *head = rqstp->rq_arg.head;
	struct kvec *tail = rqstp->rq_arg.tail;

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = xdr_decode_hyper(p, &args->offset);

	args->count = ntohl(*p++);
	args->stable = ntohl(*p++);
	len = args->len = ntohl(*p++);
	if ((void *)p > head->iov_base + head->iov_len)
		return 0;
	/*
	 * The count must equal the amount of data passed.
	 */
	if (args->count != args->len)
		return 0;

	/*
	 * Check to make sure that we got the right number of
	 * bytes.
	 */
	hdr = (void*)p - head->iov_base;
	dlen = head->iov_len + rqstp->rq_arg.page_len + tail->iov_len - hdr;
	/*
	 * Round the length of the data which was specified up to
	 * the next multiple of XDR units and then compare that
	 * against the length which was actually received.
	 * Note that when RPCSEC/GSS (for example) is used, the
	 * data buffer can be padded so dlen might be larger
	 * than required.  It must never be smaller.
	 */
	if (dlen < XDR_QUADLEN(len)*4)
		return 0;

	if (args->count > max_blocksize) {
		args->count = max_blocksize;
		len = args->len = max_blocksize;
	}

	args->first.iov_base = (void *)p;
	args->first.iov_len = head->iov_len - hdr;
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_writeargs *args = rqstp->rq_argp;
	u32 max_blocksize = svc_max_payload(rqstp);

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u64(xdr, &args->offset) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->count) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->stable) < 0)
		return 0;

	/* opaque data */
	if (xdr_stream_decode_u32(xdr, &args->len) < 0)
		return 0;

	/* request sanity */
	if (args->count != args->len)
		return 0;
	if (args->count > max_blocksize) {
		args->count = max_blocksize;
		args->len = max_blocksize;
	}
	if (!xdr_stream_subsegment(xdr, &args->payload, args->count))
		return 0;

>>>>>>> upstream/android-13
	return 1;
}

int
nfs3svc_decode_createargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_createargs *args = rqstp->rq_argp;

	if (!(p = decode_fh(p, &args->fh))
	 || !(p = decode_filename(p, &args->name, &args->len)))
		return 0;

	switch (args->createmode = ntohl(*p++)) {
	case NFS3_CREATE_UNCHECKED:
	case NFS3_CREATE_GUARDED:
		p = decode_sattr3(p, &args->attrs);
		break;
	case NFS3_CREATE_EXCLUSIVE:
		args->verf = p;
		p += 2;
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_createargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_diropargs3(xdr, &args->fh, &args->name, &args->len))
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->createmode) < 0)
		return 0;
	switch (args->createmode) {
	case NFS3_CREATE_UNCHECKED:
	case NFS3_CREATE_GUARDED:
		return svcxdr_decode_sattr3(rqstp, xdr, &args->attrs);
	case NFS3_CREATE_EXCLUSIVE:
		args->verf = xdr_inline_decode(xdr, NFS3_CREATEVERFSIZE);
		if (!args->verf)
			return 0;
>>>>>>> upstream/android-13
		break;
	default:
		return 0;
	}
<<<<<<< HEAD

	return xdr_argsize_check(rqstp, p);
=======
	return 1;
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_mkdirargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_createargs *args = rqstp->rq_argp;

	if (!(p = decode_fh(p, &args->fh)) ||
	    !(p = decode_filename(p, &args->name, &args->len)))
		return 0;
	p = decode_sattr3(p, &args->attrs);

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_createargs *args = rqstp->rq_argp;

	return svcxdr_decode_diropargs3(xdr, &args->fh,
					&args->name, &args->len) &&
		svcxdr_decode_sattr3(rqstp, xdr, &args->attrs);
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_symlinkargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_symlinkargs *args = rqstp->rq_argp;
	char *base = (char *)p;
	size_t dlen;

	if (!(p = decode_fh(p, &args->ffh)) ||
	    !(p = decode_filename(p, &args->fname, &args->flen)))
		return 0;
	p = decode_sattr3(p, &args->attrs);

	args->tlen = ntohl(*p++);

	args->first.iov_base = p;
	args->first.iov_len = rqstp->rq_arg.head[0].iov_len;
	args->first.iov_len -= (char *)p - base;

	dlen = args->first.iov_len + rqstp->rq_arg.page_len +
	       rqstp->rq_arg.tail[0].iov_len;
	if (dlen < XDR_QUADLEN(args->tlen) << 2)
		return 0;
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_symlinkargs *args = rqstp->rq_argp;
	struct kvec *head = rqstp->rq_arg.head;
	struct kvec *tail = rqstp->rq_arg.tail;
	size_t remaining;

	if (!svcxdr_decode_diropargs3(xdr, &args->ffh, &args->fname, &args->flen))
		return 0;
	if (!svcxdr_decode_sattr3(rqstp, xdr, &args->attrs))
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->tlen) < 0)
		return 0;

	/* request sanity */
	remaining = head->iov_len + rqstp->rq_arg.page_len + tail->iov_len;
	remaining -= xdr_stream_pos(xdr);
	if (remaining < xdr_align_size(args->tlen))
		return 0;

	args->first.iov_base = xdr->p;
	args->first.iov_len = head->iov_len - xdr_stream_pos(xdr);

>>>>>>> upstream/android-13
	return 1;
}

int
nfs3svc_decode_mknodargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_mknodargs *args = rqstp->rq_argp;

	if (!(p = decode_fh(p, &args->fh))
	 || !(p = decode_filename(p, &args->name, &args->len)))
		return 0;

	args->ftype = ntohl(*p++);

	if (args->ftype == NF3BLK  || args->ftype == NF3CHR
	 || args->ftype == NF3SOCK || args->ftype == NF3FIFO)
		p = decode_sattr3(p, &args->attrs);

	if (args->ftype == NF3BLK || args->ftype == NF3CHR) {
		args->major = ntohl(*p++);
		args->minor = ntohl(*p++);
	}

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_mknodargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_diropargs3(xdr, &args->fh, &args->name, &args->len))
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->ftype) < 0)
		return 0;
	switch (args->ftype) {
	case NF3CHR:
	case NF3BLK:
		return svcxdr_decode_devicedata3(rqstp, xdr, args);
	case NF3SOCK:
	case NF3FIFO:
		return svcxdr_decode_sattr3(rqstp, xdr, &args->attrs);
	case NF3REG:
	case NF3DIR:
	case NF3LNK:
		/* Valid XDR but illegal file types */
		break;
	default:
		return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_renameargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_renameargs *args = rqstp->rq_argp;

	if (!(p = decode_fh(p, &args->ffh))
	 || !(p = decode_filename(p, &args->fname, &args->flen))
	 || !(p = decode_fh(p, &args->tfh))
	 || !(p = decode_filename(p, &args->tname, &args->tlen)))
		return 0;

	return xdr_argsize_check(rqstp, p);
}

int
nfs3svc_decode_readlinkargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_readlinkargs *args = rqstp->rq_argp;

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	args->buffer = page_address(*(rqstp->rq_next_page++));

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_renameargs *args = rqstp->rq_argp;

	return svcxdr_decode_diropargs3(xdr, &args->ffh,
					&args->fname, &args->flen) &&
		svcxdr_decode_diropargs3(xdr, &args->tfh,
					 &args->tname, &args->tlen);
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_linkargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_linkargs *args = rqstp->rq_argp;

	if (!(p = decode_fh(p, &args->ffh))
	 || !(p = decode_fh(p, &args->tfh))
	 || !(p = decode_filename(p, &args->tname, &args->tlen)))
		return 0;

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_linkargs *args = rqstp->rq_argp;

	return svcxdr_decode_nfs_fh3(xdr, &args->ffh) &&
		svcxdr_decode_diropargs3(xdr, &args->tfh,
					 &args->tname, &args->tlen);
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_readdirargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_readdirargs *args = rqstp->rq_argp;
	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = xdr_decode_hyper(p, &args->cookie);
	args->verf   = p; p += 2;
	args->dircount = ~0;
	args->count  = ntohl(*p++);
	args->count  = min_t(u32, args->count, PAGE_SIZE);
	args->buffer = page_address(*(rqstp->rq_next_page++));

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_readdirargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u64(xdr, &args->cookie) < 0)
		return 0;
	args->verf = xdr_inline_decode(xdr, NFS3_COOKIEVERFSIZE);
	if (!args->verf)
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->count) < 0)
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_readdirplusargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_readdirargs *args = rqstp->rq_argp;
	int len;
	u32 max_blocksize = svc_max_payload(rqstp);

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = xdr_decode_hyper(p, &args->cookie);
	args->verf     = p; p += 2;
	args->dircount = ntohl(*p++);
	args->count    = ntohl(*p++);

	len = args->count = min(args->count, max_blocksize);
	while (len > 0) {
		struct page *p = *(rqstp->rq_next_page++);
		if (!args->buffer)
			args->buffer = page_address(p);
		len -= PAGE_SIZE;
	}

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_readdirargs *args = rqstp->rq_argp;
	u32 dircount;

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u64(xdr, &args->cookie) < 0)
		return 0;
	args->verf = xdr_inline_decode(xdr, NFS3_COOKIEVERFSIZE);
	if (!args->verf)
		return 0;
	/* dircount is ignored */
	if (xdr_stream_decode_u32(xdr, &dircount) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->count) < 0)
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

int
nfs3svc_decode_commitargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_commitargs *args = rqstp->rq_argp;
	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = xdr_decode_hyper(p, &args->offset);
	args->count = ntohl(*p++);

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_commitargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u64(xdr, &args->offset) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->count) < 0)
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

/*
 * XDR encode functions
 */
<<<<<<< HEAD
/*
 * There must be an encoding function for void results so svc_process
 * will work properly.
 */
int
nfs3svc_encode_voidres(struct svc_rqst *rqstp, __be32 *p)
{
	return xdr_ressize_check(rqstp, p);
}

/* GETATTR */
int
nfs3svc_encode_attrstat(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	if (resp->status == 0) {
		lease_get_mtime(d_inode(resp->fh.fh_dentry),
				&resp->stat.mtime);
		p = encode_fattr3(rqstp, p, &resp->fh, &resp->stat);
	}
	return xdr_ressize_check(rqstp, p);
=======

/* GETATTR */
int
nfs3svc_encode_getattrres(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		lease_get_mtime(d_inode(resp->fh.fh_dentry), &resp->stat.mtime);
		if (!svcxdr_encode_fattr3(rqstp, xdr, &resp->fh, &resp->stat))
			return 0;
		break;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* SETATTR, REMOVE, RMDIR */
int
nfs3svc_encode_wccstat(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	p = encode_wcc_data(rqstp, p, &resp->fh);
	return xdr_ressize_check(rqstp, p);
}

/* LOOKUP */
int
nfs3svc_encode_diropres(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_diropres *resp = rqstp->rq_resp;

	if (resp->status == 0) {
		p = encode_fh(p, &resp->fh);
		p = encode_post_op_attr(rqstp, p, &resp->fh);
	}
	p = encode_post_op_attr(rqstp, p, &resp->dirfh);
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	return svcxdr_encode_nfsstat3(xdr, resp->status) &&
		svcxdr_encode_wcc_data(rqstp, xdr, &resp->fh);
}

/* LOOKUP */
int nfs3svc_encode_lookupres(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_diropres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_nfs_fh3(xdr, &resp->fh))
			return 0;
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->dirfh))
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->dirfh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* ACCESS */
int
nfs3svc_encode_accessres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	p = encode_post_op_attr(rqstp, p, &resp->fh);
	if (resp->status == 0)
		*p++ = htonl(resp->access);
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->access) < 0)
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* READLINK */
int
nfs3svc_encode_readlinkres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_readlinkres *resp = rqstp->rq_resp;

	p = encode_post_op_attr(rqstp, p, &resp->fh);
	if (resp->status == 0) {
		*p++ = htonl(resp->len);
		xdr_ressize_check(rqstp, p);
		rqstp->rq_res.page_len = resp->len;
		if (resp->len & 3) {
			/* need to pad the tail */
			rqstp->rq_res.tail[0].iov_base = p;
			*p = 0;
			rqstp->rq_res.tail[0].iov_len = 4 - (resp->len&3);
		}
		return 1;
	} else
		return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_readlinkres *resp = rqstp->rq_resp;
	struct kvec *head = rqstp->rq_res.head;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->len) < 0)
			return 0;
		xdr_write_pages(xdr, resp->pages, 0, resp->len);
		if (svc_encode_result_payload(rqstp, head->iov_len, resp->len) < 0)
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* READ */
int
nfs3svc_encode_readres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_readres *resp = rqstp->rq_resp;

	p = encode_post_op_attr(rqstp, p, &resp->fh);
	if (resp->status == 0) {
		*p++ = htonl(resp->count);
		*p++ = htonl(resp->eof);
		*p++ = htonl(resp->count);	/* xdr opaque count */
		xdr_ressize_check(rqstp, p);
		/* now update rqstp->rq_res to reflect data as well */
		rqstp->rq_res.page_len = resp->count;
		if (resp->count & 3) {
			/* need to pad the tail */
			rqstp->rq_res.tail[0].iov_base = p;
			*p = 0;
			rqstp->rq_res.tail[0].iov_len = 4 - (resp->count & 3);
		}
		return 1;
	} else
		return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_readres *resp = rqstp->rq_resp;
	struct kvec *head = rqstp->rq_res.head;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->count) < 0)
			return 0;
		if (xdr_stream_encode_bool(xdr, resp->eof) < 0)
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->count) < 0)
			return 0;
		xdr_write_pages(xdr, resp->pages, rqstp->rq_res.page_base,
				resp->count);
		if (svc_encode_result_payload(rqstp, head->iov_len, resp->count) < 0)
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* WRITE */
int
nfs3svc_encode_writeres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_writeres *resp = rqstp->rq_resp;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	p = encode_wcc_data(rqstp, p, &resp->fh);
	if (resp->status == 0) {
		*p++ = htonl(resp->count);
		*p++ = htonl(resp->committed);
		/* unique identifier, y2038 overflow can be ignored */
		*p++ = htonl((u32)nn->nfssvc_boot.tv_sec);
		*p++ = htonl(nn->nfssvc_boot.tv_nsec);
	}
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_writeres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_wcc_data(rqstp, xdr, &resp->fh))
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->count) < 0)
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->committed) < 0)
			return 0;
		if (!svcxdr_encode_writeverf3(xdr, resp->verf))
			return 0;
		break;
	default:
		if (!svcxdr_encode_wcc_data(rqstp, xdr, &resp->fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* CREATE, MKDIR, SYMLINK, MKNOD */
int
nfs3svc_encode_createres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_diropres *resp = rqstp->rq_resp;

	if (resp->status == 0) {
		*p++ = xdr_one;
		p = encode_fh(p, &resp->fh);
		p = encode_post_op_attr(rqstp, p, &resp->fh);
	}
	p = encode_wcc_data(rqstp, p, &resp->dirfh);
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_diropres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_fh3(xdr, &resp->fh))
			return 0;
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (!svcxdr_encode_wcc_data(rqstp, xdr, &resp->dirfh))
			return 0;
		break;
	default:
		if (!svcxdr_encode_wcc_data(rqstp, xdr, &resp->dirfh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* RENAME */
int
nfs3svc_encode_renameres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_renameres *resp = rqstp->rq_resp;

	p = encode_wcc_data(rqstp, p, &resp->ffh);
	p = encode_wcc_data(rqstp, p, &resp->tfh);
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_renameres *resp = rqstp->rq_resp;

	return svcxdr_encode_nfsstat3(xdr, resp->status) &&
		svcxdr_encode_wcc_data(rqstp, xdr, &resp->ffh) &&
		svcxdr_encode_wcc_data(rqstp, xdr, &resp->tfh);
>>>>>>> upstream/android-13
}

/* LINK */
int
nfs3svc_encode_linkres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_linkres *resp = rqstp->rq_resp;

	p = encode_post_op_attr(rqstp, p, &resp->fh);
	p = encode_wcc_data(rqstp, p, &resp->tfh);
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_linkres *resp = rqstp->rq_resp;

	return svcxdr_encode_nfsstat3(xdr, resp->status) &&
		svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh) &&
		svcxdr_encode_wcc_data(rqstp, xdr, &resp->tfh);
>>>>>>> upstream/android-13
}

/* READDIR */
int
nfs3svc_encode_readdirres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_readdirres *resp = rqstp->rq_resp;

	p = encode_post_op_attr(rqstp, p, &resp->fh);

	if (resp->status == 0) {
		/* stupid readdir cookie */
		memcpy(p, resp->verf, 8); p += 2;
		xdr_ressize_check(rqstp, p);
		if (rqstp->rq_res.head[0].iov_len + (2<<2) > PAGE_SIZE)
			return 1; /*No room for trailer */
		rqstp->rq_res.page_len = (resp->count) << 2;

		/* add the 'tail' to the end of the 'head' page - page 0. */
		rqstp->rq_res.tail[0].iov_base = p;
		*p++ = 0;		/* no more entries */
		*p++ = htonl(resp->common.err == nfserr_eof);
		rqstp->rq_res.tail[0].iov_len = 2<<2;
		return 1;
	} else
		return xdr_ressize_check(rqstp, p);
}

static __be32 *
encode_entry_baggage(struct nfsd3_readdirres *cd, __be32 *p, const char *name,
	     int namlen, u64 ino)
{
	*p++ = xdr_one;				 /* mark entry present */
	p    = xdr_encode_hyper(p, ino);	 /* file id */
	p    = xdr_encode_array(p, name, namlen);/* name length & name */

	cd->offset = p;				/* remember pointer */
	p = xdr_encode_hyper(p, NFS_OFFSET_MAX);/* offset of next entry */

	return p;
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_readdirres *resp = rqstp->rq_resp;
	struct xdr_buf *dirlist = &resp->dirlist;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (!svcxdr_encode_cookieverf3(xdr, resp->verf))
			return 0;
		xdr_write_pages(xdr, dirlist->pages, 0, dirlist->len);
		/* no more entries */
		if (xdr_stream_encode_item_absent(xdr) < 0)
			return 0;
		if (xdr_stream_encode_bool(xdr, resp->common.err == nfserr_eof) < 0)
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

static __be32
compose_entry_fh(struct nfsd3_readdirres *cd, struct svc_fh *fhp,
		 const char *name, int namlen, u64 ino)
{
	struct svc_export	*exp;
	struct dentry		*dparent, *dchild;
	__be32 rv = nfserr_noent;

	dparent = cd->fh.fh_dentry;
	exp  = cd->fh.fh_export;

	if (isdotent(name, namlen)) {
		if (namlen == 2) {
			dchild = dget_parent(dparent);
			/*
			 * Don't return filehandle for ".." if we're at
			 * the filesystem or export root:
			 */
			if (dchild == dparent)
				goto out;
			if (dparent == exp->ex_path.dentry)
				goto out;
		} else
			dchild = dget(dparent);
	} else
<<<<<<< HEAD
		dchild = lookup_one_len_unlocked(name, dparent, namlen);
=======
		dchild = lookup_positive_unlocked(name, dparent, namlen);
>>>>>>> upstream/android-13
	if (IS_ERR(dchild))
		return rv;
	if (d_mountpoint(dchild))
		goto out;
<<<<<<< HEAD
	if (d_really_is_negative(dchild))
		goto out;
=======
>>>>>>> upstream/android-13
	if (dchild->d_inode->i_ino != ino)
		goto out;
	rv = fh_compose(fhp, exp, dchild, &cd->fh);
out:
	dput(dchild);
	return rv;
}

<<<<<<< HEAD
static __be32 *encode_entryplus_baggage(struct nfsd3_readdirres *cd, __be32 *p, const char *name, int namlen, u64 ino)
{
	struct svc_fh	*fh = &cd->scratch;
	__be32 err;

	fh_init(fh, NFS3_FHSIZE);
	err = compose_entry_fh(cd, fh, name, namlen, ino);
	if (err) {
		*p++ = 0;
		*p++ = 0;
		goto out;
	}
	p = encode_post_op_attr(cd->rqstp, p, fh);
	*p++ = xdr_one;			/* yes, a file handle follows */
	p = encode_fh(p, fh);
out:
	fh_put(fh);
	return p;
}

/*
 * Encode a directory entry. This one works for both normal readdir
 * and readdirplus.
 * The normal readdir reply requires 2 (fileid) + 1 (stringlen)
 * + string + 2 (cookie) + 1 (next) words, i.e. 6 + strlen.
 * 
 * The readdirplus baggage is 1+21 words for post_op_attr, plus the
 * file handle.
 */

#define NFS3_ENTRY_BAGGAGE	(2 + 1 + 2 + 1)
#define NFS3_ENTRYPLUS_BAGGAGE	(1 + 21 + 1 + (NFS3_FHSIZE >> 2))
static int
encode_entry(struct readdir_cd *ccd, const char *name, int namlen,
	     loff_t offset, u64 ino, unsigned int d_type, int plus)
{
	struct nfsd3_readdirres *cd = container_of(ccd, struct nfsd3_readdirres,
		       					common);
	__be32		*p = cd->buffer;
	caddr_t		curr_page_addr = NULL;
	struct page **	page;
	int		slen;		/* string (name) length */
	int		elen;		/* estimated entry length in words */
	int		num_entry_words = 0;	/* actual number of words */

	if (cd->offset) {
		u64 offset64 = offset;

		if (unlikely(cd->offset1)) {
			/* we ended up with offset on a page boundary */
			*cd->offset = htonl(offset64 >> 32);
			*cd->offset1 = htonl(offset64 & 0xffffffff);
			cd->offset1 = NULL;
		} else {
			xdr_encode_hyper(cd->offset, offset64);
		}
		cd->offset = NULL;
	}

	/*
	dprintk("encode_entry(%.*s @%ld%s)\n",
		namlen, name, (long) offset, plus? " plus" : "");
	 */

	/* truncate filename if too long */
	namlen = min(namlen, NFS3_MAXNAMLEN);

	slen = XDR_QUADLEN(namlen);
	elen = slen + NFS3_ENTRY_BAGGAGE
		+ (plus? NFS3_ENTRYPLUS_BAGGAGE : 0);

	if (cd->buflen < elen) {
		cd->common.err = nfserr_toosmall;
		return -EINVAL;
	}

	/* determine which page in rq_respages[] we are currently filling */
	for (page = cd->rqstp->rq_respages + 1;
				page < cd->rqstp->rq_next_page; page++) {
		curr_page_addr = page_address(*page);

		if (((caddr_t)cd->buffer >= curr_page_addr) &&
		    ((caddr_t)cd->buffer <  curr_page_addr + PAGE_SIZE))
			break;
	}

	if ((caddr_t)(cd->buffer + elen) < (curr_page_addr + PAGE_SIZE)) {
		/* encode entry in current page */

		p = encode_entry_baggage(cd, p, name, namlen, ino);

		if (plus)
			p = encode_entryplus_baggage(cd, p, name, namlen, ino);
		num_entry_words = p - cd->buffer;
	} else if (*(page+1) != NULL) {
		/* temporarily encode entry into next page, then move back to
		 * current and next page in rq_respages[] */
		__be32 *p1, *tmp;
		int len1, len2;

		/* grab next page for temporary storage of entry */
		p1 = tmp = page_address(*(page+1));

		p1 = encode_entry_baggage(cd, p1, name, namlen, ino);

		if (plus)
			p1 = encode_entryplus_baggage(cd, p1, name, namlen, ino);

		/* determine entry word length and lengths to go in pages */
		num_entry_words = p1 - tmp;
		len1 = curr_page_addr + PAGE_SIZE - (caddr_t)cd->buffer;
		if ((num_entry_words << 2) < len1) {
			/* the actual number of words in the entry is less
			 * than elen and can still fit in the current page
			 */
			memmove(p, tmp, num_entry_words << 2);
			p += num_entry_words;

			/* update offset */
			cd->offset = cd->buffer + (cd->offset - tmp);
		} else {
			unsigned int offset_r = (cd->offset - tmp) << 2;

			/* update pointer to offset location.
			 * This is a 64bit quantity, so we need to
			 * deal with 3 cases:
			 *  -	entirely in first page
			 *  -	entirely in second page
			 *  -	4 bytes in each page
			 */
			if (offset_r + 8 <= len1) {
				cd->offset = p + (cd->offset - tmp);
			} else if (offset_r >= len1) {
				cd->offset -= len1 >> 2;
			} else {
				/* sitting on the fence */
				BUG_ON(offset_r != len1 - 4);
				cd->offset = p + (cd->offset - tmp);
				cd->offset1 = tmp;
			}

			len2 = (num_entry_words << 2) - len1;

			/* move from temp page to current and next pages */
			memmove(p, tmp, len1);
			memmove(tmp, (caddr_t)tmp+len1, len2);

			p = tmp + (len2 >> 2);
		}
	}
	else {
		cd->common.err = nfserr_toosmall;
		return -EINVAL;
	}

	cd->buflen -= num_entry_words;
	cd->buffer = p;
	cd->common.err = nfs_ok;
	return 0;

}

int
nfs3svc_encode_entry(void *cd, const char *name,
		     int namlen, loff_t offset, u64 ino, unsigned int d_type)
{
	return encode_entry(cd, name, namlen, offset, ino, d_type, 0);
}

int
nfs3svc_encode_entry_plus(void *cd, const char *name,
			  int namlen, loff_t offset, u64 ino,
			  unsigned int d_type)
{
	return encode_entry(cd, name, namlen, offset, ino, d_type, 1);
=======
/**
 * nfs3svc_encode_cookie3 - Encode a directory offset cookie
 * @resp: readdir result context
 * @offset: offset cookie to encode
 *
 * The buffer space for the offset cookie has already been reserved
 * by svcxdr_encode_entry3_common().
 */
void nfs3svc_encode_cookie3(struct nfsd3_readdirres *resp, u64 offset)
{
	__be64 cookie = cpu_to_be64(offset);

	if (!resp->cookie_offset)
		return;
	write_bytes_to_xdr_buf(&resp->dirlist, resp->cookie_offset, &cookie,
			       sizeof(cookie));
	resp->cookie_offset = 0;
}

static bool
svcxdr_encode_entry3_common(struct nfsd3_readdirres *resp, const char *name,
			    int namlen, loff_t offset, u64 ino)
{
	struct xdr_buf *dirlist = &resp->dirlist;
	struct xdr_stream *xdr = &resp->xdr;

	if (xdr_stream_encode_item_present(xdr) < 0)
		return false;
	/* fileid */
	if (xdr_stream_encode_u64(xdr, ino) < 0)
		return false;
	/* name */
	if (xdr_stream_encode_opaque(xdr, name, min(namlen, NFS3_MAXNAMLEN)) < 0)
		return false;
	/* cookie */
	resp->cookie_offset = dirlist->len;
	if (xdr_stream_encode_u64(xdr, NFS_OFFSET_MAX) < 0)
		return false;

	return true;
}

/**
 * nfs3svc_encode_entry3 - encode one NFSv3 READDIR entry
 * @data: directory context
 * @name: name of the object to be encoded
 * @namlen: length of that name, in bytes
 * @offset: the offset of the previous entry
 * @ino: the fileid of this entry
 * @d_type: unused
 *
 * Return values:
 *   %0: Entry was successfully encoded.
 *   %-EINVAL: An encoding problem occured, secondary status code in resp->common.err
 *
 * On exit, the following fields are updated:
 *   - resp->xdr
 *   - resp->common.err
 *   - resp->cookie_offset
 */
int nfs3svc_encode_entry3(void *data, const char *name, int namlen,
			  loff_t offset, u64 ino, unsigned int d_type)
{
	struct readdir_cd *ccd = data;
	struct nfsd3_readdirres *resp = container_of(ccd,
						     struct nfsd3_readdirres,
						     common);
	unsigned int starting_length = resp->dirlist.len;

	/* The offset cookie for the previous entry */
	nfs3svc_encode_cookie3(resp, offset);

	if (!svcxdr_encode_entry3_common(resp, name, namlen, offset, ino))
		goto out_toosmall;

	xdr_commit_encode(&resp->xdr);
	resp->common.err = nfs_ok;
	return 0;

out_toosmall:
	resp->cookie_offset = 0;
	resp->common.err = nfserr_toosmall;
	resp->dirlist.len = starting_length;
	return -EINVAL;
}

static bool
svcxdr_encode_entry3_plus(struct nfsd3_readdirres *resp, const char *name,
			  int namlen, u64 ino)
{
	struct xdr_stream *xdr = &resp->xdr;
	struct svc_fh *fhp = &resp->scratch;
	bool result;

	result = false;
	fh_init(fhp, NFS3_FHSIZE);
	if (compose_entry_fh(resp, fhp, name, namlen, ino) != nfs_ok)
		goto out_noattrs;

	if (!svcxdr_encode_post_op_attr(resp->rqstp, xdr, fhp))
		goto out;
	if (!svcxdr_encode_post_op_fh3(xdr, fhp))
		goto out;
	result = true;

out:
	fh_put(fhp);
	return result;

out_noattrs:
	if (xdr_stream_encode_item_absent(xdr) < 0)
		return false;
	if (xdr_stream_encode_item_absent(xdr) < 0)
		return false;
	return true;
}

/**
 * nfs3svc_encode_entryplus3 - encode one NFSv3 READDIRPLUS entry
 * @data: directory context
 * @name: name of the object to be encoded
 * @namlen: length of that name, in bytes
 * @offset: the offset of the previous entry
 * @ino: the fileid of this entry
 * @d_type: unused
 *
 * Return values:
 *   %0: Entry was successfully encoded.
 *   %-EINVAL: An encoding problem occured, secondary status code in resp->common.err
 *
 * On exit, the following fields are updated:
 *   - resp->xdr
 *   - resp->common.err
 *   - resp->cookie_offset
 */
int nfs3svc_encode_entryplus3(void *data, const char *name, int namlen,
			      loff_t offset, u64 ino, unsigned int d_type)
{
	struct readdir_cd *ccd = data;
	struct nfsd3_readdirres *resp = container_of(ccd,
						     struct nfsd3_readdirres,
						     common);
	unsigned int starting_length = resp->dirlist.len;

	/* The offset cookie for the previous entry */
	nfs3svc_encode_cookie3(resp, offset);

	if (!svcxdr_encode_entry3_common(resp, name, namlen, offset, ino))
		goto out_toosmall;
	if (!svcxdr_encode_entry3_plus(resp, name, namlen, ino))
		goto out_toosmall;

	xdr_commit_encode(&resp->xdr);
	resp->common.err = nfs_ok;
	return 0;

out_toosmall:
	resp->cookie_offset = 0;
	resp->common.err = nfserr_toosmall;
	resp->dirlist.len = starting_length;
	return -EINVAL;
}

static bool
svcxdr_encode_fsstat3resok(struct xdr_stream *xdr,
			   const struct nfsd3_fsstatres *resp)
{
	const struct kstatfs *s = &resp->stats;
	u64 bs = s->f_bsize;
	__be32 *p;

	p = xdr_reserve_space(xdr, XDR_UNIT * 13);
	if (!p)
		return false;
	p = xdr_encode_hyper(p, bs * s->f_blocks);	/* total bytes */
	p = xdr_encode_hyper(p, bs * s->f_bfree);	/* free bytes */
	p = xdr_encode_hyper(p, bs * s->f_bavail);	/* user available bytes */
	p = xdr_encode_hyper(p, s->f_files);		/* total inodes */
	p = xdr_encode_hyper(p, s->f_ffree);		/* free inodes */
	p = xdr_encode_hyper(p, s->f_ffree);		/* user available inodes */
	*p = cpu_to_be32(resp->invarsec);		/* mean unchanged time */

	return true;
>>>>>>> upstream/android-13
}

/* FSSTAT */
int
nfs3svc_encode_fsstatres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_fsstatres *resp = rqstp->rq_resp;
	struct kstatfs	*s = &resp->stats;
	u64		bs = s->f_bsize;

	*p++ = xdr_zero;	/* no post_op_attr */

	if (resp->status == 0) {
		p = xdr_encode_hyper(p, bs * s->f_blocks);	/* total bytes */
		p = xdr_encode_hyper(p, bs * s->f_bfree);	/* free bytes */
		p = xdr_encode_hyper(p, bs * s->f_bavail);	/* user available bytes */
		p = xdr_encode_hyper(p, s->f_files);	/* total inodes */
		p = xdr_encode_hyper(p, s->f_ffree);	/* free inodes */
		p = xdr_encode_hyper(p, s->f_ffree);	/* user available inodes */
		*p++ = htonl(resp->invarsec);	/* mean unchanged time */
	}
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_fsstatres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &nfs3svc_null_fh))
			return 0;
		if (!svcxdr_encode_fsstat3resok(xdr, resp))
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &nfs3svc_null_fh))
			return 0;
	}

	return 1;
}

static bool
svcxdr_encode_fsinfo3resok(struct xdr_stream *xdr,
			   const struct nfsd3_fsinfores *resp)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, XDR_UNIT * 12);
	if (!p)
		return false;
	*p++ = cpu_to_be32(resp->f_rtmax);
	*p++ = cpu_to_be32(resp->f_rtpref);
	*p++ = cpu_to_be32(resp->f_rtmult);
	*p++ = cpu_to_be32(resp->f_wtmax);
	*p++ = cpu_to_be32(resp->f_wtpref);
	*p++ = cpu_to_be32(resp->f_wtmult);
	*p++ = cpu_to_be32(resp->f_dtpref);
	p = xdr_encode_hyper(p, resp->f_maxfilesize);
	p = encode_nfstime3(p, &nfs3svc_time_delta);
	*p = cpu_to_be32(resp->f_properties);

	return true;
>>>>>>> upstream/android-13
}

/* FSINFO */
int
nfs3svc_encode_fsinfores(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_fsinfores *resp = rqstp->rq_resp;

	*p++ = xdr_zero;	/* no post_op_attr */

	if (resp->status == 0) {
		*p++ = htonl(resp->f_rtmax);
		*p++ = htonl(resp->f_rtpref);
		*p++ = htonl(resp->f_rtmult);
		*p++ = htonl(resp->f_wtmax);
		*p++ = htonl(resp->f_wtpref);
		*p++ = htonl(resp->f_wtmult);
		*p++ = htonl(resp->f_dtpref);
		p = xdr_encode_hyper(p, resp->f_maxfilesize);
		*p++ = xdr_one;
		*p++ = xdr_zero;
		*p++ = htonl(resp->f_properties);
	}

	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_fsinfores *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &nfs3svc_null_fh))
			return 0;
		if (!svcxdr_encode_fsinfo3resok(xdr, resp))
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &nfs3svc_null_fh))
			return 0;
	}

	return 1;
}

static bool
svcxdr_encode_pathconf3resok(struct xdr_stream *xdr,
			     const struct nfsd3_pathconfres *resp)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, XDR_UNIT * 6);
	if (!p)
		return false;
	*p++ = cpu_to_be32(resp->p_link_max);
	*p++ = cpu_to_be32(resp->p_name_max);
	p = xdr_encode_bool(p, resp->p_no_trunc);
	p = xdr_encode_bool(p, resp->p_chown_restricted);
	p = xdr_encode_bool(p, resp->p_case_insensitive);
	xdr_encode_bool(p, resp->p_case_preserving);

	return true;
>>>>>>> upstream/android-13
}

/* PATHCONF */
int
nfs3svc_encode_pathconfres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_pathconfres *resp = rqstp->rq_resp;

	*p++ = xdr_zero;	/* no post_op_attr */

	if (resp->status == 0) {
		*p++ = htonl(resp->p_link_max);
		*p++ = htonl(resp->p_name_max);
		*p++ = htonl(resp->p_no_trunc);
		*p++ = htonl(resp->p_chown_restricted);
		*p++ = htonl(resp->p_case_insensitive);
		*p++ = htonl(resp->p_case_preserving);
	}

	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_pathconfres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &nfs3svc_null_fh))
			return 0;
		if (!svcxdr_encode_pathconf3resok(xdr, resp))
			return 0;
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &nfs3svc_null_fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/* COMMIT */
int
nfs3svc_encode_commitres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_commitres *resp = rqstp->rq_resp;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	p = encode_wcc_data(rqstp, p, &resp->fh);
	/* Write verifier */
	if (resp->status == 0) {
		/* unique identifier, y2038 overflow can be ignored */
		*p++ = htonl((u32)nn->nfssvc_boot.tv_sec);
		*p++ = htonl(nn->nfssvc_boot.tv_nsec);
	}
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_commitres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_wcc_data(rqstp, xdr, &resp->fh))
			return 0;
		if (!svcxdr_encode_writeverf3(xdr, resp->verf))
			return 0;
		break;
	default:
		if (!svcxdr_encode_wcc_data(rqstp, xdr, &resp->fh))
			return 0;
	}

	return 1;
>>>>>>> upstream/android-13
}

/*
 * XDR release functions
 */
void
nfs3svc_release_fhandle(struct svc_rqst *rqstp)
{
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
}

void
nfs3svc_release_fhandle2(struct svc_rqst *rqstp)
{
	struct nfsd3_fhandle_pair *resp = rqstp->rq_resp;

	fh_put(&resp->fh1);
	fh_put(&resp->fh2);
}
