<<<<<<< HEAD
/*
 * Copyright (C) 2016-2017 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2016-2017 Synopsys, Inc. (www.synopsys.com)
>>>>>>> upstream/android-13
 */

#ifndef __SOC_ARC_AUX_H__
#define __SOC_ARC_AUX_H__

#ifdef CONFIG_ARC

#define read_aux_reg(r)		__builtin_arc_lr(r)

/* gcc builtin sr needs reg param to be long immediate */
#define write_aux_reg(r, v)	__builtin_arc_sr((unsigned int)(v), r)

#else	/* !CONFIG_ARC */

static inline int read_aux_reg(u32 r)
{
	return 0;
}

/*
 * function helps elide unused variable warning
<<<<<<< HEAD
 * see: http://lists.infradead.org/pipermail/linux-snps-arc/2016-November/001748.html
=======
 * see: https://lists.infradead.org/pipermail/linux-snps-arc/2016-November/001748.html
>>>>>>> upstream/android-13
 */
static inline void write_aux_reg(u32 r, u32 v)
{
	;
}

#endif

#define READ_BCR(reg, into)				\
{							\
	unsigned int tmp;				\
	tmp = read_aux_reg(reg);			\
	if (sizeof(tmp) == sizeof(into)) {		\
		into = *((typeof(into) *)&tmp);		\
	} else {					\
		extern void bogus_undefined(void);	\
		bogus_undefined();			\
	}						\
}

#define WRITE_AUX(reg, into)				\
{							\
	unsigned int tmp;				\
	if (sizeof(tmp) == sizeof(into)) {		\
		tmp = (*(unsigned int *)&(into));	\
		write_aux_reg(reg, tmp);		\
	} else  {					\
		extern void bogus_undefined(void);	\
		bogus_undefined();			\
	}						\
}


#endif
