<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * A fairly generic DMA-API to IOMMU-API glue layer.
 *
 * Copyright (C) 2014-2015 ARM Ltd.
 *
 * based in part on arch/arm/mm/dma-mapping.c:
 * Copyright (C) 2000-2004 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/acpi_iort.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/dma-map-ops.h>
>>>>>>> upstream/android-13
#include <linux/dma-iommu.h>
#include <linux/gfp.h>
#include <linux/huge_mm.h>
#include <linux/iommu.h>
#include <linux/iova.h>
#include <linux/irq.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/pci.h>
#include <linux/scatterlist.h>
#include <linux/vmalloc.h>
#ifdef CONFIG_MTK_IOMMU_V2
#include "mtk_iommu_ext.h"
#endif
#ifdef CONFIG_MTK_IOMMU_MISC_DBG
#include "m4u_debug.h"
#endif

#define IOMMU_MAPPING_ERROR	0
=======
#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/swiotlb.h>
#include <linux/scatterlist.h>
#include <linux/vmalloc.h>
#include <linux/crash_dump.h>
#include <linux/dma-direct.h>
#include <trace/hooks/iommu.h>
>>>>>>> upstream/android-13

struct iommu_dma_msi_page {
	struct list_head	list;
	dma_addr_t		iova;
	phys_addr_t		phys;
};

enum iommu_dma_cookie_type {
	IOMMU_DMA_IOVA_COOKIE,
	IOMMU_DMA_MSI_COOKIE,
};

struct iommu_dma_cookie {
	enum iommu_dma_cookie_type	type;
	union {
		/* Full allocator for IOMMU_DMA_IOVA_COOKIE */
		struct iova_domain	iovad;
		/* Trivial linear page allocator for IOMMU_DMA_MSI_COOKIE */
		dma_addr_t		msi_iova;
	};
	struct list_head		msi_page_list;
<<<<<<< HEAD
	spinlock_t			msi_lock;
};

=======

	/* Domain for flush queue callback; NULL if flush queue not in use */
	struct iommu_domain		*fq_domain;
};

struct iommu_dma_cookie_ext {
	struct iommu_dma_cookie		cookie;
	struct mutex			mutex;
};

static DEFINE_STATIC_KEY_FALSE(iommu_deferred_attach_enabled);
bool iommu_dma_forcedac __read_mostly;

static int __init iommu_dma_forcedac_setup(char *str)
{
	int ret = kstrtobool(str, &iommu_dma_forcedac);

	if (!ret && iommu_dma_forcedac)
		pr_info("Forcing DAC for PCI devices\n");
	return ret;
}
early_param("iommu.forcedac", iommu_dma_forcedac_setup);

static void iommu_dma_entry_dtor(unsigned long data)
{
	struct page *freelist = (struct page *)data;

	while (freelist) {
		unsigned long p = (unsigned long)page_address(freelist);

		freelist = freelist->freelist;
		free_page(p);
	}
}

>>>>>>> upstream/android-13
static inline size_t cookie_msi_granule(struct iommu_dma_cookie *cookie)
{
	if (cookie->type == IOMMU_DMA_IOVA_COOKIE)
		return cookie->iovad.granule;
	return PAGE_SIZE;
}

static struct iommu_dma_cookie *cookie_alloc(enum iommu_dma_cookie_type type)
{
<<<<<<< HEAD
	struct iommu_dma_cookie *cookie;

	cookie = kzalloc(sizeof(*cookie), GFP_KERNEL);
	if (cookie) {
		spin_lock_init(&cookie->msi_lock);
		INIT_LIST_HEAD(&cookie->msi_page_list);
		cookie->type = type;
	}
	return cookie;
}

int iommu_dma_init(void)
{
	return iova_cache_get();
=======
	struct iommu_dma_cookie_ext *cookie;

	cookie = kzalloc(sizeof(*cookie), GFP_KERNEL);
	if (cookie) {
		INIT_LIST_HEAD(&cookie->cookie.msi_page_list);
		cookie->cookie.type = type;
		mutex_init(&cookie->mutex);
	}
	return &cookie->cookie;
>>>>>>> upstream/android-13
}

/**
 * iommu_get_dma_cookie - Acquire DMA-API resources for a domain
 * @domain: IOMMU domain to prepare for DMA-API usage
 *
 * IOMMU drivers should normally call this from their domain_alloc
 * callback when domain->type == IOMMU_DOMAIN_DMA.
 */
int iommu_get_dma_cookie(struct iommu_domain *domain)
{
	if (domain->iova_cookie)
		return -EEXIST;

	domain->iova_cookie = cookie_alloc(IOMMU_DMA_IOVA_COOKIE);
	if (!domain->iova_cookie)
		return -ENOMEM;

	return 0;
}
EXPORT_SYMBOL(iommu_get_dma_cookie);

/**
 * iommu_get_msi_cookie - Acquire just MSI remapping resources
 * @domain: IOMMU domain to prepare
 * @base: Start address of IOVA region for MSI mappings
 *
 * Users who manage their own IOVA allocation and do not want DMA API support,
 * but would still like to take advantage of automatic MSI remapping, can use
 * this to initialise their own domain appropriately. Users should reserve a
 * contiguous IOVA region, starting at @base, large enough to accommodate the
 * number of PAGE_SIZE mappings necessary to cover every MSI doorbell address
 * used by the devices attached to @domain.
 */
int iommu_get_msi_cookie(struct iommu_domain *domain, dma_addr_t base)
{
	struct iommu_dma_cookie *cookie;

	if (domain->type != IOMMU_DOMAIN_UNMANAGED)
		return -EINVAL;

	if (domain->iova_cookie)
		return -EEXIST;

	cookie = cookie_alloc(IOMMU_DMA_MSI_COOKIE);
	if (!cookie)
		return -ENOMEM;

	cookie->msi_iova = base;
	domain->iova_cookie = cookie;
	return 0;
}
EXPORT_SYMBOL(iommu_get_msi_cookie);

/**
 * iommu_put_dma_cookie - Release a domain's DMA mapping resources
 * @domain: IOMMU domain previously prepared by iommu_get_dma_cookie() or
 *          iommu_get_msi_cookie()
 *
 * IOMMU drivers should normally call this from their domain_free callback.
 */
void iommu_put_dma_cookie(struct iommu_domain *domain)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iommu_dma_msi_page *msi, *tmp;

	if (!cookie)
		return;

	if (cookie->type == IOMMU_DMA_IOVA_COOKIE && cookie->iovad.granule)
		put_iova_domain(&cookie->iovad);

	list_for_each_entry_safe(msi, tmp, &cookie->msi_page_list, list) {
		list_del(&msi->list);
		kfree(msi);
	}
	kfree(cookie);
	domain->iova_cookie = NULL;
}
EXPORT_SYMBOL(iommu_put_dma_cookie);

/**
 * iommu_dma_get_resv_regions - Reserved region driver helper
 * @dev: Device from iommu_get_resv_regions()
 * @list: Reserved region list from iommu_get_resv_regions()
 *
 * IOMMU drivers can use this to implement their .get_resv_regions callback
 * for general non-IOMMU-specific reservations. Currently, this covers GICv3
 * ITS region reservation on ACPI based ARM platforms that may require HW MSI
 * reservation.
 */
void iommu_dma_get_resv_regions(struct device *dev, struct list_head *list)
{

<<<<<<< HEAD
	if (!is_of_node(dev->iommu_fwspec->iommu_fwnode))
=======
	if (!is_of_node(dev_iommu_fwspec_get(dev)->iommu_fwnode))
>>>>>>> upstream/android-13
		iort_iommu_msi_get_resv_regions(dev, list);

}
EXPORT_SYMBOL(iommu_dma_get_resv_regions);

static int cookie_init_hw_msi_region(struct iommu_dma_cookie *cookie,
		phys_addr_t start, phys_addr_t end)
{
	struct iova_domain *iovad = &cookie->iovad;
	struct iommu_dma_msi_page *msi_page;
	int i, num_pages;

	start -= iova_offset(iovad, start);
	num_pages = iova_align(iovad, end - start) >> iova_shift(iovad);

	for (i = 0; i < num_pages; i++) {
		msi_page = kmalloc(sizeof(*msi_page), GFP_KERNEL);
		if (!msi_page)
			return -ENOMEM;

		msi_page->phys = start;
		msi_page->iova = start;
		INIT_LIST_HEAD(&msi_page->list);
		list_add(&msi_page->list, &cookie->msi_page_list);
		start += iovad->granule;
	}

	return 0;
}

<<<<<<< HEAD
static void iova_reserve_pci_windows(struct pci_dev *dev,
=======
static int iova_reserve_pci_windows(struct pci_dev *dev,
>>>>>>> upstream/android-13
		struct iova_domain *iovad)
{
	struct pci_host_bridge *bridge = pci_find_host_bridge(dev->bus);
	struct resource_entry *window;
	unsigned long lo, hi;
<<<<<<< HEAD
=======
	phys_addr_t start = 0, end;
>>>>>>> upstream/android-13

	resource_list_for_each_entry(window, &bridge->windows) {
		if (resource_type(window->res) != IORESOURCE_MEM)
			continue;

		lo = iova_pfn(iovad, window->res->start - window->offset);
		hi = iova_pfn(iovad, window->res->end - window->offset);
		reserve_iova(iovad, lo, hi);
	}
<<<<<<< HEAD
=======

	/* Get reserved DMA windows from host bridge */
	resource_list_for_each_entry(window, &bridge->dma_ranges) {
		end = window->res->start - window->offset;
resv_iova:
		if (end > start) {
			lo = iova_pfn(iovad, start);
			hi = iova_pfn(iovad, end);
			reserve_iova(iovad, lo, hi);
		} else if (end < start) {
			/* dma_ranges list should be sorted */
			dev_err(&dev->dev,
				"Failed to reserve IOVA [%pa-%pa]\n",
				&start, &end);
			return -EINVAL;
		}

		start = window->res->end - window->offset + 1;
		/* If window is last entry */
		if (window->node.next == &bridge->dma_ranges &&
		    end != ~(phys_addr_t)0) {
			end = ~(phys_addr_t)0;
			goto resv_iova;
		}
	}

	return 0;
>>>>>>> upstream/android-13
}

static int iova_reserve_iommu_regions(struct device *dev,
		struct iommu_domain *domain)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	struct iommu_resv_region *region;
	LIST_HEAD(resv_regions);
	int ret = 0;

<<<<<<< HEAD
	if (dev_is_pci(dev))
		iova_reserve_pci_windows(to_pci_dev(dev), iovad);
=======
	if (dev_is_pci(dev)) {
		ret = iova_reserve_pci_windows(to_pci_dev(dev), iovad);
		if (ret)
			return ret;
	}
>>>>>>> upstream/android-13

	iommu_get_resv_regions(dev, &resv_regions);
	list_for_each_entry(region, &resv_regions, list) {
		unsigned long lo, hi;

		/* We ARE the software that manages these! */
		if (region->type == IOMMU_RESV_SW_MSI)
			continue;

		lo = iova_pfn(iovad, region->start);
		hi = iova_pfn(iovad, region->start + region->length - 1);
		reserve_iova(iovad, lo, hi);
<<<<<<< HEAD
#ifdef IOMMU_DEBUG_ENABLED
		pr_notice("%s, %d, reserved iova from 0x%lx to 0x%lx\n",
			  __func__, __LINE__, lo << 12, hi << 12);
#endif
=======

>>>>>>> upstream/android-13
		if (region->type == IOMMU_RESV_MSI)
			ret = cookie_init_hw_msi_region(cookie, region->start,
					region->start + region->length);
		if (ret)
			break;
	}
	iommu_put_resv_regions(dev, &resv_regions);

	return ret;
}

<<<<<<< HEAD
=======
static void iommu_dma_flush_iotlb_all(struct iova_domain *iovad)
{
	struct iommu_dma_cookie *cookie;
	struct iommu_domain *domain;

	cookie = container_of(iovad, struct iommu_dma_cookie, iovad);
	domain = cookie->fq_domain;

	domain->ops->flush_iotlb_all(domain);
}

static bool dev_is_untrusted(struct device *dev)
{
	return dev_is_pci(dev) && to_pci_dev(dev)->untrusted;
}

static bool dev_use_swiotlb(struct device *dev)
{
	return IS_ENABLED(CONFIG_SWIOTLB) && dev_is_untrusted(dev);
}

/* sysfs updates are serialised by the mutex of the group owning @domain */
int iommu_dma_init_fq(struct iommu_domain *domain)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	int ret;

	if (cookie->fq_domain)
		return 0;

	ret = init_iova_flush_queue(&cookie->iovad, iommu_dma_flush_iotlb_all,
				    iommu_dma_entry_dtor);
	if (ret) {
		pr_warn("iova flush queue initialization failed\n");
		return ret;
	}
	/*
	 * Prevent incomplete iovad->fq being observable. Pairs with path from
	 * __iommu_dma_unmap() through iommu_dma_free_iova() to queue_iova()
	 */
	smp_wmb();
	WRITE_ONCE(cookie->fq_domain, domain);
	return 0;
}

>>>>>>> upstream/android-13
/**
 * iommu_dma_init_domain - Initialise a DMA mapping domain
 * @domain: IOMMU domain previously prepared by iommu_get_dma_cookie()
 * @base: IOVA at which the mappable address space starts
<<<<<<< HEAD
 * @size: Size of IOVA space
 * @dev: Device the domain is being initialised for
 *
 * @base and @size should be exact multiples of IOMMU page granularity to
=======
 * @limit: Last address of the IOVA space
 * @dev: Device the domain is being initialised for
 *
 * @base and @limit + 1 should be exact multiples of IOMMU page granularity to
>>>>>>> upstream/android-13
 * avoid rounding surprises. If necessary, we reserve the page at address 0
 * to ensure it is an invalid IOVA. It is safe to reinitialise a domain, but
 * any change which could make prior IOVAs invalid will fail.
 */
<<<<<<< HEAD
int iommu_dma_init_domain(struct iommu_domain *domain, dma_addr_t base,
		u64 size, struct device *dev)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	unsigned long order, base_pfn, end_pfn;
=======
static int iommu_dma_init_domain(struct iommu_domain *domain, dma_addr_t base,
				 dma_addr_t limit, struct device *dev)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iommu_dma_cookie_ext *cookie_ext;
	unsigned long order, base_pfn;
	struct iova_domain *iovad;
	int ret;
>>>>>>> upstream/android-13

	if (!cookie || cookie->type != IOMMU_DMA_IOVA_COOKIE)
		return -EINVAL;

<<<<<<< HEAD
	/* Use the smallest supported page size for IOVA granularity */
	order = __ffs(domain->pgsize_bitmap);
	base_pfn = max_t(unsigned long, 1, base >> order);
	end_pfn = (base + size - 1) >> order;
=======
	iovad = &cookie->iovad;

	/* Use the smallest supported page size for IOVA granularity */
	order = __ffs(domain->pgsize_bitmap);
	base_pfn = max_t(unsigned long, 1, base >> order);
>>>>>>> upstream/android-13

	/* Check the domain allows at least some access to the device... */
	if (domain->geometry.force_aperture) {
		if (base > domain->geometry.aperture_end ||
<<<<<<< HEAD
		    base + size <= domain->geometry.aperture_start) {
#ifndef CONFIG_MTK_IOMMU_V2
			pr_warn("specified DMA range outside IOMMU capability, base:0x%lx, size:0x%lx, aperture(0x%lx, 0x%lx)\n",
				base, size, domain->geometry.aperture_start,
				domain->geometry.aperture_end);
			return -EFAULT;
#else
			base = domain->geometry.aperture_start;
			size = domain->geometry.aperture_end -
				domain->geometry.aperture_start + 1;
#endif
=======
		    limit < domain->geometry.aperture_start) {
			pr_warn("specified DMA range outside IOMMU capability\n");
			return -EFAULT;
>>>>>>> upstream/android-13
		}
		/* ...then finally give it a kicking to make sure it fits */
		base_pfn = max_t(unsigned long, base_pfn,
				domain->geometry.aperture_start >> order);
	}

	/* start_pfn is always nonzero for an already-initialised domain */
<<<<<<< HEAD
=======
	cookie_ext = container_of(cookie, struct iommu_dma_cookie_ext, cookie);
	mutex_lock(&cookie_ext->mutex);
>>>>>>> upstream/android-13
	if (iovad->start_pfn) {
		if (1UL << order != iovad->granule ||
		    base_pfn != iovad->start_pfn) {
			pr_warn("Incompatible range for DMA domain\n");
<<<<<<< HEAD
			return -EFAULT;
		}

		return 0;
	}

	iovad->end_pfn = end_pfn;
	init_iova_domain(iovad, 1UL << order, base_pfn);
	if (!dev)
		return 0;

	return iova_reserve_iommu_regions(dev, domain);
}
EXPORT_SYMBOL(iommu_dma_init_domain);

/*
 * Should be called prior to using dma-apis
 */
int iommu_dma_reserve_iova(struct device *dev, dma_addr_t base,
			   u64 size)
{
	struct iommu_domain *domain;
	struct iova_domain *iovad;
	unsigned long pfn_lo, pfn_hi;

	domain = iommu_get_domain_for_dev(dev);
	if (!domain || !domain->iova_cookie)
		return -EINVAL;

	iovad = &((struct iommu_dma_cookie *)domain->iova_cookie)->iovad;

	/* iova will be freed automatically by put_iova_domain() */
	pfn_lo = iova_pfn(iovad, base);
	pfn_hi = iova_pfn(iovad, base + size - 1);
	if (!reserve_iova(iovad, pfn_lo, pfn_hi))
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL_GPL(iommu_dma_reserve_iova);
=======
			ret = -EFAULT;
			goto done_unlock;
		}

		ret = 0;
		goto done_unlock;
	}

	init_iova_domain(iovad, 1UL << order, base_pfn);

	/* If the FQ fails we can simply fall back to strict mode */
	if (domain->type == IOMMU_DOMAIN_DMA_FQ && iommu_dma_init_fq(domain))
		domain->type = IOMMU_DOMAIN_DMA;

	ret = iova_reserve_iommu_regions(dev, domain);

done_unlock:
	mutex_unlock(&cookie_ext->mutex);
	return ret;
}
>>>>>>> upstream/android-13

/*
 * Should be called prior to using dma-apis.
 */
int iommu_dma_enable_best_fit_algo(struct device *dev)
{
	struct iommu_domain *domain;
	struct iova_domain *iovad;

	domain = iommu_get_domain_for_dev(dev);
	if (!domain || !domain->iova_cookie)
		return -EINVAL;

	iovad = &((struct iommu_dma_cookie *)domain->iova_cookie)->iovad;
	iovad->best_fit = true;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(iommu_dma_enable_best_fit_algo);
=======
EXPORT_SYMBOL(iommu_dma_enable_best_fit_algo);
>>>>>>> upstream/android-13

/**
 * dma_info_to_prot - Translate DMA API directions and attributes to IOMMU API
 *                    page flags.
 * @dir: Direction of DMA transfer
 * @coherent: Is the DMA master cache-coherent?
 * @attrs: DMA attributes for the mapping
 *
 * Return: corresponding IOMMU API page protection flags
 */
<<<<<<< HEAD
int dma_info_to_prot(enum dma_data_direction dir, bool coherent,
=======
static int dma_info_to_prot(enum dma_data_direction dir, bool coherent,
>>>>>>> upstream/android-13
		     unsigned long attrs)
{
	int prot = coherent ? IOMMU_CACHE : 0;

	if (attrs & DMA_ATTR_PRIVILEGED)
		prot |= IOMMU_PRIV;
<<<<<<< HEAD

	if (!(attrs & DMA_ATTR_EXEC_MAPPING))
		prot |= IOMMU_NOEXEC;

	if (attrs & DMA_ATTR_IOMMU_USE_UPSTREAM_HINT)
		prot |= IOMMU_USE_UPSTREAM_HINT;

	if (attrs & DMA_ATTR_IOMMU_USE_LLC_NWA)
		prot |= IOMMU_USE_LLC_NWA;
=======
	if (attrs & DMA_ATTR_SYS_CACHE_ONLY)
		prot |= IOMMU_SYS_CACHE;
	if (attrs & DMA_ATTR_SYS_CACHE_ONLY_NWA)
		prot |= IOMMU_SYS_CACHE_NWA;

	if (attrs & DMA_ATTR_HAS_PRIV_DATA)
		prot |= DMA_ATTR_TO_PRIV_PROT(attrs) << IOMMU_PRIV_SHIFT;
>>>>>>> upstream/android-13

	switch (dir) {
	case DMA_BIDIRECTIONAL:
		return prot | IOMMU_READ | IOMMU_WRITE;
	case DMA_TO_DEVICE:
		return prot | IOMMU_READ;
	case DMA_FROM_DEVICE:
		return prot | IOMMU_WRITE;
	default:
		return 0;
	}
}

static dma_addr_t iommu_dma_alloc_iova(struct iommu_domain *domain,
<<<<<<< HEAD
		size_t size, dma_addr_t dma_limit, struct device *dev)
=======
		size_t size, u64 dma_limit, struct device *dev)
>>>>>>> upstream/android-13
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	unsigned long shift, iova_len, iova = 0;
<<<<<<< HEAD
	dma_addr_t limit;
#ifdef CONFIG_MTK_IOMMU_V2
	bool size_align = true;
#endif
=======
>>>>>>> upstream/android-13

	if (cookie->type == IOMMU_DMA_MSI_COOKIE) {
		cookie->msi_iova += size;
		return cookie->msi_iova - size;
	}

	shift = iova_shift(iovad);
	iova_len = size >> shift;
	/*
	 * Freeing non-power-of-two-sized allocations back into the IOVA caches
	 * will come back to bite us badly, so we have to waste a bit of space
	 * rounding up anything cacheable to make sure that can't happen. The
	 * order of the unadjusted size will still match upon freeing.
	 */
	if (iova_len < (1 << (IOVA_RANGE_CACHE_MAX_SIZE - 1)))
		iova_len = roundup_pow_of_two(iova_len);

<<<<<<< HEAD
	if (dev->bus_dma_mask)
		dma_limit &= dev->bus_dma_mask;

	if (domain->geometry.force_aperture) {
		dma_limit = min(dma_limit, domain->geometry.aperture_end);
#ifdef CONFIG_MTK_IOMMU_V2
		/*
		 * if user use a smaller dev->dma_coherence_mask,
		 * then the IOVA boundary config will be failed,
		 * because iovad->dma_32bit_pfn is not the size limit,
		 * and the size limit is totally decided by the
		 * limit_pfn send from user.
		 */
		if (dma_limit <= domain->geometry.aperture_start) {
			dev_notice(dev, "dma limit:0x%lx, is out of domain boundary(0x%lx~0x%lx)",
				   dma_limit, domain->geometry.aperture_start,
				   domain->geometry.aperture_end);
			WARN_ON(1);
			return 0;
		}

		if (size > (dma_limit -
			domain->geometry.aperture_start + 1)) {
			dev_notice(dev, "size:0x%lx, is out of dma limit(0x%lx~0x%lx)",
				   size, domain->geometry.aperture_start,
				   dma_limit);
			WARN_ON(1);
			return 0;
		}
#endif
	}
	/*
	 * Ensure iova is within range specified in iommu_dma_init_domain().
	 * This also prevents unnecessary work iterating through the entire
	 * rb_tree.
	 */
	limit = min_t(dma_addr_t, DMA_BIT_MASK(32) >> shift,
						iovad->end_pfn);

	/* Try to get PCI devices a SAC address */
#ifdef CONFIG_MTK_IOMMU_V2
	size_align = mtk_dev_is_size_alignment(dev);

	if (dma_limit > DMA_BIT_MASK(32) && dev_is_pci(dev))
		iova = alloc_iova_fast(iovad, iova_len,
				DMA_BIT_MASK(32) >> shift, size_align);

	if (!iova)
		iova = alloc_iova_fast(iovad, iova_len,
				dma_limit >> shift, size_align);
#else
	if (dma_limit > DMA_BIT_MASK(32) && dev_is_pci(dev))
		iova = alloc_iova_fast(iovad, iova_len, limit, false);

	if (!iova) {
		limit = min_t(dma_addr_t, dma_limit >> shift,
						iovad->end_pfn);

		iova = alloc_iova_fast(iovad, iova_len, limit, true);
	}
#endif

#ifdef CONFIG_MTK_IOMMU_MISC_DBG
	if (iova)
		mtk_iova_dbg_alloc(dev, ((dma_addr_t)iova << shift), size);
#endif

	return (dma_addr_t)iova << shift;

}

static void iommu_dma_free_iova(struct iommu_dma_cookie *cookie,
		dma_addr_t iova, size_t size)
=======
	dma_limit = min_not_zero(dma_limit, dev->bus_dma_limit);

	if (domain->geometry.force_aperture)
		dma_limit = min(dma_limit, (u64)domain->geometry.aperture_end);

	/* Try to get PCI devices a SAC address */
	if (dma_limit > DMA_BIT_MASK(32) && !iommu_dma_forcedac && dev_is_pci(dev))
		iova = alloc_iova_fast(iovad, iova_len,
				       DMA_BIT_MASK(32) >> shift, false);

	if (!iova)
		iova = alloc_iova_fast(iovad, iova_len, dma_limit >> shift,
				       true);

	trace_android_vh_iommu_iovad_alloc_iova(dev, iovad, (dma_addr_t)iova << shift, size);

	return (dma_addr_t)iova << shift;
}

static void iommu_dma_free_iova(struct iommu_dma_cookie *cookie,
		dma_addr_t iova, size_t size, struct iommu_iotlb_gather *gather)
>>>>>>> upstream/android-13
{
	struct iova_domain *iovad = &cookie->iovad;

	/* The MSI case is only ever cleaning up its most recent allocation */
	if (cookie->type == IOMMU_DMA_MSI_COOKIE)
		cookie->msi_iova -= size;
<<<<<<< HEAD
	else
		free_iova_fast(iovad, iova_pfn(iovad, iova),
				size >> iova_shift(iovad));
#ifdef CONFIG_MTK_IOMMU_MISC_DBG
	mtk_iova_dbg_free(iova, size);
#endif
}

static void __iommu_dma_unmap(struct iommu_domain *domain, dma_addr_t dma_addr,
		size_t size)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	size_t iova_off = iova_offset(iovad, dma_addr);

	dma_addr -= iova_off;
	size = iova_align(iovad, size + iova_off);

	WARN_ON(iommu_unmap(domain, dma_addr, size) != size);
	iommu_dma_free_iova(cookie, dma_addr, size);
=======
	else if (gather && gather->queued)
		queue_iova(iovad, iova_pfn(iovad, iova),
				size >> iova_shift(iovad),
				(unsigned long)gather->freelist);
	else
		free_iova_fast(iovad, iova_pfn(iovad, iova),
				size >> iova_shift(iovad));

	trace_android_vh_iommu_iovad_free_iova(iovad, iova, size);
}

static void __iommu_dma_unmap(struct device *dev, dma_addr_t dma_addr,
		size_t size)
{
	struct iommu_domain *domain = iommu_get_dma_domain(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	size_t iova_off = iova_offset(iovad, dma_addr);
	struct iommu_iotlb_gather iotlb_gather;
	size_t unmapped;

	dma_addr -= iova_off;
	size = iova_align(iovad, size + iova_off);
	iommu_iotlb_gather_init(&iotlb_gather);
	iotlb_gather.queued = READ_ONCE(cookie->fq_domain);

	unmapped = iommu_unmap_fast(domain, dma_addr, size, &iotlb_gather);
	WARN_ON(unmapped != size);

	if (!iotlb_gather.queued)
		iommu_iotlb_sync(domain, &iotlb_gather);
	iommu_dma_free_iova(cookie, dma_addr, size, &iotlb_gather);
}

static dma_addr_t __iommu_dma_map(struct device *dev, phys_addr_t phys,
		size_t size, int prot, u64 dma_mask)
{
	struct iommu_domain *domain = iommu_get_dma_domain(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	size_t iova_off = iova_offset(iovad, phys);
	dma_addr_t iova;

	if (static_branch_unlikely(&iommu_deferred_attach_enabled) &&
	    iommu_deferred_attach(dev, domain))
		return DMA_MAPPING_ERROR;

	size = iova_align(iovad, size + iova_off);

	iova = iommu_dma_alloc_iova(domain, size, dma_mask, dev);
	if (!iova)
		return DMA_MAPPING_ERROR;

	if (iommu_map_atomic(domain, iova, phys - iova_off, size, prot)) {
		iommu_dma_free_iova(cookie, iova, size, NULL);
		return DMA_MAPPING_ERROR;
	}
	return iova + iova_off;
>>>>>>> upstream/android-13
}

static void __iommu_dma_free_pages(struct page **pages, int count)
{
	while (count--)
		__free_page(pages[count]);
	kvfree(pages);
}

<<<<<<< HEAD
static struct page **__iommu_dma_alloc_pages(unsigned int count,
		unsigned long order_mask, gfp_t gfp)
{
	struct page **pages;
	unsigned int i = 0, array_size = count * sizeof(*pages);
=======
static struct page **__iommu_dma_alloc_pages(struct device *dev,
		unsigned int count, unsigned long order_mask, gfp_t gfp)
{
	struct page **pages;
	unsigned int i = 0, nid = dev_to_node(dev);
>>>>>>> upstream/android-13

	order_mask &= (2U << MAX_ORDER) - 1;
	if (!order_mask)
		return NULL;

<<<<<<< HEAD
	if (array_size <= PAGE_SIZE)
		pages = kzalloc(array_size, GFP_KERNEL);
	else
		pages = vzalloc(array_size);
=======
	pages = kvzalloc(count * sizeof(*pages), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!pages)
		return NULL;

	/* IOMMU can map any pages, so himem can also be used here */
	gfp |= __GFP_NOWARN | __GFP_HIGHMEM;

<<<<<<< HEAD
=======
	/* It makes no sense to muck about with huge pages */
	gfp &= ~__GFP_COMP;

>>>>>>> upstream/android-13
	while (count) {
		struct page *page = NULL;
		unsigned int order_size;

		/*
		 * Higher-order allocations are a convenience rather
		 * than a necessity, hence using __GFP_NORETRY until
		 * falling back to minimum-order allocations.
		 */
		for (order_mask &= (2U << __fls(count)) - 1;
		     order_mask; order_mask &= ~order_size) {
			unsigned int order = __fls(order_mask);
<<<<<<< HEAD

			order_size = 1U << order;
			page = alloc_pages((order_mask - order_size) ?
					   gfp | __GFP_NORETRY : gfp, order);
			if (!page)
				continue;
			if (!order)
				break;
			if (!PageCompound(page)) {
				split_page(page, order);
				break;
			} else if (!split_huge_page(page)) {
				break;
			}
			__free_pages(page, order);
=======
			gfp_t alloc_flags = gfp;

			order_size = 1U << order;
			if (order_mask > order_size)
				alloc_flags |= __GFP_NORETRY;
			page = alloc_pages_node(nid, alloc_flags, order);
			if (!page)
				continue;
			if (order)
				split_page(page, order);
			break;
>>>>>>> upstream/android-13
		}
		if (!page) {
			__iommu_dma_free_pages(pages, i);
			return NULL;
		}
		count -= order_size;
		while (order_size--)
			pages[i++] = page++;
	}
	return pages;
}

<<<<<<< HEAD
#ifdef CONFIG_ARM64
void iommu_dma_free_from_reserved_range(struct device *dev,
		struct page **pages, size_t size, dma_addr_t *handle)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie;
	struct iova_domain *iovad;
	unsigned long shift;
	unsigned long pfn;
	size_t unmap_sz;

	if (!domain)
		return;
	cookie = domain->iova_cookie;
	iovad = &cookie->iovad;
	shift = iova_shift(iovad);
	pfn = (*handle) >> shift;

	unmap_sz = iommu_unmap(domain, pfn << shift, size);
	WARN_ON(size > unmap_sz);

	__iommu_dma_free_pages(pages, PAGE_ALIGN(size) >> PAGE_SHIFT);
	*handle = IOMMU_MAPPING_ERROR;
}
#endif
/**
 * iommu_dma_free - Free a buffer allocated by iommu_dma_alloc()
 * @dev: Device which owns this buffer
 * @pages: Array of buffer pages as returned by iommu_dma_alloc()
 * @size: Size of buffer in bytes
 * @handle: DMA address of buffer
 *
 * Frees both the pages associated with the buffer, and the array
 * describing them
 */
void iommu_dma_free(struct device *dev, struct page **pages, size_t size,
		dma_addr_t *handle)
{
	__iommu_dma_unmap(iommu_get_domain_for_dev(dev), *handle, size);
	__iommu_dma_free_pages(pages, PAGE_ALIGN(size) >> PAGE_SHIFT);
	*handle = IOMMU_MAPPING_ERROR;
}

/**
 * iommu_dma_alloc - Allocate and map a buffer contiguous in IOVA space
 * @dev: Device to allocate memory for. Must be a real device
 *	 attached to an iommu_dma_domain
 * @size: Size of buffer in bytes
 * @gfp: Allocation flags
 * @attrs: DMA attributes for this allocation
 * @prot: IOMMU mapping flags
 * @handle: Out argument for allocated DMA handle
 * @flush_page: Arch callback which must ensure PAGE_SIZE bytes from the
 *		given VA/PA are visible to the given non-coherent device.
 *
 * If @size is less than PAGE_SIZE, then a full CPU page will be allocated,
 * but an IOMMU which supports smaller pages might not map the whole thing.
 *
 * Return: Array of struct page pointers describing the buffer,
 *	   or NULL on failure.
 */
struct page **iommu_dma_alloc(struct device *dev, size_t size, gfp_t gfp,
		unsigned long attrs, int prot, dma_addr_t *handle,
		void (*flush_page)(struct device *, const void *, phys_addr_t))
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	struct page **pages;
	struct sg_table sgt;
	dma_addr_t iova;
	unsigned int count, min_size, alloc_sizes = domain->pgsize_bitmap;

	*handle = IOMMU_MAPPING_ERROR;
=======
/*
 * If size is less than PAGE_SIZE, then a full CPU page will be allocated,
 * but an IOMMU which supports smaller pages might not map the whole thing.
 */
static struct page **__iommu_dma_alloc_noncontiguous(struct device *dev,
		size_t size, struct sg_table *sgt, gfp_t gfp, pgprot_t prot,
		unsigned long attrs)
{
	struct iommu_domain *domain = iommu_get_dma_domain(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	bool coherent = dev_is_dma_coherent(dev);
	int ioprot = dma_info_to_prot(DMA_BIDIRECTIONAL, coherent, attrs);
	unsigned int count, min_size, alloc_sizes = domain->pgsize_bitmap;
	struct page **pages;
	dma_addr_t iova;
	ssize_t ret;

	if (static_branch_unlikely(&iommu_deferred_attach_enabled) &&
	    iommu_deferred_attach(dev, domain))
		return NULL;
>>>>>>> upstream/android-13

	min_size = alloc_sizes & -alloc_sizes;
	if (min_size < PAGE_SIZE) {
		min_size = PAGE_SIZE;
		alloc_sizes |= PAGE_SIZE;
	} else {
		size = ALIGN(size, min_size);
	}
	if (attrs & DMA_ATTR_ALLOC_SINGLE_PAGES)
		alloc_sizes = min_size;

	count = PAGE_ALIGN(size) >> PAGE_SHIFT;
<<<<<<< HEAD
	pages = __iommu_dma_alloc_pages(count, alloc_sizes >> PAGE_SHIFT, gfp);
=======
	pages = __iommu_dma_alloc_pages(dev, count, alloc_sizes >> PAGE_SHIFT,
					gfp);
>>>>>>> upstream/android-13
	if (!pages)
		return NULL;

	size = iova_align(iovad, size);
	iova = iommu_dma_alloc_iova(domain, size, dev->coherent_dma_mask, dev);
	if (!iova)
		goto out_free_pages;

<<<<<<< HEAD
	if (sg_alloc_table_from_pages(&sgt, pages, count, 0, size, GFP_KERNEL))
		goto out_free_iova;

	if (!(prot & IOMMU_CACHE)) {
		struct sg_mapping_iter miter;
		/*
		 * The CPU-centric flushing implied by SG_MITER_TO_SG isn't
		 * sufficient here, so skip it by using the "wrong" direction.
		 */
		sg_miter_start(&miter, sgt.sgl, sgt.orig_nents, SG_MITER_FROM_SG);
		while (sg_miter_next(&miter))
			flush_page(dev, miter.addr, page_to_phys(miter.page));
		sg_miter_stop(&miter);
	}

	if (iommu_map_sg(domain, iova, sgt.sgl, sgt.orig_nents, prot)
	    < size) {
		pr_notice("%s, %d, err iommu map sg\n", __func__, __LINE__);
		goto out_free_sg;
	}

	*handle = iova;
	sg_free_table(&sgt);
	return pages;

out_free_sg:
	sg_free_table(&sgt);
out_free_iova:
	iommu_dma_free_iova(cookie, iova, size);
=======
	if (sg_alloc_table_from_pages(sgt, pages, count, 0, size, GFP_KERNEL))
		goto out_free_iova;

	if (!(ioprot & IOMMU_CACHE)) {
		struct scatterlist *sg;
		int i;

		for_each_sg(sgt->sgl, sg, sgt->orig_nents, i)
			arch_dma_prep_coherent(sg_page(sg), sg->length);
	}

	ret = iommu_map_sg_atomic(domain, iova, sgt->sgl, sgt->orig_nents, ioprot);
	if (ret < 0 || ret < size)
		goto out_free_sg;

	sgt->sgl->dma_address = iova;
	sgt->sgl->dma_length = size;
	return pages;

out_free_sg:
	sg_free_table(sgt);
out_free_iova:
	iommu_dma_free_iova(cookie, iova, size, NULL);
>>>>>>> upstream/android-13
out_free_pages:
	__iommu_dma_free_pages(pages, count);
	return NULL;
}

<<<<<<< HEAD
/**
 * iommu_dma_mmap - Map a buffer into provided user VMA
 * @pages: Array representing buffer from iommu_dma_alloc()
 * @size: Size of buffer in bytes
 * @vma: VMA describing requested userspace mapping
 *
 * Maps the pages of the buffer in @pages into @vma. The caller is responsible
 * for verifying the correct size and protection of @vma beforehand.
 */

int iommu_dma_mmap(struct page **pages, size_t size, struct vm_area_struct *vma)
{
	unsigned long uaddr = vma->vm_start;
	unsigned int i, count = PAGE_ALIGN(size) >> PAGE_SHIFT;
	int ret = -ENXIO;

	for (i = vma->vm_pgoff; i < count && uaddr < vma->vm_end; i++) {
		ret = vm_insert_page(vma, uaddr, pages[i]);
		if (ret)
			break;
		uaddr += PAGE_SIZE;
	}
	return ret;
}

static dma_addr_t __iommu_dma_map(struct device *dev, phys_addr_t phys,
		size_t size, int prot)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	size_t iova_off = 0;
	dma_addr_t iova;

	if (cookie->type == IOMMU_DMA_IOVA_COOKIE) {
		iova_off = iova_offset(&cookie->iovad, phys);
		size = iova_align(&cookie->iovad, size + iova_off);
	}

	iova = iommu_dma_alloc_iova(domain, size, dma_get_mask(dev), dev);
	if (!iova)
		return IOMMU_MAPPING_ERROR;

	if (iommu_map(domain, iova, phys - iova_off, size, prot)) {
		iommu_dma_free_iova(cookie, iova, size);
		return IOMMU_MAPPING_ERROR;
	}
	return iova + iova_off;
}

dma_addr_t iommu_dma_map_page(struct device *dev, struct page *page,
		unsigned long offset, size_t size, int prot)
{
	return __iommu_dma_map(dev, page_to_phys(page) + offset, size, prot);
}

void iommu_dma_unmap_page(struct device *dev, dma_addr_t handle, size_t size,
		enum dma_data_direction dir, unsigned long attrs)
{
	__iommu_dma_unmap(iommu_get_domain_for_dev(dev), handle, size);
=======
static void *iommu_dma_alloc_remap(struct device *dev, size_t size,
		dma_addr_t *dma_handle, gfp_t gfp, pgprot_t prot,
		unsigned long attrs)
{
	struct page **pages;
	struct sg_table sgt;
	void *vaddr;

	pages = __iommu_dma_alloc_noncontiguous(dev, size, &sgt, gfp, prot,
						attrs);
	if (!pages)
		return NULL;
	*dma_handle = sgt.sgl->dma_address;
	sg_free_table(&sgt);
	vaddr = dma_common_pages_remap(pages, size, prot,
			__builtin_return_address(0));
	if (!vaddr)
		goto out_unmap;
	return vaddr;

out_unmap:
	__iommu_dma_unmap(dev, *dma_handle, size);
	__iommu_dma_free_pages(pages, PAGE_ALIGN(size) >> PAGE_SHIFT);
	return NULL;
}

#ifdef CONFIG_DMA_REMAP
static struct sg_table *iommu_dma_alloc_noncontiguous(struct device *dev,
		size_t size, enum dma_data_direction dir, gfp_t gfp,
		unsigned long attrs)
{
	struct dma_sgt_handle *sh;

	sh = kmalloc(sizeof(*sh), gfp);
	if (!sh)
		return NULL;

	sh->pages = __iommu_dma_alloc_noncontiguous(dev, size, &sh->sgt, gfp,
						    PAGE_KERNEL, attrs);
	if (!sh->pages) {
		kfree(sh);
		return NULL;
	}
	return &sh->sgt;
}

static void iommu_dma_free_noncontiguous(struct device *dev, size_t size,
		struct sg_table *sgt, enum dma_data_direction dir)
{
	struct dma_sgt_handle *sh = sgt_handle(sgt);

	__iommu_dma_unmap(dev, sgt->sgl->dma_address, size);
	__iommu_dma_free_pages(sh->pages, PAGE_ALIGN(size) >> PAGE_SHIFT);
	sg_free_table(&sh->sgt);
	kfree(sh);
}
#endif /* CONFIG_DMA_REMAP */

static void iommu_dma_sync_single_for_cpu(struct device *dev,
		dma_addr_t dma_handle, size_t size, enum dma_data_direction dir)
{
	phys_addr_t phys;

	if (dev_is_dma_coherent(dev) && !dev_use_swiotlb(dev))
		return;

	phys = iommu_iova_to_phys(iommu_get_dma_domain(dev), dma_handle);
	if (!dev_is_dma_coherent(dev))
		arch_sync_dma_for_cpu(phys, size, dir);

	if (is_swiotlb_buffer(dev, phys))
		swiotlb_sync_single_for_cpu(dev, phys, size, dir);
}

static void iommu_dma_sync_single_for_device(struct device *dev,
		dma_addr_t dma_handle, size_t size, enum dma_data_direction dir)
{
	phys_addr_t phys;

	if (dev_is_dma_coherent(dev) && !dev_use_swiotlb(dev))
		return;

	phys = iommu_iova_to_phys(iommu_get_dma_domain(dev), dma_handle);
	if (is_swiotlb_buffer(dev, phys))
		swiotlb_sync_single_for_device(dev, phys, size, dir);

	if (!dev_is_dma_coherent(dev))
		arch_sync_dma_for_device(phys, size, dir);
}

static void iommu_dma_sync_sg_for_cpu(struct device *dev,
		struct scatterlist *sgl, int nelems,
		enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	if (dev_use_swiotlb(dev))
		for_each_sg(sgl, sg, nelems, i)
			iommu_dma_sync_single_for_cpu(dev, sg_dma_address(sg),
						      sg->length, dir);
	else if (!dev_is_dma_coherent(dev))
		for_each_sg(sgl, sg, nelems, i)
			arch_sync_dma_for_cpu(sg_phys(sg), sg->length, dir);
}

static void iommu_dma_sync_sg_for_device(struct device *dev,
		struct scatterlist *sgl, int nelems,
		enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	if (dev_use_swiotlb(dev))
		for_each_sg(sgl, sg, nelems, i)
			iommu_dma_sync_single_for_device(dev,
							 sg_dma_address(sg),
							 sg->length, dir);
	else if (!dev_is_dma_coherent(dev))
		for_each_sg(sgl, sg, nelems, i)
			arch_sync_dma_for_device(sg_phys(sg), sg->length, dir);
}

static dma_addr_t iommu_dma_map_page(struct device *dev, struct page *page,
		unsigned long offset, size_t size, enum dma_data_direction dir,
		unsigned long attrs)
{
	phys_addr_t phys = page_to_phys(page) + offset;
	bool coherent = dev_is_dma_coherent(dev);
	int prot = dma_info_to_prot(dir, coherent, attrs);
	struct iommu_domain *domain = iommu_get_dma_domain(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	dma_addr_t iova, dma_mask = dma_get_mask(dev);

	/*
	 * If both the physical buffer start address and size are
	 * page aligned, we don't need to use a bounce page.
	 */
	if (dev_use_swiotlb(dev) && iova_offset(iovad, phys | size)) {
		void *padding_start;
		size_t padding_size, aligned_size;

		aligned_size = iova_align(iovad, size);
		phys = swiotlb_tbl_map_single(dev, phys, size, aligned_size,
					      iova_mask(iovad), dir, attrs);

		if (phys == DMA_MAPPING_ERROR)
			return DMA_MAPPING_ERROR;

		/* Cleanup the padding area. */
		padding_start = phys_to_virt(phys);
		padding_size = aligned_size;

		if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC) &&
		    (dir == DMA_TO_DEVICE || dir == DMA_BIDIRECTIONAL)) {
			padding_start += size;
			padding_size -= size;
		}

		memset(padding_start, 0, padding_size);
	}

	if (!coherent && !(attrs & DMA_ATTR_SKIP_CPU_SYNC))
		arch_sync_dma_for_device(phys, size, dir);

	iova = __iommu_dma_map(dev, phys, size, prot, dma_mask);
	if (iova == DMA_MAPPING_ERROR && is_swiotlb_buffer(dev, phys))
		swiotlb_tbl_unmap_single(dev, phys, size, dir, attrs);
	return iova;
}

static void iommu_dma_unmap_page(struct device *dev, dma_addr_t dma_handle,
		size_t size, enum dma_data_direction dir, unsigned long attrs)
{
	struct iommu_domain *domain = iommu_get_dma_domain(dev);
	phys_addr_t phys;

	phys = iommu_iova_to_phys(domain, dma_handle);
	if (WARN_ON(!phys))
		return;

	if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC) && !dev_is_dma_coherent(dev))
		arch_sync_dma_for_cpu(phys, size, dir);

	__iommu_dma_unmap(dev, dma_handle, size);

	if (unlikely(is_swiotlb_buffer(dev, phys)))
		swiotlb_tbl_unmap_single(dev, phys, size, dir, attrs);
>>>>>>> upstream/android-13
}

/*
 * Prepare a successfully-mapped scatterlist to give back to the caller.
 *
 * At this point the segments are already laid out by iommu_dma_map_sg() to
 * avoid individually crossing any boundaries, so we merely need to check a
 * segment's start address to avoid concatenating across one.
 */
<<<<<<< HEAD
int iommu_dma_finalise_sg(struct device *dev, struct scatterlist *sg, int nents,
=======
static int __finalise_sg(struct device *dev, struct scatterlist *sg, int nents,
>>>>>>> upstream/android-13
		dma_addr_t dma_addr)
{
	struct scatterlist *s, *cur = sg;
	unsigned long seg_mask = dma_get_seg_boundary(dev);
	unsigned int cur_len = 0, max_len = dma_get_max_seg_size(dev);
	int i, count = 0;

	for_each_sg(sg, s, nents, i) {
		/* Restore this segment's original unaligned fields first */
		unsigned int s_iova_off = sg_dma_address(s);
		unsigned int s_length = sg_dma_len(s);
		unsigned int s_iova_len = s->length;

<<<<<<< HEAD
#ifdef CONFIG_MTK_IOMMU_V2
		if (!sg_page(s)) {
			pr_info("%s, page is null\n", __func__);
			s_iova_off = 0;
		}
#endif
		s->offset += s_iova_off;
		s->length = s_length;
		sg_dma_address(s) = IOMMU_MAPPING_ERROR;
=======
		s->offset += s_iova_off;
		s->length = s_length;
		sg_dma_address(s) = DMA_MAPPING_ERROR;
>>>>>>> upstream/android-13
		sg_dma_len(s) = 0;

		/*
		 * Now fill in the real DMA data. If...
		 * - there is a valid output segment to append to
		 * - and this segment starts on an IOVA page boundary
		 * - but doesn't fall at a segment boundary
		 * - and wouldn't make the resulting output segment too long
		 */
		if (cur_len && !s_iova_off && (dma_addr & seg_mask) &&
<<<<<<< HEAD
		    (cur_len + s_length <= max_len)) {
=======
		    (max_len - cur_len >= s_length)) {
>>>>>>> upstream/android-13
			/* ...then concatenate it with the previous one */
			cur_len += s_length;
		} else {
			/* Otherwise start the next output segment */
			if (i > 0)
				cur = sg_next(cur);
			cur_len = s_length;
			count++;

			sg_dma_address(cur) = dma_addr + s_iova_off;
		}

		sg_dma_len(cur) = cur_len;
		dma_addr += s_iova_len;

		if (s_length + s_iova_off < s_iova_len)
			cur_len = 0;
<<<<<<< HEAD

		if (s_iova_off)
			pr_info("[M4U] %s warning, 0x%x--0x%x, offset:%u, count:%d\n",
				__func__, s_iova_len,
				s_length, s_iova_off, count);
=======
>>>>>>> upstream/android-13
	}
	return count;
}

/*
 * If mapping failed, then just restore the original list,
 * but making sure the DMA fields are invalidated.
 */
<<<<<<< HEAD
void iommu_dma_invalidate_sg(struct scatterlist *sg, int nents)
=======
static void __invalidate_sg(struct scatterlist *sg, int nents)
>>>>>>> upstream/android-13
{
	struct scatterlist *s;
	int i;

	for_each_sg(sg, s, nents, i) {
<<<<<<< HEAD
		if (sg_dma_address(s) != IOMMU_MAPPING_ERROR)
			s->offset += sg_dma_address(s);
		if (sg_dma_len(s))
			s->length = sg_dma_len(s);
		sg_dma_address(s) = IOMMU_MAPPING_ERROR;
=======
		if (sg_dma_address(s) != DMA_MAPPING_ERROR)
			s->offset += sg_dma_address(s);
		if (sg_dma_len(s))
			s->length = sg_dma_len(s);
		sg_dma_address(s) = DMA_MAPPING_ERROR;
>>>>>>> upstream/android-13
		sg_dma_len(s) = 0;
	}
}

<<<<<<< HEAD
=======
static void iommu_dma_unmap_sg_swiotlb(struct device *dev, struct scatterlist *sg,
		int nents, enum dma_data_direction dir, unsigned long attrs)
{
	struct scatterlist *s;
	int i;

	for_each_sg(sg, s, nents, i)
		iommu_dma_unmap_page(dev, sg_dma_address(s),
				sg_dma_len(s), dir, attrs);
}

static int iommu_dma_map_sg_swiotlb(struct device *dev, struct scatterlist *sg,
		int nents, enum dma_data_direction dir, unsigned long attrs)
{
	struct scatterlist *s;
	int i;

	for_each_sg(sg, s, nents, i) {
		sg_dma_address(s) = iommu_dma_map_page(dev, sg_page(s),
				s->offset, s->length, dir, attrs);
		if (sg_dma_address(s) == DMA_MAPPING_ERROR)
			goto out_unmap;
		sg_dma_len(s) = s->length;
	}

	return nents;

out_unmap:
	iommu_dma_unmap_sg_swiotlb(dev, sg, i, dir, attrs | DMA_ATTR_SKIP_CPU_SYNC);
	return -EIO;
}

>>>>>>> upstream/android-13
/*
 * The DMA API client is passing in a scatterlist which could describe
 * any old buffer layout, but the IOMMU API requires everything to be
 * aligned to IOMMU pages. Hence the need for this complicated bit of
 * impedance-matching, to be able to hand off a suitably-aligned list,
 * but still preserve the original offsets and sizes for the caller.
 */
<<<<<<< HEAD
size_t iommu_dma_prepare_map_sg(struct device *dev, struct iova_domain *iovad,
				struct scatterlist *sg, int nents)
{
	struct scatterlist *s, *prev = NULL;
	size_t iova_len = 0;
	unsigned long mask = dma_get_seg_boundary(dev);
	int i;

=======
static int iommu_dma_map_sg(struct device *dev, struct scatterlist *sg,
		int nents, enum dma_data_direction dir, unsigned long attrs)
{
	struct iommu_domain *domain = iommu_get_dma_domain(dev);
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	struct scatterlist *s, *prev = NULL;
	int prot = dma_info_to_prot(dir, dev_is_dma_coherent(dev), attrs);
	dma_addr_t iova;
	size_t iova_len = 0;
	unsigned long mask = dma_get_seg_boundary(dev);
	ssize_t ret;
	int i;

	if (static_branch_unlikely(&iommu_deferred_attach_enabled)) {
		ret = iommu_deferred_attach(dev, domain);
		if (ret)
			goto out;
	}

	if (dev_use_swiotlb(dev))
		return iommu_dma_map_sg_swiotlb(dev, sg, nents, dir, attrs);

	if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC))
		iommu_dma_sync_sg_for_device(dev, sg, nents, dir);

>>>>>>> upstream/android-13
	/*
	 * Work out how much IOVA space we need, and align the segments to
	 * IOVA granules for the IOMMU driver to handle. With some clever
	 * trickery we can modify the list in-place, but reversibly, by
	 * stashing the unaligned parts in the as-yet-unused DMA fields.
	 */
	for_each_sg(sg, s, nents, i) {
		size_t s_iova_off = iova_offset(iovad, s->offset);
		size_t s_length = s->length;
		size_t pad_len = (mask - iova_len + 1) & mask;

<<<<<<< HEAD
		/*
		 * FIXME: Mediatek workaround for the buffer that don't has
		 * "struct page *"
		 */
#ifdef CONFIG_MTK_IOMMU_V2
#ifdef CONFIG_MTK_PSEUDO_M4U
		if (IS_ERR(sg_page(s))) {
			s_length = sg_dma_len(s);
			s->length = s_length;
			iova_len += s_length;
			prev = s;
			pr_info("%s, page is error\n", __func__);
			continue;
		}
#endif
=======
>>>>>>> upstream/android-13
		sg_dma_address(s) = s_iova_off;
		sg_dma_len(s) = s_length;
		s->offset -= s_iova_off;
		s_length = iova_align(iovad, s_length + s_iova_off);
		s->length = s_length;
<<<<<<< HEAD
		if (s->length != sg_dma_len(s))
			pr_info("%s, length is not equal dma_length, 0x%x--0x%x\n",
				__func__, s->length,
				(unsigned int)sg_dma_len(s));
#else
#ifndef CONFIG_MTK_PSEUDO_M4U
		sg_dma_address(s) = s_iova_off;
		sg_dma_len(s) = s_length;
		s->offset -= s_iova_off;
		s_length = iova_align(iovad, s_length + s_iova_off);
		s->length = s_length;
#else
		if (!sg_dma_address(s) && !sg_dma_len(s)) {
			sg_dma_address(s) = s_iova_off;
			sg_dma_len(s) = s_length;
			s->offset -= s_iova_off;
			s_length = iova_align(iovad, s_length + s_iova_off);
			s->length = s_length;
		} else {
			/*
			 * pseudo m4u store the s_length in sg_dma_len, it may
			 * be in different field depend on the
			 * CONFIG_NEED_SG_DMA_LENGTH, get the length from the
			 * macro.
			 */
			s_length = sg_dma_len(s);
			s->length = s_length;
		}
#endif
#endif
=======

>>>>>>> upstream/android-13
		/*
		 * Due to the alignment of our single IOVA allocation, we can
		 * depend on these assumptions about the segment boundary mask:
		 * - If mask size >= IOVA size, then the IOVA range cannot
		 *   possibly fall across a boundary, so we don't care.
		 * - If mask size < IOVA size, then the IOVA range must start
		 *   exactly on a boundary, therefore we can lay things out
		 *   based purely on segment lengths without needing to know
		 *   the actual addresses beforehand.
		 * - The mask must be a power of 2, so pad_len == 0 if
		 *   iova_len == 0, thus we cannot dereference prev the first
		 *   time through here (i.e. before it has a meaningful value).
		 */
		if (pad_len && pad_len < s_length - 1) {
			prev->length += pad_len;
			iova_len += pad_len;
		}

		iova_len += s_length;
		prev = s;
	}

<<<<<<< HEAD
	return iova_len;
}

int iommu_dma_map_sg(struct device *dev, struct scatterlist *sg,
		int nents, int prot)
{
	struct iommu_domain *domain;
	struct iommu_dma_cookie *cookie;
	struct iova_domain *iovad;
	dma_addr_t iova;
	size_t iova_len;

	domain = iommu_get_domain_for_dev(dev);
	if (!domain)
		return 0;
	cookie = domain->iova_cookie;
	iovad = &cookie->iovad;

	iova_len = iommu_dma_prepare_map_sg(dev, iovad, sg, nents);

	iova = iommu_dma_alloc_iova(domain, iova_len, dma_get_mask(dev), dev);
	if (!iova) {
		pr_notice("%s, %d, dev:%s domain:%p failed at alloc iova\n",
			    __func__, __LINE__, dev_name(dev), domain);
=======
	iova = iommu_dma_alloc_iova(domain, iova_len, dma_get_mask(dev), dev);
	if (!iova) {
		ret = -ENOMEM;
>>>>>>> upstream/android-13
		goto out_restore_sg;
	}

	/*
	 * We'll leave any physical concatenation to the IOMMU driver's
	 * implementation - it knows better than we do.
	 */
<<<<<<< HEAD
	if (iommu_map_sg(domain, iova, sg, nents, prot) < iova_len) {
		pr_notice("%s, %d, dev:%s domain:%p failed at map sg, iova:0x%pa, len:%lx\n",
			    __func__, __LINE__, dev_name(dev),
			    domain, &iova, iova_len);
		goto out_free_iova;
	}

	return iommu_dma_finalise_sg(dev, sg, nents, iova);

out_free_iova:
	iommu_dma_free_iova(cookie, iova, iova_len);
out_restore_sg:
	iommu_dma_invalidate_sg(sg, nents);
	return 0;
}

void iommu_dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nents,
		enum dma_data_direction dir, unsigned long attrs)
=======
	ret = iommu_map_sg_atomic(domain, iova, sg, nents, prot);
	if (ret < 0 || ret < iova_len)
		goto out_free_iova;

	return __finalise_sg(dev, sg, nents, iova);

out_free_iova:
	iommu_dma_free_iova(cookie, iova, iova_len, NULL);
out_restore_sg:
	__invalidate_sg(sg, nents);
out:
	if (ret != -ENOMEM)
		return -EINVAL;
	return ret;
}

static void iommu_dma_unmap_sg(struct device *dev, struct scatterlist *sg,
		int nents, enum dma_data_direction dir, unsigned long attrs)
>>>>>>> upstream/android-13
{
	dma_addr_t start, end;
	struct scatterlist *tmp;
	int i;
<<<<<<< HEAD
=======

	if (dev_use_swiotlb(dev)) {
		iommu_dma_unmap_sg_swiotlb(dev, sg, nents, dir, attrs);
		return;
	}

	if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC))
		iommu_dma_sync_sg_for_cpu(dev, sg, nents, dir);

>>>>>>> upstream/android-13
	/*
	 * The scatterlist segments are mapped into a single
	 * contiguous IOVA allocation, so this is incredibly easy.
	 */
	start = sg_dma_address(sg);
	for_each_sg(sg_next(sg), tmp, nents - 1, i) {
		if (sg_dma_len(tmp) == 0)
			break;
		sg = tmp;
	}
	end = sg_dma_address(sg) + sg_dma_len(sg);
<<<<<<< HEAD
	__iommu_dma_unmap(iommu_get_domain_for_dev(dev), start, end - start);
}

#ifdef CONFIG_ARM64
struct page **
iommu_dma_alloc_fix_iova(struct device *dev, size_t size, gfp_t gfp,
		unsigned long attrs, int prot, dma_addr_t handle,
		void (*flush_page)(struct device *, const void *, phys_addr_t))
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie;
	struct iova_domain *iovad;
	struct page **pages;
	struct sg_table sgt;
	dma_addr_t iova = handle;
	unsigned int count, min_size, alloc_sizes;

	if (!domain)
		return NULL;
	cookie = domain->iova_cookie;
	iovad = &cookie->iovad;
	alloc_sizes = domain->pgsize_bitmap;
	min_size = alloc_sizes & -alloc_sizes;
	if (min_size < PAGE_SIZE) {
		min_size = PAGE_SIZE;
		alloc_sizes |= PAGE_SIZE;
	} else {
		size = ALIGN(size, min_size);
	}
	if (attrs & DMA_ATTR_ALLOC_SINGLE_PAGES)
		alloc_sizes = min_size;

	count = PAGE_ALIGN(size) >> PAGE_SHIFT;
	pages = __iommu_dma_alloc_pages(count, alloc_sizes >> PAGE_SHIFT, gfp);
	if (!pages)
		return NULL;

	size = iova_align(iovad, size);
	if (sg_alloc_table_from_pages(&sgt, pages, count, 0, size, GFP_KERNEL))
		goto out_free_iova;

	if (!(prot & IOMMU_CACHE)) {
		struct sg_mapping_iter miter;
		/*
		 * The CPU-centric flushing implied by SG_MITER_TO_SG isn't
		 * sufficient here, so skip it by using the "wrong" direction.
		 */
		sg_miter_start(&miter, sgt.sgl, sgt.orig_nents,
			       SG_MITER_FROM_SG);
		while (sg_miter_next(&miter))
			flush_page(dev, miter.addr, page_to_phys(miter.page));
		sg_miter_stop(&miter);
	}

	if (iommu_map_sg(domain, iova, sgt.sgl, sgt.orig_nents, prot)
			< size)
		goto out_free_sg;

	sg_free_table(&sgt);
	return pages;

out_free_sg:
	sg_free_table(&sgt);
out_free_iova:
	iommu_dma_free_iova(cookie, iova, size);
	__iommu_dma_free_pages(pages, count);
	return NULL;
}

/*
 * User have to provide the dma address in the reserved iova area for mapping
 */
int dma_map_sg_within_reserved_iova(struct device *dev, struct scatterlist *sg,
				    int nents, int prot, dma_addr_t dma_addr)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie;
	struct iova_domain *iovad;
	struct scatterlist *s, *prev = NULL;
	size_t iova_len = 0;
	size_t map_len = 0;
	int i;

	if (!domain)
		return -EINVAL;
	cookie = domain->iova_cookie;
	iovad = &cookie->iovad;

	for_each_sg(sg, s, nents, i) {
		size_t s_iova_off = iova_offset(iovad, s->offset);
		size_t s_length = s->length;

		sg_dma_address(s) = s_iova_off;
		sg_dma_len(s) = s_length;
		s->offset -= s_iova_off;
		s_length = iova_align(iovad, s_length + s_iova_off);
		s->length = s_length;

		iova_len += s_length;
		prev = s;
	}

	map_len = iommu_map_sg(domain, dma_addr, sg, nents, prot);

	return iommu_dma_finalise_sg(dev, sg, nents, dma_addr);
}
EXPORT_SYMBOL(dma_map_sg_within_reserved_iova);

void dma_unmap_sg_within_reserved_iova(struct device *dev,
				       struct scatterlist *sg, int nents,
				       int prot, size_t size)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie;
	struct iova_domain *iovad;
	unsigned long shift;
	unsigned long pfn;

	if (!domain)
		return;
	cookie = domain->iova_cookie;
	iovad = &cookie->iovad;
	shift = iova_shift(iovad);
	pfn = sg_dma_address(sg) >> shift;
	/*
	 * The scatterlist segments are mapped into a single
	 * contiguous IOVA allocation, so this is incredibly easy.
	 */
	size -= iommu_unmap(domain, pfn << shift, size);
	WARN_ON(size > 0);
}
EXPORT_SYMBOL(dma_unmap_sg_within_reserved_iova);
#endif

dma_addr_t iommu_dma_map_resource(struct device *dev, phys_addr_t phys,
		size_t size, enum dma_data_direction dir, unsigned long attrs)
{
	return __iommu_dma_map(dev, phys, size,
			dma_info_to_prot(dir, false, attrs) | IOMMU_MMIO);
}

void iommu_dma_unmap_resource(struct device *dev, dma_addr_t handle,
		size_t size, enum dma_data_direction dir, unsigned long attrs)
{
	__iommu_dma_unmap(iommu_get_domain_for_dev(dev), handle, size);
}

int iommu_dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return dma_addr == IOMMU_MAPPING_ERROR;
}

=======
	__iommu_dma_unmap(dev, start, end - start);
}

static dma_addr_t iommu_dma_map_resource(struct device *dev, phys_addr_t phys,
		size_t size, enum dma_data_direction dir, unsigned long attrs)
{
	return __iommu_dma_map(dev, phys, size,
			dma_info_to_prot(dir, false, attrs) | IOMMU_MMIO,
			dma_get_mask(dev));
}

static void iommu_dma_unmap_resource(struct device *dev, dma_addr_t handle,
		size_t size, enum dma_data_direction dir, unsigned long attrs)
{
	__iommu_dma_unmap(dev, handle, size);
}

static void __iommu_dma_free(struct device *dev, size_t size, void *cpu_addr)
{
	size_t alloc_size = PAGE_ALIGN(size);
	int count = alloc_size >> PAGE_SHIFT;
	struct page *page = NULL, **pages = NULL;

	/* Non-coherent atomic allocation? Easy */
	if (IS_ENABLED(CONFIG_DMA_DIRECT_REMAP) &&
	    dma_free_from_pool(dev, cpu_addr, alloc_size))
		return;

	if (IS_ENABLED(CONFIG_DMA_REMAP) && is_vmalloc_addr(cpu_addr)) {
		/*
		 * If it the address is remapped, then it's either non-coherent
		 * or highmem CMA, or an iommu_dma_alloc_remap() construction.
		 */
		pages = dma_common_find_pages(cpu_addr);
		if (!pages)
			page = vmalloc_to_page(cpu_addr);
		dma_common_free_remap(cpu_addr, alloc_size);
	} else {
		/* Lowmem means a coherent atomic or CMA allocation */
		page = virt_to_page(cpu_addr);
	}

	if (pages)
		__iommu_dma_free_pages(pages, count);
	if (page)
		dma_free_contiguous(dev, page, alloc_size);
}

static void iommu_dma_free(struct device *dev, size_t size, void *cpu_addr,
		dma_addr_t handle, unsigned long attrs)
{
	__iommu_dma_unmap(dev, handle, size);
	__iommu_dma_free(dev, size, cpu_addr);
}

static void *iommu_dma_alloc_pages(struct device *dev, size_t size,
		struct page **pagep, gfp_t gfp, unsigned long attrs)
{
	bool coherent = dev_is_dma_coherent(dev);
	size_t alloc_size = PAGE_ALIGN(size);
	int node = dev_to_node(dev);
	struct page *page = NULL;
	void *cpu_addr;

	page = dma_alloc_contiguous(dev, alloc_size, gfp);
	if (!page)
		page = alloc_pages_node(node, gfp, get_order(alloc_size));
	if (!page)
		return NULL;

	if (IS_ENABLED(CONFIG_DMA_REMAP) && (!coherent || PageHighMem(page))) {
		pgprot_t prot = dma_pgprot(dev, PAGE_KERNEL, attrs);

		cpu_addr = dma_common_contiguous_remap(page, alloc_size,
				prot, __builtin_return_address(0));
		if (!cpu_addr)
			goto out_free_pages;

		if (!coherent)
			arch_dma_prep_coherent(page, size);
	} else {
		cpu_addr = page_address(page);
	}

	*pagep = page;
	memset(cpu_addr, 0, alloc_size);
	return cpu_addr;
out_free_pages:
	dma_free_contiguous(dev, page, alloc_size);
	return NULL;
}

static void *iommu_dma_alloc(struct device *dev, size_t size,
		dma_addr_t *handle, gfp_t gfp, unsigned long attrs)
{
	bool coherent = dev_is_dma_coherent(dev);
	int ioprot = dma_info_to_prot(DMA_BIDIRECTIONAL, coherent, attrs);
	struct page *page = NULL;
	void *cpu_addr;

	gfp |= __GFP_ZERO;

	if (IS_ENABLED(CONFIG_DMA_REMAP) && gfpflags_allow_blocking(gfp) &&
	    !(attrs & DMA_ATTR_FORCE_CONTIGUOUS)) {
		return iommu_dma_alloc_remap(dev, size, handle, gfp,
				dma_pgprot(dev, PAGE_KERNEL, attrs), attrs);
	}

	if (IS_ENABLED(CONFIG_DMA_DIRECT_REMAP) &&
	    !gfpflags_allow_blocking(gfp) && !coherent)
		page = dma_alloc_from_pool(dev, PAGE_ALIGN(size), &cpu_addr,
					       gfp, NULL);
	else
		cpu_addr = iommu_dma_alloc_pages(dev, size, &page, gfp, attrs);
	if (!cpu_addr)
		return NULL;

	*handle = __iommu_dma_map(dev, page_to_phys(page), size, ioprot,
			dev->coherent_dma_mask);
	if (*handle == DMA_MAPPING_ERROR) {
		__iommu_dma_free(dev, size, cpu_addr);
		return NULL;
	}

	return cpu_addr;
}

static int iommu_dma_mmap(struct device *dev, struct vm_area_struct *vma,
		void *cpu_addr, dma_addr_t dma_addr, size_t size,
		unsigned long attrs)
{
	unsigned long nr_pages = PAGE_ALIGN(size) >> PAGE_SHIFT;
	unsigned long pfn, off = vma->vm_pgoff;
	int ret;

	vma->vm_page_prot = dma_pgprot(dev, vma->vm_page_prot, attrs);

	if (dma_mmap_from_dev_coherent(dev, vma, cpu_addr, size, &ret))
		return ret;

	if (off >= nr_pages || vma_pages(vma) > nr_pages - off)
		return -ENXIO;

	if (IS_ENABLED(CONFIG_DMA_REMAP) && is_vmalloc_addr(cpu_addr)) {
		struct page **pages = dma_common_find_pages(cpu_addr);

		if (pages)
			return vm_map_pages(vma, pages, nr_pages);
		pfn = vmalloc_to_pfn(cpu_addr);
	} else {
		pfn = page_to_pfn(virt_to_page(cpu_addr));
	}

	return remap_pfn_range(vma, vma->vm_start, pfn + off,
			       vma->vm_end - vma->vm_start,
			       vma->vm_page_prot);
}

static int iommu_dma_get_sgtable(struct device *dev, struct sg_table *sgt,
		void *cpu_addr, dma_addr_t dma_addr, size_t size,
		unsigned long attrs)
{
	struct page *page;
	int ret;

	if (IS_ENABLED(CONFIG_DMA_REMAP) && is_vmalloc_addr(cpu_addr)) {
		struct page **pages = dma_common_find_pages(cpu_addr);

		if (pages) {
			return sg_alloc_table_from_pages(sgt, pages,
					PAGE_ALIGN(size) >> PAGE_SHIFT,
					0, size, GFP_KERNEL);
		}

		page = vmalloc_to_page(cpu_addr);
	} else {
		page = virt_to_page(cpu_addr);
	}

	ret = sg_alloc_table(sgt, 1, GFP_KERNEL);
	if (!ret)
		sg_set_page(sgt->sgl, page, PAGE_ALIGN(size), 0);
	return ret;
}

static unsigned long iommu_dma_get_merge_boundary(struct device *dev)
{
	struct iommu_domain *domain = iommu_get_dma_domain(dev);

	return (1UL << __ffs(domain->pgsize_bitmap)) - 1;
}

static const struct dma_map_ops iommu_dma_ops = {
	.alloc			= iommu_dma_alloc,
	.free			= iommu_dma_free,
	.alloc_pages		= dma_common_alloc_pages,
	.free_pages		= dma_common_free_pages,
#ifdef CONFIG_DMA_REMAP
	.alloc_noncontiguous	= iommu_dma_alloc_noncontiguous,
	.free_noncontiguous	= iommu_dma_free_noncontiguous,
#endif
	.mmap			= iommu_dma_mmap,
	.get_sgtable		= iommu_dma_get_sgtable,
	.map_page		= iommu_dma_map_page,
	.unmap_page		= iommu_dma_unmap_page,
	.map_sg			= iommu_dma_map_sg,
	.unmap_sg		= iommu_dma_unmap_sg,
	.sync_single_for_cpu	= iommu_dma_sync_single_for_cpu,
	.sync_single_for_device	= iommu_dma_sync_single_for_device,
	.sync_sg_for_cpu	= iommu_dma_sync_sg_for_cpu,
	.sync_sg_for_device	= iommu_dma_sync_sg_for_device,
	.map_resource		= iommu_dma_map_resource,
	.unmap_resource		= iommu_dma_unmap_resource,
	.get_merge_boundary	= iommu_dma_get_merge_boundary,
};

/*
 * The IOMMU core code allocates the default DMA domain, which the underlying
 * IOMMU driver needs to support via the dma-iommu layer.
 */
void iommu_setup_dma_ops(struct device *dev, u64 dma_base, u64 dma_limit)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);

	if (!domain)
		goto out_err;

	/*
	 * The IOMMU core code allocates the default DMA domain, which the
	 * underlying IOMMU driver needs to support via the dma-iommu layer.
	 */
	if (iommu_is_dma_domain(domain)) {
		if (iommu_dma_init_domain(domain, dma_base, dma_limit, dev))
			goto out_err;
		dev->dma_ops = &iommu_dma_ops;
	}

	return;
out_err:
	 pr_warn("Failed to set up IOMMU for device %s; retaining platform DMA ops\n",
		 dev_name(dev));
}
EXPORT_SYMBOL_GPL(iommu_setup_dma_ops);

>>>>>>> upstream/android-13
static struct iommu_dma_msi_page *iommu_dma_get_msi_page(struct device *dev,
		phys_addr_t msi_addr, struct iommu_domain *domain)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iommu_dma_msi_page *msi_page;
	dma_addr_t iova;
	int prot = IOMMU_WRITE | IOMMU_NOEXEC | IOMMU_MMIO;
	size_t size = cookie_msi_granule(cookie);

	msi_addr &= ~(phys_addr_t)(size - 1);
	list_for_each_entry(msi_page, &cookie->msi_page_list, list)
		if (msi_page->phys == msi_addr)
			return msi_page;

<<<<<<< HEAD
	msi_page = kzalloc(sizeof(*msi_page), GFP_ATOMIC);
	if (!msi_page)
		return NULL;

	iova = __iommu_dma_map(dev, msi_addr, size, prot);
	if (iommu_dma_mapping_error(dev, iova))
		goto out_free_page;

=======
	msi_page = kzalloc(sizeof(*msi_page), GFP_KERNEL);
	if (!msi_page)
		return NULL;

	iova = iommu_dma_alloc_iova(domain, size, dma_get_mask(dev), dev);
	if (!iova)
		goto out_free_page;

	if (iommu_map(domain, iova, msi_addr, size, prot))
		goto out_free_iova;

>>>>>>> upstream/android-13
	INIT_LIST_HEAD(&msi_page->list);
	msi_page->phys = msi_addr;
	msi_page->iova = iova;
	list_add(&msi_page->list, &cookie->msi_page_list);
	return msi_page;

<<<<<<< HEAD
=======
out_free_iova:
	iommu_dma_free_iova(cookie, iova, size, NULL);
>>>>>>> upstream/android-13
out_free_page:
	kfree(msi_page);
	return NULL;
}

<<<<<<< HEAD
void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg)
{
	struct device *dev = msi_desc_to_dev(irq_get_msi_desc(irq));
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_cookie *cookie;
	struct iommu_dma_msi_page *msi_page;
	phys_addr_t msi_addr = (u64)msg->address_hi << 32 | msg->address_lo;
	unsigned long flags;

	if (!domain || !domain->iova_cookie)
		return;

	cookie = domain->iova_cookie;

	/*
	 * We disable IRQs to rule out a possible inversion against
	 * irq_desc_lock if, say, someone tries to retarget the affinity
	 * of an MSI from within an IPI handler.
	 */
	spin_lock_irqsave(&cookie->msi_lock, flags);
	msi_page = iommu_dma_get_msi_page(dev, msi_addr, domain);
	spin_unlock_irqrestore(&cookie->msi_lock, flags);

	if (WARN_ON(!msi_page)) {
		/*
		 * We're called from a void callback, so the best we can do is
		 * 'fail' by filling the message with obviously bogus values.
		 * Since we got this far due to an IOMMU being present, it's
		 * not like the existing address would have worked anyway...
		 */
		msg->address_hi = ~0U;
		msg->address_lo = ~0U;
		msg->data = ~0U;
	} else {
		msg->address_hi = upper_32_bits(msi_page->iova);
		msg->address_lo &= cookie_msi_granule(cookie) - 1;
		msg->address_lo += lower_32_bits(msi_page->iova);
	}
}

#ifdef CONFIG_MTK_IOMMU_V2
static unsigned int domain_used_size;
static unsigned int domain_resev_size;
void iommu_dma_dump_iova(void *domain, unsigned long start,
	unsigned long end, unsigned long size,
	unsigned long target)
{
	phys_addr_t p_start, p_end;
	struct iommu_dma_cookie *cookie =
			((struct iommu_domain *)domain)->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;

	p_start = iommu_iova_to_phys((struct iommu_domain *)domain, start);
	p_end = iommu_iova_to_phys((struct iommu_domain *)domain, end);

	// free_iova_fast() did not remove the RB node of small size
	if (!p_start && !p_end &&
	    (order_base_2(size >> iova_shift(iovad)) <
	    IOVA_RANGE_CACHE_MAX_SIZE))
		return;

	domain_used_size += size / 1024;
	if (!p_start)
		domain_resev_size += size / 1024;

	if (target &&
	    ((start >= target + SZ_16M) ||
	    (end <= target - SZ_16M)))
		return;

	// the reserved region will not be managed in current domain
	if (!p_start)
		pr_notice(">>>    iova:0x%lx~0x%lx, pa:0x%pa/0x%pa, size:0x%lx (reserved region, check the other domain?)\n",
		  start, end, &p_start, &p_end, size);
	else if (!p_end)
		pr_notice(">>>    iova:0x%lx~0x%lx, pa:0x%pa/0x%pa, size:0x%lx (size not aligned, check IOVA list)\n",
		  start, end, &p_start, &p_end, size);
	else
		pr_notice(">>>    iova:0x%lx~0x%lx, pa:0x%pa/0x%pa, size:0x%lx\n",
			  start, end, &p_start, &p_end, size);
}

void iommu_dma_dump_iovad(struct iommu_domain *domain,
	unsigned long target)
{
	struct iommu_dma_cookie *cookie = domain->iova_cookie;
	struct iova_domain *iovad = &cookie->iovad;
	unsigned long base = iovad->start_pfn << iova_shift(iovad);
	unsigned long max = domain->geometry.aperture_end;
	unsigned int domain_size = (max - base + 1) / 1024;

	domain_used_size = 0;
	domain_resev_size = 0;

	iovad_scan_reserved_iova((void *)domain, iovad,
		iommu_dma_dump_iova, target);
	pr_notice("(0x%lx, 0x%lx) total:%uKB, used:(%uKB/%dper), resev:(%uKB/%dper), target:0x%lx\n",
		  base, max, domain_size,
		  domain_used_size,
		  domain_used_size * 100 / domain_size,
		  domain_resev_size,
		  domain_resev_size * 100 / domain_size,
		  target);
}
EXPORT_SYMBOL(iommu_dma_dump_iovad);

int iommu_dma_get_iovad_info(struct device *dev,
	unsigned long *base, unsigned long *max)
{
	struct iommu_domain *domain;
	struct iommu_dma_cookie *cookie;
	struct iova_domain *iovad;

	domain = iommu_get_domain_for_dev(dev);

	if (!domain)
		return -EINVAL;
	cookie = domain->iova_cookie;
	iovad = &cookie->iovad;

	*base = iovad->start_pfn << iova_shift(iovad);
	*max = domain->geometry.aperture_end;

	return 0;
}
EXPORT_SYMBOL(iommu_dma_get_iovad_info);
#endif
=======
int iommu_dma_prepare_msi(struct msi_desc *desc, phys_addr_t msi_addr)
{
	struct device *dev = msi_desc_to_dev(desc);
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_dma_msi_page *msi_page;
	static DEFINE_MUTEX(msi_prepare_lock); /* see below */

	if (!domain || !domain->iova_cookie) {
		desc->iommu_cookie = NULL;
		return 0;
	}

	/*
	 * In fact the whole prepare operation should already be serialised by
	 * irq_domain_mutex further up the callchain, but that's pretty subtle
	 * on its own, so consider this locking as failsafe documentation...
	 */
	mutex_lock(&msi_prepare_lock);
	msi_page = iommu_dma_get_msi_page(dev, msi_addr, domain);
	mutex_unlock(&msi_prepare_lock);

	msi_desc_set_iommu_cookie(desc, msi_page);

	if (!msi_page)
		return -ENOMEM;
	return 0;
}

void iommu_dma_compose_msi_msg(struct msi_desc *desc,
			       struct msi_msg *msg)
{
	struct device *dev = msi_desc_to_dev(desc);
	const struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	const struct iommu_dma_msi_page *msi_page;

	msi_page = msi_desc_get_iommu_cookie(desc);

	if (!domain || !domain->iova_cookie || WARN_ON(!msi_page))
		return;

	msg->address_hi = upper_32_bits(msi_page->iova);
	msg->address_lo &= cookie_msi_granule(domain->iova_cookie) - 1;
	msg->address_lo += lower_32_bits(msi_page->iova);
}

static int iommu_dma_init(void)
{
	if (is_kdump_kernel())
		static_branch_enable(&iommu_deferred_attach_enabled);

	return iova_cache_get();
}
arch_initcall(iommu_dma_init);
>>>>>>> upstream/android-13
