#include "shell.h"

/**
 *_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @cmd: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _history(CMD_T *cmd)
{
	print_list(cmd->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @cmd: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(CMD_T *cmd, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(cmd->alias),
		get_node_index(cmd->alias, node_starts_with(cmd->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @cmd: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(CMD_T *cmd, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(cmd, str));

	unset_alias(cmd, str);
	return (add_node_end(&(cmd->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(LISTCMD *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @cmd: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _alias(CMD_T *cmd)
{
	int i = 0;
	char *p = NULL;
	LISTCMD *node = NULL;

	if (cmd->argc == 1)
	{
		node = cmd->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; cmd->argv[i]; i++)
	{
		p = _strchr(cmd->argv[i], '=');
		if (p)
			set_alias(cmd, cmd->argv[i]);
		else
			print_alias(node_starts_with(cmd->alias, cmd->argv[i], '='));
	}

	return (0);
}
