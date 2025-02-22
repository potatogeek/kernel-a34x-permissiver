/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NET_NF_TABLES_CORE_H
#define _NET_NF_TABLES_CORE_H

#include <net/netfilter/nf_tables.h>

extern struct nft_expr_type nft_imm_type;
extern struct nft_expr_type nft_cmp_type;
extern struct nft_expr_type nft_lookup_type;
extern struct nft_expr_type nft_bitwise_type;
extern struct nft_expr_type nft_byteorder_type;
extern struct nft_expr_type nft_payload_type;
extern struct nft_expr_type nft_dynset_type;
extern struct nft_expr_type nft_range_type;
extern struct nft_expr_type nft_meta_type;
extern struct nft_expr_type nft_rt_type;
extern struct nft_expr_type nft_exthdr_type;

int nf_tables_core_module_init(void);
void nf_tables_core_module_exit(void);

struct nft_cmp_fast_expr {
	u32			data;
	enum nft_registers	sreg:8;
	u8			len;
};

struct nft_immediate_expr {
	struct nft_data		data;
	enum nft_registers	dreg:8;
	u8			dlen;
};


static inline u32 nft_cmp_fast_mask(unsigned int len)
{
	return cpu_to_le32(~0U >> (FIELD_SIZEOF(struct nft_cmp_fast_expr,
						data) * BITS_PER_BYTE - len));
}

extern const struct nft_expr_ops nft_cmp_fast_ops;

struct nft_payload {
	enum nft_payload_bases	base:8;
	u8			offset;
	u8			len;
	enum nft_registers	dreg:8;
};

struct nft_payload_set {
	enum nft_payload_bases	base:8;
	u8			offset;
	u8			len;
	enum nft_registers	sreg:8;
	u8			csum_type;
	u8			csum_offset;
	u8			csum_flags;
};

extern const struct nft_expr_ops nft_payload_fast_ops;

extern struct static_key_false nft_counters_enabled;
extern struct static_key_false nft_trace_enabled;

extern struct nft_set_type nft_set_rhash_type;
extern struct nft_set_type nft_set_hash_type;
extern struct nft_set_type nft_set_hash_fast_type;
extern struct nft_set_type nft_set_rbtree_type;
extern struct nft_set_type nft_set_bitmap_type;

struct nft_expr;
struct nft_regs;
struct nft_pktinfo;
void nft_meta_get_eval(const struct nft_expr *expr,
		       struct nft_regs *regs, const struct nft_pktinfo *pkt);
void nft_lookup_eval(const struct nft_expr *expr,
		     struct nft_regs *regs, const struct nft_pktinfo *pkt);
#endif 
