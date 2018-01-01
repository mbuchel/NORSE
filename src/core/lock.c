#include <norse/lock.h>

/*
 * lock_clr - clears the lock
 * @lock - lock to clear
 */
void lock_clr(struct lock *lock)
{
	lock->readers = ATOMIC_INT_LOCK_FREE;
	lock->writer = ATOMIC_BOOL_LOCK_FREE;
}

/*
 * lock_acquire_reader - acquires a reader for the lock
 * @lock - lock to acquire reader
 */
void lock_acquire_reader(struct lock *lock)
{
	atomic_bool expected = false;

	while (!atomic_compare_exchange_weak(&lock->writer, &expected, true));

	atomic_fetch_add_explicit(&lock->readers, 1, memory_order_acquire);
	atomic_store_explicit(&lock->writer, false, memory_order_release);
}

/*
 * lock_release_reader - releases a reader for the lock
 * @lock - lock to release reader from
 */
void lock_release_reader(struct lock *lock)
{
	atomic_fetch_sub_explicit(&lock->readers, 1, memory_order_release);
}

/*
 * lock_acquire_writer - acquires the writer for the lock
 * @lock - lock to acquire writer for
 */
void lock_acquire_writer(struct lock *lock)
{
	atomic_bool expected = false;
	while (!atomic_compare_exchange_weak(&lock->writer, &expected, true))
		while (atomic_load_explicit(&lock->readers, memory_order_acquire) == 0);
}

/*
 * lock_release_writer - releases the writer
 * @lock - lock to release the writer from
 */
void lock_release_writer(struct lock *lock)
{
	if (!atomic_load_explicit(&lock->writer, memory_order_acquire) || atomic_load_explicit(&lock->readers, memory_order_acquire))
		halt();

	atomic_store_explicit(&lock->writer, false, memory_order_release);
}
