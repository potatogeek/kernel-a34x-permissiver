<<<<<<< HEAD
/*
 * Copyright © 2008 Keith Packard <keithp@keithp.com>
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
 * Copyright © 2008 Keith Packard <keithp@keithp.com>
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_IO_MAPPING_H
#define _LINUX_IO_MAPPING_H

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/bug.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/pgtable.h>
>>>>>>> upstream/android-13
#include <asm/page.h>

/*
 * The io_mapping mechanism provides an abstraction for mapping
 * individual pages from an io device to the CPU in an efficient fashion.
 *
<<<<<<< HEAD
 * See Documentation/io-mapping.txt
=======
 * See Documentation/driver-api/io-mapping.rst
>>>>>>> upstream/android-13
 */

struct io_mapping {
	resource_size_t base;
	unsigned long size;
	pgprot_t prot;
	void __iomem *iomem;
};

#ifdef CONFIG_HAVE_ATOMIC_IOMAP

<<<<<<< HEAD
=======
#include <linux/pfn.h>
>>>>>>> upstream/android-13
#include <asm/iomap.h>
/*
 * For small address space machines, mapping large objects
 * into the kernel virtual space isn't practical. Where
 * available, use fixmap support to dynamically map pages
 * of the object at run time.
 */

static inline struct io_mapping *
io_mapping_init_wc(struct io_mapping *iomap,
		   resource_size_t base,
		   unsigned long size)
{
	pgprot_t prot;

	if (iomap_create_wc(base, size, &prot))
		return NULL;

	iomap->base = base;
	iomap->size = size;
	iomap->prot = prot;
	return iomap;
}

static inline void
io_mapping_fini(struct io_mapping *mapping)
{
	iomap_free(mapping->base, mapping->size);
}

/* Atomic map/unmap */
static inline void __iomem *
io_mapping_map_atomic_wc(struct io_mapping *mapping,
			 unsigned long offset)
{
	resource_size_t phys_addr;
<<<<<<< HEAD
	unsigned long pfn;

	BUG_ON(offset >= mapping->size);
	phys_addr = mapping->base + offset;
	pfn = (unsigned long) (phys_addr >> PAGE_SHIFT);
	return iomap_atomic_prot_pfn(pfn, mapping->prot);
=======

	BUG_ON(offset >= mapping->size);
	phys_addr = mapping->base + offset;
	preempt_disable();
	pagefault_disable();
	return __iomap_local_pfn_prot(PHYS_PFN(phys_addr), mapping->prot);
>>>>>>> upstream/android-13
}

static inline void
io_mapping_unmap_atomic(void __iomem *vaddr)
{
<<<<<<< HEAD
	iounmap_atomic(vaddr);
=======
	kunmap_local_indexed((void __force *)vaddr);
	pagefault_enable();
	preempt_enable();
}

static inline void __iomem *
io_mapping_map_local_wc(struct io_mapping *mapping, unsigned long offset)
{
	resource_size_t phys_addr;

	BUG_ON(offset >= mapping->size);
	phys_addr = mapping->base + offset;
	return __iomap_local_pfn_prot(PHYS_PFN(phys_addr), mapping->prot);
}

static inline void io_mapping_unmap_local(void __iomem *vaddr)
{
	kunmap_local_indexed((void __force *)vaddr);
>>>>>>> upstream/android-13
}

static inline void __iomem *
io_mapping_map_wc(struct io_mapping *mapping,
		  unsigned long offset,
		  unsigned long size)
{
	resource_size_t phys_addr;

	BUG_ON(offset >= mapping->size);
	phys_addr = mapping->base + offset;

	return ioremap_wc(phys_addr, size);
}

static inline void
io_mapping_unmap(void __iomem *vaddr)
{
	iounmap(vaddr);
}

<<<<<<< HEAD
#else

#include <linux/uaccess.h>
#include <asm/pgtable.h>
=======
#else  /* HAVE_ATOMIC_IOMAP */

#include <linux/uaccess.h>
>>>>>>> upstream/android-13

/* Create the io_mapping object*/
static inline struct io_mapping *
io_mapping_init_wc(struct io_mapping *iomap,
		   resource_size_t base,
		   unsigned long size)
{
	iomap->iomem = ioremap_wc(base, size);
	if (!iomap->iomem)
		return NULL;

	iomap->base = base;
	iomap->size = size;
#if defined(pgprot_noncached_wc) /* archs can't agree on a name ... */
	iomap->prot = pgprot_noncached_wc(PAGE_KERNEL);
#elif defined(pgprot_writecombine)
	iomap->prot = pgprot_writecombine(PAGE_KERNEL);
#else
	iomap->prot = pgprot_noncached(PAGE_KERNEL);
#endif

	return iomap;
}

static inline void
io_mapping_fini(struct io_mapping *mapping)
{
	iounmap(mapping->iomem);
}

/* Non-atomic map/unmap */
static inline void __iomem *
io_mapping_map_wc(struct io_mapping *mapping,
		  unsigned long offset,
		  unsigned long size)
{
	return mapping->iomem + offset;
}

static inline void
io_mapping_unmap(void __iomem *vaddr)
{
}

/* Atomic map/unmap */
static inline void __iomem *
io_mapping_map_atomic_wc(struct io_mapping *mapping,
			 unsigned long offset)
{
	preempt_disable();
	pagefault_disable();
	return io_mapping_map_wc(mapping, offset, PAGE_SIZE);
}

static inline void
io_mapping_unmap_atomic(void __iomem *vaddr)
{
	io_mapping_unmap(vaddr);
	pagefault_enable();
	preempt_enable();
}

<<<<<<< HEAD
#endif /* HAVE_ATOMIC_IOMAP */
=======
static inline void __iomem *
io_mapping_map_local_wc(struct io_mapping *mapping, unsigned long offset)
{
	return io_mapping_map_wc(mapping, offset, PAGE_SIZE);
}

static inline void io_mapping_unmap_local(void __iomem *vaddr)
{
	io_mapping_unmap(vaddr);
}

#endif /* !HAVE_ATOMIC_IOMAP */
>>>>>>> upstream/android-13

static inline struct io_mapping *
io_mapping_create_wc(resource_size_t base,
		     unsigned long size)
{
	struct io_mapping *iomap;

	iomap = kmalloc(sizeof(*iomap), GFP_KERNEL);
	if (!iomap)
		return NULL;

	if (!io_mapping_init_wc(iomap, base, size)) {
		kfree(iomap);
		return NULL;
	}

	return iomap;
}

static inline void
io_mapping_free(struct io_mapping *iomap)
{
	io_mapping_fini(iomap);
	kfree(iomap);
}

#endif /* _LINUX_IO_MAPPING_H */
<<<<<<< HEAD
=======

int io_mapping_map_user(struct io_mapping *iomap, struct vm_area_struct *vma,
		unsigned long addr, unsigned long pfn, unsigned long size);
>>>>>>> upstream/android-13
