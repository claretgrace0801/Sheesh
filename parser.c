#include "header.h"

char ***parse_commands(char *str, int *no_of_commands)
{
  char semi[] = ";";
  char *semi_ptr = strtok(str, semi);

  char ***q;

  char **strings = malloc(sizeof(char *));
  int commands = 0;

  while (semi_ptr != NULL)
  {
    strings = realloc(strings, (commands + 1) * sizeof(char *));
    strings[commands] = malloc((strlen(semi_ptr) + 1) * sizeof(char));
    strcpy(strings[commands], semi_ptr);
    commands++;
    semi_ptr = strtok(NULL, semi);
  }

  *no_of_commands = commands;

  int str_size = strlen(str);

  q = malloc(sizeof(char **));

  for (int command = 0; command < commands; command++)
  {
    char delim[] = "\t\n ";
    char *ptr = strtok(strings[command], delim);
    int word = 0;

    q = realloc(q, (command + 1) * sizeof(char **));
    q[command] = malloc(sizeof(char *));

    while (ptr != NULL)
    {
      q[command] = realloc(q[command], (word + 2) * sizeof(char *));
      q[command][word] = malloc((strlen(ptr) + 1) * sizeof(char));
      strcpy(q[command][word], ptr);
      word++;
      ptr = strtok(NULL, delim);
    }
    q[command][word] = malloc(sizeof(char));
    q[command][word] = NULL;
  }

  free(strings);
  return q;
}