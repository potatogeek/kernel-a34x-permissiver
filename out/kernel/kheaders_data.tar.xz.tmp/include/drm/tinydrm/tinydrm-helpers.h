

#ifndef __LINUX_TINYDRM_HELPERS_H
#define __LINUX_TINYDRM_HELPERS_H

struct backlight_device;
struct tinydrm_device;
struct drm_clip_rect;
struct spi_transfer;
struct spi_message;
struct spi_device;
struct device;


static inline bool tinydrm_machine_little_endian(void)
{
#if defined(__LITTLE_ENDIAN)
	return true;
#else
	return false;
#endif
}

bool tinydrm_merge_clips(struct drm_clip_rect *dst,
			 struct drm_clip_rect *src, unsigned int num_clips,
			 unsigned int flags, u32 max_width, u32 max_height);
int tinydrm_fb_dirty(struct drm_framebuffer *fb,
		     struct drm_file *file_priv,
		     unsigned int flags, unsigned int color,
		     struct drm_clip_rect *clips,
		     unsigned int num_clips);
void tinydrm_memcpy(void *dst, void *vaddr, struct drm_framebuffer *fb,
		    struct drm_clip_rect *clip);
void tinydrm_swab16(u16 *dst, void *vaddr, struct drm_framebuffer *fb,
		    struct drm_clip_rect *clip);
void tinydrm_xrgb8888_to_rgb565(u16 *dst, void *vaddr,
				struct drm_framebuffer *fb,
				struct drm_clip_rect *clip, bool swap);
void tinydrm_xrgb8888_to_gray8(u8 *dst, void *vaddr, struct drm_framebuffer *fb,
			       struct drm_clip_rect *clip);

size_t tinydrm_spi_max_transfer_size(struct spi_device *spi, size_t max_len);
bool tinydrm_spi_bpw_supported(struct spi_device *spi, u8 bpw);
int tinydrm_spi_transfer(struct spi_device *spi, u32 speed_hz,
			 struct spi_transfer *header, u8 bpw, const void *buf,
			 size_t len);
void _tinydrm_dbg_spi_message(struct spi_device *spi, struct spi_message *m);

#ifdef DEBUG

static inline void tinydrm_dbg_spi_message(struct spi_device *spi,
					   struct spi_message *m)
{
	if (drm_debug & DRM_UT_DRIVER)
		_tinydrm_dbg_spi_message(spi, m);
}
#else
static inline void tinydrm_dbg_spi_message(struct spi_device *spi,
					   struct spi_message *m)
{
}
#endif 

#endif 
