#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct ListStr
{
	int num;
	char *str;
	struct ListStr *next;
} LISTCMD;


/**
 *struct PASScmd - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct PASScmd
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	LISTCMD *env;
	LISTCMD *history;
	LISTCMD *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} CMD_T;

#define cmd_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct BUILTINCMD
{
	char *type;
	int (*func)(CMD_T *);
} builtin_table;


/* function_shloop.c */
int hsh(CMD_T *, char **);
int built_in(CMD_T *);
void check_cmd(CMD_T *);
void fork_pro(CMD_T *);

/* function_parser.c */
int _cmd(CMD_T *, char *);
char *dup_chars(char *, int, int);
char *find_path(CMD_T *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* function_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* function_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* function_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* hnd_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* hnd_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* function_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* function_memory.c */
int pfree(void **);

/* function_atoi.c */
int interactive(CMD_T *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* function_errors1.c */
int _erratoi(char *);
void print_error(CMD_T *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* function_builtin.c */
int _myexit(CMD_T *);
int _mycd(CMD_T *);
int _myhelp(CMD_T *);

/* function_builtin1.c */
int _history(CMD_T *);
int _alias(CMD_T *);

/*function_getline.c */
ssize_t get_input(CMD_T *);
int _getline(CMD_T *, char **, size_t *);
void sigintHandler(int);

/* function_getcmd.c */
void clear_cmd(CMD_T *);
void set_cmd(CMD_T *, char **);
void free_cmd(CMD_T *, int);

/* function_environ.c */
char *_getenv(CMD_T *, const char *);
int _env(CMD_T *);
int _mysetenv(CMD_T *);
int _myunsetenv(CMD_T *);
int populate_env_list(CMD_T *);

/* function_getenv.c */
char **get_environ(CMD_T *);
int _unsetenv(CMD_T *, char *);
int _setenv(CMD_T *, char *, char *);

/* function_history.c */
char *get_history_file(CMD_T *cmd);
int write_history(CMD_T *cmd);
int read_history(CMD_T *cmd);
int build_history_list(CMD_T *cmd, char *buf, int linecount);
int renumber_history(CMD_T *cmd);

/* function_lists.c */
LISTCMD *add_node(LISTCMD **, const char *, int);
LISTCMD *add_node_end(LISTCMD **, const char *, int);
size_t print_list_str(const LISTCMD *);
int delete_node_at_index(LISTCMD **, unsigned int);
void free_list(LISTCMD **);

/* function_lists1.c */
size_t list_len(const LISTCMD *);
char **LISTCMDO_strings(LISTCMD *);
size_t print_list(const LISTCMD *);
LISTCMD *node_starts_with(LISTCMD *, char *, char);
ssize_t get_node_index(LISTCMD *, LISTCMD *);

/* function_vars.c */
int is_chain(CMD_T *, char *, size_t *);
void check_chain(CMD_T *, char *, size_t *, size_t, size_t);
int replace_alias(CMD_T *);
int replace_vars(CMD_T *);
int replace_string(char **, char *);

#endif
