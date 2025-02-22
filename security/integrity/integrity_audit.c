<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2008 IBM Corporation
 * Author: Mimi Zohar <zohar@us.ibm.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
=======
>>>>>>> upstream/android-13
 * File: integrity_audit.c
 *	Audit calls for the integrity subsystem
 */

#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/audit.h>
#include "integrity.h"

static int integrity_audit_info;

/* ima_audit_setup - enable informational auditing messages */
static int __init integrity_audit_setup(char *str)
{
	unsigned long audit;

	if (!kstrtoul(str, 0, &audit))
		integrity_audit_info = audit ? 1 : 0;
	return 1;
}
__setup("integrity_audit=", integrity_audit_setup);

void integrity_audit_msg(int audit_msgno, struct inode *inode,
			 const unsigned char *fname, const char *op,
			 const char *cause, int result, int audit_info)
{
<<<<<<< HEAD
=======
	integrity_audit_message(audit_msgno, inode, fname, op, cause,
				result, audit_info, 0);
}

void integrity_audit_message(int audit_msgno, struct inode *inode,
			     const unsigned char *fname, const char *op,
			     const char *cause, int result, int audit_info,
			     int errno)
{
>>>>>>> upstream/android-13
	struct audit_buffer *ab;
	char name[TASK_COMM_LEN];

	if (!integrity_audit_info && audit_info == 1)	/* Skip info messages */
		return;

	ab = audit_log_start(audit_context(), GFP_KERNEL, audit_msgno);
<<<<<<< HEAD
	audit_log_format(ab, "pid=%d uid=%u auid=%u ses=%u",
			 task_pid_nr(current),
			 from_kuid(&init_user_ns, current_cred()->uid),
=======
	if (!ab)
		return;
	audit_log_format(ab, "pid=%d uid=%u auid=%u ses=%u",
			 task_pid_nr(current),
			 from_kuid(&init_user_ns, current_uid()),
>>>>>>> upstream/android-13
			 from_kuid(&init_user_ns, audit_get_loginuid(current)),
			 audit_get_sessionid(current));
	audit_log_task_context(ab);
	audit_log_format(ab, " op=%s cause=%s comm=", op, cause);
	audit_log_untrustedstring(ab, get_task_comm(name, current));
	if (fname) {
		audit_log_format(ab, " name=");
		audit_log_untrustedstring(ab, fname);
	}
	if (inode) {
		audit_log_format(ab, " dev=");
		audit_log_untrustedstring(ab, inode->i_sb->s_id);
		audit_log_format(ab, " ino=%lu", inode->i_ino);
	}
<<<<<<< HEAD
	audit_log_format(ab, " res=%d", !result);
=======
	audit_log_format(ab, " res=%d errno=%d", !result, errno);
>>>>>>> upstream/android-13
	audit_log_end(ab);
}
