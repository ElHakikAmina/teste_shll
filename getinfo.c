#include "shell.h"

/**
 * clear_info - initializes info_t_struct struct
 * @info: struct address
 */
void clear_info(info_t_struct *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes info_t_struct struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(info_t_struct *info, char **av)
{
	int i = 0;

	info->filename = av[0];
	if (info->arg)
	{
		info->argv = strtowSHLL(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdupfnc(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias_fnc_shll(info);
		replace_vars_fnc_shll(info);
	}
}

/**
 * free_info - frees info_t_struct struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t_struct *info, int all)
{
	filefree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buffer)
			free(info->arg);
		if (info->env)
			free_listfnc(&(info->env));
		if (info->history)
			free_listfnc(&(info->history));
		if (info->alias)
			free_listfnc(&(info->alias));
		filefree(info->environ);
			info->environ = NULL;
		bufferfree((void **)info->cmd_buffer);
		if (info->readfd > 2)
			close(info->readfd);
		_putcharfnc(BUFFER_FLSH);
	}
}
