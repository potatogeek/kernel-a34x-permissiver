#ifndef __NV50_KMS_LUT_H__
#define __NV50_KMS_LUT_H__
#include <nvif/mem.h>
struct drm_property_blob;
<<<<<<< HEAD
=======
struct drm_color_lut;
>>>>>>> upstream/android-13
struct nv50_disp;

struct nv50_lut {
	struct nvif_mem mem[2];
};

int nv50_lut_init(struct nv50_disp *, struct nvif_mmu *, struct nv50_lut *);
void nv50_lut_fini(struct nv50_lut *);
<<<<<<< HEAD
u32 nv50_lut_load(struct nv50_lut *, bool legacy, int buffer,
		  struct drm_property_blob *);
=======
u32 nv50_lut_load(struct nv50_lut *, int buffer, struct drm_property_blob *,
		  void (*)(struct drm_color_lut *, int size, void __iomem *));
>>>>>>> upstream/android-13
#endif
