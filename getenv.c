#include "shell.h"
/* CREATED BY Amina El Hakik, Mehdi Belaazri */
/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(info_t_struct *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings_SHLL_fnc(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenvfnc - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenvfnc(info_t_struct *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->string, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenvfnc - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenvfnc(info_t_struct *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlenfnc(var) + _strlenfnc(value) + 2);
	if (!buf)
		return (1);
	_strcpyfnc(buf, var);
	_strcatfnc(buf, "=");
	_strcatfnc(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with(node->string, var);
		if (p && *p == '=')
		{
			free(node->string);
			node->string = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	addingnode_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
