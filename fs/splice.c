<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * "splice": joining two ropes together by interweaving their strands.
 *
 * This is the "extended pipe" functionality, where a pipe is used as
 * an arbitrary in-memory buffer. Think of a pipe as a small kernel
 * buffer that you can use to transfer data from one end to the other.
 *
 * The traditional unix read/write is extended with a "splice()" operation
 * that transfers data buffers to or from a pipe buffer.
 *
 * Named by Larry McVoy, original implementation from Linus, extended by
 * Jens to support splicing to files, network, direct splicing, etc and
 * fixing lots of bugs.
 *
 * Copyright (C) 2005-2006 Jens Axboe <axboe@kernel.dk>
 * Copyright (C) 2005-2006 Linus Torvalds <torvalds@osdl.org>
 * Copyright (C) 2006 Ingo Molnar <mingo@elte.hu>
 *
 */
#include <linux/bvec.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/pagemap.h>
#include <linux/splice.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>
#include <linux/swap.h>
#include <linux/writeback.h>
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/uio.h>
#include <linux/security.h>
#include <linux/gfp.h>
#include <linux/socket.h>
<<<<<<< HEAD
#include <linux/compat.h>
=======
>>>>>>> upstream/android-13
#include <linux/sched/signal.h>

#include "internal.h"

/*
 * Attempt to steal a page from a pipe buffer. This should perhaps go into
 * a vm helper function, it's already simplified quite a bit by the
 * addition of remove_mapping(). If success is returned, the caller may
 * attempt to reuse this page for another destination.
 */
<<<<<<< HEAD
static int page_cache_pipe_buf_steal(struct pipe_inode_info *pipe,
				     struct pipe_buffer *buf)
=======
static bool page_cache_pipe_buf_try_steal(struct pipe_inode_info *pipe,
		struct pipe_buffer *buf)
>>>>>>> upstream/android-13
{
	struct page *page = buf->page;
	struct address_space *mapping;

	lock_page(page);

	mapping = page_mapping(page);
	if (mapping) {
		WARN_ON(!PageUptodate(page));

		/*
		 * At least for ext2 with nobh option, we need to wait on
		 * writeback completing on this page, since we'll remove it
		 * from the pagecache.  Otherwise truncate wont wait on the
		 * page, allowing the disk blocks to be reused by someone else
		 * before we actually wrote our data to them. fs corruption
		 * ensues.
		 */
		wait_on_page_writeback(page);

		if (page_has_private(page) &&
		    !try_to_release_page(page, GFP_KERNEL))
			goto out_unlock;

		/*
		 * If we succeeded in removing the mapping, set LRU flag
		 * and return good.
		 */
		if (remove_mapping(mapping, page)) {
			buf->flags |= PIPE_BUF_FLAG_LRU;
<<<<<<< HEAD
			return 0;
=======
			return true;
>>>>>>> upstream/android-13
		}
	}

	/*
	 * Raced with truncate or failed to remove page from current
	 * address space, unlock and return failure.
	 */
out_unlock:
	unlock_page(page);
<<<<<<< HEAD
	return 1;
=======
	return false;
>>>>>>> upstream/android-13
}

static void page_cache_pipe_buf_release(struct pipe_inode_info *pipe,
					struct pipe_buffer *buf)
{
	put_page(buf->page);
	buf->flags &= ~PIPE_BUF_FLAG_LRU;
}

/*
 * Check whether the contents of buf is OK to access. Since the content
 * is a page cache page, IO may be in flight.
 */
static int page_cache_pipe_buf_confirm(struct pipe_inode_info *pipe,
				       struct pipe_buffer *buf)
{
	struct page *page = buf->page;
	int err;

	if (!PageUptodate(page)) {
		lock_page(page);

		/*
		 * Page got truncated/unhashed. This will cause a 0-byte
		 * splice, if this is the first page.
		 */
		if (!page->mapping) {
			err = -ENODATA;
			goto error;
		}

		/*
		 * Uh oh, read-error from disk.
		 */
		if (!PageUptodate(page)) {
			err = -EIO;
			goto error;
		}

		/*
		 * Page is ok afterall, we are done.
		 */
		unlock_page(page);
	}

	return 0;
error:
	unlock_page(page);
	return err;
}

const struct pipe_buf_operations page_cache_pipe_buf_ops = {
<<<<<<< HEAD
	.can_merge = 0,
	.confirm = page_cache_pipe_buf_confirm,
	.release = page_cache_pipe_buf_release,
	.steal = page_cache_pipe_buf_steal,
	.get = generic_pipe_buf_get,
};

static int user_page_pipe_buf_steal(struct pipe_inode_info *pipe,
				    struct pipe_buffer *buf)
{
	if (!(buf->flags & PIPE_BUF_FLAG_GIFT))
		return 1;

	buf->flags |= PIPE_BUF_FLAG_LRU;
	return generic_pipe_buf_steal(pipe, buf);
}

static const struct pipe_buf_operations user_page_pipe_buf_ops = {
	.can_merge = 0,
	.confirm = generic_pipe_buf_confirm,
	.release = page_cache_pipe_buf_release,
	.steal = user_page_pipe_buf_steal,
	.get = generic_pipe_buf_get,
=======
	.confirm	= page_cache_pipe_buf_confirm,
	.release	= page_cache_pipe_buf_release,
	.try_steal	= page_cache_pipe_buf_try_steal,
	.get		= generic_pipe_buf_get,
};

static bool user_page_pipe_buf_try_steal(struct pipe_inode_info *pipe,
		struct pipe_buffer *buf)
{
	if (!(buf->flags & PIPE_BUF_FLAG_GIFT))
		return false;

	buf->flags |= PIPE_BUF_FLAG_LRU;
	return generic_pipe_buf_try_steal(pipe, buf);
}

static const struct pipe_buf_operations user_page_pipe_buf_ops = {
	.release	= page_cache_pipe_buf_release,
	.try_steal	= user_page_pipe_buf_try_steal,
	.get		= generic_pipe_buf_get,
>>>>>>> upstream/android-13
};

static void wakeup_pipe_readers(struct pipe_inode_info *pipe)
{
	smp_mb();
<<<<<<< HEAD
	if (waitqueue_active(&pipe->wait))
		wake_up_interruptible(&pipe->wait);
=======
	if (waitqueue_active(&pipe->rd_wait))
		wake_up_interruptible(&pipe->rd_wait);
>>>>>>> upstream/android-13
	kill_fasync(&pipe->fasync_readers, SIGIO, POLL_IN);
}

/**
 * splice_to_pipe - fill passed data into a pipe
 * @pipe:	pipe to fill
 * @spd:	data to fill
 *
 * Description:
 *    @spd contains a map of pages and len/offset tuples, along with
 *    the struct pipe_buf_operations associated with these pages. This
 *    function will link that data to the pipe.
 *
 */
ssize_t splice_to_pipe(struct pipe_inode_info *pipe,
		       struct splice_pipe_desc *spd)
{
	unsigned int spd_pages = spd->nr_pages;
<<<<<<< HEAD
=======
	unsigned int tail = pipe->tail;
	unsigned int head = pipe->head;
	unsigned int mask = pipe->ring_size - 1;
>>>>>>> upstream/android-13
	int ret = 0, page_nr = 0;

	if (!spd_pages)
		return 0;

	if (unlikely(!pipe->readers)) {
		send_sig(SIGPIPE, current, 0);
		ret = -EPIPE;
		goto out;
	}

<<<<<<< HEAD
	while (pipe->nrbufs < pipe->buffers) {
		int newbuf = (pipe->curbuf + pipe->nrbufs) & (pipe->buffers - 1);
		struct pipe_buffer *buf = pipe->bufs + newbuf;
=======
	while (!pipe_full(head, tail, pipe->max_usage)) {
		struct pipe_buffer *buf = &pipe->bufs[head & mask];
>>>>>>> upstream/android-13

		buf->page = spd->pages[page_nr];
		buf->offset = spd->partial[page_nr].offset;
		buf->len = spd->partial[page_nr].len;
		buf->private = spd->partial[page_nr].private;
		buf->ops = spd->ops;
		buf->flags = 0;

<<<<<<< HEAD
		pipe->nrbufs++;
=======
		head++;
		pipe->head = head;
>>>>>>> upstream/android-13
		page_nr++;
		ret += buf->len;

		if (!--spd->nr_pages)
			break;
	}

	if (!ret)
		ret = -EAGAIN;

out:
	while (page_nr < spd_pages)
		spd->spd_release(spd, page_nr++);

	return ret;
}
EXPORT_SYMBOL_GPL(splice_to_pipe);

ssize_t add_to_pipe(struct pipe_inode_info *pipe, struct pipe_buffer *buf)
{
<<<<<<< HEAD
=======
	unsigned int head = pipe->head;
	unsigned int tail = pipe->tail;
	unsigned int mask = pipe->ring_size - 1;
>>>>>>> upstream/android-13
	int ret;

	if (unlikely(!pipe->readers)) {
		send_sig(SIGPIPE, current, 0);
		ret = -EPIPE;
<<<<<<< HEAD
	} else if (pipe->nrbufs == pipe->buffers) {
		ret = -EAGAIN;
	} else {
		int newbuf = (pipe->curbuf + pipe->nrbufs) & (pipe->buffers - 1);
		pipe->bufs[newbuf] = *buf;
		pipe->nrbufs++;
=======
	} else if (pipe_full(head, tail, pipe->max_usage)) {
		ret = -EAGAIN;
	} else {
		pipe->bufs[head & mask] = *buf;
		pipe->head = head + 1;
>>>>>>> upstream/android-13
		return buf->len;
	}
	pipe_buf_release(pipe, buf);
	return ret;
}
EXPORT_SYMBOL(add_to_pipe);

/*
 * Check if we need to grow the arrays holding pages and partial page
 * descriptions.
 */
int splice_grow_spd(const struct pipe_inode_info *pipe, struct splice_pipe_desc *spd)
{
<<<<<<< HEAD
	unsigned int buffers = READ_ONCE(pipe->buffers);

	spd->nr_pages_max = buffers;
	if (buffers <= PIPE_DEF_BUFFERS)
		return 0;

	spd->pages = kmalloc_array(buffers, sizeof(struct page *), GFP_KERNEL);
	spd->partial = kmalloc_array(buffers, sizeof(struct partial_page),
=======
	unsigned int max_usage = READ_ONCE(pipe->max_usage);

	spd->nr_pages_max = max_usage;
	if (max_usage <= PIPE_DEF_BUFFERS)
		return 0;

	spd->pages = kmalloc_array(max_usage, sizeof(struct page *), GFP_KERNEL);
	spd->partial = kmalloc_array(max_usage, sizeof(struct partial_page),
>>>>>>> upstream/android-13
				     GFP_KERNEL);

	if (spd->pages && spd->partial)
		return 0;

	kfree(spd->pages);
	kfree(spd->partial);
	return -ENOMEM;
}

void splice_shrink_spd(struct splice_pipe_desc *spd)
{
	if (spd->nr_pages_max <= PIPE_DEF_BUFFERS)
		return;

	kfree(spd->pages);
	kfree(spd->partial);
}

/**
 * generic_file_splice_read - splice data from file to a pipe
 * @in:		file to splice from
 * @ppos:	position in @in
 * @pipe:	pipe to splice to
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    Will read pages from given file and fill them into a pipe. Can be
 *    used as long as it has more or less sane ->read_iter().
 *
 */
ssize_t generic_file_splice_read(struct file *in, loff_t *ppos,
				 struct pipe_inode_info *pipe, size_t len,
				 unsigned int flags)
{
	struct iov_iter to;
	struct kiocb kiocb;
<<<<<<< HEAD
	int idx, ret;

	iov_iter_pipe(&to, ITER_PIPE | READ, pipe, len);
	idx = to.idx;
=======
	unsigned int i_head;
	int ret;

	iov_iter_pipe(&to, READ, pipe, len);
	i_head = to.head;
>>>>>>> upstream/android-13
	init_sync_kiocb(&kiocb, in);
	kiocb.ki_pos = *ppos;
	ret = call_read_iter(in, &kiocb, &to);
	if (ret > 0) {
		*ppos = kiocb.ki_pos;
		file_accessed(in);
	} else if (ret < 0) {
<<<<<<< HEAD
		to.idx = idx;
=======
		to.head = i_head;
>>>>>>> upstream/android-13
		to.iov_offset = 0;
		iov_iter_advance(&to, 0); /* to free what was emitted */
		/*
		 * callers of ->splice_read() expect -EAGAIN on
		 * "can't put anything in there", rather than -EFAULT.
		 */
		if (ret == -EFAULT)
			ret = -EAGAIN;
	}

	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL(generic_file_splice_read);

const struct pipe_buf_operations default_pipe_buf_ops = {
	.can_merge = 0,
	.confirm = generic_pipe_buf_confirm,
	.release = generic_pipe_buf_release,
	.steal = generic_pipe_buf_steal,
	.get = generic_pipe_buf_get,
};

int generic_pipe_buf_nosteal(struct pipe_inode_info *pipe,
			     struct pipe_buffer *buf)
{
	return 1;
}

/* Pipe buffer operations for a socket and similar. */
const struct pipe_buf_operations nosteal_pipe_buf_ops = {
	.can_merge = 0,
	.confirm = generic_pipe_buf_confirm,
	.release = generic_pipe_buf_release,
	.steal = generic_pipe_buf_nosteal,
	.get = generic_pipe_buf_get,
};
EXPORT_SYMBOL(nosteal_pipe_buf_ops);

static ssize_t kernel_readv(struct file *file, const struct kvec *vec,
			    unsigned long vlen, loff_t offset)
{
	mm_segment_t old_fs;
	loff_t pos = offset;
	ssize_t res;

	old_fs = get_fs();
	set_fs(get_ds());
	/* The cast to a user pointer is valid due to the set_fs() */
	res = vfs_readv(file, (const struct iovec __user *)vec, vlen, &pos, 0);
	set_fs(old_fs);

	return res;
}

static ssize_t default_file_splice_read(struct file *in, loff_t *ppos,
				 struct pipe_inode_info *pipe, size_t len,
				 unsigned int flags)
{
	struct kvec *vec, __vec[PIPE_DEF_BUFFERS];
	struct iov_iter to;
	struct page **pages;
	unsigned int nr_pages;
	size_t offset, base, copied = 0;
	ssize_t res;
	int i;

	if (pipe->nrbufs == pipe->buffers)
		return -EAGAIN;

	/*
	 * Try to keep page boundaries matching to source pagecache ones -
	 * it probably won't be much help, but...
	 */
	offset = *ppos & ~PAGE_MASK;

	iov_iter_pipe(&to, ITER_PIPE | READ, pipe, len + offset);

	res = iov_iter_get_pages_alloc(&to, &pages, len + offset, &base);
	if (res <= 0)
		return -ENOMEM;

	nr_pages = DIV_ROUND_UP(res + base, PAGE_SIZE);

	vec = __vec;
	if (nr_pages > PIPE_DEF_BUFFERS) {
		vec = kmalloc_array(nr_pages, sizeof(struct kvec), GFP_KERNEL);
		if (unlikely(!vec)) {
			res = -ENOMEM;
			goto out;
		}
	}

	pipe->bufs[to.idx].offset = offset;
	pipe->bufs[to.idx].len -= offset;

	for (i = 0; i < nr_pages; i++) {
		size_t this_len = min_t(size_t, len, PAGE_SIZE - offset);
		vec[i].iov_base = page_address(pages[i]) + offset;
		vec[i].iov_len = this_len;
		len -= this_len;
		offset = 0;
	}

	res = kernel_readv(in, vec, nr_pages, *ppos);
	if (res > 0) {
		copied = res;
		*ppos += res;
	}

	if (vec != __vec)
		kfree(vec);
out:
	for (i = 0; i < nr_pages; i++)
		put_page(pages[i]);
	kvfree(pages);
	iov_iter_advance(&to, copied);	/* truncates and discards */
	return res;
}

=======
EXPORT_SYMBOL_NS(generic_file_splice_read, ANDROID_GKI_VFS_EXPORT_ONLY);

const struct pipe_buf_operations default_pipe_buf_ops = {
	.release	= generic_pipe_buf_release,
	.try_steal	= generic_pipe_buf_try_steal,
	.get		= generic_pipe_buf_get,
};

/* Pipe buffer operations for a socket and similar. */
const struct pipe_buf_operations nosteal_pipe_buf_ops = {
	.release	= generic_pipe_buf_release,
	.get		= generic_pipe_buf_get,
};
EXPORT_SYMBOL(nosteal_pipe_buf_ops);

>>>>>>> upstream/android-13
/*
 * Send 'sd->len' bytes to socket from 'sd->file' at position 'sd->pos'
 * using sendpage(). Return the number of bytes sent.
 */
static int pipe_to_sendpage(struct pipe_inode_info *pipe,
			    struct pipe_buffer *buf, struct splice_desc *sd)
{
	struct file *file = sd->u.file;
	loff_t pos = sd->pos;
	int more;

	if (!likely(file->f_op->sendpage))
		return -EINVAL;

	more = (sd->flags & SPLICE_F_MORE) ? MSG_MORE : 0;

<<<<<<< HEAD
	if (sd->len < sd->total_len && pipe->nrbufs > 1)
=======
	if (sd->len < sd->total_len &&
	    pipe_occupancy(pipe->head, pipe->tail) > 1)
>>>>>>> upstream/android-13
		more |= MSG_SENDPAGE_NOTLAST;

	return file->f_op->sendpage(file, buf->page, buf->offset,
				    sd->len, &pos, more);
}

static void wakeup_pipe_writers(struct pipe_inode_info *pipe)
{
	smp_mb();
<<<<<<< HEAD
	if (waitqueue_active(&pipe->wait))
		wake_up_interruptible(&pipe->wait);
=======
	if (waitqueue_active(&pipe->wr_wait))
		wake_up_interruptible(&pipe->wr_wait);
>>>>>>> upstream/android-13
	kill_fasync(&pipe->fasync_writers, SIGIO, POLL_OUT);
}

/**
 * splice_from_pipe_feed - feed available data from a pipe to a file
 * @pipe:	pipe to splice from
 * @sd:		information to @actor
 * @actor:	handler that splices the data
 *
 * Description:
 *    This function loops over the pipe and calls @actor to do the
 *    actual moving of a single struct pipe_buffer to the desired
 *    destination.  It returns when there's no more buffers left in
 *    the pipe or if the requested number of bytes (@sd->total_len)
 *    have been copied.  It returns a positive number (one) if the
 *    pipe needs to be filled with more data, zero if the required
 *    number of bytes have been copied and -errno on error.
 *
 *    This, together with splice_from_pipe_{begin,end,next}, may be
 *    used to implement the functionality of __splice_from_pipe() when
 *    locking is required around copying the pipe buffers to the
 *    destination.
 */
static int splice_from_pipe_feed(struct pipe_inode_info *pipe, struct splice_desc *sd,
			  splice_actor *actor)
{
<<<<<<< HEAD
	int ret;

	while (pipe->nrbufs) {
		struct pipe_buffer *buf = pipe->bufs + pipe->curbuf;
=======
	unsigned int head = pipe->head;
	unsigned int tail = pipe->tail;
	unsigned int mask = pipe->ring_size - 1;
	int ret;

	while (!pipe_empty(head, tail)) {
		struct pipe_buffer *buf = &pipe->bufs[tail & mask];
>>>>>>> upstream/android-13

		sd->len = buf->len;
		if (sd->len > sd->total_len)
			sd->len = sd->total_len;

		ret = pipe_buf_confirm(pipe, buf);
		if (unlikely(ret)) {
			if (ret == -ENODATA)
				ret = 0;
			return ret;
		}

		ret = actor(pipe, buf, sd);
		if (ret <= 0)
			return ret;

		buf->offset += ret;
		buf->len -= ret;

		sd->num_spliced += ret;
		sd->len -= ret;
		sd->pos += ret;
		sd->total_len -= ret;

		if (!buf->len) {
			pipe_buf_release(pipe, buf);
<<<<<<< HEAD
			pipe->curbuf = (pipe->curbuf + 1) & (pipe->buffers - 1);
			pipe->nrbufs--;
=======
			tail++;
			pipe->tail = tail;
>>>>>>> upstream/android-13
			if (pipe->files)
				sd->need_wakeup = true;
		}

		if (!sd->total_len)
			return 0;
	}

	return 1;
}

<<<<<<< HEAD
=======
/* We know we have a pipe buffer, but maybe it's empty? */
static inline bool eat_empty_buffer(struct pipe_inode_info *pipe)
{
	unsigned int tail = pipe->tail;
	unsigned int mask = pipe->ring_size - 1;
	struct pipe_buffer *buf = &pipe->bufs[tail & mask];

	if (unlikely(!buf->len)) {
		pipe_buf_release(pipe, buf);
		pipe->tail = tail+1;
		return true;
	}

	return false;
}

>>>>>>> upstream/android-13
/**
 * splice_from_pipe_next - wait for some data to splice from
 * @pipe:	pipe to splice from
 * @sd:		information about the splice operation
 *
 * Description:
 *    This function will wait for some data and return a positive
 *    value (one) if pipe buffers are available.  It will return zero
 *    or -errno if no more data needs to be spliced.
 */
static int splice_from_pipe_next(struct pipe_inode_info *pipe, struct splice_desc *sd)
{
	/*
	 * Check for signal early to make process killable when there are
	 * always buffers available
	 */
	if (signal_pending(current))
		return -ERESTARTSYS;

<<<<<<< HEAD
	while (!pipe->nrbufs) {
		if (!pipe->writers)
			return 0;

		if (!pipe->waiting_writers && sd->num_spliced)
=======
repeat:
	while (pipe_empty(pipe->head, pipe->tail)) {
		if (!pipe->writers)
			return 0;

		if (sd->num_spliced)
>>>>>>> upstream/android-13
			return 0;

		if (sd->flags & SPLICE_F_NONBLOCK)
			return -EAGAIN;

		if (signal_pending(current))
			return -ERESTARTSYS;

		if (sd->need_wakeup) {
			wakeup_pipe_writers(pipe);
			sd->need_wakeup = false;
		}

<<<<<<< HEAD
		pipe_wait(pipe);
	}

=======
		pipe_wait_readable(pipe);
	}

	if (eat_empty_buffer(pipe))
		goto repeat;

>>>>>>> upstream/android-13
	return 1;
}

/**
 * splice_from_pipe_begin - start splicing from pipe
 * @sd:		information about the splice operation
 *
 * Description:
 *    This function should be called before a loop containing
 *    splice_from_pipe_next() and splice_from_pipe_feed() to
 *    initialize the necessary fields of @sd.
 */
static void splice_from_pipe_begin(struct splice_desc *sd)
{
	sd->num_spliced = 0;
	sd->need_wakeup = false;
}

/**
 * splice_from_pipe_end - finish splicing from pipe
 * @pipe:	pipe to splice from
 * @sd:		information about the splice operation
 *
 * Description:
 *    This function will wake up pipe writers if necessary.  It should
 *    be called after a loop containing splice_from_pipe_next() and
 *    splice_from_pipe_feed().
 */
static void splice_from_pipe_end(struct pipe_inode_info *pipe, struct splice_desc *sd)
{
	if (sd->need_wakeup)
		wakeup_pipe_writers(pipe);
}

/**
 * __splice_from_pipe - splice data from a pipe to given actor
 * @pipe:	pipe to splice from
 * @sd:		information to @actor
 * @actor:	handler that splices the data
 *
 * Description:
 *    This function does little more than loop over the pipe and call
 *    @actor to do the actual moving of a single struct pipe_buffer to
 *    the desired destination. See pipe_to_file, pipe_to_sendpage, or
 *    pipe_to_user.
 *
 */
ssize_t __splice_from_pipe(struct pipe_inode_info *pipe, struct splice_desc *sd,
			   splice_actor *actor)
{
	int ret;

	splice_from_pipe_begin(sd);
	do {
		cond_resched();
		ret = splice_from_pipe_next(pipe, sd);
		if (ret > 0)
			ret = splice_from_pipe_feed(pipe, sd, actor);
	} while (ret > 0);
	splice_from_pipe_end(pipe, sd);

	return sd->num_spliced ? sd->num_spliced : ret;
}
EXPORT_SYMBOL(__splice_from_pipe);

/**
 * splice_from_pipe - splice data from a pipe to a file
 * @pipe:	pipe to splice from
 * @out:	file to splice to
 * @ppos:	position in @out
 * @len:	how many bytes to splice
 * @flags:	splice modifier flags
 * @actor:	handler that splices the data
 *
 * Description:
 *    See __splice_from_pipe. This function locks the pipe inode,
 *    otherwise it's identical to __splice_from_pipe().
 *
 */
ssize_t splice_from_pipe(struct pipe_inode_info *pipe, struct file *out,
			 loff_t *ppos, size_t len, unsigned int flags,
			 splice_actor *actor)
{
	ssize_t ret;
	struct splice_desc sd = {
		.total_len = len,
		.flags = flags,
		.pos = *ppos,
		.u.file = out,
	};

	pipe_lock(pipe);
	ret = __splice_from_pipe(pipe, &sd, actor);
	pipe_unlock(pipe);

	return ret;
}

/**
 * iter_file_splice_write - splice data from a pipe to a file
 * @pipe:	pipe info
 * @out:	file to write to
 * @ppos:	position in @out
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    Will either move or copy pages (determined by @flags options) from
 *    the given pipe inode to the given file.
 *    This one is ->write_iter-based.
 *
 */
ssize_t
iter_file_splice_write(struct pipe_inode_info *pipe, struct file *out,
			  loff_t *ppos, size_t len, unsigned int flags)
{
	struct splice_desc sd = {
		.total_len = len,
		.flags = flags,
		.pos = *ppos,
		.u.file = out,
	};
<<<<<<< HEAD
	int nbufs = pipe->buffers;
=======
	int nbufs = pipe->max_usage;
>>>>>>> upstream/android-13
	struct bio_vec *array = kcalloc(nbufs, sizeof(struct bio_vec),
					GFP_KERNEL);
	ssize_t ret;

	if (unlikely(!array))
		return -ENOMEM;

	pipe_lock(pipe);

	splice_from_pipe_begin(&sd);
	while (sd.total_len) {
		struct iov_iter from;
<<<<<<< HEAD
		size_t left;
		int n, idx;
=======
		unsigned int head, tail, mask;
		size_t left;
		int n;
>>>>>>> upstream/android-13

		ret = splice_from_pipe_next(pipe, &sd);
		if (ret <= 0)
			break;

<<<<<<< HEAD
		if (unlikely(nbufs < pipe->buffers)) {
			kfree(array);
			nbufs = pipe->buffers;
=======
		if (unlikely(nbufs < pipe->max_usage)) {
			kfree(array);
			nbufs = pipe->max_usage;
>>>>>>> upstream/android-13
			array = kcalloc(nbufs, sizeof(struct bio_vec),
					GFP_KERNEL);
			if (!array) {
				ret = -ENOMEM;
				break;
			}
		}

<<<<<<< HEAD
		/* build the vector */
		left = sd.total_len;
		for (n = 0, idx = pipe->curbuf; left && n < pipe->nrbufs; n++, idx++) {
			struct pipe_buffer *buf = pipe->bufs + idx;
			size_t this_len = buf->len;

			if (this_len > left)
				this_len = left;

			if (idx == pipe->buffers - 1)
				idx = -1;
=======
		head = pipe->head;
		tail = pipe->tail;
		mask = pipe->ring_size - 1;

		/* build the vector */
		left = sd.total_len;
		for (n = 0; !pipe_empty(head, tail) && left && n < nbufs; tail++) {
			struct pipe_buffer *buf = &pipe->bufs[tail & mask];
			size_t this_len = buf->len;

			/* zero-length bvecs are not supported, skip them */
			if (!this_len)
				continue;
			this_len = min(this_len, left);
>>>>>>> upstream/android-13

			ret = pipe_buf_confirm(pipe, buf);
			if (unlikely(ret)) {
				if (ret == -ENODATA)
					ret = 0;
				goto done;
			}

			array[n].bv_page = buf->page;
			array[n].bv_len = this_len;
			array[n].bv_offset = buf->offset;
			left -= this_len;
<<<<<<< HEAD
		}

		iov_iter_bvec(&from, ITER_BVEC | WRITE, array, n,
			      sd.total_len - left);
=======
			n++;
		}

		iov_iter_bvec(&from, WRITE, array, n, sd.total_len - left);
>>>>>>> upstream/android-13
		ret = vfs_iter_write(out, &from, &sd.pos, 0);
		if (ret <= 0)
			break;

		sd.num_spliced += ret;
		sd.total_len -= ret;
		*ppos = sd.pos;

		/* dismiss the fully eaten buffers, adjust the partial one */
<<<<<<< HEAD
		while (ret) {
			struct pipe_buffer *buf = pipe->bufs + pipe->curbuf;
=======
		tail = pipe->tail;
		while (ret) {
			struct pipe_buffer *buf = &pipe->bufs[tail & mask];
>>>>>>> upstream/android-13
			if (ret >= buf->len) {
				ret -= buf->len;
				buf->len = 0;
				pipe_buf_release(pipe, buf);
<<<<<<< HEAD
				pipe->curbuf = (pipe->curbuf + 1) & (pipe->buffers - 1);
				pipe->nrbufs--;
=======
				tail++;
				pipe->tail = tail;
>>>>>>> upstream/android-13
				if (pipe->files)
					sd.need_wakeup = true;
			} else {
				buf->offset += ret;
				buf->len -= ret;
				ret = 0;
			}
		}
	}
done:
	kfree(array);
	splice_from_pipe_end(pipe, &sd);

	pipe_unlock(pipe);

	if (sd.num_spliced)
		ret = sd.num_spliced;

	return ret;
}

<<<<<<< HEAD
EXPORT_SYMBOL(iter_file_splice_write);

static int write_pipe_buf(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
			  struct splice_desc *sd)
{
	int ret;
	void *data;
	loff_t tmp = sd->pos;

	data = kmap(buf->page);
	ret = __kernel_write(sd->u.file, data + buf->offset, sd->len, &tmp);
	kunmap(buf->page);

	return ret;
}

static ssize_t default_file_splice_write(struct pipe_inode_info *pipe,
					 struct file *out, loff_t *ppos,
					 size_t len, unsigned int flags)
{
	ssize_t ret;

	ret = splice_from_pipe(pipe, out, ppos, len, flags, write_pipe_buf);
	if (ret > 0)
		*ppos += ret;

	return ret;
}
=======
EXPORT_SYMBOL_NS(iter_file_splice_write, ANDROID_GKI_VFS_EXPORT_ONLY);
>>>>>>> upstream/android-13

/**
 * generic_splice_sendpage - splice data from a pipe to a socket
 * @pipe:	pipe to splice from
 * @out:	socket to write to
 * @ppos:	position in @out
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    Will send @len bytes from the pipe to a network socket. No data copying
 *    is involved.
 *
 */
ssize_t generic_splice_sendpage(struct pipe_inode_info *pipe, struct file *out,
				loff_t *ppos, size_t len, unsigned int flags)
{
	return splice_from_pipe(pipe, out, ppos, len, flags, pipe_to_sendpage);
}

EXPORT_SYMBOL(generic_splice_sendpage);

<<<<<<< HEAD
=======
static int warn_unsupported(struct file *file, const char *op)
{
	pr_debug_ratelimited(
		"splice %s not supported for file %pD4 (pid: %d comm: %.20s)\n",
		op, file, current->pid, current->comm);
	return -EINVAL;
}

>>>>>>> upstream/android-13
/*
 * Attempt to initiate a splice from pipe to file.
 */
static long do_splice_from(struct pipe_inode_info *pipe, struct file *out,
			   loff_t *ppos, size_t len, unsigned int flags)
{
<<<<<<< HEAD
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *,
				loff_t *, size_t, unsigned int);

	if (out->f_op->splice_write)
		splice_write = out->f_op->splice_write;
	else
		splice_write = default_file_splice_write;

	return splice_write(pipe, out, ppos, len, flags);
=======
	if (unlikely(!out->f_op->splice_write))
		return warn_unsupported(out, "write");
	return out->f_op->splice_write(pipe, out, ppos, len, flags);
>>>>>>> upstream/android-13
}

/*
 * Attempt to initiate a splice from a file to a pipe.
 */
static long do_splice_to(struct file *in, loff_t *ppos,
			 struct pipe_inode_info *pipe, size_t len,
			 unsigned int flags)
{
<<<<<<< HEAD
	ssize_t (*splice_read)(struct file *, loff_t *,
			       struct pipe_inode_info *, size_t, unsigned int);
=======
	unsigned int p_space;
>>>>>>> upstream/android-13
	int ret;

	if (unlikely(!(in->f_mode & FMODE_READ)))
		return -EBADF;

<<<<<<< HEAD
=======
	/* Don't try to read more the pipe has space for. */
	p_space = pipe->max_usage - pipe_occupancy(pipe->head, pipe->tail);
	len = min_t(size_t, len, p_space << PAGE_SHIFT);

>>>>>>> upstream/android-13
	ret = rw_verify_area(READ, in, ppos, len);
	if (unlikely(ret < 0))
		return ret;

	if (unlikely(len > MAX_RW_COUNT))
		len = MAX_RW_COUNT;

<<<<<<< HEAD
	if (in->f_op->splice_read)
		splice_read = in->f_op->splice_read;
	else
		splice_read = default_file_splice_read;

	return splice_read(in, ppos, pipe, len, flags);
=======
	if (unlikely(!in->f_op->splice_read))
		return warn_unsupported(in, "read");
	return in->f_op->splice_read(in, ppos, pipe, len, flags);
>>>>>>> upstream/android-13
}

/**
 * splice_direct_to_actor - splices data directly between two non-pipes
 * @in:		file to splice from
 * @sd:		actor information on where to splice to
 * @actor:	handles the data splicing
 *
 * Description:
 *    This is a special case helper to splice directly between two
 *    points, without requiring an explicit pipe. Internally an allocated
 *    pipe is cached in the process, and reused during the lifetime of
 *    that process.
 *
 */
ssize_t splice_direct_to_actor(struct file *in, struct splice_desc *sd,
			       splice_direct_actor *actor)
{
	struct pipe_inode_info *pipe;
	long ret, bytes;
	umode_t i_mode;
	size_t len;
	int i, flags, more;

	/*
	 * We require the input being a regular file, as we don't want to
	 * randomly drop data for eg socket -> socket splicing. Use the
	 * piped splicing for that!
	 */
	i_mode = file_inode(in)->i_mode;
	if (unlikely(!S_ISREG(i_mode) && !S_ISBLK(i_mode)))
		return -EINVAL;

	/*
	 * neither in nor out is a pipe, setup an internal pipe attached to
	 * 'out' and transfer the wanted data from 'in' to 'out' through that
	 */
	pipe = current->splice_pipe;
	if (unlikely(!pipe)) {
		pipe = alloc_pipe_info();
		if (!pipe)
			return -ENOMEM;

		/*
		 * We don't have an immediate reader, but we'll read the stuff
		 * out of the pipe right after the splice_to_pipe(). So set
		 * PIPE_READERS appropriately.
		 */
		pipe->readers = 1;

		current->splice_pipe = pipe;
	}

	/*
	 * Do the splice.
	 */
	ret = 0;
	bytes = 0;
	len = sd->total_len;
	flags = sd->flags;

	/*
	 * Don't block on output, we have to drain the direct pipe.
	 */
	sd->flags &= ~SPLICE_F_NONBLOCK;
	more = sd->flags & SPLICE_F_MORE;

<<<<<<< HEAD
	WARN_ON_ONCE(pipe->nrbufs != 0);

	while (len) {
		unsigned int pipe_pages;
		size_t read_len;
		loff_t pos = sd->pos, prev_pos = pos;

		/* Don't try to read more the pipe has space for. */
		pipe_pages = pipe->buffers - pipe->nrbufs;
		read_len = min(len, (size_t)pipe_pages << PAGE_SHIFT);
		ret = do_splice_to(in, &pos, pipe, read_len, flags);
=======
	WARN_ON_ONCE(!pipe_empty(pipe->head, pipe->tail));

	while (len) {
		size_t read_len;
		loff_t pos = sd->pos, prev_pos = pos;

		ret = do_splice_to(in, &pos, pipe, len, flags);
>>>>>>> upstream/android-13
		if (unlikely(ret <= 0))
			goto out_release;

		read_len = ret;
		sd->total_len = read_len;

		/*
		 * If more data is pending, set SPLICE_F_MORE
		 * If this is the last data and SPLICE_F_MORE was not set
		 * initially, clears it.
		 */
		if (read_len < len)
			sd->flags |= SPLICE_F_MORE;
		else if (!more)
			sd->flags &= ~SPLICE_F_MORE;
		/*
		 * NOTE: nonblocking mode only applies to the input. We
		 * must not do the output in nonblocking mode as then we
		 * could get stuck data in the internal pipe:
		 */
		ret = actor(pipe, sd);
		if (unlikely(ret <= 0)) {
			sd->pos = prev_pos;
			goto out_release;
		}

		bytes += ret;
		len -= ret;
		sd->pos = pos;

		if (ret < read_len) {
			sd->pos = prev_pos + ret;
			goto out_release;
		}
	}

done:
<<<<<<< HEAD
	pipe->nrbufs = pipe->curbuf = 0;
=======
	pipe->tail = pipe->head = 0;
>>>>>>> upstream/android-13
	file_accessed(in);
	return bytes;

out_release:
	/*
	 * If we did an incomplete transfer we must release
	 * the pipe buffers in question:
	 */
<<<<<<< HEAD
	for (i = 0; i < pipe->buffers; i++) {
		struct pipe_buffer *buf = pipe->bufs + i;
=======
	for (i = 0; i < pipe->ring_size; i++) {
		struct pipe_buffer *buf = &pipe->bufs[i];
>>>>>>> upstream/android-13

		if (buf->ops)
			pipe_buf_release(pipe, buf);
	}

	if (!bytes)
		bytes = ret;

	goto done;
}
EXPORT_SYMBOL(splice_direct_to_actor);

static int direct_splice_actor(struct pipe_inode_info *pipe,
			       struct splice_desc *sd)
{
	struct file *file = sd->u.file;

	return do_splice_from(pipe, file, sd->opos, sd->total_len,
			      sd->flags);
}

/**
 * do_splice_direct - splices data directly between two files
 * @in:		file to splice from
 * @ppos:	input file offset
 * @out:	file to splice to
 * @opos:	output file offset
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    For use by do_sendfile(). splice can easily emulate sendfile, but
 *    doing it in the application would incur an extra system call
 *    (splice in + splice out, as compared to just sendfile()). So this helper
 *    can splice directly through a process-private pipe.
 *
 */
long do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
		      loff_t *opos, size_t len, unsigned int flags)
{
	struct splice_desc sd = {
		.len		= len,
		.total_len	= len,
		.flags		= flags,
		.pos		= *ppos,
		.u.file		= out,
		.opos		= opos,
	};
	long ret;

	if (unlikely(!(out->f_mode & FMODE_WRITE)))
		return -EBADF;

	if (unlikely(out->f_flags & O_APPEND))
		return -EINVAL;

	ret = rw_verify_area(WRITE, out, opos, len);
	if (unlikely(ret < 0))
		return ret;

	ret = splice_direct_to_actor(in, &sd, direct_splice_actor);
	if (ret > 0)
		*ppos = sd.pos;

	return ret;
}
EXPORT_SYMBOL(do_splice_direct);

static int wait_for_space(struct pipe_inode_info *pipe, unsigned flags)
{
	for (;;) {
		if (unlikely(!pipe->readers)) {
			send_sig(SIGPIPE, current, 0);
			return -EPIPE;
		}
<<<<<<< HEAD
		if (pipe->nrbufs != pipe->buffers)
=======
		if (!pipe_full(pipe->head, pipe->tail, pipe->max_usage))
>>>>>>> upstream/android-13
			return 0;
		if (flags & SPLICE_F_NONBLOCK)
			return -EAGAIN;
		if (signal_pending(current))
			return -ERESTARTSYS;
<<<<<<< HEAD
		pipe->waiting_writers++;
		pipe_wait(pipe);
		pipe->waiting_writers--;
=======
		pipe_wait_writable(pipe);
>>>>>>> upstream/android-13
	}
}

static int splice_pipe_to_pipe(struct pipe_inode_info *ipipe,
			       struct pipe_inode_info *opipe,
			       size_t len, unsigned int flags);

<<<<<<< HEAD
/*
 * Determine where to splice to/from.
 */
static long do_splice(struct file *in, loff_t __user *off_in,
		      struct file *out, loff_t __user *off_out,
		      size_t len, unsigned int flags)
=======
long splice_file_to_pipe(struct file *in,
			 struct pipe_inode_info *opipe,
			 loff_t *offset,
			 size_t len, unsigned int flags)
{
	long ret;

	pipe_lock(opipe);
	ret = wait_for_space(opipe, flags);
	if (!ret)
		ret = do_splice_to(in, offset, opipe, len, flags);
	pipe_unlock(opipe);
	if (ret > 0)
		wakeup_pipe_readers(opipe);
	return ret;
}

/*
 * Determine where to splice to/from.
 */
long do_splice(struct file *in, loff_t *off_in, struct file *out,
	       loff_t *off_out, size_t len, unsigned int flags)
>>>>>>> upstream/android-13
{
	struct pipe_inode_info *ipipe;
	struct pipe_inode_info *opipe;
	loff_t offset;
	long ret;

<<<<<<< HEAD
	ipipe = get_pipe_info(in);
	opipe = get_pipe_info(out);
=======
	if (unlikely(!(in->f_mode & FMODE_READ) ||
		     !(out->f_mode & FMODE_WRITE)))
		return -EBADF;

	ipipe = get_pipe_info(in, true);
	opipe = get_pipe_info(out, true);
>>>>>>> upstream/android-13

	if (ipipe && opipe) {
		if (off_in || off_out)
			return -ESPIPE;

<<<<<<< HEAD
		if (!(in->f_mode & FMODE_READ))
			return -EBADF;

		if (!(out->f_mode & FMODE_WRITE))
			return -EBADF;

=======
>>>>>>> upstream/android-13
		/* Splicing to self would be fun, but... */
		if (ipipe == opipe)
			return -EINVAL;

<<<<<<< HEAD
=======
		if ((in->f_flags | out->f_flags) & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

>>>>>>> upstream/android-13
		return splice_pipe_to_pipe(ipipe, opipe, len, flags);
	}

	if (ipipe) {
		if (off_in)
			return -ESPIPE;
		if (off_out) {
			if (!(out->f_mode & FMODE_PWRITE))
				return -EINVAL;
<<<<<<< HEAD
			if (copy_from_user(&offset, off_out, sizeof(loff_t)))
				return -EFAULT;
=======
			offset = *off_out;
>>>>>>> upstream/android-13
		} else {
			offset = out->f_pos;
		}

<<<<<<< HEAD
		if (unlikely(!(out->f_mode & FMODE_WRITE)))
			return -EBADF;

=======
>>>>>>> upstream/android-13
		if (unlikely(out->f_flags & O_APPEND))
			return -EINVAL;

		ret = rw_verify_area(WRITE, out, &offset, len);
		if (unlikely(ret < 0))
			return ret;

<<<<<<< HEAD
=======
		if (in->f_flags & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

>>>>>>> upstream/android-13
		file_start_write(out);
		ret = do_splice_from(ipipe, out, &offset, len, flags);
		file_end_write(out);

		if (!off_out)
			out->f_pos = offset;
<<<<<<< HEAD
		else if (copy_to_user(off_out, &offset, sizeof(loff_t)))
			ret = -EFAULT;
=======
		else
			*off_out = offset;
>>>>>>> upstream/android-13

		return ret;
	}

	if (opipe) {
		if (off_out)
			return -ESPIPE;
		if (off_in) {
			if (!(in->f_mode & FMODE_PREAD))
				return -EINVAL;
<<<<<<< HEAD
			if (copy_from_user(&offset, off_in, sizeof(loff_t)))
				return -EFAULT;
=======
			offset = *off_in;
>>>>>>> upstream/android-13
		} else {
			offset = in->f_pos;
		}

<<<<<<< HEAD
		pipe_lock(opipe);
		ret = wait_for_space(opipe, flags);
		if (!ret) {
			unsigned int pipe_pages;

			/* Don't try to read more the pipe has space for. */
			pipe_pages = opipe->buffers - opipe->nrbufs;
			len = min(len, (size_t)pipe_pages << PAGE_SHIFT);

			ret = do_splice_to(in, &offset, opipe, len, flags);
		}
		pipe_unlock(opipe);
		if (ret > 0)
			wakeup_pipe_readers(opipe);
		if (!off_in)
			in->f_pos = offset;
		else if (copy_to_user(off_in, &offset, sizeof(loff_t)))
			ret = -EFAULT;
=======
		if (out->f_flags & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

		ret = splice_file_to_pipe(in, opipe, &offset, len, flags);
		if (!off_in)
			in->f_pos = offset;
		else
			*off_in = offset;
>>>>>>> upstream/android-13

		return ret;
	}

	return -EINVAL;
}

<<<<<<< HEAD
=======
static long __do_splice(struct file *in, loff_t __user *off_in,
			struct file *out, loff_t __user *off_out,
			size_t len, unsigned int flags)
{
	struct pipe_inode_info *ipipe;
	struct pipe_inode_info *opipe;
	loff_t offset, *__off_in = NULL, *__off_out = NULL;
	long ret;

	ipipe = get_pipe_info(in, true);
	opipe = get_pipe_info(out, true);

	if (ipipe && off_in)
		return -ESPIPE;
	if (opipe && off_out)
		return -ESPIPE;

	if (off_out) {
		if (copy_from_user(&offset, off_out, sizeof(loff_t)))
			return -EFAULT;
		__off_out = &offset;
	}
	if (off_in) {
		if (copy_from_user(&offset, off_in, sizeof(loff_t)))
			return -EFAULT;
		__off_in = &offset;
	}

	ret = do_splice(in, __off_in, out, __off_out, len, flags);
	if (ret < 0)
		return ret;

	if (__off_out && copy_to_user(off_out, __off_out, sizeof(loff_t)))
		return -EFAULT;
	if (__off_in && copy_to_user(off_in, __off_in, sizeof(loff_t)))
		return -EFAULT;

	return ret;
}

>>>>>>> upstream/android-13
static int iter_to_pipe(struct iov_iter *from,
			struct pipe_inode_info *pipe,
			unsigned flags)
{
	struct pipe_buffer buf = {
		.ops = &user_page_pipe_buf_ops,
		.flags = flags
	};
	size_t total = 0;
	int ret = 0;
	bool failed = false;

	while (iov_iter_count(from) && !failed) {
		struct page *pages[16];
		ssize_t copied;
		size_t start;
		int n;

		copied = iov_iter_get_pages(from, pages, ~0UL, 16, &start);
		if (copied <= 0) {
			ret = copied;
			break;
		}

		for (n = 0; copied; n++, start = 0) {
			int size = min_t(int, copied, PAGE_SIZE - start);
			if (!failed) {
				buf.page = pages[n];
				buf.offset = start;
				buf.len = size;
				ret = add_to_pipe(pipe, &buf);
				if (unlikely(ret < 0)) {
					failed = true;
				} else {
					iov_iter_advance(from, ret);
					total += ret;
				}
			} else {
				put_page(pages[n]);
			}
			copied -= size;
		}
	}
	return total ? total : ret;
}

static int pipe_to_user(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
			struct splice_desc *sd)
{
	int n = copy_page_to_iter(buf->page, buf->offset, sd->len, sd->u.data);
	return n == sd->len ? n : -EFAULT;
}

/*
 * For lack of a better implementation, implement vmsplice() to userspace
 * as a simple copy of the pipes pages to the user iov.
 */
static long vmsplice_to_user(struct file *file, struct iov_iter *iter,
			     unsigned int flags)
{
<<<<<<< HEAD
	struct pipe_inode_info *pipe = get_pipe_info(file);
=======
	struct pipe_inode_info *pipe = get_pipe_info(file, true);
>>>>>>> upstream/android-13
	struct splice_desc sd = {
		.total_len = iov_iter_count(iter),
		.flags = flags,
		.u.data = iter
	};
	long ret = 0;

	if (!pipe)
		return -EBADF;

	if (sd.total_len) {
		pipe_lock(pipe);
		ret = __splice_from_pipe(pipe, &sd, pipe_to_user);
		pipe_unlock(pipe);
	}

	return ret;
}

/*
 * vmsplice splices a user address range into a pipe. It can be thought of
 * as splice-from-memory, where the regular splice is splice-from-file (or
 * to file). In both cases the output is a pipe, naturally.
 */
static long vmsplice_to_pipe(struct file *file, struct iov_iter *iter,
			     unsigned int flags)
{
	struct pipe_inode_info *pipe;
	long ret = 0;
	unsigned buf_flag = 0;

	if (flags & SPLICE_F_GIFT)
		buf_flag = PIPE_BUF_FLAG_GIFT;

<<<<<<< HEAD
	pipe = get_pipe_info(file);
=======
	pipe = get_pipe_info(file, true);
>>>>>>> upstream/android-13
	if (!pipe)
		return -EBADF;

	pipe_lock(pipe);
	ret = wait_for_space(pipe, flags);
	if (!ret)
		ret = iter_to_pipe(iter, pipe, buf_flag);
	pipe_unlock(pipe);
	if (ret > 0)
		wakeup_pipe_readers(pipe);
	return ret;
}

static int vmsplice_type(struct fd f, int *type)
{
	if (!f.file)
		return -EBADF;
	if (f.file->f_mode & FMODE_WRITE) {
		*type = WRITE;
	} else if (f.file->f_mode & FMODE_READ) {
		*type = READ;
	} else {
		fdput(f);
		return -EBADF;
	}
	return 0;
}

/*
 * Note that vmsplice only really supports true splicing _from_ user memory
 * to a pipe, not the other way around. Splicing from user memory is a simple
 * operation that can be supported without any funky alignment restrictions
 * or nasty vm tricks. We simply map in the user memory and fill them into
 * a pipe. The reverse isn't quite as easy, though. There are two possible
 * solutions for that:
 *
 *	- memcpy() the data internally, at which point we might as well just
 *	  do a regular read() on the buffer anyway.
 *	- Lots of nasty vm tricks, that are neither fast nor flexible (it
 *	  has restriction limitations on both ends of the pipe).
 *
 * Currently we punt and implement it as a normal copy, see pipe_to_user().
 *
 */
<<<<<<< HEAD
static long do_vmsplice(struct file *f, struct iov_iter *iter, unsigned int flags)
{
	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;

	if (!iov_iter_count(iter))
		return 0;

	if (iov_iter_rw(iter) == WRITE)
		return vmsplice_to_pipe(f, iter, flags);
	else
		return vmsplice_to_user(f, iter, flags);
}

=======
>>>>>>> upstream/android-13
SYSCALL_DEFINE4(vmsplice, int, fd, const struct iovec __user *, uiov,
		unsigned long, nr_segs, unsigned int, flags)
{
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;
	struct iov_iter iter;
<<<<<<< HEAD
	long error;
	struct fd f;
	int type;

=======
	ssize_t error;
	struct fd f;
	int type;

	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;

>>>>>>> upstream/android-13
	f = fdget(fd);
	error = vmsplice_type(f, &type);
	if (error)
		return error;

	error = import_iovec(type, uiov, nr_segs,
			     ARRAY_SIZE(iovstack), &iov, &iter);
<<<<<<< HEAD
	if (!error) {
		error = do_vmsplice(f.file, &iter, flags);
		kfree(iov);
	}
=======
	if (error < 0)
		goto out_fdput;

	if (!iov_iter_count(&iter))
		error = 0;
	else if (iov_iter_rw(&iter) == WRITE)
		error = vmsplice_to_pipe(f.file, &iter, flags);
	else
		error = vmsplice_to_user(f.file, &iter, flags);

	kfree(iov);
out_fdput:
>>>>>>> upstream/android-13
	fdput(f);
	return error;
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
COMPAT_SYSCALL_DEFINE4(vmsplice, int, fd, const struct compat_iovec __user *, iov32,
		    unsigned int, nr_segs, unsigned int, flags)
{
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;
	struct iov_iter iter;
	long error;
	struct fd f;
	int type;

	f = fdget(fd);
	error = vmsplice_type(f, &type);
	if (error)
		return error;

	error = compat_import_iovec(type, iov32, nr_segs,
			     ARRAY_SIZE(iovstack), &iov, &iter);
	if (!error) {
		error = do_vmsplice(f.file, &iter, flags);
		kfree(iov);
	}
	fdput(f);
	return error;
}
#endif

=======
>>>>>>> upstream/android-13
SYSCALL_DEFINE6(splice, int, fd_in, loff_t __user *, off_in,
		int, fd_out, loff_t __user *, off_out,
		size_t, len, unsigned int, flags)
{
	struct fd in, out;
	long error;

	if (unlikely(!len))
		return 0;

	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;

	error = -EBADF;
	in = fdget(fd_in);
	if (in.file) {
<<<<<<< HEAD
		if (in.file->f_mode & FMODE_READ) {
			out = fdget(fd_out);
			if (out.file) {
				if (out.file->f_mode & FMODE_WRITE)
					error = do_splice(in.file, off_in,
							  out.file, off_out,
							  len, flags);
				fdput(out);
			}
=======
		out = fdget(fd_out);
		if (out.file) {
			error = __do_splice(in.file, off_in, out.file, off_out,
						len, flags);
			fdput(out);
>>>>>>> upstream/android-13
		}
		fdput(in);
	}
	return error;
}

/*
 * Make sure there's data to read. Wait for input if we can, otherwise
 * return an appropriate error.
 */
static int ipipe_prep(struct pipe_inode_info *pipe, unsigned int flags)
{
	int ret;

	/*
<<<<<<< HEAD
	 * Check ->nrbufs without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (pipe->nrbufs)
=======
	 * Check the pipe occupancy without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (!pipe_empty(pipe->head, pipe->tail))
>>>>>>> upstream/android-13
		return 0;

	ret = 0;
	pipe_lock(pipe);

<<<<<<< HEAD
	while (!pipe->nrbufs) {
=======
	while (pipe_empty(pipe->head, pipe->tail)) {
>>>>>>> upstream/android-13
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
		if (!pipe->writers)
			break;
<<<<<<< HEAD
		if (!pipe->waiting_writers) {
			if (flags & SPLICE_F_NONBLOCK) {
				ret = -EAGAIN;
				break;
			}
		}
		pipe_wait(pipe);
=======
		if (flags & SPLICE_F_NONBLOCK) {
			ret = -EAGAIN;
			break;
		}
		pipe_wait_readable(pipe);
>>>>>>> upstream/android-13
	}

	pipe_unlock(pipe);
	return ret;
}

/*
 * Make sure there's writeable room. Wait for room if we can, otherwise
 * return an appropriate error.
 */
static int opipe_prep(struct pipe_inode_info *pipe, unsigned int flags)
{
	int ret;

	/*
<<<<<<< HEAD
	 * Check ->nrbufs without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (pipe->nrbufs < pipe->buffers)
=======
	 * Check pipe occupancy without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (!pipe_full(pipe->head, pipe->tail, pipe->max_usage))
>>>>>>> upstream/android-13
		return 0;

	ret = 0;
	pipe_lock(pipe);

<<<<<<< HEAD
	while (pipe->nrbufs >= pipe->buffers) {
=======
	while (pipe_full(pipe->head, pipe->tail, pipe->max_usage)) {
>>>>>>> upstream/android-13
		if (!pipe->readers) {
			send_sig(SIGPIPE, current, 0);
			ret = -EPIPE;
			break;
		}
		if (flags & SPLICE_F_NONBLOCK) {
			ret = -EAGAIN;
			break;
		}
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
<<<<<<< HEAD
		pipe->waiting_writers++;
		pipe_wait(pipe);
		pipe->waiting_writers--;
=======
		pipe_wait_writable(pipe);
>>>>>>> upstream/android-13
	}

	pipe_unlock(pipe);
	return ret;
}

/*
 * Splice contents of ipipe to opipe.
 */
static int splice_pipe_to_pipe(struct pipe_inode_info *ipipe,
			       struct pipe_inode_info *opipe,
			       size_t len, unsigned int flags)
{
	struct pipe_buffer *ibuf, *obuf;
<<<<<<< HEAD
	int ret = 0, nbuf;
=======
	unsigned int i_head, o_head;
	unsigned int i_tail, o_tail;
	unsigned int i_mask, o_mask;
	int ret = 0;
>>>>>>> upstream/android-13
	bool input_wakeup = false;


retry:
	ret = ipipe_prep(ipipe, flags);
	if (ret)
		return ret;

	ret = opipe_prep(opipe, flags);
	if (ret)
		return ret;

	/*
	 * Potential ABBA deadlock, work around it by ordering lock
	 * grabbing by pipe info address. Otherwise two different processes
	 * could deadlock (one doing tee from A -> B, the other from B -> A).
	 */
	pipe_double_lock(ipipe, opipe);

<<<<<<< HEAD
	do {
=======
	i_tail = ipipe->tail;
	i_mask = ipipe->ring_size - 1;
	o_head = opipe->head;
	o_mask = opipe->ring_size - 1;

	do {
		size_t o_len;

>>>>>>> upstream/android-13
		if (!opipe->readers) {
			send_sig(SIGPIPE, current, 0);
			if (!ret)
				ret = -EPIPE;
			break;
		}

<<<<<<< HEAD
		if (!ipipe->nrbufs && !ipipe->writers)
=======
		i_head = ipipe->head;
		o_tail = opipe->tail;

		if (pipe_empty(i_head, i_tail) && !ipipe->writers)
>>>>>>> upstream/android-13
			break;

		/*
		 * Cannot make any progress, because either the input
		 * pipe is empty or the output pipe is full.
		 */
<<<<<<< HEAD
		if (!ipipe->nrbufs || opipe->nrbufs >= opipe->buffers) {
=======
		if (pipe_empty(i_head, i_tail) ||
		    pipe_full(o_head, o_tail, opipe->max_usage)) {
>>>>>>> upstream/android-13
			/* Already processed some buffers, break */
			if (ret)
				break;

			if (flags & SPLICE_F_NONBLOCK) {
				ret = -EAGAIN;
				break;
			}

			/*
			 * We raced with another reader/writer and haven't
			 * managed to process any buffers.  A zero return
			 * value means EOF, so retry instead.
			 */
			pipe_unlock(ipipe);
			pipe_unlock(opipe);
			goto retry;
		}

<<<<<<< HEAD
		ibuf = ipipe->bufs + ipipe->curbuf;
		nbuf = (opipe->curbuf + opipe->nrbufs) & (opipe->buffers - 1);
		obuf = opipe->bufs + nbuf;
=======
		ibuf = &ipipe->bufs[i_tail & i_mask];
		obuf = &opipe->bufs[o_head & o_mask];
>>>>>>> upstream/android-13

		if (len >= ibuf->len) {
			/*
			 * Simply move the whole buffer from ipipe to opipe
			 */
			*obuf = *ibuf;
			ibuf->ops = NULL;
<<<<<<< HEAD
			opipe->nrbufs++;
			ipipe->curbuf = (ipipe->curbuf + 1) & (ipipe->buffers - 1);
			ipipe->nrbufs--;
			input_wakeup = true;
=======
			i_tail++;
			ipipe->tail = i_tail;
			input_wakeup = true;
			o_len = obuf->len;
			o_head++;
			opipe->head = o_head;
>>>>>>> upstream/android-13
		} else {
			/*
			 * Get a reference to this pipe buffer,
			 * so we can copy the contents over.
			 */
			if (!pipe_buf_get(ipipe, ibuf)) {
				if (ret == 0)
					ret = -EFAULT;
				break;
			}
			*obuf = *ibuf;

			/*
<<<<<<< HEAD
			 * Don't inherit the gift flag, we need to
			 * prevent multiple steals of this page.
			 */
			obuf->flags &= ~PIPE_BUF_FLAG_GIFT;

			pipe_buf_mark_unmergeable(obuf);

			obuf->len = len;
			opipe->nrbufs++;
			ibuf->offset += obuf->len;
			ibuf->len -= obuf->len;
		}
		ret += obuf->len;
		len -= obuf->len;
=======
			 * Don't inherit the gift and merge flags, we need to
			 * prevent multiple steals of this page.
			 */
			obuf->flags &= ~PIPE_BUF_FLAG_GIFT;
			obuf->flags &= ~PIPE_BUF_FLAG_CAN_MERGE;

			obuf->len = len;
			ibuf->offset += len;
			ibuf->len -= len;
			o_len = len;
			o_head++;
			opipe->head = o_head;
		}
		ret += o_len;
		len -= o_len;
>>>>>>> upstream/android-13
	} while (len);

	pipe_unlock(ipipe);
	pipe_unlock(opipe);

	/*
	 * If we put data in the output pipe, wakeup any potential readers.
	 */
	if (ret > 0)
		wakeup_pipe_readers(opipe);

	if (input_wakeup)
		wakeup_pipe_writers(ipipe);

	return ret;
}

/*
 * Link contents of ipipe to opipe.
 */
static int link_pipe(struct pipe_inode_info *ipipe,
		     struct pipe_inode_info *opipe,
		     size_t len, unsigned int flags)
{
	struct pipe_buffer *ibuf, *obuf;
<<<<<<< HEAD
	int ret = 0, i = 0, nbuf;
=======
	unsigned int i_head, o_head;
	unsigned int i_tail, o_tail;
	unsigned int i_mask, o_mask;
	int ret = 0;
>>>>>>> upstream/android-13

	/*
	 * Potential ABBA deadlock, work around it by ordering lock
	 * grabbing by pipe info address. Otherwise two different processes
	 * could deadlock (one doing tee from A -> B, the other from B -> A).
	 */
	pipe_double_lock(ipipe, opipe);

<<<<<<< HEAD
=======
	i_tail = ipipe->tail;
	i_mask = ipipe->ring_size - 1;
	o_head = opipe->head;
	o_mask = opipe->ring_size - 1;

>>>>>>> upstream/android-13
	do {
		if (!opipe->readers) {
			send_sig(SIGPIPE, current, 0);
			if (!ret)
				ret = -EPIPE;
			break;
		}

<<<<<<< HEAD
		/*
		 * If we have iterated all input buffers or ran out of
		 * output room, break.
		 */
		if (i >= ipipe->nrbufs || opipe->nrbufs >= opipe->buffers)
			break;

		ibuf = ipipe->bufs + ((ipipe->curbuf + i) & (ipipe->buffers-1));
		nbuf = (opipe->curbuf + opipe->nrbufs) & (opipe->buffers - 1);
=======
		i_head = ipipe->head;
		o_tail = opipe->tail;

		/*
		 * If we have iterated all input buffers or run out of
		 * output room, break.
		 */
		if (pipe_empty(i_head, i_tail) ||
		    pipe_full(o_head, o_tail, opipe->max_usage))
			break;

		ibuf = &ipipe->bufs[i_tail & i_mask];
		obuf = &opipe->bufs[o_head & o_mask];
>>>>>>> upstream/android-13

		/*
		 * Get a reference to this pipe buffer,
		 * so we can copy the contents over.
		 */
		if (!pipe_buf_get(ipipe, ibuf)) {
			if (ret == 0)
				ret = -EFAULT;
			break;
		}

<<<<<<< HEAD
		obuf = opipe->bufs + nbuf;
		*obuf = *ibuf;

		/*
		 * Don't inherit the gift flag, we need to
		 * prevent multiple steals of this page.
		 */
		obuf->flags &= ~PIPE_BUF_FLAG_GIFT;

		pipe_buf_mark_unmergeable(obuf);

		if (obuf->len > len)
			obuf->len = len;

		opipe->nrbufs++;
		ret += obuf->len;
		len -= obuf->len;
		i++;
	} while (len);

	/*
	 * return EAGAIN if we have the potential of some data in the
	 * future, otherwise just return 0
	 */
	if (!ret && ipipe->waiting_writers && (flags & SPLICE_F_NONBLOCK))
		ret = -EAGAIN;
=======
		*obuf = *ibuf;

		/*
		 * Don't inherit the gift and merge flag, we need to prevent
		 * multiple steals of this page.
		 */
		obuf->flags &= ~PIPE_BUF_FLAG_GIFT;
		obuf->flags &= ~PIPE_BUF_FLAG_CAN_MERGE;

		if (obuf->len > len)
			obuf->len = len;
		ret += obuf->len;
		len -= obuf->len;

		o_head++;
		opipe->head = o_head;
		i_tail++;
	} while (len);
>>>>>>> upstream/android-13

	pipe_unlock(ipipe);
	pipe_unlock(opipe);

	/*
	 * If we put data in the output pipe, wakeup any potential readers.
	 */
	if (ret > 0)
		wakeup_pipe_readers(opipe);

	return ret;
}

/*
 * This is a tee(1) implementation that works on pipes. It doesn't copy
 * any data, it simply references the 'in' pages on the 'out' pipe.
 * The 'flags' used are the SPLICE_F_* variants, currently the only
 * applicable one is SPLICE_F_NONBLOCK.
 */
<<<<<<< HEAD
static long do_tee(struct file *in, struct file *out, size_t len,
		   unsigned int flags)
{
	struct pipe_inode_info *ipipe = get_pipe_info(in);
	struct pipe_inode_info *opipe = get_pipe_info(out);
	int ret = -EINVAL;

=======
long do_tee(struct file *in, struct file *out, size_t len, unsigned int flags)
{
	struct pipe_inode_info *ipipe = get_pipe_info(in, true);
	struct pipe_inode_info *opipe = get_pipe_info(out, true);
	int ret = -EINVAL;

	if (unlikely(!(in->f_mode & FMODE_READ) ||
		     !(out->f_mode & FMODE_WRITE)))
		return -EBADF;

>>>>>>> upstream/android-13
	/*
	 * Duplicate the contents of ipipe to opipe without actually
	 * copying the data.
	 */
	if (ipipe && opipe && ipipe != opipe) {
<<<<<<< HEAD
=======
		if ((in->f_flags | out->f_flags) & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

>>>>>>> upstream/android-13
		/*
		 * Keep going, unless we encounter an error. The ipipe/opipe
		 * ordering doesn't really matter.
		 */
		ret = ipipe_prep(ipipe, flags);
		if (!ret) {
			ret = opipe_prep(opipe, flags);
			if (!ret)
				ret = link_pipe(ipipe, opipe, len, flags);
		}
	}

	return ret;
}

SYSCALL_DEFINE4(tee, int, fdin, int, fdout, size_t, len, unsigned int, flags)
{
<<<<<<< HEAD
	struct fd in;
=======
	struct fd in, out;
>>>>>>> upstream/android-13
	int error;

	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;

	if (unlikely(!len))
		return 0;

	error = -EBADF;
	in = fdget(fdin);
	if (in.file) {
<<<<<<< HEAD
		if (in.file->f_mode & FMODE_READ) {
			struct fd out = fdget(fdout);
			if (out.file) {
				if (out.file->f_mode & FMODE_WRITE)
					error = do_tee(in.file, out.file,
							len, flags);
				fdput(out);
			}
=======
		out = fdget(fdout);
		if (out.file) {
			error = do_tee(in.file, out.file, len, flags);
			fdput(out);
>>>>>>> upstream/android-13
		}
 		fdput(in);
 	}

	return error;
}
