#ifndef PANIC_NORSE_H
#define PANIC_NORSE_H

#include <norse/cpu.h>
#include <norse/log.h>

#define assert(x) \
	if (!x) { \
		logf("Assertion failed: (%s) function %s, file %s, line %d.", \
			(#x), __func__, __FILE__, __LINE__); \
		panic(); \
	}

#define panic() {halt();}

#endif
