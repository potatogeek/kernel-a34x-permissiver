// SPDX-License-Identifier: GPL-2.0
/*
 * mtk-afe-platform-driver.c  --  Mediatek afe platform driver
 *
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Garlic Tseng <garlic.tseng@mediatek.com>
 */

#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <sound/soc.h>

#include "mtk-afe-platform-driver.h"
#include "mtk-base-afe.h"

int mtk_afe_combine_sub_dai(struct mtk_base_afe *afe)
{
	struct mtk_base_afe_dai *dai;
	size_t num_dai_drivers = 0, dai_idx = 0;

<<<<<<< HEAD
	/* calculate total dai driver size */
=======
	/* calcualte total dai driver size */
>>>>>>> upstream/android-13
	list_for_each_entry(dai, &afe->sub_dais, list) {
		num_dai_drivers += dai->num_dai_drivers;
	}

	dev_info(afe->dev, "%s(), num of dai %zd\n", __func__, num_dai_drivers);

	/* combine sub_dais */
	afe->num_dai_drivers = num_dai_drivers;
	afe->dai_drivers = devm_kcalloc(afe->dev,
					num_dai_drivers,
					sizeof(struct snd_soc_dai_driver),
					GFP_KERNEL);
	if (!afe->dai_drivers)
		return -ENOMEM;

	list_for_each_entry(dai, &afe->sub_dais, list) {
		/* dai driver */
		memcpy(&afe->dai_drivers[dai_idx],
		       dai->dai_drivers,
		       dai->num_dai_drivers *
		       sizeof(struct snd_soc_dai_driver));
		dai_idx += dai->num_dai_drivers;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(mtk_afe_combine_sub_dai);

int mtk_afe_add_sub_dai_control(struct snd_soc_component *component)
{
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	struct mtk_base_afe_dai *dai;

	list_for_each_entry(dai, &afe->sub_dais, list) {
		if (dai->controls)
			snd_soc_add_component_controls(component,
						       dai->controls,
						       dai->num_controls);

		if (dai->dapm_widgets)
			snd_soc_dapm_new_controls(&component->dapm,
						  dai->dapm_widgets,
						  dai->num_dapm_widgets);
	}
	/* add routes after all widgets are added */
	list_for_each_entry(dai, &afe->sub_dais, list) {
		if (dai->dapm_routes)
			snd_soc_dapm_add_routes(&component->dapm,
						dai->dapm_routes,
						dai->num_dapm_routes);
	}

	snd_soc_dapm_new_widgets(component->dapm.card);

	return 0;

}
EXPORT_SYMBOL_GPL(mtk_afe_add_sub_dai_control);

<<<<<<< HEAD
unsigned int word_size_align(unsigned int in_size)
{
	unsigned int align_size;

	/* MTK memif access need 16 bytes alignment */
	align_size = in_size & 0xFFFFFFF0;
	return align_size;
}
EXPORT_SYMBOL_GPL(word_size_align);

static snd_pcm_uframes_t mtk_afe_pcm_pointer
			 (struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	struct mtk_base_afe_memif *memif = &afe->memif[rtd->cpu_dai->id];
=======
snd_pcm_uframes_t mtk_afe_pcm_pointer(struct snd_soc_component *component,
				      struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	struct mtk_base_afe_memif *memif = &afe->memif[asoc_rtd_to_cpu(rtd, 0)->id];
>>>>>>> upstream/android-13
	const struct mtk_base_memif_data *memif_data = memif->data;
	struct regmap *regmap = afe->regmap;
	struct device *dev = afe->dev;
	int reg_ofs_base = memif_data->reg_ofs_base;
	int reg_ofs_cur = memif_data->reg_ofs_cur;
	unsigned int hw_ptr = 0, hw_base = 0;
	int ret, pcm_ptr_bytes;

	ret = regmap_read(regmap, reg_ofs_cur, &hw_ptr);
	if (ret || hw_ptr == 0) {
		dev_err(dev, "%s hw_ptr err\n", __func__);
		pcm_ptr_bytes = 0;
		goto POINTER_RETURN_FRAMES;
	}

	ret = regmap_read(regmap, reg_ofs_base, &hw_base);
	if (ret || hw_base == 0) {
		dev_err(dev, "%s hw_ptr err\n", __func__);
		pcm_ptr_bytes = 0;
		goto POINTER_RETURN_FRAMES;
	}

	pcm_ptr_bytes = hw_ptr - hw_base;

POINTER_RETURN_FRAMES:
<<<<<<< HEAD
	pcm_ptr_bytes = word_size_align(pcm_ptr_bytes);
	return bytes_to_frames(substream->runtime, pcm_ptr_bytes);
}

int mtk_afe_pcm_ack(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component =
		snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	struct mtk_base_afe_memif *memif = &afe->memif[rtd->cpu_dai->id];

	if (!memif->ack_enable)
		return 0;

	if (memif->ack)
		memif->ack(substream);
	else
		dev_warn(afe->dev, "%s(), ack_enable but ack == NULL\n",
			 __func__);

	return 0;
}

/* calculate the target DMA-buffer position to be written/read */
static void *get_dma_ptr(struct snd_pcm_runtime *runtime,
			   int channel, unsigned long hwoff)
{
	return runtime->dma_area + hwoff +
		channel * (runtime->dma_bytes / runtime->channels);
}

/* default copy_user ops for write; used for both interleaved and non- modes */
static int default_write_copy(struct snd_pcm_substream *substream,
			      int channel, unsigned long hwoff,
			      void *buf, unsigned long bytes)
{
	if (copy_from_user(get_dma_ptr(substream->runtime, channel, hwoff),
			   (void __user *)buf, bytes))
		return -EFAULT;
	return 0;
}

/* default copy_user ops for read; used for both interleaved and non- modes */
static int default_read_copy(struct snd_pcm_substream *substream,
			     int channel, unsigned long hwoff,
			     void *buf, unsigned long bytes)
{
	if (copy_to_user((void __user *)buf,
			 get_dma_ptr(substream->runtime, channel, hwoff),
			 bytes))
		return -EFAULT;
	return 0;
}

static int mtk_afe_pcm_copy_user(struct snd_pcm_substream *substream,
				 int channel, unsigned long hwoff,
				 void *buf, unsigned long bytes)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component =
		snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	int is_playback = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	mtk_sp_copy_f sp_copy;
	int ret;

	sp_copy = is_playback ? default_write_copy : default_read_copy;

	if (afe->copy) {
		ret = afe->copy(substream, channel, hwoff,
				(void __user *)buf, bytes, sp_copy);
		if (ret)
			return -EFAULT;
	} else {
		sp_copy(substream, channel, hwoff,
			(void __user *)buf, bytes);
	}

	return 0;
}

const struct snd_pcm_ops mtk_afe_pcm_ops = {
	.ioctl = snd_pcm_lib_ioctl,
	.pointer = mtk_afe_pcm_pointer,
	.ack = mtk_afe_pcm_ack,
	.copy_user = mtk_afe_pcm_copy_user,
};
EXPORT_SYMBOL_GPL(mtk_afe_pcm_ops);

int mtk_afe_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
	size_t size = 0;
	struct snd_pcm *pcm = rtd->pcm;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);
	int ret = 0;

	if (rtd->cpu_dai->id < afe->memif_size) { /* DL and UL memif pcm */
		size = afe->mtk_afe_hardware->buffer_bytes_max;
		ret = snd_pcm_lib_preallocate_pages_for_all(pcm,
							    SNDRV_DMA_TYPE_DEV,
							    afe->dev,
							    size, size);
	}

	dev_info(afe->dev, "%s(), dai_link_name : %s, memif_id : %d, size : %zu, ret : %d\n",
		 __func__,
		 rtd->dai_link->name,
		 rtd->cpu_dai->id,
		 size, ret);
	return ret;
}
EXPORT_SYMBOL_GPL(mtk_afe_pcm_new);

void mtk_afe_pcm_free(struct snd_pcm *pcm)
{
	snd_pcm_lib_preallocate_free_for_all(pcm);
}
EXPORT_SYMBOL_GPL(mtk_afe_pcm_free);

const struct snd_soc_component_driver mtk_afe_pcm_platform = {
	.name = AFE_PCM_NAME,
	.ops = &mtk_afe_pcm_ops,
	.pcm_new = mtk_afe_pcm_new,
	.pcm_free = mtk_afe_pcm_free,
=======
	return bytes_to_frames(substream->runtime, pcm_ptr_bytes);
}
EXPORT_SYMBOL_GPL(mtk_afe_pcm_pointer);

int mtk_afe_pcm_new(struct snd_soc_component *component,
		    struct snd_soc_pcm_runtime *rtd)
{
	size_t size;
	struct snd_pcm *pcm = rtd->pcm;
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);

	size = afe->mtk_afe_hardware->buffer_bytes_max;
	snd_pcm_set_managed_buffer_all(pcm, SNDRV_DMA_TYPE_DEV,
				       afe->dev, size, size);
	return 0;
}
EXPORT_SYMBOL_GPL(mtk_afe_pcm_new);

const struct snd_soc_component_driver mtk_afe_pcm_platform = {
	.name		= AFE_PCM_NAME,
	.pointer	= mtk_afe_pcm_pointer,
	.pcm_construct	= mtk_afe_pcm_new,
>>>>>>> upstream/android-13
};
EXPORT_SYMBOL_GPL(mtk_afe_pcm_platform);

MODULE_DESCRIPTION("Mediatek simple platform driver");
MODULE_AUTHOR("Garlic Tseng <garlic.tseng@mediatek.com>");
MODULE_LICENSE("GPL v2");

