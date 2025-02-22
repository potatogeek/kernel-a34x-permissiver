
#ifndef _ASM_GENERIC_ATOMIC64_H
#define _ASM_GENERIC_ATOMIC64_H
#include <linux/types.h>

typedef struct {
	long long counter;
} atomic64_t;

#define ATOMIC64_INIT(i)	{ (i) }

extern long long atomic64_read(const atomic64_t *v);
extern void	 atomic64_set(atomic64_t *v, long long i);

#define atomic64_set_release(v, i)	atomic64_set((v), (i))

#define ATOMIC64_OP(op)							\
extern void	 atomic64_##op(long long a, atomic64_t *v);

#define ATOMIC64_OP_RETURN(op)						\
extern long long atomic64_##op##_return(long long a, atomic64_t *v);

#define ATOMIC64_FETCH_OP(op)						\
extern long long atomic64_fetch_##op(long long a, atomic64_t *v);

#define ATOMIC64_OPS(op)	ATOMIC64_OP(op) ATOMIC64_OP_RETURN(op) ATOMIC64_FETCH_OP(op)

ATOMIC64_OPS(add)
ATOMIC64_OPS(sub)

#undef ATOMIC64_OPS
#define ATOMIC64_OPS(op)	ATOMIC64_OP(op) ATOMIC64_FETCH_OP(op)

ATOMIC64_OPS(and)
ATOMIC64_OPS(or)
ATOMIC64_OPS(xor)

#undef ATOMIC64_OPS
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_OP_RETURN
#undef ATOMIC64_OP

extern long long atomic64_dec_if_positive(atomic64_t *v);
#define atomic64_dec_if_positive atomic64_dec_if_positive
extern long long atomic64_cmpxchg(atomic64_t *v, long long o, long long n);
extern long long atomic64_xchg(atomic64_t *v, long long new);
extern long long atomic64_fetch_add_unless(atomic64_t *v, long long a, long long u);
#define atomic64_fetch_add_unless atomic64_fetch_add_unless

#endif  
