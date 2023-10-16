#include "shell.h"

/**
 * _myexitfnc - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int _myexitfnc(info_t_struct *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _erratoifnc(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputsfnc(info->argv[1]);
			_eputcharfnc('\n');
			return (1);
		}
		info->err_num = _erratoifnc(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _mycdfnc - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _mycdfnc(info_t_struct *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_putsfnc("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenvfnc(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenvfnc(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmpfnc(info->argv[1], "-") == 0)
	{
		if (!_getenvfnc(info, "OLDPWD="))
		{
			_putsfnc(s);
			_putcharfnc('\n');
			return (1);
		}
		_putsfnc(_getenvfnc(info, "OLDPWD=")), _putcharfnc('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenvfnc(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputsfnc(info->argv[1]), _eputcharfnc('\n');
	}
	else
	{
		_setenvfnc(info, "OLDPWD", _getenvfnc(info, "PWD="));
		_setenvfnc(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelpfnc - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myhelpfnc(info_t_struct *info)
{
	char **arg_array;

	arg_array = info->argv;
	_putsfnc("help call works. Function not yet implemented \n");
	if (0)
		_putsfnc(*arg_array); /* temp att_unused workaround */
	return (0);
}
