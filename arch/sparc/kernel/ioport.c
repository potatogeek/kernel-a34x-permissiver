// SPDX-License-Identifier: GPL-2.0
/*
 * ioport.c:  Simple io mapping allocator.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1995 Miguel de Icaza (miguel@nuclecu.unam.mx)
 *
 * 1996: sparc_free_io, 1999: ioremap()/iounmap() by Pete Zaitcev.
 *
 * 2000/01/29
 * <rth> zait: as long as pci_alloc_consistent produces something addressable, 
 *	things are ok.
 * <zaitcev> rth: no, it is relevant, because get_free_pages returns you a
 *	pointer into the big page mapping
 * <rth> zait: so what?
 * <rth> zait: remap_it_my_way(virt_to_phys(get_free_page()))
 * <zaitcev> Hmm
 * <zaitcev> Suppose I did this remap_it_my_way(virt_to_phys(get_free_page())).
 *	So far so good.
 * <zaitcev> Now, driver calls pci_free_consistent(with result of
 *	remap_it_my_way()).
 * <zaitcev> How do you find the address to pass to free_pages()?
 * <rth> zait: walk the page tables?  It's only two or three level after all.
 * <rth> zait: you have to walk them anyway to remove the mapping.
 * <zaitcev> Hmm
 * <zaitcev> Sounds reasonable
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/pci.h>		/* struct pci_dev */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD
#include <linux/dma-noncoherent.h>
=======
#include <linux/dma-map-ops.h>
>>>>>>> upstream/android-13
#include <linux/of_device.h>

#include <asm/io.h>
#include <asm/vaddrs.h>
#include <asm/oplib.h>
#include <asm/prom.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/dma.h>
#include <asm/iommu.h>
#include <asm/io-unit.h>
#include <asm/leon.h>

<<<<<<< HEAD
const struct sparc32_dma_ops *sparc32_dma_ops;

=======
>>>>>>> upstream/android-13
/* This function must make sure that caches and memory are coherent after DMA
 * On LEON systems without cache snooping it flushes the entire D-CACHE.
 */
static inline void dma_make_coherent(unsigned long pa, unsigned long len)
{
	if (sparc_cpu_model == sparc_leon) {
		if (!sparc_leon3_snooping_enabled())
			leon_flush_dcache_all();
	}
}

static void __iomem *_sparc_ioremap(struct resource *res, u32 bus, u32 pa, int sz);
static void __iomem *_sparc_alloc_io(unsigned int busno, unsigned long phys,
    unsigned long size, char *name);
static void _sparc_free_io(struct resource *res);

static void register_proc_sparc_ioport(void);

/* This points to the next to use virtual memory for DVMA mappings */
static struct resource _sparc_dvma = {
	.name = "sparc_dvma", .start = DVMA_VADDR, .end = DVMA_END - 1
};
/* This points to the start of I/O mappings, cluable from outside. */
/*ext*/ struct resource sparc_iomap = {
	.name = "sparc_iomap", .start = IOBASE_VADDR, .end = IOBASE_END - 1
};

/*
 * Our mini-allocator...
 * Boy this is gross! We need it because we must map I/O for
 * timers and interrupt controller before the kmalloc is available.
 */

#define XNMLN  15
#define XNRES  10	/* SS-10 uses 8 */

struct xresource {
	struct resource xres;	/* Must be first */
	int xflag;		/* 1 == used */
	char xname[XNMLN+1];
};

static struct xresource xresv[XNRES];

static struct xresource *xres_alloc(void) {
	struct xresource *xrp;
	int n;

	xrp = xresv;
	for (n = 0; n < XNRES; n++) {
		if (xrp->xflag == 0) {
			xrp->xflag = 1;
			return xrp;
		}
		xrp++;
	}
	return NULL;
}

static void xres_free(struct xresource *xrp) {
	xrp->xflag = 0;
}

/*
 * These are typically used in PCI drivers
 * which are trying to be cross-platform.
 *
 * Bus type is always zero on IIep.
 */
void __iomem *ioremap(phys_addr_t offset, size_t size)
{
	char name[14];

	sprintf(name, "phys_%08x", (u32)offset);
	return _sparc_alloc_io(0, (unsigned long)offset, size, name);
}
EXPORT_SYMBOL(ioremap);

/*
 * Complementary to ioremap().
 */
void iounmap(volatile void __iomem *virtual)
{
	unsigned long vaddr = (unsigned long) virtual & PAGE_MASK;
	struct resource *res;

	/*
	 * XXX Too slow. Can have 8192 DVMA pages on sun4m in the worst case.
	 * This probably warrants some sort of hashing.
	*/
	if ((res = lookup_resource(&sparc_iomap, vaddr)) == NULL) {
		printk("free_io/iounmap: cannot free %lx\n", vaddr);
		return;
	}
	_sparc_free_io(res);

	if ((char *)res >= (char*)xresv && (char *)res < (char *)&xresv[XNRES]) {
		xres_free((struct xresource *)res);
	} else {
		kfree(res);
	}
}
EXPORT_SYMBOL(iounmap);

void __iomem *of_ioremap(struct resource *res, unsigned long offset,
			 unsigned long size, char *name)
{
	return _sparc_alloc_io(res->flags & 0xF,
			       res->start + offset,
			       size, name);
}
EXPORT_SYMBOL(of_ioremap);

void of_iounmap(struct resource *res, void __iomem *base, unsigned long size)
{
	iounmap(base);
}
EXPORT_SYMBOL(of_iounmap);

/*
 * Meat of mapping
 */
static void __iomem *_sparc_alloc_io(unsigned int busno, unsigned long phys,
    unsigned long size, char *name)
{
	static int printed_full;
	struct xresource *xres;
	struct resource *res;
	char *tack;
	int tlen;
	void __iomem *va;	/* P3 diag */

	if (name == NULL) name = "???";

	if ((xres = xres_alloc()) != NULL) {
		tack = xres->xname;
		res = &xres->xres;
	} else {
		if (!printed_full) {
			printk("ioremap: done with statics, switching to malloc\n");
			printed_full = 1;
		}
		tlen = strlen(name);
		tack = kmalloc(sizeof (struct resource) + tlen + 1, GFP_KERNEL);
		if (tack == NULL) return NULL;
		memset(tack, 0, sizeof(struct resource));
		res = (struct resource *) tack;
		tack += sizeof (struct resource);
	}

	strlcpy(tack, name, XNMLN+1);
	res->name = tack;

	va = _sparc_ioremap(res, busno, phys, size);
	/* printk("ioremap(0x%x:%08lx[0x%lx])=%p\n", busno, phys, size, va); */ /* P3 diag */
	return va;
}

/*
 */
static void __iomem *
_sparc_ioremap(struct resource *res, u32 bus, u32 pa, int sz)
{
	unsigned long offset = ((unsigned long) pa) & (~PAGE_MASK);

	if (allocate_resource(&sparc_iomap, res,
	    (offset + sz + PAGE_SIZE-1) & PAGE_MASK,
	    sparc_iomap.start, sparc_iomap.end, PAGE_SIZE, NULL, NULL) != 0) {
		/* Usually we cannot see printks in this case. */
		prom_printf("alloc_io_res(%s): cannot occupy\n",
		    (res->name != NULL)? res->name: "???");
		prom_halt();
	}

	pa &= PAGE_MASK;
	srmmu_mapiorange(bus, pa, res->start, resource_size(res));

	return (void __iomem *)(unsigned long)(res->start + offset);
}

/*
 * Complementary to _sparc_ioremap().
 */
static void _sparc_free_io(struct resource *res)
{
	unsigned long plen;

	plen = resource_size(res);
	BUG_ON((plen & (PAGE_SIZE-1)) != 0);
	srmmu_unmapiorange(res->start, plen);
	release_resource(res);
}

<<<<<<< HEAD
=======
unsigned long sparc_dma_alloc_resource(struct device *dev, size_t len)
{
	struct resource *res;

	res = kzalloc(sizeof(*res), GFP_KERNEL);
	if (!res)
		return 0;
	res->name = dev->of_node->full_name;

	if (allocate_resource(&_sparc_dvma, res, len, _sparc_dvma.start,
			      _sparc_dvma.end, PAGE_SIZE, NULL, NULL) != 0) {
		printk("%s: cannot occupy 0x%zx", __func__, len);
		kfree(res);
		return 0;
	}

	return res->start;
}

bool sparc_dma_free_resource(void *cpu_addr, size_t size)
{
	unsigned long addr = (unsigned long)cpu_addr;
	struct resource *res;

	res = lookup_resource(&_sparc_dvma, addr);
	if (!res) {
		printk("%s: cannot free %p\n", __func__, cpu_addr);
		return false;
	}

	if ((addr & (PAGE_SIZE - 1)) != 0) {
		printk("%s: unaligned va %p\n", __func__, cpu_addr);
		return false;
	}

	size = PAGE_ALIGN(size);
	if (resource_size(res) != size) {
		printk("%s: region 0x%lx asked 0x%zx\n",
			__func__, (long)resource_size(res), size);
		return false;
	}

	release_resource(res);
	kfree(res);
	return true;
}

>>>>>>> upstream/android-13
#ifdef CONFIG_SBUS

void sbus_set_sbus64(struct device *dev, int x)
{
	printk("sbus_set_sbus64: unsupported\n");
}
EXPORT_SYMBOL(sbus_set_sbus64);

<<<<<<< HEAD
/*
 * Allocate a chunk of memory suitable for DMA.
 * Typically devices use them for control blocks.
 * CPU may access them without any explicit flushing.
 */
static void *sbus_alloc_coherent(struct device *dev, size_t len,
				 dma_addr_t *dma_addrp, gfp_t gfp,
				 unsigned long attrs)
{
	struct platform_device *op = to_platform_device(dev);
	unsigned long len_total = PAGE_ALIGN(len);
	unsigned long va;
	struct resource *res;
	int order;

	/* XXX why are some lengths signed, others unsigned? */
	if (len <= 0) {
		return NULL;
	}
	/* XXX So what is maxphys for us and how do drivers know it? */
	if (len > 256*1024) {			/* __get_free_pages() limit */
		return NULL;
	}

	order = get_order(len_total);
	va = __get_free_pages(gfp, order);
	if (va == 0)
		goto err_nopages;

	if ((res = kzalloc(sizeof(struct resource), GFP_KERNEL)) == NULL)
		goto err_nomem;

	if (allocate_resource(&_sparc_dvma, res, len_total,
	    _sparc_dvma.start, _sparc_dvma.end, PAGE_SIZE, NULL, NULL) != 0) {
		printk("sbus_alloc_consistent: cannot occupy 0x%lx", len_total);
		goto err_nova;
	}

	// XXX The sbus_map_dma_area does this for us below, see comments.
	// srmmu_mapiorange(0, virt_to_phys(va), res->start, len_total);
	/*
	 * XXX That's where sdev would be used. Currently we load
	 * all iommu tables with the same translations.
	 */
	if (sbus_map_dma_area(dev, dma_addrp, va, res->start, len_total) != 0)
		goto err_noiommu;

	res->name = op->dev.of_node->name;

	return (void *)(unsigned long)res->start;

err_noiommu:
	release_resource(res);
err_nova:
	kfree(res);
err_nomem:
	free_pages(va, order);
err_nopages:
	return NULL;
}

static void sbus_free_coherent(struct device *dev, size_t n, void *p,
			       dma_addr_t ba, unsigned long attrs)
{
	struct resource *res;
	struct page *pgv;

	if ((res = lookup_resource(&_sparc_dvma,
	    (unsigned long)p)) == NULL) {
		printk("sbus_free_consistent: cannot free %p\n", p);
		return;
	}

	if (((unsigned long)p & (PAGE_SIZE-1)) != 0) {
		printk("sbus_free_consistent: unaligned va %p\n", p);
		return;
	}

	n = PAGE_ALIGN(n);
	if (resource_size(res) != n) {
		printk("sbus_free_consistent: region 0x%lx asked 0x%zx\n",
		    (long)resource_size(res), n);
		return;
	}

	release_resource(res);
	kfree(res);

	pgv = virt_to_page(p);
	sbus_unmap_dma_area(dev, ba, n);

	__free_pages(pgv, get_order(n));
}

/*
 * Map a chunk of memory so that devices can see it.
 * CPU view of this memory may be inconsistent with
 * a device view and explicit flushing is necessary.
 */
static dma_addr_t sbus_map_page(struct device *dev, struct page *page,
				unsigned long offset, size_t len,
				enum dma_data_direction dir,
				unsigned long attrs)
{
	void *va = page_address(page) + offset;

	/* XXX why are some lengths signed, others unsigned? */
	if (len <= 0) {
		return 0;
	}
	/* XXX So what is maxphys for us and how do drivers know it? */
	if (len > 256*1024) {			/* __get_free_pages() limit */
		return 0;
	}
	return mmu_get_scsi_one(dev, va, len);
}

static void sbus_unmap_page(struct device *dev, dma_addr_t ba, size_t n,
			    enum dma_data_direction dir, unsigned long attrs)
{
	mmu_release_scsi_one(dev, ba, n);
}

static int sbus_map_sg(struct device *dev, struct scatterlist *sg, int n,
		       enum dma_data_direction dir, unsigned long attrs)
{
	mmu_get_scsi_sgl(dev, sg, n);
	return n;
}

static void sbus_unmap_sg(struct device *dev, struct scatterlist *sg, int n,
			  enum dma_data_direction dir, unsigned long attrs)
{
	mmu_release_scsi_sgl(dev, sg, n);
}

static void sbus_sync_sg_for_cpu(struct device *dev, struct scatterlist *sg,
				 int n,	enum dma_data_direction dir)
{
	BUG();
}

static void sbus_sync_sg_for_device(struct device *dev, struct scatterlist *sg,
				    int n, enum dma_data_direction dir)
{
	BUG();
}

static int sbus_dma_supported(struct device *dev, u64 mask)
{
	return 0;
}

static const struct dma_map_ops sbus_dma_ops = {
	.alloc			= sbus_alloc_coherent,
	.free			= sbus_free_coherent,
	.map_page		= sbus_map_page,
	.unmap_page		= sbus_unmap_page,
	.map_sg			= sbus_map_sg,
	.unmap_sg		= sbus_unmap_sg,
	.sync_sg_for_cpu	= sbus_sync_sg_for_cpu,
	.sync_sg_for_device	= sbus_sync_sg_for_device,
	.dma_supported		= sbus_dma_supported,
};

=======
>>>>>>> upstream/android-13
static int __init sparc_register_ioport(void)
{
	register_proc_sparc_ioport();

	return 0;
}

arch_initcall(sparc_register_ioport);

#endif /* CONFIG_SBUS */


/* Allocate and map kernel buffer using consistent mode DMA for a device.
 * hwdev should be valid struct pci_dev pointer for PCI devices.
 */
void *arch_dma_alloc(struct device *dev, size_t size, dma_addr_t *dma_handle,
		gfp_t gfp, unsigned long attrs)
{
<<<<<<< HEAD
	unsigned long len_total = PAGE_ALIGN(size);
	void *va;
	struct resource *res;
	int order;

	if (size == 0) {
		return NULL;
	}
	if (size > 256*1024) {			/* __get_free_pages() limit */
		return NULL;
	}

	order = get_order(len_total);
	va = (void *) __get_free_pages(gfp, order);
	if (va == NULL) {
		printk("%s: no %ld pages\n", __func__, len_total>>PAGE_SHIFT);
		goto err_nopages;
	}

	if ((res = kzalloc(sizeof(struct resource), GFP_KERNEL)) == NULL) {
		printk("%s: no core\n", __func__);
		goto err_nomem;
	}

	if (allocate_resource(&_sparc_dvma, res, len_total,
	    _sparc_dvma.start, _sparc_dvma.end, PAGE_SIZE, NULL, NULL) != 0) {
		printk("%s: cannot occupy 0x%lx", __func__, len_total);
		goto err_nova;
	}
	srmmu_mapiorange(0, virt_to_phys(va), res->start, len_total);

	*dma_handle = virt_to_phys(va);
	return (void *) res->start;

err_nova:
	kfree(res);
err_nomem:
	free_pages((unsigned long)va, order);
err_nopages:
=======
	unsigned long addr;
	void *va;

	if (!size || size > 256 * 1024)	/* __get_free_pages() limit */
		return NULL;

	size = PAGE_ALIGN(size);
	va = (void *) __get_free_pages(gfp | __GFP_ZERO, get_order(size));
	if (!va) {
		printk("%s: no %zd pages\n", __func__, size >> PAGE_SHIFT);
		return NULL;
	}

	addr = sparc_dma_alloc_resource(dev, size);
	if (!addr)
		goto err_nomem;

	srmmu_mapiorange(0, virt_to_phys(va), addr, size);

	*dma_handle = virt_to_phys(va);
	return (void *)addr;

err_nomem:
	free_pages((unsigned long)va, get_order(size));
>>>>>>> upstream/android-13
	return NULL;
}

/* Free and unmap a consistent DMA buffer.
 * cpu_addr is what was returned arch_dma_alloc, size must be the same as what
 * was passed into arch_dma_alloc, and likewise dma_addr must be the same as
 * what *dma_ndler was set to.
 *
 * References to the memory and mappings associated with cpu_addr/dma_addr
 * past this call are illegal.
 */
void arch_dma_free(struct device *dev, size_t size, void *cpu_addr,
		dma_addr_t dma_addr, unsigned long attrs)
{
<<<<<<< HEAD
	struct resource *res;

	if ((res = lookup_resource(&_sparc_dvma,
	    (unsigned long)cpu_addr)) == NULL) {
		printk("%s: cannot free %p\n", __func__, cpu_addr);
		return;
	}

	if (((unsigned long)cpu_addr & (PAGE_SIZE-1)) != 0) {
		printk("%s: unaligned va %p\n", __func__, cpu_addr);
		return;
	}

	size = PAGE_ALIGN(size);
	if (resource_size(res) != size) {
		printk("%s: region 0x%lx asked 0x%zx\n", __func__,
		    (long)resource_size(res), size);
		return;
	}

	dma_make_coherent(dma_addr, size);
	srmmu_unmapiorange((unsigned long)cpu_addr, size);

	release_resource(res);
	kfree(res);
=======
	size = PAGE_ALIGN(size);

	if (!sparc_dma_free_resource(cpu_addr, size))
		return;

	dma_make_coherent(dma_addr, size);
	srmmu_unmapiorange((unsigned long)cpu_addr, size);
>>>>>>> upstream/android-13
	free_pages((unsigned long)phys_to_virt(dma_addr), get_order(size));
}

/* IIep is write-through, not flushing on cpu to device transfer. */

<<<<<<< HEAD
void arch_sync_dma_for_cpu(struct device *dev, phys_addr_t paddr,
		size_t size, enum dma_data_direction dir)
=======
void arch_sync_dma_for_cpu(phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
>>>>>>> upstream/android-13
{
	if (dir != PCI_DMA_TODEVICE)
		dma_make_coherent(paddr, PAGE_ALIGN(size));
}

<<<<<<< HEAD
const struct dma_map_ops *dma_ops = &sbus_dma_ops;
EXPORT_SYMBOL(dma_ops);

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PROC_FS

static int sparc_io_proc_show(struct seq_file *m, void *v)
{
	struct resource *root = m->private, *r;
	const char *nm;

	for (r = root->child; r != NULL; r = r->sibling) {
		if ((nm = r->name) == NULL) nm = "???";
		seq_printf(m, "%016llx-%016llx: %s\n",
				(unsigned long long)r->start,
				(unsigned long long)r->end, nm);
	}

	return 0;
}
#endif /* CONFIG_PROC_FS */

static void register_proc_sparc_ioport(void)
{
#ifdef CONFIG_PROC_FS
	proc_create_single_data("io_map", 0, NULL, sparc_io_proc_show,
			&sparc_iomap);
	proc_create_single_data("dvma_map", 0, NULL, sparc_io_proc_show,
			&_sparc_dvma);
#endif
}
