<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_ENUM_H__
#define __NVKM_ENUM_H__
#include <core/os.h>

struct nvkm_enum {
	u32 value;
	const char *name;
	const void *data;
	u32 data2;
<<<<<<< HEAD
=======
	int inst;
>>>>>>> upstream/android-13
};

const struct nvkm_enum *nvkm_enum_find(const struct nvkm_enum *, u32 value);

struct nvkm_bitfield {
	u32 mask;
	const char *name;
};

void nvkm_snprintbf(char *, int, const struct nvkm_bitfield *, u32 value);
#endif
