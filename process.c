#include "header.h"

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

int get_proc_by_pid(pid_t pid)
{
  for (int i = 0; i < n_bg_procs; i++)
  {
    if (bg_procs[i].pid == pid)
    {
      return i;
    }
  }
  return -1;
}

int get_proc_status(pid_t pid)
{

  int status;

  char **parsed_stats;
  get_stats(pid, &parsed_stats);

  if (parsed_stats[2][0] == 'T')
  {
    status = 0;
  }
  else
  {
    status = 1;
  }

  free_array(&parsed_stats);
  return status;
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

  print_jobs(bg_procs, n_bg_procs);
}