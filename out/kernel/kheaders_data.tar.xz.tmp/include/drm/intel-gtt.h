/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DRM_INTEL_GTT_H
#define	_DRM_INTEL_GTT_H

void intel_gtt_get(u64 *gtt_total,
		   phys_addr_t *mappable_base,
		   resource_size_t *mappable_end);

int intel_gmch_probe(struct pci_dev *bridge_pdev, struct pci_dev *gpu_pdev,
		     struct agp_bridge_data *bridge);
void intel_gmch_remove(void);

bool intel_enable_gtt(void);

void intel_gtt_chipset_flush(void);
void intel_gtt_insert_page(dma_addr_t addr,
			   unsigned int pg,
			   unsigned int flags);
void intel_gtt_insert_sg_entries(struct sg_table *st,
				 unsigned int pg_start,
				 unsigned int flags);
void intel_gtt_clear_range(unsigned int first_entry, unsigned int num_entries);


#define AGP_DCACHE_MEMORY	1
#define AGP_PHYS_MEMORY		2


#define AGP_USER_CACHED_MEMORY_GFDT (1 << 3)

#ifdef CONFIG_INTEL_IOMMU
extern int intel_iommu_gfx_mapped;
#endif

#endif
