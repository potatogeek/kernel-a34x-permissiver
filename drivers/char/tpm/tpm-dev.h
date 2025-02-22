/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _TPM_DEV_H
#define _TPM_DEV_H

<<<<<<< HEAD
=======
#include <linux/poll.h>
>>>>>>> upstream/android-13
#include "tpm.h"

struct file_priv {
	struct tpm_chip *chip;
<<<<<<< HEAD

	/* Data passed to and from the tpm via the read/write calls */
	size_t data_pending;
	struct mutex buffer_mutex;

	struct timer_list user_read_timer;      /* user needs to claim result */
	struct work_struct work;
=======
	struct tpm_space *space;

	struct mutex buffer_mutex;
	struct timer_list user_read_timer;      /* user needs to claim result */
	struct work_struct timeout_work;
	struct work_struct async_work;
	wait_queue_head_t async_wait;
	ssize_t response_length;
	bool response_read;
	bool command_enqueued;
>>>>>>> upstream/android-13

	u8 data_buffer[TPM_BUFSIZE];
};

void tpm_common_open(struct file *file, struct tpm_chip *chip,
<<<<<<< HEAD
		     struct file_priv *priv);
ssize_t tpm_common_read(struct file *file, char __user *buf,
			size_t size, loff_t *off);
ssize_t tpm_common_write(struct file *file, const char __user *buf,
			 size_t size, loff_t *off, struct tpm_space *space);
void tpm_common_release(struct file *file, struct file_priv *priv);

=======
		     struct file_priv *priv, struct tpm_space *space);
ssize_t tpm_common_read(struct file *file, char __user *buf,
			size_t size, loff_t *off);
ssize_t tpm_common_write(struct file *file, const char __user *buf,
			 size_t size, loff_t *off);
__poll_t tpm_common_poll(struct file *file, poll_table *wait);

void tpm_common_release(struct file *file, struct file_priv *priv);
>>>>>>> upstream/android-13
#endif
