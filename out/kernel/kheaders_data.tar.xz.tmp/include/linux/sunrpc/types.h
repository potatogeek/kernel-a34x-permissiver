/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_SUNRPC_TYPES_H_
#define _LINUX_SUNRPC_TYPES_H_

#include <linux/timer.h>
#include <linux/sched/signal.h>
#include <linux/workqueue.h>
#include <linux/sunrpc/debug.h>
#include <linux/list.h>


#define signalled()		(signal_pending(current))

#endif 
