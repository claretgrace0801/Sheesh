#include "header.h"

int cmp_proc(const void *a, const void *b)
{
  int res = strcmp(((struct Process *)a)->name, ((struct Process *)b)->name);

  if (res != 0)
  {
    return res;
  }
  else
  {
    return ((struct Process *)a)->proc_no - ((struct Process *)b)->proc_no;
  }
}

void add_to_bg(pid_t pid, int status, char *name)
{
  proc_no++;

  struct Process proc;

  proc.proc_no = proc_no;
  proc.pid = pid;
  proc.status = status;

  proc.name = malloc(strlen(name) + 2);
  strcpy(proc.name, name);

  bg_procs[n_bg_procs] = proc;
  n_bg_procs++;
}

void update_bg_proc()
{
  // see which processes are still running and modify BG array
  struct Process temp_proc[500];
  int temp_size = 0;

  for (int i = 0; i < n_bg_procs; i++)
  {
    if (kill(bg_procs[i].pid, 0) < 0)
    {
      continue;
    }
    temp_proc[temp_size] = bg_procs[i];
    temp_size++;
  }

  n_bg_procs = 0;
  for (int i = 0; i < temp_size; i++)
  {
    bg_procs[n_bg_procs] = temp_proc[i];

    char *name = malloc(1);
    get_proc_name(bg_procs[n_bg_procs].pid, &name);
    strcpy(bg_procs[n_bg_procs].name, name);

    bg_procs[n_bg_procs].status = get_proc_status(bg_procs[n_bg_procs].pid);

    n_bg_procs++;

    free(name);
  }
}

void print_jobs(struct Process *procs, int n_procs)
{
  for (int i = 0; i < n_procs; i++)
  {

    printf("[%d] %s %s [%d]\n",
           procs[i].proc_no,
           procs[i].status ? "Running" : "Stopped",
           procs[i].name,
           procs[i].pid);
  }
}

void jobs()
{
  qsort((void *)bg_procs, n_bg_procs, sizeof(struct Process), cmp_proc);
  print_jobs(bg_procs, n_bg_procs);
}

void fg(char **args)
{
  if (arg_len(args) < 2 || !is_num(args[1]))
  {
    printf("Error: Argument Error\n");
  }

  int job_no = atoi(args[1]);

  int proc_ind = find_proc_by_jobno(job_no);
  if (proc_ind < 0)
  {
    printf("Error: Process with job no %d not found\n", job_no);
  }

  int pid = bg_procs[proc_ind].pid;

  // change foreground to bg process

  signal(SIGTTOU, SIG_IGN);

  int terminal_gid = tcgetpgrp(0);
  tcsetpgrp(0, pid);

  if (kill(pid, SIGCONT) < 0)
  {
    perror("FG Error");
  }

  int wstatus;
  waitpid(pid, &wstatus, WUNTRACED);

  tcsetpgrp(0, terminal_gid);
  signal(SIGTTOU, SIG_DFL);
}

void bg(char **args)
{
  if (arg_len(args) < 2 || !is_num(args[1]))
  {
    printf("Error: Argument Error\n");
  }

  int job_no = atoi(args[1]);

  int proc_ind = find_proc_by_jobno(job_no);
  if (proc_ind < 0)
  {
    printf("Error: Process with job no %d not found\n", job_no);
  }

  int pid = bg_procs[proc_ind].pid;

  if (kill(pid, SIGCONT) < 0)
  {
    perror("BG Error");
  }
}