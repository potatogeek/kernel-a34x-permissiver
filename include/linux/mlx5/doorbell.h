/*
 * Copyright (c) 2013-2015, Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MLX5_DOORBELL_H
#define MLX5_DOORBELL_H

#define MLX5_BF_OFFSET	      0x800
#define MLX5_CQ_DOORBELL      0x20

<<<<<<< HEAD
#if BITS_PER_LONG == 64
/* Assume that we can just write a 64-bit doorbell atomically.  s390
 * actually doesn't have writeq() but S/390 systems don't even have
 * PCI so we won't worry about it.
 */

#define MLX5_DECLARE_DOORBELL_LOCK(name)
#define MLX5_INIT_DOORBELL_LOCK(ptr)    do { } while (0)
#define MLX5_GET_DOORBELL_LOCK(ptr)      (NULL)

static inline void mlx5_write64(__be32 val[2], void __iomem *dest,
				spinlock_t *doorbell_lock)
{
	__raw_writeq(*(u64 *)val, dest);
}

#else

/* Just fall back to a spinlock to protect the doorbell if
 * BITS_PER_LONG is 32 -- there's no portable way to do atomic 64-bit
 * MMIO writes.
 */

#define MLX5_DECLARE_DOORBELL_LOCK(name) spinlock_t name;
#define MLX5_INIT_DOORBELL_LOCK(ptr)     spin_lock_init(ptr)
#define MLX5_GET_DOORBELL_LOCK(ptr)      (ptr)

static inline void mlx5_write64(__be32 val[2], void __iomem *dest,
				spinlock_t *doorbell_lock)
{
	unsigned long flags;

	if (doorbell_lock)
		spin_lock_irqsave(doorbell_lock, flags);
	__raw_writel((__force u32) val[0], dest);
	__raw_writel((__force u32) val[1], dest + 4);
	if (doorbell_lock)
		spin_unlock_irqrestore(doorbell_lock, flags);
}

#endif

=======
/* Assume that we can just write a 64-bit doorbell atomically.  s390
 * actually doesn't have writeq() but S/390 systems don't even have
 * PCI so we won't worry about it.
 *
 * Note that the write is not atomic on 32-bit systems! In contrast to 64-bit
 * ones, it requires proper locking. mlx5_write64 doesn't do any locking, so use
 * it at your own discretion, protected by some kind of lock on 32 bits.
 *
 * TODO: use write{q,l}_relaxed()
 */

static inline void mlx5_write64(__be32 val[2], void __iomem *dest)
{
#if BITS_PER_LONG == 64
	__raw_writeq(*(u64 *)val, dest);
#else
	__raw_writel((__force u32) val[0], dest);
	__raw_writel((__force u32) val[1], dest + 4);
#endif
}

>>>>>>> upstream/android-13
#endif /* MLX5_DOORBELL_H */
