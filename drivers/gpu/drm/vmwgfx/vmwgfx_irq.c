// SPDX-License-Identifier: GPL-2.0 OR MIT
/**************************************************************************
 *
 * Copyright 2009-2015 VMware, Inc., Palo Alto, CA., USA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

<<<<<<< HEAD
#include <drm/drmP.h>
=======
#include <linux/pci.h>
#include <linux/sched/signal.h>

>>>>>>> upstream/android-13
#include "vmwgfx_drv.h"

#define VMW_FENCE_WRAP (1 << 24)

<<<<<<< HEAD
=======
static u32 vmw_irqflag_fence_goal(struct vmw_private *vmw)
{
	if ((vmw->capabilities2 & SVGA_CAP2_EXTRA_REGS) != 0)
		return SVGA_IRQFLAG_REG_FENCE_GOAL;
	else
		return SVGA_IRQFLAG_FENCE_GOAL;
}

>>>>>>> upstream/android-13
/**
 * vmw_thread_fn - Deferred (process context) irq handler
 *
 * @irq: irq number
 * @arg: Closure argument. Pointer to a struct drm_device cast to void *
 *
 * This function implements the deferred part of irq processing.
 * The function is guaranteed to run at least once after the
 * vmw_irq_handler has returned with IRQ_WAKE_THREAD.
 *
 */
static irqreturn_t vmw_thread_fn(int irq, void *arg)
{
	struct drm_device *dev = (struct drm_device *)arg;
	struct vmw_private *dev_priv = vmw_priv(dev);
	irqreturn_t ret = IRQ_NONE;

	if (test_and_clear_bit(VMW_IRQTHREAD_FENCE,
			       dev_priv->irqthread_pending)) {
		vmw_fences_update(dev_priv->fman);
		wake_up_all(&dev_priv->fence_queue);
		ret = IRQ_HANDLED;
	}

	if (test_and_clear_bit(VMW_IRQTHREAD_CMDBUF,
			       dev_priv->irqthread_pending)) {
		vmw_cmdbuf_irqthread(dev_priv->cman);
		ret = IRQ_HANDLED;
	}

	return ret;
}

/**
<<<<<<< HEAD
 * vmw_irq_handler irq handler
=======
 * vmw_irq_handler: irq handler
>>>>>>> upstream/android-13
 *
 * @irq: irq number
 * @arg: Closure argument. Pointer to a struct drm_device cast to void *
 *
 * This function implements the quick part of irq processing.
 * The function performs fast actions like clearing the device interrupt
 * flags and also reasonably quick actions like waking processes waiting for
 * FIFO space. Other IRQ actions are deferred to the IRQ thread.
 */
static irqreturn_t vmw_irq_handler(int irq, void *arg)
{
	struct drm_device *dev = (struct drm_device *)arg;
	struct vmw_private *dev_priv = vmw_priv(dev);
	uint32_t status, masked_status;
	irqreturn_t ret = IRQ_HANDLED;

<<<<<<< HEAD
	status = inl(dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);
	masked_status = status & READ_ONCE(dev_priv->irq_mask);

	if (likely(status))
		outl(status, dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);
=======
	status = vmw_irq_status_read(dev_priv);
	masked_status = status & READ_ONCE(dev_priv->irq_mask);

	if (likely(status))
		vmw_irq_status_write(dev_priv, status);
>>>>>>> upstream/android-13

	if (!status)
		return IRQ_NONE;

	if (masked_status & SVGA_IRQFLAG_FIFO_PROGRESS)
		wake_up_all(&dev_priv->fifo_queue);

	if ((masked_status & (SVGA_IRQFLAG_ANY_FENCE |
<<<<<<< HEAD
			      SVGA_IRQFLAG_FENCE_GOAL)) &&
=======
			      vmw_irqflag_fence_goal(dev_priv))) &&
>>>>>>> upstream/android-13
	    !test_and_set_bit(VMW_IRQTHREAD_FENCE, dev_priv->irqthread_pending))
		ret = IRQ_WAKE_THREAD;

	if ((masked_status & (SVGA_IRQFLAG_COMMAND_BUFFER |
			      SVGA_IRQFLAG_ERROR)) &&
	    !test_and_set_bit(VMW_IRQTHREAD_CMDBUF,
			      dev_priv->irqthread_pending))
		ret = IRQ_WAKE_THREAD;

	return ret;
}

static bool vmw_fifo_idle(struct vmw_private *dev_priv, uint32_t seqno)
{

	return (vmw_read(dev_priv, SVGA_REG_BUSY) == 0);
}

<<<<<<< HEAD
void vmw_update_seqno(struct vmw_private *dev_priv,
			 struct vmw_fifo_state *fifo_state)
{
	u32 *fifo_mem = dev_priv->mmio_virt;
	uint32_t seqno = vmw_mmio_read(fifo_mem + SVGA_FIFO_FENCE);

	if (dev_priv->last_read_seqno != seqno) {
		dev_priv->last_read_seqno = seqno;
		vmw_marker_pull(&fifo_state->marker_queue, seqno);
=======
void vmw_update_seqno(struct vmw_private *dev_priv)
{
	uint32_t seqno = vmw_fence_read(dev_priv);

	if (dev_priv->last_read_seqno != seqno) {
		dev_priv->last_read_seqno = seqno;
>>>>>>> upstream/android-13
		vmw_fences_update(dev_priv->fman);
	}
}

bool vmw_seqno_passed(struct vmw_private *dev_priv,
			 uint32_t seqno)
{
<<<<<<< HEAD
	struct vmw_fifo_state *fifo_state;
=======
>>>>>>> upstream/android-13
	bool ret;

	if (likely(dev_priv->last_read_seqno - seqno < VMW_FENCE_WRAP))
		return true;

<<<<<<< HEAD
	fifo_state = &dev_priv->fifo;
	vmw_update_seqno(dev_priv, fifo_state);
	if (likely(dev_priv->last_read_seqno - seqno < VMW_FENCE_WRAP))
		return true;

	if (!(fifo_state->capabilities & SVGA_FIFO_CAP_FENCE) &&
	    vmw_fifo_idle(dev_priv, seqno))
=======
	vmw_update_seqno(dev_priv);
	if (likely(dev_priv->last_read_seqno - seqno < VMW_FENCE_WRAP))
		return true;

	if (!vmw_has_fences(dev_priv) && vmw_fifo_idle(dev_priv, seqno))
>>>>>>> upstream/android-13
		return true;

	/**
	 * Then check if the seqno is higher than what we've actually
	 * emitted. Then the fence is stale and signaled.
	 */

	ret = ((atomic_read(&dev_priv->marker_seq) - seqno)
	       > VMW_FENCE_WRAP);

	return ret;
}

int vmw_fallback_wait(struct vmw_private *dev_priv,
		      bool lazy,
		      bool fifo_idle,
		      uint32_t seqno,
		      bool interruptible,
		      unsigned long timeout)
{
<<<<<<< HEAD
	struct vmw_fifo_state *fifo_state = &dev_priv->fifo;
=======
	struct vmw_fifo_state *fifo_state = dev_priv->fifo;
	bool fifo_down = false;
>>>>>>> upstream/android-13

	uint32_t count = 0;
	uint32_t signal_seq;
	int ret;
	unsigned long end_jiffies = jiffies + timeout;
	bool (*wait_condition)(struct vmw_private *, uint32_t);
	DEFINE_WAIT(__wait);

	wait_condition = (fifo_idle) ? &vmw_fifo_idle :
		&vmw_seqno_passed;

	/**
	 * Block command submission while waiting for idle.
	 */

	if (fifo_idle) {
<<<<<<< HEAD
		down_read(&fifo_state->rwsem);
=======
>>>>>>> upstream/android-13
		if (dev_priv->cman) {
			ret = vmw_cmdbuf_idle(dev_priv->cman, interruptible,
					      10*HZ);
			if (ret)
				goto out_err;
<<<<<<< HEAD
=======
		} else if (fifo_state) {
			down_read(&fifo_state->rwsem);
			fifo_down = true;
>>>>>>> upstream/android-13
		}
	}

	signal_seq = atomic_read(&dev_priv->marker_seq);
	ret = 0;

	for (;;) {
		prepare_to_wait(&dev_priv->fence_queue, &__wait,
				(interruptible) ?
				TASK_INTERRUPTIBLE : TASK_UNINTERRUPTIBLE);
		if (wait_condition(dev_priv, seqno))
			break;
		if (time_after_eq(jiffies, end_jiffies)) {
			DRM_ERROR("SVGA device lockup.\n");
			break;
		}
		if (lazy)
			schedule_timeout(1);
		else if ((++count & 0x0F) == 0) {
			/**
			 * FIXME: Use schedule_hr_timeout here for
			 * newer kernels and lower CPU utilization.
			 */

			__set_current_state(TASK_RUNNING);
			schedule();
			__set_current_state((interruptible) ?
					    TASK_INTERRUPTIBLE :
					    TASK_UNINTERRUPTIBLE);
		}
		if (interruptible && signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
	}
	finish_wait(&dev_priv->fence_queue, &__wait);
<<<<<<< HEAD
	if (ret == 0 && fifo_idle) {
		u32 *fifo_mem = dev_priv->mmio_virt;

		vmw_mmio_write(signal_seq, fifo_mem + SVGA_FIFO_FENCE);
	}
	wake_up_all(&dev_priv->fence_queue);
out_err:
	if (fifo_idle)
=======
	if (ret == 0 && fifo_idle && fifo_state)
		vmw_fence_write(dev_priv, signal_seq);

	wake_up_all(&dev_priv->fence_queue);
out_err:
	if (fifo_down)
>>>>>>> upstream/android-13
		up_read(&fifo_state->rwsem);

	return ret;
}

void vmw_generic_waiter_add(struct vmw_private *dev_priv,
			    u32 flag, int *waiter_count)
{
	spin_lock_bh(&dev_priv->waiter_lock);
	if ((*waiter_count)++ == 0) {
<<<<<<< HEAD
		outl(flag, dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);
=======
		vmw_irq_status_write(dev_priv, flag);
>>>>>>> upstream/android-13
		dev_priv->irq_mask |= flag;
		vmw_write(dev_priv, SVGA_REG_IRQMASK, dev_priv->irq_mask);
	}
	spin_unlock_bh(&dev_priv->waiter_lock);
}

void vmw_generic_waiter_remove(struct vmw_private *dev_priv,
			       u32 flag, int *waiter_count)
{
	spin_lock_bh(&dev_priv->waiter_lock);
	if (--(*waiter_count) == 0) {
		dev_priv->irq_mask &= ~flag;
		vmw_write(dev_priv, SVGA_REG_IRQMASK, dev_priv->irq_mask);
	}
	spin_unlock_bh(&dev_priv->waiter_lock);
}

void vmw_seqno_waiter_add(struct vmw_private *dev_priv)
{
	vmw_generic_waiter_add(dev_priv, SVGA_IRQFLAG_ANY_FENCE,
			       &dev_priv->fence_queue_waiters);
}

void vmw_seqno_waiter_remove(struct vmw_private *dev_priv)
{
	vmw_generic_waiter_remove(dev_priv, SVGA_IRQFLAG_ANY_FENCE,
				  &dev_priv->fence_queue_waiters);
}

void vmw_goal_waiter_add(struct vmw_private *dev_priv)
{
<<<<<<< HEAD
	vmw_generic_waiter_add(dev_priv, SVGA_IRQFLAG_FENCE_GOAL,
=======
	vmw_generic_waiter_add(dev_priv, vmw_irqflag_fence_goal(dev_priv),
>>>>>>> upstream/android-13
			       &dev_priv->goal_queue_waiters);
}

void vmw_goal_waiter_remove(struct vmw_private *dev_priv)
{
<<<<<<< HEAD
	vmw_generic_waiter_remove(dev_priv, SVGA_IRQFLAG_FENCE_GOAL,
				  &dev_priv->goal_queue_waiters);
}

int vmw_wait_seqno(struct vmw_private *dev_priv,
		      bool lazy, uint32_t seqno,
		      bool interruptible, unsigned long timeout)
{
	long ret;
	struct vmw_fifo_state *fifo = &dev_priv->fifo;

	if (likely(dev_priv->last_read_seqno - seqno < VMW_FENCE_WRAP))
		return 0;

	if (likely(vmw_seqno_passed(dev_priv, seqno)))
		return 0;

	vmw_fifo_ping_host(dev_priv, SVGA_SYNC_GENERIC);

	if (!(fifo->capabilities & SVGA_FIFO_CAP_FENCE))
		return vmw_fallback_wait(dev_priv, lazy, true, seqno,
					 interruptible, timeout);

	if (!(dev_priv->capabilities & SVGA_CAP_IRQMASK))
		return vmw_fallback_wait(dev_priv, lazy, false, seqno,
					 interruptible, timeout);

	vmw_seqno_waiter_add(dev_priv);

	if (interruptible)
		ret = wait_event_interruptible_timeout
		    (dev_priv->fence_queue,
		     vmw_seqno_passed(dev_priv, seqno),
		     timeout);
	else
		ret = wait_event_timeout
		    (dev_priv->fence_queue,
		     vmw_seqno_passed(dev_priv, seqno),
		     timeout);

	vmw_seqno_waiter_remove(dev_priv);

	if (unlikely(ret == 0))
		ret = -EBUSY;
	else if (likely(ret > 0))
		ret = 0;

	return ret;
}

=======
	vmw_generic_waiter_remove(dev_priv, vmw_irqflag_fence_goal(dev_priv),
				  &dev_priv->goal_queue_waiters);
}

>>>>>>> upstream/android-13
static void vmw_irq_preinstall(struct drm_device *dev)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	uint32_t status;

<<<<<<< HEAD
	status = inl(dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);
	outl(status, dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);
=======
	status = vmw_irq_status_read(dev_priv);
	vmw_irq_status_write(dev_priv, status);
>>>>>>> upstream/android-13
}

void vmw_irq_uninstall(struct drm_device *dev)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
<<<<<<< HEAD
=======
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> upstream/android-13
	uint32_t status;

	if (!(dev_priv->capabilities & SVGA_CAP_IRQMASK))
		return;

<<<<<<< HEAD
	if (!dev->irq_enabled)
		return;

	vmw_write(dev_priv, SVGA_REG_IRQMASK, 0);

	status = inl(dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);
	outl(status, dev_priv->io_start + VMWGFX_IRQSTATUS_PORT);

	dev->irq_enabled = false;
	free_irq(dev->irq, dev);
=======
	vmw_write(dev_priv, SVGA_REG_IRQMASK, 0);

	status = vmw_irq_status_read(dev_priv);
	vmw_irq_status_write(dev_priv, status);

	free_irq(pdev->irq, dev);
>>>>>>> upstream/android-13
}

/**
 * vmw_irq_install - Install the irq handlers
 *
 * @dev:  Pointer to the drm device.
 * @irq:  The irq number.
 * Return:  Zero if successful. Negative number otherwise.
 */
int vmw_irq_install(struct drm_device *dev, int irq)
{
<<<<<<< HEAD
	int ret;

	if (dev->irq_enabled)
		return -EBUSY;

	vmw_irq_preinstall(dev);

	ret = request_threaded_irq(irq, vmw_irq_handler, vmw_thread_fn,
				   IRQF_SHARED, VMWGFX_DRIVER_NAME, dev);
	if (ret < 0)
		return ret;

	dev->irq_enabled = true;
	dev->irq = irq;

	return ret;
=======
	vmw_irq_preinstall(dev);

	return request_threaded_irq(irq, vmw_irq_handler, vmw_thread_fn,
				    IRQF_SHARED, VMWGFX_DRIVER_NAME, dev);
>>>>>>> upstream/android-13
}
