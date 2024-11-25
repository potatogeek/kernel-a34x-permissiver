<<<<<<< HEAD
/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> upstream/android-13
 */

#ifndef __IRQ_USER_H__
#define __IRQ_USER_H__

#include <sysdep/ptrace.h>
<<<<<<< HEAD
#include <stdbool.h>

struct irq_fd {
	struct irq_fd *next;
	void *id;
	int fd;
	int type;
	int irq;
	int events;
	bool active;
	bool pending;
	bool purge;
};

#define IRQ_READ  0
#define IRQ_WRITE 1
#define IRQ_NONE 2
#define MAX_IRQ_TYPE (IRQ_NONE + 1)



struct siginfo;
extern void sigio_handler(int sig, struct siginfo *unused_si, struct uml_pt_regs *regs);
extern void free_irq_by_fd(int fd);
extern void reactivate_fd(int fd, int irqnum);
=======

enum um_irq_type {
	IRQ_READ,
	IRQ_WRITE,
	NUM_IRQ_TYPES,
};

struct siginfo;
extern void sigio_handler(int sig, struct siginfo *unused_si, struct uml_pt_regs *regs);
void sigio_run_timetravel_handlers(void);
extern void free_irq_by_fd(int fd);
>>>>>>> upstream/android-13
extern void deactivate_fd(int fd, int irqnum);
extern int deactivate_all_fds(void);
extern int activate_ipi(int fd, int pid);

#endif
