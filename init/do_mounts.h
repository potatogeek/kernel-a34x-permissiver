/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/mount.h>
#include <linux/major.h>
#include <linux/root_dev.h>
<<<<<<< HEAD

void  change_floppy(char *fmt, ...);
=======
#include <linux/init_syscalls.h>

>>>>>>> upstream/android-13
void  mount_block_root(char *name, int flags);
void  mount_root(void);
extern int root_mountflags;

<<<<<<< HEAD
static inline int create_dev(char *name, dev_t dev)
{
	ksys_unlink(name);
	return ksys_mknod(name, S_IFBLK|0600, new_encode_dev(dev));
}

static inline u32 bstat(char *name)
{
	struct kstat stat;
	if (vfs_stat(name, &stat) != 0)
		return 0;
	if (!S_ISBLK(stat.mode))
		return 0;
	return stat.rdev;
=======
static inline __init int create_dev(char *name, dev_t dev)
{
	init_unlink(name);
	return init_mknod(name, S_IFBLK | 0600, new_encode_dev(dev));
>>>>>>> upstream/android-13
}

#ifdef CONFIG_BLK_DEV_RAM

int __init rd_load_disk(int n);
int __init rd_load_image(char *from);

#else

static inline int rd_load_disk(int n) { return 0; }
static inline int rd_load_image(char *from) { return 0; }

#endif

#ifdef CONFIG_BLK_DEV_INITRD

bool __init initrd_load(void);

#else

static inline bool initrd_load(void) { return false; }

#endif
<<<<<<< HEAD

#ifdef CONFIG_BLK_DEV_MD

void md_run_setup(void);

#else

static inline void md_run_setup(void) {}

#endif
=======
>>>>>>> upstream/android-13
