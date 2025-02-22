// SPDX-License-Identifier: GPL-2.0
/*
 * linux/fs/lockd/xdr4.c
 *
 * XDR support for lockd and the lock client.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 * Copyright (C) 1999, Trond Myklebust <trond.myklebust@fys.uio.no>
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/nfs.h>

#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/stats.h>
#include <linux/lockd/lockd.h>

<<<<<<< HEAD
#define NLMDBG_FACILITY		NLMDBG_XDR
=======
#include "svcxdr.h"
>>>>>>> upstream/android-13

static inline loff_t
s64_to_loff_t(__s64 offset)
{
	return (loff_t)offset;
}


static inline s64
loff_t_to_s64(loff_t offset)
{
	s64 res;
	if (offset > NLM4_OFFSET_MAX)
		res = NLM4_OFFSET_MAX;
	else if (offset < -NLM4_OFFSET_MAX)
		res = -NLM4_OFFSET_MAX;
	else
		res = offset;
	return res;
}

/*
<<<<<<< HEAD
 * XDR functions for basic NLM types
 */
static __be32 *
nlm4_decode_cookie(__be32 *p, struct nlm_cookie *c)
{
	unsigned int	len;

	len = ntohl(*p++);
	
	if(len==0)
	{
		c->len=4;
		memset(c->data, 0, 4);	/* hockeypux brain damage */
	}
	else if(len<=NLM_MAXCOOKIELEN)
	{
		c->len=len;
		memcpy(c->data, p, len);
		p+=XDR_QUADLEN(len);
	}
	else 
	{
		dprintk("lockd: bad cookie size %d (only cookies under "
			"%d bytes are supported.)\n",
				len, NLM_MAXCOOKIELEN);
		return NULL;
	}
	return p;
}

static __be32 *
nlm4_encode_cookie(__be32 *p, struct nlm_cookie *c)
{
	*p++ = htonl(c->len);
	memcpy(p, c->data, c->len);
	p+=XDR_QUADLEN(c->len);
	return p;
}

static __be32 *
nlm4_decode_fh(__be32 *p, struct nfs_fh *f)
{
	memset(f->data, 0, sizeof(f->data));
	f->size = ntohl(*p++);
	if (f->size > NFS_MAXFHSIZE) {
		dprintk("lockd: bad fhandle size %d (should be <=%d)\n",
			f->size, NFS_MAXFHSIZE);
		return NULL;
	}
      	memcpy(f->data, p, f->size);
	return p + XDR_QUADLEN(f->size);
}

/*
 * Encode and decode owner handle
 */
static __be32 *
nlm4_decode_oh(__be32 *p, struct xdr_netobj *oh)
{
	return xdr_decode_netobj(p, oh);
}

static __be32 *
nlm4_decode_lock(__be32 *p, struct nlm_lock *lock)
{
	struct file_lock	*fl = &lock->fl;
	__u64			len, start;
	__s64			end;

	if (!(p = xdr_decode_string_inplace(p, &lock->caller,
					    &lock->len, NLM_MAXSTRLEN))
	 || !(p = nlm4_decode_fh(p, &lock->fh))
	 || !(p = nlm4_decode_oh(p, &lock->oh)))
		return NULL;
	lock->svid  = ntohl(*p++);

	locks_init_lock(fl);
	fl->fl_owner = current->files;
	fl->fl_pid   = (pid_t)lock->svid;
	fl->fl_flags = FL_POSIX;
	fl->fl_type  = F_RDLCK;		/* as good as anything else */
	p = xdr_decode_hyper(p, &start);
	p = xdr_decode_hyper(p, &len);
	end = start + len - 1;

	fl->fl_start = s64_to_loff_t(start);

=======
 * NLM file handles are defined by specification to be a variable-length
 * XDR opaque no longer than 1024 bytes. However, this implementation
 * limits their length to the size of an NFSv3 file handle.
 */
static bool
svcxdr_decode_fhandle(struct xdr_stream *xdr, struct nfs_fh *fh)
{
	__be32 *p;
	u32 len;

	if (xdr_stream_decode_u32(xdr, &len) < 0)
		return false;
	if (len > NFS_MAXFHSIZE)
		return false;

	p = xdr_inline_decode(xdr, len);
	if (!p)
		return false;
	fh->size = len;
	memcpy(fh->data, p, len);
	memset(fh->data + len, 0, sizeof(fh->data) - len);

	return true;
}

static bool
svcxdr_decode_lock(struct xdr_stream *xdr, struct nlm_lock *lock)
{
	struct file_lock *fl = &lock->fl;
	u64 len, start;
	s64 end;

	if (!svcxdr_decode_string(xdr, &lock->caller, &lock->len))
		return false;
	if (!svcxdr_decode_fhandle(xdr, &lock->fh))
		return false;
	if (!svcxdr_decode_owner(xdr, &lock->oh))
		return false;
	if (xdr_stream_decode_u32(xdr, &lock->svid) < 0)
		return false;
	if (xdr_stream_decode_u64(xdr, &start) < 0)
		return false;
	if (xdr_stream_decode_u64(xdr, &len) < 0)
		return false;

	locks_init_lock(fl);
	fl->fl_flags = FL_POSIX;
	fl->fl_type  = F_RDLCK;
	end = start + len - 1;
	fl->fl_start = s64_to_loff_t(start);
>>>>>>> upstream/android-13
	if (len == 0 || end < 0)
		fl->fl_end = OFFSET_MAX;
	else
		fl->fl_end = s64_to_loff_t(end);
<<<<<<< HEAD
	return p;
}

/*
 * Encode result of a TEST/TEST_MSG call
 */
static __be32 *
nlm4_encode_testres(__be32 *p, struct nlm_res *resp)
{
	s64		start, len;

	dprintk("xdr: before encode_testres (p %p resp %p)\n", p, resp);
	if (!(p = nlm4_encode_cookie(p, &resp->cookie)))
		return NULL;
	*p++ = resp->status;

	if (resp->status == nlm_lck_denied) {
		struct file_lock	*fl = &resp->lock.fl;

		*p++ = (fl->fl_type == F_RDLCK)? xdr_zero : xdr_one;
		*p++ = htonl(resp->lock.svid);

		/* Encode owner handle. */
		if (!(p = xdr_encode_netobj(p, &resp->lock.oh)))
			return NULL;

		start = loff_t_to_s64(fl->fl_start);
		if (fl->fl_end == OFFSET_MAX)
			len = 0;
		else
			len = loff_t_to_s64(fl->fl_end - fl->fl_start + 1);
		
		p = xdr_encode_hyper(p, start);
		p = xdr_encode_hyper(p, len);
		dprintk("xdr: encode_testres (status %u pid %d type %d start %Ld end %Ld)\n",
			resp->status, (int)resp->lock.svid, fl->fl_type,
			(long long)fl->fl_start,  (long long)fl->fl_end);
	}

	dprintk("xdr: after encode_testres (p %p resp %p)\n", p, resp);
	return p;
=======

	return true;
}

static bool
svcxdr_encode_holder(struct xdr_stream *xdr, const struct nlm_lock *lock)
{
	const struct file_lock *fl = &lock->fl;
	s64 start, len;

	/* exclusive */
	if (xdr_stream_encode_bool(xdr, fl->fl_type != F_RDLCK) < 0)
		return false;
	if (xdr_stream_encode_u32(xdr, lock->svid) < 0)
		return false;
	if (!svcxdr_encode_owner(xdr, &lock->oh))
		return false;
	start = loff_t_to_s64(fl->fl_start);
	if (fl->fl_end == OFFSET_MAX)
		len = 0;
	else
		len = loff_t_to_s64(fl->fl_end - fl->fl_start + 1);
	if (xdr_stream_encode_u64(xdr, start) < 0)
		return false;
	if (xdr_stream_encode_u64(xdr, len) < 0)
		return false;

	return true;
}

static bool
svcxdr_encode_testrply(struct xdr_stream *xdr, const struct nlm_res *resp)
{
	if (!svcxdr_encode_stats(xdr, resp->status))
		return false;
	switch (resp->status) {
	case nlm_lck_denied:
		if (!svcxdr_encode_holder(xdr, &resp->lock))
			return false;
	}

	return true;
>>>>>>> upstream/android-13
}


/*
<<<<<<< HEAD
 * First, the server side XDR functions
 */
int
nlm4svc_decode_testargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_args *argp = rqstp->rq_argp;
	u32	exclusive;

	if (!(p = nlm4_decode_cookie(p, &argp->cookie)))
		return 0;

	exclusive = ntohl(*p++);
	if (!(p = nlm4_decode_lock(p, &argp->lock)))
=======
 * Decode Call arguments
 */

int
nlm4svc_decode_void(struct svc_rqst *rqstp, __be32 *p)
{
	return 1;
}

int
nlm4svc_decode_testargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_args *argp = rqstp->rq_argp;
	u32 exclusive;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return 0;
	if (xdr_stream_decode_bool(xdr, &exclusive) < 0)
		return 0;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
>>>>>>> upstream/android-13
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;

<<<<<<< HEAD
	return xdr_argsize_check(rqstp, p);
}

int
nlm4svc_encode_testres(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_res *resp = rqstp->rq_resp;

	if (!(p = nlm4_encode_testres(p, resp)))
		return 0;
	return xdr_ressize_check(rqstp, p);
=======
	return 1;
>>>>>>> upstream/android-13
}

int
nlm4svc_decode_lockargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nlm_args *argp = rqstp->rq_argp;
	u32	exclusive;

	if (!(p = nlm4_decode_cookie(p, &argp->cookie)))
		return 0;
	argp->block  = ntohl(*p++);
	exclusive    = ntohl(*p++);
	if (!(p = nlm4_decode_lock(p, &argp->lock)))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;
	argp->reclaim = ntohl(*p++);
	argp->state   = ntohl(*p++);
	argp->monitor = 1;		/* monitor client by default */

	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_args *argp = rqstp->rq_argp;
	u32 exclusive;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return 0;
	if (xdr_stream_decode_bool(xdr, &argp->block) < 0)
		return 0;
	if (xdr_stream_decode_bool(xdr, &exclusive) < 0)
		return 0;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;
	if (xdr_stream_decode_bool(xdr, &argp->reclaim) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &argp->state) < 0)
		return 0;
	argp->monitor = 1;		/* monitor client by default */

	return 1;
>>>>>>> upstream/android-13
}

int
nlm4svc_decode_cancargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nlm_args *argp = rqstp->rq_argp;
	u32	exclusive;

	if (!(p = nlm4_decode_cookie(p, &argp->cookie)))
		return 0;
	argp->block = ntohl(*p++);
	exclusive = ntohl(*p++);
	if (!(p = nlm4_decode_lock(p, &argp->lock)))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;
	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_args *argp = rqstp->rq_argp;
	u32 exclusive;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return 0;
	if (xdr_stream_decode_bool(xdr, &argp->block) < 0)
		return 0;
	if (xdr_stream_decode_bool(xdr, &exclusive) < 0)
		return 0;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;
	return 1;
>>>>>>> upstream/android-13
}

int
nlm4svc_decode_unlockargs(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nlm_args *argp = rqstp->rq_argp;

	if (!(p = nlm4_decode_cookie(p, &argp->cookie))
	 || !(p = nlm4_decode_lock(p, &argp->lock)))
		return 0;
	argp->lock.fl.fl_type = F_UNLCK;
	return xdr_argsize_check(rqstp, p);
}

int
nlm4svc_decode_shareargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_args *argp = rqstp->rq_argp;
	struct nlm_lock	*lock = &argp->lock;

	memset(lock, 0, sizeof(*lock));
	locks_init_lock(&lock->fl);
	lock->svid = ~(u32) 0;
	lock->fl.fl_pid = (pid_t)lock->svid;

	if (!(p = nlm4_decode_cookie(p, &argp->cookie))
	 || !(p = xdr_decode_string_inplace(p, &lock->caller,
					    &lock->len, NLM_MAXSTRLEN))
	 || !(p = nlm4_decode_fh(p, &lock->fh))
	 || !(p = nlm4_decode_oh(p, &lock->oh)))
		return 0;
	argp->fsm_mode = ntohl(*p++);
	argp->fsm_access = ntohl(*p++);
	return xdr_argsize_check(rqstp, p);
}

int
nlm4svc_encode_shareres(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_res *resp = rqstp->rq_resp;

	if (!(p = nlm4_encode_cookie(p, &resp->cookie)))
		return 0;
	*p++ = resp->status;
	*p++ = xdr_zero;		/* sequence argument */
	return xdr_ressize_check(rqstp, p);
}

int
nlm4svc_encode_res(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_res *resp = rqstp->rq_resp;

	if (!(p = nlm4_encode_cookie(p, &resp->cookie)))
		return 0;
	*p++ = resp->status;
	return xdr_ressize_check(rqstp, p);
}

int
nlm4svc_decode_notify(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_args *argp = rqstp->rq_argp;
	struct nlm_lock	*lock = &argp->lock;

	if (!(p = xdr_decode_string_inplace(p, &lock->caller,
					    &lock->len, NLM_MAXSTRLEN)))
		return 0;
	argp->state = ntohl(*p++);
	return xdr_argsize_check(rqstp, p);
}

int
nlm4svc_decode_reboot(struct svc_rqst *rqstp, __be32 *p)
{
	struct nlm_reboot *argp = rqstp->rq_argp;

	if (!(p = xdr_decode_string_inplace(p, &argp->mon, &argp->len, SM_MAXSTRLEN)))
		return 0;
	argp->state = ntohl(*p++);
	memcpy(&argp->priv.data, p, sizeof(argp->priv.data));
	p += XDR_QUADLEN(SM_PRIV_SIZE);
	return xdr_argsize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_args *argp = rqstp->rq_argp;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return 0;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return 0;
	argp->lock.fl.fl_type = F_UNLCK;

	return 1;
>>>>>>> upstream/android-13
}

int
nlm4svc_decode_res(struct svc_rqst *rqstp, __be32 *p)
{
<<<<<<< HEAD
	struct nlm_res *resp = rqstp->rq_argp;

	if (!(p = nlm4_decode_cookie(p, &resp->cookie)))
		return 0;
	resp->status = *p++;
	return xdr_argsize_check(rqstp, p);
}

int
nlm4svc_decode_void(struct svc_rqst *rqstp, __be32 *p)
{
	return xdr_argsize_check(rqstp, p);
}

int
nlm4svc_encode_void(struct svc_rqst *rqstp, __be32 *p)
{
	return xdr_ressize_check(rqstp, p);
=======
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_res *resp = rqstp->rq_argp;

	if (!svcxdr_decode_cookie(xdr, &resp->cookie))
		return 0;
	if (!svcxdr_decode_stats(xdr, &resp->status))
		return 0;

	return 1;
}

int
nlm4svc_decode_reboot(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_reboot *argp = rqstp->rq_argp;
	u32 len;

	if (xdr_stream_decode_u32(xdr, &len) < 0)
		return 0;
	if (len > SM_MAXSTRLEN)
		return 0;
	p = xdr_inline_decode(xdr, len);
	if (!p)
		return 0;
	argp->len = len;
	argp->mon = (char *)p;
	if (xdr_stream_decode_u32(xdr, &argp->state) < 0)
		return 0;
	p = xdr_inline_decode(xdr, SM_PRIV_SIZE);
	if (!p)
		return 0;
	memcpy(&argp->priv.data, p, sizeof(argp->priv.data));

	return 1;
}

int
nlm4svc_decode_shareargs(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_args *argp = rqstp->rq_argp;
	struct nlm_lock	*lock = &argp->lock;

	memset(lock, 0, sizeof(*lock));
	locks_init_lock(&lock->fl);
	lock->svid = ~(u32)0;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return 0;
	if (!svcxdr_decode_string(xdr, &lock->caller, &lock->len))
		return 0;
	if (!svcxdr_decode_fhandle(xdr, &lock->fh))
		return 0;
	if (!svcxdr_decode_owner(xdr, &lock->oh))
		return 0;
	/* XXX: Range checks are missing in the original code */
	if (xdr_stream_decode_u32(xdr, &argp->fsm_mode) < 0)
		return 0;
	if (xdr_stream_decode_u32(xdr, &argp->fsm_access) < 0)
		return 0;

	return 1;
}

int
nlm4svc_decode_notify(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_arg_stream;
	struct nlm_args *argp = rqstp->rq_argp;
	struct nlm_lock	*lock = &argp->lock;

	if (!svcxdr_decode_string(xdr, &lock->caller, &lock->len))
		return 0;
	if (xdr_stream_decode_u32(xdr, &argp->state) < 0)
		return 0;

	return 1;
}


/*
 * Encode Reply results
 */

int
nlm4svc_encode_void(struct svc_rqst *rqstp, __be32 *p)
{
	return 1;
}

int
nlm4svc_encode_testres(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nlm_res *resp = rqstp->rq_resp;

	return svcxdr_encode_cookie(xdr, &resp->cookie) &&
		svcxdr_encode_testrply(xdr, resp);
}

int
nlm4svc_encode_res(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nlm_res *resp = rqstp->rq_resp;

	return svcxdr_encode_cookie(xdr, &resp->cookie) &&
		svcxdr_encode_stats(xdr, resp->status);
}

int
nlm4svc_encode_shareres(struct svc_rqst *rqstp, __be32 *p)
{
	struct xdr_stream *xdr = &rqstp->rq_res_stream;
	struct nlm_res *resp = rqstp->rq_resp;

	if (!svcxdr_encode_cookie(xdr, &resp->cookie))
		return 0;
	if (!svcxdr_encode_stats(xdr, resp->status))
		return 0;
	/* sequence */
	if (xdr_stream_encode_u32(xdr, 0) < 0)
		return 0;

	return 1;
>>>>>>> upstream/android-13
}
