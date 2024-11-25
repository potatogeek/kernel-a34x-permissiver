<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/fs/binfmt_script.c
 *
 *  Copyright (C) 1996  Martin von Löwis
 *  original #!-checking implemented by tytso.
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/binfmts.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/err.h>
#include <linux/fs.h>

static inline bool spacetab(char c) { return c == ' ' || c == '\t'; }
<<<<<<< HEAD
static inline char *next_non_spacetab(char *first, const char *last)
=======
static inline const char *next_non_spacetab(const char *first, const char *last)
>>>>>>> upstream/android-13
{
	for (; first <= last; first++)
		if (!spacetab(*first))
			return first;
	return NULL;
}
<<<<<<< HEAD
static inline char *next_terminator(char *first, const char *last)
=======
static inline const char *next_terminator(const char *first, const char *last)
>>>>>>> upstream/android-13
{
	for (; first <= last; first++)
		if (spacetab(*first) || !*first)
			return first;
	return NULL;
}

static int load_script(struct linux_binprm *bprm)
{
<<<<<<< HEAD
	const char *i_arg, *i_name;
	char *cp, *buf_end;
=======
	const char *i_name, *i_sep, *i_arg, *i_end, *buf_end;
>>>>>>> upstream/android-13
	struct file *file;
	int retval;

	/* Not ours to exec if we don't start with "#!". */
	if ((bprm->buf[0] != '#') || (bprm->buf[1] != '!'))
		return -ENOEXEC;

	/*
<<<<<<< HEAD
	 * If the script filename will be inaccessible after exec, typically
	 * because it is a "/dev/fd/<fd>/.." path against an O_CLOEXEC fd, give
	 * up now (on the assumption that the interpreter will want to load
	 * this file).
	 */
	if (bprm->interp_flags & BINPRM_FLAGS_PATH_INACCESSIBLE)
		return -ENOENT;

	/* Release since we are not mapping a binary into memory. */
	allow_write_access(bprm->file);
	fput(bprm->file);
	bprm->file = NULL;

	/*
=======
>>>>>>> upstream/android-13
	 * This section handles parsing the #! line into separate
	 * interpreter path and argument strings. We must be careful
	 * because bprm->buf is not yet guaranteed to be NUL-terminated
	 * (though the buffer will have trailing NUL padding when the
	 * file size was smaller than the buffer size).
	 *
	 * We do not want to exec a truncated interpreter path, so either
	 * we find a newline (which indicates nothing is truncated), or
	 * we find a space/tab/NUL after the interpreter path (which
	 * itself may be preceded by spaces/tabs). Truncating the
	 * arguments is fine: the interpreter can re-read the script to
	 * parse them on its own.
	 */
	buf_end = bprm->buf + sizeof(bprm->buf) - 1;
<<<<<<< HEAD
	cp = strnchr(bprm->buf, sizeof(bprm->buf), '\n');
	if (!cp) {
		cp = next_non_spacetab(bprm->buf + 2, buf_end);
		if (!cp)
=======
	i_end = strnchr(bprm->buf, sizeof(bprm->buf), '\n');
	if (!i_end) {
		i_end = next_non_spacetab(bprm->buf + 2, buf_end);
		if (!i_end)
>>>>>>> upstream/android-13
			return -ENOEXEC; /* Entire buf is spaces/tabs */
		/*
		 * If there is no later space/tab/NUL we must assume the
		 * interpreter path is truncated.
		 */
<<<<<<< HEAD
		if (!next_terminator(cp, buf_end))
			return -ENOEXEC;
		cp = buf_end;
	}
	/* NUL-terminate the buffer and any trailing spaces/tabs. */
	*cp = '\0';
	while (cp > bprm->buf) {
		cp--;
		if ((*cp == ' ') || (*cp == '\t'))
			*cp = '\0';
		else
			break;
	}
	for (cp = bprm->buf+2; (*cp == ' ') || (*cp == '\t'); cp++);
	if (*cp == '\0')
		return -ENOEXEC; /* No interpreter name found */
	i_name = cp;
	i_arg = NULL;
	for ( ; *cp && (*cp != ' ') && (*cp != '\t'); cp++)
		/* nothing */ ;
	while ((*cp == ' ') || (*cp == '\t'))
		*cp++ = '\0';
	if (*cp)
		i_arg = cp;
=======
		if (!next_terminator(i_end, buf_end))
			return -ENOEXEC;
		i_end = buf_end;
	}
	/* Trim any trailing spaces/tabs from i_end */
	while (spacetab(i_end[-1]))
		i_end--;

	/* Skip over leading spaces/tabs */
	i_name = next_non_spacetab(bprm->buf+2, i_end);
	if (!i_name || (i_name == i_end))
		return -ENOEXEC; /* No interpreter name found */

	/* Is there an optional argument? */
	i_arg = NULL;
	i_sep = next_terminator(i_name, i_end);
	if (i_sep && (*i_sep != '\0'))
		i_arg = next_non_spacetab(i_sep, i_end);

	/*
	 * If the script filename will be inaccessible after exec, typically
	 * because it is a "/dev/fd/<fd>/.." path against an O_CLOEXEC fd, give
	 * up now (on the assumption that the interpreter will want to load
	 * this file).
	 */
	if (bprm->interp_flags & BINPRM_FLAGS_PATH_INACCESSIBLE)
		return -ENOENT;

>>>>>>> upstream/android-13
	/*
	 * OK, we've parsed out the interpreter name and
	 * (optional) argument.
	 * Splice in (1) the interpreter's name for argv[0]
	 *           (2) (optional) argument to interpreter
	 *           (3) filename of shell script (replace argv[0])
	 *
	 * This is done in reverse order, because of how the
	 * user environment and arguments are stored.
	 */
	retval = remove_arg_zero(bprm);
	if (retval)
		return retval;
<<<<<<< HEAD
	retval = copy_strings_kernel(1, &bprm->interp, bprm);
	if (retval < 0)
		return retval;
	bprm->argc++;
	if (i_arg) {
		retval = copy_strings_kernel(1, &i_arg, bprm);
=======
	retval = copy_string_kernel(bprm->interp, bprm);
	if (retval < 0)
		return retval;
	bprm->argc++;
	*((char *)i_end) = '\0';
	if (i_arg) {
		*((char *)i_sep) = '\0';
		retval = copy_string_kernel(i_arg, bprm);
>>>>>>> upstream/android-13
		if (retval < 0)
			return retval;
		bprm->argc++;
	}
<<<<<<< HEAD
	retval = copy_strings_kernel(1, &i_name, bprm);
=======
	retval = copy_string_kernel(i_name, bprm);
>>>>>>> upstream/android-13
	if (retval)
		return retval;
	bprm->argc++;
	retval = bprm_change_interp(i_name, bprm);
	if (retval < 0)
		return retval;

	/*
	 * OK, now restart the process with the interpreter's dentry.
	 */
	file = open_exec(i_name);
	if (IS_ERR(file))
		return PTR_ERR(file);

<<<<<<< HEAD
	bprm->file = file;
	retval = prepare_binprm(bprm);
	if (retval < 0)
		return retval;
	return search_binary_handler(bprm);
=======
	bprm->interpreter = file;
	return 0;
>>>>>>> upstream/android-13
}

static struct linux_binfmt script_format = {
	.module		= THIS_MODULE,
	.load_binary	= load_script,
};

static int __init init_script_binfmt(void)
{
	register_binfmt(&script_format);
	return 0;
}

static void __exit exit_script_binfmt(void)
{
	unregister_binfmt(&script_format);
}

core_initcall(init_script_binfmt);
module_exit(exit_script_binfmt);
MODULE_LICENSE("GPL");
