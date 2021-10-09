#include "header.h"

void sig(char **args)
{
  if (arg_len(args) < 3 || !is_num(args[1]) || !is_num(args[2]))
  {
    printf("Error: Provide Valid Arguments\n");
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

void stop_process_handler()
{
  int cur_pid = getpid();
  if (cur_pid != shell_pid)
  {
    if (kill(cur_pid, SIGKILL) < 0)
    {
      perror("Kill Error");
    }
  }
}

void push_to_bg_handler()
{
  int cur_pid = getpid();
  if (cur_pid != shell_pid)
  {
    kill(cur_pid, SIGSTOP);
    exit(0);
  }
}

void signal_detection()
{
  signal(SIGCHLD, bg_exit_handler);
  signal(SIGINT, stop_process_handler);
  signal(SIGTSTP, push_to_bg_handler);
}