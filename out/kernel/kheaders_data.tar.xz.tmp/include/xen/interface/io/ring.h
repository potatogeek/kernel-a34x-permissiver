/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __XEN_PUBLIC_IO_RING_H__
#define __XEN_PUBLIC_IO_RING_H__

#include <xen/interface/grant_table.h>

typedef unsigned int RING_IDX;


#define __RD2(_x)  (((_x) & 0x00000002) ? 0x2		       : ((_x) & 0x1))
#define __RD4(_x)  (((_x) & 0x0000000c) ? __RD2((_x)>>2)<<2    : __RD2(_x))
#define __RD8(_x)  (((_x) & 0x000000f0) ? __RD4((_x)>>4)<<4    : __RD4(_x))
#define __RD16(_x) (((_x) & 0x0000ff00) ? __RD8((_x)>>8)<<8    : __RD8(_x))
#define __RD32(_x) (((_x) & 0xffff0000) ? __RD16((_x)>>16)<<16 : __RD16(_x))


#define __CONST_RING_SIZE(_s, _sz)				\
	(__RD32(((_sz) - offsetof(struct _s##_sring, ring)) /	\
		sizeof(((struct _s##_sring *)0)->ring[0])))


#define __RING_SIZE(_s, _sz)						\
	(__RD32(((_sz) - (long)&(_s)->ring + (long)(_s)) / sizeof((_s)->ring[0])))



#define DEFINE_RING_TYPES(__name, __req_t, __rsp_t)			\
									\
							\
union __name##_sring_entry {						\
    __req_t req;							\
    __rsp_t rsp;							\
};									\
									\
							\
struct __name##_sring {							\
    RING_IDX req_prod, req_event;					\
    RING_IDX rsp_prod, rsp_event;					\
    uint8_t  pad[48];							\
    union __name##_sring_entry ring[1]; 		\
};									\
									\
					\
struct __name##_front_ring {						\
    RING_IDX req_prod_pvt;						\
    RING_IDX rsp_cons;							\
    unsigned int nr_ents;						\
    struct __name##_sring *sring;					\
};									\
									\
					\
struct __name##_back_ring {						\
    RING_IDX rsp_prod_pvt;						\
    RING_IDX req_cons;							\
    unsigned int nr_ents;						\
    struct __name##_sring *sring;					\
};




#define SHARED_RING_INIT(_s) do {					\
    (_s)->req_prod  = (_s)->rsp_prod  = 0;				\
    (_s)->req_event = (_s)->rsp_event = 1;				\
    memset((_s)->pad, 0, sizeof((_s)->pad));				\
} while(0)

#define FRONT_RING_INIT(_r, _s, __size) do {				\
    (_r)->req_prod_pvt = 0;						\
    (_r)->rsp_cons = 0;							\
    (_r)->nr_ents = __RING_SIZE(_s, __size);				\
    (_r)->sring = (_s);							\
} while (0)

#define BACK_RING_INIT(_r, _s, __size) do {				\
    (_r)->rsp_prod_pvt = 0;						\
    (_r)->req_cons = 0;							\
    (_r)->nr_ents = __RING_SIZE(_s, __size);				\
    (_r)->sring = (_s);							\
} while (0)


#define FRONT_RING_ATTACH(_r, _s, __size) do {				\
    (_r)->sring = (_s);							\
    (_r)->req_prod_pvt = (_s)->req_prod;				\
    (_r)->rsp_cons = (_s)->rsp_prod;					\
    (_r)->nr_ents = __RING_SIZE(_s, __size);				\
} while (0)

#define BACK_RING_ATTACH(_r, _s, __size) do {				\
    (_r)->sring = (_s);							\
    (_r)->rsp_prod_pvt = (_s)->rsp_prod;				\
    (_r)->req_cons = (_s)->req_prod;					\
    (_r)->nr_ents = __RING_SIZE(_s, __size);				\
} while (0)


#define RING_SIZE(_r)							\
    ((_r)->nr_ents)


#define RING_FREE_REQUESTS(_r)						\
    (RING_SIZE(_r) - ((_r)->req_prod_pvt - (_r)->rsp_cons))


#define RING_FULL(_r)							\
    (RING_FREE_REQUESTS(_r) == 0)


#define RING_HAS_UNCONSUMED_RESPONSES(_r)				\
    ((_r)->sring->rsp_prod - (_r)->rsp_cons)

#define RING_HAS_UNCONSUMED_REQUESTS(_r)				\
    ({									\
	unsigned int req = (_r)->sring->req_prod - (_r)->req_cons;	\
	unsigned int rsp = RING_SIZE(_r) -				\
			   ((_r)->req_cons - (_r)->rsp_prod_pvt);	\
	req < rsp ? req : rsp;						\
    })


#define RING_GET_REQUEST(_r, _idx)					\
    (&((_r)->sring->ring[((_idx) & (RING_SIZE(_r) - 1))].req))


#define RING_COPY_REQUEST(_r, _idx, _req) do {				\
				\
	*(_req) = *(volatile typeof(_req))RING_GET_REQUEST(_r, _idx);	\
} while (0)

#define RING_GET_RESPONSE(_r, _idx)					\
    (&((_r)->sring->ring[((_idx) & (RING_SIZE(_r) - 1))].rsp))


#define RING_REQUEST_CONS_OVERFLOW(_r, _cons)				\
    (((_cons) - (_r)->rsp_prod_pvt) >= RING_SIZE(_r))


#define RING_REQUEST_PROD_OVERFLOW(_r, _prod)               \
    (((_prod) - (_r)->rsp_prod_pvt) > RING_SIZE(_r))


#define RING_PUSH_REQUESTS(_r) do {					\
    virt_wmb(); 	\
    (_r)->sring->req_prod = (_r)->req_prod_pvt;				\
} while (0)

#define RING_PUSH_RESPONSES(_r) do {					\
    virt_wmb(); 	\
    (_r)->sring->rsp_prod = (_r)->rsp_prod_pvt;				\
} while (0)



#define RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(_r, _notify) do {		\
    RING_IDX __old = (_r)->sring->req_prod;				\
    RING_IDX __new = (_r)->req_prod_pvt;				\
    virt_wmb(); 	\
    (_r)->sring->req_prod = __new;					\
    virt_mb(); 	\
    (_notify) = ((RING_IDX)(__new - (_r)->sring->req_event) <		\
		 (RING_IDX)(__new - __old));				\
} while (0)

#define RING_PUSH_RESPONSES_AND_CHECK_NOTIFY(_r, _notify) do {		\
    RING_IDX __old = (_r)->sring->rsp_prod;				\
    RING_IDX __new = (_r)->rsp_prod_pvt;				\
    virt_wmb(); 	\
    (_r)->sring->rsp_prod = __new;					\
    virt_mb(); 	\
    (_notify) = ((RING_IDX)(__new - (_r)->sring->rsp_event) <		\
		 (RING_IDX)(__new - __old));				\
} while (0)

#define RING_FINAL_CHECK_FOR_REQUESTS(_r, _work_to_do) do {		\
    (_work_to_do) = RING_HAS_UNCONSUMED_REQUESTS(_r);			\
    if (_work_to_do) break;						\
    (_r)->sring->req_event = (_r)->req_cons + 1;			\
    virt_mb();								\
    (_work_to_do) = RING_HAS_UNCONSUMED_REQUESTS(_r);			\
} while (0)

#define RING_FINAL_CHECK_FOR_RESPONSES(_r, _work_to_do) do {		\
    (_work_to_do) = RING_HAS_UNCONSUMED_RESPONSES(_r);			\
    if (_work_to_do) break;						\
    (_r)->sring->rsp_event = (_r)->rsp_cons + 1;			\
    virt_mb();								\
    (_work_to_do) = RING_HAS_UNCONSUMED_RESPONSES(_r);			\
} while (0)




#ifndef XEN_PAGE_SHIFT

#define XEN_PAGE_SHIFT 12
#endif
#define XEN_FLEX_RING_SIZE(order)                                             \
    (1UL << ((order) + XEN_PAGE_SHIFT - 1))

#define DEFINE_XEN_FLEX_RING(name)                                            \
static inline RING_IDX name##_mask(RING_IDX idx, RING_IDX ring_size)          \
{                                                                             \
    return idx & (ring_size - 1);                                             \
}                                                                             \
                                                                              \
static inline unsigned char *name##_get_ring_ptr(unsigned char *buf,          \
                                                 RING_IDX idx,                \
                                                 RING_IDX ring_size)          \
{                                                                             \
    return buf + name##_mask(idx, ring_size);                                 \
}                                                                             \
                                                                              \
static inline void name##_read_packet(void *opaque,                           \
                                      const unsigned char *buf,               \
                                      size_t size,                            \
                                      RING_IDX masked_prod,                   \
                                      RING_IDX *masked_cons,                  \
                                      RING_IDX ring_size)                     \
{                                                                             \
    if (*masked_cons < masked_prod ||                                         \
        size <= ring_size - *masked_cons) {                                   \
        memcpy(opaque, buf + *masked_cons, size);                             \
    } else {                                                                  \
        memcpy(opaque, buf + *masked_cons, ring_size - *masked_cons);         \
        memcpy((unsigned char *)opaque + ring_size - *masked_cons, buf,       \
               size - (ring_size - *masked_cons));                            \
    }                                                                         \
    *masked_cons = name##_mask(*masked_cons + size, ring_size);               \
}                                                                             \
                                                                              \
static inline void name##_write_packet(unsigned char *buf,                    \
                                       const void *opaque,                    \
                                       size_t size,                           \
                                       RING_IDX *masked_prod,                 \
                                       RING_IDX masked_cons,                  \
                                       RING_IDX ring_size)                    \
{                                                                             \
    if (*masked_prod < masked_cons ||                                         \
        size <= ring_size - *masked_prod) {                                   \
        memcpy(buf + *masked_prod, opaque, size);                             \
    } else {                                                                  \
        memcpy(buf + *masked_prod, opaque, ring_size - *masked_prod);         \
        memcpy(buf, (unsigned char *)opaque + (ring_size - *masked_prod),     \
               size - (ring_size - *masked_prod));                            \
    }                                                                         \
    *masked_prod = name##_mask(*masked_prod + size, ring_size);               \
}                                                                             \
                                                                              \
static inline RING_IDX name##_queued(RING_IDX prod,                           \
                                     RING_IDX cons,                           \
                                     RING_IDX ring_size)                      \
{                                                                             \
    RING_IDX size;                                                            \
                                                                              \
    if (prod == cons)                                                         \
        return 0;                                                             \
                                                                              \
    prod = name##_mask(prod, ring_size);                                      \
    cons = name##_mask(cons, ring_size);                                      \
                                                                              \
    if (prod == cons)                                                         \
        return ring_size;                                                     \
                                                                              \
    if (prod > cons)                                                          \
        size = prod - cons;                                                   \
    else                                                                      \
        size = ring_size - (cons - prod);                                     \
    return size;                                                              \
}                                                                             \
                                                                              \
struct name##_data {                                                          \
    unsigned char *in;                            \
    unsigned char *out;                           \
}

#define DEFINE_XEN_FLEX_RING_AND_INTF(name)                                   \
struct name##_data_intf {                                                     \
    RING_IDX in_cons, in_prod;                                                \
                                                                              \
    uint8_t pad1[56];                                                         \
                                                                              \
    RING_IDX out_cons, out_prod;                                              \
                                                                              \
    uint8_t pad2[56];                                                         \
                                                                              \
    RING_IDX ring_order;                                                      \
    grant_ref_t ref[];                                                        \
};                                                                            \
DEFINE_XEN_FLEX_RING(name)

#endif 
