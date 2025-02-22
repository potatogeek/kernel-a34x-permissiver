<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/fd.h>
#include <linux/tty.h>
#include <linux/suspend.h>
#include <linux/root_dev.h>
#include <linux/security.h>
#include <linux/delay.h>
#include <linux/genhd.h>
#include <linux/mount.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/initrd.h>
#include <linux/async.h>
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <linux/ramfs.h>
#include <linux/shmem_fs.h>

#include <linux/nfs_fs.h>
#include <linux/nfs_fs_sb.h>
#include <linux/nfs_mount.h>
<<<<<<< HEAD

#include "do_mounts.h"

int __initdata rd_doload;	/* 1 = load RAM disk, 0 = don't load */

=======
#include <linux/raid/detect.h>
#include <uapi/linux/mount.h>

#include "do_mounts.h"

>>>>>>> upstream/android-13
int root_mountflags = MS_RDONLY | MS_SILENT;
static char * __initdata root_device_name;
static char __initdata saved_root_name[64];
static int root_wait;

dev_t ROOT_DEV;

static int __init load_ramdisk(char *str)
{
<<<<<<< HEAD
	rd_doload = simple_strtol(str,NULL,0) & 3;
=======
	pr_warn("ignoring the deprecated load_ramdisk= option\n");
>>>>>>> upstream/android-13
	return 1;
}
__setup("load_ramdisk=", load_ramdisk);

static int __init readonly(char *str)
{
	if (*str)
		return 0;
	root_mountflags |= MS_RDONLY;
	return 1;
}

static int __init readwrite(char *str)
{
	if (*str)
		return 0;
	root_mountflags &= ~MS_RDONLY;
	return 1;
}

__setup("ro", readonly);
__setup("rw", readwrite);

#ifdef CONFIG_BLOCK
struct uuidcmp {
	const char *uuid;
	int len;
};

/**
 * match_dev_by_uuid - callback for finding a partition using its uuid
 * @dev:	device passed in by the caller
 * @data:	opaque pointer to the desired struct uuidcmp to match
 *
 * Returns 1 if the device matches, and 0 otherwise.
 */
static int match_dev_by_uuid(struct device *dev, const void *data)
{
<<<<<<< HEAD
	const struct uuidcmp *cmp = data;
	struct hd_struct *part = dev_to_part(dev);

	if (!part->info)
		goto no_match;

	if (strncasecmp(cmp->uuid, part->info->uuid, cmp->len))
		goto no_match;

	return 1;
no_match:
	return 0;
}


=======
	struct block_device *bdev = dev_to_bdev(dev);
	const struct uuidcmp *cmp = data;

	if (!bdev->bd_meta_info ||
	    strncasecmp(cmp->uuid, bdev->bd_meta_info->uuid, cmp->len))
		return 0;
	return 1;
}

>>>>>>> upstream/android-13
/**
 * devt_from_partuuid - looks up the dev_t of a partition by its UUID
 * @uuid_str:	char array containing ascii UUID
 *
 * The function will return the first partition which contains a matching
 * UUID value in its partition_meta_info struct.  This does not search
 * by filesystem UUIDs.
 *
 * If @uuid_str is followed by a "/PARTNROFF=%d", then the number will be
 * extracted and used as an offset from the partition identified by the UUID.
 *
 * Returns the matching dev_t on success or 0 on failure.
 */
static dev_t devt_from_partuuid(const char *uuid_str)
{
<<<<<<< HEAD
	dev_t res = 0;
	struct uuidcmp cmp;
	struct device *dev = NULL;
	struct gendisk *disk;
	struct hd_struct *part;
	int offset = 0;
	bool clear_root_wait = false;
=======
	struct uuidcmp cmp;
	struct device *dev = NULL;
	dev_t devt = 0;
	int offset = 0;
>>>>>>> upstream/android-13
	char *slash;

	cmp.uuid = uuid_str;

	slash = strchr(uuid_str, '/');
	/* Check for optional partition number offset attributes. */
	if (slash) {
		char c = 0;
<<<<<<< HEAD
		/* Explicitly fail on poor PARTUUID syntax. */
		if (sscanf(slash + 1,
			   "PARTNROFF=%d%c", &offset, &c) != 1) {
			clear_root_wait = true;
			goto done;
		}
=======

		/* Explicitly fail on poor PARTUUID syntax. */
		if (sscanf(slash + 1, "PARTNROFF=%d%c", &offset, &c) != 1)
			goto clear_root_wait;
>>>>>>> upstream/android-13
		cmp.len = slash - uuid_str;
	} else {
		cmp.len = strlen(uuid_str);
	}

<<<<<<< HEAD
	if (!cmp.len) {
		clear_root_wait = true;
		goto done;
	}

	dev = class_find_device(&block_class, NULL, &cmp,
				&match_dev_by_uuid);
	if (!dev)
		goto done;

	res = dev->devt;

	/* Attempt to find the partition by offset. */
	if (!offset)
		goto no_offset;

	res = 0;
	disk = part_to_disk(dev_to_part(dev));
	part = disk_get_part(disk, dev_to_part(dev)->partno + offset);
	if (part) {
		res = part_devt(part);
		put_device(part_to_dev(part));
	}

no_offset:
	put_device(dev);
done:
	if (clear_root_wait) {
		pr_err("VFS: PARTUUID= is invalid.\n"
		       "Expected PARTUUID=<valid-uuid-id>[/PARTNROFF=%%d]\n");
		if (root_wait)
			pr_err("Disabling rootwait; root= is invalid.\n");
		root_wait = 0;
	}
	return res;
}
#endif
=======
	if (!cmp.len)
		goto clear_root_wait;

	dev = class_find_device(&block_class, NULL, &cmp, &match_dev_by_uuid);
	if (!dev)
		return 0;

	if (offset) {
		/*
		 * Attempt to find the requested partition by adding an offset
		 * to the partition number found by UUID.
		 */
		devt = part_devt(dev_to_disk(dev),
				 dev_to_bdev(dev)->bd_partno + offset);
	} else {
		devt = dev->devt;
	}

	put_device(dev);
	return devt;

clear_root_wait:
	pr_err("VFS: PARTUUID= is invalid.\n"
	       "Expected PARTUUID=<valid-uuid-id>[/PARTNROFF=%%d]\n");
	if (root_wait)
		pr_err("Disabling rootwait; root= is invalid.\n");
	root_wait = 0;
	return 0;
}

/**
 * match_dev_by_label - callback for finding a partition using its label
 * @dev:	device passed in by the caller
 * @data:	opaque pointer to the label to match
 *
 * Returns 1 if the device matches, and 0 otherwise.
 */
static int match_dev_by_label(struct device *dev, const void *data)
{
	struct block_device *bdev = dev_to_bdev(dev);
	const char *label = data;

	if (!bdev->bd_meta_info || strcmp(label, bdev->bd_meta_info->volname))
		return 0;
	return 1;
}

static dev_t devt_from_partlabel(const char *label)
{
	struct device *dev;
	dev_t devt = 0;

	dev = class_find_device(&block_class, NULL, label, &match_dev_by_label);
	if (dev) {
		devt = dev->devt;
		put_device(dev);
	}

	return devt;
}

static dev_t devt_from_devname(const char *name)
{
	dev_t devt = 0;
	int part;
	char s[32];
	char *p;

	if (strlen(name) > 31)
		return 0;
	strcpy(s, name);
	for (p = s; *p; p++) {
		if (*p == '/')
			*p = '!';
	}

	devt = blk_lookup_devt(s, 0);
	if (devt)
		return devt;

	/*
	 * Try non-existent, but valid partition, which may only exist after
	 * opening the device, like partitioned md devices.
	 */
	while (p > s && isdigit(p[-1]))
		p--;
	if (p == s || !*p || *p == '0')
		return 0;

	/* try disk name without <part number> */
	part = simple_strtoul(p, NULL, 10);
	*p = '\0';
	devt = blk_lookup_devt(s, part);
	if (devt)
		return devt;

	/* try disk name without p<part number> */
	if (p < s + 2 || !isdigit(p[-2]) || p[-1] != 'p')
		return 0;
	p[-1] = '\0';
	return blk_lookup_devt(s, part);
}
#endif /* CONFIG_BLOCK */

static dev_t devt_from_devnum(const char *name)
{
	unsigned maj, min, offset;
	dev_t devt = 0;
	char *p, dummy;

	if (sscanf(name, "%u:%u%c", &maj, &min, &dummy) == 2 ||
	    sscanf(name, "%u:%u:%u:%c", &maj, &min, &offset, &dummy) == 3) {
		devt = MKDEV(maj, min);
		if (maj != MAJOR(devt) || min != MINOR(devt))
			return 0;
	} else {
		devt = new_decode_dev(simple_strtoul(name, &p, 16));
		if (*p)
			return 0;
	}

	return devt;
}
>>>>>>> upstream/android-13

/*
 *	Convert a name into device number.  We accept the following variants:
 *
 *	1) <hex_major><hex_minor> device number in hexadecimal represents itself
 *         no leading 0x, for example b302.
 *	2) /dev/nfs represents Root_NFS (0xff)
 *	3) /dev/<disk_name> represents the device number of disk
 *	4) /dev/<disk_name><decimal> represents the device number
 *         of partition - device number of disk plus the partition number
 *	5) /dev/<disk_name>p<decimal> - same as the above, that form is
 *	   used when disk name of partitioned disk ends on a digit.
 *	6) PARTUUID=00112233-4455-6677-8899-AABBCCDDEEFF representing the
 *	   unique id of a partition if the partition table provides it.
 *	   The UUID may be either an EFI/GPT UUID, or refer to an MSDOS
 *	   partition using the format SSSSSSSS-PP, where SSSSSSSS is a zero-
 *	   filled hex representation of the 32-bit "NT disk signature", and PP
 *	   is a zero-filled hex representation of the 1-based partition number.
 *	7) PARTUUID=<UUID>/PARTNROFF=<int> to select a partition in relation to
 *	   a partition with a known unique id.
 *	8) <major>:<minor> major and minor number of the device separated by
 *	   a colon.
<<<<<<< HEAD
=======
 *	9) PARTLABEL=<name> with name being the GPT partition label.
 *	   MSDOS partitions do not support labels!
 *	10) /dev/cifs represents Root_CIFS (0xfe)
>>>>>>> upstream/android-13
 *
 *	If name doesn't have fall into the categories above, we return (0,0).
 *	block_class is used to check if something is a disk name. If the disk
 *	name contains slashes, the device name has them replaced with
 *	bangs.
 */
<<<<<<< HEAD

dev_t name_to_dev_t(const char *name)
{
	char s[32];
	char *p;
	dev_t res = 0;
	int part;

#ifdef CONFIG_BLOCK
	if (strncmp(name, "PARTUUID=", 9) == 0) {
		name += 9;
		res = devt_from_partuuid(name);
		if (!res)
			goto fail;
		goto done;
	}
#endif

	if (strncmp(name, "/dev/", 5) != 0) {
		unsigned maj, min, offset;
		char dummy;

		if ((sscanf(name, "%u:%u%c", &maj, &min, &dummy) == 2) ||
		    (sscanf(name, "%u:%u:%u:%c", &maj, &min, &offset, &dummy) == 3)) {
			res = MKDEV(maj, min);
			if (maj != MAJOR(res) || min != MINOR(res))
				goto fail;
		} else {
			res = new_decode_dev(simple_strtoul(name, &p, 16));
			if (*p)
				goto fail;
		}
		goto done;
	}

	name += 5;
	res = Root_NFS;
	if (strcmp(name, "nfs") == 0)
		goto done;
	res = Root_RAM0;
	if (strcmp(name, "ram") == 0)
		goto done;

	if (strlen(name) > 31)
		goto fail;
	strcpy(s, name);
	for (p = s; *p; p++)
		if (*p == '/')
			*p = '!';
	res = blk_lookup_devt(s, 0);
	if (res)
		goto done;

	/*
	 * try non-existent, but valid partition, which may only exist
	 * after revalidating the disk, like partitioned md devices
	 */
	while (p > s && isdigit(p[-1]))
		p--;
	if (p == s || !*p || *p == '0')
		goto fail;

	/* try disk name without <part number> */
	part = simple_strtoul(p, NULL, 10);
	*p = '\0';
	res = blk_lookup_devt(s, part);
	if (res)
		goto done;

	/* try disk name without p<part number> */
	if (p < s + 2 || !isdigit(p[-2]) || p[-1] != 'p')
		goto fail;
	p[-1] = '\0';
	res = blk_lookup_devt(s, part);
	if (res)
		goto done;

fail:
	return 0;
done:
	return res;
=======
dev_t name_to_dev_t(const char *name)
{
	if (strcmp(name, "/dev/nfs") == 0)
		return Root_NFS;
	if (strcmp(name, "/dev/cifs") == 0)
		return Root_CIFS;
	if (strcmp(name, "/dev/ram") == 0)
		return Root_RAM0;
#ifdef CONFIG_BLOCK
	if (strncmp(name, "PARTUUID=", 9) == 0)
		return devt_from_partuuid(name + 9);
	if (strncmp(name, "PARTLABEL=", 10) == 0)
		return devt_from_partlabel(name + 10);
	if (strncmp(name, "/dev/", 5) == 0)
		return devt_from_devname(name + 5);
#endif
	return devt_from_devnum(name);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(name_to_dev_t);

static int __init root_dev_setup(char *line)
{
	strlcpy(saved_root_name, line, sizeof(saved_root_name));
	return 1;
}

__setup("root=", root_dev_setup);

static int __init rootwait_setup(char *str)
{
	if (*str)
		return 0;
	root_wait = 1;
	return 1;
}

__setup("rootwait", rootwait_setup);

static char * __initdata root_mount_data;
static int __init root_data_setup(char *str)
{
	root_mount_data = str;
	return 1;
}

static char * __initdata root_fs_names;
static int __init fs_names_setup(char *str)
{
	root_fs_names = str;
	return 1;
}

static unsigned int __initdata root_delay;
static int __init root_delay_setup(char *str)
{
	root_delay = simple_strtoul(str, NULL, 0);
	return 1;
}

__setup("rootflags=", root_data_setup);
__setup("rootfstype=", fs_names_setup);
__setup("rootdelay=", root_delay_setup);

<<<<<<< HEAD
static void __init get_fs_names(char *page)
{
	char *s = page;

	if (root_fs_names) {
		strcpy(page, root_fs_names);
		while (*s++) {
			if (s[-1] == ',')
				s[-1] = '\0';
		}
	} else {
		int len = get_filesystem_list(page);
		char *p, *next;

		page[len] = '\0';
		for (p = page-1; p; p = next) {
			next = strchr(++p, '\n');
			if (*p++ != '\t')
				continue;
			while ((*s++ = *p++) != '\n')
				;
			s[-1] = '\0';
		}
	}
	*s = '\0';
}

static int __init do_mount_root(char *name, char *fs, int flags, void *data)
{
	struct super_block *s;
	int err = ksys_mount(name, "/root", fs, flags, data);
	if (err)
		return err;

	ksys_chdir("/root");
=======
/* This can return zero length strings. Caller should check */
static int __init split_fs_names(char *page, size_t size, char *names)
{
	int count = 1;
	char *p = page;

	strlcpy(p, root_fs_names, size);
	while (*p++) {
		if (p[-1] == ',') {
			p[-1] = '\0';
			count++;
		}
	}

	return count;
}

static int __init do_mount_root(const char *name, const char *fs,
				 const int flags, const void *data)
{
	struct super_block *s;
	struct page *p = NULL;
	char *data_page = NULL;
	int ret;

	if (data) {
		/* init_mount() requires a full page as fifth argument */
		p = alloc_page(GFP_KERNEL);
		if (!p)
			return -ENOMEM;
		data_page = page_address(p);
		/* zero-pad. init_mount() will make sure it's terminated */
		strncpy(data_page, data, PAGE_SIZE);
	}

	ret = init_mount(name, "/root", fs, flags, data_page);
	if (ret)
		goto out;

	init_chdir("/root");
>>>>>>> upstream/android-13
	s = current->fs->pwd.dentry->d_sb;
	ROOT_DEV = s->s_dev;
	printk(KERN_INFO
	       "VFS: Mounted root (%s filesystem)%s on device %u:%u.\n",
	       s->s_type->name,
	       sb_rdonly(s) ? " readonly" : "",
	       MAJOR(ROOT_DEV), MINOR(ROOT_DEV));
<<<<<<< HEAD
	return 0;
=======

out:
	if (p)
		put_page(p);
	return ret;
>>>>>>> upstream/android-13
}

void __init mount_block_root(char *name, int flags)
{
	struct page *page = alloc_page(GFP_KERNEL);
	char *fs_names = page_address(page);
	char *p;
<<<<<<< HEAD
#ifdef CONFIG_BLOCK
	char b[BDEVNAME_SIZE];
#else
	const char *b = name;
#endif

	get_fs_names(fs_names);
retry:
	for (p = fs_names; *p; p += strlen(p)+1) {
		int err = do_mount_root(name, p, flags, root_mount_data);
=======
	char b[BDEVNAME_SIZE];
	int num_fs, i;

	scnprintf(b, BDEVNAME_SIZE, "unknown-block(%u,%u)",
		  MAJOR(ROOT_DEV), MINOR(ROOT_DEV));
	if (root_fs_names)
		num_fs = split_fs_names(fs_names, PAGE_SIZE, root_fs_names);
	else
		num_fs = list_bdev_fs_names(fs_names, PAGE_SIZE);
retry:
	for (i = 0, p = fs_names; i < num_fs; i++, p += strlen(p)+1) {
		int err;

		if (!*p)
			continue;
		err = do_mount_root(name, p, flags, root_mount_data);
>>>>>>> upstream/android-13
		switch (err) {
			case 0:
				goto out;
			case -EACCES:
			case -EINVAL:
				continue;
		}
	        /*
		 * Allow the user to distinguish between failed sys_open
		 * and bad superblock on root device.
		 * and give them a list of the available devices
		 */
<<<<<<< HEAD
#ifdef CONFIG_BLOCK
		__bdevname(ROOT_DEV, b);
#endif
=======
>>>>>>> upstream/android-13
		printk("VFS: Cannot open root device \"%s\" or %s: error %d\n",
				root_device_name, b, err);
		printk("Please append a correct \"root=\" boot option; here are the available partitions:\n");

		printk_all_partitions();
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_BLOCK_EXT_DEVT
		printk("DEBUG_BLOCK_EXT_DEVT is enabled, you need to specify "
		       "explicit textual name for \"root=\" boot option.\n");
#endif
=======
>>>>>>> upstream/android-13
		panic("VFS: Unable to mount root fs on %s", b);
	}
	if (!(flags & SB_RDONLY)) {
		flags |= SB_RDONLY;
		goto retry;
	}

	printk("List of all partitions:\n");
	printk_all_partitions();
	printk("No filesystem could mount root, tried: ");
<<<<<<< HEAD
	for (p = fs_names; *p; p += strlen(p)+1)
		printk(" %s", p);
	printk("\n");
#ifdef CONFIG_BLOCK
	__bdevname(ROOT_DEV, b);
#endif
=======
	for (i = 0, p = fs_names; i < num_fs; i++, p += strlen(p)+1)
		printk(" %s", p);
	printk("\n");
>>>>>>> upstream/android-13
	panic("VFS: Unable to mount root fs on %s", b);
out:
	put_page(page);
}
 
#ifdef CONFIG_ROOT_NFS

#define NFSROOT_TIMEOUT_MIN	5
#define NFSROOT_TIMEOUT_MAX	30
#define NFSROOT_RETRY_MAX	5

static int __init mount_nfs_root(void)
{
	char *root_dev, *root_data;
	unsigned int timeout;
	int try, err;

	err = nfs_root_data(&root_dev, &root_data);
	if (err != 0)
		return 0;

	/*
	 * The server or network may not be ready, so try several
	 * times.  Stop after a few tries in case the client wants
	 * to fall back to other boot methods.
	 */
	timeout = NFSROOT_TIMEOUT_MIN;
	for (try = 1; ; try++) {
		err = do_mount_root(root_dev, "nfs",
					root_mountflags, root_data);
		if (err == 0)
			return 1;
		if (try > NFSROOT_RETRY_MAX)
			break;

		/* Wait, in case the server refused us immediately */
		ssleep(timeout);
		timeout <<= 1;
		if (timeout > NFSROOT_TIMEOUT_MAX)
			timeout = NFSROOT_TIMEOUT_MAX;
	}
	return 0;
}
#endif

<<<<<<< HEAD
#if defined(CONFIG_BLK_DEV_RAM) || defined(CONFIG_BLK_DEV_FD)
void __init change_floppy(char *fmt, ...)
{
	struct termios termios;
	char buf[80];
	char c;
	int fd;
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	fd = ksys_open("/dev/root", O_RDWR | O_NDELAY, 0);
	if (fd >= 0) {
		ksys_ioctl(fd, FDEJECT, 0);
		ksys_close(fd);
	}
	printk(KERN_NOTICE "VFS: Insert %s and press ENTER\n", buf);
	fd = ksys_open("/dev/console", O_RDWR, 0);
	if (fd >= 0) {
		ksys_ioctl(fd, TCGETS, (long)&termios);
		termios.c_lflag &= ~ICANON;
		ksys_ioctl(fd, TCSETSF, (long)&termios);
		ksys_read(fd, &c, 1);
		termios.c_lflag |= ICANON;
		ksys_ioctl(fd, TCSETSF, (long)&termios);
		ksys_close(fd);
	}
}
#endif

=======
#ifdef CONFIG_CIFS_ROOT

extern int cifs_root_data(char **dev, char **opts);

#define CIFSROOT_TIMEOUT_MIN	5
#define CIFSROOT_TIMEOUT_MAX	30
#define CIFSROOT_RETRY_MAX	5

static int __init mount_cifs_root(void)
{
	char *root_dev, *root_data;
	unsigned int timeout;
	int try, err;

	err = cifs_root_data(&root_dev, &root_data);
	if (err != 0)
		return 0;

	timeout = CIFSROOT_TIMEOUT_MIN;
	for (try = 1; ; try++) {
		err = do_mount_root(root_dev, "cifs", root_mountflags,
				    root_data);
		if (err == 0)
			return 1;
		if (try > CIFSROOT_RETRY_MAX)
			break;

		ssleep(timeout);
		timeout <<= 1;
		if (timeout > CIFSROOT_TIMEOUT_MAX)
			timeout = CIFSROOT_TIMEOUT_MAX;
	}
	return 0;
}
#endif

static bool __init fs_is_nodev(char *fstype)
{
	struct file_system_type *fs = get_fs_type(fstype);
	bool ret = false;

	if (fs) {
		ret = !(fs->fs_flags & FS_REQUIRES_DEV);
		put_filesystem(fs);
	}

	return ret;
}

static int __init mount_nodev_root(void)
{
	char *fs_names, *fstype;
	int err = -EINVAL;
	int num_fs, i;

	fs_names = (void *)__get_free_page(GFP_KERNEL);
	if (!fs_names)
		return -EINVAL;
	num_fs = split_fs_names(fs_names, PAGE_SIZE, root_fs_names);

	for (i = 0, fstype = fs_names; i < num_fs;
	     i++, fstype += strlen(fstype) + 1) {
		if (!*fstype)
			continue;
		if (!fs_is_nodev(fstype))
			continue;
		err = do_mount_root(root_device_name, fstype, root_mountflags,
				    root_mount_data);
		if (!err)
			break;
	}

	free_page((unsigned long)fs_names);
	return err;
}

>>>>>>> upstream/android-13
void __init mount_root(void)
{
#ifdef CONFIG_ROOT_NFS
	if (ROOT_DEV == Root_NFS) {
<<<<<<< HEAD
		if (mount_nfs_root())
			return;

		printk(KERN_ERR "VFS: Unable to mount root fs via NFS, trying floppy.\n");
		ROOT_DEV = Root_FD0;
	}
#endif
#ifdef CONFIG_BLK_DEV_FD
	if (MAJOR(ROOT_DEV) == FLOPPY_MAJOR) {
		/* rd_doload is 2 for a dual initrd/ramload setup */
		if (rd_doload==2) {
			if (rd_load_disk(1)) {
				ROOT_DEV = Root_RAM1;
				root_device_name = NULL;
			}
		} else
			change_floppy("root floppy");
	}
#endif
=======
		if (!mount_nfs_root())
			printk(KERN_ERR "VFS: Unable to mount root fs via NFS.\n");
		return;
	}
#endif
#ifdef CONFIG_CIFS_ROOT
	if (ROOT_DEV == Root_CIFS) {
		if (!mount_cifs_root())
			printk(KERN_ERR "VFS: Unable to mount root fs via SMB.\n");
		return;
	}
#endif
	if (ROOT_DEV == 0 && root_device_name && root_fs_names) {
		if (mount_nodev_root() == 0)
			return;
	}
>>>>>>> upstream/android-13
#ifdef CONFIG_BLOCK
	{
		int err = create_dev("/dev/root", ROOT_DEV);

		if (err < 0)
			pr_emerg("Failed to create /dev/root: %d\n", err);
		mount_block_root("/dev/root", root_mountflags);
	}
#endif
}

/*
 * Prepare the namespace - decide what/where to mount, load ramdisks, etc.
 */
void __init prepare_namespace(void)
{
<<<<<<< HEAD
	int is_floppy;

=======
>>>>>>> upstream/android-13
	if (root_delay) {
		printk(KERN_INFO "Waiting %d sec before mounting root device...\n",
		       root_delay);
		ssleep(root_delay);
	}

	/*
	 * wait for the known devices to complete their probing
	 *
	 * Note: this is a potential source of long boot delays.
	 * For example, it is not atypical to wait 5 seconds here
	 * for the touchpad of a laptop to initialize.
	 */
	wait_for_device_probe();

	md_run_setup();

	if (saved_root_name[0]) {
		root_device_name = saved_root_name;
		if (!strncmp(root_device_name, "mtd", 3) ||
		    !strncmp(root_device_name, "ubi", 3)) {
			mount_block_root(root_device_name, root_mountflags);
			goto out;
		}
		ROOT_DEV = name_to_dev_t(root_device_name);
		if (strncmp(root_device_name, "/dev/", 5) == 0)
			root_device_name += 5;
	}

	if (initrd_load())
		goto out;

	/* wait for any asynchronous scanning to complete */
	if ((ROOT_DEV == 0) && root_wait) {
		printk(KERN_INFO "Waiting for root device %s...\n",
			saved_root_name);
		while (driver_probe_done() != 0 ||
			(ROOT_DEV = name_to_dev_t(saved_root_name)) == 0)
			msleep(5);
		async_synchronize_full();
	}

<<<<<<< HEAD
	is_floppy = MAJOR(ROOT_DEV) == FLOPPY_MAJOR;

	if (is_floppy && rd_doload && rd_load_disk(0))
		ROOT_DEV = Root_RAM0;

	mount_root();
out:
	devtmpfs_mount("dev");
	ksys_mount(".", "/", NULL, MS_MOVE, NULL);
	ksys_chroot(".");
}

static bool is_tmpfs;
static struct dentry *rootfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	static unsigned long once;
	void *fill = ramfs_fill_super;

	if (test_and_set_bit(0, &once))
		return ERR_PTR(-ENODEV);

	if (IS_ENABLED(CONFIG_TMPFS) && is_tmpfs)
		fill = shmem_fill_super;

	return mount_nodev(fs_type, flags, data, fill);
}

static struct file_system_type rootfs_fs_type = {
	.name		= "rootfs",
	.mount		= rootfs_mount,
	.kill_sb	= kill_litter_super,
};

int __init init_rootfs(void)
{
	int err = register_filesystem(&rootfs_fs_type);

	if (err)
		return err;

	if (IS_ENABLED(CONFIG_TMPFS) && !saved_root_name[0] &&
		(!root_fs_names || strstr(root_fs_names, "tmpfs"))) {
		err = shmem_init();
		is_tmpfs = true;
	} else {
		err = init_ramfs_fs();
	}

	if (err)
		unregister_filesystem(&rootfs_fs_type);

	return err;
=======
	mount_root();
out:
	devtmpfs_mount();
	init_mount(".", "/", NULL, MS_MOVE, NULL);
	init_chroot(".");
}

static bool is_tmpfs;
static int rootfs_init_fs_context(struct fs_context *fc)
{
	if (IS_ENABLED(CONFIG_TMPFS) && is_tmpfs)
		return shmem_init_fs_context(fc);

	return ramfs_init_fs_context(fc);
}

struct file_system_type rootfs_fs_type = {
	.name		= "rootfs",
	.init_fs_context = rootfs_init_fs_context,
	.kill_sb	= kill_litter_super,
};

void __init init_rootfs(void)
{
	if (IS_ENABLED(CONFIG_TMPFS) && !saved_root_name[0] &&
		(!root_fs_names || strstr(root_fs_names, "tmpfs")))
		is_tmpfs = true;
>>>>>>> upstream/android-13
}
