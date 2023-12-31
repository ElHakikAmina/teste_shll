#include "shell.h"
/* CREATED BY Amina El Hakik, Mehdi Belaazri */
/**
 *_eputsfnc - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _eputsfnc(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputcharfnc(str[i]);
		i++;
	}
}

/**
 * _eputcharfnc - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _eputcharfnc(char c)
{
	static int i;
	static char buf[WRITE_BUFFER_SIZE];

	if (c == BUFFER_FLSH || i >= WRITE_BUFFER_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUFFER_FLSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putfdfnc - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putfdfnc(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUFFER_SIZE];

	if (c == BUFFER_FLSH || i >= WRITE_BUFFER_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUFFER_FLSH)
		buf[i++] = c;
	return (1);
}

/**
 *_putsfncfdfnc - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _putsfncfdfnc(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfdfnc(*str++, fd);
	}
	return (i);
}
