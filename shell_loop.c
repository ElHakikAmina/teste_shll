#include "shell.h"
/* CREATED BY Amina El Hakik, Mehdi Belaazri */
/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t_struct *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info_fnc_shll(info);
		if (intrctv(info))
			_putsfnc("$ ");
		_eputcharfnc(BUFFER_FLSH);
		r = get_input(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_comandd(info);
		}
		else if (intrctv(info))
			_putcharfnc('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!intrctv(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(info_t_struct *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexitfnc},
		{"env", _myenvfnc},
		{"help", _myhelpfnc},
		{"history", _myhistoryfnc},
		{"setenv", _mysetenvfnc},
		{"unsetenv", _myunsetenv},
		{"cd", _mycdfnc},
		{"alias", _myaliasfnc},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmpfnc(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_comandd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_comandd(info_t_struct *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = finding_path(info, _getenvfnc(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_comand(info);
	}
	else
	{
		if ((intrctv(info) || _getenvfnc(info, "PATH=")
			|| info->argv[0][0] == '/') && is_comand(info, info->argv[0]))
			fork_comand(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_comand - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_comand(info_t_struct *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}
