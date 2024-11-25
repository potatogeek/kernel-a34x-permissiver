// SPDX-License-Identifier: GPL-2.0
//
// Renesas R-Car
//
// Copyright (C) 2013 Renesas Solutions Corp.
// Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>

#ifndef RSND_H
#define RSND_H

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_graph.h>
#include <linux/of_irq.h>
#include <linux/sh_dma.h>
#include <linux/workqueue.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>

#define RSND_GEN1_SRU	0
#define RSND_GEN1_ADG	1
#define RSND_GEN1_SSI	2

#define RSND_GEN2_SCU	0
#define RSND_GEN2_ADG	1
#define RSND_GEN2_SSIU	2
#define RSND_GEN2_SSI	3

#define RSND_BASE_MAX	4

/*
 *	pseudo register
 *
 * The register address offsets SRU/SCU/SSIU on Gen1/Gen2 are very different.
 * This driver uses pseudo register in order to hide it.
 * see gen1/gen2 for detail
 */
enum rsnd_reg {
	/* SCU (MIX/CTU/DVC) */
<<<<<<< HEAD
	RSND_REG_SRC_I_BUSIF_MODE,
	RSND_REG_SRC_O_BUSIF_MODE,
	RSND_REG_SRC_ROUTE_MODE0,
	RSND_REG_SRC_SWRSR,
	RSND_REG_SRC_SRCIR,
	RSND_REG_SRC_ADINR,
	RSND_REG_SRC_IFSCR,
	RSND_REG_SRC_IFSVR,
	RSND_REG_SRC_SRCCR,
	RSND_REG_SRC_CTRL,
	RSND_REG_SRC_BSDSR,
	RSND_REG_SRC_BSISR,
	RSND_REG_SRC_INT_ENABLE0,
	RSND_REG_SRC_BUSIF_DALIGN,
	RSND_REG_SRCIN_TIMSEL0,
	RSND_REG_SRCIN_TIMSEL1,
	RSND_REG_SRCIN_TIMSEL2,
	RSND_REG_SRCIN_TIMSEL3,
	RSND_REG_SRCIN_TIMSEL4,
	RSND_REG_SRCOUT_TIMSEL0,
	RSND_REG_SRCOUT_TIMSEL1,
	RSND_REG_SRCOUT_TIMSEL2,
	RSND_REG_SRCOUT_TIMSEL3,
	RSND_REG_SRCOUT_TIMSEL4,
	RSND_REG_SCU_SYS_STATUS0,
	RSND_REG_SCU_SYS_STATUS1,
	RSND_REG_SCU_SYS_INT_EN0,
	RSND_REG_SCU_SYS_INT_EN1,
	RSND_REG_CMD_CTRL,
	RSND_REG_CMD_BUSIF_MODE,
	RSND_REG_CMD_BUSIF_DALIGN,
	RSND_REG_CMD_ROUTE_SLCT,
	RSND_REG_CMDOUT_TIMSEL,
	RSND_REG_CTU_SWRSR,
	RSND_REG_CTU_CTUIR,
	RSND_REG_CTU_ADINR,
	RSND_REG_CTU_CPMDR,
	RSND_REG_CTU_SCMDR,
	RSND_REG_CTU_SV00R,
	RSND_REG_CTU_SV01R,
	RSND_REG_CTU_SV02R,
	RSND_REG_CTU_SV03R,
	RSND_REG_CTU_SV04R,
	RSND_REG_CTU_SV05R,
	RSND_REG_CTU_SV06R,
	RSND_REG_CTU_SV07R,
	RSND_REG_CTU_SV10R,
	RSND_REG_CTU_SV11R,
	RSND_REG_CTU_SV12R,
	RSND_REG_CTU_SV13R,
	RSND_REG_CTU_SV14R,
	RSND_REG_CTU_SV15R,
	RSND_REG_CTU_SV16R,
	RSND_REG_CTU_SV17R,
	RSND_REG_CTU_SV20R,
	RSND_REG_CTU_SV21R,
	RSND_REG_CTU_SV22R,
	RSND_REG_CTU_SV23R,
	RSND_REG_CTU_SV24R,
	RSND_REG_CTU_SV25R,
	RSND_REG_CTU_SV26R,
	RSND_REG_CTU_SV27R,
	RSND_REG_CTU_SV30R,
	RSND_REG_CTU_SV31R,
	RSND_REG_CTU_SV32R,
	RSND_REG_CTU_SV33R,
	RSND_REG_CTU_SV34R,
	RSND_REG_CTU_SV35R,
	RSND_REG_CTU_SV36R,
	RSND_REG_CTU_SV37R,
	RSND_REG_MIX_SWRSR,
	RSND_REG_MIX_MIXIR,
	RSND_REG_MIX_ADINR,
	RSND_REG_MIX_MIXMR,
	RSND_REG_MIX_MVPDR,
	RSND_REG_MIX_MDBAR,
	RSND_REG_MIX_MDBBR,
	RSND_REG_MIX_MDBCR,
	RSND_REG_MIX_MDBDR,
	RSND_REG_MIX_MDBER,
	RSND_REG_DVC_SWRSR,
	RSND_REG_DVC_DVUIR,
	RSND_REG_DVC_ADINR,
	RSND_REG_DVC_DVUCR,
	RSND_REG_DVC_ZCMCR,
	RSND_REG_DVC_VOL0R,
	RSND_REG_DVC_VOL1R,
	RSND_REG_DVC_VOL2R,
	RSND_REG_DVC_VOL3R,
	RSND_REG_DVC_VOL4R,
	RSND_REG_DVC_VOL5R,
	RSND_REG_DVC_VOL6R,
	RSND_REG_DVC_VOL7R,
	RSND_REG_DVC_DVUER,
	RSND_REG_DVC_VRCTR,
	RSND_REG_DVC_VRPDR,
	RSND_REG_DVC_VRDBR,

	/* ADG */
	RSND_REG_BRRA,
	RSND_REG_BRRB,
	RSND_REG_BRGCKR,
	RSND_REG_DIV_EN,
	RSND_REG_AUDIO_CLK_SEL0,
	RSND_REG_AUDIO_CLK_SEL1,
	RSND_REG_AUDIO_CLK_SEL2,

	/* SSIU */
	RSND_REG_SSI_MODE,
	RSND_REG_SSI_MODE0,
	RSND_REG_SSI_MODE1,
	RSND_REG_SSI_MODE2,
	RSND_REG_SSI_CONTROL,
	RSND_REG_SSI_CTRL,
	RSND_REG_SSI_BUSIF_MODE,
	RSND_REG_SSI_BUSIF_ADINR,
	RSND_REG_SSI_BUSIF_DALIGN,
	RSND_REG_SSI_INT_ENABLE,
	RSND_REG_SSI_SYS_STATUS0,
	RSND_REG_SSI_SYS_STATUS1,
	RSND_REG_SSI_SYS_STATUS2,
	RSND_REG_SSI_SYS_STATUS3,
	RSND_REG_SSI_SYS_STATUS4,
	RSND_REG_SSI_SYS_STATUS5,
	RSND_REG_SSI_SYS_STATUS6,
	RSND_REG_SSI_SYS_STATUS7,
	RSND_REG_HDMI0_SEL,
	RSND_REG_HDMI1_SEL,

	/* SSI */
	RSND_REG_SSICR,
	RSND_REG_SSISR,
	RSND_REG_SSITDR,
	RSND_REG_SSIRDR,
	RSND_REG_SSIWSR,

	RSND_REG_MAX,
};
=======
	SRC_I_BUSIF_MODE,
	SRC_O_BUSIF_MODE,
	SRC_ROUTE_MODE0,
	SRC_SWRSR,
	SRC_SRCIR,
	SRC_ADINR,
	SRC_IFSCR,
	SRC_IFSVR,
	SRC_SRCCR,
	SRC_CTRL,
	SRC_BSDSR,
	SRC_BSISR,
	SRC_INT_ENABLE0,
	SRC_BUSIF_DALIGN,
	SRCIN_TIMSEL0,
	SRCIN_TIMSEL1,
	SRCIN_TIMSEL2,
	SRCIN_TIMSEL3,
	SRCIN_TIMSEL4,
	SRCOUT_TIMSEL0,
	SRCOUT_TIMSEL1,
	SRCOUT_TIMSEL2,
	SRCOUT_TIMSEL3,
	SRCOUT_TIMSEL4,
	SCU_SYS_STATUS0,
	SCU_SYS_STATUS1,
	SCU_SYS_INT_EN0,
	SCU_SYS_INT_EN1,
	CMD_CTRL,
	CMD_BUSIF_MODE,
	CMD_BUSIF_DALIGN,
	CMD_ROUTE_SLCT,
	CMDOUT_TIMSEL,
	CTU_SWRSR,
	CTU_CTUIR,
	CTU_ADINR,
	CTU_CPMDR,
	CTU_SCMDR,
	CTU_SV00R,
	CTU_SV01R,
	CTU_SV02R,
	CTU_SV03R,
	CTU_SV04R,
	CTU_SV05R,
	CTU_SV06R,
	CTU_SV07R,
	CTU_SV10R,
	CTU_SV11R,
	CTU_SV12R,
	CTU_SV13R,
	CTU_SV14R,
	CTU_SV15R,
	CTU_SV16R,
	CTU_SV17R,
	CTU_SV20R,
	CTU_SV21R,
	CTU_SV22R,
	CTU_SV23R,
	CTU_SV24R,
	CTU_SV25R,
	CTU_SV26R,
	CTU_SV27R,
	CTU_SV30R,
	CTU_SV31R,
	CTU_SV32R,
	CTU_SV33R,
	CTU_SV34R,
	CTU_SV35R,
	CTU_SV36R,
	CTU_SV37R,
	MIX_SWRSR,
	MIX_MIXIR,
	MIX_ADINR,
	MIX_MIXMR,
	MIX_MVPDR,
	MIX_MDBAR,
	MIX_MDBBR,
	MIX_MDBCR,
	MIX_MDBDR,
	MIX_MDBER,
	DVC_SWRSR,
	DVC_DVUIR,
	DVC_ADINR,
	DVC_DVUCR,
	DVC_ZCMCR,
	DVC_VOL0R,
	DVC_VOL1R,
	DVC_VOL2R,
	DVC_VOL3R,
	DVC_VOL4R,
	DVC_VOL5R,
	DVC_VOL6R,
	DVC_VOL7R,
	DVC_DVUER,
	DVC_VRCTR,
	DVC_VRPDR,
	DVC_VRDBR,

	/* ADG */
	BRRA,
	BRRB,
	BRGCKR,
	DIV_EN,
	AUDIO_CLK_SEL0,
	AUDIO_CLK_SEL1,
	AUDIO_CLK_SEL2,

	/* SSIU */
	SSI_MODE,
	SSI_MODE0,
	SSI_MODE1,
	SSI_MODE2,
	SSI_CONTROL,
	SSI_CTRL,
	SSI_BUSIF0_MODE,
	SSI_BUSIF1_MODE,
	SSI_BUSIF2_MODE,
	SSI_BUSIF3_MODE,
	SSI_BUSIF4_MODE,
	SSI_BUSIF5_MODE,
	SSI_BUSIF6_MODE,
	SSI_BUSIF7_MODE,
	SSI_BUSIF0_ADINR,
	SSI_BUSIF1_ADINR,
	SSI_BUSIF2_ADINR,
	SSI_BUSIF3_ADINR,
	SSI_BUSIF4_ADINR,
	SSI_BUSIF5_ADINR,
	SSI_BUSIF6_ADINR,
	SSI_BUSIF7_ADINR,
	SSI_BUSIF0_DALIGN,
	SSI_BUSIF1_DALIGN,
	SSI_BUSIF2_DALIGN,
	SSI_BUSIF3_DALIGN,
	SSI_BUSIF4_DALIGN,
	SSI_BUSIF5_DALIGN,
	SSI_BUSIF6_DALIGN,
	SSI_BUSIF7_DALIGN,
	SSI_INT_ENABLE,
	SSI_SYS_STATUS0,
	SSI_SYS_STATUS1,
	SSI_SYS_STATUS2,
	SSI_SYS_STATUS3,
	SSI_SYS_STATUS4,
	SSI_SYS_STATUS5,
	SSI_SYS_STATUS6,
	SSI_SYS_STATUS7,
	SSI_SYS_INT_ENABLE0,
	SSI_SYS_INT_ENABLE1,
	SSI_SYS_INT_ENABLE2,
	SSI_SYS_INT_ENABLE3,
	SSI_SYS_INT_ENABLE4,
	SSI_SYS_INT_ENABLE5,
	SSI_SYS_INT_ENABLE6,
	SSI_SYS_INT_ENABLE7,
	HDMI0_SEL,
	HDMI1_SEL,
	SSI9_BUSIF0_MODE,
	SSI9_BUSIF1_MODE,
	SSI9_BUSIF2_MODE,
	SSI9_BUSIF3_MODE,
	SSI9_BUSIF4_MODE,
	SSI9_BUSIF5_MODE,
	SSI9_BUSIF6_MODE,
	SSI9_BUSIF7_MODE,
	SSI9_BUSIF0_ADINR,
	SSI9_BUSIF1_ADINR,
	SSI9_BUSIF2_ADINR,
	SSI9_BUSIF3_ADINR,
	SSI9_BUSIF4_ADINR,
	SSI9_BUSIF5_ADINR,
	SSI9_BUSIF6_ADINR,
	SSI9_BUSIF7_ADINR,
	SSI9_BUSIF0_DALIGN,
	SSI9_BUSIF1_DALIGN,
	SSI9_BUSIF2_DALIGN,
	SSI9_BUSIF3_DALIGN,
	SSI9_BUSIF4_DALIGN,
	SSI9_BUSIF5_DALIGN,
	SSI9_BUSIF6_DALIGN,
	SSI9_BUSIF7_DALIGN,

	/* SSI */
	SSICR,
	SSISR,
	SSITDR,
	SSIRDR,
	SSIWSR,

	REG_MAX,
};
#define SRCIN_TIMSEL(i)		(SRCIN_TIMSEL0 + (i))
#define SRCOUT_TIMSEL(i)	(SRCOUT_TIMSEL0 + (i))
#define CTU_SVxxR(i, j)		(CTU_SV00R + (i * 8) + (j))
#define DVC_VOLxR(i)		(DVC_VOL0R + (i))
#define AUDIO_CLK_SEL(i)	(AUDIO_CLK_SEL0 + (i))
#define SSI_BUSIF_MODE(i)	(SSI_BUSIF0_MODE + (i))
#define SSI_BUSIF_ADINR(i)	(SSI_BUSIF0_ADINR + (i))
#define SSI_BUSIF_DALIGN(i)	(SSI_BUSIF0_DALIGN + (i))
#define SSI9_BUSIF_MODE(i)	(SSI9_BUSIF0_MODE + (i))
#define SSI9_BUSIF_ADINR(i)	(SSI9_BUSIF0_ADINR + (i))
#define SSI9_BUSIF_DALIGN(i)	(SSI9_BUSIF0_DALIGN + (i))
#define SSI_SYS_STATUS(i)	(SSI_SYS_STATUS0 + (i))
#define SSI_SYS_INT_ENABLE(i) (SSI_SYS_INT_ENABLE0 + (i))

>>>>>>> upstream/android-13

struct rsnd_priv;
struct rsnd_mod;
struct rsnd_dai;
struct rsnd_dai_stream;

/*
 *	R-Car basic functions
 */
<<<<<<< HEAD
#define rsnd_mod_read(m, r) \
	rsnd_read(rsnd_mod_to_priv(m), m, RSND_REG_##r)
#define rsnd_mod_write(m, r, d) \
	rsnd_write(rsnd_mod_to_priv(m), m, RSND_REG_##r, d)
#define rsnd_mod_bset(m, r, s, d) \
	rsnd_bset(rsnd_mod_to_priv(m), m, RSND_REG_##r, s, d)

u32 rsnd_read(struct rsnd_priv *priv, struct rsnd_mod *mod, enum rsnd_reg reg);
void rsnd_write(struct rsnd_priv *priv, struct rsnd_mod *mod,
		enum rsnd_reg reg, u32 data);
void rsnd_force_write(struct rsnd_priv *priv, struct rsnd_mod *mod,
		enum rsnd_reg reg, u32 data);
void rsnd_bset(struct rsnd_priv *priv, struct rsnd_mod *mod, enum rsnd_reg reg,
		    u32 mask, u32 data);
=======
u32 rsnd_mod_read(struct rsnd_mod *mod, enum rsnd_reg reg);
void rsnd_mod_write(struct rsnd_mod *mod, enum rsnd_reg reg, u32 data);
void rsnd_mod_bset(struct rsnd_mod *mod, enum rsnd_reg reg, u32 mask, u32 data);
>>>>>>> upstream/android-13
u32 rsnd_get_adinr_bit(struct rsnd_mod *mod, struct rsnd_dai_stream *io);
u32 rsnd_get_dalign(struct rsnd_mod *mod, struct rsnd_dai_stream *io);
u32 rsnd_get_busif_shift(struct rsnd_dai_stream *io, struct rsnd_mod *mod);

/*
 *	R-Car DMA
 */
int rsnd_dma_attach(struct rsnd_dai_stream *io,
		    struct rsnd_mod *mod, struct rsnd_mod **dma_mod);
int rsnd_dma_probe(struct rsnd_priv *priv);
<<<<<<< HEAD
struct dma_chan *rsnd_dma_request_channel(struct device_node *of_node,
					  struct rsnd_mod *mod, char *name);
=======
struct dma_chan *rsnd_dma_request_channel(struct device_node *of_node, char *name,
					  struct rsnd_mod *mod, char *x);
>>>>>>> upstream/android-13

/*
 *	R-Car sound mod
 */
enum rsnd_mod_type {
	RSND_MOD_AUDMAPP,
	RSND_MOD_AUDMA,
	RSND_MOD_DVC,
	RSND_MOD_MIX,
	RSND_MOD_CTU,
	RSND_MOD_CMD,
	RSND_MOD_SRC,
	RSND_MOD_SSIM3,		/* SSI multi 3 */
	RSND_MOD_SSIM2,		/* SSI multi 2 */
	RSND_MOD_SSIM1,		/* SSI multi 1 */
	RSND_MOD_SSIP,		/* SSI parent */
	RSND_MOD_SSI,
	RSND_MOD_SSIU,
	RSND_MOD_MAX,
};

struct rsnd_mod_ops {
	char *name;
	struct dma_chan* (*dma_req)(struct rsnd_dai_stream *io,
				    struct rsnd_mod *mod);
	int (*probe)(struct rsnd_mod *mod,
		     struct rsnd_dai_stream *io,
		     struct rsnd_priv *priv);
	int (*remove)(struct rsnd_mod *mod,
		      struct rsnd_dai_stream *io,
		      struct rsnd_priv *priv);
	int (*init)(struct rsnd_mod *mod,
		    struct rsnd_dai_stream *io,
		    struct rsnd_priv *priv);
	int (*quit)(struct rsnd_mod *mod,
		    struct rsnd_dai_stream *io,
		    struct rsnd_priv *priv);
	int (*start)(struct rsnd_mod *mod,
		     struct rsnd_dai_stream *io,
		     struct rsnd_priv *priv);
	int (*stop)(struct rsnd_mod *mod,
		    struct rsnd_dai_stream *io,
		    struct rsnd_priv *priv);
	int (*irq)(struct rsnd_mod *mod,
		   struct rsnd_dai_stream *io,
		   struct rsnd_priv *priv, int enable);
	int (*pcm_new)(struct rsnd_mod *mod,
		       struct rsnd_dai_stream *io,
		       struct snd_soc_pcm_runtime *rtd);
	int (*hw_params)(struct rsnd_mod *mod,
			 struct rsnd_dai_stream *io,
			 struct snd_pcm_substream *substream,
			 struct snd_pcm_hw_params *hw_params);
	int (*pointer)(struct rsnd_mod *mod,
		       struct rsnd_dai_stream *io,
		       snd_pcm_uframes_t *pointer);
	int (*fallback)(struct rsnd_mod *mod,
			struct rsnd_dai_stream *io,
			struct rsnd_priv *priv);
<<<<<<< HEAD
	int (*nolock_start)(struct rsnd_mod *mod,
		    struct rsnd_dai_stream *io,
		    struct rsnd_priv *priv);
	int (*nolock_stop)(struct rsnd_mod *mod,
		    struct rsnd_dai_stream *io,
		    struct rsnd_priv *priv);
	int (*prepare)(struct rsnd_mod *mod,
		       struct rsnd_dai_stream *io,
		       struct rsnd_priv *priv);
=======
	int (*prepare)(struct rsnd_mod *mod,
		       struct rsnd_dai_stream *io,
		       struct rsnd_priv *priv);
	int (*cleanup)(struct rsnd_mod *mod,
		       struct rsnd_dai_stream *io,
		       struct rsnd_priv *priv);
	int (*hw_free)(struct rsnd_mod *mod,
		       struct rsnd_dai_stream *io,
		       struct snd_pcm_substream *substream);
	u32 *(*get_status)(struct rsnd_mod *mod,
			   struct rsnd_dai_stream *io,
			   enum rsnd_mod_type type);
	int (*id)(struct rsnd_mod *mod);
	int (*id_sub)(struct rsnd_mod *mod);
	int (*id_cmd)(struct rsnd_mod *mod);

#ifdef CONFIG_DEBUG_FS
	void (*debug_info)(struct seq_file *m,
			   struct rsnd_dai_stream *io, struct rsnd_mod *mod);
#endif
>>>>>>> upstream/android-13
};

struct rsnd_dai_stream;
struct rsnd_mod {
	int id;
	enum rsnd_mod_type type;
	struct rsnd_mod_ops *ops;
	struct rsnd_priv *priv;
	struct clk *clk;
<<<<<<< HEAD
	u32 *(*get_status)(struct rsnd_dai_stream *io,
			   struct rsnd_mod *mod,
			   enum rsnd_mod_type type);
=======
>>>>>>> upstream/android-13
	u32 status;
};
/*
 * status
 *
<<<<<<< HEAD
 * 0xH0000CBA
 *
 * A	0: nolock_start	1: nolock_stop
 * B	0: init		1: quit
 * C	0: start	1: stop
 *
 * H is always called (see __rsnd_mod_call)
 * H	0: probe	1: remove
 * H	0: pcm_new
 * H	0: fallback
 * H	0: hw_params
 * H	0: pointer
 * H	0: prepare
 */
#define __rsnd_mod_shift_nolock_start	0
#define __rsnd_mod_shift_nolock_stop	0
=======
 * 0xH000DCB0
 *
 * B	0: init		1: quit
 * C	0: start	1: stop
 * D	0: hw_params	1: hw_free
 *
 * H is always called (see __rsnd_mod_call)
 */
>>>>>>> upstream/android-13
#define __rsnd_mod_shift_init		4
#define __rsnd_mod_shift_quit		4
#define __rsnd_mod_shift_start		8
#define __rsnd_mod_shift_stop		8
<<<<<<< HEAD
=======
#define __rsnd_mod_shift_hw_params	12
#define __rsnd_mod_shift_hw_free	12
>>>>>>> upstream/android-13
#define __rsnd_mod_shift_probe		28 /* always called */
#define __rsnd_mod_shift_remove		28 /* always called */
#define __rsnd_mod_shift_irq		28 /* always called */
#define __rsnd_mod_shift_pcm_new	28 /* always called */
#define __rsnd_mod_shift_fallback	28 /* always called */
<<<<<<< HEAD
#define __rsnd_mod_shift_hw_params	28 /* always called */
#define __rsnd_mod_shift_pointer	28 /* always called */
#define __rsnd_mod_shift_prepare	28 /* always called */

#define __rsnd_mod_add_probe		0
#define __rsnd_mod_add_remove		0
#define __rsnd_mod_add_nolock_start	 1
#define __rsnd_mod_add_nolock_stop	-1
#define __rsnd_mod_add_init		 1
#define __rsnd_mod_add_quit		-1
#define __rsnd_mod_add_start		 1
#define __rsnd_mod_add_stop		-1
#define __rsnd_mod_add_irq		0
#define __rsnd_mod_add_pcm_new		0
#define __rsnd_mod_add_fallback		0
#define __rsnd_mod_add_hw_params	0
#define __rsnd_mod_add_pointer		0
#define __rsnd_mod_add_prepare		0

#define __rsnd_mod_call_probe		0
#define __rsnd_mod_call_remove		0
#define __rsnd_mod_call_init		0
#define __rsnd_mod_call_quit		1
#define __rsnd_mod_call_start		0
#define __rsnd_mod_call_stop		1
#define __rsnd_mod_call_irq		0
#define __rsnd_mod_call_pcm_new		0
#define __rsnd_mod_call_fallback	0
#define __rsnd_mod_call_hw_params	0
#define __rsnd_mod_call_pointer		0
#define __rsnd_mod_call_nolock_start	0
#define __rsnd_mod_call_nolock_stop	1
#define __rsnd_mod_call_prepare		0

#define rsnd_mod_to_priv(mod)	((mod)->priv)
#define rsnd_mod_name(mod)	((mod)->ops->name)
#define rsnd_mod_id(mod)	((mod)->id)
=======
#define __rsnd_mod_shift_pointer	28 /* always called */
#define __rsnd_mod_shift_prepare	28 /* always called */
#define __rsnd_mod_shift_cleanup	28 /* always called */

#define __rsnd_mod_add_probe		0
#define __rsnd_mod_add_remove		0
#define __rsnd_mod_add_prepare		0
#define __rsnd_mod_add_cleanup		0
#define __rsnd_mod_add_init		 1 /* needs protect */
#define __rsnd_mod_add_quit		-1 /* needs protect */
#define __rsnd_mod_add_start		 1 /* needs protect */
#define __rsnd_mod_add_stop		-1 /* needs protect */
#define __rsnd_mod_add_hw_params	 1 /* needs protect */
#define __rsnd_mod_add_hw_free		-1 /* needs protect */
#define __rsnd_mod_add_irq		0
#define __rsnd_mod_add_pcm_new		0
#define __rsnd_mod_add_fallback		0
#define __rsnd_mod_add_pointer		0

#define __rsnd_mod_call_probe		0
#define __rsnd_mod_call_remove		0
#define __rsnd_mod_call_prepare		0
#define __rsnd_mod_call_cleanup		0
#define __rsnd_mod_call_init		0 /* needs protect */
#define __rsnd_mod_call_quit		1 /* needs protect */
#define __rsnd_mod_call_start		0 /* needs protect */
#define __rsnd_mod_call_stop		1 /* needs protect */
#define __rsnd_mod_call_hw_params	0 /* needs protect */
#define __rsnd_mod_call_hw_free		1 /* needs protect */
#define __rsnd_mod_call_irq		0
#define __rsnd_mod_call_pcm_new		0
#define __rsnd_mod_call_fallback	0
#define __rsnd_mod_call_pointer		0

#define rsnd_mod_to_priv(mod)	((mod)->priv)
>>>>>>> upstream/android-13
#define rsnd_mod_power_on(mod)	clk_enable((mod)->clk)
#define rsnd_mod_power_off(mod)	clk_disable((mod)->clk)
#define rsnd_mod_get(ip)	(&(ip)->mod)

int rsnd_mod_init(struct rsnd_priv *priv,
		  struct rsnd_mod *mod,
		  struct rsnd_mod_ops *ops,
		  struct clk *clk,
<<<<<<< HEAD
		  u32* (*get_status)(struct rsnd_dai_stream *io,
				     struct rsnd_mod *mod,
				     enum rsnd_mod_type type),
=======
>>>>>>> upstream/android-13
		  enum rsnd_mod_type type,
		  int id);
void rsnd_mod_quit(struct rsnd_mod *mod);
struct dma_chan *rsnd_mod_dma_req(struct rsnd_dai_stream *io,
				  struct rsnd_mod *mod);
void rsnd_mod_interrupt(struct rsnd_mod *mod,
			void (*callback)(struct rsnd_mod *mod,
					 struct rsnd_dai_stream *io));
<<<<<<< HEAD
u32 *rsnd_mod_get_status(struct rsnd_dai_stream *io,
			 struct rsnd_mod *mod,
			 enum rsnd_mod_type type);
=======
u32 *rsnd_mod_get_status(struct rsnd_mod *mod,
			 struct rsnd_dai_stream *io,
			 enum rsnd_mod_type type);
int rsnd_mod_id(struct rsnd_mod *mod);
int rsnd_mod_id_raw(struct rsnd_mod *mod);
int rsnd_mod_id_sub(struct rsnd_mod *mod);
char *rsnd_mod_name(struct rsnd_mod *mod);
>>>>>>> upstream/android-13
struct rsnd_mod *rsnd_mod_next(int *iterator,
			       struct rsnd_dai_stream *io,
			       enum rsnd_mod_type *array,
			       int array_size);
#define for_each_rsnd_mod(iterator, pos, io)				\
	for (iterator = 0;						\
	     (pos = rsnd_mod_next(&iterator, io, NULL, 0)); iterator++)
#define for_each_rsnd_mod_arrays(iterator, pos, io, array, size)	\
	for (iterator = 0;						\
	     (pos = rsnd_mod_next(&iterator, io, array, size)); iterator++)
#define for_each_rsnd_mod_array(iterator, pos, io, array)		\
	for_each_rsnd_mod_arrays(iterator, pos, io, array, ARRAY_SIZE(array))

<<<<<<< HEAD
void rsnd_parse_connect_common(struct rsnd_dai *rdai,
=======
void rsnd_parse_connect_common(struct rsnd_dai *rdai, char *name,
>>>>>>> upstream/android-13
		struct rsnd_mod* (*mod_get)(struct rsnd_priv *priv, int id),
		struct device_node *node,
		struct device_node *playback,
		struct device_node *capture);
<<<<<<< HEAD

=======
int rsnd_node_count(struct rsnd_priv *priv, struct device_node *node, char *name);
int rsnd_node_fixed_index(struct device_node *node, char *name, int idx);

int rsnd_channel_normalization(int chan);
>>>>>>> upstream/android-13
#define rsnd_runtime_channel_original(io) \
	rsnd_runtime_channel_original_with_params(io, NULL)
int rsnd_runtime_channel_original_with_params(struct rsnd_dai_stream *io,
				struct snd_pcm_hw_params *params);
#define rsnd_runtime_channel_after_ctu(io)			\
	rsnd_runtime_channel_after_ctu_with_params(io, NULL)
int rsnd_runtime_channel_after_ctu_with_params(struct rsnd_dai_stream *io,
				struct snd_pcm_hw_params *params);
#define rsnd_runtime_channel_for_ssi(io) \
	rsnd_runtime_channel_for_ssi_with_params(io, NULL)
int rsnd_runtime_channel_for_ssi_with_params(struct rsnd_dai_stream *io,
				 struct snd_pcm_hw_params *params);
<<<<<<< HEAD
int rsnd_runtime_is_ssi_multi(struct rsnd_dai_stream *io);
int rsnd_runtime_is_ssi_tdm(struct rsnd_dai_stream *io);
=======
int rsnd_runtime_is_multi_ssi(struct rsnd_dai_stream *io);
int rsnd_runtime_is_tdm(struct rsnd_dai_stream *io);
int rsnd_runtime_is_tdm_split(struct rsnd_dai_stream *io);
>>>>>>> upstream/android-13

/*
 * DT
 */
#define rsnd_parse_of_node(priv, node)					\
	of_get_child_by_name(rsnd_priv_to_dev(priv)->of_node, node)
#define RSND_NODE_DAI	"rcar_sound,dai"
#define RSND_NODE_SSI	"rcar_sound,ssi"
<<<<<<< HEAD
=======
#define RSND_NODE_SSIU	"rcar_sound,ssiu"
>>>>>>> upstream/android-13
#define RSND_NODE_SRC	"rcar_sound,src"
#define RSND_NODE_CTU	"rcar_sound,ctu"
#define RSND_NODE_MIX	"rcar_sound,mix"
#define RSND_NODE_DVC	"rcar_sound,dvc"

/*
 *	R-Car sound DAI
 */
#define RSND_DAI_NAME_SIZE	16
struct rsnd_dai_stream {
	char name[RSND_DAI_NAME_SIZE];
	struct snd_pcm_substream *substream;
	struct rsnd_mod *mod[RSND_MOD_MAX];
	struct rsnd_mod *dma;
	struct rsnd_dai *rdai;
	struct device *dmac_dev; /* for IPMMU */
<<<<<<< HEAD
	u32 parent_ssi_status;
};
=======
	u32 converted_rate;      /* converted sampling rate */
	int converted_chan;      /* converted channels */
	u32 parent_ssi_status;
	u32 flags;
};

/* flags */
#define RSND_STREAM_HDMI0	(1 << 0) /* for HDMI0 */
#define RSND_STREAM_HDMI1	(1 << 1) /* for HDMI1 */
#define RSND_STREAM_TDM_SPLIT	(1 << 2) /* for TDM split mode */

>>>>>>> upstream/android-13
#define rsnd_io_to_mod(io, i)	((i) < RSND_MOD_MAX ? (io)->mod[(i)] : NULL)
#define rsnd_io_to_mod_ssi(io)	rsnd_io_to_mod((io), RSND_MOD_SSI)
#define rsnd_io_to_mod_ssiu(io)	rsnd_io_to_mod((io), RSND_MOD_SSIU)
#define rsnd_io_to_mod_ssip(io)	rsnd_io_to_mod((io), RSND_MOD_SSIP)
#define rsnd_io_to_mod_src(io)	rsnd_io_to_mod((io), RSND_MOD_SRC)
#define rsnd_io_to_mod_ctu(io)	rsnd_io_to_mod((io), RSND_MOD_CTU)
#define rsnd_io_to_mod_mix(io)	rsnd_io_to_mod((io), RSND_MOD_MIX)
#define rsnd_io_to_mod_dvc(io)	rsnd_io_to_mod((io), RSND_MOD_DVC)
#define rsnd_io_to_mod_cmd(io)	rsnd_io_to_mod((io), RSND_MOD_CMD)
#define rsnd_io_to_rdai(io)	((io)->rdai)
#define rsnd_io_to_priv(io)	(rsnd_rdai_to_priv(rsnd_io_to_rdai(io)))
#define rsnd_io_is_play(io)	(&rsnd_io_to_rdai(io)->playback == io)
#define rsnd_io_to_runtime(io) ((io)->substream ? \
				(io)->substream->runtime : NULL)
<<<<<<< HEAD
=======
#define rsnd_io_converted_rate(io)	((io)->converted_rate)
#define rsnd_io_converted_chan(io)	((io)->converted_chan)
>>>>>>> upstream/android-13
int rsnd_io_is_working(struct rsnd_dai_stream *io);

struct rsnd_dai {
	char name[RSND_DAI_NAME_SIZE];
	struct rsnd_dai_stream playback;
	struct rsnd_dai_stream capture;
	struct rsnd_priv *priv;
	struct snd_pcm_hw_constraint_list constraint;

	int max_channels;	/* 2ch - 16ch */
	int ssi_lane;		/* 1lane - 4lane */
<<<<<<< HEAD
=======
	int chan_width;		/* 16/24/32 bit width */
>>>>>>> upstream/android-13

	unsigned int clk_master:1;
	unsigned int bit_clk_inv:1;
	unsigned int frm_clk_inv:1;
	unsigned int sys_delay:1;
	unsigned int data_alignment:1;
};

#define rsnd_rdai_nr(priv) ((priv)->rdai_nr)
#define rsnd_rdai_is_clk_master(rdai) ((rdai)->clk_master)
#define rsnd_rdai_to_priv(rdai) ((rdai)->priv)
#define for_each_rsnd_dai(rdai, priv, i)		\
	for (i = 0;					\
	     (i < rsnd_rdai_nr(priv)) &&		\
	     ((rdai) = rsnd_rdai_get(priv, i));		\
	     i++)

struct rsnd_dai *rsnd_rdai_get(struct rsnd_priv *priv, int id);

#define rsnd_rdai_channels_set(rdai, max_channels) \
	rsnd_rdai_channels_ctrl(rdai, max_channels)
#define rsnd_rdai_channels_get(rdai) \
	rsnd_rdai_channels_ctrl(rdai, 0)
int rsnd_rdai_channels_ctrl(struct rsnd_dai *rdai,
			    int max_channels);

#define rsnd_rdai_ssi_lane_set(rdai, ssi_lane) \
	rsnd_rdai_ssi_lane_ctrl(rdai, ssi_lane)
#define rsnd_rdai_ssi_lane_get(rdai) \
	rsnd_rdai_ssi_lane_ctrl(rdai, 0)
int rsnd_rdai_ssi_lane_ctrl(struct rsnd_dai *rdai,
			    int ssi_lane);

<<<<<<< HEAD
=======
#define rsnd_rdai_width_set(rdai, width) \
	rsnd_rdai_width_ctrl(rdai, width)
#define rsnd_rdai_width_get(rdai) \
	rsnd_rdai_width_ctrl(rdai, 0)
int rsnd_rdai_width_ctrl(struct rsnd_dai *rdai, int width);
>>>>>>> upstream/android-13
void rsnd_dai_period_elapsed(struct rsnd_dai_stream *io);
int rsnd_dai_connect(struct rsnd_mod *mod,
		     struct rsnd_dai_stream *io,
		     enum rsnd_mod_type type);

/*
 *	R-Car Gen1/Gen2
 */
int rsnd_gen_probe(struct rsnd_priv *priv);
void __iomem *rsnd_gen_reg_get(struct rsnd_priv *priv,
			       struct rsnd_mod *mod,
			       enum rsnd_reg reg);
phys_addr_t rsnd_gen_get_phy_addr(struct rsnd_priv *priv, int reg_id);
<<<<<<< HEAD
=======
#ifdef CONFIG_DEBUG_FS
void __iomem *rsnd_gen_get_base_addr(struct rsnd_priv *priv, int reg_id);
#endif
>>>>>>> upstream/android-13

/*
 *	R-Car ADG
 */
int rsnd_adg_clk_query(struct rsnd_priv *priv, unsigned int rate);
<<<<<<< HEAD
int rsnd_adg_ssi_clk_stop(struct rsnd_mod *mod);
int rsnd_adg_ssi_clk_try_start(struct rsnd_mod *mod, unsigned int rate);
=======
int rsnd_adg_ssi_clk_stop(struct rsnd_mod *ssi_mod);
int rsnd_adg_ssi_clk_try_start(struct rsnd_mod *ssi_mod, unsigned int rate);
>>>>>>> upstream/android-13
int rsnd_adg_probe(struct rsnd_priv *priv);
void rsnd_adg_remove(struct rsnd_priv *priv);
int rsnd_adg_set_src_timesel_gen2(struct rsnd_mod *src_mod,
				  struct rsnd_dai_stream *io,
				  unsigned int in_rate,
				  unsigned int out_rate);
<<<<<<< HEAD
int rsnd_adg_set_cmd_timsel_gen2(struct rsnd_mod *mod,
=======
int rsnd_adg_set_cmd_timsel_gen2(struct rsnd_mod *cmd_mod,
>>>>>>> upstream/android-13
				 struct rsnd_dai_stream *io);
#define rsnd_adg_clk_enable(priv)	rsnd_adg_clk_control(priv, 1)
#define rsnd_adg_clk_disable(priv)	rsnd_adg_clk_control(priv, 0)
void rsnd_adg_clk_control(struct rsnd_priv *priv, int enable);
<<<<<<< HEAD
=======
void rsnd_adg_clk_dbg_info(struct rsnd_priv *priv, struct seq_file *m);
>>>>>>> upstream/android-13

/*
 *	R-Car sound priv
 */
struct rsnd_priv {

	struct platform_device *pdev;
	spinlock_t lock;
	unsigned long flags;
#define RSND_GEN_MASK	(0xF << 0)
#define RSND_GEN1	(1 << 0)
#define RSND_GEN2	(2 << 0)
#define RSND_GEN3	(3 << 0)
<<<<<<< HEAD
=======
#define RSND_SOC_MASK	(0xFF << 4)
#define RSND_SOC_E	(1 << 4) /* E1/E2/E3 */
>>>>>>> upstream/android-13

	/*
	 * below value will be filled on rsnd_gen_probe()
	 */
	void *gen;

	/*
	 * below value will be filled on rsnd_adg_probe()
	 */
	void *adg;

	/*
	 * below value will be filled on rsnd_dma_probe()
	 */
	void *dma;

	/*
	 * below value will be filled on rsnd_ssi_probe()
	 */
	void *ssi;
	int ssi_nr;

	/*
	 * below value will be filled on rsnd_ssiu_probe()
	 */
	void *ssiu;
	int ssiu_nr;

	/*
	 * below value will be filled on rsnd_src_probe()
	 */
	void *src;
	int src_nr;

	/*
	 * below value will be filled on rsnd_ctu_probe()
	 */
	void *ctu;
	int ctu_nr;

	/*
	 * below value will be filled on rsnd_mix_probe()
	 */
	void *mix;
	int mix_nr;

	/*
	 * below value will be filled on rsnd_dvc_probe()
	 */
	void *dvc;
	int dvc_nr;

	/*
	 * below value will be filled on rsnd_cmd_probe()
	 */
	void *cmd;
	int cmd_nr;

	/*
	 * below value will be filled on rsnd_dai_probe()
	 */
	struct snd_soc_dai_driver *daidrv;
	struct rsnd_dai *rdai;
	int rdai_nr;
};

#define rsnd_priv_to_pdev(priv)	((priv)->pdev)
#define rsnd_priv_to_dev(priv)	(&(rsnd_priv_to_pdev(priv)->dev))

#define rsnd_is_gen1(priv)	(((priv)->flags & RSND_GEN_MASK) == RSND_GEN1)
#define rsnd_is_gen2(priv)	(((priv)->flags & RSND_GEN_MASK) == RSND_GEN2)
#define rsnd_is_gen3(priv)	(((priv)->flags & RSND_GEN_MASK) == RSND_GEN3)
<<<<<<< HEAD
=======
#define rsnd_is_e3(priv)	(((priv)->flags & \
					(RSND_GEN_MASK | RSND_SOC_MASK)) == \
					(RSND_GEN3 | RSND_SOC_E))
>>>>>>> upstream/android-13

#define rsnd_flags_has(p, f) ((p)->flags & (f))
#define rsnd_flags_set(p, f) ((p)->flags |= (f))
#define rsnd_flags_del(p, f) ((p)->flags &= ~(f))

/*
 *	rsnd_kctrl
 */
struct rsnd_kctrl_cfg {
	unsigned int max;
	unsigned int size;
	u32 *val;
	const char * const *texts;
	int (*accept)(struct rsnd_dai_stream *io);
	void (*update)(struct rsnd_dai_stream *io, struct rsnd_mod *mod);
	struct rsnd_dai_stream *io;
	struct snd_card *card;
	struct snd_kcontrol *kctrl;
	struct rsnd_mod *mod;
};

#define RSND_MAX_CHANNELS	8
struct rsnd_kctrl_cfg_m {
	struct rsnd_kctrl_cfg cfg;
	u32 val[RSND_MAX_CHANNELS];
};

struct rsnd_kctrl_cfg_s {
	struct rsnd_kctrl_cfg cfg;
	u32 val;
};
#define rsnd_kctrl_size(x)	((x).cfg.size)
#define rsnd_kctrl_max(x)	((x).cfg.max)
#define rsnd_kctrl_valm(x, i)	((x).val[i])	/* = (x).cfg.val[i] */
#define rsnd_kctrl_vals(x)	((x).val)	/* = (x).cfg.val[0] */

int rsnd_kctrl_accept_anytime(struct rsnd_dai_stream *io);
int rsnd_kctrl_accept_runtime(struct rsnd_dai_stream *io);
struct rsnd_kctrl_cfg *rsnd_kctrl_init_m(struct rsnd_kctrl_cfg_m *cfg);
struct rsnd_kctrl_cfg *rsnd_kctrl_init_s(struct rsnd_kctrl_cfg_s *cfg);
int rsnd_kctrl_new(struct rsnd_mod *mod,
		   struct rsnd_dai_stream *io,
		   struct snd_soc_pcm_runtime *rtd,
		   const unsigned char *name,
		   int (*accept)(struct rsnd_dai_stream *io),
		   void (*update)(struct rsnd_dai_stream *io,
				  struct rsnd_mod *mod),
		   struct rsnd_kctrl_cfg *cfg,
		   const char * const *texts,
		   int size,
		   u32 max);

#define rsnd_kctrl_new_m(mod, io, rtd, name, accept, update, cfg, size, max) \
	rsnd_kctrl_new(mod, io, rtd, name, accept, update, rsnd_kctrl_init_m(cfg), \
		       NULL, size, max)

#define rsnd_kctrl_new_s(mod, io, rtd, name, accept, update, cfg, max)	\
	rsnd_kctrl_new(mod, io, rtd, name, accept, update, rsnd_kctrl_init_s(cfg), \
		       NULL, 1, max)

#define rsnd_kctrl_new_e(mod, io, rtd, name, accept, update, cfg, texts, size) \
	rsnd_kctrl_new(mod, io, rtd, name, accept, update, rsnd_kctrl_init_s(cfg), \
		       texts, 1, size)

extern const char * const volume_ramp_rate[];
#define VOLUME_RAMP_MAX_DVC	(0x17 + 1)
#define VOLUME_RAMP_MAX_MIX	(0x0a + 1)

/*
 *	R-Car SSI
 */
int rsnd_ssi_probe(struct rsnd_priv *priv);
void rsnd_ssi_remove(struct rsnd_priv *priv);
struct rsnd_mod *rsnd_ssi_mod_get(struct rsnd_priv *priv, int id);
<<<<<<< HEAD
int rsnd_ssi_is_dma_mode(struct rsnd_mod *mod);
int rsnd_ssi_use_busif(struct rsnd_dai_stream *io);
u32 rsnd_ssi_multi_slaves_runtime(struct rsnd_dai_stream *io);

#define RSND_SSI_HDMI_PORT0	0xf0
#define RSND_SSI_HDMI_PORT1	0xf1
int rsnd_ssi_hdmi_port(struct rsnd_dai_stream *io);
void rsnd_ssi_parse_hdmi_connection(struct rsnd_priv *priv,
				    struct device_node *endpoint,
				    int dai_i);
=======
int rsnd_ssi_use_busif(struct rsnd_dai_stream *io);
u32 rsnd_ssi_multi_secondaries_runtime(struct rsnd_dai_stream *io);
int rsnd_ssi_is_dma_mode(struct rsnd_mod *mod);
>>>>>>> upstream/android-13

#define rsnd_ssi_is_pin_sharing(io)	\
	__rsnd_ssi_is_pin_sharing(rsnd_io_to_mod_ssi(io))
int __rsnd_ssi_is_pin_sharing(struct rsnd_mod *mod);

#define rsnd_ssi_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_SSI)
void rsnd_parse_connect_ssi(struct rsnd_dai *rdai,
			    struct device_node *playback,
			    struct device_node *capture);
<<<<<<< HEAD
unsigned int rsnd_ssi_clk_query(struct rsnd_priv *priv,
=======
unsigned int rsnd_ssi_clk_query(struct rsnd_dai *rdai,
>>>>>>> upstream/android-13
		       int param1, int param2, int *idx);

/*
 *	R-Car SSIU
 */
int rsnd_ssiu_attach(struct rsnd_dai_stream *io,
		     struct rsnd_mod *mod);
int rsnd_ssiu_probe(struct rsnd_priv *priv);
void rsnd_ssiu_remove(struct rsnd_priv *priv);
<<<<<<< HEAD
=======
void rsnd_parse_connect_ssiu(struct rsnd_dai *rdai,
			     struct device_node *playback,
			     struct device_node *capture);
#define rsnd_ssiu_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_SSIU)
bool rsnd_ssiu_busif_err_status_clear(struct rsnd_mod *mod);
>>>>>>> upstream/android-13

/*
 *	R-Car SRC
 */
int rsnd_src_probe(struct rsnd_priv *priv);
void rsnd_src_remove(struct rsnd_priv *priv);
struct rsnd_mod *rsnd_src_mod_get(struct rsnd_priv *priv, int id);

#define rsnd_src_get_in_rate(priv, io) rsnd_src_get_rate(priv, io, 1)
#define rsnd_src_get_out_rate(priv, io) rsnd_src_get_rate(priv, io, 0)
unsigned int rsnd_src_get_rate(struct rsnd_priv *priv,
			       struct rsnd_dai_stream *io,
			       int is_in);

#define rsnd_src_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_SRC)
#define rsnd_parse_connect_src(rdai, playback, capture)			\
<<<<<<< HEAD
	rsnd_parse_connect_common(rdai, rsnd_src_mod_get,		\
=======
	rsnd_parse_connect_common(rdai, "src", rsnd_src_mod_get,	\
>>>>>>> upstream/android-13
				  rsnd_src_of_node(rsnd_rdai_to_priv(rdai)), \
						   playback, capture)

/*
 *	R-Car CTU
 */
int rsnd_ctu_probe(struct rsnd_priv *priv);
void rsnd_ctu_remove(struct rsnd_priv *priv);
<<<<<<< HEAD
int rsnd_ctu_converted_channel(struct rsnd_mod *mod);
struct rsnd_mod *rsnd_ctu_mod_get(struct rsnd_priv *priv, int id);
#define rsnd_ctu_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_CTU)
#define rsnd_parse_connect_ctu(rdai, playback, capture)			\
	rsnd_parse_connect_common(rdai, rsnd_ctu_mod_get,		\
=======
struct rsnd_mod *rsnd_ctu_mod_get(struct rsnd_priv *priv, int id);
#define rsnd_ctu_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_CTU)
#define rsnd_parse_connect_ctu(rdai, playback, capture)			\
	rsnd_parse_connect_common(rdai, "ctu", rsnd_ctu_mod_get,	\
>>>>>>> upstream/android-13
				  rsnd_ctu_of_node(rsnd_rdai_to_priv(rdai)), \
						   playback, capture)

/*
 *	R-Car MIX
 */
int rsnd_mix_probe(struct rsnd_priv *priv);
void rsnd_mix_remove(struct rsnd_priv *priv);
struct rsnd_mod *rsnd_mix_mod_get(struct rsnd_priv *priv, int id);
#define rsnd_mix_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_MIX)
#define rsnd_parse_connect_mix(rdai, playback, capture)			\
<<<<<<< HEAD
	rsnd_parse_connect_common(rdai, rsnd_mix_mod_get,		\
=======
	rsnd_parse_connect_common(rdai, "mix", rsnd_mix_mod_get,	\
>>>>>>> upstream/android-13
				  rsnd_mix_of_node(rsnd_rdai_to_priv(rdai)), \
						   playback, capture)

/*
 *	R-Car DVC
 */
int rsnd_dvc_probe(struct rsnd_priv *priv);
void rsnd_dvc_remove(struct rsnd_priv *priv);
struct rsnd_mod *rsnd_dvc_mod_get(struct rsnd_priv *priv, int id);
#define rsnd_dvc_of_node(priv) rsnd_parse_of_node(priv, RSND_NODE_DVC)
#define rsnd_parse_connect_dvc(rdai, playback, capture)			\
<<<<<<< HEAD
	rsnd_parse_connect_common(rdai, rsnd_dvc_mod_get,		\
=======
	rsnd_parse_connect_common(rdai, "dvc", rsnd_dvc_mod_get,	\
>>>>>>> upstream/android-13
				  rsnd_dvc_of_node(rsnd_rdai_to_priv(rdai)), \
						   playback, capture)

/*
 *	R-Car CMD
 */
int rsnd_cmd_probe(struct rsnd_priv *priv);
void rsnd_cmd_remove(struct rsnd_priv *priv);
int rsnd_cmd_attach(struct rsnd_dai_stream *io, int id);

void rsnd_mod_make_sure(struct rsnd_mod *mod, enum rsnd_mod_type type);
#ifdef DEBUG
#define rsnd_mod_confirm_ssi(mssi)	rsnd_mod_make_sure(mssi, RSND_MOD_SSI)
#define rsnd_mod_confirm_src(msrc)	rsnd_mod_make_sure(msrc, RSND_MOD_SRC)
#define rsnd_mod_confirm_dvc(mdvc)	rsnd_mod_make_sure(mdvc, RSND_MOD_DVC)
#else
#define rsnd_mod_confirm_ssi(mssi)
#define rsnd_mod_confirm_src(msrc)
#define rsnd_mod_confirm_dvc(mdvc)
#endif

/*
 * If you don't need interrupt status debug message,
 * define RSND_DEBUG_NO_IRQ_STATUS as 1 on top of src.c/ssi.c
 *
 * #define RSND_DEBUG_NO_IRQ_STATUS 1
 */
<<<<<<< HEAD
#define rsnd_dbg_irq_status(dev, param...)		\
	if (!IS_BUILTIN(RSND_DEBUG_NO_IRQ_STATUS))	\
		dev_dbg(dev, param)
=======
#define rsnd_print_irq_status(dev, param...) do {	\
	if (!IS_BUILTIN(RSND_DEBUG_NO_IRQ_STATUS))	\
		dev_info(dev, param);			\
} while (0)
>>>>>>> upstream/android-13

/*
 * If you don't need rsnd_dai_call debug message,
 * define RSND_DEBUG_NO_DAI_CALL as 1 on top of core.c
 *
 * #define RSND_DEBUG_NO_DAI_CALL 1
 */
#define rsnd_dbg_dai_call(dev, param...)		\
	if (!IS_BUILTIN(RSND_DEBUG_NO_DAI_CALL))	\
		dev_dbg(dev, param)

#endif
<<<<<<< HEAD
=======

#ifdef CONFIG_DEBUG_FS
int rsnd_debugfs_probe(struct snd_soc_component *component);
void rsnd_debugfs_reg_show(struct seq_file *m, phys_addr_t _addr,
			   void __iomem *base, int offset, int size);
void rsnd_debugfs_mod_reg_show(struct seq_file *m, struct rsnd_mod *mod,
			       int reg_id, int offset, int size);

#else
#define rsnd_debugfs_probe  NULL
#endif
>>>>>>> upstream/android-13
