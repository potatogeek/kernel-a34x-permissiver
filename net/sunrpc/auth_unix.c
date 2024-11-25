// SPDX-License-Identifier: GPL-2.0
/*
 * linux/net/sunrpc/auth_unix.c
 *
 * UNIX-style authentication; no AUTH_SHORT support
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/mempool.h>
>>>>>>> upstream/android-13
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/auth.h>
#include <linux/user_namespace.h>

<<<<<<< HEAD
struct unx_cred {
	struct rpc_cred		uc_base;
	kgid_t			uc_gid;
	kgid_t			uc_gids[UNX_NGROUPS];
};
#define uc_uid			uc_base.cr_uid
=======
>>>>>>> upstream/android-13

#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
# define RPCDBG_FACILITY	RPCDBG_AUTH
#endif

static struct rpc_auth		unix_auth;
static const struct rpc_credops	unix_credops;
<<<<<<< HEAD
=======
static mempool_t		*unix_pool;
>>>>>>> upstream/android-13

static struct rpc_auth *
unx_create(const struct rpc_auth_create_args *args, struct rpc_clnt *clnt)
{
<<<<<<< HEAD
	dprintk("RPC:       creating UNIX authenticator for client %p\n",
			clnt);
	atomic_inc(&unix_auth.au_count);
=======
	refcount_inc(&unix_auth.au_count);
>>>>>>> upstream/android-13
	return &unix_auth;
}

static void
unx_destroy(struct rpc_auth *auth)
{
<<<<<<< HEAD
	dprintk("RPC:       destroying UNIX authenticator %p\n", auth);
	rpcauth_clear_credcache(auth->au_credcache);
}

static int
unx_hash_cred(struct auth_cred *acred, unsigned int hashbits)
{
	return hash_64(from_kgid(&init_user_ns, acred->gid) |
		((u64)from_kuid(&init_user_ns, acred->uid) <<
			(sizeof(gid_t) * 8)), hashbits);
=======
>>>>>>> upstream/android-13
}

/*
 * Lookup AUTH_UNIX creds for current process
 */
static struct rpc_cred *
unx_lookup_cred(struct rpc_auth *auth, struct auth_cred *acred, int flags)
{
<<<<<<< HEAD
	return rpcauth_lookup_credcache(auth, acred, flags, GFP_NOFS);
}

static struct rpc_cred *
unx_create_cred(struct rpc_auth *auth, struct auth_cred *acred, int flags, gfp_t gfp)
{
	struct unx_cred	*cred;
	unsigned int groups = 0;
	unsigned int i;

	dprintk("RPC:       allocating UNIX cred for uid %d gid %d\n",
			from_kuid(&init_user_ns, acred->uid),
			from_kgid(&init_user_ns, acred->gid));

	if (!(cred = kmalloc(sizeof(*cred), gfp)))
		return ERR_PTR(-ENOMEM);

	rpcauth_init_cred(&cred->uc_base, acred, auth, &unix_credops);
	cred->uc_base.cr_flags = 1UL << RPCAUTH_CRED_UPTODATE;

	if (acred->group_info != NULL)
		groups = acred->group_info->ngroups;
	if (groups > UNX_NGROUPS)
		groups = UNX_NGROUPS;

	cred->uc_gid = acred->gid;
	for (i = 0; i < groups; i++)
		cred->uc_gids[i] = acred->group_info->gid[i];
	if (i < UNX_NGROUPS)
		cred->uc_gids[i] = INVALID_GID;

	return &cred->uc_base;
}

static void
unx_free_cred(struct unx_cred *unx_cred)
{
	dprintk("RPC:       unx_free_cred %p\n", unx_cred);
	kfree(unx_cred);
=======
	struct rpc_cred *ret = mempool_alloc(unix_pool, GFP_NOFS);

	rpcauth_init_cred(ret, acred, auth, &unix_credops);
	ret->cr_flags = 1UL << RPCAUTH_CRED_UPTODATE;
	return ret;
>>>>>>> upstream/android-13
}

static void
unx_free_cred_callback(struct rcu_head *head)
{
<<<<<<< HEAD
	struct unx_cred *unx_cred = container_of(head, struct unx_cred, uc_base.cr_rcu);
	unx_free_cred(unx_cred);
=======
	struct rpc_cred *rpc_cred = container_of(head, struct rpc_cred, cr_rcu);

	put_cred(rpc_cred->cr_cred);
	mempool_free(rpc_cred, unix_pool);
>>>>>>> upstream/android-13
}

static void
unx_destroy_cred(struct rpc_cred *cred)
{
	call_rcu(&cred->cr_rcu, unx_free_cred_callback);
}

/*
<<<<<<< HEAD
 * Match credentials against current process creds.
 * The root_override argument takes care of cases where the caller may
 * request root creds (e.g. for NFS swapping).
 */
static int
unx_match(struct auth_cred *acred, struct rpc_cred *rcred, int flags)
{
	struct unx_cred	*cred = container_of(rcred, struct unx_cred, uc_base);
	unsigned int groups = 0;
	unsigned int i;


	if (!uid_eq(cred->uc_uid, acred->uid) || !gid_eq(cred->uc_gid, acred->gid))
		return 0;

	if (acred->group_info != NULL)
		groups = acred->group_info->ngroups;
	if (groups > UNX_NGROUPS)
		groups = UNX_NGROUPS;
	for (i = 0; i < groups ; i++)
		if (!gid_eq(cred->uc_gids[i], acred->group_info->gid[i]))
			return 0;
	if (groups < UNX_NGROUPS && gid_valid(cred->uc_gids[groups]))
		return 0;
=======
 * Match credentials against current the auth_cred.
 */
static int
unx_match(struct auth_cred *acred, struct rpc_cred *cred, int flags)
{
	unsigned int groups = 0;
	unsigned int i;

	if (cred->cr_cred == acred->cred)
		return 1;

	if (!uid_eq(cred->cr_cred->fsuid, acred->cred->fsuid) || !gid_eq(cred->cr_cred->fsgid, acred->cred->fsgid))
		return 0;

	if (acred->cred->group_info != NULL)
		groups = acred->cred->group_info->ngroups;
	if (groups > UNX_NGROUPS)
		groups = UNX_NGROUPS;
	if (cred->cr_cred->group_info == NULL)
		return groups == 0;
	if (groups != cred->cr_cred->group_info->ngroups)
		return 0;

	for (i = 0; i < groups ; i++)
		if (!gid_eq(cred->cr_cred->group_info->gid[i], acred->cred->group_info->gid[i]))
			return 0;
>>>>>>> upstream/android-13
	return 1;
}

/*
 * Marshal credentials.
 * Maybe we should keep a cached credential for performance reasons.
 */
<<<<<<< HEAD
static __be32 *
unx_marshal(struct rpc_task *task, __be32 *p)
{
	struct rpc_clnt	*clnt = task->tk_client;
	struct unx_cred	*cred = container_of(task->tk_rqstp->rq_cred, struct unx_cred, uc_base);
	__be32		*base, *hold;
	int		i;

	*p++ = htonl(RPC_AUTH_UNIX);
	base = p++;
	*p++ = htonl(jiffies/HZ);

	/*
	 * Copy the UTS nodename captured when the client was created.
	 */
	p = xdr_encode_array(p, clnt->cl_nodename, clnt->cl_nodelen);

	*p++ = htonl((u32) from_kuid(&init_user_ns, cred->uc_uid));
	*p++ = htonl((u32) from_kgid(&init_user_ns, cred->uc_gid));
	hold = p++;
	for (i = 0; i < UNX_NGROUPS && gid_valid(cred->uc_gids[i]); i++)
		*p++ = htonl((u32) from_kgid(&init_user_ns, cred->uc_gids[i]));
	*hold = htonl(p - hold - 1);		/* gid array length */
	*base = htonl((p - base - 1) << 2);	/* cred length */

	*p++ = htonl(RPC_AUTH_NULL);
	*p++ = htonl(0);

	return p;
=======
static int
unx_marshal(struct rpc_task *task, struct xdr_stream *xdr)
{
	struct rpc_clnt	*clnt = task->tk_client;
	struct rpc_cred	*cred = task->tk_rqstp->rq_cred;
	__be32		*p, *cred_len, *gidarr_len;
	int		i;
	struct group_info *gi = cred->cr_cred->group_info;
	struct user_namespace *userns = clnt->cl_cred ?
		clnt->cl_cred->user_ns : &init_user_ns;

	/* Credential */

	p = xdr_reserve_space(xdr, 3 * sizeof(*p));
	if (!p)
		goto marshal_failed;
	*p++ = rpc_auth_unix;
	cred_len = p++;
	*p++ = xdr_zero;	/* stamp */
	if (xdr_stream_encode_opaque(xdr, clnt->cl_nodename,
				     clnt->cl_nodelen) < 0)
		goto marshal_failed;
	p = xdr_reserve_space(xdr, 3 * sizeof(*p));
	if (!p)
		goto marshal_failed;
	*p++ = cpu_to_be32(from_kuid_munged(userns, cred->cr_cred->fsuid));
	*p++ = cpu_to_be32(from_kgid_munged(userns, cred->cr_cred->fsgid));

	gidarr_len = p++;
	if (gi)
		for (i = 0; i < UNX_NGROUPS && i < gi->ngroups; i++)
			*p++ = cpu_to_be32(from_kgid_munged(userns, gi->gid[i]));
	*gidarr_len = cpu_to_be32(p - gidarr_len - 1);
	*cred_len = cpu_to_be32((p - cred_len - 1) << 2);
	p = xdr_reserve_space(xdr, (p - gidarr_len - 1) << 2);
	if (!p)
		goto marshal_failed;

	/* Verifier */

	p = xdr_reserve_space(xdr, 2 * sizeof(*p));
	if (!p)
		goto marshal_failed;
	*p++ = rpc_auth_null;
	*p   = xdr_zero;

	return 0;

marshal_failed:
	return -EMSGSIZE;
>>>>>>> upstream/android-13
}

/*
 * Refresh credentials. This is a no-op for AUTH_UNIX
 */
static int
unx_refresh(struct rpc_task *task)
{
	set_bit(RPCAUTH_CRED_UPTODATE, &task->tk_rqstp->rq_cred->cr_flags);
	return 0;
}

<<<<<<< HEAD
static __be32 *
unx_validate(struct rpc_task *task, __be32 *p)
{
	rpc_authflavor_t	flavor;
	u32			size;

	flavor = ntohl(*p++);
	if (flavor != RPC_AUTH_NULL &&
	    flavor != RPC_AUTH_UNIX &&
	    flavor != RPC_AUTH_SHORT) {
		printk("RPC: bad verf flavor: %u\n", flavor);
		return ERR_PTR(-EIO);
	}

	size = ntohl(*p++);
	if (size > RPC_MAX_AUTH_SIZE) {
		printk("RPC: giant verf size: %u\n", size);
		return ERR_PTR(-EIO);
	}
	task->tk_rqstp->rq_cred->cr_auth->au_rslack = (size >> 2) + 2;
	p += (size >> 2);

	return p;
=======
static int
unx_validate(struct rpc_task *task, struct xdr_stream *xdr)
{
	struct rpc_auth *auth = task->tk_rqstp->rq_cred->cr_auth;
	__be32 *p;
	u32 size;

	p = xdr_inline_decode(xdr, 2 * sizeof(*p));
	if (!p)
		return -EIO;
	switch (*p++) {
	case rpc_auth_null:
	case rpc_auth_unix:
	case rpc_auth_short:
		break;
	default:
		return -EIO;
	}
	size = be32_to_cpup(p);
	if (size > RPC_MAX_AUTH_SIZE)
		return -EIO;
	p = xdr_inline_decode(xdr, size);
	if (!p)
		return -EIO;

	auth->au_verfsize = XDR_QUADLEN(size) + 2;
	auth->au_rslack = XDR_QUADLEN(size) + 2;
	auth->au_ralign = XDR_QUADLEN(size) + 2;
	return 0;
>>>>>>> upstream/android-13
}

int __init rpc_init_authunix(void)
{
<<<<<<< HEAD
	return rpcauth_init_credcache(&unix_auth);
=======
	unix_pool = mempool_create_kmalloc_pool(16, sizeof(struct rpc_cred));
	return unix_pool ? 0 : -ENOMEM;
>>>>>>> upstream/android-13
}

void rpc_destroy_authunix(void)
{
<<<<<<< HEAD
	rpcauth_destroy_credcache(&unix_auth);
=======
	mempool_destroy(unix_pool);
>>>>>>> upstream/android-13
}

const struct rpc_authops authunix_ops = {
	.owner		= THIS_MODULE,
	.au_flavor	= RPC_AUTH_UNIX,
	.au_name	= "UNIX",
	.create		= unx_create,
	.destroy	= unx_destroy,
<<<<<<< HEAD
	.hash_cred	= unx_hash_cred,
	.lookup_cred	= unx_lookup_cred,
	.crcreate	= unx_create_cred,
=======
	.lookup_cred	= unx_lookup_cred,
>>>>>>> upstream/android-13
};

static
struct rpc_auth		unix_auth = {
	.au_cslack	= UNX_CALLSLACK,
	.au_rslack	= NUL_REPLYSLACK,
<<<<<<< HEAD
	.au_flags	= RPCAUTH_AUTH_NO_CRKEY_TIMEOUT,
	.au_ops		= &authunix_ops,
	.au_flavor	= RPC_AUTH_UNIX,
	.au_count	= ATOMIC_INIT(0),
=======
	.au_verfsize	= NUL_REPLYSLACK,
	.au_ops		= &authunix_ops,
	.au_flavor	= RPC_AUTH_UNIX,
	.au_count	= REFCOUNT_INIT(1),
>>>>>>> upstream/android-13
};

static
const struct rpc_credops unix_credops = {
	.cr_name	= "AUTH_UNIX",
	.crdestroy	= unx_destroy_cred,
<<<<<<< HEAD
	.crbind		= rpcauth_generic_bind_cred,
	.crmatch	= unx_match,
	.crmarshal	= unx_marshal,
	.crrefresh	= unx_refresh,
	.crvalidate	= unx_validate,
=======
	.crmatch	= unx_match,
	.crmarshal	= unx_marshal,
	.crwrap_req	= rpcauth_wrap_req_encode,
	.crrefresh	= unx_refresh,
	.crvalidate	= unx_validate,
	.crunwrap_resp	= rpcauth_unwrap_resp_decode,
>>>>>>> upstream/android-13
};
