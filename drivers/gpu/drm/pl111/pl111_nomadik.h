// SPDX-License-Identifier: GPL-2.0+
<<<<<<< HEAD
#include <linux/device.h>
=======
>>>>>>> upstream/android-13

#ifndef PL111_NOMADIK_H
#define PL111_NOMADIK_H
#endif

<<<<<<< HEAD
=======
struct device;

>>>>>>> upstream/android-13
#ifdef CONFIG_ARCH_NOMADIK

void pl111_nomadik_init(struct device *dev);

#else

static inline void pl111_nomadik_init(struct device *dev)
{
}

#endif
