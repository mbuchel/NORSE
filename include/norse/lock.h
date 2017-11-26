#ifndef LOCK_H
#define LOCK_H

#include <stdint.h>

#define SPINLOCK_INIT 0

typedef uint8_t spinlock_t;

void acquire_spinlock(spinlock_t *s);

uint8_t release_spinlock(spinlock_t *s);

#endif
