/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _CODA_INT_
#define _CODA_INT_

struct dentry;
struct file;

extern struct file_system_type coda_fs_type;
extern unsigned long coda_timeout;
extern int coda_hard;
extern int coda_fake_statfs;

void coda_destroy_inodecache(void);
int __init coda_init_inodecache(void);
int coda_fsync(struct file *coda_file, loff_t start, loff_t end, int datasync);
<<<<<<< HEAD
void coda_sysctl_init(void);
void coda_sysctl_clean(void);

=======

#ifdef CONFIG_SYSCTL
void coda_sysctl_init(void);
void coda_sysctl_clean(void);
#else
static inline void coda_sysctl_init(void)
{
}

static inline void coda_sysctl_clean(void)
{
}
#endif
>>>>>>> upstream/android-13
#endif  /*  _CODA_INT_  */


