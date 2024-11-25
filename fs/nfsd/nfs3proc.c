// SPDX-License-Identifier: GPL-2.0
/*
 * Process version 3 NFS requests.
 *
 * Copyright (C) 1996, 1997, 1998 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/fs.h>
#include <linux/ext2_fs.h>
#include <linux/magic.h>

#include "cache.h"
#include "xdr3.h"
#include "vfs.h"

#define NFSDDBG_FACILITY		NFSDDBG_PROC

<<<<<<< HEAD
#define RETURN_STATUS(st)	{ resp->status = (st); return (st); }

=======
>>>>>>> upstream/android-13
static int	nfs3_ftypes[] = {
	0,			/* NF3NON */
	S_IFREG,		/* NF3REG */
	S_IFDIR,		/* NF3DIR */
	S_IFBLK,		/* NF3BLK */
	S_IFCHR,		/* NF3CHR */
	S_IFLNK,		/* NF3LNK */
	S_IFSOCK,		/* NF3SOCK */
	S_IFIFO,		/* NF3FIFO */
};

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
 * Get a file's attributes
 */
static __be32
nfsd3_proc_getattr(struct svc_rqst *rqstp)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: GETATTR(3)  %s\n",
		SVCFH_fmt(&argp->fh));

	fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = fh_verify(rqstp, &resp->fh, 0,
			NFSD_MAY_NOP | NFSD_MAY_BYPASS_GSS_ON_ROOT);
	if (nfserr)
		RETURN_STATUS(nfserr);

	nfserr = fh_getattr(&resp->fh, &resp->stat);

	RETURN_STATUS(nfserr);
=======
	resp->status = fh_verify(rqstp, &resp->fh, 0,
				 NFSD_MAY_NOP | NFSD_MAY_BYPASS_GSS_ON_ROOT);
	if (resp->status != nfs_ok)
		goto out;

	resp->status = fh_getattr(&resp->fh, &resp->stat);
out:
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Set a file's attributes
 */
static __be32
nfsd3_proc_setattr(struct svc_rqst *rqstp)
{
	struct nfsd3_sattrargs *argp = rqstp->rq_argp;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: SETATTR(3)  %s\n",
				SVCFH_fmt(&argp->fh));

	fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = nfsd_setattr(rqstp, &resp->fh, &argp->attrs,
			      argp->check_guard, argp->guardtime);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_setattr(rqstp, &resp->fh, &argp->attrs,
				    argp->check_guard, argp->guardtime);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Look up a path name component
 */
static __be32
nfsd3_proc_lookup(struct svc_rqst *rqstp)
{
	struct nfsd3_diropargs *argp = rqstp->rq_argp;
	struct nfsd3_diropres  *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: LOOKUP(3)   %s %.*s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				argp->name);

	fh_copy(&resp->dirfh, &argp->fh);
	fh_init(&resp->fh, NFS3_FHSIZE);

<<<<<<< HEAD
	nfserr = nfsd_lookup(rqstp, &resp->dirfh,
				    argp->name,
				    argp->len,
				    &resp->fh);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_lookup(rqstp, &resp->dirfh,
				   argp->name, argp->len,
				   &resp->fh);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Check file access
 */
static __be32
nfsd3_proc_access(struct svc_rqst *rqstp)
{
	struct nfsd3_accessargs *argp = rqstp->rq_argp;
	struct nfsd3_accessres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: ACCESS(3)   %s 0x%x\n",
				SVCFH_fmt(&argp->fh),
				argp->access);

	fh_copy(&resp->fh, &argp->fh);
	resp->access = argp->access;
<<<<<<< HEAD
	nfserr = nfsd_access(rqstp, &resp->fh, &resp->access, NULL);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_access(rqstp, &resp->fh, &resp->access, NULL);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Read a symlink.
 */
static __be32
nfsd3_proc_readlink(struct svc_rqst *rqstp)
{
<<<<<<< HEAD
	struct nfsd3_readlinkargs *argp = rqstp->rq_argp;
	struct nfsd3_readlinkres *resp = rqstp->rq_resp;
	__be32 nfserr;
=======
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd3_readlinkres *resp = rqstp->rq_resp;
>>>>>>> upstream/android-13

	dprintk("nfsd: READLINK(3) %s\n", SVCFH_fmt(&argp->fh));

	/* Read the symlink. */
	fh_copy(&resp->fh, &argp->fh);
	resp->len = NFS3_MAXPATHLEN;
<<<<<<< HEAD
	nfserr = nfsd_readlink(rqstp, &resp->fh, argp->buffer, &resp->len);
	RETURN_STATUS(nfserr);
=======
	resp->pages = rqstp->rq_next_page++;
	resp->status = nfsd_readlink(rqstp, &resp->fh,
				     page_address(*resp->pages), &resp->len);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Read a portion of a file.
 */
static __be32
nfsd3_proc_read(struct svc_rqst *rqstp)
{
	struct nfsd3_readargs *argp = rqstp->rq_argp;
	struct nfsd3_readres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
	u32	max_blocksize = svc_max_payload(rqstp);
	unsigned long cnt = min(argp->count, max_blocksize);
=======
	u32 max_blocksize = svc_max_payload(rqstp);
	unsigned int len;
	int v;
>>>>>>> upstream/android-13

	dprintk("nfsd: READ(3) %s %lu bytes at %Lu\n",
				SVCFH_fmt(&argp->fh),
				(unsigned long) argp->count,
				(unsigned long long) argp->offset);

<<<<<<< HEAD
=======
	argp->count = min_t(u32, argp->count, max_blocksize);
	if (argp->offset > (u64)OFFSET_MAX)
		argp->offset = (u64)OFFSET_MAX;
	if (argp->offset + argp->count > (u64)OFFSET_MAX)
		argp->count = (u64)OFFSET_MAX - argp->offset;

	v = 0;
	len = argp->count;
	resp->pages = rqstp->rq_next_page;
	while (len > 0) {
		struct page *page = *(rqstp->rq_next_page++);

		rqstp->rq_vec[v].iov_base = page_address(page);
		rqstp->rq_vec[v].iov_len = min_t(unsigned int, len, PAGE_SIZE);
		len -= rqstp->rq_vec[v].iov_len;
		v++;
	}

>>>>>>> upstream/android-13
	/* Obtain buffer pointer for payload.
	 * 1 (status) + 22 (post_op_attr) + 1 (count) + 1 (eof)
	 * + 1 (xdr opaque byte count) = 26
	 */
<<<<<<< HEAD
	resp->count = cnt;
	svc_reserve_auth(rqstp, ((1 + NFS3_POST_OP_ATTR_WORDS + 3)<<2) + resp->count +4);

	fh_copy(&resp->fh, &argp->fh);
	nfserr = nfsd_read(rqstp, &resp->fh,
				  argp->offset,
			   	  rqstp->rq_vec, argp->vlen,
				  &resp->count);
	if (nfserr == 0) {
		struct inode	*inode = d_inode(resp->fh.fh_dentry);
		resp->eof = nfsd_eof_on_read(cnt, resp->count, argp->offset,
							inode->i_size);
	}

	RETURN_STATUS(nfserr);
=======
	resp->count = argp->count;
	svc_reserve_auth(rqstp, ((1 + NFS3_POST_OP_ATTR_WORDS + 3)<<2) + resp->count +4);

	fh_copy(&resp->fh, &argp->fh);
	resp->status = nfsd_read(rqstp, &resp->fh, argp->offset,
				 rqstp->rq_vec, v, &resp->count, &resp->eof);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Write data to a file
 */
static __be32
nfsd3_proc_write(struct svc_rqst *rqstp)
{
	struct nfsd3_writeargs *argp = rqstp->rq_argp;
	struct nfsd3_writeres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13
	unsigned long cnt = argp->len;
	unsigned int nvecs;

	dprintk("nfsd: WRITE(3)    %s %d bytes at %Lu%s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				(unsigned long long) argp->offset,
				argp->stable? " stable" : "");

<<<<<<< HEAD
	fh_copy(&resp->fh, &argp->fh);
	resp->committed = argp->stable;
	nvecs = svc_fill_write_vector(rqstp, rqstp->rq_arg.pages,
				      &argp->first, cnt);
	if (!nvecs)
		RETURN_STATUS(nfserr_io);
	nfserr = nfsd_write(rqstp, &resp->fh, argp->offset,
			    rqstp->rq_vec, nvecs, &cnt,
			    resp->committed);
	resp->count = cnt;
	RETURN_STATUS(nfserr);
=======
	resp->status = nfserr_fbig;
	if (argp->offset > (u64)OFFSET_MAX ||
	    argp->offset + argp->len > (u64)OFFSET_MAX)
		return rpc_success;

	fh_copy(&resp->fh, &argp->fh);
	resp->committed = argp->stable;
	nvecs = svc_fill_write_vector(rqstp, &argp->payload);

	resp->status = nfsd_write(rqstp, &resp->fh, argp->offset,
				  rqstp->rq_vec, nvecs, &cnt,
				  resp->committed, resp->verf);
	resp->count = cnt;
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * With NFSv3, CREATE processing is a lot easier than with NFSv2.
 * At least in theory; we'll see how it fares in practice when the
 * first reports about SunOS compatibility problems start to pour in...
 */
static __be32
nfsd3_proc_create(struct svc_rqst *rqstp)
{
	struct nfsd3_createargs *argp = rqstp->rq_argp;
	struct nfsd3_diropres *resp = rqstp->rq_resp;
	svc_fh		*dirfhp, *newfhp = NULL;
	struct iattr	*attr;
<<<<<<< HEAD
	__be32		nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: CREATE(3)   %s %.*s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				argp->name);

	dirfhp = fh_copy(&resp->dirfh, &argp->fh);
	newfhp = fh_init(&resp->fh, NFS3_FHSIZE);
	attr   = &argp->attrs;

	/* Unfudge the mode bits */
	attr->ia_mode &= ~S_IFMT;
	if (!(attr->ia_valid & ATTR_MODE)) { 
		attr->ia_valid |= ATTR_MODE;
		attr->ia_mode = S_IFREG;
	} else {
		attr->ia_mode = (attr->ia_mode & ~S_IFMT) | S_IFREG;
	}

	/* Now create the file and set attributes */
<<<<<<< HEAD
	nfserr = do_nfsd_create(rqstp, dirfhp, argp->name, argp->len,
				attr, newfhp,
				argp->createmode, (u32 *)argp->verf, NULL, NULL);

	RETURN_STATUS(nfserr);
=======
	resp->status = do_nfsd_create(rqstp, dirfhp, argp->name, argp->len,
				      attr, newfhp, argp->createmode,
				      (u32 *)argp->verf, NULL, NULL);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Make directory. This operation is not idempotent.
 */
static __be32
nfsd3_proc_mkdir(struct svc_rqst *rqstp)
{
	struct nfsd3_createargs *argp = rqstp->rq_argp;
	struct nfsd3_diropres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: MKDIR(3)    %s %.*s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				argp->name);

	argp->attrs.ia_valid &= ~ATTR_SIZE;
	fh_copy(&resp->dirfh, &argp->fh);
	fh_init(&resp->fh, NFS3_FHSIZE);
<<<<<<< HEAD
	nfserr = nfsd_create(rqstp, &resp->dirfh, argp->name, argp->len,
				    &argp->attrs, S_IFDIR, 0, &resp->fh);
	fh_unlock(&resp->dirfh);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_create(rqstp, &resp->dirfh, argp->name, argp->len,
				   &argp->attrs, S_IFDIR, 0, &resp->fh);
	fh_unlock(&resp->dirfh);
	return rpc_success;
>>>>>>> upstream/android-13
}

static __be32
nfsd3_proc_symlink(struct svc_rqst *rqstp)
{
	struct nfsd3_symlinkargs *argp = rqstp->rq_argp;
	struct nfsd3_diropres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;

	if (argp->tlen == 0)
		RETURN_STATUS(nfserr_inval);
	if (argp->tlen > NFS3_MAXPATHLEN)
		RETURN_STATUS(nfserr_nametoolong);
=======

	if (argp->tlen == 0) {
		resp->status = nfserr_inval;
		goto out;
	}
	if (argp->tlen > NFS3_MAXPATHLEN) {
		resp->status = nfserr_nametoolong;
		goto out;
	}
>>>>>>> upstream/android-13

	argp->tname = svc_fill_symlink_pathname(rqstp, &argp->first,
						page_address(rqstp->rq_arg.pages[0]),
						argp->tlen);
<<<<<<< HEAD
	if (IS_ERR(argp->tname))
		RETURN_STATUS(nfserrno(PTR_ERR(argp->tname)));
=======
	if (IS_ERR(argp->tname)) {
		resp->status = nfserrno(PTR_ERR(argp->tname));
		goto out;
	}
>>>>>>> upstream/android-13

	dprintk("nfsd: SYMLINK(3)  %s %.*s -> %.*s\n",
				SVCFH_fmt(&argp->ffh),
				argp->flen, argp->fname,
				argp->tlen, argp->tname);

	fh_copy(&resp->dirfh, &argp->ffh);
	fh_init(&resp->fh, NFS3_FHSIZE);
<<<<<<< HEAD
	nfserr = nfsd_symlink(rqstp, &resp->dirfh, argp->fname, argp->flen,
						   argp->tname, &resp->fh);
	kfree(argp->tname);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_symlink(rqstp, &resp->dirfh, argp->fname,
				    argp->flen, argp->tname, &resp->fh);
	kfree(argp->tname);
out:
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Make socket/fifo/device.
 */
static __be32
nfsd3_proc_mknod(struct svc_rqst *rqstp)
{
	struct nfsd3_mknodargs *argp = rqstp->rq_argp;
	struct nfsd3_diropres  *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13
	int type;
	dev_t	rdev = 0;

	dprintk("nfsd: MKNOD(3)    %s %.*s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				argp->name);

	fh_copy(&resp->dirfh, &argp->fh);
	fh_init(&resp->fh, NFS3_FHSIZE);

<<<<<<< HEAD
	if (argp->ftype == 0 || argp->ftype >= NF3BAD)
		RETURN_STATUS(nfserr_inval);
	if (argp->ftype == NF3CHR || argp->ftype == NF3BLK) {
		rdev = MKDEV(argp->major, argp->minor);
		if (MAJOR(rdev) != argp->major ||
		    MINOR(rdev) != argp->minor)
			RETURN_STATUS(nfserr_inval);
	} else
		if (argp->ftype != NF3SOCK && argp->ftype != NF3FIFO)
			RETURN_STATUS(nfserr_inval);

	type = nfs3_ftypes[argp->ftype];
	nfserr = nfsd_create(rqstp, &resp->dirfh, argp->name, argp->len,
				    &argp->attrs, type, rdev, &resp->fh);
	fh_unlock(&resp->dirfh);
	RETURN_STATUS(nfserr);
=======
	if (argp->ftype == NF3CHR || argp->ftype == NF3BLK) {
		rdev = MKDEV(argp->major, argp->minor);
		if (MAJOR(rdev) != argp->major ||
		    MINOR(rdev) != argp->minor) {
			resp->status = nfserr_inval;
			goto out;
		}
	} else if (argp->ftype != NF3SOCK && argp->ftype != NF3FIFO) {
		resp->status = nfserr_badtype;
		goto out;
	}

	type = nfs3_ftypes[argp->ftype];
	resp->status = nfsd_create(rqstp, &resp->dirfh, argp->name, argp->len,
				   &argp->attrs, type, rdev, &resp->fh);
	fh_unlock(&resp->dirfh);
out:
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Remove file/fifo/socket etc.
 */
static __be32
nfsd3_proc_remove(struct svc_rqst *rqstp)
{
	struct nfsd3_diropargs *argp = rqstp->rq_argp;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: REMOVE(3)   %s %.*s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				argp->name);

	/* Unlink. -S_IFDIR means file must not be a directory */
	fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = nfsd_unlink(rqstp, &resp->fh, -S_IFDIR, argp->name, argp->len);
	fh_unlock(&resp->fh);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_unlink(rqstp, &resp->fh, -S_IFDIR,
				   argp->name, argp->len);
	fh_unlock(&resp->fh);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Remove a directory
 */
static __be32
nfsd3_proc_rmdir(struct svc_rqst *rqstp)
{
	struct nfsd3_diropargs *argp = rqstp->rq_argp;
	struct nfsd3_attrstat *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: RMDIR(3)    %s %.*s\n",
				SVCFH_fmt(&argp->fh),
				argp->len,
				argp->name);

	fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = nfsd_unlink(rqstp, &resp->fh, S_IFDIR, argp->name, argp->len);
	fh_unlock(&resp->fh);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_unlink(rqstp, &resp->fh, S_IFDIR,
				   argp->name, argp->len);
	fh_unlock(&resp->fh);
	return rpc_success;
>>>>>>> upstream/android-13
}

static __be32
nfsd3_proc_rename(struct svc_rqst *rqstp)
{
	struct nfsd3_renameargs *argp = rqstp->rq_argp;
	struct nfsd3_renameres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: RENAME(3)   %s %.*s ->\n",
				SVCFH_fmt(&argp->ffh),
				argp->flen,
				argp->fname);
	dprintk("nfsd: -> %s %.*s\n",
				SVCFH_fmt(&argp->tfh),
				argp->tlen,
				argp->tname);

	fh_copy(&resp->ffh, &argp->ffh);
	fh_copy(&resp->tfh, &argp->tfh);
<<<<<<< HEAD
	nfserr = nfsd_rename(rqstp, &resp->ffh, argp->fname, argp->flen,
				    &resp->tfh, argp->tname, argp->tlen);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_rename(rqstp, &resp->ffh, argp->fname, argp->flen,
				   &resp->tfh, argp->tname, argp->tlen);
	return rpc_success;
>>>>>>> upstream/android-13
}

static __be32
nfsd3_proc_link(struct svc_rqst *rqstp)
{
	struct nfsd3_linkargs *argp = rqstp->rq_argp;
	struct nfsd3_linkres  *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: LINK(3)     %s ->\n",
				SVCFH_fmt(&argp->ffh));
	dprintk("nfsd:   -> %s %.*s\n",
				SVCFH_fmt(&argp->tfh),
				argp->tlen,
				argp->tname);

	fh_copy(&resp->fh,  &argp->ffh);
	fh_copy(&resp->tfh, &argp->tfh);
<<<<<<< HEAD
	nfserr = nfsd_link(rqstp, &resp->tfh, argp->tname, argp->tlen,
				  &resp->fh);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_link(rqstp, &resp->tfh, argp->tname, argp->tlen,
				 &resp->fh);
	return rpc_success;
}

static void nfsd3_init_dirlist_pages(struct svc_rqst *rqstp,
				     struct nfsd3_readdirres *resp,
				     u32 count)
{
	struct xdr_buf *buf = &resp->dirlist;
	struct xdr_stream *xdr = &resp->xdr;

	count = clamp(count, (u32)(XDR_UNIT * 2), svc_max_payload(rqstp));

	memset(buf, 0, sizeof(*buf));

	/* Reserve room for the NULL ptr & eof flag (-2 words) */
	buf->buflen = count - XDR_UNIT * 2;
	buf->pages = rqstp->rq_next_page;
	rqstp->rq_next_page += (buf->buflen + PAGE_SIZE - 1) >> PAGE_SHIFT;

	/* This is xdr_init_encode(), but it assumes that
	 * the head kvec has already been consumed. */
	xdr_set_scratch_buffer(xdr, NULL, 0);
	xdr->buf = buf;
	xdr->page_ptr = buf->pages;
	xdr->iov = NULL;
	xdr->p = page_address(*buf->pages);
	xdr->end = (void *)xdr->p + min_t(u32, buf->buflen, PAGE_SIZE);
	xdr->rqst = NULL;
>>>>>>> upstream/android-13
}

/*
 * Read a portion of a directory.
 */
static __be32
nfsd3_proc_readdir(struct svc_rqst *rqstp)
{
	struct nfsd3_readdirargs *argp = rqstp->rq_argp;
	struct nfsd3_readdirres  *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32		nfserr;
	int		count;
=======
	loff_t		offset;
>>>>>>> upstream/android-13

	dprintk("nfsd: READDIR(3)  %s %d bytes at %d\n",
				SVCFH_fmt(&argp->fh),
				argp->count, (u32) argp->cookie);

<<<<<<< HEAD
	/* Make sure we've room for the NULL ptr & eof flag, and shrink to
	 * client read size */
	count = (argp->count >> 2) - 2;

	/* Read directory and encode entries on the fly */
	fh_copy(&resp->fh, &argp->fh);

	resp->buflen = count;
	resp->common.err = nfs_ok;
	resp->buffer = argp->buffer;
	resp->rqstp = rqstp;
	nfserr = nfsd_readdir(rqstp, &resp->fh, (loff_t*) &argp->cookie, 
					&resp->common, nfs3svc_encode_entry);
	memcpy(resp->verf, argp->verf, 8);
	resp->count = resp->buffer - argp->buffer;
	if (resp->offset) {
		loff_t offset = argp->cookie;

		if (unlikely(resp->offset1)) {
			/* we ended up with offset on a page boundary */
			*resp->offset = htonl(offset >> 32);
			*resp->offset1 = htonl(offset & 0xffffffff);
			resp->offset1 = NULL;
		} else {
			xdr_encode_hyper(resp->offset, offset);
		}
		resp->offset = NULL;
	}

	RETURN_STATUS(nfserr);
=======
	nfsd3_init_dirlist_pages(rqstp, resp, argp->count);

	fh_copy(&resp->fh, &argp->fh);
	resp->common.err = nfs_ok;
	resp->cookie_offset = 0;
	resp->rqstp = rqstp;
	offset = argp->cookie;
	resp->status = nfsd_readdir(rqstp, &resp->fh, &offset,
				    &resp->common, nfs3svc_encode_entry3);
	memcpy(resp->verf, argp->verf, 8);
	nfs3svc_encode_cookie3(resp, offset);

	/* Recycle only pages that were part of the reply */
	rqstp->rq_next_page = resp->xdr.page_ptr + 1;

	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Read a portion of a directory, including file handles and attrs.
 * For now, we choose to ignore the dircount parameter.
 */
static __be32
nfsd3_proc_readdirplus(struct svc_rqst *rqstp)
{
	struct nfsd3_readdirargs *argp = rqstp->rq_argp;
	struct nfsd3_readdirres  *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
	int	count = 0;
	loff_t	offset;
	struct page **p;
	caddr_t	page_addr = NULL;
=======
	loff_t	offset;
>>>>>>> upstream/android-13

	dprintk("nfsd: READDIR+(3) %s %d bytes at %d\n",
				SVCFH_fmt(&argp->fh),
				argp->count, (u32) argp->cookie);

<<<<<<< HEAD
	/* Convert byte count to number of words (i.e. >> 2),
	 * and reserve room for the NULL ptr & eof flag (-2 words) */
	resp->count = (argp->count >> 2) - 2;

	/* Read directory and encode entries on the fly */
	fh_copy(&resp->fh, &argp->fh);

	resp->common.err = nfs_ok;
	resp->buffer = argp->buffer;
	resp->buflen = resp->count;
	resp->rqstp = rqstp;
	offset = argp->cookie;

	nfserr = fh_verify(rqstp, &resp->fh, S_IFDIR, NFSD_MAY_NOP);
	if (nfserr)
		RETURN_STATUS(nfserr);

	if (resp->fh.fh_export->ex_flags & NFSEXP_NOREADDIRPLUS)
		RETURN_STATUS(nfserr_notsupp);

	nfserr = nfsd_readdir(rqstp, &resp->fh,
				     &offset,
				     &resp->common,
				     nfs3svc_encode_entry_plus);
	memcpy(resp->verf, argp->verf, 8);
	for (p = rqstp->rq_respages + 1; p < rqstp->rq_next_page; p++) {
		page_addr = page_address(*p);

		if (((caddr_t)resp->buffer >= page_addr) &&
		    ((caddr_t)resp->buffer < page_addr + PAGE_SIZE)) {
			count += (caddr_t)resp->buffer - page_addr;
			break;
		}
		count += PAGE_SIZE;
	}
	resp->count = count >> 2;
	if (resp->offset) {
		if (unlikely(resp->offset1)) {
			/* we ended up with offset on a page boundary */
			*resp->offset = htonl(offset >> 32);
			*resp->offset1 = htonl(offset & 0xffffffff);
			resp->offset1 = NULL;
		} else {
			xdr_encode_hyper(resp->offset, offset);
		}
		resp->offset = NULL;
	}

	RETURN_STATUS(nfserr);
=======
	nfsd3_init_dirlist_pages(rqstp, resp, argp->count);

	fh_copy(&resp->fh, &argp->fh);
	resp->common.err = nfs_ok;
	resp->cookie_offset = 0;
	resp->rqstp = rqstp;
	offset = argp->cookie;

	resp->status = fh_verify(rqstp, &resp->fh, S_IFDIR, NFSD_MAY_NOP);
	if (resp->status != nfs_ok)
		goto out;

	if (resp->fh.fh_export->ex_flags & NFSEXP_NOREADDIRPLUS) {
		resp->status = nfserr_notsupp;
		goto out;
	}

	resp->status = nfsd_readdir(rqstp, &resp->fh, &offset,
				    &resp->common, nfs3svc_encode_entryplus3);
	memcpy(resp->verf, argp->verf, 8);
	nfs3svc_encode_cookie3(resp, offset);

	/* Recycle only pages that were part of the reply */
	rqstp->rq_next_page = resp->xdr.page_ptr + 1;

out:
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Get file system stats
 */
static __be32
nfsd3_proc_fsstat(struct svc_rqst *rqstp)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd3_fsstatres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: FSSTAT(3)   %s\n",
				SVCFH_fmt(&argp->fh));

<<<<<<< HEAD
	nfserr = nfsd_statfs(rqstp, &argp->fh, &resp->stats, 0);
	fh_put(&argp->fh);
	RETURN_STATUS(nfserr);
=======
	resp->status = nfsd_statfs(rqstp, &argp->fh, &resp->stats, 0);
	fh_put(&argp->fh);
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Get file system info
 */
static __be32
nfsd3_proc_fsinfo(struct svc_rqst *rqstp)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd3_fsinfores *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13
	u32	max_blocksize = svc_max_payload(rqstp);

	dprintk("nfsd: FSINFO(3)   %s\n",
				SVCFH_fmt(&argp->fh));

	resp->f_rtmax  = max_blocksize;
	resp->f_rtpref = max_blocksize;
	resp->f_rtmult = PAGE_SIZE;
	resp->f_wtmax  = max_blocksize;
	resp->f_wtpref = max_blocksize;
	resp->f_wtmult = PAGE_SIZE;
<<<<<<< HEAD
	resp->f_dtpref = PAGE_SIZE;
	resp->f_maxfilesize = ~(u32) 0;
	resp->f_properties = NFS3_FSF_DEFAULT;

	nfserr = fh_verify(rqstp, &argp->fh, 0,
			NFSD_MAY_NOP | NFSD_MAY_BYPASS_GSS_ON_ROOT);
=======
	resp->f_dtpref = max_blocksize;
	resp->f_maxfilesize = ~(u32) 0;
	resp->f_properties = NFS3_FSF_DEFAULT;

	resp->status = fh_verify(rqstp, &argp->fh, 0,
				 NFSD_MAY_NOP | NFSD_MAY_BYPASS_GSS_ON_ROOT);
>>>>>>> upstream/android-13

	/* Check special features of the file system. May request
	 * different read/write sizes for file systems known to have
	 * problems with large blocks */
<<<<<<< HEAD
	if (nfserr == 0) {
=======
	if (resp->status == nfs_ok) {
>>>>>>> upstream/android-13
		struct super_block *sb = argp->fh.fh_dentry->d_sb;

		/* Note that we don't care for remote fs's here */
		if (sb->s_magic == MSDOS_SUPER_MAGIC) {
			resp->f_properties = NFS3_FSF_BILLYBOY;
		}
		resp->f_maxfilesize = sb->s_maxbytes;
	}

	fh_put(&argp->fh);
<<<<<<< HEAD
	RETURN_STATUS(nfserr);
=======
	return rpc_success;
>>>>>>> upstream/android-13
}

/*
 * Get pathconf info for the specified file
 */
static __be32
nfsd3_proc_pathconf(struct svc_rqst *rqstp)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd3_pathconfres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: PATHCONF(3) %s\n",
				SVCFH_fmt(&argp->fh));

	/* Set default pathconf */
	resp->p_link_max = 255;		/* at least */
	resp->p_name_max = 255;		/* at least */
	resp->p_no_trunc = 0;
	resp->p_chown_restricted = 1;
	resp->p_case_insensitive = 0;
	resp->p_case_preserving = 1;

<<<<<<< HEAD
	nfserr = fh_verify(rqstp, &argp->fh, 0, NFSD_MAY_NOP);

	if (nfserr == 0) {
=======
	resp->status = fh_verify(rqstp, &argp->fh, 0, NFSD_MAY_NOP);

	if (resp->status == nfs_ok) {
>>>>>>> upstream/android-13
		struct super_block *sb = argp->fh.fh_dentry->d_sb;

		/* Note that we don't care for remote fs's here */
		switch (sb->s_magic) {
		case EXT2_SUPER_MAGIC:
			resp->p_link_max = EXT2_LINK_MAX;
			resp->p_name_max = EXT2_NAME_LEN;
			break;
		case MSDOS_SUPER_MAGIC:
			resp->p_case_insensitive = 1;
			resp->p_case_preserving  = 0;
			break;
		}
	}

	fh_put(&argp->fh);
<<<<<<< HEAD
	RETURN_STATUS(nfserr);
}


=======
	return rpc_success;
}

>>>>>>> upstream/android-13
/*
 * Commit a file (range) to stable storage.
 */
static __be32
nfsd3_proc_commit(struct svc_rqst *rqstp)
{
	struct nfsd3_commitargs *argp = rqstp->rq_argp;
	struct nfsd3_commitres *resp = rqstp->rq_resp;
<<<<<<< HEAD
	__be32	nfserr;
=======
>>>>>>> upstream/android-13

	dprintk("nfsd: COMMIT(3)   %s %u@%Lu\n",
				SVCFH_fmt(&argp->fh),
				argp->count,
				(unsigned long long) argp->offset);

<<<<<<< HEAD
	if (argp->offset > NFS_OFFSET_MAX)
		RETURN_STATUS(nfserr_inval);

	fh_copy(&resp->fh, &argp->fh);
	nfserr = nfsd_commit(rqstp, &resp->fh, argp->offset, argp->count);

	RETURN_STATUS(nfserr);
=======
	if (argp->offset > NFS_OFFSET_MAX) {
		resp->status = nfserr_inval;
		goto out;
	}

	fh_copy(&resp->fh, &argp->fh);
	resp->status = nfsd_commit(rqstp, &resp->fh, argp->offset,
				   argp->count, resp->verf);
out:
	return rpc_success;
>>>>>>> upstream/android-13
}


/*
 * NFSv3 Server procedures.
 * Only the results of non-idempotent operations are cached.
 */
<<<<<<< HEAD
#define nfs3svc_decode_fhandleargs	nfs3svc_decode_fhandle
=======
>>>>>>> upstream/android-13
#define nfs3svc_encode_attrstatres	nfs3svc_encode_attrstat
#define nfs3svc_encode_wccstatres	nfs3svc_encode_wccstat
#define nfsd3_mkdirargs			nfsd3_createargs
#define nfsd3_readdirplusargs		nfsd3_readdirargs
#define nfsd3_fhandleargs		nfsd_fhandle
<<<<<<< HEAD
#define nfsd3_fhandleres		nfsd3_attrstat
#define nfsd3_attrstatres		nfsd3_attrstat
#define nfsd3_wccstatres		nfsd3_attrstat
#define nfsd3_createres			nfsd3_diropres
#define nfsd3_voidres			nfsd3_voidargs
struct nfsd3_voidargs { int dummy; };
=======
#define nfsd3_attrstatres		nfsd3_attrstat
#define nfsd3_wccstatres		nfsd3_attrstat
#define nfsd3_createres			nfsd3_diropres
>>>>>>> upstream/android-13

#define ST 1		/* status*/
#define FH 17		/* filehandle with length */
#define AT 21		/* attributes */
#define pAT (1+AT)	/* post attributes - conditional */
#define WC (7+pAT)	/* WCC attributes */

static const struct svc_procedure nfsd_procedures3[22] = {
	[NFS3PROC_NULL] = {
		.pc_func = nfsd3_proc_null,
<<<<<<< HEAD
		.pc_encode = nfs3svc_encode_voidres,
		.pc_argsize = sizeof(struct nfsd3_voidargs),
		.pc_ressize = sizeof(struct nfsd3_voidres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST,
=======
		.pc_decode = nfssvc_decode_voidarg,
		.pc_encode = nfssvc_encode_voidres,
		.pc_argsize = sizeof(struct nfsd_voidargs),
		.pc_ressize = sizeof(struct nfsd_voidres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST,
		.pc_name = "NULL",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_GETATTR] = {
		.pc_func = nfsd3_proc_getattr,
		.pc_decode = nfs3svc_decode_fhandleargs,
<<<<<<< HEAD
		.pc_encode = nfs3svc_encode_attrstatres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_fhandleargs),
		.pc_ressize = sizeof(struct nfsd3_attrstatres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT,
=======
		.pc_encode = nfs3svc_encode_getattrres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd_fhandle),
		.pc_ressize = sizeof(struct nfsd3_attrstatres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT,
		.pc_name = "GETATTR",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_SETATTR] = {
		.pc_func = nfsd3_proc_setattr,
		.pc_decode = nfs3svc_decode_sattrargs,
		.pc_encode = nfs3svc_encode_wccstatres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_sattrargs),
		.pc_ressize = sizeof(struct nfsd3_wccstatres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+WC,
<<<<<<< HEAD
=======
		.pc_name = "SETATTR",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_LOOKUP] = {
		.pc_func = nfsd3_proc_lookup,
		.pc_decode = nfs3svc_decode_diropargs,
<<<<<<< HEAD
		.pc_encode = nfs3svc_encode_diropres,
=======
		.pc_encode = nfs3svc_encode_lookupres,
>>>>>>> upstream/android-13
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_diropargs),
		.pc_ressize = sizeof(struct nfsd3_diropres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+FH+pAT+pAT,
<<<<<<< HEAD
=======
		.pc_name = "LOOKUP",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_ACCESS] = {
		.pc_func = nfsd3_proc_access,
		.pc_decode = nfs3svc_decode_accessargs,
		.pc_encode = nfs3svc_encode_accessres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_accessargs),
		.pc_ressize = sizeof(struct nfsd3_accessres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+1,
<<<<<<< HEAD
	},
	[NFS3PROC_READLINK] = {
		.pc_func = nfsd3_proc_readlink,
		.pc_decode = nfs3svc_decode_readlinkargs,
		.pc_encode = nfs3svc_encode_readlinkres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_readlinkargs),
		.pc_ressize = sizeof(struct nfsd3_readlinkres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+1+NFS3_MAXPATHLEN/4,
=======
		.pc_name = "ACCESS",
	},
	[NFS3PROC_READLINK] = {
		.pc_func = nfsd3_proc_readlink,
		.pc_decode = nfs3svc_decode_fhandleargs,
		.pc_encode = nfs3svc_encode_readlinkres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd_fhandle),
		.pc_ressize = sizeof(struct nfsd3_readlinkres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+1+NFS3_MAXPATHLEN/4,
		.pc_name = "READLINK",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_READ] = {
		.pc_func = nfsd3_proc_read,
		.pc_decode = nfs3svc_decode_readargs,
		.pc_encode = nfs3svc_encode_readres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_readargs),
		.pc_ressize = sizeof(struct nfsd3_readres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+4+NFSSVC_MAXBLKSIZE/4,
<<<<<<< HEAD
=======
		.pc_name = "READ",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_WRITE] = {
		.pc_func = nfsd3_proc_write,
		.pc_decode = nfs3svc_decode_writeargs,
		.pc_encode = nfs3svc_encode_writeres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_writeargs),
		.pc_ressize = sizeof(struct nfsd3_writeres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+WC+4,
<<<<<<< HEAD
=======
		.pc_name = "WRITE",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_CREATE] = {
		.pc_func = nfsd3_proc_create,
		.pc_decode = nfs3svc_decode_createargs,
		.pc_encode = nfs3svc_encode_createres,
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_createargs),
		.pc_ressize = sizeof(struct nfsd3_createres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+(1+FH+pAT)+WC,
<<<<<<< HEAD
=======
		.pc_name = "CREATE",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_MKDIR] = {
		.pc_func = nfsd3_proc_mkdir,
		.pc_decode = nfs3svc_decode_mkdirargs,
		.pc_encode = nfs3svc_encode_createres,
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_mkdirargs),
		.pc_ressize = sizeof(struct nfsd3_createres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+(1+FH+pAT)+WC,
<<<<<<< HEAD
=======
		.pc_name = "MKDIR",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_SYMLINK] = {
		.pc_func = nfsd3_proc_symlink,
		.pc_decode = nfs3svc_decode_symlinkargs,
		.pc_encode = nfs3svc_encode_createres,
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_symlinkargs),
		.pc_ressize = sizeof(struct nfsd3_createres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+(1+FH+pAT)+WC,
<<<<<<< HEAD
=======
		.pc_name = "SYMLINK",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_MKNOD] = {
		.pc_func = nfsd3_proc_mknod,
		.pc_decode = nfs3svc_decode_mknodargs,
		.pc_encode = nfs3svc_encode_createres,
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_mknodargs),
		.pc_ressize = sizeof(struct nfsd3_createres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+(1+FH+pAT)+WC,
<<<<<<< HEAD
=======
		.pc_name = "MKNOD",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_REMOVE] = {
		.pc_func = nfsd3_proc_remove,
		.pc_decode = nfs3svc_decode_diropargs,
		.pc_encode = nfs3svc_encode_wccstatres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_diropargs),
		.pc_ressize = sizeof(struct nfsd3_wccstatres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+WC,
<<<<<<< HEAD
=======
		.pc_name = "REMOVE",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_RMDIR] = {
		.pc_func = nfsd3_proc_rmdir,
		.pc_decode = nfs3svc_decode_diropargs,
		.pc_encode = nfs3svc_encode_wccstatres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_diropargs),
		.pc_ressize = sizeof(struct nfsd3_wccstatres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+WC,
<<<<<<< HEAD
=======
		.pc_name = "RMDIR",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_RENAME] = {
		.pc_func = nfsd3_proc_rename,
		.pc_decode = nfs3svc_decode_renameargs,
		.pc_encode = nfs3svc_encode_renameres,
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_renameargs),
		.pc_ressize = sizeof(struct nfsd3_renameres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+WC+WC,
<<<<<<< HEAD
=======
		.pc_name = "RENAME",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_LINK] = {
		.pc_func = nfsd3_proc_link,
		.pc_decode = nfs3svc_decode_linkargs,
		.pc_encode = nfs3svc_encode_linkres,
		.pc_release = nfs3svc_release_fhandle2,
		.pc_argsize = sizeof(struct nfsd3_linkargs),
		.pc_ressize = sizeof(struct nfsd3_linkres),
		.pc_cachetype = RC_REPLBUFF,
		.pc_xdrressize = ST+pAT+WC,
<<<<<<< HEAD
=======
		.pc_name = "LINK",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_READDIR] = {
		.pc_func = nfsd3_proc_readdir,
		.pc_decode = nfs3svc_decode_readdirargs,
		.pc_encode = nfs3svc_encode_readdirres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_readdirargs),
		.pc_ressize = sizeof(struct nfsd3_readdirres),
		.pc_cachetype = RC_NOCACHE,
<<<<<<< HEAD
=======
		.pc_name = "READDIR",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_READDIRPLUS] = {
		.pc_func = nfsd3_proc_readdirplus,
		.pc_decode = nfs3svc_decode_readdirplusargs,
		.pc_encode = nfs3svc_encode_readdirres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_readdirplusargs),
		.pc_ressize = sizeof(struct nfsd3_readdirres),
		.pc_cachetype = RC_NOCACHE,
<<<<<<< HEAD
=======
		.pc_name = "READDIRPLUS",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_FSSTAT] = {
		.pc_func = nfsd3_proc_fsstat,
		.pc_decode = nfs3svc_decode_fhandleargs,
		.pc_encode = nfs3svc_encode_fsstatres,
		.pc_argsize = sizeof(struct nfsd3_fhandleargs),
		.pc_ressize = sizeof(struct nfsd3_fsstatres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+2*6+1,
<<<<<<< HEAD
=======
		.pc_name = "FSSTAT",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_FSINFO] = {
		.pc_func = nfsd3_proc_fsinfo,
		.pc_decode = nfs3svc_decode_fhandleargs,
		.pc_encode = nfs3svc_encode_fsinfores,
		.pc_argsize = sizeof(struct nfsd3_fhandleargs),
		.pc_ressize = sizeof(struct nfsd3_fsinfores),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+12,
<<<<<<< HEAD
=======
		.pc_name = "FSINFO",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_PATHCONF] = {
		.pc_func = nfsd3_proc_pathconf,
		.pc_decode = nfs3svc_decode_fhandleargs,
		.pc_encode = nfs3svc_encode_pathconfres,
		.pc_argsize = sizeof(struct nfsd3_fhandleargs),
		.pc_ressize = sizeof(struct nfsd3_pathconfres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+pAT+6,
<<<<<<< HEAD
=======
		.pc_name = "PATHCONF",
>>>>>>> upstream/android-13
	},
	[NFS3PROC_COMMIT] = {
		.pc_func = nfsd3_proc_commit,
		.pc_decode = nfs3svc_decode_commitargs,
		.pc_encode = nfs3svc_encode_commitres,
		.pc_release = nfs3svc_release_fhandle,
		.pc_argsize = sizeof(struct nfsd3_commitargs),
		.pc_ressize = sizeof(struct nfsd3_commitres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+WC+2,
<<<<<<< HEAD
=======
		.pc_name = "COMMIT",
>>>>>>> upstream/android-13
	},
};

static unsigned int nfsd_count3[ARRAY_SIZE(nfsd_procedures3)];
const struct svc_version nfsd_version3 = {
	.vs_vers	= 3,
	.vs_nproc	= 22,
	.vs_proc	= nfsd_procedures3,
	.vs_dispatch	= nfsd_dispatch,
	.vs_count	= nfsd_count3,
	.vs_xdrsize	= NFS3_SVC_XDRSIZE,
};
