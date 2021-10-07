#include "header.h"

void test()
{
  int wstatus = 127;

  // printf("%d\n", WIFEXITED(wstatus));
  // printf("%d\n", WIFSIGNALED(wstatus));
  printf("%d\n", WIFSTOPPED(wstatus));
  // printf("%d\n", WEXITSTATUS(wstatus));
  // printf("%d\n", WTERMSIG(wstatus));
  // printf("%d\n", WCOREDUMP(wstatus));
  // printf("%d\n", WSTOPSIG(wstatus));
  // printf("%d\n", WIFCONTINUED(wstatus));
}
