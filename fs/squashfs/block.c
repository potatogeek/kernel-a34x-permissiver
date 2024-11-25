<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008
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
 * block.c
 */

/*
 * This file implements the low-level routines to read and decompress
 * datablocks and metadata blocks.
 */

<<<<<<< HEAD
=======
#include <linux/blkdev.h>
>>>>>>> upstream/android-13
#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/buffer_head.h>
#include <linux/bio.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs.h"
#include "decompressor.h"
#include "page_actor.h"

/*
<<<<<<< HEAD
 * Read the metadata block length, this is stored in the first two
 * bytes of the metadata block.
 */
static struct buffer_head *get_block_length(struct super_block *sb,
			u64 *cur_index, int *offset, int *length)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	struct buffer_head *bh;

	bh = sb_bread(sb, *cur_index);
	if (bh == NULL)
		return NULL;

	if (msblk->devblksize - *offset == 1) {
		*length = (unsigned char) bh->b_data[*offset];
		put_bh(bh);
		bh = sb_bread(sb, ++(*cur_index));
		if (bh == NULL)
			return NULL;
		*length |= (unsigned char) bh->b_data[0] << 8;
		*offset = 1;
	} else {
		*length = (unsigned char) bh->b_data[*offset] |
			(unsigned char) bh->b_data[*offset + 1] << 8;
		*offset += 2;

		if (*offset == msblk->devblksize) {
			put_bh(bh);
			bh = sb_bread(sb, ++(*cur_index));
			if (bh == NULL)
				return NULL;
			*offset = 0;
		}
	}

	return bh;
}

=======
 * Returns the amount of bytes copied to the page actor.
 */
static int copy_bio_to_actor(struct bio *bio,
			     struct squashfs_page_actor *actor,
			     int offset, int req_length)
{
	void *actor_addr = squashfs_first_page(actor);
	struct bvec_iter_all iter_all = {};
	struct bio_vec *bvec = bvec_init_iter_all(&iter_all);
	int copied_bytes = 0;
	int actor_offset = 0;

	if (WARN_ON_ONCE(!bio_next_segment(bio, &iter_all)))
		return 0;

	while (copied_bytes < req_length) {
		int bytes_to_copy = min_t(int, bvec->bv_len - offset,
					  PAGE_SIZE - actor_offset);

		bytes_to_copy = min_t(int, bytes_to_copy,
				      req_length - copied_bytes);
		memcpy(actor_addr + actor_offset, bvec_virt(bvec) + offset,
		       bytes_to_copy);

		actor_offset += bytes_to_copy;
		copied_bytes += bytes_to_copy;
		offset += bytes_to_copy;

		if (actor_offset >= PAGE_SIZE) {
			actor_addr = squashfs_next_page(actor);
			if (!actor_addr)
				break;
			actor_offset = 0;
		}
		if (offset >= bvec->bv_len) {
			if (!bio_next_segment(bio, &iter_all))
				break;
			offset = 0;
		}
	}
	squashfs_finish_page(actor);
	return copied_bytes;
}

static int squashfs_bio_read(struct super_block *sb, u64 index, int length,
			     struct bio **biop, int *block_offset)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	const u64 read_start = round_down(index, msblk->devblksize);
	const sector_t block = read_start >> msblk->devblksize_log2;
	const u64 read_end = round_up(index + length, msblk->devblksize);
	const sector_t block_end = read_end >> msblk->devblksize_log2;
	int offset = read_start - round_down(index, PAGE_SIZE);
	int total_len = (block_end - block) << msblk->devblksize_log2;
	const int page_count = DIV_ROUND_UP(total_len + offset, PAGE_SIZE);
	int error, i;
	struct bio *bio;

	if (page_count <= BIO_MAX_VECS)
		bio = bio_alloc(GFP_NOIO, page_count);
	else
		bio = bio_kmalloc(GFP_NOIO, page_count);

	if (!bio)
		return -ENOMEM;

	bio_set_dev(bio, sb->s_bdev);
	bio->bi_opf = READ;
	bio->bi_iter.bi_sector = block * (msblk->devblksize >> SECTOR_SHIFT);

	for (i = 0; i < page_count; ++i) {
		unsigned int len =
			min_t(unsigned int, PAGE_SIZE - offset, total_len);
		struct page *page = alloc_page(GFP_NOIO);

		if (!page) {
			error = -ENOMEM;
			goto out_free_bio;
		}
		if (!bio_add_page(bio, page, len, offset)) {
			error = -EIO;
			goto out_free_bio;
		}
		offset = 0;
		total_len -= len;
	}

	error = submit_bio_wait(bio);
	if (error)
		goto out_free_bio;

	*biop = bio;
	*block_offset = index & ((1 << msblk->devblksize_log2) - 1);
	return 0;

out_free_bio:
	bio_free_pages(bio);
	bio_put(bio);
	return error;
}
>>>>>>> upstream/android-13

/*
 * Read and decompress a metadata block or datablock.  Length is non-zero
 * if a datablock is being read (the size is stored elsewhere in the
 * filesystem), otherwise the length is obtained from the first two bytes of
 * the metadata block.  A bit in the length field indicates if the block
 * is stored uncompressed in the filesystem (usually because compression
 * generated a larger block - this does occasionally happen with compression
 * algorithms).
 */
int squashfs_read_data(struct super_block *sb, u64 index, int length,
<<<<<<< HEAD
		u64 *next_index, struct squashfs_page_actor *output)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	struct buffer_head **bh;
	int offset = index & ((1 << msblk->devblksize_log2) - 1);
	u64 cur_index = index >> msblk->devblksize_log2;
	int bytes, compressed, b = 0, k = 0, avail, i;

	bh = kcalloc(((output->length + msblk->devblksize - 1)
		>> msblk->devblksize_log2) + 1, sizeof(*bh), GFP_KERNEL);
	if (bh == NULL)
		return -ENOMEM;
=======
		       u64 *next_index, struct squashfs_page_actor *output)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	struct bio *bio = NULL;
	int compressed;
	int res;
	int offset;
>>>>>>> upstream/android-13

	if (length) {
		/*
		 * Datablock.
		 */
<<<<<<< HEAD
		bytes = -offset;
		compressed = SQUASHFS_COMPRESSED_BLOCK(length);
		length = SQUASHFS_COMPRESSED_SIZE_BLOCK(length);
		if (next_index)
			*next_index = index + length;

		TRACE("Block @ 0x%llx, %scompressed size %d, src size %d\n",
			index, compressed ? "" : "un", length, output->length);

		if (length < 0 || length > output->length ||
				(index + length) > msblk->bytes_used)
			goto read_failure;

		for (b = 0; bytes < length; b++, cur_index++) {
			bh[b] = sb_getblk(sb, cur_index);
			if (bh[b] == NULL)
				goto block_release;
			bytes += msblk->devblksize;
		}
		ll_rw_block(REQ_OP_READ, 0, b, bh);
=======
		compressed = SQUASHFS_COMPRESSED_BLOCK(length);
		length = SQUASHFS_COMPRESSED_SIZE_BLOCK(length);
		TRACE("Block @ 0x%llx, %scompressed size %d, src size %d\n",
			index, compressed ? "" : "un", length, output->length);
>>>>>>> upstream/android-13
	} else {
		/*
		 * Metadata block.
		 */
<<<<<<< HEAD
		if ((index + 2) > msblk->bytes_used)
			goto read_failure;

		bh[0] = get_block_length(sb, &cur_index, &offset, &length);
		if (bh[0] == NULL)
			goto read_failure;
		b = 1;

		bytes = msblk->devblksize - offset;
		compressed = SQUASHFS_COMPRESSED(length);
		length = SQUASHFS_COMPRESSED_SIZE(length);
		if (next_index)
			*next_index = index + length + 2;

		TRACE("Block @ 0x%llx, %scompressed size %d\n", index,
				compressed ? "" : "un", length);

		if (length < 0 || length > output->length ||
					(index + length) > msblk->bytes_used)
			goto block_release;

		for (; bytes < length; b++) {
			bh[b] = sb_getblk(sb, ++cur_index);
			if (bh[b] == NULL)
				goto block_release;
			bytes += msblk->devblksize;
		}
		ll_rw_block(REQ_OP_READ, 0, b - 1, bh + 1);
	}

	for (i = 0; i < b; i++) {
		wait_on_buffer(bh[i]);
		if (!buffer_uptodate(bh[i]))
			goto block_release;
	}

	if (compressed) {
		if (!msblk->stream)
			goto read_failure;
		length = squashfs_decompress(msblk, bh, b, offset, length,
			output);
		if (length < 0)
			goto read_failure;
	} else {
		/*
		 * Block is uncompressed.
		 */
		int in, pg_offset = 0;
		void *data = squashfs_first_page(output);

		for (bytes = length; k < b; k++) {
			in = min(bytes, msblk->devblksize - offset);
			bytes -= in;
			while (in) {
				if (pg_offset == PAGE_SIZE) {
					data = squashfs_next_page(output);
					pg_offset = 0;
				}
				avail = min_t(int, in, PAGE_SIZE -
						pg_offset);
				memcpy(data + pg_offset, bh[k]->b_data + offset,
						avail);
				in -= avail;
				pg_offset += avail;
				offset += avail;
			}
			offset = 0;
			put_bh(bh[k]);
		}
		squashfs_finish_page(output);
	}

	kfree(bh);
	return length;

block_release:
	for (; k < b; k++)
		put_bh(bh[k]);

read_failure:
	ERROR("squashfs_read_data failed to read block 0x%llx\n",
					(unsigned long long) index);
	kfree(bh);
	return -EIO;
=======
		const u8 *data;
		struct bvec_iter_all iter_all = {};
		struct bio_vec *bvec = bvec_init_iter_all(&iter_all);

		if (index + 2 > msblk->bytes_used) {
			res = -EIO;
			goto out;
		}
		res = squashfs_bio_read(sb, index, 2, &bio, &offset);
		if (res)
			goto out;

		if (WARN_ON_ONCE(!bio_next_segment(bio, &iter_all))) {
			res = -EIO;
			goto out_free_bio;
		}
		/* Extract the length of the metadata block */
		data = bvec_virt(bvec);
		length = data[offset];
		if (offset < bvec->bv_len - 1) {
			length |= data[offset + 1] << 8;
		} else {
			if (WARN_ON_ONCE(!bio_next_segment(bio, &iter_all))) {
				res = -EIO;
				goto out_free_bio;
			}
			data = bvec_virt(bvec);
			length |= data[0] << 8;
		}
		bio_free_pages(bio);
		bio_put(bio);

		compressed = SQUASHFS_COMPRESSED(length);
		length = SQUASHFS_COMPRESSED_SIZE(length);
		index += 2;

		TRACE("Block @ 0x%llx, %scompressed size %d\n", index - 2,
		      compressed ? "" : "un", length);
	}
	if (length < 0 || length > output->length ||
			(index + length) > msblk->bytes_used) {
		res = -EIO;
		goto out;
	}

	if (next_index)
		*next_index = index + length;

	res = squashfs_bio_read(sb, index, length, &bio, &offset);
	if (res)
		goto out;

	if (compressed) {
		if (!msblk->stream) {
			res = -EIO;
			goto out_free_bio;
		}
		res = squashfs_decompress(msblk, bio, offset, length, output);
	} else {
		res = copy_bio_to_actor(bio, output, offset, length);
	}

out_free_bio:
	bio_free_pages(bio);
	bio_put(bio);
out:
	if (res < 0) {
		ERROR("Failed to read block 0x%llx: %d\n", index, res);
		if (msblk->panic_on_errors)
			panic("squashfs read failed");
	}

	return res;
>>>>>>> upstream/android-13
}
