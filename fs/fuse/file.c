/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2008  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.
*/

#include "fuse_i.h"

#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/compat.h>
=======
>>>>>>> upstream/android-13
#include <linux/swap.h>
#include <linux/falloc.h>
#include <linux/uio.h>
#include <linux/sched/mm.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <mt-plat/mtk_blocktag.h>

static const struct file_operations fuse_direct_io_file_operations;

static int fuse_send_open(struct fuse_conn *fc, u64 nodeid, struct file *file,
			  int opcode, struct fuse_open_out *outargp)
=======

static int fuse_send_open(struct fuse_mount *fm, u64 nodeid,
			  unsigned int open_flags, int opcode,
			  struct fuse_open_out *outargp)
>>>>>>> upstream/android-13
{
	struct fuse_open_in inarg;
	FUSE_ARGS(args);

	memset(&inarg, 0, sizeof(inarg));
<<<<<<< HEAD
	inarg.flags = file->f_flags & ~(O_CREAT | O_EXCL | O_NOCTTY);
	if (!fc->atomic_o_trunc)
		inarg.flags &= ~O_TRUNC;
	args.in.h.opcode = opcode;
	args.in.h.nodeid = nodeid;
	args.in.numargs = 1;
	args.in.args[0].size = sizeof(inarg);
	args.in.args[0].value = &inarg;
	args.out.numargs = 1;
	args.out.args[0].size = sizeof(*outargp);
	args.out.args[0].value = outargp;

	return fuse_simple_request(fc, &args);
}

struct fuse_file *fuse_file_alloc(struct fuse_conn *fc)
{
	struct fuse_file *ff;

	ff = kzalloc(sizeof(struct fuse_file), GFP_KERNEL);
	if (unlikely(!ff))
		return NULL;

	ff->fc = fc;
	ff->reserved_req = fuse_request_alloc(0);
	if (unlikely(!ff->reserved_req)) {
=======
	inarg.flags = open_flags & ~(O_CREAT | O_EXCL | O_NOCTTY);
	if (!fm->fc->atomic_o_trunc)
		inarg.flags &= ~O_TRUNC;

	if (fm->fc->handle_killpriv_v2 &&
	    (inarg.flags & O_TRUNC) && !capable(CAP_FSETID)) {
		inarg.open_flags |= FUSE_OPEN_KILL_SUIDGID;
	}

	args.opcode = opcode;
	args.nodeid = nodeid;
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(*outargp);
	args.out_args[0].value = outargp;

	return fuse_simple_request(fm, &args);
}

struct fuse_release_args {
	struct fuse_args args;
	struct fuse_release_in inarg;
	struct inode *inode;
};

struct fuse_file *fuse_file_alloc(struct fuse_mount *fm)
{
	struct fuse_file *ff;

	ff = kzalloc(sizeof(struct fuse_file), GFP_KERNEL_ACCOUNT);
	if (unlikely(!ff))
		return NULL;

	ff->fm = fm;
	ff->release_args = kzalloc(sizeof(*ff->release_args),
				   GFP_KERNEL_ACCOUNT);
	if (!ff->release_args) {
>>>>>>> upstream/android-13
		kfree(ff);
		return NULL;
	}

	INIT_LIST_HEAD(&ff->write_entry);
<<<<<<< HEAD
=======
	mutex_init(&ff->readdir.lock);
>>>>>>> upstream/android-13
	refcount_set(&ff->count, 1);
	RB_CLEAR_NODE(&ff->polled_node);
	init_waitqueue_head(&ff->poll_wait);

<<<<<<< HEAD
	spin_lock(&fc->lock);
	ff->kh = ++fc->khctr;
	spin_unlock(&fc->lock);
=======
	ff->kh = atomic64_inc_return(&fm->fc->khctr);
>>>>>>> upstream/android-13

	return ff;
}

void fuse_file_free(struct fuse_file *ff)
{
<<<<<<< HEAD
	fuse_request_free(ff->reserved_req);
=======
	kfree(ff->release_args);
	mutex_destroy(&ff->readdir.lock);
>>>>>>> upstream/android-13
	kfree(ff);
}

static struct fuse_file *fuse_file_get(struct fuse_file *ff)
{
	refcount_inc(&ff->count);
	return ff;
}

<<<<<<< HEAD
static void fuse_release_end(struct fuse_conn *fc, struct fuse_req *req)
{
	iput(req->misc.release.inode);
=======
static void fuse_release_end(struct fuse_mount *fm, struct fuse_args *args,
			     int error)
{
	struct fuse_release_args *ra = container_of(args, typeof(*ra), args);

	iput(ra->inode);
	kfree(ra);
>>>>>>> upstream/android-13
}

static void fuse_file_put(struct fuse_file *ff, bool sync, bool isdir)
{
	if (refcount_dec_and_test(&ff->count)) {
<<<<<<< HEAD
		struct fuse_req *req = ff->reserved_req;

		if (ff->fc->no_open && !isdir) {
			/*
			 * Drop the release request when client does not
			 * implement 'open'
			 */
			__clear_bit(FR_BACKGROUND, &req->flags);
			iput(req->misc.release.inode);
			fuse_put_request(ff->fc, req);
		} else if (sync) {
			__set_bit(FR_FORCE, &req->flags);
			__clear_bit(FR_BACKGROUND, &req->flags);
			fuse_request_send(ff->fc, req);
			iput(req->misc.release.inode);
			fuse_put_request(ff->fc, req);
		} else {
			req->end = fuse_release_end;
			__set_bit(FR_BACKGROUND, &req->flags);
			fuse_request_send_background(ff->fc, req);
=======
		struct fuse_args *args = &ff->release_args->args;

		if (isdir ? ff->fm->fc->no_opendir : ff->fm->fc->no_open) {
			/* Do nothing when client does not implement 'open' */
			fuse_release_end(ff->fm, args, 0);
		} else if (sync) {
			fuse_simple_request(ff->fm, args);
			fuse_release_end(ff->fm, args, 0);
		} else {
			args->end = fuse_release_end;
			if (fuse_simple_background(ff->fm, args,
						   GFP_KERNEL | __GFP_NOFAIL))
				fuse_release_end(ff->fm, args, -ENOTCONN);
>>>>>>> upstream/android-13
		}
		kfree(ff);
	}
}

<<<<<<< HEAD
int fuse_do_open(struct fuse_conn *fc, u64 nodeid, struct file *file,
		 bool isdir)
{
	struct fuse_file *ff;
	int opcode = isdir ? FUSE_OPENDIR : FUSE_OPEN;

	ff = fuse_file_alloc(fc);
	if (!ff)
		return -ENOMEM;

	ff->fh = 0;
	ff->open_flags = FOPEN_KEEP_CACHE; /* Default for no-open */
	if (!fc->no_open || isdir) {
		struct fuse_open_out outarg;
		int err;

		err = fuse_send_open(fc, nodeid, file, opcode, &outarg);
=======
struct fuse_file *fuse_file_open(struct fuse_mount *fm, u64 nodeid,
				 unsigned int open_flags, bool isdir)
{
	struct fuse_conn *fc = fm->fc;
	struct fuse_file *ff;
	int opcode = isdir ? FUSE_OPENDIR : FUSE_OPEN;

	ff = fuse_file_alloc(fm);
	if (!ff)
		return ERR_PTR(-ENOMEM);

	ff->fh = 0;
	/* Default for no-open */
	ff->open_flags = FOPEN_KEEP_CACHE | (isdir ? FOPEN_CACHE_DIR : 0);
	if (isdir ? !fc->no_opendir : !fc->no_open) {
		struct fuse_open_out outarg;
		int err;

		err = fuse_send_open(fm, nodeid, open_flags, opcode, &outarg);
>>>>>>> upstream/android-13
		if (!err) {
			ff->fh = outarg.fh;
			ff->open_flags = outarg.open_flags;
			fuse_passthrough_setup(fc, ff, &outarg);
<<<<<<< HEAD
		} else if (err != -ENOSYS || isdir) {
			fuse_file_free(ff);
			return err;
		} else {
			fc->no_open = 1;
=======
		} else if (err != -ENOSYS) {
			fuse_file_free(ff);
			return ERR_PTR(err);
		} else {
			if (isdir)
				fc->no_opendir = 1;
			else
				fc->no_open = 1;
>>>>>>> upstream/android-13
		}
	}

	if (isdir)
		ff->open_flags &= ~FOPEN_DIRECT_IO;

	ff->nodeid = nodeid;
<<<<<<< HEAD
	file->private_data = ff;

	return 0;
=======

	return ff;
}

int fuse_do_open(struct fuse_mount *fm, u64 nodeid, struct file *file,
		 bool isdir)
{
	struct fuse_file *ff = fuse_file_open(fm, nodeid, file->f_flags, isdir);

	if (!IS_ERR(ff))
		file->private_data = ff;

	return PTR_ERR_OR_ZERO(ff);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(fuse_do_open);

static void fuse_link_write_file(struct file *file)
{
	struct inode *inode = file_inode(file);
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
>>>>>>> upstream/android-13
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_file *ff = file->private_data;
	/*
	 * file may be written through mmap, so chain it onto the
	 * inodes's write_file list
	 */
<<<<<<< HEAD
	spin_lock(&fc->lock);
	if (list_empty(&ff->write_entry))
		list_add(&ff->write_entry, &fi->write_files);
	spin_unlock(&fc->lock);
=======
	spin_lock(&fi->lock);
	if (list_empty(&ff->write_entry))
		list_add(&ff->write_entry, &fi->write_files);
	spin_unlock(&fi->lock);
>>>>>>> upstream/android-13
}

void fuse_finish_open(struct inode *inode, struct file *file)
{
	struct fuse_file *ff = file->private_data;
	struct fuse_conn *fc = get_fuse_conn(inode);

<<<<<<< HEAD
	if (ff->open_flags & FOPEN_DIRECT_IO)
		file->f_op = &fuse_direct_io_file_operations;
	if (!(ff->open_flags & FOPEN_KEEP_CACHE))
		invalidate_inode_pages2(inode->i_mapping);
=======
>>>>>>> upstream/android-13
	if (ff->open_flags & FOPEN_STREAM)
		stream_open(inode, file);
	else if (ff->open_flags & FOPEN_NONSEEKABLE)
		nonseekable_open(inode, file);
<<<<<<< HEAD
	if (fc->atomic_o_trunc && (file->f_flags & O_TRUNC)) {
		struct fuse_inode *fi = get_fuse_inode(inode);

		spin_lock(&fc->lock);
		fi->attr_version = ++fc->attr_version;
		i_size_write(inode, 0);
		spin_unlock(&fc->lock);
		fuse_invalidate_attr(inode);
		if (fc->writeback_cache)
			file_update_time(file);
	}
=======

	if (fc->atomic_o_trunc && (file->f_flags & O_TRUNC)) {
		struct fuse_inode *fi = get_fuse_inode(inode);

		spin_lock(&fi->lock);
		fi->attr_version = atomic64_inc_return(&fc->attr_version);
		i_size_write(inode, 0);
		spin_unlock(&fi->lock);
		truncate_pagecache(inode, 0);
		fuse_invalidate_attr(inode);
		if (fc->writeback_cache)
			file_update_time(file);
	} else if (!(ff->open_flags & FOPEN_KEEP_CACHE)) {
		invalidate_inode_pages2(inode->i_mapping);
	}

>>>>>>> upstream/android-13
	if ((file->f_mode & FMODE_WRITE) && fc->writeback_cache)
		fuse_link_write_file(file);
}

int fuse_open_common(struct inode *inode, struct file *file, bool isdir)
{
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
	struct fuse_conn *fc = fm->fc;
>>>>>>> upstream/android-13
	int err;
	bool is_wb_truncate = (file->f_flags & O_TRUNC) &&
			  fc->atomic_o_trunc &&
			  fc->writeback_cache;
<<<<<<< HEAD
=======
	bool dax_truncate = (file->f_flags & O_TRUNC) &&
			  fc->atomic_o_trunc && FUSE_IS_DAX(inode);

	if (fuse_is_bad(inode))
		return -EIO;
>>>>>>> upstream/android-13

	err = generic_file_open(inode, file);
	if (err)
		return err;

<<<<<<< HEAD
	if (is_wb_truncate) {
=======
	if (is_wb_truncate || dax_truncate) {
>>>>>>> upstream/android-13
		inode_lock(inode);
		fuse_set_nowrite(inode);
	}

<<<<<<< HEAD
	err = fuse_do_open(fc, get_node_id(inode), file, isdir);

	if (!err)
		fuse_finish_open(inode, file);

	if (is_wb_truncate) {
=======
	if (dax_truncate) {
		filemap_invalidate_lock(inode->i_mapping);
		err = fuse_dax_break_layouts(inode, 0, 0);
		if (err)
			goto out;
	}

	err = fuse_do_open(fm, get_node_id(inode), file, isdir);
	if (!err)
		fuse_finish_open(inode, file);

out:
	if (dax_truncate)
		filemap_invalidate_unlock(inode->i_mapping);

	if (is_wb_truncate | dax_truncate) {
>>>>>>> upstream/android-13
		fuse_release_nowrite(inode);
		inode_unlock(inode);
	}

	return err;
}

<<<<<<< HEAD
static void fuse_prepare_release(struct fuse_file *ff, int flags, int opcode)
{
	struct fuse_conn *fc = ff->fc;
	struct fuse_req *req = ff->reserved_req;
	struct fuse_release_in *inarg = &req->misc.release.in;

	spin_lock(&fc->lock);
	list_del(&ff->write_entry);
=======
static void fuse_prepare_release(struct fuse_inode *fi, struct fuse_file *ff,
				 unsigned int flags, int opcode)
{
	struct fuse_conn *fc = ff->fm->fc;
	struct fuse_release_args *ra = ff->release_args;

	/* Inode is NULL on error path of fuse_create_open() */
	if (likely(fi)) {
		spin_lock(&fi->lock);
		list_del(&ff->write_entry);
		spin_unlock(&fi->lock);
	}
	spin_lock(&fc->lock);
>>>>>>> upstream/android-13
	if (!RB_EMPTY_NODE(&ff->polled_node))
		rb_erase(&ff->polled_node, &fc->polled_files);
	spin_unlock(&fc->lock);

	wake_up_interruptible_all(&ff->poll_wait);

<<<<<<< HEAD
	inarg->fh = ff->fh;
	inarg->flags = flags;
	req->in.h.opcode = opcode;
	req->in.h.nodeid = ff->nodeid;
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(struct fuse_release_in);
	req->in.args[0].value = inarg;
}

void fuse_release_common(struct file *file, bool isdir)
{
	struct fuse_file *ff = file->private_data;
	struct fuse_req *req = ff->reserved_req;
=======
	ra->inarg.fh = ff->fh;
	ra->inarg.flags = flags;
	ra->args.in_numargs = 1;
	ra->args.in_args[0].size = sizeof(struct fuse_release_in);
	ra->args.in_args[0].value = &ra->inarg;
	ra->args.opcode = opcode;
	ra->args.nodeid = ff->nodeid;
	ra->args.force = true;
	ra->args.nocreds = true;
}

void fuse_file_release(struct inode *inode, struct fuse_file *ff,
		       unsigned int open_flags, fl_owner_t id, bool isdir)
{
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_release_args *ra = ff->release_args;
>>>>>>> upstream/android-13
	int opcode = isdir ? FUSE_RELEASEDIR : FUSE_RELEASE;

	fuse_passthrough_release(&ff->passthrough);

<<<<<<< HEAD
	fuse_prepare_release(ff, file->f_flags, opcode);

	if (ff->flock) {
		struct fuse_release_in *inarg = &req->misc.release.in;
		inarg->release_flags |= FUSE_RELEASE_FLOCK_UNLOCK;
		inarg->lock_owner = fuse_lock_owner_id(ff->fc,
						       (fl_owner_t) file);
	}
	/* Hold inode until release is finished */
	req->misc.release.inode = igrab(file_inode(file));
=======
	fuse_prepare_release(fi, ff, open_flags, opcode);

	if (ff->flock) {
		ra->inarg.release_flags |= FUSE_RELEASE_FLOCK_UNLOCK;
		ra->inarg.lock_owner = fuse_lock_owner_id(ff->fm->fc, id);
	}
	/* Hold inode until release is finished */
	ra->inode = igrab(inode);
>>>>>>> upstream/android-13

	/*
	 * Normally this will send the RELEASE request, however if
	 * some asynchronous READ or WRITE requests are outstanding,
	 * the sending will be delayed.
	 *
	 * Make the release synchronous if this is a fuseblk mount,
	 * synchronous RELEASE is allowed (and desirable) in this case
	 * because the server can be trusted not to screw up.
	 */
<<<<<<< HEAD
	fuse_file_put(ff, ff->fc->destroy_req != NULL, isdir);
=======
	fuse_file_put(ff, ff->fm->fc->destroy, isdir);
}

void fuse_release_common(struct file *file, bool isdir)
{
	fuse_file_release(file_inode(file), file->private_data, file->f_flags,
			  (fl_owner_t) file, isdir);
>>>>>>> upstream/android-13
}

static int fuse_open(struct inode *inode, struct file *file)
{
	return fuse_open_common(inode, file, false);
}

static int fuse_release(struct inode *inode, struct file *file)
{
	struct fuse_conn *fc = get_fuse_conn(inode);

	/* see fuse_vma_close() for !writeback_cache case */
	if (fc->writeback_cache)
		write_inode_now(inode, 1);

	fuse_release_common(file, false);

	/* return value is ignored by VFS */
	return 0;
}

<<<<<<< HEAD
void fuse_sync_release(struct fuse_file *ff, int flags)
{
	WARN_ON(refcount_read(&ff->count) > 1);
	fuse_prepare_release(ff, flags, FUSE_RELEASE);
=======
void fuse_sync_release(struct fuse_inode *fi, struct fuse_file *ff,
		       unsigned int flags)
{
	WARN_ON(refcount_read(&ff->count) > 1);
	fuse_prepare_release(fi, ff, flags, FUSE_RELEASE);
>>>>>>> upstream/android-13
	/*
	 * iput(NULL) is a no-op and since the refcount is 1 and everything's
	 * synchronous, we are fine with not doing igrab() here"
	 */
	fuse_file_put(ff, true, false);
}
EXPORT_SYMBOL_GPL(fuse_sync_release);

/*
 * Scramble the ID space with XTEA, so that the value of the files_struct
 * pointer is not exposed to userspace.
 */
u64 fuse_lock_owner_id(struct fuse_conn *fc, fl_owner_t id)
{
	u32 *k = fc->scramble_key;
	u64 v = (unsigned long) id;
	u32 v0 = v;
	u32 v1 = v >> 32;
	u32 sum = 0;
	int i;

	for (i = 0; i < 32; i++) {
		v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + k[sum & 3]);
		sum += 0x9E3779B9;
		v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + k[sum>>11 & 3]);
	}

	return (u64) v0 + ((u64) v1 << 32);
}

<<<<<<< HEAD
=======
struct fuse_writepage_args {
	struct fuse_io_args ia;
	struct rb_node writepages_entry;
	struct list_head queue_entry;
	struct fuse_writepage_args *next;
	struct inode *inode;
	struct fuse_sync_bucket *bucket;
};

static struct fuse_writepage_args *fuse_find_writeback(struct fuse_inode *fi,
					    pgoff_t idx_from, pgoff_t idx_to)
{
	struct rb_node *n;

	n = fi->writepages.rb_node;

	while (n) {
		struct fuse_writepage_args *wpa;
		pgoff_t curr_index;

		wpa = rb_entry(n, struct fuse_writepage_args, writepages_entry);
		WARN_ON(get_fuse_inode(wpa->inode) != fi);
		curr_index = wpa->ia.write.in.offset >> PAGE_SHIFT;
		if (idx_from >= curr_index + wpa->ia.ap.num_pages)
			n = n->rb_right;
		else if (idx_to < curr_index)
			n = n->rb_left;
		else
			return wpa;
	}
	return NULL;
}

>>>>>>> upstream/android-13
/*
 * Check if any page in a range is under writeback
 *
 * This is currently done by walking the list of writepage requests
 * for the inode, which can be pretty inefficient.
 */
static bool fuse_range_is_writeback(struct inode *inode, pgoff_t idx_from,
				   pgoff_t idx_to)
{
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_req *req;
	bool found = false;

	spin_lock(&fc->lock);
	list_for_each_entry(req, &fi->writepages, writepages_entry) {
		pgoff_t curr_index;

		BUG_ON(req->inode != inode);
		curr_index = req->misc.write.in.offset >> PAGE_SHIFT;
		if (idx_from < curr_index + req->num_pages &&
		    curr_index <= idx_to) {
			found = true;
			break;
		}
	}
	spin_unlock(&fc->lock);
=======
	struct fuse_inode *fi = get_fuse_inode(inode);
	bool found;

	spin_lock(&fi->lock);
	found = fuse_find_writeback(fi, idx_from, idx_to);
	spin_unlock(&fi->lock);
>>>>>>> upstream/android-13

	return found;
}

static inline bool fuse_page_is_writeback(struct inode *inode, pgoff_t index)
{
	return fuse_range_is_writeback(inode, index, index);
}

/*
 * Wait for page writeback to be completed.
 *
 * Since fuse doesn't rely on the VM writeback tracking, this has to
 * use some other means.
 */
<<<<<<< HEAD
static int fuse_wait_on_page_writeback(struct inode *inode, pgoff_t index)
=======
static void fuse_wait_on_page_writeback(struct inode *inode, pgoff_t index)
>>>>>>> upstream/android-13
{
	struct fuse_inode *fi = get_fuse_inode(inode);

	fuse_wait_event(fi->page_waitq, !fuse_page_is_writeback(inode, index));
<<<<<<< HEAD
	return 0;
=======
>>>>>>> upstream/android-13
}

/*
 * Wait for all pending writepages on the inode to finish.
 *
 * This is currently done by blocking further writes with FUSE_NOWRITE
 * and waiting for all sent writes to complete.
 *
 * This must be called under i_mutex, otherwise the FUSE_NOWRITE usage
 * could conflict with truncation.
 */
static void fuse_sync_writes(struct inode *inode)
{
	fuse_set_nowrite(inode);
	fuse_release_nowrite(inode);
}

static int fuse_flush(struct file *file, fl_owner_t id)
{
	struct inode *inode = file_inode(file);
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_file *ff = file->private_data;
	struct fuse_req *req;
	struct fuse_flush_in inarg;
	int err;

	if (is_bad_inode(inode))
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
	struct fuse_file *ff = file->private_data;
	struct fuse_flush_in inarg;
	FUSE_ARGS(args);
	int err;

	if (fuse_is_bad(inode))
>>>>>>> upstream/android-13
		return -EIO;

	err = write_inode_now(inode, 1);
	if (err)
		return err;

	inode_lock(inode);
	fuse_sync_writes(inode);
	inode_unlock(inode);

	err = filemap_check_errors(file->f_mapping);
	if (err)
		return err;

<<<<<<< HEAD
	if (fc->no_flush)
		return 0;

	req = fuse_get_req_nofail_nopages(fc, file);
	memset(&inarg, 0, sizeof(inarg));
	inarg.fh = ff->fh;
	inarg.lock_owner = fuse_lock_owner_id(fc, id);
	req->in.h.opcode = FUSE_FLUSH;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	__set_bit(FR_FORCE, &req->flags);
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (err == -ENOSYS) {
		fc->no_flush = 1;
		err = 0;
	}
=======
	err = 0;
	if (fm->fc->no_flush)
		goto inval_attr_out;

	memset(&inarg, 0, sizeof(inarg));
	inarg.fh = ff->fh;
	inarg.lock_owner = fuse_lock_owner_id(fm->fc, id);
	args.opcode = FUSE_FLUSH;
	args.nodeid = get_node_id(inode);
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.force = true;

	err = fuse_simple_request(fm, &args);
	if (err == -ENOSYS) {
		fm->fc->no_flush = 1;
		err = 0;
	}

inval_attr_out:
	/*
	 * In memory i_blocks is not maintained by fuse, if writeback cache is
	 * enabled, i_blocks from cached attr may not be accurate.
	 */
	if (!err && fm->fc->writeback_cache)
		fuse_invalidate_attr(inode);
>>>>>>> upstream/android-13
	return err;
}

int fuse_fsync_common(struct file *file, loff_t start, loff_t end,
<<<<<<< HEAD
		      int datasync, int isdir)
{
	struct inode *inode = file->f_mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_file *ff = file->private_data;
	FUSE_ARGS(args);
	struct fuse_fsync_in inarg;
	int err;

	if (is_bad_inode(inode))
=======
		      int datasync, int opcode)
{
	struct inode *inode = file->f_mapping->host;
	struct fuse_mount *fm = get_fuse_mount(inode);
	struct fuse_file *ff = file->private_data;
	FUSE_ARGS(args);
	struct fuse_fsync_in inarg;

	memset(&inarg, 0, sizeof(inarg));
	inarg.fh = ff->fh;
	inarg.fsync_flags = datasync ? FUSE_FSYNC_FDATASYNC : 0;
	args.opcode = opcode;
	args.nodeid = get_node_id(inode);
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	return fuse_simple_request(fm, &args);
}

static int fuse_fsync(struct file *file, loff_t start, loff_t end,
		      int datasync)
{
	struct inode *inode = file->f_mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	int err;

	if (fuse_is_bad(inode))
>>>>>>> upstream/android-13
		return -EIO;

	inode_lock(inode);

	/*
	 * Start writeback against all dirty pages of the inode, then
	 * wait for all outstanding writes, before sending the FSYNC
	 * request.
	 */
	err = file_write_and_wait_range(file, start, end);
	if (err)
		goto out;

	fuse_sync_writes(inode);

	/*
	 * Due to implementation of fuse writeback
	 * file_write_and_wait_range() does not catch errors.
	 * We have to do this directly after fuse_sync_writes()
	 */
	err = file_check_and_advance_wb_err(file);
	if (err)
		goto out;

	err = sync_inode_metadata(inode, 1);
	if (err)
		goto out;

<<<<<<< HEAD
	if ((!isdir && fc->no_fsync) || (isdir && fc->no_fsyncdir))
		goto out;

	memset(&inarg, 0, sizeof(inarg));
	inarg.fh = ff->fh;
	inarg.fsync_flags = datasync ? 1 : 0;
	args.in.h.opcode = isdir ? FUSE_FSYNCDIR : FUSE_FSYNC;
	args.in.h.nodeid = get_node_id(inode);
	args.in.numargs = 1;
	args.in.args[0].size = sizeof(inarg);
	args.in.args[0].value = &inarg;
	err = fuse_simple_request(fc, &args);
	if (err == -ENOSYS) {
		if (isdir)
			fc->no_fsyncdir = 1;
		else
			fc->no_fsync = 1;
=======
	if (fc->no_fsync)
		goto out;

	err = fuse_fsync_common(file, start, end, datasync, FUSE_FSYNC);
	if (err == -ENOSYS) {
		fc->no_fsync = 1;
>>>>>>> upstream/android-13
		err = 0;
	}
out:
	inode_unlock(inode);
<<<<<<< HEAD
	return err;
}

static int fuse_fsync(struct file *file, loff_t start, loff_t end,
		      int datasync)
{
	return fuse_fsync_common(file, start, end, datasync, 0);
}

void fuse_read_fill(struct fuse_req *req, struct file *file, loff_t pos,
		    size_t count, int opcode)
{
	struct fuse_read_in *inarg = &req->misc.read.in;
	struct fuse_file *ff = file->private_data;

	inarg->fh = ff->fh;
	inarg->offset = pos;
	inarg->size = count;
	inarg->flags = file->f_flags;
	req->in.h.opcode = opcode;
	req->in.h.nodeid = ff->nodeid;
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(struct fuse_read_in);
	req->in.args[0].value = inarg;
	req->out.argvar = 1;
	req->out.numargs = 1;
	req->out.args[0].size = count;
}

static void fuse_release_user_pages(struct fuse_req *req, bool should_dirty)
{
	unsigned i;

	for (i = 0; i < req->num_pages; i++) {
		struct page *page = req->pages[i];
		if (should_dirty)
			set_page_dirty_lock(page);
		put_page(page);
=======

	return err;
}

void fuse_read_args_fill(struct fuse_io_args *ia, struct file *file, loff_t pos,
			 size_t count, int opcode)
{
	struct fuse_file *ff = file->private_data;
	struct fuse_args *args = &ia->ap.args;

	ia->read.in.fh = ff->fh;
	ia->read.in.offset = pos;
	ia->read.in.size = count;
	ia->read.in.flags = file->f_flags;
	args->opcode = opcode;
	args->nodeid = ff->nodeid;
	args->in_numargs = 1;
	args->in_args[0].size = sizeof(ia->read.in);
	args->in_args[0].value = &ia->read.in;
	args->out_argvar = true;
	args->out_numargs = 1;
	args->out_args[0].size = count;
}

static void fuse_release_user_pages(struct fuse_args_pages *ap,
				    bool should_dirty)
{
	unsigned int i;

	for (i = 0; i < ap->num_pages; i++) {
		if (should_dirty)
			set_page_dirty_lock(ap->pages[i]);
		put_page(ap->pages[i]);
>>>>>>> upstream/android-13
	}
}

static void fuse_io_release(struct kref *kref)
{
	kfree(container_of(kref, struct fuse_io_priv, refcnt));
}

static ssize_t fuse_get_res_by_io(struct fuse_io_priv *io)
{
	if (io->err)
		return io->err;

	if (io->bytes >= 0 && io->write)
		return -EIO;

	return io->bytes < 0 ? io->size : io->bytes;
}

/**
 * In case of short read, the caller sets 'pos' to the position of
 * actual end of fuse request in IO request. Otherwise, if bytes_requested
 * == bytes_transferred or rw == WRITE, the caller sets 'pos' to -1.
 *
 * An example:
<<<<<<< HEAD
 * User requested DIO read of 64K. It was splitted into two 32K fuse requests,
=======
 * User requested DIO read of 64K. It was split into two 32K fuse requests,
>>>>>>> upstream/android-13
 * both submitted asynchronously. The first of them was ACKed by userspace as
 * fully completed (req->out.args[0].size == 32K) resulting in pos == -1. The
 * second request was ACKed as short, e.g. only 1K was read, resulting in
 * pos == 33K.
 *
 * Thus, when all fuse requests are completed, the minimal non-negative 'pos'
 * will be equal to the length of the longest contiguous fragment of
 * transferred data starting from the beginning of IO request.
 */
static void fuse_aio_complete(struct fuse_io_priv *io, int err, ssize_t pos)
{
	int left;

	spin_lock(&io->lock);
	if (err)
		io->err = io->err ? : err;
	else if (pos >= 0 && (io->bytes < 0 || pos < io->bytes))
		io->bytes = pos;

	left = --io->reqs;
	if (!left && io->blocking)
		complete(io->done);
	spin_unlock(&io->lock);

	if (!left && !io->blocking) {
		ssize_t res = fuse_get_res_by_io(io);

		if (res >= 0) {
			struct inode *inode = file_inode(io->iocb->ki_filp);
			struct fuse_conn *fc = get_fuse_conn(inode);
			struct fuse_inode *fi = get_fuse_inode(inode);

<<<<<<< HEAD
			spin_lock(&fc->lock);
			fi->attr_version = ++fc->attr_version;
			spin_unlock(&fc->lock);
=======
			spin_lock(&fi->lock);
			fi->attr_version = atomic64_inc_return(&fc->attr_version);
			spin_unlock(&fi->lock);
>>>>>>> upstream/android-13
		}

		io->iocb->ki_complete(io->iocb, res, 0);
	}

	kref_put(&io->refcnt, fuse_io_release);
}

<<<<<<< HEAD
static void fuse_aio_complete_req(struct fuse_conn *fc, struct fuse_req *req)
{
	struct fuse_io_priv *io = req->io;
	ssize_t pos = -1;

	fuse_release_user_pages(req, io->should_dirty);

	if (io->write) {
		if (req->misc.write.in.size != req->misc.write.out.size)
			pos = req->misc.write.in.offset - io->offset +
				req->misc.write.out.size;
	} else {
		if (req->misc.read.in.size != req->out.args[0].size)
			pos = req->misc.read.in.offset - io->offset +
				req->out.args[0].size;
	}

	fuse_aio_complete(io, req->out.h.error, pos);
}

static size_t fuse_async_req_send(struct fuse_conn *fc, struct fuse_req *req,
		size_t num_bytes, struct fuse_io_priv *io)
{
=======
static struct fuse_io_args *fuse_io_alloc(struct fuse_io_priv *io,
					  unsigned int npages)
{
	struct fuse_io_args *ia;

	ia = kzalloc(sizeof(*ia), GFP_KERNEL);
	if (ia) {
		ia->io = io;
		ia->ap.pages = fuse_pages_alloc(npages, GFP_KERNEL,
						&ia->ap.descs);
		if (!ia->ap.pages) {
			kfree(ia);
			ia = NULL;
		}
	}
	return ia;
}

static void fuse_io_free(struct fuse_io_args *ia)
{
	kfree(ia->ap.pages);
	kfree(ia);
}

static void fuse_aio_complete_req(struct fuse_mount *fm, struct fuse_args *args,
				  int err)
{
	struct fuse_io_args *ia = container_of(args, typeof(*ia), ap.args);
	struct fuse_io_priv *io = ia->io;
	ssize_t pos = -1;

	fuse_release_user_pages(&ia->ap, io->should_dirty);

	if (err) {
		/* Nothing */
	} else if (io->write) {
		if (ia->write.out.size > ia->write.in.size) {
			err = -EIO;
		} else if (ia->write.in.size != ia->write.out.size) {
			pos = ia->write.in.offset - io->offset +
				ia->write.out.size;
		}
	} else {
		u32 outsize = args->out_args[0].size;

		if (ia->read.in.size != outsize)
			pos = ia->read.in.offset - io->offset + outsize;
	}

	fuse_aio_complete(io, err, pos);
	fuse_io_free(ia);
}

static ssize_t fuse_async_req_send(struct fuse_mount *fm,
				   struct fuse_io_args *ia, size_t num_bytes)
{
	ssize_t err;
	struct fuse_io_priv *io = ia->io;

>>>>>>> upstream/android-13
	spin_lock(&io->lock);
	kref_get(&io->refcnt);
	io->size += num_bytes;
	io->reqs++;
	spin_unlock(&io->lock);

<<<<<<< HEAD
	req->io = io;
	req->end = fuse_aio_complete_req;

	__fuse_get_request(req);
	fuse_request_send_background(fc, req);
=======
	ia->ap.args.end = fuse_aio_complete_req;
	ia->ap.args.may_block = io->should_dirty;
	err = fuse_simple_background(fm, &ia->ap.args, GFP_KERNEL);
	if (err)
		fuse_aio_complete_req(fm, &ia->ap.args, err);
>>>>>>> upstream/android-13

	return num_bytes;
}

<<<<<<< HEAD
static size_t fuse_send_read(struct fuse_req *req, struct fuse_io_priv *io,
			     loff_t pos, size_t count, fl_owner_t owner)
{
	struct file *file = io->iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct fuse_conn *fc = ff->fc;

	fuse_read_fill(req, file, pos, count, FUSE_READ);
	if (owner != NULL) {
		struct fuse_read_in *inarg = &req->misc.read.in;

		inarg->read_flags |= FUSE_READ_LOCKOWNER;
		inarg->lock_owner = fuse_lock_owner_id(fc, owner);
	}

	if (io->async)
		return fuse_async_req_send(fc, req, count, io);

	fuse_request_send(fc, req);
	return req->out.args[0].size;
=======
static ssize_t fuse_send_read(struct fuse_io_args *ia, loff_t pos, size_t count,
			      fl_owner_t owner)
{
	struct file *file = ia->io->iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct fuse_mount *fm = ff->fm;

	fuse_read_args_fill(ia, file, pos, count, FUSE_READ);
	if (owner != NULL) {
		ia->read.in.read_flags |= FUSE_READ_LOCKOWNER;
		ia->read.in.lock_owner = fuse_lock_owner_id(fm->fc, owner);
	}

	if (ia->io->async)
		return fuse_async_req_send(fm, ia, count);

	return fuse_simple_request(fm, &ia->ap.args);
>>>>>>> upstream/android-13
}

static void fuse_read_update_size(struct inode *inode, loff_t size,
				  u64 attr_ver)
{
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);

<<<<<<< HEAD
	spin_lock(&fc->lock);
	if (attr_ver == fi->attr_version && size < inode->i_size &&
	    !test_bit(FUSE_I_SIZE_UNSTABLE, &fi->state)) {
		fi->attr_version = ++fc->attr_version;
		i_size_write(inode, size);
	}
	spin_unlock(&fc->lock);
}

static void fuse_short_read(struct fuse_req *req, struct inode *inode,
			    u64 attr_ver)
{
	size_t num_read = req->out.args[0].size;
	struct fuse_conn *fc = get_fuse_conn(inode);

	if (fc->writeback_cache) {
		/*
		 * A hole in a file. Some data after the hole are in page cache,
		 * but have not reached the client fs yet. So, the hole is not
		 * present there.
		 */
		int i;
		int start_idx = num_read >> PAGE_SHIFT;
		size_t off = num_read & (PAGE_SIZE - 1);

		for (i = start_idx; i < req->num_pages; i++) {
			zero_user_segment(req->pages[i], off, PAGE_SIZE);
			off = 0;
		}
	} else {
		loff_t pos = page_offset(req->pages[0]) + num_read;
=======
	spin_lock(&fi->lock);
	if (attr_ver == fi->attr_version && size < inode->i_size &&
	    !test_bit(FUSE_I_SIZE_UNSTABLE, &fi->state)) {
		fi->attr_version = atomic64_inc_return(&fc->attr_version);
		i_size_write(inode, size);
	}
	spin_unlock(&fi->lock);
}

static void fuse_short_read(struct inode *inode, u64 attr_ver, size_t num_read,
			    struct fuse_args_pages *ap)
{
	struct fuse_conn *fc = get_fuse_conn(inode);

	/*
	 * If writeback_cache is enabled, a short read means there's a hole in
	 * the file.  Some data after the hole is in page cache, but has not
	 * reached the client fs yet.  So the hole is not present there.
	 */
	if (!fc->writeback_cache) {
		loff_t pos = page_offset(ap->pages[0]) + num_read;
>>>>>>> upstream/android-13
		fuse_read_update_size(inode, pos, attr_ver);
	}
}

static int fuse_do_readpage(struct file *file, struct page *page)
{
<<<<<<< HEAD
	struct kiocb iocb;
	struct fuse_io_priv io;
	struct inode *inode = page->mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	size_t num_read;
	loff_t pos = page_offset(page);
	size_t count = PAGE_SIZE;
	u64 attr_ver;
	int err;
=======
	struct inode *inode = page->mapping->host;
	struct fuse_mount *fm = get_fuse_mount(inode);
	loff_t pos = page_offset(page);
	struct fuse_page_desc desc = { .length = PAGE_SIZE };
	struct fuse_io_args ia = {
		.ap.args.page_zeroing = true,
		.ap.args.out_pages = true,
		.ap.num_pages = 1,
		.ap.pages = &page,
		.ap.descs = &desc,
	};
	ssize_t res;
	u64 attr_ver;
>>>>>>> upstream/android-13

	/*
	 * Page writeback can extend beyond the lifetime of the
	 * page-cache page, so make sure we read a properly synced
	 * page.
	 */
	fuse_wait_on_page_writeback(inode, page->index);

<<<<<<< HEAD
	req = fuse_get_req(fc, 1);
	if (IS_ERR(req))
		return PTR_ERR(req);

	attr_ver = fuse_get_attr_version(fc);

	req->out.page_zeroing = 1;
	req->out.argpages = 1;
	req->num_pages = 1;
	req->pages[0] = page;
	req->page_descs[0].length = count;
	init_sync_kiocb(&iocb, file);
	io = (struct fuse_io_priv) FUSE_IO_PRIV_SYNC(&iocb);
	num_read = fuse_send_read(req, &io, pos, count, NULL);
	err = req->out.h.error;

	if (!err) {
		/*
		 * Short read means EOF.  If file size is larger, truncate it
		 */
		if (num_read < count)
			fuse_short_read(req, inode, attr_ver);

		SetPageUptodate(page);
	}

	fuse_put_request(fc, req);

	return err;
=======
	attr_ver = fuse_get_attr_version(fm->fc);

	/* Don't overflow end offset */
	if (pos + (desc.length - 1) == LLONG_MAX)
		desc.length--;

	fuse_read_args_fill(&ia, file, pos, desc.length, FUSE_READ);
	res = fuse_simple_request(fm, &ia.ap.args);
	if (res < 0)
		return res;
	/*
	 * Short read means EOF.  If file size is larger, truncate it
	 */
	if (res < desc.length)
		fuse_short_read(inode, attr_ver, res, &ia.ap);

	SetPageUptodate(page);

	return 0;
>>>>>>> upstream/android-13
}

static int fuse_readpage(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
	int err;

	err = -EIO;
<<<<<<< HEAD
	if (is_bad_inode(inode))
=======
	if (fuse_is_bad(inode))
>>>>>>> upstream/android-13
		goto out;

	err = fuse_do_readpage(file, page);
	fuse_invalidate_atime(inode);
 out:
	unlock_page(page);
	return err;
}

<<<<<<< HEAD
static void fuse_readpages_end(struct fuse_conn *fc, struct fuse_req *req)
{
	int i;
	size_t count = req->misc.read.in.size;
	size_t num_read = req->out.args[0].size;
	struct address_space *mapping = NULL;

	for (i = 0; mapping == NULL && i < req->num_pages; i++)
		mapping = req->pages[i]->mapping;
=======
static void fuse_readpages_end(struct fuse_mount *fm, struct fuse_args *args,
			       int err)
{
	int i;
	struct fuse_io_args *ia = container_of(args, typeof(*ia), ap.args);
	struct fuse_args_pages *ap = &ia->ap;
	size_t count = ia->read.in.size;
	size_t num_read = args->out_args[0].size;
	struct address_space *mapping = NULL;

	for (i = 0; mapping == NULL && i < ap->num_pages; i++)
		mapping = ap->pages[i]->mapping;
>>>>>>> upstream/android-13

	if (mapping) {
		struct inode *inode = mapping->host;

		/*
		 * Short read means EOF. If file size is larger, truncate it
		 */
<<<<<<< HEAD
		if (!req->out.h.error && num_read < count)
			fuse_short_read(req, inode, req->misc.read.attr_ver);
=======
		if (!err && num_read < count)
			fuse_short_read(inode, ia->read.attr_ver, num_read, ap);
>>>>>>> upstream/android-13

		fuse_invalidate_atime(inode);
	}

<<<<<<< HEAD
	for (i = 0; i < req->num_pages; i++) {
		struct page *page = req->pages[i];
		if (!req->out.h.error)
=======
	for (i = 0; i < ap->num_pages; i++) {
		struct page *page = ap->pages[i];

		if (!err)
>>>>>>> upstream/android-13
			SetPageUptodate(page);
		else
			SetPageError(page);
		unlock_page(page);
		put_page(page);
	}
<<<<<<< HEAD
	if (req->ff)
		fuse_file_put(req->ff, false, false);
}

static void fuse_send_readpages(struct fuse_req *req, struct file *file)
{
	struct fuse_file *ff = file->private_data;
	struct fuse_conn *fc = ff->fc;
	loff_t pos = page_offset(req->pages[0]);
	size_t count = req->num_pages << PAGE_SHIFT;

	req->out.argpages = 1;
	req->out.page_zeroing = 1;
	req->out.page_replace = 1;
	fuse_read_fill(req, file, pos, count, FUSE_READ);
	req->misc.read.attr_ver = fuse_get_attr_version(fc);

	mtk_btag_pidlog_set_pid_pages(req->pages, req->num_pages,
				      PIDLOG_MODE_FS_FUSE, false);

	if (fc->async_read) {
		req->ff = fuse_file_get(ff);
		req->end = fuse_readpages_end;
		fuse_request_send_background(fc, req);
	} else {
		fuse_request_send(fc, req);
		fuse_readpages_end(fc, req);
		fuse_put_request(fc, req);
	}
}

struct fuse_fill_data {
	struct fuse_req *req;
	struct file *file;
	struct inode *inode;
	unsigned nr_pages;
};

static int fuse_readpages_fill(void *_data, struct page *page)
{
	struct fuse_fill_data *data = _data;
	struct fuse_req *req = data->req;
	struct inode *inode = data->inode;
	struct fuse_conn *fc = get_fuse_conn(inode);

	fuse_wait_on_page_writeback(inode, page->index);

	if (req->num_pages &&
	    (req->num_pages == FUSE_MAX_PAGES_PER_REQ ||
	     (req->num_pages + 1) * PAGE_SIZE > fc->max_read ||
	     req->pages[req->num_pages - 1]->index + 1 != page->index)) {
		int nr_alloc = min_t(unsigned, data->nr_pages,
				     FUSE_MAX_PAGES_PER_REQ);
		fuse_send_readpages(req, data->file);
		if (fc->async_read)
			req = fuse_get_req_for_background(fc, nr_alloc);
		else
			req = fuse_get_req(fc, nr_alloc);

		data->req = req;
		if (IS_ERR(req)) {
			unlock_page(page);
			return PTR_ERR(req);
		}
	}

	if (WARN_ON(req->num_pages >= req->max_pages)) {
		unlock_page(page);
		fuse_put_request(fc, req);
		return -EIO;
	}

	get_page(page);
	req->pages[req->num_pages] = page;
	req->page_descs[req->num_pages].length = PAGE_SIZE;
	req->num_pages++;
	data->nr_pages--;
	return 0;
}

static int fuse_readpages(struct file *file, struct address_space *mapping,
			  struct list_head *pages, unsigned nr_pages)
{
	struct inode *inode = mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_fill_data data;
	int err;
	int nr_alloc = min_t(unsigned, nr_pages, FUSE_MAX_PAGES_PER_REQ);

	err = -EIO;
	if (is_bad_inode(inode))
		goto out;

	data.file = file;
	data.inode = inode;
	if (fc->async_read)
		data.req = fuse_get_req_for_background(fc, nr_alloc);
	else
		data.req = fuse_get_req(fc, nr_alloc);
	data.nr_pages = nr_pages;
	err = PTR_ERR(data.req);
	if (IS_ERR(data.req))
		goto out;

	err = read_cache_pages(mapping, pages, fuse_readpages_fill, &data);
	if (!err) {
		if (data.req->num_pages)
			fuse_send_readpages(data.req, file);
		else
			fuse_put_request(fc, data.req);
	}
out:
	return err;
}

static ssize_t fuse_file_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct inode *inode = iocb->ki_filp->f_mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_file *ff = iocb->ki_filp->private_data;

	if (is_bad_inode(inode))
		return -EIO;
=======
	if (ia->ff)
		fuse_file_put(ia->ff, false, false);

	fuse_io_free(ia);
}

static void fuse_send_readpages(struct fuse_io_args *ia, struct file *file)
{
	struct fuse_file *ff = file->private_data;
	struct fuse_mount *fm = ff->fm;
	struct fuse_args_pages *ap = &ia->ap;
	loff_t pos = page_offset(ap->pages[0]);
	size_t count = ap->num_pages << PAGE_SHIFT;
	ssize_t res;
	int err;

	ap->args.out_pages = true;
	ap->args.page_zeroing = true;
	ap->args.page_replace = true;

	/* Don't overflow end offset */
	if (pos + (count - 1) == LLONG_MAX) {
		count--;
		ap->descs[ap->num_pages - 1].length--;
	}
	WARN_ON((loff_t) (pos + count) < 0);

	fuse_read_args_fill(ia, file, pos, count, FUSE_READ);
	ia->read.attr_ver = fuse_get_attr_version(fm->fc);
	if (fm->fc->async_read) {
		ia->ff = fuse_file_get(ff);
		ap->args.end = fuse_readpages_end;
		err = fuse_simple_background(fm, &ap->args, GFP_KERNEL);
		if (!err)
			return;
	} else {
		res = fuse_simple_request(fm, &ap->args);
		err = res < 0 ? res : 0;
	}
	fuse_readpages_end(fm, &ap->args, err);
}

static void fuse_readahead(struct readahead_control *rac)
{
	struct inode *inode = rac->mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	unsigned int i, max_pages, nr_pages = 0;

	if (fuse_is_bad(inode))
		return;

	max_pages = min_t(unsigned int, fc->max_pages,
			fc->max_read / PAGE_SIZE);

	for (;;) {
		struct fuse_io_args *ia;
		struct fuse_args_pages *ap;

		nr_pages = readahead_count(rac) - nr_pages;
		if (nr_pages > max_pages)
			nr_pages = max_pages;
		if (nr_pages == 0)
			break;
		ia = fuse_io_alloc(NULL, nr_pages);
		if (!ia)
			return;
		ap = &ia->ap;
		nr_pages = __readahead_batch(rac, ap->pages, nr_pages);
		for (i = 0; i < nr_pages; i++) {
			fuse_wait_on_page_writeback(inode,
						    readahead_index(rac) + i);
			ap->descs[i].length = PAGE_SIZE;
		}
		ap->num_pages = nr_pages;
		fuse_send_readpages(ia, rac->file);
	}
}

static ssize_t fuse_cache_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct inode *inode = iocb->ki_filp->f_mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
>>>>>>> upstream/android-13

	/*
	 * In auto invalidate mode, always update attributes on read.
	 * Otherwise, only update if we attempt to read past EOF (to ensure
	 * i_size is up to date).
	 */
	if (fc->auto_inval_data ||
	    (iocb->ki_pos + iov_iter_count(to) > i_size_read(inode))) {
		int err;
		err = fuse_update_attributes(inode, iocb->ki_filp);
		if (err)
			return err;
	}

<<<<<<< HEAD
	if (ff->passthrough.filp)
		return fuse_passthrough_read_iter(iocb, to);
	else
		return generic_file_read_iter(iocb, to);
}

static void fuse_write_fill(struct fuse_req *req, struct fuse_file *ff,
			    loff_t pos, size_t count)
{
	struct fuse_write_in *inarg = &req->misc.write.in;
	struct fuse_write_out *outarg = &req->misc.write.out;

	inarg->fh = ff->fh;
	inarg->offset = pos;
	inarg->size = count;
	req->in.h.opcode = FUSE_WRITE;
	req->in.h.nodeid = ff->nodeid;
	req->in.numargs = 2;
	if (ff->fc->minor < 9)
		req->in.args[0].size = FUSE_COMPAT_WRITE_IN_SIZE;
	else
		req->in.args[0].size = sizeof(struct fuse_write_in);
	req->in.args[0].value = inarg;
	req->in.args[1].size = count;
	req->out.numargs = 1;
	req->out.args[0].size = sizeof(struct fuse_write_out);
	req->out.args[0].value = outarg;
}

static size_t fuse_send_write(struct fuse_req *req, struct fuse_io_priv *io,
			      loff_t pos, size_t count, fl_owner_t owner)
{
	struct kiocb *iocb = io->iocb;
	struct file *file = iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct fuse_conn *fc = ff->fc;
	struct fuse_write_in *inarg = &req->misc.write.in;

	fuse_write_fill(req, ff, pos, count);
	inarg->flags = file->f_flags;
	if (iocb->ki_flags & IOCB_DSYNC)
		inarg->flags |= O_DSYNC;
	if (iocb->ki_flags & IOCB_SYNC)
		inarg->flags |= O_SYNC;
	if (owner != NULL) {
		inarg->write_flags |= FUSE_WRITE_LOCKOWNER;
		inarg->lock_owner = fuse_lock_owner_id(fc, owner);
	}

	if (io->async)
		return fuse_async_req_send(fc, req, count, io);

	fuse_request_send(fc, req);
	return req->misc.write.out.size;
=======
	return generic_file_read_iter(iocb, to);
}

static void fuse_write_args_fill(struct fuse_io_args *ia, struct fuse_file *ff,
				 loff_t pos, size_t count)
{
	struct fuse_args *args = &ia->ap.args;

	ia->write.in.fh = ff->fh;
	ia->write.in.offset = pos;
	ia->write.in.size = count;
	args->opcode = FUSE_WRITE;
	args->nodeid = ff->nodeid;
	args->in_numargs = 2;
	if (ff->fm->fc->minor < 9)
		args->in_args[0].size = FUSE_COMPAT_WRITE_IN_SIZE;
	else
		args->in_args[0].size = sizeof(ia->write.in);
	args->in_args[0].value = &ia->write.in;
	args->in_args[1].size = count;
	args->out_numargs = 1;
	args->out_args[0].size = sizeof(ia->write.out);
	args->out_args[0].value = &ia->write.out;
}

static unsigned int fuse_write_flags(struct kiocb *iocb)
{
	unsigned int flags = iocb->ki_filp->f_flags;

	if (iocb->ki_flags & IOCB_DSYNC)
		flags |= O_DSYNC;
	if (iocb->ki_flags & IOCB_SYNC)
		flags |= O_SYNC;

	return flags;
}

static ssize_t fuse_send_write(struct fuse_io_args *ia, loff_t pos,
			       size_t count, fl_owner_t owner)
{
	struct kiocb *iocb = ia->io->iocb;
	struct file *file = iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct fuse_mount *fm = ff->fm;
	struct fuse_write_in *inarg = &ia->write.in;
	ssize_t err;

	fuse_write_args_fill(ia, ff, pos, count);
	inarg->flags = fuse_write_flags(iocb);
	if (owner != NULL) {
		inarg->write_flags |= FUSE_WRITE_LOCKOWNER;
		inarg->lock_owner = fuse_lock_owner_id(fm->fc, owner);
	}

	if (ia->io->async)
		return fuse_async_req_send(fm, ia, count);

	err = fuse_simple_request(fm, &ia->ap.args);
	if (!err && ia->write.out.size > count)
		err = -EIO;

	return err ?: ia->write.out.size;
>>>>>>> upstream/android-13
}

bool fuse_write_update_size(struct inode *inode, loff_t pos)
{
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	bool ret = false;

<<<<<<< HEAD
	spin_lock(&fc->lock);
	fi->attr_version = ++fc->attr_version;
=======
	spin_lock(&fi->lock);
	fi->attr_version = atomic64_inc_return(&fc->attr_version);
>>>>>>> upstream/android-13
	if (pos > inode->i_size) {
		i_size_write(inode, pos);
		ret = true;
	}
<<<<<<< HEAD
	spin_unlock(&fc->lock);
=======
	spin_unlock(&fi->lock);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static size_t fuse_send_write_pages(struct fuse_req *req, struct kiocb *iocb,
				    struct inode *inode, loff_t pos,
				    size_t count)
{
	size_t res;
	unsigned offset;
	unsigned i;
	struct fuse_io_priv io = FUSE_IO_PRIV_SYNC(iocb);

	for (i = 0; i < req->num_pages; i++)
		fuse_wait_on_page_writeback(inode, req->pages[i]->index);

	res = fuse_send_write(req, &io, pos, count, NULL);

	offset = req->page_descs[0].offset;
	count = res;
	for (i = 0; i < req->num_pages; i++) {
		struct page *page = req->pages[i];

		if (!req->out.h.error && !offset && count >= PAGE_SIZE)
			SetPageUptodate(page);

		if (count > PAGE_SIZE - offset)
			count -= PAGE_SIZE - offset;
		else
			count = 0;
		offset = 0;

		unlock_page(page);
		put_page(page);
	}

	return res;
}

static ssize_t fuse_fill_write_pages(struct fuse_req *req,
			       struct address_space *mapping,
			       struct iov_iter *ii, loff_t pos)
{
=======
static ssize_t fuse_send_write_pages(struct fuse_io_args *ia,
				     struct kiocb *iocb, struct inode *inode,
				     loff_t pos, size_t count)
{
	struct fuse_args_pages *ap = &ia->ap;
	struct file *file = iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct fuse_mount *fm = ff->fm;
	unsigned int offset, i;
	bool short_write;
	int err;

	for (i = 0; i < ap->num_pages; i++)
		fuse_wait_on_page_writeback(inode, ap->pages[i]->index);

	fuse_write_args_fill(ia, ff, pos, count);
	ia->write.in.flags = fuse_write_flags(iocb);
	if (fm->fc->handle_killpriv_v2 && !capable(CAP_FSETID))
		ia->write.in.write_flags |= FUSE_WRITE_KILL_SUIDGID;

	err = fuse_simple_request(fm, &ap->args);
	if (!err && ia->write.out.size > count)
		err = -EIO;

	short_write = ia->write.out.size < count;
	offset = ap->descs[0].offset;
	count = ia->write.out.size;
	for (i = 0; i < ap->num_pages; i++) {
		struct page *page = ap->pages[i];

		if (err) {
			ClearPageUptodate(page);
		} else {
			if (count >= PAGE_SIZE - offset)
				count -= PAGE_SIZE - offset;
			else {
				if (short_write)
					ClearPageUptodate(page);
				count = 0;
			}
			offset = 0;
		}
		if (ia->write.page_locked && (i == ap->num_pages - 1))
			unlock_page(page);
		put_page(page);
	}

	return err;
}

static ssize_t fuse_fill_write_pages(struct fuse_io_args *ia,
				     struct address_space *mapping,
				     struct iov_iter *ii, loff_t pos,
				     unsigned int max_pages)
{
	struct fuse_args_pages *ap = &ia->ap;
>>>>>>> upstream/android-13
	struct fuse_conn *fc = get_fuse_conn(mapping->host);
	unsigned offset = pos & (PAGE_SIZE - 1);
	size_t count = 0;
	int err;

<<<<<<< HEAD
	req->in.argpages = 1;
	req->page_descs[0].offset = offset;
=======
	ap->args.in_pages = true;
	ap->descs[0].offset = offset;
>>>>>>> upstream/android-13

	do {
		size_t tmp;
		struct page *page;
		pgoff_t index = pos >> PAGE_SHIFT;
		size_t bytes = min_t(size_t, PAGE_SIZE - offset,
				     iov_iter_count(ii));

		bytes = min_t(size_t, bytes, fc->max_write - count);

 again:
		err = -EFAULT;
<<<<<<< HEAD
		if (iov_iter_fault_in_readable(ii, bytes))
=======
		if (fault_in_iov_iter_readable(ii, bytes))
>>>>>>> upstream/android-13
			break;

		err = -ENOMEM;
		page = grab_cache_page_write_begin(mapping, index, 0);
		if (!page)
			break;

		if (mapping_writably_mapped(mapping))
			flush_dcache_page(page);

<<<<<<< HEAD
		tmp = iov_iter_copy_from_user_atomic(page, ii, offset, bytes);
		flush_dcache_page(page);

		iov_iter_advance(ii, tmp);
		if (!tmp) {
			unlock_page(page);
			put_page(page);
			bytes = min(bytes, iov_iter_single_seg_count(ii));
=======
		tmp = copy_page_from_iter_atomic(page, offset, bytes, ii);
		flush_dcache_page(page);

		if (!tmp) {
			unlock_page(page);
			put_page(page);
>>>>>>> upstream/android-13
			goto again;
		}

		err = 0;
<<<<<<< HEAD
		req->pages[req->num_pages] = page;
		req->page_descs[req->num_pages].length = tmp;
		req->num_pages++;
=======
		ap->pages[ap->num_pages] = page;
		ap->descs[ap->num_pages].length = tmp;
		ap->num_pages++;
>>>>>>> upstream/android-13

		count += tmp;
		pos += tmp;
		offset += tmp;
		if (offset == PAGE_SIZE)
			offset = 0;

<<<<<<< HEAD
		if (!fc->big_writes)
			break;
	} while (iov_iter_count(ii) && count < fc->max_write &&
		 req->num_pages < req->max_pages && offset == 0);
=======
		/* If we copied full page, mark it uptodate */
		if (tmp == PAGE_SIZE)
			SetPageUptodate(page);

		if (PageUptodate(page)) {
			unlock_page(page);
		} else {
			ia->write.page_locked = true;
			break;
		}
		if (!fc->big_writes)
			break;
	} while (iov_iter_count(ii) && count < fc->max_write &&
		 ap->num_pages < max_pages && offset == 0);
>>>>>>> upstream/android-13

	return count > 0 ? count : err;
}

<<<<<<< HEAD
static inline unsigned fuse_wr_pages(loff_t pos, size_t len)
{
	return min_t(unsigned,
		     ((pos + len - 1) >> PAGE_SHIFT) -
		     (pos >> PAGE_SHIFT) + 1,
		     FUSE_MAX_PAGES_PER_REQ);
=======
static inline unsigned int fuse_wr_pages(loff_t pos, size_t len,
				     unsigned int max_pages)
{
	return min_t(unsigned int,
		     ((pos + len - 1) >> PAGE_SHIFT) -
		     (pos >> PAGE_SHIFT) + 1,
		     max_pages);
>>>>>>> upstream/android-13
}

static ssize_t fuse_perform_write(struct kiocb *iocb,
				  struct address_space *mapping,
				  struct iov_iter *ii, loff_t pos)
{
	struct inode *inode = mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	int err = 0;
	ssize_t res = 0;

<<<<<<< HEAD
	if (is_bad_inode(inode))
		return -EIO;

=======
>>>>>>> upstream/android-13
	if (inode->i_size < pos + iov_iter_count(ii))
		set_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);

	do {
<<<<<<< HEAD
		struct fuse_req *req;
		ssize_t count;
		unsigned nr_pages = fuse_wr_pages(pos, iov_iter_count(ii));

		req = fuse_get_req(fc, nr_pages);
		if (IS_ERR(req)) {
			err = PTR_ERR(req);
			break;
		}

		count = fuse_fill_write_pages(req, mapping, ii, pos);
		if (count <= 0) {
			err = count;
		} else {
			size_t num_written;

			mtk_btag_pidlog_set_pid_pages(req->pages,
						      req->num_pages,
						      PIDLOG_MODE_FS_FUSE,
						      true);

			num_written = fuse_send_write_pages(req, iocb, inode,
							    pos, count);
			err = req->out.h.error;
			if (!err) {
=======
		ssize_t count;
		struct fuse_io_args ia = {};
		struct fuse_args_pages *ap = &ia.ap;
		unsigned int nr_pages = fuse_wr_pages(pos, iov_iter_count(ii),
						      fc->max_pages);

		ap->pages = fuse_pages_alloc(nr_pages, GFP_KERNEL, &ap->descs);
		if (!ap->pages) {
			err = -ENOMEM;
			break;
		}

		count = fuse_fill_write_pages(&ia, mapping, ii, pos, nr_pages);
		if (count <= 0) {
			err = count;
		} else {
			err = fuse_send_write_pages(&ia, iocb, inode,
						    pos, count);
			if (!err) {
				size_t num_written = ia.write.out.size;

>>>>>>> upstream/android-13
				res += num_written;
				pos += num_written;

				/* break out of the loop on short write */
				if (num_written != count)
					err = -EIO;
			}
		}
<<<<<<< HEAD
		fuse_put_request(fc, req);
=======
		kfree(ap->pages);
>>>>>>> upstream/android-13
	} while (!err && iov_iter_count(ii));

	if (res > 0)
		fuse_write_update_size(inode, pos);

	clear_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);
	fuse_invalidate_attr(inode);

	return res > 0 ? res : err;
}

<<<<<<< HEAD
static ssize_t fuse_file_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	struct fuse_file *ff = file->private_data;
=======
static ssize_t fuse_cache_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
>>>>>>> upstream/android-13
	ssize_t written = 0;
	ssize_t written_buffered = 0;
	struct inode *inode = mapping->host;
	ssize_t err;
<<<<<<< HEAD
	loff_t endbyte = 0;

	if (ff->passthrough.filp)
		return fuse_passthrough_write_iter(iocb, from);

	if (get_fuse_conn(inode)->writeback_cache) {
=======
	struct fuse_conn *fc = get_fuse_conn(inode);
	loff_t endbyte = 0;

	if (fc->writeback_cache) {
>>>>>>> upstream/android-13
		/* Update size (EOF optimization) and mode (SUID clearing) */
		err = fuse_update_attributes(mapping->host, file);
		if (err)
			return err;

<<<<<<< HEAD
		return generic_file_write_iter(iocb, from);
	}

=======
		if (fc->handle_killpriv_v2 &&
		    should_remove_suid(file_dentry(file))) {
			goto writethrough;
		}

		return generic_file_write_iter(iocb, from);
	}

writethrough:
>>>>>>> upstream/android-13
	inode_lock(inode);

	/* We can write back this queue in page reclaim */
	current->backing_dev_info = inode_to_bdi(inode);

	err = generic_write_checks(iocb, from);
	if (err <= 0)
		goto out;

	err = file_remove_privs(file);
	if (err)
		goto out;

	err = file_update_time(file);
	if (err)
		goto out;

	if (iocb->ki_flags & IOCB_DIRECT) {
		loff_t pos = iocb->ki_pos;
		written = generic_file_direct_write(iocb, from);
		if (written < 0 || !iov_iter_count(from))
			goto out;

		pos += written;

		written_buffered = fuse_perform_write(iocb, mapping, from, pos);
		if (written_buffered < 0) {
			err = written_buffered;
			goto out;
		}
		endbyte = pos + written_buffered - 1;

		err = filemap_write_and_wait_range(file->f_mapping, pos,
						   endbyte);
		if (err)
			goto out;

		invalidate_mapping_pages(file->f_mapping,
					 pos >> PAGE_SHIFT,
					 endbyte >> PAGE_SHIFT);

		written += written_buffered;
		iocb->ki_pos = pos + written_buffered;
	} else {
		written = fuse_perform_write(iocb, mapping, from, iocb->ki_pos);
		if (written >= 0)
			iocb->ki_pos += written;
	}
out:
	current->backing_dev_info = NULL;
	inode_unlock(inode);
	if (written > 0)
		written = generic_write_sync(iocb, written);

	return written ? written : err;
}

<<<<<<< HEAD
static inline void fuse_page_descs_length_init(struct fuse_req *req,
		unsigned index, unsigned nr_pages)
{
	int i;

	for (i = index; i < index + nr_pages; i++)
		req->page_descs[i].length = PAGE_SIZE -
			req->page_descs[i].offset;
}

=======
>>>>>>> upstream/android-13
static inline unsigned long fuse_get_user_addr(const struct iov_iter *ii)
{
	return (unsigned long)ii->iov->iov_base + ii->iov_offset;
}

static inline size_t fuse_get_frag_size(const struct iov_iter *ii,
					size_t max_size)
{
	return min(iov_iter_single_seg_count(ii), max_size);
}

<<<<<<< HEAD
static int fuse_get_user_pages(struct fuse_req *req, struct iov_iter *ii,
			       size_t *nbytesp, int write)
=======
static int fuse_get_user_pages(struct fuse_args_pages *ap, struct iov_iter *ii,
			       size_t *nbytesp, int write,
			       unsigned int max_pages)
>>>>>>> upstream/android-13
{
	size_t nbytes = 0;  /* # bytes already packed in req */
	ssize_t ret = 0;

	/* Special case for kernel I/O: can copy directly into the buffer */
<<<<<<< HEAD
	if (ii->type & ITER_KVEC) {
=======
	if (iov_iter_is_kvec(ii)) {
>>>>>>> upstream/android-13
		unsigned long user_addr = fuse_get_user_addr(ii);
		size_t frag_size = fuse_get_frag_size(ii, *nbytesp);

		if (write)
<<<<<<< HEAD
			req->in.args[1].value = (void *) user_addr;
		else
			req->out.args[0].value = (void *) user_addr;
=======
			ap->args.in_args[1].value = (void *) user_addr;
		else
			ap->args.out_args[0].value = (void *) user_addr;
>>>>>>> upstream/android-13

		iov_iter_advance(ii, frag_size);
		*nbytesp = frag_size;
		return 0;
	}

<<<<<<< HEAD
	while (nbytes < *nbytesp && req->num_pages < req->max_pages) {
		unsigned npages;
		size_t start;
		ret = iov_iter_get_pages(ii, &req->pages[req->num_pages],
					*nbytesp - nbytes,
					req->max_pages - req->num_pages,
=======
	while (nbytes < *nbytesp && ap->num_pages < max_pages) {
		unsigned npages;
		size_t start;
		ret = iov_iter_get_pages(ii, &ap->pages[ap->num_pages],
					*nbytesp - nbytes,
					max_pages - ap->num_pages,
>>>>>>> upstream/android-13
					&start);
		if (ret < 0)
			break;

		iov_iter_advance(ii, ret);
		nbytes += ret;

		ret += start;
<<<<<<< HEAD
		npages = (ret + PAGE_SIZE - 1) / PAGE_SIZE;

		req->page_descs[req->num_pages].offset = start;
		fuse_page_descs_length_init(req, req->num_pages, npages);

		req->num_pages += npages;
		req->page_descs[req->num_pages - 1].length -=
			(PAGE_SIZE - ret) & (PAGE_SIZE - 1);
	}

	if (write)
		req->in.argpages = 1;
	else
		req->out.argpages = 1;
=======
		npages = DIV_ROUND_UP(ret, PAGE_SIZE);

		ap->descs[ap->num_pages].offset = start;
		fuse_page_descs_length_init(ap->descs, ap->num_pages, npages);

		ap->num_pages += npages;
		ap->descs[ap->num_pages - 1].length -=
			(PAGE_SIZE - ret) & (PAGE_SIZE - 1);
	}

	ap->args.user_pages = true;
	if (write)
		ap->args.in_pages = true;
	else
		ap->args.out_pages = true;
>>>>>>> upstream/android-13

	*nbytesp = nbytes;

	return ret < 0 ? ret : 0;
}

<<<<<<< HEAD
static inline int fuse_iter_npages(const struct iov_iter *ii_p)
{
	return iov_iter_npages(ii_p, FUSE_MAX_PAGES_PER_REQ);
}

=======
>>>>>>> upstream/android-13
ssize_t fuse_direct_io(struct fuse_io_priv *io, struct iov_iter *iter,
		       loff_t *ppos, int flags)
{
	int write = flags & FUSE_DIO_WRITE;
	int cuse = flags & FUSE_DIO_CUSE;
	struct file *file = io->iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	struct fuse_file *ff = file->private_data;
<<<<<<< HEAD
	struct fuse_conn *fc = ff->fc;
=======
	struct fuse_conn *fc = ff->fm->fc;
>>>>>>> upstream/android-13
	size_t nmax = write ? fc->max_write : fc->max_read;
	loff_t pos = *ppos;
	size_t count = iov_iter_count(iter);
	pgoff_t idx_from = pos >> PAGE_SHIFT;
	pgoff_t idx_to = (pos + count - 1) >> PAGE_SHIFT;
	ssize_t res = 0;
<<<<<<< HEAD
	struct fuse_req *req;
	int err = 0;

	if (io->async)
		req = fuse_get_req_for_background(fc, fuse_iter_npages(iter));
	else
		req = fuse_get_req(fc, fuse_iter_npages(iter));
	if (IS_ERR(req))
		return PTR_ERR(req);

=======
	int err = 0;
	struct fuse_io_args *ia;
	unsigned int max_pages;

	max_pages = iov_iter_npages(iter, fc->max_pages);
	ia = fuse_io_alloc(io, max_pages);
	if (!ia)
		return -ENOMEM;

	ia->io = io;
>>>>>>> upstream/android-13
	if (!cuse && fuse_range_is_writeback(inode, idx_from, idx_to)) {
		if (!write)
			inode_lock(inode);
		fuse_sync_writes(inode);
		if (!write)
			inode_unlock(inode);
	}

	io->should_dirty = !write && iter_is_iovec(iter);
	while (count) {
<<<<<<< HEAD
		size_t nres;
		fl_owner_t owner = current->files;
		size_t nbytes = min(count, nmax);
		err = fuse_get_user_pages(req, iter, &nbytes, write);
		if (err && !nbytes)
			break;

		if (write)
			nres = fuse_send_write(req, io, pos, nbytes, owner);
		else
			nres = fuse_send_read(req, io, pos, nbytes, owner);

		if (!io->async)
			fuse_release_user_pages(req, io->should_dirty);
		if (req->out.h.error) {
			err = req->out.h.error;
			break;
		} else if (nres > nbytes) {
			res = 0;
			err = -EIO;
			break;
		}
		count -= nres;
		res += nres;
		pos += nres;
		if (nres != nbytes)
			break;
		if (count) {
			fuse_put_request(fc, req);
			if (io->async)
				req = fuse_get_req_for_background(fc,
					fuse_iter_npages(iter));
			else
				req = fuse_get_req(fc, fuse_iter_npages(iter));
			if (IS_ERR(req))
				break;
		}
	}
	if (!IS_ERR(req))
		fuse_put_request(fc, req);
=======
		ssize_t nres;
		fl_owner_t owner = current->files;
		size_t nbytes = min(count, nmax);

		err = fuse_get_user_pages(&ia->ap, iter, &nbytes, write,
					  max_pages);
		if (err && !nbytes)
			break;

		if (write) {
			if (!capable(CAP_FSETID))
				ia->write.in.write_flags |= FUSE_WRITE_KILL_SUIDGID;

			nres = fuse_send_write(ia, pos, nbytes, owner);
		} else {
			nres = fuse_send_read(ia, pos, nbytes, owner);
		}

		if (!io->async || nres < 0) {
			fuse_release_user_pages(&ia->ap, io->should_dirty);
			fuse_io_free(ia);
		}
		ia = NULL;
		if (nres < 0) {
			iov_iter_revert(iter, nbytes);
			err = nres;
			break;
		}
		WARN_ON(nres > nbytes);

		count -= nres;
		res += nres;
		pos += nres;
		if (nres != nbytes) {
			iov_iter_revert(iter, nbytes - nres);
			break;
		}
		if (count) {
			max_pages = iov_iter_npages(iter, fc->max_pages);
			ia = fuse_io_alloc(io, max_pages);
			if (!ia)
				break;
		}
	}
	if (ia)
		fuse_io_free(ia);
>>>>>>> upstream/android-13
	if (res > 0)
		*ppos = pos;

	return res > 0 ? res : err;
}
EXPORT_SYMBOL_GPL(fuse_direct_io);

static ssize_t __fuse_direct_read(struct fuse_io_priv *io,
				  struct iov_iter *iter,
				  loff_t *ppos)
{
	ssize_t res;
	struct inode *inode = file_inode(io->iocb->ki_filp);
<<<<<<< HEAD
	struct fuse_file *ff = io->iocb->ki_filp->private_data;

	if (is_bad_inode(inode))
		return -EIO;

	if (ff->passthrough.filp)
		return fuse_passthrough_read_iter(io->iocb, iter);

	res = fuse_direct_io(io, iter, ppos, 0);

	fuse_invalidate_attr(inode);
=======

	res = fuse_direct_io(io, iter, ppos, 0);

	fuse_invalidate_atime(inode);
>>>>>>> upstream/android-13

	return res;
}

<<<<<<< HEAD
static ssize_t fuse_direct_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct fuse_io_priv io = FUSE_IO_PRIV_SYNC(iocb);
	return __fuse_direct_read(&io, to, &iocb->ki_pos);
=======
static ssize_t fuse_direct_IO(struct kiocb *iocb, struct iov_iter *iter);

static ssize_t fuse_direct_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	ssize_t res;

	if (!is_sync_kiocb(iocb) && iocb->ki_flags & IOCB_DIRECT) {
		res = fuse_direct_IO(iocb, to);
	} else {
		struct fuse_io_priv io = FUSE_IO_PRIV_SYNC(iocb);

		res = __fuse_direct_read(&io, to, &iocb->ki_pos);
	}

	return res;
>>>>>>> upstream/android-13
}

static ssize_t fuse_direct_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	struct fuse_io_priv io = FUSE_IO_PRIV_SYNC(iocb);
<<<<<<< HEAD
	struct fuse_file *ff = iocb->ki_filp->private_data;
	ssize_t res;

	if (is_bad_inode(inode))
		return -EIO;

	if (ff->passthrough.filp)
		return fuse_passthrough_write_iter(iocb, from);

	/* Don't allow parallel writes to the same file */
	inode_lock(inode);
	res = generic_write_checks(iocb, from);
	if (res > 0)
		res = fuse_direct_io(&io, from, &iocb->ki_pos, FUSE_DIO_WRITE);
=======
	ssize_t res;

	/* Don't allow parallel writes to the same file */
	inode_lock(inode);
	res = generic_write_checks(iocb, from);
	if (res > 0) {
		if (!is_sync_kiocb(iocb) && iocb->ki_flags & IOCB_DIRECT) {
			res = fuse_direct_IO(iocb, from);
		} else {
			res = fuse_direct_io(&io, from, &iocb->ki_pos,
					     FUSE_DIO_WRITE);
		}
	}
>>>>>>> upstream/android-13
	fuse_invalidate_attr(inode);
	if (res > 0)
		fuse_write_update_size(inode, iocb->ki_pos);
	inode_unlock(inode);

	return res;
}

<<<<<<< HEAD
static void fuse_writepage_free(struct fuse_conn *fc, struct fuse_req *req)
{
	int i;

	for (i = 0; i < req->num_pages; i++)
		__free_page(req->pages[i]);

	if (req->ff)
		fuse_file_put(req->ff, false, false);
}

static void fuse_writepage_finish(struct fuse_conn *fc, struct fuse_req *req)
{
	struct inode *inode = req->inode;
=======
static ssize_t fuse_file_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct file *file = iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct inode *inode = file_inode(file);

	if (fuse_is_bad(inode))
		return -EIO;

	if (FUSE_IS_DAX(inode))
		return fuse_dax_read_iter(iocb, to);

	if (ff->passthrough.filp)
		return fuse_passthrough_read_iter(iocb, to);
	else if (!(ff->open_flags & FOPEN_DIRECT_IO))
		return fuse_cache_read_iter(iocb, to);
	else
		return fuse_direct_read_iter(iocb, to);
}

static ssize_t fuse_file_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
	struct inode *inode = file_inode(file);

	if (fuse_is_bad(inode))
		return -EIO;

	if (FUSE_IS_DAX(inode))
		return fuse_dax_write_iter(iocb, from);

	if (ff->passthrough.filp)
		return fuse_passthrough_write_iter(iocb, from);
	else if (!(ff->open_flags & FOPEN_DIRECT_IO))
		return fuse_cache_write_iter(iocb, from);
	else
		return fuse_direct_write_iter(iocb, from);
}

static void fuse_writepage_free(struct fuse_writepage_args *wpa)
{
	struct fuse_args_pages *ap = &wpa->ia.ap;
	int i;

	if (wpa->bucket)
		fuse_sync_bucket_dec(wpa->bucket);

	for (i = 0; i < ap->num_pages; i++)
		__free_page(ap->pages[i]);

	if (wpa->ia.ff)
		fuse_file_put(wpa->ia.ff, false, false);

	kfree(ap->pages);
	kfree(wpa);
}

static void fuse_writepage_finish(struct fuse_mount *fm,
				  struct fuse_writepage_args *wpa)
{
	struct fuse_args_pages *ap = &wpa->ia.ap;
	struct inode *inode = wpa->inode;
>>>>>>> upstream/android-13
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct backing_dev_info *bdi = inode_to_bdi(inode);
	int i;

<<<<<<< HEAD
	list_del(&req->writepages_entry);
	for (i = 0; i < req->num_pages; i++) {
		dec_wb_stat(&bdi->wb, WB_WRITEBACK);
		dec_node_page_state(req->pages[i], NR_WRITEBACK_TEMP);
=======
	for (i = 0; i < ap->num_pages; i++) {
		dec_wb_stat(&bdi->wb, WB_WRITEBACK);
		dec_node_page_state(ap->pages[i], NR_WRITEBACK_TEMP);
>>>>>>> upstream/android-13
		wb_writeout_inc(&bdi->wb);
	}
	wake_up(&fi->page_waitq);
}

<<<<<<< HEAD
/* Called under fc->lock, may release and reacquire it */
static void fuse_send_writepage(struct fuse_conn *fc, struct fuse_req *req,
				loff_t size)
__releases(fc->lock)
__acquires(fc->lock)
{
	struct fuse_inode *fi = get_fuse_inode(req->inode);
	struct fuse_write_in *inarg = &req->misc.write.in;
	__u64 data_size = req->num_pages * PAGE_SIZE;

	if (!fc->connected)
		goto out_free;

=======
/* Called under fi->lock, may release and reacquire it */
static void fuse_send_writepage(struct fuse_mount *fm,
				struct fuse_writepage_args *wpa, loff_t size)
__releases(fi->lock)
__acquires(fi->lock)
{
	struct fuse_writepage_args *aux, *next;
	struct fuse_inode *fi = get_fuse_inode(wpa->inode);
	struct fuse_write_in *inarg = &wpa->ia.write.in;
	struct fuse_args *args = &wpa->ia.ap.args;
	__u64 data_size = wpa->ia.ap.num_pages * PAGE_SIZE;
	int err;

	fi->writectr++;
>>>>>>> upstream/android-13
	if (inarg->offset + data_size <= size) {
		inarg->size = data_size;
	} else if (inarg->offset < size) {
		inarg->size = size - inarg->offset;
	} else {
		/* Got truncated off completely */
		goto out_free;
	}

<<<<<<< HEAD
	req->in.args[1].size = inarg->size;
	fi->writectr++;
	fuse_request_send_background_locked(fc, req);
	return;

 out_free:
	fuse_writepage_finish(fc, req);
	spin_unlock(&fc->lock);
	fuse_writepage_free(fc, req);
	fuse_put_request(fc, req);
	spin_lock(&fc->lock);
=======
	args->in_args[1].size = inarg->size;
	args->force = true;
	args->nocreds = true;

	err = fuse_simple_background(fm, args, GFP_ATOMIC);
	if (err == -ENOMEM) {
		spin_unlock(&fi->lock);
		err = fuse_simple_background(fm, args, GFP_NOFS | __GFP_NOFAIL);
		spin_lock(&fi->lock);
	}

	/* Fails on broken connection only */
	if (unlikely(err))
		goto out_free;

	return;

 out_free:
	fi->writectr--;
	rb_erase(&wpa->writepages_entry, &fi->writepages);
	fuse_writepage_finish(fm, wpa);
	spin_unlock(&fi->lock);

	/* After fuse_writepage_finish() aux request list is private */
	for (aux = wpa->next; aux; aux = next) {
		next = aux->next;
		aux->next = NULL;
		fuse_writepage_free(aux);
	}

	fuse_writepage_free(wpa);
	spin_lock(&fi->lock);
>>>>>>> upstream/android-13
}

/*
 * If fi->writectr is positive (no truncate or fsync going on) send
 * all queued writepage requests.
 *
<<<<<<< HEAD
 * Called with fc->lock
 */
void fuse_flush_writepages(struct inode *inode)
__releases(fc->lock)
__acquires(fc->lock)
{
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	loff_t crop = i_size_read(inode);
	struct fuse_req *req;

	while (fi->writectr >= 0 && !list_empty(&fi->queued_writes)) {
		req = list_entry(fi->queued_writes.next, struct fuse_req, list);
		list_del_init(&req->list);
		fuse_send_writepage(fc, req, crop);
	}
}

static void fuse_writepage_end(struct fuse_conn *fc, struct fuse_req *req)
{
	struct inode *inode = req->inode;
	struct fuse_inode *fi = get_fuse_inode(inode);

	mapping_set_error(inode->i_mapping, req->out.h.error);
	spin_lock(&fc->lock);
	while (req->misc.write.next) {
		struct fuse_conn *fc = get_fuse_conn(inode);
		struct fuse_write_in *inarg = &req->misc.write.in;
		struct fuse_req *next = req->misc.write.next;
		req->misc.write.next = next->misc.write.next;
		next->misc.write.next = NULL;
		next->ff = fuse_file_get(req->ff);
		list_add(&next->writepages_entry, &fi->writepages);
=======
 * Called with fi->lock
 */
void fuse_flush_writepages(struct inode *inode)
__releases(fi->lock)
__acquires(fi->lock)
{
	struct fuse_mount *fm = get_fuse_mount(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	loff_t crop = i_size_read(inode);
	struct fuse_writepage_args *wpa;

	while (fi->writectr >= 0 && !list_empty(&fi->queued_writes)) {
		wpa = list_entry(fi->queued_writes.next,
				 struct fuse_writepage_args, queue_entry);
		list_del_init(&wpa->queue_entry);
		fuse_send_writepage(fm, wpa, crop);
	}
}

static struct fuse_writepage_args *fuse_insert_writeback(struct rb_root *root,
						struct fuse_writepage_args *wpa)
{
	pgoff_t idx_from = wpa->ia.write.in.offset >> PAGE_SHIFT;
	pgoff_t idx_to = idx_from + wpa->ia.ap.num_pages - 1;
	struct rb_node **p = &root->rb_node;
	struct rb_node  *parent = NULL;

	WARN_ON(!wpa->ia.ap.num_pages);
	while (*p) {
		struct fuse_writepage_args *curr;
		pgoff_t curr_index;

		parent = *p;
		curr = rb_entry(parent, struct fuse_writepage_args,
				writepages_entry);
		WARN_ON(curr->inode != wpa->inode);
		curr_index = curr->ia.write.in.offset >> PAGE_SHIFT;

		if (idx_from >= curr_index + curr->ia.ap.num_pages)
			p = &(*p)->rb_right;
		else if (idx_to < curr_index)
			p = &(*p)->rb_left;
		else
			return curr;
	}

	rb_link_node(&wpa->writepages_entry, parent, p);
	rb_insert_color(&wpa->writepages_entry, root);
	return NULL;
}

static void tree_insert(struct rb_root *root, struct fuse_writepage_args *wpa)
{
	WARN_ON(fuse_insert_writeback(root, wpa));
}

static void fuse_writepage_end(struct fuse_mount *fm, struct fuse_args *args,
			       int error)
{
	struct fuse_writepage_args *wpa =
		container_of(args, typeof(*wpa), ia.ap.args);
	struct inode *inode = wpa->inode;
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_conn *fc = get_fuse_conn(inode);

	mapping_set_error(inode->i_mapping, error);
	/*
	 * A writeback finished and this might have updated mtime/ctime on
	 * server making local mtime/ctime stale.  Hence invalidate attrs.
	 * Do this only if writeback_cache is not enabled.  If writeback_cache
	 * is enabled, we trust local ctime/mtime.
	 */
	if (!fc->writeback_cache)
		fuse_invalidate_attr(inode);
	spin_lock(&fi->lock);
	rb_erase(&wpa->writepages_entry, &fi->writepages);
	while (wpa->next) {
		struct fuse_mount *fm = get_fuse_mount(inode);
		struct fuse_write_in *inarg = &wpa->ia.write.in;
		struct fuse_writepage_args *next = wpa->next;

		wpa->next = next->next;
		next->next = NULL;
		next->ia.ff = fuse_file_get(wpa->ia.ff);
		tree_insert(&fi->writepages, next);
>>>>>>> upstream/android-13

		/*
		 * Skip fuse_flush_writepages() to make it easy to crop requests
		 * based on primary request size.
		 *
		 * 1st case (trivial): there are no concurrent activities using
		 * fuse_set/release_nowrite.  Then we're on safe side because
		 * fuse_flush_writepages() would call fuse_send_writepage()
		 * anyway.
		 *
		 * 2nd case: someone called fuse_set_nowrite and it is waiting
		 * now for completion of all in-flight requests.  This happens
		 * rarely and no more than once per page, so this should be
		 * okay.
		 *
		 * 3rd case: someone (e.g. fuse_do_setattr()) is in the middle
		 * of fuse_set_nowrite..fuse_release_nowrite section.  The fact
		 * that fuse_set_nowrite returned implies that all in-flight
		 * requests were completed along with all of their secondary
		 * requests.  Further primary requests are blocked by negative
		 * writectr.  Hence there cannot be any in-flight requests and
		 * no invocations of fuse_writepage_end() while we're in
		 * fuse_set_nowrite..fuse_release_nowrite section.
		 */
<<<<<<< HEAD
		fuse_send_writepage(fc, next, inarg->offset + inarg->size);
	}
	fi->writectr--;
	fuse_writepage_finish(fc, req);
	spin_unlock(&fc->lock);
	fuse_writepage_free(fc, req);
}

static struct fuse_file *__fuse_write_file_get(struct fuse_conn *fc,
					       struct fuse_inode *fi)
{
	struct fuse_file *ff = NULL;

	spin_lock(&fc->lock);
=======
		fuse_send_writepage(fm, next, inarg->offset + inarg->size);
	}
	fi->writectr--;
	fuse_writepage_finish(fm, wpa);
	spin_unlock(&fi->lock);
	fuse_writepage_free(wpa);
}

static struct fuse_file *__fuse_write_file_get(struct fuse_inode *fi)
{
	struct fuse_file *ff = NULL;

	spin_lock(&fi->lock);
>>>>>>> upstream/android-13
	if (!list_empty(&fi->write_files)) {
		ff = list_entry(fi->write_files.next, struct fuse_file,
				write_entry);
		fuse_file_get(ff);
	}
<<<<<<< HEAD
	spin_unlock(&fc->lock);
=======
	spin_unlock(&fi->lock);
>>>>>>> upstream/android-13

	return ff;
}

<<<<<<< HEAD
static struct fuse_file *fuse_write_file_get(struct fuse_conn *fc,
					     struct fuse_inode *fi)
{
	struct fuse_file *ff = __fuse_write_file_get(fc, fi);
=======
static struct fuse_file *fuse_write_file_get(struct fuse_inode *fi)
{
	struct fuse_file *ff = __fuse_write_file_get(fi);
>>>>>>> upstream/android-13
	WARN_ON(!ff);
	return ff;
}

int fuse_write_inode(struct inode *inode, struct writeback_control *wbc)
{
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
>>>>>>> upstream/android-13
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_file *ff;
	int err;
	/* @fs.sec -- E8B3F75DDB82DFE8F52508F039ABE4FF -- */
	unsigned int nofs_flag = memalloc_nofs_save();

<<<<<<< HEAD
	ff = __fuse_write_file_get(fc, fi);
=======
	/*
	 * Inode is always written before the last reference is dropped and
	 * hence this should not be reached from reclaim.
	 *
	 * Writing back the inode from reclaim can deadlock if the request
	 * processing itself needs an allocation.  Allocations triggering
	 * reclaim while serving a request can't be prevented, because it can
	 * involve any number of unrelated userspace processes.
	 */
	WARN_ON(wbc->for_reclaim);

	ff = __fuse_write_file_get(fi);
>>>>>>> upstream/android-13
	err = fuse_flush_times(inode, ff);
	if (ff)
		fuse_file_put(ff, false, false);

	memalloc_nofs_restore(nofs_flag);
	return err;
}

<<<<<<< HEAD
=======
static struct fuse_writepage_args *fuse_writepage_args_alloc(void)
{
	struct fuse_writepage_args *wpa;
	struct fuse_args_pages *ap;

	wpa = kzalloc(sizeof(*wpa), GFP_NOFS);
	if (wpa) {
		ap = &wpa->ia.ap;
		ap->num_pages = 0;
		ap->pages = fuse_pages_alloc(1, GFP_NOFS, &ap->descs);
		if (!ap->pages) {
			kfree(wpa);
			wpa = NULL;
		}
	}
	return wpa;

}

static void fuse_writepage_add_to_bucket(struct fuse_conn *fc,
					 struct fuse_writepage_args *wpa)
{
	if (!fc->sync_fs)
		return;

	rcu_read_lock();
	/* Prevent resurrection of dead bucket in unlikely race with syncfs */
	do {
		wpa->bucket = rcu_dereference(fc->curr_bucket);
	} while (unlikely(!atomic_inc_not_zero(&wpa->bucket->count)));
	rcu_read_unlock();
}

>>>>>>> upstream/android-13
static int fuse_writepage_locked(struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode = mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
<<<<<<< HEAD
	struct fuse_req *req;
=======
	struct fuse_writepage_args *wpa;
	struct fuse_args_pages *ap;
>>>>>>> upstream/android-13
	struct page *tmp_page;
	int error = -ENOMEM;

	set_page_writeback(page);

<<<<<<< HEAD
	req = fuse_request_alloc_nofs(1);
	if (!req)
		goto err;

	/* writeback always goes to bg_queue */
	__set_bit(FR_BACKGROUND, &req->flags);
=======
	wpa = fuse_writepage_args_alloc();
	if (!wpa)
		goto err;
	ap = &wpa->ia.ap;

>>>>>>> upstream/android-13
	tmp_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
	if (!tmp_page)
		goto err_free;

	error = -EIO;
<<<<<<< HEAD
	req->ff = fuse_write_file_get(fc, fi);
	if (!req->ff)
		goto err_nofile;

	fuse_write_fill(req, req->ff, page_offset(page), 0);

	copy_highpage(tmp_page, page);
	req->misc.write.in.write_flags |= FUSE_WRITE_CACHE;
	req->misc.write.next = NULL;
	req->in.argpages = 1;
	req->num_pages = 1;
	req->pages[0] = tmp_page;
	req->page_descs[0].offset = 0;
	req->page_descs[0].length = PAGE_SIZE;
	req->end = fuse_writepage_end;
	req->inode = inode;
=======
	wpa->ia.ff = fuse_write_file_get(fi);
	if (!wpa->ia.ff)
		goto err_nofile;

	fuse_writepage_add_to_bucket(fc, wpa);
	fuse_write_args_fill(&wpa->ia, wpa->ia.ff, page_offset(page), 0);

	copy_highpage(tmp_page, page);
	wpa->ia.write.in.write_flags |= FUSE_WRITE_CACHE;
	wpa->next = NULL;
	ap->args.in_pages = true;
	ap->num_pages = 1;
	ap->pages[0] = tmp_page;
	ap->descs[0].offset = 0;
	ap->descs[0].length = PAGE_SIZE;
	ap->args.end = fuse_writepage_end;
	wpa->inode = inode;
>>>>>>> upstream/android-13

	inc_wb_stat(&inode_to_bdi(inode)->wb, WB_WRITEBACK);
	inc_node_page_state(tmp_page, NR_WRITEBACK_TEMP);

<<<<<<< HEAD
	spin_lock(&fc->lock);
	list_add(&req->writepages_entry, &fi->writepages);
	list_add_tail(&req->list, &fi->queued_writes);
	fuse_flush_writepages(inode);
	spin_unlock(&fc->lock);
=======
	spin_lock(&fi->lock);
	tree_insert(&fi->writepages, wpa);
	list_add_tail(&wpa->queue_entry, &fi->queued_writes);
	fuse_flush_writepages(inode);
	spin_unlock(&fi->lock);
>>>>>>> upstream/android-13

	end_page_writeback(page);

	return 0;

err_nofile:
	__free_page(tmp_page);
err_free:
<<<<<<< HEAD
	fuse_request_free(req);
=======
	kfree(wpa);
>>>>>>> upstream/android-13
err:
	mapping_set_error(page->mapping, error);
	end_page_writeback(page);
	return error;
}

static int fuse_writepage(struct page *page, struct writeback_control *wbc)
{
	int err;

	if (fuse_page_is_writeback(page->mapping->host, page->index)) {
		/*
		 * ->writepages() should be called for sync() and friends.  We
		 * should only get here on direct reclaim and then we are
		 * allowed to skip a page which is already in flight
		 */
		WARN_ON(wbc->sync_mode == WB_SYNC_ALL);

		redirty_page_for_writepage(wbc, page);
		unlock_page(page);
		return 0;
	}

	err = fuse_writepage_locked(page);
	unlock_page(page);

	return err;
}

struct fuse_fill_wb_data {
<<<<<<< HEAD
	struct fuse_req *req;
	struct fuse_file *ff;
	struct inode *inode;
	struct page **orig_pages;
};

static void fuse_writepages_send(struct fuse_fill_wb_data *data)
{
	struct fuse_req *req = data->req;
	struct inode *inode = data->inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	int num_pages = req->num_pages;
	int i;

	req->ff = fuse_file_get(data->ff);
	spin_lock(&fc->lock);
	list_add_tail(&req->list, &fi->queued_writes);
	fuse_flush_writepages(inode);
	spin_unlock(&fc->lock);
=======
	struct fuse_writepage_args *wpa;
	struct fuse_file *ff;
	struct inode *inode;
	struct page **orig_pages;
	unsigned int max_pages;
};

static bool fuse_pages_realloc(struct fuse_fill_wb_data *data)
{
	struct fuse_args_pages *ap = &data->wpa->ia.ap;
	struct fuse_conn *fc = get_fuse_conn(data->inode);
	struct page **pages;
	struct fuse_page_desc *descs;
	unsigned int npages = min_t(unsigned int,
				    max_t(unsigned int, data->max_pages * 2,
					  FUSE_DEFAULT_MAX_PAGES_PER_REQ),
				    fc->max_pages);
	WARN_ON(npages <= data->max_pages);

	pages = fuse_pages_alloc(npages, GFP_NOFS, &descs);
	if (!pages)
		return false;

	memcpy(pages, ap->pages, sizeof(struct page *) * ap->num_pages);
	memcpy(descs, ap->descs, sizeof(struct fuse_page_desc) * ap->num_pages);
	kfree(ap->pages);
	ap->pages = pages;
	ap->descs = descs;
	data->max_pages = npages;

	return true;
}

static void fuse_writepages_send(struct fuse_fill_wb_data *data)
{
	struct fuse_writepage_args *wpa = data->wpa;
	struct inode *inode = data->inode;
	struct fuse_inode *fi = get_fuse_inode(inode);
	int num_pages = wpa->ia.ap.num_pages;
	int i;

	wpa->ia.ff = fuse_file_get(data->ff);
	spin_lock(&fi->lock);
	list_add_tail(&wpa->queue_entry, &fi->queued_writes);
	fuse_flush_writepages(inode);
	spin_unlock(&fi->lock);
>>>>>>> upstream/android-13

	for (i = 0; i < num_pages; i++)
		end_page_writeback(data->orig_pages[i]);
}

<<<<<<< HEAD
static bool fuse_writepage_in_flight(struct fuse_req *new_req,
				     struct page *page)
{
	struct fuse_conn *fc = get_fuse_conn(new_req->inode);
	struct fuse_inode *fi = get_fuse_inode(new_req->inode);
	struct fuse_req *tmp;
	struct fuse_req *old_req;
	bool found = false;
	pgoff_t curr_index;

	BUG_ON(new_req->num_pages != 0);

	spin_lock(&fc->lock);
	list_del(&new_req->writepages_entry);
	list_for_each_entry(old_req, &fi->writepages, writepages_entry) {
		BUG_ON(old_req->inode != new_req->inode);
		curr_index = old_req->misc.write.in.offset >> PAGE_SHIFT;
		if (curr_index <= page->index &&
		    page->index < curr_index + old_req->num_pages) {
			found = true;
			break;
		}
	}
	if (!found) {
		list_add(&new_req->writepages_entry, &fi->writepages);
		goto out_unlock;
	}

	new_req->num_pages = 1;
	for (tmp = old_req; tmp != NULL; tmp = tmp->misc.write.next) {
		BUG_ON(tmp->inode != new_req->inode);
		curr_index = tmp->misc.write.in.offset >> PAGE_SHIFT;
		if (tmp->num_pages == 1 &&
		    curr_index == page->index) {
			old_req = tmp;
		}
	}

	if (old_req->num_pages == 1 && test_bit(FR_PENDING, &old_req->flags)) {
		struct backing_dev_info *bdi = inode_to_bdi(page->mapping->host);

		copy_highpage(old_req->pages[0], page);
		spin_unlock(&fc->lock);

		dec_wb_stat(&bdi->wb, WB_WRITEBACK);
		dec_node_page_state(new_req->pages[0], NR_WRITEBACK_TEMP);
		wb_writeout_inc(&bdi->wb);
		fuse_writepage_free(fc, new_req);
		fuse_request_free(new_req);
		goto out;
	} else {
		new_req->misc.write.next = old_req->misc.write.next;
		old_req->misc.write.next = new_req;
	}
out_unlock:
	spin_unlock(&fc->lock);
out:
	return found;
}

static int fuse_writepages_fill(struct page *page,
		struct writeback_control *wbc, void *_data)
{
	struct fuse_fill_wb_data *data = _data;
	struct fuse_req *req = data->req;
	struct inode *inode = data->inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct page *tmp_page;
	bool is_writeback;
	int err;

	if (!data->ff) {
		err = -EIO;
		data->ff = fuse_write_file_get(fc, get_fuse_inode(inode));
		if (!data->ff)
			goto out_unlock;
	}
=======
/*
 * Check under fi->lock if the page is under writeback, and insert it onto the
 * rb_tree if not. Otherwise iterate auxiliary write requests, to see if there's
 * one already added for a page at this offset.  If there's none, then insert
 * this new request onto the auxiliary list, otherwise reuse the existing one by
 * swapping the new temp page with the old one.
 */
static bool fuse_writepage_add(struct fuse_writepage_args *new_wpa,
			       struct page *page)
{
	struct fuse_inode *fi = get_fuse_inode(new_wpa->inode);
	struct fuse_writepage_args *tmp;
	struct fuse_writepage_args *old_wpa;
	struct fuse_args_pages *new_ap = &new_wpa->ia.ap;

	WARN_ON(new_ap->num_pages != 0);
	new_ap->num_pages = 1;

	spin_lock(&fi->lock);
	old_wpa = fuse_insert_writeback(&fi->writepages, new_wpa);
	if (!old_wpa) {
		spin_unlock(&fi->lock);
		return true;
	}

	for (tmp = old_wpa->next; tmp; tmp = tmp->next) {
		pgoff_t curr_index;

		WARN_ON(tmp->inode != new_wpa->inode);
		curr_index = tmp->ia.write.in.offset >> PAGE_SHIFT;
		if (curr_index == page->index) {
			WARN_ON(tmp->ia.ap.num_pages != 1);
			swap(tmp->ia.ap.pages[0], new_ap->pages[0]);
			break;
		}
	}

	if (!tmp) {
		new_wpa->next = old_wpa->next;
		old_wpa->next = new_wpa;
	}

	spin_unlock(&fi->lock);

	if (tmp) {
		struct backing_dev_info *bdi = inode_to_bdi(new_wpa->inode);

		dec_wb_stat(&bdi->wb, WB_WRITEBACK);
		dec_node_page_state(new_ap->pages[0], NR_WRITEBACK_TEMP);
		wb_writeout_inc(&bdi->wb);
		fuse_writepage_free(new_wpa);
	}

	return false;
}

static bool fuse_writepage_need_send(struct fuse_conn *fc, struct page *page,
				     struct fuse_args_pages *ap,
				     struct fuse_fill_wb_data *data)
{
	WARN_ON(!ap->num_pages);
>>>>>>> upstream/android-13

	/*
	 * Being under writeback is unlikely but possible.  For example direct
	 * read to an mmaped fuse file will set the page dirty twice; once when
	 * the pages are faulted with get_user_pages(), and then after the read
	 * completed.
	 */
<<<<<<< HEAD
	is_writeback = fuse_page_is_writeback(inode, page->index);

	if (req && req->num_pages &&
	    (is_writeback || req->num_pages == FUSE_MAX_PAGES_PER_REQ ||
	     (req->num_pages + 1) * PAGE_SIZE > fc->max_write ||
	     data->orig_pages[req->num_pages - 1]->index + 1 != page->index)) {
		fuse_writepages_send(data);
		data->req = NULL;
	}
=======
	if (fuse_page_is_writeback(data->inode, page->index))
		return true;

	/* Reached max pages */
	if (ap->num_pages == fc->max_pages)
		return true;

	/* Reached max write bytes */
	if ((ap->num_pages + 1) * PAGE_SIZE > fc->max_write)
		return true;

	/* Discontinuity */
	if (data->orig_pages[ap->num_pages - 1]->index + 1 != page->index)
		return true;

	/* Need to grow the pages array?  If so, did the expansion fail? */
	if (ap->num_pages == data->max_pages && !fuse_pages_realloc(data))
		return true;

	return false;
}

static int fuse_writepages_fill(struct page *page,
		struct writeback_control *wbc, void *_data)
{
	struct fuse_fill_wb_data *data = _data;
	struct fuse_writepage_args *wpa = data->wpa;
	struct fuse_args_pages *ap = &wpa->ia.ap;
	struct inode *inode = data->inode;
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct page *tmp_page;
	int err;

	if (!data->ff) {
		err = -EIO;
		data->ff = fuse_write_file_get(fi);
		if (!data->ff)
			goto out_unlock;
	}

	if (wpa && fuse_writepage_need_send(fc, page, ap, data)) {
		fuse_writepages_send(data);
		data->wpa = NULL;
	}

>>>>>>> upstream/android-13
	err = -ENOMEM;
	tmp_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
	if (!tmp_page)
		goto out_unlock;

	/*
	 * The page must not be redirtied until the writeout is completed
	 * (i.e. userspace has sent a reply to the write request).  Otherwise
	 * there could be more than one temporary page instance for each real
	 * page.
	 *
	 * This is ensured by holding the page lock in page_mkwrite() while
	 * checking fuse_page_is_writeback().  We already hold the page lock
	 * since clear_page_dirty_for_io() and keep it held until we add the
<<<<<<< HEAD
	 * request to the fi->writepages list and increment req->num_pages.
	 * After this fuse_page_is_writeback() will indicate that the page is
	 * under writeback, so we can release the page lock.
	 */
	if (data->req == NULL) {
		struct fuse_inode *fi = get_fuse_inode(inode);

		err = -ENOMEM;
		req = fuse_request_alloc_nofs(FUSE_MAX_PAGES_PER_REQ);
		if (!req) {
			__free_page(tmp_page);
			goto out_unlock;
		}

		fuse_write_fill(req, data->ff, page_offset(page), 0);
		req->misc.write.in.write_flags |= FUSE_WRITE_CACHE;
		req->misc.write.next = NULL;
		req->in.argpages = 1;
		__set_bit(FR_BACKGROUND, &req->flags);
		req->num_pages = 0;
		req->end = fuse_writepage_end;
		req->inode = inode;

		spin_lock(&fc->lock);
		list_add(&req->writepages_entry, &fi->writepages);
		spin_unlock(&fc->lock);

		data->req = req;
=======
	 * request to the fi->writepages list and increment ap->num_pages.
	 * After this fuse_page_is_writeback() will indicate that the page is
	 * under writeback, so we can release the page lock.
	 */
	if (data->wpa == NULL) {
		err = -ENOMEM;
		wpa = fuse_writepage_args_alloc();
		if (!wpa) {
			__free_page(tmp_page);
			goto out_unlock;
		}
		fuse_writepage_add_to_bucket(fc, wpa);

		data->max_pages = 1;

		ap = &wpa->ia.ap;
		fuse_write_args_fill(&wpa->ia, data->ff, page_offset(page), 0);
		wpa->ia.write.in.write_flags |= FUSE_WRITE_CACHE;
		wpa->next = NULL;
		ap->args.in_pages = true;
		ap->args.end = fuse_writepage_end;
		ap->num_pages = 0;
		wpa->inode = inode;
>>>>>>> upstream/android-13
	}
	set_page_writeback(page);

	copy_highpage(tmp_page, page);
<<<<<<< HEAD
	req->pages[req->num_pages] = tmp_page;
	req->page_descs[req->num_pages].offset = 0;
	req->page_descs[req->num_pages].length = PAGE_SIZE;
=======
	ap->pages[ap->num_pages] = tmp_page;
	ap->descs[ap->num_pages].offset = 0;
	ap->descs[ap->num_pages].length = PAGE_SIZE;
	data->orig_pages[ap->num_pages] = page;
>>>>>>> upstream/android-13

	inc_wb_stat(&inode_to_bdi(inode)->wb, WB_WRITEBACK);
	inc_node_page_state(tmp_page, NR_WRITEBACK_TEMP);

	err = 0;
<<<<<<< HEAD
	if (is_writeback && fuse_writepage_in_flight(req, page)) {
		end_page_writeback(page);
		data->req = NULL;
		goto out_unlock;
	}
	data->orig_pages[req->num_pages] = page;

	/*
	 * Protected by fc->lock against concurrent access by
	 * fuse_page_is_writeback().
	 */
	spin_lock(&fc->lock);
	req->num_pages++;
	spin_unlock(&fc->lock);

=======
	if (data->wpa) {
		/*
		 * Protected by fi->lock against concurrent access by
		 * fuse_page_is_writeback().
		 */
		spin_lock(&fi->lock);
		ap->num_pages++;
		spin_unlock(&fi->lock);
	} else if (fuse_writepage_add(wpa, page)) {
		data->wpa = wpa;
	} else {
		end_page_writeback(page);
	}
>>>>>>> upstream/android-13
out_unlock:
	unlock_page(page);

	return err;
}

static int fuse_writepages(struct address_space *mapping,
			   struct writeback_control *wbc)
{
	struct inode *inode = mapping->host;
<<<<<<< HEAD
=======
	struct fuse_conn *fc = get_fuse_conn(inode);
>>>>>>> upstream/android-13
	struct fuse_fill_wb_data data;
	int err;

	err = -EIO;
<<<<<<< HEAD
	if (is_bad_inode(inode))
		goto out;

	data.inode = inode;
	data.req = NULL;
	data.ff = NULL;

	err = -ENOMEM;
	data.orig_pages = kcalloc(FUSE_MAX_PAGES_PER_REQ,
=======
	if (fuse_is_bad(inode))
		goto out;

	data.inode = inode;
	data.wpa = NULL;
	data.ff = NULL;

	err = -ENOMEM;
	data.orig_pages = kcalloc(fc->max_pages,
>>>>>>> upstream/android-13
				  sizeof(struct page *),
				  GFP_NOFS);
	if (!data.orig_pages)
		goto out;

	err = write_cache_pages(mapping, wbc, fuse_writepages_fill, &data);
<<<<<<< HEAD
	if (data.req) {
		/* Ignore errors if we can write at least one page */
		BUG_ON(!data.req->num_pages);
		fuse_writepages_send(&data);
		err = 0;
=======
	if (data.wpa) {
		WARN_ON(!data.wpa->ia.ap.num_pages);
		fuse_writepages_send(&data);
>>>>>>> upstream/android-13
	}
	if (data.ff)
		fuse_file_put(data.ff, false, false);

	kfree(data.orig_pages);
out:
	return err;
}

/*
 * It's worthy to make sure that space is reserved on disk for the write,
 * but how to implement it without killing performance need more thinking.
 */
static int fuse_write_begin(struct file *file, struct address_space *mapping,
		loff_t pos, unsigned len, unsigned flags,
		struct page **pagep, void **fsdata)
{
	pgoff_t index = pos >> PAGE_SHIFT;
	struct fuse_conn *fc = get_fuse_conn(file_inode(file));
	struct page *page;
	loff_t fsize;
	int err = -ENOMEM;

	WARN_ON(!fc->writeback_cache);

	page = grab_cache_page_write_begin(mapping, index, flags);
	if (!page)
		goto error;

	fuse_wait_on_page_writeback(mapping->host, page->index);

	if (PageUptodate(page) || len == PAGE_SIZE)
		goto success;
	/*
	 * Check if the start this page comes after the end of file, in which
	 * case the readpage can be optimized away.
	 */
	fsize = i_size_read(mapping->host);
	if (fsize <= (pos & PAGE_MASK)) {
		size_t off = pos & ~PAGE_MASK;
		if (off)
			zero_user_segment(page, 0, off);
		goto success;
	}
	err = fuse_do_readpage(file, page);
	if (err)
		goto cleanup;
success:
	*pagep = page;
<<<<<<< HEAD
	mtk_btag_pidlog_set_pid(page, PIDLOG_MODE_FS_FUSE, true);
=======
>>>>>>> upstream/android-13
	return 0;

cleanup:
	unlock_page(page);
	put_page(page);
error:
	return err;
}

static int fuse_write_end(struct file *file, struct address_space *mapping,
		loff_t pos, unsigned len, unsigned copied,
		struct page *page, void *fsdata)
{
	struct inode *inode = page->mapping->host;

	/* Haven't copied anything?  Skip zeroing, size extending, dirtying. */
	if (!copied)
		goto unlock;

	if (!PageUptodate(page)) {
		/* Zero any unwritten bytes at the end of the page */
		size_t endoff = (pos + copied) & ~PAGE_MASK;
		if (endoff)
			zero_user_segment(page, endoff, PAGE_SIZE);
		SetPageUptodate(page);
	}

	fuse_write_update_size(inode, pos + copied);
	set_page_dirty(page);

unlock:
	unlock_page(page);
	put_page(page);

	return copied;
}

static int fuse_launder_page(struct page *page)
{
	int err = 0;
	if (clear_page_dirty_for_io(page)) {
		struct inode *inode = page->mapping->host;
<<<<<<< HEAD
=======

		/* Serialize with pending writeback for the same page */
		fuse_wait_on_page_writeback(inode, page->index);
>>>>>>> upstream/android-13
		err = fuse_writepage_locked(page);
		if (!err)
			fuse_wait_on_page_writeback(inode, page->index);
	}
	return err;
}

/*
 * Write back dirty pages now, because there may not be any suitable
 * open files later
 */
static void fuse_vma_close(struct vm_area_struct *vma)
{
	filemap_write_and_wait(vma->vm_file->f_mapping);
}

/*
 * Wait for writeback against this page to complete before allowing it
 * to be marked dirty again, and hence written back again, possibly
 * before the previous writepage completed.
 *
 * Block here, instead of in ->writepage(), so that the userspace fs
 * can only block processes actually operating on the filesystem.
 *
 * Otherwise unprivileged userspace fs would be able to block
 * unrelated:
 *
 * - page migration
 * - sync(2)
 * - try_to_free_pages() with order > PAGE_ALLOC_COSTLY_ORDER
 */
static vm_fault_t fuse_page_mkwrite(struct vm_fault *vmf)
{
	struct page *page = vmf->page;
	struct inode *inode = file_inode(vmf->vma->vm_file);

	file_update_time(vmf->vma->vm_file);
	lock_page(page);
	if (page->mapping != inode->i_mapping) {
		unlock_page(page);
		return VM_FAULT_NOPAGE;
	}

	fuse_wait_on_page_writeback(inode, page->index);
	return VM_FAULT_LOCKED;
}

static const struct vm_operations_struct fuse_file_vm_ops = {
	.close		= fuse_vma_close,
	.fault		= filemap_fault,
	.map_pages	= filemap_map_pages,
	.page_mkwrite	= fuse_page_mkwrite,
<<<<<<< HEAD
=======
	.speculative	= true,
>>>>>>> upstream/android-13
};

static int fuse_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct fuse_file *ff = file->private_data;

<<<<<<< HEAD
	if (ff->passthrough.filp)
		return fuse_passthrough_mmap(file, vma);

=======
	/* DAX mmap is superior to direct_io mmap */
	if (FUSE_IS_DAX(file_inode(file)))
		return fuse_dax_mmap(file, vma);

	if (ff->passthrough.filp)
		return fuse_passthrough_mmap(file, vma);

	if (ff->open_flags & FOPEN_DIRECT_IO) {
		/* Can't provide the coherency needed for MAP_SHARED */
		if (vma->vm_flags & VM_MAYSHARE)
			return -ENODEV;

		invalidate_inode_pages2(file->f_mapping);

		return generic_file_mmap(file, vma);
	}

>>>>>>> upstream/android-13
	if ((vma->vm_flags & VM_SHARED) && (vma->vm_flags & VM_MAYWRITE))
		fuse_link_write_file(file);

	file_accessed(file);
	vma->vm_ops = &fuse_file_vm_ops;
	return 0;
}

<<<<<<< HEAD
static int fuse_direct_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct fuse_file *ff = file->private_data;

	if (ff->passthrough.filp)
		return fuse_passthrough_mmap(file, vma);

	/* Can't provide the coherency needed for MAP_SHARED */
	if (vma->vm_flags & VM_MAYSHARE)
		return -ENODEV;

	invalidate_inode_pages2(file->f_mapping);

	return generic_file_mmap(file, vma);
}

=======
>>>>>>> upstream/android-13
static int convert_fuse_file_lock(struct fuse_conn *fc,
				  const struct fuse_file_lock *ffl,
				  struct file_lock *fl)
{
	switch (ffl->type) {
	case F_UNLCK:
		break;

	case F_RDLCK:
	case F_WRLCK:
		if (ffl->start > OFFSET_MAX || ffl->end > OFFSET_MAX ||
		    ffl->end < ffl->start)
			return -EIO;

		fl->fl_start = ffl->start;
		fl->fl_end = ffl->end;

		/*
		 * Convert pid into init's pid namespace.  The locks API will
		 * translate it into the caller's pid namespace.
		 */
		rcu_read_lock();
		fl->fl_pid = pid_nr_ns(find_pid_ns(ffl->pid, fc->pid_ns), &init_pid_ns);
		rcu_read_unlock();
		break;

	default:
		return -EIO;
	}
	fl->fl_type = ffl->type;
	return 0;
}

static void fuse_lk_fill(struct fuse_args *args, struct file *file,
			 const struct file_lock *fl, int opcode, pid_t pid,
			 int flock, struct fuse_lk_in *inarg)
{
	struct inode *inode = file_inode(file);
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_file *ff = file->private_data;

	memset(inarg, 0, sizeof(*inarg));
	inarg->fh = ff->fh;
	inarg->owner = fuse_lock_owner_id(fc, fl->fl_owner);
	inarg->lk.start = fl->fl_start;
	inarg->lk.end = fl->fl_end;
	inarg->lk.type = fl->fl_type;
	inarg->lk.pid = pid;
	if (flock)
		inarg->lk_flags |= FUSE_LK_FLOCK;
<<<<<<< HEAD
	args->in.h.opcode = opcode;
	args->in.h.nodeid = get_node_id(inode);
	args->in.numargs = 1;
	args->in.args[0].size = sizeof(*inarg);
	args->in.args[0].value = inarg;
=======
	args->opcode = opcode;
	args->nodeid = get_node_id(inode);
	args->in_numargs = 1;
	args->in_args[0].size = sizeof(*inarg);
	args->in_args[0].value = inarg;
>>>>>>> upstream/android-13
}

static int fuse_getlk(struct file *file, struct file_lock *fl)
{
	struct inode *inode = file_inode(file);
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
>>>>>>> upstream/android-13
	FUSE_ARGS(args);
	struct fuse_lk_in inarg;
	struct fuse_lk_out outarg;
	int err;

	fuse_lk_fill(&args, file, fl, FUSE_GETLK, 0, 0, &inarg);
<<<<<<< HEAD
	args.out.numargs = 1;
	args.out.args[0].size = sizeof(outarg);
	args.out.args[0].value = &outarg;
	err = fuse_simple_request(fc, &args);
	if (!err)
		err = convert_fuse_file_lock(fc, &outarg.lk, fl);
=======
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
	if (!err)
		err = convert_fuse_file_lock(fm->fc, &outarg.lk, fl);
>>>>>>> upstream/android-13

	return err;
}

static int fuse_setlk(struct file *file, struct file_lock *fl, int flock)
{
	struct inode *inode = file_inode(file);
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
>>>>>>> upstream/android-13
	FUSE_ARGS(args);
	struct fuse_lk_in inarg;
	int opcode = (fl->fl_flags & FL_SLEEP) ? FUSE_SETLKW : FUSE_SETLK;
	struct pid *pid = fl->fl_type != F_UNLCK ? task_tgid(current) : NULL;
<<<<<<< HEAD
	pid_t pid_nr = pid_nr_ns(pid, fc->pid_ns);
=======
	pid_t pid_nr = pid_nr_ns(pid, fm->fc->pid_ns);
>>>>>>> upstream/android-13
	int err;

	if (fl->fl_lmops && fl->fl_lmops->lm_grant) {
		/* NLM needs asynchronous locks, which we don't support yet */
		return -ENOLCK;
	}

	/* Unlock on close is handled by the flush method */
	if ((fl->fl_flags & FL_CLOSE_POSIX) == FL_CLOSE_POSIX)
		return 0;

	fuse_lk_fill(&args, file, fl, opcode, pid_nr, flock, &inarg);
<<<<<<< HEAD
	err = fuse_simple_request(fc, &args);
=======
	err = fuse_simple_request(fm, &args);
>>>>>>> upstream/android-13

	/* locking is restartable */
	if (err == -EINTR)
		err = -ERESTARTSYS;

	return err;
}

static int fuse_file_lock(struct file *file, int cmd, struct file_lock *fl)
{
	struct inode *inode = file_inode(file);
	struct fuse_conn *fc = get_fuse_conn(inode);
	int err;

	if (cmd == F_CANCELLK) {
		err = 0;
	} else if (cmd == F_GETLK) {
		if (fc->no_lock) {
			posix_test_lock(file, fl);
			err = 0;
		} else
			err = fuse_getlk(file, fl);
	} else {
		if (fc->no_lock)
			err = posix_lock_file(file, fl, NULL);
		else
			err = fuse_setlk(file, fl, 0);
	}
	return err;
}

static int fuse_file_flock(struct file *file, int cmd, struct file_lock *fl)
{
	struct inode *inode = file_inode(file);
	struct fuse_conn *fc = get_fuse_conn(inode);
	int err;

	if (fc->no_flock) {
		err = locks_lock_file_wait(file, fl);
	} else {
		struct fuse_file *ff = file->private_data;

		/* emulate flock with POSIX locks */
		ff->flock = true;
		err = fuse_setlk(file, fl, 1);
	}

	return err;
}

static sector_t fuse_bmap(struct address_space *mapping, sector_t block)
{
	struct inode *inode = mapping->host;
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
>>>>>>> upstream/android-13
	FUSE_ARGS(args);
	struct fuse_bmap_in inarg;
	struct fuse_bmap_out outarg;
	int err;

<<<<<<< HEAD
	if (!inode->i_sb->s_bdev || fc->no_bmap)
=======
	if (!inode->i_sb->s_bdev || fm->fc->no_bmap)
>>>>>>> upstream/android-13
		return 0;

	memset(&inarg, 0, sizeof(inarg));
	inarg.block = block;
	inarg.blocksize = inode->i_sb->s_blocksize;
<<<<<<< HEAD
	args.in.h.opcode = FUSE_BMAP;
	args.in.h.nodeid = get_node_id(inode);
	args.in.numargs = 1;
	args.in.args[0].size = sizeof(inarg);
	args.in.args[0].value = &inarg;
	args.out.numargs = 1;
	args.out.args[0].size = sizeof(outarg);
	args.out.args[0].value = &outarg;
	err = fuse_simple_request(fc, &args);
	if (err == -ENOSYS)
		fc->no_bmap = 1;
=======
	args.opcode = FUSE_BMAP;
	args.nodeid = get_node_id(inode);
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
	if (err == -ENOSYS)
		fm->fc->no_bmap = 1;
>>>>>>> upstream/android-13

	return err ? 0 : outarg.block;
}

static loff_t fuse_lseek(struct file *file, loff_t offset, int whence)
{
	struct inode *inode = file->f_mapping->host;
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
>>>>>>> upstream/android-13
	struct fuse_file *ff = file->private_data;
	FUSE_ARGS(args);
	struct fuse_lseek_in inarg = {
		.fh = ff->fh,
		.offset = offset,
		.whence = whence
	};
	struct fuse_lseek_out outarg;
	int err;

<<<<<<< HEAD
	if (fc->no_lseek)
		goto fallback;

	args.in.h.opcode = FUSE_LSEEK;
	args.in.h.nodeid = ff->nodeid;
	args.in.numargs = 1;
	args.in.args[0].size = sizeof(inarg);
	args.in.args[0].value = &inarg;
	args.out.numargs = 1;
	args.out.args[0].size = sizeof(outarg);
	args.out.args[0].value = &outarg;
	err = fuse_simple_request(fc, &args);
	if (err) {
		if (err == -ENOSYS) {
			fc->no_lseek = 1;
=======
	if (fm->fc->no_lseek)
		goto fallback;

	args.opcode = FUSE_LSEEK;
	args.nodeid = ff->nodeid;
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
	if (err) {
		if (err == -ENOSYS) {
			fm->fc->no_lseek = 1;
>>>>>>> upstream/android-13
			goto fallback;
		}
		return err;
	}

	return vfs_setpos(file, outarg.offset, inode->i_sb->s_maxbytes);

fallback:
	err = fuse_update_attributes(inode, file);
	if (!err)
		return generic_file_llseek(file, offset, whence);
	else
		return err;
}

static loff_t fuse_file_llseek(struct file *file, loff_t offset, int whence)
{
	loff_t retval;
	struct inode *inode = file_inode(file);

	switch (whence) {
	case SEEK_SET:
	case SEEK_CUR:
		 /* No i_mutex protection necessary for SEEK_CUR and SEEK_SET */
		retval = generic_file_llseek(file, offset, whence);
		break;
	case SEEK_END:
		inode_lock(inode);
		retval = fuse_update_attributes(inode, file);
		if (!retval)
			retval = generic_file_llseek(file, offset, whence);
		inode_unlock(inode);
		break;
	case SEEK_HOLE:
	case SEEK_DATA:
		inode_lock(inode);
		retval = fuse_lseek(file, offset, whence);
		inode_unlock(inode);
		break;
	default:
		retval = -EINVAL;
	}

	return retval;
}

/*
<<<<<<< HEAD
 * CUSE servers compiled on 32bit broke on 64bit kernels because the
 * ABI was defined to be 'struct iovec' which is different on 32bit
 * and 64bit.  Fortunately we can determine which structure the server
 * used from the size of the reply.
 */
static int fuse_copy_ioctl_iovec_old(struct iovec *dst, void *src,
				     size_t transferred, unsigned count,
				     bool is_compat)
{
#ifdef CONFIG_COMPAT
	if (count * sizeof(struct compat_iovec) == transferred) {
		struct compat_iovec *ciov = src;
		unsigned i;

		/*
		 * With this interface a 32bit server cannot support
		 * non-compat (i.e. ones coming from 64bit apps) ioctl
		 * requests
		 */
		if (!is_compat)
			return -EINVAL;

		for (i = 0; i < count; i++) {
			dst[i].iov_base = compat_ptr(ciov[i].iov_base);
			dst[i].iov_len = ciov[i].iov_len;
		}
		return 0;
	}
#endif

	if (count * sizeof(struct iovec) != transferred)
		return -EIO;

	memcpy(dst, src, transferred);
	return 0;
}

/* Make sure iov_length() won't overflow */
static int fuse_verify_ioctl_iov(struct iovec *iov, size_t count)
{
	size_t n;
	u32 max = FUSE_MAX_PAGES_PER_REQ << PAGE_SHIFT;

	for (n = 0; n < count; n++, iov++) {
		if (iov->iov_len > (size_t) max)
			return -ENOMEM;
		max -= iov->iov_len;
	}
	return 0;
}

static int fuse_copy_ioctl_iovec(struct fuse_conn *fc, struct iovec *dst,
				 void *src, size_t transferred, unsigned count,
				 bool is_compat)
{
	unsigned i;
	struct fuse_ioctl_iovec *fiov = src;

	if (fc->minor < 16) {
		return fuse_copy_ioctl_iovec_old(dst, src, transferred,
						 count, is_compat);
	}

	if (count * sizeof(struct fuse_ioctl_iovec) != transferred)
		return -EIO;

	for (i = 0; i < count; i++) {
		/* Did the server supply an inappropriate value? */
		if (fiov[i].base != (unsigned long) fiov[i].base ||
		    fiov[i].len != (unsigned long) fiov[i].len)
			return -EIO;

		dst[i].iov_base = (void __user *) (unsigned long) fiov[i].base;
		dst[i].iov_len = (size_t) fiov[i].len;

#ifdef CONFIG_COMPAT
		if (is_compat &&
		    (ptr_to_compat(dst[i].iov_base) != fiov[i].base ||
		     (compat_size_t) dst[i].iov_len != fiov[i].len))
			return -EIO;
#endif
	}

	return 0;
}


/*
 * For ioctls, there is no generic way to determine how much memory
 * needs to be read and/or written.  Furthermore, ioctls are allowed
 * to dereference the passed pointer, so the parameter requires deep
 * copying but FUSE has no idea whatsoever about what to copy in or
 * out.
 *
 * This is solved by allowing FUSE server to retry ioctl with
 * necessary in/out iovecs.  Let's assume the ioctl implementation
 * needs to read in the following structure.
 *
 * struct a {
 *	char	*buf;
 *	size_t	buflen;
 * }
 *
 * On the first callout to FUSE server, inarg->in_size and
 * inarg->out_size will be NULL; then, the server completes the ioctl
 * with FUSE_IOCTL_RETRY set in out->flags, out->in_iovs set to 1 and
 * the actual iov array to
 *
 * { { .iov_base = inarg.arg,	.iov_len = sizeof(struct a) } }
 *
 * which tells FUSE to copy in the requested area and retry the ioctl.
 * On the second round, the server has access to the structure and
 * from that it can tell what to look for next, so on the invocation,
 * it sets FUSE_IOCTL_RETRY, out->in_iovs to 2 and iov array to
 *
 * { { .iov_base = inarg.arg,	.iov_len = sizeof(struct a)	},
 *   { .iov_base = a.buf,	.iov_len = a.buflen		} }
 *
 * FUSE will copy both struct a and the pointed buffer from the
 * process doing the ioctl and retry ioctl with both struct a and the
 * buffer.
 *
 * This time, FUSE server has everything it needs and completes ioctl
 * without FUSE_IOCTL_RETRY which finishes the ioctl call.
 *
 * Copying data out works the same way.
 *
 * Note that if FUSE_IOCTL_UNRESTRICTED is clear, the kernel
 * automatically initializes in and out iovs by decoding @cmd with
 * _IOC_* macros and the server is not allowed to request RETRY.  This
 * limits ioctl data transfers to well-formed ioctls and is the forced
 * behavior for all FUSE servers.
 */
long fuse_do_ioctl(struct file *file, unsigned int cmd, unsigned long arg,
		   unsigned int flags)
{
	struct fuse_file *ff = file->private_data;
	struct fuse_conn *fc = ff->fc;
	struct fuse_ioctl_in inarg = {
		.fh = ff->fh,
		.cmd = cmd,
		.arg = arg,
		.flags = flags
	};
	struct fuse_ioctl_out outarg;
	struct fuse_req *req = NULL;
	struct page **pages = NULL;
	struct iovec *iov_page = NULL;
	struct iovec *in_iov = NULL, *out_iov = NULL;
	unsigned int in_iovs = 0, out_iovs = 0, num_pages = 0, max_pages;
	size_t in_size, out_size, transferred, c;
	int err, i;
	struct iov_iter ii;

#if BITS_PER_LONG == 32
	inarg.flags |= FUSE_IOCTL_32BIT;
#else
	if (flags & FUSE_IOCTL_COMPAT)
		inarg.flags |= FUSE_IOCTL_32BIT;
#endif

	/* assume all the iovs returned by client always fits in a page */
	BUILD_BUG_ON(sizeof(struct fuse_ioctl_iovec) * FUSE_IOCTL_MAX_IOV > PAGE_SIZE);

	err = -ENOMEM;
	pages = kcalloc(FUSE_MAX_PAGES_PER_REQ, sizeof(pages[0]), GFP_KERNEL);
	iov_page = (struct iovec *) __get_free_page(GFP_KERNEL);
	if (!pages || !iov_page)
		goto out;

	/*
	 * If restricted, initialize IO parameters as encoded in @cmd.
	 * RETRY from server is not allowed.
	 */
	if (!(flags & FUSE_IOCTL_UNRESTRICTED)) {
		struct iovec *iov = iov_page;

		iov->iov_base = (void __user *)arg;

		switch (cmd) {
		case FS_IOC_GETFLAGS:
		case FS_IOC_SETFLAGS:
			iov->iov_len = sizeof(int);
			break;
		default:
			iov->iov_len = _IOC_SIZE(cmd);
			break;
		}

		if (_IOC_DIR(cmd) & _IOC_WRITE) {
			in_iov = iov;
			in_iovs = 1;
		}

		if (_IOC_DIR(cmd) & _IOC_READ) {
			out_iov = iov;
			out_iovs = 1;
		}
	}

 retry:
	inarg.in_size = in_size = iov_length(in_iov, in_iovs);
	inarg.out_size = out_size = iov_length(out_iov, out_iovs);

	/*
	 * Out data can be used either for actual out data or iovs,
	 * make sure there always is at least one page.
	 */
	out_size = max_t(size_t, out_size, PAGE_SIZE);
	max_pages = DIV_ROUND_UP(max(in_size, out_size), PAGE_SIZE);

	/* make sure there are enough buffer pages and init request with them */
	err = -ENOMEM;
	if (max_pages > FUSE_MAX_PAGES_PER_REQ)
		goto out;
	while (num_pages < max_pages) {
		pages[num_pages] = alloc_page(GFP_KERNEL | __GFP_HIGHMEM);
		if (!pages[num_pages])
			goto out;
		num_pages++;
	}

	req = fuse_get_req(fc, num_pages);
	if (IS_ERR(req)) {
		err = PTR_ERR(req);
		req = NULL;
		goto out;
	}
	memcpy(req->pages, pages, sizeof(req->pages[0]) * num_pages);
	req->num_pages = num_pages;
	fuse_page_descs_length_init(req, 0, req->num_pages);

	/* okay, let's send it to the client */
	req->in.h.opcode = FUSE_IOCTL;
	req->in.h.nodeid = ff->nodeid;
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	if (in_size) {
		req->in.numargs++;
		req->in.args[1].size = in_size;
		req->in.argpages = 1;

		err = -EFAULT;
		iov_iter_init(&ii, WRITE, in_iov, in_iovs, in_size);
		for (i = 0; iov_iter_count(&ii) && !WARN_ON(i >= num_pages); i++) {
			c = copy_page_from_iter(pages[i], 0, PAGE_SIZE, &ii);
			if (c != PAGE_SIZE && iov_iter_count(&ii))
				goto out;
		}
	}

	req->out.numargs = 2;
	req->out.args[0].size = sizeof(outarg);
	req->out.args[0].value = &outarg;
	req->out.args[1].size = out_size;
	req->out.argpages = 1;
	req->out.argvar = 1;

	fuse_request_send(fc, req);
	err = req->out.h.error;
	transferred = req->out.args[1].size;
	fuse_put_request(fc, req);
	req = NULL;
	if (err)
		goto out;

	/* did it ask for retry? */
	if (outarg.flags & FUSE_IOCTL_RETRY) {
		void *vaddr;

		/* no retry if in restricted mode */
		err = -EIO;
		if (!(flags & FUSE_IOCTL_UNRESTRICTED))
			goto out;

		in_iovs = outarg.in_iovs;
		out_iovs = outarg.out_iovs;

		/*
		 * Make sure things are in boundary, separate checks
		 * are to protect against overflow.
		 */
		err = -ENOMEM;
		if (in_iovs > FUSE_IOCTL_MAX_IOV ||
		    out_iovs > FUSE_IOCTL_MAX_IOV ||
		    in_iovs + out_iovs > FUSE_IOCTL_MAX_IOV)
			goto out;

		vaddr = kmap_atomic(pages[0]);
		err = fuse_copy_ioctl_iovec(fc, iov_page, vaddr,
					    transferred, in_iovs + out_iovs,
					    (flags & FUSE_IOCTL_COMPAT) != 0);
		kunmap_atomic(vaddr);
		if (err)
			goto out;

		in_iov = iov_page;
		out_iov = in_iov + in_iovs;

		err = fuse_verify_ioctl_iov(in_iov, in_iovs);
		if (err)
			goto out;

		err = fuse_verify_ioctl_iov(out_iov, out_iovs);
		if (err)
			goto out;

		goto retry;
	}

	err = -EIO;
	if (transferred > inarg.out_size)
		goto out;

	err = -EFAULT;
	iov_iter_init(&ii, READ, out_iov, out_iovs, transferred);
	for (i = 0; iov_iter_count(&ii) && !WARN_ON(i >= num_pages); i++) {
		c = copy_page_to_iter(pages[i], 0, PAGE_SIZE, &ii);
		if (c != PAGE_SIZE && iov_iter_count(&ii))
			goto out;
	}
	err = 0;
 out:
	if (req)
		fuse_put_request(fc, req);
	free_page((unsigned long) iov_page);
	while (num_pages)
		__free_page(pages[--num_pages]);
	kfree(pages);

	return err ? err : outarg.result;
}
EXPORT_SYMBOL_GPL(fuse_do_ioctl);

long fuse_ioctl_common(struct file *file, unsigned int cmd,
		       unsigned long arg, unsigned int flags)
{
	struct inode *inode = file_inode(file);
	struct fuse_conn *fc = get_fuse_conn(inode);

	if (!fuse_allow_current_process(fc))
		return -EACCES;

	if (is_bad_inode(inode))
		return -EIO;

	return fuse_do_ioctl(file, cmd, arg, flags);
}

static long fuse_file_ioctl(struct file *file, unsigned int cmd,
			    unsigned long arg)
{
	return fuse_ioctl_common(file, cmd, arg, 0);
}

static long fuse_file_compat_ioctl(struct file *file, unsigned int cmd,
				   unsigned long arg)
{
	return fuse_ioctl_common(file, cmd, arg, FUSE_IOCTL_COMPAT);
}

/*
=======
>>>>>>> upstream/android-13
 * All files which have been polled are linked to RB tree
 * fuse_conn->polled_files which is indexed by kh.  Walk the tree and
 * find the matching one.
 */
static struct rb_node **fuse_find_polled_node(struct fuse_conn *fc, u64 kh,
					      struct rb_node **parent_out)
{
	struct rb_node **link = &fc->polled_files.rb_node;
	struct rb_node *last = NULL;

	while (*link) {
		struct fuse_file *ff;

		last = *link;
		ff = rb_entry(last, struct fuse_file, polled_node);

		if (kh < ff->kh)
			link = &last->rb_left;
		else if (kh > ff->kh)
			link = &last->rb_right;
		else
			return link;
	}

	if (parent_out)
		*parent_out = last;
	return link;
}

/*
 * The file is about to be polled.  Make sure it's on the polled_files
 * RB tree.  Note that files once added to the polled_files tree are
 * not removed before the file is released.  This is because a file
 * polled once is likely to be polled again.
 */
static void fuse_register_polled_file(struct fuse_conn *fc,
				      struct fuse_file *ff)
{
	spin_lock(&fc->lock);
	if (RB_EMPTY_NODE(&ff->polled_node)) {
<<<<<<< HEAD
		struct rb_node **link, *uninitialized_var(parent);
=======
		struct rb_node **link, *parent;
>>>>>>> upstream/android-13

		link = fuse_find_polled_node(fc, ff->kh, &parent);
		BUG_ON(*link);
		rb_link_node(&ff->polled_node, parent, link);
		rb_insert_color(&ff->polled_node, &fc->polled_files);
	}
	spin_unlock(&fc->lock);
}

__poll_t fuse_file_poll(struct file *file, poll_table *wait)
{
	struct fuse_file *ff = file->private_data;
<<<<<<< HEAD
	struct fuse_conn *fc = ff->fc;
=======
	struct fuse_mount *fm = ff->fm;
>>>>>>> upstream/android-13
	struct fuse_poll_in inarg = { .fh = ff->fh, .kh = ff->kh };
	struct fuse_poll_out outarg;
	FUSE_ARGS(args);
	int err;

<<<<<<< HEAD
	if (fc->no_poll)
=======
	if (fm->fc->no_poll)
>>>>>>> upstream/android-13
		return DEFAULT_POLLMASK;

	poll_wait(file, &ff->poll_wait, wait);
	inarg.events = mangle_poll(poll_requested_events(wait));

	/*
	 * Ask for notification iff there's someone waiting for it.
	 * The client may ignore the flag and always notify.
	 */
	if (waitqueue_active(&ff->poll_wait)) {
		inarg.flags |= FUSE_POLL_SCHEDULE_NOTIFY;
<<<<<<< HEAD
		fuse_register_polled_file(fc, ff);
	}

	args.in.h.opcode = FUSE_POLL;
	args.in.h.nodeid = ff->nodeid;
	args.in.numargs = 1;
	args.in.args[0].size = sizeof(inarg);
	args.in.args[0].value = &inarg;
	args.out.numargs = 1;
	args.out.args[0].size = sizeof(outarg);
	args.out.args[0].value = &outarg;
	err = fuse_simple_request(fc, &args);
=======
		fuse_register_polled_file(fm->fc, ff);
	}

	args.opcode = FUSE_POLL;
	args.nodeid = ff->nodeid;
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
>>>>>>> upstream/android-13

	if (!err)
		return demangle_poll(outarg.revents);
	if (err == -ENOSYS) {
<<<<<<< HEAD
		fc->no_poll = 1;
=======
		fm->fc->no_poll = 1;
>>>>>>> upstream/android-13
		return DEFAULT_POLLMASK;
	}
	return EPOLLERR;
}
EXPORT_SYMBOL_GPL(fuse_file_poll);

/*
 * This is called from fuse_handle_notify() on FUSE_NOTIFY_POLL and
 * wakes up the poll waiters.
 */
int fuse_notify_poll_wakeup(struct fuse_conn *fc,
			    struct fuse_notify_poll_wakeup_out *outarg)
{
	u64 kh = outarg->kh;
	struct rb_node **link;

	spin_lock(&fc->lock);

	link = fuse_find_polled_node(fc, kh, NULL);
	if (*link) {
		struct fuse_file *ff;

		ff = rb_entry(*link, struct fuse_file, polled_node);
		wake_up_interruptible_sync(&ff->poll_wait);
	}

	spin_unlock(&fc->lock);
	return 0;
}

static void fuse_do_truncate(struct file *file)
{
	struct inode *inode = file->f_mapping->host;
	struct iattr attr;

	attr.ia_valid = ATTR_SIZE;
	attr.ia_size = i_size_read(inode);

	attr.ia_file = file;
	attr.ia_valid |= ATTR_FILE;

	fuse_do_setattr(file_dentry(file), &attr, file);
}

<<<<<<< HEAD
static inline loff_t fuse_round_up(loff_t off)
{
	return round_up(off, FUSE_MAX_PAGES_PER_REQ << PAGE_SHIFT);
=======
static inline loff_t fuse_round_up(struct fuse_conn *fc, loff_t off)
{
	return round_up(off, fc->max_pages << PAGE_SHIFT);
>>>>>>> upstream/android-13
}

static ssize_t
fuse_direct_IO(struct kiocb *iocb, struct iov_iter *iter)
{
	DECLARE_COMPLETION_ONSTACK(wait);
	ssize_t ret = 0;
	struct file *file = iocb->ki_filp;
	struct fuse_file *ff = file->private_data;
<<<<<<< HEAD
	bool async_dio = ff->fc->async_dio;
	loff_t pos = 0;
	struct inode *inode;
	loff_t i_size;
	size_t count = iov_iter_count(iter);
=======
	loff_t pos = 0;
	struct inode *inode;
	loff_t i_size;
	size_t count = iov_iter_count(iter), shortened = 0;
>>>>>>> upstream/android-13
	loff_t offset = iocb->ki_pos;
	struct fuse_io_priv *io;

	pos = offset;
	inode = file->f_mapping->host;
	i_size = i_size_read(inode);

<<<<<<< HEAD
	if ((iov_iter_rw(iter) == READ) && (offset > i_size))
		return 0;

	/* optimization for short read */
	if (async_dio && iov_iter_rw(iter) != WRITE && offset + count > i_size) {
		if (offset >= i_size)
			return 0;
		iov_iter_truncate(iter, fuse_round_up(i_size - offset));
		count = iov_iter_count(iter);
	}

=======
	if ((iov_iter_rw(iter) == READ) && (offset >= i_size))
		return 0;

>>>>>>> upstream/android-13
	io = kmalloc(sizeof(struct fuse_io_priv), GFP_KERNEL);
	if (!io)
		return -ENOMEM;
	spin_lock_init(&io->lock);
	kref_init(&io->refcnt);
	io->reqs = 1;
	io->bytes = -1;
	io->size = 0;
	io->offset = offset;
	io->write = (iov_iter_rw(iter) == WRITE);
	io->err = 0;
	/*
	 * By default, we want to optimize all I/Os with async request
	 * submission to the client filesystem if supported.
	 */
<<<<<<< HEAD
	io->async = async_dio;
	io->iocb = iocb;
	io->blocking = is_sync_kiocb(iocb);

=======
	io->async = ff->fm->fc->async_dio;
	io->iocb = iocb;
	io->blocking = is_sync_kiocb(iocb);

	/* optimization for short read */
	if (io->async && !io->write && offset + count > i_size) {
		iov_iter_truncate(iter, fuse_round_up(ff->fm->fc, i_size - offset));
		shortened = count - iov_iter_count(iter);
		count -= shortened;
	}

>>>>>>> upstream/android-13
	/*
	 * We cannot asynchronously extend the size of a file.
	 * In such case the aio will behave exactly like sync io.
	 */
<<<<<<< HEAD
	if ((offset + count > i_size) && iov_iter_rw(iter) == WRITE)
=======
	if ((offset + count > i_size) && io->write)
>>>>>>> upstream/android-13
		io->blocking = true;

	if (io->async && io->blocking) {
		/*
		 * Additional reference to keep io around after
		 * calling fuse_aio_complete()
		 */
		kref_get(&io->refcnt);
		io->done = &wait;
	}

	if (iov_iter_rw(iter) == WRITE) {
		ret = fuse_direct_io(io, iter, &pos, FUSE_DIO_WRITE);
		fuse_invalidate_attr(inode);
	} else {
		ret = __fuse_direct_read(io, iter, &pos);
	}
<<<<<<< HEAD
=======
	iov_iter_reexpand(iter, iov_iter_count(iter) + shortened);
>>>>>>> upstream/android-13

	if (io->async) {
		bool blocking = io->blocking;

		fuse_aio_complete(io, ret < 0 ? ret : 0, -1);

		/* we have a non-extending, async request, so return */
		if (!blocking)
			return -EIOCBQUEUED;

		wait_for_completion(&wait);
		ret = fuse_get_res_by_io(io);
	}

	kref_put(&io->refcnt, fuse_io_release);

	if (iov_iter_rw(iter) == WRITE) {
		if (ret > 0)
			fuse_write_update_size(inode, pos);
		else if (ret < 0 && offset + count > i_size)
			fuse_do_truncate(file);
	}

	return ret;
}

<<<<<<< HEAD
=======
static int fuse_writeback_range(struct inode *inode, loff_t start, loff_t end)
{
	int err = filemap_write_and_wait_range(inode->i_mapping, start, LLONG_MAX);

	if (!err)
		fuse_sync_writes(inode);

	return err;
}

>>>>>>> upstream/android-13
static long fuse_file_fallocate(struct file *file, int mode, loff_t offset,
				loff_t length)
{
	struct fuse_file *ff = file->private_data;
	struct inode *inode = file_inode(file);
	struct fuse_inode *fi = get_fuse_inode(inode);
<<<<<<< HEAD
	struct fuse_conn *fc = ff->fc;
=======
	struct fuse_mount *fm = ff->fm;
>>>>>>> upstream/android-13
	FUSE_ARGS(args);
	struct fuse_fallocate_in inarg = {
		.fh = ff->fh,
		.offset = offset,
		.length = length,
		.mode = mode
	};
	int err;
	bool lock_inode = !(mode & FALLOC_FL_KEEP_SIZE) ||
<<<<<<< HEAD
			   (mode & FALLOC_FL_PUNCH_HOLE);

	if (mode & ~(FALLOC_FL_KEEP_SIZE | FALLOC_FL_PUNCH_HOLE))
		return -EOPNOTSUPP;

	if (fc->no_fallocate)
=======
			   (mode & (FALLOC_FL_PUNCH_HOLE |
				    FALLOC_FL_ZERO_RANGE));

	bool block_faults = FUSE_IS_DAX(inode) && lock_inode;

	if (mode & ~(FALLOC_FL_KEEP_SIZE | FALLOC_FL_PUNCH_HOLE |
		     FALLOC_FL_ZERO_RANGE))
		return -EOPNOTSUPP;

	if (fm->fc->no_fallocate)
>>>>>>> upstream/android-13
		return -EOPNOTSUPP;

	if (lock_inode) {
		inode_lock(inode);
<<<<<<< HEAD
		if (mode & FALLOC_FL_PUNCH_HOLE) {
			loff_t endbyte = offset + length - 1;
			err = filemap_write_and_wait_range(inode->i_mapping,
							   offset, endbyte);
			if (err)
				goto out;

			fuse_sync_writes(inode);
=======
		if (block_faults) {
			filemap_invalidate_lock(inode->i_mapping);
			err = fuse_dax_break_layouts(inode, 0, 0);
			if (err)
				goto out;
		}

		if (mode & (FALLOC_FL_PUNCH_HOLE | FALLOC_FL_ZERO_RANGE)) {
			loff_t endbyte = offset + length - 1;

			err = fuse_writeback_range(inode, offset, endbyte);
			if (err)
				goto out;
>>>>>>> upstream/android-13
		}
	}

	if (!(mode & FALLOC_FL_KEEP_SIZE) &&
	    offset + length > i_size_read(inode)) {
		err = inode_newsize_ok(inode, offset + length);
		if (err)
			goto out;
	}

	if (!(mode & FALLOC_FL_KEEP_SIZE))
		set_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);

<<<<<<< HEAD
	args.in.h.opcode = FUSE_FALLOCATE;
	args.in.h.nodeid = ff->nodeid;
	args.in.numargs = 1;
	args.in.args[0].size = sizeof(inarg);
	args.in.args[0].value = &inarg;
	err = fuse_simple_request(fc, &args);
	if (err == -ENOSYS) {
		fc->no_fallocate = 1;
=======
	args.opcode = FUSE_FALLOCATE;
	args.nodeid = ff->nodeid;
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	err = fuse_simple_request(fm, &args);
	if (err == -ENOSYS) {
		fm->fc->no_fallocate = 1;
>>>>>>> upstream/android-13
		err = -EOPNOTSUPP;
	}
	if (err)
		goto out;

	/* we could have extended the file */
	if (!(mode & FALLOC_FL_KEEP_SIZE)) {
		bool changed = fuse_write_update_size(inode, offset + length);

<<<<<<< HEAD
		if (changed && fc->writeback_cache)
			file_update_time(file);
	}

	if (mode & FALLOC_FL_PUNCH_HOLE)
=======
		if (changed && fm->fc->writeback_cache)
			file_update_time(file);
	}

	if (mode & (FALLOC_FL_PUNCH_HOLE | FALLOC_FL_ZERO_RANGE))
>>>>>>> upstream/android-13
		truncate_pagecache_range(inode, offset, offset + length - 1);

	fuse_invalidate_attr(inode);

out:
	if (!(mode & FALLOC_FL_KEEP_SIZE))
		clear_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);

<<<<<<< HEAD
	if (lock_inode)
		inode_unlock(inode);

	return err;
}

=======
	if (block_faults)
		filemap_invalidate_unlock(inode->i_mapping);

	if (lock_inode)
		inode_unlock(inode);

	fuse_flush_time_update(inode);

	return err;
}

static ssize_t __fuse_copy_file_range(struct file *file_in, loff_t pos_in,
				      struct file *file_out, loff_t pos_out,
				      size_t len, unsigned int flags)
{
	struct fuse_file *ff_in = file_in->private_data;
	struct fuse_file *ff_out = file_out->private_data;
	struct inode *inode_in = file_inode(file_in);
	struct inode *inode_out = file_inode(file_out);
	struct fuse_inode *fi_out = get_fuse_inode(inode_out);
	struct fuse_mount *fm = ff_in->fm;
	struct fuse_conn *fc = fm->fc;
	FUSE_ARGS(args);
	struct fuse_copy_file_range_in inarg = {
		.fh_in = ff_in->fh,
		.off_in = pos_in,
		.nodeid_out = ff_out->nodeid,
		.fh_out = ff_out->fh,
		.off_out = pos_out,
		.len = len,
		.flags = flags
	};
	struct fuse_write_out outarg;
	ssize_t err;
	/* mark unstable when write-back is not used, and file_out gets
	 * extended */
	bool is_unstable = (!fc->writeback_cache) &&
			   ((pos_out + len) > inode_out->i_size);

	if (fc->no_copy_file_range)
		return -EOPNOTSUPP;

	if (file_inode(file_in)->i_sb != file_inode(file_out)->i_sb)
		return -EXDEV;

	inode_lock(inode_in);
	err = fuse_writeback_range(inode_in, pos_in, pos_in + len - 1);
	inode_unlock(inode_in);
	if (err)
		return err;

	inode_lock(inode_out);

	err = file_modified(file_out);
	if (err)
		goto out;

	/*
	 * Write out dirty pages in the destination file before sending the COPY
	 * request to userspace.  After the request is completed, truncate off
	 * pages (including partial ones) from the cache that have been copied,
	 * since these contain stale data at that point.
	 *
	 * This should be mostly correct, but if the COPY writes to partial
	 * pages (at the start or end) and the parts not covered by the COPY are
	 * written through a memory map after calling fuse_writeback_range(),
	 * then these partial page modifications will be lost on truncation.
	 *
	 * It is unlikely that someone would rely on such mixed style
	 * modifications.  Yet this does give less guarantees than if the
	 * copying was performed with write(2).
	 *
	 * To fix this a mapping->invalidate_lock could be used to prevent new
	 * faults while the copy is ongoing.
	 */
	err = fuse_writeback_range(inode_out, pos_out, pos_out + len - 1);
	if (err)
		goto out;

	if (is_unstable)
		set_bit(FUSE_I_SIZE_UNSTABLE, &fi_out->state);

	args.opcode = FUSE_COPY_FILE_RANGE;
	args.nodeid = ff_in->nodeid;
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
	if (err == -ENOSYS) {
		fc->no_copy_file_range = 1;
		err = -EOPNOTSUPP;
	}
	if (err)
		goto out;

	truncate_inode_pages_range(inode_out->i_mapping,
				   ALIGN_DOWN(pos_out, PAGE_SIZE),
				   ALIGN(pos_out + outarg.size, PAGE_SIZE) - 1);

	if (fc->writeback_cache) {
		fuse_write_update_size(inode_out, pos_out + outarg.size);
		file_update_time(file_out);
	}

	fuse_invalidate_attr(inode_out);

	err = outarg.size;
out:
	if (is_unstable)
		clear_bit(FUSE_I_SIZE_UNSTABLE, &fi_out->state);

	inode_unlock(inode_out);
	file_accessed(file_in);

	fuse_flush_time_update(inode_out);

	return err;
}

static ssize_t fuse_copy_file_range(struct file *src_file, loff_t src_off,
				    struct file *dst_file, loff_t dst_off,
				    size_t len, unsigned int flags)
{
	ssize_t ret;

	ret = __fuse_copy_file_range(src_file, src_off, dst_file, dst_off,
				     len, flags);

	if (ret == -EOPNOTSUPP || ret == -EXDEV)
		ret = generic_copy_file_range(src_file, src_off, dst_file,
					      dst_off, len, flags);
	return ret;
}

>>>>>>> upstream/android-13
static const struct file_operations fuse_file_operations = {
	.llseek		= fuse_file_llseek,
	.read_iter	= fuse_file_read_iter,
	.write_iter	= fuse_file_write_iter,
	.mmap		= fuse_file_mmap,
	.open		= fuse_open,
	.flush		= fuse_flush,
	.release	= fuse_release,
	.fsync		= fuse_fsync,
	.lock		= fuse_file_lock,
<<<<<<< HEAD
	.flock		= fuse_file_flock,
	.splice_read	= generic_file_splice_read,
=======
	.get_unmapped_area = thp_get_unmapped_area,
	.flock		= fuse_file_flock,
	.splice_read	= generic_file_splice_read,
	.splice_write	= iter_file_splice_write,
>>>>>>> upstream/android-13
	.unlocked_ioctl	= fuse_file_ioctl,
	.compat_ioctl	= fuse_file_compat_ioctl,
	.poll		= fuse_file_poll,
	.fallocate	= fuse_file_fallocate,
<<<<<<< HEAD
};

static const struct file_operations fuse_direct_io_file_operations = {
	.llseek		= fuse_file_llseek,
	.read_iter	= fuse_direct_read_iter,
	.write_iter	= fuse_direct_write_iter,
	.mmap		= fuse_direct_mmap,
	.open		= fuse_open,
	.flush		= fuse_flush,
	.release	= fuse_release,
	.fsync		= fuse_fsync,
	.lock		= fuse_file_lock,
	.flock		= fuse_file_flock,
	.unlocked_ioctl	= fuse_file_ioctl,
	.compat_ioctl	= fuse_file_compat_ioctl,
	.poll		= fuse_file_poll,
	.fallocate	= fuse_file_fallocate,
	/* no splice_read */
=======
	.copy_file_range = fuse_copy_file_range,
>>>>>>> upstream/android-13
};

static const struct address_space_operations fuse_file_aops  = {
	.readpage	= fuse_readpage,
<<<<<<< HEAD
	.writepage	= fuse_writepage,
	.writepages	= fuse_writepages,
	.launder_page	= fuse_launder_page,
	.readpages	= fuse_readpages,
=======
	.readahead	= fuse_readahead,
	.writepage	= fuse_writepage,
	.writepages	= fuse_writepages,
	.launder_page	= fuse_launder_page,
>>>>>>> upstream/android-13
	.set_page_dirty	= __set_page_dirty_nobuffers,
	.bmap		= fuse_bmap,
	.direct_IO	= fuse_direct_IO,
	.write_begin	= fuse_write_begin,
	.write_end	= fuse_write_end,
};

void fuse_init_file_inode(struct inode *inode)
{
<<<<<<< HEAD
	inode->i_fop = &fuse_file_operations;
	inode->i_data.a_ops = &fuse_file_aops;
=======
	struct fuse_inode *fi = get_fuse_inode(inode);

	inode->i_fop = &fuse_file_operations;
	inode->i_data.a_ops = &fuse_file_aops;

	INIT_LIST_HEAD(&fi->write_files);
	INIT_LIST_HEAD(&fi->queued_writes);
	fi->writectr = 0;
	init_waitqueue_head(&fi->page_waitq);
	fi->writepages = RB_ROOT;

	if (IS_ENABLED(CONFIG_FUSE_DAX))
		fuse_dax_inode_init(inode);
>>>>>>> upstream/android-13
}
