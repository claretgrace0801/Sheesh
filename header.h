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
#include <ctype.h>

// structs

struct ints
{
  int *arr;
  int sz;
};

struct Process
{
  int proc_no;
  pid_t pid;
  char *name;
  int status;
  int og_bg;
};

// test
void test();

// global variables

char shell_home[1000];
int shell_pid;
char **history;
char *history_file;
struct Process bg_procs[500];
int n_bg_procs;
int proc_no;

// user functions

void display_user();

// parsers

char ***parse_commands(char *str, int *no_of_commands);
int parse_pipes(char ***commands, char ****queue, struct ints *inputs, struct ints *outputs);
void parse_redirection(char ***q, int n_q, struct ints *inputs, struct ints *outputs);

// running jobs

void run_job_queue(char ***queue, int no_of_jobs);

// processes

void get_proc_name(pid_t pid, char **name);
int get_proc_status(pid_t pid);
void add_to_bg(pid_t pid, int state, char *name, int og_bg);
void update_bg_proc();
void jobs();
void fg(char **args);
void bg(char **args);

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

// signals
void sig(char **args);
void signal_detection();

// helper functions

void get_stats(pid_t pid, char ***parsed_stats);
void initialisation(char *shell);
void arr_to_string(char **argv, char **s);
int get_proc_by_pid(pid_t pid);
void add_to_int_arr(struct ints *arr, int n);
char *cust_cwd(int shorten);
void parse_string(char **str);
int check_prefix(char *str, char *pre);
int find_proc_by_jobno(int job_no);
int arg_len(char **args);
void free_array(char ***args);
void free_queue(char ****q, int n);
char **convert_to_array(char *str);
void get_process_name(int pid, char **proc_name);
void add_to_array(char ***arr, char *s);
void print_array(char **arr);
void run_job(char **args, int is_bg, struct ints inputs, struct ints outputs, int ind);
void parse_path(char *path, char **new_path);
void replace_built_in(char **args, int *isFn, struct ints inputs, struct ints outputs, int ind);
void append_char(char **s, char c);
int is_num(char *s);

#endif