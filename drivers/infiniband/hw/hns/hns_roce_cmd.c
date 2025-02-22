/*
 * Copyright (c) 2016 Hisilicon Limited.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/dmapool.h>
#include <linux/platform_device.h>
#include "hns_roce_common.h"
#include "hns_roce_device.h"
#include "hns_roce_cmd.h"

<<<<<<< HEAD
#define CMD_POLL_TOKEN		0xffff
#define CMD_MAX_NUM		32
#define CMD_TOKEN_MASK		0x1f
=======
#define CMD_POLL_TOKEN 0xffff
#define CMD_MAX_NUM 32
>>>>>>> upstream/android-13

static int hns_roce_cmd_mbox_post_hw(struct hns_roce_dev *hr_dev, u64 in_param,
				     u64 out_param, u32 in_modifier,
				     u8 op_modifier, u16 op, u16 token,
				     int event)
{
<<<<<<< HEAD
	struct hns_roce_cmdq *cmd = &hr_dev->cmd;
	int ret;

	mutex_lock(&cmd->hcr_mutex);
	ret = hr_dev->hw->post_mbox(hr_dev, in_param, out_param, in_modifier,
				    op_modifier, op, token, event);
	mutex_unlock(&cmd->hcr_mutex);

	return ret;
=======
	return hr_dev->hw->post_mbox(hr_dev, in_param, out_param, in_modifier,
				     op_modifier, op, token, event);
>>>>>>> upstream/android-13
}

/* this should be called with "poll_sem" */
static int __hns_roce_cmd_mbox_poll(struct hns_roce_dev *hr_dev, u64 in_param,
				    u64 out_param, unsigned long in_modifier,
				    u8 op_modifier, u16 op,
<<<<<<< HEAD
				    unsigned long timeout)
{
	struct device *dev = hr_dev->dev;
=======
				    unsigned int timeout)
{
>>>>>>> upstream/android-13
	int ret;

	ret = hns_roce_cmd_mbox_post_hw(hr_dev, in_param, out_param,
					in_modifier, op_modifier, op,
					CMD_POLL_TOKEN, 0);
	if (ret) {
<<<<<<< HEAD
		dev_err(dev, "[cmd_poll]hns_roce_cmd_mbox_post_hw failed\n");
		return ret;
	}

	return hr_dev->hw->chk_mbox(hr_dev, timeout);
=======
		dev_err_ratelimited(hr_dev->dev,
				    "failed to post mailbox %x in poll mode, ret = %d.\n",
				    op, ret);
		return ret;
	}

	return hr_dev->hw->poll_mbox_done(hr_dev, timeout);
>>>>>>> upstream/android-13
}

static int hns_roce_cmd_mbox_poll(struct hns_roce_dev *hr_dev, u64 in_param,
				  u64 out_param, unsigned long in_modifier,
<<<<<<< HEAD
				  u8 op_modifier, u16 op, unsigned long timeout)
=======
				  u8 op_modifier, u16 op, unsigned int timeout)
>>>>>>> upstream/android-13
{
	int ret;

	down(&hr_dev->cmd.poll_sem);
	ret = __hns_roce_cmd_mbox_poll(hr_dev, in_param, out_param, in_modifier,
				       op_modifier, op, timeout);
	up(&hr_dev->cmd.poll_sem);

	return ret;
}

void hns_roce_cmd_event(struct hns_roce_dev *hr_dev, u16 token, u8 status,
			u64 out_param)
{
<<<<<<< HEAD
	struct hns_roce_cmd_context
		*context = &hr_dev->cmd.context[token & hr_dev->cmd.token_mask];

	if (token != context->token)
		return;
=======
	struct hns_roce_cmd_context *context =
		&hr_dev->cmd.context[token % hr_dev->cmd.max_cmds];

	if (unlikely(token != context->token)) {
		dev_err_ratelimited(hr_dev->dev,
				    "[cmd] invalid ae token %x,context token is %x!\n",
				    token, context->token);
		return;
	}
>>>>>>> upstream/android-13

	context->result = (status == HNS_ROCE_CMD_SUCCESS) ? 0 : (-EIO);
	context->out_param = out_param;
	complete(&context->done);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(hns_roce_cmd_event);

/* this should be called with "use_events" */
static int __hns_roce_cmd_mbox_wait(struct hns_roce_dev *hr_dev, u64 in_param,
				    u64 out_param, unsigned long in_modifier,
				    u8 op_modifier, u16 op,
				    unsigned long timeout)
=======

static int __hns_roce_cmd_mbox_wait(struct hns_roce_dev *hr_dev, u64 in_param,
				    u64 out_param, unsigned long in_modifier,
				    u8 op_modifier, u16 op,
				    unsigned int timeout)
>>>>>>> upstream/android-13
{
	struct hns_roce_cmdq *cmd = &hr_dev->cmd;
	struct hns_roce_cmd_context *context;
	struct device *dev = hr_dev->dev;
	int ret;

	spin_lock(&cmd->context_lock);
<<<<<<< HEAD
	WARN_ON(cmd->free_head < 0);
	context = &cmd->context[cmd->free_head];
	context->token += cmd->token_mask + 1;
	cmd->free_head = context->next;
	spin_unlock(&cmd->context_lock);

	init_completion(&context->done);
=======

	do {
		context = &cmd->context[cmd->free_head];
		cmd->free_head = context->next;
	} while (context->busy);

	context->busy = 1;
	context->token += cmd->max_cmds;

	spin_unlock(&cmd->context_lock);

	reinit_completion(&context->done);
>>>>>>> upstream/android-13

	ret = hns_roce_cmd_mbox_post_hw(hr_dev, in_param, out_param,
					in_modifier, op_modifier, op,
					context->token, 1);
<<<<<<< HEAD
	if (ret)
		goto out;

	/*
	 * It is timeout when wait_for_completion_timeout return 0
	 * The return value is the time limit set in advance
	 * how many seconds showing
	 */
	if (!wait_for_completion_timeout(&context->done,
					 msecs_to_jiffies(timeout))) {
		dev_err(dev, "[cmd]wait_for_completion_timeout timeout\n");
=======
	if (ret) {
		dev_err_ratelimited(dev,
				    "failed to post mailbox %x in event mode, ret = %d.\n",
				    op, ret);
		goto out;
	}

	if (!wait_for_completion_timeout(&context->done,
					 msecs_to_jiffies(timeout))) {
		dev_err_ratelimited(dev, "[cmd] token %x mailbox %x timeout.\n",
				    context->token, op);
>>>>>>> upstream/android-13
		ret = -EBUSY;
		goto out;
	}

	ret = context->result;
<<<<<<< HEAD
	if (ret) {
		dev_err(dev, "[cmd]event mod cmd process error!err=%d\n", ret);
		goto out;
	}

out:
	spin_lock(&cmd->context_lock);
	context->next = cmd->free_head;
	cmd->free_head = context - cmd->context;
	spin_unlock(&cmd->context_lock);

=======
	if (ret)
		dev_err_ratelimited(dev, "[cmd] token %x mailbox %x error %d\n",
				    context->token, op, ret);

out:
	context->busy = 0;
>>>>>>> upstream/android-13
	return ret;
}

static int hns_roce_cmd_mbox_wait(struct hns_roce_dev *hr_dev, u64 in_param,
				  u64 out_param, unsigned long in_modifier,
<<<<<<< HEAD
				  u8 op_modifier, u16 op, unsigned long timeout)
{
	int ret = 0;

	down(&hr_dev->cmd.event_sem);
	ret = __hns_roce_cmd_mbox_wait(hr_dev, in_param, out_param,
				       in_modifier, op_modifier, op, timeout);
=======
				  u8 op_modifier, u16 op, unsigned int timeout)
{
	int ret;

	down(&hr_dev->cmd.event_sem);
	ret = __hns_roce_cmd_mbox_wait(hr_dev, in_param, out_param, in_modifier,
				       op_modifier, op, timeout);
>>>>>>> upstream/android-13
	up(&hr_dev->cmd.event_sem);

	return ret;
}

int hns_roce_cmd_mbox(struct hns_roce_dev *hr_dev, u64 in_param, u64 out_param,
		      unsigned long in_modifier, u8 op_modifier, u16 op,
<<<<<<< HEAD
		      unsigned long timeout)
{
	if (hr_dev->is_reset)
		return 0;
=======
		      unsigned int timeout)
{
	bool is_busy;

	if (hr_dev->hw->chk_mbox_avail)
		if (!hr_dev->hw->chk_mbox_avail(hr_dev, &is_busy))
			return is_busy ? -EBUSY : 0;
>>>>>>> upstream/android-13

	if (hr_dev->cmd.use_events)
		return hns_roce_cmd_mbox_wait(hr_dev, in_param, out_param,
					      in_modifier, op_modifier, op,
					      timeout);
	else
		return hns_roce_cmd_mbox_poll(hr_dev, in_param, out_param,
					      in_modifier, op_modifier, op,
					      timeout);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(hns_roce_cmd_mbox);

int hns_roce_cmd_init(struct hns_roce_dev *hr_dev)
{
	struct device *dev = hr_dev->dev;

	mutex_init(&hr_dev->cmd.hcr_mutex);
	sema_init(&hr_dev->cmd.poll_sem, 1);
	hr_dev->cmd.use_events = 0;
	hr_dev->cmd.toggle = 1;
	hr_dev->cmd.max_cmds = CMD_MAX_NUM;
	hr_dev->cmd.pool = dma_pool_create("hns_roce_cmd", dev,
=======

int hns_roce_cmd_init(struct hns_roce_dev *hr_dev)
{
	sema_init(&hr_dev->cmd.poll_sem, 1);
	hr_dev->cmd.use_events = 0;
	hr_dev->cmd.max_cmds = CMD_MAX_NUM;
	hr_dev->cmd.pool = dma_pool_create("hns_roce_cmd", hr_dev->dev,
>>>>>>> upstream/android-13
					   HNS_ROCE_MAILBOX_SIZE,
					   HNS_ROCE_MAILBOX_SIZE, 0);
	if (!hr_dev->cmd.pool)
		return -ENOMEM;

	return 0;
}

void hns_roce_cmd_cleanup(struct hns_roce_dev *hr_dev)
{
	dma_pool_destroy(hr_dev->cmd.pool);
}

int hns_roce_cmd_use_events(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_cmdq *hr_cmd = &hr_dev->cmd;
	int i;

<<<<<<< HEAD
	hr_cmd->context = kmalloc_array(hr_cmd->max_cmds,
					sizeof(*hr_cmd->context),
					GFP_KERNEL);
	if (!hr_cmd->context)
		return -ENOMEM;
=======
	hr_cmd->context =
		kcalloc(hr_cmd->max_cmds, sizeof(*hr_cmd->context), GFP_KERNEL);
	if (!hr_cmd->context) {
		hr_dev->cmd_mod = 0;
		return -ENOMEM;
	}
>>>>>>> upstream/android-13

	for (i = 0; i < hr_cmd->max_cmds; ++i) {
		hr_cmd->context[i].token = i;
		hr_cmd->context[i].next = i + 1;
<<<<<<< HEAD
	}

	hr_cmd->context[hr_cmd->max_cmds - 1].next = -1;
=======
		init_completion(&hr_cmd->context[i].done);
	}
	hr_cmd->context[hr_cmd->max_cmds - 1].next = 0;
>>>>>>> upstream/android-13
	hr_cmd->free_head = 0;

	sema_init(&hr_cmd->event_sem, hr_cmd->max_cmds);
	spin_lock_init(&hr_cmd->context_lock);

<<<<<<< HEAD
	hr_cmd->token_mask = CMD_TOKEN_MASK;
	hr_cmd->use_events = 1;

	down(&hr_cmd->poll_sem);

=======
	hr_cmd->use_events = 1;

>>>>>>> upstream/android-13
	return 0;
}

void hns_roce_cmd_use_polling(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_cmdq *hr_cmd = &hr_dev->cmd;
<<<<<<< HEAD
	int i;

	hr_cmd->use_events = 0;

	for (i = 0; i < hr_cmd->max_cmds; ++i)
		down(&hr_cmd->event_sem);

	kfree(hr_cmd->context);
	up(&hr_cmd->poll_sem);
}

struct hns_roce_cmd_mailbox
	*hns_roce_alloc_cmd_mailbox(struct hns_roce_dev *hr_dev)
=======

	kfree(hr_cmd->context);
	hr_cmd->use_events = 0;
}

struct hns_roce_cmd_mailbox *
hns_roce_alloc_cmd_mailbox(struct hns_roce_dev *hr_dev)
>>>>>>> upstream/android-13
{
	struct hns_roce_cmd_mailbox *mailbox;

	mailbox = kmalloc(sizeof(*mailbox), GFP_KERNEL);
	if (!mailbox)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	mailbox->buf = dma_pool_alloc(hr_dev->cmd.pool, GFP_KERNEL,
				      &mailbox->dma);
=======
	mailbox->buf =
		dma_pool_alloc(hr_dev->cmd.pool, GFP_KERNEL, &mailbox->dma);
>>>>>>> upstream/android-13
	if (!mailbox->buf) {
		kfree(mailbox);
		return ERR_PTR(-ENOMEM);
	}

	return mailbox;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(hns_roce_alloc_cmd_mailbox);
=======
>>>>>>> upstream/android-13

void hns_roce_free_cmd_mailbox(struct hns_roce_dev *hr_dev,
			       struct hns_roce_cmd_mailbox *mailbox)
{
	if (!mailbox)
		return;

	dma_pool_free(hr_dev->cmd.pool, mailbox->buf, mailbox->dma);
	kfree(mailbox);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(hns_roce_free_cmd_mailbox);
=======
>>>>>>> upstream/android-13
