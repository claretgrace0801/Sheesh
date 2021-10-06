#include "header.h"

void add_to_bg(pid_t pid, char *name, int state)
{
  proc_no++;

  struct Process proc;

  proc.proc_no = proc_no;
  proc.pid = pid;
  proc.state = state;

  proc.name = malloc(strlen(name) + 2);
  strcpy(proc.name, name);

  bg_procs[n_bg_procs] = proc;
  n_bg_procs++;
}