#include "header.h"

void arr_to_string(char **argv, char **s)
{
  int words = arg_len(argv);

  int total_len = 0;
  for (int i = 0; i < words; i++)
  {
    total_len += (1 + strlen(argv[i]));
  }

  *s = realloc(*s, total_len);
  strcpy(*s, "");

  for (int i = 0; i < words; i++)
  {
    strcat(*s, argv[i]);
    strcat(*s, " ");
  }
}

void echo(char **argv)
{
  char *s = malloc(1);
  arr_to_string(argv + 1, &s);

  printf("%s\n", s);
  free(s);
}