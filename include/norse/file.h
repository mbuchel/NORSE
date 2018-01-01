#ifndef FILE_H
#define FILE_H

#include <norse/object.h>

// Different file types
enum file_type {
	NORMAL,
	PIPE,
	DIRECTORY,
	DEV
};

typedef enum status (*init_f)(const char *name);
typedef enum status (*read_f)(uint8_t *in, size_t size);
typedef enum status (*write_f)(const uint8_t *out, size_t size);
typedef void (*fini_f)();

// TODO: get rid of 1024 when finished with heap space initialization
struct file {
	enum file_type type;
	const char *name;
	init_f init;
	fini_f fini;
	read_f read;
	write_f write;
	struct file *parent;
	size_t child_cap;
	size_t child_count;
	struct file *children[1024];
	enum permissions permissions;
	struct object obj;
};

enum status file_system_init();
enum status file_attach_path(const char *parent_path, struct file *child);
enum status file_attach(struct file *parent, struct file *child);
enum status file_detach(struct file *parent, struct file *child);
struct file *file_from_path(const char *path);
struct file *file_find_child_named(struct file *file, const char *name, size_t size);
void file_fini(struct file *file);

extern struct file device_manager;
extern struct file root;

#endif
