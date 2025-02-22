/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_DATA_H
#define __PERF_DATA_H

<<<<<<< HEAD
=======
#include <stdio.h>
>>>>>>> upstream/android-13
#include <stdbool.h>

enum perf_data_mode {
	PERF_DATA_MODE_WRITE,
	PERF_DATA_MODE_READ,
};

<<<<<<< HEAD
struct perf_data_file {
	const char	*path;
	int		 fd;
};

struct perf_data {
	struct perf_data_file	 file;
	bool			 is_pipe;
	bool			 force;
	unsigned long		 size;
	enum perf_data_mode	 mode;
=======
enum perf_dir_version {
	PERF_DIR_SINGLE_FILE	= 0,
	PERF_DIR_VERSION	= 1,
};

struct perf_data_file {
	char		*path;
	union {
		int	 fd;
		FILE	*fptr;
	};
	unsigned long	 size;
};

struct perf_data {
	const char		*path;
	struct perf_data_file	 file;
	bool			 is_pipe;
	bool			 is_dir;
	bool			 force;
	bool			 use_stdio;
	bool			 in_place_update;
	enum perf_data_mode	 mode;

	struct {
		u64			 version;
		struct perf_data_file	*files;
		int			 nr;
	} dir;
>>>>>>> upstream/android-13
};

static inline bool perf_data__is_read(struct perf_data *data)
{
	return data->mode == PERF_DATA_MODE_READ;
}

static inline bool perf_data__is_write(struct perf_data *data)
{
	return data->mode == PERF_DATA_MODE_WRITE;
}

static inline int perf_data__is_pipe(struct perf_data *data)
{
	return data->is_pipe;
}

<<<<<<< HEAD
static inline int perf_data__fd(struct perf_data *data)
{
	return data->file.fd;
}

static inline unsigned long perf_data__size(struct perf_data *data)
{
	return data->size;
=======
static inline bool perf_data__is_dir(struct perf_data *data)
{
	return data->is_dir;
}

static inline bool perf_data__is_single_file(struct perf_data *data)
{
	return data->dir.version == PERF_DIR_SINGLE_FILE;
}

static inline int perf_data__fd(struct perf_data *data)
{
	if (data->use_stdio)
		return fileno(data->file.fptr);

	return data->file.fd;
>>>>>>> upstream/android-13
}

int perf_data__open(struct perf_data *data);
void perf_data__close(struct perf_data *data);
<<<<<<< HEAD
=======
ssize_t perf_data__read(struct perf_data *data, void *buf, size_t size);
>>>>>>> upstream/android-13
ssize_t perf_data__write(struct perf_data *data,
			      void *buf, size_t size);
ssize_t perf_data_file__write(struct perf_data_file *file,
			      void *buf, size_t size);
/*
 * If at_exit is set, only rename current perf.data to
 * perf.data.<postfix>, continue write on original data.
 * Set at_exit when flushing the last output.
 *
 * Return value is fd of new output.
 */
int perf_data__switch(struct perf_data *data,
			   const char *postfix,
<<<<<<< HEAD
			   size_t pos, bool at_exit);
=======
			   size_t pos, bool at_exit, char **new_filepath);

int perf_data__create_dir(struct perf_data *data, int nr);
int perf_data__open_dir(struct perf_data *data);
void perf_data__close_dir(struct perf_data *data);
int perf_data__update_dir(struct perf_data *data);
unsigned long perf_data__size(struct perf_data *data);
int perf_data__make_kcore_dir(struct perf_data *data, char *buf, size_t buf_sz);
char *perf_data__kallsyms_name(struct perf_data *data);
bool is_perf_data(const char *path);
>>>>>>> upstream/android-13
#endif /* __PERF_DATA_H */
