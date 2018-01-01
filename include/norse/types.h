#ifndef TYPES_NORSE_H
#define TYPES_NORSE_H

#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

enum permissions {
	perm_read = 1,
	perm_write = 2,
	perm_exec = 3
};

enum status {
	OK,
	ERROR_NO_MEM,
	ERROR_INVALID,
	ERROR_PERM,
	ERROR_PRES,
	ERROR_ABSENT,
	ERROR_FULL,
	ERROR_EMPTY
};

#endif
