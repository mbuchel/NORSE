#ifndef STRING_H
#define STRING_H

#include <stdint.h>

char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t count);
char *strncpy(char *dest, const char *src, size_t len);

char tolower(const char c);

int strcmp(const char *str1, const char *str2);
int strcasecmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t len);
int strncasecmp(const char *str1, const char *str2, size_t len);

size_t strlcpy(char *dest, const char *src, size_t len);

#endif
