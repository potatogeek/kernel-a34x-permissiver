<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2012 Intel, Inc.
 * Copyright (C) 2013 Intel, Inc.
 * Copyright (C) 2014 Linaro Limited
 * Copyright (C) 2011-2016 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* This source file contains the implementation of a special device driver
 * that intends to provide a *very* fast communication channel between the
 * guest system and the QEMU emulator.
 *
 * Usage from the guest is simply the following (error handling simplified):
 *
 *    int  fd = open("/dev/qemu_pipe",O_RDWR);
 *    .... write() or read() through the pipe.
 *
 * This driver doesn't deal with the exact protocol used during the session.
 * It is intended to be as simple as something like:
 *
 *    // do this _just_ after opening the fd to connect to a specific
 *    // emulator service.
 *    const char*  msg = "<pipename>";
 *    if (write(fd, msg, strlen(msg)+1) < 0) {
 *       ... could not connect to <pipename> service
 *       close(fd);
 *    }
 *
 *    // after this, simply read() and write() to communicate with the
 *    // service. Exact protocol details left as an exercise to the reader.
 *
 * This driver is very fast because it doesn't copy any data through
 * intermediate buffers, since the emulator is capable of translating
 * guest user addresses into host ones.
 *
 * Note that we must however ensure that each user page involved in the
 * exchange is properly mapped during a transfer.
 */

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/goldfish.h>
#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/acpi.h>
=======
#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/acpi.h>
#include <linux/bug.h>
#include "goldfish_pipe_qemu.h"
>>>>>>> upstream/android-13

/*
 * Update this when something changes in the driver's behavior so the host
 * can benefit from knowing it
 */
enum {
	PIPE_DRIVER_VERSION = 2,
	PIPE_CURRENT_DEVICE_VERSION = 2
};

<<<<<<< HEAD
/*
 * IMPORTANT: The following constants must match the ones used and defined
 * in external/qemu/hw/goldfish_pipe.c in the Android source tree.
 */

/* List of bitflags returned in status of CMD_POLL command */
enum PipePollFlags {
	PIPE_POLL_IN	= 1 << 0,
	PIPE_POLL_OUT	= 1 << 1,
	PIPE_POLL_HUP	= 1 << 2
};

/* Possible status values used to signal errors - see goldfish_pipe_error_convert */
enum PipeErrors {
	PIPE_ERROR_INVAL  = -1,
	PIPE_ERROR_AGAIN  = -2,
	PIPE_ERROR_NOMEM  = -3,
	PIPE_ERROR_IO     = -4
};

/* Bit-flags used to signal events from the emulator */
enum PipeWakeFlags {
	PIPE_WAKE_CLOSED = 1 << 0,  /* emulator closed pipe */
	PIPE_WAKE_READ   = 1 << 1,  /* pipe can now be read from */
	PIPE_WAKE_WRITE  = 1 << 2  /* pipe can now be written to */
};

/* Bit flags for the 'flags' field */
enum PipeFlagsBits {
	BIT_CLOSED_ON_HOST = 0,  /* pipe closed by host */
	BIT_WAKE_ON_WRITE  = 1,  /* want to be woken on writes */
	BIT_WAKE_ON_READ   = 2,  /* want to be woken on reads */
};

enum PipeRegs {
	PIPE_REG_CMD = 0,

	PIPE_REG_SIGNAL_BUFFER_HIGH = 4,
	PIPE_REG_SIGNAL_BUFFER = 8,
	PIPE_REG_SIGNAL_BUFFER_COUNT = 12,

	PIPE_REG_OPEN_BUFFER_HIGH = 20,
	PIPE_REG_OPEN_BUFFER = 24,

	PIPE_REG_VERSION = 36,

	PIPE_REG_GET_SIGNALLED = 48,
};

enum PipeCmdCode {
	PIPE_CMD_OPEN = 1,	/* to be used by the pipe device itself */
	PIPE_CMD_CLOSE,
	PIPE_CMD_POLL,
	PIPE_CMD_WRITE,
	PIPE_CMD_WAKE_ON_WRITE,
	PIPE_CMD_READ,
	PIPE_CMD_WAKE_ON_READ,

	/*
	 * TODO(zyy): implement a deferred read/write execution to allow
	 * parallel processing of pipe operations on the host.
	 */
	PIPE_CMD_WAKE_ON_DONE_IO,
};

=======
>>>>>>> upstream/android-13
enum {
	MAX_BUFFERS_PER_COMMAND = 336,
	MAX_SIGNALLED_PIPES = 64,
	INITIAL_PIPES_CAPACITY = 64
};

struct goldfish_pipe_dev;
<<<<<<< HEAD
struct goldfish_pipe;
struct goldfish_pipe_command;

/* A per-pipe command structure, shared with the host */
struct goldfish_pipe_command {
	s32 cmd;		/* PipeCmdCode, guest -> host */
	s32 id;			/* pipe id, guest -> host */
	s32 status;		/* command execution status, host -> guest */
=======

/* A per-pipe command structure, shared with the host */
struct goldfish_pipe_command {
	s32 cmd;	/* PipeCmdCode, guest -> host */
	s32 id;		/* pipe id, guest -> host */
	s32 status;	/* command execution status, host -> guest */
>>>>>>> upstream/android-13
	s32 reserved;	/* to pad to 64-bit boundary */
	union {
		/* Parameters for PIPE_CMD_{READ,WRITE} */
		struct {
			/* number of buffers, guest -> host */
			u32 buffers_count;
			/* number of consumed bytes, host -> guest */
			s32 consumed_size;
			/* buffer pointers, guest -> host */
			u64 ptrs[MAX_BUFFERS_PER_COMMAND];
			/* buffer sizes, guest -> host */
			u32 sizes[MAX_BUFFERS_PER_COMMAND];
		} rw_params;
	};
};

/* A single signalled pipe information */
struct signalled_pipe_buffer {
	u32 id;
	u32 flags;
};

/* Parameters for the PIPE_CMD_OPEN command */
struct open_command_param {
	u64 command_buffer_ptr;
	u32 rw_params_max_count;
};

/* Device-level set of buffers shared with the host */
struct goldfish_pipe_dev_buffers {
	struct open_command_param open_command_params;
<<<<<<< HEAD
	struct signalled_pipe_buffer signalled_pipe_buffers[
		MAX_SIGNALLED_PIPES];
=======
	struct signalled_pipe_buffer
		signalled_pipe_buffers[MAX_SIGNALLED_PIPES];
>>>>>>> upstream/android-13
};

/* This data type models a given pipe instance */
struct goldfish_pipe {
	/* pipe ID - index into goldfish_pipe_dev::pipes array */
	u32 id;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	/* The wake flags pipe is waiting for
	 * Note: not protected with any lock, uses atomic operations
	 *  and barriers to make it thread-safe.
	 */
	unsigned long flags;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	/* wake flags host have signalled,
	 *  - protected by goldfish_pipe_dev::lock
	 */
	unsigned long signalled_flags;

	/* A pointer to command buffer */
	struct goldfish_pipe_command *command_buffer;

	/* doubly linked list of signalled pipes, protected by
	 * goldfish_pipe_dev::lock
	 */
	struct goldfish_pipe *prev_signalled;
	struct goldfish_pipe *next_signalled;

	/*
	 * A pipe's own lock. Protects the following:
	 *  - *command_buffer - makes sure a command can safely write its
	 *    parameters to the host and read the results back.
	 */
	struct mutex lock;

	/* A wake queue for sleeping until host signals an event */
	wait_queue_head_t wake_queue;
<<<<<<< HEAD
	/* Pointer to the parent goldfish_pipe_dev instance */
	struct goldfish_pipe_dev *dev;
=======

	/* Pointer to the parent goldfish_pipe_dev instance */
	struct goldfish_pipe_dev *dev;

	/* A buffer of pages, too large to fit into a stack frame */
	struct page *pages[MAX_BUFFERS_PER_COMMAND];
>>>>>>> upstream/android-13
};

/* The global driver data. Holds a reference to the i/o page used to
 * communicate with the emulator, and a wake queue for blocked tasks
 * waiting to be awoken.
 */
struct goldfish_pipe_dev {
<<<<<<< HEAD
=======
	/* A magic number to check if this is an instance of this struct */
	void *magic;

>>>>>>> upstream/android-13
	/*
	 * Global device spinlock. Protects the following members:
	 *  - pipes, pipes_capacity
	 *  - [*pipes, *pipes + pipes_capacity) - array data
	 *  - first_signalled_pipe,
	 *      goldfish_pipe::prev_signalled,
	 *      goldfish_pipe::next_signalled,
	 *      goldfish_pipe::signalled_flags - all singnalled-related fields,
	 *                                       in all allocated pipes
	 *  - open_command_params - PIPE_CMD_OPEN-related buffers
	 *
	 * It looks like a lot of different fields, but the trick is that
	 * the only operation that happens often is the signalled pipes array
	 * manipulation. That's why it's OK for now to keep the rest of the
	 * fields under the same lock. If we notice too much contention because
	 * of PIPE_CMD_OPEN, then we should add a separate lock there.
	 */
	spinlock_t lock;

	/*
	 * Array of the pipes of |pipes_capacity| elements,
	 * indexed by goldfish_pipe::id
	 */
	struct goldfish_pipe **pipes;
	u32 pipes_capacity;

	/* Pointers to the buffers host uses for interaction with this driver */
	struct goldfish_pipe_dev_buffers *buffers;

	/* Head of a doubly linked list of signalled pipes */
	struct goldfish_pipe *first_signalled_pipe;

<<<<<<< HEAD
=======
	/* ptr to platform device's device struct */
	struct device *pdev_dev;

>>>>>>> upstream/android-13
	/* Some device-specific data */
	int irq;
	int version;
	unsigned char __iomem *base;
<<<<<<< HEAD
};

static struct goldfish_pipe_dev pipe_dev[1] = {};

static int goldfish_cmd_locked(struct goldfish_pipe *pipe, enum PipeCmdCode cmd)
=======

	struct miscdevice miscdev;
};

static int goldfish_pipe_cmd_locked(struct goldfish_pipe *pipe,
				    enum PipeCmdCode cmd)
>>>>>>> upstream/android-13
{
	pipe->command_buffer->cmd = cmd;
	/* failure by default */
	pipe->command_buffer->status = PIPE_ERROR_INVAL;
	writel(pipe->id, pipe->dev->base + PIPE_REG_CMD);
	return pipe->command_buffer->status;
}

<<<<<<< HEAD
static int goldfish_cmd(struct goldfish_pipe *pipe, enum PipeCmdCode cmd)
=======
static int goldfish_pipe_cmd(struct goldfish_pipe *pipe, enum PipeCmdCode cmd)
>>>>>>> upstream/android-13
{
	int status;

	if (mutex_lock_interruptible(&pipe->lock))
		return PIPE_ERROR_IO;
<<<<<<< HEAD
	status = goldfish_cmd_locked(pipe, cmd);
=======
	status = goldfish_pipe_cmd_locked(pipe, cmd);
>>>>>>> upstream/android-13
	mutex_unlock(&pipe->lock);
	return status;
}

/*
 * This function converts an error code returned by the emulator through
 * the PIPE_REG_STATUS i/o register into a valid negative errno value.
 */
static int goldfish_pipe_error_convert(int status)
{
	switch (status) {
	case PIPE_ERROR_AGAIN:
		return -EAGAIN;
	case PIPE_ERROR_NOMEM:
		return -ENOMEM;
	case PIPE_ERROR_IO:
		return -EIO;
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int pin_user_pages(unsigned long first_page, unsigned long last_page,
	unsigned int last_page_size, int is_write,
	struct page *pages[MAX_BUFFERS_PER_COMMAND],
	unsigned int *iter_last_page_size)
=======
static int goldfish_pin_pages(unsigned long first_page,
			      unsigned long last_page,
			      unsigned int last_page_size,
			      int is_write,
			      struct page *pages[MAX_BUFFERS_PER_COMMAND],
			      unsigned int *iter_last_page_size)
>>>>>>> upstream/android-13
{
	int ret;
	int requested_pages = ((last_page - first_page) >> PAGE_SHIFT) + 1;

	if (requested_pages > MAX_BUFFERS_PER_COMMAND) {
		requested_pages = MAX_BUFFERS_PER_COMMAND;
		*iter_last_page_size = PAGE_SIZE;
	} else {
		*iter_last_page_size = last_page_size;
	}

<<<<<<< HEAD
	ret = get_user_pages_fast(
			first_page, requested_pages, !is_write, pages);
=======
	ret = pin_user_pages_fast(first_page, requested_pages,
				  !is_write ? FOLL_WRITE : 0,
				  pages);
>>>>>>> upstream/android-13
	if (ret <= 0)
		return -EFAULT;
	if (ret < requested_pages)
		*iter_last_page_size = PAGE_SIZE;
<<<<<<< HEAD
	return ret;

}

static void release_user_pages(struct page **pages, int pages_count,
	int is_write, s32 consumed_size)
{
	int i;

	for (i = 0; i < pages_count; i++) {
		if (!is_write && consumed_size > 0)
			set_page_dirty(pages[i]);
		put_page(pages[i]);
	}
}

/* Populate the call parameters, merging adjacent pages together */
static void populate_rw_params(
	struct page **pages, int pages_count,
	unsigned long address, unsigned long address_end,
	unsigned long first_page, unsigned long last_page,
	unsigned int iter_last_page_size, int is_write,
	struct goldfish_pipe_command *command)
=======

	return ret;
}

/* Populate the call parameters, merging adjacent pages together */
static void populate_rw_params(struct page **pages,
			       int pages_count,
			       unsigned long address,
			       unsigned long address_end,
			       unsigned long first_page,
			       unsigned long last_page,
			       unsigned int iter_last_page_size,
			       int is_write,
			       struct goldfish_pipe_command *command)
>>>>>>> upstream/android-13
{
	/*
	 * Process the first page separately - it's the only page that
	 * needs special handling for its start address.
	 */
	unsigned long xaddr = page_to_phys(pages[0]);
	unsigned long xaddr_prev = xaddr;
	int buffer_idx = 0;
	int i = 1;
	int size_on_page = first_page == last_page
			? (int)(address_end - address)
			: (PAGE_SIZE - (address & ~PAGE_MASK));
	command->rw_params.ptrs[0] = (u64)(xaddr | (address & ~PAGE_MASK));
	command->rw_params.sizes[0] = size_on_page;
	for (; i < pages_count; ++i) {
		xaddr = page_to_phys(pages[i]);
		size_on_page = (i == pages_count - 1) ?
			iter_last_page_size : PAGE_SIZE;
		if (xaddr == xaddr_prev + PAGE_SIZE) {
			command->rw_params.sizes[buffer_idx] += size_on_page;
		} else {
			++buffer_idx;
			command->rw_params.ptrs[buffer_idx] = (u64)xaddr;
			command->rw_params.sizes[buffer_idx] = size_on_page;
		}
		xaddr_prev = xaddr;
	}
	command->rw_params.buffers_count = buffer_idx + 1;
}

static int transfer_max_buffers(struct goldfish_pipe *pipe,
<<<<<<< HEAD
	unsigned long address, unsigned long address_end, int is_write,
	unsigned long last_page, unsigned int last_page_size,
	s32 *consumed_size, int *status)
{
	static struct page *pages[MAX_BUFFERS_PER_COMMAND];
	unsigned long first_page = address & PAGE_MASK;
	unsigned int iter_last_page_size;
	int pages_count = pin_user_pages(first_page, last_page,
			last_page_size, is_write,
			pages, &iter_last_page_size);

	if (pages_count < 0)
		return pages_count;
=======
				unsigned long address,
				unsigned long address_end,
				int is_write,
				unsigned long last_page,
				unsigned int last_page_size,
				s32 *consumed_size,
				int *status)
{
	unsigned long first_page = address & PAGE_MASK;
	unsigned int iter_last_page_size;
	int pages_count;
>>>>>>> upstream/android-13

	/* Serialize access to the pipe command buffers */
	if (mutex_lock_interruptible(&pipe->lock))
		return -ERESTARTSYS;

<<<<<<< HEAD
	populate_rw_params(pages, pages_count, address, address_end,
		first_page, last_page, iter_last_page_size, is_write,
		pipe->command_buffer);

	/* Transfer the data */
	*status = goldfish_cmd_locked(pipe,
=======
	pages_count = goldfish_pin_pages(first_page, last_page,
					 last_page_size, is_write,
					 pipe->pages, &iter_last_page_size);
	if (pages_count < 0) {
		mutex_unlock(&pipe->lock);
		return pages_count;
	}

	populate_rw_params(pipe->pages, pages_count, address, address_end,
			   first_page, last_page, iter_last_page_size, is_write,
			   pipe->command_buffer);

	/* Transfer the data */
	*status = goldfish_pipe_cmd_locked(pipe,
>>>>>>> upstream/android-13
				is_write ? PIPE_CMD_WRITE : PIPE_CMD_READ);

	*consumed_size = pipe->command_buffer->rw_params.consumed_size;

<<<<<<< HEAD
	release_user_pages(pages, pages_count, is_write, *consumed_size);

	mutex_unlock(&pipe->lock);

=======
	unpin_user_pages_dirty_lock(pipe->pages, pages_count,
				    !is_write && *consumed_size > 0);

	mutex_unlock(&pipe->lock);
>>>>>>> upstream/android-13
	return 0;
}

static int wait_for_host_signal(struct goldfish_pipe *pipe, int is_write)
{
<<<<<<< HEAD
	u32 wakeBit = is_write ? BIT_WAKE_ON_WRITE : BIT_WAKE_ON_READ;

	set_bit(wakeBit, &pipe->flags);

	/* Tell the emulator we're going to wait for a wake event */
	(void)goldfish_cmd(pipe,
		is_write ? PIPE_CMD_WAKE_ON_WRITE : PIPE_CMD_WAKE_ON_READ);

	while (test_bit(wakeBit, &pipe->flags)) {
		if (wait_event_interruptible(
				pipe->wake_queue,
				!test_bit(wakeBit, &pipe->flags)))
=======
	u32 wake_bit = is_write ? BIT_WAKE_ON_WRITE : BIT_WAKE_ON_READ;

	set_bit(wake_bit, &pipe->flags);

	/* Tell the emulator we're going to wait for a wake event */
	goldfish_pipe_cmd(pipe,
		is_write ? PIPE_CMD_WAKE_ON_WRITE : PIPE_CMD_WAKE_ON_READ);

	while (test_bit(wake_bit, &pipe->flags)) {
		if (wait_event_interruptible(pipe->wake_queue,
					     !test_bit(wake_bit, &pipe->flags)))
>>>>>>> upstream/android-13
			return -ERESTARTSYS;

		if (test_bit(BIT_CLOSED_ON_HOST, &pipe->flags))
			return -EIO;
	}

	return 0;
}

static ssize_t goldfish_pipe_read_write(struct file *filp,
<<<<<<< HEAD
	char __user *buffer, size_t bufflen, int is_write)
=======
					char __user *buffer,
					size_t bufflen,
					int is_write)
>>>>>>> upstream/android-13
{
	struct goldfish_pipe *pipe = filp->private_data;
	int count = 0, ret = -EINVAL;
	unsigned long address, address_end, last_page;
	unsigned int last_page_size;

	/* If the emulator already closed the pipe, no need to go further */
	if (unlikely(test_bit(BIT_CLOSED_ON_HOST, &pipe->flags)))
		return -EIO;
	/* Null reads or writes succeeds */
	if (unlikely(bufflen == 0))
		return 0;
	/* Check the buffer range for access */
<<<<<<< HEAD
	if (unlikely(!access_ok(is_write ? VERIFY_WRITE : VERIFY_READ,
			buffer, bufflen)))
=======
	if (unlikely(!access_ok(buffer, bufflen)))
>>>>>>> upstream/android-13
		return -EFAULT;

	address = (unsigned long)buffer;
	address_end = address + bufflen;
	last_page = (address_end - 1) & PAGE_MASK;
	last_page_size = ((address_end - 1) & ~PAGE_MASK) + 1;

	while (address < address_end) {
		s32 consumed_size;
		int status;

		ret = transfer_max_buffers(pipe, address, address_end, is_write,
<<<<<<< HEAD
				last_page, last_page_size, &consumed_size,
				&status);
=======
					   last_page, last_page_size,
					   &consumed_size, &status);
>>>>>>> upstream/android-13
		if (ret < 0)
			break;

		if (consumed_size > 0) {
			/* No matter what's the status, we've transferred
			 * something.
			 */
			count += consumed_size;
			address += consumed_size;
		}
		if (status > 0)
			continue;
		if (status == 0) {
			/* EOF */
			ret = 0;
			break;
		}
		if (count > 0) {
			/*
			 * An error occurred, but we already transferred
			 * something on one of the previous iterations.
			 * Just return what we already copied and log this
			 * err.
			 */
			if (status != PIPE_ERROR_AGAIN)
<<<<<<< HEAD
				pr_info_ratelimited("goldfish_pipe: backend error %d on %s\n",
=======
				dev_err_ratelimited(pipe->dev->pdev_dev,
					"backend error %d on %s\n",
>>>>>>> upstream/android-13
					status, is_write ? "write" : "read");
			break;
		}

		/*
		 * If the error is not PIPE_ERROR_AGAIN, or if we are in
		 * non-blocking mode, just return the error code.
		 */
		if (status != PIPE_ERROR_AGAIN ||
			(filp->f_flags & O_NONBLOCK) != 0) {
			ret = goldfish_pipe_error_convert(status);
			break;
		}

		status = wait_for_host_signal(pipe, is_write);
		if (status < 0)
			return status;
	}

	if (count > 0)
		return count;
	return ret;
}

static ssize_t goldfish_pipe_read(struct file *filp, char __user *buffer,
<<<<<<< HEAD
				size_t bufflen, loff_t *ppos)
{
	return goldfish_pipe_read_write(filp, buffer, bufflen,
			/* is_write */ 0);
}

static ssize_t goldfish_pipe_write(struct file *filp,
				const char __user *buffer, size_t bufflen,
				loff_t *ppos)
{
	return goldfish_pipe_read_write(filp,
			/* cast away the const */(char __user *)buffer, bufflen,
			/* is_write */ 1);
=======
				  size_t bufflen, loff_t *ppos)
{
	return goldfish_pipe_read_write(filp, buffer, bufflen,
					/* is_write */ 0);
}

static ssize_t goldfish_pipe_write(struct file *filp,
				   const char __user *buffer, size_t bufflen,
				   loff_t *ppos)
{
	/* cast away the const */
	char __user *no_const_buffer = (char __user *)buffer;

	return goldfish_pipe_read_write(filp, no_const_buffer, bufflen,
					/* is_write */ 1);
>>>>>>> upstream/android-13
}

static __poll_t goldfish_pipe_poll(struct file *filp, poll_table *wait)
{
	struct goldfish_pipe *pipe = filp->private_data;
	__poll_t mask = 0;
	int status;

	poll_wait(filp, &pipe->wake_queue, wait);

<<<<<<< HEAD
	status = goldfish_cmd(pipe, PIPE_CMD_POLL);
=======
	status = goldfish_pipe_cmd(pipe, PIPE_CMD_POLL);
>>>>>>> upstream/android-13
	if (status < 0)
		return -ERESTARTSYS;

	if (status & PIPE_POLL_IN)
		mask |= EPOLLIN | EPOLLRDNORM;
	if (status & PIPE_POLL_OUT)
		mask |= EPOLLOUT | EPOLLWRNORM;
	if (status & PIPE_POLL_HUP)
		mask |= EPOLLHUP;
	if (test_bit(BIT_CLOSED_ON_HOST, &pipe->flags))
		mask |= EPOLLERR;

	return mask;
}

static void signalled_pipes_add_locked(struct goldfish_pipe_dev *dev,
<<<<<<< HEAD
	u32 id, u32 flags)
=======
				       u32 id, u32 flags)
>>>>>>> upstream/android-13
{
	struct goldfish_pipe *pipe;

	if (WARN_ON(id >= dev->pipes_capacity))
		return;

	pipe = dev->pipes[id];
	if (!pipe)
		return;
	pipe->signalled_flags |= flags;

<<<<<<< HEAD
	if (pipe->prev_signalled || pipe->next_signalled
		|| dev->first_signalled_pipe == pipe)
=======
	if (pipe->prev_signalled || pipe->next_signalled ||
		dev->first_signalled_pipe == pipe)
>>>>>>> upstream/android-13
		return;	/* already in the list */
	pipe->next_signalled = dev->first_signalled_pipe;
	if (dev->first_signalled_pipe)
		dev->first_signalled_pipe->prev_signalled = pipe;
	dev->first_signalled_pipe = pipe;
}

static void signalled_pipes_remove_locked(struct goldfish_pipe_dev *dev,
<<<<<<< HEAD
	struct goldfish_pipe *pipe) {
=======
					  struct goldfish_pipe *pipe)
{
>>>>>>> upstream/android-13
	if (pipe->prev_signalled)
		pipe->prev_signalled->next_signalled = pipe->next_signalled;
	if (pipe->next_signalled)
		pipe->next_signalled->prev_signalled = pipe->prev_signalled;
	if (pipe == dev->first_signalled_pipe)
		dev->first_signalled_pipe = pipe->next_signalled;
	pipe->prev_signalled = NULL;
	pipe->next_signalled = NULL;
}

static struct goldfish_pipe *signalled_pipes_pop_front(
		struct goldfish_pipe_dev *dev, int *wakes)
{
	struct goldfish_pipe *pipe;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);

	pipe = dev->first_signalled_pipe;
	if (pipe) {
		*wakes = pipe->signalled_flags;
		pipe->signalled_flags = 0;
		/*
		 * This is an optimized version of
		 * signalled_pipes_remove_locked()
		 * - We want to make it as fast as possible to
		 * wake the sleeping pipe operations faster.
		 */
		dev->first_signalled_pipe = pipe->next_signalled;
		if (dev->first_signalled_pipe)
			dev->first_signalled_pipe->prev_signalled = NULL;
		pipe->next_signalled = NULL;
	}

	spin_unlock_irqrestore(&dev->lock, flags);
	return pipe;
}

<<<<<<< HEAD
static void goldfish_interrupt_task(unsigned long unused)
{
	struct goldfish_pipe_dev *dev = pipe_dev;
	/* Iterate over the signalled pipes and wake them one by one */
=======
static irqreturn_t goldfish_interrupt_task(int irq, void *dev_addr)
{
	/* Iterate over the signalled pipes and wake them one by one */
	struct goldfish_pipe_dev *dev = dev_addr;
>>>>>>> upstream/android-13
	struct goldfish_pipe *pipe;
	int wakes;

	while ((pipe = signalled_pipes_pop_front(dev, &wakes)) != NULL) {
		if (wakes & PIPE_WAKE_CLOSED) {
			pipe->flags = 1 << BIT_CLOSED_ON_HOST;
		} else {
			if (wakes & PIPE_WAKE_READ)
				clear_bit(BIT_WAKE_ON_READ, &pipe->flags);
			if (wakes & PIPE_WAKE_WRITE)
				clear_bit(BIT_WAKE_ON_WRITE, &pipe->flags);
		}
		/*
		 * wake_up_interruptible() implies a write barrier, so don't
		 * explicitly add another one here.
		 */
		wake_up_interruptible(&pipe->wake_queue);
	}
<<<<<<< HEAD
}
static DECLARE_TASKLET(goldfish_interrupt_tasklet, goldfish_interrupt_task, 0);

/*
 * The general idea of the interrupt handling:
=======
	return IRQ_HANDLED;
}

static void goldfish_pipe_device_deinit(struct platform_device *pdev,
					struct goldfish_pipe_dev *dev);

/*
 * The general idea of the (threaded) interrupt handling:
>>>>>>> upstream/android-13
 *
 *  1. device raises an interrupt if there's at least one signalled pipe
 *  2. IRQ handler reads the signalled pipes and their count from the device
 *  3. device writes them into a shared buffer and returns the count
 *      it only resets the IRQ if it has returned all signalled pipes,
 *      otherwise it leaves it raised, so IRQ handler will be called
 *      again for the next chunk
 *  4. IRQ handler adds all returned pipes to the device's signalled pipes list
<<<<<<< HEAD
 *  5. IRQ handler launches a tasklet to process the signalled pipes from the
 *      list in a separate context
=======
 *  5. IRQ handler defers processing the signalled pipes from the list in a
 *      separate context
>>>>>>> upstream/android-13
 */
static irqreturn_t goldfish_pipe_interrupt(int irq, void *dev_id)
{
	u32 count;
	u32 i;
	unsigned long flags;
	struct goldfish_pipe_dev *dev = dev_id;

<<<<<<< HEAD
	if (dev != pipe_dev)
=======
	if (dev->magic != &goldfish_pipe_device_deinit)
>>>>>>> upstream/android-13
		return IRQ_NONE;

	/* Request the signalled pipes from the device */
	spin_lock_irqsave(&dev->lock, flags);

	count = readl(dev->base + PIPE_REG_GET_SIGNALLED);
	if (count == 0) {
		spin_unlock_irqrestore(&dev->lock, flags);
		return IRQ_NONE;
	}
	if (count > MAX_SIGNALLED_PIPES)
		count = MAX_SIGNALLED_PIPES;

	for (i = 0; i < count; ++i)
		signalled_pipes_add_locked(dev,
			dev->buffers->signalled_pipe_buffers[i].id,
			dev->buffers->signalled_pipe_buffers[i].flags);

	spin_unlock_irqrestore(&dev->lock, flags);

<<<<<<< HEAD
	tasklet_schedule(&goldfish_interrupt_tasklet);
	return IRQ_HANDLED;
=======
	return IRQ_WAKE_THREAD;
>>>>>>> upstream/android-13
}

static int get_free_pipe_id_locked(struct goldfish_pipe_dev *dev)
{
	int id;

	for (id = 0; id < dev->pipes_capacity; ++id)
		if (!dev->pipes[id])
			return id;

	{
<<<<<<< HEAD
		/* Reallocate the array */
=======
		/* Reallocate the array.
		 * Since get_free_pipe_id_locked runs with interrupts disabled,
		 * we don't want to make calls that could lead to sleep.
		 */
>>>>>>> upstream/android-13
		u32 new_capacity = 2 * dev->pipes_capacity;
		struct goldfish_pipe **pipes =
			kcalloc(new_capacity, sizeof(*pipes), GFP_ATOMIC);
		if (!pipes)
			return -ENOMEM;
		memcpy(pipes, dev->pipes, sizeof(*pipes) * dev->pipes_capacity);
		kfree(dev->pipes);
		dev->pipes = pipes;
		id = dev->pipes_capacity;
		dev->pipes_capacity = new_capacity;
	}
	return id;
}

<<<<<<< HEAD
=======
/* A helper function to get the instance of goldfish_pipe_dev from file */
static struct goldfish_pipe_dev *to_goldfish_pipe_dev(struct file *file)
{
	struct miscdevice *miscdev = file->private_data;

	return container_of(miscdev, struct goldfish_pipe_dev, miscdev);
}

>>>>>>> upstream/android-13
/**
 *	goldfish_pipe_open - open a channel to the AVD
 *	@inode: inode of device
 *	@file: file struct of opener
 *
 *	Create a new pipe link between the emulator and the use application.
 *	Each new request produces a new pipe.
 *
 *	Note: we use the pipe ID as a mux. All goldfish emulations are 32bit
 *	right now so this is fine. A move to 64bit will need this addressing
 */
static int goldfish_pipe_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct goldfish_pipe_dev *dev = pipe_dev;
=======
	struct goldfish_pipe_dev *dev = to_goldfish_pipe_dev(file);
>>>>>>> upstream/android-13
	unsigned long flags;
	int id;
	int status;

	/* Allocate new pipe kernel object */
	struct goldfish_pipe *pipe = kzalloc(sizeof(*pipe), GFP_KERNEL);
<<<<<<< HEAD
	if (pipe == NULL)
=======

	if (!pipe)
>>>>>>> upstream/android-13
		return -ENOMEM;

	pipe->dev = dev;
	mutex_init(&pipe->lock);
	init_waitqueue_head(&pipe->wake_queue);

	/*
	 * Command buffer needs to be allocated on its own page to make sure
	 * it is physically contiguous in host's address space.
	 */
<<<<<<< HEAD
=======
	BUILD_BUG_ON(sizeof(struct goldfish_pipe_command) > PAGE_SIZE);
>>>>>>> upstream/android-13
	pipe->command_buffer =
		(struct goldfish_pipe_command *)__get_free_page(GFP_KERNEL);
	if (!pipe->command_buffer) {
		status = -ENOMEM;
		goto err_pipe;
	}

	spin_lock_irqsave(&dev->lock, flags);

	id = get_free_pipe_id_locked(dev);
	if (id < 0) {
		status = id;
		goto err_id_locked;
	}

	dev->pipes[id] = pipe;
	pipe->id = id;
	pipe->command_buffer->id = id;

	/* Now tell the emulator we're opening a new pipe. */
	dev->buffers->open_command_params.rw_params_max_count =
			MAX_BUFFERS_PER_COMMAND;
	dev->buffers->open_command_params.command_buffer_ptr =
			(u64)(unsigned long)__pa(pipe->command_buffer);
<<<<<<< HEAD
	status = goldfish_cmd_locked(pipe, PIPE_CMD_OPEN);
=======
	status = goldfish_pipe_cmd_locked(pipe, PIPE_CMD_OPEN);
>>>>>>> upstream/android-13
	spin_unlock_irqrestore(&dev->lock, flags);
	if (status < 0)
		goto err_cmd;
	/* All is done, save the pipe into the file's private data field */
	file->private_data = pipe;
	return 0;

err_cmd:
	spin_lock_irqsave(&dev->lock, flags);
	dev->pipes[id] = NULL;
err_id_locked:
	spin_unlock_irqrestore(&dev->lock, flags);
	free_page((unsigned long)pipe->command_buffer);
err_pipe:
	kfree(pipe);
	return status;
}

static int goldfish_pipe_release(struct inode *inode, struct file *filp)
{
	unsigned long flags;
	struct goldfish_pipe *pipe = filp->private_data;
	struct goldfish_pipe_dev *dev = pipe->dev;

	/* The guest is closing the channel, so tell the emulator right now */
<<<<<<< HEAD
	(void)goldfish_cmd(pipe, PIPE_CMD_CLOSE);
=======
	goldfish_pipe_cmd(pipe, PIPE_CMD_CLOSE);
>>>>>>> upstream/android-13

	spin_lock_irqsave(&dev->lock, flags);
	dev->pipes[pipe->id] = NULL;
	signalled_pipes_remove_locked(dev, pipe);
	spin_unlock_irqrestore(&dev->lock, flags);

	filp->private_data = NULL;
	free_page((unsigned long)pipe->command_buffer);
	kfree(pipe);
	return 0;
}

static const struct file_operations goldfish_pipe_fops = {
	.owner = THIS_MODULE,
	.read = goldfish_pipe_read,
	.write = goldfish_pipe_write,
	.poll = goldfish_pipe_poll,
	.open = goldfish_pipe_open,
	.release = goldfish_pipe_release,
};

<<<<<<< HEAD
static struct miscdevice goldfish_pipe_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "goldfish_pipe",
	.fops = &goldfish_pipe_fops,
};

static int goldfish_pipe_device_init(struct platform_device *pdev)
{
	char *page;
	struct goldfish_pipe_dev *dev = pipe_dev;
	int err = devm_request_irq(&pdev->dev, dev->irq,
				goldfish_pipe_interrupt,
				IRQF_SHARED, "goldfish_pipe", dev);
=======
static void init_miscdevice(struct miscdevice *miscdev)
{
	memset(miscdev, 0, sizeof(*miscdev));

	miscdev->minor = MISC_DYNAMIC_MINOR;
	miscdev->name = "goldfish_pipe";
	miscdev->fops = &goldfish_pipe_fops;
}

static void write_pa_addr(void *addr, void __iomem *portl, void __iomem *porth)
{
	const unsigned long paddr = __pa(addr);

	writel(upper_32_bits(paddr), porth);
	writel(lower_32_bits(paddr), portl);
}

static int goldfish_pipe_device_init(struct platform_device *pdev,
				     struct goldfish_pipe_dev *dev)
{
	int err;

	err = devm_request_threaded_irq(&pdev->dev, dev->irq,
					goldfish_pipe_interrupt,
					goldfish_interrupt_task,
					IRQF_SHARED, "goldfish_pipe", dev);
>>>>>>> upstream/android-13
	if (err) {
		dev_err(&pdev->dev, "unable to allocate IRQ for v2\n");
		return err;
	}

<<<<<<< HEAD
	err = misc_register(&goldfish_pipe_dev);
=======
	init_miscdevice(&dev->miscdev);
	err = misc_register(&dev->miscdev);
>>>>>>> upstream/android-13
	if (err) {
		dev_err(&pdev->dev, "unable to register v2 device\n");
		return err;
	}

<<<<<<< HEAD
	dev->first_signalled_pipe = NULL;
	dev->pipes_capacity = INITIAL_PIPES_CAPACITY;
	dev->pipes = kcalloc(dev->pipes_capacity, sizeof(*dev->pipes),
					GFP_KERNEL);
	if (!dev->pipes)
		return -ENOMEM;
=======
	dev->pdev_dev = &pdev->dev;
	dev->first_signalled_pipe = NULL;
	dev->pipes_capacity = INITIAL_PIPES_CAPACITY;
	dev->pipes = kcalloc(dev->pipes_capacity, sizeof(*dev->pipes),
			     GFP_KERNEL);
	if (!dev->pipes) {
		misc_deregister(&dev->miscdev);
		return -ENOMEM;
	}
>>>>>>> upstream/android-13

	/*
	 * We're going to pass two buffers, open_command_params and
	 * signalled_pipe_buffers, to the host. This means each of those buffers
	 * needs to be contained in a single physical page. The easiest choice
	 * is to just allocate a page and place the buffers in it.
	 */
<<<<<<< HEAD
	if (WARN_ON(sizeof(*dev->buffers) > PAGE_SIZE))
		return -ENOMEM;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page) {
		kfree(dev->pipes);
		return -ENOMEM;
	}
	dev->buffers = (struct goldfish_pipe_dev_buffers *)page;

	/* Send the buffer addresses to the host */
	{
		u64 paddr = __pa(&dev->buffers->signalled_pipe_buffers);

		writel((u32)(unsigned long)(paddr >> 32),
			dev->base + PIPE_REG_SIGNAL_BUFFER_HIGH);
		writel((u32)(unsigned long)paddr,
			dev->base + PIPE_REG_SIGNAL_BUFFER);
		writel((u32)MAX_SIGNALLED_PIPES,
			dev->base + PIPE_REG_SIGNAL_BUFFER_COUNT);

		paddr = __pa(&dev->buffers->open_command_params);
		writel((u32)(unsigned long)(paddr >> 32),
			dev->base + PIPE_REG_OPEN_BUFFER_HIGH);
		writel((u32)(unsigned long)paddr,
			dev->base + PIPE_REG_OPEN_BUFFER);
	}
	return 0;
}

static void goldfish_pipe_device_deinit(struct platform_device *pdev)
{
	struct goldfish_pipe_dev *dev = pipe_dev;

	misc_deregister(&goldfish_pipe_dev);
=======
	BUILD_BUG_ON(sizeof(struct goldfish_pipe_dev_buffers) > PAGE_SIZE);
	dev->buffers = (struct goldfish_pipe_dev_buffers *)
		__get_free_page(GFP_KERNEL);
	if (!dev->buffers) {
		kfree(dev->pipes);
		misc_deregister(&dev->miscdev);
		return -ENOMEM;
	}

	/* Send the buffer addresses to the host */
	write_pa_addr(&dev->buffers->signalled_pipe_buffers,
		      dev->base + PIPE_REG_SIGNAL_BUFFER,
		      dev->base + PIPE_REG_SIGNAL_BUFFER_HIGH);

	writel(MAX_SIGNALLED_PIPES,
	       dev->base + PIPE_REG_SIGNAL_BUFFER_COUNT);

	write_pa_addr(&dev->buffers->open_command_params,
		      dev->base + PIPE_REG_OPEN_BUFFER,
		      dev->base + PIPE_REG_OPEN_BUFFER_HIGH);

	platform_set_drvdata(pdev, dev);
	return 0;
}

static void goldfish_pipe_device_deinit(struct platform_device *pdev,
					struct goldfish_pipe_dev *dev)
{
	misc_deregister(&dev->miscdev);
>>>>>>> upstream/android-13
	kfree(dev->pipes);
	free_page((unsigned long)dev->buffers);
}

static int goldfish_pipe_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	int err;
	struct resource *r;
	struct goldfish_pipe_dev *dev = pipe_dev;

	if (WARN_ON(sizeof(struct goldfish_pipe_command) > PAGE_SIZE))
		return -ENOMEM;

	/* not thread safe, but this should not happen */
	WARN_ON(dev->base != NULL);

	spin_lock_init(&dev->lock);

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (r == NULL || resource_size(r) < PAGE_SIZE) {
=======
	struct resource *r;
	struct goldfish_pipe_dev *dev;

	dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->magic = &goldfish_pipe_device_deinit;
	spin_lock_init(&dev->lock);

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r || resource_size(r) < PAGE_SIZE) {
>>>>>>> upstream/android-13
		dev_err(&pdev->dev, "can't allocate i/o page\n");
		return -EINVAL;
	}
	dev->base = devm_ioremap(&pdev->dev, r->start, PAGE_SIZE);
<<<<<<< HEAD
	if (dev->base == NULL) {
=======
	if (!dev->base) {
>>>>>>> upstream/android-13
		dev_err(&pdev->dev, "ioremap failed\n");
		return -EINVAL;
	}

	r = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
<<<<<<< HEAD
	if (r == NULL) {
		err = -EINVAL;
		goto error;
	}
=======
	if (!r)
		return -EINVAL;

>>>>>>> upstream/android-13
	dev->irq = r->start;

	/*
	 * Exchange the versions with the host device
	 *
	 * Note: v1 driver used to not report its version, so we write it before
	 *  reading device version back: this allows the host implementation to
	 *  detect the old driver (if there was no version write before read).
	 */
<<<<<<< HEAD
	writel((u32)PIPE_DRIVER_VERSION, dev->base + PIPE_REG_VERSION);
=======
	writel(PIPE_DRIVER_VERSION, dev->base + PIPE_REG_VERSION);
>>>>>>> upstream/android-13
	dev->version = readl(dev->base + PIPE_REG_VERSION);
	if (WARN_ON(dev->version < PIPE_CURRENT_DEVICE_VERSION))
		return -EINVAL;

<<<<<<< HEAD
	err = goldfish_pipe_device_init(pdev);
	if (!err)
		return 0;

error:
	dev->base = NULL;
	return err;
=======
	return goldfish_pipe_device_init(pdev, dev);
>>>>>>> upstream/android-13
}

static int goldfish_pipe_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct goldfish_pipe_dev *dev = pipe_dev;
	goldfish_pipe_device_deinit(pdev);
	dev->base = NULL;
=======
	struct goldfish_pipe_dev *dev = platform_get_drvdata(pdev);

	goldfish_pipe_device_deinit(pdev, dev);
>>>>>>> upstream/android-13
	return 0;
}

static const struct acpi_device_id goldfish_pipe_acpi_match[] = {
	{ "GFSH0003", 0 },
	{ },
};
MODULE_DEVICE_TABLE(acpi, goldfish_pipe_acpi_match);

static const struct of_device_id goldfish_pipe_of_match[] = {
	{ .compatible = "google,android-pipe", },
	{},
};
MODULE_DEVICE_TABLE(of, goldfish_pipe_of_match);

static struct platform_driver goldfish_pipe_driver = {
	.probe = goldfish_pipe_probe,
	.remove = goldfish_pipe_remove,
	.driver = {
		.name = "goldfish_pipe",
		.of_match_table = goldfish_pipe_of_match,
		.acpi_match_table = ACPI_PTR(goldfish_pipe_acpi_match),
	}
};

module_platform_driver(goldfish_pipe_driver);
MODULE_AUTHOR("David Turner <digit@google.com>");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
=======
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
