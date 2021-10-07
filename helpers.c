#include "header.h"

char helper_buf[1000];

int is_num(char *s)
{
  int l = strlen(s);
  if (l == 0)
  {
    return 0;
  }

  for (int i = 0; i < l; i++)
  {
    if (isdigit(s[i]))
    {
      continue;
    }
    return 0;
  }
  return 1;
}

int get_proc_by_pid(pid_t pid)
{
  for (int i = 0; i < n_bg_procs; i++)
  {
    if (bg_procs[i].pid == pid)
    {
      return i;
    }
  }
  return -1;
}

int find_proc_by_jobno(int job_no)
{
  for (int i = 0; i < n_bg_procs; i++)
  {
    if (bg_procs[i].proc_no == job_no)
    {
      return i;
    }
  }
  return -1;
}

int get_proc_status(pid_t pid)
{

  int status;

  char **parsed_stats;
  get_stats(pid, &parsed_stats);

  if (parsed_stats[2][0] == 'T')
  {
    status = 0;
  }
  else
  {
    status = 1;
  }

  free_array(&parsed_stats);
  return status;
}

void add_to_int_arr(struct ints *arr, int n)
{
  arr->arr = realloc(arr->arr, (arr->sz + 1) * sizeof(int));
  arr->arr[arr->sz] = n;
  arr->sz = arr->sz + 1;
}

void get_path(char *file)
{
  int l = strlen(file);
  for (int i = l - 1; i >= 0; i--)
  {
    if (file[i] == '/')
    {
      file[i] = '\0';
      return;
    }
  }
}

void initialisation(char *shell)
{
  signal_detection();

  shell_pid = getpid();
  get_path(shell);
  char *args[3] = {"cd", shell, NULL};
  cd(args);
  if (getcwd(shell_home, 1000) == NULL)
  {
    perror("Initialisation Error");
    return;
  }

  history = malloc(30 * sizeof(char *));

  for (int i = 0; i <= 20; i++)
  {
    history[i] = malloc(1);
    history[i] = NULL;
  }
  history_file = malloc(100);
  sprintf(history_file, "%s/.history", shell_home);

  read_from_history();

  // initialising bg processes
  proc_no = 0;
  n_bg_procs = 0;
}

int check_prefix(char *str, char *pre)
{
  int flag = 1;

  int l_str = strlen(str);
  int l_pre = strlen(pre);

  if (l_pre > l_str)
  {
    return 0;
  }

  for (int i = 0; i < l_pre; i++)
  {
    if (str[i] != pre[i])
    {
      return 0;
    }
  }
  return flag;
}

void parse_string(char **str)
{
  int str_sz = strlen(*str);
  char *new_str = malloc((str_sz + 1) * sizeof(char));
  strcpy(new_str, "");

  char delim[] = "\t\n ";

  char *ptr = strtok(*str, delim);

  while (ptr != NULL)
  {
    strcat(new_str, ptr);
    ptr = strtok(NULL, delim);
    if (ptr == NULL)
    {
      break;
    }
    strcat(new_str, " ");
  }

  strcpy(*str, new_str);
  free(new_str);
  return;
}

void free_array(char ***args)
{
  int l = arg_len(*args);
  for (int i = 0; i < l; i++)
  {
    free((*args)[i]);
  }
  free(*args);
}

int arg_len(char **args)
{
  int count = 0;
  for (int i = 0; 1; i++)
  {
    if (args[i] == NULL)
    {
      return count;
    }
    count++;
  }
}

char **convert_to_array(char *str)
{
  char **arr = malloc(sizeof(char *));
  char delim[] = " ";

  char *ptr = strtok(str, delim);

  int w = 0;
  while (ptr != NULL)
  {
    arr = realloc(arr, (w + 2) * sizeof(char *));
    arr[w] = malloc(strlen(ptr) + 1);
    strcpy(arr[w], ptr);

    ptr = strtok(NULL, delim);
    if (ptr == NULL)
    {
      break;
    }
    w++;
  }
  arr[w] = malloc(1);
  arr[w] = NULL;

  return arr;
}

void free_queue(char ****q, int n)
{
  for (int job = 0; job < n; job++)
  {
    for (int w = 0; 1; w++)
    {
      if ((*q)[job][w] == NULL)
      {
        break;
      }
      free((*q)[job][w]);
    }
    free((*q)[job]);
  }
  free((*q));
}

void get_process_name(int pid, char **proc_name)
{
  char *stat_file = malloc(200);
  sprintf(stat_file, "/proc/%d/stat", pid);

  FILE *fptr;
  fptr = fopen(stat_file, "r");
  if (fptr == NULL)
  {
    perror("Error");
    return;
  }

  char *info = malloc(1000);
  strcpy(info, "");

  char c;
  c = fgetc(fptr);
  int i;
  for (i = 0; c != EOF; i++)
  {
    info[i] = c;
    c = fgetc(fptr);
  }
  info[i] = '\0';

  char **parsed_stats = convert_to_array(info);

  sprintf(*proc_name, "%s", parsed_stats[1]);

  // freeing up stuff
  for (int j = 0; 1; j++)
  {
    if (parsed_stats[j] == NULL)
      break;
    free(parsed_stats[j]);
  }
  free(parsed_stats);
  free(stat_file);
  free(info);
  fclose(fptr);
}

void add_to_array(char ***arr, char *s)
{
  int n = arg_len(*arr);

  *arr = realloc(*arr, (n + 2) * sizeof(char *));

  (*arr)[n] = realloc((*arr)[n], strlen(s) + 20);
  strcpy((*arr)[n], s);

  (*arr)[n + 1] = malloc(1);
  (*arr)[n + 1] = NULL;
}

void print_array(char **arr)
{
  int n = arg_len(arr);
  for (int i = 0; i < n; i++)
  {
    printf("%s\n", arr[i]);
  }
}

void parse_path(char *path, char **new_path)
{
  int l = strlen(path);
  int total_sz = strlen(path) + strlen(shell_home) + 20;
  *new_path = realloc(*new_path, total_sz);
  strcpy(*new_path, "");

  if (path[0] == '/')
  {
    strcpy(*new_path, path);
  }
  else if (path[0] == '~')
  {
    strcpy(*new_path, shell_home);
    if (l > 1)
    {
      strcat(*new_path, path + 1);
    }
  }
  else
  {
    sprintf(*new_path, "%s/%s", cust_cwd(0), path);
  }
}

void append_char(char **s, char c)
{
  int n = strlen(*s);
  *s = realloc(*s, n + 2);
  (*s)[n] = c;
  (*s)[n + 1] = '\0';
}
