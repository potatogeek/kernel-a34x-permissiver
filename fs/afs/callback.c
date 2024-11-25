/*
 * Copyright (c) 2002, 2007 Red Hat, Inc. All rights reserved.
 *
 * This software may be freely redistributed under the terms of the
 * GNU General Public License.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Authors: David Woodhouse <dwmw2@infradead.org>
 *          David Howells <dhowells@redhat.com>
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/circ_buf.h>
#include <linux/sched.h>
#include "internal.h"

/*
<<<<<<< HEAD
 * Create volume and callback interests on a server.
 */
static struct afs_cb_interest *afs_create_interest(struct afs_server *server,
						   struct afs_vnode *vnode)
{
	struct afs_vol_interest *new_vi, *vi;
	struct afs_cb_interest *new;
	struct hlist_node **pp;

	new_vi = kzalloc(sizeof(struct afs_vol_interest), GFP_KERNEL);
	if (!new_vi)
		return NULL;

	new = kzalloc(sizeof(struct afs_cb_interest), GFP_KERNEL);
	if (!new) {
		kfree(new_vi);
		return NULL;
	}

	new_vi->usage = 1;
	new_vi->vid = vnode->volume->vid;
	INIT_HLIST_NODE(&new_vi->srv_link);
	INIT_HLIST_HEAD(&new_vi->cb_interests);

	refcount_set(&new->usage, 1);
	new->sb = vnode->vfs_inode.i_sb;
	new->vid = vnode->volume->vid;
	new->server = afs_get_server(server);
	INIT_HLIST_NODE(&new->cb_vlink);

	write_lock(&server->cb_break_lock);

	for (pp = &server->cb_volumes.first; *pp; pp = &(*pp)->next) {
		vi = hlist_entry(*pp, struct afs_vol_interest, srv_link);
		if (vi->vid < new_vi->vid)
			continue;
		if (vi->vid > new_vi->vid)
			break;
		vi->usage++;
		goto found_vi;
	}

	new_vi->srv_link.pprev = pp;
	new_vi->srv_link.next = *pp;
	if (*pp)
		(*pp)->pprev = &new_vi->srv_link.next;
	*pp = &new_vi->srv_link;
	vi = new_vi;
	new_vi = NULL;
found_vi:

	new->vol_interest = vi;
	hlist_add_head(&new->cb_vlink, &vi->cb_interests);

	write_unlock(&server->cb_break_lock);
	kfree(new_vi);
	return new;
}

/*
 * Set up an interest-in-callbacks record for a volume on a server and
 * register it with the server.
 * - Called with vnode->io_lock held.
 */
int afs_register_server_cb_interest(struct afs_vnode *vnode,
				    struct afs_server_list *slist,
				    unsigned int index)
{
	struct afs_server_entry *entry = &slist->servers[index];
	struct afs_cb_interest *cbi, *vcbi, *new, *old;
	struct afs_server *server = entry->server;

again:
	if (vnode->cb_interest &&
	    likely(vnode->cb_interest == entry->cb_interest))
		return 0;

	read_lock(&slist->lock);
	cbi = afs_get_cb_interest(entry->cb_interest);
	read_unlock(&slist->lock);

	vcbi = vnode->cb_interest;
	if (vcbi) {
		if (vcbi == cbi) {
			afs_put_cb_interest(afs_v2net(vnode), cbi);
			return 0;
		}

		/* Use a new interest in the server list for the same server
		 * rather than an old one that's still attached to a vnode.
		 */
		if (cbi && vcbi->server == cbi->server) {
			write_seqlock(&vnode->cb_lock);
			old = vnode->cb_interest;
			vnode->cb_interest = cbi;
			write_sequnlock(&vnode->cb_lock);
			afs_put_cb_interest(afs_v2net(vnode), old);
			return 0;
		}

		/* Re-use the one attached to the vnode. */
		if (!cbi && vcbi->server == server) {
			write_lock(&slist->lock);
			if (entry->cb_interest) {
				write_unlock(&slist->lock);
				afs_put_cb_interest(afs_v2net(vnode), cbi);
				goto again;
			}

			entry->cb_interest = cbi;
			write_unlock(&slist->lock);
			return 0;
		}
	}

	if (!cbi) {
		new = afs_create_interest(server, vnode);
		if (!new)
			return -ENOMEM;

		write_lock(&slist->lock);
		if (!entry->cb_interest) {
			entry->cb_interest = afs_get_cb_interest(new);
			cbi = new;
			new = NULL;
		} else {
			cbi = afs_get_cb_interest(entry->cb_interest);
		}
		write_unlock(&slist->lock);
		afs_put_cb_interest(afs_v2net(vnode), new);
	}

	ASSERT(cbi);

	/* Change the server the vnode is using.  This entails scrubbing any
	 * interest the vnode had in the previous server it was using.
	 */
	write_seqlock(&vnode->cb_lock);

	old = vnode->cb_interest;
	vnode->cb_interest = cbi;
	vnode->cb_s_break = cbi->server->cb_s_break;
	vnode->cb_v_break = vnode->volume->cb_v_break;
	clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags);

	write_sequnlock(&vnode->cb_lock);
	afs_put_cb_interest(afs_v2net(vnode), old);
	return 0;
}

/*
 * Remove an interest on a server.
 */
void afs_put_cb_interest(struct afs_net *net, struct afs_cb_interest *cbi)
{
	struct afs_vol_interest *vi;

	if (cbi && refcount_dec_and_test(&cbi->usage)) {
		if (!hlist_unhashed(&cbi->cb_vlink)) {
			write_lock(&cbi->server->cb_break_lock);

			hlist_del_init(&cbi->cb_vlink);
			vi = cbi->vol_interest;
			cbi->vol_interest = NULL;
			if (--vi->usage == 0)
				hlist_del(&vi->srv_link);
			else
				vi = NULL;

			write_unlock(&cbi->server->cb_break_lock);
			kfree(vi);
			afs_put_server(net, cbi->server);
		}
		kfree(cbi);
	}
}

/*
 * allow the fileserver to request callback state (re-)initialisation
 */
void afs_init_callback_state(struct afs_server *server)
{
	if (!test_and_clear_bit(AFS_SERVER_FL_NEW, &server->flags))
		server->cb_s_break++;
=======
 * Handle invalidation of an mmap'd file.  We invalidate all the PTEs referring
 * to the pages in this file's pagecache, forcing the kernel to go through
 * ->fault() or ->page_mkwrite() - at which point we can handle invalidation
 * more fully.
 */
void afs_invalidate_mmap_work(struct work_struct *work)
{
	struct afs_vnode *vnode = container_of(work, struct afs_vnode, cb_work);

	unmap_mapping_pages(vnode->vfs_inode.i_mapping, 0, 0, false);
}

void afs_server_init_callback_work(struct work_struct *work)
{
	struct afs_server *server = container_of(work, struct afs_server, initcb_work);
	struct afs_vnode *vnode;
	struct afs_cell *cell = server->cell;

	down_read(&cell->fs_open_mmaps_lock);

	list_for_each_entry(vnode, &cell->fs_open_mmaps, cb_mmap_link) {
		if (vnode->cb_server == server) {
			clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags);
			queue_work(system_unbound_wq, &vnode->cb_work);
		}
	}

	up_read(&cell->fs_open_mmaps_lock);
}

/*
 * Allow the fileserver to request callback state (re-)initialisation.
 * Unfortunately, UUIDs are not guaranteed unique.
 */
void afs_init_callback_state(struct afs_server *server)
{
	rcu_read_lock();
	do {
		server->cb_s_break++;
		atomic_inc(&server->cell->fs_s_break);
		if (!list_empty(&server->cell->fs_open_mmaps))
			queue_work(system_unbound_wq, &server->initcb_work);

	} while ((server = rcu_dereference(server->uuid_next)));
	rcu_read_unlock();
>>>>>>> upstream/android-13
}

/*
 * actually break a callback
 */
<<<<<<< HEAD
void afs_break_callback(struct afs_vnode *vnode)
{
	_enter("");

	write_seqlock(&vnode->cb_lock);

	clear_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags);
	if (test_and_clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags)) {
		vnode->cb_break++;
=======
void __afs_break_callback(struct afs_vnode *vnode, enum afs_cb_break_reason reason)
{
	_enter("");

	clear_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags);
	if (test_and_clear_bit(AFS_VNODE_CB_PROMISED, &vnode->flags)) {
		vnode->cb_break++;
		vnode->cb_v_break = vnode->volume->cb_v_break;
>>>>>>> upstream/android-13
		afs_clear_permits(vnode);

		if (vnode->lock_state == AFS_VNODE_LOCK_WAITING_FOR_CB)
			afs_lock_may_be_available(vnode);
<<<<<<< HEAD
	}

=======

		if (reason != afs_cb_break_for_deleted &&
		    vnode->status.type == AFS_FTYPE_FILE &&
		    atomic_read(&vnode->cb_nr_mmap))
			queue_work(system_unbound_wq, &vnode->cb_work);

		trace_afs_cb_break(&vnode->fid, vnode->cb_break, reason, true);
	} else {
		trace_afs_cb_break(&vnode->fid, vnode->cb_break, reason, false);
	}
}

void afs_break_callback(struct afs_vnode *vnode, enum afs_cb_break_reason reason)
{
	write_seqlock(&vnode->cb_lock);
	__afs_break_callback(vnode, reason);
>>>>>>> upstream/android-13
	write_sequnlock(&vnode->cb_lock);
}

/*
<<<<<<< HEAD
=======
 * Look up a volume by volume ID under RCU conditions.
 */
static struct afs_volume *afs_lookup_volume_rcu(struct afs_cell *cell,
						afs_volid_t vid)
{
	struct afs_volume *volume = NULL;
	struct rb_node *p;
	int seq = 0;

	do {
		/* Unfortunately, rbtree walking doesn't give reliable results
		 * under just the RCU read lock, so we have to check for
		 * changes.
		 */
		read_seqbegin_or_lock(&cell->volume_lock, &seq);

		p = rcu_dereference_raw(cell->volumes.rb_node);
		while (p) {
			volume = rb_entry(p, struct afs_volume, cell_node);

			if (volume->vid < vid)
				p = rcu_dereference_raw(p->rb_left);
			else if (volume->vid > vid)
				p = rcu_dereference_raw(p->rb_right);
			else
				break;
			volume = NULL;
		}

	} while (need_seqretry(&cell->volume_lock, seq));

	done_seqretry(&cell->volume_lock, seq);
	return volume;
}

/*
>>>>>>> upstream/android-13
 * allow the fileserver to explicitly break one callback
 * - happens when
 *   - the backing file is changed
 *   - a lock is released
 */
<<<<<<< HEAD
static void afs_break_one_callback(struct afs_server *server,
				   struct afs_fid *fid)
{
	struct afs_vol_interest *vi;
	struct afs_cb_interest *cbi;
	struct afs_iget_data data;
	struct afs_vnode *vnode;
	struct inode *inode;

	read_lock(&server->cb_break_lock);
	hlist_for_each_entry(vi, &server->cb_volumes, srv_link) {
		if (vi->vid < fid->vid)
			continue;
		if (vi->vid > fid->vid) {
			vi = NULL;
			break;
		}
		//atomic_inc(&vi->usage);
		break;
	}

	/* TODO: Find all matching volumes if we couldn't match the server and
	 * break them anyway.
	 */
	if (!vi)
		goto out;

	/* Step through all interested superblocks.  There may be more than one
	 * because of cell aliasing.
	 */
	hlist_for_each_entry(cbi, &vi->cb_interests, cb_vlink) {
		if (fid->vnode == 0 && fid->unique == 0) {
			/* The callback break applies to an entire volume. */
			struct afs_super_info *as = AFS_FS_S(cbi->sb);
			struct afs_volume *volume = as->volume;

			write_lock(&volume->cb_v_break_lock);
			volume->cb_v_break++;
			write_unlock(&volume->cb_v_break_lock);
		} else {
			data.volume = NULL;
			data.fid = *fid;
			inode = ilookup5_nowait(cbi->sb, fid->vnode,
						afs_iget5_test, &data);
			if (inode) {
				vnode = AFS_FS_I(inode);
				afs_break_callback(vnode);
				iput(inode);
			}
		}
	}

out:
	read_unlock(&server->cb_break_lock);
=======
static void afs_break_one_callback(struct afs_volume *volume,
				   struct afs_fid *fid)
{
	struct super_block *sb;
	struct afs_vnode *vnode;
	struct inode *inode;

	if (fid->vnode == 0 && fid->unique == 0) {
		/* The callback break applies to an entire volume. */
		write_lock(&volume->cb_v_break_lock);
		volume->cb_v_break++;
		trace_afs_cb_break(fid, volume->cb_v_break,
				   afs_cb_break_for_volume_callback, false);
		write_unlock(&volume->cb_v_break_lock);
		return;
	}

	/* See if we can find a matching inode - even an I_NEW inode needs to
	 * be marked as it can have its callback broken before we finish
	 * setting up the local inode.
	 */
	sb = rcu_dereference(volume->sb);
	if (!sb)
		return;

	inode = find_inode_rcu(sb, fid->vnode, afs_ilookup5_test_by_fid, fid);
	if (inode) {
		vnode = AFS_FS_I(inode);
		afs_break_callback(vnode, afs_cb_break_for_callback);
	} else {
		trace_afs_cb_miss(fid, afs_cb_break_for_callback);
	}
}

static void afs_break_some_callbacks(struct afs_server *server,
				     struct afs_callback_break *cbb,
				     size_t *_count)
{
	struct afs_callback_break *residue = cbb;
	struct afs_volume *volume;
	afs_volid_t vid = cbb->fid.vid;
	size_t i;

	volume = afs_lookup_volume_rcu(server->cell, vid);

	/* TODO: Find all matching volumes if we couldn't match the server and
	 * break them anyway.
	 */

	for (i = *_count; i > 0; cbb++, i--) {
		if (cbb->fid.vid == vid) {
			_debug("- Fid { vl=%08llx n=%llu u=%u }",
			       cbb->fid.vid,
			       cbb->fid.vnode,
			       cbb->fid.unique);
			--*_count;
			if (volume)
				afs_break_one_callback(volume, &cbb->fid);
		} else {
			*residue++ = *cbb;
		}
	}
>>>>>>> upstream/android-13
}

/*
 * allow the fileserver to break callback promises
 */
void afs_break_callbacks(struct afs_server *server, size_t count,
			 struct afs_callback_break *callbacks)
{
	_enter("%p,%zu,", server, count);

	ASSERT(server != NULL);
<<<<<<< HEAD
	ASSERTCMP(count, <=, AFSCBMAX);

	/* TODO: Sort the callback break list by volume ID */

	for (; count > 0; callbacks++, count--) {
		_debug("- Fid { vl=%08x n=%u u=%u }  CB { v=%u x=%u t=%u }",
		       callbacks->fid.vid,
		       callbacks->fid.vnode,
		       callbacks->fid.unique,
		       callbacks->cb.version,
		       callbacks->cb.expiry,
		       callbacks->cb.type
		       );
		afs_break_one_callback(server, &callbacks->fid);
	}

	_leave("");
	return;
}

/*
 * Clear the callback interests in a server list.
 */
void afs_clear_callback_interests(struct afs_net *net, struct afs_server_list *slist)
{
	int i;

	for (i = 0; i < slist->nr_servers; i++) {
		afs_put_cb_interest(net, slist->servers[i].cb_interest);
		slist->servers[i].cb_interest = NULL;
	}
}
=======

	rcu_read_lock();

	while (count > 0)
		afs_break_some_callbacks(server, callbacks, &count);

	rcu_read_unlock();
	return;
}
>>>>>>> upstream/android-13
