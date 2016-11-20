#include <string.h>

void* memset(void* buf, int val, size_t size)
{
	size_t i;
	unsigned char* b = (unsigned char*) buf;

	for (i = 0; i < size; ++i)
		b[i] = (unsigned char) value;

	return buf;
}
