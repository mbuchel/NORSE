#include <limits.h>
#include <stdio.h>
#include <string.h>

static bool print(const unsigned char* data, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i) {
		if (putchar(bytes[i]) == EOF) {
			i = 0;
			goto exit_print;
		}
	}

exit_print:
	return i;
}

// TODO: optimize this function later
int printf(const char* restrict format, ...)
{
	int written = 0;
	
	char c;

	size_t maxrem;
	size_t amount;
	
	va_list parameters;

	va_start(parameters, format);

	while (*format != '\0') {
		// Max possible remaining
		maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				++format;

			amount = 1;

			while (format[amount] && format[amount] != '%')
				++amount;

			if (maxrem < amount) {
				// TODO: overflow error
				written = -1;
				goto exit_printf;
			}

			if (!print(format, amount)) {
				written = -1;
				goto exit_printf;
			}

			format += amount;
			written += amount;
		}

		++format;

		switch (*format) {
		case 'c':
			++format;

			// C treats chars as ints
			c = va_arg(parameters, int);

			if (!maxrem) {
				// TODO: overflow error
				written = -1;
				goto exit_printf;
			}

			if (!print(&c, sizeof(c))) {
				written = -1;
				goto exit_printf;
			}
			++written;
			break;
		case 's': {
			const char* str = va_arg(parameters, const char*);

			++format;

			amount = strlen(str);

			if (maxrem < amount) {
				// TODO: overflow error
				written = -1;
				goto exit_printf;
			}

			if (!print(format, amount)) {
				written = -1;
				goto exit_printf;
			}

			written += amount;
			break;
		}
		default:
			--format;

			amount = strlen(format);

			if (maxrem < amount) {
				// TODO:overflow error
				written = -1;
				goto exit_printf;
			}

			if (!print(format, amount)) {
				written = -1;
				goto exit_printf;
			}

			written += amount;
			format += amount;
			break;
		}
	}

exit_printf:
	va_end(parameters);
	return written;
}
