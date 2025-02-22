

#ifndef _CHECKSUM_H
#define _CHECKSUM_H

#include <linux/errno.h>
#include <asm/types.h>
#include <asm/byteorder.h>
#include <linux/uaccess.h>
#include <asm/checksum.h>

#ifndef _HAVE_ARCH_COPY_AND_CSUM_FROM_USER
static inline
__wsum csum_and_copy_from_user (const void __user *src, void *dst,
				      int len, __wsum sum, int *err_ptr)
{
	if (access_ok(VERIFY_READ, src, len))
		return csum_partial_copy_from_user(src, dst, len, sum, err_ptr);

	if (len)
		*err_ptr = -EFAULT;

	return sum;
}
#endif

#ifndef HAVE_CSUM_COPY_USER
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

	return (__force __wsum)-1; 
}
#endif

#ifndef HAVE_ARCH_CSUM_ADD
static inline __wsum csum_add(__wsum csum, __wsum addend)
{
	u32 res = (__force u32)csum;
	res += (__force u32)addend;
	return (__force __wsum)(res + (res < (__force u32)addend));
}
#endif

static inline __wsum csum_sub(__wsum csum, __wsum addend)
{
	return csum_add(csum, ~addend);
}

static inline __sum16 csum16_add(__sum16 csum, __be16 addend)
{
	u16 res = (__force u16)csum;

	res += (__force u16)addend;
	return (__force __sum16)(res + (res < (__force u16)addend));
}

static inline __sum16 csum16_sub(__sum16 csum, __be16 addend)
{
	return csum16_add(csum, ~addend);
}

static inline __wsum
csum_block_add(__wsum csum, __wsum csum2, int offset)
{
	u32 sum = (__force u32)csum2;

	
	if (offset & 1)
		sum = ror32(sum, 8);

	return csum_add(csum, (__force __wsum)sum);
}

static inline __wsum
csum_block_add_ext(__wsum csum, __wsum csum2, int offset, int len)
{
	return csum_block_add(csum, csum2, offset);
}

static inline __wsum
csum_block_sub(__wsum csum, __wsum csum2, int offset)
{
	return csum_block_add(csum, ~csum2, offset);
}

static inline __wsum csum_unfold(__sum16 n)
{
	return (__force __wsum)n;
}

static inline __wsum csum_partial_ext(const void *buff, int len, __wsum sum)
{
	return csum_partial(buff, len, sum);
}

#define CSUM_MANGLED_0 ((__force __sum16)0xffff)

static inline void csum_replace_by_diff(__sum16 *sum, __wsum diff)
{
	*sum = csum_fold(csum_add(diff, ~csum_unfold(*sum)));
}

static inline void csum_replace4(__sum16 *sum, __be32 from, __be32 to)
{
	__wsum tmp = csum_sub(~csum_unfold(*sum), (__force __wsum)from);

	*sum = csum_fold(csum_add(tmp, (__force __wsum)to));
}


static inline void csum_replace2(__sum16 *sum, __be16 old, __be16 new)
{
	*sum = ~csum16_add(csum16_sub(~(*sum), old), new);
}

struct sk_buff;
void inet_proto_csum_replace4(__sum16 *sum, struct sk_buff *skb,
			      __be32 from, __be32 to, bool pseudohdr);
void inet_proto_csum_replace16(__sum16 *sum, struct sk_buff *skb,
			       const __be32 *from, const __be32 *to,
			       bool pseudohdr);
void inet_proto_csum_replace_by_diff(__sum16 *sum, struct sk_buff *skb,
				     __wsum diff, bool pseudohdr);

static inline void inet_proto_csum_replace2(__sum16 *sum, struct sk_buff *skb,
					    __be16 from, __be16 to,
					    bool pseudohdr)
{
	inet_proto_csum_replace4(sum, skb, (__force __be32)from,
				 (__force __be32)to, pseudohdr);
}

static inline __wsum remcsum_adjust(void *ptr, __wsum csum,
				    int start, int offset)
{
	__sum16 *psum = (__sum16 *)(ptr + offset);
	__wsum delta;

	
	csum = csum_sub(csum, csum_partial(ptr, start, 0));

	
	delta = csum_sub((__force __wsum)csum_fold(csum),
			 (__force __wsum)*psum);
	*psum = csum_fold(csum);

	return delta;
}

static inline void remcsum_unadjust(__sum16 *psum, __wsum delta)
{
	*psum = csum_fold(csum_sub(delta, (__force __wsum)*psum));
}

#endif
