/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_STRING_HELPERS_H_
#define _LINUX_STRING_HELPERS_H_

<<<<<<< HEAD
=======
#include <linux/bits.h>
#include <linux/ctype.h>
>>>>>>> upstream/android-13
#include <linux/types.h>

struct file;
struct task_struct;

/* Descriptions of the types of units to
 * print in */
enum string_size_units {
	STRING_UNITS_10,	/* use powers of 10^3 (standard SI) */
	STRING_UNITS_2,		/* use binary powers of 2^10 */
};

void string_get_size(u64 size, u64 blk_size, enum string_size_units units,
		     char *buf, int len);

<<<<<<< HEAD
#define UNESCAPE_SPACE		0x01
#define UNESCAPE_OCTAL		0x02
#define UNESCAPE_HEX		0x04
#define UNESCAPE_SPECIAL	0x08
#define UNESCAPE_ANY		\
	(UNESCAPE_SPACE | UNESCAPE_OCTAL | UNESCAPE_HEX | UNESCAPE_SPECIAL)

=======
#define UNESCAPE_SPACE		BIT(0)
#define UNESCAPE_OCTAL		BIT(1)
#define UNESCAPE_HEX		BIT(2)
#define UNESCAPE_SPECIAL	BIT(3)
#define UNESCAPE_ANY		\
	(UNESCAPE_SPACE | UNESCAPE_OCTAL | UNESCAPE_HEX | UNESCAPE_SPECIAL)

#define UNESCAPE_ALL_MASK	GENMASK(3, 0)

>>>>>>> upstream/android-13
int string_unescape(char *src, char *dst, size_t size, unsigned int flags);

static inline int string_unescape_inplace(char *buf, unsigned int flags)
{
	return string_unescape(buf, buf, 0, flags);
}

static inline int string_unescape_any(char *src, char *dst, size_t size)
{
	return string_unescape(src, dst, size, UNESCAPE_ANY);
}

static inline int string_unescape_any_inplace(char *buf)
{
	return string_unescape_any(buf, buf, 0);
}

<<<<<<< HEAD
#define ESCAPE_SPACE		0x01
#define ESCAPE_SPECIAL		0x02
#define ESCAPE_NULL		0x04
#define ESCAPE_OCTAL		0x08
#define ESCAPE_ANY		\
	(ESCAPE_SPACE | ESCAPE_OCTAL | ESCAPE_SPECIAL | ESCAPE_NULL)
#define ESCAPE_NP		0x10
#define ESCAPE_ANY_NP		(ESCAPE_ANY | ESCAPE_NP)
#define ESCAPE_HEX		0x20
=======
#define ESCAPE_SPACE		BIT(0)
#define ESCAPE_SPECIAL		BIT(1)
#define ESCAPE_NULL		BIT(2)
#define ESCAPE_OCTAL		BIT(3)
#define ESCAPE_ANY		\
	(ESCAPE_SPACE | ESCAPE_OCTAL | ESCAPE_SPECIAL | ESCAPE_NULL)
#define ESCAPE_NP		BIT(4)
#define ESCAPE_ANY_NP		(ESCAPE_ANY | ESCAPE_NP)
#define ESCAPE_HEX		BIT(5)
#define ESCAPE_NA		BIT(6)
#define ESCAPE_NAP		BIT(7)
#define ESCAPE_APPEND		BIT(8)

#define ESCAPE_ALL_MASK		GENMASK(8, 0)
>>>>>>> upstream/android-13

int string_escape_mem(const char *src, size_t isz, char *dst, size_t osz,
		unsigned int flags, const char *only);

static inline int string_escape_mem_any_np(const char *src, size_t isz,
		char *dst, size_t osz, const char *only)
{
	return string_escape_mem(src, isz, dst, osz, ESCAPE_ANY_NP, only);
}

static inline int string_escape_str(const char *src, char *dst, size_t sz,
		unsigned int flags, const char *only)
{
	return string_escape_mem(src, strlen(src), dst, sz, flags, only);
}

static inline int string_escape_str_any_np(const char *src, char *dst,
		size_t sz, const char *only)
{
	return string_escape_str(src, dst, sz, ESCAPE_ANY_NP, only);
}

<<<<<<< HEAD
=======
static inline void string_upper(char *dst, const char *src)
{
	do {
		*dst++ = toupper(*src);
	} while (*src++);
}

static inline void string_lower(char *dst, const char *src)
{
	do {
		*dst++ = tolower(*src);
	} while (*src++);
}

>>>>>>> upstream/android-13
char *kstrdup_quotable(const char *src, gfp_t gfp);
char *kstrdup_quotable_cmdline(struct task_struct *task, gfp_t gfp);
char *kstrdup_quotable_file(struct file *file, gfp_t gfp);

<<<<<<< HEAD
=======
void kfree_strarray(char **array, size_t n);

>>>>>>> upstream/android-13
#endif
