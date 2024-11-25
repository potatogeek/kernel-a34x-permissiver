<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* AFS Volume Location Service client
 *
 * Copyright (C) 2002 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/sched.h>
#include "afs_fs.h"
#include "internal.h"

/*
 * Deliver reply data to a VL.GetEntryByNameU call.
 */
static int afs_deliver_vl_get_entry_by_name_u(struct afs_call *call)
{
	struct afs_uvldbentry__xdr *uvldb;
	struct afs_vldb_entry *entry;
	bool new_only = false;
	u32 tmp, nr_servers, vlflags;
	int i, ret;

	_enter("");

	ret = afs_transfer_reply(call);
	if (ret < 0)
		return ret;

	/* unmarshall the reply once we've received all of it */
	uvldb = call->buffer;
<<<<<<< HEAD
	entry = call->reply[0];
=======
	entry = call->ret_vldb;
>>>>>>> upstream/android-13

	nr_servers = ntohl(uvldb->nServers);
	if (nr_servers > AFS_NMAXNSERVERS)
		nr_servers = AFS_NMAXNSERVERS;

	for (i = 0; i < ARRAY_SIZE(uvldb->name) - 1; i++)
		entry->name[i] = (u8)ntohl(uvldb->name[i]);
	entry->name[i] = 0;
	entry->name_len = strlen(entry->name);

	/* If there is a new replication site that we can use, ignore all the
	 * sites that aren't marked as new.
	 */
	for (i = 0; i < nr_servers; i++) {
		tmp = ntohl(uvldb->serverFlags[i]);
		if (!(tmp & AFS_VLSF_DONTUSE) &&
		    (tmp & AFS_VLSF_NEWREPSITE))
			new_only = true;
	}

	vlflags = ntohl(uvldb->flags);
	for (i = 0; i < nr_servers; i++) {
		struct afs_uuid__xdr *xdr;
		struct afs_uuid *uuid;
		int j;
		int n = entry->nr_servers;

		tmp = ntohl(uvldb->serverFlags[i]);
		if (tmp & AFS_VLSF_DONTUSE ||
		    (new_only && !(tmp & AFS_VLSF_NEWREPSITE)))
			continue;
		if (tmp & AFS_VLSF_RWVOL) {
			entry->fs_mask[n] |= AFS_VOL_VTM_RW;
			if (vlflags & AFS_VLF_BACKEXISTS)
				entry->fs_mask[n] |= AFS_VOL_VTM_BAK;
		}
		if (tmp & AFS_VLSF_ROVOL)
			entry->fs_mask[n] |= AFS_VOL_VTM_RO;
		if (!entry->fs_mask[n])
			continue;

		xdr = &uvldb->serverNumber[i];
		uuid = (struct afs_uuid *)&entry->fs_server[n];
		uuid->time_low			= xdr->time_low;
		uuid->time_mid			= htons(ntohl(xdr->time_mid));
		uuid->time_hi_and_version	= htons(ntohl(xdr->time_hi_and_version));
		uuid->clock_seq_hi_and_reserved	= (u8)ntohl(xdr->clock_seq_hi_and_reserved);
		uuid->clock_seq_low		= (u8)ntohl(xdr->clock_seq_low);
		for (j = 0; j < 6; j++)
			uuid->node[j] = (u8)ntohl(xdr->node[j]);

<<<<<<< HEAD
=======
		entry->addr_version[n] = ntohl(uvldb->serverUnique[i]);
>>>>>>> upstream/android-13
		entry->nr_servers++;
	}

	for (i = 0; i < AFS_MAXTYPES; i++)
		entry->vid[i] = ntohl(uvldb->volumeId[i]);

	if (vlflags & AFS_VLF_RWEXISTS)
		__set_bit(AFS_VLDB_HAS_RW, &entry->flags);
	if (vlflags & AFS_VLF_ROEXISTS)
		__set_bit(AFS_VLDB_HAS_RO, &entry->flags);
	if (vlflags & AFS_VLF_BACKEXISTS)
		__set_bit(AFS_VLDB_HAS_BAK, &entry->flags);

	if (!(vlflags & (AFS_VLF_RWEXISTS | AFS_VLF_ROEXISTS | AFS_VLF_BACKEXISTS))) {
		entry->error = -ENOMEDIUM;
		__set_bit(AFS_VLDB_QUERY_ERROR, &entry->flags);
	}

	__set_bit(AFS_VLDB_QUERY_VALID, &entry->flags);
	_leave(" = 0 [done]");
	return 0;
}

static void afs_destroy_vl_get_entry_by_name_u(struct afs_call *call)
{
<<<<<<< HEAD
	kfree(call->reply[0]);
=======
	kfree(call->ret_vldb);
>>>>>>> upstream/android-13
	afs_flat_call_destructor(call);
}

/*
 * VL.GetEntryByNameU operation type.
 */
static const struct afs_call_type afs_RXVLGetEntryByNameU = {
	.name		= "VL.GetEntryByNameU",
	.op		= afs_VL_GetEntryByNameU,
	.deliver	= afs_deliver_vl_get_entry_by_name_u,
	.destructor	= afs_destroy_vl_get_entry_by_name_u,
};

/*
 * Dispatch a get volume entry by name or ID operation (uuid variant).  If the
 * volname is a decimal number then it's a volume ID not a volume name.
 */
<<<<<<< HEAD
struct afs_vldb_entry *afs_vl_get_entry_by_name_u(struct afs_net *net,
						  struct afs_addr_cursor *ac,
						  struct key *key,
=======
struct afs_vldb_entry *afs_vl_get_entry_by_name_u(struct afs_vl_cursor *vc,
>>>>>>> upstream/android-13
						  const char *volname,
						  int volnamesz)
{
	struct afs_vldb_entry *entry;
	struct afs_call *call;
<<<<<<< HEAD
=======
	struct afs_net *net = vc->cell->net;
>>>>>>> upstream/android-13
	size_t reqsz, padsz;
	__be32 *bp;

	_enter("");

	padsz = (4 - (volnamesz & 3)) & 3;
	reqsz = 8 + volnamesz + padsz;

	entry = kzalloc(sizeof(struct afs_vldb_entry), GFP_KERNEL);
	if (!entry)
		return ERR_PTR(-ENOMEM);

	call = afs_alloc_flat_call(net, &afs_RXVLGetEntryByNameU, reqsz,
				   sizeof(struct afs_uvldbentry__xdr));
	if (!call) {
		kfree(entry);
		return ERR_PTR(-ENOMEM);
	}

<<<<<<< HEAD
	call->key = key;
	call->reply[0] = entry;
	call->ret_reply0 = true;
=======
	call->key = vc->key;
	call->ret_vldb = entry;
	call->max_lifespan = AFS_VL_MAX_LIFESPAN;
>>>>>>> upstream/android-13

	/* Marshall the parameters */
	bp = call->request;
	*bp++ = htonl(VLGETENTRYBYNAMEU);
	*bp++ = htonl(volnamesz);
	memcpy(bp, volname, volnamesz);
	if (padsz > 0)
		memset((void *)bp + volnamesz, 0, padsz);

	trace_afs_make_vl_call(call);
<<<<<<< HEAD
	return (struct afs_vldb_entry *)afs_make_call(ac, call, GFP_KERNEL, false);
=======
	afs_make_call(&vc->ac, call, GFP_KERNEL);
	return (struct afs_vldb_entry *)afs_wait_for_call_to_complete(call, &vc->ac);
>>>>>>> upstream/android-13
}

/*
 * Deliver reply data to a VL.GetAddrsU call.
 *
 *	GetAddrsU(IN ListAddrByAttributes *inaddr,
 *		  OUT afsUUID *uuidp1,
 *		  OUT uint32_t *uniquifier,
 *		  OUT uint32_t *nentries,
 *		  OUT bulkaddrs *blkaddrs);
 */
static int afs_deliver_vl_get_addrs_u(struct afs_call *call)
{
	struct afs_addr_list *alist;
	__be32 *bp;
	u32 uniquifier, nentries, count;
	int i, ret;

<<<<<<< HEAD
	_enter("{%u,%zu/%u}", call->unmarshall, call->offset, call->count);

again:
	switch (call->unmarshall) {
	case 0:
		call->offset = 0;
		call->unmarshall++;

		/* Extract the returned uuid, uniquifier, nentries and blkaddrs size */
	case 1:
		ret = afs_extract_data(call, call->buffer,
				       sizeof(struct afs_uuid__xdr) + 3 * sizeof(__be32),
				       true);
=======
	_enter("{%u,%zu/%u}",
	       call->unmarshall, iov_iter_count(call->iter), call->count);

	switch (call->unmarshall) {
	case 0:
		afs_extract_to_buf(call,
				   sizeof(struct afs_uuid__xdr) + 3 * sizeof(__be32));
		call->unmarshall++;

		/* Extract the returned uuid, uniquifier, nentries and
		 * blkaddrs size */
		fallthrough;
	case 1:
		ret = afs_extract_data(call, true);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		bp = call->buffer + sizeof(struct afs_uuid__xdr);
		uniquifier	= ntohl(*bp++);
		nentries	= ntohl(*bp++);
		count		= ntohl(*bp);

		nentries = min(nentries, count);
		alist = afs_alloc_addrlist(nentries, FS_SERVICE, AFS_FS_PORT);
		if (!alist)
			return -ENOMEM;
		alist->version = uniquifier;
<<<<<<< HEAD
		call->reply[0] = alist;
		call->count = count;
		call->count2 = nentries;
		call->offset = 0;
		call->unmarshall++;

		/* Extract entries */
	case 2:
		count = min(call->count, 4U);
		ret = afs_extract_data(call, call->buffer,
				       count * sizeof(__be32),
				       call->count > 4);
		if (ret < 0)
			return ret;

		alist = call->reply[0];
		bp = call->buffer;
=======
		call->ret_alist = alist;
		call->count = count;
		call->count2 = nentries;
		call->unmarshall++;

	more_entries:
		count = min(call->count, 4U);
		afs_extract_to_buf(call, count * sizeof(__be32));

		fallthrough;	/* and extract entries */
	case 2:
		ret = afs_extract_data(call, call->count > 4);
		if (ret < 0)
			return ret;

		alist = call->ret_alist;
		bp = call->buffer;
		count = min(call->count, 4U);
>>>>>>> upstream/android-13
		for (i = 0; i < count; i++)
			if (alist->nr_addrs < call->count2)
				afs_merge_fs_addr4(alist, *bp++, AFS_FS_PORT);

		call->count -= count;
		if (call->count > 0)
<<<<<<< HEAD
			goto again;
		call->offset = 0;
=======
			goto more_entries;
>>>>>>> upstream/android-13
		call->unmarshall++;
		break;
	}

	_leave(" = 0 [done]");
	return 0;
}

static void afs_vl_get_addrs_u_destructor(struct afs_call *call)
{
<<<<<<< HEAD
	afs_put_server(call->net, (struct afs_server *)call->reply[0]);
	kfree(call->reply[1]);
=======
	afs_put_addrlist(call->ret_alist);
>>>>>>> upstream/android-13
	return afs_flat_call_destructor(call);
}

/*
 * VL.GetAddrsU operation type.
 */
static const struct afs_call_type afs_RXVLGetAddrsU = {
	.name		= "VL.GetAddrsU",
	.op		= afs_VL_GetAddrsU,
	.deliver	= afs_deliver_vl_get_addrs_u,
	.destructor	= afs_vl_get_addrs_u_destructor,
};

/*
 * Dispatch an operation to get the addresses for a server, where the server is
 * nominated by UUID.
 */
<<<<<<< HEAD
struct afs_addr_list *afs_vl_get_addrs_u(struct afs_net *net,
					 struct afs_addr_cursor *ac,
					 struct key *key,
=======
struct afs_addr_list *afs_vl_get_addrs_u(struct afs_vl_cursor *vc,
>>>>>>> upstream/android-13
					 const uuid_t *uuid)
{
	struct afs_ListAddrByAttributes__xdr *r;
	const struct afs_uuid *u = (const struct afs_uuid *)uuid;
	struct afs_call *call;
<<<<<<< HEAD
=======
	struct afs_net *net = vc->cell->net;
>>>>>>> upstream/android-13
	__be32 *bp;
	int i;

	_enter("");

	call = afs_alloc_flat_call(net, &afs_RXVLGetAddrsU,
				   sizeof(__be32) + sizeof(struct afs_ListAddrByAttributes__xdr),
				   sizeof(struct afs_uuid__xdr) + 3 * sizeof(__be32));
	if (!call)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	call->key = key;
	call->reply[0] = NULL;
	call->ret_reply0 = true;
=======
	call->key = vc->key;
	call->ret_alist = NULL;
	call->max_lifespan = AFS_VL_MAX_LIFESPAN;
>>>>>>> upstream/android-13

	/* Marshall the parameters */
	bp = call->request;
	*bp++ = htonl(VLGETADDRSU);
	r = (struct afs_ListAddrByAttributes__xdr *)bp;
	r->Mask		= htonl(AFS_VLADDR_UUID);
	r->ipaddr	= 0;
	r->index	= 0;
	r->spare	= 0;
	r->uuid.time_low			= u->time_low;
	r->uuid.time_mid			= htonl(ntohs(u->time_mid));
	r->uuid.time_hi_and_version		= htonl(ntohs(u->time_hi_and_version));
	r->uuid.clock_seq_hi_and_reserved 	= htonl(u->clock_seq_hi_and_reserved);
	r->uuid.clock_seq_low			= htonl(u->clock_seq_low);
	for (i = 0; i < 6; i++)
		r->uuid.node[i] = htonl(u->node[i]);

	trace_afs_make_vl_call(call);
<<<<<<< HEAD
	return (struct afs_addr_list *)afs_make_call(ac, call, GFP_KERNEL, false);
=======
	afs_make_call(&vc->ac, call, GFP_KERNEL);
	return (struct afs_addr_list *)afs_wait_for_call_to_complete(call, &vc->ac);
>>>>>>> upstream/android-13
}

/*
 * Deliver reply data to an VL.GetCapabilities operation.
 */
static int afs_deliver_vl_get_capabilities(struct afs_call *call)
{
	u32 count;
	int ret;

<<<<<<< HEAD
	_enter("{%u,%zu/%u}", call->unmarshall, call->offset, call->count);

again:
	switch (call->unmarshall) {
	case 0:
		call->offset = 0;
		call->unmarshall++;

		/* Extract the capabilities word count */
	case 1:
		ret = afs_extract_data(call, &call->tmp,
				       1 * sizeof(__be32),
				       true);
=======
	_enter("{%u,%zu/%u}",
	       call->unmarshall, iov_iter_count(call->iter), call->count);

	switch (call->unmarshall) {
	case 0:
		afs_extract_to_tmp(call);
		call->unmarshall++;

		fallthrough;	/* and extract the capabilities word count */
	case 1:
		ret = afs_extract_data(call, true);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		count = ntohl(call->tmp);
<<<<<<< HEAD

		call->count = count;
		call->count2 = count;
		call->offset = 0;
		call->unmarshall++;

		/* Extract capabilities words */
	case 2:
		count = min(call->count, 16U);
		ret = afs_extract_data(call, call->buffer,
				       count * sizeof(__be32),
				       call->count > 16);
=======
		call->count = count;
		call->count2 = count;

		call->unmarshall++;
		afs_extract_discard(call, count * sizeof(__be32));

		fallthrough;	/* and extract capabilities words */
	case 2:
		ret = afs_extract_data(call, false);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		/* TODO: Examine capabilities */

<<<<<<< HEAD
		call->count -= count;
		if (call->count > 0)
			goto again;
		call->offset = 0;
=======
>>>>>>> upstream/android-13
		call->unmarshall++;
		break;
	}

<<<<<<< HEAD
	call->reply[0] = (void *)(unsigned long)call->service_id;

=======
>>>>>>> upstream/android-13
	_leave(" = 0 [done]");
	return 0;
}

<<<<<<< HEAD
=======
static void afs_destroy_vl_get_capabilities(struct afs_call *call)
{
	afs_put_vlserver(call->net, call->vlserver);
	afs_flat_call_destructor(call);
}

>>>>>>> upstream/android-13
/*
 * VL.GetCapabilities operation type
 */
static const struct afs_call_type afs_RXVLGetCapabilities = {
	.name		= "VL.GetCapabilities",
	.op		= afs_VL_GetCapabilities,
	.deliver	= afs_deliver_vl_get_capabilities,
<<<<<<< HEAD
	.destructor	= afs_flat_call_destructor,
};

/*
 * Probe a fileserver for the capabilities that it supports.  This can
=======
	.done		= afs_vlserver_probe_result,
	.destructor	= afs_destroy_vl_get_capabilities,
};

/*
 * Probe a volume server for the capabilities that it supports.  This can
>>>>>>> upstream/android-13
 * return up to 196 words.
 *
 * We use this to probe for service upgrade to determine what the server at the
 * other end supports.
 */
<<<<<<< HEAD
int afs_vl_get_capabilities(struct afs_net *net,
			    struct afs_addr_cursor *ac,
			    struct key *key)
=======
struct afs_call *afs_vl_get_capabilities(struct afs_net *net,
					 struct afs_addr_cursor *ac,
					 struct key *key,
					 struct afs_vlserver *server,
					 unsigned int server_index)
>>>>>>> upstream/android-13
{
	struct afs_call *call;
	__be32 *bp;

	_enter("");

	call = afs_alloc_flat_call(net, &afs_RXVLGetCapabilities, 1 * 4, 16 * 4);
	if (!call)
<<<<<<< HEAD
		return -ENOMEM;

	call->key = key;
	call->upgrade = true; /* Let's see if this is a YFS server */
	call->reply[0] = (void *)VLGETCAPABILITIES;
	call->ret_reply0 = true;
=======
		return ERR_PTR(-ENOMEM);

	call->key = key;
	call->vlserver = afs_get_vlserver(server);
	call->server_index = server_index;
	call->upgrade = true;
	call->async = true;
	call->max_lifespan = AFS_PROBE_MAX_LIFESPAN;
>>>>>>> upstream/android-13

	/* marshall the parameters */
	bp = call->request;
	*bp++ = htonl(VLGETCAPABILITIES);

	/* Can't take a ref on server */
	trace_afs_make_vl_call(call);
<<<<<<< HEAD
	return afs_make_call(ac, call, GFP_KERNEL, false);
=======
	afs_make_call(ac, call, GFP_KERNEL);
	return call;
>>>>>>> upstream/android-13
}

/*
 * Deliver reply data to a YFSVL.GetEndpoints call.
 *
 *	GetEndpoints(IN yfsServerAttributes *attr,
 *		     OUT opr_uuid *uuid,
 *		     OUT afs_int32 *uniquifier,
 *		     OUT endpoints *fsEndpoints,
 *		     OUT endpoints *volEndpoints)
 */
static int afs_deliver_yfsvl_get_endpoints(struct afs_call *call)
{
	struct afs_addr_list *alist;
	__be32 *bp;
	u32 uniquifier, size;
	int ret;

<<<<<<< HEAD
	_enter("{%u,%zu/%u,%u}", call->unmarshall, call->offset, call->count, call->count2);

again:
	switch (call->unmarshall) {
	case 0:
		call->offset = 0;
=======
	_enter("{%u,%zu,%u}",
	       call->unmarshall, iov_iter_count(call->iter), call->count2);

	switch (call->unmarshall) {
	case 0:
		afs_extract_to_buf(call, sizeof(uuid_t) + 3 * sizeof(__be32));
>>>>>>> upstream/android-13
		call->unmarshall = 1;

		/* Extract the returned uuid, uniquifier, fsEndpoints count and
		 * either the first fsEndpoint type or the volEndpoints
		 * count if there are no fsEndpoints. */
<<<<<<< HEAD
	case 1:
		ret = afs_extract_data(call, call->buffer,
				       sizeof(uuid_t) +
				       3 * sizeof(__be32),
				       true);
=======
		fallthrough;
	case 1:
		ret = afs_extract_data(call, true);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		bp = call->buffer + sizeof(uuid_t);
		uniquifier	= ntohl(*bp++);
		call->count	= ntohl(*bp++);
		call->count2	= ntohl(*bp); /* Type or next count */

		if (call->count > YFS_MAXENDPOINTS)
<<<<<<< HEAD
			return afs_protocol_error(call, -EBADMSG);
=======
			return afs_protocol_error(call, afs_eproto_yvl_fsendpt_num);
>>>>>>> upstream/android-13

		alist = afs_alloc_addrlist(call->count, FS_SERVICE, AFS_FS_PORT);
		if (!alist)
			return -ENOMEM;
		alist->version = uniquifier;
<<<<<<< HEAD
		call->reply[0] = alist;
		call->offset = 0;
=======
		call->ret_alist = alist;
>>>>>>> upstream/android-13

		if (call->count == 0)
			goto extract_volendpoints;

<<<<<<< HEAD
		call->unmarshall = 2;

		/* Extract fsEndpoints[] entries */
	case 2:
=======
	next_fsendpoint:
>>>>>>> upstream/android-13
		switch (call->count2) {
		case YFS_ENDPOINT_IPV4:
			size = sizeof(__be32) * (1 + 1 + 1);
			break;
		case YFS_ENDPOINT_IPV6:
			size = sizeof(__be32) * (1 + 4 + 1);
			break;
		default:
<<<<<<< HEAD
			return afs_protocol_error(call, -EBADMSG);
		}

		size += sizeof(__be32);
		ret = afs_extract_data(call, call->buffer, size, true);
		if (ret < 0)
			return ret;

		alist = call->reply[0];
=======
			return afs_protocol_error(call, afs_eproto_yvl_fsendpt_type);
		}

		size += sizeof(__be32);
		afs_extract_to_buf(call, size);
		call->unmarshall = 2;

		fallthrough;	/* and extract fsEndpoints[] entries */
	case 2:
		ret = afs_extract_data(call, true);
		if (ret < 0)
			return ret;

		alist = call->ret_alist;
>>>>>>> upstream/android-13
		bp = call->buffer;
		switch (call->count2) {
		case YFS_ENDPOINT_IPV4:
			if (ntohl(bp[0]) != sizeof(__be32) * 2)
<<<<<<< HEAD
				return afs_protocol_error(call, -EBADMSG);
=======
				return afs_protocol_error(
					call, afs_eproto_yvl_fsendpt4_len);
>>>>>>> upstream/android-13
			afs_merge_fs_addr4(alist, bp[1], ntohl(bp[2]));
			bp += 3;
			break;
		case YFS_ENDPOINT_IPV6:
			if (ntohl(bp[0]) != sizeof(__be32) * 5)
<<<<<<< HEAD
				return afs_protocol_error(call, -EBADMSG);
=======
				return afs_protocol_error(
					call, afs_eproto_yvl_fsendpt6_len);
>>>>>>> upstream/android-13
			afs_merge_fs_addr6(alist, bp + 1, ntohl(bp[5]));
			bp += 6;
			break;
		default:
<<<<<<< HEAD
			return afs_protocol_error(call, -EBADMSG);
=======
			return afs_protocol_error(call, afs_eproto_yvl_fsendpt_type);
>>>>>>> upstream/android-13
		}

		/* Got either the type of the next entry or the count of
		 * volEndpoints if no more fsEndpoints.
		 */
		call->count2 = ntohl(*bp++);

<<<<<<< HEAD
		call->offset = 0;
		call->count--;
		if (call->count > 0)
			goto again;
=======
		call->count--;
		if (call->count > 0)
			goto next_fsendpoint;
>>>>>>> upstream/android-13

	extract_volendpoints:
		/* Extract the list of volEndpoints. */
		call->count = call->count2;
		if (!call->count)
			goto end;
		if (call->count > YFS_MAXENDPOINTS)
<<<<<<< HEAD
			return afs_protocol_error(call, -EBADMSG);

=======
			return afs_protocol_error(call, afs_eproto_yvl_vlendpt_type);

		afs_extract_to_buf(call, 1 * sizeof(__be32));
>>>>>>> upstream/android-13
		call->unmarshall = 3;

		/* Extract the type of volEndpoints[0].  Normally we would
		 * extract the type of the next endpoint when we extract the
		 * data of the current one, but this is the first...
		 */
<<<<<<< HEAD
	case 3:
		ret = afs_extract_data(call, call->buffer, sizeof(__be32), true);
=======
		fallthrough;
	case 3:
		ret = afs_extract_data(call, true);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		bp = call->buffer;
<<<<<<< HEAD
		call->count2 = ntohl(*bp++);
		call->offset = 0;
		call->unmarshall = 4;

		/* Extract volEndpoints[] entries */
	case 4:
=======

	next_volendpoint:
		call->count2 = ntohl(*bp++);
>>>>>>> upstream/android-13
		switch (call->count2) {
		case YFS_ENDPOINT_IPV4:
			size = sizeof(__be32) * (1 + 1 + 1);
			break;
		case YFS_ENDPOINT_IPV6:
			size = sizeof(__be32) * (1 + 4 + 1);
			break;
		default:
<<<<<<< HEAD
			return afs_protocol_error(call, -EBADMSG);
		}

		if (call->count > 1)
			size += sizeof(__be32);
		ret = afs_extract_data(call, call->buffer, size, true);
=======
			return afs_protocol_error(call, afs_eproto_yvl_vlendpt_type);
		}

		if (call->count > 1)
			size += sizeof(__be32); /* Get next type too */
		afs_extract_to_buf(call, size);
		call->unmarshall = 4;

		fallthrough;	/* and extract volEndpoints[] entries */
	case 4:
		ret = afs_extract_data(call, true);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;

		bp = call->buffer;
		switch (call->count2) {
		case YFS_ENDPOINT_IPV4:
			if (ntohl(bp[0]) != sizeof(__be32) * 2)
<<<<<<< HEAD
				return afs_protocol_error(call, -EBADMSG);
=======
				return afs_protocol_error(
					call, afs_eproto_yvl_vlendpt4_len);
>>>>>>> upstream/android-13
			bp += 3;
			break;
		case YFS_ENDPOINT_IPV6:
			if (ntohl(bp[0]) != sizeof(__be32) * 5)
<<<<<<< HEAD
				return afs_protocol_error(call, -EBADMSG);
			bp += 6;
			break;
		default:
			return afs_protocol_error(call, -EBADMSG);
=======
				return afs_protocol_error(
					call, afs_eproto_yvl_vlendpt6_len);
			bp += 6;
			break;
		default:
			return afs_protocol_error(call, afs_eproto_yvl_vlendpt_type);
>>>>>>> upstream/android-13
		}

		/* Got either the type of the next entry or the count of
		 * volEndpoints if no more fsEndpoints.
		 */
<<<<<<< HEAD
		call->offset = 0;
		call->count--;
		if (call->count > 0) {
			call->count2 = ntohl(*bp++);
			goto again;
		}

	end:
		call->unmarshall = 5;

		/* Done */
	case 5:
		ret = afs_extract_data(call, call->buffer, 0, false);
		if (ret < 0)
			return ret;
		call->unmarshall = 6;
=======
		call->count--;
		if (call->count > 0)
			goto next_volendpoint;

	end:
		afs_extract_discard(call, 0);
		call->unmarshall = 5;

		fallthrough;	/* Done */
	case 5:
		ret = afs_extract_data(call, false);
		if (ret < 0)
			return ret;
		call->unmarshall = 6;
		fallthrough;
>>>>>>> upstream/android-13

	case 6:
		break;
	}

<<<<<<< HEAD
	alist = call->reply[0];

	/* Start with IPv6 if available. */
	if (alist->nr_ipv4 < alist->nr_addrs)
		alist->index = alist->nr_ipv4;

=======
>>>>>>> upstream/android-13
	_leave(" = 0 [done]");
	return 0;
}

/*
 * YFSVL.GetEndpoints operation type.
 */
static const struct afs_call_type afs_YFSVLGetEndpoints = {
	.name		= "YFSVL.GetEndpoints",
	.op		= afs_YFSVL_GetEndpoints,
	.deliver	= afs_deliver_yfsvl_get_endpoints,
	.destructor	= afs_vl_get_addrs_u_destructor,
};

/*
 * Dispatch an operation to get the addresses for a server, where the server is
 * nominated by UUID.
 */
<<<<<<< HEAD
struct afs_addr_list *afs_yfsvl_get_endpoints(struct afs_net *net,
					      struct afs_addr_cursor *ac,
					      struct key *key,
					      const uuid_t *uuid)
{
	struct afs_call *call;
=======
struct afs_addr_list *afs_yfsvl_get_endpoints(struct afs_vl_cursor *vc,
					      const uuid_t *uuid)
{
	struct afs_call *call;
	struct afs_net *net = vc->cell->net;
>>>>>>> upstream/android-13
	__be32 *bp;

	_enter("");

	call = afs_alloc_flat_call(net, &afs_YFSVLGetEndpoints,
				   sizeof(__be32) * 2 + sizeof(*uuid),
				   sizeof(struct in6_addr) + sizeof(__be32) * 3);
	if (!call)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	call->key = key;
	call->reply[0] = NULL;
	call->ret_reply0 = true;
=======
	call->key = vc->key;
	call->ret_alist = NULL;
	call->max_lifespan = AFS_VL_MAX_LIFESPAN;
>>>>>>> upstream/android-13

	/* Marshall the parameters */
	bp = call->request;
	*bp++ = htonl(YVLGETENDPOINTS);
	*bp++ = htonl(YFS_SERVER_UUID);
	memcpy(bp, uuid, sizeof(*uuid)); /* Type opr_uuid */

	trace_afs_make_vl_call(call);
<<<<<<< HEAD
	return (struct afs_addr_list *)afs_make_call(ac, call, GFP_KERNEL, false);
=======
	afs_make_call(&vc->ac, call, GFP_KERNEL);
	return (struct afs_addr_list *)afs_wait_for_call_to_complete(call, &vc->ac);
}

/*
 * Deliver reply data to a YFSVL.GetCellName operation.
 */
static int afs_deliver_yfsvl_get_cell_name(struct afs_call *call)
{
	char *cell_name;
	u32 namesz, paddedsz;
	int ret;

	_enter("{%u,%zu/%u}",
	       call->unmarshall, iov_iter_count(call->iter), call->count);

	switch (call->unmarshall) {
	case 0:
		afs_extract_to_tmp(call);
		call->unmarshall++;

		fallthrough;	/* and extract the cell name length */
	case 1:
		ret = afs_extract_data(call, true);
		if (ret < 0)
			return ret;

		namesz = ntohl(call->tmp);
		if (namesz > AFS_MAXCELLNAME)
			return afs_protocol_error(call, afs_eproto_cellname_len);
		paddedsz = (namesz + 3) & ~3;
		call->count = namesz;
		call->count2 = paddedsz - namesz;

		cell_name = kmalloc(namesz + 1, GFP_KERNEL);
		if (!cell_name)
			return -ENOMEM;
		cell_name[namesz] = 0;
		call->ret_str = cell_name;

		afs_extract_begin(call, cell_name, namesz);
		call->unmarshall++;

		fallthrough;	/* and extract cell name */
	case 2:
		ret = afs_extract_data(call, true);
		if (ret < 0)
			return ret;

		afs_extract_discard(call, call->count2);
		call->unmarshall++;

		fallthrough;	/* and extract padding */
	case 3:
		ret = afs_extract_data(call, false);
		if (ret < 0)
			return ret;

		call->unmarshall++;
		break;
	}

	_leave(" = 0 [done]");
	return 0;
}

static void afs_destroy_yfsvl_get_cell_name(struct afs_call *call)
{
	kfree(call->ret_str);
	afs_flat_call_destructor(call);
}

/*
 * VL.GetCapabilities operation type
 */
static const struct afs_call_type afs_YFSVLGetCellName = {
	.name		= "YFSVL.GetCellName",
	.op		= afs_YFSVL_GetCellName,
	.deliver	= afs_deliver_yfsvl_get_cell_name,
	.destructor	= afs_destroy_yfsvl_get_cell_name,
};

/*
 * Probe a volume server for the capabilities that it supports.  This can
 * return up to 196 words.
 *
 * We use this to probe for service upgrade to determine what the server at the
 * other end supports.
 */
char *afs_yfsvl_get_cell_name(struct afs_vl_cursor *vc)
{
	struct afs_call *call;
	struct afs_net *net = vc->cell->net;
	__be32 *bp;

	_enter("");

	call = afs_alloc_flat_call(net, &afs_YFSVLGetCellName, 1 * 4, 0);
	if (!call)
		return ERR_PTR(-ENOMEM);

	call->key = vc->key;
	call->ret_str = NULL;
	call->max_lifespan = AFS_VL_MAX_LIFESPAN;

	/* marshall the parameters */
	bp = call->request;
	*bp++ = htonl(YVLGETCELLNAME);

	/* Can't take a ref on server */
	trace_afs_make_vl_call(call);
	afs_make_call(&vc->ac, call, GFP_KERNEL);
	return (char *)afs_wait_for_call_to_complete(call, &vc->ac);
>>>>>>> upstream/android-13
}
