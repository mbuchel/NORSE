#include <norse/object.h>

/*
 * obj_retain - aquire your object
 * @obj - object to retain
 */
void obj_retain(struct object *obj)
{
	if (obj == NULL)
		panic();

	atomic_fetch_add_explicit(&obj->ref_count, 1, memory_order_acquire);
}

/*
 * obj_release - releases the object
 * @obj - object to release
 */
void obj_release(struct object *obj)
{
	if (obj == NULL)
		panic();

	if (atomic_fetch_sub_explicit(&obj->ref_count, 1, memory_order_release) == 1 && obj->free != NULL)
		obj->free(obj);
}

/*
 * obj_set_free - sets the free command for the object
 * @obj - object to set the free command on
 * @free - free command
 */
void obj_set_free(struct object *obj, obj_free_f free)
{
	if (obj == NULL)
		panic();

	obj->free = free;
}

/*
 * obj_clr - clears the object
 * @obj - object to clear
 * @free - new free function
 */
void obj_clr(struct object *obj, obj_free_f free)
{
	if (obj == NULL)
		panic();

	obj->ref_count = ATOMIC_INT_LOCK_FREE;
	obj->free = free;
	lock_clr(&obj->lock);
}
