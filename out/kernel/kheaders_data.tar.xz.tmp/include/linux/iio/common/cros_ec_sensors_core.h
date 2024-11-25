

#ifndef __CROS_EC_SENSORS_CORE_H
#define __CROS_EC_SENSORS_CORE_H

#include <linux/iio/iio.h>
#include <linux/irqreturn.h>
#include <linux/mfd/cros_ec.h>

enum {
	CROS_EC_SENSOR_X,
	CROS_EC_SENSOR_Y,
	CROS_EC_SENSOR_Z,
	CROS_EC_SENSOR_MAX_AXIS,
};


#define CROS_EC_SENSOR_BITS 16


#define CROS_EC_SAMPLE_SIZE  (sizeof(s64) * 2)


#define CROS_EC_MIN_SUSPEND_SAMPLING_FREQUENCY 1000  


struct cros_ec_sensors_core_state {
	struct cros_ec_device *ec;
	struct mutex cmd_lock;

	struct cros_ec_command *msg;
	struct ec_params_motion_sense param;
	struct ec_response_motion_sense *resp;

	enum motionsensor_type type;
	enum motionsensor_location loc;

	s16 calib[CROS_EC_SENSOR_MAX_AXIS];

	u8 samples[CROS_EC_SAMPLE_SIZE];

	int (*read_ec_sensors_data)(struct iio_dev *indio_dev,
				    unsigned long scan_mask, s16 *data);

	int curr_sampl_freq;
};


int cros_ec_sensors_read_lpc(struct iio_dev *indio_dev, unsigned long scan_mask,
			     s16 *data);


int cros_ec_sensors_read_cmd(struct iio_dev *indio_dev, unsigned long scan_mask,
			     s16 *data);

struct platform_device;

int cros_ec_sensors_core_init(struct platform_device *pdev,
			      struct iio_dev *indio_dev, bool physical_device);


irqreturn_t cros_ec_sensors_capture(int irq, void *p);


int cros_ec_motion_send_host_cmd(struct cros_ec_sensors_core_state *st,
				 u16 opt_length);


int cros_ec_sensors_core_read(struct cros_ec_sensors_core_state *st,
			      struct iio_chan_spec const *chan,
			      int *val, int *val2, long mask);


int cros_ec_sensors_core_write(struct cros_ec_sensors_core_state *st,
			       struct iio_chan_spec const *chan,
			       int val, int val2, long mask);

extern const struct dev_pm_ops cros_ec_sensors_pm_ops;


extern const struct iio_chan_spec_ext_info cros_ec_sensors_ext_info[];

#endif  
