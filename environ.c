#include "shell.h"
/* CREATED BY Amina El Hakik, Mehdi Belaazri */
/**
 * _myenvfnc - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _myenvfnc(info_t_struct *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenvfnc - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenvfnc(info_t_struct *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->string, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenvfnc - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _mysetenvfnc(info_t_struct *info)
{
	if (info->argc != 3)
	{
		_eputsfnc("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenvfnc(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myunsetenv(info_t_struct *info)
{
	int i;

	if (info->argc == 1)
	{
		_eputsfnc("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenvfnc(info, info->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_env_list(info_t_struct *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		addingnode_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
