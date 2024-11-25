<<<<<<< HEAD
/*
 * Copyright (C) 2015 Josh Poimboeuf <jpoimboe@redhat.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2015 Josh Poimboeuf <jpoimboe@redhat.com>
>>>>>>> upstream/android-13
 */

/*
 * This file reads all the special sections which have alternate instructions
 * which can be patched in or redirected to at runtime.
 */

#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
#include "special.h"
#include "warn.h"

#define EX_ENTRY_SIZE		12
#define EX_ORIG_OFFSET		0
#define EX_NEW_OFFSET		4

#define JUMP_ENTRY_SIZE		24
#define JUMP_ORIG_OFFSET	0
#define JUMP_NEW_OFFSET		8

#define ALT_ENTRY_SIZE		13
#define ALT_ORIG_OFFSET		0
#define ALT_NEW_OFFSET		4
#define ALT_FEATURE_OFFSET	8
#define ALT_ORIG_LEN_OFFSET	10
#define ALT_NEW_LEN_OFFSET	11

#define X86_FEATURE_POPCNT (4*32+23)
=======
#include <arch/special.h>
#include <objtool/builtin.h>
#include <objtool/special.h>
#include <objtool/warn.h>
#include <objtool/endianness.h>
>>>>>>> upstream/android-13

struct special_entry {
	const char *sec;
	bool group, jump_or_nop;
	unsigned char size, orig, new;
	unsigned char orig_len, new_len; /* group only */
	unsigned char feature; /* ALTERNATIVE macro CPU feature */
<<<<<<< HEAD
=======
	unsigned char key; /* jump_label key */
>>>>>>> upstream/android-13
};

struct special_entry entries[] = {
	{
		.sec = ".altinstructions",
		.group = true,
		.size = ALT_ENTRY_SIZE,
		.orig = ALT_ORIG_OFFSET,
		.orig_len = ALT_ORIG_LEN_OFFSET,
		.new = ALT_NEW_OFFSET,
		.new_len = ALT_NEW_LEN_OFFSET,
		.feature = ALT_FEATURE_OFFSET,
	},
	{
		.sec = "__jump_table",
		.jump_or_nop = true,
		.size = JUMP_ENTRY_SIZE,
		.orig = JUMP_ORIG_OFFSET,
		.new = JUMP_NEW_OFFSET,
<<<<<<< HEAD
=======
		.key = JUMP_KEY_OFFSET,
>>>>>>> upstream/android-13
	},
	{
		.sec = "__ex_table",
		.size = EX_ENTRY_SIZE,
		.orig = EX_ORIG_OFFSET,
		.new = EX_NEW_OFFSET,
	},
	{},
};

<<<<<<< HEAD
=======
void __weak arch_handle_alternative(unsigned short feature, struct special_alt *alt)
{
}

static void reloc_to_sec_off(struct reloc *reloc, struct section **sec,
			     unsigned long *off)
{
	*sec = reloc->sym->sec;
	*off = reloc->sym->offset + reloc->addend;
}

>>>>>>> upstream/android-13
static int get_alt_entry(struct elf *elf, struct special_entry *entry,
			 struct section *sec, int idx,
			 struct special_alt *alt)
{
<<<<<<< HEAD
	struct rela *orig_rela, *new_rela;
=======
	struct reloc *orig_reloc, *new_reloc;
>>>>>>> upstream/android-13
	unsigned long offset;

	offset = idx * entry->size;

	alt->group = entry->group;
	alt->jump_or_nop = entry->jump_or_nop;

	if (alt->group) {
		alt->orig_len = *(unsigned char *)(sec->data->d_buf + offset +
						   entry->orig_len);
		alt->new_len = *(unsigned char *)(sec->data->d_buf + offset +
						  entry->new_len);
	}

	if (entry->feature) {
		unsigned short feature;

<<<<<<< HEAD
		feature = *(unsigned short *)(sec->data->d_buf + offset +
					      entry->feature);

		/*
		 * It has been requested that we don't validate the !POPCNT
		 * feature path which is a "very very small percentage of
		 * machines".
		 */
		if (feature == X86_FEATURE_POPCNT)
			alt->skip_orig = true;
	}

	orig_rela = find_rela_by_dest(sec, offset + entry->orig);
	if (!orig_rela) {
		WARN_FUNC("can't find orig rela", sec, offset + entry->orig);
		return -1;
	}
	if (orig_rela->sym->type != STT_SECTION) {
		WARN_FUNC("don't know how to handle non-section rela symbol %s",
			   sec, offset + entry->orig, orig_rela->sym->name);
		return -1;
	}

	alt->orig_sec = orig_rela->sym->sec;
	alt->orig_off = orig_rela->addend;

	if (!entry->group || alt->new_len) {
		new_rela = find_rela_by_dest(sec, offset + entry->new);
		if (!new_rela) {
			WARN_FUNC("can't find new rela",
=======
		feature = bswap_if_needed(*(unsigned short *)(sec->data->d_buf +
							      offset +
							      entry->feature));
		arch_handle_alternative(feature, alt);
	}

	orig_reloc = find_reloc_by_dest(elf, sec, offset + entry->orig);
	if (!orig_reloc) {
		WARN_FUNC("can't find orig reloc", sec, offset + entry->orig);
		return -1;
	}

	reloc_to_sec_off(orig_reloc, &alt->orig_sec, &alt->orig_off);

	if (!entry->group || alt->new_len) {
		new_reloc = find_reloc_by_dest(elf, sec, offset + entry->new);
		if (!new_reloc) {
			WARN_FUNC("can't find new reloc",
>>>>>>> upstream/android-13
				  sec, offset + entry->new);
			return -1;
		}

<<<<<<< HEAD
		alt->new_sec = new_rela->sym->sec;
		alt->new_off = (unsigned int)new_rela->addend;
=======
		/*
		 * Skip retpoline .altinstr_replacement... we already rewrite the
		 * instructions for retpolines anyway, see arch_is_retpoline()
		 * usage in add_{call,jump}_destinations().
		 */
		if (arch_is_retpoline(new_reloc->sym))
			return 1;

		reloc_to_sec_off(new_reloc, &alt->new_sec, &alt->new_off);
>>>>>>> upstream/android-13

		/* _ASM_EXTABLE_EX hack */
		if (alt->new_off >= 0x7ffffff0)
			alt->new_off -= 0x7ffffff0;
	}

<<<<<<< HEAD
=======
	if (entry->key) {
		struct reloc *key_reloc;

		key_reloc = find_reloc_by_dest(elf, sec, offset + entry->key);
		if (!key_reloc) {
			WARN_FUNC("can't find key reloc",
				  sec, offset + entry->key);
			return -1;
		}
		alt->key_addend = key_reloc->addend;
	}

>>>>>>> upstream/android-13
	return 0;
}

/*
 * Read all the special sections and create a list of special_alt structs which
 * describe all the alternate instructions which can be patched in or
 * redirected to at runtime.
 */
int special_get_alts(struct elf *elf, struct list_head *alts)
{
	struct special_entry *entry;
	struct section *sec;
	unsigned int nr_entries;
	struct special_alt *alt;
	int idx, ret;

	INIT_LIST_HEAD(alts);

	for (entry = entries; entry->sec; entry++) {
		sec = find_section_by_name(elf, entry->sec);
		if (!sec)
			continue;

<<<<<<< HEAD
		if (sec->len % entry->size != 0) {
=======
		if (sec->sh.sh_size % entry->size != 0) {
>>>>>>> upstream/android-13
			WARN("%s size not a multiple of %d",
			     sec->name, entry->size);
			return -1;
		}

<<<<<<< HEAD
		nr_entries = sec->len / entry->size;
=======
		nr_entries = sec->sh.sh_size / entry->size;
>>>>>>> upstream/android-13

		for (idx = 0; idx < nr_entries; idx++) {
			alt = malloc(sizeof(*alt));
			if (!alt) {
				WARN("malloc failed");
				return -1;
			}
			memset(alt, 0, sizeof(*alt));

			ret = get_alt_entry(elf, entry, sec, idx, alt);
<<<<<<< HEAD
			if (ret)
=======
			if (ret > 0)
				continue;
			if (ret < 0)
>>>>>>> upstream/android-13
				return ret;

			list_add_tail(&alt->list, alts);
		}
	}

	return 0;
}
