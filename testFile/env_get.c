#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @cmd: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(CMD_T *cmd)
{
	if (!cmd->environ || cmd->env_changed)
	{
		cmd->environ = LISTCMDO_strings(cmd->env);
		cmd->env_changed = 0;
	}

	return (cmd->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @cmd: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(CMD_T *cmd, char *var)
{
	LISTCMD *node = cmd->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			cmd->env_changed = delete_node_at_index(&(cmd->env), i);
			i = 0;
			node = cmd->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (cmd->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @cmd: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(CMD_T *cmd, char *var, char *value)
{
	char *buf = NULL;
	LISTCMD *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = cmd->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			cmd->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(cmd->env), buf, 0);
	free(buf);
	cmd->env_changed = 1;
	return (0);
}
