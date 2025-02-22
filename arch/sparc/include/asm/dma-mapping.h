/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ___ASM_SPARC_DMA_MAPPING_H
#define ___ASM_SPARC_DMA_MAPPING_H

<<<<<<< HEAD
#include <linux/scatterlist.h>
#include <linux/mm.h>
#include <linux/dma-debug.h>

extern const struct dma_map_ops *dma_ops;

extern struct bus_type pci_bus_type;

static inline const struct dma_map_ops *get_arch_dma_ops(struct bus_type *bus)
{
#ifdef CONFIG_SPARC_LEON
	if (sparc_cpu_model == sparc_leon)
		return &dma_noncoherent_ops;
#endif
#if defined(CONFIG_SPARC32) && defined(CONFIG_PCI)
	if (bus == &pci_bus_type)
		return &dma_noncoherent_ops;
#endif
	return dma_ops;
=======
extern const struct dma_map_ops *dma_ops;

static inline const struct dma_map_ops *get_arch_dma_ops(struct bus_type *bus)
{
	/* sparc32 uses per-device dma_ops */
	return IS_ENABLED(CONFIG_SPARC64) ? dma_ops : NULL;
>>>>>>> upstream/android-13
}

#endif
