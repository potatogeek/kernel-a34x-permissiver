<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* AFS fileserver list management.
 *
 * Copyright (C) 2017 Red Hat, Inc. All Rights Reserved.
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

#include <linux/kernel.h>
#include <linux/slab.h>
#include "internal.h"

void afs_put_serverlist(struct afs_net *net, struct afs_server_list *slist)
{
	int i;

	if (slist && refcount_dec_and_test(&slist->usage)) {
<<<<<<< HEAD
		for (i = 0; i < slist->nr_servers; i++) {
			afs_put_cb_interest(net, slist->servers[i].cb_interest);
			afs_put_server(net, slist->servers[i].server);
		}
=======
		for (i = 0; i < slist->nr_servers; i++)
			afs_unuse_server(net, slist->servers[i].server,
					 afs_server_trace_put_slist);
>>>>>>> upstream/android-13
		kfree(slist);
	}
}

/*
 * Build a server list from a VLDB record.
 */
struct afs_server_list *afs_alloc_server_list(struct afs_cell *cell,
					      struct key *key,
					      struct afs_vldb_entry *vldb,
					      u8 type_mask)
{
	struct afs_server_list *slist;
	struct afs_server *server;
	int ret = -ENOMEM, nr_servers = 0, i, j;

	for (i = 0; i < vldb->nr_servers; i++)
		if (vldb->fs_mask[i] & type_mask)
			nr_servers++;

<<<<<<< HEAD
	slist = kzalloc(sizeof(struct afs_server_list) +
			sizeof(struct afs_server_entry) * nr_servers,
			GFP_KERNEL);
=======
	slist = kzalloc(struct_size(slist, servers, nr_servers), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!slist)
		goto error;

	refcount_set(&slist->usage, 1);
	rwlock_init(&slist->lock);

<<<<<<< HEAD
=======
	for (i = 0; i < AFS_MAXTYPES; i++)
		slist->vids[i] = vldb->vid[i];

>>>>>>> upstream/android-13
	/* Make sure a records exists for each server in the list. */
	for (i = 0; i < vldb->nr_servers; i++) {
		if (!(vldb->fs_mask[i] & type_mask))
			continue;

<<<<<<< HEAD
		server = afs_lookup_server(cell, key, &vldb->fs_server[i]);
=======
		server = afs_lookup_server(cell, key, &vldb->fs_server[i],
					   vldb->addr_version[i]);
>>>>>>> upstream/android-13
		if (IS_ERR(server)) {
			ret = PTR_ERR(server);
			if (ret == -ENOENT ||
			    ret == -ENOMEDIUM)
				continue;
			goto error_2;
		}

		/* Insertion-sort by UUID */
		for (j = 0; j < slist->nr_servers; j++)
			if (memcmp(&slist->servers[j].server->uuid,
				   &server->uuid,
				   sizeof(server->uuid)) >= 0)
				break;
		if (j < slist->nr_servers) {
			if (slist->servers[j].server == server) {
<<<<<<< HEAD
				afs_put_server(cell->net, server);
=======
				afs_put_server(cell->net, server,
					       afs_server_trace_put_slist_isort);
>>>>>>> upstream/android-13
				continue;
			}

			memmove(slist->servers + j + 1,
				slist->servers + j,
				(slist->nr_servers - j) * sizeof(struct afs_server_entry));
		}

		slist->servers[j].server = server;
		slist->nr_servers++;
	}

	if (slist->nr_servers == 0) {
		ret = -EDESTADDRREQ;
		goto error_2;
	}

	return slist;

error_2:
	afs_put_serverlist(cell->net, slist);
error:
	return ERR_PTR(ret);
}

/*
 * Copy the annotations from an old server list to its potential replacement.
 */
bool afs_annotate_server_list(struct afs_server_list *new,
			      struct afs_server_list *old)
{
	struct afs_server *cur;
	int i, j;

	if (old->nr_servers != new->nr_servers)
		goto changed;

	for (i = 0; i < old->nr_servers; i++)
		if (old->servers[i].server != new->servers[i].server)
			goto changed;

	return false;

changed:
<<<<<<< HEAD
	/* Maintain the same current server as before if possible. */
	cur = old->servers[old->index].server;
	for (j = 0; j < new->nr_servers; j++) {
		if (new->servers[j].server == cur) {
			new->index = j;
=======
	/* Maintain the same preferred server as before if possible. */
	cur = old->servers[old->preferred].server;
	for (j = 0; j < new->nr_servers; j++) {
		if (new->servers[j].server == cur) {
			new->preferred = j;
>>>>>>> upstream/android-13
			break;
		}
	}

<<<<<<< HEAD
	/* Keep the old callback interest records where possible so that we
	 * maintain callback interception.
	 */
	i = 0;
	j = 0;
	while (i < old->nr_servers && j < new->nr_servers) {
		if (new->servers[j].server == old->servers[i].server) {
			struct afs_cb_interest *cbi = old->servers[i].cb_interest;
			if (cbi) {
				new->servers[j].cb_interest = cbi;
				refcount_inc(&cbi->usage);
			}
			i++;
			j++;
			continue;
		}

		if (new->servers[j].server < old->servers[i].server) {
			j++;
			continue;
		}

		i++;
		continue;
	}

=======
>>>>>>> upstream/android-13
	return true;
}
