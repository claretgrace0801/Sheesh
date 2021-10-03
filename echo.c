#include "header.h"

void echo(char **argv)
{
  int words = arg_len(argv) - 1;

  int total_len = 0;
  for (int i = 1; i <= words; i++)
  {
    total_len += (1 + strlen(argv[i]));
  }

  char *s = malloc(total_len);
  strcpy(s, "");

  for (int i = 1; i <= words; i++)
  {
    strcat(s, argv[i]);
    strcat(s, " ");
  }

  parse_string(&s);

  printf("%s\n", s);
  free(s);
}