<<<<<<< HEAD
/*
 * Copyright 2006 PathScale, Inc.  All Rights Reserved.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2006 PathScale, Inc.  All Rights Reserved.
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_IO_H
#define _LINUX_IO_H

#include <linux/types.h>
#include <linux/init.h>
#include <linux/bug.h>
#include <linux/err.h>
#include <asm/io.h>
#include <asm/page.h>

struct device;
struct resource;

__visible void __iowrite32_copy(void __iomem *to, const void *from, size_t count);
void __ioread32_copy(void *to, const void __iomem *from, size_t count);
void __iowrite64_copy(void __iomem *to, const void *from, size_t count);

#ifdef CONFIG_MMU
<<<<<<< HEAD
=======
void ioremap_phys_range_hook(phys_addr_t phys_addr, size_t size, pgprot_t prot);
void iounmap_phys_range_hook(phys_addr_t phys_addr, size_t size);
>>>>>>> upstream/android-13
int ioremap_page_range(unsigned long addr, unsigned long end,
		       phys_addr_t phys_addr, pgprot_t prot);
#else
static inline int ioremap_page_range(unsigned long addr, unsigned long end,
				     phys_addr_t phys_addr, pgprot_t prot)
{
	return 0;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_HAVE_ARCH_HUGE_VMAP
void __init ioremap_huge_init(void);
int arch_ioremap_pud_supported(void);
int arch_ioremap_pmd_supported(void);
#else
static inline void ioremap_huge_init(void) { }
#endif

=======
>>>>>>> upstream/android-13
/*
 * Managed iomap interface
 */
#ifdef CONFIG_HAS_IOPORT_MAP
void __iomem * devm_ioport_map(struct device *dev, unsigned long port,
			       unsigned int nr);
void devm_ioport_unmap(struct device *dev, void __iomem *addr);
#else
static inline void __iomem *devm_ioport_map(struct device *dev,
					     unsigned long port,
					     unsigned int nr)
{
	return NULL;
}

static inline void devm_ioport_unmap(struct device *dev, void __iomem *addr)
{
}
#endif

#define IOMEM_ERR_PTR(err) (__force void __iomem *)ERR_PTR(err)

void __iomem *devm_ioremap(struct device *dev, resource_size_t offset,
			   resource_size_t size);
void __iomem *devm_ioremap_uc(struct device *dev, resource_size_t offset,
				   resource_size_t size);
<<<<<<< HEAD
void __iomem *devm_ioremap_nocache(struct device *dev, resource_size_t offset,
				   resource_size_t size);
void __iomem *devm_ioremap_wc(struct device *dev, resource_size_t offset,
				   resource_size_t size);
=======
void __iomem *devm_ioremap_wc(struct device *dev, resource_size_t offset,
				   resource_size_t size);
void __iomem *devm_ioremap_np(struct device *dev, resource_size_t offset,
				   resource_size_t size);
>>>>>>> upstream/android-13
void devm_iounmap(struct device *dev, void __iomem *addr);
int check_signature(const volatile void __iomem *io_addr,
			const unsigned char *signature, int length);
void devm_ioremap_release(struct device *dev, void *res);

void *devm_memremap(struct device *dev, resource_size_t offset,
		size_t size, unsigned long flags);
void devm_memunmap(struct device *dev, void *addr);

<<<<<<< HEAD
void *__devm_memremap_pages(struct device *dev, struct resource *res);

#ifdef CONFIG_PCI
/*
 * The PCI specifications (Rev 3.0, 3.2.5 "Transaction Ordering and
 * Posting") mandate non-posted configuration transactions. There is
 * no ioremap API in the kernel that can guarantee non-posted write
 * semantics across arches so provide a default implementation for
 * mapping PCI config space that defaults to ioremap_nocache(); arches
 * should override it if they have memory mapping implementations that
 * guarantee non-posted writes semantics to make the memory mapping
 * compliant with the PCI specification.
=======
#ifdef CONFIG_PCI
/*
 * The PCI specifications (Rev 3.0, 3.2.5 "Transaction Ordering and
 * Posting") mandate non-posted configuration transactions. This default
 * implementation attempts to use the ioremap_np() API to provide this
 * on arches that support it, and falls back to ioremap() on those that
 * don't. Overriding this function is deprecated; arches that properly
 * support non-posted accesses should implement ioremap_np() instead, which
 * this default implementation can then use to return mappings compliant with
 * the PCI specification.
>>>>>>> upstream/android-13
 */
#ifndef pci_remap_cfgspace
#define pci_remap_cfgspace pci_remap_cfgspace
static inline void __iomem *pci_remap_cfgspace(phys_addr_t offset,
					       size_t size)
{
<<<<<<< HEAD
	return ioremap_nocache(offset, size);
=======
	return ioremap_np(offset, size) ?: ioremap(offset, size);
>>>>>>> upstream/android-13
}
#endif
#endif

/*
 * Some systems do not have legacy ISA devices.
 * /dev/port is not a valid interface on these systems.
 * So for those archs, <asm/io.h> should define the following symbol.
 */
#ifndef arch_has_dev_port
#define arch_has_dev_port()     (1)
#endif

/*
 * Some systems (x86 without PAT) have a somewhat reliable way to mark a
 * physical address range such that uncached mappings will actually
 * end up write-combining.  This facility should be used in conjunction
 * with pgprot_writecombine, ioremap-wc, or set_memory_wc, since it has
 * no effect if the per-page mechanisms are functional.
 * (On x86 without PAT, these functions manipulate MTRRs.)
 *
 * arch_phys_del_wc(0) or arch_phys_del_wc(any error code) is guaranteed
 * to have no effect.
 */
#ifndef arch_phys_wc_add
static inline int __must_check arch_phys_wc_add(unsigned long base,
						unsigned long size)
{
	return 0;  /* It worked (i.e. did nothing). */
}

static inline void arch_phys_wc_del(int handle)
{
}

#define arch_phys_wc_add arch_phys_wc_add
#ifndef arch_phys_wc_index
static inline int arch_phys_wc_index(int handle)
{
	return -1;
}
#define arch_phys_wc_index arch_phys_wc_index
#endif
#endif

enum {
	/* See memremap() kernel-doc for usage description... */
	MEMREMAP_WB = 1 << 0,
	MEMREMAP_WT = 1 << 1,
	MEMREMAP_WC = 1 << 2,
	MEMREMAP_ENC = 1 << 3,
	MEMREMAP_DEC = 1 << 4,
};

void *memremap(resource_size_t offset, size_t size, unsigned long flags);
void memunmap(void *addr);

/*
 * On x86 PAT systems we have memory tracking that keeps track of
 * the allowed mappings on memory ranges. This tracking works for
 * all the in-kernel mapping APIs (ioremap*), but where the user
 * wishes to map a range from a physical device into user memory
 * the tracking won't be updated. This API is to be used by
 * drivers which remap physical device pages into userspace,
 * and wants to make sure they are mapped WC and not UC.
 */
#ifndef arch_io_reserve_memtype_wc
static inline int arch_io_reserve_memtype_wc(resource_size_t base,
					     resource_size_t size)
{
	return 0;
}

static inline void arch_io_free_memtype_wc(resource_size_t base,
					   resource_size_t size)
{
}
#endif

#endif /* _LINUX_IO_H */
