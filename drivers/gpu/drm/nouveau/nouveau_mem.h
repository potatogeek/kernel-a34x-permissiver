#ifndef __NOUVEAU_MEM_H__
#define __NOUVEAU_MEM_H__
#include <drm/ttm/ttm_bo_api.h>
<<<<<<< HEAD
struct ttm_dma_tt;
=======
struct ttm_tt;
>>>>>>> upstream/android-13

#include <nvif/mem.h>
#include <nvif/vmm.h>

<<<<<<< HEAD
static inline struct nouveau_mem *
nouveau_mem(struct ttm_mem_reg *reg)
{
	return reg->mm_node;
}

struct nouveau_mem {
=======
struct nouveau_mem {
	struct ttm_resource base;
>>>>>>> upstream/android-13
	struct nouveau_cli *cli;
	u8 kind;
	u8 comp;
	struct nvif_mem mem;
	struct nvif_vma vma[2];
};

<<<<<<< HEAD
int nouveau_mem_new(struct nouveau_cli *, u8 kind, u8 comp,
		    struct ttm_mem_reg *);
void nouveau_mem_del(struct ttm_mem_reg *);
int nouveau_mem_vram(struct ttm_mem_reg *, bool contig, u8 page);
int nouveau_mem_host(struct ttm_mem_reg *, struct ttm_dma_tt *);
=======
static inline struct nouveau_mem *
nouveau_mem(struct ttm_resource *reg)
{
	return container_of(reg, struct nouveau_mem, base);
}

int nouveau_mem_new(struct nouveau_cli *, u8 kind, u8 comp,
		    struct ttm_resource **);
void nouveau_mem_del(struct ttm_resource *);
int nouveau_mem_vram(struct ttm_resource *, bool contig, u8 page);
int nouveau_mem_host(struct ttm_resource *, struct ttm_tt *);
>>>>>>> upstream/android-13
void nouveau_mem_fini(struct nouveau_mem *);
int nouveau_mem_map(struct nouveau_mem *, struct nvif_vmm *, struct nvif_vma *);
#endif
