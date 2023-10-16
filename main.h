#ifndef MAIN_H
#define MAIN_H
#define COMMANDDELIM " \n"
#define PATHDELIM ":"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <syscall.h>

#include <errno.h>

char *readInput(void);
char **inputToTokenArr(char *inputStr);
void lookUp(char **tokensArr, char **env, char **argv);
void executeCommand(char **tokensArr, char **env, char **argv);
void freeTokens(char **tokensArr);
void printPrompt(void);

#endif /* MAIN_H */
