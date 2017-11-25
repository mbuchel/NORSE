#ifndef FILE_NORSE_H
#define FILE_NORSE_H

#include <norse/types.h>

typedef enum status (init_f)(char *name);
typedef enum status (read_f)(uint8_t *out, size_t size);
typedef enum status (write_f)(const uint8_t *in, size_t size);
typedef void (fini_f)(void);

struct file {
	char *name;
	init_f *init;
	fini_f *fini
	read_f *read;
	write_f *write;
	struct file *parent;
	struct file **children;
	atomic_uint references;
	enum permissions permissions;
};

enum status vfprintf(struct file *file, const char *restrict format, va_list ap);
enum status fprintf(struct file *file, const char *restrict format, ...);

#endif
