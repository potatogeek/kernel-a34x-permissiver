<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* handling of writes to regular files and writing back to the server
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
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

#include <linux/backing-dev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/writeback.h>
#include <linux/pagevec.h>
<<<<<<< HEAD
=======
#include <linux/netfs.h>
#include <linux/fscache.h>
>>>>>>> upstream/android-13
#include "internal.h"

/*
 * mark a page as having been made dirty and thus needing writeback
 */
int afs_set_page_dirty(struct page *page)
{
	_enter("");
	return __set_page_dirty_nobuffers(page);
}

/*
<<<<<<< HEAD
 * partly or wholly fill a page that's under preparation for writing
 */
static int afs_fill_page(struct afs_vnode *vnode, struct key *key,
			 loff_t pos, unsigned int len, struct page *page)
{
	struct afs_read *req;
	int ret;

	_enter(",,%llu", (unsigned long long)pos);

	req = kzalloc(sizeof(struct afs_read) + sizeof(struct page *),
		      GFP_KERNEL);
	if (!req)
		return -ENOMEM;

	refcount_set(&req->usage, 1);
	req->pos = pos;
	req->len = len;
	req->nr_pages = 1;
	req->pages = req->array;
	req->pages[0] = page;
	get_page(page);

	ret = afs_fetch_data(vnode, key, req);
	afs_put_read(req);
	if (ret < 0) {
		if (ret == -ENOENT) {
			_debug("got NOENT from server"
			       " - marking file deleted and stale");
			set_bit(AFS_VNODE_DELETED, &vnode->flags);
			ret = -ESTALE;
		}
	}

	_leave(" = %d", ret);
	return ret;
}

/*
=======
>>>>>>> upstream/android-13
 * prepare to perform part of a write to a page
 */
int afs_write_begin(struct file *file, struct address_space *mapping,
		    loff_t pos, unsigned len, unsigned flags,
<<<<<<< HEAD
		    struct page **pagep, void **fsdata)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));
	struct page *page;
	struct key *key = afs_file_key(file);
	unsigned long priv;
	unsigned f, from = pos & (PAGE_SIZE - 1);
	unsigned t, to = from + len;
	pgoff_t index = pos >> PAGE_SHIFT;
	int ret;

	_enter("{%x:%u},{%lx},%u,%u",
	       vnode->fid.vid, vnode->fid.vnode, index, from, to);

	/* We want to store information about how much of a page is altered in
	 * page->private.
	 */
	BUILD_BUG_ON(PAGE_SIZE > 32768 && sizeof(page->private) < 8);

	page = grab_cache_page_write_begin(mapping, index, flags);
	if (!page)
		return -ENOMEM;

	if (!PageUptodate(page) && len != PAGE_SIZE) {
		ret = afs_fill_page(vnode, key, pos & PAGE_MASK, PAGE_SIZE, page);
		if (ret < 0) {
			unlock_page(page);
			put_page(page);
			_leave(" = %d [prep]", ret);
			return ret;
		}
		SetPageUptodate(page);
	}

	/* page won't leak in error case: it eventually gets cleaned off LRU */
	*pagep = page;
=======
		    struct page **_page, void **fsdata)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));
	struct page *page;
	unsigned long priv;
	unsigned f, from;
	unsigned t, to;
	pgoff_t index;
	int ret;

	_enter("{%llx:%llu},%llx,%x",
	       vnode->fid.vid, vnode->fid.vnode, pos, len);

	/* Prefetch area to be written into the cache if we're caching this
	 * file.  We need to do this before we get a lock on the page in case
	 * there's more than one writer competing for the same cache block.
	 */
	ret = netfs_write_begin(file, mapping, pos, len, flags, &page, fsdata,
				&afs_req_ops, NULL);
	if (ret < 0)
		return ret;

	index = page->index;
	from = pos - index * PAGE_SIZE;
	to = from + len;
>>>>>>> upstream/android-13

try_again:
	/* See if this page is already partially written in a way that we can
	 * merge the new write with.
	 */
<<<<<<< HEAD
	t = f = 0;
	if (PagePrivate(page)) {
		priv = page_private(page);
		f = priv & AFS_PRIV_MAX;
		t = priv >> AFS_PRIV_SHIFT;
		ASSERTCMP(f, <=, t);
	}

	if (f != t) {
		if (PageWriteback(page)) {
			trace_afs_page_dirty(vnode, tracepoint_string("alrdy"),
					     page->index, priv);
=======
	if (PagePrivate(page)) {
		priv = page_private(page);
		f = afs_page_dirty_from(page, priv);
		t = afs_page_dirty_to(page, priv);
		ASSERTCMP(f, <=, t);

		if (PageWriteback(page)) {
			trace_afs_page_dirty(vnode, tracepoint_string("alrdy"), page);
>>>>>>> upstream/android-13
			goto flush_conflicting_write;
		}
		/* If the file is being filled locally, allow inter-write
		 * spaces to be merged into writes.  If it's not, only write
		 * back what the user gives us.
		 */
		if (!test_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags) &&
		    (to < f || from > t))
			goto flush_conflicting_write;
<<<<<<< HEAD
		if (from < f)
			f = from;
		if (to > t)
			t = to;
	} else {
		f = from;
		t = to;
	}

	priv = (unsigned long)t << AFS_PRIV_SHIFT;
	priv |= f;
	trace_afs_page_dirty(vnode, tracepoint_string("begin"),
			     page->index, priv);
	SetPagePrivate(page);
	set_page_private(page, priv);
=======
	}

	*_page = page;
>>>>>>> upstream/android-13
	_leave(" = 0");
	return 0;

	/* The previous write and this write aren't adjacent or overlapping, so
	 * flush the page out.
	 */
flush_conflicting_write:
	_debug("flush conflict");
	ret = write_one_page(page);
<<<<<<< HEAD
	if (ret < 0) {
		_leave(" = %d", ret);
		return ret;
	}

	ret = lock_page_killable(page);
	if (ret < 0) {
		_leave(" = %d", ret);
		return ret;
	}
	goto try_again;
=======
	if (ret < 0)
		goto error;

	ret = lock_page_killable(page);
	if (ret < 0)
		goto error;
	goto try_again;

error:
	put_page(page);
	_leave(" = %d", ret);
	return ret;
>>>>>>> upstream/android-13
}

/*
 * finalise part of a write to a page
 */
int afs_write_end(struct file *file, struct address_space *mapping,
		  loff_t pos, unsigned len, unsigned copied,
		  struct page *page, void *fsdata)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));
<<<<<<< HEAD
	struct key *key = afs_file_key(file);
	loff_t i_size, maybe_i_size;
	int ret;

	_enter("{%x:%u},{%lx}",
	       vnode->fid.vid, vnode->fid.vnode, page->index);

=======
	unsigned long priv;
	unsigned int f, from = pos & (thp_size(page) - 1);
	unsigned int t, to = from + copied;
	loff_t i_size, maybe_i_size;

	_enter("{%llx:%llu},{%lx}",
	       vnode->fid.vid, vnode->fid.vnode, page->index);

	if (!PageUptodate(page)) {
		if (copied < len) {
			copied = 0;
			goto out;
		}

		SetPageUptodate(page);
	}

	if (copied == 0)
		goto out;

>>>>>>> upstream/android-13
	maybe_i_size = pos + copied;

	i_size = i_size_read(&vnode->vfs_inode);
	if (maybe_i_size > i_size) {
		write_seqlock(&vnode->cb_lock);
		i_size = i_size_read(&vnode->vfs_inode);
		if (maybe_i_size > i_size)
<<<<<<< HEAD
			i_size_write(&vnode->vfs_inode, maybe_i_size);
		write_sequnlock(&vnode->cb_lock);
	}

	if (!PageUptodate(page)) {
		if (copied < len) {
			/* Try and load any missing data from the server.  The
			 * unmarshalling routine will take care of clearing any
			 * bits that are beyond the EOF.
			 */
			ret = afs_fill_page(vnode, key, pos + copied,
					    len - copied, page);
			if (ret < 0)
				goto out;
		}
		SetPageUptodate(page);
	}

	set_page_dirty(page);
	if (PageDirty(page))
		_debug("dirtied");
	ret = copied;
=======
			afs_set_i_size(vnode, maybe_i_size);
		write_sequnlock(&vnode->cb_lock);
	}

	if (PagePrivate(page)) {
		priv = page_private(page);
		f = afs_page_dirty_from(page, priv);
		t = afs_page_dirty_to(page, priv);
		if (from < f)
			f = from;
		if (to > t)
			t = to;
		priv = afs_page_dirty(page, f, t);
		set_page_private(page, priv);
		trace_afs_page_dirty(vnode, tracepoint_string("dirty+"), page);
	} else {
		priv = afs_page_dirty(page, from, to);
		attach_page_private(page, (void *)priv);
		trace_afs_page_dirty(vnode, tracepoint_string("dirty"), page);
	}

	if (set_page_dirty(page))
		_debug("dirtied %lx", page->index);
>>>>>>> upstream/android-13

out:
	unlock_page(page);
	put_page(page);
<<<<<<< HEAD
	return ret;
=======
	return copied;
>>>>>>> upstream/android-13
}

/*
 * kill all the pages in the given range
 */
static void afs_kill_pages(struct address_space *mapping,
<<<<<<< HEAD
			   pgoff_t first, pgoff_t last)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct pagevec pv;
	unsigned count, loop;

	_enter("{%x:%u},%lx-%lx",
	       vnode->fid.vid, vnode->fid.vnode, first, last);
=======
			   loff_t start, loff_t len)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct pagevec pv;
	unsigned int loop, psize;

	_enter("{%llx:%llu},%llx @%llx",
	       vnode->fid.vid, vnode->fid.vnode, len, start);
>>>>>>> upstream/android-13

	pagevec_init(&pv);

	do {
<<<<<<< HEAD
		_debug("kill %lx-%lx", first, last);

		count = last - first + 1;
		if (count > PAGEVEC_SIZE)
			count = PAGEVEC_SIZE;
		pv.nr = find_get_pages_contig(mapping, first, count, pv.pages);
		ASSERTCMP(pv.nr, ==, count);

		for (loop = 0; loop < count; loop++) {
			struct page *page = pv.pages[loop];
			ClearPageUptodate(page);
			SetPageError(page);
			end_page_writeback(page);
			if (page->index >= first)
				first = page->index + 1;
=======
		_debug("kill %llx @%llx", len, start);

		pv.nr = find_get_pages_contig(mapping, start / PAGE_SIZE,
					      PAGEVEC_SIZE, pv.pages);
		if (pv.nr == 0)
			break;

		for (loop = 0; loop < pv.nr; loop++) {
			struct page *page = pv.pages[loop];

			if (page->index * PAGE_SIZE >= start + len)
				break;

			psize = thp_size(page);
			start += psize;
			len -= psize;
			ClearPageUptodate(page);
			end_page_writeback(page);
>>>>>>> upstream/android-13
			lock_page(page);
			generic_error_remove_page(mapping, page);
			unlock_page(page);
		}

		__pagevec_release(&pv);
<<<<<<< HEAD
	} while (first <= last);
=======
	} while (len > 0);
>>>>>>> upstream/android-13

	_leave("");
}

/*
 * Redirty all the pages in a given range.
 */
static void afs_redirty_pages(struct writeback_control *wbc,
			      struct address_space *mapping,
<<<<<<< HEAD
			      pgoff_t first, pgoff_t last)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct pagevec pv;
	unsigned count, loop;

	_enter("{%x:%u},%lx-%lx",
	       vnode->fid.vid, vnode->fid.vnode, first, last);
=======
			      loff_t start, loff_t len)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct pagevec pv;
	unsigned int loop, psize;

	_enter("{%llx:%llu},%llx @%llx",
	       vnode->fid.vid, vnode->fid.vnode, len, start);
>>>>>>> upstream/android-13

	pagevec_init(&pv);

	do {
<<<<<<< HEAD
		_debug("redirty %lx-%lx", first, last);

		count = last - first + 1;
		if (count > PAGEVEC_SIZE)
			count = PAGEVEC_SIZE;
		pv.nr = find_get_pages_contig(mapping, first, count, pv.pages);
		ASSERTCMP(pv.nr, ==, count);

		for (loop = 0; loop < count; loop++) {
			struct page *page = pv.pages[loop];

			redirty_page_for_writepage(wbc, page);
			end_page_writeback(page);
			if (page->index >= first)
				first = page->index + 1;
		}

		__pagevec_release(&pv);
	} while (first <= last);
=======
		_debug("redirty %llx @%llx", len, start);

		pv.nr = find_get_pages_contig(mapping, start / PAGE_SIZE,
					      PAGEVEC_SIZE, pv.pages);
		if (pv.nr == 0)
			break;

		for (loop = 0; loop < pv.nr; loop++) {
			struct page *page = pv.pages[loop];

			if (page->index * PAGE_SIZE >= start + len)
				break;

			psize = thp_size(page);
			start += psize;
			len -= psize;
			redirty_page_for_writepage(wbc, page);
			end_page_writeback(page);
		}

		__pagevec_release(&pv);
	} while (len > 0);
>>>>>>> upstream/android-13

	_leave("");
}

/*
<<<<<<< HEAD
 * write to a file
 */
static int afs_store_data(struct address_space *mapping,
			  pgoff_t first, pgoff_t last,
			  unsigned offset, unsigned to)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct afs_fs_cursor fc;
=======
 * completion of write to server
 */
static void afs_pages_written_back(struct afs_vnode *vnode, loff_t start, unsigned int len)
{
	struct address_space *mapping = vnode->vfs_inode.i_mapping;
	struct page *page;
	pgoff_t end;

	XA_STATE(xas, &mapping->i_pages, start / PAGE_SIZE);

	_enter("{%llx:%llu},{%x @%llx}",
	       vnode->fid.vid, vnode->fid.vnode, len, start);

	rcu_read_lock();

	end = (start + len - 1) / PAGE_SIZE;
	xas_for_each(&xas, page, end) {
		if (!PageWriteback(page)) {
			kdebug("bad %x @%llx page %lx %lx", len, start, page->index, end);
			ASSERT(PageWriteback(page));
		}

		trace_afs_page_dirty(vnode, tracepoint_string("clear"), page);
		detach_page_private(page);
		page_endio(page, true, 0);
	}

	rcu_read_unlock();

	afs_prune_wb_keys(vnode);
	_leave("");
}

/*
 * Find a key to use for the writeback.  We cached the keys used to author the
 * writes on the vnode.  *_wbk will contain the last writeback key used or NULL
 * and we need to start from there if it's set.
 */
static int afs_get_writeback_key(struct afs_vnode *vnode,
				 struct afs_wb_key **_wbk)
{
>>>>>>> upstream/android-13
	struct afs_wb_key *wbk = NULL;
	struct list_head *p;
	int ret = -ENOKEY, ret2;

<<<<<<< HEAD
	_enter("%s{%x:%u.%u},%lx,%lx,%x,%x",
	       vnode->volume->name,
	       vnode->fid.vid,
	       vnode->fid.vnode,
	       vnode->fid.unique,
	       first, last, offset, to);

	spin_lock(&vnode->wb_lock);
	p = vnode->wb_keys.next;

	/* Iterate through the list looking for a valid key to use. */
try_next_key:
=======
	spin_lock(&vnode->wb_lock);
	if (*_wbk)
		p = (*_wbk)->vnode_link.next;
	else
		p = vnode->wb_keys.next;

>>>>>>> upstream/android-13
	while (p != &vnode->wb_keys) {
		wbk = list_entry(p, struct afs_wb_key, vnode_link);
		_debug("wbk %u", key_serial(wbk->key));
		ret2 = key_validate(wbk->key);
<<<<<<< HEAD
		if (ret2 == 0)
			goto found_key;
=======
		if (ret2 == 0) {
			refcount_inc(&wbk->usage);
			_debug("USE WB KEY %u", key_serial(wbk->key));
			break;
		}

		wbk = NULL;
>>>>>>> upstream/android-13
		if (ret == -ENOKEY)
			ret = ret2;
		p = p->next;
	}

	spin_unlock(&vnode->wb_lock);
<<<<<<< HEAD
	afs_put_wb_key(wbk);
	_leave(" = %d [no keys]", ret);
	return ret;

found_key:
	refcount_inc(&wbk->usage);
	spin_unlock(&vnode->wb_lock);

	_debug("USE WB KEY %u", key_serial(wbk->key));

	ret = -ERESTARTSYS;
	if (afs_begin_vnode_operation(&fc, vnode, wbk->key)) {
		while (afs_select_fileserver(&fc)) {
			fc.cb_break = afs_calc_vnode_cb_break(vnode);
			afs_fs_store_data(&fc, mapping, first, last, offset, to);
		}

		afs_check_for_remote_deletion(&fc, fc.vnode);
		afs_vnode_commit_status(&fc, vnode, fc.cb_break);
		ret = afs_end_vnode_operation(&fc);
	}

	switch (ret) {
	case 0:
		afs_stat_v(vnode, n_stores);
		atomic_long_add((last * PAGE_SIZE + to) -
				(first * PAGE_SIZE + offset),
				&afs_v2net(vnode)->n_store_bytes);
		break;
=======
	if (*_wbk)
		afs_put_wb_key(*_wbk);
	*_wbk = wbk;
	return 0;
}

static void afs_store_data_success(struct afs_operation *op)
{
	struct afs_vnode *vnode = op->file[0].vnode;

	op->ctime = op->file[0].scb.status.mtime_client;
	afs_vnode_commit_status(op, &op->file[0]);
	if (op->error == 0) {
		if (!op->store.laundering)
			afs_pages_written_back(vnode, op->store.pos, op->store.size);
		afs_stat_v(vnode, n_stores);
		atomic_long_add(op->store.size, &afs_v2net(vnode)->n_store_bytes);
	}
}

static const struct afs_operation_ops afs_store_data_operation = {
	.issue_afs_rpc	= afs_fs_store_data,
	.issue_yfs_rpc	= yfs_fs_store_data,
	.success	= afs_store_data_success,
};

/*
 * write to a file
 */
static int afs_store_data(struct afs_vnode *vnode, struct iov_iter *iter, loff_t pos,
			  bool laundering)
{
	struct afs_operation *op;
	struct afs_wb_key *wbk = NULL;
	loff_t size = iov_iter_count(iter), i_size;
	int ret = -ENOKEY;

	_enter("%s{%llx:%llu.%u},%llx,%llx",
	       vnode->volume->name,
	       vnode->fid.vid,
	       vnode->fid.vnode,
	       vnode->fid.unique,
	       size, pos);

	ret = afs_get_writeback_key(vnode, &wbk);
	if (ret) {
		_leave(" = %d [no keys]", ret);
		return ret;
	}

	op = afs_alloc_operation(wbk->key, vnode->volume);
	if (IS_ERR(op)) {
		afs_put_wb_key(wbk);
		return -ENOMEM;
	}

	i_size = i_size_read(&vnode->vfs_inode);

	afs_op_set_vnode(op, 0, vnode);
	op->file[0].dv_delta = 1;
	op->file[0].modification = true;
	op->store.write_iter = iter;
	op->store.pos = pos;
	op->store.size = size;
	op->store.i_size = max(pos + size, i_size);
	op->store.laundering = laundering;
	op->mtime = vnode->vfs_inode.i_mtime;
	op->flags |= AFS_OPERATION_UNINTR;
	op->ops = &afs_store_data_operation;

try_next_key:
	afs_begin_vnode_operation(op);
	afs_wait_for_operation(op);

	switch (op->error) {
>>>>>>> upstream/android-13
	case -EACCES:
	case -EPERM:
	case -ENOKEY:
	case -EKEYEXPIRED:
	case -EKEYREJECTED:
	case -EKEYREVOKED:
		_debug("next");
<<<<<<< HEAD
		spin_lock(&vnode->wb_lock);
		p = wbk->vnode_link.next;
		afs_put_wb_key(wbk);
		goto try_next_key;
	}

	afs_put_wb_key(wbk);
	_leave(" = %d", ret);
	return ret;
=======

		ret = afs_get_writeback_key(vnode, &wbk);
		if (ret == 0) {
			key_put(op->key);
			op->key = key_get(wbk->key);
			goto try_next_key;
		}
		break;
	}

	afs_put_wb_key(wbk);
	_leave(" = %d", op->error);
	return afs_put_operation(op);
}

/*
 * Extend the region to be written back to include subsequent contiguously
 * dirty pages if possible, but don't sleep while doing so.
 *
 * If this page holds new content, then we can include filler zeros in the
 * writeback.
 */
static void afs_extend_writeback(struct address_space *mapping,
				 struct afs_vnode *vnode,
				 long *_count,
				 loff_t start,
				 loff_t max_len,
				 bool new_content,
				 unsigned int *_len)
{
	struct pagevec pvec;
	struct page *page;
	unsigned long priv;
	unsigned int psize, filler = 0;
	unsigned int f, t;
	loff_t len = *_len;
	pgoff_t index = (start + len) / PAGE_SIZE;
	bool stop = true;
	unsigned int i;

	XA_STATE(xas, &mapping->i_pages, index);
	pagevec_init(&pvec);

	do {
		/* Firstly, we gather up a batch of contiguous dirty pages
		 * under the RCU read lock - but we can't clear the dirty flags
		 * there if any of those pages are mapped.
		 */
		rcu_read_lock();

		xas_for_each(&xas, page, ULONG_MAX) {
			stop = true;
			if (xas_retry(&xas, page))
				continue;
			if (xa_is_value(page))
				break;
			if (page->index != index)
				break;

			if (!page_cache_get_speculative(page)) {
				xas_reset(&xas);
				continue;
			}

			/* Has the page moved or been split? */
			if (unlikely(page != xas_reload(&xas))) {
				put_page(page);
				break;
			}

			if (!trylock_page(page)) {
				put_page(page);
				break;
			}
			if (!PageDirty(page) || PageWriteback(page)) {
				unlock_page(page);
				put_page(page);
				break;
			}

			psize = thp_size(page);
			priv = page_private(page);
			f = afs_page_dirty_from(page, priv);
			t = afs_page_dirty_to(page, priv);
			if (f != 0 && !new_content) {
				unlock_page(page);
				put_page(page);
				break;
			}

			len += filler + t;
			filler = psize - t;
			if (len >= max_len || *_count <= 0)
				stop = true;
			else if (t == psize || new_content)
				stop = false;

			index += thp_nr_pages(page);
			if (!pagevec_add(&pvec, page))
				break;
			if (stop)
				break;
		}

		if (!stop)
			xas_pause(&xas);
		rcu_read_unlock();

		/* Now, if we obtained any pages, we can shift them to being
		 * writable and mark them for caching.
		 */
		if (!pagevec_count(&pvec))
			break;

		for (i = 0; i < pagevec_count(&pvec); i++) {
			page = pvec.pages[i];
			trace_afs_page_dirty(vnode, tracepoint_string("store+"), page);

			if (!clear_page_dirty_for_io(page))
				BUG();
			if (test_set_page_writeback(page))
				BUG();

			*_count -= thp_nr_pages(page);
			unlock_page(page);
		}

		pagevec_release(&pvec);
		cond_resched();
	} while (!stop);

	*_len = len;
>>>>>>> upstream/android-13
}

/*
 * Synchronously write back the locked page and any subsequent non-locked dirty
 * pages.
 */
<<<<<<< HEAD
static int afs_write_back_from_locked_page(struct address_space *mapping,
					   struct writeback_control *wbc,
					   struct page *primary_page,
					   pgoff_t final_page)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct page *pages[8], *page;
	unsigned long count, priv;
	unsigned n, offset, to, f, t;
	pgoff_t start, first, last;
	int loop, ret;

	_enter(",%lx", primary_page->index);

	count = 1;
	if (test_set_page_writeback(primary_page))
		BUG();

=======
static ssize_t afs_write_back_from_locked_page(struct address_space *mapping,
					       struct writeback_control *wbc,
					       struct page *page,
					       loff_t start, loff_t end)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct iov_iter iter;
	unsigned long priv;
	unsigned int offset, to, len, max_len;
	loff_t i_size = i_size_read(&vnode->vfs_inode);
	bool new_content = test_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags);
	long count = wbc->nr_to_write;
	int ret;

	_enter(",%lx,%llx-%llx", page->index, start, end);

	if (test_set_page_writeback(page))
		BUG();

	count -= thp_nr_pages(page);

>>>>>>> upstream/android-13
	/* Find all consecutive lockable dirty pages that have contiguous
	 * written regions, stopping when we find a page that is not
	 * immediately lockable, is not dirty or is missing, or we reach the
	 * end of the range.
	 */
<<<<<<< HEAD
	start = primary_page->index;
	priv = page_private(primary_page);
	offset = priv & AFS_PRIV_MAX;
	to = priv >> AFS_PRIV_SHIFT;
	trace_afs_page_dirty(vnode, tracepoint_string("store"),
			     primary_page->index, priv);

	WARN_ON(offset == to);
	if (offset == to)
		trace_afs_page_dirty(vnode, tracepoint_string("WARN"),
				     primary_page->index, priv);

	if (start >= final_page ||
	    (to < PAGE_SIZE && !test_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags)))
		goto no_more;

	start++;
	do {
		_debug("more %lx [%lx]", start, count);
		n = final_page - start + 1;
		if (n > ARRAY_SIZE(pages))
			n = ARRAY_SIZE(pages);
		n = find_get_pages_contig(mapping, start, ARRAY_SIZE(pages), pages);
		_debug("fgpc %u", n);
		if (n == 0)
			goto no_more;
		if (pages[0]->index != start) {
			do {
				put_page(pages[--n]);
			} while (n > 0);
			goto no_more;
		}

		for (loop = 0; loop < n; loop++) {
			page = pages[loop];
			if (to != PAGE_SIZE &&
			    !test_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags))
				break;
			if (page->index > final_page)
				break;
			if (!trylock_page(page))
				break;
			if (!PageDirty(page) || PageWriteback(page)) {
				unlock_page(page);
				break;
			}

			priv = page_private(page);
			f = priv & AFS_PRIV_MAX;
			t = priv >> AFS_PRIV_SHIFT;
			if (f != 0 &&
			    !test_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags)) {
				unlock_page(page);
				break;
			}
			to = t;

			trace_afs_page_dirty(vnode, tracepoint_string("store+"),
					     page->index, priv);

			if (!clear_page_dirty_for_io(page))
				BUG();
			if (test_set_page_writeback(page))
				BUG();
			unlock_page(page);
			put_page(page);
		}
		count += loop;
		if (loop < n) {
			for (; loop < n; loop++)
				put_page(pages[loop]);
			goto no_more;
		}

		start += loop;
	} while (start <= final_page && count < 65536);

no_more:
=======
	priv = page_private(page);
	offset = afs_page_dirty_from(page, priv);
	to = afs_page_dirty_to(page, priv);
	trace_afs_page_dirty(vnode, tracepoint_string("store"), page);

	len = to - offset;
	start += offset;
	if (start < i_size) {
		/* Trim the write to the EOF; the extra data is ignored.  Also
		 * put an upper limit on the size of a single storedata op.
		 */
		max_len = 65536 * 4096;
		max_len = min_t(unsigned long long, max_len, end - start + 1);
		max_len = min_t(unsigned long long, max_len, i_size - start);

		if (len < max_len &&
		    (to == thp_size(page) || new_content))
			afs_extend_writeback(mapping, vnode, &count,
					     start, max_len, new_content, &len);
		len = min_t(loff_t, len, max_len);
	}

>>>>>>> upstream/android-13
	/* We now have a contiguous set of dirty pages, each with writeback
	 * set; the first page is still locked at this point, but all the rest
	 * have been unlocked.
	 */
<<<<<<< HEAD
	unlock_page(primary_page);

	first = primary_page->index;
	last = first + count - 1;

	_debug("write back %lx[%u..] to %lx[..%u]", first, offset, last, to);

	ret = afs_store_data(mapping, first, last, offset, to);
	switch (ret) {
	case 0:
		ret = count;
=======
	unlock_page(page);

	if (start < i_size) {
		_debug("write back %x @%llx [%llx]", len, start, i_size);

		iov_iter_xarray(&iter, WRITE, &mapping->i_pages, start, len);
		ret = afs_store_data(vnode, &iter, start, false);
	} else {
		_debug("write discard %x @%llx [%llx]", len, start, i_size);

		/* The dirty region was entirely beyond the EOF. */
		afs_pages_written_back(vnode, start, len);
		ret = 0;
	}

	switch (ret) {
	case 0:
		wbc->nr_to_write = count;
		ret = len;
>>>>>>> upstream/android-13
		break;

	default:
		pr_notice("kAFS: Unexpected error from FS.StoreData %d\n", ret);
<<<<<<< HEAD
		/* Fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	case -EACCES:
	case -EPERM:
	case -ENOKEY:
	case -EKEYEXPIRED:
	case -EKEYREJECTED:
	case -EKEYREVOKED:
<<<<<<< HEAD
		afs_redirty_pages(wbc, mapping, first, last);
=======
		afs_redirty_pages(wbc, mapping, start, len);
>>>>>>> upstream/android-13
		mapping_set_error(mapping, ret);
		break;

	case -EDQUOT:
	case -ENOSPC:
<<<<<<< HEAD
		afs_redirty_pages(wbc, mapping, first, last);
=======
		afs_redirty_pages(wbc, mapping, start, len);
>>>>>>> upstream/android-13
		mapping_set_error(mapping, -ENOSPC);
		break;

	case -EROFS:
	case -EIO:
	case -EREMOTEIO:
	case -EFBIG:
	case -ENOENT:
	case -ENOMEDIUM:
	case -ENXIO:
<<<<<<< HEAD
		afs_kill_pages(mapping, first, last);
=======
		trace_afs_file_error(vnode, ret, afs_file_error_writeback_fail);
		afs_kill_pages(mapping, start, len);
>>>>>>> upstream/android-13
		mapping_set_error(mapping, ret);
		break;
	}

	_leave(" = %d", ret);
	return ret;
}

/*
 * write a page back to the server
 * - the caller locked the page for us
 */
int afs_writepage(struct page *page, struct writeback_control *wbc)
{
<<<<<<< HEAD
	int ret;

	_enter("{%lx},", page->index);

	ret = afs_write_back_from_locked_page(page->mapping, wbc, page,
					      wbc->range_end >> PAGE_SHIFT);
	if (ret < 0) {
		_leave(" = %d", ret);
		return 0;
	}

	wbc->nr_to_write -= ret;

=======
	ssize_t ret;
	loff_t start;

	_enter("{%lx},", page->index);

	start = page->index * PAGE_SIZE;
	ret = afs_write_back_from_locked_page(page->mapping, wbc, page,
					      start, LLONG_MAX - start);
	if (ret < 0) {
		_leave(" = %zd", ret);
		return ret;
	}

>>>>>>> upstream/android-13
	_leave(" = 0");
	return 0;
}

/*
 * write a region of pages back to the server
 */
static int afs_writepages_region(struct address_space *mapping,
				 struct writeback_control *wbc,
<<<<<<< HEAD
				 pgoff_t index, pgoff_t end, pgoff_t *_next)
{
	struct page *page;
	int ret, n;

	_enter(",,%lx,%lx,", index, end);

	do {
		n = find_get_pages_range_tag(mapping, &index, end,
					PAGECACHE_TAG_DIRTY, 1, &page);
		if (!n)
			break;

		_debug("wback %lx", page->index);

		/*
		 * at this point we hold neither the i_pages lock nor the
=======
				 loff_t start, loff_t end, loff_t *_next)
{
	struct page *page;
	ssize_t ret;
	int n;

	_enter("%llx,%llx,", start, end);

	do {
		pgoff_t index = start / PAGE_SIZE;

		n = find_get_pages_range_tag(mapping, &index, end / PAGE_SIZE,
					     PAGECACHE_TAG_DIRTY, 1, &page);
		if (!n)
			break;

		start = (loff_t)page->index * PAGE_SIZE; /* May regress with THPs */

		_debug("wback %lx", page->index);

		/* At this point we hold neither the i_pages lock nor the
>>>>>>> upstream/android-13
		 * page lock: the page may be truncated or invalidated
		 * (changing page->mapping to NULL), or even swizzled
		 * back from swapper_space to tmpfs file mapping
		 */
<<<<<<< HEAD
		ret = lock_page_killable(page);
		if (ret < 0) {
			put_page(page);
			_leave(" = %d", ret);
			return ret;
		}

		if (page->mapping != mapping || !PageDirty(page)) {
=======
		if (wbc->sync_mode != WB_SYNC_NONE) {
			ret = lock_page_killable(page);
			if (ret < 0) {
				put_page(page);
				return ret;
			}
		} else {
			if (!trylock_page(page)) {
				put_page(page);
				return 0;
			}
		}

		if (page->mapping != mapping || !PageDirty(page)) {
			start += thp_size(page);
>>>>>>> upstream/android-13
			unlock_page(page);
			put_page(page);
			continue;
		}

		if (PageWriteback(page)) {
			unlock_page(page);
			if (wbc->sync_mode != WB_SYNC_NONE)
				wait_on_page_writeback(page);
			put_page(page);
			continue;
		}

		if (!clear_page_dirty_for_io(page))
			BUG();
<<<<<<< HEAD
		ret = afs_write_back_from_locked_page(mapping, wbc, page, end);
		put_page(page);
		if (ret < 0) {
			_leave(" = %d", ret);
			return ret;
		}

		wbc->nr_to_write -= ret;

		cond_resched();
	} while (index < end && wbc->nr_to_write > 0);

	*_next = index;
	_leave(" = 0 [%lx]", *_next);
=======
		ret = afs_write_back_from_locked_page(mapping, wbc, page, start, end);
		put_page(page);
		if (ret < 0) {
			_leave(" = %zd", ret);
			return ret;
		}

		start += ret;

		cond_resched();
	} while (wbc->nr_to_write > 0);

	*_next = start;
	_leave(" = 0 [%llx]", *_next);
>>>>>>> upstream/android-13
	return 0;
}

/*
 * write some of the pending data back to the server
 */
int afs_writepages(struct address_space *mapping,
		   struct writeback_control *wbc)
{
<<<<<<< HEAD
	pgoff_t start, end, next;
=======
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	loff_t start, next;
>>>>>>> upstream/android-13
	int ret;

	_enter("");

<<<<<<< HEAD
	if (wbc->range_cyclic) {
		start = mapping->writeback_index;
		end = -1;
		ret = afs_writepages_region(mapping, wbc, start, end, &next);
		if (start > 0 && wbc->nr_to_write > 0 && ret == 0)
			ret = afs_writepages_region(mapping, wbc, 0, start,
						    &next);
		mapping->writeback_index = next;
	} else if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX) {
		end = (pgoff_t)(LLONG_MAX >> PAGE_SHIFT);
		ret = afs_writepages_region(mapping, wbc, 0, end, &next);
		if (wbc->nr_to_write > 0)
			mapping->writeback_index = next;
	} else {
		start = wbc->range_start >> PAGE_SHIFT;
		end = wbc->range_end >> PAGE_SHIFT;
		ret = afs_writepages_region(mapping, wbc, start, end, &next);
	}

=======
	/* We have to be careful as we can end up racing with setattr()
	 * truncating the pagecache since the caller doesn't take a lock here
	 * to prevent it.
	 */
	if (wbc->sync_mode == WB_SYNC_ALL)
		down_read(&vnode->validate_lock);
	else if (!down_read_trylock(&vnode->validate_lock))
		return 0;

	if (wbc->range_cyclic) {
		start = mapping->writeback_index * PAGE_SIZE;
		ret = afs_writepages_region(mapping, wbc, start, LLONG_MAX, &next);
		if (ret == 0) {
			mapping->writeback_index = next / PAGE_SIZE;
			if (start > 0 && wbc->nr_to_write > 0) {
				ret = afs_writepages_region(mapping, wbc, 0,
							    start, &next);
				if (ret == 0)
					mapping->writeback_index =
						next / PAGE_SIZE;
			}
		}
	} else if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX) {
		ret = afs_writepages_region(mapping, wbc, 0, LLONG_MAX, &next);
		if (wbc->nr_to_write > 0 && ret == 0)
			mapping->writeback_index = next / PAGE_SIZE;
	} else {
		ret = afs_writepages_region(mapping, wbc,
					    wbc->range_start, wbc->range_end, &next);
	}

	up_read(&vnode->validate_lock);
>>>>>>> upstream/android-13
	_leave(" = %d", ret);
	return ret;
}

/*
<<<<<<< HEAD
 * completion of write to server
 */
void afs_pages_written_back(struct afs_vnode *vnode, struct afs_call *call)
{
	struct pagevec pv;
	unsigned long priv;
	unsigned count, loop;
	pgoff_t first = call->first, last = call->last;

	_enter("{%x:%u},{%lx-%lx}",
	       vnode->fid.vid, vnode->fid.vnode, first, last);

	pagevec_init(&pv);

	do {
		_debug("done %lx-%lx", first, last);

		count = last - first + 1;
		if (count > PAGEVEC_SIZE)
			count = PAGEVEC_SIZE;
		pv.nr = find_get_pages_contig(vnode->vfs_inode.i_mapping,
					      first, count, pv.pages);
		ASSERTCMP(pv.nr, ==, count);

		for (loop = 0; loop < count; loop++) {
			priv = page_private(pv.pages[loop]);
			trace_afs_page_dirty(vnode, tracepoint_string("clear"),
					     pv.pages[loop]->index, priv);
			set_page_private(pv.pages[loop], 0);
			end_page_writeback(pv.pages[loop]);
		}
		first += count;
		__pagevec_release(&pv);
	} while (first <= last);

	afs_prune_wb_keys(vnode);
	_leave("");
}

/*
=======
>>>>>>> upstream/android-13
 * write to an AFS file
 */
ssize_t afs_file_write(struct kiocb *iocb, struct iov_iter *from)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(iocb->ki_filp));
<<<<<<< HEAD
	ssize_t result;
	size_t count = iov_iter_count(from);

	_enter("{%x.%u},{%zu},",
=======
	struct afs_file *af = iocb->ki_filp->private_data;
	ssize_t result;
	size_t count = iov_iter_count(from);

	_enter("{%llx:%llu},{%zu},",
>>>>>>> upstream/android-13
	       vnode->fid.vid, vnode->fid.vnode, count);

	if (IS_SWAPFILE(&vnode->vfs_inode)) {
		printk(KERN_INFO
		       "AFS: Attempt to write to active swap file!\n");
		return -EBUSY;
	}

	if (!count)
		return 0;

<<<<<<< HEAD
=======
	result = afs_validate(vnode, af->key);
	if (result < 0)
		return result;

>>>>>>> upstream/android-13
	result = generic_file_write_iter(iocb, from);

	_leave(" = %zd", result);
	return result;
}

/*
 * flush any dirty pages for this process, and check for write errors.
 * - the return status from this call provides a reliable indication of
 *   whether any write errors occurred for this process.
 */
int afs_fsync(struct file *file, loff_t start, loff_t end, int datasync)
{
<<<<<<< HEAD
	struct inode *inode = file_inode(file);
	struct afs_vnode *vnode = AFS_FS_I(inode);

	_enter("{%x:%u},{n=%pD},%d",
	       vnode->fid.vid, vnode->fid.vnode, file,
	       datasync);

=======
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));
	struct afs_file *af = file->private_data;
	int ret;

	_enter("{%llx:%llu},{n=%pD},%d",
	       vnode->fid.vid, vnode->fid.vnode, file,
	       datasync);

	ret = afs_validate(vnode, af->key);
	if (ret < 0)
		return ret;

>>>>>>> upstream/android-13
	return file_write_and_wait_range(file, start, end);
}

/*
 * notification that a previously read-only page is about to become writable
 * - if it returns an error, the caller will deliver a bus error signal
 */
vm_fault_t afs_page_mkwrite(struct vm_fault *vmf)
{
<<<<<<< HEAD
	struct file *file = vmf->vma->vm_file;
	struct inode *inode = file_inode(file);
	struct afs_vnode *vnode = AFS_FS_I(inode);
	unsigned long priv;

	_enter("{{%x:%u}},{%lx}",
	       vnode->fid.vid, vnode->fid.vnode, vmf->page->index);
=======
	struct page *page = thp_head(vmf->page);
	struct file *file = vmf->vma->vm_file;
	struct inode *inode = file_inode(file);
	struct afs_vnode *vnode = AFS_FS_I(inode);
	struct afs_file *af = file->private_data;
	unsigned long priv;
	vm_fault_t ret = VM_FAULT_RETRY;

	_enter("{{%llx:%llu}},{%lx}", vnode->fid.vid, vnode->fid.vnode, page->index);

	afs_validate(vnode, af->key);
>>>>>>> upstream/android-13

	sb_start_pagefault(inode->i_sb);

	/* Wait for the page to be written to the cache before we allow it to
	 * be modified.  We then assume the entire page will need writing back.
	 */
#ifdef CONFIG_AFS_FSCACHE
<<<<<<< HEAD
	fscache_wait_on_page_write(vnode->cache, vmf->page);
#endif

	if (PageWriteback(vmf->page) &&
	    wait_on_page_bit_killable(vmf->page, PG_writeback) < 0)
		return VM_FAULT_RETRY;

	if (lock_page_killable(vmf->page) < 0)
		return VM_FAULT_RETRY;
=======
	if (PageFsCache(page) &&
	    wait_on_page_fscache_killable(page) < 0)
		goto out;
#endif

	if (wait_on_page_writeback_killable(page))
		goto out;

	if (lock_page_killable(page) < 0)
		goto out;
>>>>>>> upstream/android-13

	/* We mustn't change page->private until writeback is complete as that
	 * details the portion of the page we need to write back and we might
	 * need to redirty the page if there's a problem.
	 */
<<<<<<< HEAD
	wait_on_page_writeback(vmf->page);

	priv = (unsigned long)PAGE_SIZE << AFS_PRIV_SHIFT; /* To */
	priv |= 0; /* From */
	trace_afs_page_dirty(vnode, tracepoint_string("mkwrite"),
			     vmf->page->index, priv);
	SetPagePrivate(vmf->page);
	set_page_private(vmf->page, priv);
	file_update_time(file);

	sb_end_pagefault(inode->i_sb);
	return VM_FAULT_LOCKED;
=======
	if (wait_on_page_writeback_killable(page) < 0) {
		unlock_page(page);
		goto out;
	}

	priv = afs_page_dirty(page, 0, thp_size(page));
	priv = afs_page_dirty_mmapped(priv);
	if (PagePrivate(page)) {
		set_page_private(page, priv);
		trace_afs_page_dirty(vnode, tracepoint_string("mkwrite+"), page);
	} else {
		attach_page_private(page, (void *)priv);
		trace_afs_page_dirty(vnode, tracepoint_string("mkwrite"), page);
	}
	file_update_time(file);

	ret = VM_FAULT_LOCKED;
out:
	sb_end_pagefault(inode->i_sb);
	return ret;
>>>>>>> upstream/android-13
}

/*
 * Prune the keys cached for writeback.  The caller must hold vnode->wb_lock.
 */
void afs_prune_wb_keys(struct afs_vnode *vnode)
{
	LIST_HEAD(graveyard);
	struct afs_wb_key *wbk, *tmp;

	/* Discard unused keys */
	spin_lock(&vnode->wb_lock);

	if (!mapping_tagged(&vnode->vfs_inode.i_data, PAGECACHE_TAG_WRITEBACK) &&
	    !mapping_tagged(&vnode->vfs_inode.i_data, PAGECACHE_TAG_DIRTY)) {
		list_for_each_entry_safe(wbk, tmp, &vnode->wb_keys, vnode_link) {
			if (refcount_read(&wbk->usage) == 1)
				list_move(&wbk->vnode_link, &graveyard);
		}
	}

	spin_unlock(&vnode->wb_lock);

	while (!list_empty(&graveyard)) {
		wbk = list_entry(graveyard.next, struct afs_wb_key, vnode_link);
		list_del(&wbk->vnode_link);
		afs_put_wb_key(wbk);
	}
}

/*
 * Clean up a page during invalidation.
 */
int afs_launder_page(struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
<<<<<<< HEAD
=======
	struct iov_iter iter;
	struct bio_vec bv[1];
>>>>>>> upstream/android-13
	unsigned long priv;
	unsigned int f, t;
	int ret = 0;

	_enter("{%lx}", page->index);

	priv = page_private(page);
	if (clear_page_dirty_for_io(page)) {
		f = 0;
<<<<<<< HEAD
		t = PAGE_SIZE;
		if (PagePrivate(page)) {
			f = priv & AFS_PRIV_MAX;
			t = priv >> AFS_PRIV_SHIFT;
		}

		trace_afs_page_dirty(vnode, tracepoint_string("launder"),
				     page->index, priv);
		ret = afs_store_data(mapping, page->index, page->index, t, f);
	}

	trace_afs_page_dirty(vnode, tracepoint_string("laundered"),
			     page->index, priv);
	set_page_private(page, 0);
	ClearPagePrivate(page);

#ifdef CONFIG_AFS_FSCACHE
	if (PageFsCache(page)) {
		fscache_wait_on_page_write(vnode->cache, page);
		fscache_uncache_page(vnode->cache, page);
	}
#endif
=======
		t = thp_size(page);
		if (PagePrivate(page)) {
			f = afs_page_dirty_from(page, priv);
			t = afs_page_dirty_to(page, priv);
		}

		bv[0].bv_page = page;
		bv[0].bv_offset = f;
		bv[0].bv_len = t - f;
		iov_iter_bvec(&iter, WRITE, bv, 1, bv[0].bv_len);

		trace_afs_page_dirty(vnode, tracepoint_string("launder"), page);
		ret = afs_store_data(vnode, &iter, page_offset(page) + f, true);
	}

	trace_afs_page_dirty(vnode, tracepoint_string("laundered"), page);
	detach_page_private(page);
	wait_on_page_fscache(page);
>>>>>>> upstream/android-13
	return ret;
}
