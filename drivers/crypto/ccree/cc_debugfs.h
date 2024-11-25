/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2012-2018 ARM Limited or its affiliates. */
=======
/* Copyright (C) 2012-2019 ARM Limited (or its affiliates). */
>>>>>>> upstream/android-13

#ifndef __CC_DEBUGFS_H__
#define __CC_DEBUGFS_H__

#ifdef CONFIG_DEBUG_FS
<<<<<<< HEAD
int cc_debugfs_global_init(void);
=======
void cc_debugfs_global_init(void);
>>>>>>> upstream/android-13
void cc_debugfs_global_fini(void);

int cc_debugfs_init(struct cc_drvdata *drvdata);
void cc_debugfs_fini(struct cc_drvdata *drvdata);

#else

<<<<<<< HEAD
static inline int cc_debugfs_global_init(void)
{
	return 0;
}

=======
static inline void cc_debugfs_global_init(void) {}
>>>>>>> upstream/android-13
static inline void cc_debugfs_global_fini(void) {}

static inline int cc_debugfs_init(struct cc_drvdata *drvdata)
{
	return 0;
}

static inline void cc_debugfs_fini(struct cc_drvdata *drvdata) {}

#endif

#endif /*__CC_SYSFS_H__*/
