<<<<<<< HEAD
/*
 * Copyright (C) 2017 Josh Poimboeuf <jpoimboe@redhat.com>
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
 * Copyright (C) 2017 Josh Poimboeuf <jpoimboe@redhat.com>
>>>>>>> upstream/android-13
 */

#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
#include "orc.h"
#include "check.h"
#include "warn.h"

int create_orc(struct objtool_file *file)
{
	struct instruction *insn;

	for_each_insn(file, insn) {
		struct orc_entry *orc = &insn->orc;
		struct cfi_reg *cfa = &insn->state.cfa;
		struct cfi_reg *bp = &insn->state.regs[CFI_BP];

		orc->end = insn->state.end;

		if (cfa->base == CFI_UNDEFINED) {
			orc->sp_reg = ORC_REG_UNDEFINED;
			continue;
		}

		switch (cfa->base) {
		case CFI_SP:
			orc->sp_reg = ORC_REG_SP;
			break;
		case CFI_SP_INDIRECT:
			orc->sp_reg = ORC_REG_SP_INDIRECT;
			break;
		case CFI_BP:
			orc->sp_reg = ORC_REG_BP;
			break;
		case CFI_BP_INDIRECT:
			orc->sp_reg = ORC_REG_BP_INDIRECT;
			break;
		case CFI_R10:
			orc->sp_reg = ORC_REG_R10;
			break;
		case CFI_R13:
			orc->sp_reg = ORC_REG_R13;
			break;
		case CFI_DI:
			orc->sp_reg = ORC_REG_DI;
			break;
		case CFI_DX:
			orc->sp_reg = ORC_REG_DX;
			break;
		default:
			WARN_FUNC("unknown CFA base reg %d",
				  insn->sec, insn->offset, cfa->base);
			return -1;
		}

		switch(bp->base) {
		case CFI_UNDEFINED:
			orc->bp_reg = ORC_REG_UNDEFINED;
			break;
		case CFI_CFA:
			orc->bp_reg = ORC_REG_PREV_SP;
			break;
		case CFI_BP:
			orc->bp_reg = ORC_REG_BP;
			break;
		default:
			WARN_FUNC("unknown BP base reg %d",
				  insn->sec, insn->offset, bp->base);
			return -1;
		}

		orc->sp_offset = cfa->offset;
		orc->bp_offset = bp->offset;
		orc->type = insn->state.type;
	}

	return 0;
}

static int create_orc_entry(struct section *u_sec, struct section *ip_relasec,
				unsigned int idx, struct section *insn_sec,
				unsigned long insn_off, struct orc_entry *o)
{
	struct orc_entry *orc;
	struct rela *rela;

	/* populate ORC data */
	orc = (struct orc_entry *)u_sec->data->d_buf + idx;
	memcpy(orc, o, sizeof(*orc));

	/* populate rela for ip */
	rela = malloc(sizeof(*rela));
	if (!rela) {
		perror("malloc");
		return -1;
	}
	memset(rela, 0, sizeof(*rela));

	if (insn_sec->sym) {
		rela->sym = insn_sec->sym;
		rela->addend = insn_off;
	} else {
		/*
		 * The Clang assembler doesn't produce section symbols, so we
		 * have to reference the function symbol instead:
		 */
		rela->sym = find_symbol_containing(insn_sec, insn_off);
		if (!rela->sym) {
			/*
			 * Hack alert.  This happens when we need to reference
			 * the NOP pad insn immediately after the function.
			 */
			rela->sym = find_symbol_containing(insn_sec,
							   insn_off - 1);
		}
		if (!rela->sym) {
			WARN("missing symbol for insn at offset 0x%lx\n",
			     insn_off);
			return -1;
		}

		rela->addend = insn_off - rela->sym->offset;
	}

	rela->type = R_X86_64_PC32;
	rela->offset = idx * sizeof(int);

	list_add_tail(&rela->list, &ip_relasec->rela_list);
	hash_add(ip_relasec->rela_hash, &rela->hash, rela->offset);
=======
#include <linux/objtool.h>
#include <asm/orc_types.h>

#include <objtool/check.h>
#include <objtool/warn.h>
#include <objtool/endianness.h>

static int init_orc_entry(struct orc_entry *orc, struct cfi_state *cfi)
{
	struct instruction *insn = container_of(cfi, struct instruction, cfi);
	struct cfi_reg *bp = &cfi->regs[CFI_BP];

	memset(orc, 0, sizeof(*orc));

	orc->end = cfi->end;

	if (cfi->cfa.base == CFI_UNDEFINED) {
		orc->sp_reg = ORC_REG_UNDEFINED;
		return 0;
	}

	switch (cfi->cfa.base) {
	case CFI_SP:
		orc->sp_reg = ORC_REG_SP;
		break;
	case CFI_SP_INDIRECT:
		orc->sp_reg = ORC_REG_SP_INDIRECT;
		break;
	case CFI_BP:
		orc->sp_reg = ORC_REG_BP;
		break;
	case CFI_BP_INDIRECT:
		orc->sp_reg = ORC_REG_BP_INDIRECT;
		break;
	case CFI_R10:
		orc->sp_reg = ORC_REG_R10;
		break;
	case CFI_R13:
		orc->sp_reg = ORC_REG_R13;
		break;
	case CFI_DI:
		orc->sp_reg = ORC_REG_DI;
		break;
	case CFI_DX:
		orc->sp_reg = ORC_REG_DX;
		break;
	default:
		WARN_FUNC("unknown CFA base reg %d",
			  insn->sec, insn->offset, cfi->cfa.base);
		return -1;
	}

	switch (bp->base) {
	case CFI_UNDEFINED:
		orc->bp_reg = ORC_REG_UNDEFINED;
		break;
	case CFI_CFA:
		orc->bp_reg = ORC_REG_PREV_SP;
		break;
	case CFI_BP:
		orc->bp_reg = ORC_REG_BP;
		break;
	default:
		WARN_FUNC("unknown BP base reg %d",
			  insn->sec, insn->offset, bp->base);
		return -1;
	}

	orc->sp_offset = cfi->cfa.offset;
	orc->bp_offset = bp->offset;
	orc->type = cfi->type;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
int create_orc_sections(struct objtool_file *file)
{
	struct instruction *insn, *prev_insn;
	struct section *sec, *u_sec, *ip_relasec;
	unsigned int idx;

	struct orc_entry empty = {
		.sp_reg = ORC_REG_UNDEFINED,
		.bp_reg  = ORC_REG_UNDEFINED,
		.type    = ORC_TYPE_CALL,
	};

=======
static int write_orc_entry(struct elf *elf, struct section *orc_sec,
			   struct section *ip_sec, unsigned int idx,
			   struct section *insn_sec, unsigned long insn_off,
			   struct orc_entry *o)
{
	struct orc_entry *orc;

	/* populate ORC data */
	orc = (struct orc_entry *)orc_sec->data->d_buf + idx;
	memcpy(orc, o, sizeof(*orc));
	orc->sp_offset = bswap_if_needed(orc->sp_offset);
	orc->bp_offset = bswap_if_needed(orc->bp_offset);

	/* populate reloc for ip */
	if (elf_add_reloc_to_insn(elf, ip_sec, idx * sizeof(int), R_X86_64_PC32,
				  insn_sec, insn_off))
		return -1;

	return 0;
}

struct orc_list_entry {
	struct list_head list;
	struct orc_entry orc;
	struct section *insn_sec;
	unsigned long insn_off;
};

static int orc_list_add(struct list_head *orc_list, struct orc_entry *orc,
			struct section *sec, unsigned long offset)
{
	struct orc_list_entry *entry = malloc(sizeof(*entry));

	if (!entry) {
		WARN("malloc failed");
		return -1;
	}

	entry->orc	= *orc;
	entry->insn_sec = sec;
	entry->insn_off = offset;

	list_add_tail(&entry->list, orc_list);
	return 0;
}

static unsigned long alt_group_len(struct alt_group *alt_group)
{
	return alt_group->last_insn->offset +
	       alt_group->last_insn->len -
	       alt_group->first_insn->offset;
}

int orc_create(struct objtool_file *file)
{
	struct section *sec, *orc_sec;
	unsigned int nr = 0, idx = 0;
	struct orc_list_entry *entry;
	struct list_head orc_list;

	struct orc_entry null = {
		.sp_reg  = ORC_REG_UNDEFINED,
		.bp_reg  = ORC_REG_UNDEFINED,
		.type    = UNWIND_HINT_TYPE_CALL,
	};

	/* Build a deduplicated list of ORC entries: */
	INIT_LIST_HEAD(&orc_list);
	for_each_sec(file, sec) {
		struct orc_entry orc, prev_orc = {0};
		struct instruction *insn;
		bool empty = true;

		if (!sec->text)
			continue;

		sec_for_each_insn(file, sec, insn) {
			struct alt_group *alt_group = insn->alt_group;
			int i;

			if (!alt_group) {
				if (init_orc_entry(&orc, &insn->cfi))
					return -1;
				if (!memcmp(&prev_orc, &orc, sizeof(orc)))
					continue;
				if (orc_list_add(&orc_list, &orc, sec,
						 insn->offset))
					return -1;
				nr++;
				prev_orc = orc;
				empty = false;
				continue;
			}

			/*
			 * Alternatives can have different stack layout
			 * possibilities (but they shouldn't conflict).
			 * Instead of traversing the instructions, use the
			 * alt_group's flattened byte-offset-addressed CFI
			 * array.
			 */
			for (i = 0; i < alt_group_len(alt_group); i++) {
				struct cfi_state *cfi = alt_group->cfi[i];
				if (!cfi)
					continue;
				if (init_orc_entry(&orc, cfi))
					return -1;
				if (!memcmp(&prev_orc, &orc, sizeof(orc)))
					continue;
				if (orc_list_add(&orc_list, &orc, insn->sec,
						 insn->offset + i))
					return -1;
				nr++;
				prev_orc = orc;
				empty = false;
			}

			/* Skip to the end of the alt_group */
			insn = alt_group->last_insn;
		}

		/* Add a section terminator */
		if (!empty) {
			orc_list_add(&orc_list, &null, sec, sec->sh.sh_size);
			nr++;
		}
	}
	if (!nr)
		return 0;

	/* Create .orc_unwind, .orc_unwind_ip and .rela.orc_unwind_ip sections: */
>>>>>>> upstream/android-13
	sec = find_section_by_name(file->elf, ".orc_unwind");
	if (sec) {
		WARN("file already has .orc_unwind section, skipping");
		return -1;
	}
<<<<<<< HEAD

	/* count the number of needed orcs */
	idx = 0;
	for_each_sec(file, sec) {
		if (!sec->text)
			continue;

		prev_insn = NULL;
		sec_for_each_insn(file, sec, insn) {
			if (!prev_insn ||
			    memcmp(&insn->orc, &prev_insn->orc,
				   sizeof(struct orc_entry))) {
				idx++;
			}
			prev_insn = insn;
		}

		/* section terminator */
		if (prev_insn)
			idx++;
	}
	if (!idx)
		return -1;


	/* create .orc_unwind_ip and .rela.orc_unwind_ip sections */
	sec = elf_create_section(file->elf, ".orc_unwind_ip", sizeof(int), idx);
	if (!sec)
		return -1;

	ip_relasec = elf_create_rela_section(file->elf, sec);
	if (!ip_relasec)
		return -1;

	/* create .orc_unwind section */
	u_sec = elf_create_section(file->elf, ".orc_unwind",
				   sizeof(struct orc_entry), idx);

	/* populate sections */
	idx = 0;
	for_each_sec(file, sec) {
		if (!sec->text)
			continue;

		prev_insn = NULL;
		sec_for_each_insn(file, sec, insn) {
			if (!prev_insn || memcmp(&insn->orc, &prev_insn->orc,
						 sizeof(struct orc_entry))) {

				if (create_orc_entry(u_sec, ip_relasec, idx,
						     insn->sec, insn->offset,
						     &insn->orc))
					return -1;

				idx++;
			}
			prev_insn = insn;
		}

		/* section terminator */
		if (prev_insn) {
			if (create_orc_entry(u_sec, ip_relasec, idx,
					     prev_insn->sec,
					     prev_insn->offset + prev_insn->len,
					     &empty))
				return -1;

			idx++;
		}
	}

	if (elf_rebuild_rela_section(ip_relasec))
		return -1;

=======
	orc_sec = elf_create_section(file->elf, ".orc_unwind", 0,
				     sizeof(struct orc_entry), nr);
	if (!orc_sec)
		return -1;

	sec = elf_create_section(file->elf, ".orc_unwind_ip", 0, sizeof(int), nr);
	if (!sec)
		return -1;

	/* Write ORC entries to sections: */
	list_for_each_entry(entry, &orc_list, list) {
		if (write_orc_entry(file->elf, orc_sec, sec, idx++,
				    entry->insn_sec, entry->insn_off,
				    &entry->orc))
			return -1;
	}

>>>>>>> upstream/android-13
	return 0;
}
