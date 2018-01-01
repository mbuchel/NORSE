#include <norse/file.h>
#include <norse/string.h>

// TODO: Properly implement heap space

// Root file
struct file root = {
	.name = "",
	.type = DIRECTORY,
	.read = NULL,
	.write = NULL,
	.init = NULL,
	.fini = NULL,
	.parent = NULL,
	.child_cap = 1024,
	.child_count = 0,
	.children = NULL,
	.permissions = 0755,
	.obj.ref_count = 1,
	.obj.free = NULL
};

// Device manager file
struct file device_manager = {
	.name = "dev",
	.type = DIRECTORY,
	.read = NULL,
	.write = NULL,
	.init = NULL,
	.fini = NULL,
	.parent = &root,
	.child_cap = 1024,
	.child_count = 0,
	.children = NULL,
	.permissions = 0755,
	.obj.ref_count = 1,
	.obj.free = NULL
};

/*
 * file_system_init - initialize the filesystem
 */
enum status file_system_init()
{
	return file_attach(&root, &device_manager);
}

/*
 * file_fini - closes the file
 * @file - file to close
 */
void file_fini(struct file *file)
{
	if (file == NULL)
		return;

	for (size_t i = 0; i < file->child_count; ++i)
		file_fini(file->children[i]);

	if (file->fini != NULL)
		file->fini();

	obj_release(&file->obj);
}

/*
 * file_find_child_named - find the child with a certain name
 * @file - parent file
 * @name - name of child
 * @size - size of name
 */
struct file *file_find_child_named(struct file *file, const char *name, size_t size)
{
	struct file *ret = NULL;

	if (file == NULL || name == NULL)
		goto exit_file_find_child_named;

	for (size_t i = 0; i < file->child_count; ++i) {
		if (strncmp(name, file->children[i]->name, size) == 0) {
			obj_retain(&file->children[i]->obj);
			goto exit_file_find_child_named;
		}
	}

exit_file_find_child_named:
	return ret;
}

/*
 * file_from_path - gets a file from a path
 * @path - path name
 *
 * TODO: Create strchr and use that instead
 */
struct file *file_from_path(const char *path)
{
	struct file *parent = NULL;
	const char *start = path;

	if (path == NULL)
		goto exit_file_from_path;

	parent = &root;

	for (size_t i = 0; path[i] != '\0'; ++i) {
		if (path[i] == '/') {
			parent = file_find_child_named(parent, start, path + i - start);
			start = path + i + 1;
		}
	}

exit_file_from_path:
	return parent;
}

/*
 * valid_name - checks if file name is valid
 * TODO: use strchr here as well
 */
static enum status valid_name(const char *path)
{
	enum status ret = OK;

	if (path[0] == '\0')
		ret = ERROR_INVALID;
	
	for (size_t i = 0; path[i] != '\0' && ret != ERROR_INVALID; ++i)
		if (path[i] == '/')
			ret = ERROR_INVALID;

	return ret;
}

/*
 * file_attach_path - attach to the file path
 * @parent_path - path which is parent
 * @child - child to attach to
 */
enum status file_attach_path(const char *parent_path, struct file *child)
{
	enum status ret = OK;
	struct file *parent = file_from_path(parent_path);

	if (parent == NULL) {
		ret = ERROR_INVALID;
	} else {
		file_attach(parent, child);
		obj_release(&parent->obj);
	}

	return ret;
}

/*
 * file_detach - detaches the file
 * @parent - parent file
 * @child - child file
 */
enum status file_detach(struct file *parent, struct file *child)
{
	enum status ret = ERROR_ABSENT;

	if (parent == NULL || child == NULL)
		ret = ERROR_INVALID;

	for (size_t i = 0; i < parent->child_count && ret == ERROR_ABSENT; ++i) {
		if (parent->children[i] == child) {
			parent->children[i] = parent->children[--parent->child_count];
			obj_release(&parent->obj);
			ret = OK;
		}
	}

	return ret;
}

/*
 * file_attach - attaches a file
 * @parent - parent of file
 * @child - child of file
 */
enum status file_attach(struct file *parent, struct file *child)
{
	enum status ret = OK;

	if (parent == NULL || child == NULL) {
		ret = ERROR_INVALID;
	} else {
		ret = valid_name(child->name);
	}

	if (ret == OK) {
		parent->children[parent->child_count++] = child;
		child->parent = parent;
		obj_retain(&parent->obj);
	}

	return ret;
}
