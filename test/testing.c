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
  char *s = malloc(50);
  strcpy(s, "hello there\n");

  // int i = 2;
  // i = NULL;

  // if (i == NULL)
  // {
  //   printf("hi\n");
  // }

  // int fd = fileno(stdout);
  // // int fd = open("hello.txt", O_WRONLY);
  // dup2(fd, 1);

  // write(1, s, strlen(s));
  // close(fd);

  int pid = fork();
  int fg = 1;

  if (pid == 0)
  {
    printf("in chiln\n");

    char *arg[2] = {"ls", NULL};
    execvp("ls", arg);
  }
  else
  {
    printf("boo ya prent\n");

    // if (write(1, s, strlen(s)) < 0)
    // {
    //   perror("f");
    // }
  }
}