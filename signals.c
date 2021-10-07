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

  int proc_ind = find_proc_by_jobno(job_no);

  if (proc_ind < 0)
  {
    printf("Error: Process with job number %d not found\n", job_no);
    return;
  }

  if (kill(bg_procs[proc_ind].pid, sig) < 0)
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