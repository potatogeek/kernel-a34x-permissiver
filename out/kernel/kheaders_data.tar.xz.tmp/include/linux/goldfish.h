/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_GOLDFISH_H
#define __LINUX_GOLDFISH_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/io.h>



static inline void gf_write_ptr(const void *ptr, void __iomem *portl,
				void __iomem *porth)
{
	const unsigned long addr = (unsigned long)ptr;

	writel(lower_32_bits(addr), portl);
#ifdef CONFIG_64BIT
	writel(upper_32_bits(addr), porth);
#endif
}

static inline void gf_write_dma_addr(const dma_addr_t addr,
				     void __iomem *portl,
				     void __iomem *porth)
{
	writel(lower_32_bits(addr), portl);
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
	writel(upper_32_bits(addr), porth);
#endif
}


#endif 
