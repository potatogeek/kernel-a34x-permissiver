<<<<<<< HEAD
/**
 * Copyright (c) 2014 Raspberry Pi (Trading) Ltd. All rights reserved.
 * Copyright (c) 2010-2012 Broadcom. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2, as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=======
/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Copyright (c) 2014 Raspberry Pi (Trading) Ltd. All rights reserved.
 * Copyright (c) 2010-2012 Broadcom. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef VCHIQ_ARM_H
#define VCHIQ_ARM_H

#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/semaphore.h>
#include <linux/atomic.h>
#include "vchiq_core.h"
#include "vchiq_debugfs.h"

<<<<<<< HEAD
enum vc_suspend_status {
	VC_SUSPEND_FORCE_CANCELED = -3, /* Force suspend canceled, too busy */
	VC_SUSPEND_REJECTED = -2,  /* Videocore rejected suspend request */
	VC_SUSPEND_FAILED = -1,    /* Videocore suspend failed */
	VC_SUSPEND_IDLE = 0,       /* VC active, no suspend actions */
	VC_SUSPEND_REQUESTED,      /* User has requested suspend */
	VC_SUSPEND_IN_PROGRESS,    /* Slot handler has recvd suspend request */
	VC_SUSPEND_SUSPENDED       /* Videocore suspend succeeded */
};

enum vc_resume_status {
	VC_RESUME_FAILED = -1, /* Videocore resume failed */
	VC_RESUME_IDLE = 0,    /* VC suspended, no resume actions */
	VC_RESUME_REQUESTED,   /* User has requested resume */
	VC_RESUME_IN_PROGRESS, /* Slot handler has received resume request */
	VC_RESUME_RESUMED      /* Videocore resumed successfully (active) */
};

enum USE_TYPE_E {
	USE_TYPE_SERVICE,
	USE_TYPE_SERVICE_NO_RESUME,
	USE_TYPE_VCHIQ
};

typedef struct vchiq_arm_state_struct {
=======
/* Some per-instance constants */
#define MAX_COMPLETIONS 128
#define MAX_SERVICES 64
#define MAX_ELEMENTS 8
#define MSG_QUEUE_SIZE 128

enum USE_TYPE_E {
	USE_TYPE_SERVICE,
	USE_TYPE_VCHIQ
};

struct vchiq_arm_state {
>>>>>>> upstream/android-13
	/* Keepalive-related data */
	struct task_struct *ka_thread;
	struct completion ka_evt;
	atomic_t ka_use_count;
	atomic_t ka_use_ack_count;
	atomic_t ka_release_count;

<<<<<<< HEAD
	struct completion vc_suspend_complete;
	struct completion vc_resume_complete;

	rwlock_t susp_res_lock;
	enum vc_suspend_status vc_suspend_state;
	enum vc_resume_status vc_resume_state;

	unsigned int wake_address;

	VCHIQ_STATE_T *state;
	struct timer_list suspend_timer;
	int suspend_timer_timeout;
	int suspend_timer_running;

	/* Global use count for videocore.
	** This is equal to the sum of the use counts for all services.  When
	** this hits zero the videocore suspend procedure will be initiated.
	*/
	int videocore_use_count;

	/* Use count to track requests from videocore peer.
	** This use count is not associated with a service, so needs to be
	** tracked separately with the state.
	*/
	int peer_use_count;

	/* Flag to indicate whether resume is blocked.  This happens when the
	** ARM is suspending
	*/
	struct completion resume_blocker;
	int resume_blocked;
	struct completion blocked_blocker;
	int blocked_count;

	int autosuspend_override;

	/* Flag to indicate that the first vchiq connect has made it through.
	** This means that both sides should be fully ready, and we should
	** be able to suspend after this point.
	*/
	int first_connect;

	unsigned long long suspend_start_time;
	unsigned long long sleep_start_time;
	unsigned long long resume_start_time;
	unsigned long long last_wake_time;

} VCHIQ_ARM_STATE_T;
=======
	rwlock_t susp_res_lock;

	struct vchiq_state *state;

	/*
	 * Global use count for videocore.
	 * This is equal to the sum of the use counts for all services.  When
	 * this hits zero the videocore suspend procedure will be initiated.
	 */
	int videocore_use_count;

	/*
	 * Use count to track requests from videocore peer.
	 * This use count is not associated with a service, so needs to be
	 * tracked separately with the state.
	 */
	int peer_use_count;

	/*
	 * Flag to indicate that the first vchiq connect has made it through.
	 * This means that both sides should be fully ready, and we should
	 * be able to suspend after this point.
	 */
	int first_connect;
};

struct vchiq_drvdata {
	const unsigned int cache_line_size;
	struct rpi_firmware *fw;
};

struct user_service {
	struct vchiq_service *service;
	void __user *userdata;
	struct vchiq_instance *instance;
	char is_vchi;
	char dequeue_pending;
	char close_pending;
	int message_available_pos;
	int msg_insert;
	int msg_remove;
	struct completion insert_event;
	struct completion remove_event;
	struct completion close_event;
	struct vchiq_header *msg_queue[MSG_QUEUE_SIZE];
};

struct bulk_waiter_node {
	struct bulk_waiter bulk_waiter;
	int pid;
	struct list_head list;
};

struct vchiq_instance {
	struct vchiq_state *state;
	struct vchiq_completion_data_kernel completions[MAX_COMPLETIONS];
	int completion_insert;
	int completion_remove;
	struct completion insert_event;
	struct completion remove_event;
	struct mutex completion_mutex;

	int connected;
	int closing;
	int pid;
	int mark;
	int use_close_delivered;
	int trace;

	struct list_head bulk_waiter_list;
	struct mutex bulk_waiter_list_mutex;

	struct vchiq_debugfs_node debugfs_node;
};

struct dump_context {
	char __user *buf;
	size_t actual;
	size_t space;
	loff_t offset;
};
>>>>>>> upstream/android-13

extern int vchiq_arm_log_level;
extern int vchiq_susp_log_level;

<<<<<<< HEAD
int vchiq_platform_init(struct platform_device *pdev, VCHIQ_STATE_T *state);

extern VCHIQ_STATE_T *
vchiq_get_state(void);

extern VCHIQ_STATUS_T
vchiq_arm_vcsuspend(VCHIQ_STATE_T *state);

extern VCHIQ_STATUS_T
vchiq_arm_force_suspend(VCHIQ_STATE_T *state);

extern int
vchiq_arm_allow_resume(VCHIQ_STATE_T *state);

extern VCHIQ_STATUS_T
vchiq_arm_vcresume(VCHIQ_STATE_T *state);

extern VCHIQ_STATUS_T
vchiq_arm_init_state(VCHIQ_STATE_T *state, VCHIQ_ARM_STATE_T *arm_state);

extern int
vchiq_check_resume(VCHIQ_STATE_T *state);

extern void
vchiq_check_suspend(VCHIQ_STATE_T *state);
VCHIQ_STATUS_T
vchiq_use_service(VCHIQ_SERVICE_HANDLE_T handle);

extern VCHIQ_STATUS_T
vchiq_release_service(VCHIQ_SERVICE_HANDLE_T handle);

extern VCHIQ_STATUS_T
vchiq_check_service(VCHIQ_SERVICE_T *service);

extern VCHIQ_STATUS_T
vchiq_platform_suspend(VCHIQ_STATE_T *state);

extern int
vchiq_platform_videocore_wanted(VCHIQ_STATE_T *state);

extern int
vchiq_platform_use_suspend_timer(void);

extern void
vchiq_dump_platform_use_state(VCHIQ_STATE_T *state);

extern void
vchiq_dump_service_use_state(VCHIQ_STATE_T *state);

extern VCHIQ_ARM_STATE_T*
vchiq_platform_get_arm_state(VCHIQ_STATE_T *state);

extern int
vchiq_videocore_wanted(VCHIQ_STATE_T *state);

extern VCHIQ_STATUS_T
vchiq_use_internal(VCHIQ_STATE_T *state, VCHIQ_SERVICE_T *service,
		enum USE_TYPE_E use_type);
extern VCHIQ_STATUS_T
vchiq_release_internal(VCHIQ_STATE_T *state, VCHIQ_SERVICE_T *service);

extern VCHIQ_DEBUGFS_NODE_T *
vchiq_instance_get_debugfs_node(VCHIQ_INSTANCE_T instance);

extern int
vchiq_instance_get_use_count(VCHIQ_INSTANCE_T instance);

extern int
vchiq_instance_get_pid(VCHIQ_INSTANCE_T instance);

extern int
vchiq_instance_get_trace(VCHIQ_INSTANCE_T instance);

extern void
vchiq_instance_set_trace(VCHIQ_INSTANCE_T instance, int trace);

extern void
set_suspend_state(VCHIQ_ARM_STATE_T *arm_state,
	enum vc_suspend_status new_state);

extern void
set_resume_state(VCHIQ_ARM_STATE_T *arm_state,
	enum vc_resume_status new_state);

extern void
start_suspend_timer(VCHIQ_ARM_STATE_T *arm_state);
=======
extern spinlock_t msg_queue_spinlock;
extern struct vchiq_state g_state;

int vchiq_platform_init(struct platform_device *pdev,
			struct vchiq_state *state);

extern struct vchiq_state *
vchiq_get_state(void);

extern void
vchiq_arm_init_state(struct vchiq_state *state,
		     struct vchiq_arm_state *arm_state);

extern void
vchiq_check_suspend(struct vchiq_state *state);
enum vchiq_status
vchiq_use_service(unsigned int handle);

extern enum vchiq_status
vchiq_release_service(unsigned int handle);

extern enum vchiq_status
vchiq_check_service(struct vchiq_service *service);

extern void
vchiq_dump_platform_use_state(struct vchiq_state *state);

extern void
vchiq_dump_service_use_state(struct vchiq_state *state);

extern struct vchiq_arm_state*
vchiq_platform_get_arm_state(struct vchiq_state *state);


extern enum vchiq_status
vchiq_use_internal(struct vchiq_state *state, struct vchiq_service *service,
		   enum USE_TYPE_E use_type);
extern enum vchiq_status
vchiq_release_internal(struct vchiq_state *state,
		       struct vchiq_service *service);

extern struct vchiq_debugfs_node *
vchiq_instance_get_debugfs_node(struct vchiq_instance *instance);

extern int
vchiq_instance_get_use_count(struct vchiq_instance *instance);

extern int
vchiq_instance_get_pid(struct vchiq_instance *instance);

extern int
vchiq_instance_get_trace(struct vchiq_instance *instance);

extern void
vchiq_instance_set_trace(struct vchiq_instance *instance, int trace);

#if IS_ENABLED(CONFIG_VCHIQ_CDEV)

extern void
vchiq_deregister_chrdev(void);

extern int
vchiq_register_chrdev(struct device *parent);

#else

static inline void vchiq_deregister_chrdev(void) { }
static inline int vchiq_register_chrdev(struct device *parent) { return 0; }

#endif /* IS_ENABLED(CONFIG_VCHIQ_CDEV) */

extern enum vchiq_status
service_callback(enum vchiq_reason reason, struct vchiq_header *header,
		 unsigned int handle, void *bulk_userdata);

extern void
free_bulk_waiter(struct vchiq_instance *instance);
>>>>>>> upstream/android-13

#endif /* VCHIQ_ARM_H */
