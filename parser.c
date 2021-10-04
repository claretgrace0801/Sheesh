#include "header.h"

void separate_symbols(char *s, char ***arr)
{
  char *temp = malloc(strlen(s) + 10);
  strcpy(temp, "");

  int l = strlen(s);
  for (int i = 0; i < l; i++)
  {
    if (s[i] == '|' || s[i] == '<' || s[i] == '>')
    {
      // separate them
      if (strlen(temp))
      {
        add_to_array(arr, temp);
        strcpy(temp, "");
      }

      if (i != l - 1 && s[i + 1] == '>')
      {
        // >> case
        strcpy(temp, ">>");
        i += 2;
      }
      else
      {
        // | < > case
        sprintf(temp, "%c", s[i]);
        i++;
      }

      add_to_array(arr, temp);
      strcpy(temp, "");
    }
    else
    {
      sprintf(temp, "%s%c", temp, s[i]);
    }
  }

  if (strlen(temp))
  {
    add_to_array(arr, temp);
    strcpy(temp, "");
  }

  free(temp);
}

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

  q = malloc(sizeof(char **));

  for (int command = 0; command < commands; command++)
  {
    char delim[] = "\t\n ";
    char *ptr = strtok(strings[command], delim);

    q = realloc(q, (command + 1) * sizeof(char **));
    q[command] = malloc(sizeof(char *));
    q[command][0] = NULL;

    while (ptr != NULL)
    {
      char **separated_commands = malloc(sizeof(char *));
      separated_commands[0] = NULL;
      separate_symbols(ptr, &separated_commands);

      for (int i = 0; 1; i++)
      {
        if (separated_commands[i] == NULL)
        {
          break;
        }
        add_to_array(&(q[command]), separated_commands[i]);
      }

      ptr = strtok(NULL, delim);
    }
  }

  free(strings);
  return q;
}