<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* Handle fileserver selection and rotation.
 *
 * Copyright (C) 2017 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>
#include "internal.h"
#include "afs_fs.h"

/*
<<<<<<< HEAD
 * Initialise a filesystem server cursor for iterating over FS servers.
 */
static void afs_init_fs_cursor(struct afs_fs_cursor *fc, struct afs_vnode *vnode)
{
	memset(fc, 0, sizeof(*fc));
}

/*
 * Begin an operation on the fileserver.
 *
 * Fileserver operations are serialised on the server by vnode, so we serialise
 * them here also using the io_lock.
 */
bool afs_begin_vnode_operation(struct afs_fs_cursor *fc, struct afs_vnode *vnode,
			       struct key *key)
{
	afs_init_fs_cursor(fc, vnode);
	fc->vnode = vnode;
	fc->key = key;
	fc->ac.error = SHRT_MAX;

	if (mutex_lock_interruptible(&vnode->io_lock) < 0) {
		fc->ac.error = -EINTR;
		fc->flags |= AFS_FS_CURSOR_STOP;
		return false;
	}

	if (vnode->lock_state != AFS_VNODE_LOCK_NONE)
		fc->flags |= AFS_FS_CURSOR_CUR_ONLY;
	return true;
}

/*
 * Begin iteration through a server list, starting with the vnode's last used
 * server if possible, or the last recorded good server if not.
 */
static bool afs_start_fs_iteration(struct afs_fs_cursor *fc,
				   struct afs_vnode *vnode)
{
	struct afs_cb_interest *cbi;
	int i;

	read_lock(&vnode->volume->servers_lock);
	fc->server_list = afs_get_serverlist(vnode->volume->servers);
	read_unlock(&vnode->volume->servers_lock);

	cbi = vnode->cb_interest;
	if (cbi) {
		/* See if the vnode's preferred record is still available */
		for (i = 0; i < fc->server_list->nr_servers; i++) {
			if (fc->server_list->servers[i].cb_interest == cbi) {
				fc->start = i;
=======
 * Begin iteration through a server list, starting with the vnode's last used
 * server if possible, or the last recorded good server if not.
 */
static bool afs_start_fs_iteration(struct afs_operation *op,
				   struct afs_vnode *vnode)
{
	struct afs_server *server;
	void *cb_server;
	int i;

	read_lock(&op->volume->servers_lock);
	op->server_list = afs_get_serverlist(
		rcu_dereference_protected(op->volume->servers,
					  lockdep_is_held(&op->volume->servers_lock)));
	read_unlock(&op->volume->servers_lock);

	op->untried = (1UL << op->server_list->nr_servers) - 1;
	op->index = READ_ONCE(op->server_list->preferred);

	cb_server = vnode->cb_server;
	if (cb_server) {
		/* See if the vnode's preferred record is still available */
		for (i = 0; i < op->server_list->nr_servers; i++) {
			server = op->server_list->servers[i].server;
			if (server == cb_server) {
				op->index = i;
>>>>>>> upstream/android-13
				goto found_interest;
			}
		}

		/* If we have a lock outstanding on a server that's no longer
		 * serving this vnode, then we can't switch to another server
		 * and have to return an error.
		 */
<<<<<<< HEAD
		if (fc->flags & AFS_FS_CURSOR_CUR_ONLY) {
			fc->ac.error = -ESTALE;
=======
		if (op->flags & AFS_OPERATION_CUR_ONLY) {
			op->error = -ESTALE;
>>>>>>> upstream/android-13
			return false;
		}

		/* Note that the callback promise is effectively broken */
		write_seqlock(&vnode->cb_lock);
<<<<<<< HEAD
		ASSERTCMP(cbi, ==, vnode->cb_interest);
		vnode->cb_interest = NULL;
		if (test_and_clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags))
			vnode->cb_break++;
		write_sequnlock(&vnode->cb_lock);

		afs_put_cb_interest(afs_v2net(vnode), cbi);
		cbi = NULL;
	} else {
		fc->start = READ_ONCE(fc->server_list->index);
	}

found_interest:
	fc->index = fc->start;
=======
		ASSERTCMP(cb_server, ==, vnode->cb_server);
		vnode->cb_server = NULL;
		if (test_and_clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags))
			vnode->cb_break++;
		write_sequnlock(&vnode->cb_lock);
	}

found_interest:
>>>>>>> upstream/android-13
	return true;
}

/*
 * Post volume busy note.
 */
static void afs_busy(struct afs_volume *volume, u32 abort_code)
{
	const char *m;

	switch (abort_code) {
	case VOFFLINE:		m = "offline";		break;
	case VRESTARTING:	m = "restarting";	break;
	case VSALVAGING:	m = "being salvaged";	break;
	default:		m = "busy";		break;
	}

<<<<<<< HEAD
	pr_notice("kAFS: Volume %u '%s' is %s\n", volume->vid, volume->name, m);
=======
	pr_notice("kAFS: Volume %llu '%s' is %s\n", volume->vid, volume->name, m);
>>>>>>> upstream/android-13
}

/*
 * Sleep and retry the operation to the same fileserver.
 */
<<<<<<< HEAD
static bool afs_sleep_and_retry(struct afs_fs_cursor *fc)
{
	msleep_interruptible(1000);
	if (signal_pending(current)) {
		fc->ac.error = -ERESTARTSYS;
		return false;
=======
static bool afs_sleep_and_retry(struct afs_operation *op)
{
	if (!(op->flags & AFS_OPERATION_UNINTR)) {
		msleep_interruptible(1000);
		if (signal_pending(current)) {
			op->error = -ERESTARTSYS;
			return false;
		}
	} else {
		msleep(1000);
>>>>>>> upstream/android-13
	}

	return true;
}

/*
 * Select the fileserver to use.  May be called multiple times to rotate
 * through the fileservers.
 */
<<<<<<< HEAD
bool afs_select_fileserver(struct afs_fs_cursor *fc)
{
	struct afs_addr_list *alist;
	struct afs_server *server;
	struct afs_vnode *vnode = fc->vnode;

	_enter("%u/%u,%u/%u,%d,%d",
	       fc->index, fc->start,
	       fc->ac.index, fc->ac.start,
	       fc->ac.error, fc->ac.abort_code);

	if (fc->flags & AFS_FS_CURSOR_STOP) {
=======
bool afs_select_fileserver(struct afs_operation *op)
{
	struct afs_addr_list *alist;
	struct afs_server *server;
	struct afs_vnode *vnode = op->file[0].vnode;
	struct afs_error e;
	u32 rtt;
	int error = op->ac.error, i;

	_enter("%lx[%d],%lx[%d],%d,%d",
	       op->untried, op->index,
	       op->ac.tried, op->ac.index,
	       error, op->ac.abort_code);

	if (op->flags & AFS_OPERATION_STOP) {
>>>>>>> upstream/android-13
		_leave(" = f [stopped]");
		return false;
	}

<<<<<<< HEAD
	/* Evaluate the result of the previous operation, if there was one. */
	switch (fc->ac.error) {
=======
	op->nr_iterations++;

	/* Evaluate the result of the previous operation, if there was one. */
	switch (error) {
>>>>>>> upstream/android-13
	case SHRT_MAX:
		goto start;

	case 0:
	default:
		/* Success or local failure.  Stop. */
<<<<<<< HEAD
		fc->flags |= AFS_FS_CURSOR_STOP;
		_leave(" = f [okay/local %d]", fc->ac.error);
=======
		op->error = error;
		op->flags |= AFS_OPERATION_STOP;
		_leave(" = f [okay/local %d]", error);
>>>>>>> upstream/android-13
		return false;

	case -ECONNABORTED:
		/* The far side rejected the operation on some grounds.  This
		 * might involve the server being busy or the volume having been moved.
		 */
<<<<<<< HEAD
		switch (fc->ac.abort_code) {
=======
		switch (op->ac.abort_code) {
>>>>>>> upstream/android-13
		case VNOVOL:
			/* This fileserver doesn't know about the volume.
			 * - May indicate that the VL is wrong - retry once and compare
			 *   the results.
			 * - May indicate that the fileserver couldn't attach to the vol.
			 */
<<<<<<< HEAD
			if (fc->flags & AFS_FS_CURSOR_VNOVOL) {
				fc->ac.error = -EREMOTEIO;
				goto next_server;
			}

			write_lock(&vnode->volume->servers_lock);
			fc->server_list->vnovol_mask |= 1 << fc->index;
			write_unlock(&vnode->volume->servers_lock);

			set_bit(AFS_VOLUME_NEEDS_UPDATE, &vnode->volume->flags);
			fc->ac.error = afs_check_volume_status(vnode->volume, fc->key);
			if (fc->ac.error < 0)
				goto failed;

			if (test_bit(AFS_VOLUME_DELETED, &vnode->volume->flags)) {
				fc->ac.error = -ENOMEDIUM;
=======
			if (op->flags & AFS_OPERATION_VNOVOL) {
				op->error = -EREMOTEIO;
				goto next_server;
			}

			write_lock(&op->volume->servers_lock);
			op->server_list->vnovol_mask |= 1 << op->index;
			write_unlock(&op->volume->servers_lock);

			set_bit(AFS_VOLUME_NEEDS_UPDATE, &op->volume->flags);
			error = afs_check_volume_status(op->volume, op);
			if (error < 0)
				goto failed_set_error;

			if (test_bit(AFS_VOLUME_DELETED, &op->volume->flags)) {
				op->error = -ENOMEDIUM;
>>>>>>> upstream/android-13
				goto failed;
			}

			/* If the server list didn't change, then assume that
			 * it's the fileserver having trouble.
			 */
<<<<<<< HEAD
			if (vnode->volume->servers == fc->server_list) {
				fc->ac.error = -EREMOTEIO;
=======
			if (rcu_access_pointer(op->volume->servers) == op->server_list) {
				op->error = -EREMOTEIO;
>>>>>>> upstream/android-13
				goto next_server;
			}

			/* Try again */
<<<<<<< HEAD
			fc->flags |= AFS_FS_CURSOR_VNOVOL;
=======
			op->flags |= AFS_OPERATION_VNOVOL;
>>>>>>> upstream/android-13
			_leave(" = t [vnovol]");
			return true;

		case VSALVAGE: /* TODO: Should this return an error or iterate? */
		case VVOLEXISTS:
		case VNOSERVICE:
		case VONLINE:
		case VDISKFULL:
		case VOVERQUOTA:
<<<<<<< HEAD
			fc->ac.error = afs_abort_to_error(fc->ac.abort_code);
			goto next_server;

		case VOFFLINE:
			if (!test_and_set_bit(AFS_VOLUME_OFFLINE, &vnode->volume->flags)) {
				afs_busy(vnode->volume, fc->ac.abort_code);
				clear_bit(AFS_VOLUME_BUSY, &vnode->volume->flags);
			}
			if (fc->flags & AFS_FS_CURSOR_NO_VSLEEP) {
				fc->ac.error = -EADV;
				goto failed;
			}
			if (fc->flags & AFS_FS_CURSOR_CUR_ONLY) {
				fc->ac.error = -ESTALE;
=======
			op->error = afs_abort_to_error(op->ac.abort_code);
			goto next_server;

		case VOFFLINE:
			if (!test_and_set_bit(AFS_VOLUME_OFFLINE, &op->volume->flags)) {
				afs_busy(op->volume, op->ac.abort_code);
				clear_bit(AFS_VOLUME_BUSY, &op->volume->flags);
			}
			if (op->flags & AFS_OPERATION_NO_VSLEEP) {
				op->error = -EADV;
				goto failed;
			}
			if (op->flags & AFS_OPERATION_CUR_ONLY) {
				op->error = -ESTALE;
>>>>>>> upstream/android-13
				goto failed;
			}
			goto busy;

		case VSALVAGING:
		case VRESTARTING:
		case VBUSY:
			/* Retry after going round all the servers unless we
			 * have a file lock we need to maintain.
			 */
<<<<<<< HEAD
			if (fc->flags & AFS_FS_CURSOR_NO_VSLEEP) {
				fc->ac.error = -EBUSY;
				goto failed;
			}
			if (!test_and_set_bit(AFS_VOLUME_BUSY, &vnode->volume->flags)) {
				afs_busy(vnode->volume, fc->ac.abort_code);
				clear_bit(AFS_VOLUME_OFFLINE, &vnode->volume->flags);
			}
		busy:
			if (fc->flags & AFS_FS_CURSOR_CUR_ONLY) {
				if (!afs_sleep_and_retry(fc))
=======
			if (op->flags & AFS_OPERATION_NO_VSLEEP) {
				op->error = -EBUSY;
				goto failed;
			}
			if (!test_and_set_bit(AFS_VOLUME_BUSY, &op->volume->flags)) {
				afs_busy(op->volume, op->ac.abort_code);
				clear_bit(AFS_VOLUME_OFFLINE, &op->volume->flags);
			}
		busy:
			if (op->flags & AFS_OPERATION_CUR_ONLY) {
				if (!afs_sleep_and_retry(op))
>>>>>>> upstream/android-13
					goto failed;

				 /* Retry with same server & address */
				_leave(" = t [vbusy]");
				return true;
			}

<<<<<<< HEAD
			fc->flags |= AFS_FS_CURSOR_VBUSY;
=======
			op->flags |= AFS_OPERATION_VBUSY;
>>>>>>> upstream/android-13
			goto next_server;

		case VMOVED:
			/* The volume migrated to another server.  We consider
			 * consider all locks and callbacks broken and request
			 * an update from the VLDB.
			 *
			 * We also limit the number of VMOVED hops we will
			 * honour, just in case someone sets up a loop.
			 */
<<<<<<< HEAD
			if (fc->flags & AFS_FS_CURSOR_VMOVED) {
				fc->ac.error = -EREMOTEIO;
				goto failed;
			}
			fc->flags |= AFS_FS_CURSOR_VMOVED;

			set_bit(AFS_VOLUME_WAIT, &vnode->volume->flags);
			set_bit(AFS_VOLUME_NEEDS_UPDATE, &vnode->volume->flags);
			fc->ac.error = afs_check_volume_status(vnode->volume, fc->key);
			if (fc->ac.error < 0)
				goto failed;
=======
			if (op->flags & AFS_OPERATION_VMOVED) {
				op->error = -EREMOTEIO;
				goto failed;
			}
			op->flags |= AFS_OPERATION_VMOVED;

			set_bit(AFS_VOLUME_WAIT, &op->volume->flags);
			set_bit(AFS_VOLUME_NEEDS_UPDATE, &op->volume->flags);
			error = afs_check_volume_status(op->volume, op);
			if (error < 0)
				goto failed_set_error;
>>>>>>> upstream/android-13

			/* If the server list didn't change, then the VLDB is
			 * out of sync with the fileservers.  This is hopefully
			 * a temporary condition, however, so we don't want to
			 * permanently block access to the file.
			 *
			 * TODO: Try other fileservers if we can.
			 *
			 * TODO: Retry a few times with sleeps.
			 */
<<<<<<< HEAD
			if (vnode->volume->servers == fc->server_list) {
				fc->ac.error = -ENOMEDIUM;
=======
			if (rcu_access_pointer(op->volume->servers) == op->server_list) {
				op->error = -ENOMEDIUM;
>>>>>>> upstream/android-13
				goto failed;
			}

			goto restart_from_beginning;

		default:
<<<<<<< HEAD
			clear_bit(AFS_VOLUME_OFFLINE, &vnode->volume->flags);
			clear_bit(AFS_VOLUME_BUSY, &vnode->volume->flags);
			fc->ac.error = afs_abort_to_error(fc->ac.abort_code);
			goto failed;
		}

	case -ENETUNREACH:
	case -EHOSTUNREACH:
	case -ECONNREFUSED:
	case -ETIMEDOUT:
	case -ETIME:
		_debug("no conn");
=======
			clear_bit(AFS_VOLUME_OFFLINE, &op->volume->flags);
			clear_bit(AFS_VOLUME_BUSY, &op->volume->flags);
			op->error = afs_abort_to_error(op->ac.abort_code);
			goto failed;
		}

	case -ETIMEDOUT:
	case -ETIME:
		if (op->error != -EDESTADDRREQ)
			goto iterate_address;
		fallthrough;
	case -ERFKILL:
	case -EADDRNOTAVAIL:
	case -ENETUNREACH:
	case -EHOSTUNREACH:
	case -EHOSTDOWN:
	case -ECONNREFUSED:
		_debug("no conn");
		op->error = error;
>>>>>>> upstream/android-13
		goto iterate_address;

	case -ECONNRESET:
		_debug("call reset");
<<<<<<< HEAD
=======
		op->error = error;
>>>>>>> upstream/android-13
		goto failed;
	}

restart_from_beginning:
	_debug("restart");
<<<<<<< HEAD
	afs_end_cursor(&fc->ac);
	afs_put_cb_interest(afs_v2net(vnode), fc->cbi);
	fc->cbi = NULL;
	afs_put_serverlist(afs_v2net(vnode), fc->server_list);
	fc->server_list = NULL;
=======
	afs_end_cursor(&op->ac);
	op->server = NULL;
	afs_put_serverlist(op->net, op->server_list);
	op->server_list = NULL;
>>>>>>> upstream/android-13
start:
	_debug("start");
	/* See if we need to do an update of the volume record.  Note that the
	 * volume may have moved or even have been deleted.
	 */
<<<<<<< HEAD
	fc->ac.error = afs_check_volume_status(vnode->volume, fc->key);
	if (fc->ac.error < 0)
		goto failed;

	if (!afs_start_fs_iteration(fc, vnode))
		goto failed;

use_server:
	_debug("use");
=======
	error = afs_check_volume_status(op->volume, op);
	if (error < 0)
		goto failed_set_error;

	if (!afs_start_fs_iteration(op, vnode))
		goto failed;

	_debug("__ VOL %llx __", op->volume->vid);

pick_server:
	_debug("pick [%lx]", op->untried);

	error = afs_wait_for_fs_probes(op->server_list, op->untried);
	if (error < 0)
		goto failed_set_error;

	/* Pick the untried server with the lowest RTT.  If we have outstanding
	 * callbacks, we stick with the server we're already using if we can.
	 */
	if (op->server) {
		_debug("server %u", op->index);
		if (test_bit(op->index, &op->untried))
			goto selected_server;
		op->server = NULL;
		_debug("no server");
	}

	op->index = -1;
	rtt = U32_MAX;
	for (i = 0; i < op->server_list->nr_servers; i++) {
		struct afs_server *s = op->server_list->servers[i].server;

		if (!test_bit(i, &op->untried) ||
		    !test_bit(AFS_SERVER_FL_RESPONDING, &s->flags))
			continue;
		if (s->probe.rtt < rtt) {
			op->index = i;
			rtt = s->probe.rtt;
		}
	}

	if (op->index == -1)
		goto no_more_servers;

selected_server:
	_debug("use %d", op->index);
	__clear_bit(op->index, &op->untried);

>>>>>>> upstream/android-13
	/* We're starting on a different fileserver from the list.  We need to
	 * check it, create a callback intercept, find its address list and
	 * probe its capabilities before we use it.
	 */
<<<<<<< HEAD
	ASSERTCMP(fc->ac.alist, ==, NULL);
	server = fc->server_list->servers[fc->index].server;

	if (!afs_check_server_record(fc, server))
=======
	ASSERTCMP(op->ac.alist, ==, NULL);
	server = op->server_list->servers[op->index].server;

	if (!afs_check_server_record(op, server))
>>>>>>> upstream/android-13
		goto failed;

	_debug("USING SERVER: %pU", &server->uuid);

<<<<<<< HEAD
	/* Make sure we've got a callback interest record for this server.  We
	 * have to link it in before we send the request as we can be sent a
	 * break request before we've finished decoding the reply and
	 * installing the vnode.
	 */
	fc->ac.error = afs_register_server_cb_interest(vnode, fc->server_list,
						       fc->index);
	if (fc->ac.error < 0)
		goto failed;

	fc->cbi = afs_get_cb_interest(vnode->cb_interest);
=======
	op->flags |= AFS_OPERATION_RETRY_SERVER;
	op->server = server;
	if (vnode->cb_server != server) {
		vnode->cb_server = server;
		vnode->cb_s_break = server->cb_s_break;
		vnode->cb_fs_s_break = atomic_read(&server->cell->fs_s_break);
		vnode->cb_v_break = vnode->volume->cb_v_break;
		clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags);
	}
>>>>>>> upstream/android-13

	read_lock(&server->fs_lock);
	alist = rcu_dereference_protected(server->addresses,
					  lockdep_is_held(&server->fs_lock));
	afs_get_addrlist(alist);
	read_unlock(&server->fs_lock);

<<<<<<< HEAD
	memset(&fc->ac, 0, sizeof(fc->ac));

	/* Probe the current fileserver if we haven't done so yet. */
	if (!test_bit(AFS_SERVER_FL_PROBED, &server->flags)) {
		fc->ac.alist = afs_get_addrlist(alist);

		if (!afs_probe_fileserver(fc)) {
			switch (fc->ac.error) {
			case -ENOMEM:
			case -ERESTARTSYS:
			case -EINTR:
				goto failed;
			default:
				goto next_server;
			}
		}
	}

	if (!fc->ac.alist)
		fc->ac.alist = alist;
	else
		afs_put_addrlist(alist);

	fc->ac.start = READ_ONCE(alist->index);
	fc->ac.index = fc->ac.start;

iterate_address:
	ASSERT(fc->ac.alist);
	_debug("iterate %d/%d", fc->ac.index, fc->ac.alist->nr_addrs);
	/* Iterate over the current server's address list to try and find an
	 * address on which it will respond to us.
	 */
	if (!afs_iterate_addresses(&fc->ac))
		goto next_server;
=======
retry_server:
	memset(&op->ac, 0, sizeof(op->ac));

	if (!op->ac.alist)
		op->ac.alist = alist;
	else
		afs_put_addrlist(alist);

	op->ac.index = -1;

iterate_address:
	ASSERT(op->ac.alist);
	/* Iterate over the current server's address list to try and find an
	 * address on which it will respond to us.
	 */
	if (!afs_iterate_addresses(&op->ac))
		goto out_of_addresses;

	_debug("address [%u] %u/%u %pISp",
	       op->index, op->ac.index, op->ac.alist->nr_addrs,
	       &op->ac.alist->addrs[op->ac.index].transport);
>>>>>>> upstream/android-13

	_leave(" = t");
	return true;

<<<<<<< HEAD
next_server:
	_debug("next");
	afs_end_cursor(&fc->ac);
	afs_put_cb_interest(afs_v2net(vnode), fc->cbi);
	fc->cbi = NULL;
	fc->index++;
	if (fc->index >= fc->server_list->nr_servers)
		fc->index = 0;
	if (fc->index != fc->start)
		goto use_server;

	/* That's all the servers poked to no good effect.  Try again if some
	 * of them were busy.
	 */
	if (fc->flags & AFS_FS_CURSOR_VBUSY)
		goto restart_from_beginning;

	fc->ac.error = -EDESTADDRREQ;
	goto failed;

failed:
	fc->flags |= AFS_FS_CURSOR_STOP;
	afs_end_cursor(&fc->ac);
	_leave(" = f [failed %d]", fc->ac.error);
=======
out_of_addresses:
	/* We've now had a failure to respond on all of a server's addresses -
	 * immediately probe them again and consider retrying the server.
	 */
	afs_probe_fileserver(op->net, op->server);
	if (op->flags & AFS_OPERATION_RETRY_SERVER) {
		alist = op->ac.alist;
		error = afs_wait_for_one_fs_probe(
			op->server, !(op->flags & AFS_OPERATION_UNINTR));
		switch (error) {
		case 0:
			op->flags &= ~AFS_OPERATION_RETRY_SERVER;
			goto retry_server;
		case -ERESTARTSYS:
			goto failed_set_error;
		case -ETIME:
		case -EDESTADDRREQ:
			goto next_server;
		}
	}

next_server:
	_debug("next");
	afs_end_cursor(&op->ac);
	goto pick_server;

no_more_servers:
	/* That's all the servers poked to no good effect.  Try again if some
	 * of them were busy.
	 */
	if (op->flags & AFS_OPERATION_VBUSY)
		goto restart_from_beginning;

	e.error = -EDESTADDRREQ;
	e.responded = false;
	for (i = 0; i < op->server_list->nr_servers; i++) {
		struct afs_server *s = op->server_list->servers[i].server;

		afs_prioritise_error(&e, READ_ONCE(s->probe.error),
				     s->probe.abort_code);
	}

	error = e.error;

failed_set_error:
	op->error = error;
failed:
	op->flags |= AFS_OPERATION_STOP;
	afs_end_cursor(&op->ac);
	_leave(" = f [failed %d]", op->error);
>>>>>>> upstream/android-13
	return false;
}

/*
<<<<<<< HEAD
 * Select the same fileserver we used for a vnode before and only that
 * fileserver.  We use this when we have a lock on that file, which is backed
 * only by the fileserver we obtained it from.
 */
bool afs_select_current_fileserver(struct afs_fs_cursor *fc)
{
	struct afs_vnode *vnode = fc->vnode;
	struct afs_cb_interest *cbi = vnode->cb_interest;
	struct afs_addr_list *alist;

	_enter("");

	switch (fc->ac.error) {
	case SHRT_MAX:
		if (!cbi) {
			fc->ac.error = -ESTALE;
			fc->flags |= AFS_FS_CURSOR_STOP;
			return false;
		}

		fc->cbi = afs_get_cb_interest(vnode->cb_interest);

		read_lock(&cbi->server->fs_lock);
		alist = rcu_dereference_protected(cbi->server->addresses,
						  lockdep_is_held(&cbi->server->fs_lock));
		afs_get_addrlist(alist);
		read_unlock(&cbi->server->fs_lock);
		if (!alist) {
			fc->ac.error = -ESTALE;
			fc->flags |= AFS_FS_CURSOR_STOP;
			return false;
		}

		memset(&fc->ac, 0, sizeof(fc->ac));
		fc->ac.alist = alist;
		fc->ac.start = READ_ONCE(alist->index);
		fc->ac.index = fc->ac.start;
		goto iterate_address;

	case 0:
	default:
		/* Success or local failure.  Stop. */
		fc->flags |= AFS_FS_CURSOR_STOP;
		_leave(" = f [okay/local %d]", fc->ac.error);
		return false;

	case -ECONNABORTED:
		fc->flags |= AFS_FS_CURSOR_STOP;
		_leave(" = f [abort]");
		return false;

	case -ENETUNREACH:
	case -EHOSTUNREACH:
	case -ECONNREFUSED:
	case -ETIMEDOUT:
	case -ETIME:
		_debug("no conn");
		goto iterate_address;
	}

iterate_address:
	/* Iterate over the current server's address list to try and find an
	 * address on which it will respond to us.
	 */
	if (afs_iterate_addresses(&fc->ac)) {
		_leave(" = t");
		return true;
	}

	afs_end_cursor(&fc->ac);
	return false;
}

/*
 * Tidy up a filesystem cursor and unlock the vnode.
 */
int afs_end_vnode_operation(struct afs_fs_cursor *fc)
{
	struct afs_net *net = afs_v2net(fc->vnode);
	int ret;

	mutex_unlock(&fc->vnode->io_lock);

	afs_end_cursor(&fc->ac);
	afs_put_cb_interest(net, fc->cbi);
	afs_put_serverlist(net, fc->server_list);

	ret = fc->ac.error;
	if (ret == -ECONNABORTED)
		afs_abort_to_error(fc->ac.abort_code);

	return fc->ac.error;
=======
 * Dump cursor state in the case of the error being EDESTADDRREQ.
 */
void afs_dump_edestaddrreq(const struct afs_operation *op)
{
	static int count;
	int i;

	if (!IS_ENABLED(CONFIG_AFS_DEBUG_CURSOR) || count > 3)
		return;
	count++;

	rcu_read_lock();

	pr_notice("EDESTADDR occurred\n");
	pr_notice("FC: cbb=%x cbb2=%x fl=%x err=%hd\n",
		  op->file[0].cb_break_before,
		  op->file[1].cb_break_before, op->flags, op->error);
	pr_notice("FC: ut=%lx ix=%d ni=%u\n",
		  op->untried, op->index, op->nr_iterations);

	if (op->server_list) {
		const struct afs_server_list *sl = op->server_list;
		pr_notice("FC: SL nr=%u pr=%u vnov=%hx\n",
			  sl->nr_servers, sl->preferred, sl->vnovol_mask);
		for (i = 0; i < sl->nr_servers; i++) {
			const struct afs_server *s = sl->servers[i].server;
			pr_notice("FC: server fl=%lx av=%u %pU\n",
				  s->flags, s->addr_version, &s->uuid);
			if (s->addresses) {
				const struct afs_addr_list *a =
					rcu_dereference(s->addresses);
				pr_notice("FC:  - av=%u nr=%u/%u/%u pr=%u\n",
					  a->version,
					  a->nr_ipv4, a->nr_addrs, a->max_addrs,
					  a->preferred);
				pr_notice("FC:  - R=%lx F=%lx\n",
					  a->responded, a->failed);
				if (a == op->ac.alist)
					pr_notice("FC:  - current\n");
			}
		}
	}

	pr_notice("AC: t=%lx ax=%u ac=%d er=%d r=%u ni=%u\n",
		  op->ac.tried, op->ac.index, op->ac.abort_code, op->ac.error,
		  op->ac.responded, op->ac.nr_iterations);
	rcu_read_unlock();
>>>>>>> upstream/android-13
}
