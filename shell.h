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
#define READING_BUFFER_SIZE 1024
/* change WRITE_BUF_SIZE to WRITE_BUFFER_SIZE*/
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLSH -1

/* for command chaining */
/* change CMD_NORM to CMD_NORM_CHAINING */
#define CMD_NORM_CHAINING	0
#define CMD_OR_CHAINING		1
#define CMD_AND_CHAINING	2
#define CMD_CHAIN_CHAINING	3

/* for convert_number() */
/* change CONVERT to CNVRT */
#define CNVRT_LOWERCASE	1
#define CNVRT_UNSIGNED	2

/* 1 if using system getline() */
/* change USE_getlinefnc to USING_getlinefnc  */
#define USING_getlinefnc 0
/* #define USE_STRTOK 0 */

/* change HIST_FILE to HIST_FILE_SHLL */
#define HIST_FILE_SHLL	".simple_shell_history"
#define HIST_MAX_SHLL	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @string: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *string;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@filename: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buffer: address of pointer to cmd_buffer, on if chaining
 *@cmd_buffer_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@hstrycount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *filename;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;
/* cmd_buffer to cmd_bufferfer */
	char **cmd_buffer; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buffer_type; /* CMD_type ||, &&, ; */
	int readfd;
	/* hstrycount par hstrycount */
	int hstrycount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* toem_shloop.c */
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_comandd(info_t *);
void fork_comand(info_t *);

/* toem_parser.c */
int is_comand(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *finding_path(info_t *, char *, char *);

/* loophshfunction.c */
int loophshfunction(char **);

/* toem_errors.c */
void _eputsfnc(char *);
int _eputcharfnc(char);
int _putfdfnc(char c, int fd);
int _putsfncfdfnc(char *string, int fd);

/* toem_string.c */
int _strlenfnc(char *);
int _strcmpfnc(char *, char *);
char *starts_with(const char *, const char *);
char *_strcatfnc(char *, char *);

/* toem_string1.c */
char *_strcpyfnc(char *, char *);
char *_strdupfnc(const char *);
void _putsfnc(char *);
int _putcharfnc(char);

/* toem_exits.c */
char *_strncpyfnc(char *, char *, int);
char *_strncatfnc(char *, char *, int);
char *_strchrfnc(char *, char);

/* toem_tokenizer.c */
char **strtowSHLL(char *, char *);
char **strtowSHLL2(char *, char);

/* toem_reallocfnc.c */
char *_memsetfnc(char *, char, unsigned int);
void filefree(char **);
void *_reallocfnc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bufferfree(void **);

/* toem_atoifnc.c */
int intrctv(info_t *);
int delim(char, char *);
int _isalphafnc(int);
int _atoifnc(char *);

/* toem_errors1.c */
int _erratoifnc(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexitfnc(info_t *);
int _mycdfnc(info_t *);
int _myhelpfnc(info_t *);

/* toem_builtin1.c */
int _myhistoryfnc(info_t *);
int _myaliasfnc(info_t *);

/*toem_getlinefnc.c */
ssize_t get_input(info_t *);
int _getlinefnc(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *_getenvfnc(info_t *, const char *);
int _myenvfnc(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);

/* toem_getenvfnc.c */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* toem_history.c */
char *get_hstry_f(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* toem_lists.c */
list_t *addingnode(list_t **, const char *, int);
list_t *addingnode_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *nodestarts(list_t *, char *, char);
ssize_t node_indexSHLL(list_t *, list_t *);

/* toem_vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
