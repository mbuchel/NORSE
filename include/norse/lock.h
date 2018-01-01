#ifndef LOCK_H
#define LOCK_H

#include <norse/panic.h>
#include <norse/types.h>

#define LOCK_CLR {0, false}

struct lock {
	atomic_uint readers;
	atomic_bool writer;
};

void lock_clr(struct lock *lock);

void lock_acquire_reader(struct lock *lock);
void lock_release_reader(struct lock *lock);

void lock_acquire_writer(struct lock *lock);
void lock_release_writer(struct lock *lock);

#endif
