/* SPDX-License-Identifier: GPL-2.0 */
/*
 * A symbol table (symtab) maintains associations between symbol
 * strings and datum values.  The type of the datum values
 * is arbitrary.  The symbol table type is implemented
 * using the hash table type (hashtab).
 *
 * Author : Stephen Smalley, <sds@tycho.nsa.gov>
 */
#ifndef _SS_SYMTAB_H_
#define _SS_SYMTAB_H_

#include "hashtab.h"

struct symtab {
<<<<<<< HEAD
	struct hashtab *table;	/* hash table (keyed on a string) */
=======
	struct hashtab table;	/* hash table (keyed on a string) */
>>>>>>> upstream/android-13
	u32 nprim;		/* number of primary names in table */
};

int symtab_init(struct symtab *s, unsigned int size);

<<<<<<< HEAD
=======
int symtab_insert(struct symtab *s, char *name, void *datum);
void *symtab_search(struct symtab *s, const char *name);

>>>>>>> upstream/android-13
#endif	/* _SS_SYMTAB_H_ */


