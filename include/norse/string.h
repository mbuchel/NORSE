#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void *memset(void *s, const uint8_t c, size_t count);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t len);

char tolower(char c);

int strcmp(const char *str1, const char *str2);
int strcasecmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t len);
int strncasecmp(const char *str1, const char *str2, size_t len);

#endif
