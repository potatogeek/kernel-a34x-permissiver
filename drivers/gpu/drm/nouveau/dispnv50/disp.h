#ifndef __NV50_KMS_H__
#define __NV50_KMS_H__
<<<<<<< HEAD
#include <nvif/mem.h>

#include "nouveau_display.h"

struct nv50_disp {
	struct nvif_disp *disp;
	struct nv50_core *core;
=======
#include <linux/workqueue.h>
#include <nvif/mem.h>
#include <nvif/push.h>

#include "nouveau_display.h"

struct nv50_msto;
struct nouveau_encoder;

struct nv50_disp {
	struct nvif_disp *disp;
	struct nv50_core *core;
	struct nvif_object caps;
>>>>>>> upstream/android-13

#define NV50_DISP_SYNC(c, o)                                ((c) * 0x040 + (o))
#define NV50_DISP_CORE_NTFY                       NV50_DISP_SYNC(0      , 0x00)
#define NV50_DISP_WNDW_SEM0(c)                    NV50_DISP_SYNC(1 + (c), 0x00)
#define NV50_DISP_WNDW_SEM1(c)                    NV50_DISP_SYNC(1 + (c), 0x10)
#define NV50_DISP_WNDW_NTFY(c)                    NV50_DISP_SYNC(1 + (c), 0x20)
#define NV50_DISP_BASE_SEM0(c)                    NV50_DISP_WNDW_SEM0(0 + (c))
#define NV50_DISP_BASE_SEM1(c)                    NV50_DISP_WNDW_SEM1(0 + (c))
#define NV50_DISP_BASE_NTFY(c)                    NV50_DISP_WNDW_NTFY(0 + (c))
#define NV50_DISP_OVLY_SEM0(c)                    NV50_DISP_WNDW_SEM0(4 + (c))
#define NV50_DISP_OVLY_SEM1(c)                    NV50_DISP_WNDW_SEM1(4 + (c))
#define NV50_DISP_OVLY_NTFY(c)                    NV50_DISP_WNDW_NTFY(4 + (c))
	struct nouveau_bo *sync;

	struct mutex mutex;
};

static inline struct nv50_disp *
nv50_disp(struct drm_device *dev)
{
	return nouveau_display(dev)->priv;
}

struct nv50_disp_interlock {
	enum nv50_disp_interlock_type {
		NV50_DISP_INTERLOCK_CORE = 0,
		NV50_DISP_INTERLOCK_CURS,
		NV50_DISP_INTERLOCK_BASE,
		NV50_DISP_INTERLOCK_OVLY,
		NV50_DISP_INTERLOCK_WNDW,
		NV50_DISP_INTERLOCK_WIMM,
		NV50_DISP_INTERLOCK__SIZE
	} type;
	u32 data;
	u32 wimm;
};

void corec37d_ntfy_init(struct nouveau_bo *, u32);

<<<<<<< HEAD
=======
void head907d_olut_load(struct drm_color_lut *, int size, void __iomem *);

>>>>>>> upstream/android-13
struct nv50_chan {
	struct nvif_object user;
	struct nvif_device *device;
};

struct nv50_dmac {
	struct nv50_chan base;

<<<<<<< HEAD
	struct nvif_mem push;
=======
	struct nvif_push _push;
	struct nvif_push *push;
>>>>>>> upstream/android-13
	u32 *ptr;

	struct nvif_object sync;
	struct nvif_object vram;

	/* Protects against concurrent pushbuf access to this channel, lock is
	 * grabbed by evo_wait (if the pushbuf reservation is successful) and
	 * dropped again by evo_kick. */
	struct mutex lock;
<<<<<<< HEAD
=======

	u32 cur;
	u32 put;
	u32 max;
};

struct nv50_outp_atom {
	struct list_head head;

	struct drm_encoder *encoder;
	bool flush_disable;

	union nv50_outp_atom_mask {
		struct {
			bool ctrl:1;
		};
		u8 mask;
	} set, clr;
>>>>>>> upstream/android-13
};

int nv50_dmac_create(struct nvif_device *device, struct nvif_object *disp,
		     const s32 *oclass, u8 head, void *data, u32 size,
		     s64 syncbuf, struct nv50_dmac *dmac);
void nv50_dmac_destroy(struct nv50_dmac *);

<<<<<<< HEAD
u32 *evo_wait(struct nv50_dmac *, int nr);
void evo_kick(u32 *, struct nv50_dmac *);

#define evo_mthd(p, m, s) do {						\
	const u32 _m = (m), _s = (s);					\
	if (drm_debug & DRM_UT_KMS)					\
		pr_err("%04x %d %s\n", _m, _s, __func__);		\
	*((p)++) = ((_s << 18) | _m);					\
} while(0)

#define evo_data(p, d) do {						\
	const u32 _d = (d);						\
	if (drm_debug & DRM_UT_KMS)					\
		pr_err("\t%08x\n", _d);					\
	*((p)++) = _d;							\
} while(0)
=======
/*
 * For normal encoders this just returns the encoder. For active MST encoders,
 * this returns the real outp that's driving displays on the topology.
 * Inactive MST encoders return NULL, since they would have no real outp to
 * return anyway.
 */
struct nouveau_encoder *nv50_real_outp(struct drm_encoder *encoder);

u32 *evo_wait(struct nv50_dmac *, int nr);
void evo_kick(u32 *, struct nv50_dmac *);

extern const u64 disp50xx_modifiers[];
extern const u64 disp90xx_modifiers[];
extern const u64 wndwc57e_modifiers[];
>>>>>>> upstream/android-13
#endif
