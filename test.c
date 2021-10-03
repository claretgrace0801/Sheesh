#include "header.h"

void add_to_array1(char ***arr, char **s)
{
  int n = arg_len(*arr); // n
  // total size of array = n + 1

  *arr = realloc(*arr, (n + 2) * sizeof(char *));

  (*arr)[n] = realloc((*arr)[n], strlen(*s) + 20);
  strcpy((*arr)[n], *s);

  (*arr)[n + 1] = malloc(1);
  (*arr)[n + 1] = NULL;
}

void test()
{
  char *s = malloc(50);

  char **arr = malloc(2 * sizeof(char *));
  arr[0] = malloc(50);
  arr[1] = malloc(1);
  strcpy(arr[0], "hello there");
  arr[1] = NULL;

  print_array(arr);

  printf("round 1 complete\n\n");

  strcpy(s, "non on on on on o");
  add_to_array1(&arr, &s);

  print_array(arr);

  printf("round 2 complete\n\n");

  strcpy(s, "ooopsy doopsy");
  add_to_array1(&arr, &s);

  print_array(arr);

  printf("round 3 complete\n\n");

  // strcpy(s, "chiosy dipsy");
  // add_to_array1(arr, s);

  // print_array(arr);
  // printf("round 4 complete\n\n");
}
