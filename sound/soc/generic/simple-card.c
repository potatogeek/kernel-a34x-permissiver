// SPDX-License-Identifier: GPL-2.0
//
// ASoC simple sound card support
//
// Copyright (C) 2012 Renesas Solutions Corp.
// Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/of.h>
<<<<<<< HEAD
=======
#include <linux/of_device.h>
>>>>>>> upstream/android-13
#include <linux/platform_device.h>
#include <linux/string.h>
#include <sound/simple_card.h>
#include <sound/soc-dai.h>
#include <sound/soc.h>

<<<<<<< HEAD
struct simple_card_data {
	struct snd_soc_card snd_card;
	struct simple_dai_props {
		struct asoc_simple_dai cpu_dai;
		struct asoc_simple_dai codec_dai;
		unsigned int mclk_fs;
	} *dai_props;
	unsigned int mclk_fs;
	struct asoc_simple_jack hp_jack;
	struct asoc_simple_jack mic_jack;
	struct snd_soc_dai_link *dai_link;
};

#define simple_priv_to_card(priv) (&(priv)->snd_card)
#define simple_priv_to_props(priv, i) ((priv)->dai_props + (i))
#define simple_priv_to_dev(priv) (simple_priv_to_card(priv)->dev)
#define simple_priv_to_link(priv, i) (simple_priv_to_card(priv)->dai_link + (i))
=======
#define DPCM_SELECTABLE 1
>>>>>>> upstream/android-13

#define DAI	"sound-dai"
#define CELL	"#sound-dai-cells"
#define PREFIX	"simple-audio-card,"

<<<<<<< HEAD
static int asoc_simple_card_startup(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct simple_card_data *priv =	snd_soc_card_get_drvdata(rtd->card);
	struct simple_dai_props *dai_props =
		simple_priv_to_props(priv, rtd->num);
	int ret;

	ret = asoc_simple_card_clk_enable(&dai_props->cpu_dai);
	if (ret)
		return ret;

	ret = asoc_simple_card_clk_enable(&dai_props->codec_dai);
	if (ret)
		asoc_simple_card_clk_disable(&dai_props->cpu_dai);

	return ret;
}

static void asoc_simple_card_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct simple_card_data *priv =	snd_soc_card_get_drvdata(rtd->card);
	struct simple_dai_props *dai_props =
		simple_priv_to_props(priv, rtd->num);

	asoc_simple_card_clk_disable(&dai_props->cpu_dai);

	asoc_simple_card_clk_disable(&dai_props->codec_dai);
}

static int asoc_simple_set_clk_rate(struct asoc_simple_dai *simple_dai,
				    unsigned long rate)
{
	if (!simple_dai->clk)
		return 0;

	if (clk_get_rate(simple_dai->clk) == rate)
		return 0;

	return clk_set_rate(simple_dai->clk, rate);
}

static int asoc_simple_card_hw_params(struct snd_pcm_substream *substream,
				      struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct simple_card_data *priv = snd_soc_card_get_drvdata(rtd->card);
	struct simple_dai_props *dai_props =
		simple_priv_to_props(priv, rtd->num);
	unsigned int mclk, mclk_fs = 0;
	int ret = 0;

	if (priv->mclk_fs)
		mclk_fs = priv->mclk_fs;
	else if (dai_props->mclk_fs)
		mclk_fs = dai_props->mclk_fs;

	if (mclk_fs) {
		mclk = params_rate(params) * mclk_fs;

		ret = asoc_simple_set_clk_rate(&dai_props->codec_dai, mclk);
		if (ret < 0)
			return ret;

		ret = asoc_simple_set_clk_rate(&dai_props->cpu_dai, mclk);
		if (ret < 0)
			return ret;

		ret = snd_soc_dai_set_sysclk(codec_dai, 0, mclk,
					     SND_SOC_CLOCK_IN);
		if (ret && ret != -ENOTSUPP)
			goto err;

		ret = snd_soc_dai_set_sysclk(cpu_dai, 0, mclk,
					     SND_SOC_CLOCK_OUT);
		if (ret && ret != -ENOTSUPP)
			goto err;
	}
	return 0;
err:
	return ret;
}

static const struct snd_soc_ops asoc_simple_card_ops = {
	.startup = asoc_simple_card_startup,
	.shutdown = asoc_simple_card_shutdown,
	.hw_params = asoc_simple_card_hw_params,
};

static int asoc_simple_card_dai_init(struct snd_soc_pcm_runtime *rtd)
{
	struct simple_card_data *priv =	snd_soc_card_get_drvdata(rtd->card);
	struct snd_soc_dai *codec = rtd->codec_dai;
	struct snd_soc_dai *cpu = rtd->cpu_dai;
	struct simple_dai_props *dai_props =
		simple_priv_to_props(priv, rtd->num);
	int ret;

	ret = asoc_simple_card_init_dai(codec, &dai_props->codec_dai);
	if (ret < 0)
		return ret;

	ret = asoc_simple_card_init_dai(cpu, &dai_props->cpu_dai);
	if (ret < 0)
=======
static const struct snd_soc_ops simple_ops = {
	.startup	= asoc_simple_startup,
	.shutdown	= asoc_simple_shutdown,
	.hw_params	= asoc_simple_hw_params,
};

static int asoc_simple_parse_platform(struct device_node *node,
				      struct snd_soc_dai_link_component *dlc)
{
	struct of_phandle_args args;
	int ret;

	if (!node)
		return 0;

	/*
	 * Get node via "sound-dai = <&phandle port>"
	 * it will be used as xxx_of_node on soc_bind_dai_link()
	 */
	ret = of_parse_phandle_with_args(node, DAI, CELL, 0, &args);
	if (ret)
		return ret;

	/* dai_name is not required and may not exist for plat component */

	dlc->of_node = args.np;

	return 0;
}

static int asoc_simple_parse_dai(struct device_node *node,
				 struct snd_soc_dai_link_component *dlc,
				 int *is_single_link)
{
	struct of_phandle_args args;
	int ret;

	if (!node)
		return 0;

	/*
	 * Get node via "sound-dai = <&phandle port>"
	 * it will be used as xxx_of_node on soc_bind_dai_link()
	 */
	ret = of_parse_phandle_with_args(node, DAI, CELL, 0, &args);
	if (ret)
		return ret;

	/*
	 * FIXME
	 *
	 * Here, dlc->dai_name is pointer to CPU/Codec DAI name.
	 * If user unbinded CPU or Codec driver, but not for Sound Card,
	 * dlc->dai_name is keeping unbinded CPU or Codec
	 * driver's pointer.
	 *
	 * If user re-bind CPU or Codec driver again, ALSA SoC will try
	 * to rebind Card via snd_soc_try_rebind_card(), but because of
	 * above reason, it might can't bind Sound Card.
	 * Because Sound Card is pointing to released dai_name pointer.
	 *
	 * To avoid this rebind Card issue,
	 * 1) It needs to alloc memory to keep dai_name eventhough
	 *    CPU or Codec driver was unbinded, or
	 * 2) user need to rebind Sound Card everytime
	 *    if he unbinded CPU or Codec.
	 */
	ret = snd_soc_of_get_dai_name(node, &dlc->dai_name);
	if (ret < 0)
		return ret;

	dlc->of_node = args.np;

	if (is_single_link)
		*is_single_link = !args.args_count;

	return 0;
}

static void simple_parse_convert(struct device *dev,
				 struct device_node *np,
				 struct asoc_simple_data *adata)
{
	struct device_node *top = dev->of_node;
	struct device_node *node = of_get_parent(np);

	asoc_simple_parse_convert(top,  PREFIX, adata);
	asoc_simple_parse_convert(node, PREFIX, adata);
	asoc_simple_parse_convert(node, NULL,   adata);
	asoc_simple_parse_convert(np,   NULL,   adata);

	of_node_put(node);
}

static void simple_parse_mclk_fs(struct device_node *top,
				 struct device_node *np,
				 struct simple_dai_props *props,
				 char *prefix)
{
	struct device_node *node = of_get_parent(np);
	char prop[128];

	snprintf(prop, sizeof(prop), "%smclk-fs", PREFIX);
	of_property_read_u32(top,	prop, &props->mclk_fs);

	snprintf(prop, sizeof(prop), "%smclk-fs", prefix);
	of_property_read_u32(node,	prop, &props->mclk_fs);
	of_property_read_u32(np,	prop, &props->mclk_fs);

	of_node_put(node);
}

static int simple_parse_node(struct asoc_simple_priv *priv,
			     struct device_node *np,
			     struct link_info *li,
			     char *prefix,
			     int *cpu)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct device_node *top = dev->of_node;
	struct snd_soc_dai_link *dai_link = simple_priv_to_link(priv, li->link);
	struct simple_dai_props *dai_props = simple_priv_to_props(priv, li->link);
	struct snd_soc_dai_link_component *dlc;
	struct asoc_simple_dai *dai;
	int ret;

	if (cpu) {
		dlc = asoc_link_to_cpu(dai_link, 0);
		dai = simple_props_to_dai_cpu(dai_props, 0);
	} else {
		dlc = asoc_link_to_codec(dai_link, 0);
		dai = simple_props_to_dai_codec(dai_props, 0);
	}

	simple_parse_mclk_fs(top, np, dai_props, prefix);

	ret = asoc_simple_parse_dai(np, dlc, cpu);
	if (ret)
		return ret;

	ret = asoc_simple_parse_clk(dev, np, dai, dlc);
	if (ret)
		return ret;

	ret = asoc_simple_parse_tdm(np, dai);
	if (ret)
>>>>>>> upstream/android-13
		return ret;

	return 0;
}

<<<<<<< HEAD
static int asoc_simple_card_dai_link_of(struct device_node *node,
					struct simple_card_data *priv,
					int idx,
					bool is_top_level_node)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct snd_soc_dai_link *dai_link = simple_priv_to_link(priv, idx);
	struct simple_dai_props *dai_props = simple_priv_to_props(priv, idx);
	struct asoc_simple_dai *cpu_dai = &dai_props->cpu_dai;
	struct asoc_simple_dai *codec_dai = &dai_props->codec_dai;
	struct device_node *cpu = NULL;
	struct device_node *plat = NULL;
	struct device_node *codec = NULL;
	char prop[128];
	char *prefix = "";
	int ret, single_cpu;

	/* For single DAI link & old style of DT node */
	if (is_top_level_node)
		prefix = PREFIX;

	snprintf(prop, sizeof(prop), "%scpu", prefix);
	cpu = of_get_child_by_name(node, prop);

	if (!cpu) {
		ret = -EINVAL;
		dev_err(dev, "%s: Can't find %s DT node\n", __func__, prop);
		goto dai_link_of_err;
	}

	snprintf(prop, sizeof(prop), "%splat", prefix);
	plat = of_get_child_by_name(node, prop);

	snprintf(prop, sizeof(prop), "%scodec", prefix);
	codec = of_get_child_by_name(node, prop);

	if (!codec) {
		ret = -EINVAL;
		dev_err(dev, "%s: Can't find %s DT node\n", __func__, prop);
		goto dai_link_of_err;
	}

	ret = asoc_simple_card_parse_daifmt(dev, node, codec,
					    prefix, &dai_link->dai_fmt);
	if (ret < 0)
		goto dai_link_of_err;

	of_property_read_u32(node, "mclk-fs", &dai_props->mclk_fs);

	ret = asoc_simple_card_parse_cpu(cpu, dai_link,
					 DAI, CELL, &single_cpu);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_parse_codec(codec, dai_link, DAI, CELL);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_parse_platform(plat, dai_link, DAI, CELL);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_of_parse_tdm(cpu, cpu_dai);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_of_parse_tdm(codec, codec_dai);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_parse_clk_cpu(dev, cpu, dai_link, cpu_dai);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_parse_clk_codec(dev, codec, dai_link, codec_dai);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_canonicalize_dailink(dai_link);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_card_set_dailink_name(dev, dai_link,
						"%s-%s",
						dai_link->cpu_dai_name,
						dai_link->codec_dai_name);
	if (ret < 0)
		goto dai_link_of_err;

	dai_link->ops = &asoc_simple_card_ops;
	dai_link->init = asoc_simple_card_dai_init;

	asoc_simple_card_canonicalize_cpu(dai_link, single_cpu);

dai_link_of_err:
	of_node_put(cpu);
	of_node_put(codec);
=======
static int simple_link_init(struct asoc_simple_priv *priv,
			    struct device_node *node,
			    struct device_node *codec,
			    struct link_info *li,
			    char *prefix, char *name)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct snd_soc_dai_link *dai_link = simple_priv_to_link(priv, li->link);
	int ret;

	ret = asoc_simple_parse_daifmt(dev, node, codec,
				       prefix, &dai_link->dai_fmt);
	if (ret < 0)
		return 0;

	dai_link->init			= asoc_simple_dai_init;
	dai_link->ops			= &simple_ops;

	return asoc_simple_set_dailink_name(dev, dai_link, name);
}

static int simple_dai_link_of_dpcm(struct asoc_simple_priv *priv,
				   struct device_node *np,
				   struct device_node *codec,
				   struct link_info *li,
				   bool is_top)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct snd_soc_dai_link *dai_link = simple_priv_to_link(priv, li->link);
	struct simple_dai_props *dai_props = simple_priv_to_props(priv, li->link);
	struct device_node *top = dev->of_node;
	struct device_node *node = of_get_parent(np);
	char *prefix = "";
	char dai_name[64];
	int ret;

	dev_dbg(dev, "link_of DPCM (%pOF)\n", np);

	/* For single DAI link & old style of DT node */
	if (is_top)
		prefix = PREFIX;

	if (li->cpu) {
		struct snd_soc_dai_link_component *cpus = asoc_link_to_cpu(dai_link, 0);
		struct snd_soc_dai_link_component *platforms = asoc_link_to_platform(dai_link, 0);
		int is_single_links = 0;

		/* Codec is dummy */

		/* FE settings */
		dai_link->dynamic		= 1;
		dai_link->dpcm_merged_format	= 1;

		ret = simple_parse_node(priv, np, li, prefix, &is_single_links);
		if (ret < 0)
			goto out_put_node;

		snprintf(dai_name, sizeof(dai_name), "fe.%s", cpus->dai_name);

		asoc_simple_canonicalize_cpu(cpus, is_single_links);
		asoc_simple_canonicalize_platform(platforms, cpus);
	} else {
		struct snd_soc_dai_link_component *codecs = asoc_link_to_codec(dai_link, 0);
		struct snd_soc_codec_conf *cconf;

		/* CPU is dummy */

		/* BE settings */
		dai_link->no_pcm		= 1;
		dai_link->be_hw_params_fixup	= asoc_simple_be_hw_params_fixup;

		cconf	= simple_props_to_codec_conf(dai_props, 0);

		ret = simple_parse_node(priv, np, li, prefix, NULL);
		if (ret < 0)
			goto out_put_node;

		snprintf(dai_name, sizeof(dai_name), "be.%s", codecs->dai_name);

		/* check "prefix" from top node */
		snd_soc_of_parse_node_prefix(top, cconf, codecs->of_node,
					      PREFIX "prefix");
		snd_soc_of_parse_node_prefix(node, cconf, codecs->of_node,
					     "prefix");
		snd_soc_of_parse_node_prefix(np, cconf, codecs->of_node,
					     "prefix");
	}

	simple_parse_convert(dev, np, &dai_props->adata);

	snd_soc_dai_link_set_capabilities(dai_link);

	ret = simple_link_init(priv, node, codec, li, prefix, dai_name);

out_put_node:
	li->link++;

	of_node_put(node);
	return ret;
}

static int simple_dai_link_of(struct asoc_simple_priv *priv,
			      struct device_node *np,
			      struct device_node *codec,
			      struct link_info *li,
			      bool is_top)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct snd_soc_dai_link *dai_link = simple_priv_to_link(priv, li->link);
	struct snd_soc_dai_link_component *cpus = asoc_link_to_cpu(dai_link, 0);
	struct snd_soc_dai_link_component *codecs = asoc_link_to_codec(dai_link, 0);
	struct snd_soc_dai_link_component *platforms = asoc_link_to_platform(dai_link, 0);
	struct device_node *cpu = NULL;
	struct device_node *node = NULL;
	struct device_node *plat = NULL;
	char dai_name[64];
	char prop[128];
	char *prefix = "";
	int ret, single_cpu = 0;

	cpu  = np;
	node = of_get_parent(np);

	dev_dbg(dev, "link_of (%pOF)\n", node);

	/* For single DAI link & old style of DT node */
	if (is_top)
		prefix = PREFIX;

	snprintf(prop, sizeof(prop), "%splat", prefix);
	plat = of_get_child_by_name(node, prop);

	ret = simple_parse_node(priv, cpu, li, prefix, &single_cpu);
	if (ret < 0)
		goto dai_link_of_err;

	ret = simple_parse_node(priv, codec, li, prefix, NULL);
	if (ret < 0)
		goto dai_link_of_err;

	ret = asoc_simple_parse_platform(plat, platforms);
	if (ret < 0)
		goto dai_link_of_err;

	snprintf(dai_name, sizeof(dai_name),
		 "%s-%s", cpus->dai_name, codecs->dai_name);

	asoc_simple_canonicalize_cpu(cpus, single_cpu);
	asoc_simple_canonicalize_platform(platforms, cpus);

	ret = simple_link_init(priv, node, codec, li, prefix, dai_name);

dai_link_of_err:
	of_node_put(plat);
	of_node_put(node);

	li->link++;
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int asoc_simple_card_parse_aux_devs(struct device_node *node,
					   struct simple_card_data *priv)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct device_node *aux_node;
	struct snd_soc_card *card = simple_priv_to_card(priv);
	int i, n, len;

	if (!of_find_property(node, PREFIX "aux-devs", &len))
		return 0;		/* Ok to have no aux-devs */

	n = len / sizeof(__be32);
	if (n <= 0)
		return -EINVAL;

	card->aux_dev = devm_kcalloc(dev,
			n, sizeof(*card->aux_dev), GFP_KERNEL);
	if (!card->aux_dev)
		return -ENOMEM;

	for (i = 0; i < n; i++) {
		aux_node = of_parse_phandle(node, PREFIX "aux-devs", i);
		if (!aux_node)
			return -EINVAL;
		card->aux_dev[i].codec_of_node = aux_node;
	}

	card->num_aux_devs = n;
	return 0;
}

static int asoc_simple_card_parse_of(struct simple_card_data *priv)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct snd_soc_card *card = simple_priv_to_card(priv);
	struct device_node *dai_link;
	struct device_node *node = dev->of_node;
	int ret;

	if (!node)
		return -EINVAL;

	dai_link = of_get_child_by_name(node, PREFIX "dai-link");

	ret = asoc_simple_card_of_parse_widgets(card, PREFIX);
	if (ret < 0)
		goto card_parse_end;

	ret = asoc_simple_card_of_parse_routing(card, PREFIX, 1);
	if (ret < 0)
		goto card_parse_end;

	/* Factor to mclk, used in hw_params() */
	of_property_read_u32(node, PREFIX "mclk-fs", &priv->mclk_fs);

	/* Single/Muti DAI link(s) & New style of DT node */
	if (dai_link) {
		struct device_node *np = NULL;
		int i = 0;

		for_each_child_of_node(node, np) {
			dev_dbg(dev, "\tlink %d:\n", i);
			ret = asoc_simple_card_dai_link_of(np, priv,
							   i, false);
			if (ret < 0) {
				of_node_put(np);
				goto card_parse_end;
			}
			i++;
		}
	} else {
		/* For single DAI link & old style of DT node */
		ret = asoc_simple_card_dai_link_of(node, priv, 0, true);
		if (ret < 0)
			goto card_parse_end;
	}

	ret = asoc_simple_card_parse_card_name(card, PREFIX);
	if (ret < 0)
		goto card_parse_end;

	ret = asoc_simple_card_parse_aux_devs(node, priv);

card_parse_end:
	of_node_put(dai_link);
=======
static int __simple_for_each_link(struct asoc_simple_priv *priv,
			struct link_info *li,
			int (*func_noml)(struct asoc_simple_priv *priv,
					 struct device_node *np,
					 struct device_node *codec,
					 struct link_info *li, bool is_top),
			int (*func_dpcm)(struct asoc_simple_priv *priv,
					 struct device_node *np,
					 struct device_node *codec,
					 struct link_info *li, bool is_top))
{
	struct device *dev = simple_priv_to_dev(priv);
	struct device_node *top = dev->of_node;
	struct device_node *node;
	uintptr_t dpcm_selectable = (uintptr_t)of_device_get_match_data(dev);
	bool is_top = 0;
	int ret = 0;

	/* Check if it has dai-link */
	node = of_get_child_by_name(top, PREFIX "dai-link");
	if (!node) {
		node = of_node_get(top);
		is_top = 1;
	}

	/* loop for all dai-link */
	do {
		struct asoc_simple_data adata;
		struct device_node *codec;
		struct device_node *plat;
		struct device_node *np;
		int num = of_get_child_count(node);

		/* get codec */
		codec = of_get_child_by_name(node, is_top ?
					     PREFIX "codec" : "codec");
		if (!codec) {
			ret = -ENODEV;
			goto error;
		}
		/* get platform */
		plat = of_get_child_by_name(node, is_top ?
					    PREFIX "plat" : "plat");

		/* get convert-xxx property */
		memset(&adata, 0, sizeof(adata));
		for_each_child_of_node(node, np)
			simple_parse_convert(dev, np, &adata);

		/* loop for all CPU/Codec node */
		for_each_child_of_node(node, np) {
			if (plat == np)
				continue;
			/*
			 * It is DPCM
			 * if it has many CPUs,
			 * or has convert-xxx property
			 */
			if (dpcm_selectable &&
			    (num > 2 ||
			     adata.convert_rate || adata.convert_channels)) {
				/*
				 * np
				 *	 |1(CPU)|0(Codec)  li->cpu
				 * CPU	 |Pass  |return
				 * Codec |return|Pass
				 */
				if (li->cpu != (np == codec))
					ret = func_dpcm(priv, np, codec, li, is_top);
			/* else normal sound */
			} else {
				/*
				 * np
				 *	 |1(CPU)|0(Codec)  li->cpu
				 * CPU	 |Pass  |return
				 * Codec |return|return
				 */
				if (li->cpu && (np != codec))
					ret = func_noml(priv, np, codec, li, is_top);
			}

			if (ret < 0) {
				of_node_put(codec);
				of_node_put(np);
				goto error;
			}
		}

		of_node_put(codec);
		node = of_get_next_child(top, node);
	} while (!is_top && node);

 error:
	of_node_put(node);
	return ret;
}

static int simple_for_each_link(struct asoc_simple_priv *priv,
				struct link_info *li,
				int (*func_noml)(struct asoc_simple_priv *priv,
						 struct device_node *np,
						 struct device_node *codec,
						 struct link_info *li, bool is_top),
				int (*func_dpcm)(struct asoc_simple_priv *priv,
						 struct device_node *np,
						 struct device_node *codec,
						 struct link_info *li, bool is_top))
{
	int ret;
	/*
	 * Detect all CPU first, and Detect all Codec 2nd.
	 *
	 * In Normal sound case, all DAIs are detected
	 * as "CPU-Codec".
	 *
	 * In DPCM sound case,
	 * all CPUs   are detected as "CPU-dummy", and
	 * all Codecs are detected as "dummy-Codec".
	 * To avoid random sub-device numbering,
	 * detect "dummy-Codec" in last;
	 */
	for (li->cpu = 1; li->cpu >= 0; li->cpu--) {
		ret = __simple_for_each_link(priv, li, func_noml, func_dpcm);
		if (ret < 0)
			break;
	}

	return ret;
}

static int simple_parse_of(struct asoc_simple_priv *priv, struct link_info *li)
{
	struct snd_soc_card *card = simple_priv_to_card(priv);
	int ret;

	ret = asoc_simple_parse_widgets(card, PREFIX);
	if (ret < 0)
		return ret;

	ret = asoc_simple_parse_routing(card, PREFIX);
	if (ret < 0)
		return ret;

	ret = asoc_simple_parse_pin_switches(card, PREFIX);
	if (ret < 0)
		return ret;

	/* Single/Muti DAI link(s) & New style of DT node */
	memset(li, 0, sizeof(*li));
	ret = simple_for_each_link(priv, li,
				   simple_dai_link_of,
				   simple_dai_link_of_dpcm);
	if (ret < 0)
		return ret;

	ret = asoc_simple_parse_card_name(card, PREFIX);
	if (ret < 0)
		return ret;

	ret = snd_soc_of_parse_aux_devs(card, PREFIX "aux-devs");
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int asoc_simple_soc_card_probe(struct snd_soc_card *card)
{
	struct simple_card_data *priv = snd_soc_card_get_drvdata(card);
	int ret;

	ret = asoc_simple_card_init_hp(card, &priv->hp_jack, PREFIX);
	if (ret < 0)
		return ret;

	ret = asoc_simple_card_init_mic(card, &priv->mic_jack, PREFIX);
=======
static int simple_count_noml(struct asoc_simple_priv *priv,
			     struct device_node *np,
			     struct device_node *codec,
			     struct link_info *li, bool is_top)
{
	if (li->link >= SNDRV_MAX_LINKS) {
		struct device *dev = simple_priv_to_dev(priv);

		dev_err(dev, "too many links\n");
		return -EINVAL;
	}

	li->num[li->link].cpus		= 1;
	li->num[li->link].codecs	= 1;
	li->num[li->link].platforms	= 1;

	li->link += 1;

	return 0;
}

static int simple_count_dpcm(struct asoc_simple_priv *priv,
			     struct device_node *np,
			     struct device_node *codec,
			     struct link_info *li, bool is_top)
{
	if (li->link >= SNDRV_MAX_LINKS) {
		struct device *dev = simple_priv_to_dev(priv);

		dev_err(dev, "too many links\n");
		return -EINVAL;
	}

	if (li->cpu) {
		li->num[li->link].cpus		= 1;
		li->num[li->link].platforms	= 1;

		li->link++; /* CPU-dummy */
	} else {
		li->num[li->link].codecs	= 1;

		li->link++; /* dummy-Codec */
	}

	return 0;
}

static int simple_get_dais_count(struct asoc_simple_priv *priv,
				 struct link_info *li)
{
	struct device *dev = simple_priv_to_dev(priv);
	struct device_node *top = dev->of_node;

	/*
	 * link_num :	number of links.
	 *		CPU-Codec / CPU-dummy / dummy-Codec
	 * dais_num :	number of DAIs
	 * ccnf_num :	number of codec_conf
	 *		same number for "dummy-Codec"
	 *
	 * ex1)
	 * CPU0 --- Codec0	link : 5
	 * CPU1 --- Codec1	dais : 7
	 * CPU2 -/		ccnf : 1
	 * CPU3 --- Codec2
	 *
	 *	=> 5 links = 2xCPU-Codec + 2xCPU-dummy + 1xdummy-Codec
	 *	=> 7 DAIs  = 4xCPU + 3xCodec
	 *	=> 1 ccnf  = 1xdummy-Codec
	 *
	 * ex2)
	 * CPU0 --- Codec0	link : 5
	 * CPU1 --- Codec1	dais : 6
	 * CPU2 -/		ccnf : 1
	 * CPU3 -/
	 *
	 *	=> 5 links = 1xCPU-Codec + 3xCPU-dummy + 1xdummy-Codec
	 *	=> 6 DAIs  = 4xCPU + 2xCodec
	 *	=> 1 ccnf  = 1xdummy-Codec
	 *
	 * ex3)
	 * CPU0 --- Codec0	link : 6
	 * CPU1 -/		dais : 6
	 * CPU2 --- Codec1	ccnf : 2
	 * CPU3 -/
	 *
	 *	=> 6 links = 0xCPU-Codec + 4xCPU-dummy + 2xdummy-Codec
	 *	=> 6 DAIs  = 4xCPU + 2xCodec
	 *	=> 2 ccnf  = 2xdummy-Codec
	 *
	 * ex4)
	 * CPU0 --- Codec0 (convert-rate)	link : 3
	 * CPU1 --- Codec1			dais : 4
	 *					ccnf : 1
	 *
	 *	=> 3 links = 1xCPU-Codec + 1xCPU-dummy + 1xdummy-Codec
	 *	=> 4 DAIs  = 2xCPU + 2xCodec
	 *	=> 1 ccnf  = 1xdummy-Codec
	 */
	if (!top) {
		li->num[0].cpus		= 1;
		li->num[0].codecs	= 1;
		li->num[0].platforms	= 1;

		li->link = 1;
		return 0;
	}

	return simple_for_each_link(priv, li,
				    simple_count_noml,
				    simple_count_dpcm);
}

static int simple_soc_probe(struct snd_soc_card *card)
{
	struct asoc_simple_priv *priv = snd_soc_card_get_drvdata(card);
	int ret;

	ret = asoc_simple_init_hp(card, &priv->hp_jack, PREFIX);
	if (ret < 0)
		return ret;

	ret = asoc_simple_init_mic(card, &priv->mic_jack, PREFIX);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int asoc_simple_card_probe(struct platform_device *pdev)
{
	struct simple_card_data *priv;
	struct snd_soc_dai_link *dai_link;
	struct simple_dai_props *dai_props;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct snd_soc_card *card;
	int num, ret;

	/* Get the number of DAI links */
	if (np && of_get_child_by_name(np, PREFIX "dai-link"))
		num = of_get_child_count(np);
	else
		num = 1;
=======
static int asoc_simple_probe(struct platform_device *pdev)
{
	struct asoc_simple_priv *priv;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct snd_soc_card *card;
	struct link_info *li;
	int ret;
>>>>>>> upstream/android-13

	/* Allocate the private data and the DAI link array */
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

<<<<<<< HEAD
	dai_props = devm_kcalloc(dev, num, sizeof(*dai_props), GFP_KERNEL);
	dai_link  = devm_kcalloc(dev, num, sizeof(*dai_link), GFP_KERNEL);
	if (!dai_props || !dai_link)
		return -ENOMEM;

	priv->dai_props			= dai_props;
	priv->dai_link			= dai_link;

	/* Init snd_soc_card */
	card = simple_priv_to_card(priv);
	card->owner		= THIS_MODULE;
	card->dev		= dev;
	card->dai_link		= priv->dai_link;
	card->num_links		= num;
	card->probe		= asoc_simple_soc_card_probe;

	if (np && of_device_is_available(np)) {

		ret = asoc_simple_card_parse_of(priv);
=======
	card = simple_priv_to_card(priv);
	card->owner		= THIS_MODULE;
	card->dev		= dev;
	card->probe		= simple_soc_probe;
	card->driver_name       = "simple-card";

	li = devm_kzalloc(dev, sizeof(*li), GFP_KERNEL);
	if (!li)
		return -ENOMEM;

	ret = simple_get_dais_count(priv, li);
	if (ret < 0)
		return ret;

	if (!li->link)
		return -EINVAL;

	ret = asoc_simple_init_priv(priv, li);
	if (ret < 0)
		return ret;

	if (np && of_device_is_available(np)) {

		ret = simple_parse_of(priv, li);
>>>>>>> upstream/android-13
		if (ret < 0) {
			if (ret != -EPROBE_DEFER)
				dev_err(dev, "parse error %d\n", ret);
			goto err;
		}

	} else {
		struct asoc_simple_card_info *cinfo;
<<<<<<< HEAD
=======
		struct snd_soc_dai_link_component *cpus;
		struct snd_soc_dai_link_component *codecs;
		struct snd_soc_dai_link_component *platform;
		struct snd_soc_dai_link *dai_link = priv->dai_link;
		struct simple_dai_props *dai_props = priv->dai_props;
>>>>>>> upstream/android-13

		cinfo = dev->platform_data;
		if (!cinfo) {
			dev_err(dev, "no info for asoc-simple-card\n");
			return -EINVAL;
		}

		if (!cinfo->name ||
		    !cinfo->codec_dai.name ||
		    !cinfo->codec ||
		    !cinfo->platform ||
		    !cinfo->cpu_dai.name) {
			dev_err(dev, "insufficient asoc_simple_card_info settings\n");
			return -EINVAL;
		}

<<<<<<< HEAD
		card->name		= (cinfo->card) ? cinfo->card : cinfo->name;
		dai_link->name		= cinfo->name;
		dai_link->stream_name	= cinfo->name;
		dai_link->platform_name	= cinfo->platform;
		dai_link->codec_name	= cinfo->codec;
		dai_link->cpu_dai_name	= cinfo->cpu_dai.name;
		dai_link->codec_dai_name = cinfo->codec_dai.name;
		dai_link->dai_fmt	= cinfo->daifmt;
		dai_link->init		= asoc_simple_card_dai_init;
		memcpy(&priv->dai_props->cpu_dai, &cinfo->cpu_dai,
					sizeof(priv->dai_props->cpu_dai));
		memcpy(&priv->dai_props->codec_dai, &cinfo->codec_dai,
					sizeof(priv->dai_props->codec_dai));
=======
		cpus			= dai_link->cpus;
		cpus->dai_name		= cinfo->cpu_dai.name;

		codecs			= dai_link->codecs;
		codecs->name		= cinfo->codec;
		codecs->dai_name	= cinfo->codec_dai.name;

		platform		= dai_link->platforms;
		platform->name		= cinfo->platform;

		card->name		= (cinfo->card) ? cinfo->card : cinfo->name;
		dai_link->name		= cinfo->name;
		dai_link->stream_name	= cinfo->name;
		dai_link->dai_fmt	= cinfo->daifmt;
		dai_link->init		= asoc_simple_dai_init;
		memcpy(dai_props->cpu_dai, &cinfo->cpu_dai,
					sizeof(*dai_props->cpu_dai));
		memcpy(dai_props->codec_dai, &cinfo->codec_dai,
					sizeof(*dai_props->codec_dai));
>>>>>>> upstream/android-13
	}

	snd_soc_card_set_drvdata(card, priv);

<<<<<<< HEAD
=======
	asoc_simple_debug_info(priv);

>>>>>>> upstream/android-13
	ret = devm_snd_soc_register_card(dev, card);
	if (ret < 0)
		goto err;

<<<<<<< HEAD
	return 0;
err:
	asoc_simple_card_clean_reference(card);
=======
	devm_kfree(dev, li);
	return 0;
err:
	asoc_simple_clean_reference(card);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int asoc_simple_card_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	return asoc_simple_card_clean_reference(card);
}

static const struct of_device_id asoc_simple_of_match[] = {
	{ .compatible = "simple-audio-card", },
	{},
};
MODULE_DEVICE_TABLE(of, asoc_simple_of_match);
=======
static const struct of_device_id simple_of_match[] = {
	{ .compatible = "simple-audio-card", },
	{ .compatible = "simple-scu-audio-card",
	  .data = (void *)DPCM_SELECTABLE },
	{},
};
MODULE_DEVICE_TABLE(of, simple_of_match);
>>>>>>> upstream/android-13

static struct platform_driver asoc_simple_card = {
	.driver = {
		.name = "asoc-simple-card",
		.pm = &snd_soc_pm_ops,
<<<<<<< HEAD
		.of_match_table = asoc_simple_of_match,
	},
	.probe = asoc_simple_card_probe,
	.remove = asoc_simple_card_remove,
=======
		.of_match_table = simple_of_match,
	},
	.probe = asoc_simple_probe,
	.remove = asoc_simple_remove,
>>>>>>> upstream/android-13
};

module_platform_driver(asoc_simple_card);

MODULE_ALIAS("platform:asoc-simple-card");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("ASoC Simple Sound Card");
MODULE_AUTHOR("Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>");
