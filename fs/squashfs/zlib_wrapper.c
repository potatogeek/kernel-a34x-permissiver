<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
=======
>>>>>>> upstream/android-13
 * zlib_wrapper.c
 */


#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/buffer_head.h>
=======
#include <linux/bio.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/zlib.h>
#include <linux/vmalloc.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs.h"
#include "decompressor.h"
#include "page_actor.h"

static void *zlib_init(struct squashfs_sb_info *dummy, void *buff)
{
	z_stream *stream = kmalloc(sizeof(z_stream), GFP_KERNEL);
	if (stream == NULL)
		goto failed;
	stream->workspace = vmalloc(zlib_inflate_workspacesize());
	if (stream->workspace == NULL)
		goto failed;

	return stream;

failed:
	ERROR("Failed to allocate zlib workspace\n");
	kfree(stream);
	return ERR_PTR(-ENOMEM);
}


static void zlib_free(void *strm)
{
	z_stream *stream = strm;

	if (stream)
		vfree(stream->workspace);
	kfree(stream);
}


static int zlib_uncompress(struct squashfs_sb_info *msblk, void *strm,
<<<<<<< HEAD
	struct buffer_head **bh, int b, int offset, int length,
	struct squashfs_page_actor *output)
{
	int zlib_err, zlib_init = 0, k = 0;
=======
	struct bio *bio, int offset, int length,
	struct squashfs_page_actor *output)
{
	struct bvec_iter_all iter_all = {};
	struct bio_vec *bvec = bvec_init_iter_all(&iter_all);
	int zlib_init = 0, error = 0;
>>>>>>> upstream/android-13
	z_stream *stream = strm;

	stream->avail_out = PAGE_SIZE;
	stream->next_out = squashfs_first_page(output);
	stream->avail_in = 0;

<<<<<<< HEAD
	do {
		if (stream->avail_in == 0 && k < b) {
			int avail = min(length, msblk->devblksize - offset);
			length -= avail;
			stream->next_in = bh[k]->b_data + offset;
=======
	for (;;) {
		int zlib_err;

		if (stream->avail_in == 0) {
			const void *data;
			int avail;

			if (!bio_next_segment(bio, &iter_all)) {
				/* Z_STREAM_END must be reached. */
				error = -EIO;
				break;
			}

			avail = min(length, ((int)bvec->bv_len) - offset);
			data = bvec_virt(bvec);
			length -= avail;
			stream->next_in = data + offset;
>>>>>>> upstream/android-13
			stream->avail_in = avail;
			offset = 0;
		}

		if (stream->avail_out == 0) {
			stream->next_out = squashfs_next_page(output);
			if (stream->next_out != NULL)
				stream->avail_out = PAGE_SIZE;
		}

		if (!zlib_init) {
			zlib_err = zlib_inflateInit(stream);
			if (zlib_err != Z_OK) {
<<<<<<< HEAD
				squashfs_finish_page(output);
				goto out;
=======
				error = -EIO;
				break;
>>>>>>> upstream/android-13
			}
			zlib_init = 1;
		}

		zlib_err = zlib_inflate(stream, Z_SYNC_FLUSH);
<<<<<<< HEAD

		if (stream->avail_in == 0 && k < b)
			put_bh(bh[k++]);
	} while (zlib_err == Z_OK);

	squashfs_finish_page(output);

	if (zlib_err != Z_STREAM_END)
		goto out;

	zlib_err = zlib_inflateEnd(stream);
	if (zlib_err != Z_OK)
		goto out;

	if (k < b)
		goto out;

	return stream->total_out;

out:
	for (; k < b; k++)
		put_bh(bh[k]);

	return -EIO;
=======
		if (zlib_err == Z_STREAM_END)
			break;
		if (zlib_err != Z_OK) {
			error = -EIO;
			break;
		}
	}

	squashfs_finish_page(output);

	if (!error)
		if (zlib_inflateEnd(stream) != Z_OK)
			error = -EIO;

	return error ? error : stream->total_out;
>>>>>>> upstream/android-13
}

const struct squashfs_decompressor squashfs_zlib_comp_ops = {
	.init = zlib_init,
	.free = zlib_free,
	.decompress = zlib_uncompress,
	.id = ZLIB_COMPRESSION,
	.name = "zlib",
	.supported = 1
};

