
#ifndef _LINUX_UUID_H_
#define _LINUX_UUID_H_

#include <uapi/linux/uuid.h>
#include <linux/string.h>

#define UUID_SIZE 16

typedef struct {
	__u8 b[UUID_SIZE];
} uuid_t;

#define UUID_INIT(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)			\
((uuid_t)								\
{{ ((a) >> 24) & 0xff, ((a) >> 16) & 0xff, ((a) >> 8) & 0xff, (a) & 0xff, \
   ((b) >> 8) & 0xff, (b) & 0xff,					\
   ((c) >> 8) & 0xff, (c) & 0xff,					\
   (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7) }})


#define	UUID_STRING_LEN		36

extern const guid_t guid_null;
extern const uuid_t uuid_null;

static inline bool guid_equal(const guid_t *u1, const guid_t *u2)
{
	return memcmp(u1, u2, sizeof(guid_t)) == 0;
}

static inline void guid_copy(guid_t *dst, const guid_t *src)
{
	memcpy(dst, src, sizeof(guid_t));
}

static inline bool guid_is_null(const guid_t *guid)
{
	return guid_equal(guid, &guid_null);
}

static inline bool uuid_equal(const uuid_t *u1, const uuid_t *u2)
{
	return memcmp(u1, u2, sizeof(uuid_t)) == 0;
}

static inline void uuid_copy(uuid_t *dst, const uuid_t *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

static inline bool uuid_is_null(const uuid_t *uuid)
{
	return uuid_equal(uuid, &uuid_null);
}

void generate_random_uuid(unsigned char uuid[16]);

extern void guid_gen(guid_t *u);
extern void uuid_gen(uuid_t *u);

bool __must_check uuid_is_valid(const char *uuid);

extern const u8 guid_index[16];
extern const u8 uuid_index[16];

int guid_parse(const char *uuid, guid_t *u);
int uuid_parse(const char *uuid, uuid_t *u);


#define uuid_le_gen(u)		guid_gen(u)
#define uuid_le_to_bin(guid, u)	guid_parse(guid, u)

static inline int uuid_le_cmp(const guid_t u1, const guid_t u2)
{
	return memcmp(&u1, &u2, sizeof(guid_t));
}

#endif
