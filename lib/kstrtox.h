/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIB_KSTRTOX_H
#define _LIB_KSTRTOX_H

#define KSTRTOX_OVERFLOW	(1U << 31)
const char *_parse_integer_fixup_radix(const char *s, unsigned int *base);
<<<<<<< HEAD
=======
unsigned int _parse_integer_limit(const char *s, unsigned int base, unsigned long long *res,
				  size_t max_chars);
>>>>>>> upstream/android-13
unsigned int _parse_integer(const char *s, unsigned int base, unsigned long long *res);

#endif
