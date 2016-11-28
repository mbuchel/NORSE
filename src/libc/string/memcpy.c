#include <string.h>

void* memcpy(void* dst, const void* src, size_t len)
{
	size_t i;

	/*
	 * Does not support overlapping buffers
	 * uses words as the copy movement instead
	 * of bytes, as the computer processor can
	 * handle byte copies
	 */
	if ((uintptr_t)dst % sizeof(long) == 0 && (uintptr_t)src % sizeof(long) == 0) {
		long *d = dst;
		const long *s = src;

		for (i = 0; i < len/sizeof(long); ++i)
			d[i] = s[i];
	}

	return dst;
}
