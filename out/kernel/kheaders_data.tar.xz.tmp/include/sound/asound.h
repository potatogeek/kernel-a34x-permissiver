
#ifndef __SOUND_ASOUND_H
#define __SOUND_ASOUND_H

#include <linux/ioctl.h>
#include <linux/time.h>
#include <asm/byteorder.h>

#ifdef  __LITTLE_ENDIAN
#define SNDRV_LITTLE_ENDIAN
#else
#ifdef __BIG_ENDIAN
#define SNDRV_BIG_ENDIAN
#else
#error "Unsupported endian..."
#endif
#endif

#include <uapi/sound/asound.h>
#endif 
