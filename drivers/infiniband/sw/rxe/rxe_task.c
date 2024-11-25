<<<<<<< HEAD
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
=======
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/hardirq.h>

#include "rxe_task.h"

int __rxe_do_task(struct rxe_task *task)

{
	int ret;

	while ((ret = task->func(task->arg)) == 0)
		;

	task->ret = ret;

	return ret;
}

/*
 * this locking is due to a potential race where
 * a second caller finds the task already running
 * but looks just after the last call to func
 */
<<<<<<< HEAD
void rxe_do_task(unsigned long data)
=======
void rxe_do_task(struct tasklet_struct *t)
>>>>>>> upstream/android-13
{
	int cont;
	int ret;
	unsigned long flags;
<<<<<<< HEAD
	struct rxe_task *task = (struct rxe_task *)data;
=======
	struct rxe_task *task = from_tasklet(task, t, tasklet);
>>>>>>> upstream/android-13

	spin_lock_irqsave(&task->state_lock, flags);
	switch (task->state) {
	case TASK_STATE_START:
		task->state = TASK_STATE_BUSY;
		spin_unlock_irqrestore(&task->state_lock, flags);
		break;

	case TASK_STATE_BUSY:
		task->state = TASK_STATE_ARMED;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	case TASK_STATE_ARMED:
		spin_unlock_irqrestore(&task->state_lock, flags);
		return;

	default:
		spin_unlock_irqrestore(&task->state_lock, flags);
		pr_warn("%s failed with bad state %d\n", __func__, task->state);
		return;
	}

	do {
		cont = 0;
		ret = task->func(task->arg);

		spin_lock_irqsave(&task->state_lock, flags);
		switch (task->state) {
		case TASK_STATE_BUSY:
			if (ret)
				task->state = TASK_STATE_START;
			else
				cont = 1;
			break;

		/* soneone tried to run the task since the last time we called
		 * func, so we will call one more time regardless of the
		 * return value
		 */
		case TASK_STATE_ARMED:
			task->state = TASK_STATE_BUSY;
			cont = 1;
			break;

		default:
			pr_warn("%s failed with bad state %d\n", __func__,
				task->state);
		}
		spin_unlock_irqrestore(&task->state_lock, flags);
	} while (cont);

	task->ret = ret;
}

int rxe_init_task(void *obj, struct rxe_task *task,
		  void *arg, int (*func)(void *), char *name)
{
	task->obj	= obj;
	task->arg	= arg;
	task->func	= func;
	snprintf(task->name, sizeof(task->name), "%s", name);
	task->destroyed	= false;

<<<<<<< HEAD
	tasklet_init(&task->tasklet, rxe_do_task, (unsigned long)task);
=======
	tasklet_setup(&task->tasklet, rxe_do_task);
>>>>>>> upstream/android-13

	task->state = TASK_STATE_START;
	spin_lock_init(&task->state_lock);

	return 0;
}

void rxe_cleanup_task(struct rxe_task *task)
{
	unsigned long flags;
	bool idle;

	/*
	 * Mark the task, then wait for it to finish. It might be
	 * running in a non-tasklet (direct call) context.
	 */
	task->destroyed = true;

	do {
		spin_lock_irqsave(&task->state_lock, flags);
		idle = (task->state == TASK_STATE_START);
		spin_unlock_irqrestore(&task->state_lock, flags);
	} while (!idle);

	tasklet_kill(&task->tasklet);
}

void rxe_run_task(struct rxe_task *task, int sched)
{
	if (task->destroyed)
		return;

	if (sched)
		tasklet_schedule(&task->tasklet);
	else
<<<<<<< HEAD
		rxe_do_task((unsigned long)task);
=======
		rxe_do_task(&task->tasklet);
>>>>>>> upstream/android-13
}

void rxe_disable_task(struct rxe_task *task)
{
	tasklet_disable(&task->tasklet);
}

void rxe_enable_task(struct rxe_task *task)
{
	tasklet_enable(&task->tasklet);
}
