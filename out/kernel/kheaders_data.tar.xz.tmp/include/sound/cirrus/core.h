

#ifndef CIRRUS_MFD_CORE_H
#define CIRRUS_MFD_CORE_H

#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <sound/soc.h>

#define CIRRUS_MAX_AMPS			8

#define CS35L41_ALG_ID_HALO	0x400a4
#define CIRRUS_AMP_ALG_ID_HALO	0x4fa00
#define CIRRUS_AMP_ALG_ID_CSPL	0xcd

extern struct class *cirrus_amp_class;

struct cirrus_amp;

struct cs35l41_data {
	struct cs35l41_platform_data *pdata;
	struct device *dev;
	struct regmap *regmap;
	struct class *mfd_class;
	struct regulator_bulk_data supplies[2];
	struct gpio_desc *reset_gpio;
	int num_supplies;
	int irq;
};

struct cirrus_cal_control {
	const char *name;
	int alg_id;
};

struct cirrus_cal_controls {
	struct cirrus_cal_control cal_r;
	struct cirrus_cal_control cal_checksum;
	struct cirrus_cal_control cal_set_status;
};

struct cirrus_cal_ops {
	struct cirrus_cal_controls controls;
	int (*cal_start)(void);
	void (*cal_complete)(void);
	int (*v_val)(struct cirrus_amp *amps, int num_amps, bool separate);
	int (*cal_apply)(struct cirrus_amp *amp);
	int (*read_temp)(struct cirrus_amp *amp);
	int (*set_temp)(struct cirrus_amp *amp, int temperature);
};

struct cirrus_amp_config {
	struct snd_soc_component *component;
	struct regmap *regmap;
	struct reg_sequence *pre_config;
	struct reg_sequence *post_config;
	int cal_ops_idx;
	const char *dsp_part_name;
	const char *bd_prefix;
	unsigned int num_pre_configs;
	unsigned int num_post_configs;
	unsigned int mbox_cmd;
	unsigned int mbox_sts;
	unsigned int global_en;
	unsigned int global_en_mask;
	unsigned int vimon_alg_id;
	unsigned int halo_alg_id;
	unsigned int bd_alg_id;
	unsigned int bd_max_temp;
	unsigned int target_temp;
	unsigned int exit_temp;
	unsigned int default_redc;
	unsigned int cal_vpk_id;
	unsigned int cal_ipk_id;
	unsigned int cal_vsc_ub;
	unsigned int cal_vsc_lb;
	unsigned int cal_isc_ub;
	unsigned int cal_isc_lb;
	bool perform_vimon_cal;
	bool calibration_disable;
	bool pwr_enable;
	int (*amp_reinit)(struct snd_soc_component *component);
	bool runtime_pm;
};

struct cirrus_bd {
	const char *bd_suffix;
	const char *bd_prefix;
	unsigned int max_exc;
	unsigned int over_exc_count;
	unsigned int max_temp;
	unsigned int max_temp_keep;
	unsigned int over_temp_count;
	unsigned int abnm_mute;
	int max_temp_limit;
	int bd_alg_id;
};

struct cirrus_cal {
	unsigned int efs_cache_rdc;
	unsigned int efs_cache_vsc;
	unsigned int efs_cache_isc;
	unsigned int v_validation;
	unsigned int dsp_input1_cache;
	unsigned int dsp_input2_cache;
	int efs_cache_valid;
};

struct cirrus_pwr {
	unsigned int target_temp;
	unsigned int exit_temp;
	unsigned int amb_temp;
	unsigned int spk_temp;
	unsigned int passport_enable;
	bool amp_active;
};

struct cirrus_amp {
	struct regmap *regmap;
	struct snd_soc_component *component;
	struct cirrus_bd bd;
	struct cirrus_cal cal;
	struct cirrus_pwr pwr;
	struct reg_sequence *pre_config;
	struct reg_sequence *post_config;
	struct cirrus_cal_ops *cal_ops;
	const char *dsp_part_name;
	const char *mfd_suffix;
	unsigned int num_pre_configs;
	unsigned int num_post_configs;
	unsigned int mbox_cmd;
	unsigned int mbox_sts;
	unsigned int global_en;
	unsigned int global_en_mask;
	unsigned int vimon_alg_id;
	unsigned int halo_alg_id;
	unsigned int default_redc;
	unsigned int cal_vpk_id;
	unsigned int cal_ipk_id;
	unsigned int cal_vsc_ub;
	unsigned int cal_vsc_lb;
	unsigned int cal_isc_ub;
	unsigned int cal_isc_lb;
	int index;
	bool perform_vimon_cal;
	bool calibration_disable;
	bool v_val_separate;
	bool runtime_pm;
	int (*amp_reinit)(struct snd_soc_component *component);
	void (*i2c_callback)(const char *suffix);
	void (*error_callback)(const char *suffix);
};

struct cirrus_amp_group {
	struct device *bd_dev;
	struct device *cal_dev;
	struct device *pwr_dev;
	struct mutex cal_lock;
	struct mutex pwr_lock;
	struct delayed_work cal_complete_work;
	struct delayed_work pwr_work;
	struct workqueue_struct *pwr_workqueue;
	unsigned long long last_bd_update;
	unsigned int efs_cache_temp;
	unsigned int uptime_ms;
	unsigned int interval;
	unsigned int status;
	unsigned int target_min_time_ms;
	unsigned int pwr_enable;
	bool cal_running;
	int cal_retry;
	unsigned int num_amps;
	struct cirrus_amp amps[];
};

void cirrus_amp_register_i2c_error_callback(const char *suffix, void *func);
void cirrus_amp_register_error_callback(const char *suffix, void *func);

struct cirrus_amp *cirrus_get_amp_from_suffix(const char *suffix);
int cirrus_amp_add(const char *mfd_suffix, struct cirrus_amp_config cfg);
int cirrus_amp_read_ctl(struct cirrus_amp *amp, const char *name,
			int type, unsigned int id, unsigned int *value);
int cirrus_amp_write_ctl(struct cirrus_amp *amp, const char *name,
			 int type, unsigned int id, unsigned int value);

extern struct cirrus_amp_group *amp_group;

#ifdef CONFIG_SND_SOC_CS35L41
int cs35l41_i2c_probe(struct i2c_client *client,
				const struct i2c_device_id *id);

int cs35l41_i2c_remove(struct i2c_client *client);
#endif
#ifdef CONFIG_SND_SOC_CS35L43
extern const struct dev_pm_ops cs35l43_pm_ops;
int cs35l43_i2c_probe(struct i2c_client *client,
				const struct i2c_device_id *id);

int cs35l43_i2c_remove(struct i2c_client *client);
#endif
#endif
