// SPDX-License-Identifier: GPL-2.0
/*
 * linux/net/sunrpc/auth_null.c
 *
 * AUTH_NULL authentication. Really :-)
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/sunrpc/clnt.h>

#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
# define RPCDBG_FACILITY	RPCDBG_AUTH
#endif

static struct rpc_auth null_auth;
static struct rpc_cred null_cred;

static struct rpc_auth *
nul_create(const struct rpc_auth_create_args *args, struct rpc_clnt *clnt)
{
<<<<<<< HEAD
	atomic_inc(&null_auth.au_count);
=======
	refcount_inc(&null_auth.au_count);
>>>>>>> upstream/android-13
	return &null_auth;
}

static void
nul_destroy(struct rpc_auth *auth)
{
}

/*
 * Lookup NULL creds for current process
 */
static struct rpc_cred *
nul_lookup_cred(struct rpc_auth *auth, struct auth_cred *acred, int flags)
{
<<<<<<< HEAD
	if (flags & RPCAUTH_LOOKUP_RCU)
		return &null_cred;
=======
>>>>>>> upstream/android-13
	return get_rpccred(&null_cred);
}

/*
 * Destroy cred handle.
 */
static void
nul_destroy_cred(struct rpc_cred *cred)
{
}

/*
 * Match cred handle against current process
 */
static int
nul_match(struct auth_cred *acred, struct rpc_cred *cred, int taskflags)
{
	return 1;
}

/*
 * Marshal credential.
 */
<<<<<<< HEAD
static __be32 *
nul_marshal(struct rpc_task *task, __be32 *p)
{
	*p++ = htonl(RPC_AUTH_NULL);
	*p++ = 0;
	*p++ = htonl(RPC_AUTH_NULL);
	*p++ = 0;

	return p;
=======
static int
nul_marshal(struct rpc_task *task, struct xdr_stream *xdr)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, 4 * sizeof(*p));
	if (!p)
		return -EMSGSIZE;
	/* Credential */
	*p++ = rpc_auth_null;
	*p++ = xdr_zero;
	/* Verifier */
	*p++ = rpc_auth_null;
	*p   = xdr_zero;
	return 0;
>>>>>>> upstream/android-13
}

/*
 * Refresh credential. This is a no-op for AUTH_NULL
 */
static int
nul_refresh(struct rpc_task *task)
{
	set_bit(RPCAUTH_CRED_UPTODATE, &task->tk_rqstp->rq_cred->cr_flags);
	return 0;
}

<<<<<<< HEAD
static __be32 *
nul_validate(struct rpc_task *task, __be32 *p)
{
	rpc_authflavor_t	flavor;
	u32			size;

	flavor = ntohl(*p++);
	if (flavor != RPC_AUTH_NULL) {
		printk("RPC: bad verf flavor: %u\n", flavor);
		return ERR_PTR(-EIO);
	}

	size = ntohl(*p++);
	if (size != 0) {
		printk("RPC: bad verf size: %u\n", size);
		return ERR_PTR(-EIO);
	}

	return p;
=======
static int
nul_validate(struct rpc_task *task, struct xdr_stream *xdr)
{
	__be32 *p;

	p = xdr_inline_decode(xdr, 2 * sizeof(*p));
	if (!p)
		return -EIO;
	if (*p++ != rpc_auth_null)
		return -EIO;
	if (*p != xdr_zero)
		return -EIO;
	return 0;
>>>>>>> upstream/android-13
}

const struct rpc_authops authnull_ops = {
	.owner		= THIS_MODULE,
	.au_flavor	= RPC_AUTH_NULL,
	.au_name	= "NULL",
	.create		= nul_create,
	.destroy	= nul_destroy,
	.lookup_cred	= nul_lookup_cred,
};

static
struct rpc_auth null_auth = {
	.au_cslack	= NUL_CALLSLACK,
	.au_rslack	= NUL_REPLYSLACK,
<<<<<<< HEAD
	.au_flags	= RPCAUTH_AUTH_NO_CRKEY_TIMEOUT,
	.au_ops		= &authnull_ops,
	.au_flavor	= RPC_AUTH_NULL,
	.au_count	= ATOMIC_INIT(0),
=======
	.au_verfsize	= NUL_REPLYSLACK,
	.au_ralign	= NUL_REPLYSLACK,
	.au_ops		= &authnull_ops,
	.au_flavor	= RPC_AUTH_NULL,
	.au_count	= REFCOUNT_INIT(1),
>>>>>>> upstream/android-13
};

static
const struct rpc_credops null_credops = {
	.cr_name	= "AUTH_NULL",
	.crdestroy	= nul_destroy_cred,
<<<<<<< HEAD
	.crbind		= rpcauth_generic_bind_cred,
	.crmatch	= nul_match,
	.crmarshal	= nul_marshal,
	.crrefresh	= nul_refresh,
	.crvalidate	= nul_validate,
=======
	.crmatch	= nul_match,
	.crmarshal	= nul_marshal,
	.crwrap_req	= rpcauth_wrap_req_encode,
	.crrefresh	= nul_refresh,
	.crvalidate	= nul_validate,
	.crunwrap_resp	= rpcauth_unwrap_resp_decode,
>>>>>>> upstream/android-13
};

static
struct rpc_cred null_cred = {
	.cr_lru		= LIST_HEAD_INIT(null_cred.cr_lru),
	.cr_auth	= &null_auth,
	.cr_ops		= &null_credops,
<<<<<<< HEAD
	.cr_count	= ATOMIC_INIT(1),
=======
	.cr_count	= REFCOUNT_INIT(2),
>>>>>>> upstream/android-13
	.cr_flags	= 1UL << RPCAUTH_CRED_UPTODATE,
};
