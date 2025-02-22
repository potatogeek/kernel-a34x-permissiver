// SPDX-License-Identifier: GPL-2.0
/*
 *    Copyright IBM Corp. 2017
 */

#include <linux/console.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/sclp.h>

static void sclp_early_write(struct console *con, const char *s, unsigned int len)
{
<<<<<<< HEAD
	__sclp_early_printk(s, len, 0);
=======
	__sclp_early_printk(s, len);
>>>>>>> upstream/android-13
}

static struct console sclp_early_console = {
	.name  = "earlysclp",
	.write = sclp_early_write,
	.flags = CON_PRINTBUFFER | CON_BOOT,
	.index = -1,
};

static int __init setup_early_printk(char *buf)
{
	if (early_console)
		return 0;
	/* Accept only "earlyprintk" and "earlyprintk=sclp" */
<<<<<<< HEAD
	if (buf && strncmp(buf, "sclp", 4))
=======
	if (buf && !str_has_prefix(buf, "sclp"))
>>>>>>> upstream/android-13
		return 0;
	if (!sclp.has_linemode && !sclp.has_vt220)
		return 0;
	early_console = &sclp_early_console;
	register_console(early_console);
	return 0;
}
early_param("earlyprintk", setup_early_printk);
