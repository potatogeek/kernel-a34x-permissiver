

#ifndef DECOMPRESS_UNXZ_H
#define DECOMPRESS_UNXZ_H

int unxz(unsigned char *in, long in_size,
	 long (*fill)(void *dest, unsigned long size),
	 long (*flush)(void *src, unsigned long size),
	 unsigned char *out, long *in_used,
	 void (*error)(char *x));

#endif
