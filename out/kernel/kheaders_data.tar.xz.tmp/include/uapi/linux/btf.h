/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef _UAPI__LINUX_BTF_H__
#define _UAPI__LINUX_BTF_H__

#include <linux/types.h>

#define BTF_MAGIC	0xeB9F
#define BTF_VERSION	1

struct btf_header {
	__u16	magic;
	__u8	version;
	__u8	flags;
	__u32	hdr_len;

	
	__u32	type_off;	
	__u32	type_len;	
	__u32	str_off;	
	__u32	str_len;	
};


#define BTF_MAX_TYPE	0x000fffff

#define BTF_MAX_NAME_OFFSET	0x00ffffff

#define BTF_MAX_VLEN	0xffff

struct btf_type {
	__u32 name_off;
	
	__u32 info;
	
	union {
		__u32 size;
		__u32 type;
	};
};

#define BTF_INFO_KIND(info)	(((info) >> 24) & 0x0f)
#define BTF_INFO_VLEN(info)	((info) & 0xffff)

#define BTF_KIND_UNKN		0	
#define BTF_KIND_INT		1	
#define BTF_KIND_PTR		2	
#define BTF_KIND_ARRAY		3	
#define BTF_KIND_STRUCT		4	
#define BTF_KIND_UNION		5	
#define BTF_KIND_ENUM		6	
#define BTF_KIND_FWD		7	
#define BTF_KIND_TYPEDEF	8	
#define BTF_KIND_VOLATILE	9	
#define BTF_KIND_CONST		10	
#define BTF_KIND_RESTRICT	11	
#define BTF_KIND_MAX		11
#define NR_BTF_KINDS		12




#define BTF_INT_ENCODING(VAL)	(((VAL) & 0x0f000000) >> 24)
#define BTF_INT_OFFSET(VAL)	(((VAL  & 0x00ff0000)) >> 16)
#define BTF_INT_BITS(VAL)	((VAL)  & 0x000000ff)


#define BTF_INT_SIGNED	(1 << 0)
#define BTF_INT_CHAR	(1 << 1)
#define BTF_INT_BOOL	(1 << 2)


struct btf_enum {
	__u32	name_off;
	__s32	val;
};


struct btf_array {
	__u32	type;
	__u32	index_type;
	__u32	nelems;
};


struct btf_member {
	__u32	name_off;
	__u32	type;
	__u32	offset;	
};

#endif 
