/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
=======

#ifndef _PINCTRL_SINGLE_H
#define _PINCTRL_SINGLE_H

>>>>>>> upstream/android-13
/**
 * irq:		optional wake-up interrupt
 * rearm:	optional soc specific rearm function
 *
 * Note that the irq and rearm setup should come from device
 * tree except for omap where there are still some dependencies
 * to the legacy PRM code.
 */
struct pcs_pdata {
	int irq;
	void (*rearm)(void);
};
<<<<<<< HEAD
=======

#endif /* _PINCTRL_SINGLE_H */
>>>>>>> upstream/android-13
