// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 */

#include <asm/unaligned.h>

#include "ctree.h"

<<<<<<< HEAD
static inline u8 get_unaligned_le8(const void *p)
{
       return *(u8 *)p;
}

static inline void put_unaligned_le8(u8 val, void *p)
{
       *(u8 *)p = val;
}

/*
 * this is some deeply nasty code.
 *
 * The end result is that anyone who #includes ctree.h gets a
 * declaration for the btrfs_set_foo functions and btrfs_foo functions,
 * which are wrappers of btrfs_set_token_#bits functions and
 * btrfs_get_token_#bits functions, which are defined in this file.
 *
 * These setget functions do all the extent_buffer related mapping
 * required to efficiently read and write specific fields in the extent
 * buffers.  Every pointer to metadata items in btrfs is really just
 * an unsigned long offset into the extent buffer which has been
 * cast to a specific type.  This gives us all the gcc type checking.
 *
 * The extent buffer api is used to do the page spanning work required to
 * have a metadata blocksize different from the page size.
 */

#define DEFINE_BTRFS_SETGET_BITS(bits)					\
u##bits btrfs_get_token_##bits(const struct extent_buffer *eb,		\
			       const void *ptr, unsigned long off,	\
			       struct btrfs_map_token *token)		\
{									\
	unsigned long part_offset = (unsigned long)ptr;			\
	unsigned long offset = part_offset + off;			\
	void *p;							\
	int err;							\
	char *kaddr;							\
	unsigned long map_start;					\
	unsigned long map_len;						\
	int size = sizeof(u##bits);					\
	u##bits res;							\
									\
	if (token && token->kaddr && token->offset <= offset &&		\
	    token->eb == eb &&						\
	   (token->offset + PAGE_SIZE >= offset + size)) {	\
		kaddr = token->kaddr;					\
		p = kaddr + part_offset - token->offset;		\
		res = get_unaligned_le##bits(p + off);			\
		return res;						\
	}								\
	err = map_private_extent_buffer(eb, offset, size,		\
					&kaddr, &map_start, &map_len);	\
	if (err) {							\
		__le##bits leres;					\
									\
		read_extent_buffer(eb, &leres, offset, size);		\
		return le##bits##_to_cpu(leres);			\
	}								\
	p = kaddr + part_offset - map_start;				\
	res = get_unaligned_le##bits(p + off);				\
	if (token) {							\
		token->kaddr = kaddr;					\
		token->offset = map_start;				\
		token->eb = eb;						\
	}								\
	return res;							\
}									\
void btrfs_set_token_##bits(struct extent_buffer *eb,			\
			    const void *ptr, unsigned long off,		\
			    u##bits val,				\
			    struct btrfs_map_token *token)		\
{									\
	unsigned long part_offset = (unsigned long)ptr;			\
	unsigned long offset = part_offset + off;			\
	void *p;							\
	int err;							\
	char *kaddr;							\
	unsigned long map_start;					\
	unsigned long map_len;						\
	int size = sizeof(u##bits);					\
									\
	if (token && token->kaddr && token->offset <= offset &&		\
	    token->eb == eb &&						\
	   (token->offset + PAGE_SIZE >= offset + size)) {	\
		kaddr = token->kaddr;					\
		p = kaddr + part_offset - token->offset;		\
		put_unaligned_le##bits(val, p + off);			\
		return;							\
	}								\
	err = map_private_extent_buffer(eb, offset, size,		\
			&kaddr, &map_start, &map_len);			\
	if (err) {							\
		__le##bits val2;					\
									\
		val2 = cpu_to_le##bits(val);				\
		write_extent_buffer(eb, &val2, offset, size);		\
		return;							\
	}								\
	p = kaddr + part_offset - map_start;				\
	put_unaligned_le##bits(val, p + off);				\
	if (token) {							\
		token->kaddr = kaddr;					\
		token->offset = map_start;				\
		token->eb = eb;						\
	}								\
=======
static bool check_setget_bounds(const struct extent_buffer *eb,
				const void *ptr, unsigned off, int size)
{
	const unsigned long member_offset = (unsigned long)ptr + off;

	if (member_offset > eb->len) {
		btrfs_warn(eb->fs_info,
	"bad eb member start: ptr 0x%lx start %llu member offset %lu size %d",
			(unsigned long)ptr, eb->start, member_offset, size);
		return false;
	}
	if (member_offset + size > eb->len) {
		btrfs_warn(eb->fs_info,
	"bad eb member end: ptr 0x%lx start %llu member offset %lu size %d",
			(unsigned long)ptr, eb->start, member_offset, size);
		return false;
	}

	return true;
}

/*
 * Macro templates that define helpers to read/write extent buffer data of a
 * given size, that are also used via ctree.h for access to item members by
 * specialized helpers.
 *
 * Generic helpers:
 * - btrfs_set_8 (for 8/16/32/64)
 * - btrfs_get_8 (for 8/16/32/64)
 *
 * Generic helpers with a token (cached address of the most recently accessed
 * page):
 * - btrfs_set_token_8 (for 8/16/32/64)
 * - btrfs_get_token_8 (for 8/16/32/64)
 *
 * The set/get functions handle data spanning two pages transparently, in case
 * metadata block size is larger than page.  Every pointer to metadata items is
 * an offset into the extent buffer page array, cast to a specific type.  This
 * gives us all the type checking.
 *
 * The extent buffer pages stored in the array pages do not form a contiguous
 * phyusical range, but the API functions assume the linear offset to the range
 * from 0 to metadata node size.
 */

#define DEFINE_BTRFS_SETGET_BITS(bits)					\
u##bits btrfs_get_token_##bits(struct btrfs_map_token *token,		\
			       const void *ptr, unsigned long off)	\
{									\
	const unsigned long member_offset = (unsigned long)ptr + off;	\
	const unsigned long idx = get_eb_page_index(member_offset);	\
	const unsigned long oip = get_eb_offset_in_page(token->eb,	\
							member_offset);	\
	const int size = sizeof(u##bits);				\
	u8 lebytes[sizeof(u##bits)];					\
	const int part = PAGE_SIZE - oip;				\
									\
	ASSERT(token);							\
	ASSERT(token->kaddr);						\
	ASSERT(check_setget_bounds(token->eb, ptr, off, size));		\
	if (token->offset <= member_offset &&				\
	    member_offset + size <= token->offset + PAGE_SIZE) {	\
		return get_unaligned_le##bits(token->kaddr + oip);	\
	}								\
	token->kaddr = page_address(token->eb->pages[idx]);		\
	token->offset = idx << PAGE_SHIFT;				\
	if (INLINE_EXTENT_BUFFER_PAGES == 1 || oip + size <= PAGE_SIZE ) \
		return get_unaligned_le##bits(token->kaddr + oip);	\
									\
	memcpy(lebytes, token->kaddr + oip, part);			\
	token->kaddr = page_address(token->eb->pages[idx + 1]);		\
	token->offset = (idx + 1) << PAGE_SHIFT;			\
	memcpy(lebytes + part, token->kaddr, size - part);		\
	return get_unaligned_le##bits(lebytes);				\
}									\
u##bits btrfs_get_##bits(const struct extent_buffer *eb,		\
			 const void *ptr, unsigned long off)		\
{									\
	const unsigned long member_offset = (unsigned long)ptr + off;	\
	const unsigned long oip = get_eb_offset_in_page(eb, member_offset); \
	const unsigned long idx = get_eb_page_index(member_offset);	\
	char *kaddr = page_address(eb->pages[idx]);			\
	const int size = sizeof(u##bits);				\
	const int part = PAGE_SIZE - oip;				\
	u8 lebytes[sizeof(u##bits)];					\
									\
	ASSERT(check_setget_bounds(eb, ptr, off, size));		\
	if (INLINE_EXTENT_BUFFER_PAGES == 1 || oip + size <= PAGE_SIZE)	\
		return get_unaligned_le##bits(kaddr + oip);		\
									\
	memcpy(lebytes, kaddr + oip, part);				\
	kaddr = page_address(eb->pages[idx + 1]);			\
	memcpy(lebytes + part, kaddr, size - part);			\
	return get_unaligned_le##bits(lebytes);				\
}									\
void btrfs_set_token_##bits(struct btrfs_map_token *token,		\
			    const void *ptr, unsigned long off,		\
			    u##bits val)				\
{									\
	const unsigned long member_offset = (unsigned long)ptr + off;	\
	const unsigned long idx = get_eb_page_index(member_offset);	\
	const unsigned long oip = get_eb_offset_in_page(token->eb,	\
							member_offset);	\
	const int size = sizeof(u##bits);				\
	u8 lebytes[sizeof(u##bits)];					\
	const int part = PAGE_SIZE - oip;				\
									\
	ASSERT(token);							\
	ASSERT(token->kaddr);						\
	ASSERT(check_setget_bounds(token->eb, ptr, off, size));		\
	if (token->offset <= member_offset &&				\
	    member_offset + size <= token->offset + PAGE_SIZE) {	\
		put_unaligned_le##bits(val, token->kaddr + oip);	\
		return;							\
	}								\
	token->kaddr = page_address(token->eb->pages[idx]);		\
	token->offset = idx << PAGE_SHIFT;				\
	if (INLINE_EXTENT_BUFFER_PAGES == 1 || oip + size <= PAGE_SIZE) { \
		put_unaligned_le##bits(val, token->kaddr + oip);	\
		return;							\
	}								\
	put_unaligned_le##bits(val, lebytes);				\
	memcpy(token->kaddr + oip, lebytes, part);			\
	token->kaddr = page_address(token->eb->pages[idx + 1]);		\
	token->offset = (idx + 1) << PAGE_SHIFT;			\
	memcpy(token->kaddr, lebytes + part, size - part);		\
}									\
void btrfs_set_##bits(const struct extent_buffer *eb, void *ptr,	\
		      unsigned long off, u##bits val)			\
{									\
	const unsigned long member_offset = (unsigned long)ptr + off;	\
	const unsigned long oip = get_eb_offset_in_page(eb, member_offset); \
	const unsigned long idx = get_eb_page_index(member_offset);	\
	char *kaddr = page_address(eb->pages[idx]);			\
	const int size = sizeof(u##bits);				\
	const int part = PAGE_SIZE - oip;				\
	u8 lebytes[sizeof(u##bits)];					\
									\
	ASSERT(check_setget_bounds(eb, ptr, off, size));		\
	if (INLINE_EXTENT_BUFFER_PAGES == 1 || oip + size <= PAGE_SIZE) { \
		put_unaligned_le##bits(val, kaddr + oip);		\
		return;							\
	}								\
									\
	put_unaligned_le##bits(val, lebytes);				\
	memcpy(kaddr + oip, lebytes, part);				\
	kaddr = page_address(eb->pages[idx + 1]);			\
	memcpy(kaddr, lebytes + part, size - part);			\
>>>>>>> upstream/android-13
}

DEFINE_BTRFS_SETGET_BITS(8)
DEFINE_BTRFS_SETGET_BITS(16)
DEFINE_BTRFS_SETGET_BITS(32)
DEFINE_BTRFS_SETGET_BITS(64)

void btrfs_node_key(const struct extent_buffer *eb,
		    struct btrfs_disk_key *disk_key, int nr)
{
	unsigned long ptr = btrfs_node_key_ptr_offset(nr);
	read_eb_member(eb, (struct btrfs_key_ptr *)ptr,
		       struct btrfs_key_ptr, key, disk_key);
}
