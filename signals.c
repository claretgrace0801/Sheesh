#include "header.h"

void sig(char **args)
{
  if (arg_len(args) < 3)
  {
    printf("Error: Provide all arguments\n");
    return;
  }

  int job_no = atoi(args[1]);
  int sig = atoi(args[2]);

  struct Process proc;
  int found_proc = 0;

  for (int i = 0; i < n_bg_procs; i++)
  {
    if (bg_procs[i].proc_no == job_no)
    {
      proc = bg_procs[i];
      found_proc = 1;
    }
  }

  if (!found_proc)
  {
    printf("Error: Process with job number %d not found\n", job_no);
    return;
  }

  if (kill(proc.pid, sig) < 0)
  {
    perror("Signal Error");
  }
}

void bg_exit_handler()
{
  int wstatus;
  pid_t pid = waitpid(-1, &wstatus, WNOHANG | WUNTRACED | WCONTINUED);

  while (pid <= 0)
  {
    return;
  }

  int proc_ind = get_proc_by_pid(pid);

  if (WIFEXITED(wstatus))
  {
    printf("\n%s with pid %d ended normally\n", bg_procs[proc_ind].name, pid);
  }
  else if (WIFSIGNALED(wstatus))
  {
    printf("\n%s with pid %d ended abnormally\n", bg_procs[proc_ind].name, pid);
  }
}

void signal_detection()
{

  signal(SIGCHLD, bg_exit_handler);
}