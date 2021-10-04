#ifndef HEADER
#define HEADER
#define UNUSED(x) (void)(x)

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// test
void test();

// global variables

char shell_home[1000];
int shell_pid;
char **history;
char *history_file;

// user functions

void display_user();

// parsers

char ***parse_commands(char *str, int *no_of_commands);

// running jobs

void run_job_queue(char ***queue, int no_of_jobs);

// implemented

void cd(char **args);
void echo(char **argv);
void pwd();
void pinfo(char **args);
void ls(char **args);
void repeat(char **args);

// history
void call_history(char **args);
void read_from_history();
void write_to_history();
void add_to_history(char *command);

// helper functions

void initialisation(char *shell);
char *cust_cwd(int shorten);
void parse_string(char **str);
int check_prefix(char *str, char *pre);
int arg_len(char **args);
int isFine;
void free_queue(char ****q, int n);
char **convert_to_array(char *str);
void get_process_name(int pid, char **proc_name);
void add_to_array(char ***arr, char *s);
void print_array(char **arr);
void run_fg_job(char **args);
void parse_path(char *path, char **new_path);
void replace_built_in(char **args, int *isFn);

#endif