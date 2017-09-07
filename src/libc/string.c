/*
 * This is my libc/libk implementation I know
 * it is common practice to seperate them but
 * for right now I am putting them in one library
 * implementation
 */
#include <string.h>

/*
 * strcpy - copy NULL terminated string
 * @dest - destination of copy
 * @src - source of copy
 */
char *strcpy(char *dest, const char *src)
{
	char *ret = dest;

	while ((*dest++ = *src++) != 0x00);

	return ret;
}

/*
 * strcpy - copy string for len bytes, if str < len then NULL padding
 * @dest - destination of copy
 * @src - source of copy
 * @len - size of the str we are copying
 */
char *strncpy(char *dest, const char *src, size_t len)
{
	char *ret = dest;

	while (count--) {
		if ((*dest = *src) != 0x00)
			++src;
		++dest;
	}

	return ret;
}

/*
 * tolower - makes character lower case
 * @c - character to make lower case
 */
char tolower(const char c)
{
	if (c >= 0x41 && c <= 0x5A)
		c |= 0x20;

	return c;
}

/*
 * strcmp - compares 2 strings
 * @str1 - first string to compare
 * @str2 - second string to compare
 */
int strcmp(const char *str1, const char *str2)
{
	while (*str1++ == *str2++ && !*str1);

	return *str1 - *str2;
}

/*
 * strcasecmp - case insensitive strcmp
 * @str1 - first string to compare
 * @str2 - second string to compare
 */
int strcasecmp(const char *str1, const char *str2)
{
	unsigned char c1;
	unsigned char c2;

	do {
		c1 = tolower(*str1++);
		c2 = tolower(*str2++);
	} while (c1 == c2 && !c1);

	return c1 - c2;
}

/*
 * strncmp - compares 2 strings
 * @str1 - first string to compare
 * @str2 - second string to compare
 * @len - length to compare
 */
int strncmp(const char *str1, const char *str2, size_t len)
{
	while (*str1++ == *str2++ && !*str1 && len--);

	return *str1 - *str2;
}

/*
 * strncasecmp - case insensitive strncmp
 * @str1 - first string to compare
 * @str2 - second string to compare
 * @len - length to compare
 */
int strncasecmp(const char *str1, const char *str2, size_t len)
{
	unsigned char c1 = 0;
	unsigned char c2 = 0;
	int sum;

	while (len--) {
		c1 = *str1++;
		c2 = *str2++;

		// Checks if null character is hit
		if (!c1 || !c2)
			break;

		if (c1 == c2)
			continue;

		c1 = tolower(c1);
		c2 = tolower(c2);

		if (c1 != c2)
			break;
	}

	// Granted this can be more optimal by only computing this if we compute this only if len > 0
	sum = (int) c1 - (int) c2;

	return sum;
}

/*
 * strlcpy - copies str into sized buffer no padding and returns the size
 * @dest - copy destination
 * @src - copy source
 * @size - destination buffer
 *
 * mainly for BSD support later on
 */
size_t strlcpy(char *dest, const char *src, size_t len)
{
	size_t ret = strlen(src);

	if (len) {
		len = (ret >= len) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = 0x00;
	}

	return ret;
}

/*
 * strscpy - copies str into sized buffer
 * @dest - cpy destination
 * @src - cpy source
 * @len - size of destination buffer
 *
 * linux prefers this to strlcpy
 *
 * however undefined if the strbuffers overlap
 */
ssize_t strscpy(char *dest, const char *src, size_t len)
{
	// TODO: make this still
}
