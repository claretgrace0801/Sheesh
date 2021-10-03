#include "header.h"

void rewrite_history()
{
  for (int i = 0; i < 18; i++)
  {
    history[i] = realloc(history[i], strlen(history[i + 1]) + 5);
    strcpy(history[i], history[i + 1]);
  }
}

void write_to_history()
{
  FILE *fptr;
  fptr = fopen(history_file, "w+");
  if (fptr == NULL)
  {
    perror("Error");
    return;
  }

  int hist_len = arg_len(history);
  for (int i = 0; i < hist_len; i++)
  {
    fprintf(fptr, "%s\n", history[i]);
  }
  fclose(fptr);
}

void add_to_history(char *command)
{
  int hist_len = arg_len(history);
  if (hist_len < 20)
  {
    history[hist_len] = realloc(history[hist_len], strlen(command) + 5);
    strcpy(history[hist_len], command);
  }
  else
  {
    rewrite_history();
    history[19] = realloc(history[19], strlen(command) + 5);
    strcpy(history[19], command);
  }
}

void read_from_history()
{
  FILE *fptr;
  fptr = fopen(history_file, "r");
  if (fptr == NULL)
  {
    perror("Error");
    return;
  }

  char *history_contents = malloc(5000);
  strcpy(history_contents, "");

  char c;
  c = fgetc(fptr);
  int i;
  for (i = 0; c != EOF; i++)
  {
    history_contents[i] = c;
    c = fgetc(fptr);
  }
  history_contents[i] = '\0';

  char delim[] = "\n";
  char *ptr = strtok(history_contents, delim);
  int w = 0;

  while (ptr != NULL)
  {
    history[w] = realloc(history[w], strlen(ptr) + 5);
    strcpy(history[w], ptr);
    w++;
    ptr = strtok(NULL, delim);
  }

  free(history_contents);
  fclose(fptr);
}

void call_history(char **args)
{
  int commands = 10;
  if (arg_len(args) == 2)
  {
    commands = atoi(args[1]);
    if (commands > 20)
    {
      commands = 20;
    }
  }
  else if (arg_len(args) > 2)
  {
    printf("history cannot have more than one argument.\n");
    return;
  }

  for (int i = 0; i < commands; i++)
  {
    if (history[i] == NULL)
    {
      break;
    }
    printf("%s\n", history[i]);
  }
}