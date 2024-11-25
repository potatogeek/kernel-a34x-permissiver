/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/reboot.h>
#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

<<<<<<< HEAD
extern void timer_init(int irq);

extern void __init mmp_map_io(void);
=======
extern void mmp_timer_init(int irq, unsigned long rate);

extern void __init mmp_map_io(void);
extern void __init mmp2_map_io(void);
>>>>>>> upstream/android-13
extern void mmp_restart(enum reboot_mode, const char *);
