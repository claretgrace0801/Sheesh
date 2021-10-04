#include "header.h"

void exit_bg_proc()
{

  int pid;
  int status;

  pid = waitpid(-1, &status, WNOHANG);

  if (pid < 0)
  {
    if (errno != ECHILD)
    {
      perror("BG Process Error");
    }
    return;
  }

  if (!status)
  {
    printf("\nProcess with pid %d ended normally\n", pid);
  }
  else
  {
    printf("\nProcess with pid %d ended abnormally\n", pid);
  }
}

void run_job(char **args, int is_bg)
{
  if (is_bg)
  {
    signal(SIGCHLD, exit_bg_proc);
  }

  int pid = fork();

  if (pid == -1)
  {
    perror("Process Creation Error");
    return;
  }

  if (pid == 0)
  {
    // child
    if (is_bg)
    {
      int current_pid = getpid();
      printf("%d\n", current_pid);
    }

    if (execvp(args[0], args) == -1)
    {
      perror("Exec Error");
      return;
    }
  }
  else
  {
    // parent
    if (!is_bg)
    {
      wait(NULL);
    }
  }
}

void replace_built_in(char **args, int *isFn)
{
  int command_size = strlen(shell_home) + 30;
  args[0] = realloc(args[0], command_size);

  if (strcmp(args[0], "echo") == 0)
  {
    *isFn = 1;
    echo(args);
  }
  else if (strcmp(args[0], "cd") == 0)
  {
    *isFn = 1;
    cd(args);
  }
  else if (strcmp(args[0], "pwd") == 0)
  {
    *isFn = 1;
    pwd();
  }
  else if (strcmp(args[0], "pinfo") == 0)
  {
    *isFn = 1;
    pinfo(args);
  }
  else if (strcmp(args[0], "ls") == 0)
  {
    *isFn = 1;
    ls(args);
  }
  else if (strcmp(args[0], "repeat") == 0)
  {
    *isFn = 1;
    repeat(args);
  }
  else if (strcmp(args[0], "history") == 0)
  {
    *isFn = 1;
    call_history(args);
  }
}

void run_job_queue(char ***queue, int no_of_jobs)
{
  for (int job = 0; job < no_of_jobs; job++)
  {
    int isFn = 0;
    if (queue[job][0] != NULL)
    {
      replace_built_in(queue[job], &isFn);
      int l = arg_len(queue[job]);

      if (isFn)
      {
        continue;
      }

      int is_bg = 0;

      if (strcmp(queue[job][l - 1], "&") == 0)
      {
        queue[job][l - 1] = NULL;
        is_bg = 1;
      }

      run_job(queue[job], is_bg);
    }
  }
  free_queue(&queue, no_of_jobs);
}