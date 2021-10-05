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

void append_char(char **s, char c)
{
  int n = strlen(*s);
  *s = realloc(*s, n + 2);
  (*s)[n] = c;
  (*s)[n + 1] = '\0';
}

struct ints
{
  int *arr;
  int sz;
};

void add_to_int_arr(struct ints *arr, int n)
{
  arr->arr = realloc(arr->arr, (arr->sz + 1) * sizeof(int));
  arr->arr[arr->sz] = n;
  arr->sz = arr->sz + 1;
}

int main()
{
  char *s = malloc(6);
  strcpy(s, "hello");

  struct ints arr;
  arr.arr = malloc(sizeof(int));
  arr.sz = 0;

  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 2);
  add_to_int_arr(&arr, 3);
  add_to_int_arr(&arr, 4);
  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 5);
  add_to_int_arr(&arr, 1);
  add_to_int_arr(&arr, 1);

  for (int i = 0; i < arr.sz; i++)
  {
    printf("%d\n", arr.arr[i]);
  }

  // // printf("%s", s);

  // int p[2];
  // if (pipe(p) < 0)
  // {
  //   perror("Pipe Fucked");
  // }

  // // echo

  // int echo_pid = fork();

  // close(p[1]);

  // if (echo_pid == 0)
  // {
  //   // dup2(p[1], 1);

  //   char *echo_args[3] = {"echo", "hello there", NULL};
  //   execvp("echo", echo_args);
  // }
  // else
  // {
  //   waitpid(echo_pid, NULL, 0);
  // }

  // // cat

  // int cat_pid = fork();

  // if (cat_pid == 0)
  // {
  //   dup2(p[0], 0);

  //   char *cat_args[2] = {"ls", NULL};
  //   execvp("ls", cat_args);
  // }
  // else
  // {
  //   waitpid(cat_pid, NULL, 0);
  //   close(p[0]);
  // }
}