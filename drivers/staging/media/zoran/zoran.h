<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * zoran - Iomega Buz driver
 *
 * Copyright (C) 1999 Rainer Johanni <Rainer@Johanni.de>
 *
 * based on
 *
 * zoran.0.0.3 Copyright (C) 1998 Dave Perks <dperks@ibm.net>
 *
 * and
 *
 * bttv - Bt848 frame grabber driver
 * Copyright (C) 1996,97,98 Ralph  Metzler (rjkm@thp.uni-koeln.de)
 *                        & Marcus Metzler (mocm@thp.uni-koeln.de)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _BUZ_H_
#define _BUZ_H_

#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>
<<<<<<< HEAD
#include <media/v4l2-fh.h>

struct zoran_sync {
	unsigned long frame;	/* number of buffer that has been free'd */
	unsigned long length;	/* number of code bytes in buffer (capture only) */
	unsigned long seq;	/* frame sequence number */
	struct timeval timestamp;	/* timestamp */
};

=======
#include <media/videobuf2-core.h>
#include <media/videobuf2-v4l2.h>
#include <media/videobuf2-dma-contig.h>

#define ZR_NORM_PAL 0
#define ZR_NORM_NTSC 1
#define ZR_NORM_SECAM 2

struct zr_buffer {
	/* common v4l buffer stuff -- must be first */
	struct vb2_v4l2_buffer          vbuf;
	struct list_head                queue;
};

static inline struct zr_buffer *vb2_to_zr_buffer(struct vb2_buffer *vb)
{
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);

	return container_of(vbuf, struct zr_buffer, vbuf);
}
>>>>>>> upstream/android-13

#define ZORAN_NAME    "ZORAN"	/* name of the device */

#define ZR_DEVNAME(zr) ((zr)->name)

<<<<<<< HEAD
#define   BUZ_MAX_WIDTH   (zr->timing->Wa)
#define   BUZ_MAX_HEIGHT  (zr->timing->Ha)
=======
#define   BUZ_MAX_WIDTH   (zr->timing->wa)
#define   BUZ_MAX_HEIGHT  (zr->timing->ha)
>>>>>>> upstream/android-13
#define   BUZ_MIN_WIDTH    32	/* never display less than 32 pixels */
#define   BUZ_MIN_HEIGHT   24	/* never display less than 24 rows */

#define BUZ_NUM_STAT_COM    4
#define BUZ_MASK_STAT_COM   3

#define BUZ_MAX_FRAME     256	/* Must be a power of 2 */
#define BUZ_MASK_FRAME    255	/* Must be BUZ_MAX_FRAME-1 */

#define BUZ_MAX_INPUT       16

#if VIDEO_MAX_FRAME <= 32
#   define   V4L_MAX_FRAME   32
#elif VIDEO_MAX_FRAME <= 64
#   define   V4L_MAX_FRAME   64
#else
#   error   "Too many video frame buffers to handle"
#endif
#define   V4L_MASK_FRAME   (V4L_MAX_FRAME - 1)

#define MAX_FRAME (BUZ_MAX_FRAME > VIDEO_MAX_FRAME ? BUZ_MAX_FRAME : VIDEO_MAX_FRAME)

#include "zr36057.h"

enum card_type {
	UNKNOWN = -1,

	/* Pinnacle/Miro */
<<<<<<< HEAD
	DC10_old,		/* DC30 like */
	DC10_new,		/* DC10plus like */
	DC10plus,
	DC30,
	DC30plus,
=======
	DC10_OLD,		/* DC30 like */
	DC10_NEW,		/* DC10_PLUS like */
	DC10_PLUS,
	DC30,
	DC30_PLUS,
>>>>>>> upstream/android-13

	/* Linux Media Labs */
	LML33,
	LML33R10,

	/* Iomega */
	BUZ,

	/* AverMedia */
	AVS6EYES,

	/* total number of cards */
	NUM_CARDS
};

enum zoran_codec_mode {
	BUZ_MODE_IDLE,		/* nothing going on */
	BUZ_MODE_MOTION_COMPRESS,	/* grabbing frames */
	BUZ_MODE_MOTION_DECOMPRESS,	/* playing frames */
	BUZ_MODE_STILL_COMPRESS,	/* still frame conversion */
	BUZ_MODE_STILL_DECOMPRESS	/* still frame conversion */
};

<<<<<<< HEAD
enum zoran_buffer_state {
	BUZ_STATE_USER,		/* buffer is owned by application */
	BUZ_STATE_PEND,		/* buffer is queued in pend[] ready to feed to I/O */
	BUZ_STATE_DMA,		/* buffer is queued in dma[] for I/O */
	BUZ_STATE_DONE		/* buffer is ready to return to application */
};

enum zoran_map_mode {
	ZORAN_MAP_MODE_RAW,
	ZORAN_MAP_MODE_JPG_REC,
#define ZORAN_MAP_MODE_JPG ZORAN_MAP_MODE_JPG_REC
=======
enum zoran_map_mode {
	ZORAN_MAP_MODE_NONE,
	ZORAN_MAP_MODE_RAW,
	ZORAN_MAP_MODE_JPG_REC,
>>>>>>> upstream/android-13
	ZORAN_MAP_MODE_JPG_PLAY,
};

enum gpio_type {
	ZR_GPIO_JPEG_SLEEP = 0,
	ZR_GPIO_JPEG_RESET,
	ZR_GPIO_JPEG_FRAME,
	ZR_GPIO_VID_DIR,
	ZR_GPIO_VID_EN,
	ZR_GPIO_VID_RESET,
	ZR_GPIO_CLK_SEL1,
	ZR_GPIO_CLK_SEL2,
	ZR_GPIO_MAX,
};

enum gpcs_type {
	GPCS_JPEG_RESET = 0,
	GPCS_JPEG_START,
	GPCS_MAX,
};

struct zoran_format {
	char *name;
	__u32 fourcc;
	int colorspace;
	int depth;
	__u32 flags;
	__u32 vfespfr;
};
<<<<<<< HEAD
/* flags */
#define ZORAN_FORMAT_COMPRESSED 1<<0
#define ZORAN_FORMAT_OVERLAY    1<<1
#define ZORAN_FORMAT_CAPTURE	1<<2
#define ZORAN_FORMAT_PLAYBACK	1<<3

/* overlay-settings */
struct zoran_overlay_settings {
	int is_set;
	int x, y, width, height;	/* position */
	int clipcount;		/* position and number of clips */
	const struct zoran_format *format;	/* overlay format */
};
=======

/* flags */
#define ZORAN_FORMAT_COMPRESSED BIT(0)
#define ZORAN_FORMAT_OVERLAY BIT(1)
#define ZORAN_FORMAT_CAPTURE BIT(2)
#define ZORAN_FORMAT_PLAYBACK BIT(3)
>>>>>>> upstream/android-13

/* v4l-capture settings */
struct zoran_v4l_settings {
	int width, height, bytesperline;	/* capture size */
	const struct zoran_format *format;	/* capture format */
};

/* jpg-capture/-playback settings */
struct zoran_jpg_settings {
	int decimation;		/* this bit is used to set everything to default */
<<<<<<< HEAD
	int HorDcm, VerDcm, TmpDcm;	/* capture decimation settings (TmpDcm=1 means both fields) */
	int field_per_buff, odd_even;	/* field-settings (odd_even=1 (+TmpDcm=1) means top-field-first) */
=======
	int hor_dcm, ver_dcm, tmp_dcm;	/* capture decimation settings (tmp_dcm=1 means both fields) */
	int field_per_buff, odd_even;	/* field-settings (odd_even=1 (+tmp_dcm=1) means top-field-first) */
>>>>>>> upstream/android-13
	int img_x, img_y, img_width, img_height;	/* crop settings (subframe capture) */
	struct v4l2_jpegcompression jpg_comp;	/* JPEG-specific capture settings */
};

<<<<<<< HEAD
struct zoran_fh;

struct zoran_mapping {
	struct zoran_fh *fh;
	atomic_t count;
};

struct zoran_buffer {
	struct zoran_mapping *map;
	enum zoran_buffer_state state;	/* state: unused/pending/dma/done */
	struct zoran_sync bs;		/* DONE: info to return to application */
	union {
		struct {
			__le32 *frag_tab;	/* addresses of frag table */
			u32 frag_tab_bus;	/* same value cached to save time in ISR */
		} jpg;
		struct {
			char *fbuffer;		/* virtual address of frame buffer */
			unsigned long fbuffer_phys;/* physical address of frame buffer */
			unsigned long fbuffer_bus;/* bus address of frame buffer */
		} v4l;
	};
};

enum zoran_lock_activity {
	ZORAN_FREE,		/* free for use */
	ZORAN_ACTIVE,		/* active but unlocked */
	ZORAN_LOCKED,		/* locked */
};

/* buffer collections */
struct zoran_buffer_col {
	enum zoran_lock_activity active;	/* feature currently in use? */
	unsigned int num_buffers, buffer_size;
	struct zoran_buffer buffer[MAX_FRAME];	/* buffers */
	u8 allocated;		/* Flag if buffers are allocated  */
	u8 need_contiguous;	/* Flag if contiguous buffers are needed */
	/* only applies to jpg buffers, raw buffers are always contiguous */
};

=======
>>>>>>> upstream/android-13
struct zoran;

/* zoran_fh contains per-open() settings */
struct zoran_fh {
	struct v4l2_fh fh;
	struct zoran *zr;
<<<<<<< HEAD

	enum zoran_map_mode map_mode;		/* Flag which bufferset will map by next mmap() */

	struct zoran_overlay_settings overlay_settings;
	u32 *overlay_mask;			/* overlay mask */
	enum zoran_lock_activity overlay_active;/* feature currently in use? */

	struct zoran_buffer_col buffers;	/* buffers' info */

	struct zoran_v4l_settings v4l_settings;	/* structure with a lot of things to play with */
	struct zoran_jpg_settings jpg_settings;	/* structure with a lot of things to play with */
=======
>>>>>>> upstream/android-13
};

struct card_info {
	enum card_type type;
	char name[32];
	const char *i2c_decoder;	/* i2c decoder device */
	const unsigned short *addrs_decoder;
	const char *i2c_encoder;	/* i2c encoder device */
	const unsigned short *addrs_encoder;
	u16 video_vfe, video_codec;			/* videocodec types */
	u16 audio_chip;					/* audio type */

	int inputs;		/* number of video inputs */
	struct input {
		int muxsel;
		char name[32];
	} input[BUZ_MAX_INPUT];

	v4l2_std_id norms;
<<<<<<< HEAD
	struct tvnorm *tvn[3];	/* supported TV norms */
=======
	const struct tvnorm *tvn[3];	/* supported TV norms */
>>>>>>> upstream/android-13

	u32 jpeg_int;		/* JPEG interrupt */
	u32 vsync_int;		/* VSYNC interrupt */
	s8 gpio[ZR_GPIO_MAX];
	u8 gpcs[GPCS_MAX];

	struct vfe_polarity vfe_pol;
	u8 gpio_pol[ZR_GPIO_MAX];

	/* is the /GWS line connected? */
	u8 gws_not_connected;

	/* avs6eyes mux setting */
	u8 input_mux;

<<<<<<< HEAD
	void (*init) (struct zoran * zr);
=======
	void (*init)(struct zoran *zr);
>>>>>>> upstream/android-13
};

struct zoran {
	struct v4l2_device v4l2_dev;
	struct v4l2_ctrl_handler hdl;
	struct video_device *video_dev;
<<<<<<< HEAD
=======
	struct vb2_queue vq;
>>>>>>> upstream/android-13

	struct i2c_adapter i2c_adapter;	/* */
	struct i2c_algo_bit_data i2c_algo;	/* */
	u32 i2cbr;

	struct v4l2_subdev *decoder;	/* video decoder sub-device */
	struct v4l2_subdev *encoder;	/* video encoder sub-device */

	struct videocodec *codec;	/* video codec */
	struct videocodec *vfe;	/* video front end */

	struct mutex lock;	/* file ops serialize lock */

	u8 initialized;		/* flag if zoran has been correctly initialized */
<<<<<<< HEAD
	int user;		/* number of current users */
	struct card_info card;
	struct tvnorm *timing;
=======
	struct card_info card;
	const struct tvnorm *timing;
>>>>>>> upstream/android-13

	unsigned short id;	/* number of this device */
	char name[32];		/* name of this device */
	struct pci_dev *pci_dev;	/* PCI device */
	unsigned char revision;	/* revision of zr36057 */
	unsigned char __iomem *zr36057_mem;/* pointer to mapped IO memory */

	spinlock_t spinlock;	/* Spinlock */

	/* Video for Linux parameters */
	int input;	/* card's norm and input */
	v4l2_std_id norm;

	/* Current buffer params */
<<<<<<< HEAD
	void    *vbuf_base;
	int     vbuf_height, vbuf_width;
	int     vbuf_depth;
	int     vbuf_bytesperline;

	struct zoran_overlay_settings overlay_settings;
	u32 *overlay_mask;	/* overlay mask */
	enum zoran_lock_activity overlay_active;	/* feature currently in use? */

	wait_queue_head_t v4l_capq;

	int v4l_overlay_active;	/* Overlay grab is activated */
	int v4l_memgrab_active;	/* Memory grab is activated */

	int v4l_grab_frame;	/* Frame number being currently grabbed */
#define NO_GRAB_ACTIVE (-1)
	unsigned long v4l_grab_seq;	/* Number of frames grabbed */
	struct zoran_v4l_settings v4l_settings;	/* structure with a lot of things to play with */

	/* V4L grab queue of frames pending */
	unsigned long v4l_pend_head;
	unsigned long v4l_pend_tail;
	unsigned long v4l_sync_tail;
	int v4l_pend[V4L_MAX_FRAME];
	struct zoran_buffer_col v4l_buffers;	/* V4L buffers' info */

=======
	unsigned int buffer_size;

	struct zoran_v4l_settings v4l_settings;	/* structure with a lot of things to play with */

>>>>>>> upstream/android-13
	/* Buz MJPEG parameters */
	enum zoran_codec_mode codec_mode;	/* status of codec */
	struct zoran_jpg_settings jpg_settings;	/* structure with a lot of things to play with */

<<<<<<< HEAD
	wait_queue_head_t jpg_capq;	/* wait here for grab to finish */

=======
>>>>>>> upstream/android-13
	/* grab queue counts/indices, mask with BUZ_MASK_STAT_COM before using as index */
	/* (dma_head - dma_tail) is number active in DMA, must be <= BUZ_NUM_STAT_COM */
	/* (value & BUZ_MASK_STAT_COM) corresponds to index in stat_com table */
	unsigned long jpg_que_head;	/* Index where to put next buffer which is queued */
<<<<<<< HEAD
	unsigned long jpg_dma_head;	/* Index of next buffer which goes into stat_com  */
	unsigned long jpg_dma_tail;	/* Index of last buffer in stat_com               */
	unsigned long jpg_que_tail;	/* Index of last buffer in queue                  */
	unsigned long jpg_seq_num;	/* count of frames since grab/play started        */
	unsigned long jpg_err_seq;	/* last seq_num before error                      */
	unsigned long jpg_err_shift;
	unsigned long jpg_queued_num;	/* count of frames queued since grab/play started */
=======
	unsigned long jpg_dma_head;	/* Index of next buffer which goes into stat_com */
	unsigned long jpg_dma_tail;	/* Index of last buffer in stat_com */
	unsigned long jpg_que_tail;	/* Index of last buffer in queue */
	unsigned long jpg_seq_num;	/* count of frames since grab/play started */
	unsigned long jpg_err_seq;	/* last seq_num before error */
	unsigned long jpg_err_shift;
	unsigned long jpg_queued_num;	/* count of frames queued since grab/play started */
	unsigned long vbseq;
>>>>>>> upstream/android-13

	/* zr36057's code buffer table */
	__le32 *stat_com;		/* stat_com[i] is indexed by dma_head/tail & BUZ_MASK_STAT_COM */

<<<<<<< HEAD
	/* (value & BUZ_MASK_FRAME) corresponds to index in pend[] queue */
	int jpg_pend[BUZ_MAX_FRAME];

	/* array indexed by frame number */
	struct zoran_buffer_col jpg_buffers;	/* MJPEG buffers' info */

	/* Additional stuff for testing */
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *zoran_proc;
#else
	void *zoran_proc;
#endif
	int testing;
	int jpeg_error;
	int intr_counter_GIRQ1;
	int intr_counter_GIRQ0;
	int intr_counter_CodRepIRQ;
	int intr_counter_JPEGRepIRQ;
	int field_counter;
	int IRQ1_in;
	int IRQ1_out;
	int JPEG_in;
	int JPEG_out;
	int JPEG_0;
	int JPEG_1;
	int END_event_missed;
	int JPEG_missed;
	int JPEG_error;
	int num_errors;
	int JPEG_max_missed;
	int JPEG_min_missed;

	u32 last_isr;
	unsigned long frame_num;

	wait_queue_head_t test_q;
=======
	/* Additional stuff for testing */
	unsigned int ghost_int;
	int intr_counter_GIRQ1;
	int intr_counter_GIRQ0;
	int intr_counter_cod_rep_irq;
	int intr_counter_jpeg_rep_irq;
	int field_counter;
	int irq1_in;
	int irq1_out;
	int jpeg_in;
	int jpeg_out;
	int JPEG_0;
	int JPEG_1;
	int end_event_missed;
	int jpeg_missed;
	int jpeg_error;
	int num_errors;
	int jpeg_max_missed;
	int jpeg_min_missed;
	unsigned int prepared;
	unsigned int queued;

	u32 last_isr;
	unsigned long frame_num;
	int running;
	int buf_in_reserve;

	dma_addr_t p_sc;
	__le32 *stat_comb;
	dma_addr_t p_scb;
	enum zoran_map_mode map_mode;
	struct list_head queued_bufs;
	spinlock_t queued_bufs_lock; /* Protects queued_bufs */
	struct zr_buffer *inuse[BUZ_NUM_STAT_COM * 2];
>>>>>>> upstream/android-13
};

static inline struct zoran *to_zoran(struct v4l2_device *v4l2_dev)
{
	return container_of(v4l2_dev, struct zoran, v4l2_dev);
}

/* There was something called _ALPHA_BUZ that used the PCI address instead of
 * the kernel iomapped address for btread/btwrite.  */
<<<<<<< HEAD
#define btwrite(dat,adr)    writel((dat), zr->zr36057_mem+(adr))
#define btread(adr)         readl(zr->zr36057_mem+(adr))

#define btand(dat,adr)      btwrite((dat) & btread(adr), adr)
#define btor(dat,adr)       btwrite((dat) | btread(adr), adr)
#define btaor(dat,mask,adr) btwrite((dat) | ((mask) & btread(adr)), adr)

#endif
=======
#define btwrite(dat, adr)    writel((dat), zr->zr36057_mem + (adr))
#define btread(adr)         readl(zr->zr36057_mem + (adr))

#define btand(dat, adr)      btwrite((dat) & btread(adr), adr)
#define btor(dat, adr)       btwrite((dat) | btread(adr), adr)
#define btaor(dat, mask, adr) btwrite((dat) | ((mask) & btread(adr)), adr)

#endif

int zoran_queue_init(struct zoran *zr, struct vb2_queue *vq, int dir);
void zoran_queue_exit(struct zoran *zr);
int zr_set_buf(struct zoran *zr);
>>>>>>> upstream/android-13
