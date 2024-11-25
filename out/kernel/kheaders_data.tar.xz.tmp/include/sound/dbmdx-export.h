

#ifndef _DBMDX_EXPORT_H
#define _DBMDX_EXPORT_H

#include <sound/soc.h>
#include <linux/i2c.h>

int dbmdx_remote_add_component_controls(struct snd_soc_component *component);

typedef void (*event_cb)(int);
void dbmdx_remote_register_event_callback(event_cb func);

enum i2c_freq_t {
	I2C_FREQ_SLOW,
	I2C_FREQ_FAST
};
typedef void (*set_i2c_freq_cb)(struct i2c_adapter*, enum i2c_freq_t);
void dbmdx_remote_register_set_i2c_freq_callback(set_i2c_freq_cb func);
#endif
