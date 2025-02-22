// SPDX-License-Identifier: GPL-2.0
/* Copyright 2011 Broadcom Corporation.  All rights reserved. */

<<<<<<< HEAD
#include <linux/device.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <linux/module.h>
#include <linux/completion.h>

#include "bcm2835.h"

/* ---- Include Files -------------------------------------------------------- */

#include "vc_vchi_audioserv_defs.h"

/* ---- Private Constants and Types ------------------------------------------ */

#define BCM2835_AUDIO_STOP           0
#define BCM2835_AUDIO_START          1
#define BCM2835_AUDIO_WRITE          2

/* Logging macros (for remapping to other logging mechanisms, i.e., printf) */
#ifdef AUDIO_DEBUG_ENABLE
#define LOG_ERR(fmt, arg...)   pr_err("%s:%d " fmt, __func__, __LINE__, ##arg)
#define LOG_WARN(fmt, arg...)  pr_info("%s:%d " fmt, __func__, __LINE__, ##arg)
#define LOG_INFO(fmt, arg...)  pr_info("%s:%d " fmt, __func__, __LINE__, ##arg)
#define LOG_DBG(fmt, arg...)   pr_info("%s:%d " fmt, __func__, __LINE__, ##arg)
#else
#define LOG_ERR(fmt, arg...)   pr_err("%s:%d " fmt, __func__, __LINE__, ##arg)
#define LOG_WARN(fmt, arg...)	 no_printk(fmt, ##arg)
#define LOG_INFO(fmt, arg...)	 no_printk(fmt, ##arg)
#define LOG_DBG(fmt, arg...)	 no_printk(fmt, ##arg)
#endif

struct bcm2835_audio_instance {
	unsigned int num_connections;
	VCHI_SERVICE_HANDLE_T vchi_handle[VCHI_MAX_NUM_CONNECTIONS];
=======
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/completion.h>
#include "bcm2835.h"
#include "vc_vchi_audioserv_defs.h"

struct bcm2835_audio_instance {
	struct device *dev;
	unsigned int service_handle;
>>>>>>> upstream/android-13
	struct completion msg_avail_comp;
	struct mutex vchi_mutex;
	struct bcm2835_alsa_stream *alsa_stream;
	int result;
<<<<<<< HEAD
=======
	unsigned int max_packet;
>>>>>>> upstream/android-13
	short peer_version;
};

static bool force_bulk;
<<<<<<< HEAD

/* ---- Private Variables ---------------------------------------------------- */

/* ---- Private Function Prototypes ------------------------------------------ */

/* ---- Private Functions ---------------------------------------------------- */

static int bcm2835_audio_stop_worker(struct bcm2835_alsa_stream *alsa_stream);
static int bcm2835_audio_start_worker(struct bcm2835_alsa_stream *alsa_stream);
static int bcm2835_audio_write_worker(struct bcm2835_alsa_stream *alsa_stream,
				      unsigned int count, void *src);

// Routine to send a message across a service

static int
bcm2835_vchi_msg_queue(VCHI_SERVICE_HANDLE_T handle,
		       void *data,
		       unsigned int size)
{
	return vchi_queue_kernel_message(handle,
					 data,
					 size);
}

static const u32 BCM2835_AUDIO_WRITE_COOKIE1 = ('B' << 24 | 'C' << 16 |
						'M' << 8  | 'A');
static const u32 BCM2835_AUDIO_WRITE_COOKIE2 = ('D' << 24 | 'A' << 16 |
						'T' << 8  | 'A');

struct bcm2835_audio_work {
	struct work_struct my_work;
	struct bcm2835_alsa_stream *alsa_stream;
	int cmd;
	void *src;
	unsigned int count;
};

static void my_wq_function(struct work_struct *work)
{
	struct bcm2835_audio_work *w =
		container_of(work, struct bcm2835_audio_work, my_work);
	int ret = -9;

	switch (w->cmd) {
	case BCM2835_AUDIO_START:
		ret = bcm2835_audio_start_worker(w->alsa_stream);
		break;
	case BCM2835_AUDIO_STOP:
		ret = bcm2835_audio_stop_worker(w->alsa_stream);
		break;
	case BCM2835_AUDIO_WRITE:
		ret = bcm2835_audio_write_worker(w->alsa_stream, w->count,
						 w->src);
		break;
	default:
		LOG_ERR(" Unexpected work: %p:%d\n", w->alsa_stream, w->cmd);
		break;
	}
	kfree((void *)work);
}

int bcm2835_audio_start(struct bcm2835_alsa_stream *alsa_stream)
{
	struct bcm2835_audio_work *work;

	work = kmalloc(sizeof(*work), GFP_ATOMIC);
	/*--- Queue some work (item 1) ---*/
	if (!work) {
		LOG_ERR(" .. Error: NULL work kmalloc\n");
		return -ENOMEM;
	}
	INIT_WORK(&work->my_work, my_wq_function);
	work->alsa_stream = alsa_stream;
	work->cmd = BCM2835_AUDIO_START;
	if (!queue_work(alsa_stream->my_wq, &work->my_work)) {
		kfree(work);
		return -EBUSY;
	}
	return 0;
}

int bcm2835_audio_stop(struct bcm2835_alsa_stream *alsa_stream)
{
	struct bcm2835_audio_work *work;

	work = kmalloc(sizeof(*work), GFP_ATOMIC);
	/*--- Queue some work (item 1) ---*/
	if (!work) {
		LOG_ERR(" .. Error: NULL work kmalloc\n");
		return -ENOMEM;
	}
	INIT_WORK(&work->my_work, my_wq_function);
	work->alsa_stream = alsa_stream;
	work->cmd = BCM2835_AUDIO_STOP;
	if (!queue_work(alsa_stream->my_wq, &work->my_work)) {
		kfree(work);
		return -EBUSY;
	}
	return 0;
}

int bcm2835_audio_write(struct bcm2835_alsa_stream *alsa_stream,
			unsigned int count, void *src)
{
	struct bcm2835_audio_work *work;

	work = kmalloc(sizeof(*work), GFP_ATOMIC);
	/*--- Queue some work (item 1) ---*/
	if (!work) {
		LOG_ERR(" .. Error: NULL work kmalloc\n");
		return -ENOMEM;
	}
	INIT_WORK(&work->my_work, my_wq_function);
	work->alsa_stream = alsa_stream;
	work->cmd = BCM2835_AUDIO_WRITE;
	work->src = src;
	work->count = count;
	if (!queue_work(alsa_stream->my_wq, &work->my_work)) {
		kfree(work);
		return -EBUSY;
	}
	return 0;
}

static void my_workqueue_quit(struct bcm2835_alsa_stream *alsa_stream)
{
	flush_workqueue(alsa_stream->my_wq);
	destroy_workqueue(alsa_stream->my_wq);
	alsa_stream->my_wq = NULL;
}

static void audio_vchi_callback(void *param,
				const VCHI_CALLBACK_REASON_T reason,
				void *msg_handle)
{
	struct bcm2835_audio_instance *instance = param;
	int status;
	int msg_len;
	struct vc_audio_msg m;

	if (reason != VCHI_CALLBACK_MSG_AVAILABLE)
		return;

	if (!instance) {
		LOG_ERR(" .. instance is null\n");
		BUG();
		return;
	}
	if (!instance->vchi_handle[0]) {
		LOG_ERR(" .. instance->vchi_handle[0] is null\n");
		BUG();
		return;
	}
	status = vchi_msg_dequeue(instance->vchi_handle[0],
				  &m, sizeof(m), &msg_len, VCHI_FLAGS_NONE);
	if (m.type == VC_AUDIO_MSG_TYPE_RESULT) {
		LOG_DBG(" .. instance=%p, m.type=VC_AUDIO_MSG_TYPE_RESULT, success=%d\n",
			instance, m.u.result.success);
		instance->result = m.u.result.success;
		complete(&instance->msg_avail_comp);
	} else if (m.type == VC_AUDIO_MSG_TYPE_COMPLETE) {
		struct bcm2835_alsa_stream *alsa_stream = instance->alsa_stream;

		LOG_DBG(" .. instance=%p, m.type=VC_AUDIO_MSG_TYPE_COMPLETE, complete=%d\n",
			instance, m.u.complete.count);
		if (m.u.complete.cookie1 != BCM2835_AUDIO_WRITE_COOKIE1 ||
		    m.u.complete.cookie2 != BCM2835_AUDIO_WRITE_COOKIE2)
			LOG_ERR(" .. response is corrupt\n");
		else if (alsa_stream) {
			atomic_add(m.u.complete.count,
				   &alsa_stream->retrieved);
			bcm2835_playback_fifo(alsa_stream);
		} else {
			LOG_ERR(" .. unexpected alsa_stream=%p\n",
				alsa_stream);
		}
	} else {
		LOG_ERR(" .. unexpected m.type=%d\n", m.type);
	}
}

static struct bcm2835_audio_instance *
vc_vchi_audio_init(VCHI_INSTANCE_T vchi_instance,
		   VCHI_CONNECTION_T **vchi_connections,
		   unsigned int num_connections)
{
	unsigned int i;
	struct bcm2835_audio_instance *instance;
	int status;
	int ret;

	LOG_DBG("%s: start", __func__);

	if (num_connections > VCHI_MAX_NUM_CONNECTIONS) {
		LOG_ERR("%s: unsupported number of connections %u (max=%u)\n",
			__func__, num_connections, VCHI_MAX_NUM_CONNECTIONS);

		return ERR_PTR(-EINVAL);
	}
	/* Allocate memory for this instance */
	instance = kzalloc(sizeof(*instance), GFP_KERNEL);
	if (!instance)
		return ERR_PTR(-ENOMEM);

	instance->num_connections = num_connections;

	/* Create a lock for exclusive, serialized VCHI connection access */
	mutex_init(&instance->vchi_mutex);
	/* Open the VCHI service connections */
	for (i = 0; i < num_connections; i++) {
		SERVICE_CREATION_T params = {
			.version		= VCHI_VERSION_EX(VC_AUDIOSERV_VER, VC_AUDIOSERV_MIN_VER),
			.service_id		= VC_AUDIO_SERVER_NAME,
			.connection		= vchi_connections[i],
			.rx_fifo_size		= 0,
			.tx_fifo_size		= 0,
			.callback		= audio_vchi_callback,
			.callback_param		= instance,
			.want_unaligned_bulk_rx = 1, //TODO: remove VCOS_FALSE
			.want_unaligned_bulk_tx = 1, //TODO: remove VCOS_FALSE
			.want_crc		= 0
		};

		LOG_DBG("%s: about to open %i\n", __func__, i);
		status = vchi_service_open(vchi_instance, &params,
					   &instance->vchi_handle[i]);

		LOG_DBG("%s: opened %i: %p=%d\n", __func__, i, instance->vchi_handle[i], status);
		if (status) {
			LOG_ERR("%s: failed to open VCHI service connection (status=%d)\n",
				__func__, status);
			ret = -EPERM;
			goto err_close_services;
		}
		/* Finished with the service for now */
		vchi_service_release(instance->vchi_handle[i]);
	}

	LOG_DBG("%s: okay\n", __func__);
	return instance;

err_close_services:
	for (i = 0; i < instance->num_connections; i++) {
		LOG_ERR("%s: closing %i: %p\n", __func__, i, instance->vchi_handle[i]);
		if (instance->vchi_handle[i])
			vchi_service_close(instance->vchi_handle[i]);
	}

	kfree(instance);
	LOG_ERR("%s: error\n", __func__);

	return ERR_PTR(ret);
}

static int vc_vchi_audio_deinit(struct bcm2835_audio_instance *instance)
{
	unsigned int i;

	if (!instance) {
		LOG_ERR("%s: invalid handle %p\n", __func__, instance);

		return -1;
	}

	LOG_DBG(" .. about to lock (%d)\n", instance->num_connections);
	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n",
			instance->num_connections);
		return -EINTR;
	}

	/* Close all VCHI service connections */
	for (i = 0; i < instance->num_connections; i++) {
		int status;

		LOG_DBG(" .. %i:closing %p\n", i, instance->vchi_handle[i]);
		vchi_service_use(instance->vchi_handle[i]);

		status = vchi_service_close(instance->vchi_handle[i]);
		if (status) {
			LOG_DBG("%s: failed to close VCHI service connection (status=%d)\n",
				__func__, status);
		}
	}

	mutex_unlock(&instance->vchi_mutex);

	kfree(instance);

	return 0;
}

int bcm2835_new_vchi_ctx(struct bcm2835_vchi_ctx *vchi_ctx)
=======
module_param(force_bulk, bool, 0444);
MODULE_PARM_DESC(force_bulk, "Force use of vchiq bulk for audio");

static void bcm2835_audio_lock(struct bcm2835_audio_instance *instance)
{
	mutex_lock(&instance->vchi_mutex);
	vchiq_use_service(instance->service_handle);
}

static void bcm2835_audio_unlock(struct bcm2835_audio_instance *instance)
{
	vchiq_release_service(instance->service_handle);
	mutex_unlock(&instance->vchi_mutex);
}

static int bcm2835_audio_send_msg_locked(struct bcm2835_audio_instance *instance,
					 struct vc_audio_msg *m, bool wait)
{
	int status;

	if (wait) {
		instance->result = -1;
		init_completion(&instance->msg_avail_comp);
	}

	status = vchiq_queue_kernel_message(instance->service_handle,
					    m, sizeof(*m));
	if (status) {
		dev_err(instance->dev,
			"vchi message queue failed: %d, msg=%d\n",
			status, m->type);
		return -EIO;
	}

	if (wait) {
		if (!wait_for_completion_timeout(&instance->msg_avail_comp,
						 msecs_to_jiffies(10 * 1000))) {
			dev_err(instance->dev,
				"vchi message timeout, msg=%d\n", m->type);
			return -ETIMEDOUT;
		} else if (instance->result) {
			dev_err(instance->dev,
				"vchi message response error:%d, msg=%d\n",
				instance->result, m->type);
			return -EIO;
		}
	}

	return 0;
}

static int bcm2835_audio_send_msg(struct bcm2835_audio_instance *instance,
				  struct vc_audio_msg *m, bool wait)
{
	int err;

	bcm2835_audio_lock(instance);
	err = bcm2835_audio_send_msg_locked(instance, m, wait);
	bcm2835_audio_unlock(instance);
	return err;
}

static int bcm2835_audio_send_simple(struct bcm2835_audio_instance *instance,
				     int type, bool wait)
{
	struct vc_audio_msg m = { .type = type };

	return bcm2835_audio_send_msg(instance, &m, wait);
}

static enum vchiq_status audio_vchi_callback(enum vchiq_reason reason,
					     struct vchiq_header *header,
					     unsigned int handle, void *userdata)
{
	struct bcm2835_audio_instance *instance = vchiq_get_service_userdata(handle);
	struct vc_audio_msg *m;

	if (reason != VCHIQ_MESSAGE_AVAILABLE)
		return VCHIQ_SUCCESS;

	m = (void *)header->data;
	if (m->type == VC_AUDIO_MSG_TYPE_RESULT) {
		instance->result = m->result.success;
		complete(&instance->msg_avail_comp);
	} else if (m->type == VC_AUDIO_MSG_TYPE_COMPLETE) {
		if (m->complete.cookie1 != VC_AUDIO_WRITE_COOKIE1 ||
		    m->complete.cookie2 != VC_AUDIO_WRITE_COOKIE2)
			dev_err(instance->dev, "invalid cookie\n");
		else
			bcm2835_playback_fifo(instance->alsa_stream,
					      m->complete.count);
	} else {
		dev_err(instance->dev, "unexpected callback type=%d\n", m->type);
	}

	vchiq_release_message(handle, header);
	return VCHIQ_SUCCESS;
}

static int
vc_vchi_audio_init(struct vchiq_instance *vchiq_instance,
		   struct bcm2835_audio_instance *instance)
{
	struct vchiq_service_params_kernel params = {
		.version		= VC_AUDIOSERV_VER,
		.version_min		= VC_AUDIOSERV_MIN_VER,
		.fourcc			= VCHIQ_MAKE_FOURCC('A', 'U', 'D', 'S'),
		.callback		= audio_vchi_callback,
		.userdata		= instance,
	};
	int status;

	/* Open the VCHI service connections */
	status = vchiq_open_service(vchiq_instance, &params,
				    &instance->service_handle);

	if (status) {
		dev_err(instance->dev,
			"failed to open VCHI service connection (status=%d)\n",
			status);
		return -EPERM;
	}

	/* Finished with the service for now */
	vchiq_release_service(instance->service_handle);

	return 0;
}

static void vc_vchi_audio_deinit(struct bcm2835_audio_instance *instance)
{
	int status;

	mutex_lock(&instance->vchi_mutex);
	vchiq_use_service(instance->service_handle);

	/* Close all VCHI service connections */
	status = vchiq_close_service(instance->service_handle);
	if (status) {
		dev_err(instance->dev,
			"failed to close VCHI service connection (status=%d)\n",
			status);
	}

	mutex_unlock(&instance->vchi_mutex);
}

int bcm2835_new_vchi_ctx(struct device *dev, struct bcm2835_vchi_ctx *vchi_ctx)
>>>>>>> upstream/android-13
{
	int ret;

	/* Initialize and create a VCHI connection */
<<<<<<< HEAD
	ret = vchi_initialise(&vchi_ctx->vchi_instance);
	if (ret) {
		LOG_ERR("%s: failed to initialise VCHI instance (ret=%d)\n",
			__func__, ret);

		return -EIO;
	}

	ret = vchi_connect(NULL, 0, vchi_ctx->vchi_instance);
	if (ret) {
		LOG_ERR("%s: failed to connect VCHI instance (ret=%d)\n",
			__func__, ret);

		kfree(vchi_ctx->vchi_instance);
		vchi_ctx->vchi_instance = NULL;
=======
	ret = vchiq_initialise(&vchi_ctx->instance);
	if (ret) {
		dev_err(dev, "failed to initialise VCHI instance (ret=%d)\n",
			ret);
		return -EIO;
	}

	ret = vchiq_connect(vchi_ctx->instance);
	if (ret) {
		dev_dbg(dev, "failed to connect VCHI instance (ret=%d)\n",
			ret);

		kfree(vchi_ctx->instance);
		vchi_ctx->instance = NULL;
>>>>>>> upstream/android-13

		return -EIO;
	}

	return 0;
}

void bcm2835_free_vchi_ctx(struct bcm2835_vchi_ctx *vchi_ctx)
{
<<<<<<< HEAD
	/* Close the VCHI connection - it will also free vchi_instance */
	WARN_ON(vchi_disconnect(vchi_ctx->vchi_instance));

	vchi_ctx->vchi_instance = NULL;
}

static int bcm2835_audio_open_connection(struct bcm2835_alsa_stream *alsa_stream)
{
	struct bcm2835_audio_instance *instance =
		(struct bcm2835_audio_instance *)alsa_stream->instance;
	struct bcm2835_vchi_ctx *vhci_ctx = alsa_stream->chip->vchi_ctx;

	LOG_INFO("%s: start\n", __func__);
	BUG_ON(instance);
	if (instance) {
		LOG_ERR("%s: VCHI instance already open (%p)\n",
			__func__, instance);
		instance->alsa_stream = alsa_stream;
		alsa_stream->instance = instance;
		return 0;
	}

	/* Initialize an instance of the audio service */
	instance = vc_vchi_audio_init(vhci_ctx->vchi_instance,
				      &vhci_ctx->vchi_connection, 1);

	if (IS_ERR(instance)) {
		LOG_ERR("%s: failed to initialize audio service\n", __func__);

		/* vchi_instance is retained for use the next time. */
		return PTR_ERR(instance);
	}

	instance->alsa_stream = alsa_stream;
	alsa_stream->instance = instance;

	LOG_DBG(" success !\n");

	return 0;
=======
	/* Close the VCHI connection - it will also free vchi_ctx->instance */
	WARN_ON(vchiq_shutdown(vchi_ctx->instance));

	vchi_ctx->instance = NULL;
>>>>>>> upstream/android-13
}

int bcm2835_audio_open(struct bcm2835_alsa_stream *alsa_stream)
{
<<<<<<< HEAD
	struct bcm2835_audio_instance *instance;
	struct vc_audio_msg m;
	int status;
	int ret;

	alsa_stream->my_wq = alloc_workqueue("my_queue", WQ_HIGHPRI, 1);
	if (!alsa_stream->my_wq)
		return -ENOMEM;

	ret = bcm2835_audio_open_connection(alsa_stream);
	if (ret)
		goto free_wq;

	instance = alsa_stream->instance;
	LOG_DBG(" instance (%p)\n", instance);

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n", instance->num_connections);
		ret = -EINTR;
		goto free_wq;
	}
	vchi_service_use(instance->vchi_handle[0]);

	m.type = VC_AUDIO_MSG_TYPE_OPEN;

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);

		ret = -1;
		goto unlock;
	}

	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);

free_wq:
	if (ret)
		destroy_workqueue(alsa_stream->my_wq);

	return ret;
}

static int bcm2835_audio_set_ctls_chan(struct bcm2835_alsa_stream *alsa_stream,
				       struct bcm2835_chip *chip)
{
	struct vc_audio_msg m;
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int status;
	int ret;

	LOG_INFO(" Setting ALSA dest(%d), volume(%d)\n",
		 chip->dest, chip->volume);

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n",
			instance->num_connections);
		return -EINTR;
	}
	vchi_service_use(instance->vchi_handle[0]);

	instance->result = -1;

	m.type = VC_AUDIO_MSG_TYPE_CONTROL;
	m.u.control.dest = chip->dest;
	m.u.control.volume = chip->volume;

	/* Create the message available completion */
	init_completion(&instance->msg_avail_comp);

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);

		ret = -1;
		goto unlock;
	}

	/* We are expecting a reply from the videocore */
	wait_for_completion(&instance->msg_avail_comp);

	if (instance->result) {
		LOG_ERR("%s: result=%d\n", __func__, instance->result);

		ret = -1;
		goto unlock;
	}

	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);

	return ret;
}

int bcm2835_audio_set_ctls(struct bcm2835_chip *chip)
{
	int i;
	int ret = 0;

	LOG_DBG(" Setting ALSA dest(%d), volume(%d)\n", chip->dest, chip->volume);

	/* change ctls for all substreams */
	for (i = 0; i < MAX_SUBSTREAMS; i++) {
		if (chip->avail_substreams & (1 << i)) {
			if (!chip->alsa_stream[i]) {
				LOG_DBG(" No ALSA stream available?! %i:%p (%x)\n", i, chip->alsa_stream[i], chip->avail_substreams);
				ret = 0;
			} else if (bcm2835_audio_set_ctls_chan(chip->alsa_stream[i], chip) != 0) {
				LOG_ERR("Couldn't set the controls for stream %d\n", i);
				ret = -1;
			} else {
				LOG_DBG(" Controls set for stream %d\n", i);
			}
		}
	}
	return ret;
=======
	struct bcm2835_vchi_ctx *vchi_ctx = alsa_stream->chip->vchi_ctx;
	struct bcm2835_audio_instance *instance;
	int err;

	/* Allocate memory for this instance */
	instance = kzalloc(sizeof(*instance), GFP_KERNEL);
	if (!instance)
		return -ENOMEM;
	mutex_init(&instance->vchi_mutex);
	instance->dev = alsa_stream->chip->dev;
	instance->alsa_stream = alsa_stream;
	alsa_stream->instance = instance;

	err = vc_vchi_audio_init(vchi_ctx->instance,
				 instance);
	if (err < 0)
		goto free_instance;

	err = bcm2835_audio_send_simple(instance, VC_AUDIO_MSG_TYPE_OPEN,
					false);
	if (err < 0)
		goto deinit;

	bcm2835_audio_lock(instance);
	vchiq_get_peer_version(instance->service_handle,
			       &instance->peer_version);
	bcm2835_audio_unlock(instance);
	if (instance->peer_version < 2 || force_bulk)
		instance->max_packet = 0; /* bulk transfer */
	else
		instance->max_packet = 4000;

	return 0;

 deinit:
	vc_vchi_audio_deinit(instance);
 free_instance:
	alsa_stream->instance = NULL;
	kfree(instance);
	return err;
}

int bcm2835_audio_set_ctls(struct bcm2835_alsa_stream *alsa_stream)
{
	struct bcm2835_chip *chip = alsa_stream->chip;
	struct vc_audio_msg m = {};

	m.type = VC_AUDIO_MSG_TYPE_CONTROL;
	m.control.dest = chip->dest;
	if (!chip->mute)
		m.control.volume = CHIP_MIN_VOLUME;
	else
		m.control.volume = alsa2chip(chip->volume);

	return bcm2835_audio_send_msg(alsa_stream->instance, &m, true);
>>>>>>> upstream/android-13
}

int bcm2835_audio_set_params(struct bcm2835_alsa_stream *alsa_stream,
			     unsigned int channels, unsigned int samplerate,
			     unsigned int bps)
{
<<<<<<< HEAD
	struct vc_audio_msg m;
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int status;
	int ret;

	LOG_INFO(" Setting ALSA channels(%d), samplerate(%d), bits-per-sample(%d)\n",
		 channels, samplerate, bps);

	/* resend ctls - alsa_stream may not have been open when first send */
	ret = bcm2835_audio_set_ctls_chan(alsa_stream, alsa_stream->chip);
	if (ret) {
		LOG_ERR(" Alsa controls not supported\n");
		return -EINVAL;
	}

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n", instance->num_connections);
		return -EINTR;
	}
	vchi_service_use(instance->vchi_handle[0]);

	instance->result = -1;

	m.type = VC_AUDIO_MSG_TYPE_CONFIG;
	m.u.config.channels = channels;
	m.u.config.samplerate = samplerate;
	m.u.config.bps = bps;

	/* Create the message available completion */
	init_completion(&instance->msg_avail_comp);

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);

		ret = -1;
		goto unlock;
	}

	/* We are expecting a reply from the videocore */
	wait_for_completion(&instance->msg_avail_comp);

	if (instance->result) {
		LOG_ERR("%s: result=%d", __func__, instance->result);

		ret = -1;
		goto unlock;
	}

	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);

	return ret;
}

int bcm2835_audio_setup(struct bcm2835_alsa_stream *alsa_stream)
{

	return 0;
}

static int bcm2835_audio_start_worker(struct bcm2835_alsa_stream *alsa_stream)
{
	struct vc_audio_msg m;
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int status;
	int ret;

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n",
			instance->num_connections);
		return -EINTR;
	}
	vchi_service_use(instance->vchi_handle[0]);

	m.type = VC_AUDIO_MSG_TYPE_START;

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);

		ret = -1;
		goto unlock;
	}

	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);
	return ret;
}

static int bcm2835_audio_stop_worker(struct bcm2835_alsa_stream *alsa_stream)
{
	struct vc_audio_msg m;
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int status;
	int ret;

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n",
			instance->num_connections);
		return -EINTR;
	}
	vchi_service_use(instance->vchi_handle[0]);

	m.type = VC_AUDIO_MSG_TYPE_STOP;
	m.u.stop.draining = alsa_stream->draining;

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);

		ret = -1;
		goto unlock;
	}

	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);
	return ret;
=======
	struct vc_audio_msg m = {
		 .type = VC_AUDIO_MSG_TYPE_CONFIG,
		 .config.channels = channels,
		 .config.samplerate = samplerate,
		 .config.bps = bps,
	};
	int err;

	/* resend ctls - alsa_stream may not have been open when first send */
	err = bcm2835_audio_set_ctls(alsa_stream);
	if (err)
		return err;

	return bcm2835_audio_send_msg(alsa_stream->instance, &m, true);
}

int bcm2835_audio_start(struct bcm2835_alsa_stream *alsa_stream)
{
	return bcm2835_audio_send_simple(alsa_stream->instance,
					 VC_AUDIO_MSG_TYPE_START, false);
}

int bcm2835_audio_stop(struct bcm2835_alsa_stream *alsa_stream)
{
	return bcm2835_audio_send_simple(alsa_stream->instance,
					 VC_AUDIO_MSG_TYPE_STOP, false);
}

/* FIXME: this doesn't seem working as expected for "draining" */
int bcm2835_audio_drain(struct bcm2835_alsa_stream *alsa_stream)
{
	struct vc_audio_msg m = {
		.type = VC_AUDIO_MSG_TYPE_STOP,
		.stop.draining = 1,
	};

	return bcm2835_audio_send_msg(alsa_stream->instance, &m, false);
>>>>>>> upstream/android-13
}

int bcm2835_audio_close(struct bcm2835_alsa_stream *alsa_stream)
{
<<<<<<< HEAD
	struct vc_audio_msg m;
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int status;
	int ret;

	my_workqueue_quit(alsa_stream);

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n",
			instance->num_connections);
		return -EINTR;
	}
	vchi_service_use(instance->vchi_handle[0]);

	m.type = VC_AUDIO_MSG_TYPE_CLOSE;

	/* Create the message available completion */
	init_completion(&instance->msg_avail_comp);

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);
		ret = -1;
		goto unlock;
	}

	/* We are expecting a reply from the videocore */
	wait_for_completion(&instance->msg_avail_comp);

	if (instance->result) {
		LOG_ERR("%s: failed result (result=%d)\n",
			__func__, instance->result);

		ret = -1;
		goto unlock;
	}

	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);
=======
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int err;

	err = bcm2835_audio_send_simple(alsa_stream->instance,
					VC_AUDIO_MSG_TYPE_CLOSE, true);
>>>>>>> upstream/android-13

	/* Stop the audio service */
	vc_vchi_audio_deinit(instance);
	alsa_stream->instance = NULL;
<<<<<<< HEAD

	return ret;
}

static int bcm2835_audio_write_worker(struct bcm2835_alsa_stream *alsa_stream,
				      unsigned int count, void *src)
{
	struct vc_audio_msg m;
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	int status;
	int ret;

	LOG_INFO(" Writing %d bytes from %p\n", count, src);

	if (mutex_lock_interruptible(&instance->vchi_mutex)) {
		LOG_DBG("Interrupted whilst waiting for lock on (%d)\n",
			instance->num_connections);
		return -EINTR;
	}
	vchi_service_use(instance->vchi_handle[0]);

	if (instance->peer_version == 0 &&
	    vchi_get_peer_version(instance->vchi_handle[0], &instance->peer_version) == 0)
		LOG_DBG("%s: client version %d connected\n", __func__, instance->peer_version);

	m.type = VC_AUDIO_MSG_TYPE_WRITE;
	m.u.write.count = count;
	// old version uses bulk, new version uses control
	m.u.write.max_packet = instance->peer_version < 2 || force_bulk ? 0 : 4000;
	m.u.write.cookie1 = BCM2835_AUDIO_WRITE_COOKIE1;
	m.u.write.cookie2 = BCM2835_AUDIO_WRITE_COOKIE2;
	m.u.write.silence = src == NULL;

	/* Send the message to the videocore */
	status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
					&m, sizeof(m));

	if (status) {
		LOG_ERR("%s: failed on vchi_msg_queue (status=%d)\n",
			__func__, status);

		ret = -1;
		goto unlock;
	}
	if (!m.u.write.silence) {
		if (!m.u.write.max_packet) {
			/* Send the message to the videocore */
			status = vchi_bulk_queue_transmit(instance->vchi_handle[0],
							  src, count,
							  0 * VCHI_FLAGS_BLOCK_UNTIL_QUEUED
							  +
							  1 * VCHI_FLAGS_BLOCK_UNTIL_DATA_READ,
							  NULL);
		} else {
			while (count > 0) {
				int bytes = min_t(int, m.u.write.max_packet, count);

				status = bcm2835_vchi_msg_queue(instance->vchi_handle[0],
								src, bytes);
				src = (char *)src + bytes;
				count -= bytes;
			}
		}
		if (status) {
			LOG_ERR("%s: failed on vchi_bulk_queue_transmit (status=%d)\n",
				__func__, status);

			ret = -1;
			goto unlock;
		}
	}
	ret = 0;

unlock:
	vchi_service_release(instance->vchi_handle[0]);
	mutex_unlock(&instance->vchi_mutex);
	return ret;
}

/**
 * Returns all buffers from arm->vc
 */
void bcm2835_audio_flush_buffers(struct bcm2835_alsa_stream *alsa_stream)
{
}

/**
 * Forces VC to flush(drop) its filled playback buffers and
 * return them the us. (VC->ARM)
 */
void bcm2835_audio_flush_playback_buffers(struct bcm2835_alsa_stream *alsa_stream)
{
}

unsigned int bcm2835_audio_retrieve_buffers(struct bcm2835_alsa_stream *alsa_stream)
{
	unsigned int count = atomic_read(&alsa_stream->retrieved);

	atomic_sub(count, &alsa_stream->retrieved);
	return count;
}

module_param(force_bulk, bool, 0444);
MODULE_PARM_DESC(force_bulk, "Force use of vchiq bulk for audio");
=======
	kfree(instance);

	return err;
}

int bcm2835_audio_write(struct bcm2835_alsa_stream *alsa_stream,
			unsigned int size, void *src)
{
	struct bcm2835_audio_instance *instance = alsa_stream->instance;
	struct vc_audio_msg m = {
		.type = VC_AUDIO_MSG_TYPE_WRITE,
		.write.count = size,
		.write.max_packet = instance->max_packet,
		.write.cookie1 = VC_AUDIO_WRITE_COOKIE1,
		.write.cookie2 = VC_AUDIO_WRITE_COOKIE2,
	};
	unsigned int count;
	int err, status;

	if (!size)
		return 0;

	bcm2835_audio_lock(instance);
	err = bcm2835_audio_send_msg_locked(instance, &m, false);
	if (err < 0)
		goto unlock;

	count = size;
	if (!instance->max_packet) {
		/* Send the message to the videocore */
		status = vchiq_bulk_transmit(instance->service_handle, src,
					     count, NULL,
					     VCHIQ_BULK_MODE_BLOCKING);
	} else {
		while (count > 0) {
			int bytes = min(instance->max_packet, count);

			status = vchiq_queue_kernel_message(instance->service_handle,
							    src, bytes);
			src += bytes;
			count -= bytes;
		}
	}

	if (status) {
		dev_err(instance->dev,
			"failed on %d bytes transfer (status=%d)\n",
			size, status);
		err = -EIO;
	}

 unlock:
	bcm2835_audio_unlock(instance);
	return err;
}
>>>>>>> upstream/android-13
