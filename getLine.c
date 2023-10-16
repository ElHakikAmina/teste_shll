#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bufferfree((void **)info->cmd_buffer);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USING_getlinefnc
		r = getline(buf, &len_p, stdin);
#else
		r = _getlinefnc(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->hstrycount++);
			/* if (_strchrfnc(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buffer = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putcharfnc(BUFFER_FLSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		check_chain_fnc_shll(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chainSHLL(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmd_buffer_type = CMD_NORM_CHAINING;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlenfnc(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getlinefnc() */
	return (r); /* return length of buffer from _getlinefnc() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, READING_BUFFER_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getlinefnc - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getlinefnc(info_t *info, char **ptr, size_t *length)
{
	static char buf[READING_BUFFER_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchrfnc(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _reallocfnc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncatfnc(new_p, buf + i, k - i);
	else
		_strncpyfnc(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_putsfnc("\n");
	_putsfnc("$ ");
	_putcharfnc(BUFFER_FLSH);
}
