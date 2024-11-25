<<<<<<< HEAD
/*
 *sec_debug_extrainfo.c
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd
 *              http://www.samsung.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sec_debug.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/clock.h>
#include <asm/stacktrace.h>
#include <asm/esr.h>

#define SZ_960	0x000003c0
#define EXTRA_VERSION	"TE15"

struct sec_debug_panic_extra_info sec_debug_extra_info_init = {
	.item = {
		{"ID",		"", SZ_32},
		{"KTIME",	"", SZ_8},
		{"BIN",		"", SZ_16},
		{"FAULT",	"", SZ_32},
		{"BUG",		"", SZ_64},
		{"PANIC",	"", SZ_64},
		{"PC",		"", SZ_64},
		{"LR",		"", SZ_64},
		{"STACK",	"", SZ_256},
		{"RR",		"", SZ_8},
		{"RSTCNT", 	"", SZ_4},
		{"QBI",		"", SZ_16},
		{"DPM",		"", SZ_32},
		{"SMP",		"", SZ_8},
		{"ETC",		"", SZ_256},
		{"ESR",		"", SZ_64},
		{"KLG",		"", SZ_256},
		{"LEV",		"", SZ_4},

		/* extrb reset information */
		{"ID",		"", SZ_32},

		/* extrc reset information */
		{"ID",		"", SZ_32},

		/* extrm reset information */
		{"ID",		"", SZ_32},
		{"RR",		"", SZ_8},
		{"MFC",		"", SZ_960},
	}
};

struct sec_debug_panic_extra_info *sec_debug_extra_info;
struct sec_debug_panic_extra_info *sec_debug_extra_info_backup;
char *sec_debug_extra_info_buf;
/******************************************************************************
 * sec_debug_init_extra_info() - function to init extra info
 *
 * This function simply initialize each filed of sec_debug_panic_extra_info.
******************************************************************************/

void sec_debug_init_extra_info(struct sec_debug_shared_info *sec_debug_info,
				 phys_addr_t sec_extra_info_size, int magic_status)
{
	if (sec_debug_info) {
		pr_crit("%s: magic: %d\n", __func__, magic_status);
		sec_debug_extra_info = &sec_debug_info->sec_debug_extra_info;

		if (magic_status) {
			if (reset_reason == RR_K || reset_reason == RR_D || reset_reason == RR_P) {
				sec_debug_extra_info_backup = &sec_debug_info->sec_debug_extra_info_backup;
				sec_debug_extra_info_buf = (char *)sec_debug_info + sec_extra_info_size - SZ_1K;
				memcpy(sec_debug_extra_info_backup, sec_debug_extra_info,
						sizeof(struct sec_debug_panic_extra_info));
			}
		}

		if (sec_debug_extra_info)
			memcpy(sec_debug_extra_info, &sec_debug_extra_info_init,
					sizeof(sec_debug_extra_info_init));
	}
}

/******************************************************************************
 * sec_debug_set_extra_info() - function to set each extra info field
 *
 * This function simply set each filed of sec_debug_panic_extra_info.
******************************************************************************/

void sec_debug_set_extra_info(enum sec_debug_extra_buf_type type,
				const char *fmt, ...)
{
	va_list args;

	if (sec_debug_extra_info) {
		if (!strlen(sec_debug_extra_info->item[type].val)) {
			va_start(args, fmt);
			vsnprintf(sec_debug_extra_info->item[type].val,
					sec_debug_extra_info->item[type].max, fmt, args);
			va_end(args);
		}
	}
}

/******************************************************************************
 * sec_debug_store_extra_info - function to export extra info
 *
 * This function finally export the extra info to destination buffer.
 * The contents of buffer will be deliverd to framework at the next booting.
*****************************************************************************/

void sec_debug_store_extra_info(int start, int end)
{
	int i;
	int maxlen = MAX_EXTRA_INFO_KEY_LEN + MAX_EXTRA_INFO_VAL_LEN + 10;
	char *ptr = sec_debug_extra_info_buf;

	/* initialize extra info output buffer */
	memset((void *)ptr, 0, SZ_1K);

	if (!sec_debug_extra_info_backup)
		return;

	ptr += snprintf(ptr, maxlen, "\"%s\":\"%s\"", sec_debug_extra_info_backup->item[start].key,
		sec_debug_extra_info_backup->item[start].val);
		

	for (i = start + 1; i < end; i++) {
		if (ptr + strnlen(sec_debug_extra_info_backup->item[i].key, MAX_EXTRA_INFO_KEY_LEN)
			 + strnlen(sec_debug_extra_info_backup->item[i].val, MAX_EXTRA_INFO_VAL_LEN)
			 + MAX_EXTRA_INFO_HDR_LEN > sec_debug_extra_info_buf
			 + SZ_1K)
			break;

		ptr += snprintf(ptr, maxlen, ",\"%s\":\"%s\"",
			sec_debug_extra_info_backup->item[i].key,
			sec_debug_extra_info_backup->item[i].val);
	}
}

/******************************************************************************
 * sec_debug_store_extra_info_A
 ******************************************************************************/

void sec_debug_store_extra_info_A(void)
{
	sec_debug_store_extra_info(INFO_AID, INFO_MAX_A);
}

/******************************************************************************
 * sec_debug_store_extra_info_B
 ******************************************************************************/

void sec_debug_store_extra_info_B(void)
{
	sec_debug_store_extra_info(INFO_BID, INFO_MAX_B);
}

/******************************************************************************
 * sec_debug_store_extra_info_C
 ******************************************************************************/

void sec_debug_store_extra_info_C(void)
{
	sec_debug_store_extra_info(INFO_CID, INFO_MAX_C);
}

/******************************************************************************
 * sec_debug_store_extra_info_M
 ******************************************************************************/

void sec_debug_store_extra_info_M(void)
{
	sec_debug_store_extra_info(INFO_MID, INFO_MAX_M);
}

/******************************************************************************
 * sec_debug_set_extra_info_id
******************************************************************************/

static void sec_debug_set_extra_info_id(void)
{
	struct timespec ts;

	getnstimeofday(&ts);

	sec_debug_set_extra_info(INFO_AID, "%09lu%s", ts.tv_nsec, EXTRA_VERSION);
	sec_debug_set_extra_info(INFO_BID, "%09lu%s", ts.tv_nsec, EXTRA_VERSION);
	sec_debug_set_extra_info(INFO_CID, "%09lu%s", ts.tv_nsec, EXTRA_VERSION);
	sec_debug_set_extra_info(INFO_MID, "%09lu%s", ts.tv_nsec, EXTRA_VERSION);
}

/******************************************************************************
 * sec_debug_set_extra_info_ktime
******************************************************************************/

static void sec_debug_set_extra_info_ktime(void)
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * sec_debug_extra_info.c
 *
 * Copyright (c) 2019 Samsung Electronics Co., Ltd
 *              http://www.samsung.com
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched/clock.h>
#include <linux/kdebug.h>
#include <linux/notifier.h>
#include <linux/pm_qos.h>
#include <linux/panic_notifier.h>
#include <linux/sec_debug.h>
#include <asm/stacktrace.h>
#include <asm/esr.h>
#include <soc/samsung/exynos/debug-snapshot-log.h>

#include <trace/hooks/softlockup.h>
#include <trace/hooks/traps.h>
#include <trace/hooks/fault.h>
#include <trace/hooks/bug.h>
#include <trace/hooks/power.h>

#include "sec_debug_internal.h"
#include "sec_debug_extra_info_keys.c"

#define EXTRA_VERSION	"VE12"

#define MAX_EXTRA_INFO_HDR_LEN	6
#define MAX_CALL_ENTRY	128
#define ETR_A_PROC_SIZE SZ_2K

static bool exin_ready;
static struct sec_debug_shared_buffer *sh_buf;
static void *slot_end_addr;

static long __read_mostly rr_pwrsrc;
module_param(rr_pwrsrc, long, 0440);

/*****************************************************************/
/*                        UNIT FUNCTIONS                         */
/*****************************************************************/

static int is_exist_key(char (*keys)[MAX_ITEM_KEY_LEN], const char *key, int size)
{
	int mcnt = 0;
	int i;

	for (i = 0; i < size; i++)
		if (!strcmp(key, keys[i]))
			mcnt++;

	return mcnt;
}

static bool is_exist_abcfmt(const char *key)
{
	int match_cnt = 0;

	match_cnt += is_exist_key(akeys, key, ARRAY_SIZE(akeys));
	match_cnt += is_exist_key(bkeys, key, ARRAY_SIZE(bkeys));
	match_cnt += is_exist_key(ckeys, key, ARRAY_SIZE(ckeys));
	match_cnt += is_exist_key(fkeys, key, ARRAY_SIZE(fkeys));
	match_cnt += is_exist_key(mkeys, key, ARRAY_SIZE(mkeys));
	match_cnt += is_exist_key(tkeys, key, ARRAY_SIZE(tkeys));

	if (match_cnt == 0 || (match_cnt > 1 && strcmp(key, "ID") && strcmp(key, "RR"))) {
		pr_crit("%s: %s is exist in the abcfmt keys %d times\n", __func__, key, match_cnt);
#if !defined(CONFIG_SAMSUNG_PRODUCT_SHIP)
		panic("%s: key match error", __func__);
#endif
		return false;
	}

	return true;
}

static bool sec_debug_extra_info_check_key(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(key32); i++)
		if (!is_exist_abcfmt(key32[i]))
			return false;

	for  (i = 0; i < ARRAY_SIZE(key64); i++)
		if (!is_exist_abcfmt(key64[i]))
			return false;

	for (i = 0; i < ARRAY_SIZE(key256); i++)
		if (!is_exist_abcfmt(key256[i]))
			return false;

	for (i = 0; i < ARRAY_SIZE(key1024); i++)
		if (!is_exist_abcfmt(key1024[i]))
			return false;

	return true;
}

static int get_val_len(const char *s)
{
	if (s)
		return strnlen(s, MAX_ITEM_VAL_LEN);

	return 0;
}

static int get_key_len(const char *s)
{
	return strnlen(s, MAX_ITEM_KEY_LEN);
}

static void *get_slot_addr(int idx)
{
	return (void *)secdbg_base_get_ncva(sh_buf->sec_debug_sbidx[idx].paddr);
}

static int get_max_len(void *p)
{
	int i;

	if (p < get_slot_addr(SLOT_32) || (p >= slot_end_addr)) {
		pr_crit("%s: addr(%p) is not in range\n", __func__, p);

		return 0;
	}

	for (i = SLOT_32 + 1; i < NR_SLOT; i++)
		if (p < get_slot_addr(i))
			return sh_buf->sec_debug_sbidx[i - 1].size;

	return sh_buf->sec_debug_sbidx[SLOT_END].size;
}

static void *__get_item(int slot, unsigned int idx)
{
	void *p, *base;
	unsigned int size, nr;

	size = sh_buf->sec_debug_sbidx[slot].size;
	nr = sh_buf->sec_debug_sbidx[slot].nr;

	if (nr <= idx) {
		pr_crit("%s: SLOT %d has %d items (%d)\n",
					__func__, slot, nr, idx);

		return NULL;
	}

	base = get_slot_addr(slot);
	p = (void *)(base + (idx * size));

	return p;
}

static void *search_item_by_key(const char *key, int start, int end)
{
	int s, i, keylen = get_key_len(key);
	void *p;
	char *keyname;
	unsigned int max;

	if (!sh_buf || !exin_ready) {
		pr_info("%s: (%s) extra_info is not ready\n", __func__, key);

		return NULL;
	}

	for (s = start; s < end; s++) {
		if (sh_buf->sec_debug_sbidx[s].cnt)
			max = sh_buf->sec_debug_sbidx[s].cnt;
		else
			max = sh_buf->sec_debug_sbidx[s].nr;

		for (i = 0; i < max; i++) {
			p = __get_item(s, i);
			if (!p)
				break;

			keyname = (char *)p;
			if (keylen != get_key_len(keyname))
				continue;

			if (!strncmp(keyname, key, keylen))
				return p;
		}
	}

	return NULL;
}

static void *get_item(const char *key)
{
	return search_item_by_key(key, SLOT_32, NR_MAIN_SLOT);
}

static void *get_bk_item(const char *key)
{
	return search_item_by_key(key, SLOT_BK_32, NR_SLOT);
}

static char *get_item_val(const char *key)
{
	void *p;

	p = get_item(key);
	if (!p) {
		pr_crit("%s: no key %s\n", __func__, key);

		return NULL;
	}

	return ((char *)p + MAX_ITEM_KEY_LEN);
}

char *get_bk_item_val(const char *key)
{
	void *p;

	p = get_bk_item(key);
	if (!p)
		return NULL;

	return ((char *)p + MAX_ITEM_KEY_LEN);
}
EXPORT_SYMBOL(get_bk_item_val);

void get_bk_item_val_as_string(const char *key, char *buf)
{
	void *v;
	int len;

	v = get_bk_item_val(key);
	if (v) {
		len = get_val_len(v);
		if (len)
			memcpy(buf, v, len);
	}
}
EXPORT_SYMBOL(get_bk_item_val_as_string);

static int is_ocp;

static int is_key_in_blocklist(const char *key)
{
	char blkey[][MAX_ITEM_KEY_LEN] = {
		"KTIME", "BAT", "ODR", "DDRID",
		"PSITE", "ASB", "ASV", "IDS",
	};

	int nr_blkey, keylen, i;

	keylen = get_key_len(key);
	nr_blkey = ARRAY_SIZE(blkey);

	for (i = 0; i < nr_blkey; i++)
		if (!strncmp(key, blkey[i], keylen))
			return 1;

	if (!strncmp(key, "OCP", keylen)) {
		if (is_ocp)
			return 1;
		is_ocp = 1;
	}

	return 0;
}

static DEFINE_SPINLOCK(keyorder_lock);

static void set_key_order(const char *key)
{
	void *p;
	char *v;
	char tmp[MAX_ITEM_VAL_LEN] = {0, };
	int max = MAX_ITEM_VAL_LEN;
	int len_prev, len_remain, len_this;

	if (is_key_in_blocklist(key))
		return;

	spin_lock(&keyorder_lock);

	p = get_item("ODR");
	if (!p) {
		pr_crit("%s: fail to find %s\n", __func__, key);

		goto unlock_keyorder;
	}

	max = get_max_len(p);
	if (!max) {
		pr_crit("%s: fail to get max len %s\n", __func__, key);

		goto unlock_keyorder;
	}

	v = get_item_val(p);

	/* keep previous value */
	len_prev = get_val_len(v);
	if ((!len_prev) || (len_prev >= MAX_ITEM_VAL_LEN))
		len_prev = MAX_ITEM_VAL_LEN - 1;

	snprintf(tmp, len_prev + 1, "%s", v);

	/* calculate the remained size */
	len_remain = max;

	/* get_item_val returned address without key */
	len_remain -= MAX_ITEM_KEY_LEN;

	/* need comma */
	len_this = get_key_len(key) + 1;
	len_remain -= len_this;

	/* put last key at the first of ODR */
	/* +1 to add NULL (by snprintf) */
	snprintf(v, len_this + 1, "%s,", key);

	/* -1 to remove NULL between KEYS */
	/* +1 to add NULL (by snprintf) */
	snprintf((char *)(v + len_this), len_remain + 1, "%s", tmp);

unlock_keyorder:
	spin_unlock(&keyorder_lock);
}

static void set_item_val(const char *key, const char *fmt, ...)
{
	va_list args;
	void *p;
	char *v;
	int max = MAX_ITEM_VAL_LEN;

	p = get_item(key);
	if (!p) {
		pr_crit("%s: fail to find %s\n", __func__, key);

		return;
	}

	max = get_max_len(p);
	if (!max) {
		pr_crit("%s: fail to get max len %s\n", __func__, key);

		return;
	}

	v = get_item_val(p);
	if (!get_val_len(v)) {
		va_start(args, fmt);
		vsnprintf(v, max - MAX_ITEM_KEY_LEN, fmt, args);
		va_end(args);

		set_key_order(key);
	}
}

static void __init set_bk_item_val(const char *key, int slot, const char *fmt, ...)
{
	va_list args;
	void *p;
	char *v;
	int max = MAX_ITEM_VAL_LEN;
	unsigned int cnt;

	p = get_bk_item(key);
	if (!p) {
		pr_crit("%s: fail to find %s\n", __func__, key);

		if (slot > SLOT_MAIN_END) {
			cnt = sh_buf->sec_debug_sbidx[slot].cnt;
			sh_buf->sec_debug_sbidx[slot].cnt++;

			p = __get_item(slot, cnt);
			if (!p) {
				pr_crit("%s: slot%2d cnt: %d, fail\n", __func__, slot, cnt);

				return;
			}

			snprintf((char *)p, get_key_len(key) + 1, "%s", key);

			v = ((char *)p + MAX_ITEM_KEY_LEN);

			pr_crit("%s: add slot%2d cnt: %d (%s)\n", __func__, slot, cnt, (char *)p);

			goto set_val;
		}

		return;
	}

	max = get_max_len(p);
	if (!max) {
		pr_crit("%s: fail to get max len %s\n", __func__, key);

		if (slot > SLOT_MAIN_END) {
			max = MAX_ITEM_VAL_LEN;
		} else {
			pr_crit("%s: slot(%d) is not in bk slot\n", __func__, slot);

			return;
		}
	}

	v = get_bk_item_val(p);
	if (!v) {
		pr_crit("%s: fail to find value address for %s\n", __func__, key);
		return;
	}

	if (get_val_len(v)) {
		pr_crit("%s: some value is in %s\n", __func__, key);
		return;
	}

set_val:
	va_start(args, fmt);
	vsnprintf(v, max, fmt, args);
	va_end(args);
}

static void clear_item_val(const char *key)
{
	void *p;
	int max_len;

	p = get_item(key);
	if (!p) {
		pr_crit("%s: fail to find %s\n", __func__, key);

		return;
	}

	max_len = get_max_len(p);
	if (!max_len) {
		pr_crit("%s: fail to get max len %s\n", __func__, key);

		return;
	}

	memset(get_item_val(p), 0, max_len - MAX_ITEM_KEY_LEN);
}

#ifdef DEBUG
static void __init dump_all_keys(void)
{
	void *p;
	int s, i;
	unsigned int cnt;

	if (!exin_ready) {
		pr_crit("%s: EXIN is not ready\n", __func__);

		return;
	}

	for (s = 0; s < NR_SLOT; s++) {
		cnt = sh_buf->sec_debug_sbidx[s].cnt;

		for (i = 0; i < cnt; i++) {
			p = __get_item(s, i);
			if (!p) {
				pr_crit("%s: %d/%d: no item %p\n", __func__, s, i, p);
				break;
			}

			if (!get_key_len(p))
				break;

			pr_crit("%s: [%d][%02d] key %s\n",
							__func__, s, i, (char *)p);
		}
	}
}
#else
static void __init dump_all_keys(void) {}
#endif

static void __init init_shared_buffer(int type, int nr_keys, void *ptr)
{
	char (*keys)[MAX_ITEM_KEY_LEN];
	unsigned int base, size, nr;
	void *addr;
	int i;

	keys = (char (*)[MAX_ITEM_KEY_LEN])ptr;

	base = sh_buf->sec_debug_sbidx[type].paddr;
	size = sh_buf->sec_debug_sbidx[type].size;
	nr = sh_buf->sec_debug_sbidx[type].nr;

	addr = secdbg_base_get_ncva(base);
	memset(addr, 0, size * nr);

	pr_crit("%s: SLOT%d: nr keys: %d\n", __func__, type, nr_keys);

	for (i = 0; i < nr_keys; i++) {
		/* NULL is considered as +1 */
		snprintf((char *)addr, get_key_len(keys[i]) + 1, "%s", keys[i]);

		base += size;
		addr = secdbg_base_get_ncva(base);
	}

	sh_buf->sec_debug_sbidx[type].cnt = i;
}

static void __init sec_debug_extra_info_key_init(void)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(key32);
	init_shared_buffer(SLOT_32, nr_keys, (void *)key32);

	nr_keys = ARRAY_SIZE(key64);
	init_shared_buffer(SLOT_64, nr_keys, (void *)key64);

	nr_keys = ARRAY_SIZE(key256);
	init_shared_buffer(SLOT_256, nr_keys, (void *)key256);

	nr_keys = ARRAY_SIZE(key1024);
	init_shared_buffer(SLOT_1024, nr_keys, (void *)key1024);
}

static void __init sec_debug_extra_info_copy_shared_buffer(bool mflag)
{
	int i;
	unsigned int total_size = 0, slot_base;
	char *backup_base;

	for (i = 0; i < NR_MAIN_SLOT; i++)
		total_size += sh_buf->sec_debug_sbidx[i].nr * sh_buf->sec_debug_sbidx[i].size;

	slot_base = sh_buf->sec_debug_sbidx[SLOT_32].paddr;

	backup_base = secdbg_base_get_ncva(slot_base + total_size);

	pr_info("%s: dst: %llx src: %llx (%x)\n",
				__func__, (u64)backup_base, (u64)secdbg_base_get_ncva(slot_base), total_size);
	memcpy(backup_base, secdbg_base_get_ncva(slot_base), total_size);

	/* backup shared buffer header info */
	memcpy(&(sh_buf->sec_debug_sbidx[SLOT_BK_32]),
				&(sh_buf->sec_debug_sbidx[SLOT_32]),
				sizeof(struct sec_debug_sb_index) * (NR_SLOT - NR_MAIN_SLOT));

	for (i = SLOT_BK_32; i < NR_SLOT; i++) {
		sh_buf->sec_debug_sbidx[i].paddr += total_size;
		pr_debug("%s: SLOT %2d: paddr: 0x%x\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].paddr);
		pr_debug("%s: SLOT %2d: size: %d\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].size);
		pr_debug("%s: SLOT %2d: nr: %d\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].nr);
		pr_debug("%s: SLOT %2d: cnt: %d\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].cnt);
	}
}

static void __init sec_debug_extra_info_dump_sb_index(void)
{
	int i;

	for (i = 0; i < NR_SLOT; i++) {
		pr_debug("%s: SLOT%02d: paddr: %x\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].paddr);
		pr_debug("%s: SLOT%02d: cnt: %d\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].cnt);
		pr_debug("%s: SLOT%02d: blmark: %lx\n",
				__func__, i, sh_buf->sec_debug_sbidx[i].blmark);
		pr_debug("\n");
	}
}

static void __init sec_debug_init_extra_info_sbidx(int type, struct sec_debug_sb_index idx, bool mflag)
{
	sh_buf->sec_debug_sbidx[type].paddr = idx.paddr;
	sh_buf->sec_debug_sbidx[type].size = idx.size;
	sh_buf->sec_debug_sbidx[type].nr = idx.nr;
	sh_buf->sec_debug_sbidx[type].cnt = idx.cnt;
	sh_buf->sec_debug_sbidx[type].blmark = 0;

	pr_crit("%s: slot: %d / paddr: 0x%x / size: %d / nr: %d\n",
					__func__, type,
					sh_buf->sec_debug_sbidx[type].paddr,
					sh_buf->sec_debug_sbidx[type].size,
					sh_buf->sec_debug_sbidx[type].nr);
}

static bool __init sec_debug_extra_info_check_magic(void)
{
	if (sh_buf->magic[0] != SEC_DEBUG_SHARED_MAGIC0)
		return false;

	if (sh_buf->magic[1] != SEC_DEBUG_SHARED_MAGIC1)
		return false;

	if (sh_buf->magic[2] != SEC_DEBUG_SHARED_MAGIC2)
		return false;

	return true;
}

static void sec_debug_store_extra_info(char (*keys)[MAX_ITEM_KEY_LEN], int nr_keys, char *ptr)
{
	int i;
	unsigned long len, max_len;
	void *p;
	char *v, *start_addr = ptr;
	int last_offset = 0, offset = 0;

	memset(ptr, 0, ETR_A_PROC_SIZE);

	for (i = 0; i < nr_keys; i++) {
		p = get_bk_item(keys[i]);
		if (!p) {
			pr_crit("%s: no key: %s\n", __func__, keys[i]);

			continue;
		}

		v = p + MAX_ITEM_KEY_LEN;

		/* get_key_len returns length of the key + 1 */
		len = (unsigned long)ptr + offset + strlen(p) + get_val_len(v)
				+ MAX_EXTRA_INFO_HDR_LEN;

		max_len = (unsigned long)start_addr + ETR_A_PROC_SIZE;

		if (len > max_len) {
			*(ptr + last_offset - 1) = '\0';
			pr_crit("%s: length overfolw: fail to write - %s:%s\n", __func__, (char *)p, v);
			break;
		}
		offset += scnprintf(ptr + offset, ETR_A_PROC_SIZE - offset, "\"%s\":\"%s\"", (char *)p, v);

		if ((i + 1) != nr_keys)
			offset += scnprintf(ptr + offset, ETR_A_PROC_SIZE - offset, ",");
		last_offset = offset;
	}

	pr_info("%s: %s\n", __func__, ptr);
}

/******************************************************************************
 * sec_debug_extra_info details
 *
 *	etr_a : basic reset information
 *	etr_b : basic reset information
 *	etr_c : hard-lockup information (callstack)
 *	etr_m : mfc error information
 *
 ******************************************************************************/
void secdbg_exin_get_extra_info_A(char *ptr)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(akeys);

	sec_debug_store_extra_info(akeys, nr_keys, ptr);
}
EXPORT_SYMBOL(secdbg_exin_get_extra_info_A);

void secdbg_exin_get_extra_info_B(char *ptr)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(bkeys);

	sec_debug_store_extra_info(bkeys, nr_keys, ptr);
}
EXPORT_SYMBOL(secdbg_exin_get_extra_info_B);

void secdbg_exin_get_extra_info_C(char *ptr)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(ckeys);

	sec_debug_store_extra_info(ckeys, nr_keys, ptr);
}
EXPORT_SYMBOL(secdbg_exin_get_extra_info_C);

void secdbg_exin_get_extra_info_M(char *ptr)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(mkeys);

	sec_debug_store_extra_info(mkeys, nr_keys, ptr);
}
EXPORT_SYMBOL(secdbg_exin_get_extra_info_M);

void secdbg_exin_get_extra_info_F(char *ptr)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(fkeys);

	sec_debug_store_extra_info(fkeys, nr_keys, ptr);
}
EXPORT_SYMBOL(secdbg_exin_get_extra_info_F);

void secdbg_exin_get_extra_info_T(char *ptr)
{
	int nr_keys;

	nr_keys = ARRAY_SIZE(tkeys);

	sec_debug_store_extra_info(tkeys, nr_keys, ptr);
}
EXPORT_SYMBOL(secdbg_exin_get_extra_info_T);

static void __init sec_debug_extra_info_buffer_init(void)
{
	unsigned long tmp_addr;
	struct sec_debug_sb_index tmp_idx;
	bool flag_valid = false;
	unsigned long item_size;

	flag_valid = sec_debug_extra_info_check_magic();

	sec_debug_extra_info_dump_sb_index();

	tmp_idx.cnt = 0;
	tmp_idx.blmark = 0;

	/* SLOT_32, 32B, 128 items */
	tmp_addr = secdbg_base_get_buf_base(SDN_MAP_EXTRA_INFO);
	tmp_idx.paddr = (unsigned int)tmp_addr;
	tmp_idx.size = 32;
	tmp_idx.nr = 128;
	sec_debug_init_extra_info_sbidx(SLOT_32, tmp_idx, flag_valid);

	/* SLOT_64, 64B, 128 items */
	tmp_addr += tmp_idx.size * tmp_idx.nr;
	tmp_idx.paddr = (unsigned int)tmp_addr;
	tmp_idx.size = 64;
	tmp_idx.nr = 128;
	sec_debug_init_extra_info_sbidx(SLOT_64, tmp_idx, flag_valid);

	/* SLOT_256, 256B, 64 items */
	tmp_addr += tmp_idx.size * tmp_idx.nr;
	tmp_idx.paddr = (unsigned int)tmp_addr;
	tmp_idx.size = 256;
	tmp_idx.nr = 64;
	sec_debug_init_extra_info_sbidx(SLOT_256, tmp_idx, flag_valid);

	/* SLOT_1024, 1024B, 32 items */
	tmp_addr += tmp_idx.size * tmp_idx.nr;
	tmp_idx.paddr = (unsigned int)tmp_addr;
	tmp_idx.size = 1024;
	tmp_idx.nr = 32;
	sec_debug_init_extra_info_sbidx(SLOT_1024, tmp_idx, flag_valid);

	/*items size = 1024B item start addr + 1024B item size - exin start addr*/
	item_size = tmp_addr + (tmp_idx.size * tmp_idx.nr) - secdbg_base_get_buf_base(SDN_MAP_EXTRA_INFO);

	if (secdbg_base_get_buf_size(SDN_MAP_EXTRA_INFO) / 2 < item_size) {
		pr_crit("%s: item size overflow: rsvd: %lu, item size: %lu\n",
			__func__, secdbg_base_get_buf_size(SDN_MAP_EXTRA_INFO) / 2, item_size);
#if !defined(CONFIG_SAMSUNG_PRODUCT_SHIP)
		panic("%s: size error", __func__);
#endif
	}

	/* backup shared buffer contents */
	sec_debug_extra_info_copy_shared_buffer(flag_valid);

	sec_debug_extra_info_key_init();

	dump_all_keys();

	slot_end_addr = (void *)secdbg_base_get_ncva(sh_buf->sec_debug_sbidx[SLOT_END].paddr +
				((phys_addr_t)(sh_buf->sec_debug_sbidx[SLOT_END].size) *
				 (phys_addr_t)(sh_buf->sec_debug_sbidx[SLOT_END].nr)));
}

static void __init sec_debug_set_extra_info_id(void)
{
	struct timespec64 ts;

	ktime_get_real_ts64(&ts);

	set_bk_item_val("ID", SLOT_BK_32, "%09lu%s", ts.tv_nsec, EXTRA_VERSION);
}

static void sec_debug_set_reset_reason_using_module_param(void)
{
	char rr_c[RR_C] = {'S', 'W', 'D', 'K', 'M', 'P', 'R', 'B', 'N', 'T', 'C'};
	int reset_reason = (rr_pwrsrc & 0xff00000000) >> 32;
	int offsrc = (rr_pwrsrc & 0x00ffff0000) >> 16;
	int onsrc = rr_pwrsrc & 0x000000ffff;

	if (!get_bk_item("RR") && reset_reason > 0)
		set_bk_item_val("RR", SLOT_BK_32, "%cP", rr_c[reset_reason-1]);
	if (!get_bk_item("PWR"))
		set_bk_item_val("PWR", SLOT_BK_64, " %02X %02X", (onsrc & 0xff00) >> 8, (onsrc & 0x00ff));
	if (!get_bk_item("PWROFF"))
		set_bk_item_val("PWROFF", SLOT_BK_64, " %02X %02X", (offsrc & 0xff00) >> 8, (offsrc & 0x00ff));
}

static void secdbg_exin_set_ktime(void)
>>>>>>> upstream/android-13
{
	u64 ts_nsec;

	ts_nsec = local_clock();
	do_div(ts_nsec, 1000000000);
<<<<<<< HEAD
	sec_debug_set_extra_info(INFO_KTIME, "%lu", (unsigned long)ts_nsec);
}

/******************************************************************************
 * sec_debug_set_extra_info_fault
******************************************************************************/

void sec_debug_set_extra_info_fault(unsigned long addr, struct pt_regs *regs)
{
	if (regs) {
		pr_crit("sec_debug_set_extra_info_fault = 0x%lx\n", addr);
		sec_debug_set_extra_info(INFO_FAULT, "0x%lx", addr);
		sec_debug_set_extra_info(INFO_PC, "%pS", regs->pc);
		sec_debug_set_extra_info(INFO_LR, "%pS",
					 compat_user_mode(regs) ?
					  regs->compat_lr : regs->regs[30]);
	}
}

/******************************************************************************
 * sec_debug_set_extra_info_bug
******************************************************************************/

void sec_debug_set_extra_info_bug(const char *file, unsigned int line)
{
	sec_debug_set_extra_info(INFO_BUG, "%s:%u", file, line);
}

void sec_debug_set_extra_info_bug_verbose(unsigned long bugaddr)
{
	sec_debug_set_extra_info(INFO_BUG, "%pS", (u64)bugaddr);
}

/******************************************************************************
 * sec_debug_set_extra_info_panic
******************************************************************************/

void sec_debug_set_extra_info_panic(char *str)
{
	if (unlikely(strstr(str, "\nPC is at")))
		strcpy(strstr(str, "\nPC is at"), "");

	sec_debug_set_extra_info(INFO_PANIC, "%s", str);
}

/******************************************************************************
 * sec_debug_set_extra_info_backtrace
******************************************************************************/

void sec_debug_set_extra_info_backtrace(struct pt_regs *regs)
{
	char buf[64];
	struct stackframe frame;
	int offset = 0;
	int sym_name_len;

	pr_crit("sec_debug_store_backtrace\n");

	if (regs) {
		frame.fp = regs->regs[29];
		//frame.sp = regs->sp;
		frame.pc = regs->pc;
	} else {
		frame.fp = (unsigned long)__builtin_frame_address(0);
		//frame.sp = current_stack_pointer;
		frame.pc = (unsigned long)sec_debug_set_extra_info_backtrace;
	}

	while (1) {
		unsigned long where = frame.pc;
		int ret;

		ret = unwind_frame(NULL, &frame);
		if (ret < 0)
			break;

		snprintf(buf, sizeof(buf), "%pf", (void *)where);
		sym_name_len = strlen(buf);

		if (offset + sym_name_len > MAX_EXTRA_INFO_VAL_LEN)
			break;

		if (offset)
			offset += sprintf((char *)sec_debug_extra_info->item[INFO_STACK].val + offset, ":");

		sprintf((char *)sec_debug_extra_info->item[INFO_STACK].val + offset, "%s", buf);
		offset += sym_name_len;
	}
}

/******************************************************************************
 * sec_debug_set_extra_info_wdt_lastpc
******************************************************************************/

void sec_debug_set_extra_info_wdt_lastpc(unsigned long stackframe[][WDT_FRAME], unsigned int kick, unsigned int check)
{
	int cpu;
	char buf[64];
	int offset = 0;
	int sym_name_len;

	pr_crit("sec_debug_store_wdt_lastpc\n");

	for (cpu = 0; cpu < NR_CPUS; cpu++) {
		if ((check & (1 << cpu)) && !(kick & (1 << cpu))) {
			if (stackframe[cpu][0] != 0)
				snprintf(buf, sizeof(buf), "(%d)%pS", cpu, (void *)stackframe[cpu][0]);
			else
				snprintf(buf, sizeof(buf), "(%d)0", cpu);
			sym_name_len = strlen(buf);

			if (offset + sym_name_len > MAX_EXTRA_INFO_VAL_LEN)
				break;

			if (offset)
				offset += sprintf((char *)sec_debug_extra_info->item[INFO_KLG].val + offset, ":");
			else
				offset += sprintf((char *)sec_debug_extra_info->item[INFO_KLG].val + offset, "WDTPC:");

			sprintf((char *)sec_debug_extra_info->item[INFO_KLG].val + offset, "%s", buf);
			offset += sym_name_len;
		}
	}
}

/******************************************************************************
 * sec_debug_set_extra_info_smpl
******************************************************************************/

void sec_debug_set_extra_info_smpl(unsigned int count)
{
	sec_debug_set_extra_info(INFO_SMPL, "0x%x", count & 0x3ff);
}

/******************************************************************************
 * sec_debug_set_extra_info_zswap
******************************************************************************/

void sec_debug_set_extra_info_zswap(char *str)
{
	sec_debug_set_extra_info(INFO_ETC, "%s", str);
}

/******************************************************************************
 * sec_debug_set_extra_info_esr
******************************************************************************/

void sec_debug_set_extra_info_esr(unsigned int esr)
{
	sec_debug_set_extra_info(INFO_ESR, "%s (0x%08x)",
				esr_get_class_string(esr), esr);
}

void sec_debug_finish_extra_info(void)
{
	sec_debug_set_extra_info_ktime();
}

extern unsigned int sec_reset_cnt;
static int sec_debug_reset_rwc_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d", sec_reset_cnt);

	return 0;
}

static int sec_debug_reset_rwc_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, sec_debug_reset_rwc_proc_show, NULL);
}

static const struct file_operations sec_debug_reset_rwc_proc_fops = {
	.open = sec_debug_reset_rwc_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int set_debug_reset_extra_info_proc_show(struct seq_file *m, void *v)
{
	char buf[SZ_1K];

	if (!sec_debug_extra_info_buf)
		return -ENOENT;

	if (reset_reason == RR_K || reset_reason == RR_D || reset_reason == RR_P) {
		sec_debug_store_extra_info_A();
		memcpy(buf, sec_debug_extra_info_buf, SZ_1K);
		seq_printf(m, buf);
	} else {
		return -ENOENT;
	}
=======

	set_item_val("KTIME", "%lu", (unsigned long)ts_nsec);
}

void secdbg_exin_set_hwid(int asb_ver, int psite, const char *dramstr)
{
	set_item_val("ASB", "%d", asb_ver);
	set_item_val("PSITE", "%d", psite);

	if (dramstr)
		set_item_val("DDRID", "%s", dramstr);
}
EXPORT_SYMBOL(secdbg_exin_set_hwid);

void secdbg_exin_set_asv(int bg, int mg, int lg, int g3dg, int mifg)
{
	set_item_val("ASV", "%d-%d-%d-%d-%d", bg, mg, lg, g3dg, mifg);
}
EXPORT_SYMBOL(secdbg_exin_set_asv);

void secdbg_exin_set_ids(int bids, int mids, int lids, int gids)
{
	set_item_val("IDS", "%d-%d-%d-%d", bids, mids, lids, gids);
}
EXPORT_SYMBOL(secdbg_exin_set_ids);

void secdbg_exin_set_panic(const char *str)
{
	if (strstr(str, "\nPC is at"))
		strcpy(strstr(str, "\nPC is at"), "");

	set_item_val("PANIC", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_panic);

void secdbg_exin_set_sysmmu(const char *str)
{
	set_item_val("SMU", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_sysmmu);

void secdbg_exin_set_busmon(const char *str)
{
	set_item_val("BUS", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_busmon);

void secdbg_exin_set_smpl(unsigned long count)
{
	clear_item_val("SPCNT");
	set_item_val("SPCNT", "%lu", count);
}
EXPORT_SYMBOL(secdbg_exin_set_smpl);

void secdbg_exin_set_decon(const char *str)
{
	set_item_val("DCN", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_decon);

void secdbg_exin_set_batt(int cap, int volt, int temp, int curr)
{
	clear_item_val("BAT");
	set_item_val("BAT", "%03d/%04d/%04d/%06d", cap, volt, temp, curr);
}
EXPORT_SYMBOL(secdbg_exin_set_batt);

void secdbg_exin_set_finish(void)
{
	secdbg_exin_set_ktime();
}
EXPORT_SYMBOL(secdbg_exin_set_finish);

void secdbg_exin_set_mfc_error(const char *str)
{
	clear_item_val("STACK");
	set_item_val("STACK", "MFC ERROR");
	set_item_val("MFC", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_mfc_error);

void secdbg_exin_set_aud(const char *str)
{
	clear_item_val("AUD");
	set_item_val("AUD", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_aud);

void secdbg_exin_set_epd(const char *str)
{
	set_item_val("EPD", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_epd);

/* OCP total limitation */
#define MAX_OCP_CNT		(0xFF)

/* S2MPS23 */
#define S2MPS23_BUCK_CNT	(9)

/* S2MPS24 */
/* no irq in sub-pmic */

static int __add_pmic_irq_info(char *p, int max_buf_len, int *cnt, int nr)
{
	int i, tmp = 0, offset = 0;

	for (i = 0; i < nr; i++) {
		tmp = cnt[i];
		if (tmp > MAX_OCP_CNT)
			tmp = MAX_OCP_CNT;

		offset += scnprintf(p + offset, max_buf_len - offset, "%02x,", tmp);
	}

	/* to remove , in the end */
	if (nr != 0)
		p--;

	offset += scnprintf(p + offset, max_buf_len - offset, "/");

	return offset;
}

#define MOCP_SOCP_SIZE	SZ_256
void secdbg_exin_set_main_ocp(void *main_ocp_cnt, void *main_oi_cnt, int buck_cnt)
{
	char str_ocp[MOCP_SOCP_SIZE] = {0, };
	int offset = 0;

	offset += __add_pmic_irq_info(str_ocp, MOCP_SOCP_SIZE, main_ocp_cnt, buck_cnt);
	offset += __add_pmic_irq_info(str_ocp, MOCP_SOCP_SIZE - offset, main_oi_cnt, buck_cnt);

	if (offset >= MOCP_SOCP_SIZE)
		pr_crit("%s: length overflow\n", __func__);

	clear_item_val("MOCP");
	set_item_val("MOCP", "%s", str_ocp);
}
EXPORT_SYMBOL(secdbg_exin_set_main_ocp);

void secdbg_exin_set_sub_ocp(void)
{
#if 0 /* TODO: no irq in sub pmic (s2mps26) */
	char *p, str_ocp[SZ_64] = {0, };

	p = str_ocp;

	p = __add_pmic_irq_info(p, s2mps24_buck_ocp_cnt, S2MPS24_BUCK_CNT);
	p = __add_pmic_irq_info(p, s2mps24_buck_oi_cnt, S2MPS24_BUCK_OI_MAX);

	clear_item_val("SOCP");
	set_item_val("SOCP", "%s", str_ocp);
#endif
}
EXPORT_SYMBOL(secdbg_exin_set_sub_ocp);

void secdbg_exin_set_hardlockup_type(const char *fmt, ...)
{
	va_list args;
	char tmp[MAX_ITEM_VAL_LEN] = {0, };

	va_start(args, fmt);
	vsnprintf(tmp, MAX_ITEM_VAL_LEN, fmt, args);
	va_end(args);

	set_item_val("HLTYPE", "%s", tmp);
}
EXPORT_SYMBOL(secdbg_exin_set_hardlockup_type);

void secdbg_exin_set_hardlockup_data(const char *str)
{
	set_item_val("HLDATA", "%s", str);
}
EXPORT_SYMBOL(secdbg_exin_set_hardlockup_data);

void secdbg_exin_set_hardlockup_freq(const char *domain, struct freq_log *freq)
{
	void *p;
	char *v;
	char tmp[MAX_ITEM_VAL_LEN] = {0, };
	char freq_string[MAX_ITEM_VAL_LEN] = {0, };
	int offset = 0;

	p = get_item("HLFREQ");
	if (!p) {
		pr_crit("%s: fail to find\n", __func__);

		return;
	}

	if (!get_max_len(p)) {
		pr_crit("%s: fail to get max len\n", __func__);

		return;
	}

	v = get_item_val(p);

	offset = snprintf(freq_string, MAX_ITEM_VAL_LEN, "%s:%d>%d%c ",
		domain, freq->old_freq / 1000, freq->target_freq / 1000, (freq->en == 1) ? '+' : '-');

	snprintf(tmp, MAX_ITEM_VAL_LEN, "%s %s", v, freq_string);

	clear_item_val("HLFREQ");

	set_item_val("HLFREQ", "%s", tmp);
}
EXPORT_SYMBOL(secdbg_exin_set_hardlockup_freq);

void secdbg_exin_set_hardlockup_ehld(unsigned int hl_info, unsigned int cpu)
{
	int i;
	int offset = 0;
	char tmp[MAX_ITEM_VAL_LEN] = {0, };
	char tmp_per_cpu[MAX_ITEM_VAL_LEN] = {0, };

	for (i = 0; i < MAX_ETYPES; i++) {
		if ((hl_info & (1 << i)) != 0)
			offset += scnprintf(tmp_per_cpu + offset, MAX_ITEM_VAL_LEN - offset, "1");
		else
			offset += scnprintf(tmp_per_cpu + offset, MAX_ITEM_VAL_LEN - offset, "0");
	}

	snprintf(tmp, MAX_ITEM_VAL_LEN, "%s_%s", get_item_val("HLEHLD"), tmp_per_cpu);
	clear_item_val("HLEHLD");
	set_item_val("HLEHLD", "%s", tmp);
}
EXPORT_SYMBOL(secdbg_exin_set_hardlockup_ehld);

static int set_debug_reset_extra_info_proc_show(struct seq_file *m, void *v)
{
	char buf[ETR_A_PROC_SIZE];

	secdbg_exin_get_extra_info_A(buf);
	seq_printf(m, "%s", buf);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int sec_debug_reset_extra_info_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, set_debug_reset_extra_info_proc_show, NULL);
}

static const struct file_operations sec_debug_reset_extra_info_proc_fops = {
	.open = sec_debug_reset_extra_info_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init sec_debug_reset_extra_info_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create("reset_reason_extra_info",
			S_IWUGO, NULL, &sec_debug_reset_extra_info_proc_fops);
	
	if (!entry)
		return -ENOMEM;
	
	proc_set_size(entry, SZ_1K);

	entry = proc_create("reset_rwc", S_IWUGO, NULL,
						&sec_debug_reset_rwc_proc_fops);

	if (!entry)
		return -ENOMEM;

	sec_debug_set_extra_info_id();

	return 0;
}
device_initcall(sec_debug_reset_extra_info_init);
=======
void simulate_extra_info_force_error(unsigned int magic)
{
	if (!exin_ready) {
		pr_crit("%s: EXIN is not ready\n", __func__);
		return;
	}

	sh_buf->magic[0] = magic;
}
EXPORT_SYMBOL(simulate_extra_info_force_error);

static void secdbg_exin_set_fault(enum secdbg_exin_fault_type type,
				    unsigned long addr, struct pt_regs *regs)
{
	phys_addr_t paddr = 0;
	u64 lr;

	if (!regs)
		return;

	if (compat_user_mode(regs))
		lr = regs->compat_lr;
	else
		lr = regs->regs[30];

	set_item_val("FAULT", "0x%lx", addr);
	set_item_val("PC", "%pS", regs->pc);
	set_item_val("LR", "%pS",
			user_mode(regs) ? lr : ptrauth_strip_insn_pac(lr));

	if (type == UNDEF_FAULT && addr >= kimage_voffset) {
		paddr = virt_to_phys((void *)addr);

		pr_crit("%s: 0x%x / 0x%x\n", __func__,
			upper_32_bits(paddr), lower_32_bits(paddr));
//			exynos_pmu_write(EXYNOS_PMU_INFORM8, lower_32_bits(paddr));
//			exynos_pmu_write(EXYNOS_PMU_INFORM9, upper_32_bits(paddr));
	}
}

static void secdbg_exin_set_regs(struct pt_regs *regs)
{
	char fbuf[MAX_ITEM_VAL_LEN];
	int offset = 0, i;
	char *v;

	v = get_item_val("REGS");
	if (!v) {
		pr_crit("%s: no REGS in items\n", __func__);
		return;
	}

	if (get_val_len(v)) {
		pr_crit("%s: already %s in REGS\n", __func__, v);
		return;
	}

	memset(fbuf, 0, MAX_ITEM_VAL_LEN);

	pr_crit("%s: set regs\n", __func__);

	offset += scnprintf(fbuf + offset, MAX_ITEM_VAL_LEN - offset, "pc:%llx/", regs->pc);
	offset += scnprintf(fbuf + offset, MAX_ITEM_VAL_LEN - offset, "sp:%llx/", regs->sp);
	offset += scnprintf(fbuf + offset, MAX_ITEM_VAL_LEN - offset, "pstate:%llx/", regs->pstate);

	for (i = 0; i < 31; i++)
		offset += scnprintf(fbuf + offset, MAX_ITEM_VAL_LEN - offset, "x%d:%llx/", i, regs->regs[i]);

	set_item_val("REGS", fbuf);
}

static void secdbg_exin_set_stack(long *entries, int nr_entries)
{
	char fbuf[MAX_ITEM_VAL_LEN];
	char *v;
	unsigned int i;
	int offset = 0;

	v = get_item_val("STACK");

	if (!v) {
		pr_crit("%s: no STACK in items\n", __func__);
		return;
	}

	if (get_val_len(v)) {
		pr_crit("%s: already %s in STACK\n", __func__, v);
		return;
	}

	memset(fbuf, 0, MAX_ITEM_VAL_LEN);

	for (i = 0; i < nr_entries; i++)
		offset += scnprintf(fbuf + offset, MAX_ITEM_VAL_LEN - offset, "%ps:", (void *)entries[i]);

	set_item_val("STACK", fbuf);
}

static void secdbg_exin_set_backtrace(struct pt_regs *regs)
{
	unsigned long entry[MAX_CALL_ENTRY];
	unsigned int nr_entries = 0;

	if (!regs) {
		nr_entries = stack_trace_save(entry, ARRAY_SIZE(entry), 0);
	} else {
		nr_entries = stack_trace_save_regs(regs, entry, ARRAY_SIZE(entry), 1);
	}

	if (!nr_entries) {
		if (!regs)
			pr_err("no trace for current\n");
		else
			pr_err("no trace for [pc :%llx]\n", regs->pc);
		return;
	}

	if (regs)
		secdbg_exin_set_regs(regs);

	secdbg_exin_set_stack(entry, nr_entries);
}

static void secdbg_exin_set_backtrace_task(struct task_struct *tsk)
{
	unsigned long entry[MAX_CALL_ENTRY];
	unsigned int nr_entries = 0;

	if (!tsk) {
		nr_entries = stack_trace_save(entry, ARRAY_SIZE(entry), 0);
	} else {
		/* skipnr : skipping __switch_to */
		nr_entries = stack_trace_save_tsk(tsk, entry, ARRAY_SIZE(entry), 1);
	}

	if (!nr_entries) {
		if (!tsk)
			pr_err("no trace for current\n");
		else
			pr_err("no trace for [%s :%d]\n", tsk->comm, tsk->pid);
		return;
	}

	secdbg_exin_set_stack(entry, nr_entries);
}

static const char *esr_class_str[] = {
	[0 ... ESR_ELx_EC_MAX]		= "UNRECOGNIZED EC",
	[ESR_ELx_EC_UNKNOWN]		= "Unknown/Uncategorized",
	[ESR_ELx_EC_WFx]		= "WFI/WFE",
	[ESR_ELx_EC_CP15_32]		= "CP15 MCR/MRC",
	[ESR_ELx_EC_CP15_64]		= "CP15 MCRR/MRRC",
	[ESR_ELx_EC_CP14_MR]		= "CP14 MCR/MRC",
	[ESR_ELx_EC_CP14_LS]		= "CP14 LDC/STC",
	[ESR_ELx_EC_FP_ASIMD]		= "ASIMD",
	[ESR_ELx_EC_CP10_ID]		= "CP10 MRC/VMRS",
	[ESR_ELx_EC_PAC]		= "PAC",
	[ESR_ELx_EC_CP14_64]		= "CP14 MCRR/MRRC",
	[ESR_ELx_EC_BTI]		= "BTI",
	[ESR_ELx_EC_ILL]		= "PSTATE.IL",
	[ESR_ELx_EC_SVC32]		= "SVC (AArch32)",
	[ESR_ELx_EC_HVC32]		= "HVC (AArch32)",
	[ESR_ELx_EC_SMC32]		= "SMC (AArch32)",
	[ESR_ELx_EC_SVC64]		= "SVC (AArch64)",
	[ESR_ELx_EC_HVC64]		= "HVC (AArch64)",
	[ESR_ELx_EC_SMC64]		= "SMC (AArch64)",
	[ESR_ELx_EC_SYS64]		= "MSR/MRS (AArch64)",
	[ESR_ELx_EC_SVE]		= "SVE",
	[ESR_ELx_EC_ERET]		= "ERET/ERETAA/ERETAB",
	[ESR_ELx_EC_FPAC]		= "FPAC",
	[ESR_ELx_EC_IMP_DEF]		= "EL3 IMP DEF",
	[ESR_ELx_EC_IABT_LOW]		= "IABT (lower EL)",
	[ESR_ELx_EC_IABT_CUR]		= "IABT (current EL)",
	[ESR_ELx_EC_PC_ALIGN]		= "PC Alignment",
	[ESR_ELx_EC_DABT_LOW]		= "DABT (lower EL)",
	[ESR_ELx_EC_DABT_CUR]		= "DABT (current EL)",
	[ESR_ELx_EC_SP_ALIGN]		= "SP Alignment",
	[ESR_ELx_EC_FP_EXC32]		= "FP (AArch32)",
	[ESR_ELx_EC_FP_EXC64]		= "FP (AArch64)",
	[ESR_ELx_EC_SERROR]		= "SError",
	[ESR_ELx_EC_BREAKPT_LOW]	= "Breakpoint (lower EL)",
	[ESR_ELx_EC_BREAKPT_CUR]	= "Breakpoint (current EL)",
	[ESR_ELx_EC_SOFTSTP_LOW]	= "Software Step (lower EL)",
	[ESR_ELx_EC_SOFTSTP_CUR]	= "Software Step (current EL)",
	[ESR_ELx_EC_WATCHPT_LOW]	= "Watchpoint (lower EL)",
	[ESR_ELx_EC_WATCHPT_CUR]	= "Watchpoint (current EL)",
	[ESR_ELx_EC_BKPT32]		= "BKPT (AArch32)",
	[ESR_ELx_EC_VECTOR32]		= "Vector catch (AArch32)",
	[ESR_ELx_EC_BRK64]		= "BRK (AArch64)",
};

static void secdbg_exin_set_esr(unsigned int esr)
{
	set_item_val("ESR", "%s (0x%08x)", esr_class_str[ESR_ELx_EC(esr)], esr);
}

#define MAX_UNFZ_VAL_LEN (240)

void secdbg_exin_set_unfz(const char *comm, int pid)
{
	void *p;
	char *v;
	char tmp[MAX_UNFZ_VAL_LEN] = {0, };
	int max = MAX_UNFZ_VAL_LEN;
	int len_prev, len_remain, len_this = 0;

	p = get_item("UNFZ");
	if (!p) {
		pr_crit("%s: fail to find %s\n", __func__, comm);

		return;
	}

	max = get_max_len(p);
	if (!max) {
		pr_crit("%s: fail to get max len %s\n", __func__, comm);

		return;
	}

	v = get_item_val(p);

	/* keep previous value */
	len_prev = get_val_len(v);
	if ((!len_prev) || (len_prev >= MAX_UNFZ_VAL_LEN))
		len_prev = MAX_UNFZ_VAL_LEN - 1;

	snprintf(tmp, len_prev + 1, "%s", v);

	/* calculate the remained size */
	len_remain = max;

	/* get_item_val returned address without key */
	len_remain -= MAX_ITEM_KEY_LEN;

	/* put last key at the first of ODR */
	/* +1 to add NULL (by snprintf) */
	if (pid < 0)
		len_this = scnprintf((char *)(v + len_this), len_remain - len_this, "%s/", comm);
	else
		len_this = scnprintf((char *)(v + len_this), len_remain - len_this, "%s:%d/", comm, pid);

	/* -1 to remove NULL between KEYS */
	/* +1 to add NULL (by snprintf) */
	scnprintf((char *)(v + len_this), len_remain - len_this, "%s", tmp);
}
EXPORT_SYMBOL(secdbg_exin_set_unfz);

char *secdbg_exin_get_unfz(void)
{
	void *p;
	int max = MAX_UNFZ_VAL_LEN;

	p = get_item("UNFZ");
	if (!p) {
		pr_crit("%s: fail to find\n", __func__);

		return NULL;
	}

	max = get_max_len(p);
	if (!max) {
		pr_crit("%s: fail to get max len\n", __func__);

		return NULL;
	}

	return get_item_val(p);
}
EXPORT_SYMBOL(secdbg_exin_get_unfz);

static int secdbg_exin_panic_handler(struct notifier_block *nb,
				   unsigned long l, void *buf)
{
	secdbg_exin_set_panic(buf);
	secdbg_exin_set_backtrace(NULL);
	secdbg_exin_set_finish();

	return NOTIFY_DONE;
}

static struct notifier_block nb_panic_block = {
	.notifier_call = secdbg_exin_panic_handler,
};

static void secdbg_exin_set_bug(const char *file, unsigned int line)
{
	set_item_val("BUG", "%s:%u", file, line);
}

static void android_vh_watchdog_timer_softlockup(void *data,
		int duration, struct pt_regs *regs, bool is_panic)
{
	if (is_panic) {
		if (regs) {
			secdbg_exin_set_fault(WATCHDOG_FAULT, (unsigned long)regs->pc, regs);
			secdbg_exin_set_backtrace(regs);
		}
		panic("softlockup: hung tasks");
	}
}
static void android_rvh_do_undefinstr(void *data,
		struct pt_regs *regs)
{
	if (!user_mode(regs))
		secdbg_exin_set_fault(UNDEF_FAULT, (unsigned long)regs->pc, regs);
}

static void android_rvh_do_ptrauth_fault(void *data,
		struct pt_regs *regs, unsigned int esr)
{
	if (!user_mode(regs)) {
		secdbg_exin_set_fault(PTRAUTH_FAULT, (unsigned long)regs->pc, regs);
		secdbg_exin_set_esr(esr);
	}
}

static void android_rvh_bad_mode(void *data,
		struct pt_regs *regs, const char *vector, unsigned int esr)
{
	if (!user_mode(regs)) {
		secdbg_exin_set_fault(BAD_MODE_FAULT, (unsigned long)regs->pc, regs);
		secdbg_exin_set_esr(esr);
	}
}

static void android_rvh_arm64_serror_panic(void *data,
		struct pt_regs *regs, unsigned int esr)
{
	if (regs && !user_mode(regs)) {
		secdbg_exin_set_fault(SERROR_FAULT, (unsigned long)regs->pc, regs);
		secdbg_exin_set_esr(esr);
	}
}

static void android_rvh_die_kernel_fault(void *data,
		const char *msg, unsigned long addr, unsigned int esr, struct pt_regs *regs)
{
	secdbg_exin_set_fault(KERNEL_FAULT, addr, regs);
	secdbg_exin_set_esr(esr);
}

static void android_rvh_do_sea(void *data,
		unsigned long addr, unsigned int esr, struct pt_regs *regs)
{
	if (!user_mode(regs)) {
		secdbg_exin_set_fault(SEABORT_FAULT, addr, regs);
		secdbg_exin_set_esr(esr);
	}
}

static void android_rvh_do_mem_abort(void *data,
		unsigned long addr, unsigned int esr, struct pt_regs *regs)
{
	secdbg_exin_set_fault(MEM_ABORT_FAULT, addr, regs);
	secdbg_exin_set_esr(esr);
}

static void android_rvh_do_sp_pc_abort(void *data,
		unsigned long addr, unsigned int esr, struct pt_regs *regs)
{
	if (!user_mode(regs)) {
		secdbg_exin_set_fault(SP_PC_ABORT_FAULT, addr, regs);
		secdbg_exin_set_esr(esr);
	}
}

static bool is_bug_reported;
static void android_rvh_report_bug(void *data,
		const char *file, unsigned int line, unsigned long bugaddr)
{
	is_bug_reported = true;

	if (file)
		secdbg_exin_set_bug(file, line);
}

static void android_vh_try_to_freeze_todo_unfrozen(void *data,
		struct task_struct *p)
{
	secdbg_exin_set_backtrace_task(p);
	secdbg_exin_set_unfz(p->comm, p->pid);
}

static void android_vh_try_to_freeze_todo(void *data,
		unsigned int todo, unsigned int elapsed_msecs, bool wq_busy)
{
	const char *sys_state[SYSTEM_SUSPEND + 1] = {
		"BOOTING",
		"SCHEDULING",
		"RUNNING",
		"HALT",
		"POWER_OFF",
		"RESTART",
		"SUSPEND",
	};

	secdbg_exin_set_unfz(sys_state[system_state], -1);
	if (IS_ENABLED(CONFIG_SEC_DEBUG_FAIL_TO_FREEZE_PANIC))
		panic("fail to freeze tasks: %s", secdbg_exin_get_unfz());
}

static int secdbg_exin_die_handler(struct notifier_block *nb,
				   unsigned long l, void *buf)
{
	struct die_args *args = (struct die_args *)buf;
	struct pt_regs *regs = args->regs;

	if (regs && (!user_mode(regs)))
		secdbg_exin_set_backtrace(regs);

	if (is_bug_reported)
		secdbg_exin_set_fault(BUG_FAULT, (unsigned long)regs->pc, regs);

	return NOTIFY_DONE;
}

static struct notifier_block nb_die_block = {
	.notifier_call = secdbg_exin_die_handler,
	.priority = INT_MAX,
};

static void register_vendor_hooks(void)
{
	register_trace_android_vh_watchdog_timer_softlockup(android_vh_watchdog_timer_softlockup, NULL);
	register_trace_android_rvh_do_undefinstr(android_rvh_do_undefinstr, NULL);
	register_trace_android_rvh_do_ptrauth_fault(android_rvh_do_ptrauth_fault, NULL);
	register_trace_android_rvh_panic_unhandled(android_rvh_bad_mode, NULL);
	register_trace_android_rvh_arm64_serror_panic(android_rvh_arm64_serror_panic, NULL);
	register_trace_android_rvh_die_kernel_fault(android_rvh_die_kernel_fault, NULL);
	register_trace_android_rvh_do_sea(android_rvh_do_sea, NULL);
	register_trace_android_rvh_do_mem_abort(android_rvh_do_mem_abort, NULL);
	register_trace_android_rvh_do_sp_pc_abort(android_rvh_do_sp_pc_abort, NULL);
	register_trace_android_rvh_report_bug(android_rvh_report_bug, NULL);
	register_trace_android_vh_try_to_freeze_todo_unfrozen(android_vh_try_to_freeze_todo_unfrozen, NULL);
	register_trace_android_vh_try_to_freeze_todo(android_vh_try_to_freeze_todo, NULL);
	register_die_notifier(&nb_die_block);
}

static int __init secdbg_extra_info_init(void)
{
	struct proc_dir_entry *entry;

	pr_info("%s: start\n", __func__);

	if (!sec_debug_extra_info_check_key())
		pr_crit("%s: keys and abcfmt is not matched\n", __func__);

	sh_buf = secdbg_base_get_debug_base(SDN_MAP_EXTRA_INFO);
	if (!sh_buf) {
		pr_err("%s: No extra info buffer\n", __func__);
		return -EFAULT;
	}
	sec_debug_extra_info_buffer_init();

	sh_buf->magic[0] = SEC_DEBUG_SHARED_MAGIC0;
	sh_buf->magic[1] = SEC_DEBUG_SHARED_MAGIC1;
	sh_buf->magic[2] = SEC_DEBUG_SHARED_MAGIC2;
	sh_buf->magic[3] = SEC_DEBUG_SHARED_MAGIC3;

	exin_ready = true;

	entry = proc_create_single("reset_reason_extra_info",
				0644, NULL, set_debug_reset_extra_info_proc_show);
	if (!entry)
		return -ENOMEM;

	proc_set_size(entry, ETR_A_PROC_SIZE);

	sec_debug_set_extra_info_id();
	sec_debug_set_reset_reason_using_module_param();

	atomic_notifier_chain_register(&panic_notifier_list, &nb_panic_block);

	register_vendor_hooks();

	pr_info("%s: done\n", __func__);
	return 0;
}
module_init(secdbg_extra_info_init);

MODULE_DESCRIPTION("Samsung Debug Extra info driver");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
