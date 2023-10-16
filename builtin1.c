#include "shell.h"
/* CREATED BY Amina El Hakik, Mehdi Belaazri */
/**
 * _myhistoryfnc - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myhistoryfnc(info_t_struct *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t_struct *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchrfnc(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
		node_indexSHLL(info->alias, nodestarts(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t_struct *info, char *str)
{
	char *p;

	p = _strchrfnc(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (addingnode_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchrfnc(node->string, '=');
		for (a = node->string; a <= p; a++)
			_putcharfnc(*a);
		_putcharfnc('\'');
		_putsfnc(p + 1);
		_putsfnc("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myaliasfnc - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myaliasfnc(info_t_struct *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strchrfnc(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(nodestarts(info->alias, info->argv[i], '='));
	}

	return (0);
}
