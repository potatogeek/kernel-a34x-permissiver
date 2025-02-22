// SPDX-License-Identifier: GPL-2.0
/*
 * Implementation of the symbol table type.
 *
 * Author : Stephen Smalley, <sds@tycho.nsa.gov>
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include "symtab.h"

<<<<<<< HEAD
static unsigned int symhash(struct hashtab *h, const void *key)
=======
static unsigned int symhash(const void *key)
>>>>>>> upstream/android-13
{
	const char *p, *keyp;
	unsigned int size;
	unsigned int val;

	val = 0;
	keyp = key;
	size = strlen(keyp);
	for (p = keyp; (p - keyp) < size; p++)
		val = (val << 4 | (val >> (8*sizeof(unsigned int)-4))) ^ (*p);
<<<<<<< HEAD
	return val & (h->size - 1);
}

static int symcmp(struct hashtab *h, const void *key1, const void *key2)
=======
	return val;
}

static int symcmp(const void *key1, const void *key2)
>>>>>>> upstream/android-13
{
	const char *keyp1, *keyp2;

	keyp1 = key1;
	keyp2 = key2;
	return strcmp(keyp1, keyp2);
}

<<<<<<< HEAD

int symtab_init(struct symtab *s, unsigned int size)
{
	s->table = hashtab_create(symhash, symcmp, size);
	if (!s->table)
		return -ENOMEM;
	s->nprim = 0;
	return 0;
}

=======
static const struct hashtab_key_params symtab_key_params = {
	.hash = symhash,
	.cmp = symcmp,
};

int symtab_init(struct symtab *s, unsigned int size)
{
	s->nprim = 0;
	return hashtab_init(&s->table, size);
}

int symtab_insert(struct symtab *s, char *name, void *datum)
{
	return hashtab_insert(&s->table, name, datum, symtab_key_params);
}

void *symtab_search(struct symtab *s, const char *name)
{
	return hashtab_search(&s->table, name, symtab_key_params);
}
>>>>>>> upstream/android-13
