#include "header.h"

int check_built_in(char **args)
{
  if (strcmp(args[0], "echo") == 0 ||
      strcmp(args[0], "cd") == 0 ||
      strcmp(args[0], "pwd") == 0 ||
      strcmp(args[0], "pinfo") == 0 ||
      strcmp(args[0], "ls") == 0 ||
      strcmp(args[0], "repeat") == 0 ||
      strcmp(args[0], "history") == 0)
  {
    return 1;
  }
  return 0;
}

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

void run_job(char **args, int is_bg, struct ints inputs, struct ints outputs, int ind)
{
  // if (is_bg)
  // {
  //   signal(SIGCHLD, exit_bg_proc);
  // }

  int pid = fork();

  if (pid == -1)
  {
    perror("Process Creation Error");
    return;
  }

  if (pid == 0)
  {
    // changing IO
    dup2(inputs.arr[ind], 0);
    dup2(outputs.arr[ind], 1);

    // child
    if (is_bg)
    {
      int current_pid = getpid();
      printf("%d\n", current_pid);

      if (setpgid(0, 0) == -1)
      {
        perror("setpgid:");
        exit(1);
      }
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
      int wstatus;
      waitpid(pid, &wstatus, WUNTRACED);
    }
    close(outputs.arr[ind]);
  }
}

void replace_built_in(char **args, int *isFn, struct ints inputs, struct ints outputs, int ind)
{

  int is_built_in = check_built_in(args);

  int fd_in, fd_out;

  if (is_built_in)
  {
    *isFn = 1;
    // replace IO
    fd_in = dup(0);
    fd_out = dup(1);

    dup2(inputs.arr[ind], 0);
    dup2(outputs.arr[ind], 1);
  }

  if (strcmp(args[0], "echo") == 0)
  {
    echo(args);
  }
  else if (strcmp(args[0], "cd") == 0)
  {
    cd(args);
  }
  else if (strcmp(args[0], "pwd") == 0)
  {
    pwd();
  }
  else if (strcmp(args[0], "pinfo") == 0)
  {
    pinfo(args);
  }
  else if (strcmp(args[0], "ls") == 0)
  {
    ls(args);
  }
  else if (strcmp(args[0], "repeat") == 0)
  {
    repeat(args);
  }
  else if (strcmp(args[0], "history") == 0)
  {
    call_history(args);
  }

  if (is_built_in)
  {
    close(outputs.arr[ind]);

    dup2(fd_in, 0);
    dup2(fd_out, 1);
  }
}

void run_job_queue(char ***queue, int no_of_jobs)
{
  for (int job = 0; job < no_of_jobs; job++)
  {
    if (queue[job][0] == NULL)
    {
      continue;
    }

    char ***new_queue;
    struct ints inputs, outputs;

    int n_new_jobs = parse_pipes(&(queue[job]), &new_queue, &inputs, &outputs);
    if (n_new_jobs == 0)
    {
      break;
    }

    parse_redirection(new_queue, n_new_jobs, &inputs, &outputs);

    // save IO

    for (int new_job = 0; new_job < n_new_jobs; new_job++)
    {
      int fd_in, fd_out;
      fd_in = dup(0);
      fd_out = dup(1);

      int isFn = 0;
      if (new_queue[new_job][0] != NULL)
      {
        replace_built_in(new_queue[new_job], &isFn, inputs, outputs, new_job);
        int l = arg_len(new_queue[new_job]);

        if (isFn)
        {
          continue;
        }

        int is_bg = 0;

        if (strcmp(new_queue[new_job][l - 1], "&") == 0)
        {
          new_queue[new_job][l - 1] = NULL;
          is_bg = 1;
        }

        run_job(new_queue[new_job], is_bg, inputs, outputs, new_job);
      }
      // restore IO
      dup2(fd_in, 0);
      dup2(fd_out, 1);
    }

    for (int j = 0; j < inputs.sz; j++)
    {
      if (inputs.arr[j] != 0)
      {
        close(inputs.arr[j]);
      }
    }

    // free stuff
    free_queue(&new_queue, n_new_jobs);
    free(inputs.arr);
    free(outputs.arr);
  }
  free_queue(&queue, no_of_jobs);
}