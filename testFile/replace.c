#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @cmd: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain(CMD_T *cmd, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		cmd->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		cmd->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		cmd->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @cmd: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(CMD_T *cmd, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (cmd->cmd_buf_type == CMD_AND)
	{
		if (cmd->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (cmd->cmd_buf_type == CMD_OR)
	{
		if (!cmd->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @cmd: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(CMD_T *cmd)
{
	int i;
	LISTCMD *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(cmd->alias, cmd->argv[0], '=');
		if (!node)
			return (0);
		free(cmd->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		cmd->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @cmd: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(CMD_T *cmd)
{
	int i = 0;
	LISTCMD *node;

	for (i = 0; cmd->argv[i]; i++)
	{
		if (cmd->argv[i][0] != '$' || !cmd->argv[i][1])
			continue;

		if (!_strcmp(cmd->argv[i], "$?"))
		{
			replace_string(&(cmd->argv[i]),
				_strdup(convert_number(cmd->status, 10, 0)));
			continue;
		}
		if (!_strcmp(cmd->argv[i], "$$"))
		{
			replace_string(&(cmd->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(cmd->env, &cmd->argv[i][1], '=');
		if (node)
		{
			replace_string(&(cmd->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&cmd->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
