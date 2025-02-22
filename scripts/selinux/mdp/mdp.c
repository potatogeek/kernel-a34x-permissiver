<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *
 * mdp - make dummy policy
 *
 * When pointed at a kernel tree, builds a dummy policy for that kernel
 * with exactly one type with full rights to itself.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
=======
>>>>>>> upstream/android-13
 * Copyright (C) IBM Corporation, 2006
 *
 * Authors: Serge E. Hallyn <serue@us.ibm.com>
 */


/* NOTE: we really do want to use the kernel headers here */
#define __EXPORTED_HEADERS__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
<<<<<<< HEAD
=======
#include <linux/kconfig.h>
>>>>>>> upstream/android-13

static void usage(char *name)
{
	printf("usage: %s [-m] policy_file context_file\n", name);
	exit(1);
}

/* Class/perm mapping support */
struct security_class_mapping {
	const char *name;
	const char *perms[sizeof(unsigned) * 8 + 1];
};

#include "classmap.h"
#include "initial_sid_to_string.h"
<<<<<<< HEAD
=======
#include "policycap_names.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
>>>>>>> upstream/android-13

int main(int argc, char *argv[])
{
	int i, j, mls = 0;
	int initial_sid_to_string_len;
	char **arg, *polout, *ctxout;

	FILE *fout;

	if (argc < 3)
		usage(argv[0]);
	arg = argv+1;
	if (argc==4 && strcmp(argv[1], "-m") == 0) {
		mls = 1;
		arg++;
	}
	polout = *arg++;
	ctxout = *arg;

	fout = fopen(polout, "w");
	if (!fout) {
		printf("Could not open %s for writing\n", polout);
		usage(argv[0]);
	}

	/* print out the classes */
	for (i = 0; secclass_map[i].name; i++)
		fprintf(fout, "class %s\n", secclass_map[i].name);
	fprintf(fout, "\n");

	initial_sid_to_string_len = sizeof(initial_sid_to_string) / sizeof (char *);
	/* print out the sids */
<<<<<<< HEAD
	for (i = 1; i < initial_sid_to_string_len; i++)
		fprintf(fout, "sid %s\n", initial_sid_to_string[i]);
=======
	for (i = 1; i < initial_sid_to_string_len; i++) {
		const char *name = initial_sid_to_string[i];

		if (name)
			fprintf(fout, "sid %s\n", name);
		else
			fprintf(fout, "sid unused%d\n", i);
	}
>>>>>>> upstream/android-13
	fprintf(fout, "\n");

	/* print out the class permissions */
	for (i = 0; secclass_map[i].name; i++) {
		struct security_class_mapping *map = &secclass_map[i];
		fprintf(fout, "class %s\n", map->name);
		fprintf(fout, "{\n");
		for (j = 0; map->perms[j]; j++)
			fprintf(fout, "\t%s\n", map->perms[j]);
		fprintf(fout, "}\n\n");
	}
	fprintf(fout, "\n");

<<<<<<< HEAD
	/* NOW PRINT OUT MLS STUFF */
	if (mls) {
		printf("MLS not yet implemented\n");
		exit(1);
	}

=======
	/* print out mls declarations and constraints */
	if (mls) {
		fprintf(fout, "sensitivity s0;\n");
		fprintf(fout, "sensitivity s1;\n");
		fprintf(fout, "dominance { s0 s1 }\n");
		fprintf(fout, "category c0;\n");
		fprintf(fout, "category c1;\n");
		fprintf(fout, "level s0:c0.c1;\n");
		fprintf(fout, "level s1:c0.c1;\n");
#define SYSTEMLOW "s0"
#define SYSTEMHIGH "s1:c0.c1"
		for (i = 0; secclass_map[i].name; i++) {
			struct security_class_mapping *map = &secclass_map[i];

			fprintf(fout, "mlsconstrain %s {\n", map->name);
			for (j = 0; map->perms[j]; j++)
				fprintf(fout, "\t%s\n", map->perms[j]);
			/*
			 * This requires all subjects and objects to be
			 * single-level (l2 eq h2), and that the subject
			 * level dominate the object level (h1 dom h2)
			 * in order to have any permissions to it.
			 */
			fprintf(fout, "} (l2 eq h2 and h1 dom h2);\n\n");
		}
	}

	/* enable all policy capabilities */
	for (i = 0; i < ARRAY_SIZE(selinux_policycap_names); i++)
		fprintf(fout, "policycap %s;\n", selinux_policycap_names[i]);

>>>>>>> upstream/android-13
	/* types, roles, and allows */
	fprintf(fout, "type base_t;\n");
	fprintf(fout, "role base_r;\n");
	fprintf(fout, "role base_r types { base_t };\n");
	for (i = 0; secclass_map[i].name; i++)
		fprintf(fout, "allow base_t base_t:%s *;\n",
			secclass_map[i].name);
<<<<<<< HEAD
	fprintf(fout, "user user_u roles { base_r };\n");
	fprintf(fout, "\n");

	/* default sids */
	for (i = 1; i < initial_sid_to_string_len; i++)
		fprintf(fout, "sid %s user_u:base_r:base_t\n", initial_sid_to_string[i]);
	fprintf(fout, "\n");

	fprintf(fout, "fs_use_xattr ext2 user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr ext3 user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr ext4 user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr jfs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr xfs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr reiserfs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr jffs2 user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_xattr gfs2 user_u:base_r:base_t;\n");

	fprintf(fout, "fs_use_task eventpollfs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_task pipefs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_task sockfs user_u:base_r:base_t;\n");

	fprintf(fout, "fs_use_trans mqueue user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_trans devpts user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_trans hugetlbfs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_trans tmpfs user_u:base_r:base_t;\n");
	fprintf(fout, "fs_use_trans shm user_u:base_r:base_t;\n");

	fprintf(fout, "genfscon proc / user_u:base_r:base_t\n");
=======
	fprintf(fout, "user user_u roles { base_r }");
	if (mls)
		fprintf(fout, " level %s range %s - %s", SYSTEMLOW,
			SYSTEMLOW, SYSTEMHIGH);
	fprintf(fout, ";\n");

#define SUBJUSERROLETYPE "user_u:base_r:base_t"
#define OBJUSERROLETYPE "user_u:object_r:base_t"

	/* default sids */
	for (i = 1; i < initial_sid_to_string_len; i++) {
		const char *name = initial_sid_to_string[i];

		if (name)
			fprintf(fout, "sid %s ", name);
		else
			fprintf(fout, "sid unused%d\n", i);
		fprintf(fout, SUBJUSERROLETYPE "%s\n",
			mls ? ":" SYSTEMLOW : "");
	}
	fprintf(fout, "\n");

#define FS_USE(behavior, fstype)			    \
	fprintf(fout, "fs_use_%s %s " OBJUSERROLETYPE "%s;\n", \
		behavior, fstype, mls ? ":" SYSTEMLOW : "")

	/*
	 * Filesystems whose inode labels can be fetched via getxattr.
	 */
#ifdef CONFIG_EXT2_FS_SECURITY
	FS_USE("xattr", "ext2");
#endif
#ifdef CONFIG_EXT4_FS_SECURITY
#ifdef CONFIG_EXT4_USE_FOR_EXT2
	FS_USE("xattr", "ext2");
#endif
	FS_USE("xattr", "ext3");
	FS_USE("xattr", "ext4");
#endif
#ifdef CONFIG_JFS_SECURITY
	FS_USE("xattr", "jfs");
#endif
#ifdef CONFIG_REISERFS_FS_SECURITY
	FS_USE("xattr", "reiserfs");
#endif
#ifdef CONFIG_JFFS2_FS_SECURITY
	FS_USE("xattr", "jffs2");
#endif
#ifdef CONFIG_XFS_FS
	FS_USE("xattr", "xfs");
#endif
#ifdef CONFIG_GFS2_FS
	FS_USE("xattr", "gfs2");
#endif
#ifdef CONFIG_BTRFS_FS
	FS_USE("xattr", "btrfs");
#endif
#ifdef CONFIG_F2FS_FS_SECURITY
	FS_USE("xattr", "f2fs");
#endif
#ifdef CONFIG_OCFS2_FS
	FS_USE("xattr", "ocsfs2");
#endif
#ifdef CONFIG_OVERLAY_FS
	FS_USE("xattr", "overlay");
#endif
#ifdef CONFIG_SQUASHFS_XATTR
	FS_USE("xattr", "squashfs");
#endif

	/*
	 * Filesystems whose inodes are labeled from allocating task.
	 */
	FS_USE("task", "pipefs");
	FS_USE("task", "sockfs");

	/*
	 * Filesystems whose inode labels are computed from both
	 * the allocating task and the superblock label.
	 */
#ifdef CONFIG_UNIX98_PTYS
	FS_USE("trans", "devpts");
#endif
#ifdef CONFIG_HUGETLBFS
	FS_USE("trans", "hugetlbfs");
#endif
#ifdef CONFIG_TMPFS
	FS_USE("trans", "tmpfs");
#endif
#ifdef CONFIG_DEVTMPFS
	FS_USE("trans", "devtmpfs");
#endif
#ifdef CONFIG_POSIX_MQUEUE
	FS_USE("trans", "mqueue");
#endif

#define GENFSCON(fstype, prefix)			     \
	fprintf(fout, "genfscon %s %s " OBJUSERROLETYPE "%s\n", \
		fstype, prefix, mls ? ":" SYSTEMLOW : "")

	/*
	 * Filesystems whose inodes are labeled from path prefix match
	 * relative to the filesystem root.  Depending on the filesystem,
	 * only a single label for all inodes may be supported.  Here
	 * we list the filesystem types for which per-file labeling is
	 * supported using genfscon; any other filesystem type can also
	 * be added by only with a single entry for all of its inodes.
	 */
#ifdef CONFIG_PROC_FS
	GENFSCON("proc", "/");
#endif
#ifdef CONFIG_SECURITY_SELINUX
	GENFSCON("selinuxfs", "/");
#endif
#ifdef CONFIG_SYSFS
	GENFSCON("sysfs", "/");
#endif
#ifdef CONFIG_DEBUG_FS
	GENFSCON("debugfs", "/");
#endif
#ifdef CONFIG_TRACING
	GENFSCON("tracefs", "/");
#endif
#ifdef CONFIG_PSTORE
	GENFSCON("pstore", "/");
#endif
	GENFSCON("cgroup", "/");
	GENFSCON("cgroup2", "/");
>>>>>>> upstream/android-13

	fclose(fout);

	fout = fopen(ctxout, "w");
	if (!fout) {
		printf("Wrote policy, but cannot open %s for writing\n", ctxout);
		usage(argv[0]);
	}
<<<<<<< HEAD
	fprintf(fout, "/ user_u:base_r:base_t\n");
	fprintf(fout, "/.* user_u:base_r:base_t\n");
=======
	fprintf(fout, "/ " OBJUSERROLETYPE "%s\n", mls ? ":" SYSTEMLOW : "");
	fprintf(fout, "/.* " OBJUSERROLETYPE "%s\n", mls ? ":" SYSTEMLOW : "");
>>>>>>> upstream/android-13
	fclose(fout);

	return 0;
}
