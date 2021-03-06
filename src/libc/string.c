/*
 * This is my libc/libk implementation I know
 * it is common practice to seperate them but
 * for right now I am putting them in one library
 * implementation
 */
#include <norse/string.h>

/*
 * memset - sets the bytes of an array to a specific value
 * @s - array to set
 * @c - byte to fill area with
 * @count - number of bytes to be set
 */
void *memset(void *s, const uint8_t c, size_t count)
{
	uint8_t *temp = (uint8_t*) s;

	while (count--)
		*temp++ = c;

	return s;
}

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
 * strncpy - copy string for len bytes, if str < len then NULL padding
 * @dest - destination of copy
 * @src - source of copy
 * @len - size of the str we are copying
 */
char *strncpy(char *dest, const char *src, size_t len)
{
	char *ret = dest;

	while (len--) {
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
char tolower(char c)
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
