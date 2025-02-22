

#ifndef __IIO_DMAENGINE_H__
#define __IIO_DMAENGINE_H__

struct iio_buffer;
struct device;

struct iio_buffer *iio_dmaengine_buffer_alloc(struct device *dev,
	const char *channel);
void iio_dmaengine_buffer_free(struct iio_buffer *buffer);

#endif
