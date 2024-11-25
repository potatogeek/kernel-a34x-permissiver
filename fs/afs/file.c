<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* AFS filesystem file handling
 *
 * Copyright (C) 2002, 2007 Red Hat, Inc. All Rights Reserved.
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/writeback.h>
#include <linux/gfp.h>
#include <linux/task_io_accounting_ops.h>
<<<<<<< HEAD
=======
#include <linux/mm.h>
#include <linux/netfs.h>
>>>>>>> upstream/android-13
#include "internal.h"

static int afs_file_mmap(struct file *file, struct vm_area_struct *vma);
static int afs_readpage(struct file *file, struct page *page);
static void afs_invalidatepage(struct page *page, unsigned int offset,
			       unsigned int length);
static int afs_releasepage(struct page *page, gfp_t gfp_flags);

<<<<<<< HEAD
static int afs_readpages(struct file *filp, struct address_space *mapping,
			 struct list_head *pages, unsigned nr_pages);
=======
static void afs_readahead(struct readahead_control *ractl);
static ssize_t afs_file_read_iter(struct kiocb *iocb, struct iov_iter *iter);
static void afs_vm_open(struct vm_area_struct *area);
static void afs_vm_close(struct vm_area_struct *area);
static vm_fault_t afs_vm_map_pages(struct vm_fault *vmf, pgoff_t start_pgoff, pgoff_t end_pgoff);
>>>>>>> upstream/android-13

const struct file_operations afs_file_operations = {
	.open		= afs_open,
	.release	= afs_release,
	.llseek		= generic_file_llseek,
<<<<<<< HEAD
	.read_iter	= generic_file_read_iter,
	.write_iter	= afs_file_write,
	.mmap		= afs_file_mmap,
	.splice_read	= generic_file_splice_read,
=======
	.read_iter	= afs_file_read_iter,
	.write_iter	= afs_file_write,
	.mmap		= afs_file_mmap,
	.splice_read	= generic_file_splice_read,
	.splice_write	= iter_file_splice_write,
>>>>>>> upstream/android-13
	.fsync		= afs_fsync,
	.lock		= afs_lock,
	.flock		= afs_flock,
};

const struct inode_operations afs_file_inode_operations = {
	.getattr	= afs_getattr,
	.setattr	= afs_setattr,
	.permission	= afs_permission,
<<<<<<< HEAD
	.listxattr	= afs_listxattr,
=======
>>>>>>> upstream/android-13
};

const struct address_space_operations afs_fs_aops = {
	.readpage	= afs_readpage,
<<<<<<< HEAD
	.readpages	= afs_readpages,
=======
	.readahead	= afs_readahead,
>>>>>>> upstream/android-13
	.set_page_dirty	= afs_set_page_dirty,
	.launder_page	= afs_launder_page,
	.releasepage	= afs_releasepage,
	.invalidatepage	= afs_invalidatepage,
	.write_begin	= afs_write_begin,
	.write_end	= afs_write_end,
	.writepage	= afs_writepage,
	.writepages	= afs_writepages,
};

static const struct vm_operations_struct afs_vm_ops = {
<<<<<<< HEAD
	.fault		= filemap_fault,
	.map_pages	= filemap_map_pages,
=======
	.open		= afs_vm_open,
	.close		= afs_vm_close,
	.fault		= filemap_fault,
	.map_pages	= afs_vm_map_pages,
>>>>>>> upstream/android-13
	.page_mkwrite	= afs_page_mkwrite,
};

/*
 * Discard a pin on a writeback key.
 */
void afs_put_wb_key(struct afs_wb_key *wbk)
{
<<<<<<< HEAD
	if (refcount_dec_and_test(&wbk->usage)) {
=======
	if (wbk && refcount_dec_and_test(&wbk->usage)) {
>>>>>>> upstream/android-13
		key_put(wbk->key);
		kfree(wbk);
	}
}

/*
 * Cache key for writeback.
 */
int afs_cache_wb_key(struct afs_vnode *vnode, struct afs_file *af)
{
	struct afs_wb_key *wbk, *p;

	wbk = kzalloc(sizeof(struct afs_wb_key), GFP_KERNEL);
	if (!wbk)
		return -ENOMEM;
	refcount_set(&wbk->usage, 2);
	wbk->key = af->key;

	spin_lock(&vnode->wb_lock);
	list_for_each_entry(p, &vnode->wb_keys, vnode_link) {
		if (p->key == wbk->key)
			goto found;
	}

	key_get(wbk->key);
	list_add_tail(&wbk->vnode_link, &vnode->wb_keys);
	spin_unlock(&vnode->wb_lock);
	af->wb = wbk;
	return 0;

found:
	refcount_inc(&p->usage);
	spin_unlock(&vnode->wb_lock);
	af->wb = p;
	kfree(wbk);
	return 0;
}

/*
 * open an AFS file or directory and attach a key to it
 */
int afs_open(struct inode *inode, struct file *file)
{
	struct afs_vnode *vnode = AFS_FS_I(inode);
	struct afs_file *af;
	struct key *key;
	int ret;

<<<<<<< HEAD
	_enter("{%x:%u},", vnode->fid.vid, vnode->fid.vnode);
=======
	_enter("{%llx:%llu},", vnode->fid.vid, vnode->fid.vnode);
>>>>>>> upstream/android-13

	key = afs_request_key(vnode->volume->cell);
	if (IS_ERR(key)) {
		ret = PTR_ERR(key);
		goto error;
	}

	af = kzalloc(sizeof(*af), GFP_KERNEL);
	if (!af) {
		ret = -ENOMEM;
		goto error_key;
	}
	af->key = key;

	ret = afs_validate(vnode, key);
	if (ret < 0)
		goto error_af;

	if (file->f_mode & FMODE_WRITE) {
		ret = afs_cache_wb_key(vnode, af);
		if (ret < 0)
			goto error_af;
	}

	if (file->f_flags & O_TRUNC)
		set_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags);
	
	file->private_data = af;
	_leave(" = 0");
	return 0;

error_af:
	kfree(af);
error_key:
	key_put(key);
error:
	_leave(" = %d", ret);
	return ret;
}

/*
 * release an AFS file or directory and discard its key
 */
int afs_release(struct inode *inode, struct file *file)
{
	struct afs_vnode *vnode = AFS_FS_I(inode);
	struct afs_file *af = file->private_data;
	int ret = 0;

<<<<<<< HEAD
	_enter("{%x:%u},", vnode->fid.vid, vnode->fid.vnode);
=======
	_enter("{%llx:%llu},", vnode->fid.vid, vnode->fid.vnode);
>>>>>>> upstream/android-13

	if ((file->f_mode & FMODE_WRITE))
		ret = vfs_fsync(file, 0);

	file->private_data = NULL;
	if (af->wb)
		afs_put_wb_key(af->wb);
	key_put(af->key);
	kfree(af);
	afs_prune_wb_keys(vnode);
	_leave(" = %d", ret);
	return ret;
}

/*
<<<<<<< HEAD
=======
 * Allocate a new read record.
 */
struct afs_read *afs_alloc_read(gfp_t gfp)
{
	struct afs_read *req;

	req = kzalloc(sizeof(struct afs_read), gfp);
	if (req)
		refcount_set(&req->usage, 1);

	return req;
}

/*
>>>>>>> upstream/android-13
 * Dispose of a ref to a read record.
 */
void afs_put_read(struct afs_read *req)
{
<<<<<<< HEAD
	int i;

	if (refcount_dec_and_test(&req->usage)) {
		if (req->pages) {
			for (i = 0; i < req->nr_pages; i++)
				if (req->pages[i])
					put_page(req->pages[i]);
			if (req->pages != req->array)
				kfree(req->pages);
		}
=======
	if (refcount_dec_and_test(&req->usage)) {
		if (req->cleanup)
			req->cleanup(req);
		key_put(req->key);
>>>>>>> upstream/android-13
		kfree(req);
	}
}

<<<<<<< HEAD
#ifdef CONFIG_AFS_FSCACHE
/*
 * deal with notification that a page was read from the cache
 */
static void afs_file_readpage_read_complete(struct page *page,
					    void *data,
					    int error)
{
	_enter("%p,%p,%d", page, data, error);

	/* if the read completes with an error, we just unlock the page and let
	 * the VM reissue the readpage */
	if (!error)
		SetPageUptodate(page);
	unlock_page(page);
}
#endif
=======
static void afs_fetch_data_notify(struct afs_operation *op)
{
	struct afs_read *req = op->fetch.req;
	struct netfs_read_subrequest *subreq = req->subreq;
	int error = op->error;

	if (error == -ECONNABORTED)
		error = afs_abort_to_error(op->ac.abort_code);
	req->error = error;

	if (subreq) {
		__set_bit(NETFS_SREQ_CLEAR_TAIL, &subreq->flags);
		netfs_subreq_terminated(subreq, error ?: req->actual_len, false);
		req->subreq = NULL;
	} else if (req->done) {
		req->done(req);
	}
}

static void afs_fetch_data_success(struct afs_operation *op)
{
	struct afs_vnode *vnode = op->file[0].vnode;

	_enter("op=%08x", op->debug_id);
	afs_vnode_commit_status(op, &op->file[0]);
	afs_stat_v(vnode, n_fetches);
	atomic_long_add(op->fetch.req->actual_len, &op->net->n_fetch_bytes);
	afs_fetch_data_notify(op);
}

static void afs_fetch_data_put(struct afs_operation *op)
{
	op->fetch.req->error = op->error;
	afs_put_read(op->fetch.req);
}

static const struct afs_operation_ops afs_fetch_data_operation = {
	.issue_afs_rpc	= afs_fs_fetch_data,
	.issue_yfs_rpc	= yfs_fs_fetch_data,
	.success	= afs_fetch_data_success,
	.aborted	= afs_check_for_remote_deletion,
	.failed		= afs_fetch_data_notify,
	.put		= afs_fetch_data_put,
};
>>>>>>> upstream/android-13

/*
 * Fetch file data from the volume.
 */
<<<<<<< HEAD
int afs_fetch_data(struct afs_vnode *vnode, struct key *key, struct afs_read *desc)
{
	struct afs_fs_cursor fc;
	int ret;

	_enter("%s{%x:%u.%u},%x,,,",
=======
int afs_fetch_data(struct afs_vnode *vnode, struct afs_read *req)
{
	struct afs_operation *op;

	_enter("%s{%llx:%llu.%u},%x,,,",
>>>>>>> upstream/android-13
	       vnode->volume->name,
	       vnode->fid.vid,
	       vnode->fid.vnode,
	       vnode->fid.unique,
<<<<<<< HEAD
	       key_serial(key));

	ret = -ERESTARTSYS;
	if (afs_begin_vnode_operation(&fc, vnode, key)) {
		while (afs_select_fileserver(&fc)) {
			fc.cb_break = afs_calc_vnode_cb_break(vnode);
			afs_fs_fetch_data(&fc, desc);
		}

		afs_check_for_remote_deletion(&fc, fc.vnode);
		afs_vnode_commit_status(&fc, vnode, fc.cb_break);
		ret = afs_end_vnode_operation(&fc);
	}

	if (ret == 0) {
		afs_stat_v(vnode, n_fetches);
		atomic_long_add(desc->actual_len,
				&afs_v2net(vnode)->n_fetch_bytes);
	}

	_leave(" = %d", ret);
	return ret;
}

/*
 * read page from file, directory or symlink, given a key to use
 */
int afs_page_filler(void *data, struct page *page)
{
	struct inode *inode = page->mapping->host;
	struct afs_vnode *vnode = AFS_FS_I(inode);
	struct afs_read *req;
	struct key *key = data;
	int ret;

	_enter("{%x},{%lu},{%lu}", key_serial(key), inode->i_ino, page->index);

	BUG_ON(!PageLocked(page));

	ret = -ESTALE;
	if (test_bit(AFS_VNODE_DELETED, &vnode->flags))
		goto error;

	/* is it cached? */
#ifdef CONFIG_AFS_FSCACHE
	ret = fscache_read_or_alloc_page(vnode->cache,
					 page,
					 afs_file_readpage_read_complete,
					 NULL,
					 GFP_KERNEL);
#else
	ret = -ENOBUFS;
#endif
	switch (ret) {
		/* read BIO submitted (page in cache) */
	case 0:
		break;

		/* page not yet cached */
	case -ENODATA:
		_debug("cache said ENODATA");
		goto go_on;

		/* page will not be cached */
	case -ENOBUFS:
		_debug("cache said ENOBUFS");
	default:
	go_on:
		req = kzalloc(sizeof(struct afs_read) + sizeof(struct page *),
			      GFP_KERNEL);
		if (!req)
			goto enomem;

		/* We request a full page.  If the page is a partial one at the
		 * end of the file, the server will return a short read and the
		 * unmarshalling code will clear the unfilled space.
		 */
		refcount_set(&req->usage, 1);
		req->pos = (loff_t)page->index << PAGE_SHIFT;
		req->len = PAGE_SIZE;
		req->nr_pages = 1;
		req->pages = req->array;
		req->pages[0] = page;
		get_page(page);

		/* read the contents of the file from the server into the
		 * page */
		ret = afs_fetch_data(vnode, key, req);
		afs_put_read(req);

		if (ret < 0) {
			if (ret == -ENOENT) {
				_debug("got NOENT from server"
				       " - marking file deleted and stale");
				set_bit(AFS_VNODE_DELETED, &vnode->flags);
				ret = -ESTALE;
			}

#ifdef CONFIG_AFS_FSCACHE
			fscache_uncache_page(vnode->cache, page);
#endif
			BUG_ON(PageFsCache(page));

			if (ret == -EINTR ||
			    ret == -ENOMEM ||
			    ret == -ERESTARTSYS ||
			    ret == -EAGAIN)
				goto error;
			goto io_error;
		}

		SetPageUptodate(page);

		/* send the page to the cache */
#ifdef CONFIG_AFS_FSCACHE
		if (PageFsCache(page) &&
		    fscache_write_page(vnode->cache, page, vnode->status.size,
				       GFP_KERNEL) != 0) {
			fscache_uncache_page(vnode->cache, page);
			BUG_ON(PageFsCache(page));
		}
#endif
		unlock_page(page);
	}

	_leave(" = 0");
	return 0;

io_error:
	SetPageError(page);
	goto error;
enomem:
	ret = -ENOMEM;
error:
	unlock_page(page);
	_leave(" = %d", ret);
	return ret;
}

/*
 * read page from file, directory or symlink, given a file to nominate the key
 * to be used
 */
static int afs_readpage(struct file *file, struct page *page)
{
	struct key *key;
	int ret;

	if (file) {
		key = afs_file_key(file);
		ASSERT(key != NULL);
		ret = afs_page_filler(key, page);
	} else {
		struct inode *inode = page->mapping->host;
		key = afs_request_key(AFS_FS_S(inode->i_sb)->cell);
		if (IS_ERR(key)) {
			ret = PTR_ERR(key);
		} else {
			ret = afs_page_filler(key, page);
			key_put(key);
		}
	}
	return ret;
}

/*
 * Make pages available as they're filled.
 */
static void afs_readpages_page_done(struct afs_call *call, struct afs_read *req)
{
#ifdef CONFIG_AFS_FSCACHE
	struct afs_vnode *vnode = call->reply[0];
#endif
	struct page *page = req->pages[req->index];

	req->pages[req->index] = NULL;
	SetPageUptodate(page);

	/* send the page to the cache */
#ifdef CONFIG_AFS_FSCACHE
	if (PageFsCache(page) &&
	    fscache_write_page(vnode->cache, page, vnode->status.size,
			       GFP_KERNEL) != 0) {
		fscache_uncache_page(vnode->cache, page);
		BUG_ON(PageFsCache(page));
	}
#endif
	unlock_page(page);
	put_page(page);
}

/*
 * Read a contiguous set of pages.
 */
static int afs_readpages_one(struct file *file, struct address_space *mapping,
			     struct list_head *pages)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	struct afs_read *req;
	struct list_head *p;
	struct page *first, *page;
	struct key *key = afs_file_key(file);
	pgoff_t index;
	int ret, n, i;

	/* Count the number of contiguous pages at the front of the list.  Note
	 * that the list goes prev-wards rather than next-wards.
	 */
	first = list_entry(pages->prev, struct page, lru);
	index = first->index + 1;
	n = 1;
	for (p = first->lru.prev; p != pages; p = p->prev) {
		page = list_entry(p, struct page, lru);
		if (page->index != index)
			break;
		index++;
		n++;
	}

	req = kzalloc(sizeof(struct afs_read) + sizeof(struct page *) * n,
		      GFP_NOFS);
	if (!req)
		return -ENOMEM;

	refcount_set(&req->usage, 1);
	req->page_done = afs_readpages_page_done;
	req->pos = first->index;
	req->pos <<= PAGE_SHIFT;
	req->pages = req->array;

	/* Transfer the pages to the request.  We add them in until one fails
	 * to add to the LRU and then we stop (as that'll make a hole in the
	 * contiguous run.
	 *
	 * Note that it's possible for the file size to change whilst we're
	 * doing this, but we rely on the server returning less than we asked
	 * for if the file shrank.  We also rely on this to deal with a partial
	 * page at the end of the file.
	 */
	do {
		page = list_entry(pages->prev, struct page, lru);
		list_del(&page->lru);
		index = page->index;
		if (add_to_page_cache_lru(page, mapping, index,
					  readahead_gfp_mask(mapping))) {
#ifdef CONFIG_AFS_FSCACHE
			fscache_uncache_page(vnode->cache, page);
#endif
			put_page(page);
			break;
		}

		req->pages[req->nr_pages++] = page;
		req->len += PAGE_SIZE;
	} while (req->nr_pages < n);

	if (req->nr_pages == 0) {
		kfree(req);
		return 0;
	}

	ret = afs_fetch_data(vnode, key, req);
	if (ret < 0)
		goto error;

	task_io_account_read(PAGE_SIZE * req->nr_pages);
	afs_put_read(req);
	return 0;

error:
	if (ret == -ENOENT) {
		_debug("got NOENT from server"
		       " - marking file deleted and stale");
		set_bit(AFS_VNODE_DELETED, &vnode->flags);
		ret = -ESTALE;
	}

	for (i = 0; i < req->nr_pages; i++) {
		page = req->pages[i];
		if (page) {
#ifdef CONFIG_AFS_FSCACHE
			fscache_uncache_page(vnode->cache, page);
#endif
			SetPageError(page);
			unlock_page(page);
		}
	}

	afs_put_read(req);
	return ret;
}

/*
 * read a set of pages
 */
static int afs_readpages(struct file *file, struct address_space *mapping,
			 struct list_head *pages, unsigned nr_pages)
{
	struct key *key = afs_file_key(file);
	struct afs_vnode *vnode;
	int ret = 0;

	_enter("{%d},{%lu},,%d",
	       key_serial(key), mapping->host->i_ino, nr_pages);

	ASSERT(key != NULL);

	vnode = AFS_FS_I(mapping->host);
	if (test_bit(AFS_VNODE_DELETED, &vnode->flags)) {
		_leave(" = -ESTALE");
		return -ESTALE;
	}

	/* attempt to read as many of the pages as possible */
#ifdef CONFIG_AFS_FSCACHE
	ret = fscache_read_or_alloc_pages(vnode->cache,
					  mapping,
					  pages,
					  &nr_pages,
					  afs_file_readpage_read_complete,
					  NULL,
					  mapping_gfp_mask(mapping));
#else
	ret = -ENOBUFS;
#endif

	switch (ret) {
		/* all pages are being read from the cache */
	case 0:
		BUG_ON(!list_empty(pages));
		BUG_ON(nr_pages != 0);
		_leave(" = 0 [reading all]");
		return 0;

		/* there were pages that couldn't be read from the cache */
	case -ENODATA:
	case -ENOBUFS:
		break;

		/* other error */
	default:
		_leave(" = %d", ret);
		return ret;
	}

	while (!list_empty(pages)) {
		ret = afs_readpages_one(file, mapping, pages);
		if (ret < 0)
			break;
	}

	_leave(" = %d [netting]", ret);
	return ret;
=======
	       key_serial(req->key));

	op = afs_alloc_operation(req->key, vnode->volume);
	if (IS_ERR(op)) {
		if (req->subreq)
			netfs_subreq_terminated(req->subreq, PTR_ERR(op), false);
		return PTR_ERR(op);
	}

	afs_op_set_vnode(op, 0, vnode);

	op->fetch.req	= afs_get_read(req);
	op->ops		= &afs_fetch_data_operation;
	return afs_do_sync_operation(op);
}

static void afs_req_issue_op(struct netfs_read_subrequest *subreq)
{
	struct afs_vnode *vnode = AFS_FS_I(subreq->rreq->inode);
	struct afs_read *fsreq;

	fsreq = afs_alloc_read(GFP_NOFS);
	if (!fsreq)
		return netfs_subreq_terminated(subreq, -ENOMEM, false);

	fsreq->subreq	= subreq;
	fsreq->pos	= subreq->start + subreq->transferred;
	fsreq->len	= subreq->len   - subreq->transferred;
	fsreq->key	= key_get(subreq->rreq->netfs_priv);
	fsreq->vnode	= vnode;
	fsreq->iter	= &fsreq->def_iter;

	iov_iter_xarray(&fsreq->def_iter, READ,
			&fsreq->vnode->vfs_inode.i_mapping->i_pages,
			fsreq->pos, fsreq->len);

	afs_fetch_data(fsreq->vnode, fsreq);
	afs_put_read(fsreq);
}

static int afs_symlink_readpage(struct page *page)
{
	struct afs_vnode *vnode = AFS_FS_I(page->mapping->host);
	struct afs_read *fsreq;
	int ret;

	fsreq = afs_alloc_read(GFP_NOFS);
	if (!fsreq)
		return -ENOMEM;

	fsreq->pos	= page->index * PAGE_SIZE;
	fsreq->len	= PAGE_SIZE;
	fsreq->vnode	= vnode;
	fsreq->iter	= &fsreq->def_iter;
	iov_iter_xarray(&fsreq->def_iter, READ, &page->mapping->i_pages,
			fsreq->pos, fsreq->len);

	ret = afs_fetch_data(fsreq->vnode, fsreq);
	page_endio(page, false, ret);
	return ret;
}

static void afs_init_rreq(struct netfs_read_request *rreq, struct file *file)
{
	rreq->netfs_priv = key_get(afs_file_key(file));
}

static bool afs_is_cache_enabled(struct inode *inode)
{
	struct fscache_cookie *cookie = afs_vnode_cache(AFS_FS_I(inode));

	return fscache_cookie_enabled(cookie) && !hlist_empty(&cookie->backing_objects);
}

static int afs_begin_cache_operation(struct netfs_read_request *rreq)
{
	struct afs_vnode *vnode = AFS_FS_I(rreq->inode);

	return fscache_begin_read_operation(rreq, afs_vnode_cache(vnode));
}

static int afs_check_write_begin(struct file *file, loff_t pos, unsigned len,
				 struct page *page, void **_fsdata)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));

	return test_bit(AFS_VNODE_DELETED, &vnode->flags) ? -ESTALE : 0;
}

static void afs_priv_cleanup(struct address_space *mapping, void *netfs_priv)
{
	key_put(netfs_priv);
}

const struct netfs_read_request_ops afs_req_ops = {
	.init_rreq		= afs_init_rreq,
	.is_cache_enabled	= afs_is_cache_enabled,
	.begin_cache_operation	= afs_begin_cache_operation,
	.check_write_begin	= afs_check_write_begin,
	.issue_op		= afs_req_issue_op,
	.cleanup		= afs_priv_cleanup,
};

static int afs_readpage(struct file *file, struct page *page)
{
	if (!file)
		return afs_symlink_readpage(page);

	return netfs_readpage(file, page, &afs_req_ops, NULL);
}

static void afs_readahead(struct readahead_control *ractl)
{
	netfs_readahead(ractl, &afs_req_ops, NULL);
}

/*
 * Adjust the dirty region of the page on truncation or full invalidation,
 * getting rid of the markers altogether if the region is entirely invalidated.
 */
static void afs_invalidate_dirty(struct page *page, unsigned int offset,
				 unsigned int length)
{
	struct afs_vnode *vnode = AFS_FS_I(page->mapping->host);
	unsigned long priv;
	unsigned int f, t, end = offset + length;

	priv = page_private(page);

	/* we clean up only if the entire page is being invalidated */
	if (offset == 0 && length == thp_size(page))
		goto full_invalidate;

	 /* If the page was dirtied by page_mkwrite(), the PTE stays writable
	  * and we don't get another notification to tell us to expand it
	  * again.
	  */
	if (afs_is_page_dirty_mmapped(priv))
		return;

	/* We may need to shorten the dirty region */
	f = afs_page_dirty_from(page, priv);
	t = afs_page_dirty_to(page, priv);

	if (t <= offset || f >= end)
		return; /* Doesn't overlap */

	if (f < offset && t > end)
		return; /* Splits the dirty region - just absorb it */

	if (f >= offset && t <= end)
		goto undirty;

	if (f < offset)
		t = offset;
	else
		f = end;
	if (f == t)
		goto undirty;

	priv = afs_page_dirty(page, f, t);
	set_page_private(page, priv);
	trace_afs_page_dirty(vnode, tracepoint_string("trunc"), page);
	return;

undirty:
	trace_afs_page_dirty(vnode, tracepoint_string("undirty"), page);
	clear_page_dirty_for_io(page);
full_invalidate:
	trace_afs_page_dirty(vnode, tracepoint_string("inval"), page);
	detach_page_private(page);
>>>>>>> upstream/android-13
}

/*
 * invalidate part or all of a page
 * - release a page and clean up its private data if offset is 0 (indicating
 *   the entire page)
 */
static void afs_invalidatepage(struct page *page, unsigned int offset,
			       unsigned int length)
{
<<<<<<< HEAD
	struct afs_vnode *vnode = AFS_FS_I(page->mapping->host);
	unsigned long priv;

=======
>>>>>>> upstream/android-13
	_enter("{%lu},%u,%u", page->index, offset, length);

	BUG_ON(!PageLocked(page));

<<<<<<< HEAD
	/* we clean up only if the entire page is being invalidated */
	if (offset == 0 && length == PAGE_SIZE) {
#ifdef CONFIG_AFS_FSCACHE
		if (PageFsCache(page)) {
			struct afs_vnode *vnode = AFS_FS_I(page->mapping->host);
			fscache_wait_on_page_write(vnode->cache, page);
			fscache_uncache_page(vnode->cache, page);
		}
#endif

		if (PagePrivate(page)) {
			priv = page_private(page);
			trace_afs_page_dirty(vnode, tracepoint_string("inval"),
					     page->index, priv);
			set_page_private(page, 0);
			ClearPagePrivate(page);
		}
	}

=======
	if (PagePrivate(page))
		afs_invalidate_dirty(page, offset, length);

	wait_on_page_fscache(page);
>>>>>>> upstream/android-13
	_leave("");
}

/*
 * release a page and clean up its private state if it's not busy
 * - return true if the page can now be released, false if not
 */
static int afs_releasepage(struct page *page, gfp_t gfp_flags)
{
	struct afs_vnode *vnode = AFS_FS_I(page->mapping->host);
<<<<<<< HEAD
	unsigned long priv;

	_enter("{{%x:%u}[%lu],%lx},%x",
=======

	_enter("{{%llx:%llu}[%lu],%lx},%x",
>>>>>>> upstream/android-13
	       vnode->fid.vid, vnode->fid.vnode, page->index, page->flags,
	       gfp_flags);

	/* deny if page is being written to the cache and the caller hasn't
	 * elected to wait */
#ifdef CONFIG_AFS_FSCACHE
<<<<<<< HEAD
	if (!fscache_maybe_release_page(vnode->cache, page, gfp_flags)) {
		_leave(" = F [cache busy]");
		return 0;
=======
	if (PageFsCache(page)) {
		if (!(gfp_flags & __GFP_DIRECT_RECLAIM) || !(gfp_flags & __GFP_FS))
			return false;
		wait_on_page_fscache(page);
>>>>>>> upstream/android-13
	}
#endif

	if (PagePrivate(page)) {
<<<<<<< HEAD
		priv = page_private(page);
		trace_afs_page_dirty(vnode, tracepoint_string("rel"),
				     page->index, priv);
		set_page_private(page, 0);
		ClearPagePrivate(page);
=======
		trace_afs_page_dirty(vnode, tracepoint_string("rel"), page);
		detach_page_private(page);
>>>>>>> upstream/android-13
	}

	/* indicate that the page can be released */
	_leave(" = T");
	return 1;
}

<<<<<<< HEAD
=======
static void afs_add_open_mmap(struct afs_vnode *vnode)
{
	if (atomic_inc_return(&vnode->cb_nr_mmap) == 1) {
		down_write(&vnode->volume->cell->fs_open_mmaps_lock);

		if (list_empty(&vnode->cb_mmap_link))
			list_add_tail(&vnode->cb_mmap_link,
				      &vnode->volume->cell->fs_open_mmaps);

		up_write(&vnode->volume->cell->fs_open_mmaps_lock);
	}
}

static void afs_drop_open_mmap(struct afs_vnode *vnode)
{
	if (!atomic_dec_and_test(&vnode->cb_nr_mmap))
		return;

	down_write(&vnode->volume->cell->fs_open_mmaps_lock);

	if (atomic_read(&vnode->cb_nr_mmap) == 0)
		list_del_init(&vnode->cb_mmap_link);

	up_write(&vnode->volume->cell->fs_open_mmaps_lock);
	flush_work(&vnode->cb_work);
}

>>>>>>> upstream/android-13
/*
 * Handle setting up a memory mapping on an AFS file.
 */
static int afs_file_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	int ret;

	ret = generic_file_mmap(file, vma);
	if (ret == 0)
		vma->vm_ops = &afs_vm_ops;
	return ret;
}
=======
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));
	int ret;

	afs_add_open_mmap(vnode);

	ret = generic_file_mmap(file, vma);
	if (ret == 0)
		vma->vm_ops = &afs_vm_ops;
	else
		afs_drop_open_mmap(vnode);
	return ret;
}

static void afs_vm_open(struct vm_area_struct *vma)
{
	afs_add_open_mmap(AFS_FS_I(file_inode(vma->vm_file)));
}

static void afs_vm_close(struct vm_area_struct *vma)
{
	afs_drop_open_mmap(AFS_FS_I(file_inode(vma->vm_file)));
}

static vm_fault_t afs_vm_map_pages(struct vm_fault *vmf, pgoff_t start_pgoff, pgoff_t end_pgoff)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(vmf->vma->vm_file));
	struct afs_file *af = vmf->vma->vm_file->private_data;

	switch (afs_validate(vnode, af->key)) {
	case 0:
		return filemap_map_pages(vmf, start_pgoff, end_pgoff);
	case -ENOMEM:
		return VM_FAULT_OOM;
	case -EINTR:
	case -ERESTARTSYS:
		return VM_FAULT_RETRY;
	case -ESTALE:
	default:
		return VM_FAULT_SIGBUS;
	}
}

static ssize_t afs_file_read_iter(struct kiocb *iocb, struct iov_iter *iter)
{
	struct afs_vnode *vnode = AFS_FS_I(file_inode(iocb->ki_filp));
	struct afs_file *af = iocb->ki_filp->private_data;
	int ret;

	ret = afs_validate(vnode, af->key);
	if (ret < 0)
		return ret;

	return generic_file_read_iter(iocb, iter);
}
>>>>>>> upstream/android-13
