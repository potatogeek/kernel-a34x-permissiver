
#ifndef _LINUX_SERDEV_H
#define _LINUX_SERDEV_H

#include <linux/types.h>
#include <linux/device.h>
#include <linux/termios.h>
#include <linux/delay.h>

struct serdev_controller;
struct serdev_device;




struct serdev_device_ops {
	int (*receive_buf)(struct serdev_device *, const unsigned char *, size_t);
	void (*write_wakeup)(struct serdev_device *);
};


struct serdev_device {
	struct device dev;
	int nr;
	struct serdev_controller *ctrl;
	const struct serdev_device_ops *ops;
	struct completion write_comp;
	struct mutex write_lock;
};

static inline struct serdev_device *to_serdev_device(struct device *d)
{
	return container_of(d, struct serdev_device, dev);
}


struct serdev_device_driver {
	struct device_driver driver;
	int	(*probe)(struct serdev_device *);
	void	(*remove)(struct serdev_device *);
};

static inline struct serdev_device_driver *to_serdev_device_driver(struct device_driver *d)
{
	return container_of(d, struct serdev_device_driver, driver);
}

enum serdev_parity {
	SERDEV_PARITY_NONE,
	SERDEV_PARITY_EVEN,
	SERDEV_PARITY_ODD,
};


struct serdev_controller_ops {
	int (*write_buf)(struct serdev_controller *, const unsigned char *, size_t);
	void (*write_flush)(struct serdev_controller *);
	int (*write_room)(struct serdev_controller *);
	int (*open)(struct serdev_controller *);
	void (*close)(struct serdev_controller *);
	void (*set_flow_control)(struct serdev_controller *, bool);
	int (*set_parity)(struct serdev_controller *, enum serdev_parity);
	unsigned int (*set_baudrate)(struct serdev_controller *, unsigned int);
	void (*wait_until_sent)(struct serdev_controller *, long);
	int (*get_tiocm)(struct serdev_controller *);
	int (*set_tiocm)(struct serdev_controller *, unsigned int, unsigned int);
};


struct serdev_controller {
	struct device		dev;
	unsigned int		nr;
	struct serdev_device	*serdev;
	const struct serdev_controller_ops *ops;
};

static inline struct serdev_controller *to_serdev_controller(struct device *d)
{
	return container_of(d, struct serdev_controller, dev);
}

static inline void *serdev_device_get_drvdata(const struct serdev_device *serdev)
{
	return dev_get_drvdata(&serdev->dev);
}

static inline void serdev_device_set_drvdata(struct serdev_device *serdev, void *data)
{
	dev_set_drvdata(&serdev->dev, data);
}


static inline void serdev_device_put(struct serdev_device *serdev)
{
	if (serdev)
		put_device(&serdev->dev);
}

static inline void serdev_device_set_client_ops(struct serdev_device *serdev,
					      const struct serdev_device_ops *ops)
{
	serdev->ops = ops;
}

static inline
void *serdev_controller_get_drvdata(const struct serdev_controller *ctrl)
{
	return ctrl ? dev_get_drvdata(&ctrl->dev) : NULL;
}

static inline void serdev_controller_set_drvdata(struct serdev_controller *ctrl,
					       void *data)
{
	dev_set_drvdata(&ctrl->dev, data);
}


static inline void serdev_controller_put(struct serdev_controller *ctrl)
{
	if (ctrl)
		put_device(&ctrl->dev);
}

struct serdev_device *serdev_device_alloc(struct serdev_controller *);
int serdev_device_add(struct serdev_device *);
void serdev_device_remove(struct serdev_device *);

struct serdev_controller *serdev_controller_alloc(struct device *, size_t);
int serdev_controller_add_platform(struct serdev_controller *, bool);
void serdev_controller_remove(struct serdev_controller *);


static inline int serdev_controller_add(struct serdev_controller *ctrl)
{
	return serdev_controller_add_platform(ctrl, false);
}

static inline void serdev_controller_write_wakeup(struct serdev_controller *ctrl)
{
	struct serdev_device *serdev = ctrl->serdev;

	if (!serdev || !serdev->ops->write_wakeup)
		return;

	serdev->ops->write_wakeup(serdev);
}

static inline int serdev_controller_receive_buf(struct serdev_controller *ctrl,
					      const unsigned char *data,
					      size_t count)
{
	struct serdev_device *serdev = ctrl->serdev;

	if (!serdev || !serdev->ops->receive_buf)
		return 0;

	return serdev->ops->receive_buf(serdev, data, count);
}

#if IS_ENABLED(CONFIG_SERIAL_DEV_BUS)

int serdev_device_open(struct serdev_device *);
void serdev_device_close(struct serdev_device *);
int devm_serdev_device_open(struct device *, struct serdev_device *);
unsigned int serdev_device_set_baudrate(struct serdev_device *, unsigned int);
void serdev_device_set_flow_control(struct serdev_device *, bool);
int serdev_device_write_buf(struct serdev_device *, const unsigned char *, size_t);
void serdev_device_wait_until_sent(struct serdev_device *, long);
int serdev_device_get_tiocm(struct serdev_device *);
int serdev_device_set_tiocm(struct serdev_device *, int, int);
void serdev_device_write_wakeup(struct serdev_device *);
int serdev_device_write(struct serdev_device *, const unsigned char *, size_t, unsigned long);
void serdev_device_write_flush(struct serdev_device *);
int serdev_device_write_room(struct serdev_device *);


int __serdev_device_driver_register(struct serdev_device_driver *, struct module *);
#define serdev_device_driver_register(sdrv) \
	__serdev_device_driver_register(sdrv, THIS_MODULE)


static inline void serdev_device_driver_unregister(struct serdev_device_driver *sdrv)
{
	if (sdrv)
		driver_unregister(&sdrv->driver);
}

#define module_serdev_device_driver(__serdev_device_driver) \
	module_driver(__serdev_device_driver, serdev_device_driver_register, \
			serdev_device_driver_unregister)

#else

static inline int serdev_device_open(struct serdev_device *sdev)
{
	return -ENODEV;
}
static inline void serdev_device_close(struct serdev_device *sdev) {}
static inline unsigned int serdev_device_set_baudrate(struct serdev_device *sdev, unsigned int baudrate)
{
	return 0;
}
static inline void serdev_device_set_flow_control(struct serdev_device *sdev, bool enable) {}
static inline int serdev_device_write_buf(struct serdev_device *serdev,
					  const unsigned char *buf,
					  size_t count)
{
	return -ENODEV;
}
static inline void serdev_device_wait_until_sent(struct serdev_device *sdev, long timeout) {}
static inline int serdev_device_get_tiocm(struct serdev_device *serdev)
{
	return -ENOTSUPP;
}
static inline int serdev_device_set_tiocm(struct serdev_device *serdev, int set, int clear)
{
	return -ENOTSUPP;
}
static inline int serdev_device_write(struct serdev_device *sdev, const unsigned char *buf,
				      size_t count, unsigned long timeout)
{
	return -ENODEV;
}
static inline void serdev_device_write_flush(struct serdev_device *sdev) {}
static inline int serdev_device_write_room(struct serdev_device *sdev)
{
	return 0;
}

#define serdev_device_driver_register(x)
#define serdev_device_driver_unregister(x)

#endif 

static inline bool serdev_device_get_cts(struct serdev_device *serdev)
{
	int status = serdev_device_get_tiocm(serdev);
	return !!(status & TIOCM_CTS);
}

static inline int serdev_device_wait_for_cts(struct serdev_device *serdev, bool state, int timeout_ms)
{
	unsigned long timeout;
	bool signal;

	timeout = jiffies + msecs_to_jiffies(timeout_ms);
	while (time_is_after_jiffies(timeout)) {
		signal = serdev_device_get_cts(serdev);
		if (signal == state)
			return 0;
		usleep_range(1000, 2000);
	}

	return -ETIMEDOUT;
}

static inline int serdev_device_set_rts(struct serdev_device *serdev, bool enable)
{
	if (enable)
		return serdev_device_set_tiocm(serdev, TIOCM_RTS, 0);
	else
		return serdev_device_set_tiocm(serdev, 0, TIOCM_RTS);
}

int serdev_device_set_parity(struct serdev_device *serdev,
			     enum serdev_parity parity);


struct tty_port;
struct tty_driver;

#ifdef CONFIG_SERIAL_DEV_CTRL_TTYPORT
struct device *serdev_tty_port_register(struct tty_port *port,
					struct device *parent,
					struct tty_driver *drv, int idx);
int serdev_tty_port_unregister(struct tty_port *port);
#else
static inline struct device *serdev_tty_port_register(struct tty_port *port,
					   struct device *parent,
					   struct tty_driver *drv, int idx)
{
	return ERR_PTR(-ENODEV);
}
static inline int serdev_tty_port_unregister(struct tty_port *port)
{
	return -ENODEV;
}
#endif 

#endif 
