// SPDX-License-Identifier: GPL-2.0
/*
 * Process version 3 NFSACL requests.
 *
 * Copyright (C) 2002-2003 Andreas Gruenbacher <agruen@suse.de>
 */

#include "nfsd.h"
/* FIXME: nfsacl.h is a broken header */
#include <linux/nfsacl.h>
#include <linux/gfp.h>
#include "cache.h"
#include "xdr3.h"
#include "vfs.h"

<<<<<<< HEAD
#define RETURN_STATUS(st)	{ resp->status = (st); return (st); }

=======
>>>>>>> upstream/android-13
/*
 * NULL call.
 */
static __be32
nfsd3_proc_null(struct svc_rqst *rqstp)
{
<<<<<<< HEAD
	return nfs_ok;
=======
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Get the Access and/or Default ACL of a file.
 */
static __be32 nfsd3_proc_getacl(struct svc_rqst *rqstp)
{
	struct nfsd3_getaclargs *argp = rqstp->rq_argp;
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct posix_acl *acl;
	struct inode *inode;
	svc_fh *fh;
<<<<<<< HEAD
	__be32 nfserr = 0;

	fh = fh_copy(&resp->fh, &argp->fh);
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (nfserr)
		RETURN_STATUS(nfserr);

	inode = d_inode(fh->fh_dentry);

	if (argp->mask & ~NFS_ACL_MASK)
		RETURN_STATUS(nfserr_inval);
=======

	fh = fh_copy(&resp->fh, &argp->fh);
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (resp->status != nfs_ok)
		goto out;

	inode = d_inode(fh->fh_dentry);

	if (argp->mask & ~NFS_ACL_MASK) {
		resp->status = nfserr_inval;
		goto out;
	}
>>>>>>> upstream/android-13
	resp->mask = argp->mask;

	if (resp->mask & (NFS_ACL|NFS_ACLCNT)) {
		acl = get_acl(inode, ACL_TYPE_ACCESS);
		if (acl == NULL) {
			/* Solaris returns the inode's minimum ACL. */
			acl = posix_acl_from_mode(inode->i_mode, GFP_KERNEL);
		}
		if (IS_ERR(acl)) {
<<<<<<< HEAD
			nfserr = nfserrno(PTR_ERR(acl));
=======
			resp->status = nfserrno(PTR_ERR(acl));
>>>>>>> upstream/android-13
			goto fail;
		}
		resp->acl_access = acl;
	}
	if (resp->mask & (NFS_DFACL|NFS_DFACLCNT)) {
		/* Check how Solaris handles requests for the Default ACL
		   of a non-directory! */
		acl = get_acl(inode, ACL_TYPE_DEFAULT);
		if (IS_ERR(acl)) {
<<<<<<< HEAD
			nfserr = nfserrno(PTR_ERR(acl));
=======
			resp->status = nfserrno(PTR_ERR(acl));
>>>>>>> upstream/android-13
			goto fail;
		}
		resp->acl_default = acl;
	}

	/* resp->acl_{access,default} are released in nfs3svc_release_getacl. */
<<<<<<< HEAD
	RETURN_STATUS(0);
=======
out:
	return rpc_success;
>>>>>>> upstream/android-13

fail:
	posix_acl_release(resp->acl_access);
	posix_acl_release(resp->acl_default);
<<<<<<< HEAD
	RETURN_STATUS(nfserr);
=======
	goto out;
>>>>>>> upstream/android-13
}

/*
 * Set the Access and/or Default ACL of a file.
 */
static __be32 nfsd3_proc_setacl(struct svc_rqst *rqstp)
{
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;
	struct inode *inode;
	svc_fh *fh;
<<<<<<< HEAD
	__be32 nfserr = 0;
	int error;

	fh = fh_copy(&resp->fh, &argp->fh);
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_SATTR);
	if (nfserr)
=======
	int error;

	fh = fh_copy(&resp->fh, &argp->fh);
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_SATTR);
	if (resp->status != nfs_ok)
>>>>>>> upstream/android-13
		goto out;

	inode = d_inode(fh->fh_dentry);

	error = fh_want_write(fh);
	if (error)
		goto out_errno;

	fh_lock(fh);

<<<<<<< HEAD
	error = set_posix_acl(inode, ACL_TYPE_ACCESS, argp->acl_access);
	if (error)
		goto out_drop_lock;
	error = set_posix_acl(inode, ACL_TYPE_DEFAULT, argp->acl_default);
=======
	error = set_posix_acl(&init_user_ns, inode, ACL_TYPE_ACCESS,
			      argp->acl_access);
	if (error)
		goto out_drop_lock;
	error = set_posix_acl(&init_user_ns, inode, ACL_TYPE_DEFAULT,
			      argp->acl_default);
>>>>>>> upstream/android-13

out_drop_lock:
	fh_unlock(fh);
	fh_drop_write(fh);
out_errno:
<<<<<<< HEAD
	nfserr = nfserrno(error);
=======
	resp->status = nfserrno(error);
>>>>>>> upstream/android-13
out:
	/* argp->acl_{access,default} may have been allocated in
	   nfs3svc_decode_setaclargs. */
	posix_acl_release(argp->acl_access);
	posix_acl_release(argp->acl_default);
<<<<<<< HEAD
	RETURN_STATUS(nfserr);
=======
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * XDR decode functions
 */
<<<<<<< HEAD
static int nfs3svc_decode_getaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_getaclargs *args = rqstp->rq_argp;

	p = nfs3svc_decode_fh(p, &args->fh);
	if (!p)
		return 0;
	args->mask = ntohl(*p); p++;

	return xdr_argsize_check(rqstp, p);
}


static int nfs3svc_decode_setaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_setaclargs *args = rqstp->rq_argp;
	struct kvec *head = rqstp->rq_arg.head;
	unsigned int base;
	int n;

	p = nfs3svc_decode_fh(p, &args->fh);
	if (!p)
		return 0;
	args->mask = ntohl(*p++);
	if (args->mask & ~NFS_ACL_MASK ||
	    !xdr_argsize_check(rqstp, p))
		return 0;

	base = (char *)p - (char *)head->iov_base;
	n = nfsacl_decode(&rqstp->rq_arg, base, NULL,
			  (args->mask & NFS_ACL) ?
			  &args->acl_access : NULL);
	if (n > 0)
		n = nfsacl_decode(&rqstp->rq_arg, base + n, NULL,
				  (args->mask & NFS_DFACL) ?
				  &args->acl_default : NULL);
	return (n > 0);
=======

static int nfs3svc_decode_getaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_getaclargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_nfs_fh3(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->mask) < 0)
		return 0;

	return 1;
}

static int nfs3svc_decode_setaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;

	if (!svcxdr_decode_nfs_fh3(xdr, &argp->fh))
		return 0;
	if (xdr_stream_decode_u32(xdr, &argp->mask) < 0)
		return 0;
	if (argp->mask & ~NFS_ACL_MASK)
		return 0;
	if (!nfs_stream_decode_acl(xdr, NULL, (argp->mask & NFS_ACL) ?
				   &argp->acl_access : NULL))
		return 0;
	if (!nfs_stream_decode_acl(xdr, NULL, (argp->mask & NFS_DFACL) ?
				   &argp->acl_default : NULL))
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

/*
 * XDR encode functions
 */

/* GETACL */
static int nfs3svc_encode_getaclres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct dentry *dentry = resp->fh.fh_dentry;

	p = nfs3svc_encode_post_op_attr(rqstp, p, &resp->fh);
	if (resp->status == 0 && dentry && d_really_is_positive(dentry)) {
		struct inode *inode = d_inode(dentry);
		struct kvec *head = rqstp->rq_res.head;
		unsigned int base;
		int n;
		int w;

		*p++ = htonl(resp->mask);
		if (!xdr_ressize_check(rqstp, p))
			return 0;
		base = (char *)p - (char *)head->iov_base;
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct dentry *dentry = resp->fh.fh_dentry;
	struct kvec *head = rqstp->rq_res.head;
	struct inode *inode;
	unsigned int base;
	int n;
	int w;

	if (!svcxdr_encode_nfsstat3(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		inode = d_inode(dentry);
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->mask) < 0)
			return 0;

		base = (char *)xdr->p - (char *)head->iov_base;
>>>>>>> upstream/android-13

		rqstp->rq_res.page_len = w = nfsacl_size(
			(resp->mask & NFS_ACL)   ? resp->acl_access  : NULL,
			(resp->mask & NFS_DFACL) ? resp->acl_default : NULL);
		while (w > 0) {
			if (!*(rqstp->rq_next_page++))
				return 0;
			w -= PAGE_SIZE;
		}

		n = nfsacl_encode(&rqstp->rq_res, base, inode,
				  resp->acl_access,
				  resp->mask & NFS_ACL, 0);
		if (n > 0)
			n = nfsacl_encode(&rqstp->rq_res, base + n, inode,
					  resp->acl_default,
					  resp->mask & NFS_DFACL,
					  NFS_ACL_DEFAULT);
		if (n <= 0)
			return 0;
<<<<<<< HEAD
	} else
		if (!xdr_ressize_check(rqstp, p))
			return 0;
=======
		break;
	default:
		if (!svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh))
			return 0;
	}
>>>>>>> upstream/android-13

	return 1;
}

/* SETACL */
static int nfs3svc_encode_setaclres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	p = nfs3svc_encode_post_op_attr(rqstp, p, &resp->fh);

	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;

	return svcxdr_encode_nfsstat3(xdr, resp->status) &&
		svcxdr_encode_post_op_attr(rqstp, xdr, &resp->fh);
>>>>>>> upstream/android-13
}

/*
 * XDR release functions
 */
static void nfs3svc_release_getacl(struct svc_rqst *rqstp)
{
	struct nfsd3_getaclres *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
	posix_acl_release(resp->acl_access);
	posix_acl_release(resp->acl_default);
}

<<<<<<< HEAD
#define nfs3svc_decode_voidargs		NULL
#define nfs3svc_release_void		NULL
#define nfsd3_setaclres			nfsd3_attrstat
#define nfsd3_voidres			nfsd3_voidargs
struct nfsd3_voidargs { int dummy; };

#define PROC(name, argt, rest, relt, cache, respsize)			\
{									\
	.pc_func	= nfsd3_proc_##name,				\
	.pc_decode	= nfs3svc_decode_##argt##args,			\
	.pc_encode	= nfs3svc_encode_##rest##res,			\
	.pc_release	= nfs3svc_release_##relt,			\
	.pc_argsize	= sizeof(struct nfsd3_##argt##args),		\
	.pc_ressize	= sizeof(struct nfsd3_##rest##res),		\
	.pc_cachetype	= cache,					\
	.pc_xdrressize	= respsize,					\
}

=======
struct nfsd3_voidargs { int dummy; };

>>>>>>> upstream/android-13
#define ST 1		/* status*/
#define AT 21		/* attributes */
#define pAT (1+AT)	/* post attributes - conditional */
#define ACL (1+NFS_ACL_MAX_ENTRIES*3)  /* Access Control List */

<<<<<<< HEAD
static const struct svc_procedure nfsd_acl_procedures3[] = {
  PROC(null,	void,		void,		void,	  RC_NOCACHE, ST),
  PROC(getacl,	getacl,		getacl,		getacl,	  RC_NOCACHE, ST+1+2*(1+ACL)),
  PROC(setacl,	setacl,		setacl,		fhandle,  RC_NOCACHE, ST+pAT),
=======
static const struct svc_procedure nfsd_acl_procedures3[3] = {
	[ACLPROC3_NULL] = {
		.pc_func = nfsd3_proc_null,
		.pc_decode = nfssvc_decode_voidarg,
		.pc_encode = nfssvc_encode_voidres,
		.pc_argsize = sizeof(struct nfsd_voidargs),
		.pc_ressize = sizeof(struct nfsd_voidres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST,
		.pc_name = "NULL",
	},
	[ACLPROC3_GETACL] = {
		.pc_func = nfsd3_proc_getacl,
		.pc_decode = nfs3svc_decode_getaclargs,
		.pc_encode = nfs3svc_encode_getaclres,
		.pc_release = nfs3svc_release_getacl,
		.pc_argsize = sizeof(struct nfsd3_getaclargs),
		.pc_ressize = sizeof(struct nfsd3_getaclres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+1+2*(1+ACL),
		.pc_name = "GETACL",
	},
	[ACLPROC3_SETACL] = {
		.pc_func = nfsd3_proc_setacl,
		.pc_decode = nfs3svc_decode_setaclargs,
		.pc_encode = nfs3svc_encode_setaclres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_setaclargs),
		.pc_ressize = sizeof(struct nfsd3_attrstat),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT,
		.pc_name = "SETACL",
	},
>>>>>>> upstream/android-13
};

static unsigned int nfsd_acl_count3[ARRAY_SIZE(nfsd_acl_procedures3)];
const struct svc_version nfsd_acl_version3 = {
	.vs_vers	= 3,
	.vs_nproc	= 3,
	.vs_proc	= nfsd_acl_procedures3,
	.vs_count	= nfsd_acl_count3,
	.vs_dispatch	= nfsd_dispatch,
	.vs_xdrsize	= NFS3_SVC_XDRSIZE,
};

