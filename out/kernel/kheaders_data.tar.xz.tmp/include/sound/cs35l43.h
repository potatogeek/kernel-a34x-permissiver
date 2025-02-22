/* SPDX-License-Identifier: GPL-2.0 */



#ifndef __CS35L43_H
#define __CS35L43_H


#define CS35L43_RX_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE)
#define CS35L43_TX_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE \
				| SNDRV_PCM_FMTBIT_S32_LE)

#define CS35L43_VALID_PDATA		0x80000000
#define CS35L43_NUM_DEFAULTS	41

struct cs35l43_platform_data {
	bool gpio1_out_enable;
	bool gpio2_out_enable;
	bool classh_disable;
	bool dsp_ng_enable;
	bool vpbr_enable;
	int asp_sdout_hiz;
	int dsp_ng_pcm_thld;
	int dsp_ng_delay;
	int dout_hiz;
	int bst_vctrl;
	int bst_ipk;
	int hw_ng_sel;
	int hw_ng_delay;
	int hw_ng_thld;
	int gpio1_src_sel;
	int gpio2_src_sel;
	int vpbr_rel_rate;
	int vpbr_wait;
	int vpbr_atk_rate;
	int vpbr_atk_vol;
	int vpbr_max_att;
	int vpbr_thld;
	const char *dsp_part_name;
	const char *mfd_suffix;
};

struct cs35l43_pll_sysclk_config {
	int freq;
	int clk_cfg;
};

struct cs35l43_fs_mon_config {
	int freq;
	unsigned int fs1;
	unsigned int fs2;
};

extern const struct cs35l43_pll_sysclk_config cs35l43_pll_sysclk[64];
extern const struct cs35l43_fs_mon_config cs35l43_fs_mon[7];
extern const unsigned int cs35l43_hibernate_update_regs[CS35L43_POWER_SEQ_LENGTH];
extern const u8 cs35l43_write_seq_op_sizes[CS35L43_POWER_SEQ_NUM_OPS][2];

enum cs35l43_hibernate_state {
	CS35L43_HIBERNATE_AWAKE		= 0,
	CS35L43_HIBERNATE_STANDBY	= 1,
	CS35L43_HIBERNATE_UPDATE	= 2,
	CS35L43_HIBERNATE_NOT_LOADED	= 3,
	CS35L43_HIBERNATE_DISABLED	= 4,
};

struct cs35l43_write_seq_elem {
	u8 size;
	u16 offset; 
	u8 operation;
	u32 *words;
	struct list_head list;
};

struct cs35l43_write_seq {
	const char *name;
	struct list_head list_head;
	unsigned int num_ops;
	unsigned int length;
};

enum cs35l43_hibernate_mode {
	CS35L43_ULTRASONIC_MODE_DISABLED = 0,
	CS35L43_ULTRASONIC_MODE_INBAND = 1,
	CS35L43_ULTRASONIC_MODE_OUT_OF_BAND = 2,
};

struct cs35l43_private {
	struct wm_adsp dsp; 
	struct snd_soc_component *component;
	struct cs35l43_platform_data pdata;
	struct device *dev;
	struct regmap *regmap;
	struct regulator_bulk_data supplies[2];
	int num_supplies;
	int irq;
	int extclk_cfg;
	int clk_id;
	int lrclk_fmt;
	int sclk_fmt;
	int asp_fmt;
	int hibernate_state;
	int hibernate_delay_ms;
	int ultrasonic_mode;
	int slot_width;
	int delta_requested;
	int delta_applied;
	unsigned int max_spi_freq;
	struct gpio_desc *reset_gpio;
	struct mutex hb_lock;
	struct mutex rate_lock;
	struct cs35l43_write_seq power_on_seq;
	void (*limit_spi_clock)(struct cs35l43_private *cs35l43, bool state);
};

int cs35l43_probe(struct cs35l43_private *cs35l43,
				struct cs35l43_platform_data *pdata);
int cs35l43_remove(struct cs35l43_private *cs35l43);

bool cs35l43_readable_reg(struct device *dev, unsigned int reg);
bool cs35l43_precious_reg(struct device *dev, unsigned int reg);
bool cs35l43_volatile_reg(struct device *dev, unsigned int reg);


int cs35l43_suspend_runtime(struct device *dev);
int cs35l43_resume_runtime(struct device *dev);

extern const struct dev_pm_ops cs35l43_pm_ops;
extern const struct reg_default cs35l43_reg[CS35L43_NUM_DEFAULTS];

#endif 
