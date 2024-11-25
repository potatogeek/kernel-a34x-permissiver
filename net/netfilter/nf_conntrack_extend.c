<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* Structure dynamic extension infrastructure
 * Copyright (C) 2004 Rusty Russell IBM Corporation
 * Copyright (C) 2007 Netfilter Core Team <coreteam@netfilter.org>
 * Copyright (C) 2007 USAGI/WIDE Project <http://www.linux-ipv6.org>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/kmemleak.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack_extend.h>

static struct nf_ct_ext_type __rcu *nf_ct_ext_types[NF_CT_EXT_NUM];
static DEFINE_MUTEX(nf_ct_ext_type_mutex);
#define NF_CT_EXT_PREALLOC	128u /* conntrack events are on by default */

void nf_ct_ext_destroy(struct nf_conn *ct)
{
	unsigned int i;
	struct nf_ct_ext_type *t;

	for (i = 0; i < NF_CT_EXT_NUM; i++) {
		rcu_read_lock();
		t = rcu_dereference(nf_ct_ext_types[i]);

		/* Here the nf_ct_ext_type might have been unregisterd.
		 * I.e., it has responsible to cleanup private
		 * area in all conntracks when it is unregisterd.
		 */
		if (t && t->destroy)
			t->destroy(ct);
		rcu_read_unlock();
	}
<<<<<<< HEAD
}
EXPORT_SYMBOL(nf_ct_ext_destroy);
=======

	kfree(ct->ext);
}
>>>>>>> upstream/android-13

void *nf_ct_ext_add(struct nf_conn *ct, enum nf_ct_ext_id id, gfp_t gfp)
{
	unsigned int newlen, newoff, oldlen, alloc;
<<<<<<< HEAD
	struct nf_ct_ext *old, *new;
	struct nf_ct_ext_type *t;
=======
	struct nf_ct_ext_type *t;
	struct nf_ct_ext *new;
>>>>>>> upstream/android-13

	/* Conntrack must not be confirmed to avoid races on reallocation. */
	WARN_ON(nf_ct_is_confirmed(ct));

<<<<<<< HEAD
	old = ct->ext;

	if (old) {
=======

	if (ct->ext) {
		const struct nf_ct_ext *old = ct->ext;

>>>>>>> upstream/android-13
		if (__nf_ct_ext_exist(old, id))
			return NULL;
		oldlen = old->len;
	} else {
		oldlen = sizeof(*new);
	}

	rcu_read_lock();
	t = rcu_dereference(nf_ct_ext_types[id]);
	if (!t) {
		rcu_read_unlock();
		return NULL;
	}

	newoff = ALIGN(oldlen, t->align);
	newlen = newoff + t->len;
	rcu_read_unlock();

	alloc = max(newlen, NF_CT_EXT_PREALLOC);
<<<<<<< HEAD
	kmemleak_not_leak(old);
	new = __krealloc(old, alloc, gfp);
	if (!new)
		return NULL;

	if (!old) {
		memset(new->offset, 0, sizeof(new->offset));
		ct->ext = new;
	} else if (new != old) {
		kfree_rcu(old, rcu);
		rcu_assign_pointer(ct->ext, new);
	}
=======
	new = krealloc(ct->ext, alloc, gfp);
	if (!new)
		return NULL;

	if (!ct->ext)
		memset(new->offset, 0, sizeof(new->offset));
>>>>>>> upstream/android-13

	new->offset[id] = newoff;
	new->len = newlen;
	memset((void *)new + newoff, 0, newlen - newoff);
<<<<<<< HEAD
=======

	ct->ext = new;
>>>>>>> upstream/android-13
	return (void *)new + newoff;
}
EXPORT_SYMBOL(nf_ct_ext_add);

/* This MUST be called in process context. */
int nf_ct_extend_register(const struct nf_ct_ext_type *type)
{
	int ret = 0;

	mutex_lock(&nf_ct_ext_type_mutex);
	if (nf_ct_ext_types[type->id]) {
		ret = -EBUSY;
		goto out;
	}

	rcu_assign_pointer(nf_ct_ext_types[type->id], type);
out:
	mutex_unlock(&nf_ct_ext_type_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_extend_register);

/* This MUST be called in process context. */
void nf_ct_extend_unregister(const struct nf_ct_ext_type *type)
{
	mutex_lock(&nf_ct_ext_type_mutex);
	RCU_INIT_POINTER(nf_ct_ext_types[type->id], NULL);
	mutex_unlock(&nf_ct_ext_type_mutex);
	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(nf_ct_extend_unregister);
