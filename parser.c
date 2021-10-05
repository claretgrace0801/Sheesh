#include "header.h"

void parse_redirection(char ***q, int n_q, struct ints *inputs, struct ints *outputs)
{
  mode_t new_file_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  for (int com = 0; com < n_q; com++)
  {
    int l = arg_len(q[com]);
    for (int w = 0; w < l; w++)
    {
      if (strcmp(q[com][w], ">") == 0)
      {
        char *path = malloc(1);
        parse_path(q[com][w + 1], &path);

        int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, new_file_mode);
        if (fd < 0)
        {
          perror("Redirection Error");
          return;
        }

        free(q[com][w + 1]);

        free(q[com][w]);
        q[com][w] = NULL;

        if (outputs->arr[com] != 0 && outputs->arr[com] != 1)
        {
          close(outputs->arr[com]);
        }
        outputs->arr[com] = fd;

        free(path);
      }
      else if (strcmp(q[com][w], ">>") == 0)
      {
        char *path = malloc(1);
        parse_path(q[com][w + 1], &path);

        int fd = open(path, O_CREAT | O_APPEND | O_WRONLY, new_file_mode);
        if (fd < 0)
        {
          perror("Redirection Error");
          return;
        }

        if (outputs->arr[com] != 0 && outputs->arr[com] != 1)
        {
          close(outputs->arr[com]);
        }
        outputs->arr[com] = fd;

        free(q[com][w + 1]);

        free(q[com][w]);
        q[com][w] = NULL;

        free(path);
      }
      else if (strcmp(q[com][w], "<") == 0)
      {
        char *path = malloc(1);
        parse_path(q[com][w + 1], &path);

        int fd = open(path, O_RDONLY);
        if (fd < 0)
        {
          perror("Redirection Error");
          return;
        }

        inputs->arr[com] = fd;

        free(q[com][w + 1]);

        free(q[com][w]);
        q[com][w] = NULL;

        free(path);
      }
    }
  }
}

int parse_pipes(char ***commands, char ****queue, struct ints *inputs, struct ints *outputs)
{
  (*queue) = malloc(sizeof(char **));
  (*queue)[0] = malloc(sizeof(char *));
  (*queue)[0][0] = NULL;

  int command_ind = 0;

  // commands -> [NULL]
  for (int w = 0; 1; w++)
  {
    if ((*commands)[w] == NULL)
    {
      break;
    }

    if (strcmp((*commands)[w], "|") == 0)
    {
      int n = arg_len((*queue)[command_ind]);
      if (n > 0)
      {
        (*queue) = realloc((*queue), (command_ind + 2) * sizeof(char *));
        (*queue)[command_ind + 1] = malloc(sizeof(char *));
        (*queue)[command_ind + 1][0] = NULL;
        command_ind++;
      }
    }
    else
    {
      add_to_array(&((*queue)[command_ind]), (*commands)[w]);
    }
  }

  // making pipes

  int p[2];
  inputs->arr = malloc(sizeof(int));
  inputs->sz = 0;

  outputs->arr = malloc(sizeof(int));
  outputs->sz = 0;

  add_to_int_arr(inputs, 0);

  for (int i = 0; i < command_ind; i++)
  {
    if (pipe(p) < 0)
    {
      perror("Pipe Error");
      return 0;
    }
    add_to_int_arr(inputs, p[0]);
    add_to_int_arr(outputs, p[1]);
  }

  add_to_int_arr(outputs, 1);

  return (command_ind + 1);
}

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
        i++;
      }
      else
      {
        // | < > case
        sprintf(temp, "%c", s[i]);
      }

      add_to_array(arr, temp);
      strcpy(temp, "");
    }
    else
    {

      append_char(&temp, s[i]);
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

      for (int i = 0; 1; i++)
      {
        if (separated_commands[i] == NULL)
        {
          break;
        }
        free(separated_commands[i]);
      }
      free(separated_commands);
    }
  }

  free(strings);
  return q;
}