#include "header.h"

int main(int argc, char **argv)
{
  initialisation(argv[0]);

  // test();
  // return 0;

  // needed to get input
  char *inp = (char *)malloc(sizeof(char) * 1000);
  ssize_t *n = (ssize_t *)malloc(sizeof(ssize_t));
  *n = 1000;

  // needed to parse input
  char ***jobs;
  int no_of_jobs;

  while (1)
  {
    // displays username, hostname
    display_user();

    // take input
    if (getline(&inp, n, stdin) == -1)
    {
      printf("\n");
      return 0;
    }

    // parse input
    jobs = parse_commands(inp, &no_of_jobs);

    // pass to executer
    run_job_queue(jobs, no_of_jobs);

    if (inp[0] != '\n')
    {
      add_to_history(inp);
    }
    write_to_history();
  }
}