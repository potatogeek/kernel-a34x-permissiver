/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2012-2018 ARM Limited or its affiliates. */
=======
/* Copyright (C) 2012-2019 ARM Limited (or its affiliates). */
>>>>>>> upstream/android-13

/* \file cc_pm.h
 */

#ifndef __CC_POWER_MGR_H__
#define __CC_POWER_MGR_H__

#include "cc_driver.h"

#define CC_SUSPEND_TIMEOUT 3000

#if defined(CONFIG_PM)

extern const struct dev_pm_ops ccree_pm;

<<<<<<< HEAD
int cc_pm_init(struct cc_drvdata *drvdata);
void cc_pm_go(struct cc_drvdata *drvdata);
void cc_pm_fini(struct cc_drvdata *drvdata);
int cc_pm_suspend(struct device *dev);
int cc_pm_resume(struct device *dev);
int cc_pm_get(struct device *dev);
int cc_pm_put_suspend(struct device *dev);

#else

static inline int cc_pm_init(struct cc_drvdata *drvdata)
{
	return 0;
}

static inline void cc_pm_go(struct cc_drvdata *drvdata) {}

static inline void cc_pm_fini(struct cc_drvdata *drvdata) {}

static inline int cc_pm_suspend(struct device *dev)
{
	return 0;
}

static inline int cc_pm_resume(struct device *dev)
{
	return 0;
}

=======
int cc_pm_get(struct device *dev);
void cc_pm_put_suspend(struct device *dev);

#else

>>>>>>> upstream/android-13
static inline int cc_pm_get(struct device *dev)
{
	return 0;
}

<<<<<<< HEAD
static inline int cc_pm_put_suspend(struct device *dev)
{
	return 0;
}
=======
static inline void cc_pm_put_suspend(struct device *dev) {}
>>>>>>> upstream/android-13

#endif

#endif /*__POWER_MGR_H__*/
