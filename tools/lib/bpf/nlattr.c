<<<<<<< HEAD
// SPDX-License-Identifier: LGPL-2.1
=======
// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
>>>>>>> upstream/android-13

/*
 * NETLINK      Netlink attributes
 *
<<<<<<< HEAD
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
=======
>>>>>>> upstream/android-13
 * Copyright (c) 2003-2013 Thomas Graf <tgraf@suug.ch>
 */

#include <errno.h>
<<<<<<< HEAD
#include "nlattr.h"
#include <linux/rtnetlink.h>
#include <string.h>
#include <stdio.h>

static uint16_t nla_attr_minlen[NLA_TYPE_MAX+1] = {
	[NLA_U8]	= sizeof(uint8_t),
	[NLA_U16]	= sizeof(uint16_t),
	[NLA_U32]	= sizeof(uint32_t),
	[NLA_U64]	= sizeof(uint64_t),
	[NLA_STRING]	= 1,
	[NLA_FLAG]	= 0,
};

static int nla_len(const struct nlattr *nla)
{
	return nla->nla_len - NLA_HDRLEN;
}

=======
#include <string.h>
#include <stdio.h>
#include <linux/rtnetlink.h>
#include "nlattr.h"
#include "libbpf_internal.h"

static uint16_t nla_attr_minlen[LIBBPF_NLA_TYPE_MAX+1] = {
	[LIBBPF_NLA_U8]		= sizeof(uint8_t),
	[LIBBPF_NLA_U16]	= sizeof(uint16_t),
	[LIBBPF_NLA_U32]	= sizeof(uint32_t),
	[LIBBPF_NLA_U64]	= sizeof(uint64_t),
	[LIBBPF_NLA_STRING]	= 1,
	[LIBBPF_NLA_FLAG]	= 0,
};

>>>>>>> upstream/android-13
static struct nlattr *nla_next(const struct nlattr *nla, int *remaining)
{
	int totlen = NLA_ALIGN(nla->nla_len);

	*remaining -= totlen;
<<<<<<< HEAD
	return (struct nlattr *) ((char *) nla + totlen);
=======
	return (struct nlattr *)((void *)nla + totlen);
>>>>>>> upstream/android-13
}

static int nla_ok(const struct nlattr *nla, int remaining)
{
	return remaining >= sizeof(*nla) &&
	       nla->nla_len >= sizeof(*nla) &&
	       nla->nla_len <= remaining;
}

<<<<<<< HEAD
static void *nla_data(const struct nlattr *nla)
{
	return (char *) nla + NLA_HDRLEN;
}

=======
>>>>>>> upstream/android-13
static int nla_type(const struct nlattr *nla)
{
	return nla->nla_type & NLA_TYPE_MASK;
}

static int validate_nla(struct nlattr *nla, int maxtype,
<<<<<<< HEAD
			struct nla_policy *policy)
{
	struct nla_policy *pt;
=======
			struct libbpf_nla_policy *policy)
{
	struct libbpf_nla_policy *pt;
>>>>>>> upstream/android-13
	unsigned int minlen = 0;
	int type = nla_type(nla);

	if (type < 0 || type > maxtype)
		return 0;

	pt = &policy[type];

<<<<<<< HEAD
	if (pt->type > NLA_TYPE_MAX)
=======
	if (pt->type > LIBBPF_NLA_TYPE_MAX)
>>>>>>> upstream/android-13
		return 0;

	if (pt->minlen)
		minlen = pt->minlen;
<<<<<<< HEAD
	else if (pt->type != NLA_UNSPEC)
		minlen = nla_attr_minlen[pt->type];

	if (nla_len(nla) < minlen)
		return -1;

	if (pt->maxlen && nla_len(nla) > pt->maxlen)
		return -1;

	if (pt->type == NLA_STRING) {
		char *data = nla_data(nla);
		if (data[nla_len(nla) - 1] != '\0')
=======
	else if (pt->type != LIBBPF_NLA_UNSPEC)
		minlen = nla_attr_minlen[pt->type];

	if (libbpf_nla_len(nla) < minlen)
		return -1;

	if (pt->maxlen && libbpf_nla_len(nla) > pt->maxlen)
		return -1;

	if (pt->type == LIBBPF_NLA_STRING) {
		char *data = libbpf_nla_data(nla);

		if (data[libbpf_nla_len(nla) - 1] != '\0')
>>>>>>> upstream/android-13
			return -1;
	}

	return 0;
}

static inline int nlmsg_len(const struct nlmsghdr *nlh)
{
	return nlh->nlmsg_len - NLMSG_HDRLEN;
}

/**
 * Create attribute index based on a stream of attributes.
 * @arg tb		Index array to be filled (maxtype+1 elements).
 * @arg maxtype		Maximum attribute type expected and accepted.
 * @arg head		Head of attribute stream.
 * @arg len		Length of attribute stream.
 * @arg policy		Attribute validation policy.
 *
 * Iterates over the stream of attributes and stores a pointer to each
 * attribute in the index array using the attribute type as index to
 * the array. Attribute with a type greater than the maximum type
 * specified will be silently ignored in order to maintain backwards
 * compatibility. If \a policy is not NULL, the attribute will be
 * validated using the specified policy.
 *
 * @see nla_validate
 * @return 0 on success or a negative error code.
 */
<<<<<<< HEAD
static int nla_parse(struct nlattr *tb[], int maxtype, struct nlattr *head, int len,
		     struct nla_policy *policy)
=======
int libbpf_nla_parse(struct nlattr *tb[], int maxtype, struct nlattr *head,
		     int len, struct libbpf_nla_policy *policy)
>>>>>>> upstream/android-13
{
	struct nlattr *nla;
	int rem, err;

	memset(tb, 0, sizeof(struct nlattr *) * (maxtype + 1));

<<<<<<< HEAD
	nla_for_each_attr(nla, head, len, rem) {
=======
	libbpf_nla_for_each_attr(nla, head, len, rem) {
>>>>>>> upstream/android-13
		int type = nla_type(nla);

		if (type > maxtype)
			continue;

		if (policy) {
			err = validate_nla(nla, maxtype, policy);
			if (err < 0)
				goto errout;
		}

		if (tb[type])
<<<<<<< HEAD
			fprintf(stderr, "Attribute of type %#x found multiple times in message, "
				  "previous attribute is being ignored.\n", type);
=======
			pr_warn("Attribute of type %#x found multiple times in message, "
				"previous attribute is being ignored.\n", type);
>>>>>>> upstream/android-13

		tb[type] = nla;
	}

	err = 0;
errout:
	return err;
}

<<<<<<< HEAD
/* dump netlink extended ack error message */
int nla_dump_errormsg(struct nlmsghdr *nlh)
{
	struct nla_policy extack_policy[NLMSGERR_ATTR_MAX + 1] = {
		[NLMSGERR_ATTR_MSG]	= { .type = NLA_STRING },
		[NLMSGERR_ATTR_OFFS]	= { .type = NLA_U32 },
=======
/**
 * Create attribute index based on nested attribute
 * @arg tb              Index array to be filled (maxtype+1 elements).
 * @arg maxtype         Maximum attribute type expected and accepted.
 * @arg nla             Nested Attribute.
 * @arg policy          Attribute validation policy.
 *
 * Feeds the stream of attributes nested into the specified attribute
 * to libbpf_nla_parse().
 *
 * @see libbpf_nla_parse
 * @return 0 on success or a negative error code.
 */
int libbpf_nla_parse_nested(struct nlattr *tb[], int maxtype,
			    struct nlattr *nla,
			    struct libbpf_nla_policy *policy)
{
	return libbpf_nla_parse(tb, maxtype, libbpf_nla_data(nla),
				libbpf_nla_len(nla), policy);
}

/* dump netlink extended ack error message */
int libbpf_nla_dump_errormsg(struct nlmsghdr *nlh)
{
	struct libbpf_nla_policy extack_policy[NLMSGERR_ATTR_MAX + 1] = {
		[NLMSGERR_ATTR_MSG]	= { .type = LIBBPF_NLA_STRING },
		[NLMSGERR_ATTR_OFFS]	= { .type = LIBBPF_NLA_U32 },
>>>>>>> upstream/android-13
	};
	struct nlattr *tb[NLMSGERR_ATTR_MAX + 1], *attr;
	struct nlmsgerr *err;
	char *errmsg = NULL;
	int hlen, alen;

	/* no TLVs, nothing to do here */
	if (!(nlh->nlmsg_flags & NLM_F_ACK_TLVS))
		return 0;

	err = (struct nlmsgerr *)NLMSG_DATA(nlh);
	hlen = sizeof(*err);

	/* if NLM_F_CAPPED is set then the inner err msg was capped */
	if (!(nlh->nlmsg_flags & NLM_F_CAPPED))
		hlen += nlmsg_len(&err->msg);

	attr = (struct nlattr *) ((void *) err + hlen);
	alen = nlh->nlmsg_len - hlen;

<<<<<<< HEAD
	if (nla_parse(tb, NLMSGERR_ATTR_MAX, attr, alen, extack_policy) != 0) {
		fprintf(stderr,
			"Failed to parse extended error attributes\n");
=======
	if (libbpf_nla_parse(tb, NLMSGERR_ATTR_MAX, attr, alen,
			     extack_policy) != 0) {
		pr_warn("Failed to parse extended error attributes\n");
>>>>>>> upstream/android-13
		return 0;
	}

	if (tb[NLMSGERR_ATTR_MSG])
<<<<<<< HEAD
		errmsg = (char *) nla_data(tb[NLMSGERR_ATTR_MSG]);

	fprintf(stderr, "Kernel error message: %s\n", errmsg);
=======
		errmsg = (char *) libbpf_nla_data(tb[NLMSGERR_ATTR_MSG]);

	pr_warn("Kernel error message: %s\n", errmsg);
>>>>>>> upstream/android-13

	return 0;
}
