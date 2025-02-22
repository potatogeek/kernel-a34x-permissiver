<<<<<<< HEAD
/*
 * Copyright (c) 2012 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

=======
// SPDX-License-Identifier: ISC
/*
 * Copyright (c) 2012 Broadcom Corporation
 */

#include <linux/device.h>
>>>>>>> upstream/android-13
#include <linux/module.h> /* bug in tracepoint.h, it should include this */

#ifndef __CHECKER__
#define CREATE_TRACE_POINTS
<<<<<<< HEAD
#include "tracepoint.h"
#include "debug.h"

void __brcmf_err(const char *func, const char *fmt, ...)
=======
#include "bus.h"
#include "tracepoint.h"
#include "debug.h"

void __brcmf_err(struct brcmf_bus *bus, const char *func, const char *fmt, ...)
>>>>>>> upstream/android-13
{
	struct va_format vaf = {
		.fmt = fmt,
	};
	va_list args;

	va_start(args, fmt);
	vaf.va = &args;
<<<<<<< HEAD
	pr_err("%s: %pV", func, &vaf);
=======
	if (bus)
		dev_err(bus->dev, "%s: %pV", func, &vaf);
	else
		pr_err("%s: %pV", func, &vaf);
>>>>>>> upstream/android-13
	trace_brcmf_err(func, &vaf);
	va_end(args);
}

#endif
