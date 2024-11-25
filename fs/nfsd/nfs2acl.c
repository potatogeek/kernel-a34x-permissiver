// SPDX-License-Identifier: GPL-2.0
/*
 * Process version 2 NFSACL requests.
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

#define NFSDDBG_FACILITY		NFSDDBG_PROC
<<<<<<< HEAD
#define RETURN_STATUS(st)	{ resp->status = (st); return (st); }
=======
>>>>>>> upstream/android-13

/*
 * NULL call.
 */
static __be32
nfsacld_proc_null(struct svc_rqst *rqstp)
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
static __be32 nfsacld_proc_getacl(struct svc_rqst *rqstp)
{
	struct nfsd3_getaclargs *argp = rqstp->rq_argp;
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct posix_acl *acl;
	struct inode *inode;
	svc_fh *fh;
<<<<<<< HEAD
	__be32 nfserr = 0;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: GETACL(2acl)   %s\n", SVCFH_fmt(&argp->fh));

	fh = fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (nfserr)
		RETURN_STATUS(nfserr);

	inode = d_inode(fh->fh_dentry);

	if (argp->mask & ~NFS_ACL_MASK)
		RETURN_STATUS(nfserr_inval);
	resp->mask = argp->mask;

	nfserr = fh_getattr(fh, &resp->stat);
	if (nfserr)
		RETURN_STATUS(nfserr);
=======
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (resp->status != nfs_ok)
		goto out;

	inode = d_inode(fh->fh_dentry);

	if (argp->mask & ~NFS_ACL_MASK) {
		resp->status = nfserr_inval;
		goto out;
	}
	resp->mask = argp->mask;

	resp->status = fh_getattr(fh, &resp->stat);
	if (resp->status != nfs_ok)
		goto out;
>>>>>>> upstream/android-13

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

	/* resp->acl_{access,default} are released in nfssvc_release_getacl. */
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
static __be32 nfsacld_proc_setacl(struct svc_rqst *rqstp)
{
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;
	struct nfsd_attrstat *resp = rqstp->rq_resp;
	struct inode *inode;
	svc_fh *fh;
<<<<<<< HEAD
	__be32 nfserr = 0;
=======
>>>>>>> upstream/android-13
	int error;

	dprintk("nfsd: SETACL(2acl)   %s\n", SVCFH_fmt(&argp->fh));

	fh = fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_SATTR);
	if (nfserr)
=======
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
	if (error)
		goto out_drop_lock;

	fh_unlock(fh);

	fh_drop_write(fh);

<<<<<<< HEAD
	nfserr = fh_getattr(fh, &resp->stat);
=======
	resp->status = fh_getattr(fh, &resp->stat);
>>>>>>> upstream/android-13

out:
	/* argp->acl_{access,default} may have been allocated in
	   nfssvc_decode_setaclargs. */
	posix_acl_release(argp->acl_access);
	posix_acl_release(argp->acl_default);
<<<<<<< HEAD
	return nfserr;
=======
	return rpc_success;

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
	goto out;
}

/*
 * Check file attributes
 */
static __be32 nfsacld_proc_getattr(struct svc_rqst *rqstp)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd_attrstat *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32 nfserr;
	dprintk("nfsd: GETATTR  %s\n", SVCFH_fmt(&argp->fh));

	fh_copy(&resp->fh, &argp->fh);
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (nfserr)
		return nfserr;
	nfserr = fh_getattr(&resp->fh, &resp->stat);
	return nfserr;
=======

	dprintk("nfsd: GETATTR  %s\n", SVCFH_fmt(&argp->fh));

	fh_copy(&resp->fh, &argp->fh);
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (resp->status != nfs_ok)
		goto out;
	resp->status = fh_getattr(&resp->fh, &resp->stat);
out:
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Check file access
 */
static __be32 nfsacld_proc_access(struct svc_rqst *rqstp)
{
	struct nfsd3_accessargs *argp = rqstp->rq_argp;
	struct nfsd3_accessres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32 nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: ACCESS(2acl)   %s 0x%x\n",
			SVCFH_fmt(&argp->fh),
			argp->access);

	fh_copy(&resp->fh, &argp->fh);
	resp->access = argp->access;
<<<<<<< HEAD
	nfserr = nfsd_access(rqstp, &resp->fh, &resp->access, NULL);
	if (nfserr)
		return nfserr;
	nfserr = fh_getattr(&resp->fh, &resp->stat);
	return nfserr;
=======
	resp->status = nfsd_access(rqstp, &resp->fh, &resp->access, NULL);
	if (resp->status != nfs_ok)
		goto out;
	resp->status = fh_getattr(&resp->fh, &resp->stat);
out:
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * XDR decode functions
 */
<<<<<<< HEAD
static int nfsaclsvc_decode_getaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_getaclargs *argp = rqstp->rq_argp;

	p = nfs2svc_decode_fh(p, &argp->fh);
	if (!p)
		return 0;
	argp->mask = ntohl(*p); p++;

	return xdr_argsize_check(rqstp, p);
}


static int nfsaclsvc_decode_setaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;
	struct kvec *head = rqstp->rq_arg.head;
	unsigned int base;
	int n;

	p = nfs2svc_decode_fh(p, &argp->fh);
	if (!p)
		return 0;
	argp->mask = ntohl(*p++);
	if (argp->mask & ~NFS_ACL_MASK ||
	    !xdr_argsize_check(rqstp, p))
		return 0;

	base = (char *)p - (char *)head->iov_base;
	n = nfsacl_decode(&rqstp->rq_arg, base, NULL,
			  (argp->mask & NFS_ACL) ?
			  &argp->acl_access : NULL);
	if (n > 0)
		n = nfsacl_decode(&rqstp->rq_arg, base + n, NULL,
				  (argp->mask & NFS_DFACL) ?
				  &argp->acl_default : NULL);
	return (n > 0);
}

static int nfsaclsvc_decode_fhandleargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;

	p = nfs2svc_decode_fh(p, &argp->fh);
	if (!p)
		return 0;
	return xdr_argsize_check(rqstp, p);
=======

static int nfsaclsvc_decode_getaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_getaclargs *argp = rqstp->rq_argp;

	if (!svcxdr_decode_fhandle(xdr, &argp->fh))
		return 0;
	if (xdr_stream_decode_u32(xdr, &argp->mask) < 0)
		return 0;

	return 1;
}

static int nfsaclsvc_decode_setaclargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;

	if (!svcxdr_decode_fhandle(xdr, &argp->fh))
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

static int nfsaclsvc_decode_accessargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_accessargs *argp = rqstp->rq_argp;

	p = nfs2svc_decode_fh(p, &argp->fh);
	if (!p)
		return 0;
	argp->access = ntohl(*p++);

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nfsd3_accessargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_fhandle(xdr, &args->fh))
		return 0;
	if (xdr_stream_decode_u32(xdr, &args->access) < 0)
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
static int nfsaclsvc_encode_voidres(struct svc_rqst *rqstp, __be32 *p)
{
	return xdr_ressize_check(rqstp, p);
}

/* GETACL */
static int nfsaclsvc_encode_getaclres(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct dentry *dentry = resp->fh.fh_dentry;
	struct inode *inode;
	struct kvec *head = rqstp->rq_res.head;
	unsigned int base;
	int n;
	int w;

	/*
	 * Since this is version 2, the check for nfserr in
	 * nfsd_dispatch actually ensures the following cannot happen.
	 * However, it seems fragile to depend on that.
	 */
	if (dentry == NULL || d_really_is_negative(dentry))
		return 0;
	inode = d_inode(dentry);

	p = nfs2svc_encode_fattr(rqstp, p, &resp->fh, &resp->stat);
	*p++ = htonl(resp->mask);
	if (!xdr_ressize_check(rqstp, p))
		return 0;
	base = (char *)p - (char *)head->iov_base;
=======
/* GETACL */
static int nfsaclsvc_encode_getaclres(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct dentry *dentry = resp->fh.fh_dentry;
	struct inode *inode;
	int w;

	if (!svcxdr_encode_stat(xdr, resp->status))
		return 0;

	if (dentry == NULL || d_really_is_negative(dentry))
		return 1;
	inode = d_inode(dentry);

	if (!svcxdr_encode_fattr(rqstp, xdr, &resp->fh, &resp->stat))
		return 0;
	if (xdr_stream_encode_u32(xdr, resp->mask) < 0)
		return 0;
>>>>>>> upstream/android-13

	rqstp->rq_res.page_len = w = nfsacl_size(
		(resp->mask & NFS_ACL)   ? resp->acl_access  : NULL,
		(resp->mask & NFS_DFACL) ? resp->acl_default : NULL);
	while (w > 0) {
		if (!*(rqstp->rq_next_page++))
<<<<<<< HEAD
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
	return (n > 0);
}

static int nfsaclsvc_encode_attrstatres(struct svc_rqst *rqstp, __be32 *p)
{
	struct nfsd_attrstat *resp = rqstp->rq_resp;

	p = nfs2svc_encode_fattr(rqstp, p, &resp->fh, &resp->stat);
	return xdr_ressize_check(rqstp, p);
=======
			return 1;
		w -= PAGE_SIZE;
	}

	if (!nfs_stream_encode_acl(xdr, inode, resp->acl_access,
				   resp->mask & NFS_ACL, 0))
		return 0;
	if (!nfs_stream_encode_acl(xdr, inode, resp->acl_default,
				   resp->mask & NFS_DFACL, NFS_ACL_DEFAULT))
		return 0;

	return 1;
>>>>>>> upstream/android-13
}

/* ACCESS */
static int nfsaclsvc_encode_accessres(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	p = nfs2svc_encode_fattr(rqstp, p, &resp->fh, &resp->stat);
	*p++ = htonl(resp->access);
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_stat(xdr, resp->status))
		return 0;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_fattr(rqstp, xdr, &resp->fh, &resp->stat))
			return 0;
		if (xdr_stream_encode_u32(xdr, resp->access) < 0)
			return 0;
		break;
	}

	return 1;
>>>>>>> upstream/android-13
}

/*
 * XDR release functions
 */
static void nfsaclsvc_release_getacl(struct svc_rqst *rqstp)
{
	struct nfsd3_getaclres *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
	posix_acl_release(resp->acl_access);
	posix_acl_release(resp->acl_default);
}

<<<<<<< HEAD
static void nfsaclsvc_release_attrstat(struct svc_rqst *rqstp)
{
	struct nfsd_attrstat *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
}

=======
>>>>>>> upstream/android-13
static void nfsaclsvc_release_access(struct svc_rqst *rqstp)
{
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
}

<<<<<<< HEAD
#define nfsaclsvc_decode_voidargs	NULL
#define nfsaclsvc_release_void		NULL
#define nfsd3_fhandleargs	nfsd_fhandle
#define nfsd3_attrstatres	nfsd_attrstat
#define nfsd3_voidres		nfsd3_voidargs
struct nfsd3_voidargs { int dummy; };

#define PROC(name, argt, rest, relt, cache, respsize)			\
{									\
	.pc_func	= nfsacld_proc_##name,				\
	.pc_decode	= nfsaclsvc_decode_##argt##args,		\
	.pc_encode	= nfsaclsvc_encode_##rest##res,			\
	.pc_release	= nfsaclsvc_release_##relt,	\
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
static const struct svc_procedure nfsd_acl_procedures2[] = {
  PROC(null,	void,		void,		void,	  RC_NOCACHE, ST),
  PROC(getacl,	getacl,		getacl,		getacl,	  RC_NOCACHE, ST+1+2*(1+ACL)),
  PROC(setacl,	setacl,		attrstat,	attrstat, RC_NOCACHE, ST+AT),
  PROC(getattr, fhandle,	attrstat,	attrstat, RC_NOCACHE, ST+AT),
  PROC(access,	access,		access,		access,   RC_NOCACHE, ST+AT+1),
=======
static const struct svc_procedure nfsd_acl_procedures2[5] = {
	[ACLPROC2_NULL] = {
		.pc_func = nfsacld_proc_null,
		.pc_decode = nfssvc_decode_voidarg,
		.pc_encode = nfssvc_encode_voidres,
		.pc_argsize = sizeof(struct nfsd_voidargs),
		.pc_ressize = sizeof(struct nfsd_voidres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST,
		.pc_name = "NULL",
	},
	[ACLPROC2_GETACL] = {
		.pc_func = nfsacld_proc_getacl,
		.pc_decode = nfsaclsvc_decode_getaclargs,
		.pc_encode = nfsaclsvc_encode_getaclres,
		.pc_release = nfsaclsvc_release_getacl,
		.pc_argsize = sizeof(struct nfsd3_getaclargs),
		.pc_ressize = sizeof(struct nfsd3_getaclres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+1+2*(1+ACL),
		.pc_name = "GETACL",
	},
	[ACLPROC2_SETACL] = {
		.pc_func = nfsacld_proc_setacl,
		.pc_decode = nfsaclsvc_decode_setaclargs,
		.pc_encode = nfssvc_encode_attrstatres,
		.pc_release = nfssvc_release_attrstat,
		.pc_argsize = sizeof(struct nfsd3_setaclargs),
		.pc_ressize = sizeof(struct nfsd_attrstat),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT,
		.pc_name = "SETACL",
	},
	[ACLPROC2_GETATTR] = {
		.pc_func = nfsacld_proc_getattr,
		.pc_decode = nfssvc_decode_fhandleargs,
		.pc_encode = nfssvc_encode_attrstatres,
		.pc_release = nfssvc_release_attrstat,
		.pc_argsize = sizeof(struct nfsd_fhandle),
		.pc_ressize = sizeof(struct nfsd_attrstat),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT,
		.pc_name = "GETATTR",
	},
	[ACLPROC2_ACCESS] = {
		.pc_func = nfsacld_proc_access,
		.pc_decode = nfsaclsvc_decode_accessargs,
		.pc_encode = nfsaclsvc_encode_accessres,
		.pc_release = nfsaclsvc_release_access,
		.pc_argsize = sizeof(struct nfsd3_accessargs),
		.pc_ressize = sizeof(struct nfsd3_accessres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT+1,
		.pc_name = "SETATTR",
	},
>>>>>>> upstream/android-13
};

static unsigned int nfsd_acl_count2[ARRAY_SIZE(nfsd_acl_procedures2)];
const struct svc_version nfsd_acl_version2 = {
	.vs_vers	= 2,
	.vs_nproc	= 5,
	.vs_proc	= nfsd_acl_procedures2,
	.vs_count	= nfsd_acl_count2,
	.vs_dispatch	= nfsd_dispatch,
	.vs_xdrsize	= NFS3_SVC_XDRSIZE,
};
