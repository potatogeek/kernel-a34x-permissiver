/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PGTABLE_NOPUD_H
#define _PGTABLE_NOPUD_H

#ifndef __ASSEMBLY__

<<<<<<< HEAD
#ifdef __ARCH_USE_5LEVEL_HACK
#include <asm-generic/pgtable-nop4d-hack.h>
#else
=======
>>>>>>> upstream/android-13
#include <asm-generic/pgtable-nop4d.h>

#define __PAGETABLE_PUD_FOLDED 1

/*
 * Having the pud type consist of a p4d gets the size right, and allows
 * us to conceptually access the p4d entry that this pud is folded into
 * without casting.
 */
typedef struct { p4d_t p4d; } pud_t;

#define PUD_SHIFT	P4D_SHIFT
#define PTRS_PER_PUD	1
#define PUD_SIZE  	(1UL << PUD_SHIFT)
#define PUD_MASK  	(~(PUD_SIZE-1))

/*
 * The "p4d_xxx()" functions here are trivial for a folded two-level
 * setup: the pud is never bad, and a pud always exists (as it's folded
 * into the p4d entry)
 */
static inline int p4d_none(p4d_t p4d)		{ return 0; }
static inline int p4d_bad(p4d_t p4d)		{ return 0; }
static inline int p4d_present(p4d_t p4d)	{ return 1; }
static inline void p4d_clear(p4d_t *p4d)	{ }
#define pud_ERROR(pud)				(p4d_ERROR((pud).p4d))

#define p4d_populate(mm, p4d, pud)		do { } while (0)
<<<<<<< HEAD
=======
#define p4d_populate_safe(mm, p4d, pud)		do { } while (0)
>>>>>>> upstream/android-13
/*
 * (puds are folded into p4ds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_p4d(p4dptr, p4dval)	set_pud((pud_t *)(p4dptr), (pud_t) { p4dval })

static inline pud_t *pud_offset(p4d_t *p4d, unsigned long address)
{
	return (pud_t *)p4d;
}
<<<<<<< HEAD
=======
#define pud_offset pud_offset
>>>>>>> upstream/android-13

#define pud_val(x)				(p4d_val((x).p4d))
#define __pud(x)				((pud_t) { __p4d(x) })

#define p4d_page(p4d)				(pud_page((pud_t){ p4d }))
<<<<<<< HEAD
#define p4d_page_vaddr(p4d)			(pud_page_vaddr((pud_t){ p4d }))
=======
#define p4d_pgtable(p4d)			((pud_t *)(pud_pgtable((pud_t){ p4d })))
>>>>>>> upstream/android-13

/*
 * allocating and freeing a pud is trivial: the 1-entry pud is
 * inside the p4d, so has no extra memory associated with it.
 */
#define pud_alloc_one(mm, address)		NULL
#define pud_free(mm, x)				do { } while (0)
<<<<<<< HEAD
#define __pud_free_tlb(tlb, x, a)		do { } while (0)
=======
#define pud_free_tlb(tlb, x, a)		        do { } while (0)
>>>>>>> upstream/android-13

#undef  pud_addr_end
#define pud_addr_end(addr, end)			(end)

#endif /* __ASSEMBLY__ */
<<<<<<< HEAD
#endif /* !__ARCH_USE_5LEVEL_HACK */
=======
>>>>>>> upstream/android-13
#endif /* _PGTABLE_NOPUD_H */
