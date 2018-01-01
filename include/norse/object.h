#ifndef OBJECT_H
#define OBJECT_H

#include <norse/types.h>
#include <norse/lock.h>

#define OBJ_CLR {0, NULL, LOCK_CLR}

// To allow typedef
struct object;

typedef void (*obj_free_f)(struct object *obj);

struct object {
	atomic_uint ref_count;
	obj_free_f free;
	struct lock lock;
};

void obj_retain(struct object *obj);
void obj_release(struct object *obj);
void obj_set_free(struct object *obj, obj_free_f free);
void obj_clr(struct object *obj, obj_free_f free);

#endif
