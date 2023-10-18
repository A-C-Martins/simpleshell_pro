#include "shell.h"

/**
 * _cmd - determines if a file is an executable command
 * @cmd: the cmd struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int _cmd(CMD_T *cmd, char *path)
{
	struct stat st;

	(void)cmd;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * find_path - finds this cmdd in the PATH string
 * @cmd: the cmd struct
 * @pathstr: the PATH string
 * @cmdd: the cmdd to find
 *
 * Return: full path of cmdd if found or NULL
 */
char *find_path(CMD_T *cmd, char *pathstr, char *cmdd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmdd) > 2) && starts_with(cmdd, "./"))
	{
		if (_cmd(cmd, cmdd))
			return (cmdd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmdd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmdd);
			}
			if (_cmd(cmd, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
