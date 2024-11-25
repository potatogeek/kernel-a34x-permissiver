// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2000  Ani Joshi <ajoshi@unixbox.com>
 * Copyright (C) 2000, 2001, 06	 Ralf Baechle <ralf@linux-mips.org>
 * swiped from i386, and cloned for MIPS by Geert, polished by Ralf.
 */
#include <linux/dma-direct.h>
<<<<<<< HEAD
#include <linux/dma-noncoherent.h>
#include <linux/dma-contiguous.h>
=======
#include <linux/dma-map-ops.h>
>>>>>>> upstream/android-13
#include <linux/highmem.h>

#include <asm/cache.h>
#include <asm/cpu-type.h>
<<<<<<< HEAD
#include <asm/dma-coherence.h>
#include <asm/io.h>

#ifdef CONFIG_DMA_PERDEV_COHERENT
static inline int dev_is_coherent(struct device *dev)
{
	return dev->archdata.dma_coherent;
}
#else
static inline int dev_is_coherent(struct device *dev)
{
	switch (coherentio) {
	default:
	case IO_COHERENCE_DEFAULT:
		return hw_coherentio;
	case IO_COHERENCE_ENABLED:
		return 1;
	case IO_COHERENCE_DISABLED:
		return 0;
	}
}
#endif /* CONFIG_DMA_PERDEV_COHERENT */

=======
#include <asm/io.h>

>>>>>>> upstream/android-13
/*
 * The affected CPUs below in 'cpu_needs_post_dma_flush()' can speculatively
 * fill random cachelines with stale data at any time, requiring an extra
 * flush post-DMA.
 *
 * Warning on the terminology - Linux calls an uncached area coherent;  MIPS
 * terminology calls memory areas with hardware maintained coherency coherent.
 *
 * Note that the R14000 and R16000 should also be checked for in this condition.
 * However this function is only called on non-I/O-coherent systems and only the
 * R10000 and R12000 are used in such systems, the SGI IP28 Indigo² rsp.
 * SGI IP32 aka O2.
 */
<<<<<<< HEAD
static inline bool cpu_needs_post_dma_flush(struct device *dev)
{
	if (dev_is_coherent(dev))
		return false;

=======
static inline bool cpu_needs_post_dma_flush(void)
{
>>>>>>> upstream/android-13
	switch (boot_cpu_type()) {
	case CPU_R10000:
	case CPU_R12000:
	case CPU_BMIPS5000:
<<<<<<< HEAD
=======
	case CPU_LOONGSON2EF:
	case CPU_XBURST:
>>>>>>> upstream/android-13
		return true;
	default:
		/*
		 * Presence of MAARs suggests that the CPU supports
		 * speculatively prefetching data, and therefore requires
		 * the post-DMA flush/invalidate.
		 */
		return cpu_has_maar;
	}
}

<<<<<<< HEAD
void *arch_dma_alloc(struct device *dev, size_t size,
		dma_addr_t *dma_handle, gfp_t gfp, unsigned long attrs)
{
	void *ret;

	ret = dma_direct_alloc(dev, size, dma_handle, gfp, attrs);
	if (!ret)
		return NULL;

	if (!dev_is_coherent(dev) && !(attrs & DMA_ATTR_NON_CONSISTENT)) {
		dma_cache_wback_inv((unsigned long) ret, size);
		ret = (void *)UNCAC_ADDR(ret);
	}

	return ret;
}

void arch_dma_free(struct device *dev, size_t size, void *cpu_addr,
		dma_addr_t dma_addr, unsigned long attrs)
{
	if (!(attrs & DMA_ATTR_NON_CONSISTENT) && !dev_is_coherent(dev))
		cpu_addr = (void *)CAC_ADDR((unsigned long)cpu_addr);
	dma_direct_free(dev, size, cpu_addr, dma_addr, attrs);
}

int arch_dma_mmap(struct device *dev, struct vm_area_struct *vma,
		void *cpu_addr, dma_addr_t dma_addr, size_t size,
		unsigned long attrs)
{
	unsigned long user_count = vma_pages(vma);
	unsigned long count = PAGE_ALIGN(size) >> PAGE_SHIFT;
	unsigned long addr = (unsigned long)cpu_addr;
	unsigned long off = vma->vm_pgoff;
	unsigned long pfn;
	int ret = -ENXIO;

	if (!dev_is_coherent(dev))
		addr = CAC_ADDR(addr);

	pfn = page_to_pfn(virt_to_page((void *)addr));

	if (attrs & DMA_ATTR_WRITE_COMBINE)
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	else
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	if (dma_mmap_from_dev_coherent(dev, vma, cpu_addr, size, &ret))
		return ret;

	if (off < count && user_count <= (count - off)) {
		ret = remap_pfn_range(vma, vma->vm_start,
				      pfn + off,
				      user_count << PAGE_SHIFT,
				      vma->vm_page_prot);
	}

	return ret;
}

static inline void dma_sync_virt(void *addr, size_t size,
=======
void arch_dma_prep_coherent(struct page *page, size_t size)
{
	dma_cache_wback_inv((unsigned long)page_address(page), size);
}

void *arch_dma_set_uncached(void *addr, size_t size)
{
	return (void *)(__pa(addr) + UNCAC_BASE);
}

static inline void dma_sync_virt_for_device(void *addr, size_t size,
>>>>>>> upstream/android-13
		enum dma_data_direction dir)
{
	switch (dir) {
	case DMA_TO_DEVICE:
		dma_cache_wback((unsigned long)addr, size);
		break;
<<<<<<< HEAD

	case DMA_FROM_DEVICE:
		dma_cache_inv((unsigned long)addr, size);
		break;

	case DMA_BIDIRECTIONAL:
		dma_cache_wback_inv((unsigned long)addr, size);
		break;

=======
	case DMA_FROM_DEVICE:
		dma_cache_inv((unsigned long)addr, size);
		break;
	case DMA_BIDIRECTIONAL:
		dma_cache_wback_inv((unsigned long)addr, size);
		break;
	default:
		BUG();
	}
}

static inline void dma_sync_virt_for_cpu(void *addr, size_t size,
		enum dma_data_direction dir)
{
	switch (dir) {
	case DMA_TO_DEVICE:
		break;
	case DMA_FROM_DEVICE:
	case DMA_BIDIRECTIONAL:
		dma_cache_inv((unsigned long)addr, size);
		break;
>>>>>>> upstream/android-13
	default:
		BUG();
	}
}

/*
 * A single sg entry may refer to multiple physically contiguous pages.  But
 * we still need to process highmem pages individually.  If highmem is not
 * configured then the bulk of this loop gets optimized out.
 */
static inline void dma_sync_phys(phys_addr_t paddr, size_t size,
<<<<<<< HEAD
		enum dma_data_direction dir)
=======
		enum dma_data_direction dir, bool for_device)
>>>>>>> upstream/android-13
{
	struct page *page = pfn_to_page(paddr >> PAGE_SHIFT);
	unsigned long offset = paddr & ~PAGE_MASK;
	size_t left = size;

	do {
		size_t len = left;
<<<<<<< HEAD

		if (PageHighMem(page)) {
			void *addr;

			if (offset + len > PAGE_SIZE) {
				if (offset >= PAGE_SIZE) {
					page += offset >> PAGE_SHIFT;
					offset &= ~PAGE_MASK;
				}
				len = PAGE_SIZE - offset;
			}

			addr = kmap_atomic(page);
			dma_sync_virt(addr + offset, len, dir);
			kunmap_atomic(addr);
		} else
			dma_sync_virt(page_address(page) + offset, size, dir);
=======
		void *addr;

		if (PageHighMem(page)) {
			if (offset + len > PAGE_SIZE)
				len = PAGE_SIZE - offset;
		}

		addr = kmap_atomic(page);
		if (for_device)
			dma_sync_virt_for_device(addr + offset, len, dir);
		else
			dma_sync_virt_for_cpu(addr + offset, len, dir);
		kunmap_atomic(addr);

>>>>>>> upstream/android-13
		offset = 0;
		page++;
		left -= len;
	} while (left);
}

<<<<<<< HEAD
void arch_sync_dma_for_device(struct device *dev, phys_addr_t paddr,
		size_t size, enum dma_data_direction dir)
{
	if (!dev_is_coherent(dev))
		dma_sync_phys(paddr, size, dir);
}

void arch_sync_dma_for_cpu(struct device *dev, phys_addr_t paddr,
		size_t size, enum dma_data_direction dir)
{
	if (cpu_needs_post_dma_flush(dev))
		dma_sync_phys(paddr, size, dir);
}

void arch_dma_cache_sync(struct device *dev, void *vaddr, size_t size,
		enum dma_data_direction direction)
{
	BUG_ON(direction == DMA_NONE);

	if (!dev_is_coherent(dev))
		dma_sync_virt(vaddr, size, direction);
}
=======
void arch_sync_dma_for_device(phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
{
	dma_sync_phys(paddr, size, dir, true);
}

#ifdef CONFIG_ARCH_HAS_SYNC_DMA_FOR_CPU
void arch_sync_dma_for_cpu(phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
{
	if (cpu_needs_post_dma_flush())
		dma_sync_phys(paddr, size, dir, false);
}
#endif

#ifdef CONFIG_ARCH_HAS_SETUP_DMA_OPS
void arch_setup_dma_ops(struct device *dev, u64 dma_base, u64 size,
		const struct iommu_ops *iommu, bool coherent)
{
	dev->dma_coherent = coherent;
}
#endif
>>>>>>> upstream/android-13
