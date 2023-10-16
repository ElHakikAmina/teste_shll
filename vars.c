#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buffer_type = CMD_OR_CHAINING;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buffer_type = CMD_AND_CHAINING;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_buffer_type = CMD_CHAIN_CHAINING;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buffer_type == CMD_AND_CHAINING)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buffer_type == CMD_OR_CHAINING)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = nodestarts(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchrfnc(node->string, '=');
		if (!p)
			return (0);
		p = _strdupfnc(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars_fnc_shll - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars_fnc_shll(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmpfnc(info->argv[i], "$?"))
		{
			replace_string_fnc_shll(&(info->argv[i]),
				_strdupfnc(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmpfnc(info->argv[i], "$$"))
		{
			replace_string_fnc_shll(&(info->argv[i]),
				_strdupfnc(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = nodestarts(info->env, &info->argv[i][1], '=');
		if (node)
		{
			replace_string_fnc_shll(&(info->argv[i]),
				_strdupfnc(_strchrfnc(node->string, '=') + 1));
			continue;
		}
		replace_string_fnc_shll(&info->argv[i], _strdupfnc(""));

	}
	return (0);
}

/**
 * replace_string_fnc_shll - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string_fnc_shll(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
