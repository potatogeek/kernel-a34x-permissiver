// SPDX-License-Identifier: GPL-2.0
//
// ctu.c
//
// Copyright (c) 2015 Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>

#include "rsnd.h"

#define CTU_NAME_SIZE	16
#define CTU_NAME "ctu"

/*
 * User needs to setup CTU by amixer, and its settings are
 * based on below registers
 *
 * CTUn_CPMDR : amixser set "CTU Pass"
 * CTUn_SV0xR : amixser set "CTU SV0"
 * CTUn_SV1xR : amixser set "CTU SV1"
 * CTUn_SV2xR : amixser set "CTU SV2"
 * CTUn_SV3xR : amixser set "CTU SV3"
 *
 * [CTU Pass]
 * 0000: default
 * 0001: Connect input data of channel 0
 * 0010: Connect input data of channel 1
 * 0011: Connect input data of channel 2
 * 0100: Connect input data of channel 3
 * 0101: Connect input data of channel 4
 * 0110: Connect input data of channel 5
 * 0111: Connect input data of channel 6
 * 1000: Connect input data of channel 7
 * 1001: Connect calculated data by scale values of matrix row 0
 * 1010: Connect calculated data by scale values of matrix row 1
 * 1011: Connect calculated data by scale values of matrix row 2
 * 1100: Connect calculated data by scale values of matrix row 3
 *
 * [CTU SVx]
 * [Output0] = [SV00, SV01, SV02, SV03, SV04, SV05, SV06, SV07]
 * [Output1] = [SV10, SV11, SV12, SV13, SV14, SV15, SV16, SV17]
 * [Output2] = [SV20, SV21, SV22, SV23, SV24, SV25, SV26, SV27]
 * [Output3] = [SV30, SV31, SV32, SV33, SV34, SV35, SV36, SV37]
 * [Output4] = [ 0,   0,    0,    0,    0,    0,    0,    0   ]
 * [Output5] = [ 0,   0,    0,    0,    0,    0,    0,    0   ]
 * [Output6] = [ 0,   0,    0,    0,    0,    0,    0,    0   ]
 * [Output7] = [ 0,   0,    0,    0,    0,    0,    0,    0   ]
 *
 * [SVxx]
 * Plus					Minus
 * value	time		dB	value		time		dB
 * -----------------------------------------------------------------------
 * H'7F_FFFF	2		6	H'80_0000	2		6
 * ...
 * H'40_0000	1		0	H'C0_0000	1		0
 * ...
 * H'00_0001	2.38 x 10^-7	-132
 * H'00_0000	0		Mute	H'FF_FFFF	2.38 x 10^-7	-132
 *
 *
 * Ex) Input ch -> Output ch
 *	1ch     ->  0ch
 *	0ch     ->  1ch
 *
 *	amixer set "CTU Reset" on
 *	amixer set "CTU Pass" 9,10
 *	amixer set "CTU SV0" 0,4194304
 *	amixer set "CTU SV1" 4194304,0
 * or
 *	amixer set "CTU Reset" on
 *	amixer set "CTU Pass" 2,1
 */

struct rsnd_ctu {
	struct rsnd_mod mod;
	struct rsnd_kctrl_cfg_m pass;
<<<<<<< HEAD
	struct rsnd_kctrl_cfg_m sv0;
	struct rsnd_kctrl_cfg_m sv1;
	struct rsnd_kctrl_cfg_m sv2;
	struct rsnd_kctrl_cfg_m sv3;
=======
	struct rsnd_kctrl_cfg_m sv[4];
>>>>>>> upstream/android-13
	struct rsnd_kctrl_cfg_s reset;
	int channels;
	u32 flags;
};

#define KCTRL_INITIALIZED	(1 << 0)

#define rsnd_ctu_nr(priv) ((priv)->ctu_nr)
#define for_each_rsnd_ctu(pos, priv, i)					\
	for ((i) = 0;							\
	     ((i) < rsnd_ctu_nr(priv)) &&				\
		     ((pos) = (struct rsnd_ctu *)(priv)->ctu + i);	\
	     i++)

#define rsnd_mod_to_ctu(_mod)	\
	container_of((_mod), struct rsnd_ctu, mod)

#define rsnd_ctu_get(priv, id) ((struct rsnd_ctu *)(priv->ctu) + id)

static void rsnd_ctu_activation(struct rsnd_mod *mod)
{
	rsnd_mod_write(mod, CTU_SWRSR, 0);
	rsnd_mod_write(mod, CTU_SWRSR, 1);
}

static void rsnd_ctu_halt(struct rsnd_mod *mod)
{
	rsnd_mod_write(mod, CTU_CTUIR, 1);
	rsnd_mod_write(mod, CTU_SWRSR, 0);
}

<<<<<<< HEAD
int rsnd_ctu_converted_channel(struct rsnd_mod *mod)
{
	struct rsnd_ctu *ctu = rsnd_mod_to_ctu(mod);

	return ctu->channels;
}

=======
>>>>>>> upstream/android-13
static int rsnd_ctu_probe_(struct rsnd_mod *mod,
			   struct rsnd_dai_stream *io,
			   struct rsnd_priv *priv)
{
<<<<<<< HEAD
	return rsnd_cmd_attach(io, rsnd_mod_id(mod) / 4);
=======
	return rsnd_cmd_attach(io, rsnd_mod_id(mod));
>>>>>>> upstream/android-13
}

static void rsnd_ctu_value_init(struct rsnd_dai_stream *io,
			       struct rsnd_mod *mod)
{
	struct rsnd_ctu *ctu = rsnd_mod_to_ctu(mod);
	u32 cpmdr = 0;
	u32 scmdr = 0;
<<<<<<< HEAD
	int i;
=======
	int i, j;
>>>>>>> upstream/android-13

	for (i = 0; i < RSND_MAX_CHANNELS; i++) {
		u32 val = rsnd_kctrl_valm(ctu->pass, i);

		cpmdr |= val << (28 - (i * 4));

		if ((val > 0x8) && (scmdr < (val - 0x8)))
			scmdr = val - 0x8;
	}

	rsnd_mod_write(mod, CTU_CTUIR, 1);

	rsnd_mod_write(mod, CTU_ADINR, rsnd_runtime_channel_original(io));

	rsnd_mod_write(mod, CTU_CPMDR, cpmdr);

	rsnd_mod_write(mod, CTU_SCMDR, scmdr);

<<<<<<< HEAD
	if (scmdr > 0) {
		rsnd_mod_write(mod, CTU_SV00R, rsnd_kctrl_valm(ctu->sv0, 0));
		rsnd_mod_write(mod, CTU_SV01R, rsnd_kctrl_valm(ctu->sv0, 1));
		rsnd_mod_write(mod, CTU_SV02R, rsnd_kctrl_valm(ctu->sv0, 2));
		rsnd_mod_write(mod, CTU_SV03R, rsnd_kctrl_valm(ctu->sv0, 3));
		rsnd_mod_write(mod, CTU_SV04R, rsnd_kctrl_valm(ctu->sv0, 4));
		rsnd_mod_write(mod, CTU_SV05R, rsnd_kctrl_valm(ctu->sv0, 5));
		rsnd_mod_write(mod, CTU_SV06R, rsnd_kctrl_valm(ctu->sv0, 6));
		rsnd_mod_write(mod, CTU_SV07R, rsnd_kctrl_valm(ctu->sv0, 7));
	}
	if (scmdr > 1) {
		rsnd_mod_write(mod, CTU_SV10R, rsnd_kctrl_valm(ctu->sv1, 0));
		rsnd_mod_write(mod, CTU_SV11R, rsnd_kctrl_valm(ctu->sv1, 1));
		rsnd_mod_write(mod, CTU_SV12R, rsnd_kctrl_valm(ctu->sv1, 2));
		rsnd_mod_write(mod, CTU_SV13R, rsnd_kctrl_valm(ctu->sv1, 3));
		rsnd_mod_write(mod, CTU_SV14R, rsnd_kctrl_valm(ctu->sv1, 4));
		rsnd_mod_write(mod, CTU_SV15R, rsnd_kctrl_valm(ctu->sv1, 5));
		rsnd_mod_write(mod, CTU_SV16R, rsnd_kctrl_valm(ctu->sv1, 6));
		rsnd_mod_write(mod, CTU_SV17R, rsnd_kctrl_valm(ctu->sv1, 7));
	}
	if (scmdr > 2) {
		rsnd_mod_write(mod, CTU_SV20R, rsnd_kctrl_valm(ctu->sv2, 0));
		rsnd_mod_write(mod, CTU_SV21R, rsnd_kctrl_valm(ctu->sv2, 1));
		rsnd_mod_write(mod, CTU_SV22R, rsnd_kctrl_valm(ctu->sv2, 2));
		rsnd_mod_write(mod, CTU_SV23R, rsnd_kctrl_valm(ctu->sv2, 3));
		rsnd_mod_write(mod, CTU_SV24R, rsnd_kctrl_valm(ctu->sv2, 4));
		rsnd_mod_write(mod, CTU_SV25R, rsnd_kctrl_valm(ctu->sv2, 5));
		rsnd_mod_write(mod, CTU_SV26R, rsnd_kctrl_valm(ctu->sv2, 6));
		rsnd_mod_write(mod, CTU_SV27R, rsnd_kctrl_valm(ctu->sv2, 7));
	}
	if (scmdr > 3) {
		rsnd_mod_write(mod, CTU_SV30R, rsnd_kctrl_valm(ctu->sv3, 0));
		rsnd_mod_write(mod, CTU_SV31R, rsnd_kctrl_valm(ctu->sv3, 1));
		rsnd_mod_write(mod, CTU_SV32R, rsnd_kctrl_valm(ctu->sv3, 2));
		rsnd_mod_write(mod, CTU_SV33R, rsnd_kctrl_valm(ctu->sv3, 3));
		rsnd_mod_write(mod, CTU_SV34R, rsnd_kctrl_valm(ctu->sv3, 4));
		rsnd_mod_write(mod, CTU_SV35R, rsnd_kctrl_valm(ctu->sv3, 5));
		rsnd_mod_write(mod, CTU_SV36R, rsnd_kctrl_valm(ctu->sv3, 6));
		rsnd_mod_write(mod, CTU_SV37R, rsnd_kctrl_valm(ctu->sv3, 7));
=======
	for (i = 0; i < 4; i++) {

		if (i >= scmdr)
			break;

		for (j = 0; j < RSND_MAX_CHANNELS; j++)
			rsnd_mod_write(mod, CTU_SVxxR(i, j), rsnd_kctrl_valm(ctu->sv[i], j));
>>>>>>> upstream/android-13
	}

	rsnd_mod_write(mod, CTU_CTUIR, 0);
}

static void rsnd_ctu_value_reset(struct rsnd_dai_stream *io,
				 struct rsnd_mod *mod)
{
	struct rsnd_ctu *ctu = rsnd_mod_to_ctu(mod);
	int i;

	if (!rsnd_kctrl_vals(ctu->reset))
		return;

	for (i = 0; i < RSND_MAX_CHANNELS; i++) {
		rsnd_kctrl_valm(ctu->pass, i) = 0;
<<<<<<< HEAD
		rsnd_kctrl_valm(ctu->sv0,  i) = 0;
		rsnd_kctrl_valm(ctu->sv1,  i) = 0;
		rsnd_kctrl_valm(ctu->sv2,  i) = 0;
		rsnd_kctrl_valm(ctu->sv3,  i) = 0;
=======
		rsnd_kctrl_valm(ctu->sv[0],  i) = 0;
		rsnd_kctrl_valm(ctu->sv[1],  i) = 0;
		rsnd_kctrl_valm(ctu->sv[2],  i) = 0;
		rsnd_kctrl_valm(ctu->sv[3],  i) = 0;
>>>>>>> upstream/android-13
	}
	rsnd_kctrl_vals(ctu->reset) = 0;
}

static int rsnd_ctu_init(struct rsnd_mod *mod,
			 struct rsnd_dai_stream *io,
			 struct rsnd_priv *priv)
{
	rsnd_mod_power_on(mod);

	rsnd_ctu_activation(mod);

	rsnd_ctu_value_init(io, mod);

	return 0;
}

static int rsnd_ctu_quit(struct rsnd_mod *mod,
			 struct rsnd_dai_stream *io,
			 struct rsnd_priv *priv)
{
	rsnd_ctu_halt(mod);

	rsnd_mod_power_off(mod);

	return 0;
}

<<<<<<< HEAD
static int rsnd_ctu_hw_params(struct rsnd_mod *mod,
			      struct rsnd_dai_stream *io,
			      struct snd_pcm_substream *substream,
			      struct snd_pcm_hw_params *fe_params)
{
	struct rsnd_ctu *ctu = rsnd_mod_to_ctu(mod);
	struct snd_soc_pcm_runtime *fe = substream->private_data;

	/*
	 * CTU assumes that it is used under DPCM if user want to use
	 * channel transfer. Then, CTU should be FE.
	 * And then, this function will be called *after* BE settings.
	 * this means, each BE already has fixuped hw_params.
	 * see
	 *	dpcm_fe_dai_hw_params()
	 *	dpcm_be_dai_hw_params()
	 */
	ctu->channels = 0;
	if (fe->dai_link->dynamic) {
		struct rsnd_priv *priv = rsnd_mod_to_priv(mod);
		struct device *dev = rsnd_priv_to_dev(priv);
		struct snd_soc_dpcm *dpcm;
		struct snd_pcm_hw_params *be_params;
		int stream = substream->stream;

		list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
			be_params = &dpcm->hw_params;
			if (params_channels(fe_params) != params_channels(be_params))
				ctu->channels = params_channels(be_params);
		}

		dev_dbg(dev, "CTU convert channels %d\n", ctu->channels);
	}

	return 0;
}

=======
>>>>>>> upstream/android-13
static int rsnd_ctu_pcm_new(struct rsnd_mod *mod,
			    struct rsnd_dai_stream *io,
			    struct snd_soc_pcm_runtime *rtd)
{
	struct rsnd_ctu *ctu = rsnd_mod_to_ctu(mod);
	int ret;

	if (rsnd_flags_has(ctu, KCTRL_INITIALIZED))
		return 0;

	/* CTU Pass */
	ret = rsnd_kctrl_new_m(mod, io, rtd, "CTU Pass",
			       rsnd_kctrl_accept_anytime,
			       NULL,
			       &ctu->pass, RSND_MAX_CHANNELS,
			       0xC);
<<<<<<< HEAD
=======
	if (ret < 0)
		return ret;
>>>>>>> upstream/android-13

	/* ROW0 */
	ret = rsnd_kctrl_new_m(mod, io, rtd, "CTU SV0",
			       rsnd_kctrl_accept_anytime,
			       NULL,
<<<<<<< HEAD
			       &ctu->sv0, RSND_MAX_CHANNELS,
=======
			       &ctu->sv[0], RSND_MAX_CHANNELS,
>>>>>>> upstream/android-13
			       0x00FFFFFF);
	if (ret < 0)
		return ret;

	/* ROW1 */
	ret = rsnd_kctrl_new_m(mod, io, rtd, "CTU SV1",
			       rsnd_kctrl_accept_anytime,
			       NULL,
<<<<<<< HEAD
			       &ctu->sv1, RSND_MAX_CHANNELS,
=======
			       &ctu->sv[1], RSND_MAX_CHANNELS,
>>>>>>> upstream/android-13
			       0x00FFFFFF);
	if (ret < 0)
		return ret;

	/* ROW2 */
	ret = rsnd_kctrl_new_m(mod, io, rtd, "CTU SV2",
			       rsnd_kctrl_accept_anytime,
			       NULL,
<<<<<<< HEAD
			       &ctu->sv2, RSND_MAX_CHANNELS,
=======
			       &ctu->sv[2], RSND_MAX_CHANNELS,
>>>>>>> upstream/android-13
			       0x00FFFFFF);
	if (ret < 0)
		return ret;

	/* ROW3 */
	ret = rsnd_kctrl_new_m(mod, io, rtd, "CTU SV3",
			       rsnd_kctrl_accept_anytime,
			       NULL,
<<<<<<< HEAD
			       &ctu->sv3, RSND_MAX_CHANNELS,
=======
			       &ctu->sv[3], RSND_MAX_CHANNELS,
>>>>>>> upstream/android-13
			       0x00FFFFFF);
	if (ret < 0)
		return ret;

	/* Reset */
	ret = rsnd_kctrl_new_s(mod, io, rtd, "CTU Reset",
			       rsnd_kctrl_accept_anytime,
			       rsnd_ctu_value_reset,
			       &ctu->reset, 1);

	rsnd_flags_set(ctu, KCTRL_INITIALIZED);

	return ret;
}

<<<<<<< HEAD
=======
static int rsnd_ctu_id(struct rsnd_mod *mod)
{
	/*
	 * ctu00: -> 0, ctu01: -> 0, ctu02: -> 0, ctu03: -> 0
	 * ctu10: -> 1, ctu11: -> 1, ctu12: -> 1, ctu13: -> 1
	 */
	return mod->id / 4;
}

static int rsnd_ctu_id_sub(struct rsnd_mod *mod)
{
	/*
	 * ctu00: -> 0, ctu01: -> 1, ctu02: -> 2, ctu03: -> 3
	 * ctu10: -> 0, ctu11: -> 1, ctu12: -> 2, ctu13: -> 3
	 */
	return mod->id % 4;
}

#ifdef CONFIG_DEBUG_FS
static void rsnd_ctu_debug_info(struct seq_file *m,
				struct rsnd_dai_stream *io,
				struct rsnd_mod *mod)
{
	rsnd_debugfs_mod_reg_show(m, mod, RSND_GEN2_SCU,
				  0x500 + rsnd_mod_id_raw(mod) * 0x100, 0x100);
}
#define DEBUG_INFO .debug_info = rsnd_ctu_debug_info
#else
#define DEBUG_INFO
#endif

>>>>>>> upstream/android-13
static struct rsnd_mod_ops rsnd_ctu_ops = {
	.name		= CTU_NAME,
	.probe		= rsnd_ctu_probe_,
	.init		= rsnd_ctu_init,
	.quit		= rsnd_ctu_quit,
<<<<<<< HEAD
	.hw_params	= rsnd_ctu_hw_params,
	.pcm_new	= rsnd_ctu_pcm_new,
=======
	.pcm_new	= rsnd_ctu_pcm_new,
	.get_status	= rsnd_mod_get_status,
	.id		= rsnd_ctu_id,
	.id_sub		= rsnd_ctu_id_sub,
	.id_cmd		= rsnd_mod_id_raw,
	DEBUG_INFO
>>>>>>> upstream/android-13
};

struct rsnd_mod *rsnd_ctu_mod_get(struct rsnd_priv *priv, int id)
{
	if (WARN_ON(id < 0 || id >= rsnd_ctu_nr(priv)))
		id = 0;

	return rsnd_mod_get(rsnd_ctu_get(priv, id));
}

int rsnd_ctu_probe(struct rsnd_priv *priv)
{
	struct device_node *node;
	struct device_node *np;
	struct device *dev = rsnd_priv_to_dev(priv);
	struct rsnd_ctu *ctu;
	struct clk *clk;
	char name[CTU_NAME_SIZE];
	int i, nr, ret;

	/* This driver doesn't support Gen1 at this point */
	if (rsnd_is_gen1(priv))
		return 0;

	node = rsnd_ctu_of_node(priv);
	if (!node)
		return 0; /* not used is not error */

	nr = of_get_child_count(node);
	if (!nr) {
		ret = -EINVAL;
		goto rsnd_ctu_probe_done;
	}

	ctu = devm_kcalloc(dev, nr, sizeof(*ctu), GFP_KERNEL);
	if (!ctu) {
		ret = -ENOMEM;
		goto rsnd_ctu_probe_done;
	}

	priv->ctu_nr	= nr;
	priv->ctu	= ctu;

	i = 0;
	ret = 0;
	for_each_child_of_node(node, np) {
		ctu = rsnd_ctu_get(priv, i);

		/*
		 * CTU00, CTU01, CTU02, CTU03 => CTU0
		 * CTU10, CTU11, CTU12, CTU13 => CTU1
		 */
		snprintf(name, CTU_NAME_SIZE, "%s.%d",
			 CTU_NAME, i / 4);

		clk = devm_clk_get(dev, name);
		if (IS_ERR(clk)) {
			ret = PTR_ERR(clk);
			of_node_put(np);
			goto rsnd_ctu_probe_done;
		}

		ret = rsnd_mod_init(priv, rsnd_mod_get(ctu), &rsnd_ctu_ops,
<<<<<<< HEAD
				    clk, rsnd_mod_get_status, RSND_MOD_CTU, i);
=======
				    clk, RSND_MOD_CTU, i);
>>>>>>> upstream/android-13
		if (ret) {
			of_node_put(np);
			goto rsnd_ctu_probe_done;
		}

		i++;
	}


rsnd_ctu_probe_done:
	of_node_put(node);

	return ret;
}

void rsnd_ctu_remove(struct rsnd_priv *priv)
{
	struct rsnd_ctu *ctu;
	int i;

	for_each_rsnd_ctu(ctu, priv, i) {
		rsnd_mod_quit(rsnd_mod_get(ctu));
	}
}
