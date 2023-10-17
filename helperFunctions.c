#include "main.h"

/*
 * read_Input - Reads user input
 * This function reads input from the user and returns it.
 *
 * Return: user input
 */
char *read_Input(void)
{
	char *input;
	size_t sizeOf_Input;
	ssize_t characters_Read;
	input = NULL;
	sizeOf_Input = 0;
	characters_Read = getline(&input, &sizeOf_Input, stdin);

	if (characters_Read == -1)
	{
		free(input);
		exit(0);
	}

	return (input);
}

/**
 * inputToken_Arr - Convert user input into an array of tokens
 * @input_Str - Input received from the user
 * This function takes a user's input and splits it into an
 * array of tokens
 *
 * Return: An array of tokens called 'commands'.
 */
char **inputToken_Arr(char *input_Str)
{
	char **commndz;
	char *commnd;
	int i;

	commndz = malloc(64);
	i = 0;

	commnd = strtok(input_Str, COMMANDDELIM);

	while (commnd != NULL)
	{
		commndz[i] = strdup(commnd);
		i++;
		commnd = strtok(NULL, COMMANDDELIM);
	}
	commndz[i] = NULL;

	return (commndz);
}

/**
 * look_Up - Look up a command in the PATH and execute
 * it if it exists
 * @token_Arr: An array of tokens representing the
 * command and its arguments
 * @env: An array of environment variables
 * @argv: An array of argument variables from main
 */
void look_Up(char **tokens_Arr, char **env, char **argv)
{
	char *pathEnv = getenv("PATH");
	char *path_Token;
	char *sPath = malloc(1024);

	if (access(tokens_Arr[0], F_OK) == 0)
	{

		execve(tokens_Arr[0], tokens_Arr, env);
	}
	else
	{
		path_Token = strtok(pathEnv, PATHDELIM);
		while (path_Token != NULL)
		{
			strcpy(sPath, path_Token);
			strcat(sPath, "/");
			strcat(sPath, tokens_Arr[0]);
			if (access(sPath, F_OK) == 0)
			{
				execve(sPath, tokens_Arr, env);
			}
			path_Token = strtok(NULL, PATHDELIM);
		}

		fprintf(stderr, "%s: 1: %s: not found\n", argv[0], tokens_Arr[0]);
		free_Tokens(tokens_Arr);
		free(sPath);
		exit(127);
	}
}

/**
 * execute_Command - Execute the command for the child process
 * @tokens_Arr: An array of tokens representing the
 * command and its arguments
 * @env: Array of environment variables
 * @argv: Array of argument variables from main
 */
void execute_Command(char **tokens_Arr, char **env, char **argv)
{
	pid_t fork_result = fork();
	int status;

	if (fork_result == 0)
	{
		look_Up(tokens_Arr, env, argv);
	}
	else if (fork_result < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(fork_result, &status, 0);
	}
}

/**
 * free_tokens - Free memory for individual tokens in the token array
 * @tokens_Arr: An array of tokens to be freed
 *
 * This function is responsible for releasing the memory allocated
 * for individual tokens within the token array.
 * It should be called to avoid memory leaks.
 */
void free_Tokens(char **tokens_Arr)
{
	int i;

	if (tokens_Arr == NULL)
	{
		return;
	}

	for (i = 0; tokens_Arr[i] != NULL; i++)
	{
		free(tokensArr[i]);
		tokens_Arr[i] = NULL;
	}
	free(tokens_Arr);
}
