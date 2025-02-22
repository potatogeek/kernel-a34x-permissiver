/* SPDX-License-Identifier: GPL-2.0 */

#ifndef MINMAX_H
#define MINMAX_H

#include <linux/types.h>


struct minmax_sample {
	u32	t;	
	u32	v;	
};


struct minmax {
	struct minmax_sample s[3];
};

static inline u32 minmax_get(const struct minmax *m)
{
	return m->s[0].v;
}

static inline u32 minmax_reset(struct minmax *m, u32 t, u32 meas)
{
	struct minmax_sample val = { .t = t, .v = meas };

	m->s[2] = m->s[1] = m->s[0] = val;
	return m->s[0].v;
}

u32 minmax_running_max(struct minmax *m, u32 win, u32 t, u32 meas);
u32 minmax_running_min(struct minmax *m, u32 win, u32 t, u32 meas);

#endif
