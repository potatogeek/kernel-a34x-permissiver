/* Generate assembler source containing symbol information
 *
 * Copyright 2002       by Kai Germaschewski
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * Usage: nm -n vmlinux | scripts/kallsyms [--all-symbols] > symbols.S
 *
 *      Table compression uses all the unused char codes on the symbols and
 *  maps these to the most used substrings (tokens). For instance, it might
 *  map char code 0xF7 to represent "write_" and then in every symbol where
 *  "write_" appears it can be replaced by 0xF7, saving 5 bytes.
 *      The used codes themselves are also placed in the table so that the
 *  decompresion can work without "special cases".
 *      Applied to kernel symbols, this usually produces a compression ratio
 *  of about 50%.
 *
 */

<<<<<<< HEAD
=======
#include <stdbool.h>
>>>>>>> upstream/android-13
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

<<<<<<< HEAD
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif
=======
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
>>>>>>> upstream/android-13

#define KSYM_NAME_LEN		128

struct sym_entry {
	unsigned long long addr;
	unsigned int len;
	unsigned int start_pos;
<<<<<<< HEAD
	unsigned char *sym;
	unsigned int percpu_absolute;
=======
	unsigned int percpu_absolute;
	unsigned char sym[];
>>>>>>> upstream/android-13
};

struct addr_range {
	const char *start_sym, *end_sym;
	unsigned long long start, end;
};

static unsigned long long _text;
static unsigned long long relative_base;
static struct addr_range text_ranges[] = {
	{ "_stext",     "_etext"     },
	{ "_sinittext", "_einittext" },
<<<<<<< HEAD
	{ "_stext_l1",  "_etext_l1"  },	/* Blackfin on-chip L1 inst SRAM */
	{ "_stext_l2",  "_etext_l2"  },	/* Blackfin on-chip L2 SRAM */
=======
>>>>>>> upstream/android-13
};
#define text_range_text     (&text_ranges[0])
#define text_range_inittext (&text_ranges[1])

static struct addr_range percpu_range = {
	"__per_cpu_start", "__per_cpu_end", -1ULL, 0
};

<<<<<<< HEAD
static struct sym_entry *table;
static unsigned int table_size, table_cnt;
static int all_symbols = 0;
static int absolute_percpu = 0;
static int base_relative = 0;

int token_profit[0x10000];

/* the table that holds the result of the compression */
unsigned char best_table[256][2];
unsigned char best_table_len[256];
=======
static struct sym_entry **table;
static unsigned int table_size, table_cnt;
static int all_symbols;
static int absolute_percpu;
static int base_relative;

static int token_profit[0x10000];

/* the table that holds the result of the compression */
static unsigned char best_table[256][2];
static unsigned char best_table_len[256];
>>>>>>> upstream/android-13


static void usage(void)
{
	fprintf(stderr, "Usage: kallsyms [--all-symbols] "
			"[--base-relative] < in.map > out.S\n");
	exit(1);
}

<<<<<<< HEAD
/*
 * This ignores the intensely annoying "mapping symbols" found
 * in ARM ELF files: $a, $t and $d.
 */
static inline int is_arm_mapping_symbol(const char *str)
{
	return str[0] == '$' && strchr("axtd", str[1])
	       && (str[2] == '\0' || str[2] == '.');
}

static int check_symbol_range(const char *sym, unsigned long long addr,
			      struct addr_range *ranges, int entries)
=======
static char *sym_name(const struct sym_entry *s)
{
	return (char *)s->sym + 1;
}

static bool is_ignored_symbol(const char *name, char type)
{
	/* Symbol names that exactly match to the following are ignored.*/
	static const char * const ignored_symbols[] = {
		/*
		 * Symbols which vary between passes. Passes 1 and 2 must have
		 * identical symbol lists. The kallsyms_* symbols below are
		 * only added after pass 1, they would be included in pass 2
		 * when --all-symbols is specified so exclude them to get a
		 * stable symbol list.
		 */
		"kallsyms_addresses",
		"kallsyms_offsets",
		"kallsyms_relative_base",
		"kallsyms_num_syms",
		"kallsyms_names",
		"kallsyms_markers",
		"kallsyms_token_table",
		"kallsyms_token_index",
		/* Exclude linker generated symbols which vary between passes */
		"_SDA_BASE_",		/* ppc */
		"_SDA2_BASE_",		/* ppc */
		NULL
	};

	/* Symbol names that begin with the following are ignored.*/
	static const char * const ignored_prefixes[] = {
		"$",			/* local symbols for ARM, MIPS, etc. */
		".LASANPC",		/* s390 kasan local symbols */
		"__crc_",		/* modversions */
		"__efistub_",		/* arm64 EFI stub namespace */
		"__kvm_nvhe_",		/* arm64 non-VHE KVM namespace */
		"__AArch64ADRPThunk_",	/* arm64 lld */
		"__ARMV5PILongThunk_",	/* arm lld */
		"__ARMV7PILongThunk_",
		"__ThumbV7PILongThunk_",
		"__LA25Thunk_",		/* mips lld */
		"__microLA25Thunk_",
		NULL
	};

	/* Symbol names that end with the following are ignored.*/
	static const char * const ignored_suffixes[] = {
		"_from_arm",		/* arm */
		"_from_thumb",		/* arm */
		"_veneer",		/* arm */
		NULL
	};

	/* Symbol names that contain the following are ignored.*/
	static const char * const ignored_matches[] = {
		".long_branch.",	/* ppc stub */
		".plt_branch.",		/* ppc stub */
		NULL
	};

	const char * const *p;

	for (p = ignored_symbols; *p; p++)
		if (!strcmp(name, *p))
			return true;

	for (p = ignored_prefixes; *p; p++)
		if (!strncmp(name, *p, strlen(*p)))
			return true;

	for (p = ignored_suffixes; *p; p++) {
		int l = strlen(name) - strlen(*p);

		if (l >= 0 && !strcmp(name + l, *p))
			return true;
	}

	for (p = ignored_matches; *p; p++) {
		if (strstr(name, *p))
			return true;
	}

	if (type == 'U' || type == 'u')
		return true;
	/* exclude debugging symbols */
	if (type == 'N' || type == 'n')
		return true;

	if (toupper(type) == 'A') {
		/* Keep these useful absolute symbols */
		if (strcmp(name, "__kernel_syscall_via_break") &&
		    strcmp(name, "__kernel_syscall_via_epc") &&
		    strcmp(name, "__kernel_sigtramp") &&
		    strcmp(name, "__gp"))
			return true;
	}

	return false;
}

static void check_symbol_range(const char *sym, unsigned long long addr,
			       struct addr_range *ranges, int entries)
>>>>>>> upstream/android-13
{
	size_t i;
	struct addr_range *ar;

	for (i = 0; i < entries; ++i) {
		ar = &ranges[i];

		if (strcmp(sym, ar->start_sym) == 0) {
			ar->start = addr;
<<<<<<< HEAD
			return 0;
		} else if (strcmp(sym, ar->end_sym) == 0) {
			ar->end = addr;
			return 0;
		}
	}

	return 1;
}

static int read_symbol(FILE *in, struct sym_entry *s)
{
	char sym[500], stype;
	int rc;

	rc = fscanf(in, "%llx %c %499s\n", &s->addr, &stype, sym);
	if (rc != 3) {
		if (rc != EOF && fgets(sym, 500, in) == NULL)
			fprintf(stderr, "Read error or end of file.\n");
		return -1;
	}
	if (strlen(sym) >= KSYM_NAME_LEN) {
		fprintf(stderr, "Symbol %s too long for kallsyms (%zu >= %d).\n"
				"Please increase KSYM_NAME_LEN both in kernel and kallsyms.c\n",
			sym, strlen(sym), KSYM_NAME_LEN);
		return -1;
	}

	/* Ignore most absolute/undefined (?) symbols. */
	if (strcmp(sym, "_text") == 0)
		_text = s->addr;
	else if (check_symbol_range(sym, s->addr, text_ranges,
				    ARRAY_SIZE(text_ranges)) == 0)
		/* nothing to do */;
	else if (toupper(stype) == 'A')
	{
		/* Keep these useful absolute symbols */
		if (strcmp(sym, "__kernel_syscall_via_break") &&
		    strcmp(sym, "__kernel_syscall_via_epc") &&
		    strcmp(sym, "__kernel_sigtramp") &&
		    strcmp(sym, "__gp"))
			return -1;

	}
	else if (toupper(stype) == 'U' ||
		 is_arm_mapping_symbol(sym))
		return -1;
	/* exclude also MIPS ELF local symbols ($L123 instead of .L123) */
	else if (sym[0] == '$')
		return -1;
	/* exclude debugging symbols */
	else if (stype == 'N' || stype == 'n')
		return -1;
	/* exclude s390 kasan local symbols */
	else if (!strncmp(sym, ".LASANPC", 8))
		return -1;

	/* include the type field in the symbol name, so that it gets
	 * compressed together */
	s->len = strlen(sym) + 1;
	s->sym = malloc(s->len + 1);
	if (!s->sym) {
=======
			return;
		} else if (strcmp(sym, ar->end_sym) == 0) {
			ar->end = addr;
			return;
		}
	}
}

static struct sym_entry *read_symbol(FILE *in)
{
	char name[500], type;
	unsigned long long addr;
	unsigned int len;
	struct sym_entry *sym;
	int rc;

	rc = fscanf(in, "%llx %c %499s\n", &addr, &type, name);
	if (rc != 3) {
		if (rc != EOF && fgets(name, 500, in) == NULL)
			fprintf(stderr, "Read error or end of file.\n");
		return NULL;
	}
	if (strlen(name) >= KSYM_NAME_LEN) {
		fprintf(stderr, "Symbol %s too long for kallsyms (%zu >= %d).\n"
				"Please increase KSYM_NAME_LEN both in kernel and kallsyms.c\n",
			name, strlen(name), KSYM_NAME_LEN);
		return NULL;
	}

	if (strcmp(name, "_text") == 0)
		_text = addr;

	/* Ignore most absolute/undefined (?) symbols. */
	if (is_ignored_symbol(name, type))
		return NULL;

	check_symbol_range(name, addr, text_ranges, ARRAY_SIZE(text_ranges));
	check_symbol_range(name, addr, &percpu_range, 1);

	/* include the type field in the symbol name, so that it gets
	 * compressed together */

	len = strlen(name) + 1;

	sym = malloc(sizeof(*sym) + len + 1);
	if (!sym) {
>>>>>>> upstream/android-13
		fprintf(stderr, "kallsyms failure: "
			"unable to allocate required amount of memory\n");
		exit(EXIT_FAILURE);
	}
<<<<<<< HEAD
	strcpy((char *)s->sym + 1, sym);
	s->sym[0] = stype;

	s->percpu_absolute = 0;

	/* Record if we've found __per_cpu_start/end. */
	check_symbol_range(sym, s->addr, &percpu_range, 1);

	return 0;
}

static int symbol_in_range(struct sym_entry *s, struct addr_range *ranges,
			   int entries)
{
	size_t i;
	struct addr_range *ar;
=======
	sym->addr = addr;
	sym->len = len;
	sym->sym[0] = type;
	strcpy(sym_name(sym), name);
	sym->percpu_absolute = 0;

	return sym;
}

static int symbol_in_range(const struct sym_entry *s,
			   const struct addr_range *ranges, int entries)
{
	size_t i;
	const struct addr_range *ar;
>>>>>>> upstream/android-13

	for (i = 0; i < entries; ++i) {
		ar = &ranges[i];

		if (s->addr >= ar->start && s->addr <= ar->end)
			return 1;
	}

	return 0;
}

<<<<<<< HEAD
static int symbol_valid(struct sym_entry *s)
{
	/* Symbols which vary between passes.  Passes 1 and 2 must have
	 * identical symbol lists.  The kallsyms_* symbols below are only added
	 * after pass 1, they would be included in pass 2 when --all-symbols is
	 * specified so exclude them to get a stable symbol list.
	 */
	static char *special_symbols[] = {
		"kallsyms_addresses",
		"kallsyms_offsets",
		"kallsyms_relative_base",
		"kallsyms_num_syms",
		"kallsyms_names",
		"kallsyms_markers",
		"kallsyms_token_table",
		"kallsyms_token_index",

	/* Exclude linker generated symbols which vary between passes */
		"_SDA_BASE_",		/* ppc */
		"_SDA2_BASE_",		/* ppc */
		NULL };

	static char *special_prefixes[] = {
		"__crc_",		/* modversions */
		"__efistub_",		/* arm64 EFI stub namespace */
		NULL };

	static char *special_suffixes[] = {
		"_veneer",		/* arm */
		"_from_arm",		/* arm */
		"_from_thumb",		/* arm */
		NULL };

	int i;
	char *sym_name = (char *)s->sym + 1;
=======
static int symbol_valid(const struct sym_entry *s)
{
	const char *name = sym_name(s);
>>>>>>> upstream/android-13

	/* if --all-symbols is not specified, then symbols outside the text
	 * and inittext sections are discarded */
	if (!all_symbols) {
		if (symbol_in_range(s, text_ranges,
				    ARRAY_SIZE(text_ranges)) == 0)
			return 0;
		/* Corner case.  Discard any symbols with the same value as
		 * _etext _einittext; they can move between pass 1 and 2 when
		 * the kallsyms data are added.  If these symbols move then
		 * they may get dropped in pass 2, which breaks the kallsyms
		 * rules.
		 */
		if ((s->addr == text_range_text->end &&
<<<<<<< HEAD
				strcmp(sym_name,
				       text_range_text->end_sym)) ||
		    (s->addr == text_range_inittext->end &&
				strcmp(sym_name,
				       text_range_inittext->end_sym)))
			return 0;
	}

	/* Exclude symbols which vary between passes. */
	for (i = 0; special_symbols[i]; i++)
		if (strcmp(sym_name, special_symbols[i]) == 0)
			return 0;

	for (i = 0; special_prefixes[i]; i++) {
		int l = strlen(special_prefixes[i]);

		if (l <= strlen(sym_name) &&
		    strncmp(sym_name, special_prefixes[i], l) == 0)
			return 0;
	}

	for (i = 0; special_suffixes[i]; i++) {
		int l = strlen(sym_name) - strlen(special_suffixes[i]);

		if (l >= 0 && strcmp(sym_name + l, special_suffixes[i]) == 0)
=======
		     strcmp(name, text_range_text->end_sym)) ||
		    (s->addr == text_range_inittext->end &&
		     strcmp(name, text_range_inittext->end_sym)))
>>>>>>> upstream/android-13
			return 0;
	}

	return 1;
}

<<<<<<< HEAD
static void read_map(FILE *in)
{
	while (!feof(in)) {
=======
/* remove all the invalid symbols from the table */
static void shrink_table(void)
{
	unsigned int i, pos;

	pos = 0;
	for (i = 0; i < table_cnt; i++) {
		if (symbol_valid(table[i])) {
			if (pos != i)
				table[pos] = table[i];
			pos++;
		} else {
			free(table[i]);
		}
	}
	table_cnt = pos;

	/* When valid symbol is not registered, exit to error */
	if (!table_cnt) {
		fprintf(stderr, "No valid symbol.\n");
		exit(1);
	}
}

static void read_map(FILE *in)
{
	struct sym_entry *sym;

	while (!feof(in)) {
		sym = read_symbol(in);
		if (!sym)
			continue;

		sym->start_pos = table_cnt;

>>>>>>> upstream/android-13
		if (table_cnt >= table_size) {
			table_size += 10000;
			table = realloc(table, sizeof(*table) * table_size);
			if (!table) {
				fprintf(stderr, "out of memory\n");
				exit (1);
			}
		}
<<<<<<< HEAD
		if (read_symbol(in, &table[table_cnt]) == 0) {
			table[table_cnt].start_pos = table_cnt;
			table_cnt++;
		}
	}
}

static void output_label(char *label)
=======

		table[table_cnt++] = sym;
	}
}

static void output_label(const char *label)
>>>>>>> upstream/android-13
{
	printf(".globl %s\n", label);
	printf("\tALGN\n");
	printf("%s:\n", label);
}

<<<<<<< HEAD
/* uncompress a compressed symbol. When this function is called, the best table
 * might still be compressed itself, so the function needs to be recursive */
static int expand_symbol(unsigned char *data, int len, char *result)
=======
/* Provide proper symbols relocatability by their '_text' relativeness. */
static void output_address(unsigned long long addr)
{
	if (_text <= addr)
		printf("\tPTR\t_text + %#llx\n", addr - _text);
	else
		printf("\tPTR\t_text - %#llx\n", _text - addr);
}

/* uncompress a compressed symbol. When this function is called, the best table
 * might still be compressed itself, so the function needs to be recursive */
static int expand_symbol(const unsigned char *data, int len, char *result)
>>>>>>> upstream/android-13
{
	int c, rlen, total=0;

	while (len) {
		c = *data;
		/* if the table holds a single char that is the same as the one
		 * we are looking for, then end the search */
		if (best_table[c][0]==c && best_table_len[c]==1) {
			*result++ = c;
			total++;
		} else {
			/* if not, recurse and expand */
			rlen = expand_symbol(best_table[c], best_table_len[c], result);
			total += rlen;
			result += rlen;
		}
		data++;
		len--;
	}
	*result=0;

	return total;
}

<<<<<<< HEAD
static int symbol_absolute(struct sym_entry *s)
=======
static int symbol_absolute(const struct sym_entry *s)
>>>>>>> upstream/android-13
{
	return s->percpu_absolute;
}

static void write_src(void)
{
	unsigned int i, k, off;
	unsigned int best_idx[256];
	unsigned int *markers;
	char buf[KSYM_NAME_LEN];

<<<<<<< HEAD
	printf("#include <asm/types.h>\n");
	printf("#if BITS_PER_LONG == 64\n");
	printf("#define PTR .quad\n");
	printf("#define ALGN .align 8\n");
	printf("#else\n");
	printf("#define PTR .long\n");
	printf("#define ALGN .align 4\n");
=======
	printf("#include <asm/bitsperlong.h>\n");
	printf("#if BITS_PER_LONG == 64\n");
	printf("#define PTR .quad\n");
	printf("#define ALGN .balign 8\n");
	printf("#else\n");
	printf("#define PTR .long\n");
	printf("#define ALGN .balign 4\n");
>>>>>>> upstream/android-13
	printf("#endif\n");

	printf("\t.section .rodata, \"a\"\n");

<<<<<<< HEAD
	/* Provide proper symbols relocatability by their relativeness
	 * to a fixed anchor point in the runtime image, either '_text'
	 * for absolute address tables, in which case the linker will
	 * emit the final addresses at build time. Otherwise, use the
	 * offset relative to the lowest value encountered of all relative
	 * symbols, and emit non-relocatable fixed offsets that will be fixed
	 * up at runtime.
	 *
	 * The symbol names cannot be used to construct normal symbol
	 * references as the list of symbols contains symbols that are
	 * declared static and are private to their .o files.  This prevents
	 * .tmp_kallsyms.o or any other object from referencing them.
	 */
=======
>>>>>>> upstream/android-13
	if (!base_relative)
		output_label("kallsyms_addresses");
	else
		output_label("kallsyms_offsets");

	for (i = 0; i < table_cnt; i++) {
		if (base_relative) {
<<<<<<< HEAD
=======
			/*
			 * Use the offset relative to the lowest value
			 * encountered of all relative symbols, and emit
			 * non-relocatable fixed offsets that will be fixed
			 * up at runtime.
			 */

>>>>>>> upstream/android-13
			long long offset;
			int overflow;

			if (!absolute_percpu) {
<<<<<<< HEAD
				offset = table[i].addr - relative_base;
				overflow = (offset < 0 || offset > UINT_MAX);
			} else if (symbol_absolute(&table[i])) {
				offset = table[i].addr;
				overflow = (offset < 0 || offset > INT_MAX);
			} else {
				offset = relative_base - table[i].addr - 1;
=======
				offset = table[i]->addr - relative_base;
				overflow = (offset < 0 || offset > UINT_MAX);
			} else if (symbol_absolute(table[i])) {
				offset = table[i]->addr;
				overflow = (offset < 0 || offset > INT_MAX);
			} else {
				offset = relative_base - table[i]->addr - 1;
>>>>>>> upstream/android-13
				overflow = (offset < INT_MIN || offset >= 0);
			}
			if (overflow) {
				fprintf(stderr, "kallsyms failure: "
					"%s symbol value %#llx out of range in relative mode\n",
<<<<<<< HEAD
					symbol_absolute(&table[i]) ? "absolute" : "relative",
					table[i].addr);
				exit(EXIT_FAILURE);
			}
			printf("\t.long\t%#x\n", (int)offset);
		} else if (!symbol_absolute(&table[i])) {
			if (_text <= table[i].addr)
				printf("\tPTR\t_text + %#llx\n",
					table[i].addr - _text);
			else
				printf("\tPTR\t_text - %#llx\n",
					_text - table[i].addr);
		} else {
			printf("\tPTR\t%#llx\n", table[i].addr);
=======
					symbol_absolute(table[i]) ? "absolute" : "relative",
					table[i]->addr);
				exit(EXIT_FAILURE);
			}
			printf("\t.long\t%#x\n", (int)offset);
		} else if (!symbol_absolute(table[i])) {
			output_address(table[i]->addr);
		} else {
			printf("\tPTR\t%#llx\n", table[i]->addr);
>>>>>>> upstream/android-13
		}
	}
	printf("\n");

	if (base_relative) {
		output_label("kallsyms_relative_base");
<<<<<<< HEAD
		printf("\tPTR\t_text - %#llx\n", _text - relative_base);
=======
		output_address(relative_base);
>>>>>>> upstream/android-13
		printf("\n");
	}

	output_label("kallsyms_num_syms");
<<<<<<< HEAD
	printf("\tPTR\t%u\n", table_cnt);
=======
	printf("\t.long\t%u\n", table_cnt);
>>>>>>> upstream/android-13
	printf("\n");

	/* table of offset markers, that give the offset in the compressed stream
	 * every 256 symbols */
	markers = malloc(sizeof(unsigned int) * ((table_cnt + 255) / 256));
	if (!markers) {
		fprintf(stderr, "kallsyms failure: "
			"unable to allocate required memory\n");
		exit(EXIT_FAILURE);
	}

	output_label("kallsyms_names");
	off = 0;
	for (i = 0; i < table_cnt; i++) {
		if ((i & 0xFF) == 0)
			markers[i >> 8] = off;

<<<<<<< HEAD
		printf("\t.byte 0x%02x", table[i].len);
		for (k = 0; k < table[i].len; k++)
			printf(", 0x%02x", table[i].sym[k]);
		printf("\n");

		off += table[i].len + 1;
=======
		printf("\t.byte 0x%02x", table[i]->len);
		for (k = 0; k < table[i]->len; k++)
			printf(", 0x%02x", table[i]->sym[k]);
		printf("\n");

		off += table[i]->len + 1;
>>>>>>> upstream/android-13
	}
	printf("\n");

	output_label("kallsyms_markers");
	for (i = 0; i < ((table_cnt + 255) >> 8); i++)
<<<<<<< HEAD
		printf("\tPTR\t%d\n", markers[i]);
=======
		printf("\t.long\t%u\n", markers[i]);
>>>>>>> upstream/android-13
	printf("\n");

	free(markers);

	output_label("kallsyms_token_table");
	off = 0;
	for (i = 0; i < 256; i++) {
		best_idx[i] = off;
		expand_symbol(best_table[i], best_table_len[i], buf);
		printf("\t.asciz\t\"%s\"\n", buf);
		off += strlen(buf) + 1;
	}
	printf("\n");

	output_label("kallsyms_token_index");
	for (i = 0; i < 256; i++)
		printf("\t.short\t%d\n", best_idx[i]);
	printf("\n");
}


/* table lookup compression functions */

/* count all the possible tokens in a symbol */
<<<<<<< HEAD
static void learn_symbol(unsigned char *symbol, int len)
=======
static void learn_symbol(const unsigned char *symbol, int len)
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < len - 1; i++)
		token_profit[ symbol[i] + (symbol[i + 1] << 8) ]++;
}

/* decrease the count for all the possible tokens in a symbol */
<<<<<<< HEAD
static void forget_symbol(unsigned char *symbol, int len)
=======
static void forget_symbol(const unsigned char *symbol, int len)
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < len - 1; i++)
		token_profit[ symbol[i] + (symbol[i + 1] << 8) ]--;
}

<<<<<<< HEAD
/* remove all the invalid symbols from the table and do the initial token count */
static void build_initial_tok_table(void)
{
	unsigned int i, pos;

	pos = 0;
	for (i = 0; i < table_cnt; i++) {
		if ( symbol_valid(&table[i]) ) {
			if (pos != i)
				table[pos] = table[i];
			learn_symbol(table[pos].sym, table[pos].len);
			pos++;
		} else {
			free(table[i].sym);
		}
	}
	table_cnt = pos;
}

static void *find_token(unsigned char *str, int len, unsigned char *token)
=======
/* do the initial token count */
static void build_initial_tok_table(void)
{
	unsigned int i;

	for (i = 0; i < table_cnt; i++)
		learn_symbol(table[i]->sym, table[i]->len);
}

static unsigned char *find_token(unsigned char *str, int len,
				 const unsigned char *token)
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < len - 1; i++) {
		if (str[i] == token[0] && str[i+1] == token[1])
			return &str[i];
	}
	return NULL;
}

/* replace a given token in all the valid symbols. Use the sampled symbols
 * to update the counts */
<<<<<<< HEAD
static void compress_symbols(unsigned char *str, int idx)
=======
static void compress_symbols(const unsigned char *str, int idx)
>>>>>>> upstream/android-13
{
	unsigned int i, len, size;
	unsigned char *p1, *p2;

	for (i = 0; i < table_cnt; i++) {

<<<<<<< HEAD
		len = table[i].len;
		p1 = table[i].sym;
=======
		len = table[i]->len;
		p1 = table[i]->sym;
>>>>>>> upstream/android-13

		/* find the token on the symbol */
		p2 = find_token(p1, len, str);
		if (!p2) continue;

		/* decrease the counts for this symbol's tokens */
<<<<<<< HEAD
		forget_symbol(table[i].sym, len);
=======
		forget_symbol(table[i]->sym, len);
>>>>>>> upstream/android-13

		size = len;

		do {
			*p2 = idx;
			p2++;
			size -= (p2 - p1);
			memmove(p2, p2 + 1, size);
			p1 = p2;
			len--;

			if (size < 2) break;

			/* find the token on the symbol */
			p2 = find_token(p1, size, str);

		} while (p2);

<<<<<<< HEAD
		table[i].len = len;

		/* increase the counts for this symbol's new tokens */
		learn_symbol(table[i].sym, len);
=======
		table[i]->len = len;

		/* increase the counts for this symbol's new tokens */
		learn_symbol(table[i]->sym, len);
>>>>>>> upstream/android-13
	}
}

/* search the token with the maximum profit */
static int find_best_token(void)
{
	int i, best, bestprofit;

	bestprofit=-10000;
	best = 0;

	for (i = 0; i < 0x10000; i++) {
		if (token_profit[i] > bestprofit) {
			best = i;
			bestprofit = token_profit[i];
		}
	}
	return best;
}

/* this is the core of the algorithm: calculate the "best" table */
static void optimize_result(void)
{
	int i, best;

	/* using the '\0' symbol last allows compress_symbols to use standard
	 * fast string functions */
	for (i = 255; i >= 0; i--) {

		/* if this table slot is empty (it is not used by an actual
		 * original char code */
		if (!best_table_len[i]) {

			/* find the token with the best profit value */
			best = find_best_token();
			if (token_profit[best] == 0)
				break;

			/* place it in the "best" table */
			best_table_len[i] = 2;
			best_table[i][0] = best & 0xFF;
			best_table[i][1] = (best >> 8) & 0xFF;

			/* replace this token in all the valid symbols */
			compress_symbols(best_table[i], i);
		}
	}
}

/* start by placing the symbols that are actually used on the table */
static void insert_real_symbols_in_table(void)
{
	unsigned int i, j, c;

<<<<<<< HEAD
	memset(best_table, 0, sizeof(best_table));
	memset(best_table_len, 0, sizeof(best_table_len));

	for (i = 0; i < table_cnt; i++) {
		for (j = 0; j < table[i].len; j++) {
			c = table[i].sym[j];
=======
	for (i = 0; i < table_cnt; i++) {
		for (j = 0; j < table[i]->len; j++) {
			c = table[i]->sym[j];
>>>>>>> upstream/android-13
			best_table[c][0]=c;
			best_table_len[c]=1;
		}
	}
}

static void optimize_token_table(void)
{
	build_initial_tok_table();

	insert_real_symbols_in_table();

<<<<<<< HEAD
	/* When valid symbol is not registered, exit to error */
	if (!table_cnt) {
		fprintf(stderr, "No valid symbol.\n");
		exit(1);
	}

=======
>>>>>>> upstream/android-13
	optimize_result();
}

/* guess for "linker script provide" symbol */
static int may_be_linker_script_provide_symbol(const struct sym_entry *se)
{
<<<<<<< HEAD
	const char *symbol = (char *)se->sym + 1;
=======
	const char *symbol = sym_name(se);
>>>>>>> upstream/android-13
	int len = se->len - 1;

	if (len < 8)
		return 0;

	if (symbol[0] != '_' || symbol[1] != '_')
		return 0;

	/* __start_XXXXX */
	if (!memcmp(symbol + 2, "start_", 6))
		return 1;

	/* __stop_XXXXX */
	if (!memcmp(symbol + 2, "stop_", 5))
		return 1;

	/* __end_XXXXX */
	if (!memcmp(symbol + 2, "end_", 4))
		return 1;

	/* __XXXXX_start */
	if (!memcmp(symbol + len - 6, "_start", 6))
		return 1;

	/* __XXXXX_end */
	if (!memcmp(symbol + len - 4, "_end", 4))
		return 1;

	return 0;
}

<<<<<<< HEAD
static int prefix_underscores_count(const char *str)
{
	const char *tail = str;

	while (*tail == '_')
		tail++;

	return tail - str;
}

static int compare_symbols(const void *a, const void *b)
{
	const struct sym_entry *sa;
	const struct sym_entry *sb;
	int wa, wb;

	sa = a;
	sb = b;

=======
static int compare_symbols(const void *a, const void *b)
{
	const struct sym_entry *sa = *(const struct sym_entry **)a;
	const struct sym_entry *sb = *(const struct sym_entry **)b;
	int wa, wb;

>>>>>>> upstream/android-13
	/* sort by address first */
	if (sa->addr > sb->addr)
		return 1;
	if (sa->addr < sb->addr)
		return -1;

	/* sort by "weakness" type */
	wa = (sa->sym[0] == 'w') || (sa->sym[0] == 'W');
	wb = (sb->sym[0] == 'w') || (sb->sym[0] == 'W');
	if (wa != wb)
		return wa - wb;

	/* sort by "linker script provide" type */
	wa = may_be_linker_script_provide_symbol(sa);
	wb = may_be_linker_script_provide_symbol(sb);
	if (wa != wb)
		return wa - wb;

	/* sort by the number of prefix underscores */
<<<<<<< HEAD
	wa = prefix_underscores_count((const char *)sa->sym + 1);
	wb = prefix_underscores_count((const char *)sb->sym + 1);
=======
	wa = strspn(sym_name(sa), "_");
	wb = strspn(sym_name(sb), "_");
>>>>>>> upstream/android-13
	if (wa != wb)
		return wa - wb;

	/* sort by initial order, so that other symbols are left undisturbed */
	return sa->start_pos - sb->start_pos;
}

static void sort_symbols(void)
{
<<<<<<< HEAD
	qsort(table, table_cnt, sizeof(struct sym_entry), compare_symbols);
=======
	qsort(table, table_cnt, sizeof(table[0]), compare_symbols);
>>>>>>> upstream/android-13
}

static void make_percpus_absolute(void)
{
	unsigned int i;

	for (i = 0; i < table_cnt; i++)
<<<<<<< HEAD
		if (symbol_in_range(&table[i], &percpu_range, 1)) {
=======
		if (symbol_in_range(table[i], &percpu_range, 1)) {
>>>>>>> upstream/android-13
			/*
			 * Keep the 'A' override for percpu symbols to
			 * ensure consistent behavior compared to older
			 * versions of this tool.
			 */
<<<<<<< HEAD
			table[i].sym[0] = 'A';
			table[i].percpu_absolute = 1;
=======
			table[i]->sym[0] = 'A';
			table[i]->percpu_absolute = 1;
>>>>>>> upstream/android-13
		}
}

/* find the minimum non-absolute symbol address */
static void record_relative_base(void)
{
	unsigned int i;

<<<<<<< HEAD
	relative_base = -1ULL;
	for (i = 0; i < table_cnt; i++)
		if (!symbol_absolute(&table[i]) &&
		    table[i].addr < relative_base)
			relative_base = table[i].addr;
=======
	for (i = 0; i < table_cnt; i++)
		if (!symbol_absolute(table[i])) {
			/*
			 * The table is sorted by address.
			 * Take the first non-absolute symbol value.
			 */
			relative_base = table[i]->addr;
			return;
		}
>>>>>>> upstream/android-13
}

int main(int argc, char **argv)
{
	if (argc >= 2) {
		int i;
		for (i = 1; i < argc; i++) {
			if(strcmp(argv[i], "--all-symbols") == 0)
				all_symbols = 1;
			else if (strcmp(argv[i], "--absolute-percpu") == 0)
				absolute_percpu = 1;
			else if (strcmp(argv[i], "--base-relative") == 0)
				base_relative = 1;
			else
				usage();
		}
	} else if (argc != 1)
		usage();

	read_map(stdin);
<<<<<<< HEAD
	if (absolute_percpu)
		make_percpus_absolute();
	if (base_relative)
		record_relative_base();
	sort_symbols();
=======
	shrink_table();
	if (absolute_percpu)
		make_percpus_absolute();
	sort_symbols();
	if (base_relative)
		record_relative_base();
>>>>>>> upstream/android-13
	optimize_token_table();
	write_src();

	return 0;
}
