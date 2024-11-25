<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Checksumming functions for IP, TCP, UDP and so on
 *
 * Authors:	Jorge Cwik, <jorge@laser.satlink.net>
 *		Arnt Gulbrandsen, <agulbra@nvg.unit.no>
 *		Borrows very liberally from tcp.c and ip.c, see those
 *		files for more names.
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _CHECKSUM_H
#define _CHECKSUM_H

#include <linux/errno.h>
#include <asm/types.h>
#include <asm/byteorder.h>
#include <linux/uaccess.h>
#include <asm/checksum.h>

#ifndef _HAVE_ARCH_COPY_AND_CSUM_FROM_USER
<<<<<<< HEAD
static inline
__wsum csum_and_copy_from_user (const void __user *src, void *dst,
				      int len, __wsum sum, int *err_ptr)
{
	if (access_ok(VERIFY_READ, src, len))
		return csum_partial_copy_from_user(src, dst, len, sum, err_ptr);

	if (len)
		*err_ptr = -EFAULT;

	return sum;
=======
static __always_inline
__wsum csum_and_copy_from_user (const void __user *src, void *dst,
				      int len)
{
	if (copy_from_user(dst, src, len))
		return 0;
	return csum_partial(dst, len, ~0U);
>>>>>>> upstream/android-13
}
#endif

#ifndef HAVE_CSUM_COPY_USER
<<<<<<< HEAD
static __inline__ __wsum csum_and_copy_to_user
(const void *src, void __user *dst, int len, __wsum sum, int *err_ptr)
{
	sum = csum_partial(src, len, sum);

	if (access_ok(VERIFY_WRITE, dst, len)) {
		if (copy_to_user(dst, src, len) == 0)
			return sum;
	}
	if (len)
		*err_ptr = -EFAULT;

	return (__force __wsum)-1; /* invalid checksum */
=======
static __always_inline __wsum csum_and_copy_to_user
(const void *src, void __user *dst, int len)
{
	__wsum sum = csum_partial(src, len, ~0U);

	if (copy_to_user(dst, src, len) == 0)
		return sum;
	return 0;
}
#endif

#ifndef _HAVE_ARCH_CSUM_AND_COPY
static __always_inline __wsum
csum_partial_copy_nocheck(const void *src, void *dst, int len)
{
	memcpy(dst, src, len);
	return csum_partial(dst, len, 0);
>>>>>>> upstream/android-13
}
#endif

#ifndef HAVE_ARCH_CSUM_ADD
<<<<<<< HEAD
static inline __wsum csum_add(__wsum csum, __wsum addend)
=======
static __always_inline __wsum csum_add(__wsum csum, __wsum addend)
>>>>>>> upstream/android-13
{
	u32 res = (__force u32)csum;
	res += (__force u32)addend;
	return (__force __wsum)(res + (res < (__force u32)addend));
}
#endif

<<<<<<< HEAD
static inline __wsum csum_sub(__wsum csum, __wsum addend)
=======
static __always_inline __wsum csum_sub(__wsum csum, __wsum addend)
>>>>>>> upstream/android-13
{
	return csum_add(csum, ~addend);
}

<<<<<<< HEAD
static inline __sum16 csum16_add(__sum16 csum, __be16 addend)
=======
static __always_inline __sum16 csum16_add(__sum16 csum, __be16 addend)
>>>>>>> upstream/android-13
{
	u16 res = (__force u16)csum;

	res += (__force u16)addend;
	return (__force __sum16)(res + (res < (__force u16)addend));
}

<<<<<<< HEAD
static inline __sum16 csum16_sub(__sum16 csum, __be16 addend)
=======
static __always_inline __sum16 csum16_sub(__sum16 csum, __be16 addend)
>>>>>>> upstream/android-13
{
	return csum16_add(csum, ~addend);
}

<<<<<<< HEAD
static inline __wsum
csum_block_add(__wsum csum, __wsum csum2, int offset)
{
	u32 sum = (__force u32)csum2;

	/* rotate sum to align it with a 16b boundary */
	if (offset & 1)
		sum = ror32(sum, 8);

	return csum_add(csum, (__force __wsum)sum);
}

static inline __wsum
=======
static __always_inline __wsum csum_shift(__wsum sum, int offset)
{
	/* rotate sum to align it with a 16b boundary */
	if (offset & 1)
		return (__force __wsum)ror32((__force u32)sum, 8);
	return sum;
}

static __always_inline __wsum
csum_block_add(__wsum csum, __wsum csum2, int offset)
{
	return csum_add(csum, csum_shift(csum2, offset));
}

static __always_inline __wsum
>>>>>>> upstream/android-13
csum_block_add_ext(__wsum csum, __wsum csum2, int offset, int len)
{
	return csum_block_add(csum, csum2, offset);
}

<<<<<<< HEAD
static inline __wsum
=======
static __always_inline __wsum
>>>>>>> upstream/android-13
csum_block_sub(__wsum csum, __wsum csum2, int offset)
{
	return csum_block_add(csum, ~csum2, offset);
}

<<<<<<< HEAD
static inline __wsum csum_unfold(__sum16 n)
=======
static __always_inline __wsum csum_unfold(__sum16 n)
>>>>>>> upstream/android-13
{
	return (__force __wsum)n;
}

<<<<<<< HEAD
static inline __wsum csum_partial_ext(const void *buff, int len, __wsum sum)
=======
static __always_inline
__wsum csum_partial_ext(const void *buff, int len, __wsum sum)
>>>>>>> upstream/android-13
{
	return csum_partial(buff, len, sum);
}

#define CSUM_MANGLED_0 ((__force __sum16)0xffff)

<<<<<<< HEAD
static inline void csum_replace_by_diff(__sum16 *sum, __wsum diff)
=======
static __always_inline void csum_replace_by_diff(__sum16 *sum, __wsum diff)
>>>>>>> upstream/android-13
{
	*sum = csum_fold(csum_add(diff, ~csum_unfold(*sum)));
}

<<<<<<< HEAD
static inline void csum_replace4(__sum16 *sum, __be32 from, __be32 to)
=======
static __always_inline void csum_replace4(__sum16 *sum, __be32 from, __be32 to)
>>>>>>> upstream/android-13
{
	__wsum tmp = csum_sub(~csum_unfold(*sum), (__force __wsum)from);

	*sum = csum_fold(csum_add(tmp, (__force __wsum)to));
}

/* Implements RFC 1624 (Incremental Internet Checksum)
 * 3. Discussion states :
 *     HC' = ~(~HC + ~m + m')
 *  m : old value of a 16bit field
 *  m' : new value of a 16bit field
 */
<<<<<<< HEAD
static inline void csum_replace2(__sum16 *sum, __be16 old, __be16 new)
=======
static __always_inline void csum_replace2(__sum16 *sum, __be16 old, __be16 new)
>>>>>>> upstream/android-13
{
	*sum = ~csum16_add(csum16_sub(~(*sum), old), new);
}

<<<<<<< HEAD
=======
static inline void csum_replace(__wsum *csum, __wsum old, __wsum new)
{
	*csum = csum_add(csum_sub(*csum, old), new);
}

>>>>>>> upstream/android-13
struct sk_buff;
void inet_proto_csum_replace4(__sum16 *sum, struct sk_buff *skb,
			      __be32 from, __be32 to, bool pseudohdr);
void inet_proto_csum_replace16(__sum16 *sum, struct sk_buff *skb,
			       const __be32 *from, const __be32 *to,
			       bool pseudohdr);
void inet_proto_csum_replace_by_diff(__sum16 *sum, struct sk_buff *skb,
				     __wsum diff, bool pseudohdr);

<<<<<<< HEAD
static inline void inet_proto_csum_replace2(__sum16 *sum, struct sk_buff *skb,
					    __be16 from, __be16 to,
					    bool pseudohdr)
=======
static __always_inline
void inet_proto_csum_replace2(__sum16 *sum, struct sk_buff *skb,
			      __be16 from, __be16 to, bool pseudohdr)
>>>>>>> upstream/android-13
{
	inet_proto_csum_replace4(sum, skb, (__force __be32)from,
				 (__force __be32)to, pseudohdr);
}

<<<<<<< HEAD
static inline __wsum remcsum_adjust(void *ptr, __wsum csum,
				    int start, int offset)
=======
static __always_inline __wsum remcsum_adjust(void *ptr, __wsum csum,
					     int start, int offset)
>>>>>>> upstream/android-13
{
	__sum16 *psum = (__sum16 *)(ptr + offset);
	__wsum delta;

	/* Subtract out checksum up to start */
	csum = csum_sub(csum, csum_partial(ptr, start, 0));

	/* Set derived checksum in packet */
	delta = csum_sub((__force __wsum)csum_fold(csum),
			 (__force __wsum)*psum);
	*psum = csum_fold(csum);

	return delta;
}

<<<<<<< HEAD
static inline void remcsum_unadjust(__sum16 *psum, __wsum delta)
=======
static __always_inline void remcsum_unadjust(__sum16 *psum, __wsum delta)
>>>>>>> upstream/android-13
{
	*psum = csum_fold(csum_sub(delta, (__force __wsum)*psum));
}

#endif
