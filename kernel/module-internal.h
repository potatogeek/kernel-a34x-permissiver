<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* Module internals
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/elf.h>
#include <asm/module.h>

struct load_info {
	const char *name;
	/* pointer to module in temporary copy, freed at end of load_module() */
	struct module *mod;
	Elf_Ehdr *hdr;
	unsigned long len;
	Elf_Shdr *sechdrs;
	char *secstrings, *strtab;
<<<<<<< HEAD
	unsigned long symoffs, stroffs;
=======
	unsigned long symoffs, stroffs, init_typeoffs, core_typeoffs;
>>>>>>> upstream/android-13
	struct _ddebug *debug;
	unsigned int num_debug;
	bool sig_ok;
#ifdef CONFIG_KALLSYMS
	unsigned long mod_kallsyms_init_off;
#endif
	struct {
		unsigned int sym, str, mod, vers, info, pcpu;
	} index;
};

extern int mod_verify_sig(const void *mod, struct load_info *info);
<<<<<<< HEAD
=======

#ifdef CONFIG_MODULE_SIG_PROTECT
extern bool gki_is_module_exported_symbol(const char *name);
extern bool gki_is_module_protected_symbol(const char *name);
#else
static inline bool gki_is_module_exported_symbol(const char *name)
{
	return 0;
}
static inline bool gki_is_module_protected_symbol(const char *name)
{
	return 0;
}
#endif /* CONFIG_MODULE_SIG_PROTECT */
>>>>>>> upstream/android-13
