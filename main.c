#include "main.h"

/*
 * print_Prompt - This function is responsible for printing
 * the prompt only when operating in interactive mode.
 */
void print_Prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "$ ", 3);
		fflush(stdout);
	}
}

/**
 * main - Entry point of the program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 * @env: Array of environment variables
 * Return: 0 for success, non-zero for errors
 */
int main(int argc, char **argv, char **env)
{
	(void)argc;
	while (true)
	{
		char *user_Input;
		char *command_Tokens;
		int i = 0;

		print_Prompt();
		user_Input = readInput();

		if (strcmp(user_Input, "exit\n") == 0)
		{	
			free(user_Input);
			exit(EXIT_SUCCESS);
		}

		if (strcmp(user_Input, "env\n") == 0)
		{
			free(user_Input);
			while (env[i] != NULL)
			{
				printf("%s\n", env[i]);
				i++;
			}
		}

		command_Tokens = inputToTokenArr(userInput);
		if (command_Tokens == NULL)
		{
			free(command_Tokens);
		}

		if (command_Tokens[0] != NULL)
		{
			executeCommand(command_Tokens, env, argv);
		}
		freeTokens(command_Tokens);
		free(user_Input);
	}
	return (0);
}

		
