<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * arch/sh/kernel/iomap.c
 *
 * Copyright (C) 2000  Niibe Yutaka
 * Copyright (C) 2005 - 2007 Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/module.h>
#include <linux/io.h>

<<<<<<< HEAD
unsigned int ioread8(void __iomem *addr)
=======
unsigned int ioread8(const void __iomem *addr)
>>>>>>> upstream/android-13
{
	return readb(addr);
}
EXPORT_SYMBOL(ioread8);

<<<<<<< HEAD
unsigned int ioread16(void __iomem *addr)
=======
unsigned int ioread16(const void __iomem *addr)
>>>>>>> upstream/android-13
{
	return readw(addr);
}
EXPORT_SYMBOL(ioread16);

<<<<<<< HEAD
unsigned int ioread16be(void __iomem *addr)
=======
unsigned int ioread16be(const void __iomem *addr)
>>>>>>> upstream/android-13
{
	return be16_to_cpu(__raw_readw(addr));
}
EXPORT_SYMBOL(ioread16be);

<<<<<<< HEAD
unsigned int ioread32(void __iomem *addr)
=======
unsigned int ioread32(const void __iomem *addr)
>>>>>>> upstream/android-13
{
	return readl(addr);
}
EXPORT_SYMBOL(ioread32);

<<<<<<< HEAD
unsigned int ioread32be(void __iomem *addr)
=======
unsigned int ioread32be(const void __iomem *addr)
>>>>>>> upstream/android-13
{
	return be32_to_cpu(__raw_readl(addr));
}
EXPORT_SYMBOL(ioread32be);

void iowrite8(u8 val, void __iomem *addr)
{
	writeb(val, addr);
}
EXPORT_SYMBOL(iowrite8);

void iowrite16(u16 val, void __iomem *addr)
{
	writew(val, addr);
}
EXPORT_SYMBOL(iowrite16);

void iowrite16be(u16 val, void __iomem *addr)
{
	__raw_writew(cpu_to_be16(val), addr);
}
EXPORT_SYMBOL(iowrite16be);

void iowrite32(u32 val, void __iomem *addr)
{
	writel(val, addr);
}
EXPORT_SYMBOL(iowrite32);

void iowrite32be(u32 val, void __iomem *addr)
{
	__raw_writel(cpu_to_be32(val), addr);
}
EXPORT_SYMBOL(iowrite32be);

/*
 * These are the "repeat MMIO read/write" functions.
 * Note the "__raw" accesses, since we don't want to
 * convert to CPU byte order. We write in "IO byte
 * order" (we also don't have IO barriers).
 */
<<<<<<< HEAD
static inline void mmio_insb(void __iomem *addr, u8 *dst, int count)
=======
static inline void mmio_insb(const void __iomem *addr, u8 *dst, int count)
>>>>>>> upstream/android-13
{
	while (--count >= 0) {
		u8 data = __raw_readb(addr);
		*dst = data;
		dst++;
	}
}

<<<<<<< HEAD
static inline void mmio_insw(void __iomem *addr, u16 *dst, int count)
=======
static inline void mmio_insw(const void __iomem *addr, u16 *dst, int count)
>>>>>>> upstream/android-13
{
	while (--count >= 0) {
		u16 data = __raw_readw(addr);
		*dst = data;
		dst++;
	}
}

<<<<<<< HEAD
static inline void mmio_insl(void __iomem *addr, u32 *dst, int count)
=======
static inline void mmio_insl(const void __iomem *addr, u32 *dst, int count)
>>>>>>> upstream/android-13
{
	while (--count >= 0) {
		u32 data = __raw_readl(addr);
		*dst = data;
		dst++;
	}
}

static inline void mmio_outsb(void __iomem *addr, const u8 *src, int count)
{
	while (--count >= 0) {
		__raw_writeb(*src, addr);
		src++;
	}
}

static inline void mmio_outsw(void __iomem *addr, const u16 *src, int count)
{
	while (--count >= 0) {
		__raw_writew(*src, addr);
		src++;
	}
}

static inline void mmio_outsl(void __iomem *addr, const u32 *src, int count)
{
	while (--count >= 0) {
		__raw_writel(*src, addr);
		src++;
	}
}

<<<<<<< HEAD
void ioread8_rep(void __iomem *addr, void *dst, unsigned long count)
=======
void ioread8_rep(const void __iomem *addr, void *dst, unsigned long count)
>>>>>>> upstream/android-13
{
	mmio_insb(addr, dst, count);
}
EXPORT_SYMBOL(ioread8_rep);

<<<<<<< HEAD
void ioread16_rep(void __iomem *addr, void *dst, unsigned long count)
=======
void ioread16_rep(const void __iomem *addr, void *dst, unsigned long count)
>>>>>>> upstream/android-13
{
	mmio_insw(addr, dst, count);
}
EXPORT_SYMBOL(ioread16_rep);

<<<<<<< HEAD
void ioread32_rep(void __iomem *addr, void *dst, unsigned long count)
=======
void ioread32_rep(const void __iomem *addr, void *dst, unsigned long count)
>>>>>>> upstream/android-13
{
	mmio_insl(addr, dst, count);
}
EXPORT_SYMBOL(ioread32_rep);

void iowrite8_rep(void __iomem *addr, const void *src, unsigned long count)
{
	mmio_outsb(addr, src, count);
}
EXPORT_SYMBOL(iowrite8_rep);

void iowrite16_rep(void __iomem *addr, const void *src, unsigned long count)
{
	mmio_outsw(addr, src, count);
}
EXPORT_SYMBOL(iowrite16_rep);

void iowrite32_rep(void __iomem *addr, const void *src, unsigned long count)
{
	mmio_outsl(addr, src, count);
}
EXPORT_SYMBOL(iowrite32_rep);
