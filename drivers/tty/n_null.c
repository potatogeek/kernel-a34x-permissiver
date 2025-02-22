// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/module.h>

/*
 *  n_null.c - Null line discipline used in the failure path
 *
 *  Copyright (C) Intel 2017
 */

static int n_null_open(struct tty_struct *tty)
{
	return 0;
}

static void n_null_close(struct tty_struct *tty)
{
}

static ssize_t n_null_read(struct tty_struct *tty, struct file *file,
<<<<<<< HEAD
			   unsigned char __user * buf, size_t nr)
=======
			   unsigned char *buf, size_t nr,
			   void **cookie, unsigned long offset)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

static ssize_t n_null_write(struct tty_struct *tty, struct file *file,
			    const unsigned char *buf, size_t nr)
{
	return -EOPNOTSUPP;
}

static void n_null_receivebuf(struct tty_struct *tty,
<<<<<<< HEAD
				 const unsigned char *cp, char *fp,
=======
				 const unsigned char *cp, const char *fp,
>>>>>>> upstream/android-13
				 int cnt)
{
}

static struct tty_ldisc_ops null_ldisc = {
	.owner		=	THIS_MODULE,
<<<<<<< HEAD
	.magic		=	TTY_LDISC_MAGIC,
=======
	.num		=	N_NULL,
>>>>>>> upstream/android-13
	.name		=	"n_null",
	.open		=	n_null_open,
	.close		=	n_null_close,
	.read		=	n_null_read,
	.write		=	n_null_write,
	.receive_buf	=	n_null_receivebuf
};

static int __init n_null_init(void)
{
<<<<<<< HEAD
	BUG_ON(tty_register_ldisc(N_NULL, &null_ldisc));
=======
	BUG_ON(tty_register_ldisc(&null_ldisc));
>>>>>>> upstream/android-13
	return 0;
}

static void __exit n_null_exit(void)
{
<<<<<<< HEAD
	tty_unregister_ldisc(N_NULL);
=======
	tty_unregister_ldisc(&null_ldisc);
>>>>>>> upstream/android-13
}

module_init(n_null_init);
module_exit(n_null_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alan Cox");
MODULE_ALIAS_LDISC(N_NULL);
MODULE_DESCRIPTION("Null ldisc driver");
