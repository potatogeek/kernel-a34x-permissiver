

#ifndef _LINUX_IO_MAPPING_H
#define _LINUX_IO_MAPPING_H

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/bug.h>
#include <linux/io.h>
#include <asm/page.h>



struct io_mapping {
	resource_size_t base;
	unsigned long size;
	pgprot_t prot;
	void __iomem *iomem;
};

#ifdef CONFIG_HAVE_ATOMIC_IOMAP

#include <asm/iomap.h>


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


static inline void __iomem *
io_mapping_map_atomic_wc(struct io_mapping *mapping,
			 unsigned long offset)
{
	resource_size_t phys_addr;
	unsigned long pfn;

	BUG_ON(offset >= mapping->size);
	phys_addr = mapping->base + offset;
	pfn = (unsigned long) (phys_addr >> PAGE_SHIFT);
	return iomap_atomic_prot_pfn(pfn, mapping->prot);
}

static inline void
io_mapping_unmap_atomic(void __iomem *vaddr)
{
	iounmap_atomic(vaddr);
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

#else

#include <linux/uaccess.h>
#include <asm/pgtable.h>


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
#if defined(pgprot_noncached_wc) 
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

#endif 

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

#endif 
