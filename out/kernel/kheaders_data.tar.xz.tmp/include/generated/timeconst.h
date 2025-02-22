


#ifndef KERNEL_TIMECONST_H
#define KERNEL_TIMECONST_H

#include <linux/param.h>
#include <linux/types.h>

#if HZ != 250
#error "include/generated/timeconst.h has the wrong HZ value!"
#endif

#define HZ_TO_MSEC_MUL32	U64_C(0x80000000)
#define HZ_TO_MSEC_ADJ32	U64_C(0x0)
#define HZ_TO_MSEC_SHR32	29
#define MSEC_TO_HZ_MUL32	U64_C(0x80000000)
#define MSEC_TO_HZ_ADJ32	U64_C(0x180000000)
#define MSEC_TO_HZ_SHR32	33
#define HZ_TO_MSEC_NUM		4
#define HZ_TO_MSEC_DEN		1
#define MSEC_TO_HZ_NUM		1
#define MSEC_TO_HZ_DEN		4

#define HZ_TO_USEC_MUL32	U64_C(0xFA000000)
#define HZ_TO_USEC_ADJ32	U64_C(0x0)
#define HZ_TO_USEC_SHR32	20
#define USEC_TO_HZ_MUL32	U64_C(0x83126E98)
#define USEC_TO_HZ_ADJ32	U64_C(0x7FF7CED9168)
#define USEC_TO_HZ_SHR32	43
#define HZ_TO_USEC_NUM		4000
#define HZ_TO_USEC_DEN		1
#define USEC_TO_HZ_NUM		1
#define USEC_TO_HZ_DEN		4000
#define HZ_TO_NSEC_NUM		4000000
#define HZ_TO_NSEC_DEN		1
#define NSEC_TO_HZ_NUM		1
#define NSEC_TO_HZ_DEN		4000000

#endif 
