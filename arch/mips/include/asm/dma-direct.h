/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _MIPS_DMA_DIRECT_H
#define _MIPS_DMA_DIRECT_H 1

<<<<<<< HEAD
static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	if (!dev->dma_mask)
		return false;

	return addr + size - 1 <= *dev->dma_mask;
}

dma_addr_t __phys_to_dma(struct device *dev, phys_addr_t paddr);
phys_addr_t __dma_to_phys(struct device *dev, dma_addr_t daddr);
=======
dma_addr_t phys_to_dma(struct device *dev, phys_addr_t paddr);
phys_addr_t dma_to_phys(struct device *dev, dma_addr_t daddr);
>>>>>>> upstream/android-13

#endif /* _MIPS_DMA_DIRECT_H */
