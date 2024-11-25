<<<<<<< HEAD
/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 * Copyright 2012 Linaro Ltd.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 * Copyright 2012 Linaro Ltd.
>>>>>>> upstream/android-13
 */

#ifdef CONFIG_CPU_IDLE
extern int imx5_cpuidle_init(void);
extern int imx6q_cpuidle_init(void);
extern int imx6sl_cpuidle_init(void);
extern int imx6sx_cpuidle_init(void);
<<<<<<< HEAD
=======
extern int imx7ulp_cpuidle_init(void);
>>>>>>> upstream/android-13
#else
static inline int imx5_cpuidle_init(void)
{
	return 0;
}
static inline int imx6q_cpuidle_init(void)
{
	return 0;
}
static inline int imx6sl_cpuidle_init(void)
{
	return 0;
}
static inline int imx6sx_cpuidle_init(void)
{
	return 0;
}
<<<<<<< HEAD
=======
static inline int imx7ulp_cpuidle_init(void)
{
	return 0;
}
>>>>>>> upstream/android-13
#endif
