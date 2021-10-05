#include "header.h"

void repeat(char **args)
{
  int l = arg_len(args);

  if (l < 3)
  {
    printf("repeat should have at least 2 arguments (number of times to repeat and command)\n");
    return;
  }

  int n = atoi(args[1]);

  char **new_args = malloc(sizeof(char *));
  new_args[0] = NULL;

  for (int i = 2; i < l; i++)
  {
    add_to_array(&new_args, args[i]);
  }

  for (int i = 0; i < n; i++)
  {
    char ***new_queue;
    struct ints inputs, outputs;

    int n_new_jobs = parse_pipes(&(new_args), &new_queue, &inputs, &outputs);
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

  for (int i = 0; 1; i++)
  {
    if (new_args[i] == NULL)
      break;
    free(new_args[i]);
  }
  free(new_args);
}