/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ASM_POWERPC_DMA_DIRECT_H
#define ASM_POWERPC_DMA_DIRECT_H 1

<<<<<<< HEAD
static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
#ifdef CONFIG_SWIOTLB
	struct dev_archdata *sd = &dev->archdata;

	if (sd->max_direct_dma_addr && addr + size > sd->max_direct_dma_addr)
		return false;
#endif

	if (!dev->dma_mask)
		return false;

	return addr + size - 1 <= *dev->dma_mask;
}

static inline dma_addr_t __phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	return paddr + get_dma_offset(dev);
}

static inline phys_addr_t __dma_to_phys(struct device *dev, dma_addr_t daddr)
{
	return daddr - get_dma_offset(dev);
=======
static inline dma_addr_t phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	return paddr + dev->archdata.dma_offset;
}

static inline phys_addr_t dma_to_phys(struct device *dev, dma_addr_t daddr)
{
	return daddr - dev->archdata.dma_offset;
>>>>>>> upstream/android-13
}
#endif /* ASM_POWERPC_DMA_DIRECT_H */
