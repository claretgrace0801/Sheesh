#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main()
{
  printf("started\n");

  pid_t pid = fork();

  if (pid == 0)
  {
    char *args[2] = {"gedit", NULL};
    execvp("gedit", args);
  }
  else
  {
    while (1)
    {
      sleep(0.3);
      printf("%d\n", kill(pid, SIGCONT));
    }
  }
}